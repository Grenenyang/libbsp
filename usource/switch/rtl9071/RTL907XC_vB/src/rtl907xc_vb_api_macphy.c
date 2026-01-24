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

/******************************************************************************/
/* Includes                                                                    */
/******************************************************************************/
#include "rtl907xc_vb_api_macphy.h"
#include "rtl907xc_vb_reg_list.h"
#include "rtl907xc_vb_reg_struct.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* MACPHY Control Registers */
/* [15:0] - SCKDV: SSI Clock Divider
   [23:16] - Rx Sample Delay
   [24] - Clock source, 0: 150 MHz, 1: 200 MHz
   [25] - Macphy external config error */
#define CVB_MACPHY_BAUD_RATE_CONFIG_REG_0       CVB_HW_MISC_DMY_RESIGSER_26
#define CVB_MACPHY_BAUD_RATE_CONFIG_REG_1       CVB_HW_MISC_DMY_RESIGSER_27
#define CVB_MACPHY_RX_SAMPLE_DELAY_SHIFT        (16u)
#define CVB_MACPHY_BAUD_RATE_CLOCK_SOURCE_SHIFT (24u)
#define CVB_MACPHY_RX_SAMPLE_DELAY_MASK         (0xFFu)
#define CVB_MACPHY_BAUD_RATE_CLOCK_DIVIDER_MASK (0xFFFFu)
#define CVB_MACPHY_INTF_COMBO_SEL_SPI           (0x1u)
#define CVB_MACPHY_EXT_CONFIG_ERROR_LSP         (25u)
#define CVB_MACPHY_SEQE_LSP                     (26u)

/**
 * @defgroup RTK_MODULE_MACPHY MACPHY
 * @brief MACPHY APIs
 * @{
 */

/**
 * @addtogroup MACPHY_GET Get MACPHY SPI parameter
 * @{
 */

/**
 * @defgroup MACPHY_GET_ASIC Asic Driver
 * @brief MACPHY asic driver
 * @{
 */

/**
 * @brief Get SPI parameters of macphy.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] portId        Port Identification (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_2ND) \n
 *                          Port Identification which is used for macphy.
 * @param[out] param        SPI Parameters. \n
 *                          The pointer of SPI Parameters.
 * @retval RT_ERR_OK        The SPI parameters have been read from registers.
 * @retval -RT_ERR_INPUT    The portId is invalid.
 * @retval Others           Please refer to cvb_reg_read().
 */
RtkApiRet cvb_macphy_get_spi_param(uint32 unit, RtkPort portId, MacphySpiParam_t *param)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    configAddr  = 0u;
    uint32    configValue = 0u;

    do
    {
        /* Check portId is valid */
        if(portId == CVB_RTK_USER_PORT_1ST)
        {
            configAddr = CVB_MACPHY_BAUD_RATE_CONFIG_REG_0;
        }
        else if(portId == CVB_RTK_USER_PORT_2ND)
        {
            configAddr = CVB_MACPHY_BAUD_RATE_CONFIG_REG_1;
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        /* Get the config value */
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, configAddr, &configValue));

        /* Parse config value to SPI parameters */
        param->clockDivider  = configValue & CVB_MACPHY_BAUD_RATE_CLOCK_DIVIDER_MASK;
        param->rxSampleDelay = (configValue >> CVB_MACPHY_RX_SAMPLE_DELAY_SHIFT) & CVB_MACPHY_RX_SAMPLE_DELAY_MASK;
        param->clockSrc      = (configValue >> CVB_MACPHY_BAUD_RATE_CLOCK_SOURCE_SHIFT) & 0x1u;
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*MACPHY_GET_ASIC*/
/**@}*/ /*MACPHY_GET*/
/**@}*/ /* MACPHY */
