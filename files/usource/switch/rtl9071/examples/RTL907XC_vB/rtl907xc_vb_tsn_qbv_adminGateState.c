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

/* include file */
#include "rtkas_api_ext.h"
#include "rtkas_api_st.h"
#include "rtl907xc_vb_api_qos.h"
/* define macro */
#define CVB_QBV_INGRESS_PORT_NUM (CVB_RTK_USER_PORT_1ST)
#define CVB_QBV_EGRESS_PORT_NUM  (CVB_RTK_USER_PORT_2ND)

#define CVB_QBV_ADMIN_GATE_ALL_OPEN  (0xFFu)
#define CVB_QBV_ADMIN_GATE_HALF_OPEN (0x55u)
#define CVB_QBV_ADMIN_GATE_CLOSE     (0x0u)

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup ST_EXAMPLE QBV/ST Example
 * @{
 */
/**
 * @defgroup RTL907XCVB_ST_EXAMPLE RTL907XC_VB QBV/ST Example
 * @{
 */
/**
 * @defgroup RTL907XCVB_ST_ADMIN_GATE_STATE_ALL_OPEN_EXAMPLE QBV/ST Admin Gate State All Open Example
 *
 * @brief <b> Configure the gate state to be fully open. </b>
 *
 * @section Purpose
 *          This example demonstrates how to set gate state of egress port to all open.
 *
 * @section Description
 *          In this example, you need to configure VLAN and QOS first. \n
 *          All queues of egress port are always open.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_admin_gate_state_open(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_admin_gate_state_open(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
    uint32 i = 0u;
    /*
     * Preconfiguration
     *   All ports (P1~11) join the same VLAN (e.g., VLAN2)
     */

    /* Environment configuration
     *   Force to use dot1q priority (QoS)
     *   Preconfigure VLAN and priority to queue mapping (pri:0~7 to que:0~7)
     */
    QosPriSelInfo_t qosPriSel = {
        .qosPriSel = {
            .portPri   = 0,
            .hybridPri = 7,
            .dscpPri   = 0,
            .cvlanPri  = 0,
            .svlanPri  = 0,
        },
    };
    QosCvlanPriRemap_t qosCvlanPriRemap = {
        .idx = 0,
    };
    QosSvlanPriRemap_t qosSvlanPriRemap = {
        .idx = 0,
    };
    StGateStateInfo_t stGateState = {
        .portId    = CVB_QBV_EGRESS_PORT_NUM,
        .gateState = CVB_QBV_ADMIN_GATE_ALL_OPEN,
    };
    retVal = rtk_qos_pri_sel_set(unitChip, &qosPriSel);
    for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
    {
        qosCvlanPriRemap.iPri       = i;
        qosCvlanPriRemap.iDei       = 0;
        qosCvlanPriRemap.mappingPri = i;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
        qosCvlanPriRemap.iDei = 1;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
        qosSvlanPriRemap.oPri       = i;
        qosSvlanPriRemap.oDei       = 0;
        qosSvlanPriRemap.mappingPri = i;
        CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
        qosSvlanPriRemap.oDei = 1;
        CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
    }
    /* Step 1
     *   Program per-queue gate-state of egress-port to all open.
     */
    CHK_FUN_RET(retVal, rtk_st_admin_gate_state_set(unitChip, &stGateState));

    /* [Option] If user wants to check admin gate state */
    (void)drv_memset(&stGateState, 0, sizeof(StGateStateInfo_t));
    stGateState.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_admin_gate_state_get(unitChip, &stGateState));

    /* [Option] If user wants to check oper gate state */
    (void)drv_memset(&stGateState, 0, sizeof(StGateStateInfo_t));
    stGateState.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_oper_gate_state_get(unitChip, &stGateState));

    /* Expected Behaviour
     *   Inject VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially (48 packets)
     *   Monitor the number of TX packets on egress-port (P2).
     *   => The number of the egress-port packets should be 48 (100%).
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_ST_ADMIN_GATE_STATE_ALL_OPEN_EXAMPLE */
/**
 * @defgroup RTL907XCVB_ST_ADMIN_GATE_STATE_HALF_OPEN_EXAMPLE QBV/ST Admin Gate State Half Open Example
 *
 * @brief <b> Configure the gate state to be partially open. </b>
 *
 * @section Purpose
 *          This example demonstrates how to set gate state of egress port to half open.
 *
 * @section Description
 *          In this example, you need to configure VLAN and QOS first. \n
 *          Queue 0,2,4,6 of egress port are always open, queue 1,3,5,7 of egress port are always closed.
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_admin_gate_state_half_open(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_admin_gate_state_half_open(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
    uint32 i = 0u;

    /*
     * Preconfiguration
     *   All ports (P1~11) join the same VLAN (e.g., VLAN2)
     */

    /* Environment configuration
     *   Force to use dot1q priority (QoS)
     *   Preconfigure VLAN and priority to queue mapping (pri:0~7 to que:0~7)
     */
    QosPriSelInfo_t qosPriSel = {
        .qosPriSel = {
            .portPri   = 0,
            .hybridPri = 7,
            .dscpPri   = 0,
            .cvlanPri  = 0,
            .svlanPri  = 0,
        },
    };
    QosCvlanPriRemap_t qosCvlanPriRemap = {
        .idx = 0,
    };
    QosSvlanPriRemap_t qosSvlanPriRemap = {
        .idx = 0,
    };
    StGateStateInfo_t stGateState = {
        .portId    = CVB_QBV_EGRESS_PORT_NUM,
        .gateState = CVB_QBV_ADMIN_GATE_HALF_OPEN,
    };
    retVal = rtk_qos_pri_sel_set(unitChip, &qosPriSel);
    for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
    {
        qosCvlanPriRemap.iPri       = i;
        qosCvlanPriRemap.iDei       = 0;
        qosCvlanPriRemap.mappingPri = i;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
        qosCvlanPriRemap.iDei = 1;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
        qosSvlanPriRemap.oPri       = i;
        qosSvlanPriRemap.oDei       = 0;
        qosSvlanPriRemap.mappingPri = i;
        CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
        qosSvlanPriRemap.oDei = 1;
        CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
    }

    /* Step 1
     *   Program per-queue gate-state of egress-port to half-open.
     */
    CHK_FUN_RET(retVal, rtk_st_admin_gate_state_set(unitChip, &stGateState));
    /* [Option] If user wants to check admin gate state */
    (void)drv_memset(&stGateState, 0, sizeof(StGateStateInfo_t));
    stGateState.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_admin_gate_state_get(unitChip, &stGateState));

    /* [Option] If user wants to check oper gate state */
    (void)drv_memset(&stGateState, 0, sizeof(StGateStateInfo_t));
    stGateState.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_oper_gate_state_get(unitChip, &stGateState));

    /* Expected Behaviour
     *   Inject VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially (48 packets)
     *   Monitor the number of TX packets on egress-port (P2).
     *   => The number of the egress-port packets should be 24 (50%). (only pri 0,2,4,6 packets could pass).
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_ST_ADMIN_GATE_STATE_HALF_OPEN_EXAMPLE */
/**
 * @defgroup RTL907XCVB_ST_ADMIN_GATE_STATE_CLOSE_EXAMPLE QBV/ST Admin Gate State Close Example
 *
 * @brief <b> Configure the gate state to be closed. </b> \n
 *
 * @section Purpose
 *          This example demonstrates how to set gate state of egress port to all closed.
 *
 * @section Description
 *          In this example, you need to configure VLAN and QOS first. \n
 *          After all configurations are completed, all queues of egress port are always closed.
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_admin_gate_state_close(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_admin_gate_state_close(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
    uint32 i = 0;
    /*
     * Preconfiguration
     *   All ports (P1~11) join the same VLAN (e.g., VLAN2)
     */

    /* Environment configuration
     *   Force to use dot1q priority (QoS)
     *   Preconfigure VLAN and priority to queue mapping (pri:0~7 to que:0~7)
     */
    QosPriSelInfo_t qosPriSel = {
        .qosPriSel = {
            .portPri   = 0,
            .hybridPri = 7,
            .dscpPri   = 0,
            .cvlanPri  = 0,
            .svlanPri  = 0,
        },
    };
    QosCvlanPriRemap_t qosCvlanPriRemap = {
        .idx = 0,
    };
    QosSvlanPriRemap_t qosSvlanPriRemap = {
        .idx = 0,
    };
    StGateStateInfo_t stGateState = {
        .portId    = CVB_QBV_EGRESS_PORT_NUM,
        .gateState = CVB_QBV_ADMIN_GATE_CLOSE,
    };
    retVal = rtk_qos_pri_sel_set(unitChip, &qosPriSel);
    for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
    {
        qosCvlanPriRemap.iPri       = i;
        qosCvlanPriRemap.iDei       = 0;
        qosCvlanPriRemap.mappingPri = i;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
        qosCvlanPriRemap.iDei = 1;
        CHK_FUN_RET(retVal, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
        qosSvlanPriRemap.oPri       = i;
        qosSvlanPriRemap.oDei       = 0;
        qosSvlanPriRemap.mappingPri = i;
        CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
        qosSvlanPriRemap.oDei = 1;
        CHK_FUN_RET(retVal, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
    }
    /* Step 1
     *   Program per-queue gate-state of egress-port to all closed.
     */
    CHK_FUN_RET(retVal, rtk_st_admin_gate_state_set(unitChip, &stGateState));

    /* [Option] If user wants to check admin gate state */
    (void)drv_memset(&stGateState, 0, sizeof(StGateStateInfo_t));
    stGateState.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_admin_gate_state_get(unitChip, &stGateState));

    /* [Option] If user wants to check oper gate state */
    (void)drv_memset(&stGateState, 0, sizeof(StGateStateInfo_t));
    stGateState.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_oper_gate_state_get(unitChip, &stGateState));

    /* Expected Behaviour
     *   Inject VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially (48 packets)
     *   Monitor the number of TX packets on egress-port (P2).
     *   => The number of the egress-port packets should be zero (0%).
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_ST_ADMIN_GATE_STATE_CLOSE_EXAMPLE */
/**@}*/ /* RTL907XCVB_ST_EXAMPLE */
/**@}*/ /* ST_EXAMPLE */
/**@}*/ /* EXAMPLE */