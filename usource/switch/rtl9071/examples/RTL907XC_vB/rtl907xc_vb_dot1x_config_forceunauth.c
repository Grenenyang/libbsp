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

#include "rtkas_api_ext.h"
#include "rtkas_api_dot1x.h"

/**
 * @addtogroup DOT1X_EXAMPLE DOT1X Example
 * @{
 */
/**
 * @addtogroup RTL907XCVB_DOT1X_EXAMPLE RTL907XC_VB DOT1X Example
 * @{
 */

/**
 * @defgroup RTL907XCVB_DOT1X_CONFIG_FORCE_UNAUTH_EXAMPLE Dot1x Config Force Unauth Example
 *
 * @brief <b>Configure the force unauth function.</b> \n
 *           After this configuration, even if the host receives a successful authentication message, \n
 *           the switch port will not be authorized and cannot forward normal packets.
 *
 * @section Purpose
 *           This example demonstrates how to keep the switch in an unauthorized state.
 * @section Description
 *           Even if the authentication is successful, the switch will always remain in the authorized state.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_dot1x_config_forceunauth(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_dot1x_config_forceunauth(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
    /* Config information */
    Dot1xForceStatusInfo_t dot1xForceStatusInfo = {
        .status = ENABLED,
    };
    /* Step 1: Enable force unauth function to keep the switch in an unauthorized state */
    CHK_FUN_RET(retVal, rtk_dot1x_set_force_unauth_enable(unitChip, &dot1xForceStatusInfo));
    /* Step 2: Trigger the dynamic setting flag to set the new configuration to the switch */
    CHK_FUN_RET(retVal, rtk_dot1x_set_dynamic_reset_flag(unitChip));
    /* Expected Behavior: The switch will always remain in the authorized state */
    return retVal;
}
/**@}*/ /* RTL907XCVB_DOT1X_CONFIG_FORCE_UNAUTH_EXAMPLE */
/**@}*/ /* RTL907XCVB_DOT1X_EXAMPLE */
/**@}*/ /* DOT1X_EXAMPLE */
