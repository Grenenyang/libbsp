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

#ifndef DV1_ASICDRV_MEM_CTRL_H_
#define DV1_ASICDRV_MEM_CTRL_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_v1_api_ext.h"

/******************************************************************************
 *  Defines
 ******************************************************************************/
/* v1.5.0 */
#define DV1_DRV_INDIRECT_REG_FIELD_FW_VER (0x10500000u)

#define DV1_INDIRECT_ACCESS_CHANNEL0_COMMAND DV1_INTERNAL_CPU_VARIABLE47
#define DV1_INDIRECT_ACCESS_CHANNEL0_ADDRESS DV1_INTERNAL_CPU_VARIABLE48
#define DV1_INDIRECT_ACCESS_CHANNEL0_DATA    DV1_INTERNAL_CPU_VARIABLE49

#define DV1_INDIRECT_ACCESS_CHANNEL1_COMMAND DV1_INTERNAL_CPU_VARIABLE16
#define DV1_INDIRECT_ACCESS_CHANNEL1_ADDRESS DV1_INTERNAL_CPU_VARIABLE17
#define DV1_INDIRECT_ACCESS_CHANNEL1_DATA    DV1_INTERNAL_CPU_VARIABLE18

#define DV1_INDIRECT_ACCESS_CHANNEL0_DATA1 DV1_HW_MISC_DMY_RESIGSER_7
#define DV1_INDIRECT_ACCESS_CHANNEL1_DATA1 DV1_ALE_DMY0

#define DV1_INDIRECT_ACCESS_TARGET_SRAM     (0u)
#define DV1_INDIRECT_ACCESS_TARGET_OTP      (1u)
#define DV1_INDIRECT_ACCESS_TARGET_FLASH    (2u)
#define DV1_INDIRECT_ACCESS_TARGET_SERDES   (3u)
#define DV1_INDIRECT_ACCESS_TARGET_PCIE     (4u)
#define DV1_INDIRECT_ACCESS_TARGET_MACPHY_0 (5u)
#define DV1_INDIRECT_ACCESS_TARGET_MACPHY_1 (6u)
#define DV1_INDIRECT_ACCESS_TARGET_REGISTER (7u)

#define DV1_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB (31u)
#define DV1_INDIRECT_ACCESS_COMMAND_ERROR_LSB   (30u)
#define DV1_INDIRECT_ACCESS_COMMAND_READ_LSB    (29u)
#define DV1_INDIRECT_ACCESS_COMMAND_WRITE_LSB   (28u)
#define DV1_INDIRECT_ACCESS_COMMAND_TARGET_LSB  (24u)

#define DV1_INDIDRCT_ACCESS_ADDRESS_SERDES_PORT_LSB (27u)
#define DV1_INDIDRCT_ACCESS_ADDRESS_TYPE_PORT_LSB   (26u)
#define DV1_INDIDRCT_ACCESS_ADDRESS_PAGE_PORT_LSB   (16u)

#define DV1_INDIRECT_ACCESS_REG_INFO_LSB_LSB (0u)
#define DV1_INDIRECT_ACCESS_REG_INFO_LSB_LEN (5u)
#define DV1_INDIRECT_ACCESS_REG_INFO_LEN_LSB (5u)
#define DV1_INDIRECT_ACCESS_REG_INFO_LEN_LEN (6u)

/******************************************************************************
 *  Structure
 ******************************************************************************/

/******************************************************************************
 *  Global Variables
 ******************************************************************************/
extern uint32 dv1_g_rtkTimerVirtualChannel;

/******************************************************************************
 *  Function Prototype
 ******************************************************************************/
EXTERN_API RtkApiRet dv1_drv_hw_indirect_get(uint32 unit, uint32 regAddr, uint32 *regVal);
EXTERN_API RtkApiRet dv1_drv_hw_indirect_set(uint32 unit, uint32 regAddr, uint32 regVal);

EXTERN_API RtkApiRet dv1_drv_indirect_channel_set(uint32 channel);

#ifdef RTK_INTERNAL_DEV
EXTERN_API RtkApiRet dv1_drv_indirect_reg_field_get_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_reg_field_set_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
#else
EXTERN_API RtkApiRet dv1_drv_indirect_reg_field_get(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_reg_field_set(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
#endif /* RTK_INTERNAL_DEV */
EXTERN_API RtkApiRet dv1_drv_indirect_sram_get(uint32 unit, uint32 regAddr, uint32 *regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_sram_set(uint32 unit, uint32 regAddr, uint32 regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_sram_1byte_get(uint32 unit, uint32 regAddr, uint8 *regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_sram_1byte_set(uint32 unit, uint32 regAddr, uint8 regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_sram_2bytes_get(uint32 unit, uint32 regAddr, uint16 *regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_sram_2bytes_set(uint32 unit, uint32 regAddr, uint16 regVal);

EXTERN_API RtkApiRet dv1_drv_indirect_serdes_get(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 *regVal);
EXTERN_API RtkApiRet dv1_drv_indirect_serdes_set(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 regVal);

#endif /* DV1_ASICDRV_MEM_CTRL_H_ */
