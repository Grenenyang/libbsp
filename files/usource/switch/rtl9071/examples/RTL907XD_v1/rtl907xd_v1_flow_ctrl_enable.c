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
 * @defgroup RTL907XD_V1_FLOW_CTRL_ENABLE_EXAMPLE Flow Control Enable Example
 * @brief <b>Flow Control Enable</b>
 *
 * @section Purpose
 *   Enabling flow control prevents buffer overflow by managing data transmission pauses, thus improving network reliability.
 *
 * @section Description
 *   Enabling flow control activates RX and TX pause signals, helping manage data flow but potentially introducing minor latency.
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_flow_ctrl_enable(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_flow_ctrl_enable(void)
{
    RtkApiRet    retVal;
    UnitChip_t   unitChip;
    RtkPort      port;
    uint32       i = 0u;
    FlowctrlEn_t flowctrlEn[3];

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(flowctrlEn, 0, sizeof(flowctrlEn));

    /* Step 1. Enable port1, port2, port3 Flow Control Function */
    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_3RD; port++)
    {
        flowctrlEn[i].port = port;
        flowctrlEn[i].en   = ENABLED;
        CHK_FUN_RET(retVal, rtk_flow_ctrl_en_set(unitChip, &flowctrlEn[i]));
        i++;
    }
    /* Expected Behaviour: The flow control function on port 1, port 2, and port 3 can TX and RX pause frames. */

    return retVal;
}
/**@}*/ /* RTL907XD_V1_FLOW_CTRL_ENABLE_EXAMPLE */
/**@}*/ /* RTL907XD_V1_FLOW_CTRL_EXAMPLE */
/**@}*/ /* FLOW_CTRL_EXAMPLE */
/**@}*/ /* EXAMPLE */