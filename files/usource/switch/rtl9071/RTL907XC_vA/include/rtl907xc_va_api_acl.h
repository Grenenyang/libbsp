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

#ifndef CVA_RTK_API_ACL_H__
#define CVA_RTK_API_ACL_H__
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_api_acl.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/********* Boundary *********/
#define CVA_ACL_TEMPLATE_BOUND             (15u)
#define CVA_ACL_TEMPLATE_FIELD_BOUND       (11u)
#define CVA_IACL_FLD_LBOUND                (CVA_IACL_FLD_FMT)
#define CVA_IACL_FLD_HBOUND                (CVA_IACL_FLD_IP_TYPE)
#define CVA_ACL_RULE_BOUND                 (511u)
#define CVA_ACL_PIE_BLOCK_BOUND            (31u)
#define CVA_ACL_PIE_PRIORITY_BOUND         (31u)
#define CVA_ACL_OFFSET_BOUND               (31u)
#define CVA_ACL_OFFSET_TYPE_LBOUND         (CVA_ACL_OFFSET_TYPE_IPv4)
#define CVA_ACL_OFFSET_TYPE_HBOUND         (CVA_ACL_OFFSET_TYPE_PAYLOAD)
#define CVA_ACL_OFFSET_POSITION_BOUND      (0x3FFu)
#define CVA_FILTER_L4PORTRANGE_BOUND       (3u)
#define CVA_L4PORT_RANGE_TYPE_LBOUND       (CVA_L4PORT_RANGE_DISABLE)
#define CVA_L4PORT_RANGE_TYPE_HBOUND       (CVA_L4PORT_RANGE_TCP_OR_UDP_DEST_PORT)
#define CVA_L4PORT_RANGE_BOUND             (0xFFFFu)
#define CVA_FILTER_VIDRANGE_BOUND          (7u)
#define CVA_VID_RANGE_TYPE_LBOUND          (CVA_VID_RANGE_DISABLE)
#define CVA_VID_RANGE_TYPE_HBOUND          (CVA_VID_RANGE_IVID_OR_OVID)
#define CVA_VID_RANGE_BOUND                (0xFFFu)
#define CVA_FILTER_IPRANGE_BOUND           (7u)
#define CVA_IP_RANGE_TYPE_LBOUND           (CVA_IP_RANGE_DISABLE)
#define CVA_IP_RANGE_TYPE_HBOUND           (CVA_IP_RANGE_SOURCE_OR_DEST_IP)
#define CVA_IP_RANGE_BOUND                 (0xFFFFFFFFu)
#define CVA_FILTER_PKTLENRANGE_BOUND       (3u)
#define CVA_PKTLEN_RANGE_BOUND             (0x3FFFu)
#define CVA_FILTER_IP6RANGE_BOUND          (7u)
#define CVA_IP6_RANGE_TYPE_LBOUND          (CVA_IP_RANGE_DISABLE)
#define CVA_IP6_RANGE_TYPE_HBOUND          (CVA_IP_RANGE_SOURCE_OR_DEST_IP)
#define CVA_FILTER_MACRANGE_BOUND          (15u)
#define CVA_MAC_RANGE_TYPE_LBOUND          (CVA_MAC_RANGE_DISABLE)
#define CVA_MAC_RANGE_TYPE_HBOUND          (CVA_MAC_RANGE_SOURCE_OR_DEST_MAC)
#define CVA_FILTER_L4PORTMEMBER_BOUND      (15u)
#define CVA_L4PORT_MEMBER_TYPE_LBOUND      (CVA_L4PORT_MEMBER_SRC_PORT)
#define CVA_L4PORT_MEMBER_TYPE_HBOUND      (CVA_L4PORT_MEMBER_DEST_PORT)
#define CVA_L4PORT_MEMBER_BOUND            (0xFFFFu)
#define CVA_FILTER_VIDMEMBER_BOUND         (15u)
#define CVA_VID_MEMBER_TYPE_LBOUND         (CVA_VID_MEMBER_DISABLE)
#define CVA_VID_MEMBER_TYPE_HBOUND         (CVA_VID_MEMBER_ALL_VID)
#define CVA_VID_MEMBER_BOUND               (0xFFFu)
#define CVA_FILTER_SRCPORTMASK_BOUND       (3u)
#define CVA_FILTER_IPMEMBER_BOUND          (15u)
#define CVA_IP_MEMBER_TYPE_LBOUND          (CVA_IP_MEMBER_SIP)
#define CVA_IP_MEMBER_TYPE_HBOUND          (CVA_IP_MEMBER_DIP)
#define CVA_IP_MEMBER_BOUND                (0xFFFFFFFFu)
#define CVA_FILTER_IP6MEMBER_BOUND         (15u)
#define CVA_FILTER_MACMEMBER_BOUND         (15u)
#define CVA_MAC_MEMBER_TYPE_LBOUND         (CVA_MAC_MEMBER_SMAC)
#define CVA_MAC_MEMBER_TYPE_HBOUND         (CVA_MAC_MEMBER_DMAC)
#define CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND (63u)
#define CVA_ACL_TIME_SLOT_THRESHOLD_LBOUND (0x1u)
#define CVA_ACL_TIME_SLOT_THRESHOLD_HBOUND (0xFFFFFu)
#define CVA_ACL_HEADER_DUMP_BOUND          (99u)

/********* ACL SRAM Config *********/
#define CVA_ACL_SRAM_ACT_EN (0x001ACA45u)

/********* ACL Resource *********/
#define CVA_ACL_PIE_RULE_NUM          (16u)                                                      /* Number of ACL entry in one Packet Inspection Engine: 16 */
#define CVA_ACL_TEMPLATE_NUM          ((CVA_ACL_TEMPLATE_BOUND) + (1u))                          /* Total number of Template Generator: 16 */
#define CVA_ACL_TEMPLATE_FIELD_NUM    ((CVA_ACL_TEMPLATE_FIELD_BOUND) + (1u))                    /* Number of filed in one Template Generator: 12 */
#define CVA_ACL_TEMPLATE_PIEBLOCK_NUM (2u)                                                       /* Number of Packet Inspection Engine in one Template Generator  */
#define CVA_ACL_TEMPLATE_RULE_NUM     ((CVA_ACL_PIE_RULE_NUM) * (CVA_ACL_TEMPLATE_PIEBLOCK_NUM)) /* Number of rule in one Template Generator: 32 */

/********* Enable *********/
#define CVA_ACL_DISABLE (0u) /**< ACL disable function */
#define CVA_ACL_ENABLE  (1u) /**< ACL enable function */

/********* Default Action *********/
#define CVA_ACL_DEF_ACT_PERMIT (0u) /**< ACL lookup miss permit */
#define CVA_ACL_DEF_ACT_DROP   (1u) /**< ACL lookup miss drop */

/********* Reverse *********/
#define CVA_ACL_NORMAL  (0u) /**< keep result */
#define CVA_ACL_REVERSE (1u) /**< reverse result */

/********* Aggregator *********/
#define CVA_ACL_NOT_AGG          (0u) /**< not aggregation */
#define CVA_ACL_SEL_UP_PIE_AGG   (1u) /**< select upside PIE after aggregation, it will execute the upside action */
#define CVA_ACL_SEL_DOWN_PIE_AGG (2u) /**< select downside PIE after aggregation, it will execute the downside action */

/********* Offset Type *********/
#define CVA_ACL_OFFSET_TYPE_IPv4    (0u) /**< Start of IPv4 header ~ End of IPv4 header */
#define CVA_ACL_OFFSET_TYPE_IPv6    (1u) /**< Start of IPv6 header ~ End of IPv6 header */
#define CVA_ACL_OFFSET_TYPE_L3      (2u) /**< Start of L3 header   ~ Start of L3 header + 2047 (total 2048 bytes) */
#define CVA_ACL_OFFSET_TYPE_TCP     (3u) /**< Start of TCP header  ~ End of TCP header */
#define CVA_ACL_OFFSET_TYPE_UDP     (4u) /**< Start of UDP header  ~ End of UDP header */
#define CVA_ACL_OFFSET_TYPE_L4      (5u) /**< Start of L4 header   ~ Start of L4 header + 2047 (total 2048 bytes) */
#define CVA_ACL_OFFSET_TYPE_PAYLOAD (6u) /**< Start of Payload     ~ Start of Payload + 2047 (total 2048 bytes) */

/********* L4Port Range Type *********/
#define CVA_L4PORT_RANGE_DISABLE                (0u) /**< disable entry */
#define CVA_L4PORT_RANGE_TCP_SOURCE_PORT        (1u) /**< TCP source port */
#define CVA_L4PORT_RANGE_UDP_SOURCE_PORT        (2u) /**< UDP source port */
#define CVA_L4PORT_RANGE_TCP_OR_UDP_SOURCE_PORT (3u) /**< TCP or UDP source port */
#define CVA_L4PORT_RANGE_TCP_DEST_PORT          (5u) /**< TCP destination port */
#define CVA_L4PORT_RANGE_UDP_DEST_PORT          (6u) /**< UDP destination port */
#define CVA_L4PORT_RANGE_TCP_OR_UDP_DEST_PORT   (7u) /**< TCP or UDP destination port */

/********* VLAN Range Type *********/
#define CVA_VID_RANGE_DISABLE      (0u) /**< disable entry */
#define CVA_VID_RANGE_IVID_ONLY    (1u) /**< compare inner VLAN ID only */
#define CVA_VID_RANGE_OVID_ONLY    (2u) /**< compare outer VLAN ID only */
#define CVA_VID_RANGE_IVID_OR_OVID (3u) /**< compare either inner VLAN ID or outer VLAN ID */

/********* IP Range Type *********/
#define CVA_IP_RANGE_DISABLE           (0u) /**< disable entry */
#define CVA_IP_RANGE_SOURCE_IP         (1u) /**< compare IPv4/6 source IP */
#define CVA_IP_RANGE_DEST_IP           (2u) /**< compare IPv4/6 destination IP */
#define CVA_IP_RANGE_SOURCE_OR_DEST_IP (3u) /**< either source IP or destination IP */

/********* MAC Range Type *********/
#define CVA_MAC_RANGE_DISABLE            (0u) /**< disable entry */
#define CVA_MAC_RANGE_SOURCE_MAC         (1u) /**< compare source MAC */
#define CVA_MAC_RANGE_DEST_MAC           (2u) /**< compare destination MAC */
#define CVA_MAC_RANGE_SOURCE_OR_DEST_MAC (3u) /**< either source MAC or destination MAC */

/********* L4Port Member Type *********/
#define CVA_L4PORT_MEMBER_SRC_PORT  (0u) /**< compare source port */
#define CVA_L4PORT_MEMBER_DEST_PORT (1u) /**< compare destination port */

/********* VLAN Member Type *********/
#define CVA_VID_MEMBER_DISABLE (0u) /**< disable entry */
#define CVA_VID_MEMBER_IVID    (1u) /**< compare inner VLAN ID only */
#define CVA_VID_MEMBER_OVID    (2u) /**< compare outer VLAN ID only */
#define CVA_VID_MEMBER_ALL_VID (3u) /**< compare either inner VLAN ID or outer VLAN ID */

/********* IP Member Type *********/
#define CVA_IP_MEMBER_SIP (0u) /**< compare source IP */
#define CVA_IP_MEMBER_DIP (1u) /**< compare destination IP */

/********* MAC Member Type *********/
#define CVA_MAC_MEMBER_SMAC (0u) /**< compare source MAC */
#define CVA_MAC_MEMBER_DMAC (1u) /**< compare destination MAC */

/********* Ingress ACL Filter Key *********/
#define /*000*/ CVA_IACL_FLD_FMT (0u)
#define /*001*/ CVA_IACL_FLD_DMAC0 (1u)
#define /*002*/ CVA_IACL_FLD_DMAC1 (2u)
#define /*003*/ CVA_IACL_FLD_DMAC2 (3u)
#define /*004*/ CVA_IACL_FLD_SMAC0 (4u)
#define /*005*/ CVA_IACL_FLD_SMAC1 (5u)
#define /*006*/ CVA_IACL_FLD_SMAC2 (6u)
#define /*007*/ CVA_IACL_FLD_ETHERTYPE (7u)
#define /*008*/ CVA_IACL_FLD_OTAG (8u)
#define /*009*/ CVA_IACL_FLD_ITAG (9u)
#define /*010*/ CVA_IACL_FLD_PRIORITY (10u)
#define /*011*/ CVA_IACL_FLD_L2PROTOCOL (11u)
#define /*012*/ CVA_IACL_FLD_SIP0 (12u)
#define /*013*/ CVA_IACL_FLD_SIP1 (13u)
#define /*014*/ CVA_IACL_FLD_DIP0 (14u)
#define /*015*/ CVA_IACL_FLD_DIP1 (15u)
#define /*016*/ CVA_IACL_FLD_IPHDR (16u)
#define /*017*/ CVA_IACL_FLD_GIP0 (17u)
#define /*018*/ CVA_IACL_FLD_GIP1 (18u)
#define /*019*/ CVA_IACL_FLD_L4SPORT (19u)
#define /*020*/ CVA_IACL_FLD_L4DPORT (20u)
#define /*021*/ CVA_IACL_FLD_L4HDR (21u)
#define /*022*/ CVA_IACL_FLD_ICMPCODETYPE (22u)
#define /*023*/ CVA_IACL_FLD_SMP1 (23u)
#define /*024*/ CVA_IACL_FLD_RANGE0 (24u)
#define /*025*/ CVA_IACL_FLD_RANGE1 (25u)
#define /*026*/ CVA_IACL_FLD_PAYLOAD (26u)
#define /*027*/ CVA_IACL_FLD_SPHY_ORG (27u)
#define /*028*/ CVA_IACL_FLD_OFFSET_0 (28u)
#define /*029*/ CVA_IACL_FLD_OFFSET_1 (29u)
#define /*030*/ CVA_IACL_FLD_OFFSET_2 (30u)
#define /*031*/ CVA_IACL_FLD_OFFSET_3 (31u)
#define /*032*/ CVA_IACL_FLD_OFFSET_4 (32u)
#define /*033*/ CVA_IACL_FLD_OFFSET_5 (33u)
#define /*034*/ CVA_IACL_FLD_OFFSET_6 (34u)
#define /*035*/ CVA_IACL_FLD_OFFSET_7 (35u)
#define /*036*/ CVA_IACL_FLD_OFFSET_8 (36u)
#define /*037*/ CVA_IACL_FLD_OFFSET_9 (37u)
#define /*038*/ CVA_IACL_FLD_OFFSET_10 (38u)
#define /*039*/ CVA_IACL_FLD_OFFSET_11 (39u)
#define /*040*/ CVA_IACL_FLD_OFFSET_12 (40u)
#define /*041*/ CVA_IACL_FLD_OFFSET_13 (41u)
#define /*042*/ CVA_IACL_FLD_OFFSET_14 (42u)
#define /*043*/ CVA_IACL_FLD_OFFSET_15 (43u)
#define /*044*/ CVA_IACL_FLD_OFFSET_VLD0 (44u)
#define /*045*/ CVA_IACL_FLD_OFFSET_16 (45u)
#define /*046*/ CVA_IACL_FLD_OFFSET_17 (46u)
#define /*047*/ CVA_IACL_FLD_OFFSET_18 (47u)
#define /*048*/ CVA_IACL_FLD_OFFSET_19 (48u)
#define /*049*/ CVA_IACL_FLD_OFFSET_20 (49u)
#define /*050*/ CVA_IACL_FLD_OFFSET_21 (50u)
#define /*051*/ CVA_IACL_FLD_OFFSET_22 (51u)
#define /*052*/ CVA_IACL_FLD_OFFSET_23 (52u)
#define /*053*/ CVA_IACL_FLD_OFFSET_24 (53u)
#define /*054*/ CVA_IACL_FLD_OFFSET_25 (54u)
#define /*055*/ CVA_IACL_FLD_OFFSET_26 (55u)
#define /*056*/ CVA_IACL_FLD_OFFSET_27 (56u)
#define /*057*/ CVA_IACL_FLD_OFFSET_28 (57u)
#define /*058*/ CVA_IACL_FLD_OFFSET_29 (58u)
#define /*059*/ CVA_IACL_FLD_OFFSET_30 (59u)
#define /*060*/ CVA_IACL_FLD_OFFSET_31 (60u)
#define /*061*/ CVA_IACL_FLD_OFFSET_VLD1 (61u)
#define /*062*/ CVA_IACL_FLD_IPV6_SIP_0 (62u)
#define /*063*/ CVA_IACL_FLD_IPV6_SIP_1 (63u)
#define /*064*/ CVA_IACL_FLD_IPV6_SIP_2 (64u)
#define /*065*/ CVA_IACL_FLD_IPV6_SIP_3 (65u)
#define /*066*/ CVA_IACL_FLD_IPV6_SIP_4 (66u)
#define /*067*/ CVA_IACL_FLD_IPV6_SIP_5 (67u)
#define /*068*/ CVA_IACL_FLD_IPV6_SIP_6 (68u)
#define /*069*/ CVA_IACL_FLD_IPV6_SIP_7 (69u)
#define /*070*/ CVA_IACL_FLD_IPV6_DIP_0 (70u)
#define /*071*/ CVA_IACL_FLD_IPV6_DIP_1 (71u)
#define /*072*/ CVA_IACL_FLD_IPV6_DIP_2 (72u)
#define /*073*/ CVA_IACL_FLD_IPV6_DIP_3 (73u)
#define /*074*/ CVA_IACL_FLD_IPV6_DIP_4 (74u)
#define /*075*/ CVA_IACL_FLD_IPV6_DIP_5 (75u)
#define /*076*/ CVA_IACL_FLD_IPV6_DIP_6 (76u)
#define /*077*/ CVA_IACL_FLD_IPV6_DIP_7 (77u)
#define /*078*/ CVA_IACL_FLD_RESERVED_78 (78u)
#define /*079*/ CVA_IACL_FLD_RESERVED_79 (79u)
#define /*080*/ CVA_IACL_FLD_RESERVED_80 (80u)
#define /*081*/ CVA_IACL_FLD_RESERVED_81 (81u)
#define /*082*/ CVA_IACL_FLD_RESERVED_82 (82u)
#define /*083*/ CVA_IACL_FLD_RESERVED_83 (83u)
#define /*084*/ CVA_IACL_FLD_RESERVED_84 (84u)
#define /*085*/ CVA_IACL_FLD_RESERVED_85 (85u)
#define /*086*/ CVA_IACL_FLD_RESERVED_86 (86u)
#define /*087*/ CVA_IACL_FLD_RESERVED_87 (87u)
#define /*088*/ CVA_IACL_FLD_RESERVED_88 (88u)
#define /*089*/ CVA_IACL_FLD_RESERVED_89 (89u)
#define /*090*/ CVA_IACL_FLD_RESERVED_90 (90u)
#define /*091*/ CVA_IACL_FLD_RESERVED_91 (91u)
#define /*092*/ CVA_IACL_FLD_RANGCHK_MAC_GROUP_RC (92u)
#define /*093*/ CVA_IACL_FLD_RANGCHK_MAC (93u)
#define /*094*/ CVA_IACL_FLD_RANGCHK_MAC_RC (94u)
#define /*095*/ CVA_IACL_FLD_RANGCHK_IPV6 (95u)
#define /*096*/ CVA_IACL_FLD_RANGCHK_L4PORT (96u)
#define /*097*/ CVA_IACL_FLD_RANGCHK_VID (97u)
#define /*098*/ CVA_IACL_FLD_RANGCHK_IP (98u)
#define /*099*/ CVA_IACL_FLD_LOG_PORT (99u)
#define /*100*/ CVA_IACL_FLD_ORI_PORT (100u)
#define /*101*/ CVA_IACL_FLD_IP_TYPE (101u)

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/********* PIE *********/
typedef struct
{
    uint32 val[13]; /**< PIE table entry */

} cva_AclTblAccessPieEntry_t;

/********* Action *********/
typedef struct
{
    uint32 val[4]; /**< action table entry */

} cva_AclTblAccessActEntry_t;

/********* Counter *********/
typedef struct
{
    uint32 val[2]; /**< counter table entry */

} cva_AclTblAccessCounterEntry_t;

/********* Policer *********/
typedef struct
{
    uint32 val[2]; /**< policer table entry */

} cva_AclTblAccessPolicerEntry_t;

/********* Abnormal Times *********/
typedef struct
{
    uint32 val[2]; /**< abnormal times table entry */

} cva_AclTblAccessAbnormalTimesEntry_t;

/********* Timer Discard *********/
typedef struct
{
    uint32 val[8]; /**< timer discard table entry */

} cva_AclTblAccessTimerDiscardEntry_t;

/********* Header Dump *********/
typedef struct
{
    uint32 val[31]; /**< header dump table entry */

} cva_AclTblAccessHeaderDumpEntry_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
EXTERN_API const uint8 cva_g_aclUserPort2sysPort[CVA_RTL9075_INTERFACE_NUM];

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet cva_acl_enable_ctrl_get(uint32 unit, RtkPort portNum, AclEnable *pAclEn, AclDefAct *pDefAct);
EXTERN_API RtkApiRet cva_acl_enable_ctrl_set(uint32 unit, RtkPort portNum, AclEnable aclEn, AclDefAct defAct);
EXTERN_API RtkApiRet cva_acl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType);
EXTERN_API RtkApiRet cva_acl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type);
EXTERN_API RtkApiRet cva_acl_getting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPieParam_t *pPieParam);
EXTERN_API RtkApiRet cva_acl_setting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam);
EXTERN_API RtkApiRet cva_acl_priority_pie_get(uint32 unit, AclPieBlockId pieBlock, AclPriVal *pPriVal);
EXTERN_API RtkApiRet cva_acl_priority_pie_set(uint32 unit, AclPieBlockId pieBlock, AclPriVal priVal);
EXTERN_API RtkApiRet cva_acl_reverse_getting(uint32 unit, AclRuleId aclRuleIndex, AclReverse *pIsReverse);
EXTERN_API RtkApiRet cva_acl_reverse_setting(uint32 unit, AclRuleId aclRuleIndex, AclReverse isReverse);
EXTERN_API RtkApiRet cva_acl_aggregator_get(uint32 unit, AclRuleId aclRuleIndex, AclAggOp *pAggOp);
EXTERN_API RtkApiRet cva_acl_aggregator_set(uint32 unit, AclRuleId aclRuleIndex, AclAggOp aggOp);
EXTERN_API RtkApiRet cva_acl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet cva_acl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessActParam_t *pActParam);
EXTERN_API RtkApiRet cva_acl_offset_get(uint32 unit, AclOffsetIndex offsetIndex, AclOffsetType *pOffsetType, AclOffsetPosition *pOffsetPosition);
EXTERN_API RtkApiRet cva_acl_offset_set(uint32 unit, AclOffsetIndex offsetIndex, AclOffsetType offsetType, AclOffsetPosition offsetPosition);
EXTERN_API RtkApiRet cva_acl_l4_port_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclFilterBound *pUpper, AclFilterBound *pLower);
EXTERN_API RtkApiRet cva_acl_l4_port_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, AclFilterBound upper, AclFilterBound lower);
EXTERN_API RtkApiRet cva_acl_vid_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclFilterBound *pUpper, AclFilterBound *pLower);
EXTERN_API RtkApiRet cva_acl_vid_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, AclFilterBound upper, AclFilterBound lower);
EXTERN_API RtkApiRet cva_acl_ip_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclIpv4 *pUpper, AclIpv4 *pLower);
EXTERN_API RtkApiRet cva_acl_ip_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, AclIpv4 upper, AclIpv4 lower);
EXTERN_API RtkApiRet cva_acl_pkt_len_range_get(uint32 unit, AclFilterIndex entryIdx, AclFilterBound *pUpper, AclFilterBound *pLower);
EXTERN_API RtkApiRet cva_acl_pkt_len_range_set(uint32 unit, AclFilterIndex entryIdx, AclFilterBound upper, AclFilterBound lower);
EXTERN_API RtkApiRet cva_acl_ip6_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclIpv6 *pUpper, AclIpv6 *pLower);
EXTERN_API RtkApiRet cva_acl_ip6_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, const AclIpv6 *pUpper, const AclIpv6 *pLower);
EXTERN_API RtkApiRet cva_acl_mac_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclMac *pUpper, AclMac *pLower);
EXTERN_API RtkApiRet cva_acl_mac_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, const AclMac *pUpper, const AclMac *pLower);
EXTERN_API RtkApiRet cva_acl_l4_port_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclMember *pMember);
EXTERN_API RtkApiRet cva_acl_l4_port_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, AclMember member);
EXTERN_API RtkApiRet cva_acl_vid_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclMember *pMember);
EXTERN_API RtkApiRet cva_acl_vid_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, AclMember member);
EXTERN_API RtkApiRet cva_acl_spm_member_get(uint32 unit, AclFilterIndex entryIdx, RtkPmap *pSpm);
EXTERN_API RtkApiRet cva_acl_spm_member_set(uint32 unit, AclFilterIndex entryIdx, RtkPmap spm);
EXTERN_API RtkApiRet cva_acl_ip_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclIpv4 *pMember);
EXTERN_API RtkApiRet cva_acl_ip_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, AclIpv4 member);
EXTERN_API RtkApiRet cva_acl_ip6_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclIpv6 *pMember);
EXTERN_API RtkApiRet cva_acl_ip6_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, const AclIpv6 *pMember);
EXTERN_API RtkApiRet cva_acl_mac_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclMac *pMember);
EXTERN_API RtkApiRet cva_acl_mac_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, const AclMac *pMember);
EXTERN_API RtkApiRet cva_acl_counter_entry_get(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessCounterParam_t *pCounter);
EXTERN_API RtkApiRet cva_acl_counter_entry_set(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessCounterParam_t *pCounter);
EXTERN_API RtkApiRet cva_acl_policer_entry_get(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPolicerParam_t *pPolicer);
EXTERN_API RtkApiRet cva_acl_policer_entry_set(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPolicerParam_t *pPolicer);
EXTERN_API RtkApiRet cva_acl_getting_abnormal_times_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessAbnormalTimesParam_t *pAbnormalTimesParam);
EXTERN_API RtkApiRet cva_acl_setting_abnormal_times_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessAbnormalTimesParam_t *pAbnormalTimesParam);
EXTERN_API RtkApiRet cva_acl_time_slot_en_get(uint32 unit, AclEnable *pEnable);
EXTERN_API RtkApiRet cva_acl_time_slot_en_set(uint32 unit, AclEnable enable);
EXTERN_API RtkApiRet cva_acl_time_slot_threshold_get(uint32 unit, AclThreshold *pThreshold);
EXTERN_API RtkApiRet cva_acl_time_slot_threshold_set(uint32 unit, AclThreshold threshold);
EXTERN_API RtkApiRet cva_acl_abnormal_dpm_get(uint32 unit, RtkPmap *pDpm);
EXTERN_API RtkApiRet cva_acl_abnormal_dpm_set(uint32 unit, RtkPmap dpm);
EXTERN_API RtkApiRet cva_acl_getting_timer_discard_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessTimerDiscardParam_t *pTimerDiscardParam);
EXTERN_API RtkApiRet cva_acl_setting_timer_discard_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessTimerDiscardParam_t *pTimerDiscardParam);
EXTERN_API RtkApiRet cva_acl_getting_header_dump_entry(uint32 unit, AclHeaderDumpId aclHeaderDumpIndex, AclTblAccessHeaderDumpParam_t *pHeaderDumpParam);

#endif /* CVA_RTK_API_ACL_H__ */