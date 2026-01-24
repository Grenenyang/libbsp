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

#ifndef DVA_RTKAS_REG_LIST_FILE_SAFETY_H_
#define DVA_RTKAS_REG_LIST_FILE_SAFETY_H_

#ifdef DVA_FILE_SAFETY
/* File: Safety */
#define DVA_AGGREGATED_SAFETY_INTERRUPT_IP            0x4A6D0000u
#define DVA_AGGREGATED_SAFETY_INTERRUPT_IP_FIELDNUM   2u
#define DVA_AGGREGATED_SAFETY_INTERRUPT_IP_DEFAULTVAL 0x0u
#define DVA_AGGREGATED_SAFETY_INTERRUPT_IE            0x4A6D0004u
#define DVA_AGGREGATED_SAFETY_INTERRUPT_IE_FIELDNUM   2u
#define DVA_AGGREGATED_SAFETY_INTERRUPT_IE_DEFAULTVAL 0x0u
#define DVA_MIB_0_IP                                  0x4A6D0008u
#define DVA_MIB_0_IP_FIELDNUM                         2u
#define DVA_MIB_0_IP_DEFAULTVAL                       0x0u
#define DVA_MIB_0_IE                                  0x4A6D000Cu
#define DVA_MIB_0_IE_FIELDNUM                         2u
#define DVA_MIB_0_IE_DEFAULTVAL                       0x0u
#define DVA_MIB_1_IP                                  0x4A6D0010u
#define DVA_MIB_1_IP_FIELDNUM                         2u
#define DVA_MIB_1_IP_DEFAULTVAL                       0x0u
#define DVA_MIB_1_IE                                  0x4A6D0014u
#define DVA_MIB_1_IE_FIELDNUM                         2u
#define DVA_MIB_1_IE_DEFAULTVAL                       0x0u
#define DVA_MIB_2_IP                                  0x4A6D0018u
#define DVA_MIB_2_IP_FIELDNUM                         2u
#define DVA_MIB_2_IP_DEFAULTVAL                       0x0u
#define DVA_MIB_2_IE                                  0x4A6D001Cu
#define DVA_MIB_2_IE_FIELDNUM                         2u
#define DVA_MIB_2_IE_DEFAULTVAL                       0x0u
#define DVA_DISB_MON_RX_IP                            0x4A6D0020u
#define DVA_DISB_MON_RX_IP_FIELDNUM                   2u
#define DVA_DISB_MON_RX_IP_DEFAULTVAL                 0x0u
#define DVA_DISB_MON_RX_IE                            0x4A6D0024u
#define DVA_DISB_MON_RX_IE_FIELDNUM                   2u
#define DVA_DISB_MON_RX_IE_DEFAULTVAL                 0x0u
#define DVA_DISB_MON_TX_IP                            0x4A6D0028u
#define DVA_DISB_MON_TX_IP_FIELDNUM                   2u
#define DVA_DISB_MON_TX_IP_DEFAULTVAL                 0x0u
#define DVA_DISB_MON_TX_IE                            0x4A6D002Cu
#define DVA_DISB_MON_TX_IE_FIELDNUM                   2u
#define DVA_DISB_MON_TX_IE_DEFAULTVAL                 0x0u
#define DVA_CLK_DET_SLOW_RX_IP                        0x4A6D0030u
#define DVA_CLK_DET_SLOW_RX_IP_FIELDNUM               2u
#define DVA_CLK_DET_SLOW_RX_IP_DEFAULTVAL             0x0u
#define DVA_CLK_DET_SLOW_RX_IE                        0x4A6D0034u
#define DVA_CLK_DET_SLOW_RX_IE_FIELDNUM               2u
#define DVA_CLK_DET_SLOW_RX_IE_DEFAULTVAL             0x0u
#define DVA_CLK_DET_FAST_RX_IP                        0x4A6D0038u
#define DVA_CLK_DET_FAST_RX_IP_FIELDNUM               2u
#define DVA_CLK_DET_FAST_RX_IP_DEFAULTVAL             0x0u
#define DVA_CLK_DET_FAST_RX_IE                        0x4A6D003Cu
#define DVA_CLK_DET_FAST_RX_IE_FIELDNUM               2u
#define DVA_CLK_DET_FAST_RX_IE_DEFAULTVAL             0x0u
#define DVA_CLK_DET_DEAD_RX_IP                        0x4A6D0040u
#define DVA_CLK_DET_DEAD_RX_IP_FIELDNUM               2u
#define DVA_CLK_DET_DEAD_RX_IP_DEFAULTVAL             0x0u
#define DVA_CLK_DET_DEAD_RX_IE                        0x4A6D0044u
#define DVA_CLK_DET_DEAD_RX_IE_FIELDNUM               2u
#define DVA_CLK_DET_DEAD_RX_IE_DEFAULTVAL             0x0u
#define DVA_CLK_DET_SLOW_TX_IP                        0x4A6D0048u
#define DVA_CLK_DET_SLOW_TX_IP_FIELDNUM               2u
#define DVA_CLK_DET_SLOW_TX_IP_DEFAULTVAL             0x0u
#define DVA_CLK_DET_SLOW_TX_IE                        0x4A6D004Cu
#define DVA_CLK_DET_SLOW_TX_IE_FIELDNUM               2u
#define DVA_CLK_DET_SLOW_TX_IE_DEFAULTVAL             0x0u
#define DVA_CLK_DET_FAST_TX_IP                        0x4A6D0050u
#define DVA_CLK_DET_FAST_TX_IP_FIELDNUM               2u
#define DVA_CLK_DET_FAST_TX_IP_DEFAULTVAL             0x0u
#define DVA_CLK_DET_FAST_TX_IE                        0x4A6D0054u
#define DVA_CLK_DET_FAST_TX_IE_FIELDNUM               2u
#define DVA_CLK_DET_FAST_TX_IE_DEFAULTVAL             0x0u
#define DVA_CLK_DET_DEAD_TX_IP                        0x4A6D0058u
#define DVA_CLK_DET_DEAD_TX_IP_FIELDNUM               2u
#define DVA_CLK_DET_DEAD_TX_IP_DEFAULTVAL             0x0u
#define DVA_CLK_DET_DEAD_TX_IE                        0x4A6D005Cu
#define DVA_CLK_DET_DEAD_TX_IE_FIELDNUM               2u
#define DVA_CLK_DET_DEAD_TX_IE_DEFAULTVAL             0x0u
#define DVA_CLK_DET_SYS_IP                            0x4A6D0060u
#define DVA_CLK_DET_SYS_IP_FIELDNUM                   2u
#define DVA_CLK_DET_SYS_IP_DEFAULTVAL                 0x0u
#define DVA_CLK_DET_SYS_IE                            0x4A6D0064u
#define DVA_CLK_DET_SYS_IE_FIELDNUM                   2u
#define DVA_CLK_DET_SYS_IE_DEFAULTVAL                 0x0u
#define DVA_TCAM_ERR_IP                               0x4A6D0068u
#define DVA_TCAM_ERR_IP_FIELDNUM                      2u
#define DVA_TCAM_ERR_IP_DEFAULTVAL                    0x0u
#define DVA_TCAM_ERR_IE                               0x4A6D006Cu
#define DVA_TCAM_ERR_IE_FIELDNUM                      2u
#define DVA_TCAM_ERR_IE_DEFAULTVAL                    0x0u
#define DVA_TCAM_ERR_ACL_IP                           0x4A6D0070u
#define DVA_TCAM_ERR_ACL_IP_FIELDNUM                  2u
#define DVA_TCAM_ERR_ACL_IP_DEFAULTVAL                0x0u
#define DVA_TCAM_ERR_ACL_IE                           0x4A6D0074u
#define DVA_TCAM_ERR_ACL_IE_FIELDNUM                  2u
#define DVA_TCAM_ERR_ACL_IE_DEFAULTVAL                0x0u
#define DVA_TCAM_ERR_CB_IP                            0x4A6D0078u
#define DVA_TCAM_ERR_CB_IP_FIELDNUM                   2u
#define DVA_TCAM_ERR_CB_IP_DEFAULTVAL                 0x0u
#define DVA_TCAM_ERR_CB_IE                            0x4A6D007Cu
#define DVA_TCAM_ERR_CB_IE_FIELDNUM                   2u
#define DVA_TCAM_ERR_CB_IE_DEFAULTVAL                 0x0u
#define DVA_TCAM_ERR_ROUTE_IP                         0x4A6D0080u
#define DVA_TCAM_ERR_ROUTE_IP_FIELDNUM                2u
#define DVA_TCAM_ERR_ROUTE_IP_DEFAULTVAL              0x0u
#define DVA_TCAM_ERR_ROUTE_IE                         0x4A6D0084u
#define DVA_TCAM_ERR_ROUTE_IE_FIELDNUM                2u
#define DVA_TCAM_ERR_ROUTE_IE_DEFAULTVAL              0x0u
#define DVA_PORT0_DISB_MONITOR_CONTROL                0x4AED0000u
#define DVA_PORT0_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT0_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT1_DISB_MONITOR_CONTROL                0x4AED0800u
#define DVA_PORT1_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT1_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT2_DISB_MONITOR_CONTROL                0x4AED1000u
#define DVA_PORT2_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT2_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT3_DISB_MONITOR_CONTROL                0x4AED1800u
#define DVA_PORT3_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT3_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT4_DISB_MONITOR_CONTROL                0x4AED2000u
#define DVA_PORT4_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT4_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT5_DISB_MONITOR_CONTROL                0x4AED2800u
#define DVA_PORT5_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT5_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT6_DISB_MONITOR_CONTROL                0x4AED3000u
#define DVA_PORT6_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT6_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT7_DISB_MONITOR_CONTROL                0x4AED3800u
#define DVA_PORT7_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT7_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT8_DISB_MONITOR_CONTROL                0x4AED4000u
#define DVA_PORT8_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT8_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT9_DISB_MONITOR_CONTROL                0x4AED4800u
#define DVA_PORT9_DISB_MONITOR_CONTROL_FIELDNUM       6u
#define DVA_PORT9_DISB_MONITOR_CONTROL_DEFAULTVAL     0x0u
#define DVA_PORT10_DISB_MONITOR_CONTROL               0x4AED5000u
#define DVA_PORT10_DISB_MONITOR_CONTROL_FIELDNUM      6u
#define DVA_PORT10_DISB_MONITOR_CONTROL_DEFAULTVAL    0x0u
#define DVA_PORT11_DISB_MONITOR_CONTROL               0x4AED5800u
#define DVA_PORT11_DISB_MONITOR_CONTROL_FIELDNUM      6u
#define DVA_PORT11_DISB_MONITOR_CONTROL_DEFAULTVAL    0x0u
#define DVA_PORT12_DISB_MONITOR_CONTROL               0x4AED6000u
#define DVA_PORT12_DISB_MONITOR_CONTROL_FIELDNUM      6u
#define DVA_PORT12_DISB_MONITOR_CONTROL_DEFAULTVAL    0x0u
#define DVA_PORT13_DISB_MONITOR_CONTROL               0x4AED6800u
#define DVA_PORT13_DISB_MONITOR_CONTROL_FIELDNUM      6u
#define DVA_PORT13_DISB_MONITOR_CONTROL_DEFAULTVAL    0x0u
#define DVA_PORT14_DISB_MONITOR_CONTROL               0x4AED7000u
#define DVA_PORT14_DISB_MONITOR_CONTROL_FIELDNUM      6u
#define DVA_PORT14_DISB_MONITOR_CONTROL_DEFAULTVAL    0x0u
#define DVA_PORT15_DISB_MONITOR_CONTROL               0x4AED7800u
#define DVA_PORT15_DISB_MONITOR_CONTROL_FIELDNUM      6u
#define DVA_PORT15_DISB_MONITOR_CONTROL_DEFAULTVAL    0x0u
#define DVA_PORT0_FUSA_PG_LOCK_DMY_SAFETY             0x4AED0004u
#define DVA_PORT0_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT0_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT1_FUSA_PG_LOCK_DMY_SAFETY             0x4AED0804u
#define DVA_PORT1_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT1_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT2_FUSA_PG_LOCK_DMY_SAFETY             0x4AED1004u
#define DVA_PORT2_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT2_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT3_FUSA_PG_LOCK_DMY_SAFETY             0x4AED1804u
#define DVA_PORT3_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT3_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT4_FUSA_PG_LOCK_DMY_SAFETY             0x4AED2004u
#define DVA_PORT4_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT4_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT5_FUSA_PG_LOCK_DMY_SAFETY             0x4AED2804u
#define DVA_PORT5_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT5_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT6_FUSA_PG_LOCK_DMY_SAFETY             0x4AED3004u
#define DVA_PORT6_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT6_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT7_FUSA_PG_LOCK_DMY_SAFETY             0x4AED3804u
#define DVA_PORT7_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT7_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT8_FUSA_PG_LOCK_DMY_SAFETY             0x4AED4004u
#define DVA_PORT8_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT8_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT9_FUSA_PG_LOCK_DMY_SAFETY             0x4AED4804u
#define DVA_PORT9_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM    2u
#define DVA_PORT9_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL  0x0u
#define DVA_PORT10_FUSA_PG_LOCK_DMY_SAFETY            0x4AED5004u
#define DVA_PORT10_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM   2u
#define DVA_PORT10_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL 0x0u
#define DVA_PORT11_FUSA_PG_LOCK_DMY_SAFETY            0x4AED5804u
#define DVA_PORT11_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM   2u
#define DVA_PORT11_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL 0x0u
#define DVA_PORT12_FUSA_PG_LOCK_DMY_SAFETY            0x4AED6004u
#define DVA_PORT12_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM   2u
#define DVA_PORT12_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL 0x0u
#define DVA_PORT13_FUSA_PG_LOCK_DMY_SAFETY            0x4AED6804u
#define DVA_PORT13_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM   2u
#define DVA_PORT13_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL 0x0u
#define DVA_PORT14_FUSA_PG_LOCK_DMY_SAFETY            0x4AED7004u
#define DVA_PORT14_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM   2u
#define DVA_PORT14_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL 0x0u
#define DVA_PORT15_FUSA_PG_LOCK_DMY_SAFETY            0x4AED7804u
#define DVA_PORT15_FUSA_PG_LOCK_DMY_SAFETY_FIELDNUM   2u
#define DVA_PORT15_FUSA_PG_LOCK_DMY_SAFETY_DEFAULTVAL 0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_SAFETY_H_ */