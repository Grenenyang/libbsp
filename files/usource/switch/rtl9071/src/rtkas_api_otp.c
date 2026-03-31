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
#include "rtkas_error.h"
#include "rtkas_api_otp.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_otp.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_otp.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_otp.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_otp.h"
#endif

/**
 * @defgroup RTK_MODULE_OTP OTP
 * @brief OTP API. This component is used for accessing OTP.
 * @{
 */

/**
 * @defgroup OTP_WRITE Write
 * @brief OTP write.
 * @{
 */

/**
 * @brief OTP write. \n
 *        OTP write functionality allows permanent data to be programmed into OTP memory, ensuring that it cannot be changed once written. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] otpRw OTP Read/Write parameter (N/A) \n
 *            The struct includes the address, buffer and length. \n
 *            The address specifies the memory location, buffer holds the data, and length determines the amount of data to be written in OTP memory.
 * @param[out] None
 * @retval RT_ERR_OK              The OTP has been successfully programmed.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dv1_otp_write(), dva_otp_write(), cvb_otp_write(), cva_otp_write().
 */
RtkApiRet rtk_otp_write(UnitChip_t unitChip, const OtpRw_t *otpRw)
{
    RtkApiRet ret;

    if(NULL == otpRw)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_otp_write(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_otp_write(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_otp_write(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_otp_write(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /* OTP_WRITE */

/**
 * @defgroup OTP_READ Read
 * @brief OTP read
 * @{
 */

/**
 * @brief OTP read. \n
 *        OTP read functionality retrieves permanent data stored in OTP memory, which remains unchanged after programming. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] otpRw OTP Read/Write parameter (N/A) \n
 *            The struct includes the address, buffer and length. \n
 *            The address specifies the memory location, buffer holds the data, and length determines the amount of data to be read in OTP memory.
 * @retval RT_ERR_OK              The OTP has been successfully read.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dv1_otp_read(), dva_otp_read(), cvb_otp_read(), cva_otp_read().
 */
RtkApiRet rtk_otp_read(UnitChip_t unitChip, const OtpRw_t *otpRw)
{
    RtkApiRet ret;

    if(NULL == otpRw)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_otp_read(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_otp_read(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_otp_read(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_otp_read(unitChip.unit, otpRw->addr, otpRw->buf, otpRw->len);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /* OTP_READ */

/**@}*/ /* OTP */
