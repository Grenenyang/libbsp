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

/******************************************************************************/
/* Includes                                                                    */
/******************************************************************************/
#include "rtl907xd_va_api_someip.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"

/**
 * @defgroup RTK_MODULE_SOMEIP SOME/IP
 * @brief SOME/IP firewall APIs
 * @{
 */

/**
 * @addtogroup SOMEIP_SET_L4DPORT Set SOME/IP firewall L4 destination port
 * @{
 */

/**
 * @defgroup SOMEIP_SET_L4DPORT_ASIC Asic Driver
 * @brief Set SOME/IP firewall L4 destination port asic driver
 * @{
 */

/**
 * @brief Set SOME/IP firewall L4 destination port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] idx           Index of the L4 destination port(1 to SOMEIP_L4DPORT_NUM) \n
 *                          Index used for the L4 destination port.
 * @param[in] l4dport       L4 destination port. \n
 *                          L4 destination port.
 * @param[out] None
 * @retval RT_ERR_OK        The L4 destination port has been set in the register.
 * @retval -RT_ERR_INPUT    The l4dport is invalid.
 * @retval Others           Please refer to dva_drv_indirect_reg_field_set() and dva_drv_indirect_reg_field_get().
 */
RtkApiRet dva_someip_set_l4dport(uint32 unit, uint8 idx, uint16 l4dport)
{
    RtkApiRet ret          = RT_ERR_OK;
    uint8     portIdx      = 0u;
    uint32    l4dportAddr  = DVA_PORT1_SOMEIP_RECOGNIZING_L4PORT0;
    uint32    tmpAddr      = 0u;
    uint32    existL4dport = 0u;
    uint8     i            = 0u;

    do
    {
        if((idx > SOMEIP_L4DPORT_NUM) || (idx == 0u))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        tmpAddr = l4dportAddr;
        for(i = 1u; i <= SOMEIP_L4DPORT_NUM; i++)
        {
            if((i % 2u) != 0u)
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, tmpAddr, 0u, 16u, &existL4dport));
            }
            else
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, tmpAddr, 16u, 16u, &existL4dport));
                tmpAddr += 0x4u;
            }

            if(existL4dport == (uint32)l4dport)
            {
                ret = -RT_ERR_INPUT;
                break;
            }
        }

        if(RT_ERR_OK == ret)
        {
            if((idx % 2u) != 0u)
            {
                tmpAddr = (((uint32)idx / 2u) * 4u) + l4dportAddr;
                for(portIdx = DVA_RTK_SYS_PORT_1ST; portIdx <= DVA_RTK_SYS_PORT_END; portIdx++)
                {
                    CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, tmpAddr, 0u, 16u, l4dport));
                    tmpAddr += 0x800u;
                }
            }
            else
            {
                tmpAddr = ((((uint32)idx - 1u) / 2u) * 4u) + l4dportAddr;
                for(portIdx = DVA_RTK_SYS_PORT_1ST; portIdx <= DVA_RTK_SYS_PORT_END; portIdx++)
                {
                    CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, tmpAddr, 16u, 16u, l4dport));
                    tmpAddr += 0x800u;
                }
            }
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*SOMEIP_SET_L4DPORT_ASIC*/
/**@}*/ /*SOMEIP_SET_L4DPORT*/

/**
 * @addtogroup SOMEIP_GET_L4DPORT Get SOME/IP firewall L4 destination port
 * @{
 */

/**
 * @defgroup SOMEIP_GET_L4DPORT_ASIC Asic Driver
 * @brief Get the SOME/IP firewall L4 destination port asic driver
 * @{
 */

/**
 * @brief Get the SOME/IP firewall L4 destination port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] l4dport      L4 destination port. \n
 *                          The pointer to the L4 destination port.
 * @retval RT_ERR_OK        All L4 destination ports have been read from the registers.
 * @retval Others           Please refer to dva_drv_indirect_reg_field_get().
 */
RtkApiRet dva_someip_get_l4dport(uint32 unit, uint16 *l4dport)
{
    RtkApiRet ret                            = RT_ERR_OK;
    uint8     idx                            = 0u;
    uint32    l4dportAddr                    = DVA_PORT1_SOMEIP_RECOGNIZING_L4PORT0;
    uint32    tmpAddr                        = 0u;
    uint32    tmpL4dport[SOMEIP_L4DPORT_NUM] = {0u};

    do
    {
        tmpAddr = l4dportAddr;
        for(idx = 0u; idx < SOMEIP_L4DPORT_NUM; idx++)
        {
            if(((idx + 1u) % 2u) != 0u)
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, tmpAddr, 0u, 16u, &tmpL4dport[idx]));
                l4dport[idx] = (uint16)tmpL4dport[idx];
            }
            else
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, tmpAddr, 16u, 16u, &tmpL4dport[idx]));
                l4dport[idx] = (uint16)tmpL4dport[idx];
                tmpAddr += 0x4u;
            }
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*SOMEIP_GET_L4DPORT_ASIC*/
/**@}*/ /*SOMEIP_GET_L4DPORT*/

/**
 * @addtogroup SOMEIP_SET_HW_WHITELIST Set SOME/IP firewall HW whitelist
 * @{
 */

/**
 * @defgroup SOMEIP_SET_HW_WHITELIST_ASIC Asic Driver
 * @brief Set SOME/IP firewall HW whitelist asic driver
 * @{
 */

/**
 * @brief Set the SOME/IP firewall HW whitelist.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] idx           Index of the HW whitelist(1 to SOMEIP_HW_WHITELIST_NUM) \n
 *                          Index used for the HW whitelist.
 * @param[in] whitelist     HW whitelist. \n
 *                          HW whitelist.
 * @param[out] None
 * @retval RT_ERR_OK        The HW whitelist has been set in the register.
 * @retval -RT_ERR_INPUT    The HW whitelist is invalid.
 * @retval Others           Please refer to dva_drv_indirect_reg_field_set() and dva_drv_indirect_reg_field_get().
 */
RtkApiRet dva_someip_set_hw_whitelist(uint32 unit, uint8 idx, uint32 whitelist)
{
    RtkApiRet ret            = RT_ERR_OK;
    uint8     i              = 0u;
    uint8     portIdx        = 0u;
    uint32    whitelistAddr  = DVA_PORT1_SOMEIP_DOIP_WHITELIST0;
    uint32    tmpAddr        = 0u;
    uint32    existWhitelist = 0u;

    do
    {
        if((idx > SOMEIP_HW_WHITELIST_NUM) || (idx == 0u))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        tmpAddr = whitelistAddr;
        for(i = 0u; i < SOMEIP_HW_WHITELIST_NUM; i++)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, tmpAddr, 0u, 32u, &existWhitelist));
            if(existWhitelist == whitelist)
            {
                ret = -RT_ERR_INPUT;
                break;
            }
            tmpAddr += 0x4u;
        }

        if(ret == RT_ERR_OK)
        {
            tmpAddr = (((uint32)idx - 1u) * 4u) + whitelistAddr;
            for(portIdx = DVA_RTK_SYS_PORT_1ST; portIdx <= DVA_RTK_SYS_PORT_END; portIdx++)
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, tmpAddr, 0u, 32u, whitelist));
                tmpAddr += 0x800u;
            }
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*SOMEIP_SET_HW_WHITELIST_ASIC*/
/**@}*/ /*SOMEIP_SET_HW_WHITELIST*/

/**
 * @addtogroup SOMEIP_GET_HW_WHITELIST Get SOME/IP firewall HW whitelist
 * @{
 */

/**
 * @defgroup SOMEIP_GET_HW_WHITELIST_ASIC Asic Driver
 * @brief Get SOME/IP firewall HW whitelist asic driver
 * @{
 */

/**
 * @brief Get the SOME/IP HW whitelist.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] whitelist    HW whitelist. \n
 *                          The pointer to the HW whitelist.
 * @retval RT_ERR_OK        The SPI parameters have been read from the registers.
 * @retval Others           Please refer to dva_drv_indirect_reg_field_get().
 */
RtkApiRet dva_someip_get_hw_whitelist(uint32 unit, uint32 *whitelist)
{
    RtkApiRet ret           = RT_ERR_OK;
    uint8     idx           = 0u;
    uint32    whitelistAddr = DVA_PORT1_SOMEIP_DOIP_WHITELIST0;
    uint32    tmpAddr       = 0u;

    do
    {
        tmpAddr = whitelistAddr;
        for(idx = 0u; idx < SOMEIP_HW_WHITELIST_NUM; idx++)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, tmpAddr, 0u, 32u, &whitelist[idx]));
            tmpAddr += 0x4u;
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*SOMEIP_GET_HW_WHITELIST_ASIC*/
/**@}*/ /*SOMEIP_GET_HW_WHITELIST*/
/**@}*/ /* SOMEIP */
