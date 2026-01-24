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
 * @addtogroup RTL907XD_V1_QOS_EXAMPLE RTL907XC_V1 QoS Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_QOS_PORT_BASED_PRI_EXAMPLE QoS Port-based Priority Example
 * @brief <b>QoS Port-based Priority</b>
 *
 * @section Purpose
 *   Prioritize reliable QoS based on port priority to ensure high performance and network stability.
 *
 * @section Description
 *   Adjust QoS settings to manage port-based priorities for optimal delivery and consistent network performance.\n
 *   This example will set the QoS port-based priority of port 2 to 3. The switch will queue packets coming from port 2 with an internal priority of 3.
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_qos_port_based_priority(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_qos_port_based_priority(void)
{
    RtkApiRet       retVal;
    UnitChip_t      unitChip;
    QosPriSelInfo_t qosPriSelInfo;
    QosPriWtTbl_t   qosPriWtTbl;
    QosPortPri_t    wQosPortPri;
    QosPortPri_t    rQosPortPri;

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(&qosPriSelInfo, 0, sizeof(qosPriSelInfo));
    CHK_FUN_RET(retVal, drv_memset(&qosPriWtTbl, 0, sizeof(qosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosPortPri, 0, sizeof(wQosPortPri)));
    CHK_FUN_RET(retVal, drv_memset(&rQosPortPri, 0, sizeof(rQosPortPri)));

    /* Step 1. Set priority extraction weight table 0 */
    qosPriSelInfo.group             = 0u;
    qosPriSelInfo.qosPriSel.portPri = 7u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &qosPriSelInfo));
    /* Expected Behaviour:  In priority extraction weight table 0, only accept port-based priority as source priority */

    /* Step 2.1. Select priority extraction weight table 0 on port 2 */
    qosPriWtTbl.port = DV1_RTK_USER_PORT_2ND;
    qosPriWtTbl.idx  = 0u;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &qosPriWtTbl));

    /* Step 2.2. Set QoS port-based priority to 3 on port 2 */
    wQosPortPri.port = DV1_RTK_USER_PORT_2ND;
    wQosPortPri.pri  = 3u;
    rQosPortPri.port = DV1_RTK_USER_PORT_2ND;
    CHK_FUN_RET(retVal, rtk_qos_port_pri_set(unitChip, &wQosPortPri));
    CHK_FUN_RET(retVal, rtk_qos_port_pri_get(unitChip, &rQosPortPri));
    /* Expected Behaviour:  The internal priortiy is 3 */

    return retVal;
}
/**@}*/ /* RTL907XC_V1_QOS_PORT_BASED_PRI_EXAMPLE */
/**@}*/ /* RTL907XC_V1_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */