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

#ifndef DVA_RTKAS_REG_LIST_FILE_MAILBOX_H_
#define DVA_RTKAS_REG_LIST_FILE_MAILBOX_H_

#ifdef DVA_FILE_MAILBOX
/* File: mailbox */
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE                 0x4A470000u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE_FIELDNUM        1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE_DEFAULTVAL      0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_ADDRESS                0x4A470004u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_ADDRESS_FIELDNUM       1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_ADDRESS_DEFAULTVAL     0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_DATA                   0x4A470008u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_DATA_FIELDNUM          1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_DATA_DEFAULTVAL        0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_STATUS                 0x4A47000Cu
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_STATUS_FIELDNUM        1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_STATUS_DEFAULTVAL      0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_OPCODE                 0x4A470010u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_OPCODE_FIELDNUM        1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_OPCODE_DEFAULTVAL      0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_ADDRESS                0x4A470014u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_ADDRESS_FIELDNUM       1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_ADDRESS_DEFAULTVAL     0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_DATA                   0x4A470018u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_DATA_FIELDNUM          1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_DATA_DEFAULTVAL        0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_STATUS                 0x4A47001Cu
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_STATUS_FIELDNUM        1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_STATUS_DEFAULTVAL      0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_OPCODE                 0x4A470018u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_OPCODE_FIELDNUM        1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_OPCODE_DEFAULTVAL      0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_PARAMETER_1            0x4A47001Cu
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_PARAMETER_1_FIELDNUM   1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_PARAMETER_1_DEFAULTVAL 0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_PARAMETER_2            0x4A470020u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_PARAMETER_2_FIELDNUM   1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_2_PARAMETER_2_DEFAULTVAL 0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_OPCODE                 0x4A470024u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_OPCODE_FIELDNUM        1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_OPCODE_DEFAULTVAL      0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_PARAMETER_1            0x4A470028u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_PARAMETER_1_FIELDNUM   1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_PARAMETER_1_DEFAULTVAL 0x0u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_PARAMETER_2            0x4A47002Cu
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_PARAMETER_2_FIELDNUM   1u
#define DVA_CPU_INDIRECT_ACCESS_CHANNEL_3_PARAMETER_2_DEFAULTVAL 0x0u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE                 0x4A470030u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE_FIELDNUM        1u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE_DEFAULTVAL      0x0u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1            0x4A470034u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1_FIELDNUM   1u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1_DEFAULTVAL 0x0u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_2            0x4A470038u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_2_FIELDNUM   1u
#define DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_2_DEFAULTVAL 0x0u
#define DVA_DMY_REG_EXT_CHK_1                                    0x4A47003Cu
#define DVA_DMY_REG_EXT_CHK_1_FIELDNUM                           2u
#define DVA_DMY_REG_EXT_CHK_1_DEFAULTVAL                         0x5Au
#define DVA_DMY_REG_EXT_CHK_2                                    0x4A470040u
#define DVA_DMY_REG_EXT_CHK_2_FIELDNUM                           2u
#define DVA_DMY_REG_EXT_CHK_2_DEFAULTVAL                         0xA5u
#define DVA_DMY_REG_EXT_CHK_3                                    0x4A470044u
#define DVA_DMY_REG_EXT_CHK_3_FIELDNUM                           2u
#define DVA_DMY_REG_EXT_CHK_3_DEFAULTVAL                         0xC3u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_MAILBOX_H_ */