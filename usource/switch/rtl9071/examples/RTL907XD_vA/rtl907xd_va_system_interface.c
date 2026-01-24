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
#include "rtkas_api_phy.h"
#include "rtkas_api_interface.h"
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_reg_struct.h"

/*Declaration*/
RtkApiRet dva_port_t1_mode_set_master_example(void);
RtkApiRet dva_port_t1_mode_set_slave_example(void);
RtkApiRet dva_fe_autoNego_set_example(void);
RtkApiRet dva_fe_force_Full_100M_set_example(void);
RtkApiRet dva_rgmii_tx_delay_set_example(void);
RtkApiRet dva_rgmii_rx_delay_set_example(void);
RtkApiRet dva_xmii_driving_set_example(void);
RtkApiRet dva_xmii_latch_edge_set_example(void);
RtkApiRet dva_port_xmii_ssc_enable_example(void);
RtkApiRet dva_port_packet_max_length_enable_example(void);
RtkApiRet dva_port_rmii_refclk_sel_phy_mode_example(void);
RtkApiRet dva_xmii_link_status_set_example(void);

#define DVA_TX_DELAY_MODE       (0x1u)
#define DVA_RX_DELAY_3NS        (0x3u)
#define DVA_XMII_SSC_MIN_OFFSET (0x0u)
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup INTF_EXAMPLE Interface Example
 * @{
 */

/**
 * @addtogroup RTL907XD_VA_INTF RTL907XD_VA Interface Example
 * @{
 */

/**
 * @addtogroup RTL907XD_VA_MODE_ROLE_CFG_EXAMPLE T1-PHY Interface mode and Role Configure Example
 * @{
 */

/**
 * @brief <b>Configure port 6 to 1000BASE-T1 and set the PHY role to master</b>
 *
 * @section Purpose
 *   Set port 6 to 1000BASE-T1 and configure the PHY role.
 *
 * @section Description
 *   This function sets port 6 to 1000BASE-T1 and configures the PHY role to master.
 */
RtkApiRet dva_port_t1_mode_set_master_example(void)
{
    RtkApiRet        ret = RT_ERR_OK;
    PortMode_t       portModeSet, portModeGet;
    PhyRoleCfgInfo_t phyRoleCfgSetInfo, phyRoleCfgGetInfo;
    UnitChip_t       unitChip;

    do
    {
        /* Step 1. Set port6 as 1000base-T1 master */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portModeSet, 0, sizeof(portModeSet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&phyRoleCfgSetInfo, 0, sizeof(phyRoleCfgSetInfo)));

        unitChip.unit              = DVA_UNIT;
        unitChip.chip              = CHIP_907XD_VA;
        portModeSet.userPort       = DVA_RTK_USER_PORT_6TH;
        portModeSet.mode           = EM_MODE_EXT_RTPG_T1_PHY;
        phyRoleCfgSetInfo.isMaster = 1u;
        phyRoleCfgSetInfo.port     = DVA_RTK_USER_PORT_6TH;

        CHK_FUN_CONTINUE(ret, rtk_port_t1_mode_set(unitChip, &portModeSet));
        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_set(unitChip, &phyRoleCfgSetInfo));

        /* Step 2. Get port interface */
        CHK_FUN_CONTINUE(ret, drv_memset(&portModeGet, 0, sizeof(portModeGet)));
        portModeGet.userPort = DVA_RTK_USER_PORT_6TH;

        CHK_FUN_CONTINUE(ret, rtk_port_mode_get(unitChip, &portModeGet));
        /* Step 3. Compare the port6 current interface mode with the designated configured mode */
        if(portModeSet.mode != portModeGet.mode)
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

        /* Step 4. Get role */
        CHK_FUN_CONTINUE(ret, drv_memset(&phyRoleCfgGetInfo, 0, sizeof(phyRoleCfgGetInfo)));
        phyRoleCfgGetInfo.port = DVA_RTK_USER_PORT_6TH;

        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_get(unitChip, &phyRoleCfgGetInfo));
        /* step 5. Check role */
        if(phyRoleCfgSetInfo.isMaster != phyRoleCfgGetInfo.isMaster)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 6. Link partner need to configure correspond port as 1000base-T1 slave mode */

    } while(0u == 1u);

    /** Expected Behaviour:
     *    Check interface is correct => step 3
     *    Check master role is correct => step 5
     *    Port6 is link up and send the packet the link partner.
     *    The check function return value "ret" must return RT_ERR_OK.
     */

    return ret;
}

/**
 * @brief <b>Configuring port 6 to 1000BASE-T1 and setting the PHY role to slave</b>
 *
 * @section Purpose
 *   Set port 6 to 1000BASE-T1 and configure the PHY role.
 *
 * @section Description
 *   This function sets port 6 to 1000BASE-T1 and configures the PHY role to slave.
 */
RtkApiRet dva_port_t1_mode_set_slave_example(void)
{
    RtkApiRet        ret = RT_ERR_OK;
    PortMode_t       portModeSet, portModeGet;
    PhyRoleCfgInfo_t phyRoleCfgSetInfo, phyRoleCfgGetInfo;
    UnitChip_t       unitChip;

    do
    {
        /* Step 1. Set port6 as 1000base-T1 slave */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portModeSet, 0, sizeof(portModeSet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&phyRoleCfgSetInfo, 0, sizeof(phyRoleCfgSetInfo)));

        unitChip.unit              = DVA_UNIT;
        unitChip.chip              = CHIP_907XD_VA;
        portModeSet.userPort       = DVA_RTK_USER_PORT_6TH;
        portModeSet.mode           = EM_MODE_EXT_RTPG_T1_PHY;
        phyRoleCfgSetInfo.isMaster = 0u;
        phyRoleCfgSetInfo.port     = DVA_RTK_USER_PORT_6TH;

        CHK_FUN_CONTINUE(ret, rtk_port_t1_mode_set(unitChip, &portModeSet));
        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_set(unitChip, &phyRoleCfgSetInfo));

        /* Step 2. Get port interface */
        CHK_FUN_CONTINUE(ret, drv_memset(&portModeGet, 0, sizeof(portModeGet)));
        portModeGet.userPort = DVA_RTK_USER_PORT_6TH;

        CHK_FUN_CONTINUE(ret, rtk_port_mode_get(unitChip, &portModeGet));
        /* Step 3. Check interface */
        if(portModeSet.mode != portModeGet.mode)
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

        /* Step 4. Get role */
        CHK_FUN_CONTINUE(ret, drv_memset(&phyRoleCfgGetInfo, 0, sizeof(phyRoleCfgGetInfo)));
        phyRoleCfgGetInfo.port = DVA_RTK_USER_PORT_6TH;

        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_get(unitChip, &phyRoleCfgGetInfo));

        /* Step 5. Check role */
        if(phyRoleCfgSetInfo.isMaster != phyRoleCfgGetInfo.isMaster)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 6. Link partner need to configure correspond port as 1000base-T1 slave mode */

    } while(0u == 1u);

    /** Expected Behaviour:
     *    Check interface is correct => step 3
     *    Port6 is link up and send the packet the link partner.
     *    The check function return value "ret" must return RT_ERR_OK.
     */

    return ret;
}
/**@}*/ /* RTL907XD_VA_MODE_ROLE_CFG_EXAMPLE */

/**
 * @addtogroup RTL907XD_VA_ETH_AUTONEGO_CFG_EXAMPLE Auto-Negotiation Capability of Ethernet Configure Example
 * @{
 */

/**
 * @brief <b>Set FastEthernet as Auto-Negotiation (AN) mode</b>
 *
 * @section Purpose
 *   Configure port 7 to operate in FE AN mode with specified link capabilities.
 *
 * @section Description
 *   This function configures port 7 to operate in FastEthernet (FE) Auto-Negotiation mode. \n
 *   It sets the link capability to support both half and full duplex modes at 10 Mbps and 100 Mbps.
 */
RtkApiRet dva_fe_autoNego_set_example(void)
{
    PortEthLinkAbility_t    portEthLinkAbility;
    PortEthLinkAbilitySet_t feAbility;
    RtkApiRet               ret = RT_ERR_OK;
    UnitChip_t              unitChip;

    do
    {
        /* Step 1. Link partner need to set as FE */

        /* Step 2. Set port7 as FE AN mode and link capability support half/full duplex and 10M/100M */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portEthLinkAbility, 0, sizeof(portEthLinkAbility)));
        CHK_FUN_CONTINUE(ret, drv_memset(&feAbility, 0, sizeof(feAbility)));

        unitChip.unit                             = DVA_UNIT;
        unitChip.chip                             = CHIP_907XD_VA;
        feAbility.userPort                        = DVA_RTK_USER_PORT_7TH;
        feAbility.pEthAbility                     = &portEthLinkAbility;
        feAbility.pEthAbility->speed              = 0;
        feAbility.pEthAbility->autoNegoCapability = AN_10M_SPEED_HALF_DUPLEX | AN_10M_SPEED_FULL_DUPLEX | AN_100M_SPEED_HALF_DUPLEX | AN_100M_SPEED_FULL_DUPLEX;
        feAbility.pEthAbility->duplex             = 0;
        feAbility.pEthAbility->autoNego           = 1;

        CHK_FUN_CONTINUE(ret, rtk_port_eth_link_ability_set(unitChip, &feAbility));

        /* Expected Behaviour:
         *    Port7 is link up and send the packet to link partner.
         *    The check function return value "ret" must return RT_ERR_OK.
         */

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XD_VA_ETH_AUTONEGO_CFG_EXAMPLE */

/**
 * @addtogroup RTL907XD_VA_ETH_FORCEMODE_CFG_EXAMPLE Force Mode Configuration of Ethernet Configure Example
 * @{
 */

/**
 * @brief <b>Configuring port 7 to force 100 Mbps full duplex mode</b>
 *
 * @section Purpose
 *   Configure port 7 to operate in FE mode with a forced 100 Mbps full duplex setting.
 *
 * @section Description
 *   This function sets port 7 to operate in FastEthernet (FE) mode with a forced configuration of 100 Mbps full duplex. It disables Auto-Negotiation (AN) and directly sets the link speed and duplex mode.
 */
RtkApiRet dva_fe_force_Full_100M_set_example(void)
{
    PortEthLinkAbility_t    portEthLinkAbility;
    PortEthLinkAbilitySet_t feAbility;
    RtkApiRet               ret = RT_ERR_OK;
    UnitChip_t              unitChip;

    do
    {
        /* Step 1. Link partner need to set as FE */

        /* Step 2. Set port7 as FE force 100M full */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portEthLinkAbility, 0, sizeof(portEthLinkAbility)));
        CHK_FUN_CONTINUE(ret, drv_memset(&feAbility, 0, sizeof(feAbility)));

        unitChip.unit                   = DVA_UNIT;
        unitChip.chip                   = CHIP_907XD_VA;
        feAbility.userPort              = DVA_RTK_USER_PORT_7TH;
        feAbility.pEthAbility           = &portEthLinkAbility;
        feAbility.pEthAbility->speed    = (uint32)EM_PORT_SPEED_100M;
        feAbility.pEthAbility->duplex   = PORT_FULL_DUPLEX;
        feAbility.pEthAbility->autoNego = 0;

        CHK_FUN_CONTINUE(ret, rtk_port_eth_link_ability_set(unitChip, &feAbility));

        /* Expected Behaviour:
         *    Port7 is link up and send the packet to link partner.
         *    The check function return value "ret" must return RT_ERR_OK.
         */

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XD_VA_ETH_FORCEMODE_CFG_EXAMPLE */

/**
 * @addtogroup RTL907XD_VA_RGMII_CFG_EXAMPLE RGMII Configuration Example
 * @{
 */

/**
 * @brief <b>Set RGMII TX delay</b>
 *
 * @section Purpose
 *   Configure port 3 to operate with a specific TX delay mode in RGMII.
 *
 * @section Description
 *   This function configures the TX delay mode on port 3 for RGMII (Reduced Gigabit Media Independent Interface). \n
 *   The Link partner and DUT need to set as rgmii. \n
 *   It sets the delay mode and verifies the configuration by reading back the set value.
 */
RtkApiRet dva_rgmii_tx_delay_set_example(void)
{
    RtkApiRet        ret = RT_ERR_OK;
    UnitChip_t       unitChip;
    PortRgmiiTxdly_t portRgmiiTxdlySet, portRgmiiTxdlyGet;

    do
    {
        /* Step 1. Set tx delay mode on port 3 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portRgmiiTxdlySet, 0, sizeof(portRgmiiTxdlySet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portRgmiiTxdlyGet, 0, sizeof(portRgmiiTxdlyGet)));

        unitChip.unit              = DVA_UNIT;
        unitChip.chip              = CHIP_907XD_VA;
        portRgmiiTxdlySet.delay    = DVA_TX_DELAY_MODE;
        portRgmiiTxdlySet.userPort = DVA_RTK_USER_PORT_3RD;
        portRgmiiTxdlyGet.userPort = DVA_RTK_USER_PORT_3RD;

        CHK_FUN_CONTINUE(ret, rtk_port_rgmii_txdly_set(unitChip, &portRgmiiTxdlySet));

        /* Step 2. Get tx delay mode on port 3 */
        CHK_FUN_CONTINUE(ret, rtk_port_rgmii_txdly_get(unitChip, &portRgmiiTxdlyGet));

        /* Expected Behaviour:
         *    Value of portRgmiiTxdlyGet.delay should be DVA_TX_DELAY_MODE.
         *    The check function return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}

/**
 * @brief <b>Set RGMII RX delay</b>
 *
 * @section Purpose
 *   Configure port 3 to operate with a specific RX delay mode in RGMII.
 *
 * @section Description
 *   This function configures the RX delay mode on port 3 for RGMII (Reduced Gigabit Media Independent Interface). \n
 *   The Link partner and DUT need to set as rgmii. \n
 *   It sets the delay mode and verifies the configuration by reading back the set value.
 */
RtkApiRet dva_rgmii_rx_delay_set_example(void)
{
    RtkApiRet           ret = RT_ERR_OK;
    UnitChip_t          unitChip;
    PortRgmiiRxdlySet_t portRgmiiRxdlySet;
    PortRgmiiRxdlyGet_t portRgmiiRxdlyGet;

    do
    {
        /* Step 1. Set rx delay mode on port 3 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portRgmiiRxdlySet, 0, sizeof(portRgmiiRxdlySet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portRgmiiRxdlyGet, 0, sizeof(portRgmiiRxdlyGet)));

        unitChip.chip              = CHIP_907XD_VA;
        unitChip.unit              = DVA_UNIT;
        portRgmiiRxdlySet.userPort = DVA_RTK_USER_PORT_3RD;
        portRgmiiRxdlySet.delay    = DVA_RX_DELAY_3NS;
        portRgmiiRxdlySet.speed    = EM_PORT_SPEED_1000M;
        portRgmiiRxdlyGet.userPort = DVA_RTK_USER_PORT_3RD;

        CHK_FUN_CONTINUE(ret, rtk_port_rgmii_rxdly_set(unitChip, &portRgmiiRxdlySet));

        /* Step 2. Get ex delay mode on port 3 */
        CHK_FUN_CONTINUE(ret, rtk_port_rgmii_rxdly_get(unitChip, &portRgmiiRxdlyGet));

        /* Expected Behaviour:
         *    Value of portRgmiiRxdlyGet.delay should be DVA_RX_DELAY_3NS.
         *    The check function return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XD_VA_RGMII_CFG_EXAMPLE */

/**
 * @addtogroup RTL907XD_VA_XMII_CFG_EXAMPLE XMII Configuration Example
 * @{
 */

/**
 * @brief <b>Set xMII driving</b>
 *
 * @section Purpose
 *   Configure port 7 to operate with specific xMII driving settings.
 *
 * @section Description
 *   This function configures the xMII (Media Independent Interface) driving settings on port 7. \n
 *   The Link partner and DUT need to set as rmii. \n
 *   It sets the driving strength for the base, fall, and rise times and verifies the configuration by reading back the set values.
 */
RtkApiRet dva_xmii_driving_set_example(void)
{
    RtkApiRet         ret = RT_ERR_OK;
    UnitChip_t        unitChip;
    PortXmiiDriving_t portXmiiDrivingSet, portXmiiDrivingGet;

    do
    {
        /* Step 1. Set xmii signal driving to mode & mode2 on port 7 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portXmiiDrivingSet, 0, sizeof(portXmiiDrivingSet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portXmiiDrivingGet, 0, sizeof(portXmiiDrivingGet)));

        unitChip.chip               = CHIP_907XD_VA;
        unitChip.unit               = DVA_UNIT;
        portXmiiDrivingSet.userPort = DVA_RTK_USER_PORT_7TH;
        portXmiiDrivingSet.base     = 3u;
        portXmiiDrivingSet.fall     = 2u;
        portXmiiDrivingSet.rise     = 2u;
        portXmiiDrivingGet.userPort = DVA_RTK_USER_PORT_7TH;

        CHK_FUN_CONTINUE(ret, rtk_port_xmii_driving_set(unitChip, &portXmiiDrivingSet));

        /* Step 2. Get xmii signal driving on port 7 */
        CHK_FUN_CONTINUE(ret, rtk_port_xmii_driving_get(unitChip, &portXmiiDrivingGet));

        /* Expected Behaviour:
         *    The signal driving is successfully set to mode & mode2.
         *    The check function return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}

/**
 * @brief <b>Set xMII latch edge</b>
 *
 * @section Purpose
 *   Configure port 7 to operate with specific xMII latch edge settings.
 *
 * @section Description
 *   This function configures the xMII (Media Independent Interface) latch edge settings on port 7. \n
 *   The Link partner and DUT need to set as rmii. \n
 *   It sets the RX and TX latch edge modes and verifies the configuration by reading back the set values.
 */
RtkApiRet dva_xmii_latch_edge_set_example(void)
{
    RtkApiRet           ret = RT_ERR_OK;
    UnitChip_t          unitChip;
    PortXmiiLatchEdge_t portXmiiLatchEdgeSet, portXmiiLatchEdgeGet;

    do
    {
        /* Step 1. Invert xMII RXC and TXC clock source on port 7 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portXmiiLatchEdgeSet, 0, sizeof(portXmiiLatchEdgeSet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portXmiiLatchEdgeGet, 0, sizeof(portXmiiLatchEdgeGet)));

        unitChip.chip                      = CHIP_907XD_VA;
        unitChip.unit                      = DVA_UNIT;
        portXmiiLatchEdgeSet.isRxNegSample = true;
        portXmiiLatchEdgeSet.isTxNegSample = true;
        portXmiiLatchEdgeSet.userPort      = DVA_RTK_USER_PORT_7TH;
        portXmiiLatchEdgeGet.userPort      = DVA_RTK_USER_PORT_7TH;

        CHK_FUN_CONTINUE(ret, rtk_port_xmii_latch_edge_set(unitChip, &portXmiiLatchEdgeSet));

        /* Step 2. Get xMII RXC and TXC control on port 7 */
        CHK_FUN_CONTINUE(ret, rtk_port_xmii_latch_edge_get(unitChip, &portXmiiLatchEdgeGet));

        /* Expected Behaviour:
         *    The xMII RXC and TXC clock source successfully inverted.
         *    The check function return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}

/**
 * @brief <b>Enable spread signal control to minimum phase offset for xMII</b>
 *
 * @section Purpose
 *   Configure port 7 to enable SSC with the minimum phase offset.
 *
 * @section Description
 *   This function configures the spread signal control (SSC) settings to the minimum phase offset on port 7. \n
 *   It enables SSC and verifies the configuration by reading back the set values.
 */
RtkApiRet dva_port_xmii_ssc_enable_example(void)
{
    RtkApiRet     ret = RT_ERR_OK;
    UnitChip_t    unitChip;
    PortXmiiSsc_t pPortXmiiMaxSscSetPara, pPortXmiiSscGetPara;

    do
    {
        /* Step 1. Set spread signal control to minimum phase offset on port 7 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&pPortXmiiMaxSscSetPara, 0, sizeof(pPortXmiiMaxSscSetPara)));
        CHK_FUN_CONTINUE(ret, drv_memset(&pPortXmiiSscGetPara, 0, sizeof(pPortXmiiSscGetPara)));

        unitChip.chip                             = CHIP_907XD_VA;
        unitChip.unit                             = DVA_UNIT;
        pPortXmiiMaxSscSetPara.userPort           = DVA_RTK_USER_PORT_7TH;
        pPortXmiiMaxSscSetPara.isEnable           = ENABLE;
        pPortXmiiMaxSscSetPara.sequenceModeOffset = DVA_XMII_SSC_MIN_OFFSET;
        pPortXmiiMaxSscSetPara.phaseOffset        = DVA_XMII_SSC_MIN_OFFSET;

        CHK_FUN_CONTINUE(ret, rtk_port_xmii_ssc_set(unitChip, &pPortXmiiMaxSscSetPara));

        pPortXmiiSscGetPara.userPort = DVA_RTK_USER_PORT_7TH;
        /* Step 2. Get min spread signal control offset on port 7 */
        CHK_FUN_CONTINUE(ret, rtk_port_xmii_ssc_get(unitChip, &pPortXmiiSscGetPara));

        /* Expected Behaviour:
         *    The value of pPortXmiiSscGetPara.isEnable should be equal to 1.
         *    The value of pPortXmiiSscGetPara.sequenceModeOffset should be equal to 7.
         *    The value of pPortXmiiSscGetPara.phaseOffset should be equal to 7.
         *    The check function return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}

/**
 * @brief <b>Set xMII link status</b>
 *
 * @section Purpose
 *   Configure port 3 to operate with specific xMII link status settings.
 *
 * @section Description
 *   This function configures the xMII (Media Independent Interface) link status on port 3. \n
 *   It sets the link speed, duplex mode, and other link parameters, then verifies the configuration by reading back the set values.
 */
RtkApiRet dva_xmii_link_status_set_example(void)
{
    RtkApiRet                  ret = RT_ERR_OK;
    UnitChip_t                 unitChip;
    PortLinkConfigurationSet_t portLinkCfgSet;
    PortLinkStatusGet_t        portLinkStatusGet;

    do
    {
        /* Step 1. Set xMII link status on port 3 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portLinkCfgSet, 0, sizeof(portLinkCfgSet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portLinkStatusGet, 0, sizeof(portLinkStatusGet)));

        unitChip.chip           = CHIP_907XD_VA;
        unitChip.unit           = DVA_UNIT;
        portLinkCfgSet.userPort = DVA_RTK_USER_PORT_3RD;
        /* Set speed to 1000M */
        portLinkCfgSet.macAbility.speed = (uint32)EM_PORT_SPEED_1000M;

        CHK_FUN_CONTINUE(ret, rtk_port_xmii_link_status_set(unitChip, &portLinkCfgSet));

        /* Step 2. Get xMII link status on port 3 */
        portLinkStatusGet.userPort = DVA_RTK_USER_PORT_3RD;
        CHK_FUN_CONTINUE(ret, rtk_port_link_status_get(unitChip, &portLinkStatusGet));

        /* Expected Behaviour:
         *    The link status should reflect the settings applied.
         *    The check function return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XD_VA_XMII_CFG_EXAMPLE */

/**
 * @addtogroup RTL907XD_VA_PKTMAXLEN_CFG_EXAMPLE Packet Maximum Length Configuration Example
 * @{
 */

/**
 * @brief <b>Enable maximum packet length (10K bytes) received/transmit on the port</b>
 *
 * @section Purpose
 *   Configure port 7 to enable the reception and transmission of packets up to 10K bytes.
 *
 * @section Description
 *   This function configures the maximum packet length settings on port 7. \n
 *   It enables the maximum packet length and verifies the configuration by reading back the set values.
 */
RtkApiRet dva_port_packet_max_length_enable_example(void)
{
    RtkApiRet          ret = RT_ERR_OK;
    UnitChip_t         unitChip;
    PortPktMaxlenSet_t portPktMaxlenSet;
    PortPktMaxlenGet_t portPktMaxlenGet;

    do
    {
        /* Step 1. Set maximum packet length config on port 7 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portPktMaxlenSet, 0, sizeof(portPktMaxlenSet)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portPktMaxlenGet, 0, sizeof(portPktMaxlenGet)));

        unitChip.chip             = CHIP_907XD_VA;
        unitChip.unit             = DVA_UNIT;
        portPktMaxlenSet.enable   = 1u;
        portPktMaxlenSet.userPort = DVA_RTK_USER_PORT_7TH;
        portPktMaxlenSet.userPort = DVA_RTK_USER_PORT_7TH;

        CHK_FUN_CONTINUE(ret, rtk_port_pkt_maxlen_set(unitChip, &portPktMaxlenSet));

        /* Step 2. Get maximum packet length config on port 7 */
        CHK_FUN_CONTINUE(ret, rtk_port_pkt_maxlen_get(unitChip, &portPktMaxlenGet));

        /* Expected Behaviour:
         *    The value of portPktMaxlenGet.maxLen should be equal to 0x2800.
         *    The check funtion return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XD_VA_PKTMAXLEN_CFG_EXAMPLE */

/**
 * @addtogroup RTL907XD_VA_RMII_REF_CLK_CFG_EXAMPLE RMII reference Clock Selection Configuration Example
 * @{
 */

/**
 * @brief <b>Invert reference clock of RMII PHY mode on the port</b>
 *
 * @section Purpose
 *   Invert reference clock of RMII PHY mode.
 *
 * @section Description
 *   This function configures RMII PHY mode settings on port 7. \n
 *   Invert reference clock of RMII PHY mode and verifies the configuration by reading back the set values..
 */
RtkApiRet dva_port_rmii_refclk_sel_phy_mode_example(void)
{
    RtkApiRet           ret = RT_ERR_OK;
    UnitChip_t          unitChip;
    PortRmiiRefclkSel_t portRmiiRefclkSelSet;
    PortRmiiRefclkSel_t portRmiiRefclkSelGet;

    do
    {
        /* Step 1. Set max packet length config on port 7 */
        CHK_FUN_CONTINUE(ret, drv_memset(&unitChip, 0, sizeof(unitChip)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portRmiiRefclkSelSet, 0, sizeof(PortRmiiRefclkSel_t)));
        CHK_FUN_CONTINUE(ret, drv_memset(&portRmiiRefclkSelGet, 0, sizeof(PortRmiiRefclkSel_t)));

        unitChip.chip                      = CHIP_907XD_V1;
        unitChip.unit                      = DVA_UNIT;
        portRmiiRefclkSelSet.userPort      = DVA_RTK_USER_PORT_7TH;
        portRmiiRefclkSelSet.phyModePhase  = 1u;
        portRmiiRefclkSelSet.internalPhase = 0u;
        portRmiiRefclkSelGet.userPort      = DVA_RTK_USER_PORT_7TH;

        CHK_FUN_CONTINUE(ret, rtk_port_rmii_refclk_sel_set(unitChip, &portRmiiRefclkSelSet));

        /* Step 2. Set max packet length config on port 7 */
        CHK_FUN_CONTINUE(ret, rtk_port_rmii_refclk_sel_get(unitChip, &portRmiiRefclkSelGet));

        /* Expected Behaviour:
         *    The RMII reference clock source successfully inverted.
         *    The check function return value "ret" must return RT_ERR_OK.
         */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XD_VA_RMII_REF_CLK_CFG_EXAMPLE */
/**@}*/ /* RTL907XD_VA_INTF */
/**@}*/ /* INTF_EXAMPLE */
/**@}*/ /* EXAMPLE */