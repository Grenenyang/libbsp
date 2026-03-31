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
#include "rtl907xc_va_api.h"
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtkas_debug.h"
#include "rtl907xc_va_asicdrv_phy.h"
#include "rtl907xc_va_asicdrv_config.h"
#include "rtl907xc_va_api_phy_test.h"
#include "rtl907xc_va_api_phy.h"
#include "rtkas_api_phy.h"

/**
 * @brief Check the valid port of phytest
 * @param[in] port Port index
 * @retval RT_ERR_OK       the check was completed.
 * @retval -RT_ERR_PORT_ID the port number was illegal.
 */
RtkApiRet cva_drv_valid_phytest_userport_check(uint8 port)
{
    RtkApiRet ret = RT_ERR_OK;

    if(((uint8)CVA_RTK_USER_PORT_1ST > port) || ((uint8)CVA_RTK_USER_PORT_6TH < port))
    {
        ret = -RT_ERR_PORT_ID;
    }

    return ret;
}

/**
 * @brief Translate user port to system port
 * @param[in] port User port index
 * @param[out] sysPort System port index
 * @retval RT_ERR_OK       the check and remapping were completed.
 * @retval -RT_ERR_PORT_ID the system port index was illegal.
 */
RtkApiRet cva_drv_userport_to_sysport(uint8 port, uint32 *sysPort)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == sysPort)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if((uint8)CVA_RTL9075_INTERFACE_NUM <= port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        *sysPort = (uint32)cva_g_userPort2sysPort[port];
        if(0xFFu == *sysPort)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Translate system port to ocp address
 * @param[in] sysPort system port index
 * @param[out] ocpAddr OCP address
 * @retval RT_ERR_OK       the check and remapping were completed.
 * @retval -RT_ERR_PORT_ID the system port index was illegal.
 */
RtkApiRet cva_drv_sysport_to_ocpaddr(uint32 sysPort, uint32 *ocpAddr)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == ocpAddr)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if((uint32)CVA_RTK_SYS_PORT_ID_MAX <= sysPort)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        *ocpAddr = (uint32)cva_g_sysPortToOcpAddr[sysPort];
        if(0u == *ocpAddr)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get RTCT result
 * @param[in] unit Stacking unit
 * @param[in] portNum Port number
 * @param[out] rtctStatus The cable status
 * @param[out] rtctDetect The cable open/short location
 * @retval RT_ERR_OK the RTCT result was obtained. \n
 * @retval Others    Please refer to cva_reg_write() and cva_reg_read() in rtl906x_reg_list.c.
 * @note This is only used for misra HIS_CCM check.
 */
RtkApiRet cva_drv_rtct_status_get(uint32 unit, uint32 portNum, uint32 *rtctStatus, uint32 *rtctDetect)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Get cable status */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (CVA_OCP_UCACC_ADDR | portNum) << 1u, 0x8022u));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (CVA_OCP_UCACC_DATA | portNum) << 1u, rtctStatus));

        /* Get cable defect in meter */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (CVA_OCP_UCACC_ADDR | portNum) << 1u, 0x8023u));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (CVA_OCP_UCACC_DATA | portNum) << 1u, rtctDetect));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Check the completion of PHY register access
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @retval RT_ERR_OK                PHY MII register access was completed.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT PHY MII register access was time out.
 * @retval Others                   Please refer to cva_reg_read() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_wait_op_finished(uint32 unit)
{
    uint32    regVal = 0u;
    uint16    cnt    = 0u;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        for(cnt = 0u; cnt < CVA_MAX_POLLCNT; ++cnt)
        {
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, CVA_PHY_REG_ACCESS_CONTROL0, &regVal));
            if((regVal & 0x01u) == 0u)
            {
                break;
            }
        }

        if(cnt >= CVA_MAX_POLLCNT)
        {
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Read PHY register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] phyAddr The PHY address
 * @param[in] page The page of PHY register
 * @param[in] regAddr The address of PHY register
 * @param[in] regData The pointer to PHY register data
 * @param[out] None
 * @retval RT_ERR_OK      Read operation of PHY MII register was completed.
 * @retval -RT_ERR_FAILED Read operation of PHY MII register was time out.
 * @retval Others         Please refer to cva_reg_read() and cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_phy_reg_read(uint32 unit, uint8 phyAddr, uint8 page,
                               uint8 regAddr, uint16 *regData)
{
    RtkApiRet ret     = (RtkApiRet)RT_ERR_OK;
    RtkApiRet retVal  = (RtkApiRet)RT_ERR_OK;
    uint32    regVal  = 0u;
    uint8     phyType = CVA_PHY_ACCESS;

    do
    {
        *regData = 0;
        ret      = cva_drv_wait_op_finished(unit);
        if(RT_ERR_OK != ret)
        {
            retVal = -(RtkApiRet)RT_ERR_FAILED;
            break;
        }
        regVal = ((uint32)phyAddr << 13u) | ((uint32)regAddr << 8u) | ((uint32)page << 3u)
                 | ((uint32)phyType << 2u) | (uint32)0x3u;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_PHY_REG_ACCESS_CONTROL0, regVal));
    } while(0u == 1u);

    if(-(RtkApiRet)RT_ERR_FAILED != retVal)
    {
        do
        {
            ret = cva_drv_wait_op_finished(unit);
            if(RT_ERR_OK != ret)
            {
                retVal = -(RtkApiRet)RT_ERR_FAILED;
                break;
            }

            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_PHY_REG_ACCESS_CONTROL0, &regVal));

            regVal   = (uint32)(regVal >> 16) & (uint32)0xffffu;
            *regData = (uint16)regVal;
        } while(0u == 1u);
    }

    return retVal;
}

/**
 * @brief Write PHY register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] phyAddr The PHY address
 * @param[in] page The page of PHY register
 * @param[in] regAddr The address of PHY register
 * @param[in] regData The data of PHY register
 * @param[out] None
 * @retval RT_ERR_OK      Write operation of PHY MII register was completed.
 * @retval -RT_ERR_FAILED Write operation of PHY MII register was time out.
 * @retval Others         Please refer to cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_phy_reg_write(uint32 unit, uint8 phyAddr, uint8 page,
                                uint8 regAddr, uint16 regData)
{
    RtkApiRet ret    = (RtkApiRet)RT_ERR_OK;
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;
    uint32    regVal;
    uint8     phyType = CVA_PHY_ACCESS;

    do
    {
        ret = cva_drv_wait_op_finished(unit);
        if(RT_ERR_OK != ret)
        {
            retVal = -(RtkApiRet)RT_ERR_FAILED;
            break;
        }

        regVal = ((uint32)regData << 16u) | ((uint32)phyAddr << 13u)
                 | ((uint32)regAddr << 8u) | ((uint32)page << 3u) | ((uint32)phyType << 2u) | (uint32)0x1u;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_PHY_REG_ACCESS_CONTROL0, regVal));
    } while(0u == 1u);

    ret = cva_drv_wait_op_finished(unit);
    if(RT_ERR_OK != ret)
    {
        retVal = -(RtkApiRet)RT_ERR_FAILED;
    }

    return retVal;
}

/**
 * @brief Read PHY MMD register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] phyAddr The PHY address
 * @param[in] deviceID The device ID of PHY
 * @param[in] regAddr The address of PHY MMD register
 * @param[in] regData The pointer to PHY MMD register data
 * @retval RT_ERR_OK      Write operation of PHY MMD register was completed.
 * @retval -RT_ERR_FAILED Write operation of PHY MMD register was time out.
 * @retval Others         Please refer to cva_reg_read() and cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_mmd_reg_read(uint32 unit, uint8 phyAddr, uint8 deviceID,
                               uint16 regAddr, uint16 *regData)
{
    RtkApiRet ret    = (RtkApiRet)RT_ERR_OK;
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;
    uint32    regVal;
    uint8     phyType = CVA_MMD_ACCESS;

    do
    {
        *regData = 0;
        ret      = cva_drv_wait_op_finished(unit);
        if(RT_ERR_OK != ret)
        {
            retVal = -(RtkApiRet)RT_ERR_FAILED;
            break;
        }

        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_PHY_REG_ACCESS_CONTROL1, regAddr));

        regVal = ((uint32)phyAddr << 13u) | ((uint32)deviceID << 8u)
                 | ((uint32)phyType << 2u) | ((uint32)0x1f << 3u) | (uint32)0x3u;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_PHY_REG_ACCESS_CONTROL0, regVal));
    } while(0u == 1u);

    if(-(RtkApiRet)RT_ERR_FAILED != retVal)
    {
        do
        {
            ret = cva_drv_wait_op_finished(unit);
            if(RT_ERR_OK != ret)
            {
                retVal = -(RtkApiRet)RT_ERR_FAILED;
                break;
            }

            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_PHY_REG_ACCESS_CONTROL0, &regVal));

            regVal   = (uint32)(regVal >> 16) & (uint32)0xffffu;
            *regData = (uint16)regVal;
        } while(0u == 1u);
    }

    return retVal;
}

/**
 * @brief Write PHY MMD register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] phyAddr The PHY address
 * @param[in] deviceID The device ID of PHY
 * @param[in] regAddr The address of PHY MMD register
 * @param[in] regData The data of PHY MMD register
 * @retval RT_ERR_OK      Write operation of PHY MMD register was completed.
 * @retval -RT_ERR_FAILED Write operation of PHY MMD register was time out.
 * @retval Others         Please refer to cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_mmd_reg_write(uint32 unit, uint8 phyAddr, uint8 deviceID,
                                uint16 regAddr, uint16 regData)
{
    RtkApiRet ret    = (RtkApiRet)RT_ERR_OK;
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;
    uint32    regVal;
    uint8     phyType = CVA_MMD_ACCESS;

    do
    {
        ret = cva_drv_wait_op_finished(unit);
        if(RT_ERR_OK != ret)
        {
            retVal = -(RtkApiRet)RT_ERR_FAILED;
            break;
        }

        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_PHY_REG_ACCESS_CONTROL1, regAddr));

        regVal = ((uint32)regData << 16u) | ((uint32)phyAddr << 13u)
                 | ((uint32)deviceID << 8u) | ((uint32)phyType << 2u) | ((uint32)0x1f << 3u) | (uint32)0x1u;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_PHY_REG_ACCESS_CONTROL0, regVal));
    } while(0u == 1u);

    ret = cva_drv_wait_op_finished(unit);
    if(RT_ERR_OK != ret)
    {
        retVal = -(RtkApiRet)RT_ERR_FAILED;
    }

    return retVal;
}

/**
 * @brief Read the external giga PHY register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] page The page of the external giga PHY register
 * @param[in] regAddr The register address
 * @param[in] regData The pointer to the register data
 * @param[out] None
 * @retval RT_ERR_OK The read operation was finished.
 * @retval Others    Please refer to cva_drv_phy_reg_read() and cva_drv_phy_reg_write() in rtl906x_asicdrv_phy.c.
 */
RtkApiRet cva_drv_giga_phy_ext_read(uint32 unit, uint8 page, uint8 regAddr,
                                    uint16 *regData)
{
    uint16    pageSel;
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    do
    {
        /* save old page sel */
        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_read(unit, PN_PORT4, 0, 31, &pageSel));

        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_write(unit, PN_PORT4, 0, 31, 7));
        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_write(unit, PN_PORT4, 31, 30, page));

        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_read(unit, PN_PORT4, 31, regAddr, regData));

        /* restore page sel */
        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_write(unit, PN_PORT4, 0, 31, pageSel));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Write the external giga PHY register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] page The page of the external giga PHY register
 * @param[in] regAddr The register address
 * @param[in] regData The register data
 * @param[out] None
 * @retval RT_ERR_OK The write operation was finished.
 * @retval Others    Please refer to cva_drv_phy_reg_read() and cva_drv_phy_reg_write() in rtl906x_asicdrv_phy.c.
 */
RtkApiRet cva_drv_giga_phy_ext_write(uint32 unit, uint8 page, uint8 regAddr,
                                     uint16 regData)
{
    uint16    pageSel;
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    do
    {
        /* save old page sel */
        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_read(unit, PN_PORT4, 0, 31, &pageSel));

        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_write(unit, PN_PORT4, 0, 31, 7));
        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_write(unit, PN_PORT4, 31, 30, page));

        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_write(unit, PN_PORT4, 31, regAddr, regData));

        /* restore page sel */
        CHK_FUN_CONTINUE(retVal, cva_drv_phy_reg_write(unit, PN_PORT4, 0, 31, pageSel));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get analog control registers
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] offset The offset for channel (before divide by 4)
 * @param[in] value The pointer to register value
 * @param[out] None
 * @retval -RT_ERR_FAILED           The operation was not completed.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT FSM was not in idle state.
 * @retval -RT_ERR_NULL_POINTER     The pointer to value was null.
 * @retval Others                   Please refer to cva_reg_field_read(), cva_reg_field_set(), cva_reg_read(), and cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_phy_channel_get(uint32 unit, uint32 offset, uint32 *value)
{
    uint32    regValue   = 0u;
    uint32    realOffset = 0u;
    uint16    waitCnt    = 0u;
    RtkApiRet retVal     = -RT_ERR_FAILED;

    do
    {
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_read(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                       CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_FSM_IDLE_LSP,
                                                       CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_FSM_IDLE_LEN,
                                                       &regValue)));
        if(1u != regValue)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        if(NULL == value)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        realOffset = (offset / 4u);
        regValue   = 0u;
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_set(CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ADDR_LSP,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ADDR_LEN,
                                                      realOffset, &regValue)));
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_set(CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_READ_NWRITE_LSP,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_READ_NWRITE_LEN,
                                                      1, &regValue)));
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_set(CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ACCES_TRIGGER_LSP,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ACCES_TRIGGER_LEN,
                                                      1U, &regValue)));
        CHK_FUN_CONTINUE((retVal), (cva_reg_write(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD, regValue)));
        for(waitCnt = 0u; waitCnt < 20u; waitCnt++)
        {
            CHK_FUN_CONTINUE((retVal), (cva_reg_read(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD, &regValue)));
            if((0u == (regValue & 0x01u)) && (0u != (regValue & 0x10u)))
            {
                retVal = cva_reg_read(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_RDATA, value);
                break;
            }
        }
        if(19u < waitCnt)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set Senter port register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] offset The register offset (before divide by 4)
 * @param[in] value The register value
 * @param[out] None
 * @retval -RT_ERR_FAILED           The operation was not completed.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT FSM was not in idle state.
 * @retval RT_ERR_OK                The operation was successful.
 * @retval Others                   Please refer to cva_reg_field_read(), cva_reg_field_set(), cva_reg_read(), and cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_phy_channel_set(uint32 unit, uint32 offset, uint32 value)
{
    uint32    regValue   = 0u;
    uint32    realOffset = 0u;
    uint16    waitCnt    = 0u;
    RtkApiRet retVal     = -RT_ERR_FAILED;

    do
    {
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_read(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                       CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_FSM_IDLE_LSP,
                                                       CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_FSM_IDLE_LEN,
                                                       &regValue)));
        if(1u != regValue)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        realOffset = (offset / 4u);
        regValue   = 0u;
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_set(CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ADDR_LSP,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ADDR_LEN,
                                                      realOffset, &regValue)));
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_set(CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_READ_NWRITE_LSP,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_READ_NWRITE_LEN,
                                                      0, &regValue)));
        CHK_FUN_CONTINUE((retVal), (cva_reg_write(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_WDATA, value)));
        CHK_FUN_CONTINUE((retVal), (cva_reg_field_set(CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ACCES_TRIGGER_LSP,
                                                      CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD_INT_PHY_TOP_CTL_INDIR_ACCES_TRIGGER_LEN,
                                                      1U, &regValue)));
        CHK_FUN_CONTINUE((retVal), (cva_reg_write(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD, regValue)));

        for(waitCnt = 0u; waitCnt < 20u; waitCnt++)
        {
            CHK_FUN_CONTINUE((retVal), (cva_reg_read(unit, CVA_INT_PHY_TOP_CTL_INDIR_ACCESS_CMD, &regValue)));
            if((0u == (regValue & 0x01u)) && (0u != (regValue & 0x10u)))
            {
                retVal = RT_ERR_OK;
                break;
            }
        }
        if(19u < waitCnt)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Check link state before getting SQI
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort The user port number (1 to 6)
 * @retval RT_ERR_OK     The port is linked up.
 * @retval RT_ERR_FAILED The port is linked down.
 */
RtkApiRet cva_drv_is_link_up_for_sqi(uint32 unit, uint8 userPort)
{
    RtkApiRet retVal            = RT_ERR_FAILED;
    uint32    regVal            = 0u;
    uint32    sysPort           = 0u;
    uint32    sysPort2SMIAddr[] = {CVA_PORT0_LINK_STATUS, CVA_PORT1_LINK_STATUS, CVA_PORT2_LINK_STATUS, CVA_PORT3_LINK_STATUS,
                                   CVA_PORT4_LINK_STATUS, CVA_PORT5_LINK_STATUS, CVA_PORT6_LINK_STATUS, CVA_PORT7_LINK_STATUS,
                                   CVA_PORT8_LINK_STATUS, CVA_PORT9_LINK_STATUS, CVA_PORT10_LINK_STATUS, CVA_PORT11_LINK_STATUS,
                                   CVA_PORT12_LINK_STATUS, CVA_PORT13_LINK_STATUS, CVA_PORT14_LINK_STATUS, CVA_PORT15_LINK_STATUS};

    do
    {
        /* Check if this port is a valid port for phytest */
        CHK_FUN_CONTINUE(retVal, cva_drv_valid_phytest_userport_check(userPort));
        /* Translate the user port index to the system port index */
        CHK_FUN_CONTINUE(retVal, cva_drv_userport_to_sysport(userPort, &sysPort));
        /* Read the port link state register twice */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, sysPort2SMIAddr[sysPort], &regVal));
        if(0u != (regVal & 0x01u))
        {
            retVal = RT_ERR_OK;
        }
        else
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Sub function of the SQI of 10/100-Base ports.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] phyAddr mapped phy address in register
 * @param[out] sqiPtr The pointer to the SQI data
 * @retval RT_ERR_OK The sub function of the SQI 100Base-T1 port was done successfully.
 * @retval Others    Please refer to cva_reg_read() in rtl906x_reg_list.c.
 */
static RtkApiRet cva_drv_b_phy_sqi_get_sub(uint32 unit, uint32 phyAddr, uint8 *sqiPtr)
{
    uint32    regAddr;
    uint32    regVal;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* check link up or not */
        regAddr = (phyAddr | CVA_SQI_100M_LINK_STA_REG) << 1u;
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        if((uint32)0x44u == (regVal & 0xFFu))
        {
            regAddr = (phyAddr | CVA_OCP_SQI_INDEX) << 1u;
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
            *sqiPtr = (uint8)((regVal >> 1) & 0x07u);
        }
        else
        {
            *sqiPtr = 0;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check the speed of the indexed port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] speed Speed of the port (0 to 1) \n
 *                  0: 10Base-T1S \n
 *                  1: 100Base-T1 \n
 * @retval RT_ERR_OK         The speed was correct.
 * @retval -RT_ERR_PHY_SPEED The speed was not correct.
 * @retval Others            Please refer to cva_reg_read() in rtl906x_reg_list.c.
 */
static RtkApiRet cva_drv_check_b_phy_speed(uint32 unit, uint32 port, uint8 speed)
{
    uint32    ocpAddr;
    uint32    regAddr;
    uint32    regVal;
    uint32    regSpd;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        ocpAddr = cva_g_sysPortToOcpAddr[port];
        regAddr = (ocpAddr | CVA_OCP_BMCR) << 1u;
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        regSpd = (((regVal >> CVA_PHY_SPD1_LSP) & 0x1u) << 1u) | ((regVal >> CVA_PHY_SPD0_LSP) & 0x1u);

        if(0u == speed)
        {
            if((uint32)PHY_BMCR_SPEED_10M != regSpd)
            {
                ret = -(RtkApiRet)RT_ERR_PHY_SPEED;
                continue;
            }
        }
        else
        {
            if((uint32)PHY_BMCR_SPEED_100M != regSpd)
            {
                ret = -(RtkApiRet)RT_ERR_PHY_SPEED;
                continue;
            }
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check the speed of the indexed port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] speed Speed of the port (0 to 1) \n
 *                  0: 100Base-T1 \n
 *                  1: 1000Base-T1 \n
 * @retval RT_ERR_OK         The speed was correct.
 * @retval -RT_ERR_PHY_SPEED The speed was not correct.
 * @retval Others            Please refer to cva_reg_read() in rtl906x_reg_list.c.
 */
static RtkApiRet cva_drv_check_r_phy_speed(uint32 unit, uint32 port, uint8 speed)
{
    uint32    ocpAddr;
    uint32    regAddr;
    uint32    regVal;
    uint32    regSpd;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        ocpAddr = cva_g_sysPortToOcpAddr[port];
        regAddr = (ocpAddr | CVA_OCP_BMCR) << 1u;
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        regSpd = (((regVal >> CVA_PHY_SPD1_LSP) & 0x1u) << 1u) | ((regVal >> CVA_PHY_SPD0_LSP) & 0x1u);

        if(0u == speed)
        {
            if((uint32)PHY_BMCR_SPEED_100M != regSpd)
            {
                ret = -(RtkApiRet)RT_ERR_PHY_SPEED;
                continue;
            }
        }
        else
        {
            if((uint32)PHY_BMCR_SPEED_1000M != regSpd)
            {
                ret = -(RtkApiRet)RT_ERR_PHY_SPEED;
                continue;
            }
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set PLCA node ID for 10BASE-T1S.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ocpAddr OCP register address
 * @retval RT_ERR_OK          The speed was correct.
 * @retval -RT_ERR_PHY_DUPLEX The wrong duplex setting.
 * @retval Others             Please refer to rtl906x_reg_list.c.
 */
static RtkApiRet cva_drv_b_phy_plca_node_id_set(uint32 unit, uint32 ocpAddr)
{
    uint32    regAddr;
    uint32    regVal;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Get the duplex status in BMCR register */
        regAddr = (ocpAddr | CVA_OCP_BMCR) << 1u;
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, regAddr, CVA_PHY_REGDUP_LSP, CVA_PHY_REGDUP_LEN, &regVal));
        /* Check if it is full duplex */
        if(1u != regVal)
        {
            ret = -(RtkApiRet)RT_ERR_PHY_DUPLEX;
            continue;
        }

        /* choose point-to-point mode */
        regAddr = (ocpAddr | CVA_SQI_10M_P2P_REG) << 1u;
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        regVal |= 0x000000FFu;
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the SQI of 10/100-Base ports.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] speed Speed of the port (0 to 1) \n
 *                  0: 10Base-T1S \n
 *                  1: 100Base-T1 \n
 * @param[out] pSqi The pointer to the SQI data
 * @retval RT_ERR_OK          The SQI of the 100Base-T1 port was obtained.
 * @retval -RT_ERR_PHY_DUPLEX The wrong duplex status.
 * @retval Others             Please refer to cva_reg_read() and cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_b_phy_sqi_get(uint32 unit, uint32 port, uint8 speed, uint8 *pSqi)
{
    uint32    regAddr;
    uint32    regVal;
    uint32    newPhyAddr    = 0u;
    uint16    sqiTimeOutCnt = 0u;
    RtkApiRet ret           = RT_ERR_OK;

    do
    {
        /* Check if the speed of PHY matches */
        CHK_FUN_CONTINUE(ret, cva_drv_check_b_phy_speed(unit, port, speed));

        newPhyAddr = cva_g_sysPortToOcpAddr[port];
        if(0u == speed)
        {
            /* Set the 10M PLCA node ID for CDR10 SQI Test */
            CHK_FUN_CONTINUE(ret, cva_drv_b_phy_plca_node_id_set(unit, newPhyAddr));

            /* Delay few microseconds */
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(RTK_TIMER_SQI_USECOUND));

            /* read SQI level */
            regAddr = (newPhyAddr | CVA_SQI_10M_INDEX_REG) << 1u;
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
            *pSqi = (uint8)(regVal & 0x07u);
        }
        else
        {
            /* check if local and remote receiver are ok */
            regAddr = (newPhyAddr | CVA_OCP_PHYSR) << 1u;
            do
            {
                sqiTimeOutCnt++;
                CHK_FUN_CONTINUE(ret, rtk_usecond_delay(RTK_TIMER_SQI_USECOUND));
                CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
            } while(((uint32)0x3000u != (regVal & 0x3000u)) && (sqiTimeOutCnt < RTK_TIMER_SQI_TIMEOUT_COUNT_NUM));

            if((RT_ERR_OK != ret) || (sqiTimeOutCnt >= RTK_TIMER_SQI_TIMEOUT_COUNT_NUM))
            {
                ret = -(RtkApiRet)RT_ERR_FAILED;
                continue;
            }

            /* check link up or not */
            CHK_FUN_CONTINUE(ret, cva_drv_b_phy_sqi_get_sub(unit, newPhyAddr, pSqi));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check if the link state is up or not.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] speed Speed of the port (0 to 1) \n
 *                  0: 100Base-T1 \n
 *                  1: 1000Base-T1 \n
 * @param[in] nPhyAddr The translation PHY address of the port
 * @param[out] pSqi The pointer to the SQI data
 * @retval RT_ERR_OK The link state was up and the SQI was obtained.
 * @retval Others    Please refer to cva_reg_read() in rtl906x_reg_list.c.
 */
static RtkApiRet cva_drv_r_phy_sqi_get_sub2(uint32 unit, uint8 speed, uint32 nPhyAddr, uint8 *pSqi)
{
    uint32    regAddr;
    uint32    regVal = 0u;
    uint32    cmpValue;
    RtkApiRet ret = RT_ERR_OK;

    if(0u == speed)
    {
        cmpValue = 0x44u;
    }
    else
    {
        cmpValue = 0x37u;
    }

    do
    {
        /* check link up or not */
        regAddr = (nPhyAddr | CVA_SQI_100M_LINK_STA_REG) << 1u;
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        if(cmpValue == (regVal & 0xFFu))
        {
            regAddr = (nPhyAddr | CVA_OCP_SQI_INDEX) << 1u;
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
            *pSqi = (uint8)((regVal >> 1) & 0x07u);
        }
        else
        {
            *pSqi = 0;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check if local and remote receiver are ready.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] speed Speed of the port (0 to 1) \n
 *                  0: 100Base-T1 \n
 *                  1: 1000Base-T1 \n
 * @param[in] nPhyAddr The translation PHY address of the port
 * @retval sqiTimeOutCnt The SQI time counter.
 */
static uint16 cva_drv_r_phy_sqi_get_sub1(uint32 unit, uint8 speed, uint32 nPhyAddr)
{
    uint32    regAddr;
    uint32    regVal        = 0u;
    uint16    sqiTimeOutCnt = 0u;
    RtkApiRet ret           = RT_ERR_OK;

    /* check if local and remote receiver are ok */
    if(0u == speed)
    {
        regAddr = (nPhyAddr | CVA_OCP_PHYSR4) << 1u;
        do
        {
            sqiTimeOutCnt++;
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(RTK_TIMER_SQI_USECOUND));
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        } while(((uint32)0x0404u != (regVal & 0x0404u)) && (sqiTimeOutCnt < RTK_TIMER_SQI_TIMEOUT_COUNT_NUM));
    }
    else
    {
        regAddr = (nPhyAddr | CVA_OCP_PHYSR) << 1u;
        do
        {
            sqiTimeOutCnt++;
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(RTK_TIMER_SQI_USECOUND));
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        } while(((uint32)0x3000u != (regVal & 0x3000u)) && (sqiTimeOutCnt < RTK_TIMER_SQI_TIMEOUT_COUNT_NUM));
    }

    if(RT_ERR_OK != ret)
    {
        sqiTimeOutCnt = RTK_TIMER_SQI_TIMEOUT_COUNT_NUM;
    }

    return sqiTimeOutCnt;
}

/**
 * @brief Get the SQI of 100/1000-Base ports.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] spped Speed of the port (0 to 1) \n
 *                  0: 100Base-T1 \n
 *                  1: 1000Base-T1 \n
 * @param[out] pSqi The pointer to the SQI data
 * @retval RT_ERR_OK The SQI of the 1000Base-T1 port was obtained.
 * @retval Others    Please refer to cva_reg_read() and cva_reg_write() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_r_phy_sqi_get(uint32 unit, uint32 port, uint8 speed, uint8 *pSqi)
{
    uint32    newPhyAddr    = 0u;
    uint16    sqiTimeOutCnt = 0u;
    RtkApiRet ret           = RT_ERR_OK;

    do
    {
        /* Check if the speed of PHY matches */
        CHK_FUN_CONTINUE(ret, cva_drv_check_r_phy_speed(unit, port, speed));

        /* Translate the system port to the OCP address */
        newPhyAddr = cva_g_sysPortToOcpAddr[port];

        /* check if local and remote receiver are ok */
        sqiTimeOutCnt = cva_drv_r_phy_sqi_get_sub1(unit, speed, newPhyAddr);

        if(sqiTimeOutCnt >= RTK_TIMER_SQI_TIMEOUT_COUNT_NUM)
        {
            ret = -(RtkApiRet)RT_ERR_FAILED;
            continue;
        }

        /* check link up or not */
        CHK_FUN_CONTINUE(ret, cva_drv_r_phy_sqi_get_sub2(unit, speed, newPhyAddr, pSqi));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set 10Base-T1 mode of the indexed port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The user port index
 * @param[out] None
 * @retval RT_ERR_OK       The port mode was set.
 * @retval -RT_ERR_PORT_ID The port was illegal.
 * @retval -RT_ERR_INPUT   The testmode was illegal.
 * @retval Others          Please refer to cva_reg_write() and cva_reg_read() in rtl906x_reg_list.c. \n
 *                         Please refer to cva_drv_phy_channel_set() and cva_drv_phy_channel_get() in rtl906x_asicdrv_phy.c.
 */
static RtkApiRet cva_drv_phy_10base_port_mode_set(uint32 unit, uint32 port)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(((uint32)CVA_RTK_USER_PORT_5TH == port) || ((uint32)CVA_RTK_USER_PORT_6TH == port))
        {
            /* only for (10/100)Base ports */
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        switch(port)
        {
            case CVA_RTK_USER_PORT_1ST:
                CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_0, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P2_INTF_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P2_INTF_CB_CFG_LEN, 0));
                CHK_FUN_CONTINUE(ret, cva_reg_write(unit, 0x74800, 0x0100));
                break;
            case CVA_RTK_USER_PORT_2ND:
                CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_0, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P3_INTF_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P3_INTF_CB_CFG_LEN, 0));
                CHK_FUN_CONTINUE(ret, cva_reg_write(unit, 0x94800, 0x0100));
                break;
            case CVA_RTK_USER_PORT_3RD:
                CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_0, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P4_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P4_T1GMRG_CB_CFG_LEN, 3));
                CHK_FUN_CONTINUE(ret, cva_reg_write(unit, 0xB4800, 0x0100));
                break;
            default:
                CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_0, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P5_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P5_T1GMRG_CB_CFG_LEN, 3));
                CHK_FUN_CONTINUE(ret, cva_reg_write(unit, 0xD4800, 0x0100));
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set 10Base-T1S testmode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] testMode Testmode
 * @param[out] None
 * @retval RT_ERR_OK       The testmode was set.
 * @retval -RT_ERR_PORT_ID The port was illegal.
 * @retval -RT_ERR_INPUT   The testmode was illegal.
 * @retval Others          Please refer to cva_reg_write() and cva_reg_read() in rtl906x_reg_list.c. \n
 *                         Please refer to cva_drv_phy_channel_set() and cva_drv_phy_channel_get() in rtl906x_asicdrv_phy.c.
 */
RtkApiRet cva_drv_phy_10base_testmode_set(uint32 unit, uint32 port, uint8 const testMode)
{
    uint8     userPort = 0u;
    uint32    regAddr;
    uint32    regVal;
    uint32    newPhyAddr = 0u;
    uint32    newMode    = 0u;
    uint32    sysPort    = 0u;
    RtkApiRet ret        = RT_ERR_OK;

    do
    {
        if(((uint32)CVA_RTK_USER_PORT_5TH == port) || ((uint32)CVA_RTK_USER_PORT_6TH == port))
        {
            /* only for (10/100)Base ports */
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Set the interface 10BASE-T1 */
        CHK_FUN_CONTINUE(ret, cva_drv_phy_10base_port_mode_set(unit, (cva_RtkPort)port));

        /* Translate the user port index to the sys port index */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(ret, cva_drv_userport_to_sysport(userPort, &sysPort));
        /* Translate the system port index to the OCP address */
        CHK_FUN_CONTINUE(ret, cva_drv_sysport_to_ocpaddr(sysPort, &newPhyAddr));
        newMode = ((uint32)testMode & 0x07u) << 13u;
        switch(testMode)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                /* set test mode */
                regAddr = (newPhyAddr | CVA_OCP_PHY_10TSTMOD) << 1u;
                CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
                regVal = (regVal & 0x1FFFu) | newMode;
                CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set TX_CLK 66MHz in 100Base-T1 test mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] prtSel
 * @param[out] None
 * @retval RT_ERR_OK       The testmode was set.
 * @retval Others          Please refer to cva_reg_write(), cva_reg_read() and cva_reg_field_set() in rtl906x_reg_list.c.
 * @note
 */
RtkApiRet cva_drv_phy_100tm_tx_clk66_set(uint32 unit, uint32 prtSel)
{
    uint32    regValue;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* set TX_CLK */
        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_REG_11, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_REG_11,
                                                 CVA_AFE_REG_11_CKR66M_RX_L_LSP,
                                                 CVA_AFE_REG_11_CKR66M_RX_L_LEN,
                                                 prtSel, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_REG_11, regValue)))

        /* set RTT to TX_CLK66 */
        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_REG_1, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_REG_1,
                                                 CVA_AFE_REG_1_REGEN_CK66M_L_LSP,
                                                 CVA_AFE_REG_1_REGEN_CK66M_L_LEN,
                                                 1u, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_REG_1, regValue)))

        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_REG_9, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_REG_9,
                                                 CVA_AFE_REG_9_REG9_L_LSP,
                                                 CVA_AFE_REG_9_REG9_L_LEN,
                                                 0u, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_REG_9, regValue)))

        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_CENTER_REG_2, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_CENTER_REG_2,
                                                 CVA_AFE_CENTER_REG_2_RTT_SEL_L_LSP,
                                                 CVA_AFE_CENTER_REG_2_RTT_SEL_L_LEN,
                                                 0x0Bu, &regValue)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_CENTER_REG_2, regValue)));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Clear 1000Base-T1 testmode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] phyAddr PHY address
 * @param[out] None
 * @retval RT_ERR_OK     The testmode was set.
 * @retval Others        Please refer to cva_reg_write() and cva_reg_read().
 */
static RtkApiRet cva_drv_clear_1000base_testmode(uint32 unit, uint32 phyAddr)
{
    RtkApiRet ret = RT_ERR_OK;
    uint32    regAddr;
    uint32    regVal;

    do
    {
        // clear 1000Base-T1 test mode
        regAddr = (phyAddr | CVA_OCP_PHYCR) << 1u;
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        regVal &= 0x1FFFu;
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Sub function of cva_drv_phy_100base_testmode_set
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] regAddr register address
 * @param[in] testMode test mode
 * @param[out] None
 * @retval RT_ERR_OK     The testmode was set.
 * @retval Others        Please refer to cva_reg_write() and cva_reg_read().
 */
static RtkApiRet cva_drv_phy_100base_testmode_set_sub(uint32 unit, uint32 regAddr, uint32 testMode)
{
    RtkApiRet ret = RT_ERR_OK;
    uint32    regVal;

    do
    {
        /* set test mode */
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        regVal = (regVal & 0x1FFFu) | testMode;
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set 100Base-T1 testmode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] testMode Testmode
 * @param[out] None
 * @retval RT_ERR_OK     The testmode was set.
 * @retval -RT_ERR_INPUT The testmode was illegal.
 * @retval Others        Please refer to cva_reg_write() and cva_reg_read() in rtl906x_reg_list.c.
 */
RtkApiRet cva_drv_phy_100base_testmode_set(uint32 unit, uint32 port, uint8 const testMode)
{
    uint8       userPort = 0u;
    uint32      regAddr;
    uint32      newPhyAddr      = 0u;
    uint32      newMode         = 0u;
    uint32      ocpAddr         = 0u;
    RtkPort     portSel         = 0u;
    RtkPort     sysPort         = 0u;
    RtkApiRet   ret             = RT_ERR_OK;
    const uint8 portSelTable[8] = {0xFFu, 0xFFu, 0x04u, 0x08u, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu};

    do
    {
        /* Translate the user port index to the sys port index */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(ret, cva_drv_userport_to_sysport(userPort, &sysPort));
        /* Translate the system port index to the OCP address */
        CHK_FUN_CONTINUE(ret, cva_drv_sysport_to_ocpaddr(sysPort, &newPhyAddr));
        newMode = ((uint32)testMode & 0x07u) << 13u;

        if(((uint32)CVA_RTK_USER_PORT_5TH == port) || ((uint32)CVA_RTK_USER_PORT_6TH == port))
        {
            /* for (100/1000)Base ports */
            ocpAddr = CVA_OCP_PHY_COMBO_100TSTMOD;
        }
        else
        {
            /* for (10/100)Base ports */
            ocpAddr = CVA_OCP_PHY_100TSTMOD;
        }

        // clear 1000Base-T1 test mode
        CHK_FUN_CONTINUE(ret, cva_drv_clear_1000base_testmode(unit, newPhyAddr));

        switch(testMode)
        {
            case 0:
            case 4:
                /* Check if in the test mode 0 : check if testMode is 0 */
                if(0u == testMode)
                {
                    /* Set the slave role */
                    CHK_FUN_CONTINUE(ret, cva_phy_phycr_role_set(unit, port, 0));
                }
                portSel = ((uint32)portSelTable[sysPort]) & 0x0Fu;
                CHK_FUN_CONTINUE(ret, cva_drv_phy_100tm_tx_clk66_set(unit, portSel));

                /* set test mode */
                regAddr = (newPhyAddr | ocpAddr) << 1u;
                CHK_FUN_CONTINUE(ret, cva_drv_phy_100base_testmode_set_sub(unit, regAddr, newMode));
                break;
            case 1:
            case 2:
            case 5:
                /* Check if in the test mode 2 : check if testMode is 2 */
                if(2u == testMode)
                {
                    /* Set the master role */
                    CHK_FUN_CONTINUE(ret, cva_phy_phycr_role_set(unit, port, 1u));
                }
                /* set test mode */
                regAddr = (newPhyAddr | ocpAddr) << 1u;
                CHK_FUN_CONTINUE(ret, cva_drv_phy_100base_testmode_set_sub(unit, regAddr, newMode));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set ATP/ATN to TX_CLK125.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sysPort
 * @param[in] prtSel
 * @param[in] isMaster
 *            0: slave
 *            0: master
 * @param[out] None
 * @retval RT_ERR_OK       The testmode was set.
 * @retval Others          Please refer to cva_reg_write(), cva_reg_read() and cva_reg_field_set() in rtl906x_reg_list.c.
 * @note This is only used for misra HIS_CCM check.
 */
RtkApiRet cva_drv_phy_1000tm_tx_clk125_set(uint32 unit, uint32 sysPort, uint32 prtSel, uint8 isMaster)
{
    uint32    regVal;
    uint8     testSel;
    uint8     jtTest;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* set TX_CLK_125M */
        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_REG_11, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_REG_11,
                                                 CVA_AFE_REG_11_CKR125M_RX_L_LSP,
                                                 CVA_AFE_REG_11_CKR125M_RX_L_LEN,
                                                 prtSel, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_REG_11, regVal)));

        /* select cktest source */
        if((uint32)PN_PORT6 == sysPort)
        {
            testSel = 0u;
        }
        else
        {
            testSel = 1u;
        }
        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_REG_7, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_REG_7,
                                                 CVA_AFE_REG_7_REG_CKTEST_SEL_LSP,
                                                 CVA_AFE_REG_7_REG_CKTEST_SEL_LEN,
                                                 testSel, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_REG_7, regVal)));

        /* set ATP/ATN to TX_CLK125 */
        if(0u == isMaster)
        {
            jtTest = 0x19u;
        }
        else
        {
            jtTest = 0x01u;
        }
        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_REG_6, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_REG_6,
                                                 CVA_AFE_REG_6_REG_125M_ATP_ATN_LSP,
                                                 CVA_AFE_REG_6_REG_125M_ATP_ATN_LEN,
                                                 jtTest, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_REG_6, regVal)));

        CHK_FUN_CONTINUE(ret, (cva_reg_read(unit, CVA_AFE_REG_9, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_field_set(CVA_AFE_REG_9,
                                                 CVA_AFE_REG_9_REG_EN_TEST_OUT_L_LSP,
                                                 CVA_AFE_REG_9_REG_EN_TEST_OUT_L_LEN,
                                                 1u, &regVal)));
        CHK_FUN_CONTINUE(ret, (cva_reg_write(unit, CVA_AFE_REG_9, regVal)));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set 1000Base-T1 testmode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] testMode Testmode
 * @param[in] isMaster
 *            0: slave
 *            0: master
 * @param[out] None
 * @retval RT_ERR_OK       The testmode was set.
 * @retval -RT_ERR_PORT_ID The port was illegal.
 * @retval -RT_ERR_INPUT   The testmode was illegal.
 * @retval Others          Please refer to cva_reg_write() and cva_reg_read() in rtl906x_reg_list.c. \n
 *                         Please refer to cva_drv_phy_channel_set() and cva_drv_phy_channel_get() in rtl906x_asicdrv_phy.c.
 */
RtkApiRet cva_drv_phy_1000base_testmode_set(uint32 unit, uint32 port, uint8 const testMode, uint8 isMaster)
{
    uint8       userPort = 0u;
    uint8       master   = 0u;
    uint32      regAddr;
    uint32      regVal;
    uint32      newPhyAddr      = 0u;
    uint32      newMode         = 0u;
    RtkPort     portSel         = 0u;
    RtkPort     sysPort         = 0u;
    RtkApiRet   ret             = RT_ERR_OK;
    const uint8 portSelTable[8] = {0xFFu, 0xFFu, 0x04u, 0x08u, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu};

    do
    {
        if((uint32)CVA_RTK_USER_PORT_5TH > port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Translate the user port index to the sys port index */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(ret, cva_drv_userport_to_sysport(userPort, &sysPort));
        /* Translate the system port index to the OCP address */
        CHK_FUN_CONTINUE(ret, cva_drv_sysport_to_ocpaddr(sysPort, &newPhyAddr));
        newMode = ((uint32)testMode & 0x07u) << 13u;
        switch(testMode)
        {
            case 0:
            case 2:
            case 5:
            case 6:
            case 7:
                if(7u == testMode)
                {
                    newMode = 0u;
                }
                /* set test mode */
                regAddr = (newPhyAddr | CVA_OCP_PHYCR) << 1u;
                CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
                regVal = (regVal & 0x1FFFu) | newMode;
                CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));
                break;
            case 1:
            case 4:
                if(4u == testMode)
                {
                    master = 1u;
                }
                else
                {
                    master = isMaster;
                }
                /* set TX_CLK125 and (master/slave) */
                portSel = ((uint32)portSelTable[sysPort]) & 0x0Fu;
                CHK_FUN_CONTINUE(ret, cva_drv_phy_1000tm_tx_clk125_set(unit, sysPort, portSel, master));

                /* Set the role of master or slave */
                CHK_FUN_CONTINUE(ret, cva_phy_phycr_role_set(unit, port, master));

                /* set test mode */
                regAddr = (newPhyAddr | CVA_OCP_PHYCR) << 1u;
                CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
                regVal = (regVal & 0x1FFFu) | newMode;
                CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief The API is used to set the PHY enter patch mode when a patch code is required.
 *
 * @param[in] unit Stacking unit
 * @param[in] port          port number
 *                          The port number is the offset of PHY OCP address.
 * @param[in] mode          Operation mode
 *                          This mode is used to indicator whether configuration over ethernet or not.
 * @param[in] lastPacket    Last packet flag
 *                          This flash is used to idnetify whether this packet is last or not for ethernect access mode.
 * @param[out] None
 * @retval RT_ERR_OK                The function is executed correctly
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT When polling the phy status, it is timeout.
 * @retval Others                   Please refer to cva_reg_read() and cva_reg_write().
 */
RtkApiRet cva_drv_phy_patch_request(uint32 unit, uint32 port, uint8 mode, uint8 lastPacket)
{
    uint32    data       = 0u;
    RtkApiRet ret        = RT_ERR_OK;
    uint8     ethernetEn = 0u;
    uint16    timeout    = 0u;
    uint32    address[5] = {0u};
    uint32    val[5]     = {0u};

    ethernetEn = (mode & 0x40u) >> 6;

    do
    {
        /* Patch request & wait patch_rdy = 1 */
        if(0u == ethernetEn)
        {
            address[0] = ((port << 16) | 0xA436u) * 2u;
            val[0]     = 0xB820u;
            address[1] = ((port << 16) | 0xA438u) * 2u;
            val[1]     = 0x10u;
            address[2] = ((port << 16) | 0xA436u) * 2u;
            val[2]     = 0xB830u;
            address[3] = ((port << 16) | 0xA438u) * 2u;
            val[3]     = 0x8000u;
            address[4] = ((port << 16) | 0xA436u) * 2u;
            val[4]     = 0xB800u;
            CHK_FUN_CONTINUE(ret, cva_reg_write_array(unit, address, val, 5u));

            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, ((port << 16) | 0xA438u) * 2u, &data));

            data = (data & 0x40u) >> 6;

            /* polling at least 5ms */
            while((0u == data) && (timeout < (CVA_TIMEOUT_5MS / CVA_TICK_100US)))
            {
                CHK_FUN_CONTINUE(ret, cva_reg_read(unit, ((port << 16) | 0xA438u) * 2u, &data));
                data = (data & 0x40u) >> 6;
                CHK_FUN_CONTINUE(ret, g_fp_udelay(CVA_TICK_100US));
                timeout++;
            }

            if(timeout >= (CVA_TIMEOUT_5MS / CVA_TICK_100US))
            {
                ret = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
        }
        else
        {
            if(0u == lastPacket)
            {
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((port << 16) | 0xA436u) * 2u, 0xB820u, lastPacket));
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((port << 16) | 0xA438u) * 2u, 0x10u, lastPacket));
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((port << 16) | 0xA436u) * 2u, 0xB830u, lastPacket));
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((port << 16) | 0xA438u) * 2u, 0x8000u, lastPacket));
            }
            else
            {
                /* Last packet dont care cmd,addr,data */
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(0u, 0u, 0u, lastPacket));
            }
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief The API is used to release the PHY from patch mode.
 *
 * @param[in] unit Stacking unit
 * @param[in] port          port number
 *                          The port number is the offset of PHY OCP address.
 * @param[in] mode          Operation mode
 *                          This mode is used to indicator whether configuration over ethernet or not.
 * @param[in] lastPacket    Last packet flag
 *                          This flash is used to idnetify whether this packet is last or not for ethernect access mode.
 * @param[out] None
 * @retval RT_ERR_OK                The function is executed correctly
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT When polling the phy status, it is timeout.
 * @retval Others                   Please refer to cva_reg_read() and cva_reg_write().
 */
RtkApiRet cva_drv_phy_patch_release(uint32 unit, uint32 port, uint8 mode, uint8 lastPacket)
{
    uint32    data;
    RtkApiRet ret = RT_ERR_OK;
    uint8     ethernetEn;
    uint16    timeout = 0u;

    ethernetEn = (mode & 0x40u) >> 6;

    do
    {
        /* Release patch request & wait patch_rls = 0 */
        if(0u == ethernetEn)
        {
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, ((port << 16) | 0xA436u) * 2u, 0xB820u));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, ((port << 16) | 0xA438u) * 2u, 0x0u));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, ((port << 16) | 0xA436u) * 2u, 0xB800u));
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, ((port << 16) | 0xA438u) * 2u, &data));
            data = (data & 0x40u) >> 6;

            /* polling at least 5ms */
            while((data != 0u) && (timeout < (CVA_TIMEOUT_5MS / CVA_TICK_100US)))
            {
                CHK_FUN_CONTINUE(ret, cva_reg_read(unit, ((port << 16) | 0xA438u) * 2u, &data));
                data = (data & 0x40u) >> 6;
                CHK_FUN_CONTINUE(ret, g_fp_udelay(CVA_TICK_100US));
                timeout++;
            }

            if(timeout >= (CVA_TIMEOUT_5MS / CVA_TICK_100US))
            {
                ret = -RT_ERR_BUSYWAIT_TIMEOUT;
                break;
            }
        }
        else
        {
            if(0u == lastPacket)
            {
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((port << 16) | 0xA436u) * 2u, 0xB820u, lastPacket));
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((port << 16) | 0xA438u) * 2u, 0x0u, lastPacket));
            }
            else
            {
                /* Last packet dont care cmd,addr,data */
                CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(0u, 0u, 0u, lastPacket));
            }
        }
    } while(0u == 1u);

    return ret;
}
