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
 * @defgroup RTL907XC_VA_QOS_PORT_BW_LIMIT_EXAMPLE QoS Port Bandwidth Limit Example
 * @brief <b>QoS Port Bandwidth Limit</b>
 *
 * @section Purpose
 *   Setting QoS port bandwidth limit aims to control network traffic, ensuring the stability and efficiency of critical applications, and preventing any single port from consuming excessive bandwidth.
 *
 * @section Description
 *   QoS port bandwidth limit allows network administrators to set a maximum traffic bandwidth for each port. This ensures fair bandwidth distribution, prevents overuse by certain devices, and maintains network stability. \n
 *   It helps prioritize important traffic and improves overall network performance. \n
 *   This example sets the ingress data bandwidth of port 3 to 125 * 8 Kbps and the egress data bandwidth of port 1 to 1000 * 8 Kbps.
 *
 * @image html qos/port_bw_limit.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_qos_port_bw_limit(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_qos_port_bw_limit(void)
{
    RtkApiRet          retVal;
    UnitChip_t         unitChip;
    QosIgrBwCtrlInfo_t wQosIgrBwCtrl;
    QosIgrBwCtrlInfo_t rQosIgrBwCtrl;
    QosEgrBwCtrl_t     wQosEgrBwCtrl;
    QosEgrBwCtrl_t     rQosEgrBwCtrl;

    /* Initialize struct */
    unitChip.unit = CVA_UNIT;
    unitChip.chip = CHIP_907XC_VA;
    retVal        = drv_memset(&wQosIgrBwCtrl, 0, sizeof(wQosIgrBwCtrl));
    CHK_FUN_RET(retVal, drv_memset(&rQosIgrBwCtrl, 0, sizeof(rQosIgrBwCtrl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosEgrBwCtrl, 0, sizeof(wQosEgrBwCtrl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosEgrBwCtrl, 0, sizeof(rQosEgrBwCtrl)));

    /* Step 1.
     * Set port 3 ingress bandwidth control & ingress flow control
     * Set the upper limit of LB0 bandwidth to 125 * 8 Kbps
     * Set the upper limit of LB1 bandwidth to 0bps
     * Enable LB0 ingress bandwidth control
     * Disable LB1 ingress bandwdith control
     */
    wQosIgrBwCtrl.port                     = CVA_RTK_USER_PORT_3RD;
    wQosIgrBwCtrl.qosIgrBwCtrl.bwCtrlEn0   = ENABLED;
    wQosIgrBwCtrl.qosIgrBwCtrl.igrBw0      = 125u;
    wQosIgrBwCtrl.qosIgrBwCtrl.flowCtrlEn0 = DISABLED;
    wQosIgrBwCtrl.qosIgrBwCtrl.bwCtrlEn1   = DISABLED;
    wQosIgrBwCtrl.qosIgrBwCtrl.igrBw1      = 0u;
    wQosIgrBwCtrl.qosIgrBwCtrl.flowCtrlEn1 = DISABLED;
    rQosIgrBwCtrl.port                     = CVA_RTK_USER_PORT_3RD;
    CHK_FUN_RET(retVal, rtk_qos_igr_bw_ctrl_set(unitChip, &wQosIgrBwCtrl));
    CHK_FUN_RET(retVal, rtk_qos_igr_bw_ctrl_get(unitChip, &rQosIgrBwCtrl));
    /* Expected Behaviour: The upper limit of ingress bandwidth of port 3 is 125 * 8 Kbps, LB0 ingress bandwidth control of port 3 is enabled, and LB1 ingress bandwidth control of port 3 is disabled */

    return retVal;
}
/**@}*/ /* RTL907XC_VA_QOS_PORT_BW_LIMIT_EXAMPLE */
/**@}*/ /* RTL907XC_VA_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */
