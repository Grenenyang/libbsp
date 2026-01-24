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

#ifndef CVB_RTK_API_STP_H__
#define CVB_RTK_API_STP_H__

#include "rtl907xc_vb_api.h"
#include "rtkas_api_stp.h"

EXTERN_API RtkApiRet cvb_stp_state_set(uint32 unit, RtkEnable stpState);
EXTERN_API RtkApiRet cvb_stp_state_get(uint32 unit, RtkEnable *pStpState);
EXTERN_API RtkApiRet cvb_stp_type_set(uint32 unit, RtkStpType_e stpType);
EXTERN_API RtkApiRet cvb_stp_type_get(uint32 unit, RtkStpType_e *pStpType);
EXTERN_API RtkApiRet cvb_stp_bridge_priority_set(uint32 unit, RtkStpBdgPri stpBridgePriority);
EXTERN_API RtkApiRet cvb_stp_bridge_priority_get(uint32 unit, RtkStpBdgPri *pStpBridgePriority);
EXTERN_API RtkApiRet cvb_stp_forward_delay_set(uint32 unit, RtkStpFowardDelay stpForwardDelay);
EXTERN_API RtkApiRet cvb_stp_forward_delay_get(uint32 unit, RtkStpFowardDelay *pStpForwardDelay);
EXTERN_API RtkApiRet cvb_stp_max_age_set(uint32 unit, RtkStpMaxAge stpMaxAge);
EXTERN_API RtkApiRet cvb_stp_max_age_get(uint32 unit, RtkStpMaxAge *pStpMaxAge);
EXTERN_API RtkApiRet cvb_stp_hello_time_set(uint32 unit, RtkStpHelloTime stpHelloTime);
EXTERN_API RtkApiRet cvb_stp_hello_time_get(uint32 unit, RtkStpHelloTime *pStpHelloTime);
EXTERN_API RtkApiRet cvb_stp_port_state_set(uint32 unit, RtkStpMstiId msti, RtkPort portId, RtkStpPortState_e portState);
EXTERN_API RtkApiRet cvb_stp_port_state_get(uint32 unit, RtkStpMstiId msti, RtkPort portId, RtkStpPortState_e *pPortState);
EXTERN_API RtkApiRet cvb_stp_port_priority_set(uint32 unit, RtkPort portId, RtkStpPortPri stpPortPriority);
EXTERN_API RtkApiRet cvb_stp_port_priority_get(uint32 unit, RtkPort portId, RtkStpPortPri *pStpPortPriority);
EXTERN_API RtkApiRet cvb_stp_port_path_cost_set(uint32 unit, RtkPort portId, RtkStpPortPathCost stpPortPathCost);
EXTERN_API RtkApiRet cvb_stp_port_path_cost_get(uint32 unit, RtkPort portId, RtkStpPortPathCost *pStpPortPathCost);
EXTERN_API RtkApiRet cvb_stp_port_admin_edge_set(uint32 unit, RtkPort portId, RtkEnable stpPortAdminEdge);
EXTERN_API RtkApiRet cvb_stp_port_admin_edge_get(uint32 unit, RtkPort portId, RtkEnable *pStpPortAdminEdge);
EXTERN_API RtkApiRet cvb_stp_port_auto_edge_set(uint32 unit, RtkPort portId, RtkEnable stpPortAutoEdge);
EXTERN_API RtkApiRet cvb_stp_port_auto_edge_get(uint32 unit, RtkPort portId, RtkEnable *pStpPortAutoEdge);
EXTERN_API RtkApiRet cvb_stp_port_p2p_set(uint32 unit, RtkPort portId, RtkStpPortP2p_e stpPortP2p);
EXTERN_API RtkApiRet cvb_stp_port_p2p_get(uint32 unit, RtkPort portId, RtkStpPortP2p_e *pStpPortP2p);
EXTERN_API RtkApiRet cvb_stp_port_migration_check_set(uint32 unit, RtkPort portId);

#endif /* CVB_RTK_API_STP_H__ */
