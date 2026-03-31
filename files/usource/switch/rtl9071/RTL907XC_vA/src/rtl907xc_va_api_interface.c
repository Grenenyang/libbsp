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

#include "rtl907xc_va_asicdrv_phy.h"
#include "rtl907xc_va_asicdrv_port.h"
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_asicdrv_serdes.h"

/**
 * @addtogroup RTK_MODULE_INTERFACE Interface
 * @{
 */

/**
 * @addtogroup INTF_T1_CFG T1 Interface Configuration
 * @{
 */

/**
 * @defgroup INTF_T1_CFG_ASIC Low Level Driver
 * @brief T1 Interface Configuration Low Level Driver
 * @{
 */

/**
 * @brief Configure the addressed T1 port mode type to 100Base-T1 or 1000Base-T1.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_6TH) \n
 *                     The designated port id to configured the T1 interface mode.
 * @param[in] mode     The port mode (EM_MODE_EXT_100BASE_T1_PHY, EM_MODE_EXT_RTPG_T1_PHY) \n
 *                     Configure the addressed user port to EM_MODE_EXT_100BASE_T1_PHY or EM_MODE_EXT_RTPG_T1_PHY.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID   The input port is invalid
 * @retval -RT_ERR_PORT_MODE The input port mode is invalid
 * @retval Others               Please refer to cva_reg_field_write() and cva_reg_write().
 */
RtkApiRet cva_port_t1_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e mode)
{
    RtkApiRet ret               = RT_ERR_OK;
    RtkPort   port              = 0u;
    uint32    regFieldLen[2][3] = {{CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P6_T1GM_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P6_T1GM_CB_CFG_LEN, 0xF4800u},
                                   {CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P7_T1GM_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P7_T1GM_CB_CFG_LEN, 0x114800u}};
    uint32    regIdx;

    do
    {
        /* check the validation of portID */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* translate the user port index to the MAC port index */
        port = cva_g_userPort2sysPort[userPort];
        /* check MAC port index is valid or not */
        if((INTF_HW_PORT6 != port) && (INTF_HW_PORT7 != port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if(INTF_HW_PORT6 == port)
        {
            regIdx = 0u;
        }
        else
        {
            regIdx = 1u;
        }

        if(EM_MODE_EXT_100BASE_T1_PHY == mode)
        {
            /* if port mode is 100BASE-T1, set the port control config */
            CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_0, regFieldLen[regIdx][0], regFieldLen[regIdx][1], 0));
            /* set the PHY reg0 to 100M full duplex */
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regFieldLen[regIdx][2], 0x2100));
        }
        else if(EM_MODE_EXT_RTPG_T1_PHY == mode)
        {
            /* if port mode is 1000BASE-T1 */
            CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_0, regFieldLen[regIdx][0], regFieldLen[regIdx][1], 1));
            /* set the PHY reg0 to 1000M full duplex */
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regFieldLen[regIdx][2], 0x0140));
        }
        else
        {
            /* Unsupported port mode */
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* INTF_T1_CFG_ASIC */
/**@}*/ /* INTF_T1_CFG */

/**
 * @addtogroup INTF_LINK_STS Interface Mode and Link Status
 * @{
 */

/**
 * @defgroup INTF_LINK_STS_ASIC Low Level Driver
 * @brief Get Interface Mode and Current Link Status Low Level Driver
 * @{
 */

/**
 * @brief Get the port mode type of the addressed port
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort      port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                          The port id indicators which port mode is read back.
 * @param[out] mode         port interface mode
 *                          The pointer of port mode
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval RT_ERR_OK        To execute this function correctly
 * @retval Others           Please refer to cva_reg_read() and port_mode_get_fptr()
 */
RtkApiRet cva_port_mode_get(uint32 unit, RtkPort userPort, RtkModeExt_e *mode)
{
    uint32    portDisMask = 0u;
    RtkApiRet ret         = RT_ERR_OK;
    RtkPort   port        = 0u;
    /* The port mode type callback */
    RtkApiRet (*port_mode_get_fptr[15])(uint32 unit, RtkModeExt_e *mode);
    port_mode_get_fptr[2]  = &cva_drv_port_sub_port02_mode_get;
    port_mode_get_fptr[3]  = &cva_drv_port_sub_port03_mode_get;
    port_mode_get_fptr[4]  = &cva_drv_port_sub_port04_mode_get;
    port_mode_get_fptr[5]  = &cva_drv_port_sub_port05_mode_get;
    port_mode_get_fptr[6]  = &cva_drv_port_sub_port06_mode_get;
    port_mode_get_fptr[7]  = &cva_drv_port_sub_port07_mode_get;
    port_mode_get_fptr[8]  = &cva_drv_port_sub_port08_mode_get;
    port_mode_get_fptr[9]  = &cva_drv_port_sub_port09_mode_get;
    port_mode_get_fptr[10] = &cva_drv_port_sub_port10_mode_get;
    port_mode_get_fptr[11] = &cva_drv_port_sub_port11_mode_get;
    port_mode_get_fptr[12] = &cva_drv_port_sub_port12_mode_get;

    do
    {
        /* check the validation of portID */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* translate the user port index to the MAC port index */
        port = cva_g_userPort2sysPort[userPort];
        /* check port disable mask */
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, CVA_SMI_DMY0, &portDisMask));
        portDisMask &= ((uint32)0x1 << port);

        if(0u != portDisMask)
        {
            *mode = EM_MODE_EXT_PULLDOWN;
            continue;
        }
        /* get the port mode of the addressed port by the corresponding callback function */
        CHK_FUN_CONTINUE(ret, (*port_mode_get_fptr[port])(unit, mode))

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get port mac linking status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[out] pStatus          port link status (N/A) \n
 *                              To indicator the current link status of the addressed port.
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID       The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER  The pointer of link status is null
 * @retval Others               Please refer to cva_reg_read() and read_write()
 * @note The API could get port status (link/duplex/speed/nway/txpause/rxpause/forcemode).
 */
RtkApiRet cva_port_link_status_get(uint32 unit, RtkPort userPort, PortLinkStatus_t *pStatus)
{
    uint32    regVal                                     = 0u;
    RtkApiRet ret                                        = RT_ERR_OK;
    RtkPort   port                                       = 0u;
    uint32    portSpeed                                  = 0u;
    uint32    sysPort2SMIAddr[CVA_RTL9075_INTERFACE_NUM] = {CVA_PORT0_LINK_STATUS, CVA_PORT1_LINK_STATUS, CVA_PORT2_LINK_STATUS, CVA_PORT3_LINK_STATUS,
                                                            CVA_PORT4_LINK_STATUS, CVA_PORT5_LINK_STATUS, CVA_PORT6_LINK_STATUS, CVA_PORT7_LINK_STATUS,
                                                            CVA_PORT8_LINK_STATUS, CVA_PORT9_LINK_STATUS, CVA_PORT10_LINK_STATUS, CVA_PORT11_LINK_STATUS,
                                                            CVA_PORT12_LINK_STATUS, CVA_PORT13_LINK_STATUS, CVA_PORT14_LINK_STATUS, CVA_PORT15_LINK_STATUS};

    do
    {
        /* check the validation of portID */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL parameter */
        if(NULL == pStatus)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get the MAC port index of the addressed port index */
        port = cva_g_userPort2sysPort[userPort];
        /* get the MAC port link status */
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, sysPort2SMIAddr[port], &regVal));

        /* get the MAC port link up/down status */
        pStatus->link = ((0u != (regVal & 0x1u)) ? (uint8)PORT_LINKUP : (uint8)PORT_LINKDOWN);
        /* if the port mode type is serdes, get the speed from PHY PCS */
        if(RT_ERR_OK == cva_drv_sds_is_serdes_port(unit, userPort))
        {
            CHK_FUN_CONTINUE(ret, cva_drv_sds_port_speed_get(unit, port, &portSpeed));

            pStatus->speed  = (uint8)portSpeed;
            pStatus->duplex = (uint8)PORT_FULL_DUPLEX;
        }
        else
        {
            /* get the MAC port link speed */
            switch((regVal >> 1u) & 0x3u)
            {
                case 0x0u:
                    /* 2'b00: EM_PORT_SPEED_10M(0x1) */
                    pStatus->speed = (uint8)EM_PORT_SPEED_10M;
                    break;
                case 0x1u:
                    /* 2'b01: EM_PORT_SPEED_10M(0x2) */
                    pStatus->speed = (uint8)EM_PORT_SPEED_100M;
                    break;
                case 0x2u:
                    /* 2'b10: EM_PORT_SPEED_10M(0x4) */
                    pStatus->speed = (uint8)EM_PORT_SPEED_1000M;
                    break;
                default:
                    /* do nothing */
                    break;
            }
            /* get the duplex mode  */
            pStatus->duplex = (uint8)((0u != (regVal & ((uint32)0x1 << 3u))) ? (uint8)PORT_FULL_DUPLEX : (uint8)PORT_HALF_DUPLEX);
        }
        /* get the tx pause status */
        pStatus->txpause = (uint8)((0u != (regVal & ((uint32)0x1 << 4u))) ? 1u : 0u);
        /* get the rx pause status */
        pStatus->rxpause = (uint8)((0u != (regVal & ((uint32)0x1 << 5u))) ? 1u : 0u);

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set packet maximum length configuration of the specific port.
 *
 * This function sets the maximum packet length configuration for a specified port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7). \n
 *                 The stacking unit ID used to indicate which switch to access.
 * @param[in] enable Enable flag (0 to 1). \n
 *                   To enable the max length packet capability (1: enable, 0: disable). \n
 *                   Enable: set max packet length to 10k bytes (including VLAN tag). \n
 *                   Disable: set packet length to 1522 bytes (including VLAN tag).
 * @param[in] userPort Port ID (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END). \n
 *                     The addressed port ID to be configured.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in cva_reg_write().
 *
 * @note Jumbo frame support up to 10k bytes (including VLAN tag).
 */
RtkApiRet cva_port_pkt_maxlen_set(uint32 unit, RtkEnable enable, RtkPort userPort)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regValue;
    RtkPort   port;

    do
    {
        /* check port id is valid or not */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* get MAC port id */
        port = cva_g_userPort2sysPort[userPort];
        if(ENABLE == enable)
        {
            /* Set to 10K bytes */
            regValue = 0x6800u;
        }
        else
        {
            /* Set to 1522 bytes */
            regValue = 0x45F2u;
        }
        /* Configure the maximum packet length */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_PORT0_MAXLEN_CONTROL + ((uint32)0x4 * (uint32)port), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get packet maximum length configuration of the specific port.
 *
 * This function retrieves the maximum packet length configuration for a specified port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                              The port id indicators which port mode is read back.
 * @param[out] *maxLen          Register value (0x05F2 to 0x2800) \n
 *                              This return value is the max packet length which is configured for this port.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in cva_reg_read().
 *
 * @note Jumbo frame support to 10k bytes  (including vlan tag).
 */
RtkApiRet cva_port_pkt_maxlen_get(uint32 unit, RtkPort userPort, uint32 *maxLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port;
    uint32    regValue;

    do
    {
        /* check port id is valid or not */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* get MAC port id */
        port = cva_g_userPort2sysPort[userPort];
        /* get enable or disable maximum packet length configuration */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_PORT0_MAXLEN_CONTROL + ((uint32)0x4 * (uint32)port), &regValue));
        *maxLen = regValue & 0x3FFFu;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* INTF_LINK_STS_ASIC */
/**@}*/ /* INTF_LINK_STS */

/**
 * @addtogroup ETH_LINK_CFG Ethernet Link Configuration
 * @{
 */

/**
 * @defgroup ETH_LINK_CFG_ASIC Low Level Driver
 * @brief Ethernet interface configuration setting and getting Low Level Driver
 * @{
 */

/**
 * @brief Set Ethernet linking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort                  port id (CVA_RTK_USER_PORT_7TH)
 *                                      The port id indicators which port mode is be configured.
 * @param[in] pEthAbility               Ethernet link ability (N/A) \n
 *                                      This parameter include link speed/duplex and link status which to be set.
 * @retval RT_ERR_OK                    To execute this function correctly
 * @retval -RT_ERR_PORT_ID              The port ID is out of range
 * @retval -RT_ERR_PORT_MODE            The configured is not match for Ethernet
 * @retval -RT_ERR_MAC_FORCE_1000       The Ethernet doesn't support 1000 link speed
 * @retval Others                       Please refer to cva_port_mode_get(), cva_drv_port_eth_an_mode_set() and cva_drv_port_eth_force_mode_set.
 */
RtkApiRet cva_port_eth_link_status_set(uint32 unit, RtkPort userPort, const PortEthLinkAbility_t *pEthAbility)
{
    RtkModeExt_e mode;
    RtkApiRet    ret = SUCCESS;
    RtkPort      port;

    do
    {
        /* check input port index is valid or not */
        port = cva_g_userPort2sysPort[userPort];
        if(INTF_HW_PORT8 != port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* get the port mode type of the addressed port */
        CHK_FUN_CONTINUE(ret, cva_port_mode_get(unit, userPort, &mode));
        /* check the port mode of addressed port is Ethernet or not */
        if(mode != EM_MODE_EXT_FAST_ETHERNET)
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }
        /* check configure speed */
        if(0u != (pEthAbility->speed & (uint32)EM_PORT_SPEED_1000M))
        {
            ret = -RT_ERR_MAC_FORCE_1000;
            continue;
        }
        /* Configure port ability */
        if(1u == pEthAbility->autoNego)
        {
            /* configure auto-nego link ability */
            CHK_FUN_CONTINUE(ret, cva_drv_port_eth_link_status_an_mode_set(unit, pEthAbility));
        }
        else
        {
            /* configure force mode link ability */
            CHK_FUN_CONTINUE(ret, cva_drv_port_eth_link_status_force_mode_set(unit, pEthAbility));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get Ethernet linking status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort              port id (CVA_RTK_USER_PORT_7TH) \n
 *                                  The port id indicators which port mode is be configured.
 * @param[out] pStatus              Ethernet link status (N/A) \n
 *                                  To indicate the current link status.
 * @retval RT_ERR_OK                To execute this function correctly
 * @retval -RT_ERR_PORT_MODE        The interface is not configured as Ethernet mode
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval Others                   Please refer to cva_port_mode_get(), cva_reg_field_read() and cva_reg_read()
 */
RtkApiRet cva_port_eth_link_status_get(uint32 unit, RtkPort userPort, PortRtkEthLinkStatus_t *pStatus)
{
    uint32       phyIdx = 0xBu;
    uint32       regVal = 0u;
    RtkModeExt_e mode;
    RtkApiRet    ret = SUCCESS;
    RtkPort      port;

    do
    {
        /* get MAC port index and check the port index is valid or not */
        port = cva_g_userPort2sysPort[userPort];
        if(INTF_HW_PORT8 != port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* get the port mode type of the addressed port */
        CHK_FUN_CONTINUE(ret, cva_port_mode_get(unit, userPort, &mode));
        /* check the port mode of addressed port is Ethernet or not */
        if(mode != EM_MODE_EXT_FAST_ETHERNET)
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

        /* get reg 0 bit12: auto_nego */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, (((phyIdx << 16u) + CVA_OCP_BMCR) << 1u), 12u, 1u, &regVal));
        pStatus->autoNego = (uint8)(regVal & (uint8)0xFFu);
        /* if auto negotiation is enabled */
        if(1u == pStatus->autoNego)
        {
            /* Get speed & duplex */
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, (((phyIdx << 16u) + 0xA408u) << 1u), &regVal));
            pStatus->autoNegoCapability = 0;

            if(0u != (regVal & 0x0020u))
            {
                /* 10M HALF*/
                pStatus->autoNegoCapability |= (uint8)AN_10M_SPEED_HALF_DUPLEX;
            }
            if(0u != (regVal & 0x0040u))
            {
                /* 10M FULL*/
                pStatus->autoNegoCapability |= (uint8)AN_10M_SPEED_FULL_DUPLEX;
            }
            if(0u != (regVal & 0x0080u))
            {
                /* 100M HALF*/
                pStatus->autoNegoCapability |= (uint8)AN_100M_SPEED_HALF_DUPLEX;
            }
            if(0u != (regVal & 0x0100u))
            {
                /* 100M FULL*/
                pStatus->autoNegoCapability |= (uint8)AN_100M_SPEED_FULL_DUPLEX;
            }
        }
        else
        {
            /* if auto negotiation is disabled. force mode. */

            /* Get speed & duplex
             * speed [5:4].
             *      2'b00: 10M
             *      2'b01: 100M
             *      2'b10: 1000M
             * duplex [3:3]
             */
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, (((phyIdx << 16u) + 0xA434u) << 1u), &regVal));
            pStatus->speed  = 0;
            pStatus->duplex = 0;

            /* Check speed is valid or not */
            if(0u != (regVal & 0x20u))
            {
                continue;
            }

            if(0u != (regVal & 0x10u))
            {
                /* 100M */
                pStatus->speed |= (uint8)EM_PORT_SPEED_100M;
            }
            else
            {
                /* 10M */
                pStatus->speed |= (uint8)EM_PORT_SPEED_10M;
            }

            if(0u != (regVal & 0x8u))
            {
                /* Full duplex */
                pStatus->duplex |= (uint8)PORT_FULL_DUPLEX;
            }
            else
            {
                /* Half duplex */
                pStatus->duplex |= (uint8)PORT_HALF_DUPLEX;
            }
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* ETH_LINK_CFG_ASIC */
/**@}*/ /* ETH_LINK_CFG */

/**
 * @addtogroup INTF_RGMII_CFG RGMII Link Configuration
 * @{
 */

/**
 * @defgroup INTF_RGMII_CFG_ASIC Low Level Driver
 * @brief RGMII interface configuration setting and getting ASIC drivers.
 * @{
 */

/**
 * @brief Set RGMII TX delay mode. The value 0 means disable TX delay mode, and 1 means enable TX delay mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort               port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                                  The port id indicators which port to be configured
 * @param[in] delay                 TX delay (0 to 1)
 *                                  To indicate the tx delay is set or not, 0: not to set, 1: to set txdelay
 * @param[out] None
 * @retval  RT_ERR_OK               To execute this function correctly
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_OUT_OF_RANGE     The delay flag is out of range
 * @retval Others                   Please refer to cva_reg_read() and read_write()
 * @note  This API sets TX delay for a RGMII port.
 *      All TX signals excluding TX clock are delayed for 2ns in delay mode.
 */
RtkApiRet cva_port_rgmii_txdly_set(uint32 unit, RtkPort userPort, uint8 delay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {

        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(1u < delay)
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        switch(port)
        {
            case INTF_HW_PORT4:
                retVal = cva_reg_field_write(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGCFG_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT5:
                retVal = cva_reg_field_write(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGCFG_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT8:
                retVal = cva_reg_field_write(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGCFG_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT9:
                retVal = cva_reg_field_write(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGCFG_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT11:
                retVal = cva_reg_field_write(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL6, CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGCFG_LEN, (uint32)delay);
                break;
            default:
                retVal = -(RtkApiRet)RT_ERR_PORT_ID;
                break;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get RGMII TX delay mode. The value 0 means disable TX delay mode, and 1 means enable TX delay mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort              port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                                  The port id indicators which port to be configured
 * @param[out] pDelay               Delay pointer
 *                                  Used to store the read dealy value
 * @retval  RT_ERR_OK               To execute this function correctly
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER     The delay parameter pointer is null
 * @retval Others                   Please refer to cva_reg_read() and read_write()
 * @note This API gets TX delay for a RGMII port.
 *      All TX signals excluding TX clock are delayed for 2ns in delay mode.
 */
RtkApiRet cva_port_rgmii_txdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay)
{
    uint32    rgcfg;
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == pDelay)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        switch(port)
        {
            case INTF_HW_PORT4:
                retVal = cva_reg_field_read(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGCFG_LEN, &rgcfg);
                break;
            case INTF_HW_PORT5:
                retVal = cva_reg_field_read(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGCFG_LEN, &rgcfg);
                break;
            case INTF_HW_PORT8:
                retVal = cva_reg_field_read(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGCFG_LEN, &rgcfg);
                break;
            case INTF_HW_PORT9:
                retVal = cva_reg_field_read(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGCFG_LEN, &rgcfg);
                break;
            case INTF_HW_PORT11:
                retVal = cva_reg_field_read(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL6, CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGCFG_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGCFG_LEN, &rgcfg);
                break;
            default:
                retVal = -(RtkApiRet)RT_ERR_PORT_ID;
                break;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        *pDelay = (uint8)(rgcfg & (uint8)0xFFu);

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set RGMII RX delay mode. For VA, the value 1 means 1ns in 1000Mbps and around 1~3ns in 10M/100Mbps depended on PVT. For V2 and V3, the value 1 means 0.1~0.3ns depended on PVT.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[in] speed             Link speed
 *                              The link speed for this port
 * @param[in] delay             RX delay (0~3 for VA, 0~7 for others)
 *                              Used to configure the RGMII parameter for this port
 * @param[out] None
 * @retval RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_PHY_SPEED    The wrong speed which is not support by RGMII
 * @retval Others               Please refer to cva_reg_read() and read_write()
 * @note This API sets RX delay for a RGMII port.
 *      All RX signals excluding RX clock are delayed inside in delay mode.
 */
RtkApiRet cva_port_rgmii_rxdly_set(uint32 unit, RtkPort userPort, RtkPortSpeed_e speed, uint8 delay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        if((EM_PORT_SPEED_10M != speed) && (EM_PORT_SPEED_100M != speed) && (EM_PORT_SPEED_1000M != speed))
        {
            retVal = -(RtkApiRet)RT_ERR_PHY_SPEED;
            continue;
        }

        if(3u < delay)
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }
        retVal = cva_drv_port_rgmii_addll_set(unit, port, speed, delay);

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get RGMII RX delay mode. The value 1 is around 1ns (for VA) and 0.3ns (for others) depended on PVT.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[out] delay            RX delay
 *                              The RX delay value pointer which used to store the read back value.
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER The RX delay pointer is null
 * @retval Others               Please refer to cva_reg_read() and read_write()
 * @note This API gets RX delay for a RGMII port.
 *      All RX signals excluding RX clock are delayed inside in delay mode.
 */
RtkApiRet cva_port_rgmii_rxdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        if(NULL == pDelay)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        retVal = cva_drv_port_rgmii_addll_get(unit, port, pDelay);

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* INTF_RGMII_CFG_ASIC */
/**@}*/ /* INTF_RGMII_CFG */

/**
 * @addtogroup INTF_XMII_CFG XMII Interface Configuration
 * @{
 */

/**
 * @defgroup INTF_XMII_CFG_ASIC XMII Interface Configuration
 * @brief Configure the XMII interface Low Level Driver
 * @{
 */

/**
 * @brief Set xMII port force linking configuration
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[in] pMacAbility      Port link ability
 *                              The pointer of port ability configuration
 * @param[out] None
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID       The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER  The port ability pointer is null
 * @retval Others               Please refer to cva_reg_read() and read_write()
 * @note This API can set port MAC force mode properties.
 */
RtkApiRet cva_port_xmii_link_status_set(uint32 unit, RtkPort userPort, PortMacAbility_t *pMacAbility)
{
    uint32    regVal            = 0u;
    RtkApiRet ret               = RT_ERR_OK;
    uint32    sysPort2SMIAddr[] = {CVA_PORT0_PROPERTY_CONFIGURE, CVA_PORT1_PROPERTY_CONFIGURE, CVA_PORT2_PROPERTY_CONFIGURE, CVA_PORT3_PROPERTY_CONFIGURE,
                                   CVA_PORT4_PROPERTY_CONFIGURE, CVA_PORT5_PROPERTY_CONFIGURE, CVA_PORT6_PROPERTY_CONFIGURE, CVA_PORT7_PROPERTY_CONFIGURE,
                                   CVA_PORT8_PROPERTY_CONFIGURE, CVA_PORT9_PROPERTY_CONFIGURE, CVA_PORT10_PROPERTY_CONFIGURE, CVA_PORT11_PROPERTY_CONFIGURE,
                                   CVA_PORT12_PROPERTY_CONFIGURE, CVA_PORT13_PROPERTY_CONFIGURE, CVA_PORT14_PROPERTY_CONFIGURE, CVA_PORT15_PROPERTY_CONFIGURE};
    RtkPort   port              = 0u;
    do
    {
        /* check the validation of portID */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        if(NULL == pMacAbility)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, cva_drv_indirect_reg_field_get(unit, sysPort2SMIAddr[port], 0, 32u, &regVal));

        regVal &= (~0x3FFU);

        /* set port portability to MAC address */
        switch(pMacAbility->speed)
        {
            case(uint32)EM_PORT_SPEED_1000M:
                if(INTF_HW_PORT4 > port)
                {
                    ret = -RT_ERR_INPUT;
                }
                else
                {
                    regVal |= ((uint32)0x1 << 7u);
                }
                break;
            case(uint32)EM_PORT_SPEED_100M:
                regVal |= ((uint32)0x1 << 6u);
                break;
            case(uint32)EM_PORT_SPEED_10M:
                regVal |= ((uint32)0x1 << 4u);
                break;
            default:
                ret = RT_ERR_OK;
                break;
        }
        if(RT_ERR_OK != ret)
        {
            continue;
        }

        CHK_FUN_CONTINUE(ret, cva_drv_port_covert_port_ability_to_reg(&regVal, pMacAbility));

        CHK_FUN_CONTINUE(ret, cva_drv_indirect_reg_field_set(unit, sysPort2SMIAddr[port], 0, 32u, regVal));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set xMII TX driving strength which includes rising and falling. The bigger value means higher driving strength.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is configured.
 * @param[in] base              Driving base
 *                              TX strength base which affects rising and falling
 * @param[in] rise              Driving rise
 *                              TX rising strength
 * @param[in] fall              Driving fall
 *                              TX falling strength
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_OUT_OF_RANGE The base value is out of range
 * @retval Others               Please refer to cva_reg_read() and read_write()
 * @note This API sets pad driving strengthes for a RGMII port.
 */
RtkApiRet cva_port_xmii_driving_set(uint32 unit, RtkPort userPort, uint8 base, uint8 rise, uint8 fall)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    result0;
    uint32    result1;
    RtkPort   port;
    uint32    regIdx;
    uint32    regValue[4] = {0u, 0u, 0u, 0u};
    uint32    i           = 0u;

    const uint32 drivRegFieldLsp[3][4] = {{CVA_IO_CONTROL18_MODE2_P4_LSP, CVA_IO_CONTROL18_MODE_P4_LSP, CVA_IO_CONTROL18_DP_P4_LSP, CVA_IO_CONTROL18_DN_P4_LSP},
                                          {CVA_IO_CONTROL18_MODE2_P5_LSP, CVA_IO_CONTROL18_MODE_P5_LSP, CVA_IO_CONTROL18_DP_P5_LSP, CVA_IO_CONTROL18_DN_P5_LSP},
                                          {CVA_IO_CONTROL18_MODE2_P9_LSP, CVA_IO_CONTROL18_MODE_P9_LSP, CVA_IO_CONTROL18_DP_P9_LSP, CVA_IO_CONTROL18_DN_P9_LSP}};

    const uint32 drivRegFieldLen[3][4] = {{CVA_IO_CONTROL18_MODE2_P4_LEN, CVA_IO_CONTROL18_MODE_P4_LEN, CVA_IO_CONTROL18_DP_P4_LEN, CVA_IO_CONTROL18_DN_P4_LEN},
                                          {CVA_IO_CONTROL18_MODE2_P5_LEN, CVA_IO_CONTROL18_MODE_P5_LEN, CVA_IO_CONTROL18_DP_P5_LEN, CVA_IO_CONTROL18_DN_P5_LEN},
                                          {CVA_IO_CONTROL18_MODE2_P9_LEN, CVA_IO_CONTROL18_MODE_P9_LEN, CVA_IO_CONTROL18_DP_P9_LEN, CVA_IO_CONTROL18_DN_P9_LEN}};

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        if((3u < base) || (7u < rise) || (7u < fall))
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }

        result0 = (base > 1u) ? 1u : 0u;
        result1 = ((base % 2u) == 1u) ? 1u : 0u;
        switch(port)
        {
            case INTF_HW_PORT4:
                regIdx = 0u;
                break;
            case INTF_HW_PORT5:
            case INTF_HW_PORT8:
            case INTF_HW_PORT11:
                regIdx = 1u;
                break;
            case INTF_HW_PORT9:
                regIdx = 2u;
                break;
            default:
                retVal = -(RtkApiRet)RT_ERR_PORT_ID;
                break;
        }

        if(RT_ERR_OK != retVal)
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }
        regValue[0] = result0;
        regValue[1] = result1;
        regValue[2] = rise;
        regValue[3] = fall;
        for(i = 0; i < 4u; i++)
        {
            CHK_FUN_BREAK(retVal, cva_reg_field_write(unit, CVA_IO_CONTROL18, drivRegFieldLsp[regIdx][i], drivRegFieldLen[regIdx][i], regValue[i]));
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get xMII TX driving strength which includes rising and falling. The bigger value means higher driving strength.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort              port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                                  The port id indicators which port mode is read back.
 * @param[out] pBase                Base value pointer
 *                                  The pointer of TX strength base
 * @param[out] pRise                Rise value pointer
 *                                  The pointer of TX rising strength
 * @param[out] pFall                Fall value pointer
 *                                  The pointer of TX falling strength
 * @retval RT_ERR_OK                To execute this function correctly
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER     The pointer is null.
 * @retval Others                   Please refer to cva_reg_read() and read_write()
 * @note This API gets pad driving strengthes for a RGMII port.
 */
RtkApiRet cva_port_xmii_driving_get(uint32 unit, RtkPort userPort, uint8 *pBase, uint8 *pRise, uint8 *pFall)
{
    uint32    m2;
    uint32    m1;
    uint32    dp;
    uint32    dn;
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port;
    uint32    regVal;

    uint32 regFieldLen[3][8] = {{CVA_IO_CONTROL18_MODE2_P4_LSP, CVA_IO_CONTROL18_MODE2_P4_LEN, CVA_IO_CONTROL18_MODE_P4_LSP, CVA_IO_CONTROL18_MODE_P4_LEN,
                                 CVA_IO_CONTROL18_DP_P4_LSP, CVA_IO_CONTROL18_DP_P4_LEN, CVA_IO_CONTROL18_DN_P4_LSP, CVA_IO_CONTROL18_DN_P4_LEN},
                                {CVA_IO_CONTROL18_MODE2_P5_LSP, CVA_IO_CONTROL18_MODE2_P5_LEN, CVA_IO_CONTROL18_MODE_P5_LSP, CVA_IO_CONTROL18_MODE_P5_LEN,
                                 CVA_IO_CONTROL18_DP_P5_LSP, CVA_IO_CONTROL18_DP_P5_LEN, CVA_IO_CONTROL18_DN_P5_LSP, CVA_IO_CONTROL18_DN_P5_LEN},
                                {CVA_IO_CONTROL18_MODE2_P9_LSP, CVA_IO_CONTROL18_MODE2_P9_LEN, CVA_IO_CONTROL18_MODE_P9_LSP, CVA_IO_CONTROL18_MODE_P9_LEN,
                                 CVA_IO_CONTROL18_DP_P9_LSP, CVA_IO_CONTROL18_DP_P9_LEN, CVA_IO_CONTROL18_DN_P9_LSP, CVA_IO_CONTROL18_DN_P9_LEN}};
    uint32 regIdx;

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        if((NULL == pBase) || (NULL == pRise) || (NULL == pFall))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        if((uint32)INTF_HW_PORT4 == port)
        {
            regIdx = 0u;
        }
        else if(((uint32)INTF_HW_PORT5 == port) || ((uint32)INTF_HW_PORT8 == port) || ((uint32)INTF_HW_PORT11 == port))
        {
            regIdx = 1u;
        }
        else if((uint32)INTF_HW_PORT9 == port)
        {
            regIdx = 2u;
        }
        else
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_IO_CONTROL18, &regVal));

        m2 = (regVal >> regFieldLen[regIdx][0]) & ((0x1u << regFieldLen[regIdx][1]) - 1u);
        m1 = (regVal >> regFieldLen[regIdx][2]) & ((0x1u << regFieldLen[regIdx][3]) - 1u);
        dp = (regVal >> regFieldLen[regIdx][4]) & ((0x1u << regFieldLen[regIdx][5]) - 1u);
        dn = (regVal >> regFieldLen[regIdx][6]) & ((0x1u << regFieldLen[regIdx][7]) - 1u);

        *pBase = (uint8)((((m2 & 0xFFu) * 0x2u) + (m1 & 0xFFu)) & 0xFFu);
        *pRise = (uint8)(dp & 0xFFu);
        *pFall = (uint8)(dn & 0xFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set xMII RX and TX single sampling edge, user could set as positive latch or negative latch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is configured.
 * @param[in] isRxNegSample     RX sampling edge
 *                              Set RX sampling edge, false = Negative sampling, true = Postive sampling.
 * @param[in] isTxNegSample     TX sampling edge
 *                              Set TX sampling edge, false = Negative sampling, true = Postive sampling.
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to cva_reg_read() and read_write()
 * @note This API set the sampling edge for xmii RX and TX direction.
 */
RtkApiRet cva_port_xmii_latch_edge_set(uint32 unit, RtkPort userPort, bool isRxNegSample, bool isTxNegSample)
{
    RtkApiRet retVal         = RT_ERR_OK;
    RtkPort   port           = 0u;
    uint8     miiIdx         = 0xFFu;
    uint32    edgeLatchField = 0u;

    const uint32 edgeLatchCtrlRegList[5] = {CVA_GLOBAL_MAC_INTERFACE_INTERNAL4,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL4,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL6};
    const uint32 edgeLatchCtrlLsp[5]     = {CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_MII_SEL_NEG_LSP};
    const uint32 edgeLatchCtrlLen[5]     = {CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_MII_SEL_NEG_LEN};
    const uint8  sysPortToMiiIdx[]       = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0, 1, 0xFFu, 0xFFu, 2, 3, 0xFFu, 4, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        miiIdx = sysPortToMiiIdx[port];

        if(0xFFu == miiIdx)
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(isRxNegSample)
        {
            edgeLatchField |= 0x1u;
        }

        if(isTxNegSample)
        {
            edgeLatchField |= 0x2u;
        }

        CHK_FUN_BREAK(retVal, cva_reg_field_write(unit, edgeLatchCtrlRegList[miiIdx], edgeLatchCtrlLsp[miiIdx], edgeLatchCtrlLen[miiIdx], edgeLatchField));

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get xMII RX and TX single sampling edge, user could set as positive latch or negative latch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is configured.
 * @param[out] isRxNegSample    RX sampling edge
 *                              Get RX sampling edge, false = Negative sampling, true = Postive sampling.
 * @param[out] isTxNegSample    TX sampling edge
 *                              Get TX sampling edge, false = Negative sampling, true = Postive sampling.
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to cva_reg_read() and read_write()
 * @note This API get the current sampling edge for xmii RX and TX direction.
 */
RtkApiRet cva_port_xmii_latch_edge_get(uint32 unit, RtkPort userPort, bool *isRxNegSample, bool *isTxNegSample)
{
    RtkApiRet retVal         = RT_ERR_OK;
    RtkPort   port           = 0u;
    uint8     miiIdx         = 0xFFu;
    uint32    edgeLatchField = 0u;

    const uint32 edgeLatchCtrlRegList[5] = {CVA_GLOBAL_MAC_INTERFACE_INTERNAL4,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL4,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL6};
    const uint32 edgeLatchCtrlLsp[5]     = {CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_MII_SEL_NEG_LSP,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_MII_SEL_NEG_LSP};
    const uint32 edgeLatchCtrlLen[5]     = {CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_MII_SEL_NEG_LEN,
                                            CVA_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_MII_SEL_NEG_LEN};
    const uint8  sysPortToMiiIdx[]       = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0, 1, 0xFFu, 0xFFu, 2, 3, 0xFFu, 4, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        miiIdx = sysPortToMiiIdx[port];

        if(0xFFu == miiIdx)
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_BREAK(retVal, cva_reg_field_read(unit, edgeLatchCtrlRegList[miiIdx], edgeLatchCtrlLsp[miiIdx], edgeLatchCtrlLen[miiIdx], &edgeLatchField));

        if(0u != (edgeLatchField & 0x1u))
        {
            *isRxNegSample = true;
        }
        else
        {
            *isRxNegSample = false;
        }

        if(0u != (edgeLatchField & 0x2u))
        {
            *isTxNegSample = true;
        }
        else
        {
            *isTxNegSample = false;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the signal to spread over the widest range of frequencies.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicates which port mode is configured.
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to cva_drv_indirect_reg_field_get()
 * @note This API set the signal to spread over the widest range of frequencies.
 */
RtkApiRet cva_port_xmii_ssc_set(uint32 unit, RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_port_xmii_ssc_err_chk(userPort, isEnable, sequenceModeOffset, phaseOffset));

        port = cva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(retVal, cva_drv_port_xmii_ssc_set(unit, port, isEnable, sequenceModeOffset, phaseOffset));

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the signal to spread over the widest range of frequencies.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END)
 *                              The port id indicates which port mode is configured.
 * @param[out] isEnable         Spread signal contoller enable.
 *                              Get spread signal contoller state, 0 = enable , 1 = disable spread signal contoller
 * @param[out] sequenceMode     Phase offset in sequence mode
 *                              Get Phase offset in sequence mode, 0x3 = default pffset , 0x7 = maximum phase offset
 * @param[out] phaseOffset      SSC controller to output the phase offset
 *                              Get SSC controller to output the phase offset, 0x3 = default pffset , 0x7 = maximum phase offset
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to cva_drv_indirect_reg_field_get()
 * @note This API get the signal to spread over the widest range of frequencies.
 */
RtkApiRet cva_port_xmii_ssc_get(uint32 unit, RtkPort userPort, RtkEnable *isEnable, uint32 *sequenceModeOffset, uint32 *phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        if(0u == CVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cva_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT4) && (port != INTF_HW_PORT5) && (port != INTF_HW_PORT8) && (port != INTF_HW_PORT9) && (port != INTF_HW_PORT11))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cva_drv_port_xmii_ssc_get(unit, port, isEnable, sequenceModeOffset, phaseOffset));

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* INTF_XMII_CFG_ASIC */
/**@}*/ /* INTF_XMII_CFG */

/**@}*/ /* Interface */
