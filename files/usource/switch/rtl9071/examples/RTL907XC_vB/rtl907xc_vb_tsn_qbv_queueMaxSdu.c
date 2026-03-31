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

#define CVB_QBV_MAX_SDU_PASS_128BYTE (106u)
#define CVB_QBV_MAX_SDU_DROP_128BYTE (105u)

/**
 * @addtogroup ST_EXAMPLE QBV/ST Example
 * @{
 */
/**
 * @addtogroup RTL907XCVB_ST_EXAMPLE RTL907XC_VB QBV/ST Example
 * @{
 */
/**
 * @defgroup RTL907XCVB_ST_QUEUE_MAX_SDU_PASS_EXAMPLE QBV/ST Queue Max SDU Pass Example
 *
 * @brief <b> Configure maximum SDU and check the configuration </b>
 *
 * @section Purpose
 *          This example code demonstrates how to set all queue max-SDU of egress port to 106.
 *
 * @section Description
 *          In this example, you need to configure VLAN and QOS first. \n
 *          After all configurations are completed,
 *          all VLAN packets whose SDU is larger than 106 will be dropped, \n
 *          all VLAN packets whose SDU is equal to or less than 106 will be accepted and transmitted.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_queue_max_sdu_pass(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_queue_max_sdu_pass(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint32     i        = 0;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
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
    StQueueMaxSduInfo_t stQueueMaxSdu = {
        .portId = CVB_QBV_EGRESS_PORT_NUM,
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
     *   Set all queue max-SDU to 106.
     */
    for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
    {
        /* 0~7 */
        stQueueMaxSdu.queueMaxSdu[i] = CVB_QBV_MAX_SDU_PASS_128BYTE;
    }
    CHK_FUN_RET(retVal, rtk_st_queue_max_sdu_set(unitChip, &stQueueMaxSdu));

    /* [option] if user wants to check configuration */
    (void)drv_memset(&stQueueMaxSdu, 0, sizeof(StQueueMaxSduInfo_t));
    stQueueMaxSdu.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_queue_max_sdu_get(unitChip, &stQueueMaxSdu));

    /* Expected Behavior
     *   Inject 128-byte VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially (48 packets)
     *   Monitor the number of TX 128-byte VLAN packets on egress-port (P2) (max SDU is 106).
     *   => Finally, we could observe 48 128-byte VLAN packets were transmitted. (egress from all queues)
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_ST_QUEUE_MAX_SDU_PASS_EXAMPLE */
/**
 * @defgroup RTL907XCVB_ST_QUEUE_MAX_SDU_HALF_PASS_EXAMPLE QBV/ST Queue Max SDU Half Pass Example
 *
 * @brief <b> Configure maximum SDU and check the configuration </b>
 *
 * @section Purpose
 *          This example code demonstrates how to set queue 0,2,4,6 max-SDU of egress port to 106, \n
 *          set queue 1,3,5,7 max-SDU of egress port to 105.
 *
 * @section Description
 *          In this example, you need to configure VLAN and QOS first. \n
 *          After all configurations are completed,
 *          for queues 0, 2, 4, and 6, all VLAN packets with an SDU larger than 106 will be dropped, \n
 *          while those with an SDU of 106 or less will be accepted and transmitted. \n
 *          For queues 1, 3, 5, and 7, all VLAN packets with an SDU larger than 105 will be dropped, \n
 *          while those with an SDU of 105 or less will be accepted and transmitted.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_queue_max_sdu_half_pass(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_queue_max_sdu_half_pass(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint32     i        = 0;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};

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
    StQueueMaxSduInfo_t stQueueMaxSdu = {
        .portId = CVB_QBV_EGRESS_PORT_NUM,
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
     *   Set queue 0,2,4,6 max-SDU to 106.
     *   Set queue 1,3,5,7 max-SDU to 105.
     */
    for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
    {
        if(0u == (i % 2u))
        {
            /* 0,2,4,6 */
            stQueueMaxSdu.queueMaxSdu[i] = CVB_QBV_MAX_SDU_PASS_128BYTE;
        }
        else
        {
            /* 1,3,5,7 */
            stQueueMaxSdu.queueMaxSdu[i] = CVB_QBV_MAX_SDU_DROP_128BYTE;
        }
    }
    CHK_FUN_RET(retVal, rtk_st_queue_max_sdu_set(unitChip, &stQueueMaxSdu));

    /* [option] if user wants to check configuration */
    (void)drv_memset(&stQueueMaxSdu, 0, sizeof(StQueueMaxSduInfo_t));
    stQueueMaxSdu.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_queue_max_sdu_get(unitChip, &stQueueMaxSdu));

    /* Expected Behavior
     *   Inject 128-byte VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially (48 packets)
     *   Monitor the number of TX 128-byte VLAN packets on egress-port (P2) (max SDU is 106).
     *   => Finally, we could observe 24 128-byte VLAN packets were transmitted. (egress from queue 0,2,4,6)
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_ST_QUEUE_MAX_SDU_HALF_PASS_EXAMPLE */
/**
 * @defgroup RTL907XCVB_ST_QUEUE_MAX_SDU_DROP_EXAMPLE QBV/ST Queue Max SDU Drop Example
 *
 * @brief <b> Configure maximum SDU and check the configuration </b>
 *
 * @section Purpose
 *          This example code demonstrates how to set all queue max-SDU of egress port to 105.
 *
 * @section Description
 *          In this example, you need to configure VLAN and QOS first. \n
 *          After all configurations are completed,
 *          all VLAN packets whose SDU is larger than 105 will be dropped, \n
 *          all VLAN packets whose SDU is equal to or less than 105 will be accepted and transmitted.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_queue_max_sdu_drop(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_tsn_qbv_queue_max_sdu_drop(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint32     i        = 0;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};

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
    StQueueMaxSduInfo_t stQueueMaxSdu = {
        .portId = CVB_QBV_EGRESS_PORT_NUM,
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
     *   Set all queue max-SDU to 105.
     */
    for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
    {
        /* 0~7 */
        stQueueMaxSdu.queueMaxSdu[i] = CVB_QBV_MAX_SDU_DROP_128BYTE;
    }

    CHK_FUN_RET(retVal, rtk_st_queue_max_sdu_set(unitChip, &stQueueMaxSdu));

    /* [option] if user wants to check configuration */
    (void)drv_memset(&stQueueMaxSdu, 0, sizeof(StQueueMaxSduInfo_t));
    stQueueMaxSdu.portId = CVB_QBV_EGRESS_PORT_NUM;
    CHK_FUN_RET(retVal, rtk_st_queue_max_sdu_get(unitChip, &stQueueMaxSdu));

    /* Expected Behavior
     *   Inject 128-byte VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially (48 packets)
     *   Monitor the number of TX 128-byte VLAN packets on egress-port (P2) (max SDU is 106).
     *   => Finally, we could observe 0 128-byte VLAN packets were transmitted. (none-queue egress)
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_ST_QUEUE_MAX_SDU_HALF_DROP_EXAMPLE */
/**@}*/ /* RTL907XCVB_ST_EXAMPLE */
/**@}*/ /* ST_EXAMPLE */