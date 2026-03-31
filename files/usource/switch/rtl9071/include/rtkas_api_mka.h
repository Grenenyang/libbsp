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

#ifndef RTKAS_API_MKA_H__
#define RTKAS_API_MKA_H__
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_types.h"
#include "rtkas_error.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define RTK_MKA_MAX_CKN_LEN (32u)

#define RTK_MKA_MIN_CAK_IDX (0u)
#define RTK_MKA_MAX_CAK_IDX (15u)

#define RTK_MKA_KEY_LEN_128_BITS (0u)
#define RTK_MKA_KEY_LEN_256_BITS (1u)

#define RTK_MKA_MIN_PN_EXHAUSTION (0x10000000u)
#define RTK_MKA_MAX_PN_EXHAUSTION (0xDFFFFFFFu)

#define RTK_MKA_MIN_XPN_PN_EXHAUSTION (0x1000000000000000u)
#define RTK_MKA_MAX_XPN_PN_EXHAUSTION (0xDFFFFFFFFFFFFFFFu)

#define RTK_MKA_MIN_BYPASS_VLAN_NUMBER (0u)
#define RTK_MKA_MAX_BYPASS_VLAN_NUMBER (2u)

#define RTK_MKA_MIN_EXTERNAL_PHY_TYPE (1u)
#define RTK_MKA_MAX_EXTERNAL_PHY_TYPE (1u)

#define RTK_MKA_PORT_CONFIG_FLAG                   (0u)
#define RTK_MKA_PORT_CONFIG_REPLAY_PROTECT_WINDOW  (1u)
#define RTK_MKA_PORT_CONFIG_VALIDATE_FRAMES_WINDOW (2u)
#define RTK_MKA_PORT_CONFIG_CA_INDEX               (3u)
#define RTK_MKA_PORT_CONFIG_PRIORITY               (4u)
#define RTK_MKA_PORT_CONFIG_PN_EXHAUSTION          (5u)
#define RTK_MKA_PORT_CONFIG_CIPHER_SUITE           (6u)
#define RTK_MKA_PORT_CONFIG_CONFIDENTIALITY_OFFSET (7u)
#define RTK_MKA_PORT_CONFIG_BYPASS_VLAN_NUMBER     (8u)
#define RTK_MKA_PORT_CONFIG_EXTERNAL_PHY           (9u)

#define RTK_MKA_DEFAULT_HELLO_TIME        (0x2000u)
#define RTK_MKA_DEFAULT_LIFE_TIME         (0x6000u)
#define RTK_MKA_DEFAULT_RETIRE_TIME       (0x6000u)
#define RTK_MKA_DEFAULT_NEW_SAK_TIME      (0x0u)
#define RTK_MKA_DEFAULT_CA_ESTABLISH_TIME (0x0u)

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/**
 * @brief Enumeration for MKA version
 */
typedef enum
{
    EM_RTK_MKA_VERSION_1 = 0,  /**< MKA Version 1(802.1X-2010)                 */
    EM_RTK_MKA_VERSION_2,      /**< MKA Version 2(802.1Xbx-2014), not support  */
    EM_RTK_MKA_VERSION_3,      /**< MKA Version 3(802.1X-2010)                 */
    EM_RTK_MKA_VERSION_UNKNOWN /**< Unknown MKA version                        */
} RtkMkaVersion_e;

/**
 * @brief Enumeration for MKA CAK source
 */
typedef enum
{
    EM_RTK_MKA_CAK_SOURCE_OTP = 0, /**< CAK stored in OTP                      */
    EM_RTK_MKA_CAK_SOURCE_FLASH,   /**< CAK stored in flash and is AES wrapped */
    EM_RTK_MKA_CAK_SOURCE_UNKNOWN  /**< Unknown CAK type                       */
} RtkMkaCakSource_e;

/**
 * @brief Enumeration for MKA cipher suite
 */
typedef enum
{
    EM_RTK_MKA_CIPHER_SUITE_NONE = 0,        /**< MACsec Cipher Suite mode None            */
    EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128,     /**< MACsec Cipher Suite mode GCM-AES-128     */
    EM_RTK_MKA_CIPHER_SUITE_GCM_AES_256,     /**< MACsec Cipher Suite mode GCM-AES-256     */
    EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_128, /**< MACsec Cipher Suite mode GCM-AES-XPN-128 */
    EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256  /**< MACsec Cipher Suite mode GCM-AES-XPN-256 */
} RtkMkaCipherSuite_e;

/**
 * @brief Enumeration for MKA validate frames mode
 */
typedef enum
{
    EM_RTK_MKA_VALID_FRAMES_DISABLED = 0, /**< SecY Validate Frames mode: disabled, remove SecTAGs and ICVs from received frames    */
    EM_RTK_MKA_VALID_FRAMES_CHECKED,      /**< SecY Validate Frames mode: checked, enable validation, do not discard invalid frames */
    EM_RTK_MKA_VALID_FRAMES_STRICT,       /**< SecY Validate Frames mode: strict, enable validation and discard invalid frames      */
    EM_RTK_MKA_VALID_FRAMES_NULL          /**< SecY Validate Frames mode: null, no processing, do not remove SecTAGs of ICVs        */
} RtkMkaValidateFrames_e;

/**
 * @brief Enumeration for MKA external PHY type
 */
typedef enum
{
    EM_RTK_MKA_EXTERNAL_PHY_NONE    = 0, /**< Not External Phy                 */
    EM_RTK_MKA_EXTERNAL_PHY_RTL9010 = 1  /**< Supported External PHY: RTL9010B */
} RtkMkaExternaPhyType_e;

/**
 * @brief Enumeration for MKA confidentiality offset
 */
typedef enum
{
    EM_RTK_CONFIDENT_OFFSET_0 = 0,          /**< Confidentiality Offset 0 bytes        */
    EM_RTK_CONFIDENT_OFFSET_30,             /**< Confidentiality Offset 30 bytes       */
    EM_RTK_CONFIDENT_OFFSET_50,             /**< Confidentiality Offset 50 bytes       */
    EM_RTK_CONFIDENT_OFFSET_UNKNOWN = 0xFFu /**< Confidentiality Offset unknown bytest */
} RtkMkaConfidentOffset_e;

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/**
 * @brief Structure for MKA CA configuration
 */
typedef struct
{
    bool  enable;                   /**< Status of this CA entry (false to true)                                          */
    uint8 keyLen;                   /**< The key length of this CA (RTK_MKA_KEY_LEN_128_BITS to RTK_MKA_KEY_LEN_256_BITS) */
    uint8 reserved[2];              /**< Reserved for field alignment (0x0 to 0x0 for 2 elements)                         */
    uint8 ckn[RTK_MKA_MAX_CKN_LEN]; /**< secure Connectivity Association Key Name (0x0 to 0xFF for 32 elements)           */
} RtkMkaCaConfig_t;

/**
 * @brief Structure for MKA port configuration
 */
typedef struct
{
    bool                    enabled;                   /**< Enables or disables MACsec function of this port (false to true)                                                                      */
    bool                    desired;                   /**< Deprecated member, should be always true(false to true)                                                                               */
    bool                    protect;                   /**< SecY parameters: Enables or disables protection of transmitted frames (false to true)                                                 */
    bool                    replayProtect;             /**< SecY parameters: Enables or disables replay protection (false to true)                                                                */
    bool                    forceSci;                  /**< SecY parameters: Force to include SCI in the SecTag (false to true)                                                                   */
    bool                    useES;                     /**< SecY parameters: Enables use of the ES bit in the SecTAG (false to true)                                                              */
    bool                    useSCB;                    /**< SecY parameters: Enables use of the SCB bit in the SecTAG (false to true)                                                             */
    bool                    confidentialityProtection; /**< SecY parameters: Enables confidentiality protection (false to true)                                                                   */
    bool                    automotiveMka;             /**< Enables or disables the automotive MKA enhancement (false to true)                                                                    */
    bool                    isPointToPoint;            /**< Enables or disables the multiple peer support for this port (false to true)                                                           */
    uint8                   caIndex;                   /**< The correspond CA index of this port (RTK_MKA_MIN_CAK_IDX to RTK_MKA_MAX_CAK_IDX)                                                     */
    uint8                   priority;                  /**< The key server priority (0 to 255)                                                                                                    */
    uint8                   bypassVlanNumber;          /**< SecY parameters: The number of VLAN tags will be bypass (RTK_MKA_MIN_BYPASS_VLAN_NUMBER to RTK_MKA_MAX_BYPASS_VLAN_NUMBER)            */
    uint8                   externalPhyId;             /**< The SMI PHY ID of external PHY (0 to 31)                                                                                              */
    uint32                  replayProtectWindow;       /**< SecY parameters: The replay protection window size (0x0 to 0xFFFFFFFF)                                                                */
    uint64                  pnExhaustion;              /**< The PN Exhaustion value (RTK_MKA_MIN_PN_EXHAUSTION to RTK_MKA_MAX_PN_EXHAUSTION)                                                      */
    RtkMkaConfidentOffset_e confidentialityOffset;     /**< The configentiality protection offset options for the selected cipher suite (EM_RTK_CONFIDENT_OFFSET_0 to EM_RTK_CONFIDENT_OFFSET_50) */
    RtkMkaValidateFrames_e  validateFrames;            /**< SecY parameters: cntrol validation of received frames (EM_RTK_MKA_VALID_FRAMES_DISABLED to EM_RTK_MKA_VALID_FRAMES_NULL)              */
    RtkMkaCipherSuite_e     cipherSuite;               /**< The selected cipher suite of this port(EM_RTK_MKA_CIPHER_SUITE_NONE to EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256)                       */
    RtkMkaExternaPhyType_e  externalPhyType;           /**< The external PHY type (EM_RTK_MKA_EXTERNAL_PHY_NONE to EM_RTK_MKA_EXTERNAL_PHY_RTL9010)                                               */

    uint16 helloTime;   /**< The interval at which MKA participants exchange MKPDU messages to maintain and validate the network connectivity status, \n
                             the unit is ms. (0x0 to 0xFFFF)                                                                                                       */
    uint16 lifeTime;    /**< The duration for which a peer remains valid, the unit is ms. (0x0 to 0xFFFF)                                                          */
    uint16 retireTime;  /**< The period after which an expired key is considered no longer usable and must be completely removed, the unit is ms. (0x0 to 0xFFFF)  */
    uint16 reserved;    /**< Reserved for 4-byte alignment                                                                                                         */
    uint32 newSakTime;  /**< The duation of time-based rekey mechanism, the unit is ms. (0x0 to 0xFFFFFFFF)                                                        */
    uint32 caSetupTime; /**< The security mechanism to establish a MACsec channel. \n
                             If the MACsec channel cannot be established within this time period, \n
                             the switch will trigger an interrupt to notify external host. \n
                             The unit is ms. (0x0 to 0xFFFFFFFF)                                                                                                   */
} RtkMkaPortConfig_t;

/**
 * @brief Structure for MKA status
 */
typedef struct
{
    RtkEnable enable; /**< The global state of MKA status (DISABLED, ENABLED) */
} MkaStatus_t;

/**
 * @brief Structure for MKA force live peer
 */
typedef struct
{
    RtkEnable enable; /**< The global state of Realtek's force-peer-promotion enhancement status (DISABLED, ENABLED) */
} MkaForceLivePeer_t;

/**
 * @brief Structure for MKA version
 */
typedef struct
{
    RtkMkaVersion_e version; /**< MKA Protocol Version (EM_RTK_MKA_VERSION_1 to EM_RTK_MKA_VERSION_UNKNOWN) */
} MkaVersion_t;

/**
 * @brief Structure for MKA CAK source
 */
typedef struct
{
    RtkMkaCakSource_e source;    /**< The source type of CAK (EM_RTK_MKA_CAK_SOURCE_OTP to EM_RTK_MKA_CAK_SOURCE_FLASH) */
    uint8             unwrapIdx; /**< The unwrap AES key Index for flash type CAK (RTK_MKA_MIN_CAK_IDX to RTK_MKA_MAX_CAK_IDX) */
} MkaCakSource_t;

/**
 * @brief Structure for MKA CA configuration
 */
typedef struct
{
    uint8            idx;    /**< The source type of CAK (EM_RTK_MKA_CAK_SOURCE_OTP to EM_RTK_MKA_CAK_SOURCE_FLASH) */
    RtkMkaCaConfig_t config; /**< The configuration parameters of the specific CA */
} MkaCaConfig_t;

/**
 * @brief Structure for MKA port configuration
 */
typedef struct
{
    RtkPort            port;   /**< The specific user port identity (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkMkaPortConfig_t config; /**< The configuration parameters of the specific port */
} MkaPortConfig_t;

/******************************************************************************/
/* Function Declarations                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_mka_get_status(UnitChip_t unitChip, MkaStatus_t *mkaStatus);
EXTERN_API RtkApiRet rtk_mka_set_status(UnitChip_t unitChip, const MkaStatus_t *mkaStatus);
EXTERN_API RtkApiRet rtk_mka_get_force_live_peer(UnitChip_t unitChip, MkaForceLivePeer_t *mkaForceLivePeer);
EXTERN_API RtkApiRet rtk_mka_set_force_live_peer(UnitChip_t unitChip, const MkaForceLivePeer_t *mkaForceLivePeer);
EXTERN_API RtkApiRet rtk_mka_get_version(UnitChip_t unitChip, MkaVersion_t *mkaVersion);
EXTERN_API RtkApiRet rtk_mka_set_version(UnitChip_t unitChip, const MkaVersion_t *mkaVersion);
EXTERN_API RtkApiRet rtk_mka_get_cak_source(UnitChip_t unitChip, MkaCakSource_t *mkaCakSource);
EXTERN_API RtkApiRet rtk_mka_set_cak_source(UnitChip_t unitChip, const MkaCakSource_t *mkaCakSource);
EXTERN_API RtkApiRet rtk_mka_get_ca_config(UnitChip_t unitChip, MkaCaConfig_t *mkaCaConfig);
EXTERN_API RtkApiRet rtk_mka_set_ca_config(UnitChip_t unitChip, const MkaCaConfig_t *mkaCaConfig);
EXTERN_API RtkApiRet rtk_mka_get_port_config(UnitChip_t unitChip, MkaPortConfig_t *mkaPortConfig);
EXTERN_API RtkApiRet rtk_mka_set_port_config(UnitChip_t unitChip, const MkaPortConfig_t *mkaPortConfig);

#endif /* RTKAS_API_MKA_H__ */