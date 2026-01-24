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
 * @addtogroup RTL907CVB_ACL_EXAMPLE RTL907XC_VB ACL Example
 * @{
 */
/**
 * @addtogroup RTL907CVB_ACL_REDIRECT ACL Redirect Action Example
 * @brief This example illustrates how to configure ACL rules to redirect packets to multiple ports while enforcing rate limits.
 *
 * @section Purpose
 *   This example demonstrates the configuration and usage of ACL for packet redirection with rate limiting.
 *
 * @section Description
 *   This example configures ACL to redirect packets based on specific rules and apply rate limiting: \n
 *   a. Configure ACL entry 64 in Template2. \n
 *   b. Redirect packets received from physical PORT_1ST to physical PORT_2ND and PORT_3RD. \n
 *   c. Apply a forward rate limit of 80000 Kbps. \n
 *   d. Drop packets on any lookup miss in the ACL.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet cvb_acl_redirect(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet cvb_acl_redirect(uint32 unit)
{
    /* +-------------------------------------------------------------+ */
    /* |                   Template 2, PIE entry 64                  | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0    | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | FMT  | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0800 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 3C00 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = CVB_RTK_USER_PORT_1ST,
        .aclEn   = CVB_ACL_ENABLE,
        .defAct  = CVB_ACL_DEF_ACT_DROP,
    };
    RtkPort                    port                = CVB_RTK_USER_PORT_1ST;
    AclRuleId                  index               = 64u;
    AclTemplateId              tpIdx               = index / CVB_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t          templateInfo        = {0};
    AclPieEntryInfo_t          pieEntryInfo        = {0};
    AclTblAccessPieParam_t     pieParam            = {0};
    AclActionEntryInfo_t       actionEntryInfo     = {0};
    AclTblAccessActParam_t     actParam            = {0};
    AclPolicerEntryInfo_t      policerEntryInfo    = {0};
    AclTblAccessPolicerParam_t policerParam        = {0};
    AclPolicerEntryInfo_t      policerEntryInfoGet = {0};
    AclTblAccessPolicerParam_t policerParamGet     = {0};
    AclCounterEntryInfo_t      counterEntryInfo    = {0};
    AclTblAccessCounterParam_t counterParam        = {0};
    AclCounterEntryInfo_t      counterEntryInfoGet = {0};
    AclTblAccessCounterParam_t counterParamGet     = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 2 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 64 */

    /* SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST) */
    pieParam.data0 = (uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u;
    pieParam.mask0 = 0x3C00u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 64 */

    /*
        redir = 1,
        opcode = 0 (normal forward),
        dpm = CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD,
        crsvlan = 0 (forward in VLAN)
    */
    actParam.redir             = 1u;
    actParam.redirInfo.opcode  = 0u;
    actParam.redirInfo.dpm     = CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD;
    actParam.redirInfo.crsvlan = 0u;

    /* Configure action entry 64 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure policer entry 64 */

    /*
        enable = 1,
        tokencnt = 0 bytes (initial value),
        threshold = 8 Kbytes (initial bandwidth, in unit of 8 Kbytes),
        rate = 80000 Kbps (10000 Kbytes/second, in units of 8 Kbps)
    */
    policerParam.enable   = 1u;
    policerParam.tokencnt = 0u;
    /* 1 * 8Kbytes = 8 Kbytes */
    policerParam.threshold = 1u;
    /* 10000 * 8Kbps = 80000 Kbps */
    policerParam.rate = 10000u;

    /* Configure and retrieve policer entry 64 */
    policerEntryInfo.aclRuleIndex  = index;
    policerEntryInfo.pPolicerParam = &policerParam;
    CHK_FUN_RET(retVal, rtk_acl_policer_entry_set(unitChip, &policerEntryInfo));
    policerEntryInfoGet.aclRuleIndex  = index;
    policerEntryInfoGet.pPolicerParam = &policerParamGet;
    CHK_FUN_RET(retVal, rtk_acl_policer_entry_get(unitChip, &policerEntryInfoGet));

    /* Step 6. Configure counter entry 64 */

    /*
        enable = 1,
        cntmode = 1 (byte-based counter),
        counterH = 0 (initial value),
        counterL = 0 (initial value)
    */
    counterParam.enable   = 1u;
    counterParam.cntmode  = 1u;
    counterParam.counterH = 0u;
    counterParam.counterL = 0u;

    /* Configure and retrieve counter entry 64 */
    counterEntryInfo.aclRuleIndex  = index;
    counterEntryInfo.pCounterParam = &counterParam;
    CHK_FUN_RET(retVal, rtk_acl_counter_entry_set(unitChip, &counterEntryInfo));
    counterEntryInfoGet.aclRuleIndex  = index;
    counterEntryInfoGet.pCounterParam = &counterParamGet;
    CHK_FUN_RET(retVal, rtk_acl_counter_entry_get(unitChip, &counterEntryInfoGet));

    /* Expected Behavior:
        a. Configure ACL entry 64 in Template2 and use template field 0.
        b. All packets received from physical PORT_1ST will be redirected to physical PORT_2ND and PORT_3RD, with a forwarding rate of 80000 Kbps.
        c. Any lookup miss in the ACL function will result in the packet being dropped.
        d. When a packet hits this ACL rule, the total bytes of these packets will be recorded in the corresponding counter table.
    */
    return retVal;
}
/**@}*/ /* RTL907CVB_ACL_REDIRECT */
/**@}*/ /* RTL907CVB_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */