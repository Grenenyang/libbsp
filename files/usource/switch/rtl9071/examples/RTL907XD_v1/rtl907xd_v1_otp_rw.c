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
#include "rtkas_api_otp.h"
#include "rtl907xd_v1_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup OTP_EXAMPLE OTP Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_OTP_EXAMPLE RTL907XD_V1 OTP Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_OTP_RW_EXAMPLE OTP Read Write Example
 * @brief <b>OTP Read Write</b>
 *
 * @section Purpose
 *   This example is to demonstrate how to read and write data to the One-Time Programmable (OTP) memory, allowing for the storage and retrieval of non-volatile data on a hardware device.\n\n
 *
 * @section Description
 *   This example shows how to perform both read and write operations to the OTP memory. \n
 *   The read operation retrieves permanent data from the OTP, while the write operation stores data that will remain unchanged after programming.\n
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_otp_rw(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_otp_rw(void)
{
    RtkApiRet  retVal;
    UnitChip_t unitChip;
    OtpRw_t    otpRw;
    uint8      rBuf[1] = {0x0u};
    uint8      wBuf[1] = {0xB3u};

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(&otpRw, 0, sizeof(otpRw));

    /* Step 1. Write OTP via API */
    otpRw.addr = 0xC4u;
    otpRw.len  = 1u;
    otpRw.buf  = wBuf;
    CHK_FUN_RET(retVal, rtk_otp_write(unitChip, &otpRw));

    /* Step 2. Read OTP via API */
    otpRw.buf = rBuf;
    CHK_FUN_RET(retVal, rtk_otp_read(unitChip, &otpRw));

    /* Expected Behaviour: The rBuf is the same as the wBuf. */

    return retVal;
}
/**@}*/ /* RTL907XD_V1_OTP_RW_EXAMPLE */
/**@}*/ /* RTL907XD_V1_OTP_EXAMPLE */
/**@}*/ /* OTP_EXAMPLE */
/**@}*/ /* EXAMPLE */
