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

#ifndef RTK_API_STP_H__
#define RTK_API_STP_H__

/* Minimum and maximum value for checking values' range. */
#define MSTIMAX                     (15u)
#define STP_BRIDGE_PORT_PRIORITYMAX (0xFu)
#define STP_AGEMIN                  (6u)
#define STP_AGEMAX                  (40u)
#define STP_FORWARD_DELAYMIN        (4u)
#define STP_FORWARD_DELAYMAX        (30u)
#define STP_HELLOTIMEMIN            (1u)
#define STP_HELLOTIMEMAX            (2u)
#define STP_PATHCOSTMAX             (0xFu)

/* Bridge port priority */
#define RTK_STP_BRIDGE_PORT_PRIORITY_0     (0u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_4096  (1u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_8192  (2u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_12288 (3u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_16384 (4u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_20480 (5u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_24576 (6u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_28672 (7u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_32768 (8u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_36864 (9u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_40960 (10u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_45056 (11u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_49152 (12u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_53248 (13u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_57344 (14u)
#define RTK_STP_BRIDGE_PORT_PRIORITY_61440 (15u)

/* Port priority */
#define RTK_STP_PORT_PRIORITY_0   (0u)
#define RTK_STP_PORT_PRIORITY_16  (1u)
#define RTK_STP_PORT_PRIORITY_32  (2u)
#define RTK_STP_PORT_PRIORITY_48  (3u)
#define RTK_STP_PORT_PRIORITY_64  (4u)
#define RTK_STP_PORT_PRIORITY_80  (5u)
#define RTK_STP_PORT_PRIORITY_96  (6u)
#define RTK_STP_PORT_PRIORITY_112 (7u)
#define RTK_STP_PORT_PRIORITY_128 (8u)
#define RTK_STP_PORT_PRIORITY_144 (9u)
#define RTK_STP_PORT_PRIORITY_160 (10u)
#define RTK_STP_PORT_PRIORITY_176 (11u)
#define RTK_STP_PORT_PRIORITY_192 (12u)
#define RTK_STP_PORT_PRIORITY_208 (13u)
#define RTK_STP_PORT_PRIORITY_224 (14u)
#define RTK_STP_PORT_PRIORITY_240 (15u)

/* Port path cost */
#define RTK_STP_PATHCOST_AUTO      (0u)
#define RTK_STP_PATHCOST_200000000 (1u)
#define RTK_STP_PATHCOST_20000000  (2u)
#define RTK_STP_PATHCOST_2000000   (3u)
#define RTK_STP_PATHCOST_1999999   (4u)
#define RTK_STP_PATHCOST_1000000   (5u)
#define RTK_STP_PATHCOST_200000    (6u)
#define RTK_STP_PATHCOST_199999    (7u)
#define RTK_STP_PATHCOST_100000    (8u)
#define RTK_STP_PATHCOST_20000     (9u)
#define RTK_STP_PATHCOST_19999     (10u)
#define RTK_STP_PATHCOST_10000     (11u)
#define RTK_STP_PATHCOST_2000      (12u)
#define RTK_STP_PATHCOST_200       (13u)
#define RTK_STP_PATHCOST_20        (14u)
#define RTK_STP_PATHCOST_2         (15u)

typedef uint32 RtkStpMstiId;       /* The data type of instance. */
typedef uint32 RtkStpBdgPri;       /* The data type of bridge priority. */
typedef uint32 RtkStpPortPri;      /* The data type of port priority. */
typedef uint32 RtkStpMaxAge;       /* The data type of max age. */
typedef uint32 RtkStpHelloTime;    /* The data type of hello time. */
typedef uint32 RtkStpFowardDelay;  /* The data type of forward delay. */
typedef uint32 RtkStpPortPathCost; /* The data type of port path cost. */

typedef enum
{
    EM_STP_TYPE_STP = 0, /**< The type is STP. */
    EM_STP_TYPE_RSTP,    /**< The type is RSTP. */
    EM_STP_TYPE_END
} RtkStpType_e;

typedef enum
{
    EM_STP_DISCARDING = 0, /**< The port state is discarding. */
    EM_STP_BLOCKING,       /**< The port state is blocking. */
    EM_STP_LEARNING,       /**< The port state is learning. */
    EM_STP_FORWARDING,     /**< The port state is forwarding. */
    EM_STP_END
} RtkStpPortState_e;

typedef enum
{
    EM_STP_P2P_AUTO = 0, /**< The p2p is auto. */
    EM_STP_P2P_TURE,     /**< The p2p is true. It recognize agreement BPDU and the convergence is fast. */
    EM_STP_P2P_FALSE,    /**< The p2p is false. It cannot recognize agreement BPDU and the convergence after max age time. */
    EM_STP_P2P_END
} RtkStpPortP2p_e;

/**
 * @brief This struct is used for STP/RSTP state configuration.
 */
typedef struct
{
    RtkEnable stpState; /**< [in,out] The p2p is auto. Indicate the global state of STP/RSTP is enabled or disabled. (DISABLED, ENABLED)*/
} StpStateConfig_t;

/**
 * @brief This struct is used for STP/RSTP type configuration.
 */
typedef struct
{
    RtkStpType_e stpType; /**< [in,out] Indicate the operating protocol is STP or RSTP. (N/A)*/
} StpTypeConfig_t;

/**
 * @brief This struct is used for bridge priority configuration.
 */
typedef struct
{
    RtkStpBdgPri stpBridgePriority; /**< [in,out] Indicate the bridge priority. (RTK_STP_BRIDGE_PORT_PRIORITY_0 to RTK_STP_BRIDGE_PORT_PRIORITY_61440)*/
} StpBridgePriorityConfig_t;

/**
 * @brief This struct is used for forward delay configuration.
 */
typedef struct
{
    RtkStpFowardDelay stpForwardDelay; /**< [in,out] Indicate the forward delay time. (4 to 30)*/
} StpForwardDelayConfig_t;

/**
 * @brief This struct is used for max age configuration.
 */
typedef struct
{
    RtkStpMaxAge stpMaxAge; /**< [in,out] Indicate the max age time. (6 to 40)*/
} StpMaxAgeConfig_t;

/**
 * @brief This struct is used for hello time configuration.
 */
typedef struct
{
    RtkStpHelloTime stpHelloTime; /**< [in,out] Indicate the hello time. (1 to 2)*/
} StpHelloTimeConfig_t;

/**
 * @brief This struct is used for hello time configuration.
 */
typedef struct
{
    RtkStpMstiId      msti;      /**< [in] Indicate the instance ID for setting or getting port state. (0 to 14)*/
    RtkPort           portId;    /**< [in] Indicate the port ID for getting port state. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    RtkStpPortState_e portState; /**< [in,out] Indicate the port state. (N/A)*/
} StpPortStateConfig_t;

/**
 * @brief This struct is used for port priority configuration.
 */
typedef struct
{
    RtkPort       portId;          /**< [in] Indicate the port ID for setting or getting port priority. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    RtkStpPortPri stpPortPriority; /**< [in,out] Indicate the port priority. (RTK_STP_PORT_PRIORITY_0 to RTK_STP_PORT_PRIORITY_240)*/
} StpPortPriorityConfig_t;

/**
 * @brief This struct is used for port path cost configuration.
 */
typedef struct
{
    RtkPort            portId;          /**< [in] Indicate the port ID for setting or getting port path cost. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    RtkStpPortPathCost stpPortPathCost; /**< [in,out] Indicate the port path cost. (RTK_STP_PATHCOST_AUTO to RTK_STP_PATHCOST_2)*/
} StpPortPathCostConfig_t;

/**
 * @brief This struct is used for administrator edge configuration.
 */
typedef struct
{
    RtkPort   portId;           /**< [in] Indicate the port ID for setting or getting the state of port admin edge. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    RtkEnable stpPortAdminEdge; /**< [in,out] Indicate the state of port admin edge. (DISABLED, ENABLED)*/
} StpPortAdminEdgeConfig_t;

/**
 * @brief This struct is used for auto edge configuration.
 */
typedef struct
{
    RtkPort   portId;          /**< [in] Indicate the port ID for setting or getting the state of port auto edge. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    RtkEnable stpPortAutoEdge; /**< [in,out] Indicate the state of port auto edge. (DISABLED, ENABLED)*/
} StpPortAutoEdgeConfig_t;

/**
 * @brief This struct is used for point to point configuration.
 */
typedef struct
{
    RtkPort         portId;     /**< [in] Indicate the port ID for setting or getting p2p. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
    RtkStpPortP2p_e stpPortP2p; /**< [in,out] Indicate the p2p type. (N/A)*/
} StpPortP2PConfig_t;

/**
 * @brief This struct is used for migration check configuration.
 */
typedef struct
{
    RtkPort portId; /**< [in] Indicate the port ID for setting migration check. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END)*/
} StpPortMigrationCheckSet_t;

EXTERN_API RtkApiRet rtk_stp_state_set(UnitChip_t unitChip, StpStateConfig_t *stpStateSet);
EXTERN_API RtkApiRet rtk_stp_state_get(UnitChip_t unitChip, StpStateConfig_t *stpStateGet);
EXTERN_API RtkApiRet rtk_stp_type_set(UnitChip_t unitChip, StpTypeConfig_t *stpTypeSet);
EXTERN_API RtkApiRet rtk_stp_type_get(UnitChip_t unitChip, StpTypeConfig_t *stpTypeGet);
EXTERN_API RtkApiRet rtk_stp_bridge_priority_set(UnitChip_t unitChip, StpBridgePriorityConfig_t *stpBridgePrioritySet);
EXTERN_API RtkApiRet rtk_stp_bridge_priority_get(UnitChip_t unitChip, StpBridgePriorityConfig_t *stpBridgePriorityGet);
EXTERN_API RtkApiRet rtk_stp_forward_delay_set(UnitChip_t unitChip, StpForwardDelayConfig_t *stpForwardDelaySet);
EXTERN_API RtkApiRet rtk_stp_forward_delay_get(UnitChip_t unitChip, StpForwardDelayConfig_t *stpForwardDelayGet);
EXTERN_API RtkApiRet rtk_stp_max_age_set(UnitChip_t unitChip, StpMaxAgeConfig_t *stpMaxAgeSet);
EXTERN_API RtkApiRet rtk_stp_max_age_get(UnitChip_t unitChip, StpMaxAgeConfig_t *stpMaxAgeGet);
EXTERN_API RtkApiRet rtk_stp_hello_time_set(UnitChip_t unitChip, StpHelloTimeConfig_t *stpHelloTimeSet);
EXTERN_API RtkApiRet rtk_stp_hello_time_get(UnitChip_t unitChip, StpHelloTimeConfig_t *stpHelloTimeGet);
EXTERN_API RtkApiRet rtk_stp_port_state_set(UnitChip_t unitChip, StpPortStateConfig_t *stpPortStateSet);
EXTERN_API RtkApiRet rtk_stp_port_state_get(UnitChip_t unitChip, StpPortStateConfig_t *stpPortStateGet);
EXTERN_API RtkApiRet rtk_stp_port_priority_set(UnitChip_t unitChip, StpPortPriorityConfig_t *stpPortPrioritySet);
EXTERN_API RtkApiRet rtk_stp_port_priority_get(UnitChip_t unitChip, StpPortPriorityConfig_t *stpPortPriorityGet);
EXTERN_API RtkApiRet rtk_stp_port_path_cost_set(UnitChip_t unitChip, StpPortPathCostConfig_t *stpPortPathCostSet);
EXTERN_API RtkApiRet rtk_stp_port_path_cost_get(UnitChip_t unitChip, StpPortPathCostConfig_t *stpPortPathCostGet);
EXTERN_API RtkApiRet rtk_stp_port_admin_edge_set(UnitChip_t unitChip, StpPortAdminEdgeConfig_t *stpPortAdminEdgeSet);
EXTERN_API RtkApiRet rtk_stp_port_admin_edge_get(UnitChip_t unitChip, StpPortAdminEdgeConfig_t *stpPortAdminEdgeGet);
EXTERN_API RtkApiRet rtk_stp_port_auto_edge_set(UnitChip_t unitChip, StpPortAutoEdgeConfig_t *stpPortAutoEdgeSet);
EXTERN_API RtkApiRet rtk_stp_port_auto_edge_get(UnitChip_t unitChip, StpPortAutoEdgeConfig_t *stpPortAutoEdgeGet);
EXTERN_API RtkApiRet rtk_stp_port_p2p_set(UnitChip_t unitChip, StpPortP2PConfig_t *stpPortP2pSet);
EXTERN_API RtkApiRet rtk_stp_port_p2p_get(UnitChip_t unitChip, StpPortP2PConfig_t *stpPortP2pGet);
EXTERN_API RtkApiRet rtk_stp_port_migration_check_set(UnitChip_t unitChip, StpPortMigrationCheckSet_t *stpPortMigrationCheckSet);

#endif /* RTK_API_STP_H__ */
