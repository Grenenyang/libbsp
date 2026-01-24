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
#include "rtkas_api_qav.h"
#include "rtkas_api_vlan.h"
#include "rtl907xc_vb_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup QAV_EXAMPLE Qav Example
 * @{
 */
/**
 * @defgroup RTL907XC_VB_QAV_EXAMPLE RTL907XC_VB Qav Example
 * @{
 */
/**
 * @defgroup RTL907XC_VB_QAV_PRI_REGEN_OVERRIDE_EXAMPLE Qav Priority Regeneration Override Example
 * @brief <b>Qav Priority Regeneration Override</b>
 *
 * @section Purpose
 *   The Qav priority regeneration override function is used to manage and control the priorities of network traffic, ensuring critical data is transmitted with priority, thereby improving network performance.
 *
 * @section Description
 *   This example illustrates the flexibility of configuring the priority regeneration override table.\n
 *   To simplify the example, we assume that the VLAN and traffic classification, and priority-to-queue mapping are configured as linear mappings (0->0, 1->1, ..., 6->6, 7->7). (The example uses VLAN ID 100 and C-tag.)\n
 * Example 1 is ingress on p1 and egress on p3, with the priority regeneration override not enabled for this traffic.\n
 * Example 2 is ingress on p2 and egress on p3, with the priority regeneration override enabled for this traffic on C-tags.\n
 * The following table shows the configuration of priority regeneration override values and the expected results on p3. (When priority override is used, the priority after override will be used when enqueuing the frame, and the PCP of the selected VLAN tag will also be changed.)\n\n
 *
 * | Receive on Port 1 |     	     	   |                   |                   |
 * | :---------------- | :---------------- | :---------------- | :---------------- |
 * | Piroirty          | Override enable   | Override value    | Expected result   |
 * | 0                 | disable           | N/A               | 0                 |
 * | 1                 | disable           | N/A               | 1                 |
 * | 2                 | disable           | N/A               | 2                 |
 * | 3                 | disable           | N/A               | 3                 |
 * | 4                 | disable           | N/A               | 4                 |
 * | 5                 | disable           | N/A               | 5                 |
 * | 6                 | disable           | N/A               | 6                 |
 * | 7                 | disable           | N/A               | 7                 |
 * \n
 * | Receive on Port 2 |                   |                   |                   |
 * | :---------------- | :---------------- | :---------------- | :---------------- |
 * | Piroirty          | Override enable   | Override value    | Expected result   |
 * | 0                 | enable            | 0                 | 0                 |
 * | 1                 | enable            | 0                 | 0                 |
 * | 2                 | enable            | 0                 | 0                 |
 * | 3                 | enable            | 0                 | 0                 |
 * | 4                 | enable            | 1                 | 1                 |
 * | 5                 | enable            | 2                 | 2                 |
 * | 6                 | enable            | 3                 | 3                 |
 * | 7                 | enable            | 4                 | 4                 |
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_qav_priority_regeneration_override(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_qav_priority_regeneration_override(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    VlanInfo_t         vlanInfo;
    QosPriSelInfo_t    qosPriSelInfo;
    QosPriWtTbl_t      qosPriWtTbl;
    QosCvlanPriRemap_t wQosCvlanPriRemap;
    QosCvlanPriRemap_t rQosCvlanPriRemap[8];
    QosCvlanRemapTbl_t wQosCvlanRemapTbl;
    QosCvlanRemapTbl_t rQosCvlanRemapTbl;
    QosPriQueMapping_t wQosPriQueMapping;
    QosPriQueMapping_t rQosPriQueMapping[8];
    QavRegenTagSel_t   wQavRegenTagSel;
    QavRegenTagSel_t   rQavRegenTagSel[2];
    QavPriRegen_t      wQavPriRegen;
    QavPriRegen_t      rQavPriRegen[8];
    RtkPort            port;
    RtkQoSPri          pri;
    RtkQoSPri          oriPri;

    /* Initialize struct */
    unitChip.unit = CVB_UNIT;
    unitChip.chip = CHIP_907XC_VB;
    retVal        = drv_memset(&vlanInfo, 0, sizeof(vlanInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosCvlanPriRemap, 0, sizeof(wQosCvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(rQosCvlanPriRemap, 0, sizeof(rQosCvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&wQosCvlanRemapTbl, 0, sizeof(wQosCvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosCvlanRemapTbl, 0, sizeof(rQosCvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosPriQueMapping, 0, sizeof(wQosPriQueMapping)));
    CHK_FUN_RET(retVal, drv_memset(rQosPriQueMapping, 0, sizeof(rQosPriQueMapping)));
    CHK_FUN_RET(retVal, drv_memset(&wQavRegenTagSel, 0, sizeof(wQavRegenTagSel)));
    CHK_FUN_RET(retVal, drv_memset(rQavRegenTagSel, 0, sizeof(rQavRegenTagSel)));
    CHK_FUN_RET(retVal, drv_memset(&wQavPriRegen, 0, sizeof(wQavPriRegen)));
    CHK_FUN_RET(retVal, drv_memset(rQavPriRegen, 0, sizeof(rQavPriRegen)));

    /* Pre-configuration: setup the acceptable vlan. for more detail, please refer rtk_vlan_init.c */
    CHK_FUN_RET(retVal, rtk_vlan_init(unitChip));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = CVB_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Pre-configuration: mapping priority frame to corresponding queue, please refer qos_hybridBasedPriority.c */
    /* linear mapping, the pcp x will be pri x, and mapping to queue x */
    qosPriSelInfo.group              = 0u;
    qosPriSelInfo.qosPriSel.cvlanPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    qosPriWtTbl.idx = 0;
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; ++port)
    {
        /* port n use pri weight table 0 */
        qosPriWtTbl.port = port;
        CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));
    }

    /* Set pcp to priority mapping */
    wQosCvlanPriRemap.idx        = 0u;
    wQosCvlanPriRemap.iPri       = 0u;
    wQosCvlanPriRemap.iDei       = 0u;
    wQosCvlanPriRemap.mappingPri = 0u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));
    wQosCvlanPriRemap.iPri       = 1u;
    wQosCvlanPriRemap.mappingPri = 1u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));
    wQosCvlanPriRemap.iPri       = 2u;
    wQosCvlanPriRemap.mappingPri = 2u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));
    wQosCvlanPriRemap.iPri       = 3u;
    wQosCvlanPriRemap.mappingPri = 3u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));
    wQosCvlanPriRemap.iPri       = 4u;
    wQosCvlanPriRemap.mappingPri = 4u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));
    wQosCvlanPriRemap.iPri       = 5u;
    wQosCvlanPriRemap.mappingPri = 5u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));
    wQosCvlanPriRemap.iPri       = 6u;
    wQosCvlanPriRemap.mappingPri = 6u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));
    wQosCvlanPriRemap.iPri       = 7u;
    wQosCvlanPriRemap.mappingPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &wQosCvlanPriRemap));

    /* Get pcp to priority mapping */
    for(pri = 0u; pri <= QOS_PRI_MAX; pri++)
    {
        rQosCvlanPriRemap[pri].idx  = 0u;
        rQosCvlanPriRemap[pri].iPri = pri;
        rQosCvlanPriRemap[pri].iDei = 0u;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_get(unitChip, &(rQosCvlanPriRemap[pri])));
    }

    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; ++port)
    {
        /* port n use cvlan remap table 0 */
        wQosCvlanRemapTbl.port = port;
        wQosCvlanRemapTbl.idx  = 0u;
        rQosCvlanRemapTbl.port = port;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_remap_tbl_set(unitChip, &wQosCvlanRemapTbl));
        CHK_FUN_RET(retVal, rtk_qos_cvlan_remap_tbl_get(unitChip, &rQosCvlanRemapTbl));
    }

    /* priority to queue mapping */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; ++port)
    {
        wQosPriQueMapping.port = port;
        for(pri = 0u; pri <= QOS_QUE_MAX; ++pri)
        {
            wQosPriQueMapping.pri       = pri;
            wQosPriQueMapping.que       = pri;
            rQosPriQueMapping[pri].port = port;
            rQosPriQueMapping[pri].pri  = pri;
            CHK_FUN_RET(retVal, rtk_qos_pri_que_mapping_set(unitChip, &wQosPriQueMapping));
            CHK_FUN_RET(retVal, rtk_qos_pri_que_mapping_get(unitChip, &(rQosPriQueMapping[pri])));
        }
    }

    /* Step 1.1. setup the priority regeneration override target vlan tag */
    /* enable change pcp for C-tag on port 2*/
    wQavRegenTagSel.port   = CVB_RTK_USER_PORT_1ST;
    wQavRegenTagSel.enIpri = DISABLED;
    wQavRegenTagSel.enOpri = DISABLED;
    CHK_FUN_RET(retVal, rtk_qav_regen_tag_sel_set(unitChip, &wQavRegenTagSel));
    wQavRegenTagSel.port   = CVB_RTK_USER_PORT_2ND;
    wQavRegenTagSel.enIpri = ENABLED;
    wQavRegenTagSel.enOpri = DISABLED;
    CHK_FUN_RET(retVal, rtk_qav_regen_tag_sel_set(unitChip, &wQavRegenTagSel));
    /* Expected Behaviour
     *     after priority regeneration override is configured,
     *     when egress regeneration override will not change tag from p1, will change tag from p2.
     */

    /* Step 1.2. Get the priority regeneration override target vlan tag */
    rQavRegenTagSel[0].port = CVB_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_qav_regen_tag_sel_get(unitChip, &(rQavRegenTagSel[0])));
    rQavRegenTagSel[1].port = CVB_RTK_USER_PORT_2ND;
    CHK_FUN_RET(retVal, rtk_qav_regen_tag_sel_get(unitChip, &(rQavRegenTagSel[1])));

    /* Step 2.1. setup the priority regeneration override table for port 1 as example */
    wQavPriRegen.port     = CVB_RTK_USER_PORT_1ST;
    wQavPriRegen.oriPri   = 0u;
    wQavPriRegen.en       = DISABLED;
    wQavPriRegen.regenPri = 0u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 1u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 2u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 3u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 4u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 5u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 6u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 7u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    /* Expected Behaviour
     *     ingress frame on port 1 does not change priority, and does not change pcp when egress
     */

    /* Step 2.2. setup the priority regeneration override table for port 2 as example */
    wQavPriRegen.port     = CVB_RTK_USER_PORT_2ND;
    wQavPriRegen.oriPri   = 0u;
    wQavPriRegen.en       = ENABLED;
    wQavPriRegen.regenPri = 0u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 1u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 2u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri = 3u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri   = 4u;
    wQavPriRegen.regenPri = 1u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri   = 5u;
    wQavPriRegen.regenPri = 2u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri   = 6u;
    wQavPriRegen.regenPri = 3u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    wQavPriRegen.oriPri   = 7u;
    wQavPriRegen.regenPri = 4u;
    CHK_FUN_RET(retVal, rtk_qav_pri_regen_set(unitChip, &wQavPriRegen));
    /* Expected Behaviour
     *     ingress frame on port 2 with pcp 0~3 will use priority 0 for enqueue and change pcp to 0 when egress
     *     ingress frame on port 2 with pcp 4 will use priority 1 for enqueue and change pcp to 1 when egress
     *     ingress frame on port 2 with pcp 5 will use priority 2 for enqueue and change pcp to 2 when egress
     *     ingress frame on port 2 with pcp 6 will use priority 3 for enqueue and change pcp to 3 when egress
     *     ingress frame on port 2 with pcp 7 will use priority 4 for enqueue and change pcp to 4 when egress
     */

    /* Step 2.3. Get the priority regeneration override table for port 1 */
    for(oriPri = 0u; oriPri <= QOS_PRI_MAX; ++oriPri)
    {
        rQavPriRegen[oriPri].port   = CVB_RTK_USER_PORT_1ST;
        rQavPriRegen[oriPri].oriPri = oriPri;
        CHK_FUN_RET(retVal, rtk_qav_pri_regen_get(unitChip, &(rQavPriRegen[oriPri])));
    }

    return retVal;
}
/**@}*/ /* RTL907XC_VB_QAV_PRI_REGEN_OVERRIDE_EXAMPLE */
/**@}*/ /* RTL907XC_VB_QAV_EXAMPLE */
/**@}*/ /* QAV_EXAMPLE */
/**@}*/ /* EXAMPLE */