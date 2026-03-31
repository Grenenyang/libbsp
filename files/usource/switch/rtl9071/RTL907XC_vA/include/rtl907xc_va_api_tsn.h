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

#ifndef CVA_RTK_API_TSN_H__
#define CVA_RTK_API_TSN_H__

#include "rtl907xc_va_api.h"

#ifndef CVA_TSN_COMMON_HEADER
#define CVA_TSN_COMMON_HEADER
#define CVA_TSN_MAX_QUEUE      (7u)
#define CVA_TSN_NUMBER_OF_PORT (13u)
/* queue 0 ~ queue 7 */
#define CVA_TSN_NUMBER_OF_QUEUE (8u)

#define CVA_TSN_NUMBER_OF_PRIOIRTY (8u)
#define CVA_COMMON_TSN_UNIT        (1u)
/* 1 second = 10^9 ns */
#define CVA_TSN_1B_NANOSECOND (1000000000)

typedef struct
{
    uint32 section[3]; /**< section[2]: bit 95-64, section[1]: bit 63-32, section[0]: bit 31-0, indicates nanoseconds */
} cva_RtkTsnLongNanosecond_t;

/* !CVA_TSN_COMMON_HEADER */
#endif

#define CVA_TSN_SELECTED_FILED_DSTADR     (0x0001u)
#define CVA_TSN_SELECTED_FILED_SRCADR     (0x0002u)
#define CVA_TSN_SELECTED_FILED_ETHTERTYPE (0x0004u)
#define CVA_TSN_SELECTED_FILED_VID        (0x0008u)
#define CVA_TSN_SELECTED_FILED_SRCIP      (0x0010u)
#define CVA_TSN_SELECTED_FILED_DSTIP      (0x0020u)
#define CVA_TSN_SELECTED_FILED_DSCP       (0x0040u)
#define CVA_TSN_SELECTED_FILED_NEXTPROC   (0x0080u)
#define CVA_TSN_SELECTED_FIELD_SRCPORT    (0x0100u)
#define CVA_TSN_SELECTED_FIELD_DSTPORT    (0x0200u)

#define CVA_TSN_CAREBITS_MAX           (0x3FFu)
#define CVA_TSN_FRER_STREAM_HANDLE_MAX (95u)
#define CVA_TSN_SID_INSTANCE_NUM_MAX   (96u)
#endif /* CVA_RTK_API_TSN_H__ */