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

#ifndef CVA_RTK_API_SYSTEM_H__
#define CVA_RTK_API_SYSTEM_H__

#include "rtkas_api_system.h"
#include "rtl907xc_va_api.h"

EXTERN_API RtkApiRet cva_sys_init(uint32 unit);
EXTERN_API RtkApiRet cva_sys_status_get(uint32 unit, RtkSysStatus_e *status);
EXTERN_API RtkApiRet cva_sys_otp_version_get(uint32 unit, uint8 *major, uint8 *minor);
EXTERN_API RtkApiRet cva_sys_fw_version_get(uint32 unit, FwVersionInfo_t *version);
EXTERN_API RtkApiRet cva_sys_chip_version_get(uint32 unit, uint32 *version);
EXTERN_API RtkApiRet cva_sys_chip_id_get(uint32 unit, uint32 *value);
EXTERN_API RtkApiRet cva_sys_indirect_channel_set(uint32 channel);

#endif /* CVA_RTK_API_SYSTEM_H__ */