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

#ifndef DVA_RTK_API_STORM_CTL_H__
#define DVA_RTK_API_STORM_CTL_H__

#include "rtl907xd_va_api.h"
#include "rtkas_api_storm_ctl.h"

#define DVA_STORM_MAX_RATE (0xFFFFFFu)

EXTERN_API RtkApiRet dva_storm_ctl_broadcast_set(uint32 unit, RtkPort port, RtkEnable enable);
EXTERN_API RtkApiRet dva_storm_ctl_broadcast_get(uint32 unit, RtkPort port, RtkEnable *pEnable);
EXTERN_API RtkApiRet dva_storm_ctl_broadcast_rate_set(uint32 unit, RtkPort port, RtkStormUnit_e stormUnit, uint32 rate);
EXTERN_API RtkApiRet dva_storm_ctl_broadcast_rate_get(uint32 unit, RtkPort port, RtkStormUnit_e *pStormUnit, uint32 *pRate);
EXTERN_API RtkApiRet dva_storm_ctl_multicast_set(uint32 unit, RtkPort port, RtkEnable enable, uint8 type);
EXTERN_API RtkApiRet dva_storm_ctl_multicast_get(uint32 unit, RtkPort port, RtkEnable *pEnable, uint8 *pType);
EXTERN_API RtkApiRet dva_storm_ctl_multicast_rate_set(uint32 unit, RtkPort port, RtkStormUnit_e stormUnit, uint32 rate);
EXTERN_API RtkApiRet dva_storm_ctl_multicast_rate_get(uint32 unit, RtkPort port, RtkStormUnit_e *pStormUnit, uint32 *pRate);
EXTERN_API RtkApiRet dva_storm_ctl_unicast_set(uint32 unit, RtkPort port, RtkEnable enable);
EXTERN_API RtkApiRet dva_storm_ctl_unicast_get(uint32 unit, RtkPort port, RtkEnable *pEnable);
EXTERN_API RtkApiRet dva_storm_ctl_unicast_rate_set(uint32 unit, RtkPort port, RtkStormUnit_e stormUnit, uint32 rate);
EXTERN_API RtkApiRet dva_storm_ctl_unicast_rate_get(uint32 unit, RtkPort port, RtkStormUnit_e *pStormUnit, uint32 *pRate);

#endif /* DVA_RTK_API_STORM_CTL_H__ */