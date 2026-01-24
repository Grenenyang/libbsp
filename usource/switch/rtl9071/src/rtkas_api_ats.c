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
#include "rtkas_api_ats.h"
#include "rtkas_types.h"
#include "rtkas_system_porting.h"

#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ats.h"
#endif

/**
 * @defgroup RTK_MODULE_PSCM PSCM(QCI/PSFP & QCR/ATS)
 * @brief PSFP APIs. Filter the ingress frame and enforce the bandwidth policies on a stream basis. \n
 * ATS APIs. Asynchronous Traffic Shaping (ATS) Scheduler provides filtering mechanisms for streams.
 * @{
 */

/**
 * @defgroup ATS_SCHEDULER ATS Scheduler
 * @brief Used to configure the ATS scheduler.
 * @{
 */

/**
 * @brief This API initializes the ATS scheduler. \n
 *        The init function only needs to be called once before using ATS. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK                The ATS has been successfully initialized. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dva_ats_init()
 */
RtkApiRet rtk_ats_init(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_ats_init(unitChip.unit);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief This API gets the information including receive port number, transmit port number, maximum variation of clock offset, maximum deviation of clock rate, maximum recognition delay of arrival time, maximum processing delay, minimum processing delay for the ATS scheduler. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] atsGetInfo           Asynchronous Traffic Shaping Get Information (N/A) \n
 *                                  This parameter is used to obtain ATS information.
 * @retval RT_ERR_OK                The information of ats scheduler were obtained. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval Others                   Please refer to dva_ats_cbs_cir_get()
 */
RtkApiRet rtk_ats_get_info(UnitChip_t unitChip, AtsGetInfo_t *atsGetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsGetInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_get_info(unitChip.unit, atsGetInfo);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API sets the group ID, CBS and CIR for the ATS scheduler. \n
 *        Note: Committed Burst Size (CBS) in this function call is not related to IEEE 802.1Qav - Credit Based Shaping (CBS). CBS referred to in this function is a parameter for Asynchronous Traffic Shaping (ATS) only. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] atsCbsCirInfo         Asynchronous Traffic Shaping Committed Burst Size Committed Information Rate Information (N/A) \n
 *                                  This parameter is used to set or get the CBS or CIR information of ATS scheduler.
 * @param[out] None
 * @retval RT_ERR_OK                The settings of ats were configured. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval Others                   Please refer to dva_ats_cbs_cir_set()
 */
RtkApiRet rtk_ats_cbs_cir_set(UnitChip_t unitChip, AtsCbsCirInfo_t *atsCbsCirInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsCbsCirInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_cbs_cir_set(unitChip.unit, atsCbsCirInfo->schedulerId, atsCbsCirInfo->groupId, atsCbsCirInfo->cbs, atsCbsCirInfo->cirKbps);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }

    return ret;
}

/**
 * @brief This API gets the configurations including group ID, CBS and CIR for the ATS scheduler. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] atsCbsCirInfo     Asynchronous Traffic Shaping Committed Burst Size Committed Information Rate Information (N/A) \n
 *                                  This parameter is used to set or get the CBS or CIR information of ATS scheduler.
 * @retval RT_ERR_OK                The settings of ats scheduler were obtained. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval Others                   Please refer to dva_ats_cbs_cir_get()
 */
RtkApiRet rtk_ats_cbs_cir_get(UnitChip_t unitChip, AtsCbsCirInfo_t *atsCbsCirInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsCbsCirInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_cbs_cir_get(unitChip.unit, atsCbsCirInfo->schedulerId, &atsCbsCirInfo->groupId, &atsCbsCirInfo->cbs, &atsCbsCirInfo->cirKbps);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API sets the group residence time for the ATS scheduler. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip                          Stacking unit and chip ID (N/A) \n
 *                                              Stacking unit ID is used to indicate which switch is to be accessed.
 *                                              Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] atsGroupResidenceTimeInfo         Asynchronous Traffic Shaping Group Residence Time Information (N/A) \n
 *                                              This parameter is used to set or get the residence time of the ATS scheduler group.
 * @param[out] None
 * @retval RT_ERR_OK                            Successfully set the residence time for the ATS Scheduler group. \n
 * @retval -RT_ERR_NOT_SUPPORT                  Not supported. \n
 * @retval -RT_ERR_NULL_POINTER                 Null pointer for parameter. \n
 * @retval Others                               Please refer to dva_ats_group_residence_time_set()
 */
RtkApiRet rtk_ats_group_residence_time_set(UnitChip_t unitChip, AtsGroupResidenceTimeInfo_t *atsGroupResidenceTimeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsGroupResidenceTimeInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_group_residence_time_set(unitChip.unit, atsGroupResidenceTimeInfo->groupId, atsGroupResidenceTimeInfo->maxResidenceTimeNs);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API gets the configurations including group residence time for the ATS scheduler. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip                       Stacking unit and chip ID (N/A) \n
 *                                           Stacking unit ID is used to indicate which switch is to be accessed.
 *                                           Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] atsGroupResidenceTimeInfo  Asynchronous Traffic Shaping Group Residence Time Information (N/A) \n
 *                                           This parameter is used to set or get the residence time of the ATS scheduler group.
 * @retval RT_ERR_OK                         Obtain the residence time settings for the ATS Scheduler group. \n
 * @retval -RT_ERR_NOT_SUPPORT               Not supported. \n
 * @retval -RT_ERR_NULL_POINTER              Null pointer for parameter. \n
 * @retval Others                            Please refer to dva_ats_group_residence_time_get()
 */
RtkApiRet rtk_ats_group_residence_time_get(UnitChip_t unitChip, AtsGroupResidenceTimeInfo_t *atsGroupResidenceTimeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsGroupResidenceTimeInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_group_residence_time_get(unitChip.unit, atsGroupResidenceTimeInfo->groupId, &atsGroupResidenceTimeInfo->maxResidenceTimeNs);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API gets the drop counter for a specific port of ATS scheduler. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip                 Stacking unit and chip ID (N/A) \n
 *                                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] atsDropCounterGet    Asynchronous Traffic Shaping Drop Counter Get (N/A) \n
 *                                     This parameter is used to retrieve the drop counter of a specific port.
 * @retval RT_ERR_OK                   Successfully obtained the drop counter for a specific port in the ATS Scheduler. \n
 * @retval -RT_ERR_NOT_SUPPORT         Not supported. \n
 * @retval -RT_ERR_NULL_POINTER        Null pointer for parameter. \n
 * @retval Others                      Please refer to dva_ats_drop_counter_get()
 */
RtkApiRet rtk_ats_drop_counter_get(UnitChip_t unitChip, AtsDropCounterGet_t *atsDropCounterGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsDropCounterGet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_drop_counter_get(unitChip.unit, atsDropCounterGet->port, &atsDropCounterGet->counter);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API clears the drop counter for a specific port of ATS scheduler. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip                Stacking unit and chip ID (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] atsDropCounterClear     Asynchronous Traffic Shaping Drop Counter Clear (N/A) \n
 *                                    This parameter is used to clear the drop counter of a specific port.
 * @param[out] None
 * @retval RT_ERR_OK                  The drop counter for a specific port of ats were cleared. \n
 * @retval -RT_ERR_NOT_SUPPORT        Not supported. \n
 * @retval -RT_ERR_NULL_POINTER       Null pointer for parameter. \n
 * @retval Others                     Please refer to dva_ats_drop_counter_clear()
 */
RtkApiRet rtk_ats_drop_counter_clear(UnitChip_t unitChip, AtsDropCounterClear_t *atsDropCounterClear)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsDropCounterClear)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_drop_counter_clear(unitChip.unit, atsDropCounterClear->port);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API sets the ATS scheduler for a specific queue on a specific port. \n
 *        Supported chip: RTL907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] atsSetInfo            Asynchronous Traffic Shaping Set Information (N/A) \n
 *                                  This parameter is used to set the ATS scheduler of a specific queue on a specific port.
 * @param[out] None
 * @retval RT_ERR_OK                The settings of the ATS scheduler for a specific queue on a specific port were configured. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval Others                   Please refer to dva_ats_scheduler_set()
 */
RtkApiRet rtk_ats_scheduler_set(UnitChip_t unitChip, AtsSetInfo_t *atsSetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == atsSetInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ats_scheduler_set(unitChip.unit, atsSetInfo->port, atsSetInfo->queue, atsSetInfo->mode);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /* ATS_SCHEDULER */
/**@}*/ /* RTK_MODULE_PSCM */
