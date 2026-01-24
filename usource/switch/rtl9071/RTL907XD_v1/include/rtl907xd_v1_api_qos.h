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

#ifndef DV1_RTK_API_QOS_H__
#define DV1_RTK_API_QOS_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xd_v1_api.h"
#include "rtkas_api_qos.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DV1_QOS_PORT_BW_MAX (((uint32)0x1u << DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_RATE_LEN) - 1u)
#define DV1_QOS_QUE_BW_MAX  (((uint32)0x1u << DV1_PORT0_BANDWITH_CONTROL_BWRATE_LEN) - 1u)
#define DV1_QOS_SCH_SP      (0u)              /**< The scheduler of Strict Priority */
#define DV1_QOS_SCH_WFQ     (1u)              /**< The scheduler of Weighted Round Robin */
#define DV1_QOS_SCH_WRR     (2u)              /**< The scheduler of Weighted Fair Queuing */
#define DV1_QOS_SCH_CBS     (3u)              /**< The scheduler of Credit-Based Shaper */
#define DV1_QOS_SCH_END     (DV1_QOS_SCH_CBS) /**< The scheduler of End */
/* Offset */
#define DV1_QOS_GROUP_OFFSET    (4u)
#define DV1_QOS_PRI_SIZE        (3u)
#define DV1_QOS_PRI_OFFSET      (4u)
#define DV1_QOS_QUE_BASE        (4u)
#define DV1_QOS_QUE_SIZE        (8u)
#define DV1_QOS_DEI_SIZE        (4u)
#define DV1_QOS_DEI_OFFSET      (4u)
#define DV1_QOS_DSCP_SIZE       (8u)
#define DV1_QOS_DSCP_OFFSET     (4u)
#define DV1_QOS_DSCP_NUM        (5u)
#define DV1_QOS_DSCP_RMK_SIZE   (6u)
#define DV1_QOS_DSCP_RMK_OFFSET (0x20u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 dv1_QosPri;    /**< Priority vlaue (0 to QOS_PRI_MAX) */
typedef uint32 dv1_QosDei;    /**< Dei vlaue (0 to QOS_DEI_MAX) */
typedef uint32 dv1_QosQueNum; /**< Tx queue number (0 to QOS_QUE_MAX)*/
typedef uint32 dv1_QosTblIdx; /**< Arbitration/Mapping table index (0 to QOS_TBL_IDX_MAX) */
typedef uint32 dv1_QosWt;     /**< Weight value of WRR or WFQ (0 to QOS_WT_MAX) */
typedef uint32 dv1_QosDscp;   /**< Dscp vlaue (0 to QOS_DSCP_MAX) */
typedef uint32 dv1_QosBw;     /**< Bandwidth of the port or queue (0 to DV1_QOS_PORT/QUE_BW_MAX) */

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
/* Ingress Bandwidth Control */
EXTERN_API RtkApiRet dv1_qos_igr_bw_ctrl_set(uint32 unit, dv1_RtkPort port, const QosIgrBwCtrl_t *pIgrBwCtrl);
EXTERN_API RtkApiRet dv1_qos_igr_bw_ctrl_get(uint32 unit, dv1_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl);
/* Traffic Classification */
EXTERN_API RtkApiRet dv1_qos_port_pri_set(uint32 unit, dv1_RtkPort port, dv1_QosPri pri);
EXTERN_API RtkApiRet dv1_qos_port_pri_get(uint32 unit, dv1_RtkPort port, dv1_QosPri *pPri);
EXTERN_API RtkApiRet dv1_qos_pri_sel_set(uint32 unit, dv1_QosTblIdx group, const QosPriSel_t *pPriDec);
EXTERN_API RtkApiRet dv1_qos_pri_sel_get(uint32 unit, dv1_QosTblIdx group, QosPriSel_t *pPriDec);
EXTERN_API RtkApiRet dv1_qos_pri_wt_tbl_set(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx idx);
EXTERN_API RtkApiRet dv1_qos_pri_wt_tbl_get(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx *pIdx);
EXTERN_API RtkApiRet dv1_qos_cvlan_pri_remap_set(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri iPri, dv1_QosDei iDei, dv1_QosPri mappingPri);
EXTERN_API RtkApiRet dv1_qos_cvlan_pri_remap_get(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri iPri, dv1_QosDei iDei, dv1_QosPri *pMappingPri);
EXTERN_API RtkApiRet dv1_qos_svlan_pri_remap_set(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oPri, dv1_QosDei oDei, dv1_QosPri mappingPri);
EXTERN_API RtkApiRet dv1_qos_svlan_pri_remap_get(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oPri, dv1_QosDei oDei, dv1_QosPri *pMappingPri);
EXTERN_API RtkApiRet dv1_qos_dscp_pri_remap_set(uint32 unit, dv1_QosTblIdx idx, dv1_QosDscp dscp, dv1_QosPri mappingPri);
EXTERN_API RtkApiRet dv1_qos_dscp_pri_remap_get(uint32 unit, dv1_QosTblIdx idx, dv1_QosDscp dscp, dv1_QosPri *pMappingPri);
EXTERN_API RtkApiRet dv1_qos_cvlan_remap_tbl_set(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx idx);
EXTERN_API RtkApiRet dv1_qos_cvlan_remap_tbl_get(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx *pIdx);
EXTERN_API RtkApiRet dv1_qos_svlan_remap_tbl_set(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx idx);
EXTERN_API RtkApiRet dv1_qos_svlan_remap_tbl_get(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx *pIdx);
EXTERN_API RtkApiRet dv1_qos_dscp_remap_tbl_set(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx idx);
EXTERN_API RtkApiRet dv1_qos_dscp_remap_tbl_get(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx *pIdx);
EXTERN_API RtkApiRet dv1_qos_pri_que_mapping_set(uint32 unit, dv1_RtkPort port, dv1_QosPri pri, dv1_QosQueNum que);
EXTERN_API RtkApiRet dv1_qos_pri_que_mapping_get(uint32 unit, dv1_RtkPort port, dv1_QosPri pri, dv1_QosQueNum *pQue);
/* Traffic Scheduling */
EXTERN_API RtkApiRet dv1_qos_que_sch_set(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, RtkQoSSch queSch);
EXTERN_API RtkApiRet dv1_qos_que_sch_get(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, RtkQoSSch *pQueSch);
EXTERN_API RtkApiRet dv1_qos_que_wfq_wrr_wt_set(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_QosWt queWt);
EXTERN_API RtkApiRet dv1_qos_que_wfq_wrr_wt_get(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_QosWt *pQueWt);
/* Egress Bandwidth Control */
EXTERN_API RtkApiRet dv1_qos_egr_bw_ctrl_set(uint32 unit, dv1_RtkPort port, dv1_RtkEnable en, dv1_QosBw bw);
EXTERN_API RtkApiRet dv1_qos_egr_bw_ctrl_get(uint32 unit, dv1_RtkPort port, dv1_RtkEnable *pEn, dv1_QosBw *pPortBw);
EXTERN_API RtkApiRet dv1_qos_egr_que_bw_ctrl_set(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_RtkEnable en, dv1_QosBw bw);
EXTERN_API RtkApiRet dv1_qos_egr_que_bw_ctrl_get(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_RtkEnable *pEn, dv1_QosBw *pQueBw);
/* Traffic Priority Remarking */
EXTERN_API RtkApiRet dv1_qos_cvlan_rmk_en_set(uint32 unit, dv1_RtkPort port, dv1_RtkEnable en);
EXTERN_API RtkApiRet dv1_qos_cvlan_rmk_en_get(uint32 unit, dv1_RtkPort port, dv1_RtkEnable *pEn);
EXTERN_API RtkApiRet dv1_qos_svlan_rmk_en_set(uint32 unit, dv1_RtkPort port, dv1_RtkEnable en);
EXTERN_API RtkApiRet dv1_qos_svlan_rmk_en_get(uint32 unit, dv1_RtkPort port, dv1_RtkEnable *pEn);
EXTERN_API RtkApiRet dv1_qos_dscp_rmk_en_set(uint32 unit, dv1_RtkPort port, dv1_RtkEnable en);
EXTERN_API RtkApiRet dv1_qos_dscp_rmk_en_get(uint32 unit, dv1_RtkPort port, dv1_RtkEnable *pEn);
EXTERN_API RtkApiRet dv1_qos_cvlan_rmk_set(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oriPri, dv1_QosPri rmkPri, dv1_QosDei rmkDei);
EXTERN_API RtkApiRet dv1_qos_cvlan_rmk_get(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oriPri, dv1_QosPri *pRmkPri, dv1_QosDei *pRmkDei);
EXTERN_API RtkApiRet dv1_qos_svlan_rmk_set(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oriPri, dv1_QosPri rmkPri, dv1_QosDei rmkDei);
EXTERN_API RtkApiRet dv1_qos_svlan_rmk_get(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oriPri, dv1_QosPri *pRmkPri, dv1_QosDei *pRmkDei);
EXTERN_API RtkApiRet dv1_qos_dscp_rmk_set(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oriPri, dv1_QosDscp rmkDscp);
EXTERN_API RtkApiRet dv1_qos_dscp_rmk_get(uint32 unit, dv1_QosTblIdx idx, dv1_QosPri oriPri, dv1_QosDscp *pRmkDscp);
EXTERN_API RtkApiRet dv1_qos_cvlan_rmk_tbl_set(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx idx);
EXTERN_API RtkApiRet dv1_qos_cvlan_rmk_tbl_get(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx *pIdx);
EXTERN_API RtkApiRet dv1_qos_svlan_rmk_tbl_set(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx idx);
EXTERN_API RtkApiRet dv1_qos_svlan_rmk_tbl_get(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx *pIdx);
EXTERN_API RtkApiRet dv1_qos_dscp_rmk_tbl_set(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx idx);
EXTERN_API RtkApiRet dv1_qos_dscp_rmk_tbl_get(uint32 unit, dv1_RtkPort port, dv1_QosTblIdx *pIdx);

#endif /* DV1_RTK_API_QOS_H__ */
