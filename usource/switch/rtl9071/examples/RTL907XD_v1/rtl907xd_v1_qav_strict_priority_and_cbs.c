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
#include "rtl907xd_v1_api_qos.h"
#include "rtl907xd_v1_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup QAV_EXAMPLE Qav Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_QAV_EXAMPLE RTL907XD_V1 Qav Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_QAV_STRICT_PRI_CBS_EXAMPLE Qav Strict Priority and CBS Example
 * @brief <b>Qav Strict Priority and CBS</b>
 *
 * @section Purpose
 *   Ensure minimal delay for high-priority traffic by assigning it absolute priority, and regulate traffic flow to ensure fair bandwidth distribution and prevent congestion.
 *
 * @section Description
 *   Strict Priority prioritizes critical traffic types to guarantee their timely delivery and reduce latency, while CBS controls transmission rates using a credit system to balance high-priority traffic with overall network performance.\n
 *   To simplify the example, we assume the vlan and traffic classification, and priority-to-queue mapping are configured as linear mappings (0->0, 1->1, ¡K, 6->6, 7->7).
 *   (This example uses vlan id 100 and C-tag) \n\n
 *   As shown in the figure below, port 3 is configured to use strict priority (SP) on queues 0 to 5 and a credit-based shaper (CBS) on queues 6 and 7.
 *   The bandwidth for queue 7 is configured as 30 Mbps, and the bandwidth for queue 6 is configured as 20 Mbps.
 *
 * @image html qos/strict_pri_and_cbs_example.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_qav_strict_priority_and_cbs(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_qav_strict_priority_and_cbs(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    VlanInfo_t         vlanInfo;
    QosPriSelInfo_t    qosPriSelInfo;
    QosPriWtTbl_t      qosPriWtTbl;
    QosCvlanPriRemap_t qosCvlanPriRemap;
    QosCvlanRemapTbl_t qosCvlanRemapTbl;
    QosPriQueMapping_t qosPriQueMapping;
    QavCbs_t           wQavCbs[8];
    QavCbs_t           rQavCbs[8];
    RtkPort            port;
    RtkQoSPri          pri;
    RtkQoSQue          que;

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(&vlanInfo, 0, sizeof(vlanInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanPriRemap, 0, sizeof(qosCvlanPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&qosCvlanRemapTbl, 0, sizeof(qosCvlanRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriQueMapping, 0, sizeof(qosPriQueMapping)));
    CHK_FUN_RET(retVal, drv_memset(wQavCbs, 0, sizeof(wQavCbs)));
    CHK_FUN_RET(retVal, drv_memset(rQavCbs, 0, sizeof(rQavCbs)));

    /* Pre-configuration: setup the acceptable vlan. for more detail, please refer rtk_vlan_init.c */
    CHK_FUN_RET(retVal, rtk_vlan_init(unitChip));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = DV1_RTK_USER_PMAP_ALL;
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
    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; ++port)
    {
        /* port n use pri weight table 0 */
        qosPriWtTbl.port = port;
        CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));
    }

    /* Pcp to priority mapping */
    for(pri = 0u; pri <= QOS_PRI_MAX; pri++)
    {
        qosCvlanPriRemap.idx        = 0u;
        qosCvlanPriRemap.iPri       = pri;
        qosCvlanPriRemap.iDei       = 0u;
        qosCvlanPriRemap.mappingPri = pri;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
    }
    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; ++port)
    {
        /* Port n use cvlan remap table 0 */
        qosCvlanRemapTbl.port = port;
        qosCvlanRemapTbl.idx  = 0u;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_remap_tbl_set(unitChip, &qosCvlanRemapTbl));
    }

    /* Priority to queue mapping */
    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; ++port)
    {
        qosPriQueMapping.port = port;
        for(pri = 0u; pri <= QOS_PRI_MAX; ++pri)
        {
            qosPriQueMapping.pri = pri;
            qosPriQueMapping.que = pri;
            CHK_FUN_RET(retVal, rtk_qos_pri_que_mapping_set(unitChip, &qosPriQueMapping));
        }
    }

    /* Step 1. configure port 3 scheduler and bandwidth for each queue */
    /* queue 7 use cbs with bandwidth 30 Mbps */
    /* queue 6 use cbs with bandwidth 20 Mbps */
    /* queue 0-5 use sp */
    for(que = 0u; que <= QOS_QUE_MAX; que++)
    {
        wQavCbs[que].port = DV1_RTK_USER_PORT_3RD;
        wQavCbs[que].que  = que;
        if(que <= 5u)
        {
            wQavCbs[que].en = DISABLED;
            wQavCbs[que].bw = 0u;
        }
        else if(que <= 6u)
        {
            wQavCbs[que].en = ENABLED;
            wQavCbs[que].bw = 20000000u;
        }
        else
        {
            wQavCbs[que].en = ENABLED;
            wQavCbs[que].bw = 30000000u;
        }
        CHK_FUN_RET(retVal, rtk_qav_cbs_set(unitChip, &(wQavCbs[que])));
    }
    /* Expected Behaviour after step 1
     *	 queue 7 and queue 6 egress bandwidth is 30 Mbps and 20 Mbps.
     *	 queue 0-5 egress bandwidth is not limited.
     */

    /* Step 2. Get port 3 scheduler and bandwidth for each queue */
    for(que = 0u; que <= QOS_PRI_MAX; ++que)
    {
        rQavCbs[que].port = DV1_RTK_USER_PORT_3RD;
        rQavCbs[que].que  = que;
        CHK_FUN_RET(retVal, rtk_qav_cbs_get(unitChip, &(rQavCbs[que])));
    }

    return retVal;
}
/**@}*/ /* RTL907XD_V1_QAV_STRICT_PRI_CBS_EXAMPLE */
/**@}*/ /* RTL907XD_V1_QAV_EXAMPLE */
/**@}*/ /* QAV_EXAMPLE */
/**@}*/ /* EXAMPLE */