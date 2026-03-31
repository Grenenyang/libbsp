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

#ifndef DVA_RTKAS_REG_LIST_FILE_INTERRUPT_TOP_H_
#define DVA_RTKAS_REG_LIST_FILE_INTERRUPT_TOP_H_

#ifdef DVA_FILE_INTERRUPT_TOP
/* File: Interrupt Top */
#define DVA_RG_INTERRUPT_TOP_VERSION                  0x4A6F0000u
#define DVA_RG_INTERRUPT_TOP_VERSION_FIELDNUM         1u
#define DVA_RG_INTERRUPT_TOP_VERSION_DEFAULTVAL       0x2u
#define DVA_RG_INTERRUPT_TOP_EXT_INTR_MODE            0x4A6F0004u
#define DVA_RG_INTERRUPT_TOP_EXT_INTR_MODE_FIELDNUM   7u
#define DVA_RG_INTERRUPT_TOP_EXT_INTR_MODE_DEFAULTVAL 0x0u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP0              0x4A6F0008u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP0_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP0_DEFAULTVAL   0x20000000u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP1              0x4A6F000Cu
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP1_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP1_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP2              0x4A6F0010u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP2_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP2_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP3              0x4A6F0014u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP3_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IE_GRP3_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP0              0x4A6F0018u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP0_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP0_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP1              0x4A6F001Cu
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP1_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP1_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP2              0x4A6F0020u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP2_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP2_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP3              0x4A6F0024u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP3_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_INT_IP_GRP3_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP0              0x4A6F0028u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP0_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP0_DEFAULTVAL   0x23000000u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP1              0x4A6F002Cu
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP1_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP1_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP2              0x4A6F0030u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP2_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP2_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP3              0x4A6F0034u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP3_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IE_GRP3_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP0              0x4A6F0038u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP0_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP0_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP1              0x4A6F003Cu
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP1_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP1_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP2              0x4A6F0040u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP2_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP2_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP3              0x4A6F0044u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP3_FIELDNUM     1u
#define DVA_RG_INTERRUPT_TOP_EXT_IP_GRP3_DEFAULTVAL   0x0u
#define DVA_RG_INTERRUPT_TOP_POL_GRP0                 0x4A6F0048u
#define DVA_RG_INTERRUPT_TOP_POL_GRP0_FIELDNUM        1u
#define DVA_RG_INTERRUPT_TOP_POL_GRP0_DEFAULTVAL      0xFFDFFFFFu
#define DVA_RG_INTERRUPT_TOP_POL_GRP1                 0x4A6F004Cu
#define DVA_RG_INTERRUPT_TOP_POL_GRP1_FIELDNUM        1u
#define DVA_RG_INTERRUPT_TOP_POL_GRP1_DEFAULTVAL      0xFFFFFFFFu
#define DVA_RG_INTERRUPT_TOP_POL_GRP2                 0x4A6F0050u
#define DVA_RG_INTERRUPT_TOP_POL_GRP2_FIELDNUM        1u
#define DVA_RG_INTERRUPT_TOP_POL_GRP2_DEFAULTVAL      0xFDFF807Fu
#define DVA_RG_INTERRUPT_TOP_POL_GRP3                 0x4A6F0054u
#define DVA_RG_INTERRUPT_TOP_POL_GRP3_FIELDNUM        1u
#define DVA_RG_INTERRUPT_TOP_POL_GRP3_DEFAULTVAL      0xFFFFFFE0u
#define DVA_FUSA_PG_LOCK_DMY_INTERRUPT_TOP            0x4A6F0058u
#define DVA_FUSA_PG_LOCK_DMY_INTERRUPT_TOP_FIELDNUM   2u
#define DVA_FUSA_PG_LOCK_DMY_INTERRUPT_TOP_DEFAULTVAL 0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_INTERRUPT_TOP_H_ */