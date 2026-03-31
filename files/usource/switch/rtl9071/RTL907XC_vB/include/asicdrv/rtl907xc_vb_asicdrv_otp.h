
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

#ifndef CVB_ASICDRV_OTP_H_
#define CVB_ASICDRV_OTP_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_api_otp.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* General - OTP byte address count */
#define CVB_OTP_BYTE_CNT (5460u)

/* Indirect Access - Channel */
#define CVB_OTP_INDIRECT_CHANNEL_NUM      (2u)
#define CVB_OTP_INDIRECT_ACCESS_COMMAND_0 (CVB_INTERNAL_CPU_VARIABLE47)
#define CVB_OTP_INDIRECT_ACCESS_ADDR_0    (CVB_INTERNAL_CPU_VARIABLE48)
#define CVB_OTP_INDIRECT_ACCESS_DATA_0    (CVB_INTERNAL_CPU_VARIABLE49)
#define CVB_OTP_INDIRECT_ACCESS_COMMAND_1 (CVB_INTERNAL_CPU_VARIABLE16)
#define CVB_OTP_INDIRECT_ACCESS_ADDR_1    (CVB_INTERNAL_CPU_VARIABLE17)
#define CVB_OTP_INDIRECT_ACCESS_DATA_1    (CVB_INTERNAL_CPU_VARIABLE18)

/* Indirect Access - Trigger */
#define CVB_OTP_INDIRECT_ACCESS_TRIGGER_DELAY (100u)

/* Indirect Access - Operation[31:28] */
#define CVB_OTP_INDIRECT_ACCESS_TRIGGER (31u)
#define CVB_OTP_INDIRECT_ACCESS_ERROR   (30u)
#define CVB_OTP_INDIRECT_ACCESS_READ    (29u)
#define CVB_OTP_INDIRECT_ACCESS_WRITE   (28u)
/* Indirect Access - Target[27:24] */
#define CVB_OTP_INDIRECT_ACCESS_TARGET     (24u)
#define CVB_OTP_INDIRECT_ACCESS_TARGET_LEN (4u)
/* Indirect Access - Error [23:0] */
#define CVB_OTP_INDIRECT_ACCESS_ERROR_NUMBER_MASK (0xFFFFFFu)

/* Indirect Access - Data Channel 0 */
#define CVB_OTP_INDIRECT_ACCESS_OTP_DATA1_0 (CVB_HW_MISC_DMY_RESIGSER_7)
#define CVB_OTP_INDIRECT_ACCESS_OTP_DATA2_0 (CVB_INTERNAL_CPU_VARIABLE35)
#define CVB_OTP_INDIRECT_ACCESS_OTP_DATA3_0 (CVB_INTERNAL_CPU_VARIABLE37)

/* Indirect Access - Data Channel 1 */
#define CVB_OTP_INDIRECT_ACCESS_OTP_DATA1_1 (CVB_ALE_DMY0)
#define CVB_OTP_INDIRECT_ACCESS_OTP_DATA2_1 (CVB_ALE_DMY1)
#define CVB_OTP_INDIRECT_ACCESS_OTP_DATA3_1 (CVB_ALE_DMY2)

/* Indirect Access - Buffer size */
#define CVB_OTP_INDIRECT_ACCESS_BUF_SIZE (16u)

/* Indirect Access - Target */
#define CVB_OTP_INDIRECT_ACCESS_TARGET_OTP ((uint32)0x1u)

/* indirect OTP - CMD */
#define CVB_OTP_CMD_OP_MASK   (0x1Fu)
#define CVB_OTP_CMD_ADDR_LSP  (16u)
#define CVB_OTP_CMD_ADDR_MASK (0xFFFFu)
#define CVB_OTP_CMD_LEN_LSP   (11u)
#define CVB_OTP_CMD_LEN_MASK  (0x1Fu)

/* indirect OTP - OP */
#define CVB_OTP_CMD_OP_DSP                 (0x0u)         /**< Deep sleep mode */
#define CVB_OTP_CMD_OP_WAKEUP              (0x1u)         /**< Wake up */
#define CVB_OTP_CMD_OP_RESET               (0x2u)         /**< Reset */
#define CVB_OTP_CMD_OP_DECODER_CHK         (0x3u)         /**< Decoder Array Check */
#define CVB_OTP_CMD_OP_MAIN_CHK            (0x4u)         /**< Main Array Check */
#define CVB_OTP_CMD_OP_REPAIR              (0x5u)         /**< Repair */
#define CVB_OTP_CMD_OP_DECODER_PGM         (0x6u)         /**< Decoder Program */
#define CVB_OTP_CMD_OP_PGM                 (0x7u)         /**< Program */
#define CVB_OTP_CMD_OP_READ                ((uint32)0x8u) /**< Read */
#define CVB_OTP_CMD_OP_RT_READ             (0x9u)         /**< Room Temp Read */
#define CVB_OTP_CMD_OP_RT_INIT_MARGIN_READ (0xAu)         /**< Room Temp Initial Margin Read */
#define CVB_OTP_CMD_OP_RT_PGM_MARGIN_READ  (0xBu)         /**< Room Temp Program Margin Read */
#define CVB_OTP_CMD_OP_HT_INIT_MARGIN_READ (0xCu)         /**< High Temp Initial Margin Read */
#define CVB_OTP_CMD_OP_HT_PGM_MARGIN_READ  (0xDu)         /**< High Temp Program Margin Read */
#define CVB_OTP_CMD_OP_CHKSUM_CAL          (0xEu)         /**< Checksum Calculation */
#define CVB_OTP_CMD_OP_CHKSUM_PGM          (0xFu)         /**< Checksum Program */
#define CVB_OTP_CMD_OP_CHKSUM_VERIFY       (0x10u)        /**< Checksum Verify */
#define CVB_OTP_CMD_OP_LOCKINFO_GET        (0x11u)        /**< Lock info */
#define CVB_OTP_CMD_OP_END                 (0x12u)        /**< End */

/* indirect OTP - CP/FT Mode */
#define CVB_OTP_CMD_TESTING_LSP  (9u)
#define CVB_OTP_CMD_TESTING_MASK (0x3u)

/* Indirect OTP - Firmware status */
#define CVB_OTP_FLASHLESS_FW_STATUS0            (0xB3u)
#define CVB_OTP_FLASHLESS_FW_STATUS1            (0xB4u)
#define CVB_OTP_INDIR_ACCESS_FW_STATUS          (0xBFu)
#define CVB_OTP_INDIR_ACCESS_FW_STATUS_MASK     (0xFF000000u)
#define CVB_OTP_INDIR_ACCESS_FW_STATUS_STARTBIT (24u)

/* OTP Resource Lock */
#define CVB_OTP_LOCK_INT (0u)
#define CVB_OTP_LOCK_EXT (1u)
/* General define */
#define CVB_OTP_BYTE_END_ADDR   ((uint32)0x1553u)
#define CVB_OTP_4BYTE_ADDR_MASK (0xFFFu)
/* Timeout value */
#define CVB_OTP_TIMEOUT_CNT (0x7FFFu)
/* FSM case # */
#define CVB_OTP_FSM      (0u)
#define CVB_OTP_MAIN_FSM (1u)
#define CVB_OTP_IF_FSM   (2u)
/* FSM reg val */
#define CVB_OTP_FSM_READY       ((uint8)1u)
#define CVB_OTP_MAIN_FSM_ACTIVE ((uint8)4u)
#define CVB_OTP_MAIN_FSM_DSP    ((uint8)0u)
#define CVB_OTP_IF_FSM_IDLE     ((uint8)0u)
/* Poll case # */
#define CVB_OTP_DONE_INT     (0u)
#define CVB_OTP_MISMATCH_INT (1u)
#define CVB_OTP_NG_INT       (2u)
#define CVB_OTP_KILLCHIP_INT (3u)
/* Access Mode case # */
#define CVB_OTP_ACCESS_MAIN    (0u)
#define CVB_OTP_ACCESS_TESTROW (1u)
#define CVB_OTP_ACCESS_TESTCOL (3u)
/* OTP Mode */
#define CVB_OTP_MODE_READ (0u)
/* OTP Read Mode */
#define CVB_OTP_BASIC_READ_MODE        (0u)
#define CVB_OTP_BURST_READ_MODE        (1u)
#define CVB_OTP_BASIC_MARGIN_READ_MODE (2u)
#define CVB_OTP_BURST_MARGIN_READ_MODE (3u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
extern uint32 cvb_g_otpTimeoutCnt;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
RtkApiRet cvb_drv_otp_indir_access(const uint32 unit, const uint32 ch, const uint32 cmd, uint32 *data);
#endif
EXTERN_API RtkApiRet cvb_drv_otp_write_err_chk(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cvb_drv_otp_read_err_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cvb_drv_otp_fw_status_chk(uint32 unit);
EXTERN_API RtkApiRet cvb_drv_otp_indir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cvb_drv_otp_indir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);

#endif /* CVB_ASICDRV_OTP_H_ */
