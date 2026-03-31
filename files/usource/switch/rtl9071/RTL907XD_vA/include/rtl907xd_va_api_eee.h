#ifndef DVA_RTK_API_EEE_H
#define DVA_RTK_API_EEE_H

#include "rtkas_common_types.h"
#include "rtl907xd_va_api.h"
#include "rtkas_api_eee.h"

#define DVA_EEE_USER_PORT_MIN DVA_RTK_USER_PORT_1ST
#define DVA_EEE_USER_PORT_MAX DVA_RTK_USER_PORT_8TH

#define DVA_EEE_TIMER_TP_CFG_BIT_LSP    (0u)
#define DVA_EEE_TIMER_TD_CFG_BIT_LSP    (8u)
#define DVA_EEE_TIMER_TR_CFG_BIT_LSP    (16u)
#define DVA_EEE_TIMER_TW_CFG_BIT_LSP    (24u)
#define DVA_EEE_LPI_REQ_TXRATE_BIT_LSP  (0u)
#define DVA_EEE_LPI_REQ_QEMPTY_BIT_LSP  (1u)
#define DVA_EEE_LPI_REQ_RXPAUSE_BIT_LSP (2u)

EXTERN_API RtkApiRet dva_eee_set_en(uint32 unit, uint32 cfg);
EXTERN_API RtkApiRet dva_eee_get_en(uint32 unit, uint32 *pCfg);
EXTERN_API RtkApiRet dva_eee_get_phy_func_sts(uint32 unit, RtkPort port, EeePhyStatus_t *pEeePhyStatus);
EXTERN_API RtkApiRet dva_eee_set_port_en(uint32 unit, RtkPort port, RtkEnable enPortEee);
EXTERN_API RtkApiRet dva_eee_get_port_en(uint32 unit, RtkPort port, RtkEnable *pEnPortEee);
EXTERN_API RtkApiRet dva_eee_get_port_func_sts(uint32 unit, RtkPort port, EeePortStatus_t *pEeeStatus);
EXTERN_API RtkApiRet dva_eee_set_timer_unit(uint32 unit, EeeTimerType_e type, EeeTimerUnit_e timerUnit);
EXTERN_API RtkApiRet dva_eee_get_timer_unit(uint32 unit, EeeTimerType_e type, EeeTimerUnit_e *pTimerUnit);
EXTERN_API RtkApiRet dva_eee_set_timer_value(uint32 unit, EeeTimerType_e type, EeeTimerValue_t eeeTimerValueSet);
EXTERN_API RtkApiRet dva_eee_get_timer_value(uint32 unit, EeeTimerType_e type, EeeTimerValue_t *pEeeTimerValueGet);
EXTERN_API RtkApiRet dva_eee_set_port_lpi_req(uint32 unit, RtkPort port, RtkEnable txRate, RtkEnable queueEmpty, RtkEnable rxPauseOnFrame);
EXTERN_API RtkApiRet dva_eee_get_port_lpi_req(uint32 unit, RtkPort port, RtkEnable *pTxRate, RtkEnable *pQueueEmpty, RtkEnable *pRxPauseOnFrame);
EXTERN_API RtkApiRet dva_eee_set_port_lpi_qtx_thr(uint32 unit, RtkPort port, uint32 lpiQtxThr);
EXTERN_API RtkApiRet dva_eee_get_port_lpi_qtx_thr(uint32 unit, RtkPort port, uint32 *pLpiQtxThr);
EXTERN_API RtkApiRet dva_eee_set_port_qtx_pri(uint32 unit, RtkPort port, uint32 queuePriCfg);
EXTERN_API RtkApiRet dva_eee_get_port_qtx_pri(uint32 unit, RtkPort port, uint32 *pQueuePriCfg);
EXTERN_API RtkApiRet dva_eee_set_port_wake(uint32 unit, RtkPort port, EeeWakeSet_e wakeSet);
EXTERN_API RtkApiRet dva_eee_get_port_wake(uint32 unit, RtkPort port, EeeWakeSet_e *pWakeSet);
#endif /* DVA_RTK_API_EEE_H */
