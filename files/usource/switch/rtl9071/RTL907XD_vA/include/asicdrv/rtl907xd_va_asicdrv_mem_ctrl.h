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

#ifndef DVA_ASICDRV_MEM_CTRL_H_
#define DVA_ASICDRV_MEM_CTRL_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api_ext.h"

/******************************************************************************
 *  Defines
 ******************************************************************************/
#define DVA_INDIRECT_ACCESS_PORT_BASE_ADDRESS (0x4A800000u)
#define DVA_INDIRECT_ACCESS_PORT_CHK_MSK      (0xF800u)

#define DVA_INDIRECT_ACCESS_CHANNEL0_COMMAND DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE
#define DVA_INDIRECT_ACCESS_CHANNEL0_ADDRESS DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_ADDRESS
#define DVA_INDIRECT_ACCESS_CHANNEL0_DATA    DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_DATA

#define DVA_INDIRECT_ACCESS_CHANNEL1_COMMAND DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_OPCODE
#define DVA_INDIRECT_ACCESS_CHANNEL1_ADDRESS DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_ADDRESS
#define DVA_INDIRECT_ACCESS_CHANNEL1_DATA    DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_DATA

#define DVA_INDIRECT_ACCESS_CHANNEL0_DATA1 DVA_HW_MISC_DMY7
#define DVA_INDIRECT_ACCESS_CHANNEL1_DATA1 DVA_ALE_DMY0

#define DVA_INDIRECT_ACCESS_TARGET_SRAM     (0u)
#define DVA_INDIRECT_ACCESS_TARGET_OTP      (1u)
#define DVA_INDIRECT_ACCESS_TARGET_FLASH    (2u)
#define DVA_INDIRECT_ACCESS_TARGET_SERDES   (3u)
#define DVA_INDIRECT_ACCESS_TARGET_PCIE     (4u)
#define DVA_INDIRECT_ACCESS_TARGET_MACPHY_0 (5u)
#define DVA_INDIRECT_ACCESS_TARGET_MACPHY_1 (6u)
#define DVA_INDIRECT_ACCESS_TARGET_PLATFORM (7u)
#define DVA_INDIRECT_ACCESS_TARGET_REG      (8u)
#define DVA_INDIRECT_ACCESS_TARGET_PORT     (9u)

#define DVA_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB (31u)
#define DVA_INDIRECT_ACCESS_COMMAND_ERROR_LSB   (30u)
#define DVA_INDIRECT_ACCESS_COMMAND_READ_LSB    (29u)
#define DVA_INDIRECT_ACCESS_COMMAND_WRITE_LSB   (28u)
#define DVA_INDIRECT_ACCESS_COMMAND_TARGET_LSB  (24u)

#define DVA_INDIDRCT_ACCESS_ADDRESS_SERDES_PORT_LSB (27u)
#define DVA_INDIDRCT_ACCESS_ADDRESS_TYPE_PORT_LSB   (26u)
#define DVA_INDIDRCT_ACCESS_ADDRESS_PAGE_PORT_LSB   (16u)

#define DVA_INDIRECT_ACCESS_REG_INFO_LSB_LSB  (0u)
#define DVA_INDIRECT_ACCESS_REG_INFO_LSB_LEN  (5u)
#define DVA_INDIRECT_ACCESS_REG_INFO_LEN_LSB  (5u)
#define DVA_INDIRECT_ACCESS_REG_INFO_LEN_LEN  (6u)
#define DVA_INDIRECT_ACCESS_REG_INFO_PORT_LSB (11u)
#define DVA_INDIRECT_ACCESS_REG_INFO_PORT_LEN (4u)

/******************************************************************************
 *  Structure
 ******************************************************************************/

/******************************************************************************
 *  Global Variables
 ******************************************************************************/
extern uint32 dva_g_rtkTimerVirtualChannel;

/******************************************************************************
 *  Function Prototype
 ******************************************************************************/
EXTERN_API RtkApiRet dva_drv_hw_indirect_get(uint32 unit, uint32 regAddr, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_hw_indirect_set(uint32 unit, uint32 regAddr, uint32 regVal);

EXTERN_API RtkApiRet dva_drv_indirect_channel_set(uint32 channel);

#ifdef RTK_INTERNAL_DEV
EXTERN_API RtkApiRet dva_drv_indirect_reg_field_get_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_reg_field_set_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
EXTERN_API RtkApiRet dva_drv_direct_indirect_get_reg_field_internal(uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_direct_indirect_set_reg_field_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
EXTERN_API RtkApiRet dva_drv_indirect_target_get_reg_field_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_target_set_reg_field_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
#else
EXTERN_API RtkApiRet dva_drv_indirect_reg_field_get(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_target_get_reg_field(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_reg_field_set(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
EXTERN_API RtkApiRet dva_drv_direct_indirect_get_reg_field(uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_direct_indirect_set_reg_field(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
EXTERN_API RtkApiRet dva_drv_indirect_target_set_reg_field(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal);
#endif /* RTK_INTERNAL_DEV */

EXTERN_API RtkApiRet dva_drv_indirect_sram_get(uint32 unit, uint32 regAddr, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_sram_set(uint32 unit, uint32 regAddr, uint32 regVal);
EXTERN_API RtkApiRet dva_drv_indirect_sram_1byte_get(uint32 unit, uint32 regAddr, uint8 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_sram_1byte_set(uint32 unit, uint32 regAddr, uint8 regVal);
EXTERN_API RtkApiRet dva_drv_indirect_sram_2bytes_get(uint32 unit, uint32 regAddr, uint16 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_sram_2bytes_set(uint32 unit, uint32 regAddr, uint16 regVal);

EXTERN_API RtkApiRet dva_drv_indirect_serdes_get(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 *regVal);
EXTERN_API RtkApiRet dva_drv_indirect_serdes_set(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 regVal);

#endif /* DVA_ASICDRV_MEM_CTRL_H_ */
