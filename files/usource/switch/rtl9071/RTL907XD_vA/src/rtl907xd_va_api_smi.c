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

#include "rtl907xd_va_asicdrv_smi.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"

/**
 * @addtogroup RTK_MODULE_SMI_MASTER
 * @{
 */
/**
 * @addtogroup MDX_INIT
 * @{
 */
/**
 * @addtogroup MDX_INIT_ASIC
 * @{
 */

/**
 * @brief MDX initialization.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] gpio  GPIO select (EM_MDX_MASTER_GPIOD_1_0 to EM_MDX_MASTER_GPIOC_2_1) \n
 *                  Select GPIOC or GPIOD as the MDX interface.
 * @param[in] baudr Baud rate (1 to 31) \n
 *                  The output clock equals the value of 12.5 MHz divided by baudr.
 * @param[in] preamble Preamble bit length (EM_MDX_PREAMBLE_0 to EM_MDX_PREAMBLE_32) \n
 *                     EM_MDX_PREAMBLE_0 = 0 (0-bit preamble) \n
 *                     EM_MDX_PREAMBLE_8 = 1 (8-bit preamble) \n
 *                     EM_MDX_PREAMBLE_16 = 2 (16-bit preamble) \n
 *                     EM_MDX_PREAMBLE_32 = 3 (32-bit preamble)
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT The baud rate setting is out of range. \n
 *                       The preamble is out of range.
 * @retval Others        Please refer to dva_drv_indirect_reg_field_set() in rtl907xd_va_asicdrv_mem_ctrl.c.
 */
RtkApiRet dva_std_mdx_init(uint32 unit, MdxMasterPin_e gpio, MdxBaudRate baudr, MdxPreamble_e preamble)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;
    uint32    gpioC, gpioD;

    do
    {
        /* The MDX clock = 12.5MHz / baudr */
        if((baudr > (MdxBaudRate)0x1F) || ((MdxBaudRate)0u == baudr))
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        if(preamble >= EM_MDX_PREAMBLE_END)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        if(EM_MDX_MASTER_GPIO_END <= gpio)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        /* Set baud rate */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_SMI_FREQ_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_SMI_FREQ_LEN,
                                                                (uint32)baudr));
        /* Set preamble */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_PREM_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL0_SEL_PREM_LEN,
                                                                (uint32)preamble));

        /* Select the pins of GPIOC or GPIOD as the MDX interface */
        if(EM_MDX_MASTER_GPIOC_2_1 == gpio)
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
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* MDX_INIT_ASIC */
/**@}*/ /* MDX_INIT */

/**
 * @addtogroup MDX_ACCESS
 * @{
 */
/**
 * @addtogroup MDX_ACCESS_ASIC
 * @{
 */

/**
 * @brief Standard MDX read operation to access the register of the slave device.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] phyID PHY ID (0 to 31) \n
 *                  The ID of the slave device.
 * @param[in] regNum Register number (0 to 31) \n
 *                   The register number of the slave device.
 * @param[out] mdxData MDC/MDIO data buffer \n
 *                     Read data buffer
 * @retval RT_ERR_OK           The value of the register was obtained.
 * @retval RT_ERR_NULL_POINTER The pointer is null.
 * @retval -RT_ERR_INPUT       Illegal PHY ID or register.
 * @retval Others        Please refer to dva_drv_indirect_reg_field_set(), dva_drv_indirect_reg_field_get() \n
 *                       and dva_drv_mdx_polling_cmd_bit() in rtl907xd_va_asicdrv_mem_ctrl.c.
 */
RtkApiRet dva_std_mdx_register_get(uint32 unit, MdxSlvPhyID phyID, MdxSlvReg regNum, MdxSlvData *mdxData)
{
    RtkApiRet retVal   = (int32)RT_ERR_OK;
    uint32    regVal   = (uint32)0u;
    uint32    data_tmp = (uint32)0u;

    do
    {
        /* Check if mdxData is a null pointer */
        if(NULL == mdxData)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(((MdxSlvPhyID)RTK_STD_PHY_ADDR_MAX < phyID) || ((MdxSlvReg)RTK_STD_PHY_REG_MAX < regNum))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* Set PHY ID */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LEN,
                                                                (uint32)phyID));

        /* Set control reg */
        regVal = ((uint32)regNum << 8u) | (uint32)0xFAu;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        /* Polling controller */
        CHK_FUN_CONTINUE(retVal, dva_mdx_polling_cmd_bit(unit));

        /* Read data */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit,
                                                                (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0,
                                                                (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_DATA_15_0_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_DATA_15_0_LEN,
                                                                &data_tmp));
        *mdxData = (uint16)(data_tmp & 0x0000FFFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Standard MDX write operation to access the register of the slave device.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] phyID PHY ID (0 to 31) \n
 *                  The ID of the slave device.
 * @param[in] regNum Register number (0 to 31) \n
 *                   The register number of the slave device.
 * @param[in] mdxData MDC/MDIO data buffer (0 to 0xFFFF) \n
 *                    Programmed data buffer
 * @param[out] None
 * @retval RT_ERR_OK     The target register was set.
 * @retval -RT_ERR_INPUT Illegal PHY ID or register.
 * @retval Others        Please refer to dva_drv_indirect_reg_field_set(), dva_drv_indirect_reg_field_get() \n
 *                       and dva_drv_mdx_polling_cmd_bit() in rtl907xd_va_asicdrv_mem_ctrl.c.
 */
RtkApiRet dva_std_mdx_register_set(uint32 unit, MdxSlvPhyID phyID, MdxSlvReg regNum, MdxSlvData mdxData)
{
    RtkApiRet retVal = (int32)RT_ERR_OK;
    uint32    regVal = (uint32)0u;

    do
    {
        if(((MdxSlvPhyID)RTK_STD_PHY_ADDR_MAX < phyID) || ((MdxSlvReg)RTK_STD_PHY_REG_MAX < regNum))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* Set PHY ID */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LSP,
                                                                (uint32)DVA_MDC_MDIO_MASTER_CONTROL1_PHYADDR_4_0_LEN,
                                                                (uint32)phyID));

        /* Set control register */
        regVal = ((uint32)mdxData << 16) | ((uint32)regNum << 8u) | (uint32)0xF8u;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, (uint32)DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0, 0u, 32u, regVal));

        CHK_FUN_CONTINUE(retVal, dva_mdx_polling_cmd_bit(unit));

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* MDX_ACCESS_ASIC */
/**@}*/ /* MDX_ACCESS */
/**@}*/ /* RTK_MODULE_SMI_MASTER */
