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

#ifndef DV1_RTKAS_REG_LIST_FILE_REMARKING_H_
#define DV1_RTKAS_REG_LIST_FILE_REMARKING_H_

#ifdef DV1_FILE_REMARKING
/* File: Remarking */
#define DV1_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0                    0x4A320000u
#define DV1_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_FIELDNUM           16u
#define DV1_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_DEFAULTVAL         0x76543201u
#define DV1_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL1                    0x4A320004u
#define DV1_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL1_FIELDNUM           16u
#define DV1_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL1_DEFAULTVAL         0x76543201u
#define DV1_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0            0x4A320008u
#define DV1_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_FIELDNUM   16u
#define DV1_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_DEFAULTVAL 0x76543201u
#define DV1_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL1            0x4A32000Cu
#define DV1_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL1_FIELDNUM   16u
#define DV1_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL1_DEFAULTVAL 0x76543201u
#define DV1_DSCP_REMARKING_TABLE_CONTROL0                                  0x4A320010u
#define DV1_DSCP_REMARKING_TABLE_CONTROL0_FIELDNUM                         6u
#define DV1_DSCP_REMARKING_TABLE_CONTROL0_DEFAULTVAL                       0x20610200u
#define DV1_DSCP_REMARKING_TABLE_CONTROL1                                  0x4A320014u
#define DV1_DSCP_REMARKING_TABLE_CONTROL1_FIELDNUM                         4u
#define DV1_DSCP_REMARKING_TABLE_CONTROL1_DEFAULTVAL                       0x38C28u
#define DV1_DSCP_REMARKING_TABLE_CONTROL2                                  0x4A320018u
#define DV1_DSCP_REMARKING_TABLE_CONTROL2_FIELDNUM                         6u
#define DV1_DSCP_REMARKING_TABLE_CONTROL2_DEFAULTVAL                       0x20610200u
#define DV1_DSCP_REMARKING_TABLE_CONTROL3                                  0x4A32001Cu
#define DV1_DSCP_REMARKING_TABLE_CONTROL3_FIELDNUM                         4u
#define DV1_DSCP_REMARKING_TABLE_CONTROL3_DEFAULTVAL                       0x38C28u
#define DV1_PORT0_PRIORITY_REMARKING_CONTROL                               0x4AB20000u
#define DV1_PORT0_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT0_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT1_PRIORITY_REMARKING_CONTROL                               0x4AB20800u
#define DV1_PORT1_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT1_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT2_PRIORITY_REMARKING_CONTROL                               0x4AB21000u
#define DV1_PORT2_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT2_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT3_PRIORITY_REMARKING_CONTROL                               0x4AB21800u
#define DV1_PORT3_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT3_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT4_PRIORITY_REMARKING_CONTROL                               0x4AB22000u
#define DV1_PORT4_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT4_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT5_PRIORITY_REMARKING_CONTROL                               0x4AB22800u
#define DV1_PORT5_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT5_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT6_PRIORITY_REMARKING_CONTROL                               0x4AB23000u
#define DV1_PORT6_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT6_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT7_PRIORITY_REMARKING_CONTROL                               0x4AB23800u
#define DV1_PORT7_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT7_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT8_PRIORITY_REMARKING_CONTROL                               0x4AB24000u
#define DV1_PORT8_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT8_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT9_PRIORITY_REMARKING_CONTROL                               0x4AB24800u
#define DV1_PORT9_PRIORITY_REMARKING_CONTROL_FIELDNUM                      7u
#define DV1_PORT9_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                    0x0u
#define DV1_PORT10_PRIORITY_REMARKING_CONTROL                              0x4AB25000u
#define DV1_PORT10_PRIORITY_REMARKING_CONTROL_FIELDNUM                     7u
#define DV1_PORT10_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                   0x0u
#define DV1_PORT11_PRIORITY_REMARKING_CONTROL                              0x4AB25800u
#define DV1_PORT11_PRIORITY_REMARKING_CONTROL_FIELDNUM                     7u
#define DV1_PORT11_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                   0x0u
#define DV1_PORT12_PRIORITY_REMARKING_CONTROL                              0x4AB26000u
#define DV1_PORT12_PRIORITY_REMARKING_CONTROL_FIELDNUM                     7u
#define DV1_PORT12_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                   0x0u
#define DV1_PORT13_PRIORITY_REMARKING_CONTROL                              0x4AB26800u
#define DV1_PORT13_PRIORITY_REMARKING_CONTROL_FIELDNUM                     7u
#define DV1_PORT13_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                   0x0u
#define DV1_PORT14_PRIORITY_REMARKING_CONTROL                              0x4AB27000u
#define DV1_PORT14_PRIORITY_REMARKING_CONTROL_FIELDNUM                     7u
#define DV1_PORT14_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                   0x0u
#define DV1_PORT15_PRIORITY_REMARKING_CONTROL                              0x4AB27800u
#define DV1_PORT15_PRIORITY_REMARKING_CONTROL_FIELDNUM                     7u
#define DV1_PORT15_PRIORITY_REMARKING_CONTROL_DEFAULTVAL                   0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_REMARKING_H_ */