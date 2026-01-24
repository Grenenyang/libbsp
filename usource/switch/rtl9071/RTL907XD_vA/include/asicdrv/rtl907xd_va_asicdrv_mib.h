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

#ifndef DVA_ASICDRV_MIB_H_
#define DVA_ASICDRV_MIB_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api_mib.h"

#define DVA_PORTMIB_OFFSET (0x3Cu)
EXTERN_API RtkApiRet dva_drv_mib_counter_reset_system_debug(uint32 unit, uint32 gReset);
EXTERN_API RtkApiRet dva_drv_mib_counter_reset_port(uint32 unit, RtkPmap portMask);
EXTERN_API RtkApiRet dva_drv_mib_counter_reset_overflow(uint32 unit, uint32 gReset, RtkPmap portMask);
EXTERN_API RtkApiRet dva_drv_mib_counter_reset(uint32 unit, uint32 gReset, RtkPmap portMask);
EXTERN_API RtkApiRet dva_drv_mib_counter_get(uint32 unit, RtkMibCounterType_e counterType, RtkPort port, uint32 *counterValue);
EXTERN_API RtkApiRet dva_drv_mib_counter_enable(uint32 unit, RtkEnable enable);

#endif
