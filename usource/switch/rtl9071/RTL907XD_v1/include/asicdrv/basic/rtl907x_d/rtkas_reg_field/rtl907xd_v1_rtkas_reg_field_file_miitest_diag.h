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

#ifndef DV1_RTKAS_REG_FIELD_FILE_MIITEST_DIAG_H_
#define DV1_RTKAS_REG_FIELD_FILE_MIITEST_DIAG_H_

#ifdef DV1_FILE_MIITEST_DIAG
/* File: MII Test Related Diagnostic */
#define DV1_MAC_MII_TEST_MODE_TX_CONTROL_TXTMENPM_LSP  0u
#define DV1_MAC_MII_TEST_MODE_TX_CONTROL_TXTMENPM_LEN  16u
#define DV1_MAC_MII_TEST_MODE_TX_CONTROL_TXTMENPM_TYPE DV1_FIELD_TYPE_RW

#define DV1_MAC_MII_TEST_MODE_RX_CONTROL_RXTMENPM_LSP  0u
#define DV1_MAC_MII_TEST_MODE_RX_CONTROL_RXTMENPM_LEN  16u
#define DV1_MAC_MII_TEST_MODE_RX_CONTROL_RXTMENPM_TYPE DV1_FIELD_TYPE_RW

#define DV1_MAC_TEST_MODE_GMII_RX_RXC_LSP      17u
#define DV1_MAC_TEST_MODE_GMII_RX_RXC_LEN      1u
#define DV1_MAC_TEST_MODE_GMII_RX_RXC_TYPE     DV1_FIELD_TYPE_RWS
#define DV1_MAC_TEST_MODE_GMII_RX_RXEN_LSP     16u
#define DV1_MAC_TEST_MODE_GMII_RX_RXEN_LEN     1u
#define DV1_MAC_TEST_MODE_GMII_RX_RXEN_TYPE    DV1_FIELD_TYPE_RWS
#define DV1_MAC_TEST_MODE_GMII_RX_RXPN_LSP     8u
#define DV1_MAC_TEST_MODE_GMII_RX_RXPN_LEN     4u
#define DV1_MAC_TEST_MODE_GMII_RX_RXPN_TYPE    DV1_FIELD_TYPE_RWS
#define DV1_MAC_TEST_MODE_GMII_RX_RXD_7_0_LSP  0u
#define DV1_MAC_TEST_MODE_GMII_RX_RXD_7_0_LEN  8u
#define DV1_MAC_TEST_MODE_GMII_RX_RXD_7_0_TYPE DV1_FIELD_TYPE_RWS

#define DV1_MAC_TEST_MODE_GMII_TX_LATCH_TXREG_LSP  17u
#define DV1_MAC_TEST_MODE_GMII_TX_LATCH_TXREG_LEN  1u
#define DV1_MAC_TEST_MODE_GMII_TX_LATCH_TXREG_TYPE DV1_FIELD_TYPE_RWS
#define DV1_MAC_TEST_MODE_GMII_TX_TXEN_LSP         16u
#define DV1_MAC_TEST_MODE_GMII_TX_TXEN_LEN         1u
#define DV1_MAC_TEST_MODE_GMII_TX_TXEN_TYPE        DV1_FIELD_TYPE_RS
#define DV1_MAC_TEST_MODE_GMII_TX_TXPN_LSP         8u
#define DV1_MAC_TEST_MODE_GMII_TX_TXPN_LEN         4u
#define DV1_MAC_TEST_MODE_GMII_TX_TXPN_TYPE        DV1_FIELD_TYPE_RWS
#define DV1_MAC_TEST_MODE_GMII_TX_TXD_7_0_LSP      0u
#define DV1_MAC_TEST_MODE_GMII_TX_TXD_7_0_LEN      8u
#define DV1_MAC_TEST_MODE_GMII_TX_TXD_7_0_TYPE     DV1_FIELD_TYPE_RS

#endif
#endif /* DV1_RTKAS_REG_FIELD_FILE_MIITEST_DIAG_H_ */