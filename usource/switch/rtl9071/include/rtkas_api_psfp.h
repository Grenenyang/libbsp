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

#ifndef RTKAS_API_PSFP_H__
#define RTKAS_API_PSFP_H__
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common.h"
#include "rtkas_api_st.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 psfp_cycleTimeEx;
typedef uint32 psfp_cycleTime;
typedef uint64 psfp_counter;
/******************************************************************************/
/* Enumeration                                                                */
/******************************************************************************/

/******************************************************************************/
/* Struct                                                                     */
/******************************************************************************/
/* Structure of stream filter entry  */
typedef struct
{
    int32 streamHandleSpec;     /**< Stream handle specification (-1 to 95, where -1 means the stream handle specification is wildcard) */
    int32 prioritySpec;         /**< Priority specification (-1 to 7, where -1 means the prioritySpec is wildcard) */
    int32 streamGateInstanceId; /**< Stream gate instance identifier (0 to 95) */
    int32 streamMaxSDU;         /**< Stream maximum service data unit (-1 to 10240, where -1 means the stream maximum service data unit is bypassed) */
    int32 flowMeterInstanceId;  /**< Flow meter instance identifier(-1 to 95, where -1 means the flow meter instance identifier is bypassed) */
#if defined(RTL_907XD_VA)
    int32 atsSchedulerInstanceId; /**< Ats Scheduler instance identifier(-1 to 95, where -1 means the Ats scheduler instance is bypassed) */
#endif
} PsfpStmFilterParam_t;

/* Structure of PSFP gate control list entry  */
typedef struct
{
    uint8  gateState;         /**< the gate state of current operation (0 to 1) */
    int32  internalPriority;  /**< the value assign to internal priority when not bypass (-1 to 7, where -1 means the internal priority is bypassed) */
    uint32 timeInterval;      /**< the down count time interval of current gate operation (0 to 0x7FFFFFFF)*/
    uint32 intervalOctectMax; /**< number of bytes allower to pass during the current operation time interval (0 to 0xFFFFFFFF)*/
} PsfpGateControlListParam_t;
/* Structure of flow meter entry  */
typedef struct
{
    uint32 committedInfoRate;    /**< Committed Information Rate of specific flow meter in 1000 bps (0 to 10000000) */
    uint32 committedBurstSize;   /**< Committed Burst Size of specific flow meter (0 to 0xFFFFFFFF) */
    uint32 excessInfoRate;       /**< Excess Information Rate of  specific flow meter in 1000 bps (0 to 10000000) */
    uint32 excessBurstSize;      /**< Excess Burst Size of specific flow meter (0 to 0xFFFFFFFF) */
    uint32 couplingFlag;         /**< Coupling Flag of specific flow meter (0 to 1) */
    uint32 colorMode;            /**< Color Mode of specific flow meter, 0 stands for color-blind or 1 stands for color-aware (0 to 1) */
    uint32 dropOnYellow;         /**< To determine whether to drop the frame in yellow zone (0 to 1) */
    uint32 selectionDEI;         /**< The drop eligibility indicator(DEI) bit that the flow meter considers, configurable to either the inner VLAN tag or the outer VLAN tag (0 to 1) */
    uint32 isEnableIPVForYellow; /**< To enable or disable the modification of IPV for frames in yellow (0 to 1) */
    uint32 yellowIPV;            /**< The IPV for frames in yellow (0 to 7) */
} PsfpFlowMeterParam_t;

/**
 * @brief This is the PSFP mode structure.
 *
 * This structure contains information regarding PSFP mode, including
 * various settings and configurations required.
 */
typedef struct
{
    uint32 mode; /**< [in,out] Psfp mode: 0 means do not drop the trapped destination port mask, 1 means drop the trapped destination port mask (0 to 1) */

} PsfpMode_t;

/**
 * @brief This is the PSFP Stream filter structure.
 *
 * This structure contains information regarding the PSFP Stream filter, including
 * entry index and various stream filter parameters.
 */
typedef struct
{
    uint32               index;         /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE - 1u) */
    PsfpStmFilterParam_t stmFilteParam; /**< [in,out] Stream filter entry parameters. (N/A) */

} PsfpStmFilterEntry_t;

/**
 * @brief This structure represents the BlockStreamDueToOversizeFrameEnable status and control for a specified stream filter instance.
 *
 * This structure contains information regarding the BlockStreamDueToOversizeFrameEnable, including
 * the entry index and various BlockStreamDueToOversizeFrameEnable parameters.
 */
typedef struct
{
    uint32 index;    /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE - 1u) */
    uint32 isEnable; /**< [in,out] Enable state of BlockStreamDueToOversizeFrameEnable: 0 means disable, 1 means enable(0 to 1) */
    uint32 status;   /**< [in,out] Current status: 0 means the BlockStreamDueToOversizeFrame flag is not set, 1 means the BlockStreamDueToOversizeFrameEnable flag is set.
                                    When this parameter is input, it represents whether the current state needs to be cleared: 0 means do not clear. 1 means clear (0 to 1)*/

} PsfpStmBlockOversize_t;

/**
 * @brief This structure represents the validity status of a stream filter entry.
 *
 * This structure includes information about the entry index and its validity status.
 */
typedef struct
{
    uint32 index; /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE - 1u) */
    uint32 valid; /**< [in,out] Validation bit of stream filter entry: 0 means invalid, 1 means valid(0 or 1) */

} PsfpStmFilterEntryValid_t;

/**
 * @brief This structure represents the current count of a specified type for a specified index.
 *
 * This structure includes the index, the type of count, and the value of the associated PSFP counter.
 */
typedef struct
{
    uint32       index;   /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_FILTER_INSTANCE - 1u) */
    uint32       type;    /**< [in] Type (<CHIPID>_MATCHING_FRAMES_COUNT to <CHIPID>_COUNT_END) */
    psfp_counter counter; /**< [in,out] Counter (0 to <CHIPID>_PSFP_COUNTER_MAX) */

} PsfpFrameCounter_t;

/**
 * @brief This structure represents the enable status of a stream gate.
 *
 * This structure includes the specified stream gate index and the enable status of the stream gate.
 */
typedef struct
{
    uint32 index;       /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    uint32 gateEnabled; /**< [in,out] Gate enable: 0 means the gate is disabled, 1 means the gate is enabled (0 to 1) */
} PsfpGateEnabled_t;

/**
 * @brief This structure represents the current gate state of a specified stream gate.
 *
 * This structure includes the specified stream gate index and the current gate state.
 */
typedef struct
{
    uint32 index;     /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    uint32 gateState; /**< [in,out] Gate state: 0 means gate is closed, 1 means gate is open (0 to 1) */

} PsfpGateState_t;

/**
 * @brief This structure represents the configuration of a specified stream gate.
 *
 * The structure contains comprehensive information regarding the PSFP (Per-Stream Filtering and Policing)
 * stream gate, including its entry index and various configuration parameters.
 */
typedef struct
{
    uint32                      index;              /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    psfp_cycleTime              cycleTime;          /**< [out] Cycle time (0 to DVA_PSFP_CYCLETIME_MAX) */
    psfp_cycleTimeEx            cycleTimeExtension; /**< [in,out] Cycle time extension (0 to DVA_PSFP_CYCLETIMEEX_MAX) */
    uint32                      controlListLength;  /**< [in,out] Control list length (<CHIPID>_PSFP_SUPPORT_LIST_MIN to <CHIPID>_PSFP_SUPPORT_LIST_MAX) */
    RtkTsnPtpTime_t             baseTime;           /**< [in,out] Base time (N/A) */
    PsfpGateControlListParam_t *gateControlList;    /**< [in,out] Gate control list, the size of the gate control list is determined by "controlListLength" (N/A) */

} PsfpGateConfig_t;

/**
 * @brief This structure represents the CloseGateDueToInvalidRx status and control for a specified stream gate instance.
 *
 * This structure contains comprehensive information pertaining to the CloseGateDueToInvalidRx status
 * and its control mechanisms, including the entry index and various parameters related to CloseGateDueToInvalidRx.
 */
typedef struct
{
    uint32 index;    /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    uint32 isEnable; /**< [in,out] Enable state of CloseGateDueToInvalidRx: 0 means disable, 1 means enable(0 to 1) */
    uint32 status;   /**< [in,out] Current status: 0 means the CloseGateDueToInvalidRx flag is not set, 1 means the CloseGateDueToInvalidRx flag is set.
                                    When this parameter is input, it represents whether the current state needs to be cleared: 0 means do not clear. 1 means clear (0 to 1)*/

} PsfpGateClosedInvRx_t;

/**
 * @brief This structure represents the GateClosedDueToOctetsExceeded status and control for a specified stream gate instance.
 *
 * This structure contains comprehensive information pertaining to the GateClosedDueToOctetsExceeded status
 * and its control mechanisms, including the entry index and various parameters related to GateClosedDueToOctetsExceeded.
 */
typedef struct
{
    uint32 index;    /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    uint32 isEnable; /**< [in,out] Enable state of GateClosedDueToOctetsExceeded: 0 means disable, 1 means enable(0 to 1) */
    uint32 status;   /**< [in,out] Current status: 0 means the GateClosedDueToOctetsExceeded flag is not set, 1 means the GateClosedDueToOctetsExceeded flag is set.
                                    When this parameter is input, it represents whether the current state needs to be cleared: 0 means do not clear. 1 means clear (0 to 1)*/

} PsfpGateClosedOctExd_t;

/**
 * @brief This structure represents the validity status of a stream gate entry.
 *
 * This structure includes information about the entry index and its validity status.
 */
typedef struct
{
    uint32 index; /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    uint32 valid; /**< [in,out] Validation bit of stream gate entry: 0 means invalid, 1 means valid(0 or 1) */

} PsfpGateEntValid_t;

/**
 * @brief This structure represents the configuration change time of a gate.
 *
 * This structure contains information regarding the time at which the gate configuration was changed,
 * including the index of the gate and the specific timestamp of the configuration change.
 */
typedef struct
{
    uint32          index;            /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    RtkTsnPtpTime_t configChangeTime; /**< [out] Config change time (N/A) */
} PsfpConfigChangeTimeGet_t;

/**
 * @brief This structure can be used to obtain the current time.
 *
 * This structure contains information regarding the current time.
 */
typedef struct
{
    RtkTsnPtpTime_t currentTime; /**< [out] Current time (N/A) */
} PsfpCurrentTimeGet_t;

/**
 * @brief This structure can be used to check if a stream gate change configuration is in a pending state.
 *
 * This structure contains information about the stream gate's index
 * and a flag indicating whether the configuration change is currently pending.
 */
typedef struct
{
    uint32 index;         /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    uint32 configPending; /**< [out] Configuration pending flag: 0 means there is no configuration pending, 1 means there is a configuration pending(0 to 1) */

} PsfpConfigPendingGet_t;

/**
 * @brief This structure represents the internal priority specification (IPV) value.
 *
 * This structure includes the stream gate index and the internal priority specification value (IPV).
 */
typedef struct
{
    uint32 index; /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    int32  ipv;   /**< [in,out] Stream gate internal priority value (-1 to 7, where -1 means the ipv is bypassed) */

} PsfpGateIpv_t;

/**
 * @brief This structure can get the change error count of a stream gate.
 *
 * This structure includes the stream gate index and the error count value, representing the number of
 * configuration change errors associated with the specified stream gate.
 */
typedef struct
{
    uint32 index;                    /**< [in] Index: There are <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_STREAM_GATE_INSTANCE - 1u) */
    uint64 configChangeErrorCounter; /**< [out] Config Change Error Count (0 to 0xFFFFFFFFFFFFFFFFu) */

} PsfpConfChangeErrGet_t;

/**
 * @brief This structure represents the configuration of a specified flow meter.
 *
 * The structure contains comprehensive information regarding the PSFP (Per-Stream Filtering and Policing)
 * flow meter, including its entry index and various configuration parameters.
 */
typedef struct
{

    uint32               index;          /**< [in] Index: There are <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE - 1u) */
    PsfpFlowMeterParam_t flowMeterParam; /**< [in,out] Flow meter entry parameters. (N/A) */

} PsfpFmEntry_t;

/**
 * @brief This structure represents the MarkAllFramesRed status and control for a specified flow meter instance.
 *
 * This structure contains comprehensive information pertaining to the MarkAllFramesRed status
 * and its control mechanisms, including the entry index and various parameters related to MarkAllFramesRede.
 */
typedef struct
{
    uint32 index;    /**< [in] Index: There are <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE - 1u) */
    uint32 isEnable; /**< [in,out] Enable state of MarkAllFramesRed: 0 means disable, 1 means enable (0 to 1) */
    uint32 status;   /**< [in,out] Current status: 0 means the MarkAllFramesRed flag is not set, 1 means the MarkAllFramesRed flag is set.
                                   When this parameter is input, it represents whether the current state needs to be cleared: 0 means do not clear. 1 means clear (0 to 1)*/

} PsfpMarkAllFramesRed_t;

/**
 * @brief This structure represents the validity status of a flow meter entry.
 *
 * This structure includes information about the entry index and its validity status.
 */
typedef struct
{
    uint32 index; /**< [in] Index: There are <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE-1. (0 to <CHIPID>_PSFP_MAX_FLOW_METER_INSTANCE - 1u) */
    uint32 valid; /**< [in,out] Validation bit of flow meter entry: 0 means invalid, 1 means valid(0 or 1) */
} PsfpFmEntryValid_t;
/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_psfp_mode_get(UnitChip_t unitChip, PsfpMode_t *psfpMode);
EXTERN_API RtkApiRet rtk_psfp_mode_set(UnitChip_t unitChip, PsfpMode_t *psfpMode);

EXTERN_API RtkApiRet rtk_psfp_stm_filter_entry_set(UnitChip_t unitChip, PsfpStmFilterEntry_t *stmFilterEntry);
EXTERN_API RtkApiRet rtk_psfp_stm_filter_entry_get(UnitChip_t unitChip, PsfpStmFilterEntry_t *stmFilterEntry);
EXTERN_API RtkApiRet rtk_psfp_stm_block_oversize_get(UnitChip_t unitChip, PsfpStmBlockOversize_t *stmBlockOverSize);
EXTERN_API RtkApiRet rtk_psfp_stm_block_oversize_set(UnitChip_t unitChip, PsfpStmBlockOversize_t *stmBlockOverSize);
EXTERN_API RtkApiRet rtk_psfp_stm_fl_entry_valid_get(UnitChip_t unitChip, PsfpStmFilterEntryValid_t *stmFlEntryValid);
EXTERN_API RtkApiRet rtk_psfp_stm_fl_entry_valid_set(UnitChip_t unitChip, PsfpStmFilterEntryValid_t *stmFlEntryValid);
EXTERN_API RtkApiRet rtk_psfp_frame_counters_get(UnitChip_t unitChip, PsfpFrameCounter_t *frameCounters);
EXTERN_API RtkApiRet rtk_psfp_frame_counters_set(UnitChip_t unitChip, PsfpFrameCounter_t *frameCounters);

EXTERN_API RtkApiRet rtk_psfp_gate_enabled_get(UnitChip_t unitChip, PsfpGateEnabled_t *gateEnabled);
EXTERN_API RtkApiRet rtk_psfp_gate_enabled_set(UnitChip_t unitChip, PsfpGateEnabled_t *gateEnabled);
EXTERN_API RtkApiRet rtk_psfp_admin_gate_state_get(UnitChip_t unitChip, PsfpGateState_t *psfpAdminGateStateInfo);
EXTERN_API RtkApiRet rtk_psfp_admin_gate_state_set(UnitChip_t unitChip, PsfpGateState_t *psfpAdminGateStateInfo);
EXTERN_API RtkApiRet rtk_psfp_oper_gate_state_get(UnitChip_t unitChip, PsfpGateState_t *psfpOperGateStateInfo);
EXTERN_API RtkApiRet rtk_psfp_admin_gate_config_get(UnitChip_t unitChip, PsfpGateConfig_t *psfpAdminGateConfigInfo);
EXTERN_API RtkApiRet rtk_psfp_admin_gate_config_set(UnitChip_t unitChip, PsfpGateConfig_t *psfpAdminGateConfigInfo);
EXTERN_API RtkApiRet rtk_psfp_oper_gate_config_get(UnitChip_t unitChip, PsfpGateConfig_t *psfpOperGateConfigInfo);
EXTERN_API RtkApiRet rtk_psfp_gate_closed_inv_rx_get(UnitChip_t unitChip, PsfpGateClosedInvRx_t *psfpGateClosedInvRxInfo);
EXTERN_API RtkApiRet rtk_psfp_gate_closed_inv_rx_set(UnitChip_t unitChip, PsfpGateClosedInvRx_t *psfpGateClosedInvRxInfo);
EXTERN_API RtkApiRet rtk_psfp_gate_closed_oct_exd_get(UnitChip_t unitChip, PsfpGateClosedOctExd_t *psfpGateClosedOctExdInfo);
EXTERN_API RtkApiRet rtk_psfp_gate_closed_oct_exd_set(UnitChip_t unitChip, PsfpGateClosedOctExd_t *psfpGateClosedOctExdInfo);
EXTERN_API RtkApiRet rtk_psfp_stm_gate_ent_valid_get(UnitChip_t unitChip, PsfpGateEntValid_t *psfpStmGateEntValidInfo);
EXTERN_API RtkApiRet rtk_psfp_stm_gate_ent_valid_set(UnitChip_t unitChip, PsfpGateEntValid_t *psfpStmGateEntValidInfo);
EXTERN_API RtkApiRet rtk_psfp_config_change_time_get(UnitChip_t unitChip, PsfpConfigChangeTimeGet_t *psfpConfigChangeTimeInfo);
EXTERN_API RtkApiRet rtk_psfp_current_time_get(UnitChip_t unitChip, PsfpCurrentTimeGet_t *psfpCurrentTimeInfo);
EXTERN_API RtkApiRet rtk_psfp_config_pending_get(UnitChip_t unitChip, PsfpConfigPendingGet_t *psfpConfigPendingInfo);
EXTERN_API RtkApiRet rtk_psfp_admin_ipv_get(UnitChip_t unitChip, PsfpGateIpv_t *psfpAdminIpvInfo);
EXTERN_API RtkApiRet rtk_psfp_admin_ipv_set(UnitChip_t unitChip, PsfpGateIpv_t *psfpAdminIpvInfo);
EXTERN_API RtkApiRet rtk_psfp_oper_ipv_get(UnitChip_t unitChip, PsfpGateIpv_t *psfpOperIpvInfo);
EXTERN_API RtkApiRet rtk_psfp_conf_change_err_get(UnitChip_t unitChip, PsfpConfChangeErrGet_t *psfpConfChangeErrInfo);

EXTERN_API RtkApiRet rtk_psfp_fm_entry_get(UnitChip_t unitChip, PsfpFmEntry_t *psfpFmEntryInfo);
EXTERN_API RtkApiRet rtk_psfp_fm_entry_set(UnitChip_t unitChip, PsfpFmEntry_t *psfpFmEntryInfo);
EXTERN_API RtkApiRet rtk_psfp_mark_all_frames_red_get(UnitChip_t unitChip, PsfpMarkAllFramesRed_t *psfpMarkAllFramesRedInfo);
EXTERN_API RtkApiRet rtk_psfp_mark_all_frames_red_set(UnitChip_t unitChip, PsfpMarkAllFramesRed_t *psfpMarkAllFramesRedInfo);
EXTERN_API RtkApiRet rtk_psfp_fm_entry_valid_get(UnitChip_t unitChip, PsfpFmEntryValid_t *psfpFmEntryValidInfo);
EXTERN_API RtkApiRet rtk_psfp_fm_entry_valid_set(UnitChip_t unitChip, PsfpFmEntryValid_t *psfpFmEntryValidInfo);
#endif /* RTKAS_API_PSFP_H__ */
