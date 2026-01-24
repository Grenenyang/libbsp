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

#ifndef DV1_RTKAS_REG_LIST_FILE_UART_H_
#define DV1_RTKAS_REG_LIST_FILE_UART_H_

#ifdef DV1_FILE_UART
/* File: UART */
#define DV1_DAT0_UART_DATA            0x4A680000u
#define DV1_DAT0_UART_DATA_FIELDNUM   2u
#define DV1_DAT0_UART_DATA_DEFAULTVAL 0x0u
#define DV1_DAT0_UART_IER             0x4A680004u
#define DV1_DAT0_UART_IER_FIELDNUM    6u
#define DV1_DAT0_UART_IER_DEFAULTVAL  0x0u
#define DV1_DAT0_UART_FCR             0x4A680008u
#define DV1_DAT0_UART_FCR_FIELDNUM    8u
#define DV1_DAT0_UART_FCR_DEFAULTVAL  0x1u
#define DV1_DAT0_UART_LCR             0x4A68000Cu
#define DV1_DAT0_UART_LCR_FIELDNUM    8u
#define DV1_DAT0_UART_LCR_DEFAULTVAL  0x3u
#define DV1_DAT0_UART_MCR             0x4A680010u
#define DV1_DAT0_UART_MCR_FIELDNUM    6u
#define DV1_DAT0_UART_MCR_DEFAULTVAL  0x0u
#define DV1_DAT0_UART_LSR             0x4A680014u
#define DV1_DAT0_UART_LSR_FIELDNUM    9u
#define DV1_DAT0_UART_LSR_DEFAULTVAL  0x0u
#define DV1_DAT0_UART_MSR             0x4A680018u
#define DV1_DAT0_UART_MSR_FIELDNUM    9u
#define DV1_DAT0_UART_MSR_DEFAULTVAL  0x0u
#define DV1_DAT0_UART_SPR             0x4A68001Cu
#define DV1_DAT0_UART_SPR_FIELDNUM    2u
#define DV1_DAT0_UART_SPR_DEFAULTVAL  0x2Bu
#define DV1_DAT0_UART_RTHR            0x4A680020u
#define DV1_DAT0_UART_RTHR_FIELDNUM   3u
#define DV1_DAT0_UART_RTHR_DEFAULTVAL 0xC8u
#define DV1_DAT0_UART_ISR             0x4A680024u
#define DV1_DAT0_UART_ISR_FIELDNUM    4u
#define DV1_DAT0_UART_ISR_DEFAULTVAL  0xC1u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_UART_H_ */