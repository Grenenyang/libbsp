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
 * @defgroup RTL907XCVB_DOT1X_RESET_EXAMPLE Dot1x Reset Example
 *
 * @brief <b>Reset DOT1X register to default values.</b>
 *
 * @section Purpose
 *          This example demonstrates how to reset DOT1X register.
 * @section Description
 *          DOT1X register will be reset to default values.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_dot1x_reset(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_dot1x_reset(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
    /* Step 1: Reset DOT1X */
    CHK_FUN_RET(retVal, rtk_dot1x_hw_reset(unitChip));
    /* Expected Behavior: DOT1X registers are reset to default values */
    return retVal;
}
/**@}*/ /* RTL907XCVB_DOT1X_RESET_EXAMPLE */
/**@}*/ /* RTL907XCVB_DOT1X_EXAMPLE */
/**@}*/ /* DOT1X_EXAMPLE */
