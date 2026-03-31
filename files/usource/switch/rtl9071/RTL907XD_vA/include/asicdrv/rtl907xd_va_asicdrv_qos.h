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

#ifndef DVA_ASICDRV_QOS_H_
#define DVA_ASICDRV_QOS_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DVA_QOS_DOT1QSCKALGTYPE_CBS (0u)
#define DVA_QOS_DOT1QSCKALGTYPE_ATS (1u)
#define DVA_QOS_DOT1QSCKALGTYPE_END (DVA_QOS_DOT1QSCKALGTYPE_ATS)

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_drv_qos_igr_bw_ctrl_set_ret_chk(uint32 unit, dva_RtkPort port, const QosIgrBwCtrl_t *pIgrBwCtrl);
RtkApiRet dva_drv_qos_igr_bw_ctrl_get_ret_chk(uint32 unit, dva_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl);
RtkApiRet dva_drv_qos_igr_bw_ctrl_get_0(uint32 unit, dva_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl);
RtkApiRet dva_drv_qos_igr_bw_ctrl_get_1(uint32 unit, dva_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl);
RtkApiRet dva_drv_qos_que_sch_set_ret_chk(uint32 unit, dva_RtkPort port, dva_QosQueNum que, RtkQoSSch queSch);
RtkApiRet dva_drv_qos_que_sch_get_ret_chk(uint32 unit, dva_RtkPort port, dva_QosQueNum que, RtkQoSSch *pQueSch);
RtkApiRet dva_drv_qos_que_sch_set_info_set(uint32 unit, dva_RtkPort port, dva_QosQueNum que, RtkQoSSch queSch);
RtkApiRet dva_drv_qos_que_sch_get_info_get(uint32 unit, dva_RtkPort port, dva_QosQueNum que, RtkQoSSch *pQueSch);
RtkApiRet dva_drv_qos_egr_que_bw_ctrl_ret_chk(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_RtkEnable *pEn, dva_QosBw *pQueBw);

#endif /* DVA_ASICDRV_QOS_H_ */
