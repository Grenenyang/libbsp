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
#include "rtl907xd_v1_api_stp.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_stp.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_stp.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_stp.h"
#endif

#include "rtkas_api_stp.h"

/**
 * @defgroup RTK_MODULE_STP STP
 * @brief STP APIs. Spanning Tree Protocol (STP) and Rapid Spanning Tree Protocol (RSTP) are used to prevent network loops by creating a loop-free topology in Ethernet networks.
 * @{
 */
/**
 * @defgroup STP_BRIDGE_CONFIGURATION Bridge Configuration
 * @brief STP/RSTP bridge configuration for global settings.
 * @{
 */
/**
 * @brief This API is used to enable or disable STP.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpStateSet STP state set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set STP state successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_state_set(), dva_stp_state_set(), cvb_stp_state_set(), cva_stp_state_set().
 */
RtkApiRet rtk_stp_state_set(UnitChip_t unitChip, StpStateConfig_t *stpStateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpStateSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_state_set(unitChip.unit, stpStateSet->stpState);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_state_set(unitChip.unit, stpStateSet->stpState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_state_set(unitChip.unit, stpStateSet->stpState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_state_set(unitChip.unit, stpStateSet->stpState);
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
 * @brief This API is used to check whether STP is enabled or disabled.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] stpStateGet STP state get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get STP state successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_state_get(), dva_stp_state_get(), cvb_stp_state_get(), cva_stp_state_get().
 */
RtkApiRet rtk_stp_state_get(UnitChip_t unitChip, StpStateConfig_t *stpStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpStateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_state_get(unitChip.unit, &stpStateGet->stpState);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_state_get(unitChip.unit, &stpStateGet->stpState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_state_get(unitChip.unit, &stpStateGet->stpState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_state_get(unitChip.unit, &stpStateGet->stpState);
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
 * @brief This API is used to switch STP protocol between STP and RSTP.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpTypeSet STP type set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set STP type successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_type_set(), dva_stp_type_set(), cvb_stp_type_set(), cva_stp_type_set().
 */
RtkApiRet rtk_stp_type_set(UnitChip_t unitChip, StpTypeConfig_t *stpTypeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpTypeSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_type_set(unitChip.unit, stpTypeSet->stpType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_type_set(unitChip.unit, stpTypeSet->stpType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_type_set(unitChip.unit, stpTypeSet->stpType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_type_set(unitChip.unit, stpTypeSet->stpType);
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
 * @brief This API is used to get spanning tree type.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] stpTypeGet STP type get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get STP type successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_type_get(), dva_stp_type_get(), cvb_stp_type_get(), cva_stp_type_get().
 */
RtkApiRet rtk_stp_type_get(UnitChip_t unitChip, StpTypeConfig_t *stpTypeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpTypeGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_type_get(unitChip.unit, &stpTypeGet->stpType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_type_get(unitChip.unit, &stpTypeGet->stpType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_type_get(unitChip.unit, &stpTypeGet->stpType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_type_get(unitChip.unit, &stpTypeGet->stpType);
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
 * @brief This API is used to set switch priority.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpBridgePrioritySet STP bridge priority set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set switch priority successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_bridge_priority_set(), dva_stp_bridge_priority_set(), cvb_stp_bridge_priority_set(), cva_stp_bridge_priority_set().
 */
RtkApiRet rtk_stp_bridge_priority_set(UnitChip_t unitChip, StpBridgePriorityConfig_t *stpBridgePrioritySet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpBridgePrioritySet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_bridge_priority_set(unitChip.unit, stpBridgePrioritySet->stpBridgePriority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_bridge_priority_set(unitChip.unit, stpBridgePrioritySet->stpBridgePriority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_bridge_priority_set(unitChip.unit, stpBridgePrioritySet->stpBridgePriority);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_bridge_priority_set(unitChip.unit, stpBridgePrioritySet->stpBridgePriority);
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
 * @brief This API is used to get switch priority.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] stpBridgePriorityGet STP bridge priority get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get switch priority successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_bridge_priority_get(), dva_stp_bridge_priority_get(), cvb_stp_bridge_priority_get(), cva_stp_bridge_priority_get().
 */
RtkApiRet rtk_stp_bridge_priority_get(UnitChip_t unitChip, StpBridgePriorityConfig_t *stpBridgePriorityGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpBridgePriorityGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_bridge_priority_get(unitChip.unit, &stpBridgePriorityGet->stpBridgePriority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_bridge_priority_get(unitChip.unit, &stpBridgePriorityGet->stpBridgePriority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_bridge_priority_get(unitChip.unit, &stpBridgePriorityGet->stpBridgePriority);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_bridge_priority_get(unitChip.unit, &stpBridgePriorityGet->stpBridgePriority);
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
 * @brief This API is used to set forward delay.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpForwardDelaySet STP forward delay set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set forward delay successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_forward_delay_set(), dva_stp_forward_delay_set(), cvb_stp_forward_delay_set(), cva_stp_forward_delay_set().
 */
RtkApiRet rtk_stp_forward_delay_set(UnitChip_t unitChip, StpForwardDelayConfig_t *stpForwardDelaySet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpForwardDelaySet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_forward_delay_set(unitChip.unit, stpForwardDelaySet->stpForwardDelay);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_forward_delay_set(unitChip.unit, stpForwardDelaySet->stpForwardDelay);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_forward_delay_set(unitChip.unit, stpForwardDelaySet->stpForwardDelay);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_forward_delay_set(unitChip.unit, stpForwardDelaySet->stpForwardDelay);
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
 * @brief This API is used to get forward delay.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] stpForwardDelayGet STP forward delay get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get forward delay successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_forward_delay_get(), dva_stp_forward_delay_get(), cvb_stp_forward_delay_get(), cva_stp_forward_delay_get().
 */
RtkApiRet rtk_stp_forward_delay_get(UnitChip_t unitChip, StpForwardDelayConfig_t *stpForwardDelayGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpForwardDelayGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_forward_delay_get(unitChip.unit, &stpForwardDelayGet->stpForwardDelay);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_forward_delay_get(unitChip.unit, &stpForwardDelayGet->stpForwardDelay);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_forward_delay_get(unitChip.unit, &stpForwardDelayGet->stpForwardDelay);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_forward_delay_get(unitChip.unit, &stpForwardDelayGet->stpForwardDelay);
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
 * @brief This API is used to set max age time.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpMaxAgeSet STP max age set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set max age time successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_max_age_set(), dva_stp_max_age_set(), cvb_stp_max_age_set(), cva_stp_max_age_set().
 */
RtkApiRet rtk_stp_max_age_set(UnitChip_t unitChip, StpMaxAgeConfig_t *stpMaxAgeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpMaxAgeSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_max_age_set(unitChip.unit, stpMaxAgeSet->stpMaxAge);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_max_age_set(unitChip.unit, stpMaxAgeSet->stpMaxAge);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_max_age_set(unitChip.unit, stpMaxAgeSet->stpMaxAge);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_max_age_set(unitChip.unit, stpMaxAgeSet->stpMaxAge);
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
 * @brief This API is used to get max age.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] stpMaxAgeGet STP max age get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get max age time successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_max_age_get(), dva_stp_max_age_get(), cvb_stp_max_age_get(), cva_stp_max_age_get().
 */
RtkApiRet rtk_stp_max_age_get(UnitChip_t unitChip, StpMaxAgeConfig_t *stpMaxAgeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpMaxAgeGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_max_age_get(unitChip.unit, &stpMaxAgeGet->stpMaxAge);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_max_age_get(unitChip.unit, &stpMaxAgeGet->stpMaxAge);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_max_age_get(unitChip.unit, &stpMaxAgeGet->stpMaxAge);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_max_age_get(unitChip.unit, &stpMaxAgeGet->stpMaxAge);
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
 * @brief This API is used to set hello time.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpHelloTimeSet STP hello time set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set hello time successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_hello_time_set(), dva_stp_hello_time_set(), cvb_stp_hello_time_set(), cva_stp_hello_time_set().
 */
RtkApiRet rtk_stp_hello_time_set(UnitChip_t unitChip, StpHelloTimeConfig_t *stpHelloTimeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpHelloTimeSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_hello_time_set(unitChip.unit, stpHelloTimeSet->stpHelloTime);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_hello_time_set(unitChip.unit, stpHelloTimeSet->stpHelloTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_hello_time_set(unitChip.unit, stpHelloTimeSet->stpHelloTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_hello_time_set(unitChip.unit, stpHelloTimeSet->stpHelloTime);
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
 * @brief This API is used to get hello time.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] stpHelloTimeGet STP hello time get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get hello time successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_hello_time_get(), dva_stp_hello_time_get(), cvb_stp_hello_time_get(), cva_stp_hello_time_get().
 */
RtkApiRet rtk_stp_hello_time_get(UnitChip_t unitChip, StpHelloTimeConfig_t *stpHelloTimeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpHelloTimeGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_hello_time_get(unitChip.unit, &stpHelloTimeGet->stpHelloTime);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_hello_time_get(unitChip.unit, &stpHelloTimeGet->stpHelloTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_hello_time_get(unitChip.unit, &stpHelloTimeGet->stpHelloTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_hello_time_get(unitChip.unit, &stpHelloTimeGet->stpHelloTime);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* STP_BRIDGE_CONFIGURATION */

/**
 * @defgroup STP_PORT_CONFIGURATION Port Configuration
 * @brief STP/RSTP port configuration for per-port settings.
 * @{
 */
/**
 * @brief This API is used to set spanning tree port states by instance.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpPortStateSet STP port state set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set port state by instance successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_state_set(), dva_stp_port_state_set(), cvb_stp_port_state_set(), cva_stp_port_state_set().
 */
RtkApiRet rtk_stp_port_state_set(UnitChip_t unitChip, StpPortStateConfig_t *stpPortStateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortStateSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_state_set(unitChip.unit, stpPortStateSet->msti, stpPortStateSet->portId, stpPortStateSet->portState);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_state_set(unitChip.unit, stpPortStateSet->msti, stpPortStateSet->portId, stpPortStateSet->portState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_state_set(unitChip.unit, stpPortStateSet->msti, stpPortStateSet->portId, stpPortStateSet->portState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_state_set(unitChip.unit, stpPortStateSet->msti, stpPortStateSet->portId, stpPortStateSet->portState);
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
 * @brief This API is used to get spanning tree port states by instance.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stpPortStateGet STP port state get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get port state by instance successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_state_get(), dva_stp_port_state_get(), cvb_stp_port_state_get(), cva_stp_port_state_get().
 */
RtkApiRet rtk_stp_port_state_get(UnitChip_t unitChip, StpPortStateConfig_t *stpPortStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortStateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_state_get(unitChip.unit, stpPortStateGet->msti, stpPortStateGet->portId, &stpPortStateGet->portState);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_state_get(unitChip.unit, stpPortStateGet->msti, stpPortStateGet->portId, &stpPortStateGet->portState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_state_get(unitChip.unit, stpPortStateGet->msti, stpPortStateGet->portId, &stpPortStateGet->portState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_state_get(unitChip.unit, stpPortStateGet->msti, stpPortStateGet->portId, &stpPortStateGet->portState);
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
 * @brief This API is used to set port priority.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpPortPrioritySet STP port priority set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set port priority successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_priority_set(), dva_stp_port_priority_set(), cvb_stp_port_priority_set(), cva_stp_port_priority_set().
 */
RtkApiRet rtk_stp_port_priority_set(UnitChip_t unitChip, StpPortPriorityConfig_t *stpPortPrioritySet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortPrioritySet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_priority_set(unitChip.unit, stpPortPrioritySet->portId, stpPortPrioritySet->stpPortPriority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_priority_set(unitChip.unit, stpPortPrioritySet->portId, stpPortPrioritySet->stpPortPriority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_priority_set(unitChip.unit, stpPortPrioritySet->portId, stpPortPrioritySet->stpPortPriority);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_priority_set(unitChip.unit, stpPortPrioritySet->portId, stpPortPrioritySet->stpPortPriority);
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
 * @brief This API is used to get port priority.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stpPortPriorityGet STP port priority get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get port priority successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_priority_get(), dva_stp_port_priority_get(), cvb_stp_port_priority_get(), cva_stp_port_priority_get().
 */
RtkApiRet rtk_stp_port_priority_get(UnitChip_t unitChip, StpPortPriorityConfig_t *stpPortPriorityGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortPriorityGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_priority_get(unitChip.unit, stpPortPriorityGet->portId, &stpPortPriorityGet->stpPortPriority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_priority_get(unitChip.unit, stpPortPriorityGet->portId, &stpPortPriorityGet->stpPortPriority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_priority_get(unitChip.unit, stpPortPriorityGet->portId, &stpPortPriorityGet->stpPortPriority);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_priority_get(unitChip.unit, stpPortPriorityGet->portId, &stpPortPriorityGet->stpPortPriority);
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
 * @brief This API is used to set port path cost.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpPortPathCostSet STP port path cost set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set port path cost successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_path_cost_set(), dva_stp_port_path_cost_set(), cvb_stp_port_path_cost_set(), cva_stp_port_path_cost_set().
 */
RtkApiRet rtk_stp_port_path_cost_set(UnitChip_t unitChip, StpPortPathCostConfig_t *stpPortPathCostSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortPathCostSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_path_cost_set(unitChip.unit, stpPortPathCostSet->portId, stpPortPathCostSet->stpPortPathCost);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_path_cost_set(unitChip.unit, stpPortPathCostSet->portId, stpPortPathCostSet->stpPortPathCost);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_path_cost_set(unitChip.unit, stpPortPathCostSet->portId, stpPortPathCostSet->stpPortPathCost);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_path_cost_set(unitChip.unit, stpPortPathCostSet->portId, stpPortPathCostSet->stpPortPathCost);
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
 * @brief This API is used to get port path cost.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stpPortPathCostGet STP port path cost get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get port path cost successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_path_cost_get(), dva_stp_port_path_cost_get(), cvb_stp_port_path_cost_get(), cva_stp_port_path_cost_get().
 */
RtkApiRet rtk_stp_port_path_cost_get(UnitChip_t unitChip, StpPortPathCostConfig_t *stpPortPathCostGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortPathCostGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_path_cost_get(unitChip.unit, stpPortPathCostGet->portId, &stpPortPathCostGet->stpPortPathCost);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_path_cost_get(unitChip.unit, stpPortPathCostGet->portId, &stpPortPathCostGet->stpPortPathCost);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_path_cost_get(unitChip.unit, stpPortPathCostGet->portId, &stpPortPathCostGet->stpPortPathCost);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_path_cost_get(unitChip.unit, stpPortPathCostGet->portId, &stpPortPathCostGet->stpPortPathCost);
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
 * @brief This API is used to set port admin edge.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpPortAdminEdgeSet STP port admin edge set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set port admin edge successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_admin_edge_set(), dva_stp_port_admin_edge_set(), cvb_stp_port_admin_edge_set(), cva_stp_port_admin_edge_set().
 */
RtkApiRet rtk_stp_port_admin_edge_set(UnitChip_t unitChip, StpPortAdminEdgeConfig_t *stpPortAdminEdgeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortAdminEdgeSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_admin_edge_set(unitChip.unit, stpPortAdminEdgeSet->portId, stpPortAdminEdgeSet->stpPortAdminEdge);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_admin_edge_set(unitChip.unit, stpPortAdminEdgeSet->portId, stpPortAdminEdgeSet->stpPortAdminEdge);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_admin_edge_set(unitChip.unit, stpPortAdminEdgeSet->portId, stpPortAdminEdgeSet->stpPortAdminEdge);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_admin_edge_set(unitChip.unit, stpPortAdminEdgeSet->portId, stpPortAdminEdgeSet->stpPortAdminEdge);
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
 * @brief This API is used to get port admin edge.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stpPortAdminEdgeGet STP port admin edge get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get port admin edge successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_admin_edge_get(), dva_stp_port_admin_edge_get(), cvb_stp_port_admin_edge_get(), cva_stp_port_admin_edge_get().
 */
RtkApiRet rtk_stp_port_admin_edge_get(UnitChip_t unitChip, StpPortAdminEdgeConfig_t *stpPortAdminEdgeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortAdminEdgeGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_admin_edge_get(unitChip.unit, stpPortAdminEdgeGet->portId, &stpPortAdminEdgeGet->stpPortAdminEdge);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_admin_edge_get(unitChip.unit, stpPortAdminEdgeGet->portId, &stpPortAdminEdgeGet->stpPortAdminEdge);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_admin_edge_get(unitChip.unit, stpPortAdminEdgeGet->portId, &stpPortAdminEdgeGet->stpPortAdminEdge);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_admin_edge_get(unitChip.unit, stpPortAdminEdgeGet->portId, &stpPortAdminEdgeGet->stpPortAdminEdge);
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
 * @brief This API is used to set port auto edge.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpPortAutoEdgeSet STP port auto edge set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set port auto edge successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_auto_edge_set(), dva_stp_port_auto_edge_set(), cvb_stp_port_auto_edge_set(), cva_stp_port_auto_edge_set().
 */
RtkApiRet rtk_stp_port_auto_edge_set(UnitChip_t unitChip, StpPortAutoEdgeConfig_t *stpPortAutoEdgeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortAutoEdgeSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_auto_edge_set(unitChip.unit, stpPortAutoEdgeSet->portId, stpPortAutoEdgeSet->stpPortAutoEdge);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_auto_edge_set(unitChip.unit, stpPortAutoEdgeSet->portId, stpPortAutoEdgeSet->stpPortAutoEdge);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_auto_edge_set(unitChip.unit, stpPortAutoEdgeSet->portId, stpPortAutoEdgeSet->stpPortAutoEdge);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_auto_edge_set(unitChip.unit, stpPortAutoEdgeSet->portId, stpPortAutoEdgeSet->stpPortAutoEdge);
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
 * @brief This API is used to get port auto edge.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stpPortAutoEdgeGet STP port auto edge get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get port auto edge successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_auto_edge_get(), dva_stp_port_auto_edge_get(), cvb_stp_port_auto_edge_get(), cva_stp_port_auto_edge_get().
 */
RtkApiRet rtk_stp_port_auto_edge_get(UnitChip_t unitChip, StpPortAutoEdgeConfig_t *stpPortAutoEdgeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortAutoEdgeGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_auto_edge_get(unitChip.unit, stpPortAutoEdgeGet->portId, &stpPortAutoEdgeGet->stpPortAutoEdge);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_auto_edge_get(unitChip.unit, stpPortAutoEdgeGet->portId, &stpPortAutoEdgeGet->stpPortAutoEdge);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_auto_edge_get(unitChip.unit, stpPortAutoEdgeGet->portId, &stpPortAutoEdgeGet->stpPortAutoEdge);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_auto_edge_get(unitChip.unit, stpPortAutoEdgeGet->portId, &stpPortAutoEdgeGet->stpPortAutoEdge);
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
 * @brief This API is used to set port p2p.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpPortP2pSet STP port p2p set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set port p2p successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_p2p_set(), dva_stp_port_p2p_set(), cvb_stp_port_p2p_set(), cva_stp_port_p2p_set().
 */
RtkApiRet rtk_stp_port_p2p_set(UnitChip_t unitChip, StpPortP2PConfig_t *stpPortP2pSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortP2pSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_p2p_set(unitChip.unit, stpPortP2pSet->portId, stpPortP2pSet->stpPortP2p);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_p2p_set(unitChip.unit, stpPortP2pSet->portId, stpPortP2pSet->stpPortP2p);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_p2p_set(unitChip.unit, stpPortP2pSet->portId, stpPortP2pSet->stpPortP2p);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_p2p_set(unitChip.unit, stpPortP2pSet->portId, stpPortP2pSet->stpPortP2p);
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
 * @brief This API is used to get port p2p attribution.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stpPortP2pGet STP port p2p get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @retval RT_ERR_OK Get port p2p successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_p2p_get(), dva_stp_port_p2p_get(), cvb_stp_port_p2p_get(), cva_stp_port_p2p_get().
 */
RtkApiRet rtk_stp_port_p2p_get(UnitChip_t unitChip, StpPortP2PConfig_t *stpPortP2pGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortP2pGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_p2p_get(unitChip.unit, stpPortP2pGet->portId, &stpPortP2pGet->stpPortP2p);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_p2p_get(unitChip.unit, stpPortP2pGet->portId, &stpPortP2pGet->stpPortP2p);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_p2p_get(unitChip.unit, stpPortP2pGet->portId, &stpPortP2pGet->stpPortP2p);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_p2p_get(unitChip.unit, stpPortP2pGet->portId, &stpPortP2pGet->stpPortP2p);
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
 * @brief This API is used to trigger port migration check.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stpPortMigrationCheckSet STP port migration check set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_stp.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Trigger port migration check successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stp_port_migration_check_set(), dva_stp_port_migration_check_set(), cvb_stp_port_migration_check_set(), cva_stp_port_migration_check_set().
 */
RtkApiRet rtk_stp_port_migration_check_set(UnitChip_t unitChip, StpPortMigrationCheckSet_t *stpPortMigrationCheckSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stpPortMigrationCheckSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stp_port_migration_check_set(unitChip.unit, stpPortMigrationCheckSet->portId);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stp_port_migration_check_set(unitChip.unit, stpPortMigrationCheckSet->portId);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stp_port_migration_check_set(unitChip.unit, stpPortMigrationCheckSet->portId);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stp_port_migration_check_set(unitChip.unit, stpPortMigrationCheckSet->portId);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* STP_PORT_CONFIGURATION */

/**@}*/