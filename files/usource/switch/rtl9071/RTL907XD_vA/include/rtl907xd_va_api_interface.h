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

#ifndef DVA_RTK_API_INTERFACE_H__
#define DVA_RTK_API_INTERFACE_H__

#include "rtkas_common_types.h"
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_asicdrv_port.h"

EXTERN_API RtkApiRet dva_port_t1_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e mode);
EXTERN_API RtkApiRet dva_port_mode_get(uint32 unit, RtkPort userPort, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_port_xmii_link_status_set(uint32 unit, RtkPort userPort, PortMacAbility_t *pMacAbility);
EXTERN_API RtkApiRet dva_port_link_status_get(uint32 unit, RtkPort userPort, PortLinkStatus_t *pStatus);
EXTERN_API RtkApiRet dva_port_eth_link_ability_set(uint32 unit, RtkPort userPort, const PortEthLinkAbility_t *pEthAbility);
EXTERN_API RtkApiRet dva_port_eth_link_ability_get(uint32 unit, RtkPort userPort, PortRtkEthLinkStatus_t *pStatus);
EXTERN_API RtkApiRet dva_port_rgmii_txdly_set(uint32 unit, RtkPort userPort, uint8 delay);
EXTERN_API RtkApiRet dva_port_rgmii_txdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay);
EXTERN_API RtkApiRet dva_port_rgmii_rxdly_set(uint32 unit, RtkPort userPort, RtkPortSpeed_e speed, uint8 delay);
EXTERN_API RtkApiRet dva_port_rgmii_rxdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay);
EXTERN_API RtkApiRet dva_port_xmii_driving_set(uint32 unit, RtkPort userPort, uint8 base, uint8 rise, uint8 fall);
EXTERN_API RtkApiRet dva_port_xmii_driving_get(uint32 unit, RtkPort userPort, uint8 *pBase, uint8 *pRise, uint8 *pFall);
EXTERN_API RtkApiRet dva_port_pkt_maxlen_set(uint32 unit, RtkEnable enable, RtkPort userPort);
EXTERN_API RtkApiRet dva_port_pkt_maxlen_get(uint32 unit, RtkPort userPort, uint32 *maxLen);
EXTERN_API RtkApiRet dva_port_xmii_latch_edge_set(uint32 unit, RtkPort userPort, bool isRxNegSample, bool isTxNegSample);
EXTERN_API RtkApiRet dva_port_xmii_latch_edge_get(uint32 unit, RtkPort userPort, bool *isRxNegSample, bool *isTxNegSample);
EXTERN_API RtkApiRet dva_port_rmii_refclk_sel_set(uint32 unit, RtkPort userPort, uint32 phyModePhase, uint32 internalPhase);
EXTERN_API RtkApiRet dva_port_rmii_refclk_sel_get(uint32 unit, RtkPort userPort, uint32 *phyModePhase, uint32 *internalPhase);
EXTERN_API RtkApiRet dva_port_xmii_ssc_set(uint32 unit, RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset);
EXTERN_API RtkApiRet dva_port_xmii_ssc_get(uint32 unit, RtkPort userPort, RtkEnable *isEnable, uint32 *sequenceModeOffset, uint32 *phaseOffset);
EXTERN_API RtkApiRet dva_port_xmii_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode);
EXTERN_API RtkApiRet dva_port_xmii_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 speed, uint32 linkStatus);
EXTERN_API RtkApiRet dva_port_fe_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, RtkEnable anEnable, uint32 speed);
EXTERN_API RtkApiRet dva_port_t1_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 isMaster, uint32 speed);
#endif /* DVA_RTK_API_INTERFACE_H__ */
