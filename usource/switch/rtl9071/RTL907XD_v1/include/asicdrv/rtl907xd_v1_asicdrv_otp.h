
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

#ifndef DV1_ASICDRV_OTP_H_
#define DV1_ASICDRV_OTP_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_v1_api_otp.h"
#include "rtl907xd_v1_asicdrv_otp.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* General - OTP byte address count */
#define DV1_OTP_BYTE_END_ADDR ((uint32)0x1553u)
#define DV1_OTP_BYTE_CNT      (5460u)

/* Indirect Access - Channel */
#define DV1_OTP_INDIRECT_CHANNEL_NUM      (2u)
#define DV1_OTP_INDIRECT_ACCESS_COMMAND_0 (DV1_INTERNAL_CPU_VARIABLE47)
#define DV1_OTP_INDIRECT_ACCESS_ADDR_0    (DV1_INTERNAL_CPU_VARIABLE48)
#define DV1_OTP_INDIRECT_ACCESS_DATA_0    (DV1_INTERNAL_CPU_VARIABLE49)
#define DV1_OTP_INDIRECT_ACCESS_COMMAND_1 (DV1_INTERNAL_CPU_VARIABLE16)
#define DV1_OTP_INDIRECT_ACCESS_ADDR_1    (DV1_INTERNAL_CPU_VARIABLE17)
#define DV1_OTP_INDIRECT_ACCESS_DATA_1    (DV1_INTERNAL_CPU_VARIABLE18)

/* Indirect Access - Trigger */
#define DV1_OTP_INDIRECT_ACCESS_TRIGGER_DELAY (100u)

/* Indirect Access - Operation[31:28] */
#define DV1_OTP_INDIRECT_ACCESS_TRIGGER (31u)
#define DV1_OTP_INDIRECT_ACCESS_ERROR   (30u)
#define DV1_OTP_INDIRECT_ACCESS_READ    (29u)
#define DV1_OTP_INDIRECT_ACCESS_WRITE   (28u)
/* Indirect Access - Target[27:24] */
#define DV1_OTP_INDIRECT_ACCESS_TARGET     (24u)
#define DV1_OTP_INDIRECT_ACCESS_TARGET_LEN (4u)
/* Indirect Access - Error [23:0] */
#define DV1_OTP_INDIRECT_ACCESS_ERR_LSP (0u)
#define DV1_OTP_INDIRECT_ACCESS_ERR_LEN (24u)
#define DV1_OTP_INDIRECT_ACCESS_ERR_MAX (0xFFFFFFu)

/* Indirect Access - Data Channel 0 */
#define DV1_OTP_INDIRECT_ACCESS_OTP_DATA1_0 (DV1_HW_MISC_DMY_RESIGSER_7)
#define DV1_OTP_INDIRECT_ACCESS_OTP_DATA2_0 (DV1_INTERNAL_CPU_VARIABLE35)
#define DV1_OTP_INDIRECT_ACCESS_OTP_DATA3_0 (DV1_INTERNAL_CPU_VARIABLE37)

/* Indirect Access - Data Channel 1 */
#define DV1_OTP_INDIRECT_ACCESS_OTP_DATA1_1 (DV1_ALE_DMY0)
#define DV1_OTP_INDIRECT_ACCESS_OTP_DATA2_1 (DV1_ALE_DMY1)
#define DV1_OTP_INDIRECT_ACCESS_OTP_DATA3_1 (DV1_ALE_DMY2)

/* Indirect Access - Buffer size */
#define DV1_OTP_INDIRECT_ACCESS_BUF_SIZE (16u)

/* Indirect Access - Target */
#define DV1_OTP_INDIRECT_ACCESS_TARGET_OTP ((uint32)0x1u)

/* Indirect OTP - CMD */
#define DV1_OTP_CMD_OP_MASK   (0x1Fu)
#define DV1_OTP_CMD_ADDR_LSP  (16u)
#define DV1_OTP_CMD_ADDR_MASK (0xFFFFu)
#define DV1_OTP_CMD_LEN_LSP   (11u)
#define DV1_OTP_CMD_LEN_MASK  (0x1Fu)

/* Indirect OTP - OP */
#define DV1_OTP_CMD_OP_DSP              (0x0u)         /**< Deep sleep mode */
#define DV1_OTP_CMD_OP_WAKEUP           (0x1u)         /**< Wake up */
#define DV1_OTP_CMD_OP_RESET            (0x2u)         /**< Reset */
#define DV1_OTP_CMD_OP_DECODER_CHK      (0x3u)         /**< Decoder Array Check */
#define DV1_OTP_CMD_OP_MAIN_CHK         (0x4u)         /**< Main Array Check */
#define DV1_OTP_CMD_OP_REPAIR           (0x5u)         /**< Repair */
#define DV1_OTP_CMD_OP_DECODER_PGM      (0x6u)         /**< Decoder Program */
#define DV1_OTP_CMD_OP_PGM              (0x7u)         /**< Program */
#define DV1_OTP_CMD_OP_READ             ((uint32)0x8u) /**< Read */
#define DV1_OTP_CMD_OP_NORMAL_READ      (0x9u)         /**< Normal Read */
#define DV1_OTP_CMD_OP_INIT_MARGIN_READ (0xAu)         /**< Initial Margin Read */
#define DV1_OTP_CMD_OP_PGM_MARGIN_READ  (0xBu)         /**< Program Margin Read */
#define DV1_OTP_CMD_OP_TEMP             ((uint32)0xCu) /**< Temperature */
/* reserved 0xD */
#define DV1_OTP_CMD_OP_CHKSUM_CAL    (0xEu)                         /**< Checksum Calculation */
#define DV1_OTP_CMD_OP_CHKSUM_PGM    (0xFu)                         /**< Checksum Program */
#define DV1_OTP_CMD_OP_CHKSUM_VERIFY (0x10u)                        /**< Checksum Verify */
#define DV1_OTP_CMD_OP_END           (DV1_OTP_CMD_OP_CHKSUM_VERIFY) /**< End */

/* Indirect OTP - Temp */
#define DV1_OTP_CMD_TEMP_LSP  (8u)
#define DV1_OTP_CMD_TEMP_MASK (0x3u)
#define DV1_OTP_CMD_TEMP_LT   ((uint32)0u)
#define DV1_OTP_CMD_TEMP_RT   ((uint32)1u)
#define DV1_OTP_CMD_TEMP_HT   ((uint32)2u)
#define DV1_OTP_CMD_TEMP_END  (DV1_OTP_CMD_TEMP_HT)

/* Indirect OTP - Firmware status */
#define DV1_OTP_INDIR_ACCESS_FW_STATUS          (0xBFu)
#define DV1_OTP_INDIR_ACCESS_FW_STATUS_MASK     (0xFF000000u)
#define DV1_OTP_INDIR_ACCESS_FW_STATUS_STARTBIT (24u)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
extern uint32 dv1_g_otpTimeoutCnt;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet dv1_drv_otp_write_err_chk(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet dv1_drv_otp_read_err_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet dv1_drv_otp_fw_status_chk(uint32 unit);
EXTERN_API RtkApiRet dv1_drv_otp_indir_access(const uint32 unit, const uint32 ch, const uint32 cmd, uint32 *data);
EXTERN_API RtkApiRet dv1_drv_otp_indir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet dv1_drv_otp_indir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);

#endif /* DV1_ASICDRV_OTP_H_ */
