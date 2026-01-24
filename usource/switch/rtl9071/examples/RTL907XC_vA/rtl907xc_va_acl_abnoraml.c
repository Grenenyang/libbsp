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
 * @addtogroup RTL907CVA_ACL_EXAMPLE RTL907XC_VA ACL Example
 * @{
 */
/**
 * @addtogroup RTL907CVA_ACL_ABNORMAL ACL Abnormal Example
 * @brief This example provides a detailed setup for using ACL to handle and control abnormal network traffic scenarios.
 *
 * @section Purpose
 *   This example demonstrates the advanced configuration of ACL to manage and mitigate abnormal network traffic conditions effectively.
 *
 * @section Description
 *   This example provides a comprehensive setup for managing abnormal network conditions using ACL: \n
 *   a. Setting up abnormal times entries to monitor traffic anomalies. \n
 *   b. Enabling time slot functions to apply specific rules within defined time intervals. \n
 *   c. Configuring abnormal destination port masks to trap packets.
 * @{
 */
/******************************************************************************/
/* Function Prototype                                                         */
/******************************************************************************/
RtkApiRet cva_acl_abnormal(uint32 unit);

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet cva_acl_abnormal(uint32 unit)
{
    /* +-------------------------------------------------------------+ */
    /* |                   Template 0, PIE entry 0                   | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0    | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | FMT  | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 0800 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */
    /* | 3C00 | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | */
    /* +------+----+----+----+----+----+----+----+----+----+----+----+ */

    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VA,
    };
    AclEnableCtrlInfo_t enableCtrlInfo = {
        .portNum = CVA_RTK_USER_PORT_1ST,
        .aclEn   = CVA_ACL_ENABLE,
        .defAct  = CVA_ACL_DEF_ACT_DROP,
    };
    RtkPort                          port                      = CVA_RTK_USER_PORT_1ST;
    AclRuleId                        index                     = 0u;
    AclTemplateId                    tpIdx                     = index / CVA_ACL_TEMPLATE_RULE_NUM;
    AclTemplateInfo_t                templateInfo              = {0};
    AclPieEntryInfo_t                pieEntryInfo              = {0};
    AclTblAccessPieParam_t           pieParam                  = {0};
    AclActionEntryInfo_t             actionEntryInfo           = {0};
    AclTblAccessActParam_t           actParam                  = {0};
    AclPolicerEntryInfo_t            policerEntryInfo          = {0};
    AclTblAccessPolicerParam_t       policerParam              = {0};
    AclAbnormalTimesEntryInfo_t      abnormalTimesEntryInfo    = {0};
    AclTblAccessAbnormalTimesParam_t abnormalTimesParam        = {0};
    AclAbnormalTimesEntryInfo_t      abnormalTimesEntryInfoGet = {0};
    AclTblAccessAbnormalTimesParam_t abnormalTimesParamGet     = {0};
    AclTimeSlotEnInfo_t              timeSlotEnInfo            = {0};
    AclTimeSlotEnInfo_t              timeSlotEnInfoGet         = {0};
    AclTimeSlotThresholdInfo_t       timeSlotThresholdInfo     = {0};
    AclTimeSlotThresholdInfo_t       timeSlotThresholdInfoGet  = {0};
    AclAbnormalDpmInfo_t             abnormalDpmInfo           = {0};
    AclAbnormalDpmInfo_t             abnormalDpmInfoGet        = {0};

    /* Step 1. Enable ACL lookup for each port and configure the default action to drop packets */
    for(port = CVA_RTK_USER_PORT_1ST; port <= CVA_RTK_USER_PORT_END; port++)
    {
        enableCtrlInfo.portNum = port;
        CHK_FUN_RET(retVal, rtk_acl_enable_ctrl_set(unitChip, &enableCtrlInfo));
    }

    /* Step 2. Configure field type of template 0 */

    /* Field 0 */
    templateInfo.tpIdx    = tpIdx;
    templateInfo.fieldIdx = 0u;
    templateInfo.type     = CVA_IACL_FLD_FMT;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &templateInfo));

    /* Step 3. Configure PIE entry 0 */

    /* SPHY[13:10]  = CVA_RTK_USER_PORT_1ST (ingress port number = CVA_RTK_USER_PORT_1ST) */
    pieParam.data0 = (uint16)cva_g_aclUserPort2sysPort[CVA_RTK_USER_PORT_1ST] << 10u;
    pieParam.mask0 = 0x3C00u;

    /* valid = 1 */
    pieParam.valid = 1u;

    /* Configure PIE entry 0 */
    pieEntryInfo.aclRuleIndex = index;
    pieEntryInfo.pPieParam    = &pieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &pieEntryInfo));

    /* Step 4. Configure action entry 0 */

    /*
        redir = 1,
        opcode = 0 (normal forward),
        dpm = CVA_RTK_USER_PMAP_2ND,
        crsvlan = 0 (forward in VLAN)
    */
    actParam.redir             = 1u;
    actParam.redirInfo.opcode  = 0u;
    actParam.redirInfo.dpm     = CVA_RTK_USER_PMAP_2ND;
    actParam.redirInfo.crsvlan = 0u;

    /* Configure action entry 0 */
    actionEntryInfo.aclRuleIndex = index;
    actionEntryInfo.pActParam    = &actParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &actionEntryInfo));

    /* Step 5. Configure policer entry 0 */

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

    /* Configure policer entry 0 */
    policerEntryInfo.aclRuleIndex  = index;
    policerEntryInfo.pPolicerParam = &policerParam;
    CHK_FUN_RET(retVal, rtk_acl_policer_entry_set(unitChip, &policerEntryInfo));

    /* Step 6. Configure abnormal times entry 0 */

    /*
        enable = 1 (enable abnormal times)
        abnormalThreshold = 0xFFFF (abnormal counter threshold)
    */
    abnormalTimesParam.enable    = 1u;
    abnormalTimesParam.threshold = 0xFFFFu;

    /* Configure and retrieve abnormal times entry 0 */
    abnormalTimesEntryInfo.aclRuleIndex        = index;
    abnormalTimesEntryInfo.pAbnormalTimesParam = &abnormalTimesParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_abnormal_times_entry(unitChip, &abnormalTimesEntryInfo));
    abnormalTimesEntryInfoGet.aclRuleIndex        = index;
    abnormalTimesEntryInfoGet.pAbnormalTimesParam = &abnormalTimesParamGet;
    CHK_FUN_RET(retVal, rtk_acl_getting_abnormal_times_entry(unitChip, &abnormalTimesEntryInfoGet));

    /* Step 7. Configure time slot function */

    /* Enable the time slot function and retrieve the time slot enable status */
    timeSlotEnInfo.enable = 1u;
    CHK_FUN_RET(retVal, rtk_acl_time_slot_en_set(unitChip, &timeSlotEnInfo));
    CHK_FUN_RET(retVal, rtk_acl_time_slot_en_get(unitChip, &timeSlotEnInfoGet));

    /* Configure and retrieve the time slot threshold (the interval of time slot) */
    timeSlotThresholdInfo.threshold = 0x1u;
    CHK_FUN_RET(retVal, rtk_acl_time_slot_threshold_set(unitChip, &timeSlotThresholdInfo));
    CHK_FUN_RET(retVal, rtk_acl_time_slot_threshold_get(unitChip, &timeSlotThresholdInfoGet));

    /* Step 8. Configure abnormal dpm */

    /* Configure and retrieve the abnormal destination port mask */
    abnormalDpmInfo.dpm = CVA_RTK_USER_PMAP_3RD;
    CHK_FUN_RET(retVal, rtk_acl_abnormal_dpm_set(unitChip, &abnormalDpmInfo));
    CHK_FUN_RET(retVal, rtk_acl_abnormal_dpm_get(unitChip, &abnormalDpmInfoGet));

    /* Expected Behavior:
        a. Configure ACL entry 0 in Template0 and use template field 0.
        b. All packets received from physical PORT_1ST will be redirected to physical PORT_2ND, and the forward rate is 80000 Kbps.
        c. Abnormal times function will be enabled and the abnormal times occurring over 0xffff will trigger an interrupt.
        d. Enable time slot function and time slot threshold (interval) will be set to 1ms.
        e. When the abnormal times occur, the frame will be forwarded to PORT_3RD (abnormal dpm).
        f. Any lookup miss in the ACL function will result in the packet being dropped.
    */
    return retVal;
}
/**@}*/ /* RTL907CVA_ACL_ABNORMAL */
/**@}*/ /* RTL907CVA_ACL_EXAMPLE */
/**@}*/ /* ACL_EXAMPLE */
/**@}*/ /* EXAMPLE */