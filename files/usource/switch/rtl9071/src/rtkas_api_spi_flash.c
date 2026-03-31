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
#include "rtkas_common.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_spi_flash.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_spi_flash.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_spi_flash.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_spi_flash.h"
#endif

#include "rtkas_api_spi_flash.h"

/**
 * @defgroup RTK_MODULE_SPI_FLASH SPI Flash
 * @brief SPI FLASH API. This component is used to control SPIC for accessing flash memory,
 * including reading the flash ID, performing calibration, operating the flash and updating the switch image.
 * @{
 */

/**
 * @defgroup SPI_INIT SPI Software initialization
 * @brief SPIC initialization
 * @{
 */

/**
 * @brief Initialize the software settings to access SPI flash. \n
 *        After the switch boots up, only need to execute once before accessing flash.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Initialize success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_spiflash_init(),dva_spiflash_init(),cvb_spiflash_init(),cva_spiflash_init().
 */
RtkApiRet rtk_spiflash_init(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_spiflash_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_spiflash_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_spiflash_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_spiflash_init(unitChip.unit);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}
/**@}*/ /*SPI_INIT*/

/**
 * @defgroup SPI_CAL Set Speed and Channel for calibration
 * @brief Calibrate the system delay and other settings for desired channel and speed.
 * @{
 */

/**
 * @brief Set the speed and channel for SPI Master, and then calibrate to determine the correct settings between the SPI master and flash for flash memory access. \n
 *        rtk_spiflash_update() will store the settings into the image header. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *spiCal Flash Calibration parameter (N/A) \n
 *            Contains channel, speed and calibration pattern.
 * @retval RT_ERR_OK Calibration success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_cal(),dva_spiflash_cal(),cvb_spiflash_cal(),cva_spiflash_cal().
 */
RtkApiRet rtk_spiflash_cal(UnitChip_t unitChip, SpiCal_t *spiCal)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiCal)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_cal(unitChip.unit, spiCal->ch, spiCal->spd, spiCal->spicPattern);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_cal(unitChip.unit, spiCal->ch, spiCal->spd, spiCal->spicPattern);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_cal(unitChip.unit, spiCal->ch, spiCal->spd, spiCal->spicPattern);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_cal(unitChip.unit, spiCal->ch, spiCal->spd, spiCal->spicPattern);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SPI_CAL*/

/**
 * @defgroup SPI_UPDATE Update Image to Flash
 * @brief Update the firmware and configuration image.
 * @{
 */
#ifndef RTL906X_CHIP_VERIFY
/**
 * @brief Update the binary file with the firmware code and configuration into flash. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *spiUpdate Flash Update parameter (N/A) \n
 *            Pointer to an spiUpdate struct containing the filename of the patch code.
 * @retval RT_ERR_OK Update image to flash success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_update(),dva_spiflash_update(),cvb_spiflash_update(),cva_spiflash_update().
 */
RtkApiRet rtk_spiflash_update(UnitChip_t unitChip, SpiUpdate_t *spiUpdate)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiUpdate)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_update(unitChip.unit, spiUpdate->fileName);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_update(unitChip.unit, spiUpdate->fileName);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_update(unitChip.unit, spiUpdate->fileName);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_update(unitChip.unit, spiUpdate->fileName);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
#endif  /* RTL906X_CHIP_VERIFY */
/**@}*/ /*SPI_Update*/

/**
 * @defgroup SPI_ACCESS Read/Program/Erase Flash
 * @brief Basic read, write and erase function for flash memory.
 * @{
 */

/**
 * @brief Erase all data from flash memory, by setting every bit to "1". \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Flash chip erase success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_spiflash_eraseall(),dva_spiflash_eraseall(),cvb_spiflash_eraseall(),cva_spiflash_eraseall().
 */
RtkApiRet rtk_spiflash_eraseall(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_spiflash_eraseall(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_spiflash_eraseall(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_spiflash_eraseall(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_spiflash_eraseall(unitChip.unit);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief Erase data from a block (64K bytes) specified by blockAddr, by setting every bit in that block to '1'. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *spiEraseBlock Block Erase parameter(N/A) \n
 *            Contains block address of the flash memory.
 * @retval RT_ERR_OK Flash block erase success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_update(),dva_spiflash_update(),cvb_spiflash_eraseblock(),cva_spiflash_eraseblock().
 */
RtkApiRet rtk_spiflash_eraseblock(UnitChip_t unitChip, SpiEraseBlock_t *spiEraseBlock)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiEraseBlock)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_eraseblock(unitChip.unit, spiEraseBlock->blockAddr);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_eraseblock(unitChip.unit, spiEraseBlock->blockAddr);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_eraseblock(unitChip.unit, spiEraseBlock->blockAddr);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_eraseblock(unitChip.unit, spiEraseBlock->blockAddr);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Erase data from a sector (4K bytes) specified by sectorAddr, by setting every bit in that sector to be '1'. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *spiEraseSector Sector Erase parameter(N/A) \n
 *            Contains sector address of the flash memory
 * @retval RT_ERR_OK Flash sector erase success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_update(),dva_spiflash_update(),cvb_spiflash_erasesector(),cva_spiflash_erasesector().
 */
RtkApiRet rtk_spiflash_erasesector(UnitChip_t unitChip, SpiEraseSector_t *spiEraseSector)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiEraseSector)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_erasesector(unitChip.unit, spiEraseSector->sectorAddr);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_erasesector(unitChip.unit, spiEraseSector->sectorAddr);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_erasesector(unitChip.unit, spiEraseSector->sectorAddr);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_erasesector(unitChip.unit, spiEraseSector->sectorAddr);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Program data into flash memory. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *spiProgram spiProgram parameter (N/A) \n
 *            Contains address, length and data buffer. This data buffer holds data to be programmed to flash memory.
 * @retval RT_ERR_OK Program flash success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_program(),dva_spiflash_program(),cvb_spiflash_program(),cva_spiflash_program().
 */
RtkApiRet rtk_spiflash_program(UnitChip_t unitChip, SpiProgram_t *spiProgram)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiProgram)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_program(unitChip.unit, spiProgram->address, spiProgram->len, spiProgram->buf);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_program(unitChip.unit, spiProgram->address, spiProgram->len, spiProgram->buf);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_program(unitChip.unit, spiProgram->address, spiProgram->len, spiProgram->buf);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_program(unitChip.unit, spiProgram->address, spiProgram->len, spiProgram->buf);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Read data from flash memory. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *spiRead spiRead parameter (N/A) \n
 *            Contains address, length and data buffer. This data buffer holds data read from flash memmory.
 * @retval RT_ERR_OK Read Flash success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_read(),dva_spiflash_read(),cvb_spiflash_read(),cva_spiflash_read().
 */
RtkApiRet rtk_spiflash_read(UnitChip_t unitChip, SpiRead_t *spiRead)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiRead)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_read(unitChip.unit, spiRead->address, spiRead->len, spiRead->buf);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_read(unitChip.unit, spiRead->address, spiRead->len, spiRead->buf);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_read(unitChip.unit, spiRead->address, spiRead->len, spiRead->buf);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_read(unitChip.unit, spiRead->address, spiRead->len, spiRead->buf);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SPI_ACCESS*/

/**
 * @defgroup SPI_READID  Read Flash ID
 * @brief Read flash JEDEC ID.
 * @{
 */

/**
 * @brief Read the flash chip ID. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *spiRead spiRead parameter (N/A) \n
 *            Contains address, length and data buffer. This data buffer holds data read from flash memmory.
 * @retval RT_ERR_OK Read Flash ID success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_read_id(),dva_spiflash_read_id(),cvb_spiflash_read_id(),cva_spiflash_read_id().
 */
RtkApiRet rtk_spiflash_read_id(UnitChip_t unitChip, SpiReadId_t *spiReadId)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiReadId)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_read_id(unitChip.unit, spiReadId->flashId);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_read_id(unitChip.unit, spiReadId->flashId);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_read_id(unitChip.unit, spiReadId->flashId);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_read_id(unitChip.unit, spiReadId->flashId);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SPI_READID*/

/**
 * @defgroup SPI_CAL_ETH Set Speed and Channel for calibration, in preparation for image header update via Ethernet
 * @brief Calibrate the system delay based on the desired channel and speed for image header update via Ethernet.
 * @{
 */

/**
 * @brief Set the speed and channel for SPI Master, and then calibrate to determine the correct settings between the SPI master and flash for flash memory access. \n
 *        These settings will not be stored; they are the calibration results collected for updating the image header via Ethernet. \n
 *        Users can manually replace the image header from 0x0 to 0x20 with the spicPattern obtained from this API. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *spiCal Calibration parameter (N/A) \n
 *            Contains channel, speed and calibration pattern.
 * @retval RT_ERR_OK Calibrate for eth success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_cal_for_ethernet(),dva_spiflash_cal_for_ethernet(),cvb_spiflash_cal_for_ethernet(),cva_spiflash_cal_for_ethernet().
 */
RtkApiRet rtk_spiflash_cal_for_ethernet(UnitChip_t unitChip, SpiCalEth_t *spiCalEth)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiCalEth)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_cal_for_ethernet(spiCalEth->ch, spiCalEth->spd, spiCalEth->spicPattern);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_cal_for_ethernet(spiCalEth->ch, spiCalEth->spd, spiCalEth->spicPattern);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_cal_for_ethernet(spiCalEth->ch, spiCalEth->spd, spiCalEth->spicPattern);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_cal_for_ethernet(spiCalEth->ch, spiCalEth->spd, spiCalEth->spicPattern);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SPI_CAL_ETH*/

/**
 * @defgroup SPI_UPDATE_ETH Update Image to Flash via Ethernet
 * @brief Update the firmware and configuration image via Ethernet.
 * @{
 */

/**
 * @brief Update the binary file with the firmware code and configuration into flash via etherAccess (Ethernet). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *spiUpdate Flash Update parameter(N/A) \n
 *            Pointer to an spiUpdate struct containing the filename of the patch code.
 * @retval RT_ERR_OK Update image to flash via Ethernet success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_spiflash_update(),dva_spiflash_update(),cvb_spiflash_update_for_ethernet(),cva_spiflash_update_for_ethernet().
 */
RtkApiRet rtk_spiflash_update_for_ethernet(UnitChip_t unitChip, SpiUpdateEth_t *spiUpdateEth)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == spiUpdateEth)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_spiflash_update_for_ethernet(spiUpdateEth->fileName);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_spiflash_update_for_ethernet(spiUpdateEth->fileName);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_spiflash_update_for_ethernet(spiUpdateEth->fileName);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_spiflash_update_for_ethernet(spiUpdateEth->fileName);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*DVA_SPI_UPDATE_ETH*/

/**@}*/ /*SPI Flash*/
