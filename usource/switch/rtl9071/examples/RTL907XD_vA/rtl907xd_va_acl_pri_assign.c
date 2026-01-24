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
 * @addtogroup RTL907DVA_ACL_EXAMPLE RTL907XD_VA ACL Example
 * @{
 */
/**
 * @addtogroup RTL907DVA_ACL_PRI_ASSIGN ACL Priority Assignment and Group Member Check Example
 * @brief This example showcases the configuration of ACL rules to prioritize traffic based on specific Layer 4 port conditions and group membership checks.
 *
 * @section Purpose
 *   This example demonstrates the assignment of flow-based priority and the use of group member checks in ACL configurations.
 *
 * @section Description
 *   This example is designed to illustrate how ACL can be used to manage traffic flow priorities based on group membership: \n
 *   a. Assign a specific internal TX queue priority based on L4 source or destination port conditions. \n
 *   b. Utilize group membership as a filter rule to further refine the ACL conditions. \n
 *   c. Allow packets that do not match the ACL conditions to be processed by L2 and VLAN functions.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dva_acl_pri_assign(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_acl_pri_assign(uint32 unit)
{
    /* +-----------------------------------------------------------------+ */
    /* |                     Template 2, PIE entry 64                    | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0        | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | SPHY_ORG | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 8000     | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 8000     | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = DVA_RTK_USER_PORT_1ST,
        .aclEn   = DVA_ACL_ENABLE,
        .defAct  = DVA_ACL_DEF_ACT_PERMIT,
    };
    RtkPort                port             = DVA_RTK_USER_PORT_1ST;
    AclRuleId              index            = 64u;
    AclTemplateId          tpIdx            = index / DVA_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t      templateInfo     = {0};
    AclPieEntryInfo_t      pieEntryInfo     = {0};
    AclTblAccessPieParam_t pieParam         = {0};
    AclActionEntryInfo_t   actionEntryInfo  = {0};
    AclTblAccessActParam_t actParam         = {0};
    AclL4PortMemberInfo_t  l4PortMemberInfo = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to permit packets */
    for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 2 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = DVA_IACL_FLD_SPHY_ORG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 64 */

    /* PORT_GROUP[15] = 1, Layer 4 TCP/UDP Port member check */
    pieParam.data0 = 0x8000u;
    pieParam.mask0 = 0x8000u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 64 */

    /*
        prioasn = 1,
        priority = 3 (flow-based priority)
    */
    actParam.prioasn           = 1u;
    actParam.prioInfo.priority = 3u;

    /* Configure action entry 64 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure member check entry */

    /* Configure the entry 0 of Layer 4 TCP/UDP port member check and member type is source port */
    l4PortMemberInfo.entryIdx = 0u;
    l4PortMemberInfo.type     = DVA_L4PORT_MEMBER_SRC_PORT;
    l4PortMemberInfo.member   = 0x1111u;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_member_set(unitChip, &l4PortMemberInfo));

    /* Configure the entry 1 of Layer 4 TCP/UDP port member check and member type is source port */
    l4PortMemberInfo.entryIdx = 1u;
    l4PortMemberInfo.type     = DVA_L4PORT_MEMBER_SRC_PORT;
    l4PortMemberInfo.member   = 0x2222;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_member_set(unitChip, &l4PortMemberInfo));

    /* Configure the entry 2 of Layer 4 TCP/UDP port member check and member type is destination port */
    l4PortMemberInfo.entryIdx = 1u;
    l4PortMemberInfo.type     = DVA_L4PORT_MEMBER_DEST_PORT;
    l4PortMemberInfo.member   = 0x3333;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_member_set(unitChip, &l4PortMemberInfo));

    /* Configure the entry 3 of Layer 4 TCP/UDP port member check and member type is destination port */
    l4PortMemberInfo.entryIdx = 1u;
    l4PortMemberInfo.type     = DVA_L4PORT_MEMBER_DEST_PORT;
    l4PortMemberInfo.member   = 0x4444;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_member_set(unitChip, &l4PortMemberInfo));

    /* Expected Behaviour:
        a. Configure ACL entry 64 in Template2 and use template field 0.
        b. If the packet's L4 source port is 1111 or 2222, or the destination port is 3333 or 4444, assign the internal TX queue priority to 3.
        c. A lookup miss in the ACL function is permitted to continue processing by the L2 and VLAN functions.
    */
    return retVal;
}
/**@}*/ /* RTL907DVA_ACL_PRI_ASSIGN */
/**@}*/ /* RTL907DVA_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */