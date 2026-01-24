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

#ifndef DVA_ASICDRV_MKA_H_
#define DVA_ASICDRV_MKA_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_std.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api_mka.h"
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DVA_MKA_CAK_SOURCE_OTP   (0u)
#define DVA_MKA_CAK_SOURCE_FLASH (1u)

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
RtkApiRet dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(RtkPort port, RtkPort *portIdx);
RtkApiRet dva_drv_mka_enable_set(uint32 unit, bool enabled);
RtkApiRet dva_drv_mka_enable_get(uint32 unit, bool *enabled);
RtkApiRet dva_drv_mka_force_promotion_set(uint32 unit, bool force);
RtkApiRet dva_drv_mka_force_promotion_get(uint32 unit, bool *force);
RtkApiRet dva_drv_mka_version_set(uint32 unit, uint8 version);
RtkApiRet dva_drv_mka_version_get(uint32 unit, uint8 *version);
RtkApiRet dva_drv_mka_cak_type_set(uint32 unit, uint8 type);
RtkApiRet dva_drv_mka_cak_type_get(uint32 unit, uint8 *type);
RtkApiRet dva_drv_mka_cak_unwrap_idx_set(uint32 unit, uint8 unwrapKeyIdx);
RtkApiRet dva_drv_mka_cak_unwrap_idx_get(uint32 unit, uint8 *unwrapKeyIdx);
RtkApiRet dva_drv_mka_ca_info_set(uint32 unit, uint32 index, bool enable, uint8 keyLen, const uint8 *ckn);
RtkApiRet dva_drv_mka_ca_info_get(uint32 unit, uint32 index, bool *enable, uint8 *keyLen, uint8 *ckn);

RtkApiRet dva_drv_mka_get_port_config_flags(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig);
RtkApiRet dva_drv_mka_get_port_config(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig);
RtkApiRet dva_drv_mka_get_port_config_extra(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig);
RtkApiRet dva_drv_mka_get_port_timers(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig);

RtkApiRet dva_drv_mka_verify_port_config(const RtkMkaPortConfig_t *portConfig);
RtkApiRet dva_drv_mka_set_port_config_flags(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig);
RtkApiRet dva_drv_mka_set_port_config(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig);
RtkApiRet dva_drv_mka_set_port_config_extra(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig);
RtkApiRet dva_drv_mka_set_port_timers(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig);

#endif /* RTKAS_TEMPLATE_H_ */
