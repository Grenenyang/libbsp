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

#ifndef DVA_RTKAS_REG_LIST_FILE_CPU_AUX_SYS_H_
#define DVA_RTKAS_REG_LIST_FILE_CPU_AUX_SYS_H_

#ifdef DVA_FILE_CPU_AUX_SYS
/* File: cpu_aux_sys */
#define DVA_INTERNAL_CPU_RESET_REQUEST                  0x42002400u
#define DVA_INTERNAL_CPU_RESET_REQUEST_FIELDNUM         14u
#define DVA_INTERNAL_CPU_RESET_REQUEST_DEFAULTVAL       0x0u
#define DVA_INTERNAL_CPU_GLOBAL_STATUS                  0x42002404u
#define DVA_INTERNAL_CPU_GLOBAL_STATUS_FIELDNUM         6u
#define DVA_INTERNAL_CPU_GLOBAL_STATUS_DEFAULTVAL       0x6u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_0                  0x42002408u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_0_FIELDNUM         18u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_0_DEFAULTVAL       0xFF3F015Du
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_1                  0x4200240Cu
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_1_FIELDNUM         3u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_1_DEFAULTVAL       0x5u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_2                  0x42002410u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_2_FIELDNUM         5u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_2_DEFAULTVAL       0x4A0000D0u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_3                  0x42002414u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_3_FIELDNUM         5u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_3_DEFAULTVAL       0x8000000u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_4                  0x42002418u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_4_FIELDNUM         5u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_4_DEFAULTVAL       0x60000000u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_5                  0x4200241Cu
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_5_FIELDNUM         9u
#define DVA_INTERNAL_CPU_GLOBAL_CTRL_5_DEFAULTVAL       0x4F2u
#define DVA_INTERNAL_CPU_GLOBAL_ERROR_ISR               0x42002420u
#define DVA_INTERNAL_CPU_GLOBAL_ERROR_ISR_FIELDNUM      5u
#define DVA_INTERNAL_CPU_GLOBAL_ERROR_ISR_DEFAULTVAL    0x0u
#define DVA_INTERNAL_CPU_GLOBAL_ERROR_IMR               0x42002424u
#define DVA_INTERNAL_CPU_GLOBAL_ERROR_IMR_FIELDNUM      5u
#define DVA_INTERNAL_CPU_GLOBAL_ERROR_IMR_DEFAULTVAL    0x800F0000u
#define DVA_INTERNAL_CPU_CORE0_CTRL_0                   0x42002428u
#define DVA_INTERNAL_CPU_CORE0_CTRL_0_FIELDNUM          11u
#define DVA_INTERNAL_CPU_CORE0_CTRL_0_DEFAULTVAL        0x15550034u
#define DVA_INTERNAL_CPU_CORE0_CTRL_1                   0x4200242Cu
#define DVA_INTERNAL_CPU_CORE0_CTRL_1_FIELDNUM          2u
#define DVA_INTERNAL_CPU_CORE0_CTRL_1_DEFAULTVAL        0x0u
#define DVA_INTERNAL_CPU_CORE0_MEMERRMEM                0x42002430u
#define DVA_INTERNAL_CPU_CORE0_MEMERRMEM_FIELDNUM       4u
#define DVA_INTERNAL_CPU_CORE0_MEMERRMEM_DEFAULTVAL     0x0u
#define DVA_INTERNAL_CPU_CORE0_PRIMEMERRIDX             0x42002434u
#define DVA_INTERNAL_CPU_CORE0_PRIMEMERRIDX_FIELDNUM    2u
#define DVA_INTERNAL_CPU_CORE0_PRIMEMERRIDX_DEFAULTVAL  0x0u
#define DVA_INTERNAL_CPU_CORE0_SECMEMERRIDX             0x42002438u
#define DVA_INTERNAL_CPU_CORE0_SECMEMERRIDX_FIELDNUM    2u
#define DVA_INTERNAL_CPU_CORE0_SECMEMERRIDX_DEFAULTVAL  0x0u
#define DVA_INTERNAL_CPU_CORE0_STATUS                   0x4200243Cu
#define DVA_INTERNAL_CPU_CORE0_STATUS_FIELDNUM          5u
#define DVA_INTERNAL_CPU_CORE0_STATUS_DEFAULTVAL        0x0u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_ISR_0             0x42002440u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_ISR_0_FIELDNUM    2u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_ISR_0_DEFAULTVAL  0x0u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_IMR_0             0x42002444u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_IMR_0_FIELDNUM    2u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_IMR_0_DEFAULTVAL  0x0u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_ISR_1             0x42002448u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_ISR_1_FIELDNUM    2u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_ISR_1_DEFAULTVAL  0x0u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_IMR_1             0x4200244Cu
#define DVA_INTERNAL_CPU_CORE_0_ERROR_IMR_1_FIELDNUM    2u
#define DVA_INTERNAL_CPU_CORE_0_ERROR_IMR_1_DEFAULTVAL  0x3FFFu
#define DVA_INTERNAL_CPU_FSS_RECORDER_CTRL              0x42002450u
#define DVA_INTERNAL_CPU_FSS_RECORDER_CTRL_FIELDNUM     9u
#define DVA_INTERNAL_CPU_FSS_RECORDER_CTRL_DEFAULTVAL   0x0u
#define DVA_INTERNAL_CPU_FSS0_CTRL_0                    0x42002454u
#define DVA_INTERNAL_CPU_FSS0_CTRL_0_FIELDNUM           16u
#define DVA_INTERNAL_CPU_FSS0_CTRL_0_DEFAULTVAL         0x0u
#define DVA_INTERNAL_CPU_FSS0_CTRL_1                    0x42002458u
#define DVA_INTERNAL_CPU_FSS0_CTRL_1_FIELDNUM           8u
#define DVA_INTERNAL_CPU_FSS0_CTRL_1_DEFAULTVAL         0x0u
#define DVA_INTERNAL_CPU_FSS0_STATUS                    0x4200245Cu
#define DVA_INTERNAL_CPU_FSS0_STATUS_FIELDNUM           13u
#define DVA_INTERNAL_CPU_FSS0_STATUS_DEFAULTVAL         0x0u
#define DVA_INTERNAL_CPU_FSS1_CTRL_0                    0x42002460u
#define DVA_INTERNAL_CPU_FSS1_CTRL_0_FIELDNUM           16u
#define DVA_INTERNAL_CPU_FSS1_CTRL_0_DEFAULTVAL         0x0u
#define DVA_INTERNAL_CPU_FSS1_CTRL_1                    0x42002464u
#define DVA_INTERNAL_CPU_FSS1_CTRL_1_FIELDNUM           8u
#define DVA_INTERNAL_CPU_FSS1_CTRL_1_DEFAULTVAL         0x0u
#define DVA_INTERNAL_CPU_FSS1_STATUS                    0x42002468u
#define DVA_INTERNAL_CPU_FSS1_STATUS_FIELDNUM           13u
#define DVA_INTERNAL_CPU_FSS1_STATUS_DEFAULTVAL         0x0u
#define DVA_INTERNAL_CPU_SRAM_MODE_0                    0x4200246Cu
#define DVA_INTERNAL_CPU_SRAM_MODE_0_FIELDNUM           6u
#define DVA_INTERNAL_CPU_SRAM_MODE_0_DEFAULTVAL         0x4Bu
#define DVA_INTERNAL_CPU_SRAM_MODE_1                    0x42002470u
#define DVA_INTERNAL_CPU_SRAM_MODE_1_FIELDNUM           4u
#define DVA_INTERNAL_CPU_SRAM_MODE_1_DEFAULTVAL         0xBu
#define DVA_INTERNAL_CPU_SRAM_MODE_2                    0x42002474u
#define DVA_INTERNAL_CPU_SRAM_MODE_2_FIELDNUM           4u
#define DVA_INTERNAL_CPU_SRAM_MODE_2_DEFAULTVAL         0xBu
#define DVA_INTERNAL_CPU_SRAM_MODE_3                    0x42002478u
#define DVA_INTERNAL_CPU_SRAM_MODE_3_FIELDNUM           4u
#define DVA_INTERNAL_CPU_SRAM_MODE_3_DEFAULTVAL         0xBu
#define DVA_INTERNAL_CPU_SRAM_MODE_4                    0x4200247Cu
#define DVA_INTERNAL_CPU_SRAM_MODE_4_FIELDNUM           4u
#define DVA_INTERNAL_CPU_SRAM_MODE_4_DEFAULTVAL         0xBu
#define DVA_INTERNAL_CPU_SRAM_MODE_5                    0x42002480u
#define DVA_INTERNAL_CPU_SRAM_MODE_5_FIELDNUM           4u
#define DVA_INTERNAL_CPU_SRAM_MODE_5_DEFAULTVAL         0xBu
#define DVA_INTERNAL_CPU_SRAM_MODE_6                    0x42002484u
#define DVA_INTERNAL_CPU_SRAM_MODE_6_FIELDNUM           5u
#define DVA_INTERNAL_CPU_SRAM_MODE_6_DEFAULTVAL         0x4Bu
#define DVA_INTERNAL_CPU_SRAM_MODE_7                    0x42002488u
#define DVA_INTERNAL_CPU_SRAM_MODE_7_FIELDNUM           4u
#define DVA_INTERNAL_CPU_SRAM_MODE_7_DEFAULTVAL         0xAu
#define DVA_INTERNAL_CPU_SRAM_MODE_8                    0x4200248Cu
#define DVA_INTERNAL_CPU_SRAM_MODE_8_FIELDNUM           2u
#define DVA_INTERNAL_CPU_SRAM_MODE_8_DEFAULTVAL         0x4Bu
#define DVA_INTERNAL_CPU_PLATFORM_DEBUG_CTRL            0x42002490u
#define DVA_INTERNAL_CPU_PLATFORM_DEBUG_CTRL_FIELDNUM   2u
#define DVA_INTERNAL_CPU_PLATFORM_DEBUG_CTRL_DEFAULTVAL 0x0u
#define DVA_INTERNAL_CPU_SPARE_0                        0x42002494u
#define DVA_INTERNAL_CPU_SPARE_0_FIELDNUM               1u
#define DVA_INTERNAL_CPU_SPARE_0_DEFAULTVAL             0x0u
#define DVA_INTERNAL_CPU_SPARE_1                        0x42002498u
#define DVA_INTERNAL_CPU_SPARE_1_FIELDNUM               1u
#define DVA_INTERNAL_CPU_SPARE_1_DEFAULTVAL             0x0u
#define DVA_INTERNAL_CPU_SPARE_2                        0x4200249Cu
#define DVA_INTERNAL_CPU_SPARE_2_FIELDNUM               1u
#define DVA_INTERNAL_CPU_SPARE_2_DEFAULTVAL             0x0u
#define DVA_INTERNAL_CPU_SPARE_3                        0x420024A0u
#define DVA_INTERNAL_CPU_SPARE_3_FIELDNUM               1u
#define DVA_INTERNAL_CPU_SPARE_3_DEFAULTVAL             0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_CPU_AUX_SYS_H_ */