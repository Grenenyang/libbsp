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

#ifndef DVA_ASICDRV_VLAN_H__
#define DVA_ASICDRV_VLAN_H__

#include "rtkas_common_types.h"
#include "rtkas_error.h"

#include "rtl907xd_va_api_vlan.h"

/****************************************************************/
/* Header File inclusion                                        */
/****************************************************************/

/****************************************************************/
/* Constant Definition                                          */
/****************************************************************/

#define DVA_VLAN_DATA0_MASK (0x00FFFFFF)
#define DVA_VLAN_DATA1_MASK (0x0001FFFF)
#define DVA_VLAN_DATA2_MASK (0x00FFFFFF)

#define DVA_VLAN_IGRFILTER_DROP        (0u)
#define DVA_VLAN_IGRFILTER_TRAP_TO_CPU (1u)
#define DVA_VLAN_IGRFILTER_END         (2u)

#define DVA_VLAN_FLUSH_TIME_MAX_US (1000u)
#define DVA_VLAN_FLUSH_TIME_MIN_US (0u)

typedef enum
{
    DVA_EM_PORTROLE_UNI = 0u,
    DVA_EM_PORTROLE_NNI = 1u,
    DVA_EM_PORTROLE_END = 2u
} dva_VlanPortRole_e;

typedef struct
{
    uint8  interfaceId;    /* L3 routing interface id */
    uint8  interfaceValid; /* indicate this interface id is valid(1) or invalid(0) */
    uint32 mbr;            /* Member ports (0x0000 ~ 0xFFFF) */
    uint32 utag;           /* Untaged ports (0x0000 ~ 0xFFFF) */
    uint8  fid;            /* filtering identifier (0 ~ 14) */
    uint8  ulfid;          /* Unicast DA lookup use VID(0) or FID(1) */
    uint8  vbfwd;          /* forwarding based on DVA_VLAN+L2(0) or DVA_VLAN(1) */
    uint8  valid;          /* indicate this entry is valid(1) or invalid(0) */
} dva_VlanTblParam_t;

typedef struct
{
    uint32 val[3];
} dva_VlanTblAsicEntry_t;

/****************************************************************/
/* Type Definition                                              */
/****************************************************************/

/****************************************************************/
/* Driver Proto Type Definition                                 */
/****************************************************************/
EXTERN_API RtkApiRet dva_drv_vlan_tbl_entry_set(uint32 unit, const uint32 index, dva_VlanTblParam_t *const pParam);
EXTERN_API RtkApiRet dva_drv_vlan_tbl_entry_get(uint32 unit, const uint32 index, dva_VlanTblParam_t *const pParam);
EXTERN_API RtkApiRet dva_drv_vlan_tblparam2entry(uint32 unit, const dva_VlanTblParam_t *const pVlanTblParam, VlanEntry_t *const pVlanEntry);
EXTERN_API RtkApiRet dva_drv_vlan_entry2tblparam(uint32 unit, const VlanEntry_t *const pVlanEntry, dva_VlanTblParam_t *const pVlanTblParam);

/* Function Name:
 *      dva_drv_vlan_port_pvid_set
 * Description:
 *      Set DVA_VLAN port-based VID and priority
 * Input:
 *      unit                          - stacking unit
 *      port                          - Port id
 *      Vid                           - Port-based vid
 *      Pri                           - Port-base vlan priority
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_PORT_ID
 *      -RT_ERR_SMI
 *
 * Note:
 *
 */
EXTERN_API RtkApiRet dva_drv_vlan_port_pvid_set(uint32 unit, const dva_RtkPort port, const uint32 vid, const uint32 pri);

/* Function Name:
 *      dva_drv_vlan_port_pvid_get
 * Description:
 *      Get DVA_VLAN port-based VID and priority
 * Input:
 *      unit                          - stacking unit
 *      port                          - Port id
 * Output:
 *      pVid                          - the pointer of port-based vid
 *      pPri                          - the pointer of port-base vlan priority
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_PORT_ID
 *      -RT_ERR_SMI
 *
 * Note:
 *
 */
EXTERN_API RtkApiRet dva_drv_vlan_port_pvid_get(uint32 unit, const dva_RtkPort port, uint32 *const pVid, uint32 *const pPri);

/* Function Name:
 *      dva_drv_vlan_port_acpt_type_set
 * Description:
 *      Set DVA_VLAN support frame type
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
 *     The API can set the port accept frame type.
 *     The accept frame type as following:
 *         FRAME_TYPE_BOTH
 *         FRAME_TYPE_TAGGED_ONLY
 *         FRAME_TYPE_UNTAGGED_ONLY
 *         FRAME_TYPE_NONE
 */
EXTERN_API RtkApiRet dva_drv_vlan_port_acpt_type_set(uint32 unit, const dva_RtkPort port, const VlanAcceptFrameType_e acceptFrameType);

/* Function Name:
 *      dva_drv_vlan_port_acpt_type_get
 * Description:
 *      Get DVA_VLAN support frame type
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
 *     The API can get the port accept frame type.
 *     The accept frame type as following:
 *         FRAME_TYPE_BOTH
 *         FRAME_TYPE_TAGGED_ONLY
 *         FRAME_TYPE_UNTAGGED_ONLY
 *         FRAME_TYPE_NONE
 */
EXTERN_API RtkApiRet dva_drv_vlan_port_acpt_type_get(uint32 unit, const dva_RtkPort port, VlanAcceptFrameType_e *const pAcceptFrameType);

/* Function Name:
 *      dva_drv_vlan_port_egr_tag_mode_get
 * Description:
 *      Get CVLAN egress tag mode
 * Input:
 *      unit        - stacking unit
 *      pTagRxu     - the pkts whose souce port is UNI port tx tag status
 *      pTagRxn     - the pkts whose souce port is NNI port tx tag status
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_PORT_ID
 *      -RT_ERR_NULL_POINTER
 *      -RT_ERR_SMI
 *
 * Note:
 *    The API is used for setting CVLAN egress tag mode
 *    The accept frame type as following:
 *    EM_VLAN_TAG_MODE_ORIGINAL
 *    EM_VLAN_TAG_MODE_KEEP_FORMAT
 *    EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT
 */
EXTERN_API RtkApiRet dva_drv_vlan_port_egr_tag_mode_get(uint32 unit, const dva_RtkPort port, VlanTagMode_e *const pTagRxu, VlanTagMode_e *const pTagRxn);

EXTERN_API RtkApiRet dva_drv_vlan_port_tag_aware_set(uint32 unit, const dva_RtkPort port, const uint32 enabled);

EXTERN_API RtkApiRet dva_drv_vlan_port_tag_aware_get(uint32 unit, const dva_RtkPort port, uint32 *const pEnabled);

EXTERN_API RtkApiRet dva_drv_vlan_port_parser_tag_set(uint32 unit, const dva_RtkPort port, const uint32 enabled);
EXTERN_API RtkApiRet dva_drv_vlan_port_parser_tag_get(uint32 unit, const dva_RtkPort port, uint32 *const pEnabled);

EXTERN_API RtkApiRet dva_drv_vlan_port_igr_filter_set(uint32 unit, const dva_RtkPort port, const uint32 enabled);
EXTERN_API RtkApiRet dva_drv_vlan_port_igr_filter_get(uint32 unit, const dva_RtkPort port, uint32 *const pEnabled);
EXTERN_API RtkApiRet dva_drv_vlan_igr_filter_act_set(uint32 unit, const dva_RtkPort port, const uint32 act);
EXTERN_API RtkApiRet dva_drv_vlan_igr_filter_act_get(uint32 unit, const dva_RtkPort port, uint32 *const pAct);
EXTERN_API RtkApiRet dva_drv_vlan_port_egr_filter_set(uint32 unit, const dva_RtkPort port, const uint32 enabled);
EXTERN_API RtkApiRet dva_drv_vlan_port_egr_filter_get(uint32 unit, const dva_RtkPort port, uint32 *const pEnabled);

EXTERN_API RtkApiRet dva_drv_vlan_port_role_set(uint32 unit, const dva_RtkPort port, const dva_VlanPortRole_e portrole);

EXTERN_API RtkApiRet dva_drv_vlan_port_role_get(uint32 unit, const dva_RtkPort port, dva_VlanPortRole_e *const pPortrole);

/* Function Name:
 *      dva_drv_vlan_egr_tag_status_set
 * Description:
 *      Set egress port CVLAN tag status
 * Input:
 *      unit        - stacking unit
 *      port        - port id
 *      tagRxu      - tx CVLAN tag status for the pkts received from UNI port
 *      tagRxn      - tx CVLAN tag status for the pkts received from NNI port
 * Output:
 *       none
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_ENABLE
 * Note:
 *      The API set egree port transimiting pakcet with or without
 *      CVLAN tag. Its priority is lower than ACL, and equial to DVA_VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of DVA_VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
EXTERN_API RtkApiRet dva_drv_vlan_egr_tag_status_set(uint32 unit, const dva_RtkPort port, const VlanTxTagStatus_e tagRxu, const VlanTxTagStatus_e tagRxn);

/* Function Name:
 *      dva_drv_vlan_egr_tag_status_get
 * Description:
 *      Get egress port CVLAN tag status
 * Input:
 *      unit         - stacking unit
 *      port         - port id
 * Output:
 *      pTagRxu      - the pointer of tx CVLAN tag status for the pkts received from UNI port
 *      pTagRxn      - the pointer of tx CVLAN tag status for the pkts received from NNI port
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_ENABLE
 * Note:
 *      The API gets egree port transimiting pakcet with or without
 *      CVLAN tags. Its priority is lower than ACL, and equial to DVA_VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of DVA_VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
EXTERN_API RtkApiRet dva_drv_vlan_egr_tag_status_get(uint32 unit, const dva_RtkPort port, VlanTxTagStatus_e *const pTagRxu, VlanTxTagStatus_e *const pTagRxn);

EXTERN_API RtkApiRet dva_drv_vlan_tpid_set(uint32 unit, const uint32 tpid);

EXTERN_API RtkApiRet dva_drv_vlan_tpid_get(uint32 unit, uint32 *const pTpid);

EXTERN_API RtkApiRet dva_drv_vlan_flush_set(uint32 unit);

/* Function Name:
 *      dva_drv_vlan_igr_retagging_get
 * Description:
 *      Get an ingress DVA_VLAN retagging entry
 * Input:
 *      unit            - stacking unit
 *      idx             - Ingress DVA_VLAN retagging table index
 * Output:
 *      pOriginalVid    - Original DVA_VLAN ID to be replaced
 *      pNewVid         - New DVA_VLAN ID
 *      pPbm            - Affected ingress port mask
 *      pValid          - Entry valid bit
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_NULL_POINTER
 */
EXTERN_API RtkApiRet dva_drv_vlan_igr_retagging_get(uint32 unit, const uint32 idx, dva_RtkVlan *const pOriginalVid, dva_RtkVlan *const pNewVid, RtkPmap *const pPbm, uint8 *const pValid);

/* Function Name:
 *      dva_drv_vlan_igr_retagging_set
 * Description:
 *      Set an ingress DVA_VLAN retagging entry
 * Input:
 *      unit            - stacking unit
 *      idx             - Ingress DVA_VLAN retagging table index
 *      originalVid     - Original DVA_VLAN ID to be replaced
 *      newVid          - New DVA_VLAN ID
 *      pbm             - Affected ingress port mask
 *      valid           - Entry valid bit
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 */
EXTERN_API RtkApiRet dva_drv_vlan_igr_retagging_set(uint32 unit, const uint32 idx, const dva_RtkVlan originalVid, const dva_RtkVlan newVid, const RtkPmap pbm, const uint8 valid);

/* Function Name:
 *      dva_drv_vlan_egress_retagging_get
 * Description:
 *      Get an egress DVA_VLAN retagging entry
 * Input:
 *      unit            - stacking unit
 *      idx             - Egress DVA_VLAN retagging table index
 * Output:
 *      pOriginalVid    - Original DVA_VLAN ID to be replaced
 *      pNewVid         - New DVA_VLAN ID
 *      pPbm            - Affected egress port mask
 *      pValid          - Entry valid bit
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_NULL_POINTER
 */
EXTERN_API RtkApiRet dva_drv_vlan_egress_retagging_get(uint32 unit, const uint32 idx, dva_RtkVlan *const pOriginalVid, dva_RtkVlan *const pNewVid, RtkPmap *const pPbm, uint8 *const pValid);

/* Function Name:
 *      dva_drv_vlan_egress_retagging_set
 * Description:
 *      Set an egress DVA_VLAN retagging entry
 * Input:
 *      unit            - stacking unit
 *      idx             - Egress DVA_VLAN retagging table index
 *      originalVid     - Original DVA_VLAN ID to be replaced
 *      newVid          - New DVA_VLAN ID
 *      pbm             - Affected egress port mask
 *      valid           - Entry valid bit
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 */
EXTERN_API RtkApiRet dva_drv_vlan_egress_retagging_set(uint32 unit, const uint32 idx, const dva_RtkVlan originalVid, const dva_RtkVlan newVid, const RtkPmap pbm, const uint8 valid);

/* Function Name:
 *      dva_drv_vlan_igr_retagging_en_get
 * Description:
 *      Get ingress DVA_VLAN retagging status
 * Input:
 *      unit            - stacking unit
 *      port            - Port number
 * Output:
 *      pEnable         - Status
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_NULL_POINTER
 */
EXTERN_API RtkApiRet dva_drv_vlan_igr_retagging_en_get(uint32 unit, const dva_RtkPort port, dva_RtkEnable *const pEnable);

/* Function Name:
 *      dva_drv_vlan_igr_retagging_en_set
 * Description:
 *      Set ingress DVA_VLAN retagging status
 * Input:
 *      unit            - stacking unit
 *      port            - Port number
 *      enable          - Status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 */
EXTERN_API RtkApiRet dva_drv_vlan_igr_retagging_en_set(uint32 unit, const dva_RtkPort port, const dva_RtkEnable enable);

/* Function Name:
 *      dva_drv_vlan_egr_retagging_en_get
 * Description:
 *      Get egress DVA_VLAN retagging status
 * Input:
 *      unit            - stacking unit
 *      port            - Port number
 * Output:
 *      pEnable         - Status
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 *      -RT_ERR_NULL_POINTER
 */
EXTERN_API RtkApiRet dva_drv_vlan_egr_retagging_en_get(uint32 unit, const dva_RtkPort port, dva_RtkEnable *const pEnable);

/* Function Name:
 *      dva_drv_vlan_egr_retagging_en_set
 * Description:
 *      Set egress DVA_VLAN retagging status
 * Input:
 *      unit            - stacking unit
 *      port            - Port number
 *      enable          - Status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      -RT_ERR_FAILED
 */
EXTERN_API RtkApiRet dva_drv_vlan_egr_retagging_en_set(uint32 unit, const dva_RtkPort port, const dva_RtkEnable enable);

EXTERN_API RtkApiRet dva_drv_vlan_check_valid_user_port(uint32 unit, const dva_RtkPort port);

#endif /* DVA_ASICDRV_VLAN_H__ */
