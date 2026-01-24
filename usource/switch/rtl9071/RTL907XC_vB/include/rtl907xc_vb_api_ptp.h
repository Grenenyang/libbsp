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

#ifndef CVB_RTK_API_PTP_H__
#define CVB_RTK_API_PTP_H__

#include "rtl907xc_vb_api.h"
#include "rtkas_api_ptp.h"

#define CVB_CFiled_t_LENGTH_IN_FW (12u)
#define CVB_MIN_TIMEOUT           (1u)

#define CVB_TX_LATENCY_100BASE_T1                    (1447u)
#define CVB_TX_LATENCY_10BASE_T1                     (0u)
#define CVB_TX_LATENCY_FAST_ETHERNET_100             (1376u)
#define CVB_TX_LATENCY_FAST_ETHERNET_10              (0u)
#define CVB_TX_LATENCY_FAST_ETHERNET_AN              (1376u)
#define CVB_TX_LATENCY_MII_100                       (1200u)
#define CVB_TX_LATENCY_MII_10                        (0u)
#define CVB_TX_LATENCY_RMII_100                      (1200u)
#define CVB_TX_LATENCY_RMII_10                       (0u)
#define CVB_TX_LATENCY_RGMII_1000                    (120u)
#define CVB_TX_LATENCY_RGMII_100                     (1200u)
#define CVB_TX_LATENCY_RGMII_10                      (0u)
#define CVB_TX_LATENCY_SGMII_1000_P7                 (275u)
#define CVB_TX_LATENCY_SGMII_100_P7                  (1387u)
#define CVB_TX_LATENCY_SGMII_10                      (0u)
#define CVB_TX_LATENCY_SGMII_AN                      (275u)
#define CVB_TX_LATENCY_1000BASE_T1                   (359u)
#define CVB_TX_LATENCY_HISGMII_2500                  (182u)
#define CVB_TX_LATENCY_5GKR                          (258u)
#define CVB_TX_LATENCY_USXGMII_5000                  (18u)
#define CVB_TX_LATENCY_USXGMII_2500                  (35u)
#define CVB_TX_LATENCY_USXGMII_1000                  (8u)
#define CVB_TX_LATENCY_USXGMII_AN                    (18u)
#define CVB_TX_LATENCY_RTPGE_100BASE_T1              (1426u)
#define CVB_TX_LATENCY_MACPHY                        (0u)
#define CVB_TX_LATENCY_SGMII_1000_P9                 (8u)
#define CVB_TX_LATENCY_SGMII_100_P9                  (516u)
#define CVB_TX_LATENCY_PURE_100BASE_T1_WTIH_MACSEC   (11687u)
#define CVB_TX_LATENCY_RTPGE_1000BASE_T1_WITH_MACSEC (1369u)
#define CVB_TX_LATENCY_RTPGE_100BASE_T1_WITH_MACSEC  (11701u)
#define CVB_TX_LATENCY_MII_100_WITH_MACSEC           (11440u)
#define CVB_TX_LATENCY_RMII_100_WITH_MACSEC          (11440u)
#define CVB_TX_LATENCY_RGMII_1000_WITH_MACSEC        (1144u)
#define CVB_TX_LATENCY_RGMII_100_WITH_MACSEC         (11440u)
#define CVB_TX_LATENCY_SGMII_WTG_1000_WITH_MACSEC    (920u)
#define CVB_TX_LATENCY_SGMII_WTG_100_WITH_MACSEC     (9635u)
#define CVB_TX_LATENCY_HISGMII_WTG_2500_WITH_MACSEC  (550u)
#define CVB_TX_LATENCY_USXGMII_WTG_5000_WITH_MACSEC  (1079u)
#define CVB_TX_LATENCY_USXGMII_WTG_2500_WITH_MACSEC  (2162u)
#define CVB_TX_LATENCY_BaseKR_WTG_5000_WITH_MACSEC   (1325u)
#define CVB_TX_LATENCY_NOT_SUPPORT                   (65535u)

#define CVB_RX_LATENCY_100BASE_T1                    (800u)
#define CVB_RX_LATENCY_10BASE_T1                     (0u)
#define CVB_RX_LATENCY_FAST_ETHERNET_100             (448u)
#define CVB_RX_LATENCY_FAST_ETHERNET_10              (0u)
#define CVB_RX_LATENCY_FAST_ETHERNET_AN              (448u)
#define CVB_RX_LATENCY_MII_100                       (312u)
#define CVB_RX_LATENCY_MII_10                        (0u)
#define CVB_RX_LATENCY_RMII_100                      (312u)
#define CVB_RX_LATENCY_RMII_10                       (0u)
#define CVB_RX_LATENCY_RGMII_1000                    (8u)
#define CVB_RX_LATENCY_RGMII_100                     (536u)
#define CVB_RX_LATENCY_RGMII_10                      (0u)
#define CVB_RX_LATENCY_SGMII_1000_P7                 (165u)
#define CVB_RX_LATENCY_SGMII_100_P7                  (365u)
#define CVB_RX_LATENCY_SGMII_10                      (0u)
#define CVB_RX_LATENCY_SGMII_AN                      (165u)
#define CVB_RX_LATENCY_1000BASE_T1                   (4457u)
#define CVB_RX_LATENCY_HISGMII_2500                  (131u)
#define CVB_RX_LATENCY_5GKR                          (356u)
#define CVB_RX_LATENCY_USXGMII_5000                  (839u)
#define CVB_RX_LATENCY_USXGMII_2500                  (1016u)
#define CVB_RX_LATENCY_USXGMII_1000                  (8u)
#define CVB_RX_LATENCY_USXGMII_AN                    (839u)
#define CVB_RX_LATENCY_RTPGE_100BASE_T1              (1414u)
#define CVB_RX_LATENCY_MACPHY                        (0u)
#define CVB_RX_LATENCY_SGMII_1000_P9                 (352u)
#define CVB_RX_LATENCY_SGMII_100_P9                  (484u)
#define CVB_RX_LATENCY_PURE_100BASE_T1_WTIH_MACSEC   (11680)
#define CVB_RX_LATENCY_RTPGE_1000BASE_T1_WITH_MACSEC (5686u)
#define CVB_RX_LATENCY_RTPGE_100BASE_T1_WITH_MACSEC  (12109u)
#define CVB_RX_LATENCY_MII_100_WITH_MACSEC           (11192u)
#define CVB_RX_LATENCY_RMII_100_WITH_MACSEC          (11192u)
#define CVB_RX_LATENCY_RGMII_1000_WITH_MACSEC        (1096u)
#define CVB_RX_LATENCY_RGMII_100_WITH_MACSEC         (11376u)
#define CVB_RX_LATENCY_SGMII_WTG_1000_WITH_MACSEC    (1328u)
#define CVB_RX_LATENCY_SGMII_WTG_100_WITH_MACSEC     (10244u)
#define CVB_RX_LATENCY_HISGMII_WTG_2500_WITH_MACSEC  (523u)
#define CVB_RX_LATENCY_USXGMII_WTG_5000_WITH_MACSEC  (1991u)
#define CVB_RX_LATENCY_USXGMII_WTG_2500_WITH_MACSEC  (3352u)
#define CVB_RX_LATENCY_BaseKR_WTG_5000_WITH_MACSEC   (1507u)
#define CVB_RX_LATENCY_NOT_SUPPORT                   (65535u)

/********* BEGIN TIME SYNC/PTP *****************/
EXTERN_API RtkApiRet cvb_ptp_get_interface_latency(uint32 unit, uint8 direction, PtpPortType_e interface, uint16 *latency);
EXTERN_API RtkApiRet cvb_ptp_log_to_intval(uint32 unit, int8 const loginterval, uint16 *intval);
EXTERN_API RtkApiRet cvb_ptp_get_port_time_sync_en(uint32 unit, RtkPort port, uint8 *ptimeSyncEn);
EXTERN_API RtkApiRet cvb_ptp_set_port_time_sync_en(uint32 unit, RtkPort port, uint8 timeSyncEn);
EXTERN_API RtkApiRet cvb_ptp_get_hw_enable(uint32 unit, uint8 *pptpEnable);
EXTERN_API RtkApiRet cvb_ptp_set_hw_enable(uint32 unit, uint8 ptpEnable);
EXTERN_API RtkApiRet cvb_ptp_get_sys_time_en(uint32 unit, uint8 *psysTimeEn);
EXTERN_API RtkApiRet cvb_ptp_set_sys_time_en(uint32 unit, uint8 sysTimeEn);
EXTERN_API RtkApiRet cvb_ptp_get_tai_pps_en(uint32 unit, uint8 ppsidx, uint8 *pppsEn);
EXTERN_API RtkApiRet cvb_ptp_set_sys_pps_en(uint32 unit, uint8 ppsidx, uint8 ppsEn);
EXTERN_API RtkApiRet cvb_ptp_get_redirect_cpumask(uint32 unit, uint16 *pcpumask);
EXTERN_API RtkApiRet cvb_ptp_set_redirect_cpumask(uint32 unit, uint16 cpumask);
EXTERN_API RtkApiRet cvb_ptp_get_redirect_act(uint32 unit, uint8 *pptpRedirAct);
EXTERN_API RtkApiRet cvb_ptp_set_redirect_act(uint32 unit, uint8 ptpRedirAct);
EXTERN_API RtkApiRet cvb_ptp_get_master_en(uint32 unit, uint8 *pmasterEn);
EXTERN_API RtkApiRet cvb_ptp_set_master_en(uint32 unit, uint8 masterEn);
EXTERN_API RtkApiRet cvb_ptp_get_slave2_en(uint32 unit, uint8 *pslave2En);
EXTERN_API RtkApiRet cvb_ptp_set_slave2_en(uint32 unit, uint8 slave2En);
EXTERN_API RtkApiRet cvb_ptp_get_slave1_en(uint32 unit, uint8 *pslave1En);
EXTERN_API RtkApiRet cvb_ptp_set_slave1_en(uint32 unit, uint8 slave1En);
EXTERN_API RtkApiRet cvb_ptp_get_slave0_en(uint32 unit, uint8 *pslave0En);
EXTERN_API RtkApiRet cvb_ptp_set_slave0_en(uint32 unit, uint8 slave0En);
EXTERN_API RtkApiRet cvb_ptp_get_ether_check_enable0(uint32 unit, RtkPort port, uint8 *checkEnable);
EXTERN_API RtkApiRet cvb_ptp_set_ether_check_enable0(uint32 unit, RtkPort port, uint8 checkEnable);
EXTERN_API RtkApiRet cvb_ptp_get_port_ptp_100tx(uint32 unit, RtkPort port, uint8 *pptp100tx);
EXTERN_API RtkApiRet cvb_ptp_set_port_ptp_100tx(uint32 unit, RtkPort port, uint8 ptp100tx);
EXTERN_API RtkApiRet cvb_ptp_get_port_rxlatency(uint32 unit, RtkPort port, uint16 *prxLatency);
EXTERN_API RtkApiRet cvb_ptp_set_port_rxlatency(uint32 unit, RtkPort port, uint16 rxLatency);
EXTERN_API RtkApiRet cvb_ptp_get_port_txlatency(uint32 unit, RtkPort port, uint16 *ptxLatency);
EXTERN_API RtkApiRet cvb_ptp_set_port_txlatency(uint32 unit, RtkPort port, uint16 txLatency);
EXTERN_API RtkApiRet cvb_ptp_get_port_slave_time_domain(uint32 unit, RtkPort port, uint32 domainid, uint8 *domainvalue);
EXTERN_API RtkApiRet cvb_ptp_set_port_slave_time_domain(uint32 unit, RtkPort port, uint32 domainid, uint8 domainvalue);
EXTERN_API RtkApiRet cvb_ptp_get_curnt_pdelayreq_intval(uint32 unit, RtkPort port, uint32 *curntPdelayReqIntval);
EXTERN_API RtkApiRet cvb_ptp_set_curnt_pdelayreq_intval(uint32 unit, RtkPort port, uint32 curntPdelayReqIntval);
EXTERN_API RtkApiRet cvb_ptp_get_portmode(uint32 unit, RtkPort port, uint8 *pportmode);
EXTERN_API RtkApiRet cvb_ptp_set_portmode(uint32 unit, RtkPort port, uint8 portmode);
EXTERN_API RtkApiRet cvb_ptp_get_glenable(uint32 unit, uint8 *glenable);
EXTERN_API RtkApiRet cvb_ptp_set_glenable(uint32 unit, uint8 glenable);
EXTERN_API RtkApiRet cvb_ptp_get_cfggm(uint32 unit, uint8 *cfggm);
EXTERN_API RtkApiRet cvb_ptp_set_cfggm(uint32 unit, uint8 cfggm);
EXTERN_API RtkApiRet cvb_ptp_get_autoasenable(uint32 unit, RtkPort port, uint8 *autoasenable);
EXTERN_API RtkApiRet cvb_ptp_set_autoasenable(uint32 unit, RtkPort port, uint8 autoasenable);
EXTERN_API RtkApiRet cvb_ptp_get_ppsenable(uint32 unit, uint16 ppsIdx, uint8 *ppsenable);
EXTERN_API RtkApiRet cvb_ptp_set_ppsenable(uint32 unit, uint16 ppsIdx, uint8 ppsenable);
EXTERN_API RtkApiRet cvb_ptp_get_signalingen(uint32 unit, uint32 domainid, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_signalingen(uint32 unit, uint32 domainid, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_slaveportid(uint32 unit, uint32 domainid, uint8 *slaveportid);
EXTERN_API RtkApiRet cvb_ptp_set_slaveportid(uint32 unit, uint32 domainid, uint8 slaveportid);
EXTERN_API RtkApiRet cvb_ptp_get_slaveportmask(uint32 unit, uint16 *slaveportmask);
EXTERN_API RtkApiRet cvb_ptp_set_slaveportmask(uint32 unit, uint16 slvMask);
EXTERN_API RtkApiRet cvb_ptp_get_priority1(uint32 unit, uint32 domainid, uint8 *priority1);
EXTERN_API RtkApiRet cvb_ptp_set_priority1(uint32 unit, uint32 domainid, uint8 priority1);
EXTERN_API RtkApiRet cvb_ptp_get_delaythresh(uint32 unit, PtpTs_t *delaythresh);
EXTERN_API RtkApiRet cvb_ptp_set_delaythresh(uint32 unit, PtpTs_t const *const delaythresh);
EXTERN_API RtkApiRet cvb_ptp_get_initlogpdelayintval(uint32 unit, RtkPort port, int8 *pdelayintval);
EXTERN_API RtkApiRet cvb_ptp_set_initlogpdelayintval(uint32 unit, RtkPort port, int8 pdelayintval);
EXTERN_API RtkApiRet cvb_ptp_get_operlogsyncintval(uint32 unit, uint32 domainid, int8 *syncintval);
EXTERN_API RtkApiRet cvb_ptp_set_operlogsyncintval(uint32 unit, uint32 domainid, int8 syncintval);
EXTERN_API RtkApiRet cvb_ptp_get_operlogpdelayintval(uint32 unit, RtkPort port, int8 *pdelayintval);
EXTERN_API RtkApiRet cvb_ptp_set_operlogpdelayintval(uint32 unit, RtkPort port, int8 pdelayintval);
EXTERN_API RtkApiRet cvb_ptp_get_exchangeInfo(uint32 unit, uint8 *exchangeInfo);
EXTERN_API RtkApiRet cvb_ptp_set_exchangeInfo(uint32 unit, uint8 exchangeInfo);
EXTERN_API RtkApiRet cvb_ptp_get_firstbootuptimetarget(uint32 unit, uint16 *timetarget);
EXTERN_API RtkApiRet cvb_ptp_set_firstbootuptimetarget(uint32 unit, uint16 timetarget);
EXTERN_API RtkApiRet cvb_ptp_get_curntsyncintval(uint32 unit, RtkPort port, uint32 domainid, uint32 *curntsyncintval);
EXTERN_API RtkApiRet cvb_ptp_set_curntsyncintval(uint32 unit, RtkPort port, uint32 domainid, uint32 curntsyncintval);
EXTERN_API RtkApiRet cvb_ptp_get_sw_portmode(uint32 unit, RtkPort port, uint32 domainid, uint8 *portmode);
EXTERN_API RtkApiRet cvb_ptp_set_sw_portmode(uint32 unit, RtkPort port, uint32 domainid, uint8 portmode);
EXTERN_API RtkApiRet cvb_ptp_get_en_portremapping(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_en_portremapping(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_onesteptxsync(uint32 unit, RtkPort port, uint8 *oneStepTxSync);
EXTERN_API RtkApiRet cvb_ptp_set_onesteptxsync(uint32 unit, RtkPort port, uint8 oneStepTxSync);
EXTERN_API RtkApiRet cvb_ptp_get_initlogsyncintval(uint32 unit, RtkPort port, uint32 domainid, int8 *initLogSyncIntval);
EXTERN_API RtkApiRet cvb_ptp_set_initlogsyncintval(uint32 unit, RtkPort port, uint32 domainid, int8 initLogSyncIntval);
EXTERN_API RtkApiRet cvb_ptp_get_domainnum(uint32 unit, RtkPort port, uint32 domainid, uint8 *domainnum);
EXTERN_API RtkApiRet cvb_ptp_set_domainnum(uint32 unit, RtkPort port, uint32 domainid, uint8 domainnum);
EXTERN_API RtkApiRet cvb_ptp_get_domainmap(uint32 unit, uint32 domainid, uint16 *domainmap);
EXTERN_API RtkApiRet cvb_ptp_set_domainmap(uint32 unit, uint32 domainid, uint16 domainmap);
EXTERN_API RtkApiRet cvb_ptp_get_portremapping(uint32 unit, RtkPort port, RtkPort *remapping);
EXTERN_API RtkApiRet cvb_ptp_set_portremapping(uint32 unit, RtkPort port, RtkPort remapping);
EXTERN_API RtkApiRet cvb_ptp_set_custom_ethertype(uint32 unit, RtkPort port, uint16 ethertype);
EXTERN_API RtkApiRet cvb_ptp_get_custom_ethertype(uint32 unit, RtkPort port, uint16 *ethertype);
EXTERN_API RtkApiRet cvb_ptp_set_ethertype_enable_custom(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_ethertype_enable_custom(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_custom_set_vlan_enable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_custom_get_vlan_enable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_custom_damac01(uint32 unit, RtkPort port, uint8 const *const damac);
EXTERN_API RtkApiRet cvb_ptp_get_custom_damac01(uint32 unit, RtkPort port, uint8 *damac);
EXTERN_API RtkApiRet cvb_ptp_set_damac01_enable_custom(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_damac01_enable_custom(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_clkstable_get(uint32 unit, uint32 domainid, uint8 *clkstable);
EXTERN_API RtkApiRet cvb_ptp_reinitprotocol(uint32 unit);
EXTERN_API RtkApiRet cvb_ptp_protocolportmask_set(uint32 unit, uint32 domainid, uint16 const *const portmask);
EXTERN_API RtkApiRet cvb_ptp_set_skipfirstbootrecovery(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_set_silenceifneversynced(uint32 unit, uint32 domainid, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_overwrite_ptpid_set(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_set_send_pdelay_from_masterport(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_onesteptxen_sw_set(uint32 unit, RtkPort port, uint32 domainid, uint8 oneStepTxSync);
EXTERN_API RtkApiRet cvb_ptp_tag_set(uint32 unit, uint32 domainid, ptpqTaginfo *taginfo);
EXTERN_API RtkApiRet cvb_ptp_rx_sync_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_rx_follow_up_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_rx_pdelay_request_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_rx_pdelay_response_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_rx_pdelay_resp_follow_up_mib_get(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_rx_ptp_packet_discard_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_sync_receipt_timeout_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_pdelay_allowed_lost_response_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_tx_sync_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_tx_follow_up_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_tx_pdelay_request_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_tx_pdelay_response_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_tx_pdelay_resp_follow_up_mib_get(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_ptp_onesteprxen_sw_set(uint32 unit, RtkPort port, uint32 domainid, uint8 oneStepRxSync);
EXTERN_API RtkApiRet cvb_ptp_set_initonesteptxoper(uint32 unit, RtkPort port, uint32 domainid, uint8 initOneStepTXOperEn);
EXTERN_API RtkApiRet cvb_ptp_get_initonesteptxoper(uint32 unit, RtkPort port, uint32 domainid, uint8 *initOneStepTXOperEn);
EXTERN_API RtkApiRet cvb_ptp_get_flash_runtime_access(uint32 unit, uint8 *flashAccess);
EXTERN_API RtkApiRet cvb_ptp_set_flash_runtime_access(uint32 unit, uint8 flashAccess);
EXTERN_API RtkApiRet cvb_ptp_set_pdelay_wrflash_thres(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet cvb_ptp_get_pdelay_wrflash_thres(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet cvb_ptp_set_synclock_config(uint32 unit, uint8 synclockStatus);
EXTERN_API RtkApiRet cvb_ptp_get_synclock_config(uint32 unit, uint8 *synclockStatus);
EXTERN_API RtkApiRet cvb_ptp_set_allowedfaults(uint32 unit, RtkPort port, uint8 allowedfaults);
EXTERN_API RtkApiRet cvb_ptp_get_allowedfaults(uint32 unit, RtkPort port, uint8 *allowedfaults);
EXTERN_API RtkApiRet cvb_ptp_set_delaymeasure_mech(uint32 unit, uint32 mechnisam);
EXTERN_API RtkApiRet cvb_ptp_get_delaymeasure_mech(uint32 unit, uint32 *mechnisam);
EXTERN_API RtkApiRet cvb_ptp_set_delay_asymmetry(uint32 unit, RtkPort port, CFiled_t delayAsymmetry);
EXTERN_API RtkApiRet cvb_ptp_get_delay_asymmetry(uint32 unit, RtkPort port, CFiled_t *delayAsymmetry);
EXTERN_API RtkApiRet cvb_ptp_get_use_mgtsettable_logsyncintval(uint32 unit, uint32 domain, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_use_mgtsettable_logsyncintval(uint32 unit, uint32 domain, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_set_logsyncintval_mgtsettable(uint32 unit, uint32 domain, int8 logIntval);
EXTERN_API RtkApiRet cvb_ptp_get_logsyncintval_mgtsettable(uint32 unit, uint32 domain, int8 *logIntval);
EXTERN_API RtkApiRet cvb_ptp_get_usemgtsettable_logpdelayreqintval(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_usemgtsettable_logpdelayreqintval(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_set_logpdelayreqintval_mgtsettable(uint32 unit, RtkPort port, int8 intval);
EXTERN_API RtkApiRet cvb_ptp_get_logpdelayreqintval_mgtsettable(uint32 unit, RtkPort port, int8 *intval);
EXTERN_API RtkApiRet cvb_ptp_set_rateratio_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_rateratio_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_rateratio_mgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_rateratio_mgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_linkdelay_mgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_linkdelay_mgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_1steptxoper_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_1steptxoper_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_set_1steptxoper_mgtsettable(uint32 unit, RtkPort port, uint32 domain, uint8 txOper);
EXTERN_API RtkApiRet cvb_ptp_get_1steptxoper_mgtsettable(uint32 unit, RtkPort port, uint32 domain, uint8 *txOper);
EXTERN_API RtkApiRet cvb_ptp_set_gptpmsgintval_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_gptpmsgintval_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_get_gptpmsglogintval_mgtsettable(uint32 unit, RtkPort port, uint32 domain, int8 *intval);
EXTERN_API RtkApiRet cvb_ptp_set_gptpmsglogintval_mgtsettable(uint32 unit, RtkPort port, uint32 domain, int8 intval);
EXTERN_API RtkApiRet cvb_ptp_get_init_gptpmsgintval(uint32 unit, RtkPort port, uint32 domain, int8 *intval);
EXTERN_API RtkApiRet cvb_ptp_set_init_gptpmsgintval(uint32 unit, RtkPort port, uint32 domain, int8 intval);
EXTERN_API RtkApiRet cvb_ptp_get_neigptprxtimerout(uint32 unit, RtkPort port, uint8 *timeoutCnt);
EXTERN_API RtkApiRet cvb_ptp_set_neigptprxtimerout(uint32 unit, RtkPort port, uint8 timeoutCnt);
EXTERN_API RtkApiRet cvb_ptp_get_as2020_status(uint32 unit, uint8 *as2020En);
EXTERN_API RtkApiRet cvb_ptp_set_as2020_status(uint32 unit, uint8 as2020En);
EXTERN_API RtkApiRet cvb_ptp_get_syncslowdown_config(uint32 unit, uint8 *syncSlowdownStatus);
EXTERN_API RtkApiRet cvb_ptp_set_syncslowdown_config(uint32 unit, uint8 syncSlowdownStatus);
EXTERN_API RtkApiRet cvb_ptp_set_linkdelay_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet cvb_ptp_get_linkdelay_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_tag_get(uint32 unit, uint32 domainid, ptpqTaginfo *taginfo);
EXTERN_API RtkApiRet cvb_ptp_overwrite_clockid_get(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_get_send_pdelay_from_masterport(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_get_skipfirstbootrecovery(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_get_silenceifneversynced(uint32 unit, uint32 domainid, uint8 *enable);
EXTERN_API RtkApiRet cvb_ptp_get_en_inter_serv_pri(uint32 unit, uint16 *priority_en);
EXTERN_API RtkApiRet cvb_ptp_set_en_inter_serv_pri(uint32 unit, uint16 priority_en);
EXTERN_API RtkApiRet cvb_ptp_get_inter_serv_pri(uint32 unit, uint8 *priority);
EXTERN_API RtkApiRet cvb_ptp_set_inter_serv_pri(uint32 unit, uint8 priority);
EXTERN_API RtkApiRet cvb_ptp_get_isready(uint32 unit, uint32 *isPtpRdy);
EXTERN_API RtkApiRet cvb_ptp_time_direct_get(uint32 unit, uint8 timesel, PtpTs_t *ts);
/********* END  TIME SYNC/PTP *****************/

#endif /* CVB_RTK_API_PTP_H__ */