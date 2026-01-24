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

#ifndef DVA_RTKAS_REG_LIST_FILE_CHIP_VERSION_H_
#define DVA_RTKAS_REG_LIST_FILE_CHIP_VERSION_H_

#ifdef DVA_FILE_CHIP_VERSION
/* File: Chip Version */
#define DVA_MODEL_REVISION_INFORMATION                 0x4A46FFF0u
#define DVA_MODEL_REVISION_INFORMATION_FIELDNUM        2u
#define DVA_MODEL_REVISION_INFORMATION_DEFAULTVAL      0x0u
#define DVA_MODEL_NAME_INFORMATION_REGSITER            0x4A46FFF4u
#define DVA_MODEL_NAME_INFORMATION_REGSITER_FIELDNUM   6u
#define DVA_MODEL_NAME_INFORMATION_REGSITER_DEFAULTVAL 0x0u
#define DVA_CHIP_VERSION_ID                            0x4A46FFF8u
#define DVA_CHIP_VERSION_ID_FIELDNUM                   5u
#define DVA_CHIP_VERSION_ID_DEFAULTVAL                 0x67650000u
#define DVA_FUSA_PG_LOCK_DMY_CHIP_VERSION              0x4A46FFFCu
#define DVA_FUSA_PG_LOCK_DMY_CHIP_VERSION_FIELDNUM     2u
#define DVA_FUSA_PG_LOCK_DMY_CHIP_VERSION_DEFAULTVAL   0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_CHIP_VERSION_H_ */