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

#ifndef DV1_RTKAS_REG_LIST_FILE_MIITEST_DIAG_H_
#define DV1_RTKAS_REG_LIST_FILE_MIITEST_DIAG_H_

#ifdef DV1_FILE_MIITEST_DIAG
/* File: MII Test Related Diagnostic */
#define DV1_MAC_MII_TEST_MODE_TX_CONTROL            0x4A6E0000u
#define DV1_MAC_MII_TEST_MODE_TX_CONTROL_FIELDNUM   2u
#define DV1_MAC_MII_TEST_MODE_TX_CONTROL_DEFAULTVAL 0x0u
#define DV1_MAC_MII_TEST_MODE_RX_CONTROL            0x4A6E0004u
#define DV1_MAC_MII_TEST_MODE_RX_CONTROL_FIELDNUM   2u
#define DV1_MAC_MII_TEST_MODE_RX_CONTROL_DEFAULTVAL 0x0u
#define DV1_MAC_TEST_MODE_GMII_RX                   0x4A6E0008u
#define DV1_MAC_TEST_MODE_GMII_RX_FIELDNUM          6u
#define DV1_MAC_TEST_MODE_GMII_RX_DEFAULTVAL        0x0u
#define DV1_MAC_TEST_MODE_GMII_TX                   0x4A6E000Cu
#define DV1_MAC_TEST_MODE_GMII_TX_FIELDNUM          6u
#define DV1_MAC_TEST_MODE_GMII_TX_DEFAULTVAL        0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_MIITEST_DIAG_H_ */