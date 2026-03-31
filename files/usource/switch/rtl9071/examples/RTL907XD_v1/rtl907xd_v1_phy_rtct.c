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

#include "rtkas_api_ext.h"
#include "rtkas_api_phy_test.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_reg_struct.h"
#endif

RtkApiRet dv1_get_phy_rtct(void);

/**
 * @addtogroup RTL907DV1_PHYTEST_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907DV1_RTCT_EXAMPLE RTCT Configuration Example
 * @brief Demonstrates how to use APIs for RTCT configuration.
 *
 * @section Purpose
 * This example demonstrates how to trigger the RTCT process and get the result.
 *
 * @section Description
 * Call rtk_rtct_run_cable_diag() to trigger the RTCT process for port 1; \n
 * Wait for at least 5 milliseconds; \n
 * Call rtk_rtct_get_cable_status() to obtain the result.
 *
 * @{
 */
RtkApiRet dv1_get_phy_rtct(void)
{
    RtkApiRet            retVal      = RT_ERR_OK;
    uint16               rtctTimeCnt = 0u;
    UnitChip_t           uintChip;
    RTCTInfo_t           rtctInfo;
    RtkRtctCableStatus_e expSta = EM_RTK_RTCT_SHORT;
    uintChip.unit               = DV1_UNIT;
    uintChip.chip               = CHIP_907XD_V1;
    rtctInfo.port               = (RtkRtctPortNumber_e)EM_RTK_RTCT_PORT1;
    rtctInfo.status             = (RtkRtctCableStatus_e)EM_RTK_RTCT_NORMAL;

    do
    {
        /* Step 1. Start to run the cable diagnostic of port 1 */
        CHK_FUN_CONTINUE(retVal, rtk_rtct_run_cable_diag(uintChip, &rtctInfo));

        /* Step 2. Wait for the RTCT result to be ready, the minimum waiting time is 5ms */
        do
        {
            rtctTimeCnt++;
            /* delay 5us */
            (void)rtk_usecond_delay(5u);
        } while(rtctTimeCnt < RTK_TIMER_RTCT_TIMEOUT_COUNT_NUM);

        /* Step 3. Get the cable status of port 1 */
        CHK_FUN_CONTINUE(retVal, rtk_rtct_get_cable_status(uintChip, &rtctInfo));

        /* Expected Behavior: The cable status should be normal/short/open,
         * and the defect location should be non-zero when the cable status is not normal.
         */
        if(expSta != rtctInfo.status)
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907DV1_RTCT_EXAMPLE */
/**@}*/ /* RTL907DV1_PHYTEST_EXAMPLE */
