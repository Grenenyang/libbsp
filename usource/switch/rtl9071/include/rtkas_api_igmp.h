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

#ifndef RTKAS_API_IGMP_H__
#define RTKAS_API_IGMP_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define IGMP_CONTROL_0 (0x4A1B0008u)
#define IGMP_CONTROL_1 (0x4A1B0050u)

#define IGMP_CTRL_MASK_0 (0x20003E00u)
#define IGMP_CTRL_MASK_1 (0xFFFF0000u)
#define IGMP_CTRL_MASK_2 (0x0000FFFFu)

#define IGMP_GRP_MEM_AGE_TIME_LSB  (24u)
#define IGMP_GRP_MEM_AGE_TIME_LEN  (8u)
#define IGMP_LAST_MEM_AGE_TIME_LSB (16u)
#define IGMP_LAST_MEM_AGE_TIME_LEN (8u)
#define IGMP_ROUTER_PORT_MASK_LSB  (0u)
#define IGMP_ROUTER_PORT_MASK_LEN  (16u)
#define IGMP_IGMP_ABILITY_LSB      (13u)
#define IGMP_MLD_ABILITY_LSB       (12u)
#define IGMP_FASTLEAVE_ABILITY_LSB (11u)
#define IGMP_CROSSVLAN_ABILITY_LSB (10u)
#define IGMP_L2_DEF_DROP_LSB       (29u)
#define IGMP_L2_CUS_LU_MISS_LSB    (9u)

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/**
 * @brief IGMP information structure
 *
 * This structure is used to set or get IGMP ability.
 */
typedef struct
{
    uint32 enable; /**< [in,out] The parameter used to configure or obtain IGMP ability; 0:disable, 1:enable (0 to 1) */
} IgmpInfo_t;

/**
 * @brief MLD information structure
 *
 * This structure is used to set or get MLD ability.
 */
typedef struct
{
    uint32 enable; /**< [in,out] The parameter used to configure or obtain MLD ability; 0:disable, 1:enable (0 to 1) */
} MldInfo_t;

/**
 * @brief IGMP/MLD fastleave information structure
 *
 * This structure is used to set or get IGMP/MLD fastleave ability.
 */
typedef struct
{
    uint32 enable; /**< [in,out] This is used to configure IGMP/MLD fastleave ability; 0:disable, 1:enable (0 to 1) */
} FastLeaveInfo_t;

/**
 * @brief IGMP/MLD crossvlan information structure
 *
 * This structure is used to set or get IGMP/MLD crossvlan ability.
 */
typedef struct
{
    uint32 enable; /**< [in,out] This is used to configure IGMP/MLD crossvlan ability; 0:disable, 1:enable (0 to 1) */
} CrossVlanInfo_t;

/**
 * @brief IGMP/MLD information structure of group member aging time
 *
 * This structure is used to set or get IGMP/MLD group member aging time.
 */
typedef struct
{
    uint8 grpTime; /**< [in,out] Group member aging time in seconds (0 to 0xFF) */
} GroupAgeTimeInfo_t;

/**
 * @brief IGMP/MLD information structure of last member aging time
 *
 * This structure is used to set or get IGMP/MLD last member aging time.
 */
typedef struct
{
    uint8 lstTime; /**< [in,out] Last member aging time in seconds (0 to 0xFF) */
} LastAgeTimeInfo_t;

/**
 * @brief IGMP/MLD information structure of L2 default drop ability
 *
 * This structure is used to set or get IGMP/MLD L2 default drop ability.
 */
typedef struct
{
    uint32 drop; /**< [in,out] L2 default drop ability; 0:disable, 1:enable (0 to 1) */
} L2D4DropInfo_t;

/**
 * @brief IGMP/MLD information structure of L2 customization ability
 *
 * This structure is used to set or get IGMP/MLD L2 customization ability.
 */
typedef struct
{
    uint32 enable; /**< [in,out] L2 customization ability; 0:disable, 1:enable (0 to 1) */
} L2CustomizeInfo_t;

/**
 * @brief IGMP/MLD information structure of the example configuration
 *
 * This structure is used to get IGMP/MLD configuration.
 */
typedef struct
{
    uint32 igmpEn;           /**< [out] IGMP ability; 0:disable, 1:enable (0u to 1u) */
    uint32 mldEn;            /**< [out] MLD ability; 0:disable, 1:enable (0u to 1u) */
    uint32 crosVlanEn;       /**< [out] Cross VLAN ability; 0:disable, 1:enable (0u to 1u) */
    uint32 fastLeavEn;       /**< [out] fastleave ability; 0:disable, 1:enable (0u to 1u) */
    uint32 l2DropEn;         /**< [out] L2 drop ability; 0:disable, 1:enable (0u to 1u) */
    uint8  lastMemAgingTime; /**< [out] last member aging time in seconds (0u to 0xFFu) */
    uint8  grpMemAgingTime;  /**< [out] group member aging time in seconds (0u to 0xFFu) */
} RtkIgmpConfig_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_igmpmld_grpmemagingtime_set(UnitChip_t unitChip, GroupAgeTimeInfo_t *grpAgeTimeInfo);
EXTERN_API RtkApiRet rtk_igmpmld_grpmemagingtime_get(UnitChip_t unitChip, GroupAgeTimeInfo_t *grpAgeTimeInfo);
EXTERN_API RtkApiRet rtk_igmpmld_lastmemagingtime_set(UnitChip_t unitChip, LastAgeTimeInfo_t *lastAgeTimeInfo);
EXTERN_API RtkApiRet rtk_igmpmld_lastmemagingtime_get(UnitChip_t unitChip, LastAgeTimeInfo_t *lastAgeTimeInfo);
EXTERN_API RtkApiRet rtk_igmpmld_igmp_ability_set(UnitChip_t unitChip, IgmpInfo_t *igmpInfo);
EXTERN_API RtkApiRet rtk_igmpmld_igmp_ability_get(UnitChip_t unitChip, IgmpInfo_t *igmpInfo);
EXTERN_API RtkApiRet rtk_igmpmld_mld_ability_set(UnitChip_t unitChip, MldInfo_t *mldInfo);
EXTERN_API RtkApiRet rtk_igmpmld_mld_ability_get(UnitChip_t unitChip, MldInfo_t *mldInfo);
EXTERN_API RtkApiRet rtk_igmpmld_fastleave_ability_set(UnitChip_t unitChip, FastLeaveInfo_t *fastLeaveInfo);
EXTERN_API RtkApiRet rtk_igmpmld_fastleave_ability_get(UnitChip_t unitChip, FastLeaveInfo_t *fastLeaveInfo);
EXTERN_API RtkApiRet rtk_igmpmld_crossvlan_ability_set(UnitChip_t unitChip, CrossVlanInfo_t *crossVlanInfo);
EXTERN_API RtkApiRet rtk_igmpmld_crossvlan_ability_get(UnitChip_t unitChip, CrossVlanInfo_t *crossVlanInfo);
EXTERN_API RtkApiRet rtk_igmpmld_l2defaultdrop_set(UnitChip_t unitChip, L2D4DropInfo_t *l2D4DropInfo);
EXTERN_API RtkApiRet rtk_igmpmld_l2defaultdrop_get(UnitChip_t unitChip, L2D4DropInfo_t *l2D4DropInfo);
EXTERN_API RtkApiRet rtk_igmpmld_l2custmzlumiss_set(UnitChip_t unitChip, L2CustomizeInfo_t *l2CustomizeInfo);
EXTERN_API RtkApiRet rtk_igmpmld_l2custmzlumiss_get(UnitChip_t unitChip, L2CustomizeInfo_t *l2CustomizeInfo);

#endif /* RTKAS_API_IGMP_H__ */