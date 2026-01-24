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
 * @addtogroup RTL907DVA_SPI_EXAMPLE RTL907XD_VA SPI Flash Example
 * @{
 */
/**
 * @addtogroup RTL907DVA_SPI_READ_ID_EXAMPLE SPI Flash Read ID Example
 *
 * @brief <b>Read ID of SPI flash</b>
 *
 * @section Purpose
 *   Read the flash ID
 *
 * @section Description
 *          This example shows how to read the flash chip ID which includes 8 bits Manufacturer ID and 16 bits Device ID.
 *
 * @image html flash/read_id.jpg
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_spi_flash_read_id(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_spi_flash_read_id(uint32 unit)
{
    RtkApiRet   ret     = RT_ERR_OK;
    uint32      flashId = (uint32)0u;
    UnitChip_t  unitChip;
    SpiReadId_t spiReadId;
    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;
    do
    {
        /* Step 1. Initialize SPI Flash */
        CHK_FUN_CONTINUE(ret, rtk_spiflash_init(unitChip));

        /* Step 2. Get the flash chip ID */
        spiReadId.flashId = &flashId;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_read_id(unitChip, &spiReadId));
        if((uint32)0x00FFFFFFu == flashId)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        /* Expected Behavior:
         *   Flash ID can be read back.
         */
    } while(0u == 1u);
    return ret;
}
/**@}*/ /* RTL907DVA_SPI_READ_ID_EXAMPLE */
/**@}*/ /* RTL907DVA_SPI_EXAMPLE */
/**@}*/ /* SPI_EXAMPLE */
