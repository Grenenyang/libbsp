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

#ifndef DV1_RTK_API_TSN_H__
#define DV1_RTK_API_TSN_H__

#include "rtl907xd_v1_api.h"

#ifndef DV1_TSN_COMMON_HEADER
#define DV1_TSN_COMMON_HEADER
#define DV1_TSN_MAX_QUEUE      (7u)
#define DV1_TSN_NUMBER_OF_PORT (13u)
/* queue 0 ~ queue 7 */
#define DV1_TSN_NUMBER_OF_QUEUE (8u)

#define DV1_TSN_NUMBER_OF_PRIOIRTY (8u)
#define DV1_COMMON_TSN_UNIT        (1u)
/* 1 second = 10^9 ns */
#define DV1_TSN_1B_NANOSECOND (1000000000)

typedef struct
{
    uint16 msbSecond;  /**< seconds in high bits (0 to 0xFFFF) */
    uint32 lsbSecond;  /**< seconds in low bits (0 to 0xFFFFFFFF) */
    uint32 nanosecond; /**< nanoseconds (0 to 0xFFFFFFFF) */
} dv1_RtkTsnPtpTime_t;

typedef struct
{
    uint32 section[3]; /**< section[2]: bit 95-64, section[1]: bit 63-32, section[0]: bit 31-0, indicates nanoseconds */
} dv1_RtkTsnLongNanosecond_t;

/* !DV1_TSN_COMMON_HEADER */
#endif

#define DV1_TSN_SELECTED_FILED_DSTADR     (0x0001u)
#define DV1_TSN_SELECTED_FILED_SRCADR     (0x0002u)
#define DV1_TSN_SELECTED_FILED_ETHTERTYPE (0x0004u)
#define DV1_TSN_SELECTED_FILED_VID        (0x0008u)
#define DV1_TSN_SELECTED_FILED_SRCIP      (0x0010u)
#define DV1_TSN_SELECTED_FILED_DSTIP      (0x0020u)
#define DV1_TSN_SELECTED_FILED_DSCP       (0x0040u)
#define DV1_TSN_SELECTED_FILED_NEXTPROT   (0x0080u)
#define DV1_TSN_SELECTED_FIELD_SRCPORT    (0x0100u)
#define DV1_TSN_SELECTED_FIELD_DSTPORT    (0x0200u)

#endif /* DV1_RTK_API_TSN_H__ */