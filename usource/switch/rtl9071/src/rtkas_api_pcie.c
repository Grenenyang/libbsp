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
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_pcie.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_pcie.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_pcie.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_pcie.h"
#endif

#include "rtkas_api_pcie.h"

/**
 * @defgroup RTK_MODULE_PCIE PCIE
 * @brief PCIE APIs. This component is used to configure PCIE paramters, link speed, or lane.
 * @{
 */

/**
 * @defgroup PCIE_PARM PCIE Parameters
 * @brief Get the PCIE port MACID, SMIDSVID and Serial Number.
 * @{
 */

/**
 * @brief Get the PCIE port MACID, SMIDSVID and Serial Number.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pcieParam Get PCIE port parameters (N/A) \n
 *             Contain MACID, SMIDSVID and Serial Number.
 * @retval RT_ERR_OK The PCIE parameters have been read from OTP.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_pcie_parm_get(), dva_pcie_parm_get(), cvb_pcie_parm_get(), cva_pcie_parm_get().
 */
RtkApiRet rtk_pcie_parm_get(UnitChip_t unitChip, PcieParm_t *pcieParam)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_pcie_parm_get(unitChip.unit, pcieParam->pciePort, pcieParam);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_pcie_parm_get(unitChip.unit, pcieParam->pciePort, pcieParam);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_pcie_parm_get(unitChip.unit, pcieParam->pciePort, pcieParam);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_pcie_parm_get(unitChip.unit, pcieParam->pciePort, pcieParam);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**@}*/ /*PCIE_PARM*/

/**
 * @defgroup PCIE_SPEED PCIE Link Speed
 * @brief Get PCIE Link Speed
 * @{
 */

/**
 * @brief Get PCIE port link speed .
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pcieLinkspeed PCIE port link speed get information (N/A) \n
 *             Link speed used for the PCIE port.
 * @retval RT_ERR_OK The PCIE port link speed has been read from the register.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_pcie_linkspeed_get(), dva_pcie_linkspeed_get(), cvb_pcie_linkspeed_get(), cva_pcie_linkspeed_get().
 */
RtkApiRet rtk_pcie_linkspeed_get(UnitChip_t unitChip, PcieLinkspeed_t *pcieLinkspeed)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_pcie_linkspeed_get(unitChip.unit, pcieLinkspeed->pciePort, &pcieLinkspeed->linkSpeed);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_pcie_linkspeed_get(unitChip.unit, pcieLinkspeed->pciePort, &pcieLinkspeed->linkSpeed);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_pcie_linkspeed_get(unitChip.unit, pcieLinkspeed->pciePort, &pcieLinkspeed->linkSpeed);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_pcie_linkspeed_get(unitChip.unit, pcieLinkspeed->pciePort, &pcieLinkspeed->linkSpeed);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**@}*/ /*PCIE_SPEED*/

/**
 * @defgroup PCIE_LANE PCIE Lanes
 * @brief Get number of PCIE Lanes
 * @{
 */

/**
 * @brief Get the number of PCIE lanes.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pcieLaneInfo PCIE lane get information (N/A) \n
 *             Number of lanes used for the PCIE port.
 * @retval RT_ERR_OK The number of PCIE port lanes has been read from the register.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_pcie_lane_get(), dva_pcie_lane_get().
 */
RtkApiRet rtk_pcie_lane_get(UnitChip_t unitChip, PcieLaneInfo_t *pcieLaneInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_pcie_lane_get(unitChip.unit, pcieLaneInfo->pciePort, &pcieLaneInfo->pcie2LaneEn);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_pcie_lane_get(unitChip.unit, pcieLaneInfo->pciePort, &pcieLaneInfo->pcie2LaneEn);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**@}*/ /*PCIE_LANE*/

/**
 * @defgroup PCIE_PTM PCIE PTM Sync to the Switch
 * @brief Get the information about the PCIE PTM Sync to the Switch.
 * @{
 */

/**
 * @brief Get the PCIE port for PTM Sync to the Switch.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] *pciePtmInfo Information about the PCIE PTM Sync to the Switch (N/A) \n
 *             PCIE port for PTM Sync to the Switch.
 * @retval RT_ERR_OK  The PCIE port for PTM Sync to the Switch has been read from the register.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_pcie_ptm_sync_info_get(), dva_pcie_ptm_info_port_get().
 */
RtkApiRet rtk_pcie_ptm_sync_info_get(UnitChip_t unitChip, PciePtmInfo_t *pciePtmInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_pcie_ptm_sync_info_get(unitChip.unit, pciePtmInfo->pciePort, &pciePtmInfo->pciePtmSync);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_pcie_ptm_sync_info_get(unitChip.unit, pciePtmInfo->pciePort, &pciePtmInfo->pciePtmSync);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**@}*/ /*PCIE_PTM*/
/**@}*/ /* PCIE */
