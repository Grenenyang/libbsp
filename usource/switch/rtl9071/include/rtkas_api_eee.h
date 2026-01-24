/*
 * Copyright (c) 2024 Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * This software component is confidential and proprietary to Realtek
 * Semiconductor Corp. Disclosure, reproduction, redistribution, in whole
 * or in part, of this work and its derivatives without express permission
 * is prohibited.
 */

#ifndef RTKAS_API_EEE_H
#define RTKAS_API_EEE_H

#include "rtkas_common.h"

#define EEE_ENABLE_ALL_PORT  (0u)
#define EEE_DISABLE_ALL_PORT (1u)

#define EEE_WAKE_NORMAL_MODE (0x1u)
#define EEE_WAKE_QOS_MODE    (0x2u)

/**
 * @brief The unit for timer T_tx_rate and T_wakeup in the EEE function
 */
typedef enum
{
    EM_EEE_TIMER_UNIT_1US = 0, /**< 1us:    2'b00 */
    EM_EEE_TIMER_UNIT_16US,    /**< 16us:   2'b01 */
    EM_EEE_TIMER_UNIT_128US,   /**< 128us:  2'b10 */
    EM_EEE_TIMER_UNIT_1024US,  /**< 1024us: 2'b11 */
    EM_EEE_TIMER_UNIT_END,
} EeeTimerUnit_e;

/**
 * @brief The type of timer unit in the EEE function
 */
typedef enum
{
    EM_EEE_TIMER_100M = 0, /**< 100M */
    EM_EEE_TIMER_1000M,    /**< 1000M */
    EM_EEE_TIMER_10M,      /**< 10M */
    EM_EEE_TIMER_TYPE_END,
} EeeTimerType_e;

/**
 * @brief The type of EEE wake configuration
 */
typedef enum
{
    EM_EEE_WAKE_NORMAL_MODE = 0x1u, /**< 2'b01: Normal Mode.  */
    EM_EEE_WAKE_QOS_MODE    = 0x2u, /**< 2'b10: QoS Mode. */
    EM_EEE_WAKE_SET_END,
} EeeWakeSet_e;

/**
 * @brief Structure for enabling and disabling all port EEE functions.
 *
 * For set function, this structure holds the configuration of enabling or disabling the EEE function. \n
 * For get function, this structure holds the state of the current all port EEE function state.
 */
typedef struct
{
    uint32 cfg; /**< [in,out] Disable all port EEE functions. DISABLE: Disable EEE function for all ports. ENABLE: Enable EEE function for all ports (DISABLE to ENABLE) */
} EeeEnableCfg_t;

/**
 * @brief Structure for configuring per-port EEE TX_EN.
 *
 * For set function, this structure holds the configuration of enabling or disabling EEE enable RX and enable TX of the designated port. \n
 * For get function, this structure holds the current EEE enable RX and enable TX configuration of the designated port.
 */
typedef struct
{
    RtkPort   port;      /**< [in]     Port ID (<CHIPID>_EEE_USER_PORT_MIN to <CHIPID>_EEE_USER_PORT_MAX) */
    RtkEnable enPortEee; /**< [in,out] Per-port EEE_TX_EN and EEE_RX_EN configuration (ENABLE, DISABLE) */
} EeePerPortEnCfg_t;

/**
 * @brief Structure of per-port EEE_RX_STS, EEE_TX_STS and EEE_STS.
 */
typedef struct
{
    uint8 rxStatus; /**< [out] Per-port LPI RX status (0 to 1)
                        - 0: Port TX LPI is not asserted
                        - 1: Port TX LPI is asserted */
    uint8 txStatus; /**< [out] Per-port LPI TX status (0 to 1)
                        - 0: Port TX LPI is not asserted
                        - 1: Port TX LPI is asserted */
    uint8 status;   /**< [out] Indicate the EEE status. 1: Both TX and RX are in LPI mode simultaneously. 0: TX or RX is in normal mode. (0 to 1) */
} EeePortStatus_t;

/**
 * @brief Structure for getting the EEE status of designated port.
 */
typedef struct
{
    RtkPort         port;      /**< [in]  Port ID (<CHIPID>_EEE_USER_PORT_MIN to <CHIPID>_EEE_USER_PORT_MAX) */
    EeePortStatus_t eeeStatus; /**< [out] EEE status (NA) */
} EeePerPortStatus_t;

/**
 * @brief Structure of PHY LPI RX Status, PHY LPI TX Status.
 */
typedef struct
{
    uint8 phyRxSts; /**< [out] Per-PHY LPI RX status (0 to 1)
                        - 0: T1-PHY RX LPI is not asserted
                        - 1: T1-PHY RX LPI is asserted */
    uint8 phyTxSts; /**< [out] Per-PHY LPI TX status (0 to 1)
                        - 0: T1-PHY TX LPI is not asserted
                        - 1: T1-PHY TX LPI is asserted */
} EeePhyStatus_t;

/**
 * @brief Structure for getting the EEE status of designated port.
 */
typedef struct
{
    RtkPort        port;         /**< [in]  Port ID (<CHIPID>_EEE_USER_PORT_MIN to <CHIPID>_EEE_USER_PORT_MAX) */
    EeePhyStatus_t phyLpiStatus; /**< [out] EEE status of PHY (NA) */
} EeePerPhyStatus_t;

/**
 * @brief Structure for time unit configuration in EEE.
 *
 * For set function, this structure holds the configuration for setting the timer unit of the specific timer type. \n
 * For get function, this structure holds the timer unit of the specific timer type.
 */
typedef struct
{
    EeeTimerType_e type;      /**< [in]     timer type. (NA) */
    EeeTimerUnit_e timerUnit; /**< [in,out] timer unit of the specific timer type (NA)*/
} EeeTimerUnitCfg_t;

/**
 * @brief Structure for all timer value configurations in EEE.
 *
 * <b>T_pause</b>:\n
 * If the Request LPI set enable rxPauseOnFrame, upon receiving a Pause Frame, start the T_pause timer. \n
 * If a non-pause frame is not received before the T_pause timer expires, set the EEE_Pause flag and enter the LPI state. \n
 * \n
 * <b>T_tx_delay</b>:\n
 * If the EEE Wake set is EM_EEE_WAKE_QOS_MODE and the EEE_Pause flag is not set, if there is a new packet to be forwarded and the packet is sent to the low priority TX queue, start the T_tx_delay timer. \n
 * Exit the LPI state after the T_tx_delay timer expires. \n
 * Before the T_tx_delay timer expires, the LPI state will be exited immediately under the following two conditions: \n
 * 1.There are packets in the high priority queue. \n
 * 2.The accumulated number of packets exceeds the TX threshold setting. \n
 * \n
 * <b>T_tx_rate</b>:\n
 * If the Request LPI set enable txRate, the T_tx_rate timer starts. \n
 * If the accumulated TX bytes of the port are below the Queue TX threshold before the T_tx_rate timer expires, set the Request LPI. \n
 * \n
 * <b>T_wakeup</b>:\n
 * After the MAC signals the PHY to exit the LPI state, start the T_wakeup timer. \n
 * Upon the T_wakeup timer expiring, the PHY sends a WAKE signal to the link partner. \n\n
 *
 * For set function, this structure holds the configuration for setting the timer value of the specific timer type. \n
 * For get function, this structure holds the timer value of the specific timer type.
 */
typedef struct
{
    uint8 tp; /**< [in,out] Timer value for T_pause, in units defined by the timer unit setting. (0 to 255). */
    uint8 td; /**< [in,out] timer value of T_tx_delay, in units defined by the timer unit setting. (0 to 255) */
    uint8 tr; /**< [in,out] timer value of T_tx_rate, in units defined by the timer unit setting (1 to 255)*/
    uint8 tw; /**< [in,out] timer value of T_wakeup, in units defined by the timer unit setting (12 to 255) */
} EeeTimerValue_t;

/**
 * @brief Structure for timer value configuration of the designated timer type.
 *
 * For set function, this structure holds the timer value struct for setting the timer value of the specific timer type. \n
 * For get function, this structure holds the timer value of the specific timer type.
 */
typedef struct
{
    EeeTimerType_e  type;        /**< [in]     timer type. (NA) */
    EeeTimerValue_t eeeTimerVal; /**< [in,out] timer value of T_pause, T_tx_delay, T_tx_rate, T_wakeup (NA) */
} EeeTimerValueCfg_t;

/**
 * @brief Structure for LPI decision maker of the designated timer type.
 *
 * For set function, this structure holds the LPI decision maker configurations of the designated port \n
 * For get function, this structure returns the LPI decision maker configurations of the designated port.
 */
typedef struct
{
    RtkPort   port;           /**< [in]     Port ID (<CHIPID>_EEE_USER_PORT_MIN to <CHIPID>_EEE_USER_PORT_MAX) */
    RtkEnable txRate;         /**< [in,out] Enable LPI TX request Rate decision maker for EEE (DISABLE to ENABLE) */
    RtkEnable queueEmpty;     /**< [in,out] Enable LPI TX request Rate decision maker for EEE (DISABLE to ENABLE) */
    RtkEnable rxPauseOnFrame; /**< [in,out] Enable LPI TX request Rate decision maker for EEE (DISABLE to ENABLE) */
} EeeLpiRegDecisionMakerCfg_t;

/**
 * @brief Structure for TX threshold configuration of TX Rate LPI request Decision Maker of the designated port.
 *
 * For set function, this structure holds the TX threshold count value of TX Rate LPI request Decision Maker of the designated port \n
 * For get function, this structure returns the TX threshold count value of TX Rate LPI request Decision Maker of the designated port.
 */
typedef struct
{
    RtkPort port;   /**< [in]     Port ID (<CHIPID>_EEE_USER_PORT_MIN to <CHIPID>_EEE_USER_PORT_MAX) */
    uint32  qTxThr; /**< [in,out] EEE Queue TX Threshold for LPI TX request Rate decision maker (1 to 255)*/
} EeeQueueTxThrCfg_t;

/**
 * @brief Structure for high priority queue configuration of QoS mode wake LPI decision maker of the designated port.
 *
 * For set function, this structure holds the queue priority configuration of wake decision maker of the designated port. \n
 * For get function, this structure return the queue priority configuration of wake decision maker of the designated port.
 */
typedef struct
{
    RtkPort port;        /**< [in]     Port ID (<CHIPID>_EEE_USER_PORT_MIN to <CHIPID>_EEE_USER_PORT_MAX) */
    uint32  queuePriCfg; /**< [in,out] Specify high priority TX Queue for wake decision maker (0 to 255)
                                        - bit[0]: Tx queue 0
                                        - bit[1]: Tx queue 1
                                        - bit[2]: Tx queue 2
                                        - bit[3]: Tx queue 3
                                        - bit[4]: Tx queue 4
                                        - bit[5]: Tx queue 5
                                        - bit[6]: Tx queue 6
                                        - bit[7]: Tx queue 7 */
} EeeTxQueuePriCfg_t;

/**
 * @brief Structure for wake decision maker of the designated timer type.
 *
 * For set function, this structure holds the enable TX high priority packet Wakeup decision maker configuration of the designated port \n
 * For get function, this structure returns the enable configuration of TX high priority packet Wakeup decision maker of the designated port.
 */
typedef struct
{
    RtkPort      port;    /**< [in]     Port ID (<CHIPID>_EEE_USER_PORT_MIN to <CHIPID>_EEE_USER_PORT_MAX) */
    EeeWakeSet_e wakeSet; /**< [in,out] EEE TX Wake Set (NA) */
} EeeWakeDecisionMakerCfg_t;

/* EEE enable */
EXTERN_API RtkApiRet rtk_eee_set_en(UnitChip_t unitChip, EeeEnableCfg_t *pEeeEnableCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_en(UnitChip_t unitChip, EeeEnableCfg_t *pEeeEnableCfgGet);
EXTERN_API RtkApiRet rtk_eee_set_port_en(UnitChip_t unitChip, EeePerPortEnCfg_t *pEeePerPortRxTxEnCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_port_en(UnitChip_t unitChip, EeePerPortEnCfg_t *pEeePerPortRxTxEnCfgGet);
/* EEE Timer */
EXTERN_API RtkApiRet rtk_eee_set_timer_unit(UnitChip_t unitChip, EeeTimerUnitCfg_t *pEeeTimerUnitCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_timer_unit(UnitChip_t unitChip, EeeTimerUnitCfg_t *pEeeTimerUnitCfgGet);
EXTERN_API RtkApiRet rtk_eee_set_timer_value(UnitChip_t unitChip, EeeTimerValueCfg_t *pEeeTimerValueCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_timer_value(UnitChip_t unitChip, EeeTimerValueCfg_t *pEeeTimerValueCfgGet);
/* EEE Policy. LPI and Wake Decision Maker */
EXTERN_API RtkApiRet rtk_eee_set_port_lpi_req(UnitChip_t unitChip, EeeLpiRegDecisionMakerCfg_t *pEeeLpiRegDecisionMakerCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_port_lpi_req(UnitChip_t unitChip, EeeLpiRegDecisionMakerCfg_t *pEeeLpiRegDecisionMakerCfgGet);
EXTERN_API RtkApiRet rtk_eee_set_port_lpi_qtx_thr(UnitChip_t unitChip, EeeQueueTxThrCfg_t *pEeeLpiQtxThrCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_port_lpi_qtx_thr(UnitChip_t unitChip, EeeQueueTxThrCfg_t *pEeeLpiQtxThrCfgGet);
EXTERN_API RtkApiRet rtk_eee_set_port_qtx_pri(UnitChip_t unitChip, EeeTxQueuePriCfg_t *pEeeWakeTxQueuePriCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_port_qtx_pri(UnitChip_t unitChip, EeeTxQueuePriCfg_t *pEeeWakeTxQueuePriCfgGet);
EXTERN_API RtkApiRet rtk_eee_set_port_wake(UnitChip_t unitChip, EeeWakeDecisionMakerCfg_t *pEeeWakeDecisionMakerCfgSet);
EXTERN_API RtkApiRet rtk_eee_get_port_wake(UnitChip_t unitChip, EeeWakeDecisionMakerCfg_t *pEeeWakeDecisionMakerCfgGet);
/* EEE Status */
EXTERN_API RtkApiRet rtk_eee_get_phy_func_sts(UnitChip_t unitChip, EeePerPhyStatus_t *pEeePerPhyStsGet);
EXTERN_API RtkApiRet rtk_eee_get_port_func_sts(UnitChip_t unitChip, EeePerPortStatus_t *pEeePerPortEeeStsGet);

#endif /* RTKAS_API_EEE_H */
