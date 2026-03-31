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

#ifndef DVA_RTK_API_MKA_H__
#define DVA_RTK_API_MKA_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_std.h"
#include "rtkas_common_types.h"
#include "rtkas_api_mka.h"
#include "rtl907xd_va_api.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

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
EXTERN_API RtkApiRet dva_mka_get_status(uint32 unit, RtkEnable *adminStatus);
EXTERN_API RtkApiRet dva_mka_set_status(uint32 unit, RtkEnable adminStatus);
EXTERN_API RtkApiRet dva_mka_get_force_live_peer(uint32 unit, RtkEnable *force);
EXTERN_API RtkApiRet dva_mka_set_force_live_peer(uint32 unit, RtkEnable force);
EXTERN_API RtkApiRet dva_mka_get_version(uint32 unit, RtkMkaVersion_e *version);
EXTERN_API RtkApiRet dva_mka_set_version(uint32 unit, RtkMkaVersion_e version);
EXTERN_API RtkApiRet dva_mka_get_cak_source(uint32 unit, RtkMkaCakSource_e *cakSource, uint8 *unwrapIdx);
EXTERN_API RtkApiRet dva_mka_set_cak_source(uint32 unit, RtkMkaCakSource_e cakSource, uint8 unwrapIdx);
EXTERN_API RtkApiRet dva_mka_get_ca_config(uint32 unit, uint8 caIndex, RtkMkaCaConfig_t *caConfig);
EXTERN_API RtkApiRet dva_mka_set_ca_config(uint32 unit, uint8 caIndex, const RtkMkaCaConfig_t *caConfig);

EXTERN_API RtkApiRet dva_mka_get_port_config(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig);
EXTERN_API RtkApiRet dva_mka_set_port_config(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig);

#endif /* DVA_RTK_API_MKA_H__ */
