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
#include "rtl907xc_va_api_qos.h"
#include "rtl907xc_va_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup QOS_EXAMPLE QoS Example
 * @{
 */
/**
 * @addtogroup RTL907XC_VA_QOS_EXAMPLE RTL907XC_VA QoS Example
 * @{
 */
/**
 * @defgroup RTL907XC_VA_QOS_QUE_BW_EXAMPLE QoS Queue Bandwidth Limit Example
 * @brief <b>QoS Queue Bandwidth Limit</b>
 *
 * @section Purpose
 *   QoS queue bandwidth limiting is to control the amount of traffic allowed through a network interface, ensuring fair bandwidth distribution, preventing network congestion, and maintaining stable network performance by prioritizing critical traffic over less important data.
 *
 * @section Description
 *   QoS queue bandwidth limiting controls the traffic bandwidth for specific queues in a network device, ensuring prioritized and fair bandwidth allocation.\n
 *   This prevents congestion, ensures critical traffic gets the necessary bandwidth, and maintains stable network performance.
 *   This example sets the egress queue bandwidth of TX queue 1 of port 1 to 2500 * 8 Kbps and the egress queue bandwidth of TX queue 2 of port 1 to 1250 * 8 Kbps.
 *
 * @image html qos/que_bw_limit.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_qos_queue_bw_limit(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_qos_queue_bw_limit(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    VlanInfo_t         vlanInfo;
    QosPriSelInfo_t    qosPriSelInfo;
    QosPriWtTbl_t      qosPriWtTbl;
    QosCvlanPriRemap_t qosCvlanPriRemap;
    QosCvlanRemapTbl_t qosCvlanRemapTbl;
    QosQueSchInfo_t    queSchInfo;
    QosEgrQueBwCtrl_t  qosEgrQueBwCtrl;
    RtkQoSPri          pri;
    RtkQoSQue          que;

    /* Initialize struct */
    unitChip.unit = CVA_UNIT;
    unitChip.chip = CHIP_907XC_VA;
    retVal        = drv_memset(&vlanInfo, 0, sizeof(vlanInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanPriRemap, 0, sizeof(qosCvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanRemapTbl, 0, sizeof(qosCvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&queSchInfo, 0, sizeof(queSchInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosEgrQueBwCtrl, 0, sizeof(qosEgrQueBwCtrl)));

    /* Step 1. Create VLANs */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_3RD;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behaviour: The frames with vid 100 will be flooded to port1, 3. */

    /* Step 2. Set priority extraction weight table 0 */
    qosPriSelInfo.group              = 0u;
    qosPriSelInfo.qosPriSel.cvlanPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    /* Expected Behaviour: In priority extraction weight table 0, only accept inner priority as source priority */

    /* Step 3. Set port 3 (ingress port) to use table 0 */
    qosPriWtTbl.port = CVA_RTK_USER_PORT_3RD;
    qosPriWtTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));
    /* Expected Behaviour: select priority extraction weight table 0 on port 3 (ingress port). */

    /* Step 4.1. Set innertag remapping table 0
     * inner priority 0 & idei 0 mapping to internal priority 0
     * inner priority 1 & idei 0 mapping to internal priority 1
     * inner priority 2 & idei 0 mapping to internal priority 2
     * inner priority 3 & idei 0 mapping to internal priority 3
     * inner priority 4 & idei 0 mapping to internal priority 4
     * inner priority 5 & idei 0 mapping to internal priority 5
     * inner priority 6 & idei 0 mapping to internal priority 6
     * inner priority 7 & idei 0 mapping to internal priority 7
     */
    for(pri = 0u; pri <= QOS_PRI_MAX; pri++)
    {
        qosCvlanPriRemap.idx        = 0u;
        qosCvlanPriRemap.iPri       = pri;
        qosCvlanPriRemap.iDei       = 0u;
        qosCvlanPriRemap.mappingPri = pri;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    }

    /* Step 4.2. Using inner tag remapping table 0 on port 1 */
    qosCvlanRemapTbl.port = CVA_RTK_USER_PORT_1ST;
    qosCvlanRemapTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_remap_tbl_set(unitChip, &qosCvlanRemapTbl));

    /* Setp 4.3. On port 1 (egress port), set queue 0~7 as Strict Priority (SP). */
    queSchInfo.port   = CVA_RTK_USER_PORT_1ST;
    queSchInfo.queSch = CVA_QOS_SCH_SP;
    for(que = 0u; que <= QOS_QUE_MAX; que++)
    {
        queSchInfo.que = que;
        CHK_FUN_RET(retVal, rtk_qos_que_sch_set(unitChip, &queSchInfo));
    }
    /* Expected Behaviour: Queue 0~7 are set as Strict Priority on port 1 (egress port). */

    /* Step 5. Set egress queue bandwidth.
     * Set Q1 bandwidth of port 1 (egress) to 2500 * 8 Kbps
     * Set Q2 bandwidth of port 1 (egress) to 1250 * 8 Kbps
     */
    qosEgrQueBwCtrl.port = CVA_RTK_USER_PORT_1ST;
    qosEgrQueBwCtrl.que  = 1u;
    qosEgrQueBwCtrl.en   = ENABLED;
    qosEgrQueBwCtrl.bw   = 2500u;
    CHK_FUN_RET(retVal, rtk_qos_egr_que_bw_ctrl_set(unitChip, &qosEgrQueBwCtrl));
    qosEgrQueBwCtrl.que = 2u;
    qosEgrQueBwCtrl.bw  = 1250u;
    CHK_FUN_RET(retVal, rtk_qos_egr_que_bw_ctrl_set(unitChip, &qosEgrQueBwCtrl));
    /* Expected Behaviour: Q1 bandwidth of port 1 (egress) is 2500 * 8 Kbps, and Q2 bandwidth of port 1 (egress) is 1250 * 8 Kbps. */

    return retVal;
}
/**@}*/ /* RTL907XC_VA_QOS_QUE_BW_LIMIT_EXAMPLE */
/**@}*/ /* RTL907XC_VA_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */
