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

#ifndef RTKAS_API_QAV_H__
#define RTKAS_API_QAV_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_api_qos.h"

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint64 RtkQavBw; /**< Bandwidth of queue (0 to <CHIPID>_QAV_BW_MAX) */

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
/**
 * @brief This struct is used for cbs.
 * The port, que and en are inputs, and the bw can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSQue que;  /**< [in] Queue number (0 to QOS_QUE_MAX) */
    RtkEnable en;   /**< [in,out] Enable status (DISABLED to ENABLED) */
    RtkQavBw  bw;   /**< [in,out] Bandwidth (0x0 to <CHIPID>_QAV_BW_MAX) */
} QavCbs_t;

/**
 * @brief This struct is used for priority regeneration.
 * The port and the oriPri are inputs, and the en and the regenPri can each be either an input or an output.
 */
typedef struct
{
    RtkPort   port;     /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSPri oriPri;   /**< [in] Original priority (0 to QOS_PRI_MAX) */
    RtkEnable en;       /**< [in,out] Enable status (DISABLED to ENABLED) */
    RtkQoSPri regenPri; /**< [in,out] Priority regenerated (0 to QOS_PRI_MAX) */
} QavPriRegen_t;

/**
 * @brief The struct is used for the tag selection during regeneration.
 * The port is an input, and the enIpri and the enOpri can each be either an input or an output.
 */
typedef struct
{
    RtkPort   port;   /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable enIpri; /**< [in,out] Enable status of inner priority (DISABLED to ENABLED) */
    RtkEnable enOpri; /**< [in,out] Enable status of outter priority (DISABLED to ENABLED) */
} QavRegenTagSel_t;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_qav_cbs_set(UnitChip_t unitChip, const QavCbs_t *qavCbs);
EXTERN_API RtkApiRet rtk_qav_cbs_get(UnitChip_t unitChip, QavCbs_t *qavCbs);
EXTERN_API RtkApiRet rtk_qav_pri_regen_set(UnitChip_t unitChip, const QavPriRegen_t *qavPriRegen);
EXTERN_API RtkApiRet rtk_qav_pri_regen_get(UnitChip_t unitChip, QavPriRegen_t *qavPriRegen);
EXTERN_API RtkApiRet rtk_qav_regen_tag_sel_set(UnitChip_t unitChip, const QavRegenTagSel_t *qavRegenTagSel);
EXTERN_API RtkApiRet rtk_qav_regen_tag_sel_get(UnitChip_t unitChip, QavRegenTagSel_t *qavRegenTagSel);

#endif /* RTKAS_API_QAV_H__ */
