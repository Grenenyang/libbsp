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
#include "rtkas_api_ext.h"
#include "rtkas_api_acl_ext.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup ACL_EXAMPLE ACL Example
 * @{
 */
/**
 * @addtogroup RTL907DV1_ACL_EXAMPLE RTL907XD_V1 ACL Example
 * @{
 */
/**
 * @addtogroup RTL907DV1_ACL_TIMER_DISCARD ACL Timer Discard Example
 * @brief This example illustrates how to configure and utilize the ACL timer discard functionality to manage network traffic by discarding packets based on a timer.
 *
 * @section Purpose
 *   This example demonstrates the setup and use of the ACL timer discard feature.
 *
 * @section Description
 *   This example provides a detailed setup for configuring a timer discard action in ACL, which helps in managing network traffic by discarding packets based on a timer: \n
 *   a. Enabling the timer discard action. \n
 *   b. Setting and retrieving the timer discard entry.
 *   @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dv1_acl_timer_discard(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dv1_acl_timer_discard(uint32 unit)
{
    /* +-------------------------------------------------------------+ */
    /* |                   Template 1, PIE entry 32                  | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0    | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | FMT  | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0C00 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 3C00 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_V1,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = DV1_RTK_USER_PORT_1ST,
        .aclEn   = DV1_ACL_ENABLE,
        .defAct  = DV1_ACL_DEF_ACT_DROP,
    };
    RtkPort                         port                     = DV1_RTK_USER_PORT_1ST;
    AclRuleId                       index                    = 32u;
    AclTemplateId                   tpIdx                    = index / DV1_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t               templateInfo             = {0};
    AclPieEntryInfo_t               pieEntryInfo             = {0};
    AclTblAccessPieParam_t          pieParam                 = {0};
    AclActionEntryInfo_t            actionEntryInfo          = {0};
    AclTblAccessActParam_t          actParam                 = {0};
    AclTimerDiscardEntryInfo_t      timerDiscardEntryInfo    = {0};
    AclTblAccessTimerDiscardParam_t timerDiscardParam        = {0};
    AclTimerDiscardEntryInfo_t      timerDiscardEntryInfoGet = {0};
    AclTblAccessTimerDiscardParam_t timerDiscardParamGet     = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 1 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = DV1_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 32 */

    /* SPHY[13:10]  = DV1_RTK_USER_PORT_3RD (ingress port number = DV1_RTK_USER_PORT_3RD) */
    pieParam.data0 = (uint16)dv1_g_aclUserPort2sysPort[DV1_RTK_USER_PORT_3RD] << 10u;
    pieParam.mask0 = 0x3C00u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 32 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 32 */

    /*
        redir = 1,
        opcode = 0 (normal forward),
        dpm = DV1_RTK_USER_PMAP_4TH,
        crsvlan = 0 (forward in VLAN)
    */
    actParam.redir             = 1u;
    actParam.redirInfo.opcode  = 0u;
    actParam.redirInfo.dpm     = DV1_RTK_USER_PMAP_4TH;
    actParam.redirInfo.crsvlan = 0u;

    /* Configure timerDiscard to 1 for entry 32 to start the timer discard function */
    actParam.timerDiscard = 1u;

    /* Configure action entry 32 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure timer discard entry 32 */

    /*
        withdraw    = 0 (timer discard function is active),
        threshold   = 3 (this means only 3 packets are allowed to be received; otherwise, it will be dropped),
        monitorTime = 0x47868C0 (0x47868C0 * 4ns = 0.3s),
        guardTime   = 0x7735940 (0x7735940 * 4ns = 0.5s)
    */
    timerDiscardParam.withdraw    = 0u;
    timerDiscardParam.threshold   = 0x3u;
    timerDiscardParam.monitorTime = 0x47868C0u;
    timerDiscardParam.guardTime   = 0x7735940u;

    /* Configure and retrieve timer discard entry 32 */
    timerDiscardEntryInfo.aclRuleIndex       = index;
    timerDiscardEntryInfo.pTimerDiscardParam = &timerDiscardParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_timer_discard_entry(unitChip, &timerDiscardEntryInfo));
    timerDiscardEntryInfoGet.aclRuleIndex       = index;
    timerDiscardEntryInfoGet.pTimerDiscardParam = &timerDiscardParamGet;
    CHK_FUN_RET(retVal, rtk_acl_getting_timer_discard_entry(unitChip, &timerDiscardEntryInfoGet));

    /* Expected Behaviour:
        a. Configure ACL entry 32 in Template1 and use template field 0.
        b. All packets received from physical PORT_3RD will be redirected to physical PORT_4TH.
        c. The timer discard function will be enabled, with a threshold of 3, a monitor time of 0.3 seconds, and a guard time of 0.5 seconds.
        d. When PORT_3RD receives a packet, it will start the timer discard function, and the first packet will be forwarded to PORT_4TH.
        e. If 3 packets are continuously received, the next packets will be dropped.
        f. Therefore, a total of 4 packets (including the packet that starts the timer discard function) can be forwarded to PORT_4TH.
        g. The fifth packet will be dropped.
        h. Any ACL function lookup miss will result in the packet being dropped.
    */
    return retVal;
}
/**@}*/ /* RTL907DV1_ACL_TIMER_DISCARD */
/**@}*/ /* RTL907DV1_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */