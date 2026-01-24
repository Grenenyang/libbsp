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

#ifndef DVA_RTK_API_PTP_H__
#define DVA_RTK_API_PTP_H__

#include "rtl907xd_va_api.h"
#include "rtkas_api_ptp.h"

#define DVA_CFiled_t_LENGTH_IN_FW (12u)
#define DVA_MIN_TIMEOUT           (1u)
#define DVA_MAX_SLAVE_DOMAIN      (3u)
#define DVA_PTP_MAX_ACL_RULE      (3u)
#define DVA_PTP_DOMAIN0           (0u)
#define DVA_PTP_DOMAIN1           (1u)
#define DVA_PTP_DOMAIN2           (2u)

#define DVA_TX_LATENCY_RTPGE_1000BASE_T1             (1073u)
#define DVA_TX_LATENCY_RTPGE_100BASE_T1              (1625u)
#define DVA_TX_LATENCY_FAST_ETHERNET_100             (1376u)
#define DVA_TX_LATENCY_FAST_ETHERNET_10              (0u)
#define DVA_TX_LATENCY_FAST_ETHERNET_AN              (0u)
#define DVA_TX_LATENCY_MII_100                       (1196u)
#define DVA_TX_LATENCY_MII_10                        (0u)
#define DVA_TX_LATENCY_RMII_100                      (1196u)
#define DVA_TX_LATENCY_RMII_10                       (0u)
#define DVA_TX_LATENCY_RGMII_1000                    (120u)
#define DVA_TX_LATENCY_RGMII_100                     (1200u)
#define DVA_TX_LATENCY_RGMII_10                      (0u)
#define DVA_TX_LATENCY_SGMII_WRAP_1000               (228u)
#define DVA_TX_LATENCY_SGMII_WRAP_100                (1200u)
#define DVA_TX_LATENCY_SGMII_10                      (0u)
#define DVA_TX_LATENCY_SGMII_AN                      (0u)
#define DVA_TX_LATENCY_HISGMII_WRAP_2500             (44u)
#define DVA_TX_LATENCY_HISGMII_WTG_2500              (4u)
#define DVA_TX_LATENCY_USXGMII_WTG_10000             (12u)
#define DVA_TX_LATENCY_USXGMII_WTG_5000              (28u)
#define DVA_TX_LATENCY_USXGMII_WTG_2500              (52u)
#define DVA_TX_LATENCY_USXGMII_AN                    (0u)
#define DVA_TX_LATENCY_SGMII_WTG_1000                (8u)
#define DVA_TX_LATENCY_SGMII_WTG_100                 (80u)
#define DVA_TX_LATENCY_BASEKR_10000                  (16u)
#define DVA_TX_LATENCY_BASEKR_5000                   (28u)
#define DVA_TX_LATENCY_XFI_WTG_10000                 (131u)
#define DVA_TX_LATENCY_XFI_WTG_5000                  (28u)
#define DVA_TX_LATENCY_MACPHY                        (0u)
#define DVA_TX_LATENCY_GIGABIT_ETHERNET              (0u)
#define DVA_TX_LATENCY_RTPGE_1000BASE_T1_WITH_MACSEC (1209u)
#define DVA_TX_LATENCY_RTPGE_100BASE_T1_WITH_MACSEC  (10081u)
#define DVA_TX_LATENCY_MII_100_WITH_MACSEC           (9840u)
#define DVA_TX_LATENCY_RMII_100_WITH_MACSEC          (9840u)
#define DVA_TX_LATENCY_RGMII_1000_WITH_MACSEC        (984u)
#define DVA_TX_LATENCY_RGMII_100_WITH_MACSEC         (9840u)
#define DVA_TX_LATENCY_SGMII_WRAP_1000_WITH_MACSEC   (984u)
#define DVA_TX_LATENCY_SGMII_WRAP_100_WITH_MACSEC    (9840u)
#define DVA_TX_LATENCY_HISGMII_WRAP_2500_WITH_MACSEC (392u)
#define DVA_TX_LATENCY_NOT_SUPPORT                   (65535u)

#define DVA_RX_LATENCY_RTPGE_1000BASE_T1             (4719u)
#define DVA_RX_LATENCY_RTPGE_100BASE_T1              (1305u)
#define DVA_RX_LATENCY_FAST_ETHERNET_100             (500u)
#define DVA_RX_LATENCY_FAST_ETHERNET_10              (0u)
#define DVA_RX_LATENCY_FAST_ETHERNET_AN              (0u)
#define DVA_RX_LATENCY_MII_100                       (276u)
#define DVA_RX_LATENCY_MII_10                        (0u)
#define DVA_RX_LATENCY_RMII_100                      (316u)
#define DVA_RX_LATENCY_RMII_10                       (0u)
#define DVA_RX_LATENCY_RGMII_1000                    (16u)
#define DVA_RX_LATENCY_RGMII_100                     (620u)
#define DVA_RX_LATENCY_RGMII_10                      (0u)
#define DVA_RX_LATENCY_SGMII_WRAP_1000               (165u)
#define DVA_RX_LATENCY_SGMII_WRAP_100                (156u)
#define DVA_RX_LATENCY_SGMII_10                      (0u)
#define DVA_RX_LATENCY_SGMII_AN                      (0u)
#define DVA_RX_LATENCY_HISGMII_WRAP_2500             (4u)
#define DVA_RX_LATENCY_HISGMII_WTG_2500              (4u)
#define DVA_RX_LATENCY_USXGMII_WTG_10000             (64u)
#define DVA_RX_LATENCY_USXGMII_WTG_5000              (140u)
#define DVA_RX_LATENCY_USXGMII_WTG_2500              (256u)
#define DVA_RX_LATENCY_USXGMII_AN                    (0u)
#define DVA_RX_LATENCY_SGMII_WTG_1000                (12u)
#define DVA_RX_LATENCY_SGMII_WTG_100                 (156u)
#define DVA_RX_LATENCY_BASEKR_10000                  (68u)
#define DVA_RX_LATENCY_BASEKR_5000                   (140u)
#define DVA_RX_LATENCY_XFI_WTG_10000                 (151u)
#define DVA_RX_LATENCY_XFI_WTG_5000                  (140u)
#define DVA_RX_LATENCY_MACPHY                        (0u)
#define DVA_RX_LATENCY_GIGABIT_ETHERNET              (0u)
#define DVA_RX_LATENCY_RTPGE_1000BASE_T1_WITH_MACSEC (5615u)
#define DVA_RX_LATENCY_RTPGE_100BASE_T1_WITH_MACSEC  (10369u)
#define DVA_RX_LATENCY_MII_100_WITH_MACSEC           (9560u)
#define DVA_RX_LATENCY_RMII_100_WITH_MACSEC          (9600u)
#define DVA_RX_LATENCY_RGMII_1000_WITH_MACSEC        (940u)
#define DVA_RX_LATENCY_RGMII_100_WITH_MACSEC         (9860u)
#define DVA_RX_LATENCY_SGMII_WRAP_1000_WITH_MACSEC   (944u)
#define DVA_RX_LATENCY_SGMII_WRAP_100_WITH_MACSEC    (9440u)
#define DVA_RX_LATENCY_HISGMII_WRAP_2500_WITH_MACSEC (376u)
#define DVA_RX_LATENCY_NOT_SUPPORT                   (65535u)

typedef struct
{
    uint8  isEnabled;       /**< [in] Enable to turn on/off PTP ACL(0, 1) */
    uint16 ethertype;       /**< [in] Ethertype configuration. Standard is 0x88F7.(0 to 0xFFFF) */
    uint8 *DA;              /**< [in] Destination address. If DA is 01:80:C2:00:00:0E, DA={0x1,0x8,0xc2,0x0,0x0,0xe},array[5] = 0xe, array[4] = 0, array[3] = 0, array[2] = 0xc2, array[1] = 0x80, array[0] = 0x01 (An array[6]) */
    uint16 spmask;          /**< [in] Source port bit mask to avoid acl redirect. 0: check for this port, 1:Ignore this port. Igrnore internal cpu port mask = 0x8000u.(0x8000) */
    uint8  priority;        /**< [in] The priority assigned to PTP packets. It is recommand to use 6 in 802.1AS(0 to 7) */
    uint32 redirectPortMsk; /**< [in] Redirect port mask. 0: Not redirect to this port, 1:redirect to this port. Redirect to internal cpu port mask = 0x8000u.(0x8000). */
    uint8  ptpAclIdx;       /**< [in] PTP ACL index have to be added. (1 to 3) */
} dva_ptpAclParams_t;

/********* BEGIN TIME SYNC/PTP *****************/
EXTERN_API RtkApiRet dva_ptp_get_interface_latency(uint32 unit, uint8 direction, PtpPortType_e interface, uint16 *latency);
EXTERN_API RtkApiRet dva_ptp_log_to_intval(uint32 unit, int8 const loginterval, uint16 *intval);
EXTERN_API RtkApiRet dva_ptp_get_port_time_sync_en(uint32 unit, RtkPort port, uint8 *ptimeSyncEn);
EXTERN_API RtkApiRet dva_ptp_set_port_time_sync_en(uint32 unit, RtkPort port, uint8 timeSyncEn);
EXTERN_API RtkApiRet dva_ptp_get_hw_enable(uint32 unit, uint8 *pptpEnable);
EXTERN_API RtkApiRet dva_ptp_set_hw_enable(uint32 unit, uint8 ptpEnable);
EXTERN_API RtkApiRet dva_ptp_get_sys_time_en(uint32 unit, uint8 *psysTimeEn);
EXTERN_API RtkApiRet dva_ptp_set_sys_time_en(uint32 unit, uint8 sysTimeEn);
EXTERN_API RtkApiRet dva_ptp_get_tai_pps_en(uint32 unit, uint8 ppsidx, uint8 *pppsEn);
EXTERN_API RtkApiRet dva_ptp_set_sys_pps_en(uint32 unit, uint8 ppsidx, uint8 ppsEn);
EXTERN_API RtkApiRet dva_ptp_get_redirect_cpumask(uint32 unit, uint16 *pcpumask);
EXTERN_API RtkApiRet dva_ptp_set_redirect_cpumask(uint32 unit, uint16 cpumask);
EXTERN_API RtkApiRet dva_ptp_get_redirect_act(uint32 unit, uint8 *pptpRedirAct);
EXTERN_API RtkApiRet dva_ptp_set_redirect_act(uint32 unit, uint8 ptpRedirAct);
EXTERN_API RtkApiRet dva_ptp_get_master_en(uint32 unit, uint8 *pmasterEn);
EXTERN_API RtkApiRet dva_ptp_set_master_en(uint32 unit, uint8 masterEn);
EXTERN_API RtkApiRet dva_ptp_get_slave2_en(uint32 unit, uint8 *pslave2En);
EXTERN_API RtkApiRet dva_ptp_set_slave2_en(uint32 unit, uint8 slave2En);
EXTERN_API RtkApiRet dva_ptp_get_slave1_en(uint32 unit, uint8 *pslave1En);
EXTERN_API RtkApiRet dva_ptp_set_slave1_en(uint32 unit, uint8 slave1En);
EXTERN_API RtkApiRet dva_ptp_get_slave0_en(uint32 unit, uint8 *pslave0En);
EXTERN_API RtkApiRet dva_ptp_set_slave0_en(uint32 unit, uint8 slave0En);
EXTERN_API RtkApiRet dva_ptp_get_ether_check_enable0(uint32 unit, RtkPort port, uint8 *checkEnable);
EXTERN_API RtkApiRet dva_ptp_set_ether_check_enable0(uint32 unit, RtkPort port, uint8 checkEnable);
EXTERN_API RtkApiRet dva_ptp_get_port_ptp_100tx(uint32 unit, RtkPort port, uint8 *pptp100tx);
EXTERN_API RtkApiRet dva_ptp_set_port_ptp_100tx(uint32 unit, RtkPort port, uint8 ptp100tx);
EXTERN_API RtkApiRet dva_ptp_get_port_rxlatency(uint32 unit, RtkPort port, uint16 *prxLatency);
EXTERN_API RtkApiRet dva_ptp_set_port_rxlatency(uint32 unit, RtkPort port, uint16 rxLatency);
EXTERN_API RtkApiRet dva_ptp_get_port_txlatency(uint32 unit, RtkPort port, uint16 *ptxLatency);
EXTERN_API RtkApiRet dva_ptp_set_port_txlatency(uint32 unit, RtkPort port, uint16 txLatency);
EXTERN_API RtkApiRet dva_ptp_get_port_slave_time_domain(uint32 unit, RtkPort port, uint32 domainid, uint8 *domainvalue);
EXTERN_API RtkApiRet dva_ptp_set_port_slave_time_domain(uint32 unit, RtkPort port, uint32 domainid, uint8 domainvalue);
EXTERN_API RtkApiRet dva_ptp_get_curnt_pdelayreq_intval(uint32 unit, RtkPort port, uint32 *curntPdelayReqIntval);
EXTERN_API RtkApiRet dva_ptp_set_curnt_pdelayreq_intval(uint32 unit, RtkPort port, uint32 curntPdelayReqIntval);
EXTERN_API RtkApiRet dva_ptp_get_portmode(uint32 unit, RtkPort port, uint8 *pportmode);
EXTERN_API RtkApiRet dva_ptp_set_portmode(uint32 unit, RtkPort port, uint8 portmode);
EXTERN_API RtkApiRet dva_ptp_get_glenable(uint32 unit, uint8 *glenable);
EXTERN_API RtkApiRet dva_ptp_set_portmask_in_glenable(uint32 unit);
EXTERN_API RtkApiRet dva_ptp_set_glenable(uint32 unit, uint8 glenable);
EXTERN_API RtkApiRet dva_ptp_get_cfggm(uint32 unit, uint8 *cfggm);
EXTERN_API RtkApiRet dva_ptp_set_cfggm(uint32 unit, uint8 cfggm);
EXTERN_API RtkApiRet dva_ptp_get_autoasenable(uint32 unit, RtkPort port, uint8 *autoasenable);
EXTERN_API RtkApiRet dva_ptp_set_autoasenable(uint32 unit, RtkPort port, uint8 autoasenable);
EXTERN_API RtkApiRet dva_ptp_get_ppsenable(uint32 unit, uint16 ppsIdx, uint8 *ppsenable);
EXTERN_API RtkApiRet dva_ptp_set_ppsenable(uint32 unit, uint16 ppsIdx, uint8 ppsenable);
EXTERN_API RtkApiRet dva_ptp_get_signalingen(uint32 unit, uint32 domainid, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_signalingen(uint32 unit, uint32 domainid, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_slaveportid(uint32 unit, uint32 domainid, uint8 *slaveportid);
EXTERN_API RtkApiRet dva_ptp_set_slaveportid(uint32 unit, uint32 domainid, uint8 slaveportid);
EXTERN_API RtkApiRet dva_ptp_get_slaveportmask(uint32 unit, uint16 *slaveportmask);
EXTERN_API RtkApiRet dva_ptp_set_slaveportmask(uint32 unit, uint16 slvMask);
EXTERN_API RtkApiRet dva_ptp_get_priority1(uint32 unit, uint32 domainid, uint8 *priority1);
EXTERN_API RtkApiRet dva_ptp_set_priority1(uint32 unit, uint32 domainid, uint8 priority1);
EXTERN_API RtkApiRet dva_ptp_get_delaythresh(uint32 unit, PtpTs_t *delaythresh);
EXTERN_API RtkApiRet dva_ptp_set_delaythresh(uint32 unit, PtpTs_t const *const delaythresh);
EXTERN_API RtkApiRet dva_ptp_get_initlogpdelayintval(uint32 unit, RtkPort port, int8 *pdelayintval);
EXTERN_API RtkApiRet dva_ptp_set_initlogpdelayintval(uint32 unit, RtkPort port, int8 pdelayintval);
EXTERN_API RtkApiRet dva_ptp_get_operlogsyncintval(uint32 unit, uint32 domainid, int8 *syncintval);
EXTERN_API RtkApiRet dva_ptp_set_operlogsyncintval(uint32 unit, uint32 domainid, int8 syncintval);
EXTERN_API RtkApiRet dva_ptp_get_operlogpdelayintval(uint32 unit, RtkPort port, int8 *pdelayintval);
EXTERN_API RtkApiRet dva_ptp_set_operlogpdelayintval(uint32 unit, RtkPort port, int8 pdelayintval);
EXTERN_API RtkApiRet dva_ptp_get_exchangeInfo(uint32 unit, uint8 *exchangeInfo);
EXTERN_API RtkApiRet dva_ptp_set_exchangeInfo(uint32 unit, uint8 exchangeInfo);
EXTERN_API RtkApiRet dva_ptp_get_firstbootuptimetarget(uint32 unit, uint16 *timetarget);
EXTERN_API RtkApiRet dva_ptp_set_firstbootuptimetarget(uint32 unit, uint16 timetarget);
EXTERN_API RtkApiRet dva_ptp_get_curntsyncintval(uint32 unit, RtkPort port, uint32 domainid, uint32 *curntsyncintval);
EXTERN_API RtkApiRet dva_ptp_set_curntsyncintval(uint32 unit, RtkPort port, uint32 domainid, uint32 curntsyncintval);
EXTERN_API RtkApiRet dva_ptp_get_sw_portmode(uint32 unit, RtkPort port, uint32 domainid, uint8 *portmode);
EXTERN_API RtkApiRet dva_ptp_set_sw_portmode(uint32 unit, RtkPort port, uint32 domainid, uint8 portmode);
EXTERN_API RtkApiRet dva_ptp_get_en_portremapping(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_en_portremapping(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_onesteptxsync(uint32 unit, RtkPort port, uint8 *oneStepTxSync);
EXTERN_API RtkApiRet dva_ptp_set_onesteptxsync(uint32 unit, RtkPort port, uint8 oneStepTxSync);
EXTERN_API RtkApiRet dva_ptp_get_initlogsyncintval(uint32 unit, RtkPort port, uint32 domainid, int8 *initLogSyncIntval);
EXTERN_API RtkApiRet dva_ptp_set_initlogsyncintval(uint32 unit, RtkPort port, uint32 domainid, int8 initLogSyncIntval);
EXTERN_API RtkApiRet dva_ptp_get_domainnum(uint32 unit, RtkPort port, uint32 domainid, uint8 *domainnum);
EXTERN_API RtkApiRet dva_ptp_set_domainnum(uint32 unit, RtkPort port, uint32 domainid, uint8 domainnum);
EXTERN_API RtkApiRet dva_ptp_get_domainmap(uint32 unit, uint32 domainid, uint16 *domainmap);
EXTERN_API RtkApiRet dva_ptp_set_domainmap(uint32 unit, uint32 domainid, uint16 domainmap);
EXTERN_API RtkApiRet dva_ptp_get_portremapping(uint32 unit, RtkPort port, RtkPort *remapping);
EXTERN_API RtkApiRet dva_ptp_set_portremapping(uint32 unit, RtkPort port, RtkPort remapping);
EXTERN_API RtkApiRet dva_ptp_set_custom_ethertype(uint32 unit, RtkPort port, uint16 ethertype);
EXTERN_API RtkApiRet dva_ptp_get_custom_ethertype(uint32 unit, RtkPort port, uint16 *ethertype);
EXTERN_API RtkApiRet dva_ptp_set_ethertype_enable_custom(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_ethertype_enable_custom(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_custom_set_vlan_enable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_custom_get_vlan_enable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_custom_damac01(uint32 unit, uint32 domainid, RtkPort port, uint8 const *const damac);
EXTERN_API RtkApiRet dva_ptp_get_custom_damac01(uint32 unit, uint32 domainid, RtkPort port, uint8 *damac);
EXTERN_API RtkApiRet dva_ptp_set_damac01_enable_custom(uint32 unit, uint32 domainid, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_damac01_enable_custom(uint32 unit, uint32 domainid, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_clkstable_get(uint32 unit, uint32 domainid, uint8 *clkstable);
EXTERN_API RtkApiRet dva_ptp_reinitprotocol(uint32 unit);
EXTERN_API RtkApiRet dva_ptp_protocolportmask_set(uint32 unit, uint32 domainid, uint16 const *const portmask);
EXTERN_API RtkApiRet dva_ptp_set_skipfirstbootrecovery(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_set_silenceifneversynced(uint32 unit, uint32 domainid, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_overwrite_ptpid_set(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_set_send_pdelay_from_masterport(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_onesteptxen_sw_set(uint32 unit, RtkPort port, uint32 domainid, uint8 oneStepTxSync);
EXTERN_API RtkApiRet dva_ptp_tag_set(uint32 unit, uint32 domainid, ptpqTaginfo *taginfo);
EXTERN_API RtkApiRet dva_ptp_rx_sync_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_rx_follow_up_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_rx_pdelay_request_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_rx_pdelay_response_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_rx_pdelay_resp_follow_up_mib_get(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_rx_ptp_packet_discard_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_sync_receipt_timeout_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_pdelay_allowed_lost_response_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_tx_sync_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_tx_follow_up_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_tx_pdelay_request_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_tx_pdelay_response_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_tx_pdelay_resp_follow_up_mib_get(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_ptp_onesteprxen_sw_set(uint32 unit, RtkPort port, uint32 domainid, uint8 oneStepRxSync);
EXTERN_API RtkApiRet dva_ptp_set_initonesteptxoper(uint32 unit, RtkPort port, uint32 domainid, uint8 initOneStepTXOperEn);
EXTERN_API RtkApiRet dva_ptp_get_initonesteptxoper(uint32 unit, RtkPort port, uint32 domainid, uint8 *initOneStepTXOperEn);
EXTERN_API RtkApiRet dva_ptp_get_flash_runtime_access(uint32 unit, uint8 *flashAccess);
EXTERN_API RtkApiRet dva_ptp_set_flash_runtime_access(uint32 unit, uint8 flashAccess);
EXTERN_API RtkApiRet dva_ptp_set_pdelay_wrflash_thres(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet dva_ptp_get_pdelay_wrflash_thres(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet dva_ptp_set_synclock_config(uint32 unit, uint8 synclockStatus);
EXTERN_API RtkApiRet dva_ptp_get_synclock_config(uint32 unit, uint8 *synclockStatus);
EXTERN_API RtkApiRet dva_ptp_set_allowedfaults(uint32 unit, RtkPort port, uint8 allowedfaults);
EXTERN_API RtkApiRet dva_ptp_get_allowedfaults(uint32 unit, RtkPort port, uint8 *allowedfaults);
EXTERN_API RtkApiRet dva_ptp_set_delaymeasure_mech(uint32 unit, uint32 mechnisam);
EXTERN_API RtkApiRet dva_ptp_get_delaymeasure_mech(uint32 unit, uint32 *mechnisam);
EXTERN_API RtkApiRet dva_ptp_set_delay_asymmetry(uint32 unit, RtkPort port, CFiled_t delayAsymmetry);
EXTERN_API RtkApiRet dva_ptp_get_delay_asymmetry(uint32 unit, RtkPort port, CFiled_t *delayAsymmetry);
EXTERN_API RtkApiRet dva_ptp_get_use_mgtsettable_logsyncintval(uint32 unit, uint32 domain, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_use_mgtsettable_logsyncintval(uint32 unit, uint32 domain, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_set_logsyncintval_mgtsettable(uint32 unit, uint32 domain, int8 logIntval);
EXTERN_API RtkApiRet dva_ptp_get_logsyncintval_mgtsettable(uint32 unit, uint32 domain, int8 *logIntval);
EXTERN_API RtkApiRet dva_ptp_get_usemgtsettable_logpdelayreqintval(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_usemgtsettable_logpdelayreqintval(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_set_logpdelayreqintval_mgtsettable(uint32 unit, RtkPort port, int8 intval);
EXTERN_API RtkApiRet dva_ptp_get_logpdelayreqintval_mgtsettable(uint32 unit, RtkPort port, int8 *intval);
EXTERN_API RtkApiRet dva_ptp_set_rateratio_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_rateratio_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_rateratio_mgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_rateratio_mgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_linkdelay_mgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_linkdelay_mgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_1steptxoper_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_1steptxoper_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_set_1steptxoper_mgtsettable(uint32 unit, RtkPort port, uint32 domain, uint8 txOper);
EXTERN_API RtkApiRet dva_ptp_get_1steptxoper_mgtsettable(uint32 unit, RtkPort port, uint32 domain, uint8 *txOper);
EXTERN_API RtkApiRet dva_ptp_set_gptpmsgintval_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_gptpmsgintval_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_get_gptpmsglogintval_mgtsettable(uint32 unit, RtkPort port, uint32 domain, int8 *intval);
EXTERN_API RtkApiRet dva_ptp_set_gptpmsglogintval_mgtsettable(uint32 unit, RtkPort port, uint32 domain, int8 intval);
EXTERN_API RtkApiRet dva_ptp_get_init_gptpmsgintval(uint32 unit, RtkPort port, uint32 domain, int8 *intval);
EXTERN_API RtkApiRet dva_ptp_set_init_gptpmsgintval(uint32 unit, RtkPort port, uint32 domain, int8 intval);
EXTERN_API RtkApiRet dva_ptp_get_neigptprxtimerout(uint32 unit, RtkPort port, uint8 *timeoutCnt);
EXTERN_API RtkApiRet dva_ptp_set_neigptprxtimerout(uint32 unit, RtkPort port, uint8 timeoutCnt);
EXTERN_API RtkApiRet dva_ptp_get_as2020_status(uint32 unit, uint8 *as2020En);
EXTERN_API RtkApiRet dva_ptp_set_as2020_status(uint32 unit, uint8 as2020En);
EXTERN_API RtkApiRet dva_ptp_get_syncslowdown_config(uint32 unit, uint8 *syncSlowdownStatus);
EXTERN_API RtkApiRet dva_ptp_set_syncslowdown_config(uint32 unit, uint8 syncSlowdownStatus);
EXTERN_API RtkApiRet dva_ptp_set_linkdelay_usemgtsettable(uint32 unit, RtkPort port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_get_linkdelay_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_tag_get(uint32 unit, uint32 domainid, ptpqTaginfo *taginfo);
EXTERN_API RtkApiRet dva_ptp_overwrite_clockid_get(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_get_send_pdelay_from_masterport(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_get_skipfirstbootrecovery(uint32 unit, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_get_silenceifneversynced(uint32 unit, uint32 domainid, uint8 *enable);
EXTERN_API RtkApiRet dva_ptp_get_en_inter_serv_pri(uint32 unit, uint16 *priority_en);
EXTERN_API RtkApiRet dva_ptp_set_en_inter_serv_pri(uint32 unit, uint16 priority_en);
EXTERN_API RtkApiRet dva_ptp_get_inter_serv_pri(uint32 unit, uint8 *priority);
EXTERN_API RtkApiRet dva_ptp_set_inter_serv_pri(uint32 unit, uint8 priority);
EXTERN_API RtkApiRet dva_ptp_time_direct_get(uint32 unit, uint8 timesel, PtpTs_t *ts);
EXTERN_API RtkApiRet dva_ptp_get_isready(uint32 unit, uint32 *isPtpRdy);
EXTERN_API RtkApiRet dva_ptp_set_domain_profiles(uint32 unit, uint32 domainid, PtpTpSupportProfiles_e profiles);

EXTERN_API RtkApiRet dva_ptp_get_sync_receipt_timeout(uint32 unit, uint32 domainid, uint8 *syncReceiptTimeout);
EXTERN_API RtkApiRet dva_ptp_set_sync_receipt_timeout(uint32 unit, uint32 domainid, uint8 syncReceiptTimeout);
EXTERN_API RtkApiRet dva_ptp_get_allowed_lost_presp_num(uint32 unit, uint8 *allowedLostResponses);
EXTERN_API RtkApiRet dva_ptp_set_allowed_lost_presp_num(uint32 unit, uint8 allowedLostResponses);
EXTERN_API RtkApiRet dva_ptp_neidelay_get(uint32 unit, uint32 port, uint32 *neiDelay);
EXTERN_API RtkApiRet dva_ptp_neidelay_set(uint32 unit, uint32 port, uint32 neiDelay);
EXTERN_API RtkApiRet dva_ptp_caldelayen_set(uint32 unit, uint32 port, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_crc_support(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_frame_pri(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_tx_pdelayreq_period(uint32 unit, uint32 pdelayIntval);
EXTERN_API RtkApiRet dva_ptp_atsar_set_pdelay_latency_thresh(uint32 unit, uint32 delayThresh);
EXTERN_API RtkApiRet dva_ptp_atsar_set_prop_delay(uint32 unit, uint32 pDelay);
EXTERN_API RtkApiRet dva_ptp_atsar_set_tx_period(uint32 unit, uint32 intval);
EXTERN_API RtkApiRet dva_ptp_atsar_set_msg_compliance(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_rx_crc_validated(uint32 unit, PtpTpCrcValidated_e validated);
EXTERN_API RtkApiRet dva_ptp_atsar_set_crc_flags(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_time_subtlv(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_status_subtlv(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_udata_subtlv(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_ofs_subtlv(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_tx_crc_time_flags(uint32 unit, uint8 flags);
EXTERN_API RtkApiRet dva_ptp_atsar_set_tx_time_crc_secured(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_atsar_set_pdelay_resp_enable(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_ptp_tag_vid_pri_set(uint32 unit, uint32 domainid, ptpNewQTaginfo *taginfo);
EXTERN_API RtkApiRet dva_ptp_atsar_set_foloup_dataid_list(uint32 unit, uint8 dataid[16]);
EXTERN_API RtkApiRet dva_ptp_atsar_set_ofs_multicity(uint32 unit, uint8 ofsMulti);
EXTERN_API RtkApiRet dva_ptp_intval_to_log(uint32 unit, PtpIntvaType_e const curntIntval, int8 *logIntval);

/********* END  TIME SYNC/PTP *****************/

#endif /* DVA_RTK_API_PTP_H__ */