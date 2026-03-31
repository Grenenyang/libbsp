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

#ifndef CVA_ASICDRV_MACSEC_H_
#define CVA_ASICDRV_MACSEC_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_std.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_va_api.h"
#include "rtl907xc_va_api_macsec.h"
#include "rtl907xc_va_asicdrv_macsec.h"
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CVA_MACSEC_SECY_SA_MATCH_SOURCE_MAC_MASK      ((uint32)0x3Fu << 0u)
#define CVA_MACSEC_SECY_SA_MATCH_DESTINATION_MAC_MASK ((uint32)0x3Fu << 6u)
#define CVA_MACSEC_SECY_SA_MATCH_ETHER_TYPE_MASK      ((uint32)0x1u << 12u)
#define CVA_MACSEC_SECY_SA_MATCH_OUTER_VLAN_UP_MASK   ((uint32)0x1u << 17u)
#define CVA_MACSEC_SECY_SA_MATCH_OUTER_VLAN_ID_MASK   ((uint32)0x1u << 18u)
#define CVA_MACSEC_SECY_SA_MATCH_SOURCE_PORT_MASK     ((uint32)0x1u << 19u)
#define CVA_MACSEC_SECY_SA_MATCH_INNER_VLAN_UP_MASK   ((uint32)0x1u << 21u)
#define CVA_MACSEC_SECY_SA_MATCH_INNER_VLAN_ID_MASK   ((uint32)0x1u << 22u)
#define CVA_MACSEC_SECY_SA_MATCH_SCI_MASK             ((uint32)0x1u << 23u)
#define CVA_MACSEC_SECY_SA_MATCH_AN_0_MASK            ((uint32)0x1u << 24u)
#define CVA_MACSEC_SECY_SA_MATCH_AN_1_MASK            ((uint32)0x1u << 25u)

#define CVA_MACSEC_SECY_RX_GLOBAL_TRANSFORM_ERROR_PKTS                   (0u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_CTRL                           (1u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_NO_TAG                         (2u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNTAGGED                       (3u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_TAGGED                         (4u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_BAD_TAG                        (5u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNTAGGED_MISS                  (6u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_NO_SCI                         (7u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNKNOW_SCI                     (8u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_CONTROLLED_NOT_PASS   (9u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_UNCONTROLLED_NOT_PASS (10u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_CONTROLLED_PASS       (11u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_UNCONTROLLED_PASS     (12u)
#define CVA_MACSEC_SECY_RX_GLOBAL_IN_OVER_SIZE                           (13u)

#define CVA_MACSEC_SECY_RX_SA_IN_OCTETS_DECRYPYED  (0u)
#define CVA_MACSEC_SECY_RX_SA_IN_RESERVED          (1u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_UNCHECKED    (2u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_DELAYED      (3u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_LATE         (4u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_OK           (5u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_INVALID      (6u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_NOT_VALID    (7u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_NOT_USING_SA (8u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_UNUSED_SA    (9u)
#define CVA_MACSEC_SECY_RX_SA_IN_PKTS_UNTAGGED_HIT (10u)

#define CVA_MACSEC_SECY_TX_GLOBAL_TRANSFORM_ERROR_PKTS (0u)
#define CVA_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_CTRL        (1u)
#define CVA_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_UNKNOWNSA   (2u)
#define CVA_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_UNTAGGED    (3u)
#define CVA_MACSEC_SECY_TX_GLOBAL_OUT_OVERSIZE_PKTS    (4u)

#define CVA_MACSEC_SECY_TX_SA_OUT_OCTETS_ENCRYPTED (0u)
#define CVA_MACSEC_SECY_TX_SA_OUT_RESERVED         (1u)
#define CVA_MACSEC_SECY_TX_SA_OUT_PKTS_ENCRYPTED   (2u)
#define CVA_MACSEC_SECY_TX_SA_OUT_PKTS_TOOLONG     (3u)

#define CVA_MACSEC_SECY_FLOW_CTRL_FLOW_TYPE_RX (2u)
#define CVA_MACSEC_SECY_FLOW_CTRL_FLOW_TYPE_TX (3u)

#define CVA_MACSEC_SECY_FLOW_CTRL_DEST_PORT_COMMON       (0u)
#define CVA_MACSEC_SECY_FLOW_CTRL_DEST_PORT_CONTROLLED   (2u)
#define CVA_MACSEC_SECY_FLOW_CTRL_DEST_PORT_UNCONTROLLED (3u)
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
typedef struct
{
    bool vlanValid;
    bool qInQValid;
    bool stagValid;
    bool qtagValid;
} cva_MacsecSecyVlanTagValid_t;

typedef struct
{
    uint8 macsecClass;
    bool  secTagValid;
    bool  sciValid;
    bool  macsecValid;
} cva_MacsecSecySectagIdentified_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
/* SA Match Rule */
RtkApiRet cva_drv_secy_mr_mac_sa_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *macSa);
RtkApiRet cva_drv_secy_mr_mac_sa_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *macSa);
RtkApiRet cva_drv_secy_mr_ethertype_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 etherType);
RtkApiRet cva_drv_secy_mr_ethertype_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *etherType);
RtkApiRet cva_drv_secy_mr_mac_da_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *macDa);
RtkApiRet cva_drv_secy_mr_mac_da_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *macDa);
RtkApiRet cva_drv_secy_mr_vlan_id_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanId);
RtkApiRet cva_drv_secy_mr_vlan_id_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanId);
RtkApiRet cva_drv_secy_mr_an_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 an);
RtkApiRet cva_drv_secy_mr_an_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *an);
RtkApiRet cva_drv_secy_mr_prioirty_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 priority);
RtkApiRet cva_drv_secy_mr_prioirty_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *priority);
RtkApiRet cva_drv_secy_mr_source_port_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 sourcePort);
RtkApiRet cva_drv_secy_mr_source_port_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *sourcePort);
RtkApiRet cva_drv_secy_mr_vlan_up_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanUserPriority);
RtkApiRet cva_drv_secy_mr_vlan_up_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanUserPriority);
RtkApiRet cva_drv_secy_mr_inner_vlan_id_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanId);
RtkApiRet cva_drv_secy_mr_inner_vlan_id_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanId);
RtkApiRet cva_drv_secy_mr_inner_vlan_up_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanUserPriority);
RtkApiRet cva_drv_secy_mr_inner_vlan_up_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanUserPriority);
RtkApiRet cva_drv_secy_mr_sci_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *sci);
RtkApiRet cva_drv_secy_mr_sci_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *sci);
RtkApiRet cva_drv_secy_mr_flow_ctrl_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 flowCtrlIdx);
RtkApiRet cva_drv_secy_mr_flow_ctrl_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *flowCtrlIdx);
RtkApiRet cva_drv_secy_mr_mask_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 mask);
RtkApiRet cva_drv_secy_mr_mask_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *mask);
RtkApiRet cva_drv_secy_mr_entry_enable_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool enable);
RtkApiRet cva_drv_secy_mr_entry_enable_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *enable);

/* Flow Control */
RtkApiRet cva_drv_secy_fc_common_params_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecFlowControl_t *params);
RtkApiRet cva_drv_secy_fc_common_params_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecFlowControl_t *params);
RtkApiRet cva_drv_secy_fc_rx_params_set(uint32 unit, RtkPort sysPort, uint8 entryIdx, const RtkMacsecFlowControl_t *params);
RtkApiRet cva_drv_secy_fc_rx_params_get(uint32 unit, RtkPort sysPort, uint8 entryIdx, RtkMacsecFlowControl_t *params);
RtkApiRet cva_drv_secy_fc_tx_params_set(uint32 unit, RtkPort sysPort, uint8 entryIdx, const RtkMacsecFlowControl_t *params);
RtkApiRet cva_drv_secy_fc_tx_params_get(uint32 unit, RtkPort sysPort, uint8 entryIdx, RtkMacsecFlowControl_t *params);
RtkApiRet cva_drv_secy_fc_flow_type_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 flowType);
RtkApiRet cva_drv_secy_fc_flow_type_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *flowType);
RtkApiRet cva_drv_secy_fc_dest_port_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 srcPort);
RtkApiRet cva_drv_secy_fc_dest_port_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *srcPort);
RtkApiRet cva_drv_secy_fc_rx_rp_set(uint32 unit, RtkPort sysPort, uint8 index, bool replayProtect);
RtkApiRet cva_drv_secy_fc_rx_rp_get(uint32 unit, RtkPort sysPort, uint8 index, bool *replayProtect);
RtkApiRet cva_drv_secy_fc_rx_vf_set(uint32 unit, RtkPort sysPort, uint8 index, uint32 validateFrames);
RtkApiRet cva_drv_secy_fc_rx_vf_get(uint32 unit, RtkPort sysPort, uint8 index, uint32 *validateFrames);
RtkApiRet cva_drv_secy_fc_tx_pf_set(uint32 unit, RtkPort sysPort, uint8 index, bool protectFrame);
RtkApiRet cva_drv_secy_fc_tx_pf_get(uint32 unit, RtkPort sysPort, uint8 index, bool *protectFrame);
RtkApiRet cva_drv_secy_fc_tx_incl_sci_set(uint32 unit, RtkPort sysPort, uint8 index, bool includeSci);
RtkApiRet cva_drv_secy_fc_tx_incl_sci_get(uint32 unit, RtkPort sysPort, uint8 index, bool *includeSci);
RtkApiRet cva_drv_secy_fc_tx_use_es_set(uint32 unit, RtkPort sysPort, uint8 index, bool useEs);
RtkApiRet cva_drv_secy_fc_tx_use_es_get(uint32 unit, RtkPort sysPort, uint8 index, bool *useEs);
RtkApiRet cva_drv_secy_fc_tx_use_scb_set(uint32 unit, RtkPort sysPort, uint8 index, bool useScb);
RtkApiRet cva_drv_secy_fc_tx_use_scb_get(uint32 unit, RtkPort sysPort, uint8 index, bool *useScb);
RtkApiRet cva_drv_secy_fc_tx_cp_set(uint32 unit, RtkPort sysPort, uint8 index, bool confProtect);
RtkApiRet cva_drv_secy_fc_tx_cp_get(uint32 unit, RtkPort sysPort, uint8 index, bool *confProtect);
RtkApiRet cva_drv_secy_fc_tx_tb_set(uint32 unit, RtkPort sysPort, uint8 index, uint32 tagBypassSize);
RtkApiRet cva_drv_secy_fc_tx_tb_get(uint32 unit, RtkPort sysPort, uint8 index, uint32 *tagBypassSize);
RtkApiRet cva_drv_secy_fc_sa_index_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 saIndex);
RtkApiRet cva_drv_secy_fc_sa_index_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *saIndex);
RtkApiRet cva_drv_secy_fc_sa_in_use_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool saInUse);
RtkApiRet cva_drv_secy_fc_sa_in_use_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *saInUse);
RtkApiRet cva_drv_secy_fc_conf_offset_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 confOffset);
RtkApiRet cva_drv_secy_fc_conf_offset_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *confOffset);

/* Xform Record */

RtkApiRet cva_drv_secy_sa_init(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 an);
RtkApiRet cva_drv_secy_sa_an_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *an);
RtkApiRet cva_drv_secy_sa_key_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, const uint8 *sak, const uint8 *hkey);
RtkApiRet cva_drv_secy_sa_key_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 *sak, uint8 *hkey);
RtkApiRet cva_drv_secy_sa_sci_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, const uint8 *sci);
RtkApiRet cva_drv_secy_sa_sci_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 *sci);
RtkApiRet cva_drv_secy_sa_seq_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint64 seq);
RtkApiRet cva_drv_secy_sa_seq_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint64 *seq);
/* for ingress */
RtkApiRet cva_drv_secy_sa_mask_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint32 mask);
RtkApiRet cva_drv_secy_sa_mask_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint32 *mask);
/* for XPN ciphersuite */
RtkApiRet cva_drv_secy_sa_ctx_salt_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, const uint8 *salt, const uint8 *ssci);
RtkApiRet cva_drv_secy_sa_ctx_salt_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 *ctxSalt);

/* Counter */
RtkApiRet cva_drv_secy_counter_global_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 counterType, uint64 *value);
RtkApiRet cva_drv_secy_counter_global_clear(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 counterType);
RtkApiRet cva_drv_secy_counter_sa_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 counterType, uint64 *value);
RtkApiRet cva_drv_secy_counter_sa_clear(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 counterType);
RtkApiRet cva_drv_secy_counter_clear_all(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction);

RtkApiRet cva_drv_secy_counter_rxgp1_get(uint32 unit, RtkPort sysPort, RtkMacsecRxGlobalCounter_t *rxCounters);
RtkApiRet cva_drv_secy_counter_rxgp2_get(uint32 unit, RtkPort sysPort, RtkMacsecRxGlobalCounter_t *rxCounters);
RtkApiRet cva_drv_secy_counter_rxgp1_clear(uint32 unit, RtkPort sysPort);
RtkApiRet cva_drv_secy_counter_rxgp2_clear(uint32 unit, RtkPort sysPort);

/* control packet classification parameters */
RtkApiRet cva_drv_secy_cp_da_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *macDa);
RtkApiRet cva_drv_secy_cp_da_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *macDa);
RtkApiRet cva_drv_secy_cp_da_enable_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool enable);
RtkApiRet cva_drv_secy_cp_da_enable_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *enable);
RtkApiRet cva_drv_secy_cp_et_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 etherType);
RtkApiRet cva_drv_secy_cp_et_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *etherType);
RtkApiRet cva_drv_secy_cp_et_enable_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool enable);
RtkApiRet cva_drv_secy_cp_et_enable_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *enable);

/* vlan tag parser */
RtkApiRet cva_drv_secy_vp_control_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, bool parseQinQ, bool parseStag);
RtkApiRet cva_drv_secy_vp_control_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, bool *parseQinQ, bool *parseStag);
RtkApiRet cva_drv_secy_vp_qtag_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 qtag);
RtkApiRet cva_drv_secy_vp_qtag_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 *qtag);
RtkApiRet cva_drv_secy_vp_stag_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 stag);
RtkApiRet cva_drv_secy_vp_stag_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 *stag);

/* debug */
RtkApiRet cva_drv_secy_dbg_vlan_valid_flags_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, cva_MacsecSecyVlanTagValid_t *flags);
RtkApiRet cva_drv_secy_dbg_sectag_identified_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, cva_MacsecSecySectagIdentified_t *flags);

/* helper */
bool      cva_drv_secy_helper_is_params_modified(uint32 modifiedMask, uint32 bit);
RtkApiRet cva_drv_secy_helper_match_rule_parameter_check(const RtkMacsecSaMatchEntry_t *params);
RtkApiRet cva_drv_secy_helper_vlan_match_rule_parameter_check(const RtkMacsecSaMatchEntry_t *params);
RtkApiRet cva_drv_secy_helper_flow_control_parameter_check(const RtkMacsecFlowControl_t *params);
RtkApiRet cva_drv_secy_helper_sa_entry_parameter_check(uint32 cipherSuite, const RtkMacsecSecurityAssociation_t *params);
RtkApiRet cva_drv_secy_helper_verify_user_port_and_covert_to_sys_port(RtkPort port, RtkPort *sysPort);

#endif /* RTL906X_ASICDRV_secy_H_ */
