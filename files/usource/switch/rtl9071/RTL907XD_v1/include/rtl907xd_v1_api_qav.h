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

#ifndef DV1_RTK_API_QAV_H__
#define DV1_RTK_API_QAV_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_v1_api.h"
#include "rtl907xd_v1_api_qos.h"
#include "rtkas_api_qos.h"
#include "rtkas_api_qav.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DV1_QAV_BW_MAX         (0x7FFFFFFFFFu)
#define DV1_QAV_PRECISION_BITS (20u)
#define DV1_QAV_CLK_FREQ       (250000000u)
/* Offset */
#define DV1_QAV_ADMIN_IDLE_SLOPE_REG_OFFSET ((uint32)0x8u)
#define DV1_QAV_PRI_REGEN_REG_OFFSET        ((uint32)0x4u)
#define DV1_QAV_PRI_REGEN_LSP_OFFSET        ((uint32)0x3u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint64 dv1_QavBw; /**< Bandwidth of port or queue (0 to DV1_QAV_BW_MAX) */

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet dv1_qav_slope_set(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_QavBw bw);
EXTERN_API RtkApiRet dv1_qav_slope_get(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_QavBw *bw);
EXTERN_API RtkApiRet dv1_qav_cbs_set(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_RtkEnable en, dv1_QavBw bw);
EXTERN_API RtkApiRet dv1_qav_cbs_get(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_RtkEnable *en, dv1_QavBw *bw);
EXTERN_API RtkApiRet dv1_qav_pri_regen_set(uint32 unit, dv1_RtkPort port, dv1_QosPri oriPri, dv1_RtkEnable en, dv1_QosPri regenPri);
EXTERN_API RtkApiRet dv1_qav_pri_regen_get(uint32 unit, dv1_RtkPort port, dv1_QosPri oriPri, dv1_RtkEnable *en, dv1_QosPri *regenPri);
EXTERN_API RtkApiRet dv1_qav_regen_tag_sel_set(uint32 unit, dv1_RtkPort port, dv1_RtkEnable enIPri, dv1_RtkEnable enOPri);
EXTERN_API RtkApiRet dv1_qav_regen_tag_sel_get(uint32 unit, dv1_RtkPort port, dv1_RtkEnable *pEnIPri, dv1_RtkEnable *pEnOPri);

#endif /* DV1_RTK_API_QAV_H__ */