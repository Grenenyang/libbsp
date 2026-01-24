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

#if defined(RTL_907XC_VA)
#include "rtl907xc_va_reg_struct.h"
#endif

RtkApiRet cva_rtk_get_phy_sqi(void);

/**
 * @addtogroup RTL907CVA_PHYTEST_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907CVA_SQI_EXAMPLE SQI Example
 * @brief SQI example of the 10BASE-T1S port and the 100/1000BASE-T1 port.
 *
 * @section Purpose
 * Demonstrates how to obtain SQIs of ports which are 10BASE-T1S, and 100/1000BASE-T1.
 *
 * @section Description
 * Configure port 1 as a 10BASE-T1S port and link it up; \n
 * Configure port 2 as a 100BASE-T1 port and link it up; \n
 * Configure port 5 as a 100BASE-T1 port and link it up; \n
 * Configure port 6 as a 1000BASE-T1 port and link it up; \n
 * Call rtk_port_sqi_get() to obtain SQIs of these ports; \n
 * These SQI results should all be 7.
 *
 * @{
 */
RtkApiRet cva_rtk_get_phy_sqi(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   uintChip;
    SqiGetInfo_t sqiGet1, sqiGet2, sqiGet3, sqiGet4;
    uintChip.unit = CVA_UNIT;
    uintChip.chip = CHIP_907XC_VA;

    do
    {
        /* Step 1. Get the SQI of 10Base-T1S port 1 */
        sqiGet1.port  = (RtkPort)CVA_RTK_USER_PORT_1ST;
        sqiGet1.speed = (uint8)0u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet1));

        /* Step 2. Get the SQI of 100Base-T1 port 2 */
        sqiGet2.port  = (RtkPort)CVA_RTK_USER_PORT_2ND;
        sqiGet2.speed = (uint8)1u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet2));

        /* Step 3. Get the SQI of 100Base-T1 port 5 */
        sqiGet3.port  = (RtkPort)CVA_RTK_USER_PORT_5TH;
        sqiGet3.speed = (uint8)0u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet3));

        /* Step 4. Get the SQI of 1000Base-T1 port 6 */
        sqiGet4.port  = (RtkPort)CVA_RTK_USER_PORT_6TH;
        sqiGet4.speed = (uint8)1u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet4));

        /* Expected Behavior: The SQI value should be between 7 and 0;
         * 0 is the worst, and 7 is the best.
         */
        if((7u != sqiGet1.sqi) || (7u != sqiGet2.sqi) || (7u != sqiGet3.sqi) || (7u != sqiGet4.sqi))
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907CVA_SQI_EXAMPLE */
/**@}*/ /* RTL907CVA_PHYTEST_EXAMPLE */