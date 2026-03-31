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

#ifndef DV1_RTKAS_REG_LIST_FILE_CPU_EFUSE_H_
#define DV1_RTKAS_REG_LIST_FILE_CPU_EFUSE_H_

#ifdef DV1_FILE_CPU_EFUSE
/* File: EFUSE */
#define DV1_EFUSE_COMMAND                             0x4A190000u
#define DV1_EFUSE_COMMAND_FIELDNUM                    9u
#define DV1_EFUSE_COMMAND_DEFAULTVAL                  0x7000000u
#define DV1_EFUSE_DIRECT_PG                           0x4A190004u
#define DV1_EFUSE_DIRECT_PG_FIELDNUM                  12u
#define DV1_EFUSE_DIRECT_PG_DEFAULTVAL                0x0u
#define DV1_EFUSE_DMY_0                               0x4A190008u
#define DV1_EFUSE_DMY_0_FIELDNUM                      1u
#define DV1_EFUSE_DMY_0_DEFAULTVAL                    0x0u
#define DV1_EFUSE_DMY_1                               0x4A19000Cu
#define DV1_EFUSE_DMY_1_FIELDNUM                      1u
#define DV1_EFUSE_DMY_1_DEFAULTVAL                    0x0u
#define DV1_EFUSE_CONFIG0                             0x4A190010u
#define DV1_EFUSE_CONFIG0_FIELDNUM                    16u
#define DV1_EFUSE_CONFIG0_DEFAULTVAL                  0x0u
#define DV1_EFUSE_CONFIG1                             0x4A190014u
#define DV1_EFUSE_CONFIG1_FIELDNUM                    5u
#define DV1_EFUSE_CONFIG1_DEFAULTVAL                  0x0u
#define DV1_EFUSE_DMY_2                               0x4A190018u
#define DV1_EFUSE_DMY_2_FIELDNUM                      1u
#define DV1_EFUSE_DMY_2_DEFAULTVAL                    0x0u
#define DV1_EFUSE_DMY_3                               0x4A19001Cu
#define DV1_EFUSE_DMY_3_FIELDNUM                      1u
#define DV1_EFUSE_DMY_3_DEFAULTVAL                    0x0u
#define DV1_DIS_EFUSE_PG                              0x4A190020u
#define DV1_DIS_EFUSE_PG_FIELDNUM                     2u
#define DV1_DIS_EFUSE_PG_DEFAULTVAL                   0x1u
#define DV1_OTP_TRIGGER                               0x4A190024u
#define DV1_OTP_TRIGGER_FIELDNUM                      5u
#define DV1_OTP_TRIGGER_DEFAULTVAL                    0x0u
#define DV1_OTP_DATA_REG                              0x4A190028u
#define DV1_OTP_DATA_REG_FIELDNUM                     1u
#define DV1_OTP_DATA_REG_DEFAULTVAL                   0x0u
#define DV1_OTP_CTRL_REG                              0x4A19002Cu
#define DV1_OTP_CTRL_REG_FIELDNUM                     10u
#define DV1_OTP_CTRL_REG_DEFAULTVAL                   0x10u
#define DV1_OTP_TIMING_CTRL_REG_0                     0x4A190030u
#define DV1_OTP_TIMING_CTRL_REG_0_FIELDNUM            6u
#define DV1_OTP_TIMING_CTRL_REG_0_DEFAULTVAL          0x271064Du
#define DV1_OTP_TIMING_CTRL_REG_1                     0x4A190034u
#define DV1_OTP_TIMING_CTRL_REG_1_FIELDNUM            7u
#define DV1_OTP_TIMING_CTRL_REG_1_DEFAULTVAL          0x187D7E7Du
#define DV1_OTP_TIMING_CTRL_REG_2                     0x4A190038u
#define DV1_OTP_TIMING_CTRL_REG_2_FIELDNUM            13u
#define DV1_OTP_TIMING_CTRL_REG_2_DEFAULTVAL          0x8FF0031u
#define DV1_OTP_INT_STS_REG                           0x4A19003Cu
#define DV1_OTP_INT_STS_REG_FIELDNUM                  22u
#define DV1_OTP_INT_STS_REG_DEFAULTVAL                0x8FF0000u
#define DV1_OTP_FSM_STATE                             0x4A190040u
#define DV1_OTP_FSM_STATE_FIELDNUM                    6u
#define DV1_OTP_FSM_STATE_DEFAULTVAL                  0x0u
#define DV1_OTP_INIT_RD_NG_ADR_REG                    0x4A190044u
#define DV1_OTP_INIT_RD_NG_ADR_REG_FIELDNUM           4u
#define DV1_OTP_INIT_RD_NG_ADR_REG_DEFAULTVAL         0x0u
#define DV1_OTP_SW_DIRECT_REG                         0x4A190048u
#define DV1_OTP_SW_DIRECT_REG_FIELDNUM                2u
#define DV1_OTP_SW_DIRECT_REG_DEFAULTVAL              0x0u
#define DV1_OTP_TIMING_CTRL_REG_3                     0x4A19004Cu
#define DV1_OTP_TIMING_CTRL_REG_3_FIELDNUM            7u
#define DV1_OTP_TIMING_CTRL_REG_3_DEFAULTVAL          0x403F0000u
#define DV1_OTP_INT_STATUS_READ                       0x4A190050u
#define DV1_OTP_INT_STATUS_READ_FIELDNUM              11u
#define DV1_OTP_INT_STATUS_READ_DEFAULTVAL            0x0u
#define DV1_OTP_READ_DATA_REG_0                       0x4A190054u
#define DV1_OTP_READ_DATA_REG_0_FIELDNUM              1u
#define DV1_OTP_READ_DATA_REG_0_DEFAULTVAL            0x0u
#define DV1_OTP_READ_DATA_REG_1                       0x4A190058u
#define DV1_OTP_READ_DATA_REG_1_FIELDNUM              1u
#define DV1_OTP_READ_DATA_REG_1_DEFAULTVAL            0x0u
#define DV1_OTP_READ_DATA_REG_2                       0x4A19005Cu
#define DV1_OTP_READ_DATA_REG_2_FIELDNUM              1u
#define DV1_OTP_READ_DATA_REG_2_DEFAULTVAL            0x0u
#define DV1_OTP_CTL_2                                 0x4A190060u
#define DV1_OTP_CTL_2_FIELDNUM                        2u
#define DV1_OTP_CTL_2_DEFAULTVAL                      0x0u
#define DV1_OTP_LOCK_BIT_REGION_01                    0x4A190064u
#define DV1_OTP_LOCK_BIT_REGION_01_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_01_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_02                    0x4A190068u
#define DV1_OTP_LOCK_BIT_REGION_02_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_02_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_03                    0x4A19006Cu
#define DV1_OTP_LOCK_BIT_REGION_03_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_03_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_04                    0x4A190070u
#define DV1_OTP_LOCK_BIT_REGION_04_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_04_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_05                    0x4A190074u
#define DV1_OTP_LOCK_BIT_REGION_05_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_05_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_06                    0x4A190078u
#define DV1_OTP_LOCK_BIT_REGION_06_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_06_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_07                    0x4A19007Cu
#define DV1_OTP_LOCK_BIT_REGION_07_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_07_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_08                    0x4A190080u
#define DV1_OTP_LOCK_BIT_REGION_08_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_08_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_09                    0x4A190084u
#define DV1_OTP_LOCK_BIT_REGION_09_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_09_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_10                    0x4A190088u
#define DV1_OTP_LOCK_BIT_REGION_10_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_10_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_11                    0x4A19008Cu
#define DV1_OTP_LOCK_BIT_REGION_11_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_11_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_12                    0x4A190090u
#define DV1_OTP_LOCK_BIT_REGION_12_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_12_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_13                    0x4A190094u
#define DV1_OTP_LOCK_BIT_REGION_13_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_13_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_14                    0x4A190098u
#define DV1_OTP_LOCK_BIT_REGION_14_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_14_DEFAULTVAL         0xFFFFFFFFu
#define DV1_OTP_LOCK_BIT_REGION_15                    0x4A19009Cu
#define DV1_OTP_LOCK_BIT_REGION_15_FIELDNUM           1u
#define DV1_OTP_LOCK_BIT_REGION_15_DEFAULTVAL         0xFFFFu
#define DV1_OTP_DUMMY_REG_00                          0x4A1900A0u
#define DV1_OTP_DUMMY_REG_00_FIELDNUM                 1u
#define DV1_OTP_DUMMY_REG_00_DEFAULTVAL               0x0u
#define DV1_OTP_FAULT_INJECTION_00                    0x4A1900A4u
#define DV1_OTP_FAULT_INJECTION_00_FIELDNUM           2u
#define DV1_OTP_FAULT_INJECTION_00_DEFAULTVAL         0x0u
#define DV1_OTP_FAULT_INJECTION_01                    0x4A1900A8u
#define DV1_OTP_FAULT_INJECTION_01_FIELDNUM           1u
#define DV1_OTP_FAULT_INJECTION_01_DEFAULTVAL         0x0u
#define DV1_OTP_FAULT_INJECTION_02                    0x4A1900ACu
#define DV1_OTP_FAULT_INJECTION_02_FIELDNUM           1u
#define DV1_OTP_FAULT_INJECTION_02_DEFAULTVAL         0x0u
#define DV1_OTP_FAULT_INJECTION_03                    0x4A1900B0u
#define DV1_OTP_FAULT_INJECTION_03_FIELDNUM           1u
#define DV1_OTP_FAULT_INJECTION_03_DEFAULTVAL         0x0u
#define DV1_OTP_INDIRECT_OCCUPY_STATUS_REG            0x4A1900B4u
#define DV1_OTP_INDIRECT_OCCUPY_STATUS_REG_FIELDNUM   1u
#define DV1_OTP_INDIRECT_OCCUPY_STATUS_REG_DEFAULTVAL 0x0u
#endif
#endif /* DV1_RTKAS_REG_LIST_FILE_CPU_EFUSE_H_ */