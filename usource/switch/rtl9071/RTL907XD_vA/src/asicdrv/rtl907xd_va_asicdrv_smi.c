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

#include "rtkas_std.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_system_porting.h"

#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtkas_debug.h"

#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_smi.h"
#include "rtl907xd_va_asicdrv_timer.h"
#include "rtkas_api_smi.h"
#include "rtl907xd_va_api_esm.h"

/* MDX master */
static uint8 dva_mdx_phyID[4];

/**
 * @brief
 *
 * @param[in] phyID
 * @param[in] gpio
 * @param[in] baudr
 * @param[in] preamble
 * @retval
 */
RtkApiRet dva_mdx_init(const uint8 phyID[], uint8 gpio, uint8 baudr, uint8 preamble)
{
    /* The mdx clock = 12.5MHz / baudr */
    int32  retVal = (int32)RT_ERR_OK;
    uint32 gpioC, gpioD;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set((uint32)DVA_UNIT,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_SMI_FREQ_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_SMI_FREQ_LEN,
                                                                (uint32)baudr));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set((uint32)DVA_UNIT,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_PREM_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_PREM_LEN,
                                                                (uint32)preamble));
        /* Select GPIOC or GPIOD to be used as MDC/MDIO Master */
        if((uint8)EM_MDX_MASTER_GPIOC_2_1 == gpio)
        {
            gpioC = (uint32)DVA_PAD_MUX_GPIOC_1_2_MDIO_MASTER_FUNC & 0xFu;
            gpioD = (uint32)DVA_PAD_MUX_GPIOD_0_1_DEFAULT_FUNC & 0xFu;
        }
        else
        {
            gpioC = (uint32)DVA_PAD_MUX_GPIOC_1_2_DEFAULT_FUNC & 0xFu;
            gpioD = (uint32)DVA_PAD_MUX_GPIOD_0_1_MDIO_MASTER_FUNC & 0xFu;
        }

        /* Config GPIOC_1 */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set((uint32)DVA_UNIT,
                                                                (uint32)DVA_PAD_MUX_GPIOC_1,
                                                                (uint32)DVA_PAD_MUX_GPIOC_1_RG_GPIOC_1_FUNC_SEL_LSP,
                                                                (uint32)DVA_PAD_MUX_GPIOC_1_RG_GPIOC_1_FUNC_SEL_LEN,
                                                                gpioC));
        /* Config GPIOC_2 */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set((uint32)DVA_UNIT,
                                                                (uint32)DVA_PAD_MUX_GPIOC_2,
                                                                (uint32)DVA_PAD_MUX_GPIOC_2_RG_GPIOC_2_FUNC_SEL_LSP,
                                                                (uint32)DVA_PAD_MUX_GPIOC_2_RG_GPIOC_2_FUNC_SEL_LEN,
                                                                gpioC));
        /* Config GPIOD_0 */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set((uint32)DVA_UNIT,
                                                                (uint32)DVA_PAD_MUX_GPIOD_0,
                                                                (uint32)DVA_PAD_MUX_GPIOD_0_RG_GPIOD_0_FUNC_SEL_LSP,
                                                                (uint32)DVA_PAD_MUX_GPIOD_0_RG_GPIOD_0_FUNC_SEL_LEN,
                                                                gpioD));
        /* Config GPIOD_1 */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set((uint32)DVA_UNIT,
                                                                (uint32)DVA_PAD_MUX_GPIOD_1,
                                                                (uint32)DVA_PAD_MUX_GPIOD_1_RG_GPIOD_1_FUNC_SEL_LSP,
                                                                (uint32)DVA_PAD_MUX_GPIOD_1_RG_GPIOD_1_FUNC_SEL_LEN,
                                                                gpioD));

        dva_mdx_phyID[0] = phyID[0];
        dva_mdx_phyID[1] = phyID[1];
        dva_mdx_phyID[2] = phyID[2];
        dva_mdx_phyID[3] = phyID[3];
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief
 *
 * @retval
 */
RtkApiRet dva_mdx_polling_cmd_bit(uint32 unit)
{
    RtkApiRet          retVal = (RtkApiRet)RT_ERR_OK;
    RegPollingConfig_t mdxPollingConfig;

    /* Enabled(CHECK_EN Bit[2]) Hardware CRC to calculate CRC value */
    mdxPollingConfig.triggerRegAddr = DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0;
    mdxPollingConfig.triggerRegLsp  = DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_CDM_LSP;
    mdxPollingConfig.triggerRegLen  = DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_CDM_LEN;
    mdxPollingConfig.action         = 1u;
    mdxPollingConfig.chkPortClk     = 1u;
    mdxPollingConfig.port           = 0u;
    /* Polling CRC fail bit, because no update golden CRC  */
    mdxPollingConfig.pollingRegAddr = DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0;
    mdxPollingConfig.pollingRegLsp  = DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_CDM_LSP;
    mdxPollingConfig.pollingRegLen  = DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_CDM_LEN;
    mdxPollingConfig.expVal         = 0u;
    mdxPollingConfig.eventID        = DVA_EM_EVENT_MDX_MASTER_POLLING_TIMEOUT;

    CHK_FUN_RET(retVal, dva_drv_timer_polling_reg_timeout(unit, &mdxPollingConfig, (uint32)DVA_MDX_POLLING_MAX_US, (uint32)DVA_MDX_POLLING_MIN_US));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] phyID_idx
 * @param[in] page
 * @param[in] reg
 * @param[in] mdxData
 * @retval
 */
RtkApiRet dva_mdx_register_set(uint8 phyID_idx, uint16 page, uint8 reg, uint16 mdxData)
{
    RtkApiRet retVal = (int32)RT_ERR_OK;
    uint32    regVal = (uint32)0u;
    uint32    unit   = (uint32)DVA_UNIT;

    do
    {
        /* Set PHY ID */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LEN,
                                                                (uint32)dva_mdx_phyID[phyID_idx]));
        /* Set page */
        regVal = ((uint32)page << 16u) | (uint32)0x1FF9u;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, dva_mdx_polling_cmd_bit(unit));

        /* Set control register and trigger*/
        regVal = ((uint32)mdxData << 16u) | ((uint32)reg << 8u) | (uint32)0xF9u;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, dva_mdx_polling_cmd_bit(unit));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief
 *
 * @param[in] phyID_idx
 * @param[in] page
 * @param[in] reg
 * @param[in] mdxData
 * @retval
 */
RtkApiRet dva_mdx_register_get(uint8 phyID_idx, uint16 page, uint8 reg, uint16 *mdxData)
{
    RtkApiRet retVal   = (int32)RT_ERR_OK;
    uint32    regVal   = (uint32)0u;
    uint32    data_tmp = (uint32)0u;
    uint32    unit     = (uint32)DVA_UNIT;

    do
    {
        /* Set PHY ID */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit,
                                                     (uint32)DVA_MDC_MDIO_MASTER_CONTROL1,
                                                     (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LSP,
                                                     (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LEN,
                                                     (uint32)dva_mdx_phyID[phyID_idx]));

        /* Set page */
        regVal = ((uint32)page << 16u) | (uint32)0x1FF9u;
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, dva_mdx_polling_cmd_bit(unit));

        /* set register */
        regVal = ((uint32)reg << 8u) | (uint32)0xFBu;
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, dva_mdx_polling_cmd_bit(unit));

        /* read data */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit,
                                                    (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0,
                                                    (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_DATA_15_0_LSP,
                                                    (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_DATA_15_0_LEN,
                                                    &data_tmp));
        *mdxData = (uint16)(data_tmp & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}
