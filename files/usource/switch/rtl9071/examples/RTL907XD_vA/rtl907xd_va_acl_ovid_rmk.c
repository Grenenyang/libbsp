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
#include "rtkas_api_vlan.h"

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
 * @addtogroup RTL907DVA_ACL_OVID_RMK ACL Outer VLAN Tag Remarking Example
 * @brief This example showcases the dynamic modification of the outer VLAN ID for packets using ACL rules.
 *
 * @section Purpose
 *   This example demonstrates the capability to dynamically remark the outer VLAN ID of packets based on ACL rules.
 *
 * @section Description
 *   This example configures an ACL to remark the outer VLAN ID of packets: \n
 *   a. It targets packets with a specific source IP address. \n
 *   b. It increments the outer VLAN ID by 1. \n
 *   c. It ensures that packets not matching the ACL criteria are processed by standard L2 and VLAN functions.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dva_acl_ovid_rmk(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_acl_ovid_rmk(uint32 unit)
{
    /* +---------------------------------------------------------------+ */
    /* |                    Template 2, PIE entry 64                   | */
    /* +------+------+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0    | 1    | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +------+------+----+----+----+----+----+----+----+----+----+----+ */
    /* | SIP1 | SIP0 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+------+----+----+----+----+----+----+----+----+----+----+ */
    /* | C0A8 | 0102 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+------+----+----+----+----+----+----+----+----+----+----+ */
    /* | FFFF | FFFF | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+------+----+----+----+----+----+----+----+----+----+----+ */

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
    RtkPort                  port              = DVA_RTK_USER_PORT_1ST;
    AclRuleId                index             = 64u;
    AclTemplateId            tpIdx             = index / DVA_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t        templateInfo      = {0};
    AclPieEntryInfo_t        pieEntryInfo      = {0};
    AclTblAccessPieParam_t   pieParam          = {0};
    AclActionEntryInfo_t     actionEntryInfo   = {0};
    AclTblAccessActParam_t   actParam          = {0};
    SvlanPortPktParserInfo_t portPktParserInfo = {0};

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
    templateInfo.type     = DVA_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = DVA_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 64 */

    /* SIP: 192.168.1.2 */
    /* SIP1 = 0xC0A8u */
    pieParam.data0 = 0xC0A8u;
    pieParam.mask0 = 0xFFFFu;
    /* SIP1 = 0x0102u */
    pieParam.data1 = 0x0102u;
    pieParam.mask1 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 64 */

    /*
        otag = 1,
        outVidCtl = 3 (packet's new outer VLAN ID = packet's outer tag VLAN ID + OutVIDInfo),
        outVidInfo = 1 (outer VLAN offset),
        outTagOp = 1 (packet sent with modified outer VLAN tag)
    */
    actParam.otag                    = 1u;
    actParam.outTagOpInfo.outVidCtl  = 3u;
    actParam.outTagOpInfo.outVidInfo = 1u;
    actParam.outTagOpInfo.outTagOp   = 1u;

    /* Configure action entry 64 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Enable outer tag parser for PORT_1ST */
    portPktParserInfo.enable = 1u;
    portPktParserInfo.port   = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_svlan_port_pkt_parser_en_set(unitChip, &portPktParserInfo));

    /* Expected Behavior:
        a. Configure ACL entry 64 in Template2 and use template fields 0 and 1.
        b. This ACL entry only hits when the DIP is 192.168.1.2.
        c. This packet will be remarked with the new VLAN outer tag ID, which is the original OVID plus 1.
        d. Lookup miss in the ACL function is permitted to keep the process by L2 and VLAN function.
    */
    return retVal;
}
/**@}*/ /* RTL907DVA_ACL_OVID_RMK */
/**@}*/ /* RTL907DVA_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */