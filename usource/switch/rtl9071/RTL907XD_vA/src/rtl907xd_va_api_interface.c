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
#include "rtl907xd_va_asicdrv_port.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_asicdrv_serdes.h"
#include "rtl907xd_va_api_phy.h"

#define PHY_POWER_ON  (0u)
#define PHY_POWER_OFF (1u)

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
 * @brief Configures the addressed T1 port mode type to 100Base-T1 or 1000Base-T1.
 * @param[in] unit              Stacking unit (1 to 7) \n \n
 *            The stacking unit ID used to indicate which switch to access.
 * @param[in] userPort          Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_6TH) \n
 *                              The designated port ID to configure the T1 interface mode.
 * @param[in] mode              The port mode (EM_MODE_EXT_100BASE_T1_PHY, EM_MODE_EXT_RTPG_T1_PHY) \n
 *                              Configures the addressed user port to EM_MODE_EXT_100BASE_T1_PHY or EM_MODE_EXT_RTPG_T1_PHY.
 * @param[out] None
 * @retval RT_ERR_OK            Successfully configured the port mode.
 * @retval -RT_ERR_PORT_ID      The input port ID is invalid.
 * @retval -RT_ERR_PORT_MODE    The input port mode is invalid.
 * @retval Others               Refer to dva_drv_indirect_reg_field_set() and dva_reg_write() for other possible return values.
 */
RtkApiRet dva_port_t1_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e mode)
{
    RtkApiRet ret  = RT_ERR_OK;
    RtkPort   port = 0u;
    do
    {
        /* check the validation of portID */
        if(0u == DVA_IS_VALID_T1PHY_PORT_ID(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* translate the user port index to the MAC port index */
        port = dva_g_userPort2sysPort[userPort];
        if(EM_MODE_EXT_100BASE_T1_PHY == mode)
        {
            /* if port mode is 100BASE-T1, set the port control config */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, (uint32)COMBO_INTF_T1PHY_MII));
            /* set the PHY reg0 to 100M full duplex */
            CHK_FUN_CONTINUE(ret, dva_reg_write(unit, (((port << 16u) | 0xA400u) << 1u), 0x2100u));
        }
        else if(EM_MODE_EXT_RTPG_T1_PHY == mode)
        {
            /* if port mode is 1000BASE-T1 */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, (uint32)COMBO_INTF_T1PHY_GMII));
            /* set the PHY reg0 to 1000M full duplex */
            CHK_FUN_CONTINUE(ret, dva_reg_write(unit, (((port << 16u) | 0xA400u) << 1u), 0x0140u));
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

/**
 * @brief Configure the T1 interface for a specified port in flashless over ethernet mode.
 * @param[in] unit          Stacking unit (1 to 7)
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort      Port ID (within range DVA_PHY_T1_USER_PORT_MIN to DVA_PHY_T1_USER_PORT_MAX)
 *                          The port ID indicates which port is being configured.
 * @param[in] portMode      Port mode to be set (should be T1)
 *                          Specifies the T1 mode for the port.
 * @param[in] isMaster      PHY role (master or slave)
 *                          Specifies whether the PHY should operate as master or slave.
 * @param[in] speed         Speed setting for the port
 *                          Specifies the speed at which the port should operate.
 * @retval  RT_ERR_OK       To execute this function correctly
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval Others           Please refer to dva_drv_t1_err_chk()
 * @note This API configures the T1 interface for the specified port and mode.
 */
RtkApiRet dva_port_t1_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 isMaster, uint32 speed)
{
    /* Initialize return value to indicate no error */
    RtkApiRet ret = RT_ERR_OK;
    /* Initialize port map to zero */
    RtkPmap portMap = 0u;
    /* Initialize boot status value */
    uint32 bootupStatus = 0u;

    do
    {
        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* if boot status is equal to flashless ethernet mode */
        if(DVA_SYS_BOOT_UP_FLASHLESS_ETH == bootupStatus)
        {

            /* Error handler for portMode check, which portMode should be t1 */
            CHK_FUN_CONTINUE(ret, dva_drv_t1_err_chk(userPort, portMode));

            /* Set the portMap for the specified userPort */
            portMap = (RtkPmap)ENABLE << userPort;

            /* Disable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, DISABLE));

            /* Set the combo interface for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_combo_intf_set(unit, userPort, portMode));

            /* Disable state change latch */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, DISABLE));

            /* Enable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, ENABLE));

            /* Power down PHY */
            CHK_FUN_CONTINUE(ret, dva_phy_bmcr_power_down_set(unit, portMap, ENABLE));

            /* Set speed */
            CHK_FUN_CONTINUE(ret, dva_phy_bmcr_speed_set(unit, userPort, speed));

            /* Set PHY role */
            CHK_FUN_CONTINUE(ret, dva_phy_phycr_role_set(unit, userPort, isMaster));

            /* Power up PHY */
            CHK_FUN_CONTINUE(ret, dva_phy_bmcr_power_down_set(unit, portMap, DISABLE));

            /* Enable state change latch */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, ENABLE));
        }
        else
        {
            /* return error code */
            ret = -RT_ERR_ETH_FLASH_STATE;
        }
    } while(0u == 1u);

    /* Return the result of the configuration */
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
 * @brief Get the port mode type of the addressed port.
 *
 * This function retrieves the mode type of a specified port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7).
 *                 The stacking unit ID used to indicate which switch to access.
 * @param[in] userPort Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END).
 *                     The designated port ID to get the interface mode.
 * @param[out] mode Pointer to the port interface mode.
 *                  This parameter will be set to the current mode of the specified port.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in dva_reg_field_read() and port_mode_get_fptr().
 */
RtkApiRet dva_port_mode_get(uint32 unit, RtkPort userPort, RtkModeExt_e *mode)
{
    RtkApiRet ret  = RT_ERR_OK;
    RtkPort   port = 0u;
    uint32    ifType;
    /* The port mode type callback */
    RtkApiRet (*port_mode_get_fptr[15])(uint32 unit, RtkModeExt_e *mode);
    port_mode_get_fptr[1]  = &dva_drv_port_sub_port01_mode_get;
    port_mode_get_fptr[2]  = &dva_drv_port_sub_port02_mode_get;
    port_mode_get_fptr[3]  = &dva_drv_port_sub_port03_mode_get;
    port_mode_get_fptr[4]  = &dva_drv_port_sub_port04_mode_get;
    port_mode_get_fptr[5]  = &dva_drv_port_sub_port05_mode_get;
    port_mode_get_fptr[6]  = &dva_drv_port_sub_port06_mode_get;
    port_mode_get_fptr[7]  = &dva_drv_port_sub_port07_mode_get;
    port_mode_get_fptr[8]  = &dva_drv_port_sub_port08_mode_get;
    port_mode_get_fptr[9]  = &dva_drv_port_sub_port09_mode_get;
    port_mode_get_fptr[10] = &dva_drv_port_sub_port10_mode_get;
    port_mode_get_fptr[11] = &dva_drv_port_sub_port11_mode_get;
    port_mode_get_fptr[12] = &dva_drv_port_sub_port12_mode_get;

    do
    {
        /* check the validation of portID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* translate the user port index to the MAC port index */
        port = dva_g_userPort2sysPort[userPort];
        /* check the port configuration is disabled or note */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));
        /*  */
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
 * @brief Get port MAC linking status.
 *
 * This function retrieves the current link status of a specified port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7).
 *                 The stacking unit ID used to indicate which switch to access.
 * @param[in] userPort Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END).
 *                     The designated port ID to get the link status.
 * @param[out] pStatus Pointer to the port link status structure.
 *                     This parameter will be set to the current link status of the specified port.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval -RT_ERR_NULL_POINTER if the link status pointer is null.
 * @retval Other error codes as defined in dva_drv_indirect_reg_field_get().
 *
 * @note This API retrieves the port status, including link, duplex, speed, nway, txpause, rxpause, and forcemode.
 */
RtkApiRet dva_port_link_status_get(uint32 unit, RtkPort userPort, PortLinkStatus_t *pStatus)
{
    uint32    regVal                                     = 0u;
    RtkApiRet ret                                        = RT_ERR_OK;
    RtkPort   port                                       = 0u;
    uint32    portSpeed                                  = 0u;
    uint32    sysPort2SMIAddr[DVA_RTL9075_INTERFACE_NUM] = {DVA_PORT0_LINK_STATUS, DVA_PORT1_LINK_STATUS, DVA_PORT2_LINK_STATUS, DVA_PORT3_LINK_STATUS,
                                                            DVA_PORT4_LINK_STATUS, DVA_PORT5_LINK_STATUS, DVA_PORT6_LINK_STATUS, DVA_PORT7_LINK_STATUS,
                                                            DVA_PORT8_LINK_STATUS, DVA_PORT9_LINK_STATUS, DVA_PORT10_LINK_STATUS, DVA_PORT11_LINK_STATUS,
                                                            DVA_PORT12_LINK_STATUS, DVA_PORT13_LINK_STATUS, DVA_PORT14_LINK_STATUS, DVA_PORT15_LINK_STATUS};

    do
    {
        /* check the validation of port index */
        if(0u == DVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
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
        port = dva_g_userPort2sysPort[userPort];
        /* get the MAC port link status */
        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, sysPort2SMIAddr[port], &regVal));

        /* get the MAC port link up/down status */
        pStatus->link = ((0u != (regVal & 0x1u)) ? (uint8)PORT_LINKUP : (uint8)PORT_LINKDOWN);

        /* if the port mode type is serdes, get the speed from PHY PCS */
        if(RT_ERR_OK == dva_drv_sds_is_serdes_port(unit, userPort))
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_port_speed_get(unit, port, &portSpeed));
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
            /* get the MAC port duplex mode  */
            pStatus->duplex = (uint8)((0u != (regVal & ((uint32)0x1 << 3u))) ? (uint8)PORT_FULL_DUPLEX : (uint8)PORT_HALF_DUPLEX);
        }
        /* get the MAC port tx pause status */
        pStatus->txpause = (uint8)((0u != (regVal & ((uint32)0x1 << 4u))) ? 1u : 0u);
        /* get the MAC port rx pause status */
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
 * @param[in] userPort Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END). \n
 *                     The addressed port ID to be configured.
 * @param[out] None
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in dva_drv_indirect_reg_field_set().
 *
 * @note Jumbo frame support up to 10k bytes (including VLAN tag).
 */
RtkApiRet dva_port_pkt_maxlen_set(uint32 unit, RtkEnable enable, RtkPort userPort)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regValue;
    RtkPort   port;

    do
    {
        /* check port id is valid or not */
        if(0u == DVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* get MAC port id */
        port = dva_g_userPort2sysPort[userPort];
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
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_MAXLEN_CONTROL + (PERPORT_REG_OFFSET * (uint32)port), DVA_PORT0_MAXLEN_CONTROL_MAXLEN_LSP, DVA_PORT0_MAXLEN_CONTROL_MAXLEN_LEN, regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get packet maximum length configuration of the specific port.
 *
 * This function retrieves the maximum packet length configuration for a specified port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7).
 *                 The stacking unit ID used to indicate which switch to access.
 * @param[in] userPort Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END).
 *                     The port ID indicates which port mode is read back.
 * @param[out] maxLen  Register value (0x05F2 to 0x2800) \n
 *                     This return value is the max packet length which is configured for this port.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in dva_drv_indirect_reg_field_get().
 *
 * @note Jumbo frame support up to 10k bytes (including VLAN tag).
 */
RtkApiRet dva_port_pkt_maxlen_get(uint32 unit, RtkPort userPort, uint32 *maxLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port;

    do
    {
        /* check port id is valid or not */
        if(0u == DVA_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* get MAC port id */
        port = dva_g_userPort2sysPort[userPort];
        /* get enable or disable maximum packet length configuration */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_MAXLEN_CONTROL + (PERPORT_REG_OFFSET * (uint32)port), DVA_PORT0_MAXLEN_CONTROL_MAXLEN_LSP, DVA_PORT0_MAXLEN_CONTROL_MAXLEN_LEN, maxLen));
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
 *
 * This function sets the Ethernet link ability for a specified port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7).
 *                 The stacking unit ID used to indicate which switch to access.
 * @param[in] userPort Port ID (DVA_RTK_USER_PORT_7TH).
 *                     The port ID indicates which port mode is to be configured.
 * @param[in] pEthAbility Pointer to the Ethernet link ability structure.
 *                        This parameter includes link speed, duplex, and link status to be set.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval -RT_ERR_PORT_MODE if the configuration does not match for Ethernet.
 * @retval Other error codes as defined in dva_port_mode_get(), dva_drv_port_eth_an_mode_set(), and dva_drv_port_eth_force_mode_set().
 */
RtkApiRet dva_port_eth_link_ability_set(uint32 unit, RtkPort userPort, const PortEthLinkAbility_t *pEthAbility)
{
    RtkModeExt_e mode;
    RtkApiRet    ret = SUCCESS;
    RtkPort      port;

    do
    {
        /* check input port index is valid or not */
        port = dva_g_userPort2sysPort[userPort];
        if(INTF_HW_PORT7 != port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* get the port mode type of the addressed port */
        CHK_FUN_CONTINUE(ret, dva_port_mode_get(unit, userPort, &mode));
        /* check the port mode of addressed port is Ethernet or not */
        if((mode != EM_MODE_EXT_FAST_ETHERNET) && (mode != EM_MODE_EXT_GIGA_ETHERNET))
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

        /* Configure port ability */
        if(1u == pEthAbility->autoNego)
        {
            /* configure auto-nego link ability */
            CHK_FUN_CONTINUE(ret, dva_drv_port_eth_an_mode_set(unit, pEthAbility));
        }
        else
        {
            /* configure force mode link ability */
            CHK_FUN_CONTINUE(ret, dva_drv_port_eth_force_mode_set(unit, pEthAbility));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get ETH linking status.
 *
 * This function retrieves the Ethernet link ability for a specified port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7).
 *                 The stacking unit ID used to indicate which switch to access.
 * @param[in] userPort Port ID (DVA_RTK_USER_PORT_7TH).
 *                     The port ID indicates which port mode is to be configured.
 * @param[out] pStatus Pointer to the Ethernet link status structure.
 *                     This parameter will be set to the current link status of the specified port.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_MODE if the interface is not configured as ETH mode.
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval Other error codes as defined in dva_port_mode_get(), dva_reg_field_read(), and dva_reg_read().
 */
RtkApiRet dva_port_eth_link_ability_get(uint32 unit, RtkPort userPort, PortRtkEthLinkStatus_t *pStatus)
{
    uint32       phyIdx    = 0xBu;
    uint32       regVal    = 0u;
    uint16       anAbility = 0u;
    RtkModeExt_e mode;
    RtkApiRet    ret = SUCCESS;
    RtkPort      port;

    do
    {
        /* get MAC port index and check the port index is valid or not */
        port = dva_g_userPort2sysPort[userPort];
        if(INTF_HW_PORT7 != port)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* get the port mode type of the addressed port */
        CHK_FUN_CONTINUE(ret, dva_port_mode_get(unit, userPort, &mode));
        /* check the port mode of addressed port is Ethernet or not */
        if((mode != EM_MODE_EXT_FAST_ETHERNET) && (mode != EM_MODE_EXT_GIGA_ETHERNET))
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

        /* get reg 0 bit12: auto_nego */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, (((phyIdx << 16u) + DVA_OCP_BMCR) << 1u), 12u, 1u, &regVal));
        pStatus->autoNego = (uint8)(regVal & (uint8)0xFFu);
        /* if auto negotiation is enabled */
        if(1u == pStatus->autoNego)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_port_eth_get_an_ability(unit, mode, userPort, &anAbility));
            pStatus->autoNegoCapability = anAbility;
        }
        else
        {
            /* if auto negotiation is disabled. force mode. */
            /* Get speed & duplex
             * speed [5:4].
             *      2'b00(0x0): 10M   (return EM_PORT_SPEED_10M = 1)
             *      2'b01(0x1): 100M  (return EM_PORT_SPEED_100M = 2)
             *      2'b10(0x2): 1000M (return EM_PORT_SPEED_1000M = 4)
             * duplex [3:3]
             *      1: PORT_FULL_DUPLEX = 2
             *      0: PORT_HALF_DUPLEX = 1
             */
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, (((phyIdx << 16u) + 0xA434u) << 1u), 3u, 3u, &regVal));
            /* duplex [3:3] */
            if(0u != (regVal & 0x1u))
            {
                /* Full duplex */
                pStatus->duplex = (uint8)PORT_FULL_DUPLEX;
            }
            else
            {
                /* Half duplex */
                pStatus->duplex = (uint8)PORT_HALF_DUPLEX;
            }
            /* speed [5:4] */
            pStatus->speed = (1u << (regVal >> 1u));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Configure the FE/GPHY interface for a specified port in flashless over ethernet mode.
 * @param[in] unit          Stacking unit (1 to 7)
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort      Port ID (should be DVA_RTK_USER_PORT_7TH)
 *                          The port ID indicates which port is being configured.
 * @param[in] portMode      Port mode to be set (FE or Giga Ethernet)
 *                          Specifies the mode for the port.
 * @param[in] anEnable      Auto-negotiation enable/disable
 *                          Specifies whether auto-negotiation should be enabled or disabled.
 * @param[in] speed         Speed setting for the port
 *                          Specifies the speed at which the port should operate.
 * @retval  RT_ERR_OK       To execute this function correctly
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval -RT_ERR_PORT_MODE The port mode is invalid
 * @note This API configures the FE/GPHY interface for the specified port and mode.
 */
RtkApiRet dva_port_fe_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, RtkEnable anEnable, uint32 speed)
{
    /* Initialize return value to indicate no error */
    RtkApiRet ret = RT_ERR_OK;
    /* Initialize boot status value */
    uint32 bootupStatus = 0u;

    do
    {
        /* Error handler for port check, which port number should be fe/gphy */
        CHK_FUN_CONTINUE(ret, dva_drv_fe_err_chk(userPort, portMode));

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* if boot status is equal to flashless ethernet mode */
        if(DVA_SYS_BOOT_UP_FLASHLESS_ETH == bootupStatus)
        {
            /* Disable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, DISABLE));

            /* Set the combo interface for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_combo_intf_set(unit, userPort, portMode));

            /* Disable state change latch */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, DISABLE));

            /* Enable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, ENABLE));

            /* Power down Ethernet PHY */
            CHK_FUN_CONTINUE(ret, dva_ethphy_bmcr_power_down_set(unit, PHY_POWER_OFF));

            /* Set speed for Ethernet PHY */
            CHK_FUN_CONTINUE(ret, dva_ethphy_bmcr_speed_set(unit, speed));

            /* Set auto-negotiation mode for Ethernet PHY */
            CHK_FUN_CONTINUE(ret, dva_ethphy_bmcr_an_mode_set(unit, anEnable));

            /* Power up Ethernet PHY */
            CHK_FUN_CONTINUE(ret, dva_ethphy_bmcr_power_down_set(unit, PHY_POWER_ON));

            /* Enable state change latch */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, ENABLE));
        }
        else
        {
            /* return error code */
            ret = -RT_ERR_ETH_FLASH_STATE;
        }
    } while(0u == 1u);

    /* Return the result of the configuration */
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
 * @param[in] unit                  Stacking unit (1 to 7) \n \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort              Port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                                  The port id indicates which port is to be configured
 * @param[in] delay                 TX delay (0 to 1)
 *                                  To indicate if the tx delay is set or not, 0: not to set, 1: to set tx delay
 * @param[out] None
 * @retval  RT_ERR_OK               To execute this function correctly
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_OUT_OF_RANGE     The delay flag is out of range
 * @retval Others                   Please refer to dva_reg_read() and read_write()
 * @note  This API sets TX delay for a RGMII port.
 *       All TX signals excluding TX clock are delayed for 2ns in delay mode.
 */
RtkApiRet dva_port_rgmii_txdly_set(uint32 unit, RtkPort userPort, uint8 delay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        /* Check if userPort is a valid port ID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Check if delay value is in valid range */
        if(1u < delay)
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Map userPort to system port */
        port = dva_g_userPort2sysPort[userPort];

        /* Call driver API to set RGMII delay on port */
        retVal = dva_drv_port_rgmii_delay_cell_set(unit, port, delay);

        /* Check return value and retry if failed */
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get RGMII TX delay mode. The value 0 means disable TX delay mode, and 1 means enable TX delay mode.
 * @param[in] unit                  Stacking unit (1 to 7) \n \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort              Port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                                  The port id indicates which port is to be configured
 * @param[out] pDelay               Delay pointer
 *                                  Used to store the read delay value
 * @retval  RT_ERR_OK               To execute this function correctly
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER     The delay parameter pointer is null
 * @retval Others                   Please refer to dva_reg_read() and read_write()
 * @note This API gets TX delay for a RGMII port.
 *      All TX signals excluding TX clock are delayed for 2ns in delay mode.
 */
RtkApiRet dva_port_rgmii_txdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        /* Check if user port number is valid */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Check if delay pointer is null */
        if(NULL == pDelay)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Convert to system port number */
        port = dva_g_userPort2sysPort[userPort];

        /* Get TX delay value */
        retVal = dva_drv_port_rgmii_delay_cell_get(unit, port, pDelay);

        /* Check return value */
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set RGMII RX delay mode. For VA, the value 1 means 1ns in 1000Mbps and around 1~3ns in 10M/100Mbps depended on PVT. For V2 and V3, the value 1 means 0.1~0.3ns depended on PVT.
 * @param[in] unit              Stacking unit (1 to 7) \n \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          Port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                              The port id indicates which port mode is read back.
 * @param[in] speed             Link speed
 *                              The link speed for this port
 * @param[in] delay             RX delay (0~3 for VA, 0~7 for others)
 *                              Used to configure the RGMII parameter for this port
 * @param[out] None
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_PHY_SPEED    The wrong speed which is not supported by RGMII
 * @retval Others               Please refer to dva_reg_read() and read_write()
 * @note This API sets RX delay for a RGMII port.
 *      All RX signals excluding RX clock are delayed inside in delay mode.
 */
RtkApiRet dva_port_rgmii_rxdly_set(uint32 unit, RtkPort userPort, RtkPortSpeed_e speed, uint8 delay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        /* Check if user port number is valid */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Convert to system port number */
        port = dva_g_userPort2sysPort[userPort];

        /* Check if speed value is valid */
        if((EM_PORT_SPEED_10M != speed) && (EM_PORT_SPEED_100M != speed) && (EM_PORT_SPEED_1000M != speed))
        {
            retVal = -(RtkApiRet)RT_ERR_PHY_SPEED;
            continue;
        }

        /* Check delay value range */
        if(3u < delay)
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set RX delay value */
        retVal = dva_drv_port_rgmii_addll_set(unit, port, speed, delay);

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get RGMII RX delay mode. The value 1 is around 1ns (for VA) and 0.3ns (for others) depended on PVT.
 * @param[in] unit              Stacking unit (1 to 7) \n \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          Port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                              The port id indicates which port mode is read back.
 * @param[out] delay            RX delay
 *                              The RX delay value pointer which is used to store the read back value.
 * @retval RT_ERR_OK            To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER The RX delay pointer is null
 * @retval Others               Please refer to dva_reg_read() and read_write()
 * @note This API gets RX delay for a RGMII port.
 *      All RX signals excluding RX clock are delayed inside in delay mode.
 */
RtkApiRet dva_port_rgmii_rxdly_get(uint32 unit, RtkPort userPort, uint8 *pDelay)
{
    RtkApiRet retVal;
    RtkPort   port;

    do
    {
        /* Check if user port number is valid */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Convert to system port number */
        port = dva_g_userPort2sysPort[userPort];

        if(NULL == pDelay)
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get RX delay value */
        retVal = dva_drv_port_rgmii_addll_get(unit, port, pDelay);

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
 * @brief Sets xMII port force linking configuration.
 *
 * This function sets the force linking configuration for a specified xMII port on a given stacking unit.
 *
 * @param[in] unit Stacking unit (1 to 7).
 *                 The stacking unit ID used to indicate which switch to access.
 * @param[in] userPort Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END).
 *                     The designated port ID to set the xMII link status.
 * @param[in] pMacAbility Pointer to the port link ability configuration.
 *                        This parameter specifies the port ability configuration.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_PORT_ID if the port ID is out of range.
 * @retval -RT_ERR_NULL_POINTER if the port ability pointer is null.
 * @retval Other error codes as defined in dva_drv_indirect_reg_field_get(), dva_drv_port_covert_port_ability_to_reg(), and dva_drv_indirect_reg_field_set().
 *
 * @note This API can set port MAC force mode properties.
 */
RtkApiRet dva_port_xmii_link_status_set(uint32 unit, RtkPort userPort, PortMacAbility_t *pMacAbility)
{
    uint32    regVal            = 0u;
    RtkApiRet ret               = RT_ERR_OK;
    uint32    sysPort2SMIAddr[] = {DVA_PORT0_PROPERTY_CONFIGURE, DVA_PORT1_PROPERTY_CONFIGURE, DVA_PORT2_PROPERTY_CONFIGURE, DVA_PORT3_PROPERTY_CONFIGURE,
                                   DVA_PORT4_PROPERTY_CONFIGURE, DVA_PORT5_PROPERTY_CONFIGURE, DVA_PORT6_PROPERTY_CONFIGURE, DVA_PORT7_PROPERTY_CONFIGURE,
                                   DVA_PORT8_PROPERTY_CONFIGURE, DVA_PORT9_PROPERTY_CONFIGURE, DVA_PORT10_PROPERTY_CONFIGURE, DVA_PORT11_PROPERTY_CONFIGURE,
                                   DVA_PORT12_PROPERTY_CONFIGURE, DVA_PORT13_PROPERTY_CONFIGURE, DVA_PORT14_PROPERTY_CONFIGURE, DVA_PORT15_PROPERTY_CONFIGURE};
    RtkPort   port              = 0u;
    do
    {
        /* check the validation of portID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL parameter pointer */
        if(NULL == pMacAbility)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        port = dva_g_userPort2sysPort[userPort];
        /* get MAC port SMI configuration */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, sysPort2SMIAddr[port], 0, 32u, &regVal));
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
        /* get the port SMI reg setting value according to MAC ability struct */
        CHK_FUN_CONTINUE(ret, dva_drv_port_covert_port_ability_to_reg(&regVal, pMacAbility));
        /* set the port SMI configuration */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, sysPort2SMIAddr[port], 0, 32u, regVal));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Sets xMII TX driving strength which includes rising and falling values. The bigger value means higher driving strength.
 * @param[in] unit              Stacking unit (1 to 7) \n \n
 *                              Stacking unit ID is used to indicate which switch to access.
 * @param[in] userPort          Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                              The port ID indicates which port mode is configured.
 * @param[in] base              Driving base
 *                              TX strength base which affects rising and falling
 * @param[in] rise              Driving rise
 *                              TX rising strength
 * @param[in] fall              Driving fall
 *                              TX falling strength
 * @param[out] None
 * @retval  RT_ERR_OK           Function executed correctly.
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval -RT_ERR_OUT_OF_RANGE The base value is out of range
 * @retval Others               Please refer to dva_reg_read() and read_write()
 * @note This API sets pad driving strengths for an RGMII port.
 */
RtkApiRet dva_port_xmii_driving_set(uint32 unit, RtkPort userPort, uint8 base, uint8 rise, uint8 fall)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    result0;
    uint32    result1;
    RtkPort   port;
    uint32    regIdx;
    uint32    regValue[4] = {0u, 0u, 0u, 0u};
    uint32    i           = 0u;

    const uint32 drvRegAddr[2]         = {DVA_PAD_MUX_XMII_0_GBIAS, DVA_PAD_MUX_XMII_1_GBIAS};
    const uint32 drivRegFieldLsp[2][4] = {{DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LSP, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LSP, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LSP, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LSP},
                                          {DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LSP, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LSP, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LSP, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LSP}};

    const uint32 drivRegFieldLen[2][4] = {{DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LEN, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LEN, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LEN, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LEN},
                                          {DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LEN, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LEN, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LEN, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LEN}};

    do
    {
        /* Check if user port number is valid */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            /* Set error return value */
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Convert to system port number */
        port = dva_g_userPort2sysPort[userPort];

        /* Check parameter range */
        if((3u < base) || (7u < rise) || (7u < fall))
        {
            retVal = -(RtkApiRet)RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Calculate register values */
        result0 = (base > 1u) ? 1u : 0u;
        result1 = ((base % 2u) == 1u) ? 1u : 0u;

        /* Determine register index by port */
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

        /* Check if index is valid */
        if(RT_ERR_OK != retVal)
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Set values to write to registers */
        regValue[0] = result0;
        regValue[1] = result1;
        regValue[2] = rise;
        regValue[3] = fall;

        /* Write registers */
        for(i = 0; i < 4u; i++)
        {
            CHK_FUN_BREAK(retVal, dva_reg_field_write(unit, drvRegAddr[regIdx], drivRegFieldLsp[regIdx][i], drivRegFieldLen[regIdx][i], regValue[i]));
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Gets xMII TX driving strength which includes rising and falling values. The bigger value means higher driving strength.
 * @param[in] unit                  Stacking unit (1 to 7) \n \n
 *                                  Stacking unit ID is used to indicate which switch to access.
 * @param[in] userPort              port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                                  The port id indicates which port mode is read back.
 * @param[out] pBase                Base value pointer
 *                                  The pointer of TX strength base
 * @param[out] pRise                Rise value pointer
 *                                  The pointer of TX rising strength
 * @param[out] pFall                Fall value pointer
 *                                  The pointer of TX falling strength
 * @retval RT_ERR_OK                Function executed correctly.
 * @retval -RT_ERR_PORT_ID          The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER     The pointer is null.
 * @retval Others                   Please refer to dva_reg_read() and read_write()
 * @note This API gets pad driving strengths for an RGMII port.
 */
RtkApiRet dva_port_xmii_driving_get(uint32 unit, RtkPort userPort, uint8 *pBase, uint8 *pRise, uint8 *pFall)
{
    uint32       m2;
    uint32       m1;
    uint32       dp;
    uint32       dn;
    RtkApiRet    retVal = RT_ERR_OK;
    RtkPort      port;
    uint32       regVal;
    const uint32 drvRegAddr[2]         = {DVA_PAD_MUX_XMII_0_GBIAS, DVA_PAD_MUX_XMII_1_GBIAS};
    const uint32 drivRegFieldLsp[2][4] = {{DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LSP, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LSP, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LSP, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LSP},
                                          {DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LSP, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LSP, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LSP, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LSP}};

    const uint32 drivRegFieldLen[2][4] = {{DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE2_LEN, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_MODE_LEN, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DP_LEN, DVA_PAD_MUX_XMII_0_GBIAS_RG_XMII_0_GBIAS_DN_LEN},
                                          {DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE2_LEN, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_MODE_LEN, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DP_LEN, DVA_PAD_MUX_XMII_1_GBIAS_RG_XMII_1_GBIAS_DN_LEN}};

    uint32 regIdx;

    do
    {
        /* Check validity of userPort */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Convert userPort to system port number */
        port = dva_g_userPort2sysPort[userPort];

        /* Check input parameters */
        if((NULL == pBase) || (NULL == pRise) || (NULL == pFall))
        {
            retVal = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        /* Determine register index based on port */
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

        /* Read driving register value */
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, drvRegAddr[regIdx], &regVal));

        /* Parse driving fields */
        m2 = (regVal >> drivRegFieldLsp[regIdx][0]) & ((0x1u << drivRegFieldLen[regIdx][0]) - 1u);
        m1 = (regVal >> drivRegFieldLsp[regIdx][1]) & ((0x1u << drivRegFieldLen[regIdx][1]) - 1u);
        dp = (regVal >> drivRegFieldLsp[regIdx][2]) & ((0x1u << drivRegFieldLen[regIdx][2]) - 1u);
        dn = (regVal >> drivRegFieldLsp[regIdx][3]) & ((0x1u << drivRegFieldLen[regIdx][3]) - 1u);

        /* Calculate results */
        *pBase = (uint8)((((m2 & 0xFFu) * 0x2u) + (m1 & 0xFFu)) & 0xFFu);
        *pRise = (uint8)(dp & 0xFFu);
        *pFall = (uint8)(dn & 0xFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Sets xMII RX and TX single sampling edge. User could set as positive latch or negative latch.
 * @param[in] unit              Stacking unit (1 to 7) \n \n
 *                              Stacking unit ID is used to indicate which switch to access.
 * @param[in] userPort          port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                              The port id indicates which port mode is configured.
 * @param[in] isRxNegSample     RX sampling edge
 *                              Sets RX sampling edge, false = Negative sampling, true = Positive sampling.
 * @param[in] isTxNegSample     TX sampling edge
 *                              Sets TX sampling edge, false = Negative sampling, true = Positive sampling.
 * @param[out] None
 * @retval  RT_ERR_OK           Function executed correctly.
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dva_drv_indirect_reg_field_set()
 * @note This API sets the sampling edge for xMII RX and TX direction.
 */
RtkApiRet dva_port_xmii_latch_edge_set(uint32 unit, RtkPort userPort, bool isRxNegSample, bool isTxNegSample)
{
    RtkApiRet retVal         = RT_ERR_OK;
    RtkPort   port           = 0u;
    uint32    edgeLatchField = 0u;

    do
    {
        /* Check validity of userPort */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Convert userPort to system port number */
        port = dva_g_userPort2sysPort[userPort];

        /* Check validity of port */
        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Parse Rx sampling edge configuration */
        if(isRxNegSample)
        {
            edgeLatchField |= 0x1u;
        }

        /* Parse Tx sampling edge configuration */
        if(isTxNegSample)
        {
            edgeLatchField |= 0x2u;
        }

        /* Set sampling edge field */
        CHK_FUN_BREAK(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LEN, edgeLatchField));

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Gets xMII RX and TX single sampling edge. User could set as positive latch or negative latch.
 * @param[in] unit              Stacking unit (1 to 7) \n \n
 *                              Stacking unit ID is used to indicate which switch to access.
 * @param[in] userPort          Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END).
 *                              The port ID indicates which port mode is configured.
 * @param[out] isRxNegSample    RX sampling edge.
 *                              Gets RX sampling edge, false = Negative sampling, true = Positive sampling.
 * @param[out] isTxNegSample    TX sampling edge
 *                              Gets TX sampling edge, false = Negative sampling, true = Positive sampling.
 * @retval  RT_ERR_OK           Function executed correctly.
 * @retval -RT_ERR_PORT_ID      The port ID is out of range.
 * @retval Others               Please refer to dva_drv_indirect_reg_field_get().
 * @note This API gets the current sampling edge for xMII RX and TX directions.
 */
RtkApiRet dva_port_xmii_latch_edge_get(uint32 unit, RtkPort userPort, bool *isRxNegSample, bool *isTxNegSample)
{
    RtkApiRet retVal         = RT_ERR_OK;
    RtkPort   port           = 0u;
    uint32    edgeLatchField = 0u;

    do
    {
        /* Check validity of userPort */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Convert userPort to system port number */
        port = dva_g_userPort2sysPort[userPort];

        /* Check validity of port */
        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Read edgeLatchField value */
        CHK_FUN_BREAK(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_SEL_NEG_LEN, &edgeLatchField));

        /* Parse Rx sampling edge configuration */
        if(0u != (edgeLatchField & 0x1u))
        {
            *isRxNegSample = true;
        }
        else
        {
            *isRxNegSample = false;
        }

        /* Parse Tx sampling edge configuration */
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
 * @param[in] unit                Stacking unit (1 to 7) \n\n
 *                                Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort            Port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                                The port id indicates which port mode is configured.
 * @param[in] isEnable            Spread signal controller enable status. \n
 *                                Gets spread signal controller state, 0 = enabled, 1 = disabled.
 * @param[in] sequenceModeOffset  Phase offset in sequence mode. \n
 *                                Gets phase offset in sequence mode(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[in] phaseOffset         SSC controller output phase offset. \n
 *                                Gets SSC controller output phase offset(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dva_drv_indirect_reg_field_get()
 * @note This API sets the signal to spread over the widest range of frequencies.
 */
RtkApiRet dva_port_xmii_ssc_set(uint32 unit, RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        /* error check */
        CHK_FUN_CONTINUE(retVal, dva_drv_port_xmii_ssc_err_chk(userPort, isEnable, sequenceModeOffset, phaseOffset));

        port = dva_g_userPort2sysPort[userPort];

        if(port == INTF_HW_PORT8)
        {
            /* xmii 1 */
            /* enable SSC controller */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_SSC_0_CONTROL4, DVA_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LSP, DVA_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LEN, isEnable));
            /*The maximum phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_SSC_0_CONTROL2, DVA_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LSP, DVA_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LEN, sequenceModeOffset));
            /* Allow the SSC controller to output the maximum phase offset */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_SSC_0_CONTROL4, DVA_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LSP, DVA_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LEN, phaseOffset));
        }
        else
        {
            /* xmii 0 */
            /* enable SSC controller */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_SSC_1_CONTROL4, DVA_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LSP, DVA_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LEN, isEnable));
            /*The maximum phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_SSC_1_CONTROL2, DVA_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LSP, DVA_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LEN, sequenceModeOffset));
            /* Allow the SSC controller to output the maximum phase offset */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_SSC_1_CONTROL4, DVA_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LSP, DVA_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LEN, phaseOffset));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Gets the signal spread to the widest range of frequencies.
 * @param[in] unit              Stacking unit (1 to 7) \n
 *                              Stacking unit ID indicates which switch to access.
 * @param[in] userPort          port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                              Port ID indicates which port's mode is configured.
 * @param[out] isEnable         Spread spectrum controller enable status.
 *                              Gets spread spectrum controller state, 0 = enabled, 1 = disabled.
 * @param[out] sequenceMode     Phase offset in sequence mode
 *                              Gets phase offset in sequence mode, 0x3 = default offset, 0x7 = maximum phase offset
 * @param[out] phaseOffset      SSC controller output phase offset
 *                              Gets SSC controller output phase offset, 0x3 = default offset, 0x7 = maximum phase offset
 * @param[out] None
 * @retval  RT_ERR_OK           Function executed correctly
 * @retval -RT_ERR_PORT_ID      Port ID is out of range
 * @retval Others               Please refer to dva_drv_indirect_reg_field_get()
 * @note Gets signal spread to the widest range of frequencies.
 */
RtkApiRet dva_port_xmii_ssc_get(uint32 unit, RtkPort userPort, RtkEnable *isEnable, uint32 *sequenceModeOffset, uint32 *phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        /* Check for valid user port ID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Map user port to system port */
        port = dva_g_userPort2sysPort[userPort];

        /* Check supported ports */
        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(port == INTF_HW_PORT8)
        {
            /* xmii 1 */
            /* Get SSC controller */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_SSC_0_CONTROL4, DVA_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LSP, DVA_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LEN, isEnable));
            /* Get the phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_SSC_0_CONTROL2, DVA_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LSP, DVA_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LEN, sequenceModeOffset));
            /* Get the SSC controller output phase offset */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_SSC_0_CONTROL4, DVA_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LSP, DVA_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LEN, phaseOffset));
        }
        else
        {
            /* xmii 0 */
            /* Get SSC controller */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_SSC_1_CONTROL4, DVA_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LSP, DVA_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LEN, isEnable));
            /* Get the phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_SSC_1_CONTROL2, DVA_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LSP, DVA_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LEN, sequenceModeOffset));
            /* Get the SSC controller output phase offset */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_SSC_1_CONTROL4, DVA_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LSP, DVA_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LEN, phaseOffset));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Configure the xMII interface for a specified port in flashless over ethernet mode.
 * @param[in] unit          Stacking unit (1 to 7)
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort      Port ID (DVA_RTK_USER_PORT_3RD, DVA_RTK_USER_PORT_7TH, DVA_RTK_USER_PORT_8TH)
 *                          The port ID indicates which port mode is configured.
 * @param[in] portMode      Port mode to be set (usxgmii, hisgmii, sgmii, 5gkr)
 *                          Specifies the xMII mode for the port.
 * @retval  RT_ERR_OK       To execute this function correctly
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval Others           Please refer to dva_drv_xmii_mode_chk()
 * @note This API configures the xMII interface for the specified port and mode.
 */
RtkApiRet dva_port_xmii_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode)
{
    /* Initialize return value to indicate no error */
    RtkApiRet ret = RT_ERR_OK;
    /* Initialize boot status value */
    uint32 bootupStatus = 0u;

    do
    {
        /* Error handler for port check, which port number should be serdes user port (7, 9, 10, and 11) */
        if((DVA_RTK_USER_PORT_3RD != userPort) && (DVA_RTK_USER_PORT_7TH != userPort) && (DVA_RTK_USER_PORT_8TH != userPort))
        {
            /* Return error if userPort is not a valid serdes user port */
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* if boot status is equal to flashless ethernet mode */
        if(DVA_SYS_BOOT_UP_FLASHLESS_ETH == bootupStatus)
        {
            /* Error handler for portMode check, which portMode should be usxgmii, hisgmii, sgmii, and 5gkr */
            CHK_FUN_CONTINUE(ret, dva_drv_xmii_mode_chk(portMode));

            /* Disable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, DISABLE));

            /* Set the combo interface for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_combo_intf_set(unit, userPort, portMode));

            /* Set the xMII mode for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_xmii_mode_set(unit, userPort, portMode));

            /* Disable state change latch */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, DISABLE));

            /* Enable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, ENABLE));
        }
        else
        {
            /* return error code */
            ret = -RT_ERR_ETH_FLASH_STATE;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the force link configuration for the xMII interface on a specified port in flashless over ethernet mode.
 * @param[in] unit          Stacking unit (1 to 7)
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort      Port ID (DVA_RTK_USER_PORT_3RD, DVA_RTK_USER_PORT_7TH, DVA_RTK_USER_PORT_8TH)
 *                          The port ID indicates which port is being configured.
 * @param[in] portMode      Port mode to be set (EM_MODE_EXT_RMII_PHY, EM_MODE_EXT_RMII_MAC, EM_MODE_EXT_RGMII)
 *                          Specifies the xMII mode for the port.
 * @param[in] speed         Speed setting for the port (e.g., DVA_SPEED_1000MBPS)
 *                          Specifies the speed at which the port should operate.
 * @param[in] linkStatus    Link status to be set (e.g., link up or down)
 *                          Specifies whether the link should be forced up or down.
 * @retval  RT_ERR_OK       To execute this function correctly
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval Others           Please refer to dva_drv_port_force_link_set()
 * @note This API sets the force link configuration for the specified port and mode, including RMII and RGMII specific settings.
 */
RtkApiRet dva_port_xmii_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 speed, uint32 linkStatus)
{
    /* Initialize return value to indicate no error */
    RtkApiRet ret = RT_ERR_OK;
    /* Initialize boot status value */
    uint32 bootupStatus = 0u;

    do
    {
        /* Check if the userPort is a valid serdes user port (7, 9, 10, and 11) */
        if((DVA_RTK_USER_PORT_3RD != userPort) && (DVA_RTK_USER_PORT_7TH != userPort) && (DVA_RTK_USER_PORT_8TH != userPort))
        {
            /* Return error if userPort is not valid */
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* if boot status is equal to flashless ethernet mode */
        if(DVA_SYS_BOOT_UP_FLASHLESS_ETH == bootupStatus)
        {
            /* xmii config set */
            CHK_FUN_CONTINUE(ret, dva_drv_port_xmii_force_link_set(unit, userPort, portMode, speed, linkStatus));
        }
        else
        {
            /* return error code */
            ret = -RT_ERR_ETH_FLASH_STATE;
        }
    } while(0u == 1u);

    return ret;
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
 * @brief Set RMII PHY mode and internal mode reference clock selection, user could set PHY mode and internal mode clock phase
 * @param[in] unit              Stacking unit (1 to 7) \n
 *                              Stacking unit ID indicates which switch to access.
 * @param[in] userPort          port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                              The port id indicates which port mode to configure.
 * @param[in] phyModePhase      PHY mode reference clock selection
 *                              Set PHY mode reference clock, 0 = original phase, 1 = Invert clock phase of output reference clock
 * @param[in] internalPhase     Internal reference clock selection
 *                              Set TX sampling edge, 0 = Negative sampling, 1 = Invert clock phase for internal MAC
 * @param[out] None
 * @retval  RT_ERR_OK           Function executed correctly
 * @retval -RT_ERR_PORT_ID      Port ID is out of range
 * @retval Others               Please refer to dva_drv_indirect_reg_field_set()
 * @note This API sets the RMII PHY mode and internal mode reference clock selection.
 */
RtkApiRet dva_port_rmii_refclk_sel_set(uint32 unit, RtkPort userPort, uint32 phyModePhase, uint32 internalPhase)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        /* Check for valid user port ID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Map user port to system port */
        port = dva_g_userPort2sysPort[userPort];

        /* Check supported ports */
        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Set PHY mode reference clock select */
        if(1u == phyModePhase)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LSP, DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LEN, 1u));
        }
        else if(0u == phyModePhase)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LSP, DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LEN, 0u));
        }
        else
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        /* Set internal reference clock select */
        if(1u == internalPhase)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LSP, DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LEN, 1u));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LSP, DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LEN, 0u));
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get RMII PHY mode and internal mode refer clock selection, user could get as phy mode and internal mode clock phase latch or negative latch.
 * @param[in] unit              Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort          port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                              The port id indicates which port mode is configured.
 * @param[out] phyModePhase      PHY mode reference clock selection
 *                              Set PHY mode reference clock, 0 = original phase , 1 = Invert clock phase of output reference clock
 * @param[out] internalPhase     Internal reference clock selection
 *                              Set TX sampling edge, 0 = Negative sampling, 1 = Invert clock phase for internal MAC
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly
 * @retval -RT_ERR_PORT_ID      The port ID is out of range
 * @retval Others               Please refer to dva_drv_indirect_reg_field_get()
 * @note This API get the RMII PHY mode and internal mode refer clock selection.
 */
RtkApiRet dva_port_rmii_refclk_sel_get(uint32 unit, RtkPort userPort, uint32 *phyModePhase, uint32 *internalPhase)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        /* Check for valid user port ID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Map user port to system port */
        port = dva_g_userPort2sysPort[userPort];

        /* Check supported ports */
        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        /* Get PHY mode reference clock select configuration */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LSP, DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_PHY_MODE_REFCLK_SEL_NEG_LEN, phyModePhase));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_RMII_CLK_PHASE_CTRL + (port * PERPORT_REG_OFFSET), DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LSP, DVA_PORT0_RMII_CLK_PHASE_CTRL_RG_RMII_INTERNAL_REFCLK_SEL_NEG_LEN, internalPhase));

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* INTF_RMII_CFG_ASIC */
/**@}*/ /* INTF_RMII_CFG */
/**@}*/ /* Interface */
