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

#ifndef DV1_RTKAS_REG_FIELD_FILE_CHIP_VERSION_H_
#define DV1_RTKAS_REG_FIELD_FILE_CHIP_VERSION_H_

#ifdef DV1_FILE_CHIP_VERSION
/* File: Chip Version */
#define DV1_MODEL_REVISION_INFORMATION_REV_ID_LSP  0u
#define DV1_MODEL_REVISION_INFORMATION_REV_ID_LEN  8u
#define DV1_MODEL_REVISION_INFORMATION_REV_ID_TYPE DV1_FIELD_TYPE_RWS

#define DV1_MODEL_NAME_INFORMATION_REGSITER_RTL_ID_LSP        20u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_RTL_ID_LEN        12u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_RTL_ID_TYPE       DV1_FIELD_TYPE_RWS
#define DV1_MODEL_NAME_INFORMATION_REGSITER_PORT_NUM_LSP      16u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_PORT_NUM_LEN      4u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_PORT_NUM_TYPE     DV1_FIELD_TYPE_RWS
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_3_LSP  11u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_3_LEN  5u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_3_TYPE DV1_FIELD_TYPE_RWS
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_2_LSP  6u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_2_LEN  5u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_2_TYPE DV1_FIELD_TYPE_RWS
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_1_LSP  1u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_1_LEN  5u
#define DV1_MODEL_NAME_INFORMATION_REGSITER_MODEL_CHAR_1_TYPE DV1_FIELD_TYPE_RWS

#define DV1_CHIP_VERSION_ID_RL_15_0_LSP    16u
#define DV1_CHIP_VERSION_ID_RL_15_0_LEN    16u
#define DV1_CHIP_VERSION_ID_RL_15_0_TYPE   DV1_FIELD_TYPE_RWS
#define DV1_CHIP_VERSION_ID_RLVID_3_0_LSP  12u
#define DV1_CHIP_VERSION_ID_RLVID_3_0_LEN  4u
#define DV1_CHIP_VERSION_ID_RLVID_3_0_TYPE DV1_FIELD_TYPE_RWS
#define DV1_CHIP_VERSION_ID_MCID_3_0_LSP   8u
#define DV1_CHIP_VERSION_ID_MCID_3_0_LEN   4u
#define DV1_CHIP_VERSION_ID_MCID_3_0_TYPE  DV1_FIELD_TYPE_RWS
#define DV1_CHIP_VERSION_ID_BOID_3_0_LSP   4u
#define DV1_CHIP_VERSION_ID_BOID_3_0_LEN   4u
#define DV1_CHIP_VERSION_ID_BOID_3_0_TYPE  DV1_FIELD_TYPE_RWS

#endif
#endif /* DV1_RTKAS_REG_FIELD_FILE_CHIP_VERSION_H_ */