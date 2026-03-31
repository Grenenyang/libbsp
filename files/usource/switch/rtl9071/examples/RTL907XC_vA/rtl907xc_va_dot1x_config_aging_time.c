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
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @defgroup DOT1X_EXAMPLE DOT1X Example
 * @{
 */
/**
 * @defgroup RTL907XCVA_DOT1X_EXAMPLE RTL907XC_VA DOT1X Example
 * @{
 */

/**
 * @defgroup RTL907XCVA_DOT1X_CONFIG_AGING_TIME_EXAMPLE Dot1x Config Aging Time Example
 *
 * @brief <b>Configure aging function</b> \n
 *           The authorized port will become unauthorized after timer overflows, \n
 *           and normal packets(i.e., Packets other than RSTP and DOT1X) will no longer be forwarded by this port.
 *
 * @section Purpose
 *          This example demonstrates how to remove the authentication status of port-based DOT1X after the set aging time.
 * @section Description
 *          The switch will become unauthorized after 100 seconds of successful authentication.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_dot1x_config_aging_time(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_dot1x_config_aging_time(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VA};
    /* Config information */
    Dot1xAgeStatusInfo_t dot1xAgeStatusInfo = {.status = ENABLED};
    Dot1xAgeOutValInfo_t dot1xAgeOutValInfo = {.agingTime = 100u};
    /* Step 1: Enable aging function to remove the authentication status of port-based DOT1X after the set aging time */
    CHK_FUN_RET(retVal, rtk_dot1x_set_aging_time_enable(unitChip, &dot1xAgeStatusInfo));
    /* Step 2: Set aging time */
    CHK_FUN_RET(retVal, rtk_dot1x_set_aging_time(unitChip, &dot1xAgeOutValInfo));
    /* Step 3: Trigger the dynamic setting flag to set the new configuration to the switch */
    CHK_FUN_RET(retVal, rtk_dot1x_set_dynamic_reset_flag(unitChip));
    /* Expected Behavior: After 100 seconds of successful authentication, the switch will become unauthorized */
    return retVal;
}
/**@}*/ /* RTL907XCVA_DOT1X_CONFIG_AGING_TIME_EXAMPLE */
/**@}*/ /* RTL907XCVA_DOT1X_EXAMPLE */
/**@}*/ /* DOT1X_EXAMPLE */
/**@}*/ /* EXAMPLE */