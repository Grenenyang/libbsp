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

#include "rtkas_types.h"
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_asicdrv_tsn_common.h"
#include "rtl907xc_va_asicdrv_st.h"
#include "rtkas_system_porting.h"
#include "rtl907xc_va_asicdrv_ptp.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
static uint32 cva_g_stAdminIndex[CVA_TSN_NUMBER_OF_PORT];
static uint32 cva_g_stOperIndex[CVA_TSN_NUMBER_OF_PORT];

static RtkApiRet cva_drv_st_ad_ctrl_list_intf_chk(cva_RtkPort portId, uint32 numberOfEntry);
static RtkApiRet cva_drv_st_bc_sgmii_10m_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_sgmii_100m_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_sgmii_1000m_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_mii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rmii_mii_10_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rmii_mii_100_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_sgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_hisgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_usxgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rgmii_1000_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rgmii_100_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rgmii_10_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_100baset1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_10baset1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rtpg_1000t1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_bc_rtpg_100t1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);
static RtkApiRet cva_drv_st_generate_consecutive_gate_open_time_table(const StGateControlListParam_t      gateControlList[CVA_ST_SUPPORT_LIST_MAX],
                                                                      uint32                              numberOfEntry,
                                                                      cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u]);
static RtkApiRet cva_drv_st_sgot_table_special_rules_handler(const StGateControlListParam_t      gateControlList[CVA_ST_SUPPORT_LIST_MAX],
                                                             uint32                              numberOfEntry,
                                                             cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u]);
/**
 * @brief Check if the QBV configuration is complete.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully check the QBV configuration. \n
 * @retval -RT_ERR_ST_WAIT_PRECONFIG          QBV configuration is not complete. \n
 * @retval Others                             Please refer to cva_drv_indirect_sram_get().
 */
RtkApiRet cva_drv_st_isdone_chk(uint32 unit)
{
    RtkApiRet result  = RT_ERR_OK;
    uint32    regAddr = 0;
    uint32    isDone  = 0;

    do
    {
        /* Get isDone flag in firmware */
        regAddr = (uint32)CVA_ST_CONFIG_DONE_ADDRESS;
        CHK_FUN_CONTINUE(result, cva_drv_indirect_sram_get(unit, regAddr, &isDone));
        /* Check if isDone is 0 */
        if(0u == (isDone & 0x1u))
        {
            result = -RT_ERR_ST_WAIT_ISDONE;
            continue;
        }
    } while(0u == 1u);
    return result;
}
/**
 * @brief To check if Qbv preconfig is ready.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully check the qbv preconfig \n
 * @retval -RT_ERR_ST_WAIT_PRECONFIG          Qbv preconfig is not ready \n
 * @retval Others                             Please refer to cva_drv_indirect_sram_get() and cva_drv_indirect_reg_field_get().
 */
RtkApiRet cva_drv_st_preconfig_chk(uint32 unit)
{
    RtkApiRet result   = RT_ERR_OK;
    uint32    regAddr  = 0;
    uint32    isDone   = 0;
    uint32    isPtpRdy = 0;

    do
    {
        /* Get isDone flag in firmware */
        regAddr = (uint32)CVA_ST_CONFIG_DONE_ADDRESS;
        CHK_FUN_CONTINUE(result, cva_drv_indirect_sram_get(unit, regAddr, &isDone));
        /* Get isPtpRdy flag in firmware */
        regAddr = (uint32)CVA_PTP_ISREADY_ADDRESS;
        CHK_FUN_CONTINUE(result, cva_drv_indirect_sram_get(unit, regAddr, &isPtpRdy));
        /* Check if isDone is 0 or isptpReady is 0 */
        if((0u == (isDone & 0x1u)) || (0u == (isPtpRdy & 0x1u)))
        {
            result = -RT_ERR_ST_WAIT_PRECONFIG;
            continue;
        }
    } while(0u == 1u);
    return result;
}

RtkApiRet cva_drv_st_gt_conf_set_intf_chk(cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension,
                                          const StGateControlListParam_t gateControlList[], uint32 controlListLength)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_USER_PORT_1ST > portId) || (CVA_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(baseTime->nanosecond >= (uint32)CVA_TSN_1B_NANOSECOND)
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        if((controlListLength > (uint32)CVA_ST_SUPPORT_LIST_MAX) || (controlListLength < CVA_ST_SUPPORT_LIST_MIN))
        {
            result = -RT_ERR_ST_INVALID_LENGTH;
            continue;
        }

        /* configure transmission gate */
        if(NULL == gateControlList)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Check API cva_st_admin_gate_config_get interface
 * @param[in] portId              Switch port (CVA_RTK_SYS_PORT_1ST to CVA_RTK_SYS_PORT_11TH)  \n
 *                                Idegress port id.
 * @param[out] baseTime           Base time  \n
 *                                Admin base time as defined in standard
 * @param[out] cycleTime          Cycle time (2 to 2^32)  \n
 *                                Admin cycle time as defined in standard
 * @param[out] cycleTimeExtension Cycle time extension (2 to 2^32)  \n
 *                                Admin cycle time extension as defined in standard
 * @param[out] gateControlList    Gate control list
 *                                Admin gate control list
 * @param[out] controlListLength  Control list length (2 to 128)  \n
 *                                Admin gate control list length
 * @retval  RT_ERR_OK             The function is done.
 * @retval -RT_ERR_PORT_ID        The port id was out of range.
 * @retval -RT_ERR_NULL_POINTER   Input is NULL pointer.
 * @retval -RT_ERR_OUT_OF_RANGE   Input is out of range or invalid.
 */
RtkApiRet cva_drv_st_gt_conf_get_intf_chk(cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, RtkTsnCycleTime_t *cycleTime,
                                          uint32 *cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 *controlListLength)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_USER_PORT_1ST > portId) || (CVA_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(baseTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(cycleTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(cycleTimeExtension == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(gateControlList == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(controlListLength == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Check API byte_count interface range
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] portType port-type (100BASE-T1, 10BAST-T1, Fast Ethernet, MII, RMII, RGMII, SGMII, HISGMII, USXGMII)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_ST_MACSEC_CONFIG
 * @retval -RT_ERR_ST_PORT_TYPE
 */
RtkApiRet cva_drv_st_byte_count_intf_chk(uint32 unit, cva_RtkPort portId, uint32 portType, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_USER_PORT_1ST > portId) || (CVA_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if((CVA_NO_MACSEC != macsecIPG) && (CVA_MACSEC_IPG24 != macsecIPG) && (CVA_MACSEC_IPG32 != macsecIPG))
        {
            result = -RT_ERR_ST_MACSEC_CONFIG;
            continue;
        }

        if(CVA_ST_PORT_TYPE_END < portType)
        {
            result = -RT_ERR_ST_PORT_TYPE;
            continue;
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the byte count value of xmii interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] portType port-type (MII, RMII, RGMII, SGMII, HISGMII, USXGMII)
 * @param[in] speed port speed port speed (CVA_RTK_PORT_SPEED_10M to CVA_RTK_PORT_SPEED_5000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_ST_MACSEC_CONFIG
 * @retval -RT_ERR_ST_PORT_TYPE
 * @retval -RT_ERR_ST_PORT_SPEED
 */
RtkApiRet cva_drv_st_byte_count_xmii_set(uint32 unit, cva_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {

        CHK_FUN_CONTINUE(result, cva_drv_st_byte_count_intf_chk(unit, portId, portType, macsecIPG));

        switch(portType)
        {
            case CVA_ST_PORT_TYPE_MII:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_mii_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_RMII:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_rmii_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_RGMII:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_SGMII:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_sgmii_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_HISGMII:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_hisgmii_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_USXGMII:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_usxgmii_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            default:
                result = -RT_ERR_ST_PORT_TYPE;
                break;
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the byte count value of T1 interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] portType port-type (1000BASE-T1, 100BASE-T1, 10BAST-T1)
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M to CVA_RTK_PORT_SPEED_1000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_ST_MACSEC_CONFIG
 * @retval -RT_ERR_ST_PORT_TYPE
 * @retval -RT_ERR_ST_PORT_SPEED
 */
RtkApiRet cva_drv_st_byte_count_t1_set(uint32 unit, cva_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(result, cva_drv_st_byte_count_intf_chk(unit, portId, portType, macsecIPG));

        switch(portType)
        {
            case CVA_ST_PORT_TYPE_RTPG_1000BASE_T1:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_rtpg_1000t1_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_RTPG_100BASE_T1:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_rtpg_100t1_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_100BASE_T1:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_100baset1_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            case CVA_ST_PORT_TYPE_10BASE_T1:
                CHK_FUN_CONTINUE(result, cva_drv_st_bc_10baset1_set(unit, cva_g_userPort2sysPort[portId], speed, macsecIPG));
                break;
            default:
                result = -RT_ERR_ST_PORT_TYPE;
                break;
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the byte count value of 100 Base T1 interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_100M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_100baset1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_PORT_SPEED_10M != speed) && (CVA_RTK_PORT_SPEED_100M != speed))
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }

        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_BRPHY_1GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_BRPHY_1GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_23));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_BRPHY_1GMAC_100M));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the byte count value of 10 Base T1 interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_10baset1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_10M != speed)
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }

        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_BRPHY_1GMAC_10M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_BRPHY_1GMAC_10M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_11));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_BRPHY_1GMAC_10M));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_fe_100m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_FE_1GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_FE_1GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_13));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_FE_1GMAC_100M));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_fe_10m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_FE_1GMAC_10M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_FE_1GMAC_10M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_FE_1GMAC_10M));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the byte count value of fast ethernet interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M to CVA_RTK_PORT_SPEED_100M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
RtkApiRet cva_drv_st_byte_count_fe_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_100M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_fe_100m_set(unit, cva_g_userPort2sysPort[portId], macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_10M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_fe_10m_set(unit, cva_g_userPort2sysPort[portId], macsecIPG));
        }
        else
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_5g_rmii_mii_10_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_10M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_10M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_5GMAC_10M));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_1g_rmii_mii_10_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_10M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_10M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_1GMAC_10M));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rmii_mii_10_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_SYS_PORT_10TH == portId)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_5g_rmii_mii_10_set(unit, portId, macsecIPG));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_1g_rmii_mii_10_set(unit, portId, macsecIPG));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_5g_rmii_mii_100_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_5GMAC_100M));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_1g_rmii_mii_100_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_13));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_1GMAC_100M));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rmii_mii_100_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_SYS_PORT_10TH == portId)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_5g_rmii_mii_100_set(unit, portId, macsecIPG));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_1g_rmii_mii_100_set(unit, portId, macsecIPG));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the byte count value of mii interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M to CVA_RTK_PORT_SPEED_100M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_mii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_100M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rmii_mii_100_set(unit, portId, speed, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_10M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rmii_mii_10_set(unit, portId, speed, macsecIPG));
        }
        else
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the byte count value of rmii interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M to CVA_RTK_PORT_SPEED_100M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_rmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_100M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rmii_mii_100_set(unit, portId, speed, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_10M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rmii_mii_10_set(unit, portId, speed, macsecIPG));
        }
        else
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rgmii_5g_10m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_10M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_10M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_5GMAC_10M));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rgmii_1g_10m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_10M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_10M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_1GMAC_10M));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_rgmii_10_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_10M != speed)
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }

        if(CVA_RTK_SYS_PORT_10TH == portId)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_5g_10m_set(unit, portId, macsecIPG));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_1g_10m_set(unit, portId, macsecIPG));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rgmii_5g_100m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_5GMAC_100M));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rgmii_1g_100m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_13));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_1GMAC_100M));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_100M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_rgmii_100_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_100M != speed)
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }

        if(CVA_RTK_SYS_PORT_10TH == portId)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_5g_100m_set(unit, portId, macsecIPG));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_1g_100m_set(unit, portId, macsecIPG));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rgmii_5g_1000m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_30));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_1000M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_38));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_1000M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_13));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_5GMAC_1000M));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_rgmii_1g_1000m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_41));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_1000M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_41));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_1000M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_41));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_MII_1GMAC_1000M));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_1000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_rgmii_1000_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_1000M != speed)
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }

        if(CVA_RTK_SYS_PORT_10TH == portId)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_5g_1000m_set(unit, portId, macsecIPG));
        }
        else
        {
            /* for CVA_RTK_SYS_PORT_4TH and CVA_RTK_SYS_PORT_7TH*/
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_1g_1000m_set(unit, portId, macsecIPG));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M to CVA_RTK_PORT_SPEED_1000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_rgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {

        if(CVA_RTK_PORT_SPEED_1000M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_1000_set(unit, portId, speed, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_100M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_100_set(unit, portId, speed, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_10M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_rgmii_10_set(unit, portId, speed, macsecIPG));
        }
        else
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_100M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_rtpg_100t1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_38));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_RTPG_1GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_46));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_RTPG_1GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_13));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_RTPG_1GMAC_100M));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_1000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_rtpg_1000t1_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {

        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_92));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_RTPG_1GMAC_1000M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_92));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_RTPG_1GMAC_1000M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_92));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_RTPG_1GMAC_1000M));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_sgmii_wrap_10m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*wrap 1G MAC: system P8*/
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_SGMII_1GMAC_10M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_SGMII_1GMAC_10M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_SGMII_1GMAC_10M));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_sgmii_10m_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*wtg 5G MAC: system P10,11,12*/
        if((CVA_RTK_SYS_PORT_9TH == portId) || (CVA_RTK_SYS_PORT_10TH == portId) || (CVA_RTK_SYS_PORT_11TH == portId))
        {
            if(CVA_MACSEC_IPG24 == macsecIPG)
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_SGMI_5GMAC_10M));
            }
            else if(CVA_MACSEC_IPG32 == macsecIPG)
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_SGMI_5GMAC_10M));
            }
            else
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_SGMI_5GMAC_10M));
            }
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_sgmii_wrap_10m_set(unit, portId, macsecIPG));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_sgmii_wrap_100m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*wrap 1G MAC: system P8*/
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_SGMII_1GMAC_100M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_SGMII_1GMAC_100M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_13));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_SGMII_1GMAC_100M));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_100M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_sgmii_100m_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*wtg 5G MAC: system P10,11,12*/
        if((CVA_RTK_SYS_PORT_9TH == portId) || (CVA_RTK_SYS_PORT_10TH == portId) || (CVA_RTK_SYS_PORT_11TH == portId))
        {
            if(CVA_MACSEC_IPG24 == macsecIPG)
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_37));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_SGMI_5GMAC_100M));
            }
            else if(CVA_MACSEC_IPG32 == macsecIPG)
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_45));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_SGMI_5GMAC_100M));
            }
            else
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_10));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_SGMI_5GMAC_100M));
            }
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_sgmii_wrap_100m_set(unit, portId, macsecIPG));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_sgmii_wrap_1000m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*wrap 1G MAC: system P8*/
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_42));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_SGMII_1GMAC_1000M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_42));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_SGMII_1GMAC_1000M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_42));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_SGMII_1GMAC_1000M));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_1000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_sgmii_1000m_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*wtg 5G MAC: system P10,11,12*/
        if((CVA_RTK_SYS_PORT_9TH == portId) || (CVA_RTK_SYS_PORT_10TH == portId) || (CVA_RTK_SYS_PORT_11TH == portId))
        {
            if(CVA_MACSEC_IPG24 == macsecIPG)
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_31));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_SGMI_5GMAC_1000M));
            }
            else if(CVA_MACSEC_IPG32 == macsecIPG)
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_39));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_SGMI_5GMAC_1000M));
            }
            else
            {
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_14));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_SGMI_5GMAC_1000M));
            }
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_sgmii_wrap_1000m_set(unit, portId, macsecIPG));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_10M to CVA_RTK_PORT_SPEED_1000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_sgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_1000M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_sgmii_1000m_set(unit, portId, speed, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_100M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_sgmii_100m_set(unit, portId, speed, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_10M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_sgmii_10m_set(unit, portId, speed, macsecIPG));
        }
        else
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_2500M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_hisgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_PORT_SPEED_2500M != speed))
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }

        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_19));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_HSG_5GMAC));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_22));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_HSG_5GMAC));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_19));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_HSG_5GMAC));
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_usxgmii_5000m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_MACSEC_IPG24 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_59));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_USXG_5GMAC_5000M));
        }
        else if(CVA_MACSEC_IPG32 == macsecIPG)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_59));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_USXG_5GMAC_5000M));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_59));
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_USXG_5GMAC_5000M));
        }

    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_usxgmii_2500m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*for HIS-Level*/
        switch(macsecIPG)
        {
            case CVA_MACSEC_IPG24:
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_39));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_USXG_5GMAC_2500M));
                break;
            case CVA_MACSEC_IPG32:
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_39));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_USXG_5GMAC_2500M));
                break;
            default:
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_39));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_USXG_5GMAC_2500M));
                break;
        }
    } while(0u == 1u);

    return result;
}

static RtkApiRet cva_drv_st_bc_usxgmii_1000m_set(uint32 unit, cva_RtkPort portId, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /*for HIS-Level*/
        switch(macsecIPG)
        {
            case CVA_MACSEC_IPG24:
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_30));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG24_USXG_5GMAC_1000M));
                break;
            case CVA_MACSEC_IPG32:
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_38));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_WITH_IPG32_USXG_5GMAC_1000M));
                break;
            default:
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_BC0 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LSP, CVA_PORT0_QBV_BC0_QBV_HEAD_BC_ADJ_LEN, CVA_BYTE_PRE_FRAME_13));
                CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL27 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LSP, CVA_PORT0_QBV_CONTROL27_QBV_CYCLE_DELAY_LEN, CVA_CYCLE_DELAY_USXG_5GMAC_1000M));
                break;
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId egress port id.
 * @param[in] speed port speed (CVA_RTK_PORT_SPEED_2500M to CVA_RTK_PORT_SPEED_5000M)
 * @param[in] macsecIPG (CVA_NO_MACSEC to CVA_MACSEC_IPG32)
 * @param[out] None
 * @retval
 */
static RtkApiRet cva_drv_st_bc_usxgmii_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(CVA_RTK_PORT_SPEED_5000M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_usxgmii_5000m_set(unit, portId, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_2500M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_usxgmii_2500m_set(unit, portId, macsecIPG));
        }
        else if(CVA_RTK_PORT_SPEED_1000M == speed)
        {
            CHK_FUN_CONTINUE(result, cva_drv_st_bc_usxgmii_1000m_set(unit, portId, macsecIPG));
        }
        else
        {
            result = -RT_ERR_ST_PORT_SPEED;
            continue;
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @retval
 */
static RtkApiRet cva_drv_st_update_admin_oper_index(uint32 unit, cva_RtkPort portId)
{
    RtkApiRet result;
    uint32    registerValue;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        /* check the status */
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit,
                                                    CVA_PORT0_QBV_CONTROL9 + (PERPORT_REG_OFFSET * portId),
                                                    CVA_PORT0_QBV_CONTROL9_CFGCHG_SUCCESS_LSP,
                                                    CVA_PORT0_QBV_CONTROL9_CFGCHG_SUCCESS_LEN,
                                                    &registerValue));
        /* instance 0 is not operational */
        if(0u == (registerValue & (uint32)0x1u))
        {
            cva_g_stAdminIndex[portId] = 0u;
            cva_g_stOperIndex[portId]  = 1u;
        }
        /* instance 0 is operational */
        else
        {
            cva_g_stAdminIndex[portId] = 1u;
            cva_g_stOperIndex[portId]  = 0u;
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @retval
 */
static RtkApiRet cva_drv_st_check_pending(uint32 unit, cva_RtkPort portId)
{
    RtkApiRet result;
    uint32    configPending;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit,
                                                    CVA_PORT0_QBV_CONTROL21 + (PERPORT_REG_OFFSET * portId),
                                                    CVA_PORT0_QBV_CONTROL21_CONFIGPENDING_LSP,
                                                    CVA_PORT0_QBV_CONTROL21_CONFIGPENDING_LEN,
                                                    &configPending));

        if(configPending == 0u)
        {
            result = RT_ERR_OK;
        }
        else
        {
            result = -RT_ERR_ST_WAIT_PENDING;
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 *
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @retval
 */
static RtkApiRet cva_drv_st_check_control_list_compatibility(const StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    iteratorList;
    do
    {
        /* HW constraint */
        if(numberOfEntry < 2u)
        {
            result = -RT_ERR_ST_INVALID_LENGTH;
            continue;
        }

        /* To notify user about the incompatible concern, return error directly. */
        for(iteratorList = 0; iteratorList < numberOfEntry; ++iteratorList)
        {
            if(result != RT_ERR_OK)
            {
                continue;
            }

            if((gateControlList[iteratorList].operation != (uint32)CVA_ST_GATE_OPERATION_SET_GATE_STATE))
            {
                /* currently only support 0x00 set gate state */
                result = -RT_ERR_ST_INVALID_OPERAION;
                continue;
            }

            if(gateControlList[iteratorList].timeInterval < (uint32)CVA_ST_MIN_GATE_INTERVAL)
            {
                /* digital circuit could not support the operation of 0 tick. */
                /* the hardware limits the min value of interval. */
                result = -RT_ERR_ST_INVALID_TIME_INTERVAL;
                continue;
            }

            if((gateControlList[iteratorList].timeInterval % (uint32)CVA_ST_TICK_IN_NANOSECOND) != 0u)
            {
                /* the time interval should be tick_in_nanosecond aligned. e.g. 8ns for 125MHz system clock. */
                result = -RT_ERR_ST_NONALIGNED_TIME_INTERVAL;
                continue;
            }
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 *
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @param[in] consecutiveList
 * @retval
 */
static RtkApiRet cva_drv_st_sgot_table_special_rules_handler(const StGateControlListParam_t      gateControlList[CVA_ST_SUPPORT_LIST_MAX],
                                                             uint32                              numberOfEntry,
                                                             cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u])
{
    RtkApiRet result = RT_ERR_OK;
    uint32    iteratorList;
    uint32    iteratorQueue;
    uint32    processingGateState;
    uint32    hammingGatestate;

    hammingGatestate = 0xFF;
    for(iteratorList = 0; iteratorList < numberOfEntry; ++iteratorList)
    {
        /* if gatestate is always open, the corresponding hamming bit is 1 */
        hammingGatestate = hammingGatestate & gateControlList[iteratorList].gateState;
    }
    for(iteratorQueue = 0; iteratorQueue < (uint32)CVA_TSN_NUMBER_OF_QUEUE; ++iteratorQueue)
    {
        processingGateState = (hammingGatestate >> iteratorQueue) & 0x01u;
        if(processingGateState == 0x1u)
        {
            consecutiveList[iteratorQueue][0].timeInterval   = CVA_GCL_TABLE_MAX_TIME_INTERVAL;
            consecutiveList[iteratorQueue][0].forcedInterval = 1u;
        }
    }

    return result;
}

/**
 * @brief
 *
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @param[in] consecutiveList
 * @retval
 */
static RtkApiRet cva_drv_st_generate_consecutive_gate_open_time_table(const StGateControlListParam_t      gateControlList[CVA_ST_SUPPORT_LIST_MAX],
                                                                      uint32                              numberOfEntry,
                                                                      cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u])
{
    RtkApiRet result = RT_ERR_OK;
    uint32    iteratorList, iteratorQueue, lastEntryIndex;
    uint32    processingGateState;
    uint32    firstEntryGateState;
    uint32    consecutiveOpenTime[CVA_TSN_NUMBER_OF_QUEUE];
    uint32    consecutiveListIndex[CVA_TSN_NUMBER_OF_QUEUE];

    /* initialize entire table to 0 for ease of checking */
    for(iteratorQueue = 0; iteratorQueue < (uint32)CVA_TSN_NUMBER_OF_QUEUE; ++iteratorQueue)
    {
        for(iteratorList = 0; iteratorList < ((uint32)CVA_ST_SUPPORT_LIST_MAX / 2u); ++iteratorList)
        {
            consecutiveList[iteratorQueue][iteratorList].forcedInterval = 0u;
            consecutiveList[iteratorQueue][iteratorList].eor            = 0u;
            consecutiveList[iteratorQueue][iteratorList].boundary       = 0u;
            consecutiveList[iteratorQueue][iteratorList].timeInterval   = 0u;
        }
    }

    lastEntryIndex = numberOfEntry - 1u;
    for(iteratorQueue = 0; iteratorQueue < (uint32)CVA_TSN_NUMBER_OF_QUEUE; ++iteratorQueue)
    {
        consecutiveOpenTime[iteratorQueue]  = 0u;
        consecutiveListIndex[iteratorQueue] = 0u;
    }

    /* for all entry in table in one port */
    for(iteratorList = 0; iteratorList < numberOfEntry; ++iteratorList)
    {
        /* for all queue */
        for(iteratorQueue = 0; iteratorQueue < (uint32)CVA_TSN_NUMBER_OF_QUEUE; ++iteratorQueue)
        {
            /* gate state for specific queue */
            processingGateState = (((uint32)gateControlList[iteratorList].gateState >> iteratorQueue) & (uint32)0x01u);
            if(processingGateState == 0x01u)
            {
                /* cumulate time when gate is open */
                consecutiveOpenTime[iteratorQueue] = consecutiveOpenTime[iteratorQueue] + gateControlList[iteratorList].timeInterval;

                /* finalize the consecutive list if last entry. */
                if(iteratorList == lastEntryIndex)
                {
                    /* insert an entry */
                    consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].timeInterval = consecutiveOpenTime[iteratorQueue];

                    /* set eor and boundary bit */
                    firstEntryGateState = (((uint32)gateControlList[0].gateState >> iteratorQueue) & (uint32)0x01u);
                    if(firstEntryGateState == 0x01u)
                    {
                        /* This is last entry for consecutiveList. */
                        /* The corresponding gate state is open in first entry of gate contrl list table */
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].eor      = 0u;
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].boundary = 1u;
                    }
                    else
                    {
                        /* This is last entry for consecutiveList. */
                        /* The corresponding gate state is close in first entry of gate contrl list table */
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].eor      = 1u;
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].boundary = 1u;
                    }

                    /* make it consistent point to empty entry if consecutiveListIndex[iteratorQueue] is propagated and used as entry_length */
                    consecutiveListIndex[iteratorQueue] = consecutiveListIndex[iteratorQueue] + 1u;
                }
            }
            else
            {
                /* insert an entry when gate was open before.  (To avoid insert 0 when close -> close) */
                if(consecutiveOpenTime[iteratorQueue] > 0u)
                {
                    /* encounter a close event, record previous consecutive open time to list. */
                    consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].timeInterval = consecutiveOpenTime[iteratorQueue];
                    consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].eor          = 0u;
                    consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].boundary     = 0u;

                    consecutiveListIndex[iteratorQueue] = consecutiveListIndex[iteratorQueue] + 1u;
                    consecutiveOpenTime[iteratorQueue]  = 0u;
                }

                /* finalize the consecutive list if last entry. */
                if(iteratorList == lastEntryIndex)
                {
                    if(consecutiveListIndex[iteratorQueue] != 0u)
                    {
                        /* the index is already shift when close event [open -> close]. the last valid entry is idx-1. */
                        /* This is last entry for consecutiveList. */
                        /* The current gate state is close, don't care gate state of first entry. */
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue] - 1u].eor      = 1u;
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue] - 1u].boundary = 0u;
                    }
                    else
                    {
                        /* reach here only when there is no gate open entry. */
                        /* does this works? */
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].timeInterval = 0u;
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].eor          = 1u;
                        consecutiveList[iteratorQueue][consecutiveListIndex[iteratorQueue]].boundary     = 0u;
                    }
                }
            }
        }
    }

    /* special rules for always open */
    result = cva_drv_st_sgot_table_special_rules_handler(gateControlList, numberOfEntry, consecutiveList);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @retval
 */
static RtkApiRet cva_drv_st_set_estimated_integer_multiple_cycle_time(uint32 unit, cva_RtkPort portId)
{
    RtkApiRet                  result = RT_ERR_OK;
    RtkTsnPtpTime_t            baseTime;
    RtkTsnPtpTime_t            currentTime;
    cva_RtkTsnLongNanosecond_t estimatedLongNs;
    RtkTsnCycleTime_t          cycleTime;
    uint32                     cycleTimeNs;

    do
    {
        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        /* collect necessary information */
        CHK_FUN_CONTINUE(result, cva_drv_st_admin_base_time_get(unit, portId, &baseTime));

        CHK_FUN_CONTINUE(result, cva_drv_st_admin_cycle_time_get(unit, portId, &cycleTime));
        cycleTimeNs = cycleTime.numerator;

        CHK_FUN_CONTINUE(result, cva_drv_st_current_time_get(unit, &currentTime));

        cva_drv_tsn_multicycle_est(baseTime, currentTime, cycleTimeNs, &estimatedLongNs);

        /* write the estimated time to register as reference for hardware */
        if(0u == cva_g_stAdminIndex[portId])
        { /* admin is instance 0, configure istance 0 */
            CHK_FUN_CONTINUE(result, cva_reg_write(unit, (CVA_PORT0_QBV_CONTROL4 + (PERPORT_REG_OFFSET * portId)), estimatedLongNs.section[0]));

            CHK_FUN_CONTINUE(result, cva_reg_write(unit, (CVA_PORT0_QBV_CONTROL5 + (PERPORT_REG_OFFSET * portId)), estimatedLongNs.section[1]));

            CHK_FUN_CONTINUE(result, cva_reg_write(unit, (CVA_PORT0_QBV_CONTROL6 + (PERPORT_REG_OFFSET * portId)), estimatedLongNs.section[2]));
        }
        else
        { /* admin is instance 1, configure istance 1 */
            CHK_FUN_CONTINUE(result, cva_reg_write(unit, (CVA_PORT0_QBV_CONTROL13 + (PERPORT_REG_OFFSET * portId)), estimatedLongNs.section[0]));

            CHK_FUN_CONTINUE(result, cva_reg_write(unit, (CVA_PORT0_QBV_CONTROL14 + (PERPORT_REG_OFFSET * portId)), estimatedLongNs.section[1]));

            CHK_FUN_CONTINUE(result, cva_reg_write(unit, (CVA_PORT0_QBV_CONTROL15 + (PERPORT_REG_OFFSET * portId)), estimatedLongNs.section[2]));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] queueId
 * @param[in] queueMaxSDU
 * @retval
 */
RtkApiRet cva_drv_st_queue_max_sdu_get(uint32 unit, cva_RtkPort portId, uint32 queueId, uint32 *queueMaxSDU)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;
    uint32    offset;
    uint32    registerLsp;
    uint32    registerLen;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(queueId >= (uint32)CVA_TSN_NUMBER_OF_QUEUE)
        {
            result = -RT_ERR_QUEUE_ID;
            continue;
        }

        if(queueMaxSDU == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* 2 parameter in 1 register. */
        offset          = queueId >> 1u;
        registerAddress = CVA_PORT_QBV_CONTROL0 + (CVA_PERQUEUE_MAX_SDU_OFFSET * portId) + (offset * NEXT_REG_OFFSET);

        /* position is only 0 or 16 */
        registerLsp = (uint32)CVA_PORT_QBV_CONTROL0_Q0_MAX_SDU_P0_LSP + ((queueId % 2u) * CVA_PORT_QBV_CONTROL0_Q1_MAX_SDU_P0_LSP);

        /* length is only 14 */
        registerLen = CVA_PORT_QBV_CONTROL0_Q0_MAX_SDU_P0_LEN;

        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress, registerLsp, registerLen, queueMaxSDU));
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] queueId
 * @param[in] queueMaxSDU
 * @retval
 */
RtkApiRet cva_drv_st_queue_max_sdu_set(uint32 unit, cva_RtkPort portId, uint32 queueId, uint32 queueMaxSDU)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;
    uint32    offset;
    uint32    registerLsp;
    uint32    registerLen;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(queueId >= (uint32)CVA_TSN_NUMBER_OF_QUEUE)
        {
            result = -RT_ERR_QUEUE_ID;
            continue;
        }

        if(queueMaxSDU > 0x3FFFu)
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* 2 parameter in 1 register. */
        offset          = queueId >> 1;
        registerAddress = CVA_PORT_QBV_CONTROL0 + (CVA_PERQUEUE_MAX_SDU_OFFSET * portId) + (offset * NEXT_REG_OFFSET);

        /* position is only 0 or 16 */
        registerLsp = (uint32)CVA_PORT_QBV_CONTROL0_Q0_MAX_SDU_P0_LSP + ((queueId % 2u) * CVA_PORT_QBV_CONTROL0_Q1_MAX_SDU_P0_LSP);

        /* length is only 14 */
        registerLen = CVA_PORT_QBV_CONTROL0_Q0_MAX_SDU_P0_LEN;

        CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, registerAddress, registerLsp, registerLen, queueMaxSDU));
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateEnabled
 * @retval
 */
RtkApiRet cva_drv_st_gate_hw_enabled_get(uint32 unit, cva_RtkPort portId, uint32 *gateEnabled)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(gateEnabled == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, CVA_PORT0_QBV_CONTROL0 + (PERPORT_REG_OFFSET * portId),
                                                    CVA_PORT0_QBV_CONTROL0_GATEENABLED_LSP, CVA_PORT0_QBV_CONTROL0_GATEENABLED_LEN,
                                                    gateEnabled));

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateEnabled
 * @param[in] isImage
 * @retval
 */
RtkApiRet cva_drv_st_gate_hw_enabled_set(uint32 unit, cva_RtkPort portId, uint32 gateEnabled, uint32 isImage)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }
        if(((uint32)DISABLE != gateEnabled) && ((uint32)ENABLE != gateEnabled))
        {
            result = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DISABLE == isImage)
        {
            CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_QBV_CONTROL0, CVA_QBV_CONTROL0_QBV_BEGIN_LSP, CVA_QBV_CONTROL0_QBV_BEGIN_LEN, 0u));
        }
        else
        {
            ; /* do nothing */
        }
        /* configure GateEnabled parameter */
        CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL0 + (PERPORT_REG_OFFSET * portId),
                                                     CVA_PORT0_QBV_CONTROL0_GATEENABLED_LSP, CVA_PORT0_QBV_CONTROL0_GATEENABLED_LEN,
                                                     ((uint32)DISABLE == gateEnabled) ? 0x0u : 0x1u));

        /* configure tx_alarm_bc_disable & cfg_pkt_end_alarm_bc */
        if((uint32)DISABLE == gateEnabled)
        {
            result = cva_reg_write(unit, CVA_PORT0_SCHEDULER_BUCKET_CONTROL5 + (PERPORT_REG_OFFSET * portId), CVA_PORT0_SCHEDULER_BUCKET_CONTROL5_DEFAULTVAL);
        }
        else
        {

            result = cva_reg_write(unit, CVA_PORT0_SCHEDULER_BUCKET_CONTROL5 + (PERPORT_REG_OFFSET * portId), 0x00u);
        }

    } while(0u == 1u);

    return result;
}

/*
 * Expose for GUI
 *    Check HW
 */
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateEnabled
 * @retval
 */
RtkApiRet cva_drv_st_gate_enabled_get(uint32 unit, cva_RtkPort portId, uint32 *gateEnabled)
{
    RtkApiRet result   = RT_ERR_OK;
    uint32    gateHwEn = 0u;
    uint32    gateDmEn = 0u;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == gateEnabled)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* hw enabled */
        CHK_FUN_CONTINUE(result, cva_drv_st_gate_hw_enabled_get(unit, portId, &gateHwEn));

        *gateEnabled = (gateHwEn | gateDmEn);

    } while(0u == 1u);

    return result;
}

/*
 * Expose for GUI
 *
 */
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateEnabled
 * @param[in] isImage
 * @retval
 */
RtkApiRet cva_drv_st_gate_enabled_set(uint32 unit, cva_RtkPort portId, uint32 gateEnabled, uint32 isImage)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_USER_PORT_1ST > portId) || (CVA_RTK_USER_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }
        if(((uint32)DISABLE != gateEnabled) && ((uint32)ENABLE != gateEnabled))
        {
            result = -RT_ERR_INPUT;
            continue;
        }
        if(((uint32)DISABLE != isImage) && ((uint32)ENABLE != isImage))
        {
            result = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE(result, cva_drv_st_gate_hw_enabled_set(unit, cva_g_userPort2sysPort[portId], gateEnabled, isImage));

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateState
 * @retval
 */
RtkApiRet cva_drv_st_admin_gate_states_get(uint32 unit, cva_RtkPort portId, uint32 *gateState)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(gateState == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, CVA_PORT0_QBV_CONTROL0 + (PERPORT_REG_OFFSET * portId),
                                                    CVA_PORT0_QBV_CONTROL0_ADMINGATESTATES_LSP, CVA_PORT0_QBV_CONTROL0_ADMINGATESTATES_LEN,
                                                    gateState));

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateState
 * @retval
 */
RtkApiRet cva_drv_st_admin_gate_states_set(uint32 unit, cva_RtkPort portId, uint32 gateState)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(gateState > CVA_GCL_TABLE_MAX_GATE_STATE)
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, CVA_PORT0_QBV_CONTROL0 + (PERPORT_REG_OFFSET * portId),
                                                     CVA_PORT0_QBV_CONTROL0_ADMINGATESTATES_LSP, CVA_PORT0_QBV_CONTROL0_ADMINGATESTATES_LEN,
                                                     gateState));
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateState
 * @retval
 */
RtkApiRet cva_drv_st_oper_gate_states_get(uint32 unit, cva_RtkPort portId, uint32 *gateState)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(gateState == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, CVA_PORT0_QBV_CONTROL0 + (PERPORT_REG_OFFSET * portId),
                                                    CVA_PORT0_QBV_CONTROL0_OPERGATESTATES_LSP, CVA_PORT0_QBV_CONTROL0_OPERGATESTATES_LEN,
                                                    gateState));
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] baseTime
 * @param[in] cycleTimeExtension
 * @param[in] gateControlList
 * @param[in] controlListLength
 * @retval
 */
RtkApiRet cva_drv_st_gate_config_set(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension,
                                     const StGateControlListParam_t gateControlList[], uint32 controlListLength)
{
    RtkApiRet         result = RT_ERR_OK;
    uint32            iteratorList;
    uint32            isPending = 0u;
    RtkTsnCycleTime_t cycleTime;
    uint32            isImage = CVA_DISABLED;
    do
    {

        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        /* even API will check pending, it recommended to explicit check before configuration. */
        result = cva_drv_st_config_pending_get(unit, portId, &isPending);
        if(0u != isPending)
        {
            result = -RT_ERR_ST_WAIT_PENDING;
            continue;
        }

        /* configure transmission gate */
        if(NULL == gateControlList)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_ad_ctrl_lst_len_set(unit, portId, controlListLength));
        CHK_FUN_CONTINUE(result, cva_drv_st_admin_ctrl_list_set(unit, portId, gateControlList, controlListLength, isImage));

        /* cycleTime - tool generated */
        cycleTime.numerator   = 0u;
        cycleTime.denominator = CVA_TSN_1B_NANOSECOND;
        for(iteratorList = 0; iteratorList < (uint32)controlListLength; ++iteratorList)
        {
            if(CVA_ST_MAX_CYCLE_TIME <= cycleTime.numerator)
            {
                result = -RT_ERR_OUT_OF_RANGE;
                break;
            }
            cycleTime.numerator += gateControlList[iteratorList].timeInterval;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_admin_cycle_time_set(unit, portId, cycleTime));
        CHK_FUN_CONTINUE(result, cva_drv_st_ad_cycle_time_ext_set(unit, portId, cycleTimeExtension));
        CHK_FUN_CONTINUE(result, cva_drv_st_admin_base_time_set(unit, portId, baseTime));
        /* hw en = 1 */
        CHK_FUN_CONTINUE(result, cva_drv_st_gate_hw_enabled_set(unit, portId, ENABLE, isImage));
        CHK_FUN_CONTINUE(result, cva_drv_st_config_change_set(unit, portId, 1));

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] controlListLength
 * @retval
 */
RtkApiRet cva_drv_st_ad_ctrl_lst_len_get(uint32 unit, cva_RtkPort portId, uint32 *controlListLength)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(controlListLength == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL3 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL3_CONTROLLISTLENGTH_LSP, CVA_PORT0_QBV_CONTROL3_CONTROLLISTLENGTH_LEN,
                                                    controlListLength));

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] controlListLength
 * @retval
 */
RtkApiRet cva_drv_st_ad_ctrl_lst_len_set(uint32 unit, cva_RtkPort portId, uint32 controlListLength)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if((controlListLength > (uint32)CVA_ST_SUPPORT_LIST_MAX) || (controlListLength < CVA_ST_SUPPORT_LIST_MIN))
        {
            result = -RT_ERR_ST_INVALID_LENGTH;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_check_pending(unit, portId));

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL3 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, registerAddress,
                                                     CVA_PORT0_QBV_CONTROL3_CONTROLLISTLENGTH_LSP, CVA_PORT0_QBV_CONTROL3_CONTROLLISTLENGTH_LEN,
                                                     controlListLength));
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] controlListLength
 * @retval
 */
RtkApiRet cva_drv_st_op_ctrl_lst_len_get(uint32 unit, cva_RtkPort portId, uint32 *controlListLength)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(controlListLength == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));
        registerAddress = CVA_PORT0_QBV_CONTROL3 + (PERPORT_REG_OFFSET * portId) + (cva_g_stOperIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL3_CONTROLLISTLENGTH_LSP, CVA_PORT0_QBV_CONTROL3_CONTROLLISTLENGTH_LEN,
                                                    controlListLength));
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @retval
 */
RtkApiRet cva_drv_st_admin_ctrl_list_get(uint32 unit, cva_RtkPort portId, StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           iteratorList;
    uint32           baseAddress;
    uint32           tableAddress, tableData[2];
    cva_table_list_t tableType = CVA_QBV_GATE_CONTROL_LIST_TABLE;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(gateControlList == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        if((numberOfEntry < CVA_ST_SUPPORT_LIST_MIN) || (numberOfEntry > (uint32)CVA_ST_SUPPORT_LIST_MAX))
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        baseAddress = (((portId & 0xfu) << 8u) | ((cva_g_stAdminIndex[portId] & 0x1u) << 7u));
        for(iteratorList = 0; iteratorList < numberOfEntry; ++iteratorList)
        {
            if(result != RT_ERR_OK)
            {
                continue;
            }

            tableAddress = baseAddress | iteratorList;
            CHK_FUN_CONTINUE(result, cva_table_read(unit, tableType, tableAddress, (uint32 *)tableData));
            /* convert from 8ns to ns */
            gateControlList[iteratorList].timeInterval = ((tableData[0] & CVA_GCL_TABLE_TIME_INTERVAL_BITMASK) << 3u);
            gateControlList[iteratorList].gateState    = (uint8)(tableData[1] & CVA_GCL_TABLE_GATE_STATE_BITMASK);
            gateControlList[iteratorList].operation    = (uint8)((tableData[1] >> 8u) & 0x3u);
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @retval
 */
static RtkApiRet cva_drv_st_ad_ctrl_list_intf_chk(cva_RtkPort portId, uint32 numberOfEntry)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if((numberOfEntry < CVA_ST_SUPPORT_LIST_MIN) || (numberOfEntry > (uint32)CVA_ST_SUPPORT_LIST_MAX))
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

    } while(0u == 1u);

    return result;
}
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @param[in] consecutiveList
 * @param[in] isImage
 * @retval
 */
static RtkApiRet cva_drv_st_update_consecutive_gate_open_time_table(uint32 unit, cva_RtkPort portId, const StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry,
                                                                    cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u], uint32 isImage)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           baseAddress;
    uint32           tableAddress, tableData[9];
    cva_table_list_t tableType;
    uint32           iteratorList, iteratorQueue, dataRegIndex;
    uint32           tempTimeInteral = 0u;
    do
    {
        if(CVA_ENABLED != isImage)
        {
            /* Generate consecutive gate open time */
            CHK_FUN_CONTINUE(result, cva_drv_st_generate_consecutive_gate_open_time_table(gateControlList, numberOfEntry, consecutiveList));
            /* Write consecutive table */
            /* 10. SGOT Table : ACCADDR [15:0] = {Reserved[4:0], port_num[3:0], oper_admin_sel[0] (0: operation),  index[5:0]} */
            tableType   = CVA_QBV_SERIAL_GATE_OPEN_TIME_TABLE;
            baseAddress = (((portId & 0xfu) << 7u) | ((cva_g_stAdminIndex[portId] & 0x1u) << 6u));
            for(iteratorList = 0; iteratorList < ((uint32)CVA_ST_SUPPORT_LIST_MAX / 2u); ++iteratorList)
            {
                for(iteratorQueue = 0; iteratorQueue < (uint32)CVA_TSN_NUMBER_OF_QUEUE; ++iteratorQueue)
                {
                    if(iteratorQueue <= 5u)
                    {
                        dataRegIndex = iteratorQueue;
                    }
                    else
                    {
                        dataRegIndex = iteratorQueue + 1u;
                    }
                    /* Check always open case : check if consecutiveList[iteratorQueue][iteratorList].forcedInterval is 0 */
                    if(0u == (consecutiveList[iteratorQueue][iteratorList].forcedInterval & (uint8)0x1u))
                    {
                        /* Covert interval time from 4ns to ns */
                        /* Set tempTimeInteral to consecutiveList[iteratorQueue][iteratorList].timeInterval >> 3u */
                        tempTimeInteral = (((consecutiveList[iteratorQueue][iteratorList].timeInterval) >> 3u) & CVA_GCL_TABLE_TIME_INTERVAL_BITMASK);
                    }
                    else
                    {
                        /* consecutiveList[iteratorQueue][iteratorList].forcedInterval is 1 */
                        /* Set tempTimeInteral to consecutiveList[iteratorQueue][iteratorList].timeInterval */
                        tempTimeInteral = ((consecutiveList[iteratorQueue][iteratorList].timeInterval) & CVA_GCL_TABLE_TIME_INTERVAL_BITMASK);
                    }

                    tableData[dataRegIndex] = (((uint32)consecutiveList[iteratorQueue][iteratorList].forcedInterval << 31u) | ((uint32)consecutiveList[iteratorQueue][iteratorList].eor << 30u) | ((uint32)consecutiveList[iteratorQueue][iteratorList].boundary << 29u) | tempTimeInteral);
                }

                tableAddress = baseAddress | iteratorList;

                CHK_FUN_CONTINUE(result, cva_table_write(unit, tableType, tableAddress, (uint32 *)tableData));

                /* the generation function of sgot ensure write entire list to table is safe, so we don't check reach end of list of each queue indenpendently. */
            }
        }
    } while(0u == 1u);
    return result;
}
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @param[in] isImage
 * @retval
 */
RtkApiRet cva_drv_st_admin_ctrl_list_set(uint32 unit, cva_RtkPort portId, const StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry, uint32 isImage)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    iteratorList;

    cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u];

    uint32           baseAddress;
    uint32           tableAddress, tableData[9];
    cva_table_list_t tableType;

    do
    {

        CHK_FUN_CONTINUE(result, drv_memset(consecutiveList, 0, sizeof(consecutiveList)));

        CHK_FUN_CONTINUE(result, drv_memset(tableData, 0, sizeof(tableData)));

        CHK_FUN_CONTINUE(result, cva_drv_st_ad_ctrl_list_intf_chk(portId, numberOfEntry));

        CHK_FUN_CONTINUE(result, cva_drv_st_check_pending(unit, portId));

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        /* Checkc control list compatibility */

        CHK_FUN_CONTINUE(result, cva_drv_st_check_control_list_compatibility(gateControlList, numberOfEntry));
        /* Write control list table */
        /* 9. Gate Control List Table : ACCADDR [15:0] = {Reserved[3:0], port_num[3:0], oper_admin_sel[0] (0: operation), index[6:0]} */
        tableType   = CVA_QBV_GATE_CONTROL_LIST_TABLE;
        baseAddress = (((portId & 0xfu) << 8u) | ((cva_g_stAdminIndex[portId] & 0x1u) << 7u));
        for(iteratorList = 0; iteratorList < numberOfEntry; ++iteratorList)
        {
            tableAddress = baseAddress | iteratorList;
            tableData[0] = ((gateControlList[iteratorList].timeInterval) >> 3u);
            tableData[1] = (((uint32)gateControlList[iteratorList].operation) << 8u) | ((uint32)gateControlList[iteratorList].gateState);

            CHK_FUN_CONTINUE(result, cva_table_write(unit, tableType, tableAddress, (uint32 *)tableData));
        }

        if(result != RT_ERR_OK)
        {
            continue;
        }
        CHK_FUN_CONTINUE(result, cva_drv_st_update_consecutive_gate_open_time_table(unit, portId, gateControlList, numberOfEntry, consecutiveList, isImage));

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] gateControlList
 * @param[in] numberOfEntry
 * @retval
 */
RtkApiRet cva_drv_st_oper_ctrl_list_get(uint32 unit, cva_RtkPort portId, StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           iteratorList;
    uint32           baseAddress;
    uint32           tableAddress, tableData[2];
    cva_table_list_t tableType = CVA_QBV_GATE_CONTROL_LIST_TABLE;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(gateControlList == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        if((numberOfEntry < CVA_ST_SUPPORT_LIST_MIN) || (numberOfEntry > (uint32)CVA_ST_SUPPORT_LIST_MAX))
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));
        baseAddress = (((portId & 0xfu) << 8u) | ((cva_g_stOperIndex[portId] & 0x1u) << 7u));
        for(iteratorList = 0; iteratorList < numberOfEntry; ++iteratorList)
        {
            if(result != RT_ERR_OK)
            {
                continue;
            }

            tableAddress = baseAddress | iteratorList;
            CHK_FUN_CONTINUE(result, cva_table_read(unit, tableType, tableAddress, (uint32 *)tableData));
            /* convert from 8ns to ns */
            gateControlList[iteratorList].timeInterval = ((tableData[0] & CVA_GCL_TABLE_TIME_INTERVAL_BITMASK) << 3u);
            gateControlList[iteratorList].gateState    = (uint8)(tableData[1] & CVA_GCL_TABLE_GATE_STATE_BITMASK);
            gateControlList[iteratorList].operation    = (uint8)((tableData[1] >> 8u) & 0x3u);
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] cycleTime
 * @retval
 */
RtkApiRet cva_drv_st_admin_cycle_time_get(uint32 unit, cva_RtkPort portId, RtkTsnCycleTime_t *cycleTime)
{
    RtkApiRet result          = RT_ERR_OK;
    uint32    registerAddress = 0;
    uint32    registerValue   = 0;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(cycleTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL7 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        result          = cva_reg_read(unit, registerAddress, &registerValue);

        cycleTime->numerator   = registerValue;
        cycleTime->denominator = CVA_TSN_1B_NANOSECOND;

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] cycleTime
 * @retval
 */
RtkApiRet cva_drv_st_admin_cycle_time_set(uint32 unit, cva_RtkPort portId, RtkTsnCycleTime_t cycleTime)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        /* 135 ns is minimun time for 64 byte frame at 5Gbps */
        if((cycleTime.denominator != (uint32)CVA_TSN_1B_NANOSECOND) || (cycleTime.numerator <= 135u))
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_check_pending(unit, portId));

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL7 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        result          = cva_reg_write(unit, registerAddress, cycleTime.numerator);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] cycleTime
 * @retval
 */
RtkApiRet cva_drv_st_oper_cycle_time_get(uint32 unit, cva_RtkPort portId, RtkTsnCycleTime_t *cycleTime)
{
    RtkApiRet result          = RT_ERR_OK;
    uint32    registerAddress = 0;
    uint32    registerValue   = 0;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(cycleTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL7 + (PERPORT_REG_OFFSET * portId) + (cva_g_stOperIndex[portId] * 4u * 9u);
        result          = cva_reg_read(unit, registerAddress, &registerValue);

        cycleTime->numerator   = registerValue;
        cycleTime->denominator = CVA_TSN_1B_NANOSECOND;

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] cycleTimeExtension
 * @retval
 */
RtkApiRet cva_drv_st_ad_cycle_time_ext_get(uint32 unit, cva_RtkPort portId, uint32 *cycleTimeExtension)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(cycleTimeExtension == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL8 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        result          = cva_reg_read(unit, registerAddress, cycleTimeExtension);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] cycleTimeExtension
 * @retval
 */
RtkApiRet cva_drv_st_ad_cycle_time_ext_set(uint32 unit, cva_RtkPort portId, uint32 cycleTimeExtension)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_check_pending(unit, portId));

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL8 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        result          = cva_reg_write(unit, registerAddress, cycleTimeExtension);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] cycleTimeExtension
 * @retval
 */
RtkApiRet cva_drv_st_op_cycle_time_ext_get(uint32 unit, cva_RtkPort portId, uint32 *cycleTimeExtension)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(cycleTimeExtension == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL8 + (PERPORT_REG_OFFSET * portId) + (cva_g_stOperIndex[portId] * 4u * 9u);
        result          = cva_reg_read(unit, registerAddress, cycleTimeExtension);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] baseTime
 * @retval
 */
RtkApiRet cva_drv_st_admin_base_time_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;
    uint32    registerValue;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(baseTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL1 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL1_BASETIME_NS_LSP, CVA_PORT0_QBV_CONTROL1_BASETIME_NS_LEN,
                                                    &registerValue));
        baseTime->nanosecond = (uint16)registerValue;

        registerAddress = CVA_PORT0_QBV_CONTROL2 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL2_BASETIME_S_LSB_LSP, CVA_PORT0_QBV_CONTROL2_BASETIME_S_LSB_LEN,
                                                    &registerValue));
        baseTime->lsbSecond = (uint16)registerValue;

        registerAddress = CVA_PORT0_QBV_CONTROL3 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL3_BASETIME_S_MSB_LSP, CVA_PORT0_QBV_CONTROL3_BASETIME_S_MSB_LEN,
                                                    &registerValue));
        baseTime->msbSecond = (uint16)registerValue;

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] baseTime
 * @retval
 */
RtkApiRet cva_drv_st_admin_base_time_set(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(baseTime->nanosecond >= (uint32)CVA_TSN_1B_NANOSECOND)
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_check_pending(unit, portId));

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL1 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, registerAddress,
                                                     CVA_PORT0_QBV_CONTROL1_BASETIME_NS_LSP, CVA_PORT0_QBV_CONTROL1_BASETIME_NS_LEN,
                                                     baseTime->nanosecond));

        registerAddress = CVA_PORT0_QBV_CONTROL2 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, registerAddress,
                                                     CVA_PORT0_QBV_CONTROL2_BASETIME_S_LSB_LSP, CVA_PORT0_QBV_CONTROL2_BASETIME_S_LSB_LEN,
                                                     baseTime->lsbSecond));

        registerAddress = CVA_PORT0_QBV_CONTROL3 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_write(unit, registerAddress,
                                                     CVA_PORT0_QBV_CONTROL3_BASETIME_S_MSB_LSP, CVA_PORT0_QBV_CONTROL3_BASETIME_S_MSB_LEN,
                                                     baseTime->msbSecond));

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] baseTime
 * @retval
 */
RtkApiRet cva_drv_st_oper_base_time_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;
    uint32    registerValue;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(baseTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL1 + (PERPORT_REG_OFFSET * portId) + (cva_g_stOperIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL1_BASETIME_NS_LSP, CVA_PORT0_QBV_CONTROL1_BASETIME_NS_LEN,
                                                    &registerValue));
        baseTime->nanosecond = (uint16)registerValue;

        registerAddress = CVA_PORT0_QBV_CONTROL2 + (PERPORT_REG_OFFSET * portId) + (cva_g_stOperIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL2_BASETIME_S_LSB_LSP, CVA_PORT0_QBV_CONTROL2_BASETIME_S_LSB_LEN,
                                                    &registerValue));
        baseTime->lsbSecond = (uint16)registerValue;

        registerAddress = CVA_PORT0_QBV_CONTROL3 + (PERPORT_REG_OFFSET * portId) + (cva_g_stOperIndex[portId] * 4u * 9u);
        CHK_FUN_CONTINUE(result, cva_reg_field_read(unit, registerAddress,
                                                    CVA_PORT0_QBV_CONTROL3_BASETIME_S_MSB_LSP, CVA_PORT0_QBV_CONTROL3_BASETIME_S_MSB_LEN,
                                                    &registerValue));
        baseTime->msbSecond = (uint16)registerValue;

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] configChange
 * @retval
 */
RtkApiRet cva_drv_st_config_change_set(uint32 unit, cva_RtkPort portId, uint32 configChange)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(configChange > 1u)
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_check_pending(unit, portId));

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        CHK_FUN_CONTINUE(result, cva_drv_st_set_estimated_integer_multiple_cycle_time(unit, portId));

        registerAddress = CVA_PORT0_QBV_CONTROL9 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        result          = cva_reg_field_write(unit, registerAddress,
                                              CVA_PORT0_QBV_CONTROL9_CFG_RDY_LSP, CVA_PORT0_QBV_CONTROL9_CFG_RDY_LEN, configChange);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] currentTime
 * @retval
 */
RtkApiRet cva_drv_st_current_time_get(uint32 unit, RtkTsnPtpTime_t *currentTime)
{
    return cva_drv_tsn_current_time_get(unit, currentTime);
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] configPending
 * @retval
 */
RtkApiRet cva_drv_st_config_pending_get(uint32 unit, cva_RtkPort portId, uint32 *configPending)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(configPending == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerAddress = CVA_PORT0_QBV_CONTROL21 + (PERPORT_REG_OFFSET * portId);
        result          = cva_reg_field_read(unit, registerAddress,
                                             CVA_PORT0_QBV_CONTROL21_CONFIGPENDING_LSP, CVA_PORT0_QBV_CONTROL21_CONFIGPENDING_LEN, configPending);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] queueId
 * @param[in] transmissionOverrunCounter
 * @retval
 */
RtkApiRet cva_drv_st_transmission_overrun_get(uint32 unit, cva_RtkPort portId, uint32 queueId, uint64 *transmissionOverrunCounter)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;
    uint32    msb, lsb;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(queueId >= (uint32)CVA_TSN_NUMBER_OF_QUEUE)
        {
            result = -RT_ERR_QUEUE_ID;
            continue;
        }

        if(transmissionOverrunCounter == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Overrun counter LSB */
        registerAddress = CVA_PORT0_MIBCOUNTER64 + (PERPORT_REG_OFFSET * portId) + (2u * queueId * 4u);
        CHK_FUN_CONTINUE(result, cva_reg_read(unit, registerAddress, &lsb));

        /* Overrun counter MSB */
        registerAddress = CVA_PORT0_MIBCOUNTER65 + (PERPORT_REG_OFFSET * portId) + (2u * queueId * 4u);
        CHK_FUN_CONTINUE(result, cva_reg_read(unit, registerAddress, &msb));

        *transmissionOverrunCounter = (((uint64)msb) << 32u) | ((uint64)lsb);
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] instance
 * @param[in] consecutiveList
 * @retval
 */
RtkApiRet cva_drv_st_consecutive_gate_open_list_get(uint32 unit, cva_RtkPort portId, uint32 instance, cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u])
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           iteratorQueue;
    uint32           iteratorList;
    uint32           dataRegIndex;
    uint32           baseAddress;
    uint32           tableAddress, tableData[9];
    cva_table_list_t tableType = CVA_QBV_GATE_CONTROL_LIST_TABLE;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if((instance != 0u) && (instance != 1u))
        {
            result = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        if(consecutiveList == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        tableType   = CVA_QBV_SERIAL_GATE_OPEN_TIME_TABLE;
        baseAddress = (((portId & 0xfu) << 7u) | ((instance) << 6u));

        for(iteratorList = 0; iteratorList < ((uint32)CVA_ST_SUPPORT_LIST_MAX / 2u); ++iteratorList)
        {
            tableAddress = baseAddress | iteratorList;
            CHK_FUN_CONTINUE(result, cva_table_read(unit, tableType, tableAddress, (uint32 *)tableData));

            for(iteratorQueue = 0; iteratorQueue < (uint32)CVA_TSN_NUMBER_OF_QUEUE; ++iteratorQueue)
            {
                if(iteratorQueue <= 5u)
                {
                    dataRegIndex = iteratorQueue;
                }
                else
                {
                    dataRegIndex = iteratorQueue + 1u;
                }
                consecutiveList[iteratorQueue][iteratorList].forcedInterval = (uint8)((tableData[dataRegIndex] >> 31u) & 0x1u);
                if(1u == consecutiveList[iteratorQueue][iteratorList].forcedInterval)
                {
                    consecutiveList[iteratorQueue][iteratorList].timeInterval = (uint32)((tableData[dataRegIndex] & CVA_GCL_TABLE_TIME_INTERVAL_BITMASK));
                }
                else
                {
                    consecutiveList[iteratorQueue][iteratorList].timeInterval = (uint32)((tableData[dataRegIndex] & CVA_GCL_TABLE_TIME_INTERVAL_BITMASK) << 3u);
                }
                consecutiveList[iteratorQueue][iteratorList].boundary = (uint8)((tableData[dataRegIndex] >> 29u) & 0x1u);
                consecutiveList[iteratorQueue][iteratorList].eor      = (uint8)((tableData[dataRegIndex] >> 30u) & 0x1u);
            }
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] baseTime
 * @param[in] cycleTimeExtension
 * @param[in] gateControlList
 * @param[in] controlListLength
 * @retval
 */
RtkApiRet cva_drv_st_gate_config_image_set(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t baseTime, uint32 cycleTimeExtension,
                                           const StGateControlListParam_t gateControlList[], uint32 controlListLength)
{
    RtkApiRet         result;
    uint32            iteratorList;
    uint32            isPending = 0u;
    RtkTsnCycleTime_t cycleTime;
    uint32            isImage = CVA_ENABLED;

    do
    {
        /* even API will check pending, it recommended to explicit check before configuration. */
        result = cva_drv_st_config_pending_get(unit, portId, &isPending);
        if(0u != isPending)
        {
            result = -RT_ERR_ST_WAIT_PENDING;
            continue;
        }

        /* configure transmission gate */
        if(NULL == gateControlList)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_ad_ctrl_lst_len_set(unit, portId, controlListLength));
        CHK_FUN_CONTINUE(result, cva_drv_st_admin_ctrl_list_set(unit, portId, gateControlList, controlListLength, isImage));

        /* cycleTime - tool generated */
        cycleTime.numerator   = 0u;
        cycleTime.denominator = CVA_TSN_1B_NANOSECOND;
        for(iteratorList = 0; iteratorList < (uint32)(controlListLength); ++iteratorList)
        {
            if(CVA_ST_MAX_CYCLE_TIME <= cycleTime.numerator)
            {
                result = -RT_ERR_OUT_OF_RANGE;
                break;
            }
            cycleTime.numerator += gateControlList[iteratorList].timeInterval;
        }
        if(RT_ERR_OK != result)
        {
            continue;
        }
        CHK_FUN_CONTINUE(result, cva_drv_st_admin_cycle_time_set(unit, portId, cycleTime));
        CHK_FUN_CONTINUE(result, cva_drv_st_ad_cycle_time_ext_set(unit, portId, cycleTimeExtension));
        CHK_FUN_CONTINUE(result, cva_drv_st_admin_base_time_set(unit, portId, &baseTime));
        /*hw en=0*/
        CHK_FUN_CONTINUE(result, cva_drv_st_gate_hw_enabled_set(unit, portId, ENABLE, isImage));
#if 0
        /* image-mode, don't config this */
        CHK_FUN_CONTINUE(result, cva_drv_st_config_change_set(portId, 1));
#endif

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] configChange
 * @retval
 */
RtkApiRet cva_drv_st_config_change_get(uint32 unit, cva_RtkPort portId, uint32 *configChange)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(configChange == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(result, cva_drv_st_update_admin_oper_index(unit, portId));

        /* It is self-clear in next tick. It is impossible for external interface to read configChange with value 1 under normal condition; */
        registerAddress = CVA_PORT0_QBV_CONTROL9 + (PERPORT_REG_OFFSET * portId) + (cva_g_stAdminIndex[portId] * 4u * 9u);
        result          = cva_reg_field_read(unit, registerAddress,
                                             CVA_PORT0_QBV_CONTROL9_CFG_RDY_LSP, CVA_PORT0_QBV_CONTROL9_CFG_RDY_LEN, configChange);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] configChangeTime
 * @retval
 */
RtkApiRet cva_drv_st_config_change_time_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *configChangeTime)
{
    RtkApiRet                  result = RT_ERR_OK;
    uint32                     registerAddress;
    uint32                     registerValue;
    cva_RtkTsnLongNanosecond_t configChangeTimeLongNs;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(configChangeTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerAddress = CVA_PORT0_QBV_CONTROL19 + (PERPORT_REG_OFFSET * portId);
        CHK_FUN_CONTINUE(result, cva_reg_read(unit, registerAddress, &(registerValue)));
        configChangeTimeLongNs.section[0] = registerValue;

        registerAddress = CVA_PORT0_QBV_CONTROL20 + (PERPORT_REG_OFFSET * portId);
        CHK_FUN_CONTINUE(result, cva_reg_read(unit, registerAddress, &(registerValue)));
        configChangeTimeLongNs.section[1] = registerValue;

        registerAddress = CVA_PORT0_QBV_CONTROL21 + (PERPORT_REG_OFFSET * portId);
        CHK_FUN_CONTINUE(result, cva_reg_read(unit, registerAddress, &(registerValue)));
        /* only want bit 0-15 */
        configChangeTimeLongNs.section[2] = registerValue & 0xFFFFu;

        /* convert to PTT format */
        cva_drv_tsn_cov_ns_to_ptptime(configChangeTimeLongNs, configChangeTime);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 *
 * @param[in] tickGranularity
 * @retval
 */
RtkApiRet cva_drv_st_tick_granularity_get(uint32 *tickGranularity)
{
    *tickGranularity = CVA_ST_TICK_GRANULARITY;
    return RT_ERR_OK;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId
 * @param[in] configChangeErrorCounter
 * @retval
 */
RtkApiRet cva_drv_st_config_change_error_get(uint32 unit, cva_RtkPort portId, uint64 *configChangeErrorCounter)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerAddress;
    uint32    lsb, msb;

    do
    {
        if((CVA_RTK_SYS_PORT_1ST > portId) || (CVA_RTK_SYS_PORT_11TH < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(configChangeErrorCounter == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerAddress = CVA_PORT0_QBV_CONTROL25 + (PERPORT_REG_OFFSET * portId);
        CHK_FUN_CONTINUE(result, cva_reg_read(unit, registerAddress, &lsb));

        registerAddress = CVA_PORT0_QBV_CONTROL26 + (PERPORT_REG_OFFSET * portId);
        CHK_FUN_CONTINUE(result, cva_reg_read(unit, registerAddress, &msb));

        *configChangeErrorCounter = (((uint64)msb) << 32u) | ((uint64)lsb);

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 *
 * @param[in] supportedListMax
 * @retval
 */
RtkApiRet cva_drv_st_supported_list_max_get(uint32 *supportedListMax)
{
    RtkApiRet result = RT_ERR_OK;

    if(supportedListMax == NULL)
    {
        result = -RT_ERR_NULL_POINTER;
    }
    else
    {
        *supportedListMax = CVA_ST_SUPPORT_LIST_MAX;
        result            = RT_ERR_OK;
    }

    return result;
}
