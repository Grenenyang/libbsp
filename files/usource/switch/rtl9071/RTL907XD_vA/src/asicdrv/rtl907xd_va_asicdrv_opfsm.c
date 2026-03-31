/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2024, Realtek Semiconductor Corp. All rights reserved.
 *
 * This software is a confidential and proprietary property of Realtek
 * Semiconductor Corp. Disclosure, reproduction, redistribution, in
 * whole or in part, of this work and its derivatives without express
 * permission is prohibited.
 *
 * Realtek Semiconductor Corp. reserves the right to update, modify, or
 * discontinue this software at any time without notice. This software is
 * provided "as is" and any express or implied warranties, including, but
 * not limited to, the implied warranties of merchantability and fitness for
 * a particular purpose are disclaimed. In no event shall Realtek
 * Semiconductor Corp. be liable for any direct, indirect, incidental,
 * special, exemplary, or consequential damages (including, but not limited
 * to, procurement of substitute goods or services; loss of use, data, or
 * profits; or business interruption) however caused and on any theory of
 * liability, whether in contract, strict liability, or tort (including
 * negligence or otherwise) arising in any way out of the use of this software,
 * even if advised of the possibility of such damage.
 */

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xd_va_asicdrv_phy.h"
#include "rtl907xd_va_asicdrv_opfsm.h"
#include "rtl907xd_va_asicdrv_config.h"

const uint8 dva_g_userPort2T1phy[DVA_RTK_SYS_PORT_ID_MAX] = DVA_MAP_USERPORT_TO_T1PHY;

/**
 * @brief Covert user port mask <-> T1phy port mask. \n
 * @param[in]   srcPmap  Source port mask. (0 to 0x7E) \n
 *                       The port mask to be converted.
 * @param[in]   mode     Convert Mode. (N/A) \n
 *                       From user port mask to T1phy port mask or from T1phy port mask to user port mask.
 * @param[out]  *dstPmap Destination port mask (0 to 0x7E)\n
 *                       The port mask conversion output.
 * @retval RT_ERR_OK            Success.
 * @retval -RT_ERR_NULL_POINTER The input parameter *dstPmap is NULL.
 * @retval -RT_ERR_INPUT        The Input mode is invalid.
 */
RtkApiRet dva_drv_t1phy_pmap_convert(RtkPmap srcPmap, RtkPmap *dstPmap, dva_RtkT1PHYMapConvertMode_e mode)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPmap   tmpPmap = 0u;
    RtkPmap   usrPmap = DVA_RTK_USER_PMAP_WITHT1PHY;
    RtkPmap   t1Pmap  = DVA_RTK_T1PHY_PMAP_ALL;

    do
    {
        if(NULL == dstPmap)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(DVA_EM_USER_TO_T1PHY == mode)
        {
            if(DVA_RTK_USER_PMAP_WITHT1PHY < srcPmap)
            {
                ret = -RT_ERR_PORT_MASK;
                continue;
            }

            if(srcPmap != (srcPmap & usrPmap))
            {
                ret = -RT_ERR_INPUT;
                continue;
            }

            tmpPmap |= (((uint32)0x1u & (srcPmap >> DVA_RTK_USER_PORT_1ST)) << dva_g_userPort2T1phy[DVA_RTK_USER_PORT_1ST]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> DVA_RTK_USER_PORT_2ND)) << dva_g_userPort2T1phy[DVA_RTK_USER_PORT_2ND]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> DVA_RTK_USER_PORT_3RD)) << dva_g_userPort2T1phy[DVA_RTK_USER_PORT_3RD]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> DVA_RTK_USER_PORT_4TH)) << dva_g_userPort2T1phy[DVA_RTK_USER_PORT_4TH]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> DVA_RTK_USER_PORT_5TH)) << dva_g_userPort2T1phy[DVA_RTK_USER_PORT_5TH]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> DVA_RTK_USER_PORT_6TH)) << dva_g_userPort2T1phy[DVA_RTK_USER_PORT_6TH]);
        }
        else if(DVA_EM_T1PHY_TO_USER == mode)
        {
            if(DVA_RTK_T1PHY_PMAP_ALL < srcPmap)
            {
                ret = -RT_ERR_PORT_MASK;
                continue;
            }

            if(srcPmap != (srcPmap & t1Pmap))
            {
                ret = -RT_ERR_INPUT;
                continue;
            }

            tmpPmap |= (((srcPmap & DVA_RTK_T1PHY_PMAP_1ST) >> DVA_RTK_T1PHY_PORT_1ST) << DVA_RTK_USER_PORT_1ST);
            tmpPmap |= (((srcPmap & DVA_RTK_T1PHY_PMAP_2ND) >> DVA_RTK_T1PHY_PORT_2ND) << DVA_RTK_USER_PORT_2ND);
            tmpPmap |= (((srcPmap & DVA_RTK_T1PHY_PMAP_3RD) >> DVA_RTK_T1PHY_PORT_3RD) << DVA_RTK_USER_PORT_3RD);
            tmpPmap |= (((srcPmap & DVA_RTK_T1PHY_PMAP_4TH) >> DVA_RTK_T1PHY_PORT_4TH) << DVA_RTK_USER_PORT_4TH);
            tmpPmap |= (((srcPmap & DVA_RTK_T1PHY_PMAP_5TH) >> DVA_RTK_T1PHY_PORT_5TH) << DVA_RTK_USER_PORT_5TH);
            tmpPmap |= (((srcPmap & DVA_RTK_T1PHY_PMAP_6TH) >> DVA_RTK_T1PHY_PORT_6TH) << DVA_RTK_USER_PORT_6TH);
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        *dstPmap = tmpPmap;

    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set 1000M-T1 PHY's Sleep Capability Enable or Disable.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] regOffset     reg address offset (0 to 0xFFFFFFFF) \n
 *                          PHY sleep capability reg address offset.
 * @param[in] regOAMOffset  OAM reg address (0 to 0xFFFFFFFF) \n
 *                          PHY OAM capability reg address offset.
 * @param[in] sleepCapOn    The sleep capability setting. (DISABLE or ENABLE) \n
 *                          ENABLE  : Enable the sleep capability \n
 *                          DISABLE : Disable the sleep capability
 * @param[out] None
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to dva_reg_read() and dva_reg_write().
 */
RtkApiRet dva_drv_phy_1000mt1_phy_sleep_cap_set(uint32 unit, uint32 regOffset, uint32 regOAMOffset, RtkEnable sleepCapOn)
{
    uint32    regValue    = 0;
    uint32    regOAMValue = 0;
    RtkApiRet retVal      = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regOffset, &regValue));
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regOAMOffset, &regOAMValue));
        if((sleepCapOn != 0u) && ((regValue & (0x1u)) == (0x0u)))
        {
            /* turns 0 to 1 */
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, regOffset, regValue | (0x1u)));
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, regOAMOffset, regOAMValue | (0x2u)));
        }
        else if((sleepCapOn == 0u) && ((regValue & (0x1u)) == (0x1u)))
        {
            /* turns 1 to 0 */
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, regOffset, regValue & 0xFFFEu));
        }
        else
        {
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set 100M-T1 PHY's Sleep Capability Enable or Disable.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] regOffset     reg address offset (0 to 0xFFFFFFFF) \n
 *                          PHY sleep capability reg address offset.
 * @param[in] sleepCapOn    The sleep capability setting. (DISABLE or ENABLE) \n
 *                          ENABLE  : Enable the sleep capability \n
 *                          DISABLE : Disable the sleep capability
 * @param[out] None
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to dva_reg_read() and dva_reg_write().
 */
RtkApiRet dva_drv_phy_100mt1_phy_sleep_cap_set(uint32 unit, uint32 regOffset, RtkEnable sleepCapOn)
{
    uint32    regValue = 0;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regOffset, &regValue));

        if((sleepCapOn != 0u) && ((regValue & (0x1u)) == (0x0u)))
        {
            /* turns 0 to 1 */
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, regOffset, regValue | (0x1u)));
        }
        else if((sleepCapOn == 0u) && ((regValue & (0x1u)) == (0x1u)))
        {
            /* turns 1 to 0 */
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, regOffset, regValue & 0xFFFEu));
        }
        else
        {
            continue;
        }
    } while(0u == 1u);

    return retVal;
}
/**
 * @brief Set wake output forwarding pulse configuration of GPIOB1 and GPIOB2.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] type           The pulse type (0/1) \n
 *                           45us(0) or 500ms(1).
 * @param[out] None
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to reg_write().
 */
RtkApiRet dva_drv_op_wakeo_pulse_set(uint32 unit, uint32 type)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* 45us */
        if(0u == type)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, 0x4A0010B4, 0x2Du));
        }
        /* 500ms */
        else if(1u == type)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, 0x4A0010B4, 0x5F4u));
        }
        else
        {
            retVal = RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get wake output forwarding pulse configuration of GPIOB1 and GPIOB2.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] type           The pulse type (0/1) \n
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to reg_write().
 */
RtkApiRet dva_drv_op_wakeo_pulse_get(uint32 unit, uint32 *type)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;
    do
    {
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, 0x4A0010B4, &regVal));
        /* 45us */
        if(0x2Du == regVal)
        {
            *type = 0u;
        }
        /* 500ms */
        else if(0x5F4u == regVal)
        {
            *type = 1u;
        }
        else
        {
            retVal = RT_ERR_FAILED;
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Set wake output forwarding configuration of GPIOB1 and GPIOB2.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] configIdx      GPIO index (0/1) \n
 *                           0 is GPIOB1, 1 is GPIOB2.
 * @param[in] enablePortMask The configure port mask (0 to 0xFFFFFFFF) \n
 *                           The configured port mask. \n
 *                           - bit1: If the bit1 value is 1, port1 enabled wake-out forwarding on the selected GPIO
 *                           - bit2: If the bit2 value is 1, port2 enabled wake-out forwarding on the selected GPIO
 *                           - bit3: If the bit3 value is 1, port3 enabled wake-out forwarding on the selected GPIO
 *                           - bit4: If the bit4 value is 1, port4 enabled wake-out forwarding on the selected GPIO
 *                           - bit5: If the bit5 value is 1, port5 enabled wake-out forwarding on the selected GPIO
 *                           - bit6: If the bit6 value is 1, port6 enabled wake-out forwarding on the selected GPIO
 * @param[out] None
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to reg_write().
 */
RtkApiRet dva_drv_op_wakeo_portmsk_set(uint32 unit, uint32 configIdx, RtkPmap enablePortMask)
{
    RtkApiRet retVal = RT_ERR_OK;
    /* Convert to OP port index */
    RtkPmap cfgPmap = enablePortMask >> 1u;
    do
    {
        /* GPIOB1 */
        if(0u == configIdx)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, 0x4A0010BC, cfgPmap));
        }
        /* GPIOB2 */
        else if(1u == configIdx)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, 0x4A0010C0, cfgPmap));
        }
        else
        {
            retVal = RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get wake output forwarding configuration of GPIOB1 and GPIOB2.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] configIdx      GPIO index (0/1) \n
 *                           0 is GPIOB1, 1 is GPIOB2.
 * @param[out] enablePortMask The configure port mask (0 to 0xFFFFFFFF) \n
 *                            The configured port mask.
 *                            - bit1: Select port1
 *                            - bit2: Select port2
 *                            - bit3: Select port3
 *                            - bit4: Select port4
 *                            - bit5: Select port5
 *                            - bit6: Select port6
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to reg_read().
 */
RtkApiRet dva_drv_op_wakeo_portmsk_get(uint32 unit, uint32 configIdx, RtkPmap *enablePortMask)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   cfgPmap = 0x0;
    do
    {
        /* GPIOB1 */
        if(0u == configIdx)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, 0x4A0010BC, &cfgPmap));
        }
        /* GPIOB2 */
        else if(1u == configIdx)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, 0x4A0010C0, &cfgPmap));
        }
        else
        {
            retVal = RT_ERR_INPUT;
            continue;
        }
        /* Convert from OP port index to user port index */
        *enablePortMask = (cfgPmap << 1u);
    } while(0u == 1u);
    return retVal;
}
