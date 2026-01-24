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

#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_types.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_routing.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_routing.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_routing.h"
#endif
#include "rtkas_api_routing.h"

/**
 * @defgroup RTK_MODULE_ROUTING Routing
 * @brief Routing APIs. Routing is the process of determining the path for data packets to travel from a source to a destination across an IP network.
 * @{
 */

/**
 * @defgroup RTK_MODULE_ROUTING_GLB_CTRL Global routing configuration
 * @brief APIs to set global routing configuration
 * @{
 */

/**
 * @brief Enable routing \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] param    Set parameter (N/A) \n
 *                     To turn ON/OFF the routing feature.
 * @param[out] None
 * @retval RT_ERR_OK Enable/Disable routing successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_disable(), cvb_route_init_process_trigger(), dv1_route_disable(), dv1_route_init_process_trigger(), dva_route_disable().
 */
RtkApiRet rtk_route_enable(UnitChip_t unitChip, const RtEnable_t *param)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_disable(unitChip.unit, (ENABLED == param->enable) ? DISABLED : ENABLED);
            CHK_FUN_RET(ret, cvb_route_init_process_trigger(unitChip.unit, param->enable));
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_disable(unitChip.unit, (ENABLED == param->enable) ? DISABLED : ENABLED);
            CHK_FUN_RET(ret, dv1_route_init_process_trigger(unitChip.unit, param->enable));
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_disable(unitChip.unit, (ENABLED == param->enable) ? DISABLED : ENABLED);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Enable/Disable range check function \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] param    Set parameter (N/A) \n
 *                     To turn ON/OFF range check function.
 * @param[out] None
 * @retval RT_ERR_OK Enable/Disable range check successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_rng_chk_set(), dv1_route_rng_chk_set(), dva_route_rng_chk_set().
 */
RtkApiRet rtk_route_rng_chk_set(UnitChip_t unitChip, const RtRngChkSet_t *param)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_rng_chk_set(unitChip.unit, param->enable);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_rng_chk_set(unitChip.unit, param->enable);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_rng_chk_set(unitChip.unit, param->enable);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Flush all routing tables \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Flush tables successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_tbl_flush(), dv1_route_tbl_flush(), dva_route_tbl_flush().
 */
RtkApiRet rtk_route_tbl_flush(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_tbl_flush(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_tbl_flush(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_tbl_flush(unitChip.unit);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**@}*/ /* RTK_MODULE_ROUTING_GLB_CTRL */

/**
 * @defgroup RTK_MODULE_ROUTING_TBL_INTF Routing interface table control
 * @brief APIs to access routing interface table to configure the desired routing gateway.
 * @{
 */

/**
 * @brief Set routing interface table \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] intfCfg  Interface configuration (N/A) \n
 *                     To configure the interface entry.
 * @param[out] None
 * @retval RT_ERR_OK Set interface entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_intf_set(), dv1_route_intf_set(), dva_route_intf_set().
 */
RtkApiRet rtk_route_intf_set(UnitChip_t unitChip, RtIntfCfg_t *intfCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_intf_set(unitChip.unit, intfCfg);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_intf_set(unitChip.unit, intfCfg);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_intf_set(unitChip.unit, intfCfg);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Get routing interface table \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] intfCfg  Interface configuration (N/A) \n
 *                         To get the interface entry.
 * @retval RT_ERR_OK Get interface entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_intf_get(), dv1_route_intf_get(), dva_route_intf_get().
 */
RtkApiRet rtk_route_intf_get(UnitChip_t unitChip, RtIntfCfg_t *intfCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_intf_get(unitChip.unit, intfCfg);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_intf_get(unitChip.unit, intfCfg);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_intf_get(unitChip.unit, intfCfg);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**@}*/ /* RTK_MODULE_ROUTING_TBL_INTF */

/**
 * @defgroup RTK_MODULE_ROUTING_TBL_RT Routing table control
 * @brief APIs for accessing the routing table to configure the desired routing path.
 * @{
 */

/**
 * @brief Set routing unicast entry \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ucEntry Unicast entry configuration (N/A) \n
 *                        To configure the unicast entry.
 * @retval RT_ERR_OK Set unicast entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_uc_set(), dv1_route_uc_set(), dva_route_uc_set().
 */
RtkApiRet rtk_route_uc_set(UnitChip_t unitChip, RtUcEntry_t *ucEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_uc_set(unitChip.unit, ucEntry);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_uc_set(unitChip.unit, ucEntry);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_uc_set(unitChip.unit, ucEntry);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Get routing unicast entry \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ucEntry Unicast entry configuration (N/A) \n
 *                        To get the unicast entry.
 * @retval RT_ERR_OK Get unicast entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_uc_get(), dv1_route_uc_get(), dva_route_uc_get().
 */
RtkApiRet rtk_route_uc_get(UnitChip_t unitChip, RtUcEntry_t *ucEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_uc_get(unitChip.unit, ucEntry);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_uc_get(unitChip.unit, ucEntry);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_uc_get(unitChip.unit, ucEntry);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Set routing multicast entry \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] mcEntry Multicast entry configuration (N/A) \n
 *                        To configure the multicast entry.
 * @retval RT_ERR_OK Set multicast entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_mc_set(), dv1_route_mc_set(), dva_route_mc_set().
 */
RtkApiRet rtk_route_mc_set(UnitChip_t unitChip, RtMcEntry_t *mcEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_mc_set(unitChip.unit, mcEntry);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_mc_set(unitChip.unit, mcEntry);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_mc_set(unitChip.unit, mcEntry);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Get routing multicast entry \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] mcEntry Multicast entry configuration (N/A) \n
 *                        To get the multicast entry.
 * @retval RT_ERR_OK Get multicast entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_mc_get(), dv1_route_mc_get(), dva_route_mc_get().
 */
RtkApiRet rtk_route_mc_get(UnitChip_t unitChip, RtMcEntry_t *mcEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_mc_get(unitChip.unit, mcEntry);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_mc_get(unitChip.unit, mcEntry);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_mc_get(unitChip.unit, mcEntry);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Set subnet for network routing \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] rtEntry Network routing entry (N/A) \n
 *                        To configure routing entry.
 * @retval RT_ERR_OK Set network routing entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_rttbl_set(), dv1_route_rttbl_set(), dva_route_rttbl_set().
 */
RtkApiRet rtk_route_rttbl_set(UnitChip_t unitChip, RtTbl_t *rtEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_rttbl_set(unitChip.unit, rtEntry);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_rttbl_set(unitChip.unit, rtEntry);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_rttbl_set(unitChip.unit, rtEntry);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Get subnet for network routing \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] rtEntry Routing entry (N/A) \n
 *                        Information of the neighbor cache entry.
 * @retval RT_ERR_OK Get netroute entry successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_rttbl_get(), dv1_route_rttbl_get(), dva_route_rttbl_get().
 */
RtkApiRet rtk_route_rttbl_get(UnitChip_t unitChip, RtTbl_t *rtEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_rttbl_get(unitChip.unit, rtEntry);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_rttbl_get(unitChip.unit, rtEntry);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_rttbl_get(unitChip.unit, rtEntry);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Lookup routing table \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] searchInfo Set/Get lookup information (N/A) \n
 *                       To configure lookup keys and get lookup result.
 * @retval -RT_ENTRY_NOT_FOUND Not found available entry.
 * @retval -RT_ENTRY_FOUND Found available entry.
 * @retval Others Please refer to cvb_route_tbl_lookup(), dv1_route_tbl_lookup(), dva_route_tbl_lookup().
 */
RtkApiRet rtk_route_tbl_lookup(UnitChip_t unitChip, RtTableSearchInfo_t *searchInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_tbl_lookup(unitChip.unit, searchInfo);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_tbl_lookup(unitChip.unit, searchInfo);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_tbl_lookup(unitChip.unit, searchInfo);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Get host entry type list \n
 *        Support chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] typeList Host entry type list (N/A) \n
 *                         To get the host entry type list.
 * @retval RT_ERR_OK Get host entry type list successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to cvb_route_host_entry_typelist_get(), dv1_route_host_entry_typelist_get(), dva_route_host_entry_typelist_get().
 */
RtkApiRet rtk_route_host_entry_typelist_get(UnitChip_t unitChip, RtHostTypeList_t *typeList)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_route_host_entry_typelist_get(unitChip.unit, typeList);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_route_host_entry_typelist_get(unitChip.unit, typeList);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_route_host_entry_typelist_get(unitChip.unit, typeList);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}
/**@}*/ /* RTK_MODULE_ROUTING_TBL_RT */
/**@}*/ /* RTK_MODULE_ROUTING */