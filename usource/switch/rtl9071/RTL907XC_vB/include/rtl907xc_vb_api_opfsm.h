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

#ifndef CVB_RTK_API_OPFSM_H__
#define CVB_RTK_API_OPFSM_H__

#define CVB_OP_VIRINTERUPT_POLLING_DISABLE (0x01u) /**< OPFSM_INTR_BY_OPFSM */
#define CVB_OP_VIRINTERUPT_POLLING_ENABLE  (0x02u) /**< OPFSM_INTR_BY_SW */

#define CVB_OP_RTPGE_USER_PORTMAP (0x60u) /**< user port map of RTPGE */
#define CVB_OP_BWPHY_USER_PORTMAP (0x1Eu) /**< user port map of BWPHY */

#define CVB_OP_T1_USER_PORT_MIN (CVB_RTK_USER_PORT_1ST) /**< Min user port index with T1PHY */
#define CVB_OP_T1_USER_PORT_MAX (CVB_RTK_USER_PORT_6TH) /**< Max user port index with T1PHY */

EXTERN_API RtkApiRet cvb_op_get_phy_sleep_cap(uint32 unit, RtkEnable sleepCap, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_phy_sleep_cap(uint32 unit, RtkPmap portMap, RtkEnable sleepCap);
EXTERN_API RtkApiRet cvb_op_get_global_state(uint32 unit, uint16 *pState);
EXTERN_API RtkApiRet cvb_op_set_global_state(uint32 unit, uint16 state);
EXTERN_API RtkApiRet cvb_op_get_port_local_state(uint32 unit, RtkPort portId, uint16 *pState);
EXTERN_API RtkApiRet cvb_op_set_port_local_state(uint32 unit, RtkPort portId, uint16 state);
EXTERN_API RtkApiRet cvb_op_get_portmap_local_state(uint32 unit, uint16 state, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_portmap_local_state(uint32 unit, RtkPmap portMap, uint16 state);
EXTERN_API RtkApiRet cvb_op_set_accept_sleep_req_cap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue);
EXTERN_API RtkApiRet cvb_op_get_accept_sleep_req_cap(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_txsleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue);
EXTERN_API RtkApiRet cvb_op_get_txsleep_portmap(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_en_wholechip_sleep(uint32 unit, RtkEnable wholeChipOn);
EXTERN_API RtkApiRet cvb_op_get_en_wholechip_sleep(uint32 unit, RtkEnable *pWholeChipOn);
EXTERN_API RtkApiRet cvb_op_set_wholechip_sleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue);
EXTERN_API RtkApiRet cvb_op_get_wholechip_sleep_portmap(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_port_send_sleep_request(uint32 unit, RtkPmap portMap);
EXTERN_API RtkApiRet cvb_op_set_sleep_event_intr_en(uint32 unit, RtkEnable extIntrOn);
EXTERN_API RtkApiRet cvb_op_get_sleep_event_intr_en(uint32 unit, RtkEnable *pExtIntrOn);
EXTERN_API RtkApiRet cvb_op_get_rx_sleep_flag(uint32 unit, uint16 *pRxSlpFlag);
EXTERN_API RtkApiRet cvb_op_get_sleep_fail_flag(uint32 unit, uint16 *pSlpFailFlag);
EXTERN_API RtkApiRet cvb_op_get_sleep_success_flag(uint32 unit, uint16 *pSlpSuccessFlag);
EXTERN_API RtkApiRet cvb_op_port_send_rwake(uint32 unit, RtkPmap portMap);
EXTERN_API RtkApiRet cvb_op_set_disable_rwake_rx(uint32 unit, RtkPmap portMap, uint8 cfgValue);
EXTERN_API RtkApiRet cvb_op_get_disable_rwake_rx(uint32 unit, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_lwake_wake_port(uint32 unit, RtkPmap portMap, RtkEnable cfgValue);
EXTERN_API RtkApiRet cvb_op_get_lwake_wake_port(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_en_wupr_tx(uint32 unit, RtkPmap portMap, RtkEnable cfgValue);
EXTERN_API RtkApiRet cvb_op_get_en_wupr_tx(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_wur_ext_intr_msk(uint32 unit, RtkPmap portMap, uint8 cfgValue);
EXTERN_API RtkApiRet cvb_op_get_wur_ext_intr_msk(uint32 unit, uint8 cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_wup_ext_intr_msk(uint32 unit, RtkPmap portMap, uint8 cfgValue);
EXTERN_API RtkApiRet cvb_op_get_wup_ext_intr_msk(uint32 unit, uint8 cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_event_ext_intr_msk(uint32 unit, uint16 cfgValue);
EXTERN_API RtkApiRet cvb_op_get_event_ext_intr_msk(uint32 unit, uint16 *pCfgValue);
EXTERN_API RtkApiRet cvb_op_get_wup_flag(uint32 unit, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_get_wur_flag(uint32 unit, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_wakeflag_clearall(uint32 unit);
EXTERN_API RtkApiRet cvb_op_set_lwake_edge_sel(uint32 unit, uint16 lwakeEdge);
EXTERN_API RtkApiRet cvb_op_get_lwake_edge_sel(uint32 unit, uint16 *pLwakeEdge);
EXTERN_API RtkApiRet cvb_op_set_portmap_fwd_rwake_to_port(uint32 unit, RtkPort portSel, RtkPmap portMap, RtkEnable cfgValue);
EXTERN_API RtkApiRet cvb_op_get_portmap_fwd_rwake_to_port(uint32 unit, RtkPort portSel, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_wakeo_portmap(uint32 unit, uint8 configIdx, RtkPmap portMap, RtkEnable cfgValue);
EXTERN_API RtkApiRet cvb_op_get_wakeo_portmap(uint32 unit, uint8 configIdx, RtkEnable cfgValue, RtkPmap *pPortMap);
EXTERN_API RtkApiRet cvb_op_set_wakeo_pulse(uint32 unit, uint16 cfgValue);
EXTERN_API RtkApiRet cvb_op_get_wakeo_pulse(uint32 unit, uint16 *pCfgValue);

#endif /*CVB_RTK_API_OPFSM_H__*/
