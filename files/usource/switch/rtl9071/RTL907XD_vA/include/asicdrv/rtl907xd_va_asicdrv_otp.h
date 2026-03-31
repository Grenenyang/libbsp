
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

#ifndef DVA_ASICDRV_OTP_H_
#define DVA_ASICDRV_OTP_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api_otp.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* General - OTP byte address count */
#define DVA_OTP_BYTE_END_ADDR ((uint32)0x1553u)
#define DVA_OTP_BYTE_CNT      (5460u)

/* Indirect Access - Channel */
#define DVA_OTP_INDIRECT_CHANNEL_NUM      (2u)
#define DVA_OTP_INDIRECT_ACCESS_COMMAND_0 (DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE)
#define DVA_OTP_INDIRECT_ACCESS_ADDR_0    (DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_ADDRESS)
#define DVA_OTP_INDIRECT_ACCESS_DATA_0    (DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_DATA)
#define DVA_OTP_INDIRECT_ACCESS_COMMAND_1 (DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_OPCODE)
#define DVA_OTP_INDIRECT_ACCESS_ADDR_1    (DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_ADDRESS)
#define DVA_OTP_INDIRECT_ACCESS_DATA_1    (DVA_CPU_INDIRECT_ACCESS_CHANNEL_1_DATA)

/* Indirect Access - Trigger */
#define DVA_OTP_INDIRECT_ACCESS_TRIGGER_LEN    (1u)
#define DVA_OTP_INDIRECT_ACCESS_TRIGGER_MAXNS  (100000000u)
#define DVA_OTP_INDIRECT_ACCESS_TRIGGER_MINNS  (0u)
#define DVA_OTP_INDIRECT_ACCESS_TRIGGER_ACTION (1u)
#define DVA_OTP_INDIRECT_ACCESS_TRIGGER_EXPVAL (0u)

/* Indirect Access - Operation[31:28] */
#define DVA_OTP_INDIRECT_ACCESS_TRIGGER (31u)
#define DVA_OTP_INDIRECT_ACCESS_ERROR   (30u)
#define DVA_OTP_INDIRECT_ACCESS_READ    (29u)
#define DVA_OTP_INDIRECT_ACCESS_WRITE   (28u)
/* Indirect Access - Target[27:24] */
#define DVA_OTP_INDIRECT_ACCESS_TARGET     (24u)
#define DVA_OTP_INDIRECT_ACCESS_TARGET_LEN (4u)
/* Indirect Access - Error [23:0] */
#define DVA_OTP_INDIRECT_ACCESS_ERR_LSP (0u)
#define DVA_OTP_INDIRECT_ACCESS_ERR_LEN (24u)
#define DVA_OTP_INDIRECT_ACCESS_ERR_MAX (0xFFFFFFu)

/* Indirect Access - Data Channel 0 */
#define DVA_OTP_INDIRECT_ACCESS_OTP_DATA1_0 (DVA_HW_MISC_DMY7)
#define DVA_OTP_INDIRECT_ACCESS_OTP_DATA2_0 (DVA_INTERNAL_CPU_VARIABLE35)
#define DVA_OTP_INDIRECT_ACCESS_OTP_DATA3_0 (DVA_INTERNAL_CPU_VARIABLE37)

/* Indirect Access - Data Channel 1 */
#define DVA_OTP_INDIRECT_ACCESS_OTP_DATA1_1 (DVA_ALE_DMY0)
#define DVA_OTP_INDIRECT_ACCESS_OTP_DATA2_1 (DVA_ALE_DMY1)
#define DVA_OTP_INDIRECT_ACCESS_OTP_DATA3_1 (DVA_ALE_DMY2)

/* Indirect Access - Buffer size */
#define DVA_OTP_INDIRECT_ACCESS_BUF_SIZE (16u)

/* Indirect Access - Target */
#define DVA_OTP_INDIRECT_ACCESS_TARGET_OTP ((uint32)0x1u)

/* Indirect OTP - CMD */
#define DVA_OTP_CMD_OP_MASK   (0x1Fu)
#define DVA_OTP_CMD_ADDR_LSP  (16u)
#define DVA_OTP_CMD_ADDR_MASK (0xFFFFu)
#define DVA_OTP_CMD_LEN_LSP   (11u)
#define DVA_OTP_CMD_LEN_MASK  (0x1Fu)

/* Indirect OTP - OP */
#define DVA_OTP_CMD_OP_DSP                 (0x0u)                    /**< Deep sleep mode */
#define DVA_OTP_CMD_OP_WAKEUP              (0x1u)                    /**< Wake up */
#define DVA_OTP_CMD_OP_RESET               (0x2u)                    /**< Reset */
#define DVA_OTP_CMD_OP_DECODER_CHK         (0x3u)                    /**< Decoder Array Check */
#define DVA_OTP_CMD_OP_MAIN_CHK            (0x4u)                    /**< Main Array Check */
#define DVA_OTP_CMD_OP_REPAIR              (0x5u)                    /**< Repair */
#define DVA_OTP_CMD_OP_DECODER_PGM         (0x6u)                    /**< Decoder Program */
#define DVA_OTP_CMD_OP_PGM                 (0x7u)                    /**< Program */
#define DVA_OTP_CMD_OP_READ                ((uint32)0x8u)            /**< Read */
#define DVA_OTP_CMD_OP_NORMAL_READ         (0x9u)                    /**< Normal Read */
#define DVA_OTP_CMD_OP_INIT_MARGIN_READ    (0xAu)                    /**< Initial Margin Read */
#define DVA_OTP_CMD_OP_PGM_MARGIN_READ     (0xBu)                    /**< Program Margin Read */
#define DVA_OTP_CMD_OP_THREE_READ_MODE_CHK ((uint32)0xCu)            /**< Three Read Mode Check */
#define DVA_OTP_CMD_OP_TEMP                ((uint32)0xDu)            /**< Temperature */
#define DVA_OTP_CMD_OP_CHKSUM_CAL          (0xEu)                    /**< Checksum Calculation */
#define DVA_OTP_CMD_OP_CHKSUM_PGM          (0xFu)                    /**< Checksum Program */
#define DVA_OTP_CMD_OP_CHKSUM_VERIFY       (0x10u)                   /**< Checksum Verify */
#define DVA_OTP_CMD_OP_ARR_TYPE            (0x11u)                   /**< Array Type */
#define DVA_OTP_CMD_OP_END                 (DVA_OTP_CMD_OP_ARR_TYPE) /**< End */

/* Indirect OTP - Temp */
#define DVA_OTP_CMD_TEMP_LSP  (8u)
#define DVA_OTP_CMD_TEMP_MASK (0x3u)
#define DVA_OTP_CMD_TEMP_LT   ((uint32)0u)
#define DVA_OTP_CMD_TEMP_RT   ((uint32)1u)
#define DVA_OTP_CMD_TEMP_HT   ((uint32)2u)
#define DVA_OTP_CMD_TEMP_END  (DVA_OTP_CMD_TEMP_HT)

/* Indirect OTP - Firmware status */
#define DVA_OTP_INDIR_ACCESS_FW_STATUS          (0xBFu)
#define DVA_OTP_INDIR_ACCESS_FW_STATUS_MASK     (0xFF000000u)
#define DVA_OTP_INDIR_ACCESS_FW_STATUS_STARTBIT (24u)

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet dva_drv_otp_write_err_chk(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet dva_drv_otp_read_err_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet dva_drv_otp_fw_status_chk(uint32 unit);
EXTERN_API RtkApiRet dva_drv_otp_indir_access(const uint32 unit, const uint32 ch, const uint32 cmd, uint32 *data);
EXTERN_API RtkApiRet dva_drv_otp_indir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet dva_drv_otp_indir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);

#endif /* DVA_ASICDRV_OTP_H_ */
