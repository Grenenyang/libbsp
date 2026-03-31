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

#include "rtkas_common_types.h"
#include "rtkas_common.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_single_loop.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_single_loop.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_single_loop.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_single_loop.h"
#endif

#include "rtkas_api_single_loop.h"

/**
 * @defgroup RTK_MODULE_SINGLE_LOOP Single Loop
 * @brief Single Loop APIs. Single Loop is a Realtek proprietary feature used to prevent network loops by creating a loop-free ring topology in Ethernet networks.
 * @{
 */
/**
 * @defgroup SINGLE_LOOP_CONFIGURATION Single Loop Configuration
 * @brief Get Single Loop configuration for setting and state.
 * @{
 */
/**
 * @brief Get the state of Single Loop.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] slStateGet Single Loop state get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_single_loop.h files for detailed content.
 * @retval RT_ERR_OK Get the state of Single Loop successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sl_state_get(), dva_sl_state_get(), cvb_sl_state_get(), cva_sl_state_get().
 */
RtkApiRet rtk_sl_state_get(UnitChip_t unitChip, SlStateConfig_t *slStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == slStateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sl_state_get(unitChip.unit, &slStateGet->state, &slStateGet->autoRecovery);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sl_state_get(unitChip.unit, &slStateGet->state, &slStateGet->autoRecovery);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sl_state_get(unitChip.unit, &slStateGet->state, &slStateGet->autoRecovery);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sl_state_get(unitChip.unit, &slStateGet->state, &slStateGet->autoRecovery);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get the default control port state of Single Loop.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] slDefaultPortStateGet Single Loop default port state get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_single_loop.h files for detailed content.
 * @retval RT_ERR_OK Get the default control port state of Single Loop successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sl_default_port_state_get(), dva_sl_default_port_state_get(), cvb_sl_default_port_state_get(), cva_sl_default_port_state_get().
 */
RtkApiRet rtk_sl_default_port_state_get(UnitChip_t unitChip, SlDefaultPortStateConfig_t *slDefaultPortStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == slDefaultPortStateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sl_default_port_state_get(unitChip.unit, &slDefaultPortStateGet->controlPortA, &slDefaultPortStateGet->controlPortAState, &slDefaultPortStateGet->controlPortB, &slDefaultPortStateGet->controlPortBState);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sl_default_port_state_get(unitChip.unit, &slDefaultPortStateGet->controlPortA, &slDefaultPortStateGet->controlPortAState, &slDefaultPortStateGet->controlPortB, &slDefaultPortStateGet->controlPortBState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sl_default_port_state_get(unitChip.unit, &slDefaultPortStateGet->controlPortA, &slDefaultPortStateGet->controlPortAState, &slDefaultPortStateGet->controlPortB, &slDefaultPortStateGet->controlPortBState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sl_default_port_state_get(unitChip.unit, &slDefaultPortStateGet->controlPortA, &slDefaultPortStateGet->controlPortAState, &slDefaultPortStateGet->controlPortB, &slDefaultPortStateGet->controlPortBState);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get the operation control port state of Single Loop.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] slPortStateGet Single Loop port state get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_single_loop.h files for detailed content.
 * @retval RT_ERR_OK Get the operation control port state of Single Loop successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sl_port_state_get(), dva_sl_port_state_get(), cvb_sl_port_state_get(), cva_sl_port_state_get().
 */
RtkApiRet rtk_sl_port_state_get(UnitChip_t unitChip, SlPortStateConfig_t *slPortStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == slPortStateGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sl_port_state_get(unitChip.unit, &slPortStateGet->controlPortAState, &slPortStateGet->controlPortBState);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sl_port_state_get(unitChip.unit, &slPortStateGet->controlPortAState, &slPortStateGet->controlPortBState);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sl_port_state_get(unitChip.unit, &slPortStateGet->controlPortAState, &slPortStateGet->controlPortBState);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sl_port_state_get(unitChip.unit, &slPortStateGet->controlPortAState, &slPortStateGet->controlPortBState);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Re-initialize Single Loop.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Re-initialize Single Loop successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_sl_reinit(), dva_sl_reinit(), cvb_sl_reinit(), cva_sl_reinit().
 */
RtkApiRet rtk_sl_reinit(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sl_reinit(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sl_reinit(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sl_reinit(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sl_reinit(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }

    return ret;
}

/**@}*/ /* SINGLE_LOOP_CONFIGURATION */

/**@}*/
