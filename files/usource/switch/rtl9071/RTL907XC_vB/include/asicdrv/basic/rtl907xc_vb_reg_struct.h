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

#ifndef CVB_REG_STRUCT_H__
#define CVB_REG_STRUCT_H__

#include "rtkas_std.h"
#include "rtkas_common_types.h"
#include "rtl907xc_vb_reg_define.h"
#ifdef RTK_MODULE_STACKING
#include "rtl907xc_vb_asicdrv_stacking.h"
#endif

#define CVB_FIELD_TYPE_RO   (0u) /* read only */
#define CVB_FIELD_TYPE_RW   (1u) /* read, write */
#define CVB_FIELD_TYPE_RWS  (2u) /* read, write side effect */
#define CVB_FIELD_TYPE_RS   (3u) /* read side effect */
#define CVB_FIELD_TYPE_RSW  (4u) /* read side effect, write */
#define CVB_FIELD_TYPE_RC   (5u) /* read to clear */
#define CVB_FIELD_TYPE_RW1C (6u) /* read, write 1 to clear */
#define CVB_FIELD_TYPE_WO   (7u) /* write only */
#define CVB_FIELD_TYPE_WS   (8u) /* write side effect */
#define CVB_FIELD_TYPE_NONE (9u) /* no use */
#define CVB_FIELD_TYPE_END  (10u)

#define CVB_REG_DATA_LEN  (32u)
#define CVB_REG_DATA_MASK (0xFFFFFFFFu)
#define CVB_REG_PAGE_ADDR (0xFFFF01u)

#ifdef RTK_MODULE_STACKING
#define CVB_UNIT cvb_g_stackingMaster
#else
#define CVB_UNIT (1u)
#endif
EXTERN_API bool      cvb_reg_check_unit(uint32 unit);
EXTERN_API RtkApiRet cvb_reg_read(uint32 unit, uint32 regAddr, uint32 *value);
EXTERN_API RtkApiRet cvb_reg_write(uint32 unit, uint32 regAddr, uint32 value);
EXTERN_API RtkApiRet cvb_reg_write_array(uint32 unit, uint32 regAddr[], uint32 value[], uint32 num);
EXTERN_API RtkApiRet cvb_reg_field_get(uint32 regAddr, uint32 lsp, uint32 len, uint32 *value, uint32 data);
EXTERN_API RtkApiRet cvb_reg_field_set(uint32 regAddr, uint32 lsp, uint32 len, uint32 value, uint32 *data);
#ifdef RTK_INTERNAL_DEV
EXTERN_API RtkApiRet cvb_reg_field_read_internal(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value);
EXTERN_API RtkApiRet cvb_reg_field_write_internal(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 value);
#else
EXTERN_API RtkApiRet cvb_reg_field_read(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value);
EXTERN_API RtkApiRet cvb_reg_field_write(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 value);
#endif /* RTK_INTERNAL_DEV */
EXTERN_API RtkApiRet cvb_reg_bit_get(uint32 unit, uint32 reg, uint32 bit, uint32 *value);
EXTERN_API RtkApiRet cvb_reg_bit_set(uint32 unit, uint32 reg, uint32 bit, uint32 value);
EXTERN_API RtkApiRet cvb_reg_page_read(uint32 unit, uint32 *page);
EXTERN_API RtkApiRet cvb_reg_page_write(uint32 unit, uint32 page);
EXTERN_API RtkApiRet cvb_reg_read_burst(uint32 unit, uint32 regAddr, uint32 *value, uint32 length);
#endif /* CVB_REG_STRUCT_H__ */
