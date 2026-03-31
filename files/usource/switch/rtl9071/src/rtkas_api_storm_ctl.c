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
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_storm_ctl.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_storm_ctl.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_storm_ctl.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_storm_ctl.h"
#endif

#include "rtkas_api_storm_ctl.h"

/**
 * @defgroup RTK_MODULE_STORM_CTRL Storm Control
 * @brief Storm Control APIs. Storm control is used to monitor ingress traffic levels and discard broadcast, multicast and unknown unicast packets
 *        when the specific traffic level is exceeded.
 * @{
 */
/**
 * @defgroup STORM_BROADCAST Storm Control Broadcast
 * @brief Broadcast storm control setting for enabling storm control.
 * @{
 */
/**
 * @brief Set broadcast storm control.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stormCtlBroadcastSet Storm control broadcast set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set broadcast storm control successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_broadcast_set(), dva_storm_ctl_broadcast_set(), cvb_storm_ctl_broadcast_set(), cva_storm_ctl_broadcast_set().
 */
RtkApiRet rtk_storm_ctl_broadcast_set(UnitChip_t unitChip, StormCtlBroadcastConfig_t *stormCtlBroadcastSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlBroadcastSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_broadcast_set(unitChip.unit, stormCtlBroadcastSet->port, stormCtlBroadcastSet->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_broadcast_set(unitChip.unit, stormCtlBroadcastSet->port, stormCtlBroadcastSet->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_broadcast_set(unitChip.unit, stormCtlBroadcastSet->port, stormCtlBroadcastSet->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_broadcast_set(unitChip.unit, stormCtlBroadcastSet->port, stormCtlBroadcastSet->enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get broadcast storm control.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stormCtlBroadcastGet Storm control broadcast get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @retval RT_ERR_OK Get broadcast storm control successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_broadcast_get(), dva_storm_ctl_broadcast_get(), cvb_storm_ctl_broadcast_get(), cva_storm_ctl_broadcast_get().
 */
RtkApiRet rtk_storm_ctl_broadcast_get(UnitChip_t unitChip, StormCtlBroadcastConfig_t *stormCtlBroadcastGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlBroadcastGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_broadcast_get(unitChip.unit, stormCtlBroadcastGet->port, &stormCtlBroadcastGet->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_broadcast_get(unitChip.unit, stormCtlBroadcastGet->port, &stormCtlBroadcastGet->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_broadcast_get(unitChip.unit, stormCtlBroadcastGet->port, &stormCtlBroadcastGet->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_broadcast_get(unitChip.unit, stormCtlBroadcastGet->port, &stormCtlBroadcastGet->enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* STORM_BROADCAST */

/**
 * @defgroup STORM_BROADCAST_RATE Storm Control Broadcast Rate
 * @brief Broadcast storm control setting for setting rate.
 * @{
 */
/**
 * @brief Set broadcast storm control rate.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stormCtlBroadcastRateSet Storm control broadcast rate set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set broadcast storm control rate successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_broadcast_rate_set(), dva_storm_ctl_broadcast_rate_set(), cvb_storm_ctl_broadcast_rate_set(), cva_storm_ctl_broadcast_rate_set().
 */
RtkApiRet rtk_storm_ctl_broadcast_rate_set(UnitChip_t unitChip, StormCtlBroadcastRateConfig_t *stormCtlBroadcastRateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlBroadcastRateSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_broadcast_rate_set(unitChip.unit, stormCtlBroadcastRateSet->port, stormCtlBroadcastRateSet->stormUnit, stormCtlBroadcastRateSet->rate);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_broadcast_rate_set(unitChip.unit, stormCtlBroadcastRateSet->port, stormCtlBroadcastRateSet->stormUnit, stormCtlBroadcastRateSet->rate);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_broadcast_rate_set(unitChip.unit, stormCtlBroadcastRateSet->port, stormCtlBroadcastRateSet->stormUnit, stormCtlBroadcastRateSet->rate);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_broadcast_rate_set(unitChip.unit, stormCtlBroadcastRateSet->port, stormCtlBroadcastRateSet->stormUnit, stormCtlBroadcastRateSet->rate);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get broadcast storm control rate.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stormCtlBroadcastRateGet Storm control broadcast rate get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @retval RT_ERR_OK Get broadcast storm control rate successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_broadcast_rate_get(), dva_storm_ctl_broadcast_rate_get(), cvb_storm_ctl_broadcast_rate_get(), cva_storm_ctl_broadcast_rate_get().
 */
RtkApiRet rtk_storm_ctl_broadcast_rate_get(UnitChip_t unitChip, StormCtlBroadcastRateConfig_t *stormCtlBroadcastRateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlBroadcastRateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_broadcast_rate_get(unitChip.unit, stormCtlBroadcastRateGet->port, &stormCtlBroadcastRateGet->stormUnit, &stormCtlBroadcastRateGet->rate);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_broadcast_rate_get(unitChip.unit, stormCtlBroadcastRateGet->port, &stormCtlBroadcastRateGet->stormUnit, &stormCtlBroadcastRateGet->rate);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_broadcast_rate_get(unitChip.unit, stormCtlBroadcastRateGet->port, &stormCtlBroadcastRateGet->stormUnit, &stormCtlBroadcastRateGet->rate);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_broadcast_rate_get(unitChip.unit, stormCtlBroadcastRateGet->port, &stormCtlBroadcastRateGet->stormUnit, &stormCtlBroadcastRateGet->rate);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* STORM_BROADCAST_RATE */

/**
 * @defgroup STORM_MULTICAST Storm Control Multicast
 * @brief Multicast storm control setting for enabling storm control.
 * @{
 */
/**
 * @brief Set multicast storm control.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stormCtlMulticastSet Storm control multicast set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set multicast storm control successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_multicast_set(), dva_storm_ctl_multicast_set(), cvb_storm_ctl_multicast_set(), cva_storm_ctl_multicast_set().
 */
RtkApiRet rtk_storm_ctl_multicast_set(UnitChip_t unitChip, StormCtlMulticastConfig_t *stormCtlMulticastSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlMulticastSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_multicast_set(unitChip.unit, stormCtlMulticastSet->port, stormCtlMulticastSet->enable, stormCtlMulticastSet->type);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_multicast_set(unitChip.unit, stormCtlMulticastSet->port, stormCtlMulticastSet->enable, stormCtlMulticastSet->type);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_multicast_set(unitChip.unit, stormCtlMulticastSet->port, stormCtlMulticastSet->enable, stormCtlMulticastSet->type);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_multicast_set(unitChip.unit, stormCtlMulticastSet->port, stormCtlMulticastSet->enable, stormCtlMulticastSet->type);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get multicast storm control.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stormCtlMulticastGet Storm control multicast get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @retval RT_ERR_OK Get multicast storm control successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_multicast_get(), dva_storm_ctl_multicast_get(), cvb_storm_ctl_multicast_get(), cva_storm_ctl_multicast_get().
 */
RtkApiRet rtk_storm_ctl_multicast_get(UnitChip_t unitChip, StormCtlMulticastConfig_t *stormCtlMulticastGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlMulticastGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_multicast_get(unitChip.unit, stormCtlMulticastGet->port, &stormCtlMulticastGet->enable, &stormCtlMulticastGet->type);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_multicast_get(unitChip.unit, stormCtlMulticastGet->port, &stormCtlMulticastGet->enable, &stormCtlMulticastGet->type);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_multicast_get(unitChip.unit, stormCtlMulticastGet->port, &stormCtlMulticastGet->enable, &stormCtlMulticastGet->type);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_multicast_get(unitChip.unit, stormCtlMulticastGet->port, &stormCtlMulticastGet->enable, &stormCtlMulticastGet->type);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* STORM_MULTICAST */

/**
 * @defgroup STORM_MULTICAST_RATE Storm Control Multicast Rate
 * @brief Multicast storm control setting for setting rate.
 * @{
 */
/**
 * @brief Set multicast storm control rate.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stormCtlMulticastRateSet Storm control multicast rate set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set multicast storm control rate successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_multicast_rate_set(), dva_storm_ctl_multicast_rate_set(), cvb_storm_ctl_multicast_rate_set(), cva_storm_ctl_multicast_rate_set().
 */
RtkApiRet rtk_storm_ctl_multicast_rate_set(UnitChip_t unitChip, StormCtlMulticastRateConfig_t *stormCtlMulticastRateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlMulticastRateSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_multicast_rate_set(unitChip.unit, stormCtlMulticastRateSet->port, stormCtlMulticastRateSet->stormUnit, stormCtlMulticastRateSet->rate);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_multicast_rate_set(unitChip.unit, stormCtlMulticastRateSet->port, stormCtlMulticastRateSet->stormUnit, stormCtlMulticastRateSet->rate);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_multicast_rate_set(unitChip.unit, stormCtlMulticastRateSet->port, stormCtlMulticastRateSet->stormUnit, stormCtlMulticastRateSet->rate);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_multicast_rate_set(unitChip.unit, stormCtlMulticastRateSet->port, stormCtlMulticastRateSet->stormUnit, stormCtlMulticastRateSet->rate);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get multicast storm control rate.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stormCtlMulticastRateGet Storm control multicast rate get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @retval RT_ERR_OK Get multicast storm control rate successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_multicast_rate_get(), dva_storm_ctl_multicast_rate_get(), cvb_storm_ctl_multicast_rate_get(), cva_storm_ctl_multicast_rate_get().
 */
RtkApiRet rtk_storm_ctl_multicast_rate_get(UnitChip_t unitChip, StormCtlMulticastRateConfig_t *stormCtlMulticastRateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlMulticastRateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_multicast_rate_get(unitChip.unit, stormCtlMulticastRateGet->port, &stormCtlMulticastRateGet->stormUnit, &stormCtlMulticastRateGet->rate);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_multicast_rate_get(unitChip.unit, stormCtlMulticastRateGet->port, &stormCtlMulticastRateGet->stormUnit, &stormCtlMulticastRateGet->rate);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_multicast_rate_get(unitChip.unit, stormCtlMulticastRateGet->port, &stormCtlMulticastRateGet->stormUnit, &stormCtlMulticastRateGet->rate);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_multicast_rate_get(unitChip.unit, stormCtlMulticastRateGet->port, &stormCtlMulticastRateGet->stormUnit, &stormCtlMulticastRateGet->rate);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* STORM_MULTICAST_RATE */

/**
 * @defgroup STORM_UNICAST Storm Control Unicast
 * @brief Unicast storm control setting for enabling storm control.
 * @{
 */
/**
 * @brief Set unicast storm control.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stormCtlUnicastSet Storm control unicast set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set unicast storm control successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_unicast_set(), dva_storm_ctl_unicast_set(), cvb_storm_ctl_unicast_set(), cva_storm_ctl_unicast_set().
 */
RtkApiRet rtk_storm_ctl_unicast_set(UnitChip_t unitChip, StormCtlUnicastConfig_t *stormCtlUnicastSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlUnicastSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_unicast_set(unitChip.unit, stormCtlUnicastSet->port, stormCtlUnicastSet->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_unicast_set(unitChip.unit, stormCtlUnicastSet->port, stormCtlUnicastSet->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_unicast_set(unitChip.unit, stormCtlUnicastSet->port, stormCtlUnicastSet->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_unicast_set(unitChip.unit, stormCtlUnicastSet->port, stormCtlUnicastSet->enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get unicast storm control.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stormCtlUnicastGet Storm control unicast get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @retval RT_ERR_OK Get unicast storm control successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_unicast_get(), dva_storm_ctl_unicast_get(), cvb_storm_ctl_unicast_get(), cva_storm_ctl_unicast_get().
 */
RtkApiRet rtk_storm_ctl_unicast_get(UnitChip_t unitChip, StormCtlUnicastConfig_t *stormCtlUnicastGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlUnicastGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_unicast_get(unitChip.unit, stormCtlUnicastGet->port, &stormCtlUnicastGet->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_unicast_get(unitChip.unit, stormCtlUnicastGet->port, &stormCtlUnicastGet->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_unicast_get(unitChip.unit, stormCtlUnicastGet->port, &stormCtlUnicastGet->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_unicast_get(unitChip.unit, stormCtlUnicastGet->port, &stormCtlUnicastGet->enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* STORM_UNICAST */

/**
 * @defgroup STORM_UNICAST_RATE Storm Control Unicast Rate
 * @brief Unicast storm control setting for setting rate.
 * @{
 */
/**
 * @brief Set unicast storm control rate.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stormCtlUnicastRateSet Storm control unicast rate set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set unicast storm control rate successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_unicast_rate_set(), dva_storm_ctl_unicast_rate_set(), cvb_storm_ctl_unicast_rate_set(), cva_storm_ctl_unicast_rate_set().
 */
RtkApiRet rtk_storm_ctl_unicast_rate_set(UnitChip_t unitChip, StormCtlUnicastRateConfig_t *stormCtlUnicastRateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlUnicastRateSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_unicast_rate_set(unitChip.unit, stormCtlUnicastRateSet->port, stormCtlUnicastRateSet->stormUnit, stormCtlUnicastRateSet->rate);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_unicast_rate_set(unitChip.unit, stormCtlUnicastRateSet->port, stormCtlUnicastRateSet->stormUnit, stormCtlUnicastRateSet->rate);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_unicast_rate_set(unitChip.unit, stormCtlUnicastRateSet->port, stormCtlUnicastRateSet->stormUnit, stormCtlUnicastRateSet->rate);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_unicast_rate_set(unitChip.unit, stormCtlUnicastRateSet->port, stormCtlUnicastRateSet->stormUnit, stormCtlUnicastRateSet->rate);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get unicast storm control rate.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stormCtlUnicastRateGet Storm control unicast rate get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_storm_ctl.h files for detailed content.
 * @retval RT_ERR_OK Get unicast storm control rate successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_storm_ctl_unicast_rate_get(), dva_storm_ctl_unicast_rate_get(), cvb_storm_ctl_unicast_rate_get(), cva_storm_ctl_unicast_rate_get().
 */
RtkApiRet rtk_storm_ctl_unicast_rate_get(UnitChip_t unitChip, StormCtlUnicastRateConfig_t *stormCtlUnicastRateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stormCtlUnicastRateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_storm_ctl_unicast_rate_get(unitChip.unit, stormCtlUnicastRateGet->port, &stormCtlUnicastRateGet->stormUnit, &stormCtlUnicastRateGet->rate);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_storm_ctl_unicast_rate_get(unitChip.unit, stormCtlUnicastRateGet->port, &stormCtlUnicastRateGet->stormUnit, &stormCtlUnicastRateGet->rate);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_storm_ctl_unicast_rate_get(unitChip.unit, stormCtlUnicastRateGet->port, &stormCtlUnicastRateGet->stormUnit, &stormCtlUnicastRateGet->rate);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_storm_ctl_unicast_rate_get(unitChip.unit, stormCtlUnicastRateGet->port, &stormCtlUnicastRateGet->stormUnit, &stormCtlUnicastRateGet->rate);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* STORM_UNICAST_RATE */

/**@}*/