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

#ifndef DVA_RTKAS_REG_LIST_FILE_RLDP_H_
#define DVA_RTKAS_REG_LIST_FILE_RLDP_H_

#ifdef DVA_FILE_RLDP
/* File: RLDP */
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL0                 0x4A610000u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL0_FIELDNUM        1u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL0_DEFAULTVAL      0x0u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL1                 0x4A610004u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL1_FIELDNUM        2u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL1_DEFAULTVAL      0x0u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL2                 0x4A610008u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL2_FIELDNUM        1u
#define DVA_RTKPP_RANDOM_NUMBER_CONTROL2_DEFAULTVAL      0x0u
#define DVA_RLDP_LOOP_STATE_RETRY_CONTROL                0x4A61000Cu
#define DVA_RLDP_LOOP_STATE_RETRY_CONTROL_FIELDNUM       3u
#define DVA_RLDP_LOOP_STATE_RETRY_CONTROL_DEFAULTVAL     0xFF03E8u
#define DVA_RLDP_CHECK_STATE_RETRY_CONTROL               0x4A610010u
#define DVA_RLDP_CHECK_STATE_RETRY_CONTROL_FIELDNUM      3u
#define DVA_RLDP_CHECK_STATE_RETRY_CONTROL_DEFAULTVAL    0xFFEA60u
#define DVA_RLDP_ENABLE_PORT_MASK_CONTROL                0x4A610014u
#define DVA_RLDP_ENABLE_PORT_MASK_CONTROL_FIELDNUM       2u
#define DVA_RLDP_ENABLE_PORT_MASK_CONTROL_DEFAULTVAL     0x0u
#define DVA_RLDP_LOOP_PORT_MASK_CONTROL0                 0x4A610018u
#define DVA_RLDP_LOOP_PORT_MASK_CONTROL0_FIELDNUM        4u
#define DVA_RLDP_LOOP_PORT_MASK_CONTROL0_DEFAULTVAL      0x0u
#define DVA_RLDP_LOOP_PORT_MASK_CONTROL1                 0x4A61001Cu
#define DVA_RLDP_LOOP_PORT_MASK_CONTROL1_FIELDNUM        4u
#define DVA_RLDP_LOOP_PORT_MASK_CONTROL1_DEFAULTVAL      0x0u
#define DVA_RLDP_GLOBAL_CONTROL                          0x4A610020u
#define DVA_RLDP_GLOBAL_CONTROL_FIELDNUM                 8u
#define DVA_RLDP_GLOBAL_CONTROL_DEFAULTVAL               0x100000u
#define DVA_TX_DISABLE_GLOBAL_CONTROL                    0x4A610024u
#define DVA_TX_DISABLE_GLOBAL_CONTROL_FIELDNUM           2u
#define DVA_TX_DISABLE_GLOBAL_CONTROL_DEFAULTVAL         0x0u
#define DVA_RX_DISABLE_GLOBAL_CONTROL                    0x4A610028u
#define DVA_RX_DISABLE_GLOBAL_CONTROL_FIELDNUM           2u
#define DVA_RX_DISABLE_GLOBAL_CONTROL_DEFAULTVAL         0x0u
#define DVA_PORT0_LOOP_PORT                              0x4A61002Cu
#define DVA_PORT0_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT0_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT1_LOOP_PORT                              0x4A610030u
#define DVA_PORT1_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT1_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT2_LOOP_PORT                              0x4A610034u
#define DVA_PORT2_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT2_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT3_LOOP_PORT                              0x4A610038u
#define DVA_PORT3_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT3_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT4_LOOP_PORT                              0x4A61003Cu
#define DVA_PORT4_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT4_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT5_LOOP_PORT                              0x4A610040u
#define DVA_PORT5_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT5_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT6_LOOP_PORT                              0x4A610044u
#define DVA_PORT6_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT6_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT7_LOOP_PORT                              0x4A610048u
#define DVA_PORT7_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT7_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT8_LOOP_PORT                              0x4A61004Cu
#define DVA_PORT8_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT8_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT9_LOOP_PORT                              0x4A610050u
#define DVA_PORT9_LOOP_PORT_FIELDNUM                     2u
#define DVA_PORT9_LOOP_PORT_DEFAULTVAL                   0x8u
#define DVA_PORT10_LOOP_PORT                             0x4A610054u
#define DVA_PORT10_LOOP_PORT_FIELDNUM                    2u
#define DVA_PORT10_LOOP_PORT_DEFAULTVAL                  0x8u
#define DVA_PORT11_LOOP_PORT                             0x4A610058u
#define DVA_PORT11_LOOP_PORT_FIELDNUM                    2u
#define DVA_PORT11_LOOP_PORT_DEFAULTVAL                  0x8u
#define DVA_PORT12_LOOP_PORT                             0x4A61005Cu
#define DVA_PORT12_LOOP_PORT_FIELDNUM                    2u
#define DVA_PORT12_LOOP_PORT_DEFAULTVAL                  0x8u
#define DVA_PORT13_LOOP_PORT                             0x4A610060u
#define DVA_PORT13_LOOP_PORT_FIELDNUM                    2u
#define DVA_PORT13_LOOP_PORT_DEFAULTVAL                  0x8u
#define DVA_PORT14_LOOP_PORT                             0x4A610064u
#define DVA_PORT14_LOOP_PORT_FIELDNUM                    2u
#define DVA_PORT14_LOOP_PORT_DEFAULTVAL                  0x8u
#define DVA_SWITCH_OWN_MAC_CONTROL0                      0x4A610068u
#define DVA_SWITCH_OWN_MAC_CONTROL0_FIELDNUM             1u
#define DVA_SWITCH_OWN_MAC_CONTROL0_DEFAULTVAL           0xE04C00u
#define DVA_SWITCH_OWN_MAC_CONTROL1                      0x4A61006Cu
#define DVA_SWITCH_OWN_MAC_CONTROL1_FIELDNUM             4u
#define DVA_SWITCH_OWN_MAC_CONTROL1_DEFAULTVAL           0x0u
#define DVA_SWITCH_OWN_MAC_CONTROL2                      0x4A610070u
#define DVA_SWITCH_OWN_MAC_CONTROL2_FIELDNUM             2u
#define DVA_SWITCH_OWN_MAC_CONTROL2_DEFAULTVAL           0x8000u
#define DVA_EXTRA_PORT_DEBUG_CONTROL                     0x4A610074u
#define DVA_EXTRA_PORT_DEBUG_CONTROL_FIELDNUM            3u
#define DVA_EXTRA_PORT_DEBUG_CONTROL_DEFAULTVAL          0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL0             0x4A610078u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL0_FIELDNUM    7u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL0_DEFAULTVAL  0x4000u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL1             0x4A61007Cu
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL1_FIELDNUM    1u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL1_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL2             0x4A610080u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL2_FIELDNUM    2u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL2_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL3             0x4A610084u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL3_FIELDNUM    1u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL3_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL4             0x4A610088u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL4_FIELDNUM    2u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL4_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL5             0x4A61008Cu
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL5_FIELDNUM    1u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL5_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL6             0x4A610090u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL6_FIELDNUM    1u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL6_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL7             0x4A610094u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL7_FIELDNUM    1u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL7_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL8             0x4A610098u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL8_FIELDNUM    1u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL8_DEFAULTVAL  0x0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL9             0x4A61009Cu
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL9_FIELDNUM    4u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL9_DEFAULTVAL  0x100u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL10            0x4A6100A0u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL10_FIELDNUM   4u
#define DVA_SIMPLE_PACKET_GENERATOR_CONTROL10_DEFAULTVAL 0x3200300u
#define DVA_FUSA_PG_LOCK_DMY_RLDP                        0x4A6100A4u
#define DVA_FUSA_PG_LOCK_DMY_RLDP_FIELDNUM               2u
#define DVA_FUSA_PG_LOCK_DMY_RLDP_DEFAULTVAL             0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_RLDP_H_ */