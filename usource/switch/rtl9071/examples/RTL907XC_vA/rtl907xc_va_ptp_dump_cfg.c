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
 * @defgroup RTL907XCVA_PTP_EXAMPLE RTL907XC_VA PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XCVA_PTPDUMP_CFG_EXAMPLE PTP dump configuration Example
 *  @brief <b>Dump PTP Configurations</b> \n
 *  @section Purpose
 *          Dump PTP configurations \n\n
 *  @section Description
 *          There are many PTP configurations, this example code integrating PTP configuration get functions. \n
 *  @{
 */

RtkApiRet cva_ptp_get_usemgtsettable_config(UnitChip_t unitChip);
RtkApiRet cva_ptp_other_enable_result(UnitChip_t unitChip);
RtkApiRet cva_ptp_dump_cfg(void);

RtkApiRet cva_ptp_get_usemgtsettable_config(UnitChip_t unitChip)
{
    uint32    domainid = 0u;
    uint32    port     = 0u;
    uint8     u8temp;
    RtkApiRet ret      = RT_ERR_OK;
    RtkApiRet retAPI   = RT_ERR_OK;
    RtkApiRet retPrint = RT_ERR_OK;

    PtpUseMgtsettableLogsyncintval_t      PtpUseMgtsettableLogsyncintval;
    PtpUsemgtsettableLogpdelayreqintval_t PtpUsemgtsettableLogpdelayreqintval;
    PtpUsemgtsettableRateratio_t          PtpUsemgtsettableRateratio;
    PtpMgtsettableRateratio_t             PtpMgtsettableRateratio;
    PtpUsemgtsettable1Steptxoper_t        PtpUsemgtsettable1Steptxoper;
    PtpUsemgtsettableGptpmsgintval_t      PtpUsemgtsettableGptpmsgintval;
    PtpUsemgtsettableLinkdelay_t          PtpUsemgtsettableLinkdelay;
    PtpMgtsettableLinkdelay_t             PtpMgtsettableLinkdelay;

    do
    {
        for(domainid = 0u; domainid < 3u; ++domainid)
        {
            /* Step 57.1. Get useMgtSettableLogSyncInterval configs. */
            PtpUseMgtsettableLogsyncintval.domain = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_use_mgtsettable_logsyncintval(unitChip, &PtpUseMgtsettableLogsyncintval)));
            u8temp = PtpUseMgtsettableLogsyncintval.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("use_mgtsettable_logsyncintval (domain%u): " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, domainid, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("usemgtsettable_logpdelayreqintval:" PTPMSGCOLOR "\r\n" ANSI_COLOR_RESET)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 57.2. Get useMgtSettableLogPdelayReqInterval configs. */
            PtpUsemgtsettableLogpdelayreqintval.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_usemgtsettable_logpdelayreqintval(unitChip, &PtpUsemgtsettableLogpdelayreqintval)));
            u8temp = PtpUsemgtsettableLogpdelayreqintval.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port%u:" PTPMSGCOLOR "%u " ANSI_COLOR_RESET, port, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("usemgtsettable_rateratio:" PTPMSGCOLOR "\r\n" ANSI_COLOR_RESET)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 57.3. Get useMgtSettableRateRatio configs. */
            PtpUsemgtsettableRateratio.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_rateratio_usemgtsettable(unitChip, &PtpUsemgtsettableRateratio)));
            u8temp = PtpUsemgtsettableRateratio.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port%u:" PTPMSGCOLOR "%u " ANSI_COLOR_RESET, port, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("mgtsettable_rateratio:" PTPMSGCOLOR "\r\n" ANSI_COLOR_RESET)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 57.4. Get mgtSettableRateRatio configs. */
            PtpMgtsettableRateratio.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_rateratio_mgtsettable(unitChip, &PtpMgtsettableRateratio)));
            u8temp = PtpMgtsettableRateratio.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port%u:" PTPMSGCOLOR "%u " ANSI_COLOR_RESET, port, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("usemgtsettable_1steptxoper:" PTPMSGCOLOR "\r\n" ANSI_COLOR_RESET)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 57.5. Get useMgtSettableOneStepTxOper configs. */
            PtpUsemgtsettable1Steptxoper.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_1steptxoper_usemgtsettable(unitChip, &PtpUsemgtsettable1Steptxoper)));
            u8temp = PtpUsemgtsettable1Steptxoper.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port%u:" PTPMSGCOLOR "%u " ANSI_COLOR_RESET, port, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("usemgtsettable_gptpmsgintval:" PTPMSGCOLOR "\r\n" ANSI_COLOR_RESET)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 57.6. Get useMgtSettableLogGptpCapableMessageInterval configs. */
            PtpUsemgtsettableGptpmsgintval.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_gptpmsgintval_usemgtsettable(unitChip, &PtpUsemgtsettableGptpmsgintval)));
            u8temp = PtpUsemgtsettableGptpmsgintval.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port%u:" PTPMSGCOLOR "%u " ANSI_COLOR_RESET, port, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("usemgtsettable_linkdelay:" PTPMSGCOLOR "\r\n" ANSI_COLOR_RESET)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 57.7. Get useMgtSettableLinkDelay configs. */
            PtpUsemgtsettableLinkdelay.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_linkdelay_usemgtsettable(unitChip, &PtpUsemgtsettableLinkdelay)));
            u8temp = PtpUsemgtsettableLinkdelay.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port%u:" PTPMSGCOLOR "%u " ANSI_COLOR_RESET, port, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("mgtsettable_linkdelay:" PTPMSGCOLOR "\r\n" ANSI_COLOR_RESET)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 57.8. Get mgtSettableLinkDelay configs. */
            PtpMgtsettableLinkdelay.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_linkdelay_mgtsettable(unitChip, &PtpMgtsettableLinkdelay)));
            u8temp = PtpMgtsettableLinkdelay.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port%u:" PTPMSGCOLOR "%u " ANSI_COLOR_RESET, port, u8temp)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_ptp_other_enable_result(UnitChip_t unitChip)
{
    uint32    port = 0u;
    uint8     u8tmp;
    uint16    u16tmp     = 0u;
    RtkApiRet ret        = RT_ERR_OK;
    RtkApiRet retAPI     = RT_ERR_OK;
    uint32    retAPIFlag = 0u;
    RtkApiRet retPrint   = RT_ERR_OK;

    PtpSysTimeEn_t           ptpSysTimeEn;
    PtpHwEnable_t            ptpHwEnable;
    PtpMasterEn_t            ptpMasterEn;
    PtpSlave0En_t            ptpSlave0En;
    PtpSlave1En_t            ptpSlave1En;
    PtpSlave2En_t            ptpSlave2En;
    PtpExchangeinfo_t        ptpExchangeinfo;
    PtpRedirectCpumask_t     ptpRedirectCpumask;
    PtpRedirectAct_t         ptpRedirectAct;
    PtpPortmode_t            ptpPortmode;
    PtpPortPtp100Tx_t        ptpPortPtp100Tx;
    PtpEtherCheckEnable0_t   ptpEtherCheckEnable0;
    PtpPortSlaveTimeDomain_t PtpPortSlaveTimeDomain;
    PtpPortTimeSyncEn_t      ptpPortTimeSyncEn;
    PtpPortRxlatency_t       ptpPortRxlatency;
    PtpPortTxlatency_t       ptpPortTxlatency;

    do
    {
        /* Step 28.1. Get system time enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_sys_time_en(unitChip, &ptpSysTimeEn)));
        u8tmp = ptpSysTimeEn.sysTimeEn;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_sys_time_en: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));
        /* Step 28.2. Get PTP hardware enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_hw_enable(unitChip, &ptpHwEnable)));
        u8tmp = ptpHwEnable.ptpEnable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_hw_enable: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));
        /* Step 28.3. Get PTP master clock enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_master_en(unitChip, &ptpMasterEn)));
        u8tmp = ptpMasterEn.masterEn;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_master_en: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));
        /* Step 28.4. Get PTP slave0 clock enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_slave0_en(unitChip, &ptpSlave0En)));
        u8tmp = ptpSlave0En.slave0En;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_slave0_en: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));
        /* Step 28.5. Get PTP slave1 clock enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_slave1_en(unitChip, &ptpSlave1En)));
        u8tmp = ptpSlave1En.slave1En;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_slave1_en: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));
        /* Step 28.6. Get PTP slave2 clock enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_slave2_en(unitChip, &ptpSlave2En)));
        u8tmp = ptpSlave2En.slave2En;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_slave2_en: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));
        /* Step 28.7. Get virtual channel status to exchange information with firmware. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_exchangeInfo(unitChip, &ptpExchangeinfo)));
        u8tmp = ptpExchangeinfo.exchangeInfo;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_exchangeInfo: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));
        /* Step 28.9. Get PTP packet redirect port mask. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_redirect_cpumask(unitChip, &ptpRedirectCpumask)));
        u16tmp = ptpRedirectCpumask.cpumask;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_redirect_cpumask: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u16tmp)));
        /* Step 28.10. Get action of redirect PTP packet. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_redirect_act(unitChip, &ptpRedirectAct)));
        u8tmp = ptpRedirectAct.ptpRedirAct;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_redirect_act: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, u8tmp)));

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 28.11. Get hardware port mode config for user port. */
            ptpPortmode.port = port;
            retAPI           = rtk_ptp_get_portmode(unitChip, &ptpPortmode);
            u8tmp            = ptpPortmode.portmode;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 0;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_portmode: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
            }
            /* Step 28.12. Get latch time from rxdv/txen(MII) for user port. */
            ptpPortPtp100Tx.port = port;
            retAPI               = rtk_ptp_get_port_ptp_100tx(unitChip, &ptpPortPtp100Tx);
            u8tmp                = ptpPortPtp100Tx.ptp100tx;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 1;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_ptp_100tx: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
            }
            /* Step 28.13. Get hardware ethertype check for user port. */
            ptpEtherCheckEnable0.port = port;
            retAPI                    = rtk_ptp_get_ether_check_enable0(unitChip, &ptpEtherCheckEnable0);
            u8tmp                     = ptpEtherCheckEnable0.rtkEnable;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 2;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_ether_check_enable0: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
            }
            /* Step 28.14. Get hardware slave0 time domain value for user port. */

            PtpPortSlaveTimeDomain.port     = port;
            PtpPortSlaveTimeDomain.domainid = 0u;
            retAPI                          = rtk_ptp_get_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain);
            u8tmp                           = PtpPortSlaveTimeDomain.domainvalue;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 3;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_slave_time_domain: Port %u domain 0 " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
            }
            /* Step 28.15. Get hardware slave1 time domain value for user port. */
            PtpPortSlaveTimeDomain.port     = port;
            PtpPortSlaveTimeDomain.domainid = 1u;
            retAPI                          = rtk_ptp_get_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain);
            u8tmp                           = PtpPortSlaveTimeDomain.domainvalue;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 4;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_slave_time_domain: Port %u domain 1 " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
            }
            /* Step 28.16. Get hardware slave2 time domain value for user port. */
            PtpPortSlaveTimeDomain.port     = port;
            PtpPortSlaveTimeDomain.domainid = 2u;
            retAPI                          = rtk_ptp_get_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain);
            u8tmp                           = PtpPortSlaveTimeDomain.domainvalue;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 5;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_slave_time_domain: Port %u domain 2 " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
            }
            /* Step 28.17. Get hardware time sync enable status for user port. */
            ptpPortTimeSyncEn.port = port;
            retAPI                 = rtk_ptp_get_port_time_sync_en(unitChip, &ptpPortTimeSyncEn);
            u8tmp                  = ptpPortTimeSyncEn.timeSyncEn;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 6;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_time_sync_en: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
            }
            /* Step 28.18. Get hardware ingress latency for user port. */
            ptpPortRxlatency.port = port;
            retAPI                = rtk_ptp_get_port_rxlatency(unitChip, &ptpPortRxlatency);
            u16tmp                = ptpPortRxlatency.rxLatency;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 7;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_rxlatency: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u16tmp)));
            }
            /* Step 28.19. Get hardware egress latency for user port. */
            ptpPortTxlatency.port = port;
            retAPI                = rtk_ptp_get_port_txlatency(unitChip, &ptpPortTxlatency);
            u16tmp                = ptpPortTxlatency.txLatency;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= (uint32)1u << 8;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_txlatency: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u16tmp)));
            }

            if(0u != retAPIFlag)
            {
                retAPI = -RT_ERR_FAILED;
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        port = CVA_RTK_CPU_PORT;
        /* Step 28.20. Get hardware port mode config for CPU port. */
        ptpPortmode.port = port;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_portmode(unitChip, &ptpPortmode)));
        u8tmp = ptpPortmode.portmode;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_portmode: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
        /* Step 28.21. Get latch time from rxdv/txen(MII) for CPU port. */
        ptpPortPtp100Tx.port = port;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_port_ptp_100tx(unitChip, &ptpPortPtp100Tx)));
        u8tmp = ptpPortPtp100Tx.ptp100tx;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_ptp_100tx: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
        /* Step 28.22. Get hardware ethertype check for CPU port. */
        ptpEtherCheckEnable0.port = port;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_ether_check_enable0(unitChip, &ptpEtherCheckEnable0)));
        u8tmp = ptpEtherCheckEnable0.rtkEnable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_ether_check_enable0: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
        /* Step 28.23. Get hardware slave0 time domain value for CPU port. */
        PtpPortSlaveTimeDomain.port     = port;
        PtpPortSlaveTimeDomain.domainid = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain)));
        u8tmp = PtpPortSlaveTimeDomain.domainvalue;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_slave_time_domain: Port %u domain 0 " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
        /* Step 28.24. Get hardware slave1 time domain value for CPU port. */
        PtpPortSlaveTimeDomain.port     = port;
        PtpPortSlaveTimeDomain.domainid = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain)));
        u8tmp = PtpPortSlaveTimeDomain.domainvalue;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_slave_time_domain: Port %u domain 1 " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
        /* Step 28.25. Get hardware slave2 time domain value for CPU port. */
        PtpPortSlaveTimeDomain.port     = port;
        PtpPortSlaveTimeDomain.domainid = 2u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_port_slave_time_domain(unitChip, &PtpPortSlaveTimeDomain)));
        u8tmp = PtpPortSlaveTimeDomain.domainvalue;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_slave_time_domain: Port %u domain 2 " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
        /* Step 28.26. Get hardware time sync enable status for CPU port. */
        ptpPortTimeSyncEn.port = port;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_port_time_sync_en(unitChip, &ptpPortTimeSyncEn)));
        u8tmp = ptpPortTimeSyncEn.timeSyncEn;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_time_sync_en: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u8tmp)));
        /* Step 28.27. Get hardware ingress latency for CPU port. */
        ptpPortRxlatency.port = port;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_port_rxlatency(unitChip, &ptpPortRxlatency)));
        u16tmp = ptpPortRxlatency.rxLatency;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_rxlatency: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u16tmp)));
        /* Step 28.28. Get hardware egress latency for CPU port. */
        ptpPortTxlatency.port = port;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_port_txlatency(unitChip, &ptpPortTxlatency)));
        u16tmp = ptpPortTxlatency.txLatency;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("rtk_ptp_get_port_txlatency: Port %u " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, port, u16tmp)));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_ptp_dump_cfg(void)
{
    UnitChip_t  unitChip;
    uint8       d8 = 0;
    int8        sd8;
    uint16      d16 = 0;
    uint32      port, domainid;
    PtpTs_t     ts;
    CFiled_t    cf;
    ptpqTaginfo vtag;
    uint8       da[6];
    uint16      ethertype  = 0u;
    uint32      d32        = 0;
    RtkApiRet   ret        = RT_ERR_OK;
    RtkApiRet   retAPI     = RT_ERR_OK;
    uint32      retAPIFlag = 0u;
    RtkApiRet   retPrint   = RT_ERR_OK;

    unitChip.chip = CHIP_907XC_VA;
    unitChip.unit = CVA_UNIT;

    PtpGlenable_t                      ptpGlenable;
    PtpCfggm_t                         ptpCfggm;
    PtpAutoasenable_t                  ptpAutoasenable;
    PtpAs2020Status_t                  ptpAs2020Status;
    PtpSynclockConfig_t                ptpSynclockConfig;
    PtpSyncslowdownConfig_t            ptpSyncslowdownConfig;
    PtpFlashRuntimeAccess_t            ptpFlashRuntimeAccess;
    PtpPdelayWrflashThres_t            ptpPdelayWrflashThres;
    PtpPpsenable_t                     ptpPpsenable;
    PtpTaiPpsEn_t                      ptpTaiPpsEn;
    PtpSignalingen_t                   ptpSignalingen;
    PtpSlaveportmask_t                 ptpSlaveportmask;
    PtpSlaveportid_t                   ptpSlaveportid;
    PtpPriority1_t                     ptpPriority1;
    PtpAllowedfaults_t                 ptpAllowedfaults;
    PtpDelaythresh_t                   ptpDelaythresh;
    PtpDelaymeasureMech_t              ptpDelaymeasureMech;
    PtpDelayAsymmetry_t                ptpDelayAsymmetry;
    PtpOperlogsyncintval_t             ptpOperlogsyncintval;
    PtpMgtsettableLogsyncintval_t      PtpMgtsettableLogsyncintval;
    PtpInitlogpdelayintval_t           ptpInitlogpdelayintval;
    PtpOperlogpdelayintval_t           ptpOperlogpdelayintval;
    PtpMgtsettableLogpdelayreqintval_t PtpMgtsettableLogpdelayreqintval;
    PtpPortremappingen_t               ptpPortremappingen;
    PtpPortremapping_t                 ptpPortremapping;
    PtpDomainmap_t                     ptpDomainmap;
    PtpDomainnum_t                     ptpDomainnum;
    PtpInitlogsyncintval_t             ptpInitlogsyncintval;
    PtpOnesteptxsync_t                 ptpOnesteptxsync;
    PtpInitonesteptxoper_t             ptpInitonesteptxoper;
    PtpMgtsettable1Steptxoper_t        PtpMgtsettable1Steptxoper;
    PtpSwPortmode_t                    ptpSwPortmode;
    PtpCurntPdelayreqIntval_t          PtpCurntPdelayreqIntval;
    PtpCurntsyncintval_t               ptpCurntsyncintval;
    PtpInitGptpmsgintval_t             ptpInitGptpmsgintval;
    PtpMgtsettableGptpmsglogintval_t   PtpMgtsettableGptpmsglogintval;
    PtpNeigptprxtimerout_t             ptpNeigptprxtimerout;
    PtpSendPdelayFromMasterport_t      PtpSendPdelayFromMasterport;
    PtpOverwriteClockid_t              ptpOverwriteClockid;
    PtpFirstbootuptimetarget_t         ptpFirstbootuptimetarget;
    PtpSkipfirstbootrecovery_t         ptpSkipfirstbootrecovery;
    PtpSilenceifneversynced_t          ptpSilenceifneversynced;
    PtpInterServPriEn_t                PtpInterServPriEn;
    PtpCustomDamac01Enable_t           ptpCustomDamac01Enable;
    PtpCustomDamac01_t                 ptpCustomDamac01;
    PtpCustomEthertypeEnable_t         PtpCustomEthertypeEnable;
    PtpCustomEthertype_t               ptpCustomEthertype;
    PtpCustomVlanEnable_t              ptpCustomVlanEnable;
    PtpTag_t                           ptpTag;

    do
    {
        /* Step 1. Get PTP global enable. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_glenable(unitChip, &ptpGlenable)));
        d8 = ptpGlenable.glenable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u glenable: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d8)));
        /* Step 2. Get PTP GM mode. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_cfggm(unitChip, &ptpCfggm)));
        d8 = ptpCfggm.cfggm;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u cfggm: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d8)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u autoasenable: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 3. Get PTP automotive profile supported status. */
            ptpAutoasenable.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_autoasenable(unitChip, &ptpAutoasenable)));
            d8 = ptpAutoasenable.autoasenable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        /* Step 4. Get AS2020 supported status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_as2020_status(unitChip, &ptpAs2020Status)));
        d8 = ptpAs2020Status.as2020En;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u AS2020 status: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d8)));
        /* Step 5. Get PTP synclock configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_synclock_config(unitChip, &ptpSynclockConfig)));
        d8 = ptpSynclockConfig.synclockStatus;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u sync lock config: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d8)));
        /* Step 6. Get PTP sync slowdown configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_syncslowdown_config(unitChip, &ptpSyncslowdownConfig)));
        d8 = ptpSyncslowdownConfig.syncSlowdownStatus;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u sync slowdown config: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d8)));
        /* Step 7. Get PTP runtime access flash configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_flash_runtime_access(unitChip, &ptpFlashRuntimeAccess)));
        d8 = ptpFlashRuntimeAccess.flashAccess;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u runtime access flash: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d8)));
        /* Step 8. Get the threshold of pdelay result writting to flash. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_pdelay_wrflash_thres(unitChip, &ptpPdelayWrflashThres)));
        d32 = ptpPdelayWrflashThres.threshold;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u pdelay write to flash threshold: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d32)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ppsenable: " PTPMSGCOLOR, __LINE__)));
        /* Step 9. Get PPS enable status of index 0. */
        ptpPpsenable.ppsIdx = 0;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_ppsenable(unitChip, &ptpPpsenable)));
        d8 = ptpPpsenable.ppsenable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        /* Step 10. Get PPS enable status of index 1. */
        ptpPpsenable.ppsIdx = 1;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_ppsenable(unitChip, &ptpPpsenable)));
        d8 = ptpPpsenable.ppsenable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u" ANSI_COLOR_RESET "\r\n", d8)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u TAI GPIO enable: " PTPMSGCOLOR, __LINE__)));
        /* Step 11. Get TAI funtion enable status of index 0. */
        ptpTaiPpsEn.ppsidx = 0;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_tai_pps_en(unitChip, &ptpTaiPpsEn)));
        d8 = ptpTaiPpsEn.ppsEn;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        /* Step 12. Get TAI funtion enable status of index 1. */
        ptpTaiPpsEn.ppsidx = 1;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_tai_pps_en(unitChip, &ptpTaiPpsEn)));
        d8 = ptpTaiPpsEn.ppsidx;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u" ANSI_COLOR_RESET "\r\n", d8)));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u sigenable: " PTPMSGCOLOR, __LINE__)));
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Step 13. Get signal message supported status of domain0 to domain2. */
            ptpSignalingen.domainid = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_signalingen(unitChip, &ptpSignalingen)));
            d8 = ptpSignalingen.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        /* Step 14. Get slave port mask of domain0 to domain2. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_slaveportmask(unitChip, &ptpSlaveportmask)));
        d16 = ptpSlaveportmask.slvMask;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u slaveportmask: " PTPMSGCOLOR, __LINE__)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d16)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u slaveportid: " PTPMSGCOLOR, __LINE__)));
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Step 15. Get slave port index of domain0 to domain2. */
            ptpSlaveportid.domainid = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_slaveportid(unitChip, &ptpSlaveportid)));
            d8 = ptpSlaveportid.slaveportid;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u priority1: " PTPMSGCOLOR, __LINE__)));
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Step 16. Get priority1 configuration of domain0 to domain2. */
            ptpPriority1.domainid = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_priority1(unitChip, &ptpPriority1)));
            d8 = ptpPriority1.priority1;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u allowedfaults: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 17. Get allowed faults in pdelay message for each port. */
            ptpAllowedfaults.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_allowedfaults(unitChip, &ptpAllowedfaults)));
            d8 = ptpAllowedfaults.allowedfaults;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%d ", d8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        (void)rtl_memset(&ts, 0, sizeof(ts));
        /* Step 18. Get the threshold of pdelay calculating result. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_delaythresh(unitChip, &ptpDelaythresh)));
        ts = ptpDelaythresh.delaythresh;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u pdelay threshold: " PTPMSGCOLOR "ep:%u s:%u ns:%u" ANSI_COLOR_RESET "\r\n", __LINE__, ts.up16Sec, ts.low32Sec, ts.nanoSec)));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Delay measure mechanism: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 19. Get delay measurement mechanism for each port. */
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_delaymeasure_mech(unitChip, &ptpDelaymeasureMech)));
            d32 = ptpDelaymeasureMech.mechnisam;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d32)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        (void)rtl_memset(&cf, 0, sizeof(cf));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Delay asymmetry: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 20. Get delay asymmetry for each port. */
            ptpDelayAsymmetry.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_delay_asymmetry(unitChip, &ptpDelayAsymmetry)));
            cf = ptpDelayAsymmetry.delayAsymmetry;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Port %u: " PTPMSGCOLOR "up16ns:%u mid32ns:%u sub16ns:%u" ANSI_COLOR_RESET "\r\n", port, cf.up16BitNs, cf.mid32BitNs, cf.sub16bitNs)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u operlogsyncintval: " PTPMSGCOLOR, __LINE__)));
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Step 21. Get operLogSyncInterval for each domain. */
            ptpOperlogsyncintval.domainid = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_operlogsyncintval(unitChip, &ptpOperlogsyncintval)));
            sd8 = ptpOperlogsyncintval.syncintval;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%d ", sd8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u mgtsettable_logsyncintval: " PTPMSGCOLOR, __LINE__)));
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Step 22. Get mgtSettableLogSyncInterval for each domain. */
            PtpMgtsettableLogsyncintval.domain = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_logsyncintval_mgtsettable(unitChip, &PtpMgtsettableLogsyncintval)));
            sd8 = PtpMgtsettableLogsyncintval.logIntval;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%d ", sd8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u initlogpdelayintval: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 23. Get initLogPdelayInterval for each port. */
            ptpInitlogpdelayintval.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_initlogpdelayintval(unitChip, &ptpInitlogpdelayintval)));
            sd8 = ptpInitlogpdelayintval.pdelayintval;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%d ", sd8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u operlogpdelayintval: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 24. Get operLogPdelayInterval for each port. */
            ptpOperlogpdelayintval.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_operlogpdelayintval(unitChip, &ptpOperlogpdelayintval)));
            sd8 = ptpOperlogpdelayintval.pdelayintval;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%d ", sd8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u mgtsettable_logpdelayreqintval: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 25. Get mgtSettableLogPdelayReqInterval for each port. */
            PtpMgtsettableLogpdelayreqintval.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_logpdelayreqintval_mgtsettable(unitChip, &PtpMgtsettableLogpdelayreqintval)));
            sd8 = PtpMgtsettableLogpdelayreqintval.intval;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%d ", sd8)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        /* Step 26. Get port remapping enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_en_portremapping(unitChip, &ptpPortremappingen)));
        d8 = ptpPortremappingen.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port remapping: " PTPMSGCOLOR, __LINE__)));
        if(1u == d8)
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Enable")));
        }
        else
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Disable")));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET " mapping table: " PTPMSGCOLOR)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 27. Get port remapping number for each port. */
            ptpPortremapping.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_portremapping(unitChip, &ptpPortremapping)));
            d32 = ptpPortremapping.remapping;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d32)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Step 28. Get joined port mask for each domain. */
            ptpDomainmap.domainid = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_domainmap(unitChip, &ptpDomainmap)));
            d16 = ptpDomainmap.domainmap;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u domain %u map " PTPMSGCOLOR "0x%X" ANSI_COLOR_RESET "\r\n", __LINE__, domainid, d16)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port:%u ", port, __LINE__)));
            for(domainid = 0u; domainid < 3u; domainid++)
            {
                /* Step 29. Get domain number for each domain. */
                ptpDomainnum.port     = port;
                ptpDomainnum.domainid = domainid;
                CHK_FUN_BREAK((retAPI), (rtk_ptp_get_domainnum(unitChip, &ptpDomainnum)));
                d8 = ptpDomainnum.domainnum;
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("domain %u" PTPMSGCOLOR " domainnum %u " ANSI_COLOR_RESET, domainid, d8)));
            }
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
            if(RT_ERR_OK != retAPI)
            {
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port:%u\r\n", __LINE__, port)));
            for(domainid = 0u; domainid < 3u; domainid++)
            {
                /* Step 30. Get initLogSyncInterval for each domain. */
                ptpInitlogsyncintval.port     = port;
                ptpInitlogsyncintval.domainid = domainid;
                CHK_FUN_BREAK((retAPI), (rtk_ptp_get_initlogsyncintval(unitChip, &ptpInitlogsyncintval)));
                sd8 = ptpInitlogsyncintval.initLogSyncIntval;
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("\tdomain %u" PTPMSGCOLOR " initLogSyncIntval %d " ANSI_COLOR_RESET "\r\n", domainid, sd8)));
            }
            if(RT_ERR_OK != retAPI)
            {
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u oneStepTxSync: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 31. Get one step trasmission operability for each port. */
            ptpOnesteptxsync.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_onesteptxsync(unitChip, &ptpOnesteptxsync)));
            d8 = ptpOnesteptxsync.oneStepTxSync;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port: %u ", __LINE__, port)));
            for(domainid = 0u; domainid < 3u; domainid++)
            {
                /* Step 32. Get one step initialized trasmission operability for each domain. */
                ptpInitonesteptxoper.port     = port;
                ptpInitonesteptxoper.domainid = domainid;
                retAPI                        = rtk_ptp_get_initonesteptxoper(unitChip, &ptpInitonesteptxoper);
                d8                            = ptpInitonesteptxoper.initOneStepTXOperEn;
                if(RT_ERR_OK != retAPI)
                {
                    retAPIFlag |= 1u << 1u;
                }
                else
                {
                    CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("domain %u init 1step tx oper" PTPMSGCOLOR " %u " ANSI_COLOR_RESET "\r\n", domainid, d8)));
                }

                /* Step 33. Get one step MgtSettableOneStepTxOper for each domain. */
                PtpMgtsettable1Steptxoper.port   = port;
                PtpMgtsettable1Steptxoper.domain = domainid;
                retAPI                           = rtk_ptp_get_1steptxoper_mgtsettable(unitChip, &PtpMgtsettable1Steptxoper);
                d8                               = PtpMgtsettable1Steptxoper.txOper;
                if(RT_ERR_OK != retAPI)
                {
                    retAPIFlag |= 1u << 2u;
                }
                else
                {
                    CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("domain %u mgtsettable 1step tx oper" PTPMSGCOLOR " %u " ANSI_COLOR_RESET "\r\n", domainid, d8)));
                }
                if(0u != retAPIFlag)
                {
                    retAPI = -RT_ERR_FAILED;
                    break;
                }
            }
            if(RT_ERR_OK != retAPI)
            {
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port: %u ", __LINE__, port)));
            for(domainid = 0u; domainid < 3u; domainid++)
            {
                /* Step 34. Get SW port mode configuration for each domain. */
                ptpSwPortmode.port     = port;
                ptpSwPortmode.domainid = domainid;
                CHK_FUN_BREAK((retAPI), (rtk_ptp_get_sw_portmode(unitChip, &ptpSwPortmode)));
                d8 = ptpSwPortmode.portmode;
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("domain %u portmode" PTPMSGCOLOR " %u " ANSI_COLOR_RESET "\r\n", domainid, d8)));
            }
            if(RT_ERR_OK != retAPI)
            {
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 35. Get currect pdelay request interval for each port. */
            PtpCurntPdelayreqIntval.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_curnt_pdelayreq_intval(unitChip, &PtpCurntPdelayreqIntval)));
            d32 = PtpCurntPdelayreqIntval.curntPdelayReqIntval;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port %u curntPdelayReqIntval: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, __LINE__, port, d32)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port: %u \r\n", __LINE__, port)));
            for(domainid = 0u; domainid < 3u; domainid++)
            {
                /* Step 36. Get currect sync interval for each domain. */
                ptpCurntsyncintval.port     = port;
                ptpCurntsyncintval.domainid = domainid;
                CHK_FUN_BREAK((retAPI), (rtk_ptp_get_curntsyncintval(unitChip, &ptpCurntsyncintval)));
                d32 = ptpCurntsyncintval.curntsyncintval;
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("\tdomain %u curntsyncintval" PTPMSGCOLOR " %u\r\n" ANSI_COLOR_RESET, domainid, d32)));
            }
            if(RT_ERR_OK != retAPI)
            {
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }

        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u port: %u ", __LINE__, port)));
            for(domainid = 0u; domainid < 3u; domainid++)
            {
                /* Step 37. Get initialized gPTPCapable message interval for each domain. */
                ptpInitGptpmsgintval.port   = port;
                ptpInitGptpmsgintval.domain = domainid;
                retAPI                      = rtk_ptp_get_init_gptpmsgintval(unitChip, &ptpInitGptpmsgintval);
                sd8                         = ptpInitGptpmsgintval.intval;
                if(RT_ERR_OK != retAPI)
                {
                    retAPIFlag |= 1u << 1;
                }
                else
                {
                    CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("domain %u GPTP message interval" PTPMSGCOLOR " %d " ANSI_COLOR_RESET "\r\n", domainid, sd8)));
                }
                /* Step 38. Get mgtSettable gPTPCapable message interval for each domain. */
                PtpMgtsettableGptpmsglogintval.port   = port;
                PtpMgtsettableGptpmsglogintval.domain = domainid;
                retAPI                                = rtk_ptp_get_gptpmsglogintval_mgtsettable(unitChip, &PtpMgtsettableGptpmsglogintval);
                sd8                                   = PtpMgtsettableGptpmsglogintval.intval;
                if(RT_ERR_OK != retAPI)
                {
                    retAPIFlag |= 1u << 2;
                }
                else
                {
                    CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("domain %u mgtsettable GPTP message interval" PTPMSGCOLOR " %d " ANSI_COLOR_RESET "\r\n", domainid, sd8)));
                }

                if(0u != retAPIFlag)
                {
                    retAPI = -RT_ERR_FAILED;
                    break;
                }
            }
            if(RT_ERR_OK != retAPI)
            {
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u neighbor GPTP message rx timeout count:" PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 39. Get neighbor GPTP message rx timeout count for each domain. */
            ptpNeigptprxtimerout.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_neigptprxtimerout(unitChip, &ptpNeigptprxtimerout)));
            d8 = ptpNeigptprxtimerout.timeoutCnt;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        /* Step 40. Get sending pdelay request from master port configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_send_pdelay_from_masterport(unitChip, &PtpSendPdelayFromMasterport)));
        d8 = PtpSendPdelayFromMasterport.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Send Pdelay from master port: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, d8)));
        /* Step 41. Get overwrite clockID configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_overwrite_clockid_get(unitChip, &ptpOverwriteClockid)));
        d8 = ptpOverwriteClockid.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Over Write PTP Clock ID on Sync/Follow-Up: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, d8)));
        /* Step 42. Get first sync target time in bootup. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_firstbootuptimetarget(unitChip, &ptpFirstbootuptimetarget)));
        d16 = ptpFirstbootuptimetarget.timetarget;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("First sync target time in bootup: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, d16)));
        /* Step 43. Get recovery configuration once reach firstbootuptimetarget. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_skipfirstbootrecovery(unitChip, &ptpSkipfirstbootrecovery)));
        d8 = ptpSkipfirstbootrecovery.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Skip firstboot recovery: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, d8)));
        /* Step 44. Get recovery configuration after firstbootuptimetarget. */
        ptpSilenceifneversynced.domainid = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));
        d8 = ptpSilenceifneversynced.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("Silenceifneversynced: " PTPMSGCOLOR "%u \r\n" ANSI_COLOR_RESET, d8)));
        /* Step 45. Get internal sublayer service priority enable status. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_en_inter_serv_pri(unitChip, &PtpInterServPriEn)));
        d16 = PtpInterServPriEn.priority_en;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ISS priority enable: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET "\r\n", __LINE__, d16)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize DA enable: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 46. Get customized DA enable status for each port. */
            ptpCustomDamac01Enable.port = port;
            retAPI                      = rtk_ptp_get_damac01_enable_custom(unitChip, &ptpCustomDamac01Enable);
            d8                          = ptpCustomDamac01Enable.enable;
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 1;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
            }
            /* Step 47. Get customized DA for each port. */
            ptpCustomDamac01.port = port;
            retAPI                = rtk_ptp_get_custom_damac01(unitChip, &ptpCustomDamac01);
            for(uint8 i = 0u; i < 6u; i++)
            {
                da[i] = ptpCustomDamac01.damac[i];
            }
            if(RT_ERR_OK != retAPI)
            {
                retAPIFlag |= 1u << 2;
            }
            else
            {
                CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize DA:" PTPMSGCOLOR " %02x:%02x:%02x:%02x:%02x:%02x\r\n" ANSI_COLOR_RESET, __LINE__, da[0], da[1], da[2], da[3], da[4], da[5])));
            }
            if(0u != retAPIFlag)
            {
                retAPI = -RT_ERR_FAILED;
                break;
            }
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        /* Step 48. Get customized DA enable status for CPU port. */
        ptpCustomDamac01Enable.port = CVA_RTK_CPU_PORT;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_damac01_enable_custom(unitChip, &ptpCustomDamac01Enable)));
        d8 = ptpCustomDamac01Enable.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("CPU Port:%u ", d8)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize DA:" PTPMSGCOLOR " %02x:%02x:%02x:%02x:%02x:%02x\r\n" ANSI_COLOR_RESET, __LINE__, da[0], da[1], da[2], da[3], da[4], da[5])));
        /* Step 49. Get customized DA for CPU port. */
        ptpCustomDamac01.port = CVA_RTK_CPU_PORT;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_custom_damac01(unitChip, &ptpCustomDamac01)));
        for(uint8 i = 0u; i < (sizeof(da) / sizeof(uint8)); i++)
        {
            da[i] = ptpCustomDamac01.damac[i];
        }
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize DA(CPU Port):" PTPMSGCOLOR " %02x:%02x:%02x:%02x:%02x:%02x\r\n" ANSI_COLOR_RESET, __LINE__, da[0], da[1], da[2], da[3], da[4], da[5])));

        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize etherType enable: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 50. Get customized ethertype enable status for each port. */
            PtpCustomEthertypeEnable.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_ethertype_enable_custom(unitChip, &PtpCustomEthertypeEnable)));
            d8 = PtpCustomEthertypeEnable.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        /* Step 51. Get customized ethertype enable status for CPU port. */
        PtpCustomEthertypeEnable.port = CVA_RTK_CPU_PORT;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_ethertype_enable_custom(unitChip, &PtpCustomEthertypeEnable)));
        d8 = PtpCustomEthertypeEnable.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("CPU Port:%u ", d8)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize etherType :" PTPMSGCOLOR " 0x%04x\r\n" ANSI_COLOR_RESET, __LINE__, ethertype)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 52. Get customized ethertype for each port. */
            ptpCustomEthertype.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_get_custom_ethertype(unitChip, &ptpCustomEthertype)));
            ethertype = ptpCustomEthertype.ethertype;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", ethertype)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        /* Step 53. Get customized ethertype for CPU port. */
        ptpCustomEthertype.port = CVA_RTK_CPU_PORT;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_get_custom_ethertype(unitChip, &ptpCustomEthertype)));
        ethertype = ptpCustomEthertype.ethertype;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("CPU Port:%u ", ethertype)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize PTP VLAN enable: " PTPMSGCOLOR, __LINE__)));
        for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_11TH; port++)
        {
            /* Step 54. Get customized VLAN enable status for each port. */
            ptpCustomVlanEnable.port = port;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_custom_get_vlan_enable(unitChip, &ptpCustomVlanEnable)));
            d8 = ptpCustomVlanEnable.enable;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u ", d8)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        /* Step 55. Get customized VLAN enable status for CPU port. */
        ptpCustomVlanEnable.port = CVA_RTK_CPU_PORT;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_custom_get_vlan_enable(unitChip, &ptpCustomVlanEnable)));
        d8 = ptpCustomVlanEnable.enable;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("CPU Port:%u ", d8)));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(ANSI_COLOR_RESET "\r\n")));
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("%u Customize PTP VLAN Tag: " PTPMSGCOLOR, __LINE__)));
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            (void)rtl_memset(&vtag, 0, sizeof(vtag));
            /* Step 56. Get customized VLAN tag for each domain. */
            ptpTag.domainid = domainid;
            CHK_FUN_BREAK((retAPI), (rtk_ptp_tag_get(unitChip, &ptpTag)));
            vtag = ptpTag.taginfo;
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("fix:" PTPMSGCOLOR "%u" ANSI_COLOR_RESET " enable:" PTPMSGCOLOR "%u" ANSI_COLOR_RESET " tagLength: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET " \r\n", vtag.fix, vtag.enable, vtag.tagLength)));
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("qtag[0]: TCI: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET " enable:" PTPMSGCOLOR "%u" ANSI_COLOR_RESET " \r\n", vtag.qtag[0].tci, vtag.qtag[0].enable)));
            CHK_FUN_CONTINUE((retPrint), (rtlglue_printf("qtag[1]: TCI: " PTPMSGCOLOR "%u" ANSI_COLOR_RESET " enable:" PTPMSGCOLOR "%u" ANSI_COLOR_RESET " \r\n", vtag.qtag[1].tci, vtag.qtag[1].enable)));
        }
        if(RT_ERR_OK != retAPI)
        {
            ret = retAPI;
            continue;
        }
        /* Step 57. Get useMgtSettable configs. */
        CHK_FUN_CONTINUE((ret), (cva_ptp_get_usemgtsettable_config(unitChip)));
        /* Step 58. Get Other enable status. */
        CHK_FUN_CONTINUE((ret), (cva_ptp_other_enable_result(unitChip)));
    } while(0u == 1u);

    return ret;
}

/* Expected Behaviour: Dump all PTP configuration. */

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XCVA_PTP_EXAMPLE */
/**@}*/ /* RTL907XCVA_PTPDUMP_CFG_EXAMPLE */