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

#ifndef CVA_ASICDRV_H_
#define CVA_ASICDRV_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"

#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
#define CVA_ASIC_INTERNAL
#else
#define CVA_ASIC_INTERNAL static
#endif

#define CVA_RTL9075_INTERFACE_NUM   (16u)
#define CVA_RTK_MAX_NUM_OF_SYS_PORT (11u)
#define CVA_RTK_SYS_PORT_ID_MAX     (12u)
#define CVA_RTK_PHY_ID_MAX          (11u)
#define CVA_RTK_INVALID_PORT_ID     (0xFFu)

#define CVA_RTK_SYS_PORT_1ST  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_1ST])
#define CVA_RTK_SYS_PORT_2ND  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_2ND])
#define CVA_RTK_SYS_PORT_3RD  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_3RD])
#define CVA_RTK_SYS_PORT_4TH  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_4TH])
#define CVA_RTK_SYS_PORT_5TH  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_5TH])
#define CVA_RTK_SYS_PORT_6TH  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_6TH])
#define CVA_RTK_SYS_PORT_7TH  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_7TH])
#define CVA_RTK_SYS_PORT_8TH  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_8TH])
#define CVA_RTK_SYS_PORT_9TH  (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_9TH])
#define CVA_RTK_SYS_PORT_10TH (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_10TH])
#define CVA_RTK_SYS_PORT_11TH (cva_g_userPort2sysPort[CVA_RTK_USER_PORT_11TH])
#define CVA_RTK_CPU_PORT      (15u)

#define CVA_RTK_SYS_PMAP_1ST  ((uint32)1u << CVA_RTK_SYS_PORT_1ST)
#define CVA_RTK_SYS_PMAP_2ND  ((uint32)1u << CVA_RTK_SYS_PORT_2ND)
#define CVA_RTK_SYS_PMAP_3RD  ((uint32)1u << CVA_RTK_SYS_PORT_3RD)
#define CVA_RTK_SYS_PMAP_4TH  ((uint32)1u << CVA_RTK_SYS_PORT_4TH)
#define CVA_RTK_SYS_PMAP_5TH  ((uint32)1u << CVA_RTK_SYS_PORT_5TH)
#define CVA_RTK_SYS_PMAP_6TH  ((uint32)1u << CVA_RTK_SYS_PORT_6TH)
#define CVA_RTK_SYS_PMAP_7TH  ((uint32)1u << CVA_RTK_SYS_PORT_7TH)
#define CVA_RTK_SYS_PMAP_8TH  ((uint32)1u << CVA_RTK_SYS_PORT_8TH)
#define CVA_RTK_SYS_PMAP_9TH  ((uint32)1u << CVA_RTK_SYS_PORT_9TH)
#define CVA_RTK_SYS_PMAP_10TH ((uint32)1u << CVA_RTK_SYS_PORT_10TH)
#define CVA_RTK_SYS_PMAP_11TH ((uint32)1u << CVA_RTK_SYS_PORT_11TH)
#define CVA_RTK_SYS_PMAP_ALL  (uint32)(CVA_RTK_SYS_PMAP_1ST | CVA_RTK_SYS_PMAP_2ND | CVA_RTK_SYS_PMAP_3RD | CVA_RTK_SYS_PMAP_4TH | CVA_RTK_SYS_PMAP_5TH | CVA_RTK_SYS_PMAP_6TH | CVA_RTK_SYS_PMAP_7TH | CVA_RTK_SYS_PMAP_8TH | CVA_RTK_SYS_PMAP_9TH | CVA_RTK_SYS_PMAP_10TH | CVA_RTK_SYS_PMAP_11TH)
#define CVA_RTK_CPU_PMAP      ((uint32)1u << CVA_RTK_CPU_PORT)

typedef enum
{
    CVA_EM_USER_TO_SYS = 0,
    CVA_EM_SYS_TO_USER,
    CVA_EM_PMAP_COVERT_MODE_LAST
} cva_RtkPmapCovertMode_e;

typedef uint32 cva_RtkPmap;

extern const uint8 cva_g_userPort2sysPort[CVA_RTL9075_INTERFACE_NUM];
extern const uint8 cva_g_sysPort2userPort[CVA_RTL9075_INTERFACE_NUM];

void      cva_drv_timer_init(void);
RtkApiRet cva_drv_pmap_convert(cva_RtkPmap srcPmap, cva_RtkPmap *dstPmap, cva_RtkPmapCovertMode_e mode);
RtkApiRet cva_drv_drv_memset(void *str, int32 val, size_t size);
RtkApiRet cva_drv_drv_memcpy(void *dst, const void *src, size_t size);
uint32    cva_drv_tbl_fieldlen2mask(uint32 fieldlen);
#endif /* #ifndef CVA_ASICDRV_H_ */
