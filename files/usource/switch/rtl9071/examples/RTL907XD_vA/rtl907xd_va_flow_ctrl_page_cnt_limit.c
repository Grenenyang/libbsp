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
#include "rtkas_api_vlan.h"
#include "rtl907xd_va_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup FLOW_CTRL_EXAMPLE Flow Control Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_FLOW_CTRL_EXAMPLE RTL907XD_VA Flow Control Example
 * @{
 */
/**
 * @defgroup RTL907XD_VA_FLOW_CTRL_PAGE_CNT_LIMIT_EXAMPLE Flow Control Page Count Limit Example
 * @brief <b>Flow Control Page Count Limit</b>
 *
 * @section Purpose
 *    The flow control page count is to prevent a single queue from occupying the buffer.
 *
 * @section Description
 *    This setting ensures that no single queue can consume all the resources available in the buffer by limiting the maximum number of pages that a single queue can use.
 *    This helps in maintaining balanced resource allocation and prevents potential buffer overflow scenarios that could degrade overall system performance.
 *    This example sets the page count for Q1/Q2 of Port 1.
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_flow_ctrl_page_cnt_limit(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_flow_ctrl_page_cnt_limit(void)
{
    RtkApiRet         retVal;
    UnitChip_t        unitChip;
    VlanInfo_t        vlanInfo;
    FlowctrlPageCnt_t flowctrlPageCnt1;
    FlowctrlPageCnt_t flowctrlPageCnt2;

    /* Initialize struct */
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;
    retVal        = drv_memset(&vlanInfo, 0, sizeof(vlanInfo));
    CHK_FUN_RET(retVal, drv_memset(&flowctrlPageCnt1, 0, sizeof(flowctrlPageCnt1)));
    CHK_FUN_RET(retVal, drv_memset(&flowctrlPageCnt2, 0, sizeof(flowctrlPageCnt2)));

    /* Step 1. Create VLANs */
    /* P1 and P3 */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_3RD;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behaviour: The frames with vid 100 will be flooded to port1, 3. */

    /* Step 2. Set egress queue page count.
     * Set Q1 queue page count of port 1 (egress) to 0x40
     * Set Q2 queue page count of port 1 (egress) to 0x80
     */
    flowctrlPageCnt1.port    = DVA_RTK_USER_PORT_1ST;
    flowctrlPageCnt1.que     = 1;
    flowctrlPageCnt1.pageCnt = 0x40;
    flowctrlPageCnt2.port    = DVA_RTK_USER_PORT_1ST;
    flowctrlPageCnt2.que     = 2;
    flowctrlPageCnt2.pageCnt = 0x80;
    CHK_FUN_RET(retVal, rtk_flow_ctrl_page_cnt_set(unitChip, &flowctrlPageCnt1));
    CHK_FUN_RET(retVal, rtk_flow_ctrl_page_cnt_set(unitChip, &flowctrlPageCnt2));
    /* Expected Behaviour: Q1 buffer size of port 1 (egress) is 64 * 128byte, and Q2 buffer size of port 1 (egress) is 128 * 128byte. */

    /* Step 3. Get egress queue page count. */
    CHK_FUN_RET(retVal, rtk_flow_ctrl_page_cnt_get(unitChip, &flowctrlPageCnt1));
    CHK_FUN_RET(retVal, rtk_flow_ctrl_page_cnt_get(unitChip, &flowctrlPageCnt2));
    /* Expected Behaviour:
     * The Q1 queue page count of port 1 (egress) is 0x40
     * The Q2 queue page count of port 1 (egress) is 0x80
     */

    return retVal;
}
/**@}*/ /* RTL907XD_VA_FLOW_CTRL_PAGE_CNT_LIMIT_EXAMPLE */
/**@}*/ /* RTL907XD_VA_FLOW_CTRL_EXAMPLE */
/**@}*/ /* FLOW_CTRL_EXAMPLE */
/**@}*/ /* EXAMPLE */