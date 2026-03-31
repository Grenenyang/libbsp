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

#ifndef DVA_RTKAS_REG_LIST_FILE_CPU_AUX_VSFC_H_
#define DVA_RTKAS_REG_LIST_FILE_CPU_AUX_VSFC_H_

#ifdef DVA_FILE_CPU_AUX_VSFC
/* File: cpu_aux_vsfc */
#define DVA_VSFC_CTRL                  0x42002000u
#define DVA_VSFC_CTRL_FIELDNUM         16u
#define DVA_VSFC_CTRL_DEFAULTVAL       0x0u
#define DVA_VSFC_CTRL2                 0x42002004u
#define DVA_VSFC_CTRL2_FIELDNUM        4u
#define DVA_VSFC_CTRL2_DEFAULTVAL      0x0u
#define DVA_VSFC_STATUS                0x42002008u
#define DVA_VSFC_STATUS_FIELDNUM       3u
#define DVA_VSFC_STATUS_DEFAULTVAL     0x1u
#define DVA_VSFC_TRIG_CTRL             0x4200200Cu
#define DVA_VSFC_TRIG_CTRL_FIELDNUM    8u
#define DVA_VSFC_TRIG_CTRL_DEFAULTVAL  0x0u
#define DVA_VSFC_RECV_CTRL0            0x42002010u
#define DVA_VSFC_RECV_CTRL0_FIELDNUM   2u
#define DVA_VSFC_RECV_CTRL0_DEFAULTVAL 0x1u
#define DVA_VSFC_RECV_CTRL1            0x42002014u
#define DVA_VSFC_RECV_CTRL1_FIELDNUM   2u
#define DVA_VSFC_RECV_CTRL1_DEFAULTVAL 0x10u
#define DVA_VSFC_RECV_CTRL2            0x42002018u
#define DVA_VSFC_RECV_CTRL2_FIELDNUM   2u
#define DVA_VSFC_RECV_CTRL2_DEFAULTVAL 0x10u
#define DVA_VSFC_RECV_CTRL3            0x4200201Cu
#define DVA_VSFC_RECV_CTRL3_FIELDNUM   2u
#define DVA_VSFC_RECV_CTRL3_DEFAULTVAL 0x10u
#define DVA_VSFC_RECV_CTRL4            0x42002020u
#define DVA_VSFC_RECV_CTRL4_FIELDNUM   2u
#define DVA_VSFC_RECV_CTRL4_DEFAULTVAL 0x0u
#define DVA_VSFC_SW_RECV               0x42002024u
#define DVA_VSFC_SW_RECV_FIELDNUM      5u
#define DVA_VSFC_SW_RECV_DEFAULTVAL    0x0u
#define DVA_VSFC_PROF_CTRL0            0x42002028u
#define DVA_VSFC_PROF_CTRL0_FIELDNUM   4u
#define DVA_VSFC_PROF_CTRL0_DEFAULTVAL 0x0u
#define DVA_VSFC_PROF_ST0              0x4200202Cu
#define DVA_VSFC_PROF_ST0_FIELDNUM     3u
#define DVA_VSFC_PROF_ST0_DEFAULTVAL   0x0u
#define DVA_VSFC_PROF_ST1              0x42002030u
#define DVA_VSFC_PROF_ST1_FIELDNUM     2u
#define DVA_VSFC_PROF_ST1_DEFAULTVAL   0xFFFF0000u
#define DVA_VSFC_PROF_ST2              0x42002034u
#define DVA_VSFC_PROF_ST2_FIELDNUM     2u
#define DVA_VSFC_PROF_ST2_DEFAULTVAL   0x0u
#define DVA_VSFC_PROF_ST3              0x42002038u
#define DVA_VSFC_PROF_ST3_FIELDNUM     6u
#define DVA_VSFC_PROF_ST3_DEFAULTVAL   0x0u
#define DVA_VSFC_PROF_ST4              0x4200203Cu
#define DVA_VSFC_PROF_ST4_FIELDNUM     4u
#define DVA_VSFC_PROF_ST4_DEFAULTVAL   0x0u
#define DVA_VSFC_INTST                 0x42002040u
#define DVA_VSFC_INTST_FIELDNUM        4u
#define DVA_VSFC_INTST_DEFAULTVAL      0x0u
#define DVA_VSFC_INTEN                 0x42002044u
#define DVA_VSFC_INTEN_FIELDNUM        4u
#define DVA_VSFC_INTEN_DEFAULTVAL      0x0u
#define DVA_VSFC_SPARE_0               0x42002048u
#define DVA_VSFC_SPARE_0_FIELDNUM      1u
#define DVA_VSFC_SPARE_0_DEFAULTVAL    0x0u
#define DVA_VSFC_SPARE_1               0x4200204Cu
#define DVA_VSFC_SPARE_1_FIELDNUM      1u
#define DVA_VSFC_SPARE_1_DEFAULTVAL    0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_CPU_AUX_VSFC_H_ */