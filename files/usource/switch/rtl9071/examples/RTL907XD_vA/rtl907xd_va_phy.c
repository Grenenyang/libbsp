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

#if defined(RTL_907XD_VA)
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_api_phy.h"
#endif

#define DVA_T1_PORTMAP ((DVA_RTK_USER_PORT_1ST << 1u) | (DVA_RTK_USER_PORT_2ND << 1u) | (DVA_RTK_USER_PORT_3RD << 1u) | (DVA_RTK_USER_PORT_4TH << 1u) | (DVA_RTK_USER_PORT_5TH << 1u) | (DVA_RTK_USER_PORT_6TH << 1u))
#define OUI_REALTEK    (0x732u)

RtkApiRet dva_enable_phy_loopback_mode(RtkPmap pmap);
RtkApiRet dva_enable_phy_isolate_mode(RtkPmap pmap);
RtkApiRet dva_set_phy_force_link_speed(RtkPort port, uint32 speed, uint8 isMaster);
RtkApiRet dva_set_phy_enable_an_mode(RtkPmap pmap);
RtkApiRet dva_set_phy_reset(RtkPmap pmap);
RtkApiRet dva_set_phy_role_master(RtkPort port);
RtkApiRet dva_set_phy_role_slave(RtkPort port);
RtkApiRet dva_get_phy_status(RtkPort port);
RtkApiRet dva_get_phy_mib_counter(RtkPort port, uint32 *pCntVal, uint32 *pErrCntVal);
RtkApiRet dva_set_ethphy_loopback_mode(void);
RtkApiRet dva_set_ethphy_isolate_mode(void);
RtkApiRet dva_set_ethphy_force_link_speed(uint32 speed);
RtkApiRet dva_set_ethphy_enable_an_mode(void);
RtkApiRet dva_set_ethphy_reset(void);
RtkApiRet dva_set_ethphy_role_auto(void);
RtkApiRet dva_set_ethphy_role_master(void);
RtkApiRet dva_set_ethphy_role_slave(void);
RtkApiRet dva_get_ethphy_status(void);
RtkApiRet dva_set_ethphy_pwr_save_mode(RtkEnable enPwrSaving);
RtkApiRet dva_set_ethphy_mdix_mode(uint8 mode);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup PHY_EXAMPLE PHY Example
 * @{
 */

/** @defgroup RTL907XD_VA_PHY_EXAMPLE RTL907XD_VA PHY Example
 *  @{
 */

/**
 * @defgroup RTL907XD_VA_PHY_BASIC_MODE_CONTROL_EXAMPLE T1-PHY Basic Mode Control Register Example
 * @brief <b>Configuring and managing T1-PHY by Basic Mode Control Register</b>
 *
 * @section Purpose
 * Demonstrates how to configure and manage various aspects of T1-PHY using the SDK API.
 *
 * @section Description
 * The functions cover the following aspects:
 * - Configuring the PCS loopback mode for T1 ports.
 * - Enabling isolate mode for all T1 ports.
 * - Forcing the link speed of a T1-PHY.
 * - Enabling auto-negotiation (AN) mode for all T1 ports.
 * - Resetting all T1 ports.
 * @{
 */

/**
 * @brief Enable the PCS loopback mode for the specified T1-PHY ports.
 *
 * @details
 * This function uses the SDK API to enable the PCS loopback mode for the specified T1-PHY ports.
 *
 * @section Purpose
 * This function demonstrates how to enable the PCS loopback mode for T1-PHY ports using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Disable PCS loopback mode for all T1 ports specified in the port map.
 * - Step 2: Enable PCS loopback mode for the specified ports.
 * - Step 3: Retrieve the port map of T1 ports that have PCS loopback mode enabled.
 * - Step 4: Check if the retrieved port map matches the input port map. If not, return an error.
 */
RtkApiRet dva_enable_phy_loopback_mode(RtkPmap pmap)
{
    RtkApiRet        ret = RT_ERR_OK;
    UnitChip_t       uintChip;
    PhyPcsLoopback_t lbkSet;
    PhyPcsLoopback_t lbkeGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Disable PCS loopback mode for all T1 ports */
        lbkSet.portMap   = DVA_T1_PORTMAP;
        lbkSet.enableCfg = DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_pcs_loopback_set(uintChip, &lbkSet));

        /* Step 2. Enable PCS loopback mode for the T1 ports specified in the port map */
        lbkSet.portMap   = pmap;
        lbkSet.enableCfg = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_pcs_loopback_set(uintChip, &lbkSet));

        /* Step 3. Get the port map of T1 ports with PCS loopback mode enabled */
        lbkeGet.portMap = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_pcs_loopback_get(uintChip, &lbkeGet));

        /* Expected Behavior:
         *    The retrieved port map should be map matches the input port map
         */
        if(lbkSet.portMap != lbkeGet.portMap)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Enable isolate mode for the specified T1-PHY ports.
 *
 * @details
 * This function uses the SDK API to enable isolate mode for the specified T1-PHY ports.
 *
 * @section Purpose
 * This function demonstrates how to enable isolate mode for T1-PHY ports using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Disable isolate mode for all T1 ports.
 * - Step 2: Enable isolate mode for the T1 ports specified in the port map.
 * - Step 3: Retrieve the port map of T1 ports with isolate mode enabled.
 * - Step 4: Check if the retrieved port map matches the input port map. If not, return an error.
 */
RtkApiRet dva_enable_phy_isolate_mode(RtkPmap pmap)
{
    RtkApiRet    ret = RT_ERR_OK;
    UnitChip_t   uintChip;
    PhyIsolate_t isolateSet;
    PhyIsolate_t isolateGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Disable isolate mode for all T1 ports */
        isolateSet.portMap = DVA_T1_PORTMAP;
        isolateSet.isoCfg  = DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_isolate_set(uintChip, &isolateSet));

        /* Step 2: Enable isolate mode for the T1 ports specified in the port map */
        isolateSet.portMap = pmap;
        isolateSet.isoCfg  = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_isolate_set(uintChip, &isolateSet));

        /* Step 3: Get the port map of T1 ports with isolate mode enabled */
        isolateGet.portMap = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_isolate_get(uintChip, &isolateGet));

        /* Expected Behavior:
         *    The retrieved port map should be map matches the input port map
         */
        if(pmap != isolateGet.portMap)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Force the link speed of the specified T1-PHY port.
 *
 * @details
 * This function uses the SDK API to configure the T1-PHY link settings for the specified port, forcing its link speed.
 *
 * @section Purpose
 * This function demonstrates how to force the link speed of a T1-PHY port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Power down the specified port before configuring the PHY link settings.
 * - Step 2: Verify that the specified port is powered down.
 * - Step 3: Force the link speed of the specified port.
 * - Step 4: Verify the port speed setting.
 * - Step 5: Disable auto-negotiation (AN) mode for the specified port.
 * - Step 6: Verify that AN mode is disabled for the specified port.
 * - Step 7: Set the role based on the isMaster parameter.
 * - Step 8: Power up the addressed port.
 * - Verify that the specified port links up with the specified speed.
 */
RtkApiRet dva_set_phy_force_link_speed(RtkPort port, uint32 speed, uint8 isMaster)
{
    RtkApiRet        ret = RT_ERR_OK;
    UnitChip_t       uintChip;
    PhyPwd_t         phyPwd;
    PhySpd_t         spdSet;
    PhySpd_t         spdGet;
    PhyAnMode_t      anModeSet;
    PhyAnMode_t      anModeGet;
    PhyRoleCfgInfo_t roleSet;
    PhyLinkStatus_t  linkStatus;
    RtkPmap          targetPmap;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Power down the specified port before configuring the PHY link settings */
        if(DVA_PHY_T1_USER_PORT_MAX < port)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        targetPmap     = ((RtkPmap)1u << port);
        phyPwd.portMap = targetPmap;
        phyPwd.pwdCfg  = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_power_down_set(uintChip, &phyPwd));

        /* Step 2: Verify the specified port is powered down */
        phyPwd.portMap = 0x0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_power_down_get(uintChip, &phyPwd));
        if(0u == (phyPwd.portMap & targetPmap))
        {
            /* If the port is not powered down, return an error */
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 3: Force the specified port link speed */
        spdSet.port  = port;
        spdSet.speed = speed;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_speed_set(uintChip, &spdSet));

        /* Step 4: Verify the port speed setting */
        spdGet.port  = port;
        spdGet.speed = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_speed_get(uintChip, &spdGet));
        if(speed != spdGet.speed)
        {
            /* If the speed is not set correctly, return an error */
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 5: Disable auto-negotiation (AN) mode for the specified port */
        anModeSet.portMap     = targetPmap;
        anModeSet.anEnableCfg = DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_an_mode_set(uintChip, &anModeSet));

        /* Step 6: Verify that AN mode is disabled for the specified port */
        anModeGet.portMap = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_an_mode_get(uintChip, &anModeGet));
        if(0u == (anModeGet.portMap & targetPmap))
        {
            /* If AN mode is not disabled, return an error */
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 7: Set the role based on the isMaster parameter */
        roleSet.port     = port;
        roleSet.isMaster = isMaster;
        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_set(uintChip, &roleSet));

        /* Step 8: Power up the addressed port */
        phyPwd.portMap = targetPmap;
        phyPwd.pwdCfg  = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_power_down_set(uintChip, &phyPwd));

        /* Verify that the specified port links up with the specified speed */
        linkStatus.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmsr_link_status_get(uintChip, &linkStatus));
        if(PHY_LINKUP != linkStatus.linkSts)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Enable auto-negotiation (AN) mode for the specified T1-PHY ports.
 *
 * @details
 * This function uses the SDK API to enable auto-negotiation (AN) mode for the specified T1-PHY ports.
 *
 * @section Purpose
 * This function demonstrates how to enable AN mode for T1-PHY ports using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Enable AN mode for the specified T1 ports.
 * - Step 2: Command the T1-PHY to restart AN mode.
 */
RtkApiRet dva_set_phy_enable_an_mode(RtkPmap pmap)
{
    RtkApiRet      ret = RT_ERR_OK;
    UnitChip_t     uintChip;
    PhyAnMode_t    anModeSet;
    PhyRestartAn_t restartAn;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Enable AN mode for the specified T1 ports */
        anModeSet.portMap     = pmap;
        anModeSet.anEnableCfg = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_an_mode_set(uintChip, &anModeSet));

        /* Step 2: Command the T1-PHY to restart AN mode */
        restartAn.portMap = pmap;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_restart_an(uintChip, &restartAn));

        /* Expected Behavior:
         * If the link partners of all T1 ports also have AN mode enabled, all T1 ports will link up with the negotiated speed.
         */
    } while(FALSE);

    return ret;
}

/**
 * @brief Reset the specified T1-PHY ports.
 *
 * @details
 * This function uses the SDK API to trigger a reset of the specified T1-PHY ports.
 *
 * @section Purpose
 * This function demonstrates how to reset T1-PHY ports using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Trigger a reset of the specified T1 ports.
 * - Verify that all ports are reset.
 */
RtkApiRet dva_set_phy_reset(RtkPmap pmap)
{
    RtkApiRet      ret = RT_ERR_OK;
    UnitChip_t     uintChip;
    PhyResetInfo_t resetInfo;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Trigger a reset of the specified T1 ports */
        resetInfo.portMap = pmap;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_reset(uintChip, &resetInfo));

        /* Expected Behavior:
         * All specified ports are reset.
         */
    } while(FALSE);

    return ret;
}

/**@}*/ /* RTL907XD_VA_PHY_BASIC_MODE_CONTROL_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_PHY_ROLE_EXAMPLE T1-PHY Role Configuration
 * @brief <b>Functions for configuring the role of T1-PHY ports</b>
 *
 * @section Purpose
 * These functions demonstrate how to configure the role of T1-PHY ports as either master or slave using the SDK API.
 *
 * @section Description
 * The functions perform the following steps:
 * - Set the specified port as master or slave by configuring the PHY Control Register (PHYCR).
 * - Retrieve the role configuration of the specified port to verify it has been set correctly.
 * - Check if the role configuration is correct. If not, return an error.
 * @{
 */

/**
 * @brief Configure the T1-PHY role of the specified port as master.
 *
 * @details
 * This function uses the SDK API to set the PHY Control Register (PHYCR) to configure the PHY role to master.
 *
 * @section Purpose
 * This function demonstrates how to configure the role of a T1-PHY port as master using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Set the specified port as master by configuring the PHYCR register.
 * - Step 2: Retrieve the role configuration of the specified port to verify it has been set to master.
 * - Step 3: Check if the role configuration is correct. If not, return an error.
 */
RtkApiRet dva_set_phy_role_master(RtkPort port)
{
    RtkApiRet        ret = RT_ERR_OK;
    UnitChip_t       uintChip;
    PhyRoleCfgInfo_t roleSet;
    PhyRoleCfgInfo_t roleGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Set the specified port as master */
        roleSet.port     = port;
        roleSet.isMaster = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_set(uintChip, &roleSet));

        /* Step 2: Get the role configuration of the specified port */
        roleGet.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_get(uintChip, &roleGet));

        /* Step 3: Check if the role configuration is correct */
        if(roleGet.isMaster != roleSet.isMaster)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Configure the T1-PHY role of the specified port as slave.
 *
 * @details
 * This function uses the SDK API to set the PHY Control Register (PHYCR) to configure the PHY role to slave.
 *
 * @section Purpose
 * This function demonstrates how to configure the role of a T1-PHY port as slave using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Set the specified port as slave by configuring the PHYCR register.
 * - Step 2: Retrieve the role configuration of the specified port to verify it has been set to slave.
 * - Step 3: Check if the role configuration is correct. If not, return an error.
 */
RtkApiRet dva_set_phy_role_slave(RtkPort port)
{
    RtkApiRet        ret = RT_ERR_OK;
    UnitChip_t       uintChip;
    PhyRoleCfgInfo_t roleSet;
    PhyRoleCfgInfo_t roleGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Set the specified port as slave */
        roleSet.port     = port;
        roleSet.isMaster = 0u;
        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_set(uintChip, &roleSet));

        /* Step 2: Get the role configuration of the specified port */
        roleGet.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_phycr_role_get(uintChip, &roleGet));

        /* Step 3: Check if the role configuration is correct */
        if(roleGet.isMaster != roleSet.isMaster)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}
/**@}*/ /* RTL907XD_VA_PHY_ROLE_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_PHY_STATUS_DUMP_EXAMPLE T1-PHY Status Dump Example
 * @brief <b>Example functions for dumping T1-PHY Status Information</b>
 *
 * @section Purpose
 * Demonstrates how to retrieve and dump T1-PHY status information using the SDK API.
 *
 * @section Description
 * The functions cover various aspects of PHY status retrieval, including:
 * - OUI information
 * - PCS status
 * - PHY status sub-flags
 * - Detailed status from PHY status registers
 *
 * @{
 */

/**
 * @brief Get the PHY status of the specified T1-PHY port.
 *
 * @details
 * This function uses the SDK API to retrieve the PHY status information for the specified T1-PHY port.
 *
 * @section Purpose
 * This function demonstrates how to get the PHY status of a T1-PHY port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Get the OUI information of the specified port.
 * - Step 2: Get the current PCS status of the specified port's T1-PHY.
 * - Step 3: Get the PHY status sub-flag (PHYSFR).
 * - Step 4: Get the PHY role, speed, duplex, and real-time link status from the PHY Status Register 2 (PHYSR2).
 * - Step 5: If the speed is 1000M, get the PHY status from the PHY Status Register 1 (PHYSR1).
 * - Step 6: If the speed is 100M, get the PHY status from the PHY Status Register 4 (PHYSR4).
 */
RtkApiRet dva_get_phy_status(RtkPort port)
{
    RtkApiRet   ret = RT_ERR_OK;
    UnitChip_t  uintChip;
    PhyIdInfo_t phyIdInfo;
    PhySpd_t    phySpd;
    PhyPCS_t    phyPCSFlag;
    PhyPHYSFR_t phyPHYSFR;
    PhyPHYSR1_t phyPHYSR1;
    PhyPHYSR2_t phyPHYSR2;
    PhyPHYSR4_t phyPHYSR4;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Get the OUI information of the specified port */
        phyIdInfo.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_phyid_get(uintChip, &phyIdInfo));
        /* Expected Behavior:
         * The expected OUI is Realtek OUI
         */
        if(OUI_REALTEK != phyIdInfo.oui)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 2: Get the current PCS status of the specified port's T1-PHY */
        phyPCSFlag.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_pcs_info_cfg(uintChip, &phyPCSFlag));
        /* Expected Behavior:
         * If the T1-PHY is linked up with 1000BASE-T1, the PCS state value will be 0x37
         * If the T1-PHY is linked up with 100BASE-T1, the PCS state value will be 0x44
         */
        if(0x37u != phyPCSFlag.phyPCS)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 3: Get the PHY status sub-flag (PHYSFR) */
        phyPHYSFR.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_physfr_info_cfg(uintChip, &phyPHYSFR));
        /* Expected Behavior:
         * The PHYSFR register provides the PHY current status. If the PHY is linked up, the flag will be 0x3
         */
        if(0x3u != phyPHYSFR.phySubFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 4: Get the PHY role, speed, duplex, and real-time link status from the PHY Status Register 2 (PHYSR2) */
        phyPHYSR2.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_physr2_info_cfg(uintChip, &phyPHYSR2));
        /* Expected Behavior:
         * The PHYSR2 register provides the following 100/1000BASE-T1 PHY information:
         * - Master Mode: Role setting after Auto-Negotiation.
         * - Speed
         * - Duplex
         * - Real-time link status
         */
        if(PHY_LINKUP != phyPHYSR2.realTimelinkSts)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        if(PHY_BMCR_SPEED_1000M != phyPHYSR2.speed)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        phySpd.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_speed_get(uintChip, &phySpd));
        if(PHY_BMCR_SPEED_1000M == phySpd.speed)
        {
            /* Step 5: Get the PHY status from the PHY Status Register 1 (PHYSR1) */
            phyPHYSR1.port = port;
            CHK_FUN_CONTINUE(ret, rtk_phy_physr1_info_cfg(uintChip, &phyPHYSR1));
            /* Expected Behavior:
             * If the interface is 1000BASE-T1, the PHYSR1 register includes the following 1000BASE-T1 PHY information:
             * - Master/Slave configuration result
             * - Local receiver status
             * - Remote receiver status
             */
        }
        else if(PHY_BMCR_SPEED_100M == phySpd.speed)
        {
            /* Step 6: Get the PHY status from the PHY Status Register 4 (PHYSR4) */
            phyPHYSR4.port = port;
            CHK_FUN_CONTINUE(ret, rtk_phy_physr4_info_cfg(uintChip, &phyPHYSR4));
            /* Expected Behavior:
             * If the interface is 100BASE-T1, the PHYSR4 register includes the following 100BASE-T1 PHY information:
             * - Transmit Polarity for 100BASE-T1
             * - Receive Polarity for 100BASE-T1
             * - Remote Receiver Status for 100BASE-T1
             * - Local Receiver Status for 100BASE-T1
             */
        }
        else
        {
            /* 10M or another speed not supported */
        }
    } while(FALSE);

    return ret;
}
/**@}*/ /* RTL907XD_VA_PHY_STATUS_DUMP_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_PHY_COUNTER_DUMP_EXAMPLE T1-PHY MIB Counter Example
 * @brief <b>Example functions for dumping T1-PHY MIB Counters</b>
 *
 * @section Purpose
 * Demonstrates how to retrieve and dump T1-PHY internal MIB counters using the SDK API.
 *
 * @section Description
 * This example dumps the PHY internal value of MIB counter registers.
 *
 * @{
 */

/**
 * @brief Get the MIB counter values for the specified T1-PHY port.
 *
 * @details
 * This example uses the SDK API to retrieve the MIB counter values for the specified T1-PHY port and then resets the counters.
 *
 * @section Purpose
 * This example demonstrates how to get and reset the MIB counter values for a T1-PHY port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Set the MIB counter path select of the specified port using the PHY Internal Packet Counter Register 1 (IPCR1) register.
 * - Step 2: Get the current MIB counter values of the specified port's T1-PHY.
 * - Step 3: Reset the PHY MIB counter using the PHY Internal Packet Counter Register 2 (IPCR2) register.
 */
RtkApiRet dva_get_phy_mib_counter(RtkPort port, uint32 *pCntVal, uint32 *pErrCntVal)
{
    RtkApiRet       ret = RT_ERR_OK;
    UnitChip_t      uintChip;
    PhyIPCR1_t      phyIPCR1;
    PhyIPCR2_t      phyIPCR2;
    PhyIPCRCntVal_t phyIPCRCnt;
    uint32          counterVal;
    uint32          errCounterVal;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Set the MIB counter path select of the specified port using the IPCR1 register */
        phyIPCR1.port         = port;
        phyIPCR1.enPktCounter = ENABLE;
        phyIPCR1.enRemoteLbk  = ENABLE;
        phyIPCR1.pathSel      = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_ipcr1_info_cfg(uintChip, &phyIPCR1));
        /* Expected Behavior:
         * The specified port's PHY MIB settings will be:
         * - Enable packet counter
         * - Enable remote loopback mode
         * - Monitor MAC’s TX path
         */

        /* Step 2: Get the current MIB counter values of the specified port's T1-PHY */
        phyIPCRCnt.port = port;
        CHK_FUN_CONTINUE(ret, rtk_phy_ipcr_cnt(uintChip, &phyIPCRCnt));
        /* Expected Behavior:
         * Retrieve the specified port's PHY MIB counter value and error counter value, and then return the values
         */
        counterVal    = ((phyIPCRCnt.intCntMSB & 0xFFFFu) << 16u) | (phyIPCRCnt.intCntLSB & 0xFFFFu);
        *pCntVal      = counterVal;
        errCounterVal = phyIPCRCnt.intErrCnt;
        *pErrCntVal   = errCounterVal;

        /* Step 3: Reset the PHY MIB counter using the IPCR2 register */
        phyIPCR2.port         = port;
        phyIPCR2.intCntClr    = 1u;
        phyIPCR2.intCntErrClr = 1u;
        phyIPCR2.rxPkCntRst   = 1u;
        CHK_FUN_CONTINUE(ret, rtk_phy_ipcr2_info_cfg(uintChip, &phyIPCR2));
        /* Expected Behavior:
         * The PHY MIB counter will be cleared and reset.
         */
    } while(FALSE);

    return ret;
}
/**@}*/ /* RTL907XD_VA_PHY_COUNTER_DUMP_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_ETHPHY_BASIC_MODE_CONTROL_EXAMPLE Ethernet-PHY Basic Mode Control Register Example
 * @brief <b>Configuring and managing Ethernet-PHY by Basic Mode Control Register</b>
 *
 * @section Purpose
 * Demonstrates how to configure and manage various aspects of Ethernet-PHY using the SDK API.
 *
 * @section Description
 * The functions cover the following aspects:
 * - Configuring the PCS loopback mode for the Ethernet port.
 * - Enabling isolate mode for  for the Ethernet port.
 * - Forcing the link speed of the Ethernet port.
 * - Enabling auto-negotiation (AN) mode for the Ethernet port.
 * - Resetting the Ethernet port.
 * @{
 */

/**
 * @brief Configure the PCS loopback mode for the Ethernet port.
 *
 * @details
 * This function uses the SDK API to configure the PCS loopback mode for the Ethernet port.
 *
 * @section Purpose
 * This function demonstrates how to enable or disable the PCS loopback mode for the Ethernet port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Enable PCS loopback mode for the Ethernet port.
 * - Step 2: Retrieve the PCS loopback mode of the Ethernet port.
 * - Step 3: Check if the Ethernet port has PCS loopback mode enabled. If not, return an error.
 * - Step 4: Disable PCS loopback mode for the Ethernet port.
 * - Step 5: Retrieve the PCS loopback mode of the Ethernet port.
 * - Step 6: Check if the Ethernet port has PCS loopback mode disabled. If not, return an error.
 */
RtkApiRet dva_set_ethphy_loopback_mode(void)
{
    RtkApiRet        ret = RT_ERR_OK;
    UnitChip_t       uintChip;
    PhyPcsLoopback_t lbkSet;
    PhyPcsLoopback_t lbkeGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Enable PCS loopback mode for the Ethernet port */
        lbkSet.enableCfg = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_pcs_loopback_set(uintChip, &lbkSet));

        /* Step 2: Get the PCS loopback mode setting of the Ethernet port */
        lbkeGet.enableCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_pcs_loopback_get(uintChip, &lbkeGet));

        /* Expected Behavior:
         * The PCS loopback mode of the Ethernet port is enabled
         */
        if(ENABLE != lbkeGet.enableCfg)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 3: Disable PCS loopback mode for the Ethernet port */
        lbkSet.enableCfg = DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_pcs_loopback_set(uintChip, &lbkSet));

        /* Step 4: Get the PCS loopback mode setting of the Ethernet port */
        lbkeGet.enableCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_phy_bmcr_pcs_loopback_get(uintChip, &lbkeGet));

        /* Expected Behavior:
         * The PCS loopback mode of the Ethernet port is disabled
         */
        if(DISABLE != lbkeGet.enableCfg)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Configure the isolate mode for the Ethernet port.
 *
 * @details
 * This function uses the SDK API to configure the isolate mode for the Ethernet port.
 *
 * @section Purpose
 * This function demonstrates how to enable or disable the isolate mode for the Ethernet port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Enable isolate mode for the Ethernet port.
 * - Step 2: Retrieve the isolate mode of the Ethernet port.
 * - Step 3: Check if the Ethernet port has isolate mode enabled. If not, return an error.
 * - Step 4: Disable isolate mode for the Ethernet port.
 * - Step 5: Retrieve the isolate mode of the Ethernet port.
 * - Step 6: Check if the Ethernet port has isolate mode disabled. If not, return an error.
 */
RtkApiRet dva_set_ethphy_isolate_mode(void)
{
    RtkApiRet    ret = RT_ERR_OK;
    UnitChip_t   uintChip;
    PhyIsolate_t isolateSet;
    PhyIsolate_t isolateGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Enable isolate mode for the Ethernet port */
        isolateSet.isoCfg = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_isolate_set(uintChip, &isolateSet));

        /* Step 2: Get the isolate mode setting of the Ethernet port */
        isolateGet.isoCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_isolate_get(uintChip, &isolateGet));

        /* Expected Behavior:
         * The isolate mode of the Ethernet port is enabled
         */
        if(ENABLE != isolateGet.isoCfg)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 3: Disable isolate mode for the Ethernet port */
        isolateSet.isoCfg = DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_isolate_set(uintChip, &isolateSet));

        /* Step 4: Get the isolate mode setting of the Ethernet port */
        isolateGet.isoCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_isolate_get(uintChip, &isolateGet));

        /* Expected Behavior:
         * The isolate mode of the Ethernet port is disabled
         */
        if(DISABLE != isolateGet.isoCfg)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Force the link speed of the Ethernet port to 1000M.
 *
 * @details
 * This function uses the SDK API to configure the Ethernet link settings for the Ethernet port, forcing its link speed to 1000M.
 *
 * @section Purpose
 * This function demonstrates how to force the link speed of the Ethernet port to 1000M using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Power down the Ethernet port before configuring the link settings.
 * - Step 2: Verify that the Ethernet port is powered down.
 * - Step 3: Force the link speed of the Ethernet port to 1000M.
 * - Step 4: Verify the port speed setting.
 * - Step 5: Disable auto-negotiation (AN) mode for the Ethernet port.
 * - Step 6: Verify that AN mode is disabled for the Ethernet port.
 * - Step 7: Power up the Ethernet port.
 * - Verify that the Ethernet port links up with speed 1000M.
 */
RtkApiRet dva_set_ethphy_force_link_speed(uint32 speed)
{
    RtkApiRet       ret = RT_ERR_OK;
    UnitChip_t      uintChip;
    PhyPwd_t        phyPwd;
    PhySpd_t        spdSet;
    PhySpd_t        spdGet;
    PhyAnMode_t     anModeSet;
    PhyAnMode_t     anModeGet;
    PhyLinkStatus_t linkStatus;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Power down the Ethernet port before configuring the link settings */
        phyPwd.pwdCfg = 1u;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_power_down_set(uintChip, &phyPwd));

        /* Step 2: Verify that the Ethernet port is powered down */
        phyPwd.pwdCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_power_down_get(uintChip, &phyPwd));
        if(1u != phyPwd.pwdCfg)
        {
            /* If the Ethernet port is not powered down, return an error */
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 3: Force the link speed of the Ethernet port to 1000M */
        spdSet.speed = speed;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_speed_set(uintChip, &spdSet));

        /* Step 4: Verify the port speed setting */
        spdGet.speed = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_speed_get(uintChip, &spdGet));
        if(speed != spdGet.speed)
        {
            /* If the speed is not set to 1000M, return an error */
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 5: Disable auto-negotiation (AN) mode for the Ethernet port */
        anModeSet.anEnableCfg = DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_an_mode_set(uintChip, &anModeSet));

        /* Step 6: Verify that AN mode is disabled for the Ethernet port */
        anModeSet.anEnableCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_an_mode_get(uintChip, &anModeGet));
        if(DISABLE != anModeSet.anEnableCfg)
        {
            /* If AN mode is not disabled, return an error */
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 7: Power up the Ethernet port */
        phyPwd.pwdCfg = 0u;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_power_down_set(uintChip, &phyPwd));

        /* Expected Behavior:
         * The Ethernet port links up with the specified speed
         */
        linkStatus.linkSts = 0u;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmsr_link_status_get(uintChip, &linkStatus));
        if(PHY_LINKUP != linkStatus.linkSts)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

    } while(FALSE);

    return ret;
}

/**
 * @brief Enable auto-negotiation (AN) mode for the Ethernet port.
 *
 * @details
 * This function uses the SDK API to enable auto-negotiation (AN) mode for the Ethernet port.
 *
 * @section Purpose
 * This function demonstrates how to enable AN mode for the Ethernet port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Enable AN mode for the Ethernet port.
 * - Step 2: Verify that the link status of the Ethernet port is up.
 * - Step 3: Command the Ethernet-PHY to restart AN mode.
 */
RtkApiRet dva_set_ethphy_enable_an_mode(void)
{
    RtkApiRet       ret = RT_ERR_OK;
    UnitChip_t      uintChip;
    PhyAnMode_t     anModeSet;
    PhyLinkStatus_t linkStatus;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Enable AN mode for the Ethernet port */
        anModeSet.anEnableCfg = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_an_mode_set(uintChip, &anModeSet));

        /* Step 2: Command the Ethernet-PHY to restart AN mode */
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_restart_an(uintChip));

        /* Expected Behavior:
         * If the link partner of the Ethernet port also has AN mode enabled, the Ethernet port will link up with the negotiated speed.
         */
        linkStatus.linkSts = 0x0u;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmsr_link_status_get(uintChip, &linkStatus));
        if(PHY_LINKUP != linkStatus.linkSts)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Reset the Ethernet port.
 *
 * @details
 * This function uses the SDK API to trigger a reset of the Ethernet port.
 *
 * @section Purpose
 * This function demonstrates how to reset the Ethernet port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Trigger a reset of the Ethernet port.
 * - Verify that the port is reset.
 */
RtkApiRet dva_set_ethphy_reset(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t uintChip;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Trigger a reset of the Ethernet port */
        CHK_FUN_CONTINUE(ret, rtk_ethphy_bmcr_reset(uintChip));

        /* Expected Behavior:
         * The Ethernet port is reset.
         */
    } while(FALSE);

    return ret;
}
/**@}*/ /* RTL907XD_VA_ETHPHY_BASIC_MODE_CONTROL_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_ETHPHY_ROLE_EXAMPLE Ethernet-PHY Role Example
 * @brief <b>Example functions for Configuring Ethernet-PHY Role</b>
 *
 * @section Purpose
 * Demonstrates how to configure the Ethernet-PHY role of port 6 as either master or slave using the SDK API.
 *
 * @section Description
 * The functions perform the following steps:
 * - Set the Ethernet-PHY role by configuring the MASTER-SLAVE Control Register (MSCR).
 * - Retrieve the role configuration of Ethernet port to verify it has been set correctly.
 * - Check if the role configuration is correct. If not, return an error.
 *
 * @{
 */

/**
 * @brief Configure the Ethernet port role to auto.
 *
 * @details
 * This function uses the SDK API to set the MASTER-SLAVE Control Register (MSCR) to configure the Ethernet port role to auto.
 *
 * @section Purpose
 * This function demonstrates how to configure the role of the Ethernet port to auto using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Set the Ethernet port role to auto by configuring the MSCR register.
 * - Step 2: Retrieve the role configuration of the Ethernet port to verify it has been set to auto.
 * - Step 3: Check if the role configuration is correct. If not, return an error.
 */
RtkApiRet dva_set_ethphy_role_auto(void)
{
    RtkApiRet    ret = RT_ERR_OK;
    UnitChip_t   uintChip;
    PhyMSCRCfg_t roleSet;
    PhyMSCRCfg_t roleGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Set the Ethernet port role to auto */
        roleSet.mscrRoleCfg = PHY_ROLE_AUTO;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mscr_role_cfg_set(uintChip, &roleSet));

        /* Step 2: Get the Ethernet port role */
        roleGet.mscrRoleCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mscr_role_cfg_get(uintChip, &roleGet));

        /* Expected Behavior:
         * Check if the auto role is correct
         */
        if(PHY_ROLE_AUTO != roleGet.mscrRoleCfg)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Configure the Ethernet port role as master.
 *
 * @details
 * This function uses the SDK API to set the MASTER-SLAVE Control Register (MSCR) to configure the Ethernet port role to master.
 *
 * @section Purpose
 * This function demonstrates how to configure the role of the Ethernet port as master using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Set the Ethernet port as master by configuring the MSCR register.
 * - Step 2: Retrieve the role configuration of the Ethernet port to verify it has been set to master.
 * - Step 3: Check if the role configuration is correct. If not, return an error.
 */
RtkApiRet dva_set_ethphy_role_master(void)
{
    RtkApiRet    ret = RT_ERR_OK;
    UnitChip_t   uintChip;
    PhyMSCRCfg_t roleSet;
    PhyMSCRCfg_t roleGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Set the Ethernet port role to master */
        roleSet.mscrRoleCfg = PHY_ROLE_MANUAL_MASTER;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mscr_role_cfg_set(uintChip, &roleSet));

        /* Step 2: Get the Ethernet port role */
        roleGet.mscrRoleCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mscr_role_cfg_get(uintChip, &roleGet));

        /* Expected Behavior:
         * Check if the master role is correct
         */
        if(PHY_ROLE_MANUAL_MASTER != roleGet.mscrRoleCfg)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**
 * @brief Configure the Ethernet port role as slave.
 *
 * @details
 * This function uses the SDK API to set the MASTER-SLAVE Control Register (MSCR) to configure the Ethernet port role to slave.
 *
 * @return RtkApiRet Returns RT_ERR_OK on success, or an error code on failure.
 *
 * @section Purpose
 * This function demonstrates how to configure the role of the Ethernet port as slave using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Set the Ethernet port as slave by configuring the MSCR register.
 * - Step 2: Retrieve the role configuration of the Ethernet port to verify it has been set to slave.
 * - Step 3: Check if the role configuration is correct. If not, return an error.
 */
RtkApiRet dva_set_ethphy_role_slave(void)
{
    RtkApiRet    ret = RT_ERR_OK;
    UnitChip_t   uintChip;
    PhyMSCRCfg_t roleSet;
    PhyMSCRCfg_t roleGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Set the Ethernet port role to slave */
        roleSet.mscrRoleCfg = PHY_ROLE_MANUAL_SLAVE;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mscr_role_cfg_set(uintChip, &roleSet));

        /* Step 2: Get the Ethernet port role */
        roleGet.mscrRoleCfg = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mscr_role_cfg_get(uintChip, &roleGet));

        /* Expected Behavior:
         * Check if the slave role is correct
         */
        if(PHY_ROLE_MANUAL_SLAVE != roleGet.mscrRoleCfg)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}
/**@}*/ /* RTL907XD_VA_ETHPHY_ROLE_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_ETHPHY_STATUS_DUMP_EXAMPLE Etherent-PHY Status Dump Example
 * @brief <b>Example functions for dumping Etherent-PHY Status Information</b>
 *
 * @section Purpose
 * Demonstrates how to retrieve and dump Etherent-PHY status information using the SDK API.
 *
 * @section Description
 * The functions cover various aspects of Etherent-PHY status retrieval, including:
 * - OUI information
 * - PCS status
 * - PHY status sub-flags
 *
 * @{
 */

/**
 * @brief Get the PHY status of the Ethernet port.
 *
 * @details
 * This function uses the SDK API to retrieve the PHY status information for the Ethernet port.
 *
 * @section Purpose
 * This function demonstrates how to get the PHY status of the Ethernet port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Get the OUI information of the Ethernet port.
 * - Step 2: Get the current PCS status of the Ethernet port's PHY.
 * - Step 3: Get the PHY status sub-flag (PHYSFR).
 */
RtkApiRet dva_get_ethphy_status(void)
{
    RtkApiRet   ret = RT_ERR_OK;
    UnitChip_t  uintChip;
    PhyIdInfo_t phyIdInfo = {0};
    PhyPCS_t    phyPCSFlag;
    PhyPHYSFR_t phyPHYSFR;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Get the OUI information of the Ethernet port */
        CHK_FUN_CONTINUE(ret, rtk_phy_phyid_get(uintChip, &phyIdInfo));
        /* Expected Behavior:
         * The expected OUI is Realtek OUI
         */
        if(OUI_REALTEK != phyIdInfo.oui)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 2: Get the current PCS status of the Ethernet port's PHY */
        CHK_FUN_CONTINUE(ret, rtk_ethphy_pcs_info_cfg(uintChip, &phyPCSFlag));
        /* Expected Behavior:
         * If the Ethernet port is linked up with 1000BASE-T, the PCS state value will be 0x37
         * If the Ethernet port is linked up with 100BASE-TX, the PCS state value will be 0x44
         */

        /* Step 3: Get the PHY status sub-flag (PHYSFR) */
        CHK_FUN_CONTINUE(ret, rtk_phy_physfr_info_cfg(uintChip, &phyPHYSFR));
        /* Expected Behavior:
         * The PHYSFR register provides the PHY current status. If the PHY is linked up, the flag will be 0x3
         */
        if(0x3u != phyPHYSFR.phySubFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}
/**@}*/ /* RTL907XD_VA_ETHPHY_STATUS_DUMP_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_ETHPHY_PWR_SAVE_EXAMPLE Ethernet-PHY Configure Power Saving Mode Example
 * @brief <b>Configuring Ethernet-PHY Power Saving Mode</b>
 *
 * @section Purpose
 * Demonstrates how to configure the power save mode for the Ethernet-PHY using the SDK API.
 *
 * @section Description
 * The functions cover the following aspects:
 * - Enable or disable power save mode for the Ethernet port.
 * - Verifying the power save mode setting.
 *
 * @{
 */

/**
 * @brief Configure the power save mode for the Ethernet port.
 *
 * @details
 * This function uses the SDK API to configure the power save mode for the Ethernet port.
 *
 * @return RtkApiRet Returns RT_ERR_OK on success, or an error code on failure.
 *
 * @section Purpose
 * This function demonstrates how to enable or disable the power save mode for the Ethernet port using the SDK API.
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Configure power saving mode for the Ethernet port accord to the parameter enPwrSaving.
 * - Step 2: Verify the power saving mode setting of the Ethernet port.
 */
RtkApiRet dva_set_ethphy_pwr_save_mode(RtkEnable enPwrSaving)
{
    RtkApiRet    ret = RT_ERR_OK;
    UnitChip_t   uintChip;
    PhyPwrSave_t pwrSaveSet;
    PhyPwrSave_t pwrSaveGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Configure power saving mode for the Ethernet port */
        pwrSaveSet.enPwrSaving = enPwrSaving;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_power_save_en_set(uintChip, &pwrSaveSet));

        /* Step 2: Get the power saving mode setting of the Ethernet port */
        pwrSaveGet.enPwrSaving = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_power_save_en_get(uintChip, &pwrSaveGet));

        /* Expected Behavior:
         * Enable or disable the power saving mode of the Ethernet port according to the parameter
         */
        if(enPwrSaving != pwrSaveGet.enPwrSaving)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}

/**@}*/ /* RTL907XD_VA_ETHPHY_PWR_SAVE_EXAMPLE */

/**
 * @defgroup RTL907XD_VA_ETHPHY_MDI_CFG_EXAMPLE Ethernet-PHY Configure MDI/MDIX Example
 * @brief <b>Configuring Ethernet-PHY MDI/MDIX Function</b>
 *
 * @section Purpose
 * Demonstrates how to configure the Ethernet MDI/MDIX setting for the Ethernet-PHY using the SDK API.
 *
 * @section Description
 * The functions cover the following aspects:
 * - Setting the MDI/MDIX mode for the Ethernet port.
 * - Verifying the MDI/MDIX mode setting.
 * @{
 */

/**
 * @brief Configure the MDI/MDIX mode for the Ethernet port.
 *
 * @details
 * This function uses the SDK API to set the MDI/MDIX mode for the Ethernet port.
 *
 * @section Purpose
 * This function demonstrates how to configure the MDI/MDIX mode for the Ethernet port using the SDK API.
 * Valid MDI/MDIX mode are PHY_AUTO_MDIX_MODE(0), PHY_FORCE_MDIX_MODE(2) and PHY_FORCE_MDI_MODE(3).
 *
 * @section Description
 * The function performs the following steps:
 * - Step 1: Set the MDI/MDIX mode for the Ethernet port.
 * - Step 2: Retrieve the MDI/MDIX mode of the Ethernet port to verify it has been set correctly.
 * - Step 3: Check if the MDI/MDIX mode configuration is correct. If not, return an error.
 */
RtkApiRet dva_set_ethphy_mdix_mode(uint8 mode)
{
    RtkApiRet    ret = RT_ERR_OK;
    UnitChip_t   uintChip;
    PhyMdixCfg_t modeSet;
    PhyMdixCfg_t modeGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1: Set the MDI/MDIX mode for the Ethernet port */
        modeSet.mdixMode = mode;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mdix_mode_set(uintChip, &modeSet));

        /* Step 2: Get the MDI/MDIX mode of the Ethernet port */
        modeGet.mdixMode = 0xFu;
        CHK_FUN_CONTINUE(ret, rtk_ethphy_mdix_mode_get(uintChip, &modeGet));

        /* Expected Behavior:
         * Check if the MDI/MDIX mode is set correctly
         */
        if(mode != modeGet.mdixMode)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(FALSE);

    return ret;
}
/**@}*/ /* RTL907XD_VA_ETHPHY_MDI_CFG_EXAMPLE */

/**@}*/ /* PHY_EXAMPLE */
/**@}*/ /* EXAMPLE */
