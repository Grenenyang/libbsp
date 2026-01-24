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

#ifndef CVA_RTK_API_MACSEC_H__
#define CVA_RTK_API_MACSEC_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_types.h"
#include "rtkas_api_macsec.h"
#include "rtl907xc_va_api.h"
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet cva_macsec_get_match_rule_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecSaMatchEntry_t *params);
EXTERN_API RtkApiRet cva_macsec_get_match_rule_vlan_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecSaMatchEntry_t *params);
EXTERN_API RtkApiRet cva_macsec_get_match_rule_misc_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecSaMatchEntry_t *params);
EXTERN_API RtkApiRet cva_macsec_set_match_rule_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecSaMatchEntry_t *params);
EXTERN_API RtkApiRet cva_macsec_set_match_rule_vlan_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecSaMatchEntry_t *params);
EXTERN_API RtkApiRet cva_macsec_set_match_rule_misc_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecSaMatchEntry_t *params);

EXTERN_API RtkApiRet cva_macsec_get_match_rule_status(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool *enable);
EXTERN_API RtkApiRet cva_macsec_set_match_rule_status(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool enable);

EXTERN_API RtkApiRet cva_macsec_get_flow_control_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecFlowControl_t *params);
EXTERN_API RtkApiRet cva_macsec_set_flow_control_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecFlowControl_t *params);

EXTERN_API RtkApiRet cva_macsec_get_sa_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, uint32 cipherSuite, RtkMacsecSecurityAssociation_t *params);
EXTERN_API RtkApiRet cva_macsec_set_sa_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, uint32 cipherSuite, const RtkMacsecSecurityAssociation_t *params);

EXTERN_API RtkApiRet cva_macsec_get_tx_global_counter(uint32 unit, RtkPort port, RtkMacsecTxGlobalCounter_t *txCounters);
EXTERN_API RtkApiRet cva_macsec_clear_tx_global_counter(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet cva_macsec_get_rx_global_counter(uint32 unit, RtkPort port, RtkMacsecRxGlobalCounter_t *rxCounters);
EXTERN_API RtkApiRet cva_macsec_clear_rx_global_counter(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet cva_macsec_get_tx_sa_counter(uint32 unit, RtkPort port, uint8 index, RtkMacsecTxSaCounter_t *txCounters);
EXTERN_API RtkApiRet cva_macsec_clear_tx_sa_counter(uint32 unit, RtkPort port, uint8 index);
EXTERN_API RtkApiRet cva_macsec_get_rx_sa_counter(uint32 unit, RtkPort port, uint8 index, RtkMacsecRxSaCounter_t *rxCounters);
EXTERN_API RtkApiRet cva_macsec_clear_rx_sa_counter(uint32 unit, RtkPort port, uint8 index);
EXTERN_API RtkApiRet cva_macsec_clear_all_tx_counter(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet cva_macsec_clear_all_rx_counter(uint32 unit, RtkPort port);

EXTERN_API RtkApiRet cva_macsec_get_control_packet_mac_da_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool *enable, uint8 *macDa);
EXTERN_API RtkApiRet cva_macsec_set_control_packet_mac_da_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool enable, const uint8 *macDa);

EXTERN_API RtkApiRet cva_macsec_get_control_packet_ether_type_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool *enable, uint16 *etherType);
EXTERN_API RtkApiRet cva_macsec_set_control_packet_ether_type_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool enable, uint16 etherType);

EXTERN_API RtkApiRet cva_macsec_get_vlan_parser_settings(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, RtkMacsecVlanPaser_t *vlanParser);
EXTERN_API RtkApiRet cva_macsec_set_vlan_parser_settings(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, const RtkMacsecVlanPaser_t *vlanParser);

#endif /* CVA_RTK_API_MACSEC_H__ */
