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

#ifndef RTKAS_API_SPI_FLASH_H__
#define RTKAS_API_SPI_FLASH_H__

/*flash channel*/
#define RTK_SPI_FLASH_SINGLE_CHANNEL (1u)
#define RTK_SPI_FLASH_DUAL_CHANNEL   (2u)
#define RTK_SPI_FLASH_QUAD_CHANNEL   (4u)

/*flash speed*/
#define RTK_SPI_FLASH_125MHz (125u)
#define RTK_SPI_FLASH_100MHz (100u)
#define RTK_SPI_FLASH_83MHz  (83u)
#define RTK_SPI_FLASH_75MHz  (75u)
#define RTK_SPI_FLASH_66MHz  (66u)
#define RTK_SPI_FLASH_50MHz  (50u)
#define RTK_SPI_FLASH_33MHz  (33u)
#define RTK_SPI_FLASH_12MHz  (12u)

/*
    SPI FLASH PATTERN Table
*/
typedef struct
{
    uint8 pattern[32]; /**< SPI controller pattern (each byte range 0 to 0xFF) */
} SpicPattern_t;

/**
 * @brief This struct is used for calibration, containing
 * the channel and speed as inputs, and spicPattern as output.
 */
typedef struct
{
    uint8          ch;          /**< [in] SPI controller channel (1/2/4) */
    uint8          spd;         /**< [in] SPI controller speed (12/33/50/83/100/125) */
    SpicPattern_t *spicPattern; /**< [out] SPI controller pattern (each byte range 0 to 0xFF) */
} SpiCal_t;

/**
 * @brief This struct is used for calibration for EtherAccess, containing
 * the channel and speed as inputs, and spicPattern as output.
 */
typedef struct
{
    uint8          ch;          /**< [in] SPI controller channel (1/2/4) */
    uint8          spd;         /**< [in] SPI controller speed (12/33/50/83/100/125) */
    SpicPattern_t *spicPattern; /**< [out] SPI controller pattern (each byte range 0 to 0xFF) */
} SpiCalEth_t;

/**
 * @brief This struct is used for updating the image, containing
 * the filename as input.
 */
typedef struct
{
    const char *fileName; /**< [in] Update file name (each byte range 0 to 0xFF) */
} SpiUpdate_t;

/**
 * @brief This struct is used for updating the image via eth, containing
 * the filename as input.
 */
typedef struct
{
    const char *fileName; /**< [in] Update file name (each byte range 0 to 0xFF) */
} SpiUpdateEth_t;

/**
 * @brief This struct is used for erasing the flash block, containing
 * the block address as input.
 */
typedef struct
{
    uint32 blockAddr; /**< [in] Erased Block Address (0x0 to 0xFF) */
} SpiEraseBlock_t;

/**
 * @brief This struct is used for erasing the flash sector, containing
 * the sector address as input.
 */
typedef struct
{
    uint32 sectorAddr; /**< [in] Erased Sector Address (0x0 to 0xFFF) */
} SpiEraseSector_t;

/**
 * @brief This struct is used for programming the flash, containing
 * the address, length and buf as input.
 */
typedef struct
{
    uint32       address; /**< [in] Flash address (0x0 to 0xFFFFFF) */
    uint32       len;     /**< [in] Data length (0x0 to 0xFFFFFF) */
    const uint8 *buf;     /**< [in] Data buffer (0x0 to 0xFF) */
} SpiProgram_t;

/**
 * @brief This struct is used for reading the flash, containing
 * the address, length as input, and buf as output.
 */
typedef struct
{
    uint32 address; /**< [in] Flash address (0x0 to 0xFFFFFF) */
    uint32 len;     /**< [in] Data length (0x0 to 0xFFFFFF) */
    uint8 *buf;     /**< [out] Data buffer (0x0 to 0xFF) */
} SpiRead_t;

/**
 * @brief This struct is used for reading the flash ID, containing
 * the flashId as output.
 */
typedef struct
{
    uint32 *flashId; /**< [out]Flash ID (0x0 to 0xffffff) */
} SpiReadId_t;

EXTERN_API RtkApiRet rtk_spiflash_init(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_spiflash_cal(UnitChip_t unitChip, SpiCal_t *spiCal);
EXTERN_API RtkApiRet rtk_spiflash_update(UnitChip_t unitChip, SpiUpdate_t *spiUpdate);
EXTERN_API RtkApiRet rtk_spiflash_eraseall(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_spiflash_eraseblock(UnitChip_t unitChip, SpiEraseBlock_t *spiEraseBlock);
EXTERN_API RtkApiRet rtk_spiflash_erasesector(UnitChip_t unitChip, SpiEraseSector_t *spiEraseSector);
EXTERN_API RtkApiRet rtk_spiflash_program(UnitChip_t unitChip, SpiProgram_t *spiProgram);
EXTERN_API RtkApiRet rtk_spiflash_read(UnitChip_t unitChip, SpiRead_t *spiRead);
EXTERN_API RtkApiRet rtk_spiflash_read_id(UnitChip_t unitChip, SpiReadId_t *spiReadId);
EXTERN_API RtkApiRet rtk_spiflash_cal_for_ethernet(UnitChip_t unitChip, SpiCalEth_t *spiCalEth);
EXTERN_API RtkApiRet rtk_spiflash_update_for_ethernet(UnitChip_t unitChip, SpiUpdateEth_t *spiUpdateEth);

#endif /* RTKAS_API_SPI_FLASH_H__ */
