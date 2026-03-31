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

#ifndef DVA_RTKAS_REG_LIST_FILE_SPANNING_TREE_H_
#define DVA_RTKAS_REG_LIST_FILE_SPANNING_TREE_H_

#ifdef DVA_FILE_SPANNING_TREE
/* File: Spanning Tree */
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL0                      0x4A720000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL0_FIELDNUM             16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL0_DEFAULTVAL           0xFFFFFFFFu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL1_RESERVED             0x4A720004u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL1_RESERVED_FIELDNUM    1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL1_RESERVED_DEFAULTVAL  0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL2                      0x4A720008u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL2_FIELDNUM             16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL2_DEFAULTVAL           0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL3_RESERVED             0x4A72000Cu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL3_RESERVED_FIELDNUM    1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL3_RESERVED_DEFAULTVAL  0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL4                      0x4A720010u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL4_FIELDNUM             16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL4_DEFAULTVAL           0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL5_RESERVED             0x4A720014u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL5_RESERVED_FIELDNUM    1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL5_RESERVED_DEFAULTVAL  0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL6                      0x4A720018u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL6_FIELDNUM             16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL6_DEFAULTVAL           0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL7_RESERVED             0x4A72001Cu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL7_RESERVED_FIELDNUM    1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL7_RESERVED_DEFAULTVAL  0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL8                      0x4A720020u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL8_FIELDNUM             16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL8_DEFAULTVAL           0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL9_RESERVED             0x4A720024u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL9_RESERVED_FIELDNUM    1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL9_RESERVED_DEFAULTVAL  0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL10                     0x4A720028u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL10_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL10_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL11_RESERVED            0x4A72002Cu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL11_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL11_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL12                     0x4A720030u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL12_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL12_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL13_RESERVED            0x4A720034u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL13_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL13_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL14                     0x4A720038u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL14_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL14_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL15_RESERVED            0x4A72003Cu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL15_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL15_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL16                     0x4A720040u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL16_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL16_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL17_RESERVED            0x4A720044u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL17_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL17_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL18                     0x4A720048u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL18_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL18_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL19_RESERVED            0x4A72004Cu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL19_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL19_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL20                     0x4A720050u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL20_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL20_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL21_RESERVED            0x4A720054u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL21_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL21_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL22                     0x4A720058u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL22_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL22_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL23_RESERVED            0x4A72005Cu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL23_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL23_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL24                     0x4A720060u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL24_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL24_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL25_RESERVED            0x4A720064u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL25_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL25_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL26                     0x4A720068u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL26_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL26_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL27_RESERVED            0x4A72006Cu
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL27_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL27_RESERVED_DEFAULTVAL 0x0u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL28                     0x4A720070u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL28_FIELDNUM            16u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL28_DEFAULTVAL          0xFFFC0000u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL29_RESERVED            0x4A720074u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL29_RESERVED_FIELDNUM   1u
#define DVA_SPANNING_TREE_PORT_STATE_CONTROL29_RESERVED_DEFAULTVAL 0x0u
#define DVA_RSTP_GLOBAL                                            0x4A720078u
#define DVA_RSTP_GLOBAL_FIELDNUM                                   7u
#define DVA_RSTP_GLOBAL_DEFAULTVAL                                 0x1F523u
#define DVA_RSTP_PORT0                                             0x4A72007Cu
#define DVA_RSTP_PORT0_FIELDNUM                                    14u
#define DVA_RSTP_PORT0_DEFAULTVAL                                  0x80008u
#define DVA_RSTP_PORT1                                             0x4A720080u
#define DVA_RSTP_PORT1_FIELDNUM                                    14u
#define DVA_RSTP_PORT1_DEFAULTVAL                                  0x80008u
#define DVA_RSTP_PORT2                                             0x4A720084u
#define DVA_RSTP_PORT2_FIELDNUM                                    14u
#define DVA_RSTP_PORT2_DEFAULTVAL                                  0x80008u
#define DVA_RSTP_PORT3                                             0x4A720088u
#define DVA_RSTP_PORT3_FIELDNUM                                    14u
#define DVA_RSTP_PORT3_DEFAULTVAL                                  0x80008u
#define DVA_RSTP_PORT4                                             0x4A72008Cu
#define DVA_RSTP_PORT4_FIELDNUM                                    14u
#define DVA_RSTP_PORT4_DEFAULTVAL                                  0x80008u
#define DVA_RSTP_PORT5                                             0x4A720090u
#define DVA_RSTP_PORT5_FIELDNUM                                    14u
#define DVA_RSTP_PORT5_DEFAULTVAL                                  0x80008u
#define DVA_RSTP_PORT6                                             0x4A720094u
#define DVA_RSTP_PORT6_FIELDNUM                                    14u
#define DVA_RSTP_PORT6_DEFAULTVAL                                  0x80008u
#define DVA_RSTP_PORT7                                             0x4A720098u
#define DVA_RSTP_PORT7_FIELDNUM                                    7u
#define DVA_RSTP_PORT7_DEFAULTVAL                                  0x8u
#define DVA_RSTP_PORT8                                             0x4A72009Cu
#define DVA_RSTP_PORT8_FIELDNUM                                    1u
#define DVA_RSTP_PORT8_DEFAULTVAL                                  0x0u
#define DVA_RSTP_PORT9                                             0x4A7200A0u
#define DVA_RSTP_PORT9_FIELDNUM                                    1u
#define DVA_RSTP_PORT9_DEFAULTVAL                                  0x0u
#define DVA_RSTP_PORT10                                            0x4A7200A4u
#define DVA_RSTP_PORT10_FIELDNUM                                   1u
#define DVA_RSTP_PORT10_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT11                                            0x4A7200A8u
#define DVA_RSTP_PORT11_FIELDNUM                                   1u
#define DVA_RSTP_PORT11_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT12                                            0x4A7200ACu
#define DVA_RSTP_PORT12_FIELDNUM                                   1u
#define DVA_RSTP_PORT12_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT13                                            0x4A7200B0u
#define DVA_RSTP_PORT13_FIELDNUM                                   1u
#define DVA_RSTP_PORT13_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT14                                            0x4A7200B4u
#define DVA_RSTP_PORT14_FIELDNUM                                   1u
#define DVA_RSTP_PORT14_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT15                                            0x4A7200B8u
#define DVA_RSTP_PORT15_FIELDNUM                                   1u
#define DVA_RSTP_PORT15_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT16                                            0x4A7200BCu
#define DVA_RSTP_PORT16_FIELDNUM                                   1u
#define DVA_RSTP_PORT16_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT17                                            0x4A7200C0u
#define DVA_RSTP_PORT17_FIELDNUM                                   1u
#define DVA_RSTP_PORT17_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT18                                            0x4A7200C4u
#define DVA_RSTP_PORT18_FIELDNUM                                   1u
#define DVA_RSTP_PORT18_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT19                                            0x4A7200C8u
#define DVA_RSTP_PORT19_FIELDNUM                                   1u
#define DVA_RSTP_PORT19_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT20                                            0x4A7200CCu
#define DVA_RSTP_PORT20_FIELDNUM                                   1u
#define DVA_RSTP_PORT20_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT21                                            0x4A7200D0u
#define DVA_RSTP_PORT21_FIELDNUM                                   1u
#define DVA_RSTP_PORT21_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT22                                            0x4A7200D4u
#define DVA_RSTP_PORT22_FIELDNUM                                   1u
#define DVA_RSTP_PORT22_DEFAULTVAL                                 0x0u
#define DVA_RSTP_PORT23                                            0x4A7200D8u
#define DVA_RSTP_PORT23_FIELDNUM                                   1u
#define DVA_RSTP_PORT23_DEFAULTVAL                                 0x0u
#define DVA_FUSA_PG_LOCK_DMY_SPANNING_TREE                         0x4A7200DCu
#define DVA_FUSA_PG_LOCK_DMY_SPANNING_TREE_FIELDNUM                2u
#define DVA_FUSA_PG_LOCK_DMY_SPANNING_TREE_DEFAULTVAL              0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_SPANNING_TREE_H_ */