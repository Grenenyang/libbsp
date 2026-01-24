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
 * @addtogroup RTL907DV1_ACL_EXAMPLE RTL907XD_V1 ACL Example
 * @{
 */
/**
 * @addtogroup RTL907DV1_ACL_IVID_RMK ACL Inner VLAN Tag Remarking Example
 * @brief This example shows how to use ACL to dynamically modify the inner VLAN ID of incoming packets.
 *
 * @section Purpose
 *   This example demonstrates the capability to modify the inner VLAN ID of packets dynamically using ACL.
 *
 * @section Description
 *   This example configures an ACL to remark the inner VLAN ID for packets: \n
 *   a. Configures ACL entry 64 in Template2 using template field 0. \n
 *   b. All packets received from physical ports PORT_1ST will be remarked with the new VLAN inner tag ID of 0xAAA. \n
 *   c. Lookup miss in ACL function is permitted to keep the process by L2 and VLAN function.
 *  @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dv1_acl_ivid_rmk(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dv1_acl_ivid_rmk(uint32 unit)
{
    /* +-----------------------------------------------------------------+ */
    /* |                     Template 2, PIE entry 64                    | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0        | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | ORI_PORT | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0000     | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 1FFC     | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+----+----+----+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_V1,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = DV1_RTK_USER_PORT_1ST,
        .aclEn   = DV1_ACL_ENABLE,
        .defAct  = DV1_ACL_DEF_ACT_PERMIT,
    };
    RtkPort                port            = DV1_RTK_USER_PORT_1ST;
    AclRuleId              index           = 64u;
    AclTemplateId          tpIdx           = index / DV1_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};
    VlanInfo_t             vlanInfo        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to permit packets */
    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 2 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = DV1_IACL_FLD_ORI_PORT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 64 */

    /* SPMASK: DV1_RTK_USER_PMAP_ALL & ~(1u << dv1_g_aclUserPort2sysPort[DV1_RTK_USER_PORT_1ST]) */
    pieParam.data0 = 0x0000u;
    pieParam.mask0 = 0x1FFCu;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 64 */

    /*
        itag = 1,
        inVidCtl = 1 (provide new inner VLAN ID as inVidInfo)
        inVidInfo = 0xAAA (new inner VLAN ID value)
        inTagOp = 1 (packet sent with modified inner VLAN tag)
    */
    actParam.itag                  = 1u;
    actParam.inTagOpInfo.inVidCtl  = 1u;
    actParam.inTagOpInfo.inVidInfo = 0xAAAu;
    actParam.inTagOpInfo.inTagOp   = 1u;

    /* Configure action entry 64 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 7. Configure VLAN 0xAAA */
    vlanInfo.vid                      = 0xAAAu;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)DV1_RTK_USER_PMAP_ALL;
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Expected Behaviour:
        a. Configure ACL entry 64 in Template2 and use template field 0.
        b. All packets received from physical PORT_1ST will be remarked with the new VLAN inner tag ID of 0xAAA and keep the VLAN tag format.
        c. Lookup miss in ACL function is permitted to keep the process by L2 and VLAN function.
    */
    return retVal;
}
/**@}*/ /* RTL907DV1_ACL_IVID_RMK */
/**@}*/ /* RTL907DV1_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */