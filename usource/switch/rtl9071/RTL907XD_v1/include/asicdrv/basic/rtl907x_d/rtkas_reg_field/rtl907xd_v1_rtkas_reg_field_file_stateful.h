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

#ifndef DV1_RTKAS_REG_FIELD_FILE_STATEFUL_H_
#define DV1_RTKAS_REG_FIELD_FILE_STATEFUL_H_

#ifdef DV1_FILE_STATEFUL
/* File: stateful */
#define DV1_STATEFUL_CONTROL0_STATEFUL_CONTROL_REGISTER_0_LSP  0u
#define DV1_STATEFUL_CONTROL0_STATEFUL_CONTROL_REGISTER_0_LEN  10u
#define DV1_STATEFUL_CONTROL0_STATEFUL_CONTROL_REGISTER_0_TYPE DV1_FIELD_TYPE_RW

#define DV1_STATEFUL_CONTROL1_STATEFUL_CONTROL_REGISTER_1_LSP  0u
#define DV1_STATEFUL_CONTROL1_STATEFUL_CONTROL_REGISTER_1_LEN  1u
#define DV1_STATEFUL_CONTROL1_STATEFUL_CONTROL_REGISTER_1_TYPE DV1_FIELD_TYPE_RWS

#define DV1_STATEFUL_CPUMASK_STATEFUL_CPUMASK_LSP  0u
#define DV1_STATEFUL_CPUMASK_STATEFUL_CPUMASK_LEN  16u
#define DV1_STATEFUL_CPUMASK_STATEFUL_CPUMASK_TYPE DV1_FIELD_TYPE_RW

#define DV1_STATEFUL_MONITORHANDSHAKESPORTMASK_STATEFUL_MONITORHANDSHAKESPORTMASK_LSP  0u
#define DV1_STATEFUL_MONITORHANDSHAKESPORTMASK_STATEFUL_MONITORHANDSHAKESPORTMASK_LEN  16u
#define DV1_STATEFUL_MONITORHANDSHAKESPORTMASK_STATEFUL_MONITORHANDSHAKESPORTMASK_TYPE DV1_FIELD_TYPE_RW

#define DV1_STATEFUL_IGNORE_PORTMASK_STATEFUL_IGNORE_PORTMASK_LSP  0u
#define DV1_STATEFUL_IGNORE_PORTMASK_STATEFUL_IGNORE_PORTMASK_LEN  16u
#define DV1_STATEFUL_IGNORE_PORTMASK_STATEFUL_IGNORE_PORTMASK_TYPE DV1_FIELD_TYPE_RW

#define DV1_STATEFUL_TIMEOUT_THRESHOLD_SEC0_TIMEOUT_THRESHOLD_SEC_0_LSP  0u
#define DV1_STATEFUL_TIMEOUT_THRESHOLD_SEC0_TIMEOUT_THRESHOLD_SEC_0_LEN  32u
#define DV1_STATEFUL_TIMEOUT_THRESHOLD_SEC0_TIMEOUT_THRESHOLD_SEC_0_TYPE DV1_FIELD_TYPE_RW

#define DV1_STATEFUL_TIMEOUT_THRESHOLD_SEC1_TIMEOUT_THRESHOLD_SEC_1_LSP  0u
#define DV1_STATEFUL_TIMEOUT_THRESHOLD_SEC1_TIMEOUT_THRESHOLD_SEC_1_LEN  16u
#define DV1_STATEFUL_TIMEOUT_THRESHOLD_SEC1_TIMEOUT_THRESHOLD_SEC_1_TYPE DV1_FIELD_TYPE_RW

#define DV1_STATEFUL_TIMEOUT_THRESHOLD_NS_TIMEOUT_THRESHOLD_NS_LSP  0u
#define DV1_STATEFUL_TIMEOUT_THRESHOLD_NS_TIMEOUT_THRESHOLD_NS_LEN  32u
#define DV1_STATEFUL_TIMEOUT_THRESHOLD_NS_TIMEOUT_THRESHOLD_NS_TYPE DV1_FIELD_TYPE_RW

#endif
#endif /* DV1_RTKAS_REG_FIELD_FILE_STATEFUL_H_ */