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

#include "rtkas_api_ptp_ext.h"
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup PTP PTP Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_PTP_EXAMPLE RTL907XD_VA PTP Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_PTP_AUTOSAR_TIME_SYNC_EXAMPLE PTP Autosar Time Sync Example
 * @brief <b>Autosar Time Sync Configuration</b> \n
 * @section Purpose
 *              This example is to synchronize time by 2-step sync/follow up with Autosar Time Sync profile. \n\n
 * @section Description
 *              It would configure PTP enable, mode, domain ID 0, slave port 1, and joined ports 1 ~ 3. \n
 *              After executing this example, the switch act as a time-aware bridge which synchronize time with GM clock and forward Sync/Follow up message to end point. \n
 *              It will run Autosar Time Sync behavior, validated Follow-Up autosar sub-TLV.
 * @{
 */

RtkApiRet        dva_ptp_autosar_time_sync(void);
static RtkApiRet dva_ptp_set_internal_port_config_ats_tsync(void);

static RtkApiRet dva_ptp_set_internal_port_config_ats_tsync(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t unitChip;
    uint32     port = DVA_RTK_CPU_PORT;
    /* HW port mode to P2P */
    uint8 ptpModeInHWReg = 4u;

    unitChip.chip = CHIP_907XD_VA;
    unitChip.unit = DVA_UNIT;

    PtpPortmode_t            ptpPortmode;
    PtpPortPtp100Tx_t        ptpPortPtp100Tx;
    PtpEtherCheckEnable0_t   ptpEtherCheckEnable0;
    PtpPortSlaveTimeDomain_t PtpPortSlaveTimeDomain;
    PtpPortTimeSyncEn_t      ptpPortTimeSyncEn;
    PtpPortRxlatency_t       ptpPortRxlatency;
    PtpPortTxlatency_t       ptpPortTxlatency;

    do
    {
        /* Step 1.12.1 Set all ports, HW port mode to P2P */
        ptpPortmode.port     = port;
        ptpPortmode.portmode = ptpModeInHWReg;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_portmode(unitChip, &ptpPortmode)));

        /* Step 1.12.2 Set all ports, Setup HW timestamp point enable. */
        ptpPortPtp100Tx.port     = port;
        ptpPortPtp100Tx.ptp100tx = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_port_ptp_100tx(unitChip, &ptpPortPtp100Tx)));

        /* Step 1.12.3 Set all ports, Enable RX ethertype filtering */
        ptpEtherCheckEnable0.port      = port;
        ptpEtherCheckEnable0.rtkEnable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_ether_check_enable0(unitChip, &ptpEtherCheckEnable0)));

        /* Step 1.12.4 Set all ports, Set slave time domain 0. */
        PtpPortSlaveTimeDomain.port        = port;
        PtpPortSlaveTimeDomain.domainid    = 0u;
        PtpPortSlaveTimeDomain.domainvalue = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain)));

        /* Step 1.12.5 Set all ports, Set slave time domain 1. */
        PtpPortSlaveTimeDomain.port        = port;
        PtpPortSlaveTimeDomain.domainid    = 1u;
        PtpPortSlaveTimeDomain.domainvalue = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain)));

        /* Step 1.12.6 Set all ports, Set slave time domain 2. */
        PtpPortSlaveTimeDomain.port        = port;
        PtpPortSlaveTimeDomain.domainid    = 1u;
        PtpPortSlaveTimeDomain.domainvalue = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain)));

        /* Step 1.12.7 Set all ports, Enable time sync. */
        ptpPortTimeSyncEn.port       = port;
        ptpPortTimeSyncEn.timeSyncEn = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_port_time_sync_en(unitChip, &ptpPortTimeSyncEn)));

        /* Step 1.12.8 Set all ports, Initial latency correction for ingress timestamp at MAC. */
        ptpPortRxlatency.port      = port;
        ptpPortRxlatency.rxLatency = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_port_rxlatency(unitChip, &ptpPortRxlatency)));

        /* Step 1.12.9 Set all ports, Initial latency correction for egress timestamp at MAC. */
        ptpPortTxlatency.port      = port;
        ptpPortTxlatency.txLatency = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_port_txlatency(unitChip, &ptpPortTxlatency)));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_ptp_set_autosar_config(uint8 slavePort, uint16 joinedPortMask, uint8 domainId, PtpTs_t pdelayThres)
{
    UnitChip_t unitChip;
    unitChip.chip        = CHIP_907XD_VA;
    unitChip.unit        = DVA_UNIT;
    RtkApiRet ret        = RT_ERR_OK;
    uint8     portIdx    = DVA_RTK_USER_PORT_1ST;
    uint8     dataid[16] = {0x01u, 0x23u, 0x45u, 0x67u, 0x89u, 0xABu, 0xCDu, 0xEFu,
                            0x01u, 0x23u, 0x45u, 0x67u, 0x89u, 0xABu, 0xCDu, 0xEFu};

    PtpAtsarMsgCompliance_t       ptpAtsarMsgCompliance;
    PtpAtsarRxCrcValidated_t      ptpAtsarRxCrcValidated;
    PtpAtsarCrcFlags_t            ptpAtsarCrcFlags;
    PtpAtsarTimeSubtlv_t          ptpAtsarTimeSubtlv;
    PtpAtsarStatusSubtlv_t        ptpAtsarStatusSubtlv;
    PtpAtsarUdataSubtlv_t         ptpAtsarUdataSubtlv;
    PtpAtsarOfsSubtlv_t           ptpAtsarOfsSubtlv;
    PtpAtsarTxCrcTimeFlags_t      ptpAtsarTxCrcTimeFlags;
    PtpAtsarTxTimeCrcSecured_t    ptpAtsarTxTimeCrcSecured;
    PtpAtsarPdelayRespEnable_t    ptpAtsarPdelayRespEnable;
    PtpAtsarOfsMulticity_t        ptpAtsarOfsMulticity;
    PtpAtsarFoloupDataidList_t    ptpAtsarFoloupDataidList;
    PtpAtsarCrcSupport_t          ptpAtsarCrcSupport;
    PtpAtsarFramePri_t            ptpAtsarFramePri;
    PtpAtsarTagVidPri_t           ptpAtsarTagVidPri;
    PtpCustomVlanEnable_t         ptpCustomVlanEnable;
    PtpAtsarTxPdelayReqPeriod_t   ptpAtsarTxPdelayReqPeriod;
    PtpInitlogpdelayintval_t      ptpInitlogpdelayintval;
    PtpOperlogpdelayintval_t      ptpOperlogpdelayintval;
    PtpCurntPdelayreqIntval_t     ptpCurntPdelayreqIntval;
    PtpAtsarPdelayLatencyThresh_t ptpAtsarPdelayLatencyThresh;
    PtpAtsarPropDelay_t           ptpAtsarPropDelay;
    PtpNeidelay_t                 ptpNeidelay;
    PtpAtsarTxPeriod_t            ptpAtsarTxPeriod;
    PtpInitlogsyncintval_t        ptpInitlogsyncintval;
    PtpOperlogsyncintval_t        ptpOperlogsyncintval;
    PtpCurntsyncintval_t          ptpCurntsyncintval;
    PtpMgtsettableLogsyncintval_t ptpMgtsettableLogsyncintval;
    PtpDelaythresh_t              ptpDelaythresh;
    PtpIntvalToLog_t              ptpIntvalToLog;
    ptpNewQTaginfo                taginfo;

    do
    {
        /* Step 1.20.1 Enable autosar crc support, set to 1. */
        ptpAtsarCrcSupport.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_crc_support(unitChip, &ptpAtsarCrcSupport)));

        /* Step 1.20.2 Disable autosar frame priority vlan support, set to 0. */
        ptpAtsarFramePri.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_frame_pri(unitChip, &ptpAtsarFramePri)));
        for(portIdx = DVA_RTK_USER_PORT_1ST; portIdx <= DVA_RTK_USER_PORT_END; portIdx++)
        {
            if(0u == (joinedPortMask & ((uint16)1u << portIdx)))
            {
                continue;
            }
            ptpCustomVlanEnable.enable = 0u;
            ptpCustomVlanEnable.port   = portIdx;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_custom_set_vlan_enable(unitChip, &ptpCustomVlanEnable)));
        }
        ptpAtsarTagVidPri.taginfo          = &taginfo;
        ptpAtsarTagVidPri.domainid         = domainId;
        ptpAtsarTagVidPri.taginfo->enable0 = 0u;
        ptpAtsarTagVidPri.taginfo->pcp0    = 0u;
        ptpAtsarTagVidPri.taginfo->cfi0    = 0u;
        ptpAtsarTagVidPri.taginfo->fix     = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_tag_vid_pri_set(unitChip, &ptpAtsarTagVidPri)));

        /* Step 1.20.3 Set autosar pdelay request transmission period to 1000ms. This configuration would overwrite the Step 1.13.11 Set pdelay request logMessageInterval */
        ptpAtsarTxPdelayReqPeriod.pdelayIntval = (uint32)EM_PTP_1000_MS;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_tx_pdelayreq_period(unitChip, &ptpAtsarTxPdelayReqPeriod)));
        for(portIdx = DVA_RTK_USER_PORT_1ST; portIdx <= DVA_RTK_USER_PORT_END; portIdx++)
        {
            if(0u == (joinedPortMask & ((uint16)1u << portIdx)))
            {
                continue;
            }

            ptpIntvalToLog.curntIntval = EM_PTP_1000_MS;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_intval_to_log(unitChip, &ptpIntvalToLog)));
            ptpInitlogpdelayintval.pdelayintval = ptpIntvalToLog.logIntval;
            ptpInitlogpdelayintval.port         = portIdx;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_set_initlogpdelayintval(unitChip, &ptpInitlogpdelayintval)));

            ptpOperlogpdelayintval.pdelayintval = ptpIntvalToLog.logIntval;
            ptpOperlogpdelayintval.port         = portIdx;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_set_operlogpdelayintval(unitChip, &ptpOperlogpdelayintval)));

            ptpCurntPdelayreqIntval.curntPdelayReqIntval = (uint32)EM_PTP_1000_MS;
            ptpCurntPdelayreqIntval.port                 = portIdx;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_set_curnt_pdelayreq_intval(unitChip, &ptpCurntPdelayreqIntval)));
        }

        /* Step 1.20.4 Set the valid pdelay threshold to 500ms. */
        ptpAtsarPdelayLatencyThresh.delayThresh = pdelayThres.nanoSec;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_pdelay_latency_thresh(unitChip, &ptpAtsarPdelayLatencyThresh)));
        ptpDelaythresh.delaythresh.up16Sec  = pdelayThres.up16Sec;
        ptpDelaythresh.delaythresh.low32Sec = pdelayThres.low32Sec;
        ptpDelaythresh.delaythresh.nanoSec  = pdelayThres.nanoSec;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_delaythresh(unitChip, &ptpDelaythresh)));

        /* Step 1.20.5 Set the propagation delay default value to 0. */
        ptpAtsarPropDelay.pDelay = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_prop_delay(unitChip, &ptpAtsarPropDelay)));
        if(0u != ptpAtsarPropDelay.pDelay)
        {
            ptpNeidelay.neiDelay = 0u;
            ptpNeidelay.port     = slavePort;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_neidelay_set(unitChip, &ptpNeidelay)));
        }

        /* Step 1.20.6 Set autosar sync/followUp transmission period to 125ms. It would overwrite Step 1.13.17 Set sync logMessageInterval. */
        ptpAtsarTxPeriod.intval = (uint32)EM_PTP_125_MS;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_tx_period(unitChip, &ptpAtsarTxPeriod)));
        ptpIntvalToLog.curntIntval = EM_PTP_125_MS;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_intval_to_log(unitChip, &ptpIntvalToLog)));
        ptpOperlogsyncintval.domainid   = domainId;
        ptpOperlogsyncintval.syncintval = ptpIntvalToLog.logIntval;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_operlogsyncintval(unitChip, &ptpOperlogsyncintval)));
        ptpMgtsettableLogsyncintval.domain    = domainId;
        ptpMgtsettableLogsyncintval.logIntval = ptpIntvalToLog.logIntval;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_logsyncintval_mgtsettable(unitChip, &ptpMgtsettableLogsyncintval)));
        for(portIdx = DVA_RTK_USER_PORT_1ST; portIdx <= DVA_RTK_USER_PORT_END; portIdx++)
        {
            if(0u == (joinedPortMask & ((uint16)1u << portIdx)))
            {
                continue;
            }
            ptpInitlogsyncintval.domainid          = domainId;
            ptpInitlogsyncintval.initLogSyncIntval = ptpIntvalToLog.logIntval;
            ptpInitlogsyncintval.port              = portIdx;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_set_initlogsyncintval(unitChip, &ptpInitlogsyncintval)));

            ptpCurntsyncintval.curntsyncintval = (uint32)EM_PTP_125_MS;
            ptpCurntsyncintval.domainid        = domainId;
            ptpCurntsyncintval.port            = portIdx;
            CHK_FUN_CONTINUE((ret), (rtk_ptp_set_curntsyncintval(unitChip, &ptpCurntsyncintval)));
        }

        /* Step 1.20.7 Set autosar MessageCompliance to false: IEEE 802.1AS message format with AUTOSAR extension will be used. */
        ptpAtsarMsgCompliance.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_msg_compliance(unitChip, &ptpAtsarMsgCompliance)));

        /* Step 1.20.8 Set autosar RxCrcValidated to CRC_VALIDATED. */
        ptpAtsarRxCrcValidated.validated = EM_PTP_CRC_VALIDATED;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_rx_crc_validated(unitChip, &ptpAtsarRxCrcValidated)));
        /* Step 1.20.9 Enable autosar CrcFlagsRxValidated, set to 1 */
        ptpAtsarCrcFlags.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_crc_flags(unitChip, &ptpAtsarCrcFlags)));
        /* Step 1.20.10 Enable Autosar Time Secured sub-TLV */
        ptpAtsarTimeSubtlv.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_time_subtlv(unitChip, &ptpAtsarTimeSubtlv)));
        /* Step 1.20.11 Enable Autosar Status Secured sub-TLV */
        ptpAtsarStatusSubtlv.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_status_subtlv(unitChip, &ptpAtsarStatusSubtlv)));
        /* Step 1.20.12 Enable Autosar UserData Secured sub-TLV */
        ptpAtsarUdataSubtlv.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_udata_subtlv(unitChip, &ptpAtsarUdataSubtlv)));
        /* Step 1.20.13 Enable Autosar OFS sub-TLV */
        ptpAtsarOfsSubtlv.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_ofs_subtlv(unitChip, &ptpAtsarOfsSubtlv)));
        /* Step 1.20.14 Set CrcFlagsRxValidated to 0x3F to use CRC calculation on MessageLength, DomainNumber, CorrectionField, SourcePortIdentity, SequenceIdentity. */
        ptpAtsarTxCrcTimeFlags.flags = 0x3Fu;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_tx_crc_time_flags(unitChip, &ptpAtsarTxCrcTimeFlags)));
        /* Step 1.20.15 Enable transmission CRC calculation, set to 1. */
        ptpAtsarTxTimeCrcSecured.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_tx_time_crc_secured(unitChip, &ptpAtsarTxTimeCrcSecured)));
        /* Step 1.20.16 Enable pdelay resp, set to 1. */
        ptpAtsarPdelayRespEnable.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_pdelay_resp_enable(unitChip, &ptpAtsarPdelayRespEnable)));
        /* Step 1.20.17 Set OFS sub-TLV multiplicity to 3. */
        ptpAtsarOfsMulticity.ofsMulti = 3u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_ofs_multicity(unitChip, &ptpAtsarOfsMulticity)));
        /* Step 1.20.18 Set the DataIDList arrary which is used to do CRC calculation. */
        (void)rtl_memcpy(ptpAtsarFoloupDataidList.dataid, dataid, sizeof(ptpAtsarFoloupDataidList.dataid));
        CHK_FUN_CONTINUE((ret), (rtk_ptp_atsar_set_foloup_dataid_list(unitChip, &ptpAtsarFoloupDataidList)));

    } while(false);

    return ret;
}

RtkApiRet dva_ptp_autosar_time_sync(void)
{
    RtkPort    port             = 0u;
    uint16     slvPtMask        = 0u;
    RtkApiRet  ret              = RT_ERR_OK;
    uint64     callAPIerrorFlag = 0u;
    RtkApiRet  apiresult        = RT_ERR_OK;
    UnitChip_t unitChip;
    unitChip.chip = CHIP_907XD_VA;
    unitChip.unit = DVA_UNIT;

    /* Slave port: 1 */
    uint8 slaveIF = DVA_RTK_USER_PORT_1ST;
    /* Domain: 0 */
    uint8    domainNum     = 0u;
    CFiled_t delayAsymTime = {0u, 0u, 0u};
    /*  SW port mode to P2P */
    uint8 ptpModeinCPUTag = 3u;
    /* HW port mode to P2P */
    uint8 ptpModeInHWReg = 4u;
    /* portmask[0]: port enable mask
     * portmask[1]: ascapable mask
     * portmask[2]: calDelay mask
     * Set port mask to Port1/Port2/Port3
     */
    uint16 ptpPortmask[3] = {DVA_RTK_USER_PMAP_1_2_3, DVA_RTK_USER_PMAP_1_2_3, DVA_RTK_USER_PMAP_1_2_3};
    /* Default pdelay threshold is 500ms */
    PtpTs_t pdelayThres = {0u, 0u, 500000000u};
    uint16  latency     = 0u;
    uint16  interval    = 0u;

    PtpRedirectAct_t                      ptpRedirectAct;
    PtpGlenable_t                         ptpGlenable;
    PtpProtocolportmask_t                 ptpProtocolportmask;
    PtpDomainmap_t                        ptpDomainmap;
    PtpSysTimeEn_t                        ptpSysTimeEn;
    PtpHwEnable_t                         ptpHwEnable;
    PtpMasterEn_t                         ptpMasterEn;
    PtpSlave0En_t                         ptpSlave0En;
    PtpSlave1En_t                         ptpSlave1En;
    PtpSlave2En_t                         ptpSlave2En;
    PtpExchangeinfo_t                     ptpExchangeinfo;
    PtpRedirectCpumask_t                  ptpRedirectCpumask;
    PtpPortmode_t                         ptpPortmode;
    PtpPortPtp100Tx_t                     ptpPortPtp100Tx;
    PtpEtherCheckEnable0_t                ptpEtherCheckEnable0;
    PtpPortSlaveTimeDomain_t              PtpPortSlaveTimeDomain;
    PtpPortTimeSyncEn_t                   ptpPortTimeSyncEn;
    PtpInterfaceLatency_t                 ptpInterfaceLatency;
    PtpPortRxlatency_t                    ptpPortRxlatency;
    PtpPortTxlatency_t                    ptpPortTxlatency;
    PtpUsemgtsettableLogpdelayreqintval_t PtpUsemgtsettableLogpdelayreqintval;
    PtpInitlogpdelayintval_t              ptpInitlogpdelayintval;
    PtpLogToIntval_t                      ptpLogToIntval;
    PtpCurntPdelayreqIntval_t             PtpCurntPdelayreqIntval;
    PtpOperlogpdelayintval_t              ptpOperlogpdelayintval;
    PtpPortremappingen_t                  ptpPortremappingen;
    PtpPortremapping_t                    ptpPortremapping;
    PtpUsemgtsettable1Steptxoper_t        PtpUsemgtsettable1Steptxoper;
    PtpOnesteptxsync_t                    ptpOnesteptxsync;
    PtpMgtsettable1Steptxoper_t           PtpMgtsettable1Steptxoper;
    PtpOnesteprxenSw_t                    ptpOnesteprxenSw;
    PtpOnesteptxenSw_t                    ptpOnesteptxenSw;
    PtpInitonesteptxoper_t                ptpInitonesteptxoper;
    PtpSwPortmode_t                       ptpSwPortmode;
    PtpDomainnum_t                        ptpDomainnum;
    PtpInitlogsyncintval_t                ptpInitlogsyncintval;
    PtpCurntsyncintval_t                  ptpCurntsyncintval;
    PtpOperlogsyncintval_t                ptpOperlogsyncintval;
    PtpMgtsettableLogsyncintval_t         PtpMgtsettableLogsyncintval;
    PtpUsemgtsettableLinkdelay_t          PtpUsemgtsettableLinkdelay;
    PtpMgtsettableLinkdelay_t             PtpMgtsettableLinkdelay;
    PtpUsemgtsettableRateratio_t          PtpUsemgtsettableRateratio;
    PtpMgtsettableRateratio_t             PtpMgtsettableRateratio;
    PtpNeigptprxtimerout_t                ptpNeigptprxtimerout;
    PtpDelayAsymmetry_t                   ptpDelayAsymmetry;
    PtpAllowedfaults_t                    ptpAllowedfaults;
    PtpSkipfirstbootrecovery_t            ptpSkipfirstbootrecovery;
    PtpFirstbootuptimetarget_t            ptpFirstbootuptimetarget;
    PtpSilenceifneversynced_t             ptpSilenceifneversynced;
    PtpOverwritePtpid_t                   ptpOverwritePtpid;
    PtpSendPdelayFromMasterport_t         PtpSendPdelayFromMasterport;
    PtpSignalingen_t                      ptpSignalingen;
    PtpUseMgtsettableLogsyncintval_t      PtpUseMgtsettableLogsyncintval;
    PtpSlaveportid_t                      ptpSlaveportid;
    PtpSlaveportmask_t                    ptpSlaveportmask;
    PtpFlashRuntimeAccess_t               ptpFlashRuntimeAccess;
    PtpPdelayWrflashThres_t               ptpPdelayWrflashThres;
    PtpPriority1_t                        ptpPriority1;
    PtpCfggm_t                            ptpCfggm;
    PtpInterServPriEn_t                   ptpInterServPriEn;
    PtpDomainProfiles_t                   ptpDomainProfiles;

    do
    {
        /* Step 1.1 Drop PTP packets and Disable PTP to initial PTP. It is normal behavior if there is PTP packets drop count */
        /* This action is for runtime changing PTP configuration. */
        ptpRedirectAct.ptpRedirAct = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_act(unitChip, &ptpRedirectAct)));
        ptpGlenable.glenable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 1.2 Set protocol port mask. */
        /* port enable mask portmask[0]
         * ascapable mask portmask[1]
         * calDelay mask portmask[2]
         * Set port mask to Port1/Port2/Port3
         */
        ptpProtocolportmask.domainid = domainNum;
        ptpProtocolportmask.portmask = ptpPortmask;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_protocolportmask_set(unitChip, &ptpProtocolportmask)));

        /* Step 1.3 Set domain map. Port1/Port2/Port3 join to domain0 */
        ptpDomainmap.domainid  = domainNum;
        ptpDomainmap.domainmap = DVA_RTK_USER_PMAP_1_2_3;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domainmap(unitChip, &ptpDomainmap)));

        /* Step 1.4 Set global time sync enable. */
        ptpSysTimeEn.sysTimeEn = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_sys_time_en(unitChip, &ptpSysTimeEn)));
        /* Step 1.5 Set HW block enable. */
        ptpHwEnable.ptpEnable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_hw_enable(unitChip, &ptpHwEnable)));
        /* Step 1.6 Set master clock enable. */
        ptpMasterEn.masterEn = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_master_en(unitChip, &ptpMasterEn)));
        /* Step 1.7 Set slave 0 clock enable. */
        ptpSlave0En.slave0En = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slave0_en(unitChip, &ptpSlave0En)));
        /* Step 1.8 Set slave 1 clock enable. */
        ptpSlave1En.slave1En = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slave1_en(unitChip, &ptpSlave1En)));
        /* Step 1.9 Set slave 2 clock enable. */
        ptpSlave2En.slave2En = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slave2_en(unitChip, &ptpSlave2En)));
        /* Step 1.10 Set virtual channel enable to exchange infomation with firmware. */
        ptpExchangeinfo.exchangeInfo = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_exchangeInfo(unitChip, &ptpExchangeinfo)));

        /* Step 1.11 Set PTP packet redirect port to internal port. */
        ptpRedirectCpumask.cpumask = DVA_RTK_CPU_PMAP;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_cpumask(unitChip, &ptpRedirectCpumask)));

        /* Step 1.12 Set internal port config */
        CHK_FUN_CONTINUE((ret), (dva_ptp_set_internal_port_config_ats_tsync()));

        /* Step 1.13 Set user port configs */
        for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
        {
            /* Step 1.13.1 Set all ports, HW port mode to P2P */
            ptpPortmode.port     = port;
            ptpPortmode.portmode = ptpModeInHWReg;
            apiresult            = rtk_ptp_set_portmode(unitChip, &ptpPortmode);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 0);
            }
            /* Step 1.13.2 Set all ports, Setup HW timestamp point enable. */
            ptpPortPtp100Tx.port     = port;
            ptpPortPtp100Tx.ptp100tx = 0u;
            apiresult                = rtk_ptp_set_port_ptp_100tx(unitChip, &ptpPortPtp100Tx);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 1);
            }
            /* Step 1.13.3 Set all ports, Enable RX ethertype filtering */
            ptpEtherCheckEnable0.port      = port;
            ptpEtherCheckEnable0.rtkEnable = 1u;
            apiresult                      = rtk_ptp_set_ether_check_enable0(unitChip, &ptpEtherCheckEnable0);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 2);
            }
            /* Step 1.13.4 Set all ports, Set slave time domain 0. */
            PtpPortSlaveTimeDomain.port        = port;
            PtpPortSlaveTimeDomain.domainid    = 0u;
            PtpPortSlaveTimeDomain.domainvalue = 0u;
            apiresult                          = rtk_ptp_set_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 3);
            }
            /* Step 1.13.5 Set all ports, Set slave time domain 1. */
            PtpPortSlaveTimeDomain.port        = port;
            PtpPortSlaveTimeDomain.domainid    = 1u;
            PtpPortSlaveTimeDomain.domainvalue = 1u;
            apiresult                          = rtk_ptp_set_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 4);
            }
            /* Step 1.13.6 Set all ports, Set slave time domain 2. */
            PtpPortSlaveTimeDomain.port        = port;
            PtpPortSlaveTimeDomain.domainid    = 2u;
            PtpPortSlaveTimeDomain.domainvalue = 2u;
            apiresult                          = rtk_ptp_set_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 5);
            }
            /* Step 1.13.7 Set all ports, Enable time sync. */
            ptpPortTimeSyncEn.port       = port;
            ptpPortTimeSyncEn.timeSyncEn = 1u;
            apiresult                    = rtk_ptp_set_port_time_sync_en(unitChip, &ptpPortTimeSyncEn);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 6);
            }
            /* Step 1.13.8 Set all ports, Initial latency correction for ingress timestamp at MAC. Here use 100BASE T1 as the interface. */
            ptpInterfaceLatency.direction = 0u;
            ptpInterfaceLatency.interface = EM_PORT_TYPE_RTPGE_100BASE_T1;
            ptpInterfaceLatency.latency   = latency;
            apiresult                     = rtk_ptp_get_interface_latency(unitChip, &ptpInterfaceLatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 7);
            }
            ptpPortRxlatency.port      = port;
            ptpPortRxlatency.rxLatency = ptpInterfaceLatency.latency;
            apiresult                  = rtk_ptp_set_port_rxlatency(unitChip, &ptpPortRxlatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 7);
            }
            /* Step 1.13.9 Set all ports, Initial latency correction for egress timestamp at MAC. Here use 100BASE T1 as the interface. */
            ptpInterfaceLatency.direction = 1u;
            ptpInterfaceLatency.interface = EM_PORT_TYPE_RTPGE_100BASE_T1;
            ptpInterfaceLatency.latency   = latency;
            apiresult                     = rtk_ptp_get_interface_latency(unitChip, &ptpInterfaceLatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 8);
            }
            ptpPortTxlatency.port      = port;
            ptpPortTxlatency.txLatency = ptpInterfaceLatency.latency;
            apiresult                  = rtk_ptp_set_port_txlatency(unitChip, &ptpPortTxlatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 8);
            }

            /* Step 1.13.10 Set useMgtSettableLogPdelayReqInterval to false. Pdelay log interval refer to initlogpdelayintval */
            PtpUsemgtsettableLogpdelayreqintval.port   = port;
            PtpUsemgtsettableLogpdelayreqintval.enable = 0u;
            apiresult                                  = rtk_ptp_set_usemgtsettable_logpdelayreqintval(unitChip, &PtpUsemgtsettableLogpdelayreqintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (10u));
            }
            /* Step 1.13.11 Set pdelay request logMessageInterval to 2^0(1s) on domain 0. */
            ptpInitlogpdelayintval.port         = port;
            ptpInitlogpdelayintval.pdelayintval = (int8)0;
            apiresult                           = rtk_ptp_set_initlogpdelayintval(unitChip, &ptpInitlogpdelayintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (11u));
            }
            ptpLogToIntval.loginterval = (int8)0;
            apiresult                  = rtk_ptp_log_to_intval(unitChip, &ptpLogToIntval);
            interval                   = ptpLogToIntval.intval;
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (12u));
            }
            PtpCurntPdelayreqIntval.port                 = port;
            PtpCurntPdelayreqIntval.curntPdelayReqIntval = interval;
            apiresult                                    = rtk_ptp_set_curnt_pdelayreq_intval(unitChip, &PtpCurntPdelayreqIntval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (12u));
            }
            ptpOperlogpdelayintval.port         = port;
            ptpOperlogpdelayintval.pdelayintval = (int8)0;
            apiresult                           = rtk_ptp_set_operlogpdelayintval(unitChip, &ptpOperlogpdelayintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (12u));
            }
            PtpUsemgtsettableLogpdelayreqintval.port   = port;
            PtpUsemgtsettableLogpdelayreqintval.enable = (uint8)0;
            apiresult                                  = rtk_ptp_set_usemgtsettable_logpdelayreqintval(unitChip, &PtpUsemgtsettableLogpdelayreqintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (13u));
            }

            /* Step 1.13.12 Set port remapping enable to match FW port defined. */
            ptpPortremappingen.enable = 1;
            apiresult                 = rtk_ptp_set_en_portremapping(unitChip, &ptpPortremappingen);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (14u));
            }
            ptpPortremapping.port      = port;
            ptpPortremapping.remapping = port;
            apiresult                  = rtk_ptp_set_portremapping(unitChip, &ptpPortremapping);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (14u));
            }
            /* Step 1.13.13 Set useMgtSettableOneStepTxOper to false, 1step tx operability refer to onesteptxsync */
            PtpUsemgtsettable1Steptxoper.port   = port;
            PtpUsemgtsettable1Steptxoper.enable = 0u;
            apiresult                           = rtk_ptp_set_1steptxoper_usemgtsettable(unitChip, &PtpUsemgtsettable1Steptxoper);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 15u);
            }
            /* Step 1.13.14 Set port one step capability to 0, using 2 steps. */
            ptpOnesteptxsync.port          = port;
            ptpOnesteptxsync.oneStepTxSync = 0u;
            apiresult                      = rtk_ptp_set_onesteptxsync(unitChip, &ptpOnesteptxsync);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (16u + domainNum));
            }
            PtpMgtsettable1Steptxoper.port   = port;
            PtpMgtsettable1Steptxoper.domain = domainNum;
            PtpMgtsettable1Steptxoper.txOper = 0u;
            apiresult                        = rtk_ptp_set_1steptxoper_mgtsettable(unitChip, &PtpMgtsettable1Steptxoper);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (17u + domainNum));
            }
            ptpOnesteprxenSw.port          = port;
            ptpOnesteprxenSw.domainid      = domainNum;
            ptpOnesteprxenSw.oneStepRxSync = 0u;
            apiresult                      = rtk_ptp_onesteprxen_sw_set(unitChip, &ptpOnesteprxenSw);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (18u + domainNum));
            }
            ptpOnesteptxenSw.port          = port;
            ptpOnesteptxenSw.domainid      = domainNum;
            ptpOnesteptxenSw.oneStepTxSync = 0u;
            apiresult                      = rtk_ptp_onesteptxen_sw_set(unitChip, &ptpOnesteptxenSw);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (18u + domainNum));
            }
            ptpInitonesteptxoper.port                = port;
            ptpInitonesteptxoper.domainid            = domainNum;
            ptpInitonesteptxoper.initOneStepTXOperEn = 0u;
            apiresult                                = rtk_ptp_set_initonesteptxoper(unitChip, &ptpInitonesteptxoper);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (19u + domainNum));
            }

            /* Step 1.13.15 Set software protocol port mode to P2P for domain 0. */
            ptpSwPortmode.port     = port;
            ptpSwPortmode.domainid = domainNum;
            ptpSwPortmode.portmode = ptpModeinCPUTag;
            apiresult              = rtk_ptp_set_sw_portmode(unitChip, &ptpSwPortmode);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (20u + domainNum));
            }
            /* Step 1.13.16 Set port domain number to 0. */
            ptpDomainnum.port      = port;
            ptpDomainnum.domainid  = domainNum;
            ptpDomainnum.domainnum = (uint8)domainNum;
            apiresult              = rtk_ptp_set_domainnum(unitChip, &ptpDomainnum);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (21u + domainNum));
            }
            /* Step 1.13.17 Set sync logMessageInterval to 2^-3(125ms) on domain 0. */
            ptpInitlogsyncintval.port              = port;
            ptpInitlogsyncintval.domainid          = domainNum;
            ptpInitlogsyncintval.initLogSyncIntval = (int8)-3;
            apiresult                              = rtk_ptp_set_initlogsyncintval(unitChip, &ptpInitlogsyncintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (22u + domainNum));
            }
            ptpLogToIntval.loginterval = (int8)-3;
            apiresult                  = rtk_ptp_log_to_intval(unitChip, &ptpLogToIntval);
            interval                   = ptpLogToIntval.intval;
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (21u + domainNum));
            }
            ptpCurntsyncintval.port            = port;
            ptpCurntsyncintval.domainid        = domainNum;
            ptpCurntsyncintval.curntsyncintval = interval;
            apiresult                          = rtk_ptp_set_curntsyncintval(unitChip, &ptpCurntsyncintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (21u + domainNum));
            }
            ptpOperlogsyncintval.domainid   = domainNum;
            ptpOperlogsyncintval.syncintval = (int8)-3;
            apiresult                       = rtk_ptp_set_operlogsyncintval(unitChip, &ptpOperlogsyncintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (22u + domainNum));
            }
            PtpMgtsettableLogsyncintval.domain    = domainNum;
            PtpMgtsettableLogsyncintval.logIntval = (int8)-3;
            apiresult                             = rtk_ptp_set_logsyncintval_mgtsettable(unitChip, &PtpMgtsettableLogsyncintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (23u + domainNum));
            }
            if(0u != callAPIerrorFlag)
            {
                break;
            }
            /* Step 1.13.18 Set usemgtsettable link delay to false. */
            PtpUsemgtsettableLinkdelay.port   = port;
            PtpUsemgtsettableLinkdelay.enable = 0u;
            apiresult                         = rtk_ptp_set_linkdelay_usemgtsettable(unitChip, &PtpUsemgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 24u);
            }
            /* Step 1.13.19 Set mgtSettableComputeMeanLinkDelay to true. */
            PtpMgtsettableLinkdelay.port   = port;
            PtpMgtsettableLinkdelay.enable = 1u;
            apiresult                      = rtk_ptp_set_linkdelay_mgtsettable(unitChip, &PtpMgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 25);
            }
            /* Step 1.13.20 Set usemgtsettable rateratio to false. */
            PtpUsemgtsettableRateratio.port   = port;
            PtpUsemgtsettableRateratio.enable = 0u;
            apiresult                         = rtk_ptp_set_rateratio_usemgtsettable(unitChip, &PtpUsemgtsettableRateratio);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 26);
            }
            /* Step 1.13.22 Set mgtSettableComputeNeighborRateRatio to true. */
            PtpMgtsettableRateratio.port   = port;
            PtpMgtsettableRateratio.enable = 1u;
            apiresult                      = rtk_ptp_set_rateratio_mgtsettable(unitChip, &PtpMgtsettableRateratio);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 27);
            }
            /* Step 1.13.23 Set neighbor GPTP capable messabe receipt timeout to 9. */
            ptpNeigptprxtimerout.port       = port;
            ptpNeigptprxtimerout.timeoutCnt = 9u;
            apiresult                       = rtk_ptp_set_neigptprxtimerout(unitChip, &ptpNeigptprxtimerout);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 28);
            }
            /* Step 1.13.24 Set delay Asymmetry time to 0. */
            ptpDelayAsymmetry.port           = port;
            ptpDelayAsymmetry.delayAsymmetry = delayAsymTime;
            apiresult                        = rtk_ptp_set_delay_asymmetry(unitChip, &ptpDelayAsymmetry);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 29);
            }
            /* Step 1.13.25 Set allowed faults to 9. */
            ptpAllowedfaults.port          = port;
            ptpAllowedfaults.allowedfaults = 9u;
            apiresult                      = rtk_ptp_set_allowedfaults(unitChip, &ptpAllowedfaults);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 31);
            }
        }
        if(0u != callAPIerrorFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        /* Step 1.14 Disable no recovery after firstbootuptimetarget */
        ptpSkipfirstbootrecovery.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_skipfirstbootrecovery(unitChip, &ptpSkipfirstbootrecovery)));
        ptpFirstbootuptimetarget.timetarget = 3000u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_firstbootuptimetarget(unitChip, &ptpFirstbootuptimetarget)));

        /* Step 1.15 Disable if dut never get sync from slave port, no recovery on domain 0. */
        ptpSilenceifneversynced.domainid = domainNum;
        ptpSilenceifneversynced.enable   = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));
        /* Step 1.16 Enable overwritting clockID with my clockID in sync/follow up. */
        ptpOverwritePtpid.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_overwrite_ptpid_set(unitChip, &ptpOverwritePtpid)));
        /* Step 1.17 Send out Pdelay_req from master port. */
        PtpSendPdelayFromMasterport.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_send_pdelay_from_masterport(unitChip, &PtpSendPdelayFromMasterport)));

        /* Step 1.18 Set signal function on domain 0 to false. */
        ptpSignalingen.domainid = domainNum;
        ptpSignalingen.enable   = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_signalingen(unitChip, &ptpSignalingen)));

        /* Step 1.19 Set Set useMgtSettableLogSyncinterval on domain 0 to false. */
        PtpUseMgtsettableLogsyncintval.domain = domainNum;
        PtpUseMgtsettableLogsyncintval.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_use_mgtsettable_logsyncintval(unitChip, &PtpUseMgtsettableLogsyncintval)));

        /* Step 1.20 Set Profiles to Autosar Time Sync. */
        ptpDomainProfiles.domainid = domainNum;
        ptpDomainProfiles.profiles = EM_PTP_PRF_AUTOSAR;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domain_profiles(unitChip, &ptpDomainProfiles)));
        CHK_FUN_CONTINUE((ret), (dva_ptp_set_autosar_config(slaveIF, DVA_RTK_USER_PMAP_1_2_3, domainNum, pdelayThres)));

        /* Step 1.21 Set slave port to port1 */
        ptpSlaveportid.domainid    = domainNum;
        ptpSlaveportid.slaveportid = slaveIF;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportid(unitChip, &ptpSlaveportid)));

        /* Step 1.22 Set slave port mask. */
        slvPtMask |= ((uint16)1u << slaveIF);
        ptpSlaveportmask.slvMask = slvPtMask;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportmask(unitChip, &ptpSlaveportmask)));

        /* Step 1.23 Disable PTP flash runtime access. */
        ptpFlashRuntimeAccess.flashAccess = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_flash_runtime_access(unitChip, &ptpFlashRuntimeAccess)));
        /* Step 1.24 Set the write flash threshold of pdelay calculating result. */
        ptpPdelayWrflashThres.threshold = 200u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_pdelay_wrflash_thres(unitChip, &ptpPdelayWrflashThres)));

        /* Step 1.25 Set the priority to 248 which refer to Table 8-1—Default values for priority1. */
        ptpPriority1.domainid  = domainNum;
        ptpPriority1.priority1 = 248;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_priority1(unitChip, &ptpPriority1)));
        /* Step 1.26 Set the GM mode to false. */
        ptpCfggm.cfggm = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_cfggm(unitChip, &ptpCfggm)));

        /* Step 1.27 Set sublayer service priority to false. */
        ptpInterServPriEn.priority_en = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_en_inter_serv_pri(unitChip, &ptpInterServPriEn)));

        /* Step 1.28 Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 1.29 Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_reinitprotocol(unitChip)));

        /* Step 1.30 Trap PTP packets to CPU. */
        ptpRedirectAct.ptpRedirAct = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_act(unitChip, &ptpRedirectAct)));

        /* Expected Behaviour: Port 1 is slave port. Others are master port. Connect slave port(port1) to GM.
         * 1. Check clock stable on DUT. clkstable: > 0 means stable. 0 means unstable.
         * - Check it by API rtk_ptp_clkstable_get(0, clkstable).
         * 2. Observe the follow up packet from master port(port2~port11).
         * - subdomainNumber filed is 0.
         * - Correction field is changed.
         * - logMessagePeriod is -3. Sync/Follow_up interval is 125ms.
         * - preciseOriginTimestamp is the same as the follow_up GM sent.
         * - FollowUp message has autosar sub-TLV Time Secured(type:0x28), Status Secured(type:0x50), UserDate Secured(0x60), OFS Secured(0x44).
         *   And OFS Secured sub-TLV has 3 instances.
         * */
    } while(0u == 1u);
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XDVA_PTP_EXAMPLE */
/**@}*/ /* RTL907XDVA_PTP_AUTOSAR_TIME_SYNC_EXAMPLE */