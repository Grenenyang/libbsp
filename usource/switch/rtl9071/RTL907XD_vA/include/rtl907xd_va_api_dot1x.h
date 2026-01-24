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

#ifndef DVA_RTK_API_DOT1X_H__
#define DVA_RTK_API_DOT1X_H__

/********* Include Header *********/
#include "rtl907xd_va_api_vlan.h"
#include "rtl907xd_va_asicdrv_vlan.h"
/********* Definition *********/
#define DVA_RTK_USER_CPU_PMAP_ALL (0x9FFEU)

#define DVA_ARP_TO_ALLPORT       (0u)
#define DVA_ARP_TO_RADIUS_CONFIG (1u)
#define DVA_ARP_TO_DOT1X_CONFIG  (2u)

#define DVA_DOT1X_SECRET_KEY_LENGTH     (32u)
#define DVA_DOT1X_P1_SECRET_KEY_OFFSET  (0x01107F70U)
#define DVA_DOT1X_P2_SECRET_KEY_OFFSET  (0x01107F90U)
#define DVA_DOT1X_P3_SECRET_KEY_OFFSET  (0x01107FB0U)
#define DVA_DOT1X_P4_SECRET_KEY_OFFSET  (0x01107FD0U)
#define DVA_DOT1X_P5_SECRET_KEY_OFFSET  (0x01107FF0U)
#define DVA_DOT1X_P6_SECRET_KEY_OFFSET  (0x01108010U)
#define DVA_DOT1X_P7_SECRET_KEY_OFFSET  (0x01108030U)
#define DVA_DOT1X_P8_SECRET_KEY_OFFSET  (0x01108050U)
#define DVA_DOT1X_P9_SECRET_KEY_OFFSET  (0x01108070U)
#define DVA_DOT1X_P10_SECRET_KEY_OFFSET (0x01108090U)
#define DVA_DOT1X_P11_SECRET_KEY_OFFSET (0x011080B0U)
#define DVA_DOT1X_P12_SECRET_KEY_OFFSET (0x011080D0U)

#define DVA_DOT1X_SWITCH_IP_OFFSET          (0x011080F0U)
#define DVA_DOT1X_RADIUS_SERVER_IP_OFFSET   (0x011080F4U)
#define DVA_DOT1X_ENABLE_PORTMASK_OFFSET    (0x011080F8U)
#define DVA_DOT1X_RADIUS_UDP_PORT_OFFSET    (0x011080FAU)
#define DVA_DOT1X_AGING_ENABLE_OFFSET       (0x01108107U)
#define DVA_DOT1X_AGING_TIME_OFFSET         (0x01108108U)
#define DVA_DOT1X_FORCE_UNAUTH_OFFSET       (0x0110810AU)
#define DVA_DOT1X_DYNAMIC_RESET_OFFSET      (0x0110810BU)
#define DVA_DOT1X_RADIUS_SERVER_PORT_OFFSET (0x0110810CU)
#define DVA_DOT1X_SET_ARP_PORT_SELECT       (0x0110810EU)

/*****for agingout timer range*****/
#define DVA_DOT1X_AGING_OUT_TIME_MAX (0xFFFFU)

/********* Enumeration *********/

#define DVA_DOT1X_PKT_INTAG    (0u) /**< Inner tag packet */
#define DVA_DOT1X_PKT_INUNTAG  (1u) /**< Inner untag packet */
#define DVA_DOT1X_PKT_OUTTAG   (2u) /**< Outer tag packet */
#define DVA_DOT1X_PKT_OUTUNTAG (3u) /**< Outer untag packet */
#define DVA_DOT1X_PKT_END      (DVA_DOT1X_PKT_OUTUNTAG)

#define DVA_DOT1X_ACTION_DROP      (0u) /**< The unauthorized packet action is drop */
#define DVA_DOT1X_ACTION_TRAP2CPU  (1u) /**< The unauthorized packet action is trop2cpu */
#define DVA_DOT1X_ACTION_GUESTVLAN (2u) /**< The unauthorized packet action is toguestvlan */
#define DVA_DOT1X_ACTION_END       (DVA_DOT1X_ACTION_GUESTVLAN)

#define DVA_DOT1X_UNAUTH          (0u) /**< The status of 802.1x port is unauthorized */
#define DVA_DOT1X_AUTH            (1u) /**< The status of 802.1x port is authorized */
#define DVA_DOT1X_AUTH_STATUS_END (DVA_DOT1X_AUTH)

#define DVA_DOT1X_BOTH          (0u) /**< Operate controlled direction of 802.1x is BOTH */
#define DVA_DOT1X_DIR_IN        (1u) /**< Operate controlled direction of 802.1x is IN */
#define DVA_DOT1X_DIRECTION_END (DVA_DOT1X_DIR_IN)

EXTERN_API RtkApiRet dva_dot1x_get_enable_portmask(uint32 unit, RtkPmap *pPortMask);
EXTERN_API RtkApiRet dva_dot1x_set_enable_portmask(uint32 unit, RtkPmap portMask);
EXTERN_API RtkApiRet dva_dot1x_get_secret_key(uint32 unit, RtkPort port, uint32 secretKeyBufSize, uint8 *pSecretKey);
EXTERN_API RtkApiRet dva_dot1x_set_secret_key(uint32 unit, RtkPort port, uint32 secretKeyLength, const uint8 *pSecretKey);
EXTERN_API RtkApiRet dva_dot1x_get_switch_ip(uint32 unit, uint8 pIpAddr[DVA_IPV4_ADDR_LEN]);
EXTERN_API RtkApiRet dva_dot1x_set_switch_ip(uint32 unit, const uint8 pIpAddr[DVA_IPV4_ADDR_LEN]);
EXTERN_API RtkApiRet dva_dot1x_get_radius_server_ip(uint32 unit, uint8 pIpAddr[DVA_IPV4_ADDR_LEN]);
EXTERN_API RtkApiRet dva_dot1x_set_radius_server_ip(uint32 unit, const uint8 pIpAddr[IPV4_ADDR_LEN]);
EXTERN_API RtkApiRet dva_dot1x_get_radius_server_portmask(uint32 unit, RtkPmap *pPortMask);
EXTERN_API RtkApiRet dva_dot1x_set_radius_server_portmask(uint32 unit, RtkPmap portMask);
EXTERN_API RtkApiRet dva_dot1x_get_radius_udp_port(uint32 unit, uint16 *pPortNum);
EXTERN_API RtkApiRet dva_dot1x_set_radius_udp_port(uint32 unit, uint16 portNum);
EXTERN_API RtkApiRet dva_dot1x_get_aging_time_enable(uint32 unit, uint32 *pStatus);
EXTERN_API RtkApiRet dva_dot1x_set_aging_time_enable(uint32 unit, uint32 status);
EXTERN_API RtkApiRet dva_dot1x_get_aging_time(uint32 unit, uint16 *pAgingTime);
EXTERN_API RtkApiRet dva_dot1x_set_aging_time(uint32 unit, uint16 agingTime);
EXTERN_API RtkApiRet dva_dot1x_get_force_unauth_enable(uint32 unit, uint32 *pStatus);
EXTERN_API RtkApiRet dva_dot1x_set_force_unauth_enable(uint32 unit, uint32 status);
EXTERN_API RtkApiRet dva_dot1x_set_dynamic_reset_flag(uint32 unit);

EXTERN_API RtkApiRet dva_dot1x_hw_reset(uint32 unit);
EXTERN_API RtkApiRet dva_dot1x_unauth_packet_oper_set(uint32 unit, uint32 pktType, uint32 unauthAction);
EXTERN_API RtkApiRet dva_dot1x_unauth_packet_oper_get(uint32 unit, uint32 pktType, uint32 *pUnauthAction);
EXTERN_API RtkApiRet dva_dot1x_portbased_auth_get(uint32 unit, RtkPort port, uint32 *pPortAuth);
EXTERN_API RtkApiRet dva_dot1x_portbased_direction_set(uint32 unit, RtkPort port, uint32 portDirection);
EXTERN_API RtkApiRet dva_dot1x_portbased_direction_get(uint32 unit, RtkPort port, uint32 *pPortDirection);
EXTERN_API RtkApiRet dva_dot1x_guest_vlan_entry_set(uint32 unit, RtkVlan vid, VlanEntry_t *pVlanEntry);
EXTERN_API RtkApiRet dva_dot1x_guest_vlan_entry_get(uint32 unit, RtkVlan *pVid, VlanEntry_t *pVlanEntry);
EXTERN_API RtkApiRet dva_dot1x_set_arp_port_select(uint32 unit, uint32 status);
EXTERN_API RtkApiRet dva_dot1x_get_arp_port_select(uint32 unit, uint32 *pStatus);
EXTERN_API RtkApiRet dva_dot1x_cpuportmask_set(uint32 unit, RtkPmap cpuPort);
EXTERN_API RtkApiRet dva_dot1x_cpuportmask_get(uint32 unit, RtkPmap *cpuPort);
#endif /* DVA_RTK_API_DOT1X_H__ */
