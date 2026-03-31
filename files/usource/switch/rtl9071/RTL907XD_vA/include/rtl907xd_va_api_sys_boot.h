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

#ifndef DVA_RTK_API_SYS_BOOT_H__
#define DVA_RTK_API_SYS_BOOT_H__

#include "rtl907xd_va_api.h"

#define DVA_SYS_BOOT_UP_FLASHLESS         (0xB5u)
#define DVA_SYS_BOOT_UP_FLASHLESS_ENTERED (0xB6u)
#define DVA_SYS_BOOT_UP_FLASHLESS_ETH     (0xB7u)
#define DVA_SYS_BOOT_UP_FULL_INITIZED     (0xBFu)

#define DVA_FLASHLESS_EXIT_CMD       (0x99u)
#define DVA_FLASHLESS_EXIT_CFG_CMD   (0x96u)
#define DVA_FLASHLESS_BYPASS_ETH_CMD (0x88u)

#define DVA_EXINT_BOOT_INITZED_CONFIG_SUCCESS    (0u)
#define DVA_EXINT_BOOT_READY_FOR_CONFIG_REGIF    (1u)
#define DVA_EXINT_BOOT_READY_FOR_CONFIG_ETHERNET (2u)

#define DVA_BYPASS_ENABLE  (1u)
#define DVA_BYPASS_DISABLE (0u)

#define DVA_SYS_EVENT_PENDING_MASK_REG (DVA_INTERNAL_CPU_VARIABLE58)

#define DVA_FL_TIMEOUT_VALUE (0x10000u)

EXTERN_API RtkApiRet dva_sys_check_flashless_mode(uint32 unit, uint8 byPassEth);
EXTERN_API RtkApiRet dva_sys_leave_fw_to_cfg(uint32 unit);
EXTERN_API RtkApiRet dva_sys_leave_flashless(uint32 unit);
EXTERN_API RtkApiRet dva_sys_check_leave_flashless(uint32 unit);
EXTERN_API RtkApiRet dva_sys_flashless_eth_port_otp_disable(uint32 unit, RtkPort userPort);
EXTERN_API RtkApiRet dva_sys_update_firmware(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet dva_sys_update_config(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet dva_sys_eth_leave_fw_to_cfg(uint32 unit);
EXTERN_API RtkApiRet dva_sys_eth_cfg_update(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet dva_sys_eth_fw_update(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet dva_sys_eth_disable_non_configure_port_rx(uint32 unit, RtkPort userPort);

#endif /* DVA_RTK_API_SYS_BOOT_H__ */
