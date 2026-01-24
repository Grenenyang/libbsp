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

#ifndef RTK_API_LINK_AGGREGATION_H__
#define RTK_API_LINK_AGGREGATION_H__

#define RTK_LA_MIN_NUM_OF_GROUP (1u)
#define RTK_LA_MAX_NUM_OF_GROUP (8u)

/* link aggregation balance bit */
#define RTK_LAG_BALANCE_DISABLE   (0x00000000u)
#define RTK_LAG_BALANCE_ETHERTYPE (0x00000001u)
#define RTK_LAG_BALANCE_PROTOCOL  (0x00000002u)
#define RTK_LAG_BALANCE_L4DPORT   (0x00000004u)
#define RTK_LAG_BALANCE_ALL       (0x00000008u)

/* link aggregation hash bit */
#define RTK_LAG_HASH_INPORT (0x01u)
#define RTK_LAG_HASH_DMAC   (0x02u)
#define RTK_LAG_HASH_SMAC   (0x04u)
#define RTK_LAG_HASH_DIP    (0x08u)
#define RTK_LAG_HASH_SIP    (0x10u)
#define RTK_LAG_HASH_DPORT  (0x20u)
#define RTK_LAG_HASH_SPORT  (0x40u)
#define RTK_LAG_HASH_ALL    (0x7Fu)

/* link aggregation hash shift lsp */
#define RTK_LAG_HASH_SHIFT_OFFSET_INPORT (0u)
#define RTK_LAG_HASH_SHIFT_OFFSET_DMAC   (2u)
#define RTK_LAG_HASH_SHIFT_OFFSET_SMAC   (4u)
#define RTK_LAG_HASH_SHIFT_OFFSET_DIP    (6u)
#define RTK_LAG_HASH_SHIFT_OFFSET_SIP    (8u)
#define RTK_LAG_HASH_SHIFT_OFFSET_DPORT  (10u)
#define RTK_LAG_HASH_SHIFT_OFFSET_SPORT  (12u)
#define RTK_LAG_HASH_SHIFT_OFFSET_ALL    (0x3FFFu)

/**
 * @brief This struct is used for link aggregation member configuration.
 */
typedef struct
{
    uint32  group;    /**< [in] Indicate group ID for setting or getting member ports. (1 to 8) */
    RtkPmap portMask; /**< [in, out] Indicate member ports in group. (0 to <CHIPID>_RTK_USER_PMAP_ALL) */
} LagMemberConfig_t;

/**
 * @brief This struct is used for link aggregation hash mode configuration.
 */
typedef struct
{
    uint32 group;     /**< [in] Indicate group ID for setting or getting hash mask and hash shift. (1 to 8) */
    uint8  hashMask;  /**< [in, out] Indicate hash mask for calculating hash value. Hash mask can contain multiple hash keys. (RTK_LAG_HASH_ALL) */
    uint16 hashShift; /**< [in, out] Indicate hash shift for hash key shifting. (RTK_LAG_HASH_SHIFT_OFFSET_ALL) */
} LagHashConfig_t;

/**
 * @brief This struct is used for link aggregation balance mode configuration.
 */
typedef struct
{
    uint32 group;       /**< [in] Indicate group ID for setting or getting balance mode. (1 to 8) */
    uint32 balanceMode; /**< [in, out] Indicate balance Mode. (RTK_LAG_BALANCE_DISABLE, RTK_LAG_BALANCE_ETHERTYPE, RTK_LAG_BALANCE_PROTOCOL, RTK_LAG_BALANCE_L4DPORT, DV1_RTK_LAG_BALANCE_ALL) */
} LagBalanceConfig_t;

/**
 * @brief This struct is used for link aggregation ether type balance mode configuration.
 */
typedef struct
{
    uint32 group;     /**< [in] Indicate group ID for setting or getting ether type value. (1 to 8) */
    uint16 etherType; /**< [in, out] Indicate ether type value. (0 to 65535) */
} LagBalanceEthertypeConfig_t;

/**
 * @brief This struct is used for link aggregation protocol balance mode configuration.
 */
typedef struct
{
    uint32 group;    /**< [in] Indicate group ID for setting or getting protocol value. (1 to 8) */
    uint8  protocol; /**< [in, out] Indicate protocol value. (0 to 255) */
} LagBalanceProtocolConfig_t;

/**
 * @brief This struct is used for link aggregation L4 destination port balance mode configuration.
 */
typedef struct
{
    uint32 group;       /**< [in] Indicate group ID for setting or getting L4 destination port value and mask. (1 to 8) */
    uint16 l4Dport;     /**< [in, out] Indicate L4 destination port value. (0 to 65535) */
    uint16 l4DportMask; /**< [in, out] Indicate L4 destination port mask value. (0 to 65535) */
} LagBalanceL4DportConfig_t;

EXTERN_API RtkApiRet rtk_lag_member_set(UnitChip_t unitChip, LagMemberConfig_t *lagMemberSet);
EXTERN_API RtkApiRet rtk_lag_member_get(UnitChip_t unitChip, LagMemberConfig_t *lagMemberGet);
EXTERN_API RtkApiRet rtk_lag_hash_set(UnitChip_t unitChip, LagHashConfig_t *lagHashSet);
EXTERN_API RtkApiRet rtk_lag_hash_get(UnitChip_t unitChip, LagHashConfig_t *lagHashGet);
EXTERN_API RtkApiRet rtk_lag_balance_set(UnitChip_t unitChip, LagBalanceConfig_t *lagBalanceSet);
EXTERN_API RtkApiRet rtk_lag_balance_get(UnitChip_t unitChip, LagBalanceConfig_t *lagBalanceGet);
EXTERN_API RtkApiRet rtk_lag_balance_ethertype_set(UnitChip_t unitChip, LagBalanceEthertypeConfig_t *lagBalanceEthertypeSet);
EXTERN_API RtkApiRet rtk_lag_balance_ethertype_get(UnitChip_t unitChip, LagBalanceEthertypeConfig_t *lagBalanceEthertypeGet);
EXTERN_API RtkApiRet rtk_lag_balance_protocol_set(UnitChip_t unitChip, LagBalanceProtocolConfig_t *lagBalanceProtocolSet);
EXTERN_API RtkApiRet rtk_lag_balance_protocol_get(UnitChip_t unitChip, LagBalanceProtocolConfig_t *lagBalanceProtocolGet);
EXTERN_API RtkApiRet rtk_lag_balance_l4dport_set(UnitChip_t unitChip, LagBalanceL4DportConfig_t *lagBalanceL4DportSet);
EXTERN_API RtkApiRet rtk_lag_balance_l4dport_get(UnitChip_t unitChip, LagBalanceL4DportConfig_t *lagBalanceL4DportGet);
#endif /*RTK_API_LINK_AGGREGATION_H__ */
