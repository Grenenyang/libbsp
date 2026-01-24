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
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_psfp.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_psfp.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_psfp.h"
#endif

#include "rtkas_api_psfp.h"
/**
 * @defgroup RTK_MODULE_PSCM PSCM(QCI/PSFP & QCR/ATS)
 * @brief PSFP APIs. Filter the ingress frame and enforce the bandwidth policies on a stream basis. \n
 * ATS APIs. Asynchronous Traffic Shaping (ATS) Scheduler provides filtering mechanisms for streams.
 * @{
 */

/**
 * @defgroup PSFP_GLO Global Parameters
 * @brief This APIs can configure the drop capability for the PSFP module.
 * @{
 */

/**
 * @brief To get whether the PSFP module is able to drop the trapped destination port mask. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] mode              PSFP module mode parameters (N/A)  \n
 *                               Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully obtained PSFP module drop mode.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_mode_get(), cvb_psfp_mode_get(), cva_psfp_mode_get().
 */
RtkApiRet rtk_psfp_mode_get(UnitChip_t unitChip, PsfpMode_t *psfpMode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpMode)
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
                ret = dva_psfp_mode_get(unitChip.unit, &psfpMode->mode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_mode_get(unitChip.unit, &psfpMode->mode);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_mode_get(unitChip.unit, &psfpMode->mode);
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
 * @brief To set whether the PSFP module is able to drop the trapped destination port mask. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] mode               PSFP module mode parameters (N/A)  \n
 *                               Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully set PSFP module drop mode.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_mode_set(), cvb_psfp_mode_set(), cva_psfp_mode_set.
 */
RtkApiRet rtk_psfp_mode_set(UnitChip_t unitChip, PsfpMode_t *psfpMode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpMode)
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
                ret = dva_psfp_mode_set(unitChip.unit, psfpMode->mode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_mode_set(unitChip.unit, psfpMode->mode);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_mode_set(unitChip.unit, psfpMode->mode);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PSFP_GLO */

/**
 * @defgroup PSFP_FILTER Stream Filter
 * @brief PSFP stream filter configuration and information acquisition
 * @{
 */

/**
 * @brief To set stream handle, priority, stream gate instance ID, maximum stream SDU, flow meter instance ID and ATS scheduler instance ID of the specified stream filter instance. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @retval RT_ERR_OK Successfully set the entry value of the specific stream filter instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_psfp_stm_filter_entry_set(), cvb_psfp_stm_filter_entry_set(), cva_psfp_stm_filter_entry_set().
 */
RtkApiRet rtk_psfp_stm_filter_entry_set(UnitChip_t unitChip, PsfpStmFilterEntry_t *stmFilterEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stmFilterEntry)
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
                ret = dva_psfp_stm_filter_entry_set(unitChip.unit, stmFilterEntry->index, &stmFilterEntry->stmFilteParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_filter_entry_set(unitChip.unit, stmFilterEntry->index, &stmFilterEntry->stmFilteParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_filter_entry_set(unitChip.unit, stmFilterEntry->index, &stmFilterEntry->stmFilteParam);
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
 * @brief To get stream handle, priority, stream gate instance ID, maximum stream SDU, flow meter instance ID and ATS scheduler instance ID of the specified stream filter instance. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stmFilterEntry Stream filter instance parameters (N/A) \n
 *                               Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the entry value of the specific stream filter instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_stm_filter_entry_get(), cvb_psfp_stm_filter_entry_get(), cva_psfp_stm_filter_entry_get().
 */
RtkApiRet rtk_psfp_stm_filter_entry_get(UnitChip_t unitChip, PsfpStmFilterEntry_t *stmFilterEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stmFilterEntry)
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
                ret = dva_psfp_stm_filter_entry_get(unitChip.unit, stmFilterEntry->index, &stmFilterEntry->stmFilteParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_filter_entry_get(unitChip.unit, stmFilterEntry->index, &stmFilterEntry->stmFilteParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_filter_entry_get(unitChip.unit, stmFilterEntry->index, &stmFilterEntry->stmFilteParam);
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
 * @brief To get BlockStreamDueToOversizeFrame enable and status of the specified stream filter instance. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stmBlockOverSize BlockStreamDueToOversizeFrame parameters (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the truth value of BlockStreamDueToOversizeFrameEnable \n
 *                   and BlockStreamDueToOversizeFrame.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_stm_block_oversize_get(), cvb_psfp_stm_block_oversize_get(), cva_psfp_stm_block_oversize_get().
 */
RtkApiRet rtk_psfp_stm_block_oversize_get(UnitChip_t unitChip, PsfpStmBlockOversize_t *stmBlockOverSize)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stmBlockOverSize)
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
                ret = dva_psfp_stm_block_oversize_get(unitChip.unit, stmBlockOverSize->index, &stmBlockOverSize->isEnable, &stmBlockOverSize->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_block_oversize_get(unitChip.unit, stmBlockOverSize->index, &stmBlockOverSize->isEnable, &stmBlockOverSize->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_block_oversize_get(unitChip.unit, stmBlockOverSize->index, &stmBlockOverSize->isEnable, &stmBlockOverSize->status);
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
 * @brief To enable or disable BlockStreamDueToOversizeFrame and determine whether to clear the current BlockStreamDueToOversizeFrame status. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] stmBlockOverSize BlockStreamDueToOversizeFrame parameters (N/A) \n
 *                              Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully set the truth value of BlockStreamDueToOversizeFrameEnable \n
 *                   (and clear the current state of BlockStreamDueToOversize).
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_stm_block_oversize_set(), cvb_psfp_stm_block_oversize_set(), cva_psfp_stm_block_oversize_set().
 */
RtkApiRet rtk_psfp_stm_block_oversize_set(UnitChip_t unitChip, PsfpStmBlockOversize_t *stmBlockOverSize)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stmBlockOverSize)
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
                ret = dva_psfp_stm_block_oversize_set(unitChip.unit, stmBlockOverSize->index, stmBlockOverSize->isEnable, stmBlockOverSize->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_block_oversize_set(unitChip.unit, stmBlockOverSize->index, stmBlockOverSize->isEnable, stmBlockOverSize->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_block_oversize_set(unitChip.unit, stmBlockOverSize->index, stmBlockOverSize->isEnable, stmBlockOverSize->status);
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
 * @brief To get whether a specific stream filter entry is valid or not. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stmFlEntryValid Stream filter entry valid status parameters (N/A) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the validation value of the specified stream filter entry.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_stm_fl_entry_valid_get(), cvb_psfp_stm_fl_entry_valid_get(), cva_psfp_stm_fl_entry_valid_get().
 */
RtkApiRet rtk_psfp_stm_fl_entry_valid_get(UnitChip_t unitChip, PsfpStmFilterEntryValid_t *stmFlEntryValid)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stmFlEntryValid)
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
                ret = dva_psfp_stm_fl_entry_valid_get(unitChip.unit, stmFlEntryValid->index, &stmFlEntryValid->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_fl_entry_valid_get(unitChip.unit, stmFlEntryValid->index, &stmFlEntryValid->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_fl_entry_valid_get(unitChip.unit, stmFlEntryValid->index, &stmFlEntryValid->valid);
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
 * @brief To set whether a specific stream filter entry is valid or not. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] stmFlEntryValid Stream filter entry valid status parameters (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully set the validation value of the specified stream filter entry.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_stm_fl_entry_valid_set(), cvb_psfp_stm_fl_entry_valid_set(), cva_psfp_stm_fl_entry_valid_set().
 */
RtkApiRet rtk_psfp_stm_fl_entry_valid_set(UnitChip_t unitChip, PsfpStmFilterEntryValid_t *stmFlEntryValid)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stmFlEntryValid)
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
                ret = dva_psfp_stm_fl_entry_valid_set(unitChip.unit, stmFlEntryValid->index, stmFlEntryValid->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_fl_entry_valid_set(unitChip.unit, stmFlEntryValid->index, stmFlEntryValid->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_fl_entry_valid_set(unitChip.unit, stmFlEntryValid->index, stmFlEntryValid->valid);
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
 * @brief To get the current count of the specified counter for the specified index. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *                   stream filter instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_psfp_frame_counters_get(), cvb_psfp_frame_counters_get(), cva_psfp_frame_counters_get().
 */
RtkApiRet rtk_psfp_frame_counters_get(UnitChip_t unitChip, PsfpFrameCounter_t *frameCounters)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == frameCounters)
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
                ret = dva_psfp_frame_counters_get(unitChip.unit, frameCounters->index, frameCounters->type, &frameCounters->counter);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_frame_counters_get(unitChip.unit, frameCounters->index, frameCounters->type, &frameCounters->counter);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_frame_counters_get(unitChip.unit, frameCounters->index, frameCounters->type, &frameCounters->counter);
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
 * @brief To set the current count of the specified counter for the specified index. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] frameCounters Specified counter current parameters (N/A) \n
 *                          Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully set the counts of the counter type within the specified stream filter \n
 *                   instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_frame_counters_set(), cvb_psfp_frame_counters_set(), cva_psfp_frame_counters_set().
 */
RtkApiRet rtk_psfp_frame_counters_set(UnitChip_t unitChip, PsfpFrameCounter_t *frameCounters)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == frameCounters)
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
                ret = dva_psfp_frame_counters_set(unitChip.unit, frameCounters->index, frameCounters->type, frameCounters->counter);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_frame_counters_set(unitChip.unit, frameCounters->index, frameCounters->type, frameCounters->counter);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_frame_counters_set(unitChip.unit, frameCounters->index, frameCounters->type, frameCounters->counter);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PSFP_FILTER */

/**
 * @defgroup PSFP_GATE Stream Gate
 * @brief PSFP stream gate configuration and information acquisition
 * @{
 */
/**
 * @brief To get whether the specific stream gate is enabled or disabled. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] gateEnabled Specific stream gate status parameters (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the enable status of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_gate_enabled_get(), cvb_psfp_gate_enabled_get(), cva_psfp_gate_enabled_get().
 */
RtkApiRet rtk_psfp_gate_enabled_get(UnitChip_t unitChip, PsfpGateEnabled_t *gateEnabled)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == gateEnabled)
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
                ret = dva_psfp_gate_enabled_get(unitChip.unit, gateEnabled->index, &gateEnabled->gateEnabled);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_gate_enabled_get(unitChip.unit, gateEnabled->index, &gateEnabled->gateEnabled);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_gate_enabled_get(unitChip.unit, gateEnabled->index, &gateEnabled->gateEnabled);
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
 * @brief To enable or disable a specific stream gate. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] gateEnabled Specific stream gate status parameters (N/A) \n
 *                        Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully set the enable status of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_gate_enabled_set(), cvb_psfp_gate_enabled_set(), cva_psfp_gate_enabled_set().
 */
RtkApiRet rtk_psfp_gate_enabled_set(UnitChip_t unitChip, PsfpGateEnabled_t *gateEnabled)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == gateEnabled)
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
                ret = dva_psfp_gate_enabled_set(unitChip.unit, gateEnabled->index, gateEnabled->gateEnabled);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_gate_enabled_set(unitChip.unit, gateEnabled->index, gateEnabled->gateEnabled);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_gate_enabled_set(unitChip.unit, gateEnabled->index, gateEnabled->gateEnabled);
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
 * @brief To get the current admin state of the specified stream gate. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpAdminGateStateInfo Specified stream gate admin state parameters (N/A) \n
 *                                   Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the gate state of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_admin_gate_state_get(), cvb_psfp_admin_gate_state_get(), cva_psfp_admin_gate_state_get().
 */
RtkApiRet rtk_psfp_admin_gate_state_get(UnitChip_t unitChip, PsfpGateState_t *psfpAdminGateStateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpAdminGateStateInfo)
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
                ret = dva_psfp_admin_gate_state_get(unitChip.unit, psfpAdminGateStateInfo->index, &psfpAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_admin_gate_state_get(unitChip.unit, psfpAdminGateStateInfo->index, &psfpAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_admin_gate_state_get(unitChip.unit, psfpAdminGateStateInfo->index, &psfpAdminGateStateInfo->gateState);
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
 * @brief To set the current admin state of the specified stream gate. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpAdminGateStateInfo Specified stream gate admin state parameters (N/A) \n
 *                               Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully set the gate state of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_admin_gate_state_set(), cvb_psfp_admin_gate_state_set(), cva_psfp_admin_gate_state_set().
 */
RtkApiRet rtk_psfp_admin_gate_state_set(UnitChip_t unitChip, PsfpGateState_t *psfpAdminGateStateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpAdminGateStateInfo)
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
                ret = dva_psfp_admin_gate_state_set(unitChip.unit, psfpAdminGateStateInfo->index, psfpAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_admin_gate_state_set(unitChip.unit, psfpAdminGateStateInfo->index, psfpAdminGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_admin_gate_state_set(unitChip.unit, psfpAdminGateStateInfo->index, psfpAdminGateStateInfo->gateState);
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
 * @brief To get the current operation state of the specified stream gate. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpOperGateStateInfo Specified stream gate operation state parameters (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the gate state of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_oper_gate_state_get(), cvb_psfp_oper_gate_state_get(), cva_psfp_oper_gate_state_get().
 */
RtkApiRet rtk_psfp_oper_gate_state_get(UnitChip_t unitChip, PsfpGateState_t *psfpOperGateStateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpOperGateStateInfo)
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
                ret = dva_psfp_oper_gate_state_get(unitChip.unit, psfpOperGateStateInfo->index, &psfpOperGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_oper_gate_state_get(unitChip.unit, psfpOperGateStateInfo->index, &psfpOperGateStateInfo->gateState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_oper_gate_state_get(unitChip.unit, psfpOperGateStateInfo->index, &psfpOperGateStateInfo->gateState);
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
 * @brief To get the admin gate configuration of the specified stream gate instance. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpAdminGateConfigInfo Admin gate configuration parameters (N/A) \n
 *                                    Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the admin gate configuration of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_admin_gate_config_get().
 */
RtkApiRet rtk_psfp_admin_gate_config_get(UnitChip_t unitChip, PsfpGateConfig_t *psfpAdminGateConfigInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpAdminGateConfigInfo)
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
                ret = dva_psfp_admin_gate_config_get(unitChip.unit, psfpAdminGateConfigInfo->index, &psfpAdminGateConfigInfo->baseTime,
                                                     &psfpAdminGateConfigInfo->cycleTime, &psfpAdminGateConfigInfo->cycleTimeExtension,
                                                     psfpAdminGateConfigInfo->gateControlList, &psfpAdminGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_admin_gate_config_get(unitChip.unit, psfpAdminGateConfigInfo->index, &psfpAdminGateConfigInfo->baseTime,
                                                     &psfpAdminGateConfigInfo->cycleTime, &psfpAdminGateConfigInfo->cycleTimeExtension,
                                                     psfpAdminGateConfigInfo->gateControlList, &psfpAdminGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_admin_gate_config_get(unitChip.unit, psfpAdminGateConfigInfo->index, &psfpAdminGateConfigInfo->baseTime,
                                                     &psfpAdminGateConfigInfo->cycleTime, &psfpAdminGateConfigInfo->cycleTimeExtension,
                                                     psfpAdminGateConfigInfo->gateControlList, &psfpAdminGateConfigInfo->controlListLength);
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
 * @brief To set the admin gate configuration of the specified stream gate instance. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpAdminGateConfigInfo Admin gate configuration parameters (N/A) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully set the admin gate configuration of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_admin_gate_config_set(), cvb_psfp_admin_gate_config_set(), cva_psfp_admin_gate_config_set().
 */
RtkApiRet rtk_psfp_admin_gate_config_set(UnitChip_t unitChip, PsfpGateConfig_t *psfpAdminGateConfigInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpAdminGateConfigInfo)
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
                ret = dva_psfp_admin_gate_config_set(unitChip.unit, psfpAdminGateConfigInfo->index, &psfpAdminGateConfigInfo->baseTime,
                                                     psfpAdminGateConfigInfo->cycleTimeExtension, psfpAdminGateConfigInfo->gateControlList,
                                                     psfpAdminGateConfigInfo->controlListLength, ENABLED);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_admin_gate_config_set(unitChip.unit, psfpAdminGateConfigInfo->index, &psfpAdminGateConfigInfo->baseTime,
                                                     psfpAdminGateConfigInfo->cycleTimeExtension, psfpAdminGateConfigInfo->gateControlList,
                                                     psfpAdminGateConfigInfo->controlListLength, ENABLED);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_admin_gate_config_set(unitChip.unit, psfpAdminGateConfigInfo->index, &psfpAdminGateConfigInfo->baseTime,
                                                     psfpAdminGateConfigInfo->cycleTimeExtension, psfpAdminGateConfigInfo->gateControlList,
                                                     psfpAdminGateConfigInfo->controlListLength, ENABLED);
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
 * @brief To get the operation gate configuration of the specified stream gate instance. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpOperGateConfigInfo Operation gate configuration parameters (N/A) \n
 *                                   Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the operation gate configuration of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_oper_gate_config_get(), cvb_psfp_oper_gate_config_get(), cva_psfp_oper_gate_config_get().
 */
RtkApiRet rtk_psfp_oper_gate_config_get(UnitChip_t unitChip, PsfpGateConfig_t *psfpOperGateConfigInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpOperGateConfigInfo)
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
                ret = dva_psfp_oper_gate_config_get(unitChip.unit, psfpOperGateConfigInfo->index, &psfpOperGateConfigInfo->baseTime, &psfpOperGateConfigInfo->cycleTime,
                                                    &psfpOperGateConfigInfo->cycleTimeExtension, psfpOperGateConfigInfo->gateControlList, &psfpOperGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_oper_gate_config_get(unitChip.unit, psfpOperGateConfigInfo->index, &psfpOperGateConfigInfo->baseTime, &psfpOperGateConfigInfo->cycleTime,
                                                    &psfpOperGateConfigInfo->cycleTimeExtension, psfpOperGateConfigInfo->gateControlList, &psfpOperGateConfigInfo->controlListLength);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_oper_gate_config_get(unitChip.unit, psfpOperGateConfigInfo->index, &psfpOperGateConfigInfo->baseTime, &psfpOperGateConfigInfo->cycleTime,
                                                    &psfpOperGateConfigInfo->cycleTimeExtension, psfpOperGateConfigInfo->gateControlList, &psfpOperGateConfigInfo->controlListLength);
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
 * @brief To get the CloseGateDueToInvalidRx status and whether it is enabled or disabled. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpGateClosedInvRxInfo CloseGateDueToInvalidRx parameters (N/A) \n
 *                                    Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get whether GateClosedDueToInvalidRxEnable is enabled \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_gate_closed_inv_rx_get(), cvb_psfp_gate_closed_inv_rx_get(), cva_psfp_gate_closed_inv_rx_get().
 */
RtkApiRet rtk_psfp_gate_closed_inv_rx_get(UnitChip_t unitChip, PsfpGateClosedInvRx_t *psfpGateClosedInvRxInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpGateClosedInvRxInfo)
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
                ret = dva_psfp_gate_closed_inv_rx_get(unitChip.unit, psfpGateClosedInvRxInfo->index, &psfpGateClosedInvRxInfo->isEnable,
                                                      &psfpGateClosedInvRxInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_gate_closed_inv_rx_get(unitChip.unit, psfpGateClosedInvRxInfo->index, &psfpGateClosedInvRxInfo->isEnable,
                                                      &psfpGateClosedInvRxInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_gate_closed_inv_rx_get(unitChip.unit, psfpGateClosedInvRxInfo->index, &psfpGateClosedInvRxInfo->isEnable,
                                                      &psfpGateClosedInvRxInfo->status);
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
 * @brief To enable or disable CloseGateDueToInvalidRx and determine whether to clear the current CloseGateDueToInvalidRx status. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpGateClosedInvRxInfo CloseGateDueToInvalidRx parameters (N/A) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully enable or disable GateClosedDueToInvalidRxEnable \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_gate_closed_inv_rx_set(), cvb_psfp_gate_closed_inv_rx_set(), cva_psfp_gate_closed_inv_rx_set().
 */
RtkApiRet rtk_psfp_gate_closed_inv_rx_set(UnitChip_t unitChip, PsfpGateClosedInvRx_t *psfpGateClosedInvRxInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpGateClosedInvRxInfo)
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
                ret = dva_psfp_gate_closed_inv_rx_set(unitChip.unit, psfpGateClosedInvRxInfo->index, psfpGateClosedInvRxInfo->isEnable,
                                                      psfpGateClosedInvRxInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_gate_closed_inv_rx_set(unitChip.unit, psfpGateClosedInvRxInfo->index, psfpGateClosedInvRxInfo->isEnable,
                                                      psfpGateClosedInvRxInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_gate_closed_inv_rx_set(unitChip.unit, psfpGateClosedInvRxInfo->index, psfpGateClosedInvRxInfo->isEnable,
                                                      psfpGateClosedInvRxInfo->status);
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
 * @brief To get the GateClosedDueToOctetsExceeded status and whether it is enabled or disabled. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpGateClosedOctExdInfo GateClosedDueToOctetsExceeded parameters (N/A) \n
 *                                     Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get whether GateClosedDueToOctetsExceededEnable \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_gate_closed_oct_exd_get(), cvb_psfp_gate_closed_oct_exd_get(), cva_psfp_gate_closed_oct_exd_get().
 */
RtkApiRet rtk_psfp_gate_closed_oct_exd_get(UnitChip_t unitChip, PsfpGateClosedOctExd_t *psfpGateClosedOctExdInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpGateClosedOctExdInfo)
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
                ret = dva_psfp_gate_closed_oct_exd_get(unitChip.unit, psfpGateClosedOctExdInfo->index, &psfpGateClosedOctExdInfo->isEnable,
                                                       &psfpGateClosedOctExdInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_gate_closed_oct_exd_get(unitChip.unit, psfpGateClosedOctExdInfo->index, &psfpGateClosedOctExdInfo->isEnable,
                                                       &psfpGateClosedOctExdInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_gate_closed_oct_exd_get(unitChip.unit, psfpGateClosedOctExdInfo->index, &psfpGateClosedOctExdInfo->isEnable,
                                                       &psfpGateClosedOctExdInfo->status);
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
 * @brief To enable or disable GateClosedDueToOctetsExceeded and determine whether to clear the current GateClosedDueToOctetsExceeded status. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpGateClosedOctExdInfo GateClosedDueToOctetsExceeded parameters (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully enable or disable GateClosedDueToOctetsExceeded \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_gate_closed_oct_exd_set(), cvb_psfp_gate_closed_oct_exd_set(), cva_psfp_gate_closed_oct_exd_set().
 */
RtkApiRet rtk_psfp_gate_closed_oct_exd_set(UnitChip_t unitChip, PsfpGateClosedOctExd_t *psfpGateClosedOctExdInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpGateClosedOctExdInfo)
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
                ret = dva_psfp_gate_closed_oct_exd_set(unitChip.unit, psfpGateClosedOctExdInfo->index, psfpGateClosedOctExdInfo->isEnable,
                                                       psfpGateClosedOctExdInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_gate_closed_oct_exd_set(unitChip.unit, psfpGateClosedOctExdInfo->index, psfpGateClosedOctExdInfo->isEnable,
                                                       psfpGateClosedOctExdInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_gate_closed_oct_exd_set(unitChip.unit, psfpGateClosedOctExdInfo->index, psfpGateClosedOctExdInfo->isEnable,
                                                       psfpGateClosedOctExdInfo->status);
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
 * @brief To get the validation of the specific stream gate entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpStmGateEntValidInfo Specific stream gate entry valid status parameters (N/A) \n
 *                                    Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the validation value of the specified stream gate \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_stm_gate_ent_valid_get(), cvb_psfp_stm_gate_ent_valid_get(), cva_psfp_stm_gate_ent_valid_get().
 */
RtkApiRet rtk_psfp_stm_gate_ent_valid_get(UnitChip_t unitChip, PsfpGateEntValid_t *psfpStmGateEntValidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpStmGateEntValidInfo)
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
                ret = dva_psfp_stm_gate_ent_valid_get(unitChip.unit, psfpStmGateEntValidInfo->index, &psfpStmGateEntValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_gate_ent_valid_get(unitChip.unit, psfpStmGateEntValidInfo->index, &psfpStmGateEntValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_gate_ent_valid_get(unitChip.unit, psfpStmGateEntValidInfo->index, &psfpStmGateEntValidInfo->valid);
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
 * @brief To set the validation of the specific stream gate entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpStmGateEntValidInfo Specific stream gate entry valid status parameters (N/A) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully set the validation value of the specified stream gate \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_stm_gate_ent_valid_set(), cvb_psfp_stm_gate_ent_valid_set(), cva_psfp_stm_gate_ent_valid_set().
 */
RtkApiRet rtk_psfp_stm_gate_ent_valid_set(UnitChip_t unitChip, PsfpGateEntValid_t *psfpStmGateEntValidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpStmGateEntValidInfo)
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
                ret = dva_psfp_stm_gate_ent_valid_set(unitChip.unit, psfpStmGateEntValidInfo->index, psfpStmGateEntValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_stm_gate_ent_valid_set(unitChip.unit, psfpStmGateEntValidInfo->index, psfpStmGateEntValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_stm_gate_ent_valid_set(unitChip.unit, psfpStmGateEntValidInfo->index, psfpStmGateEntValidInfo->valid);
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
 * @brief To get the configuration change time of gate control list. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpConfigChangeTimeInfo Configuration change time parameters (N/A) \n
 *                                     Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully get the configuration change time.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_config_change_time_get(), cvb_psfp_config_change_time_get(), cva_psfp_config_change_time_get().
 */
RtkApiRet rtk_psfp_config_change_time_get(UnitChip_t unitChip, PsfpConfigChangeTimeGet_t *psfpConfigChangeTimeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpConfigChangeTimeInfo)
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
                ret = dva_psfp_config_change_time_get(unitChip.unit, psfpConfigChangeTimeInfo->index, &psfpConfigChangeTimeInfo->configChangeTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_config_change_time_get(unitChip.unit, psfpConfigChangeTimeInfo->index, &psfpConfigChangeTimeInfo->configChangeTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_config_change_time_get(unitChip.unit, psfpConfigChangeTimeInfo->index, &psfpConfigChangeTimeInfo->configChangeTime);
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
 * @brief To get the current time. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpCurrentTimeInfo Current time parameters (N/A) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets the current time.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_current_time_get(), cvb_psfp_current_time_get(), cva_psfp_current_time_get().
 */
RtkApiRet rtk_psfp_current_time_get(UnitChip_t unitChip, PsfpCurrentTimeGet_t *psfpCurrentTimeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpCurrentTimeInfo)
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
                ret = dva_psfp_current_time_get(unitChip.unit, &psfpCurrentTimeInfo->currentTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_current_time_get(unitChip.unit, &psfpCurrentTimeInfo->currentTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_current_time_get(unitChip.unit, &psfpCurrentTimeInfo->currentTime);
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
 * @brief To get whether it is in a pending status of changing configuration. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpConfigPendingInfo Configuration pending status parameters (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets whether it is in a pending status \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_config_pending_get(), cvb_psfp_config_pending_get(), cva_psfp_config_pending_get().
 */
RtkApiRet rtk_psfp_config_pending_get(UnitChip_t unitChip, PsfpConfigPendingGet_t *psfpConfigPendingInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpConfigPendingInfo)
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
                ret = dva_psfp_config_pending_get(unitChip.unit, psfpConfigPendingInfo->index, &psfpConfigPendingInfo->configPending);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_config_pending_get(unitChip.unit, psfpConfigPendingInfo->index, &psfpConfigPendingInfo->configPending);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_config_pending_get(unitChip.unit, psfpConfigPendingInfo->index, &psfpConfigPendingInfo->configPending);
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
 * @brief To get the admin internal priority specification value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpAdminIpvInfo Admin internal priority parameters (N/A) \n
 *                             Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets the ipv of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_admin_ipv_get(), cvb_psfp_admin_ipv_get(), cva_psfp_admin_ipv_get().
 */
RtkApiRet rtk_psfp_admin_ipv_get(UnitChip_t unitChip, PsfpGateIpv_t *psfpAdminIpvInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpAdminIpvInfo)
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
                ret = dva_psfp_admin_ipv_get(unitChip.unit, psfpAdminIpvInfo->index, &psfpAdminIpvInfo->ipv);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_admin_ipv_get(unitChip.unit, psfpAdminIpvInfo->index, &psfpAdminIpvInfo->ipv);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_admin_ipv_get(unitChip.unit, psfpAdminIpvInfo->index, &psfpAdminIpvInfo->ipv);
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
 * @brief To set the admin internal priority specification value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpAdminIpvInfo Admin internal priority parameters (N/A) \n
 *                             Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully sets the ipv of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_admin_ipv_set(), cvb_psfp_admin_ipv_set(), cva_psfp_admin_ipv_set().
 */
RtkApiRet rtk_psfp_admin_ipv_set(UnitChip_t unitChip, PsfpGateIpv_t *psfpAdminIpvInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpAdminIpvInfo)
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
                ret = dva_psfp_admin_ipv_set(unitChip.unit, psfpAdminIpvInfo->index, psfpAdminIpvInfo->ipv);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_admin_ipv_set(unitChip.unit, psfpAdminIpvInfo->index, psfpAdminIpvInfo->ipv);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_admin_ipv_set(unitChip.unit, psfpAdminIpvInfo->index, psfpAdminIpvInfo->ipv);
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
 * @brief To get the operation internal priority specification value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpOperIpvInfo Operation internal priority parameters (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets the oper ipv of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_oper_ipv_get(), cvb_psfp_oper_ipv_get(), cva_psfp_oper_ipv_get().
 */
RtkApiRet rtk_psfp_oper_ipv_get(UnitChip_t unitChip, PsfpGateIpv_t *psfpOperIpvInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpOperIpvInfo)
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
                ret = dva_psfp_oper_ipv_get(unitChip.unit, psfpOperIpvInfo->index, &psfpOperIpvInfo->ipv);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_oper_ipv_get(unitChip.unit, psfpOperIpvInfo->index, &psfpOperIpvInfo->ipv);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_oper_ipv_get(unitChip.unit, psfpOperIpvInfo->index, &psfpOperIpvInfo->ipv);
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
 * @brief To get configuration change error counts. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpConfChangeErrInfo Configuration change error counter parameters (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets the configuration change error count of the specified stream gate instance.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_conf_change_err_get(), cvb_psfp_conf_change_err_get(), cva_psfp_conf_change_err_get().
 */
RtkApiRet rtk_psfp_conf_change_err_get(UnitChip_t unitChip, PsfpConfChangeErrGet_t *psfpConfChangeErrInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpConfChangeErrInfo)
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
                ret = dva_psfp_conf_change_err_get(unitChip.unit, psfpConfChangeErrInfo->index, &psfpConfChangeErrInfo->configChangeErrorCounter);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_conf_change_err_get(unitChip.unit, psfpConfChangeErrInfo->index, &psfpConfChangeErrInfo->configChangeErrorCounter);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_conf_change_err_get(unitChip.unit, psfpConfChangeErrInfo->index, &psfpConfChangeErrInfo->configChangeErrorCounter);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PSFP_GATE */

/**
 * @defgroup PSFP_METER Flow Meter
 * @brief PSFP flow meter configuration and information acquisition
 * @{
 */
/**
 * @brief To get the entry of a specific flow meter. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpFmEntryInfo Specific flow meter entry parameters (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets all parameters of the entry of the specified flow meter \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_fm_entry_get(), cvb_psfp_fm_entry_get(), cva_psfp_fm_entry_get().
 */
RtkApiRet rtk_psfp_fm_entry_get(UnitChip_t unitChip, PsfpFmEntry_t *psfpFmEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpFmEntryInfo)
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
                ret = dva_psfp_fm_entry_get(unitChip.unit, psfpFmEntryInfo->index, &psfpFmEntryInfo->flowMeterParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_fm_entry_get(unitChip.unit, psfpFmEntryInfo->index, &psfpFmEntryInfo->flowMeterParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_fm_entry_get(unitChip.unit, psfpFmEntryInfo->index, &psfpFmEntryInfo->flowMeterParam);
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
 * @brief To set the entry of a specific flow meter. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpFmEntryInfo Specific flow meter entry parameters (N/A) \n
 *                        Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets all parameters of the entry of the specified flow meter \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_fm_entry_set(), cvb_psfp_fm_entry_set(), cva_psfp_fm_entry_set().
 */
RtkApiRet rtk_psfp_fm_entry_set(UnitChip_t unitChip, PsfpFmEntry_t *psfpFmEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpFmEntryInfo)
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
                ret = dva_psfp_fm_entry_set(unitChip.unit, psfpFmEntryInfo->index, &psfpFmEntryInfo->flowMeterParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_fm_entry_set(unitChip.unit, psfpFmEntryInfo->index, &psfpFmEntryInfo->flowMeterParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_fm_entry_set(unitChip.unit, psfpFmEntryInfo->index, &psfpFmEntryInfo->flowMeterParam);
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
 * @brief To get the MarkAllFramesRed status of a specific flow meter and whether it is enabled or disabled. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpMarkAllFramesRedInfo MarkAllFramesRed status parameters (N/A) \n
 *                                     Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets whether MarkAllFramesRedEnable is enabled or disabled \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_mark_all_frames_red_get(), cvb_psfp_mark_all_frames_red_get(), cva_psfp_mark_all_frames_red_get().
 */
RtkApiRet rtk_psfp_mark_all_frames_red_get(UnitChip_t unitChip, PsfpMarkAllFramesRed_t *psfpMarkAllFramesRedInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpMarkAllFramesRedInfo)
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
                ret = dva_psfp_mark_all_frames_red_get(unitChip.unit, psfpMarkAllFramesRedInfo->index, &psfpMarkAllFramesRedInfo->isEnable,
                                                       &psfpMarkAllFramesRedInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_mark_all_frames_red_get(unitChip.unit, psfpMarkAllFramesRedInfo->index, &psfpMarkAllFramesRedInfo->isEnable,
                                                       &psfpMarkAllFramesRedInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_mark_all_frames_red_get(unitChip.unit, psfpMarkAllFramesRedInfo->index, &psfpMarkAllFramesRedInfo->isEnable,
                                                       &psfpMarkAllFramesRedInfo->status);
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
 * @brief To set the MarkAllFramesRedEnable status of a specific flow meter and determine whether to clear the current status of MarkAllFramesRed. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpMarkAllFramesRedInfo MarkAllFramesRed status parameters (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully enables or disables MarkAllFramesRedEnable. \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_mark_all_frames_red_set(), cvb_psfp_mark_all_frames_red_set(), cva_psfp_mark_all_frames_red_set().
 */
RtkApiRet rtk_psfp_mark_all_frames_red_set(UnitChip_t unitChip, PsfpMarkAllFramesRed_t *psfpMarkAllFramesRedInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpMarkAllFramesRedInfo)
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
                ret = dva_psfp_mark_all_frames_red_set(unitChip.unit, psfpMarkAllFramesRedInfo->index, psfpMarkAllFramesRedInfo->isEnable,
                                                       psfpMarkAllFramesRedInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_mark_all_frames_red_set(unitChip.unit, psfpMarkAllFramesRedInfo->index, psfpMarkAllFramesRedInfo->isEnable,
                                                       psfpMarkAllFramesRedInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_mark_all_frames_red_set(unitChip.unit, psfpMarkAllFramesRedInfo->index, psfpMarkAllFramesRedInfo->isEnable,
                                                       psfpMarkAllFramesRedInfo->status);
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
 * @brief To get the validation status of a specific flow meter. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] psfpFmEntryValidInfo Validation status parameters (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Successfully gets the validation value of the entry of the specified flow meter. \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_fm_entry_valid_get(), cvb_psfp_fm_entry_valid_get(), cva_psfp_fm_entry_valid_get().
 */
RtkApiRet rtk_psfp_fm_entry_valid_get(UnitChip_t unitChip, PsfpFmEntryValid_t *psfpFmEntryValidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpFmEntryValidInfo)
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
                ret = dva_psfp_fm_entry_valid_get(unitChip.unit, psfpFmEntryValidInfo->index, &psfpFmEntryValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_fm_entry_valid_get(unitChip.unit, psfpFmEntryValidInfo->index, &psfpFmEntryValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_fm_entry_valid_get(unitChip.unit, psfpFmEntryValidInfo->index, &psfpFmEntryValidInfo->valid);
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
 * @brief To set the validation status of a specific flow meter. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] psfpFmEntryValidInfo Validation status parameters (N/A) \n
 *                             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the validation value of the entry of the specified flow meter. \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_psfp_fm_entry_valid_set(), cvb_psfp_fm_entry_valid_set(), cva_psfp_fm_entry_valid_set().
 */
RtkApiRet rtk_psfp_fm_entry_valid_set(UnitChip_t unitChip, PsfpFmEntryValid_t *psfpFmEntryValidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == psfpFmEntryValidInfo)
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
                ret = dva_psfp_fm_entry_valid_set(unitChip.unit, psfpFmEntryValidInfo->index, psfpFmEntryValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_psfp_fm_entry_valid_set(unitChip.unit, psfpFmEntryValidInfo->index, psfpFmEntryValidInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_psfp_fm_entry_valid_set(unitChip.unit, psfpFmEntryValidInfo->index, psfpFmEntryValidInfo->valid);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PSFP_METER */
/**@}*/ /* RTK_MODULE_PSCM */
