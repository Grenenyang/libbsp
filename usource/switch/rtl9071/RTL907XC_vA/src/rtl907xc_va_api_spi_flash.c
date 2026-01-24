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

#include "rtl907xc_va_asicdrv_flash.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtkas_api_spi_flash.h"
/**
 * @addtogroup RTK_MODULE_SPI_FLASH SPI Flash
 * @{
 */

/**
 * @addtogroup SPI_INIT SPI Software initialization
 * @{
 */

/**
 * @addtogroup SPI_INIT_ASIC Low Level Driver
 * @brief SPIC Low Level Driver
 * @{
 */

/**
 * @brief Initialize the software settings to access SPI flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Initialization successful.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_FAILED Failed to initialize SPIC.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),
 *                cva_drv_spi_flash_init(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_init(uint32 unit)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Occupied by SDK */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
        /* Backup page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
        /* Set page to SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Initialize SPIC */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_init(unit));
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}
/**@}*/ /*SPI_INIT_ASIC*/
/**@}*/ /*SPI_INIT*/

/**
 * @addtogroup SPI_CAL Set Speed and Channel for Calibration
 * @{
 */
/**
 * @addtogroup SPI_CAL_ASIC Low Level Driver
 * @brief SPIC Low Level Driver
 * @{
 */

/**
 * @brief Set the speed and channel for SPI Master, and then calibrate to determine the correct settings between the SPI master and flash for flash memory access. \n
 *        rtk_spiflash_update will store the settings into the image header.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ch SPI channel (CVA_RTK_SPI_FLASH_SINGLE_CHANNEL,CVA_RTK_SPI_FLASH_DUAL_CHANNEL,CVA_RTK_SPI_FLASH_QUAD_CHANNEL) \n
 *            Select single mode, dual mode or quad mode of channel for SPI flash.
 * @param[in] spd SPI speed (CVA_RTK_SPI_FLASH_100MHz,CVA_RTK_SPI_FLASH_83MHz,CVA_RTK_SPI_FLASH_50MHz,CVA_RTK_SPI_FLASH_33MHz,CVA_RTK_SPI_FLASH_12MHz) \n
 *            Select 12Mhz,33Mhz,50Mhz,83Mhz or 100Mhz of speed for SPI flash.
 * @param[out] spicPattern SPI controller pattern 32 byte each (0x0 to 0xFF) \n
 *             The flash setting header for image.
 * @retval RT_ERR_OK Calibration success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_INPUT Invalid speed or channel.
 * @retval -RT_ERR_FAILED Calibration failed.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),
 *                cva_drv_spi_flash_sel_spi_speed_for_update(),cva_drv_spi_flash_sel_spi_channel_for_update(),
 *                cva_drv_spi_flash_sel_spi_channel_for_update(),cva_drv_spi_flash_set_spi_config_for_update(),
 *                cva_drv_spi_flash_calibration(),cva_drv_spi_flash_get_pattern().
 */
RtkApiRet cva_spiflash_cal(uint32 unit, uint8 ch, uint8 spd, SpicPattern_t *spicPattern)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Occupied by SDK */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit)); /* Calling cva_spiflash_cal() will lock the internal CPU to access the flash and will not unlock it.
                                                          After using this API, need to call cva_spiflash_update() and then reboot.
                                                          After rebooting, the new settings will be applied to the internal cpu & hardware.                            */
        /* Backup page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
        /* Set page to SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Select speed for update */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_sel_spi_speed_for_update(unit, spd));
        /* Select channel for update */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_sel_spi_channel_for_update(unit, ch));
        /* Update channel/speed */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_set_spi_config_for_update(unit));
        /* Calibration */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_calibration(unit));
        /* Get calibration pattern*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_pattern(unit, spicPattern));
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}
/**@}*/ /*SPI_CAL_ASIC*/
/**@}*/ /*SPI_CAL*/

/**
 * @addtogroup SPI_UPDATE Update Image to Flash
 * @{
 */
/**
 * @addtogroup SPI_UPDATE_ASIC Low Level Driver
 * @brief SPIC Low Level Driver
 * @{
 */

/**
 * @brief Update the binary file with the patch code and configuration into flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The filename of the input image.
 * @param[out] None
 * @retval RT_ERR_OK Update image to flash success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_NOT_ALLOWED The image is inconsistent with the SDK version.
 * @retval -RT_ERR_CHIP_NOT_SUPPORTED The IC version is inconsistent with the SDK version.
 * @retval -RT_ERR_INPUT Invalid image.
 * @retval -RT_ERR_FAILED  Update failed.
 * @retval -RT_ERR_IC_PART_NUM Failed to read otp data of IC part number .
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),
 *                cva_drv_spi_flash_update(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_update(uint32 unit, const char *fileName)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Occupied by SDK */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
        /* Backup page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
        /* Set page to SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Update image */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update(unit, fileName));
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}
/**@}*/ /*SPI_Update_ASIC*/
/**@}*/ /*SPI_Update*/

/**
 * @addtogroup SPI_ACCESS Read/Program/Erase Flash
 * @{
 */
/**
 * @addtogroup SPI_ACCESS_ASIC Low Level Driver
 * @brief SPIC Low Level Driver
 * @{
 */

/**
 * @brief Erases all data from flash memory, by setting every bits to '1'.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Flash chip erase success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_FAILED Chip erase failed.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),cva_reg_write(),
 *                cva_drv_spi_flash_erase(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_eraseall(uint32 unit)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Occupied by SDK */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
        /* Backup page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
        /* Set page to SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Erase all */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase(unit, (uint32)0u, (uint8)CVA_SPIC_CHIP_ERASE));
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}

/**
 * @brief Erase data from a block (64K bytes) specified by blockAddr, by setting every bit in that block to '1'.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] blockAddr Block address (0x0 to 0xFF) \n
 *            The block address of the SPI flash memory.
 * @param[out] None
 * @retval RT_ERR_OK Flash block erase success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_FAILED Block erase failed.
 * @retval -RT_ERR_INPUT Invalid block address.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),cva_reg_write(),
 *                cva_drv_spi_flash_erase(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_eraseblock(uint32 unit, uint32 blockAddr)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;
    uint32    addr   = (uint32)0u;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Check block address validity */
        if((uint32)0xFFu < blockAddr)
        {
            retVal = (int32)(-RT_ERR_INPUT);
            continue;
        }
        addr = blockAddr * (uint32)0x10000u;

        do
        {
            /* Occupied by SDK */
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
            /* Backup page */
            CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
            /* Set page to SPIC */
            CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
            /* Block erase */
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase(unit, addr, (uint8)CVA_SPIC_BLK64K_ERASE));
        } while(0u == 1u);
    } while(0u == 1u);
    /* Return to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}

/**
 * @brief Erase data from a sector (4K bytes) specified by sectorAddr, by setting every bit in that sector to be '1'.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sectorAddr Sector address (0x0 to 0xFFF) \n
 *            The sector address of the SPI flash memory.
 * @param[out] None
 * @retval RT_ERR_OK Flash sector erase success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_FAILED Sector erase failed.
 * @retval -RT_ERR_INPUT Invalid sector address.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),
 *                cva_drv_spi_flash_erase(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_erasesector(uint32 unit, uint32 sectorAddr)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;
    uint32    addr   = (uint32)0u;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Check sector address validity */
        if((uint32)0xFFFu < sectorAddr)
        {

            retVal = -RT_ERR_INPUT;
            continue;
        }
        addr = sectorAddr * (uint32)0x1000u;
        do
        {
            /* Occupied by SDK */
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
            /* Backup page */
            CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
            /* Set page to SPIC */
            CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
            /* Sector erase */
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase(unit, addr, (uint8)CVA_SPIC_SECTOR_ERASE));
        } while(0u == 1u);
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}

/**
 * @brief Program data into flash memory.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] address Flash address (0x0 to 0xFFFFFF) \n
 *            The address to program in SPI flash.
 * @param[in] len Data length (0x0 to 0xFFFFFF) \n
 *            The length to program in SPI flash.
 * @param[in] buf Data buffer (0x0 to 0xFF) \n
 *            The data to program in SPI flash.
 * @param[out] None
 * @retval RT_ERR_OK Program flash success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_FAILED Program flash failed.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),
 *                cva_drv_spi_flash_write(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_program(uint32 unit, uint32 address, uint32 len, const uint8 *buf)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Occupied by SDK */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
        /* Backup page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
        /* Set page to SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Program flash */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write(unit, address, buf, len));
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}

/**
 * @brief Read data from flash memory.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] address Flash address (0x0 to 0xFFFFFF) \n
 *            The address to read in SPI flash.
 * @param[in] len Data length (0x0 to 0xFFFFFF) \n
 *            The length to read in SPI flash.
 * @param[out] buf Data buffer \n
 *             The data read from SPI flash.
 * @retval RT_ERR_OK Read flash success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_FAILED Read flash failed.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_read(),cva_reg_page_write(),
 *                cva_drv_spi_flash_read(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_read(uint32 unit, uint32 address, uint32 len, uint8 *buf)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;

    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Occupied by SDK */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
        /* Backup page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
        /* Set page to SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Read flash */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, address, buf, len));
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}
/**@}*/ /*SPI_ACCESS_ASIC*/
/**@}*/ /*SPI_ACCESS*/

/**
 * @addtogroup SPI_READID  Read Flash ID
 * @{
 */
/**
 * @addtogroup SPI_READID_ASIC Low Level Driver
 * @brief SPIC Low Level Driver
 * @{
 */

/**
 * @brief Read the flash chip ID.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] flashId Flash ID (0x0 to 0xffffff) \n
 *             The JEDEC ID of SPI flash.
 * @retval RT_ERR_OK Read Flash ID success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_FAILED Read Flash ID failed.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(), cva_reg_page_read(), cva_reg_page_write(),
 *                cva_drv_spi_flash_read_id(),cva_drv_spi_flash_spic_unlock().
 */
RtkApiRet cva_spiflash_read_id(uint32 unit, uint32 *flashId)
{
    uint32    pgTmp  = 0u;
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retTmp = RT_ERR_OK;
    do
    {
        /* Flash driver always operates on master */
        if(CVA_UNIT != unit)
        {
            retVal = -RT_ERR_UNIT_ID;
            continue;
        }
        /* Occupied by SDK */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
        /* Backup page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_read(unit, &pgTmp));
        /* Set page to SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Read flash ID */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_id(unit, flashId));
    } while(0u == 1u);
    /* Back to the original page */
    retTmp = cva_reg_page_write(unit, pgTmp);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    /* Release Occupy flag */
    retTmp = cva_drv_spi_flash_spic_unlock(unit);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}
/**@}*/ /*SPI_READID_ASIC*/
/**@}*/ /*SPI_READID*/

/**
 * @addtogroup SPI_CAL_ETH Set Speed and Channel for calibration, in preparation for image header update via Ethernet
 * @{
 */
/**
 * @addtogroup SPI_CAL_ETH_ASIC Low Level Driver
 * @brief SPIC Low Level Driver
 * @{
 */

/**
 * @brief Set the speed and channel for SPI Master, and then calibrate to determine the correct settings between the SPI master and flash for flash memory access. \n
 *        These settings will not be stored; they are the calibration results collected for updating the image header via Ethernet. \n
 *        Users can manually replace the image header from 0x0 to 0x20 with the spicPattern obtained from this API.
 * @param[in] ch SPI channel (CVA_RTK_SPI_FLASH_SINGLE_CHANNEL,CVA_RTK_SPI_FLASH_DUAL_CHANNEL,CVA_RTK_SPI_FLASH_QUAD_CHANNEL) \n
 *            Select the number of channels for SPI Master to communicate with SPI flash - single, dual or quad channel.
 * @param[in] spd SPI speed (CVA_RTK_SPI_FLASH_100MHz,CVA_RTK_SPI_FLASH_83MHz,CVA_RTK_SPI_FLASH_50MHz,CVA_RTK_SPI_FLASH_33MHz,CVA_RTK_SPI_FLASH_12MHz) \n
 *            Select the flash clock speed - 12MHz, 33MHz, 50MHz, 83MHz, 100MHz.
 * @param[out] spicPattern SPI controller pattern 32 byte each (0x0 to 0xFF) \n
 *             The flash setting header for image.
 * @retval RT_ERR_OK Calibrate for eth success.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Occupy timeout or SPIC busy timeout.
 * @retval -RT_ERR_INPUT Invalid speed or channel.
 * @retval -RT_ERR_FAILED Calibration for eth failed.
 * @retval -RT_ERR_UNIT_ID Invalid unit ID.
 * @retval Others Please refer to cva_drv_spi_flash_spic_lock(),cva_reg_page_write(),
 *                cva_drv_spi_flash_sel_spi_speed_for_update(),cva_drv_spi_flash_sel_spi_channel_for_update(),
 *                cva_drv_spi_flash_sel_spi_channel_for_update(),cva_drv_spi_flash_set_spi_config_for_update(),
 *                cva_drv_spi_flash_calibration(),cva_drv_spi_flash_get_pattern(),cva_drv_spi_flash_speed_revert()
 *                cva_drv_spi_flash_delaychain_revert().
 */

RtkApiRet cva_spiflash_cal_for_ethernet(uint8 ch, uint8 spd, SpicPattern_t *spicPattern)
{
    RtkApiRet retVal        = RT_ERR_OK;
    RtkApiRet retTmp        = RT_ERR_OK;
    uint32    baudTmp       = 0u;
    uint32    fbaudTmp      = 0u;
    uint32    delaychainTmp = 0u;
    uint32    srcclockTmp   = 0u;
    uint32    unit          = CVA_UNIT;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_spic_lock(unit));
        /* calibration */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* backup delaychain, SPIC clock */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_speed_backup(unit, &baudTmp, &fbaudTmp, &srcclockTmp));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_delaychain_backup(unit, &delaychainTmp));

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_sel_and_set_spi_for_update(unit, ch, spd));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_calibration(unit));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_pattern(unit, spicPattern));

        /* revert delaychain, SPIC clock */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_speed_revert(unit, baudTmp, fbaudTmp, srcclockTmp));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_delaychain_revert(unit, delaychainTmp));

    } while(0u == 1u);

    retTmp = cva_drv_spi_flash_spic_unlock(unit);

    /*back to the original page.*/
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    retTmp = cva_reg_page_write(unit, CVA_SW_PAGE);
    if(RT_ERR_OK != retTmp)
    {
        retVal = retTmp;
    }
    return retVal;
}
/**@}*/ /*SPI_CAL_ETH_ASIC*/
/**@}*/ /*SPI_CAL_ETH*/

/**
 * @addtogroup SPI_UPDATE_ETH Update Image to Flash via Ethernet
 * @{
 */
/**
 * @addtogroup SPI_UPDATE_ETH_ASIC Low Level Driver
 * @brief SPIC Low Level Driver
 * @{
 */

/**
 * @brief Update the binary file with the patch code and configuration into flash via etherAccess (Ethernet).
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The filename of the input image.
 * @param[out] None
 * @retval RT_ERR_OK Update image to flash via eth success.
 * @retval Others Please refer to cva_drv_spi_flash_update_for_ethernet().
 */
RtkApiRet cva_spiflash_update_for_ethernet(const char *fileName)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Update image via ethernet access*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_for_ethernet(fileName));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*SPI_UPDATE_ETH_ASIC*/
/**@}*/ /*SPI_UPDATE_ETH*/

/**@}*/ /*SPI Flash*/
