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

#if defined(RTL_907XD_VA)
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_phy.h"
#endif

RtkApiRet dva_get_phy_sqi_ex_100(void);
RtkApiRet dva_get_phy_sqi_ex_1g(void);
RtkApiRet dva_get_phy_mse(void);
RtkApiRet dva_get_phy_wc_mse(void);
RtkApiRet dva_get_phy_pmse(void);
RtkApiRet dva_get_phy_wc_pmse(void);
RtkApiRet dva_get_phy_ltt(void);
RtkApiRet dva_get_phy_lrt(void);
RtkApiRet dva_get_phy_rrt(void);
RtkApiRet dva_get_phy_lfl_loss(void);
RtkApiRet dva_get_phy_lfl_fail(void);

/**
 * @addtogroup RTL907DVA_PHYTEST_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907DVA_SQI_EXAMPLE SQIs Example
 * @{
 */

/**
 * @defgroup RTL907DVA_SQI_EXAMPLE_DCQ Dynamic Channel Quality Examples
 * @{
 */
/**
 * @defgroup MSE_EXAM Mean Square Error
 * @brief This example demonstrates how to use the API to get MSE.
 *
 * @section Purpose
 * Demonstrate how to get MSE measurement.
 *
 * @section Description
 * Use this API to get the MSE value of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_mse(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    MseGetInfo_t mseGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        mseGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain MSE */
        CHK_FUN_CONTINUE(retVal, rtk_port_mse_get(unitChip, &mseGetInfo));

        /* Expected Behavior:
         * When MSE is invalid, the MSE API should return -RT_ERR_FAILED;
         * When MSE is valid, the MSE API should return RT_ERR_OK, and the MSE value (0 to 511) is stored in mseGetInfo.value;
         */
        if((uint16)DVA_MSE_DATA_MAX < mseGetInfo.value)
        {
            retVal = -RT_ERR_OUT_OF_RANGE;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* MSE_EXAM */

/**
 * @defgroup WCMSE_EXAM Worst Case of Mean Square Error
 * @brief This example demonstrates how to use the API to get the worst case of MSE.
 *
 * @section Purpose
 * Demonstrate how to get the worst case of MSE measurement.
 *
 * @section Description
 * Use this API to get the worst case of MSE of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_wc_mse(void)
{
    RtkApiRet      retVal = RT_ERR_OK;
    UnitChip_t     unitChip;
    WcMseGetInfo_t wcMseGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        wcMseGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain the worst case of MSE */
        CHK_FUN_CONTINUE(retVal, rtk_port_wc_mse_get(unitChip, &wcMseGetInfo));

        /* Expected Behavior:
         * When WCMSE is invalid, the WCMSE API should return -RT_ERR_FAILED;
         * When WCMSE is valid, the WCMSE API should return RT_ERR_OK, and the WCMSE value (0 to 511) is stored in wcMseGetInfo.value;
         */
        if((uint16)DVA_WCMSE_DATA_MAX < wcMseGetInfo.value)
        {
            retVal = -RT_ERR_OUT_OF_RANGE;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* WCMSE_EXAM */

/**
 * @defgroup PMSE_EXAM Peak Mean Square Error
 * @brief This example demonstrates how to use the API to get the peak MSE.
 *
 * @section Purpose
 * Demonstrate how to get the peak MSE measurement.
 *
 * @section Description
 * Use this API to get the peak MSE of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_pmse(void)
{
    RtkApiRet        retVal = RT_ERR_OK;
    UnitChip_t       unitChip;
    PeakMseGetInfo_t pkMseGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        pkMseGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain the peak MSE */
        CHK_FUN_CONTINUE(retVal, rtk_port_peak_mse_get(unitChip, &pkMseGetInfo));

        /* Expected Behavior:
         * When PMSE is invalid, the PMSE API should return -RT_ERR_FAILED;
         * When PMSE is valid, the PMSE API should return RT_ERR_OK, and the PMSE value (0 to 63) is stored in pkMseGetInfo.value;
         */
        if((uint8)DVA_PMSE_DATA_MAX < pkMseGetInfo.value)
        {
            retVal = -RT_ERR_OUT_OF_RANGE;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PMSE_EXAM */

/**
 * @defgroup WCPMSE_EXAM Worst Case of Peak Mean Square Error
 * @brief This example demonstrates how to use the API to get the worst case of the peak MSE.
 *
 * @section Purpose
 * Demonstrate how to get the worst case of the peak MSE measurement.
 *
 * @section Description
 * Use this API to get the worst case of the peak MSE of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_wc_pmse(void)
{
    RtkApiRet          retVal = RT_ERR_OK;
    UnitChip_t         unitChip;
    WcPeakMseGetInfo_t wcPkMseGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        wcPkMseGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain the worst case of the peak MSE */
        CHK_FUN_CONTINUE(retVal, rtk_port_wc_peak_mse_get(unitChip, &wcPkMseGetInfo));

        /* Expected Behavior:
         * When WCPMSE is invalid, the WCPMSE API should return -RT_ERR_FAILED;
         * When WCPMSE is valid, the WCPMSE API should return RT_ERR_OK, and the WCPMSE value (0 to 63) is stored in wcPkMseGetInfo.value;
         */
        if((uint8)DVA_WCPMSE_DATA_MAX < wcPkMseGetInfo.value)
        {
            retVal = -RT_ERR_OUT_OF_RANGE;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* WCPMSE_EXAM */

/**
 * @defgroup SQI_EXAM Signal Quality Index
 * @brief SQI examples of the 100/1000BASE-T1 ports.
 * @{
 */
/**
 * @defgroup DVA_SQI_EXAM_100 SQI Example - 100BASE-T1 Ports
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
RtkApiRet dva_get_phy_sqi_ex_100(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    SqiGetInfo_t sqiGet1, sqiGet2;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Get the SQI of 100Base-T1 port 1 */
        sqiGet1.port  = (RtkPort)DVA_RTK_USER_PORT_1ST;
        sqiGet1.speed = (uint8)0u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(unitChip, &sqiGet1));

        /* Step 2. Get the SQI of 100Base-T1 port 5 */
        sqiGet2.port  = (RtkPort)DVA_RTK_USER_PORT_5TH;
        sqiGet2.speed = (uint8)0u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(unitChip, &sqiGet2));

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
/**@}*/ /* DVA_SQI_EXAM_100 */

/**
 * @defgroup DVA_SQI_EXAM_1000 SQI Example - 1000BASE-T1 Ports
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
RtkApiRet dva_get_phy_sqi_ex_1g(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    SqiGetInfo_t sqiGet1, sqiGet2;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Get the SQI of 1000Base-T1 port 2 */
        sqiGet1.port  = (RtkPort)DVA_RTK_USER_PORT_2ND;
        sqiGet1.speed = (uint8)1u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(unitChip, &sqiGet1));

        /* Step 2. Get the SQI of 1000Base-T1 port 6 */
        sqiGet2.port  = (RtkPort)DVA_RTK_USER_PORT_6TH;
        sqiGet2.speed = (uint8)1u;
        CHK_FUN_CONTINUE(retVal, rtk_port_sqi_get(unitChip, &sqiGet2));

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
/**@}*/ /* DVA_SQI_EXAM_1000 */
/**@}*/ /* SQI_EXAM */
/**@}*/ /* RTL907DVA_SQI_EXAMPLE_DCQ */

/**
 * @defgroup RTL907DVA_SQI_EXAMPLE_LQ Link Quality Examples
 * @{
 */
/**
 * @defgroup LTT_EXAM Link Training Time
 * @brief This example demonstrates how to use the API to get LTT.
 *
 * @section Purpose
 * Demonstrate how to get LTT measurement.
 *
 * @section Description
 * Use this API to get LTT of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_ltt(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    LttGetInfo_t lttGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        lttGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain LTT */
        CHK_FUN_CONTINUE(retVal, rtk_port_ltt_get(unitChip, &lttGetInfo));

        /* Expected Behavior:
         * The LTT API should return RT_ERR_OK, and the LTT value (0 to 0xFF) is stored in lttGetInfo.lttTime;
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* LTT_EXAM */

/**
 * @defgroup LRT_EXAM Local Receiver Time
 * @brief This example demonstrates how to use the API to get LRT.
 *
 * @section Purpose
 * Demonstrate how to get LRT measurement.
 *
 * @section Description
 * Use this API to get LRT of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_lrt(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    LrtGetInfo_t lrtGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        lrtGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain LRT */
        CHK_FUN_CONTINUE(retVal, rtk_port_lrt_get(unitChip, &lrtGetInfo));

        /* Expected Behavior:
         * The LRT API should return RT_ERR_OK, and the LRT value (0 to 0xFF) is stored in lrtGetInfo.lrtTime;
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* LRT_EXAM */

/**
 * @defgroup RRT_EXAM Remote Receiver Time
 * @brief This example demonstrates how to use the API to get RRT.
 *
 * @section Purpose
 * Demonstrate how to get RRT measurement.
 *
 * @section Description
 * Use this API to get RRT of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_rrt(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    RrtGetInfo_t rrtGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        rrtGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain RRT */
        CHK_FUN_CONTINUE(retVal, rtk_port_rrt_get(unitChip, &rrtGetInfo));

        /* Expected Behavior:
         * The RRT API should return RT_ERR_OK, and the RRT value (0 to 0xFF) is stored in rrtGetInfo.rrtTime;
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RRT_EXAM */

/**
 * @defgroup LFL_LOSS_EXAM Link Losses
 * @brief This example demonstrates how to use the API to get the link loss counter.
 *
 * @section Purpose
 * Demonstrate how to get the link loss counter.
 *
 * @section Description
 * Use this API to get the link loss counter of port 1 when port 1 links up.
 *
 * @{
 */
RtkApiRet dva_get_phy_lfl_loss(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    LflGetInfo_t lflGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        lflGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain the counter */
        CHK_FUN_CONTINUE(retVal, rtk_port_lfl_loss_get(unitChip, &lflGetInfo));

        /* Expected Behavior:
         * The API should return RT_ERR_OK;
         * And the link loss counter is stored in lflGetInfo.lossCnt;
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* LFL_LOSS_EXAM */

/**
 * @defgroup LFL_FAIL_EXAM Link Failures
 * @brief This example demonstrates how to use the API to get the link failure counter.
 *
 * @section Purpose
 * Demonstrate how to get the link failure counter.
 *
 * @section Description
 * Use this API to get the link failure counter of port 1 when port 1 is linked up and in 100BASE-T1 mode.
 *
 * @{
 */
RtkApiRet dva_get_phy_lfl_fail(void)
{
    RtkApiRet    retVal = RT_ERR_OK;
    UnitChip_t   unitChip;
    LflGetInfo_t lflGetInfo;
    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Set port 1 as the required port */
        lflGetInfo.port = (RtkPort)DVA_RTK_USER_PORT_1ST;

        /* Step 2. Use the function to obtain the counter */
        CHK_FUN_CONTINUE(retVal, rtk_port_lfl_fail_get(unitChip, &lflGetInfo));

        /* Expected Behavior:
         * The API should return RT_ERR_OK if the port is in 100BASE-T1 mode;
         * And the link failure counter is stored in lflGetInfo.failCnt;
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* LFL_FAIL_EXAM */
/**@}*/ /* RTL907DVA_SQI_EXAMPLE_LQ */

/**@}*/ /* RTL907DVA_SQI_EXAMPLE */
/**@}*/ /* RTL907DVA_PHYTEST_EXAMPLE */