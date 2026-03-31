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
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @defgroup SPI_EXAMPLE SPI Flash Example
 * @{
 */
/**
 * @defgroup RTL907XCVA_SPI_EXAMPLE RTL907XC_VA SPI Flash Example
 * @{
 */
/**
 * @defgroup RTL907XCVA_SPI_ACCESS_EXAMPLE SPI Flash Access Example
 *
 * @brief <b>Access SPI flash</b>
 *
 * @section Purpose
 *   Access the flash, including read, program and erase operation.
 *
 * @section Description
 *   This example shows how to erase flash and program flash with specific contents to different addresses.\n
 *   Then, read back to compare if the content is correct after the operation.
 *
 * @image html flash/access.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_spi_flash_access(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_spi_flash_access(uint32 unit)
{
    RtkApiRet ret = RT_ERR_OK;
    /* The buffer for the data programmed into the flash */
    const uint8 wBuf[4] = {(uint8)0x12u, (uint8)0x34u, (uint8)0x56u, (uint8)0x78u};
    /* The buffer for the data read from the flash. */
    uint8            rBuf0[10] = {(uint8)0u};
    uint8            rBuf1[10] = {(uint8)0u};
    uint8            i;
    UnitChip_t       unitChip;
    SpiEraseSector_t spiEraseSector;
    SpiEraseBlock_t  spiEraseBlock;
    SpiProgram_t     spiProgram;
    SpiRead_t        spiRead;
    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;

    do
    {
        /* Step 1. Initialize SPI Flash */
        CHK_FUN_CONTINUE(ret, rtk_spiflash_init(unitChip));

        /* Step 2. Erase Sector 0x3F (flash address : 0x3F000~0x3FFFF) to "1" */
        spiEraseSector.sectorAddr = 0x3Fu;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_erasesector(unitChip, &spiEraseSector));

        /* Step 3. Program the flash (start from address 0x3F000) with 4 bytes data */
        spiProgram.address = 0x3F000u;
        spiProgram.len     = 4u;
        spiProgram.buf     = wBuf;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_program(unitChip, &spiProgram));

        /* Step 4. Read the flash (start from address 0x3F000) with 10 bytes data */
        spiRead.address = 0x3F000u;
        spiRead.len     = 10u;
        spiRead.buf     = rBuf0;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_read(unitChip, &spiRead));

        /* Step 5. Erase Block 0x3 (flash address : 0x30000~0x3FFFF) to "1" */
        spiEraseBlock.blockAddr = 0x3u;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_eraseblock(unitChip, &spiEraseBlock));

        /* Step 6. Program the flash (start from address 0x30000) with 4 bytes data */
        spiProgram.address = 0x30000u;
        spiProgram.len     = 4u;
        spiProgram.buf     = wBuf;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_program(unitChip, &spiProgram));

        /* Step 7. Read the flash (start from address 0x30000) with 10 bytes data */
        spiRead.address = 0x30000u;
        spiRead.len     = 10u;
        spiRead.buf     = rBuf1;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_read(unitChip, &spiRead));

        /* Expected Behaviour:
         *   The data programmed into the flash and the data read from the flash are matched.
         */
        for(i = 0u; i < (uint8)4u; i++)
        {
            if(wBuf[i] != rBuf0[i])
            {
                ret = -RT_ERR_FAILED;
            }
        }
        for(i = 0u; i < (uint8)4u; i++)
        {
            if(wBuf[i] != rBuf1[i])
            {
                ret = -RT_ERR_FAILED;
            }
        }
    } while(0u == 1u);
    return ret;
}
/**@}*/ /* RTL907XCVA_SPI_ACCESS_EXAMPLE */
/**@}*/ /* RTL907XCVA_SPI_EXAMPLE */
/**@}*/ /* SPI_EXAMPLE */
/**@}*/ /* EXAMPLE */
