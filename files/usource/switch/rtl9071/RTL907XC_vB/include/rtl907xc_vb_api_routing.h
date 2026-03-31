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

#ifndef CVB_RTK_API_ROUTING_H__
#define CVB_RTK_API_ROUTING_H__

/********* Include Header *********/
#include "rtkas_api_routing.h"
#include "rtl907xc_vb_api.h"

/********* Definition *********/
/* Routing tables */
#define CVB_HOST_ROUTE_TABLE_MAX    (1024u)
#define CVB_RH_TABLE_MAX            (2048u)
#define CVB_OIL_TABLE_MAX           (8192u)
#define CVB_EGIF_TABLE_MAX          (160u)
#define CVB_INGIF_TABLE_MAX         (64u)
#define CVB_NETWORK_ROUTE_TABLE_MAX (256u)
#define CVB_MULTICAST_COL_TABLE_MAX (64u)
#define CVB_NEXT_HOP_TABLE_MAX      (4096u)
#define CVB_UC6FIR_SLOT_NUM         (2u)
#define CVB_IP6_VER                 (6u)
#define CVB_IP4_VER                 (4u)
/*
    [0]: L4 learning on/off
    [1]: reset routing table
    [2:17]: host table alarm threshold
    [18:25]: network route table alarm threshold
    [26]: host table alarm
    [27]: network route table alarm
    [28]: trigger NDP resolution
 */
#define CVB_RT_L4_LEARN_CTRL (CVB_HW_MISC_DMY_RESIGSER_4)
/*
    [0:7]: Polling interval (sec)
    [8:11]: polling time to age out
 */
#define CVB_RT_L4_LEARN_AGING_CTRL            (CVB_HW_MISC_DMY_RESIGSER_12)
#define CVB_RT_L4_LEARN_ENABLE_OFFSET         (0u)
#define CVB_RT_L4_LEARN_ENABLE_LEN            (1u)
#define CVB_RT_L4_LEARN_RST_TBL_OFFSET        (1u)
#define CVB_RT_L4_LEARN_RST_TBL_LEN           (1u)
#define CVB_RT_L4_LEARN_HOST_THR_OFFSET       (2u)
#define CVB_RT_L4_LEARN_HOST_THR_LEN          (16u)
#define CVB_RT_L4_LEARN_NET_THR_OFFSET        (18u)
#define CVB_RT_L4_LEARN_NET_THR_LEN           (8u)
#define CVB_RT_L4_LEARN_HOST_ALARM_OFFSET     (26u)
#define CVB_RT_L4_LEARN_HOST_ALARM_LEN        (1u)
#define CVB_RT_L4_LEARN_NET_ALARM_OFFSET      (27u)
#define CVB_RT_L4_LEARN_NET_ALARM_LEN         (1u)
#define CVB_RT_L4_NDP_RESOL_TRIGGER_OFFSET    (28u)
#define CVB_RT_L4_NDP_RESOL_TRIGGER_LEN       (1u)
#define CVB_RT_L4_TBL_POLLING_INTERVAL_OFFSET (0u)
#define CVB_RT_L4_TBL_POLLING_INTERVAL_LEN    (12u)
#define CVB_RT_L4FMT_TCP                      (0u)
#define CVB_RT_L4FMT_UDP                      (1u)
#define CVB_RT_L4FMT_BOTH                     (2u)
#define CVB_RT_L4FMT_NOCARE                   (3u)
#define CVB_RT_L4FMT_MASK                     (0x3u)
#define CVB_RT_L4FMT_STATIC_HOST_OFFSET       (13u)
#define CVB_RT_L4FMT_MC_OFFSET                (6u)
#define CVB_RT_L4FMT_NET_OFFSET               (0u)
#define CVB_IP4_ADDR_LEN                      (4u)
#define CVB_IP6_ADDR_LEN                      (16u)
#define CVB_RT_UC_CFG_IP_VER_MASK             (0x1u)
#define CVB_RT_UC_CFG_FIREWALL_MASK           (0x2u)
#define CVB_RT_MC_CFG_FIREWALL_MASK           (0x2u)
#define CVB_RT_IPV4                           (0x0u)
#define CVB_RT_IPV6                           (0x1u)
#define CVB_RT_NET_DUMMY_ENTRY_NUM            (1u)
#define CVB_RT_MAX_BUCKET_NUM                 (12u)
#define CVB_RT_MAX_NETTBL_NUM                 (256u)
#define CVB_RT_MAX_MCCOLTBL_NUM               (64u)
#define CVB_RT_MAX_INTERFACE_NUM              (64u)
#define CVB_RT_MAX_VALID_UC_NUM               (4096u)
#define CVB_RT_MAX_VALID_MC_NUM               (2048u)
#define CVB_RT_MAX_HOSTTBL_NUM                (1024u)
#define CVB_RT_MAX_RHTBL_NUM                  (1024u)
#define CVB_RT_RHINDEX_MASK                   (0xFFFu)
#define CVB_RT_INVALID_IDX8                   (0xFFu)
#define CVB_RT_INVALID_IDX16                  (0xFFFFu)
#define CVB_RT_INVALID_IDX32                  (0xFFFFFFFFu)
#define CVB_RT_HOST_TBL_BOTTOM_START          (512u)
#define CVB_RT_ROUTING_MODE                   (1u)
#define CVB_RT_FILTER_MODE                    (0u)
#define CVB_HW_MISC_CTRL00                    (CVB_INTERNAL_CPU_VARIABLE22)
#define CVB_HWCTRL_ROUTING_EN                 (0x00000008u)
#define CVB_HWCTRL_ROUTING_EN_OFFSET          (3u)
#define CVB_HWCTRL_ROUTING_EN_LEN             (1u)
#define CVB_ACT_ROUTE                         (0u)
#define CVB_ACT_TRAP                          (1u)
#define CVB_ACT_COPY                          (2u)
#define CVB_ACT_DROP                          (3u)
#define CVB_ACT_RESERVED                      (4u)
#define CVB_ACT_BRIDGE                        (5u)
#define CVB_HOSTTBL_L2_VALID_GRP_MAX          (32u)
#define CVB_NETTBL_L2_VALID_GRP_MAX           (8u)
#define CVB_VLAN_NUM_MAX                      (4096u)
#define CVB_DFT_INTF_L2_START_IDX             (255u)
#define CVB_TBL_CLEAR_POLLING_TIME            (5u)

/********* Enumeration *********/

/********* Structure *********/
typedef struct
{
    uint8  destIpAddr[IPV6_ADDR_LEN]; /**< Array of destination IP (0x0 to 0xFF for 16 elements) */
    uint8  subnetMask[IPV6_ADDR_LEN]; /**< Array of destination IP mask (0x0 to 0xFF for 16 elements) */
    uint8  nhIntfId;                  /**< Next hop interface index (0x0 to 0xFF) */
    uint8  ipVer;                     /**< IP version (IP4_VER, IP6_VER) */
    uint8  hit;                       /**< Hit field (DV1_DISABLED to ENABLED) */
    uint8  valid;                     /**< Valid field (DV1_DISABLED to ENABLED) */
    uint8  nextHopIp[IPV6_ADDR_LEN];  /**< Array of next hop IP (0x0 to 0xFF for 16 elements) */
    uint16 sp;                        /**< TCP/UDP source port (0x0 to 0xFFFF) */
    uint16 dp;                        /**< TCP/UDP destination port (0x0 to 0xFFFF) */
    uint8  sip[IPV6_ADDR_LEN];        /**< Array of source IP (0x0 to 0xFF for 16 elements) */
} cvb_RtFwRtTbl_t;

typedef struct
{
    uint16 intFidIvid;
    uint8  mac[MAC_ADDR_LEN];
    uint8  gwIp[IPV6_ADDR_LEN];
    uint8  valid;
    uint8  physicalPn;
    uint16 mtuIdx;
    uint16 sp;
    uint16 dp;
} cvb_RtFwIntfTbl_t;

/********* Extern Global Variable Declaration *********/

/********* Function Declaration *********/
EXTERN_API RtkApiRet cvb_route_init_process_trigger(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet cvb_route_disable(uint32 unit, uint8 disable);
EXTERN_API RtkApiRet cvb_route_rng_chk_set(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet cvb_route_tbl_flush(uint32 unit);
EXTERN_API RtkApiRet cvb_route_intf_set(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet cvb_route_intf_get(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet cvb_route_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet cvb_route_uc_set(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet cvb_route_uc_get(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet cvb_route_mc_set(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet cvb_route_mc_get(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet cvb_route_rttbl_set(uint32 unit, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet cvb_route_rttbl_get(uint32 unit, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet cvb_route_host_entry_typelist_get(uint32 unit, RtHostTypeList_t *typeList);
EXTERN_API RtkApiRet cvb_drv_rt_sw_intf_clear(uint32 unit);
EXTERN_API RtkApiRet cvb_drv_rt_rttbl_clear(uint32 unit);
#endif /* CVB_RTK_API_ROUTING_H__ */