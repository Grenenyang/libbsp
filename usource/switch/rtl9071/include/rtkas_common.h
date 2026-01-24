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

#ifndef RTKAS_COMMON_H__
#define RTKAS_COMMON_H__

#include "rtkas_common_types.h"

extern uint32 g_dbgMsk;

typedef uint32 RtkPort;
typedef uint32 RtkPmap;
typedef uint32 RtkEnable;
typedef uint32 RtkVlan; /**< The data type of <CHIPID>_VLAN ID (0 to 0xFFF) */
typedef struct
{
    uint32 unit;
    uint32 chip;
} UnitChip_t;

#define CHIP_907XD_V1 (0u)
#define CHIP_907XD_VA (1u)
#define CHIP_907XC_VB (2u)
#define CHIP_907XC_VA (3u)

#define DISABLED       (0u) /* Disable function */
#define ENABLED        (1u) /* Enable function */
#define RTK_ENABLE_END (2u)

#define IS_VALID_CHIPID(chip) \
    ((CHIP_907XD_V1 == chip) ? true : ((CHIP_907XD_VA == chip) ? true : ((CHIP_907XC_VA == chip) ? true : ((CHIP_907XC_VB == chip) ? true : false))))

EXTERN_API RtkApiRet reg_direct_read(uint32 regAddr, uint32 *value);
EXTERN_API RtkApiRet reg_direct_write(uint32 regAddr, uint32 value);
EXTERN_API RtkApiRet reg_direct_write_bytes(uint32 regAddr, uint32 value);
EXTERN_API RtkApiRet reg_direct_read_burst(uint32 regAddr, uint32 *value, uint32 length);
EXTERN_API RtkApiRet reg_read(UnitChip_t unitChip, uint32 regAddr, uint32 *value);
EXTERN_API RtkApiRet reg_write(UnitChip_t unitChip, uint32 regAddr, uint32 value);
EXTERN_API RtkApiRet reg_field_get(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value, uint32 data);
EXTERN_API RtkApiRet reg_field_set(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 value, uint32 *data);
EXTERN_API RtkApiRet reg_field_read(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value);
EXTERN_API RtkApiRet reg_field_write(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 value);
EXTERN_API RtkApiRet reg_bit_get(UnitChip_t unitChip, uint32 reg, uint32 bit, uint32 *value);
EXTERN_API RtkApiRet reg_bit_set(UnitChip_t unitChip, uint32 reg, uint32 bit, uint32 value);
EXTERN_API RtkApiRet reg_page_read(UnitChip_t unitChip, uint32 *page);
EXTERN_API RtkApiRet reg_page_write(UnitChip_t unitChip, uint32 page);
EXTERN_API RtkApiRet asicdrv_hw_indirect_set(UnitChip_t unitChip, uint32 regAddr, uint32 regVal);
EXTERN_API RtkApiRet asicdrv_hw_indirect_get(UnitChip_t unitChip, uint32 regAddr, uint32 *regVal);
EXTERN_API RtkApiRet drv_memset(void *str, int32 val, size_t size);
EXTERN_API RtkApiRet drv_memcpy(void *dst, const void *src, size_t size);
EXTERN_API RtkApiRet drv_endian_change(uint8 *src, uint8 *dst, uint32 len);
EXTERN_API RtkApiRet regif_eth_partial_indir_write(uint32 regAddr, uint32 mask, uint32 value);
#endif /* RTKAS_API_SYSTEM_H__ */
