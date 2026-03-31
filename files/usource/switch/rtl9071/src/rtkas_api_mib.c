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
#include "rtl907xd_v1_api_mib.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_mib.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_mib.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_mib.h"
#endif

#include "rtkas_api_mib.h"

/**
 * @defgroup RTK_MODULE_MIB MIB
 * @brief MIB APIs. Management Information Base (MIB) counter is used to monitor and analyze traffic. The counters classify as per port/global counters and divide into standard/Realtek defined counters.
 * @{
 */
/**
 * @defgroup MIB_ALL_RESET Reset All MIB Counters
 * @brief Reset all ports and global MIB counters.
 * @{
 */
/**
 * @brief Reset all ports and global MIB counters.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Reset all ports and global MIB counters successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_stat_reset_all(), dva_stat_reset_all(), cvb_stat_reset_all(), cva_stat_reset_all().
 */
RtkApiRet rtk_stat_reset_all(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_stat_reset_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_stat_reset_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_stat_reset_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_stat_reset_all(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }

    return ret;
}

/**@}*/ /* MIB_ALL_RESET */

/**
 * @defgroup MIB_ALL_ENABLE Enable All MIB Counters
 * @brief Start all ports and global MIB counters.
 * @{
 */
/**
 * @brief Start all MIB counters.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Start all MIB counters successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_stat_start_all(), dva_stat_start_all(), cvb_stat_start_all(), cva_stat_start_all().
 */
RtkApiRet rtk_stat_start_all(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_stat_start_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_stat_start_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_stat_start_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_stat_start_all(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }

    return ret;
}

/**@}*/ /* MIB_ALL_ENABLE */

/**
 * @defgroup MIB_ALL_DISABLE Disable All MIB Counters
 * @brief Stop all ports and global MIB counters.
 * @{
 */
/**
 * @brief Stop all MIB counters.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Stop all MIB counters successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_stat_stop_all(), dva_stat_stop_all(), cvb_stat_stop_all(), cva_stat_stop_all().
 */
RtkApiRet rtk_stat_stop_all(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_stat_stop_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_stat_stop_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_stat_stop_all(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_stat_stop_all(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }

    return ret;
}

/**@}*/ /* MIB_ALL_DISABLE */

/**
 * @defgroup MIB_PORT_REASON_ALE Get ALE Drop Reason
 * @brief Get the drop reason from the ALE stage.
 * @{
 */
/**
 * @brief Get the ALE drop reason.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] statGetAleDropReason stat get ale drop reason information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @retval RT_ERR_OK Get the ALE drop reason successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stat_get_ale_drop_reason(), dva_stat_get_ale_drop_reason(), cvb_stat_get_ale_drop_reason(), cva_stat_get_ale_drop_reason().
 */
RtkApiRet rtk_stat_get_ale_drop_reason(UnitChip_t unitChip, StatGetAleDropReason_t *statGetAleDropReason)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statGetAleDropReason)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stat_get_ale_drop_reason(unitChip.unit, &statGetAleDropReason->dropReason);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_get_ale_drop_reason(unitChip.unit, &statGetAleDropReason->dropReason);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stat_get_ale_drop_reason(unitChip.unit, &statGetAleDropReason->dropReason);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stat_get_ale_drop_reason(unitChip.unit, &statGetAleDropReason->dropReason);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIB_PORT_REASON_ALE */

/**
 * @defgroup MIB_PORT_QUEUE Get Specific Port RX Max Queue Level
 * @brief Get the maximum queue level by the specified port.
 * @{
 */
/**
 * @brief Get the specified port max queue level.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] statPortMaxQueueLevelGet stat port max queue level get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @retval RT_ERR_OK Get the specified port max queue level successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stat_port_max_queue_level_get(), dva_stat_port_max_queue_level_get(), cvb_stat_port_max_queue_level_get(), cva_stat_port_max_queue_level_get().
 */
RtkApiRet rtk_stat_port_max_queue_level_get(UnitChip_t unitChip, StatPortMaxQueueLevelGet_t *statPortMaxQueueLevelGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortMaxQueueLevelGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stat_port_max_queue_level_get(unitChip.unit, statPortMaxQueueLevelGet->port, &statPortMaxQueueLevelGet->maxQueueLevel);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_max_queue_level_get(unitChip.unit, statPortMaxQueueLevelGet->port, &statPortMaxQueueLevelGet->maxQueueLevel);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stat_port_max_queue_level_get(unitChip.unit, statPortMaxQueueLevelGet->port, &statPortMaxQueueLevelGet->maxQueueLevel);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stat_port_max_queue_level_get(unitChip.unit, statPortMaxQueueLevelGet->port, &statPortMaxQueueLevelGet->maxQueueLevel);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIB_PORT_QUEUE */

/**
 * @defgroup MIB_PORT_RESET Reset Specific Port MIB Counters
 * @brief Reset the MIB counter by the specified port.
 * @{
 */
/**
 * @brief Reset the specified port MIB counter.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  statPortReset stat port reset information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Reset the specified port MIB counter successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stat_port_reset(), dva_stat_port_reset(), cvb_stat_port_reset(), cva_stat_port_reset().
 */
RtkApiRet rtk_stat_port_reset(UnitChip_t unitChip, StatPortConfig_t *statPortReset)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortReset)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stat_port_reset(unitChip.unit, statPortReset->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_reset(unitChip.unit, statPortReset->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stat_port_reset(unitChip.unit, statPortReset->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stat_port_reset(unitChip.unit, statPortReset->port);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIB_PORT_RESET */

/**
 * @defgroup MIB_PORT_ENABLE Enable Specific Port MIB Counters
 * @brief Start the MIB counter by the specified port.
 * @{
 */
/**
 * @brief Start the specified port MIB counter.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  statPortStart stat port start information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Start the specified port MIB counter successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stat_port_start(), dva_stat_port_start(), cvb_stat_port_start(), cva_stat_port_start().
 */
RtkApiRet rtk_stat_port_start(UnitChip_t unitChip, StatPortConfig_t *statPortStart)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortStart)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stat_port_start(unitChip.unit, statPortStart->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_start(unitChip.unit, statPortStart->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stat_port_start(unitChip.unit, statPortStart->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stat_port_start(unitChip.unit, statPortStart->port);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIB_PORT_ENABLE */

/**
 * @defgroup MIB_PORT_DISABLE Disable Specific Port MIB Counters
 * @brief Stop the MIB counter by the specified port.
 * @{
 */
/**
 * @brief Stop the specified port MIB counter.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  statPortStop stat port stop information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Stop the specified port MIB counter successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stat_port_stop(), dva_stat_port_stop(), cvb_stat_port_stop(), cva_stat_port_stop().
 */
RtkApiRet rtk_stat_port_stop(UnitChip_t unitChip, StatPortConfig_t *statPortStop)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortStop)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stat_port_stop(unitChip.unit, statPortStop->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_stop(unitChip.unit, statPortStop->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stat_port_stop(unitChip.unit, statPortStop->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stat_port_stop(unitChip.unit, statPortStop->port);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIB_PORT_DISABLE */

/**
 * @defgroup MIB_PORT_GET Get Specific Port/Global Counter
 * @brief Get the MIB counter by the specified port.
 * @{
 */
/**
 * @brief Get the specified port MIB counter.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] statPortGet stat port get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @retval RT_ERR_OK Get the specified port MIB counter successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stat_port_get(), dva_stat_port_get(), cvb_stat_port_get(), cva_stat_port_get().
 */
RtkApiRet rtk_stat_port_get(UnitChip_t unitChip, StatPortGet_t *statPortGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stat_port_get(unitChip.unit, statPortGet->port, statPortGet->counterType, &statPortGet->counterValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_get(unitChip.unit, statPortGet->port, statPortGet->counterType, &statPortGet->counterValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stat_port_get(unitChip.unit, statPortGet->port, statPortGet->counterType, &statPortGet->counterValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stat_port_get(unitChip.unit, statPortGet->port, statPortGet->counterType, &statPortGet->counterValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIB_PORT_GET */

/**
 * @defgroup MIB_PORT_REASON_RX Get RX Drop Reason
 * @brief Get the RX drop reason by the specified port.
 * @{
 */
/**
 * @brief Get the RX drop reason.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] statPortGetRxDropReason stat port get RX drop reason information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @retval RT_ERR_OK Get the RX drop reason successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_stat_port_get_rx_drop_reason(), dva_stat_port_get_rx_drop_reason(), cvb_stat_port_get_rx_drop_reason(), cva_stat_port_get_rx_drop_reason().
 */
RtkApiRet rtk_stat_port_get_rx_drop_reason(UnitChip_t unitChip, StatPortGetRxDropReason_t *statPortGetRxDropReason)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortGetRxDropReason)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stat_port_get_rx_drop_reason(unitChip.unit, statPortGetRxDropReason->port, &statPortGetRxDropReason->dropReason);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_get_rx_drop_reason(unitChip.unit, statPortGetRxDropReason->port, &statPortGetRxDropReason->dropReason);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stat_port_get_rx_drop_reason(unitChip.unit, statPortGetRxDropReason->port, &statPortGetRxDropReason->dropReason);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stat_port_get_rx_drop_reason(unitChip.unit, statPortGetRxDropReason->port, &statPortGetRxDropReason->dropReason);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* MIB_PORT_REASON_RX */

/**
 * @defgroup MIB_PORT_RATE_CONFIG_SET Set Port Rate Config
 * @brief The port rate feature allows you to monitor the RX and TX port rates at intervals of 0.5, 1, or 2 seconds.
          This enables rate metering with a specified period.
 * @{
 */
/**
 * @brief Set port RX and TX rate configuration.\n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] statPortRateConfigSet stat port rate configuration set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @retval RT_ERR_OK Set the port rate configuration successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dva_stat_port_rate_config_set().
 */
RtkApiRet rtk_stat_port_rate_config_set(UnitChip_t unitChip, StatPortRateConfig_t *statPortRateConfigSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortRateConfigSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_rate_config_set(unitChip.unit, statPortRateConfigSet->port, statPortRateConfigSet->state, statPortRateConfigSet->ratePeriod);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* MIB_PORT_RATE_CONFIG_SET */

/**
 * @defgroup MIB_PORT_RATE_CONFIG_GET Get Port Rate Config
 * @brief The port rate feature allows you to monitor the RX and TX port rates at intervals of 0.5, 1, or 2 seconds.
          This gets the current configuration of rate meter and period.
 * @{
 */
/**
 * @brief Get port RX and TX rate configuration.
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] statPortRateConfigGet stat port rate configuration get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @retval RT_ERR_OK Get the port rate configuration successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dva_stat_port_rate_config_get().
 */
RtkApiRet rtk_stat_port_rate_config_get(UnitChip_t unitChip, StatPortRateConfig_t *statPortRateConfigGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortRateConfigGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_rate_config_get(unitChip.unit, statPortRateConfigGet->port, &statPortRateConfigGet->state, &statPortRateConfigGet->ratePeriod);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIB_PORT_RATE_CONFIG_GET */

/**
 * @defgroup MIB_PORT_RATE_Get Get Port Rate
 * @brief Get the current RX or TX port rate for the specified port based on the port rate configuration.
 * @{
 */
/**
 * @brief Get the port current RX and TX rate.
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] statPortRateGet stat port rate RX and TX get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_mib.h files for detailed content.
 * @retval RT_ERR_OK Get the port rate successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dva_stat_port_rate_get().
 */
RtkApiRet rtk_stat_port_rate_get(UnitChip_t unitChip, StatPortRateGet_t *statPortRateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == statPortRateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stat_port_rate_get(unitChip.unit, statPortRateGet->port, &statPortRateGet->rxRate, &statPortRateGet->txRate);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* MIB_PORT_RATE_Get */

/**@}*/
