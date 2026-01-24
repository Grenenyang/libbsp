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

#ifndef DVA_RTK_API_OTP_H__
#define DVA_RTK_API_OTP_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xd_va_api.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* Indirect Access OTP buffer byte data max */
#define DVA_OTP_INDIRECT_ACCESS_BUF_BYTE_DATA_MAX (0xFF000000u)
/* Indirect Access OTP alignment byte */
#define DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BYTE (4u)
/* Indirect Access OTP alignment bit */
#define DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BIT (8u)

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet dva_otp_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet dva_otp_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);

#endif /* DVA_RTK_API_OTP_H__ */
