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

#include "rtkas_types.h"
#include "rtkas_system_porting.h"
#include "rtl907xc_va_asicdrv_stacking.h"
#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtl907xc_va_api_stacking.h"
#include "rtkas_api_stacking.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup STK_EXAMPLE Stacking Example
 * @{
 */

/**
 * @addtogroup RTL907CVA_STACKING_EXAMPLE RTL907XC_VA Stacking Example
 * @{
 */

/**
 * @addtogroup RTL907CVA_STACKING_OTP_EXAMPLE Stacking OTP Set Example
 * @brief <b>Setup stacking OTP</b>
 *
 * @section Purpose
 *   Set and get OTP configuration for stacking role and link.
 *
 * @section Description
 *   This example shows how to set and get the stacking role and link with OTP.
 *
 * @image html stacking/stacking_otp.jpg
 *
 * @{
 */
RtkApiRet cva_stacking_otp_set(uint32 unit);
RtkApiRet cva_stacking_otp_set(uint32 unit)
{
    RtkApiRet            retVal = RT_ERR_OK;
    UnitChip_t           unitChip;
    StackingBootOtpSet_t stackingBootOtpSet;
    StackingBootOtpGet_t stackingBootOtpGet;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;

    do
    {
        /* Step 1. Program the stacking OTP to
         * Stacking role: SLAVE
         * Port S0(Port 10): Enable for stacking
         * Port S1(Port 11): Disable
         */
        stackingBootOtpSet.role         = STACK_BOOT_SLAVE;
        stackingBootOtpSet.portS0Enable = PORT_STACKING;
        stackingBootOtpSet.portS1Enable = PORT_NON_STACKING;
        CHK_FUN_CONTINUE(retVal, rtk_stacking_boot_otp_set(unitChip, &stackingBootOtpSet));
        /* Expected Behaviour: The stacking related OTP is been programmed */

        /* Step 2. Read back the stacking OTP state */
        CHK_FUN_CONTINUE(retVal, rtk_stacking_boot_otp_get(unitChip, &stackingBootOtpGet));
        /* Expected Behaviour: Get role == STACK_BOOT_SLAVE, portS0Enable == PORT_STACKING, portS1Enable == PORT_NON_STACKING */
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907CVA_STACKING_OTP_EXAMPLE */
/**@}*/ /* RTL907CVA_STACKING_EXAMPLE */
/**@}*/ /* STK_EXAMPLE */
/**@}*/ /* EXAMPLE */