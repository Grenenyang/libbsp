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
 * @addtogroup RTL907DV1_ACL_COPY_TO_CPU ACL Copy to CPU Example
 * @brief This example demonstrates the setup of ACL rules to forward packets from one port to multiple CPU ports for detailed examination.
 *
 * @section Purpose
 *   This example demonstrates how to copy packets to the CPU ports for further processing or analysis.
 *
 * @section Description
 *   This example configures ACL to copy incoming packets to specific CPU ports: \n
 *   a. Configures ACL entry 64 in Template2 using template field 0. \n
 *   b. Copies all packets received from physical PORT_1ST to physical PORT_2ND and PORT_3RD. \n
 *   c. Ensures that any lookup miss in the ACL function results in the packet being dropped.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet dv1_acl_cpy_to_cpu(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dv1_acl_cpy_to_cpu(uint32 unit)
{
    /* +-------------------------------------------------------------+ */
    /* |                   Template 2, PIE entry 64                  | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0    | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | FMT  | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0400 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 3C00 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_V1,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = DV1_RTK_USER_PORT_1ST,
        .aclEn   = DV1_ACL_ENABLE,
        .defAct  = DV1_ACL_DEF_ACT_DROP,
    };
    RtkPort                port            = DV1_RTK_USER_PORT_1ST;
    AclRuleId              index           = 64u;
    AclTemplateId          tpIdx           = index / DV1_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t      templateInfo    = {0};
    AclPieEntryInfo_t      pieEntryInfo    = {0};
    AclTblAccessPieParam_t pieParam        = {0};
    AclActionEntryInfo_t   actionEntryInfo = {0};
    AclTblAccessActParam_t actParam        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 2 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = DV1_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 64 */

    /* SPHY[13:10]  = DV1_RTK_USER_PORT_1ST (ingress port number = DV1_RTK_USER_PORT_1ST) */
    pieParam.data0 = (uint16)dv1_g_aclUserPort2sysPort[DV1_RTK_USER_PORT_1ST] << 10u;
    pieParam.mask0 = 0x3C00u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 64 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 64 */

    /*
        copytocpu = 1,
        copyCpuDpm = DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD
        This packet must be forwarded normally for the copy action to be executed.
    */
    actParam.copytocpu  = 1u;
    actParam.copyCpuDpm = DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD;

    /* Configure action entry 64 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Expected Behavior:
        a. Configure ACL entry 64 in Template2 and use template field 0.
        b. All packets received from physical PORT_1ST will be copied to physical PORT_2ND and PORT_3RD.
        c. Any lookup miss in the ACL function will result in the packet being dropped.
    */
    return retVal;
}
/**@}*/ /* RTL907DV1_ACL_COPY_TO_CPU */
/**@}*/ /* RTL907DV1_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */