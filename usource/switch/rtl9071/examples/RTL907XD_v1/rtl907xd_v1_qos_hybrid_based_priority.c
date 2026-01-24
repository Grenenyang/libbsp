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
 * @defgroup RTL907XD_V1_QOS_HYBRID_BASED_PRI_EXAMPLE QoS Hybrid-based Priority Example
 * @brief <b>QoS Hybrid-based Priority</b>
 *
 * @section Purpose
 *   Optimize traffic management by combining multiple priority mechanisms.
 *
 * @section Description
 *   Enhance QoS performance using hybrid priority, where the priority comes from either port-based or tagged-based sources. \n
 *   For packets coming from port 3, if those packets have a priority value in their VLAN tag, use the provided priority value.\n
 *   Otherwise, the switch assigns a default internal priority of 1 to those packets.
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_qos_hybrid_based_priority(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_qos_hybrid_based_priority(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    VlanInfo_t         vlanInfo;
    QosPriSelInfo_t    qosPriSelInfo;
    QosPriWtTbl_t      qosPriWtTbl;
    QosCvlanPriRemap_t qosCvlanPriRemap;
    QosCvlanRemapTbl_t qosCvlanRemapTbl;
    QosSvlanPriRemap_t qosSvlanPriRemap;
    QosSvlanRemapTbl_t qosSvlanRemapTbl;
    VlanPortPvidInfo_t vlanPortPvidInfo;
    RtkQoSPri          pri;

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(&vlanInfo, 0, sizeof(vlanInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanPriRemap, 0, sizeof(qosCvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanRemapTbl, 0, sizeof(qosCvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosSvlanPriRemap, 0, sizeof(qosSvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&qosSvlanRemapTbl, 0, sizeof(qosSvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));

    /* Step 1. Create VLANs */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = DV1_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behaviour: The frames with vid 100 will be flooded to port0~14. */

    /* Step 2. Set priority extraction weight table 0 */
    qosPriSelInfo.group               = 0u;
    qosPriSelInfo.qosPriSel.hybridPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    /* Expected Behaviour:  In priority extraction weight table 0, only accept hybrid priority as source priority */

    /* Step 3.1. Select priority extraction weight table 0 on port 3 */
    qosPriWtTbl.port = DV1_RTK_USER_PORT_3RD;
    qosPriWtTbl.idx  = 0;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));

    /* Step 3.2. Set default VID = 1 and vlan port-based priority = 1 on port 3 */
    vlanPortPvidInfo.port     = DV1_RTK_USER_PORT_3RD;
    vlanPortPvidInfo.pvid     = 1u;
    vlanPortPvidInfo.priority = 1u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    /* Step 3.3. Set innertag remapping table 0
     * inner priority 0 & idei 0 mapping to internal priority 0
     * inner priority 1 & idei 0 mapping to internal priority 1
     * inner priority 2 & idei 0 mapping to internal priority 2
     * inner priority 3 & idei 0 mapping to internal priority 3
     * inner priority 4 & idei 0 mapping to internal priority 4
     * inner priority 5 & idei 0 mapping to internal priority 5
     * inner priority 6 & idei 0 mapping to internal priority 6
     * inner priority 7 & idei 0 mapping to internal priority 7
     */
    for(pri = 0u; pri <= QOS_QUE_MAX; pri++)
    {
        qosCvlanPriRemap.idx        = 0u;
        qosCvlanPriRemap.iPri       = pri;
        qosCvlanPriRemap.iDei       = 0u;
        qosCvlanPriRemap.mappingPri = pri;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    }

    /* Step 3.4. Using inner tag remapping table 0 on port 3 */
    qosCvlanRemapTbl.port = DV1_RTK_USER_PORT_3RD;
    qosCvlanRemapTbl.idx  = 0;
    CHK_FUN_RET(retVal, rtk_qos_cvlan_remap_tbl_set(unitChip, &qosCvlanRemapTbl));

    /* Step 3.5. Set outertag remapping table 0
     * outer priority 0 & idei 0 mapping to internal priority 0
     * outer priority 1 & idei 0 mapping to internal priority 1
     * outer priority 2 & idei 0 mapping to internal priority 2
     * outer priority 3 & idei 0 mapping to internal priority 3
     * outer priority 4 & idei 0 mapping to internal priority 4
     * outer priority 5 & idei 0 mapping to internal priority 5
     * outer priority 6 & idei 0 mapping to internal priority 6
     * outer priority 7 & idei 0 mapping to internal priority 7
     */
    CHK_FUN_RET(retVal, drv_memset(&qosSvlanPriRemap, 0, sizeof(qosSvlanPriRemap)));
    for(pri = 0u; pri <= QOS_QUE_MAX; pri++)
    {
        qosSvlanPriRemap.idx        = 0u;
        qosSvlanPriRemap.oPri       = pri;
        qosSvlanPriRemap.oDei       = 0u;
        qosSvlanPriRemap.mappingPri = pri;
        CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
    }

    /* Step 3.6. Using outer tag remapping table 0 on port 3 */
    qosSvlanRemapTbl.port = DV1_RTK_USER_PORT_3RD;
    qosSvlanRemapTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_svlan_remap_tbl_set(unitChip, &qosSvlanRemapTbl));
    /* Expected Behaviour: The priority will be mapped to the corresponding internal priority according to the mapping table */

    return retVal;
}
/**@}*/ /* RTL907XD_V1_QOS_HYBRID_BASED_PRI_EXAMPLE */
/**@}*/ /* RTL907XD_V1_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */
