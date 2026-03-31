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

#ifndef DV1_RTKAS_REG_LIST_FILE_CHIP_VERSION_H_
#define DV1_RTKAS_REG_LIST_FILE_CHIP_VERSION_H_

#ifdef DV1_FILE_CHIP_VERSION
/* File: Chip Version */
#define DV1_MODEL_REVISION_INFORMATION                 0x4A46FFF4u
#define DV1_MODEL_REVISION_INFORMATION_FIELDNUM        2u
#define DV1_MODEL_REVISION_INFORMATION_DEFAULTVAL      0x0u
#define DV1_MODEL_NAME_INFORMATION_REGSITER            0x4A46FFF8u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_FIELDNUM   6u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_DEFAULTVAL 0x0u
#define DV1_CHIP_VERSION_ID                            0x4A46FFFCu
#define DV1_CHIP_VERSION_ID_FIELDNUM                   5u
#define DV1_CHIP_VERSION_ID_DEFAULTVAL                 0x67640000u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_CHIP_VERSION_H_ */