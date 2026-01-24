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
 * @defgroup PTP PTP Example
 * @{
 */
/**
 * @defgroup RTL907XCVB_PTP_EXAMPLE RTL907XC_VB PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XCVB_PTP_GM_2STEPS_SYNC_EXAMPLE PTP GM 2steps sync Example
 *  @brief <b>AS2020 2-Step Sync/Follow-Up GM Mode</b> \n
 *  @section Purpose
 *          This example code demonstrates the GM mode AS2020 2-step sync/follow-up with a multi-domain clock. \n\n
 *  @section Description
 *          It acts as a GM to send sync/follow-up messages. \n
 *          Basic configuration: Domain: 0, 1, 2; Slave port: 1 for 3 domains; Joined Port: 1 to 3 for 3 domains.
 *  @{
 */

/* Max Port Number */
extern uint8 cvb_g_gmMaxPortNum;
uint8        cvb_g_gmMaxPortNum = CVB_RTK_USER_PORT_3RD;
/* Slave Port for 3 Domains */
extern uint8 cvb_g_gmSlaveIf[DOMAIN_COUNTS];
uint8        cvb_g_gmSlaveIf[DOMAIN_COUNTS] = {CVB_RTK_USER_PORT_1ST, CVB_RTK_USER_PORT_1ST, CVB_RTK_USER_PORT_1ST};
/* Join Port Mask
 * cvb_g_gmJoinPortMask[0]: port enable mask
 * cvb_g_gmJoinPortMask[1]: ascapable mask
 * cvb_g_gmJoinPortMask[2]: calDelay mask */
extern uint16 cvb_g_gmJoinPortMask[3];
uint16        cvb_g_gmJoinPortMask[3] = {CVB_RTK_USER_PMAP_1_2_3, CVB_RTK_USER_PMAP_1_2_3, CVB_RTK_USER_PMAP_1_2_3};
/* Sync Interval */
extern int8 cvb_g_gmLogSyncInterval;
int8        cvb_g_gmLogSyncInterval = (int8)-3;
extern int8 cvb_g_gmOperLogSyncInterval;
int8        cvb_g_gmOperLogSyncInterval = (int8)-3;
/* Pdelay Interval */
extern int8 cvb_g_gmLogPdelayInterval;
int8        cvb_g_gmLogPdelayInterval = (int8)0;
extern int8 cvb_g_gmOperLogPdelayInterval;
int8        cvb_g_gmOperLogPdelayInterval = (int8)0;

/* Feature Enable Options */
extern uint8 cvb_g_gmAutoAS;
uint8        cvb_g_gmAutoAS = 1u;
/* 2: PTP_P2P_MECH, 3: PTP_COMMONP2P_MECH(CMLDS) */
extern uint8 cvb_g_gmCmlds;
uint8        cvb_g_gmCmlds = 3u;
/* Delay Asymmetry */
extern CFiled_t cvb_g_gmDelayAsymTime;
CFiled_t        cvb_g_gmDelayAsymTime = {0u, 0u, 0u};

RtkApiRet        cvb_ptp_gm_2steps_sync_as2020(void);
static RtkApiRet cvb_ptp_dumpcfg_ptp_gm_2steps_sync_as2020(void);
static RtkApiRet cvb_ptp_set_internal_port_config_gm_as2020(void);

static RtkApiRet cvb_ptp_dumpcfg_ptp_gm_2steps_sync_as2020(void)
{
    RtkApiRet ret      = RT_ERR_FAILED;
    RtkApiRet printRet = RT_ERR_OK;
    do
    {
        if(NULL != rtlglue_printf)
        {
            printRet = rtlglue_printf("Max Port Num:%u\n", cvb_g_gmMaxPortNum);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("D0 Slave Port:%u, D1 Slave Port:%u, D2 Slave Port:%u\n", cvb_g_gmSlaveIf[0], cvb_g_gmSlaveIf[1], cvb_g_gmSlaveIf[2]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Port Enable Mask:%x\n", cvb_g_gmJoinPortMask[0]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("asCapable Mask:%x\n", cvb_g_gmJoinPortMask[1]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Cal delay Mask:%x\n", cvb_g_gmJoinPortMask[2]);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Log Sync Interval:%d\n", cvb_g_gmLogSyncInterval);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Oper Log Sync Interval:%d\n", cvb_g_gmOperLogSyncInterval);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Log Pdelay Interval:%d\n", cvb_g_gmLogPdelayInterval);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Oper Log Pdelay Interval:%d\n", cvb_g_gmOperLogPdelayInterval);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Automotive profile enable:%u\n", cvb_g_gmAutoAS);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("CMLDS:%u\n", cvb_g_gmCmlds);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Delay Asmmetry Up16 Nano Seconds:%u\n", cvb_g_gmDelayAsymTime.up16BitNs);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Delay Asmmetry Mid32 Nano Seconds:%u\n", cvb_g_gmDelayAsymTime.mid32BitNs);
            if(printRet < 0)
            {
                continue;
            }
            printRet = rtlglue_printf("Delay Asmmetry Sub16 Nano Seconds:%u\n", cvb_g_gmDelayAsymTime.sub16bitNs);
            if(printRet < 0)
            {
                continue;
            }
            ret = RT_ERR_OK;
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet cvb_ptp_set_internal_port_config_gm_as2020(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t unitChip;
    RtkPort    port = CVB_RTK_CPU_PORT;
    /* HW port mode to P2P */
    uint8 ptpModeInHWRegInAS2020 = 4u;

    unitChip.chip = CHIP_907XC_VB;
    unitChip.unit = CVB_UNIT;

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

RtkApiRet cvb_ptp_gm_2steps_sync_as2020(void)
{
    uint32     port             = 0u;
    uint16     slvPtMask        = 0u;
    RtkApiRet  ret              = RT_ERR_OK;
    uint64     callAPIerrorFlag = 0LL;
    RtkApiRet  apiresult        = RT_ERR_OK;
    uint32     domainid         = 0u;
    UnitChip_t unitChip;
    uint16     interval = 0u;

    /* SW port mode to P2P */
    uint8 ptpModeinCPUTag = 3u;
    /* HW port mode to P2P */
    uint8 ptpModeInHWRegInAS2020 = 4u;
    /* Default pdelay threshold is 500ms */
    PtpTs_t pdelayThres = {0u, 0u, 500000000u};
    uint16  latency     = 0u;

    unitChip.chip = CHIP_907XC_VB;
    unitChip.unit = CVB_UNIT;

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
    PtpDelaymeasureMech_t                 ptpDelaymeasureMech;
    PtpUsemgtsettableLogpdelayreqintval_t PtpUsemgtsettableLogpdelayreqintval;
    PtpInitlogpdelayintval_t              ptpInitlogpdelayintval;
    PtpOperlogpdelayintval_t              ptpOperlogpdelayintval;
    PtpLogToIntval_t                      ptpLogToIntval;
    PtpCurntPdelayreqIntval_t             PtpCurntPdelayreqIntval;
    PtpMgtsettableLogpdelayreqintval_t    PtpMgtsettableLogpdelayreqintval;
    PtpPortremappingen_t                  ptpPortremappingen;
    PtpPortremapping_t                    ptpPortremapping;
    PtpUsemgtsettableGptpmsgintval_t      PtpUsemgtsettableGptpmsgintval;
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
    PtpMgtsettableGptpmsglogintval_t      PtpMgtsettableGptpmsglogintval;
    PtpInitGptpmsgintval_t                ptpInitGptpmsgintval;
    PtpUsemgtsettableLinkdelay_t          PtpUsemgtsettableLinkdelay;
    PtpMgtsettableLinkdelay_t             PtpMgtsettableLinkdelay;
    PtpUsemgtsettableRateratio_t          PtpUsemgtsettableRateratio;
    PtpMgtsettableRateratio_t             PtpMgtsettableRateratio;
    PtpNeigptprxtimerout_t                ptpNeigptprxtimerout;
    PtpDelayAsymmetry_t                   ptpDelayAsymmetry;
    PtpAllowedfaults_t                    ptpAllowedfaults;
    PtpAutoasenable_t                     ptpAutoasenable;
    PtpSkipfirstbootrecovery_t            ptpSkipfirstbootrecovery;
    PtpFirstbootuptimetarget_t            ptpFirstbootuptimetarget;
    PtpSilenceifneversynced_t             ptpSilenceifneversynced;
    PtpOverwritePtpid_t                   ptpOverwritePtpid;
    PtpSendPdelayFromMasterport_t         PtpSendPdelayFromMasterport;
    PtpSignalingen_t                      ptpSignalingen;
    PtpUseMgtsettableLogsyncintval_t      PtpUseMgtsettableLogsyncintval;
    PtpAs2020Status_t                     ptpAs2020Status;
    PtpSynclockConfig_t                   ptpSynclockConfig;
    PtpSyncslowdownConfig_t               ptpSyncslowdownConfig;
    PtpSlaveportid_t                      ptpSlaveportid;
    PtpSlaveportmask_t                    ptpSlaveportmask;
    PtpFlashRuntimeAccess_t               ptpFlashRuntimeAccess;
    PtpPdelayWrflashThres_t               ptpPdelayWrflashThres;
    PtpDelaythresh_t                      ptpDelaythresh;
    PtpPriority1_t                        ptpPriority1;
    PtpCfggm_t                            ptpCfggm;
    PtpInterServPriEn_t                   PtpInterServPriEn;

    do
    {
        CHK_FUN_CONTINUE((ret), (cvb_ptp_dumpcfg_ptp_gm_2steps_sync_as2020()));
        /* Step 1.1 Drop PTP packets and Disable PTP to initial PTP. It is normal behavior if there is PTP packets drop count */
        /* This action is for runtime changing PTP configuration. */
        ptpRedirectAct.ptpRedirAct = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_act(unitChip, &ptpRedirectAct)));
        ptpGlenable.glenable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 1.2 Set protocol port mask for 3 domains. */
        ptpProtocolportmask.domainid = DOMAIN_INDEX1;
        ptpProtocolportmask.portmask = cvb_g_gmJoinPortMask;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_protocolportmask_set(unitChip, &ptpProtocolportmask)));
        ptpProtocolportmask.domainid = DOMAIN_INDEX2;
        ptpProtocolportmask.portmask = cvb_g_gmJoinPortMask;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_protocolportmask_set(unitChip, &ptpProtocolportmask)));
        ptpProtocolportmask.domainid = DOMAIN_INDEX3;
        ptpProtocolportmask.portmask = cvb_g_gmJoinPortMask;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_protocolportmask_set(unitChip, &ptpProtocolportmask)));

        /* Step 1.3 Set domain map. All ports join to domain0, domain1, domain2. */
        ptpDomainmap.domainid  = DOMAIN_INDEX1;
        ptpDomainmap.domainmap = cvb_g_gmJoinPortMask[0];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domainmap(unitChip, &ptpDomainmap)));
        ptpDomainmap.domainid  = DOMAIN_INDEX2;
        ptpDomainmap.domainmap = cvb_g_gmJoinPortMask[0];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domainmap(unitChip, &ptpDomainmap)));
        ptpDomainmap.domainid  = DOMAIN_INDEX3;
        ptpDomainmap.domainmap = cvb_g_gmJoinPortMask[0];
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
        ptpRedirectCpumask.cpumask = CVB_RTK_CPU_PMAP;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_cpumask(unitChip, &ptpRedirectCpumask)));

        /* Step 1.12 Set internal port config */
        CHK_FUN_CONTINUE((ret), (cvb_ptp_set_internal_port_config_gm_as2020()));

        for(port = CVB_RTK_USER_PORT_1ST; port <= cvb_g_gmMaxPortNum; port++)
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
            ptpInterfaceLatency.interface = EM_PORT_TYPE_PURE_100BASE_T1;
            apiresult                     = rtk_ptp_get_interface_latency(unitChip, &ptpInterfaceLatency);
            latency                       = ptpInterfaceLatency.latency;
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 7);
            }
            ptpPortRxlatency.port      = port;
            ptpPortRxlatency.rxLatency = latency;
            apiresult                  = rtk_ptp_set_port_rxlatency(unitChip, &ptpPortRxlatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 7);
            }
            /* Step 1.13.9 Set all ports, Initial latency correction for egress timestamp at MAC. */
            ptpInterfaceLatency.direction = 1u;
            ptpInterfaceLatency.interface = EM_PORT_TYPE_PURE_100BASE_T1;
            apiresult                     = rtk_ptp_get_interface_latency(unitChip, &ptpInterfaceLatency);
            latency                       = ptpInterfaceLatency.latency;
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 8);
            }
            ptpPortTxlatency.port      = port;
            ptpPortTxlatency.txLatency = latency;
            apiresult                  = rtk_ptp_set_port_txlatency(unitChip, &ptpPortTxlatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 8);
            }

            /* Step 1.13.10 Set delaymeasure mechanism to COMMON_P2P. */
            ptpDelaymeasureMech.mechnisam = cvb_g_gmCmlds;
            apiresult                     = rtk_ptp_set_delaymeasure_mech(unitChip, &ptpDelaymeasureMech);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (9u));
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
            ptpInitlogpdelayintval.pdelayintval = (int8)cvb_g_gmLogPdelayInterval;
            apiresult                           = rtk_ptp_set_initlogpdelayintval(unitChip, &ptpInitlogpdelayintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (11u));
            }
            ptpOperlogpdelayintval.port         = port;
            ptpOperlogpdelayintval.pdelayintval = (int8)cvb_g_gmOperLogPdelayInterval;
            apiresult                           = rtk_ptp_set_operlogpdelayintval(unitChip, &ptpOperlogpdelayintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (12u));
            }
            ptpLogToIntval.loginterval = cvb_g_gmLogPdelayInterval;
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
            PtpMgtsettableLogpdelayreqintval.intval = (int8)cvb_g_gmLogPdelayInterval;
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

            /* Step 1.13.15 Set useMgtSettableOneStepTxOper to false, 1step tx operability refer to initOneStepTxOper */
            PtpUsemgtsettable1Steptxoper.port   = port;
            PtpUsemgtsettable1Steptxoper.enable = 0u;
            apiresult                           = rtk_ptp_set_1steptxoper_usemgtsettable(unitChip, &PtpUsemgtsettable1Steptxoper);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 14u);
            }

            /* Step 1.13.16 Set port one step capability to 0, using 2 steps. */
            ptpOnesteptxsync.port          = port;
            ptpOnesteptxsync.oneStepTxSync = 0u;
            apiresult                      = rtk_ptp_set_onesteptxsync(unitChip, &ptpOnesteptxsync);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 15);
            }

            for(domainid = DOMAIN_INDEX1; domainid < DOMAIN_COUNTS; domainid++)
            {
                PtpMgtsettable1Steptxoper.port   = port;
                PtpMgtsettable1Steptxoper.domain = domainid;
                PtpMgtsettable1Steptxoper.txOper = 0u;
                apiresult                        = rtk_ptp_set_1steptxoper_mgtsettable(unitChip, &PtpMgtsettable1Steptxoper);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (16u + domainid));
                }
                ptpOnesteprxenSw.port          = port;
                ptpOnesteprxenSw.domainid      = domainid;
                ptpOnesteprxenSw.oneStepRxSync = 0u;
                apiresult                      = rtk_ptp_onesteprxen_sw_set(unitChip, &ptpOnesteprxenSw);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (17u + domainid));
                }
                ptpOnesteptxenSw.port          = port;
                ptpOnesteptxenSw.domainid      = domainid;
                ptpOnesteptxenSw.oneStepTxSync = 0u;
                apiresult                      = rtk_ptp_onesteptxen_sw_set(unitChip, &ptpOnesteptxenSw);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (18u + domainid));
                }
                ptpInitonesteptxoper.port                = port;
                ptpInitonesteptxoper.domainid            = domainid;
                ptpInitonesteptxoper.initOneStepTXOperEn = 0u;
                apiresult                                = rtk_ptp_set_initonesteptxoper(unitChip, &ptpInitonesteptxoper);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (18u + domainid));
                }

                /* Step 1.13.17 Set software protocol port mode to P2P for domain 0. */
                ptpSwPortmode.port     = port;
                ptpSwPortmode.domainid = domainid;
                ptpSwPortmode.portmode = ptpModeinCPUTag;
                apiresult              = rtk_ptp_set_sw_portmode(unitChip, &ptpSwPortmode);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (19u + domainid));
                }
                /* Step 1.13.18 Set port domain number to 0. */
                ptpDomainnum.port      = port;
                ptpDomainnum.domainid  = domainid;
                ptpDomainnum.domainnum = (uint8)domainid;
                apiresult              = rtk_ptp_set_domainnum(unitChip, &ptpDomainnum);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (20u + domainid));
                }
                /* Step 1.13.19 Set sync logMessageInterval to 2^-3(125ms) on all domain. */
                ptpInitlogsyncintval.port              = port;
                ptpInitlogsyncintval.domainid          = domainid;
                ptpInitlogsyncintval.initLogSyncIntval = (int8)cvb_g_gmLogSyncInterval;
                apiresult                              = rtk_ptp_set_initlogsyncintval(unitChip, &ptpInitlogsyncintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (21u + domainid));
                }
                ptpLogToIntval.loginterval = cvb_g_gmLogSyncInterval;
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
                ptpOperlogsyncintval.syncintval = (int8)cvb_g_gmOperLogSyncInterval;
                apiresult                       = rtk_ptp_set_operlogsyncintval(unitChip, &ptpOperlogsyncintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (22u + domainid));
                }
                PtpMgtsettableLogsyncintval.domain    = domainid;
                PtpMgtsettableLogsyncintval.logIntval = (int8)cvb_g_gmLogSyncInterval;
                apiresult                             = rtk_ptp_set_logsyncintval_mgtsettable(unitChip, &PtpMgtsettableLogsyncintval);
                if(RT_ERR_OK != apiresult)
                {
                    callAPIerrorFlag |= ((uint64)1u << (22u + domainid));
                }

                /* Step 1.13.20 Set mgtSettableLogGptpCapableMsgInterval to 2^0(1ms) on all domain . */
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
            /* Step 1.13.21 Set compute link delay refering to mgtSettableComputeMeanLinkDelay. */
            PtpUsemgtsettableLinkdelay.port   = port;
            PtpUsemgtsettableLinkdelay.enable = 1u;
            apiresult                         = rtk_ptp_set_linkdelay_usemgtsettable(unitChip, &PtpUsemgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 25u);
            }
            /* Step 1.13.22 Set mgtSettableComputeMeanLinkDelay to true. */
            PtpMgtsettableLinkdelay.port   = port;
            PtpMgtsettableLinkdelay.enable = 1u;
            apiresult                      = rtk_ptp_set_linkdelay_mgtsettable(unitChip, &PtpMgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 26);
            }
            /* Step 1.13.23 Set compute rate ratio refering to mgtSettableComputeNeighborRateRatio. */
            PtpUsemgtsettableRateratio.port   = port;
            PtpUsemgtsettableRateratio.enable = 1u;
            apiresult                         = rtk_ptp_set_rateratio_usemgtsettable(unitChip, &PtpUsemgtsettableRateratio);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 27);
            }
            /* Step 1.13.24 Set mgtSettableComputeNeighborRateRatio to true. */
            PtpMgtsettableRateratio.port   = port;
            PtpMgtsettableRateratio.enable = 1u;
            apiresult                      = rtk_ptp_set_rateratio_mgtsettable(unitChip, &PtpMgtsettableRateratio);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 28);
            }
            /* Step 1.13.25 Set neighbor GPTP capable messabe receipt timeout to 9. */
            ptpNeigptprxtimerout.port       = port;
            ptpNeigptprxtimerout.timeoutCnt = 9u;
            apiresult                       = rtk_ptp_set_neigptprxtimerout(unitChip, &ptpNeigptprxtimerout);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 29);
            }
            /* Step 1.13.26 Set delay Asymmetry time to 0. */
            ptpDelayAsymmetry.port           = port;
            ptpDelayAsymmetry.delayAsymmetry = cvb_g_gmDelayAsymTime;
            apiresult                        = rtk_ptp_set_delay_asymmetry(unitChip, &ptpDelayAsymmetry);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 30);
            }
            /* Step 1.13.27 Set allowed faults to 9. */
            ptpAllowedfaults.port          = port;
            ptpAllowedfaults.allowedfaults = 9u;
            apiresult                      = rtk_ptp_set_allowedfaults(unitChip, &ptpAllowedfaults);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 31);
            }

            /* Step 1.13.28 Enable automotive profile */
            ptpAutoasenable.port         = port;
            ptpAutoasenable.autoasenable = cvb_g_gmAutoAS;
            apiresult                    = rtk_ptp_set_autoasenable(unitChip, &ptpAutoasenable);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 32);
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
        /* Step 1.14 Disable no recovery after firstbootuptimetarget */
        ptpSkipfirstbootrecovery.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_skipfirstbootrecovery(unitChip, &ptpSkipfirstbootrecovery)));
        ptpFirstbootuptimetarget.timetarget = 3000u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_firstbootuptimetarget(unitChip, &ptpFirstbootuptimetarget)));
        /* Step 1.15 Disable if dut never get sync from slave port, no recovery on domain 0, 1, 2. */
        ptpSilenceifneversynced.domainid = DOMAIN_INDEX1;
        ptpSilenceifneversynced.enable   = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));
        ptpSilenceifneversynced.domainid = DOMAIN_INDEX2;
        ptpSilenceifneversynced.enable   = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));
        ptpSilenceifneversynced.domainid = DOMAIN_INDEX3;
        ptpSilenceifneversynced.enable   = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));

        /* Step 1.16 Enable overwritting clockID with my clockID in sync/follow up. */
        ptpOverwritePtpid.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_overwrite_ptpid_set(unitChip, &ptpOverwritePtpid)));
        /* Step 1.17 Send out Pdelay_req from master port. */
        PtpSendPdelayFromMasterport.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_send_pdelay_from_masterport(unitChip, &PtpSendPdelayFromMasterport)));

        /* Step 1.18 Set signal function on all domains to true. */
        ptpSignalingen.domainid = DOMAIN_INDEX1;
        ptpSignalingen.enable   = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_signalingen(unitChip, &ptpSignalingen)));
        ptpSignalingen.domainid = DOMAIN_INDEX2;
        ptpSignalingen.enable   = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_signalingen(unitChip, &ptpSignalingen)));
        ptpSignalingen.domainid = DOMAIN_INDEX3;
        ptpSignalingen.enable   = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_signalingen(unitChip, &ptpSignalingen)));

        /* Step 1.19 Set useMgtSettableLogSyncinterval on all domains to false. */
        PtpUseMgtsettableLogsyncintval.domain = DOMAIN_INDEX1;
        PtpUseMgtsettableLogsyncintval.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_use_mgtsettable_logsyncintval(unitChip, &PtpUseMgtsettableLogsyncintval)));
        PtpUseMgtsettableLogsyncintval.domain = DOMAIN_INDEX2;
        PtpUseMgtsettableLogsyncintval.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_use_mgtsettable_logsyncintval(unitChip, &PtpUseMgtsettableLogsyncintval)));
        PtpUseMgtsettableLogsyncintval.domain = DOMAIN_INDEX3;
        PtpUseMgtsettableLogsyncintval.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_use_mgtsettable_logsyncintval(unitChip, &PtpUseMgtsettableLogsyncintval)));

        /* Step 1.20 Set AS2020 flag to true. */
        ptpAs2020Status.as2020En = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_as2020_status(unitChip, &ptpAs2020Status)));

        /* Step 1.21 Set Synclock config to 1, support AS2020 synclock. */
        ptpSynclockConfig.synclockStatus = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_synclock_config(unitChip, &ptpSynclockConfig)));

        /* Step 1.22 Set Sync slowdown config to true, support AS2020 sync slowdown. */
        ptpSyncslowdownConfig.syncSlowdownStatus = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_syncslowdown_config(unitChip, &ptpSyncslowdownConfig)));

        /* Step 1.23 Set slave port on all domains. */
        ptpSlaveportid.domainid    = DOMAIN_INDEX1;
        ptpSlaveportid.slaveportid = cvb_g_gmSlaveIf[DOMAIN_INDEX1];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportid(unitChip, &ptpSlaveportid)));
        ptpSlaveportid.domainid    = DOMAIN_INDEX2;
        ptpSlaveportid.slaveportid = cvb_g_gmSlaveIf[DOMAIN_INDEX2];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportid(unitChip, &ptpSlaveportid)));
        ptpSlaveportid.domainid    = DOMAIN_INDEX3;
        ptpSlaveportid.slaveportid = cvb_g_gmSlaveIf[DOMAIN_INDEX3];
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportid(unitChip, &ptpSlaveportid)));

        /* Step 1.24 Set slave port mask. */
        slvPtMask |= ((uint16)1u << cvb_g_gmSlaveIf[DOMAIN_INDEX1]) | ((uint16)1u << cvb_g_gmSlaveIf[DOMAIN_INDEX2]) | ((uint16)1u << cvb_g_gmSlaveIf[DOMAIN_INDEX3]);
        ptpSlaveportmask.slvMask = slvPtMask;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportmask(unitChip, &ptpSlaveportmask)));

        /* Step 1.25 Disable PTP flash runtime access. */
        ptpFlashRuntimeAccess.flashAccess = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_flash_runtime_access(unitChip, &ptpFlashRuntimeAccess)));
        /* Step 1.26 Set the write flash threshold of pdelay calculating result. */
        ptpPdelayWrflashThres.threshold = 200u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_pdelay_wrflash_thres(unitChip, &ptpPdelayWrflashThres)));
        /* Step 1.27 Set the valid pdelay threshold to 500ms. */
        ptpDelaythresh.delaythresh = pdelayThres;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_delaythresh(unitChip, &ptpDelaythresh)));
        /* Step 1.28 Set the priority to 248 which refer to Table 8-1—Default values for priority1. */
        ptpPriority1.domainid  = DOMAIN_INDEX1;
        ptpPriority1.priority1 = 248;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_priority1(unitChip, &ptpPriority1)));
        ptpPriority1.domainid  = DOMAIN_INDEX2;
        ptpPriority1.priority1 = 248;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_priority1(unitChip, &ptpPriority1)));
        ptpPriority1.domainid  = DOMAIN_INDEX3;
        ptpPriority1.priority1 = 248;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_priority1(unitChip, &ptpPriority1)));
        /* Step 1.29 Set the GM mode to true. */
        ptpCfggm.cfggm = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_cfggm(unitChip, &ptpCfggm)));
        /* Step 1.30 Set sublayer service priority to false. */
        PtpInterServPriEn.priority_en = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_en_inter_serv_pri(unitChip, &PtpInterServPriEn)));
        /* Step 1.31 Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 1.32 Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_reinitprotocol(unitChip)));

        /* Step 1.33 Trap PTP packets to CPU. */
        ptpRedirectAct.ptpRedirAct = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_act(unitChip, &ptpRedirectAct)));

        /* Expected Behaviour: Port 1 is slave port. Others are master port. Connect slave port(port1) to GM.
         * 1. Check clock stable on all domain DUT. clkstable: > 0 means stable. 0 means unstable.
         * - Check it by API rtk_ptp_clkstable_get(domainId, clkstable).
         * 2. Observe the follow up packet from master port(port2~port11).
         * - subdomainNumber filed is 0, 1, 2.
         * - Correction field is changed.
         * - logMessagePeriod is -3. Sync/Follow_up interval is 125ms.
         * - preciseOriginTimestamp is the same as the follow_up GM sent.
         * */
    } while(0u == 1u);
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XCVB_PTP_EXAMPLE */
/**@}*/ /* RTL907XCVB_PTP_GM_2STEPS_SYNC_EXAMPLE */