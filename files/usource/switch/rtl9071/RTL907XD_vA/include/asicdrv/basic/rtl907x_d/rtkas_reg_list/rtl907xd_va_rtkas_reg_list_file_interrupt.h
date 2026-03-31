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

#ifndef DVA_RTKAS_REG_LIST_FILE_INTERRUPT_H_
#define DVA_RTKAS_REG_LIST_FILE_INTERRUPT_H_

#ifdef DVA_FILE_INTERRUPT
/* File: Interrupt */
#define DVA_INTERRUPT_RSV_0                                             0x4A650000u
#define DVA_INTERRUPT_RSV_0_FIELDNUM                                    1u
#define DVA_INTERRUPT_RSV_0_DEFAULTVAL                                  0x0u
#define DVA_INTERRUPT_RSV_1                                             0x4A650004u
#define DVA_INTERRUPT_RSV_1_FIELDNUM                                    1u
#define DVA_INTERRUPT_RSV_1_DEFAULTVAL                                  0x0u
#define DVA_INTERRUPT_RSV_2                                             0x4A650008u
#define DVA_INTERRUPT_RSV_2_FIELDNUM                                    1u
#define DVA_INTERRUPT_RSV_2_DEFAULTVAL                                  0x0u
#define DVA_SWITCH_GLOBAL_INTERRUPT_ENABLE_0                            0x4A65000Cu
#define DVA_SWITCH_GLOBAL_INTERRUPT_ENABLE_0_FIELDNUM                   2u
#define DVA_SWITCH_GLOBAL_INTERRUPT_ENABLE_0_DEFAULTVAL                 0x0u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_ENABLE_0                   0x4A650010u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_ENABLE_0_FIELDNUM          2u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_ENABLE_0_DEFAULTVAL        0x0u
#define DVA_SWITCH_GLOBAL_INTERRUPT_PENDING_0                           0x4A650014u
#define DVA_SWITCH_GLOBAL_INTERRUPT_PENDING_0_FIELDNUM                  2u
#define DVA_SWITCH_GLOBAL_INTERRUPT_PENDING_0_DEFAULTVAL                0x0u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_PENDING_0                  0x4A650018u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_PENDING_0_FIELDNUM         2u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_PENDING_0_DEFAULTVAL       0x0u
#define DVA_SWITCH_GLOBAL_INTERRUPT_ENABLE_3                            0x4A65001Cu
#define DVA_SWITCH_GLOBAL_INTERRUPT_ENABLE_3_FIELDNUM                   2u
#define DVA_SWITCH_GLOBAL_INTERRUPT_ENABLE_3_DEFAULTVAL                 0x0u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_ENABLE_3                   0x4A650020u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_ENABLE_3_FIELDNUM          2u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_ENABLE_3_DEFAULTVAL        0x0u
#define DVA_SWITCH_GLOBAL_INTERRUPT_PENDING_3                           0x4A650024u
#define DVA_SWITCH_GLOBAL_INTERRUPT_PENDING_3_FIELDNUM                  2u
#define DVA_SWITCH_GLOBAL_INTERRUPT_PENDING_3_DEFAULTVAL                0x0u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_PENDING_3                  0x4A650028u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_PENDING_3_FIELDNUM         2u
#define DVA_SWITCH_GLOBAL_EXTERNAL_INTERRUPT_PENDING_3_DEFAULTVAL       0x0u
#define DVA_PACKET_DROP_INTERRUPT_ENABLE_0                              0x4A65002Cu
#define DVA_PACKET_DROP_INTERRUPT_ENABLE_0_FIELDNUM                     1u
#define DVA_PACKET_DROP_INTERRUPT_ENABLE_0_DEFAULTVAL                   0x0u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_ENABLE_0                     0x4A650030u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_ENABLE_0_FIELDNUM            1u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_ENABLE_0_DEFAULTVAL          0x0u
#define DVA_PACKET_DROP_INTERRUPT_PENDING_0                             0x4A650034u
#define DVA_PACKET_DROP_INTERRUPT_PENDING_0_FIELDNUM                    1u
#define DVA_PACKET_DROP_INTERRUPT_PENDING_0_DEFAULTVAL                  0x0u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_PENDING_0                    0x4A650038u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_PENDING_0_FIELDNUM           1u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_PENDING_0_DEFAULTVAL         0x0u
#define DVA_PACKET_DROP_INTERRUPT_ENABLE_1                              0x4A65003Cu
#define DVA_PACKET_DROP_INTERRUPT_ENABLE_1_FIELDNUM                     2u
#define DVA_PACKET_DROP_INTERRUPT_ENABLE_1_DEFAULTVAL                   0x0u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_ENABLE_1                     0x4A650040u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_ENABLE_1_FIELDNUM            2u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_ENABLE_1_DEFAULTVAL          0x0u
#define DVA_PACKET_DROP_INTERRUPT_PENDING_1                             0x4A650044u
#define DVA_PACKET_DROP_INTERRUPT_PENDING_1_FIELDNUM                    2u
#define DVA_PACKET_DROP_INTERRUPT_PENDING_1_DEFAULTVAL                  0x0u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_PENDING_1                    0x4A650048u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_PENDING_1_FIELDNUM           2u
#define DVA_PACKET_DROP_EXTERNAL_INTERRUPT_PENDING_1_DEFAULTVAL         0x0u
#define DVA_LINK_CHANGE_INTERRUPT_ENABLE                                0x4A65004Cu
#define DVA_LINK_CHANGE_INTERRUPT_ENABLE_FIELDNUM                       2u
#define DVA_LINK_CHANGE_INTERRUPT_ENABLE_DEFAULTVAL                     0x0u
#define DVA_LINK_CHANGE_EXTERNAL_INTERRUPT_ENABLE                       0x4A650050u
#define DVA_LINK_CHANGE_EXTERNAL_INTERRUPT_ENABLE_FIELDNUM              2u
#define DVA_LINK_CHANGE_EXTERNAL_INTERRUPT_ENABLE_DEFAULTVAL            0x0u
#define DVA_LINK_CHANGE_INTERRUPT_PENDING                               0x4A650054u
#define DVA_LINK_CHANGE_INTERRUPT_PENDING_FIELDNUM                      2u
#define DVA_LINK_CHANGE_INTERRUPT_PENDING_DEFAULTVAL                    0x8000u
#define DVA_LINK_CHANGE_EXTERNAL_INTERRUPT_PENDING                      0x4A650058u
#define DVA_LINK_CHANGE_EXTERNAL_INTERRUPT_PENDING_FIELDNUM             2u
#define DVA_LINK_CHANGE_EXTERNAL_INTERRUPT_PENDING_DEFAULTVAL           0x8000u
#define DVA_SA_LEARNING_CONSTRAIN_INTERRUPT_ENABLE                      0x4A65005Cu
#define DVA_SA_LEARNING_CONSTRAIN_INTERRUPT_ENABLE_FIELDNUM             2u
#define DVA_SA_LEARNING_CONSTRAIN_INTERRUPT_ENABLE_DEFAULTVAL           0x0u
#define DVA_SA_LEARNING_CONSTRAIN_EXTERNAL_INTERRUPT_ENABLE             0x4A650060u
#define DVA_SA_LEARNING_CONSTRAIN_EXTERNAL_INTERRUPT_ENABLE_FIELDNUM    2u
#define DVA_SA_LEARNING_CONSTRAIN_EXTERNAL_INTERRUPT_ENABLE_DEFAULTVAL  0x0u
#define DVA_SA_LEARNING_CONSTRAIN_INTERRUPT_PENDING                     0x4A650064u
#define DVA_SA_LEARNING_CONSTRAIN_INTERRUPT_PENDING_FIELDNUM            2u
#define DVA_SA_LEARNING_CONSTRAIN_INTERRUPT_PENDING_DEFAULTVAL          0x0u
#define DVA_SA_LEARNING_CONSTRAIN_EXTERNAL_INTERRUPT_PENDING            0x4A650068u
#define DVA_SA_LEARNING_CONSTRAIN_EXTERNAL_INTERRUPT_PENDING_FIELDNUM   2u
#define DVA_SA_LEARNING_CONSTRAIN_EXTERNAL_INTERRUPT_PENDING_DEFAULTVAL 0x0u
#define DVA_MISC_INTERRUPT_ENABLE                                       0x4A65006Cu
#define DVA_MISC_INTERRUPT_ENABLE_FIELDNUM                              2u
#define DVA_MISC_INTERRUPT_ENABLE_DEFAULTVAL                            0x0u
#define DVA_MISC_EXTERNAL_INTERRUPT_ENABLE                              0x4A650070u
#define DVA_MISC_EXTERNAL_INTERRUPT_ENABLE_FIELDNUM                     2u
#define DVA_MISC_EXTERNAL_INTERRUPT_ENABLE_DEFAULTVAL                   0x0u
#define DVA_MISC_INTERRUPT_PENDING                                      0x4A650074u
#define DVA_MISC_INTERRUPT_PENDING_FIELDNUM                             2u
#define DVA_MISC_INTERRUPT_PENDING_DEFAULTVAL                           0x0u
#define DVA_MISC_EXTERNAL_INTERRUPT_PENDING                             0x4A650078u
#define DVA_MISC_EXTERNAL_INTERRUPT_PENDING_FIELDNUM                    2u
#define DVA_MISC_EXTERNAL_INTERRUPT_PENDING_DEFAULTVAL                  0x0u
#define DVA_L2_INTERRUPT_ENABLE_0                                       0x4A65007Cu
#define DVA_L2_INTERRUPT_ENABLE_0_FIELDNUM                              2u
#define DVA_L2_INTERRUPT_ENABLE_0_DEFAULTVAL                            0x0u
#define DVA_L2_EXTERNAL_INTERRUPT_ENABLE_0                              0x4A650080u
#define DVA_L2_EXTERNAL_INTERRUPT_ENABLE_0_FIELDNUM                     2u
#define DVA_L2_EXTERNAL_INTERRUPT_ENABLE_0_DEFAULTVAL                   0x0u
#define DVA_L2_INTERRUPT_PENDING_0                                      0x4A650084u
#define DVA_L2_INTERRUPT_PENDING_0_FIELDNUM                             2u
#define DVA_L2_INTERRUPT_PENDING_0_DEFAULTVAL                           0x0u
#define DVA_L2_EXTERNAL_INTERRUPT_PENDING_0                             0x4A650088u
#define DVA_L2_EXTERNAL_INTERRUPT_PENDING_0_FIELDNUM                    2u
#define DVA_L2_EXTERNAL_INTERRUPT_PENDING_0_DEFAULTVAL                  0x0u
#define DVA_TCAM_ERROR_INTERRUPT_ENABLE_0                               0x4A65008Cu
#define DVA_TCAM_ERROR_INTERRUPT_ENABLE_0_FIELDNUM                      2u
#define DVA_TCAM_ERROR_INTERRUPT_ENABLE_0_DEFAULTVAL                    0x0u
#define DVA_TCAM_ERROR_EXTERNAL_INTERRUPT_ENABLE_0                      0x4A650090u
#define DVA_TCAM_ERROR_EXTERNAL_INTERRUPT_ENABLE_0_FIELDNUM             2u
#define DVA_TCAM_ERROR_EXTERNAL_INTERRUPT_ENABLE_0_DEFAULTVAL           0x0u
#define DVA_TCAM_ERROR_INTERRUPT_PENDING_0                              0x4A650094u
#define DVA_TCAM_ERROR_INTERRUPT_PENDING_0_FIELDNUM                     2u
#define DVA_TCAM_ERROR_INTERRUPT_PENDING_0_DEFAULTVAL                   0x0u
#define DVA_TCAM_ERROR_EXTERNAL_INTERRUPT_PENDING_0                     0x4A650098u
#define DVA_TCAM_ERROR_EXTERNAL_INTERRUPT_PENDING_0_FIELDNUM            2u
#define DVA_TCAM_ERROR_EXTERNAL_INTERRUPT_PENDING_0_DEFAULTVAL          0x0u
#define DVA_XMII_TXC_DEADLOCK_INTERRUPT_ENABLE                          0x4A65009Cu
#define DVA_XMII_TXC_DEADLOCK_INTERRUPT_ENABLE_FIELDNUM                 2u
#define DVA_XMII_TXC_DEADLOCK_INTERRUPT_ENABLE_DEFAULTVAL               0x0u
#define DVA_XMII_TXC_DEADLOCK_EXTERNAL_INTERRUPT_ENABLE                 0x4A6500A0u
#define DVA_XMII_TXC_DEADLOCK_EXTERNAL_INTERRUPT_ENABLE_FIELDNUM        2u
#define DVA_XMII_TXC_DEADLOCK_EXTERNAL_INTERRUPT_ENABLE_DEFAULTVAL      0x0u
#define DVA_XMII_TXC_DEADLOCK_INTERRUPT_PENDING                         0x4A6500A4u
#define DVA_XMII_TXC_DEADLOCK_INTERRUPT_PENDING_FIELDNUM                2u
#define DVA_XMII_TXC_DEADLOCK_INTERRUPT_PENDING_DEFAULTVAL              0x0u
#define DVA_XMII_TXC_DEADLOCK_EXTERNAL_INTERRUPT_PENDING                0x4A6500A8u
#define DVA_XMII_TXC_DEADLOCK_EXTERNAL_INTERRUPT_PENDING_FIELDNUM       2u
#define DVA_XMII_TXC_DEADLOCK_EXTERNAL_INTERRUPT_PENDING_DEFAULTVAL     0x0u
#define DVA_XMII_RXC_DEADLOCK_INTERRUPT_ENABLE                          0x4A6500ACu
#define DVA_XMII_RXC_DEADLOCK_INTERRUPT_ENABLE_FIELDNUM                 2u
#define DVA_XMII_RXC_DEADLOCK_INTERRUPT_ENABLE_DEFAULTVAL               0x0u
#define DVA_XMII_RXC_DEADLOCK_EXTERNAL_INTERRUPT_ENABLE                 0x4A6500B0u
#define DVA_XMII_RXC_DEADLOCK_EXTERNAL_INTERRUPT_ENABLE_FIELDNUM        2u
#define DVA_XMII_RXC_DEADLOCK_EXTERNAL_INTERRUPT_ENABLE_DEFAULTVAL      0x0u
#define DVA_XMII_RXC_DEADLOCK_INTERRUPT_PENDING                         0x4A6500B4u
#define DVA_XMII_RXC_DEADLOCK_INTERRUPT_PENDING_FIELDNUM                2u
#define DVA_XMII_RXC_DEADLOCK_INTERRUPT_PENDING_DEFAULTVAL              0x0u
#define DVA_XMII_RXC_DEADLOCK_EXTERNAL_INTERRUPT_PENDING                0x4A6500B8u
#define DVA_XMII_RXC_DEADLOCK_EXTERNAL_INTERRUPT_PENDING_FIELDNUM       2u
#define DVA_XMII_RXC_DEADLOCK_EXTERNAL_INTERRUPT_PENDING_DEFAULTVAL     0x0u
#define DVA_FUSA_PG_LOCK_DMY_INTERRUPT                                  0x4A6500BCu
#define DVA_FUSA_PG_LOCK_DMY_INTERRUPT_FIELDNUM                         2u
#define DVA_FUSA_PG_LOCK_DMY_INTERRUPT_DEFAULTVAL                       0x0u
#define DVA_RX_FILTER_INTERRUPT_ENABLE                                  0x4A6500C0u
#define DVA_RX_FILTER_INTERRUPT_ENABLE_FIELDNUM                         2u
#define DVA_RX_FILTER_INTERRUPT_ENABLE_DEFAULTVAL                       0x0u
#define DVA_RX_FILTER_INTERRUPT_PENDING                                 0x4A6500C4u
#define DVA_RX_FILTER_INTERRUPT_PENDING_FIELDNUM                        2u
#define DVA_RX_FILTER_INTERRUPT_PENDING_DEFAULTVAL                      0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_INTERRUPT_H_ */