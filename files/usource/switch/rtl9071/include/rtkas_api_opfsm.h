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

#ifndef RTKAS_API_OPFSM_H__
#define RTKAS_API_OPFSM_H__

#define OP_OCP_BASE_ADDR (0x800u) /**< The OPFSM OCP base address offset */

/* OPFSM state */
#define OP_GLOB_RESET            (0x10u) /**< Global state: reset */
#define OP_GLOB_STANDBY          (0x20u) /**< Global state: standby */
#define OP_GLOB_NORMAL           (0x30u) /**< Global state: normal */
#define OP_GLOB_SAFETY           (0x31u) /**< Global state: safety */
#define OP_GLOB_WHOLE_CHIP_SLEEP (0x40u) /**< Global state: whole chip sleep */
#define OP_LOCAL_SLEEP           (0x10u) /**< Port local state: sleep */
#define OP_LOCAL_STANDBY         (0x20u) /**< Port local state: standby */
#define OP_LOCAL_NORMAL          (0x30u) /**< Port local state: normal */

/* Bit definition */
#define OP_LOC_SLP_REQ_BIT       (9u)  /**< bit of initializing a sleep request */
#define OP_WHOLE_SLEEP_CTRL_BIT  (10u) /**< control bit of enable whole chip sleep function. */
#define OP_SLP_EXT_INTR_CTRL_BIT (12u) /**< control bit of enable CPU alert interrupt when a specific sleep event happens */

/* Rwake state of Enable/Disable RWAKE detection */
#define OP_DISABLE_RWAKE (1u)
#define OP_ENABLE_RWAKE  (0u)

/* Event interrupt mask bit */
#define OP_REC_RG_RST_INTR_MSK  (1u << 3u) /**< Enable reset by reg to trigger interrupt */
#define OP_REC_PIN_RST_INTR_MSK (1u << 2u) /**< Enable reset by pin to trigger interrupt */
#define OP_PWON_INTR_MSK        (1u << 1u) /**< Enable power on event to trigger interrupt */
#define OP_LWAKE_INTR_MSK       (1u << 0u) /**< Enable local wake to trigger interrupt */

/* WakeO config */
#define OP_WAKEO_GPIOB1_ID         (1u) /**< The index represents GPIOB1 */
#define OP_WAKEO_GPIOB2_ID         (2u) /**< The index represents GPIOB2 */
#define OP_WAKEO_PULSETYPE_45US    (0u) /**< The index represents Wake-out forwarding pulse setting 45us */
#define OP_WAKEO_PULSETYPE_500MS   (1u) /**< The index represents Wake-out forwarding pulse setting 500ms */
#define OP_WAKEO_PULSETYPE_UNKNOWN (2u) /**< The index represents Wake-out forwarding pulse setting is invalid */

/* LWAKE edge */
#define OP_LWAKE_RISING_EDGE  (0u) /**< 0: local wake event by detecting rising edge */
#define OP_LWAKE_FALLING_EDGE (1u) /**< 1: local wake event by detecting falling edge */

/**
 * @brief Structure for sleep capability set/get functions.
 *
 * This structure holds the configurations for the sleep capability of designated ports,
 * including portMap and sleepCap.
 *
 * For the set function, this structure holds a portMap indicating ports to configure the sleep capability according to sleepCap value. \n
 * For the get function, this structure holds a portMap indicating ports with the designated sleepCap value.
 */
typedef struct
{
    RtkPmap   portMap;  /**< [in,out] Map of ports (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP))*/
    RtkEnable sleepCap; /**< [in]     The sleep capability set and get configuration (DISABLE/ENABLE) */
} OpSlpCap_t;

/**
 * @brief Structure for global OPFSM set and get functions.
 *
 * For the set function, this structure holds the current state for configure the global OPFSM state. \n
 * For the get function, this structure holds the current state of the global OPFSM state
 */
typedef struct
{
    uint16 state; /**< [in,out] Global OPFSM state. (OP_GLOB_STANDBY / OP_GLOB_NORMAL / OP_GLOB_WHOLE_CHIP_SLEEP) */
} OpGlobState_t;

/**
 * @brief Structure for local OPFSM set and get functions of a port.
 *
 * For the set function, this structure holds a port to address a designated port and the state value is the local OPFSM state to configure. \n
 * For the get function, this structure holds a port to address a designated port and return the local OPFSM state value by state.
 */
typedef struct
{
    RtkPort port;  /**< [in]     Port ID (<CHIPID>_OP_T1_USER_PORT_MIN to <CHIPID>_OP_T1_USER_PORT_MAX) */
    uint16  state; /**< [in,out] Local OPFSM state (OP_LOCAL_SLEEP/ OP_LOCAL_STANDBY / OP_LOCAL_NORMAL) */
} OpPortLocalState_t;

/**
 * @brief Structure for local OPFSM set and get functions of ports.
 *
 * For the set function, this structure holds a port map to addressed designated ports, and the state value is the local OPFSM state to configure. \n
 * For the get function, this structure holds a state to specify the designated local OPFSM state, and return the ports with the designated local OPFSM state by portMap.
 */
typedef struct
{
    RtkPmap portMap; /**< [in,out] The port map indicating selected ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP))*/
    uint16  state;   /**< [in]     Local OPFSM state (OP_LOCAL_SLEEP/ OP_LOCAL_STANDBY / OP_LOCAL_NORMAL) */
} OpPmapLocalState_t;

/**
 * @brief Structure for setting and getting the accept sleep capability of of ports.
 *
 * For the set function, this structure holds a portMap indicating ports to configure the accept sleep capability according to cfgValue value. \n
 * For the get function, this structure holds a portMap indicating ports with the accept sleep capability configured to the cfgValue value.
 */
typedef struct
{
    RtkPmap   portMap;  /**< [in,out] The port map indicating selected ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP))*/
    RtkEnable cfgValue; /**< [in]     Accept Sleep Capability Setting (DISABLE/ENABLE) */
} OpAcceptSlpReqCap_t;

/**
 * @brief Structure for setting and getting sleep request forwarding configuration of of ports.
 *
 * For the set function, this structure holds a portMap indicating ports to configure the sleep request forwarding according to cfgValue value. \n
 * For the get function, this structure holds a portMap indicating ports with the accept sleep request forwarding configured to the cfgValue value.
 */
typedef struct
{
    RtkPmap   portMap;  /**< [in,out] The port map indicating selected ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    RtkEnable cfgValue; /**< [in]     Sleep Request Forwarding Setting (DISABLE/ENABLE) */
} OpTxSleepMsk_t;

/**
 * @brief Structure for Whole-Chip Sleep enable set and get functions.
 *
 * For the set function, this structure holds the wholeChipOn for enable (1) or disable (0) the whole-chip sleep function. \n
 * For the get function, this structure holds the wholeChipOn of the current whole-chip sleep function is enable (1) or disable (0).
 */
typedef struct
{
    RtkEnable wholeChipOn; /**< [in,out] WholeChipSleep configuration. (DISABLE/ENABLE) */
} OpWholechipSlpEn_t;

/**
 * @brief Structure for setting and getting port members of the whole-chip sleep port map setting.
 *
 * For the set function, this structure holds a portMap indicating ports to be included (cfgValue = 1) or excluded (cfgValue = 0) in the whole-chip sleep port map. \n
 * For the get function, this structure holds a portMap indicating ports included (cfgValue = 1) or excluded (cfgValue = 0) in the whole-chip sleep port map.
 */
typedef struct
{
    RtkPmap   portMap;  /**< [in,out] The port map indicating selected ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    RtkEnable cfgValue; /**< [in]     Whole chip sleep port setting. (DISABLE/ENABLE) */
} OpWholechipSlpMsk_t;

/**
 * @brief Structure for holding ports to initialize a TC10 sleep request.
 *
 * This structure holds a portMap indicating ports to initialize a sleep handshake by sending a sleep request to its link partner.
 */
typedef struct
{
    RtkPmap portMap; /**< [in] The port map indicating selected ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpPortSendSlpReq_t;

/**
 * @brief Structure for holding configuration of enabling or disabling sleep external event interrupts.
 *
 * For the set function, this structure holds the extIntrOn for enabling (1) or disabling (0) the sleep event external interrupt. \n
 * For the get function, this structure holds the extIntrOn configuration indicating the current sleep event external interrupt is enable(1) or disable(0).
 */
typedef struct
{
    RtkEnable extIntrOn; /**< [in,out] External interrupt triggered by sleep events configuration. (DISABLE/ENABLE) */
} OpSlpEventIntrEn_t;

/**
 * @brief Structure for holding a port map including ports that have received sleep events.
 *
 * Each bit in the port map value corresponds to a specific port received sleep event flag.
 */
typedef struct
{
    uint16 rxSlpFlag; /**< [out] The receiving sleep flag. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpRxSlpFlagGet_t;

/**
 * @brief Structure for holding a port map including ports that have sleep fail event.
 *
 * Each bit in the port map value corresponds to a specific port sleep fail event flag.
 */
typedef struct
{
    uint16 slpFailFlag; /**< [out] The sleep handshake fail flag. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpSlpFailFlagGet_t;

/**
 * @brief Structure for holding a port map including ports that have sleep success events.
 *
 * Each bit in the port map value corresponds to a specific port sleep success event flag.
 */
typedef struct
{
    uint16 slpSuccessFlag; /**< [out] The sleep handshake success flag. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpRxSlpSuccessFlagGet_t;

/**
 * @brief Structure for holding ports to initialize a TC10 wakeup.
 *
 * This structure holds a portMap indicating ports to send a remote wakeup to its link partner.
 */
typedef struct
{
    RtkPmap portMap; /**< [in] The port map indicating selected ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpPortSendRwake_t;

/**
 * @brief Structure for enabling or disabling the remote wakeup detection of ports and returning the remote wakeup detection configuration of each port.
 *
 * For the set function, this structure holds a portMap indicating ports to disable(cfgValue = 1) or enable(cfgValue = 0) the remote wakeup detection. \n
 * For the get function, this structure holds a portMap indicating ports that disable remote wakeup detection. If the bit value is 1, the remote wakeup detection of the corresponding port is disabled.
 */
typedef struct
{
    RtkPmap portMap;  /**< [in,out] The port map indicating ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    uint8   cfgValue; /**< [in]     Set value. (OP_ENABLE_RWAKE/OP_DISABLE_RWAKE) */
} OpDisRwake_t;

/**
 * @brief Structure for enabling or disabling the remote wakeup transmit of ports and returning the remote wakeup transmit configuration of each port.
 *
 * For the set function, this structure holds a portMap indicating ports to enable(cfgValue = 1) or disable(cfgValue = 0) remote wakeup transmit. \n
 * For the get function, this structure holds a portMap indicating ports configured to enable(cfgValue = 1) or disable(cfgValue = 0) remote wakeup transmit.
 */
typedef struct
{
    RtkPmap   portMap;  /**< [in,out] The port map indicating setting of WUP/WUR transmitting. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    RtkEnable cfgValue; /**< [in]     Set value. (DISABLE/ENABLE) */
} OpEnWuprTx_t;

/**
 * @brief Structure for enabling or disabling the designated ports from wakeup by a local wakeup, and returning the current configuration of ports.
 *
 * For the set function, this structure holds a portMap indicating ports to enable (cfgValue = 1) or disable (cfgValue = 0) waking up when detecting local wakeup. \n
 * For the get function, this structure holds a portMap indicating ports that are enabled (cfgValue = 1) or disabled (cfgValue = 0) waking up when detecting local wakeup.
 */
typedef struct
{
    RtkPmap   portMap;  /**< [in,out] lwake_wake_port port map. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    RtkEnable cfgValue; /**< [in]     Set value. (DISABLE/ENABLE) */
} OpLwakeWakePort_t;

/**
 * @brief Structure for getting the remote wakeup forwarding configuration of a designated port,
 * including the selected port index and a port map value related to the remote wakeup forwarding behavior.
 *
 * This structure holds a portSel indicating a specific port, and portmap indicating the ports that will trigger the specific port to send a remote wakeup.
 */
typedef struct
{
    RtkPort portSel; /**< [in] Port ID. (<CHIPID>_OP_T1_USER_PORT_MIN to <CHIPID>_OP_T1_USER_PORT_MAX) */
    RtkPmap portmap; /**< [out] Port map indicates forwarding remote wake setting. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpRwakeFwdPxCfgGet_t;

/**
 * @brief Structure for specifying the remote wakeup forwarding configuration of a designated port,
 * including the selected port index and a port map which will make the port send a remote wakeup to its link partner.
 *
 * This structure holds a portSel indicating a specific port, and portmap indicating the ports to enable(cfgValue = 1) or disable(cfgValue = 0) to trigger the specific port to send a remote wakeup.
 */
typedef struct
{
    RtkPort   portSel;  /**< [in] Port ID. (<CHIPID>_OP_T1_USER_PORT_MIN to <CHIPID>_OP_T1_USER_PORT_MAX) */
    RtkPmap   portMap;  /**< [in] The port map indicating selected ports. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    RtkEnable cfgValue; /**< [in] Set value. (DISABLE/ENABLE) */
} OpRwakeFwdPxCfgSet_t;

/**
 * @brief Structure for configuring the WUR external interrupt mask of a designated port,
 * including the selected port index and a port map value related to the remote wakeup forwarding behavior.
 *
 * For the set function, this structure holds mask(cfgValue = 1) or unmask(cfgValue = 0) WUR external interrupt of the designated port map. \n
 * For the get function, this structure holds a portMap value and cfgValue. \n
 * If cfgValue is 1, the portMap indicates ports mask the WUR external interrupt. \n
 * If cfgValue is 0, the portMap indicates ports unmask the WUR external interrupt.
 */
typedef struct
{
    RtkPmap portMap;  /**< [in,out] Designated port map to configure WUR external interrupt mask (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    uint8   cfgValue; /**< [in] Set value of external interrupt port mask. (0/1) */
} OpWurExtIntrMsk_t;

/**
 * @brief Structure for configuring the WUP external interrupt mask of a designated port,
 * including the selected port index and a port map value related to the remote wakeup forwarding behavior.
 *
 * For the set function, this structure holds mask(cfgValue = 1) or unmask(cfgValue = 0) WUP external interrupt of the designated port map. \n
 * For the get function, this structure holds a portMap value and cfgValue. \n
 * If cfgValue is 1, the portMap indicating ports mask the WUP external interrupt. \n
 * If cfgValue is 0, the portMap indicating ports unmask the WUP external interrupt.
 */
typedef struct
{
    RtkPmap portMap;  /**< [in,out] Designated port map to configure WUP external interrupt mask (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    uint8   cfgValue; /**< [in] Set value of external interrupt port mask. (0/1) */
} OpWupExtIntrMsk_t;

/**
 * @brief Structure for setting and getting the external interrupt mask of OPFSM related system events.
 *
 * For the set function, this structure holds a cfgValue to set the external interrupt mask of OPFSM related system events. \n
 * For the get function, this structure holds a cfgValue indicating current external interrupt mask of OPFSM related system events. \n
 * Definition of each bit in cfgValue: \n
 *   bit3: rec_rg_rst_intr_msk (bit value 0x1: stop reset by reg to trigger interrupt) \n
 *   bit2: rec_pin_rst_intr_msk (bit value 0x1: stop reset by pin to trigger interrupt) \n
 *   bit1: pwon_intr_msk (bit value 0x1: stop enable power on event to trigger interrupt) \n
 *   bit0: lwake_intr_msk (bit value 0x1: stop local wake to trigger interrupt)
 */
typedef struct
{
    uint16 cfgValue; /**< [in,out] Set value of port mask of event external interrupt. (0/1) */
} OpEventExtIntrMsk_t;

/**
 * @brief Structure for holding a port map indicating each port's WUP flag.
 *
 * Each bit in the port map value corresponds to a specific port's WUP flag.
 */
typedef struct
{
    RtkPmap portMap; /**< [out] The WUP flag. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpWupFlagGet_t;

/**
 * @brief Structure for holding a port map indicating each port's WUR flag.
 *
 * Each bit in the port map value corresponds to a specific port's WUR flag.
 */
typedef struct
{
    RtkPmap portMap; /**< [out] The WUR flag. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
} OpWurFlagGet_t;

/**
 * @brief Structure for holding the local wakeup edge selection.
 *
 * For the set function, this structure holds the lwakeEdge for configuring the local wakeup edge to rising edge(0) or falling edge(1). \n
 * For the get function, this structure holds the lwakeEdge of the current local wakeup edge selection.
 */
typedef struct
{
    uint16 lwakeEdge; /**< [in,out] The LWAKE signal edge. (<CHIPOP_LWAKE_RISING_EDGE5, <CHIPOP_LWAKE_FALLING_EDGE) */
} OpLwakeEdgeSelect_t;

/**
 * @brief Structure for wake-out forwarding configuration.
 *
 * For the set function, this structure holds the configIdx indicating the selected config index, \n
 * and cfgValue indicating enable(cfgValue = 1) or disable(cfgValue = 0) wake-output of designated ports addressed by portMap. \n
 * For the get function, this structure holds the portMap indicating ports in the inputted configIdx with enable(cfgValue = 1) or disable(cfgValue = 0) wake-output function.
 */
typedef struct
{
    uint8     configIdx; /**< [in] The wake-out forwarding port map config index. (0/1) */
    RtkPmap   portMap;   /**< [in,out] The Wake-out forwarding port map. (0x0 to (<CHIPID>_OP_RTPGE_USER_PORTMAP | <CHIPID>_OP_BWPHY_USER_PORTMAP)) */
    RtkEnable cfgValue;  /**< [in] Set value. ((DISABLE/ENABLE) */
} OpWakeoPortMap_t;

/**
 * @brief Structure for holding the wake-out forwarding pulse configuration.
 *
 * For the set function, this structure holds the pulseCfg for configuring the wake-out forwarding pulse width to 45us(0) or 500ms(1). \n
 * For the get function, this structure holds the pulseCfg of the current wake-out forwarding pulse width.
 */
typedef struct
{
    uint16 pulseCfg; /**< [in,out] The wake-out forwarding pulse width setting. (<CHIPID>_OP_WAKEO_PULSETYPE_45US, <CHIPID>_OP_WAKEO_PULSETYPE_500MS) */
} OpWakeoPulse_t;

/** Wake and Sleep Interface Configuration **/
/* Sleep */
EXTERN_API RtkApiRet rtk_op_phy_sleep_cap_get(UnitChip_t unitChip, OpSlpCap_t *pOpSlpCapGet);
EXTERN_API RtkApiRet rtk_op_phy_sleep_cap_set(UnitChip_t unitChip, OpSlpCap_t *pOpSlpCapSet);
EXTERN_API RtkApiRet rtk_op_accept_sleep_req_cap_get(UnitChip_t unitChip, OpAcceptSlpReqCap_t *pOpAcceptSlpReqCapGet);
EXTERN_API RtkApiRet rtk_op_accept_sleep_req_cap_set(UnitChip_t unitChip, OpAcceptSlpReqCap_t *pOpAcceptSlpReqCapSet);
EXTERN_API RtkApiRet rtk_op_txsleep_portmap_get(UnitChip_t unitChip, OpTxSleepMsk_t *pOpTxSleepMskGet);
EXTERN_API RtkApiRet rtk_op_txsleep_portmap_set(UnitChip_t unitChip, OpTxSleepMsk_t *pOpTxSleepMskSet);
EXTERN_API RtkApiRet rtk_op_wholechip_sleep_en_get(UnitChip_t unitChip, OpWholechipSlpEn_t *pOpWholechipSlpEnGet);
EXTERN_API RtkApiRet rtk_op_wholechip_sleep_en_set(UnitChip_t unitChip, OpWholechipSlpEn_t *pOpWholechipSlpEnSet);
EXTERN_API RtkApiRet rtk_op_get_wholechip_sleep_portmap(UnitChip_t unitChip, OpWholechipSlpMsk_t *pOpWholechipSlpMskGet);
EXTERN_API RtkApiRet rtk_op_set_wholechip_sleep_portmap(UnitChip_t unitChip, OpWholechipSlpMsk_t *pOpWholechipSlpMskSet);
/* Wake */
EXTERN_API RtkApiRet rtk_op_lwake_wake_port_get(UnitChip_t unitChip, OpLwakeWakePort_t *pOpLwakeWakePortGet);
EXTERN_API RtkApiRet rtk_op_lwake_wake_port_set(UnitChip_t unitChip, OpLwakeWakePort_t *pOpLwakeWakePortSet);
EXTERN_API RtkApiRet rtk_op_lwake_edge_sel_get(UnitChip_t unitChip, OpLwakeEdgeSelect_t *pOpLwakeEdgeSelectGet);
EXTERN_API RtkApiRet rtk_op_lwake_edge_sel_set(UnitChip_t unitChip, OpLwakeEdgeSelect_t *pOpLwakeEdgeSelectSet);
EXTERN_API RtkApiRet rtk_op_get_portmap_fwd_rwake_to_port(UnitChip_t unitChip, OpRwakeFwdPxCfgGet_t *pOpRwakeFwdPxCfgGet);
EXTERN_API RtkApiRet rtk_op_set_portmap_fwd_rwake_to_port(UnitChip_t unitChip, OpRwakeFwdPxCfgSet_t *pOpRwakeFwdPxCfgSet);
EXTERN_API RtkApiRet rtk_op_wakeo_portmap_get(UnitChip_t unitChip, OpWakeoPortMap_t *pOpWakeoPortMapGet);
EXTERN_API RtkApiRet rtk_op_wakeo_portmap_set(UnitChip_t unitChip, OpWakeoPortMap_t *pOpWakeoPortMapSet);
EXTERN_API RtkApiRet rtk_op_wakeo_pulse_get(UnitChip_t unitChip, OpWakeoPulse_t *pOpWakeoPulseGet);
EXTERN_API RtkApiRet rtk_op_wakeo_pulse_set(UnitChip_t unitChip, OpWakeoPulse_t *pOpWakeoPulseSet);

/* Sleep/Wake Service Primitives and Interfaces */
/* OPFSM */
EXTERN_API RtkApiRet rtk_op_global_state_get(UnitChip_t unitChip, OpGlobState_t *pOpGlobStateGet);
EXTERN_API RtkApiRet rtk_op_global_state_set(UnitChip_t unitChip, OpGlobState_t *pOpGlobStateSet);
EXTERN_API RtkApiRet rtk_op_port_local_state_get(UnitChip_t unitChip, OpPortLocalState_t *pOpPortLocalStateGet);
EXTERN_API RtkApiRet rtk_op_port_local_state_set(UnitChip_t unitChip, OpPortLocalState_t *pOpPortLocalStateSet);
EXTERN_API RtkApiRet rtk_op_portmap_local_state_get(UnitChip_t unitChip, OpPmapLocalState_t *pOpPmapLocalStateGet);
EXTERN_API RtkApiRet rtk_op_portmap_local_state_set(UnitChip_t unitChip, OpPmapLocalState_t *pOpPmapLocalStateSet);
/* sleep request and indication */
EXTERN_API RtkApiRet rtk_op_port_send_sleep_request(UnitChip_t unitChip, OpPortSendSlpReq_t *pOpPortSendSlpReq);
EXTERN_API RtkApiRet rtk_op_rx_sleep_flag_get(UnitChip_t unitChip, OpRxSlpFlagGet_t *pOpRxSlpFlagGet);
EXTERN_API RtkApiRet rtk_op_sleep_fail_flag_get(UnitChip_t unitChip, OpSlpFailFlagGet_t *pOpSlpFailFlagGet);
EXTERN_API RtkApiRet rtk_op_sleep_success_flag_get(UnitChip_t unitChip, OpRxSlpSuccessFlagGet_t *pOpRxSlpSuccessFlagGet);
/* wake request and indication */
EXTERN_API RtkApiRet rtk_op_port_send_rwake(UnitChip_t unitChip, OpPortSendRwake_t *pOpPortSendRwake);
EXTERN_API RtkApiRet rtk_op_en_wupr_tx_get(UnitChip_t unitChip, OpEnWuprTx_t *pOpEnWuprTxGet);
EXTERN_API RtkApiRet rtk_op_en_wupr_tx_set(UnitChip_t unitChip, OpEnWuprTx_t *pOpEnWuprTxSet);
EXTERN_API RtkApiRet rtk_op_disable_rwake_get(UnitChip_t unitChip, OpDisRwake_t *pOpDisRwakeGet);
EXTERN_API RtkApiRet rtk_op_disable_rwake_set(UnitChip_t unitChip, OpDisRwake_t *pOpDisRwakeSet);
EXTERN_API RtkApiRet rtk_op_wup_flag_get(UnitChip_t unitChip, OpWupFlagGet_t *pOpWupFlagGet);
EXTERN_API RtkApiRet rtk_op_wur_flag_get(UnitChip_t unitChip, OpWurFlagGet_t *pOpWurFlagGet);
EXTERN_API RtkApiRet rtk_op_wakeflag_clearall(UnitChip_t unitChip);
/* interrupt configuration */
EXTERN_API RtkApiRet rtk_op_wur_ext_intr_msk_get(UnitChip_t unitChip, OpWurExtIntrMsk_t *pOpWurExtIntrMskGet);
EXTERN_API RtkApiRet rtk_op_wur_ext_intr_msk_set(UnitChip_t unitChip, OpWurExtIntrMsk_t *pOpWurExtIntrMskSet);
EXTERN_API RtkApiRet rtk_op_wup_ext_intr_msk_get(UnitChip_t unitChip, OpWupExtIntrMsk_t *pOpWupExtIntrMskGet);
EXTERN_API RtkApiRet rtk_op_wup_ext_intr_msk_set(UnitChip_t unitChip, OpWupExtIntrMsk_t *pOpWupExtIntrMskSet);
EXTERN_API RtkApiRet rtk_op_event_ext_intr_msk_get(UnitChip_t unitChip, OpEventExtIntrMsk_t *pOpEventExtIntrMskGet);
EXTERN_API RtkApiRet rtk_op_event_ext_intr_msk_set(UnitChip_t unitChip, OpEventExtIntrMsk_t *pOpEventExtIntrMskSet);
EXTERN_API RtkApiRet rtk_op_sleep_event_intr_en_get(UnitChip_t unitChip, OpSlpEventIntrEn_t *pOpSlpEventIntrEnGet);
EXTERN_API RtkApiRet rtk_op_sleep_event_intr_en_set(UnitChip_t unitChip, OpSlpEventIntrEn_t *pOpSlpEventIntrEnSet);

#endif /* RTKAS_API_OPFSM_H__ */
