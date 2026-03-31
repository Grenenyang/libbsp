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

#ifndef CVB_RTK_API_QOS_H__
#define CVB_RTK_API_QOS_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xc_vb_api.h"
#include "rtkas_api_qos.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CVB_QOS_PORT_BW_MAX (((uint32)0x1u << CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_RATE_LEN) - 1u)
#define CVB_QOS_QUE_BW_MAX  (((uint32)0x1u << CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LEN) - 1u)
#define CVB_QOS_SCH_SP      (0u)              /**< The scheduler of Strict Priority */
#define CVB_QOS_SCH_WFQ     (1u)              /**< The scheduler of Weighted Round Robin */
#define CVB_QOS_SCH_WRR     (2u)              /**< The scheduler of Weighted Fair Queuing */
#define CVB_QOS_SCH_CBS     (3u)              /**< The scheduler of Credit-Based Shaper */
#define CVB_QOS_SCH_END     (CVB_QOS_SCH_CBS) /**< The scheduler of End */
/* Offset */
#define CVB_QOS_GROUP_OFFSET    (4u)
#define CVB_QOS_PRI_SIZE        (3u)
#define CVB_QOS_PRI_OFFSET      (4u)
#define CVB_QOS_QUE_BASE        (4u)
#define CVB_QOS_QUE_SIZE        (8u)
#define CVB_QOS_DEI_SIZE        (4u)
#define CVB_QOS_DEI_OFFSET      (4u)
#define CVB_QOS_DSCP_SIZE       (8u)
#define CVB_QOS_DSCP_OFFSET     (4u)
#define CVB_QOS_DSCP_NUM        (5u)
#define CVB_QOS_DSCP_RMK_SIZE   (6u)
#define CVB_QOS_DSCP_RMK_OFFSET (0x20u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 cvb_QosPri;    /**< Priority vlaue (0 to QOS_PRI_MAX) */
typedef uint32 cvb_QosDei;    /**< Dei vlaue (0 to QOS_DEI_MAX) */
typedef uint32 cvb_QosQueNum; /**< Tx queue number (0 to QOS_QUE_MAX)*/
typedef uint32 cvb_QosTblIdx; /**< Arbitration/Mapping table index (0 to QOS_TBL_IDX_MAX) */
typedef uint32 cvb_QosWt;     /**< Weight value of WRR or WFQ (0 to QOS_WT_MAX) */
typedef uint32 cvb_QosDscp;   /**< Dscp vlaue (0 to QOS_WT_MAX) */
typedef uint32 cvb_QosBw;     /**< Bandwidth of the port or queue (0 to CVB_QOS_PORT/QUE_BW_MAX) */

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
/* Ingress Bandwidth Control */
EXTERN_API RtkApiRet cvb_qos_igr_bw_ctrl_set(uint32 unit, cvb_RtkPort port, const QosIgrBwCtrl_t *pIgrBwCtrl);
EXTERN_API RtkApiRet cvb_qos_igr_bw_ctrl_get(uint32 unit, cvb_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl);
/* Traffic Classification */
EXTERN_API RtkApiRet cvb_qos_port_pri_set(uint32 unit, cvb_RtkPort port, cvb_QosPri pri);
EXTERN_API RtkApiRet cvb_qos_port_pri_get(uint32 unit, cvb_RtkPort port, cvb_QosPri *pPri);
EXTERN_API RtkApiRet cvb_qos_pri_sel_set(uint32 unit, cvb_QosTblIdx group, const QosPriSel_t *pPriDec);
EXTERN_API RtkApiRet cvb_qos_pri_sel_get(uint32 unit, cvb_QosTblIdx group, QosPriSel_t *pPriDec);
EXTERN_API RtkApiRet cvb_qos_pri_wt_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx);
EXTERN_API RtkApiRet cvb_qos_pri_wt_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx);
EXTERN_API RtkApiRet cvb_qos_cvlan_pri_remap_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri iPri, cvb_QosDei iDei, cvb_QosPri mappingPri);
EXTERN_API RtkApiRet cvb_qos_cvlan_pri_remap_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri iPri, cvb_QosDei iDei, cvb_QosPri *pMappingPri);
EXTERN_API RtkApiRet cvb_qos_cvlan_remap_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx);
EXTERN_API RtkApiRet cvb_qos_cvlan_remap_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx);
EXTERN_API RtkApiRet cvb_qos_svlan_pri_remap_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oPri, cvb_QosDei oDei, cvb_QosPri mappingPri);
EXTERN_API RtkApiRet cvb_qos_svlan_pri_remap_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oPri, cvb_QosDei oDei, cvb_QosPri *pMappingPri);
EXTERN_API RtkApiRet cvb_qos_dscp_pri_remap_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosDscp dscp, cvb_QosPri mappingPri);
EXTERN_API RtkApiRet cvb_qos_dscp_pri_remap_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosDscp dscp, cvb_QosPri *pMappingPri);
EXTERN_API RtkApiRet cvb_qos_svlan_remap_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx);
EXTERN_API RtkApiRet cvb_qos_svlan_remap_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx);
EXTERN_API RtkApiRet cvb_qos_dscp_remap_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx);
EXTERN_API RtkApiRet cvb_qos_dscp_remap_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx);
EXTERN_API RtkApiRet cvb_qos_pri_que_mapping_set(uint32 unit, cvb_RtkPort port, cvb_QosPri pri, cvb_QosQueNum que);
EXTERN_API RtkApiRet cvb_qos_pri_que_mapping_get(uint32 unit, cvb_RtkPort port, cvb_QosPri pri, cvb_QosQueNum *pQue);
/* Traffic Scheduling */
EXTERN_API RtkApiRet cvb_qos_que_sch_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, RtkQoSSch queSch);
EXTERN_API RtkApiRet cvb_qos_que_sch_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, RtkQoSSch *pQueSch);
EXTERN_API RtkApiRet cvb_qos_que_wfq_wrr_wt_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_QosWt queWt);
EXTERN_API RtkApiRet cvb_qos_que_wfq_wrr_wt_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_QosWt *pQueWt);
/* Egress Bandwidth Control */
EXTERN_API RtkApiRet cvb_qos_egr_que_bw_ctrl_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_RtkEnable en, cvb_QosBw bw);
EXTERN_API RtkApiRet cvb_qos_egr_que_bw_ctrl_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_RtkEnable *pEn, cvb_QosBw *pQueBw);
/* Traffic Priority Remarking */
EXTERN_API RtkApiRet cvb_qos_cvlan_rmk_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en);
EXTERN_API RtkApiRet cvb_qos_cvlan_rmk_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn);
EXTERN_API RtkApiRet cvb_qos_svlan_rmk_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en);
EXTERN_API RtkApiRet cvb_qos_svlan_rmk_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn);
EXTERN_API RtkApiRet cvb_qos_dscp_rmk_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en);
EXTERN_API RtkApiRet cvb_qos_dscp_rmk_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn);
EXTERN_API RtkApiRet cvb_qos_cvlan_rmk_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri rmkPri, cvb_QosDei rmkDei);
EXTERN_API RtkApiRet cvb_qos_cvlan_rmk_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri *pRmkPri, cvb_QosDei *pRmkDei);
EXTERN_API RtkApiRet cvb_qos_svlan_rmk_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri rmkPri, cvb_QosDei rmkDei);
EXTERN_API RtkApiRet cvb_qos_svlan_rmk_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri *pRmkPri, cvb_QosDei *pRmkDei);
EXTERN_API RtkApiRet cvb_qos_dscp_rmk_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosDscp rmkDscp);
EXTERN_API RtkApiRet cvb_qos_dscp_rmk_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosDscp *pRmkDscp);
EXTERN_API RtkApiRet cvb_qos_cvlan_rmk_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx);
EXTERN_API RtkApiRet cvb_qos_cvlan_rmk_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx);
EXTERN_API RtkApiRet cvb_qos_svlan_rmk_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx);
EXTERN_API RtkApiRet cvb_qos_svlan_rmk_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx);
EXTERN_API RtkApiRet cvb_qos_dscp_rmk_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx);
EXTERN_API RtkApiRet cvb_qos_dscp_rmk_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx);

#endif /* CVB_RTK_API_QOS_H__ */
