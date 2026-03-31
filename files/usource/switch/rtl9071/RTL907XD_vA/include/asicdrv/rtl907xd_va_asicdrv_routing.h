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

#ifndef DVA_RTK_ASICDRV_ROUTING_H__
#define DVA_RTK_ASICDRV_ROUTING_H__

/********* Include Header *********/
#include "rtkas_api_routing.h"
#include "rtl907xd_va_api.h"

/********* Definition *********/
/* Routing tables */
#define DVA_HOST_ROUTE_TABLE_MAX     (1024u)
#define DVA_RH_TABLE_MAX             (2048u)
#define DVA_OIL_TABLE_MAX            (8192u)
#define DVA_EGIF_TABLE_MAX           (160u)
#define DVA_INGIF_TABLE_MAX          (64u)
#define DVA_NETWORK_ROUTE_TABLE_MAX  (256u)
#define DVA_MULTICAST_COL_TABLE_MAX  (64u)
#define DVA_NEXT_HOP_TABLE_MAX       (4096u)
#define DVA_UC6FIR_SLOT_NUM          (2u)
#define DVA_routing_mc_tbl_size      (28u)
#define DVA_routing_arp_ndp_tbl_size (24u)
/*
    [0]: L4 learning on/off
    [1]: reset routing table
    [2:17]: host table alarm threshold
    [18:25]: network route table alarm threshold
    [26]: host table alarm
    [27]: network route table alarm
    [28]: trigger NDP resolution
 */
#define DVA_RT_L4_LEARN_CTRL (HW_MISC_DMY_RESIGSER_4)
/*
    [0:7]: Polling interval (sec)
    [8:11]: polling time to age out
 */
#define DVA_RT_L4_LEARN_AGING_CTRL            (HW_MISC_DMY_RESIGSER_12)
#define DVA_RT_L4_LEARN_ENABLE_OFFSET         (0u)
#define DVA_RT_L4_LEARN_ENABLE_LEN            (1u)
#define DVA_RT_L4_LEARN_RST_TBL_OFFSET        (1u)
#define DVA_RT_L4_LEARN_RST_TBL_LEN           (1u)
#define DVA_RT_L4_LEARN_HOST_THR_OFFSET       (2u)
#define DVA_RT_L4_LEARN_HOST_THR_LEN          (16u)
#define DVA_RT_L4_LEARN_NET_THR_OFFSET        (18u)
#define DVA_RT_L4_LEARN_NET_THR_LEN           (8u)
#define DVA_RT_L4_LEARN_HOST_ALARM_OFFSET     (26u)
#define DVA_RT_L4_LEARN_HOST_ALARM_LEN        (1u)
#define DVA_RT_L4_LEARN_NET_ALARM_OFFSET      (27u)
#define DVA_RT_L4_LEARN_NET_ALARM_LEN         (1u)
#define DVA_RT_L4_NDP_RESOL_TRIGGER_OFFSET    (28u)
#define DVA_RT_L4_NDP_RESOL_TRIGGER_LEN       (1u)
#define DVA_RT_L4_TBL_POLLING_INTERVAL_OFFSET (0u)
#define DVA_RT_L4_TBL_POLLING_INTERVAL_LEN    (12u)
#define DVA_RT_L4FMT_TCP                      (0u)
#define DVA_RT_L4FMT_UDP                      (1u)
#define DVA_RT_L4FMT_BOTH                     (2u)
#define DVA_RT_L4FMT_NOCARE                   (3u)
#define DVA_RT_L4FMT_MASK                     (0x3u)
#define DVA_RT_L4FMT_STATIC_HOST_OFFSET       (13u)
#define DVA_RT_L4FMT_MC_OFFSET                (6u)
#define DVA_RT_L4FMT_NET_OFFSET               (0u)
#define DVA_RT_UC_CFG_IP_VER_MASK             (0x1u)
#define DVA_RT_UC_CFG_FIREWALL_MASK           (0x2u)
#define DVA_RT_MC_CFG_FIREWALL_MASK           (0x2u)
#define DVA_RT_NET_DUMMY_ENTRY_NUM            (1u)
#define DVA_RT_MAX_BUCKET_NUM                 (12u)
#define DVA_RT_MAX_NETTBL_NUM                 (256u)
#define DVA_RT_MAX_MCCOLTBL_NUM               (64u)
#define DVA_RT_MAX_INTERFACE_NUM              (64u)
#define DVA_RT_MAX_VALID_UC_NUM               (4096u)
#define DVA_RT_MAX_VALID_MC_NUM               (2048u)
#define DVA_RT_MAX_HOSTTBL_NUM                (1024u)
#define DVA_RT_MAX_RHTBL_NUM                  (2048u)
#define DVA_RT_RHINDEX_MASK                   (0xFFFu)
#define DVA_RT_INVALID_IDX16                  (0xFFFFu)
#define DVA_RT_INVALID_IDX32                  (0xFFFFFFFFu)
#define DVA_RT_HOST_TBL_BOTTOM_START          (512u)
#define DVA_RT_ROUTING_MODE                   (1u)
#define DVA_RT_FILTER_MODE                    (0u)
#define DVA_HW_MISC_CTRL00                    (DVA_INTERNAL_CPU_VARIABLE22)
#define DVA_HWCTRL_ROUTING_EN                 (0x00000008u)
#define DVA_HWCTRL_ROUTING_EN_OFFSET          (3u)
#define DVA_HWCTRL_ROUTING_EN_LEN             (1u)
#define DVA_ACT_ROUTE                         (0u)
#define DVA_ACT_TRAP                          (1u)
#define DVA_ACT_COPY                          (2u)
#define DVA_ACT_DROP                          (3u)
#define DVA_ACT_RESERVED                      (4u)
#define DVA_ACT_BRIDGE                        (5u)
/* Routing HW driver */
#define DVA_L3_IP_VER                   (0u)
#define DVA_L3_IP4_TOS                  (1u)
#define DVA_L3_IP4_TOTALLEN             (2u)
#define DVA_L3_IP4_TTL                  (8u)
#define DVA_L3_IP4_PTL                  (9u)
#define DVA_L3_IP4_CHKSUM               (10u)
#define DVA_L3_IP4_SIP                  (12u)
#define DVA_L3_IP4_DIP                  (16u)
#define DVA_L3_IP6_VER                  (0u)
#define DVA_L3_IP6_PLDLEN               (4u)
#define DVA_L3_IP6_NH                   (6u)
#define DVA_L3_IP6_HL                   (7u)
#define DVA_L3_IP6_SIP                  (8u)
#define DVA_L3_IP6_DIP                  (24u)
#define DVA_L3_ICMP_CHKSUM              (2u)
#define DVA_RT_TBL_SIZE                 (256u)
#define DVA_RT_INTF_SIZE                (64u)
#define DVA_RT_CPU_INTF_IDX             (63u)
#define DVA_ROUTING_MC_TABLE_SIZE       (256u)
#define DVA_GLBCFG_RT_ATTACH_VTAG_ADDR  (0x01102bc8u)
#define DVA_GLBCFG_RT_DISABLE_ADDR      (0x01102bc9u)
#define DVA_GLBCFG_RT_MODE_ADDR         (0x01102bcau)
#define DVA_GLBCFG_RT_HW_SEARCH         (0x01102bcbu)
#define DVA_SW_RT_TBL_BASE              (0x01102bccu)
#define DVA_SW_INTF_TBL_BASE            (0x01104fccu)
#define DVA_RT_NH_POOL_ADDR             (0x011056ccu)
#define DVA_GLBCFG_FIR_EN_ADDR          (0x01102628u)
#define DVA_GLBCFG_FIR_MODE_ADDR        (0x01102629u)
#define DVA_FIR_STATEFUL_MODE_FILTER    (1u)
#define DVA_REG_FIR_GLB_CTRL            (DVA_HW_MISC_DMY4)
#define DVA_REG_FIR_GLB_CTRL_RST_OFFSET (0u)
#define DVA_LUT_RESERVED_FID            (15u)
#define DVA_DFT_INTF_L2_START_IDX       (255u)
/* ICMP Header */
#define DVA_ICMPTYPE   (0x22 + CPU_TAG_LEN)
#define DVA_ICMPCODE   (0x23 + CPU_TAG_LEN)
#define DVA_ICMPCHKSUM (0x24 + CPU_TAG_LEN)
#define DVA_ICMPQUENCH (0x26 + CPU_TAG_LEN)
#define DVA_ICMPDATA   (0x2A + CPU_TAG_LEN)
/* ICMPv6 Header */
#define DVA_ICMP6TYPE       (0x36 + CPU_TAG_LEN)
#define DVA_ICMP6CODE       (0x37 + CPU_TAG_LEN)
#define DVA_ICMP6CHKSUM     (0x38 + CPU_TAG_LEN)
#define DVA_ICMP6TAGADDR    (0x3E + CPU_TAG_LEN)
#define DVA_ICMP6SRCMACADDR (0x50 + CPU_TAG_LEN)
#define DVA_ICMP6END        (0x56 + CPU_TAG_LEN)
/* IP Header */
#define DVA_IP4_NORMAL_HL                (20u)
#define DVA_IP6_NORMAL_HL                (40u)
#define DVA_ICMP_NORMAL_HL               (8u)
#define DVA_ICMP_NORMAL_HL               (8u)
#define DVA_BITS_PER_DATA                (68u)
#define DVA_BITS_PER_GROUP_DATA          (40u)
#define DVA_OCCUPIED_SLOT_NUM_UC4        (1u)
#define DVA_OCCUPIED_SLOT_NUM_UC4FIR     (2u)
#define DVA_OCCUPIED_SLOT_NUM_UC6        (3u)
#define DVA_OCCUPIED_SLOT_NUM_UC6FIR     (6u)
#define DVA_OCCUPIED_SLOT_NUM_MC4        (3u)
#define DVA_OCCUPIED_SLOT_NUM_MC4FIR     (3u)
#define DVA_OCCUPIED_SLOT_NUM_MC6        (6u)
#define DVA_OCCUPIED_SLOT_NUM_MC6FIR     (6u)
#define DVA_TYPE_UC4                     (0u)
#define DVA_TYPE_MC4                     (1u)
#define DVA_TYPE_UC6                     (2u)
#define DVA_TYPE_MC6                     (3u)
#define DVA_TYPE_UC4FIR                  (4u)
#define DVA_TYPE_MC4FIR                  (5u)
#define DVA_TYPE_UC6FIR                  (6u)
#define DVA_TYPE_MC6FIR                  (7u)
#define DVA_IGRIF_SEL_FID                (0u)
#define DVA_IGRIF_SEL_VID                (1u)
#define DVA_IGRIF_ACT_BRIDGE             (0u)
#define DVA_IGRIF_ACT_COPY               (1u)
#define DVA_IGRIF_ACT_TRAP               (2u)
#define DVA_IGRIF_ACT_ROUTE              (3u)
#define DVA_IGRIF_MC_LU_MISS_ACT_DROP    (0u)
#define DVA_IGRIF_MC_LU_MISS_ACT_TRAP    (1u)
#define DVA_IGRIF_MC_LU_MISS_ACT_BRIDGE  (2u)
#define DVA_RT_MAX_SLOT_NUM              (12u)
#define DVA_RT_MAX_UC4_ENTRY_NUM         (DVA_RT_MAX_SLOT_NUM)
#define DVA_RT_MAX_UC4FIR_ENTRY_NUM      (6u)
#define DVA_RT_MAX_MC4_ENTRY_NUM         (4u)
#define DVA_RT_MAX_MC4FIR_ENTRY_NUM      (4u)
#define DVA_RT_MAX_UC6_ENTRY_NUM         (4u)
#define DVA_RT_MAX_UC6FIR_ENTRY_NUM      (2u)
#define DVA_RT_MAX_MC6_ENTRY_NUM         (2u)
#define DVA_RT_MAX_MC6FIR_ENTRY_NUM      (2u)
#define DVA_RT_MAX_OIL_NUM               (8192u)
#define DVA_RT_HASH0_BUF_SIZE            (34u)
#define DVA_RT_HASH1_BUF_SIZE            (27u)
#define DVA_RT_DFT_MTU_MAX_SIZE          (2048u)
#define DVA_RT_RH_TBL_RESERVED_ENTRY_NUM (2u)
#define DVA_OIL_VALID_FLAG               (0x2000u)
#define DVA_RT_UC                        (0u)
#define DVA_RT_MC                        (1u)
#define DVA_MAC_FMT_SA                   (0u)
#define DVA_MAC_FMT_DA                   (1u)
#define DVA_MTU_IDX_IPV6                 (0x00FFu)
#define DVA_MTU_IDX_IPV4                 (0xFF00u)
#define DVA_INDR_NUM_RESERVED            (6u)
#define DVA_INDR_NUM_RESERVED_NET        (7u)
#define DVA_L2_BCAM_VALID                (0x1000u)
#define DVA_NH_POOL_SIZE                 (DVA_NEXT_HOP_TABLE_MAX / 32u * 4u)
#define DVA_RT_INIT_WAIT_TIMEOUT         (1000u)
#define DVA_RT_CLEAR_IGRIF_TBL           (0x1u)  /* bit 0 */
#define DVA_RT_CLEAR_HOST_TBL            (0x2u)  /* bit 1 */
#define DVA_RT_CLEAR_NET_TBL             (0x4u)  /* bit 2 */
#define DVA_RT_CLEAR_MCCOL_TBL           (0x8u)  /* bit 3 */
#define DVA_RT_CLEAR_RH_TBL              (0x10u) /* bit 4 */
#define DVA_RT_CLEAR_NH_TBL              (0x20u) /* bit 5 */
#define DVA_RT_CLEAR_OIL_TBL             (0x40u) /* bit 6 */
#define DVA_RT_CLEAR_EGRIF_TBL           (0x80u) /* bit 7 */
#define DVA_RT_CLEAR_L3_TBL              (DVA_RT_CLEAR_IGRIF_TBL | DVA_RT_CLEAR_HOST_TBL | DVA_RT_CLEAR_NET_TBL | DVA_RT_CLEAR_MCCOL_TBL | DVA_RT_CLEAR_RH_TBL | DVA_RT_CLEAR_NH_TBL | DVA_RT_CLEAR_EGRIF_TBL)
#define DVA_VID_MASK                     (0xFFFu)
#define DVA_RT_SLOT_VLAID_MASK           (0xFFFu)
#define DVA_VLAN_NUM_MAX                 (4096u)
#define DVA_QUCIK_SEARCH_TIMEOUT         (5u) /* 5 usecond */
#define DVA_HOST_ENTRY_VALID_MASK        (0x1u)
#define DVA_HOST_ENTRY_TYPE_OFFSET       (0x1u)
#define DVA_HOST_ENTRY_TYPE_MASK         (0x7u)
#define DVA_RT_HOST_BTM_IDX_OFFSET       (512u)
#define DVA_CB_EGR_ENTRY_NUM             (96u)
#define DVA_OIL_IDX_MASK                 (0x1FFFu)
#define ROUTING_DRV_TBL_CLEAR_MAX_US     (120u)
#define ROUTING_DRV_TBL_CLEAR_MIN_US     (1u)
#define ROUTING_DRV_QUICK_SEARCH_MAX_US  (10u)
#define ROUTING_DRV_QUICK_SEARCH_MIN_US  (1u)
#define ROUTING_DRV_INIT_MAX_US          (RT_INIT_TIMEOUT)
#define ROUTING_DRV_INIT_MIN_US          (1u)
#define DVA_RT_STATIC_MODE               (0u)
#define DVA_RT_DYNAMIC_MODE              (1u)

/********* Enumeration *********/

/********* Structure *********/
typedef struct
{
    uint32 val[9]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} dva_RtRhTbl_t;

/*
    Outgoing Interface List Table
*/
typedef struct
{
    uint32 val[1];
} dva_tblAccess_OIL_TABLE_t;

/*
    Egress IF Table
*/
typedef struct
{
    uint32 val[3];
} dva_RtEgIfTbl_t;

/*
    Ingress Interface Table
*/
typedef struct
{
    uint32 val[9]; /* DATAREG */
} dva_RtIngIfTbl_t;

typedef struct
{
    uint8  valid;
    uint8  portId; /* On 1st lookup round, Become an IGRIF compared key if (portIdCare == 1s) */
    uint16 ivid;   /* On 1st lookup round, Become an IGRIF compared key if (intfIdVidKeySel == 1) & (ividCare == 1s) */
    uint8  intfIdVidKeySel;
    uint16 intfId;     /* On 1st lookup round, Become an IGRIF compared key if (intfIdVidKeySel == 0) */
    uint16 macId15to0; /* On 1st lookup round, Become an IGRIF compared key if (MACID_xx_xx_Care == 1s) */
    uint32 macId47to16;
    uint16 sp;       /* On 1st lookup round, Become an IGRIF compared key if (spCare == 1s) */
    uint16 dp;       /* On 1st lookup round, Become an IGRIF compared key if (dpCare == 1s) */
    uint8  spFmt;    /* On 2nd lookup round, Become an IGRIF compared key if (firMode == 1) */
                     /* Moreover, While =3, assign SP=0 for HOST rtk_route_hashx_ip_uc_idx_get */
    uint8 dpFmt;     /* On 2nd lookup round, Become an IGRIF compared key if (firMode == 1) */
                     /* Moreover, While =3, assign DP=0 for HOST rtk_route_hashx_ip_uc_idx_get */
    uint8 dipNoCare; /* DIP_DONT_CARE, While =1, assign DIP=0 for HOST rtk_route_hashx_ip_uc_idx_get */
    uint8 sipNoCare; /* SIP_DONT_CARE, While =1, assign SIP=0 for HOST rtk_route_hashx_ip_uc_idx_get */
    uint8 vidNoCare; /* VID_DONT_CARE, While =1, assign vid=0 for HOST rtk_route_hashx_ip_uc_idx_get */
    uint8 act;       /* 0:route 1:tp2cpu 2:cp2cpu 3:drop */
    uint8 ipUcRouteEn;
    uint8 ip6UcRouteEn;
    uint8 ipMcRouteEn;
    uint8 ip6McRouteEn;
    uint8 ipMcLuMisAct;
    uint8 ip6McLuMisAct;
    uint8 ipUrpfFailAct;
    uint8 ipUrpfChkEn;
    uint8 ipUrpfDftRouteEn;
    uint8 ipUrpfChkMode;
    uint8 ip6UrpfFailAct;
    uint8 ip6UrpfChkEn;
    uint8 ip6UrpfDftRouteEn;
    uint8 ip6UrpfChkMode;
    uint8 firMode;
    uint8 cmp0Gip; /* MCCOL_CMP_MODE */
    /* Care bits */
    uint8  portIdCare;
    uint16 ividCare;
    uint16 macId15to0Care;
    uint32 macId47to16Care;
    uint16 spCare;
    uint16 dpCare;
} dva_RtIngIfTblParm_t;

/*
    L3 Host Route Lookup Table
*/
typedef struct
{
    uint32 val[26]; /* DATAREG */
} dva_RtHostTbl_t;

/* IPv4 UC */
typedef struct
{
    uint8  valid;
    uint8  type;
    uint8  qosPri;
    uint8  qosEn;
    uint8  ttlChk;
    uint8  ttlDec;
    uint8  dstNullIntf;
    uint8  hit;
    uint8  act;
    uint16 nhIdx;
    uint8  spFmt;
    uint8  dpFmt;
    uint8  arbValid; /* 0:origin 1:read arbMode */
    uint8  arbMode;  /* 0:route action 1:normal action */
    uint32 ipv4Dip;
} dva_RtHostTblUc4Parm_t;

/* IPv4 UC with SIP/sp/dp */
typedef struct
{
    uint8  valid;
    uint8  type;
    uint8  qosPri;
    uint8  qosEn;
    uint8  ttlChk;
    uint8  ttlDec;
    uint8  dstNullIntf;
    uint8  hit;
    uint8  act;
    uint16 nhIdx;
    uint8  spFmt;
    uint8  dpFmt;
    uint8  arbValid;
    uint8  arbMode;
    uint32 ipv4Dip;
    uint32 ipv4Sip;
    uint16 sp;
    uint16 dp;
} dva_RtHostTblUc4FirParm_t;

/* IPv6 UC */
typedef struct
{
    uint8  valid;
    uint8  type;
    uint8  qosPri;
    uint8  qosEn;
    uint8  ttlChk;
    uint8  ttlDec;
    uint8  dstNullIntf;
    uint8  hit;
    uint8  act;
    uint16 nhIdx;
    uint8  spFmt;
    uint8  dpFmt;
    uint8  arbValid;
    uint8  arbMode;
    uint32 ipv6Dip31to0;
    uint32 ipv6Dip63to32;
    uint32 ipv6Dip95to64;
    uint32 ipv6Dip127to96;
} dva_RtHostTblUc6Parm_t;

/* IPv6 UC with SIP/sp/dp */
typedef struct
{
    uint8  valid;
    uint8  type;
    uint8  qosPri;
    uint8  qosEn;
    uint8  ttlChk;
    uint8  ttlDec;
    uint8  dstNullIntf;
    uint8  hit;
    uint8  act;
    uint16 nhIdx;
    uint8  spFmt;
    uint8  dpFmt;
    uint8  arbValid;
    uint8  arbMode;
    uint16 sp;
    uint16 dp;
    uint32 ipv6Dip31to0;
    uint32 ipv6Dip63to32;
    uint32 ipv6Dip95to64;
    uint32 ipv6Dip127to96;
    uint32 ipv6Sip31to0;
    uint32 ipv6Sip63to32;
    uint32 ipv6Sip95to64;
    uint32 ipv6Sip127to96;
} dva_RtHostTblUc6FirParm_t;

/* IPv4 MC */
typedef struct
{
    uint8       valid;
    uint8       type;
    uint8       qosPri;
    uint8       qosEn;
    uint8       ttlChk;
    uint8       ttlDec;
    uint8       dstNullIntf;
    uint8       hit;
    uint8       act;
    uint16      rhIdx;
    uint8       spFmt;
    uint8       dpFmt;
    uint8       arbValid;
    uint8       arbMode;
    uint32      ipv4Dip;
    uint32      ipv4Sip;
    dva_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin; /* If ttl < ttlMin, copy to cpu */
    uint16      mtuMax;
} dva_RtHostTblMc4Parm_t;

/* IPv4 MC with sp/dp */
typedef struct
{
    uint8       valid;
    uint8       type;
    uint8       qosPri;
    uint8       qosEn;
    uint8       ttlChk;
    uint8       ttlDec;
    uint8       dstNullIntf;
    uint8       hit;
    uint8       act;
    uint16      rhIdx;
    uint8       spFmt;
    uint8       dpFmt;
    uint8       arbValid;
    uint8       arbMode;
    uint32      ipv4Dip;
    uint32      ipv4Sip;
    uint16      sp;
    uint16      dp;
    dva_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin; /* If ttl < ttlMin, copy to cpu */
    uint16      mtuMax;
} dva_RtHostTblMc4FirParm_t;

/* IPv6 MC */
typedef struct
{
    uint8       valid;
    uint8       type;
    uint8       qosPri;
    uint8       qosEn;
    uint8       ttlChk;
    uint8       ttlDec;
    uint8       dstNullIntf;
    uint8       hit;
    uint8       act;
    uint16      rhIdx;
    uint8       spFmt;
    uint8       dpFmt;
    uint8       arbValid;
    uint8       arbMode;
    uint32      ipv6Dip31to0;
    uint32      ipv6Dip63to32;
    uint32      ipv6Dip95to64;
    uint32      ipv6Dip127to96;
    uint32      ipv6Sip31to0;
    uint32      ipv6Sip63to32;
    uint32      ipv6Sip95to64;
    uint32      ipv6Sip127to96;
    dva_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin;
    uint16      mtuMax;
} dva_RtHostTblMc6Parm_t;

/* IPv6 MC with sp/dp */
typedef struct
{
    uint8       valid;
    uint8       type;
    uint8       qosPri;
    uint8       qosEn;
    uint8       ttlChk;
    uint8       ttlDec;
    uint8       dstNullIntf;
    uint8       hit;
    uint8       act;
    uint16      rhIdx;
    uint8       spFmt;
    uint8       dpFmt;
    uint8       arbValid;
    uint8       arbMode;
    uint16      sp;
    uint16      dp;
    uint32      ipv6Dip31to0;
    uint32      ipv6Dip63to32;
    uint32      ipv6Dip95to64;
    uint32      ipv6Dip127to96;
    uint32      ipv6Sip31to0;
    uint32      ipv6Sip63to32;
    uint32      ipv6Sip95to64;
    uint32      ipv6Sip127to96;
    dva_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin;
    uint16      mtuMax;
} dva_RtHostTblMc6FirParm_t;

/*
    NETWORK ROUTE Table
*/
typedef struct
{
    uint32 val[23]; /* DATAREG */
} dva_RtNetRouteTbl_t;

typedef struct
{
    uint32 ipv6Sip31to0;
    uint32 ipv6Sip63to32;
    uint32 ipv6Sip95to64;
    uint32 ipv6Sip127to96;
    uint32 ipv6Dip31to0;
    uint32 ipv6Dip63to32;
    uint32 ipv6Dip95to64;
    uint32 ipv6Dip127to96;
    uint16 sp;
    uint16 dp;
    uint8  valid;
    uint8  type;
    uint8  hit;
    uint16 nhIdx;
    uint8  hostRoute;
    uint8  dftRoute;
    uint8  dstNullIntf;
    uint8  spFmt;
    uint8  dpFmt;
    uint8  act;
    uint8  ttlDec;
    uint8  ttlChk;
    uint8  qosEn;
    uint8  qosPri;
    uint8  eppIdx;
    uint16 vid;
    uint8  arbValid;
    uint8  arbMode;
    uint32 ipv6Sip31to0Care;
    uint32 ipv6Sip63to32Care;
    uint32 ipv6Sip95to64Care;
    uint32 ipv6Sip127to96Care;
    uint32 ipv6Dip31to0Care;
    uint32 ipv6Dip63to32Care;
    uint32 ipv6Dip95to64Care;
    uint32 ipv6Dip127to96Care;
    uint16 spCare;
    uint16 dpCare;
    uint8  typeCare;
    uint8  spFmtCare;
    uint8  dpFmtCare;
    uint16 vidCare;
} dva_RtNetRouteTblParm_t;

/*
    MULTICAST COLLISION  Table
*/
typedef struct
{
    uint32 val[12]; /* DATAREG */
} dva_RtMcColTbl_t;

typedef struct
{
    uint32      ipv6Sip31to0;
    uint32      ipv6Sip63to32;
    uint32      ipv6Sip95to64;
    uint32      ipv6Sip127to96;
    uint32      ipv6Dip31to0;
    uint32      ipv6Dip63to32;
    uint32      ipv6Dip95to64;
    uint32      ipv6Dip127to96;
    uint16      sp;
    uint16      dp;
    uint8       valid;
    uint8       type;
    uint8       spFmt;
    uint8       dpFmt;
    uint8       hit;
    uint16      rhIdx;
    uint8       act;
    uint16      ttlMin;
    dva_RtkPmap dpm;
    uint16      mtuMax;
    uint8       rpfChkEn;
    uint8       qosEn;
    uint8       qosPri;
    uint8       rpfFailAct;
    uint16      vid;
    uint8       arbValid;
    uint8       arbMode;
} dva_RtMcColTblParm_t;

/*
    NEXT HOP Table
*/
typedef struct
{
    uint32 val[1];
} dva_RtNhTbl_t;

typedef struct
{
    uint8  egrIfIdx;
    uint16 l2Idx;
} dva_RtNhTblParm_t;

/*
    HOST DVA_GROUP Table
*/
typedef struct
{
    uint32 val[18]; /* DATAREG */
} dva_RtHostGrpTbl_t;

typedef struct
{
    uint8  sipGrp;
    uint8  dipGrp;
    uint8  spGrp;
    uint8  dpGrp;
    uint16 vid;
    uint8  aclBlkIdx0; /* 5 bits, [4]=0x1 valid, [3:0]=blk_idx */
    uint8  aclBlkIdx1;
    uint8  aclBlkIdx4;
    uint8  aclBlkIdx5;
    uint8  aclBlkIdx8;
    uint8  aclBlkIdx9;
    uint8  aclBlkIdx12;
    uint8  aclBlkIdx13;
} dva_RtHostGrpTblParm_t;

typedef struct
{
    uint16 intFidIvid;
    uint8  mac[DVA_MAC_ADDR_LEN];
    uint8  gwIp[IPV6_ADDR_LEN];
    uint8  valid;
    uint8  physicalPn;
    uint8  ipVer;
} dva_RtFwIntfTbl_t;

typedef struct
{
    uint8  valid;                 /**< Valid bit (DVA_DISABLED to ENABLED) */
    uint8  firewall;              /**< Firewall mode (DVA_DISABLED to ENABLED) */
    uint16 intfFidVid;            /**< VID (0 to 4095) */
    uint8  mac[DVA_MAC_ADDR_LEN]; /**< Array of interface MAC (0x0 to 0xFF for 16 elements) */
    uint8  gwIp[IPV6_ADDR_LEN];   /**< Array of gateway IP (0x0 to 0xFF for 16 elements) */
    uint16 mtuIdx;                /**< MTU index, [7:0]: IPv6 [15:8]: IPv4 (0xFF00, 0x00FF) */
    uint16 sp;                    /**< TCP/UDP source port (0x0 to 0xFFFF) */
    uint16 dp;                    /**< TCP/UDP destination port (0x0 to 0xFFFF) */
    uint8  portId;                /**< physical port (0x0 to 0xFF) */
    uint8  index;                 /**< Interface table index (0 to 63) */
    uint8  l4Fmt;                 /**< Layer 4 header format (DVA_RT_L4FMT_TCP to DVA_RT_L4FMT_NOCARE) */
    uint8  ipVer;                 /**< IP version (IP4_VER, IP6_VER) */
} dva_RtIntfTbl_t;

typedef struct
{
    uint8 destIpAddr[IPV6_ADDR_LEN];
    uint8 nextHopIp[IPV6_ADDR_LEN];
    uint8 subnetMask;
    uint8 nhIntfId;
    uint8 ipVer;
    uint8 valid;
} dva_RtFwRtTbl_t;

typedef struct
{
    uint32 topIdx;
    uint32 btmIdx;
    uint32 topValidMsk;
    uint32 btmValidMsk;
    uint32 found;
} dva_RtHwSearchInfo_t;

typedef struct
{
    uint8  port;
    uint32 portEgrIfList[15][2];
    uint8  ttl;
} dva_RtOilListInfo_t;

typedef struct
{
    uint8             valid[RT_HOST_ENTRY_MAX_SIZE];
    RtHostEntryType_e type[RT_HOST_ENTRY_MAX_SIZE];
} dva_RtHostValidType_t;

/********* Function Declaration *********/
EXTERN_API RtkApiRet dva_drv_rt_route_mode_set(uint32 unit, uint8 *rtMode);
EXTERN_API RtkApiRet dva_drv_rt_sw_intf_clear(uint32 unit);
EXTERN_API RtkApiRet dva_drv_rt_rttbl_clear(uint32 unit);
EXTERN_API RtkApiRet dva_drv_rt_nhpool_clear(uint32 unit);
EXTERN_API RtkApiRet dva_drv_rt_rttbl_param_chk(const RtTbl_t *rtEntry);
EXTERN_API RtkApiRet dva_drv_rt_rttbl_sdk_to_fw(const RtTbl_t *rtEntry, dva_RtFwRtTbl_t *fwRtEntry);
EXTERN_API RtkApiRet dva_drv_rt_rttbl_fw_to_sdk(const dva_RtFwRtTbl_t *fwRtEntry, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet dva_drv_rt_intf_param_chk_runtime(const RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet dva_drv_rt_hw_igr_intf_set_runtime(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet dva_drv_rt_hw_egr_intf_set(uint32 unit, RtIntfTbl_t *intfEntry);
EXTERN_API RtkApiRet dva_drv_rt_sw_intf_set(uint32 unit, const RtIntfTbl_t *intfEntry);
EXTERN_API RtkApiRet dva_drv_rt_sw_intf_get(uint32 unit, RtIntfTbl_t *intfEntry);
EXTERN_API RtkApiRet dva_drv_rt_hw_igr_intf_get(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet dva_drv_rt_hw_egr_intf_get(uint32 unit, RtIntfTbl_t *intfEntry);
EXTERN_API RtkApiRet dva_drv_rt_host_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet dva_drv_rt_net_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet dva_drv_rt_mccol_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet dva_drv_rt_subnet_set(uint32 unit, RtTbl_t *rtEntry, uint32 index);
EXTERN_API RtkApiRet dva_drv_rt_nhtbl_avail_get_runtime(uint32 unit, uint16 *nhIdx);
EXTERN_API RtkApiRet dva_drv_rt_uc_host_tbl_entry_runtime_set(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dva_drv_rt_net_tbl_entry_set(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dva_drv_rt_nhtbl_avail_set_runtime(uint32 unit, uint16 nhIdx, uint8 valid);
EXTERN_API RtkApiRet dva_drv_rt_uc_host_tbl_entry_get(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dva_drv_rt_net_tbl_entry_get(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dva_drv_rt_mc_host_tbl_entry_runtime_set(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dva_drv_rt_mc_col_tbl_entry_set(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dva_drv_rt_mc_host_tbl_entry_get(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dva_drv_rt_mc_col_tbl_entry_get(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dva_drv_host_raw_get(uint32 unit, uint16 entryNum, dva_RtHostTbl_t *hostTblEntry);
EXTERN_API RtkApiRet dva_drv_host_entry_type_valid_get(const dva_RtHostTbl_t *hostTblEntry, dva_RtHostValidType_t *entryValidType);
EXTERN_API RtkApiRet dva_drv_host_entry_typelist_get(const dva_RtHostValidType_t *entryValidType, RtHostTypeList_t *hostTypeList);
EXTERN_API RtkApiRet dva_drv_filter_mode_set(uint32 unit, uint8 rtDisable);
EXTERN_API bool      dva_drv_mc_both_ip_exist(const uint8 *sip, const uint8 *dip);
EXTERN_API RtkApiRet dva_drv_rt_disable_process(uint32 unit, uint8 disable);
#endif /* DVA_RTK_ASICDRV_ROUTING_H__ */