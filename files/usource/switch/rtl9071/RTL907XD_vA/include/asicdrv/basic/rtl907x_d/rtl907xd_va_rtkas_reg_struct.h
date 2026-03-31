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

#ifndef DVA_RTKAS_REG_STRUCT_H_
#define DVA_RTKAS_REG_STRUCT_H_

#include "rtkas_common_types.h"
#include "rtl907xd_va_rtkas_reg_define.h"

#define DVA_FIELD_TYPE_RO   (0u) /* read only */
#define DVA_FIELD_TYPE_RW   (1u) /* read, write */
#define DVA_FIELD_TYPE_RWS  (2u) /* read, write side effect */
#define DVA_FIELD_TYPE_RS   (3u) /* read side effect */
#define DVA_FIELD_TYPE_RSW  (4u) /* read side effect, write */
#define DVA_FIELD_TYPE_RC   (5u) /* read to clear */
#define DVA_FIELD_TYPE_RW1C (6u) /* read, write 1 to clear */
#define DVA_FIELD_TYPE_WO   (7u) /* write only */
#define DVA_FIELD_TYPE_WS   (8u) /* write side effect */
#define DVA_FIELD_TYPE_NONE (9u) /* no use */
#define DVA_FIELD_TYPE_END  (10u)

#define DVA_REG_DATA_LEN  (32u)
#define DVA_REG_DATA_MASK (0xFFFFFFFFu)

EXTERN_API int32 dva_reg_read(uint32 unit, uint32 regAddr, uint32 *value);
EXTERN_API int32 dva_reg_write(uint32 unit, uint32 regAddr, uint32 value);
EXTERN_API int32 dva_reg_field_get(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value, uint32 data);
EXTERN_API int32 dva_reg_field_set(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 value, uint32 *data);
EXTERN_API int32 dva_reg_field_read(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value);
EXTERN_API int32 dva_reg_field_write(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 value);

#endif /* DVA_RTKAS_REG_STRUCT_H_ */
