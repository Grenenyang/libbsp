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

#include "rtl907xd_va_asicdrv_phy.h"
#include "rtl907xd_va_asicdrv.h"
#include "rtl907xd_va_api_ext.h"

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
 * @param[in] port Port index (DVA_EM_RTK_RTCT_PORT1 to DVA_EM_RTK_RTCT_PORT6) \n
 *                 Select which port to perform RTCT to get the cable status from the switch port.
 * @param[out] None
 * @return RT_ERR_OK     The acceptable frame type of the port was set.
 * @return -RT_ERR_INPUT The port index was illegal.
 * @return Others        Please refer to \n
 *                       dva_drv_userport_to_sysport() and dva_drv_valid_phytest_userport_check() \n
 *                       dva_reg_write()
 */
RtkApiRet dva_rtct_run_cable_diag(uint32 unit, RtkRtctPortNumber_e port)
{
    RtkApiRet   retVal                              = (RtkApiRet)RT_ERR_OK;
    uint32      sysPort                             = 0u;
    uint8       userPort                            = 0u;
    const uint8 rtctToUserPort[EM_RTK_RTCT_PORTMAX] = {0xFFu, DVA_RTK_USER_PORT_1ST, DVA_RTK_USER_PORT_2ND, DVA_RTK_USER_PORT_3RD,
                                                       DVA_RTK_USER_PORT_4TH, DVA_RTK_USER_PORT_5TH, DVA_RTK_USER_PORT_6TH};

    do
    {
        if((EM_RTK_RTCT_PORTMAX <= port) || ((uint8)0xFFu == rtctToUserPort[port]))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Assign RTCT port index to userPort */
        userPort = rtctToUserPort[port];
        /* Translate the user port to the system port */
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        /* Check if the system port is the valid phytest port */
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Trigger RTCT */
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, (dva_g_sysPortToOcpAddr[sysPort] | DVA_OCP_RTCT_CTL) << 1u, 0x1u));
        /* Wait until
           the valid port index is checked, and RTCT is triggered: */
        /* while loop: 0 is 1 */
    } while(0u == 1u);
    /* return retVal */
    return retVal;
}

/**
 * @brief Get RTK cable diagnostic result.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port index (DVA_EM_RTK_RTCT_PORT1 to DVA_EM_RTK_RTCT_PORT6) \n
 *                 Select which port to perform RTCT to get the cable status from the switch port.
 * @param[out] rtkRtctCableStatus Get cable status (0 to 3) \n
 *                                Including the state of normal/open/short \n
 *                                0: Normal \n
 *                                1: Open \n
 *                                2: Short \n
 *                                3: RTCT progress fail
 * @param[out] rtkRtctCableDefect Returned the cable open short or open location (0x0 to 0xFFFF) \n
 *                                For port 1~6 (100/1000Base), transforming the hexadecimal value into decimal value then dividing by 7.5 \n
 *                                The result is cable open/short location (in meters) \n
 *                                Note that if cable status is Normal, this value will always be 0
 * @return RT_ERR_OK              The acceptable frame type of the port was set.
 * @return -RT_ERR_NULL_POINTER   The pointer is NULL.
 * @return -RT_ERR_FAILED         The RTCT result was not ready.
 * @return Others                 Please refer to \n
 *                                dva_drv_userport_to_sysport() and dva_drv_valid_phytest_userport_check() \n
 *                                dva_reg_read() \n
 *                                dva_drv_rtct_status_get()
 */
RtkApiRet dva_rtct_get_cable_status(uint32                unit,
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
    const uint8            rtctToUserPort[EM_RTK_RTCT_PORTMAX] = {0xFFu, DVA_RTK_USER_PORT_1ST, DVA_RTK_USER_PORT_2ND, DVA_RTK_USER_PORT_3RD,
                                                                  DVA_RTK_USER_PORT_4TH, DVA_RTK_USER_PORT_5TH, DVA_RTK_USER_PORT_6TH};
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
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        /* Check if the system port is the valid phytest port */
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read RTCT register to check if the result of RTK cable diag is ready */
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, (dva_g_sysPortToOcpAddr[sysPort] | DVA_OCP_RTCT_CTL) << 1u, &regData));
        /* Check RTCT ready bit: check if the 15th bit of regData is set */
        if(0u != (regData & 0x8000u))
        {
            /* This situation indicates RTCT result is ready */
            /* Get the cable status and defect in meters */
            CHK_FUN_CONTINUE(retVal, dva_drv_rtct_status_get(unit, dva_g_sysPortToOcpAddr[sysPort], &cableStatus, &cableDefect));
            /* Compare the cable status value with the level in rtct array, find the matched status result,
             　and write back the cable defect location: */
            /* For loop : rtctIndx = 0; rtctIndx < 3; ++rtctIndx */
            for(rtctIndx = 0u; rtctIndx < DVA_RTK_RTCT_ARRAY; ++rtctIndx)
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
 * @param[in] port Port index (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH)
 * @param[in] testMode Test mode \n
 *                     for 100Base-T1, 0/1/2/4/5 \n
 *                     for 1000Base-T1, 0/1/2/4/5/6/7
 * @param[in] speed 100Base-T1 or 1000Base-T1 \n
 *                  0: 100Base-T1 \n
 *                  1: 1000Base-T1
 * @param[in] isMaster Master jitter select \n
 *            0: slave jitter select \n
 *            1: master jitter select
 * @param[out] None
 * @return RT_ERR_OK       The acceptable frame type of the port was set.
 * @return -RT_ERR_INPUT   The speed or isMaster was illegal.
 * @return Others          Please refer to \n
 *                         dva_drv_userport_to_sysport() and dva_drv_valid_phytest_userport_check() \n
 *                         dva_phy_100base_testmode_set() and dva_phy_1000base_testmode_set() \n
 *                         in rtl906x_asicdrv_phy.c.
 */
RtkApiRet dva_phy_testmode_set(uint32 unit, uint8 port, uint8 const testMode,
                               uint8 speed, uint8 isMaster)
{
    RtkApiRet   retVal                                                                = RT_ERR_OK;
    uint32      sysPort                                                               = 0u;
    const uint8 portSelTable[DVA_PHY_TEST_USER_PORT_END]                              = {0xFFu, 0u, 0x01u, 0x02u, 0x03u, 0x04u, 0x08u};
    const uint8 supportMode[DVA_PHY_TESTMODE_SPEED_NUM_END][DVA_PHY_TESTMODE_NUM_END] = {{1u, 1u, 1u, 0u, 1u, 1u, 0u, 0u},
                                                                                         {1u, 1u, 1u, 0u, 1u, 1u, 1u, 1u}};

    do
    {
        if((DVA_PHY_TESTMODE_SPEED_NUM_END <= speed) || (DVA_PHY_TESTMODE_ROLE_NUM_END <= isMaster))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if((DVA_PHY_TESTMODE_NUM_END <= testMode) || (0u == supportMode[(uint8)speed][testMode]))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(port, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(port));

        if(0u == speed)
        {
            /* Set testMode for port in 100Base speed */
            CHK_FUN_CONTINUE(retVal, dva_drv_phy_100base_testmode_set(unit, sysPort, portSelTable[port], testMode));
        }
        else
        {
            /* Set testMode for port in 1000Base speed */
            CHK_FUN_CONTINUE(retVal, dva_drv_phy_1000base_testmode_set(unit, sysPort, testMode, isMaster));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get test mode status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH)
 * @param[in] speed 100Base-T1 or 1000Base-T1 \n
 *                  for Port 1-6, 0: 100Base-T1, 1: 1000Base-T1 \n
 * @param[out] testMode Test mode
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval -RT_ERR_INPUT        The speed was illegal.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport() and dva_drv_valid_phytest_userport_check() \n
 *                              dva_reg_read()
 */
RtkApiRet dva_phy_testmode_get(uint32 unit, uint8 port, uint8 speed, uint8 *testMode)
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

        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(port, &phyId));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(port));

        newPhyId = dva_g_sysPortToOcpAddr[phyId];
        if(0u == speed)
        {
            regOffset = (newPhyId | DVA_OCP_PHY_COMBO_100TSTMOD) << 1u;
        }
        else
        {
            regOffset = (newPhyId | DVA_OCP_PHYCR) << 1u;
        }
        /* Get testMode information from regOffset */
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regOffset, &regValue));
        *testMode = (uint8)((regValue & DVA_PHY_TESTMODE_BIT_MASK) >> DVA_PHY_TESTMODE_LSB);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PTM_ASIC */
/**@}*/ /* PTM */

/**
 * @addtogroup MSE
 * @{
 */
/**
 * @defgroup MSE_ASIC Low Level Driver
 * @brief MSE Low Level Driver
 * @{
 */
/**
 * @brief Get MSE measurement.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read MSE.
 * @param[out] pMse Mean square error (0 to 511)
 *                  The higher the signal-to-noise ratio, the lower the MSE.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval -RT_ERR_PHY_SQI_INVALID  MSE is invalid.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_mse_get(uint32 unit, RtkPort port, uint16 *pMse)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint16    temp       = 0u;
    uint16    invalid    = (uint16)DVA_MSE_INVALID_BIT_MASK;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointer pMse is NULL */
        if(NULL == pMse)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read MSE value */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_MSE) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));
        temp = (uint16)(regData & 0x0000FFFFu);

        /* Check if the value is invalid */
        if(0u != (temp & invalid))
        {
            retVal = -(RtkApiRet)RT_ERR_PHY_SQI_INVALID;
            continue;
        }

        *pMse = temp & (uint16)DVA_MSE_DATA_MASK;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the worst case of MSE measurement.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read the worst case of MSE.
 * @param[out] pWcMse Worst case of mean square error (0 to 511)
 *                    The lower the better.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval -RT_ERR_PHY_SQI_INVALID  The value is invalid.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_wc_mse_get(uint32 unit, RtkPort port, uint16 *pWcMse)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint16    temp       = 0u;
    uint16    invalid    = (uint16)DVA_WCMSE_INVALID_BIT_MASK;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointer pWcMse is NULL */
        if(NULL == pWcMse)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read the worst case of MSE */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_WC_MSE) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));
        temp = (uint16)(regData & 0x0000FFFFu);

        /* Check if the value is invalid */
        if(0u != (temp & invalid))
        {
            retVal = -(RtkApiRet)RT_ERR_PHY_SQI_INVALID;
            continue;
        }

        *pWcMse = temp & (uint16)DVA_WCMSE_DATA_MASK;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* MSE_ASIC */
/**@}*/ /* MSE */

/**
 * @addtogroup PMSE
 * @{
 */
/**
 * @defgroup PMSE_ASIC Low Level Driver
 * @brief The peak MSE Low Level Driver
 * @{
 */
/**
 * @brief Get the peak MSE measurement.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read the peak MSE value.
 * @param[out] pPkMse The peak mean square error (0 to 63)
 *                    The higher the signal-to-noise ratio, the lower the peak MSE.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval -RT_ERR_PHY_SQI_INVALID  The value is invalid.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_peak_mse_get(uint32 unit, RtkPort port, uint8 *pPkMse)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint8     temp       = 0u;
    uint8     invalid    = (uint8)DVA_PMSE_INVALID_BIT_MASK;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointer pPkMse is NULL */
        if(NULL == pPkMse)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read the peak MSE value */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_PMSE) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));
        temp = (uint8)((regData >> DVA_PMSE_LSP) & DVA_PMSE_MASK);

        /* Check if the value is invalid */
        if(0u != (temp & invalid))
        {
            retVal = -(RtkApiRet)RT_ERR_PHY_SQI_INVALID;
            continue;
        }

        *pPkMse = temp & (uint8)DVA_PMSE_DATA_MASK;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the worst case of the peak MSE measurement.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read the worst case of the peak MSE value.
 * @param[out] pWcPkMse The worst case of the peak mean square error (0 to 63)
 *                      The higher the signal-to-noise ratio, the lower the worst case of the peak MSE.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval -RT_ERR_PHY_SQI_INVALID  The value is invalid.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_wc_peak_mse_get(uint32 unit, RtkPort port, uint8 *pWcPkMse)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint8     temp       = 0u;
    uint8     invalid    = (uint8)DVA_WCPMSE_INVALID_BIT_MASK;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointer pWcPkMse is NULL */
        if(NULL == pWcPkMse)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read the worst case of the peak MSE value */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_WC_PMSE) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));
        temp = (uint8)((regData >> DVA_WCPMSE_LSP) & DVA_WCPMSE_MASK);

        /* Check if the value is invalid */
        if(0u != (temp & invalid))
        {
            retVal = -(RtkApiRet)RT_ERR_PHY_SQI_INVALID;
            continue;
        }

        *pWcPkMse = temp & (uint8)DVA_WCPMSE_DATA_MASK;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PMSE_ASIC */
/**@}*/ /* PMSE */

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
 * @brief Get SQI index.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read SQI.
 * @param[in] speed Speed of the port (0 to 1) \n
 *                  for Port 1-6, 0: 100Base-T1, 1: 1000Base-T1
 * @param[out] pSqi Signal quality index (0~7)
 *                  7 is the best.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_INPUT        The speed was illegal.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport() and dva_drv_valid_phytest_userport_check() \n
 *                              dva_is_link_up_for_sqi() and dva_phy_1000base_sqi_get()
 * @note This API sets SQI for a T1 port.
 */
RtkApiRet dva_port_sqi_get(uint32 unit, RtkPort port, uint8 speed, uint8 *pSqi)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    sysPort  = 0u;
    uint8     userPort = 0u;

    do
    {
        /* Check if the speed is out of range */
        if(1u < speed)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        /* Check if the pointer pSqi is NULL */
        if(NULL == pSqi)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Check the link state */
        CHK_FUN_CONTINUE(retVal, dva_drv_is_link_up_for_sqi(unit, userPort));

        /* Obtain SQI according to speed */
        CHK_FUN_CONTINUE(retVal, dva_drv_r_phy_sqi_get(unit, sysPort, speed, pSqi));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* SQI_ASIC */
/**@}*/ /* SQI */

/**
 * @addtogroup LTT
 * @{
 */
/**
 * @defgroup LTT_ASIC Low Level Driver
 * @brief LTT Low Level Driver
 * @{
 */
/**
 * @brief Get LTT time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read LTT.
 * @param[out] pTime Link training time (0 to 250)
 *                   The time unit is microsecond.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_ltt_get(uint32 unit, RtkPort port, uint8 *pTime)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint8     temp       = 0u;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointer pTime is NULL */
        if(NULL == pTime)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read LTT value */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_LTT) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));
        temp = (uint8)(regData & DVA_OCP_LTT_MASK);

        *pTime = temp;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* LTT_ASIC */
/**@}*/ /* LTT */

/**
 * @addtogroup LRT
 * @{
 */
/**
 * @defgroup LRT_ASIC Low Level Driver
 * @brief LRT Low Level Driver
 * @{
 */
/**
 * @brief Get LRT time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read LRT.
 * @param[out] pTime Local receiver time (0 to 250)
 *                   The time unit is microsecond.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_lrt_get(uint32 unit, RtkPort port, uint8 *pTime)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint8     temp       = 0u;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointer pTime is NULL */
        if(NULL == pTime)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read LRT value */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_LRT) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));
        temp = (uint8)(regData & DVA_OCP_LRT_MASK);

        *pTime = temp;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* LRT_ASIC */
/**@}*/ /* LRT */

/**
 * @addtogroup RRT
 * @{
 */
/**
 * @defgroup RRT_ASIC Low Level Driver
 * @brief RRT Low Level Driver
 * @{
 */
/**
 * @brief Get RRT time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read RRT.
 * @param[out] pTime Remote receiver time (0 to 250)
 *                   The time unit is microsecond.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_rrt_get(uint32 unit, RtkPort port, uint8 *pTime)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint8     temp       = 0u;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointer pTime is NULL */
        if(NULL == pTime)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read RRT value */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_RRT) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));
        temp = (uint8)(regData & DVA_OCP_RRT_MASK);

        *pTime = temp;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RRT_ASIC */
/**@}*/ /* RRT */

/**
 * @addtogroup LFL
 * @{
 */
/**
 * @defgroup LFL_ASIC Low Level Driver
 * @brief LFL Low Level Driver
 * @{
 */
/**
 * @brief Get the link loss counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read the counter.
 * @param[out] pLoss Link loss counter (0 to 0x3F)
 *                   The number of times.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_lfl_loss_get(uint32 unit, RtkPort port, uint8 *pLoss)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    newPhyAddr = 0u;
    uint32    regAddr    = 0u;
    uint32    regData    = 0u;
    uint8     loss       = 0u;
    uint8     userPort   = 0u;

    do
    {
        /* Check if the pointers are null */
        if(NULL == pLoss)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read LFL counters */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_LFL) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));

        loss = (uint8)((regData >> DVA_OCP_LFL_LOSS_LSP) & DVA_OCP_LFL_LOSS_MASK);

        *pLoss = loss;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the link failure counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                 Select which port to read the counter.
 * @param[out] pFail Link failure counter (0 to 0x3FFu)
 *                   The number of times.
 * @retval RT_ERR_OK            The acceptable frame type of the port was set.
 * @retval -RT_ERR_NULL_POINTER The pointer is NULL.
 * @retval -RT_ERR_NOT_SUPPORT  The port mode is not supported.
 * @retval Others               Please refer to \n
 *                              dva_drv_userport_to_sysport(), dva_drv_valid_phytest_userport_check() and dva_reg_read()
 */
RtkApiRet dva_port_lfl_fail_get(uint32 unit, RtkPort port, uint16 *pFail)
{
    RtkApiRet    retVal     = RT_ERR_OK;
    uint32       sysPort    = 0u;
    uint32       newPhyAddr = 0u;
    uint32       regAddr    = 0u;
    uint32       regData    = 0u;
    uint16       fail       = 0u;
    uint8        userPort   = 0u;
    RtkModeExt_e mode       = EM_MODE_EXT_100BASE_T1_PHY;

    do
    {
        /* Check if the pointers are null */
        if(NULL == pFail)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Check if the port is in 100BASE-T1 mode */
        CHK_FUN_CONTINUE(retVal, dva_port_mode_get(unit, port, &mode));
        if(EM_MODE_EXT_100BASE_T1_PHY != mode)
        {
            retVal = -(RtkApiRet)RT_ERR_NOT_SUPPORT;
            continue;
        }

        /* Translate the user port index to the system port index
           and check if the port is valid for the phytest APIs */
        userPort = (uint8)(port & 0xFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_userport_to_sysport(userPort, &sysPort));
        CHK_FUN_CONTINUE(retVal, dva_drv_valid_phytest_userport_check(userPort));

        /* Read LFL counters */
        newPhyAddr = dva_g_sysPortToOcpAddr[sysPort];
        regAddr    = (newPhyAddr | DVA_OCP_LFL) << 1u;
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, regAddr, &regData));

        fail = (uint8)((regData >> DVA_OCP_LFL_FAIL_LSP) & DVA_OCP_LFL_FAIL_MASK);

        *pFail = fail;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* LFL_ASIC */
/**@}*/ /* LFL */

/**@}*/ /* RTK_MODULE_PHY_TEST_MODE */
