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

#ifndef DV1_RTKAS_REG_LIST_FILE_RLDP_H_
#define DV1_RTKAS_REG_LIST_FILE_RLDP_H_

#ifdef DV1_FILE_RLDP
/* File: RLDP */
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL0                 0x4A610000u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL0_FIELDNUM        1u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL0_DEFAULTVAL      0x0u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL1                 0x4A610004u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL1_FIELDNUM        2u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL1_DEFAULTVAL      0x0u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL2                 0x4A610008u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL2_FIELDNUM        1u
#define DV1_RTKPP_RANDOM_NUMBER_CONTROL2_DEFAULTVAL      0x0u
#define DV1_RLDP_LOOP_STATE_RETRY_CONTROL                0x4A61000Cu
#define DV1_RLDP_LOOP_STATE_RETRY_CONTROL_FIELDNUM       3u
#define DV1_RLDP_LOOP_STATE_RETRY_CONTROL_DEFAULTVAL     0xFF03E8u
#define DV1_RLDP_CHECK_STATE_RETRY_CONTROL               0x4A610010u
#define DV1_RLDP_CHECK_STATE_RETRY_CONTROL_FIELDNUM      3u
#define DV1_RLDP_CHECK_STATE_RETRY_CONTROL_DEFAULTVAL    0xFFEA60u
#define DV1_RLDP_ENABLE_PORT_MASK_CONTROL                0x4A610014u
#define DV1_RLDP_ENABLE_PORT_MASK_CONTROL_FIELDNUM       2u
#define DV1_RLDP_ENABLE_PORT_MASK_CONTROL_DEFAULTVAL     0x0u
#define DV1_RLDP_LOOP_PORT_MASK_CONTROL0                 0x4A610018u
#define DV1_RLDP_LOOP_PORT_MASK_CONTROL0_FIELDNUM        4u
#define DV1_RLDP_LOOP_PORT_MASK_CONTROL0_DEFAULTVAL      0x0u
#define DV1_RLDP_LOOP_PORT_MASK_CONTROL1                 0x4A61001Cu
#define DV1_RLDP_LOOP_PORT_MASK_CONTROL1_FIELDNUM        4u
#define DV1_RLDP_LOOP_PORT_MASK_CONTROL1_DEFAULTVAL      0x0u
#define DV1_RLDP_GLOBAL_CONTROL                          0x4A610020u
#define DV1_RLDP_GLOBAL_CONTROL_FIELDNUM                 8u
#define DV1_RLDP_GLOBAL_CONTROL_DEFAULTVAL               0x100000u
#define DV1_TX_DISABLE_GLOBAL_CONTROL                    0x4A610024u
#define DV1_TX_DISABLE_GLOBAL_CONTROL_FIELDNUM           2u
#define DV1_TX_DISABLE_GLOBAL_CONTROL_DEFAULTVAL         0x0u
#define DV1_RX_DISABLE_GLOBAL_CONTROL                    0x4A610028u
#define DV1_RX_DISABLE_GLOBAL_CONTROL_FIELDNUM           2u
#define DV1_RX_DISABLE_GLOBAL_CONTROL_DEFAULTVAL         0x0u
#define DV1_PORT0_LOOP_PORT                              0x4A61002Cu
#define DV1_PORT0_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT0_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT1_LOOP_PORT                              0x4A610030u
#define DV1_PORT1_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT1_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT2_LOOP_PORT                              0x4A610034u
#define DV1_PORT2_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT2_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT3_LOOP_PORT                              0x4A610038u
#define DV1_PORT3_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT3_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT4_LOOP_PORT                              0x4A61003Cu
#define DV1_PORT4_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT4_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT5_LOOP_PORT                              0x4A610040u
#define DV1_PORT5_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT5_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT6_LOOP_PORT                              0x4A610044u
#define DV1_PORT6_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT6_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT7_LOOP_PORT                              0x4A610048u
#define DV1_PORT7_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT7_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT8_LOOP_PORT                              0x4A61004Cu
#define DV1_PORT8_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT8_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT9_LOOP_PORT                              0x4A610050u
#define DV1_PORT9_LOOP_PORT_FIELDNUM                     2u
#define DV1_PORT9_LOOP_PORT_DEFAULTVAL                   0x8u
#define DV1_PORT10_LOOP_PORT                             0x4A610054u
#define DV1_PORT10_LOOP_PORT_FIELDNUM                    2u
#define DV1_PORT10_LOOP_PORT_DEFAULTVAL                  0x8u
#define DV1_PORT11_LOOP_PORT                             0x4A610058u
#define DV1_PORT11_LOOP_PORT_FIELDNUM                    2u
#define DV1_PORT11_LOOP_PORT_DEFAULTVAL                  0x8u
#define DV1_PORT12_LOOP_PORT                             0x4A61005Cu
#define DV1_PORT12_LOOP_PORT_FIELDNUM                    2u
#define DV1_PORT12_LOOP_PORT_DEFAULTVAL                  0x8u
#define DV1_PORT13_LOOP_PORT                             0x4A610060u
#define DV1_PORT13_LOOP_PORT_FIELDNUM                    2u
#define DV1_PORT13_LOOP_PORT_DEFAULTVAL                  0x8u
#define DV1_PORT14_LOOP_PORT                             0x4A610064u
#define DV1_PORT14_LOOP_PORT_FIELDNUM                    2u
#define DV1_PORT14_LOOP_PORT_DEFAULTVAL                  0x8u
#define DV1_SWITCH_OWN_MAC_CONTROL0                      0x4A610068u
#define DV1_SWITCH_OWN_MAC_CONTROL0_FIELDNUM             1u
#define DV1_SWITCH_OWN_MAC_CONTROL0_DEFAULTVAL           0xE04C00u
#define DV1_SWITCH_OWN_MAC_CONTROL1                      0x4A61006Cu
#define DV1_SWITCH_OWN_MAC_CONTROL1_FIELDNUM             4u
#define DV1_SWITCH_OWN_MAC_CONTROL1_DEFAULTVAL           0x0u
#define DV1_SWITCH_OWN_MAC_CONTROL2                      0x4A610070u
#define DV1_SWITCH_OWN_MAC_CONTROL2_FIELDNUM             2u
#define DV1_SWITCH_OWN_MAC_CONTROL2_DEFAULTVAL           0x8000u
#define DV1_EXTRA_PORT_DEBUG_CONTROL                     0x4A610074u
#define DV1_EXTRA_PORT_DEBUG_CONTROL_FIELDNUM            3u
#define DV1_EXTRA_PORT_DEBUG_CONTROL_DEFAULTVAL          0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL0             0x4A610078u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL0_FIELDNUM    7u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL0_DEFAULTVAL  0x4000u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL1             0x4A61007Cu
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL1_FIELDNUM    1u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL1_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL2             0x4A610080u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL2_FIELDNUM    2u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL2_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL3             0x4A610084u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL3_FIELDNUM    1u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL3_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL4             0x4A610088u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL4_FIELDNUM    2u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL4_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL5             0x4A61008Cu
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL5_FIELDNUM    1u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL5_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL6             0x4A610090u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL6_FIELDNUM    1u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL6_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL7             0x4A610094u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL7_FIELDNUM    1u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL7_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL8             0x4A610098u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL8_FIELDNUM    1u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL8_DEFAULTVAL  0x0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL9             0x4A61009Cu
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL9_FIELDNUM    4u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL9_DEFAULTVAL  0x100u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL10            0x4A6100A0u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL10_FIELDNUM   4u
#define DV1_SIMPLE_PACKET_GENERATOR_CONTROL10_DEFAULTVAL 0x1900180u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_RLDP_H_ */