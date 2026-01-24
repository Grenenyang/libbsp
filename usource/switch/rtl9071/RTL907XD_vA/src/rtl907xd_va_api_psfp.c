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

#include "rtl907xd_va_asicdrv_psfp.h"
#include "rtl907xd_va_asicdrv_tsn_common.h"
#include "rtl907xd_va_api_ext.h"

/**
 * @addtogroup RTK_MODULE_PSCM PSCM(QCI/PSFP & QCR/ATS)
 * @{
 */

/**
 * @addtogroup PSFP_GLO Global Parameters
 * @{
 */
/**
 * @addtogroup PSFP_GLO_ASIC Low Level Driver
 * @brief PSFP Low Level Driver
 * @{
 */

/**
 * @brief To get whether the PSFP module is able to drop the trapped destination port mask.
 * @param[in]  unit                           Stacking unit (1 to 7) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] mode                           Mode (0 to 1) \n
 *                                            The current QCI mode.
 * @retval     RT_ERR_OK                      Successfully obtained PSFP module drop mode.
 * @retval     -RT_ERR_NULL_POINTER           Parameter mode is a null pointer.
 * @retval     Others                         Please refer to dva_reg_field_read().
 */
RtkApiRet dva_psfp_mode_get(uint32 unit, uint32 *mode)
{
    RtkApiRet result = RT_ERR_OK;

    if(NULL == mode)
    {
        result = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* get qci mode from register */
        result = dva_reg_field_read(unit,
                                    DVA_QCI_MODE,
                                    DVA_QCI_MODE_QCI_MODE_LSP,
                                    DVA_QCI_MODE_QCI_MODE_LEN,
                                    mode);
    }

    return result;
}

/**
 * @brief PSFP module initalization and set whether the PSFP module is able to drop the trapped destination port mask.
 * @param[in]  unit                           Stacking unit (1 to 7) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  mode                           Mode (0 to 1) \n
 *                                            Set the QCI mode.
 * @param[out] None
 * @retval     RT_ERR_OK                      Successfully set PSFP module drop mode.
 * @retval     -RT_ERR_OUT_OF_RANGE           Parameter mode is neither 1 nor 0.
 * @retval     Others                         Please refer to dva_reg_field_write().
 */
RtkApiRet dva_psfp_mode_set(uint32 unit, uint32 mode)
{
    RtkApiRet result = RT_ERR_OK;

    if((mode != 0u) && (mode != 1u))
    {
        result = -RT_ERR_OUT_OF_RANGE;
    }
    else
    {
        /* enable qci drop feature */
        result = dva_reg_field_write(unit,
                                     (uint32)DVA_QCI_MODE,
                                     (uint32)DVA_QCI_MODE_QCI_DROP_ENABLE_LSP,
                                     (uint32)DVA_QCI_MODE_QCI_DROP_ENABLE_LEN,
                                     (uint32)DVA_CONFIG_QCI_DROP_ENABLE);
        /* enable ipv feature */
        CHK_FUN_RET(result, dva_reg_field_write(unit,
                                                (uint32)DVA_QCI_MODE,
                                                (uint32)DVA_QCI_MODE_QCI_IPV_ENABLE_LSP,
                                                (uint32)DVA_QCI_MODE_QCI_IPV_ENABLE_LEN,
                                                (uint32)DVA_CONFIG_QCI_IPV_ENABLE));
        /* enable qci dei feature */
        CHK_FUN_RET(result, dva_reg_field_write(unit,
                                                (uint32)DVA_QCI_MODE,
                                                (uint32)DVA_QCI_MODE_QCI_DEI_ENABLE_LSP,
                                                (uint32)DVA_QCI_MODE_QCI_DEI_ENABLE_LEN,
                                                (uint32)DVA_CONFIG_QCI_DEI_ENABLE));

        /* set parameter mode to register */
        CHK_FUN_RET(result, dva_reg_field_write(unit,
                                                (uint32)DVA_QCI_MODE,
                                                (uint32)DVA_QCI_MODE_QCI_MODE_LSP,
                                                (uint32)DVA_QCI_MODE_QCI_MODE_LEN,
                                                mode));
    }

    return result;
}
/**@}*/ /* PSFP_GLO_ASIC */
/**@}*/ /* PSFP_GLO */

/**
 * @addtogroup PSFP_FILTER Stream filter
 * @{
 */
/**
 * @addtogroup PSFP_FILTER_ASIC Low Level Driver
 * @brief PSFP Low Level Driver
 * @{
 */

/**
 * @brief To get stream Handle, priority, stream gate instance ID, maximum stream SDU, and flow meter instance ID of the specified stream filter instance.
 * @param[in]  unit                           Stacking unit (1 to 7) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  index                          Index (0 to MAX_STREAM_FILTER_INSTANCE - 1) \n
 *                                            Specify the index of the stream filter instance.
 * @param[out] psfpStmFilterEntry             PSFP stream filter entry parameters \n
 * @retval     RT_ERR_OK                      Successfully got the entry value of the specific stream filter instance.
 * @retval     -RT_ERR_PSFP_INVALID_ENTRY_INDEX Parameter index is out of range of the maximum stream filter instance.
 * @retval     -RT_ERR_NULL_POINTER           At least one of the parameters stream_handle_spec, priority_spec, \n
 *                                            stream_gate_instance_id, stream_max_SDU, flow_meter_instance_id is a null pointer.
 * @retval     Others                         Please refer to dva_reg_read(), dva_table_read(), dva_reg_field_get(), and dva_drv_psfp_stm_fl_tbl_val_get().
 */
RtkApiRet dva_psfp_stm_filter_entry_get(uint32 unit, uint32 index, PsfpStmFilterParam_t *psfpStmFilterEntry)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           matchRegisterIndex;
    uint32           policyRegisterIndex;
    uint32           matchRegisterValue;
    uint32           policyRegisterValue[2];
    uint32           temp;
    uint32           tableAddress;
    dva_table_list_t tableType;

    do
    {
        /* interface error check */
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        /* interface error check */
        if(NULL == psfpStmFilterEntry)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* read entire register, process fields later */
        matchRegisterIndex = (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0 + (uint32)(NEXT_REG_OFFSET * index);
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, matchRegisterIndex, &matchRegisterValue));

        /* read entire register, process fields later */
        policyRegisterIndex = (uint32)index;
        tableType           = DVA_QCI_STREAM_FILTER;
        tableAddress        = policyRegisterIndex;
        CHK_FUN_CONTINUE(result, dva_table_read(DVA_UNIT, tableType, tableAddress, (uint32 *)&policyRegisterValue));

        /* streamHandleSpec */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(matchRegisterIndex,
                                                   (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_STREAM_HANDLE_SPECIFICATION_LSP,
                                                   (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_STREAM_HANDLE_SPECIFICATION_LEN,
                                                   &temp,
                                                   matchRegisterValue));

        if(1u == temp)
        {
            /* wildcard */
            psfpStmFilterEntry->streamHandleSpec = (int32)-1;
        }
        else
        {
            CHK_FUN_CONTINUE(result, dva_reg_field_get(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_STREAM_HANDLE_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_STREAM_HANDLE_SPECIFICATION_LEN,
                                                       &temp,
                                                       matchRegisterValue));
            psfpStmFilterEntry->streamHandleSpec = (int32)temp;
        }

        /* prioritySpec */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(matchRegisterIndex,
                                                   (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_PRIORITY_SPECIFICATION_LSP,
                                                   (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_PRIORITY_SPECIFICATION_LEN,
                                                   &temp,
                                                   matchRegisterValue));

        if(1u == temp)
        {
            /* wildcard */
            psfpStmFilterEntry->prioritySpec = (int32)-1;
        }
        else
        {
            CHK_FUN_CONTINUE(result, dva_reg_field_get(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_PRIORITY_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_PRIORITY_SPECIFICATION_LEN,
                                                       &temp,
                                                       matchRegisterValue));
            psfpStmFilterEntry->prioritySpec = (int32)temp;
        }
        /* get table entry value */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_stm_fl_tbl_val_get(unit, policyRegisterIndex, &psfpStmFilterEntry->streamGateInstanceId, &psfpStmFilterEntry->streamMaxSDU,
                                                                 &psfpStmFilterEntry->flowMeterInstanceId, &psfpStmFilterEntry->atsSchedulerInstanceId, policyRegisterValue));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To set stream Handle, priority, stream gate instance ID, maximum stream SDU, and flow meter instance ID of the specified stream filter instance.
 * @param[in]  unit                           Stacking unit (1 to 7) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  index                          Index (0 to MAX_STREAM_FILTER_INSTANCE - 1) \n
 *                                            Specify the index of the stream filter instance.
 * @param[out] psfpStmFilterEntry             PSFP stream filter entry parameters \n
 * @param[out] None
 * @retval     RT_ERR_OK                      Successfully set the entry value of the specific stream filter instance.
 * @retval     Others                         Please refer to dva_drv_psfp_sf_set_intf_chk(), dva_drv_psfp_sf_entry_active_chk(), dva_reg_field_set(), \n
 *                                            dva_drv_psfp_stm_fl_tbl_val_set(), and dva_table_write().
 */
RtkApiRet dva_psfp_stm_filter_entry_set(uint32 unit, uint32 index, PsfpStmFilterParam_t *psfpStmFilterEntry)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           matchRegisterIndex;
    uint32           policyRegisterIndex;
    uint32           matchRegisterValue;
    uint32           policyRegisterValue[2];
    uint32           tableAddress;
    dva_table_list_t tableType;

    do
    {
        /* interface error check */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_sf_set_intf_chk(index, psfpStmFilterEntry->streamHandleSpec, psfpStmFilterEntry->prioritySpec, psfpStmFilterEntry->streamGateInstanceId,
                                                              psfpStmFilterEntry->streamMaxSDU, psfpStmFilterEntry->flowMeterInstanceId, psfpStmFilterEntry->atsSchedulerInstanceId));

        /* prepare index to register */
        matchRegisterIndex = (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0 + (uint32)(NEXT_REG_OFFSET * index);

        /* check if target entry is inactive */
        policyRegisterIndex = (uint32)index;

        CHK_FUN_CONTINUE(result, dva_drv_psfp_sf_entry_active_chk(unit, matchRegisterIndex));

        /* initialize value */
        matchRegisterValue     = (uint32)0;
        policyRegisterValue[0] = (uint32)0;
        policyRegisterValue[1] = (uint32)0;

        /* streamHandleSpec */
        if(-1 == psfpStmFilterEntry->streamHandleSpec)
        {
            /* wildcard, no need to set stream handle spec value since it's don't care */
            CHK_FUN_CONTINUE(result, dva_reg_field_set(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_STREAM_HANDLE_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_STREAM_HANDLE_SPECIFICATION_LEN,
                                                       (uint32)TRUE,
                                                       &matchRegisterValue));
        }
        else
        {
            /* set no wildcard */
            CHK_FUN_CONTINUE(result, dva_reg_field_set(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_STREAM_HANDLE_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_STREAM_HANDLE_SPECIFICATION_LEN,
                                                       (uint32)FALSE,
                                                       &matchRegisterValue));

            /* set stream handle spec value */
            CHK_FUN_CONTINUE(result, dva_reg_field_set(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_STREAM_HANDLE_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_STREAM_HANDLE_SPECIFICATION_LEN,
                                                       (uint32)psfpStmFilterEntry->streamHandleSpec,
                                                       &matchRegisterValue));
        }

        /* prioritySpec */
        if(-1 == psfpStmFilterEntry->prioritySpec)
        {
            /* wildcard, no need to set priority value since it's don't care */
            CHK_FUN_CONTINUE(result, dva_reg_field_set(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_PRIORITY_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_PRIORITY_SPECIFICATION_LEN,
                                                       (uint32)TRUE,
                                                       &matchRegisterValue));
        }
        else
        {
            /* set no wildcard */
            CHK_FUN_CONTINUE(result, dva_reg_field_set(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_PRIORITY_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_WILDCARD_PRIORITY_SPECIFICATION_LEN,
                                                       (uint32)FALSE,
                                                       &matchRegisterValue));

            /* set priority value */
            CHK_FUN_CONTINUE(result, dva_reg_field_set(matchRegisterIndex,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_PRIORITY_SPECIFICATION_LSP,
                                                       (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_PRIORITY_SPECIFICATION_LEN,
                                                       (uint32)psfpStmFilterEntry->prioritySpec,
                                                       &matchRegisterValue));
        }

        /* combine table entry value */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_stm_fl_tbl_val_set(unit, policyRegisterIndex, psfpStmFilterEntry->streamGateInstanceId, psfpStmFilterEntry->streamMaxSDU,
                                                                 psfpStmFilterEntry->flowMeterInstanceId, psfpStmFilterEntry->atsSchedulerInstanceId, policyRegisterValue));

        CHK_FUN_CONTINUE(result, dva_reg_write(unit, matchRegisterIndex, matchRegisterValue));

        tableType    = DVA_QCI_STREAM_FILTER;
        tableAddress = policyRegisterIndex;
        CHK_FUN_CONTINUE(result, dva_table_write(unit, tableType, (uint32)tableAddress, (uint32 *)&policyRegisterValue));

    } while(1u == 0u);

    return result;
}
/**
 * @brief To get BlockStreamDueToOversizeFrame enable and status of the specified stream filter instance.
 * @param[in]  unit                           Stacking unit (1 to 7) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  index                          Index (0 to MAX_STREAM_FILTER_INSTANCE - 1) \n
 *                                            Specify the index of the stream filter instance.
 * @param[out] isEnable                       Enable state (0 or 1) \n
 *                                            The truth value of BlockStreamDueToOversizeFrameEnable.
 * @param[out] status                         Current status (0 or 1) \n
 *                                            The truth value of BlockStreamDueToOversizeFrame.
 * @retval     RT_ERR_OK                      Successfully got the truth value of BlockStreamDueToOversizeFrameEnable \n
 *                                            and BlockStreamDueToOversizeFrame.
 * @retval     -RT_ERR_PSFP_INVALID_ENTRY_INDEX Parameter index is out of range of the maximum stream filter instance.
 * @retval     -RT_ERR_NULL_POINTER           At least one of the parameters isEnable and status is a null pointer.
 * @retval     Others                         Please refer to dva_reg_read().
 */
RtkApiRet dva_psfp_stm_block_oversize_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((NULL == isEnable) || (NULL == status))
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* equal to index/32 */
        registerIndex = (uint32)DVA_BLOCKSTREAMDUETOOVERSIZEFRAMEENABLE_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* get the corresponding bit */
        *isEnable = ((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

        /* equal to index/32 */
        registerIndex = (uint32)DVA_BLOCKSTREAMDUETOOVERSIZEFRAME_STATUS_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* get the corresponding bit */
        *status = ((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

    } while(1u == 0u);

    return result;
}

/**
 * @brief To enable or disable BlockStreamDueToOversizeFrame and determine whether to clear the current BlockStreamDueToOversizeFrame status.
 * @param[in]  unit                           Stacking unit (1 to 7) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  index                          Index (0 to MAX_STREAM_FILTER_INSTANCE - 1) \n
 *                                            Specify the index of the stream filter instance.
 * @param[in]  isEnable                       Enable state (0 or 1) \n
 *                                            Set the truth value of BlockStreamDueToOversizeFrameEnable.
 * @param[in]  clearStatus                    Clear status (0 or 1) \n
 *                                            Determine whether to clear the current BlockStreamDueToOversizeFrame status or not.
 * @param[out] None
 * @retval     RT_ERR_OK                      Successfully set the truth value of BlockStreamDueToOversizeFrameEnable \n
 *                                            (and clear the current state of BlockStreamDueToOversize).
 * @retval     -RT_ERR_PSFP_INVALID_ENTRY_INDEX Parameter index is out of range of the maximum stream filter instance.
 * @retval     -RT_ERR_OUT_OF_RANGE           At least one of the values of parameters isEnable and clearStatus is neither TRUE nor FALSE.
 * @retval     Others                         Please refer to dva_reg_read() and dva_reg_write().
 */
RtkApiRet dva_psfp_stm_block_oversize_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((isEnable != 0u) && (isEnable != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((clearStatus != 0u) && (clearStatus != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* read -> update -> write the isEnable */
        /* equal to index/32 */
        registerIndex = (uint32)DVA_BLOCKSTREAMDUETOOVERSIZEFRAMEENABLE_0 + (uint32)((uint32)NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        if(1u == isEnable)
        {
            registerValue |= (uint32)((uint32)0x1u << (index % DVA_BIT_MASK_UNIT));
        }
        else
        {
            registerValue &= (uint32)(~((uint32)0x1u << (index % DVA_BIT_MASK_UNIT)));
        }

        CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));

        /* only need following operation when clear is requested. */
        if(1u == clearStatus)
        {
            /* equal to index/32 */
            registerIndex = (uint32)DVA_BLOCKSTREAMDUETOOVERSIZEFRAME_STATUS_0 + (uint32)((uint32)NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));

            /* this register is write 1 clear */
            registerValue = (uint32)((uint32)0x1u << (index % DVA_BIT_MASK_UNIT));
            CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get whether a specific stream filter entry is valid or not.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_FILTER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream filter instance.
 * @param[out] valid                          Valid (0 or 1)  \n
 *                                            Validation of the stream filter entry.
 * @retval RT_ERR_OK                          Successfully got the validation value of the specified stream filter entry.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream filter instance.
 * @retval -RT_ERR_NULL_POINTER               Parameter valid is a null pointer.
 * @retval Others                             Please refer to dva_reg_field_read().
 */
RtkApiRet dva_psfp_stm_fl_entry_valid_get(uint32 unit, uint32 index, uint32 *valid)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == valid)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Get flow meter entry valid bit */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_FILTERENTRYACTIVE_LSP,
                                                    (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_FILTERENTRYACTIVE_LEN,
                                                    valid));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To set whether a specific stream filter entry is valid or not.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_FILTER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream filter instance.
 * @param[in] valid                           Valid (0 or 1)  \n
 *                                            Validation of the stream filter entry.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the validation value of the specified stream filter entry.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream filter instances.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter valid is neither TRUE nor FALSE.
 * @retval Others                             Please refer to dva_reg_field_write().
 */
RtkApiRet dva_psfp_stm_fl_entry_valid_set(uint32 unit, uint32 index, uint32 valid)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((valid != 0u) && (valid != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Set flow meter entry valid bit */
        CHK_FUN_CONTINUE(result, dva_reg_field_write(unit,
                                                     registerIndex,
                                                     (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_FILTERENTRYACTIVE_LSP,
                                                     (uint32)DVA_STREAM_FILTER_CONTROLMATCH_0_FILTERENTRYACTIVE_LEN,
                                                     valid));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the current count of the specified counter for the specified index.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_FILTER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream filter instance.
 * @param[in] type                            Type (DVA_MATCHING_FRAMES_COUNT to DVA_RED_FRAMES_COUNT)  \n
 *                                            Counter type of frames.
 * @param[out] counter                        Counter (0 to 0xFFFFFFFFFFFFFFFF)  \n
 *                                            Counts of the specified type of counter.
 * @retval RT_ERR_OK                          Successfully got the counts of the counter type within the specified stream filter instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream filter instance.
 * @retval -RT_ERR_PSFP_INVALID_TYPE          Parameter type is an invalid counter type.
 * @retval -RT_ERR_NULL_POINTER               Parameter counter is a null pointer.
 * @retval Others                             Please refer to dva_table_read().
 */
RtkApiRet dva_psfp_frame_counters_get(uint32 unit, uint32 index, uint32 type, uint64 *counter)
{
    RtkApiRet result = RT_ERR_OK;

    uint32           tableAddress;
    uint32           tableData[2];
    dva_table_list_t tableType;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == counter)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(DVA_RED_FRAMES_COUNT < type)
        {
            result = -(RT_ERR_PSFP_INVALID_TYPE);
            continue;
        }
        /* Read table */
        if(DVA_MATCHING_FRAMES_COUNT == type)
        {
            tableType = DVA_QCI_HIT_COUNTER;
        }
        else if(DVA_PASSING_FRAMES_COUNT == type)
        {
            tableType = DVA_QCI_GATE_PASS_COUNTER;
        }
        else if(DVA_NOT_PASSING_FRAMES_COUNT == type)
        {
            tableType = DVA_QCI_GATE_DROP_COUNTER;
        }
        else if(DVA_PASSING_SDU_COUNT == type)
        {
            tableType = DVA_QCI_SDU_PASS_COUNTER;
        }
        else if(DVA_NOT_PASSING_SDU_COUNT == type)
        {
            tableType = DVA_QCI_SDU_DROP_COUNTER;
        }
        else
        {
            /* DVA_RED_FRAMES_COUNT */
            tableType = DVA_QCI_METER_DROP_COUNTER;
        }

        tableAddress = (index);
        /* Read specified QCI table data */
        CHK_FUN_CONTINUE(result, dva_table_read(unit, tableType, tableAddress, (uint32 *)tableData));

        *counter = (((uint64)(tableData[0])) | (((uint64)(tableData[1])) << 32u));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the current count of the specified counter for the specified index.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_FILTER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream filter instance.
 * @param[in] type                            Type (DVA_MATCHING_FRAMES_COUNT to DVA_RED_FRAMES_COUNT)  \n
 *                                            Counter type of frames.
 * @param[in] counter                         Counter (0 to 0xFFFFFFFFFFFFFFFF)  \n
 *                                            Counts of the specified type of counter.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the counts of the counter type within the specified stream filter instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream filter instances.
 * @retval -RT_ERR_PSFP_INVALID_TYPE          Parameter type is an invalid counter type.
 * @retval Others                             Please refer to dva_table_write().
 */
RtkApiRet dva_psfp_frame_counters_set(uint32 unit, uint32 index, uint32 type, uint64 counter)
{
    RtkApiRet result;

    uint32           tableAddress;
    uint32           tableData[2];
    dva_table_list_t tableType;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(DVA_RED_FRAMES_COUNT < type)
        {
            result = -(RT_ERR_PSFP_INVALID_TYPE);
            continue;
        }

        /* Read table, total 6 counters, 0~5 */
        if(DVA_MATCHING_FRAMES_COUNT == type)
        {
            tableType = DVA_QCI_HIT_COUNTER;
        }
        else if(DVA_PASSING_FRAMES_COUNT == type)
        {
            tableType = DVA_QCI_GATE_PASS_COUNTER;
        }
        else if(DVA_NOT_PASSING_FRAMES_COUNT == type)
        {
            tableType = DVA_QCI_GATE_DROP_COUNTER;
        }
        else if(DVA_PASSING_SDU_COUNT == type)
        {
            tableType = DVA_QCI_SDU_PASS_COUNTER;
        }
        else if(DVA_NOT_PASSING_SDU_COUNT == type)
        {
            tableType = DVA_QCI_SDU_DROP_COUNTER;
        }
        else
        {
            /* DVA_RED_FRAMES_COUNT */
            tableType = DVA_QCI_METER_DROP_COUNTER;
        }

        tableAddress = (index);

        tableData[0] = (uint32)(counter & 0xFFFFFFFFu);
        tableData[1] = (uint32)((counter >> 32u) & 0xFFFFFFFFu);
        /* Write specified QCI table data */
        CHK_FUN_CONTINUE(result, dva_table_write(unit, tableType, tableAddress, (uint32 *)tableData));
    } while(1u == 0u);

    return result;
}
/**@}*/ /* PSFP_FILTER_ASIC */
/**@}*/ /* PSFP_FILTER */

/**
 * @addtogroup PSFP_GATE Stream gate
 * @{
 */
/**
 * @addtogroup PSFP_GATE_ASIC Low Level Driver
 * @brief PSFP Low Level Driver
 * @{
 */

/**
 * @brief To get the status of whether a specific stream gate is enabled or disabled.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[out] gateEnabled                    Gate enabled (0 or 1)  \n
 *                                            Status of the specified stream gate instance.
 * @retval RT_ERR_OK                          Successfully got the enable status of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_NULL_POINTER               Parameter gateEnabled is a null pointer.
 * @retval Others                             Please refer to dva_reg_field_read().
 */
RtkApiRet dva_psfp_gate_enabled_get(uint32 unit, uint32 index, uint32 *gateEnabled)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == gateEnabled)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Get PSFP gate enable */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPGATEENABLED_LSP,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPGATEENABLED_LEN,
                                                    gateEnabled));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To enable or disable a specific stream gate control list.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[in] gateEnabled                     Gate enabled (0 or 1)  \n
 *                                            Status of the specified stream gate instance.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the enable status of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter gateEnabled is neither TRUE nor FALSE.
 * @retval Others                             Please refer to dva_drv_psfp_preconfig_chk(), dva_drv_psfp_isdone_chk(), and dva_drv_psfp_gate_enabled_set().
 */
RtkApiRet dva_psfp_gate_enabled_set(uint32 unit, uint32 index, uint32 gateEnabled)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(gateEnabled > 1u)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }
        if(1u == (gateEnabled & 0x1u))
        {
            /* Check PSFP preconfig at runtime */
            CHK_FUN_CONTINUE(result, dva_drv_psfp_preconfig_chk(unit));
        }
        else
        {
            /* Check PSFP isDone flag at runtime */
            CHK_FUN_CONTINUE(result, dva_drv_psfp_isdone_chk(unit));
        }
        /* Set gate enable status */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_gate_enabled_set(unit, index, gateEnabled));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the current admin state of a specified stream gate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the stream gate.
 * @param[out] gateState                      Gate state (0 or 1)  \n
 *                                            Current stream gate state.
 * @retval RT_ERR_OK                          Successfully got the gate state of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_NULL_POINTER               Parameter gateState is a null pointer.
 * @retval Others                             Please refer to dva_reg_field_read().
 */
RtkApiRet dva_psfp_admin_gate_state_get(uint32 unit, uint32 index, uint32 *gateState)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == gateState)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Get admin gate state */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINGATESTATES_LSP,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINGATESTATES_LEN,
                                                    gateState));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the current admin state of a specified stream gate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the stream gate.
 * @param[in] gateState                       Gate state (0 or 1)  \n
 *                                            The target state of the stream gate.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the gate state of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter gateState is neither 0 nor 1.
 * @retval Others                             Please refer to dva_reg_field_write().
 */
RtkApiRet dva_psfp_admin_gate_state_set(uint32 unit, uint32 index, uint32 gateState)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(gateState > 1u)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Set admin gate state */
        CHK_FUN_CONTINUE(result, dva_reg_field_write(unit,
                                                     registerIndex,
                                                     (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINGATESTATES_LSP,
                                                     (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINGATESTATES_LEN,
                                                     gateState));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the current operation state of a specified stream gate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the stream gate.
 * @param[out] gateState                      Gate state (0 or 1)  \n
 *                                            Current stream gate state.
 * @retval RT_ERR_OK                          Successfully got the gate state of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_NULL_POINTER               Parameter gateState is a null pointer.
 * @retval Others                             Please refer to dva_reg_field_read().
 */
RtkApiRet dva_psfp_oper_gate_state_get(uint32 unit, uint32 index, uint32 *gateState)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == gateState)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Get oper gate state */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPOPERGATESTATES_LSP,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPOPERGATESTATES_LEN,
                                                    gateState));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the admin gate configuration of a specified stream gate instance.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream identification entry.
 * @param[out] baseTime                       Base time  \n
 *                                            Base time of the specified stream gate instance.
 * @param[out] cycleTime                      Cycle time (0 to 0xFFFFFFFF)  \n
 *                                            Cycle time of the specified stream gate instance.
 * @param[out] cycleTimeExtension             Cycle time extension  \n
 *                                            Extension cycle time of the specified stream gate instance.
 * @param[out] gateControlList                Gate control list  \n
 *                                            The control list of the specified stream gate instance.
 * @param[out] controlListLength              Control list length (2 to 8)  \n
 *                                            The length of the control list of the specified stream gate instance.
 * @retval RT_ERR_OK                          Successfully got the admin gate configuration of the specified stream gate instance.
 * @retval Others                             Please refer to dva_drv_psfp_conf_get_intf_chk(), dva_drv_psfp_ad_ctrl_lst_le_get(), dva_drv_psfp_admin_ctrl_lst_get(), \n
 *                                            dva_drv_psfp_admin_cycle_time_get(), dva_drv_psfp_ad_cle_time_ext_get() and \n
 *                                            dva_drv_psfp_admin_base_time_get().
 */
RtkApiRet dva_psfp_admin_gate_config_get(uint32                     unit,
                                         uint32                     index,
                                         RtkTsnPtpTime_t           *baseTime,
                                         uint32                    *cycleTime,
                                         uint32                    *cycleTimeExtension,
                                         PsfpGateControlListParam_t gateControlList[],
                                         uint32                    *controlListLength)
{
    RtkApiRet         result = RT_ERR_OK;
    RtkTsnCycleTime_t cycleTimeTmp;

    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_conf_get_intf_chk(index, baseTime, cycleTime, cycleTimeExtension, gateControlList, controlListLength));

        /* Get the current control list length */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_ad_ctrl_lst_le_get(unit, index, controlListLength));

        if((DVA_PSFP_SUPPORT_LIST_MIN <= *controlListLength) && (DVA_PSFP_SUPPORT_LIST_MAX >= *controlListLength))
        {
            /* Get the admin control list */
            CHK_FUN_CONTINUE(result, dva_drv_psfp_admin_ctrl_lst_get(unit, index, gateControlList, *controlListLength));
        }
        /* Get the calculated admin control list cycle time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_admin_cycle_time_get(unit, index, &cycleTimeTmp));
        *cycleTime = cycleTimeTmp.numerator;

        /* Get the extended admin control list cycle time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_ad_cle_time_ext_get(unit, index, cycleTimeExtension));

        /* Get the admin base time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_admin_base_time_get(unit, index, baseTime));

    } while(0u == 1u);

    return result;
}

/**
 * @brief To set the admin gate configuration of a specified stream gate instance.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1) \n
 *                                            Specify the index of the stream identification entry.
 * @param[in] baseTime                        Base time  \n
 *                                            Base time of the specified stream gate instance.
 * @param[in] cycleTimeExtension              Cycle time extension (0 to 0xFFFFFFFF) \n
 *                                            Extension cycle time of the specified stream gate instance.
 * @param[in] gateControlList                 Gate control list  \n
 *                                            The control list of the specified stream gate instance.
 * @param[in] controlListLength               Control list length (2 to 8)  \n
 *                                            The length of the control list of the specified stream gate instance.
 * @param[in] setConfigChange                 Set config change (0 or 1)  \n
 *                                            If the variable value is 1, execute control list configuration change.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the admin gate configuration of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_WAIT_PENDING          List Config state machine to signal that there is a new cycle \n
 *                                            configuration awaiting installation.
 * @retval Others                             Please refer to dva_drv_psfp_conf_set_intf_chk(), dva_drv_psfp_preconfig_chk(), dva_psfp_config_pending_get(), dva_drv_psfp_ad_ctrl_lst_le_set(), \n
 *                                            dva_drv_psfp_admin_ctrl_lst_set(), dva_drv_psfp_admin_cycle_time_set(), \n
 *                                            dva_drv_psfp_ad_cle_time_ext_set(), dva_drv_psfp_admin_base_time_set(), \n
 *                                            dva_drv_psfp_gate_enabled_set(), dva_drv_psfp_config_change_set().
 * @note                                      The maximum time for polling in dva_drv_psfp_config_change_set() is equal to DVA_QCI_ACCESS_GATE_COMPUTEDONE_MAX_US, \n
 *                                            DVA_QCI_ACCESS_GATE_COMPUTEDONE_MAX_US is determined by the frequency of the platform, \n
 *                                            the reference frequency for the value in the SDK is 250MHz.
 */
RtkApiRet dva_psfp_admin_gate_config_set(uint32                           unit,
                                         uint32                           index,
                                         RtkTsnPtpTime_t                 *baseTime,
                                         uint32                           cycleTimeExtension,
                                         const PsfpGateControlListParam_t gateControlList[],
                                         uint32                           controlListLength,
                                         uint32                           setConfigChange)
{
    RtkApiRet         result = RT_ERR_OK;
    uint32            iteratorList;
    uint32            isPending = 0u;
    RtkTsnCycleTime_t cycleTime;
    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_conf_set_intf_chk(index, baseTime, cycleTimeExtension, gateControlList, controlListLength, setConfigChange));
        if(1u == setConfigChange)
        {
            /* Check PSFP preconfig at runtime */
            CHK_FUN_CONTINUE(result, dva_drv_psfp_preconfig_chk(unit));
        }
        /* Even API will check pending, it is recommended to explicitly check before configuration. */
        CHK_FUN_CONTINUE(result, dva_psfp_config_pending_get(unit, index, &isPending));
        if(isPending != 0u)
        {
            result = -(RT_ERR_PSFP_WAIT_PENDING);
            continue;
        }

        /* Set the admin control list length of the specified stream gate */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_ad_ctrl_lst_le_set(unit, index, controlListLength));

        /* Set the admin control list of the specified stream gate */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_admin_ctrl_lst_set(unit, index, gateControlList, controlListLength));

        cycleTime.numerator   = (uint32)0;
        cycleTime.denominator = DVA_TSN_1B_NANOSECOND;
        for(iteratorList = (uint32)0; iteratorList < controlListLength; ++iteratorList)
        {
            cycleTime.numerator += gateControlList[iteratorList].timeInterval;
        }

        /* Set the calculated admin control list cycle time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_admin_cycle_time_set(unit, index, cycleTime));

        /* Set the extended admin control list cycle time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_ad_cle_time_ext_set(unit, index, cycleTimeExtension));

        /* Set the admin base time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_admin_base_time_set(unit, index, baseTime));

        /* Enable the specific stream gate */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_gate_enabled_set(unit, index, ENABLE));

        if(1u == setConfigChange)
        {
            CHK_FUN_CONTINUE(result, dva_drv_psfp_config_change_set(unit, index, (uint32)1));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the operation gate configuration of a specified stream gate instance.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream identification entry.
 * @param[out] baseTime                       Base time  \n
 *                                            Base time of the specified stream gate instance.
 * @param[out] cycleTime                      Cycle time  \n
 *                                            Cycle time of the specified stream gate instance.
 * @param[out] cycleTimeExtension             Cycle time extension (0 to 0xFFFFFFFF)  \n
 *                                            Extension cycle time of the specified stream gate instance.
 * @param[out] gateControlList                Gate control list  \n
 *                                            The control list of the specified stream gate instance.
 * @param[out] controlListLength              Control list length (2 to 8)  \n
 *                                            The length of the control list of the specified stream gate instance.
 * @retval RT_ERR_OK                          Successfully got the operation gate configuration of the specified stream gate instance.
 * @retval Others                             Please refer to dva_drv_psfp_conf_get_intf_chk(), dva_drv_psfp_op_ctrl_lst_le_get(), dva_drv_psfp_oper_ctrl_lst_get(), \n
 *                                            dva_drv_psfp_oper_cycle_time_get(), dva_drv_psfp_op_cle_time_ext_get() and \n
 *                                            dva_drv_psfp_oper_base_time_get().
 */
RtkApiRet dva_psfp_oper_gate_config_get(uint32                     unit,
                                        uint32                     index,
                                        RtkTsnPtpTime_t           *baseTime,
                                        uint32                    *cycleTime,
                                        uint32                    *cycleTimeExtension,
                                        PsfpGateControlListParam_t gateControlList[],
                                        uint32                    *controlListLength)
{
    RtkApiRet         result = RT_ERR_OK;
    RtkTsnCycleTime_t cycleTimeTmp;

    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_conf_get_intf_chk(index, baseTime, cycleTime, cycleTimeExtension, gateControlList, controlListLength));

        /* Get the operation control list length of the index */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_op_ctrl_lst_le_get(unit, index, controlListLength));

        if((DVA_PSFP_SUPPORT_LIST_MIN <= *controlListLength) && (DVA_PSFP_SUPPORT_LIST_MAX >= *controlListLength))
        {
            /* Get the operation control list of the index */
            CHK_FUN_CONTINUE(result, dva_drv_psfp_oper_ctrl_lst_get(unit, index, gateControlList, *controlListLength));
        }
        /* Get the calculated operation control list cycle time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_oper_cycle_time_get(unit, index, &cycleTimeTmp));
        *cycleTime = cycleTimeTmp.numerator;

        /* Get the extended operation control list cycle time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_op_cle_time_ext_get(unit, index, cycleTimeExtension));

        /* Get the operation base time */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_oper_base_time_get(unit, index, baseTime));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the status of CloseGateDueToInvalidRx and whether it is enabled or disabled.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[out] isEnable                       Enable state (0 or 1)  \n
 *                                            GateClosedDueToInvalidRxEnable is enabled or disabled.
 * @param[out] status                         Status (0 or 1)  \n
 *                                            Current status of GateClosedDueToInvalidRx.
 * @retval RT_ERR_OK                          Successfully got whether GateClosedDueToInvalidRxEnable is enabled \n
 *                                            or not and the current status of GateClosedDueToInvalidRx.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream gate instances.
 * @retval -RT_ERR_NULL_POINTER               At least one of the parameters isEnable and status is a null pointer.
 * @retval Others                             Please refer to dva_reg_read().
 */
RtkApiRet dva_psfp_gate_closed_inv_rx_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((NULL == isEnable) || (NULL == status))
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Equal to index/32 */
        registerIndex = (uint32)DVA_CLOSEGATEDUETOINVALIDRXENABLE_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get the corresponding bit */
        *isEnable = (uint32)((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

        /* Equal to index/32 */
        registerIndex = (uint32)DVA_CLOSEGATEDUETOINVALIDRX_STATUS_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get the corresponding bit */
        *status = (uint32)((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

    } while(1u == 0u);

    return result;
}

/**
 * @brief To enable or disable CloseGateDueToInvalidRx and to determine whether to clear the current CloseGateDueToInvalidRx status or not.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[in] isEnable                        Enable state (0 or 1)  \n
 *                                            GateClosedDueToInvalidRx is enabled or disabled.
 * @param[in] clearStatus                     Clear status (0 or 1)  \n
 *                                            Determine whether to clear the current status of \n
 *                                            GateClosedDueToInvalidRx.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully enabled or disabled GateClosedDueToInvalidRxEnable \n
 *                                            (and cleared the current status of GateClosedDueToInvalidRx).
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream gate instances.
 * @retval -RT_ERR_OUT_OF_RANGE               At least one of the parameters isEnable and clearStatus is neither 0 nor 1.
 * @retval Others                             Please refer to dva_reg_read() and dva_reg_write().
 */
RtkApiRet dva_psfp_gate_closed_inv_rx_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((isEnable != 0u) && (isEnable != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((clearStatus != 0u) && (clearStatus != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* Read -> update -> write the isEnable */
        /* Equal to index/32 */
        registerIndex = (uint32)DVA_CLOSEGATEDUETOINVALIDRXENABLE_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        if(1u == isEnable)
        {
            registerValue |= (uint32)((uint32)0x1u << (index % DVA_BIT_MASK_UNIT));
        }
        else
        {
            registerValue &= (uint32)(~((uint32)0x1u << (index % DVA_BIT_MASK_UNIT)));
        }

        CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));

        /* Only need the following operation when clear is requested. */
        if(1u == clearStatus)
        {
            /* Equal to index/32 */
            registerIndex = (uint32)DVA_CLOSEGATEDUETOINVALIDRX_STATUS_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));

            /* This register is write 1 clear */
            registerValue = (uint32)((uint32)0x1u << (index % DVA_BIT_MASK_UNIT));
            CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the status of GateClosedDueToOctetsExceeded and whether it is enabled or disabled.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_FILTER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[out] isEnable                       Enable state (0 or 1)  \n
 *                                            GateClosedDueToOctetsExceeded is enabled or disabled.
 * @param[out] status                         Status (0 or 1)  \n
 *                                            Current status of GateClosedDueToOctetsExceeded.
 * @retval RT_ERR_OK                          Successfully got whether GateClosedDueToOctetsExceededEnable \n
 *                                            is enabled or not and the current status of \n
 *                                            GateClosedDueToOctetsExceeded.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream gate instances.
 * @retval -RT_ERR_NULL_POINTER               At least one of the parameters isEnable or status is a null pointer.
 * @retval Others                             Please refer to dva_reg_read().
 */
RtkApiRet dva_psfp_gate_closed_oct_exd_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((NULL == isEnable) || (NULL == status))
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Equal to index/32 */
        registerIndex = (uint32)DVA_CLOSEGATEDUETOOCTETSEXCEEDEDENABLE_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get the corresponding bit */
        *isEnable = (uint32)((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

        /* Equal to index/32 */
        registerIndex = (uint32)DVA_CLOSEGATEDUETOOCTETSEXCEEDED_STATUS_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get the corresponding bit */
        *status = (uint32)((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

    } while(1u == 0u);

    return result;
}

/**
 * @brief To enable or disable GateClosedDueToOctetsExceeded and to determine whether to clear the current GateClosedDueToOctetsExceeded status or not.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_FILTER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[in] isEnable                        Enable state (0 or 1)  \n
 *                                            GateClosedDueToOctetsExceeded is enabled or disabled.
 * @param[out] clearStatus                    Clear status (0 or 1)  \n
 *                                            Determine whether to clear the current status of GateClosedDueToOctetsExceeded.
 * @retval RT_ERR_OK                          Successfully enabled or disabled \n
 *                                            GateClosedDueToOctetsExceededEnable (and cleared the current \n
 *                                            status of GateClosedDueToOctetsExceeded).
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream gate \n
 *                                            instances.
 * @retval -RT_ERR_OUT_OF_RANGE               At least one of the parameters isEnable or clearStatus is neither 0 nor 1.
 * @retval Others                             Please refer to dva_reg_read() and dva_reg_write().
 */
RtkApiRet dva_psfp_gate_closed_oct_exd_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((isEnable != 0u) && (isEnable != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((clearStatus != 0u) && (clearStatus != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* Read -> update -> write the isEnable */
        /* Equal to index/32 */
        registerIndex = (uint32)DVA_CLOSEGATEDUETOOCTETSEXCEEDEDENABLE_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        if(1u == isEnable)
        {
            registerValue |= (uint32)((uint32)0x1u << (index % DVA_BIT_MASK_UNIT));
        }
        else
        {
            registerValue &= (uint32)(~((uint32)0x1u << (index % DVA_BIT_MASK_UNIT)));
        }

        CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));

        /* Only need the following operation when clear is requested. */
        if(1u == clearStatus)
        {
            /* Equal to index/32 */
            registerIndex = (uint32)DVA_CLOSEGATEDUETOOCTETSEXCEEDED_STATUS_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));

            /* This register is write 1 clear */
            registerValue = ((uint32)0x1u << (index % DVA_BIT_MASK_UNIT));
            CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the validation of a specific stream gate entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[out] valid                          Valid (0 or 1)  \n
 *                                            Validation of the stream gate entry.
 * @retval RT_ERR_OK                          Successfully got the validation value of the specified stream gate \n
 *                                            instance entry.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream gate instances.
 * @retval -RT_ERR_NULL_POINTER               Parameter valid is a null pointer.
 * @retval Others                             Please refer to dva_reg_field_read().
 */
/* Currently this is the dummy bit for SDK to record which one is active/inactive */
RtkApiRet dva_psfp_stm_gate_ent_valid_get(uint32 unit, uint32 index, uint32 *valid)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((NULL == valid))
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Get stream gate entry valid bit */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_GATEENTRYACTIVE_LSP,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_GATEENTRYACTIVE_LEN,
                                                    valid));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the validation of a specific stream gate entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[in] valid                           Valid (0 or 1)  \n
 *                                            Validation of the stream gate entry.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the validation value of the specified stream gate \n
 *                                            instance entry.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream filter \n
 *                                            instances.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter valid is neither 0 nor 1.
 * @retval Others                             Please refer to dva_drv_psfp_stm_gate_ent_valid_set().
 */
/* Currently this is the dummy bit for SDK to record which one is active/inactive */
RtkApiRet dva_psfp_stm_gate_ent_valid_set(uint32 unit, uint32 index, uint32 valid)
{
    RtkApiRet result = RT_ERR_OK;
    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((valid != 0u) && (valid != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }
        /* Set stream gate entry valid bit */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_stm_gate_ent_valid_set(unit, index, valid));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the configuration change time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[out] configChangeTime               Config change time (N/A) \n
 *                                            The calculated configuration change time.
 * @retval RT_ERR_OK                          Successfully got the configuration change time.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum flow meter instances.
 * @retval -RT_ERR_NULL_POINTER               Parameter configChangeTime is a null pointer.
 * @retval Others                             Please refer to dva_reg_read().
 */
RtkApiRet dva_psfp_config_change_time_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *configChangeTime)
{
    RtkApiRet                  result = RT_ERR_OK;
    uint32                     registerIndex;
    uint32                     registerValue;
    dva_RtkTsnLongNanosecond_t configChangeTimeLongNs;
    RtkTsnPtpTime_t            configChangeTimetemp = {0};
    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == configChangeTime)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get config change time for config change, bit 31:0 for nanoseconds */
        registerIndex = (uint32)DVA_STREAM_GATE_CONFIGCHANGETIME_0_31TO0 + (uint32)(index * (uint32)3u * (uint32)4u);
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));
        configChangeTimeLongNs.section[0] = registerValue;

        /* Get config change time for config change, bit 63:32 for nanoseconds */
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex + (uint32)((uint32)1u * (uint32)4u), &registerValue));
        configChangeTimeLongNs.section[1] = registerValue;

        /* Get config change time for config change, bit 79:64 for nanoseconds */
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex + (uint32)((uint32)2u * (uint32)4u), &registerValue));
        configChangeTimeLongNs.section[2] = registerValue & (uint32)0xFFFFu;

        /* Convert to PTP format */
        dva_drv_tsn_cov_ns_to_ptptime(configChangeTimeLongNs, &configChangeTimetemp);
        dva_drv_tsn_cov_half_ptptime(&configChangeTimetemp, configChangeTime);

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the current time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] currentTime        Current time (N/A) \n
 *                                Current PTP time from the specified clock.
 * @retval RT_ERR_OK              Successfully got the current time.
 * @retval -RT_ERR_NULL_POINTER   Parameter currentTime is a null pointer.
 * @retval Others                 Please refer to dva_drv_tsn_current_time_get.
 */
RtkApiRet dva_psfp_current_time_get(uint32 unit, RtkTsnPtpTime_t *currentTime)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(NULL == currentTime)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get current time */
        CHK_FUN_CONTINUE(result, dva_drv_tsn_current_time_get(unit, currentTime));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get whether it is pending status of changing configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the stream gate instance.
 * @param[out] configPending                  Configuration pending status (0 to 1) \n
 *                                            Get TRUE by the List Config state machine to signal \n
 *                                            that there is a new cycle configuration awaiting installation. \n
 *                                            Get FALSE when the List Config state machine has installed the \n
 *                                            new configuration.
 * @retval RT_ERR_OK                          Successfully got whether it is pending status \n
 *                                            of changing configuration.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum stream gate instances.
 * @retval -RT_ERR_NULL_POINTER               Parameter configPending is a null pointer.
 * @retval Others                             Please refer to dva_reg_field_read().
 */
RtkApiRet dva_psfp_config_pending_get(uint32 unit, uint32 index, uint32 *configPending)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == configPending)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Get config pending value from register */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPCONFIGPENDING_LSP,
                                                    (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPCONFIGPENDING_LEN,
                                                    configPending));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the admin internal priority specification value.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the stream gate instance.
 * @param[out] IPV                            Stream gate internal priority value (-1 to 7)  \n
 *                                            IPV of the specified stream gate instance.
 * @retval RT_ERR_OK                          Successfully got the IPV of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_NULL_POINTER               Parameter IPV is a null pointer.
 * @retval Others                             Please refer to dva_reg_read(), dva_reg_field_get() and dva_reg_field_get().
 */
RtkApiRet dva_psfp_admin_ipv_get(uint32 unit, uint32 index, int32 *IPV)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;
    uint32    bypass;
    uint32    tmp;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == IPV)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* IPV */
        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get admin IPV value from register */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_ADMINBYPASSIPV_LSP,
                                                   (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_ADMINBYPASSIPV_LEN,
                                                   &bypass,
                                                   registerValue));
        if(1u == bypass)
        {
            /* Bypass */
            *IPV = (int32)-1;
        }
        else
        {
            CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                       DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINIPV_LSP,
                                                       DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINIPV_LEN,
                                                       &tmp,
                                                       registerValue));
            *IPV = (int32)tmp;
        }
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the admin internal priority specification value.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the stream gate instance.
 * @param[in] IPV                             Stream gate internal priority value (-1 to 7)  \n
 *                                            IPV of the specified stream gate instance.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the IPV of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter IPV is not in the range of -1 to 7.
 * @retval Others                             Please refer to dva_reg_field_write().
 */
RtkApiRet dva_psfp_admin_ipv_set(uint32 unit, uint32 index, int32 IPV)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((IPV < DVA_PRIORITY_WILDCARD) || (IPV > DVA_MAX_PRIORITY_VALUE))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* IPV */
        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);
        if(IPV == -1)
        {
            /* Enable admin stream gate bypass IPV */
            CHK_FUN_CONTINUE(result, dva_reg_field_write(unit,
                                                         registerIndex,
                                                         (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_ADMINBYPASSIPV_LSP,
                                                         (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_ADMINBYPASSIPV_LEN,
                                                         (uint32)1));

            /* Not required, but play safe. */
            CHK_FUN_CONTINUE(result, dva_reg_field_write(unit,
                                                         registerIndex,
                                                         (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINIPV_LSP,
                                                         (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINIPV_LEN,
                                                         (uint32)0));
        }
        else
        {
            /* Disable admin stream gate bypass IPV */
            CHK_FUN_CONTINUE(result, dva_reg_field_write(unit,
                                                         registerIndex,
                                                         (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_ADMINBYPASSIPV_LSP,
                                                         (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_ADMINBYPASSIPV_LEN,
                                                         (uint32)0));
            /* Enable admin stream gate IPV value */
            CHK_FUN_CONTINUE(result, dva_reg_field_write(unit,
                                                         registerIndex,
                                                         DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINIPV_LSP,
                                                         DVA_STREAM_GATE_CONTROL_STATUS0_PSFPADMINIPV_LEN,
                                                         (uint32)IPV));
        }
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the operation internal priority specification value.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the stream gate instance.
 * @param[out] IPV                            Stream gate internal priority value (-1 to 7)  \n
 *                                            IPV of the specified stream gate instance.
 * @retval RT_ERR_OK                          Successfully got the operation IPV of the specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_NULL_POINTER               Parameter IPV is a null pointer.
 * @retval Others                             Please refer to dva_reg_read() and dva_reg_field_get().
 */
RtkApiRet dva_psfp_oper_ipv_get(uint32 unit, uint32 index, int32 *IPV)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;
    uint32    bypass;
    uint32    tmp;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == IPV)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* IPV */
        registerIndex = (uint32)DVA_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get stream gate IPV bypass bit */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_OPERBYPASSIPV_LSP,
                                                   (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_OPERBYPASSIPV_LEN,
                                                   &bypass,
                                                   registerValue));

        if(1u == bypass)
        {
            /* Bypass */
            *IPV = (int32)-1;
        }
        else
        {
            /* Get stream gate IPV value */
            CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                       (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPOPERIPV_LSP,
                                                       (uint32)DVA_STREAM_GATE_CONTROL_STATUS0_PSFPOPERIPV_LEN,
                                                       &tmp,
                                                       registerValue));
            *IPV = (int32)tmp;
        }
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get configuration change error counts.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the stream gate instance.
 * @param[out] configChangeErrorCounter       Configuration change error count (0 to 0xFFFFFFFFFFFFFFFF)  \n
 *                                            The count of configuration change errors.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of the maximum stream gate instance.
 * @retval -RT_ERR_NULL_POINTER               Parameter configChangeErrorCounter is a null pointer.
 * @retval RT_ERR_OK                          Successfully got the configuration change error count of the specified stream gate instance.
 * @retval Others                             Please refer to dva_reg_read(), dva_reg_write().
 */
RtkApiRet dva_psfp_conf_change_err_get(uint32 unit, uint32 index, uint64 *configChangeErrorCounter)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerValue;
    uint32    isProcessing;
    uint32    pollDowncount;
    uint32    pollResult;
    uint32    counterMsb;
    uint32    counterLsb;
    uint32    breakFlag;

    do
    {
        if(index >= DVA_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == configChangeErrorCounter)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Without FW set 0, with FW set 1 */
#if 1
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_PSFP_ACCESS, &registerValue));

        isProcessing = (uint32)((registerValue >> 1u) & 0x1u);
        if(1u == isProcessing)
        {
            continue;
        }

        /* Set bit 0: tool request, clear other bits */
        /* Set index */
        registerValue = ((index << 8u) | DVA_PSFP_COUNTER_COMMAND_REQUEST);
        CHK_FUN_CONTINUE(result, dva_reg_write(unit, (uint32)DVA_PSFP_ACCESS, registerValue));

        pollDowncount = DVA_PSFP_POLLING_COUNT;
        breakFlag     = DVA_PSFP_POLLING_NOT_YET;
        while(DVA_PSFP_POLLING_NOT_YET == breakFlag)
        {
            result = dva_reg_read(unit, (uint32)DVA_PSFP_ACCESS, &registerValue);
            if(result != RT_ERR_OK)
            {
                breakFlag = DVA_PSFP_POLLING_NOT_DONE;
                continue;
            }

            /* Only check bit 3:0 */
            pollResult = (uint32)(registerValue & 0xFu);

            /* Bit 2: FW done */
            if(DVA_PSFP_COUNTER_COMMAND_DONE == pollResult)
            {
                breakFlag = DVA_PSFP_POLLING_NOT_DONE;
                continue;
            }

            /* Bit 3: error if any */
            if(DVA_PSFP_COUNTER_COMMAND_ERROR == pollResult)
            {
                result    = -(RT_ERR_PSFP_REQUEST_FAIL);
                breakFlag = DVA_PSFP_POLLING_NOT_DONE;
                continue;
            }

            if(0u == pollDowncount)
            {
                result    = -(RT_ERR_PSFP_REQUEST_FAIL);
                breakFlag = DVA_PSFP_POLLING_NOT_DONE;
                continue;
            }
            --pollDowncount;
        }

        if(result != (int32)RT_ERR_OK)
        {
            continue;
        }

        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_PSFP_COUNTER_MSB, &counterMsb));

        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_PSFP_COUNTER_LSB, &counterLsb));

        *configChangeErrorCounter = (((uint64)counterLsb) | (((uint64)counterMsb) << 32u));
#else
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_CONFIGCHANGEERROR_STATUS, &registerValue));

        *configChangeErrorCounter = (registerValue & 0xFFFFFFu);
#endif
    } while(1u == 0u);

    return result;
}
/**@}*/ /* PSFP_GATE_ASIC */
/**@}*/ /* PSFP_GATE */

/**
 * @addtogroup PSFP_METER Flow meter
 * @{
 */
/**
 * @addtogroup PSFP_METER_ASIC Low Level Driver
 * @brief PSFP Low Level Driver
 * @{
 */

/**
 * @brief To get the entry of a specific flow meter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the flow meter instance.
 * @param[out] flowMeterEntry                 Flow meter entry  \n
 *                                            Specify the parameters for the flow meter instance, such as CIR, CBS, etc.
 * @retval RT_ERR_OK                          Successfully got all parameters of the entry of the specified flow meter \n
 *                                            instance.
 * @retval Others                             Please refer to dva_drv_psfp_fm_get_intf_chk(), dva_reg_read(), dva_drv_psfp_conv_token_to_rate(), \n
 *                                            dva_drv_psfp_conv_token_to_rate() and dva_reg_field_get().
 */
/* CIR, EIR unit is changed from "1 bps" to "1000 bps" to support 5Gbps with 32bit */
/* CBS and EBS unit is byte */
RtkApiRet dva_psfp_fm_entry_get(uint32 unit, uint32 index, PsfpFlowMeterParam_t *flowMeterEntry)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    controlRegisterValue;
    uint32    token;

    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_fm_get_intf_chk(index, flowMeterEntry));

        /* CIR */
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_FLOW_METER_COMMITTED_INFORMATION_RATE_0 + (uint32)(NEXT_REG_OFFSET * index), &token));
        dva_drv_psfp_conv_token_to_rate(token, &flowMeterEntry->committedInfoRate);

        /* CBS */
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_FLOW_METER_COMMITTED_BURST_SIZE_0 + (uint32)(NEXT_REG_OFFSET * index), &flowMeterEntry->committedBurstSize));

        /* EIR */
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_FLOW_METER_EXCESS_INFORMATION_RATE_0 + (uint32)(NEXT_REG_OFFSET * index), &token));
        dva_drv_psfp_conv_token_to_rate(token, &flowMeterEntry->excessInfoRate);

        /* EBS */
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, (uint32)DVA_FLOW_METER_EXCESS_BURST_SIZE_0 + (uint32)(NEXT_REG_OFFSET * index), &flowMeterEntry->excessBurstSize));

        registerIndex = (uint32)DVA_FLOW_METER_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &controlRegisterValue));

        /* Coupling flag */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COUPLINGFLAG_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COUPLINGFLAG_LEN,
                                                   &flowMeterEntry->couplingFlag,
                                                   controlRegisterValue));

        /* Color mode */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COLORMODE_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COLORMODE_LEN,
                                                   &flowMeterEntry->colorMode,
                                                   controlRegisterValue));

        /* Drop on yellow */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_DROPONYELLOW_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_DROPONYELLOW_LEN,
                                                   &flowMeterEntry->dropOnYellow,
                                                   controlRegisterValue));

        /* Selection DEI */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERDEISELECTION_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERDEISELECTION_LEN,
                                                   &flowMeterEntry->selectionDEI,
                                                   controlRegisterValue));

        /* Enable IPV for yellow */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOWENABLE_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOWENABLE_LEN,
                                                   &flowMeterEntry->isEnableIPVForYellow,
                                                   controlRegisterValue));

        /* Yellow IPV */
        CHK_FUN_CONTINUE(result, dva_reg_field_get(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOW_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOW_LEN,
                                                   &flowMeterEntry->yellowIPV,
                                                   controlRegisterValue));

    } while(0u == 1u);

    return result;
}

/**
 * @brief To set the entry of a specific flow meter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the flow meter instance.
 * @param[in] flowMeterEntry                  Flow meter entry  \n
 *                                            Specify the parameters for the flow meter instance, such as CIR, CBS, etc.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set all parameters of the entry of the specified flow meter \n
 *                                            instance.
 * @retval Others                             Please refer to dva_drv_psfp_fm_set_intf_chk(), dva_reg_read(), dva_drv_psfp_fm_active_entry_chk(), dva_drv_psfp_conv_rate_to_token(), \n
 *                                            dva_reg_write() and dva_reg_field_set().
 */
RtkApiRet dva_psfp_fm_entry_set(uint32 unit, uint32 index, PsfpFlowMeterParam_t *flowMeterEntry)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    controlRegisterValue;
    uint32    token;

    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_fm_set_intf_chk(unit, index, *flowMeterEntry));

        registerIndex = (uint32)DVA_FLOW_METER_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &controlRegisterValue));

        /* Should not change active entry, check before updating register */
        CHK_FUN_CONTINUE(result, dva_drv_psfp_fm_active_entry_chk(registerIndex, controlRegisterValue));

        /* CIR */
        dva_drv_psfp_conv_rate_to_token(flowMeterEntry->committedInfoRate, &token);
        CHK_FUN_CONTINUE(result, dva_reg_write(unit, (uint32)DVA_FLOW_METER_COMMITTED_INFORMATION_RATE_0 + (uint32)(NEXT_REG_OFFSET * index), token));

        /* CBS */
        CHK_FUN_CONTINUE(result, dva_reg_write(unit, (uint32)DVA_FLOW_METER_COMMITTED_BURST_SIZE_0 + (uint32)(NEXT_REG_OFFSET * index), flowMeterEntry->committedBurstSize));

        /* EIR */
        dva_drv_psfp_conv_rate_to_token(flowMeterEntry->excessInfoRate, &token);
        CHK_FUN_CONTINUE(result, dva_reg_write(unit, (uint32)DVA_FLOW_METER_EXCESS_INFORMATION_RATE_0 + (uint32)(NEXT_REG_OFFSET * index), token));

        /* EBS */
        CHK_FUN_CONTINUE(result, dva_reg_write(unit, (uint32)DVA_FLOW_METER_EXCESS_BURST_SIZE_0 + (uint32)(NEXT_REG_OFFSET * index), flowMeterEntry->excessBurstSize));

        /* Coupling flag */
        CHK_FUN_CONTINUE(result, dva_reg_field_set(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COUPLINGFLAG_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COUPLINGFLAG_LEN,
                                                   flowMeterEntry->couplingFlag,
                                                   &controlRegisterValue));

        /* Color mode */
        CHK_FUN_CONTINUE(result, dva_reg_field_set(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COLORMODE_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_COLORMODE_LEN,
                                                   flowMeterEntry->colorMode,
                                                   &controlRegisterValue));

        /* Drop on yellow */
        CHK_FUN_CONTINUE(result, dva_reg_field_set(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_DROPONYELLOW_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_DROPONYELLOW_LEN,
                                                   flowMeterEntry->dropOnYellow,
                                                   &controlRegisterValue));

        /* Selection DEI */
        CHK_FUN_CONTINUE(result, dva_reg_field_set(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERDEISELECTION_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERDEISELECTION_LEN,
                                                   flowMeterEntry->selectionDEI,
                                                   &controlRegisterValue));

        /* Enable IPV for yellow */
        CHK_FUN_CONTINUE(result, dva_reg_field_set(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOWENABLE_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOWENABLE_LEN,
                                                   flowMeterEntry->isEnableIPVForYellow,
                                                   &controlRegisterValue));

        /* Yellow IPV */
        CHK_FUN_CONTINUE(result, dva_reg_field_set(registerIndex,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOW_LSP,
                                                   (uint32)DVA_FLOW_METER_CONTROL_STATUS0_IPVFORYELLOW_LEN,
                                                   flowMeterEntry->yellowIPV,
                                                   &controlRegisterValue));

        /* Write the data to register */
        CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, controlRegisterValue));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the MarkAllFramesRed status of a specific flow meter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the flow meter instance.
 * @param[out] isEnable                       Enable state (0 or 1)  \n
 *                                            MarkAllFramesRed is enabled or disabled.
 * @param[out] status                         Status (0 or 1)  \n
 *                                            Current status of MarkAllFramesRed.
 * @retval RT_ERR_OK                          Successfully got whether MarkAllFramesRedEnable is enabled or disabled \n
 *                                            and the current state of MarkAllFramesRed.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum flow meter instances.
 * @retval -RT_ERR_NULL_POINTER               At least one of the parameters isEnable and status is a null pointer.
 * @retval Others                             Please refer to dva_reg_read().
 */
RtkApiRet dva_psfp_mark_all_frames_red_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_FLOW_METER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((NULL == isEnable) || (NULL == status))
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Equal to index/32 */
        registerIndex = (uint32)DVA_MARKALLFRAMEREDENABLE_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get the corresponding bit */
        *isEnable = (uint32)((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

        /* Equal to index/32 */
        registerIndex = (uint32)DVA_MARKALLFRAMERED_STATUS_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        /* Get the corresponding bit */
        *status = (uint32)((registerValue >> (index % DVA_BIT_MASK_UNIT)) & 0x1u);

    } while(0u == 1u);

    return result;
}

/**
 * @brief To set the MarkAllFramesRedEnable status of a specific flow meter and determine whether to clear the current status of MarkAllFramesRed.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the flow meter instance.
 * @param[in] isEnable                        Enable state (0 or 1)  \n
 *                                            MarkAllFramesRedEnable is enabled or disabled.
 * @param[in] clearStatus                     Clear status (0 or 1)  \n
 *                                            Determine whether to clear the current status of MarkAllFramesRed.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully enabled or disabled MarkAllFramesRedEnable \n
 *                                            (and cleared the current status of MarkAllFramesRed).
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum flow meter instances.
 * @retval -RT_ERR_OUT_OF_RANGE               At least one of the parameters isEnable and clearStatus is neither 0 nor 1.
 * @retval Others                             Please refer to dva_reg_read() and dva_reg_write().
 */
RtkApiRet dva_psfp_mark_all_frames_red_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;

    do
    {
        if(index >= DVA_PSFP_MAX_FLOW_METER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((isEnable != 0u) && (isEnable != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((clearStatus != 0u) && (clearStatus != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* Read -> update -> write the isEnable */
        /* Equal to index/32 */
        registerIndex = (uint32)DVA_MARKALLFRAMEREDENABLE_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
        CHK_FUN_CONTINUE(result, dva_reg_read(unit, registerIndex, &registerValue));

        if(1u == isEnable)
        {
            registerValue |= (uint32)((uint32)0x1u << (index % DVA_BIT_MASK_UNIT));
        }
        else
        {
            registerValue &= (uint32)(~((uint32)0x1u << (index % DVA_BIT_MASK_UNIT)));
        }

        CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));

        /* Only need the following operation when clear is requested. */
        if(1u == clearStatus)
        {
            /* Equal to index/32 */
            registerIndex = (uint32)DVA_MARKALLFRAMERED_STATUS_0 + (uint32)(NEXT_REG_OFFSET * (index >> DVA_DIVISOR_BIT_MASK_UNIT));
            CHK_FUN_CONTINUE(result, dva_reg_write(unit, registerIndex, registerValue));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the validation status of a specific flow meter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the flow meter instance.
 * @param[out] valid                          Validation (0 or 1)  \n
 *                                            Validation of the flow meter entry.
 * @retval RT_ERR_OK                          Successfully got the validation value of the entry of the specified flow \n
 *                                            meter.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum flow meter instances.
 * @retval -RT_ERR_NULL_POINTER               Parameter valid is a null pointer.
 * @retval Others                             Please refer to dva_reg_field_read().
 */
RtkApiRet dva_psfp_fm_entry_valid_get(uint32 unit, uint32 index, uint32 *valid)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= DVA_PSFP_MAX_FLOW_METER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((NULL == valid))
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        registerIndex = (uint32)DVA_FLOW_METER_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        /* Get flow meter valid bit */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERENTRYACTIVE_LSP,
                                                    (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERENTRYACTIVE_LEN,
                                                    valid));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the validation status of a specific flow meter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_FLOW_METER_INSTANCE - 1)  \n
 *                                            Specify the index of the flow meter instance.
 * @param[in] valid                           Validation (0 or 1)  \n
 *                                            Validation of the flow meter entry.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the validation value of the entry of the specified flow \n
 *                                            meter.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of the number of maximum flow meter instances.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter valid is neither 0 nor 1.
 * @retval Others                             Please refer to dva_psfp_fm_entry_set() and \n
 *                                            dva_reg_field_write().
 */
RtkApiRet dva_psfp_fm_entry_valid_set(uint32 unit, uint32 index, uint32 valid)
{
    RtkApiRet            result = RT_ERR_OK;
    uint32               registerIndex;
    PsfpFlowMeterParam_t flowMeterEntry;

    do
    {
        if(index >= DVA_PSFP_MAX_FLOW_METER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((valid != 0u) && (valid != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        registerIndex = (uint32)DVA_FLOW_METER_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);

        if(0u == valid)
        {
            /* Set CBS/EBS = 0 if inactivating flow meter */
            flowMeterEntry.committedInfoRate    = 0u;
            flowMeterEntry.committedBurstSize   = 0u;
            flowMeterEntry.excessInfoRate       = 0u;
            flowMeterEntry.excessBurstSize      = 0u;
            flowMeterEntry.couplingFlag         = 0u;
            flowMeterEntry.colorMode            = 0u;
            flowMeterEntry.dropOnYellow         = 0u;
            flowMeterEntry.selectionDEI         = 0u;
            flowMeterEntry.isEnableIPVForYellow = 0u;
            flowMeterEntry.yellowIPV            = 0u;

            CHK_FUN_CONTINUE(result, dva_psfp_fm_entry_set(unit, index, &flowMeterEntry));
        }

        /* Set flow meter valid bit */
        CHK_FUN_CONTINUE(result, dva_reg_field_write(unit,
                                                     registerIndex,
                                                     (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERENTRYACTIVE_LSP,
                                                     (uint32)DVA_FLOW_METER_CONTROL_STATUS0_FLOWMETERENTRYACTIVE_LEN,
                                                     valid));

    } while(1u == 0u);

    return result;
}
/**@}*/ /* PSFP_METER_ASIC */
/**@}*/ /* PSFP_METER */
/**@}*/ /* RTK_MODULE_PSCM */
