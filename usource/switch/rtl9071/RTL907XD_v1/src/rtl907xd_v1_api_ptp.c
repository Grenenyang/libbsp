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

#include "rtl907xd_v1_asicdrv_ptp.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#include "rtl907xd_v1_asicdrv_flash.h"
#include "rtl907xd_v1_api_ext.h"

/**
 * @addtogroup RTK_MODULE_PTP PTP
 * @brief PTP APIs
 * @{
 */
/********* BEGIN TIME SYNC/PTP *****************/

/**
 * @addtogroup PTP_TIME Time application interface configuration
 * @{
 */

/**
 * @addtogroup PTP_ASIC Low Level Driver
 * @brief PTP Low Level Driver.
 * @{
 */

/**
 * @brief Get PPS enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in]  ppsIdx       Pulse Per Second index (0, 1) \n
 *                          Pulse Per Second index, range is 0, 1
 * @param[out] ppsenable    Pulse Per Second enable status (0, 1) \n
 *                          Pointer to Pulse Per Second enable status, range is 0,1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_ppsenable(uint32 unit, uint16 ppsIdx, uint8 *ppsenable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == ppsenable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if((ppsIdx != 0u) && (ppsIdx != 1u))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM PPS enable configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_PPSENABLE + (uint32)ppsIdx, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *ppsenable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set PPS enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] ppsIdx        Pulse Per Second index (0, 1) \n
 *                          Pulse Per Second index, range is 0, 1
 * @param[in] ppsenable     Pulse Per Second enable status (0, 1) \n
 *                          To set Pulse Per Second enable status, range is 0,1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_ppsenable(uint32 unit, uint16 ppsIdx, uint8 ppsenable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        if((ppsIdx != 0u) && (ppsIdx != 1u))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM PPS enable configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_PPSENABLE + (uint32)ppsIdx, ppsenable);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get PPS enable status
 * @param[in]  unit         Unit \n
 *                          Stacking unit
 * @param[in]  ppsidx       Pulse Per Second index (0, 1) \n
 *                          To get which pps index enable status, range is 0, 1
 * @param[out] pppsEn       Pointer to Pulse Per Second enable status (0, 1) \n
 *                          Pointer indicate to pps enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_tai_pps_en(uint32 unit, uint8 ppsidx, uint8 *pppsEn)
{
    uint32    u32data     = 0u;
    uint32    fieldOffset = 0u;
    uint32    fildlen     = 0u;
    RtkApiRet retv        = RT_ERR_OK;
    RtkApiRet apiret      = RT_ERR_OK;
    do
    {
        if(NULL == pppsEn)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* PPS index must be 0 or 1. */
        if(0u == ppsidx)
        {
            fieldOffset = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB1_MAC_EN_LSP;
            fildlen     = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB1_MAC_EN_LEN;
        }
        else if(1u == ppsidx)
        {
            fieldOffset = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB2_MAC_EN_LSP;
            fildlen     = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB2_MAC_EN_LEN;
        }
        else
        {
            retv = -RT_ERR_INPUT;
            continue;
        }

        /* Get TAI GPIO function config register status */
        apiret = dv1_reg_field_read(unit, DV1_TAI_GPIO_FUNC_CFG, fieldOffset, fildlen, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pppsEn = (uint8)u32data;
    } while(0u == 1u);
    return retv;
}

/**
 * @brief  Set hardware PPS enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] ppsidx        Pulse Per Second index (0, 1) \n
 *                          To set which pps index enable status, range is 0, 1
 * @param[in] ppsEn         Pulse Per Second enable status (0, 1)\n
 *                          To set pps enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_sys_pps_en(uint32 unit, uint8 ppsidx, uint8 ppsEn)
{
    uint32    u32data     = (uint32)ppsEn;
    uint32    fieldOffset = 0u;
    uint32    fildlen     = 0u;
    uint32    gpioPadAddr = 0u;
    uint32    gpioPadLsp  = 0u;
    uint32    gpioPadLen  = 0u;
    RtkApiRet retv        = RT_ERR_OK;
    RtkApiRet apiret      = RT_ERR_OK;
    do
    {
        /* PPS index must be 0 or 1. */
        if(0u == ppsidx)
        {
            gpioPadAddr = DV1_PAD_MUX_GPIOB_1;
            gpioPadLsp  = DV1_PAD_MUX_GPIOB_1_RG_GPIOB_1_FUNC_SEL_LSP;
            gpioPadLen  = DV1_PAD_MUX_GPIOB_1_RG_GPIOB_1_FUNC_SEL_LEN;
            fieldOffset = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB1_MAC_EN_LSP;
            fildlen     = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB1_MAC_EN_LEN;
        }
        else if(1u == ppsidx)
        {
            gpioPadAddr = DV1_PAD_MUX_GPIOB_2;
            gpioPadLsp  = DV1_PAD_MUX_GPIOB_2_RG_GPIOB_2_FUNC_SEL_LSP;
            gpioPadLen  = DV1_PAD_MUX_GPIOB_2_RG_GPIOB_2_FUNC_SEL_LEN;
            fieldOffset = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB2_MAC_EN_LSP;
            fildlen     = DV1_TAI_GPIO_FUNC_CFG_TAI_GPIOB2_MAC_EN_LEN;
        }
        else
        {
            retv = -RT_ERR_INPUT;
            continue;
        }
        /* Switch function mux */
        apiret = dv1_reg_field_write(unit, gpioPadAddr, gpioPadLsp, gpioPadLen, 0u);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set TAI GPIO function config register */
        apiret = dv1_reg_field_write(unit, DV1_TAI_GPIO_FUNC_CFG, fieldOffset, fildlen, u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retv;
}

/**@}*/ /* PTP_ASIC */
/**@}*/ /* PTP_TIME */

/**
 * @addtogroup PTP_CLOCK Clock related configuration
 * @{
 */

/**
 * @addtogroup PTP_ASIC Low Level Driver
 * @brief PTP Low Level Driver.
 * @{
 */

/**
 * @brief Get per port time sync enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] ptimeSyncEn   Pointer to time sync enable status (0, 1) \n
 *                          Pointer indicate the time sync enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_port_time_sync_en(uint32 unit, RtkPort port, uint8 *ptimeSyncEn)
{
    uint32    u32data = 0u;
    RtkApiRet apiret  = RT_ERR_OK;
    RtkApiRet retv    = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retv = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == ptimeSyncEn)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get time sync enable register. */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_EAV_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EAV_CONTROL_TIME_SYNC_EN_LSP, DV1_PORT0_EAV_CONTROL_TIME_SYNC_EN_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *ptimeSyncEn = (uint8)u32data;
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Set per port time sync enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] timeSyncEn    Time sync enable status (0, 1)\n
 *                          To set time sync enable, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                    The API was executed correctly.
 * @retval -RT_ERR_PORT_ID               There was wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_port_time_sync_en(uint32 unit, RtkPort port, uint8 timeSyncEn)
{
    RtkApiRet apiret = RT_ERR_OK;
    RtkApiRet retv   = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retv = -RT_ERR_PORT_ID;
            continue;
        }

        /* Set time sync enable register. */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_EAV_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EAV_CONTROL_TIME_SYNC_EN_LSP, DV1_PORT0_EAV_CONTROL_TIME_SYNC_EN_LEN, (uint32)timeSyncEn);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retv;
}

/**
 * @brief Get PTP enable status
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[out] pptpEnable    Pointer to ptp hardware enable status (0, 1) \n
 *                           Pointer indicate to ptp hardware enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_hw_enable(uint32 unit, uint8 *pptpEnable)
{
    uint32    u32data = 0u;
    RtkApiRet apiret  = RT_ERR_OK;
    RtkApiRet retv    = RT_ERR_OK;
    do
    {
        if(NULL == pptpEnable)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get PTP enable register status */
        apiret = dv1_reg_field_read(unit, DV1_PTP_CONTROL_AND_CLK_CONFIG, DV1_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LSP, DV1_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pptpEnable = (uint8)u32data;
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Set hardware PTP enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] ptpEnable     PTP hardware enable status (0, 1) \n
 *                          To set PTP hardware enable, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_hw_enable(uint32 unit, uint8 ptpEnable)
{
    RtkApiRet apiret = RT_ERR_OK;
    RtkApiRet retv   = RT_ERR_OK;
    do
    {
        /* Set PTP enable register */
        apiret = dv1_reg_field_write(unit, DV1_PTP_CONTROL_AND_CLK_CONFIG, DV1_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LSP, DV1_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LEN, (uint32)ptpEnable);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retv;
}

/**
 * @brief Get system time enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[out] psysTimeEn   Pointer to system time enable status (0, 1) \n
 *                          Pointer indicate to system time enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_sys_time_en(uint32 unit, uint8 *psysTimeEn)
{
    uint32    u32data = 0u;
    RtkApiRet apiret  = RT_ERR_OK;
    RtkApiRet retv    = RT_ERR_OK;
    do
    {
        if(NULL == psysTimeEn)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get system time enable register status */
        apiret = dv1_reg_field_read(unit, DV1_EAV_GLOBAL_CONTROL, DV1_EAV_GLOBAL_CONTROL_SYS_TIME_EN_LSP, DV1_EAV_GLOBAL_CONTROL_SYS_TIME_EN_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *psysTimeEn = (uint8)u32data;
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Set system time enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] sysTimeEn     System time enable status (0, 1) \n
 *                          To set system time enable, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_sys_time_en(uint32 unit, uint8 sysTimeEn)
{
    RtkApiRet apiret = RT_ERR_OK;
    RtkApiRet retv   = RT_ERR_OK;
    do
    {
        /* Set system time enable register */
        apiret = dv1_reg_field_write(unit, DV1_EAV_GLOBAL_CONTROL, DV1_EAV_GLOBAL_CONTROL_SYS_TIME_EN_LSP, DV1_EAV_GLOBAL_CONTROL_SYS_TIME_EN_LEN, (uint32)sysTimeEn);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Get MASTER clock enable status
 * @param[in]   unit         Unit \n
 *                           Stacking unit
 * @param[out]  pmasterEn    Pointer to master clock enable status (0, 1) \n
 *                           Pointer indicate to master clock enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_master_en(uint32 unit, uint8 *pmasterEn)
{
    uint32    u32data = 0u;
    RtkApiRet retval  = RT_ERR_OK;
    RtkApiRet apiret  = RT_ERR_OK;

    do
    {

        if(NULL == pmasterEn)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get master clock register enable status */
        apiret = dv1_reg_field_read(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_MASTER_EN_LSP, DV1_PTP_CLOCK_CTRL_MASTER_EN_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pmasterEn = (uint8)u32data;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set MASTER clock enable
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  masterEn      Master clock enable status (0, 1) \n
 *                           To set master clock enable, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval
 */
RtkApiRet dv1_ptp_set_master_en(uint32 unit, uint8 masterEn)
{
    RtkApiRet retval = RT_ERR_OK;
    RtkApiRet apiret = RT_ERR_OK;

    do
    {
        /* Set master clock register */
        apiret = dv1_reg_field_write(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_MASTER_EN_LSP, DV1_PTP_CLOCK_CTRL_MASTER_EN_LEN, (uint32)masterEn);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get SLAVE2 clock enable status
 * @param[in]  unit         Unit \n
 *                          Stacking unit
 * @param[out] pslave2En    Pointer to slave2 clock enable status (0, 1) \n
 *                          Pointer indicate to slave2 clock enable statusrange is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_slave2_en(uint32 unit, uint8 *pslave2En)
{
    uint32    u32data = 0u;
    RtkApiRet retval  = RT_ERR_OK;
    RtkApiRet apiret  = RT_ERR_OK;

    do
    {

        if(NULL == pslave2En)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get slave2 clock register enable status */
        apiret = dv1_reg_field_read(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_SLAVE2_EN_LSP, DV1_PTP_CLOCK_CTRL_SLAVE2_EN_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pslave2En = (uint8)u32data;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set SLAVE2 clock enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] slave2En      SLAVE2 enable status (0, 1) \n
 *                          To set slave2 clock enable status, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_slave2_en(uint32 unit, uint8 slave2En)
{
    RtkApiRet retval = RT_ERR_OK;
    RtkApiRet apiret = RT_ERR_OK;

    do
    {
        /* Set slave2 clock register */
        apiret = dv1_reg_field_write(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_SLAVE2_EN_LSP, DV1_PTP_CLOCK_CTRL_SLAVE2_EN_LEN, (uint32)slave2En);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get SLAVE1 clock enable status
 * @param[in]   unit        Unit \n
 *                          Stacking unit
 * @param[out]  pslave1En   Pointer to SLAVE1 enable status (0, 1) \n
 *                          Pointer indicate to SLAVE1 enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_slave1_en(uint32 unit, uint8 *pslave1En)
{
    uint32    u32data = 0u;
    RtkApiRet retval  = RT_ERR_OK;
    RtkApiRet apiret  = RT_ERR_OK;

    do
    {
        if(NULL == pslave1En)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get slave1 clock register enable status */
        apiret = dv1_reg_field_read(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_SLAVE1_EN_LSP, DV1_PTP_CLOCK_CTRL_SLAVE1_EN_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pslave1En = (uint8)u32data;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set SLAVE1 clock enable status
 * @param[in]   unit        Unit \n
 *                          Stacking unit
 * @param[in]   slave1En    SLAVE1 clock enable status (0, 1) \n
 *                          To set SLAVE1 clock enable status, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_slave1_en(uint32 unit, uint8 slave1En)
{
    RtkApiRet retval = RT_ERR_OK;
    RtkApiRet apiret = RT_ERR_OK;
    do
    {
        /* Set slave1 clock register enable status */
        apiret = dv1_reg_field_write(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_SLAVE1_EN_LSP, DV1_PTP_CLOCK_CTRL_SLAVE1_EN_LEN, (uint32)slave1En);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get SLAVE0 clock enable status
 * @param[in]   unit        Unit \n
 *                          Stacking unit
 * @param[in]  pslave0En    Pointer to SLAVE0 enable status (0, 1) \n
 *                          Pointer indicate to SLAVE0 enable statusrange is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_slave0_en(uint32 unit, uint8 *pslave0En)
{
    uint32    u32data = 0u;
    RtkApiRet retval  = RT_ERR_OK;
    RtkApiRet apiret  = RT_ERR_OK;

    do
    {

        if(NULL == pslave0En)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get slave0 clock register enable status */
        apiret = dv1_reg_field_read(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_SLAVE0_EN_LSP, DV1_PTP_CLOCK_CTRL_SLAVE0_EN_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pslave0En = (uint8)u32data;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set SLAVE0 clock enable status
 * @param[in]   unit        Unit \n
 *                          Stacking unit
 * @param[in]  slave0En     SLAVE0 clock enable status (0, 1) \n
 *                          To set SLAVE0 clock enable status, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_slave0_en(uint32 unit, uint8 slave0En)
{
    RtkApiRet retval = RT_ERR_OK;
    RtkApiRet apiret = RT_ERR_OK;

    do
    {
        /* Set slave0 clock register */
        apiret = dv1_reg_field_write(unit, DV1_PTP_CLOCK_CTRL, DV1_PTP_CLOCK_CTRL_SLAVE0_EN_LSP, DV1_PTP_CLOCK_CTRL_SLAVE0_EN_LEN, (uint32)slave0En);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief When RTL906x_7x is time aware bridge (TC+BC), it will sync time from its slave port. \n
 *      If the time difference between RTL906x_7x and GM is less than pre-defined value 75ns.  \n
 *      Protocol will increase this counter. And the maximum value is 5. \n
 *      This counter does not mean PPS result between GM and RTL906x_7x is correct.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] clkstable    Pointer to the clock stability counter. \n
 *                          A value greater than 0 indicates stability; 0 indicates instability.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter others than port and pointer, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_clkstable_get(uint32 unit, uint32 domainid, uint8 *clkstable)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint8     cfgdata = 0u;
    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == clkstable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM clock stable status. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_CLKSTABLE + domainid, &cfgdata);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *clkstable = cfgdata;
    } while(0u == 1u);
    return retval;
}

/**@}*/ /* PTP_ASIC */
/**@}*/ /* PTP_CLOCK */

/**
 * @addtogroup PTP_PARSER Packet parser related configuration
 * @{
 */

/**
 * @addtogroup PTP_ASIC Low Level Driver
 * @brief PTP Low Level Driver.
 * @{
 */

/**
 * @brief Get redirect port mask value
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] pcpumask      Pointer to CPU mask (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                          Bitmask to indicate the CPU port
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_redirect_cpumask(uint32 unit, uint16 *pcpumask)
{
    uint32    u32data              = 0u;
    RtkApiRet retv                 = RT_ERR_OK;
    RtkApiRet apiret               = RT_ERR_OK;
    uint8     isEnabled            = 0u;
    uint16    ethertype            = 0u;
    uint8     DA[DV1_MAC_ADDR_LEN] = {0x1u, 0x80u, 0xC2u, 0x0u, 0x0u, 0xEu};
    uint16    spmask               = 0u;
    uint8     priority             = 0u;
    uint32    redirectPortMsk      = 0u;

    do
    {
        if(NULL == pcpumask)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get PTP ACL configuration. */
        apiret  = dv1_drv_ptp_acl_redirect_get(unit, &isEnabled, &ethertype, DA, &spmask, &priority, &redirectPortMsk);
        u32data = redirectPortMsk;

        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pcpumask = (uint16)u32data;
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Set redirect port mask value
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] cpumask       CPU mask (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                          Bitmask to indicate the CPU port
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_redirect_cpumask(uint32 unit, uint16 cpumask)
{
    RtkApiRet retv                 = RT_ERR_OK;
    uint32    u32data              = (uint32)cpumask;
    RtkApiRet apiret               = RT_ERR_OK;
    uint8     isEnabled            = 0u;
    uint16    ethertype            = 0u;
    uint8     DA[DV1_MAC_ADDR_LEN] = {0x1u, 0x80u, 0xC2u, 0x0u, 0x0u, 0xEu};
    uint16    spmask               = 0u;
    uint8     priority             = 0u;
    uint32    redirectPortMsk      = 0u;

    do
    {
        if(0u != (cpumask & ~(DV1_RTK_USER_PMAP_ALL | DV1_RTK_CPU_PMAP)))
        {
            retv = -RT_ERR_INPUT;
            continue;
        }

        /* Get PTP ACL configuration */
        apiret = dv1_drv_ptp_acl_redirect_get(unit, &isEnabled, &ethertype, DA, &spmask, &priority, &redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        redirectPortMsk = u32data;
        /* Set PTP ACL to redirect PTP packets. */
        apiret = dv1_drv_ptp_acl_redirect_set(unit, isEnabled, ethertype, DA, spmask, priority, redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Get action of redirect PTP packet.
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[out]  pptpRedirAct Pointer to redirect action enable status (0, 1) \n
 *                           Pointer indicate redirect action enable status range is 0, 1
 * @retval  RT_ERR_OK             The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER   There was wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_get_redirect_act(uint32 unit, uint8 *pptpRedirAct)
{
    RtkApiRet retv                 = RT_ERR_OK;
    RtkApiRet apiret               = RT_ERR_OK;
    uint8     isEnabled            = 0u;
    uint16    ethertype            = 0u;
    uint8     DA[DV1_MAC_ADDR_LEN] = {0x1u, 0x80u, 0xC2u, 0x0u, 0x0u, 0xEu};
    uint16    spmask               = 0u;
    uint8     priority             = 0u;
    uint32    redirectPortMsk      = 0u;

    do
    {
        if(NULL == pptpRedirAct)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get PTP ACL configuration */
        apiret = dv1_drv_ptp_acl_redirect_get(unit, &isEnabled, &ethertype, DA, &spmask, &priority, &redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(1u == isEnabled)
        {
            *pptpRedirAct = 0x1u; /*trap*/
        }
        else
        {
            *pptpRedirAct = 0x0u; /*normal forward*/
        }
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Set action for redirect PTP packet
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in]  ptpRedirAct   Redirect action enable status (0, 1) \n
 *                           To set redirect action enable status, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_redirect_act(uint32 unit, uint8 ptpRedirAct)
{

    RtkApiRet retv                 = RT_ERR_OK;
    RtkApiRet apiret               = RT_ERR_OK;
    uint8     isEnabled            = 0u;
    uint16    ethertype            = 0u;
    uint8     DA[DV1_MAC_ADDR_LEN] = {0x1u, 0x80u, 0xC2u, 0x0u, 0x0u, 0xEu};
    uint16    spmask               = 0u;
    uint8     priority             = 0u;
    uint32    redirectPortMsk      = 0u;
    do
    {
        /* Get PTP ACL configuration */
        apiret = dv1_drv_ptp_acl_redirect_get(unit, &isEnabled, &ethertype, DA, &spmask, &priority, &redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(1u == ptpRedirAct)
        {
            isEnabled = 1u;
        }
        else
        {
            isEnabled = 0u;
        }
        /* Set PTP ACL configuration */
        apiret = dv1_drv_ptp_acl_redirect_set(unit, isEnabled, ethertype, DA, spmask, priority, redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Get ethertype check enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] checkEnable   Check enable statusv(0, 1) \n
 *                          Pointer to indicate check PTP Ethertype enable status, range is 0, 1
 * @retval RT_ERR_OK                      The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_ether_check_enable0(uint32 unit, RtkPort port, uint8 *checkEnable)
{
    uint32    u32data = 0u;
    RtkApiRet retv    = RT_ERR_OK;
    int32     apiret  = RT_ERR_OK;
    do
    {
        if(NULL == checkEnable)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retv = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get Ethertype check register status */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE0_LSP, DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE0_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *checkEnable = (uint8)u32data;
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Turn on this rx parser will enable ethertype filtering during RX.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] checkEnable   Check enable status (0, 1) \n
 *                          To set ethertype check enable status, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_ether_check_enable0(uint32 unit, RtkPort port, uint8 checkEnable)
{
    RtkApiRet retv   = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retv = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set Ethertype check register */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE0_LSP, DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE0_LEN, (uint32)checkEnable);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Get latch timestamp timing from rxdv/txen(MII)
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                           Port index
 * @param[out]  pptp100tx    Pointer to ptp_100tx (0, 1, 2) \n
 *                           Pointer to latch timestamp timing, range is 0: MAC, 1: Phy, 2: Interface.
 * @retval  RT_ERR_OK                    The API was executed correctly.
 * @retval -RT_ERR_PORT_ID               There was wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_port_ptp_100tx(uint32 unit, RtkPort port, uint8 *pptp100tx)
{
    uint32    u32data = 0u;
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == pptp100tx)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get 100TX latch time register */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_PTP_100TX_LSP, DV1_PORT0_802DOT1AS_CONFIG_PTP_100TX_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pptp100tx = (uint8)u32data;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Latch timestamp timing from rxdv/txen(MII)
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                           Port index
 * @param[in]  ptp100tx      ptp_100tx timing (0, 1, 2) \n
 *                           To set latch timestamp timing, range is 0: MAC, 1: Phy, 2: Interface.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_port_ptp_100tx(uint32 unit, RtkPort port, uint8 ptp100tx)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set 100TX latch time register */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_PTP_100TX_LSP, DV1_PORT0_802DOT1AS_CONFIG_PTP_100TX_LEN, (uint32)ptp100tx);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get compensation time for ingress management plane
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                           Port index
 * @param[out] prx_latency   Pointer to receipt latency (0 to 0xFFFF) \n
 *                           Pointer indicate to receipt latency
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_port_rxlatency(uint32 unit, RtkPort port, uint16 *prxLatency)
{
    uint32    u32data = 0u;
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == prxLatency)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get ingress latency correction register */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_RX_LATENCY_LSP, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_RX_LATENCY_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *prxLatency = (uint16)(u32data);
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set compensation time for ingress management plane
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                           Port index
 * @param[out] rxLatency     Receipt latency (0 to 0xFFFF) \n
 *                           To set ingress latency
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_port_rxlatency(uint32 unit, RtkPort port, uint16 rxLatency)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }

        /* Set ingress latency correction register */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_RX_LATENCY_LSP, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_RX_LATENCY_LEN, (uint32)rxLatency);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get compensation time for egress management plane
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                           Port index
 * @param[out] tx_latency    Pointer to transmit latency (0 to 0xFFFF) \n
 *                           Pointer indicate to transmit latency
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID               There was wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_port_txlatency(uint32 unit, RtkPort port, uint16 *ptxLatency)
{
    uint32    u32data = 0u;
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == ptxLatency)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get egress latency correction register */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_TX_LATENCY_LSP, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_TX_LATENCY_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *ptxLatency = (uint16)u32data;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set compensation time for egress management plane
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] tx_latency   Transmit latency (0 to 0xFFFF) \n
 *                         To set transmit latency
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_port_txlatency(uint32 unit, RtkPort port, uint16 txLatency)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set egress latency correction register */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_TX_LATENCY_LSP, DV1_PORT0_802DOT1AS_LATENCY_CORRECTION_TX_LATENCY_LEN, (uint32)txLatency);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get slave time domain of specific port
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  domainid     Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] domainvalue  Pointer to domain value (0 to 0xFF) \n
 *                          The pointer indicate to the number of this domain
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_port_slave_time_domain(uint32 unit, RtkPort port, uint32 domainid, uint8 *domainvalue)
{
    uint32    u32data     = 0u;
    RtkApiRet retv        = RT_ERR_OK;
    uint32    fieldoffset = 0u;
    uint32    fieldlen    = 0u;
    int32     apiret      = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retv = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retv = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == domainvalue)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* domain index must be 0, 1, or 2 */
        if(domainid == 0u)
        {
            fieldoffset = (uint32)DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN0_LSP;
            fieldlen    = (uint32)DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN0_LEN;
        }
        else if(domainid == 1u)
        {
            fieldoffset = (uint32)DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN1_LSP;
            fieldlen    = (uint32)DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN1_LEN;
        }
        else if(domainid == 2u)
        {
            fieldoffset = (uint32)DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN2_LSP;
            fieldlen    = (uint32)DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN2_LEN;
        }
        else
        {
            retv = -RT_ERR_INPUT;
            continue;
        }
        /* Get time domain value register configuration */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_TIME_DOMAIN_VALUE + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), fieldoffset, fieldlen, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *domainvalue = (uint8)u32data;
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Set slave time domain of specific port
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] domainvalue   Domain value (0 to 0xFF) \n
 *                          The number of this domain
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_port_slave_time_domain(uint32 unit, RtkPort port, uint32 domainid, uint8 domainvalue)
{
    uint32    u32data     = (uint32)domainvalue;
    RtkApiRet retv        = RT_ERR_OK;
    uint32    fieldoffset = 0u;
    uint32    fieldlen    = 0u;
    int32     apiret      = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retv = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retv = -RT_ERR_INPUT;
            continue;
        }
        if(domainid == 0u)
        {
            fieldoffset = DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN0_LSP;
            fieldlen    = DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN0_LEN;
        }
        else if(domainid == 1u)
        {
            fieldoffset = DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN1_LSP;
            fieldlen    = DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN1_LEN;
        }
        else if(domainid == 2u)
        {
            fieldoffset = DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN2_LSP;
            fieldlen    = DV1_PORT0_TIME_DOMAIN_VALUE_SLAVE_TIME_DOMAIN2_LEN;
        }
        else
        {
            retv = -RT_ERR_INPUT;
            continue;
        }
        /* Set time domain value register */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_TIME_DOMAIN_VALUE + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), fieldoffset, fieldlen, u32data);
        if(RT_ERR_OK != apiret)
        {
            retv = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retv;
}

/**
 * @brief Set Custom EtherType of PTP
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  ethertype    Customized ethertype (>= 0x600) \n
 *                          The Customized ethertype, value is >= 0x600
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_custom_ethertype(uint32 unit, RtkPort port, uint16 ethertype)
{
    int32  ret                    = RT_ERR_OK;
    int32  apicall                = RT_ERR_OK;
    uint16 bigendian_ethertype    = 0u;
    uint16 littleendian_ethertype = 0u;
    uint32 address                = DV1_PTPCFG_OPTIONETHERHEADER;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
#ifdef RTK_LITTLE_ENDIAN
        /*Change little to big*/
        bigendian_ethertype = (ethertype >> 8u) & 0xFFu;
        bigendian_ethertype |= (ethertype << 8u) & 0xFF00u;
        littleendian_ethertype = ethertype;
#else
        bigendian_ethertype    = ethertype;
        littleendian_ethertype = (ethertype << 8u) & 0xFFu;
        littleendian_ethertype |= (ethertype >> 8u) & 0xFF00u;
#endif
        /* Set ethertype check register. */
        apicall = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_ETHER_TYPE_FOR_CHECKING + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_ETHER_TYPE_FOR_CHECKING_ETH_FOR_CHECK_LSP, DV1_PORT0_802DOT1AS_ETHER_TYPE_FOR_CHECKING_ETH_FOR_CHECK_LEN, (uint32)littleendian_ethertype);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apicall = dv1_drv_indirect_sram_2bytes_set(unit, address, bigendian_ethertype);

        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get Custom EtherType of PTP
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[out] ethertype    Customized ethertype. (>= 0x600) \n
 *                          Pointer to the customized EtherType of the PTP frame in little-endian format. The value must be greater than or equal to 0x600. \n
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_get_custom_ethertype(uint32 unit, RtkPort port, uint16 *ethertype)
{
    int32  ret     = RT_ERR_OK;
    uint32 regdata = 0u;
    int32  apicall = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == ethertype)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get ethertype check register. */
        apicall = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_ETHER_TYPE_FOR_CHECKING + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_ETHER_TYPE_FOR_CHECKING_ETH_FOR_CHECK_LSP, DV1_PORT0_802DOT1AS_ETHER_TYPE_FOR_CHECKING_ETH_FOR_CHECK_LEN, &regdata);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *ethertype = (uint16)regdata;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Enable/Disable to check custom etherType.
 *      The origianl name is rtk_ptp_custom_etherType_enable_set. MISRA-C 5.1 does not allow this kind of name.
 *      Changed name from rtk_ptp_custom_etherType_enable_set to dv1_ptp_set_ethertype_enable_custom.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set customized ethertype enable status, 0:Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_ethertype_enable_custom(uint32 unit, RtkPort port, uint8 enable)
{
    int32     ret                  = RT_ERR_OK;
    int32     apicall              = RT_ERR_OK;
    uint32    domainid             = 0u;
    uint8     booleanEnable        = 0u;
    uint32    address              = DV1_PTPCFG_ISOPTIONETHERHEADER;
    RtkApiRet apiret               = 0;
    uint8     isEnabled            = 0u;
    uint16    ethertype            = 0u;
    uint8     DA[DV1_MAC_ADDR_LEN] = {0x1u, 0x80u, 0xC2u, 0x0u, 0x0u, 0xEu};
    uint16    spmask               = 0u;
    uint8     priority             = 0u;
    uint32    redirectPortMsk      = 0u;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set ethertype check enable register. */
        apicall = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE1_LSP, DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE1_LEN, (uint32)enable);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(1u == enable)
        {
            booleanEnable = 1u;
        }
        else
        {
            booleanEnable = 0u;
        }
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Set SRAM ether type check enable configuration */
            apicall = dv1_drv_indirect_sram_1byte_set(unit, address + domainid, booleanEnable);
            if(RT_ERR_OK != apicall)
            {
                ret = -RT_ERR_PTP_CALL_API_FAILED;
                continue;
            }
        }
        apiret = dv1_drv_ptp_acl_redirect_get(unit, &isEnabled, &ethertype, DA, &spmask, &priority, &redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(1u == booleanEnable)
        {
            apicall = dv1_ptp_get_custom_ethertype(unit, port, &ethertype);
            if(RT_ERR_OK != apicall)
            {
                ret = -RT_ERR_PTP_CALL_API_FAILED;
                continue;
            }
        }
        else
        {
            ethertype = 0x88F7u;
        }
        /* Set PTP ACL with new ether type */
        apiret = dv1_drv_ptp_acl_redirect_set(unit, isEnabled, ethertype, DA, spmask, priority, redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get Enable/Disable to check custom etherType. \n
 *      The origianl name is rtk_ptp_custom_etherType_enable_get. MISRA-C 5.1 does not allow this kind of name. \n
 *      Changed name from rtk_ptp_custom_etherType_enable_get to dv1_ptp_get_ethertype_enable_custom.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  enable       Enable status (0, 1) \n
 *                          Pointer to enable status, 0:Disable, 1:Enable
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_ethertype_enable_custom(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet ret     = RT_ERR_OK;
    int32     apicall = RT_ERR_OK;
    uint32    regdata = 0u;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get ether type check register. */
        apicall = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE1_LSP, DV1_PORT0_802DOT1AS_CONFIG_ETHER_CHECK_ENABLE1_LEN, &regdata);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)regdata;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief When Enable this, packets with DV1_VLAN header will be checked belonging to PTP or not.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set customized vlan enable status, 0:Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_custom_set_vlan_enable(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet ret     = RT_ERR_OK;
    int32     apicall = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set vlan check register. */
        apicall = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_VLAN_CHECK_ENABLE_LSP, DV1_PORT0_802DOT1AS_CONFIG_VLAN_CHECK_ENABLE_LEN, (uint32)enable);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief When Enable this, packets with DV1_VLAN header will be checked belonging to PTP or not.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  enable       Enable status pointer (0, 1) \n
 *                          Pointer indicated to customized vlan enable status, 0:Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_custom_get_vlan_enable(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regdata = 0u;
    int32     apicall = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get vlan check register. */
        apicall = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_VLAN_CHECK_ENABLE_LSP, DV1_PORT0_802DOT1AS_CONFIG_VLAN_CHECK_ENABLE_LEN, &regdata);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)regdata;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Configure Custom DA of PTP (using while parsing incoming packets)
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  damac        Destination MAC address. (N/A) \n
 *                          It's 6 octects arrary,DA MAC[6]. For example, 00:e0:4c:00:01:02 = DAMAC[6] = {0x00, 0xe0, 0x4c, 0x00, 0x01, 0x02}
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_custom_damac01(uint32 unit, RtkPort port, uint8 const *const damac)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    da00    = 0u;
    uint32    da01    = 0u;
    int32     apicall = RT_ERR_OK;

    uint32 address = DV1_PTPCFG_OPTIONDA;
    uint16 u16data = 0u;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == damac)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        da01 = (((uint32)damac[0]) << 8u) | (((uint32)damac[1]));
        da00 = (((uint32)damac[2]) << 24u) | (((uint32)damac[3]) << 16u) | (((uint32)damac[4]) << 8u) | (((uint32)damac[5]));
        /* Set destination address check register, 0~31 bits */
        apicall = dv1_reg_write(unit, DV1_PORT0_802DOT1AS_DESTINATION_ADDRESS_FOR_CHECKING0_31_0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), da00);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set destination address check register, 32~48 bits */
        apicall = dv1_reg_write(unit, DV1_PORT0_802DOT1AS_DESTINATION_ADDRESS_FOR_CHECKING0_47_32 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), da01);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        u16data = (((uint16)damac[0])) | (((uint16)damac[1]) << 8u);
        /* Set SRAM destination address check configuration 1st and 2nd byte. */
        CHK_FUN_CONTINUE(ret, dv1_drv_indirect_sram_2bytes_set(unit, address, u16data));
        u16data = (((uint16)damac[2])) | (((uint16)damac[3]) << 8u);
        /* Set SRAM destination address check configuration 3rd and 4th byte. */
        CHK_FUN_CONTINUE(ret, dv1_drv_indirect_sram_2bytes_set(unit, address + 2u, u16data));
        u16data = (((uint16)damac[4])) | (((uint16)damac[5]) << 8u);
        /* Set SRAM destination address check configuration 5th and 6th byte. */
        apicall = dv1_drv_indirect_sram_2bytes_set(unit, address + 4u, u16data);

        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get custom DA of PTP (using while parsing incoming packets).
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  damac:       Destination MAC address. (N/A) \n
 *                          It's 6 octects arrary,DA MAC[6]. For example, 00:e0:4c:00:01:02 = DAMAC[6] = {0x00, 0xe0, 0x4c, 0x00, 0x01, 0x02}
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_custom_damac01(uint32 unit, RtkPort port, uint8 *damac)
{
    RtkApiRet ret       = RT_ERR_OK;
    uint32    da00      = 0u;
    uint32    da01      = 0u;
    int32     apicall   = RT_ERR_OK;
    uint32    cmpresult = 0u;

    uint32 address = DV1_PTPCFG_OPTIONDA;
    uint16 u16data = 0u;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == damac)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get destination address check register, 0~31 bits */
        apicall = dv1_reg_read(unit, DV1_PORT0_802DOT1AS_DESTINATION_ADDRESS_FOR_CHECKING0_31_0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), &da00);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Get destination address check register, 32~47 bits */
        apicall = dv1_reg_read(unit, DV1_PORT0_802DOT1AS_DESTINATION_ADDRESS_FOR_CHECKING0_47_32 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), &da01);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        damac[0] = (uint8)((da01 & (uint32)0xFF00u) >> 8u);
        damac[1] = (uint8)(da01 & (uint32)0xFFu);
        damac[2] = (uint8)((da00 & (uint32)0xFF000000u) >> 24u);
        damac[3] = (uint8)((da00 & (uint32)0xFF0000u) >> 16u);
        damac[4] = (uint8)((da00 & (uint32)0xFF00u) >> 8u);
        damac[5] = (uint8)(da00 & (uint32)0xFFu);

        /* Get SRAM destination address check configuration 1st and 2nd byte. */
        CHK_FUN_CONTINUE(ret, (dv1_drv_indirect_sram_2bytes_get(unit, address, &u16data)));
        if(u16data != ((((uint16)damac[0])) | (((uint16)damac[1]) << 8u)))
        {
            cmpresult = 1u;
        }
        /* Get SRAM destination address check configuration 3rc and 4th byte. */
        CHK_FUN_CONTINUE(ret, (dv1_drv_indirect_sram_2bytes_get(unit, address + 2u, &u16data)));
        if(u16data != ((((uint16)damac[2])) | (((uint16)damac[3]) << 8u)))
        {
            cmpresult = 1u;
        }
        /* Get SRAM destination address check configuration 5th and 6th byte. */
        CHK_FUN_CONTINUE(ret, (dv1_drv_indirect_sram_2bytes_get(unit, address + 4u, &u16data)));
        if(u16data != ((((uint16)damac[4])) | (((uint16)damac[5]) << 8u)))
        {
            cmpresult = 1u;
        }
        if(0u != cmpresult)
        {
            /*Hardware and protocol configuration does not match. Call set again to wirte them well*/
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief When Enable this, packets with non-standard DMAC is compared with custom DMAC settings
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set customized destination MAC enable status, 0:Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_damac01_enable_custom(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet ret                  = RT_ERR_OK;
    int32     apicall              = RT_ERR_OK;
    uint8     da_sel               = 0u;
    uint32    domainid             = 0u;
    uint32    address              = DV1_PTPCFG_DASEL;
    RtkApiRet apiret               = 0;
    uint8     isEnabled            = 0u;
    uint16    ethertype            = 0u;
    uint8     DA[DV1_MAC_ADDR_LEN] = {0x1u, 0x80u, 0xC2u, 0x0u, 0x0u, 0xEu};
    uint16    spmask               = 0u;
    uint8     priority             = 0u;
    uint32    redirectPortMsk      = 0u;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set destination address check enable register. */
        apicall = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_DA_MATCH_ENABLE1_LSP, DV1_PORT0_802DOT1AS_CONFIG_DA_MATCH_ENABLE1_LEN, (uint32)enable);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(1u == enable)
        {
            da_sel = 2u;
        }
        else
        {
            da_sel = 0u;
        }

        for(domainid = 0u; domainid < 3u; domainid++)
        {
            /* Set SRAM destination address check enable configuration. */
            apicall = dv1_drv_indirect_sram_1byte_set(unit, address + domainid, da_sel);
            if(RT_ERR_OK != apicall)
            {
                ret = -RT_ERR_PTP_CALL_API_FAILED;
                continue;
            }
        }

        apiret = dv1_drv_ptp_acl_redirect_get(unit, &isEnabled, &ethertype, DA, &spmask, &priority, &redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if(1u == enable)
        {
            apicall = dv1_ptp_get_damac01_enable_custom(unit, port, DA);
            if(RT_ERR_OK != apicall)
            {
                ret = -RT_ERR_PTP_CALL_API_FAILED;
                continue;
            }
        }
        /* Set PTP ACL with new destination address. */
        apiret = dv1_drv_ptp_acl_redirect_set(unit, isEnabled, ethertype, DA, spmask, priority, redirectPortMsk);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief When enable this, packets with non-standard DMAC is compared with custom DMAC settings
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] enable        Enable status pointer (0, 1) \n
 *                          Pointer indicated to customized destination mac enable status, 0:Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_damac01_enable_custom(uint32 unit, RtkPort port, uint8 *enable)
{
    int32  ret     = RT_ERR_OK;
    uint32 regdata = 0u;
    int32  apicall = RT_ERR_OK;
    uint8  da_sel  = 0u;

    uint32 address = DV1_PTPCFG_DASEL;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get destination address check enable register. */
        apicall = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_DA_MATCH_ENABLE1_LSP, DV1_PORT0_802DOT1AS_CONFIG_DA_MATCH_ENABLE1_LEN, &regdata);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = (uint8)regdata;

        /* Get SRAM destination address check enable configuration. */
        apicall = dv1_drv_indirect_sram_1byte_get(unit, address, &da_sel);
        if(RT_ERR_OK != apicall)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        if((1u == *enable) && (2u != da_sel))
        {
            /*Hardware and protocol configuration does not match. Call set again to wirte them well*/
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Tag info of domain. DV1_VLAN tag is requrited from OPEN TC11. \n
 * The following text comes from 802.1AS standard:\n
 * Frames carrying IEEE 802.1AS messages are neither DV1_VLAN-tagged nor priority-tagged, i.e., they are\n
 * untagged (see 11.3.3).
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] ptpqTaginfo   802.1Q tag information (N/A) \n
 *                          Tag Info of domainid. Please set the qtag[0]
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_tag_set(uint32 unit, uint32 domainid, ptpqTaginfo *taginfo)
{
    RtkApiRet retval    = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret    = RT_ERR_OK;
    uint16    taglength = 0u;
    uint32    offset    = (DV1_PTPCFG_TAG + ((uint32)domainid * 0xCu));
    uint8     u8data    = 0u;
    uint16    u16data   = 0u;
    do
    {
        if(NULL == taginfo)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if((taginfo->qtag[0].tci > 4095u) || (taginfo->qtag[1].tci > 4095u))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(1u == taginfo->qtag[0].enable)
        {
            taglength += 4u;
        }
        if(1u == taginfo->qtag[1].enable)
        {
            taglength += 4u;
        }
        taginfo->tagLength = (uint8)taglength;

        u16data = taginfo->qtag[0].tci;
        /* Set SRAM qtag index0 infomation tci(tag control information). */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, offset, u16data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }

        offset += 2u;
        u8data = taginfo->qtag[0].enable;
        /* Set SRAM qtag index0 infomation enable. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }

        offset += 2u;
        u16data = taginfo->qtag[1].tci;
        /* Set SRAM qtag index1 infomation tci(tag control information). */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, offset, u16data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }

        offset += 2u;
        u8data = taginfo->qtag[1].enable;
        /* Set SRAM qtag index1 infomation enable. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }

        offset += 2u;
        u8data = taginfo->fix;
        /* Set SRAM qtag infomation fix. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }

        offset += 1u;
        u8data = taginfo->enable;
        /* Set SRAM qtag infomation enable. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }

        offset += 1u;
        u8data = taginfo->tagLength;
        /* Set SRAM qtag infomation tag length. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get hardware interface latency of egress or ingress.
 * @param[in]  direction Direction (0, 1) \n
 *                       The direction of interface latency. 0: ingress, 1: egress.
 * @param[in]  interface Interface (EM_PORT_TYPE_RTPGE_1000BASE_T1 to DV1_EM_PORT_TYPE_END)\n
 *                       The interface to get latency
 * @param[out] latency   Latency \n
 *                       Latency of the specific direction and interface
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_get_interface_latency(uint32 unit, uint8 direction, PtpPortType_e interface, uint16 *latency)
{
    RtkApiRet retv = RT_ERR_OK;

    do
    {
        if((0u != direction) && (1u != direction))
        {
            retv = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == latency)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Return latency accroding to direction and interface */
        if(0u == direction)
        {
            retv = dv1_drv_ptp_if_rxlatency_get(interface, latency);
        }
        else
        {
            retv = dv1_drv_ptp_if_txlatency_get(interface, latency);
        }

    } while(0u == 1u);

    return retv;
}

/**
 * @brief Get Tag info of domain. DV1_VLAN tag is requrited from OPEN TC11. The following text comes from 802.1AS standard: \n
 * Frames carrying IEEE 802.1AS messages are neither DV1_VLAN-tagged nor priority-tagged, i.e., they are untagged (see 11.3.3).
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] taginfo      Tag Info of domainid (N/A) \n
 *                          The structure pointer to DV1_VLAN tag information.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_tag_get(uint32 unit, uint32 domainid, ptpqTaginfo *taginfo)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = 0;
    uint32    offset = (DV1_PTPCFG_TAG + ((uint32)domainid * 0xCu));

    uint8  u8data  = 0u;
    uint16 u16data = 0u;
    do
    {
        if(NULL == taginfo)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM qtag index0 tci(tag control information). */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, offset, &u16data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->qtag[0].tci = (uint16)u16data;
        /* Get SRAM qtag index0 enable. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, offset + 2u, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->qtag[0].enable = (uint8)u8data;
        /* Get SRAM qtag index1 tci. */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, offset + 4u, &u16data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->qtag[1].tci = (uint16)u16data;
        /* Get SRAM qtag index1 enable. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, offset + 6u, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->qtag[1].enable = (uint8)u8data;
        /* Get SRAM qtag fix flag. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, offset + 8u, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->fix = (uint8)u8data;
        /* Get SRAM qtag global enable. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, offset + 9u, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->enable = (uint8)u8data;
        /* Get SRAM qtag tag length. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, offset + 10u, &u8data);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        taginfo->tagLength = (uint8)u8data;

        retval = RT_ERR_OK;
    } while(0u == 1u);

    return retval;
}

/**@}*/ /* PTP_ASIC */
/**@}*/ /* PTP_PARSER */

/**
 * @addtogroup PTP_PROTOCOL Protocol stack related configuration
 * @{
 */

/**
 * @addtogroup PTP_ASIC Low Level Driver
 * @brief PTP Low Level Driver.
 * @{
 */

/**
 * @brief Convert log value to related message interval
 * @param[in]  loginterval log interval (-5 to 3, and 127) \n
 *                         Log of interval value
 * @param[out]  interval   interval (2^-5 to 2^3 in ms, and 0xFFFF)\n
 *                         Interval value, Unit is ms.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_log_to_intval(uint32 unit, int8 const loginterval, uint16 *intval)
{
    RtkApiRet retv = RT_ERR_OK;

    do
    {
        if(NULL == intval)
        {
            retv = -RT_ERR_NULL_POINTER;
            continue;
        }
        switch(loginterval)
        {
            case(int8)127:
                *intval = 0xFFFFu;
                break;
            case(int8)3:
                *intval = 8001U;
                break;
            case(int8)2:
                *intval = 4001U;
                break;
            case(int8)1:
                *intval = 2001U;
                break;
            case(int8)0:
                *intval = 1001U;
                break;
            case(int8)-1:
                *intval = 501U;
                break;
            case(int8)-2:
                *intval = 251U;
                break;
            case(int8)-3:
                *intval = 126U;
                break;
            case(int8)-4:
                *intval = 62U;
                break;
            case(int8)-5:
                *intval = 32U;
                break;
            default:
                /* intentionally left blank */
                retv = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return retv;
}

/**
 * @brief Get current pdelay request interval(ms) of specific port
 * @param[in] unit                     Unit \n
 *                                     Stacking unit
 * @param[in] port                     Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                                     Port index
 * @param[out] curntPdelayReqIntval    Pointer to current pdelay request interval (ref to dv1_ptp_log_to_intval) \n
 *                                     Pointer indicate to current pdelay request interval
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_curnt_pdelayreq_intval(uint32 unit, RtkPort port, uint32 *curntPdelayReqIntval)
{
    RtkApiRet retval = RT_ERR_OK;
    uint32    cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == curntPdelayReqIntval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM current pdelay request interval configuration. */
        apiret = dv1_drv_indirect_sram_get(unit, DV1_PTPCFG_CURNTPDELAYREQINTVAL + ((uint32)dv1_g_userPort2sysPort[port] * ((uint32)4u)), &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *curntPdelayReqIntval = cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set current pdelay request interval(ms) of specific port
 * @param[in] unit                    Unit \n
 *                                    Stacking unit
 * @param[in] port                    Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                                    The specific port
 * @param[in] curntPdelayReqIntval    Current pdelay request interval (ref to dv1_ptp_log_to_intval) \n
 *                                    To set current pdelay request interval
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_curnt_pdelayreq_intval(uint32 unit, RtkPort port, uint32 curntPdelayReqIntval)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {

        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM current pdelay request interval configuration. */
        apiret = dv1_drv_indirect_sram_set(unit, DV1_PTPCFG_CURNTPDELAYREQINTVAL + ((uint32)dv1_g_userPort2sysPort[port] * ((uint32)4u)), curntPdelayReqIntval);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get hardware port mode of specific port \n
 * All port in the same domain should be the same settings.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in]  portmode     pointer to port mode. (0, 1, 2, 3, 4) \n
 *                          0: Disable, 1: BC mode, 2: TC p2p mode, 3: TC e2e mode, 4:AS mode
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_portmode(uint32 unit, RtkPort port, uint8 *pportmode)
{
    uint32 u32data = 0u;

    int32     apiret = RT_ERR_OK;
    RtkApiRet retval = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == pportmode)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get port mode register configuration */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_PORTMODE_LSP, DV1_PORT0_802DOT1AS_CONFIG_PORTMODE_LEN, &u32data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pportmode = (uint8)u32data;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set hardware port mode of specific port \n
 * All port in the same domain should be the same settings.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH & DV1_RTK_CPU_PORT) \n
 *                          Port index
 * @param[in] portmode      Pointer to port mode (0, 1, 2, 3, 4) \n
 *                          0: Disable, 1: BC mode, 2: TC p2p mode, 3: TC e2e mode, 4:AS mode
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_portmode(uint32 unit, RtkPort port, uint8 portmode)
{
    int32     apiret = RT_ERR_OK;
    RtkApiRet retval = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set port mode register configuration */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_PORTMODE_LSP, DV1_PORT0_802DOT1AS_CONFIG_PORTMODE_LEN, (uint32)portmode);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get PTP global enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] glenable      Pointer to global enable status (0, 1) \n
 *                          Pointer indicate to global enable status , range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_glenable(uint32 unit, uint8 *glenable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == glenable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM global enable configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_GLENABLE, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *glenable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set PTP global enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] glenable      Global enable status (0, 1) \n
 *                          To set global enable status, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_glenable(uint32 unit, uint8 glenable)
{
    RtkApiRet retval               = RT_ERR_OK;
    uint8     cfg                  = (uint8)glenable;
    int32     apiret               = RT_ERR_OK;
    uint8     isEnabled            = 0u;
    uint16    ethertype            = 0u;
    uint8     DA[DV1_MAC_ADDR_LEN] = {0x1u, 0x80u, 0xC2u, 0x0u, 0x0u, 0xEu};
    uint16    spmask               = 0u;
    uint8     priority             = 0u;
    uint32    redirectPortMsk      = 0u;
    uint16    iSSpriorityen        = 0u;
    uint16    cpumask              = 0u;

    do
    {
        CHK_FUN_CONTINUE((apiret), (dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_GLENABLE, cfg)));
        if((1u == glenable))
        {

            /*Align settings of DA, Ethertype and priority: related registers and ACL*/
            CHK_FUN_CONTINUE((apiret), (dv1_drv_ptp_acl_redirect_get(unit, &isEnabled, &ethertype, DA, &spmask, &priority, &redirectPortMsk)));
            CHK_FUN_CONTINUE((apiret), (dv1_drv_ptp_customize_get_in_glenable(unit, &ethertype, DA)));
            CHK_FUN_CONTINUE((apiret), (dv1_ptp_get_en_inter_serv_pri(unit, &iSSpriorityen)));
            CHK_FUN_CONTINUE((apiret), (dv1_ptp_get_inter_serv_pri(unit, &priority)));

            CHK_FUN_CONTINUE((apiret), (dv1_ptp_get_redirect_cpumask(unit, &cpumask)));
            if(0u == cpumask)
            {
                /*No CPU port. Port 15 as default*/
                cpumask = 0x8000u;
            }
            /*The usage is diffent between spmaks, and redirectPortMsk. And
             * the 0 in spmaks means don't case the port.
             * the 0 in redirectPortMsk means that redirect to that port.
             * Hence, the cpumask could assign to both of two.
             */
            spmask          = cpumask;
            redirectPortMsk = (uint32)cpumask;
            /* Set SRAM global enable configuration */
            CHK_FUN_CONTINUE((apiret), (dv1_drv_ptp_acl_redirect_set(unit, isEnabled, ethertype, DA, spmask, priority, redirectPortMsk)));
        }
    } while(0u == 1u);
    if(RT_ERR_OK != apiret)
    {
        retval = -RT_ERR_PTP_CALL_API_FAILED;
    }
    return retval;
}

/**
 * @brief Get grand master config.
 * @param[in]   unit        Unit \n
 *                          Stacking unit
 * @param[out]  cfggm       Pointer to GM mode configuration (0, 1) \n
 *                          GM:1: others:0
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_cfggm(uint32 unit, uint8 *cfggm)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == cfggm)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM GM mode configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_CFGGM, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *cfggm = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set grand master config
 * @param[in]  unit         Unit \n
 *                          Stacking unit
 * @param[in]  cfggm        GM mode configuration (0, 1) \n
 *                          GM:1: others:0
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_cfggm(uint32 unit, uint8 cfggm)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = (uint8)cfggm;
    int32     apiret = RT_ERR_OK;
    do
    {
        /* Set SRAM GM mode configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_CFGGM, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get automotive profile enable status
 * @param[in]  unit           Unit \n
 *                            Stacking unit
 * @param[in]  port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                            Port index
 * @param[out] autoasenable   Pointer to automotive profile enable status (0, 1) \n
 *                            range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_autoasenable(uint32 unit, RtkPort port, uint8 *autoasenable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == autoasenable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM automotive profile configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_AUTOASENABLE + (uint32)dv1_g_userPort2sysPort[port], &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *autoasenable = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set automotive profile enable
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port
 * @param[in] autoasenable   Automotive profile enable status (0, 1) \n
 *                           Automotive profile enable status
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_autoasenable(uint32 unit, RtkPort port, uint8 autoasenable)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = (uint8)autoasenable;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM automotive profile configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_AUTOASENABLE + (uint32)dv1_g_userPort2sysPort[port], cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get per domain signaling enable.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] enable       Enable status (0, 1) \n
 *                          Pointer to enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_get_signalingen(uint32 unit, uint32 domainid, uint8 *enable)
{
    RtkApiRet retval  = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret  = RT_ERR_OK;
    uint8     cfg     = 0u;
    uint32    address = 0u;
    uint16    data1   = 0u;
    uint16    mask    = 0u;

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(0u == domainid)
        {
            /* Get SRAM signling domain0 enable configuration */
            apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_SIGENABLE, &cfg);
        }
        else
        {
            address = DV1_SIGENABLE_D1D2;
            /* Get SRAM signling domain1 and domain2 enable configuration */
            apiret = dv1_drv_indirect_sram_2bytes_get(unit, address, &data1);
            if(RT_ERR_OK != apiret)
            {
                retval = -RT_ERR_PTP_CALL_API_FAILED;
                continue;
            }
            /* 1st byte low 4 bit is D1, high 4 bit is D2 */
            mask = (domainid == 1u) ? 0x000Fu : 0x00F0u;
            cfg  = (uint8)((data1 & mask) >> (4u * (domainid - 1u)));
        }
        *enable = cfg;
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set per domain signaling enable. ES2 patch+ROM is not supported.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] enable        Enable status (0, 1) \n
 *                          To set enable status, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_set_signalingen(uint32 unit, uint32 domainid, uint8 enable)
{
    RtkApiRet retval  = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret  = RT_ERR_OK;
    uint8     cfg     = ((enable == 1u) ? 1u : 0u);
    uint32    address = 0u;
    uint16    data1   = 0u;
    uint16    mask    = 0u;

    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(0u == domainid)
        {
            /* Set SRAM signling domain0 enable configuration */
            apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_SIGENABLE, cfg);
        }
        else
        {
            address = DV1_SIGENABLE_D1D2;
            /* Get SRAM signling domain1 and domain2 enable configuration */
            apiret = dv1_drv_indirect_sram_2bytes_get(unit, address, (uint16 *)&data1);
            if(RT_ERR_OK != apiret)
            {
                continue;
            }
            /* 1st byte low 4 bit is D1, high 4 bit is D2 */
            mask  = (domainid == 1u) ? 0xFFF0u : 0xFF0Fu;
            data1 = (data1 & mask) | ((uint16)cfg << ((uint16)4u * (uint16)((uint16)domainid - 1u)));
            /* Set SRAM signling domain1 and domain2 enable configuration */
            apiret = dv1_drv_indirect_sram_2bytes_set(unit, address, data1);
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
 * @brief Get time sync slave port id
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] slaveportid  Slave port index (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          Pointer to the slave port index
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_slaveportid(uint32 unit, uint32 domainid, uint8 *slaveportid)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == slaveportid)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM slave port index configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_SLAVEPORTID + (uint32)domainid, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *slaveportid = dv1_g_sysPort2userPort[cfg];
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set the slave port we will sync time with
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] slaveportid   The slave index (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          To set the slave port index
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_slaveportid(uint32 unit, uint32 domainid, uint8 slaveportid)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = dv1_g_userPort2sysPort[slaveportid];
    int32     apiret = RT_ERR_OK;
    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(slaveportid) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM slave port index configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_SLAVEPORTID + (uint32)domainid, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get time sync slave portmask
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[out] slaveportmask Slave port mask (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                           Pointer to all time domain slave port bitmask.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_slaveportmask(uint32 unit, uint16 *slaveportmask)
{
    RtkApiRet   retval  = RT_ERR_OK;
    uint16      cfg     = 0u;
    int32       apiret  = RT_ERR_OK;
    dv1_RtkPmap srcPmap = 0u;
    dv1_RtkPmap dstPmap = 0u;
    do
    {
        if(NULL == slaveportmask)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM slave port mask configuration */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_SLAVEPORTMASK, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        srcPmap = cfg;
        apiret  = dv1_drv_pmap_convert(srcPmap, &dstPmap, DV1_EM_SYS_TO_USER);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *slaveportmask = (uint16)dstPmap;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set the slave portmask we will sync time with \n
 * Does not turn on more than 3 bits to 1.
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  slvMask       Slave port mask (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                           All domain slave port bitmask.
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_slaveportmask(uint32 unit, uint16 slvMask)
{
    RtkApiRet   retval    = RT_ERR_OK;
    int32       apiret    = RT_ERR_OK;
    uint8       turnoncnt = 0u;
    uint16      portid    = 0u;
    dv1_RtkPmap srcPmap   = slvMask;
    dv1_RtkPmap dstPmap   = 0u;
    do
    {
        for(portid = 0u; portid < PN_PORT15; ++portid)
        {
            if(0u != (((uint16)1u << portid) & slvMask))
            {
                turnoncnt++;
            }
        }
        if(turnoncnt > 3u)
        {
            /*RTL906x_7x only has 3 clock for sync time. The mask is always  more than 3 bits turned to 1*/
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(0u != (0x8000u & slvMask))
        {
            /*internal cpu port (port 15) is not slave port*/
            retval = -RT_ERR_INPUT;
            continue;
        }
        apiret = dv1_drv_pmap_convert(srcPmap, &dstPmap, DV1_EM_USER_TO_SYS);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Get SRAM slave port mask configuration */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_SLAVEPORTMASK, (uint16)dstPmap);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get priority1 value of sync/follow-up, default = 246
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] priority1    Priority1 value (0 to 255) \n
 *                          Pointer to priority1 value in PTP packet, range is 0~255, default is 246
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_priority1(uint32 unit, uint32 domainid, uint8 *priority1)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == priority1)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM priority1 configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_PRIORITY1 + (uint32)domainid, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *priority1 = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set priority1 value of sync/follow-up, default = 246
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] priority1     Priority1 value (0 to 255) \n
 *                          Priority1 value in PTP packet, range is 0~255, default is 246
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_priority1(uint32 unit, uint32 domainid, uint8 priority1)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM priority1 configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_PRIORITY1 + (uint32)domainid, priority1);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get path delay threshold.
 * @param[in]  unit          Unit \n
 *                           Stacking unit
 * @param[in]  delaythresh   Pdelay threshold (N/A) \n
 *                           Pointer to pdelay threshold to judge the pdelay calculating result is valid or not.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_delaythresh(uint32 unit, PtpTs_t *delaythresh)
{
    RtkApiRet retval  = RT_ERR_OK;
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == delaythresh)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM valid link delay threshold configuration up16sec field. */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_DELAYTHRESH, &cfg15_0);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        delaythresh->up16Sec = cfg15_0;
        /* Get SRAM valid link delay threshold configuration low32Sec field. */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_DELAYTHRESH + 4u, &cfg15_0);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_DELAYTHRESH + 4u + 2u, &cfg31_16);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        delaythresh->low32Sec = ((uint32)cfg15_0 | ((uint32)cfg31_16 << 16u));
        apiret                = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_DELAYTHRESH + 8u, &cfg15_0);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Get SRAM valid link delay threshold configuration nanosec field. */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_DELAYTHRESH + 8u + 2u, &cfg31_16);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        delaythresh->nanoSec = ((uint32)cfg15_0 | ((uint32)cfg31_16 << 16u));

    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set path delay threshold
 * @param[in]   unit         Unit \n
 *                           Stacking unit
 * @param[in]  delaythresh   Pdelay threshold (N/A) \n
 *                           To judge the pdelay calculating result is valid or not.
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_delaythresh(uint32 unit, PtpTs_t const *const delaythresh)
{
    RtkApiRet retval  = RT_ERR_OK;
    uint16    cfg15_0 = 0u, cfg31_16 = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == delaythresh)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Set SRAM valid link delay threshold configuration up16Sec */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_DELAYTHRESH, delaythresh->up16Sec);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        cfg15_0  = (uint16)delaythresh->low32Sec;
        cfg31_16 = (uint16)(delaythresh->low32Sec >> 16u);
        /* Set SRAM valid link delay threshold configuration low32Sec */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_DELAYTHRESH + 4u, cfg15_0);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_DELAYTHRESH + 4u + 2u, cfg31_16);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        cfg15_0  = (uint16)delaythresh->nanoSec;
        cfg31_16 = (uint16)(delaythresh->nanoSec >> 16u);
        /* Set SRAM valid link delay threshold configuration nanoSec */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_DELAYTHRESH + 8u, cfg15_0);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_DELAYTHRESH + 8u + 2u, cfg31_16);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get initial interval of pdelay request
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] pdelayintval Pdelay request message interval (-5 to 3, -128, 127) \n
 *                          Pointer to pdelay request message interval range is -5 to 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_initlogpdelayintval(uint32 unit, RtkPort port, int8 *pdelayintval)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == pdelayintval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM initial log interval of pdelay request configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_INITLOGPDELAYINTVAL + (uint32)dv1_g_userPort2sysPort[port], &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pdelayintval = (int8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set initial interval of pdelay request
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  pdelayintval Pdelay request message interval (-5 to 3, -128, 127) \n
 *                          Pdelay request message interval
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PTP_INVALID_LOGINTVAL There was wrong syncintval parameter which is not in the valid range, return error.
 */
RtkApiRet dv1_ptp_set_initlogpdelayintval(uint32 unit, RtkPort port, int8 pdelayintval)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = (uint8)pdelayintval;
    int32     apiret = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE((retval), (dv1_drv_ptp_invalid_logintval(pdelayintval)));
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if((int32)pdelayintval < -5)
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM initial log interval of pdelay request configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_INITLOGPDELAYINTVAL + (uint32)dv1_g_userPort2sysPort[port], cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get operated sync interval value
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] syncintval   Point to sync message interval (-5 to 3, -128, 127) \n
 *                          Operated log sync interval, range is -5 to 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_operlogsyncintval(uint32 unit, uint32 domainid, int8 *syncintval)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == syncintval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM operated log interval of sync message. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_OPERLOGSYNCINTVAL + domainid, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *syncintval = (int8)cfg;
        retval      = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set operate sync interval value
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] syncintval    Sync message interval (-5 to 3, -128, 127) \n
 *                          To set operated log sync interval, range is -5 to 3, -128, 127
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PTP_INVALID_LOGINTVAL There was wrong syncintval parameter which is not in the valid range, return error.
 */
RtkApiRet dv1_ptp_set_operlogsyncintval(uint32 unit, uint32 domainid, int8 syncintval)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = (uint8)syncintval;
    int32     apiret = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE((retval), (dv1_drv_ptp_invalid_logintval(syncintval)));
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM operated log interval of sync message. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_OPERLOGSYNCINTVAL + domainid, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get operate pdelay interval of specific port
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] pdelayintval Pointer to pdelay request message interval (-5 to 3, -128, 127) \n
 *                          Operated log pdelay request message interval, range is -5 to 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_operlogpdelayintval(uint32 unit, RtkPort port, int8 *pdelayintval)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == pdelayintval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM operated log interval of pdelay. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_OPERLOGPDELAYINTVAL + (uint32)dv1_g_userPort2sysPort[port], &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *pdelayintval = (int8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set operate pdelay interval of specific port
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  pdelayintval Pdelay request message interval (-5 to 3, -128, 127) \n
 *                          To set operated log pdelay request message interval, range is -5 to 3, -128, 127
 * @retval  RT_ERR_OK                    The API was executed correctly.
 * @retval -RT_ERR_PORT_ID               There was wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED   There was call API internal error, return error.
 * @retval -RT_ERR_PTP_INVALID_LOGINTVAL There was wrong syncintval parameter which is not in the valid range, return error.
 */
RtkApiRet dv1_ptp_set_operlogpdelayintval(uint32 unit, RtkPort port, int8 pdelayintval)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = (uint8)pdelayintval;
    int32     apiret = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE((retval), (dv1_drv_ptp_invalid_logintval(pdelayintval)));
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM operated log interval of pdelay. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_OPERLOGPDELAYINTVAL + (uint32)dv1_g_userPort2sysPort[port], cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get virtual channel enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] exchangeInfo  Exchange Info flag (0, 1) \n
 *                          Pointer indicate to exchange Info flag, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_exchangeInfo(uint32 unit, uint8 *exchangeInfo)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == exchangeInfo)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM exchange information flag. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_EXCHANGEINFO, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *exchangeInfo = cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set virtual channel enable status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] exchangeInfo  Exchange Info flag (0, 1) \n
 *                          To set exchange Info flag, range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_exchangeInfo(uint32 unit, uint8 exchangeInfo)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        /* Set SRAM exchange information flag. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_EXCHANGEINFO, exchangeInfo);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get bootup time for grand master sync/follow.
 * @param[in] unit                      Unit \n
 *                                      Stacking unit
 * @param[out] firstbootuptimetarget    First boot up time target (0 to 0xFFFF) \n
 *                                      Pointer to the sync timer target time in bootup condition.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_firstbootuptimetarget(uint32 unit, uint16 *timetarget)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    uint16    cfg;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == timetarget)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM First boot up time target configuration. */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_FISTBOOTUPTIMTARGET, &cfg);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        *timetarget = cfg;
        retval      = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set bootup time for grand master sync/follow, default is 3 sec
 * @param[in] unit                    Unit \n
 *                                    Stacking unit
 * @param[in] firstbootuptimetarget   First boot up time target (0 to 0xFFFF) \n
 *                                    The sync timer target time in bootup condition.
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_firstbootuptimetarget(uint32 unit, uint16 timetarget)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        /* Set SRAM First boot up time target configuration. */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_FISTBOOTUPTIMTARGET, timetarget);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get current sync message interval value in ms
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                           The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid       Domain index (0, 1, 2) \n
 *                           The specific domain, range is 0, 1, 2
 * @param[out] curntsyncintval   Current sync interval (ref to dv1_ptp_log_to_intval) \n
 *                               Pointer to current sync interval. Range refers to dv1_ptp_log_to_intval
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_curntsyncintval(uint32 unit, RtkPort port, uint32 domainid, uint32 *curntsyncintval)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    uint32    cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == curntsyncintval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM current sync interval configuration */
        apiret = dv1_drv_indirect_sram_get(unit, DV1_PTPCFG_CURNTSYNCINTVAL + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)12u) + (domainid * (uint32)4u)), &cfg);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        *curntsyncintval = cfg;
        retval           = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set current sync message interval value in ms
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                           The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid       Domain index (0, 1, 2) \n
 *                           The specific domain, range is 0, 1, 2
 * @param[in] curntsyncintval   Current sync interval (ref to dv1_ptp_log_to_intval) \n
 *                              To set current sync interval, range refers to dv1_ptp_log_to_intval
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_curntsyncintval(uint32 unit, RtkPort port, uint32 domainid, uint32 curntsyncintval)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM current sync interval configuration */
        apiret = dv1_drv_indirect_sram_set(unit, DV1_PTPCFG_CURNTSYNCINTVAL + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)12u) + (domainid * (uint32)4u)), curntsyncintval);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Protocol port mode: GM (0x0), P2P (0x3)
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                           The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid       Domain index (0, 1, 2) \n
 *                           The specific domain, range is 0, 1, 2
 * @param[out]  portmode:    Port mode (0, 3) \n
 *                           Pointer to software port mode, GM (0x0), P2P (0x3)
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_get_sw_portmode(uint32 unit, RtkPort port, uint32 domainid, uint8 *portmode)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    uint8     cfg    = 0u;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == portmode)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM SW port mode configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_PORTMODE + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), &cfg);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *portmode = cfg;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Protocol port mode: GM (0x0), P2P (0x3)
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                           The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid       Domain index (0, 1, 2) \n
 *                           The specific domain, range is 0, 1, 2
 * @param[in]  portmode:     Port mode (0, 3) \n
 *                           To set software port mode, GM (0x0), P2P (0x3)
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_sw_portmode(uint32 unit, RtkPort port, uint32 domainid, uint8 portmode)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM SW port mode configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_PORTMODE + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), portmode);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Sync interval
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                           The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid       Domain index (0, 1, 2) \n
 *                           The specific domain, range is 0, 1, 2
 * @param[out]  initLogSyncIntval: Initial log sync interval (-5 to 3, -128, 127) \n
 *                                 The logarithm to the base 2 of the sync interval (in seconds), it is logMessageInterval field of the time-synchronization messages.
 * @retval  RT_ERR_OK                     The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED   There was call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_get_initlogsyncintval(uint32 unit, RtkPort port, uint32 domainid, int8 *initLogSyncIntval)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    uint8     cfg    = 0u;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == initLogSyncIntval)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM initial log interval of sync message configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_INITLOGSYNCINTVAL + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domainid, &cfg);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *initLogSyncIntval = (int8)cfg;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Sync interval
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                           The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid       Domain index (0, 1, 2) \n
 *                           The specific domain, range is 0, 1, 2
 * @param[in] initLogSyncIntval Initial log sync interval (-5 to 3, -128, 127) \n
 *                              The logarithm to the base 2 of the sync interval (in seconds), it is logMessageInterval field of the time-synchronization messages.
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_initlogsyncintval(uint32 unit, RtkPort port, uint32 domainid, int8 initLogSyncIntval)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM initial log interval of sync message configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_INITLOGSYNCINTVAL + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), (uint8)initLogSyncIntval);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get Domain Number of domain. This is per port seeting and should be the same for each port if they join the same domain
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] port           Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                           The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid       Domain index (0, 1, 2) \n
 *                           The specific domain, range is 0, 1, 2
 * @param[out] domainnum     Domain number (0 to 255) \n
 *                           Pointer to the number of time domain, range is 0~255
 * @param[out] None
 * @retval RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED There was call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_domainnum(uint32 unit, RtkPort port, uint32 domainid, uint8 *domainnum)
{
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        if(NULL == domainnum)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM domain index number configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_DOMAINNUM + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), &cfg);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *domainnum = cfg;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Set Domain Number of domain. This is per port seeting and should be the same for each port if they join the same domain
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] domainnum:    Domain number (0 to 255) \n
 *                          To set the domain numver, range is 0~255
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_domainnum(uint32 unit, RtkPort port, uint32 domainid, uint8 domainnum)
{
    int32 apiret = RT_ERR_OK;

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM domain index number configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_DOMAINNUM + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), domainnum);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get which ports join into specified domain
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] domainmap     Domain port map. (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                          Pointer to the joined port bitmask.
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_get_domainmap(uint32 unit, uint32 domainid, uint16 *domainmap)
{
    uint16      cfg15_0 = 0u;
    int32       apiret  = RT_ERR_OK;
    RtkApiRet   ret     = RT_ERR_OK;
    dv1_RtkPmap srcPmap = 0u;
    dv1_RtkPmap dstPmap = 0u;
    do
    {
        if(NULL == domainmap)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM domain port mask configuration. */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_PTPCFG_DOMAINMAP + (domainid * (uint32)2u), &cfg15_0);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        srcPmap = cfg15_0;
        apiret  = dv1_drv_pmap_convert(srcPmap, &dstPmap, DV1_EM_SYS_TO_USER);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *domainmap = (uint16)dstPmap;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Set which ports join into specified domain
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] domainmap     Domain port map (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                          Joined port bitmask.
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_set_domainmap(uint32 unit, uint32 domainid, uint16 domainmap)
{
    RtkApiRet   ret     = RT_ERR_OK;
    int32       apiret  = 0;
    dv1_RtkPmap srcPmap = domainmap;
    dv1_RtkPmap dstPmap = 0u;
    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        apiret = dv1_drv_pmap_convert(srcPmap, &dstPmap, DV1_EM_USER_TO_SYS);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set SRAM domain port mask configuration. */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_DOMAINMAP + (domainid * (uint32)2u), (uint16)dstPmap);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the port remapping index.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] remapping     Remapping port number (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          Source Port id In Sync/follow-up/Pdelay_xxx/singal
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_get_portremapping(uint32 unit, RtkPort port, RtkPort *remapping)
{
    uint8     cfg7_0 = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        if(NULL == remapping)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM port remapping configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_REMAPPINGPORTNUM + (uint32)dv1_g_userPort2sysPort[port], &cfg7_0);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *remapping = cfg7_0;
        /* Increase 1 to revert. */
        *remapping += 1u;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Set the port remapping index.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] remapping     Remapping port number (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          Source Port id in Sync/follow-up/Pdelay_xxx/singal
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_portremapping(uint32 unit, RtkPort port, RtkPort remapping)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = 0;
    RtkPort   transRemapping;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if((remapping == 0u) || (remapping > 0xffu))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Decrease 1 because FW would do remapping+1 */
        transRemapping = remapping;
        transRemapping -= 1u;
        /* Set SRAM port remapping configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_REMAPPINGPORTNUM + (uint32)dv1_g_userPort2sysPort[port], (uint8)transRemapping);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get the port remapping function enable.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[out]  enable      Enable status pointer (0, 1) \n
 *                          Configuration to enanble/disable port remapping function
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_en_portremapping(uint32 unit, uint8 *enable)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint8     cfg7_0 = 0u;
    int32     apiret = 0;

    do
    {
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM port remapping enable configuration. */
        apiret  = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_ENABLE_PORTREMAPPING, &cfg7_0);
        *enable = cfg7_0;
    } while(0u == 1u);

    if(RT_ERR_OK != apiret)
    {
        ret = -RT_ERR_PTP_CALL_API_FAILED;
    }
    return ret;
}

/**
 * @brief Set the port remapping function enable.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in]  enable       Enable status (0, 1) \n
 *                          Configuration to enanble/disable port remapping function
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_en_portremapping(uint32 unit, uint8 enable)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint8     cfg7_0 = ((enable == 1u) ? 1u : 0u);
    int32     apiret = 0;

    do
    {
        /* Set SRAM port remapping enable configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_ENABLE_PORTREMAPPING, cfg7_0);
    } while(0u == 1u);

    if(apiret != RT_ERR_OK)
    {
        ret = -RT_ERR_PTP_CALL_API_FAILED;
    }
    return ret;
}

/**
 * @brief 1-Step transmite (Hardware cfg)
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  oneStepTxSync  Pointer to 1-step transmition sync message operability (0, 1) \n
 *                            Pointer indicate to 1-step transmition sync message operability, 0: Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_onesteptxsync(uint32 unit, RtkPort port, uint8 *oneStepTxSync)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = -RT_ERR_OK;
    uint32    data1  = 0u;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == oneStepTxSync)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get 1-step hardware offload register configuration. */
        apiret = dv1_reg_field_read(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_SYNC_OFFLOAD_ENABLE_LSP, DV1_PORT0_802DOT1AS_CONFIG_SYNC_OFFLOAD_ENABLE_LEN, &data1);
        if(apiret != RT_ERR_OK)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *oneStepTxSync = (uint8)(data1 & 0x1u);
    } while(0u == 1u);

    return ret;
}

/**
 * @brief 1-Step transmite  (Hardware cfg)
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  oneStepTxSync  1-step transmition sync message operability (0, 1) \n
 *                            To set 1-step transmition sync message operability, 0: Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_onesteptxsync(uint32 unit, RtkPort port, uint8 oneStepTxSync)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = -RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set 1-step hardware offload register configuration. */
        apiret = dv1_reg_field_write(unit, DV1_PORT0_802DOT1AS_CONFIG + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_802DOT1AS_CONFIG_SYNC_OFFLOAD_ENABLE_LSP, DV1_PORT0_802DOT1AS_CONFIG_SYNC_OFFLOAD_ENABLE_LEN, (uint32)oneStepTxSync);
        if(apiret != RT_ERR_OK)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Reinit protocol configurations. \n
 *  This API is provided to TC11 test when run-time changing. \n
 *  Before call this API, gPTP protocol must be off.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_reinitprotocol(uint32 unit)
{
    uint32    regval = (((uint32)1u << 15) | ((uint32)DV1_PTP_CFG_REINIT << 16));
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        /*Sync ever recevied status reg. Clear them on domain0*/
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_GOTSYNC_D0, 0u);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /*Sync ever recevied status reg. Clear them on domain1*/
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_GOTSYNC_D0 + 1u, 0u);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /*Sync ever recevied status reg. Clear them on domain2*/
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_GOTSYNC_D0 + 2u, 0u);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set PTP config flag to prevent re-entry. */
        apiret = dv1_reg_write(unit, DV1_PTP_CFG_001, regval);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Reinit done, clear the PTP config flag. */
        apiret = dv1_drv_ptp_check_reinitprotocol_done(unit, regval);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set protocolPortmask
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] portmask      Portmask array (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                          have 3 index. \n
 *                          portmask[0]: Port enabled mask \n
 *                          portmask[1]: Ascapable mask \n
 *                          portmask[2]: Calculate mean link delay mask
 * @param[out] None
 * @retval RT_ERR_OK                    The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_protocolportmask_set(uint32 unit, uint32 domainid, uint16 const *const portmask)
{
    RtkApiRet   retval  = -RT_ERR_PTP_CALL_API_FAILED;
    int32       apiret  = RT_ERR_OK;
    dv1_RtkPmap srcPmap = 0u;
    dv1_RtkPmap dstPmap = 0u;
    do
    {
        if(NULL == portmask)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        srcPmap = portmask[0];
        apiret  = dv1_drv_pmap_convert(srcPmap, &dstPmap, DV1_EM_USER_TO_SYS);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set SRAM port enable mask configuration. */
        CHK_FUN_CONTINUE((retval), (dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_PORTENABLEMASK, (uint16)dstPmap)));
        srcPmap = portmask[1];
        apiret  = dv1_drv_pmap_convert(srcPmap, &dstPmap, DV1_EM_USER_TO_SYS);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set SRAM ascapable mask configuration. */
        CHK_FUN_CONTINUE((retval), (dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_ASCAPABLEMASK + (domainid * 2u), (uint16)dstPmap)));
        srcPmap = portmask[2];
        apiret  = dv1_drv_pmap_convert(srcPmap, &dstPmap, DV1_EM_USER_TO_SYS);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set SRAM calculating link delay mask configuration. */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_PTPCFG_CALDELAYENMASK, (uint16)dstPmap);
        if(RT_ERR_OK != apiret)
        {
            continue;
        }
        retval = RT_ERR_OK;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Do not to do recovery if in first boot status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set skip boot up recovery enable status, 0: Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_skipfirstbootrecovery(uint32 unit, uint8 enable)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = 0;
    do
    {
        /* Set SRAM skip recovery if in first boot status configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_NORECOVERYFIRSTBOOT, enable);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief If DUT never get sync from slave port, no recovery
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] enable        Enable status (0, 1) \n
 *                          To set silence if never received sync enable status, 0: Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_set_silenceifneversynced(uint32 unit, uint32 domainid, uint8 enable)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = 0;

    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM no recovery if never get sync from slave port configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_SILENCEIFNEVERSYNCED + domainid, enable);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Over wirte with my clockID
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in]  enable:      Enable status (0, 1) \n
 *                          To set overwrite with my clockID enable status, 0: Disable, 1:Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_overwrite_ptpid_set(uint32 unit, uint8 enable)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = 0;

    do
    {
        /* Set SRAM use my clock ID configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_USEMYCLOCKID, enable);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief  Send out Pdelay_req from master port.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] enable:       Enable status (0, 1) \n
 *                          To set pdelay request from master port enable status, 0: Disable, 1:Enable
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_send_pdelay_from_masterport(uint32 unit, uint8 enable)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = 0;
    do
    {
        /* Set SRAM sending pdelay from master configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_PDELAYTOMASTERPORT, enable);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set 1-Step transmite (Protocol cfg)
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] oneStepTxSync 1-Step transmition sync message operability (0, 1) \n
 *                          To set 1-Step transmition sync message operability, 0: Disable, 1:Enable
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_onesteptxen_sw_set(uint32 unit, RtkPort port, uint32 domainid, uint8 oneStepTxSync)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM 1step transmission configuration. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_ONESTEPTXSYNC + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), oneStepTxSync);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief rxSyncCount. A counter that increments every time synchronization is received.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the recipt sync message counter
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_rx_sync_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(2u == domainid)
        {
            /* Get RX sync counter on domain2 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_SYNC_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(1u == domainid)
        {
            /* Get RX sync counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_SYNC_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get RX sync counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_SYNC_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief rxFollowUpCount. A counter that increments every time Follow_up message is received.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the receipt follow up counter
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_rx_follow_up_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get RX follow up counter on domain2 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_FOLLOW_UP_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(1u == domainid)
        {
            /* Get RX follow up counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_FOLLOW_UP_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get RX follow up counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_FOLLOW_UP_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief rxPdelayRequestCount. A counter that increments every time a Pdelay_Req message is received.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the receipt pdelay request counter
 * @retval RT_ERR_OK                             The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_rx_pdelay_request_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get RX pdelay request counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_REQUEST_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get RX pdelay request counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_REQUEST_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get RX pdelay request counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_REQUEST_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief rxPdelayResponseCount. A counter that increments every time a Pdelay_Resp message is received.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the receipt pdelay response counter
 * @retval  RT_ERR_OK           The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_rx_pdelay_response_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get RX pdelay response counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_RESPONSE_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get RX pdelay response counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_RESPONSE_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get RX pdelay response counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_RESPONSE_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief rxPdelayResponseFollowUpCount. A counter that increments every time a Pdelay_Resp_Follow_Up message is received.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the receipt pdelay response follow up counter
 * @retval  RT_ERR_OK           The API was executed correctly..
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_rx_pdelay_resp_follow_up_mib_get(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get RX pdelay response follow up counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get RX pdelay response follow up counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get RX pdelay response follow up counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief rxPTPPacketDiscardCount. A counter that increments every time a PTP message is discarded.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the ingress discard counter
 * @retval  RT_ERR_OK           The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_rx_ptp_packet_discard_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(2u == domainid)
        {
            /* Get RX packet discard counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_RX_PTP_PACKET_DISCARD_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get RX packet discard counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PTP_PACKET_DISCARD_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get RX packet discard counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_RX_PTP_PACKET_DISCARD_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief syncReceiptTimeoutCount. A counter that increments every time sync receipt timeout occurs.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the sync receipt timeout counter
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_sync_receipt_timeout_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get sync receipt timeout counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_SYNC_RECEIPT_TIMEOUT_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get sync receipt timeout counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_SYNC_RECEIPT_TIMEOUT_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get sync receipt timeout counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_SYNC_RECEIPT_TIMEOUT_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief pdelayAllowedLostResponsesExceededCount. \n
 *  A counter that increments every time the value of the variable lostResponses (see, 11.2.15.1.10) exceeds the value of the variable allowedLostResponses (see 11.2.12.4).
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the pdelay allowed lost response counter
 * @retval  RT_ERR_OK           The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_pdelay_allowed_lost_response_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(2u == domainid)
        {
            /* Get pdelay allowed lost response counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_PDELAY_ALLOWED_LOST_RESPONSE_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get pdelay allowed lost response counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_PDELAY_ALLOWED_LOST_RESPONSE_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get pdelay allowed lost response counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_PDELAY_ALLOWED_LOST_RESPONSE_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief txSyncCount. A counter that increments every time synchronization information is transmitted.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the transmission sync counter
 * @retval  RT_ERR_OK           The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_tx_sync_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get tx sync counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_TX_SYNC_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get tx sync counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_SYNC_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get tx sync counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_SYNC_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief txFollowUpCount. A counter that increments every time a Follow_Up message is transmitted.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the transmission follow up counter
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_tx_follow_up_mib_get_count(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get tx follow up counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_TX_FOLLOW_UP_COUNT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get tx follow up counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_FOLLOW_UP_COUNT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get tx follow up counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_FOLLOW_UP_COUNT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief txPdelayRequestCount. A counter that increments every time a Pdelay_Req message is transmitted.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the transmission pdelay request counter returned
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_tx_pdelay_request_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get tx pdelay request counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_REQUEST_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get tx pdelay request counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_REQUEST_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get tx pdelay request counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_REQUEST_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief txPdelayResponseCount. A counter that increments every time a Pdelay_Resp message is transmitted.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the transmission pdelay response counter returned
 * @retval  RT_ERR_OK           The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_tx_pdelay_response_mib_get_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get tx pdelay response counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_RESPONSE_CONT_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get tx pdelay response counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_RESPONSE_CONT_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get tx pdelay response counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_RESPONSE_CONT_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief txPdelayResponseFollowUpCount. A counter that increments every time a Pdelay_Resp_Follow_Up message is transmitted.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] mib          Mib counter (0 to 0xFFFFFFFF) \n
 *                          32-bit of the transmission pdelay response follow up counter returned
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_tx_pdelay_resp_follow_up_mib_get(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib)
{
    RtkApiRet retval = -RT_ERR_PTP_CALL_API_FAILED;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(NULL == mib)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        if(2u == domainid)
        {
            /* Get tx pdelay response follow up counter on domain2 on hardware register */
            CHK_FUN_CONTINUE((retval), (dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib)));
        }
        else if(1u == domainid)
        {
            /* Get tx pdelay response follow up counter on domain1 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
        }
        else if(0u == domainid)
        {
            /* Get tx pdelay response follow up counter on domain0 on hardware register */
            apiret = dv1_reg_read(unit, DV1_PORT0_TX_PDELAY_RESPONSE_FOLLOW_UP_DOMAIN0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), mib);
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
 * @brief Set 1-Step Receive (Protocol cfg)
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] oneStepRxSync 1-Step received sync operability. (0, 1) \n
 *                          To set 1-Step received sync operability, 0: Disable, 1:Enable
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_onesteprxen_sw_set(uint32 unit, RtkPort port, uint32 domainid, uint8 oneStepRxSync)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT((port)) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM 1step rx enable configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_ONESTEPRXSYNC + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), oneStepRxSync);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Initialization One Step Transmit operability
 * @param[in] unit                 Unit \n
 *                                 Stacking unit
 * @param[in] port                 Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                                 The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid             Domain index (0, 1, 2) \n
 *                                 The specific time domain, range is 0, 1, 2
 * @param[in] initOneStepTXOperEn  Initial one stap transmission operability enable status (0, 1) \n
 *                                 To set initial one step transmission operability enable status
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dv1_ptp_set_initonesteptxoper(uint32 unit, RtkPort port, uint32 domainid, uint8 initOneStepTXOperEn)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM initial 1step tx operability configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_INITONESTEPTXOPER + (((uint32)dv1_g_userPort2sysPort[port] * (uint32)3u) + domainid), (uint8)initOneStepTXOperEn);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get Initialization One Step Transmit Operation
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] initOneStepTXOperEn  Initial one stap transmission operability enable status (0, 1) \n
 *                                  Pointer to set initial one step transmission operability enable status
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED
 */
RtkApiRet dv1_ptp_get_initonesteptxoper(uint32 unit, RtkPort port, uint32 domainid, uint8 *initOneStepTXOperEn)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    uint8     cfg    = 0u;
    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        if(NULL == initOneStepTXOperEn)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM initial 1step tx operability configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_INITONESTEPTXOPER + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domainid, &cfg);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *initOneStepTXOperEn = cfg;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get PTP flash access flag status
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[out] flashAccess  Flash access flag status (0, 1) \n
 *                          Pointer indicate to flash access flag status
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_flash_runtime_access(uint32 unit, uint8 *flashAccess)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == flashAccess)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM access flash configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_ACCESSFLASH, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *flashAccess = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set PTP flash access flag configuration.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] flashAccess   Flash access flag status (0, 1) \n
 *                          To set flash access flag status
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_flash_runtime_access(uint32 unit, uint8 flashAccess)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = (uint8)flashAccess;
    int32     apiret = RT_ERR_OK;
    do
    {
        /* Set SRAM access flash configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_ACCESSFLASH, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set PTP pdelay write flash threshold.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] threshold     Write flash threshold (0 to 0xFFFFFFFF) \n
 *                          To set the diff threshold of pdelay result to write flash
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_pdelay_wrflash_thres(uint32 unit, uint32 threshold)
{
    RtkApiRet retval = RT_ERR_OK;
    uint32    cfg    = threshold;
    int32     apiret = RT_ERR_OK;
    do
    {
        /* Set SRAM thrshould configuration of pdelay write to flash */
        apiret = dv1_drv_indirect_sram_set(unit, DV1_PTPCFG_PDELAYTHRESHOLD, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}
/**
 * @brief Get PTP pdelay write flash threshold.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] threshold     Write flash threshold (0 to 0xFFFFFFFF) \n
 *                          Pointer to set the diff threshold of pdelay result to write flash
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_pdelay_wrflash_thres(uint32 unit, uint32 *threshold)
{
    RtkApiRet retval = RT_ERR_OK;
    uint32    cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == threshold)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM thrshould configuration of pdelay write to flash */
        apiret = dv1_drv_indirect_sram_get(unit, DV1_PTPCFG_PDELAYTHRESHOLD, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *threshold = cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get Sync Lock Configuration
 * @param[in] unit             Unit \n
 *                             tacking unit
 * @param[out] synclockStatus  Sync locked status (0, 1, 2) \n
 *                             Pointer the to sync locked status, 0: Disable, 1: AS2020 Sync Lock 2: AS2011 Force Sync Lock
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_synclock_config(uint32 unit, uint8 *synclockStatus)
{
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        if(NULL == synclockStatus)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM sync lock configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_SYNCLOCKSWITCH, &cfg);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *synclockStatus = cfg;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Set Sync Lock Configuration
 * @param[in] unit           Unit \n
 *                           Stacking unit
 * @param[in] synclockStatus Sync locked status (0, 1, 2) \n
 *                           To set sync locked status, 0: Disable, 1: AS2020 Sync Lock 2: AS2011 Force Sync Lock
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_synclock_config(uint32 unit, uint8 synclockStatus)
{
    uint8     cfg    = synclockStatus;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        /* Set SRAM sync lock configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_SYNCLOCKSWITCH, cfg);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get Sync Slow Down Configuration
 * @param[in] unit                     Unit \n
 *                                     Stacking unit
 * @param[out] syncSlowdownStatus      Sync slowdown status (0, 1) \n
 *                                     Pointer to sync slowdown status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_get_syncslowdown_config(uint32 unit, uint8 *syncSlowdownStatus)
{
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        if(NULL == syncSlowdownStatus)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM sync slowdown configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_SYNCSLOWDOWNSWITCH, &cfg);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *syncSlowdownStatus = cfg;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Set Sync Slow Down Configuration
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] syncSlowdownStatus Sync slowdown status (0, 1) \n
 *                               To set sync slowdown status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_syncslowdown_config(uint32 unit, uint8 syncSlowdownStatus)
{
    uint8     cfg    = syncSlowdownStatus;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        /* Set SRAM sync slowdown configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_SYNCSLOWDOWNSWITCH, cfg);

        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Set the pdelay response allowed faults
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  allowedfaults The pdelay response allowed faults (0 to 0xFF) \n
 *                           To set pdelay response allowed faults.
 * @retval RT_ERR_OK                    The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_allowedfaults(uint32 unit, RtkPort port, uint8 allowedfaults)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = allowedfaults;
    uint32    offset = DV1_PTPCFG_ALLOWEDFAULTS + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM pdelay response allowed faults configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the pdelay response allowed faults
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] allowedfaults The pdelay response allowed faults (0 to 0xFF) \n
 *                           Pointer to the pdelay response allowed faults.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_allowedfaults(uint32 unit, RtkPort port, uint8 *allowedfaults)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_ALLOWEDFAULTS + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == allowedfaults)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM pdelay response allowed faults configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *allowedfaults = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set the delay measurement mechnisam
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] mechnisam    The delay measurement mechnisam (2, 3) \n
 *                         To set delay measurement mechnisam 2: PTP_P2P_MECH, 3: PTP_COMMONP2P_MECH.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_delaymeasure_mech(uint32 unit, uint32 mechnisam)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    cfg    = 0u;
    uint32    offset = DV1_PTPCFG_DELAYMEASUREMECH;

    do
    {
        cfg = mechnisam;
        /* Set SRAM delay measure mechnisam configuration */
        apiret = dv1_drv_indirect_sram_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the delay measurement mechnisam
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] mechnisam   The delay measurement mechnisam (2, 3) \n
 *                         The pointer to the delay measurement mechnisam 2: PTP_P2P_MECH, 3: PTP_COMMONP2P_MECH.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_delaymeasure_mech(uint32 unit, uint32 *mechnisam)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint32    cfg;
    uint32    cfgoffset = DV1_PTPCFG_DELAYMEASUREMECH;

    do
    {
        if(NULL == mechnisam)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get SRAM delay measure mechnisam configuration */
        apiret = dv1_drv_indirect_sram_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *mechnisam = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set the delay asymmetry value.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] delayAsymmetry     The delay asymmetry value (N/A) \n
 *                               To set The delay asymmetry value.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_delay_asymmetry(uint32 unit, RtkPort port, CFiled_t delayAsymmetry)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint16    u16data;
    uint32    offset = DV1_PTPCFG_DELAYASYMMETRY + ((uint32)dv1_g_userPort2sysPort[port] * DV1_CFiled_t_LENGTH_IN_FW);

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        u16data = (uint16)delayAsymmetry.up16BitNs;
        /* Set SRAM delay asymmetry configuration up16BitNs */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, offset, u16data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        /* Skip cfgoffset + 2u because padding */
        u16data = (uint16)(delayAsymmetry.mid32BitNs);
        /* Set SRAM delay asymmetry configuration mid32BitNs */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, offset + 4u, u16data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set SRAM delay asymmetry configuration mid32BitNs */
        u16data = (uint16)(delayAsymmetry.mid32BitNs >> 16);
        apiret  = dv1_drv_indirect_sram_2bytes_set(unit, offset + 6u, u16data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Set SRAM delay asymmetry configuration sub16BitNs */
        u16data = (uint16)(delayAsymmetry.sub16bitNs);
        apiret  = dv1_drv_indirect_sram_2bytes_set(unit, offset + 8u, u16data);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get the delay asymmetry value.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] delayAsymmetry    The delay asymmetry value (N/A) \n
 *                               The pointer to the delay asymmetry value
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_delay_asymmetry(uint32 unit, RtkPort port, CFiled_t *delayAsymmetry)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint16    u16data[4];
    uint32    cfgoffset = DV1_PTPCFG_DELAYASYMMETRY + ((uint32)dv1_g_userPort2sysPort[port] * DV1_CFiled_t_LENGTH_IN_FW);

    do
    {
        if(NULL == delayAsymmetry)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM delay asymmetry configuration up16BitNs */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, cfgoffset, &u16data[0]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Skip cfgoffset + 2u because padding */
        /* Get SRAM delay asymmetry configuration mid32BitNs */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, cfgoffset + 4u, &u16data[1]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Get SRAM delay asymmetry configuration mid32BitNs */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, cfgoffset + 6u, &u16data[2]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Get SRAM delay asymmetry configuration sub16BitNs */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, cfgoffset + 8u, &u16data[3]);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        delayAsymmetry->up16BitNs  = u16data[0];
        delayAsymmetry->mid32BitNs = ((uint32)u16data[2]) << 16;
        delayAsymmetry->mid32BitNs |= (uint32)u16data[1];
        delayAsymmetry->sub16bitNs = u16data[3];
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set useMgtSettableLogSyncinterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set useMgtSettableLogSyncinterval enable status, range is 0, 1.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_use_mgtsettable_logsyncintval(uint32 unit, uint32 domain, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_USEMGTSETTABLELOGSYNCINTERVAL + domain;

    do
    {
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM USEMGTSETTABLELOGSYNCINTERVAL configuration to enable mgtsettable logsyncintval. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get useMgtSettableLogSyncinterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] enable       Enable status (0, 1) \n
 *                          The pointer to useMgtSettableLogSyncinterval enable status, range is 0, 1.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 */
RtkApiRet dv1_ptp_get_use_mgtsettable_logsyncintval(uint32 unit, uint32 domain, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_USEMGTSETTABLELOGSYNCINTERVAL + domain;

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM USEMGTSETTABLELOGSYNCINTERVAL configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set mgtSettableLogSyncinterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in]  logIntval    Log interval (-5 to 3, -128, 127) \n
 *                          Log interval of mgtSettableLogSyncinterval, range is -5 ~ 3, -128, 127.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PTP_INVALID_LOGINTVAL There was wrong syncintval parameter which is not in the valid range, return error.
 */
RtkApiRet dv1_ptp_set_logsyncintval_mgtsettable(uint32 unit, uint32 domain, int8 logIntval)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = (uint8)logIntval;
    uint32    offset = DV1_PTPCFG_MGTSETTABLELOGSYNCINTERVAL + domain;

    do
    {
        CHK_FUN_CONTINUE((retval), (dv1_drv_ptp_invalid_logintval(logIntval)));
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM MGTSETTABLELOGSYNCINTERVAL configuration to change logsyncintval. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get mgtSettableLogSyncinterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] logIntval    Log interval (-5 to 3, -128, 127) \n
 *                          The pointer to mgtSettableLogSyncinterval, range is -5 ~ 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_logsyncintval_mgtsettable(uint32 unit, uint32 domain, int8 *logIntval)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_MGTSETTABLELOGSYNCINTERVAL + domain;

    do
    {
        if(NULL == logIntval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM MGTSETTABLE LOGSYNCINTERVAL configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
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
 * @brief Set useMgtSettableLogPdelayReqInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       Enable status (0, 1) \n
 *                          To set useMgtSettableLogPdelayReqInterval enable status, range is 0, 1.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_usemgtsettable_logpdelayreqintval(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_USEMGTSETTABLELOGPDELAYREQINTERVAL + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM USEMGTSETTABLELOGPDELAYREQINTERVAL configuration to enable mgtsettable logpdelayreqintval. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get useMgtSettableLogPdelayReqInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       Enable status (0, 1) \n
 *                          The pointer to useMgtSettableLogPdelayReqInterval enable status, range is 0, 1.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_usemgtsettable_logpdelayreqintval(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_USEMGTSETTABLELOGPDELAYREQINTERVAL + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM USEMGTSETTABLELOGPDELAYREQINTERVAL configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set mgtSettableLogPdelayReqInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  intval       Log interval (-5 to 3, -128, 127) \n
 *                          Log interval of mgtSettableLogPdelayReqInterval, range is -5 to 3, -128, 127.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PTP_INVALID_LOGINTVAL There was wrong syncintval parameter which is not in the valid range, return error.
 */
RtkApiRet dv1_ptp_set_logpdelayreqintval_mgtsettable(uint32 unit, RtkPort port, int8 intval)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = (uint8)intval;
    uint32    offset = DV1_PTPCFG_MGTSETTABLELOGPDELAYREQINTERVAL + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        CHK_FUN_CONTINUE((retval), (dv1_drv_ptp_invalid_logintval(intval)));
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM MGTSETTABLELOGPDELAYREQINTERVAL configuration to change mgtsettable logpdelayreqintval. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get mgtSettableLogPdelayReqInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] intval       Log interval (-5 to 3, -128, 127) \n
 *                          The pointer to mgtSettableLogPdelayReqInterval, range is -5 to 3, -128, 127.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_logpdelayreqintval_mgtsettable(uint32 unit, RtkPort port, int8 *intval)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_MGTSETTABLELOGPDELAYREQINTERVAL + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == intval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM MGTSETTABLELOGPDELAYREQINTERVAL configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *intval = (int8)cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set useMgtSettableComputeNeighborRateRatio
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set useMgtSettableComputeNeighborRateRatio enable status, range is 0, 1.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_rateratio_usemgtsettable(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_USEMGTSETTABLECOMPUTENEIGHBORRATERATIO + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM USEMGTSETTABLECOMPUTENEIGHBORRATERATIO configuration to enable mgtsettable rateratio. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get useMgtSettableComputeNeighborRateRatio
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       Enable status (0, 1) \n
 *                          The pointer to useMgtSettableComputeNeighborRateRatio enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_rateratio_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_USEMGTSETTABLECOMPUTENEIGHBORRATERATIO + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM USEMGTSETTABLECOMPUTENEIGHBORRATERATIO configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set mgtSettableComputeNeighborRateRatio
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set mgtSettableComputeNeighborRateRatio enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_rateratio_mgtsettable(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_MGTSETTABLECOMPUTENEIGHBORRATERATIO + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM MGTSETTABLECOMPUTENEIGHBORRATERATIO configuration to change mgtsettable rateratio. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get mgtSettableComputeNeighborRateRatio
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out]  enable      Enable status (0, 1) \n
 *                          The pointer to mgtSettableComputeNeighborRateRatio enable status, range is 0, 1.
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_rateratio_mgtsettable(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_MGTSETTABLECOMPUTENEIGHBORRATERATIO + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM MGTSETTABLECOMPUTENEIGHBORRATERATIO configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set useMgtSettableComputeMeanLinkDelay
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] enable        Enable status (0, 1) \n
 *                          To set useMgtSettableComputeMeanLinkDelay enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_linkdelay_usemgtsettable(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_USEMGTSETTABLECOMPUTEMEANLINKDELAY + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM USEMGTSETTABLECOMPUTEMEANLINKDELAY configuration to enable mgtsettable linkdelay. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get useMgtSettableComputeMeanLinkDelay
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       Enable status (0, 1) \n
 *                          The pointer to useMgtSettableComputeMeanLinkDelay enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_linkdelay_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_USEMGTSETTABLECOMPUTEMEANLINKDELAY + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM USEMGTSETTABLECOMPUTEMEANLINKDELAY configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set mgtSettableComputeMeanLinkDelay
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in]  enable       Enable status (0, 1) \n
 *                          To set mgtSettableComputeMeanLinkDelay enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_linkdelay_mgtsettable(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_MGTSETTABLECOMPUTEMEANLINKDELAY + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM UMGTSETTABLECOMPUTEMEANLINKDELAY configuration to change mgtsettable linkdelay. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get mgtSettableComputeMeanLinkDelay
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       Enable status (0, 1) \n
 *                          The pointer to mgtSettableComputeMeanLinkDelay, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_linkdelay_mgtsettable(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_MGTSETTABLECOMPUTEMEANLINKDELAY + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM UMGTSETTABLECOMPUTEMEANLINKDELAY configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set useMgtSettableOneStepTxOper
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       Enable status (0, 1) \n
 *                          To set useMgtSettableOneStepTxOper enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_1steptxoper_usemgtsettable(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_USEMGTSETTABLEONESTEPTXOPER + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM USEMGTSETTABLEONESTEPTXOPER configuration to enable mgtsettable 1steptxoper. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get useMgtSettableOneStepTxOper
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       Enable status (0, 1) \n
 *                          The pointer to useMgtSettableOneStepTxOper, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_1steptxoper_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_USEMGTSETTABLEONESTEPTXOPER + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM USEMGTSETTABLEONESTEPTXOPER configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set mgtSettableOneStepTxOper
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in]  txOper       The 1-step transmition operability (0, 1) \n
 *                          To set mgtSettableOneStepTxOper, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_1steptxoper_mgtsettable(uint32 unit, RtkPort port, uint32 domain, uint8 txOper)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = txOper;
    uint32    offset = DV1_PTPCFG_MGTSETTABLEONESTEPTXOPER + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM MGTSETTABLEONESTEPTXOPER configuration to change mgtsettable 1steptxoper. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get mgtSettableOneStepTxOper
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] txOper       The 1-step transmition operability (0, 1) \n
 *                          The pointer to mgtSettableOneStepTxOper, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_get_1steptxoper_mgtsettable(uint32 unit, RtkPort port, uint32 domain, uint8 *txOper)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_MGTSETTABLEONESTEPTXOPER + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        if(NULL == txOper)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM MGTSETTABLEONESTEPTXOPER configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *txOper = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set useMgtSettableGptpCapableMsgInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] enable        The enable status (0, 1) \n
 *                          To set useMgtSettableGptpCapableMsgInterval enable status, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_gptpmsgintval_usemgtsettable(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = enable;
    uint32    offset = DV1_PTPCFG_USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL configuration to enable mgtsettable gptpmsgintval. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get useMgtSettableGptpCapableMsgInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[out] enable       The enable status (0, 1) \n
 *                          The pointer to useMgtSettableGptpCapableMsgInterval, range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_gptpmsgintval_usemgtsettable(uint32 unit, RtkPort port, uint8 *enable)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == enable)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *enable = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set mgtSettableLogGptpCapableMsgInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] intval        Log interval (-5 to 3, -128, 127) \n
 *                          To set log interval of mgtSettableLogGptpCapableMsgInterval, range is -5 ~ 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PTP_INVALID_LOGINTVAL There was wrong syncintval parameter which is not in the valid range, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_gptpmsglogintval_mgtsettable(uint32 unit, RtkPort port, uint32 domain, int8 intval)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = (uint8)intval;
    uint32    offset = DV1_PTPCFG_MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        CHK_FUN_CONTINUE((retval), (dv1_drv_ptp_invalid_logintval(intval)));
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL configuration to change mgtsettable gptpmsgintval. */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get mgtSettableLogGptpCapableMsgInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] intval       Log interval (-5 to 3, -128, 127) \n
 *                          The poiter to mgtSettableLogGptpCapableMsgInterval, range is -5 ~ 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_get_gptpmsglogintval_mgtsettable(uint32 unit, RtkPort port, uint32 domain, int8 *intval)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        if(NULL == intval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *intval = (int8)cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set initLogGptpCapableMsgInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] intval        Log interval (-5 to 3, -128, 127) \n
 *                          Log interval of initLogGptpCapableMsgInterval, range is -5 ~ 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PTP_INVALID_LOGINTVAL There was wrong syncintval parameter which is not in the valid range, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_init_gptpmsgintval(uint32 unit, RtkPort port, uint32 domain, int8 intval)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = (uint8)intval;
    uint32    offset = DV1_PTPCFG_INITLOGGPTPCAPABLEMSGINTVAL + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        CHK_FUN_CONTINUE((retval), (dv1_drv_ptp_invalid_logintval(intval)));
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Set SRAM initial gptpcapable log interval configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get initLogGptpCapableMsgInterval
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] intval       Log interval (-5 to 3, -128, 127) \n
 *                          Log interval of initLogGptpCapableMsgInterval, range is -5 ~ 3, -128, 127
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_get_init_gptpmsgintval(uint32 unit, RtkPort port, uint32 domain, int8 *intval)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_INITLOGGPTPCAPABLEMSGINTVAL + ((uint32)dv1_g_userPort2sysPort[port] * 3u) + domain;

    do
    {
        if(NULL == intval)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domain))
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM initial gptpcapable log interval configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *intval = (int8)cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Set neighbor GPTP RX timeout count.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] timeoutCnt    Timeout count (1 to 255) \n
 *                          The receipt neighbor GPTP message timeout count, the range is 1~255
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 */
RtkApiRet dv1_ptp_set_neigptprxtimerout(uint32 unit, RtkPort port, uint8 timeoutCnt)
{
    RtkApiRet retval = RT_ERR_OK;
    int32     apiret = 0;
    uint8     cfg    = timeoutCnt;
    uint32    offset = DV1_PTPCFG_NEIGHBORGPTPRXTIMEOUT + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(timeoutCnt < DV1_MIN_TIMEOUT)
        {
            retval = -RT_ERR_INPUT;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set SRAM neighbor gptp rx timeout */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, offset, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get neighbor GPTP RX timeout count.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] port          Port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH) \n
 *                          The specific port, range is DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_12TH
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[out] timeoutCnt   Timeout count (1 to 255) \n
 *                          The pointer to receipt neighbor GPTP timeout count, the range is 1~255
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PORT_ID              There was a wrong port parameter, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_neigptprxtimerout(uint32 unit, RtkPort port, uint8 *timeoutCnt)
{
    RtkApiRet retval    = RT_ERR_OK;
    int32     apiret    = 0;
    uint8     cfg       = 0u;
    uint32    cfgoffset = DV1_PTPCFG_NEIGHBORGPTPRXTIMEOUT + (uint32)dv1_g_userPort2sysPort[port];

    do
    {
        if(NULL == timeoutCnt)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(DV1_IS_VALID_PORT_ID_WO_CPUPORT(port) == 0u)
        {
            retval = -RT_ERR_PORT_ID;
            continue;
        }
        /* Get SRAM neighbor gptp rx timeout */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, cfgoffset, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

        *timeoutCnt = cfg;
    } while(0u == 1u);

    return retval;
}

/**
 * @brief Get AS2020 flag to turn on/off AS2020 new features.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[out] as2020En     PTP AS2020 enable status (0, 1) \n
 *                          Pointer to AS2020 enable status, the range is 0, 1
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_as2020_status(uint32 unit, uint8 *as2020En)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = 0u;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == as2020En)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM as2020 configuration */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_AS2020, &cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *as2020En = (uint8)cfg;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set AS2020 flag to turn on/off AS2020 new features.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] as2020En      AS2020 enabled status (0, 1) \n
 *                          To set AS2020 enabled status, the range is 0, 1
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_as2020_status(uint32 unit, uint8 as2020En)
{
    RtkApiRet retval = RT_ERR_OK;
    uint8     cfg    = as2020En;
    int32     apiret = RT_ERR_OK;
    do
    {
        /* Set SRAM as2020 configuration */
        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTPCFG_AS2020, cfg);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Get Over wirte my clockID when mode is TC
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in]  enable       Enable status (0, 1) \n
 *                          Pointer to over wirte my clockID enable status, 0: Disable, 1: Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_overwrite_clockid_get(uint32 unit, uint8 *enable)
{
    uint8     cfg7_0 = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM use my clock id configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_USEMYCLOCKID, &cfg7_0);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = cfg7_0;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Send out Pdelay_req from master port.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in]  enable       Enable status (0, 1) \n
 *                          The pointer to sending out pdelay_req from master port, 0: Disable, 1: Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_send_pdelay_from_masterport(uint32 unit, uint8 *enable)
{
    uint8     cfg7_0 = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM sending pdelay from master port configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_PDELAYTOMASTERPORT, &cfg7_0);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = cfg7_0;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Do not recovery sync message after firstbootuptimetarget.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] enable        Enable status (0, 1) \n
 *                          The pointer to no recovery sync message after firstbootuptimetarget, 0: Disable, 1: Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_skipfirstbootrecovery(uint32 unit, uint8 *enable)
{
    uint8     cfg7_0 = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM no recovery after first boot up time target configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_NORECOVERYFIRSTBOOT, &cfg7_0);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = cfg7_0;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief If switch never get sync from slave port, no recovery
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] domainid      Domain index (0, 1, 2) \n
 *                          The specific time domain, range is 0, 1, 2
 * @param[in] enable        Enable status (0, 1) \n
 *                          The pointer to no recovery if switch never get sync from slave port, 0: Disable, 1: Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_INPUT                There was a wrong input parameter other than port and pointer, return error.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_silenceifneversynced(uint32 unit, uint32 domainid, uint8 *enable)
{
    uint8     cfg7_0 = 0u;
    int32     apiret = RT_ERR_OK;
    RtkApiRet ret    = RT_ERR_OK;
    do
    {
        if(NULL == enable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(1u == dv1_drv_ptp_invalid_ptpdomainid(domainid))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        /* Get SRAM sience if never sync received configuration. */
        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTPCFG_SILENCEIFNEVERSYNCED + domainid, &cfg7_0);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *enable = cfg7_0;
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get PTP Internal Sublayer Service priority enable in switch fabric (Protocol cfg) \n
 *     The internal priority is 6 as defined in 802.1AS-2011. 802.1AS-2020 suggested to use \n
 *     at least two queues to transmit nonexpedited and expedited traffic.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[out] priority_en  Priority enable status (0, 1) \n
 *                          PTP Internal Sublayer Service priority enable status, 0: Disable, 1: Enable
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_NULL_POINTER         There was a wrong pointer parameter which is NULL, return error.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_en_inter_serv_pri(uint32 unit, uint16 *priority_en)
{
    RtkApiRet ret    = RT_ERR_OK;
    int32     apiret = RT_ERR_OK;
    do
    {
        if(NULL == priority_en)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get SRAM internal priority configuration. */
        apiret = dv1_drv_indirect_sram_2bytes_get(unit, DV1_ASSIGNTXPRI_ENABLE, priority_en);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Set PTP Internal Sublayer Service priority enable in switch fabric (Protocol cfg) \n
 *     The internal priority is 6 as defined in 802.1AS-2011. 802.1AS-2020 suggested to use \n
 *     at least two queues to transmit nonexpedited and expedited traffic.
 * @param[in] unit          Unit \n
 *                          Stacking unit
 * @param[in] priority_en   Priority enable status (0, 1) \n
 *                          The pointer to PTP Internal Sublayer Service priority enable status, 0: Disable, 1: Enable
 * @param[out] None
 * @retval  RT_ERR_OK                   The API was executed correctly.
 * @retval -RT_ERR_PTP_CALL_API_FAILED  There was a call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_en_inter_serv_pri(uint32 unit, uint16 priority_en)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint16    data   = priority_en;
    int32     apiret = RT_ERR_OK;

    do
    {
        /* Set SRAM internal priority configuration. */
        apiret = dv1_drv_indirect_sram_2bytes_set(unit, DV1_ASSIGNTXPRI_ENABLE, data);
        if(RT_ERR_OK != apiret)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get PTP Internal Sublayer Service priority enable in switch fabric (Protocol cfg) \n
 *        The internal priority is 6 as defined in 802.1AS-2011. 802.1AS-2020 suggested to use \n
 *        at least two queues to transmit nonexpedited and expedited traffic.
 * @param[in] unit       Unit \n
 *                       Stacking Unit
 * @param[out] priority  Priority (1 to 7) \n
 *                       Internal priority bit 3~0 is the current priority value for PTP.
 * @retval  RT_ERR_OK                        The API was executed correctly.
 * @retval -RT_ERR_INPUT: priority is bigger than 7.
 * @retval -RT_ERR_PTP_CALL_API_FAILED       There was call API internal error, return error.
 */
RtkApiRet dv1_ptp_get_inter_serv_pri(uint32 unit, uint8 *priority)
{
    RtkApiRet retval   = RT_ERR_OK;
    uint8     regvalue = 0u;
    int32     apiret   = 0;

    do
    {
        if(NULL == priority)
        {
            retval = -RT_ERR_NULL_POINTER;
            continue;
        }

        apiret = dv1_drv_indirect_sram_1byte_get(unit, DV1_PTP_PRIORITY, &regvalue);
        if(SUCCESS != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        *priority = regvalue;
    } while(0u == 1u);
    return retval;
}

/**
 * @brief Set PTP Internal Sublayer Service priority enable in switch fabric (Protocol cfg) \n
 *        The internal priority is 6 as defined in 802.1AS-2011. 802.1AS-2020 suggested to use \n
 *        at least two queues to transmit nonexpedited and expedited traffic.
 * @param[in] unit     Unit \n
 *                     Stacking unit
 * @param[in] priority Priority (0 to 7) \n
 *                     Internal priority bit 2~0 is the current priority value for PTP. Others are reserved
 * @param[out] None
 * @retval RT_ERR_OK                     The API was executed correctly.
 * @retval -RT_ERR_INPUT: priority is > 7
 * @retval -RT_ERR_PTP_CALL_API_FAILED   There was call API internal error, return error.
 */
RtkApiRet dv1_ptp_set_inter_serv_pri(uint32 unit, uint8 priority)
{
    RtkApiRet retval  = RT_ERR_OK;
    int32     apiret  = 0;
    uint32    priMask = 0u;

    do
    {
        if(priority > 7u)
        {
            retval = -RT_ERR_INPUT;
            continue;
        }

        apiret = dv1_drv_indirect_sram_1byte_set(unit, DV1_PTP_PRIORITY, priority);
        if(SUCCESS != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }
        /* Assign NIC Priority Buffer */
        priMask = ((uint32)1u << priority);
        apiret  = dv1_reg_field_write(unit, DV1_RXPBUF_PRIORITY_MSK, DV1_RXPBUF_PRIORITY_MSK_RXPRI_PRI_MSK_LSP, DV1_RXPBUF_PRIORITY_MSK_RXPRI_PRI_MSK_LEN, priMask);
        if(RT_ERR_OK != apiret)
        {
            retval = -RT_ERR_PTP_CALL_API_FAILED;
            continue;
        }

    } while(0u == 1u);
    return retval;
}

/**
 * @brief Directly Get time in hardware register.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] timesel time selection. (0 to 4)
 *                    Hardware time module: 1:Local, 2:Domain 0, 3:Domain 1, 4: Domain 2.
 * @param[out] ts Time (N/A)
 *                Getting time in hardware register. Please refer to PtpTs_t.
 *
 * @retval RT_ERR_OK                      PTP ready status was gotten correctly.
 * @retval -RT_ERR_INPUT:                 timesel is > 4
 * @retval -RT_ERR_NULL_POINTER           There was wrong pointer parameter which is NULL, return error.
 */
RtkApiRet dv1_ptp_time_direct_get(uint32 unit, uint8 timesel, PtpTs_t *ts)
{
    uint32    u32data = 1u, waitcnt = 0u;
    uint32    timeoutCnt = (dv1_g_ptpTimeConfigTimeout >= DV1_PTP_POLLING_INTERVAL) ? (dv1_g_ptpTimeConfigTimeout / DV1_PTP_POLLING_INTERVAL) : 1u;
    RtkApiRet ret        = RT_ERR_OK;
    do
    {
        if(NULL == ts)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(timesel > 4u)
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        (void)dv1_reg_read(unit, DV1_PTP_CONTROL_AND_CLK_CONFIG, &u32data);
        (void)dv1_reg_field_set(DV1_PTP_CONTROL_AND_CLK_CONFIG, DV1_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LSP, DV1_PTP_CONTROL_AND_CLK_CONFIG_PTP_ENABLE_LEN, 1u, &u32data);
        (void)dv1_reg_field_set(DV1_PTP_CONTROL_AND_CLK_CONFIG, DV1_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_SET_LSP, DV1_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_SET_LEN, 1u, &u32data);
        (void)dv1_reg_field_set(DV1_PTP_CONTROL_AND_CLK_CONFIG, DV1_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_MODE_LSP, DV1_PTP_CONTROL_AND_CLK_CONFIG_CLK_ADJ_MODE_LEN, 2u, &u32data);
        (void)dv1_reg_field_set(DV1_PTP_CONTROL_AND_CLK_CONFIG, DV1_PTP_CONTROL_AND_CLK_CONFIG_TIME_CONFIG_SEL_LSP, DV1_PTP_CONTROL_AND_CLK_CONFIG_TIME_CONFIG_SEL_LEN, timesel, &u32data);
        (void)dv1_reg_write(unit, DV1_PTP_CONTROL_AND_CLK_CONFIG, u32data);
        for(waitcnt = 0u; waitcnt < timeoutCnt; waitcnt++)
        {
            (void)dv1_reg_read(unit, DV1_PTP_CONTROL_AND_CLK_CONFIG, &u32data);
            if((u32data & 0x10000u) == 0u)
            {
                (void)dv1_reg_read(unit, DV1_PTP_TIME_CONFIG_NS, &ts->nanoSec);
                (void)dv1_reg_read(unit, DV1_PTP_TIME_CONFIG_S_0, &ts->low32Sec);
                (void)dv1_reg_read(unit, DV1_PTP_TIME_CONFIG_S_1, &u32data);
                ts->up16Sec = (uint16)u32data;
                break;
            }
            CHK_FUN_RET(ret, rtk_usecond_delay(DV1_PTP_POLLING_INTERVAL));
        }
        if(waitcnt >= timeoutCnt)
        {
            ret = -RT_ERR_PTP_CALL_API_FAILED;
        }
    } while(0u == 1u);
    return ret;
}
/**@}*/  /* PTP_ASIC */
/**@}*/  /* PTP_PROTOCOL */
         /********* END  TIME SYNC/PTP *****************/
/**@}*/  // PTP