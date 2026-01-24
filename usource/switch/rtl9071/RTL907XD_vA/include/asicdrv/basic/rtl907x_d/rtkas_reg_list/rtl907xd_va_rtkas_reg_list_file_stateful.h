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

#ifndef DVA_RTKAS_REG_LIST_FILE_STATEFUL_H_
#define DVA_RTKAS_REG_LIST_FILE_STATEFUL_H_

#ifdef DVA_FILE_STATEFUL
/* File: stateful */
#define DVA_STATEFUL_CONTROL0                             0x4A7E0000u
#define DVA_STATEFUL_CONTROL0_FIELDNUM                    2u
#define DVA_STATEFUL_CONTROL0_DEFAULTVAL                  0x0u
#define DVA_STATEFUL_CONTROL1                             0x4A7E0004u
#define DVA_STATEFUL_CONTROL1_FIELDNUM                    2u
#define DVA_STATEFUL_CONTROL1_DEFAULTVAL                  0x0u
#define DVA_STATEFUL_CPUMASK                              0x4A7E0008u
#define DVA_STATEFUL_CPUMASK_FIELDNUM                     2u
#define DVA_STATEFUL_CPUMASK_DEFAULTVAL                   0x8000u
#define DVA_STATEFUL_MONITORHANDSHAKESPORTMASK            0x4A7E000Cu
#define DVA_STATEFUL_MONITORHANDSHAKESPORTMASK_FIELDNUM   2u
#define DVA_STATEFUL_MONITORHANDSHAKESPORTMASK_DEFAULTVAL 0x0u
#define DVA_STATEFUL_IGNORE_PORTMASK                      0x4A7E0010u
#define DVA_STATEFUL_IGNORE_PORTMASK_FIELDNUM             2u
#define DVA_STATEFUL_IGNORE_PORTMASK_DEFAULTVAL           0xFFFFu
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_SEC0               0x4A7E0014u
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_SEC0_FIELDNUM      1u
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_SEC0_DEFAULTVAL    0x14u
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_SEC1               0x4A7E0018u
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_SEC1_FIELDNUM      2u
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_SEC1_DEFAULTVAL    0x0u
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_NS                 0x4A7E001Cu
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_NS_FIELDNUM        1u
#define DVA_STATEFUL_TIMEOUT_THRESHOLD_NS_DEFAULTVAL      0x0u
#define DVA_FUSA_PG_LOCK_DMY_STATEFUL                     0x4A7E0020u
#define DVA_FUSA_PG_LOCK_DMY_STATEFUL_FIELDNUM            2u
#define DVA_FUSA_PG_LOCK_DMY_STATEFUL_DEFAULTVAL          0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_STATEFUL_H_ */