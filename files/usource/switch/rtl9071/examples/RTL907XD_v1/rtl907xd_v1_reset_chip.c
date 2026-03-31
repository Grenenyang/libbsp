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
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtkas_api_reset.h"
#include "rtl907xd_v1_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup RESET_EXAMPLE Reset Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_RESET_EXAMPLE RTL907XD_V1 Reset Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_RESET_CHIP_EXAMPLE Reset Chip Example
 * @brief <b>Reset Chip</b>
 *
 * @section Purpose
 *   To initiate a reset of the switch by triggering a software-configured reset command, which restarts the internal processes.
 *
 * @section Description
 *   This reset operation is triggered via a software configuration setting, causing the switch's internal hardware circuits and registers to reset.\n
 *   Further details about the boot-up process are provided in the related boot-up document.\n
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_reset_chip(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_reset_chip(void)
{
    RtkApiRet  retVal;
    UnitChip_t unitChip;

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;

    /* Step 1. Trigger the chip reset */
    retVal = rtk_reset_chip_rst(unitChip);
    /* Expected Behaviour:
     * The return value is RT_ERROK.
     * The internal digital circuit within the SWITCH will be reset.
     */

    return retVal;
}
/**@}*/ /* RTL907XD_V1_RESET_CHIP_EXAMPLE */
/**@}*/ /* RTL907XD_V1_RESET_EXAMPLE */
/**@}*/ /* RESET_EXAMPLE */
/**@}*/ /* EXAMPLE */