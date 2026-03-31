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

#ifndef CVA_RTK_API_VLAN_H__
#define CVA_RTK_API_VLAN_H__

/********* Include Header *********/
#include "rtl907xc_va_api.h"
#include "rtkas_api_vlan.h"
/***/

/********* Definition *********/
#define CVA_VLAN_VID_MAX (0xFFFu)
#define CVA_VLAN_FID_MAX (14u)
#define CVA_VLAN_PRI_MAX (7u)
#define CVA_VLAN_DEI_MAX (1u)

#define CVA_VLAN_INGRESS_RETAG_TBLSIZE (32u)
#define CVA_VLAN_EGRESS_RETAG_TBLSIZE  (32u)
/***/

/********* Type Definition *********/
typedef uint32 cva_VlanFid; /**< the data type of the fitering database identifier (0 to 15) */
typedef uint32 cva_VlanPri; /**< Priority vlaue (0 to 7) */
typedef uint32 cva_VlanDei; /**< Dei vlaue (0 to 1) */
typedef uint32 cva_RtkVlan; /* the data type of VLAN ID (0 to 0xFFF) */
/***/

EXTERN_API RtkApiRet cva_vlan_port_acpt_type_set(uint32 unit, RtkPort port, VlanAcceptFrameType_e acceptFrameType);

EXTERN_API RtkApiRet cva_vlan_port_acpt_type_get(uint32 unit, RtkPort port, VlanAcceptFrameType_e *pAcceptFrameType);

EXTERN_API RtkApiRet cva_svlan_port_acpt_type_set(uint32 unit, RtkPort port, VlanAcceptFrameType_e acceptFrameType);

EXTERN_API RtkApiRet cva_svlan_port_acpt_type_get(uint32 unit, RtkPort port, VlanAcceptFrameType_e *pAcceptFrameType);

EXTERN_API RtkApiRet cva_vlan_port_acpt_double_tag_set(uint32 unit, RtkPort port, VlanAcceptDoubleTag_e acceptDoubleTag);

EXTERN_API RtkApiRet cva_vlan_port_acpt_double_tag_get(uint32 unit, RtkPort port, VlanAcceptDoubleTag_e *pAcceptDoubleTag);

EXTERN_API RtkApiRet cva_vlan_tag_aware_set(uint32 unit, RtkPort port, cva_RtkEnable enabled);

EXTERN_API RtkApiRet cva_vlan_tag_aware_get(uint32 unit, RtkPort port, cva_RtkEnable *pEnabled);

EXTERN_API RtkApiRet cva_vlan_port_pvid_set(uint32 unit, RtkPort port, cva_RtkVlan pvid, cva_VlanPri priority);

EXTERN_API RtkApiRet cva_vlan_port_pvid_get(uint32 unit, RtkPort port, cva_RtkVlan *pPvid, cva_VlanPri *pPriority);

EXTERN_API RtkApiRet cva_svlan_port_pvid_set(uint32 unit, RtkPort port, cva_RtkVlan pvid, cva_VlanPri priority, cva_VlanDei dei);

EXTERN_API RtkApiRet cva_svlan_port_pvid_get(uint32 unit, RtkPort port, cva_RtkVlan *pPvid, cva_VlanPri *pPriority, cva_VlanDei *pDei);

EXTERN_API RtkApiRet cva_vlan_init(uint32 unit);

EXTERN_API RtkApiRet cva_vlan_set(uint32 unit, cva_RtkVlan vid, VlanEntry_t *pVlanEntry);

EXTERN_API RtkApiRet cva_vlan_get(uint32 unit, cva_RtkVlan vid, VlanEntry_t *pVlanEntry);

EXTERN_API RtkApiRet cva_vlan_destroy(uint32 unit, cva_RtkVlan vid);

EXTERN_API RtkApiRet cva_svlan_port_pkt_parser_en_set(uint32 unit, RtkPort port, cva_RtkEnable enable);

EXTERN_API RtkApiRet cva_svlan_port_pkt_parser_en_get(uint32 unit, RtkPort port, cva_RtkEnable *pEnable);

EXTERN_API RtkApiRet cva_svlan_fwd_base_ovid_set(uint32 unit, RtkPort port, cva_RtkEnable enable);

EXTERN_API RtkApiRet cva_svlan_fwd_base_ovid_get(uint32 unit, RtkPort port, cva_RtkEnable *pEnable);

EXTERN_API RtkApiRet cva_vlan_port_igr_filter_en_set(uint32 unit, RtkPort port, cva_RtkEnable igrFilter);

EXTERN_API RtkApiRet cva_vlan_port_igr_filter_en_get(uint32 unit, RtkPort port, cva_RtkEnable *pIgrFilter);

EXTERN_API RtkApiRet cva_vlan_igr_retagging_init(uint32 unit);

EXTERN_API RtkApiRet cva_vlan_igr_retagging_set(uint32      unit,
                                                uint32      idx,
                                                cva_RtkVlan originalVid,
                                                cva_RtkVlan newVid,
                                                RtkPmap     pbm,
                                                uint8       valid);

EXTERN_API RtkApiRet cva_vlan_igr_retagging_get(uint32       unit,
                                                uint32       idx,
                                                cva_RtkVlan *pOriginalVid,
                                                cva_RtkVlan *pNewVid,
                                                RtkPmap     *pPbm,
                                                uint8       *pValid);

EXTERN_API RtkApiRet cva_vlan_igr_retagging_en_set(uint32 unit, RtkPort port, cva_RtkEnable enable);

EXTERN_API RtkApiRet cva_vlan_igr_retagging_en_get(uint32 unit, RtkPort port, cva_RtkEnable *pEnable);

EXTERN_API RtkApiRet cva_vlan_port_egr_tag_status_set(uint32 unit, RtkPort port, VlanTxTagStatus_e tagStatus);

EXTERN_API RtkApiRet cva_vlan_port_egr_tag_status_get(uint32 unit, RtkPort port, VlanTxTagStatus_e *pTagStatus);

EXTERN_API RtkApiRet cva_svlan_port_egr_tag_status_set(uint32 unit, RtkPort port, VlanTxTagStatus_e tagStatus);

EXTERN_API RtkApiRet cva_svlan_port_egr_tag_status_get(uint32 unit, RtkPort port, VlanTxTagStatus_e *pTagStatus);

EXTERN_API RtkApiRet cva_vlan_port_egr_filter_en_set(uint32 unit, RtkPort port, cva_RtkEnable egrFilter);

EXTERN_API RtkApiRet cva_vlan_port_egr_filter_en_get(uint32 unit, RtkPort port, cva_RtkEnable *pEgrFilter);

EXTERN_API RtkApiRet cva_vlan_egr_retagging_init(uint32 unit);

EXTERN_API RtkApiRet cva_vlan_egr_retagging_set(uint32      unit,
                                                uint32      idx,
                                                cva_RtkVlan originalVid,
                                                cva_RtkVlan newVid,
                                                RtkPmap     pbm,
                                                uint8       valid);

EXTERN_API RtkApiRet cva_vlan_egr_retagging_get(uint32       unit,
                                                uint32       idx,
                                                cva_RtkVlan *pOriginalVid,
                                                cva_RtkVlan *pNewVid,
                                                RtkPmap     *pPbm,
                                                uint8       *pValid);

EXTERN_API RtkApiRet cva_vlan_egr_retagging_en_set(uint32 unit, RtkPort port, cva_RtkEnable enable);

EXTERN_API RtkApiRet cva_vlan_egr_retagging_en_get(uint32 unit, RtkPort port, cva_RtkEnable *pEnable);

#endif /* CVA_RTK_API_VLAN_H__ */
