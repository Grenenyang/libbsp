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

#ifndef RTKAS_API_ROUTING_H__
#define RTKAS_API_ROUTING_H__

/********* Include Header *********/
#include "rtkas_common.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_api.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_api.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_api.h"
#endif
/********* Definition *********/
#define ACL_BLK_IDX_VALID         (0x10u)
#define IP6_TYPE_BIT              (0x2u)
#define IP4_VER                   (4u)
#define IP6_VER                   (6u)
#define INVALID_VER               (0xFFu)
#define RT_INTF_TBL_MAX_SIZE      (64u)
#define RT_HOST_TBL_MAX_SIZE      (1024u)
#define RT_NET_TBL_MAX_SIZE       (256u)
#define RT_MCCOL_TBL_MAX_SIZE     (64u)
#define RT_HOST_ENTRY_MAX_SIZE    (12u)
#define RT_L4FMT_TCP              (0u)
#define RT_L4FMT_UDP              (1u)
#define RT_L4FMT_BOTH             (2u)
#define RT_L4FMT_NOCARE           (3u)
#define RT_TBL_INVALID_IDX        (0xFFFFu)
#define RT_HOST_ENTRY_INVALID_IDX (0xFFu)
#define RT_INVALID                (0u)
#define RT_VALID                  (1u)
#define RT_ARB_ROUTING            (0u)
#define RT_ARB_NORMAL             (1u)

/********* Enumeration *********/
/**
 * @brief Enumeration for entry type of host table.
 */
typedef enum
{
    EM_RT_UC4 = 0u, /**< IPv4 Unicast */
    EM_RT_MC4,      /**< IPv4 Multicast */
    EM_RT_UC6,      /**< IPv6 Unicast */
    EM_RT_MC6,      /**< IPv6 Multicast */
    EM_RT_UC4FIR,   /**< IPv4 Unicast with firewall */
    EM_RT_MC4FIR,   /**< IPv4 Multicast with firewall */
    EM_RT_UC6FIR,   /**< IPv6 Unicast with firewall */
    EM_RT_MC6FIR,   /**< IPv6 Multicast with firewall */
    EM_RT_MAX       /**< Maximum of entry type */
} RtHostEntryType_e;

/**
 * @brief Enumeration for routing table type.
 */
typedef enum
{
    EM_HOST_TBL = 0u, /**< Host table */
    EM_NET_TBL,       /**< Net table */
    EM_MCCOL_TBL,     /**< Multicast collision table */
    EM_TBL_MAX        /**< Maximum of table type */
} RtTblType_e;

/**
 * @brief Enumeration for indicating routing mode.
 */
typedef enum
{
    EM_ROUTE_DYNAMIC_MODE = 0u, /**< Dynamic routing mode (0) */
    EM_FILTER_MODE,             /**< Filter mode (1) */
    EM_ROUTE_STATIC_4KEY_MODE,  /**< 4-key static routing mode with dynamic L2 (2) */
    EM_ROUTE_STATIC_MODE,       /**< Static routing mode (3) */
    EM_ROUTE_L4_LEARN_MODE,     /**< Semi-stateful firewall mode (4) */
} RtRouteMode_e;

/********* Structure *********/
/**
 * @brief This struct is used to store the table lookup key
 */
typedef struct
{
    uint8             sip[IPV6_ADDR_LEN]; /**< lookup key: 8-bit array of source IP (0x0 to 0xFF for 16 elements) */
    uint8             dip[IPV6_ADDR_LEN]; /**< lookup key: 8-bit array of destination IP (0x0 to 0xFF for 16 elements) */
    uint16            sp;                 /**< lookup key: TCP/UDP destination port (0 to 65535) */
    uint16            dp;                 /**< lookup key: TCP/UDP source port (0 to 65535) */
    uint16            vid;                /**< lookup key: VLAN ID (0 to 4095) */
    RtHostEntryType_e entryType;          /**< table entry type (N/A) */
    uint8             spFmt;              /**< Header format of source port (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
    uint8             dpFmt;              /**< Header format of destination port (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
} RtlookUpKey_t;

/**
 * @brief This struct is used to store the table entry attribute
 */
typedef struct
{
    RtTblType_e tblType;  /**< Table type (N/A) */
    uint16      index;    /**< Table index (0 to 1024) */
    uint8       entryIdx; /**< Entry index (Host table only) (0 to 12) */
} RtEntryAttr_t;

/**
 * @brief This struct is used to store the table searched infomation
 */
typedef struct
{
    RtlookUpKey_t luKey;         /**< Lookup key (N/A) */
    RtTblType_e   hitTblType;    /**< Hit table type (N/A) */
    uint16        hitIdx;        /**< Hit entry index (HOST_ROUTE_TABLE: 0 to 1023, NETWORK_ROUTE_TABLE: 0 to 255, MULTICAST_COL_TABLE: 0 to 63 */
    uint8         hitEntryIdx;   /**< Hit host table entry index (0~11) */
    RtTblType_e   availTblType;  /**< Available table type (N/A) */
    uint16        availIdx;      /**< Available entry index (HOST_ROUTE_TABLE: 0 to 1023, NETWORK_ROUTE_TABLE: 0 to 255, MULTICAST_COL_TABLE: 0 to 63 */
    uint8         availEntryIdx; /**< Available host table entry index (0~11) */
} RtTableSearchInfo_t;

/**
 * @brief This struct is used to indicate lookup key mode
 */
typedef struct
{
    uint8 vidNoCare; /**< VID don't care (DISABLED to ENABLED) */
    uint8 dipNoCare; /**< Destination IP don't care (DISABLED to ENABLED) */
    uint8 sipNoCare; /**< Source IP don't care (DISABLED to ENABLED) */
    uint8 spNoCare;  /**< TCP/UDP source port don't care (DISABLED to ENABLED) */
    uint8 dpNoCare;  /**< TCP/UDP destination port don't care (DISABLED to ENABLED) */
} RtLuKeyMode_t;

/**
 * @brief This struct is used to store interface table information
 */
typedef struct
{
    uint8  valid;               /**< Valid bit (RT_INVALID to RT_VALID) */
    uint8  firewall;            /**< Firewall mode (DISABLED to ENABLED) */
    uint16 intfFidVid;          /**< VID (0 to 4095) */
    uint8  mac[MAC_ADDR_LEN];   /**< Array of interface MAC (0x0 to 0xFF for 16 elements) */
    uint8  gwIp[IPV6_ADDR_LEN]; /**< Array of gateway IP (0x0 to 0xFF for 16 elements) */
    uint8  ipv4MtuIdx;          /**< IPv4 MTU index */
    uint8  ipv6MtuIdx;          /**< IPv6 MTU index */
    uint8  portId;              /**< physical port (0x0 to 0xFF) */
    uint8  index;               /**< Interface table index (0 to 63) */
    uint8  spFmt;               /**< Layer 4 header format (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
    uint8  dpFmt;               /**< Layer 4 header format (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
    uint8  ipVer;               /**< IP version (IP4_VER, IP6_VER) */
    uint8  filter;              /**< Filter mode ON/OFF (DISABLED to ENABLED) */
} RtIntfTbl_t;

/**
 * @brief This struct is used to store interface table information & lookup mode
 */
typedef struct
{
    RtIntfTbl_t   intfEntry; /**< Interface entry configuration (N/A) */
    RtLuKeyMode_t luKeyMode; /**< Lookup key mode (N/A) */
} RtIntfCfg_t;

/**
 * @brief This struct is used to store the host table lookup key
 */
typedef struct
{
    uint8             sipAddr[IPV6_ADDR_LEN]; /**< lookup key: 8-bit array of source IP (0x0 to 0xFF for 16 elements) */
    uint8             dipAddr[IPV6_ADDR_LEN]; /**< lookup key: 8-bit array of destination IP (0x0 to 0xFF for 16 elements) */
    uint16            sp;                     /**< lookup key: TCP/UDP destination port (0 to 65535) */
    uint16            dp;                     /**< lookup key: TCP/UDP source port (0 to 65535) */
    uint16            vid;                    /**< lookup key: VLAN ID (0 to 4095) */
    RtHostEntryType_e entryType;              /**< table entry type (N/A) */
    uint8             spFmt;                  /**< Header format of source port (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
    uint8             dpFmt;                  /**< Header format of destination port (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
    uint8             dipGrp;                 /**< Destination IP group bitmap (0 to 0xFF) */
    uint8             sipGrp;                 /**< Source IP group bitmap (0 to 0xFF) */
    uint8             dpGrp;                  /**< TCP/UDP destination port group bitmap (0 to 0xF) */
    uint8             spGrp;                  /**< TCP/UDP source port group bitmap (0 to 0xF) */
    uint8             aclBlkIdx0;             /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
    uint8             aclBlkIdx1;             /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
    uint8             aclBlkIdx4;             /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
    uint8             aclBlkIdx5;             /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
    uint8             aclBlkIdx8;             /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
    uint8             aclBlkIdx9;             /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
    uint8             aclBlkIdx12;            /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
    uint8             aclBlkIdx13;            /**< Combined ACL index, [4]: Valid bit (DISABLED, ENABLED), [0:3]: PIE index (0 to 0xF) */
} RtHostlookUpKey_t;

/**
 * @brief This struct is used to store the net table lookup key
 */
typedef struct
{
    uint8             sipAddr[IPV6_ADDR_LEN]; /**< lookup key: 8-bit array of source IP (0x0 to 0xFF for 16 elements) */
    uint8             dipAddr[IPV6_ADDR_LEN]; /**< lookup key: 8-bit array of destination IP (0x0 to 0xFF for 16 elements) */
    uint16            sp;                     /**< lookup key: TCP/UDP destination port (0 to 65535) */
    uint16            dp;                     /**< lookup key: TCP/UDP source port (0 to 65535) */
    uint16            vid;                    /**< lookup key: VLAN ID (0 to 4095) */
    RtHostEntryType_e entryType;              /**< table entry type (N/A) */
    uint8             spFmt;                  /**< Header format of source port (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
    uint8             dpFmt;                  /**< Header format of destination port (RT_L4FMT_TCP, RT_L4FMT_UDP, RT_L4FMT_BOTH and RT_L4FMT_NOCARE) */
    uint8             sipCare[IPV6_ADDR_LEN]; /**< 8-bit array of source ip mask (0x0 to 0xFF for 16 elements) */
    uint8             dipCare[IPV6_ADDR_LEN]; /**< 8-bit array of destination ip mask (0x0 to 0xFF for 16 elements) */
    uint16            spCare;                 /**< source port mask (0x0 to 0xFFFF) */
    uint16            dpCare;                 /**< destination port mask (0x0 to 0xFFFF) */
    uint16            vidCare;                /**< VLAN ID mask (0x0 to 0xFFF) */
    uint8             typeCare;               /**< Type care (DISABLED to ENABLED) */
    uint8             spFmtCare;              /**< TCP/UDP source port care (DISABLED to ENABLED) */
    uint8             dpFmtCare;              /**< TCP/UDP destination port care (DISABLED to ENABLED) */
} RtNetlookUpKey_t;

/**
 * @brief This struct is used to store the unicast entry information
 */
typedef struct
{
    uint8             valid;     /**< Valid (RT_INVALID to RT_VALID) */
    RtHostlookUpKey_t hostLuKey; /**< Lookup key for host table (N/A) */
    RtNetlookUpKey_t  netLuKey;  /**< Lookup key for net table (N/A) */
    uint16            l2Idx;     /**< L2 table index (0 to 4095) */
    uint8             egrIfIdx;  /**< Egress interface index (0 to 63) */
    uint8             ttl;       /**< TTL decrease and check (DISABLED to ENABLED) */
    uint8             nhAlloc;   /**< Next hop allocated status (RT_INVALID to RT_VALID) */
    uint16            nhIdx;     /**< Next hop index (0 to 4095) */
    uint8             arbValid;  /**< Arbitration mode status (RT_INVALID to RT_VALID) */
    uint8             arbMode;   /**< Arbitration mode (RT_ARB_ROUTING to RT_ARB_NORMAL) */
    RtEntryAttr_t     tblInfo;   /**< Table entry infomation (N/A) */
} RtUcEntry_t;

/**
 * @brief This struct is used to store the multicast entry information
 */
typedef struct
{
    uint8             valid;        /**< Valid (RT_INVALID to RT_VALID) */
    RtHostlookUpKey_t luKey;        /**< Lookup key (N/A) */
    uint8             ttl;          /**< TTL decrease and check (DISABLED to ENABLED) */
    RtkPmap           dpm;          /**< Destination port mask (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL) */
    uint8             nextHopIf[8]; /**< Array of next hop interface list (0x0 to 0xFF for 8 elements) */
    uint8             rhAlloc;      /**< Replication header table entry allocated status (RT_INVALID to RT_VALID) */
    uint16            rhTblIdx;     /**< Replication header table entry index (0 to 1023) */
    uint8             arbValid;     /**< Arbitration mode status (RT_INVALID to RT_VALID) */
    uint8             arbMode;      /**< Arbitration mode (RT_ARB_ROUTING to RT_ARB_NORMAL) */
    RtEntryAttr_t     tblInfo;      /**< Table entry infomation (N/A) */
} RtMcEntry_t;

/**
 * @brief This struct is used to store the network routing entry information
 */
typedef struct
{
    uint8 index;                     /**< Network Routing Table index (0 to 255) */
    uint8 valid;                     /**< Valid field (RT_INVALID to RT_VALID) */
    uint8 ipVer;                     /**< IP version (IP4_VER, IP6_VER) */
    uint8 destIpAddr[IPV6_ADDR_LEN]; /**< Array of destination IP (0x0 to 0xFF for 16 elements) */
    uint8 nextHopIp[IPV6_ADDR_LEN];  /**< Array of next hop IP (0x0 to 0xFF for 16 elements) */
    uint8 subnetMask;                /**< Destination IP mask (IPv4: 0 to 32, IPv6: 0 to 128) */
    uint8 nhIntfId;                  /**< Next hop interface index (0x0 to 0xFF) */
} RtTbl_t;

/**
 * @brief This struct is used to store the host entry type information
 */
typedef struct
{
    RtHostEntryType_e entryType; /**< Entry type (EM_RT_UC4 to EM_RT_UC6FIR) */
    uint8             entryIdx;  /**< Entry index (Host table only) (0 to 12) */
    uint8             valid;     /**< Valid or not (RT_INVALID to RT_VALID) */
} RtHostEntryInfo_t;

/**
 * @brief This struct is used to store the host entry type list
 */
typedef struct
{
    uint16            index;                            /**< Host table entry index (0 to 1023) */
    RtHostEntryInfo_t typeList[RT_HOST_ENTRY_MAX_SIZE]; /**< List types of stored entries (N/A) */
} RtHostTypeList_t;

/**
 * @brief This struct is used to turn on/off the routing fumction
 */
typedef struct
{
    uint8 enable; /**< Enable/Disable routing feature (DISABLED to ENABLED) */
} RtEnable_t;

/**
 * @brief This struct is used to turn on/off the range check fumction
 */
typedef struct
{
    uint8 enable; /**< Enable/Disable range check function (DISABLED to ENABLED) */
} RtRngChkSet_t;

EXTERN_API RtkApiRet rtk_route_enable(UnitChip_t unitChip, const RtEnable_t *param);
EXTERN_API RtkApiRet rtk_route_tbl_flush(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_route_rng_chk_set(UnitChip_t unitChip, const RtRngChkSet_t *param);
EXTERN_API RtkApiRet rtk_route_intf_set(UnitChip_t unitChip, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet rtk_route_intf_get(UnitChip_t unitChip, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet rtk_route_uc_set(UnitChip_t unitChip, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet rtk_route_uc_get(UnitChip_t unitChip, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet rtk_route_mc_set(UnitChip_t unitChip, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet rtk_route_mc_get(UnitChip_t unitChip, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet rtk_route_rttbl_set(UnitChip_t unitChip, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet rtk_route_rttbl_get(UnitChip_t unitChip, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet rtk_route_tbl_lookup(UnitChip_t unitChip, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet rtk_route_host_entry_typelist_get(UnitChip_t unitChip, RtHostTypeList_t *typeList);
#endif /* RTKAS_API_ROUTING_H__ */