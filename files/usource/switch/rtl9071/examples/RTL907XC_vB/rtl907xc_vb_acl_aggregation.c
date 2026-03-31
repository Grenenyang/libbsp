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
 * @addtogroup RTL907CVB_ACL_AGGREGATION ACL Aggregation Example
 * @brief This example showcases the process of configuring and aggregating ACL rules to manage network traffic based on specific Layer4 port conditions.
 *
 * @section Purpose
 *   This example demonstrates the integration of multiple ACL rules to create sophisticated network traffic filters.
 *
 * @section Description
 *   This example illustrates the setup of ACL rules to filter packets based on combined source and destination port conditions: \n
 *   a. Configuring ACL entry 0 in Template0 for destination port filtering. \n
 *   b. Configuring ACL entry 32 in Template1 for source port filtering. \n
 *   c. Demonstrating the aggregation of these entries to filter packets that meet both source and destination conditions.
 *  @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet cvb_acl_aggregation(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet cvb_acl_aggregation(uint32 unit)
{
    /* +----------------------------------------------------------------+----------------------------------------------------------------+ */
    /* |                     Template 0, PIE entry 0                    |                    Template 1, PIE entry 32                    | */
    /* +---------+----+----+----+----+----+----+----+----+----+----+----+---------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0       | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 0       | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +---------+----+----+----+----+----+----+----+----+----+----+----+---------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | L4DPORT | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | L4SPORT | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +---------+----+----+----+----+----+----+----+----+----+----+----+---------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 1111    | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | 2222    | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +---------+----+----+----+----+----+----+----+----+----+----+----+---------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | FFFF    | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | FFFF    | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +---------+----+----+----+----+----+----+----+----+----+----+----+---------+----+----+----+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = CVB_RTK_USER_PORT_1ST,
        .aclEn   = CVB_ACL_ENABLE,
        .defAct  = CVB_ACL_DEF_ACT_PERMIT,
    };
    RtkPort                port              = CVB_RTK_USER_PORT_1ST;
    AclRuleId              index0            = 0u;
    AclRuleId              index32           = 32u;
    AclTemplateId          tpIdx0            = index0 / CVB_ACL_TEMPLATE_RULE_NUM;
    AclTemplateId          tpIdx1            = index32 / CVB_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t      templateInfo      = {0};
    AclPieEntryInfo_t      pieEntryInfo      = {0};
    AclTblAccessPieParam_t pieParam          = {0};
    AclActionEntryInfo_t   actionEntryInfo   = {0};
    AclTblAccessActParam_t actParam          = {0};
    AclAggregatorInfo_t    aggregatorInfo    = {0};
    AclAggregatorInfo_t    aggregatorInfoGet = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to permit packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx0;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_L4DPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure field type of template 1 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx1;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 4. Configure PIE entry 0 */

    /* DPORT = 0x1111u */
    pieParam.data0 = 0x1111u;
    pieParam.mask0 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index0;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 5. Configure PIE entry 32 */

    /* Clear PIE param */
    CHK_FUN_RET(retVal, drv_memset(&pieParam, 0, sizeof(pieParam)));

    /* SPORT = 0x2222 */
    pieParam.data0 = 0x2222u;
    pieParam.mask0 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 32 */
    pieEntryInfo.aclRuleIndex = index32;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 6. Configure action entry 0 */

    /*
        drop = 1 (drop action)
    */
    actParam.drop = 1u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index0;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 7. Configure action entry 32 */

    /*
        redir = 1,
        opcode = 0 (normal forward)
        dpm = CVB_RTK_USER_PORT_2ND | CVB_RTK_USER_PORT_3RD
        crsvlan = 0 (forward in VLAN)
    */
    actParam.redir             = 1u;
    actParam.redirInfo.opcode  = 0u;
    actParam.redirInfo.dpm     = CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD;
    actParam.redirInfo.crsvlan = 0u;

    /* Configure action entry 32 */
    actionEntryInfo.aclRuleIndex = index0;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 8. Configure PIE aggregation */

    /* Configure and retrieve PIE aggregation*/
    /*
        aclRuleIndex = index0
        aggOp = CVB_ACL_SEL_DOWN_PIE_AGG (select downside PIE after aggregation, it will execute the downside action)
    */
    aggregatorInfo.aclRuleIndex = index0;
    aggregatorInfo.aggOp        = CVB_ACL_SEL_DOWN_PIE_AGG;
    CHK_FUN_RET(retVal, rtk_acl_aggregator_set(unitChip, &aggregatorInfo));
    aggregatorInfoGet.aclRuleIndex = index0;
    CHK_FUN_RET(retVal, rtk_acl_aggregator_get(unitChip, &aggregatorInfoGet));

    /* Expected Behaviour:
        a. Configure the ACL entry 0 in Template0 and entry 32 in Template1, both using field 0.
        b. All packets that meet the rule of the L4 destination port being 0x1111 and the L4 source port being 0x2222 will be forwarded to PORT_2ND and PORT_3RD.
        c. Lookup miss in the ACL function is permitted to keep the process by L2 and VLAN functions.
    */
    return retVal;
}
/**@}*/ /* RTL907CVB_ACL_AGGREGATION  */
/**@}*/ /* RTL907CVB_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */