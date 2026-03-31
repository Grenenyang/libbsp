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

#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_api_phy_test.h"
#endif

RtkApiRet cvb_rtk_set_phy_testmode1_1g_master(void);
RtkApiRet cvb_rtk_set_phy_testmode4_1g_slave(void);
RtkApiRet cvb_rtk_set_phy_testmode6_1g(void);
RtkApiRet cvb_rtk_set_phy_testmode2_comb_100(void);
RtkApiRet cvb_rtk_set_phy_testmode4_comb_100(void);
RtkApiRet cvb_rtk_set_phy_testmode2_100(void);
RtkApiRet cvb_rtk_set_phy_testmode4_100(void);
RtkApiRet cvb_rtk_set_phy_testmode3_10(void);

/**
 * @addtogroup PHY_TEST_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907CVB_PHYTEST_EXAMPLE RTL907XC_VB PHY Test Example
 * @{
 */
/**
 * @defgroup RTL907CVB_PTM_EXAMPLE UNH IOL Test Mode Example
 * @{
 */
/**
 * @defgroup RTL907CVB_PTM_EXAMPLE_1000 UNH IOL Test - 1000BASE-T1
 * @brief UNH IOL Test examples of 1000BASE-T1 ports.
 *
 * @section Purpose
 * Demonstrates how to set and get the UNH IOL test mode of 1000BASE-T1 ports.
 *
 * @section Description
 * There are 3 examples: \n
 * - cvb_rtk_set_phy_testmode1_1g_master(): let port 1 be in test mode 1 with master jitter selected; \n
 * - cvb_rtk_set_phy_testmode4_1g_slave(): let port 5 be in test mode 4; \n
 * - cvb_rtk_set_phy_testmode6_1g(): let port 6 be in test mode 6;
 *
 * @{
 */
RtkApiRet cvb_rtk_set_phy_testmode1_1g_master(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Configure port 6 as 1000Base-T1, test mode 1, master jitter select */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_6TH;
        tmSet.speed    = (uint8)CVB_PHYTEST_RTP_1000M;
        tmSet.testMode = 1u;
        tmSet.isMaster = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 6 test mode */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_6TH;
        tmGet.speed = (uint8)CVB_PHYTEST_RTP_1000M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. TX_TCLK is on ATP and ATN for master transmit clock jitter test;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_rtk_set_phy_testmode4_1g_slave(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Configure port 5 as 1000Base-T1, test mode 4 */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_5TH;
        tmSet.speed    = (uint8)CVB_PHYTEST_RTP_1000M;
        tmSet.testMode = 4u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 5 test mode */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_5TH;
        tmGet.speed = (uint8)CVB_PHYTEST_RTP_1000M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in transmit distortion test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_rtk_set_phy_testmode6_1g(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Configure port 6 as 1000Base-T1, test mode 6 */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_6TH;
        tmSet.speed    = (uint8)CVB_PHYTEST_RTP_1000M;
        tmSet.testMode = 6u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 6 test mode */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_6TH;
        tmGet.speed = (uint8)CVB_PHYTEST_RTP_1000M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in transmitter droop test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907CVB_PTM_EXAMPLE_1000 */

/**
 * @defgroup RTL907CVB_PTM_EXAMPLE_100 UNH IOL Test - 100BASE-T1
 * @brief UNH IOL Test examples of 100BASE-T1 ports.
 *
 * @section Purpose
 * Demonstrates how to set and get the UNH IOL test mode of 100BASE-T1 ports.
 *
 * @section Description
 * There are 4 examples: \n
 * - cvb_rtk_set_phy_testmode2_comb_100(): let port 5 be in test mode 2; \n
 * - cvb_rtk_set_phy_testmode4_comb_100(): let port 6 be in test mode 4; \n
 * - cvb_rtk_set_phy_testmode2_100(): let port 1 be in test mode 2; \n
 * - cvb_rtk_set_phy_testmode4_100(): let port 2 be in test mode 4;
 *
 * @{
 */
RtkApiRet cvb_rtk_set_phy_testmode2_comb_100(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Set port 5 as 100Base-T1, test mode 2 */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_5TH;
        tmSet.speed    = (uint8)CVB_PHYTEST_RTP_100M;
        tmSet.testMode = 2u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 5 test mode */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_5TH;
        tmGet.speed = (uint8)CVB_PHYTEST_RTP_100M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. transmit jitter test in master mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_rtk_set_phy_testmode4_comb_100(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Set port 6 as 100Base-T1, test mode 4 */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_6TH;
        tmSet.speed    = (uint8)CVB_PHYTEST_RTP_100M;
        tmSet.testMode = 4u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 6 test mode */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_6TH;
        tmGet.speed = (uint8)CVB_PHYTEST_RTP_100M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in transmit distortion test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_rtk_set_phy_testmode2_100(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Set port 1 as 100Base-T1, test mode 2 */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_1ST;
        tmSet.speed    = (uint8)CVB_PHYTEST_BW_100M;
        tmSet.testMode = 2u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 1 test mode */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_1ST;
        tmGet.speed = (uint8)CVB_PHYTEST_BW_100M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behaviour:
         *     1. the obtained test mode is expected;
         *     2. transmit jitter test in master mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_rtk_set_phy_testmode4_100(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Set port 2 as 100Base-T1, test mode 4 */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_2ND;
        tmSet.speed    = (uint8)CVB_PHYTEST_BW_100M;
        tmSet.testMode = 4u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 2 test mode */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_2ND;
        tmGet.speed = (uint8)CVB_PHYTEST_BW_100M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in transmit distortion test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = RT_ERR_FAILED;
            continue;
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907CVB_PTM_EXAMPLE_100 */

/**
 * @defgroup RTL907CVB_PTM_EXAMPLE_10 UNH IOL Test - 10BASE-T1S
 * @brief UNH IOL Test examples of 10BASE-T1S ports.
 *
 * @section Purpose
 * Demonstrates how to set and get the UNH IOL test mode of 10BASE-T1S ports.
 *
 * @section Description
 * There is 1 example: \n
 * - cvb_rtk_set_phy_testmode3_10(): let port 3 be in test mode 3;
 *
 * @{
 */
RtkApiRet cvb_rtk_set_phy_testmode3_10(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Set port 3 as 10Base-T1S, test mode 3 */
        tmSet.port     = (uint8)CVB_RTK_USER_PORT_3RD;
        tmSet.speed    = (uint8)CVB_PHYTEST_BW_10M;
        tmSet.testMode = 3u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 3 as test mode 3 */
        tmGet.port  = (uint8)CVB_RTK_USER_PORT_3RD;
        tmGet.speed = (uint8)CVB_PHYTEST_BW_10M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in PSD mask test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907CVB_PTM_EXAMPLE_10 */

/**@}*/ /* RTL907CVB_PTM_EXAMPLE */
/**@}*/ /* RTL907CVB_PHYTEST_EXAMPLE */
/**@}*/ /* PHY_TEST_EXAMPLE */