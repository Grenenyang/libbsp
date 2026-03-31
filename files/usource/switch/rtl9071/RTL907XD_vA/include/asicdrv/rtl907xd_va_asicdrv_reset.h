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

#ifndef DVA_ASICDRV_RESET_H_
#define DVA_ASICDRV_RESET_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api_reset.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* Reset time in us */
#define DVA_RESET_TIME_US (1000u)
/* Reset gated register address */
#define DVA_RESET_GATED_ADDR (0x4A00141Cu)
/* Reset gated register bit */
#define DVA_RESET_GATED_BIT (3u)
/* Reset chip register address */
#define DVA_RESET_CHIP_ADDR (0x1200u)
/* Reset chip register bit */
#define DVA_RESET_CHIP_BIT (5u)
/* Reset chip register mask */
#define DVA_RESET_CHIP_MASK (0x1u)

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
RtkApiRet dva_drv_reset_chip_rst(uint32 unit);

#endif /* DVA_ASICDRV_RESET_H_ */