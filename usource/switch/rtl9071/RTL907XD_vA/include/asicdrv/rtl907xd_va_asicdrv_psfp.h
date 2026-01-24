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

#ifndef DVA_ASICDRV_PSFP_H
#define DVA_ASICDRV_PSFP_H

#include "rtkas_common_types.h"

#include "rtl907xd_va_api_psfp.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_tsn.h"
#include "rtkas_api_st.h"
#include "rtkas_api_psfp.h"
/* For polling timeout issue */
extern uint32 dva_g_psfpTimeoutCounter;
#define DVA_PSFP_MAX_STREAM_FILTER_INSTANCE (96u)
#define DVA_PSFP_MAX_STREAM_GATE_INSTANCE   (96u)
#define DVA_PSFP_MAX_FLOW_METER_INSTANCE    (96u)
#define DVA_PSFP_SUPPORT_LIST_MAX           (8u)
#define DVA_PSFP_SUPPORT_LIST_MIN           (2u)
#define DVA_STREAM_MAXSDU_UPPER             (0x2800u)

#define DVA_TYPE_MATCHING_FRAMES_COUNT    (0u)
#define DVA_TYPE_PASSING_FRAMES_COUNT     (1u)
#define DVA_TYPE_NOT_PASSING_FRAMES_COUNT (2u)
#define DVA_TYPE_PASSING_SDU_COUNT        (3u)
#define DVA_TYPE_NOT_PASSING_SDU_COUNT    (4u)
#define DVA_TYPE_RED_FRAMES_COUNT         (5u)
#define DVA_STMFL_COUNT_END               (DVA_TYPE_RED_FRAMES_COUNT)

/* stream filter table field define */
#define DVA_QCI_STREAM_FILETER_TABLE_STREAM_GATE_ID_LSP       (0u)
#define DVA_QCI_STREAM_FILETER_TABLE_STREAM_GATE_ID_LEN       (8u)
#define DVA_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LSP        (8u)
#define DVA_QCI_STREAM_FILETER_TABLE_FLOW_METER_ID_LEN        (7u)
#define DVA_QCI_STREAM_FILETER_TABLE_BYPASS_FLOW_METER_LSP    (15u)
#define DVA_QCI_STREAM_FILETER_TABLE_BYPASS_FLOW_METER_LEN    (1u)
#define DVA_QCI_STREAM_FILETER_TABLE_MAX_SDU_LSP              (16u)
#define DVA_QCI_STREAM_FILETER_TABLE_MAX_SDU_LEN              (15u)
#define DVA_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LSP       (31u)
#define DVA_QCI_STREAM_FILETER_TABLE_BYPASS_MAX_SDU_LEN       (1u)
#define DVA_QCI_STREAM_FILETER_TABLE_ATS_SCHEDULER_LSP        (0u)
#define DVA_QCI_STREAM_FILETER_TABLE_ATS_SCHEDULER_LEN        (7u)
#define DVA_QCI_STREAM_FILETER_TABLE_BYPASS_ATS_SCHEDULER_LSP (7u)
#define DVA_QCI_STREAM_FILETER_TABLE_BYPASS_ATS_SCHEDULER_LEN (1u)

/* ieee8021PSFPFlowMeterTablee */
#define DVA_PSFP_TICK_FREQUENCY (DVA_TSN_SYSTEM_CLOCK / 4u)
/* for psfp sram address */
#define DVA_PSFP_CONFIG_DONE_ADDRESS (0x010FE814u)
/*****for polling compute Done*****/
#define DVA_PSFP_ACCESS_GATE_COMPUTEDONE_MAX_NS (1000000u)
#define DVA_PSFP_ACCESS_GATE_COMPUTEDONE_MIN_NS (24u)
#define DVA_QCI_SET_CONFIG_CHANGE               (1u)
#define DVA_QCI_COMPUTATION_DONE                (1u)
EXTERN_API RtkApiRet dva_drv_psfp_isdone_chk(uint32 unit);
EXTERN_API RtkApiRet dva_drv_psfp_preconfig_chk(uint32 unit);
EXTERN_API RtkApiRet dva_drv_psfp_gate_enabled_set(uint32 unit, uint32 index, uint32 gateEnabled);
EXTERN_API RtkApiRet dva_drv_psfp_stm_gate_ent_valid_set(uint32 unit, uint32 index, uint32 valid);

EXTERN_API RtkApiRet dva_drv_psfp_max_stm_fl_inst_get(uint32 *supportedInstance);
EXTERN_API RtkApiRet dva_drv_psfp_max_stm_ga_inst_get(uint32 *supportedInstance);
EXTERN_API RtkApiRet dva_drv_psfp_max_fm_inst_get(uint32 *supportedInstance);
EXTERN_API RtkApiRet dva_drv_psfp_sup_list_max_get(uint32 *supportedList);

EXTERN_API RtkApiRet dva_drv_psfp_ad_ctrl_lst_le_get(uint32 unit, uint32 index, uint32 *controlListLength);
EXTERN_API RtkApiRet dva_drv_psfp_ad_ctrl_lst_le_set(uint32 unit, uint32 index, uint32 controlListLength);
EXTERN_API RtkApiRet dva_drv_psfp_op_ctrl_lst_le_get(uint32 unit, uint32 index, uint32 *controlListLength);

EXTERN_API RtkApiRet dva_drv_psfp_admin_ctrl_lst_get(uint32 unit, uint32 index, PsfpGateControlListParam_t gateControlList[DVA_PSFP_SUPPORT_LIST_MAX], uint32 numberOfEntry);
EXTERN_API RtkApiRet dva_drv_psfp_admin_ctrl_lst_set(uint32 unit, uint32 index, const PsfpGateControlListParam_t gateControlList[DVA_PSFP_SUPPORT_LIST_MAX], uint32 numberOfEntry);
EXTERN_API RtkApiRet dva_drv_psfp_oper_ctrl_lst_get(uint32 unit, uint32 index, PsfpGateControlListParam_t gateControlList[DVA_PSFP_SUPPORT_LIST_MAX], uint32 numberOfEntry);

EXTERN_API RtkApiRet dva_drv_psfp_admin_cycle_time_get(uint32 unit, uint32 index, RtkTsnCycleTime_t *cycleTime);
EXTERN_API RtkApiRet dva_drv_psfp_admin_cycle_time_set(uint32 unit, uint32 index, RtkTsnCycleTime_t cycleTime);
EXTERN_API RtkApiRet dva_drv_psfp_oper_cycle_time_get(uint32 unit, uint32 index, RtkTsnCycleTime_t *cycleTime);

EXTERN_API RtkApiRet dva_drv_psfp_ad_cle_time_ext_get(uint32 unit, uint32 index, uint32 *cycleTimeExtension);
EXTERN_API RtkApiRet dva_drv_psfp_ad_cle_time_ext_set(uint32 unit, uint32 index, uint32 cycleTimeExtension);
EXTERN_API RtkApiRet dva_drv_psfp_op_cle_time_ext_get(uint32 unit, uint32 index, uint32 *cycleTimeExtension);

EXTERN_API RtkApiRet dva_drv_psfp_admin_base_time_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime);
EXTERN_API RtkApiRet dva_drv_psfp_admin_base_time_set(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime);
EXTERN_API RtkApiRet dva_drv_psfp_oper_base_time_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime);

EXTERN_API RtkApiRet dva_drv_psfp_config_change_get(uint32 unit, uint32 index, uint32 *configChange);
EXTERN_API RtkApiRet dva_drv_psfp_config_change_set(uint32 unit, uint32 index, uint32 configChange);

EXTERN_API RtkApiRet dva_drv_psfp_tick_granularity_get(uint32 *tickGranularity);

/* for rate/token coversion */
EXTERN_API void dva_drv_psfp_conv_rate_to_token(uint32 rate, uint32 *token);
EXTERN_API void dva_drv_psfp_conv_token_to_rate(uint32 token, uint32 *rate);

EXTERN_API RtkApiRet dva_drv_psfp_fm_set_intf_chk(uint32 unit, uint32 index, PsfpFlowMeterParam_t flowMeterEntry);
EXTERN_API RtkApiRet dva_drv_psfp_fm_active_entry_chk(uint32 registerIndex, uint32 controlRegisterValue);
EXTERN_API RtkApiRet dva_drv_psfp_fm_get_intf_chk(uint32 index, PsfpFlowMeterParam_t *flowMeterEntry);
EXTERN_API RtkApiRet dva_drv_psfp_conf_get_intf_chk(uint32 index, RtkTsnPtpTime_t *baseTime, uint32 *cycleTime, uint32 *cycleTimeExtension, PsfpGateControlListParam_t gateControlList[], uint32 *controlListLength);
EXTERN_API RtkApiRet dva_drv_psfp_conf_set_intf_chk(uint32 index, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension, const PsfpGateControlListParam_t gateControlList[], uint32 controlListLength, uint32 setConfigChange);
EXTERN_API RtkApiRet dva_drv_psfp_sf_set_intf_chk(uint32 index, int32 streamHandleSpec, int32 prioritySpec, int32 streamGateInstanceId, int32 streamMaxSDU, int32 flowMeterInstanceId, int32 atsSchedulerInstanceId);
EXTERN_API RtkApiRet dva_drv_psfp_sf_entry_active_chk(uint32 unit, uint32 matchRegisterIndex);
EXTERN_API RtkApiRet dva_drv_psfp_stm_fl_tbl_val_set(uint32 unit, uint32 policyRegisterIndex, int32 streamGateInstanceId, int32 streamMaxSDU, int32 flowMeterInstanceId, int32 atsSchedulerInstanceId, uint32 *policyRegisterValue);
EXTERN_API RtkApiRet dva_drv_psfp_stm_fl_tbl_val_get(uint32 unit, uint32 policyRegisterIndex, int32 *streamGateInstanceId, int32 *streamMaxSDU, int32 *flowMeterInstanceId, int32 *atsSchedulerInstanceId, uint32 *policyRegisterValue);
#endif
