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

#ifndef CVB_RTK_API_SYS_BOOT_H__
#define CVB_RTK_API_SYS_BOOT_H__

#include "rtl907xc_vb_api.h"

#define CVB_SYS_BOOT_UP_FLASHLESS     (0xB3u)
#define CVB_SYS_BOOT_UP_FLASHLESS_ETH (0xB4u)
#define CVB_SYS_BOOT_UP_FULL_INITIZED (0xBFu)

#define CVB_FLASHLESS_BYPASS_10S_CMD (0x88u)
#define CVB_FLASHLESS_EXIT_CMD       (0x99u)

#define CVB_SYSMSG_INITZED_CONFIG_SUCCESS    (0x6Fu)
#define CVB_SYSMSG_READY_FOR_CONFIG_REGIF    (0x68u)
#define CVB_SYSMSG_READY_FOR_CONFIG_ETHERNET (0x67u)

#define CVB_BYPASS_ENABLE  (1u)
#define CVB_BYPASS_DISABLE (0u)

#define CVB_FL_TIMEOUT_VALUE (0x10000u)

#define CVB_ADDR_L2_MEMORY_BASE (0x00100000u)

EXTERN_API RtkApiRet cvb_sys_check_flashless_mode(uint32 unit, uint8 byPassEth);
EXTERN_API RtkApiRet cvb_sys_leave_flashless(uint32 unit);
EXTERN_API RtkApiRet cvb_sys_check_leave_flashless(uint32 unit);
EXTERN_API RtkApiRet cvb_sys_flashless_eth_port_otp_disable(uint32 unit, cvb_RtkPort userPort);
EXTERN_API RtkApiRet cvb_sys_update_firmware(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet cvb_sys_update_config(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet cvb_sys_eth_update_config(uint32 unit, cvb_RtkPort userPort, const int8 *fileName);
EXTERN_API RtkApiRet cvb_sys_eth_update_firmware(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet cvb_sys_eth_disable_non_configure_port_rx(uint32 unit, cvb_RtkPort userPort);

#endif /* CVB_RTK_API_SYS_BOOT_H__ */
