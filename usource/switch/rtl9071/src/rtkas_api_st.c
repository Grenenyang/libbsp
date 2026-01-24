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

#include "rtkas_api_ext.h"
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_st.h"
#include "rtl907xd_va_api.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_st.h"
#include "rtl907xc_vb_api.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_st.h"
#include "rtl907xc_va_api.h"
#endif

#include "rtkas_api_st.h"
/**
 * @defgroup RTK_MODULE_ST_QBV ST/QBV
 * @brief ST APIs. This section details the functions for configuring maximum queue SDU, admin gate state and admin configuration to enhance schedule traffic.
 * @{
 */
/**
 * @defgroup QUEUEMAXSDU QueueMaxSDU
 * @brief ST/QBV queueMaxSDU configuration
 * @{
 */
/**
 * @brief Set the maximum queue SDU on a specific port using the provided input parameters. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stQueueMaxSduInfo ST queue maximum queue SDU information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the maximum queue SDU on a specific port using the provided input parameters.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_queue_max_sdu_set(), cvb_st_queue_max_sdu_set(), cva_st_queue_max_sdu_set().
 */
RtkApiRet rtk_st_queue_max_sdu_set(UnitChip_t unitChip, StQueueMaxSduInfo_t *stQueueMaxSduInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stQueueMaxSduInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_queue_max_sdu_set(unitChip.unit, (dva_RtkPort)stQueueMaxSduInfo->portId, stQueueMaxSduInfo->queueMaxSdu);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_queue_max_sdu_set(unitChip.unit, (cvb_RtkPort)stQueueMaxSduInfo->portId, stQueueMaxSduInfo->queueMaxSdu);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_queue_max_sdu_set(unitChip.unit, (cva_RtkPort)stQueueMaxSduInfo->portId, stQueueMaxSduInfo->queueMaxSdu);
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
 * @brief Get the maximum queue SDU on a specific port using the provided input parameters. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stQueueMaxSduInfo ST queue max SDU get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the maximum queue SDU on a specific port using the provided input parameters.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_queue_max_sdu_get(), cvb_st_queue_max_sdu_get(), cva_st_queue_max_sdu_get().
 */
RtkApiRet rtk_st_queue_max_sdu_get(UnitChip_t unitChip, StQueueMaxSduInfo_t *stQueueMaxSduInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stQueueMaxSduInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_queue_max_sdu_get(unitChip.unit, (dva_RtkPort)stQueueMaxSduInfo->portId, stQueueMaxSduInfo->queueMaxSdu);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_queue_max_sdu_get(unitChip.unit, (cvb_RtkPort)stQueueMaxSduInfo->portId, stQueueMaxSduInfo->queueMaxSdu);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_queue_max_sdu_get(unitChip.unit, (cva_RtkPort)stQueueMaxSduInfo->portId, stQueueMaxSduInfo->queueMaxSdu);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* QUEUEMAXSDU */
/**
 * @defgroup TRANSMISSION_GATE Transmission Gate
 * @brief ST/QBV transmission gate configuration
 * @{
 */
/**
 * @brief Initialize the ST to IEEE 802.1Q recommended value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully initializes the ST to IEEE 802.1Q recommended value.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_st_init(), cvb_st_init(), cva_st_init().
 */
RtkApiRet rtk_st_init(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_st_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_st_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_st_init(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Set the Byte-Count related to the internal TX-interval parameter.  \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stByteCountInfo ST byte count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the Byte-Count related to the internal TX-interval parameter.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_byte_count_set(), cvb_st_byte_count_set(), cva_st_byte_count_set().
 */
RtkApiRet rtk_st_byte_count_set(UnitChip_t unitChip, StByteCountSet_t *stByteCountInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stByteCountInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_byte_count_set(unitChip.unit, (dva_RtkPort)stByteCountInfo->portId, stByteCountInfo->portType,
                                            stByteCountInfo->speed, stByteCountInfo->macsecIPG);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_byte_count_set(unitChip.unit, (cvb_RtkPort)stByteCountInfo->portId, stByteCountInfo->portType,
                                            stByteCountInfo->speed, stByteCountInfo->macsecIPG);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_byte_count_set(unitChip.unit, (cva_RtkPort)stByteCountInfo->portId, stByteCountInfo->portType,
                                            stByteCountInfo->speed, stByteCountInfo->macsecIPG);
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
 * @brief Get the Byte-Count related to the internal TX-interval parameter. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stByteCountInfo ST byte count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the Byte-Count related to the internal TX-interval parameter.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_byte_count_get(), cvb_st_byte_count_get(), cva_st_byte_count_get().
 */
RtkApiRet rtk_st_byte_count_get(UnitChip_t unitChip, StByteCountGet_t *stByteCountInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stByteCountInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_byte_count_get(unitChip.unit, (dva_RtkPort)stByteCountInfo->portId, &stByteCountInfo->byteCount);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_byte_count_get(unitChip.unit, (cvb_RtkPort)stByteCountInfo->portId, &stByteCountInfo->byteCount);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_byte_count_get(unitChip.unit, (cva_RtkPort)stByteCountInfo->portId, &stByteCountInfo->byteCount);
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
 * @brief To enable/disable the transmission gate on a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stGateEnableInfo ST gate enable status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the transmission gate enable state on a specific port.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_gate_enable_set(), cvb_st_gate_enable_set(), cva_st_gate_enable_set().
 */
RtkApiRet rtk_st_gate_enable_set(UnitChip_t unitChip, StGateEnableInfo_t *stGateEnableInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stGateEnableInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_gate_enable_set(unitChip.unit, stGateEnableInfo->portId, stGateEnableInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_gate_enable_set(unitChip.unit, stGateEnableInfo->portId, stGateEnableInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_gate_enable_set(unitChip.unit, stGateEnableInfo->portId, stGateEnableInfo->enable);
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
 * @brief Get the transmission gate state on a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stGateEnableInfo ST gate enable status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the transmission gate state on a specific port.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_gate_enable_get(), cvb_st_gate_enable_get(), cva_st_gate_enable_get().
 */
RtkApiRet rtk_st_gate_enable_get(UnitChip_t unitChip, StGateEnableInfo_t *stGateEnableInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stGateEnableInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_gate_enable_get(unitChip.unit, (dva_RtkPort)stGateEnableInfo->portId, &stGateEnableInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_gate_enable_get(unitChip.unit, (cvb_RtkPort)stGateEnableInfo->portId, &stGateEnableInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_gate_enable_get(unitChip.unit, (cva_RtkPort)stGateEnableInfo->portId, &stGateEnableInfo->enable);
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
 * @brief Configure the transmission gate on a specific port using the provided input parameter. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stGateConfigInfo ST gate configuration information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Successfully configures the transmission gate on a specific port using the provided input parameter.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_gate_config_set(), cvb_st_gate_config_set(), cva_st_gate_config_set().
 */
RtkApiRet rtk_st_gate_config_set(UnitChip_t unitChip, StGateConfigInfo_t *stGateConfigInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stGateConfigInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_gate_config_set(unitChip.unit, (dva_RtkPort)stGateConfigInfo->portId, stGateConfigInfo->baseTimeIsOffset, &stGateConfigInfo->baseTime, (uint32)stGateConfigInfo->cycleTimeExtension,
                                             stGateConfigInfo->gateControlList, stGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_gate_config_set(unitChip.unit, (cvb_RtkPort)stGateConfigInfo->portId, &stGateConfigInfo->baseTime, (uint32)stGateConfigInfo->cycleTimeExtension,
                                             stGateConfigInfo->gateControlList, stGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_gate_config_set(unitChip.unit, (cva_RtkPort)stGateConfigInfo->portId, &stGateConfigInfo->baseTime, (uint32)stGateConfigInfo->cycleTimeExtension,
                                             stGateConfigInfo->gateControlList, stGateConfigInfo->controlListLength);
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
 * @brief Get the admin configuration parameters. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stAdminGateConfigInfo ST admin gate configuration information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the admin configuration parameters.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_admin_gate_config_get(), cvb_st_admin_gate_config_get(), cva_st_admin_gate_config_get().
 */
RtkApiRet rtk_st_admin_gate_config_get(UnitChip_t unitChip, StGateConfigInfo_t *stAdminGateConfigInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stAdminGateConfigInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_admin_gate_config_get(unitChip.unit, stAdminGateConfigInfo->portId, &stAdminGateConfigInfo->baseTime, &stAdminGateConfigInfo->cycleTime,
                                                   &stAdminGateConfigInfo->cycleTimeExtension, stAdminGateConfigInfo->gateControlList, &stAdminGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_admin_gate_config_get(unitChip.unit, stAdminGateConfigInfo->portId, &stAdminGateConfigInfo->baseTime, &stAdminGateConfigInfo->cycleTime,
                                                   &stAdminGateConfigInfo->cycleTimeExtension, stAdminGateConfigInfo->gateControlList, &stAdminGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_admin_gate_config_get(unitChip.unit, stAdminGateConfigInfo->portId, &stAdminGateConfigInfo->baseTime, &stAdminGateConfigInfo->cycleTime,
                                                   &stAdminGateConfigInfo->cycleTimeExtension, stAdminGateConfigInfo->gateControlList, &stAdminGateConfigInfo->controlListLength);
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
 * @brief Get the operate configuration parameters. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stOperGateConfigInfo ST oper gate configuration information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the operate configuration  parameters.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_oper_gate_config_get(), cvb_st_oper_gate_config_get(), cva_st_oper_gate_config_get().
 */
RtkApiRet rtk_st_oper_gate_config_get(UnitChip_t unitChip, StGateConfigInfo_t *stOperGateConfigInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stOperGateConfigInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_oper_gate_config_get(unitChip.unit, stOperGateConfigInfo->portId, &stOperGateConfigInfo->baseTime, &stOperGateConfigInfo->cycleTime,
                                                  &stOperGateConfigInfo->cycleTimeExtension, stOperGateConfigInfo->gateControlList, &stOperGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_oper_gate_config_get(unitChip.unit, stOperGateConfigInfo->portId, &stOperGateConfigInfo->baseTime, &stOperGateConfigInfo->cycleTime,
                                                  &stOperGateConfigInfo->cycleTimeExtension, stOperGateConfigInfo->gateControlList, &stOperGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_oper_gate_config_get(unitChip.unit, stOperGateConfigInfo->portId, &stOperGateConfigInfo->baseTime, &stOperGateConfigInfo->cycleTime,
                                                  &stOperGateConfigInfo->cycleTimeExtension, stOperGateConfigInfo->gateControlList, &stOperGateConfigInfo->controlListLength);
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
 * @brief Set the admin gate state on a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stAdminGateStateInfo ST admin gate state information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the admin gate state on a specific port.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_admin_gate_state_set(), cvb_st_admin_gate_state_set(), cva_st_admin_gate_state_set().
 */
RtkApiRet rtk_st_admin_gate_state_set(UnitChip_t unitChip, StGateStateInfo_t *stAdminGateStateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stAdminGateStateInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_admin_gate_state_set(unitChip.unit, (dva_RtkPort)stAdminGateStateInfo->portId, stAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_admin_gate_state_set(unitChip.unit, (cvb_RtkPort)stAdminGateStateInfo->portId, stAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_admin_gate_state_set(unitChip.unit, (cva_RtkPort)stAdminGateStateInfo->portId, stAdminGateStateInfo->gateState);
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
 * @brief Get the admin gate state on a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stAdminGateStateInfo ST admin gate state information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the admin gate state on a specific port.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_admin_gate_state_get(), cvb_st_admin_gate_state_get(), cva_st_admin_gate_state_get().
 */
RtkApiRet rtk_st_admin_gate_state_get(UnitChip_t unitChip, StGateStateInfo_t *stAdminGateStateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stAdminGateStateInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_admin_gate_state_get(unitChip.unit, (dva_RtkPort)stAdminGateStateInfo->portId, &stAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_admin_gate_state_get(unitChip.unit, (cvb_RtkPort)stAdminGateStateInfo->portId, &stAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_admin_gate_state_get(unitChip.unit, (cva_RtkPort)stAdminGateStateInfo->portId, &stAdminGateStateInfo->gateState);
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
 * @brief Get the operate gate state on a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stOperGateStateInfo ST oper gate state information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the operate gate state on a specific port.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_oper_gate_state_get(), cvb_st_oper_gate_state_get(), cva_st_oper_gate_state_get().
 */
RtkApiRet rtk_st_oper_gate_state_get(UnitChip_t unitChip, StGateStateInfo_t *stOperGateStateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stOperGateStateInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_oper_gate_state_get(unitChip.unit, (dva_RtkPort)stOperGateStateInfo->portId, &stOperGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_oper_gate_state_get(unitChip.unit, (cvb_RtkPort)stOperGateStateInfo->portId, &stOperGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_oper_gate_state_get(unitChip.unit, (cva_RtkPort)stOperGateStateInfo->portId, &stOperGateStateInfo->gateState);
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
 * @brief Get the configuration pending flag. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stConfigPendingInfo ST config pending status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Successfully gets the configuration pending flag.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_st_config_pending_get(), cvb_st_config_pending_get(), cva_st_config_pending_get().
 */
RtkApiRet rtk_st_config_pending_get(UnitChip_t unitChip, StConfigPendingGet_t *stConfigPendingInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stConfigPendingInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_st_config_pending_get(unitChip.unit, (dva_RtkPort)stConfigPendingInfo->portId, &stConfigPendingInfo->configPending);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_st_config_pending_get(unitChip.unit, (cvb_RtkPort)stConfigPendingInfo->portId, &stConfigPendingInfo->configPending);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_st_config_pending_get(unitChip.unit, (cva_RtkPort)stConfigPendingInfo->portId, &stConfigPendingInfo->configPending);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* TRANSMISSION_GATE */
/**@}*/ /* ST/QBV */
