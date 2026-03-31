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

#include "rtl907xc_vb_asicdrv_phy.h"
#include "rtl907xc_vb_api_ext.h"

/**
 * @addtogroup RTK_MODULE_PHY_TEST_MODE PHY Test
 * @{
 */
/**
 * @addtogroup RTCT
 * @{
 */
/**
 * @addtogroup RTCT_ASIC Low Level Driver
 * @{
 */

/**
 * @brief Start to run RTK cable diagnostic.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port index (EM_RTK_RTCT_PORT1 to EM_RTK_RTCT_PORT6) \n
 *                 Select which port to perform RTCT to get the cable status from the switch port.
 * @param[out] None
 * @return RT_ERR_OK     The acceptable frame type of the port was set.
 * @return -RT_ERR_INPUT The port index was illegal.
 * @return Others        Please refer to cvb_drv_userport_to_sysport() and cvb_drv_valid_phytest_userport_check() \n
 *                       in rtl907xc_vb_asicdrv_phy.c. \n
 *                       Please refer to cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_rtct_run_cable_diag(uint32 unit, RtkRtctPortNumber_e port)
{
    RtkApiRet   retVal                              = (RtkApiRet)RT_ERR_OK;
    uint32      sysPort                             = 0u;
    uint8       userPort                            = 0u;
    const uint8 rtctToUserPort[EM_RTK_RTCT_PORTMAX] = {0xFFu, CVB_RTK_USER_PORT_1ST, CVB_RTK_USER_PORT_2ND, CVB_RTK_USER_PORT_3RD,
                                                       CVB_RTK_USER_PORT_4TH, CVB_RTK_USER_PORT_5TH, CVB_RTK_USER_PORT_6TH};

    do
    {
        /* Check if the port is valid */
        if((EM_RTK_RTCT_PORTMAX <= port) || ((uint8)0xFFu == rtctToUserPort[port]))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Assign RTCT port index to userPort */
        userPort = rtctToUserPort[port];
        /* Translate the user port to the system port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Check if the system port is the valid phytest port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_valid_phytest_userport_check(userPort));

        /* Trigger RTCT */
        CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_RTCT_CTL) << 1u, 0x1u));
        /* Wait until
           the valid port index is checked, and RTCT is triggered : */
        /* while loop : 0 is 1 */
    } while(0u == 1u);
    /* return retVal */
    return retVal;
}

/**
 * @brief Get RTK cable diagnostic result.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port index (EM_RTK_RTCT_PORT1 to EM_RTK_RTCT_PORT6) \n
 *                 Select which port to perform RTCT to get the cable status from the switch port.
 * @param[out] rtkRtctCableStatus Get cable status (0 to 3) \n
 *                                Including the state of normal/open/short \n
 *                                0: Normal \n
 *                                1: Open \n
 *                                2: Short \n
 *                                3: RTCT progress fail
 * @param[out] rtkRtctCableDefect Returned the cable open short or open location (0x0 to 0xFFFF) \n
 *                                For port 1~4 (10/100Base), transforming the hexadecimal value into decimal value then dividing by 80 \n
 *                                For port 5/6 (100/1000Base), transforming the hexadecimal value into decimal value then dividing by 7.5 \n
 *                                The result is cable open/short location (in meters) \n
 *                                Note that if cable status is Normal, this value will always be 0
 * @return RT_ERR_OK              The acceptable frame type of the port was set.
 * @return -RT_ERR_FAILED         The RTCT result was not ready.
 * @return -RT_ERR_NULL_POINTER   The pointer is NULL.
 * @return Others                 Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c. \n
 *                                Please refer to \n
 *                                cvb_drv_userport_to_sysport(), cvb_drv_valid_phytest_userport_check(), and cvb_drv_rtct_status_get() \n
 *                                in rtl907xc_vb_asicdrv_phy.c.
 */
RtkApiRet cvb_rtct_get_cable_status(uint32                unit,
                                    RtkRtctPortNumber_e   port,
                                    RtkRtctCableStatus_e *rtkRtctCableStatus,
                                    RtkRtctCableDefect   *rtkRtctCableDefect)
{
    uint32                 regData                             = 0;
    uint32                 cableStatus                         = 0;
    uint32                 cableDefect                         = 0;
    uint32                 sysPort                             = 0u;
    RtkApiRet              retVal                              = (RtkApiRet)RT_ERR_OK;
    uint8                  rtctIndx                            = 0u;
    uint8                  userPort                            = 0u;
    const uint8            rtctToUserPort[EM_RTK_RTCT_PORTMAX] = {0xFFu, CVB_RTK_USER_PORT_1ST, CVB_RTK_USER_PORT_2ND, CVB_RTK_USER_PORT_3RD,
                                                                  CVB_RTK_USER_PORT_4TH, CVB_RTK_USER_PORT_5TH, CVB_RTK_USER_PORT_6TH};
    const RtkRtctCompare_t rtkRtctArray[RTK_RTCT_ARRAY]        = {{0x6000u, EM_RTK_RTCT_NORMAL}, {0x4800u, EM_RTK_RTCT_OPEN}, {0x5000u, EM_RTK_RTCT_SHORT}};

    do
    {
        /* Check if the port is valid */
        if((EM_RTK_RTCT_PORTMAX <= port) || ((uint8)0xFFu == rtctToUserPort[port]))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Check null pointers: check if (rtkRtctCableStatus is null) or (rtkRtctCableDefect is null) */
        if((NULL == rtkRtctCableStatus) || (NULL == rtkRtctCableDefect))
        {
            /* This situation indicates the null pointer exists */
            /* Assign -RT_ERR_NULL_POINTER(=8) to retVal */
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            /* Leave the do-while loop */
            continue;
        }

        /* Initialize the input parameters */
        *rtkRtctCableStatus = EM_RTK_RTCT_ERROR;
        *rtkRtctCableDefect = 0;
        /* Assign RTCT port index to userPort */
        userPort = rtctToUserPort[port];
        /* Translate the user port to the system port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Check if the system port is the valid phytest port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_valid_phytest_userport_check(userPort));

        /* Read RTCT register to check if the result of RTK cable diag is ready */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_RTCT_CTL) << 1u, &regData));
        /* Check RTCT ready bit: check if the 15th bit of regData is set */
        if(0u != (regData & 0x8000u))
        {
            /* This situation indicates RTCT result is ready */
            /* Get the cable status and defect in meters */
            CHK_FUN_CONTINUE(retVal, cvb_drv_rtct_status_get(unit, cvb_g_sysPortToOcpAddr[sysPort], &cableStatus, &cableDefect));
            /* Compare the cable status value with the level in rtct array, find the matched status result,
               and write back the cable defect location: */
            /* For loop: rtctIndx = 0; rtctIndx < 3; ++rtctIndx */
            for(rtctIndx = 0u; rtctIndx < CVB_RTK_RTCT_ARRAY; ++rtctIndx)
            {
                /* Check RTCT defect location: check if the result fits the level in rtkRtctArray */
                if(rtkRtctArray[rtctIndx].level == (uint16)(cableStatus & 0xFF00u))
                {
                    /* Write back the rtkRtctArray result to the input parameter */
                    *rtkRtctCableStatus = rtkRtctArray[rtctIndx].result;
                    /* Write back the defect location to the input parameter */
                    *rtkRtctCableDefect = cableDefect;
                }
            }
        }
        else
        {
            /* This situation indicates RTCT result is not ready */
            /* Assign -RT_ERR_FAILED(=1) to retVal */
            retVal = -(RtkApiRet)RT_ERR_FAILED;
            /* Leave the do-while loop */
            continue;
        }
        /* Wait until
           the valid port index is checked,
           and the polling of the ready bit of RTCT is done: */
        /* while loop: 0 is 1 */
    } while(0u == 1u);
    /* Return retVal */
    return retVal;
}
/**@}*/ /* RTCT_ASIC */
/**@}*/ /* RTCT */

/**
 * @addtogroup PTM
 * @{
 */
/**
 * @addtogroup PTM_ASIC Low Level Driver
 * @{
 */

/**
 * @brief Set test mode status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH)
 * @param[in] testMode Test mode \n
 *                     for 10Base-T1S, 0/1/2/3/ \n
 *                     for 100Base-T1, 0/1/2/4/5 \n
 *                     for 1000Base-T1, 0/1/2/4/5/6/7
 * @param[in] speed 10Base-T1S, 100Base-T1 or 1000Base-T1 \n
 *                  for Port 1-4, 0: 10Base-T1S, 1: 100Base-T1 \n
 *                  for Port 5/6, 0: 100Base-T1, 1: 1000Base-T1 \n
 * @param[in] isMaster
 *            0: slave
 *            1: master
 * @param[out] None
 * @retval RT_ERR_OK       The acceptable frame type of the port was set.
 * @retval -RT_ERR_PORT_ID The port was out of range.
 * @retval -RT_ERR_INPUT   The speed or isMaster was illegal.
 * @retval Others          Please refer to cvb_drv_phy_10base_testmode_set() \n
 *                                         cvb_drv_phy_100base_testmode_set() \n
 *                                         cvb_drv_phy_1000base_testmode_set() \n
 *                         in rtl907xc_vb_asicdrv_phy.c.
 */
RtkApiRet cvb_phy_testmode_set(uint32 unit, uint8 port, uint8 const testMode,
                               uint8 speed, uint8 isMaster)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    userPort = (uint32)port;

    do
    {
        if((1u < speed) || (1u < isMaster))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_drv_valid_phytest_userport_check(port));

        if((uint8)CVB_RTK_USER_PORT_4TH >= port)
        {
            if(0u == speed)
            {
                /* Set testMode for port 1-4 in 10Base speed */
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_10base_testmode_set(unit, userPort, testMode));
            }
            else
            {
                /* Set testMode for port 1-4 in 100Base speed */
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_100base_testmode_set(unit, userPort, testMode));
            }
        }
        else
        {
            if(0u == speed)
            {
                /* Set testMode for port 5/6 in 100Base speed */
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_100base_testmode_set(unit, userPort, testMode));
            }
            else
            {
                /* Set testMode for port 5/6 in 1000Base speed */
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_1000base_testmode_set(unit, userPort, testMode, isMaster));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get test mode status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port ID (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH)
 * @param[in] speed 10Base-T1S, 100Base-T1 or 1000Base-T1 \n
 *                  for Port 1-4, 0: 10Base-T1S, 1: 100Base-T1 \n
 *                  for Port 5/6, 0: 100Base-T1, 1: 1000Base-T1 \n
 * @param[out] testMode Test mode
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval -RT_ERR_PORT_ID      The Port ID was illegal.
 * @retval -RT_ERR_INPUT        The speed was illegal.
 * @retval Others               Please refer to cvb_reg_read(), \n
 *                              cvb_drv_userport_to_sysport() and cvb_drv_valid_phytest_userport_check()
 */
RtkApiRet cvb_phy_testmode_get(uint32 unit, uint8 port, uint8 speed, uint8 *testMode)
{
    uint32    regOffset = 0u;
    uint32    regValue  = 0u;
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    phyId     = 0u;
    uint32    newPhyId  = 0u;

    do
    {
        if(NULL == testMode)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(1u < speed)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(port, &phyId));
        CHK_FUN_CONTINUE(retVal, cvb_drv_valid_phytest_userport_check(port));

        newPhyId = cvb_g_sysPortToOcpAddr[phyId];
        if(((uint8)CVB_RTK_USER_PORT_5TH == port) || ((uint8)CVB_RTK_USER_PORT_6TH == port))
        {
            if(0u == speed)
            {
                regOffset = (newPhyId | CVB_OCP_PHY_COMBO_100TSTMOD) << 1u;
            }
            else
            {
                regOffset = (newPhyId | CVB_OCP_PHYCR) << 1u;
            }
        }
        else
        {
            if(0u == speed)
            {
                regOffset = (newPhyId | 0xA596u) << 1u;
            }
            else
            {
                regOffset = (newPhyId | 0xA5BAu) << 1u;
            }
        }
        /* Get testMode information from regOffset */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, &regValue));
        *testMode = (uint8)((regValue & CVB_PHY_TESTMODE_BIT_MASK) >> CVB_PHY_TESTMODE_LSB);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PTM_ASIC */
/**@}*/ /* PTM */

/**
 * @addtogroup SQI
 * @{
 */
/**
 * @defgroup SQI_ASIC Low Level Driver
 * @brief SQI Low Level Driver
 * @{
 */

/**
 * @brief Get SQI status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] port Port ID (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH) \n
 *                 Select which port to read SQI.
 * @param[in] speed Speed of the port (0 to 1) \n
 *                  for Port 1-4, 0: 10Base-T1S, 1: 100Base-T1 \n
 *                  for Port 5/6, 0: 100Base-T1, 1: 1000Base-T1
 * @param[out] pSqi Signal quality index (0~7) \n
 *                  7 is the best.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_INPUT        The speed was illegal.
 * @retval -RT_ERR_PORT_ID      The port was out of range.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval Others               Please refer to cvb_drv_b_phy_sqi_get() and cvb_drv_r_phy_sqi_get() in rtl907xc_vb_asicdrv_phy.c.
 * @note This API sets SQI for a T1 port.
 */
RtkApiRet cvb_port_sqi_get(uint32 unit, RtkPort port, uint8 speed, uint8 *pSqi)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    sysPort  = 0u;
    uint8     userPort = 0u;

    do
    {
        if(1u < speed)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        if(NULL == pSqi)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_valid_phytest_userport_check(userPort));
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));

        /* Check the link state */
        CHK_FUN_CONTINUE(retVal, cvb_drv_is_link_up_for_sqi(unit, userPort));

        if((uint8)CVB_RTK_USER_PORT_4TH >= userPort)
        {
            /* if port 1,2,3,4 */
            CHK_FUN_CONTINUE(retVal, cvb_drv_b_phy_sqi_get(unit, sysPort, speed, pSqi));
        }
        else
        {
            /* if port 5,6 */
            CHK_FUN_CONTINUE(retVal, cvb_drv_r_phy_sqi_get(unit, sysPort, speed, pSqi));
        }

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* SQI_ASIC */
/**@}*/ /* SQI */
/**@}*/ /* RTK_MODULE_PHY_TEST_MODE */