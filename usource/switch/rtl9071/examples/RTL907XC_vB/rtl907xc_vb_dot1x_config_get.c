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
 * @defgroup RTL907XCVB_DOT1X_CONFIG_GET_EXAMPLE Dot1x Get Configuration Example
 *
 * @brief <b>Get DOT1X all configuration.</b>
 *
 * @section Purpose
 *          This example demonstrates how to get port-based DOT1X configuration and authorization status.
 * @section Description
 *          Get all DOT1X configuration and authorization status.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_dot1x_config_get(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_dot1x_config_get(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};

    uint8                  pSecretKey[CVB_DOT1X_SECRET_KEY_LENGTH];
    Dot1xEnPortMaskInfo_t  dot1xEnPortMaskInfo  = {0};
    Dot1xSwitchIpInfo_t    dot1xSwitchIpInfo    = {0};
    Dot1xRadiusIpInfo_t    dot1xRadiusIpInfo    = {0};
    Dot1xRadiusPMaskInfo_t dot1xRadiusPMaskInfo = {0};
    Dot1xUdpPortInfo_t     dot1xUdpPortInfo     = {0};
    Dot1xAgeStatusInfo_t   dot1xAgeStatusInfo   = {0};
    Dot1xAgeOutValInfo_t   dot1xAgeOutValInfo   = {0};
    Dot1xForceStatusInfo_t dot1xForceStatusInfo = {0};
    Dot1xArpMethodInfo_t   dot1xArpMethodInfo   = {0};
    Dot1xCpuPMaskInfo_t    dot1xCpuPMaskInfo    = {0};
    Dot1xSecretKeyInfo_t   dot1xSecretKeyInfo   = {
            .port            = CVB_RTK_USER_PORT_1ST,
            .secretKeyLength = CVB_DOT1X_SECRET_KEY_LENGTH,
            .pSecretKey      = pSecretKey,
    };
    Dot1xUnauthPktActionInfo_t dot1xUnauthPktActionInfo = {
        .pktType = CVB_DOT1X_PKT_OUTTAG,
    };
    Dot1xDirInfo_t dot1xDirInfo = {
        .port = CVB_RTK_USER_PORT_1ST,
    };
    Dot1xGuestVidEntryInfo_t dot1xGuestVidEntryInfo;
    (void)drv_memset(&dot1xGuestVidEntryInfo, 0, sizeof(Dot1xGuestVidEntryInfo_t));
    Dot1xGetAuthStatus_t dot1xGetAuthStatusInfo = {
        .port = CVB_RTK_USER_PORT_1ST,
    };
    /* Step 1: Get enabled port mask */
    CHK_FUN_RET(retVal, rtk_dot1x_get_enable_portmask(unitChip, &dot1xEnPortMaskInfo));
    /* Step 2: Get secret key */
    CHK_FUN_RET(retVal, rtk_dot1x_get_secret_key(unitChip, &dot1xSecretKeyInfo));
    /* Step 3: Get switch IP address */
    CHK_FUN_RET(retVal, rtk_dot1x_get_switch_ip(unitChip, &dot1xSwitchIpInfo));
    /* Step 4: Get RADIUS server IP address */
    CHK_FUN_RET(retVal, rtk_dot1x_get_radius_server_ip(unitChip, &dot1xRadiusIpInfo));
    /* Step 5: Get RADIUS server port mask */
    CHK_FUN_RET(retVal, rtk_dot1x_get_radius_server_portmask(unitChip, &dot1xRadiusPMaskInfo));
    /* Step 6: Get RADIUS UDP port */
    CHK_FUN_RET(retVal, rtk_dot1x_get_radius_udp_port(unitChip, &dot1xUdpPortInfo));
    /* Step 7: Get aging time enable status */
    CHK_FUN_RET(retVal, rtk_dot1x_get_aging_time_enable(unitChip, &dot1xAgeStatusInfo));
    /* Step 8: Get aging time */
    CHK_FUN_RET(retVal, rtk_dot1x_get_aging_time(unitChip, &dot1xAgeOutValInfo));
    /* Step 9: Get force unauth enable status */
    CHK_FUN_RET(retVal, rtk_dot1x_get_force_unauth_enable(unitChip, &dot1xForceStatusInfo));
    /* Step 10: Get method of searching RADIUS server */
    CHK_FUN_RET(retVal, rtk_dot1x_get_arp_port_select(unitChip, &dot1xArpMethodInfo));
    /* Step 11: Get unauth packet action */
    CHK_FUN_RET(retVal, rtk_dot1x_unauth_packet_oper_get(unitChip, &dot1xUnauthPktActionInfo));
    /* Step 12: Get port-based control direction */
    CHK_FUN_RET(retVal, rtk_dot1x_portbased_direction_get(unitChip, &dot1xDirInfo));
    /* Step 13: Get guest VLAN entry */
    CHK_FUN_RET(retVal, rtk_dot1x_guest_vlan_entry_get(unitChip, &dot1xGuestVidEntryInfo));
    /* Step 14: Get CPU port mask */
    CHK_FUN_RET(retVal, rtk_dot1x_cpuportmask_get(unitChip, &dot1xCpuPMaskInfo));
    /* Step 15: Get port-based authorization status */
    CHK_FUN_RET(retVal, rtk_dot1x_portbased_auth_get(unitChip, &dot1xGetAuthStatusInfo));

    /* Expected Behavior: Get all dot1x configuration and authorization status */
    return retVal;
}
/**@}*/ /* RTL907XCVB_DOT1X_CONFIG_GET_EXAMPLE */
/**@}*/ /* RTL907XCVB_DOT1X_EXAMPLE */
/**@}*/ /* DOT1X_EXAMPLE */
