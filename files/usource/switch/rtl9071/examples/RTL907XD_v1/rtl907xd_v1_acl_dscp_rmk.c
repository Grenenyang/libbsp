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
 * @addtogroup RTL907DV1_ACL_EXAMPLE RTL907XD_V1 ACL Example
 * @{
 */
/**
 * @addtogroup RTL907DV1_ACL_DSCP_RMK ACL DSCP Remarking and Any Offset Setting Example
 * @brief This example showcases how to configure an ACL entry to modify the DSCP field of packets under certain criteria.
 *
 * @section Purpose
 *   This example demonstrates the capability of the ACL to remark the DSCP field based on specific conditions.
 *
 * @section Description
 *   This example configures an ACL entry to remark the DSCP field of packets that meet certain criteria: \n
 *   a. The packet payload address is between 100 and 101. \n
 *   b. The payload value at the specified address is 0x8899. \n
 *   c. The DSCP field is remarked to 32. \n
 *   d. The ACL entry withdraws any previous drop action if another PIE hit occurs. \n
 *   e. The packet header is dumped if this ACL entry is hit. \n
 *   f. Lookup miss in the ACL function allows the packet to continue to be processed by L2 and VLAN functions.
 *  @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dv1_acl_dscp_rmk(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dv1_acl_dscp_rmk(uint32 unit)
{
    /* +--------------------------------------------------------------------------+ */
    /* |                         Template 2, PIE entry 64                         | */
    /* +----------+-------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0        | 1           | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +----------+-------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | OFFSET_0 | OFFSET_VLD0 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+-------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | 8899     | 0001        | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+-------------+----+----+----+----+----+----+----+----+----+----+ */
    /* | FFFF     | 0001        | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +----------+-------------+----+----+----+----+----+----+----+----+----+----+ */
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
    RtkPort                       port                = DV1_RTK_USER_PORT_1ST;
    AclRuleId                     index               = 64u;
    AclTemplateId                 tpIdx               = index / DV1_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t             templateInfo        = {0};
    AclPieEntryInfo_t             pieEntryInfo        = {0};
    AclTblAccessPieParam_t        pieParam            = {0};
    AclActionEntryInfo_t          actionEntryInfo     = {0};
    AclTblAccessActParam_t        actParam            = {0};
    AclOffsetIndex                offsetIndex         = 0u;
    uint16                        offsetAddr          = 100u;
    AclOffsetInfo_t               offsetInfo          = {0};
    AclOffsetInfo_t               offsetInfoGet       = {0};
    AclHeaderDumpEntryInfo_t      headerDumpEntryInfo = {0};
    AclTblAccessHeaderDumpParam_t headerDumpParam     = {0};

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
    templateInfo.type     = DV1_IACL_FLD_OFFSET_0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Field 1 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 1u;
    templateInfo.type     = DV1_IACL_FLD_OFFSET_VLD0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 64 */

    /* Configure offset value = 0x8899 */
    pieParam.data0 = 0x8899u;
    pieParam.mask0 = 0xFFFFu;
    /* Set the offset valid flag to 1 */
    pieParam.data1 = ((uint16)1u << offsetIndex);
    pieParam.mask1 = ((uint16)1u << offsetIndex);

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 64 */

    /*
        drop = 2 (withdraw drop)
        headerDump = 1 (dump the packet header)
    */
    actParam.drop       = 2u;
    actParam.headerDump = 1u;

    /*
        dscprmk = 1
        opcode = 0 (DSCP remarking)
        dscpIpPrecDtr = 32 (define ACL-based DSCP)
    */
    actParam.dscprmk                   = 1u;
    actParam.dscpRmkInfo.opcode        = 0u;
    actParam.dscpRmkInfo.dscpIpPrecDtr = 32u;

    /* Configure action entry 64 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure offset entry 0 */

    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; port++)
    {
        /*
            offsetIndex = offsetIndex (0)
            offsetType = DV1_ACL_OFFSET_TYPE_PAYLOAD (Start of Payload ~ Start of Payload + 2047 (total 2048 bytes))
            offsetPosition = 50 (offsetPosition = header offset address / 2)
        */
        offsetInfo.offsetIndex    = offsetIndex;
        offsetInfo.offsetType     = DV1_ACL_OFFSET_TYPE_PAYLOAD;
        offsetInfo.offsetPosition = (AclOffsetPosition)(offsetAddr / 2u);
        offsetInfo.portNum        = port;
        /* Configure and retrieve offset entry 0 */
        CHK_FUN_RET(retVal, rtk_acl_offset_set(unitChip, &offsetInfo));
        offsetInfoGet.offsetIndex = offsetIndex;
        offsetInfoGet.portNum     = port;
        CHK_FUN_RET(retVal, rtk_acl_offset_get(unitChip, &offsetInfoGet));
    }

    /* Step 6. Header dump packet */

    /* If any packet hits this ACL entry, the header of the packet can be dumped */
    headerDumpEntryInfo.aclHeaderDumpIndex = 0u;
    headerDumpEntryInfo.pHeaderDumpParam   = &headerDumpParam;
    CHK_FUN_RET(retVal, rtk_acl_getting_header_dump_entry(unitChip, &headerDumpEntryInfo));

    /* Expected Behaviour:
        a. Configure ACL entry 64 in Template2 and use template field 0 and 1.
        b. This ACL entry only hits the packet payload address at 100 ~ 101, and the value is 0x8899.
        c. This packet will be remarked as the DSCP to 32.
        d. If the ACL function has another PIE hit and does the drop action, this ACL entry will withdraw the drop action and perform this DSCP remarking action.
        e. Lookup miss in the ACL function is permitted to keep the process by L2 and VLAN function.
        f. If any packet hits this ACL entry, the header of the packet can be dumped.
    */
    return retVal;
}
/**@}*/ /* RTL907DV1_ACL_DSCP_RMK */
/**@}*/ /* RTL907DV1_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */