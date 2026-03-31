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

#ifndef DV1_RTK_API_ROUTING_H__
#define DV1_RTK_API_ROUTING_H__

/********* Include Header *********/
#include "rtkas_api_routing.h"
#include "rtl907xd_v1_api.h"

/********* Definition *********/
/* Routing tables */
#define DV1_HOST_ROUTE_TABLE_MAX        (1024u)
#define DV1_RH_TABLE_MAX                (2048u)
#define DV1_OIL_TABLE_MAX               (8192u)
#define DV1_EGIF_TABLE_MAX              (160u)
#define DV1_INGIF_TABLE_MAX             (64u)
#define DV1_NETWORK_ROUTE_TABLE_MAX     (256u)
#define DV1_MULTICAST_COL_TABLE_MAX     (64u)
#define DV1_NEXT_HOP_TABLE_MAX          (4096u)
#define DV1_UC6FIR_SLOT_NUM             (2u)
#define DV1_VLAN_NUM_MAX                (4096u)
#define DV1_RT_L4FMT_TCP                (0u)
#define DV1_RT_L4FMT_UDP                (1u)
#define DV1_RT_L4FMT_BOTH               (2u)
#define DV1_RT_L4FMT_NOCARE             (3u)
#define DV1_RT_L4FMT_MASK               (0x3u)
#define DV1_RT_L4FMT_STATIC_HOST_OFFSET (13u)
#define DV1_RT_L4FMT_MC_OFFSET          (6u)
#define DV1_RT_L4FMT_NET_OFFSET         (0u)
#define DV1_IP4_ADDR_LEN                (4u)
#define DV1_IP6_ADDR_LEN                (16u)
#define DV1_RT_UC_CFG_IP_VER_MASK       (0x1u)
#define DV1_RT_UC_CFG_FIREWALL_MASK     (0x2u)
#define DV1_RT_MC_CFG_FIREWALL_MASK     (0x2u)
#define DV1_RT_IPV4                     (0x0u)
#define DV1_RT_IPV6                     (0x1u)
#define DV1_RT_NET_DUMMY_ENTRY_NUM      (1u)
#define DV1_RT_MAX_BUCKET_NUM           (12u)
#define DV1_RT_MAX_NETTBL_NUM           (256u)
#define DV1_RT_MAX_MCCOLTBL_NUM         (64u)
#define DV1_RT_MAX_INTERFACE_NUM        (64u)
#define DV1_RT_MAX_VALID_UC_NUM         (4096u)
#define DV1_RT_MAX_VALID_MC_NUM         (2048u)
#define DV1_RT_MAX_HOSTTBL_NUM          (1024u)
#define DV1_RT_MAX_RHTBL_NUM            (2048u)
#define DV1_RT_RHINDEX_MASK             (0xFFFu)
#define DV1_RT_INVALID_IDX16            (0xFFFFu)
#define DV1_RT_INVALID_IDX32            (0xFFFFFFFFu)
#define DV1_RT_HOST_TBL_BOTTOM_START    (512u)
#define DV1_RT_ROUTING_MODE             (1u)
#define DV1_RT_FILTER_MODE              (0u)
#define DV1_HW_MISC_CTRL00              (DV1_INTERNAL_CPU_VARIABLE22)
#define DV1_HWCTRL_ROUTING_EN           (0x00000008u)
#define DV1_HWCTRL_ROUTING_EN_OFFSET    (3u)
#define DV1_HWCTRL_ROUTING_EN_LEN       (1u)
#define DV1_ACT_ROUTE                   (0u)
#define DV1_ACT_TRAP                    (1u)
#define DV1_ACT_COPY                    (2u)
#define DV1_ACT_DROP                    (3u)
#define DV1_ACT_RESERVED                (4u)
#define DV1_ACT_BRIDGE                  (5u)
#define DV1_HOSTTBL_L2_VALID_GRP_MAX    (32u)
#define DV1_NETTBL_L2_VALID_GRP_MAX     (8u)
#define DV1_TBL_CLEAR_POLLING_TIME      (5u)

/********* Enumeration *********/
typedef enum
{
    DV1_EM_RT_UC4 = 0u, /**< IPv4 Unicast */
    DV1_EM_RT_MC4,      /**< IPv4 Multicast */
    DV1_EM_RT_UC6,      /**< IPv6 Unicast */
    DV1_EM_RT_MC6,      /**< IPv6 Multicast */
    DV1_EM_RT_UC4FIR,   /**< IPv4 Unicast with firewall */
    DV1_EM_RT_MC4FIR,   /**< IPv4 Multicast with firewall */
    DV1_EM_RT_UC6FIR,   /**< IPv6 Unicast with firewall */
    DV1_EM_RT_MC6FIR    /**< IPv6 Multicast with firewall */
} dv1_RtHostEntryType_e;

/********* Structure *********/
typedef struct
{
    uint8  destIpAddr[DV1_IP6_ADDR_LEN]; /**< Array of destination IP (0x0 to 0xFF for 16 elements) */
    uint8  subnetMask[DV1_IP6_ADDR_LEN]; /**< Array of destination IP mask (0x0 to 0xFF for 16 elements) */
    uint8  nhIntfId;                     /**< Next hop interface index (0x0 to 0xFF) */
    uint8  ipVer;                        /**< IP version (IP4_VER, IP6_VER) */
    uint8  hit;                          /**< Hit field (DV1_DISABLED to ENABLED) */
    uint8  valid;                        /**< Valid field (DV1_DISABLED to ENABLED) */
    uint8  nextHopIp[DV1_IP6_ADDR_LEN];  /**< Array of next hop IP (0x0 to 0xFF for 16 elements) */
    uint16 sp;                           /**< TCP/UDP source port (0x0 to 0xFFFF) */
    uint16 dp;                           /**< TCP/UDP destination port (0x0 to 0xFFFF) */
    uint8  sip[DV1_IP6_ADDR_LEN];        /**< Array of source IP (0x0 to 0xFF for 16 elements) */
} dv1_RtFwRtTbl_t;

typedef struct
{
    uint32 tblType;         /**< Table type (19 to 20) */
    uint32 tblParamType;    /**< Table parameter type (0 to 7) */
    uint8  isFindNextEntry; /**< Get the next entry (0 to 1) */
    uint32 tblIdx;          /**< Table entry index (0 to 12276) */
    uint8  isValid;         /**< Check the valid entry (0 to 1) */
    uint32 ipv4Dip;         /**< Ipv4 destination address (0 to 0xFFFFFFFF) */
    uint32 ipv6Dip31to0;    /**< Ipv6 destination address [0:31] (0 to 0xFFFFFFFF) */
    uint32 ipv6Dip63to32;   /**< Ipv6 destination address [32:63] (0 to 0xFFFFFFFF) */
    uint32 ipv6Dip95to64;   /**< Ipv6 destination address [64:95] (0 to 0xFFFFFFFF) */
    uint32 ipv6Dip127to96;  /**< Ipv6 destination address [96:127] (0 to 0xFFFFFFFF) */
    uint8  dstPort;         /**< Next hop destination port (0 to 65535) */
    uint8  dstMac[6];       /**< Array of  next hop destination mac address (0 to 0xFF) */
} dv1_RtNeighCacheEntry_t;

/********* Extern Global Variable Declaration *********/

/********* Function Declaration *********/
EXTERN_API RtkApiRet dv1_route_rttbl_set(uint32 unit, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet dv1_route_rttbl_get(uint32 unit, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet dv1_route_init_process_trigger(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dv1_route_disable(uint32 unit, uint8 disable);
EXTERN_API RtkApiRet dv1_route_rng_chk_set(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dv1_route_tbl_flush(uint32 unit);
EXTERN_API RtkApiRet dv1_route_intf_set(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet dv1_route_intf_get(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet dv1_route_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet dv1_route_uc_set(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dv1_route_uc_get(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dv1_route_mc_set(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dv1_route_mc_get(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dv1_route_host_entry_typelist_get(uint32 unit, RtHostTypeList_t *typeList);
#endif /* DV1_RTK_API_ROUTING_H__ */