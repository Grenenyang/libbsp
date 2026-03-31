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
 * @defgroup RTL907XDVA_PTP_1588_TRANSPARENT_CLOCK_EXAMPLE PTP 1588 Transparent Clock Example
 * @brief <b>1588 Transparent Clock Configuration</b> \n
 * @section Purpose
 *              This example is to configure switch as a 1588 peer-to-peer transparent clock. \n\n
 * @section Description
 *              It would configure PTP enable, mode, domain ID 0, and joined ports 1 ~ 12. \n
 *              After executing this example, the switch acts as a 1588 tansparent clock forwarding Sync/Follow up message with residence time in correctionField to end point. \n

 * @{
 */

RtkApiRet        dva_ptp_1588_transparent_clock(void);
static RtkApiRet dva_ptp_set_internal_port_config_1588(void);

static RtkApiRet dva_ptp_set_internal_port_config_1588(void)
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

RtkApiRet dva_ptp_1588_transparent_clock(void)
{
    RtkPort    port             = 0u;
    RtkApiRet  ret              = RT_ERR_OK;
    uint64     callAPIerrorFlag = 0u;
    RtkApiRet  apiresult        = RT_ERR_OK;
    UnitChip_t unitChip;
    unitChip.chip = CHIP_907XD_VA;
    unitChip.unit = DVA_UNIT;

    /* Domain: 0 */
    uint8 domainNum = 0u;
    /*  SW port mode to P2P */
    uint8 ptpModeinCPUTag = 3u;
    /* HW port mode to P2P */
    uint8 ptpModeInHWReg = 4u;
    /* portmask[0]: port enable mask
     * portmask[1]: ascapable mask
     * portmask[2]: calDelay mask
     * Set port mask to all ports, port1 to port12.
     */
    uint16 ptpPortmask[3] = {(uint16)DVA_RTK_USER_PMAP_ALL, (uint16)DVA_RTK_USER_PMAP_ALL, (uint16)DVA_RTK_USER_PMAP_ALL};
    /* Default pdelay threshold is 500ms */
    PtpTs_t pdelayThres = {0u, 0u, 500000000u};
    uint16  latency     = 0u;

    PtpRedirectAct_t              ptpRedirectAct;
    PtpGlenable_t                 ptpGlenable;
    PtpProtocolportmask_t         ptpProtocolportmask;
    PtpDomainmap_t                ptpDomainmap;
    PtpSysTimeEn_t                ptpSysTimeEn;
    PtpHwEnable_t                 ptpHwEnable;
    PtpMasterEn_t                 ptpMasterEn;
    PtpSlave0En_t                 ptpSlave0En;
    PtpSlave1En_t                 ptpSlave1En;
    PtpSlave2En_t                 ptpSlave2En;
    PtpExchangeinfo_t             ptpExchangeinfo;
    PtpRedirectCpumask_t          ptpRedirectCpumask;
    PtpPortmode_t                 ptpPortmode;
    PtpPortPtp100Tx_t             ptpPortPtp100Tx;
    PtpEtherCheckEnable0_t        ptpEtherCheckEnable0;
    PtpPortSlaveTimeDomain_t      PtpPortSlaveTimeDomain;
    PtpPortTimeSyncEn_t           ptpPortTimeSyncEn;
    PtpInterfaceLatency_t         ptpInterfaceLatency;
    PtpPortRxlatency_t            ptpPortRxlatency;
    PtpPortTxlatency_t            ptpPortTxlatency;
    PtpSwPortmode_t               ptpSwPortmode;
    PtpDomainnum_t                ptpDomainnum;
    PtpOverwritePtpid_t           ptpOverwritePtpid;
    PtpSendPdelayFromMasterport_t PtpSendPdelayFromMasterport;
    PtpDelaythresh_t              ptpDelaythresh;
    PtpDomainProfiles_t           ptpDomainProfiles;

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
         * Set port mask to all ports, port1 to port12.
         */
        ptpProtocolportmask.domainid = domainNum;
        ptpProtocolportmask.portmask = ptpPortmask;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_protocolportmask_set(unitChip, &ptpProtocolportmask)));

        /* Step 1.3 Set domain map. all ports join to domain0 */
        ptpDomainmap.domainid  = domainNum;
        ptpDomainmap.domainmap = (uint16)DVA_RTK_USER_PMAP_ALL;
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
        CHK_FUN_CONTINUE((ret), (dva_ptp_set_internal_port_config_1588()));

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

            /* Step 1.13.10 Set software protocol port mode to P2P for domain 0. */
            ptpSwPortmode.port     = port;
            ptpSwPortmode.domainid = domainNum;
            ptpSwPortmode.portmode = ptpModeinCPUTag;
            apiresult              = rtk_ptp_set_sw_portmode(unitChip, &ptpSwPortmode);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (20u + domainNum));
            }
            /* Step 1.13.11 Set port domain number to 0. */
            ptpDomainnum.port      = port;
            ptpDomainnum.domainid  = domainNum;
            ptpDomainnum.domainnum = (uint8)domainNum;
            apiresult              = rtk_ptp_set_domainnum(unitChip, &ptpDomainnum);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint64)1u << (21u + domainNum));
            }
        }
        if(0u != callAPIerrorFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        /* Step 1.14 Enable overwritting clockID with my clockID in sync/follow up. */
        ptpOverwritePtpid.enable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_overwrite_ptpid_set(unitChip, &ptpOverwritePtpid)));
        /* Step 1.15 Send out Pdelay_req from master port. */
        PtpSendPdelayFromMasterport.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_send_pdelay_from_masterport(unitChip, &PtpSendPdelayFromMasterport)));

        /* Step 1.16 Set Profiles to 1588 P2P Transparent Clock. */
        ptpDomainProfiles.domainid = domainNum;
        ptpDomainProfiles.profiles = EM_PTP_PRF_1588_P2PTC;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_domain_profiles(unitChip, &ptpDomainProfiles)));

        /* Step 1.17 Set the valid pdelay threshold to 500ms. */
        ptpDelaythresh.delaythresh = pdelayThres;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_delaythresh(unitChip, &ptpDelaythresh)));

        /* Step 1.18 Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 1.19 Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_reinitprotocol(unitChip)));

        /* Step 1.20 Trap PTP packets to CPU. */
        ptpRedirectAct.ptpRedirAct = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_redirect_act(unitChip, &ptpRedirectAct)));

        /* Expected Behaviour:
         * 1. Port1 ~ Port12 are joined port. Switch should do pdelay measurement on all joined ports.
         * 2. Sync/Follow Up should be forwarding to other ports except receiving port. And should have the correctionField value.
         * */
    } while(0u == 1u);
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XDVA_PTP_EXAMPLE */
/**@}*/ /* RTL907XDVA_PTP_1588_TRANSPARENT_CLOCK_EXAMPLE */