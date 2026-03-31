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

#ifndef CVA_RTK_API_ST_H__
#define CVA_RTK_API_ST_H__

#include "rtl907xc_va_api_tsn.h"
#include "rtkas_api_st.h"
typedef struct
{
    /* it does not help to use bit field; the size will be 8 byte due to alignment. */
    uint8  forcedInterval; /**< special case for specifeid queue (0 to 1) */
    uint8  eor;            /**< end of ring for specified queue (0 to 1) */
    uint8  boundary;       /**< boundary for specified queue (0 to 1) */
    uint32 timeInterval;   /**< consecutive gate open time for serial gate operation (0 to 0x1FFFFFFF) */
} cva_RtkStConsecutiveGateOpenEntry_t;

#define CVA_ST_PORT_TYPE_100BASE_T1       (0u)
#define CVA_ST_PORT_TYPE_START            (CVA_ST_PORT_TYPE_100BASE_T1)
#define CVA_ST_PORT_TYPE_10BASE_T1        (1u)
#define CVA_ST_PORT_TYPE_FAST_ETHERNET    (2u)
#define CVA_ST_PORT_TYPE_MII              (3u)
#define CVA_ST_PORT_TYPE_RMII             (4u)
#define CVA_ST_PORT_TYPE_RGMII            (5u)
#define CVA_ST_PORT_TYPE_SGMII            (6u)
#define CVA_ST_PORT_TYPE_RTPG_1000BASE_T1 (7u)
#define CVA_ST_PORT_TYPE_HISGMII          (8u)
#define CVA_ST_PORT_TYPE_USXGMII          (9u)
#define CVA_ST_PORT_TYPE_RTPG_100BASE_T1  (10u)
#define CVA_ST_PORT_TYPE_END              (CVA_ST_PORT_TYPE_RTPG_100BASE_T1)

#define CVA_ST_GATE_OPERATION_SET_GATE_STATE (0u)

#define CVA_ST_SUPPORT_LIST_MAX (128u)
#define CVA_ST_SUPPORT_LIST_MIN (2u)

#define CVA_NO_MACSEC    (0u)
#define CVA_MACSEC_IPG24 (1u)
#define CVA_MACSEC_IPG32 (2u)

/* per queue max sdu offset address*/
#define CVA_PERQUEUE_MAX_SDU_OFFSET (0x10u)

/* maximum value of time interval field in gate control list table*/
#define CVA_GCL_TABLE_MAX_TIME_INTERVAL     (0x1FFFFFFFu)
#define CVA_GCL_TABLE_TIME_INTERVAL_BITMASK (0x1FFFFFFFu)
#define CVA_GCL_TABLE_GATE_STATE_BITMASK    (0xFFu)
#define CVA_GCL_TABLE_MAX_GATE_STATE        (0xFFu)
/* in API */
EXTERN_API RtkApiRet cva_st_init(uint32 unit);
EXTERN_API RtkApiRet cva_st_oper_gate_state_get(uint32 unit, cva_RtkPort portId, uint32 *operGateState);
EXTERN_API RtkApiRet cva_st_admin_gate_config_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, RtkTsnCycleTime_t *cycleTime,
                                                  uint32 *cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 *controlListLength);
EXTERN_API RtkApiRet cva_st_oper_gate_config_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, RtkTsnCycleTime_t *cycleTime,
                                                 uint32 *cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 *controlListLength);
EXTERN_API RtkApiRet cva_st_byte_count_set(uint32 unit, cva_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG);
EXTERN_API RtkApiRet cva_st_byte_count_get(uint32 unit, cva_RtkPort portId, uint32 *byteCount);
EXTERN_API RtkApiRet cva_st_queue_max_sdu_set(uint32 unit, cva_RtkPort portId, const uint32 *queueMaxSdu);
EXTERN_API RtkApiRet cva_st_queue_max_sdu_get(uint32 unit, cva_RtkPort portId, uint32 queueMaxSdu[]);
EXTERN_API RtkApiRet cva_st_config_pending_get(uint32 unit, cva_RtkPort portId, uint32 *configPending);
EXTERN_API RtkApiRet cva_st_gate_enable_set(uint32 unit, cva_RtkPort portId, uint32 enable);
EXTERN_API RtkApiRet cva_st_gate_enable_get(uint32 unit, cva_RtkPort portId, uint32 *enable);
EXTERN_API RtkApiRet cva_st_gate_config_set(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension,
                                            const StGateControlListParam_t gateControlList[], uint32 controlListLength);
EXTERN_API RtkApiRet cva_st_admin_gate_state_set(uint32 unit, cva_RtkPort portId, uint32 adminGateState);
EXTERN_API RtkApiRet cva_st_admin_gate_state_get(uint32 unit, cva_RtkPort portId, uint32 *adminGateState);

#endif /* CVA_RTK_API_ST_H__ */