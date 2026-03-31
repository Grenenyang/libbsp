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

#ifndef CVB_RTK_API_QAV_H__
#define CVB_RTK_API_QAV_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_api_qos.h"
#include "rtkas_api_qos.h"
#include "rtkas_api_qav.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CVB_QAV_BW_MAX         (0xFFFFFFFFu)
#define CVB_QAV_PRECISION_BITS (20u)
#define CVB_QAV_CLK_FREQ       (125000000u)
/* Offset */
#define CVB_QAV_PRI_REGEN_REG_OFFSET ((uint32)0x4u)
#define CVB_QAV_PRI_REGEN_LSP_OFFSET ((uint32)0x3u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint64 cvb_QavBw; /**< Bandwidth of port or queue (0 to CVB_QAV_BW_MAX) */

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet cvb_qav_slope_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_QavBw bw);
EXTERN_API RtkApiRet cvb_qav_slope_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_QavBw *bw);
EXTERN_API RtkApiRet cvb_qav_cbs_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_RtkEnable en, cvb_QavBw bw);
EXTERN_API RtkApiRet cvb_qav_cbs_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_RtkEnable *en, cvb_QavBw *bw);
EXTERN_API RtkApiRet cvb_qav_pri_regen_set(uint32 unit, cvb_RtkPort port, cvb_QosPri oriPri, cvb_RtkEnable en, cvb_QosPri regenPri);
EXTERN_API RtkApiRet cvb_qav_pri_regen_get(uint32 unit, cvb_RtkPort port, cvb_QosPri oriPri, cvb_RtkEnable *en, cvb_QosPri *regenPri);
EXTERN_API RtkApiRet cvb_qav_regen_tag_sel_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable enIPri, cvb_RtkEnable enOPri);
EXTERN_API RtkApiRet cvb_qav_regen_tag_sel_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEnIPri, cvb_RtkEnable *pEnOPri);

#endif /* CVB_RTK_API_QAV_H__ */
