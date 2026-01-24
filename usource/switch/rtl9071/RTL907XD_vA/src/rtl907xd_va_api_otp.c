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

#include <stdint.h>
#include "rtkas_types.h"
#include "rtkas_api_otp.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv.h"
#include "rtl907xd_va_asicdrv_otp.h"

/**
 * @addtogroup RTK_MODULE_OTP OTP
 * @brief OTP APIs
 * @{
 */

/**
 * @addtogroup OTP_WRITE OTP write
 * @{
 */

/**
 * @defgroup OTP_WRITE_ASIC Low Level Driver
 * @brief OTP Low Level Driver
 * @{
 */

/**
 * @brief Write OTP memory
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr  Address (0x0 to <CHIPID>_OTP_BYTE_CNT - len) \n
 *                  Address
 * @param[in] buf   Data buffer (each byte 0x0 to 0xFF) \n
 *                  Data buffer
 * @param[in] len   Length (1 to <CHIPID>_OTP_BYTE_CNT) \n
 *                  Length
 * @param[out] None
 * @retval RT_ERR_OK                          The OTP memory write successfully.
 * @retval Others Please refer to dva_drv_otp_write_err_chk(), dva_drv_otp_fw_status_chk(), dva_drv_otp_indir_write().
 */
RtkApiRet dva_otp_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal;

    retVal = dva_drv_otp_write_err_chk(unit, addr, buf, len);
    CHK_FUN_RET(retVal, dva_drv_otp_fw_status_chk(unit));

    /* Write the OTP indirectly */
    CHK_FUN_RET(retVal, dva_drv_otp_indir_write(unit, addr, buf, len));

    return retVal;
}
/**@}*/ /* OTP_WRITE_ASIC */
/**@}*/ /* OTP_WRITE */

/**
 * @addtogroup OTP_READ OTP read
 * @{
 */

/**
 * @defgroup OTP_READ_ASIC Low Level Driver
 * @brief OTP Low Level Driver
 * @{
 */

/**
 * @brief Read OTP memory
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr   Address (0x0 to <CHIPID>_OTP_BYTE_CNT - len) \n
 *                   Address
 * @param[out] buf   Data buffer (each byte 0x0 to 0xFF) \n
 *                   Data buffer
 * @param[in] len    Length (1 to <CHIPID>_OTP_BYTE_CNT) \n
 *                   Length
 * @param[out] None
 * @retval RT_ERR_OK                          The OTP memory read successfully.
 * @retval Others Please refer to dva_drv_otp_read_err_chk(), dva_drv_otp_fw_status_chk(), dva_drv_otp_indir_read().
 */
RtkApiRet dva_otp_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal;

    retVal = dva_drv_otp_read_err_chk(unit, addr, buf, len);
    CHK_FUN_RET(retVal, dva_drv_otp_fw_status_chk(unit));

    /* Read the OTP indirectly */
    CHK_FUN_RET(retVal, dva_drv_otp_indir_read(unit, addr, buf, len));

    return retVal;
}
/**@}*/ /* OTP_READ_ASIC */
/**@}*/ /* OTP_READ */

/**@}*/ /* OTP */
