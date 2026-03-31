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

#ifndef RTK_API_SINGLE_LOOP_H__
#define RTK_API_SINGLE_LOOP_H__

typedef enum
{
    EM_SINGLE_LOOP_DISABLED = 0, /**< The port state is disabled. */
    EM_SINGLE_LOOP_BLOCKING,     /**< The port state is blocking. */
    EM_SINGLE_LOOP_INVALID,      /**< Invalid state. */
    EM_SINGLE_LOOP_FORWARDING,   /**< The port state is forwarding. */
    EM_SINGLE_LOOP_END
} SingleLoopPortState_e;

/**
 * @brief This struct is used for getting Single Loop state and auto recovery state.
 */
typedef struct
{
    RtkEnable state;        /**< [out] Indicate the global state of Single Loop is enabled or disabled. (DISABLED, ENABLED)*/
    RtkEnable autoRecovery; /**< [out] Indicate the auto recovery state is enabled or disabled. (DISABLED, ENABLED)*/
} SlStateConfig_t;

/**
 * @brief This struct is used for getting Single Loop default control port state.
 */
typedef struct
{
    RtkPort               controlPortA;      /**< [out] Indicate the control port A ID. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    SingleLoopPortState_e controlPortAState; /**< [out] Indicate the control port A default port state. (N/A)*/
    RtkPort               controlPortB;      /**< [out] Indicate the control port B ID. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    SingleLoopPortState_e controlPortBState; /**< [out] Indicate the control port A default port state. (N/A)*/
} SlDefaultPortStateConfig_t;

/**
 * @brief This struct is used for getting Single Loop current control port status.
 */
typedef struct
{
    SingleLoopPortState_e controlPortAState; /**< [out] Indicate the control port A operation port state. (N/A)*/
    SingleLoopPortState_e controlPortBState; /**< [out] Indicate the control port B operation port state. (N/A)*/
} SlPortStateConfig_t;

EXTERN_API RtkApiRet rtk_sl_state_get(UnitChip_t unitChip, SlStateConfig_t *slStateGet);
EXTERN_API RtkApiRet rtk_sl_default_port_state_get(UnitChip_t unitChip, SlDefaultPortStateConfig_t *slDefaultPortStateGet);
EXTERN_API RtkApiRet rtk_sl_port_state_get(UnitChip_t unitChip, SlPortStateConfig_t *slPortStateGet);
EXTERN_API RtkApiRet rtk_sl_reinit(UnitChip_t unitChip);

#endif /*RTK_API_SINGLE_LOOP_H__ */
