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
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtkas_debug.h"

#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_asicdrv_smi.h"

/* MDX master */
uint8 cva_mdx_phyID[4];

/**
 * @brief
 *
 * @param[in] phyID
 * @param[in] baudr
 * @param[in] preamble
 * @retval
 */
int32 cva_drv_mdx_init(const uint8 phyID[], uint8 baudr, uint8 preamble)
{
    /* The mdx clock = 12.5MHz / baudr */
    int32 retVal = (int32)RT_ERR_OK;

    do
    {
        /* Initialize the baud rate */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL0,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL0_SEL_SMI_FREQ_LSP,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL0_SEL_SMI_FREQ_LEN,
                                                                (uint32)baudr));
        /* Initialize the preamble */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL0,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL0_SEL_PREM_LSP,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL0_SEL_PREM_LEN,
                                                                (uint32)preamble));
        /* Initialize GPIOD pins as MDX interface */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_SEL_MDIOMASTER_GPIO_LSP,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_SEL_MDIOMASTER_GPIO_LEN,
                                                                0u));

        cva_mdx_phyID[0] = phyID[0];
        cva_mdx_phyID[1] = phyID[1];
        cva_mdx_phyID[2] = phyID[2];
        cva_mdx_phyID[3] = phyID[3];
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief
 *
 * @retval
 */
int32 cva_drv_mdx_polling_cmd_bit(void)
{
    int32  retVal  = (int32)RT_ERR_OK;
    uint32 regval  = (uint32)0u;
    uint32 timeout = (uint32)0u;
    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(RTK_TIMER_MDX_USECOUND));
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_get((uint32)CVA_UNIT,
                                                                (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0,
                                                                (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_CDM_LSP,
                                                                (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_CDM_LEN,
                                                                &regval));
        timeout++;
    } while((regval != (uint32)0x0u) && (timeout < (uint32)RTK_TIMER_MDX_TRYTIMES));

    if((uint32)0u == timeout)
    {
        retVal = -(int32)RT_ERR_BUSYWAIT_TIMEOUT;
    }
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
int32 cva_drv_mdx_register_set(uint8 phyID_idx, uint16 page, uint8 reg, uint16 mdxData)
{
    int32  retVal  = (int32)RT_ERR_OK;
    uint32 regVal  = (uint32)0u;
    uint32 regTemp = (uint32)0u;

    do
    {
        /* Save the current state of the GPIO pins' function selection */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_get((uint32)CVA_UNIT,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LSP,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LEN,
                                                                &regTemp));

        /* Enable MDX master via GPIO */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LSP,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LEN,
                                                                1u));

        /* Set PHY ID */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL1,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LSP,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LEN,
                                                                (uint32)cva_mdx_phyID[phyID_idx]));
        /* Set page */
        regVal = ((uint32)page << 16u) | (uint32)0x1FF9u;
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT, (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, cva_drv_mdx_polling_cmd_bit());

        /* Set control register and trigger*/
        regVal = ((uint32)mdxData << 16u) | ((uint32)reg << 8u) | (uint32)0xF9u;
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT, (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, cva_drv_mdx_polling_cmd_bit());

        /* Restore the state of the GPIO pins' function selection */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LSP,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LEN,
                                                                regTemp));
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
int32 cva_drv_mdx_register_get(uint8 phyID_idx, uint16 page, uint8 reg, uint16 *mdxData)
{
    int32  retVal   = (int32)RT_ERR_OK;
    uint32 regVal   = (uint32)0u;
    uint32 regTemp  = (uint32)0u;
    uint32 data_tmp = (uint32)0u;

    do
    {
        /* Save the current state of the GPIO pins' function selection */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_get((uint32)CVA_UNIT,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LSP,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LEN,
                                                                &regTemp));

        /* Enable MDX master via GPIO */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LSP,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LEN,
                                                                1u));

        /* Set PHY ID */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL1,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LSP,
                                                                (uint32)CVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LEN,
                                                                (uint32)cva_mdx_phyID[phyID_idx]));

        /* Set page */
        regVal = ((uint32)page << 16u) | (uint32)0x1FF9u;
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT, (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, cva_drv_mdx_polling_cmd_bit());

        /* set register */
        regVal = ((uint32)reg << 8u) | (uint32)0xFBu;
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT, (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        /* polling controller */
        CHK_FUN_CONTINUE(retVal, cva_drv_mdx_polling_cmd_bit());

        /* read data */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_get((uint32)CVA_UNIT,
                                                                (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0,
                                                                (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_DATA_15_0_LSP,
                                                                (uint32)CVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_DATA_15_0_LEN,
                                                                &data_tmp));
        *mdxData = (uint16)(data_tmp & 0xFFFFu);

        /* Restore the state of the GPIO pins' function selection */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_reg_field_set((uint32)CVA_UNIT,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LSP,
                                                                (uint32)CVA_GPIO_FUNCSELECT_REG_MDIO_MST_GPIO_EN_LEN,
                                                                regTemp));
    } while(0u == 1u);

    return retVal;
}
