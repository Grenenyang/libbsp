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
#include "rtl907xd_v1_api_phy_test.h"
#endif

RtkApiRet dv1_set_phytest_1g_tm1_ckm_jts(void);
RtkApiRet dv1_set_phytest_1g_tm1_cks_jtm(void);
RtkApiRet dv1_set_phytest_1g_tm4(void);
RtkApiRet dv1_set_phytest_1g_tm2(void);
RtkApiRet dv1_set_phytest_1g_tm6(void);
RtkApiRet dv1_set_phytest_100_tm1(void);
RtkApiRet dv1_set_phytest_100_tm5(void);
RtkApiRet dv1_set_phytest_100_tm4(void);
RtkApiRet dv1_set_phytest_100_tm0(void);

/**
 * @addtogroup PHY_TEST_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907DV1_PHYTEST_EXAMPLE RTL907XD_V1 PHY Test Example
 * @{
 */
/**
 * @defgroup RTL907DV1_PTM_EXAMPLE UNH IOL Test Example
 * @{
 */
/**
 * @defgroup RTL907DV1_PTM_EXAMPLE_1000 UNH IOL Test - 1000BASE-T1
 * @brief UNH IOL Test examples of 1000BASE-T1 ports.
 *
 * @section Purpose
 * Demonstrates how to set and get the UNH IOL test mode of 1000BASE-T1 ports.
 *
 * @section Description
 * There are 5 examples: \n
 * - dv1_set_phytest_1g_tm1_ckm_jts(): let port 1 be in test mode 1 with slave jitter selected; \n
 * - dv1_set_phytest_1g_tm1_cks_jtm(): let port 2 be in test mode 1 with master jitter selected; \n
 * - dv1_set_phytest_1g_tm4(): let port 3 be in test mode 4; \n
 * - dv1_set_phytest_1g_tm2(): let port 4 be in test mode 2; \n
 * - dv1_set_phytest_1g_tm6(): let port 5 be in test mode 6;
 *
 * @{
 */
RtkApiRet dv1_set_phytest_1g_tm1_ckm_jts(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 1, 1000Base-T1, test mode 1, slave jitter select */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_1ST;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
        tmSet.testMode = 1u;
        tmSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 1 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_1ST;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. TX_TCLK is on ATP and ATN for slave transmit clock jitter test;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_set_phytest_1g_tm1_cks_jtm(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 2, 1000Base-T1, test mode 1, master jitter select */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_2ND;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
        tmSet.testMode = 1u;
        tmSet.isMaster = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 2 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_2ND;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
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

RtkApiRet dv1_set_phytest_1g_tm4(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet;
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 3, 1000Base-T1, test mode 4 */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_3RD;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
        tmSet.testMode = 4u;
        tmSet.isMaster = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 3 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_3RD;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
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

RtkApiRet dv1_set_phytest_1g_tm2(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet = {0u};
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 4, 1000Base-T1, test mode 2 */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_4TH;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
        tmSet.testMode = 2u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 4 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_4TH;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in master transmit MDI jitter test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_set_phytest_1g_tm6(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet = {0u};
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 5, 1000Base-T1, test mode 6 */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_5TH;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
        tmSet.testMode = 6u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 5 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_5TH;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_1000M;
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
/**@}*/ /* RTL907DV1_PTM_EXAMPLE_1000 */

/**
 * @defgroup RTL907DV1_PTM_EXAMPLE_100 UNH IOL Test - 100BASE-T1
 * @brief UNH IOL Test examples of 100BASE-T1 ports.
 *
 * @section Purpose
 * Demonstrates how to set and get the UNH IOL test mode of 100BASE-T1 ports.
 *
 * @section Description
 * There are 4 examples: \n
 * - dv1_set_phytest_100_tm1(): let port 1 be in test mode 1; \n
 * - dv1_set_phytest_100_tm5(): let port 5 be in test mode 5; \n
 * - dv1_set_phytest_100_tm4(): let port 2 be in test mode 4; \n
 * - dv1_set_phytest_100_tm0(): let port 6 be in test mode 0;
 *
 * @{
 */
RtkApiRet dv1_set_phytest_100_tm1(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet = {0u};
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 1, 100Base-T1, test mode 1 */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_1ST;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
        tmSet.testMode = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 1 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_1ST;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in transmit droop test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_set_phytest_100_tm5(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet = {0u};
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 5, 100Base-T1, test mode 5 */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_5TH;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
        tmSet.testMode = 5u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 5 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_5TH;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
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

RtkApiRet dv1_set_phytest_100_tm4(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet = {0u};
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 2, 100Base-T1, test mode 4 */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_2ND;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
        tmSet.testMode = 4u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 2 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_2ND;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
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

RtkApiRet dv1_set_phytest_100_tm0(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        uintChip;
    PhyTestModeInfo_t tmSet = {0u};
    PhyTestModeInfo_t tmGet;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Configure port 6, 100Base-T1, test mode 0 */
        tmSet.port     = (uint8)DV1_RTK_USER_PORT_6TH;
        tmSet.speed    = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
        tmSet.testMode = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_set(uintChip, &tmSet));

        /* Step 2. Get port 6 test mode */
        tmGet.port  = (uint8)DV1_RTK_USER_PORT_6TH;
        tmGet.speed = (uint8)DV1_PHY_TESTMODE_SPEED_100M;
        CHK_FUN_CONTINUE(ret, rtk_phy_testmode_get(uintChip, &tmGet));

        /* Expected Behavior:
         *     1. the obtained test mode is expected;
         *     2. be in slave transmit timing jitter test mode;
         */
        if(tmGet.testMode != tmSet.testMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907DV1_PTM_EXAMPLE_100 */

/**@}*/ /* RTL907DV1_PTM_EXAMPLE */
/**@}*/ /* RTL907DV1_PHYTEST_EXAMPLE */
/**@}*/ /* PHY_TEST_EXAMPLE */