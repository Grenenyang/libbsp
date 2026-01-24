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

#ifndef CVB_ASICDRV_SVLAN_H__
#define CVB_ASICDRV_SVLAN_H__

#include "rtkas_common_types.h"
#include "rtkas_error.h"

#include "rtl907xc_vb_api_vlan.h"

/* Function Name:
 *      asicdrv_svlan_portAcceptFrameType_set
 * Description:
 *      Set SVLAN support frame type
 * Input:
 *      unit                   - stacking unit
 *      port                   - Port id
 *      acceptFrameType        - accept frame type
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_INPUT
 *
 * Note:
 *     The API can set the port SVLAN accept frame type.
 *     The accept frame type as following:
 *         EM_ACCEPT_FRAME_TYPE_ALL
 *         EM_ACCEPT_FRAME_TYPE_TAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_NONE
 */
EXTERN_API RtkApiRet cvb_drv_svlan_port_acpt_type_set(uint32 unit, RtkPort port, VlanAcceptFrameType_e acceptFrameType);

/* Function Name:
 *      cvb_drv_svlan_port_acpt_type_get
 * Description:
 *      Get SVLAN support frame type
 * Input:
 *      unit                   - stacking unit
 *      port                   - Port id
 * Output:
 *      pAcceptFrameType       - accept frame type
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_PORT_ID
 *      -RT_ERR_SMI
 *
 * Note:
 *     The API can get the port SVLAN accept frame type.
 *     The accept frame type as following:
 *         EM_ACCEPT_FRAME_TYPE_ALL
 *         EM_ACCEPT_FRAME_TYPE_TAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_NONE
 */
EXTERN_API RtkApiRet cvb_drv_svlan_port_acpt_type_get(uint32 unit, RtkPort port, VlanAcceptFrameType_e *pAcceptFrameType);

EXTERN_API RtkApiRet cvb_drv_svlan_port_tag_aware_set(uint32 unit, RtkPort port, uint32 enabled);

EXTERN_API RtkApiRet cvb_drv_svlan_port_tag_aware_get(uint32 unit, RtkPort port, uint32 *pEnabled);

EXTERN_API RtkApiRet cvb_drv_svlan_pkt_parser_en_set(uint32 unit, RtkPort port, uint32 enabled);

EXTERN_API RtkApiRet cvb_drv_svlan_pkt_parser_en_get(uint32 unit, RtkPort port, uint32 *pEnabled);

/* Function Name:
 *      asicdrv_svlan_portTagMode_set
 * Description:
 *      Set SVLAN egress tag mode
 * Input:
 *      unit        - stacking unit
 *      port        - Port id
 *      tagRxu      - the pkts whose souce port is UNI port tx tag status
 *      tagRxn      - the pkts whose souce port is NNI port tx tag status
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_PORT_ID
 *      -RT_ERR_INPUT
 *      -RT_ERR_SMI
 *
 * Note:
 *    The API is used for setting SVLAN egress tag mode
 *    The accept frame type as following:
 *    EM_VLAN_TAG_MODE_ORIGINAL
 *    EM_VLAN_TAG_MODE_KEEP_FORMAT
 *    EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT
 */
EXTERN_API RtkApiRet cvb_drv_svlan_egr_tag_mode_set(uint32 unit, RtkPort port, VlanTagMode_e tagRxu, VlanTagMode_e tagRxn);

/* Function Name:
 *      asicdrv_svlan_portTagMode_get
 * Description:
 *      Get SVLAN egress tag mode
 * Input:
 *      unit         - stacking unit
 *      port         - Port id
 * Output:
 *      pTagRxu      - the pkts whose souce port is UNI port tx tag status
 *      pTagRxn      - the pkts whose souce port is NNI port tx tag status
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_PORT_ID
 *      -RT_ERR_NULL_POINTER
 *      -RT_ERR_SMI
 *
 * Note:
 *    The API is used for setting SVLAN egress tag mode
 *    The accept frame type as following:
 *    EM_VLAN_TAG_MODE_ORIGINAL
 *    EM_VLAN_TAG_MODE_KEEP_FORMAT
 *    EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT
 */
EXTERN_API RtkApiRet cvb_drv_svlan_egr_tag_mode_get(uint32 unit, RtkPort port, VlanTagMode_e *pTagRxu, VlanTagMode_e *pTagRxn);

/* Function Name:
 *      cvb_drv_svlan_port_egr_stat_set
 * Description:
 *      Get egress port SVLAN tag status
 * Input:
 *      unit        - stacking unit
 *      port        - port id
 *      tagRxu      - tx SVLAN tag status for the pkts received from UNI port
 *      tagRxn      - tx SVLAN tag status for the pkts received from NNI port
 * Output:
 *       none
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_ENABLE
 * Note:
 *      The API sets egree port transimiting pakcet with or without
 *      SVLAN tag. Its priority is lower than ACL, and equial to CVB_VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of CVB_VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
EXTERN_API RtkApiRet cvb_drv_svlan_port_egr_stat_set(uint32 unit, RtkPort port, VlanTxTagStatus_e tagRxu, VlanTxTagStatus_e tagRxn);

/* Function Name:
 *      cvb_drv_svlan_port_egr_stat_get
 * Description:
 *      Get egress port SVLAN tag status
 * Input:
 *      unit         - stacking unit
 *      port         -  port id
 * Output:
 *      pTagRxu     - the pointer of tx SVLAN tag status for the pkts received from UNI port
 *      pTagRxn     - the pointer of tx SVLAN tag status for the pkts received from NNI port
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_ENABLE
 * Note:
 *      The API gets egree port transimiting pakcet with or without
 *      SVLAN tags. Its priority is lower than ACL, and equial to CVB_VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of CVB_VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
EXTERN_API RtkApiRet cvb_drv_svlan_port_egr_stat_get(uint32 unit, RtkPort port, VlanTxTagStatus_e *pTagRxu, VlanTxTagStatus_e *pTagRxn);

EXTERN_API RtkApiRet cvb_drv_svlan_port_pvid_set(uint32 unit, RtkPort port, uint32 svid, uint32 pri, uint32 dei);

EXTERN_API RtkApiRet cvb_drv_svlan_port_pvid_get(uint32 unit, RtkPort port, uint32 *pSvid, uint32 *pPri, uint32 *pDei);

EXTERN_API RtkApiRet cvb_drv_svlan_fwd_base_ovid_set(uint32 unit, RtkPort port, uint32 enabled);

EXTERN_API RtkApiRet cvb_drv_svlan_fwd_base_ovid_get(uint32 unit, RtkPort port, uint32 *pEnabled);

EXTERN_API RtkApiRet cvb_drv_svlan_tpid_set(uint32 unit, uint32 tpid);

EXTERN_API RtkApiRet cvb_drv_svlan_tpid_get(uint32 unit, uint32 *pTpid);

EXTERN_API RtkApiRet cvb_drv_svlan_check_valid_user_port(uint32 unit, const RtkPort port);

#endif /* CVB_ASICDRV_SVLAN_H__ */
