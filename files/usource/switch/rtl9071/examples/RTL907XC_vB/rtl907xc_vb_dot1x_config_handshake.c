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
#include "rtkas_api_dot1x.h"

/**
 * @addtogroup DOT1X_EXAMPLE DOT1X Example
 * @{
 */
/**
 * @addtogroup RTL907XCVB_DOT1X_EXAMPLE RTL907XC_VB DOT1X Example
 * @{
 */

/**
 * @defgroup RTL907XCVB_DOT1X_CONFIG_HANDSHAKE_EXAMPLE Dot1x Config Handshake Information Example
 *
 * @brief <b>Configure DOT1X protocol.</b> \n
 *           Switch will execute the authentication flow after configuring secret key, switch IP address, RADIUS server IP address, \n
 *           RADIUS server UDP port, the method for searching the RADIUS server, RADIUS server port mask and DOT1X enabled port mask.
 *
 * @section Purpose
 *          This example demonstrates how to configure port-based DOT1X handshake information.
 * @section Description
 *          The switch will handshake with the server and execute the authentication flow.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_dot1x_config_handshake(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_dot1x_config_handshake(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};

    uint8                 pSecretKey[5]       = "test";
    Dot1xEnPortMaskInfo_t dot1xEnPortMaskInfo = {
        .portMask = CVB_RTK_USER_PMAP_1ST,
    };
    Dot1xSecretKeyInfo_t dot1xSecretKeyInfo = {
        .port            = CVB_RTK_USER_PORT_1ST,
        .secretKeyLength = 5u,
        .pSecretKey      = pSecretKey,
    };
    Dot1xSwitchIpInfo_t dot1xSwitchIpInfo = {
        .pIpAddr = {192, 168, 1, 4},
    };
    Dot1xRadiusIpInfo_t dot1xRadiusIpInfo = {
        .pIpAddr = {192, 168, 1, 11},
    };
    Dot1xArpMethodInfo_t dot1xArpMethodInfo = {
        .status = CVB_ARP_TO_RADIUS_CONFIG,
    };
    Dot1xRadiusPMaskInfo_t dot1xRadiusPMaskInfo = {
        .portMask = 0xFFEu,
    };
    Dot1xUdpPortInfo_t dot1xUdpPortInfo = {
        .portNum = 0x5678u,
    };

    /* Step 1: Set secret key */
    CHK_FUN_RET(retVal, rtk_dot1x_set_secret_key(unitChip, &dot1xSecretKeyInfo));
    /* Step 2: Set switch IP address */
    CHK_FUN_RET(retVal, rtk_dot1x_set_switch_ip(unitChip, &dot1xSwitchIpInfo));
    /* Step 3: Set RADIUS server IP address */
    CHK_FUN_RET(retVal, rtk_dot1x_set_radius_server_ip(unitChip, &dot1xRadiusIpInfo));
    /* Step 4: Set the method for searching the RADIUS server */
    CHK_FUN_RET(retVal, rtk_dot1x_set_arp_port_select(unitChip, &dot1xArpMethodInfo));
    /* Step 5: Set RADIUS server port mask */
    CHK_FUN_RET(retVal, rtk_dot1x_set_radius_server_portmask(unitChip, &dot1xRadiusPMaskInfo));
    /* Step 6: Set UDP port number */
    CHK_FUN_RET(retVal, rtk_dot1x_set_radius_udp_port(unitChip, &dot1xUdpPortInfo));
    /* Step 7: Enable DOT1X */
    CHK_FUN_RET(retVal, rtk_dot1x_set_enable_portmask(unitChip, &dot1xEnPortMaskInfo));
    /* Step 8: Trigger the dynamic setting flag to set the new configuration to the switch */
    CHK_FUN_RET(retVal, rtk_dot1x_set_dynamic_reset_flag(unitChip));
    /* Expected Behavior: The switch will handshake with the server and execute the authentication flow */
    return retVal;
}
/**@}*/ /* RTL907XCVB_DOT1X_CONFIG_HANDSHAKE_EXAMPLE */
/**@}*/ /* RTL907XCVB_DOT1X_EXAMPLE */
/**@}*/ /* DOT1X_EXAMPLE */
