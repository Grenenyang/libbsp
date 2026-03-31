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

#ifndef DV1_RTK_API_IGMP_H__
#define DV1_RTK_API_IGMP_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtl907xd_v1_api.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DV1_IGMP_CONTROL_0 DV1_INTERNAL_CPU_VARIABLE22
#define DV1_IGMP_CONTROL_1 DV1_INTERNAL_CPU_VARIABLE40

#define DV1_IGMP_CTRL_MASK_0 (0x20003E00u)
#define DV1_IGMP_CTRL_MASK_1 (0xFFFF0000u)
#define DV1_IGMP_CTRL_MASK_2 (0x0000FFFFu)

#define DV1_IGMP_GRP_MEM_AGE_TIME_LSB  (24u)
#define DV1_IGMP_GRP_MEM_AGE_TIME_LEN  (8u)
#define DV1_IGMP_LAST_MEM_AGE_TIME_LSB (16u)
#define DV1_IGMP_LAST_MEM_AGE_TIME_LEN (8u)
#define DV1_IGMP_ROUTER_PORT_MASK_LSB  (0u)
#define DV1_IGMP_ROUTER_PORT_MASK_LEN  (16u)
#define DV1_IGMP_IGMP_ABILITY_LSB      (13u)
#define DV1_IGMP_MLD_ABILITY_LSB       (12u)
#define DV1_IGMP_FASTLEAVE_ABILITY_LSB (11u)
#define DV1_IGMP_CROSSVLAN_ABILITY_LSB (10u)
#define DV1_IGMP_L2_DEF_DROP_LSB       (29u)
#define DV1_IGMP_L2_CUS_LU_MISS_LSB    (9u)

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dv1_igmpmld_grpmemagingtime_set(uint32 unit, uint8 grpMemAgingTime);
EXTERN_API RtkApiRet dv1_igmpmld_grpmemagingtime_get(uint32 unit, uint8 *pGrpMemAgingTime);
EXTERN_API RtkApiRet dv1_igmpmld_lastmemagingtime_set(uint32 unit, uint8 lastMemAgingTime);
EXTERN_API RtkApiRet dv1_igmpmld_lastmemagingtime_get(uint32 unit, uint8 *pLastMemAgingTime);
EXTERN_API RtkApiRet dv1_igmpmld_igmp_ability_set(uint32 unit, uint32 igmpAbility);
EXTERN_API RtkApiRet dv1_igmpmld_igmp_ability_get(uint32 unit, uint32 *pIgmpAbility);
EXTERN_API RtkApiRet dv1_igmpmld_mld_ability_set(uint32 unit, uint32 mldAbility);
EXTERN_API RtkApiRet dv1_igmpmld_mld_ability_get(uint32 unit, uint32 *pMldAbility);
EXTERN_API RtkApiRet dv1_igmpmld_fastleave_ability_set(uint32 unit, uint32 fastLeaveEn);
EXTERN_API RtkApiRet dv1_igmpmld_fastleave_ability_get(uint32 unit, uint32 *pFastLeaveEn);
EXTERN_API RtkApiRet dv1_igmpmld_crossvlan_ability_set(uint32 unit, uint32 crossVlanEn);
EXTERN_API RtkApiRet dv1_igmpmld_crossvlan_ability_get(uint32 unit, uint32 *pCrossVlanEn);
EXTERN_API RtkApiRet dv1_igmpmld_l2defaultdrop_set(uint32 unit, uint32 l2DefaultDrop);
EXTERN_API RtkApiRet dv1_igmpmld_l2defaultdrop_get(uint32 unit, uint32 *pl2DefaultDrop);
EXTERN_API RtkApiRet dv1_igmpmld_l2custmzlumiss_set(uint32 unit, uint32 l2Customize);
EXTERN_API RtkApiRet dv1_igmpmld_l2custmzlumiss_get(uint32 unit, uint32 *pl2Customize);

#endif /* DV1_RTK_API_IGMP_H__ */