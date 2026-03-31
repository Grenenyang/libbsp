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

#include "rtl907xd_v1_asicdrv_phy.h"
#include "rtl907xd_v1_asicdrv_port.h"
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_asicdrv_serdes.h"

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
 * @param[in] userPort port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_6TH) \n
 *                     The designated port id to configured the T1 interface mode.
 * @param[in] mode     The port mode (EM_MODE_EXT_100BASE_T1_PHY, EM_MODE_EXT_RTPG_T1_PHY) \n
 *                     Configure the addressed user port to EM_MODE_EXT_100BASE_T1_PHY or EM_MODE_EXT_RTPG_T1_PHY.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID   The input port is invalid
 * @retval -RT_ERR_PORT_MODE The input port mode is invalid
 * @retval Others            Please refer to dv1_reg_field_write() and dv1_reg_write().
 */
RtkApiRet dv1_port_t1_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e mode)
{
    RtkApiRet ret  = RT_ERR_OK;
    RtkPort   port = 0u;
    do
    {
        /* check the validation of portID */
        if(0u == DV1_IS_VALID_T1PHY_PORT_ID(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* translate the user port index to the MAC port index */
        port = dv1_g_userPort2sysPort[userPort];
        if(EM_MODE_EXT_100BASE_T1_PHY == mode)
        {
            /* if port mode is 100BASE-T1, set the port control config */
            CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, (uint32)COMBO_INTF_T1PHY_MII));
            /* set the PHY reg0 to 100M full duplex */
            CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, (((0x10000u * port) | 0xA400u) * 2u), 0x2100u));
        }
        else if(EM_MODE_EXT_RTPG_T1_PHY == mode)
        {
            /* if port mode is 1000BASE-T1 */
            CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, (uint32)COMBO_INTF_T1PHY_GMII));
            /* set the PHY reg0 to 1000M full duplex */
            CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, (((0x10000u * port) | 0xA400u) * 2u), 0x0140u));
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
 * @param[in] userPort      port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                          The port id indicators which port mode is read back.
 * @param[out] mode         port interface mode
 *                          The pointer of port mode
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval RT_ERR_OK        To execute this function correctly
 * @retval Others           Please refer to dv1_reg_field_read() and port_mode_get_fptr()
 */
RtkApiRet dv1_port_mode_get(uint32 unit, RtkPort userPort, RtkModeExt_e *mode)
{
    RtkApiRet ret  = RT_ERR_OK;
    RtkPort   port = 0u;
    uint32    ifType;
    /* The port mode type callback */
    RtkApiRet (*port_mode_get_fptr[15])(uint32 unit, RtkModeExt_e *mode);
    port_mode_get_fptr[1]  = &dv1_drv_port_sub_port01_mode_get;
    port_mode_get_fptr[2]  = &dv1_drv_port_sub_port02_mode_get;
    port_mode_get_fptr[3]  = &dv1_drv_port_sub_port03_mode_get;
    port_mode_get_fptr[4]  = &dv1_drv_port_sub_port04_mode_get;
    port_mode_get_fptr[5]  = &dv1_drv_port_sub_port05_mode_get;
    port_mode_get_fptr[6]  = &dv1_drv_port_sub_port06_mode_get;
    port_mode_get_fptr[7]  = &dv1_drv_port_sub_port07_mode_get;
    port_mode_get_fptr[8]  = &dv1_drv_port_sub_port08_mode_get;
    port_mode_get_fptr[9]  = &dv1_drv_port_sub_port09_mode_get;
    port_mode_get_fptr[10] = &dv1_drv_port_sub_port10_mode_get;
    port_mode_get_fptr[11] = &dv1_drv_port_sub_port11_mode_get;
    port_mode_get_fptr[12] = &dv1_drv_port_sub_port12_mode_get;

    do
    {
        /* check the validation of portID */
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* translate the user port index to the MAC port index */
        port = dv1_g_userPort2sysPort[userPort];
        /* check the port configuration is disabled or note */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));
        if(COMBO_INTF_DISB == ifType)
        {
            *mode = EM_MODE_EXT_PULLDOWN;
        }
        else
        {
            /* get the port mode of the addressed port by the corresponding callback function */
            CHK_FUN_CONTINUE(ret, (*port_mode_get_fptr[port])(unit, mode))
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get port mac linking status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[out] pStatus          port link status (N/A) \n
 *                              To indicator the current link status of the addressed port.
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID       The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER  The pointer of link status is null
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note The API could get port status (link/duplex/speed/nway/txpause/rxpause/forcemode).
 */
RtkApiRet dv1_port_link_status_get(uint32 unit, RtkPort userPort, PortLinkStatus_t *pStatus)
{
    uint32    regVal                                     = 0u;
    RtkApiRet ret                                        = RT_ERR_OK;
    RtkPort   port                                       = 0u;
    uint32    portSpeed                                  = 0u;
    uint32    sysPort2SMIAddr[DV1_RTL9075_INTERFACE_NUM] = {DV1_PORT0_LINK_STATUS, DV1_PORT1_LINK_STATUS, DV1_PORT2_LINK_STATUS, DV1_PORT3_LINK_STATUS,
                                                            DV1_PORT4_LINK_STATUS, DV1_PORT5_LINK_STATUS, DV1_PORT6_LINK_STATUS, DV1_PORT7_LINK_STATUS,
                                                            DV1_PORT8_LINK_STATUS, DV1_PORT9_LINK_STATUS, DV1_PORT10_LINK_STATUS, DV1_PORT11_LINK_STATUS,
                                                            DV1_PORT12_LINK_STATUS, DV1_PORT13_LINK_STATUS, DV1_PORT14_LINK_STATUS, DV1_PORT15_LINK_STATUS};

    do
    {
        /* check the validation of portID */
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
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
        port = dv1_g_userPort2sysPort[userPort];
        /* get the MAC port link status */
        CHK_FUN_CONTINUE(ret, dv1_reg_read(unit, sysPort2SMIAddr[port], &regVal));

        /* get the MAC port link up/down status */
        pStatus->link = ((0u != (regVal & 0x1u)) ? (uint8)PORT_LINKUP : (uint8)PORT_LINKDOWN);

        /* if the port mode type is serdes, get the speed from PHY PCS */
        if(RT_ERR_OK == dv1_drv_sds_is_serdes_port(unit, userPort))
        {
            CHK_FUN_CONTINUE(ret, dv1_drv_sds_port_speed_get(unit, port, &portSpeed));
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
 * @brief Set packet maximum length configuration of the specific port. \n
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] enable           Enable flag (0 to 1) \n
 *                             To enable the max length packet capability (1: enable, the other: disable) \n
 *                             Enable: to set max pkt length 10k bytes (including vlan tag). \n
 *                             Disable: to set pkt length 1522 byte (including vlan tag).
 * @param[in] userPort         port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
 *                             The addressed port id to be configured.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in dva_drv_indirect_reg_field_set().
 *
 * @note Jumbo frame support to 10k bytes  (including vlan tag).
 */
RtkApiRet dv1_port_pkt_maxlen_set(uint32 unit, RtkEnable enable, RtkPort userPort)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regValue;
    RtkPort   port;

    do
    {
        /* check port id is valid or not */
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* get MAC port id */
        port = dv1_g_userPort2sysPort[userPort];
        if(ENABLE == enable)
        {
            /* Set to 10K bytes */
            regValue = 0x2800u;
        }
        else
        {
            /* Set to 1522 bytes */
            regValue = 0x05F2u;
        }
        /* Configure the maximum packet length */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_MAXLEN_CONTROL + (PERPORT_REG_OFFSET * (uint32)port), DV1_PORT0_MAXLEN_CONTROL_MAXLEN_LSP, DV1_PORT0_MAXLEN_CONTROL_MAXLEN_LEN, regValue));
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
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
 *                              The port id indicators which port mode is read back.
 * @param[out] *maxLen          Register value (0x05F2 to 0x2800) \n
 *                              This return value is the max packet length which is configured for this port.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in dva_drv_indirect_reg_field_get().
 *
 * @note Jumbo frame support to 10k bytes  (including vlan tag).
 */
RtkApiRet dv1_port_pkt_maxlen_get(uint32 unit, RtkPort userPort, uint32 *maxLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port;
    uint32    regValue;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PORT0_MAXLEN_CONTROL + (PERPORT_REG_OFFSET * (uint32)port), DV1_PORT0_MAXLEN_CONTROL_MAXLEN_LSP, DV1_PORT0_MAXLEN_CONTROL_MAXLEN_LEN, &regValue));
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
 * @brief Set ETH linking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort                  port id (DV1_RTK_USER_PORT_7TH) \n
 *                                      The port id indicators which port mode is be configured.
 * @param[in] *pEthAbility              ETH link ability (N/A) \n
 *                                      This parameter include link speed/duplex and link status which to be set.
 * @retval RT_ERR_OK                    To execute this function correctly
 * @retval -RT_ERR_PORT_ID              The port ID is out of range
 * @retval -RT_ERR_PORT_MODE            The configured is not match for Ethernet
 * @retval -RT_ERR_MAC_FORCE_1000       The Ethernet doesn't support 1000 link speed
 * @retval Others                       Please refer to dv1_port_mode_get(), dv1_drv_port_eth_an_mode_set() and dv1_drv_port_eth_force_mode_set.
 */
RtkApiRet dv1_port_eth_link_ability_set(uint32 unit, RtkPort userPort, const PortEthLinkAbility_t *pEthAbility)
{
    RtkModeExt_e mode;
    RtkApiRet    ret = SUCCESS;
    RtkPort      port;

    do
    {
        /* check input port index is valid or not */
        port = dv1_g_userPort2sysPort[userPort];
        if(INTF_HW_PORT7 != port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* get the port mode type of the addressed port */
        CHK_FUN_CONTINUE(ret, dv1_port_mode_get(unit, userPort, &mode));
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
            CHK_FUN_CONTINUE(ret, dv1_drv_port_eth_an_mode_set(unit, pEthAbility));
        }
        else
        {
            /* configure force mode link ability */
            CHK_FUN_CONTINUE(ret, dv1_drv_port_eth_force_mode_set(unit, pEthAbility));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get ETH linking status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort              port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                                  The port id indicators which port mode is be configured.
 * @param[out] pStatus              ETH link status
 *                                  To indicate the current link status.
 * @retval RT_ERR_OK                To execute this function correctly
 * @retval -RT_ERR_PORT_MODE        The interface is not configured as ETH mode
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval Others                   Please refer to dv1_port_mode_get(), dv1_reg_field_read() and dv1_reg_read()
 */
RtkApiRet dv1_port_eth_link_ability_get(uint32 unit, RtkPort userPort, PortRtkEthLinkStatus_t *pStatus)
{
    uint32       phyIdx = 0xBu;
    uint32       regVal = 0u;
    RtkModeExt_e mode;
    RtkApiRet    ret = SUCCESS;
    RtkPort      port;

    do
    {
        /* get MAC port index and check the port index is valid or not */
        port = dv1_g_userPort2sysPort[userPort];
        if(INTF_HW_PORT7 != port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* get the port mode type of the addressed port */
        CHK_FUN_CONTINUE(ret, dv1_port_mode_get(unit, userPort, &mode));
        /* check the port mode of addressed port is Ethernet or not */
        if(mode != EM_MODE_EXT_FAST_ETHERNET)
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

        /* get reg 0 bit12: auto_nego */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (((phyIdx << 16u) + DV1_OCP_BMCR) << 1u), 12u, 1u, &regVal));
        pStatus->autoNego = (uint8)(regVal & (uint8)0xFFu);
        /* if auto negotiation is enabled */
        if(1u == pStatus->autoNego)
        {
            /* Get speed & duplex */
            CHK_FUN_CONTINUE(ret, dv1_reg_read(unit, (((phyIdx << 16u) + 0xA408u) << 1u), &regVal));
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
            CHK_FUN_CONTINUE(ret, dv1_reg_read(unit, (((phyIdx << 16u) + 0xA434u) << 1u), &regVal));
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
 * @param[in] userPort               port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                                  The port id indicators which port to be configured
 * @param[in] delay                 TX delay (0 to 1)
 *                                  To indicate the tx delay is set or not, 0: not to set, 1: to set txdelay
 * @param[out] None
 * @retval  RT_ERR_OK               To execute this function correctly
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_OUT_OF_RANGE     The delay flag is out of range
 * @retval Others                   Please refer to dv1_reg_read() and read_write()
 * @note  This API sets TX delay for a RGMII port.
 *      All TX signals excluding TX clock are delayed for 2ns in delay mode.
 */
RtkApiRet dv1_port_rgmii_txdly_set(uint32 unit, RtkPort userPort, uint8 delay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(1u < delay)
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        retVal = dv1_drv_port_rgmii_delay_cell_set(unit, port, delay);

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
 * @param[in] userPort              port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                                  The port id indicators which port to be configured
 * @param[out] pDelay               Delay pointer
 *                                  Used to store the read dealy value
 * @retval  RT_ERR_OK               To execute this function correctly
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER     The delay parameter pointer is null
 * @retval Others                   Please refer to dv1_reg_read() and read_write()
 * @note This API gets TX delay for a RGMII port.
 *      All TX signals excluding TX clock are delayed for 2ns in delay mode.
 */
RtkApiRet dv1_port_rgmii_txdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == pDelay)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        retVal = dv1_drv_port_rgmii_delay_cell_get(unit, port, pDelay);

        if(RT_ERR_OK != retVal)
        {
            continue;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set RGMII RX delay mode. For VA, the value 1 means 1ns in 1000Mbps and around 1~3ns in 10M/100Mbps depended on PVT. For V2 and V3, the value 1 means 0.1~0.3ns depended on PVT.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[in] speed             Link speed
 *                              The link speed for this port
 * @param[in] delay             RX delay (0~3 for VA, 0~7 for others)
 *                              Used to configure the RGMII parameter for this port
 * @param[out] None
 * @retval RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_PHY_SPEED    The wrong speed which is not support by RGMII
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API sets RX delay for a RGMII port.
 *      All RX signals excluding RX clock are delayed inside in delay mode.
 */
RtkApiRet dv1_port_rgmii_rxdly_set(uint32 unit, RtkPort userPort, RtkPortSpeed_e speed, uint8 delay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

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
        retVal = dv1_drv_port_rgmii_addll_set(unit, port, speed, delay);

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get RGMII RX delay mode. The value 1 is around 1ns (for VA) and 0.3ns (for others) depended on PVT.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[out] delay            RX delay
 *                              The RX delay value pointer which used to store the read back value.
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER The RX delay pointer is null
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API gets RX delay for a RGMII port.
 *      All RX signals excluding RX clock are delayed inside in delay mode.
 */
RtkApiRet dv1_port_rgmii_rxdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if(NULL == pDelay)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        retVal = dv1_drv_port_rgmii_addll_get(unit, port, pDelay);

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
 * @defgroup INTF_XMII_CFG_ASIC Low Level Driver
 * @brief Configure the XMII interface Low Level Driver
 * @{
 */

/**
 * @brief Set xMII port force linking configuration
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is read back.
 * @param[in] pMacAbility      Port link ability
 *                              The pointer of port ability configuration
 * @param[out] None
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID       The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER  The port ability pointer is null
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API can set port MAC force mode properties.
 */
RtkApiRet dv1_port_xmii_link_status_set(uint32 unit, RtkPort userPort, PortMacAbility_t *pMacAbility)
{
    uint32    regVal            = 0u;
    RtkApiRet ret               = RT_ERR_OK;
    uint32    sysPort2SMIAddr[] = {DV1_PORT0_PROPERTY_CONFIGURE, DV1_PORT1_PROPERTY_CONFIGURE, DV1_PORT2_PROPERTY_CONFIGURE, DV1_PORT3_PROPERTY_CONFIGURE,
                                   DV1_PORT4_PROPERTY_CONFIGURE, DV1_PORT5_PROPERTY_CONFIGURE, DV1_PORT6_PROPERTY_CONFIGURE, DV1_PORT7_PROPERTY_CONFIGURE,
                                   DV1_PORT8_PROPERTY_CONFIGURE, DV1_PORT9_PROPERTY_CONFIGURE, DV1_PORT10_PROPERTY_CONFIGURE, DV1_PORT11_PROPERTY_CONFIGURE,
                                   DV1_PORT12_PROPERTY_CONFIGURE, DV1_PORT13_PROPERTY_CONFIGURE, DV1_PORT14_PROPERTY_CONFIGURE, DV1_PORT15_PROPERTY_CONFIGURE};
    RtkPort   port              = 0u;
    do
    {
        /* check the validation of portID */
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if(NULL == pMacAbility)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, dv1_drv_indirect_reg_field_get(unit, sysPort2SMIAddr[port], 0, 32u, &regVal));

        regVal &= (~0x3FFU);

        /* set port portability to MAC address */
        switch(pMacAbility->speed)
        {
            case(uint32)EM_PORT_SPEED_1000M:
                regVal |= ((uint32)0x1 << 7u);
                break;
            case(uint32)EM_PORT_SPEED_100M:
                regVal |= ((uint32)0x1 << 6u);
                break;
            case(uint32)EM_PORT_SPEED_10M:
                regVal |= ((uint32)0x1 << 4u);
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        }
        if(RT_ERR_OK != ret)
        {
            continue;
        }

        CHK_FUN_CONTINUE(ret, dv1_drv_port_covert_port_ability_to_reg(&regVal, pMacAbility));

        CHK_FUN_CONTINUE(ret, dv1_drv_indirect_reg_field_set(unit, sysPort2SMIAddr[port], 0, 32u, regVal));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set xMII TX driving strength which includes rising and falling. The bigger value means higher driving strength.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
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
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API sets pad driving strengthes for a RGMII port.
 */
RtkApiRet dv1_port_xmii_driving_set(uint32 unit, RtkPort userPort, uint8 base, uint8 rise, uint8 fall)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    result0;
    uint32    result1;
    RtkPort   port;
    uint32    regIdx;
    uint32    regValue[4] = {0u, 0u, 0u, 0u};
    uint32    i           = 0u;

    const uint32 drvRegAddr[2]         = {DV1_PAD_MUX_XMII_0_GBIAS, DV1_PAD_MUX_XMII_1_GBIAS};
    const uint32 drivRegFieldLsp[2][4] = {{DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LSP, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LSP, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LSP, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LSP},
                                          {DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LSP, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LSP, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LSP, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LSP}};

    const uint32 drivRegFieldLen[2][4] = {{DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LEN, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LEN, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LEN, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LEN},
                                          {DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LEN, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LEN, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LEN, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LEN}};

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((3u < base) || (7u < rise) || (7u < fall))
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }

        result0 = (base > 1u) ? 1u : 0u;
        result1 = ((base % 2u) == 1u) ? 1u : 0u;
        switch(port)
        {
            case INTF_HW_PORT3:
            case INTF_HW_PORT7:
                regIdx = 0u;
                break;
            case INTF_HW_PORT8:
                regIdx = 1u;
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
            CHK_FUN_BREAK(retVal, dv1_reg_field_write(unit, drvRegAddr[regIdx], drivRegFieldLsp[regIdx][i], drivRegFieldLen[regIdx][i], regValue[i]));
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get xMII TX driving strength which includes rising and falling. The bigger value means higher driving strength.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort              port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
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
 * @retval Others                   Please refer to dv1_reg_read() and read_write()
 * @note This API gets pad driving strengthes for a RGMII port.
 */
RtkApiRet dv1_port_xmii_driving_get(uint32 unit, RtkPort userPort, uint8 *pBase, uint8 *pRise, uint8 *pFall)
{
    uint32       m2;
    uint32       m1;
    uint32       dp;
    uint32       dn;
    RtkApiRet    retVal = RT_ERR_OK;
    RtkPort      port;
    uint32       regVal;
    const uint32 drvRegAddr[2]         = {DV1_PAD_MUX_XMII_0_GBIAS, DV1_PAD_MUX_XMII_1_GBIAS};
    const uint32 drivRegFieldLsp[2][4] = {{DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LSP, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LSP, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LSP, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LSP},
                                          {DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LSP, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LSP, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LSP, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LSP}};

    const uint32 drivRegFieldLen[2][4] = {{DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LEN, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LEN, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LEN, DV1_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LEN},
                                          {DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LEN, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LEN, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LEN, DV1_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LEN}};

    uint32 regIdx;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((NULL == pBase) || (NULL == pRise) || (NULL == pFall))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port))
        {
            regIdx = 0u;
        }
        else if((uint32)INTF_HW_PORT8 == port)
        {
            regIdx = 1u;
        }
        else
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dv1_reg_read(unit, drvRegAddr[regIdx], &regVal));

        m2 = (regVal >> drivRegFieldLsp[regIdx][0]) & ((0x1u << drivRegFieldLen[regIdx][0]) - 1u);
        m1 = (regVal >> drivRegFieldLsp[regIdx][1]) & ((0x1u << drivRegFieldLen[regIdx][1]) - 1u);
        dp = (regVal >> drivRegFieldLsp[regIdx][2]) & ((0x1u << drivRegFieldLen[regIdx][2]) - 1u);
        dn = (regVal >> drivRegFieldLsp[regIdx][3]) & ((0x1u << drivRegFieldLen[regIdx][3]) - 1u);

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
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is configured.
 * @param[in] isRxNegSample     RX sampling edge
 *                              Set RX sampling edge, false = Negative sampling, true = Postive sampling.
 * @param[in] isTxNegSample     TX sampling edge
 *                              Set TX sampling edge, false = Negative sampling, true = Postive sampling.
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API set the sampling edge for xmii RX and TX direction.
 */
RtkApiRet dv1_port_xmii_latch_edge_set(uint32 unit, RtkPort userPort, bool isRxNegSample, bool isTxNegSample)
{
    RtkApiRet retVal         = RT_ERR_OK;
    RtkPort   port           = 0u;
    uint32    edgeLatchField = 0u;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
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

        CHK_FUN_BREAK(retVal, dv1_reg_field_write(unit, DV1_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LEN, edgeLatchField));

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get xMII RX and TX single sampling edge, user could set as positive latch or negative latch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is configured.
 * @param[out] isRxNegSample    RX sampling edge
 *                              Get RX sampling edge, false = Negative sampling, true = Postive sampling.
 * @param[out] isTxNegSample    TX sampling edge
 *                              Get TX sampling edge, false = Negative sampling, true = Postive sampling.
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API get the current sampling edge for xmii RX and TX direction.
 */
RtkApiRet dv1_port_xmii_latch_edge_get(uint32 unit, RtkPort userPort, bool *pIsRxNegSample, bool *pIsTxNegSample)
{
    RtkApiRet retVal         = RT_ERR_OK;
    RtkPort   port           = 0u;
    uint32    edgeLatchField = 0u;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_BREAK(retVal, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LEN, &edgeLatchField));

        if(0u != (edgeLatchField & 0x1u))
        {
            *pIsRxNegSample = true;
        }
        else
        {
            *pIsRxNegSample = false;
        }

        if(0u != (edgeLatchField & 0x2u))
        {
            *pIsTxNegSample = true;
        }
        else
        {
            *pIsTxNegSample = false;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the signal to spread over the widest range of frequencies.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicates which port mode is configured.
 * @param[in] isEnable         Spread signal controller enable status. \n
 *                              Gets spread signal controller state, 0 = enabled, 1 = disabled.
 * @param[in] sequenceModeOffset     Phase offset in sequence mode. \n
 *                              Gets phase offset in sequence mode(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[in] phaseOffset      SSC controller output phase offset. \n
 *                              Gets SSC controller output phase offset(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dv1_drv_indirect_reg_field_get()
 * @note This API set the signal to spread over the widest range of frequencies.
 */
RtkApiRet dv1_port_xmii_ssc_set(uint32 unit, RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        /* error check */
        CHK_FUN_CONTINUE(retVal, dv1_drv_port_xmii_ssc_err_chk(userPort, isEnable, sequenceModeOffset, phaseOffset));

        port = dv1_g_userPort2sysPort[userPort];

        if(port == INTF_HW_PORT8)
        {
            /* xmii 1 */
            /* enable SSC controller */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_SSC_0_CONTROL4, DV1_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LSP, DV1_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LEN, isEnable));
            /*The maximum phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_SSC_0_CONTROL2, DV1_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LSP, DV1_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LEN, sequenceModeOffset));
            /* Allow the SSC controller to output the maximum phase offset */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_SSC_0_CONTROL4, DV1_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LSP, DV1_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LEN, phaseOffset));
        }
        else
        {
            /* xmii 0 */
            /* enable SSC controller */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_SSC_1_CONTROL4, DV1_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LSP, DV1_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LEN, isEnable));
            /*The maximum phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_SSC_1_CONTROL2, DV1_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LSP, DV1_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LEN, sequenceModeOffset));
            /* Allow the SSC controller to output the maximum phase offset */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_SSC_1_CONTROL4, DV1_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LSP, DV1_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LEN, phaseOffset));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the signal to spread over the widest range of frequencies.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
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
 * @retval Others               Please refer to dv1_drv_indirect_reg_field_get()
 * @note This API get the signal to spread over the widest range of frequencies.
 */
RtkApiRet dv1_port_xmii_ssc_get(uint32 unit, RtkPort userPort, RtkEnable *pIsEnable, uint32 *pSequenceModeOffset, uint32 *pPhaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(port == INTF_HW_PORT8)
        {
            /* xmii 1 */
            /* Get SSC controller */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_SSC_0_CONTROL4, DV1_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LSP, DV1_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LEN, pIsEnable));
            /* Get the phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_SSC_0_CONTROL2, DV1_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LSP, DV1_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LEN, pSequenceModeOffset));
            /* Get the SSC controller output phase offset */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_SSC_0_CONTROL4, DV1_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LSP, DV1_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LEN, pPhaseOffset));
        }
        else
        {
            /* xmii 0 */
            /* Get SSC controller */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_SSC_1_CONTROL4, DV1_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LSP, DV1_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LEN, pIsEnable));
            /* Get the phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_SSC_1_CONTROL2, DV1_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LSP, DV1_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LEN, pSequenceModeOffset));
            /* Get the SSC controller output phase offset */
            CHK_FUN_CONTINUE(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_SSC_1_CONTROL4, DV1_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LSP, DV1_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LEN, pPhaseOffset));
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* INTF_XMII_CFG_ASIC */
/**@}*/ /* INTF_XMII_CFG */

/**
 * @addtogroup INTF_RMII_CFG RGMII Link Configuration
 * @{
 */

/**
 * @defgroup INTF_RMII_CFG_ASIC Low Level Driver
 * @brief Configure the RGMII interface Low Level Driver
 * @{
 */

/**
 * @brief Set RMII PHY mode and internal mode refer clock selection, user could set as phy mode and internal mode clock phase
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is configured.
 * @param[in] phyModePhase      PHY mode reference clock selection
 *                              Set PHY mode reference clock, 0 = original phase , 1 = Invert clock phase of output reference clock
 * @param[in] internalPhase     Internal  reference clock selection
 *                              Set TX sampling edge, 0 = Negative sampling, 1 = Invert clock phase for internal MAC
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API set the sampling edge for xmii RX and TX direction.
 */
RtkApiRet dv1_port_rmii_refclk_sel_set(uint32 unit, RtkPort userPort, uint32 phyModePhase, uint32 internalPhase)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(1u == phyModePhase)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LSP, DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LEN, 1u));
        }
        else if(0u == phyModePhase)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LSP, DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LEN, 0u));
        }
        else
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        if(1u == internalPhase)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LSP, DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LEN, 1u));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LSP, DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LEN, 0u));
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get RMII PHY mode and internal mode refer clock selection, user could get as phy mode and internal mode clock phase latch or negative latch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                              The port id indicators which port mode is configured.
 * @param[out] phyModePhase      PHY mode reference clock selection
 *                              Set PHY mode reference clock, 0 = original phase , 1 = Invert clock phase of output reference clock
 * @param[out] internalPhase     Internal reference clock selection
 *                              Set TX sampling edge, 0 = Negative sampling, 1 = Invert clock phase for internal MAC
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dv1_reg_read() and read_write()
 * @note This API set the sampling edge for xmii RX and TX direction.
 */
RtkApiRet dv1_port_rmii_refclk_sel_get(uint32 unit, RtkPort userPort, uint32 *pPhyModePhase, uint32 *pInternalPhase)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LSP, DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LEN, pPhyModePhase));
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LSP, DV1_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LEN, pInternalPhase));

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* INTF_RMII_CFG_ASIC */
/**@}*/ /* INTF_RMII_CFG */

/**@}*/ /* Interface */
