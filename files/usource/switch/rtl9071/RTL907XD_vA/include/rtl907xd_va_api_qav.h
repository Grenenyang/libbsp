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

#ifndef DVA_RTK_API_QAV_H__
#define DVA_RTK_API_QAV_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api_qos.h"
#include "rtkas_api_qos.h"
#include "rtkas_api_qav.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DVA_QAV_BW_MAX         (0x7FFFFFFFFFu)
#define DVA_QAV_PRECISION_BITS (20u)
#define DVA_QAV_CLK_FREQ       (250000000u)
/* Offset */
#define DVA_QAV_ADMIN_IDLE_SLOPE_REG_OFFSET ((uint32)0x8u)
#define DVA_QAV_PRI_REGEN_REG_OFFSET        ((uint32)0x4u)
#define DVA_QAV_PRI_REGEN_LSP_OFFSET        ((uint32)0x3u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint64 dva_QavBw; /**< Bandwidth of port or queue (0 to DVA_QAV_BW_MAX) */

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet dva_qav_slope_set(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_QavBw bw);
EXTERN_API RtkApiRet dva_qav_slope_get(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_QavBw *bw);
EXTERN_API RtkApiRet dva_qav_cbs_set(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_RtkEnable en, dva_QavBw bw);
EXTERN_API RtkApiRet dva_qav_cbs_get(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_RtkEnable *en, dva_QavBw *bw);
EXTERN_API RtkApiRet dva_qav_pri_regen_set(uint32 unit, dva_RtkPort port, dva_QosPri oriPri, dva_RtkEnable en, dva_QosPri regenPri);
EXTERN_API RtkApiRet dva_qav_pri_regen_get(uint32 unit, dva_RtkPort port, dva_QosPri oriPri, dva_RtkEnable *en, dva_QosPri *regenPri);
EXTERN_API RtkApiRet dva_qav_regen_tag_sel_set(uint32 unit, dva_RtkPort port, dva_RtkEnable enIPri, dva_RtkEnable enOPri);
EXTERN_API RtkApiRet dva_qav_regen_tag_sel_get(uint32 unit, dva_RtkPort port, dva_RtkEnable *pEnIPri, dva_RtkEnable *pEnOPri);

#endif /* DVA_RTK_API_QAV_H__ */