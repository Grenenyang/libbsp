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

#ifndef DVA_ASICDRV_H_
#define DVA_ASICDRV_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"

#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
#define DVA_ASIC_INTERNAL
#else
#define DVA_ASIC_INTERNAL static
#endif

#define DVA_RTL9075_INTERFACE_NUM   (16u)
#define DVA_RTK_MAX_NUM_OF_SYS_PORT (11u)
#define DVA_RTK_SYS_PORT_ID_MAX     (12u)
#define DVA_RTK_PHY_ID_MAX          (11u)
#define DVA_RTK_INVALID_PORT_ID     (0xFFu)

#define DVA_RTK_SYS_PORT_1ST  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_1ST])
#define DVA_RTK_SYS_PORT_2ND  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_2ND])
#define DVA_RTK_SYS_PORT_3RD  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_3RD])
#define DVA_RTK_SYS_PORT_4TH  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_4TH])
#define DVA_RTK_SYS_PORT_5TH  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_5TH])
#define DVA_RTK_SYS_PORT_6TH  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_6TH])
#define DVA_RTK_SYS_PORT_7TH  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_7TH])
#define DVA_RTK_SYS_PORT_8TH  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_8TH])
#define DVA_RTK_SYS_PORT_9TH  (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_9TH])
#define DVA_RTK_SYS_PORT_10TH (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_10TH])
#define DVA_RTK_SYS_PORT_11TH (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_11TH])
#define DVA_RTK_SYS_PORT_12TH (dva_g_userPort2sysPort[DVA_RTK_USER_PORT_12TH])
#define DVA_RTK_SYS_PORT_END  (DVA_RTK_SYS_PORT_12TH)
#define DVA_RTK_CPU_PORT      (15u)

#define DVA_RTK_SYS_PMAP_1ST  ((uint32)1u << DVA_RTK_SYS_PORT_1ST)
#define DVA_RTK_SYS_PMAP_2ND  ((uint32)1u << DVA_RTK_SYS_PORT_2ND)
#define DVA_RTK_SYS_PMAP_3RD  ((uint32)1u << DVA_RTK_SYS_PORT_3RD)
#define DVA_RTK_SYS_PMAP_4TH  ((uint32)1u << DVA_RTK_SYS_PORT_4TH)
#define DVA_RTK_SYS_PMAP_5TH  ((uint32)1u << DVA_RTK_SYS_PORT_5TH)
#define DVA_RTK_SYS_PMAP_6TH  ((uint32)1u << DVA_RTK_SYS_PORT_6TH)
#define DVA_RTK_SYS_PMAP_7TH  ((uint32)1u << DVA_RTK_SYS_PORT_7TH)
#define DVA_RTK_SYS_PMAP_8TH  ((uint32)1u << DVA_RTK_SYS_PORT_8TH)
#define DVA_RTK_SYS_PMAP_9TH  ((uint32)1u << DVA_RTK_SYS_PORT_9TH)
#define DVA_RTK_SYS_PMAP_10TH ((uint32)1u << DVA_RTK_SYS_PORT_10TH)
#define DVA_RTK_SYS_PMAP_11TH ((uint32)1u << DVA_RTK_SYS_PORT_11TH)
#define DVA_RTK_SYS_PMAP_12TH ((uint32)1u << DVA_RTK_SYS_PORT_12TH)
#define DVA_RTK_SYS_PMAP_ALL  (uint32)(DVA_RTK_SYS_PMAP_1ST | DVA_RTK_SYS_PMAP_2ND | DVA_RTK_SYS_PMAP_3RD | DVA_RTK_SYS_PMAP_4TH | DVA_RTK_SYS_PMAP_5TH | DVA_RTK_SYS_PMAP_6TH | DVA_RTK_SYS_PMAP_7TH | DVA_RTK_SYS_PMAP_8TH | DVA_RTK_SYS_PMAP_9TH | DVA_RTK_SYS_PMAP_10TH | DVA_RTK_SYS_PMAP_11TH | DVA_RTK_SYS_PMAP_12TH)
#define DVA_RTK_CPU_PMAP      ((uint32)1u << DVA_RTK_CPU_PORT)

typedef enum
{
    DVA_EM_USER_TO_SYS = 0,
    DVA_EM_SYS_TO_USER,
    DVA_EM_PMAP_COVERT_MODE_LAST
} dva_RtkPmapCovertMode_e;

typedef uint32 dva_RtkPmap;

extern const uint8 dva_g_userPort2sysPort[DVA_RTL9075_INTERFACE_NUM];
extern const uint8 dva_g_sysPort2userPort[DVA_RTL9075_INTERFACE_NUM];

void      dva_drv_timer_init(void);
RtkApiRet dva_drv_pmap_convert(RtkPmap srcPmap, RtkPmap *dstPmap, dva_RtkPmapCovertMode_e mode);
RtkApiRet dva_drv_memset(void *str, int32 val, size_t size);
RtkApiRet dva_drv_memcpy(void *dst, const void *src, size_t size);
uint32    dva_drv_tbl_fieldlen2mask(uint32 fieldlen);
#endif /* #ifndef DVA_ASICDRV_H_ */
