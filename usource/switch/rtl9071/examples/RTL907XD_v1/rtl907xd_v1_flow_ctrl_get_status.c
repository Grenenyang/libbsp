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
#include "rtkas_api_flow_ctrl.h"
#include "rtl907xd_v1_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup FLOW_CTRL_EXAMPLE Flow Control Example
 * @{
 */
/**
 * @addtogroup RTL907XD_V1_FLOW_CTRL_EXAMPLE RTL907XD_V1 Flow Control Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_FLOW_CTRL_GET_STATUS_EXAMPLE Flow Control Get Status Example
 * @brief <b>Flow Control Get Status</b>
 *
 * @section Purpose
 *   Querying the flow control status allows you to understand the flow management settings of networking devices, enabling appropriate adjustments.
 *
 * @section Description
 *   Retrieving the flow control status displays the current configuration of RX and TX pause signals, aiding in network performance optimization and troubleshooting.
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_flow_ctrl_get_status(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_flow_ctrl_get_status(void)
{
    RtkApiRet    retVal;
    UnitChip_t   unitChip;
    FlowctrlEn_t rFlowctrlEn[3];

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(rFlowctrlEn, 0, sizeof(rFlowctrlEn));

    /* Step 1. Get port1, port2, port3 Flow Control Function Status */
    rFlowctrlEn[0].port = DV1_RTK_USER_PORT_1ST;
    rFlowctrlEn[1].port = DV1_RTK_USER_PORT_2ND;
    rFlowctrlEn[2].port = DV1_RTK_USER_PORT_3RD;
    CHK_FUN_RET(retVal, rtk_flow_ctrl_en_get(unitChip, &rFlowctrlEn[0]));
    CHK_FUN_RET(retVal, rtk_flow_ctrl_en_get(unitChip, &rFlowctrlEn[1]));
    CHK_FUN_RET(retVal, rtk_flow_ctrl_en_get(unitChip, &rFlowctrlEn[2]));

    /* Expected Behaviour:
     *   if the rFlowctrlEn[X-1] = 1 -> Flow Control Enable on port X
     *   if the rFlowctrlEn[X-1] = 0 -> Flow Control Disable on port X
     */

    return retVal;
}
/**@}*/ /* RTL907XD_V1_FLOW_CTRL_GET_STATUS_EXAMPLE */
/**@}*/ /* RTL907XD_V1_FLOW_CTRL_EXAMPLE */
/**@}*/ /* FLOW_CTRL_EXAMPLE */
/**@}*/ /* EXAMPLE */
