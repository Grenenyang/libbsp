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
 * @defgroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup PTP PTP Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_PTP_EXAMPLE RTL907XD_VA PTP Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_PTP_MULTI_PROFILES_EXAMPLE PTP Multi Profiles Example
 * @brief <b>Configure multi-profiles with multi-domain clock</b>
 *
 *  @section Purpose
 *              This example is to configure AS2020 profiles on domain0 and Autosar Time Sync profiles on domain1. \n\n
 *  @section Description
 *              It would configure PTP enable, mode, domain ID 0/1, slave port 1 for 2 domains, and joined ports 1 ~ 3 for 2 domains. \n
 *              After executing this example, the switch act as a time-aware bridge which synchronize time with GM clock and forward Sync/Follow up message to end point. \n
 *              Domain0 will run 802.1 AS2020 behavior, and domain1 will run Autosar Time Sync behavior. \n
 * @{
 */

/* Max Port Number */
extern uint8 dva_g_maxPortNumMulProf;
uint8        dva_g_maxPortNumMulProf = DVA_RTK_USER_PORT_3RD;
/* Slave Port for 3 Domains */
extern uint8 dva_g_slaveIfMulProf[DOMAIN_COUNTS];
uint8        dva_g_slaveIfMulProf[DOMAIN_COUNTS] = {DVA_RTK_USER_PORT_1ST, DVA_RTK_USER_PORT_1ST, DVA_RTK_USER_PORT_1ST};
/* Join Port Mask
 * dva_g_joinPortMaskMulProf[0]: port enable mask
 * dva_g_joinPortMaskMulProf[1]: ascapable mask
 * dva_g_joinPortMaskMulProf[2]: calDelay mask */
extern uint16 dva_g_joinPortMaskMulProf[3];
uint16        dva_g_joinPortMaskMulProf[3] = {DVA_RTK_USER_PMAP_1_2_3, DVA_RTK_USER_PMAP_1_2_3, DVA_RTK_USER_PMAP_1_2_3};
/* Sync Interval */
extern int8 dva_g_logSyncIntervalMulProf;
int8        dva_g_logSyncIntervalMulProf = (int8)-3;
extern int8 dva_g_operLogSyncIntervalMulProf;
int8        dva_g_operLogSyncIntervalMulProf = (int8)-3;
/* Pdelay Interval */
extern int8 dva_g_logPdelayIntervalMulProf;
int8        dva_g_logPdelayIntervalMulProf = (int8)0;
extern int8 dva_g_operLogPdelayIntervalMulProf;
int8        dva_g_operLogPdelayIntervalMulProf = (int8)0;

/* Delay Asymmetry */
extern CFiled_t dva_g_delayAsymTimeMulProf;
CFiled_t        dva_g_delayAsymTimeMulProf = {0u, 0u, 0u};

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet        dva_ptp_multi_profiles(void);
static RtkApiRet dva_ptp_dumpcfg_multi_profiles(void);
static RtkApiRet dva_ptp_set_internal_port_config_multi_profiles(void);

/******************************************************************************
 * Function
 ******************************************************************************/
static RtkApiRet dva_ptp_dumpcfg_multi_profiles(void)
{
    RtkApiRet ret      = RT_ERR_FAILED;
    RtkApiRet printRet = RT_ERR_OK;
    do
    {
        if(NULL != rtlglue_printf)
        {
            printRet = rtlglue_printf("Max Port Num:%u\n", dva_g_maxPortNumMulProf);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("D0 Slave Port:%u, D1 Slave Port:%u\n", dva_g_slaveIfMulProf[0], dva_g_slaveIfMulProf[1]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Port Enable Mask:%x\n", dva_g_joinPortMaskMulProf[0]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("asCapable Mask:%x\n", dva_g_joinPortMaskMulProf[1]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Cal delay Mask:%x\n", dva_g_joinPortMaskMulProf[2]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Log Sync Interval:%d\n", dva_g_logSyncIntervalMulProf);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Oper Log Sync Interval:%d\n", dva_g_operLogSyncIntervalMulProf);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Log Pdelay Interval:%d\n", dva_g_logPdelayIntervalMulProf);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Oper Log Pdelay Interval:%d\n", dva_g_operLogPdelayIntervalMulProf);
            if(printRet < 0)
            {
                continue;
            }

            printRet = rtlglue_printf("Delay Asmmetry Up16 Nano Seconds:%u\n", dva_g_delayAsymTimeMulProf.up16BitNs);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Delay Asmmetry Mid32 Nano Seconds:%u\n", dva_g_delayAsymTimeMulProf.mid32BitNs);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Delay Asmmetry Sub16 Nano Seconds:%u\n", dva_g_delayAsymTimeMulProf.sub16bitNs);
            if(printRet < 0)
            {
                continue;
            }

            ret = RT_ERR_OK;
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_ptp_set_autosar_config_multi_profiles(uint8 slavePort, uint16 joinedPortMask, uint8 domainId, PtpTs_t pdelayThres)
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
    ptpNewQTaginfo                taginfo;
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

static RtkApiRet dva_ptp_set_internal_port_config_multi_profiles(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t unitChip;
    RtkPort    port = DVA_RTK_CPU_PORT;

    unitChip.chip = CHIP_907XD_VA;
    unitChip.unit = DVA_UNIT;

    /* HW port mode to P2P */
    uint8 ptpModeInHWRegInAS2020 = 4u;

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
        ptpPortmode.portmode = ptpModeInHWRegInAS2020;
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
        PtpPortSlaveTimeDomain.domainid    = 2u;
        PtpPortSlaveTimeDomain.domainvalue = 2u;
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

RtkApiRet dva_ptp_multi_profiles(void)
{
    uint32     port             = 0u;
    RtkApiRet  ret              = RT_ERR_OK;
    uint64     callAPIerrorFlag = 0LL;
    RtkApiRet  apiresult        = RT_ERR_OK;
    uint32     domainid         = 0u;
    UnitChip_t unitChip;
    unitChip.chip = CHIP_907XD_VA;
    unitChip.unit = DVA_UNIT;
    /* SW port mode to P2P */
    uint8 ptpModeinCPUTag = 3u;
    /* HW port mode to P2P */
    uint8 ptpModeInHWRegInAS2020 = 4u;
    /* Default pdelay threshold is 500ms */
    PtpTs_t pdelayThres = {0u, 0u, 500000000u};
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
    PtpOperlogpdelayintval_t              ptpOperlogpdelayintval;
    PtpLogToIntval_t                      ptpLogToIntval;
    PtpCurntPdelayreqIntval_t             PtpCurntPdelayreqIntval;
    PtpMgtsettableLogpdelayreqintval_t    PtpMgtsettableLogpdelayreqintval;
    PtpPortremappingen_t                  ptpPortremappingen;
    PtpPortremapping_t                    ptpPortremapping;
    PtpUsemgtsettableGptpmsgintval_t      PtpUsemgtsettableGptpmsgintval;
    PtpSwPortmode_t                       ptpSwPortmode;
    PtpDomainnum_t                        ptpDomainnum;
    PtpInitlogsyncintval_t                ptpInitlogsyncintval;
    PtpCurntsyncintval_t                  ptpCurntsyncintval;
    PtpOperlogsyncintval_t                ptpOperlogsyncintval;
    PtpMgtsettableLogsyncintval_t         PtpMgtsettableLogsyncintval;
    PtpMgtsettableGptpmsglogintval_t      PtpMgtsettableGptpmsglogintval;
    PtpInitGptpmsgintval_t                ptpInitGptpmsgintval;
    PtpUsemgtsettableLinkdelay_t          PtpUsemgtsettableLinkdelay;
    PtpMgtsettableLinkdelay_t             PtpMgtsettableLinkdelay;
    PtpUsemgtsettableRateratio_t          PtpUsemgtsettableRateratio;
    PtpMgtsettableRateratio_t             PtpMgtsettableRateratio;
    PtpNeigptprxtimerout_t                ptpNeigptprxtimerout;
    PtpDelayAsymmetry_t                   ptpDelayAsymmetry;
    PtpAllowedfaults_t                    ptpAllowedfaults;
    PtpOverwritePtpid_t                   ptpOverwritePtpid;
    PtpSendPdelayFromMasterport_t         PtpSendPdelayFromMasterport;
    PtpSignalingen_t                      ptpSignalingen;
    PtpUseMgtsettableLogsyncintval_t      PtpUseMgtsettableLogsyncintval;
    PtpSlaveportid_t                      ptpSlaveportid;
    PtpDelaythresh_t                      ptpDelaythresh;
    PtpPriority1_t                        ptpPriority1;
    PtpCfggm_t                            ptpCfggm;
    PtpInterServPriEn_t                   PtpInterServPriEn;
    PtpDomainProfiles_t                   ptpDomainProfiles;
    do
    {
        CHK_FUN_CONTINUE((ret), (dva_ptp_dumpcfg_multi_profiles()));
        /* Step 1.1 Drop PTP packets and Disable PTP to initial PTP. It is normal behavior if there is PTP packets drop count */
        /* This action is for runtime changing PTP configuration. */
        ptpRedirectAct.ptpRedirAct = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_act(unitChip, &ptpRedirectAct)));
        ptpGlenable.glenable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 1.2 Set protocol port mask for 3 domains. */
        ptpProtocolportmask.domainid = DOMAIN_INDEX1;
        ptpProtocolportmask.portmask = dva_g_joinPortMaskMulProf;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_protocolportmask_set(unitChip, &ptpProtocolportmask)));
        ptpProtocolportmask.domainid = DOMAIN_INDEX2;
        ptpProtocolportmask.portmask = dva_g_joinPortMaskMulProf;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_protocolportmask_set(unitChip, &ptpProtocolportmask)));

        /* Step 1.3 Set domain map. All ports join to domain0, domain1. */
        ptpDomainmap.domainid  = DOMAIN_INDEX1;
        ptpDomainmap.domainmap = dva_g_joinPortMaskMulProf[0];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domainmap(unitChip, &ptpDomainmap)));
        ptpDomainmap.domainid  = DOMAIN_INDEX2;
        ptpDomainmap.domainmap = dva_g_joinPortMaskMulProf[0];
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
        CHK_FUN_CONTINUE((ret), (dva_ptp_set_internal_port_config_multi_profiles()));

        for(port = DVA_RTK_USER_PORT_1ST; port <= dva_g_maxPortNumMulProf; port++)
        {
            /* Step 1.13.1 Set all ports, HW port mode to P2P */
            ptpPortmode.port     = port;
            ptpPortmode.portmode = ptpModeInHWRegInAS2020;
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
            /* Step 1.13.8 Set all ports, Initial latency correction for ingress timestamp at MAC. */
            ptpInterfaceLatency.direction = 0u;
            ptpInterfaceLatency.interface = EM_PORT_TYPE_RTPGE_100BASE_T1;
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
            /* Step 1.13.9 Set all ports, Initial latency correction for egress timestamp at MAC. */
            ptpInterfaceLatency.direction = 1u;
            ptpInterfaceLatency.interface = EM_PORT_TYPE_RTPGE_100BASE_T1;
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

            /* Step 1.13.11 Set useMgtSettableLogPdelayReqInterval to false. pdelayintval refer to initlogpdelayintval. */
            PtpUsemgtsettableLogpdelayreqintval.port   = port;
            PtpUsemgtsettableLogpdelayreqintval.enable = 0u;
            apiresult                                  = rtk_ptp_set_usemgtsettable_logpdelayreqintval(unitChip, &PtpUsemgtsettableLogpdelayreqintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (10u));
            }
            /* Step 1.13.12 Set pdelay request logMessageInterval to 2^0(1s) on all port. */
            ptpInitlogpdelayintval.port         = port;
            ptpInitlogpdelayintval.pdelayintval = (int8)dva_g_logPdelayIntervalMulProf;
            apiresult                           = rtk_ptp_set_initlogpdelayintval(unitChip, &ptpInitlogpdelayintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (11u));
            }
            ptpOperlogpdelayintval.port         = port;
            ptpOperlogpdelayintval.pdelayintval = (int8)dva_g_operLogPdelayIntervalMulProf;
            apiresult                           = rtk_ptp_set_operlogpdelayintval(unitChip, &ptpOperlogpdelayintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (12u));
            }
            ptpLogToIntval.loginterval = dva_g_logPdelayIntervalMulProf;
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
            PtpMgtsettableLogpdelayreqintval.port   = port;
            PtpMgtsettableLogpdelayreqintval.intval = (int8)dva_g_logPdelayIntervalMulProf;
            apiresult                               = rtk_ptp_set_logpdelayreqintval_mgtsettable(unitChip, &PtpMgtsettableLogpdelayreqintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (12u));
            }
            /* Step 1.13.13 Set port remapping enable to match FW port defined. */
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
                callAPIerrorFlag |= ((uint64)1u << (13u));
            }

            /* Step 1.13.14 Set useMgtSettableGptpCapableMsgInterval to false, gptpmsgintval refer to init_gptpmsgintval */
            PtpUsemgtsettableGptpmsgintval.port   = port;
            PtpUsemgtsettableGptpmsgintval.enable = 0u;
            apiresult                             = rtk_ptp_set_gptpmsgintval_usemgtsettable(unitChip, &PtpUsemgtsettableGptpmsgintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 14u);
            }

            for(domainid = DOMAIN_INDEX1; domainid < DOMAIN_COUNTS; domainid++)
            {
                /* Step 1.13.15 Set software protocol port mode to P2P for domain 0. */
                ptpSwPortmode.port     = port;
                ptpSwPortmode.domainid = domainid;
                ptpSwPortmode.portmode = ptpModeinCPUTag;
                apiresult              = rtk_ptp_set_sw_portmode(unitChip, &ptpSwPortmode);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (19u + domainid));
                }
                /* Step 1.13.16 Set port domain number to 0. */
                ptpDomainnum.port      = port;
                ptpDomainnum.domainid  = domainid;
                ptpDomainnum.domainnum = (uint8)domainid;
                apiresult              = rtk_ptp_set_domainnum(unitChip, &ptpDomainnum);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (20u + domainid));
                }
                /* Step 1.13.17 Set sync logMessageInterval to 2^-3(125ms) on all domain. */
                ptpInitlogsyncintval.port              = port;
                ptpInitlogsyncintval.domainid          = domainid;
                ptpInitlogsyncintval.initLogSyncIntval = (int8)dva_g_logSyncIntervalMulProf;
                apiresult                              = rtk_ptp_set_initlogsyncintval(unitChip, &ptpInitlogsyncintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (21u + domainid));
                }
                ptpLogToIntval.loginterval = dva_g_logSyncIntervalMulProf;
                apiresult                  = rtk_ptp_log_to_intval(unitChip, &ptpLogToIntval);
                interval                   = ptpLogToIntval.intval;
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (21u + domainid));
                }
                ptpCurntsyncintval.port            = port;
                ptpCurntsyncintval.domainid        = domainid;
                ptpCurntsyncintval.curntsyncintval = interval;
                apiresult                          = rtk_ptp_set_curntsyncintval(unitChip, &ptpCurntsyncintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (21u + domainid));
                }
                ptpOperlogsyncintval.domainid   = domainid;
                ptpOperlogsyncintval.syncintval = (int8)dva_g_operLogSyncIntervalMulProf;
                apiresult                       = rtk_ptp_set_operlogsyncintval(unitChip, &ptpOperlogsyncintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (22u + domainid));
                }
                PtpMgtsettableLogsyncintval.domain    = domainid;
                PtpMgtsettableLogsyncintval.logIntval = (int8)dva_g_logSyncIntervalMulProf;
                apiresult                             = rtk_ptp_set_logsyncintval_mgtsettable(unitChip, &PtpMgtsettableLogsyncintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (22u + domainid));
                }

                /* Step 1.13.18 Set mgtSettableLogGptpCapableMsgInterval to 2^0(1ms) on all domain . */
                PtpMgtsettableGptpmsglogintval.port   = port;
                PtpMgtsettableGptpmsglogintval.domain = domainid;
                PtpMgtsettableGptpmsglogintval.intval = (int8)0;
                apiresult                             = rtk_ptp_set_gptpmsglogintval_mgtsettable(unitChip, &PtpMgtsettableGptpmsglogintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (23u + domainid));
                }
                ptpInitGptpmsgintval.port   = port;
                ptpInitGptpmsgintval.domain = domainid;
                ptpInitGptpmsgintval.intval = (int8)0;
                apiresult                   = rtk_ptp_set_init_gptpmsgintval(unitChip, &ptpInitGptpmsgintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (24u + domainid));
                }
            }
            /* Step 1.13.19 Set compute link delay refering to mgtSettableComputeMeanLinkDelay. */
            PtpUsemgtsettableLinkdelay.port   = port;
            PtpUsemgtsettableLinkdelay.enable = 1u;
            apiresult                         = rtk_ptp_set_linkdelay_usemgtsettable(unitChip, &PtpUsemgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 25u);
            }
            /* Step 1.13.20 Set mgtSettableComputeMeanLinkDelay to true. */
            PtpMgtsettableLinkdelay.port   = port;
            PtpMgtsettableLinkdelay.enable = 1u;
            apiresult                      = rtk_ptp_set_linkdelay_mgtsettable(unitChip, &PtpMgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 26);
            }
            /* Step 1.13.21 Set compute rate ratio refering to mgtSettableComputeNeighborRateRatio. */
            PtpUsemgtsettableRateratio.port   = port;
            PtpUsemgtsettableRateratio.enable = 1u;
            apiresult                         = rtk_ptp_set_rateratio_usemgtsettable(unitChip, &PtpUsemgtsettableRateratio);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 27);
            }
            /* Step 1.13.22 Set mgtSettableComputeNeighborRateRatio to true. */
            PtpMgtsettableRateratio.port   = port;
            PtpMgtsettableRateratio.enable = 1u;
            apiresult                      = rtk_ptp_set_rateratio_mgtsettable(unitChip, &PtpMgtsettableRateratio);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 28);
            }
            /* Step 1.13.23 Set neighbor GPTP capable messabe receipt timeout to 9. */
            ptpNeigptprxtimerout.port       = port;
            ptpNeigptprxtimerout.timeoutCnt = 9u;
            apiresult                       = rtk_ptp_set_neigptprxtimerout(unitChip, &ptpNeigptprxtimerout);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 29);
            }
            /* Step 1.13.24 Set delay Asymmetry time to 0. */
            ptpDelayAsymmetry.port           = port;
            ptpDelayAsymmetry.delayAsymmetry = dva_g_delayAsymTimeMulProf;
            apiresult                        = rtk_ptp_set_delay_asymmetry(unitChip, &ptpDelayAsymmetry);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 30);
            }
            /* Step 1.13.25 Set allowed faults to 9. */
            ptpAllowedfaults.port          = port;
            ptpAllowedfaults.allowedfaults = 9u;
            apiresult                      = rtk_ptp_set_allowedfaults(unitChip, &ptpAllowedfaults);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 31);
            }

            if(0uLL != callAPIerrorFlag)
            {
                break;
            }
        }
        if(0uLL != callAPIerrorFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 1.14 Enable overwritting clockID with my clockID in sync/follow up. */
        ptpOverwritePtpid.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_overwrite_ptpid_set(unitChip, &ptpOverwritePtpid)));
        /* Step 1.15 Send out Pdelay_req from master port. */
        PtpSendPdelayFromMasterport.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_send_pdelay_from_masterport(unitChip, &PtpSendPdelayFromMasterport)));

        /* Step 1.16 Set signal function domain0 to true. */
        ptpSignalingen.domainid = DOMAIN_INDEX1;
        ptpSignalingen.enable   = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_signalingen(unitChip, &ptpSignalingen)));
        /* Step 1.17 Set signal function domain1 to false. */
        ptpSignalingen.domainid = DOMAIN_INDEX2;
        ptpSignalingen.enable   = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_signalingen(unitChip, &ptpSignalingen)));

        /* Step 1.18 Set useMgtSettableLogSyncinterval on all domains to false. */
        PtpUseMgtsettableLogsyncintval.domain = DOMAIN_INDEX1;
        PtpUseMgtsettableLogsyncintval.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_use_mgtsettable_logsyncintval(unitChip, &PtpUseMgtsettableLogsyncintval)));
        PtpUseMgtsettableLogsyncintval.domain = DOMAIN_INDEX2;
        PtpUseMgtsettableLogsyncintval.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_use_mgtsettable_logsyncintval(unitChip, &PtpUseMgtsettableLogsyncintval)));

        /* Step 1.19 Set domain0 profiles to AS2020. */
        ptpDomainProfiles.domainid = DOMAIN_INDEX1;
        ptpDomainProfiles.profiles = EM_PTP_PRF_AS2020;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domain_profiles(unitChip, &ptpDomainProfiles)));
        /* Step 1.20 Set domain1 profiles to Autosar Time Sync. */
        ptpDomainProfiles.domainid = DOMAIN_INDEX2;
        ptpDomainProfiles.profiles = EM_PTP_PRF_AUTOSAR;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domain_profiles(unitChip, &ptpDomainProfiles)));
        CHK_FUN_CONTINUE((ret), (dva_ptp_set_autosar_config_multi_profiles(dva_g_slaveIfMulProf[DOMAIN_INDEX2], DVA_RTK_USER_PMAP_1_2_3, DOMAIN_INDEX2, pdelayThres)));

        /* Step 1.21 Set slave port on all domains. */
        ptpSlaveportid.domainid    = DOMAIN_INDEX1;
        ptpSlaveportid.slaveportid = dva_g_slaveIfMulProf[DOMAIN_INDEX1];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportid(unitChip, &ptpSlaveportid)));
        ptpSlaveportid.domainid    = DOMAIN_INDEX2;
        ptpSlaveportid.slaveportid = dva_g_slaveIfMulProf[DOMAIN_INDEX2];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportid(unitChip, &ptpSlaveportid)));

        /* Step 1.22 Set the valid pdelay threshold to 500ms. */
        ptpDelaythresh.delaythresh = pdelayThres;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_delaythresh(unitChip, &ptpDelaythresh)));
        /* Step 1.23 Set the priority to 248 which refer to Table 8-1—Default values for priority1. */
        ptpPriority1.domainid  = DOMAIN_INDEX1;
        ptpPriority1.priority1 = 248;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_priority1(unitChip, &ptpPriority1)));
        ptpPriority1.domainid  = DOMAIN_INDEX2;
        ptpPriority1.priority1 = 248;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_priority1(unitChip, &ptpPriority1)));

        /* Step 1.24 Set the GM mode to false. */
        ptpCfggm.cfggm = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_cfggm(unitChip, &ptpCfggm)));
        /* Step 1.25 Set sublayer service priority to false. */
        PtpInterServPriEn.priority_en = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_en_inter_serv_pri(unitChip, &PtpInterServPriEn)));
        /* Step 1.26 Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 1.27 Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_reinitprotocol(unitChip)));

        /* Step 1.28 Trap PTP packets to CPU. */
        ptpRedirectAct.ptpRedirAct = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_act(unitChip, &ptpRedirectAct)));

        /* Expected Behaviour: Port 1 is slave port. Others are master port. Running AS2020 profiles on domain0, and running autosar time sync on domain1.
         * 1. Check packets on domain0 which should follow AS2020 behavior.
         * 2. Check packets on domain1 which should follow Autosar Time Sync behavior.
         * 3. The time synchronization on domain0 slave device and domain1 slave device should be successful.
         * */
    } while(0u == 1u);
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XDVA_PTP_EXAMPLE */
/**@}*/ /* RTL907XDVA_PTP_MULTI_PROFILES_EXAMPLE */