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
 * @defgroup RTL907XD_VA_QOS_TRAFFIC_QUE_SCH_EXAMPLE QoS Traffic Queuing and Scheduler Example
 * @brief <b>QoS Traffic Queuing and Scheduler</b>
 *
 * @section Purpose
 *   Optimize network resource allocation and traffic management by prioritizing different types of traffic.
 *
 * @section Description
 *   QoS Traffic Queuing and Scheduler mechanisms ensure efficient network performance by implementing traffic prioritization and fair bandwidth distribution, thereby enhancing overall network efficiency and reliability.
 *   For packets coming from port 1, the switch enqueues those packets to the TX queue 2 of outgoing ports.
 *   For packets coming from port 2, the switch enqueues those packets to the TX queue 3 of outgoing ports.
 *   Set the traffic scheduler of TX queue 2 of port 3 to WRR (Weighted Round Robin) and set the WRR weight to 5.
 *   Set the traffic scheduler of TX queue 3 of port 3 to WRR and set the WRR weight to 10.
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_qos_traffic_queuing_and_scheduler(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_qos_traffic_queuing_and_scheduler(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    QosPriSelInfo_t    qosPriSelInfo;
    QosPriWtTbl_t      qosPriWtTbl;
    QosPortPri_t       qosPortPri;
    QosPriQueMapping_t qosPriQueMapping;
    QosQueSchInfo_t    wQueSchInfo;
    QosQueSchInfo_t    rQosQueSchInfo[2];
    QosQueWfqWrrWt_t   qosQueWfqWrrWt;

    /* Initialize struct */
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;
    retVal        = drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&qosPortPri, 0, sizeof(qosPortPri)));
    CHK_FUN_RET(retVal, drv_memset(&qosPriQueMapping, 0, sizeof(qosPriQueMapping)));
    CHK_FUN_RET(retVal, drv_memset(&wQueSchInfo, 0, sizeof(wQueSchInfo)));
    CHK_FUN_RET(retVal, drv_memset(rQosQueSchInfo, 0, sizeof(rQosQueSchInfo)));
    CHK_FUN_RET(retVal, drv_memset(&qosQueWfqWrrWt, 0, sizeof(qosQueWfqWrrWt)));

    /* Step 1. Set priority extraction weight table 0 */
    qosPriSelInfo.group             = 0u;
    qosPriSelInfo.qosPriSel.portPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    /* Expected Behaviour:  In priority extraction weight table 0, only accept port-based priority as source priority */

    /* Step 2.1. Select priority extraction weight table 0 on port 1 */
    qosPriWtTbl.port = DVA_RTK_USER_PORT_1ST;
    qosPriWtTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));

    /* Step 2.2. Select priority extraction weight table 0 on port 2 */
    qosPriWtTbl.port = DVA_RTK_USER_PORT_2ND;
    qosPriWtTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));

    /* Step 2.3. Set QoS port-based priority to 2 on port 1 */
    qosPortPri.port = DVA_RTK_USER_PORT_1ST;
    qosPortPri.pri  = 2u;
    CHK_FUN_RET(retVal, rtk_qos_port_pri_set(unitChip, &qosPortPri));

    /* Step 2.4. Set QoS port-based priority to 3 on port 2 */
    qosPortPri.port = DVA_RTK_USER_PORT_2ND;
    qosPortPri.pri  = 3u;
    CHK_FUN_RET(retVal, rtk_qos_port_pri_set(unitChip, &qosPortPri));
    /* Expected Behaviour:
     *   The internal priority of the packet from port 1 is 2.
     *   The internal priority of the packet from port 2 is 3.
     */

    /* Step 3.
     * Set internal priority 2 mapping to TX queue 2 for outgoing port 3
     * Set internal priority 3 mapping to TX queue 3 for outgoing port 3
     */
    qosPriQueMapping.port = DVA_RTK_USER_PORT_1ST;
    qosPriQueMapping.pri  = 2u;
    qosPriQueMapping.que  = 2u;
    CHK_FUN_RET(retVal, rtk_qos_pri_que_mapping_set(unitChip, &qosPriQueMapping));
    qosPriQueMapping.port = DVA_RTK_USER_PORT_2ND;
    qosPriQueMapping.pri  = 3u;
    qosPriQueMapping.que  = 3u;
    CHK_FUN_RET(retVal, rtk_qos_pri_que_mapping_set(unitChip, &qosPriQueMapping));
    /* Expected Behaviour:
     * Internal priority 2 maps to TX queue 2 for outgoing port 3
     * Internal priority 3 maps to TX queue 3 for outgoing port 3
     */

    /* Step 4.1.
     * Set traffic scheduler of TX queue 2 to WRR for outgoing port 3
     * Set traffic scheduler of TX queue 3 to WRR for outgoing port 3
     */
    wQueSchInfo.port   = DVA_RTK_USER_PORT_3RD;
    wQueSchInfo.que    = 2u;
    wQueSchInfo.queSch = DVA_QOS_SCH_WRR;
    CHK_FUN_RET(retVal, rtk_qos_que_sch_set(unitChip, &wQueSchInfo));
    wQueSchInfo.que = 3u;
    CHK_FUN_RET(retVal, rtk_qos_que_sch_set(unitChip, &wQueSchInfo));

    /* Step 4.2
     * Set WRR weight of TX queue 2 to 5 for outgoing port 3
     * Set WRR weight of TX queue 3 to 10 for outgoing port 3
     */
    qosQueWfqWrrWt.port  = DVA_RTK_USER_PORT_3RD;
    qosQueWfqWrrWt.que   = 2u;
    qosQueWfqWrrWt.queWt = 5u;
    CHK_FUN_RET(retVal, rtk_qos_que_wfq_wrr_wt_set(unitChip, &qosQueWfqWrrWt));
    qosQueWfqWrrWt.que   = 3u;
    qosQueWfqWrrWt.queWt = 10u;
    CHK_FUN_RET(retVal, rtk_qos_que_wfq_wrr_wt_set(unitChip, &qosQueWfqWrrWt));
    /* Expected Behaviour:
     *   WRR weight of TX queue 2 is 5 for outgoing port 3
     *   WRR weight of TX queue 3 is 10 for outgoing port 3
     */

    /* Step 4.3.
     * Get traffic scheduler of TX queue 2 to WRR for outgoing port 3
     * Get traffic scheduler of TX queue 3 to WRR for outgoing port 3
     */
    rQosQueSchInfo[0].port = DVA_RTK_USER_PORT_3RD;
    rQosQueSchInfo[0].que  = 2u;
    CHK_FUN_RET(retVal, rtk_qos_que_sch_get(unitChip, &(rQosQueSchInfo[0])));
    rQosQueSchInfo[1].port = DVA_RTK_USER_PORT_3RD;
    rQosQueSchInfo[1].que  = 3u;
    CHK_FUN_RET(retVal, rtk_qos_que_sch_get(unitChip, &(rQosQueSchInfo[1])));

    return retVal;
}
/**@}*/ /* RTL907XD_VA_QOS_TRAFFIC_QUE_SCH_EXAMPLE */
/**@}*/ /* RTL907XD_VA_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */