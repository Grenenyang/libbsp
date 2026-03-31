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
 * @defgroup RTL907XD_VA_QOS_INNER_PRI_RMK_EXAMPLE QoS Inner Priority Remarking Example
 * @brief <b>QoS Inner Priority Remarking</b>
 *
 * @section Purpose
 *   Prioritize reliable traffic, ensure high performance, and maintain network stability.
 *
 * @section Description
 *   Adjust traffic priorities for optimal delivery and stability. \n
 *   As shown in the figure below, the switch received a packet with an inner tag priority of 4 and DEI of 0 at port 3.\n
 *   The switch then remarked the inner tag priority to 6 and its DEI to 0 at the outgoing port 1.
 *
 * @image html qos/rmk_pri.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_qos_inner_priority_remarking(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_qos_inner_priority_remarking(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    VlanInfo_t         vlanInfo;
    QosPriSelInfo_t    qosPriSelInfo;
    QosPriWtTbl_t      qosPriWtTbl;
    QosCvlanPriRemap_t qosCvlanPriRemap;
    QosCvlanRemapTbl_t qosCvlanRemapTbl;
    QosCvlanRmkEn_t    qosCvlanRmkEn;
    QosCvlanRmkEn_t    rQosCvlanRmkEn;
    QosCvlanRmk_t      wQosCvlanRmk;
    QosCvlanRmk_t      rQosCvlanRmk;
    QosCvlanRmkTbl_t   wQosCvlanRmkTbl;
    QosCvlanRmkTbl_t   rQosCvlanRmkTbl;

    /* Initialize struct */
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;
    retVal        = drv_memset(&vlanInfo, 0, sizeof(vlanInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanPriRemap, 0, sizeof(qosCvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanRemapTbl, 0, sizeof(qosCvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanRmkEn, 0, sizeof(qosCvlanRmkEn)));
    CHK_FUN_RET(retVal, drv_memset(&rQosCvlanRmkEn, 0, sizeof(rQosCvlanRmkEn)));
    CHK_FUN_RET(retVal, drv_memset(&wQosCvlanRmk, 0, sizeof(wQosCvlanRmk)));
    CHK_FUN_RET(retVal, drv_memset(&rQosCvlanRmk, 0, sizeof(rQosCvlanRmk)));
    CHK_FUN_RET(retVal, drv_memset(&wQosCvlanRmkTbl, 0, sizeof(wQosCvlanRmkTbl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosCvlanRmkTbl, 0, sizeof(rQosCvlanRmkTbl)));

    /* Step 1. Create VLANs */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_3RD;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behaviour: The frames with vid 100 will be flooded to port1 and port3. */

    /* Step 2. Set priority extraction weight table 0 */
    qosPriSelInfo.group              = 0u;
    qosPriSelInfo.qosPriSel.cvlanPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    /* Expected Behaviour:  In priority extraction weight table 0, only accept inner priority as source priority */

    /* Step 3. Set port 3 (ingress port) to use table 0 */
    qosPriWtTbl.port = DVA_RTK_USER_PORT_3RD;
    qosPriWtTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));
    /* Expected Behaviour: select priority extraction weight table 0 on port 3 (ingress port). */

    /* Step 4.1. Set inner tag remapping table 0. priority 4 & idei 0 mapping to internal priority 4 */
    qosCvlanPriRemap.idx        = 0u;
    qosCvlanPriRemap.iPri       = 4u;
    qosCvlanPriRemap.iDei       = 0u;
    qosCvlanPriRemap.mappingPri = 4u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));

    /* Step 4.2. Using inner tag remapping table 0 on port 3 */
    qosCvlanRemapTbl.port = DVA_RTK_USER_PORT_3RD;
    qosCvlanRemapTbl.idx  = 0;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_remap_tbl_set(unitChip, &qosCvlanRemapTbl));

    /* Step 4.3. Enable inner tag priority remarking for port 1 */
    qosCvlanRmkEn.port  = DVA_RTK_USER_PORT_1ST;
    qosCvlanRmkEn.en    = ENABLED;
    rQosCvlanRmkEn.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_rmk_en_set(unitChip, &qosCvlanRmkEn));
    CHK_FUN_RET(retVal, rtk_qos_cvlan_rmk_en_get(unitChip, &rQosCvlanRmkEn));

    /* Step 4.4. Set inner tag priority remarking table 0, internal priority 4 remark to priority 6 & idei 0 */
    wQosCvlanRmk.idx    = 0u;
    wQosCvlanRmk.oriPri = 4u;
    wQosCvlanRmk.rmkPri = 6u;
    wQosCvlanRmk.rmkDei = 0u;
    rQosCvlanRmk.idx    = 0u;
    rQosCvlanRmk.oriPri = 4u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_rmk_set(unitChip, &wQosCvlanRmk));
    CHK_FUN_RET(retVal, rtk_qos_cvlan_rmk_get(unitChip, &rQosCvlanRmk));

    /* Step 4.5. Select inner tag priority remarking table 0 on port 1 */
    wQosCvlanRmkTbl.port = DVA_RTK_USER_PORT_1ST;
    wQosCvlanRmkTbl.idx  = 0u;
    rQosCvlanRmkTbl.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_rmk_tbl_set(unitChip, &wQosCvlanRmkTbl));
    CHK_FUN_RET(retVal, rtk_qos_cvlan_rmk_tbl_get(unitChip, &rQosCvlanRmkTbl));
    /* Expected Behaviour: When received a packet with inner tag priority 4 and dei 0 at port 3, and then remark the inner tag priority to 6 and its dei to 0 at outgoing of port 1. */

    return retVal;
}
/**@}*/ /* RTL907XD_VA_QOS_INNER_PRI_RMK_EXAMPLE */
/**@}*/ /* RTL907XD_VA_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */
