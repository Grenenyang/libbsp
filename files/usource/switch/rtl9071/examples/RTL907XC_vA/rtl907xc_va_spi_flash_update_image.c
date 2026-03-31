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
 * @addtogroup RTL907XCVA_SPI_EXAMPLE RTL907XC_VA SPI Flash Example
 * @{
 */
/**
 * @addtogroup RTL907XCVA_SPI_UPDTE_EXAMPLE SPI Update Flash Example
 *
 * @brief <b>Update image to SPI flash</b>
 *
 * @section Purpose
 *   Update the switch image including firmware and configuration.
 *
 * @section Description
 *   This example shows how to update the image.\n
 *   Before updating the image, calibration needs to be done with a specific channel and speed first.\n
 *   This example selects single channel and 50MHz as calibration parameters for updating the image.\n
 *   When this program completes execution, the system should boot normally upon resetting the chip.
 *
 * @image html flash/update.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_spi_flash_update_image(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_spi_flash_update_image(uint32 unit)
{
    RtkApiRet     ret = RT_ERR_OK;
    SpicPattern_t spicPattern;
    UnitChip_t    unitChip;
    SpiCal_t      spiCal;
    SpiUpdate_t   spiUpdate;
    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;

    do
    {
        /* Step 1. Set SPIC to single channel, clock rate to 50MHz, and perform the calibration */
        spiCal.ch          = RTK_SPI_FLASH_SINGLE_CHANNEL;
        spiCal.spd         = RTK_SPI_FLASH_50MHz;
        spiCal.spicPattern = &spicPattern;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_cal(unitChip, &spiCal));

        /* Step 2. Update the flash with binary file */
        spiUpdate.fileName = "./image/RTL907XC_vA/Default_test.bin";
        CHK_FUN_CONTINUE(ret, rtk_spiflash_update(unitChip, &spiUpdate));

        /* Expected Behaviour:
         *   The flash is updated with the binary file correctly.
         */
    } while(0u == 1u);
    return ret;
}
/**@}*/ /* RTL907XCVA_SPI_UPDTE_EXAMPLE */
/**@}*/ /* RTL907XCVA_SPI_EXAMPLE */
/**@}*/ /* SPI_EXAMPLE */
