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

#ifndef CVB_ASICDRV_ACL_H__
#define CVB_ASICDRV_ACL_H__

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/********* Boundary *********/
#define CVB_ACL_INT_TEMPLATE_BOUND    (17u)
#define CVB_ACL_INT_RULE_BOUND        (575u)
#define CVB_ACL_INT_RULE_PIE33_LBOUND ((33u) * (CVB_ACL_PIE_RULE_NUM))                                    /* PIE33 first index: 528 */
#define CVB_ACL_INT_RULE_PIE33_HBOUND ((CVB_ACL_INT_RULE_PIE33_LBOUND) + ((CVB_ACL_PIE_RULE_NUM) - (1u))) /* PIE33 last  index: 543 */
#define CVB_ACL_INT_RULE_PIE35_LBOUND ((35u) * (CVB_ACL_PIE_RULE_NUM))                                    /* PIE35 first index: 560 */
#define CVB_ACL_INT_RULE_PIE35_HBOUND ((CVB_ACL_INT_RULE_PIE35_LBOUND) + ((CVB_ACL_PIE_RULE_NUM) - (1u))) /* PIE35 last  index: 575 */
#define CVB_ACL_AIF_DATA_REG_BOUND    (3u)                                                                /* AIF 1-3 == dataReg 1-3 */

/********* Internal Defines *********/
#define CVB_ACL_IP6_RANGE_LBOUND_TYPE (0u)
#define CVB_ACL_IP6_RANGE_HBOUND_TYPE (1u)

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet cvb_drv_acl_userport_2_sysport(RtkPort userPort, RtkPort *pSysPort);
EXTERN_API RtkApiRet cvb_drv_acl_userpmap_2_syspmap(RtkPmap userPmap, RtkPmap *pSysPmap);
EXTERN_API RtkApiRet cvb_drv_acl_internal_rule_id_check(uint32 unit, AclRuleId aclRuleIndex);
EXTERN_API RtkApiRet cvb_drv_acl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType);
EXTERN_API RtkApiRet cvb_drv_acl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type);
EXTERN_API RtkApiRet cvb_drv_acl_getting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPieParam_t *pPieParam);
EXTERN_API RtkApiRet cvb_drv_acl_setting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam);
EXTERN_API RtkApiRet cvb_drv_acl_copytocpu_action_get(uint32 unit, AclRuleId aclRuleIndex, RtkPmap *copyCpuDpm);
EXTERN_API RtkApiRet cvb_drv_acl_copytocpu_action_set(uint32 unit, AclRuleId aclRuleIndex, RtkPmap copyCpuDpm);
EXTERN_API RtkApiRet cvb_drv_acl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet cvb_drv_acl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet cvb_drv_acl_ip6_range_check(const AclIpv6 *pUpper, const AclIpv6 *pLower);
EXTERN_API RtkApiRet cvb_drv_acl_ip6_range_set_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, const AclIpv6 *ipv6);
EXTERN_API RtkApiRet cvb_drv_acl_ip6_range_get_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, AclIpv6 *ipv6);
#endif /* CVB_ASICDRV_ACL_H__ */
