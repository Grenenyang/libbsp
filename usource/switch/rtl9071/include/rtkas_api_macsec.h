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

#ifndef RTKAS_API_MACSEC_H__
#define RTKAS_API_MACSEC_H__
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_types.h"
#include "rtkas_error.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define RTK_MACSEC_CIPHERSUITE_GCM_128     (0u)
#define RTK_MACSEC_CIPHERSUITE_GCM_256     (1u)
#define RTK_MACSEC_CIPHERSUITE_GCM_XPN_128 (2u)
#define RTK_MACSEC_CIPHERSUITE_GCM_XPN_256 (3u)
#define RTK_MACSEC_CIPHERSUITE_GCM_NONE    (4u)

#define RTK_MACSEC_SOURCE_PORT_COMMON_PORT       (0u)
#define RTK_MACSEC_SOURCE_PORT_RESERVED_PORT     (1u)
#define RTK_MACSEC_SOURCE_PORT_CONTROLLED_PORT   (2u)
#define RTK_MACSEC_SOURCE_PORT_UNCONTROLLED_PORT (3u)

#define RTK_MACSEC_VALID_FRAME_DISABLED (0u)
#define RTK_MACSEC_VALID_FRAME_CHECK    (1u)
#define RTK_MACSEC_VALID_FRAME_STRICT   (2u)
#define RTK_MACSEC_VALID_FRAME_RESERVED (3u)

#define RTK_MACSEC_SA_MATCH_SOURCE_MAC      (0u)
#define RTK_MACSEC_SA_MATCH_DESTINATION_MAC (1u)
#define RTK_MACSEC_SA_MATCH_OUTER_VLAN_ID   (2u)
#define RTK_MACSEC_SA_MATCH_OUTER_VLAN_PCP  (3u)
#define RTK_MACSEC_SA_MATCH_INNER_VLAN_ID   (4u)
#define RTK_MACSEC_SA_MATCH_INNER_VLAN_PCP  (5u)
#define RTK_MACSEC_SA_MATCH_ETHER_TYPE      (6u)
#define RTK_MACSEC_SA_MATCH_SOURCE_PORT     (7u)
#define RTK_MACSEC_SA_MATCH_SCI             (8u)
#define RTK_MACSEC_SA_MATCH_AN_0            (9u)
#define RTK_MACSEC_SA_MATCH_AN_1            (10u)
#define RTK_MACSEC_SA_MATCH_FLOW_CTRL       (11u)

#define RTK_MACSEC_VLAN_PARSER_DEFAULT_OUTER_TPID (0x88A8u)
#define RTK_MACSEC_VLAN_PARSER_DEFAULT_INNER_TPID (0x8100u)

#define RTK_MACSEC_VLAN_PARSER_OUTER_TPID (0u)
#define RTK_MACSEC_VLAN_PARSER_INNER_TPID (1u)

#define RTK_MACSEC_MIN_ENTRY_PRIORITY_VALUE (0u)
#define RTK_MACSEC_MAX_ENTRY_PRIORITY_VALUE (15u)

#define RTK_MACSEC_MIN_VLAN_ID_VALUE (0u)
#define RTK_MACSEC_MAX_VLAN_ID_VALUE (4095u)

#define RTK_MACSEC_MIN_VLAN_PCP_VALUE (0u)
#define RTK_MACSEC_MAX_VLAN_PCP_VALUE (7u)

#define RTK_MACSEC_MIN_AN_VALUE (0u)
#define RTK_MACSEC_MAX_AN_VALUE (3u)

#define RTK_MACSEC_MIN_AN_0_VALUE (0u)
#define RTK_MACSEC_MAX_AN_0_VALUE (1u)

#define RTK_MACSEC_MIN_ENTRY_IDX (0u)
#define RTK_MACSEC_MAX_ENTRY_IDX (31u)

#define RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX (0u)
#define RTK_MACSEC_MAX_CTRL_PACKET_ENTRY_IDX (7u)

#define RTK_MACSEC_MIN_BYPASS_TAG_SIZE (0u)
#define RTK_MACSEC_MAX_BYPASS_TAG_SIZE (2u)

#define RTK_MACSEC_MIN_CONFIDENTIAL_OFFSET (0u)
#define RTK_MACSEC_MAX_CONFIDENTIAL_OFFSET (127u)

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/**
 * @brief The direction of MACsec Engine
 */
typedef enum
{
    EM_RTK_MACSEC_DIRECTION_TX, /**< The egress direction of MACsec Engine  */
    EM_RTK_MACSEC_DIRECTION_RX  /**< The ingress direction of MACsec Engine */
} RtkMacsecDirection_e;

/**
 * @brief The VLAN parser type
 */
typedef enum
{
    EM_RTK_MACSEC_VLAN_PARSER_TYPE_SINGLE_TAG, /**< Single tag type VLAN parser */
    EM_RTK_MACSEC_VLAN_PARSER_TYPE_DOUBLE_TAG, /**< Double tag type VLAN parser */
    EM_RTK_MACSEC_VLAN_PARSER_TYPE_END         /**< The end of VLAN parser type */
} RtkMacsecVlanParserType_e;

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/**
 * @brief Structure for MACsec SA match entry parameters
 */
typedef struct
{
    uint16 modifiedMask;                /**< Identify which parameter is changed in setter API, reserved for getter API (0x0 to 0xFFF) */
    uint16 enableMask;                  /**< Identify which parameter need to be compared for a specific entry (0x0 to 0xFFF) */
    uint8  an;                          /**< MACsec AN byte compare value (RTK_MACSEC_MIN_AN_VALUE to RTK_MACSEC_MAX_AN_VALUE) */
    uint8  sourcePort;                  /**< Source port compare value (RTK_MACSEC_SOURCE_PORT_COMMON_PORT to RTK_MACSEC_SOURCE_PORT_UNCONTROLLED_PORT) */
    uint8  outerVlanPcp;                /**< Outer VLAN PCP compare value (RTK_MACSEC_MIN_VLAN_PCP_VALUE to RTK_MACSEC_MAX_VLAN_PCP_VALUE) */
    uint8  innerVlanPcp;                /**< Inner VLAN PCP compare value (RTK_MACSEC_MIN_VLAN_PCP_VALUE to RTK_MACSEC_MAX_VLAN_PCP_VALUE) */
    uint8  flowCtrlIdx;                 /**< The index of flow control entry, valid value range: 0 ~ 31 */
    uint8  reservd;                     /**< Reserved for parameters are 4-byte alignment */
    uint16 outerVlanid;                 /**< Outer VLAN ID compare value (RTK_MACSEC_MIN_VLAN_ID_VALUE to RTK_MACSEC_MAX_VLAN_ID_VALUE) */
    uint16 innerVlanid;                 /**< Inner VLAN ID compare value (RTK_MACSEC_MIN_VLAN_ID_VALUE to RTK_MACSEC_MAX_VLAN_ID_VALUE) */
    uint16 etherType;                   /**< Ether type compare value (0x0 to 0xFFFF) */
    uint32 entryPriority;               /**< Entry priority (RTK_MACSEC_MIN_ENTRY_PRIORITY_VALUE to RTK_MACSEC_MAX_ENTRY_PRIORITY_VALUE) */
    uint8  sci[8];                      /**< SCI compare 64 bits value (0x0 to 0xFF for 8 elements) */
    uint8  macSrcAddr[ETHER_ADDR_LEN];  /**< Source MAC address (0x0 to 0xFF for 32 elements) */
    uint8  macDestAddr[ETHER_ADDR_LEN]; /**< Destination MAC address (0x0 to 0xFF for 32 elements) */
} RtkMacsecSaMatchEntry_t;

/**
 * @brief Structure for MACsec flow control parameters
 */
typedef struct
{
    bool  replayProtect;         /**< Enables or disables replay protection, ingress only (false to true) */
    bool  protectFrame;          /**< Enables or disables protection of transmitted frames, egress only (false to true) */
    bool  saInUse;               /**< MACsec SA in use for the looked-up SA (false to true) */
    bool  includeSci;            /**< Force to include SCI in the SecTag, egress only (false to true) */
    bool  useEs;                 /**< Enables use of the ES bit in the SecTAG, egress only (false to true) */
    bool  useScb;                /**< Enables use of the SCB bit in the SecTAG, egress only (false to true) */
    bool  confProtect;           /**< Enables confidentiality protection, egress only (false to true) */
    uint8 validateFrames;        /**< Control validation of received frames, ingress only (0 to 3) */
    uint8 confidentialityOffset; /**< The confidentiality protection offset options for the selected cipher suite (0 to 127) */
    uint8 bypassTagSize;         /**< The number of DV1_VLAN tags will be bypassed, egress only (0 to 2) */
    uint8 saEntryIdx;            /**< The Index of the associated SA entry (0 to 31) */
} RtkMacsecFlowControl_t;

/**
 * @brief Structure for MACsec security association parameters
 */
typedef struct
{
    uint8  an;           /**< This is the AN (Association Number) of this SA, egress only (0 to 3) */
    uint8  reserved;     /**< This is to keep other fields aligned to 4 bytes (0 to 0) */
    uint8  key[32];      /**< This is the AES encryption key for the MACsec SA in big-endian order. For GCM-128 and GCM-XPN-128, the key[4] ~ key[7] are reserved (0x0 to 0xFF for 32 elements) */
    uint8  hkey[16];     /**< This is hkey = E(Key, 0^128), which is the result of 128-bit AES-ECB encryption operation with key and a block of 128 zero bits as plaintext (0x0 to 0xFF for 16 elements) */
    uint64 pn;           /**< This is the packet number. For a new Egress MACsec, this should be initialized to 0 thus the PN in packet will start from 1. For ingress MACsec, this should be initialized to 1 (0x0 to 0xFFFFFFFFFFFFFFFF) */
    uint32 replayWindow; /**< This is the replay window size which controls the range of packet numbers considered valid, ingress only (0x0 to 0xFFFFFFFF) */
    uint8  sci[8];       /**< This is the SCI that belongs to the specific MACsec SA in big endian order (0x0 to 0xFF for 8 elements) */
    uint8  ssci[4];      /**< This is the SSCI of the specific MACsec SA, used for set XPN type only (0x0 to 0xFF for 4 elements) */
    uint8  salt[12];     /**< This is a 96-bit salt described in IEEE 802.1AEbw, used for XPN type cipher suite (0x0 to 0xFF for 12 elements). For set API: This is salt [0:95] value in big endian. For get API: the most significant 32 bits are XOR-ed with SSCI field, the lower 64-bit are salt[32:95] */
} RtkMacsecSecurityAssociation_t;

/**
 * @brief Structure for MACsec Rx global counters
 */
typedef struct
{
    uint64 transformErrorPkts;                /**< The ingress packet counters of packets that transform error internally */
    uint64 inPktsCtrl;                        /**< The ingress packet counters of control packets */
    uint64 inPktsNoTag;                       /**< The ingress packet counters of packets that do not have valid SecTag and the validation level is not strict */
    uint64 inPktsUntagged;                    /**< The ingress packet counters of packets that do not have valid SecTag and the validation level is strict */
    uint64 inPktsTagged;                      /**< The ingress packet counters of packets that have valid SecTag */
    uint64 inPktsBadTag;                      /**< The ingress packet counters of packets that have invalid SecTag */
    uint64 inPktsUntaggedMiss;                /**< The ingress packet counters of packets that do not have SecTag and no SA is found to match */
    uint64 inPktsNoSci;                       /**< The ingress packet counters of packets that have valid SecTag but are SA non-matched, and the validation level is strict */
    uint64 inPktsUnknownSci;                  /**< The ingress packet counters of packets that have valid SecTag but are SA non-matched, and the validation level is not strict */
    uint64 inConsistCheckControlledNotPass;   /**< The ingress packet counters of packets that are internally used */
    uint64 inConsistCheckUncontrolledNotPass; /**< The ingress packet counters of packets that are internally used */
    uint64 inConsistCheckControlledPass;      /**< The ingress packet counters of packets that are internally used */
    uint64 inConsistCheckUncontrolledPass;    /**< The ingress packet counters of packets that are internally used */
    uint64 inOverSize;                        /**< The ingress packet counters of packets that exceeded the MTU size */
} RtkMacsecRxGlobalCounter_t;

/**
 * @brief Structure for MACsec Tx global counters
 */
typedef struct
{
    uint64 transformErrorPkts; /**< The egress packet counters of packets that transform error internally */
    uint64 outPktsCtrl;        /**< The egress packet counters of control packets */
    uint64 outPktsUnknownSa;   /**< The egress packet counters of packets that are unknown SA packets */
    uint64 outPktsUntagged;    /**< The egress packet counters of packets that are not MACsec protected */
    uint64 outOversizePkts;    /**< The egress packet counters of packets that are too long */
} RtkMacsecTxGlobalCounter_t;

/**
 * @brief Structure for MACsec Rx SA counters
 */
typedef struct
{
    uint64 inOctetsDecrypted; /**< The ingress byte counters of decrypted or validated packets */
    uint64 inPktsUnchecked;   /**< The ingress packet counters of packets that have SecTag and ICV but are not validated */
    uint64 inPktsDelayed;     /**< The ingress packet counters of packets that have PN which is under the lowest acceptable PN and the replay protect is disabled */
    uint64 inPktsLate;        /**< The ingress packet counters of packets that do not pass the replay protection */
    uint64 inPktsOk;          /**< The ingress packet counters of packets that pass all MACsec validation */
    uint64 inPktsInvalid;     /**< The ingress packet counters of packets that do not pass the integrity check and the validation level is strict */
    uint64 inPktsNotValid;    /**< The ingress packet counters of packets that do not pass the integrity check and the validation level is not strict */
    uint64 inPktsNotUsingSa;  /**< The ingress packet counters of packets that the SA in use status in flow control entry is configured false and the validation level is strict */
    uint64 inPktsUnusedSa;    /**< The ingress packet counters of packets that the SA in use status in flow control entry is configured false and the validation level is not strict */
    uint64 inPktsUntaggedHit; /**< The ingress packet counters of packets that do not have SecTag but an ingress SA is found to match */
} RtkMacsecRxSaCounter_t;

/**
 * @brief Structure for MACsec Tx SA counters
 */
typedef struct
{
    uint64 outOctetsEncrypted; /**< The egress byte counters of protected or encrypted packets */
    uint64 outPktsEncrypted;   /**< The egress packet counters of protected or encrypted packets */
    uint64 outPktsTooLong;     /**< The egress packet counters of packets that exceeded the MTU size */
} RtkMacsecTxSaCounter_t;

/**
 * @brief Structure for MACsec VLAN parser settings
 */
typedef struct
{
    RtkMacsecVlanParserType_e type;    /**< The VLAN parser type (EM_RTK_MACSEC_VLAN_PARSER_TYPE_SINGLE_TAG to EM_RTK_MACSEC_VLAN_PARSER_TYPE_DOUBLE_TAG) */
    uint16                    tpid[2]; /**< The outer/inner TPID (0x0 to 0xFFFF for 2 elements) */
} RtkMacsecVlanPaser_t;

/**
 * @brief Structure for MACsec match rule parameters
 */
typedef struct
{
    RtkPort                 port;      /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecDirection_e    direction; /**< The ingress or egress direction of MACsec engine (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX) */
    uint8                   entryIdx;  /**< The index of match rule entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
    RtkMacsecSaMatchEntry_t params;    /**< The detailed configuration parameters of this match rule entry */
} MacsecMatchRuleParams_t;

/**
 * @brief Structure for MACsec match rule status
 */
typedef struct
{
    RtkPort              port;      /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecDirection_e direction; /**< The ingress or egress direction of MACsec engine (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX) */
    uint8                entryIdx;  /**< The index of match rule entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
    bool                 enable;    /**< The status of a specific MACsec flow (true to false) */
} MacsecMatchRuleStatus_t;

/**
 * @brief Structure for MACsec flow control entry
 */
typedef struct
{
    RtkPort                port;      /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecDirection_e   direction; /**< The ingress or egress direction of MACsec engine (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX) */
    uint8                  entryIdx;  /**< The index of flow control entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
    RtkMacsecFlowControl_t params;    /**< The detailed configuration parameters of this flow control entry */
} MacsecFlowControlEntry_t;

/**
 * @brief Structure for MACsec SA entry
 */
typedef struct
{
    RtkPort                        port;        /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecDirection_e           direction;   /**< The ingress or egress direction of MACsec engine (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX) */
    uint8                          entryIdx;    /**< The index of SA entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
    uint32                         cipherSuite; /**< The cipher suite of this SA (EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128 to EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256) */
    RtkMacsecSecurityAssociation_t params;      /**< The detailed configuration parameters of this SA entry */
} MacsecSaEntry_t;

/**
 * @brief Structure for MACsec Tx global counter
 */
typedef struct
{
    RtkPort                    port;     /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecTxGlobalCounter_t counters; /**< The counters of egress MACsec entry engine */
} MacsecTxGlobalCounter_t;

/**
 * @brief Structure for clearing MACsec Tx global counter
 */
typedef struct
{
    RtkPort port; /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
} MacsecClearTxGlobalCounter_t;

/**
 * @brief Structure for MACsec Rx global counter
 */
typedef struct
{
    RtkPort                    port;     /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecRxGlobalCounter_t counters; /**< The counters of ingress MACsec entry engine */
} MacsecRxGlobalCounter_t;

/**
 * @brief Structure for clearing MACsec Rx global counter
 */
typedef struct
{
    RtkPort port; /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
} MacsecClearRxGlobalCounter_t;

/**
 * @brief Structure for MACsec Tx SA counter
 */
typedef struct
{
    RtkPort                port;     /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8                  index;    /**< The index of a specific SA entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
    RtkMacsecTxSaCounter_t counters; /**< The counters of per SA egress MACsec entry engine */
} MacsecTxSaCounter_t;

/**
 * @brief Structure for clearing MACsec Tx SA counter
 */
typedef struct
{
    RtkPort port;  /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   index; /**< The index of a specific SA entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
} MacsecClearTxSaCounter_t;

/**
 * @brief Structure for MACsec Rx SA counter
 */
typedef struct
{
    RtkPort                port;     /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8                  index;    /**< The index of a specific SA entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
    RtkMacsecRxSaCounter_t counters; /**< The counters of per SA ingress MACsec entry engine */
} MacsecRxSaCounter_t;

/**
 * @brief Structure for clearing MACsec Rx SA counter
 */
typedef struct
{
    RtkPort port;  /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   index; /**< The index of a specific SA entry (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX) */
} MacsecClearRxSaCounter_t;

/**
 * @brief Structure for clearing all MACsec Tx counters
 */
typedef struct
{
    RtkPort port; /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
} MacsecClearAllTxCounter_t;

/**
 * @brief Structure for clearing all MACsec Rx counters
 */
typedef struct
{
    RtkPort port; /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
} MacsecClearAllRxCounter_t;

/**
 * @brief Structure for MACsec control packet MAC DA entry
 */
typedef struct
{
    RtkPort              port;      /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecDirection_e direction; /**< The ingress or egress direction of MACsec engine (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX) */
    uint8                entryIdx;  /**< Entry index (RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX to RTK_MACSEC_MAX_CTRL_PACKET_ENTRY_IDX) */
    bool                 enable;    /**< The status of this entry (true to false) */
    uint8                mac[6];    /**< A specific destination MAC address that will be classified as control packet (0x0 to 0xFF for 6 elements) */
} MacsecControlPacketMacDaEntry_t;

/**
 * @brief Structure for MACsec control packet EtherType entry
 */
typedef struct
{
    RtkPort              port;      /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecDirection_e direction; /**< The ingress or egress direction of MACsec engine (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX) */
    uint8                entryIdx;  /**< Entry index (RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX to RTK_MACSEC_MAX_CTRL_PACKET_ENTRY_IDX) */
    bool                 enable;    /**< The status of this entry (true to false) */
    uint16               etherType; /**< A specific EtherType value that will be classified as control packet (0x0 to 0xFFFF) */
} MacsecControlPacketEtherTypeEntry_t;

/**
 * @brief Structure for MACsec VLAN parser settings
 */
typedef struct
{
    RtkPort              port;       /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMacsecDirection_e direction;  /**< The ingress or egress direction of MACsec engine (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX) */
    RtkMacsecVlanPaser_t vlanParser; /**< VLAN parser settings to control VLAN single or double tag parsing and the corresponding TPID */
} MacsecVlanParserSettings_t;

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_macsec_get_match_rule_params(UnitChip_t unitChip, MacsecMatchRuleParams_t *matchRuleParams);
EXTERN_API RtkApiRet rtk_macsec_get_match_rule_vlan_params(UnitChip_t unitChip, MacsecMatchRuleParams_t *matchRuleParams);
EXTERN_API RtkApiRet rtk_macsec_get_match_rule_misc_params(UnitChip_t unitChip, MacsecMatchRuleParams_t *matchRuleParams);
EXTERN_API RtkApiRet rtk_macsec_set_match_rule_params(UnitChip_t unitChip, const MacsecMatchRuleParams_t *matchRuleParams);
EXTERN_API RtkApiRet rtk_macsec_set_match_rule_vlan_params(UnitChip_t unitChip, const MacsecMatchRuleParams_t *matchRuleParams);
EXTERN_API RtkApiRet rtk_macsec_set_match_rule_misc_params(UnitChip_t unitChip, const MacsecMatchRuleParams_t *matchRuleParams);

EXTERN_API RtkApiRet rtk_macsec_get_match_rule_status(UnitChip_t unitChip, MacsecMatchRuleStatus_t *matchRuleStatus);
EXTERN_API RtkApiRet rtk_macsec_set_match_rule_status(UnitChip_t unitChip, const MacsecMatchRuleStatus_t *matchRuleStatus);

EXTERN_API RtkApiRet rtk_macsec_get_flow_control_entry(UnitChip_t unitChip, MacsecFlowControlEntry_t *flowControlEntry);
EXTERN_API RtkApiRet rtk_macsec_set_flow_control_entry(UnitChip_t unitChip, const MacsecFlowControlEntry_t *flowControlEntry);

EXTERN_API RtkApiRet rtk_macsec_get_sa_entry(UnitChip_t unitChip, MacsecSaEntry_t *saEntry);
EXTERN_API RtkApiRet rtk_macsec_set_sa_entry(UnitChip_t unitChip, const MacsecSaEntry_t *saEntry);

EXTERN_API RtkApiRet rtk_macsec_get_tx_global_counter(UnitChip_t unitChip, MacsecTxGlobalCounter_t *txGlobalCounter);
EXTERN_API RtkApiRet rtk_macsec_clear_tx_global_counter(UnitChip_t unitChip, const MacsecClearTxGlobalCounter_t *txGlobalCounter);

EXTERN_API RtkApiRet rtk_macsec_get_rx_global_counter(UnitChip_t unitChip, MacsecRxGlobalCounter_t *rxGlobalCounter);
EXTERN_API RtkApiRet rtk_macsec_clear_rx_global_counter(UnitChip_t unitChip, const MacsecClearRxGlobalCounter_t *rxGlobalCounter);

EXTERN_API RtkApiRet rtk_macsec_get_tx_sa_counter(UnitChip_t unitChip, MacsecTxSaCounter_t *txSaCounter);
EXTERN_API RtkApiRet rtk_macsec_clear_tx_sa_counter(UnitChip_t unitChip, const MacsecClearTxSaCounter_t *txSaCounter);

EXTERN_API RtkApiRet rtk_macsec_get_rx_sa_counter(UnitChip_t unitChip, MacsecRxSaCounter_t *rxSaCounter);
EXTERN_API RtkApiRet rtk_macsec_clear_rx_sa_counter(UnitChip_t unitChip, const MacsecClearRxSaCounter_t *rxSaCounter);

EXTERN_API RtkApiRet rtk_macsec_clear_all_tx_counter(UnitChip_t unitChip, const MacsecClearAllTxCounter_t *txCounter);
EXTERN_API RtkApiRet rtk_macsec_clear_all_rx_counter(UnitChip_t unitChip, const MacsecClearAllRxCounter_t *rxCounter);

EXTERN_API RtkApiRet rtk_macsec_get_control_packet_mac_da_entry(UnitChip_t unitChip, MacsecControlPacketMacDaEntry_t *controlPacketMacEntry);
EXTERN_API RtkApiRet rtk_macsec_set_control_packet_mac_da_entry(UnitChip_t unitChip, const MacsecControlPacketMacDaEntry_t *controlPacketMacEntry);

EXTERN_API RtkApiRet rtk_macsec_get_control_packet_ether_type_entry(UnitChip_t unitChip, MacsecControlPacketEtherTypeEntry_t *controlPacketEtherTypeEntry);
EXTERN_API RtkApiRet rtk_macsec_set_control_packet_ether_type_entry(UnitChip_t unitChip, const MacsecControlPacketEtherTypeEntry_t *controlPacketEtherTypeEntry);

EXTERN_API RtkApiRet rtk_macsec_get_vlan_parser_settings(UnitChip_t unitChip, MacsecVlanParserSettings_t *vlanParserSettings);
EXTERN_API RtkApiRet rtk_macsec_set_vlan_parser_settings(UnitChip_t unitChip, const MacsecVlanParserSettings_t *vlanParserSettings);

#endif /* RTKAS_API_MACSEC_H__ */