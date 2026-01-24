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

#ifndef CVB_RTK_API_PSFP_H__
#define CVB_RTK_API_PSFP_H__

#include "rtl907xc_vb_api_tsn.h"
#include "rtkas_api_st.h"
#include "rtkas_api_psfp.h"
/* rtl906x_asicdrv_psfp.h */
#define CVB_SHOW_PSFP_FIXME (0u)
/* FIXME: read on inactive; write on active protection */
#define CVB_DIVISOR_BIT_MASK_UNIT (5u)
#define CVB_BIT_MASK_UNIT         (32u)

#define CVB_MATCHING_FRAMES_COUNT    (0u)
#define CVB_PASSING_FRAMES_COUNT     (1u)
#define CVB_NOT_PASSING_FRAMES_COUNT (2u)
#define CVB_PASSING_SDU_COUNT        (3u)
#define CVB_NOT_PASSING_SDU_COUNT    (4u)
#define CVB_RED_FRAMES_COUNT         (5u)
#define CVB_STMFL_COUNT_END          (CVB_RED_FRAMES_COUNT)

#define CVB_CONFIG_QCI_DROP_ENABLE  (1u)
#define CVB_CONFIG_QCI_IPV_ENABLE   (1u)
#define CVB_CONFIG_QCI_DEI_ENABLE   (1u)
#define CVB_PSFP_TICK_GRANULARITY   (320u)
#define CVB_TABLE_PSFP_COUNTER_BASE (QCI_FRAME_MATCH_COUNTER)

#define CVB_PRIORITY_WILDCARD  (-1)
#define CVB_MAX_PRIORITY_VALUE (7)

#define CVB_PSFP_COUNTER_COMMAND_REQUEST (0x01u)
#define CVB_PSFP_COUNTER_COMMAND_RUNNING (0x02u)
#define CVB_PSFP_COUNTER_COMMAND_DONE    (0x04u)
#define CVB_PSFP_COUNTER_COMMAND_ERROR   (0x08u)

#define CVB_PSFP_POLLING_COUNT    (1000u)
#define CVB_PSFP_POLLING_NOT_YET  (0u)
#define CVB_PSFP_POLLING_NOT_DONE (1u)

#define CVB_PSFP_CIR_MAX (5000000u)
#define CVB_PSFP_EIR_MAX (5000000u)
/* this valid get function is shortcut to avoid dump whole entry. */
EXTERN_API RtkApiRet cvb_psfp_mode_get(uint32 unit, uint32 *mode);
EXTERN_API RtkApiRet cvb_psfp_mode_set(uint32 unit, uint32 mode);
EXTERN_API RtkApiRet cvb_psfp_stm_filter_entry_get(uint32 unit, uint32 index, PsfpStmFilterParam_t *psfpStmFilterEntry);
EXTERN_API RtkApiRet cvb_psfp_stm_filter_entry_set(uint32 unit, uint32 index, PsfpStmFilterParam_t *psfpStmFilterEntry);
EXTERN_API RtkApiRet cvb_psfp_frame_counters_get(uint32 unit, uint32 index, uint32 type, uint64 *counter);
EXTERN_API RtkApiRet cvb_psfp_frame_counters_set(uint32 unit, uint32 index, uint32 type, uint64 counter);
EXTERN_API RtkApiRet cvb_psfp_stm_block_oversize_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status);
EXTERN_API RtkApiRet cvb_psfp_stm_block_oversize_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus);
EXTERN_API RtkApiRet cvb_psfp_stm_fl_entry_valid_get(uint32 unit, uint32 index, uint32 *valid);
EXTERN_API RtkApiRet cvb_psfp_stm_fl_entry_valid_set(uint32 unit, uint32 index, uint32 valid);
EXTERN_API RtkApiRet cvb_psfp_admin_ipv_get(uint32 unit, uint32 index, int32 *IPV);
EXTERN_API RtkApiRet cvb_psfp_admin_ipv_set(uint32 unit, uint32 index, int32 IPV);
EXTERN_API RtkApiRet cvb_psfp_oper_ipv_get(uint32 unit, uint32 index, int32 *IPV);
EXTERN_API RtkApiRet cvb_psfp_admin_gate_state_get(uint32 unit, uint32 index, uint32 *gateState);
EXTERN_API RtkApiRet cvb_psfp_admin_gate_state_set(uint32 unit, uint32 index, uint32 gateState);
EXTERN_API RtkApiRet cvb_psfp_oper_gate_state_get(uint32 unit, uint32 index, uint32 *gateState);
EXTERN_API RtkApiRet cvb_psfp_admin_gate_config_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime, uint32 *cycleTime, uint32 *cycleTimeExtension,
                                                    PsfpGateControlListParam_t gateControlList[], uint32 *controlListLength);
EXTERN_API RtkApiRet cvb_psfp_admin_gate_config_set(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension,
                                                    const PsfpGateControlListParam_t gateControlList[], uint32 controlListLength, uint32 setConfigChange);
EXTERN_API RtkApiRet cvb_psfp_oper_gate_config_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *baseTime, uint32 *cycleTime,
                                                   uint32 *cycleTimeExtension, PsfpGateControlListParam_t gateControlList[], uint32 *controlListLength);
EXTERN_API RtkApiRet cvb_psfp_gate_enabled_get(uint32 unit, uint32 index, uint32 *gateEnabled);
EXTERN_API RtkApiRet cvb_psfp_gate_enabled_set(uint32 unit, uint32 index, uint32 gateEnabled);
EXTERN_API RtkApiRet cvb_psfp_gate_closed_inv_rx_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status);
EXTERN_API RtkApiRet cvb_psfp_gate_closed_inv_rx_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus);
EXTERN_API RtkApiRet cvb_psfp_gate_closed_oct_exd_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status);
EXTERN_API RtkApiRet cvb_psfp_gate_closed_oct_exd_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus);
EXTERN_API RtkApiRet cvb_psfp_stm_gate_ent_valid_get(uint32 unit, uint32 index, uint32 *valid);
EXTERN_API RtkApiRet cvb_psfp_stm_gate_ent_valid_set(uint32 unit, uint32 index, uint32 valid);
EXTERN_API RtkApiRet cvb_psfp_fm_entry_get(uint32 unit, uint32 index, PsfpFlowMeterParam_t *flowMeterEntry);
EXTERN_API RtkApiRet cvb_psfp_fm_entry_set(uint32 unit, uint32 index, PsfpFlowMeterParam_t *flowMeterEntry);
EXTERN_API RtkApiRet cvb_psfp_mark_all_frames_red_get(uint32 unit, uint32 index, uint32 *isEnable, uint32 *status);
EXTERN_API RtkApiRet cvb_psfp_mark_all_frames_red_set(uint32 unit, uint32 index, uint32 isEnable, uint32 clearStatus);
EXTERN_API RtkApiRet cvb_psfp_fm_entry_valid_get(uint32 unit, uint32 index, uint32 *valid);
EXTERN_API RtkApiRet cvb_psfp_fm_entry_valid_set(uint32 unit, uint32 index, uint32 valid);
EXTERN_API RtkApiRet cvb_psfp_config_change_time_get(uint32 unit, uint32 index, RtkTsnPtpTime_t *configChangeTime);
EXTERN_API RtkApiRet cvb_psfp_current_time_get(uint32 unit, RtkTsnPtpTime_t *currentTime);
EXTERN_API RtkApiRet cvb_psfp_config_pending_get(uint32 unit, uint32 index, uint32 *configPending);
EXTERN_API RtkApiRet cvb_psfp_conf_change_err_get(uint32 unit, uint32 index, uint64 *configChangeErrorCounter);

#endif /* CVB_RTK_API_PSFP_H__ */