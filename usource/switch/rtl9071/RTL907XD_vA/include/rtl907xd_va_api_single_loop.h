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

#ifndef DVA_RTK_API_SINGLE_LOOP_H__
#define DVA_RTK_API_SINGLE_LOOP_H__

#include "rtl907xd_va_api.h"
#include "rtkas_api_single_loop.h"

EXTERN_API RtkApiRet dva_sl_state_get(uint32 unit, RtkEnable *pState, RtkEnable *pAutoRecovery);
EXTERN_API RtkApiRet dva_sl_default_port_state_get(uint32 unit, RtkPort *pControlPortA, SingleLoopPortState_e *pControlPortAState, RtkPort *pControlPortB, SingleLoopPortState_e *pControlPortBState);
EXTERN_API RtkApiRet dva_sl_port_state_get(uint32 unit, SingleLoopPortState_e *pControlPortAState, SingleLoopPortState_e *pControlPortBState);
EXTERN_API RtkApiRet dva_sl_reinit(uint32 unit);

#endif /*DVA_RTK_API_SINGLE_LOOP_H__ */
