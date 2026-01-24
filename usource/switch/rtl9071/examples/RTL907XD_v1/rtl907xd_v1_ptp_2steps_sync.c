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
 * @defgroup RTL907XDV1_PTP_EXAMPLE RTL907XD_V1 PTP Example
 * @{
 */

/**
 * @defgroup RTL907XDV1_PTP_2steps_EXAMPLE PTP 2steps Example
 * @brief <b>AS2011 2-step sync/follow up</b> \n
 * @section Purpose
 *              This example is to synchronize time by 2-step sync/follow up. \n\n
 * @section Description
 *              It would configure PTP enable, mode, domain ID 0, slave port 1, and joined ports 1 ~ 3. \n
 *              After executing this example, the switch act as a time-aware bridge which synchronize time with GM clock and forward Sync/Follow up message to end point. \n
 *              It will run 802.1 AS2011 behavior.
 * @{
 */

RtkApiRet        dv1_ptp_2steps_sync(uint8 slavePort);
static RtkApiRet dv1_ptp_set_internal_port_config(void);

static RtkApiRet dv1_ptp_set_internal_port_config(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t unitChip;
    uint32     port = DV1_RTK_CPU_PORT;
    /* HW port mode to P2P */
    uint8 ptpModeInHWReg = 4u;

    unitChip.chip = CHIP_907XD_V1;
    unitChip.unit = DV1_UNIT;

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

RtkApiRet dv1_ptp_2steps_sync(uint8 slavePort)
{
    RtkPort    port             = 0u;
    uint16     slvPtMask        = 0u;
    RtkApiRet  ret              = RT_ERR_OK;
    uint64     callAPIerrorFlag = 0u;
    RtkApiRet  apiresult        = RT_ERR_OK;
    UnitChip_t unitChip;
    unitChip.chip = CHIP_907XD_V1;
    unitChip.unit = DV1_UNIT;

    /* Slave port: 1 */
    uint8 slaveIF = slavePort;
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
    uint16 ptpPortmask[3] = {(uint16)DV1_RTK_USER_PMAP_ALL, (uint16)DV1_RTK_USER_PMAP_ALL, (uint16)DV1_RTK_USER_PMAP_ALL};
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
    PtpDelaymeasureMech_t                 ptpDelaymeasureMech;
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
        ptpDomainmap.domainmap = (uint16)DV1_RTK_USER_PMAP_ALL;
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
        ptpRedirectCpumask.cpumask = DV1_RTK_CPU_PMAP;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_cpumask(unitChip, &ptpRedirectCpumask)));

        /* Step 1.12 Set internal port config */
        CHK_FUN_CONTINUE((ret), (dv1_ptp_set_internal_port_config()));

        /* Step 1.13 Set user port configs */
        for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; port++)
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
            /* Step 1.13.8 Set all ports, Initial latency correction for ingress timestamp at MAC. */
            ptpInterfaceLatency.direction = 0u;
            ptpInterfaceLatency.interface = EM_PORT_TYPE_RTPGE_100BASE_T1;
            ptpInterfaceLatency.latency   = latency;
            apiresult                     = rtk_ptp_get_interface_latency(unitChip, &ptpInterfaceLatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 7);
            }
            latency                    = ptpInterfaceLatency.latency;
            ptpPortRxlatency.port      = port;
            ptpPortRxlatency.rxLatency = latency;
            apiresult                  = rtk_ptp_set_port_rxlatency(unitChip, &ptpPortRxlatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 7);
            }
            /* Step 1.13.9 Set all ports, Initial latency correction for egress timestamp at MAC. */
            ptpInterfaceLatency.direction = 1u;
            ptpInterfaceLatency.interface = EM_PORT_TYPE_RTPGE_100BASE_T1;
            ptpInterfaceLatency.latency   = latency;
            apiresult                     = rtk_ptp_get_interface_latency(unitChip, &ptpInterfaceLatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 8);
            }
            latency                    = ptpInterfaceLatency.latency;
            ptpPortTxlatency.port      = port;
            ptpPortTxlatency.txLatency = latency;
            apiresult                  = rtk_ptp_set_port_txlatency(unitChip, &ptpPortTxlatency);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 8);
            }

            /* Step 1.13.10 Set delaymeasure mechanism to P2P. */
            ptpDelaymeasureMech.mechnisam = 2u;
            apiresult                     = rtk_ptp_set_delaymeasure_mech(unitChip, &ptpDelaymeasureMech);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (9u));
            }
            /* Step 1.13.11 Set useMgtSettableLogPdelayReqInterval to false. Pdelay log interval refer to initlogpdelayintval */
            PtpUsemgtsettableLogpdelayreqintval.port   = port;
            PtpUsemgtsettableLogpdelayreqintval.enable = 0u;
            apiresult                                  = rtk_ptp_set_usemgtsettable_logpdelayreqintval(unitChip, &PtpUsemgtsettableLogpdelayreqintval);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (10u));
            }
            /* Step 1.13.12 Set pdelay request logMessageInterval to 2^0(1s) on domain 0. */
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
                callAPIerrorFlag |= ((uint64)1u << (14u));
            }
            /* Step 1.13.14 Set useMgtSettableOneStepTxOper to false, 1step tx operability refer to onesteptxsync */
            PtpUsemgtsettable1Steptxoper.port   = port;
            PtpUsemgtsettable1Steptxoper.enable = 0u;
            apiresult                           = rtk_ptp_set_1steptxoper_usemgtsettable(unitChip, &PtpUsemgtsettable1Steptxoper);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 15u);
            }
            /* Step 1.13.15 Set port one step capability to 0, using 2 steps. */
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

            /* Step 1.13.16 Set software protocol port mode to P2P for domain 0. */
            ptpSwPortmode.port     = port;
            ptpSwPortmode.domainid = domainNum;
            ptpSwPortmode.portmode = ptpModeinCPUTag;
            apiresult              = rtk_ptp_set_sw_portmode(unitChip, &ptpSwPortmode);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (20u + domainNum));
            }
            /* Step 1.13.17 Set port domain number to 0. */
            ptpDomainnum.port      = port;
            ptpDomainnum.domainid  = domainNum;
            ptpDomainnum.domainnum = (uint8)domainNum;
            apiresult              = rtk_ptp_set_domainnum(unitChip, &ptpDomainnum);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (21u + domainNum));
            }
            /* Step 1.13.18 Set sync logMessageInterval to 2^-3(125ms) on domain 0. */
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
            /* Step 1.13.19 Set usemgtsettable link delay to false. */
            PtpUsemgtsettableLinkdelay.port   = port;
            PtpUsemgtsettableLinkdelay.enable = 0u;
            apiresult                         = rtk_ptp_set_linkdelay_usemgtsettable(unitChip, &PtpUsemgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 24u);
            }
            /* Step 1.13.20 Set mgtSettableComputeMeanLinkDelay to true. */
            PtpMgtsettableLinkdelay.port   = port;
            PtpMgtsettableLinkdelay.enable = 1u;
            apiresult                      = rtk_ptp_set_linkdelay_mgtsettable(unitChip, &PtpMgtsettableLinkdelay);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 25);
            }
            /* Step 1.13.21 Set usemgtsettable rateratio to false. */
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

            /* Step 1.13.26 Enable automotive profile */
            ptpAutoasenable.port         = port;
            ptpAutoasenable.autoasenable = 1u;
            apiresult                    = rtk_ptp_set_autoasenable(unitChip, &ptpAutoasenable);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << 32);
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

        /* Step 1.20 Set AS2020 flag to false. */
        ptpAs2020Status.as2020En = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_as2020_status(unitChip, &ptpAs2020Status)));

        /* Step 1.21 Set Synclock config to 0, disabled. */
        ptpSynclockConfig.synclockStatus = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_synclock_config(unitChip, &ptpSynclockConfig)));

        /* Step 1.22 Set Sync slowdown config to false, disabled. */
        ptpSyncslowdownConfig.syncSlowdownStatus = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_syncslowdown_config(unitChip, &ptpSyncslowdownConfig)));

        /* Step 1.23 Set slave port to port1 */
        ptpSlaveportid.domainid    = domainNum;
        ptpSlaveportid.slaveportid = slaveIF;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_slaveportid(unitChip, &ptpSlaveportid)));

        /* Step 1.24 Set slave port mask. */
        slvPtMask |= ((uint16)1u << slaveIF);
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
        ptpPriority1.domainid  = domainNum;
        ptpPriority1.priority1 = 248;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_priority1(unitChip, &ptpPriority1)));
        /* Step 1.29 Set the GM mode to false. */
        ptpCfggm.cfggm = 0u;
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
         * 1. Check clock stable on DUT. clkstable: > 0 means stable. 0 means unstable.
         * - Check it by API rtk_ptp_clkstable_get(0, clkstable).
         * 2. Observe the follow up packet from master port(port2~port11).
         * - subdomainNumber filed is 0.
         * - Correction field is changed.
         * - logMessagePeriod is -3. Sync/Follow_up interval is 125ms.
         * - preciseOriginTimestamp is the same as the follow_up GM sent.
         * */
    } while(0u == 1u);
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XDV1_PTP_EXAMPLE */
/**@}*/ /* RTL907XDV1_PTP_2steps_EXAMPLE */