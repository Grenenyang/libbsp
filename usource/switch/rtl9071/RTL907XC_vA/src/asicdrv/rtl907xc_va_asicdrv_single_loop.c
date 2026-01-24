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

#include "rtl907xc_va_asicdrv_single_loop.h"

#ifdef RTK_MODULE_SINGLE_LOOP
/**
 * @brief Check default port state.
 * @param[in] portAState Port A State (N/A) \n
 *            Indicate the port A state for checking if the parameter is valid.
 * @param[in] portBState Port B State (N/A) \n
 *            Indicate the port B state for checking if the parameter is valid.
 * @param[out] None
 * @retval RT_ERR_OK The input port state is valid.
 * @retval -RT_ERR_OUT_OF_RANGE The input port state is invalid.
 */
RtkApiRet cva_drv_sl_default_port_state_check(SingleLoopPortState_e portAState, SingleLoopPortState_e portBState)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((EM_SINGLE_LOOP_BLOCKING != portAState) && (EM_SINGLE_LOOP_FORWARDING != portAState))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if((EM_SINGLE_LOOP_BLOCKING != portBState) && (EM_SINGLE_LOOP_FORWARDING != portBState))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get default port state translation.
 * @param[in] mode Translation Mode (0 to 1) \n
 *            Indicate the mode for port state translation. \n
 *            0: Translate port state from enum to uint32 based on sl bit definition.\n
 *            1: Translate port state from uint32 based on sl bit definition to enum.
 * @param[out] pPortStateE Port State (N/A) \n
 *             Indicate the port state.
 * @param[out] pPortStateU Port State (0 to 1) \n
 *             Indicate the port state in sl bit definition. \n
 *             0: Default state bit for blocking \n
 *             1: Default state bit for forwarding
 * @retval RT_ERR_OK Get default port state translation successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The input port state based on mode is invalid for the translation.
 */
RtkApiRet cva_drv_sl_default_port_state_get(uint32 mode, SingleLoopPortState_e *pPortStateE, uint32 *pPortStateU)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(0u == mode)
        {
            if(EM_SINGLE_LOOP_BLOCKING == *pPortStateE)
            {
                *pPortStateU = 0u;
            }
            else if(EM_SINGLE_LOOP_FORWARDING == *pPortStateE)
            {
                *pPortStateU = 1u;
            }
            else
            {
                ret = -RT_ERR_OUT_OF_RANGE;
            }
        }
        else if(1u == mode)
        {
            if(0u == *pPortStateU)
            {
                *pPortStateE = EM_SINGLE_LOOP_BLOCKING;
            }
            else if(1u == *pPortStateU)
            {
                *pPortStateE = EM_SINGLE_LOOP_FORWARDING;
            }
            else
            {
                *pPortStateE = EM_SINGLE_LOOP_END;
                ret          = -RT_ERR_OUT_OF_RANGE;
            }
        }
        else
        {
            ret = -RT_ERR_OUT_OF_RANGE;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get port state translation.
 * @param[in] portState Port State (0 to 3) \n
 *            Indicate the operation port state from register bits.
 * @param[out] pPortState Port State (N/A) \n
 *             Indicate the operation port state.
 * @retval RT_ERR_OK Get port state translation successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The input port state is invalid for the translation.
 */
RtkApiRet cva_drv_sl_port_state_get(uint32 portState, SingleLoopPortState_e *pPortState)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((uint32)EM_SINGLE_LOOP_DISABLED == portState)
        {
            *pPortState = EM_SINGLE_LOOP_DISABLED;
        }
        else if((uint32)EM_SINGLE_LOOP_BLOCKING == portState)
        {
            *pPortState = EM_SINGLE_LOOP_BLOCKING;
        }
        else if((uint32)EM_SINGLE_LOOP_FORWARDING == portState)
        {
            *pPortState = EM_SINGLE_LOOP_FORWARDING;
        }
        else
        {
            *pPortState = EM_SINGLE_LOOP_END;
            ret         = -RT_ERR_OUT_OF_RANGE;
        }
    } while(0u == 1u);

    return ret;
}
#endif
