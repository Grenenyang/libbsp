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

#ifndef DV1_RTKAS_REG_LIST_FILE_SAFETY_H_
#define DV1_RTKAS_REG_LIST_FILE_SAFETY_H_

#ifdef DV1_FILE_SAFETY
/* File: Safety */
#define DV1_SAFE_IRPT_IP_SPF_0                0x4A6D0000u
#define DV1_SAFE_IRPT_IP_SPF_0_FIELDNUM       2u
#define DV1_SAFE_IRPT_IP_SPF_0_DEFAULTVAL     0x0u
#define DV1_SAFE_IRPT_IP_LF_0                 0x4A6D0004u
#define DV1_SAFE_IRPT_IP_LF_0_FIELDNUM        2u
#define DV1_SAFE_IRPT_IP_LF_0_DEFAULTVAL      0x0u
#define DV1_SAFE_IRPT_IE_SPF_0                0x4A6D0008u
#define DV1_SAFE_IRPT_IE_SPF_0_FIELDNUM       2u
#define DV1_SAFE_IRPT_IE_SPF_0_DEFAULTVAL     0x0u
#define DV1_SAFE_IRPT_IE_LF_0                 0x4A6D000Cu
#define DV1_SAFE_IRPT_IE_LF_0_FIELDNUM        2u
#define DV1_SAFE_IRPT_IE_LF_0_DEFAULTVAL      0x0u
#define DV1_DISB_SAFETY_REG_0                 0x4A6D0010u
#define DV1_DISB_SAFETY_REG_0_FIELDNUM        4u
#define DV1_DISB_SAFETY_REG_0_DEFAULTVAL      0x8u
#define DV1_DISB_SAFETY_REG_1                 0x4A6D0014u
#define DV1_DISB_SAFETY_REG_1_FIELDNUM        1u
#define DV1_DISB_SAFETY_REG_1_DEFAULTVAL      0x0u
#define DV1_MAC_TXRX_SAFETY_REG_TX            0x4A6D0018u
#define DV1_MAC_TXRX_SAFETY_REG_TX_FIELDNUM   6u
#define DV1_MAC_TXRX_SAFETY_REG_TX_DEFAULTVAL 0xFF0000u
#define DV1_MAC_TXRX_SAFETY_REG_RX            0x4A6D001Cu
#define DV1_MAC_TXRX_SAFETY_REG_RX_FIELDNUM   6u
#define DV1_MAC_TXRX_SAFETY_REG_RX_DEFAULTVAL 0xFF0000u
#define DV1_SAFE_IRPT_IP_MIB                  0x4A6D0020u
#define DV1_SAFE_IRPT_IP_MIB_FIELDNUM         2u
#define DV1_SAFE_IRPT_IP_MIB_DEFAULTVAL       0x0u
#define DV1_SAFE_IRPT_IE_MIB                  0x4A6D0024u
#define DV1_SAFE_IRPT_IE_MIB_FIELDNUM         2u
#define DV1_SAFE_IRPT_IE_MIB_DEFAULTVAL       0x0u
#define DV1_MIB_IRPT_IP_0                     0x4A6D0028u
#define DV1_MIB_IRPT_IP_0_FIELDNUM            2u
#define DV1_MIB_IRPT_IP_0_DEFAULTVAL          0x0u
#define DV1_MIB_IRPT_IE_0                     0x4A6D002Cu
#define DV1_MIB_IRPT_IE_0_FIELDNUM            2u
#define DV1_MIB_IRPT_IE_0_DEFAULTVAL          0x0u
#define DV1_MIB_IRPT_IP_1                     0x4A6D0030u
#define DV1_MIB_IRPT_IP_1_FIELDNUM            2u
#define DV1_MIB_IRPT_IP_1_DEFAULTVAL          0x0u
#define DV1_MIB_IRPT_IE_1                     0x4A6D0034u
#define DV1_MIB_IRPT_IE_1_FIELDNUM            2u
#define DV1_MIB_IRPT_IE_1_DEFAULTVAL          0x0u
#define DV1_MIB_IRPT_IP_2                     0x4A6D0038u
#define DV1_MIB_IRPT_IP_2_FIELDNUM            2u
#define DV1_MIB_IRPT_IP_2_DEFAULTVAL          0x0u
#define DV1_MIB_IRPT_IE_2                     0x4A6D003Cu
#define DV1_MIB_IRPT_IE_2_FIELDNUM            2u
#define DV1_MIB_IRPT_IE_2_DEFAULTVAL          0x0u
#define DV1_SAFE_IRPT_IP_DISB                 0x4A6D0040u
#define DV1_SAFE_IRPT_IP_DISB_FIELDNUM        2u
#define DV1_SAFE_IRPT_IP_DISB_DEFAULTVAL      0x0u
#define DV1_SAFE_IRPT_IE_DISB                 0x4A6D0044u
#define DV1_SAFE_IRPT_IE_DISB_FIELDNUM        2u
#define DV1_SAFE_IRPT_IE_DISB_DEFAULTVAL      0x0u
#define DV1_DISB_IRPT_IP_0                    0x4A6D0048u
#define DV1_DISB_IRPT_IP_0_FIELDNUM           2u
#define DV1_DISB_IRPT_IP_0_DEFAULTVAL         0x0u
#define DV1_DISB_IRPT_IE_0                    0x4A6D004Cu
#define DV1_DISB_IRPT_IE_0_FIELDNUM           2u
#define DV1_DISB_IRPT_IE_0_DEFAULTVAL         0x0u
#define DV1_DISB_IRPT_IP_1                    0x4A6D0050u
#define DV1_DISB_IRPT_IP_1_FIELDNUM           2u
#define DV1_DISB_IRPT_IP_1_DEFAULTVAL         0x0u
#define DV1_DISB_IRPT_IE_1                    0x4A6D0054u
#define DV1_DISB_IRPT_IE_1_FIELDNUM           2u
#define DV1_DISB_IRPT_IE_1_DEFAULTVAL         0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_SAFETY_H_ */