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
#include "rtkas_api_psfp.h"
#include "rtl907xd_va_asicdrv_psfp.h"
#include "rtl907xd_va_api_qos.h"
#include "rtl907xd_va_api_vlan.h"
#include "rtl907xd_va_api_ptp.h"
#include "rtl907xd_va_api_cb.h"
#include "rtkas_api_cb.h"
/* define macro */
#define DVA_OPEN   (1u)
#define DVA_CLOSED (0u)

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @defgroup PSCM_EXAMPLE PSCM(QCI/PSFP & QCR/ATS) Example
 * @{
 */
/**
 * @defgroup RTL907DVA_PSFP_EXAMPLE RTL907XD_VA PSFP Example
 * @{
 */
/**
 * @defgroup RTL907XDVA_PSFP_CONFIGURATION_GET_EXAMPLE PSFP Configuration Get Example
 *
 * @brief <b> Get the PSFP configuration. </b>
 *
 * @section Purpose
 *          This example code demonstrates how to get the PSFP configuration.
 * @section Description
 *          Get the PSFP configuration as follows:
 *          1.PSFP mode.
 *          2.Stream filter configuration.
 *          3.Stream gate configuration.
 *          4.Flow meter configuration.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_psfp_configuration_get(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_psfp_configuration_get(uint32 unit)
{
    RtkApiRet  result   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};
    PsfpGateControlListParam_t gateControlList[DVA_PSFP_SUPPORT_LIST_MAX];
    PsfpMode_t                 psfpMode;
    PsfpStmFilterEntry_t       psfpStmFilterEntry;
    PsfpStmBlockOversize_t     psfpStmBlockOversize;
    PsfpGateIpv_t              psfpIpv;
    PsfpGateEnabled_t          psfpGateEnabled;
    PsfpGateState_t            psfpGateState;
    PsfpGateConfig_t           psfpGateConfig;
    PsfpGateClosedInvRx_t      psfpGateClosedInv;
    PsfpGateClosedOctExd_t     psfpGateClosedOctExd;
    PsfpGateEntValid_t         psfpStmGateEntValid;
    PsfpFmEntry_t              psfpFmEntry;
    PsfpMarkAllFramesRed_t     psfpMarkAllFramesRed;
    PsfpFmEntryValid_t         psfpFmEntryValid;
    PsfpConfigChangeTimeGet_t  psfpConfigChangeTimeGet;
    PsfpConfigPendingGet_t     psfpConfigPendingGet;
    PsfpCurrentTimeGet_t       psfpCurrentTimeGet;
    PsfpFrameCounter_t         psfpFrameCounter;
    PsfpConfChangeErrGet_t     psfpConfChangeErrGet;
    PsfpStmFilterEntryValid_t  psfpStmFilterEntryValid;
    do
    {
        /* Step 1: Get the PSFP drop mode enabled */
        psfpMode.mode = (uint32)ENABLED;
        CHK_FUN_CONTINUE(result, rtk_psfp_mode_set(unitChip, &psfpMode));
        (void)drv_memset(&psfpMode, 0, sizeof(PsfpMode_t));
        CHK_FUN_CONTINUE(result, rtk_psfp_mode_get(unitChip, &psfpMode));

        /****************************************************************************/
        /****  Step 2: Get stream filter entry 1:                                ****/
        /****                                   Stream Handle:  0x0              ****/
        /****                                        Priority:  0                ****/
        /****                                   Stream MAX SDU:  64              ****/
        /****                             Stream Gate Instance:  0               ****/
        /****                             Flow Meter Instance:  bypass           ****/
        /****                             ATS Scheduler Instance:  bypass        ****/
        /****  Expected Behavior: To direct specific stream to assigned stream   ****/
        /****                      and flow meter                                ****/
        /****************************************************************************/
        psfpStmFilterEntry.index                                = (uint32)1;
        psfpStmFilterEntry.stmFilteParam.streamHandleSpec       = (int32)0x0;
        psfpStmFilterEntry.stmFilteParam.prioritySpec           = (int32)0;
        psfpStmFilterEntry.stmFilteParam.streamGateInstanceId   = (int32)0;
        psfpStmFilterEntry.stmFilteParam.streamMaxSDU           = (int32)64;
        psfpStmFilterEntry.stmFilteParam.flowMeterInstanceId    = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.atsSchedulerInstanceId = (int32)-1;
        /* Get stream filter entry of stream filter for entry 1 */
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_filter_entry_set(unitChip, &psfpStmFilterEntry));

        (void)drv_memset(&psfpStmFilterEntry, 0, sizeof(PsfpStmFilterEntry_t));
        psfpStmFilterEntry.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_filter_entry_get(unitChip, &psfpStmFilterEntry));

        psfpStmBlockOversize.index    = (uint32)1;
        psfpStmBlockOversize.isEnable = ENABLED;
        psfpStmBlockOversize.status   = TSN_VALID;
        /* Step 3: Get BlockStreamDueToOversizeFrame enable and status of stream filter for entry 1 */
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_block_oversize_set(unitChip, &psfpStmBlockOversize));

        (void)drv_memset(&psfpStmBlockOversize, 0, sizeof(PsfpStmBlockOversize_t));
        psfpStmBlockOversize.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_block_oversize_get(unitChip, &psfpStmBlockOversize));

        /* Step 4: Get admin/oper IPV of stream gate for entry 1 */
        psfpIpv.index = (uint32)1;
        psfpIpv.ipv   = 0;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_ipv_set(unitChip, &psfpIpv));

        (void)drv_memset(&psfpIpv, 0, sizeof(PsfpGateIpv_t));
        psfpIpv.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_ipv_get(unitChip, &psfpIpv));

        (void)drv_memset(&psfpIpv, 0, sizeof(PsfpGateIpv_t));
        psfpIpv.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_oper_ipv_get(unitChip, &psfpIpv));

        psfpGateEnabled.index       = (uint32)1;
        psfpGateEnabled.gateEnabled = ENABLED;
        /* Step 5: Get status of stream gate is enabled or disabled for entry 1 */
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_enabled_set(unitChip, &psfpGateEnabled));

        (void)drv_memset(&psfpGateEnabled, 0, sizeof(PsfpGateEnabled_t));
        psfpGateEnabled.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_enabled_get(unitChip, &psfpGateEnabled));

        psfpGateState.index     = (uint32)1;
        psfpGateState.gateState = DVA_CLOSED;
        /* Step 6: Get admin gate state for entry 1 */
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_state_set(unitChip, &psfpGateState));

        (void)drv_memset(&psfpGateState, 0, sizeof(PsfpGateState_t));
        psfpGateState.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_state_get(unitChip, &psfpGateState));

        (void)drv_memset(&psfpGateState, 0, sizeof(PsfpGateState_t));
        psfpGateState.index = (uint32)1;
        /* Step 7: Get oper gate state for entry 1 */
        CHK_FUN_CONTINUE(result, rtk_psfp_oper_gate_state_get(unitChip, &psfpGateState));

        /*********************************************************************************************/
        /****  Step 8: Get admin stream gate entry 1:                                             ****/
        /****                                   Cycle time extension:  50000                      ****/
        /****                                              Base Time:  0                          ****/
        /****                               Gate Control List Length:  2                          ****/
        /****                                      Gate Control List:  Enabled                    ****/
        /****                     Gate State       IPV       Duration       MAX Interval Octet    ****/
        /****    Control List 1         open    bypass        5000000                       1000  ****/
        /****    Control List 2       closed    bypass       20000000                        0    ****/
        /****                            Block Stream if Gate Closed:  TRUE                       ****/
        /****              Block Stream if Exceed MAX Interval Octet:  TRUE                       ****/
        /****                                                                                     ****/
        /****  Expected Behavior: The obtained stream gate parameters are the same as those       ****/
        /****                      mentioned above                                                ****/
        /*********************************************************************************************/
        psfpGateConfig.baseTime.msbSecond   = (uint16)0;
        psfpGateConfig.baseTime.lsbSecond   = (uint32)0;
        psfpGateConfig.baseTime.nanosecond  = (uint32)0;
        gateControlList[0].gateState        = (uint8)DVA_OPEN;
        gateControlList[0].internalPriority = (int32)-1;
        /* nanosecond */
        gateControlList[0].timeInterval      = (uint32)5000000;
        gateControlList[0].intervalOctectMax = (uint32)1000;
        gateControlList[1].gateState         = (uint8)DVA_CLOSED;
        gateControlList[1].internalPriority  = (int32)-1;
        /* nanosecond */
        gateControlList[1].timeInterval      = (uint32)20000000;
        gateControlList[1].intervalOctectMax = (uint32)0;

        psfpGateConfig.index              = (uint32)1;
        psfpGateConfig.cycleTimeExtension = (uint32)50000;
        psfpGateConfig.controlListLength  = (uint32)2;
        psfpGateConfig.gateControlList    = gateControlList;
        /* Get the admin stream gate config for entry 1 */
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_config_set(unitChip, &psfpGateConfig));

        (void)drv_memset(&psfpGateConfig, 0, sizeof(PsfpGateConfig_t));
        psfpGateConfig.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_config_get(unitChip, &psfpGateConfig));

        /* Step 9: Get the oper stream gate config for entry 1 */
        (void)drv_memset(&psfpGateConfig, 0, sizeof(PsfpGateConfig_t));
        psfpGateConfig.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_oper_gate_config_get(unitChip, &psfpGateConfig));

        /* Step 10: Get the CloseGateDueToInvalidRx status and enable flag for entry 1 */
        psfpGateClosedInv.index    = (uint32)1;
        psfpGateClosedInv.isEnable = ENABLED;
        psfpGateClosedInv.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_inv_rx_set(unitChip, &psfpGateClosedInv));

        (void)drv_memset(&psfpGateClosedInv, 0, sizeof(PsfpGateClosedInvRx_t));
        psfpGateClosedInv.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_inv_rx_get(unitChip, &psfpGateClosedInv));

        /* Step 11: Get the GateClosedDueToOctetsExceeded status and enable flag for entry 1 */
        psfpGateClosedOctExd.index    = (uint32)1;
        psfpGateClosedOctExd.isEnable = ENABLED;
        psfpGateClosedOctExd.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_oct_exd_set(unitChip, &psfpGateClosedOctExd));

        (void)drv_memset(&psfpGateClosedOctExd, 0, sizeof(PsfpGateClosedOctExd_t));
        psfpGateClosedOctExd.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_oct_exd_get(unitChip, &psfpGateClosedOctExd));

        /* Step 12: Get validation for stream gate entry 1 */
        psfpStmGateEntValid.index = (uint32)1;
        psfpStmGateEntValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_gate_ent_valid_set(unitChip, &psfpStmGateEntValid));

        (void)drv_memset(&psfpStmGateEntValid, 0, sizeof(PsfpGateEntValid_t));
        psfpStmGateEntValid.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_gate_ent_valid_get(unitChip, &psfpStmGateEntValid));

        /**********************************************************************************/
        /****  Step 13: Get flow meter entry 1:                                        ****/
        /****                                  committedInfoRate:  10000               ****/
        /****                                  committedBurstSize:  20000              ****/
        /****                                  excessInfoRate:  0                      ****/
        /****                                  excessBurstSize:  0                     ****/
        /****                        Coupling Flag:  FALSE                             ****/
        /****                           Color Mode:  Color-Blind                       ****/
        /****                    Drop Yellow Frame:  FALSE                             ****/
        /****                        DEI Selection:  Inner Tag                         ****/
        /****                 IPV for Yellow Frame:  Bypass                            ****/
        /****            Mark Stream Red when drop:  Disable                           ****/
        /****                                                                          ****/
        /****  Expected Behavior: The obtained flow meter parameters are the           ****/
        /****                      same as those mentioned above                       ****/
        /**********************************************************************************/
        /* in kbps */
        psfpFmEntry.flowMeterParam.committedInfoRate = (uint32)10;
        /* in bytes */
        psfpFmEntry.flowMeterParam.committedBurstSize = (uint32)20000;
        /* in kbps */
        psfpFmEntry.flowMeterParam.excessInfoRate = (uint32)0;
        /* in bytes */
        psfpFmEntry.flowMeterParam.excessBurstSize      = (uint32)0;
        psfpFmEntry.flowMeterParam.couplingFlag         = (uint32)0;
        psfpFmEntry.flowMeterParam.colorMode            = (uint32)0;
        psfpFmEntry.flowMeterParam.dropOnYellow         = (uint32)TSN_INVALID;
        psfpFmEntry.flowMeterParam.selectionDEI         = (uint32)0;
        psfpFmEntry.flowMeterParam.isEnableIPVForYellow = (uint32)TSN_INVALID;
        psfpFmEntry.flowMeterParam.yellowIPV            = (uint32)0;

        psfpFmEntry.index = (uint32)1;
        /* Get flow meter configuration for entry 1 */
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_set(unitChip, &psfpFmEntry));

        (void)drv_memset(&psfpFmEntry, 0, sizeof(PsfpFmEntry_t));
        psfpFmEntry.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_get(unitChip, &psfpFmEntry));

        /* Step 14: Get MarkAllFramesRedEnable status for entry 1 */
        psfpMarkAllFramesRed.index    = (uint32)1;
        psfpMarkAllFramesRed.isEnable = ENABLED;
        psfpMarkAllFramesRed.status   = TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_mark_all_frames_red_set(unitChip, &psfpMarkAllFramesRed));

        (void)drv_memset(&psfpMarkAllFramesRed, 0, sizeof(PsfpMarkAllFramesRed_t));
        psfpMarkAllFramesRed.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_mark_all_frames_red_get(unitChip, &psfpMarkAllFramesRed));

        /* Step 15: Get MarkAllFramesRedEnable status for entry 1 */
        psfpFmEntryValid.index = (uint32)1;
        psfpFmEntryValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_valid_set(unitChip, &psfpFmEntryValid));

        (void)drv_memset(&psfpMarkAllFramesRed, 0, sizeof(PsfpMarkAllFramesRed_t));
        psfpFmEntryValid.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_valid_get(unitChip, &psfpFmEntryValid));

        /* Step 16: Get stream filter valid for entry 1 */
        psfpStmFilterEntryValid.index = (uint32)1;
        psfpStmFilterEntryValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_fl_entry_valid_set(unitChip, &psfpStmFilterEntryValid));

        (void)drv_memset(&psfpStmFilterEntryValid, 0, sizeof(PsfpStmFilterEntryValid_t));
        psfpStmFilterEntryValid.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_fl_entry_valid_get(unitChip, &psfpStmFilterEntryValid));

        /* Step 17: Get config change time for entry 1 */
        psfpConfigChangeTimeGet.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_config_change_time_get(unitChip, &psfpConfigChangeTimeGet));

        /* Step 18: Get config change time */
        CHK_FUN_CONTINUE(result, rtk_psfp_current_time_get(unitChip, &psfpCurrentTimeGet));

        /* Step 19: Get whether configuration changes for entry 1 is pending */
        psfpConfigPendingGet.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_config_pending_get(unitChip, &psfpConfigPendingGet));

        /* Step 20: Get the counters of table DVA_QCI_HIT_COUNTER(type = 0) for entry 1 */
        psfpFrameCounter.index   = (uint32)1;
        psfpFrameCounter.type    = 0u;
        psfpFrameCounter.counter = 30u;
        CHK_FUN_CONTINUE(result, rtk_psfp_frame_counters_set(unitChip, &psfpFrameCounter));

        (void)drv_memset(&psfpFrameCounter, 0, sizeof(PsfpFrameCounter_t));
        psfpFrameCounter.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_frame_counters_get(unitChip, &psfpFrameCounter));

        psfpConfChangeErrGet.index = (uint32)1;
        /* Step 21: Get config change error counter */
        CHK_FUN_CONTINUE(result, rtk_psfp_conf_change_err_get(unitChip, &psfpConfChangeErrGet));

    } while(0u == 1u);

    return result;
}
/**@}*/ /* RTL907XDVA_PSFP_CONFIGURATION_GET_EXAMPLE */

static RtkApiRet dva_psfp_general_init(UnitChip_t unitChip)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i;
    /* Qos config */
    QosPriSelInfo_t qosPriSel = {
        .group     = 0,
        .qosPriSel = {
            .portPri   = 0,
            .hybridPri = 7,
            .dscpPri   = 0,
            .cvlanPri  = 0,
            .svlanPri  = 0,
        },
    };
    QosCvlanPriRemap_t qosCvlanPriRemap;
    /* PTP config */
    PtpMasterEn_t ptpMasterEnInfo = {
        .masterEn = ENABLED,
    };
    PtpSlave0En_t ptpSlave0EnInfo = {
        .slave0En = ENABLED,
    };
    PtpSlave1En_t ptpSlave1EnInfo = {
        .slave1En = ENABLED,
    };
    PtpSlave2En_t ptpSlave2EnInfo = {
        .slave2En = ENABLED,
    };
    PtpHwEnable_t ptpHwEnableInfo = {
        .ptpEnable = ENABLED,
    };
    /* VLAN config */
    VlanInfo_t vlanInfo;
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(VlanInfo_t)));
    SvlanFwdBaseOvidInfo_t svlanFwdBaseOvidSetInfo = {0};
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSel));
    qosCvlanPriRemap.idx        = (uint32)0;
    qosCvlanPriRemap.iPri       = (uint32)0;
    qosCvlanPriRemap.iDei       = (uint32)0;
    qosCvlanPriRemap.mappingPri = (uint32)0;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.iPri       = (uint32)1;
    qosCvlanPriRemap.iDei       = (uint32)0;
    qosCvlanPriRemap.mappingPri = (uint32)1;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.iPri       = (uint32)0;
    qosCvlanPriRemap.iDei       = (uint32)1;
    qosCvlanPriRemap.mappingPri = (uint32)0;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.iPri       = (uint32)1;
    qosCvlanPriRemap.iDei       = (uint32)1;
    qosCvlanPriRemap.mappingPri = (uint32)1;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.idx        = (uint32)1;
    qosCvlanPriRemap.iPri       = (uint32)0;
    qosCvlanPriRemap.iDei       = (uint32)0;
    qosCvlanPriRemap.mappingPri = (uint32)0;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.idx        = (uint32)1;
    qosCvlanPriRemap.iPri       = (uint32)1;
    qosCvlanPriRemap.iDei       = (uint32)0;
    qosCvlanPriRemap.mappingPri = (uint32)1;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.idx        = (uint32)1;
    qosCvlanPriRemap.iPri       = (uint32)0;
    qosCvlanPriRemap.iDei       = (uint32)1;
    qosCvlanPriRemap.mappingPri = (uint32)0;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.idx        = (uint32)1;
    qosCvlanPriRemap.iPri       = (uint32)1;
    qosCvlanPriRemap.iDei       = (uint32)1;
    qosCvlanPriRemap.mappingPri = (uint32)1;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));

    vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    vlanInfo.vid                      = (uint32)10u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    svlanFwdBaseOvidSetInfo.enable = (uint32)0;
    for(i = DVA_RTK_USER_PORT_1ST; i <= DVA_RTK_USER_PORT_END; i++)
    {
        svlanFwdBaseOvidSetInfo.port = i;
        CHK_FUN_RET(retVal, rtk_svlan_fwd_base_ovid_set(unitChip, &svlanFwdBaseOvidSetInfo));
    }
    /* Env
     *   Start PTP
     */
    CHK_FUN_RET(retVal, rtk_ptp_set_master_en(unitChip, &ptpMasterEnInfo));
    CHK_FUN_RET(retVal, rtk_ptp_set_slave0_en(unitChip, &ptpSlave0EnInfo));
    CHK_FUN_RET(retVal, rtk_ptp_set_slave1_en(unitChip, &ptpSlave1EnInfo));
    CHK_FUN_RET(retVal, rtk_ptp_set_slave2_en(unitChip, &ptpSlave2EnInfo));
    CHK_FUN_RET(retVal, rtk_ptp_set_hw_enable(unitChip, &ptpHwEnableInfo));
    return retVal;
}
/**
 * @defgroup RTL907XDVA_PSFP_CONFIGURATION_EXAMPLE PSFP Configuration Example
 *
 * @brief <b> PSFP configuration flow. </b>
 *
 * @section Purpose
 *          This example demonstrates how to set PSFP configuration flow.
 *
 * @section Description
 *          In this example, you need to configure PTP, QOS, VLAN first. \n
 *          After all configurations are completed, te behavior is as follows, \n
 *          Stream handle 0: The host sends packets with source MAC 00-e0-4c-00-00-02, destination MAC 00-e0-4c-00-00-02, \n
 *                                         and VLAN priority 0 to port 1. If the packet's payload size is greater than 64, it will be \n
 *                                         discarded and the stream filter keeps blocking the stream until BlockStreamDueToOversizeFrameEnable \n
 *                                         is disabled or the status of BlockStreamDueToOversizeFrame has been cleared manually. This stream is \n
 *                                         controlled by the control list and bypasses the flow meter. \n
 *          Stream handle 1: The host sends packets with VLAN priority 0, and specific source IP, destination IP, next protocol, \n
 *                                         source port, and destination port to port 2. If the packet's payload size is greater than 1514, \n
 *                                         it will be discarded. This stream is controlled by the control list and a flow meter whose CIR is \n
 *                                         25kbps and EIR is 25kbps. \n
 *          Stream handle 2: The host sends packets with VLAN ID 10 to port 1. If the packet's payload size is greater than 1024, \n
 *                                         it will be discarded. This stream gate is always open and bypasses the flow meter. \n
 *          Stream handle 3: The host sends packets with destination MAC 00-e0-4c-00-00-02 to ports 1, 2, and 3. If the packet's payload \n
 *                                         size is greater than 1514, it will be discarded. This stream gate is always open and controlled \n
 *                                         by a flow meter whose CIR is 5kbps and EIR is 0kbps. \n
 *          The rest of the streams will be directed to a closed gate. \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_psfp_configuration_start_to_end(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_psfp_configuration_start_to_end(uint32 unit)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    index;
    /* Parameters for stream identification */
    uint8 da[MAC_ADDR_LEN]   = {0x00, 0xe0, 0x4c, 0x00, 0x00, 0x02};
    uint8 sip[IPV6_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0xFF, 0xFF, 0xC0, 0xa8, 0x01, 0x64};
    uint8 dip[IPV6_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0xFF, 0xFF, 0xC0, 0xa8, 0x01, 0x65};

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};
    CbSidEnabledInfo_t cbSidEnabledInfo;
    PsfpMode_t         psfpMode;

    /* Stream identification */
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .sidentryParam = {
            .carebits     = (uint32)DVA_TSN_SELECTED_FILED_SRCADR | (uint32)DVA_TSN_SELECTED_FILED_DSTADR,
            .srcMac       = {0x00, 0xe0, 0x4c, 0x00, 0x00, 0x01},
            .dstMac       = {0x00, 0xe0, 0x4c, 0x00, 0x00, 0x02},
            .portMask     = (uint16)DVA_RTK_USER_PMAP_2ND,
            .isValid      = (uint8)TSN_VALID,
            .streamHandle = (uint8)0x0,
        },
    };
    /* Stream filter */
    PsfpStmFilterEntry_t      psfpStmFilterEntry;
    PsfpStmBlockOversize_t    psfpStmBlockOversize;
    PsfpStmFilterEntryValid_t stmFlEntryValid;
    /* Stream gate */
    PsfpGateControlListParam_t gateControlList[DVA_PSFP_SUPPORT_LIST_MAX];
    PsfpGateIpv_t              psfpGateIpv;
    PsfpGateEnabled_t          psfpGateEnabled;
    PsfpGateState_t            psfpGateState;
    PsfpGateConfig_t           psfpGateConfig;
    PsfpGateClosedInvRx_t      psfpGateClosedInvRx;
    PsfpGateClosedOctExd_t     psfpGateClosedOctExd;
    PsfpGateEntValid_t         psfpGateEntValid;
    /* Flow meter */
    PsfpFmEntry_t          psfpFmEntry;
    PsfpMarkAllFramesRed_t psfpMarkAllFramesRed;
    PsfpFmEntryValid_t     psfpFmEntryValid;
    do
    {
        /************************************************************/
        /****  Pre-configuration for the environment            *****/
        /************************************************************/
        /* Configure QoS/VLAN/PTP settings  */

        CHK_FUN_CONTINUE(result, dva_psfp_general_init(unitChip));

        /************************************************************/
        /****  Step 1. Basic initialization for CB and PSFP     *****/
        /************************************************************/

        /* Enable CB stream identification */
        cbSidEnabledInfo.status = ENABLED;
        CHK_FUN_CONTINUE(result, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));

        /* Enable the PSFP drop mode */
        psfpMode.mode = ENABLED;
        CHK_FUN_CONTINUE(result, rtk_psfp_mode_set(unitChip, &psfpMode));

        /****************************************************************************************************/
        /****  Step 2. Set stream identification: set the stream handle for specific stream properties  *****/
        /****************************************************************************************************/

        /* Set the entry of stream identification with the rules and the corresponding stream handle. */
        /* Set the identification rules of index 1 to 3 respectively for examples */

        /****************************************************************************/
        /****  Step 2.1. Set stream identification entry 1 for port 2:           ****/
        /****                           Source Address:  00:E0:4C:00:00:01       ****/
        /****                      Destination Address:  00:E0:4C:00:00:02       ****/
        /****                                                                    ****/
        /****  Expected Behavior: To return stream handle for specific stream    ****/
        /****************************************************************************/

        /* Set all the configurations to stream identification entry 1 */
        cbIngressPassiveSidInfo.index = (uint8)1;
        CHK_FUN_CONTINUE(result, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));

        /****************************************************************************/
        /****  Step 2.2. Set stream identification entry 2 for port 1:           ****/
        /****                              Source IP:  192.168.1.100             ****/
        /****                         Destination IP:  192.168.1.101             ****/
        /****                          Next Protocol:  0x6 (TCP)                 ****/
        /****                            Source Port:  50098                     ****/
        /****                       Destination Port:  48876                     ****/
        /****                                                                    ****/
        /****  Expected Behavior: To return stream handle for specific stream    ****/
        /****************************************************************************/

        cbIngressPassiveSidInfo.sidentryParam.carebits = (uint32)DVA_TSN_SELECTED_FILED_SRCIP | (uint32)DVA_TSN_SELECTED_FILED_DSTIP
                                                         | (uint32)DVA_TSN_SELECTED_FILED_NEXTPROC | (uint32)DVA_TSN_SELECTED_FIELD_SRCPORT | (uint32)DVA_TSN_SELECTED_FIELD_DSTPORT;
        (void)drv_memcpy(cbIngressPassiveSidInfo.sidentryParam.srcIp, sip, IPV6_ADDR_LEN);
        (void)drv_memcpy(cbIngressPassiveSidInfo.sidentryParam.dstIp, dip, IPV6_ADDR_LEN);
        cbIngressPassiveSidInfo.sidentryParam.srcPort      = (uint16)50098;
        cbIngressPassiveSidInfo.sidentryParam.dstPort      = (uint16)48876;
        cbIngressPassiveSidInfo.sidentryParam.nextProtocol = (uint8)0x6;
        cbIngressPassiveSidInfo.sidentryParam.portMask     = (uint16)DVA_RTK_USER_PMAP_1ST;
        cbIngressPassiveSidInfo.sidentryParam.isValid      = (uint8)TSN_VALID;
        cbIngressPassiveSidInfo.sidentryParam.streamHandle = (uint8)0x1;

        cbIngressPassiveSidInfo.index = (uint8)2;
        /* Set all the configurations to stream identification entry 2 */
        CHK_FUN_CONTINUE(result, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));

        /****************************************************************************/
        /****  Step 2.3. Set stream identification entry 3 for port 1:           ****/
        /****                                   VID:  10                         ****/
        /****                                                                    ****/
        /****  Expected Behavior: To return stream handle for specific stream    ****/
        /****************************************************************************/
        cbIngressPassiveSidInfo.sidentryParam.carebits     = (uint32)DVA_TSN_SELECTED_FILED_VID;
        cbIngressPassiveSidInfo.sidentryParam.vlanId       = (uint16)10;
        cbIngressPassiveSidInfo.sidentryParam.portMask     = (uint16)DVA_RTK_USER_PMAP_1ST;
        cbIngressPassiveSidInfo.sidentryParam.isValid      = (uint8)TSN_VALID;
        cbIngressPassiveSidInfo.sidentryParam.streamHandle = (uint8)0x2;

        cbIngressPassiveSidInfo.index = (uint8)3;
        /* Set all the configurations to stream identification entry 3 */
        CHK_FUN_CONTINUE(result, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));

        /****************************************************************************/
        /****  Step 2.4. Set stream identification entry 4 for ports 1 to 3:     ****/
        /****                             DA: 00:E0:4C:00:00:02                  ****/
        /****                                                                    ****/
        /****  Expected Behavior: To return stream handle for specific stream    ****/
        /****************************************************************************/
        cbIngressPassiveSidInfo.sidentryParam.carebits = (uint32)DVA_TSN_SELECTED_FILED_DSTADR;
        (void)drv_memcpy(cbIngressPassiveSidInfo.sidentryParam.dstMac, da, MAC_ADDR_LEN);
        cbIngressPassiveSidInfo.sidentryParam.portMask     = (uint16)DVA_RTK_USER_PMAP_1ST | (uint16)DVA_RTK_USER_PMAP_2ND | (uint16)DVA_RTK_USER_PMAP_3RD;
        cbIngressPassiveSidInfo.sidentryParam.isValid      = (uint8)TSN_VALID;
        cbIngressPassiveSidInfo.sidentryParam.streamHandle = (uint8)0x3;

        cbIngressPassiveSidInfo.index = (uint8)4;
        /* Set all the configurations to stream identification entry 4 */
        CHK_FUN_CONTINUE(result, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));

        /**************************************************************************************************************/
        /****  Step 3. Set stream filters: direct the streams to specific stream gate and flow meter instance     *****/
        /**************************************************************************************************************/
        /* Set the stream filters entry 0 to 3, each with a certain value of stream handle, priority, and the corresponding index of stream gate and flow meter, and the maximum SDU size. */
        /* Set the entry parameters of stream filters */

        /***************************************************************************************************/
        /****  Step 3.1. Set stream filter entry 0:                                                     ****/
        /****                                   Stream Handle:  0x0                                     ****/
        /****                                        Priority:  0                                       ****/
        /****                                   Stream MAX SDU:  64                                     ****/
        /****                             Stream Gate Instance:  0                                      ****/
        /****                             Flow Meter Instance:  bypass                                  ****/
        /****                             ATS Scheduler Instance:  bypass                               ****/
        /****                             BlockStreamDueToOversizeFrame: enable                       ****/
        /****                             Clear the current BlockStreamDueToOversizeFrame status: Yes ****/
        /****  Expected Behavior: To direct specific stream to assigned stream                          ****/
        /***************************************************************************************************/
        psfpStmFilterEntry.stmFilteParam.streamHandleSpec       = (int32)0x0;
        psfpStmFilterEntry.stmFilteParam.prioritySpec           = (int32)0;
        psfpStmFilterEntry.stmFilteParam.streamGateInstanceId   = (int32)0;
        psfpStmFilterEntry.stmFilteParam.streamMaxSDU           = (int32)64;
        psfpStmFilterEntry.stmFilteParam.flowMeterInstanceId    = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.atsSchedulerInstanceId = (int32)-1;

        psfpStmFilterEntry.index = 0;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_filter_entry_set(unitChip, &psfpStmFilterEntry));

        psfpStmBlockOversize.index    = (uint32)0;
        psfpStmBlockOversize.isEnable = (uint32)ENABLED;
        psfpStmBlockOversize.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_block_oversize_set(unitChip, &psfpStmBlockOversize));

        /***************************************************************************************************/
        /****  Step 3.2. Set stream filter entry 1:                                                     ****/
        /****                                   Stream Handle:  0x1                                     ****/
        /****                                        Priority:  0                                       ****/
        /****                                   Stream MAX SDU:  1514                                   ****/
        /****                             Stream Gate Instance:  1                                      ****/
        /****                             Flow Meter Instance:  1                                       ****/
        /****                             ATS Scheduler Instance:  bypass                               ****/
        /****                             BlockStreamDueToOversizeFrame: disable                      ****/
        /****                             Clear the current BlockStreamDueToOversizeFrame status: No  ****/
        /****  Expected Behavior: To direct specific stream to assigned stream                          ****/
        /****                      and flow meter                                                       ****/
        /***************************************************************************************************/

        psfpStmFilterEntry.stmFilteParam.streamHandleSpec       = (int32)0x1;
        psfpStmFilterEntry.stmFilteParam.prioritySpec           = (int32)0;
        psfpStmFilterEntry.stmFilteParam.streamGateInstanceId   = (int32)1;
        psfpStmFilterEntry.stmFilteParam.streamMaxSDU           = (int32)1514;
        psfpStmFilterEntry.stmFilteParam.flowMeterInstanceId    = (int32)1;
        psfpStmFilterEntry.stmFilteParam.atsSchedulerInstanceId = (int32)-1;

        psfpStmFilterEntry.index = (uint32)1;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_filter_entry_set(unitChip, &psfpStmFilterEntry));

        psfpStmBlockOversize.index    = (int32)0x1;
        psfpStmBlockOversize.isEnable = (uint32)DISABLED;
        psfpStmBlockOversize.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_block_oversize_set(unitChip, &psfpStmBlockOversize));

        /***************************************************************************************************/
        /****  Step 3.3. Set stream filter entry 2:                                                     ****/
        /****                                   Stream Handle:  0x2                                     ****/
        /****                                        Priority:  Wildcard                                ****/
        /****                                   Stream MAX SDU:  1024                                   ****/
        /****                             Stream Gate Instance:  2                                      ****/
        /****                             Flow Meter Instance:  bypass                                  ****/
        /****                             ATS Scheduler Instance:  bypass                               ****/
        /****                             BlockStreamDueToOversizeFrame: disable                      ****/
        /****                             Clear the current BlockStreamDueToOversizeFrame status: No  ****/
        /****  Expected Behavior: To direct specific stream to assigned stream                          ****/
        /****                      and flow meter                                                       ****/
        /***************************************************************************************************/
        psfpStmFilterEntry.stmFilteParam.streamHandleSpec       = (int32)0x2;
        psfpStmFilterEntry.stmFilteParam.prioritySpec           = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.streamGateInstanceId   = (int32)2;
        psfpStmFilterEntry.stmFilteParam.streamMaxSDU           = (int32)1024;
        psfpStmFilterEntry.stmFilteParam.flowMeterInstanceId    = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.atsSchedulerInstanceId = (int32)-1;

        psfpStmFilterEntry.index = (uint32)2;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_filter_entry_set(unitChip, &psfpStmFilterEntry));

        psfpStmBlockOversize.index    = (int32)0x2;
        psfpStmBlockOversize.isEnable = (uint32)DISABLED;
        psfpStmBlockOversize.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_block_oversize_set(unitChip, &psfpStmBlockOversize));

        /***************************************************************************************************/
        /****  Step 3.4. Set stream filter entry 3:                                                     ****/
        /****                                   Stream Handle:  3                                       ****/
        /****                                        Priority:  Wildcard                                ****/
        /****                                   Stream MAX SDU:  1514                                   ****/
        /****                             Stream Gate Instance:  2                                      ****/
        /****                             Flow Meter Instance:  3                                       ****/
        /****                             ATS Scheduler Instance:  bypass                               ****/
        /****                             BlockStreamDueToOversizeFrame: disable                      ****/
        /****                             Clear the current BlockStreamDueToOversizeFrame status: No  ****/
        /****  Expected Behavior: To direct specific stream to assigned stream                          ****/
        /****                      and flow meter                                                       ****/
        /***************************************************************************************************/

        psfpStmFilterEntry.stmFilteParam.streamHandleSpec       = (int32)0x3;
        psfpStmFilterEntry.stmFilteParam.prioritySpec           = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.streamGateInstanceId   = (int32)2;
        psfpStmFilterEntry.stmFilteParam.streamMaxSDU           = (int32)1514;
        psfpStmFilterEntry.stmFilteParam.flowMeterInstanceId    = (int32)3;
        psfpStmFilterEntry.stmFilteParam.atsSchedulerInstanceId = (int32)-1;

        psfpStmFilterEntry.index = (uint32)3;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_filter_entry_set(unitChip, &psfpStmFilterEntry));

        psfpStmBlockOversize.index    = (uint32)3;
        psfpStmBlockOversize.isEnable = (uint32)DISABLED;
        psfpStmBlockOversize.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_block_oversize_set(unitChip, &psfpStmBlockOversize));

        /***************************************************************************************************/
        /****  Step 3.5. Set stream filter entry 4:                                                     ****/
        /****                                   Stream Handle:  wildcard                                ****/
        /****                                         Priority:  wildcard                               ****/
        /****                                   Stream MAX SDU:  bypass                                 ****/
        /****                             Stream Gate Instance:  3                                      ****/
        /****                             Flow Meter Instance:  bypass                                  ****/
        /****                             ATS Scheduler Instance:  bypass                               ****/
        /****                             BlockStreamDueToOversizeFrame: disable                      ****/
        /****                             Clear the current BlockStreamDueToOversizeFrame status: No  ****/
        /****  Expected Behavior: To direct specific stream to assigned stream                          ****/
        /***************************************************************************************************/
        /* To block the rest of streams, direct them to a closed gate. */
        psfpStmFilterEntry.stmFilteParam.streamHandleSpec       = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.prioritySpec           = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.streamGateInstanceId   = (int32)3;
        psfpStmFilterEntry.stmFilteParam.streamMaxSDU           = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.flowMeterInstanceId    = (int32)-1;
        psfpStmFilterEntry.stmFilteParam.atsSchedulerInstanceId = (int32)-1;

        psfpStmFilterEntry.index = (uint32)4;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_filter_entry_set(unitChip, &psfpStmFilterEntry));

        psfpStmBlockOversize.index    = (uint32)4;
        psfpStmBlockOversize.isEnable = (uint32)DISABLED;
        psfpStmBlockOversize.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_block_oversize_set(unitChip, &psfpStmBlockOversize));

        /***************************************************************************************************/
        /****  Step 4. Set stream gate: set the gate state or gate control list for packet operation   *****/
        /***************************************************************************************************/
        /* Set the entry parameters of the stream gate and enable GateClosedDueToOctetsExceeds with respective index. */
        /* Set the admin control list */

        /*********************************************************************************************/
        /****  Step 4.1 Set stream gate entry 0:                                                  ****/
        /****                                   Cycle time extension:  50000                      ****/
        /****                                              Base Time:  0                          ****/
        /****                               Gate Control List Length:  2                          ****/
        /****                                      Gate Control List:  Enabled                    ****/
        /****                     Gate State       IPV       Duration       MAX Interval Octet    ****/
        /****    Control List 1         open    bypass        5000000                     1000    ****/
        /****    Control List 2       closed    bypass       20000000                        0    ****/
        /****                            Block Stream if Gate Closed:  TRUE                       ****/
        /****              Block Stream if Exceed MAX Interval Octet:  TRUE                       ****/
        /****                                                                                     ****/
        /****  Expected Behavior: The frames that direct to the gate shall either be dropped or   ****/
        /****                      passed according to the configured gate control list            ****/
        /*********************************************************************************************/

        index             = (uint32)0;
        psfpGateIpv.index = index;
        psfpGateIpv.ipv   = (int32)-1;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_ipv_set(unitChip, &psfpGateIpv));

        /* Enable control list */
        psfpGateEnabled.index       = index;
        psfpGateEnabled.gateEnabled = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_enabled_set(unitChip, &psfpGateEnabled));

        psfpGateState.index     = index;
        psfpGateState.gateState = (uint32)DVA_CLOSED;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_state_set(unitChip, &psfpGateState));

        /* Set gate always closed, since the gate state depends on the control list */

        psfpGateConfig.baseTime.msbSecond  = (uint16)0;
        psfpGateConfig.baseTime.lsbSecond  = (uint32)0;
        psfpGateConfig.baseTime.nanosecond = (uint32)0;

        gateControlList[0].gateState        = (uint8)DVA_OPEN;
        gateControlList[0].internalPriority = (int32)-1;
        /* nanosecond */
        gateControlList[0].timeInterval      = (uint32)5000000;
        gateControlList[0].intervalOctectMax = (uint32)1000;
        gateControlList[1].gateState         = (uint8)DVA_CLOSED;
        gateControlList[1].internalPriority  = (int32)-1;
        /* nanosecond */
        gateControlList[1].timeInterval      = (uint32)20000000;
        gateControlList[1].intervalOctectMax = (uint32)0;

        psfpGateConfig.index              = index;
        psfpGateConfig.cycleTimeExtension = (uint32)50000;
        psfpGateConfig.controlListLength  = (uint32)2;
        psfpGateConfig.gateControlList    = gateControlList;

        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_config_set(unitChip, &psfpGateConfig));

        psfpGateClosedInvRx.index    = index;
        psfpGateClosedInvRx.isEnable = (uint32)ENABLED;
        psfpGateClosedInvRx.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_inv_rx_set(unitChip, &psfpGateClosedInvRx));

        psfpGateClosedOctExd.index    = index;
        psfpGateClosedOctExd.isEnable = (uint32)ENABLED;
        psfpGateClosedOctExd.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_oct_exd_set(unitChip, &psfpGateClosedOctExd));

        psfpGateEntValid.index = index;
        psfpGateEntValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_gate_ent_valid_set(unitChip, &psfpGateEntValid));

        /*********************************************************************************************/
        /****  Step 4.2 Set stream gate entry 1:                                                  ****/
        /****                                   Cycle time extension:  0                          ****/
        /****                                              Base Time:  0                          ****/
        /****                               Gate Control List Length:  4                          ****/
        /****                                      Gate Control List:  Enabled                    ****/
        /****                     Gate State       IPV       Duration       MAX Interval Octet    ****/
        /****    Control List 1         open         1        10000000                   20000    ****/
        /****    Control List 2       closed    bypass        10000000                        0   ****/
        /****    Control List 3         open         1        10000000                    20000   ****/
        /****    Control List 4       closed    bypass        10000000                        0   ****/
        /****                            Block Stream if Gate Closed:  TRUE                       ****/
        /****              Block Stream if Exceed MAX Interval Octet:  FALSE                      ****/
        /****                                                                                     ****/
        /****  Expected Behavior: The frames that direct to the gate shall either be dropped or   ****/
        /****                      passed according to the configured gate control list            ****/
        /*********************************************************************************************/
        index                              = (uint32)1;
        psfpGateConfig.baseTime.msbSecond  = (uint16)0;
        psfpGateConfig.baseTime.lsbSecond  = (uint32)0;
        psfpGateConfig.baseTime.nanosecond = (uint32)0;

        gateControlList[0].gateState        = (uint8)DVA_OPEN;
        gateControlList[0].internalPriority = (int32)-1;
        /* nanosecond */
        gateControlList[0].timeInterval      = (uint32)10000000;
        gateControlList[0].intervalOctectMax = (uint32)20000;
        gateControlList[1].gateState         = (uint8)DVA_CLOSED;
        gateControlList[1].internalPriority  = (int32)-1;
        /* nanosecond */
        gateControlList[1].timeInterval      = (uint32)10000000;
        gateControlList[1].intervalOctectMax = (uint32)0;
        gateControlList[2].gateState         = (uint8)DVA_OPEN;
        gateControlList[2].internalPriority  = (int32)-1;
        /* nanosecond */
        gateControlList[2].timeInterval      = (uint32)10000000;
        gateControlList[2].intervalOctectMax = (uint32)20000;
        gateControlList[3].gateState         = (uint8)DVA_CLOSED;
        gateControlList[3].internalPriority  = (int32)-1;
        /* nanosecond */
        gateControlList[3].timeInterval      = (uint32)10000000;
        gateControlList[3].intervalOctectMax = (uint32)0;

        psfpGateIpv.index = index;
        psfpGateIpv.ipv   = (int32)-1;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_ipv_set(unitChip, &psfpGateIpv));

        /* Enable control list */
        psfpGateEnabled.index       = index;
        psfpGateEnabled.gateEnabled = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_enabled_set(unitChip, &psfpGateEnabled));

        /* Set gate always closed, since the gate state depends on the control list */
        psfpGateState.index     = index;
        psfpGateState.gateState = (uint32)DVA_CLOSED;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_state_set(unitChip, &psfpGateState));

        psfpGateConfig.index              = index;
        psfpGateConfig.cycleTimeExtension = (uint32)0;
        psfpGateConfig.controlListLength  = (uint32)4;
        psfpGateConfig.gateControlList    = gateControlList;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_config_set(unitChip, &psfpGateConfig));

        psfpGateClosedInvRx.index    = index;
        psfpGateClosedInvRx.isEnable = (uint32)ENABLED;
        psfpGateClosedInvRx.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_inv_rx_set(unitChip, &psfpGateClosedInvRx));

        psfpGateClosedOctExd.index    = index;
        psfpGateClosedOctExd.isEnable = (uint32)DISABLED;
        psfpGateClosedOctExd.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_oct_exd_set(unitChip, &psfpGateClosedOctExd));

        psfpGateEntValid.index = index;
        psfpGateEntValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_gate_ent_valid_set(unitChip, &psfpGateEntValid));

        /*********************************************************************************************/
        /****  Step 4.3 Set stream gate entry 2:                                                  ****/
        /****                                      Gate Control List:  Disabled                   ****/
        /****                                             Gate State:  open                       ****/
        /****                                                    IPV:  7                          ****/
        /****                            Block Stream if Gate Closed:  FALSE                      ****/
        /****              Block Stream if Exceed MAX Interval Octet:  FALSE                      ****/
        /****                                                                                     ****/
        /****  Expected Behavior: Any frames that direct to the gate shall be discarded since     ****/
        /****                      the gate is configured closed.                                 ****/
        /*********************************************************************************************/
        index = (uint32)2;
        /* Disable control list */
        psfpGateEnabled.index       = index;
        psfpGateEnabled.gateEnabled = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_enabled_set(unitChip, &psfpGateEnabled));

        /* Set gate always open */
        psfpGateState.index     = index;
        psfpGateState.gateState = (uint32)DVA_OPEN;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_state_set(unitChip, &psfpGateState));

        /* Set IPV */
        psfpGateIpv.index = index;
        psfpGateIpv.ipv   = (int32)7;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_ipv_set(unitChip, &psfpGateIpv));

        psfpGateClosedInvRx.index    = index;
        psfpGateClosedInvRx.isEnable = (uint32)DISABLED;
        psfpGateClosedInvRx.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_inv_rx_set(unitChip, &psfpGateClosedInvRx));

        psfpGateClosedOctExd.index    = index;
        psfpGateClosedOctExd.isEnable = (uint32)DISABLED;
        psfpGateClosedOctExd.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_oct_exd_set(unitChip, &psfpGateClosedOctExd));

        psfpGateEntValid.index = index;
        psfpGateEntValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_gate_ent_valid_set(unitChip, &psfpGateEntValid));

        /*********************************************************************************************/
        /****  Step 4.4 Set stream gate entry 3:                                                  ****/
        /****                                      Gate Control List:  Disabled                   ****/
        /****                                             Gate State:  closed                     ****/
        /****                                                    IPV:  bypass                     ****/
        /****                            Block Stream if Gate Closed:  FALSE                      ****/
        /****              Block Stream if Exceed MAX Interval Octet:  FALSE                      ****/
        /****                                                                                     ****/
        /****  Expected Behavior: Any frames that direct to the gate shall be discarded since     ****/
        /****                      the gate is configured closed.                                 ****/
        /*********************************************************************************************/
        index = (uint32)3;
        /* Disable control list */
        psfpGateEnabled.index       = index;
        psfpGateEnabled.gateEnabled = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_enabled_set(unitChip, &psfpGateEnabled));
        /* Set gate always closed */
        psfpGateState.index     = index;
        psfpGateState.gateState = (uint32)DVA_CLOSED;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_gate_state_set(unitChip, &psfpGateState));
        /* Set bypass IPV */
        psfpGateIpv.index = index;
        psfpGateIpv.ipv   = (int32)-1;
        CHK_FUN_CONTINUE(result, rtk_psfp_admin_ipv_set(unitChip, &psfpGateIpv));
        psfpGateClosedInvRx.index    = index;
        psfpGateClosedInvRx.isEnable = (uint32)DISABLED;
        psfpGateClosedInvRx.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_inv_rx_set(unitChip, &psfpGateClosedInvRx));
        psfpGateClosedOctExd.index    = index;
        psfpGateClosedOctExd.isEnable = (uint32)DISABLED;
        psfpGateClosedOctExd.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_gate_closed_oct_exd_set(unitChip, &psfpGateClosedOctExd));
        psfpGateEntValid.index = index;
        psfpGateEntValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_stm_gate_ent_valid_set(unitChip, &psfpGateEntValid));

        /**************************************************************************************/
        /****  Step 5. Set flow meter: set the flow meter configuration for flow control  *****/
        /**************************************************************************************/

        /**********************************************************************************/
        /****  Step 5.1 Set flow meter entry 0:                                        ****/
        /****                                  committedInfoRate:  10000               ****/
        /****                                  committedBurstSize:  20000              ****/
        /****                                  excessInfoRate:  0                      ****/
        /****                                  excessBurstSize:  0                     ****/
        /****                        Coupling Flag:  FALSE                             ****/
        /****                           Color Mode:  Color-Blind                       ****/
        /****                    Drop Yellow Frame:  FALSE                             ****/
        /****                        DEI Selection:  Inner Tag                         ****/
        /****                 IPV for Yellow Frame:  Bypass                            ****/
        /****            Mark Stream Red when drop:  Disable                           ****/
        /****                                                                          ****/
        /****  Expected Behavior: The frames that are directed to the flow meter shall ****/
        /****                      be dropped if the outer DVA_VLAN Tag DEI is set.    ****/
        /****                      The maximum traffic of this flow meter is 10kbps.   ****/
        /**********************************************************************************/
        index = (uint32)0;
        /* in kbps */
        psfpFmEntry.flowMeterParam.committedInfoRate = (uint32)10;
        /* in bytes */
        psfpFmEntry.flowMeterParam.committedBurstSize = (uint32)20000;
        /* in kbps */
        psfpFmEntry.flowMeterParam.excessInfoRate = (uint32)0;
        /* in bytes */
        psfpFmEntry.flowMeterParam.excessBurstSize      = (uint32)0;
        psfpFmEntry.flowMeterParam.couplingFlag         = (uint32)0;
        psfpFmEntry.flowMeterParam.colorMode            = (uint32)0;
        psfpFmEntry.flowMeterParam.dropOnYellow         = (uint32)TSN_INVALID;
        psfpFmEntry.flowMeterParam.selectionDEI         = (uint32)0;
        psfpFmEntry.flowMeterParam.isEnableIPVForYellow = (uint32)TSN_INVALID;
        psfpFmEntry.flowMeterParam.yellowIPV            = (uint32)0;

        psfpFmEntry.index = index;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_set(unitChip, &psfpFmEntry));
        psfpMarkAllFramesRed.index    = index;
        psfpMarkAllFramesRed.isEnable = (uint32)DISABLED;
        psfpMarkAllFramesRed.status   = (uint32)TSN_INVALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_mark_all_frames_red_set(unitChip, &psfpMarkAllFramesRed));
        psfpFmEntryValid.index = index;
        psfpFmEntryValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_valid_set(unitChip, &psfpFmEntryValid));

        /**********************************************************************************/
        /****  Step 5.2 Set flow meter entry 1:                                        ****/
        /****                                  committedInfoRate:  25000               ****/
        /****                                  committedBurstSize:  80000              ****/
        /****                                  excessInfoRate:  25000                  ****/
        /****                                  excessBurstSize:  80000                 ****/
        /****                        Coupling Flag:  TRUE                              ****/
        /****                           Color Mode:  Color-Aware                       ****/
        /****                    Drop Yellow Frame:  FALSE                             ****/
        /****                        DEI Selection:  Inner Tag                         ****/
        /****                 IPV for Yellow Frame:  0                                 ****/
        /****            Mark Stream Red when drop:  Enable                            ****/
        /****                                                                          ****/
        /****  Expected Behavior: The IPV of frames that are directed to the flow      ****/
        /****                      meter shall be changed to 0 if the inner DVA_VLAN   ****/
        /****                      Tag DEI of the frame is set.                        ****/
        /****                      The maximum traffic of this flow meter is 25kbps    ****/
        /****                      for both green and yellow frames.                   ****/
        /**********************************************************************************/
        index = (uint32)1;

        psfpFmEntry.flowMeterParam.committedInfoRate    = (uint32)25;
        psfpFmEntry.flowMeterParam.committedBurstSize   = (uint32)80000;
        psfpFmEntry.flowMeterParam.excessInfoRate       = (uint32)25;
        psfpFmEntry.flowMeterParam.excessBurstSize      = (uint32)80000;
        psfpFmEntry.flowMeterParam.couplingFlag         = (uint32)1;
        psfpFmEntry.flowMeterParam.colorMode            = (uint32)1;
        psfpFmEntry.flowMeterParam.dropOnYellow         = (uint32)TSN_INVALID;
        psfpFmEntry.flowMeterParam.selectionDEI         = (uint32)0;
        psfpFmEntry.flowMeterParam.isEnableIPVForYellow = (uint32)TSN_VALID;
        psfpFmEntry.flowMeterParam.yellowIPV            = (uint32)0;
        psfpFmEntry.index                               = index;

        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_set(unitChip, &psfpFmEntry));
        psfpMarkAllFramesRed.index    = index;
        psfpMarkAllFramesRed.isEnable = (uint32)ENABLED;
        psfpMarkAllFramesRed.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_mark_all_frames_red_set(unitChip, &psfpMarkAllFramesRed));
        psfpFmEntryValid.index = index;
        psfpFmEntryValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_valid_set(unitChip, &psfpFmEntryValid));
        /**********************************************************************************/
        /****  Step 5.3 Set flow meter entry 3:                                        ****/
        /****                                  committedInfoRate:  5000                ****/
        /****                                  committedBurstSize:  10000              ****/
        /****                                  excessInfoRate:  0                      ****/
        /****                                  excessBurstSize:  0                     ****/
        /****                        Coupling Flag:  FALSE                             ****/
        /****                           Color Mode:  Color-Blind                       ****/
        /****                    Drop Yellow Frame:  FALSE                             ****/
        /****                        DEI Selection:  Inner Tag                         ****/
        /****                 IPV for Yellow Frame:  Bypass                            ****/
        /****            Mark Stream Red when drop:  Enable                            ****/
        /****                                                                          ****/
        /****  Expected Behavior: The IPV of frames that are directed to the flow      ****/
        /****                      meter shall be changed to 0 if the outer DVA_VLAN   ****/
        /****                      Tag DEI of the frame is set.                        ****/
        /****                      The maximum traffic of this flow meter is 5kbps.    ****/
        /**********************************************************************************/
        index = (uint32)3;

        psfpFmEntry.flowMeterParam.committedInfoRate    = (uint32)5;
        psfpFmEntry.flowMeterParam.committedBurstSize   = (uint32)10000;
        psfpFmEntry.flowMeterParam.excessInfoRate       = (uint32)0;
        psfpFmEntry.flowMeterParam.excessBurstSize      = (uint32)0;
        psfpFmEntry.flowMeterParam.couplingFlag         = (uint32)0;
        psfpFmEntry.flowMeterParam.colorMode            = (uint32)0;
        psfpFmEntry.flowMeterParam.dropOnYellow         = (uint32)TSN_INVALID;
        psfpFmEntry.flowMeterParam.selectionDEI         = (uint32)0;
        psfpFmEntry.flowMeterParam.isEnableIPVForYellow = (uint32)TSN_INVALID;
        psfpFmEntry.flowMeterParam.yellowIPV            = (uint32)0;
        psfpFmEntry.index                               = index;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_set(unitChip, &psfpFmEntry));
        psfpMarkAllFramesRed.index    = index;
        psfpMarkAllFramesRed.isEnable = (uint32)ENABLED;
        psfpMarkAllFramesRed.status   = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_mark_all_frames_red_set(unitChip, &psfpMarkAllFramesRed));
        psfpFmEntryValid.index = index;
        psfpFmEntryValid.valid = (uint32)TSN_VALID;
        CHK_FUN_CONTINUE(result, rtk_psfp_fm_entry_valid_set(unitChip, &psfpFmEntryValid));

        /************************************************************************************************************************************************/
        /****  Step 6. As recommended, the validity of stream filters dominates the PSFP flow, so activate the stream filter entry in the last step  ****/
        /****                                                                                                                                        ****/
        /****  Expected Behavior: The frames start to pass to the stream gate and flow meter, or be blocked by the stream filter.                    ****/
        /************************************************************************************************************************************************/
        stmFlEntryValid.valid = (uint32)TSN_VALID;
        for(index = (uint32)0; index < 5u; index++)
        {
            /* Set stream filter valid in the last step as recommended */
            stmFlEntryValid.index = index;
            CHK_FUN_CONTINUE(result, rtk_psfp_stm_fl_entry_valid_set(unitChip, &stmFlEntryValid));
        }

    } while(0u == 1u);
    return result;
}
/**@}*/ /* RTL907XDVA_PSFP_CONFIGURATION_EXAMPLE */
/**@}*/ /* RTL907XDVA_PSFP_EXAMPLE */
/**@}*/ /* PSCM_EXAMPLE */
/**@}*/ /* EXAMPLE */