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

#ifndef DVA_RTK_API_TSN_H__
#define DVA_RTK_API_TSN_H__

#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api.h"

#ifndef DVA_TSN_COMMON_HEADER
#define DVA_TSN_COMMON_HEADER
#define DVA_TSN_MAX_QUEUE      (7u)
#define DVA_TSN_NUMBER_OF_PORT (13u)
/* queue 0 ~ queue 7 */
#define DVA_TSN_NUMBER_OF_QUEUE (8u)

#define DVA_TSN_NUMBER_OF_PRIOIRTY (8u)
#define DVA_COMMON_TSN_UNIT        (1u)
/* 1 second = 10^9 ns */
#define DVA_TSN_1B_NANOSECOND (1000000000)

typedef struct
{
    uint32 section[3]; /**< section[2]: bit 95-64, section[1]: bit 63-32, section[0]: bit 31-0, indicates nanoseconds */
} dva_RtkTsnLongNanosecond_t;

/* !DVA_TSN_COMMON_HEADER */
#endif
/* for qci and cb */
#define DVA_TSN_SELECTED_FILED_DSTADR      (0x0001u)
#define DVA_TSN_SELECTED_FILED_SRCADR      (0x0002u)
#define DVA_TSN_SELECTED_FILED_ETHTERTYPE  (0x0004u)
#define DVA_TSN_SELECTED_FILED_VID         (0x0008u)
#define DVA_TSN_SELECTED_FILED_SRCIP       (0x0010u)
#define DVA_TSN_SELECTED_FILED_DSTIP       (0x0020u)
#define DVA_TSN_SELECTED_FILED_DSCP        (0x0040u)
#define DVA_TSN_SELECTED_FILED_NEXTPROC    (0x0080u)
#define DVA_TSN_SELECTED_FIELD_SRCPORT     (0x0100u)
#define DVA_TSN_SELECTED_FIELD_DSTPORT     (0x0200u)
#define DVA_TSN_SELECTED_FIELD_VIDPRI      (0x0400u)
#define DVA_TSN_SELECTED_FIELD_L4PORTRANGE (0x0800u)

#define DVA_TSN_CAREBITS_MAX           (0xFFFu)
#define DVA_TSN_FRER_STREAM_HANDLE_MAX (95u)
#define DVA_TSN_SID_INSTANCE_NUM_MAX   (96u)

#define DVA_TSN_SYSTEM_CLOCK_250M (250u * (1000000u)) /* 250 * (10 ^ 6) */
#define DVA_TSN_SYSTEM_CLOCK      (SYSTEM_CLOCK_250M)

#define DVA_TSN_CIR_MAX (10000000u)
#define DVA_TSN_EIR_MAX (10000000u)
#endif /* DVA_RTK_API_TSN_H__ */