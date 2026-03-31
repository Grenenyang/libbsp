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
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtkas_api_spi_flash.h"

/**
 * @addtogroup SPI_EXAMPLE SPI Flash Example
 * @{
 */
/**
 * @addtogroup RTL907CVB_SPI_EXAMPLE RTL907XC_VB SPI Flash Example
 * @{
 */
/**
 * @addtogroup RTL907CVB_SPI_ERASE_ALL_EXAMPLE SPI Flash Erase All Example
 *
 * @brief <b>Erase SPI flash</b>
 *
 * @section Purpose
 *   Erase the whole flash
 *
 * @section Description
 *   This example shows how to erase whole flash chip.\n
 *   The every bit of flash memory will all be "1" after erasing.
 *
 * @image html flash/erase_all.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_spi_flash_erase_all(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_spi_flash_erase_all(uint32 unit)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t unitChip;
    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Initialize SPI Flash */
        CHK_FUN_CONTINUE(ret, rtk_spiflash_init(unitChip));

        /* Step 2. Erase the whole flash memory */
        CHK_FUN_CONTINUE(ret, rtk_spiflash_eraseall(unitChip));

        /* Expected Behaviour:
         *   The every bit of whole flash memory is erased to "1"
         */
    } while(0u == 1u);
    return ret;
}
/**@}*/ /* RTL907CVB_SPI_ERASE_ALL_EXAMPLE */
/**@}*/ /* RTL907CVB_SPI_EXAMPLE */
/**@}*/ /* SPI_EXAMPLE */
