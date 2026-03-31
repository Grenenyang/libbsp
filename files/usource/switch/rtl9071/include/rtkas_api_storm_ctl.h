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

#ifndef RTK_API_STORM_CTL_H__
#define RTK_API_STORM_CTL_H__

#define STORM_MAX_RATE  (0xFFFFFFu)
#define MC_TYPE_UNKNOWN (0x0u)
#define MC_TYPE_BOTH    (0x1u)

/* type of storm control unit */
typedef enum
{
    EM_STORM_UNIT_PPS = 0u, /**< The unit is packet per second. */
    EM_STORM_UNIT_BPS,      /**< The unit is 0.5 Kbit per second. */
    EM_STORM_UNIT_END
} RtkStormUnit_e;

/**
 * @brief This struct is used for broadcast storm control port and state configuration.
 */
typedef struct
{
    RtkPort   port;   /**< [in] Indicate the port ID for setting or getting the state of broadcast storm control. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable enable; /**< [in,out] Indicate the state of broadcast storm control. (DISABLED, ENABLED) */
} StormCtlBroadcastConfig_t;

/**
 * @brief This struct is used for broadcast storm control unit and rate configuration.
 */
typedef struct
{
    RtkPort        port;      /**< [in] Indicate the port ID for setting or getting the broadcast storm unit and rate. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkStormUnit_e stormUnit; /**< [in,out] Indicate the broadcast storm unit of rate. (N/A) */
    uint32         rate;      /**< [in,out] Indicate the limited rate for broadcast storm in bytes or packets. (0 to 67108863) */
} StormCtlBroadcastRateConfig_t;

/**
 * @brief This struct is used for multicast storm control port, state and type configuration.
 */
typedef struct
{
    RtkPort   port;   /**< [in] Indicate the port ID for setting or getting the state of multicast storm control. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable enable; /**< [in,out] Indicate the state of multicast storm control. (DISABLED, ENABLED) */
    uint8     type;   /**< [in,out] Indicate the type of multicast storm. (MC_TYPE_UNKNOWN, MC_TYPE_BOTH) */
} StormCtlMulticastConfig_t;

/**
 * @brief This struct is used for multicast storm control unit and rate configuration.
 */
typedef struct
{
    RtkPort        port;      /**< [in] Indicate the port ID for setting or getting the multicast storm unit and rate. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkStormUnit_e stormUnit; /**< [in,out] Indicate the multicast storm unit of rate. (N/A) */
    uint32         rate;      /**< [in,out] Indicate the limited rate for multicast storm in bytes or packets. (0 to 67108863) */
} StormCtlMulticastRateConfig_t;

/**
 * @brief This struct is used for unicast storm control port and state configuration.
 */
typedef struct
{
    RtkPort   port;   /**< [in] Indicate the port ID for setting or getting the state of unicast storm control. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable enable; /**< [in,out] Indicate the state of unicast storm control. (DISABLED, ENABLED) */
} StormCtlUnicastConfig_t;

/**
 * @brief This struct is used for unicast storm control unit and rate configuration.
 */
typedef struct
{
    RtkPort        port;      /**< [in] Indicate the port ID for setting or getting the unicast storm unit and rate. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkStormUnit_e stormUnit; /**< [in,out] Indicate the unicast storm unit of rate. (N/A) */
    uint32         rate;      /**< [in,out] Indicate the limited rate for unicast storm in bytes or packets. (0 to 67108863) */
} StormCtlUnicastRateConfig_t;

EXTERN_API RtkApiRet rtk_storm_ctl_broadcast_set(UnitChip_t unitChip, StormCtlBroadcastConfig_t *stormCtlBroadcastSet);
EXTERN_API RtkApiRet rtk_storm_ctl_broadcast_get(UnitChip_t unitChip, StormCtlBroadcastConfig_t *stormCtlBroadcastGet);
EXTERN_API RtkApiRet rtk_storm_ctl_broadcast_rate_set(UnitChip_t unitChip, StormCtlBroadcastRateConfig_t *stormCtlBroadcastRateSet);
EXTERN_API RtkApiRet rtk_storm_ctl_broadcast_rate_get(UnitChip_t unitChip, StormCtlBroadcastRateConfig_t *stormCtlBroadcastRateGet);
EXTERN_API RtkApiRet rtk_storm_ctl_multicast_set(UnitChip_t unitChip, StormCtlMulticastConfig_t *stormCtlMulticastSet);
EXTERN_API RtkApiRet rtk_storm_ctl_multicast_get(UnitChip_t unitChip, StormCtlMulticastConfig_t *stormCtlMulticastGet);
EXTERN_API RtkApiRet rtk_storm_ctl_multicast_rate_set(UnitChip_t unitChip, StormCtlMulticastRateConfig_t *stormCtlMulticastRateSet);
EXTERN_API RtkApiRet rtk_storm_ctl_multicast_rate_get(UnitChip_t unitChip, StormCtlMulticastRateConfig_t *stormCtlMulticastRateGet);
EXTERN_API RtkApiRet rtk_storm_ctl_unicast_set(UnitChip_t unitChip, StormCtlUnicastConfig_t *stormCtlUnicastSet);
EXTERN_API RtkApiRet rtk_storm_ctl_unicast_get(UnitChip_t unitChip, StormCtlUnicastConfig_t *stormCtlUnicastGet);
EXTERN_API RtkApiRet rtk_storm_ctl_unicast_rate_set(UnitChip_t unitChip, StormCtlUnicastRateConfig_t *stormCtlUnicastRateSet);
EXTERN_API RtkApiRet rtk_storm_ctl_unicast_rate_get(UnitChip_t unitChip, StormCtlUnicastRateConfig_t *stormCtlUnicastRateGet);

#endif /* RTK_API_STORM_CTL_H__ */
