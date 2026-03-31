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
#include "rtkas_api_phy.h"

/* The mapping from sys port index to OCP addr offset
 *                               OCP Base address of sys port               p2        p3        p4        p5        p6        p7        P8                        */
const uint32 cvb_g_sysPortToOcpAddr[CVB_RTK_SYS_PORT_ID_MAX] = {0x0u, 0x0u, 0x30000u, 0x40000u, 0x50000u, 0x60000u, 0x70000u, 0x80000u, 0xB0000u, 0x0u, 0x0u, 0x0u};

/**
 * @addtogroup RTK_MODULE_PHY PHY
 * @{
 */
/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_BMCR BMCR
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_BMCR_ASIC Low Level Driver
 * @brief BMCR Low Level Driver
 * @{
 */

/**
 * @brief Perform a software reset on the PHY.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map containing ports to trigger reset (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                    The valid bits in portMap are: \n
 *                    - bit1: port1 10/100BASE-T1 PHY
 *                    - bit2: port2 10/100BASE-T1 PHY
 *                    - bit3: port3 10/100BASE-T1 PHY
 *                    - bit4: port4 10/100BASE-T1 PHY
 *                    - bit5: port5 100/1000BASE-T1 PHY
 *                    - bit6: port6 100/1000BASE-T1 PHY
 * @param[out] None
 * @retval RT_ERR_OK The software reset was successful.
 * @retval -RT_ERR_PORT_MASK if the input portMap is invalid.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_bmcr_reset(uint32 unit, RtkPmap portMap)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check input parameters */
        if(CVB_PHY_T1_USER_PORTMAP_ALL < portMap)
        {
            retVal = -(RT_ERR_PORT_MASK);
            continue;
        }
        /* Translate system portmap to user port map */
        userPmap = (portMap & (RtkPmap)CVB_PHY_T1_USER_PORTMAP_ALL);
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert((RtkPmap)userPmap, &sysPmap, CVB_EM_USER_TO_SYS));
        /* Trigger PHY software reset of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            if(0u != (((RtkPort)1u << sysPort) & sysPmap))
            {
                /* Set the BMCR bit[15]:reset */
                CHK_FUN_BREAK(retVal, cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_RST_LSP, 1u));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the ports enabled in PCS loopback mode.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                     The mapping between port and bits in portMap are: \n
 *                     - bit1: port1 10/100BASE-T1 PHY
 *                     - bit2: port2 10/100BASE-T1 PHY
 *                     - bit3: port3 10/100BASE-T1 PHY
 *                     - bit4: port4 10/100BASE-T1 PHY
 *                     - bit5: port5 100/1000BASE-T1 PHY
 *                     - bit6: port6 100/1000BASE-T1 PHY
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_bit_get() and cvb_drv_pmap_convert().
 */
RtkApiRet cvb_phy_bmcr_pcs_loopback_get(uint32 unit, RtkPmap *pPortMap)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;
    uint32    bitVal   = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Check PCS loopback of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            /* Get the BMCR bit[14]: PCS loopback */
            CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_DIGILBK_LSP, &bitVal));
            if(1u == bitVal)
            {
                sysPmap |= ((RtkPort)1u << sysPort);
            }
        }
        /* Translate system port map to user port map */
        if(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(sysPmap, &userPmap, CVB_EM_SYS_TO_USER));
            *pPortMap = userPmap;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable the PCS loopback mode of the designated ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                    The valid bits in portMap are: \n
 *                    - bit1: port1 10/100BASE-T1 PHY
 *                    - bit2: port2 10/100BASE-T1 PHY
 *                    - bit3: port3 10/100BASE-T1 PHY
 *                    - bit4: port4 10/100BASE-T1 PHY
 *                    - bit5: port5 100/1000BASE-T1 PHY
 *                    - bit6: port6 100/1000BASE-T1 PHY
 * @param[in] enableCfg The value to set (DISABLE or ENABLE). \n
 *                      Enable or disable the PCS loopback mode of the selected ports.
 *                      - enableCfg = ENABLE: Enable the PHY PCS loopback mode.
 *                      - enableCfg = DISABLE: Disable the PHY PCS loopback mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_MASK if the input portMap is invalid.
 * @retval -RT_ERR_INPUT if the input enableCfg is invalid.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_bmcr_pcs_loopback_set(uint32 unit, RtkPmap portMap, RtkEnable enableCfg)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check input parameters */
        if(CVB_PHY_T1_USER_PORTMAP_ALL < portMap)
        {
            retVal = -(RT_ERR_PORT_MASK);
            continue;
        }
        if((ENABLE != enableCfg) && (DISABLE != enableCfg))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        /* Translate user port map to system port map */
        userPmap = (portMap & (RtkPmap)CVB_PHY_T1_USER_PORTMAP_ALL);
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert((RtkPmap)userPmap, &sysPmap, CVB_EM_USER_TO_SYS));
        /* Enable PCS loopback of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            if(0u != (((RtkPort)1u << sysPort) & sysPmap))
            {
                /* Configure the BMCR bit[14]: PCS loopback to input parameter value */
                CHK_FUN_BREAK(retVal, cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_DIGILBK_LSP, enableCfg));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the BMCR speed setting of the designated port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX). \n
 *                 The designated port to configure.
 * @param[out] pSpeed Speed (PHY_BMCR_SPEED_10M/PHY_BMCR_SPEED_100M/PHY_BMCR_SPEED_1000M). \n
 *                    The BMCR speed setting.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_PHY_SPEED if the BMCR register value does not map to a PHY speed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport() and cvb_reg_read().
 */
RtkApiRet cvb_phy_bmcr_speed_get(uint32 unit, RtkPort port, uint32 *pSpeed)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPort   sysPort  = 0u;
    uint32    regVal   = 0u;
    uint32    speedVal = 0u;
    uint8     userPort = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > port) || (CVB_PHY_T1_USER_PORT_MAX < port))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == pSpeed)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Translate user port to system port */
        userPort = (uint8)(port & 0xFFu);
        /* Translate the user port index to the system port index */
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get the BMCR of the designated port */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, &regVal));
        /* Get the speed from bit6, bit13*/
        speedVal = (((regVal >> CVB_PHY_SPD1_LSP) & 0x1u) << 1u) | ((regVal >> CVB_PHY_SPD0_LSP) & 0x1u);
        switch(speedVal)
        {
            case 0x2u:
                *pSpeed = PHY_BMCR_SPEED_1000M;
                break;
            case 0x1u:
                *pSpeed = PHY_BMCR_SPEED_100M;
                break;
            case 0x0u:
                *pSpeed = PHY_BMCR_SPEED_10M;
                break;
            default:
                retVal = -(RT_ERR_PHY_SPEED);
                break;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the BMCR speed setting of the designated port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX). \n
 *                 The designated port to configure.
 * @param[in] speed Speed (PHY_BMCR_SPEED_10M/PHY_BMCR_SPEED_100M/PHY_BMCR_SPEED_1000M). \n
 *                  The BMCR speed setting.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port is invalid.
 * @retval -RT_ERR_PHY_SPEED if the BMCR register value does not map to a PHY speed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport(), cvb_reg_read(), and cvb_reg_write().
 */
RtkApiRet cvb_phy_bmcr_speed_set(uint32 unit, RtkPort port, uint32 speed)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPort   sysPort  = 0u;
    uint32    regVal   = 0u;
    uint32    speedVal = 0u;
    uint8     userPort = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > port) || (CVB_PHY_T1_USER_PORT_MAX < port))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        /* Translate user port to system port */
        userPort = (uint8)(port & 0xFFu);
        /* Translate the user port index to the system port index */
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Mapping the speed setting to a BMCR register value */
        switch(speed)
        {
            case PHY_BMCR_SPEED_1000M:
                speedVal = ((uint32)1u << CVB_PHY_SPD1_LSP);
                break;
            case PHY_BMCR_SPEED_100M:
                speedVal = ((uint32)1u << CVB_PHY_SPD0_LSP);
                break;
            case PHY_BMCR_SPEED_10M:
                speedVal = 0u;
                break;
            default:
                retVal = -(RT_ERR_PHY_SPEED);
                break;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        /* Set the BMCR of the designated port */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, &regVal));
        /* Set bit6, bit13 to 0 */
        regVal &= 0xFFFFDFBFu;
        /* Update bit6, bit13 to current speed setting */
        regVal |= speedVal;
        CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, regVal));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the ports enabled in auto-negotiation mode.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                     The mapping between port and bits in portMap are: \n
 *                     - bit1: port1 10/100BASE-T1 PHY
 *                     - bit2: port2 10/100BASE-T1 PHY
 *                     - bit3: port3 10/100BASE-T1 PHY
 *                     - bit4: port4 10/100BASE-T1 PHY
 *                     - bit5: port5 100/1000BASE-T1 PHY
 *                     - bit6: port6 100/1000BASE-T1 PHY
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_get().
 */
RtkApiRet cvb_phy_bmcr_an_mode_get(uint32 unit, RtkPmap *pPortMap)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;
    uint32    bitVal   = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Check auto-negotiation mode of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            /* Get the BMCR bit[12]: auto-negotiation mode */
            CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_AN_MODE_LSP, &bitVal));
            if(1u == bitVal)
            {
                sysPmap |= ((RtkPort)1u << sysPort);
            }
        }
        /* Translate system port map to user port map */
        if(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(sysPmap, &userPmap, CVB_EM_SYS_TO_USER));
            *pPortMap = userPmap;
        }
    } while(0u == 1u);

    return retVal;
}
/**
 * @brief Enable or disable the auto-negotiation mode of the designated ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                    The valid bits in portMap are: \n
 *                    - bit1: port1 10/100BASE-T1 PHY
 *                    - bit2: port2 10/100BASE-T1 PHY
 *                    - bit3: port3 10/100BASE-T1 PHY
 *                    - bit4: port4 10/100BASE-T1 PHY
 *                    - bit5: port5 100/1000BASE-T1 PHY
 *                    - bit6: port6 100/1000BASE-T1 PHY
 * @param[in] anEnableCfg The value to set (DISABLE or ENABLE). \n
 *                        Enable or disable the auto-negotiation mode of the selected ports.
 *                        - anEnableCfg = ENABLE: Enable the auto-negotiation mode.
 *                        - anEnableCfg = DISABLE: Disable the auto-negotiation mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_MASK if the input portMap is invalid.
 * @retval -RT_ERR_INPUT if the input anEnableCfg is invalid.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_bmcr_an_mode_set(uint32 unit, RtkPmap portMap, RtkEnable anEnableCfg)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check input parameters */
        if(CVB_PHY_T1_USER_PORTMAP_ALL < portMap)
        {
            retVal = -(RT_ERR_PORT_MASK);
            continue;
        }
        if((ENABLE != anEnableCfg) && (DISABLE != anEnableCfg))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        /* Translate user port map to system port map */
        userPmap = (portMap & (RtkPmap)CVB_PHY_T1_USER_PORTMAP_ALL);
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert((RtkPmap)userPmap, &sysPmap, CVB_EM_USER_TO_SYS));
        /* Enable auto-negotiation mode of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            if(0u != (((RtkPort)1u << sysPort) & sysPmap))
            {
                /* Configure the BMCR bit[12]: auto-negotiation mode */
                CHK_FUN_BREAK(retVal, cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_AN_MODE_LSP, anEnableCfg));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Restart auto-negotiation of the designated ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                    The valid bits in portMap are: \n
 *                    - bit1: port1 10/100BASE-T1 PHY
 *                    - bit2: port2 10/100BASE-T1 PHY
 *                    - bit3: port3 10/100BASE-T1 PHY
 *                    - bit4: port4 10/100BASE-T1 PHY
 *                    - bit5: port5 100/1000BASE-T1 PHY
 *                    - bit6: port6 100/1000BASE-T1 PHY
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_MASK if the input portMap is invalid.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_bmcr_restart_an(uint32 unit, RtkPmap portMap)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check input parameters */
        if(CVB_PHY_T1_USER_PORTMAP_ALL < portMap)
        {
            retVal = -(RT_ERR_PORT_MASK);
            continue;
        }
        /* Translate user port map to system port map */
        userPmap = (portMap & (RtkPmap)CVB_PHY_T1_USER_PORTMAP_ALL);
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert((RtkPmap)userPmap, &sysPmap, CVB_EM_USER_TO_SYS));
        /* Restart auto-negotiation of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            if(0u != (((RtkPort)1u << sysPort) & sysPmap))
            {
                /* Configure the BMCR bit[9]: auto-negotiation to input parameter value */
                CHK_FUN_BREAK(retVal, cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_AN_RESTART_LSP, 1u));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the ports set to power down.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                     The mapping between port and bits in portMap are: \n
 *                     - bit1: port1 10/100BASE-T1 PHY
 *                     - bit2: port2 10/100BASE-T1 PHY
 *                     - bit3: port3 10/100BASE-T1 PHY
 *                     - bit4: port4 10/100BASE-T1 PHY
 *                     - bit5: port5 100/1000BASE-T1 PHY
 *                     - bit6: port6 100/1000BASE-T1 PHY
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_get().
 */
RtkApiRet cvb_phy_bmcr_power_down_get(uint32 unit, RtkPmap *pPortMap)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;
    uint32    bitVal   = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Check the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            /* Get the BMCR bit[11]: power down */
            CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_PWRDN_LSP, &bitVal));
            if(1u == bitVal)
            {
                sysPmap |= ((RtkPort)1u << sysPort);
            }
        }
        /* Translate system port map to user port map */
        if(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(sysPmap, &userPmap, CVB_EM_SYS_TO_USER));
            *pPortMap = userPmap;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set or unset power down of the designated ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                    The valid bits in portMap are: \n
 *                    - bit1: port1 10/100BASE-T1 PHY
 *                    - bit2: port2 10/100BASE-T1 PHY
 *                    - bit3: port3 10/100BASE-T1 PHY
 *                    - bit4: port4 10/100BASE-T1 PHY
 *                    - bit5: port5 100/1000BASE-T1 PHY
 *                    - bit6: port6 100/1000BASE-T1 PHY
 * @param[in] pwdCfg The value to set (DISABLE or ENABLE). \n
 *                   Enable or disable the power down of the selected ports.
 *                   - pwdCfg = ENABLE: Set power down.
 *                   - pwdCfg = DISABLE: Unset power down.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_MASK if the input portMap is invalid.
 * @retval -RT_ERR_INPUT if the input pwdCfg is invalid.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_bmcr_power_down_set(uint32 unit, RtkPmap portMap, RtkEnable pwdCfg)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check input parameters */
        if(CVB_PHY_T1_USER_PORTMAP_ALL < portMap)
        {
            retVal = -(RT_ERR_PORT_MASK);
            continue;
        }
        if((ENABLE != pwdCfg) && (DISABLE != pwdCfg))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        /* Translate user port map to system port map */
        userPmap = (portMap & (RtkPmap)CVB_PHY_T1_USER_PORTMAP_ALL);
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert((RtkPmap)userPmap, &sysPmap, CVB_EM_USER_TO_SYS));
        /* Set power down of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            if(0u != (((RtkPort)1u << sysPort) & sysPmap))
            {
                /* Configure the BMCR bit[11]: power down to input parameter value */
                CHK_FUN_BREAK(retVal, cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_PWRDN_LSP, pwdCfg));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the ports enabled in isolate mode.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                     The mapping between port and bits in portMap are: \n
 *                     - bit1: port1 10/100BASE-T1 PHY
 *                     - bit2: port2 10/100BASE-T1 PHY
 *                     - bit3: port3 10/100BASE-T1 PHY
 *                     - bit4: port4 10/100BASE-T1 PHY
 *                     - bit5: port5 100/1000BASE-T1 PHY
 *                     - bit6: port6 100/1000BASE-T1 PHY
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_get().
 */
RtkApiRet cvb_phy_bmcr_isolate_get(uint32 unit, RtkPmap *pPortMap)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;
    uint32    bitVal   = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Check ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            /* Get the BMCR bit[10]: Isolate */
            CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_MII_ISO_LSP, &bitVal));
            if(1u == bitVal)
            {
                sysPmap |= ((RtkPort)1u << sysPort);
            }
        }
        /* Translate system port map to user port map */
        if(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(sysPmap, &userPmap, CVB_EM_SYS_TO_USER));
            *pPortMap = userPmap;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable the isolate mode of the designated ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map (0x0 to CVB_PHY_T1_USER_PORTMAP_ALL). \n
 *                    The valid bits in portMap are: \n
 *                    - bit1: port1 10/100BASE-T1 PHY
 *                    - bit2: port2 10/100BASE-T1 PHY
 *                    - bit3: port3 10/100BASE-T1 PHY
 *                    - bit4: port4 10/100BASE-T1 PHY
 *                    - bit5: port5 100/1000BASE-T1 PHY
 *                    - bit6: port6 100/1000BASE-T1 PHY
 * @param[in] isoCfg The value to set (DISABLE or ENABLE). \n
 *                   Enable or disable the isolate mode of the selected ports.
 *                   - isoCfg = ENABLE: Enable the isolate mode.
 *                   - isoCfg = DISABLE: Disable the isolate mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_MASK if the input portMap is invalid.
 * @retval -RT_ERR_INPUT if the input isoCfg is invalid.
 * @retval Others Please refer to cvb_drv_pmap_convert() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_bmcr_isolate_set(uint32 unit, RtkPmap portMap, RtkEnable isoCfg)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPmap   userPmap = 0u;
    RtkPmap   sysPmap  = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check input parameters */
        if(CVB_PHY_T1_USER_PORTMAP_ALL < portMap)
        {
            retVal = -(RT_ERR_PORT_MASK);
            continue;
        }
        if((ENABLE != isoCfg) && (DISABLE != isoCfg))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        /* Translate user port map to system port map */
        userPmap = (portMap & (RtkPmap)CVB_PHY_T1_USER_PORTMAP_ALL);
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert((RtkPmap)userPmap, &sysPmap, CVB_EM_USER_TO_SYS));
        /* Set isolate setting of the ports in portmap */
        for(sysPort = CVB_PHY_T1_SYS_PORT_MIN; sysPort <= CVB_PHY_T1_SYS_PORT_MAX; sysPort++)
        {
            if(0u != (((RtkPort)1u << sysPort) & sysPmap))
            {
                /* Configure the BMCR bit[10]: isolate to input parameter value */
                CHK_FUN_BREAK(retVal, cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, CVB_PHY_MII_ISO_LSP, isoCfg));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_BMCR_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_BMCR */

/**
 * @addtogroup PHY_100_1000_BASE_T1_PHYID PHYID
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_PHYID_ASIC Low Level Driver
 * @brief PHYID Low Level Driver
 * @{
 */
/**
 * @brief Get the PHY identifier information by PHYID1 (PHY Identifier Register 1) and PHYID2 (PHY Identifier Register 2).
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100/1000BASE-T1 port to get the PHY OUI.
 * @param[out] pOui The PHY OUI (0x0 to 0xFFFFFFFF). \n
 *                  The PHY Organizationally Unique Identifier.
 * @param[out] pModleNum The Manufacturer’s Model Number (0x0 to 0x3F). \n
 *                       The PHY Manufacturer’s Model Number.
 * @param[out] pRevNum The Revision number (0x0 to 0xF). \n
 *                     The PHY Revision number.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport() and cvb_reg_field_read().
 */
RtkApiRet cvb_phy_phyid_get(uint32 unit, RtkPort portId, uint32 *pOui, uint8 *pModleNum, uint8 *pRevNum)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regOffset = 0x0u;
    uint32    regValue  = 0x0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_ETH_USER_PORT_IDX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((NULL == pOui) || (NULL == pModleNum) || (NULL == pRevNum))
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get the PHYID1 OCP offset of the designated system port */
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYID1) << 1u;
        /* Get the current realtek OUI bit[21:6] from PHYID1 */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PHYID1_OUI_MSB_LSP, CVB_PHY_PHYID1_OUI_MSB_LEN, &regValue));
        *pOui = regValue << 6u;

        /* Get the PHYID2 OCP offset of the designated system port */
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYID2) << 1u;
        /* Get the current realtek OUI bit[5:0] from PHYID2 */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PHYID2_OUI_LSB_LSP, CVB_PHY_PHYID2_OUI_LSB_LEN, &regValue));
        *pOui |= regValue;
        /* Get the Model number from PHYID2 */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PHYID2_MODEL_NUM_LSP, CVB_PHY_PHYID2_MODEL_NUM_LEN, &regValue));
        *pModleNum = (uint8)(regValue & 0xFFu);
        /* Get the Revision number from PHYID2 */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PHYID2_REV_NUM_LSP, CVB_PHY_PHYID2_REV_NUM_LEN, &regValue));
        *pRevNum = (uint8)(regValue & 0xFFu);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_PHYID_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_PHYID */

/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_PHYCR PHYCR
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_PHYCR_ASIC Low Level Driver
 * @brief PHYCR Low Level Driver
 * @{
 */
/**
 * @brief Configure the PHYCR to set the PHY role to slave or master.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100/1000BASE-T1 port to configure the PHY role.
 * @param[in] isMaster The role setting. \n
 *                     - isMaster = TRUE: Configure the PHY role of the designated port to master.
 *                     - isMaster = FALSE: Configure the PHY role of the designated port to slave.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_INPUT if the input parameter is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport(), cvb_reg_bit_get(), and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_phycr_role_set(uint32 unit, RtkPort portId, uint32 isMaster)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regOffset = 0x0u;
    uint32    regValue  = 0x0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((1u != isMaster) && (0u != isMaster))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }

        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get the PHYCR OCP offset of the designated system port */
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYCR) << 1u;
        /* Get the current PHY role configuration of the system port from PHYCR */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_get(unit, regOffset, CVB_PHYCR_ROLE_LSP, &regValue));
        /* If the current role configuration is different from the configuration role */
        if(((0u == regValue) && (1u == isMaster)) || ((1u == regValue) && (0u == isMaster)))
        {
            /* Update the input role configuration to the PHYCR bit[11] */
            CHK_FUN_CONTINUE(retVal, cvb_reg_bit_set(unit, regOffset, CVB_PHYCR_ROLE_LSP, isMaster));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Obtain the PHY role status.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100/1000BASE-T1 port index.
 * @param[out] isMaster The role setting. \n
 *                      - isMaster = TRUE: The PHY role of the designated port is master.
 *                      - isMaster = FALSE: The PHY role of the designated port is slave.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_INPUT if the input parameter is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_bit_get() and cvb_drv_userport_to_sysport().
 */
RtkApiRet cvb_phy_phycr_role_get(uint32 unit, RtkPort portId, uint32 *pIsMaster)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint8     userPort = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == pIsMaster)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get the current PHY role configuration of the system port from PHYCR */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYCR) << 1u, CVB_PHYCR_ROLE_LSP, pIsMaster));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYCR_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYCR */

/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_BMSR BMSR
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_BMSR_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing BMSR
 * @{
 */
/**
 * @brief Get the PHY link status.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100/1000BASE-T1 port index.
 * @param[out] pLinkSts The PHY link status. (PHY_LINKDOWN or PHY_LINKUP)\n
 *                      - PHY_LINKUP: PHY link up
 *                      - PHY_LINKDOWN: PHY link down
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport(), cvb_reg_read(), and cvb_reg_bit_get().
 */
RtkApiRet cvb_phy_bmsr_link_status_get(uint32 unit, RtkPort portId, uint32 *pLinkSts)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == pLinkSts)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get the current PHY link status from BMSR */
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMSR) << 1u;
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, pLinkSts));
        /* Because this is a latched-low register, it must be read twice to determine the current link status. */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_get(unit, regOffset, CVB_PHY_BMSR_LINK_STS_LSP, pLinkSts));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_BMSR_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_BMSR */

/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_PHYSR1 PHYSR1
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_PHYSR1_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing PHYSR1
 * @{
 */
/**
 * @brief Get the PHY PHYSR register value. The PHYSR register value contains the local and remote receiver status of 1000BASE-T1.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100/1000BASE-T1 port index.
 * @param[out] pRegVal Register value (0 to 0xFFFFFFFF). \n
 *                     The current register value.
 * @param[out] pRoleCfg The PHY role configuration status (0 or 1). \n
 *                      - 1: Master
 *                      - 0: Slave
 * @param[out] pLocRcvReady The local receiver status (0 or 1). \n
 *                          Indicates the local receiver status.
 *                          - 1: OK
 *                          - 0: Not OK
 * @param[out] pRemRcvReady The remote receiver status (0 or 1). \n
 *                          Indicates the remote receiver status.
 *                          - 1: OK
 *                          - 0: Not OK
 * @param[out] pIdleErrorCnt The idle error count (0 to 0xFF). \n
 *                           The counter stops automatically when it reaches 0xFF. \n
 *                           This counter is only available in 100Base-T1 mode. The pIdleErrorCnt value returns 0 if in 1000Base-T1 mode.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport() and cvb_reg_read().
 */
RtkApiRet cvb_phy_physr1_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint8 *pRoleCfg, uint8 *pLocRcvReady, uint8 *pRemRcvReady, uint8 *pIdleErrorCnt)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint8     userPort = 0u;
    RtkPort   sysPort  = 0u;
    uint32    regValue = 0u;
    uint32    speedVal = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_RTPGE_USER_PORT_MIN > portId) || (CVB_PHY_T1_RTPGE_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((NULL == pRegVal) || (NULL == pRoleCfg) || (NULL == pLocRcvReady) || (NULL == pRemRcvReady))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Check the current BMCR speed configuration */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, &regValue));
        speedVal = (((regValue >> CVB_PHY_SPD1_LSP) & 0x1u) << 1u) | ((regValue >> CVB_PHY_SPD0_LSP) & 0x1u);
        /* Get the register value of PHYSR1 */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYSR) << 1u, &regValue));
        *pRegVal = regValue;
        /* Get the role config bit value */
        *pRoleCfg = (uint8)((regValue >> CVB_PHY_PHYSR_ROLE_CFG_LSP) & 0x1u);
        /* If speed is 1000M */
        if(0x2u == speedVal)
        {
            /* Get the local receiver status bit value */
            *pLocRcvReady = (uint8)((regValue >> CVB_PHY_PHYSR_LOC_RCV_STS_LSP) & 0x1u);
            /* Get the remote receiver status bit value */
            *pRemRcvReady = (uint8)((regValue >> CVB_PHY_PHYSR_REM_RCV_STS_LSP) & 0x1u);
            /* The idle error counter value is 0 if the interface is 1000BASE-T1 */
            *pIdleErrorCnt = 0u;
        }
        else
        {
            *pLocRcvReady = 0u;
            *pRemRcvReady = 0u;
            /* Get the idle error counter value */
            *pIdleErrorCnt = (uint8)(regValue & 0xFFu);
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSR1_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSR1 */

/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_PHYSR2 PHYSR2
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_PHYSR2_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing PHYSR2
 * @{
 */
/**
 * @brief Get the PHY PHYSR2 register value.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100/1000BASE-T1 port index.
 * @param[out] pRegVal Register value (0 to 0xFFFFFFFF). \n
 *                     The current register value.
 * @param[out] pMasterMode The PHY role configuration status (0 or 1). \n
 *                         - 1: Master mode.
 *                         - 0: Slave mode.
 * @param[out] pSpeed The PHY speed (PHY_BMCR_SPEED_10M to PHY_BMCR_SPEED_1000M). \n
 *                    The current PHY link speed.
 *                    - 2'b00: 10M
 *                    - 2'b01: 100M
 *                    - 2'b10: 1000M
 * @param[out] pDuplex The PHY duplex mode (0 or 1). \n
 *                     The current PHY duplex mode.
 *                     - 1: Full duplex.
 *                     - 0: Half duplex.
 * @param[out] pRealTimelinkSts The PHY link status (0 or 1). \n
 *                              The real-time link status of the addressed PHY.
 *                              - 1: Link is OK.
 *                              - 0: Link is not OK.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport(), cvb_reg_read(), and cvb_reg_field_read().
 */
RtkApiRet cvb_phy_physr2_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint8 *pMasterMode, uint8 *pSpeed, uint8 *pDuplex, uint8 *pRealTimelinkSts)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint8     userPort = 0u;
    RtkPort   sysPort  = 0u;
    uint32    regValue = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((NULL == pRegVal) || (NULL == pMasterMode) || (NULL == pSpeed) || (NULL == pDuplex) || (NULL == pRealTimelinkSts))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get the register value of PHYSR2 */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYSR2) << 1u, &regValue));
        *pRegVal = regValue;
        /* Get the master mode bit value */
        *pMasterMode = (uint8)((regValue >> CVB_PHY_PHYSR2_MASTER_MODE_LSP) & 0x1u);
        /* Get the duplex mode bit value */
        *pDuplex = (uint8)((regValue >> CVB_PHY_PHYSR2_DUPLEX_LSP) & 0x1u);
        /* Get the link status bit value */
        *pRealTimelinkSts = (uint8)((regValue >> CVB_PHY_PHYSR2_LINK_STS_LSP) & 0x1u);
        /* Get the field value of speed */
        regValue = (regValue >> CVB_PHY_PHYSR2_SPEED_LSP) & ((CVB_PHY_PHYSR2_SPEED_LEN << 1u) - 1u);
        /* Get the PHY speed */
        if(0u == regValue)
        {
            *pSpeed = PHY_BMCR_SPEED_10M;
        }
        else if(1u == regValue)
        {
            *pSpeed = PHY_BMCR_SPEED_100M;
        }
        else if(2u == regValue)
        {
            *pSpeed = PHY_BMCR_SPEED_1000M;
        }
        else
        {
            /* regValue is 3 */
            *pSpeed = PHY_BMCR_SPEED_500M;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSR2_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSR2 */

/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_PHYSR4 PHYSR4
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_PHYSR4_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing PHYSR4
 * @{
 */
/**
 * @brief Get the PHY PHYSR4 register value. The PHYSR4 register value contains the local and remote receiver status of 100BASE-T1.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100BASE-T1 port.
 * @param[out] pRegVal Register value (0 to 0xFFFFFFFF). \n
 *                     The current register value.
 * @param[out] pRxPolarity The receive polarity (0 or 1). \n
 *                         The receive polarity for 100Base-T1.
 *                         - 1: Receive polarity is reversed.
 *                         - 0: Receive polarity is not reversed.
 * @param[out] pTxPolarity The transmit polarity (0 or 1). \n
 *                         The transmit polarity for 100Base-T1.
 *                         - 1: Transmit polarity is reversed.
 *                         - 0: Transmit polarity is not reversed.
 * @param[out] pRemoteReceiverSts The remote receiver status (0 or 1). \n
 *                                The remote receiver status of 100Base-T1.
 *                                - 1: The remote receiver is OK.
 *                                - 0: The remote receiver is not OK.
 * @param[out] pLocalReceiverSts The local receiver status (0 or 1). \n
 *                               The local receiver status of 100Base-T1.
 *                               - 1: The local receiver is OK.
 *                               - 0: The local receiver is not OK.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport() and cvb_reg_read().
 */
RtkApiRet cvb_phy_physr4_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint8 *pRxPolarity, uint8 *pTxPolarity, uint8 *pRemoteReceiverSts, uint8 *plocalReceiverSts)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regValue  = 0u;
    uint32    speedVal  = 0u;
    uint32    bmcrValue = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((NULL == pRegVal) || (NULL == pRxPolarity) || (NULL == pTxPolarity) || (NULL == pRemoteReceiverSts) || (NULL == plocalReceiverSts))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }
        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Check the current BMCR speed configuration */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_BMCR) << 1u, &bmcrValue));
        speedVal = (((bmcrValue >> CVB_PHY_SPD1_LSP) & 0x1u) << 1u) | ((bmcrValue >> CVB_PHY_SPD0_LSP) & 0x1u);
        /* Get the register value of PHYSR4 */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYSR4) << 1u, &regValue));
        *pRegVal = regValue;
        /* If speed is not 100M */
        if(0x1u == speedVal)
        {
            /* Get the RX polarity bit value */
            *pRxPolarity = (uint8)((regValue >> CVB_PHY_PHYSR4_RX_POLARITY_STS_LSP) & 0x1u);
            /* Get the TX polarity bit value */
            *pTxPolarity = (uint8)((regValue >> CVB_PHY_PHYSR4_TX_POLARITY_STS_LSP) & 0x1u);
            /* Get the remote receiver status bit value */
            *pRemoteReceiverSts = (uint8)((regValue >> CVB_PHY_PHYSR4_LOCRCV_STS_LSP) & 0x1u);
            /* Get the local receiver status bit value */
            *plocalReceiverSts = (uint8)((regValue >> CVB_PHY_PHYSR4_REMRCV_STS_LSP) & 0x1u);
        }
        else
        {
            /* all parameter return 0 */
            *pRxPolarity        = 0u;
            *pTxPolarity        = 0u;
            *pRemoteReceiverSts = 0u;
            *plocalReceiverSts  = 0u;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSR4_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSR4 */

/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_PHYSFR PHYSFR
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_PHYSFR_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing PHYSFR
 * @{
 */
/**
 * @brief Get the PHYSFR register value.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100BASE-T1 port.
 * @param[out] pRegVal Register value (0 to 0xFFFFFFFF). \n
 *                     The current register value.
 * @param[out] pPhySubFlag The PHY status indicator (0 to 7). \n
 *                         - PHYSFR Bit[2:0] = 0x7: Reserved.
 *                         - PHYSFR Bit[2:0] = 0x6: Reserved.
 *                         - PHYSFR Bit[2:0] = 0x5: PHY power down (BMCR bit 11 is set).
 *                         - PHYSFR Bit[2:0] = 0x4: IEEE Std. reset (BMCR bit 15 is set).
 *                         - PHYSFR Bit[2:0] = 0x3: PHY ready.
 *                         - PHYSFR Bit[2:0] = 0x2: Reserved.
 *                         - PHYSFR Bit[2:0] = 0x1: PHY initialization.
 *                         - PHYSFR Bit[2:0] = 0x0: Reserved.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport(), cvb_reg_read(), and cvb_reg_field_read().
 *
 * @note When the PHY is powered on, the register value of Bit[2:0] will turn into 0x3.
 */
RtkApiRet cvb_phy_physfr_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint32 *pPhySubFlag)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regValue  = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((NULL == pRegVal) || (NULL == pPhySubFlag))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }
        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get the PHYSFR address offset */
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHYSFR) << 1u;
        /* Get the PHY status sub-flag from PHYSFR */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PHYSFR_STS_SUB_FLAG_LSP, CVB_PHY_PHYSFR_STS_SUB_FLAG_LEN, &regValue));
        *pPhySubFlag = regValue;
        /* Read the reg value of PHYSFR */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, pRegVal));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSFR_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PHYSFR */

/**
 * @addtogroup PHY_100_1000_BASE_T1_BASIC_PCS PCS State
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_BASIC_PCS_ASIC Low Level Driver
 * @brief The Low Level Driver of PHY PCS status
 * @{
 */
/**
 * @brief Get the PHY PCS status.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100BASE-T1 port.
 * @param[out] pRegVal Register value (0 to 0xFFFFFFFF). \n
 *                     The current register value.
 * @param[out] pPhyPCS The PHY PCS status indicator (0 to 0xFF). \n
 *                     Operating in 1000BASE-T1: \n
 *                     - Bit[7:0] = 0x8X: Link Synchronization
 *                     - Bit[7:0] = 0x31: SLAVE SILENT
 *                     - Bit[7:0] = 0x32: TRAINING
 *                     - Bit[7:0] = 0x37: Link up
 *                     - Bit[7:0] = Other value: Reserved.
 *                     Operating in 100BASE-T1: \n
 *                     - Bit[7:0] = 0x00: Disable Transmitter State
 *                     - Bit[7:0] = 0x41: SLAVE SILENT
 *                     - Bit[7:0] = 0x42: TRAINING
 *                     - Bit[7:0] = 0x43: SEND IDLE
 *                     - Bit[7:0] = 0x44: SEND IDLE OR DATA
 *                     - Bit[7:0] = Other value: Reserved.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport() and cvb_reg_read().
 *
 * @note When the PHY is powered on, the register value of Bit[2:0] will turn into 0x3.
 */
RtkApiRet cvb_phy_pcs_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint32 *pPhyPCS)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regValue  = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((NULL == pRegVal) || (NULL == pPhyPCS))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }
        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Check the current register value */
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_PHY_PCS) << 1u;
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, &regValue));
        *pRegVal = regValue;
        /* Check the current PHY PCS */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PCS_LSP, CVB_PHY_PCS_LEN, &regValue));
        *pPhyPCS = regValue;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PCS_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_BASIC_PCS */

/**@}*/ /* PHY_100_1000_BASE_T1_BASIC */

/**
 * @addtogroup PHY_100_1000_BASE_T1_MIB_IPCR1 IPCR1
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_MIB_IPCR1_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing IPCR1
 * @{
 */
/**
 * @brief Enable or disable the PHY internal packet counter.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100BASE-T1 port.
 * @param[in] enRemoteLbk Remote loopback mode (DISABLE or ENABLE). \n
 *                        - ENABLE: Enable remote loopback mode.
 *                        - DISABLE: Disable remote loopback mode.
 * @param[in] enPktCounter PHY internal packet counter (DISABLE or ENABLE). \n
 *                         Enable or disable the PHY internal packet counter.
 *                         - ENABLE: Enable packet counter.
 *                         - DISABLE: Disable packet counter.
 * @param[in] pathSel The PHY PCS status indicator (0 or 1). \n
 *                    The selection of monitor path for the PHY counter.
 *                    - 0x1: Monitor MAC’s TX path.
 *                    - 0x0: Monitor PHY's RX path. normal path.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_INPUT if the input parameter is invalid.
 * @retval Others Please refer to cvb_drv_userport_to_sysport(), cvb_reg_field_write(), and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_ipcr1_info_cfg(uint32 unit, RtkPort portId, RtkEnable enRemoteLbk, RtkEnable enPktCounter, uint8 pathSel)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((DISABLE != enRemoteLbk) && (ENABLE != enRemoteLbk))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        if((DISABLE != enPktCounter) && (ENABLE != enPktCounter))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        if((0u != pathSel) && (1u != pathSel))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Configure remote loopback */
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_IPCR1) << 1u;
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, regOffset, CVB_PHY_IPCR1_REMOTE_LBK_LSP, CVB_PHY_IPCR1_REMOTE_LBK_LEN, enRemoteLbk));
        /* Enable or disable PHY internal packet counter */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_set(unit, regOffset, CVB_PHY_IPCR1_EN_INT_CNT_LSP, enPktCounter));
        /* Monitor path select */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, regOffset, CVB_PHY_IPCR1_PATH_SEL_LSP, CVB_PHY_IPCR1_PATH_SEL_LEN, pathSel));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_MIB_IPCR1_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_MIB_IPCR1 */

/**
 * @addtogroup PHY_100_1000_BASE_T1_MIB_IPCR2 IPCR2
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_MIB_IPCR2_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing IPCR2
 * @{
 */
/**
 * @brief Enable or disable the PHY internal packet counter.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100BASE-T1 port.
 * @param[in] intCntErrClr Clear error packet counter (0 or 1). \n
 *                         Write 1 to clear the error packet counter.
 * @param[in] intCntClr Clear PHY internal packet counter (0 or 1). \n
 *                      Write 1 to clear the PHY internal packet counter.
 * @param[in] rxPkCntRst Clear the received packet counter (0 or 1). \n
 *                       Write 1 to clear the received packet counter.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_INPUT if the input parameter is invalid.
 * @retval Others Please refer to cvb_drv_userport_to_sysport() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_ipcr2_info_cfg(uint32 unit, RtkPort portId, uint8 intCntErrClr, uint8 intCntClr, uint8 rxPkCntRst)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     userPort  = 0u;
    RtkPort   sysPort   = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((0u != intCntErrClr) && (1u != intCntErrClr))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        if((0u != intCntClr) && (1u != intCntClr))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        if((0u != rxPkCntRst) && (1u != rxPkCntRst))
        {
            retVal = -(RT_ERR_INPUT);
            continue;
        }
        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        regOffset = (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_IPCR2) << 1u;
        /* Clear error packet counter */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_set(unit, regOffset, CVB_PHY_IPCR2_CLR_CNTERR_LSP, intCntErrClr));
        /* Clear internal packet counter */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_set(unit, regOffset, CVB_PHY_IPCR2_CLR_INTCNT_LSP, intCntClr));
        /* Clear received packet counter */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_set(unit, regOffset, CVB_PHY_IPCR2_CLR_RXCNT_LSP, rxPkCntRst));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_MIB_IPCR2_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_MIB_IPCR2 */

/**
 * @addtogroup PHY_100_1000_BASE_T1_MIB_IPCR_CNT IPCR3_IPCR4_IEPCR
 * @{
 */
/**
 * @defgroup PHY_100_1000_BASE_T1_MIB_IPCR_CNT_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing IPCR3, IPCR4 and IEPCR
 * @{
 */
/**
 * @brief Read the PHY internal packet counter values.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_6TH). \n
 *                   The designated 100BASE-T1 port.
 * @param[out] intCntLSB Internal counter LSB (0 to 0xFFFF). \n
 *                       Value of bit[15:0] of the received packet counter.
 * @param[out] intCntMSB Internal counter MSB (0 to 0xFFFF). \n
 *                       Value of bit[31:16] of the received packet counter.
 * @param[out] intErrCnt Receive error packet counter (0 to 0xFFFF). \n
 *                       The receive error packet counter.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_ID if the input port index is invalid.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_drv_userport_to_sysport() and cvb_reg_bit_set().
 */
RtkApiRet cvb_phy_ipcr_cnt(uint32 unit, RtkPort portId, uint32 *intCntLSB, uint32 *intCntMSB, uint32 *intErrCnt)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint8     userPort = 0u;
    RtkPort   sysPort  = 0u;
    uint32    regValue = 0u;

    do
    {
        /* Check input parameters */
        if((CVB_PHY_T1_USER_PORT_MIN > portId) || (CVB_PHY_T1_USER_PORT_MAX < portId))
        {
            retVal = -(RT_ERR_PORT_ID);
            continue;
        }
        if((NULL == intCntLSB) || (NULL == intCntMSB) || (NULL == intErrCnt))
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Translate the user port index to the system port index */
        userPort = (uint8)(portId & 0xFFu);
        CHK_FUN_CONTINUE(retVal, cvb_drv_userport_to_sysport(userPort, &sysPort));
        /* Get Internal counter LSB */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_IPCR3) << 1u, &regValue));
        *intCntLSB = regValue;
        /* Get Internal counter MSB */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_IPCR4) << 1u, &regValue));
        *intCntLSB = regValue;
        /* Get error packet counter */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[sysPort] | CVB_OCP_IEPCR) << 1u, &regValue));
        *intCntLSB = regValue;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_100_1000_BASE_T1_MIB_IPCR_CNT_ASIC */
/**@}*/ /* PHY_100_1000_BASE_T1_MIB_IPCR_CNT */

/**
 * @addtogroup PHY_ETH PHY_ETH 10/1000BASE-T and 100BASE-TX PHY
 * @{
 */

/**
 * @addtogroup PHY_ETH_BASIC_BMCR BMCR
 * @{
 */
/**
 * @defgroup PHY_ETH_BASIC_BMCR_ASIC Low Level Driver
 * @brief Ethernet PHY BMCR ASIC driver
 * @{
 */

/**
 * @brief Trigger Ethernet PHY software reset.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The software reset was successful.
 * @retval Others Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_ethphy_bmcr_reset(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Set the BMCR bit[15]:reset */
    retVal = cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_RST_LSP, 1u);

    return retVal;
}

/**
 * @brief Get the PCS loopback mode enable setting of the Ethernet port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pEnableCfg The PCS loopback enable configuration (DISABLE or ENABLE). \n
 *                        Enable or disable the PCS loopback mode of the Ethernet port.
 *                        - *pEnableCfg = ENABLE: Enable the PHY PCS loopback mode.
 *                        - *pEnableCfg = DISABLE: Disable the PHY PCS loopback mode.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_bit_get().
 */
RtkApiRet cvb_ethphy_bmcr_pcs_loopback_get(uint32 unit, RtkEnable *pEnableCfg)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    bitVal = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pEnableCfg)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Get the BMCR bit[14]: PCS loopback */
        CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_DIGILBK_LSP, &bitVal));
        if(1u == bitVal)
        {
            *pEnableCfg = ENABLE;
        }
        else
        {
            *pEnableCfg = DISABLE;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable the PCS loopback mode of the Ethernet port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] enableCfg The value to set (DISABLE or ENABLE). \n
 *                      Enable or disable the PCS loopback mode of the Ethernet port.
 *                      - enableCfg = ENABLE: Enable the PHY PCS loopback mode.
 *                      - enableCfg = DISABLE: Disable the PHY PCS loopback mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_INPUT if the input enableCfg is invalid.
 * @retval Others Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_ethphy_bmcr_pcs_loopback_set(uint32 unit, RtkEnable enableCfg)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check input parameters */
    if((ENABLE != enableCfg) && (DISABLE != enableCfg))
    {
        retVal = -(RT_ERR_INPUT);
    }
    else
    {
        /* Configure the BMCR bit[14]: PCS loopback to input parameter value */
        retVal = cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_DIGILBK_LSP, enableCfg);
    }

    return retVal;
}

/**
 * @brief Get the BMCR speed setting of the Ethernet port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pSpeed Speed (PHY_BMCR_SPEED_10M or PHY_BMCR_SPEED_100M). \n
 *                    The BMCR speed setting of the Ethernet port.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_PHY_SPEED if the BMCR register value does not map to a valid PHY speed.
 * @retval Others Please refer to cvb_reg_read().
 */
RtkApiRet cvb_ethphy_bmcr_speed_get(uint32 unit, uint32 *pSpeed)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regVal   = 0u;
    uint32    speedVal = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pSpeed)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get the BMCR of the designated port */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, &regVal));
        /* Get the speed from bit6, bit13*/
        speedVal = (((regVal >> CVB_PHY_SPD1_LSP) & 0x1u) << 1u) | ((regVal >> CVB_PHY_SPD0_LSP) & 0x1u);
        switch(speedVal)
        {
            case 0x1u:
                *pSpeed = PHY_BMCR_SPEED_100M;
                break;
            case 0x0u:
                *pSpeed = PHY_BMCR_SPEED_10M;
                break;
            default:
                retVal = -(RT_ERR_PHY_SPEED);
                break;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the BMCR speed setting of the Ethernet port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] speed Speed (PHY_BMCR_SPEED_10M or PHY_BMCR_SPEED_100M). \n
 *                  The BMCR speed setting of the Ethernet port.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PHY_SPEED if the BMCR register value does not map to a valid PHY speed.
 * @retval Others Please refer to cvb_reg_read() and cvb_reg_write().
 */
RtkApiRet cvb_ethphy_bmcr_speed_set(uint32 unit, uint32 speed)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regVal   = 0u;
    uint32    speedVal = 0u;

    do
    {
        /* Mapping the speed setting to a BMCR register value */
        switch(speed)
        {
            case PHY_BMCR_SPEED_100M:
                speedVal = ((uint32)1u << CVB_PHY_SPD0_LSP);
                break;
            case PHY_BMCR_SPEED_10M:
                speedVal = 0u;
                break;
            default:
                retVal = -(RT_ERR_PHY_SPEED);
                break;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        /* Set the BMCR of the designated port */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, &regVal));
        /* Set bit6, bit13 to 0 */
        regVal &= 0xFFFFDFBFu;
        /* Update bit6, bit13 to current speed setting */
        regVal |= speedVal;
        CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, regVal));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the Ethernet port auto-negotiation setting.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pAnEnableCfg The auto-negotiation setting (DISABLE or ENABLE). \n
 *                          The Ethernet port auto-negotiation enable setting.
 *                          - *pAnEnableCfg = ENABLE: The auto-negotiation mode is enabled.
 *                          - *pAnEnableCfg = DISABLE: The auto-negotiation mode is disabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_bit_get().
 */
RtkApiRet cvb_ethphy_bmcr_an_mode_get(uint32 unit, RtkEnable *pAnEnableCfg)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    bitVal = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pAnEnableCfg)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get the BMCR bit[12]: AN mode */
        CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_AN_MODE_LSP, &bitVal));
        if(1u == bitVal)
        {
            *pAnEnableCfg = ENABLE;
        }
        else
        {
            *pAnEnableCfg = DISABLE;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable the auto-negotiation mode of the Ethernet ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] anEnableCfg The value to set (DISABLE or ENABLE). \n
 *                        Enable or disable the auto-negotiation mode of the Ethernet port.
 *                        - anEnableCfg = ENABLE: Enable the auto-negotiation mode.
 *                        - anEnableCfg = DISABLE: Disable the auto-negotiation mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_INPUT if the input anEnableCfg is invalid.
 * @retval Others Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_ethphy_bmcr_an_mode_set(uint32 unit, RtkEnable anEnableCfg)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check input parameters */
    if((ENABLE != anEnableCfg) && (DISABLE != anEnableCfg))
    {
        retVal = -(RT_ERR_INPUT);
    }
    else
    {
        /* Configure the BMCR bit[12]: AN mode to input parameter value */
        retVal = cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_AN_MODE_LSP, anEnableCfg);
    }

    return retVal;
}

/**
 * @brief Restart the auto-negotiation mode of the Ethernet ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_PORT_MASK if the input portMap is invalid.
 * @retval Others Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_ethphy_bmcr_restart_an(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Configure the BMCR bit[14]: PCS loopback to input parameter value */
    retVal = cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_AN_RESTART_LSP, 1u);

    return retVal;
}

/**
 * @brief Get the power down setting of the Ethernet port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pPwdCfg The power down setting (DISABLE or ENABLE). \n
 *                     The power down setting of the Ethernet port.
 *                     - pPwdCfg = ENABLE: Power down the Ethernet PHY.
 *                     - pPwdCfg = DISABLE: Do not power down the Ethernet PHY.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_bit_get().
 */
RtkApiRet cvb_ethphy_bmcr_power_down_get(uint32 unit, RtkEnable *pPwdCfg)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    bitVal = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pPwdCfg)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get the BMCR bit[11]: Power down */
        CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_PWRDN_LSP, &bitVal));
        if(1u == bitVal)
        {
            *pPwdCfg = ENABLE;
        }
        else
        {
            *pPwdCfg = DISABLE;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set or unset power down of the designated ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] pwdCfg The value to set (DISABLE or ENABLE). \n
 *                   Power down or not power down the Ethernet port.
 *                   - pwdCfg = ENABLE: Set power down.
 *                   - pwdCfg = DISABLE: Unset power down.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_INPUT if the input pwdCfg is invalid.
 * @retval Others Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_ethphy_bmcr_power_down_set(uint32 unit, RtkEnable pwdCfg)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check input parameters */
    if((ENABLE != pwdCfg) && (DISABLE != pwdCfg))
    {
        retVal = -(RT_ERR_INPUT);
    }
    else
    {
        /* Configure the BMCR bit[11]: power down to input parameter value */
        retVal = cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_PWRDN_LSP, pwdCfg);
    }
    return retVal;
}

/**
 * @brief Get the isolate mode setting of the Ethernet port.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pIsoCfg The isolate setting (DISABLE or ENABLE). \n
 *                     The isolate setting of the Ethernet port.
 *                     - *pIsoCfg = ENABLE: Isolate mode is enabled.
 *                     - *pIsoCfg = DISABLE: Isolate mode is disabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_bit_get().
 */
RtkApiRet cvb_ethphy_bmcr_isolate_get(uint32 unit, RtkEnable *pIsoCfg)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    bitVal = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pIsoCfg)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get the BMCR bit[10]: isolate */
        CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_MII_ISO_LSP, &bitVal));
        if(1u == bitVal)
        {
            *pIsoCfg = ENABLE;
        }
        else
        {
            *pIsoCfg = DISABLE;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable the isolate mode of the designated ports.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] isoCfg The value to set (DISABLE or ENABLE). \n
 *                   Enable or disable the isolate mode of the Ethernet port.
 *                   - isoCfg = ENABLE: Enable the isolate mode.
 *                   - isoCfg = DISABLE: Disable the isolate mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_INPUT if the input isoCfg is invalid.
 * @retval Others Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_ethphy_bmcr_isolate_set(uint32 unit, RtkEnable isoCfg)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check input parameters */
    if((ENABLE != isoCfg) && (DISABLE != isoCfg))
    {
        retVal = -(RT_ERR_INPUT);
    }
    else
    {
        /* Configure the BMCR bit[10]: isolate to input parameter value */
        retVal = cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMCR) << 1u, CVB_PHY_MII_ISO_LSP, isoCfg);
    }

    return retVal;
}

/**@}*/ /* PHY_ETH_BASIC_BMCR_ASIC */
/**@}*/ /* PHY_ETH_BASIC_BMCR */

/**
 * @addtogroup PHY_ETH_BASIC_BMSR BMSR
 * @{
 */
/**
 * @defgroup PHY_ETH_BASIC_BMSR_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing Ethernet BMSR
 * @{
 */
/**
 * @brief Get the Ethernet PHY link status.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pLinkSts The PHY link status. (PHY_LINKDOWN or PHY_LINKUP)\n
 *                      - PHY_LINKUP: PHY link up
 *                      - PHY_LINKDOWN: PHY link down
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_read() and cvb_reg_bit_get().
 */
RtkApiRet cvb_ethphy_bmsr_link_status_get(uint32 unit, uint32 *pLinkSts)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pLinkSts)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get the current PHY link status from BMSR */
        regOffset = (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_BMSR) << 1u;
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, pLinkSts));
        /* Because this is a latched-low register, it must be read twice to determine the current link status. */
        CHK_FUN_CONTINUE(retVal, cvb_reg_bit_get(unit, regOffset, CVB_PHY_BMSR_LINK_STS_LSP, pLinkSts));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_ETH_BASIC_BMSR_ASIC */
/**@}*/ /* PHY_ETH_BASIC_BMSR */

/**
 * @addtogroup PHY_ETH_BASIC_PHYSFR PHYSFR
 * @{
 */
/**
 * @defgroup PHY_ETH_BASIC_PHYSFR_ASIC Low Level Driver
 * @brief The Low Level Driver of accessing Ethernet PHYSFR
 * @{
 */
/**
 * @brief Get the PHYSFR register value.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pRegVal Register Value (0 to 0xFFFFFFFF). \n
 *                     The current register value.
 * @param[out] pPhySubFlag The PHY Status Indicator (0 to 7). \n
 *                         - 0x7: Reserved.
 *                         - 0x6: Reserved.
 *                         - 0x5: PHY power down (BMCR bit 11 is set).
 *                         - 0x4: IEEE Std. reset (BMCR bit 15 is set).
 *                         - 0x3: PHY ready.
 *                         - 0x2: Reserved.
 *                         - 0x1: PHY initialization.
 *                         - 0x0: Reserved.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_read() and cvb_reg_field_read().
 *
 * @note When the PHY is powered on, the register value of Bit[2:0] will turn into 0x3.
 */
RtkApiRet cvb_ethphy_physfr_info_cfg(uint32 unit, uint32 *pRegVal, uint32 *pPhySubFlag)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regValue  = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((NULL == pRegVal) || (NULL == pPhySubFlag))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get the PHYSFR address offset */
        regOffset = (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_PHYSFR) << 1u;
        /* Get the PHY status sub-flag from PHYSFR */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PHYSFR_STS_SUB_FLAG_LSP, CVB_PHY_PHYSFR_STS_SUB_FLAG_LEN, &regValue));
        *pPhySubFlag = regValue;
        /* Read the reg value of PHYSFR */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, pRegVal));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_ETH_BASIC_PHYSFR_ASIC */
/**@}*/ /* PHY_ETH_BASIC_PHYSFR */

/**
 * @addtogroup PHY_ETH_BASIC_PCS PCS State
 * @{
 */
/**
 * @defgroup PHY_ETH_BASIC_PCS_ASIC Low Level Driver
 * @brief The Low Level Driver of Ethernet PHY PCS status
 * @{
 */
/**
 * @brief Get the Ethernet PHY PCS status
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] pRegVal Register Value (0 to 0xFFFFFFFF) \n
 *                     The current register value.
 * @param[out] pPhyPCS The PHY PCS Status Indicator (0 to 0xFF) \n
 *                     Operating in 100BASE-T1: \n
 *                     - Bit[7:0] = 0x10: NWAY
 *                     - Bit[7:0] = 0x20: PWR10
 *                     - Bit[7:0] = 0x23: LINK10
 *                     - Bit[7:0] = 0x30: PWR100
 *                     - Bit[7:0] = 0x31: LINK100
 *                     - Bit[7:0] = 0x41: SLAVE SILENT
 *                     - Bit[7:0] = 0x42: TRAINING
 *                     - Bit[7:0] = 0x43: SEND IDLE
 *                     - Bit[7:0] = 0x44: SEND IDLE or DATA
 *                     - Bit[7:0] = Other value: Reserved.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_read().
 *
 * @note When the PHY has powered on, the register value of Bit[2:0] would be turning into 0x3.
 */
RtkApiRet cvb_ethphy_pcs_info_cfg(uint32 unit, uint32 *pRegVal, uint32 *pPhyPCS)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regValue  = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((NULL == pRegVal) || (NULL == pPhyPCS))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }
        /* Check the current register value */
        regOffset = (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_PHY_PCS) << 1u;
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, &regValue));
        *pRegVal = regValue;
        /* Check the current PHY PCS */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regOffset, CVB_PHY_PCS_LSP, CVB_PHY_PCS_LEN, &regValue));
        *pPhyPCS = regValue;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* PHY_ETH_BASIC_PCS_ASIC */
/**@}*/ /* PHY_ETH_BASIC_PCS */

/**
 * @addtogroup PHY_ETH_BASIC_ETH_PHYCR ETH PHYCR
 * @{
 */
/**
 * @defgroup PHY_ETH_BASIC_ETH_PHYCR_ASIC Low Level Driver
 * @brief The Low Level Driver of Ethernet PHY Control Register
 * @{
 */

/**
 * @brief Get the Ethernet PHY MDI/MDIX configuration.\n
 *
 * This function retrieves the current MDI/MDIX configuration of the Ethernet PHY based on the PHYCR register.
 *
 * @param[in] unit The stacking unit ID (1 to 7)\n
 *                 indicating which switch to access.
 * @param[out] pMdixMode Pointer to store the current MDI/MDIX configuration. (PHY_AUTO_MDIX_MODE/PHY_FORCE_MDI_MODE/PHY_FORCE_MDIX_MODE)\n
 *                       Possible values are:
 *                       - PHY_AUTO_MDIX_MODE: Automatic MDI/MDIX selection.
 *                       - PHY_FORCE_MDI_MODE: Manual MDI mode.
 *                       - PHY_FORCE_MDIX_MODE: Manual MDIX mode.
 *
 * @retval RT_ERR_OK The function executed successfully.
 * @retval -RT_ERR_NULL_POINTER A NULL pointer was passed as a parameter.
 * @retval Others Refer to the return values of the cvb_reg_read() function.
 */
RtkApiRet cvb_ethphy_mdix_mode_get(uint32 unit, uint32 *pMdixMode)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regValue  = 0u;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pMdixMode)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get the Ethernet PHYCR register address offset */
        regOffset = (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_ETHPHYCR) << 1u;

        /* Get the Ethernet PHYCR register value */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, &regValue));

        /* Determine the MDI setting based on the Ethernet PHYCR register value */
        if(1u == ((regValue >> CVB_PHY_ETHPHYCR_MNL_MDI_EN_LSP) & 0x1u))
        {
            /* Manual MDI/MDIX setting is enabled */
            if(1u == ((regValue >> CVB_PHY_ETHPHYCR_MNL_MDI_VAL_LSP) & 0x1u))
            {
                *pMdixMode = PHY_FORCE_MDI_MODE;
            }
            else
            {
                *pMdixMode = PHY_FORCE_MDIX_MODE;
            }
        }
        else
        {
            /* Auto MDI/MDIX mode */
            *pMdixMode = PHY_AUTO_MDIX_MODE;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the Ethernet PHY MDI/MDIX configuration by configuring the PHYCR register.\n
 *
 * This function sets the MDI/MDIX configuration of the Ethernet PHY by configuring the PHYCR register.
 *
 * @param[in] unit The stacking unit ID (1 to 7)\n
 *                 indicating which switch to access.
 * @param[in] mdixMode The desired MDI/MDIX configuration.(PHY_AUTO_MDIX_MODE/PHY_FORCE_MDI_MODE/PHY_FORCE_MDIX_MODE)\n
 *                     Possible values are:
 *                     - PHY_AUTO_MDIX_MODE: Automatic MDI/MDIX selection.
 *                     - PHY_FORCE_MDI_MODE: Manual MDI mode.
 *                     - PHY_FORCE_MDIX_MODE: Manual MDIX mode.
 *
 * @retval RT_ERR_OK The function executed successfully.
 * @retval -RT_ERR_INPUT The input MDI/MDIX configuration is invalid.
 * @retval Others Refer to the return values of the cvb_reg_field_write() function.
 */
RtkApiRet cvb_ethphy_mdix_mode_set(uint32 unit, uint32 mdixMode)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regOffset = 0u;

    do
    {
        /* Check input parameters */
        if((mdixMode != PHY_AUTO_MDIX_MODE) && (mdixMode != PHY_FORCE_MDI_MODE) && (mdixMode != PHY_FORCE_MDIX_MODE))
        {
            retVal = -(RtkApiRet)(RT_ERR_INPUT);
            continue;
        }

        /* Get MSCR register address offset */
        regOffset = (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_ETHPHYCR) << 1u;

        /* Set the MSCR role config */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, regOffset, CVB_PHY_ETHPHYCR_MNL_MDI_VAL_LSP, (CVB_PHY_ETHPHYCR_MNL_MDI_VAL_LEN + CVB_PHY_ETHPHYCR_MNL_MDI_EN_LEN), mdixMode));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the Ethernet PHY power-saving mode setting.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pEnPwrSaving The power-saving mode setting (DISABLE or ENABLE). \n
 *                          The Ethernet PHY power-saving mode enable setting.
 *                          - *pEnPwrSaving = ENABLE: The power-saving mode is enabled.
 *                          - *pEnPwrSaving = DISABLE: The power-saving mode is disabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval Others Please refer to cvb_reg_bit_get().
 */
RtkApiRet cvb_ethphy_power_save_en_get(uint32 unit, RtkEnable *pEnPwrSaving)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    bitVal = 0u;

    do
    {
        /* Check input parameters */
        if(NULL == pEnPwrSaving)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Get the power-saving mode bit */
        CHK_FUN_BREAK(retVal, cvb_reg_bit_get(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_ETHPHYCR) << 1u, CVB_PHY_ETHPHYCR_PWR_SAVING_LSP, &bitVal));
        /* Set the power-saving mode based on the bit value */
        if(1u == bitVal)
        {
            *pEnPwrSaving = ENABLE;
        }
        else
        {
            *pEnPwrSaving = DISABLE;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable the power-saving mode of the Ethernet PHY.
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] enPwrSaving The value to set (DISABLE or ENABLE). \n
 *                        Enable or disable the power-saving mode of the Ethernet PHY.
 *                        - enPwrSaving = ENABLE: Enable the power-saving mode.
 *                        - enPwrSaving = DISABLE: Disable the power-saving mode.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_INPUT if the input enPwrSaving is invalid.
 * @retval Others Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_ethphy_power_save_en_set(uint32 unit, RtkEnable enPwrSaving)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check input parameters */
    if((ENABLE != enPwrSaving) && (DISABLE != enPwrSaving))
    {
        retVal = -(RT_ERR_INPUT);
    }
    else
    {
        /* Configure the power-saving mode bit to the input parameter value */
        retVal = cvb_reg_bit_set(unit, (cvb_g_sysPortToOcpAddr[CVB_PHY_ETH_SYS_PORT_IDX] | CVB_OCP_ETHPHYCR) << 1u, CVB_PHY_ETHPHYCR_PWR_SAVING_LSP, enPwrSaving);
    }

    return retVal;
}
/**@}*/ /* PHY_ETH_BASIC_ETH_PHYCR_ASIC */
/**@}*/ /* PHY_ETH_BASIC_ETH_PHYCR */

/**@}*/ /* PHY_ETH */

/**@}*/ /* RTK_MODULE_PHY */
