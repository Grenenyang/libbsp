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

#ifndef RTKAS_API_VLAN_H__
#define RTKAS_API_VLAN_H__

typedef uint32 VlanFid; /**< The data type of the filtering database identifier (0 to 15) */
typedef uint32 VlanPri; /**< Priority value (0 to 7) */
typedef uint32 VlanDei; /**< DEI value (0 to 1) */

/* Acceptable frame type of VLAN */
typedef enum
{
    EM_ACCEPT_FRAME_TYPE_ALL = 0,    /**< Accept untagged, priority-tagged, and tagged packets */
    EM_ACCEPT_FRAME_TYPE_TAG_ONLY,   /**< Accept tagged packets */
    EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY, /**< Accept untagged and priority-tagged packets */
    EM_ACCEPT_FRAME_TYPE_NONE,       /**< Don't accept tagged packets */
    EM_ACCEPT_FRAME_TYPE_END
} VlanAcceptFrameType_e;

/* Acceptable double tag mode of VLAN */
typedef enum
{
    EM_ACCEPT_DOUBLE_TAG = 0, /**< Accept double tag */
    EM_DENY_DOUBLE_TAG,       /**< Deny double tag */
    EM_ACCEPT_DOUBLE_TAG_END
} VlanAcceptDoubleTag_e;

/* Tag mode of VLAN */
typedef enum
{
    EM_VLAN_TAG_MODE_ORIGINAL = 0,     /**< Tx tag keeps original format */
    EM_VLAN_TAG_MODE_KEEP_FORMAT,      /**< Keep tag format if ALE has not determined */
    EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT, /**< Keep tag format whether ALE has determined or not */
    EM_VLAN_TAG_MODE_END
} VlanTagMode_e;

/* Tag status of VLAN */
typedef enum
{
    EM_TAG_STATUS_WITH_TAG = 0, /**< Tx with CVLAN tag */
    EM_TAG_STATUS_WITHOUT_TAG,  /**< Tx without CVLAN tag */
    EM_TAG_STATUS_END
} VlanTxTagStatus_e;

/* Unicast MAC address lookup control of VLAN entry */
typedef enum
{
    EM_VLAN_IVL = 0, /**< Independent VLAN learning */
    EM_VLAN_SVL,     /**< Shared VLAN learning */
    EM_VLAN_IVLSVL_END
} VlanIvlSvl_e;

/* Forwarding control of VLAN entry */
typedef enum
{
    EM_ALE_BASED_FWD = 0, /**< Forwarding based on VLAN and L2 */
    EM_VLAN_BASED_FWD,    /**< Forwarding based on VLAN */
    EM_VLAN_FWDMODE_END
} VlanFwdMode_e;

/* Structure of VLAN entry */
typedef struct
{
    RtkPmap       mbrMsk;         /**< Member port mask (bit array range: <CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */
    RtkPmap       utagMsk;        /**< Untagged port mask (bit array range: <CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */
    RtkPmap       statefulMsk;    /**< Stateful firewall mask (bit array range: <CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */
    VlanFid       fid;            /**< Filtering identifier (0 to 14) */
    VlanIvlSvl_e  ulfid;          /**< Unicast DA lookup uses VID(0) or FID(1) (0 to 1) */
    VlanFwdMode_e vbfwd;          /**< Forwarding based on VLAN+L2(0) or VLAN(1) (0 to 1) */
    uint8         interfaceId;    /**< L3 routing interface ID (0 to 63) */
    uint8         interfaceValid; /**< Indicates if this interface ID is valid(1) or invalid(0) (0 to 1) */
} VlanEntry_t;

/**
 * @brief This structure is used to set/get acceptable inner tag frame type for a specific port.
 */
typedef struct
{
    RtkPort               port;            /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    VlanAcceptFrameType_e acceptFrameType; /**< [in,out] Acceptable frame type (EM_ACCEPT_FRAME_TYPE_ALL to EM_ACCEPT_FRAME_TYPE_NONE) */
} VlanPortAcptTypeInfo_t;

/**
 * @brief This structure is used to set/get acceptable outer tag frame type for a specific port.
 */
typedef struct
{
    RtkPort               port;            /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    VlanAcceptFrameType_e acceptFrameType; /**< [in,out] Acceptable frame type (EM_ACCEPT_FRAME_TYPE_ALL to EM_ACCEPT_FRAME_TYPE_NONE) */
} SvlanPortAcptTypeInfo_t;

/**
 * @brief This structure is used to set/get acceptable double tag for a specific port.
 */
typedef struct
{
    RtkPort               port;            /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    VlanAcceptDoubleTag_e acceptDoubleTag; /**< [in,out] Accept double tag (EM_ACCEPT_DOUBLE_TAG to EM_DENY_DOUBLE_TAG) */
} VlanPortAcptDoubleTagInfo_t;

/**
 * @brief This structure is used to set/get the configuration of ignoring VLAN tag function for a specific port.
 */
typedef struct
{
    RtkPort   port;    /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable enabled; /**< [in,out] Enable or disable the ignoring VLAN tag function (<CHIPID>_DISABLED to <CHIPID>_ENABLED) */
} VlanTagAwareInfo_t;

/**
 * @brief This structure is used to set/get the default VLAN ID and priority of inner VLAN tag for a specific port.
 */
typedef struct
{
    RtkPort port;     /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkVlan pvid;     /**< [in,out] IEEE 802.1Q port-based VLAN ID (1 to 0xFFFu) */
    VlanPri priority; /**< [in,out] IEEE 802.1p VLAN priority for the "pvid" (0 to 7) */
} VlanPortPvidInfo_t;

/**
 * @brief This structure is used to set/get the default VLAN ID, priority and DEI of outer VLAN tag for a specific port.
 */
typedef struct
{
    RtkPort port;     /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkVlan pvid;     /**< [in,out] IEEE 802.1Q port-based VLAN ID (1 to 0xFFFu) */
    VlanPri priority; /**< [in,out] IEEE 802.1p VLAN priority for the "pvid" (0 to 7) */
    VlanDei dei;      /**< [in,out] IEEE 802.1Q drop eligible indicator (0 to 1) */
} SvlanPortPvidInfo_t;

/**
 * @brief This structure is used to set/get VLAN entry.
 */
typedef struct
{
    RtkVlan     vid;       /**< [in]     VLAN ID (0 to 0xFFFu) */
    VlanEntry_t vlanEntry; /**< [in,out] VLAN entry data (N/A) */
} VlanInfo_t;

/**
 * @brief This structure is used to destroy a VLAN entry in the VLAN table.
 */
typedef struct
{
    RtkVlan vid; /**< [in] VLAN ID (0 to 0xFFFu) */
} VlanDestroyInfo_t;

/**
 * @brief This structure is used to set/get whether to enable the outer tag parser ability.
 */
typedef struct
{
    RtkPort   port;   /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable enable; /**< [in,out] Enable or disable (0 to 1) */
} SvlanPortPktParserInfo_t;

/**
 * @brief This structure is used to set/get whether forwarding is based on outer VID or inner VID.
 */
typedef struct
{
    RtkPort   port;   /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable enable; /**< [in,out] Enable or disable (0 to 1) */
} SvlanFwdBaseOvidInfo_t;

/**
 * @brief This structure is used to set/get ingress VLAN filter for a specific port.
 */
typedef struct
{
    RtkPort   port;      /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable igrFilter; /**< [in,out] Ingress filter (0 to 1) */
} VlanPortIgrFilterInfo_t;

/**
 * @brief This structure is used to set/get an ingress VLAN retagging entry.
 */
typedef struct
{
    uint32  idx;         /**< [in]     Index (0 to 31) */
    uint8   valid;       /**< [in,out] Valid bit (0 to 1) */
    RtkPmap pbm;         /**< [in,out] Ingress member port mask (N/A) */
    RtkVlan originalVid; /**< [in,out] Original VLAN ID (0 to 0xFFFu) */
    RtkVlan newVid;      /**< [in,out] New VLAN ID (0 to 0xFFFu) */
} VlanIgrRetaggingInfo_t;

/**
 * @brief This structure is used to set/get ingress VLAN retagging status.
 */
typedef struct
{
    RtkPort   port;   /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable enable; /**< [in,out] Enable or disable the ingress retagging function (0 to 1) */
} VlanIgrRetaggingEnInfo_t;

/**
 * @brief This structure is used to set/get inner tag status for a specific egress port.
 */
typedef struct
{
    RtkPort           port;      /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    VlanTxTagStatus_e tagStatus; /**< [in,out] Inner tag status of the egress port (0 to 2) */
} VlanPortEgrTagStatusInfo_t;

/**
 * @brief This structure is used to set/get outer tag status for a specific egress port.
 */
typedef struct
{
    RtkPort           port;      /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    VlanTxTagStatus_e tagStatus; /**< [in,out] Outer tag status of the egress port (0 to 2) */
} SvlanPortEgrTagStatusInfo_t;

/**
 * @brief This structure is used to set/get egress VLAN filter status for a specific port.
 */
typedef struct
{
    RtkPort   port;      /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable egrFilter; /**< [in,out] Egress filter (0 to 1) */
} VlanPortEgrFilterInfo_t;

/**
 * @brief This structure is used to set/get an egress VLAN retagging entry.
 */
typedef struct
{
    uint32  idx;         /**< [in]     Index (0 to 31) */
    uint8   valid;       /**< [in,out] Valid bit (0 to 1) */
    RtkPmap pbm;         /**< [in,out] Ingress member port mask (N/A) */
    RtkVlan originalVid; /**< [in,out] Original VLAN ID (0 to 0xFFFu) */
    RtkVlan newVid;      /**< [in,out] New VLAN ID (0 to 0xFFFu) */
} VlanEgrRetaggingInfo_t;

/**
 * @brief This structure is used to set/get egress VLAN retagging status.
 */
typedef struct
{
    RtkPort   port;   /**< [in]     Port ID (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable enable; /**< [in,out] Enable or disable the egress retagging function (0 to 1) */
} VlanEgrRetaggingEnInfo_t;

EXTERN_API RtkApiRet rtk_vlan_port_acpt_type_set(UnitChip_t unitChip, VlanPortAcptTypeInfo_t *vlanPortAcptTypeInfo);
EXTERN_API RtkApiRet rtk_vlan_port_acpt_type_get(UnitChip_t unitChip, VlanPortAcptTypeInfo_t *vlanPortAcptTypeInfo);
EXTERN_API RtkApiRet rtk_svlan_port_acpt_type_set(UnitChip_t unitChip, SvlanPortAcptTypeInfo_t *svlanPortAcptTypeInfo);
EXTERN_API RtkApiRet rtk_svlan_port_acpt_type_get(UnitChip_t unitChip, SvlanPortAcptTypeInfo_t *svlanPortAcptTypeInfo);
EXTERN_API RtkApiRet rtk_vlan_port_acpt_double_tag_set(UnitChip_t unitChip, VlanPortAcptDoubleTagInfo_t *vlanPortAcptDoubleTagInfo);
EXTERN_API RtkApiRet rtk_vlan_port_acpt_double_tag_get(UnitChip_t unitChip, VlanPortAcptDoubleTagInfo_t *vlanPortAcptDoubleTagInfo);
EXTERN_API RtkApiRet rtk_vlan_tag_aware_set(UnitChip_t unitChip, VlanTagAwareInfo_t *vlanTagAwareInfo);
EXTERN_API RtkApiRet rtk_vlan_tag_aware_get(UnitChip_t unitChip, VlanTagAwareInfo_t *vlanTagAwareInfo);
EXTERN_API RtkApiRet rtk_vlan_port_pvid_set(UnitChip_t unitChip, VlanPortPvidInfo_t *vlanPortPvidInfo);
EXTERN_API RtkApiRet rtk_vlan_port_pvid_get(UnitChip_t unitChip, VlanPortPvidInfo_t *vlanPortPvidInfo);
EXTERN_API RtkApiRet rtk_svlan_port_pvid_set(UnitChip_t unitChip, SvlanPortPvidInfo_t *svlanPortPvidInfo);
EXTERN_API RtkApiRet rtk_svlan_port_pvid_get(UnitChip_t unitChip, SvlanPortPvidInfo_t *svlanPortPvidInfo);
EXTERN_API RtkApiRet rtk_vlan_init(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_vlan_set(UnitChip_t unitChip, VlanInfo_t *vlanInfo);
EXTERN_API RtkApiRet rtk_vlan_get(UnitChip_t unitChip, VlanInfo_t *vlanInfo);
EXTERN_API RtkApiRet rtk_vlan_destroy(UnitChip_t unitChip, VlanDestroyInfo_t *vlanDestroyInfo);
EXTERN_API RtkApiRet rtk_svlan_port_pkt_parser_en_set(UnitChip_t unitChip, SvlanPortPktParserInfo_t *svlanPortPktParserInfo);
EXTERN_API RtkApiRet rtk_svlan_port_pkt_parser_en_get(UnitChip_t unitChip, SvlanPortPktParserInfo_t *svlanPortPktParserInfo);
EXTERN_API RtkApiRet rtk_svlan_fwd_base_ovid_set(UnitChip_t unitChip, SvlanFwdBaseOvidInfo_t *svlanFwdBaseOvidInfo);
EXTERN_API RtkApiRet rtk_svlan_fwd_base_ovid_get(UnitChip_t unitChip, SvlanFwdBaseOvidInfo_t *svlanFwdBaseOvidInfo);
EXTERN_API RtkApiRet rtk_vlan_port_igr_filter_en_set(UnitChip_t unitChip, VlanPortIgrFilterInfo_t *vlanPortIgrFilterInfo);
EXTERN_API RtkApiRet rtk_vlan_port_igr_filter_en_get(UnitChip_t unitChip, VlanPortIgrFilterInfo_t *vlanPortIgrFilterInfo);
EXTERN_API RtkApiRet rtk_vlan_igr_retagging_init(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_vlan_igr_retagging_set(UnitChip_t unitChip, VlanIgrRetaggingInfo_t *vlanIgrRetaggingInfo);
EXTERN_API RtkApiRet rtk_vlan_igr_retagging_get(UnitChip_t unitChip, VlanIgrRetaggingInfo_t *vlanIgrRetaggingInfo);
EXTERN_API RtkApiRet rtk_vlan_igr_retagging_en_set(UnitChip_t unitChip, VlanIgrRetaggingEnInfo_t *vlanIgrRetaggingEnInfo);
EXTERN_API RtkApiRet rtk_vlan_igr_retagging_en_get(UnitChip_t unitChip, VlanIgrRetaggingEnInfo_t *vlanIgrRetaggingEnInfo);
EXTERN_API RtkApiRet rtk_vlan_port_egr_tag_status_set(UnitChip_t unitChip, VlanPortEgrTagStatusInfo_t *vlanPortEgrTagStatusInfo);
EXTERN_API RtkApiRet rtk_vlan_port_egr_tag_status_get(UnitChip_t unitChip, VlanPortEgrTagStatusInfo_t *vlanPortEgrTagStatusInfo);
EXTERN_API RtkApiRet rtk_svlan_port_egr_tag_status_set(UnitChip_t unitChip, SvlanPortEgrTagStatusInfo_t *svlanPortEgrTagStatusInfo);
EXTERN_API RtkApiRet rtk_svlan_port_egr_tag_status_get(UnitChip_t unitChip, SvlanPortEgrTagStatusInfo_t *svlanPortEgrTagStatusInfo);
EXTERN_API RtkApiRet rtk_vlan_port_egr_filter_en_set(UnitChip_t unitChip, VlanPortEgrFilterInfo_t *vlanPortEgrFilterInfo);
EXTERN_API RtkApiRet rtk_vlan_port_egr_filter_en_get(UnitChip_t unitChip, VlanPortEgrFilterInfo_t *vlanPortEgrFilterInfo);
EXTERN_API RtkApiRet rtk_vlan_egr_retagging_init(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_vlan_egr_retagging_set(UnitChip_t unitChip, VlanEgrRetaggingInfo_t *vlanEgrRetaggingInfo);
EXTERN_API RtkApiRet rtk_vlan_egr_retagging_get(UnitChip_t unitChip, VlanEgrRetaggingInfo_t *vlanEgrRetaggingInfo);
EXTERN_API RtkApiRet rtk_vlan_egr_retagging_en_set(UnitChip_t unitChip, VlanEgrRetaggingEnInfo_t *vlanEgrRetaggingEnInfo);
EXTERN_API RtkApiRet rtk_vlan_egr_retagging_en_get(UnitChip_t unitChip, VlanEgrRetaggingEnInfo_t *vlanEgrRetaggingEnInfo);

#endif /* RTKAS_API_VLAN_H__ */