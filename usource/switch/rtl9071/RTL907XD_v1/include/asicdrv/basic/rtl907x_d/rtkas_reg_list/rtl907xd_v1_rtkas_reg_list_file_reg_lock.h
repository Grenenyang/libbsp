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

#ifndef DV1_RTKAS_REG_LIST_FILE_REG_LOCK_H_
#define DV1_RTKAS_REG_LIST_FILE_REG_LOCK_H_

#ifdef DV1_FILE_REG_LOCK
/* File: reg lock */
#define DV1_SWITCH_OCP_LOCK                     0x4A1E0000u
#define DV1_SWITCH_OCP_LOCK_FIELDNUM            24u
#define DV1_SWITCH_OCP_LOCK_DEFAULTVAL          0x0u
#define DV1_SWITCH_ALE_REG_LOCK                 0x4A1E0004u
#define DV1_SWITCH_ALE_REG_LOCK_FIELDNUM        25u
#define DV1_SWITCH_ALE_REG_LOCK_DEFAULTVAL      0x0u
#define DV1_SWITCH_MAC_REG_LOCK                 0x4A1E0008u
#define DV1_SWITCH_MAC_REG_LOCK_FIELDNUM        32u
#define DV1_SWITCH_MAC_REG_LOCK_DEFAULTVAL      0x0u
#define DV1_SWITCH_REG_LOCK                     0x4A1E000Cu
#define DV1_SWITCH_REG_LOCK_FIELDNUM            18u
#define DV1_SWITCH_REG_LOCK_DEFAULTVAL          0x0u
#define DV1_CPUREG_WRAP_LOCK                    0x4A1E0010u
#define DV1_CPUREG_WRAP_LOCK_FIELDNUM           7u
#define DV1_CPUREG_WRAP_LOCK_DEFAULTVAL         0x0u
#define DV1_SWITCH_OCP_LOCK_MASK                0x4A1E0014u
#define DV1_SWITCH_OCP_LOCK_MASK_FIELDNUM       24u
#define DV1_SWITCH_OCP_LOCK_MASK_DEFAULTVAL     0x0u
#define DV1_SWITCH_ALE_REG_LOCK_MASK            0x4A1E0018u
#define DV1_SWITCH_ALE_REG_LOCK_MASK_FIELDNUM   25u
#define DV1_SWITCH_ALE_REG_LOCK_MASK_DEFAULTVAL 0x0u
#define DV1_SWITCH_MAC_REG_LOCK_MASK            0x4A1E001Cu
#define DV1_SWITCH_MAC_REG_LOCK_MASK_FIELDNUM   32u
#define DV1_SWITCH_MAC_REG_LOCK_MASK_DEFAULTVAL 0x0u
#define DV1_SWITCH_REG_LOCK_MASK                0x4A1E0020u
#define DV1_SWITCH_REG_LOCK_MASK_FIELDNUM       17u
#define DV1_SWITCH_REG_LOCK_MASK_DEFAULTVAL     0x0u
#define DV1_CPUREG_WRAP_LOCK_MASK               0x4A1E0024u
#define DV1_CPUREG_WRAP_LOCK_MASK_FIELDNUM      7u
#define DV1_CPUREG_WRAP_LOCK_MASK_DEFAULTVAL    0x0u
#define DV1_LOCK_ENABLE                         0x4A1E0028u
#define DV1_LOCK_ENABLE_FIELDNUM                1u
#define DV1_LOCK_ENABLE_DEFAULTVAL              0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_REG_LOCK_H_ */