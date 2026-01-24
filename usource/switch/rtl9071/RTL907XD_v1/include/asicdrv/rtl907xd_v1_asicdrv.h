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

#ifndef DV1_ASICDRV_H_
#define DV1_ASICDRV_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"

#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
#define DV1_ASIC_INTERNAL
#else
#define DV1_ASIC_INTERNAL static
#endif

#define DV1_RTL9075_INTERFACE_NUM   (16u)
#define DV1_RTK_MAX_NUM_OF_SYS_PORT (11u)
#define DV1_RTK_SYS_PORT_ID_MAX     (12u)
#define DV1_RTK_PHY_ID_MAX          (11u)
#define DV1_RTK_INVALID_PORT_ID     (0xFFu)

#define DV1_RTK_SYS_PORT_1ST  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_1ST])
#define DV1_RTK_SYS_PORT_2ND  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_2ND])
#define DV1_RTK_SYS_PORT_3RD  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_3RD])
#define DV1_RTK_SYS_PORT_4TH  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_4TH])
#define DV1_RTK_SYS_PORT_5TH  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_5TH])
#define DV1_RTK_SYS_PORT_6TH  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_6TH])
#define DV1_RTK_SYS_PORT_7TH  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_7TH])
#define DV1_RTK_SYS_PORT_8TH  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_8TH])
#define DV1_RTK_SYS_PORT_9TH  (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_9TH])
#define DV1_RTK_SYS_PORT_10TH (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_10TH])
#define DV1_RTK_SYS_PORT_11TH (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_11TH])
#define DV1_RTK_SYS_PORT_12TH (dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_12TH])
#define DV1_RTK_SYS_PORT_END  (DV1_RTK_SYS_PORT_12TH)
#define DV1_RTK_CPU_PORT      (15u)

#define DV1_RTK_SYS_PMAP_1ST  ((uint32)1u << DV1_RTK_SYS_PORT_1ST)
#define DV1_RTK_SYS_PMAP_2ND  ((uint32)1u << DV1_RTK_SYS_PORT_2ND)
#define DV1_RTK_SYS_PMAP_3RD  ((uint32)1u << DV1_RTK_SYS_PORT_3RD)
#define DV1_RTK_SYS_PMAP_4TH  ((uint32)1u << DV1_RTK_SYS_PORT_4TH)
#define DV1_RTK_SYS_PMAP_5TH  ((uint32)1u << DV1_RTK_SYS_PORT_5TH)
#define DV1_RTK_SYS_PMAP_6TH  ((uint32)1u << DV1_RTK_SYS_PORT_6TH)
#define DV1_RTK_SYS_PMAP_7TH  ((uint32)1u << DV1_RTK_SYS_PORT_7TH)
#define DV1_RTK_SYS_PMAP_8TH  ((uint32)1u << DV1_RTK_SYS_PORT_8TH)
#define DV1_RTK_SYS_PMAP_9TH  ((uint32)1u << DV1_RTK_SYS_PORT_9TH)
#define DV1_RTK_SYS_PMAP_10TH ((uint32)1u << DV1_RTK_SYS_PORT_10TH)
#define DV1_RTK_SYS_PMAP_11TH ((uint32)1u << DV1_RTK_SYS_PORT_11TH)
#define DV1_RTK_SYS_PMAP_12TH ((uint32)1u << DV1_RTK_SYS_PORT_12TH)
#define DV1_RTK_SYS_PMAP_ALL  (uint32)(DV1_RTK_SYS_PMAP_1ST | DV1_RTK_SYS_PMAP_2ND | DV1_RTK_SYS_PMAP_3RD | DV1_RTK_SYS_PMAP_4TH | DV1_RTK_SYS_PMAP_5TH | DV1_RTK_SYS_PMAP_6TH | DV1_RTK_SYS_PMAP_7TH | DV1_RTK_SYS_PMAP_8TH | DV1_RTK_SYS_PMAP_9TH | DV1_RTK_SYS_PMAP_10TH | DV1_RTK_SYS_PMAP_11TH | DV1_RTK_SYS_PMAP_12TH)
#define DV1_RTK_CPU_PMAP      ((uint32)1u << DV1_RTK_CPU_PORT)

typedef enum
{
    DV1_EM_USER_TO_SYS = 0,
    DV1_EM_SYS_TO_USER,
    DV1_EM_PMAP_COVERT_MODE_LAST
} dv1_RtkPmapCovertMode_e;

typedef uint32 dv1_RtkPmap;

extern const uint8 dv1_g_userPort2sysPort[DV1_RTL9075_INTERFACE_NUM];
extern const uint8 dv1_g_sysPort2userPort[DV1_RTL9075_INTERFACE_NUM];

void      dv1_drv_timer_init(void);
RtkApiRet dv1_drv_pmap_convert(RtkPmap srcPmap, RtkPmap *dstPmap, dv1_RtkPmapCovertMode_e mode);
RtkApiRet dv1_drv_memset(void *str, int32 val, size_t size);
RtkApiRet dv1_drv_memcpy(void *dst, const void *src, size_t size);
uint32    dv1_drv_tbl_fieldlen2mask(uint32 fieldlen);
#endif /* #ifndef DV1_ASICDRV_H_ */
