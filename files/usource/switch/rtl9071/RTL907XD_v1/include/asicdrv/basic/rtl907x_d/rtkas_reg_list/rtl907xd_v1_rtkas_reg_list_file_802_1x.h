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

#ifndef DV1_RTKAS_REG_LIST_FILE_802_1X_H_
#define DV1_RTKAS_REG_LIST_FILE_802_1X_H_

#ifdef DV1_FILE_802_1X
/* File: Dot1x */
#define DV1_GUEST_VLAN_GLOBAL_CONTROL0                      0x4A740000u
#define DV1_GUEST_VLAN_GLOBAL_CONTROL0_FIELDNUM             6u
#define DV1_GUEST_VLAN_GLOBAL_CONTROL0_DEFAULTVAL           0x100u
#define DV1_GUEST_VLAN_GLOBAL_CONTROL1                      0x4A740004u
#define DV1_GUEST_VLAN_GLOBAL_CONTROL1_FIELDNUM             2u
#define DV1_GUEST_VLAN_GLOBAL_CONTROL1_DEFAULTVAL           0x1000u
#define DV1_DOT1X_NETWORK_ACCESS_RELATED_CONTROL            0x4A740008u
#define DV1_DOT1X_NETWORK_ACCESS_RELATED_CONTROL_FIELDNUM   3u
#define DV1_DOT1X_NETWORK_ACCESS_RELATED_CONTROL_DEFAULTVAL 0x0u
#define DV1_PORT0_BASED_DOT1X_CONTROL                       0x4AF40000u
#define DV1_PORT0_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT0_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT1_BASED_DOT1X_CONTROL                       0x4AF40800u
#define DV1_PORT1_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT1_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT2_BASED_DOT1X_CONTROL                       0x4AF41000u
#define DV1_PORT2_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT2_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT3_BASED_DOT1X_CONTROL                       0x4AF41800u
#define DV1_PORT3_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT3_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT4_BASED_DOT1X_CONTROL                       0x4AF42000u
#define DV1_PORT4_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT4_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT5_BASED_DOT1X_CONTROL                       0x4AF42800u
#define DV1_PORT5_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT5_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT6_BASED_DOT1X_CONTROL                       0x4AF43000u
#define DV1_PORT6_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT6_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT7_BASED_DOT1X_CONTROL                       0x4AF43800u
#define DV1_PORT7_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT7_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT8_BASED_DOT1X_CONTROL                       0x4AF44000u
#define DV1_PORT8_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT8_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT9_BASED_DOT1X_CONTROL                       0x4AF44800u
#define DV1_PORT9_BASED_DOT1X_CONTROL_FIELDNUM              3u
#define DV1_PORT9_BASED_DOT1X_CONTROL_DEFAULTVAL            0x0u
#define DV1_PORT10_BASED_DOT1X_CONTROL                      0x4AF45000u
#define DV1_PORT10_BASED_DOT1X_CONTROL_FIELDNUM             3u
#define DV1_PORT10_BASED_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT11_BASED_DOT1X_CONTROL                      0x4AF45800u
#define DV1_PORT11_BASED_DOT1X_CONTROL_FIELDNUM             3u
#define DV1_PORT11_BASED_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT12_BASED_DOT1X_CONTROL                      0x4AF46000u
#define DV1_PORT12_BASED_DOT1X_CONTROL_FIELDNUM             3u
#define DV1_PORT12_BASED_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT13_BASED_DOT1X_CONTROL                      0x4AF46800u
#define DV1_PORT13_BASED_DOT1X_CONTROL_FIELDNUM             3u
#define DV1_PORT13_BASED_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT14_BASED_DOT1X_CONTROL                      0x4AF47000u
#define DV1_PORT14_BASED_DOT1X_CONTROL_FIELDNUM             3u
#define DV1_PORT14_BASED_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT15_BASED_DOT1X_CONTROL                      0x4AF47800u
#define DV1_PORT15_BASED_DOT1X_CONTROL_FIELDNUM             3u
#define DV1_PORT15_BASED_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT0_EGRESS_DOT1X_CONTROL                      0x4AF40004u
#define DV1_PORT0_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT0_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT1_EGRESS_DOT1X_CONTROL                      0x4AF40804u
#define DV1_PORT1_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT1_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT2_EGRESS_DOT1X_CONTROL                      0x4AF41004u
#define DV1_PORT2_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT2_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT3_EGRESS_DOT1X_CONTROL                      0x4AF41804u
#define DV1_PORT3_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT3_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT4_EGRESS_DOT1X_CONTROL                      0x4AF42004u
#define DV1_PORT4_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT4_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT5_EGRESS_DOT1X_CONTROL                      0x4AF42804u
#define DV1_PORT5_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT5_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT6_EGRESS_DOT1X_CONTROL                      0x4AF43004u
#define DV1_PORT6_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT6_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT7_EGRESS_DOT1X_CONTROL                      0x4AF43804u
#define DV1_PORT7_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT7_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT8_EGRESS_DOT1X_CONTROL                      0x4AF44004u
#define DV1_PORT8_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT8_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT9_EGRESS_DOT1X_CONTROL                      0x4AF44804u
#define DV1_PORT9_EGRESS_DOT1X_CONTROL_FIELDNUM             2u
#define DV1_PORT9_EGRESS_DOT1X_CONTROL_DEFAULTVAL           0x0u
#define DV1_PORT10_EGRESS_DOT1X_CONTROL                     0x4AF45004u
#define DV1_PORT10_EGRESS_DOT1X_CONTROL_FIELDNUM            2u
#define DV1_PORT10_EGRESS_DOT1X_CONTROL_DEFAULTVAL          0x0u
#define DV1_PORT11_EGRESS_DOT1X_CONTROL                     0x4AF45804u
#define DV1_PORT11_EGRESS_DOT1X_CONTROL_FIELDNUM            2u
#define DV1_PORT11_EGRESS_DOT1X_CONTROL_DEFAULTVAL          0x0u
#define DV1_PORT12_EGRESS_DOT1X_CONTROL                     0x4AF46004u
#define DV1_PORT12_EGRESS_DOT1X_CONTROL_FIELDNUM            2u
#define DV1_PORT12_EGRESS_DOT1X_CONTROL_DEFAULTVAL          0x0u
#define DV1_PORT13_EGRESS_DOT1X_CONTROL                     0x4AF46804u
#define DV1_PORT13_EGRESS_DOT1X_CONTROL_FIELDNUM            2u
#define DV1_PORT13_EGRESS_DOT1X_CONTROL_DEFAULTVAL          0x0u
#define DV1_PORT14_EGRESS_DOT1X_CONTROL                     0x4AF47004u
#define DV1_PORT14_EGRESS_DOT1X_CONTROL_FIELDNUM            2u
#define DV1_PORT14_EGRESS_DOT1X_CONTROL_DEFAULTVAL          0x0u
#define DV1_PORT15_EGRESS_DOT1X_CONTROL                     0x4AF47804u
#define DV1_PORT15_EGRESS_DOT1X_CONTROL_FIELDNUM            2u
#define DV1_PORT15_EGRESS_DOT1X_CONTROL_DEFAULTVAL          0x0u
#define DV1_PORT0_MAC_BASED_DOT1X_CONTROL                   0x4AF40008u
#define DV1_PORT0_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT0_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT1_MAC_BASED_DOT1X_CONTROL                   0x4AF40808u
#define DV1_PORT1_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT1_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT2_MAC_BASED_DOT1X_CONTROL                   0x4AF41008u
#define DV1_PORT2_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT2_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT3_MAC_BASED_DOT1X_CONTROL                   0x4AF41808u
#define DV1_PORT3_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT3_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT4_MAC_BASED_DOT1X_CONTROL                   0x4AF42008u
#define DV1_PORT4_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT4_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT5_MAC_BASED_DOT1X_CONTROL                   0x4AF42808u
#define DV1_PORT5_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT5_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT6_MAC_BASED_DOT1X_CONTROL                   0x4AF43008u
#define DV1_PORT6_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT6_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT7_MAC_BASED_DOT1X_CONTROL                   0x4AF43808u
#define DV1_PORT7_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT7_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT8_MAC_BASED_DOT1X_CONTROL                   0x4AF44008u
#define DV1_PORT8_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT8_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT9_MAC_BASED_DOT1X_CONTROL                   0x4AF44808u
#define DV1_PORT9_MAC_BASED_DOT1X_CONTROL_FIELDNUM          2u
#define DV1_PORT9_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL        0x0u
#define DV1_PORT10_MAC_BASED_DOT1X_CONTROL                  0x4AF45008u
#define DV1_PORT10_MAC_BASED_DOT1X_CONTROL_FIELDNUM         2u
#define DV1_PORT10_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL       0x0u
#define DV1_PORT11_MAC_BASED_DOT1X_CONTROL                  0x4AF45808u
#define DV1_PORT11_MAC_BASED_DOT1X_CONTROL_FIELDNUM         2u
#define DV1_PORT11_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL       0x0u
#define DV1_PORT12_MAC_BASED_DOT1X_CONTROL                  0x4AF46008u
#define DV1_PORT12_MAC_BASED_DOT1X_CONTROL_FIELDNUM         2u
#define DV1_PORT12_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL       0x0u
#define DV1_PORT13_MAC_BASED_DOT1X_CONTROL                  0x4AF46808u
#define DV1_PORT13_MAC_BASED_DOT1X_CONTROL_FIELDNUM         2u
#define DV1_PORT13_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL       0x0u
#define DV1_PORT14_MAC_BASED_DOT1X_CONTROL                  0x4AF47008u
#define DV1_PORT14_MAC_BASED_DOT1X_CONTROL_FIELDNUM         2u
#define DV1_PORT14_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL       0x0u
#define DV1_PORT15_MAC_BASED_DOT1X_CONTROL                  0x4AF47808u
#define DV1_PORT15_MAC_BASED_DOT1X_CONTROL_FIELDNUM         2u
#define DV1_PORT15_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL       0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_802_1X_H_ */