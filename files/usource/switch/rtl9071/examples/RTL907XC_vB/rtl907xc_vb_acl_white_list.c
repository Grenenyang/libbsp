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
 * @addtogroup RTL907CVB_ACL_EXAMPLE RTL907XC_VB ACL Example
 * @{
 */
/**
 * @addtogroup RTL907CVB_ACL_WHITELIST ACL Whitelist Example
 * @brief This example illustrates the process of setting up ACL to create a whitelist that permits only specific types of network traffic while blocking all others.
 *
 * @section Purpose
 *   This example demonstrates configuring ACL to create a whitelist that allows only specified traffic.
 *
 * @section Description
 *   This example provides a detailed explanation of how to configure ACL to create a whitelist, \n
 *   allowing only specified traffic and dropping all others: \n
 *   a. Configures ACL templates and entries. \n
 *   b. Sets up actions for allowed traffic. \n
 *   c. Demonstrates packet flow through the configured ACL.
 *
 * @image html acl/acl_whitelist.jpg
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet cvb_acl_white_list1(uint32 unit);
RtkApiRet cvb_acl_white_list2(uint32 unit);
RtkApiRet cvb_acl_white_list3(uint32 unit);
RtkApiRet cvb_acl_white_list4(uint32 unit);
RtkApiRet cvb_acl_white_list5(uint32 unit);
RtkApiRet cvb_acl_white_list6(uint32 unit);
RtkApiRet cvb_acl_white_list7(uint32 unit);
RtkApiRet cvb_acl_white_list8(uint32 unit);
RtkApiRet cvb_acl_white_list9(uint32 unit);
RtkApiRet cvb_acl_white_list10(uint32 unit);
RtkApiRet cvb_acl_white_list11(uint32 unit);
RtkApiRet cvb_acl_white_list12(uint32 unit);
RtkApiRet cvb_acl_white_list13(uint32 unit);
RtkApiRet cvb_acl_white_list14(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet cvb_acl_white_list1(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4411  | 2233  | 0011  | 02BC | 070B | C0A8 | 0006  | 3458    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:11 */
    pieParam.data1 = 0x4411u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 700 */
    pieParam.data4 = 0x02BCu;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 7.11 */
    pieParam.data5 = 0x070Bu;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 13400 */
    pieParam.data8 = 0x3458u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 700 */
    vlanInfo.vid                      = 700u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:11
    VLAN Tag ID                : 700
    IP Address                 : 192.168.7.11
    Protocol                   : TCP
    Sport                      : 13400
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list2(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4412  | 2233  | 0011  | 0064 | 010C | C0A8 | 0006  | 3039    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:12 */
    pieParam.data1 = 0x4412u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 100 */
    pieParam.data4 = 0x0064u;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 1.12 */
    pieParam.data5 = 0x010Cu;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 12345 */
    pieParam.data8 = 0x3039u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 100 */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:12
    VLAN Tag ID                : 100
    IP Address                 : 192.168.1.12
    Protocol                   : TCP
    Sport                      : 12345
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list3(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4412  | 2233  | 0011  | 02BC | 070C | C0A8 | 0006  | 3458    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:12 */
    pieParam.data1 = 0x4412u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 700 */
    pieParam.data4 = 0x02BCu;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 7.12 */
    pieParam.data5 = 0x070Cu;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 13400 */
    pieParam.data8 = 0x3458u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 700 */
    vlanInfo.vid                      = 700u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:12
    VLAN Tag ID                : 700
    IP Address                 : 192.168.7.12
    Protocol                   : TCP
    Sport                      : 13400
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list4(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4423  | 2233  | 0011  | 0064 | 0117 | C0A8 | 0006  | 3039    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:23 */
    pieParam.data1 = 0x4423u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 100 */
    pieParam.data4 = 0x0064u;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 1.23 */
    pieParam.data5 = 0x0117u;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 12345 */
    pieParam.data8 = 0x3039u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 100 */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:23
    VLAN Tag ID                : 100
    IP Address                 : 192.168.1.23
    Protocol                   : TCP
    Sport                      : 12345
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list5(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4423  | 2233  | 0011  | 02BC | 0717 | C0A8 | 0006  | 3458    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:23 */
    pieParam.data1 = 0x4423u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 700 */
    pieParam.data4 = 0x02BCu;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 7.23 */
    pieParam.data5 = 0x0717u;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 13400 */
    pieParam.data8 = 0x3458u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 700 */
    vlanInfo.vid                      = 700u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:23
    VLAN Tag ID                : 700
    IP Address                 : 192.168.7.23
    Protocol                   : TCP
    Sport                      : 13400
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list6(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4424  | 2233  | 0011  | 0064 | 0118 | C0A8 | 0006  | 3039    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:24 */
    pieParam.data1 = 0x4424u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 100 */
    pieParam.data4 = 0x0064u;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 7.24 */
    pieParam.data5 = 0x0118u;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 12345 */
    pieParam.data8 = 0x3039u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 100 */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:24
    VLAN Tag ID                : 100
    IP Address                 : 192.168.1.24
    Protocol                   : TCP
    Sport                      : 12345
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list7(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4424  | 2233  | 0011  | 02BC | 0718 | C0A8 | 0006  | 3458    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:24 */
    pieParam.data1 = 0x4424u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 700 */
    pieParam.data4 = 0x02BCu;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 7.24 */
    pieParam.data5 = 0x0718u;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 13400 */
    pieParam.data8 = 0x3458u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 700 */
    vlanInfo.vid                      = 700u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:24
    VLAN Tag ID                : 700
    IP Address                 : 192.168.7.24
    Protocol                   : TCP
    Sport                      : 13400
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list8(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4425  | 2233  | 0011  | 0064 | 0119 | C0A8 | 0006  | 3039    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:25 */
    pieParam.data1 = 0x4425u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 100 */
    pieParam.data4 = 0x0064u;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 1.25 */
    pieParam.data5 = 0x0119u;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 12345 */
    pieParam.data8 = 0x3039u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 100 */
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:25
    VLAN Tag ID                : 100
    IP Address                 : 192.168.1.25
    Protocol                   : TCP
    Sport                      : 12345
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list9(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                     Template 0, PIE entry 0                                       | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 08A0 | 4425  | 2233  | 0011  | 02BC | 0719 | C0A8 | 0006  | 3458    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = CVB_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = CVB_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = CVB_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = CVB_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = CVB_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = CVB_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        SPHY[13:10]  = CVB_RTK_USER_PORT_1ST (ingress port number = CVB_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)cvb_g_aclUserPort2sysPort[CVB_RTK_USER_PORT_1ST] << 10u));
    pieParam.mask0 = 0x3CBEu;

    /* SMAC[15:0] = 44:25 */
    pieParam.data1 = 0x4425u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data2 = 0x2233u;
    pieParam.mask2 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data3 = 0x0011u;
    pieParam.mask3 = 0xFFFFu;

    /* IVID = 700 */
    pieParam.data4 = 0x02BCu;
    pieParam.mask4 = 0x0FFFu;

    /* SIP[15:0] = 1.25 */
    pieParam.data5 = 0x0719u;
    pieParam.mask5 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data6 = 0xC0A8u;
    pieParam.mask6 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data7 = 0x0006u;
    pieParam.mask7 = 0x00FFu;

    /* SPORT = 13400 */
    pieParam.data8 = 0x3458u;
    pieParam.mask8 = 0xFFFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure VLAN 700 */
    vlanInfo.vid                      = 700u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:25
    VLAN Tag ID                : 700
    IP Address                 : 192.168.7.25
    Protocol                   : TCP
    Sport                      : 13400
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list10(uint32 unit)
{
    /* +-------------------------------------------------------------------+ */
    /* |                      Template 0, PIE entry 0                      | */
    /* +--------+--------+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0      | 1      | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +--------+--------+----+----+----+----+----+----+----+----+----+----+ */
    /* | RANGE0 | RANGE1 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+--------+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0011   | 1101   | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+--------+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0011   | 1101   | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+--------+----+----+----+----+----+----+----+----+----+----+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    AclPktLenRangeInfo_t   pktLenRangeInfo = {0};
    AclVidRangeInfo_t      vidRangeInfo    = {0};
    AclIpRangeInfo_t       ipRangeInfo     = {0};
    AclL4PortRangeInfo_t   l4PortRangeInfo = {0};
    AclSpmMemberInfo_t     spmMemberInfo   = {0};
    AclPieBlockId          pieBlockId      = 0u;
    AclPriorityPieInfo_t   priorityPieInfo = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_RANGE0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_RANGE1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        LENRC[3:0] = 1, packet length range check for 4 regions, use entry 0
        VIDRC[11:4] = 1, VLAN ID range check for 8 regions, use entry 0
    */
    pieParam.data0 = 0x0011u;
    pieParam.mask0 = 0x0011u;

    /*
        IPRC[7:0] = 1, IPv4 range check for 8 regions, use entry 0
        SPM[11:8] = 1, source port mask member check for 4 regions, use entry 0
        PRC[15:12] = 1, layer4 TCP/UDP port range check for 4 regions, use entry 0
    */
    pieParam.data1 = 0x1101u;
    pieParam.mask1 = 0x1101u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure range check entry */

    /* Configure the entry 0 of packet length range check */
    pktLenRangeInfo.entryIdx = 0u;
    pktLenRangeInfo.upper    = 513u;
    pktLenRangeInfo.lower    = 511u;
    CHK_FUN_RET(retVal, rtk_acl_pkt_len_range_set(unitChip, &pktLenRangeInfo));

    /* Configure the entry 0 of VLAN ID range check */
    vidRangeInfo.entryIdx = 0u;
    vidRangeInfo.type     = CVB_VID_RANGE_IVID_ONLY;
    vidRangeInfo.upper    = 8u;
    vidRangeInfo.lower    = 6u;
    CHK_FUN_RET(retVal, rtk_acl_vid_range_set(unitChip, &vidRangeInfo));

    /* Configure the entry 0 of IPv4 range check */
    ipRangeInfo.entryIdx = 0u;
    ipRangeInfo.type     = CVB_IP_RANGE_SOURCE_IP;
    ipRangeInfo.upper    = 0x22222223u;
    ipRangeInfo.lower    = 0x22222221u;
    CHK_FUN_RET(retVal, rtk_acl_ip_range_set(unitChip, &ipRangeInfo));

    /* Configure the entry 0 of layer4 TCP port range check */
    l4PortRangeInfo.entryIdx = 0u;
    l4PortRangeInfo.type     = CVB_L4PORT_RANGE_TCP_SOURCE_PORT;
    l4PortRangeInfo.upper    = 43708u;
    l4PortRangeInfo.lower    = 43706u;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_range_set(unitChip, &l4PortRangeInfo));

    /* Step 6. Configure member check entry */

    /* Configure the entry 0 of source port mask member check */
    spmMemberInfo.entryIdx = 0u;
    spmMemberInfo.spm      = (uint32)CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND;
    CHK_FUN_RET(retVal, rtk_acl_spm_member_set(unitChip, &spmMemberInfo));

    /* Step 7. Configure each PIE block priority from high to low, with the PIE block index ranging from smallest to largest */
    for(pieBlockId = 0u; pieBlockId <= CVB_ACL_PIE_BLOCK_BOUND; pieBlockId++)
    {
        priorityPieInfo.pieBlock = pieBlockId;
        priorityPieInfo.priVal   = CVB_ACL_PIE_BLOCK_BOUND - pieBlockId;
        CHK_FUN_RET(retVal, rtk_acl_priority_pie_set(unitChip, &priorityPieInfo));
    }

    /* Step 8. Configure VLAN 7 */
    vlanInfo.vid                      = 7u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)CVB_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST and CVB_RTK_USER_PORT_2ND
    VLAN Tag ID                : 6 ~ 8
    IP Address                 : 34.34.34.33 ~ 34.34.34.35
    Protocol                   : TCP
    Sport                      : 43706 ~ 43708
    Packet Length              : 511 ~ 513
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list11(uint32 unit)
{
    /* +-------------------------------------------------------------------------------------+ */
    /* |                               Template 0, PIE entry 0                               | */
    /* +--------+--------+----------+----------------+----+----+----+----+----+----+----+----+ */
    /* | 0      | 1      | 2        | 3              | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +--------+--------+----------+----------------+----+----+----+----+----+----+----+----+ */
    /* | RANGE0 | RANGE1 | SPHY_ORG | RANGCHK_MAC_RC | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+--------+----------+----------------+----+----+----+----+----+----+----+----+ */
    /* | 0011   | 1100   | 0001     | 0001           | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+--------+----------+----------------+----+----+----+----+----+----+----+----+ */
    /* | 0011   | 1100   | 0001     | 0001           | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+--------+----------+----------------+----+----+----+----+----+----+----+----+ */

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
    AclEnableCtrlInfo_t    enableCtrlInfoGet  = {0};
    RtkPort                port               = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx              = 0u;
    AclRuleId              index              = 0u;
    AclTemplateInfo_t      templateInfo       = {0};
    AclTemplateInfo_t      templateInfoGet    = {0};
    AclPieEntryInfo_t      pieEntryInfo       = {0};
    AclPieEntryInfo_t      pieEntryInfoGet    = {0};
    AclTblAccessPieParam_t pieParam           = {0};
    AclTblAccessPieParam_t pieParamGet        = {0};
    AclActionEntryInfo_t   actionEntryInfo    = {0};
    AclActionEntryInfo_t   actionEntryInfoGet = {0};
    AclTblAccessActParam_t actParam           = {0};
    AclTblAccessActParam_t actParamGet        = {0};
    AclPktLenRangeInfo_t   pktLenRangeInfo    = {0};
    AclPktLenRangeInfo_t   pktLenRangeInfoGet = {0};
    AclVidRangeInfo_t      vidRangeInfo       = {0};
    AclVidRangeInfo_t      vidRangeInfoGet    = {0};
    AclL4PortRangeInfo_t   l4PortRangeInfo    = {0};
    AclL4PortRangeInfo_t   l4PortRangeInfoGet = {0};
    AclIp6RangeInfo_t      ip6RangeInfo       = {0};
    AclIp6RangeInfo_t      ip6RangeInfoGet    = {0};
    AclMacRangeInfo_t      macRangeInfo       = {0};
    AclMacRangeInfo_t      macRangeInfoGet    = {0};
    AclSpmMemberInfo_t     spmMemberInfo      = {0};
    AclSpmMemberInfo_t     spmMemberInfoGet   = {0};
    AclPieBlockId          pieBlockId         = 0u;
    AclPriorityPieInfo_t   priorityPieInfo    = {0};
    AclPriorityPieInfo_t   priorityPieInfoGet = {0};
    VlanInfo_t             vlanInfo           = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        /* Configure acl enable control */
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
        /* Retrieve acl enable control */
        enableCtrlInfoGet.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_get(unitChip, &enableCtrlInfoGet));
    }

    /* Step 2. Configure field type of template 0 */

    /* Configure and retrieve field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_RANGE0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));
    templateInfoGet.tpIdx    = tpIdx;
    templateInfoGet.fieldIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_template_getting(unitChip, &templateInfoGet));

    /* Configure and retrieve field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_RANGE1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));
    templateInfoGet.tpIdx    = tpIdx;
    templateInfoGet.fieldIdx = 1u;
    CHK_FUN_RET(retVal, rtk_acl_template_getting(unitChip, &templateInfoGet));

    /* Configure and retrieve field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = CVB_IACL_FLD_SPHY_ORG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));
    templateInfoGet.tpIdx    = tpIdx;
    templateInfoGet.fieldIdx = 2u;
    CHK_FUN_RET(retVal, rtk_acl_template_getting(unitChip, &templateInfoGet));

    /* Configure and retrieve field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = CVB_IACL_FLD_RANGCHK_MAC_RC;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));
    templateInfoGet.tpIdx    = tpIdx;
    templateInfoGet.fieldIdx = 3u;
    CHK_FUN_RET(retVal, rtk_acl_template_getting(unitChip, &templateInfoGet));

    /* Step 3. Configure PIE entry 0 */

    /*
        LENRC[3:0] = 1, packet length range check for 4 regions, use entry 0
        VIDRC[11:4] = 1, VLAN ID range check for 8 regions, use entry 0
    */
    pieParam.data0 = 0x0011u;
    pieParam.mask0 = 0x0011u;

    /*
        SPM[11:8] = 1, source port mask member check for 4 regions, use entry 0
        PRC[15:12] = 1, layer4 TCP/UDP port range check for 4 regions, use entry 0
    */
    pieParam.data1 = 0x1100u;
    pieParam.mask1 = 0x1100u;

    /*
        IP6RC[7:0] = 1, IPv6 range check for 8 regions, use entry 0
    */
    pieParam.data2 = 0x0001u;
    pieParam.mask2 = 0x0001u;

    /*
        MACRC[15:0] = 1, MAC range check for 16 region, use entry 0
    */
    pieParam.data3 = 0x0001u;
    pieParam.mask3 = 0x0001u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure and retrieve Pie entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));
    pieEntryInfoGet.aclRuleIndex = index;
    pieEntryInfoGet.pPieParam    = &pieParamGet;
    CHK_FUN_RET(retVal, rtk_acl_getting_pie_entry(unitChip, &pieEntryInfoGet));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure and retrieve action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));
    actionEntryInfoGet.aclRuleIndex = index;
    actionEntryInfoGet.pActParam    = &actParamGet;
    CHK_FUN_RET(retVal, rtk_acl_getting_action_entry(unitChip, &actionEntryInfoGet));

    /* Step 5. Configure range check entry */

    /* Configure and retrieve the entry 0 of packet length range check */
    pktLenRangeInfo.entryIdx = 0u;
    pktLenRangeInfo.upper    = 513u;
    pktLenRangeInfo.lower    = 511u;
    CHK_FUN_RET(retVal, rtk_acl_pkt_len_range_set(unitChip, &pktLenRangeInfo));
    pktLenRangeInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_pkt_len_range_get(unitChip, &pktLenRangeInfoGet));

    /* Configure and retrieve the entry 0 of VLAN ID range check */
    vidRangeInfo.entryIdx = 0u;
    vidRangeInfo.type     = CVB_VID_RANGE_IVID_ONLY;
    vidRangeInfo.upper    = 8u;
    vidRangeInfo.lower    = 6u;
    CHK_FUN_RET(retVal, rtk_acl_vid_range_set(unitChip, &vidRangeInfo));
    vidRangeInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_vid_range_get(unitChip, &vidRangeInfoGet));

    /* Configure and retrieve the entry 0 of layer4 TCP port range check */
    l4PortRangeInfo.entryIdx = 0u;
    l4PortRangeInfo.type     = CVB_L4PORT_RANGE_TCP_SOURCE_PORT;
    l4PortRangeInfo.upper    = 43708u;
    l4PortRangeInfo.lower    = 43706u;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_range_set(unitChip, &l4PortRangeInfo));
    l4PortRangeInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_range_get(unitChip, &l4PortRangeInfoGet));

    /* Configure and retrieve the entry 0 of IPv6 range check */
    /*
        Upper: 2000:0000:0000:0000:0000:0000:0000:0003
        Lower: 2000:0000:0000:0000:0000:0000:0000:0001
    */
    ip6RangeInfo.entryIdx  = 0u;
    ip6RangeInfo.type      = CVB_IP_RANGE_SOURCE_IP;
    ip6RangeInfo.upper[0]  = 0x20u;
    ip6RangeInfo.upper[1]  = 0x00u;
    ip6RangeInfo.upper[2]  = 0x00u;
    ip6RangeInfo.upper[3]  = 0x00u;
    ip6RangeInfo.upper[4]  = 0x00u;
    ip6RangeInfo.upper[5]  = 0x00u;
    ip6RangeInfo.upper[6]  = 0x00u;
    ip6RangeInfo.upper[7]  = 0x00u;
    ip6RangeInfo.upper[8]  = 0x00u;
    ip6RangeInfo.upper[9]  = 0x00u;
    ip6RangeInfo.upper[10] = 0x00u;
    ip6RangeInfo.upper[11] = 0x00u;
    ip6RangeInfo.upper[12] = 0x00u;
    ip6RangeInfo.upper[13] = 0x00u;
    ip6RangeInfo.upper[14] = 0x00u;
    ip6RangeInfo.upper[15] = 0x03u;

    ip6RangeInfo.lower[0]  = 0x20u;
    ip6RangeInfo.lower[1]  = 0x00u;
    ip6RangeInfo.lower[2]  = 0x00u;
    ip6RangeInfo.lower[3]  = 0x00u;
    ip6RangeInfo.lower[4]  = 0x00u;
    ip6RangeInfo.lower[5]  = 0x00u;
    ip6RangeInfo.lower[6]  = 0x00u;
    ip6RangeInfo.lower[7]  = 0x00u;
    ip6RangeInfo.lower[8]  = 0x00u;
    ip6RangeInfo.lower[9]  = 0x00u;
    ip6RangeInfo.lower[10] = 0x00u;
    ip6RangeInfo.lower[11] = 0x00u;
    ip6RangeInfo.lower[12] = 0x00u;
    ip6RangeInfo.lower[13] = 0x00u;
    ip6RangeInfo.lower[14] = 0x00u;
    ip6RangeInfo.lower[15] = 0x01u;
    CHK_FUN_RET(retVal, rtk_acl_ip6_range_set(unitChip, &ip6RangeInfo));
    ip6RangeInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_ip6_range_get(unitChip, &ip6RangeInfoGet));

    /* Configure and retrieve the entry 0 of MAC range check */
    /*
        Upper: 00:11:22:33:44:25
        Lower: 00:11:22:33:44:23
    */
    macRangeInfo.entryIdx = 0u;
    macRangeInfo.type     = CVB_MAC_RANGE_SOURCE_MAC;
    macRangeInfo.upper[0] = 0x00;
    macRangeInfo.upper[1] = 0x11;
    macRangeInfo.upper[2] = 0x22;
    macRangeInfo.upper[3] = 0x33;
    macRangeInfo.upper[4] = 0x44;
    macRangeInfo.upper[5] = 0x25;

    macRangeInfo.lower[0] = 0x00;
    macRangeInfo.lower[1] = 0x11;
    macRangeInfo.lower[2] = 0x22;
    macRangeInfo.lower[3] = 0x33;
    macRangeInfo.lower[4] = 0x44;
    macRangeInfo.lower[5] = 0x23;
    CHK_FUN_RET(retVal, rtk_acl_mac_range_set(unitChip, &macRangeInfo));
    macRangeInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_mac_range_get(unitChip, &macRangeInfoGet));

    /* Step 6. Configure member check entry */

    /* Configure and retrieve the entry 0 of source port mask member check */
    spmMemberInfo.entryIdx = 0u;
    spmMemberInfo.spm      = (uint32)CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND;
    CHK_FUN_RET(retVal, rtk_acl_spm_member_set(unitChip, &spmMemberInfo));
    spmMemberInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_spm_member_get(unitChip, &spmMemberInfoGet));

    /* Step 7. Configure each PIE block priority from high to low, with the PIE block index ranging from smallest to largest */
    for(pieBlockId = 0u; pieBlockId <= CVB_ACL_PIE_BLOCK_BOUND; pieBlockId++)
    {
        /* Configure PIE priority value */
        priorityPieInfo.pieBlock = pieBlockId;
        priorityPieInfo.priVal   = CVB_ACL_PIE_BLOCK_BOUND - pieBlockId;
        CHK_FUN_RET(retVal, rtk_acl_priority_pie_set(unitChip, &priorityPieInfo));
        /* Retrieve PIE priority value */
        priorityPieInfoGet.pieBlock = pieBlockId;
        CHK_FUN_RET(retVal, rtk_acl_priority_pie_get(unitChip, &priorityPieInfoGet));
    }

    /* Step 8. Configure VLAN 7 */
    vlanInfo.vid                      = 7u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)CVB_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    Switch Physical Port ID    : CVB_RTK_USER_PORT_1ST and CVB_RTK_USER_PORT_2ND
    MAC Address                : 00:11:22:33:44:23 ~ 00:11:22:33:44:25
    VLAN Tag ID                : 6 ~ 8
    IPv6 Address               : 2000::01 ~ 2000::03
    Protocol                   : TCP
    Sport                      : 43706 ~ 43708
    Packet Length              : 511 ~ 513
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list12(uint32 unit)
{
    /* +-----------------------------------------------------------------------------------+ */
    /* |                              Template 0, PIE entry 0                              | */
    /* +----------+----------------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0        | 1                    | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +----------+----------------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | SPHY_ORG | RANGCHK_MAC_GROUP_RC | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----------------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | E000     | 0001                 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----------------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | E000     | 0001                 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----------------------+----+----+----+----+----+----+----+----+----+----+ */

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
    RtkPort                port                = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx               = 0u;
    AclRuleId              index               = 0u;
    AclTemplateInfo_t      templateInfo        = {0};
    AclPieEntryInfo_t      pieEntryInfo        = {0};
    AclTblAccessPieParam_t pieParam            = {0};
    AclActionEntryInfo_t   actionEntryInfo     = {0};
    AclTblAccessActParam_t actParam            = {0};
    AclL4PortMemberInfo_t  l4PortMemberInfo    = {0};
    AclL4PortMemberInfo_t  l4PortMemberInfoGet = {0};
    AclVidMemberInfo_t     vidMemberInfo       = {0};
    AclVidMemberInfo_t     vidMemberInfoGet    = {0};
    AclIpMemberInfo_t      ipMemberInfo        = {0};
    AclIpMemberInfo_t      ipMemberInfoGet     = {0};
    AclMacMemberInfo_t     macMemberInfo       = {0};
    AclMacMemberInfo_t     macMemberInfoGet    = {0};
    AclPieBlockId          pieBlockId          = 0u;
    AclPriorityPieInfo_t   priorityPieInfo     = {0};
    VlanInfo_t             vlanInfo            = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_SPHY_ORG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = CVB_IACL_FLD_RANGCHK_MAC_GROUP_RC;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        PORT_GROUP[15] = 1, Layer 4 TCP/UDP Port member check
        VID_GROUP[14] = 1, VLAN ID member check
        IP_GROUP[13] = 1, IPv4 member check
    */
    pieParam.data0 = 0xE000u;
    pieParam.mask0 = 0xE000u;

    /*
        MAC_GROUP[0] = 1, MAC member check
    */
    pieParam.data1 = 0x0001u;
    pieParam.mask1 = 0x0001u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure member check entry */

    /* Configure and retrieve the entry 0 of Layer 4 TCP/UDP port member check */
    l4PortMemberInfo.entryIdx = 0u;
    l4PortMemberInfo.type     = CVB_L4PORT_MEMBER_SRC_PORT;
    l4PortMemberInfo.member   = 43707u;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_member_set(unitChip, &l4PortMemberInfo));
    l4PortMemberInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_member_get(unitChip, &l4PortMemberInfoGet));

    /* Configure and retrieve the entry 0 of VLAN ID member check */
    vidMemberInfo.entryIdx = 0u;
    vidMemberInfo.type     = CVB_VID_MEMBER_IVID;
    vidMemberInfo.member   = 7u;
    CHK_FUN_RET(retVal, rtk_acl_vid_member_set(unitChip, &vidMemberInfo));
    vidMemberInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_vid_member_get(unitChip, &vidMemberInfoGet));

    /* Configure and retrieve the entry 0 of IPv4 member check */
    ipMemberInfo.entryIdx = 0u;
    ipMemberInfo.type     = CVB_IP_MEMBER_SIP;
    ipMemberInfo.member   = 0x22222222u;
    CHK_FUN_RET(retVal, rtk_acl_ip_member_set(unitChip, &ipMemberInfo));
    ipMemberInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_ip_member_get(unitChip, &ipMemberInfoGet));

    /* Configure and retrieve the entry 0 of MAC member check */
    /* member: 00:11:22:33:44:25 */
    macMemberInfo.entryIdx  = 0u;
    macMemberInfo.type      = CVB_MAC_MEMBER_SMAC;
    macMemberInfo.member[0] = 0x00u;
    macMemberInfo.member[1] = 0x11u;
    macMemberInfo.member[2] = 0x22u;
    macMemberInfo.member[3] = 0x33u;
    macMemberInfo.member[4] = 0x44u;
    macMemberInfo.member[5] = 0x25u;
    CHK_FUN_RET(retVal, rtk_acl_mac_member_set(unitChip, &macMemberInfo));
    macMemberInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_mac_member_get(unitChip, &macMemberInfoGet));

    /* Step 6. Configure each PIE block priority from high to low, with the PIE block index ranging from smallest to largest */
    for(pieBlockId = 0u; pieBlockId <= CVB_ACL_PIE_BLOCK_BOUND; pieBlockId++)
    {
        priorityPieInfo.pieBlock = pieBlockId;
        priorityPieInfo.priVal   = CVB_ACL_PIE_BLOCK_BOUND - pieBlockId;
        CHK_FUN_RET(retVal, rtk_acl_priority_pie_set(unitChip, &priorityPieInfo));
    }

    /* Step 7. Configure VLAN 7 */
    vlanInfo.vid                      = 7u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)CVB_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    MAC Address                : 00:11:22:33:44:25
    VLAN Tag ID                : 7
    IP Address                 : 34.34.34.34
    Protocol                   : TCP or UDP
    Sport                      : 43707
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list13(uint32 unit)
{
    /* +-----------------------------------------------------------------+ */
    /* |                     Template 0, PIE entry 0                     | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0        | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | SPHY_ORG | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | D000     | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | D000     | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */

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
    RtkPort                port             = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx            = 0u;
    AclRuleId              index            = 0u;
    AclTemplateInfo_t      templateInfo     = {0};
    AclPieEntryInfo_t      pieEntryInfo     = {0};
    AclTblAccessPieParam_t pieParam         = {0};
    AclActionEntryInfo_t   actionEntryInfo  = {0};
    AclTblAccessActParam_t actParam         = {0};
    AclL4PortMemberInfo_t  l4PortMemberInfo = {0};
    AclVidMemberInfo_t     vidMemberInfo    = {0};
    AclIp6MemberInfo_t     ip6MemberInfo    = {0};
    AclIp6MemberInfo_t     ip6MemberInfoGet = {0};
    AclPieBlockId          pieBlockId       = 0u;
    AclPriorityPieInfo_t   priorityPieInfo  = {0};
    VlanInfo_t             vlanInfo         = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_SPHY_ORG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        PORT_GROUP[15] = 1, Layer 4 TCP/UDP Port member check
        VID_GROUP[14] = 1, VLAN ID member check
        IP6_GROUP[12] = 1, IPv6 member check
    */
    pieParam.data0 = 0xD000u;
    pieParam.mask0 = 0xD000u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure member check entry */

    /* Configure the entry 0 of Layer 4 TCP/UDP port member check */
    l4PortMemberInfo.entryIdx = 0u;
    l4PortMemberInfo.type     = CVB_L4PORT_MEMBER_SRC_PORT;
    l4PortMemberInfo.member   = 43707u;
    CHK_FUN_RET(retVal, rtk_acl_l4_port_member_set(unitChip, &l4PortMemberInfo));

    /* Configure the entry 0 of VLAN ID member check */
    vidMemberInfo.entryIdx = 0u;
    vidMemberInfo.type     = CVB_VID_MEMBER_IVID;
    vidMemberInfo.member   = 7u;
    CHK_FUN_RET(retVal, rtk_acl_vid_member_set(unitChip, &vidMemberInfo));

    /* Configure and retrieve the entry 0 of IPv6 member check */
    /* Member: 2000:0000:0000:0000:0000:0000:0000:0002 */
    ip6MemberInfo.entryIdx   = 0u;
    ip6MemberInfo.type       = CVB_IP_MEMBER_SIP;
    ip6MemberInfo.member[0]  = 0x20u;
    ip6MemberInfo.member[1]  = 0x00u;
    ip6MemberInfo.member[2]  = 0x00u;
    ip6MemberInfo.member[3]  = 0x00u;
    ip6MemberInfo.member[4]  = 0x00u;
    ip6MemberInfo.member[5]  = 0x00u;
    ip6MemberInfo.member[6]  = 0x00u;
    ip6MemberInfo.member[7]  = 0x00u;
    ip6MemberInfo.member[8]  = 0x00u;
    ip6MemberInfo.member[9]  = 0x00u;
    ip6MemberInfo.member[10] = 0x00u;
    ip6MemberInfo.member[11] = 0x00u;
    ip6MemberInfo.member[12] = 0x00u;
    ip6MemberInfo.member[13] = 0x00u;
    ip6MemberInfo.member[14] = 0x00u;
    ip6MemberInfo.member[15] = 0x02u;
    CHK_FUN_RET(retVal, rtk_acl_ip6_member_set(unitChip, &ip6MemberInfo));
    ip6MemberInfoGet.entryIdx = 0u;
    CHK_FUN_RET(retVal, rtk_acl_ip6_member_get(unitChip, &ip6MemberInfoGet));

    /* Step 6. Configure each PIE block priority from high to low, with the PIE block index ranging from smallest to largest */
    for(pieBlockId = 0u; pieBlockId <= CVB_ACL_PIE_BLOCK_BOUND; pieBlockId++)
    {
        priorityPieInfo.pieBlock = pieBlockId;
        priorityPieInfo.priVal   = CVB_ACL_PIE_BLOCK_BOUND - pieBlockId;
        CHK_FUN_RET(retVal, rtk_acl_priority_pie_set(unitChip, &priorityPieInfo));
    }

    /* Step 7. Configure VLAN 7 */
    vlanInfo.vid                      = 7u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)CVB_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Expected Behaviour:
    Check if the packet format below can pass through our switch because it belongs to the whitelist
    *************************************************************
    VLAN Tag ID                : 7
    IPv6 Address               : 2000::02
    Protocol                   : TCP or UDP
    Sport                      : 43707
    *************************************************************
    */
    return retVal;
}

RtkApiRet cvb_acl_white_list14(uint32 unit)
{
    /* +---------------------------------------------------------------+ */
    /* |                    Template 0, PIE entry 0                    | */
    /* +--------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0      | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +--------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | RANGE0 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0001   | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0001   | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +--------+----+----+----+----+----+----+----+----+----+----+----+ */

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
    RtkPort                port            = CVB_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclRuleId              index           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    AclIpRangeInfo_t       ipRangeInfo     = {0};
    AclReverseInfo_t       reverseInfo     = {0};
    AclReverseInfo_t       reverseInfoGet  = {0};
    AclPieBlockId          pieBlockId      = 0u;
    AclPriorityPieInfo_t   priorityPieInfo = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVB_IACL_FLD_RANGE1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /*
        IPRC[7:0] = 1, IPv4 range check for 8 regions, use entry 0
    */
    pieParam.data0 = 0x0001u;
    pieParam.mask0 = 0x0001u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure range check entry */

    /* Configure the entry 0 of IPv4 range check */
    ipRangeInfo.entryIdx = 0u;
    ipRangeInfo.type     = CVB_IP_RANGE_SOURCE_IP;
    ipRangeInfo.upper    = 0x22222223u;
    ipRangeInfo.lower    = 0x22222221u;
    CHK_FUN_RET(retVal, rtk_acl_ip_range_set(unitChip, &ipRangeInfo));

    /* Step 6. Reverse the hit result */

    /* Configure and retrieve the ACL result reverse value */
    reverseInfo.aclRuleIndex = index;
    reverseInfo.isReverse    = CVB_ACL_REVERSE;
    CHK_FUN_RET(retVal, rtk_acl_reverse_setting(unitChip, &reverseInfo));
    reverseInfoGet.aclRuleIndex = index;
    CHK_FUN_RET(retVal, rtk_acl_reverse_getting(unitChip, &reverseInfoGet));

    /* Step 7. Configure each PIE block priority from high to low, with the PIE block index ranging from smallest to largest */
    for(pieBlockId = 0u; pieBlockId <= CVB_ACL_PIE_BLOCK_BOUND; pieBlockId++)
    {
        priorityPieInfo.pieBlock = pieBlockId;
        priorityPieInfo.priVal   = CVB_ACL_PIE_BLOCK_BOUND - pieBlockId;
        CHK_FUN_RET(retVal, rtk_acl_priority_pie_set(unitChip, &priorityPieInfo));
    }

    /* Step 8. Configure VLAN 7 */
    vlanInfo.vid                      = 7u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)CVB_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /*
    Check if the source IP range 34.34.34.33 ~ 34.34.34.35 is in the blocklist, and ensure that all packets outside this range can pass through the switch
    */
    return retVal;
}
/**@}*/ /* RTL907CVB_ACL_WHITELIST */
/**@}*/ /* RTL907CVB_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */