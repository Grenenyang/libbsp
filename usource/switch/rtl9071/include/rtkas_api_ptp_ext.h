#ifndef RTKAS_API_PTP_EXT_H__
#define RTKAS_API_PTP_EXT_H__
#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_api_ptp.h"
#include "rtkas_types.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#endif

#define ANSI_COLOR_LIGHT_WHITE "\x1b" \
                               "[1;37m"
#define BG_COLOR_BLACK "\x1b" \
                       "[40m"
#define ANSI_COLOR_RESET "\x1b" \
                         "[0m"
#define PTPMSGCOLOR ANSI_COLOR_LIGHT_WHITE BG_COLOR_BLACK

#define DOMAIN_INDEX1 (0u)
#define DOMAIN_INDEX2 (1u)
#define DOMAIN_INDEX3 (2u)
#define DOMAIN_COUNTS (3u)

#if defined(RTL_907XD_V1)
#define DV1_RTK_USER_PMAP_1_2_3 (DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD)
#endif
#if defined(RTL_907XD_VA)
#define DVA_RTK_USER_PMAP_1_2_3 (DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD)
#endif
#if defined(RTL_907XC_VB)
#define CVB_RTK_USER_PMAP_1_2_3 (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD)
#endif
#if defined(RTL_907XC_VA)
#define CVA_RTK_USER_PMAP_1_2_3 (CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_2ND | CVA_RTK_USER_PMAP_3RD)
#endif

#endif /* RTKAS_API_PTP_EXT_H__ */