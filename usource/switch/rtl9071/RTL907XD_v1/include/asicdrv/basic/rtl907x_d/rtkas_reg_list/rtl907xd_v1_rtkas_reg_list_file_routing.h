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

#ifndef DV1_RTKAS_REG_LIST_FILE_ROUTING_H_
#define DV1_RTKAS_REG_LIST_FILE_ROUTING_H_

#ifdef DV1_FILE_ROUTING
/* File: Routing */
#define DV1_L3_IPUC_ROUTE_CTRL                   0x4A790000u
#define DV1_L3_IPUC_ROUTE_CTRL_FIELDNUM          15u
#define DV1_L3_IPUC_ROUTE_CTRL_DEFAULTVAL        0x9000u
#define DV1_L3_IP6UC_ROUTE_CTRL                  0x4A790004u
#define DV1_L3_IP6UC_ROUTE_CTRL_FIELDNUM         16u
#define DV1_L3_IP6UC_ROUTE_CTRL_DEFAULTVAL       0x60000u
#define DV1_L3_IPMC_ROUTE_CTRL                   0x4A790008u
#define DV1_L3_IPMC_ROUTE_CTRL_FIELDNUM          11u
#define DV1_L3_IPMC_ROUTE_CTRL_DEFAULTVAL        0x0u
#define DV1_L3_IP6MC_ROUTE_CTRL                  0x4A79000Cu
#define DV1_L3_IP6MC_ROUTE_CTRL_FIELDNUM         13u
#define DV1_L3_IP6MC_ROUTE_CTRL_DEFAULTVAL       0x0u
#define DV1_L3_HOST_TBL_CTRL                     0x4A790010u
#define DV1_L3_HOST_TBL_CTRL_FIELDNUM            3u
#define DV1_L3_HOST_TBL_CTRL_DEFAULTVAL          0x2u
#define DV1_L3_PORT_IP_ROUTE_CTRL                0x4A790014u
#define DV1_L3_PORT_IP_ROUTE_CTRL_FIELDNUM       2u
#define DV1_L3_PORT_IP_ROUTE_CTRL_DEFAULTVAL     0x0u
#define DV1_L3_PORT_IP6_ROUTE_CTRL               0x4A790018u
#define DV1_L3_PORT_IP6_ROUTE_CTRL_FIELDNUM      2u
#define DV1_L3_PORT_IP6_ROUTE_CTRL_DEFAULTVAL    0x0u
#define DV1_L3_TRAP_CPU_DPM                      0x4A79001Cu
#define DV1_L3_TRAP_CPU_DPM_FIELDNUM             2u
#define DV1_L3_TRAP_CPU_DPM_DEFAULTVAL           0x8000u
#define DV1_L3_COPY_CPU_DPM                      0x4A790020u
#define DV1_L3_COPY_CPU_DPM_FIELDNUM             2u
#define DV1_L3_COPY_CPU_DPM_DEFAULTVAL           0x8000u
#define DV1_ROUTING_RSV_0                        0x4A790024u
#define DV1_ROUTING_RSV_0_FIELDNUM               1u
#define DV1_ROUTING_RSV_0_DEFAULTVAL             0x0u
#define DV1_ROUTING_RSV_1                        0x4A790028u
#define DV1_ROUTING_RSV_1_FIELDNUM               1u
#define DV1_ROUTING_RSV_1_DEFAULTVAL             0x0u
#define DV1_ROUTING_RSV_2                        0x4A79002Cu
#define DV1_ROUTING_RSV_2_FIELDNUM               1u
#define DV1_ROUTING_RSV_2_DEFAULTVAL             0x0u
#define DV1_ROUTING_RSV_3                        0x4A790030u
#define DV1_ROUTING_RSV_3_FIELDNUM               1u
#define DV1_ROUTING_RSV_3_DEFAULTVAL             0x0u
#define DV1_ROUTING_RSV_4                        0x4A790034u
#define DV1_ROUTING_RSV_4_FIELDNUM               1u
#define DV1_ROUTING_RSV_4_DEFAULTVAL             0x0u
#define DV1_ROUTING_RSV_5                        0x4A790038u
#define DV1_ROUTING_RSV_5_FIELDNUM               1u
#define DV1_ROUTING_RSV_5_DEFAULTVAL             0x0u
#define DV1_ROUTING_RSV_6                        0x4A79003Cu
#define DV1_ROUTING_RSV_6_FIELDNUM               1u
#define DV1_ROUTING_RSV_6_DEFAULTVAL             0x0u
#define DV1_ROUTING_RSV_7                        0x4A790040u
#define DV1_ROUTING_RSV_7_FIELDNUM               1u
#define DV1_ROUTING_RSV_7_DEFAULTVAL             0x0u
#define DV1_L3_GLOBAL_CONTROL0                   0x4A790044u
#define DV1_L3_GLOBAL_CONTROL0_FIELDNUM          5u
#define DV1_L3_GLOBAL_CONTROL0_DEFAULTVAL        0x3u
#define DV1_L3_STATUS                            0x4A790048u
#define DV1_L3_STATUS_FIELDNUM                   11u
#define DV1_L3_STATUS_DEFAULTVAL                 0x0u
#define DV1_L3_UCAST_PACKET_CNT0                 0x4A79004Cu
#define DV1_L3_UCAST_PACKET_CNT0_FIELDNUM        1u
#define DV1_L3_UCAST_PACKET_CNT0_DEFAULTVAL      0x0u
#define DV1_L3_UCAST_PACKET_CNT1                 0x4A790050u
#define DV1_L3_UCAST_PACKET_CNT1_FIELDNUM        1u
#define DV1_L3_UCAST_PACKET_CNT1_DEFAULTVAL      0x0u
#define DV1_L3_MBCAST_PACKET_CNT0                0x4A790054u
#define DV1_L3_MBCAST_PACKET_CNT0_FIELDNUM       1u
#define DV1_L3_MBCAST_PACKET_CNT0_DEFAULTVAL     0x0u
#define DV1_L3_MBCAST_PACKET_CNT1                0x4A790058u
#define DV1_L3_MBCAST_PACKET_CNT1_FIELDNUM       1u
#define DV1_L3_MBCAST_PACKET_CNT1_DEFAULTVAL     0x0u
#define DV1_L3_ALL_BYTE_CNT0                     0x4A79005Cu
#define DV1_L3_ALL_BYTE_CNT0_FIELDNUM            1u
#define DV1_L3_ALL_BYTE_CNT0_DEFAULTVAL          0x0u
#define DV1_L3_ALL_BYTE_CNT1                     0x4A790060u
#define DV1_L3_ALL_BYTE_CNT1_FIELDNUM            1u
#define DV1_L3_ALL_BYTE_CNT1_DEFAULTVAL          0x0u
#define DV1_L3_TABLE_CLEAR_CTRL                  0x4A790064u
#define DV1_L3_TABLE_CLEAR_CTRL_FIELDNUM         10u
#define DV1_L3_TABLE_CLEAR_CTRL_DEFAULTVAL       0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL0             0x4A790068u
#define DV1_L3_TABLE_SEARCH_CONTROL0_FIELDNUM    4u
#define DV1_L3_TABLE_SEARCH_CONTROL0_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL1             0x4A79006Cu
#define DV1_L3_TABLE_SEARCH_CONTROL1_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL1_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL2             0x4A790070u
#define DV1_L3_TABLE_SEARCH_CONTROL2_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL2_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL3             0x4A790074u
#define DV1_L3_TABLE_SEARCH_CONTROL3_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL3_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL4             0x4A790078u
#define DV1_L3_TABLE_SEARCH_CONTROL4_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL4_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL5             0x4A79007Cu
#define DV1_L3_TABLE_SEARCH_CONTROL5_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL5_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL6             0x4A790080u
#define DV1_L3_TABLE_SEARCH_CONTROL6_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL6_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL7             0x4A790084u
#define DV1_L3_TABLE_SEARCH_CONTROL7_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL7_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL8             0x4A790088u
#define DV1_L3_TABLE_SEARCH_CONTROL8_FIELDNUM    1u
#define DV1_L3_TABLE_SEARCH_CONTROL8_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL9             0x4A79008Cu
#define DV1_L3_TABLE_SEARCH_CONTROL9_FIELDNUM    2u
#define DV1_L3_TABLE_SEARCH_CONTROL9_DEFAULTVAL  0x0u
#define DV1_L3_TABLE_SEARCH_CONTROL10            0x4A790090u
#define DV1_L3_TABLE_SEARCH_CONTROL10_FIELDNUM   2u
#define DV1_L3_TABLE_SEARCH_CONTROL10_DEFAULTVAL 0x0u
#define DV1_L3_TABLE_SEARCH_STATUS0              0x4A790094u
#define DV1_L3_TABLE_SEARCH_STATUS0_FIELDNUM     4u
#define DV1_L3_TABLE_SEARCH_STATUS0_DEFAULTVAL   0x0u
#define DV1_L3_TABLE_SEARCH_STATUS1              0x4A790098u
#define DV1_L3_TABLE_SEARCH_STATUS1_FIELDNUM     3u
#define DV1_L3_TABLE_SEARCH_STATUS1_DEFAULTVAL   0x0u
#define DV1_L3_TCAM_ERROR                        0x4A79009Cu
#define DV1_L3_TCAM_ERROR_FIELDNUM               3u
#define DV1_L3_TCAM_ERROR_DEFAULTVAL             0x0u
#define DV1_L3NH_L2DA_CONTROL                    0x4A7900A0u
#define DV1_L3NH_L2DA_CONTROL_FIELDNUM           4u
#define DV1_L3NH_L2DA_CONTROL_DEFAULTVAL         0x0u
#define DV1_L3_TRAP_QOS0                         0x4A7900A4u
#define DV1_L3_TRAP_QOS0_FIELDNUM                16u
#define DV1_L3_TRAP_QOS0_DEFAULTVAL              0x0u
#define DV1_L3_TRAP_QOS1                         0x4A7900A8u
#define DV1_L3_TRAP_QOS1_FIELDNUM                15u
#define DV1_L3_TRAP_QOS1_DEFAULTVAL              0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_ROUTING_H_ */