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

#ifndef RTKAS_API_FLOW_CTRL_H__
#define RTKAS_API_FLOW_CTRL_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_api_qos.h"

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 RtkFlowctrlPageCnt; /**< Page count (0x0 to <CHIPID>_FLOWCTRL_PAGE_CNT_MAX) */

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
/**
 * @brief This struct is used to enable or disable.
 * The port is input, and the en can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable en;   /**< [in,out] Enable status (DISABLED to ENABLED) */
} FlowctrlEn_t;

/**
 * @brief This struct is used for page count.
 * The port and que are inputs, and the pageCnt can be either an input or an output.
 */
typedef struct
{
    RtkPort            port;    /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSQue          que;     /**< [in] Queue number (0 to QOS_QUE_MAX) */
    RtkFlowctrlPageCnt pageCnt; /**< [in,out] Page count (0x0 to <CHIPID>_FLOWCTRL_PAGE_CNT_MAX) */
} FlowctrlPageCnt_t;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_flow_ctrl_en_set(UnitChip_t unitChip, const FlowctrlEn_t *flowctrlEn);
EXTERN_API RtkApiRet rtk_flow_ctrl_en_get(UnitChip_t unitChip, FlowctrlEn_t *flowctrlEn);
EXTERN_API RtkApiRet rtk_flow_ctrl_page_cnt_set(UnitChip_t unitChip, const FlowctrlPageCnt_t *flowctrlPageCnt);
EXTERN_API RtkApiRet rtk_flow_ctrl_page_cnt_get(UnitChip_t unitChip, FlowctrlPageCnt_t *flowctrlPageCnt);

#endif /* RTKAS_API_FLOW_CTRL_H__ */
