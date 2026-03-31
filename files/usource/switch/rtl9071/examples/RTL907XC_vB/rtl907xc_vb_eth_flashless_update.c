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

#include "rtkas_common_types.h"
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtkas_api_ethernet_access.h"
#include "rtkas_api_sys_boot.h"

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_eth_flashless_update_flow(UnitChip_t unitChip);
RtkApiRet cvb_eth_flashless_disable_port_flow(UnitChip_t unitChip);
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup FLASHLESS Flashless Example
 * @{
 */

/**
 * @addtogroup RTL907C_VB_FLASHLESS_EXAMPLE RTL907XC_VB Flashless Example
 * @{
 */
/**
 * @addtogroup RTL907C_VB_FLASHLESS_ETH_EXAMPLE Flashless Eth Update Example
 * @brief <b>Flashless Ethernet Access update flow</b>
 *
 * @section Purpose
 *   External update firmware and configuration.\n\n
 *
 * @section Description
 *   This example demonstrates how to update the firmware and configuration via the ethernet access in flashless mode.
 *
 * @image html flashless/eth_flashless.jpg
 *
 * @{
 */

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_eth_flashless_update_flow(UnitChip_t unitChip)
{
    const int8       *fileName   = "image/RTL907XC_vB/flashless/drvTest_flashless_testcase1.bin";
    RtkApiRet         ret        = RT_ERR_OK;
    RtkPort           configPort = CVB_RTK_USER_PORT_7TH;
    SysCheckFl_t      sysCheckFl;
    SysEthPortRxDis_t sysEthPortRxDis;
    SysEthUpdateFw_t  sysEthUpdateFw;
    SysEthUpdateCfg_t sysEthUpdateCfg;

    do
    {
        /* Step 1. Etheraccess flashless update only supports unicast. Please make sure DMAC is set correctly */
        /* Step 2. Link partner uses FE to connect with port7 FE of switch*/
        /* Step 3. Check flashless mode. If wanting to bypass eth link checking, set byPassEth to 1*/
        sysCheckFl.byPassEth = 1u;
        CHK_FUN_CONTINUE(ret, rtk_sys_check_flashless_mode(unitChip, &sysCheckFl));

        /* Step 4. Set port7 as configure port, and disable other port rx_en */
        sysEthPortRxDis.userPort = configPort;
        CHK_FUN_CONTINUE(ret, rtk_sys_eth_disable_non_configure_port_rx(unitChip, &sysEthPortRxDis));

        /* Step 5. Update firmware and configuration
         * At this stage, user can manually configure switch or use rtk_sys_eth_update_config to
         * load config after firmware is updated.
         */
        /* Update firmware */
        sysEthUpdateFw.fileName = fileName;
        CHK_FUN_CONTINUE(ret, rtk_sys_eth_update_firmware(unitChip, &sysEthUpdateFw));
        /* Update config*/
        sysEthUpdateCfg.userPort = configPort;
        sysEthUpdateCfg.fileName = fileName;
        CHK_FUN_CONTINUE(ret, rtk_sys_eth_update_config(unitChip, &sysEthUpdateCfg));
        /* Step 6. Leave flashless*/
        CHK_FUN_CONTINUE(ret, rtk_sys_leave_flashless(unitChip));
        /* Step 7. Check leave flashless is successful*/
        CHK_FUN_CONTINUE(ret, rtk_sys_check_leave_flashless(unitChip));
    } while(0u == 1u);

    /* Expected Behaviour:
     *   Check the reg 0x4A1B004C is the same as image version.
     *   The check function return value "ret" must return RT_ERR_OK.
     */

    return ret;
}

/**@}*/ /* RTL907C_VB_FLASHLESS_ETH_EXAMPLE */

/**
 * @addtogroup RTL907C_VB_FLASHLESS_ETH_PORT_DIS_EXAMPLE Flashless Eth Disable Port Example
 * @brief <b>Disable the port for Ethernet Access Flashless</b>
 *
 * @section Purpose
 *   Disable the port to load configuration.
 *
 * @section Description
 *   This example shows how to disable a specific port for ethernet access flashless update.
 *
 * @image html flashless/disable_port.jpg
 *
 * @{
 */
RtkApiRet cvb_eth_flashless_disable_port_flow(UnitChip_t unitChip)
{
    RtkApiRet          ret = RT_ERR_OK;
    SysEthPortOtpDis_t sysEthPortOtpDis;

    do
    {
        /* Step 1. Select the port to disable*/
        sysEthPortOtpDis.userPort = CVB_RTK_USER_PORT_6TH;

        /* Step 2. Disable port by OTP*/
        CHK_FUN_RET(ret, rtk_sys_flashless_eth_port_otp_disable(unitChip, &sysEthPortOtpDis));
    } while(0u == 1u);

    /* Expected Behaviour:
     *   Flashless update via port 6 is not allowed.
     *   The check function return value "ret" must return RT_ERR_OK.
     */

    return ret;
}
/**@}*/ /* RTL907C_VB_FLASHLESS_ETH_PORT_DIS_EXAMPLE */
/**@}*/ /* RTL907C_VB_FLASHLESS_EXAMPLE */
/**@}*/ /* FLASHLESS */
/**@}*/ /* EXAMPLE */
