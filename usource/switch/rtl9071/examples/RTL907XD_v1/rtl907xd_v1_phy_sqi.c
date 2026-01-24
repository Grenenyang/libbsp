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

RtkApiRet dv1_get_phy_sqi_ex_100(void);
RtkApiRet dv1_get_phy_sqi_ex_1g(void);

/**
 * @addtogroup RTL907DV1_PHYTEST_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907DV1_SQI_EXAMPLE SQI Example
 * @brief SQI examples of the 100/1000BASE-T1 ports.
 * @{
 */
/**
 * @defgroup DV1_SQI_EXAM_100 SQI Example - 100BASE-T1 Ports
 * @brief SQI example of 100BASE-T1 ports.
 *
 * @section Purpose
 * Demonstrates how to obtain the SQI of the 100BASE-T1 port.
 *
 * @section Description
 * Let port 1 and port 5 be 100BASE-T1 ports; \n
 * Let these 2 ports be linked up; \n
 * Call rtk_port_sqi_get() to obtain SQIs; \n
 * These SQI results should both be 7.
 *
 * @{
 */
RtkApiRet dv1_get_phy_sqi_ex_100(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   uintChip;
    SqiGetInfo_t sqiGet1, sqiGet2;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Get the SQI of 100Base-T1 port 1 */
        sqiGet1.port  = (RtkPort)DV1_RTK_USER_PORT_1ST;
        sqiGet1.speed = (uint8)0u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet1));

        /* Step 2. Get the SQI of 100Base-T1 port 5 */
        sqiGet2.port  = (RtkPort)DV1_RTK_USER_PORT_5TH;
        sqiGet2.speed = (uint8)0u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet2));

        /* Expected Behavior: The SQI value should be between 7 and 0;
         * 0 is the worst, and 7 is the best.
         */
        if((7u != sqiGet1.sqi) || (7u != sqiGet2.sqi))
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* DV1_SQI_EXAM_100 */

/**
 * @defgroup DV1_SQI_EXAM_1000 SQI Example - 1000BASE-T1 Ports
 * @brief SQI example of 1000BASE-T1 ports.
 *
 * @section Purpose
 * Demonstrates how to obtain the SQI of the 1000BASE-T1 port.
 *
 * @section Description
 * Let port 2 and port 6 be 1000BASE-T1 ports; \n
 * Let these 2 ports be linked up; \n
 * Call rtk_port_sqi_get() to obtain SQIs; \n
 * These SQI results should both be 7.
 *
 * @{
 */
RtkApiRet dv1_get_phy_sqi_ex_1g(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   uintChip;
    SqiGetInfo_t sqiGet1, sqiGet2;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Get the SQI of 1000Base-T1 port 2 */
        sqiGet1.port  = (RtkPort)DV1_RTK_USER_PORT_2ND;
        sqiGet1.speed = (uint8)1u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet1));

        /* Step 2. Get the SQI of 1000Base-T1 port 6 */
        sqiGet2.port  = (RtkPort)DV1_RTK_USER_PORT_6TH;
        sqiGet2.speed = (uint8)1u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(uintChip, &sqiGet2));

        /* Expected Behavior: The SQI value should be between 7 and 0;
         * 0 is the worst, and 7 is the best.
         */
        if((7u != sqiGet1.sqi) || (7u != sqiGet2.sqi))
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* DV1_SQI_EXAM_1000 */
/**@}*/ /* RTL907DV1_SQI_EXAMPLE */
/**@}*/ /* RTL907DV1_PHYTEST_EXAMPLE */