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

#ifndef RTKAS_API_PORT_ISOLATION_H__
#define RTKAS_API_PORT_ISOLATION_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/**
 * @brief Port isolation information structure
 *
 * This structure is used to access port isolation.
 */
typedef struct
{
    RtkPort port;     /**< [in] The indexed port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkPmap portmask; /**< [in,out] The isolated port mask (0 to <CHIPID>_RTK_USER_PMAP_ALL) */
} PortIsoInfo_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_port_isolation_set(UnitChip_t unitChip, PortIsoInfo_t *portIsoInfo);
EXTERN_API RtkApiRet rtk_port_isolation_get(UnitChip_t unitChip, PortIsoInfo_t *portIsoInfo);

#endif /* RTKAS_API_PORT_ISOLATION_H__ */