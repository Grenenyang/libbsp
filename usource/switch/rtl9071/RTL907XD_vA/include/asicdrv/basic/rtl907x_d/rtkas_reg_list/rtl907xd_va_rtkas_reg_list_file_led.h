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

#ifndef DVA_RTKAS_REG_LIST_FILE_LED_H_
#define DVA_RTKAS_REG_LIST_FILE_LED_H_

#ifdef DVA_FILE_LED
/* File: LED */
#define DVA_LED_CONTROL0                      0x4A670000u
#define DVA_LED_CONTROL0_FIELDNUM             20u
#define DVA_LED_CONTROL0_DEFAULTVAL           0x0u
#define DVA_LED_CONTROL1                      0x4A670004u
#define DVA_LED_CONTROL1_FIELDNUM             13u
#define DVA_LED_CONTROL1_DEFAULTVAL           0x0u
#define DVA_LED_STATUS_CONTROL0               0x4A670008u
#define DVA_LED_STATUS_CONTROL0_FIELDNUM      8u
#define DVA_LED_STATUS_CONTROL0_DEFAULTVAL    0x0u
#define DVA_LED_STATUS_CONTROL1               0x4A67000Cu
#define DVA_LED_STATUS_CONTROL1_FIELDNUM      5u
#define DVA_LED_STATUS_CONTROL1_DEFAULTVAL    0xBCu
#define DVA_LED_INDICATOR_CONTROL0            0x4A670010u
#define DVA_LED_INDICATOR_CONTROL0_FIELDNUM   7u
#define DVA_LED_INDICATOR_CONTROL0_DEFAULTVAL 0x0u
#define DVA_LED_INDICATOR_CONTROL1            0x4A670014u
#define DVA_LED_INDICATOR_CONTROL1_FIELDNUM   7u
#define DVA_LED_INDICATOR_CONTROL1_DEFAULTVAL 0x0u
#define DVA_LED_INDICATOR_CONTROL2            0x4A670018u
#define DVA_LED_INDICATOR_CONTROL2_FIELDNUM   2u
#define DVA_LED_INDICATOR_CONTROL2_DEFAULTVAL 0x0u
#define DVA_LED_INDICATOR_CONTROL3            0x4A67001Cu
#define DVA_LED_INDICATOR_CONTROL3_FIELDNUM   2u
#define DVA_LED_INDICATOR_CONTROL3_DEFAULTVAL 0x0u
#define DVA_LED_INDICATOR_CONTROL4            0x4A670020u
#define DVA_LED_INDICATOR_CONTROL4_FIELDNUM   2u
#define DVA_LED_INDICATOR_CONTROL4_DEFAULTVAL 0x0u
#define DVA_LED_INDICATOR_CONTROL5            0x4A670024u
#define DVA_LED_INDICATOR_CONTROL5_FIELDNUM   2u
#define DVA_LED_INDICATOR_CONTROL5_DEFAULTVAL 0x0u
#define DVA_LED_INDICATOR_CONTROL6            0x4A670028u
#define DVA_LED_INDICATOR_CONTROL6_FIELDNUM   2u
#define DVA_LED_INDICATOR_CONTROL6_DEFAULTVAL 0x0u
#define DVA_LED_INDICATOR_CONTROL7            0x4A67002Cu
#define DVA_LED_INDICATOR_CONTROL7_FIELDNUM   2u
#define DVA_LED_INDICATOR_CONTROL7_DEFAULTVAL 0x0u
#define DVA_FUSA_PG_LOCK_DMY_LED              0x4A670030u
#define DVA_FUSA_PG_LOCK_DMY_LED_FIELDNUM     2u
#define DVA_FUSA_PG_LOCK_DMY_LED_DEFAULTVAL   0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_LED_H_ */