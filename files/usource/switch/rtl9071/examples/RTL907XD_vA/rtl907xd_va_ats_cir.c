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
#include "rtkas_api_ats.h"
#include "rtkas_api_ptp.h"
#include "rtkas_api_psfp.h"
#include "rtkas_api_qos.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup PSCM_EXAMPLE PSCM(QCI/PSFP & QCR/ATS) Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_ATS_EXAMPLE RTL907XD_VA ATS Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_ATS_CIR_EXAMPLE ATS CIR Example
 * @brief <b>ATS CIR speed limit</b> \n
 *           Configure ATS CIR to limit the transmission rate.
 *
 * @section Purpose
 *          In order to achieve the rate limiting for the TX port.
 *
 * @section Description
 *          The outgoing traffic from port 3 TX has a maximum rate of 500 Kbps. Any traffic exceeding this rate will be limited to 500 Kbps.
 * @{
 */

RtkApiRet dva_ats_cir(void);

/* Function */

RtkApiRet dva_ats_cir(void)
{
    RtkApiRet                 retValue            = RT_ERR_OK;
    UnitChip_t                unitChip            = {0};
    uint8                     schedulerId1        = 4u;
    AtsCbsCirInfo_t           atsCbsCirInfo       = {0};
    PsfpStmFilterEntry_t      stmFilterEntry      = {0};
    PsfpGateEntValid_t        psfpStmGateEntValid = {0};
    PsfpGateState_t           psfpAdminGateState  = {0};
    PsfpStmFilterEntryValid_t stmFlEntryValid     = {0};
    QosPriQueMapping_t        qosPriQueMapping    = {0};
    AtsSetInfo_t              atsSetInfo          = {0};
    uint32                    queue               = 0u;
    QosPriSelInfo_t           qosPriSelInfo       = {0};
    QosPriWtTbl_t             qosPriWtTbl         = {0};
    PsfpMode_t                psfpMode            = {0};
    QosQueSchInfo_t           queSchInfo          = {0};

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

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = DVA_UNIT;
        unitChip.chip = CHIP_907XD_VA;

        /* Step 2. Start PTP */
        CHK_FUN_CONTINUE(retValue, rtk_ptp_set_master_en(unitChip, &ptpMasterEnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_get_master_en(unitChip, &ptpMasterEnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_set_slave0_en(unitChip, &ptpSlave0EnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_get_slave0_en(unitChip, &ptpSlave0EnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_set_slave1_en(unitChip, &ptpSlave1EnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_get_slave1_en(unitChip, &ptpSlave1EnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_set_slave2_en(unitChip, &ptpSlave2EnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_get_slave2_en(unitChip, &ptpSlave2EnInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_set_hw_enable(unitChip, &ptpHwEnableInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ptp_get_hw_enable(unitChip, &ptpHwEnableInfo));

        /* Step 3. Set a weight for each priority source.
            Priority arbitration use cvlan. */
        qosPriSelInfo.group              = 0u;
        qosPriSelInfo.qosPriSel.cvlanPri = 7u;
        CHK_FUN_CONTINUE(retValue, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
        CHK_FUN_CONTINUE(retValue, rtk_qos_pri_sel_get(unitChip, &qosPriSelInfo));

        /* Step 4. Set port 3 to use the first priority extraction arbitration table.  */
        qosPriWtTbl.port = DVA_RTK_USER_PORT_3RD;
        qosPriWtTbl.idx  = 0;
        CHK_FUN_CONTINUE(retValue, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));
        CHK_FUN_CONTINUE(retValue, rtk_qos_pri_wt_tbl_get(unitChip, &qosPriWtTbl));

        /* Step 5. To set whether the PSFP module is able to drop the trapped dpm */
        psfpMode.mode = 1u;
        CHK_FUN_CONTINUE(retValue, rtk_psfp_mode_set(unitChip, &psfpMode));
        CHK_FUN_CONTINUE(retValue, rtk_psfp_mode_get(unitChip, &psfpMode));

        /* Step 6. Set the stream filter entry */
        stmFilterEntry.stmFilteParam.streamHandleSpec       = -1; /* wildcard */
        stmFilterEntry.stmFilteParam.prioritySpec           = 2;  /* priority 2 */
        stmFilterEntry.stmFilteParam.streamGateInstanceId   = 3;
        stmFilterEntry.stmFilteParam.streamMaxSDU           = -1; /* bypass */
        stmFilterEntry.stmFilteParam.flowMeterInstanceId    = -1; /* bypass */
        stmFilterEntry.stmFilteParam.atsSchedulerInstanceId = 4;
        stmFilterEntry.index                                = 2;
        CHK_FUN_CONTINUE(retValue, rtk_psfp_stm_filter_entry_set(unitChip, &stmFilterEntry));
        CHK_FUN_CONTINUE(retValue, rtk_psfp_stm_filter_entry_get(unitChip, &stmFilterEntry));

        stmFlEntryValid.index = 2;
        stmFlEntryValid.valid = 1;
        CHK_FUN_CONTINUE(retValue, rtk_psfp_stm_fl_entry_valid_set(unitChip, &stmFlEntryValid));
        CHK_FUN_CONTINUE(retValue, rtk_psfp_stm_fl_entry_valid_get(unitChip, &stmFlEntryValid));

        /* Step 7. Set the stream gate entry valid and gate state open */
        psfpStmGateEntValid.index = 3u;
        psfpStmGateEntValid.valid = 1u;
        CHK_FUN_CONTINUE(retValue, rtk_psfp_stm_gate_ent_valid_set(unitChip, &psfpStmGateEntValid));
        CHK_FUN_CONTINUE(retValue, rtk_psfp_stm_gate_ent_valid_get(unitChip, &psfpStmGateEntValid));

        psfpAdminGateState.index     = 3u;
        psfpAdminGateState.gateState = 1u;
        CHK_FUN_CONTINUE(retValue, rtk_psfp_admin_gate_state_set(unitChip, &psfpAdminGateState));
        CHK_FUN_CONTINUE(retValue, rtk_psfp_admin_gate_state_get(unitChip, &psfpAdminGateState));

        /* Step 8. Set the ATS scheduler */
        CHK_FUN_CONTINUE(retValue, rtk_ats_init(unitChip));

        atsCbsCirInfo.schedulerId = schedulerId1;
        atsCbsCirInfo.groupId     = schedulerId1;
        atsCbsCirInfo.cbs         = 100;
        atsCbsCirInfo.cirKbps     = QCR_CIR_MIN;
        CHK_FUN_CONTINUE(retValue, rtk_ats_cbs_cir_set(unitChip, &atsCbsCirInfo));
        CHK_FUN_CONTINUE(retValue, rtk_ats_cbs_cir_get(unitChip, &atsCbsCirInfo));

        /* Step 9. Set port 3 queue 2 mapping to queue 2 */
        qosPriQueMapping.port = DVA_RTK_USER_PORT_3RD;
        qosPriQueMapping.pri  = 2;
        qosPriQueMapping.que  = 2;
        CHK_FUN_CONTINUE(retValue, rtk_qos_pri_que_mapping_set(unitChip, &qosPriQueMapping));
        CHK_FUN_CONTINUE(retValue, rtk_qos_pri_que_mapping_get(unitChip, &qosPriQueMapping));

        /* Step 10. Set port 3 queue 2 is ATS queue, and the mode is enable */
        atsSetInfo.port  = DVA_RTK_USER_PORT_3RD;
        atsSetInfo.queue = 2u;
        atsSetInfo.mode  = ENABLE;
        CHK_FUN_CONTINUE(retValue, rtk_ats_scheduler_set(unitChip, &atsSetInfo));

        queSchInfo.port = DVA_RTK_USER_PORT_3RD;
        queSchInfo.que  = queue;
        CHK_FUN_CONTINUE(retValue, rtk_qos_que_sch_get(unitChip, &queSchInfo));

        /* Expected Behavior: The outgoing speed from port 3 TX will be limited to 500 Kbps. */
    } while(0u == 1u);

    return retValue;
}

/**@}*/ /* RTL907XD_VA_ATS_CIR_EXAMPLE */
/**@}*/ /* RTL907XD_VA_ATS_EXAMPLE */
/**@}*/ /* PSCM_EXAMPLE */
/**@}*/ /* EXAMPLE */
