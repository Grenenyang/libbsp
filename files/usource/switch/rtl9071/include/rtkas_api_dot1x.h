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

#ifndef RTKAS_API_DOT1X_H__
#define RTKAS_API_DOT1X_H__
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_api_vlan.h"
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_dot1x.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_dot1x.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_dot1x.h"
#endif
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint16 dot1x_timer;
/******************************************************************************/
/* Struct                                                                     */
/******************************************************************************/
/**
 * @brief This structure represents the port-based DOT1X enabled port mask.
 *
 * This structure contains the enabled port mask for port-based DOT1X.
 */
typedef struct
{
    RtkPmap portMask; /**< [in,out] The enabled port mask for Port-based DOT1X (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL) */
} Dot1xEnPortMaskInfo_t;

/**
 * @brief This structure represents the secret key information for port-based DOT1X .
 *
 * This structure contains the switch port number, secret key length, and the secret key for port-based DOT1X.
 */
typedef struct
{
    RtkPort port;            /**< [in] Port number(<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  secretKeyLength; /**< [in] Secret key length. Excluding the string terminator, the maximum string length can be set to <CHIPID>_DOT1X_SECRET_KEY_LENGTH - 1u.
                                       When setting the secret key, the range is 0 to <CHIPID>_DOT1X_SECRET_KEY_LENGTH-1u, when getting the secret key,the range must be equal to <CHIPID>_DOT1X_SECRET_KEY_LENGTH-1u), */
    uint8 *pSecretKey;       /**< [in,out] Secret key string (N/A) */
} Dot1xSecretKeyInfo_t;

/**
 * @brief This structure represents the DOT1X switch IP address information.
 *
 * This structure contains the switch IP address information for DOT1X.
 */
typedef struct
{
    uint8 pIpAddr[IPV4_ADDR_LEN]; /**< [in,out] Switch IP address (0 to 255 for 4 elements) */
} Dot1xSwitchIpInfo_t;

/**
 * @brief This structure represents the RADIUS IP address information.
 *
 * This structure contains the RADIUS IP address information for DOT1X.
 */
typedef struct
{
    uint8 pIpAddr[IPV4_ADDR_LEN]; /**< [in,out] RADIUS IP address (0 to 255 for 4 elements) */
} Dot1xRadiusIpInfo_t;

/**
 * @brief This structure represents the RADIUS UDP port information.
 *
 * This structure contains the RADIUS UDP port number information for DOT1X.
 */
typedef struct
{
    uint16 portNum; /**< [in,out] UDP port number (0 to 0xFFFFu) */
} Dot1xUdpPortInfo_t;

/**
 * @brief This structure represents the port mask connected to the RADIUS server.
 *
 * This structure contains the RADIUS server port mask configuration.
 */
typedef struct
{
    RtkPmap portMask; /**< [in,out] RADIUS server port mask (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL) */
} Dot1xRadiusPMaskInfo_t;

/**
 * @brief This structure represents the method for searching the RADIUS server.
 *
 * This structure contains the method information used for searching the RADIUS server.
 */
typedef struct
{
    uint32 status; /**< [in,out] The method of searching for the RADIUS server (<CHIPID>_ARP_TO_ALLPORT to <CHIPID>_ARP_TO_DOT1X_CONFIG) */
} Dot1xArpMethodInfo_t;

/**
 * @brief This structure represents the enable status of the aging function.
 *
 * This structure contains the enable status information for the aging function.
 */
typedef struct
{
    uint32 status; /**< [in,out] Enable status of the aging function: 0 means disabled, 1 means enabled (0 to 1) */
} Dot1xAgeStatusInfo_t;

/**
 * @brief This structure represents the time value for the aging function.
 *
 * This structure contains the time duration information used by the aging function to determine
 * when port authorization status should expire.
 */
typedef struct
{
    dot1x_timer agingTime; /**< [in,out] The value of the aging function, in seconds.(0 to <CHIPID>_DOT1X_AGING_OUT_TIME_MAX) */
} Dot1xAgeOutValInfo_t;

/**
 * @brief This structure represents the enable status of the force unauthorization function.
 *
 * This structure contains the enable status of the force unauthorization function.
 */
typedef struct
{
    uint32 status; /**< [in,out] Enable status of the force unauthorization function: 0 means disabled, 1 means enabled (0 to 1) */
} Dot1xForceStatusInfo_t;

/**
 * @brief This structure represents the action for a specified unauthorized packet.
 *
 * This structure contains the unauthorized packet type and the corresponding action.
 */
typedef struct
{
    uint32 pktType;      /**< [in] The type of packet (<CHIPID>_DOT1X_PKT_INTAG to <CHIPID>_DOT1X_PKT_END) */
    uint32 unauthAction; /**< [in,out] Indicate unauthorized packet action (<CHIPID>_DOT1X_ACTION_DROP to <CHIPID>_DOT1X_ACTION_END) */
} Dot1xUnauthPktActionInfo_t;

/**
 * @brief This structure represents the switch port authorization status.
 *
 * This structure contains the port number and the authorization status of the switch port.
 */
typedef struct
{
    RtkPort port;     /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  portAuth; /**< [out] Authentication status (<CHIPID>_VA_DOT1X_UNAUTH to <CHIPID>_VA_DOT1X_AUTH_STATUS_END) */
} Dot1xGetAuthStatus_t;

/**
 * @brief This structure represents the DOT1X guest VLAN entry information.
 *
 * This structure contains the VLAN ID and the associated VLAN entry information.
 */
typedef struct
{
    RtkVlan     vid;        /**< [in,out] VLAN ID (0 to 4095) */
    VlanEntry_t pVlanEntry; /**< [in] VLAN entry (N/A) */
} Dot1xGuestVidEntryInfo_t;

/**
 * @brief This structure represents the DOT1X CPU port mask.
 *
 * This structure contains the DOT1X CPU port mask.
 */
typedef struct
{
    RtkPmap cpuPortMask; /**< [in,out] CPU port mask (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL) */
} Dot1xCpuPMaskInfo_t;

/**
 * @brief This structure represents the configuration for port-based DOT1X control direction.
 *
 * This structure contains the switch port number and associated control direction information of the switch port.
 */
typedef struct
{
    RtkPort port;          /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  portDirection; /**< [in,out] Control direction (<CHIPID>_DOT1X_BOTH to <CHIPID>_DOT1X_DIRECTION_END) */
} Dot1xDirInfo_t;
/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_dot1x_get_enable_portmask(UnitChip_t unitChip, Dot1xEnPortMaskInfo_t *dot1xEnPortMaskInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_enable_portmask(UnitChip_t unitChip, Dot1xEnPortMaskInfo_t *dot1xEnPortMaskInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_secret_key(UnitChip_t unitChip, Dot1xSecretKeyInfo_t *dot1xSecretKeyInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_secret_key(UnitChip_t unitChip, Dot1xSecretKeyInfo_t *dot1xSecretKeyInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_switch_ip(UnitChip_t unitChip, Dot1xSwitchIpInfo_t *dot1xSwitchIpInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_switch_ip(UnitChip_t unitChip, Dot1xSwitchIpInfo_t *dot1xSwitchIpInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_radius_server_ip(UnitChip_t unitChip, Dot1xRadiusIpInfo_t *dot1xRadiusIpInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_radius_server_ip(UnitChip_t unitChip, Dot1xRadiusIpInfo_t *dot1xRadiusIpInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_radius_udp_port(UnitChip_t unitChip, Dot1xUdpPortInfo_t *dot1xUdpPortInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_radius_udp_port(UnitChip_t unitChip, Dot1xUdpPortInfo_t *dot1xUdpPortInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_radius_server_portmask(UnitChip_t unitChip, Dot1xRadiusPMaskInfo_t *dot1xradiusPMaskInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_radius_server_portmask(UnitChip_t unitChip, Dot1xRadiusPMaskInfo_t *dot1xradiusPMaskInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_arp_port_select(UnitChip_t unitChip, Dot1xArpMethodInfo_t *dot1xArpMethodInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_arp_port_select(UnitChip_t unitChip, Dot1xArpMethodInfo_t *dot1xArpMethodInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_dynamic_reset_flag(UnitChip_t unitChip);

EXTERN_API RtkApiRet rtk_dot1x_get_aging_time_enable(UnitChip_t unitChip, Dot1xAgeStatusInfo_t *dot1xAgeStatusInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_aging_time_enable(UnitChip_t unitChip, Dot1xAgeStatusInfo_t *dot1xAgeStatusInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_aging_time(UnitChip_t unitChip, Dot1xAgeOutValInfo_t *dot1xAgeOutValInfo);
EXTERN_API RtkApiRet rtk_dot1x_set_aging_time(UnitChip_t unitChip, Dot1xAgeOutValInfo_t *dot1xAgeOutValInfo);

EXTERN_API RtkApiRet rtk_dot1x_set_force_unauth_enable(UnitChip_t unitChip, Dot1xForceStatusInfo_t *dot1xForceStatusInfo);
EXTERN_API RtkApiRet rtk_dot1x_get_force_unauth_enable(UnitChip_t unitChip, Dot1xForceStatusInfo_t *dot1xForceStatusInfo);

EXTERN_API RtkApiRet rtk_dot1x_hw_reset(UnitChip_t unitChip);

EXTERN_API RtkApiRet rtk_dot1x_unauth_packet_oper_set(UnitChip_t unitChip, Dot1xUnauthPktActionInfo_t *dot1xUnauthInfo);
EXTERN_API RtkApiRet rtk_dot1x_unauth_packet_oper_get(UnitChip_t unitChip, Dot1xUnauthPktActionInfo_t *dot1xUnauthInfo);
EXTERN_API RtkApiRet rtk_dot1x_portbased_auth_get(UnitChip_t unitChip, Dot1xGetAuthStatus_t *dot1xAuthStatusInfo);
EXTERN_API RtkApiRet rtk_dot1x_guest_vlan_entry_set(UnitChip_t unitChip, Dot1xGuestVidEntryInfo_t *dot1xGuestVidEntryInfo);
EXTERN_API RtkApiRet rtk_dot1x_guest_vlan_entry_get(UnitChip_t unitChip, Dot1xGuestVidEntryInfo_t *dot1xGuestVidEntryInfo);
EXTERN_API RtkApiRet rtk_dot1x_cpuportmask_get(UnitChip_t unitChip, Dot1xCpuPMaskInfo_t *dot1xCpuPMaskInfo);
EXTERN_API RtkApiRet rtk_dot1x_cpuportmask_set(UnitChip_t unitChip, Dot1xCpuPMaskInfo_t *dot1xCpuPMaskInfo);

EXTERN_API RtkApiRet rtk_dot1x_portbased_direction_set(UnitChip_t unitChip, Dot1xDirInfo_t *dot1xDirInfo);
EXTERN_API RtkApiRet rtk_dot1x_portbased_direction_get(UnitChip_t unitChip, Dot1xDirInfo_t *dot1xDirInfo);
#endif /* RTKAS_API_DOT1X_H__ */