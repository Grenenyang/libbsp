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

#include <stdint.h>
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_asicdrv_otp.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"

uint32 cva_g_otpLockRole   = CVA_OTP_LOCK_EXT;
uint32 cva_g_otpTimeoutCnt = RTK_TIMER_OTP_INDIRECT_CHANNEL;

static RtkApiRet cva_drv_otp_indir_retval_get(uint32 unit, uint32 indirCmdAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    CHK_FUN_RET(retVal, cva_reg_field_read(unit, indirCmdAddr, 0u, 24u, &regVal));
    if((RT_ERR_OK == retVal) && (regVal <= 0x7FFFFFFFu))
    {
        retVal = -(RtkApiRet)regVal;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_indir_store(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(CVA_OTP_CMD_OP_PGM == op)
    {
        /* [in]: op
         * [in]: addr
         * [in]: len
         * [in]: data[3..0]
         * Write 1/.../16byte data {[7:0] of data[0] / ... / [31:24] of data[3]}
         */
        CHK_FUN_RET(retVal, cva_reg_write(unit, indirDataAddr[0], data[0]));
        CHK_FUN_RET(retVal, cva_reg_write(unit, indirDataAddr[1], data[1]));
        CHK_FUN_RET(retVal, cva_reg_write(unit, indirDataAddr[2], data[2]));
        CHK_FUN_RET(retVal, cva_reg_write(unit, indirDataAddr[3], data[3]));
    }
    else if(CVA_OTP_CMD_OP_CHKSUM_CAL == op)
    {
        /* [in]: op
         * [in]: addr
         * [in]: len
         * [in]: only sum if [31] of data[0] is 1
         * [out]: calculated checksum in [7:0] of data[0]
         * [reserved]: [30:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cva_reg_write(unit, indirDataAddr[0], data[0]));
    }
    else if(CVA_OTP_CMD_OP_CHKSUM_PGM == op)
    {
        /* [in]: op
         * [in]: testing
         * [in]: chksum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cva_reg_write(unit, indirDataAddr[0], data[0]));
    }
    else if(CVA_OTP_CMD_OP_CHKSUM_VERIFY == op)
    {
        /* [in]: op
         * [in]: testing
         * [in]: sum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cva_reg_write(unit, indirDataAddr[0], data[0]));
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_indir_trigger(const uint32 unit, const uint32 indirCmdAddr)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    indirCmd = 0x0u;
    uint32    regVal   = 0x0u;
    uint32    tryTimes = 0u;

    indirCmd = ((uint32)1u << CVA_OTP_INDIRECT_ACCESS_TRIGGER) | ((uint32)1u << CVA_OTP_INDIRECT_ACCESS_READ) | (CVA_OTP_INDIRECT_ACCESS_TARGET_OTP << CVA_OTP_INDIRECT_ACCESS_TARGET);
    CHK_FUN_RET(retVal, cva_reg_write(unit, indirCmdAddr, indirCmd));

    /* Polling indir channel done */
    do
    {
        if(tryTimes++ > cva_g_otpTimeoutCnt)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            break;
        }
        CHK_FUN_RET(retVal, cva_reg_bit_get(unit, indirCmdAddr, CVA_OTP_INDIRECT_ACCESS_TRIGGER, &regVal));
        CHK_FUN_RET(retVal, rtk_usecond_delay(CVA_OTP_INDIRECT_ACCESS_TRIGGER_DELAY));
    } while(0u != regVal);

    return retVal;
}

static RtkApiRet cva_drv_otp_indir_load_0(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(CVA_OTP_CMD_OP_LOCKINFO_GET == op)
    {
        /* CVA_OTP_CMD_OP_LOCKINFO_GET
         * [in]: op
         * [in]: addr
         * [out]: otpLockAddr in [15:0] of data[0]
         * [out]: mcuLockAddr in [31:16] of data[0]
         * [out]: otpLockBit#: [5:0] of data[1]
         * [out]: mcuLockBit#: [21:16] of data[1]
         * [reserved]: [15:6]/[31:22] of data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cva_reg_read(unit, indirDataAddr[0], &data[0]));
        CHK_FUN_RET(retVal, cva_reg_read(unit, indirDataAddr[1], &data[1]));
    }
    else if((CVA_OTP_CMD_OP_CHKSUM_CAL == op) || (CVA_OTP_CMD_OP_CHKSUM_VERIFY == op))
    {
        /* CVA_OTP_CMD_OP_CHKSUM_CAL
         * [in]: op
         * [in]: addr
         * [in]: len
         * [in]: only sum if [31] of data[0] is 1
         * [out]: calculated checksum in [7:0] of data[0]
         * [reserved]: [30:8] of data[0], data[1], data[2], data[3]
         *
         * CVA_OTP_CMD_OP_CHKSUM_VERIFY
         * [in]: op
         * [in]: testing
         * [in]: sum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cva_reg_read(unit, indirDataAddr[0], &data[0]));
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_indir_load(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((CVA_OTP_CMD_OP_MAIN_CHK == op) || ((op >= CVA_OTP_CMD_OP_READ) && (op <= CVA_OTP_CMD_OP_HT_PGM_MARGIN_READ)))
    {
        /*
         * CVA_OTP_CMD_OP_MAIN_CHK
         * [out]: ngFlag in [0] of data[0]
         * [out]: ngKill in [1] of data[0]
         * [out]: ngCnt in data[1]
         * [out]: ngAddr0 in data[2]
         * [out]: ngAddr1 in data[3]
         *
         * CVA_OTP_CMD_OP_READ
         * [in]: op
         * [in]: addr
         * [in]: len
         * [out]: data[3..0]
         * read 1/.../16byte data into {[7:0] of data[0]/.../[31:24] of data[3]}
         *
         * CVA_OTP_CMD_OP_READ/CVA_OTP_CMD_OP_RT_READ / OTP_CMD_OP_RT/HT_INIT_MARGIN_READ / OTP_CMD_OP_RT/HT_PGM_MARGIN_READ
         * [in]: op
         * [in]: addr
         * [out]: data[3..0]
         * read 4byte data into data[3..0]
         *
         * OTP_CMD_OP_CHECKSUM_CAL
         * [out]: calculated checksum in [7:0] of data[0]
         */
        CHK_FUN_RET(retVal, cva_reg_read(unit, indirDataAddr[0], &data[0]));
        CHK_FUN_RET(retVal, cva_reg_read(unit, indirDataAddr[1], &data[1]));
        CHK_FUN_RET(retVal, cva_reg_read(unit, indirDataAddr[2], &data[2]));
        CHK_FUN_RET(retVal, cva_reg_read(unit, indirDataAddr[3], &data[3]));
    }
    else
    {
        CHK_FUN_RET(retVal, cva_drv_otp_indir_load_0(unit, op, indirDataAddr, data));
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_indir_ch_get(uint32 ch, uint32 *indirCmdAddr, uint32 *otpCmdAddr, uint32 *indirDataAddr)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(0u == ch)
    {
        *indirCmdAddr    = CVA_OTP_INDIRECT_ACCESS_COMMAND_0;
        *otpCmdAddr      = CVA_OTP_INDIRECT_ACCESS_ADDR_0;
        indirDataAddr[0] = CVA_OTP_INDIRECT_ACCESS_DATA_0;
        indirDataAddr[1] = CVA_OTP_INDIRECT_ACCESS_OTP_DATA1_0;
        indirDataAddr[2] = CVA_OTP_INDIRECT_ACCESS_OTP_DATA2_0;
        indirDataAddr[3] = CVA_OTP_INDIRECT_ACCESS_OTP_DATA3_0;
    }
    else if(1u == ch)
    {
        *indirCmdAddr    = CVA_OTP_INDIRECT_ACCESS_COMMAND_1;
        *otpCmdAddr      = CVA_OTP_INDIRECT_ACCESS_ADDR_1;
        indirDataAddr[0] = CVA_OTP_INDIRECT_ACCESS_DATA_1;
        indirDataAddr[1] = CVA_OTP_INDIRECT_ACCESS_OTP_DATA1_1;
        indirDataAddr[2] = CVA_OTP_INDIRECT_ACCESS_OTP_DATA2_1;
        indirDataAddr[3] = CVA_OTP_INDIRECT_ACCESS_OTP_DATA3_1;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Access OTP via indirect channel
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ch indirect channel number
 * @param[in] cmd indirect channel command
 * @param[in] data Read back data for read operation
 * @param[out] data Written data for write operation
 * @retval RT_ERR_OK
 * @retval RT_ERR_OTP_FSM_MAIN_TIMEOUT
 * @retval RT_ERR_OTP_FSM_IF_TIMEOUT
 * @retval RT_ERR_OTP_FSM_READY_TIMEOUT
 * @retval RT_ERR_OTP_INT
 * @retval RT_ERR_OTP_MISMATCH
 * @retval RT_ERR_OTP_LOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_LOCK
 * @retval RT_ERR_OTP_UNLOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_UNLOCK
 * @retval RT_ERR_OTP_INIT_MARGIN_READ_CMP
 * @retval RT_ERR_OTP_PGM_MARGIN_READ_CMP
 * @retval RT_ERR_OTP_BASIC_READ_CMP
 * @retval RT_ERR_OTP_READ_LOCKDATA
 * @retval RT_ERR_OTP_PGM_SAME_DATA
 * @retval RT_ERR_OTP_PGM_INVALID_DATA
 * @retval RT_ERR_OTP_PGM_LOCKDATA
 * @retval RT_ERR_OTP_DEADLOCK
 * @retval RT_ERR_OTP_NG
 * @retval RT_ERR_OTP_KILLCHIP
 * @retval RT_ERR_OTP_NOT_REPAIR
 * @retval RT_ERR_OTP_TESTROW_INIT_MARGIN_READ
 * @retval RT_ERR_OTP_TESTROW_PGM_MARGIN_READ
 * @retval RT_ERR_OTP_TESTROW_NORMAL_READ
 * @retval RT_ERR_OTP_TESTCOL_INIT_MARGIN_READ
 * @retval RT_ERR_OTP_TESTCOL_PGM_MARGIN_READ
 * @retval RT_ERR_OTP_TESTCOL_NORMAL_READ
 * @retval RT_ERR_OTP_CHKSUM_ERROR
 * @note op = cmd[4:0] / addr = cmd[31:16] / len = cmd[15:11] / testing = cmd[10:9] / reserved = cmd[8:5] / [31] of data[0] = sum(1)/checksum(0)
 */
RtkApiRet cva_drv_otp_indir_access(const uint32 unit, const uint32 ch, const uint32 cmd, uint32 *data)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    op               = cmd & CVA_OTP_CMD_OP_MASK;
    uint32    indirCmdAddr     = 0x0u;
    uint32    otpCmdAddr       = 0x0u;
    uint32    indirDataAddr[4] = {0};

    if((ch >= CVA_OTP_INDIRECT_CHANNEL_NUM) || (op >= CVA_OTP_CMD_OP_END))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if((NULL == data) && ((op == CVA_OTP_CMD_OP_MAIN_CHK) || (op >= CVA_OTP_CMD_OP_PGM)))
    {
        /* data shall be used */
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get channel register addresses */
    CHK_FUN_RET(retVal, cva_drv_otp_indir_ch_get(ch, &indirCmdAddr, &otpCmdAddr, indirDataAddr));

    /* Set cmd to OTP command reg */
    CHK_FUN_RET(retVal, cva_reg_write(unit, otpCmdAddr, cmd));

    /* Store data in indir data reg before trigger indir channel command for program operation*/
    if(NULL != data)
    {
        CHK_FUN_RET(retVal, cva_drv_otp_indir_store(unit, op, indirDataAddr, data));
    }

    /* Trigger indir channel command */
    CHK_FUN_RET(retVal, cva_drv_otp_indir_trigger(unit, indirCmdAddr));
    /* Load indir data reg to data after trigger indir channel cmd for read operation */
    if(NULL != data)
    {
        CHK_FUN_RET(retVal, cva_drv_otp_indir_load(unit, op, indirDataAddr, data));
    }

    /* Get error code from indir cmd reg */
    CHK_FUN_RET(retVal, cva_drv_otp_indir_retval_get(unit, indirCmdAddr));

    return retVal;
}

/**
 * @brief Check the input parameter
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr        Address
 * @param[in] len         Buffer length
 * @param[out] buf        Data buffer
 * @retval RT_ERR_OK             The input parameter is valid.
 * @retval -RT_ERR_INPUT         The input paraemter is invalid.
 * @retval -RT_ERR_NULL_POINTER  The input paraemter is NULL.
 */
RtkApiRet cva_drv_otp_write_err_chk(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((0u == len) || (addr > CVA_OTP_BYTE_END_ADDR) || (len > CVA_OTP_BYTE_CNT) || (addr > (CVA_OTP_BYTE_CNT - len)))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == buf)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Check the input parameter
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr        Address
 * @param[in] len         Buffer length
 * @param[out] buf        Data buffer
 * @retval RT_ERR_OK             The input parameter is valid.
 * @retval -RT_ERR_INPUT         The input paraemter is invalid.
 * @retval -RT_ERR_NULL_POINTER  The input paraemter is NULL.
 */
RtkApiRet cva_drv_otp_read_err_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((0u == len) || (addr > CVA_OTP_BYTE_END_ADDR) || (len > CVA_OTP_BYTE_CNT) || (addr > (CVA_OTP_BYTE_CNT - len)))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == buf)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Get the firmware status for indirect access channel
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] fwStatus   Firmware status
 * @retval RT_ERR_OK             The firmware status read successfully.
 * @retval -RT_ERR_NULL_POINTER  The input paraemter is NULL.
 * @retval -RT_ERR_OTP_INDIR_ACCESS The indirect access is failed
 */
RtkApiRet cva_drv_otp_fw_status_get(uint32 unit, uint8 *const fwStatus)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if(NULL == fwStatus)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, cva_reg_read(CVA_UNIT, CVA_CPU_DMY_1, &regVal));
    if(RT_ERR_OK == retVal)
    {
        *fwStatus = (uint8)((regVal & CVA_OTP_INDIR_ACCESS_FW_STATUS_MASK) >> CVA_OTP_INDIR_ACCESS_FW_STATUS_STARTBIT);
    }

    return retVal;
}

/**
 * @brief Write OTP memory via the indirectly access
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr        Address
 * @param[in] buf         Data buffer
 * @param[in] len         Buffer length
 * @param[out] None
 * @retval RT_ERR_OK      The OTP memory write successfully.
 * @retval -RT_ERR_INPUT  The input paraemters are invalid.
 * @retval -RT_ERR_NULL_POINTER  The input buf is NULL.
 * @retval RT_ERR_OTP_FSM_MAIN_TIMEOUT
 * @retval RT_ERR_OTP_FSM_IF_TIMEOUT
 * @retval RT_ERR_OTP_FSM_READY_TIMEOUT
 * @retval RT_ERR_OTP_INT
 * @retval RT_ERR_OTP_MISMATCH
 * @retval RT_ERR_OTP_LOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_LOCK
 * @retval RT_ERR_OTP_UNLOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_UNLOCK
 * @retval RT_ERR_OTP_INIT_MARGIN_READ_CMP
 * @retval RT_ERR_OTP_PGM_MARGIN_READ_CMP
 * @retval RT_ERR_OTP_BASIC_READ_CMP
 * @retval RT_ERR_OTP_READ_LOCKDATA
 * @retval RT_ERR_OTP_PGM_SAME_DATA
 * @retval RT_ERR_OTP_PGM_INVALID_DATA
 * @retval RT_ERR_OTP_PGM_LOCKDATA
 * @retval RT_ERR_OTP_DEADLOCK
 */
RtkApiRet cva_drv_otp_indir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    iBuf         = 0u;
    uint32    iIndirData   = 0u;
    uint32    indirCmd     = 0u;
    uint32    indirAddr    = 0u;
    uint32    indirLen     = 0u;
    uint32    indirData[4] = {0u};

    for(iBuf = 0u; iBuf < len; iBuf += CVA_OTP_INDIRECT_ACCESS_BUF_SIZE)
    {
        if(addr <= (CVA_OTP_BYTE_CNT - iBuf))
        {
            indirAddr = addr + iBuf;
            if((iBuf + CVA_OTP_INDIRECT_ACCESS_BUF_SIZE) <= len)
            {
                indirLen = CVA_OTP_INDIRECT_ACCESS_BUF_SIZE;
            }
            else
            {
                /* Write OTP for last round */
                indirLen = len - iBuf;
            }
        }
        else
        {
            retVal = -RT_ERR_INPUT;
        }

        /* Cast buf to indirData */
        CHK_FUN_RET(retVal, cva_drv_drv_memset(indirData, 0, sizeof(indirData)));
        if(RT_ERR_OK == retVal)
        {
            for(iIndirData = 0u; iIndirData < indirLen; iIndirData++)
            {
                if(((uint32)buf[iIndirData + iBuf] << (8u * (iIndirData % 4u))) <= 0xFF000000u)
                {
                    indirData[iIndirData / 4u] |= (uint32)((uint32)buf[iIndirData + iBuf] << (8u * (iIndirData % 4u)));
                }
            }

            /* Indir access */
            indirCmd = CVA_OTP_CMD_OP_PGM | (indirAddr << CVA_OTP_CMD_ADDR_LSP) | (indirLen << CVA_OTP_CMD_LEN_LSP);
            CHK_FUN_RET(retVal, cva_drv_otp_indir_access(unit, 0u, indirCmd, indirData));
        }
    }

    return retVal;
}

/**
 * @brief Read OTP memory via the indirectly access
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr        Address
 * @param[in] len         Buffer length
 * @param[out] buf        Data buffer
 * @retval RT_ERR_OK      The OTP memory read successfully.
 * @retval -RT_ERR_INPUT  The input paraemters are invalid.
 * @retval -RT_ERR_NULL_POINTER  The input buf is NULL.
 * @retval RT_ERR_OTP_FSM_MAIN_TIMEOUT
 * @retval RT_ERR_OTP_FSM_IF_TIMEOUT
 * @retval RT_ERR_OTP_FSM_READY_TIMEOUT
 * @retval RT_ERR_OTP_INT
 * @retval RT_ERR_OTP_MISMATCH
 * @retval RT_ERR_OTP_LOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_LOCK
 * @retval RT_ERR_OTP_UNLOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_UNLOCK
 * @retval RT_ERR_OTP_INIT_MARGIN_READ_CMP
 * @retval RT_ERR_OTP_PGM_MARGIN_READ_CMP
 * @retval RT_ERR_OTP_BASIC_READ_CMP
 * @retval RT_ERR_OTP_READ_LOCKDATA
 * @retval RT_ERR_OTP_PGM_SAME_DATA
 * @retval RT_ERR_OTP_PGM_INVALID_DATA
 * @retval RT_ERR_OTP_PGM_LOCKDATA
 * @retval RT_ERR_OTP_DEADLOCK
 */
RtkApiRet cva_drv_otp_indir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal;
    uint32    iBuf         = 0u;
    uint32    iIndirData   = 0u;
    uint32    indirCmd     = 0u;
    uint32    indirAddr    = 0u;
    uint32    indirLen     = 0u;
    uint32    indirData[4] = {0u};

    /* Other modules read OTP indirectly in the normal mode */
    for(iBuf = 0u; iBuf < len; iBuf += CVA_OTP_INDIRECT_ACCESS_BUF_SIZE)
    {
        if(addr <= (CVA_OTP_BYTE_CNT - iBuf))
        {
            indirAddr = addr + iBuf;
            if((iBuf + CVA_OTP_INDIRECT_ACCESS_BUF_SIZE) <= len)
            {
                indirLen = CVA_OTP_INDIRECT_ACCESS_BUF_SIZE;
            }
            else
            {
                /* Read OTP for last round */
                indirLen = len - iBuf;
            }

            /* Indir access */
            if(0u == iBuf)
            {
                retVal = cva_drv_drv_memset(indirData, 0, sizeof(indirData));
            }
            else
            {
                CHK_FUN_RET(retVal, cva_drv_drv_memset(indirData, 0, sizeof(indirData)));
            }
            indirCmd = CVA_OTP_CMD_OP_READ | (indirAddr << CVA_OTP_CMD_ADDR_LSP) | (indirLen << CVA_OTP_CMD_LEN_LSP);
            CHK_FUN_RET(retVal, cva_drv_otp_indir_access(unit, 0u, indirCmd, indirData));

            if(RT_ERR_OK == retVal)
            {
                /* Cast indirData to buf */
                for(iIndirData = 0u; iIndirData < indirLen; iIndirData++)
                {
                    buf[iIndirData + iBuf] = (uint8)((indirData[iIndirData / 4u] >> (8u * (iIndirData % 4u))) & 0xFFu);
                }
            }
        }
        else
        {
            retVal = -RT_ERR_INPUT;
        }
    }

    return retVal;
}

/**
 * @brief OTP mutex lock
 * @retval RT_ERR_OK             OTP lock successfully.
 * @retval RT_ERR_OTP_DEADLOCK     The OTP lock bit is already 1.
 * @retval RT_ERR_OTP_LOCK         The register access is abnormal.
 * @retval RT_ERR_OTP_LOCK_ROLE_INVALID         The lock role is invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_lock(uint32 unit)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    intLock  = 0u;
    uint32    regVal   = 0u;
    uint32    tryTimes = 0u;

    /* Check whether internal/external CPU has already get the resource, it indicates a software error
     * due to internal CPU doesn't released the lock previously.
     */
    if(cva_g_otpLockRole <= CVA_OTP_LOCK_EXT)
    {
        CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_INDIRECT_OCCUPY_STATUS_REG, &regVal));
        intLock = (regVal >> cva_g_otpLockRole) & (uint32)0x1u;

        if(1u == intLock)
        {
            retVal = -RT_ERR_OTP_DEADLOCK_XC_VA;
        }
        else
        {
            /* do nothing */
        }

        do
        {
            /* OTP channel idle */
            CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_INDIRECT_OCCUPY_STATUS_REG, &regVal));
            if(0u == regVal)
            {
                CHK_FUN_RET(retVal, cva_reg_bit_set(unit, CVA_OTP_INDIRECT_OCCUPY_STATUS_REG, cva_g_otpLockRole, 1u));
                CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_INDIRECT_OCCUPY_STATUS_REG, &regVal));
                intLock = (regVal >> cva_g_otpLockRole) & (uint32)0x1u;

                if(1u == intLock)
                {
                    break;
                }
                else
                {
                    /* do nothing */
                }
            }
            else if(tryTimes++ > CVA_OTP_TIMEOUT_CNT)
            {
                retVal = -RT_ERR_OTP_LOCK_XC_VA;
            }
            else
            {
                /* do nothing */
            }
        } while(RT_ERR_OK == retVal);
    }
    else
    {
        retVal = -RT_ERR_OTP_LOCK_ROLE_INVALID_XC_VA;
    }

    return retVal;
}

/**
 * @brief OTP mutex unlock
 * @retval RT_ERR_OK             OTP unlock successfully.
 * @retval RT_ERR_OTP_UNLOCK     The OTP unlock fail.
 * @retval RT_ERR_FAILED         The register access is abnormal.
 * @retval RT_ERR_OTP_UNLOCK_ROLE_INVALID         The unlock role is invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_unlock(uint32 unit)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    intLock = 0u;
    uint32    regVal  = 0u;

    /* Judge whether internal/external cpu get the lock */
    if(cva_g_otpLockRole <= CVA_OTP_LOCK_EXT)
    {
        CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_INDIRECT_OCCUPY_STATUS_REG, &regVal));
        intLock = (regVal >> cva_g_otpLockRole) & (uint32)0x1u;

        if(1u == intLock)
        {
            CHK_FUN_RET(retVal, cva_reg_bit_set(unit, CVA_OTP_INDIRECT_OCCUPY_STATUS_REG, cva_g_otpLockRole, 0u));
            CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_INDIRECT_OCCUPY_STATUS_REG, &regVal));
            intLock = (regVal >> cva_g_otpLockRole) & (uint32)0x1u;
            if(1u == intLock)
            {
                retVal = -RT_ERR_OTP_UNLOCK_XC_VA;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            retVal = RT_ERR_FAILED;
        }
    }
    else
    {
        retVal = -RT_ERR_OTP_UNLOCK_ROLE_INVALID_XC_VA;
    }

    return retVal;
}

/**
 * @brief Check OTP fsm
 * @param[in]  fsm       OTP fsm
 * @param[in]  val       fsm value
 * @retval RT_ERR_OK             Check OTP fsm successfully.
 * @retval RT_ERR_INPUT         The input parameter is invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_fsm_chk(uint32 unit, const uint8 fsm, const uint8 val)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regVal   = 0u;
    uint32    tryTimes = 0u;

    switch(fsm)
    {
        case CVA_OTP_FSM:
            do
            {
                if(tryTimes++ > CVA_OTP_TIMEOUT_CNT)
                {
                    retVal = -RT_ERR_OTP_FSM_READY_TIMEOUT_XC_VA;
                    break;
                }
                CHK_FUN_RET(retVal, cva_reg_field_read(unit, CVA_OTP_FSM_STATE, CVA_OTP_FSM_STATE_OTP_READY_LSP, CVA_OTP_FSM_STATE_OTP_READY_LEN, &regVal));
            } while(val != regVal);
            break;
        case CVA_OTP_MAIN_FSM:
            do
            {
                if(tryTimes++ > CVA_OTP_TIMEOUT_CNT)
                {
                    retVal = -RT_ERR_OTP_FSM_MAIN_TIMEOUT_XC_VA;
                    break;
                }
                CHK_FUN_RET(retVal, cva_reg_field_read(unit, CVA_OTP_FSM_STATE, CVA_OTP_FSM_STATE_OTP_MAIN_FSM_LSP, CVA_OTP_FSM_STATE_OTP_MAIN_FSM_LEN, &regVal));
            } while(val != regVal);
            break;
        case CVA_OTP_IF_FSM:
            do
            {
                if(tryTimes++ > CVA_OTP_TIMEOUT_CNT)
                {
                    retVal = -RT_ERR_OTP_FSM_IF_TIMEOUT_XC_VA;
                    break;
                }
                else
                {
                    /* do nothing */
                }
                CHK_FUN_RET(retVal, cva_reg_field_read(unit, CVA_OTP_FSM_STATE, CVA_OTP_FSM_STATE_OTP_IF_FSM_LSP, CVA_OTP_FSM_STATE_OTP_IF_FSM_LEN, &regVal));
            } while(val != regVal);
            break;
        default:
            retVal = -RT_ERR_INPUT;
            break;
    }

    return retVal;
}

/**
 * @brief Check OTP interrupt flag
 * @param[in]  flg       OTP interrupt flag type
 * @param[in]  val       flag value
 * @retval RT_ERR_OK             Check OTP interrupt flag successfully.
 * @retval RT_ERR_INPUT         The input parameter is invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_flg_chk(uint32 unit, const uint8 flg, const uint8 val)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regVal   = 0u;
    uint32    tryTimes = 0u;

    switch(flg)
    {
        case CVA_OTP_DONE_INT:
            do
            {
                if(tryTimes++ > CVA_OTP_TIMEOUT_CNT)
                {
                    retVal = -RT_ERR_OTP_INT_XC_VA;
                    break;
                }
                CHK_FUN_RET(retVal, cva_reg_field_read(unit, CVA_OTP_INT_STS_REG, CVA_OTP_INT_STS_REG_OTP_OP_DONE_INT_LSP, CVA_OTP_INT_STS_REG_OTP_OP_DONE_INT_LEN, &regVal));
            } while(val != regVal);
            break;
        default:
            retVal = -RT_ERR_INPUT;
            break;
    }

    return retVal;
}

/**
 * @brief Clear OTP interrupt
 * @param[in]  flg       OTP interrupt flag type
 * @retval RT_ERR_OK             Clear OTP interrupt successfully.
 * @retval RT_ERR_INPUT         The input parameter is invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_int_clr(uint32 unit, uint8 flg)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    switch(flg)
    {
        case CVA_OTP_DONE_INT:
            regVal = (uint32)1u << CVA_OTP_INT_STS_REG_OTP_OP_DONE_INT_LSP;
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_INT_STS_REG, regVal));
            CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 0u));
            break;
        case CVA_OTP_MISMATCH_INT:
            regVal = (uint32)1u << CVA_OTP_INT_STS_REG_OTP_PDOUT_MISMATCH_INT_LSP;
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_INT_STS_REG, regVal));
            CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 0u));
            break;
        case CVA_OTP_NG_INT:
            regVal = (uint32)1u << CVA_OTP_INT_STS_REG_OTP_INIT_RD_CMP_HIGH_NG_INT_LSP;
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_INT_STS_REG, regVal));
            CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 0u));
            break;
        case CVA_OTP_KILLCHIP_INT:
            regVal = (uint32)1u << CVA_OTP_INT_STS_REG_OTP_INIT_RD_KILLCHIP_INT_LSP;
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_INT_STS_REG, regVal));
            CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 0u));
            break;
        default:
            retVal = -RT_ERR_INPUT;
            break;
    }

    return retVal;
}

/**
 * @brief Set OTP read mode
 * @param[in]  mode       OTP read mode
 * @retval RT_ERR_OK            Set OTP read mode successfully.
 * @retval RT_ERR_INPUT         The input parameter is invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_read_mode_set(uint32 unit, uint8 mode)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    uint32    rtw     = 0u;
    uint32    raa     = 1u;
    uint32    autoClr = 0u;

    switch(mode)
    {
        case CVA_OTP_BASIC_READ_MODE:
            rtw     = (uint32)0u;
            raa     = (uint32)0u;
            autoClr = (uint32)0u;
            break;
        case CVA_OTP_BURST_READ_MODE:
            rtw     = (uint32)1u;
            raa     = (uint32)0u;
            autoClr = (uint32)1u;
            break;
        case CVA_OTP_BASIC_MARGIN_READ_MODE:
            rtw     = (uint32)1u;
            raa     = (uint32)0u;
            autoClr = (uint32)0u;
            break;
        case CVA_OTP_BURST_MARGIN_READ_MODE:
            rtw     = (uint32)0u;
            raa     = (uint32)1u;
            autoClr = (uint32)0u;
            break;
        default:
            retVal = -RT_ERR_INPUT;
            break;
    }

    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_CTRL_REG, &regVal));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_READ_THEN_WAIT_EN_LSP, CVA_OTP_CTRL_REG_OTP_READ_THEN_WAIT_EN_LEN, rtw, &regVal));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_READ_ALL_ADDR_EN_LSP, CVA_OTP_CTRL_REG_OTP_READ_ALL_ADDR_EN_LEN, raa, &regVal));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_AUTO_CLR_STATUS_LSP, CVA_OTP_CTRL_REG_OTP_AUTO_CLR_STATUS_LEN, autoClr, &regVal));
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_CTRL_REG, regVal));

    return retVal;
}

/**
 * @brief Trigger OTP
 * @param[in]  mode       OTP mode
 * @param[in]  startAddr  OTP start address
 * @retval RT_ERR_OK      The OTP trigger successfully.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_trigger(uint32 unit, const uint8 mode, const uint32 startAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_TRIGGER, &regVal));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_TRIGGER, CVA_OTP_TRIGGER_OTP_MODE_LSP, CVA_OTP_TRIGGER_OTP_MODE_LEN, (uint32)mode, &regVal));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_TRIGGER, CVA_OTP_TRIGGER_OTP_START_ADDR_LSP, CVA_OTP_TRIGGER_OTP_START_ADDR_LEN, (uint32)startAddr & CVA_OTP_4BYTE_ADDR_MASK, &regVal));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_TRIGGER, CVA_OTP_TRIGGER_OTP_OP_START_LSP, CVA_OTP_TRIGGER_OTP_OP_START_LEN, 1u, &regVal));
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_TRIGGER, regVal));

    return retVal;
}

/**
 * @brief OTP deep sleep (no testAndSet)
 * @retval RT_ERR_OK             The OTP deep sleep successfully.
 * @retval RT_ERR_OTP_FSM_READY_TIMEOUT         The OTP ready fsm is timeout.
 * @retval RT_ERR_OTP_FSM_MAIN_TIMEOUT         The OTP main fsm is timeout.
 * @retval RT_ERR_OTP_FSM_IF_TIMEOUT         The OTP interface fsm is timeout.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_dsp(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    CHK_FUN_RET(retVal, cva_reg_field_read(unit, CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_DS_EN_LSP, CVA_OTP_CTRL_REG_OTP_DS_EN_LEN, &regVal));

    if(0u == regVal)
    {
        /* step 1 */
        CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_MAIN_FSM, CVA_OTP_MAIN_FSM_ACTIVE));

        /* step 2 */
        CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_IF_FSM, CVA_OTP_IF_FSM_IDLE));

        /* step 3 */
        CHK_FUN_RET(retVal, cva_reg_field_write(unit, CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_DS_EN_LSP, CVA_OTP_CTRL_REG_OTP_DS_EN_LEN, 1));
        CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_MAIN_FSM, CVA_OTP_MAIN_FSM_DSP));
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief OTP wakeup (no testAndSet)
 * @retval RT_ERR_OK             The OTP wakeup successfully.
 * @retval RT_ERR_OTP_FSM_READY_TIMEOUT         The OTP ready fsm is timeout.
 * @retval RT_ERR_OTP_FSM_MAIN_TIMEOUT         The OTP main fsm is timeout.
 * @retval RT_ERR_OTP_FSM_IF_TIMEOUT         The OTP interface fsm is timeout.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_wakeup(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* step 1 */
    CHK_FUN_RET(retVal, cva_reg_field_write(unit, CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_DS_EN_LSP, CVA_OTP_CTRL_REG_OTP_DS_EN_LEN, 0u));

    /* step 2 */
    CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_FSM, CVA_OTP_FSM_READY));

    /* step 3 */
    CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_MAIN_FSM, CVA_OTP_MAIN_FSM_ACTIVE));

    /* step 4 */
    CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_IF_FSM, CVA_OTP_IF_FSM_IDLE));

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cva_drv_otp_access_mode_test(uint32 unit, uint8 mode)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    switch(mode)
    {
        case CVA_OTP_ACCESS_TESTROW:
            CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_CTRL_REG, &regVal));
            CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_TEST_ROW_COL_SEL_LSP, CVA_OTP_CTRL_REG_OTP_TEST_ROW_COL_SEL_LEN, 0u, &regVal));
            CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_TEST_ARRAY_EN_LSP, CVA_OTP_CTRL_REG_OTP_TEST_ARRAY_EN_LEN, 1u, &regVal));
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_CTRL_REG, regVal));
            break;
        case CVA_OTP_ACCESS_TESTCOL:
            /* No 2 out of 3 */
            CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_CTRL_REG, &regVal));
            CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_TEST_ROW_COL_SEL_LSP, CVA_OTP_CTRL_REG_OTP_TEST_ROW_COL_SEL_LEN, 1u, &regVal));
            CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_TEST_ARRAY_EN_LSP, CVA_OTP_CTRL_REG_OTP_TEST_ARRAY_EN_LEN, 1u, &regVal));
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_CTRL_REG, regVal));
            break;
        default:
            retVal = -RT_ERR_INPUT;
            break;
    }

    return retVal;
}

/**
 * @brief OTP access mode
 * @param[in]  mode       OTP access mode
 * @retval RT_ERR_OK             The OTP basic read successfully.
 * @retval RT_ERR_INPUT         The input paraemter is invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_access_mode(uint32 unit, uint8 mode)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    switch(mode)
    {
        case CVA_OTP_ACCESS_MAIN:
            CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_CTRL_REG, &regVal));
            CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_TEST_ROW_COL_SEL_LSP, CVA_OTP_CTRL_REG_OTP_TEST_ROW_COL_SEL_LEN, 0u, &regVal));
            CHK_FUN_RET(retVal, cva_reg_field_set(CVA_OTP_CTRL_REG, CVA_OTP_CTRL_REG_OTP_TEST_ARRAY_EN_LSP, CVA_OTP_CTRL_REG_OTP_TEST_ARRAY_EN_LEN, 0u, &regVal));
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_CTRL_REG, regVal));
            break;
        default:
            CHK_FUN_RET(retVal, cva_drv_otp_access_mode_test(unit, mode));
            break;
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_basic_read_core0(uint32 unit, const cvaOtpReadInfo_t *const otpReadInfo, const uint32 i1, const uint32 i2)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;
    uint32    i      = 0u;
    uint32    bufIdx = 0u;

    /* step 1 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step 2 */
    CHK_FUN_RET(retVal, cva_drv_otp_read_mode_set(unit, CVA_OTP_BASIC_READ_MODE));

    /* step 3 */
    CHK_FUN_RET(retVal, cva_drv_otp_trigger(unit, otpReadInfo->mode, otpReadInfo->startAddr));

    /* step 4 */
    CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 1u));

    /* step 5 */
    if(RT_ERR_OK == retVal)
    {
        CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_DATA_REG, &regVal));
        for(i = i1; i <= i2; i++)
        {
            otpReadInfo->buf[bufIdx] = (uint8)((regVal >> (8u * i)) & 0xFFu);
            if((bufIdx + 1u) > otpReadInfo->len)
            {
                retVal = -RT_ERR_INPUT;
            }
            else
            {
                bufIdx++;
            }
        }
    }

    /* step 6 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step 7 */
    CHK_FUN_RET(retVal, cva_drv_otp_dsp(unit));

    return retVal;
}

/**
 * @brief OTP baisc read
 * @param[in]  otpReadInfo       OTP read information
 * @param[out] otpReadInfo       The read back data is stored in buf.
 * @retval RT_ERR_OK             The OTP basic read successfully.
 * @retval -RT_ERR_NULL_POINTER  The input paraemter is NULL.
 * @retval -RT_ERR_INPUT         The input paraemters are invalid.
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_basic_read(uint32 unit, cvaOtpReadInfo_t *otpReadInfo)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i1     = 0u;
    uint32    i2     = 0u;

    if(NULL == otpReadInfo)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((0u == otpReadInfo->len) || (otpReadInfo->len > CVA_OTP_BYTE_CNT) || (otpReadInfo->addr > CVA_OTP_BYTE_END_ADDR) || ((otpReadInfo->addr + otpReadInfo->len) > (CVA_OTP_BYTE_END_ADDR + 1u)))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        i1 = otpReadInfo->addr % 4u;
        i2 = (otpReadInfo->addr + otpReadInfo->len - 1u) % 4u;
    }

    CHK_FUN_RET(retVal, cva_drv_otp_access_mode(unit, otpReadInfo->testMode));
    CHK_FUN_RET(retVal, cva_drv_otp_wakeup(unit));
    CHK_FUN_RET(retVal, cva_drv_otp_basic_read_core0(unit, otpReadInfo, i1, i2));

    return retVal;
}

static RtkApiRet cva_drv_otp_pcs_read_core2(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* step N-2 */
    CHK_FUN_RET(retVal, cva_drv_otp_read_mode_set(unit, CVA_OTP_BASIC_READ_MODE));
    CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_MAIN_FSM, CVA_OTP_MAIN_FSM_ACTIVE));
    CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_IF_FSM, CVA_OTP_IF_FSM_IDLE));

    /* step N-1 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step N */
    CHK_FUN_RET(retVal, cva_drv_otp_dsp(unit));

    return retVal;
}

static RtkApiRet cva_drv_otp_pcs_read_core1(uint32 unit, const cvaOtpReadInfo_t *const otpReadInfo)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal;
    uint32    iAddr = otpReadInfo->startAddr;
    uint32    i;
    uint32    i1 = 0u;
    uint32    i2;
    uint32    bufIdx       = 0u;
    uint32    startAddrIdx = otpReadInfo->addr % 4u;
    uint32    endAddrIdx   = 0u;

    if((0u == otpReadInfo->len) || (otpReadInfo->len > CVA_OTP_BYTE_CNT) || (otpReadInfo->addr > CVA_OTP_BYTE_END_ADDR) || ((otpReadInfo->addr + otpReadInfo->len) > (CVA_OTP_BYTE_END_ADDR + 1u)))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        endAddrIdx = (otpReadInfo->addr + otpReadInfo->len - 1u) % 4u;
    }
    do
    {
        /* step 1 */
        CHK_FUN_RET(retVal, cva_drv_otp_trigger(unit, otpReadInfo->mode, iAddr));

        /* step 2 */
        CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 1u));
        /* step 3. */
        if(RT_ERR_OK == retVal)
        {
            CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_DATA_REG, &regVal));
            /* calculate the start/end idx of the bufIdx */
            if(iAddr == otpReadInfo->startAddr)
            {
                i1 = startAddrIdx;
                i2 = (uint32)3u;
            }
            else if(iAddr == otpReadInfo->endAddr)
            {
                i1 = (uint32)0u;
                i2 = endAddrIdx;
            }
            else
            {
                i1 = 0u;
                i2 = 3u;
            }
            /* cast OTP_DATA_REG to buf */
            for(i = i1; i <= i2; i++)
            {
                otpReadInfo->buf[bufIdx] = (uint8)((regVal >> (8u * i)) & 0xFFu);
                if((bufIdx + 1u) > otpReadInfo->len)
                {
                    retVal = -RT_ERR_INPUT;
                }
                else
                {
                    bufIdx++;
                }
            }
        }
        else
        {
            break;
        }
        ++iAddr;
    } while(iAddr <= otpReadInfo->endAddr);

    return retVal;
}

static RtkApiRet cva_drv_otp_pcs_read_core0(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* step 1 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step 2 */
    CHK_FUN_RET(retVal, cva_drv_otp_read_mode_set(unit, CVA_OTP_BURST_READ_MODE));

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cva_drv_otp_pcs_read(uint32 unit, cvaOtpReadInfo_t *otpReadInfo)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Step 2. Burst read operation */
    CHK_FUN_RET(retVal, cva_drv_otp_access_mode(unit, otpReadInfo->testMode));
    CHK_FUN_RET(retVal, cva_drv_otp_wakeup(unit));

    CHK_FUN_RET(retVal, cva_drv_otp_pcs_read_core0(unit));
    CHK_FUN_RET(retVal, cva_drv_otp_pcs_read_core1(unit, otpReadInfo));
    CHK_FUN_RET(retVal, cva_drv_otp_pcs_read_core2(unit));

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cva_drv_otp_basic_pgm(uint32 unit, cvaOtpBasicPgmInfo_t *otpBasicPgmInfo)
{
    RtkApiRet retVal;

    retVal = cva_drv_otp_access_mode(unit, otpBasicPgmInfo->testMode);
    CHK_FUN_RET(retVal, cva_drv_otp_wakeup(unit));

    /* step 1 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step 2 */
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_OTP_DATA_REG, otpBasicPgmInfo->data));

    /* step 3 */
    CHK_FUN_RET(retVal, cva_drv_otp_trigger(unit, otpBasicPgmInfo->mode, otpBasicPgmInfo->iAddr));

    /* step 4 */
    CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 1u));

    /* step 5 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step 6 */
    if(CVA_OTP_MODE_REPAIR_PGM != otpBasicPgmInfo->mode)
    {
        CHK_FUN_RET(retVal, cva_drv_otp_dsp(unit));
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_margin_read_core0(uint32 unit, cvaOtpMarginReadInfo_t *param)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* step 2 */
    switch(param->mode)
    {
        case CVA_OTP_MODE_READ:
        case CVA_OTP_MODE_RT_PGM_MARGIN_READ:
        case CVA_OTP_MODE_HT_PGM_MARGIN_READ:
        case CVA_OTP_MODE_REPAIR_CHECK:
            /* i.e. function_basic_read, pgm_margin_read_by_cpu */
            CHK_FUN_RET(retVal, cva_drv_otp_read_mode_set(unit, CVA_OTP_BASIC_READ_MODE));
            break;
        case CVA_OTP_MODE_RT_INIT_MARGIN_READ:
        case CVA_OTP_MODE_HT_INIT_MARGIN_READ:
        case CVA_OTP_MODE_REPAIR_MARGIN_READ:
            if(0u == param->autoMode)
            {
                /* i.e. initial_margin_read_by_cpu */
                CHK_FUN_RET(retVal, cva_drv_otp_read_mode_set(unit, CVA_OTP_BASIC_MARGIN_READ_MODE));
            }
            else
            {
                /* i.e. initial_margin_read_by_HW, repair_margin_read */
                CHK_FUN_RET(retVal, cva_drv_otp_read_mode_set(unit, CVA_OTP_BURST_MARGIN_READ_MODE));
            }
            break;
        default:
            retVal = -RT_ERR_INPUT;
            break;
    }

    /* step 3 */
    CHK_FUN_RET(retVal, cva_drv_otp_trigger(unit, param->mode, param->iAddr));

    /* step 4 */
    CHK_FUN_RET(retVal, cva_drv_otp_flg_chk(unit, CVA_OTP_DONE_INT, 1u));

    /* step 5 */
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_READ_DATA_REG_0, &(param->data[0])));
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_READ_DATA_REG_1, &(param->data[1])));
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_READ_DATA_REG_2, &(param->data[2])));
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_OTP_DATA_REG, &(param->data[3])));

    return retVal;
}

/* no testAndSet */
DYNAMIC_SCOPE RtkApiRet cva_drv_otp_margin_read(uint32 unit, cvaOtpMarginReadInfo_t *param)
{
    RtkApiRet retVal;

    retVal = cva_drv_otp_access_mode(unit, param->testMode);
    CHK_FUN_RET(retVal, cva_drv_otp_wakeup(unit));

    /* step 1 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step 2/.../5 */
    CHK_FUN_RET(retVal, cva_drv_otp_margin_read_core0(unit, param));

    /* step 6 */
    CHK_FUN_RET(retVal, cva_drv_otp_read_mode_set(unit, CVA_OTP_BASIC_READ_MODE));
    CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_MAIN_FSM, CVA_OTP_MAIN_FSM_ACTIVE));
    CHK_FUN_RET(retVal, cva_drv_otp_fsm_chk(unit, CVA_OTP_IF_FSM, CVA_OTP_IF_FSM_IDLE));

    /* step 7 */
    CHK_FUN_RET(retVal, cva_drv_otp_int_clr(unit, CVA_OTP_DONE_INT));

    /* step 8 */
    if((CVA_OTP_MODE_REPAIR_MARGIN_READ != param->mode) && (CVA_OTP_MODE_REPAIR_CHECK != param->mode))
    {
        CHK_FUN_RET(retVal, cva_drv_otp_dsp(unit));
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_read_input_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == buf)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((0u == len) || (len > CVA_OTP_BYTE_CNT) || (addr > CVA_OTP_BYTE_END_ADDR) || ((addr + len) > (CVA_OTP_BYTE_END_ADDR + 1u)))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Read OTP via direct access
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr        Address
 * @param[in] len         Buffer length
 * @param[out] buf        Data buffer
 * @retval RT_ERR_OK      The OTP memory read successfully.
 * @retval -RT_ERR_INPUT  The input paraemters are invalid.
 * @retval -RT_ERR_NULL_POINTER  The input buf is NULL.
 * @retval RT_ERR_OTP_FSM_MAIN_TIMEOUT
 * @retval RT_ERR_OTP_FSM_IF_TIMEOUT
 * @retval RT_ERR_OTP_FSM_READY_TIMEOUT
 * @retval RT_ERR_OTP_INT
 * @retval RT_ERR_OTP_MISMATCH
 * @retval RT_ERR_OTP_LOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_LOCK
 * @retval RT_ERR_OTP_UNLOCK_ROLE_INVALID
 * @retval RT_ERR_OTP_UNLOCK
 * @retval RT_ERR_OTP_BASIC_READ_CMP
 * @retval RT_ERR_OTP_DEADLOCK
 */
RtkApiRet cva_drv_otp_dir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet        retVal;
    RtkApiRet        retLockVal;
    cvaOtpReadInfo_t otpReadInfo;

    retVal = cva_drv_otp_read_input_chk(unit, addr, buf, len);

    if(RT_ERR_OK == retVal)
    {
        /* init otpReadInfo */
        retVal                = drv_memset(&otpReadInfo, 0, sizeof(otpReadInfo));
        otpReadInfo.addr      = addr;
        otpReadInfo.buf       = buf;
        otpReadInfo.len       = len;
        otpReadInfo.startAddr = otpReadInfo.addr >> 2u;
        otpReadInfo.endAddr   = (otpReadInfo.addr + otpReadInfo.len - 1u) >> 2u;
        otpReadInfo.mode      = CVA_OTP_MODE_READ;
        otpReadInfo.testMode  = CVA_OTP_ACCESS_MAIN;

        retLockVal = cva_drv_otp_lock(unit);
        if(RT_ERR_OK == retLockVal)
        {
            if(otpReadInfo.startAddr == otpReadInfo.endAddr)
            {
                /* Read one 4byte address data */
                CHK_FUN_RET(retVal, cva_drv_otp_basic_read(unit, &otpReadInfo));
            }
            else
            {
                /* Read one 4byte address data */
                CHK_FUN_RET(retVal, cva_drv_otp_pcs_read(unit, &otpReadInfo));
            }
            retLockVal = cva_drv_otp_unlock(unit);
        }
        else
        {
            /* do nothing */
        }

        if(RT_ERR_OK == retVal)
        {
            retVal = retLockVal;
        }
        else
        {
            /* do nothing */
        }
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_pgm_core0(uint32 unit, const cvaOtpPgmInfo_t *const otpPgmInfo, cvaOtpMarginReadInfo_t *otpMarginReadInfo)
{
    RtkApiRet retVal;

    /* Step 3. PGM_Marginal_Read_Customer_Code (VddStd, VddHi, VddLo) */
    otpMarginReadInfo->mode = CVA_OTP_MODE_RT_PGM_MARGIN_READ;
    retVal                  = drv_memset(&(otpMarginReadInfo->data), 0, sizeof(otpMarginReadInfo->data));
    CHK_FUN_RET(retVal, cva_drv_otp_margin_read(unit, otpMarginReadInfo));
    if((RT_ERR_OK == retVal) && ((otpMarginReadInfo->data[0] != otpPgmInfo->newData) || (otpMarginReadInfo->data[1] != otpPgmInfo->newData) || (otpMarginReadInfo->data[2] != otpPgmInfo->newData)))
    {
        retVal = -RT_ERR_OTP_PGM_MARGIN_READ_CMP_XC_VA;
    }
    else
    {
        /* Step 4. Normal Read to check the data */
        otpMarginReadInfo->mode = CVA_OTP_MODE_READ;
        CHK_FUN_RET(retVal, drv_memset(&(otpMarginReadInfo->data), 0, sizeof(otpMarginReadInfo->data)));
        CHK_FUN_RET(retVal, cva_drv_otp_margin_read(unit, otpMarginReadInfo));
        if((RT_ERR_OK == retVal) && ((otpMarginReadInfo->data[0] != otpPgmInfo->newData) || (otpMarginReadInfo->data[1] != otpPgmInfo->newData) || (otpMarginReadInfo->data[2] != otpPgmInfo->newData) || (otpMarginReadInfo->data[3] != otpPgmInfo->newData)))
        {
            retVal = -RT_ERR_OTP_BASIC_READ_CMP_XC_VA;
        }
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_pgm(uint32 unit, cvaOtpPgmInfo_t *otpPgmInfo)
{
    RtkApiRet              retVal;
    cvaOtpBasicPgmInfo_t   otpBasicPgmInfo;
    cvaOtpMarginReadInfo_t otpMarginReadInfo;

    retVal = drv_memset(&otpBasicPgmInfo, 0, sizeof(otpBasicPgmInfo));
    CHK_FUN_RET(retVal, drv_memset(&otpMarginReadInfo, 0, sizeof(otpMarginReadInfo)));

    /* Step 1. PGM_Customer_Code (VddStd) */
    otpBasicPgmInfo.testMode = CVA_OTP_ACCESS_MAIN;
    otpBasicPgmInfo.mode     = CVA_OTP_MODE_PGM;
    otpBasicPgmInfo.iAddr    = otpPgmInfo->iAddr;
    otpBasicPgmInfo.data     = otpPgmInfo->pgmData;
    CHK_FUN_RET(retVal, cva_drv_otp_basic_pgm(unit, &otpBasicPgmInfo));

    /* Step 2. Initial_Margin_Read_Customer_Code (VddStd, VddHi, VddLo) */
    otpMarginReadInfo.testMode = CVA_OTP_ACCESS_MAIN;
    otpMarginReadInfo.mode     = CVA_OTP_MODE_RT_INIT_MARGIN_READ;
    otpMarginReadInfo.iAddr    = otpPgmInfo->iAddr;
    CHK_FUN_RET(retVal, cva_drv_otp_margin_read(unit, &otpMarginReadInfo));
    if((RT_ERR_OK == retVal) && ((otpMarginReadInfo.data[0] != otpPgmInfo->newData) || (otpMarginReadInfo.data[1] != otpPgmInfo->newData) || (otpMarginReadInfo.data[2] != otpPgmInfo->newData)))
    {
        retVal = -RT_ERR_OTP_INIT_MARGIN_READ_CMP_XC_VA;
    }
    else
    {
        CHK_FUN_RET(retVal, cva_drv_otp_pgm_core0(unit, otpPgmInfo, &otpMarginReadInfo));
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_write_core2(const cvaOtpWriteInfo_t *const otpWriteInfo, const uint32 iAddr, const uint32 startAddrIdx, const uint32 endAddrIdx, uint32 *const bufIdx, uint32 *const wData, uint32 *const onyByteMask)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i;
    uint32    i1;
    uint32    i2;

    /* Get the 4byte wData from 1byte buf */
    *wData = (uint32)0x0u;
    if(otpWriteInfo->startAddr == otpWriteInfo->endAddr)
    {
        i1 = startAddrIdx;
        i2 = endAddrIdx;
    }
    else if(iAddr == otpWriteInfo->startAddr)
    {
        i1 = startAddrIdx;
        i2 = (uint32)3u;
    }
    else if(iAddr == otpWriteInfo->endAddr)
    {
        i1 = (uint32)0u;
        i2 = endAddrIdx;
    }
    else
    {
        i1 = 0u;
        i2 = 3u;
    }

    if((uint32)0u != i1)
    {
        /* padding 0xFF */
        for(i = 0u; i < i1; i++)
        {
            *wData |= (uint32)0xFFu << ((uint32)8u * i);
        }
    }

    if((uint32)3u != i2)
    {
        if((i2 + (uint32)1u) > (uint32)3u)
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            /* padding 0xFF */
            for(i = i2 + (uint32)1u; i < (uint32)4u; i++)
            {
                *wData |= (uint32)0xFFu << ((uint32)8u * i);
            }
        }
    }

    *onyByteMask = (uint32)0x0u;
    for(i = i1; i <= i2; i++)
    {
        *onyByteMask |= (uint32)0xFFu << ((uint32)8u * i);
        *wData |= (uint32)otpWriteInfo->buf[*bufIdx] << ((uint32)8u * i);
        if((*bufIdx) > (otpWriteInfo->len - 1u))
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            (*bufIdx)++;
        }
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_write_core1(uint32 unit, const cvaOtpWriteInfo_t *const otpWriteInfo)
{
    RtkApiRet        retVal = RT_ERR_OK;
    uint32           iAddr;
    uint32           startAddrIdx = otpWriteInfo->addr % 4u;
    uint32           endAddrIdx   = otpWriteInfo->addr % 4u;
    uint32           bufIdx       = 0u;
    uint32           data         = 0u;
    uint32           wData        = 0u;
    uint32           onyByteMask  = 0u;
    cvaOtpReadInfo_t otpReadInfo;
    cvaOtpPgmInfo_t  otpPgmInfo;

    retVal = drv_memset(&otpReadInfo, 0, sizeof(otpReadInfo));
    CHK_FUN_RET(retVal, drv_memset(&otpPgmInfo, 0, sizeof(otpPgmInfo)));

    if((0u == otpWriteInfo->len) || (otpWriteInfo->len > CVA_OTP_BYTE_CNT) || (otpWriteInfo->addr > CVA_OTP_BYTE_END_ADDR) || ((otpWriteInfo->addr + otpWriteInfo->len) > (CVA_OTP_BYTE_END_ADDR + 1u)))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        endAddrIdx = (otpWriteInfo->addr + otpWriteInfo->len - 1u) % 4u;

        for(iAddr = otpWriteInfo->startAddr; iAddr <= otpWriteInfo->endAddr; iAddr++)
        {
            otpPgmInfo.iAddr = iAddr;
            /* Get the 4byte wData from 1byte buf */
            CHK_FUN_RET(retVal, cva_drv_otp_write_core2(otpWriteInfo, iAddr, startAddrIdx, endAddrIdx, &bufIdx, &wData, &onyByteMask));

            /* Calculate opData and write to OTP */
            otpReadInfo.testMode  = CVA_OTP_ACCESS_MAIN;
            otpReadInfo.mode      = CVA_OTP_MODE_READ;
            otpReadInfo.addr      = iAddr << 2u;
            otpReadInfo.buf       = (uint8 *)&data;
            otpReadInfo.len       = 4u;
            otpReadInfo.startAddr = iAddr;
            otpReadInfo.endAddr   = otpReadInfo.startAddr;
            CHK_FUN_RET(retVal, cva_drv_otp_basic_read(unit, &otpReadInfo));
            /* New data after programming */
            otpPgmInfo.newData = wData & data;
            /* Calculate real input data */
            otpPgmInfo.pgmData = otpPgmInfo.newData | (~data);

            if(RT_ERR_OK == retVal)
            {
                if((wData & onyByteMask) == (data & onyByteMask))
                {
                    /* bypass RT_ERR_OTP_PGM_SAME_DATA */
                }
                else if((wData & onyByteMask) == (otpPgmInfo.newData & onyByteMask))
                {
                    /* wData is equal to newData */
                    retVal = cva_drv_otp_pgm(unit, &otpPgmInfo);
                }
                else
                {
                    retVal = -RT_ERR_OTP_PGM_INVALID_DATA_XC_VA;
                }
            }

            if(RT_ERR_OK != retVal)
            {
                break;
            }
        }
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_write_core(uint32 unit, const cvaOtpWriteInfo_t *const otpWriteInfo)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkApiRet retLockVal;

    retLockVal = cva_drv_otp_lock(unit);
    if(RT_ERR_OK == retLockVal)
    {
        CHK_FUN_RET(retVal, cva_drv_otp_write_core1(unit, otpWriteInfo));
        retLockVal = cva_drv_otp_unlock(unit);
    }

    if(RT_ERR_OK == retVal)
    {
        retVal = retLockVal;
    }

    return retVal;
}

static RtkApiRet cva_drv_otp_write_input_chk(cvaOtpWriteInfo_t *const otpWriteInfo)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((0u == otpWriteInfo->len) || (otpWriteInfo->len > CVA_OTP_BYTE_CNT) || (otpWriteInfo->addr > CVA_OTP_BYTE_END_ADDR) || ((otpWriteInfo->addr + otpWriteInfo->len) > (CVA_OTP_BYTE_END_ADDR + 1u)))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        otpWriteInfo->endAddr = (otpWriteInfo->addr + otpWriteInfo->len - 1u) >> 2u;
        if(otpWriteInfo->endAddr > CVA_OTP_4BYTE_END_ADDR)
        {
            retVal = -RT_ERR_INPUT;
        }
    }

    return retVal;
}

RtkApiRet cva_drv_otp_dir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet         retVal;
    cvaOtpWriteInfo_t otpWriteInfo;

    retVal                 = drv_memset(&otpWriteInfo, 0, sizeof(otpWriteInfo));
    otpWriteInfo.testMode  = CVA_OTP_ACCESS_MAIN;
    otpWriteInfo.mode      = CVA_OTP_MODE_PGM;
    otpWriteInfo.addr      = addr;
    otpWriteInfo.buf       = buf;
    otpWriteInfo.len       = len;
    otpWriteInfo.startAddr = otpWriteInfo.addr >> 2u;
    otpWriteInfo.endAddr   = 0u;
    otpWriteInfo.retVal    = RT_ERR_OK;

    CHK_FUN_RET(retVal, cva_drv_otp_write_input_chk(&otpWriteInfo));
    CHK_FUN_RET(retVal, cva_drv_otp_write_core(unit, &otpWriteInfo));

    return retVal;
}