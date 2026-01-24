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

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api.h"
#include "rtkas_timer.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_flash.h"
#include "rtl907xd_va_asicdrv_ptp.h"
#include "rtl907xd_va_api_esm.h"

uint32 dva_g_ptpTimeConfigTimeout       = PTP_TIME_DIRECT_GET_TIMEOUT;
uint32 dva_g_ptpControlClkConfigTimeout = PTP_TIME_DIRECT_SET_TIMEOUT;
uint32 dva_g_ptpConfigMultipleTimeout   = PTP_TIME_GET_TIMEOUT;
uint32 dva_g_ptpConfigTimeout           = PTP_TIME_SET_TIMEOUT;
uint32 dva_g_ptpPortTXRXTSTimeout       = PTP_PORT_TX_RX_GET_TIMEOUT;
uint32 dva_g_ptpTaiConfigTimeout        = PTP_TAI_GET_TIME_TIMEOUT;

enum
{
    EM_PTP_DA_BUF_AS = 0,      /**< Destination MAC Address buffer for 802.1AS */
    EM_PTP_DA_BUF_1588,        /**< Destination MAC Address buffer for IEEE-1588 */
    EM_PTP_DA_BUF_CUSTOMIZED1, /**< Destination MAC Address buffer for customized1 */
    EM_PTP_DA_BUF_CUSTOMIZED2, /**< Destination MAC Address buffer for customized2 */
    EM_PTP_DA_BUF_CUSTOMIZED3, /**< Destination MAC Address buffer for customized3 */
    EM_PTP_DA_BUF_END
};

typedef struct
{
    PtpPortType_e intf;
    uint16        intTxLatency;
} dva_ptpIntfLatency_t;

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] timesel
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_time_direct_set(uint32 unit, uint8 timesel, PtpTs_t ts)
{
    uint32    u32data = 1u, waitcnt = 0u;
    uint32    timeoutCnt = (dva_g_ptpControlClkConfigTimeout >= DVA_PTP_POLLING_INTERVAL) ? (dva_g_ptpControlClkConfigTimeout / DVA_PTP_POLLING_INTERVAL) : 1u;
    RtkApiRet ret        = RT_ERR_OK;
    do
    {
        if(timesel > 4u)
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        (void)dva_reg_read(unit, DVA_PTP_CONTROL_AND_CLK_CONFIG, &u32data);
        (void)dva_reg_field_set(DVA_PTP_CONTROL_AND_CLK_CONFIG, DVA_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LSP, DVA_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LEN, 1u, &u32data);
        (void)dva_reg_field_set(DVA_PTP_CONTROL_AND_CLK_CONFIG, DVA_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_SET_LSP, DVA_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_SET_LEN, 1u, &u32data);
        (void)dva_reg_field_set(DVA_PTP_CONTROL_AND_CLK_CONFIG, DVA_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_MODE_LSP, DVA_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_MODE_LEN, 3u, &u32data);
        (void)dva_reg_field_set(DVA_PTP_CONTROL_AND_CLK_CONFIG, DVA_PTP_CONTROL_AND_CLK_CONFIG_TIME_CONFIG_SEL_LSP, DVA_PTP_CONTROL_AND_CLK_CONFIG_TIME_CONFIG_SEL_LEN, timesel, &u32data);
        (void)dva_reg_write(unit, DVA_PTP_TIME_CONFIG_NS, ts.nanoSec);
        (void)dva_reg_write(unit, DVA_PTP_TIME_CONFIG_S_0, ts.low32Sec);
        (void)dva_reg_write(unit, DVA_PTP_TIME_CONFIG_S_1, ts.up16Sec);

        (void)dva_reg_write(unit, DVA_PTP_CONTROL_AND_CLK_CONFIG, u32data);
        for(waitcnt = 0u; waitcnt < timeoutCnt; waitcnt++)
        {
            (void)dva_reg_read(unit, DVA_PTP_CONTROL_AND_CLK_CONFIG, &u32data);
            if((u32data & 0x10000u) == 0u)
            {
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DVA_PTP_POLLING_INTERVAL));
        }

        if(waitcnt >= timeoutCnt)
        {
            dva_esm_event_log_set(DVA_EM_EVENT_PTP_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            ret = -RT_ERR_PTP_CALL_API_FAILED;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get specified time of the domain
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] timesel Hardware time module: 1:Local, 2:Domain 0, 3:Domain 1, 4: Domain 2
 * @param[out] ts time
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT
 */
RtkApiRet dva_drv_ptp_time_get(uint32 unit, uint8 timesel, PtpTs_t *ts)
{
    uint32    data = 0, waitcnt = 0u;
    uint32    timeoutCnt = (dva_g_ptpConfigMultipleTimeout >= DVA_PTP_POLLING_INTERVAL) ? (dva_g_ptpConfigMultipleTimeout / DVA_PTP_POLLING_INTERVAL) : 1u;
    uint32    regval     = 0u;
    int32     apiret     = 0;
    RtkApiRet ret        = RT_ERR_OK;
    do
    {
        if(NULL == ts)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_reg_read(unit, DVA_PTP_CFG_001, &regval);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(0u != (regval & 0x8000u))
        {
            /*last accessing is ongoing*/
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        regval = ((uint32)1u << 15u) | ((uint32)DVA_PTP_CFG_TYPE_TIME << 16u) | (((uint32)DVA_PTP_CFG_TIM_GET << 24u)) | (uint32)timesel;
        apiret = dva_reg_write(unit, DVA_PTP_CFG_001, regval);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        for(waitcnt = 0; waitcnt < timeoutCnt; waitcnt++)
        {
            apiret = dva_reg_read(unit, DVA_PTP_CFG_001, &regval);
            if(RT_ERR_OK != apiret)
            {
                continue;
            }
            if(0u == (regval & 0x8000u))
            {
                (void)dva_reg_read(unit, DVA_PTP_CFG_002, &data);
                ts->up16Sec = (uint16)data;
                (void)dva_reg_read(unit, DVA_PTP_CFG_003, &data);
                ts->low32Sec = data;
                (void)dva_reg_read(unit, DVA_PTP_CFG_004, &data);
                ts->nanoSec = data;
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DVA_PTP_POLLING_INTERVAL));
        }

        if(waitcnt >= timeoutCnt)
        {
            dva_esm_event_log_set(DVA_EM_EVENT_PTP_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }

        if(((regval & 0x8000u) == 0u) && (RT_ERR_OK != apiret))
        {
            ret = -(RT_ERR_PTP_CALL_API_FAILED);
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_check_ptp_time_set_done(uint32 unit, uint32 regval)
{
    uint32    timeoutCnt = (dva_g_ptpConfigTimeout >= DVA_PTP_POLLING_INTERVAL) ? (dva_g_ptpConfigTimeout / DVA_PTP_POLLING_INTERVAL) : 1u;
    int32     apiret     = 0;
    RtkApiRet ret        = RT_ERR_OK;
    uint32    waitcnt    = 0u;
    uint32    regTmp     = regval;

    do
    {
        for(waitcnt = 0; waitcnt < timeoutCnt; waitcnt++)
        {
            apiret = dva_reg_read(unit, DVA_PTP_CFG_001, &regTmp);
            if(RT_ERR_OK != apiret)
            {
                continue;
            }
            if(0u == (regTmp & 0x8000u))
            {
                /*Done*/
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DVA_PTP_POLLING_INTERVAL));
        }
        if(waitcnt >= timeoutCnt)
        {
            dva_esm_event_log_set(DVA_EM_EVENT_PTP_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        if(((regTmp & 0x8000u) != 0u) && (RT_ERR_OK == apiret))
        {
            ret = -(RT_ERR_PTP_CALL_API_FAILED);
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set specified time domain to increase and decrease
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] timesel Hardware time module: 1:Local, 2:Domain 0, 3:Domain 1, 4: Domain 2
 * @param[in] timeoper DVA_PTP_CFG_TIM_DEC (decrease) and DVA_PTP_CFG_TIM_INC (increase)
 * @param[in] ts Time
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT
 */
RtkApiRet dva_drv_ptp_time_set(uint32 unit, uint8 timesel, uint8 timeoper, PtpTs_t const *const ts)
{
    uint32    regval = 0u;
    int32     apiret = 0;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        if(NULL == ts)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if((DVA_PTP_CFG_TIM_INC != timeoper) && (DVA_PTP_CFG_TIM_DEC != timeoper))
        {
            ret = -(RT_ERR_INPUT);
            continue;
        }

        apiret = dva_reg_read(unit, DVA_PTP_CFG_001, &regval);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        if(0u != (regval & 0x8000u))
        {
            /*last accessing is ongoing*/
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        CHK_FUN_CONTINUE((ret), (dva_reg_write(unit, DVA_PTP_CFG_002, (uint32)ts->up16Sec)));
        CHK_FUN_CONTINUE((ret), (dva_reg_write(unit, DVA_PTP_CFG_003, ts->low32Sec)));
        CHK_FUN_CONTINUE((ret), (dva_reg_write(unit, DVA_PTP_CFG_004, ts->nanoSec)));

        regval = ((uint32)1u << 15u) | ((uint32)1u << 14u) | ((uint32)DVA_PTP_CFG_TYPE_TIME << 16u) | (((uint32)timeoper << 24u)) | (uint32)timesel;
        apiret = dva_reg_write(unit, DVA_PTP_CFG_001, regval);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        CHK_FUN_CONTINUE((ret), (dva_drv_check_ptp_time_set_done(unit, regval)));
    } while(0u == 1u);

    return ret;
}

/*
 *ppsidx: 0 GPIOB1, 1:GPIOB2
 * dva_taicfg_t: configurations
 */
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ppsidx 0: GPIOB1, 1: GPIOB2
 * @param[in] cfg
 * @retval
 */
RtkApiRet dva_drv_ptp_tai_cfg_set(uint32 unit, uint8 ppsidx, dva_taicfg_t cfg)
{
    uint32    taicfg0 = 0u, regvalue = 0u;
    uint32    func_sel = 0u, func_sel_len = 0u;
    uint32    clk_sel = 0u, clk_sel_len = 0u;
    uint32    gene_mode = 0u, gene_mode_len = 0u;
    uint32    dc = 0u, dc_len = 0u;
    uint32    amt = 0u, amt_len = 0u;
    uint32    amt_unit = 0u, amt_unit_len = 0u;
    uint32    capture_det = 0u, capture_det_len = 0u;
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if((0u) == ppsidx)
        {
            taicfg0         = DVA_TAI_0_CFG_0;
            func_sel        = DVA_TAI_0_CFG_0_TAI0_FUNCTION_SEL_LSP;
            clk_sel         = DVA_TAI_0_CFG_0_TAI0_CLOCK_SEL_LSP;
            gene_mode       = DVA_TAI_0_CFG_0_TAI0_TRIG_MOD_SEL_LSP;
            dc              = DVA_TAI_0_CFG_0_TAI0_PULSE_DC_LSP;
            amt             = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_LSP;
            amt_unit        = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_UNIT_LSP;
            capture_det     = DVA_TAI_0_CFG_0_TAI0_EVNT_RF_DET_LSP;
            func_sel_len    = DVA_TAI_0_CFG_0_TAI0_FUNCTION_SEL_LEN;
            clk_sel_len     = DVA_TAI_0_CFG_0_TAI0_CLOCK_SEL_LEN;
            gene_mode_len   = DVA_TAI_0_CFG_0_TAI0_TRIG_MOD_SEL_LEN;
            dc_len          = DVA_TAI_0_CFG_0_TAI0_PULSE_DC_LEN;
            amt_len         = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_LEN;
            amt_unit_len    = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_UNIT_LEN;
            capture_det_len = DVA_TAI_0_CFG_0_TAI0_EVNT_RF_DET_LEN;
        }
        else if((1u) == ppsidx)
        {
            taicfg0         = DVA_TAI_1_CFG_0;
            func_sel        = DVA_TAI_1_CFG_0_TAI1_FUNCTION_SEL_LSP;
            clk_sel         = DVA_TAI_1_CFG_0_TAI1_CLOCK_SEL_LSP;
            gene_mode       = DVA_TAI_1_CFG_0_TAI1_TRIG_MOD_SEL_LSP;
            dc              = DVA_TAI_1_CFG_0_TAI1_PULSE_DC_LSP;
            amt             = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_LSP;
            amt_unit        = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_UNIT_LSP;
            capture_det     = DVA_TAI_1_CFG_0_TAI1_EVNT_RF_DET_LSP;
            func_sel_len    = DVA_TAI_1_CFG_0_TAI1_FUNCTION_SEL_LEN;
            clk_sel_len     = DVA_TAI_1_CFG_0_TAI1_CLOCK_SEL_LEN;
            gene_mode_len   = DVA_TAI_1_CFG_0_TAI1_TRIG_MOD_SEL_LEN;
            dc_len          = DVA_TAI_1_CFG_0_TAI1_PULSE_DC_LEN;
            amt_len         = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_LEN;
            amt_unit_len    = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_UNIT_LEN;
            capture_det_len = DVA_TAI_1_CFG_0_TAI1_EVNT_RF_DET_LEN;
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        (void)dva_reg_field_set(taicfg0, func_sel, func_sel_len, cfg.func_sel, &regvalue);
        (void)dva_reg_field_set(taicfg0, clk_sel, clk_sel_len, cfg.clk_sel, &regvalue);
        (void)dva_reg_field_set(taicfg0, gene_mode, gene_mode_len, cfg.gene_mode, &regvalue);
        (void)dva_reg_field_set(taicfg0, dc, dc_len, cfg.dc, &regvalue);
        (void)dva_reg_field_set(taicfg0, amt, amt_len, cfg.amt, &regvalue);
        (void)dva_reg_field_set(taicfg0, amt_unit, amt_unit_len, cfg.amt_unit, &regvalue);
        (void)dva_reg_field_set(taicfg0, capture_det, capture_det_len, cfg.capture_det, &regvalue);
        CHK_FUN_CONTINUE((ret), (dva_reg_write(unit, taicfg0, regvalue)));
    } while(0u == 1u);

    return ret;
}

/*
 *ppsidx: 0 GPIOB1, 1:GPIOB2
 * dva_taicfg_t: configurations
 * for demo capture cfg:
 * func_sel=2 (event capture)
 * capture_det=0 (detection of a rising edge)
 * clk_sel depends on which DUT.
 * gene_mode = 0
 * dc = 0
 * amt = 0
 * amt_unit = 0
 */

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ppsidx 0: GPIOB1, 1: GPIOB2
 * @param[in] cfg
 * @retval
 */
RtkApiRet dva_drv_ptp_tai_cfg_get(uint32 unit, uint8 ppsidx, dva_taicfg_t *cfg)
{
    uint32    taicfg0 = 0u, regvalue = 0u;
    uint32    func_sel = 0u, func_sel_len = 0u;
    uint32    clk_sel = 0u, clk_sel_len = 0u;
    uint32    gene_mode = 0u, gene_mode_len = 0u;
    uint32    dc = 0u, dc_len = 0u;
    uint32    amt = 0u, amt_len = 0u;
    uint32    amt_unit = 0u, amt_unit_len = 0u;
    uint32    capture_det = 0u, capture_det_len = 0u;
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == cfg)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if((0u) == ppsidx)
        {
            taicfg0         = DVA_TAI_0_CFG_0;
            func_sel        = DVA_TAI_0_CFG_0_TAI0_FUNCTION_SEL_LSP;
            clk_sel         = DVA_TAI_0_CFG_0_TAI0_CLOCK_SEL_LSP;
            gene_mode       = DVA_TAI_0_CFG_0_TAI0_TRIG_MOD_SEL_LSP;
            dc              = DVA_TAI_0_CFG_0_TAI0_PULSE_DC_LSP;
            amt             = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_LSP;
            amt_unit        = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_UNIT_LSP;
            capture_det     = DVA_TAI_0_CFG_0_TAI0_EVNT_RF_DET_LSP;
            func_sel_len    = DVA_TAI_0_CFG_0_TAI0_FUNCTION_SEL_LEN;
            clk_sel_len     = DVA_TAI_0_CFG_0_TAI0_CLOCK_SEL_LEN;
            gene_mode_len   = DVA_TAI_0_CFG_0_TAI0_TRIG_MOD_SEL_LEN;
            dc_len          = DVA_TAI_0_CFG_0_TAI0_PULSE_DC_LEN;
            amt_len         = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_LEN;
            amt_unit_len    = DVA_TAI_0_CFG_0_TAI0_PULSE_AMOUNT_UNIT_LEN;
            capture_det_len = DVA_TAI_0_CFG_0_TAI0_EVNT_RF_DET_LEN;
        }
        else if((1u) == ppsidx)
        {
            taicfg0         = DVA_TAI_1_CFG_0;
            func_sel        = DVA_TAI_1_CFG_0_TAI1_FUNCTION_SEL_LSP;
            clk_sel         = DVA_TAI_1_CFG_0_TAI1_CLOCK_SEL_LSP;
            gene_mode       = DVA_TAI_1_CFG_0_TAI1_TRIG_MOD_SEL_LSP;
            dc              = DVA_TAI_1_CFG_0_TAI1_PULSE_DC_LSP;
            amt             = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_LSP;
            amt_unit        = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_UNIT_LSP;
            capture_det     = DVA_TAI_1_CFG_0_TAI1_EVNT_RF_DET_LSP;
            func_sel_len    = DVA_TAI_1_CFG_0_TAI1_FUNCTION_SEL_LEN;
            clk_sel_len     = DVA_TAI_1_CFG_0_TAI1_CLOCK_SEL_LEN;
            gene_mode_len   = DVA_TAI_1_CFG_0_TAI1_TRIG_MOD_SEL_LEN;
            dc_len          = DVA_TAI_1_CFG_0_TAI1_PULSE_DC_LEN;
            amt_len         = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_LEN;
            amt_unit_len    = DVA_TAI_1_CFG_0_TAI1_PULSE_AMOUNT_UNIT_LEN;
            capture_det_len = DVA_TAI_1_CFG_0_TAI1_EVNT_RF_DET_LEN;
        }
        else
        {

            ret = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE((ret), (dva_reg_read(unit, taicfg0, &regvalue)));
        (void)dva_reg_field_get(taicfg0, func_sel, func_sel_len, &cfg->func_sel, regvalue);
        (void)dva_reg_field_get(taicfg0, clk_sel, clk_sel_len, &cfg->clk_sel, regvalue);
        (void)dva_reg_field_get(taicfg0, gene_mode, gene_mode_len, &cfg->gene_mode, regvalue);
        (void)dva_reg_field_get(taicfg0, dc, dc_len, &cfg->dc, regvalue);
        (void)dva_reg_field_get(taicfg0, amt, amt_len, &cfg->amt, regvalue);
        (void)dva_reg_field_get(taicfg0, amt_unit, amt_unit_len, &cfg->amt_unit, regvalue);
        (void)dva_reg_field_get(taicfg0, capture_det, capture_det_len, &cfg->capture_det, regvalue);
    } while(0u == 1u);
    return ret;
}

/**
 * @brief
 *
 * @param[in] ppsidx
 * @retval
 */
RtkApiRet dva_drv_ptp_tai_run(uint32 unit, uint8 ppsidx)
{
    uint32    taicfg0 = 0u, regvalue = 0u;
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if((0u) == ppsidx)
        {
            taicfg0 = DVA_TAI_0_CFG_0;
        }
        else if((1u) == ppsidx)
        {
            taicfg0 = DVA_TAI_1_CFG_0;
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        (void)dva_reg_read(unit, taicfg0, &regvalue);
        CHK_FUN_CONTINUE((ret), dva_reg_write(unit, taicfg0, (regvalue | (uint32)0x100u)));
    } while(0u == 1u);

    return ret;
}

/**
 * ishwRunning 1: Running, 0:done;
 */
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ppsidx
 * @param[in] ishwRunning 1: Running, 0: done
 * @retval
 */
RtkApiRet dva_drv_ptp_tai_done(uint32 unit, uint8 ppsidx, uint8 *ishwRunning)
{
    uint32    taicfg0 = 0u, cfgset = 0u, cfgset_len = 0u, regvalue = 0u;
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == ishwRunning)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if((0u) == ppsidx)
        {
            taicfg0    = DVA_TAI_0_CFG_0;
            cfgset     = DVA_TAI_0_CFG_0_TAI0_CONFIG_SET_LSP;
            cfgset_len = DVA_TAI_0_CFG_0_TAI0_CONFIG_SET_LEN;
        }
        else if((1u) == ppsidx)
        {
            taicfg0    = DVA_TAI_1_CFG_0;
            cfgset     = DVA_TAI_1_CFG_0_TAI1_CONFIG_SET_LSP;
            cfgset_len = DVA_TAI_1_CFG_0_TAI1_CONFIG_SET_LEN;
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE((ret), (dva_reg_field_read(unit, taicfg0, cfgset, cfgset_len, &regvalue)));
        *ishwRunning = (uint8)(regvalue & 0x1u);
    } while(0u == 1u);
    return ret;
}

/**
ppsidx: 0 GPIOB1, 1:GPIOB2
Need to excute dva_drv_ptp_tai_done and check ishwRunning parameter before dva_drv_ptp_tai_get_time.
**/
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ppsidx 0: GPIOB1, 1: GPIOB2
 * @param[in] pts
 * @retval
 */
RtkApiRet dva_drv_ptp_tai_get_time(uint32 unit, uint8 ppsidx, PtpTs_t *pts)
{
    uint32    taicfg0 = 0u, taits_s0 = 0u, taits_s1 = 0u, taits_ns = 0u, vlded = 0u, regvalue = 0u, waitCnt = 0u;
    uint32    timeoutCnt = (dva_g_ptpTaiConfigTimeout >= DVA_PTP_POLLING_INTERVAL) ? (dva_g_ptpTaiConfigTimeout / DVA_PTP_POLLING_INTERVAL) : 1u;
    RtkApiRet ret        = RT_ERR_OK;
    do
    {
        if(NULL == pts)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }
        if((0u) == ppsidx)
        {
            taicfg0  = DVA_TAI_0_CFG_0;
            taits_s0 = DVA_TAI0_TIMESTAMP_S_0;
            taits_s1 = DVA_TAI0_TIMESTAMP_S_1;
            taits_ns = DVA_TAI0_TIMESTAMP_NS;
            vlded    = ((uint32)1u) << 8u;
        }
        else if((1u) == ppsidx)
        {
            taicfg0  = DVA_TAI_1_CFG_0;
            taits_s0 = DVA_TAI1_TIMESTAMP_S_0;
            taits_s1 = DVA_TAI1_TIMESTAMP_S_1;
            taits_ns = DVA_TAI1_TIMESTAMP_NS;
            vlded    = ((uint32)1u) << 9u;
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        (void)dva_reg_read(unit, taicfg0, &regvalue);
        (void)dva_reg_write(unit, taicfg0, (regvalue | (uint32)0x100u));
        /*for-loop to check hw fetch time done*/
        for(waitCnt = 0u; waitCnt < timeoutCnt; waitCnt++)
        {
            (void)dva_reg_read(unit, taicfg0, &regvalue);
            if((regvalue & (uint32)0x100u) == (uint32)0u)
            {
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DVA_PTP_POLLING_INTERVAL));
        }
        if(waitCnt >= timeoutCnt)
        {
            dva_esm_event_log_set(DVA_EM_EVENT_PTP_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        (void)dva_reg_read(unit, DVA_TAI_GPIO_FUNC_CFG, &regvalue);
        /*HW done but not vld truns on.*/
        if((regvalue & vlded) == 0u)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        (void)dva_reg_read(unit, taits_ns, &pts->nanoSec);
        (void)dva_reg_read(unit, taits_s0, &pts->low32Sec);
        CHK_FUN_CONTINUE((ret), (dva_reg_read(unit, taits_s1, &regvalue)));
        pts->up16Sec = (uint16)regvalue;
    } while(0u == 1u);

    return ret;
}

/*
 *ppsidx: 0 GPIOB1, 1:GPIOB2
 * isnotified: 0: None, others(non-zero) are notified
 */
/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ppsidx 0: GPIOB1, 1: GPIOB2
 * @param[in] isnotified 0: None, others(non-zero) are notified
 * @retval
 */
RtkApiRet dva_drv_ptp_tai_notified(uint32 unit, uint8 ppsidx, uint8 *isnotified)
{
    uint32    notified = 0u, regvalue = 0u;
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == isnotified)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }
        if((0u) == ppsidx)
        {
            notified = ((uint32)1u) << 2u;
        }
        else if((1u) == ppsidx)
        {
            notified = ((uint32)1u) << 6u;
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE((ret), (dva_reg_read(unit, DVA_TAI_GPIO_FUNC_CFG, &regvalue)));
        *isnotified = (uint8)(regvalue & notified);
    } while(0u == 1u);

    return ret;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domain
 * @param[in] msgtype
 * @param[in] txrxsel
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_get_port_tx_rx(uint32 unit, RtkPort port, uint8 domain, uint8 msgtype, uint8 txrxsel, PtpTs_t *ts)
{
    uint32 u32data = 0u, waitcnt = 0u, timevalue = 0u;
    uint32 timeoutCnt = (dva_g_ptpPortTXRXTSTimeout >= DVA_PTP_POLLING_INTERVAL) ? (dva_g_ptpPortTXRXTSTimeout / DVA_PTP_POLLING_INTERVAL) : 1u;

    RtkApiRet ret = -RT_ERR_PTP_CALL_API_FAILED;
    do
    {
        if(DVA_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == ts)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if((domain > 2u) || (msgtype > 3u) || (txrxsel > 1u))
        {
            ret = -(RT_ERR_INPUT);
            continue;
        }
        (void)dva_reg_field_read(unit, DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_TXRX_TS_CONTROL_TRXTS_OVERWR_EN_LSP, DVA_PORT0_TXRX_TS_CONTROL_TRXTS_OVERWR_EN_LEN, &u32data);
        (void)dva_reg_field_set(DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_TXRX_TS_CONTROL_DOMAIN_SELECT_LSP, DVA_PORT0_TXRX_TS_CONTROL_DOMAIN_SELECT_LEN, (uint32)domain, &u32data);
        (void)dva_reg_field_set(DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_TXRX_TS_CONTROL_TRXTS_MSGTYPE_SEL_LSP, DVA_PORT0_TXRX_TS_CONTROL_TRXTS_MSGTYPE_SEL_LEN, (uint32)msgtype, &u32data);
        (void)dva_reg_field_set(DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_TXRX_TS_CONTROL_TRXTS_SEL_LSP, DVA_PORT0_TXRX_TS_CONTROL_TRXTS_SEL_LEN, (uint32)txrxsel, &u32data);
        (void)dva_reg_field_set(DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_TXRX_TS_CONTROL_TRXTS_RD_LSP, DVA_PORT0_TXRX_TS_CONTROL_TRXTS_RD_LEN, 1u, &u32data);
        (void)dva_reg_write(unit, DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), u32data);
        for(waitcnt = 0; waitcnt < timeoutCnt; waitcnt++)
        {
            (void)dva_reg_read(unit, DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), &u32data);
            if((u32data & 0x40u) == 0u)
            {
                (void)dva_reg_read(unit, DVA_PORT0_TXRX_TS_LOCAL_SECOND1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), &timevalue);
                ts[0].up16Sec = (uint16)timevalue;
                (void)dva_reg_read(unit, DVA_PORT0_TXRX_TS_LOCAL_SECOND0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), &timevalue);
                ts[0].low32Sec = timevalue;
                (void)dva_reg_read(unit, DVA_PORT0_TXRX_TS_LOCAL_NEROSECOND + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), &timevalue);
                ts[0].nanoSec = timevalue;
                (void)dva_reg_read(unit, DVA_PORT0_TXRX_TS_SLAVE_SECOND1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), &timevalue);
                ts[1].up16Sec = (uint16)timevalue;
                (void)dva_reg_read(unit, DVA_PORT0_TXRX_TS_SLAVE_SECOND0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), &timevalue);
                ts[1].low32Sec = timevalue;
                (void)dva_reg_read(unit, DVA_PORT0_TXRX_TS_SLAVE_NEROSECOND + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), &timevalue);
                ts[1].nanoSec = timevalue;
                ret           = RT_ERR_OK;
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DVA_PTP_POLLING_INTERVAL));
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] ptrxts_overwr_en
 * @retval
 */
RtkApiRet dva_drv_ptp_get_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 *ptrxts_overwr_en)
{
    uint32    u32data = 0u;
    RtkApiRet ret     = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == ptrxts_overwr_en)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE((ret), (dva_reg_field_read(unit, DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_TXRX_TS_CONTROL_TRXTS_OVERWR_EN_LSP, DVA_PORT0_TXRX_TS_CONTROL_TRXTS_OVERWR_EN_LEN, &u32data)));
        *ptrxts_overwr_en = (uint8)u32data;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] trxts_overwr_en
 * @retval
 */
RtkApiRet dva_drv_ptp_set_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 trxts_overwr_en)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE((ret), (dva_reg_field_write(unit, DVA_PORT0_TXRX_TS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_TXRX_TS_CONTROL_TRXTS_OVERWR_EN_LSP, DVA_PORT0_TXRX_TS_CONTROL_TRXTS_OVERWR_EN_LEN, (uint32)trxts_overwr_en)));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_rx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_SYNC_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_SYNC_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_SYNC_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
            /*This statement won't happend. The code flow check domainid at the beginning.*/
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_rx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_FOLLOW_UP_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_FOLLOW_UP_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_FOLLOW_UP_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
            /*This statement won't happend. The code flow check domainid at the beginning.*/
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_rx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PDELAY_REQUEST_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PDELAY_REQUEST_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PDELAY_REQUEST_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_tx_presonse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_RX_PDELAY_RESPONSE_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PDELAY_RESPONSE_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PDELAY_RESPONSE_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_rx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_RX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_tx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -(RT_ERR_PTP_CALL_API_FAILED);
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_TX_SYNC_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_SYNC_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_SYNC_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_get_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_read(unit, DVA_PORT0_TX_ONE_STEP_SYNC_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_read(unit, DVA_PORT0_TX_ONE_STEP_SYNC_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_read(unit, DVA_PORT0_TX_ONE_STEP_SYNC_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_TX_ONE_STEP_SYNC_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_ONE_STEP_SYNC_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_ONE_STEP_SYNC_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_tx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_TX_FOLLOW_UP_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_FOLLOW_UP_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_FOLLOW_UP_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_tx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_TX_PDELAY_REQUEST_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_PDELAY_REQUEST_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_PDELAY_REQUEST_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_tx_presponse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_TX_PDELAY_RESPONSE_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_PDELAY_RESPONSE_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_PDELAY_RESPONSE_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_tx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_TX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_TX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_sync_receipt_timeout_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_SYNC_RECEIPT_TIMEOUT_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_SYNC_RECEIPT_TIMEOUT_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_SYNC_RECEIPT_TIMEOUT_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_rx_ptp_packet_discard_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }
        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_RX_PTP_PACKET_DISCARD_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PTP_PACKET_DISCARD_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_RX_PTP_PACKET_DISCARD_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] mib
 * @retval
 */
RtkApiRet dva_drv_ptp_mib_set_pdelay_allowed_lost_response_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }
        if(2u == domainid)
        {
            CHK_FUN_CONTINUE((retval), (dva_reg_write(unit, DVA_PORT0_PDELAY_ALLOWED_LOST_RESPONSE_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_PDELAY_ALLOWED_LOST_RESPONSE_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            apiret = dva_reg_write(unit, DVA_PORT0_PDELAY_ALLOWED_LOST_RESPONSE_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]), mib);
        }
        else
        {
            /* intentionally left blank */
        }
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_expLog_loss_of_sync_get(uint32 unit, uint32 domainid, PtpTs_t *ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u;
    uint32    cfg31_0 = 0u;
    int32     apiret  = RT_ERR_OK;
    do
    {
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid), &cfg15_0)));
        ts->up16Sec = cfg15_0;
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 4u, &cfg31_0)));
        ts->low32Sec = cfg31_0;
        apiret       = dva_drv_indirect_sram_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 8u, &cfg31_0);
        ts->nanoSec  = cfg31_0;
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_set_expLog_loss_of_sync(uint32 unit, uint32 domainid, PtpTs_t const *const ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid), ts->up16Sec)));

        cfg15_0  = (uint16)(ts->low32Sec);
        cfg31_16 = (uint16)(ts->low32Sec >> 16u);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 4u, cfg15_0)));
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 4u + 2u, cfg31_16)));

        cfg15_0  = (uint16)(ts->nanoSec);
        cfg31_16 = (uint16)(ts->nanoSec >> 16u);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 4u + 8u, cfg15_0)));
        apiret = dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 4u + 8u + 2u, cfg31_16);

        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_expLog_link_dn_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu), &cfg15_0)));
        ts->up16Sec = cfg15_0;
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u, &cfg15_0)));
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 2u, &cfg31_16)));
        ts->low32Sec = (uint32)cfg15_0 | ((uint32)cfg31_16 << 16u);

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u, &cfg15_0)));
        apiret      = dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u + 2u, &cfg31_16);
        ts->nanoSec = (uint32)cfg15_0 | ((uint32)cfg31_16 << 16u);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_set_explog_link_dn_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu), ts->up16Sec)));

        cfg15_0  = (uint16)ts->low32Sec;
        cfg31_16 = (uint16)(ts->low32Sec >> 16);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u, cfg15_0)));
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 2u, cfg31_16)));

        cfg15_0  = (uint16)ts->nanoSec;
        cfg31_16 = (uint16)(ts->nanoSec >> 16);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u, cfg15_0)));
        apiret = dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xCu + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u + 2u, cfg31_16);

        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_expLog_link_up_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu), &cfg15_0)));
        ts->up16Sec = cfg15_0;
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u, &cfg15_0)));
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 2u, &cfg31_16)));
        ts->low32Sec = (uint32)cfg15_0 | ((uint32)cfg31_16 << 16u);

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u, &cfg15_0)));
        apiret      = dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u + 2u, &cfg31_16);
        ts->nanoSec = (uint32)cfg15_0 | ((uint32)cfg31_16 << 16u);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_set_explog_link_up_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu), ts->up16Sec)));

        cfg15_0  = (uint16)(ts->low32Sec);
        cfg31_16 = (uint16)(ts->low32Sec >> 16);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u, cfg15_0)));
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 2u, cfg31_16)));

        cfg15_0  = (uint16)(ts->nanoSec);
        cfg31_16 = (uint16)(ts->nanoSec >> 16);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u, cfg15_0)));
        apiret = dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0xC0u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u + 2u, cfg31_16);

        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_get_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu), &cfg15_0)));
        ts->up16Sec = cfg15_0;
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u, &cfg15_0)));
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 2u, &cfg31_16)));
        ts->low32Sec = (uint32)cfg15_0 | ((uint32)cfg31_16 << 16);

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u, &cfg15_0)));
        apiret      = dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u + 2u, &cfg31_16);
        ts->nanoSec = (uint32)cfg15_0 | ((uint32)cfg31_16 << 16);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] domainid
 * @param[in] ts
 * @retval
 */
RtkApiRet dva_drv_ptp_set_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts)
{
    RtkApiRet retval  = -(RT_ERR_PTP_CALL_API_FAILED);
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        if(NULL == ts)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu), ts->up16Sec)));

        cfg15_0  = (uint16)(ts->low32Sec);
        cfg31_16 = (uint16)(ts->low32Sec >> 16);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u, cfg15_0)));
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 2u, cfg31_16)));

        cfg15_0  = (uint16)(ts->nanoSec);
        cfg31_16 = (uint16)(ts->nanoSec >> 16);
        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u, cfg15_0)));
        apiret = dva_drv_indirect_sram_2bytes_set(unit, DVA_PTPCFG_EXPLOG + (0x228u * domainid) + 0x174u + ((uint32)dva_g_userPort2sysPort[port] * 0xCu) + 4u + 8u + 2u, cfg31_16);

        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] u8data
 * @retval
 */
RtkApiRet dva_drv_ptp_get_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 *u8data)
{
    RtkApiRet retval = -(RT_ERR_PTP_CALL_API_FAILED);
    uint8     cfg;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == u8data)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_PTPCFG_SYNCRXTIMOUTTXSYNCEN + (uint32)dva_g_userPort2sysPort[port], &cfg);
        if((int32)RT_ERR_OK != apiret)
        {
            continue;
        }
        *u8data = (uint8)cfg;
        retval  = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] u8data
 * @retval
 */
RtkApiRet dva_drv_ptp_set_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 u8data)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            retval = -(RT_ERR_PORT_ID);
            continue;
        }
        apiret = dva_drv_indirect_sram_set(unit, DVA_PTPCFG_SYNCRXTIMOUTTXSYNCEN + (uint32)dva_g_userPort2sysPort[port], u8data);
        if((int32)RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

static RtkApiRet dva_drv_ptp_portmask_converter(dva_RtkPmap srcPmap, uint16 *pmask)
{
    RtkApiRet   retval  = RT_ERR_OK;
    int32       apiret  = RT_ERR_OK;
    dva_RtkPmap dstPmap = 0u;

    do
    {
        if((srcPmap != 0u) && (srcPmap != 0x7FFFu))
        {
            apiret = dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_SYS_TO_USER);
            if(RT_ERR_OK != apiret)
            {
                retval = -RT_ERR_PTP_CALL_API_FAILED;
                continue;
            }
            *pmask = (uint16)dstPmap;
        }
        else if(srcPmap == 0x7FFFu)
        {
            dstPmap = DVA_RTK_USER_PMAP_ALL;
            *pmask  = (uint16)dstPmap;
        }
        else
        {
            *pmask = 0u;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get protocolPortmask
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] domainid domain index
 * @param[out] portmask
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_protocolPortmask_get(uint32 unit, uint32 domainid, uint16 *portmask)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    uint16    cfg16  = 0u;
    do
    {
        if(NULL == portmask)
        {
            retval = -(RT_ERR_NULL_POINTER);
            continue;
        }
        portmask[0] = 0u;
        portmask[1] = 0u;
        portmask[2] = 0u;

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_PORTENABLEMASK, &cfg16)));
        CHK_FUN_CONTINUE((retval), (dva_drv_ptp_portmask_converter(cfg16, &portmask[0])));

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_ASCAPABLEMASK + (domainid * 2u), &cfg16)));
        CHK_FUN_CONTINUE((retval), (dva_drv_ptp_portmask_converter(cfg16, &portmask[1])));

        CHK_FUN_CONTINUE((retval), (dva_drv_indirect_sram_2bytes_get(unit, DVA_PTPCFG_CALDELAYENMASK, &cfg16)));
        CHK_FUN_CONTINUE((retval), (dva_drv_ptp_portmask_converter(cfg16, &portmask[2])));

        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get 1-Step transmite enable status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port: 1,2,...11
 * @param[in] domainid
 * @param[in] oneStepTxSync 0: Disable, 1: Enable
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet dva_drv_ptp_onesteptxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepTxSync)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = -RT_ERR_OK;
    uint8     cfg    = 0u;
    do
    {
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == oneStepTxSync)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_PTPCFG_ONESTEPTXSYNC + (((uint32)dva_g_userPort2sysPort[port] * (uint32)3u) + domainid), &cfg);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *oneStepTxSync = cfg;
    } while(0u == 1u);
    return ret;
}

static int8 dva_drv_ptp_ts_cmp_ns(const PtpTs_t *a, const PtpTs_t *b)
{
    int8 ret = (int8)0;
    do
    {
        if(a->nanoSec > b->nanoSec)
        {
            ret = (int8)1;
            continue;
        }
        else if(a->nanoSec < b->nanoSec)
        {
            ret = (int8)-1;
            continue;
        }
        else
        {
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Compare two timestamp a,b.
 * @param[in] a 0,1,2,...14
 * @param[in] b 0,1,2
 * @param[out] None
 * @retval  1: a > b
 * @retval -1: b > a
 * @retval  0: a = b, a or b is NULL pointer
 */
int8 dva_drv_ptp_ts_cmp(const PtpTs_t *a, const PtpTs_t *b)
{
    int8 ret = (int8)0;
    /* if a>b return  1 */
    /* if a<b return -1 */
    /* if a=b return  0 */
    do
    {
        if((NULL == a) || (NULL == b))
        {
            continue;
        }
        if(a->up16Sec > b->up16Sec)
        {
            ret = (int8)1;
            continue;
        }
        else if(a->up16Sec < b->up16Sec)
        {
            ret = (int8)-1;
            continue;
        }
        else
        {
            if(a->low32Sec > b->low32Sec)
            {
                ret = (int8)1;
                continue;
            }
            else if(a->low32Sec < b->low32Sec)
            {
                ret = (int8)-1;
                continue;
            }
            else
            {
                ret = dva_drv_ptp_ts_cmp_ns(a, b);
                continue;
            }
        }
    } while(0u == 1u);
    return ret;
}

void dva_drv_ptp_ts_trim_sub(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime)
{
    uint8 borrowFlag = 0u;

    do
    {
        if((DVA_PTP_STAMP_SUB == modiType) && ((NULL != oriTime) && (NULL != subTime)))
        {
            if(oriTime->nanoSec < subTime->nanoSec)
            {
                if(oriTime->low32Sec == 0u)
                {
                    if(oriTime->up16Sec != 0u)
                    {
                        oriTime->up16Sec--;
                        oriTime->low32Sec = 0xFFFFFFFFu;
                    }
                    else
                    {
                        continue; /* oops , there is an error */
                    }
                }
                else
                {
                    oriTime->low32Sec--;
                }
                borrowFlag = 1u;
            }
            else
            {
            }
            if(0u != borrowFlag)
            {
                borrowFlag = 0u;
                oriTime->nanoSec += DVA_PTP_STAMP_MAX_NS;
            }
            oriTime->nanoSec -= subTime->nanoSec;
            /* second */
            if(oriTime->low32Sec < subTime->low32Sec)
            {
                if(oriTime->up16Sec != 0u)
                {
                    oriTime->up16Sec--;
                }
                else
                {
                    continue; /* oops , there is an error */
                }
                borrowFlag = 1u;
            }
            if(0u != borrowFlag)
            {
                /*We do not have to clear borrowFlag.borrowFlag = 0u;*/
                subTime->low32Sec = (0xFFFFFFFFu - subTime->low32Sec);
                oriTime->low32Sec += (subTime->low32Sec + 1u);
            }
            else
            {
                oriTime->low32Sec -= subTime->low32Sec;
            }
            /* epoch */
            if(oriTime->up16Sec < subTime->up16Sec)
            {
                continue; /* oops , there is an error */
            }
            else
            {
                oriTime->up16Sec -= subTime->up16Sec;
            }
        }
        else
        {
            /* oops */
        }
    } while(0u == 1u);
}

/**
 * @brief Timestamp operation: DVA_PTP_STAMP_SUB (Sub), DVA_PTP_STAMP_ADD (Add), DVA_PTP_STAMP_COPY (Copy), DVA_PTP_STAMP_RESET (Reset).
 *      Save result into oriTime.
 *      oriTime = oriTime - subTime  (Sub, oriTime > subTime)
 *      oriTime = oriTime + subTime  (Add, oriTime > subTime)
 *      oriTime = subTime (Copy)
 *      oriTime = 0 (Reset)
 * @param[in] modiType DVA_PTP_STAMP_SUB, DVA_PTP_STAMP_COPY, DVA_PTP_STAMP_RESET
 * @param[in] oriTime timestap
 * @param[in] subTime timestap
 * @param[out] oriTime result
 * @retval None
 */
void dva_drv_ptp_ts_trim(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime)
{
    uint8  borrowFlag  = 0u;
    uint32 assistTemp1 = 0u;
    do
    {
        if((DVA_PTP_STAMP_SUB == modiType) && ((NULL != oriTime) && (NULL != subTime)))
        {
            dva_drv_ptp_ts_trim_sub(modiType, oriTime, subTime);
        }
        else if(DVA_PTP_STAMP_ADD == modiType)
        {
            oriTime->nanoSec += subTime->nanoSec;
            if(oriTime->nanoSec >= DVA_PTP_STAMP_MAX_NS)
            {
                borrowFlag = 1u;
                oriTime->nanoSec -= DVA_PTP_STAMP_MAX_NS;
            }

            assistTemp1 = oriTime->low32Sec + subTime->low32Sec;
            if((0xFFFFFFFFu - subTime->low32Sec) < oriTime->low32Sec)
            {
                oriTime->up16Sec += 1u;
            }
            else
            {
                if((0xFFFFFFFFu - (uint32)borrowFlag) < assistTemp1)
                {
                    oriTime->up16Sec += 1u;
                }
            }
            oriTime->low32Sec = (assistTemp1 + (uint32)borrowFlag);
            oriTime->up16Sec += subTime->up16Sec;
        }
        else if((DVA_PTP_STAMP_COPY == modiType) && ((NULL != oriTime) && (NULL != subTime)))
        {
            oriTime->up16Sec  = subTime->up16Sec;
            oriTime->low32Sec = subTime->low32Sec;
            oriTime->nanoSec  = subTime->nanoSec;
        }
        else if((DVA_PTP_STAMP_RESET == modiType) && (NULL != oriTime))
        {
            oriTime->up16Sec  = 0u;
            oriTime->low32Sec = 0u;
            oriTime->nanoSec  = 0u;
        }
        else
        { /* oops */
        }
    } while(0u == 1u);
}

/**
 * @brief If input domainid is not 0,1, and 2, it return 1
 * @param[in] domainid 0,1,2
 * @param[out] None
 * @retval 0: domainid is 0,1, and 2
 * @retval 1: domainid is not 0,1, and 2
 */
uint8 dva_drv_ptp_invalid_ptpdomainid(uint32 domainid)
{
    uint8 invalid_ptpdomainid = 0u;
    if(domainid > 2u)
    {
        invalid_ptpdomainid = 1u;
    }
    return invalid_ptpdomainid;
}

RtkApiRet dva_drv_ptp_invalid_logintval(int8 logintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if((logintval == (int8)127) || (logintval == (int8)-128))
    {
    }
    else if((logintval >= (int8)-5) && (logintval <= (int8)3))
    {
    }
    else
    {
        ret = -RT_ERR_PTP_INVALID_LOGINTVAL;
    }
    return ret;
}

/**
 * \brief Use ACL to trap PTP packets
 * \param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used for indicating which switch to be accessed.
 * \param[in] params ptp acl information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * \param[out] None
 * \return RT_ERR_OK: Set OK
 * \return -RT_ERR_INPUT: Invalid value in params.
 * \return -RT_ERR_NULL_POINTER: Paraments DA is NULL.
 * \return -RT_ERR_PTP_CALL_API_FAILED: API Set Failed.
 */
RtkApiRet dva_drv_ptp_acl_redirect_set(uint32 unit, dva_ptpAclParams_t *params)
{
    uint32                  index[DVA_PTP_MAX_ACL_RULE] = {DVA_PTP_ACL_IDX1, DVA_PTP_ACL_IDX0, DVA_PTP_ACL_IDX2};
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    RtkApiRet               ret = RT_ERR_OK;
    RtkApiRet               apiresult;
    uint16                  isAssignPRIen = 0u;

    do
    {
        if(NULL == params->DA)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(params->priority > 7u)
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(0u != (params->redirectPortMsk & 0xFFFF0000u))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(params->ptpAclIdx >= DVA_PTP_MAX_ACL_RULE)
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        apiresult = dva_ptp_get_en_inter_serv_pri(unit, &isAssignPRIen);
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /*
            Step 1.2. clear parameter
        */
        (void)dva_drv_memset(&pieParam, 0, sizeof(pieParam));
        (void)dva_drv_memset(&actParam, 0, sizeof(actParam));

        /*
            Step 1.3. configure EtheType 0x88F7
        */
        pieParam.data6 = params->ethertype;
        pieParam.mask6 = 0xFFFFu;

        /*
            Step 1.3. configure DA=01:80:C2:00:00:0E,DA={0x1,0x80,0xc2,0x0,0x0,0xe}
        */
        pieParam.data2 = (((uint16)params->DA[0]) << (uint16)8) | ((uint16)params->DA[1]);
        pieParam.mask2 = 0xFFFFu;
        pieParam.data1 = (((uint16)params->DA[2]) << (uint16)8) | ((uint16)params->DA[3]);
        pieParam.mask1 = 0xFFFFu;
        pieParam.data0 = (((uint16)params->DA[4]) << (uint16)8) | ((uint16)params->DA[5]);
        pieParam.mask0 = 0xFFFFu;

        /* SPMASK. Need it = 0 */
        pieParam.data9 = 0x0000u;
        pieParam.mask9 = params->spmask;

        /*
            Step 1.4. configure entry DVA_PTP_ACL_IDX to valid
         */
        if(1u == params->isEnabled)
        {
            pieParam.valid = 0x01u;
        }
        else
        {
            pieParam.valid = 0x0u;
        }

        /*
            Step 1.5. Configure the ACL entry DVA_PTP_ACL_IDX1 ~ DVA_PTP_ACL_IDX3 in TG17
        */
        apiresult = dva_drv_acl_setting_pie_entry(unit, index[params->ptpAclIdx], &pieParam);
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        /*
            Step 1.6. configure redir = 1, opcode = 1 (trap), dpm = bit[15] (port redirectPort), for entry DVA_PTP_ACL_IDX
        */
        actParam.redir               = 1u;
        actParam.egrRedirInfo.opcode = 1u;
        actParam.egrRedirInfo.dpm    = params->redirectPortMsk;

        actParam.prioasn           = (uint32)isAssignPRIen;
        actParam.prioInfo.priority = (uint32)params->priority;

        /*
            Step 1.7. configure prepared action to hardware table
        */
        apiresult = dva_drv_eacl_setting_action_entry(unit, index[params->ptpAclIdx], &actParam);

        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

    } while(0u == 1u);
    return ret;
}

/**
 * \brief Use ACL to trap PTP packets
 * \param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used for indicating which switch to be accessed.
 * \param[out] params ptp acl information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * \return RT_ERR_OK: Set OK
 * \return -RT_ERR_NULL_POINTER: isEnable,DA,priority is NULL
 * \return -RT_ERR_PTP_CALL_API_FAILED: Set Fail
 */
RtkApiRet dva_drv_ptp_acl_redirect_get(uint32 unit, dva_ptpAclParams_t *params)
{
    uint32                  index[DVA_PTP_MAX_ACL_RULE] = {DVA_PTP_ACL_IDX1, DVA_PTP_ACL_IDX0, DVA_PTP_ACL_IDX2};
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    RtkApiRet               ret = RT_ERR_OK;
    RtkApiRet               apiresult;
    do
    {
        if((NULL == params) || (NULL == params->DA))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /*
            Step 1.1. clear parameter
        */
        (void)dva_drv_memset(&pieParam, 0, sizeof(pieParam));
        (void)dva_drv_memset(&actParam, 0, sizeof(actParam));
        apiresult = dva_drv_acl_getting_pie_entry(unit, index[params->ptpAclIdx], &pieParam);
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiresult = dva_drv_eacl_getting_action_entry(unit, index[params->ptpAclIdx], &actParam);
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(0x1u == pieParam.valid)
        {
            params->isEnabled = 1u;
        }
        else
        {
            params->isEnabled = 0u;
        }
        /*
            Step 1.2. Get EtheType
        */
        params->ethertype = pieParam.data6;
        /*
            Step 1.3. Get DA
        */
        params->DA[0] = (uint8)((pieParam.data2 & 0xFF00u) >> 8);
        params->DA[1] = (uint8)(pieParam.data2 & 0xFFu);

        params->DA[2] = (uint8)((pieParam.data1 & 0xFF00u) >> 8);
        params->DA[3] = (uint8)(pieParam.data1 & 0xFFu);

        params->DA[4] = (uint8)((pieParam.data0 & 0xFF00u) >> 8);
        params->DA[5] = (uint8)(pieParam.data0 & 0xFFu);

        /* Step 1.4. Get SPMASK */
        params->spmask = pieParam.mask9;
        /* Step 1.5. Get redirect port mask */
        params->redirectPortMsk = actParam.egrRedirInfo.dpm;

        /* Step 1.6. Get priority */
        if(1u == actParam.prioasn)
        {
            params->priority = (uint8)(actParam.prioInfo.priority & 0x7u);
        }
        else
        {
            params->priority = 0u;
        }

    } while(0u == 1u);
    return ret;
}

/**
 * @brief 1-Step receive
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                 The specific port, range is 1~11
 * @param[in] domain               The specific domain, range is 0, 1, 2
 * @param[in] oneStepRxSync:       1-Step received operability. 0: Disable, 1:Enable
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * @retval Others are fail
 */
RtkApiRet dva_drv_ptp_onesteprxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepRxSync)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = -RT_ERR_OK;
    uint8     cfg    = 0u;
    do
    {
        if(DVA_IS_VALID_PORT_ID((port)) == 0u)
        {
            ret = -(RT_ERR_PORT_ID);
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == oneStepRxSync)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_PTPCFG_ONESTEPRXSYNC + (((uint32)dva_g_userPort2sysPort[port] * (uint32)3u) + domainid), &cfg);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *oneStepRxSync = cfg;
    } while(0u == 1u);
    return ret;
}

/* Function Name:
 *      dva_drv_ptp_urgentTXInfo_get
 * Description:
 *      Get the TX information remainTXTime(ms), remain TxTimeBuffer(ms), and domain&port mask
 * Input:
 *      uint         -Stacking Uint.
 * Output:
 *      urgentTXInfo -Structure including TX information remainTXTime(ms), remain TxTimeBuffer(ms), and domain&port mask.
 * Return:
 *      RT_ERR_OK
 *      Others are fail
 * Note:
 *
 */
RtkApiRet dva_drv_ptp_urgentTXInfo_get(uint32 unit, dva_ptpUrgentTxInfo_t *urgentTXInfo)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint16    u16data[8];
    uint32    cfgoffset = DVA_PTPCFG_URGETTXINFO;

    do
    {
        if(NULL == urgentTXInfo)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset, &u16data[0]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset + 2u, &u16data[1]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset + 4u, &u16data[2]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset + 6u, &u16data[3]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset + 8u, &u16data[4]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset + 10u, &u16data[5]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset + 12u, &u16data[6]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset + 14u, &u16data[7]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        urgentTXInfo->remainTxTime = (uint32)u16data[1];
        urgentTXInfo->remainTxTime |= (uint32)u16data[0] << 16;
        urgentTXInfo->remainTxTimeBuffer = (uint32)u16data[3];
        urgentTXInfo->remainTxTimeBuffer |= ((uint32)u16data[2]) << 16;
        urgentTXInfo->urgentDomainPortMask = (uint64)u16data[7];
        urgentTXInfo->urgentDomainPortMask |= ((uint64)u16data[6]) << 16;
        urgentTXInfo->urgentDomainPortMask |= ((uint64)u16data[5]) << 32;
        urgentTXInfo->urgentDomainPortMask |= ((uint64)u16data[4]) << 48;
    } while(0u == 1u);

    return retval;
}
/* Function Name:
 *      dva_drv_ptp_urgentTXTimeBuffer_set
 * Description:
 *      Set the TX information remain TxTimeBuffer(ms)
 * Input:
 *      uint               -Stacking Uint.
 *      urgentTXTimeBuffer(ms) -The TX time buffer to reserve time for PTP message TX out.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      Others are fail
 * Note:
 *
 */
RtkApiRet dva_drv_ptp_urgentTXTimeBuffer_set(uint32 unit, uint32 txTimeBuffer)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    offset = DVA_PTPCFG_URGETTXINFO + 4u;

    do
    {
        apiret = dva_drv_indirect_sram_set(unit, offset, txTimeBuffer);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/* Function Name:
 *      dva_drv_ptp_get_txtimesup_threshold
 * Description:
 *      Get the PTP TX time's up error threshold
 * Input:
 *      uint               -Stacking Uint.
 *      threshold -The threshould of PTP TX time's up error. If reach this threshold PTP TX task is not the 1st priority.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      Others are fail
 * Note:
 *
 */
RtkApiRet dva_drv_ptp_get_txtimesup_threshold(uint32 unit, uint32 *threshold)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_TXTIMESUPTHRESHOLD;

    do
    {
        if(NULL == threshold)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *threshold = u32data;
    } while(0u == 1u);

    return retval;
}

/* Function Name:
 *      dva_drv_ptp_set_txtimesup_threshold
 * Description:
 *      Set the PTP TX time's up error threshold
 * Input:
 *      uint               -Stacking Uint.
 *      threshold -The threshould of PTP TX time's up error. If reach this threshold PTP TX task is not the 1st priority.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      Others are fail
 * Note:
 *
 */
RtkApiRet dva_drv_ptp_set_txtimesup_threshold(uint32 unit, uint32 threshold)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    u32data = 0u;
    uint32    offset  = DVA_PTPCFG_TXTIMESUPTHRESHOLD;

    do
    {
        u32data = threshold;
        apiret  = dva_drv_indirect_sram_set(unit, offset, u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/* Function Name:
 *      dva_drv_ptp_tx_times_up_cnt_get
 * Description:
 *      Get the PTP TX time's up current error count.
 * Input:
 *      uint  -Stacking Uint.
 * Output:
 *      count -The PTP TX time's up current error count.
 * Return:
 *      RT_ERR_OK
 *      Others are fail
 * Note:
 *
 */
RtkApiRet dva_drv_ptp_tx_times_up_cnt_get(uint32 unit, uint32 *count)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_TXTIMESUPCNT;

    do
    {
        if(NULL == count)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *count = u32data;
    } while(0u == 1u);

    return retval;
}

/* Function Name:
 *      dva_drv_ptp_tx_times_up_cnt_set
 * Description:
 *      Set the PTP TX time's up current error count.
 * Input:
 *      uint  -Stacking Uint.
 *      count -The PTP TX time's up current error count.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      Others are fail
 * Note:
 *
 */
RtkApiRet dva_drv_ptp_tx_times_up_cnt_set(uint32 unit, uint32 count)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    u32data = 0u;
    uint32    offset  = DVA_PTPCFG_TXTIMESUPCNT;

    do
    {
        u32data = count;
        apiret  = dva_drv_indirect_sram_set(unit, offset, u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] u8data
 * @retval
 */
RtkApiRet dva_drv_ptp_txLatency_set(uint32 unit, RtkPort port, uint32 latency)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    u32data = 0u;
    uint32    offset  = DVA_PTPCFG_TXLATENCY + ((uint32)dva_g_userPort2sysPort[port] * 4u);

    do
    {
        u32data = latency;
        apiret  = dva_drv_indirect_sram_set(unit, offset, u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_txLatency_get(uint32 unit, RtkPort port, uint32 *latency)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_TXLATENCY + ((uint32)dva_g_userPort2sysPort[port] * 4u);

    do
    {
        if(NULL == latency)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *latency = u32data;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] u8data
 * @retval
 */
RtkApiRet dva_drv_ptp_rxLatency_set(uint32 unit, RtkPort port, uint32 latency)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    u32data = 0u;
    uint32    offset  = DVA_PTPCFG_RXLATENCY + ((uint32)dva_g_userPort2sysPort[port] * 4u);

    do
    {
        u32data = latency;
        apiret  = dva_drv_indirect_sram_set(unit, offset, u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_rxLatency_get(uint32 unit, RtkPort port, uint32 *latency)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_RXLATENCY + ((uint32)dva_g_userPort2sysPort[port] * 4u);

    do
    {
        if(NULL == latency)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *latency = u32data;
    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_ppsmaclatency_set(uint32 unit, uint32 latency)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    u32data = 0u;
    uint32    offset  = DVA_PTPCFG_PPSMACLATENCY;

    do
    {
        u32data = latency;
        apiret  = dva_drv_indirect_sram_set(unit, offset, u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_ppsmaclatency_get(uint32 unit, uint32 *latency)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_PPSMACLATENCY;

    do
    {
        if(NULL == latency)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *latency = u32data;

    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_set_ppstrig_low_threshold(uint32 unit, uint32 threshold)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    u32data = 0u;
    uint32    offset  = DVA_PTPCFG_PPSTRIGLOWTHRESHOLD;

    do
    {
        u32data = threshold;
        apiret  = dva_drv_indirect_sram_set(unit, offset, u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_get_ppstrig_low_threshold(uint32 unit, uint32 *threshold)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_PPSTRIGLOWTHRESHOLD;

    do
    {
        if(NULL == threshold)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *threshold = u32data;
    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_set_ppstrig_high_threshold(uint32 unit, uint32 threshold)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    u32data = 0u;
    uint32    offset  = DVA_PTPCFG_PPSTRIGHIGHTHRESHOLD;

    do
    {
        u32data = threshold;
        apiret  = dva_drv_indirect_sram_set(unit, offset, u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_get_ppstrig_high_threshold(uint32 unit, uint32 *threshold)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_PPSTRIGHIGHTHRESHOLD;

    do
    {
        if(NULL == threshold)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *threshold = u32data;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the pdelay response detected faults
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port to configure
 * @param[out] detectfaults The pdelay response detected faults
 * @retval
 */
RtkApiRet dva_drv_ptp_detectedfaults_get(uint32 unit, RtkPort port, uint8 *detectfaults)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_DETECTEDFAULTS + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        if(NULL == detectfaults)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *detectfaults = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set the ascapablecrossdomains in pdelay response process function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port to configure
 * @param[in] ascapablecrosdomain The ascapablecrosdomain in specific port in pdelay response process function.
 * @retval
 */
RtkApiRet dva_drv_ptp_set_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 ascapablecrosdomain)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = ascapablecrosdomain;
    uint32    offset = DVA_PTPCFG_ASCAPABLEACROSSDOMAINS + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        apiret = dva_drv_indirect_sram_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the ascapablecrossdomains in pdelay response process function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port to configure
 * @param[out] ascapablecrosdomain The ascapablecrosdomain in specific port in pdelay response process function.
 * @retval
 */
RtkApiRet dva_drv_ptp_get_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 *ascapablecrosdomain)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_ASCAPABLEACROSSDOMAINS + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        if(NULL == ascapablecrosdomain)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *ascapablecrosdomain = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set the asymmetry measurement mode
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port to configure
 * @param[in] allowedfaults The asymmetry measurement mode
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_set_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 mode)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = mode;
    uint32    offset = DVA_PTPCFG_ASYMMETRYMEASUREMENTMODE + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        apiret = dva_drv_indirect_sram_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the asymmetry measurement mode
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port to configure
 * @param[out] mode The asymmetry measurement mode
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_get_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 *mode)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_ASYMMETRYMEASUREMENTMODE + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        if(NULL == mode)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *mode = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the hw pdelay domain.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port The port to configure
 * @param[out] ascapablecrosdomain The ascapablecrosdomain in specific port in pdelay response process function.
 * @retval
 */
RtkApiRet dva_drv_ptp_hw_pdelay_domain_get(uint32 unit, RtkPort port, uint8 *domain)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_HWPDELAYDOMAIN + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        if(NULL == domain)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *domain = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the parentlog interval.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] domain The domain to get
 * @param[out] parentlog The parentlog in specific domain.
 * @retval
 */
RtkApiRet dva_drv_ptp_parentlog_interval_get(uint32 unit, uint32 domain, uint32 *parentlog)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = 0u;

    do
    {
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -(RT_ERR_INPUT);
            continue;
        }
        if(NULL == parentlog)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        cfgoffset = DVA_PTPCFG_PARENTLOGSYNCINTERVAL + (domain * 4u);
        apiret    = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *parentlog = u32data;

    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get runtime synclock status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port: 1,2,...11
 * @param[in] domainid 0,1,2
 * @param[in] rtSyncLockStatus runtime synclock status
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet dva_drv_ptp_runtime_synclock_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncLockStatus)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = -RT_ERR_OK;
    uint8     cfg    = 0u;
    do
    {
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == rtSyncLockStatus)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_PTPCFG_SYNCLOCKED + (((uint32)dva_g_userPort2sysPort[port] * (uint32)3u) + domainid), &cfg);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *rtSyncLockStatus = cfg;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get runtime syncslowdown status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port: 1,2,...11
 * @param[in] domainid 0,1,2
 * @param[in] rtSyncLockStatus runtime sync slowdown status
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet dva_drv_ptp_runtime_syncslowdown_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncSlowdownStatus)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = -RT_ERR_OK;
    uint8     cfg    = 0u;
    do
    {
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == rtSyncSlowdownStatus)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_PTPCFG_SYNCSLOWDOWN + (((uint32)dva_g_userPort2sysPort[port] * (uint32)3u) + domainid), &cfg);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *rtSyncSlowdownStatus = cfg;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get runtime gptpCapableMsgInterval which is set from FW in runtime.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] intval gptpcapable message interval(ms)
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_runtime_gptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_GPTPCAPABLEMSGINTERVAL + ((((uint32)dva_g_userPort2sysPort[port] * 3u) + domain) * 4u);

    do
    {
        if(NULL == intval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *intval = u32data;

    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get runtime lastgptpCapableMsgInterval which is set from FW in runtime.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] intval last gptpcapable message interval(ms)
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_runtime_lastgptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_OLDGPTPCAPABLEMSGINTERVAL + ((((uint32)dva_g_userPort2sysPort[port] * 3u) + domain) * 4u);

    do
    {
        if(NULL == intval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *intval = u32data;

    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get runtime gptpSlowdown capability
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] gptpSlowdown 0, 1
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_runtime_gptpslowdown_get(uint32 unit, RtkPort port, uint32 domain, uint8 *gptpSlowdown)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_GPTPSLOWDOWN + ((uint32)dva_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        if(NULL == gptpSlowdown)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *gptpSlowdown = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get runtime neighbor gptp capability
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] neiGptpCapable 0, 1
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_runtime_neigptpcapable_get(uint32 unit, RtkPort port, uint32 domain, uint8 *neiGptpCapable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_NEIGHBORGPTPCAPABLE + ((uint32)dva_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        if(NULL == neiGptpCapable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *neiGptpCapable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get neighbor gptpcapable message timer which is set from FW in runtime.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] timerCnt timer count(ms)
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_neigptp_rxtimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_NEIGHBORGPTPRXTIMER + ((((uint32)dva_g_userPort2sysPort[port] * 3u) + domain) * 4u);

    do
    {
        if(NULL == timerCnt)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *timerCnt = u32data;

    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get neighbor gptpcapable message timer target which is set from FW in runtime.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] targetTime timer target(ms)
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_neigptp_rxtimertarget_get(uint32 unit, RtkPort port, uint32 domain, uint32 *targetTime)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_NEIGHBORGPTPRXTIMERTARGET + ((((uint32)dva_g_userPort2sysPort[port] * 3u) + domain) * 4u);

    do
    {
        if(NULL == targetTime)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *targetTime = u32data;

    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get latestNeighGptpRxIntervalLog
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] logIntval -5 ~ 3
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_last_neigptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, int8 *logIntval)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_LATESTNEIGHGPTPRXINTERVALLOG + ((uint32)dva_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        if(NULL == logIntval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *logIntval = (int8)cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get RX pdelay request CMLDS status.
 * @param[in] port 1~11
 * @param[out] cmldsStatus 0, 1
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_rx_pdelayreq_cmds_get(uint32 unit, RtkPort port, uint8 *cmldsStatus)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_RXCMLDSPDELAYREQ + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        if(NULL == cmldsStatus)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *cmldsStatus = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get RX pdelay request domain number.
 * @param[in] port 1~11
 * @param[out] cmldsStatus 0, 1
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_rx_pdelay_domainnum_get(uint32 unit, RtkPort port, uint8 *pdelayDomainNum)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DVA_PTPCFG_RXPDELAYREQDOMAINNUM + (uint32)dva_g_userPort2sysPort[port];

    do
    {
        if(NULL == pdelayDomainNum)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *pdelayDomainNum = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get gptpcapable message timer count which is set from FW in runtime.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port 1~11
 * @param[in] domain 0, 1, 2
 * @param[out] timerCnt timer target(ms)
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_gptpcapabletimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    u32data;
    uint32    cfgoffset = DVA_PTPCFG_GPTPCAPABLETIMER + ((((uint32)dva_g_userPort2sysPort[port] * 3u) + domain) * 4u);

    do
    {
        if(NULL == timerCnt)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_get(unit, cfgoffset, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *timerCnt = u32data;

    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_set_ts_polling_error_recover_count(uint32 unit, uint16 recoverCnt)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint16    u16data = 0u;
    uint32    offset  = DVA_PTPCFG_POLLINGERRORRECOVERCNT;

    do
    {
        u16data = (uint16)recoverCnt;
        apiret  = dva_drv_indirect_sram_2bytes_set(unit, offset, u16data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

RtkApiRet dva_drv_ptp_get_ts_polling_error_recover_count(uint32 unit, uint16 *recoverCnt)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint16    u16data;
    uint32    cfgoffset = DVA_PTPCFG_POLLINGERRORRECOVERCNT;

    do
    {
        if(NULL == recoverCnt)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        apiret = dva_drv_indirect_sram_2bytes_get(unit, cfgoffset, &u16data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *recoverCnt = u16data;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Check reinit protocol process done. \n
 *  This API is to check run-time changing done in FW.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval  RT_ERR_OK
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dva_drv_ptp_check_reinitprotocol_done(uint32 unit, uint32 regval)
{
    RtkApiRet ret        = RT_ERR_OK;
    int32     apiret     = 0;
    uint32    waitcnt    = 0u;
    uint32    timeoutCnt = (dva_g_ptpConfigTimeout >= DVA_PTP_POLLING_INTERVAL) ? (dva_g_ptpConfigTimeout / DVA_PTP_POLLING_INTERVAL) : 1u;
    uint32    regTmp     = regval;

    do
    {
        for(waitcnt = 0; waitcnt < timeoutCnt; waitcnt++)
        {
            apiret = dva_reg_read(unit, DVA_PTP_CFG_001, &regTmp);
            if(RT_ERR_OK != apiret)
            {
                continue;
            }
            if(0u != (regTmp & 0x8000u))
            {
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DVA_PTP_POLLING_INTERVAL));
        }

        if(((regTmp & 0x8000u) == 0u) && (RT_ERR_OK != apiret))
        {
            ret = -(RT_ERR_PTP_CALL_API_FAILED);
            continue;
        }
        for(waitcnt = 0; waitcnt < timeoutCnt; waitcnt++)
        {
            apiret = dva_reg_read(unit, DVA_PTP_CFG_001, &regTmp);
            if(RT_ERR_OK != apiret)
            {
                continue;
            }
            if(0u == (regTmp & 0x8000u))
            {
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DVA_PTP_POLLING_INTERVAL));
        }
        if(waitcnt >= timeoutCnt)
        {
            dva_esm_event_log_set(DVA_EM_EVENT_PTP_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_ptp_get_tempda(uint32 unit, uint8 tempDA[5][DVA_MAC_ADDR_LEN])
{
    uint32                 domainid  = 0u;
    uint32                 gotDA     = 0u;
    uint16                 iserror   = 0u;
    uint8                  customEn  = 0u;
    uint16                 domainmap = 0u;
    RtkPort                port      = 0u;
    int32                  apiret    = RT_ERR_OK;
    RtkApiRet              retval    = RT_ERR_OK;
    PtpTpSupportProfiles_e profiles  = EM_PTP_PRF_AS2011;

    for(domainid = 0u; domainid < DVA_MAX_SLAVE_DOMAIN; domainid++)
    {
        gotDA = 0u;
        for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
        {
            customEn  = 0u;
            domainmap = 0u;
            apiret    = dva_ptp_get_domainmap(unit, domainid, &domainmap);
            if(RT_ERR_OK != apiret)
            {
                iserror = 1u << 0;
            }
            if(0u == (domainmap & ((uint16)1u << port)))
            {
                continue;
            }

            apiret = dva_ptp_get_damac01_enable_custom(unit, domainid, port, &customEn);
            if(RT_ERR_OK != apiret)
            {
                iserror = 1u << 1;
            }
            if(1u == customEn)
            {
                apiret = dva_ptp_get_custom_damac01(unit, domainid, port, tempDA[(uint32)EM_PTP_DA_BUF_CUSTOMIZED1 + domainid]);
                if(RT_ERR_OK != apiret)
                {
                    iserror = 1u << 2;
                }
                else
                {
                    gotDA = 1u;
                }
            }
            else
            {
                apiret = dva_drv_ptp_get_domain_profiles(unit, domainid, &profiles);
                if(RT_ERR_OK != apiret)
                {
                    iserror = 1u << 3;
                }
                if((EM_PTP_PRF_1588_E2ETC == profiles) || (EM_PTP_PRF_1588_P2PTC == profiles))
                {
                    /* 1588 */
                    /* All except peer-to-peer delay mechanism messages */
                    tempDA[EM_PTP_DA_BUF_1588][0] = 0x1u;
                    tempDA[EM_PTP_DA_BUF_1588][1] = 0x1Bu;
                    tempDA[EM_PTP_DA_BUF_1588][2] = 0x19u;
                    tempDA[EM_PTP_DA_BUF_1588][3] = 0x00u;
                    tempDA[EM_PTP_DA_BUF_1588][4] = 0x00u;
                    tempDA[EM_PTP_DA_BUF_1588][5] = 0x00u;
                    /* Peer-to-peer delay mechanism messages */
                    tempDA[EM_PTP_DA_BUF_AS][0] = 0x1u;
                    tempDA[EM_PTP_DA_BUF_AS][1] = 0x80u;
                    tempDA[EM_PTP_DA_BUF_AS][2] = 0xC2u;
                    tempDA[EM_PTP_DA_BUF_AS][3] = 0x0u;
                    tempDA[EM_PTP_DA_BUF_AS][4] = 0x0u;
                    tempDA[EM_PTP_DA_BUF_AS][5] = 0xEu;
                    gotDA                       = 1u;
                }
                else
                {
                    /* 802.1AS */
                    tempDA[EM_PTP_DA_BUF_AS][0] = 0x1u;
                    tempDA[EM_PTP_DA_BUF_AS][1] = 0x80u;
                    tempDA[EM_PTP_DA_BUF_AS][2] = 0xC2u;
                    tempDA[EM_PTP_DA_BUF_AS][3] = 0x0u;
                    tempDA[EM_PTP_DA_BUF_AS][4] = 0x0u;
                    tempDA[EM_PTP_DA_BUF_AS][5] = 0xEu;
                    gotDA                       = 1u;
                }
            }

            if((1u == gotDA) || (0u != iserror))
            {
                break;
            }
        }
        if(0u != iserror)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            break;
        }
    }
    return retval;
}

/**
 * @brief Get customized option for glenable
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * \param[out] da        The destination address in configuration
 * \param[out] aclDAcount   Occupied ACL count.
 * \return  RT_ERR_OK
 * \return -RT_ERR_PTP_CALL_API_FAILED
 * \return -RT_ERR_NULL_POINTER
 * \return -RT_ERR_PTP_EXCEED_DA_LIMITATION
 */
RtkApiRet dva_drv_ptp_customize_da_get_in_glenable(uint32 unit, uint8 da[DVA_MAX_SLAVE_DOMAIN][DVA_MAC_ADDR_LEN], uint8 *aclDAcount)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     count  = 0u;
    uint8     count2 = 0u;
    int32     apiret = RT_ERR_OK;
    /* tempDA[0]: 802.1AS, tempDA[1]: 1588, tempDA[2]: Customized1, tempDA[3]: Customized2, tempDA[4]: Customized3 */
    uint8 tempDA[5][DVA_MAC_ADDR_LEN] = {0u};
    uint8 aclDA[5][DVA_MAC_ADDR_LEN]  = {0u};
    uint8 nullDA[DVA_MAC_ADDR_LEN]    = {0u};

    do
    {
        apiret = dva_drv_ptp_get_tempda(unit, tempDA);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        for(count = (uint8)EM_PTP_DA_BUF_AS; count < (uint8)EM_PTP_DA_BUF_END; count++)
        {
            /* Skip NULL DA. */
            if(0 == rtl_memcmp(tempDA[count], nullDA, DVA_MAC_ADDR_LEN))
            {
                continue;
            }

            for(count2 = 0u; count2 < *aclDAcount; count2++)
            {
                /* Skip existing DA. */
                if(0 == rtl_memcmp(tempDA[count], aclDA[count2], DVA_MAC_ADDR_LEN))
                {
                    continue;
                }
            }
            (void)rtl_memcpy(aclDA[*aclDAcount], tempDA[count], DVA_MAC_ADDR_LEN);
            (*aclDAcount)++;
        }

        if(*aclDAcount > DVA_PTP_MAX_ACL_RULE)
        {
            retval = -RT_ERR_PTP_EXCEED_DA_LIMITATION;
            continue;
        }
        (void)rtl_memcpy(da, aclDA, *aclDAcount * DVA_MAC_ADDR_LEN);
    } while(0u == 1u);
    return retval;
}

/**
 * \brief Get customized Ethertype for glenable
 * \param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used for indicating which switch to be accessed.
 * \param[out] ethertype The ethertype in configuration
 * \return  RT_ERR_OK
 * \return -RT_ERR_PTP_CALL_API_FAILED
 * \return -RT_ERR_NULL_POINTER
 */
RtkApiRet dva_drv_ptp_customize_ethertype_get_in_glenable(uint32 unit, uint16 *ethertype)
{
    RtkApiRet retval              = RT_ERR_OK;
    int32     apiret              = RT_ERR_OK;
    RtkPort   port                = 0u;
    uint32    cusomizeEtherTypeEn = 0u;
    uint32    cusEtherPort        = 0u;
    uint16    iserror             = 0u;
    uint8     anycustomizeEn      = 0u;

    do
    {
        if(NULL == ethertype)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        for(port = DVA_RTK_USER_PORT_1ST; port <= DVA_RTK_USER_PORT_END; port++)
        {
            anycustomizeEn = 0u;
            apiret         = dva_ptp_get_ethertype_enable_custom(unit, port, &anycustomizeEn);
            if(RT_ERR_OK != apiret)
            {
                iserror |= (1u << 3);
            }
            else if(1u == anycustomizeEn)
            {
                cusEtherPort = port;
                cusomizeEtherTypeEn |= ((uint32)1u << port);
            }
            else
            {
                /* Nothing */
                ;
            }
            if(0u != iserror)
            {
                break;
            }
        }
        if(0u != iserror)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        if(0u != cusomizeEtherTypeEn)
        {
            CHK_FUN_CONTINUE((apiret), (dva_ptp_get_custom_ethertype(unit, cusEtherPort, ethertype)));
        }
        else
        {
            /*If no customize, use 802.1AS as EtherType*/
            *ethertype = 0x88F7u;
        }
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_if_rxlatency_get(PtpPortType_e interface, uint16 *latency)
{
    RtkApiRet retv  = RT_ERR_OK;
    uint32    index = 0u;

    dva_ptpIntfLatency_t dva_g_latencyRxAry[(uint32)EM_PORT_TYPE_END] =
        {
            {EM_PORT_TYPE_START, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_PURE_100BASE_T1, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_PURE_10BASE_T1, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_FAST_ETHERNET_100, DVA_RX_LATENCY_FAST_ETHERNET_100},
            {EM_PORT_TYPE_FAST_ETHERNET_10, DVA_RX_LATENCY_FAST_ETHERNET_10},
            {EM_PORT_TYPE_FAST_ETHERNET_AN, DVA_RX_LATENCY_FAST_ETHERNET_AN},
            {EM_PORT_TYPE_MII_100, DVA_RX_LATENCY_MII_100},
            {EM_PORT_TYPE_MII_10, DVA_RX_LATENCY_MII_10},
            {EM_PORT_TYPE_RMII_100, DVA_RX_LATENCY_RMII_100},
            {EM_PORT_TYPE_RMII_10, DVA_RX_LATENCY_RMII_10},
            {EM_PORT_TYPE_RGMII_1000, DVA_RX_LATENCY_RGMII_1000},
            {EM_PORT_TYPE_RGMII_100, DVA_RX_LATENCY_RGMII_100},
            {EM_PORT_TYPE_RGMII_10, DVA_RX_LATENCY_RGMII_10},
            {EM_PORT_TYPE_SGMII_WRAP_1000, DVA_RX_LATENCY_SGMII_WRAP_1000},
            {EM_PORT_TYPE_SGMII_WRAP_100, DVA_RX_LATENCY_SGMII_WRAP_100},
            {EM_PORT_TYPE_SGMII_10, DVA_RX_LATENCY_SGMII_10},
            {EM_PORT_TYPE_SGMII_AN, DVA_RX_LATENCY_SGMII_AN},
            {EM_PORT_TYPE_RTPGE_1000BASE_T1, DVA_RX_LATENCY_RTPGE_1000BASE_T1},
            {EM_PORT_TYPE_HISGMII_WTG_2500, DVA_RX_LATENCY_HISGMII_WTG_2500},
            {EM_PORT_TYPE_BaseKR_10000, DVA_RX_LATENCY_BASEKR_10000},
            {EM_PORT_TYPE_BaseKR_5000, DVA_RX_LATENCY_BASEKR_5000},
            {EM_PORT_TYPE_USXGMII_10000, DVA_RX_LATENCY_USXGMII_WTG_10000},
            {EM_PORT_TYPE_USXGMII_5000, DVA_RX_LATENCY_USXGMII_WTG_5000},
            {EM_PORT_TYPE_USXGMII_2500, DVA_RX_LATENCY_USXGMII_WTG_2500},
            {EM_PORT_TYPE_USXGMII_1000, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_USXGMII_AN, DVA_RX_LATENCY_USXGMII_AN},
            {EM_PORT_TYPE_RTPGE_100BASE_T1, DVA_RX_LATENCY_RTPGE_100BASE_T1},
            {EM_PORT_TYPE_MACPHY, DVA_RX_LATENCY_MACPHY},
            {EM_PORT_TYPE_SGMII_WTG_1000, DVA_RX_LATENCY_SGMII_WTG_1000},
            {EM_PORT_TYPE_SGMII_WTG_100, DVA_RX_LATENCY_SGMII_WTG_100},
            {EM_PORT_TYPE_HISGMII_WRAP_2500, DVA_RX_LATENCY_HISGMII_WRAP_2500},
            {EM_PORT_TYPE_XFI_WTG_10000, DVA_RX_LATENCY_XFI_WTG_10000},
            {EM_PORT_TYPE_XFI_WTG_5000, DVA_RX_LATENCY_XFI_WTG_5000},
            {EM_PORT_TYPE_GIGABIT_ETHERNET, DVA_TX_LATENCY_GIGABIT_ETHERNET},
            {EM_PORT_TYPE_PURE_100BASE_T1_WTIH_MACSEC, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_RTPGE_1000BASE_T1_WITH_MACSEC, DVA_RX_LATENCY_RTPGE_1000BASE_T1_WITH_MACSEC},
            {EM_PORT_TYPE_RTPGE_100BASE_T1_WITH_MACSEC, DVA_RX_LATENCY_RTPGE_100BASE_T1_WITH_MACSEC},
            {EM_PORT_TYPE_MII_100_WITH_MACSEC, DVA_RX_LATENCY_MII_100_WITH_MACSEC},
            {EM_PORT_TYPE_RMII_100_WITH_MACSEC, DVA_RX_LATENCY_RMII_100_WITH_MACSEC},
            {EM_PORT_TYPE_RGMII_1000_WITH_MACSEC, DVA_RX_LATENCY_RGMII_1000_WITH_MACSEC},
            {EM_PORT_TYPE_RGMII_100_WITH_MACSEC, DVA_RX_LATENCY_RGMII_100_WITH_MACSEC},
            {EM_PORT_TYPE_SGMII_WRAP_1000_WITH_MACSEC, DVA_RX_LATENCY_SGMII_WRAP_1000_WITH_MACSEC},
            {EM_PORT_TYPE_SGMII_WRAP_100_WITH_MACSEC, DVA_RX_LATENCY_SGMII_WRAP_100_WITH_MACSEC},
            {EM_PORT_TYPE_HISGMII_WRAP_2500_WITH_MACSEC, DVA_RX_LATENCY_HISGMII_WRAP_2500_WITH_MACSEC},
            {EM_PORT_TYPE_SGMII_WTG_1000_WITH_MACSEC, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_SGMII_WTG_100_WITH_MACSEC, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_HISGMII_WTG_2500_WITH_MACSEC, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_USXGMII_WTG_5000_WITH_MACSEC, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_USXGMII_WTG_2500_WITH_MACSEC, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_BaseKR_WTG_5000_WITH_MACSEC, DVA_RX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_NOT_SUPPORT, DVA_RX_LATENCY_NOT_SUPPORT}};

    for(index = 0u; index < (uint32)EM_PORT_TYPE_END; index++)
    {
        if(interface == dva_g_latencyRxAry[index].intf)
        {
            *latency = dva_g_latencyRxAry[index].intTxLatency;
            break;
        }
    }

    if((uint32)EM_PORT_TYPE_END == index)
    {
        *latency = 0u;
        retv     = RT_ERR_FAILED;
    }

    if(DVA_RX_LATENCY_NOT_SUPPORT == *latency)
    {
        retv = RT_ERR_FAILED;
    }

    return retv;
}

RtkApiRet dva_drv_ptp_if_txlatency_get(PtpPortType_e interface, uint16 *latency)
{
    RtkApiRet retv  = RT_ERR_OK;
    uint32    index = 0u;

    dva_ptpIntfLatency_t dva_g_latencyTxAry[(uint32)EM_PORT_TYPE_END] =
        {
            {EM_PORT_TYPE_START, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_PURE_100BASE_T1, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_PURE_10BASE_T1, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_FAST_ETHERNET_100, DVA_TX_LATENCY_FAST_ETHERNET_100},
            {EM_PORT_TYPE_FAST_ETHERNET_10, DVA_TX_LATENCY_FAST_ETHERNET_10},
            {EM_PORT_TYPE_FAST_ETHERNET_AN, DVA_TX_LATENCY_FAST_ETHERNET_AN},
            {EM_PORT_TYPE_MII_100, DVA_TX_LATENCY_MII_100},
            {EM_PORT_TYPE_MII_10, DVA_TX_LATENCY_MII_10},
            {EM_PORT_TYPE_RMII_100, DVA_TX_LATENCY_RMII_100},
            {EM_PORT_TYPE_RMII_10, DVA_TX_LATENCY_RMII_10},
            {EM_PORT_TYPE_RGMII_1000, DVA_TX_LATENCY_RGMII_1000},
            {EM_PORT_TYPE_RGMII_100, DVA_TX_LATENCY_RGMII_100},
            {EM_PORT_TYPE_RGMII_10, DVA_TX_LATENCY_RGMII_10},
            {EM_PORT_TYPE_SGMII_WRAP_1000, DVA_TX_LATENCY_SGMII_WRAP_1000},
            {EM_PORT_TYPE_SGMII_WRAP_100, DVA_TX_LATENCY_SGMII_WRAP_100},
            {EM_PORT_TYPE_SGMII_10, DVA_TX_LATENCY_SGMII_10},
            {EM_PORT_TYPE_SGMII_AN, DVA_TX_LATENCY_SGMII_AN},
            {EM_PORT_TYPE_RTPGE_1000BASE_T1, DVA_TX_LATENCY_RTPGE_1000BASE_T1},
            {EM_PORT_TYPE_HISGMII_WTG_2500, DVA_TX_LATENCY_HISGMII_WTG_2500},
            {EM_PORT_TYPE_BaseKR_10000, DVA_TX_LATENCY_BASEKR_10000},
            {EM_PORT_TYPE_BaseKR_5000, DVA_TX_LATENCY_BASEKR_5000},
            {EM_PORT_TYPE_USXGMII_10000, DVA_TX_LATENCY_USXGMII_WTG_10000},
            {EM_PORT_TYPE_USXGMII_5000, DVA_TX_LATENCY_USXGMII_WTG_5000},
            {EM_PORT_TYPE_USXGMII_2500, DVA_TX_LATENCY_USXGMII_WTG_2500},
            {EM_PORT_TYPE_USXGMII_1000, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_USXGMII_AN, DVA_TX_LATENCY_USXGMII_AN},
            {EM_PORT_TYPE_RTPGE_100BASE_T1, DVA_TX_LATENCY_RTPGE_100BASE_T1},
            {EM_PORT_TYPE_MACPHY, DVA_TX_LATENCY_MACPHY},
            {EM_PORT_TYPE_SGMII_WTG_1000, DVA_TX_LATENCY_SGMII_WTG_1000},
            {EM_PORT_TYPE_SGMII_WTG_100, DVA_TX_LATENCY_SGMII_WTG_100},
            {EM_PORT_TYPE_HISGMII_WRAP_2500, DVA_TX_LATENCY_HISGMII_WRAP_2500},
            {EM_PORT_TYPE_XFI_WTG_10000, DVA_TX_LATENCY_XFI_WTG_10000},
            {EM_PORT_TYPE_XFI_WTG_5000, DVA_TX_LATENCY_XFI_WTG_5000},
            {EM_PORT_TYPE_GIGABIT_ETHERNET, DVA_TX_LATENCY_GIGABIT_ETHERNET},
            {EM_PORT_TYPE_PURE_100BASE_T1_WTIH_MACSEC, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_RTPGE_1000BASE_T1_WITH_MACSEC, DVA_TX_LATENCY_RTPGE_1000BASE_T1_WITH_MACSEC},
            {EM_PORT_TYPE_RTPGE_100BASE_T1_WITH_MACSEC, DVA_TX_LATENCY_RTPGE_100BASE_T1_WITH_MACSEC},
            {EM_PORT_TYPE_MII_100_WITH_MACSEC, DVA_TX_LATENCY_MII_100_WITH_MACSEC},
            {EM_PORT_TYPE_RMII_100_WITH_MACSEC, DVA_TX_LATENCY_RMII_100_WITH_MACSEC},
            {EM_PORT_TYPE_RGMII_1000_WITH_MACSEC, DVA_TX_LATENCY_RGMII_1000_WITH_MACSEC},
            {EM_PORT_TYPE_RGMII_100_WITH_MACSEC, DVA_TX_LATENCY_RGMII_100_WITH_MACSEC},
            {EM_PORT_TYPE_SGMII_WRAP_1000_WITH_MACSEC, DVA_TX_LATENCY_SGMII_WRAP_1000_WITH_MACSEC},
            {EM_PORT_TYPE_SGMII_WRAP_100_WITH_MACSEC, DVA_TX_LATENCY_SGMII_WRAP_100_WITH_MACSEC},
            {EM_PORT_TYPE_HISGMII_WRAP_2500_WITH_MACSEC, DVA_TX_LATENCY_HISGMII_WRAP_2500_WITH_MACSEC},
            {EM_PORT_TYPE_SGMII_WTG_1000_WITH_MACSEC, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_SGMII_WTG_100_WITH_MACSEC, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_HISGMII_WTG_2500_WITH_MACSEC, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_USXGMII_WTG_5000_WITH_MACSEC, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_USXGMII_WTG_2500_WITH_MACSEC, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_BaseKR_WTG_5000_WITH_MACSEC, DVA_TX_LATENCY_NOT_SUPPORT},
            {EM_PORT_TYPE_NOT_SUPPORT, DVA_TX_LATENCY_NOT_SUPPORT}};

    for(index = 0u; index < (uint32)EM_PORT_TYPE_END; index++)
    {
        if(interface == dva_g_latencyTxAry[index].intf)
        {
            *latency = dva_g_latencyTxAry[index].intTxLatency;
            break;
        }
    }

    if((uint32)EM_PORT_TYPE_END == index)
    {
        *latency = 0u;
        retv     = RT_ERR_FAILED;
    }

    if(DVA_TX_LATENCY_NOT_SUPPORT == *latency)
    {
        retv = RT_ERR_FAILED;
    }

    return retv;
}

RtkApiRet dva_drv_ptp_invalid_intval(uint32 pdelayIntval)
{
    RtkApiRet retval = RT_ERR_OK;

    switch(pdelayIntval)
    {
        case 65535:
            break;
        case 8001:
            break;
        case 4001:
            break;
        case 2001:
            break;
        case 1001:
            break;
        case 501:
            break;
        case 251:
            break;
        case 126:
            break;
        case 62:
            break;
        case 32:
            break;
        default:
            retval = RT_ERR_FAILED;
            break;
    }

    return retval;
}

/**
 * @brief Get the profiles setting on specific domain. \n
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] profiles     PTP Profiles on this domain. \n
 *                          Pointer to profiles information. Please refer to PtpTpSupportProfiles_e.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was wrong pointer paramenter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was wrong domain id, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was call API internal error, return error.
 */
RtkApiRet dva_drv_ptp_get_domain_profiles(uint32 unit, uint32 domainid, PtpTpSupportProfiles_e *profiles)
{
    RtkApiRet              ret            = RT_ERR_OK;
    uint32                 regAddr        = 0u;
    uint32                 regData        = 0u;
    PtpTpSupportProfiles_e profilesAry[6] = {EM_PTP_PRF_AS2011, EM_PTP_PRF_AUTOMOTIVE, EM_PTP_PRF_AUTOSAR, EM_PTP_PRF_AS2020, EM_PTP_PRF_1588_E2ETC, EM_PTP_PRF_1588_P2PTC};

    do
    {
        if(NULL == profiles)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        regAddr = DVA_DOMAIN_PROFILES + (4u * domainid);
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_get(unit, regAddr, &regData));
        if(regData >= 6u)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        *profiles = profilesAry[regData];
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get the autosar time sync protocol cyclic redundancy check(CRC) support configuration. \n
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[out] *enable      enable status (0, 1) \n
 *                          Status of CRC support.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was call API internal error, return error.
 */
RtkApiRet dva_drv_ptp_atsar_get_crc_support(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_CRCSUPPORT, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get the autosar time sync protocol frame priority configuration. \n
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] enable      enable status (0, 1) \n
 *                        Status of frame priority.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was call API internal error, return error.
 */
RtkApiRet dva_drv_ptp_atsar_get_frame_pri(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_FRAMEPRIO, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_rx_pdelayreq_period(uint32 unit, uint32 *pdelayIntval)
{
    RtkApiRet retval = RT_ERR_OK;
    uint32    cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == pdelayIntval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_get(unit, DVA_AUTOSAR_GLOBALTIMETXPDELAYREQPERIOD, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pdelayIntval = cfg;
    } while(0u == 1u);
    return retval;
}
RtkApiRet dva_drv_ptp_atsar_get_pdelay_latency_thresh(uint32 unit, uint32 *delayThresh)
{
    RtkApiRet retval = RT_ERR_OK;
    uint32    cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == delayThresh)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_get(unit, DVA_AUTOSAR_PDELAYLATENCYTHRESHOLD, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *delayThresh = cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_prop_delay(uint32 unit, uint32 *pDelay)
{
    RtkApiRet retval = RT_ERR_OK;
    uint32    cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == pDelay)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_get(unit, DVA_AUTOSAR_GLOBALTIMEPROPAGATIONDELAY, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pDelay = cfg;
    } while(0u == 1u);
    return retval;
}
RtkApiRet dva_drv_ptp_atsar_get_tx_period(uint32 unit, uint32 *intval)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    uint32    cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == intval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_get(unit, DVA_AUTOSAR_GLOBALTIMETXPERIOD, &cfg);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        *intval = cfg;
        retval  = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_msg_compliance(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_MESSAGECOMPLIANCE, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_rx_crc_validated(uint32 unit, PtpTpCrcValidated_e *validated)
{
    RtkApiRet           retval        = RT_ERR_OK;
    uint32              cfg           = 0u;
    int32               apiret        = RT_ERR_OK;
    PtpTpCrcValidated_e crcOptions[4] = {EM_PTP_CRC_IGNORED, EM_PTP_CRC_NOT_VALIDATED, EM_PTP_CRC_OPTIONAL, EM_PTP_CRC_VALIDATED};

    do
    {
        if(NULL == validated)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM thrshould configuration of pdelay write to flash */
        apiret = dva_drv_indirect_sram_get(unit, DVA_AUTOSAR_RXCRCVALIDATED, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(cfg >= 4u)
        {
            retval = -RT_ERR_FAILED;
            continue;
        }

        *validated = crcOptions[cfg];
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_crc_flags(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_CRCFLAGSRXVALIDATED, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_time_subtlv(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_TLVFOLLOWUPTIMESUBTLV, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_status_subtlv(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_TLVFOLLOWUPSTATUSSUBTLV, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_udata_subtlv(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_TLVFOLLOWUPUSERDATASUBTLV, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_ofs_subtlv(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_TSYNTLVFOLLOWUPOFSSUBTLV, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_tx_crc_time_flags(uint32 unit, uint8 *flags)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == flags)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_CRCTIMEFLAGSTXSECURED, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *flags = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_tx_time_crc_secured(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_GLOBALTIMETXCRCSECURED, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_pdelay_resp_enable(uint32 unit, uint8 *enable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_GLOBALTIMEPDELAYRESPENABLE, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_tag_vid_pri_get(uint32 unit, uint32 domainid, ptpNewQTaginfo *taginfo)
{
    RtkApiRet retval  = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret  = RT_ERR_OK;
    uint32    offset  = (DVA_PTPCFG_TAG + ((uint32)domainid * 0xCu));
    uint8     u8data  = 0u;
    uint16    u16data = 0u;
    do
    {
        if(NULL == taginfo)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dva_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        /* Set SRAM qtag index0 infomation tci(tag control information). */
        apiret = dva_drv_indirect_sram_2bytes_get(unit, offset, &u16data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->pcp0 = (u16data & 0xE000u) >> 13u;
        taginfo->cfi0 = (u16data & 0x1000u) >> 12u;
        taginfo->vid0 = (u16data & 0xFFFu);

        offset += 2u;
        /* Set SRAM qtag index0 infomation enable. */
        apiret = dva_drv_indirect_sram_1byte_get(unit, offset, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->enable0 = u8data;

        offset += 2u;
        /* Set SRAM qtag index1 infomation tci(tag control information). */
        apiret = dva_drv_indirect_sram_2bytes_get(unit, offset, &u16data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->pcp1 = (u16data & 0xE000u) >> 13u;
        taginfo->cfi1 = (u16data & 0x1000u) >> 12u;
        taginfo->vid1 = (u16data & 0xFFFu);

        offset += 2u;
        /* Set SRAM qtag index1 infomation enable. */
        apiret = dva_drv_indirect_sram_1byte_get(unit, offset, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->enable1 = u8data;

        offset += 2u;

        /* Set SRAM qtag infomation fix. */
        apiret = dva_drv_indirect_sram_1byte_get(unit, offset, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->fix = u8data;

        offset += 1u;
        /* Set SRAM qtag infomation enable. */
        apiret = dva_drv_indirect_sram_1byte_get(unit, offset, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->enable = u8data;

        offset += 1u;
        /* Set SRAM qtag infomation tag length. */
        apiret = dva_drv_indirect_sram_1byte_get(unit, offset, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }

        taginfo->tagLength = u8data;
        retval             = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_foloup_dataid_list(uint32 unit, uint8 dataid[16])
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    uint8     i      = 0u;
    do
    {
        for(i = 0u; i < 16u; i++)
        {
            apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_DATAIDLIST + i, &cfg);
            if(RT_ERR_OK != apiret)
            {
                retval = -RT_ERR_PTP_CALL_API_FAILED;
                break;
            }
            dataid[i] = (uint8)cfg;
        }
    } while(0u == 1u);
    return retval;
}

RtkApiRet dva_drv_ptp_atsar_get_ofs_multicity(uint32 unit, uint8 *ofsMulti)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == ofsMulti)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dva_drv_indirect_sram_1byte_get(unit, DVA_AUTOSAR_OFSMULTIPLICITY, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *ofsMulti = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**@}*/  // PTP