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
 * @addtogroup RTL907DVA_ACL_PRI_RMK ACL Priority Remarking and Range Check Example
 * @brief This example demonstrates how to configure ACL to remark VLAN tag priorities based on specific IP range criteria.
 *
 * @section Purpose
 *   This example demonstrates the configuration of ACL to remark priorities based on IP ranges.
 *
 * @section Description
 *   This example provides a detailed setup for remarking packet priorities when the source IP falls within a specific range: \n
 *   a. Configures ACL entry 64 in Template2. \n
 *   b. Uses template field 0 for IP range checks. \n
 *   c. Sets both inner and outer VLAN tag priorities to 3 if the IP is within the specified range. \n
 *   d. Allows packets that do not match the ACL to be processed by L2 and VLAN functions.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dva_acl_pri_rmk(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_acl_pri_rmk(uint32 unit)
{
    /* +--------------------------------------------------------------+ */
    /* |                   Template 2, PIE entry 64                   | */
    /* +-------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0     | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +-------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | RANG1 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +-------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0008  | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +-------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0008  | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +-------+----+----+----+----+----+----+----+----+----+----+----+ */

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
    RtkPort                     port                      = DVA_RTK_USER_PORT_1ST;
    AclRuleId                   index                     = 64u;
    AclTemplateId               tpIdx                     = index / DVA_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t           templateInfo              = {0};
    AclPieEntryInfo_t           pieEntryInfo              = {0};
    AclTblAccessPieParam_t      pieParam                  = {0};
    AclActionEntryInfo_t        actionEntryInfo           = {0};
    AclTblAccessActParam_t      actParam                  = {0};
    AclIpRangeInfo_t            ipRangeInfo               = {0};
    SvlanPortPktParserInfo_t    portPktParserInfo         = {0};
    VlanInfo_t                  vlanInfo                  = {0};
    SvlanPortEgrTagStatusInfo_t svlanPortEgrTagStatusInfo = {0};

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
    templateInfo.type     = DVA_IACL_FLD_RANGE1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 64 */

    /* IPRC[7:0] = 1, IPv4 range check for 8 regions, use entry 3 */
    pieParam.data0 = 0x0008u;
    pieParam.mask0 = 0x0008u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 64 */

    /*
        priormk = 1,
        tagSel = 3 (remark inner/outer tag priority),
        inPri = 3 (remarked inner priority to 3),
        outPri = 3 (remarked outer priority to 3)
    */
    actParam.priormk           = 1u;
    actParam.priRmkInfo.tagSel = 3u;
    actParam.priRmkInfo.inPri  = 3u;
    actParam.priRmkInfo.outPri = 3u;

    /* Configure action entry 64 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure IPv4 range check entry 3 */

    /* Configure the entry 3 of IPv4 range check */
    ipRangeInfo.entryIdx = 3u;
    ipRangeInfo.type     = DVA_IP_RANGE_SOURCE_IP;
    /* 192.168.1.255 */
    ipRangeInfo.upper = 0xC0A801FFu;
    /* 192.168.1.1 */
    ipRangeInfo.lower = 0xC0A80101u;
    CHK_FUN_RET(retVal, rtk_acl_ip_range_set(unitChip, &ipRangeInfo));

    /* Step 6. Enable outer tag parser for PORT_1ST */
    portPktParserInfo.enable = 1u;
    portPktParserInfo.port   = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_svlan_port_pkt_parser_en_set(unitChip, &portPktParserInfo));

    /* Step 7. Configure VLAN 100 */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)DVA_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 8. Configure all ports with VLAN outer tag */
    for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
    {
        svlanPortEgrTagStatusInfo.port      = port;
        svlanPortEgrTagStatusInfo.tagStatus = EM_TAG_STATUS_WITH_TAG;
        CHK_FUN_RET(retVal, rtk_svlan_port_egr_tag_status_set(unitChip, &svlanPortEgrTagStatusInfo));
    }

    /* Expected Behaviour:
        a. Configure ACL entry 64 in Template2 and use template field 0.
        b. If the IP range is between 192.168.1.1 and 192.168.1.255, the inner and outer tag priorities will both be set to 3.
        c. Lookup misses in the ACL function are permitted to continue processing by L2 and VLAN functions.
    */
    return retVal;
}
/**@}*/ /* RTL907DVA_ACL_PRI_RMK */
/**@}*/ /* RTL907DVA_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */