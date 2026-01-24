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

#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_asicdrv_single_loop.h"

/**
 * @addtogroup RTK_MODULE_SINGLE_LOOP Single Loop
 * @{
 */

/**
 * @addtogroup SINGLE_LOOP_CONFIGURATION Single Loop Configuration
 * @{
 */

/**
 * @addtogroup SINGLE_LOOP_CONFIGURATION_ASIC Low Level Driver
 * @brief Single Loop Configuration Low Level Driver
 * @{
 */

/**
 * @brief Get the state of Single Loop.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] *pState Single Loop Global State (DISABLED, ENABLED) \n
 *             Indicates whether the global state of Single Loop is enabled or disabled. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @param[out] *pAutoRecovery Auto Recovery State (DISABLED, ENABLED) \n
 *             Indicates whether the auto recovery state is enabled or disabled. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @retval RT_ERR_OK Successfully got the state of Single Loop.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cvb_reg_field_read() for the error.
 * @note This API gets the state and auto recovery state of Single Loop.
 */
RtkApiRet cvb_sl_state_get(uint32 unit, RtkEnable *pState, RtkEnable *pAutoRecovery)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((NULL == pState) || (NULL == pAutoRecovery))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get Single Loop config register. */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_AUTO_RECOVERY_LSP, CVB_SL_CONFIG_AUTO_RECOVERY_LEN, pAutoRecovery));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_ENABLE_LSP, CVB_SL_CONFIG_ENABLE_LEN, pState));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the default control port state of Single Loop.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] *pControlPortA Control Port A Identification (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *             Indicates the control port A ID.
 * @param[out] *pControlPortAState Control Port A Default Port State (N/A) \n
 *             Indicates the control port A default port state.
 * @param[out] *pControlPortB Control port B Identification (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *             Indicates the control port B ID.
 * @param[out] *pControlPortBState Control Port B Default Port State (N/A) \n
 *             Indicates the control port B default port state.
 * @retval RT_ERR_OK Successfully got the default control port state of Single Loop.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cvb_reg_field_read() or cvb_drv_sl_default_port_state_get() for the error.
 * @note This API gets the default state of control port A and B.
 */
RtkApiRet cvb_sl_default_port_state_get(uint32 unit, RtkPort *pControlPortA, SingleLoopPortState_e *pControlPortAState, RtkPort *pControlPortB, SingleLoopPortState_e *pControlPortBState)
{
    RtkApiRet ret = RT_ERR_OK;
    uint32    val = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((NULL == pControlPortA) || (NULL == pControlPortAState) || (NULL == pControlPortB) || (NULL == pControlPortBState))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get Single Loop config register for control port A. */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_CONTROL_PORT_A_LSP, CVB_SL_CONFIG_CONTROL_PORT_A_LEN, &val));
        /* Transform the mapping between user port and system port. */
        *pControlPortA = (RtkPort)cvb_g_sysPort2userPort[val];
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_CONTROL_PORT_A_STATE_LSP, CVB_SL_CONFIG_CONTROL_PORT_A_STATE_LEN, &val));
        /* Transform the mapping for port state. */
        CHK_FUN_CONTINUE(ret, cvb_drv_sl_default_port_state_get(1u, pControlPortAState, &val));

        /* Get Single Loop config register for control port B. */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_CONTROL_PORT_B_LSP, CVB_SL_CONFIG_CONTROL_PORT_B_LEN, &val));
        /* Transform the mapping between user port and system port. */
        *pControlPortB = (RtkPort)cvb_g_sysPort2userPort[val];
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_CONTROL_PORT_B_STATE_LSP, CVB_SL_CONFIG_CONTROL_PORT_B_STATE_LEN, &val));
        /* Transform the mapping for port state. */
        CHK_FUN_CONTINUE(ret, cvb_drv_sl_default_port_state_get(1u, pControlPortBState, &val));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the operation control port state of Single Loop.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] controlPortAState Control Port A Port State (N/A) \n
 *             Indicates the control port A operation port state.
 * @param[out] controlPortBState Control Port B Port State (N/A) \n
 *             Indicates the control port B operation port state.
 * @retval RT_ERR_OK Successfully got the operation control port state of Single Loop.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cvb_reg_field_read() or cvb_drv_sl_port_state_get() for the error.
 * @note This API gets the operation state of control port A and B.
 */
RtkApiRet cvb_sl_port_state_get(uint32 unit, SingleLoopPortState_e *pControlPortAState, SingleLoopPortState_e *pControlPortBState)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((NULL == pControlPortAState) || (NULL == pControlPortBState))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get Single Loop config and port state register for control port A. */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_CONTROL_PORT_A_LSP, CVB_SL_CONFIG_CONTROL_PORT_A_LEN, &sysPort));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_PORT_STATE, CVB_SL_PORT_STATE_PORT0_STATE_LSP + (sysPort * (uint32)CVB_SL_PORT_STATE_PORT0_STATE_LEN), CVB_SL_PORT_STATE_PORT0_STATE_LEN, &val));
        /* Transform the mapping for port state. */
        CHK_FUN_CONTINUE(ret, cvb_drv_sl_port_state_get(val, pControlPortAState));

        /* Get Single Loop config and port state register for control port B. */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_CONFIG, CVB_SL_CONFIG_CONTROL_PORT_B_LSP, CVB_SL_CONFIG_CONTROL_PORT_B_LEN, &sysPort));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_SL_PORT_STATE, CVB_SL_PORT_STATE_PORT0_STATE_LSP + (sysPort * (uint32)CVB_SL_PORT_STATE_PORT0_STATE_LEN), CVB_SL_PORT_STATE_PORT0_STATE_LEN, &val));
        /* Transform the mapping for port state. */
        CHK_FUN_CONTINUE(ret, cvb_drv_sl_port_state_get(val, pControlPortBState));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Re-initialize Single Loop.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully re-initialized Single Loop.
 * @retval Others Refer to cvb_reg_field_write() for the error.
 * @note This API re-initializes Single Loop.
 */
RtkApiRet cvb_sl_reinit(uint32 unit)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Set Single Loop control register. */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_SL_CONTROL, CVB_SL_CONTROL_REINIT_LSP, CVB_SL_CONTROL_REINIT_LEN, 0x1u));
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* SINGLE_LOOP_CONFIGURATION_ASIC */
/**@}*/ /* SINGLE_LOOP_CONFIGURATION */
/**@}*/
