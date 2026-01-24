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

#ifndef CVB_ASICDRV_ROUTING_H_
#define CVB_ASICDRV_ROUTING_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_api_routing.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtl907xc_vb_asicdrv.h"

/* Routing HW driver */
#define CVB_L3_IP_VER                  (0u)
#define CVB_L3_IP4_TOS                 (1u)
#define CVB_L3_IP4_TOTALLEN            (2u)
#define CVB_L3_IP4_TTL                 (8u)
#define CVB_L3_IP4_PTL                 (9u)
#define CVB_L3_IP4_CHKSUM              (10u)
#define CVB_L3_IP4_SIP                 (12u)
#define CVB_L3_IP4_DIP                 (16u)
#define CVB_L3_IP6_VER                 (0u)
#define CVB_L3_IP6_PLDLEN              (4u)
#define CVB_L3_IP6_NH                  (6u)
#define CVB_L3_IP6_HL                  (7u)
#define CVB_L3_IP6_SIP                 (8u)
#define CVB_L3_IP6_DIP                 (24u)
#define CVB_L3_ICMP_CHKSUM             (2u)
#define CVB_RT_TBL_SIZE                (256u)
#define CVB_RT_INTF_SIZE               (64u)
#define CVB_ROUTING_MC_TABLE_SIZE      (256u)
#define CVB_GLBCFG_RT_DISABLE_ADDR     (0x001a254cu)
#define CVB_GLBCFG_RT_FIR_MODE_ADDR    (0x001a2550u)
#define CVB_GLBCFG_RT_EXT_GLB_CFG_ADDR (0x001a254cu)
#define CVB_SW_INTF_TBL_BASE           (0x001a255au)
#define CVB_SW_RT_TBL_BASE             (0x001a2e5au)
#define CVB_NH_POOL_ADDR               (0x001aa67cu)
#define CVB_LUT_RESERVED_FID           (15u)
#define CVB_CB_EGR_ENTRY_NUM           (96u)
/* ICMP Header */
#define CVB_ICMPTYPE   (0x22 + CPU_TAG_LEN)
#define CVB_ICMPCODE   (0x23 + CPU_TAG_LEN)
#define CVB_ICMPCHKSUM (0x24 + CPU_TAG_LEN)
#define CVB_ICMPQUENCH (0x26 + CPU_TAG_LEN)
#define CVB_ICMPDATA   (0x2A + CPU_TAG_LEN)
/* ICMPv6 Header */
#define CVB_ICMP6TYPE       (0x36 + CPU_TAG_LEN)
#define CVB_ICMP6CODE       (0x37 + CPU_TAG_LEN)
#define CVB_ICMP6CHKSUM     (0x38 + CPU_TAG_LEN)
#define CVB_ICMP6TAGADDR    (0x3E + CPU_TAG_LEN)
#define CVB_ICMP6SRCMACADDR (0x50 + CPU_TAG_LEN)
#define CVB_ICMP6END        (0x56 + CPU_TAG_LEN)
/* IP Header */
#define CVB_IP4_NORMAL_HL                    (20u)
#define CVB_IP6_NORMAL_HL                    (40u)
#define CVB_ICMP_NORMAL_HL                   (8u)
#define CVB_ICMP_NORMAL_HL                   (8u)
#define CVB_BITS_PER_DATA                    (68u)
#define CVB_BITS_PER_GROUP_DATA              (40u)
#define CVB_OCCUPIED_SLOT_NUM_UC4            (1u)
#define CVB_OCCUPIED_SLOT_NUM_UC4FIR         (2u)
#define CVB_OCCUPIED_SLOT_NUM_UC6            (3u)
#define CVB_OCCUPIED_SLOT_NUM_UC6FIR         (6u)
#define CVB_OCCUPIED_SLOT_NUM_MC4            (3u)
#define CVB_OCCUPIED_SLOT_NUM_MC4FIR         (3u)
#define CVB_OCCUPIED_SLOT_NUM_MC6            (6u)
#define CVB_OCCUPIED_SLOT_NUM_MC6FIR         (6u)
#define CVB_TYPE_UC4                         (0u)
#define CVB_TYPE_MC4                         (1u)
#define CVB_TYPE_UC6                         (2u)
#define CVB_TYPE_MC6                         (3u)
#define CVB_TYPE_UC4FIR                      (4u)
#define CVB_TYPE_MC4FIR                      (5u)
#define CVB_TYPE_UC6FIR                      (6u)
#define CVB_TYPE_MC6FIR                      (7u)
#define CVB_IGRIF_SEL_FID                    (0u)
#define CVB_IGRIF_SEL_VID                    (1u)
#define CVB_IGRIF_ACT_BRIDGE                 (0u)
#define CVB_IGRIF_ACT_COPY                   (1u)
#define CVB_IGRIF_ACT_TRAP                   (2u)
#define CVB_IGRIF_ACT_ROUTE                  (3u)
#define CVB_IGRIF_MC_LU_MISS_ACT_DROP        (0u)
#define CVB_IGRIF_MC_LU_MISS_ACT_TRAP        (1u)
#define CVB_IGRIF_MC_LU_MISS_ACT_BRIDGE      (2u)
#define CVB_RT_MAX_SLOT_NUM                  (12u)
#define CVB_RT_MAX_UC4_ENTRY_NUM             (CVB_RT_MAX_SLOT_NUM)
#define CVB_RT_MAX_UC4FIR_ENTRY_NUM          (6u)
#define CVB_RT_MAX_MC4_ENTRY_NUM             (4u)
#define CVB_RT_MAX_MC4FIR_ENTRY_NUM          (4u)
#define CVB_RT_MAX_UC6_ENTRY_NUM             (4u)
#define CVB_RT_MAX_UC6FIR_ENTRY_NUM          (2u)
#define CVB_RT_MAX_MC6_ENTRY_NUM             (2u)
#define CVB_RT_MAX_MC6FIR_ENTRY_NUM          (2u)
#define CVB_RT_HOST_ENTRY_SIZE_MAX           (408u)                       /* bit */
#define CVB_RT_HOST_TBL_ENTRY_SIZE_MAX       (816u)                       /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_UC4           (68u)                        /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_UC4FIR        (136u)                       /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_UC6           (204u)                       /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_UC6FIR        (408u)                       /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_MC4           (204u)                       /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_MC4FIR        (204u)                       /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_MC6           (CVB_RT_HOST_ENTRY_SIZE_MAX) /* bit */
#define CVB_RT_HOST_ENTRY_SIZE_MC6FIR        (CVB_RT_HOST_ENTRY_SIZE_MAX) /* bit */
#define CVB_RT_HOST_GRP_TBL_SLOT_SIZE_UC4FIR (80u)                        /* bit */
#define CVB_RT_HOST_GRP_TBL_SLOT_SIZE_UC6FIR (240u)                       /* bit */
#define CVB_RT_HOST_GRP_TBL_SLOT_SIZE_MC4FIR (120u)                       /* bit */
#define CVB_RT_HOST_GRP_TBL_SLOT_SIZE_MC6FIR (240u)                       /* bit */
#define CVB_RT_MAX_OIL_NUM                   (8192u)
#define CVB_RT_HASH0_BUF_SIZE                (34u)
#define CVB_RT_HASH1_BUF_SIZE                (27u)
#define CVB_RT_DFT_MTU_MAX_SIZE              (2048u)
#define CVB_RT_RH_TBL_RESERVED_ENTRY_NUM     (2u)
#define CVB_OIL_VALID_FLAG                   (0x2000u)
#define CVB_OIL_IDX_MASK                     (0x1FFFu)
#define CVB_RT_UC                            (0u)
#define CVB_RT_MC                            (1u)
#define CVB_MAC_FMT_SA                       (0u)
#define CVB_MAC_FMT_DA                       (1u)
#define CVB_MTU_IDX_IPV6                     (0x00FFu)
#define CVB_MTU_IDX_IPV4                     (0xFF00u)
#define CVB_INDR_NUM_RESERVED                (6u)
#define CVB_L2_BCAM_VALID                    (0x1000u)
#define CVB_NH_POOL_SIZE                     (CVB_NEXT_HOP_TABLE_MAX / 32u * 4u)
#define CVB_RT_INIT_WAIT_TIMEOUT             (1000u)
#define CVB_RT_CLEAR_IGRIF_TBL               (0x1u)  /* bit 0 */
#define CVB_RT_CLEAR_HOST_TBL                (0x2u)  /* bit 1 */
#define CVB_RT_CLEAR_NET_TBL                 (0x4u)  /* bit 2 */
#define CVB_RT_CLEAR_MCCOL_TBL               (0x8u)  /* bit 3 */
#define CVB_RT_CLEAR_RH_TBL                  (0x10u) /* bit 4 */
#define CVB_RT_CLEAR_NH_TBL                  (0x20u) /* bit 5 */
#define CVB_RT_CLEAR_OIL_TBL                 (0x40u) /* bit 6 */
#define CVB_RT_CLEAR_EGRIF_TBL               (0x80u) /* bit 7 */
#define CVB_RT_CLEAR_L3_TBL                  (CVB_RT_CLEAR_IGRIF_TBL | CVB_RT_CLEAR_HOST_TBL | CVB_RT_CLEAR_NET_TBL | CVB_RT_CLEAR_MCCOL_TBL | CVB_RT_CLEAR_RH_TBL | CVB_RT_CLEAR_NH_TBL | CVB_RT_CLEAR_EGRIF_TBL)
#define CVB_QUCIK_SEARCH_TIMEOUT             (5u) /* 5 usecond */
#define CVB_VID_MASK                         (0xFFFu)
#define CVB_RT_LOOKUP_HIT                    (1u)
#define CVB_RT_SLOT_VLAID_MASK               (0xFFFu)
#define CVB_RT_HOST_BTM_IDX_OFFSET           (512u)
#define CVB_RT_HOST_TBL_INDR_NUM             (27u) /* g_rtkasTableList[CVB_HOST_ROUTE_TABLE].dataregNum */
#define CVB_RT_HOST_GRP_TBL_INDR_NUM         (16u) /* g_rtkasTableList[CVB_HOST_GROUP_TABLE].dataregNum */
#define CVB_HOST_ENTRY_VALID_MASK            (0x1u)
#define CVB_HOST_ENTRY_TYPE_OFFSET           (0x1u)
#define CVB_HOST_ENTRY_TYPE_MASK             (0x7u)

typedef struct
{
    uint32 val[9]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} cvb_RtRhTbl_t;

typedef struct
{
    uint16 portRhIdx[15];
} cvb_RtRhTblParm_t;

/*
    Outgoing Interface List Table
*/
typedef struct
{
    uint32 val[1];
} cvb_tblAccess_OIL_TABLE_t;

typedef struct
{
    uint8  ttlChk;
    uint8  ttlDec;
    uint8  egrIfIdx;
    uint16 oilNext;
} cvb_RtOilTblParm_t;

/*
    Egress IF Table
*/
typedef struct
{
    uint32 val[3];
} cvb_RtEgIfTbl_t;

typedef struct
{
    uint32 macId31to0;
    uint16 macId47to32;
    uint8  macFmtSel;
    uint8  macReplace;
    uint16 vid;
    uint8  vidReplace;
    uint8  pcp;
    uint8  pcpReplace;
    uint8  ipMtuIdx;
    uint8  ip6MtuIdx;
    uint8  ipMcTtl;
    uint8  ip6McHl;
    uint8  icmpRedirChkEn;
    uint8  icmpRedirAct;
    uint8  rpfChkEn;
    uint8  rpfFailAct;
} cvb_RtEgIfTblParm_t;

/*
    Ingress Interface Table
*/
typedef struct
{
    uint32 val[9]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} cvb_RtIngIfTbl_t;

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
} cvb_RtIngIfTblParm_t;

/*
    L3 Host Route Lookup Table
*/
typedef struct
{
    uint32 val[CVB_RT_HOST_TBL_INDR_NUM]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} cvb_RtHostTbl_t;

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
} cvb_RtHostTblUc4Parm_t;

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
} cvb_RtHostTblUc4FirParm_t;

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
} cvb_RtHostTblUc6Parm_t;

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
} cvb_RtHostTblUc6FirParm_t;

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
    cvb_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin; /* If ttl < ttlMin, copy to cpu */
    uint16      mtuMax;
} cvb_RtHostTblMc4Parm_t;

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
    cvb_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin; /* If ttl < ttlMin, copy to cpu */
    uint16      mtuMax;
} cvb_RtHostTblMc4FirParm_t;

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
    cvb_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin;
    uint16      mtuMax;
} cvb_RtHostTblMc6Parm_t;

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
    cvb_RtkPmap dpm;
    uint8       rpfFailAct;
    uint8       rpfChkEn;
    uint8       ttlMin;
    uint16      mtuMax;
} cvb_RtHostTblMc6FirParm_t;

/*
    NETWORK ROUTE Table
*/
typedef struct
{
    uint32 val[23]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} cvb_RtNetRouteTbl_t;

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
} cvb_RtNetRouteTblParm_t;

/*
    MULTICAST COLLISION  Table
*/
typedef struct
{
    uint32 val[13]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} cvb_RtMcColTbl_t;

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
    cvb_RtkPmap dpm;
    uint16      mtuMax;
    uint8       rpfChkEn;
    uint8       qosEn;
    uint8       qosPri;
    uint8       rpfFailAct;
    uint16      vid;
    uint8       arbValid;
    uint8       arbMode;
} cvb_RtMcColTblParm_t;

/*
    NEXT HOP Table
*/
typedef struct
{
    uint32 val[1];
} cvb_RtNhTbl_t;

typedef struct
{
    uint8  egrIfIdx;
    uint16 l2Idx;
} cvb_RtNhTblParm_t;

/*
    HOST CVB_GROUP Table
*/
typedef struct
{
    uint32 val[CVB_RT_HOST_GRP_TBL_INDR_NUM]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} cvb_RtHostGrpTbl_t;

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
} cvb_RtHostGrpTblParm_t;

typedef struct
{
    uint32 topIdx;
    uint32 btmIdx;
    uint32 topValidMsk;
    uint32 btmValidMsk;
    uint32 found;
} cvb_RtHwSearchInfo_t;

typedef struct
{
    uint8  port;
    uint32 portEgrIfList[15][2];
    uint8  ttl;
} cvb_RtOilListInfo_t;

typedef struct
{
    uint8             valid[RT_HOST_ENTRY_MAX_SIZE];
    RtHostEntryType_e type[RT_HOST_ENTRY_MAX_SIZE];
} cvb_RtHostValidType_t;

EXTERN_API void      cvb_drv_rt_array_bit32_get(const uint32 *src, uint32 *dest, uint32 lsb, uint32 msb);
EXTERN_API void      cvb_drv_rt_array_bit32_set(uint32 *src, uint32 value, uint32 lsb, uint32 msb);
EXTERN_API void      cvb_drv_rt_max_host_entry_idx_num_get(RtHostEntryType_e entryType, uint8 *maxSlotNum);
EXTERN_API void      cvb_drv_rt_occupied_slot_num_get(RtHostEntryType_e entryType, uint8 *slotLen);
EXTERN_API RtkApiRet cvb_drv_rt_rhtbl_get(uint32 unit, uint32 entryNum, cvb_RtRhTblParm_t *rhTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_rhtbl_set(uint32 unit, uint32 entryNum, const cvb_RtRhTblParm_t *rhTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_oiltbl_get(uint32 unit, uint32 entryNum, cvb_RtOilTblParm_t *oilTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_oiltbl_set(uint32 unit, uint32 entryNum, const cvb_RtOilTblParm_t *oilTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_egriftbl_get(uint32 unit, uint32 entryNum, cvb_RtEgIfTblParm_t *egIfTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_egriftbl_set(uint32 unit, uint32 entryNum, const cvb_RtEgIfTblParm_t *egIfTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_igriftbl_get(uint32 unit, uint32 entryNum, cvb_RtIngIfTblParm_t *ingIfTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_igriftbl_set(uint32 unit, uint32 entryNum, const cvb_RtIngIfTblParm_t *ingIfTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc4_get(uint32 unit, uint32 entryNum, cvb_RtHostTblUc4Parm_t *hostTblUc4Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc4_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblUc4Parm_t *hostTblUc4Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc4fir_get(uint32 unit, uint32 entryNum, cvb_RtHostTblUc4FirParm_t *hostTblUc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc4fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblUc4FirParm_t *hostTblUc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc6_get(uint32 unit, uint32 entryNum, cvb_RtHostTblUc6Parm_t *hostTblUc6Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc6_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblUc6Parm_t *hostTblUc6Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc6fir_get(uint32 unit, uint32 entryNum, cvb_RtHostTblUc6FirParm_t *hostTblUc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_uc6fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblUc6FirParm_t *hostTblUc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc4_get(uint32 unit, uint32 entryNum, cvb_RtHostTblMc4Parm_t *hostTblMc4Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc4_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblMc4Parm_t *hostTblMc4Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc4fir_get(uint32 unit, uint32 entryNum, cvb_RtHostTblMc4FirParm_t *hostTblMc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc4fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblMc4FirParm_t *hostTblMc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc6_get(uint32 unit, uint32 entryNum, cvb_RtHostTblMc6Parm_t *hostTblMc6Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc6_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblMc6Parm_t *hostTblMc6Parm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc6fir_get(uint32 unit, uint32 entryNum, cvb_RtHostTblMc6FirParm_t *hostTblMc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_mc6fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostTblMc6FirParm_t *hostTblMc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_nettbl_get(uint32 unit, uint32 entryNum, cvb_RtNetRouteTblParm_t *netRouteTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_nettbl_set(uint32 unit, uint32 entryNum, const cvb_RtNetRouteTblParm_t *netRouteTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_mccol_get(uint32 unit, uint32 entryNum, cvb_RtMcColTblParm_t *mcColTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_mccol_set(uint32 unit, uint32 entryNum, const cvb_RtMcColTblParm_t *mcColTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_nhtbl_get(uint32 unit, uint16 entryNum, cvb_RtNhTblParm_t *nhTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_nhtbl_set(uint32 unit, uint16 entryNum, const cvb_RtNhTblParm_t *nhTblParm);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_uc4fir_get(uint32 unit, uint32 entryNum, cvb_RtHostGrpTblParm_t *hostGrpTblUc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_uc4fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostGrpTblParm_t *hostGrpTblUc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_uc6fir_get(uint32 unit, uint32 entryNum, cvb_RtHostGrpTblParm_t *hostGrpTblUc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_uc6fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostGrpTblParm_t *hostGrpTblUc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_mc4fir_get(uint32 unit, uint32 entryNum, cvb_RtHostGrpTblParm_t *hostGrpTblMc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_mc4fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostGrpTblParm_t *hostGrpTblMc4FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_mc6fir_get(uint32 unit, uint32 entryNum, cvb_RtHostGrpTblParm_t *hostGrpTblMc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hostgrp_mc6fir_set(uint32 unit, uint32 entryNum, const cvb_RtHostGrpTblParm_t *hostGrpTblMc6FirParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_host_grp_tbl_get(uint32 unit, RtHostEntryType_e entryType, uint32 entryNum, cvb_RtHostGrpTblParm_t *hostGrpTableParm, uint32 slotNum);
EXTERN_API RtkApiRet cvb_drv_rt_hw_egr_intf_set(uint32 unit, const RtIntfTbl_t *intfEntry);
EXTERN_API uint8     cvb_drv_rt_host_only_key_exist(RtHostlookUpKey_t hostLuKey);
EXTERN_API RtkApiRet cvb_drv_rt_route_mode_set(uint32 unit, RtRouteMode_e *rtMode);
EXTERN_API RtkApiRet cvb_drv_rt_dummy_intf_chk(uint32 unit, bool *status);
EXTERN_API RtkApiRet cvb_drv_rt_hosttbl_clear(uint32 unit);
EXTERN_API RtkApiRet cvb_drv_rt_glb_reg_cfg_on(uint32 unit);
EXTERN_API RtkApiRet cvb_drv_rt_glb_reg_cfg_off(uint32 unit);
EXTERN_API RtkApiRet cvb_drv_rt_intf_param_chk_runtime(const RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet cvb_drv_rt_hw_igr_intf_set_runtime(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet cvb_drv_rt_nhtbl_avail_get_runtime(uint32 unit, uint16 *nhIdx);
EXTERN_API RtkApiRet cvb_drv_rt_uc_host_tbl_entry_runtime_set(uint32 unit, RtUcEntry_t *hostEntry);
EXTERN_API RtkApiRet cvb_drv_rt_mc_host_tbl_entry_runtime_set(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet cvb_drv_rt_net_tbl_entry_set_runtime(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet cvb_drv_rt_nhtbl_avail_set_runtime(uint32 unit, uint16 nhIdx, uint8 valid);
EXTERN_API RtkApiRet cvb_drv_rt_mc_col_tbl_entry_set_runtime(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet cvb_drv_host_entry_type_valid_get(const cvb_RtHostTbl_t *hostTblEntry, cvb_RtHostValidType_t *entryValidType);
EXTERN_API RtkApiRet cvb_drv_rt_sw_intf_set(uint32 unit, const RtIntfTbl_t *intfEntry);
EXTERN_API RtkApiRet cvb_drv_rt_sw_intf_get(uint32 unit, RtIntfTbl_t *intfEntry);
EXTERN_API RtkApiRet cvb_drv_rt_hw_igr_intf_get(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet cvb_drv_rt_hw_egr_intf_get(uint32 unit, RtIntfTbl_t *intfEntry);
EXTERN_API RtkApiRet cvb_drv_rt_host_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet cvb_drv_rt_net_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet cvb_drv_rt_mccol_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet cvb_drv_rt_uc_host_tbl_entry_get(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet cvb_drv_rt_net_tbl_entry_get(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet cvb_drv_rt_mc_host_tbl_entry_get(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet cvb_drv_rt_mc_col_tbl_entry_get(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet cvb_drv_rt_rttbl_param_chk(const RtTbl_t *rtEntry);
EXTERN_API RtkApiRet cvb_drv_rt_rttbl_sdk_to_fw(const RtTbl_t *rtEntry, cvb_RtFwRtTbl_t *fwRtEntry);
EXTERN_API RtkApiRet cvb_drv_rt_subnet_set(uint32 unit, RtTbl_t *rtEntry, uint32 index);
EXTERN_API RtkApiRet cvb_drv_rt_rttbl_fw_to_sdk(const cvb_RtFwRtTbl_t *fwRtEntry, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet cvb_drv_rt_nhpool_clear(uint32 unit);
EXTERN_API RtkApiRet cvb_drv_host_raw_get(uint32 unit, uint16 entryNum, cvb_RtHostTbl_t *hostTblEntry);
EXTERN_API RtkApiRet cvb_drv_host_entry_typelist_get(const cvb_RtHostValidType_t *entryValidType, RtHostTypeList_t *hostTypeList);
#endif /*CVB_ASICDRV_ROUTING_H_*/