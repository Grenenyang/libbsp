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

#ifndef DV1_RTKAS_REG_LIST_FILE_MIRRORING_H_
#define DV1_RTKAS_REG_LIST_FILE_MIRRORING_H_

#ifdef DV1_FILE_MIRRORING
/* File: Mirroring */
#define DV1_TRAFFIC_MIRROR0_CONTROL0                      0x4A370000u
#define DV1_TRAFFIC_MIRROR0_CONTROL0_FIELDNUM             8u
#define DV1_TRAFFIC_MIRROR0_CONTROL0_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR0_CONTROL1                      0x4A370004u
#define DV1_TRAFFIC_MIRROR0_CONTROL1_FIELDNUM             4u
#define DV1_TRAFFIC_MIRROR0_CONTROL1_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR0_CONTROL2                      0x4A370008u
#define DV1_TRAFFIC_MIRROR0_CONTROL2_FIELDNUM             2u
#define DV1_TRAFFIC_MIRROR0_CONTROL2_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR0_CONTROL3                      0x4A37000Cu
#define DV1_TRAFFIC_MIRROR0_CONTROL3_FIELDNUM             2u
#define DV1_TRAFFIC_MIRROR0_CONTROL3_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR0_CONTROL4                      0x4A370010u
#define DV1_TRAFFIC_MIRROR0_CONTROL4_FIELDNUM             2u
#define DV1_TRAFFIC_MIRROR0_CONTROL4_DEFAULTVAL           0x0u
#define DV1_MIRROR0_PORT_EGRESS_FILTER_CONTROL            0x4A370014u
#define DV1_MIRROR0_PORT_EGRESS_FILTER_CONTROL_FIELDNUM   2u
#define DV1_MIRROR0_PORT_EGRESS_FILTER_CONTROL_DEFAULTVAL 0x0u
#define DV1_TRAFFIC_MIRROR0_SAMPLE_CONTROL                0x4A370018u
#define DV1_TRAFFIC_MIRROR0_SAMPLE_CONTROL_FIELDNUM       2u
#define DV1_TRAFFIC_MIRROR0_SAMPLE_CONTROL_DEFAULTVAL     0x10000u
#define DV1_TRAFFIC_MIRROR0_SAMPLE_STATISTIC              0x4A37001Cu
#define DV1_TRAFFIC_MIRROR0_SAMPLE_STATISTIC_FIELDNUM     1u
#define DV1_TRAFFIC_MIRROR0_SAMPLE_STATISTIC_DEFAULTVAL   0x0u
#define DV1_REMOTE_MIRROR0_TX0                            0x4A370020u
#define DV1_REMOTE_MIRROR0_TX0_FIELDNUM                   2u
#define DV1_REMOTE_MIRROR0_TX0_DEFAULTVAL                 0x0u
#define DV1_REMOTE_MIRROR0_TX1                            0x4A370024u
#define DV1_REMOTE_MIRROR0_TX1_FIELDNUM                   4u
#define DV1_REMOTE_MIRROR0_TX1_DEFAULTVAL                 0x81000000u
#define DV1_REMOTE_MIRROR0_RX                             0x4A370028u
#define DV1_REMOTE_MIRROR0_RX_FIELDNUM                    4u
#define DV1_REMOTE_MIRROR0_RX_DEFAULTVAL                  0x0u
#define DV1_TRAFFIC_MIRROR1_CONTROL0                      0x4A37002Cu
#define DV1_TRAFFIC_MIRROR1_CONTROL0_FIELDNUM             8u
#define DV1_TRAFFIC_MIRROR1_CONTROL0_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR1_CONTROL1                      0x4A370030u
#define DV1_TRAFFIC_MIRROR1_CONTROL1_FIELDNUM             4u
#define DV1_TRAFFIC_MIRROR1_CONTROL1_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR1_CONTROL2                      0x4A370034u
#define DV1_TRAFFIC_MIRROR1_CONTROL2_FIELDNUM             2u
#define DV1_TRAFFIC_MIRROR1_CONTROL2_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR1_CONTROL3                      0x4A370038u
#define DV1_TRAFFIC_MIRROR1_CONTROL3_FIELDNUM             2u
#define DV1_TRAFFIC_MIRROR1_CONTROL3_DEFAULTVAL           0x0u
#define DV1_TRAFFIC_MIRROR1_CONTROL4                      0x4A37003Cu
#define DV1_TRAFFIC_MIRROR1_CONTROL4_FIELDNUM             2u
#define DV1_TRAFFIC_MIRROR1_CONTROL4_DEFAULTVAL           0x0u
#define DV1_MIRROR1_PORT_EGRESS_FILTER_CONTROL            0x4A370040u
#define DV1_MIRROR1_PORT_EGRESS_FILTER_CONTROL_FIELDNUM   2u
#define DV1_MIRROR1_PORT_EGRESS_FILTER_CONTROL_DEFAULTVAL 0x0u
#define DV1_TRAFFIC_MIRROR1_SAMPLE_CONTROL                0x4A370044u
#define DV1_TRAFFIC_MIRROR1_SAMPLE_CONTROL_FIELDNUM       2u
#define DV1_TRAFFIC_MIRROR1_SAMPLE_CONTROL_DEFAULTVAL     0x10000u
#define DV1_TRAFFIC_MIRROR1_SAMPLE_STATISTIC              0x4A370048u
#define DV1_TRAFFIC_MIRROR1_SAMPLE_STATISTIC_FIELDNUM     1u
#define DV1_TRAFFIC_MIRROR1_SAMPLE_STATISTIC_DEFAULTVAL   0x0u
#define DV1_REMOTE_MIRROR1_TX0                            0x4A37004Cu
#define DV1_REMOTE_MIRROR1_TX0_FIELDNUM                   2u
#define DV1_REMOTE_MIRROR1_TX0_DEFAULTVAL                 0x0u
#define DV1_REMOTE_MIRROR1_TX1                            0x4A370050u
#define DV1_REMOTE_MIRROR1_TX1_FIELDNUM                   4u
#define DV1_REMOTE_MIRROR1_TX1_DEFAULTVAL                 0x81000000u
#define DV1_REMOTE_MIRROR1_RX                             0x4A370054u
#define DV1_REMOTE_MIRROR1_RX_FIELDNUM                    4u
#define DV1_REMOTE_MIRROR1_RX_DEFAULTVAL                  0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_MIRRORING_H_ */