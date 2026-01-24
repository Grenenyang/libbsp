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

#ifndef CVB_RTK_API_FLOW_CTL_H__
#define CVB_RTK_API_FLOW_CTL_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_api_qos.h"
#include "rtkas_api_qos.h"
#include "rtkas_api_flow_ctrl.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CVB_FLOWCTRL_REG_OFFSET           (0x4u)
#define CVB_FLOWCTRL_STATUS_REG_PORTA_MAX (8u)
#define CVB_FLOWCTRL_STATUS_REG_PORTB_MIN (9u)
#define CVB_FLOWCTRL_STATUS_REG_PORTB_MAX (12u)
#define CVB_FLOWCTRL_STATUS_REG_PORTC_MIN (13u)
#define CVB_FLOWCTRL_STATUS_REG_PORTC_MAX (15u)
#define CVB_FLOWCTRL_PAGE_CNT_MAX         (((uint32)0x1u << CVB_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LEN) - 1u)
#define CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET1 (16u)
#define CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET2 (2u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 cvb_FlowctrlPageCnt; /**< Page count (0x0 to CVB_FLOWCTRL_PAGE_CNT_MAX) */

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet cvb_flow_ctrl_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en);
EXTERN_API RtkApiRet cvb_flow_ctrl_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn);
EXTERN_API RtkApiRet cvb_flow_ctrl_page_cnt_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_FlowctrlPageCnt pageCnt);
EXTERN_API RtkApiRet cvb_flow_ctrl_page_cnt_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_FlowctrlPageCnt *pPageCnt);

#endif /* CVB_RTK_API_FLOW_CTRL_H__ */
