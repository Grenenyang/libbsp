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

#include "rtl907xc_vb_asicdrv_tsn_common.h"
#include "rtl907xc_vb_asicdrv_psfp.h"
#include "rtkas_endian.h"
#include "rtkas_system_porting.h"
#include "rtkas_timer.h"
#include "rtl907xc_vb_asicdrv_ptp.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
static uint32 cvb_psfpAdminIndex[CVB_PSFP_MAX_STREAM_GATE_INSTANCE];
static uint32 cvb_psfpOperIndex[CVB_PSFP_MAX_STREAM_GATE_INSTANCE];
uint32        cvb_g_psfpTimeoutCounter = PSFP_CONFIG_CHANGE_TIMEOUT;

static RtkApiRet cvb_drv_psfp_update_admin_oper_index(uint32 unit, uint32 index);
static RtkApiRet cvb_drv_psfp_estimation_time_set(uint32 unit, uint32 index, cvb_RtkTsnLongNanosecond_t estimatedLongNs);
static RtkApiRet cvb_drv_psfp_polling_done_bit(uint32 unit);
/**
 * @brief To check if psfp isDone flag is ready.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully check the psfp isDone flag  \n
 * @retval -RT_ERR_PSFP_WAIT_ISDONE           psfp isDone flag is not ready \n
 * @retval Others                             Please refer to cvb_drv_indirect_sram_get().
 */
RtkApiRet cvb_drv_psfp_isdone_chk(uint32 unit)
{
    uint32    regAddr = 0;
    uint32    isDone  = 0;
    RtkApiRet result  = RT_ERR_OK;
    do
    {
        /* Get isDone flag in firmware */
        regAddr = CVB_PSFP_CONFIG_DONE_ADDRESS;
        CHK_FUN_CONTINUE(result, cvb_drv_indirect_sram_get(unit, regAddr, &isDone));
        /* Check if isDone is 0 */
        if(0u == (isDone & 0x1u))
        {
            result = -RT_ERR_PSFP_WAIT_ISDONE;
            continue;
        }
    } while(0u == 1u);
    return result;
}

/**
 * @brief To check if psfp preconfig is ready.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully check the psfp preconfig \n
 * @retval -RT_ERR_PSFP_WAIT_PRECONFIG        psfp preconfig is not ready \n
 * @retval Others                             Please refer to cvb_drv_indirect_sram_get().
 */
RtkApiRet cvb_drv_psfp_preconfig_chk(uint32 unit)
{
    uint32    regAddr  = 0;
    uint32    isDone   = 0;
    uint32    isPtpRdy = 0;
    RtkApiRet result   = RT_ERR_OK;
    do
    {
        /* Get isDone flag in firmware */
        regAddr = CVB_PSFP_CONFIG_DONE_ADDRESS;
        CHK_FUN_CONTINUE(result, cvb_drv_indirect_sram_get(unit, regAddr, &isDone));
        /* Get isPtpRdy flag in firmware */
        regAddr = (uint32)CVB_PTP_ISREADY_ADDRESS;
        CHK_FUN_CONTINUE(result, cvb_drv_indirect_sram_get(unit, regAddr, &isPtpRdy));
        /* Check if isDone is 0 or ptp is not ready */
        if((0u == (isDone & 0x1u)) || (0u == (isPtpRdy & 0x1u)))
        {
            result = -RT_ERR_PSFP_WAIT_PRECONFIG;
            continue;
        }
    } while(0u == 1u);
    return result;
}
/**
 * @brief To enable or disable specific stream gate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of stream gate instance
 * @param[in] gateEnabled                     Gate enabled (0 or 1)  \n
 *                                            Status of the specified stream gate instance
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the enable status of specified stream gate instance.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of range of maximum stream gate instance.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter gate_enabled is neither TRUE nor FALSE.
 * @retval Others                             Please refer to cvb_reg_field_write().
 */
RtkApiRet cvb_drv_psfp_gate_enabled_set(uint32 unit, uint32 index, uint32 gateEnabled)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(gateEnabled > 1u)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        registerIndex = (uint32)CVB_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);
        /* set psfp gate enable */
        CHK_FUN_CONTINUE(result, cvb_reg_field_write(unit,
                                                     registerIndex,
                                                     (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_PSFPGATEENABLED_LSP,
                                                     (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_PSFPGATEENABLED_LEN,
                                                     gateEnabled));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set validation of the specific stream gate entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                           Index (0 to MAX_STREAM_GATE_INSTANCE - 1)  \n
 *                                            Specify the index of stream gate instance
 * @param[in] valid                           Vaild (0 or 1)  \n
 *                                            Validation of the stream gate entry
 * @param[out] None
 * @retval RT_ERR_OK                          Successfully set the validation value of specified stream gate  \n
 *                                            instance entry.
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX   Parameter index is out of number of maximum stream filter  \n
 *                                            instance.
 * @retval -RT_ERR_OUT_OF_RANGE               Parameter valid is neither 0 nor 1.
 * @retval Others                             Please refer to cvb_reg_field_write().
 */
RtkApiRet cvb_drv_psfp_stm_gate_ent_valid_set(uint32 unit, uint32 index, uint32 valid)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((valid != 0u) && (valid != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }
        registerIndex = (uint32)CVB_STREAM_GATE_CONTROL_STATUS0 + (uint32)(NEXT_REG_OFFSET * index);
        /* set stram gate entry valid bit */
        CHK_FUN_CONTINUE(result, cvb_reg_field_write(unit,
                                                     registerIndex,
                                                     (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_GATEENTRYACTIVE_LSP,
                                                     (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_GATEENTRYACTIVE_LEN,
                                                     valid));
    } while(0u == 1u);
    return result;
}
/**
 * @brief To get the maximum number of supported stream filter instances
 * @param[in] None
 * @param[out] supportedInstance supported instance number
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_max_stm_fl_inst_get(uint32 *supportedInstance)
{
    RtkApiRet result = RT_ERR_OK;

    if(NULL == supportedInstance)
    {
        result = -RT_ERR_NULL_POINTER;
    }
    else
    {
        *supportedInstance = (uint32)CVB_PSFP_MAX_STREAM_FILTER_INSTANCE;
    }

    return result;
}

/**
 * @brief To get the maximum number of supported stream gate instances
 * @param[in] None
 * @param[out] supportedInstance supported instance number
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_max_stm_ga_inst_get(uint32 *supportedInstance)
{
    RtkApiRet result = RT_ERR_OK;

    if(NULL == supportedInstance)
    {
        result = -RT_ERR_NULL_POINTER;
    }
    else
    {
        *supportedInstance = (uint32)CVB_PSFP_MAX_STREAM_GATE_INSTANCE;
    }

    return result;
}

/**
 * @brief To get the maximum number of supported flow meter instances
 * @param[in] None
 * @param[out] supportedInstance supported instance number
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_max_fm_inst_get(uint32 *supportedInstance)
{
    RtkApiRet result = RT_ERR_OK;

    if(NULL == supportedInstance)
    {
        result = -RT_ERR_NULL_POINTER;
    }
    else
    {
        *supportedInstance = (uint32)CVB_PSFP_MAX_FLOW_METER_INSTANCE;
    }

    return result;
}

/**
 * @brief To get the maximum length of supported control list per stream gate
 * @param[in] None
 * @param[out] upported_list supported length of control list
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_sup_list_max_get(uint32 *supportedList)
{
    RtkApiRet result = RT_ERR_OK;

    if(NULL == supportedList)
    {
        result = -RT_ERR_NULL_POINTER;
    }
    else
    {
        *supportedList = (uint32)CVB_PSFP_SUPPORT_LIST_MAX;
    }

    return result;
}

/**
 * @brief To update the admin or operation instance for specified stream gate
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] None
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval RT_ERR_OK
 */
static RtkApiRet cvb_drv_psfp_update_admin_oper_index(uint32 unit, uint32 index)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    status;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        registerIndex = (uint32)CVB_STREAM_GATE_CONTROL_STATUS0 + (uint32)(4u * index);
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_INSTANCESTATE0_LSP,
                                                    (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_INSTANCESTATE0_LEN,
                                                    &status));

        if(1u == status)
        {
            cvb_psfpAdminIndex[index] = (uint32)1;
            cvb_psfpOperIndex[index]  = (uint32)0;
        }
        else
        {
            cvb_psfpAdminIndex[index] = (uint32)0;
            cvb_psfpOperIndex[index]  = (uint32)1;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the current control list length of stream gate
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] controlListLength the control list length of stream gate
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_ad_ctrl_lst_le_get(uint32 unit, uint32 index, uint32 *controlListLength)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == controlListLength)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_STREAM_GATE_CONTROL_STATUS0 + (uint32)(4u * index);
        if(1u == cvb_psfpAdminIndex[index])
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit, registerIndex, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH1_LSP, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH1_LEN, controlListLength));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit, registerIndex, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH0_LSP, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH0_LEN, controlListLength));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the admin control list length of specifed stream gate
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[in] controlListLength the control list length of stream gate
 * @param[out] None
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_OUT_OF_RANGE
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_ad_ctrl_lst_le_set(uint32 unit, uint32 index, uint32 controlListLength)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((CVB_PSFP_SUPPORT_LIST_MAX < controlListLength) || (CVB_PSFP_SUPPORT_LIST_MIN > controlListLength))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_STREAM_GATE_CONTROL_STATUS0 + (uint32)(4u * index);

        if(1u == cvb_psfpAdminIndex[index])
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_write(unit, registerIndex, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH1_LSP, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH1_LEN, controlListLength));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_write(unit, registerIndex, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH0_LSP, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH0_LEN, controlListLength));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the operation control list length of specifed stream gate
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] controlListLength the control list length of stream gate
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_op_ctrl_lst_le_get(uint32 unit, uint32 index, uint32 *controlListLength)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == controlListLength)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_STREAM_GATE_CONTROL_STATUS0 + (uint32)(4u * index);
        if(1u == cvb_psfpOperIndex[index])
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit, registerIndex, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH1_LSP, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH1_LEN, controlListLength));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit, registerIndex, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH0_LSP, (uint32)CVB_STREAM_GATE_CONTROL_STATUS0_CONTROLLISTLENGTH0_LEN, controlListLength));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the admin control list of specifed stream gate
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[in] numberOfEntry length of control list
 * @param[out] gateControlList the control list of stream gate
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_OUT_OF_RANGE
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_admin_ctrl_lst_get(uint32 unit, uint32 index, PsfpGateControlListParam_t gateControlList[CVB_PSFP_SUPPORT_LIST_MAX], uint32 numberOfEntry)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           iteratorList;
    uint32           baseAddress;
    uint32           tableAddress;
    uint32           tableData[3];
    cvb_table_list_t tableType = CVB_QCI_CONTROL_LIST;
    uint32           bypass;
    uint32           breakFlag;
    uint32           tmpData;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == gateControlList)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if((numberOfEntry < CVB_PSFP_SUPPORT_LIST_MIN) || (numberOfEntry > CVB_PSFP_SUPPORT_LIST_MAX))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        /* bit 10, 9:3, 2:0 */
        baseAddress = ((cvb_psfpAdminIndex[index] & (uint32)0x1u) << 10u) | ((index & (uint32)0x7fu) << 3u);
        breakFlag   = (uint32)0;
        for(iteratorList = 0u; (iteratorList < numberOfEntry); ++iteratorList)
        {
            /* bit 2:0 */
            tableAddress = baseAddress | (iteratorList);
            result       = cvb_table_read(unit, tableType, tableAddress, (uint32 *)tableData);
            if(result != RT_ERR_OK)
            {
                breakFlag = (uint32)1;
            }

            gateControlList[iteratorList].gateState         = (uint8)(tableData[0] & (uint32)0x1u);
            gateControlList[iteratorList].timeInterval      = tableData[1];
            gateControlList[iteratorList].intervalOctectMax = tableData[2];

            bypass = (uint32)((tableData[0] & (uint32)0x10u) >> 4u);
            if(1u == bypass)
            {
                gateControlList[iteratorList].internalPriority = (int32)-1;
            }
            else
            {
                tmpData                                        = (tableData[0] & (uint32)0xEu) >> 1;
                gateControlList[iteratorList].internalPriority = (int32)tmpData;
            }
        }
        if(1u == breakFlag)
        {
            result = -RT_ERR_FAILED;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the admin control list of specifed stream gate
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[in] numberOfEntry
 * @param[out] gateControlList the control list of stream gate
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_OUT_OF_RANGE
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_admin_ctrl_lst_set(uint32 unit, uint32 index, const PsfpGateControlListParam_t gateControlList[CVB_PSFP_SUPPORT_LIST_MAX], uint32 numberOfEntry)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           iteratorList;
    uint32           baseAddress;
    uint32           tableAddress;
    uint32           tableData[3] = {0};
    cvb_table_list_t tableType    = CVB_QCI_CONTROL_LIST;
    uint32           breakFlag;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -RT_ERR_PSFP_INVALID_ENTRY_INDEX;
            continue;
        }

        if(NULL == gateControlList)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if((numberOfEntry < CVB_PSFP_SUPPORT_LIST_MIN) || (numberOfEntry > CVB_PSFP_SUPPORT_LIST_MAX))
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        breakFlag = (uint32)0;
        for(iteratorList = (uint32)0; (iteratorList < numberOfEntry); ++iteratorList)
        {
            /* time interval of gate control list must be greater than 64 for user setting */
            if(gateControlList[iteratorList].timeInterval < 64u)
            {
                breakFlag = (uint32)1;
            }
        }

        if(1u == breakFlag)
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        /* bit 10, 9:3, 2:0 */
        baseAddress = ((cvb_psfpAdminIndex[index] & 0x1u) << 10u) | ((index & 0x7fu) << 3u);
        breakFlag   = (uint32)0;
        for(iteratorList = (uint32)0; (iteratorList < numberOfEntry); ++iteratorList)
        {
            /* bit 2:0 */
            tableAddress = baseAddress | (iteratorList);

            tableData[0] = gateControlList[iteratorList].gateState;
            tableData[1] = gateControlList[iteratorList].timeInterval;
            tableData[2] = gateControlList[iteratorList].intervalOctectMax;

            if(-1 == gateControlList[iteratorList].internalPriority)
            {
                /* bypass = 1 */
                tableData[0] |= (uint32)0x10u;
            }
            else
            {
                tableData[0] |= (((uint32)gateControlList[iteratorList].internalPriority & 0x7u) << 1u);
            }

            result = cvb_table_write(unit, tableType, tableAddress, (uint32 *)tableData);
            if(result != RT_ERR_OK)
            {
                breakFlag = (uint32)1;
            }
        }
        if(1u == breakFlag)
        {
            result = -RT_ERR_FAILED;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the operation control list of specifed stream gate
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[in] numberOfEntry length of control list
 * @param[out] gateControlList the control list of stream gate
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_OUT_OF_RANGE
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_oper_ctrl_lst_get(uint32 unit, uint32 index, PsfpGateControlListParam_t gateControlList[CVB_PSFP_SUPPORT_LIST_MAX], uint32 numberOfEntry)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           iteratorList;
    uint32           baseAddress;
    uint32           tableAddress;
    uint32           tableData[3];
    cvb_table_list_t tableType = CVB_QCI_CONTROL_LIST;
    uint32           bypass;
    uint32           breakFlag;
    uint32           tmpData;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == gateControlList)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if((numberOfEntry < CVB_PSFP_SUPPORT_LIST_MIN) || (numberOfEntry > CVB_PSFP_SUPPORT_LIST_MAX))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        /* bit 10, 9:3, 2:0 */
        baseAddress = ((cvb_psfpOperIndex[index] & (uint32)0x1u) << 10u) | ((index & (uint32)0x7fu) << 3u);
        breakFlag   = (uint32)0;
        for(iteratorList = (uint32)0; (iteratorList < numberOfEntry); ++iteratorList)
        {
            /* bit 2:0 */
            tableAddress = baseAddress | (iteratorList);
            result       = cvb_table_read(unit, tableType, tableAddress, (uint32 *)tableData);
            if(result != RT_ERR_OK)
            {
                breakFlag = (uint32)1;
            }

            gateControlList[iteratorList].gateState         = (uint8)(tableData[0] & (uint32)0x1u);
            gateControlList[iteratorList].timeInterval      = tableData[1];
            gateControlList[iteratorList].intervalOctectMax = tableData[2];

            bypass = (tableData[0] & (uint32)0x10u) >> 4u;
            if(1u == bypass)
            {
                gateControlList[iteratorList].internalPriority = (int32)-1;
            }
            else
            {
                tmpData                                        = (tableData[0] & (uint32)0xEu) >> 1;
                gateControlList[iteratorList].internalPriority = (int32)tmpData;
            }
        }
        if(1u == breakFlag)
        {
            result = -RT_ERR_FAILED;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the calculated admin control list cycle time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] cycleTime the calculated admin control list cycle time
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_admin_cycle_time_get(uint32 unit, uint32 index, RtkTsnCycleTime_t *cycleTime)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == cycleTime)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_INSTANCE_0_CYCLE_TIME_0 + (uint32)(4u * (index + ((cvb_psfpAdminIndex[index] * 2u) * CVB_PSFP_MAX_STREAM_GATE_INSTANCE)));
        /* 10^9, as unit nanosecond */
        cycleTime->denominator = (uint32)CVB_TSN_1B_NANOSECOND;
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LSP,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LEN,
                                                    &(cycleTime->numerator)));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the calculated admin control list cycle time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[in] cycleTime the calculated admin control list cycle time
 * @param[out] None
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_OUT_OF_RANGE
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_admin_cycle_time_set(uint32 unit, uint32 index, RtkTsnCycleTime_t cycleTime)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(cycleTime.denominator != (uint32)CVB_TSN_1B_NANOSECOND)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_INSTANCE_0_CYCLE_TIME_0 + (uint32)(4u * (index + ((cvb_psfpAdminIndex[index] * 2u) * CVB_PSFP_MAX_STREAM_GATE_INSTANCE)));
        CHK_FUN_CONTINUE(result, cvb_reg_field_write(unit, registerIndex,
                                                     (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LSP,
                                                     (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LEN,
                                                     cycleTime.numerator));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the calculated operation control list cycle time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] cycleTime the calculated admin control list cycle time
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_oper_cycle_time_get(uint32 unit, uint32 index, RtkTsnCycleTime_t *cycleTime)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == cycleTime)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_INSTANCE_0_CYCLE_TIME_0 + (uint32)(4u * (index + ((cvb_psfpOperIndex[index] * 2u) * CVB_PSFP_MAX_STREAM_GATE_INSTANCE)));
        /* 10^9, as unit nanosecond */
        cycleTime->denominator = (uint32)CVB_TSN_1B_NANOSECOND;
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LSP,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LEN,
                                                    &(cycleTime->numerator)));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the extendted admin control list cycle time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] cycleTimeExtension the extended admin control list cycle time
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_ad_cle_time_ext_get(uint32 unit, uint32 index, uint32 *cycleTimeExtension)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == cycleTimeExtension)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0 + (uint32)(4u * (index + ((cvb_psfpAdminIndex[index] * 2u) * CVB_PSFP_MAX_STREAM_GATE_INSTANCE)));
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0_CYCLETIME0_EXTENSION_LSP,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0_CYCLETIME0_EXTENSION_LEN,
                                                    cycleTimeExtension));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the extendted admin control list cycle time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[in] cycleTimeExtension the extended admin control list cycle time
 * @param[out] None
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_ad_cle_time_ext_set(uint32 unit, uint32 index, uint32 cycleTimeExtension)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        /* full 32 bit is valid for cycleTimeExtension */
        /* no check here. */

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0 + (uint32)(4u * (index + ((cvb_psfpAdminIndex[index] * 2u) * CVB_PSFP_MAX_STREAM_GATE_INSTANCE)));
        CHK_FUN_CONTINUE(result, cvb_reg_field_write(unit,
                                                     registerIndex,
                                                     (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0_CYCLETIME0_EXTENSION_LSP,
                                                     (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0_CYCLETIME0_EXTENSION_LEN,
                                                     cycleTimeExtension));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the extendted operation control list cycle time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] cycleTimeExtension the extended operation control list cycle time
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_op_cle_time_ext_get(uint32 unit, uint32 index, uint32 *cycleTimeExtension)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == cycleTimeExtension)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0 + (uint32)(4u * (index + ((cvb_psfpOperIndex[index] * 2u) * CVB_PSFP_MAX_STREAM_GATE_INSTANCE)));
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    registerIndex,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0_CYCLETIME0_EXTENSION_LSP,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_EXTENSION_0_CYCLETIME0_EXTENSION_LEN,
                                                    cycleTimeExtension));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the admin base time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] baseTime the admin base time
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_admin_base_time_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           tableAddress;
    uint32           tableData[6];
    cvb_table_list_t tableType = CVB_QCI_GATE_FILTER;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == baseTime)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        tableAddress = ((cvb_psfpAdminIndex[index] & (uint32)0x1u) << 7u) | (index & (uint32)0x7Fu);

        CHK_FUN_CONTINUE(result, cvb_table_read(unit, tableType, tableAddress, (uint32 *)tableData));

        baseTime->nanosecond = tableData[0];
        baseTime->lsbSecond  = tableData[1];
        baseTime->msbSecond  = (uint16)(tableData[2] & 0xFFFFu);

    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the admin base time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[in] baseTime the admin base time
 * @param[out] None
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_OUT_OF_RANGE
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_admin_base_time_set(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           tableAddress;
    uint32           tableData[6];
    cvb_table_list_t tableType = CVB_QCI_GATE_FILTER;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(baseTime->nanosecond > (uint32)CVB_TSN_1B_NANOSECOND)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        tableAddress = ((cvb_psfpAdminIndex[index] & 0x1u) << 7u) | (index & 0x7Fu);

        tableData[0] = baseTime->nanosecond;
        tableData[1] = baseTime->lsbSecond;
        tableData[2] = (uint16)(baseTime->msbSecond & 0xFFFFu);

        CHK_FUN_CONTINUE(result, cvb_table_write(unit, tableType, tableAddress, (uint32 *)tableData));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the operation base time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] baseTime the operation base time
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_oper_base_time_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime)
{
    RtkApiRet        result = RT_ERR_OK;
    uint32           tableAddress;
    uint32           tableData[3];
    cvb_table_list_t tableType = CVB_QCI_GATE_FILTER;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == baseTime)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        tableAddress = ((cvb_psfpOperIndex[index] & 0x1u) << 7u) | (index & 0x7Fu);

        CHK_FUN_CONTINUE(result, cvb_table_read(unit, tableType, tableAddress, (uint32 *)tableData));

        baseTime->nanosecond = tableData[0];
        baseTime->lsbSecond  = tableData[1];
        baseTime->msbSecond  = (uint16)(tableData[2] & 0xFFFFu);

    } while(1u == 0u);

    return result;
}

/**
 * @brief To get the config change status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index to specify the stream gate
 * @param[out] configChange the config change status
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_config_change_get(uint32 unit, uint32 index, uint32 *configChange)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == configChange)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL,
                                                    (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGE_LSP,
                                                    (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGE_LEN,
                                                    configChange));
    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the config change status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] indexto specify the stream gate
 * @param[in] configChange the config change status
 * @param[out] None
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_OUT_OF_RANGE
 * @retval -RT_ERR_PSFP_POLL_DONE_FAIL
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_config_change_set(uint32 unit, uint32 index, uint32 configChange)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    registerIndex;
    uint32    registerValue;
    uint32    doneBit;

    /* for compute multi-cycle */
    RtkTsnPtpTime_t            currentTime;
    RtkTsnPtpTime_t            baseTime;
    uint32                     cycleTime;
    cvb_RtkTsnLongNanosecond_t estimatedLongNs;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(configChange != 1u)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        CHK_FUN_CONTINUE(result, cvb_drv_psfp_update_admin_oper_index(unit, index));

        registerIndex = (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL;
        CHK_FUN_CONTINUE(result, cvb_reg_read(unit, registerIndex, &registerValue));

        doneBit = ((registerValue >> 1u) & 0x1u);
        if(doneBit != 1u)
        {
            result = -(RT_ERR_PSFP_POLL_DONE_FAIL);
            continue;
        }

        /* ok to continue, prepare configuration value */
        registerValue = (uint32)0;
        CHK_FUN_CONTINUE(result, cvb_reg_field_set(registerIndex,
                                                   (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGEENTRY_LSP,
                                                   (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGEENTRY_LEN,
                                                   index,
                                                   &registerValue));

        CHK_FUN_CONTINUE(result, cvb_reg_field_set(registerIndex,
                                                   (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGEINSTANCE_LSP,
                                                   (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGEINSTANCE_LEN,
                                                   cvb_psfpAdminIndex[index],
                                                   &registerValue));

        CHK_FUN_CONTINUE(result, cvb_reg_field_set(registerIndex,
                                                   (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGE_LSP,
                                                   (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_CONFIGCHANGE_LEN,
                                                   (uint32)1,
                                                   &registerValue));

        /* compute and update multi cycle time before set config change */
        /* get base time */
        CHK_FUN_CONTINUE(result, cvb_drv_psfp_admin_base_time_get(unit, index, &baseTime));

        /* get current time */
        CHK_FUN_CONTINUE(result, cvb_drv_tsn_current_time_get(unit, &currentTime));

        /* cycle time */
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_0 + (uint32)(4u * (index + ((cvb_psfpAdminIndex[index] * 2u) * CVB_PSFP_MAX_STREAM_GATE_INSTANCE))),
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LSP,
                                                    (uint32)CVB_INSTANCE_0_CYCLE_TIME_0_CYCLETIME0_LEN,
                                                    &cycleTime));

        cvb_drv_tsn_multicycle_est(baseTime, currentTime, cycleTime, &estimatedLongNs);

        /* set estimation time */
        CHK_FUN_CONTINUE(result, cvb_drv_psfp_estimation_time_set(unit, index, estimatedLongNs));

        /* set config change */
        CHK_FUN_CONTINUE(result, cvb_reg_write(unit, registerIndex, registerValue));

        /*polling computation done bit*/
        CHK_FUN_CONTINUE(result, cvb_drv_psfp_polling_done_bit(unit));

    } while(1u == 0u);

    return result;
}

/**
 * @brief To set the system tick granulairty value
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] tickGranularity system tick granulairty value
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet cvb_drv_psfp_tick_granularity_get(uint32 *tickGranularity)
{
    RtkApiRet result = RT_ERR_OK;

    if(NULL == tickGranularity)
    {
        result = -RT_ERR_NULL_POINTER;
    }
    else
    {
        *tickGranularity = (uint32)CVB_PSFP_TICK_GRANULARITY;
    }

    return result;
}

/* CIR, EIR unit is change from "1 bps" to "1000 bps" to support 5Gbps with 32bit */
/* CBS and EBS unit is byte */
/**
 * @brief
 *
 * @param[in] rate
 * @param[in] token
 */
void cvb_drv_psfp_conv_rate_to_token(uint32 rate, uint32 *token)
{
    uint64 temp;

    temp = (uint64)rate;

    /* the result is always within range of 32bit */
    temp   = ((temp * 1000u) << 22u) / CVB_PSFP_TICK_FREQUENCY;
    *token = (uint32)temp;
}

/**
 * @brief
 *
 * @param[in] token
 * @param[in] rate
 */
void cvb_drv_psfp_conv_token_to_rate(uint32 token, uint32 *rate)
{
    uint64 temp;

    temp = (uint64)token;

    /* the result is always within range of 32bit */
    temp  = ((temp * CVB_PSFP_TICK_FREQUENCY) >> 22u) / 1000u;
    *rate = (uint32)temp;

    if(((temp * 1000u) % CVB_PSFP_TICK_FREQUENCY) != 0u)
    {
        *rate += 1u;
    }
}

/**
 * @brief to poll the computation done bit
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_POLL_DONE_FAILk
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 */
static RtkApiRet cvb_drv_psfp_polling_done_bit(uint32 unit)
{
    uint32    breakFlag;
    uint32    doneBit;
    RtkApiRet result       = RT_ERR_OK;
    uint32    pollingCount = 0u;
    uint32    timeoutCount = cvb_g_psfpTimeoutCounter;
    uint32    registerIndex;

    breakFlag     = (int32)0;
    registerIndex = (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL;

    do
    {
        while(0u == breakFlag)
        {
            result = cvb_reg_field_read(unit,
                                        registerIndex,
                                        (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_COMPUTATIONDONE_LSP,
                                        (uint32)CVB_STREAM_GATE_CONFIG_CHANGE_CONTROL_COMPUTATIONDONE_LEN,
                                        &doneBit);
            if(result != RT_ERR_OK)
            {
                breakFlag = (uint32)1;
                continue;
            }

            if(1u == doneBit)
            {
                breakFlag = (uint32)1;
                continue;
            }

            CHK_FUN_RET(result, rtk_usecond_delay(1u));

            if(timeoutCount < pollingCount)
            {
                result    = -(RT_ERR_PSFP_POLL_DONE_FAIL);
                breakFlag = (uint32)1;
                continue;
            }
            ++pollingCount;
        }
    } while(1u == 0u);

    return result;
}

/**
 * @brief to set the estimated time
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] estimatedLongNs estimated time
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 */
static RtkApiRet cvb_drv_psfp_estimation_time_set(uint32 unit, uint32 index, cvb_RtkTsnLongNanosecond_t estimatedLongNs)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(1u == cvb_psfpAdminIndex[index])
        {
            CHK_FUN_CONTINUE(result, cvb_reg_write(unit, CVB_QCIADMINMULTICYCLENEARBY_0, estimatedLongNs.section[0]));
            CHK_FUN_CONTINUE(result, cvb_reg_write(unit, CVB_QCIADMINMULTICYCLENEARBY_1, estimatedLongNs.section[1]));
            CHK_FUN_CONTINUE(result, cvb_reg_write(unit, CVB_QCIADMINMULTICYCLENEARBY_2, (estimatedLongNs.section[2] & 0xFFFFu)));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_write(unit, CVB_QCIOPERMULTICYCLENEARBY_0, estimatedLongNs.section[0]));
            CHK_FUN_CONTINUE(result, cvb_reg_write(unit, CVB_QCIOPERMULTICYCLENEARBY_1, estimatedLongNs.section[1]));
            CHK_FUN_CONTINUE(result, cvb_reg_write(unit, CVB_QCIOPERMULTICYCLENEARBY_2, (estimatedLongNs.section[2] & 0xFFFFu)));
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief Error checking for the cvb_psfp_fm_entry_set() interface
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index the specify the stream gate instance
 * @param[in] flowMeterEntry Specify the parameters for the flow meter instance. link: CIR, CBS etc.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_OUT_OF_RANGE
 */
RtkApiRet cvb_drv_psfp_fm_set_intf_chk(uint32 unit, uint32 index, PsfpFlowMeterParam_t *flowMeterEntry)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(index >= CVB_PSFP_MAX_FLOW_METER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((flowMeterEntry->committedInfoRate > CVB_PSFP_CIR_MAX) || (flowMeterEntry->excessInfoRate > CVB_PSFP_EIR_MAX))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((flowMeterEntry->couplingFlag != 0u) && (flowMeterEntry->couplingFlag != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((flowMeterEntry->colorMode != 0u) && (flowMeterEntry->colorMode != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((flowMeterEntry->dropOnYellow != 0u) && (flowMeterEntry->dropOnYellow != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((flowMeterEntry->selectionDEI != 0u) && (flowMeterEntry->selectionDEI != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((flowMeterEntry->isEnableIPVForYellow != 0u) && (flowMeterEntry->isEnableIPVForYellow != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* 0~7 is ok */
        if(flowMeterEntry->yellowIPV > 7u)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief to check the flow meter entry is active
 * @param[in] registerIndex Specify the index of flow meter instance
 * @param[in] controlRegisterValue specify the control register of  flow meter instance
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_WRITE_ON_ACTIVE_ENTRY
 */
RtkApiRet cvb_drv_psfp_fm_active_entry_chk(uint32 registerIndex, uint32 controlRegisterValue)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    isActive;

    do
    {

        CHK_FUN_CONTINUE(result, cvb_reg_field_get(registerIndex,
                                                   (uint32)CVB_FLOW_METER_CONTROL_STATUS0_FLOWMETERENTRYACTIVE_LSP,
                                                   (uint32)CVB_FLOW_METER_CONTROL_STATUS0_FLOWMETERENTRYACTIVE_LEN,
                                                   &isActive,
                                                   controlRegisterValue));

        if(1u == isActive)
        {
            result = -(RT_ERR_PSFP_WRITE_ON_ACTIVE_ENTRY);
            continue;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief Error checking for the cvb_psfp_fm_entry_get() interface
 * @param[in] index the specify the stream gate instance
 * @param[in] flowMeterEntry Specify the parameters for the flow meter instance. link: CIR, CBS etc.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 */
RtkApiRet cvb_drv_psfp_fm_get_intf_chk(uint32 index, PsfpFlowMeterParam_t *flowMeterEntry)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(index >= CVB_PSFP_MAX_FLOW_METER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == flowMeterEntry)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief Error checking for the cvb_psfp_admin_gate_config_get()  and cvb_psfp_oper_gate_config_get interface
 * @param[in] index                 Specify the indx of stream identification entry
 * @param[in] baseTime              Base time of the specified stream gate instance
 * @param[in] cycleTime             Cycle time of specified stream gate instance
 * @param[in] cycleTimeExtension    Extension cycle time of specified stream gate instance
 * @param[in] gateControlList       The control list of specified stream gate instance
 * @param[in] controlListLength     Control list length
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_OUT_OF_RANGE
 */
RtkApiRet cvb_drv_psfp_conf_get_intf_chk(uint32                     index,
                                         RtkTsnPtpTime_t           *baseTime,
                                         uint32                    *cycleTime,
                                         uint32                    *cycleTimeExtension,
                                         PsfpGateControlListParam_t gateControlList[],
                                         uint32                    *controlListLength)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == baseTime)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == cycleTime)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == cycleTimeExtension)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == gateControlList)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == controlListLength)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

    } while(1u == 0u);

    return result;
}

RtkApiRet cvb_drv_psfp_conf_set_intf_chk(uint32                           index,
                                         RtkTsnPtpTime_t                 *baseTime,
                                         uint32                           cycleTimeExtension,
                                         const PsfpGateControlListParam_t gateControlList[],
                                         uint32                           controlListLength,
                                         uint32                           setConfigChange)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_GATE_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if(NULL == baseTime)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(baseTime->nanosecond >= (uint32)CVB_TSN_1B_NANOSECOND)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if(cycleTimeExtension > 0xFFFFu)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if(NULL == gateControlList)
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if((CVB_PSFP_SUPPORT_LIST_MAX < controlListLength) || (CVB_PSFP_SUPPORT_LIST_MIN > controlListLength))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        if((setConfigChange != 0u) && (setConfigChange != 1u))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

    } while(1u == 0u);

    return result;
}
/**
 * @brief Error checking for the cvb_psfp_stm_filter_entry_set() interface
 * @param[in] index The specify the stream gate instance
 * @param[in] streamHandleSpec         The stream handle value to be matched
 * @param[in] prioritySpec             Priority value of the specified stream filter
 * @param[in] streamGateInstanceId     Stream gate used by the specified stream filter
 * @param[in] streamMaxSDU             Maximum stream SDU value
 * @param[in] flowMeterInstanceId      Flow meter used by the specified stream filter
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_OUT_OF_RANGE
 */
RtkApiRet cvb_drv_psfp_sf_set_intf_chk(uint32 index, int32 streamHandleSpec, int32 prioritySpec, int32 streamGateInstanceId, int32 streamMaxSDU, int32 flowMeterInstanceId)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    temp;
    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        /* -1 wildcard, 0 ~ nr_instance-1 */
        if((streamHandleSpec > (int32)CVB_TSN_FRER_STREAM_HANDLE_MAX) || (streamHandleSpec < -1))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* -1 wildcard, 0 ~ 7 */
        if(prioritySpec >= (int32)0)
        {
            temp = (uint32)prioritySpec;
            if(temp >= CVB_TSN_NUMBER_OF_PRIOIRTY)
            {
                result = -(RT_ERR_OUT_OF_RANGE);
                continue;
            }
        }
        else if(prioritySpec < -1)
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }
        else
        {
            /* MISRA-C */
            ;
        }

        /* 0 ~ nr_instance-1 */
        if((streamGateInstanceId >= (int32)CVB_PSFP_MAX_STREAM_GATE_INSTANCE) || (streamGateInstanceId < 0))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* -1 bypass, 0 ~ 10*1024 */
        if((streamMaxSDU > (int32)CVB_STREAM_MAXSDU_UPPER) || (streamMaxSDU < -1))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

        /* -1 bypass, 0 ~ nr_instance-1 */
        if((flowMeterInstanceId >= (int32)CVB_PSFP_MAX_FLOW_METER_INSTANCE) || (flowMeterInstanceId < -1))
        {
            result = -(RT_ERR_OUT_OF_RANGE);
            continue;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief error checking for the cvb_psfp_stm_filter_entry_get() interface
 * @param[in] index the specify the stream gate instance
 * @param[in] streamHandleSpec         The stream handle value to be matched
 * @param[in] prioritySpec             Priority value of the specified stream filter
 * @param[in] streamGateInstanceId     Stream gate used by the specified stream filter
 * @param[in] streamMaxSDU             Maximum stream SDU value
 * @param[in] flowMeterInstanceId      Flow meter used by the specified stream filter
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_INVALID_ENTRY_INDEX
 * @retval -RT_ERR_NULL_POINTER
 */
RtkApiRet cvb_drv_psfp_sf_get_intf_chk(uint32 index, int32 *streamHandleSpec, int32 *prioritySpec, int32 *streamGateInstanceId, int32 *streamMaxSDU, int32 *flowMeterInstanceId)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if(index >= CVB_PSFP_MAX_STREAM_FILTER_INSTANCE)
        {
            result = -(RT_ERR_PSFP_INVALID_ENTRY_INDEX);
            continue;
        }

        if((NULL == streamHandleSpec) || (NULL == prioritySpec) || (NULL == streamGateInstanceId) || (NULL == streamMaxSDU) || (NULL == flowMeterInstanceId))
        {
            result = -(RT_ERR_NULL_POINTER);
            continue;
        }

    } while(1u == 0u);

    return result;
}

/**
 * @brief to check the stream filter entry is active
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] matchRegisterIndex specify the index of stream filter instance
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PSFP_WRITE_ON_ACTIVE_ENTRY
 */
RtkApiRet cvb_drv_psfp_sf_entry_active_chk(uint32 unit, uint32 matchRegisterIndex)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    temp;

    do
    {
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit,
                                                    matchRegisterIndex,
                                                    (uint32)CVB_STREAM_FILTER_CONTROLMATCH_0_FILTERENTRYACTIVE_LSP,
                                                    (uint32)CVB_STREAM_FILTER_CONTROLMATCH_0_FILTERENTRYACTIVE_LEN,
                                                    &temp));

        if(1u == temp)
        {
            result = -(RT_ERR_PSFP_WRITE_ON_ACTIVE_ENTRY);
            continue;
        }

    } while(1u == 0u);

    return result;
}

RtkApiRet cvb_drv_psfp_stm_fl_tbl_val_set(uint32 unit, uint32 policyRegisterIndex, int32 streamGateInstanceId, int32 streamMaxSDU, int32 flowMeterInstanceId, uint32 policyRegisterValue[])
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /* streamGateInstanceId */
        CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                   (uint32)CVB_QCI_STREAM_FILETER_TABLE_STREAM_GATE_ID_LSP,
                                                   (uint32)CVB_QCI_STREAM_FILETER_TABLE_STREAM_GATE_ID_LEN,
                                                   (uint32)streamGateInstanceId,
                                                   &policyRegisterValue[0]));
        /* streamMaxSDU */
        if(-1 == streamMaxSDU)
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LEN,
                                                       (uint32)TRUE,
                                                       &policyRegisterValue[0]));
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_MAX_SDU_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_MAX_SDU_LEN,
                                                       (uint32)0x3FFF,
                                                       &policyRegisterValue[0]));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LEN,
                                                       (uint32)FALSE,
                                                       &policyRegisterValue[0]));
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_MAX_SDU_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_MAX_SDU_LEN,
                                                       (uint32)streamMaxSDU,
                                                       &policyRegisterValue[0]));
        }

        /* flowMeterInstanceId */
        if(-1 == flowMeterInstanceId)
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_FLOW_METER_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_FLOW_METER_LEN,
                                                       (uint32)TRUE,
                                                       &policyRegisterValue[0]));
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LEN,
                                                       (uint32)policyRegisterIndex,
                                                       &policyRegisterValue[0]));
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_FLOW_METER_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_FLOW_METER_LEN,
                                                       (uint32)FALSE,
                                                       &policyRegisterValue[0]));
            CHK_FUN_CONTINUE(result, cvb_reg_field_set(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LEN,
                                                       (uint32)flowMeterInstanceId,
                                                       &policyRegisterValue[0]));
        }
    } while(1u == 0u);

    return result;
}

RtkApiRet cvb_drv_psfp_stm_fl_tbl_val_get(uint32 unit, uint32 policyRegisterIndex, int32 *streamGateInstanceId, int32 *streamMaxSDU, int32 *flowMeterInstanceId, uint32 policyRegisterValue[])
{
    RtkApiRet result = RT_ERR_OK;
    uint32    temp;

    do
    {
        /* streamGateInstanceId */
        CHK_FUN_CONTINUE(result, cvb_reg_field_get(policyRegisterIndex,
                                                   (uint32)CVB_QCI_STREAM_FILETER_TABLE_STREAM_GATE_ID_LSP,
                                                   (uint32)CVB_QCI_STREAM_FILETER_TABLE_STREAM_GATE_ID_LEN,
                                                   &temp,
                                                   policyRegisterValue[0]));
        *streamGateInstanceId = (int32)temp;

        /* get bypass MAX SDU */
        CHK_FUN_CONTINUE(result, cvb_reg_field_get(policyRegisterIndex,
                                                   (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LSP,
                                                   (uint32)CVB_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LEN,
                                                   &temp,
                                                   policyRegisterValue[0]));

        /* streamMaxSDU */
        if(1u == temp)
        {
            /* bypass */
            *streamMaxSDU = (int32)-1;
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_get(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_MAX_SDU_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_MAX_SDU_LEN,
                                                       &temp,
                                                       policyRegisterValue[0]));
            *streamMaxSDU = (int32)temp;
        }

        /* flowMeterInstanceId */
        temp = ((policyRegisterValue[0] >> 15u) & 0x1u);

        if(1u == temp)
        {
            /* bypass */
            *flowMeterInstanceId = (int32)-1;
        }
        else
        {
            CHK_FUN_CONTINUE(result, cvb_reg_field_get(policyRegisterIndex,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LSP,
                                                       (uint32)CVB_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LEN,
                                                       &temp,
                                                       policyRegisterValue[0]));
            *flowMeterInstanceId = (int32)temp;
        }
    } while(1u == 0u);

    return result;
}
/**@}*/  // PSFP