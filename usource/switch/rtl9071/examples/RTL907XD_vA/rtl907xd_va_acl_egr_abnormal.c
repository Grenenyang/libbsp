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
 * @addtogroup RTL907DVA_ACL_EGR_ABNORMAL Egress ACL Abnormal Example
 * @brief This example showcases the setup and utilization of Egress ACL to effectively handle and mitigate abnormal traffic patterns in a network environment.
 *
 * @section Purpose
 * This example demonstrates how to configure and manage Egress ACL to handle abnormal conditions in a network environment.
 *
 * @section Description
 * This example provides a detailed step-by-step procedure to set up EACL for abnormal condition handling. The key operations include: \n
 * a. Configuring ACL partitions to specific modes for both ingress and egress ACL. \n
 * b. Enabling ACL lookup and setting default actions for both ingress and egress directions. \n
 * c. Setting up ACL templates for specific field types that are relevant to abnormal condition detection. \n
 * d. Configuring PIE entries to define rules for traffic matching. \n
 * e. Setting action entries to define the actions to be taken when a rule is matched. \n
 * f. Configuring range check and member check entries to validate IP and MAC addresses against specified ranges and membership. \n
 * g. Setting up policer entries to enforce rate limits and monitor traffic for abnormal patterns. \n
 * h. Configuring abnormal times entries to detect and act upon frequent rule matches that suggest potential network issues. \n
 * i. Enabling and configuring time slot functions to manage and monitor ACL activities within specified time intervals. \n
 * j. Setting up abnormal destination port masks to isolate traffic to specific ports when abnormal conditions are detected. \n
 * k. Configuring VLAN settings to segregate and manage network traffic efficiently.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dva_acl_egr_abnormal(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_acl_egr_abnormal(uint32 unit)
{
    /* +-----------------------------------------------------------------------------+-----------------------------------------------------------------------------------------+ */
    /* |                                     IACL                                    |                                           EACL                                          | */
    /* |                           Template 2, PIE entry 64                          |                                Template 14, PIE entry 448                               | */
    /* +----------+----------------+----+----+----+----+----+----+----+----+----+----+----------+----------------------+--------+------+----+----+----+----+----+----+----+----+ */
    /* | 0        | 1              | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 0        | 1                    | 2      | 3    | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +----------+----------------+----+----+----+----+----+----+----+----+----+----+----------+----------------------+--------+------+----+----+----+----+----+----+----+----+ */
    /* | SPHY_ORG | RANGCHK_MAC_RC | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | SPHY_ORG | RANGCHK_MAC_GROUP_RC | LKMISS | DPM  | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----------------+----+----+----+----+----+----+----+----+----+----+----------+----------------------+--------+------+----+----+----+----+----+----+----+----+ */
    /* | 0001     | 0001           | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | 1000     | 0001                 | 0001   | 0008 | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----------------+----+----+----+----+----+----+----+----+----+----+----------+----------------------+--------+------+----+----+----+----+----+----+----+----+ */
    /* | 0001     | 0001           | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | 1000     | 0001                 | 0001   | 0008 | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----------------+----+----+----+----+----+----+----+----+----+----+----------+----------------------+--------+------+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA,
    };
    AclEnableCtrlInfo_t iaclEnableCtrlInfo = {
        .portNum = DVA_RTK_USER_PORT_1ST,
        .aclEn   = DVA_ACL_ENABLE,
        .defAct  = DVA_ACL_DEF_ACT_DROP,
    };
    EaclEnableCtrlInfo_t eaclEnableCtrlInfo = {
        .portNum = DVA_RTK_USER_PORT_1ST,
        .aclEn   = DVA_ACL_ENABLE,
        .defAct  = DVA_ACL_DEF_ACT_DROP,
    };

    RtkPort                          port                         = DVA_RTK_USER_PORT_1ST;
    AclTemplateId                    tpIdx                        = 0u;
    AclRuleId                        index                        = 0u;
    AclTemplateInfo_t                iaclTemplateInfo             = {0};
    EaclTemplateInfo_t               eaclTemplateInfo             = {0};
    AclPieEntryInfo_t                pieEntryInfo                 = {0};
    AclTblAccessPieParam_t           pieParam                     = {0};
    AclActionEntryInfo_t             iaclActionEntryInfo          = {0};
    AclTblAccessActParam_t           iaclActParam                 = {0};
    EaclActionEntryInfo_t            eaclActionEntryInfo          = {0};
    EaclTblAccessActParam_t          eaclActParam                 = {0};
    AclIp6RangeInfo_t                ip6RangeInfo                 = {0};
    AclMacRangeInfo_t                macRangeInfo                 = {0};
    AclIp6MemberInfo_t               ip6MemberInfo                = {0};
    AclMacMemberInfo_t               macMemberInfo                = {0};
    AclPolicerEntryInfo_t            policerEntryInfo             = {0};
    AclTblAccessPolicerParam_t       policerParam                 = {0};
    AclAbnormalTimesEntryInfo_t      abnormalTimesEntryInfo       = {0};
    AclTblAccessAbnormalTimesParam_t abnormalTimesParam           = {0};
    EaclTimeSlotEnInfo_t             eaclTimeSlotEnInfo           = {0};
    EaclTimeSlotEnInfo_t             eaclTimeSlotEnInfoGet        = {0};
    EaclTimeSlotThresholdInfo_t      eaclTimeSlotThresholdInfo    = {0};
    EaclTimeSlotThresholdInfo_t      eaclTimeSlotThresholdInfoGet = {0};
    EaclAbnormalDpmInfo_t            eaclAbnormalDpmInfo          = {0};
    EaclAbnormalDpmInfo_t            eaclAbnormalDpmInfoGet       = {0};
    VlanInfo_t                       vlanInfo                     = {0};

    /* Step 1. Use GUI tool to config ACL partition to ACL mode 7 */
    /*
        ACL Mode 7
        Ingress ACL (IACL):
        # TG 0 – 13
        # Entry 0 - 447
        Egress ACL (EACL):
        # TG 14 – 15
        # Entry 448 – 511
    */

    /* Step 2. Enable ingress ACL lookup for each port and configure the default action to drop packets */
    for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
    {
        /* Configure ingress ACL enable control */
        iaclEnableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &iaclEnableCtrlInfo));
    }

    /* Step 3. Enable egress ACL lookup for each port and configure the default action to drop packets */
    for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
    {
        /* Configure egress ACL enable control */
        eaclEnableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_eacl_enable_ctrl_set(unitChip, &eaclEnableCtrlInfo));
    }

    /* Step 4. Configure field type of ingress ACL template 2 */

    /* Template index 2 */
    tpIdx = 2u;

    /* Field 0 */
    iaclTemplateInfo.tpIdx    = tpIdx;
    iaclTemplateInfo.fieldIdx = 0u;
    iaclTemplateInfo.type     = DVA_IACL_FLD_SPHY_ORG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &iaclTemplateInfo));

    /* Field 1 */
    iaclTemplateInfo.tpIdx    = tpIdx;
    iaclTemplateInfo.fieldIdx = 1u;
    iaclTemplateInfo.type     = DVA_IACL_FLD_RANGCHK_MAC_RC;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &iaclTemplateInfo));

    /* Step 5. Configure field type of egress ACL template 14 */

    /* Template index 14 */
    tpIdx = 14u;

    /* Field 0 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 0u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_SPHY_ORG;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));

    /* Field 1 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 1u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_RANGCHK_MAC_GROUP_RC;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));

    /* Field 2 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 2u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_PPI_LKMISS;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));

    /* Field 3 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 3u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_PPI_DPM;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));

    /* Step 6. Configure ingress ACL PIE entry 64 */

    /* entry 64 */
    index = 64u;

    /*
        IP6RC[7:0] = 1, IPv6 range check for 8 regions, use entry 0
    */
    pieParam.data0 = 0x0001u;
    pieParam.mask0 = 0x0001u;

    /*
        MACRC[15:0] = 1, MAC range check for 16 region, use entry 0
    */
    pieParam.data1 = 0x0001u;
    pieParam.mask1 = 0x0001u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 7. Configure egress ACL PIE entry 448 */

    /* entry 448 */
    index = 448u;

    /*
        IP6_GROUP[12] = 1, IPv6 member check
    */
    pieParam.data0 = 0x1000u;
    pieParam.mask0 = 0x1000u;

    /*
        MAC_GROUP[0] = 1, MAC member check
    */
    pieParam.data1 = 0x0001u;
    pieParam.mask1 = 0x0001u;

    /* L2 Table Destination MAC lookup miss */
    pieParam.data2 = 0x0001u;
    pieParam.mask2 = 0x0001u;

    /* Destination port mask = 0x8 (port 3) */
    pieParam.data3 = 0x0008u;
    pieParam.mask3 = 0x0008u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 448 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 8. Configure ingress ACL action entry 64 */

    /* entry 64 */
    index = 64u;

    /* Configure permit action */
    iaclActParam.drop = 0u;

    /* Configure action entry 0 */
    iaclActionEntryInfo.aclRuleIndex = index;
    iaclActionEntryInfo.pActParam    = &iaclActParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &iaclActionEntryInfo));

    /* Step 9. Configure egress ACL action entry 448 */

    /* entry 448 */
    index = 448u;

    /* Configure permit action */
    eaclActParam.drop = 0u;

    /* Configure action entry 448 */
    eaclActionEntryInfo.aclRuleIndex = index;
    eaclActionEntryInfo.pEactParam   = &eaclActParam;
    CHK_FUN_RET(retVal, rtk_eacl_setting_action_entry(unitChip, &eaclActionEntryInfo));

    /* Step 10. Configure range check entry */

    /* Configure the entry 0 of IPv6 range check */
    /*
        Upper: FD53:7CB8:383:8::125
        Lower: FD53:7CB8:383:8::001
    */
    ip6RangeInfo.entryIdx  = 0u;
    ip6RangeInfo.type      = DVA_IP_RANGE_SOURCE_IP;
    ip6RangeInfo.upper[0]  = 0xFDu;
    ip6RangeInfo.upper[1]  = 0x53u;
    ip6RangeInfo.upper[2]  = 0x7Cu;
    ip6RangeInfo.upper[3]  = 0xB8u;
    ip6RangeInfo.upper[4]  = 0x03u;
    ip6RangeInfo.upper[5]  = 0x83u;
    ip6RangeInfo.upper[6]  = 0x00u;
    ip6RangeInfo.upper[7]  = 0x08u;
    ip6RangeInfo.upper[8]  = 0x00u;
    ip6RangeInfo.upper[9]  = 0x00u;
    ip6RangeInfo.upper[10] = 0x00u;
    ip6RangeInfo.upper[11] = 0x00u;
    ip6RangeInfo.upper[12] = 0x00u;
    ip6RangeInfo.upper[13] = 0x00u;
    ip6RangeInfo.upper[14] = 0x01u;
    ip6RangeInfo.upper[15] = 0x25u;

    ip6RangeInfo.lower[0]  = 0xFDu;
    ip6RangeInfo.lower[1]  = 0x53u;
    ip6RangeInfo.lower[2]  = 0x7Cu;
    ip6RangeInfo.lower[3]  = 0xB8u;
    ip6RangeInfo.lower[4]  = 0x03u;
    ip6RangeInfo.lower[5]  = 0x83u;
    ip6RangeInfo.lower[6]  = 0x00u;
    ip6RangeInfo.lower[7]  = 0x08u;
    ip6RangeInfo.lower[8]  = 0x00u;
    ip6RangeInfo.lower[9]  = 0x00u;
    ip6RangeInfo.lower[10] = 0x00u;
    ip6RangeInfo.lower[11] = 0x00u;
    ip6RangeInfo.lower[12] = 0x00u;
    ip6RangeInfo.lower[13] = 0x00u;
    ip6RangeInfo.lower[14] = 0x00u;
    ip6RangeInfo.lower[15] = 0x01u;
    CHK_FUN_RET(retVal, rtk_acl_ip6_range_set(unitChip, &ip6RangeInfo));

    /* Configure the entry 0 of MAC range check */
    /*
        Upper: 00:11:22:AA:BB:EE
        Lower: 00:11:22:AA:BB:CC
    */
    macRangeInfo.entryIdx = 0u;
    macRangeInfo.type     = DVA_MAC_RANGE_SOURCE_MAC;
    macRangeInfo.upper[0] = 0x00;
    macRangeInfo.upper[1] = 0x11;
    macRangeInfo.upper[2] = 0x22;
    macRangeInfo.upper[3] = 0xAA;
    macRangeInfo.upper[4] = 0xBB;
    macRangeInfo.upper[5] = 0xEE;

    macRangeInfo.lower[0] = 0x00;
    macRangeInfo.lower[1] = 0x11;
    macRangeInfo.lower[2] = 0x22;
    macRangeInfo.lower[3] = 0xAA;
    macRangeInfo.lower[4] = 0xBB;
    macRangeInfo.lower[5] = 0xCC;
    CHK_FUN_RET(retVal, rtk_acl_mac_range_set(unitChip, &macRangeInfo));

    /* Step 11. Configure member check entry */

    /* Configure the entry 0 of IPv6 member check */
    /* Member: FD53:7CB8:383:8::DD */
    ip6MemberInfo.entryIdx   = 0u;
    ip6MemberInfo.type       = DVA_IP_MEMBER_SIP;
    ip6MemberInfo.member[0]  = 0xFDu;
    ip6MemberInfo.member[1]  = 0x53u;
    ip6MemberInfo.member[2]  = 0x7Cu;
    ip6MemberInfo.member[3]  = 0xB8u;
    ip6MemberInfo.member[4]  = 0x03u;
    ip6MemberInfo.member[5]  = 0x83u;
    ip6MemberInfo.member[6]  = 0x00u;
    ip6MemberInfo.member[7]  = 0x08u;
    ip6MemberInfo.member[8]  = 0x00u;
    ip6MemberInfo.member[9]  = 0x00u;
    ip6MemberInfo.member[10] = 0x00u;
    ip6MemberInfo.member[11] = 0x00u;
    ip6MemberInfo.member[12] = 0x00u;
    ip6MemberInfo.member[13] = 0x00u;
    ip6MemberInfo.member[14] = 0x00u;
    ip6MemberInfo.member[15] = 0xDDu;
    CHK_FUN_RET(retVal, rtk_acl_ip6_member_set(unitChip, &ip6MemberInfo));

    /* Configure and retrieve the entry 0 of MAC member check */
    /* Member: 00:11:22:AA:BB:DD */
    macMemberInfo.entryIdx  = 0u;
    macMemberInfo.type      = DVA_MAC_MEMBER_SMAC;
    macMemberInfo.member[0] = 0x00u;
    macMemberInfo.member[1] = 0x11u;
    macMemberInfo.member[2] = 0x22u;
    macMemberInfo.member[3] = 0xAAu;
    macMemberInfo.member[4] = 0xBBu;
    macMemberInfo.member[5] = 0xDDu;
    CHK_FUN_RET(retVal, rtk_acl_mac_member_set(unitChip, &macMemberInfo));

    /* Step 12. Configure policer entry 448 */

    /* entry 448 */
    index = 448u;

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

    /* Configure policer entry 448 */
    policerEntryInfo.aclRuleIndex  = index;
    policerEntryInfo.pPolicerParam = &policerParam;
    CHK_FUN_RET(retVal, rtk_acl_policer_entry_set(unitChip, &policerEntryInfo));

    /* Step 13. Configure abnormal times entry 448 */

    /* entry 448 */
    index = 448u;

    /*
        enable = 1 (enable abnormal times)
        abnormalThreshold = 0xFFFF (abnormal counter threshold)
    */
    abnormalTimesParam.enable    = 1u;
    abnormalTimesParam.threshold = 0xFFFFu;

    /* Configure and retrieve abnormal times entry 448 */
    abnormalTimesEntryInfo.aclRuleIndex        = index;
    abnormalTimesEntryInfo.pAbnormalTimesParam = &abnormalTimesParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_abnormal_times_entry(unitChip, &abnormalTimesEntryInfo));

    /* Step 14. Configure time slot function */

    /* Enable the time slot function and retrieve the time slot enable status */
    eaclTimeSlotEnInfo.enable = 1u;
    CHK_FUN_RET(retVal, rtk_eacl_time_slot_en_set(unitChip, &eaclTimeSlotEnInfo));
    CHK_FUN_RET(retVal, rtk_eacl_time_slot_en_get(unitChip, &eaclTimeSlotEnInfoGet));

    /* Configure and retrieve the time slot threshold (the interval of time slot) */
    eaclTimeSlotThresholdInfo.threshold = 0x1u;
    CHK_FUN_RET(retVal, rtk_eacl_time_slot_threshold_set(unitChip, &eaclTimeSlotThresholdInfo));
    CHK_FUN_RET(retVal, rtk_eacl_time_slot_threshold_get(unitChip, &eaclTimeSlotThresholdInfoGet));

    /* Step 15. Configure abnormal dpm */

    /* Configure and retrieve the abnormal destination port mask */
    eaclAbnormalDpmInfo.dpm = DVA_RTK_USER_PMAP_4TH;
    CHK_FUN_RET(retVal, rtk_eacl_abnormal_dpm_set(unitChip, &eaclAbnormalDpmInfo));
    CHK_FUN_RET(retVal, rtk_eacl_abnormal_dpm_get(unitChip, &eaclAbnormalDpmInfoGet));

    /* Step 16. Configure VLAN 700 */
    vlanInfo.vid                      = 700u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Expected Behavior:
        a. Configure ACL entry 64 in Template 2 and entry 448 in Template 14.
        b. All packets that match the IPv6 range within FD53:7CB8:383:8::125 ~ FD53:7CB8:383:8::1 and MAC range within 00:11:22:AA:BB:EE ~ 00:11:22:AA:BB:CC in ingress ACL entry 64 will be permitted.
        c. The packets received by PORT_3RD that successfully match the specific IPv6 address (FD53:7CB8:383:8::DD) and MAC address (00:11:22:AA:BB:DD) and its DMAC not in the L2 table will hit the egress ACL.
        d. The packets that hit the egress ACL will have their forward rate limited to 80000 Kbps.
        e. The abnormal times function will be enabled, and occurrences over the threshold of 0xFFFF will trigger an interrupt.
        f. The time slot function will be enabled, and the time slot threshold (interval) will be set to 1ms.
        g. When abnormal times occur, the frame will be forwarded to PORT_4TH (abnormal DPM).
        h. VLAN settings are configured to ensure proper packet forwarding within VLAN 700.
        i. Any packet that does not match any of the specified ACL entries will be dropped.
    */
    return retVal;
}
/**@}*/ /* RTL907DVA_ACL_EGR_ABNORMAL */
/**@}*/ /* RTL907DVA_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */