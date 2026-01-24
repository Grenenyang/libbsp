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

#include "rtkas_common_types.h"
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtkas_api_qos.h"
#include "rtkas_api_vlan.h"
#include "rtl907xd_va_api_qos.h"
#include "rtl907xd_va_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup QOS_EXAMPLE QoS Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_QOS_EXAMPLE RTL907XD_VA QoS Example
 * @{
 */
/**
 * @defgroup RTL907XD_VA_QOS_STRICT_PRI_WRR_EXAMPLE QoS Strict Priority and WRR Example
 * @brief <b>QoS Strict Priority and WRR</b>
 *
 * @section Purpose
 *   Ensure minimal delay for high-priority traffic by assigning it absolute priority, while also distributing bandwidth fairly among different traffic types to prevent congestion.
 *
 * @section Description
 *   Strict Priority prioritizes critical traffic to guarantee timely delivery and reduce latency, while Weighted Round Robin (WRR) allocates bandwidth based on assigned weights to balance overall network performance.
 *   As shown in the figure below, the switch receives 3 streams at port 1, then enqueues the streams into different TX queues of port 2, configured as follows:
 *   Set the weighted value for the inner tag priority source to the highest value in the priority extraction weight table.
 *   On port 1, map inner tag priority 3 to internal priority 7, inner tag priority 6 to internal priority 6, and inner tag priority 5 to internal priority 5.
 *   On port 2, map internal priority 7 to queue 7, internal priority 6 to queue 6, and internal priority 5 to queue 5.
 *   On port 2, set queue 7 as Strict Priority (SP) and queues 0 to 6 as Weighted Round Robin (WRR).
 *   On port 2, set the weight of queue 6 to 1 and the weight of queue 5 to 2. The others remain at their default value.
 *   On port 2, set the egress queue bandwidth of TX queue 7 to 800 * 8 Kbps.
 *   Note: WFQ (Weighted Fair Queuing) settings are similar to WRR. The difference between WRR and WFQ is that the weight of WRR is based on the number of packets, while the weight of WFQ is based on the length of the packet, and WFQ does not support queue bandwidth limitation.
 *
 * @image html qos/strict_pri_and_wrr_example.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_qos_strict_priority_and_wrr(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_qos_strict_priority_and_wrr(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    VlanInfo_t         vlanInfo;
    QosPriSelInfo_t    qosPriSelInfo;
    QosPriWtTbl_t      qosPriWtTbl;
    QosCvlanPriRemap_t qosCvlanPriRemap;
    QosCvlanRemapTbl_t qosCvlanRemapTbl;
    QosPriQueMapping_t qosPriQueMapping;
    QosQueSchInfo_t    queSchInfo;
    QosQueWfqWrrWt_t   wQosQueWfqWrrWt[2];
    QosQueWfqWrrWt_t   rQosQueWfqWrrWt[2];
    QosEgrQueBwCtrl_t  qosEgrQueBwCtrl;
    RtkQoSPri          pri;
    RtkQoSQue          que;

    /* Initialize struct */
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;
    retVal        = drv_memset(&vlanInfo, 0, sizeof(vlanInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanPriRemap, 0, sizeof(qosCvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanRemapTbl, 0, sizeof(qosCvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriQueMapping, 0, sizeof(qosPriQueMapping)));
    CHK_FUN_RET(retVal, drv_memset(&queSchInfo, 0, sizeof(queSchInfo)));
    CHK_FUN_RET(retVal, drv_memset(wQosQueWfqWrrWt, 0, sizeof(wQosQueWfqWrrWt)));
    CHK_FUN_RET(retVal, drv_memset(rQosQueWfqWrrWt, 0, sizeof(rQosQueWfqWrrWt)));
    CHK_FUN_RET(retVal, drv_memset(&qosEgrQueBwCtrl, 0, sizeof(qosEgrQueBwCtrl)));

    /* Step 1. Create VLANs */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behaviour: The frames with vid 100 will be flooded to port1, 2. */

    /* Step 2. Set priority extraction weight table 0 */
    qosPriSelInfo.group              = 0u;
    qosPriSelInfo.qosPriSel.cvlanPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    /* Expected Behaviour:  In priority extraction weight table 0, only accept inner priority as source priority. */

    /* Step 3. Set port 1 (ingress port) to use table 0 */
    qosPriWtTbl.port = DVA_RTK_USER_PORT_1ST;
    qosPriWtTbl.idx  = 0;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));
    /* Expected Behaviour: select priority extraction weight table 0 on port 1 (ingress port). */

    /* Step 4.1. Set innertag remapping table 0
     * inner priority 3 & idei 0 mapping to internal priority 7
     * inner priority 6 & idei 0 mapping to internal priority 6
     * inner priority 5 & idei 0 mapping to internal priority 5
     */
    qosCvlanPriRemap.idx        = 0u;
    qosCvlanPriRemap.iPri       = 3u;
    qosCvlanPriRemap.iDei       = 0u;
    qosCvlanPriRemap.mappingPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.iPri       = 6u;
    qosCvlanPriRemap.mappingPri = 6u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    qosCvlanPriRemap.iPri       = 5u;
    qosCvlanPriRemap.mappingPri = 5u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));

    /* Step 4.2. Using inner tag remapping table 0 on port 1 */
    qosCvlanRemapTbl.port = DVA_RTK_USER_PORT_1ST;
    qosCvlanRemapTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_remap_tbl_set(unitChip, &qosCvlanRemapTbl));

    /* Step 4.3. On port 2, set the internal priority 7 maps to queue 7, and the internal priority 6 maps to queue 6, and the internal priority 5 maps to queue 5. */
    for(pri = 5u; pri <= QOS_QUE_MAX; pri++)
    {
        qosPriQueMapping.port = DVA_RTK_USER_PORT_2ND;
        qosPriQueMapping.pri  = pri;
        qosPriQueMapping.que  = pri;
        CHK_FUN_RET(retVal, rtk_qos_pri_que_mapping_set(unitChip, &qosPriQueMapping));
    }

    /* Step 4.4. On port 2, set queue 7 as Strict Priority (SP), and queue 0~6 as Weighted Round Robin (WRR). */
    queSchInfo.port   = DVA_RTK_USER_PORT_2ND;
    queSchInfo.que    = 7u;
    queSchInfo.queSch = DVA_QOS_SCH_SP;
    CHK_FUN_RET(retVal, rtk_qos_que_sch_set(unitChip, &queSchInfo));
    for(que = 0; que < QOS_QUE_MAX; que++)
    {
        queSchInfo.que    = que;
        queSchInfo.queSch = DVA_QOS_SCH_WRR;
        CHK_FUN_RET(retVal, rtk_qos_que_sch_set(unitChip, &queSchInfo));
    }

    /* Step 4.5. On port 2, set the weight of queue 6 to 1, and set the weight of queue 5 to 2. The others keep default value. */
    wQosQueWfqWrrWt[0].port  = DVA_RTK_USER_PORT_2ND;
    wQosQueWfqWrrWt[0].que   = 6u;
    wQosQueWfqWrrWt[0].queWt = 1u;
    wQosQueWfqWrrWt[1].port  = DVA_RTK_USER_PORT_2ND;
    wQosQueWfqWrrWt[1].que   = 5u;
    wQosQueWfqWrrWt[1].queWt = 2u;
    rQosQueWfqWrrWt[0].port  = DVA_RTK_USER_PORT_2ND;
    rQosQueWfqWrrWt[0].que   = 6u;
    rQosQueWfqWrrWt[1].port  = DVA_RTK_USER_PORT_2ND;
    rQosQueWfqWrrWt[1].que   = 5u;
    CHK_FUN_RET(retVal, rtk_qos_que_wfq_wrr_wt_set(unitChip, &(wQosQueWfqWrrWt[0])));
    CHK_FUN_RET(retVal, rtk_qos_que_wfq_wrr_wt_set(unitChip, &(wQosQueWfqWrrWt[1])));
    CHK_FUN_RET(retVal, rtk_qos_que_wfq_wrr_wt_get(unitChip, &(rQosQueWfqWrrWt[0])));
    CHK_FUN_RET(retVal, rtk_qos_que_wfq_wrr_wt_get(unitChip, &(rQosQueWfqWrrWt[1])));
    /* Expected Behaviour:
     *   Queue 7 is Strict Priority (SP), and queue 0~6 is Weighted Round Robin (WRR)
     *   The weight of queue 6 is 1, and the weight of queue 5 is 2, and the others keep default
     */

    /* Step 5. On port 2, set egress queue bandwidth of TX queue 7 to 800 * 8 Kbps. */
    qosEgrQueBwCtrl.port = DVA_RTK_USER_PORT_2ND;
    qosEgrQueBwCtrl.que  = 7u;
    qosEgrQueBwCtrl.en   = ENABLED;
    qosEgrQueBwCtrl.bw   = 800u;
    CHK_FUN_RET(retVal, rtk_qos_egr_que_bw_ctrl_set(unitChip, &qosEgrQueBwCtrl));
    /* Expected Behaviour: Q7 bandwidth of port 1 is 800 * 8 Kbps */

    return retVal;
}
/**@}*/ /* RTL907XD_VA_QOS_STRICT_PRI_WRR */
/**@}*/ /* RTL907XD_VA_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */
