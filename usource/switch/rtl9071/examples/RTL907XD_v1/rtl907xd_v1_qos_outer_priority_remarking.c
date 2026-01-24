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
#include "rtl907xd_v1_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup QOS_EXAMPLE QoS Example
 * @{
 */
/**
 * @addtogroup RTL907XD_V1_QOS_EXAMPLE RTL907XD_V1 QoS Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_QOS_OUTER_PRI_RMK_EXAMPLE QoS Outer Priority Remarking Example
 * @brief <b>QoS Outer Priority Remarking</b>
 *
 * @section Purpose
 *   Prioritize reliable traffic, ensure high performance, and maintain network stability.
 *
 * @section Description
 *   Adjust traffic priorities for optimal delivery and stability. \n
 *   As shown in the figure below, the switch received a packet with an outer tag priority of 4 and DEI of 0 at port 3.\n
 *   The switch then remarked the outer tag priority to 6 and its DEI to 0 at the outgoing port 1.
 *
 * @image html qos/rmk_outer_pri.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_qos_outer_priority_remarking(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_qos_outer_priority_remarking(void)
{
    RtkApiRet                   retVal;
    UnitChip_t                  unitChip;
    SvlanPortPktParserInfo_t    svlanPortPktParserInfo;
    SvlanFwdBaseOvidInfo_t      svlanFwdBaseOvidInfo;
    SvlanPortEgrTagStatusInfo_t svlanPortEgrTagStatusInfo;
    VlanInfo_t                  vlanInfo;
    QosPriSelInfo_t             qosPriSelInfo;
    QosPriWtTbl_t               qosPriWtTbl;
    QosSvlanPriRemap_t          wQosSvlanPriRemap;
    QosSvlanPriRemap_t          rQosSvlanPriRemap;
    QosSvlanRemapTbl_t          wQosSvlanRemapTbl;
    QosSvlanRemapTbl_t          rQosSvlanRemapTbl;
    QosSvlanRmkEn_t             wQosSvlanRmkEn;
    QosSvlanRmkEn_t             rQosSvlanRmkEn;
    QosSvlanRmk_t               wQosSvlanRmk;
    QosSvlanRmk_t               rQosSvlanRmk;
    QosSvlanRmkTbl_t            wQosSvlanRmkTbl;
    QosSvlanRmkTbl_t            rQosSvlanRmkTbl;

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(&svlanPortPktParserInfo, 0, sizeof(svlanPortPktParserInfo));
    CHK_FUN_RET(retVal, drv_memset(&svlanFwdBaseOvidInfo, 0, sizeof(svlanFwdBaseOvidInfo)));
    CHK_FUN_RET(retVal, drv_memset(&svlanPortEgrTagStatusInfo, 0, sizeof(svlanPortEgrTagStatusInfo)));
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosSvlanPriRemap, 0, sizeof(wQosSvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&rQosSvlanPriRemap, 0, sizeof(rQosSvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&wQosSvlanRemapTbl, 0, sizeof(wQosSvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosSvlanRmkTbl, 0, sizeof(rQosSvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosSvlanRmkEn, 0, sizeof(wQosSvlanRmkEn)));
    CHK_FUN_RET(retVal, drv_memset(&rQosSvlanRmkEn, 0, sizeof(rQosSvlanRmkEn)));
    CHK_FUN_RET(retVal, drv_memset(&wQosSvlanRmk, 0, sizeof(wQosSvlanRmk)));
    CHK_FUN_RET(retVal, drv_memset(&rQosSvlanRmk, 0, sizeof(rQosSvlanRmk)));
    CHK_FUN_RET(retVal, drv_memset(&wQosSvlanRmkTbl, 0, sizeof(wQosSvlanRmkTbl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosSvlanRmkTbl, 0, sizeof(rQosSvlanRmkTbl)));

    /* Step 1.1. Enable Otag parser */
    svlanPortPktParserInfo.port   = DV1_RTK_USER_PORT_3RD;
    svlanPortPktParserInfo.enable = ENABLED;
    CHK_FUN_RET(retVal, rtk_svlan_port_pkt_parser_en_set(unitChip, &svlanPortPktParserInfo));

    /* Step 1.2. Learning and forwarding based on outer VID */
    svlanFwdBaseOvidInfo.port   = DV1_RTK_USER_PORT_3RD;
    svlanFwdBaseOvidInfo.enable = ENABLED;
    CHK_FUN_RET(retVal, rtk_svlan_fwd_base_ovid_set(unitChip, &svlanFwdBaseOvidInfo));

    /* Step 1.3.
     * Set port egress tag status is both outer and inner tag when source port is UNI port
     * Set port egress tag status is both outer and inner tag when source port is NNI port
     */
    svlanPortEgrTagStatusInfo.port      = DV1_RTK_USER_PORT_1ST;
    svlanPortEgrTagStatusInfo.tagStatus = EM_TAG_STATUS_WITH_TAG;
    CHK_FUN_RET(retVal, rtk_svlan_port_egr_tag_status_set(unitChip, &svlanPortEgrTagStatusInfo));

    /* Step 1.4 Create VLANs */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_3RD;
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
    qosPriSelInfo.qosPriSel.svlanPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    /* Expected Behaviour:  In priority extraction weight table 0, only accept outer priority as source priority */

    /* Step 3. Set port 3 (ingress port) to use table 0 */
    qosPriWtTbl.port = DV1_RTK_USER_PORT_3RD;
    qosPriWtTbl.idx  = 0;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));
    /* Expected Behaviour: select priority extraction weight table 0 on port 3 (ingress port). */

    /* Step 4.1. Set outer tag remapping table 0. priority 4 & odei 0 mapping to internal priority 4 */
    wQosSvlanPriRemap.idx        = 0u;
    wQosSvlanPriRemap.oPri       = 4u;
    wQosSvlanPriRemap.oDei       = 0u;
    wQosSvlanPriRemap.mappingPri = 4u;
    rQosSvlanPriRemap.idx        = 0u;
    rQosSvlanPriRemap.oPri       = 4u;
    rQosSvlanPriRemap.oDei       = 0u;
    CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &wQosSvlanPriRemap));
    CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_get(unitChip, &rQosSvlanPriRemap));

    /* Step 4.2. Using outer tag remapping table 0 on port 3 */
    wQosSvlanRemapTbl.port = DV1_RTK_USER_PORT_3RD;
    wQosSvlanRemapTbl.idx  = 0;
    rQosSvlanRemapTbl.port = DV1_RTK_USER_PORT_3RD;
    CHK_FUN_RET(retVal, rtk_qos_svlan_remap_tbl_set(unitChip, &wQosSvlanRemapTbl));
    CHK_FUN_RET(retVal, rtk_qos_svlan_remap_tbl_get(unitChip, &rQosSvlanRemapTbl));

    /* Step 4.3. Enable outer tag priority remarking for port 1 */
    wQosSvlanRmkEn.port = DV1_RTK_USER_PORT_1ST;
    wQosSvlanRmkEn.en   = ENABLED;
    rQosSvlanRmkEn.port = DV1_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_qos_svlan_rmk_en_set(unitChip, &wQosSvlanRmkEn));
    CHK_FUN_RET(retVal, rtk_qos_svlan_rmk_en_get(unitChip, &rQosSvlanRmkEn));

    /* Step 4.4. Set outer tag priority remarking table 0, internal priority 4 remark to priority 6 & idei 0 */
    wQosSvlanRmk.idx    = 0u;
    wQosSvlanRmk.oriPri = 4u;
    wQosSvlanRmk.rmkPri = 6u;
    wQosSvlanRmk.rmkDei = 0u;
    rQosSvlanRmk.idx    = 0u;
    rQosSvlanRmk.oriPri = 4u;
    CHK_FUN_RET(retVal, rtk_qos_svlan_rmk_set(unitChip, &wQosSvlanRmk));
    CHK_FUN_RET(retVal, rtk_qos_svlan_rmk_get(unitChip, &rQosSvlanRmk));

    /* Step 4.5. Select outer tag priority remarking table 0 on port 1 */
    wQosSvlanRmkTbl.port = DV1_RTK_USER_PORT_1ST;
    wQosSvlanRmkTbl.idx  = 0u;
    rQosSvlanRmkTbl.port = DV1_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_qos_svlan_rmk_tbl_set(unitChip, &wQosSvlanRmkTbl));
    CHK_FUN_RET(retVal, rtk_qos_svlan_rmk_tbl_get(unitChip, &rQosSvlanRmkTbl));
    /* Expected Behaviour: When received a packet with inner tag priority 4 and dei 0 at port 3, and then remark the outer tag priority to 6 and its dei to 0 at outgoing of port 1. */

    return retVal;
}
/**@}*/ /* RTL907XD_V1_QOS_OUTER_PRI_RMK_EXAMPLE */
/**@}*/ /* RTL907XD_V1_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */
