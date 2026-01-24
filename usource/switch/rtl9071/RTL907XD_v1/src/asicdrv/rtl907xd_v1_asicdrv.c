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

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_timer.h"

#include "rtl907xd_v1_api_system.h"
#include "rtkas_types.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_table_struct.h"
#include "rtl907xd_v1_asicdrv.h"
#include "rtl907xd_v1_asicdrv_lut.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#include "rtl907xd_v1_asicdrv_otp.h"
#include "rtl907xd_v1_asicdrv_ptp.h"
#include "rtl907xd_v1_asicdrv_vlan.h"
#include "rtl907xd_v1_asicdrv_reset.h"
#include "rtl907xd_v1_asicdrv_serdes.h"

/* port mapping */
#ifdef AUTOSAR_PORT
const uint8 dv1_g_userPort2sysPort[DVA_RTL9075_INTERFACE_NUM] =
    {1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 0xFFu, 0xFFu, 0xFFu, 15u};

const uint8 dv1_g_sysPort2userPort[DVA_RTL9075_INTERFACE_NUM] =
    {0xFFu, 0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 0xFFu, 0xFFu, 15u};
#else
const uint8 dv1_g_userPort2sysPort[DV1_RTL9075_INTERFACE_NUM] =
    {0xFFu, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 0xFFu, 0xFFu, 15u};

const uint8 dv1_g_sysPort2userPort[DV1_RTL9075_INTERFACE_NUM] =
    {0xFFu, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 0xFFu, 0xFFu, 15u};
#endif
/**
 * @brief Covert user pmap <-> sys pmap for RL6760
 *
 * @param srcPmap
 * @param dstPmap
 * @param mode
 * @retval RtkApiRet
 */
RtkApiRet dv1_drv_pmap_convert(RtkPmap srcPmap, RtkPmap *dstPmap, dv1_RtkPmapCovertMode_e mode)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPmap   tmpPmap = 0u;

    do
    {
        if(NULL == dstPmap)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(DV1_EM_USER_TO_SYS == mode)
        {
            if((srcPmap & (DV1_RTK_USER_PMAP_ALL | DV1_RTK_CPU_PMAP)) != srcPmap)
            {
                ret = -RT_ERR_INPUT;
                continue;
            }

            tmpPmap = (srcPmap & DV1_RTK_USER_PMAP_ALL);
        }
        else if(DV1_EM_SYS_TO_USER == mode)
        {
            if((srcPmap & (DV1_RTK_SYS_PMAP_ALL | DV1_RTK_CPU_PMAP)) != srcPmap)
            {
                ret = -RT_ERR_INPUT;
                continue;
            }

            tmpPmap = (srcPmap & DV1_RTK_SYS_PMAP_ALL);
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        if(DV1_RTK_CPU_PMAP == (srcPmap & DV1_RTK_CPU_PMAP))
        {
            tmpPmap |= DV1_RTK_CPU_PMAP;
        }

        *dstPmap = tmpPmap;

    } while(1u == 0u);

    return ret;
}

/**
 * @brief
 *
 */
void dv1_drv_timer_init(void)
{
#ifdef RTK_MODULE_PTP
    dv1_g_ptpTimeConfigTimeout       = PTP_TIME_DIRECT_GET_TIMEOUT;
    dv1_g_ptpControlClkConfigTimeout = PTP_TIME_DIRECT_SET_TIMEOUT;
    dv1_g_ptpConfigMultipleTimeout   = PTP_TIME_GET_TIMEOUT;
    dv1_g_ptpConfigTimeout           = PTP_TIME_SET_TIMEOUT;
    dv1_g_ptpPortTXRXTSTimeout       = PTP_PORT_TX_RX_GET_TIMEOUT;
    dv1_g_ptpTaiConfigTimeout        = PTP_TAI_GET_TIME_TIMEOUT;
#endif

#ifdef RTK_MODULE_SYSTEM
    dv1_g_otpTimeoutCnt = RTK_TIMER_OTP_INDIRECT_CHANNEL;
#endif

#ifdef RTK_MODULE_VLAN
    dv1_g_vlanTime = VLAN_FLUSH_TIMEOUT;
#endif

#if 0
    g_psfpTimeoutCounter = PSFP_CONFIG_CHANGE_TIMEOUT;
#endif

#ifdef RTK_MODULE_L2
    dv1_g_lutTime = LUT_SEARCH_TIMEOUT;
#endif

#ifdef RTK_MODULE_STACKING
    dv1_g_stackingTimeout = STACKING_TIMEOUT;
#endif
}

RtkApiRet dv1_drv_memset(void *str, int32 val, size_t size)
{
    RtkApiRet ret = RT_ERR_OK;

    if(str != rtl_memset(str, val, size))
    {
        ret = -RT_ERR_MEM_ACCESS;
    }

    return ret;
}

RtkApiRet dv1_drv_memcpy(void *dst, const void *src, size_t size)
{
    RtkApiRet ret = RT_ERR_OK;

    if(dst != rtl_memcpy(dst, src, size))
    {
        ret = -RT_ERR_MEM_ACCESS;
    }

    return ret;
}

uint32 dv1_drv_tbl_fieldlen2mask(uint32 fieldlen)
{
    uint32 mask = 0u;
    uint32 len  = 0u;

    if(fieldlen > DV1_REG_DATA_LEN)
    {
        len = DV1_REG_DATA_LEN;
    }
    else
    {
        len = fieldlen;
    }
    mask = (uint32)((((uint64)1u << len) - 1u) & 0xFFFFFFFFu);
    return mask;
}
