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
 * @defgroup RTL907DVA_ACL_EGR_REDIRECT Egress ACL Redirect Action Example
 * @brief This example shows how to set up Egress ACL to control and redirect network traffic through different ports based on defined rules and conditions.
 *
 * @section Purpose
 *   Demonstrates how to configure Egress ACL (EACL) to redirect packets based on specific criteria.
 *
 * @section Description
 *   This example illustrates the process of setting up Egress ACL to redirect network traffic through different ports based on the source IP, protocol, and port number. The key steps involved include: \n
 *   a. Configuring the ACL partition to ACL mode 6 using a GUI tool. \n
 *   b. Enabling ingress/egress ACL lookup for each port and setting the default action to drop packets. \n
 *   c. Setting up ingress/egress ACL templates to define the fields involved in the ACL rules. \n
 *   d. Configuring ingress/egress ACL PIE entries to match specific network traffic patterns. \n
 *   e. Setting up ingress/egress ACL action entries to redirect matched packets to specified ports. \n
 *   f. Verifying the configuration by retrieving and checking the settings. \n
 *   g. Integrating VLAN settings to ensure proper packet forwarding within the specified VLANs. \n
 *   The example provides a comprehensive guide on using EACL to control packet flow in a network, enhancing security and traffic management.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dva_acl_egr_redirect(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_acl_egr_redirect(uint32 unit)
{
    /* +----------------------------------------------------------------------+------------------------------------------------------------------------------+ */
    /* |                                 IACL                                 |                                     EACL                                     | */
    /* |                       Template 2, PIE entry 64                       |                          Template 12, PIE entry 384                          | */
    /* +-------+-------+-------+------+----+----+----+----+----+----+----+----+------+------+-------+---------+---------+------+----+----+----+----+----+----+ */
    /* | 0     | 1     | 2     | 3    | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 0    | 1    | 2     | 3       | 4       | 5    | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +-------+-------+-------+------+----+----+----+----+----+----+----+----+------+------+-------+---------+---------+------+----+----+----+----+----+----+ */
    /* | SMAC0 | SMAC1 | SMAC2 | ITAG | -- | -- | -- | -- | -- | -- | -- | -- | SIP0 | SIP1 | IPHDR | L4SPORT | ACTIDX3 | DPM  | -- | -- | -- | -- | -- | -- | */
    /* +-------+-------+-------+------+----+----+----+----+----+----+----+----+------+------+-------+---------+---------+------+----+----+----+----+----+----+ */
    /* | 4411  | 2233  | 0011  | 02BC | -- | -- | -- | -- | -- | -- | -- | -- | 070B | C0A8 | 0006  | 3458    | 0000    | 0008 | -- | -- | -- | -- | -- | -- | */
    /* +-------+-------+-------+------+----+----+----+----+----+----+----+----+------+------+-------+---------+---------+------+----+----+----+----+----+----+ */
    /* | FFFF  | FFFF  | FFFF  | 0FFF | -- | -- | -- | -- | -- | -- | -- | -- | FFFF | FFFF | 00FF  | FFFF    | 01E0    | 0008 | -- | -- | -- | -- | -- | -- | */
    /* +-------+-------+-------+------+----+----+----+----+----+----+----+----+------+------+-------+---------+---------+------+----+----+----+----+----+----+ */

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
    EaclEnableCtrlInfo_t    eaclEenableCtrlInfoGet = {0};
    RtkPort                 port                   = DVA_RTK_USER_PORT_1ST;
    AclTemplateId           tpIdx                  = 0u;
    AclRuleId               index                  = 0u;
    AclTemplateInfo_t       iaclTemplateInfo       = {0};
    EaclTemplateInfo_t      eaclTemplateInfo       = {0};
    EaclTemplateInfo_t      eaclTemplateInfoGet    = {0};
    AclPieEntryInfo_t       pieEntryInfo           = {0};
    AclTblAccessPieParam_t  pieParam               = {0};
    AclActionEntryInfo_t    iaclActionEntryInfo    = {0};
    AclTblAccessActParam_t  iaclActParam           = {0};
    EaclActionEntryInfo_t   eaclActionEntryInfo    = {0};
    EaclActionEntryInfo_t   eaclActionEntryInfoGet = {0};
    EaclTblAccessActParam_t eaclActParam           = {0};
    EaclTblAccessActParam_t eaclActParamGet        = {0};
    VlanInfo_t              vlanInfo               = {0};

    /* Step 1. Use GUI tool to config ACL partition to ACL mode 6 */
    /*
        ACL Mode 6
        Ingress ACL (IACL):
        # TG 0 – 11
        # Entry 0 - 383
        Egress ACL (EACL):
        # TG 12 – 15
        # Entry 384 – 511
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
        /* Retrieve acl enable control */
        eaclEenableCtrlInfoGet.portNum = port;
        CHK_FUN_RET(retVal, rtk_eacl_enable_ctrl_get(unitChip, &eaclEenableCtrlInfoGet));
    }

    /* Step 4. Configure field type of ingress ACL template 2 */

    /* Template index 2 */
    tpIdx = 2u;

    /* Field 0 */
    iaclTemplateInfo.tpIdx    = tpIdx;
    iaclTemplateInfo.fieldIdx = 0u;
    iaclTemplateInfo.type     = DVA_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &iaclTemplateInfo));

    /* Field 1 */
    iaclTemplateInfo.tpIdx    = tpIdx;
    iaclTemplateInfo.fieldIdx = 1u;
    iaclTemplateInfo.type     = DVA_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &iaclTemplateInfo));

    /* Field 2 */
    iaclTemplateInfo.tpIdx    = tpIdx;
    iaclTemplateInfo.fieldIdx = 2u;
    iaclTemplateInfo.type     = DVA_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &iaclTemplateInfo));

    /* Field 3 */
    iaclTemplateInfo.tpIdx    = tpIdx;
    iaclTemplateInfo.fieldIdx = 3u;
    iaclTemplateInfo.type     = DVA_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &iaclTemplateInfo));

    /* Step 5. Configure field type of egress ACL template 12 */

    /* Template index 12 */
    tpIdx = 12u;

    /* Configure and retrieve field 0 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 0u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_SIP0;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));
    eaclTemplateInfoGet.tpIdx    = tpIdx;
    eaclTemplateInfoGet.fieldIdx = 0u;
    CHK_FUN_RET(retVal, rtk_eacl_template_getting(unitChip, &eaclTemplateInfoGet));

    /* Configure and retrieve field 1 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 1u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_SIP1;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));
    eaclTemplateInfoGet.tpIdx    = tpIdx;
    eaclTemplateInfoGet.fieldIdx = 1u;
    CHK_FUN_RET(retVal, rtk_eacl_template_getting(unitChip, &eaclTemplateInfoGet));

    /* Configure and retrieve field 2 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 2u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_IPHDR;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));
    eaclTemplateInfoGet.tpIdx    = tpIdx;
    eaclTemplateInfoGet.fieldIdx = 2u;
    CHK_FUN_RET(retVal, rtk_eacl_template_getting(unitChip, &eaclTemplateInfoGet));

    /* Configure and retrieve field 3 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 3u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_L4SPORT;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));
    eaclTemplateInfoGet.tpIdx    = tpIdx;
    eaclTemplateInfoGet.fieldIdx = 3u;
    CHK_FUN_RET(retVal, rtk_eacl_template_getting(unitChip, &eaclTemplateInfoGet));

    /* Configure and retrieve field 4 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 4u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_PPI_ACTIDX3;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));
    eaclTemplateInfoGet.tpIdx    = tpIdx;
    eaclTemplateInfoGet.fieldIdx = 4u;
    CHK_FUN_RET(retVal, rtk_eacl_template_getting(unitChip, &eaclTemplateInfoGet));

    /* Configure and retrieve field 5 */
    eaclTemplateInfo.tpIdx    = tpIdx;
    eaclTemplateInfo.fieldIdx = 5u;
    eaclTemplateInfo.type     = DVA_EACL_FLD_PPI_DPM;
    CHK_FUN_RET(retVal, rtk_eacl_template_setting(unitChip, &eaclTemplateInfo));
    eaclTemplateInfoGet.tpIdx    = tpIdx;
    eaclTemplateInfoGet.fieldIdx = 5u;
    CHK_FUN_RET(retVal, rtk_eacl_template_getting(unitChip, &eaclTemplateInfoGet));

    /* Step 6. Configure ingress ACL PIE entry 64 */

    /* entry 64 */
    index = 64u;

    /* SMAC[15:0] = 44:11 */
    pieParam.data0 = 0x4411u;
    pieParam.mask0 = 0xFFFFu;

    /* SMAC[31:16] = 22:33 */
    pieParam.data1 = 0x2233u;
    pieParam.mask1 = 0xFFFFu;

    /* SMAC[47:32] = 00:11 */
    pieParam.data2 = 0x0011u;
    pieParam.mask2 = 0xFFFFu;

    /* IVID = 700 */
    pieParam.data3 = 0x02BCu;
    pieParam.mask3 = 0x0FFFu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 7. Configure egress ACL PIE entry 384 */

    /* entry 384 */
    index = 384u;

    /* SIP[15:0] = 7.11 */
    pieParam.data0 = 0x070Bu;
    pieParam.mask0 = 0xFFFFu;

    /* SIP[31:16] = 192.168 */
    pieParam.data1 = 0xC0A8u;
    pieParam.mask1 = 0xFFFFu;

    /* PROTOCOL[7:0] = 6 (6 is TCP) */
    pieParam.data2 = 0x0006u;
    pieParam.mask2 = 0x00FFu;

    /* SPORT = 13400 */
    pieParam.data3 = 0x3458u;
    pieParam.mask3 = 0xFFFFu;

    /* ACTIDX: Hit PIE 4 entry 64 (ACTIDX4, index 0) */
    pieParam.data4 = 0x0000u;
    pieParam.mask4 = 0x01E0u;

    /* Destination port mask = 0x8 (port 3) */
    pieParam.data5 = 0x0008u;
    pieParam.mask5 = 0x0008u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 384 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 8. Configure ingress ACL action entry 64 */

    /* entry 64 */
    index = 64u;

    /*
        redir = 1,
        opcode = 0 (normal forward),
        dpm = DVA_RTK_USER_PMAP_3RD,
        crsvlan = 0 (forward in VLAN)
    */
    iaclActParam.redir             = 1u;
    iaclActParam.redirInfo.opcode  = 0u;
    iaclActParam.redirInfo.dpm     = DVA_RTK_USER_PMAP_3RD;
    iaclActParam.redirInfo.crsvlan = 0u;

    /* Configure action entry 64 */
    iaclActionEntryInfo.aclRuleIndex = index;
    iaclActionEntryInfo.pActParam    = &iaclActParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &iaclActionEntryInfo));

    /* Step 9. Configure egress ACL action entry 384 */

    /* entry 384 */
    index = 384u;

    /*
        redir = 1,
        opcode = 2 (recovery dpm & operation selection),
        dpm = DVA_RTK_USER_PMAP_2ND,
        recDpm = 1 (DPM from PPI2 (1, IACL output)),
        actOpSel = 3 (output ingress DPM | egress DPM)
    */
    eaclActParam.redir                 = 1u;
    eaclActParam.egrRedirInfo.opcode   = 2u;
    eaclActParam.egrRedirInfo.dpm      = DVA_RTK_USER_PMAP_2ND;
    eaclActParam.egrRedirInfo.recDpm   = 1u;
    eaclActParam.egrRedirInfo.actOpSel = 3u;

    /* Configure action entry 384 */
    eaclActionEntryInfo.aclRuleIndex = index;
    eaclActionEntryInfo.pEactParam   = &eaclActParam;
    CHK_FUN_RET(retVal, rtk_eacl_setting_action_entry(unitChip, &eaclActionEntryInfo));
    eaclActionEntryInfoGet.aclRuleIndex = index;
    eaclActionEntryInfoGet.pEactParam   = &eaclActParamGet;
    CHK_FUN_RET(retVal, rtk_eacl_getting_action_entry(unitChip, &eaclActionEntryInfoGet));

    /* Step 10. Configure VLAN 700 */
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
        a. Configure ACL entry 64 in Template 2 and entry 384 in Template 12.
        b. All packets that match the source MAC address and inner VLAN ID specified in ingress ACL entry 64 will be processed by the ACL and forwarded to PORT_3RD.
        c. TCP packets that successfully match the criteria in ingress ACL entry 64 and also meet the conditions specified in egress ACL entry 384 (source IPv4 address and Layer 4 port) will hit the egress ACL.
        d. The packets that hit the egress ACL, the destination port mask (DPM) will be recovered, and the original DPM will be combined with PORT_2ND using the "OR" operation.
        e. VLAN settings are integrated to ensure proper packet forwarding within VLAN 700.
        f. Any packet that does not match any of the specified ACL entries will be dropped.
    */
    return retVal;
}
/**@}*/ /* RTL907DVA_ACL_EGR_REDIRECT */
/**@}*/ /* RTL907DVA_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */