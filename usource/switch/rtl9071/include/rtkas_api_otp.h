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

#ifndef RTKAS_API_OTP_H__
#define RTKAS_API_OTP_H__

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
/**
 * @brief This struct is used for reading and writing.
 * The addr and len are inputs, and the buf can be either an input or an output.
 */
typedef struct
{
    uint32 addr; /**< [in] OTP address (0x0 to <CHIPID>_OTP_BYTE_END_ADDR) */
    uint8 *buf;  /**< [in,out] OTP data buffer (each byte ranges from 0x00 to 0xFF) */
    uint32 len;  /**< [in] OTP length (1 to <CHIPID>_OTP_BYTE_CNT) */
} OtpRw_t;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_otp_write(UnitChip_t unitChip, const OtpRw_t *otpRw);
EXTERN_API RtkApiRet rtk_otp_read(UnitChip_t unitChip, const OtpRw_t *otpRw);

#endif /* RTKAS_API_OTP_H__ */
