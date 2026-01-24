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

#ifndef DV1_RTK_API_MIB_H__
#define DV1_RTK_API_MIB_H__

#include "rtl907xd_v1_api.h"
#include "rtkas_api_mib.h"

EXTERN_API RtkApiRet dv1_stat_reset_all(uint32 unit);
EXTERN_API RtkApiRet dv1_stat_start_all(uint32 unit);
EXTERN_API RtkApiRet dv1_stat_stop_all(uint32 unit);
EXTERN_API RtkApiRet dv1_stat_get_ale_drop_reason(uint32 unit, uint32 *dropReason);
EXTERN_API RtkApiRet dv1_stat_port_max_queue_level_get(uint32 unit, RtkPort port, uint32 *maxQueueLevel);
EXTERN_API RtkApiRet dv1_stat_port_reset(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet dv1_stat_port_start(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet dv1_stat_port_stop(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet dv1_stat_port_get(uint32 unit, RtkPort port, RtkMibCounterType_e counterType, uint32 *counterValue);
EXTERN_API RtkApiRet dv1_stat_port_get_rx_drop_reason(uint32 unit, RtkPort port, uint32 *dropReason);

#endif /* DV1_RTK_API_MIB_H__ */
