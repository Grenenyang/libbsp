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

#ifndef DV1_RTK_API_SPI_FLASH_H__
#define DV1_RTK_API_SPI_FLASH_H__

#include "rtl907xd_v1_api.h"
#include "rtkas_api_spi_flash.h"

EXTERN_API RtkApiRet dv1_spiflash_init(uint32 unit);

EXTERN_API RtkApiRet dv1_spiflash_cal(uint32 unit, uint8 ch, uint8 spd, SpicPattern_t *spicPattern);

EXTERN_API RtkApiRet dv1_spiflash_update(uint32 unit, const char *fileName);

EXTERN_API RtkApiRet dv1_spiflash_eraseall(uint32 unit);

EXTERN_API RtkApiRet dv1_spiflash_eraseblock(uint32 unit, uint32 blockAddr); /*64KByte*/

EXTERN_API RtkApiRet dv1_spiflash_erasesector(uint32 unit, uint32 sectorAddr); /*4KByte*/

EXTERN_API RtkApiRet dv1_spiflash_program(uint32 unit, uint32 address, uint32 len, const uint8 *buf);

EXTERN_API RtkApiRet dv1_spiflash_read(uint32 unit, uint32 address, uint32 len, uint8 *buf);

EXTERN_API RtkApiRet dv1_spiflash_read_id(uint32 unit, uint32 *flashId);

EXTERN_API RtkApiRet dv1_spiflash_update_for_ethernet(const char *fileName);

EXTERN_API RtkApiRet dv1_spiflash_cal_for_ethernet(uint8 ch, uint8 spd, SpicPattern_t *spicPattern);

#endif /* DV1_RTK_API_SPI_FLASH_H__ */
