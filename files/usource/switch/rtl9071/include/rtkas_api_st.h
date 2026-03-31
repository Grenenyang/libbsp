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

#ifndef RTKAS_API_ST_H__
#define RTKAS_API_ST_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common.h"
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define TSN_NUMBER_OF_QUEUE (8u)
/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 RtkStCycleTimeEx;
/******************************************************************************/
/* Struct                                                                     */
/******************************************************************************/
/* Structure of TSN time */
typedef struct
{
    uint16 msbSecond;  /**< seconds in high bits (0 to 0xFFFF) */
    uint32 lsbSecond;  /**< seconds in low bits (0 to 0xFFFFFFFF) */
    uint32 nanosecond; /**< nanoseconds (0 to 0xFFFFFFFF) */
} RtkTsnPtpTime_t;
/* Structure of cycle time of TSN gate control list  */
typedef struct
{
    uint32 numerator;   /**< convert nanoseconds to units (0 to DVA_TSN_1B_NANOSECOND - 1) */
    uint32 denominator; /**< convert nanoseconds to units (DVA_TSN_1B_NANOSECOND) */
} RtkTsnCycleTime_t;
/* Structure of QBV gate control list  */
typedef struct
{
    uint8  operation;    /**< operation for state machine (0) */
    uint8  gateState;    /**< gate state for current operation (0 to 0xFF) */
    uint32 timeInterval; /**< operation interval for current operation (0 to 0xFFFFFFFFF) */
} StGateControlListParam_t;

/**
 * @brief This structure contains information used to determine the byte count value.
 *
 * This structure includes the switch port number, port type, speed, macsecIPG, used to determine its byte count value.
 */
typedef struct
{
    RtkPort portId;    /**< [in]Switch port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  portType;  /**< [in]Port type (<CHIPID>_ST_PORT_TYPE_START to <CHIPID>_ST_PORT_TYPE_END) */
    uint32  speed;     /**< [in]Port speed (<CHIPID>_RTK_PORT_SPEED_10M to <CHIPID>_RTK_PORT_SPEED_END) */
    uint32  macsecIPG; /**< [in]Macsec IPG (<CHIPID>_NO_MACSEC to <CHIPID>_MACSEC_END) */
} StByteCountSet_t;
/**
 * @brief This structure contains information about the byte count value of a specific port.
 *
 * This structure includes the switch port number and its byte count value.
 */
typedef struct
{
    RtkPort portId;    /**< [in]Switch port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  byteCount; /**< [out]Byte count  (<CHIPID>_BYTE_PRE_FRAME_START to <CHIPID>_BYTE_PRE_FRAME_END ) */
} StByteCountGet_t;
/**
 * @brief This structure contains information about the enable state of a specific port's gate.
 *
 * This structure includes the switch port number and the enable state of its gate.
 */
typedef struct
{
    RtkPort   portId; /**< [in]Switch port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable enable; /**< [in,out]Gate enable state: 0 means disabled, 1 means enabled. (0 to 1) */
} StGateEnableInfo_t;

/**
 * @brief This structure contains configuration information of a specific port's gate.
 *
 * This structure includes the switch port number and its gate configuration details.
 */
typedef struct
{
    RtkPort          portId;             /**< [in]Switch port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkStCycleTimeEx cycleTimeExtension; /**< [in,out]Cycle time extension (0 to <CHIPID>_ST_CYCLETIMEEX_MAX) */
    uint32           controlListLength;  /**< [in,out]Control list length (CHIPID>_SUPPORT_LIST_MIN to <CHIPID>_ST_SUPPORT_LIST_MAX) */
#if defined(RTL_907XD_VA)
    uint32 baseTimeIsOffset; /**< [in,out]Indicates whether "baseTime" is an offset from PTP time: 1 represents an offset, 0 represents absolute PTP time.(0 to 1) */
#endif
    RtkTsnPtpTime_t           baseTime;        /**< [in,out]Base time (N/A) */
    RtkTsnCycleTime_t         cycleTime;       /**< [out]Cycle time (N/A) */
    StGateControlListParam_t *gateControlList; /**< [in,out]Gate control list, the size of the gate control list is determined by "controlListLength". (N/A) */

} StGateConfigInfo_t;

/**
 * @brief This structure contains a specific port's gate state.
 *
 * This structure includes the switch port number and its gate state.
 */
typedef struct
{
    RtkPort portId;    /**< [in]Switch port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  gateState; /**< [in,out]Admin gate state (0 to 0xFFu) */

} StGateStateInfo_t;
/**
 * @brief This structure contains the configPending flag information of a specific port.
 *
 * This structure includes the switch port number and its configPending flag.
 */

typedef struct
{
    RtkPort portId;        /**< [in]Switch port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  configPending; /**< [out] Configuration pending flag: 0 means there is no configuration pending, 1 means there is a configuration pending(0 to 1) */

} StConfigPendingGet_t;

/**
 * @brief This structure contains the maximum queue SDU information of a specific port.
 *
 * This structure includes the switch port number and its maximum queue SDU information.
 */
typedef struct
{
    RtkPort portId;                           /**< [in]Switch port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  queueMaxSdu[TSN_NUMBER_OF_QUEUE]; /**< [in,out]Queue maximum SDU (0~16383u for 8 elements) */
} StQueueMaxSduInfo_t;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_st_init(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_st_byte_count_set(UnitChip_t unitChip, StByteCountSet_t *stByteCountInfo);
EXTERN_API RtkApiRet rtk_st_byte_count_get(UnitChip_t unitChip, StByteCountGet_t *stByteCountInfo);
EXTERN_API RtkApiRet rtk_st_gate_enable_set(UnitChip_t unitChip, StGateEnableInfo_t *stGateEnableInfo);
EXTERN_API RtkApiRet rtk_st_gate_enable_get(UnitChip_t unitChip, StGateEnableInfo_t *stGateEnableInfo);
EXTERN_API RtkApiRet rtk_st_gate_config_set(UnitChip_t unitChip, StGateConfigInfo_t *stGateConfigInfo);
EXTERN_API RtkApiRet rtk_st_admin_gate_config_get(UnitChip_t unitChip, StGateConfigInfo_t *stAdminGateConfigInfo);
EXTERN_API RtkApiRet rtk_st_oper_gate_config_get(UnitChip_t unitChip, StGateConfigInfo_t *stOperGateConfigInfo);
EXTERN_API RtkApiRet rtk_st_admin_gate_state_set(UnitChip_t unitChip, StGateStateInfo_t *stAdminGateStateInfo);
EXTERN_API RtkApiRet rtk_st_admin_gate_state_get(UnitChip_t unitChip, StGateStateInfo_t *stAdminGateStateInfo);
EXTERN_API RtkApiRet rtk_st_oper_gate_state_get(UnitChip_t unitChip, StGateStateInfo_t *stOperGateStateInfo);
EXTERN_API RtkApiRet rtk_st_config_pending_get(UnitChip_t unitChip, StConfigPendingGet_t *stConfigPendingInfo);

EXTERN_API RtkApiRet rtk_st_queue_max_sdu_set(UnitChip_t unitChip, StQueueMaxSduInfo_t *stQueueMaxSduInfo);
EXTERN_API RtkApiRet rtk_st_queue_max_sdu_get(UnitChip_t unitChip, StQueueMaxSduInfo_t *stQueueMaxSduInfo);

#endif /* RTKAS_API_ST_H__ */
