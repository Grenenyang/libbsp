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

#ifndef RTKAS_API_L2_H__
#define RTKAS_API_L2_H__

#include "rtkas_api_vlan.h"

/* Type of LUT entry */
typedef enum
{
    EM_LUT_UNI            = 0u, /**< Type of LUT entry is unicast */
    EM_LUT_MULTI          = 1u, /**< Type of LUT entry is multicast */
    EM_LUT_IPMULTI        = 2u, /**< Type of LUT entry is IP multicast */
    EM_LUT_INVALID_ENTRY  = 3u, /**< Type of LUT entry is invalid */
    EM_LUT_ENTRY_TYPE_END = 4u,
} LutEntryType_e;

/* Action of legal port */
typedef enum
{
    EM_LEGAL_FWD            = 0u, /**< Legal port action: forward (default) */
    EM_LEGAL_FWD_N_COPY_CPU = 1u, /**< Legal port action: forward and copy to CPU ports */
    EM_LEGAL_TRAP_CPU       = 2u, /**< Legal port action: trap to CPU ports */
    EM_LEGAL_PORT_ACT_END   = 3u,
} LegalPortAct_e;

/* Action of illegal port */
typedef enum
{
    EM_ILLEGAL_DROP         = 0u, /**< Illegal port action: drop (default) */
    EM_ILLEGAL_TRAP_CPU     = 1u, /**< Illegal port action: trap to CPU ports */
    EM_ILLEGAL_PORT_ACT_END = 2u,
} IllegalPortAct_e;

/* Structure of unicast entry */
typedef struct
{
    uint8  eavPriEn;          /**< N/A */
    uint8  eavPri;            /**< N/A */
    uint8  fid;               /**< Filter ID */
    uint8  bstatic;           /**< Determine whether to update port number when a matched entry is found */
    uint8  auth;              /**< 802.1x MAC-based authentication */
    uint8  dablk;             /**< Destination MAC blocking */
    uint8  sablk;             /**< Source MAC blocking */
    uint8  spa;               /**< Source port of learning address */
    uint16 age;               /**< Aging time */
    uint8  l3entry;           /**< Indicate the entry is L2 entry or IP multicast entry */
    uint8  mac[MAC_ADDR_LEN]; /**< MAC address */
    uint16 vid;               /**< VLAN ID */
    uint8  ulfid;             /**< Use FID for hash (0 to 1) */
    uint8  nh;                /**< Use for routing next hop (0 to 1) */
} LutUniParam_t;

/* Structure of multicast entry */
typedef struct
{
    uint8  eavPriEn;          /**< N/A */
    uint8  eavPri;            /**< N/A */
    uint8  fid;               /**< Filter ID */
    uint32 mbr;               /**< Forwarding port mask */
    uint8  vleaky;            /**< Group entry VLAN leaky or not */
    uint8  l3entry;           /**< Indicate the entry is L2 entry or IP multicast entry */
    uint8  mac[MAC_ADDR_LEN]; /**< MAC address */
    uint16 vid;               /**< VLAN ID */
} LutMultiParam_t;

/* Structure of IP multicast entry */
typedef struct
{
    uint8  eavPriEn;           /**< N/A */
    uint8  eavPri;             /**< N/A */
    uint8  fid;                /**< Filter ID */
    uint32 mbr;                /**< Forwarding port mask */
    uint8  vleaky;             /**< Group entry VLAN leaky or not */
    uint8  l3entry;            /**< Indicate the entry is L2 entry or IP multicast entry */
    uint8  sip[IPV4_ADDR_LEN]; /**< Source IP */
    uint8  dip[IPV4_ADDR_LEN]; /**< Destination IP */
} LutIpMultiParam_t;

/* Structure of L2 entry */
typedef struct
{
    LutEntryType_e    type;  /**< Type of L2 entry */
    LutUniParam_t     uni;   /**< Structure of unicast entry */
    LutMultiParam_t   multi; /**< Structure of multicast entry */
    LutIpMultiParam_t ip;    /**< Structure of IP multicast entry */
    uint16            index; /**< Index of L2 entry */
} LutParam_t;

/* Structure of SA move record */
typedef struct
{
    uint32 newPortNum;        /**< Record the new port number of triggering legal/illegal SA move */
    uint32 oldPortNum;        /**< Record the old port number of triggering legal/illegal SA move */
    uint8  mac[MAC_ADDR_LEN]; /**< Record the MAC source address of triggering legal/illegal SA move */
} SaMoveRecord_t;

/**
 * @brief This structure is used to get the hash algorithm from the address table.
 */
typedef struct
{
    uint32 algo; /**< [in,out] Hash algorithm (0 to 1) */
} LutHashAlgoInfo_t;

/**
 * @brief This structure is used to get an L2 entry by table index.
 */
typedef struct
{
    uint16     index;    /**< [in]  L2 entry index (0 to 4095) */
    LutParam_t lutEntry; /**< [out] L2 entry data (N/A) */
} LutGetInfo_t;

/**
 * @brief This structure is used to search for an L2 entry by MAC address.
 */
typedef struct
{
    uint8      macAddr[MAC_ADDR_LEN]; /**< [in]  The array of MAC address (0x0 to 0xFF for 6 elements) */
    LutParam_t lutEntry;              /**< [out] L2 entry data (N/A) */
} LutSearchInfo_t;

/**
 * @brief This structure is used to search for a multicast entry by MAC and FID.
 */
typedef struct
{
    uint8      macAddr[MAC_ADDR_LEN]; /**< [in]  The array of MAC address (0x0 to 0xFF for 6 elements) */
    VlanFid    fid;                   /**< [in]  Filter ID (0 to 14) */
    LutParam_t lutEntry;              /**< [out] L2 entry data (N/A) */
} LutMultiGetInfo_t;

/**
 * @brief This structure is used to search for an IP multicast entry by SIP and DIP.
 */
typedef struct
{
    uint8   sip[IPV4_ADDR_LEN]; /**< [in]     The array of source IP address (0 to 255 for 4 elements) */
    uint8   dip[IPV4_ADDR_LEN]; /**< [in]     The array of destination IP address (0 to 255 for 4 elements) */
    RtkPmap portmask;           /**< [in,out] Port mask (N/A) */
} LutIpMultiInfo_t;

/**
 * @brief This structure is used to get an L2 entry from the BCAM table by BCAM index.
 */
typedef struct
{
    uint16     index;    /**< [in]  L2 BCAM entry index (0 to 4095) */
    LutParam_t lutEntry; /**< [out] L2 entry data (N/A) */
} LutCamGetInfo_t;

/**
 * @brief This structure is used to control the action when an address table lookup misses.
 */
typedef struct
{
    uint8   type;   /**< [in] Lookup miss type (0 to 5) */
    uint8   action; /**< [in] Lookup miss action (0 to 2) */
    RtkPort port;   /**< [in] Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
} LutLookupMissCtrlInfo_t;

/**
 * @brief This structure is used to enable MAC learning for address table lookup.
 */
typedef struct
{
    uint8   enable;   /**< [in] Enable or disable (0 to 1) */
    RtkPmap portmask; /**< [in] Port mask (N/A) */
} LutMacLrnInfo_t;

/**
 * @brief This structure is used to add a static unicast entry to the lookup address table.
 */
typedef struct
{
    uint8  mac[MAC_ADDR_LEN]; /**< [in] The array of MAC address (0x0 to 0xFF for 6 elements) */
    uint16 vid;               /**< [in] VLAN ID (0 to 4095) */
    uint8  port;              /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    uint16 fid;               /**< [in] Filter ID (0 to 14) */
    uint8  ulfid;             /**< [in] Use FID for hash (0 or 1) */
    uint8  nh;                /**< [in] Use for routing next hop (0 to 1) */
} LutUniAddInfo_t;

/**
 * @brief This structure is used to add a multicast entry to the lookup address table.
 */
typedef struct
{
    uint8   mac[MAC_ADDR_LEN]; /**< [in] The array of MAC address (0x0 to 0xFF for 6 elements) */
    RtkPmap portmask;          /**< [in] Port mask (N/A) */
    RtkVlan vid;               /**< [in] VLAN ID (0 to 4095) */
    VlanFid fid;               /**< [in] Filter ID (0 to 14) */
} LutMultiAddInfo_t;

/**
 * @brief This structure is used to delete an entry from the lookup address table.
 */
typedef struct
{
    uint8  mac[MAC_ADDR_LEN]; /**< [in] The array of MAC address (0x0 to 0xFF for 6 elements) */
    uint16 vid;               /**< [in] VLAN ID (0 to 4095) */
    uint16 fid;               /**< [in] Filter ID (0 to 14) */
    uint8  ulfid;             /**< [in] Use FID for hash (0 or 1) */
} LutDeleteInfo_t;

/**
 * @brief This structure is used to delete an IP multicast entry from the address table.
 */
typedef struct
{
    uint8 sip[IPV4_ADDR_LEN]; /**< [in] The array of source IP address (0 to 255 for 4 elements) */
    uint8 dip[IPV4_ADDR_LEN]; /**< [in] The array of destination IP address (0 to 255 for 4 elements) */
} LutIpMultiDeleteInfo_t;

/**
 * @brief This structure is used to set/get the SA move legal ports of the specified port.
 */
typedef struct
{
    RtkPort port;     /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkPmap portmask; /**< [in,out] Port mask (N/A) */
} LutSaMoveLegalPortInfo_t;

/**
 * @brief This structure is used to set/get the SA move legal port action of the specified port.
 */
typedef struct
{
    RtkPort        port;         /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    LegalPortAct_e legalPortAct; /**< [in,out] The action of legal port (EM_LEGAL_FWD to EM_LEGAL_TRAP_CPU) */
} LutSaMoveLegalPortActInfo_t;

/**
 * @brief This structure is used to set/get the SA move illegal port action of the specified port.
 */
typedef struct
{
    RtkPort          port;           /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    IllegalPortAct_e illegalPortAct; /**< [in,out] The action of illegal port (EM_ILLEGAL_DROP to EM_ILLEGAL_TRAP_CPU) */
} LutSaMoveIllegalPortActInfo_t;

/**
 * @brief This structure is used to set/get the SA move CPU ports.
 */
typedef struct
{
    RtkPmap portmask; /**< [in,out] Port mask (N/A) */
} LutSaMoveCpuPortInfo_t;

/**
 * @brief This structure is used to enable/disable the legal SA move record for a specific port.
 */
typedef struct
{
    uint8   enable; /**< [in] The legal SA move record enable (0 to 1) */
    RtkPort port;   /**< [in] Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
} LutLegalSaMoveRecordInfo_t;

/**
 * @brief This structure is used to enable/disable the illegal SA move record for a specific port.
 */
typedef struct
{
    uint8   enable; /**< [in] The illegal SA move record enable (0 to 1) */
    RtkPort port;   /**< [in] Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
} LutIllegalSaMoveRecordInfo_t;

/**
 * @brief This structure is used to get the legal SA move record.
 */
typedef struct
{
    uint8          idx;        /**< [in]  The index of legal SA move record  (0 to 5) */
    SaMoveRecord_t moveRecord; /**< [out] Legal SA move record (N/A) */
} LutLegalSaMoveRecordGetInfo_t;

/**
 * @brief This structure is used to get the illegal SA move record.
 */
typedef struct
{
    uint8          idx;        /**< [in]  The index of illegal SA move record  (0 to 5) */
    SaMoveRecord_t moveRecord; /**< [out] Illegal SA move record (N/A) */
} LutIllegalSaMoveRecordGetInfo_t;

EXTERN_API RtkApiRet rtk_lut_hash_algo_get(UnitChip_t unitChip, LutHashAlgoInfo_t *lutHashAlgoInfo);
EXTERN_API RtkApiRet rtk_lut_entry_get(UnitChip_t unitChip, LutGetInfo_t *lutGetInfo);
EXTERN_API RtkApiRet rtk_lut_entry_search(UnitChip_t unitChip, LutSearchInfo_t *lutSearchInfo);
EXTERN_API RtkApiRet rtk_lut_multi_entry_get(UnitChip_t unitChip, LutMultiGetInfo_t *lutMultiGetInfo);
EXTERN_API RtkApiRet rtk_lut_ip_multi_entry_get(UnitChip_t unitChip, LutIpMultiInfo_t *lutIpMultiInfo);
EXTERN_API RtkApiRet rtk_lut_cam_entry_get(UnitChip_t unitChip, LutCamGetInfo_t *lutCamGetInfo);
EXTERN_API RtkApiRet rtk_lut_lookup_miss_ctrl(UnitChip_t unitChip, LutLookupMissCtrlInfo_t *lutLookupMissCtrlInfo);
EXTERN_API RtkApiRet rtk_lut_mac_lrn_en(UnitChip_t unitChip, LutMacLrnInfo_t *lutMacLrnInfo);
EXTERN_API RtkApiRet rtk_lut_uni_entry_add(UnitChip_t unitChip, LutUniAddInfo_t *lutUniAddInfo);
EXTERN_API RtkApiRet rtk_lut_multi_entry_add(UnitChip_t unitChip, LutMultiAddInfo_t *lutMultiAddInfo);
EXTERN_API RtkApiRet rtk_lut_ip_multi_entry_add(UnitChip_t unitChip, LutIpMultiInfo_t *lutIpMultiInfo);
EXTERN_API RtkApiRet rtk_lut_entry_delete(UnitChip_t unitChip, LutDeleteInfo_t *lutDeleteInfo);
EXTERN_API RtkApiRet rtk_lut_ip_multi_entry_delete(UnitChip_t unitChip, LutIpMultiDeleteInfo_t *lutIpMultiDeleteInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_legal_port_get(UnitChip_t unitChip, LutSaMoveLegalPortInfo_t *lutSaMoveLegalPortInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_legal_port_set(UnitChip_t unitChip, LutSaMoveLegalPortInfo_t *lutSaMoveLegalPortInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_legal_port_get_act(UnitChip_t unitChip, LutSaMoveLegalPortActInfo_t *lutSaMoveLegalPortActInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_legal_port_set_act(UnitChip_t unitChip, LutSaMoveLegalPortActInfo_t *lutSaMoveLegalPortActInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_illegal_port_get_act(UnitChip_t unitChip, LutSaMoveIllegalPortActInfo_t *lutSaMoveIllegalPortActInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_illegal_port_set_act(UnitChip_t unitChip, LutSaMoveIllegalPortActInfo_t *lutSaMoveIllegalPortActInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_cpu_port_get(UnitChip_t unitChip, LutSaMoveCpuPortInfo_t *lutSaMoveCpuPortInfo);
EXTERN_API RtkApiRet rtk_lut_sa_move_cpu_port_set(UnitChip_t unitChip, LutSaMoveCpuPortInfo_t *lutSaMoveCpuPortInfo);
EXTERN_API RtkApiRet rtk_lut_legal_sa_move_record_en(UnitChip_t unitChip, LutLegalSaMoveRecordInfo_t *lutLegalSaMoveRecordInfo);
EXTERN_API RtkApiRet rtk_lut_illegal_sa_move_record_en(UnitChip_t unitChip, LutIllegalSaMoveRecordInfo_t *lutIllegalSaMoveRecordInfo);
EXTERN_API RtkApiRet rtk_lut_legal_sa_move_get_record(UnitChip_t unitChip, LutLegalSaMoveRecordGetInfo_t *lutLegalSaMoveRecordGetInfo);
EXTERN_API RtkApiRet rtk_lut_illegal_sa_move_get_record(UnitChip_t unitChip, LutIllegalSaMoveRecordGetInfo_t *lutIllegalSaMoveRecordGetInfo);

#endif /* RTKAS_API_L2_H__ */