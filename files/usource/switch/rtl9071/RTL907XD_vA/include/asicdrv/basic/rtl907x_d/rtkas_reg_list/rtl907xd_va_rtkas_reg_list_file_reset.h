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

#ifndef DVA_RTKAS_REG_LIST_FILE_RESET_H_
#define DVA_RTKAS_REG_LIST_FILE_RESET_H_

#ifdef DVA_FILE_RESET
/* File: Reset */
#define DVA_RESET_GLOBAL_CONTROL0                    0x4A420000u
#define DVA_RESET_GLOBAL_CONTROL0_FIELDNUM           6u
#define DVA_RESET_GLOBAL_CONTROL0_DEFAULTVAL         0x1Fu
#define DVA_RESET_GLOBAL_CONTROL1                    0x4A420004u
#define DVA_RESET_GLOBAL_CONTROL1_FIELDNUM           10u
#define DVA_RESET_GLOBAL_CONTROL1_DEFAULTVAL         0x1u
#define DVA_RESET_GLOBAL_CONTROL2                    0x4A420008u
#define DVA_RESET_GLOBAL_CONTROL2_FIELDNUM           30u
#define DVA_RESET_GLOBAL_CONTROL2_DEFAULTVAL         0x5C3FF01u
#define DVA_RESET_GLOBAL_STATUS                      0x4A42000Cu
#define DVA_RESET_GLOBAL_STATUS_FIELDNUM             24u
#define DVA_RESET_GLOBAL_STATUS_DEFAULTVAL           0x0u
#define DVA_RESET_DMY                                0x4A420010u
#define DVA_RESET_DMY_FIELDNUM                       2u
#define DVA_RESET_DMY_DEFAULTVAL                     0xFFFFu
#define DVA_RESET_DMY2                               0x4A420014u
#define DVA_RESET_DMY2_FIELDNUM                      2u
#define DVA_RESET_DMY2_DEFAULTVAL                    0x0u
#define DVA_PAST_GLOBAL_CONTROL_STATUS               0x4A420018u
#define DVA_PAST_GLOBAL_CONTROL_STATUS_FIELDNUM      30u
#define DVA_PAST_GLOBAL_CONTROL_STATUS_DEFAULTVAL    0x0u
#define DVA_CURRENT_GLOBAL_CONTROL_STATUS            0x4A42001Cu
#define DVA_CURRENT_GLOBAL_CONTROL_STATUS_FIELDNUM   30u
#define DVA_CURRENT_GLOBAL_CONTROL_STATUS_DEFAULTVAL 0x0u
#define DVA_PER_PORT_RESET_CONTROL                   0x4A420020u
#define DVA_PER_PORT_RESET_CONTROL_FIELDNUM          2u
#define DVA_PER_PORT_RESET_CONTROL_DEFAULTVAL        0x0u
#define DVA_FUSA_PG_LOCK_DMY_RESET                   0x4A420024u
#define DVA_FUSA_PG_LOCK_DMY_RESET_FIELDNUM          2u
#define DVA_FUSA_PG_LOCK_DMY_RESET_DEFAULTVAL        0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_RESET_H_ */