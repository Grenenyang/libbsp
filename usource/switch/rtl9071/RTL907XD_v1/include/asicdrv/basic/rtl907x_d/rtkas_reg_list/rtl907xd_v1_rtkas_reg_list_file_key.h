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

#ifndef DV1_RTKAS_REG_LIST_FILE_KEY_H_
#define DV1_RTKAS_REG_LIST_FILE_KEY_H_

#ifdef DV1_FILE_KEY
/* File: KEY */
#define DV1_SECURITY_PARA_0                  0x4A1D0000u
#define DV1_SECURITY_PARA_0_FIELDNUM         1u
#define DV1_SECURITY_PARA_0_DEFAULTVAL       0x0u
#define DV1_SECURITY_PARA_1                  0x4A1D0004u
#define DV1_SECURITY_PARA_1_FIELDNUM         1u
#define DV1_SECURITY_PARA_1_DEFAULTVAL       0x0u
#define DV1_SECURITY_PARA_2                  0x4A1D0008u
#define DV1_SECURITY_PARA_2_FIELDNUM         1u
#define DV1_SECURITY_PARA_2_DEFAULTVAL       0x0u
#define DV1_SECURITY_PARA_3                  0x4A1D000Cu
#define DV1_SECURITY_PARA_3_FIELDNUM         1u
#define DV1_SECURITY_PARA_3_DEFAULTVAL       0x0u
#define DV1_SECURITY_PARA_4                  0x4A1D0010u
#define DV1_SECURITY_PARA_4_FIELDNUM         1u
#define DV1_SECURITY_PARA_4_DEFAULTVAL       0x0u
#define DV1_SECURITY_PARA_5                  0x4A1D0014u
#define DV1_SECURITY_PARA_5_FIELDNUM         1u
#define DV1_SECURITY_PARA_5_DEFAULTVAL       0x0u
#define DV1_SECURITY_PARA_6                  0x4A1D0018u
#define DV1_SECURITY_PARA_6_FIELDNUM         1u
#define DV1_SECURITY_PARA_6_DEFAULTVAL       0x0u
#define DV1_SECURITY_PARA_7                  0x4A1D001Cu
#define DV1_SECURITY_PARA_7_FIELDNUM         1u
#define DV1_SECURITY_PARA_7_DEFAULTVAL       0x0u
#define DV1_SECUREBOOT_MCU_INTR_N            0x4A1D0020u
#define DV1_SECUREBOOT_MCU_INTR_N_FIELDNUM   2u
#define DV1_SECUREBOOT_MCU_INTR_N_DEFAULTVAL 0x7FFFu
#define DV1_MCU_INTR_DONE                    0x4A1D0024u
#define DV1_MCU_INTR_DONE_FIELDNUM           3u
#define DV1_MCU_INTR_DONE_DEFAULTVAL         0x0u
#define DV1_MCU_INTR_CTRL_REG0               0x4A1D0028u
#define DV1_MCU_INTR_CTRL_REG0_FIELDNUM      2u
#define DV1_MCU_INTR_CTRL_REG0_DEFAULTVAL    0x0u
#define DV1_DMA_START_ADDR_L                 0x4A1D002Cu
#define DV1_DMA_START_ADDR_L_FIELDNUM        2u
#define DV1_DMA_START_ADDR_L_DEFAULTVAL      0x0u
#define DV1_DMA_START_ADDR_H                 0x4A1D0030u
#define DV1_DMA_START_ADDR_H_FIELDNUM        2u
#define DV1_DMA_START_ADDR_H_DEFAULTVAL      0x0u
#define DV1_MCU_CONFIG_REG                   0x4A1D0034u
#define DV1_MCU_CONFIG_REG_FIELDNUM          3u
#define DV1_MCU_CONFIG_REG_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_0                    0x4A1D0038u
#define DV1_MACSEC_PARA_0_FIELDNUM           5u
#define DV1_MACSEC_PARA_0_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_1                    0x4A1D003Cu
#define DV1_MACSEC_PARA_1_FIELDNUM           2u
#define DV1_MACSEC_PARA_1_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_2                    0x4A1D0040u
#define DV1_MACSEC_PARA_2_FIELDNUM           2u
#define DV1_MACSEC_PARA_2_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_3                    0x4A1D0044u
#define DV1_MACSEC_PARA_3_FIELDNUM           2u
#define DV1_MACSEC_PARA_3_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_4                    0x4A1D0048u
#define DV1_MACSEC_PARA_4_FIELDNUM           2u
#define DV1_MACSEC_PARA_4_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_5                    0x4A1D004Cu
#define DV1_MACSEC_PARA_5_FIELDNUM           2u
#define DV1_MACSEC_PARA_5_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_6                    0x4A1D0050u
#define DV1_MACSEC_PARA_6_FIELDNUM           1u
#define DV1_MACSEC_PARA_6_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_7                    0x4A1D0054u
#define DV1_MACSEC_PARA_7_FIELDNUM           1u
#define DV1_MACSEC_PARA_7_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_8                    0x4A1D0058u
#define DV1_MACSEC_PARA_8_FIELDNUM           1u
#define DV1_MACSEC_PARA_8_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_9                    0x4A1D005Cu
#define DV1_MACSEC_PARA_9_FIELDNUM           1u
#define DV1_MACSEC_PARA_9_DEFAULTVAL         0x0u
#define DV1_MACSEC_PARA_10                   0x4A1D0060u
#define DV1_MACSEC_PARA_10_FIELDNUM          1u
#define DV1_MACSEC_PARA_10_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_11                   0x4A1D0064u
#define DV1_MACSEC_PARA_11_FIELDNUM          1u
#define DV1_MACSEC_PARA_11_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_12                   0x4A1D0068u
#define DV1_MACSEC_PARA_12_FIELDNUM          1u
#define DV1_MACSEC_PARA_12_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_13                   0x4A1D006Cu
#define DV1_MACSEC_PARA_13_FIELDNUM          1u
#define DV1_MACSEC_PARA_13_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_14                   0x4A1D0070u
#define DV1_MACSEC_PARA_14_FIELDNUM          1u
#define DV1_MACSEC_PARA_14_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_15                   0x4A1D0074u
#define DV1_MACSEC_PARA_15_FIELDNUM          1u
#define DV1_MACSEC_PARA_15_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_16                   0x4A1D0078u
#define DV1_MACSEC_PARA_16_FIELDNUM          1u
#define DV1_MACSEC_PARA_16_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_17                   0x4A1D007Cu
#define DV1_MACSEC_PARA_17_FIELDNUM          1u
#define DV1_MACSEC_PARA_17_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_18                   0x4A1D0080u
#define DV1_MACSEC_PARA_18_FIELDNUM          1u
#define DV1_MACSEC_PARA_18_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_19                   0x4A1D0084u
#define DV1_MACSEC_PARA_19_FIELDNUM          1u
#define DV1_MACSEC_PARA_19_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_20                   0x4A1D0088u
#define DV1_MACSEC_PARA_20_FIELDNUM          1u
#define DV1_MACSEC_PARA_20_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_21                   0x4A1D008Cu
#define DV1_MACSEC_PARA_21_FIELDNUM          1u
#define DV1_MACSEC_PARA_21_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_22                   0x4A1D0090u
#define DV1_MACSEC_PARA_22_FIELDNUM          1u
#define DV1_MACSEC_PARA_22_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_23                   0x4A1D0094u
#define DV1_MACSEC_PARA_23_FIELDNUM          1u
#define DV1_MACSEC_PARA_23_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_24                   0x4A1D0098u
#define DV1_MACSEC_PARA_24_FIELDNUM          1u
#define DV1_MACSEC_PARA_24_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_25                   0x4A1D009Cu
#define DV1_MACSEC_PARA_25_FIELDNUM          1u
#define DV1_MACSEC_PARA_25_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_26                   0x4A1D00A0u
#define DV1_MACSEC_PARA_26_FIELDNUM          1u
#define DV1_MACSEC_PARA_26_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_27                   0x4A1D00A4u
#define DV1_MACSEC_PARA_27_FIELDNUM          1u
#define DV1_MACSEC_PARA_27_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_28                   0x4A1D00A8u
#define DV1_MACSEC_PARA_28_FIELDNUM          1u
#define DV1_MACSEC_PARA_28_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_29                   0x4A1D00ACu
#define DV1_MACSEC_PARA_29_FIELDNUM          1u
#define DV1_MACSEC_PARA_29_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_30                   0x4A1D00B0u
#define DV1_MACSEC_PARA_30_FIELDNUM          1u
#define DV1_MACSEC_PARA_30_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_31                   0x4A1D00B4u
#define DV1_MACSEC_PARA_31_FIELDNUM          1u
#define DV1_MACSEC_PARA_31_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_32                   0x4A1D00B8u
#define DV1_MACSEC_PARA_32_FIELDNUM          1u
#define DV1_MACSEC_PARA_32_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_33                   0x4A1D00BCu
#define DV1_MACSEC_PARA_33_FIELDNUM          1u
#define DV1_MACSEC_PARA_33_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_34                   0x4A1D00C0u
#define DV1_MACSEC_PARA_34_FIELDNUM          1u
#define DV1_MACSEC_PARA_34_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_35                   0x4A1D00C4u
#define DV1_MACSEC_PARA_35_FIELDNUM          1u
#define DV1_MACSEC_PARA_35_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_36                   0x4A1D00C8u
#define DV1_MACSEC_PARA_36_FIELDNUM          1u
#define DV1_MACSEC_PARA_36_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_37                   0x4A1D00CCu
#define DV1_MACSEC_PARA_37_FIELDNUM          1u
#define DV1_MACSEC_PARA_37_DEFAULTVAL        0x0u
#define DV1_SECURE_INTF_PARA_0               0x4A1D00D0u
#define DV1_SECURE_INTF_PARA_0_FIELDNUM      4u
#define DV1_SECURE_INTF_PARA_0_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_1               0x4A1D00D4u
#define DV1_SECURE_INTF_PARA_1_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_1_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_2               0x4A1D00D8u
#define DV1_SECURE_INTF_PARA_2_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_2_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_3               0x4A1D00DCu
#define DV1_SECURE_INTF_PARA_3_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_3_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_4               0x4A1D00E0u
#define DV1_SECURE_INTF_PARA_4_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_4_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_5               0x4A1D00E4u
#define DV1_SECURE_INTF_PARA_5_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_5_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_6               0x4A1D00E8u
#define DV1_SECURE_INTF_PARA_6_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_6_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_7               0x4A1D00ECu
#define DV1_SECURE_INTF_PARA_7_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_7_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_8               0x4A1D00F0u
#define DV1_SECURE_INTF_PARA_8_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_8_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_9               0x4A1D00F4u
#define DV1_SECURE_INTF_PARA_9_FIELDNUM      1u
#define DV1_SECURE_INTF_PARA_9_DEFAULTVAL    0x0u
#define DV1_SECURE_INTF_PARA_10              0x4A1D00F8u
#define DV1_SECURE_INTF_PARA_10_FIELDNUM     1u
#define DV1_SECURE_INTF_PARA_10_DEFAULTVAL   0x0u
#define DV1_SECURE_INTF_PARA_11              0x4A1D00FCu
#define DV1_SECURE_INTF_PARA_11_FIELDNUM     1u
#define DV1_SECURE_INTF_PARA_11_DEFAULTVAL   0x0u
#define DV1_SECURE_INTF_PARA_12              0x4A1D0100u
#define DV1_SECURE_INTF_PARA_12_FIELDNUM     1u
#define DV1_SECURE_INTF_PARA_12_DEFAULTVAL   0x0u
#define DV1_SECURE_INTF_PARA_13              0x4A1D0104u
#define DV1_SECURE_INTF_PARA_13_FIELDNUM     3u
#define DV1_SECURE_INTF_PARA_13_DEFAULTVAL   0x3u
#define DV1_SECURE_INTF_PARA_14              0x4A1D0108u
#define DV1_SECURE_INTF_PARA_14_FIELDNUM     3u
#define DV1_SECURE_INTF_PARA_14_DEFAULTVAL   0x3u
#define DV1_MACSEC_PARA_38                   0x4A1D010Cu
#define DV1_MACSEC_PARA_38_FIELDNUM          3u
#define DV1_MACSEC_PARA_38_DEFAULTVAL        0x0u
#define DV1_MACSEC_PARA_39                   0x4A1D0110u
#define DV1_MACSEC_PARA_39_FIELDNUM          3u
#define DV1_MACSEC_PARA_39_DEFAULTVAL        0x0u
#define DV1_MCU_DBG_REG_01                   0x4A1D0114u
#define DV1_MCU_DBG_REG_01_FIELDNUM          1u
#define DV1_MCU_DBG_REG_01_DEFAULTVAL        0x0u
#define DV1_MCU_DBG_REG_02                   0x4A1D0118u
#define DV1_MCU_DBG_REG_02_FIELDNUM          1u
#define DV1_MCU_DBG_REG_02_DEFAULTVAL        0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_KEY_H_ */