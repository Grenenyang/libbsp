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
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtkas_api_phy.h"
#include "rtkas_api_interface.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ext.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ext.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ext.h"
#endif

/**
 * @defgroup RTK_MODULE_PHY PHY
 * @brief Provide PHY APIs to configure the PHY functionality or retrieve the PHY status.
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1 100/1000BASE-T1 PHY
 * @brief The 100/1000BASE-T1 PHY APIs for setting basic PHY configurations, getting status registers and dumping PHY MIB counters
 * @{
 */

/**
 * @brief Trigger a software reset on the specific ports by setting the basic management and control register (BMCR).
 * @param[in] unitChip Stacking unit and chip ID (N/A). \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] phyResetInfo PHY reset information (N/A). \n
 *                         Parameters required to reset PHY.
 * @param[out] None
 *
 * @retval RT_ERR_OK PHY reset was finished.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_phy_bmcr_reset(), dva_phy_bmcr_reset(), cvb_phy_bmcr_reset(), and cva_phy_bmcr_reset().
 */
RtkApiRet rtk_phy_bmcr_reset(UnitChip_t unitChip, PhyResetInfo_t *phyResetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == phyResetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_reset(unitChip.unit, phyResetInfo->portMap);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_reset(unitChip.unit, phyResetInfo->portMap);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_reset(unitChip.unit, phyResetInfo->portMap);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_reset(unitChip.unit, phyResetInfo->portMap);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the basic management and control register (BMCR) PCS loopback mode on the specific ports.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhyPcsLoopback PCS loopback Configuration information (N/A). \n
 *                             Parameters of specific ports to enable PCS loopback mode.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_pcs_loopback_set(), dva_phy_bmcr_pcs_loopback_set(), cvb_phy_bmcr_pcs_loopback_set(), cva_phy_bmcr_pcs_loopback_set().
 */
RtkApiRet rtk_phy_bmcr_pcs_loopback_get(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPcsLoopback)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->portMap));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Enable or disable PCS loopback mode on the specific ports by setting the the basic management and control register (BMCR). \n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyPcsLoopback PCS loopback Configuration information (N/A). \n
 *                            Parameters of specific ports to enable PCS loopback mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_pcs_loopback_set(), dva_phy_bmcr_pcs_loopback_set(), cvb_phy_bmcr_pcs_loopback_set(), cva_phy_bmcr_pcs_loopback_set().
 */
RtkApiRet rtk_phy_bmcr_pcs_loopback_set(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPcsLoopback)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->portMap, pPhyPcsLoopback->enableCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->portMap, pPhyPcsLoopback->enableCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->portMap, pPhyPcsLoopback->enableCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->portMap, pPhyPcsLoopback->enableCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the basic management and control register (BMCR) speed setting of the specific port.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhySpd Speed Configuration information (N/A). \n
 *                     Parameters of the speed of the specific port.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_speed_get(), dva_phy_bmcr_speed_get(), cvb_phy_bmcr_speed_get(), cva_phy_bmcr_speed_get().
 */
RtkApiRet rtk_phy_bmcr_speed_get(UnitChip_t unitChip, PhySpd_t *pPhySpd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhySpd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_speed_get(unitChip.unit, pPhySpd->port, &(pPhySpd->speed));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_speed_get(unitChip.unit, pPhySpd->port, &(pPhySpd->speed));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_speed_get(unitChip.unit, pPhySpd->port, &(pPhySpd->speed));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_speed_get(unitChip.unit, pPhySpd->port, &(pPhySpd->speed));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Set the basic management and control register (BMCR) speed setting of the specific port.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhySpd Speed Configuration information (N/A). \n
 *                    Parameters of the speed of the specific port.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_speed_set(), dva_phy_bmcr_speed_set(), cvb_phy_bmcr_speed_set(), cva_phy_bmcr_speed_set().
 */
RtkApiRet rtk_phy_bmcr_speed_set(UnitChip_t unitChip, PhySpd_t *pPhySpd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhySpd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_speed_set(unitChip.unit, pPhySpd->port, pPhySpd->speed);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_speed_set(unitChip.unit, pPhySpd->port, pPhySpd->speed);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_speed_set(unitChip.unit, pPhySpd->port, pPhySpd->speed);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_speed_set(unitChip.unit, pPhySpd->port, pPhySpd->speed);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get ports enable Auto-Negotiation setting from the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhyAnMode Auto-Negotiation Configuration information (N/A). \n
 *                        Parameters of specific ports enable Auto-Negotiation.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_an_mode_get(), dva_phy_bmcr_an_mode_get(), cvb_phy_bmcr_an_mode_get(), cva_phy_bmcr_an_mode_get().
 */
RtkApiRet rtk_phy_bmcr_an_mode_get(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyAnMode)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->portMap));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Enable or disable the basic management and control register (BMCR) Auto-Negotiation setting on the specific ports.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyAnMode Auto-Negotiation  Configuration information (N/A). \n
 *                       Parameters of specific ports enable Auto-Negotiation.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_an_mode_set(), dva_phy_bmcr_an_mode_set(), cvb_phy_bmcr_an_mode_set(), cva_phy_bmcr_an_mode_set().
 */
RtkApiRet rtk_phy_bmcr_an_mode_set(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyAnMode)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->portMap, pPhyAnMode->anEnableCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->portMap, pPhyAnMode->anEnableCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->portMap, pPhyAnMode->anEnableCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->portMap, pPhyAnMode->anEnableCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Trigger Auto-Negotiation on the specific ports by setting the the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyRestartAn Auto-Negotiation Restart Configuration information (N/A). \n
 *                          Parameters of specific ports to restart Auto-Negotiation.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_restart_an(), dva_phy_bmcr_restart_an(), cvb_phy_bmcr_restart_an(), cva_phy_bmcr_restart_an().
 */
RtkApiRet rtk_phy_bmcr_restart_an(UnitChip_t unitChip, PhyRestartAn_t *pPhyRestartAn)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyRestartAn)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_restart_an(unitChip.unit, pPhyRestartAn->portMap);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_restart_an(unitChip.unit, pPhyRestartAn->portMap);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_restart_an(unitChip.unit, pPhyRestartAn->portMap);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_restart_an(unitChip.unit, pPhyRestartAn->portMap);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get ports configured the basic management and control register(BMCR) power down setting.
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhyPwd PHY Power Down Configuration information (N/A). \n
 *                     Parameters to get the power down setting on specific ports.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_power_down_get(), dva_phy_bmcr_power_down_get(), cvb_phy_bmcr_power_down_get(), cva_phy_bmcr_power_down_get().
 */
RtkApiRet rtk_phy_bmcr_power_down_get(UnitChip_t unitChip, PhyPwd_t *pPhyPwd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPwd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->portMap));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Set the basic management and control register (BMCR) power down setting to power down or power up the PHY of specific ports.
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyPwd  PHY Power Down Configuration information (N/A). \n
 *                     Parameters to power down or power up specific ports.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_power_down_set(), dva_phy_bmcr_power_down_set(), cvb_phy_bmcr_power_down_set(), cva_phy_bmcr_power_down_set().
 */
RtkApiRet rtk_phy_bmcr_power_down_set(UnitChip_t unitChip, PhyPwd_t *pPhyPwd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPwd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_power_down_set(unitChip.unit, pPhyPwd->portMap, pPhyPwd->pwdCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_power_down_set(unitChip.unit, pPhyPwd->portMap, pPhyPwd->pwdCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_power_down_set(unitChip.unit, pPhyPwd->portMap, pPhyPwd->pwdCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_power_down_set(unitChip.unit, pPhyPwd->portMap, pPhyPwd->pwdCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the the basic management and control register (BMCR) isolate configuration on the specific ports.\n
 *        PHY Isolate Mode enables communication via the MDIO/MDC interface without MAC management.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhyIsolate PHY Isolate Configuration information (N/A). \n
 *                        Parameters to get the isolate setting on the specific ports.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_isolate_set(), dva_phy_bmcr_isolate_set(), cvb_phy_bmcr_isolate_set(), cva_phy_bmcr_isolate_set().
 */
RtkApiRet rtk_phy_bmcr_isolate_get(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIsolate)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->portMap));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Configure the the basic management and control register (BMCR) isolate configuration on the specific ports.\n
 *        PHY Isolate Mode enables communication via the MDIO/MDC interface without MAC management.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyIsolate PHY Isolate Configuration information (N/A). \n
 *                        Parameters to configure the isolate on the specific ports.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_phy_bmcr_isolate_set(), dva_phy_bmcr_isolate_set(), cvb_phy_bmcr_isolate_set(), cva_phy_bmcr_isolate_set().
 */
RtkApiRet rtk_phy_bmcr_isolate_set(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIsolate)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->portMap, pPhyIsolate->isoCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->portMap, pPhyIsolate->isoCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->portMap, pPhyIsolate->isoCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->portMap, pPhyIsolate->isoCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the PHY Identifier Registers including PHYID1 and PHYID2 to get the Organizationally Unique Identifier(OUI).
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyIdInfo Pointer to a struct contains the PHY information (N/A). \n
 *                              Include OUI, Manufacture’s Model Number and Revision Number.
 * @retVal RT_ERR_OK            get PHY OUI successfully
 * @retVal -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retVal -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retVal Others               Please refer to dv1_phy_phyid_get(), dva_phy_phyid_get(), cvb_phy_phyid_get(), or cva_phy_phyid_get().
 */
RtkApiRet rtk_phy_phyid_get(UnitChip_t unitChip, PhyIdInfo_t *pPhyIdInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIdInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_phyid_get(unitChip.unit, pPhyIdInfo->port, &(pPhyIdInfo->oui), &(pPhyIdInfo->modleNum), &(pPhyIdInfo->revNum));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_phyid_get(unitChip.unit, pPhyIdInfo->port, &(pPhyIdInfo->oui), &(pPhyIdInfo->modleNum), &(pPhyIdInfo->revNum));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_phyid_get(unitChip.unit, pPhyIdInfo->port, &(pPhyIdInfo->oui), &(pPhyIdInfo->modleNum), &(pPhyIdInfo->revNum));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_phyid_get(unitChip.unit, pPhyIdInfo->port, &(pPhyIdInfo->oui), &(pPhyIdInfo->modleNum), &(pPhyIdInfo->revNum));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Configure the PHY Control Register (PHYCR) to set the PHY role to slave or master.
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyRoleCfgInfo Pointer to a struct to access PHY role configuration (N/A). \n
 *                                The struct includes the port index and isMaster.\n
 *                                - isMaster = TRUE: Configure the PHY role of designated port to master
 *                                - isMaster = FALSE: Configure the PHY role of designated port to slave
 *
 * @retval RT_ERR_OK            set PHY role configuratiThe function was executed successfully.fully
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_phycr_role_set(), dva_phy_phycr_role_set(), cvb_phy_phycr_role_set(), or cva_phy_phycr_role_set().
 */
RtkApiRet rtk_phy_phycr_role_set(UnitChip_t unitChip, PhyRoleCfgInfo_t *pPhyRoleCfgInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyRoleCfgInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_phycr_role_set(unitChip.unit, pPhyRoleCfgInfo->port, pPhyRoleCfgInfo->isMaster);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_phycr_role_set(unitChip.unit, pPhyRoleCfgInfo->port, pPhyRoleCfgInfo->isMaster);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_phycr_role_set(unitChip.unit, pPhyRoleCfgInfo->port, pPhyRoleCfgInfo->isMaster);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_phycr_role_set(unitChip.unit, pPhyRoleCfgInfo->port, pPhyRoleCfgInfo->isMaster);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Control Register (PHYCR) to get the PHY role .
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyRoleCfgInfo Pointer to a struct to access PHY role configuration (N/A). \n
 *                                The struct includes the port index and isMaster.\n
 *                                - isMaster = TRUE: The PHY role of designated port is master
 *                                - isMaster = FALSE: The PHY role of designated port is slave
 *
 * @retval RT_ERR_OK            get PHY role configuratiThe function was executed successfully.fully
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_phycr_role_get(), dva_phy_phycr_role_get(), cvb_phy_phycr_role_get(), or cva_phy_phycr_role_get().
 */
RtkApiRet rtk_phy_phycr_role_get(UnitChip_t unitChip, PhyRoleCfgInfo_t *pPhyRoleCfgInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyRoleCfgInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_phycr_role_get(unitChip.unit, pPhyRoleCfgInfo->port, &(pPhyRoleCfgInfo->isMaster));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_phycr_role_get(unitChip.unit, pPhyRoleCfgInfo->port, &(pPhyRoleCfgInfo->isMaster));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_phycr_role_get(unitChip.unit, pPhyRoleCfgInfo->port, &(pPhyRoleCfgInfo->isMaster));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_phycr_role_get(unitChip.unit, pPhyRoleCfgInfo->port, &(pPhyRoleCfgInfo->isMaster));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Basic Mode Status Register (BMSR) to get the PHY link status.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyLinkStatus Pointer to a struct to access PHY link status (N/A). \n
 *                               - linkSts = 1: PHY link up
 *                               - linkSts = 0: PHY link down
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_bmsr_link_status_get(), dva_phy_bmsr_link_status_get(), cvb_phy_bmsr_link_status_get(), or cva_phy_bmsr_link_status_get().
 */
RtkApiRet rtk_phy_bmsr_link_status_get(UnitChip_t unitChip, PhyLinkStatus_t *pPhyLinkStatus)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyLinkStatus)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_bmsr_link_status_get(unitChip.unit, pPhyLinkStatus->port, &(pPhyLinkStatus->linkSts));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_bmsr_link_status_get(unitChip.unit, pPhyLinkStatus->port, &(pPhyLinkStatus->linkSts));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_bmsr_link_status_get(unitChip.unit, pPhyLinkStatus->port, &(pPhyLinkStatus->linkSts));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_bmsr_link_status_get(unitChip.unit, pPhyLinkStatus->port, &(pPhyLinkStatus->linkSts));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Status Register 1 (PHYSR1) value to get the role setting, idle error count and local and remote receiver status of 1000BASE-T1.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyPHYSR1 Pointer to a struct to access PHY PHYSR1 register settings (N/A). \n
 *                        The PHYSR1 contains role, local receiver status, remote receiver status, and idle error counter.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_physr1_info_cfg(), dva_phy_physr1_info_cfg(), cvb_phy_physr1_info_cfg(), or cva_phy_physr1_info_cfg().
 */
RtkApiRet rtk_phy_physr1_info_cfg(UnitChip_t unitChip, PhyPHYSR1_t *pPhyPHYSR1)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPHYSR1)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_physr1_info_cfg(unitChip.unit, pPhyPHYSR1->port, &(pPhyPHYSR1->regValue), &(pPhyPHYSR1->roleCfg), &(pPhyPHYSR1->locRcvReady), &(pPhyPHYSR1->remRcvReady), &(pPhyPHYSR1->idleErrorCnt));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_physr1_info_cfg(unitChip.unit, pPhyPHYSR1->port, &(pPhyPHYSR1->regValue), &(pPhyPHYSR1->roleCfg), &(pPhyPHYSR1->locRcvReady), &(pPhyPHYSR1->remRcvReady), &(pPhyPHYSR1->idleErrorCnt));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_physr1_info_cfg(unitChip.unit, pPhyPHYSR1->port, &(pPhyPHYSR1->regValue), &(pPhyPHYSR1->roleCfg), &(pPhyPHYSR1->locRcvReady), &(pPhyPHYSR1->remRcvReady), &(pPhyPHYSR1->idleErrorCnt));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_physr1_info_cfg(unitChip.unit, pPhyPHYSR1->port, &(pPhyPHYSR1->regValue), &(pPhyPHYSR1->roleCfg), &(pPhyPHYSR1->locRcvReady), &(pPhyPHYSR1->remRcvReady), &(pPhyPHYSR1->idleErrorCnt));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Status Register 2 (PHYSR2) to get the PHY master mode setting, speed, duplex and real-time link status.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyPHYSR2 Pointer to a struct to access PHY PHYSR2 register settings (N/A). \n
 *                        The PHYSR2 contains role config, speed, duplex, and real time link status.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_physr2_info_cfg(), dva_phy_physr2_info_cfg(), cvb_phy_physr2_info_cfg(), or cva_phy_physr2_info_cfg().
 */
RtkApiRet rtk_phy_physr2_info_cfg(UnitChip_t unitChip, PhyPHYSR2_t *pPhyPHYSR2)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPHYSR2)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_physr2_info_cfg(unitChip.unit, pPhyPHYSR2->port, &(pPhyPHYSR2->regValue), &(pPhyPHYSR2->masterMode), &(pPhyPHYSR2->speed), &(pPhyPHYSR2->duplex), &(pPhyPHYSR2->realTimelinkSts));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_physr2_info_cfg(unitChip.unit, pPhyPHYSR2->port, &(pPhyPHYSR2->regValue), &(pPhyPHYSR2->masterMode), &(pPhyPHYSR2->speed), &(pPhyPHYSR2->duplex), &(pPhyPHYSR2->realTimelinkSts));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_physr2_info_cfg(unitChip.unit, pPhyPHYSR2->port, &(pPhyPHYSR2->regValue), &(pPhyPHYSR2->masterMode), &(pPhyPHYSR2->speed), &(pPhyPHYSR2->duplex), &(pPhyPHYSR2->realTimelinkSts));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_physr2_info_cfg(unitChip.unit, pPhyPHYSR2->port, &(pPhyPHYSR2->regValue), &(pPhyPHYSR2->masterMode), &(pPhyPHYSR2->speed), &(pPhyPHYSR2->duplex), &(pPhyPHYSR2->realTimelinkSts));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Status Register 4 (PHYSR4) to get the register value contains RX and TX polarity, and the local and remote receiver status of 100BASE-T1.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyPHYSR4 Pointer to a struct to access PHY PHYSR4 register settings (N/A). \n
 *                        The PHYSR2 contains tx and rx polarity, remote and local receiver status.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_physr4_info_cfg(), dva_phy_physr4_info_cfg(), cvb_phy_physr4_info_cfg(), or cva_phy_physr4_info_cfg().
 */
RtkApiRet rtk_phy_physr4_info_cfg(UnitChip_t unitChip, PhyPHYSR4_t *pPhyPHYSR4)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPHYSR4)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_physr4_info_cfg(unitChip.unit, pPhyPHYSR4->port, &(pPhyPHYSR4->regValue), &(pPhyPHYSR4->rxPolarity), &(pPhyPHYSR4->txPolarity), &(pPhyPHYSR4->remoteReceiverSts), &(pPhyPHYSR4->localReceiverSts));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_physr4_info_cfg(unitChip.unit, pPhyPHYSR4->port, &(pPhyPHYSR4->regValue), &(pPhyPHYSR4->rxPolarity), &(pPhyPHYSR4->txPolarity), &(pPhyPHYSR4->remoteReceiverSts), &(pPhyPHYSR4->localReceiverSts));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_physr4_info_cfg(unitChip.unit, pPhyPHYSR4->port, &(pPhyPHYSR4->regValue), &(pPhyPHYSR4->rxPolarity), &(pPhyPHYSR4->txPolarity), &(pPhyPHYSR4->remoteReceiverSts), &(pPhyPHYSR4->localReceiverSts));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_physr4_info_cfg(unitChip.unit, pPhyPHYSR4->port, &(pPhyPHYSR4->regValue), &(pPhyPHYSR4->rxPolarity), &(pPhyPHYSR4->txPolarity), &(pPhyPHYSR4->remoteReceiverSts), &(pPhyPHYSR4->localReceiverSts));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Status Sub-Flag Register (PHYSFR) to get the PHY status sub-flag.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyPHYSFR Pointer to a struct to access PHYSFR register settings (N/A). \n
 *                           The PHYSFR contains PHY status sub-flag.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_physfr_info_cfg(), dva_phy_physfr_info_cfg(), cvb_phy_physfr_info_cfg(), or cva_phy_physfr_info_cfg().
 */
RtkApiRet rtk_phy_physfr_info_cfg(UnitChip_t unitChip, PhyPHYSFR_t *pPhyPHYSFR)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPHYSFR)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_physfr_info_cfg(unitChip.unit, pPhyPHYSFR->port, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_physfr_info_cfg(unitChip.unit, pPhyPHYSFR->port, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_physfr_info_cfg(unitChip.unit, pPhyPHYSFR->port, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_physfr_info_cfg(unitChip.unit, pPhyPHYSFR->port, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY PCS status.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyPCS Pointer to a struct to access PHY PCS status (N/A). \n
 *                        The PHY current PCS status indicating the link situation or the state of the addressed PHY.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_pcs_info_cfg(), dva_phy_pcs_info_cfg(), cvb_phy_pcs_info_cfg(), or cva_phy_pcs_info_cfg().
 */
RtkApiRet rtk_phy_pcs_info_cfg(UnitChip_t unitChip, PhyPCS_t *pPhyPCS)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPCS)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_pcs_info_cfg(unitChip.unit, pPhyPCS->port, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_pcs_info_cfg(unitChip.unit, pPhyPCS->port, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_pcs_info_cfg(unitChip.unit, pPhyPCS->port, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_pcs_info_cfg(unitChip.unit, pPhyPCS->port, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Configure the PHY Internal Packet Counter Register 1 (IPCR1) to enable or disable remote loopback, packet counter and configure path select.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyIPCR1 Pointer to a struct to access PHY IPCR1 register (N/A). \n
 *                          The IPCR1 register indicates the monitoring path of the MIB counter.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_PHY_IPCR1_info_cfg(), dva_phy_PHY_IPCR1_info_cfg(), cvb_phy_PHY_IPCR1_info_cfg(), or cva_phy_PHY_IPCR1_info_cfg().
 */
RtkApiRet rtk_phy_ipcr1_info_cfg(UnitChip_t unitChip, PhyIPCR1_t *pPhyIPCR1)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIPCR1)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_ipcr1_info_cfg(unitChip.unit, pPhyIPCR1->port, pPhyIPCR1->enRemoteLbk, pPhyIPCR1->enPktCounter, pPhyIPCR1->pathSel);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_ipcr1_info_cfg(unitChip.unit, pPhyIPCR1->port, pPhyIPCR1->enRemoteLbk, pPhyIPCR1->enPktCounter, pPhyIPCR1->pathSel);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_ipcr1_info_cfg(unitChip.unit, pPhyIPCR1->port, pPhyIPCR1->enRemoteLbk, pPhyIPCR1->enPktCounter, pPhyIPCR1->pathSel);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_ipcr1_info_cfg(unitChip.unit, pPhyIPCR1->port, pPhyIPCR1->enRemoteLbk, pPhyIPCR1->enPktCounter, pPhyIPCR1->pathSel);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Configure the PHY Internal Packet Counter Register 2 (IPCR2) to clear internal MIB counters.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyIPCR2 Pointer to a struct to access PHY IPCR2 register (N/A). \n
 *                          Set the IPCR2 register to reset all MIB counter values.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_PHY_IPCR1_info_cfg(), dva_phy_PHY_IPCR1_info_cfg(), cvb_phy_PHY_IPCR1_info_cfg(), or cva_phy_PHY_IPCR1_info_cfg().
 */
RtkApiRet rtk_phy_ipcr2_info_cfg(UnitChip_t unitChip, PhyIPCR2_t *pPhyIPCR2)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIPCR2)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_ipcr2_info_cfg(unitChip.unit, pPhyIPCR2->port, pPhyIPCR2->intCntErrClr, pPhyIPCR2->intCntClr, pPhyIPCR2->rxPkCntRst);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_ipcr2_info_cfg(unitChip.unit, pPhyIPCR2->port, pPhyIPCR2->intCntErrClr, pPhyIPCR2->intCntClr, pPhyIPCR2->rxPkCntRst);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_ipcr2_info_cfg(unitChip.unit, pPhyIPCR2->port, pPhyIPCR2->intCntErrClr, pPhyIPCR2->intCntClr, pPhyIPCR2->rxPkCntRst);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_ipcr2_info_cfg(unitChip.unit, pPhyIPCR2->port, pPhyIPCR2->intCntErrClr, pPhyIPCR2->intCntClr, pPhyIPCR2->rxPkCntRst);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Internal Packet Counter Register 3 (IPCR3), Internal Packet Counter Register 4 (IPCR4), Internal Error Packet Counter Register (IEPCR) status.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyIPCRCntVal Pointer to a struct to access PHY internal packet counter (N/A). \n
 *                              Get the PHY internal packet counter.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_phy_ipcr_cnt(), dva_phy_ipcr_cnt(), cvb_phy_ipcr_cnt(), or cva_phy_ipcr_cnt().
 */
RtkApiRet rtk_phy_ipcr_cnt(UnitChip_t unitChip, PhyIPCRCntVal_t *pPhyIPCRCntVal)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIPCRCntVal)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_ipcr_cnt(unitChip.unit, pPhyIPCRCntVal->port, &(pPhyIPCRCntVal->intCntLSB), &(pPhyIPCRCntVal->intCntMSB), &(pPhyIPCRCntVal->intErrCnt));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_ipcr_cnt(unitChip.unit, pPhyIPCRCntVal->port, &(pPhyIPCRCntVal->intCntLSB), &(pPhyIPCRCntVal->intCntMSB), &(pPhyIPCRCntVal->intErrCnt));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_ipcr_cnt(unitChip.unit, pPhyIPCRCntVal->port, &(pPhyIPCRCntVal->intCntLSB), &(pPhyIPCRCntVal->intCntMSB), &(pPhyIPCRCntVal->intErrCnt));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_ipcr_cnt(unitChip.unit, pPhyIPCRCntVal->port, &(pPhyIPCRCntVal->intCntLSB), &(pPhyIPCRCntVal->intCntMSB), &(pPhyIPCRCntVal->intErrCnt));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1 */

/**
 * @defgroup PHY_ETH 10/1000BASE-T and 100BASE-TX PHY
 * @brief The 10/1000BASE-T and 100BASE-TX PHY APIs for setting basic PHY configurations and getting status registers
 * @{
 */

/**
 * @brief Trigger a software reset on the Ethernet ports by configuring the basic management and control register (BMCR).
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] None
 *
 * @retval RT_ERR_OK           PHY reset was finished.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_reset(), dva_ethphy_bmcr_reset(), cvb_ethphy_bmcr_reset(), cva_ethphy_bmcr_reset().
 */
RtkApiRet rtk_ethphy_bmcr_reset(UnitChip_t unitChip)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            retVal = dv1_ethphy_bmcr_reset(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            retVal = dva_ethphy_bmcr_reset(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            retVal = cvb_ethphy_bmcr_reset(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            retVal = cva_ethphy_bmcr_reset(unitChip.unit);
            break;
#endif
        default:
            retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
            break;
    }

    return retVal;
}

/**
 * @brief Get PCS loopback mode of the Ethernet port by configuring the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhyPcsLoopback PCS loopback Configuration information (N/A). \n
 *                             Parameters of the Ethernet port to enable PCS loopback mode.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_pcs_loopback_set(), dva_ethphy_bmcr_pcs_loopback_set(), cvb_ethphy_bmcr_pcs_loopback_set(), cva_ethphy_bmcr_pcs_loopback_set().
 */
RtkApiRet rtk_ethphy_bmcr_pcs_loopback_get(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPcsLoopback)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->enableCfg));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->enableCfg));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->enableCfg));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_pcs_loopback_get(unitChip.unit, &(pPhyPcsLoopback->enableCfg));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Enable or disable PCS loopback mode of the Ethernet port by configuring the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyPcsLoopback PCS loopback Configuration information (N/A). \n
 *                            Parameters of the Ethernet port to enable PCS loopback mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_pcs_loopback_set(), dva_ethphy_bmcr_pcs_loopback_set(), cvb_ethphy_bmcr_pcs_loopback_set(), cva_ethphy_bmcr_pcs_loopback_set().
 */
RtkApiRet rtk_ethphy_bmcr_pcs_loopback_set(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPcsLoopback)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->enableCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->enableCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->enableCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_pcs_loopback_set(unitChip.unit, pPhyPcsLoopback->enableCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the basic management and control register(BMCR) speed setting of the Ethernet port.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhySpd Speed Configuration information (N/A). \n
 *                     Parameters of the speed of the Ethernet port.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_speed_get(), dva_ethphy_bmcr_speed_get(), cvb_ethphy_bmcr_speed_get(), cva_ethphy_bmcr_speed_get().
 */
RtkApiRet rtk_ethphy_bmcr_speed_get(UnitChip_t unitChip, PhySpd_t *pPhySpd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhySpd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_speed_get(unitChip.unit, &(pPhySpd->speed));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_speed_get(unitChip.unit, &(pPhySpd->speed));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_speed_get(unitChip.unit, &(pPhySpd->speed));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_speed_get(unitChip.unit, &(pPhySpd->speed));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Set the basic management and control register(BMCR) speed setting of the Ethernet port.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhySpd Speed Configuration information (N/A). \n
 *                    Parameters of the speed of the Ethernet port.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_speed_set(), dva_ethphy_bmcr_speed_set(), cvb_ethphy_bmcr_speed_set(), cva_ethphy_bmcr_speed_set().
 */
RtkApiRet rtk_ethphy_bmcr_speed_set(UnitChip_t unitChip, PhySpd_t *pPhySpd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhySpd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_speed_set(unitChip.unit, pPhySpd->speed);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_speed_set(unitChip.unit, pPhySpd->speed);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_speed_set(unitChip.unit, pPhySpd->speed);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_speed_set(unitChip.unit, pPhySpd->speed);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get Auto-Negotiation setting of the Ethernet port by the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyAnMode Auto-Negotiation Configuration information (N/A). \n
 *                       Parameters to get the Auto-Negotiation of the Ethernet port.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_an_mode_get(), dva_ethphy_bmcr_an_mode_get(), cvb_ethphy_bmcr_an_mode_get(), cva_ethphy_bmcr_an_mode_get().
 */
RtkApiRet rtk_ethphy_bmcr_an_mode_get(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyAnMode)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->anEnableCfg));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->anEnableCfg));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->anEnableCfg));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_an_mode_get(unitChip.unit, &(pPhyAnMode->anEnableCfg));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Enable or disable Auto-Negotiation setting on the Ethernet port by configuring the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyAnMode Auto-Negotiation  Configuration information (N/A). \n
 *                       Parameters to enable Auto-Negotiation of the Ethernet port.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_an_mode_set(), dva_ethphy_bmcr_an_mode_set(), cvb_ethphy_bmcr_an_mode_set(), cva_ethphy_bmcr_an_mode_set().
 */
RtkApiRet rtk_ethphy_bmcr_an_mode_set(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyAnMode)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->anEnableCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->anEnableCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->anEnableCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_an_mode_set(unitChip.unit, pPhyAnMode->anEnableCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Trigger Auto-Negotiation on the Ethernet port by configuring the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_restart_an(), dva_ethphy_bmcr_restart_an(), cvb_ethphy_bmcr_restart_an(), cva_ethphy_bmcr_restart_an().
 */
RtkApiRet rtk_ethphy_bmcr_restart_an(UnitChip_t unitChip)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            retVal = dv1_ethphy_bmcr_restart_an(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            retVal = dva_ethphy_bmcr_restart_an(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            retVal = cvb_ethphy_bmcr_restart_an(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            retVal = cva_ethphy_bmcr_restart_an(unitChip.unit);
            break;
#endif
        default:
            retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
            break;
    }

    return retVal;
}

/**
 * @brief Get PHY power down configuration of the Ethernet port by configuring the basic management and control register (BMCR).
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhyPwd PHY Power Down Configuration information (N/A). \n
 *                     Parameters to get the power down setting on the Ethernet port.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_power_down_get(), dva_ethphy_bmcr_power_down_get(), cvb_ethphy_bmcr_power_down_get(), cva_ethphy_bmcr_power_down_get().
 */
RtkApiRet rtk_ethphy_bmcr_power_down_get(UnitChip_t unitChip, PhyPwd_t *pPhyPwd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPwd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->pwdCfg));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->pwdCfg));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->pwdCfg));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_power_down_get(unitChip.unit, &(pPhyPwd->pwdCfg));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Power down or power up the PHY of the Ethernet port by configuring the basic management and control register (BMCR).
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyPwd  PHY Power Down Configuration information (N/A). \n
 *                     Parameters to power down or power up the Ethernet ports.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_power_down_set(), dva_ethphy_bmcr_power_down_set(), cvb_ethphy_bmcr_power_down_set(), cva_ethphy_bmcr_power_down_set().
 */
RtkApiRet rtk_ethphy_bmcr_power_down_set(UnitChip_t unitChip, PhyPwd_t *pPhyPwd)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPwd)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_power_down_set(unitChip.unit, pPhyPwd->pwdCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_power_down_set(unitChip.unit, pPhyPwd->pwdCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_power_down_set(unitChip.unit, pPhyPwd->pwdCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_power_down_set(unitChip.unit, pPhyPwd->pwdCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the isolate configuration on the Ethernet ports by configuring the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[out] pPhyIsolate PHY Isolate Configuration information (N/A). \n
 *                        Parameters to get the isolate setting of the Ethernet ports.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_isolate_set(), dva_ethphy_bmcr_isolate_set(), cvb_ethphy_bmcr_isolate_set(), cva_ethphy_bmcr_isolate_set().
 */
RtkApiRet rtk_ethphy_bmcr_isolate_get(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIsolate)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->isoCfg));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->isoCfg));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->isoCfg));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_isolate_get(unitChip.unit, &(pPhyIsolate->isoCfg));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Configure the isolate configuration on the Ethernet ports by configuring the basic management and control register (BMCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyIsolate PHY Isolate Configuration information (N/A). \n
 *                        Parameters to configure the isolate on the Ethernet port.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_bmcr_isolate_set(), dva_ethphy_bmcr_isolate_set(), cvb_ethphy_bmcr_isolate_set(), cva_ethphy_bmcr_isolate_set().
 */
RtkApiRet rtk_ethphy_bmcr_isolate_set(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyIsolate)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->isoCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->isoCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->isoCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmcr_isolate_set(unitChip.unit, pPhyIsolate->isoCfg);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the BMSR to get the Ethernet-PHY link status by configuring the basic mode status register (BMSR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyLinkStatus Pointer to a struct to access PHY link status (N/A). \n
 *                               - linkSts = 1: PHY link up
 *                               - linkSts = 0: PHY link down
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_ethphy_bmsr_link_status_get(), dva_ethphy_bmsr_link_status_get(), cvb_ethphy_bmsr_link_status_get(), or cva_ethphy_bmsr_link_status_get().
 */
RtkApiRet rtk_ethphy_bmsr_link_status_get(UnitChip_t unitChip, PhyLinkStatus_t *pPhyLinkStatus)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyLinkStatus)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_bmsr_link_status_get(unitChip.unit, &(pPhyLinkStatus->linkSts));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_bmsr_link_status_get(unitChip.unit, &(pPhyLinkStatus->linkSts));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_bmsr_link_status_get(unitChip.unit, &(pPhyLinkStatus->linkSts));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_bmsr_link_status_get(unitChip.unit, &(pPhyLinkStatus->linkSts));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY Status Sub-Flag Register (PHYSFR) to get the PHY status sub-flag.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyPHYSFR Pointer to a struct to access PHY PHYSFR register settings (N/A). \n
 *                        The PHYSFR contains PHY status sub-flag.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_ethphy_physfr_info_cfg(), dva_ethphy_physfr_info_cfg(), cvb_ethphy_physfr_info_cfg(), or cva_ethphy_physfr_info_cfg().
 */
RtkApiRet rtk_ethphy_physfr_info_cfg(UnitChip_t unitChip, PhyPHYSFR_t *pPhyPHYSFR)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPHYSFR)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_physfr_info_cfg(unitChip.unit, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_physfr_info_cfg(unitChip.unit, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_physfr_info_cfg(unitChip.unit, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_physfr_info_cfg(unitChip.unit, &(pPhyPHYSFR->regValue), &(pPhyPHYSFR->phySubFlag));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the PHY PCS status.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyPCS Pointer to a struct to access PHY PCS status (N/A). \n
 *                        The PHY current PCS status indicating the link situation or the state of the addressed PHY.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_ethphy_pcs_info_cfg(), dva_ethphy_pcs_info_cfg(), cvb_ethphy_pcs_info_cfg(), or cva_ethphy_pcs_info_cfg().
 */
RtkApiRet rtk_ethphy_pcs_info_cfg(UnitChip_t unitChip, PhyPCS_t *pPhyPCS)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPCS)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_pcs_info_cfg(unitChip.unit, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_pcs_info_cfg(unitChip.unit, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_pcs_info_cfg(unitChip.unit, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_pcs_info_cfg(unitChip.unit, &(pPhyPCS->regValue), &(pPhyPCS->phyPCS));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Set the Master-slave control register (MSCR) to configuring the master/slave mode related setting.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A). \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyMSCRCfg Pointer to a struct to access PHY role configuration (N/A). \n
 *                            The struct includes role setting. \n
 *                            - mscrRoleCfg = PHY_ROLE_MANUAL_MASTER: Manually configure the PHY role of the Ethernet port to master. \n
 *                            - mscrRoleCfg = PHY_ROLE_MANUAL_SLAVE: Manually configure the PHY role of the Ethernet port to slave. \n
 *                            - mscrRoleCfg = PHY_ROLE_AUTO: Disable manual configuration of the PHY role.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dva_ethphy_mscr_role_set().
 */
RtkApiRet rtk_ethphy_mscr_role_cfg_set(UnitChip_t unitChip, PhyMSCRCfg_t *pPhyMSCRCfg)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyMSCRCfg)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_mscr_role_set(unitChip.unit, pPhyMSCRCfg->mscrRoleCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the Master-slave control register (MSCR) to get the the master/slave mode related setting.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A). \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyRoleCfgInfo Pointer to a struct to access PHY role configuration (N/A). \n
 *                                The struct includes role setting. \n
 *                                - mscrRoleCfg = PHY_ROLE_MANUAL_MASTER: Manually configure the PHY role of the Ethernet port to master. \n
 *                                - mscrRoleCfg = PHY_ROLE_MANUAL_SLAVE: Manually configure the PHY role of the Ethernet port to slave. \n
 *                                - mscrRoleCfg = PHY_ROLE_AUTO: Disable manual configuration of the PHY role.
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dva_ethphy_mscr_role_get().
 */
RtkApiRet rtk_ethphy_mscr_role_cfg_get(UnitChip_t unitChip, PhyMSCRCfg_t *pPhyMSCRCfg)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyMSCRCfg)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_mscr_role_get(unitChip.unit, &(pPhyMSCRCfg->mscrRoleCfg));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Configure the Ethernet-PHY Control Register (PHYCR) to set the MDI/MDIX mode.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in] pPhyMdixCfg Pointer to a struct to access PHY MDI/MDIX configuration (N/A). \n
 *                        The struct includes the mdixMode.\n
 *                        - mdixMode = 0: Manual force MDIX mode
 *                        - mdixMode = 1: Manual force MDI mode
 *                        - mdixMode = 2: Auto MDI/MDIX mode
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_ethphy_mdix_mode_set(), dva_ethphy_mdix_mode_set(), cvb_ethphy_mdix_mode_set(), or cva_ethphy_mdix_mode_set().
 */
RtkApiRet rtk_ethphy_mdix_mode_set(UnitChip_t unitChip, PhyMdixCfg_t *pPhyMdixCfg)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyMdixCfg)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_mdix_mode_set(unitChip.unit, pPhyMdixCfg->mdixMode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_mdix_mode_set(unitChip.unit, pPhyMdixCfg->mdixMode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_mdix_mode_set(unitChip.unit, pPhyMdixCfg->mdixMode);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_mdix_mode_set(unitChip.unit, pPhyMdixCfg->mdixMode);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Read the Ethernet-PHY Control Register (PHYCR) to get the MDI/MDIX mode.\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pPhyMdixCfg Pointer to a struct to access PHY MDI/MDIX configuration (N/A). \n
 *                            The struct includes the mdixMode.\n
 *                            - mdixMode = 0: Manual force MDIX mode
 *                            - mdixMode = 1: Manual force MDI mode
 *                            - mdixMode = 2: Auto MDI/MDIX mode
 *
 * @retval RT_ERR_OK            The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT  if the specified chip ID is not supported.
 * @retval Others               Please refer to dv1_ethphy_mdix_mode_get(), dva_ethphy_mdix_mode_get(), cvb_ethphy_mdix_mode_get(), or cva_ethphy_mdix_mode_get().
 */
RtkApiRet rtk_ethphy_mdix_mode_get(UnitChip_t unitChip, PhyMdixCfg_t *pPhyMdixCfg)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyMdixCfg)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_mdix_mode_get(unitChip.unit, &(pPhyMdixCfg->mdixMode));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_mdix_mode_get(unitChip.unit, &(pPhyMdixCfg->mdixMode));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_mdix_mode_get(unitChip.unit, &(pPhyMdixCfg->mdixMode));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_mdix_mode_get(unitChip.unit, &(pPhyMdixCfg->mdixMode));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Enable or disable power save mode on the Ethernet port by the PHY control register (PHYCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unit Stacking unit ID (N/A) \n
 *                 The stacking unit ID indicates which switch to access.
 * @param[in] pPhyPwrSave Power Save Configuration information (N/A). \n
 *                        Parameters to enable or disable power save mode on the Ethernet port.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_power_save_en_set(), dva_ethphy_power_save_en_set(), cvb_ethphy_power_save_en_set(), cva_ethphy_power_save_en_set().
 */
RtkApiRet rtk_ethphy_power_save_en_set(UnitChip_t unitChip, PhyPwrSave_t *pPhyPwrSave)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPwrSave)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_power_save_en_set(unitChip.unit, pPhyPwrSave->enPwrSaving);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_power_save_en_set(unitChip.unit, pPhyPwrSave->enPwrSaving);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_power_save_en_set(unitChip.unit, pPhyPwrSave->enPwrSaving);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_power_save_en_set(unitChip.unit, pPhyPwrSave->enPwrSaving);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get power save mode status of the Ethernet port by the PHY control register (PHYCR).\n
 * WARNING: This API is intended for debugging purposes only.
 *
 * @param[in] unit Stacking unit ID (N/A) \n
 *                 The stacking unit ID indicates which switch to access.
 * @param[out] pPhyPwrSave Power Save Configuration information (N/A). \n
 *                         Parameters to get the power save mode status of the Ethernet port.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_ethphy_power_save_en_get(), dva_ethphy_power_save_en_get(), cvb_ethphy_power_save_en_get(), cva_ethphy_power_save_en_get().
 */
RtkApiRet rtk_ethphy_power_save_en_get(UnitChip_t unitChip, PhyPwrSave_t *pPhyPwrSave)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pPhyPwrSave)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_ethphy_power_save_en_get(unitChip.unit, &(pPhyPwrSave->enPwrSaving));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_ethphy_power_save_en_get(unitChip.unit, &(pPhyPwrSave->enPwrSaving));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_ethphy_power_save_en_get(unitChip.unit, &(pPhyPwrSave->enPwrSaving));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_ethphy_power_save_en_get(unitChip.unit, &(pPhyPwrSave->enPwrSaving));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**@}*/ /* PHY_ETH */

/**@}*/ /* RTK_MODULE_PHY */
