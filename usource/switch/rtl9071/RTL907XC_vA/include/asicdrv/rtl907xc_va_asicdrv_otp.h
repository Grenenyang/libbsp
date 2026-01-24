
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

#ifndef CVA_ASICDRV_OTP_H_
#define CVA_ASICDRV_OTP_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_va_api_otp.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* General - OTP byte address count */
#define CVA_OTP_BYTE_CNT (5460u)

/* Indirect Access - Channel */
#define CVA_OTP_INDIRECT_CHANNEL_NUM      (2u)
#define CVA_OTP_INDIRECT_ACCESS_COMMAND_0 (CVA_INTERNAL_CPU_VARIABLE47)
#define CVA_OTP_INDIRECT_ACCESS_ADDR_0    (CVA_INTERNAL_CPU_VARIABLE48)
#define CVA_OTP_INDIRECT_ACCESS_DATA_0    (CVA_INTERNAL_CPU_VARIABLE49)
#define CVA_OTP_INDIRECT_ACCESS_COMMAND_1 (CVA_INTERNAL_CPU_VARIABLE16)
#define CVA_OTP_INDIRECT_ACCESS_ADDR_1    (CVA_INTERNAL_CPU_VARIABLE17)
#define CVA_OTP_INDIRECT_ACCESS_DATA_1    (CVA_INTERNAL_CPU_VARIABLE18)

/* Indirect Access - Trigger */
#define CVA_OTP_INDIRECT_ACCESS_TRIGGER_DELAY (100u)

/* Indirect Access - Operation[31:28] */
#define CVA_OTP_INDIRECT_ACCESS_TRIGGER (31u)
#define CVA_OTP_INDIRECT_ACCESS_ERROR   (30u)
#define CVA_OTP_INDIRECT_ACCESS_READ    (29u)
#define CVA_OTP_INDIRECT_ACCESS_WRITE   (28u)
/* Indirect Access - Target[27:24] */
#define CVA_OTP_INDIRECT_ACCESS_TARGET     (24u)
#define CVA_OTP_INDIRECT_ACCESS_TARGET_LEN (4u)
/* Indirect Access - Error [23:0] */
#define CVA_OTP_INDIRECT_ACCESS_ERROR_NUMBER_MASK (0xFFFFFFu)

/* Indirect Access - Data Channel 0 */
#define CVA_OTP_INDIRECT_ACCESS_OTP_DATA1_0 (CVA_HW_MISC_DMY_RESIGSER_7)
#define CVA_OTP_INDIRECT_ACCESS_OTP_DATA2_0 (CVA_INTERNAL_CPU_VARIABLE35)
#define CVA_OTP_INDIRECT_ACCESS_OTP_DATA3_0 (CVA_INTERNAL_CPU_VARIABLE37)

/* Indirect Access - Data Channel 1 */
#define CVA_OTP_INDIRECT_ACCESS_OTP_DATA1_1 (CVA_ALE_DMY0)
#define CVA_OTP_INDIRECT_ACCESS_OTP_DATA2_1 (CVA_ALE_DMY1)
#define CVA_OTP_INDIRECT_ACCESS_OTP_DATA3_1 (CVA_ALE_DMY2)

/* Indirect Access - Buffer size */
#define CVA_OTP_INDIRECT_ACCESS_BUF_SIZE (16u)

/* Indirect Access - Target */
#define CVA_OTP_INDIRECT_ACCESS_TARGET_OTP ((uint32)0x1u)

/* Indirect OTP - CMD */
#define CVA_OTP_CMD_OP_MASK   (0x1Fu)
#define CVA_OTP_CMD_ADDR_LSP  (16u)
#define CVA_OTP_CMD_ADDR_MASK (0xFFFFu)
#define CVA_OTP_CMD_LEN_LSP   (11u)
#define CVA_OTP_CMD_LEN_MASK  (0x1Fu)

/* Indirect OTP - OP */
#define CVA_OTP_CMD_OP_DSP                 (0x0u)         /**< Deep sleep mode */
#define CVA_OTP_CMD_OP_WAKEUP              (0x1u)         /**< Wake up */
#define CVA_OTP_CMD_OP_RESET               (0x2u)         /**< Reset */
#define CVA_OTP_CMD_OP_DECODER_CHK         (0x3u)         /**< Decoder Array Check */
#define CVA_OTP_CMD_OP_MAIN_CHK            (0x4u)         /**< Main Array Check */
#define CVA_OTP_CMD_OP_REPAIR              (0x5u)         /**< Repair */
#define CVA_OTP_CMD_OP_DECODER_PGM         (0x6u)         /**< Decoder Program */
#define CVA_OTP_CMD_OP_PGM                 (0x7u)         /**< Program */
#define CVA_OTP_CMD_OP_READ                ((uint32)0x8u) /**< Read */
#define CVA_OTP_CMD_OP_RT_READ             (0x9u)         /**< Room Temp Read */
#define CVA_OTP_CMD_OP_RT_INIT_MARGIN_READ (0xAu)         /**< Room Temp Initial Margin Read */
#define CVA_OTP_CMD_OP_RT_PGM_MARGIN_READ  (0xBu)         /**< Room Temp Program Margin Read */
#define CVA_OTP_CMD_OP_HT_INIT_MARGIN_READ (0xCu)         /**< High Temp Initial Margin Read */
#define CVA_OTP_CMD_OP_HT_PGM_MARGIN_READ  (0xDu)         /**< High Temp Program Margin Read */
#define CVA_OTP_CMD_OP_CHKSUM_CAL          (0xEu)         /**< Checksum Calculation */
#define CVA_OTP_CMD_OP_CHKSUM_PGM          (0xFu)         /**< Checksum Program */
#define CVA_OTP_CMD_OP_CHKSUM_VERIFY       (0x10u)        /**< Checksum Verify */
#define CVA_OTP_CMD_OP_LOCKINFO_GET        (0x11u)        /**< Lock info */
#define CVA_OTP_CMD_OP_END                 (0x12u)        /**< End */

/* Indirect OTP - CP/FT Mode */
#define CVA_OTP_CMD_TESTING_LSP  (9u)
#define CVA_OTP_CMD_TESTING_MASK (0x3u)

/* Indirect OTP - Firmware status */
#define CVA_OTP_FLASHLESS_FW_STATUS0            (0xB3u)
#define CVA_OTP_FLASHLESS_FW_STATUS1            (0xB4u)
#define CVA_OTP_INDIR_ACCESS_FW_STATUS          (0xBFu)
#define CVA_OTP_INDIR_ACCESS_FW_STATUS_MASK     (0xFF000000u)
#define CVA_OTP_INDIR_ACCESS_FW_STATUS_STARTBIT (24u)

/* Direct OTP - Resource Lock Role */
#define CVA_OTP_LOCK_INT (0u)
#define CVA_OTP_LOCK_EXT (1u)
#define CVA_OTP_LOCK_MCU (2u)
#define CVA_OTP_LOCK_END (31u)

/* Direct OTP - General define */
#define CVA_OTP_BYTE_END_ADDR   ((uint32)0x1553u)
#define CVA_OTP_4BYTE_END_ADDR  ((uint32)0x554u)
#define CVA_OTP_4BYTE_ADDR_MASK (0xFFFu)

/* Timeout value */
#define CVA_OTP_TIMEOUT_CNT (0x7FFFu)

/* Direct OTP - FSM case # */
#define CVA_OTP_FSM      (0u)
#define CVA_OTP_MAIN_FSM (1u)
#define CVA_OTP_IF_FSM   (2u)

/* Direct OTP - FSM reg val */
#define CVA_OTP_FSM_READY       ((uint8)1u)
#define CVA_OTP_MAIN_FSM_ACTIVE ((uint8)4u)
#define CVA_OTP_MAIN_FSM_DSP    ((uint8)0u)
#define CVA_OTP_IF_FSM_IDLE     ((uint8)0u)

/* Direct OTP - Poll case # */
#define CVA_OTP_DONE_INT     (0u)
#define CVA_OTP_MISMATCH_INT (1u)
#define CVA_OTP_NG_INT       (2u)
#define CVA_OTP_KILLCHIP_INT (3u)

/* Direct OTP - Access Mode case # */
#define CVA_OTP_ACCESS_MAIN    (0u)
#define CVA_OTP_ACCESS_TESTROW (1u)
#define CVA_OTP_ACCESS_TESTCOL (3u)

/* Direct OTP - OTP Mode */
#define CVA_OTP_MODE_READ                (0u)
#define CVA_OTP_MODE_PGM                 (4u)
#define CVA_OTP_MODE_RT_INIT_MARGIN_READ (2u)
#define CVA_OTP_MODE_RT_PGM_MARGIN_READ  (8u)
#define CVA_OTP_MODE_HT_INIT_MARGIN_READ (0x12u)
#define CVA_OTP_MODE_HT_PGM_MARGIN_READ  (0x18u)
#define CVA_OTP_MODE_REPAIR_MARGIN_READ  (3u)
#define CVA_OTP_MODE_REPAIR_CHECK        (9u)
#define CVA_OTP_MODE_REPAIR_PGM          (5u)

/* Direct OTP - OTP Read Mode */
#define CVA_OTP_BASIC_READ_MODE        (0u)
#define CVA_OTP_BURST_READ_MODE        (1u)
#define CVA_OTP_BASIC_MARGIN_READ_MODE (2u)
#define CVA_OTP_BURST_MARGIN_READ_MODE (3u)

/******************************************************************************/
/* Struct                                                                     */
/******************************************************************************/
typedef struct
{
    /* [in] */
    uint32 iAddr;
    /* [in] */
    uint32 pgmData;
    /* [in] */
    uint32 newData;
    /* [out] */
    RtkApiRet retVal;
} cvaOtpPgmInfo_t;

typedef struct
{
    /* [in] */
    uint8 testMode;
    /* [in] */
    uint8 mode;
    /* [in] */
    uint32 addr;
    /* [out] */
    uint8 *buf;
    /* [in] */
    uint32 len;
    /* [in] */
    uint32 startAddr;
    /* [in] */
    uint32 endAddr;
    /* [out] */
    RtkApiRet retVal;
} cvaOtpReadInfo_t;

typedef struct
{
    /* [in] */
    uint8 testMode;
    /* [in] */
    uint8 mode;
    /* [in] for initial_margin_read */
    uint8 autoMode;
    /* [in] */
    uint32 iAddr;
    /* [out] */
    uint32 data[4];
    /* [out] */
    RtkApiRet retVal;
} cvaOtpMarginReadInfo_t;

typedef struct
{
    /* [in] */
    uint8 testMode;
    /* [in] */
    uint8 mode;
    /* [in] */
    uint32 iAddr;
    /* [in] */
    uint32 data;
    /* [out] */
    RtkApiRet retVal;
} cvaOtpBasicPgmInfo_t;

typedef struct
{
    /* [in] */
    uint8 testMode;
    /* [in] */
    uint8 mode;
    /* [in] */
    uint32 addr;
    /* [in] */
    const uint8 *buf;
    /* [in] */
    uint32 len;
    /* [in] */
    uint32 startAddr;
    /* [in] */
    uint32 endAddr;
    /* [out] */
    RtkApiRet retVal;
} cvaOtpWriteInfo_t;

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
extern uint32 cva_g_otpLockRole;
extern uint32 cva_g_otpTimeoutCnt;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet cva_drv_otp_write_err_chk(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cva_drv_otp_read_err_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cva_drv_otp_fw_status_get(uint32 unit, uint8 *const fwStatus);
EXTERN_API RtkApiRet cva_drv_otp_indir_access(const uint32 unit, const uint32 ch, const uint32 cmd, uint32 *data);
EXTERN_API RtkApiRet cva_drv_otp_indir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cva_drv_otp_indir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cva_drv_otp_dir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len);
EXTERN_API RtkApiRet cva_drv_otp_dir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len);

/* Sub-function for direct OTP */
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
EXTERN_API RtkApiRet cva_drv_otp_lock(uint32 unit);
EXTERN_API RtkApiRet cva_drv_otp_unlock(uint32 unit);
EXTERN_API RtkApiRet cva_drv_otp_fsm_chk(uint32 unit, const uint8 fsm, const uint8 val);
EXTERN_API RtkApiRet cva_drv_otp_flg_chk(uint32 unit, const uint8 flg, const uint8 val);
EXTERN_API RtkApiRet cva_drv_otp_int_clr(uint32 unit, uint8 flg);
EXTERN_API RtkApiRet cva_drv_otp_read_mode_set(uint32 unit, uint8 mode);
EXTERN_API RtkApiRet cva_drv_otp_trigger(uint32 unit, const uint8 mode, const uint32 startAddr);
EXTERN_API RtkApiRet cva_drv_otp_dsp(uint32 unit);
EXTERN_API RtkApiRet cva_drv_otp_wakeup(uint32 unit);
EXTERN_API RtkApiRet cva_drv_otp_access_mode(uint32 unit, uint8 mode);
EXTERN_API RtkApiRet cva_drv_otp_basic_pgm(uint32 unit, cvaOtpBasicPgmInfo_t *otpBasicPgmInfo);
EXTERN_API RtkApiRet cva_drv_otp_margin_read(uint32 unit, cvaOtpMarginReadInfo_t *param);
#endif

#endif /* CVA_ASICDRV_OTP_H_ */
