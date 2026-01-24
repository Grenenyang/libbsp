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

#ifndef DV1_RTKAS_REG_LIST_FILE_INTERRUPT_TOP_H_
#define DV1_RTKAS_REG_LIST_FILE_INTERRUPT_TOP_H_

#ifdef DV1_FILE_INTERRUPT_TOP
/* File: Interrupt Top */
#define DV1_RG_INTERRUPT_TOP_VERSION                      0x4A6F0000u
#define DV1_RG_INTERRUPT_TOP_VERSION_FIELDNUM             1u
#define DV1_RG_INTERRUPT_TOP_VERSION_DEFAULTVAL           0x1u
#define DV1_RG_INTERRUPT_TOP_EXT_INTR_MODE                0x4A6F0004u
#define DV1_RG_INTERRUPT_TOP_EXT_INTR_MODE_FIELDNUM       7u
#define DV1_RG_INTERRUPT_TOP_EXT_INTR_MODE_DEFAULTVAL     0x0u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_IE                  0x4A6F0008u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_IE_FIELDNUM         1u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_IE_DEFAULTVAL       0x7FFFFFFFu
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_IE                  0x4A6F000Cu
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_IE_FIELDNUM         1u
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_IE_DEFAULTVAL       0xFFFFFFDFu
#define DV1_RG_INTERRUPT_TOP_INTERNAL_IP                  0x4A6F0010u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_IP_FIELDNUM         1u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_IP_DEFAULTVAL       0x0u
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_IP                  0x4A6F0014u
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_IP_FIELDNUM         1u
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_IP_DEFAULTVAL       0x0u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_POLARITY            0x4A6F0018u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_POLARITY_FIELDNUM   1u
#define DV1_RG_INTERRUPT_TOP_INTERNAL_POLARITY_DEFAULTVAL 0xEFFB006Bu
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_POLARITY            0x4A6F001Cu
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_POLARITY_FIELDNUM   1u
#define DV1_RG_INTERRUPT_TOP_EXTERNAL_POLARITY_DEFAULTVAL 0xEFFB006Fu
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_INTERRUPT_TOP_H_ */