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

#ifndef DVA_ASICDRV_ACL_H__
#define DVA_ASICDRV_ACL_H__

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/********* Boundary *********/
#define DVA_ACL_INT_TEMPLATE_BOUND    (17u)
#define DVA_ACL_INT_RULE_BOUND        (575u)
#define DVA_ACL_INT_RULE_PIE33_LBOUND ((33u) * (DVA_ACL_PIE_RULE_NUM))                                    /* PIE33 first index: 528 */
#define DVA_ACL_INT_RULE_PIE33_HBOUND ((DVA_ACL_INT_RULE_PIE33_LBOUND) + ((DVA_ACL_PIE_RULE_NUM) - (1u))) /* PIE33 last  index: 543 */
#define DVA_ACL_INT_RULE_PIE35_LBOUND ((35u) * (DVA_ACL_PIE_RULE_NUM))                                    /* PIE35 first index: 560 */
#define DVA_ACL_INT_RULE_PIE35_HBOUND ((DVA_ACL_INT_RULE_PIE35_LBOUND) + ((DVA_ACL_PIE_RULE_NUM) - (1u))) /* PIE35 last  index: 575 */
#define DVA_ACL_AIF_DATA_REG_BOUND    (2u)                                                                /* AIF 1-3 == dataReg 0-2 */

/********* Internal Defines *********/
#define DVA_ACL_IS_IACL               (0u)
#define DVA_ACL_IS_EACL               (1u)
#define DVA_ACL_IP6_RANGE_LBOUND_TYPE (0u)
#define DVA_ACL_IP6_RANGE_HBOUND_TYPE (1u)
#define DVA_ACL_PROTOCOL_PRIORITY     (0x6u)

/********* Internal Protocols *********/
/* TG 16 */
#define DVA_ACL_MKA              (513u)
#define DVA_ACL_DOT1X_ARP        (514u)
#define DVA_ACL_DOT1X_RADIUS     (515u)
#define DVA_ACL_DOT1X_EAP        (516u)
#define DVA_ACL_ROUTE_ARP        (517u)
#define DVA_ACL_ROUTE_ICMP6_NS   (518u)
#define DVA_ACL_ROUTE_ICMP6_NA   (519u)
#define DVA_ACL_ROUTE_UC_LK_MISS (520u)
#define DVA_ACL_IGMP4            (521u)
#define DVA_ACL_MLD_ICMP6_QUERY  (522u)
#define DVA_ACL_MLD_ICMP6_REPORT (523u)
#define DVA_ACL_MLD_ICMP6_DONE   (524u)
/* TG 17 */
#define DVA_ACL_PTP2          (544u)
#define DVA_ACL_STP           (545u)
#define DVA_ACL_MSRP          (546u)
#define DVA_ACL_MVRP_CUSTOMER (547u)
#define DVA_ACL_MVRP_PROVIDER (548u)
#define DVA_ACL_MMRP          (549u)
#define DVA_ACL_MIRROR        (550u)
#define DVA_ACL_PTP           (551u)
#define DVA_ACL_PTP3          (552u)
#define DVA_ACL_STACK_S0MAC   (553u)
#define DVA_ACL_STACK_S1MAC   (554u)
#define DVA_ACL_ETH_BC        (555u)
#define DVA_ACL_ETH_L3        (556u)
#define DVA_ACL_ETH_MC        (557u)
#define DVA_ACL_ETH_UC        (558u)

/********* Template *********/
#define DVA_ACL_TEMPLATE0  (0u)
#define DVA_ACL_TEMPLATE1  (1u)
#define DVA_ACL_TEMPLATE2  (2u)
#define DVA_ACL_TEMPLATE3  (3u)
#define DVA_ACL_TEMPLATE4  (4u)
#define DVA_ACL_TEMPLATE5  (5u)
#define DVA_ACL_TEMPLATE6  (6u)
#define DVA_ACL_TEMPLATE7  (7u)
#define DVA_ACL_TEMPLATE8  (8u)
#define DVA_ACL_TEMPLATE9  (9u)
#define DVA_ACL_TEMPLATE10 (10u)
#define DVA_ACL_TEMPLATE11 (11u)
#define DVA_ACL_TEMPLATE12 (12u)
#define DVA_ACL_TEMPLATE13 (13u)
#define DVA_ACL_TEMPLATE14 (14u)
#define DVA_ACL_TEMPLATE15 (15u)
#define DVA_ACL_TEMPLATE16 (16u)
#define DVA_ACL_TEMPLATE17 (17u)

/********* Shared Memory *********/
#define DVA_ACL_SHARE_MEMORY_BASE_ADDR   (0x01136db8)
#define DVA_ACL_SHARE_MEMORY_IN_USE_ADDR (0x01136dbc)
#define DVA_ACL_SHARE_MEMORY_IN_USE      (1u)
#define DVA_ACL_SHARE_MEMORY_NOT_IN_USE  (0u)

/********* Data Format *********/
#define DVA_ACL_PIE_DMA_BASE_ADDR            (0x511C0000u)
#define DVA_ACL_PIE_DMA_ACTUAL_DATA_REG_SIZE (0x40u)
#define DVA_ACL_WRITE_ENTRY_NUM_PER_ROUND    (128u) /* The max value is 128, because Mode 0 domain handler 1 block number */
#define DVA_ACL_CONFIG_TABLE_MODE0_INFO_BYTE (4u)
#define DVA_ACL_PIE_DATA_MAX_LEN             ((DVA_ACL_CONFIG_TABLE_MODE0_INFO_BYTE) + (52u) + (18u * 4u)) /* 52u: pie entry length, 18u: 18 crc golden registers, 4u: 4 bytes; total length: 128u */

/********* Indirect Access *********/
#define DVA_ACL_INDAC_TRIGGER_LSP        (31u)
#define DVA_ACL_INDAC_ERROR_LSP          (30u)
#define DVA_ACL_INDAC_LOCK_LSP           (28u)
#define DVA_ACL_INDAC_FSM_LSP            (24u)
#define DVA_ACL_INDAC_FSM_STATE_LSP      (16u)
#define DVA_ACL_INDAC_FSM_STATE_LEN      (8u)
#define DVA_ACL_INDAC_TARGET_FSM         (0xFu)
#define DVA_ACL_INDAC_ACL_OP_WRITE       (0x0400u)
#define DVA_ACL_INDAC_CORE_OP_ONGOING    (0x0u)
#define DVA_ACL_INDAC_CORE_STATE_IDLE    (0u)
#define DVA_ACL_INDAC_CORE_STATE_ONGOING (2u)
#define DVA_ACL_INDAC_TRY_TIMES          (600u) /* DVA_ACL_INDAC_DELAY_US * DVA_ACL_INDAC_TRY_TIMES = 60ms */
#define DVA_ACL_INDAC_DELAY_US           (100u) /* 100us */
#define DVA_ACL_INDAC_PIE_SET_STATUS     (DVA_HW_MISC_DMY38)
#define DVA_ACL_INDAC_PIE_SET_FAIL_LSP   (1u)

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dva_drv_acl_userport_2_sysport(RtkPort userPort, RtkPort *pSysPort);
EXTERN_API RtkApiRet dva_drv_acl_userpmap_2_syspmap(RtkPmap userPmap, RtkPmap *pSysPmap);
EXTERN_API RtkApiRet dva_drv_acl_internal_rule_id_check(uint32 unit, AclRuleId aclRuleIndex);
EXTERN_API RtkApiRet dva_drv_acl_partition_get(uint32 unit, AclPartition *pAclPartition);
EXTERN_API RtkApiRet dva_drv_acl_partition_set(uint32 unit, AclPartition aclPartition);
EXTERN_API RtkApiRet dva_drv_acl_mode_template_index_check(uint32 unit, AclTemplateId tpIdx, AclMode aclMode);
EXTERN_API RtkApiRet dva_drv_acl_mode_rule_index_check(uint32 unit, AclRuleId aclRuleIndex, AclMode aclMode);
EXTERN_API RtkApiRet dva_drv_acl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType);
EXTERN_API RtkApiRet dva_drv_acl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type);
EXTERN_API RtkApiRet dva_drv_eacl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType);
EXTERN_API RtkApiRet dva_drv_eacl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type);
EXTERN_API RtkApiRet dva_drv_acl_getting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPieParam_t *pPieParam);
EXTERN_API RtkApiRet dva_drv_acl_setting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam);
EXTERN_API RtkApiRet dva_drv_acl_runtime_add_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam);
EXTERN_API RtkApiRet dva_drv_acl_runtime_trigger_pie_entries(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_copytocpu_action_get(uint32 unit, AclRuleId aclRuleIndex, RtkPmap *copyCpuDpm);
EXTERN_API RtkApiRet dva_drv_acl_copytocpu_action_set(uint32 unit, AclRuleId aclRuleIndex, RtkPmap copyCpuDpm);
EXTERN_API RtkApiRet dva_drv_acl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet dva_drv_acl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet dva_drv_eacl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, EaclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet dva_drv_eacl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const EaclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet dva_drv_acl_ip6_range_check(const AclIpv6 *pUpper, const AclIpv6 *pLower);
EXTERN_API RtkApiRet dva_drv_acl_ip6_range_set_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, const AclIpv6 *ipv6);
EXTERN_API RtkApiRet dva_drv_acl_ip6_range_get_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, AclIpv6 *ipv6);
EXTERN_API RtkApiRet dva_drv_acl_config_mka(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_dot1x_arp(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_dot1x_radius(uint32 unit, uint16 udpPort);
EXTERN_API RtkApiRet dva_drv_acl_config_dot1x_eap(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_route_arp(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_route_ns(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_route_na(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_route_uc_lk_miss(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_igmp(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_mld_query(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_mld_report(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_mld_done(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_stp(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_msrp(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_mvrp_customer(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_mvrp_provider(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_mmrp(uint32 unit);
EXTERN_API RtkApiRet dva_drv_acl_config_clear(uint32 unit, AclRuleId aclRuleIndex);
#endif /* DVA_ASICDRV_ACL_H__ */
