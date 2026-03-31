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
 * @addtogroup RTL907DVA_ACL_RUNTIME_WHITELIST ACL Runtime Setting Whitelist Example
 * @brief This example demonstrates how to configure ACL to establish a whitelist, allowing only certain types of network traffic and blocking all other traffic during runtime.
 *
 * @section Purpose
 *   This example shows how to set up ACL to form a whitelist, ensuring that only designated network traffic is permitted during runtime.
 *
 * @section Description
 *   This example offers a comprehensive guide on configuring ACL to establish a whitelist,
 *   permitting only specified network traffic while blocking all others:
 *   a. Configure ACL templates and entries.
 *   b. Set up actions for permitted traffic.
 *   c. Illustrate packet flow through the configured ACL.
 * @note Please note:
 *   a. Ensure that `aclRuleIndex` in `AclPieEntryInfo_t` is unique to avoid duplication.
 *   b. Adding entries in batches of 128 will trigger data preprocessing, requiring a brief pause. This is a standard procedure, so rest assured that packet transmission will remain uninterrupted, although this example does not exhibit this behavior.
 *   c. After all entries are added, execute `rtk_acl_runtime_trigger_pie_entries` to load them into the switch.
 *   d. During the initial execution of `rtk_acl_runtime_trigger_pie_entries`, expect a short wait for data preprocessing. Following this, the Switch's transmission will briefly pause for no more than 2ms. Once complete, the ACL's PIE will be activated, allowing packet transmission to resume without disruption.
 *   @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dva_acl_runtime_white_list(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_acl_runtime_white_list(uint32 unit)
{
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                             Template 0                                            | */
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                             PIE entry 0                                           | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 0    | 1     | 2     | 3     | 4    | 5    | 6    | 7     | 8       | 9       | 10      | 11      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | FMT  | SMAC0 | SMAC1 | SMAC2 | ITAG | SIP0 | SIP1 | IPHDR | L4SPORT | OFFSET0 | OFFSET1 | OFFSET2 | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 04A0 | 4411  | 2233  | 0011  | 02BC | 070B | C0A8 | 0006  | 3458    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                             PIE entry 1                                           | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 04A0 | 4412  | 2233  | 0011  | 0064 | 010C | C0A8 | 0006  | 3039    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                             PIE entry 2                                           | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 04A0 | 4412  | 2233  | 0011  | 02BC | 070C | C0A8 | 0006  | 3458    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* +---------------------------------------------------------------------------------------------------+ */
    /* |                                             PIE entry 3                                           | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 04A0 | 4423  | 2233  | 0011  | 0064 | 0117 | C0A8 | 0006  | 3039    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */
    /* | 3CBE | FFFF  | FFFF  | FFFF  | 0FFF | FFFF | FFFF | 00FF  | FFFF    | --      | --      | --      | */
    /* +------+-------+-------+-------+------+------+------+-------+---------+---------+---------+---------+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = DVA_RTK_USER_PORT_1ST,
        .aclEn   = DVA_ACL_ENABLE,
        .defAct  = DVA_ACL_DEF_ACT_DROP,
    };
    RtkPort                port            = DVA_RTK_USER_PORT_1ST;
    AclTemplateId          tpIdx           = 0u;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = DVA_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = DVA_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 2 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 2u;
    templateInfo.type     = DVA_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 3 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 3u;
    templateInfo.type     = DVA_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 4 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 4u;
    templateInfo.type     = DVA_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 5 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 5u;
    templateInfo.type     = DVA_IACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 6 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 6u;
    templateInfo.type     = DVA_IACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 7 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 7u;
    templateInfo.type     = DVA_IACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 8 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 8u;
    templateInfo.type     = DVA_IACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 9 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 9u;
    templateInfo.type     = DVA_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 10 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 10u;
    templateInfo.type     = DVA_IACL_FLD_OFFSET_1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 11 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 11u;
    templateInfo.type     = DVA_IACL_FLD_OFFSET_2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3.1 Configure PIE entry 0 */

    CHK_FUN_RET(retVal, drv_memset(&pieEntryInfo, 0, sizeof(pieEntryInfo)));
    CHK_FUN_RET(retVal, drv_memset(&pieParam, 0, sizeof(pieParam)));

    /*
        SPHY[13:10]  = DVA_RTK_USER_PORT_1ST (ingress port number = DVA_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)dva_g_aclUserPort2sysPort[DVA_RTK_USER_PORT_1ST] << 10u));
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
    pieEntryInfo.aclRuleIndex = 0u;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_runtime_add_pie_entry(unitChip, &pieEntryInfo));

    /* Step 3.2 Configure PIE entry 1 */

    /*
        SPHY[13:10]  = DVA_RTK_USER_PORT_1ST (ingress port number = DVA_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */

    CHK_FUN_RET(retVal, drv_memset(&pieEntryInfo, 0, sizeof(pieEntryInfo)));
    CHK_FUN_RET(retVal, drv_memset(&pieParam, 0, sizeof(pieParam)));

    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)dva_g_aclUserPort2sysPort[DVA_RTK_USER_PORT_1ST] << 10u));
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

    /* Configure PIE entry 1 */
    pieEntryInfo.aclRuleIndex = 1u;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_runtime_add_pie_entry(unitChip, &pieEntryInfo));

    /* Step 3.3 Configure PIE entry 2 */

    CHK_FUN_RET(retVal, drv_memset(&pieEntryInfo, 0, sizeof(pieEntryInfo)));
    CHK_FUN_RET(retVal, drv_memset(&pieParam, 0, sizeof(pieParam)));

    /*
        SPHY[13:10]  = DVA_RTK_USER_PORT_1ST (ingress port number = DVA_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)dva_g_aclUserPort2sysPort[DVA_RTK_USER_PORT_1ST] << 10u));
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

    /* Configure PIE entry 2 */
    pieEntryInfo.aclRuleIndex = 2u;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_runtime_add_pie_entry(unitChip, &pieEntryInfo));

    /* Step 3.4 Configure PIE entry 3 */

    CHK_FUN_RET(retVal, drv_memset(&pieEntryInfo, 0, sizeof(pieEntryInfo)));
    CHK_FUN_RET(retVal, drv_memset(&pieParam, 0, sizeof(pieParam)));

    /*
        SPHY[13:10]  = DVA_RTK_USER_PORT_1ST (ingress port number = DVA_RTK_USER_PORT_1ST)
        ITAGEXIST[7] = 1
        L3FMT[5:4]   = 2 (IPv4)
        L4FMT[3:1]   = 0 (TCP)
    */
    pieParam.data0 = (uint16)((0u << 1u) | (2u << 4u) | (1u << 7u) | ((uint16)dva_g_aclUserPort2sysPort[DVA_RTK_USER_PORT_1ST] << 10u));
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

    /* Configure PIE entry 3 */
    pieEntryInfo.aclRuleIndex = 3u;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_runtime_add_pie_entry(unitChip, &pieEntryInfo));

    /* Step 3.5 Trigger PIE entries */

    /* Load PIE entries into the switch */
    CHK_FUN_RET(retVal, rtk_acl_runtime_trigger_pie_entries(unitChip));

    /* Step 4 Configure action entry 0 - 3 */

    /* Configure permit action */
    actParam.drop = 0u;

    /* Configure action entry 0 - 3 */
    actionEntryInfo.pActParam    = &actParam;
    actionEntryInfo.aclRuleIndex = 0u;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));
    actionEntryInfo.aclRuleIndex = 1u;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));
    actionEntryInfo.aclRuleIndex = 2u;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));
    actionEntryInfo.aclRuleIndex = 3u;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5.1 Configure VLAN 700 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 700u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 5.2 Configure VLAN 100 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
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
    Switch Physical Port ID    : DVA_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:11
    VLAN Tag ID                : 700
    IP Address                 : 192.168.7.11
    Protocol                   : TCP
    Sport                      : 13400
    *************************************************************
    Switch Physical Port ID    : DVA_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:12
    VLAN Tag ID                : 100
    IP Address                 : 192.168.1.12
    Protocol                   : TCP
    Sport                      : 12345
    *************************************************************
    Switch Physical Port ID    : DVA_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:12
    VLAN Tag ID                : 700
    IP Address                 : 192.168.7.12
    Protocol                   : TCP
    Sport                      : 13400
    *************************************************************
    Switch Physical Port ID    : DVA_RTK_USER_PORT_1ST
    MAC Address                : 00:11:22:33:44:23
    VLAN Tag ID                : 100
    IP Address                 : 192.168.1.23
    Protocol                   : TCP
    Sport                      : 12345
    *************************************************************
    */
    return retVal;
}
