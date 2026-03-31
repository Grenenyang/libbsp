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
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_asicdrv_otp.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"

uint32 cvb_g_otpTimeoutCnt = RTK_TIMER_OTP_INDIRECT_CHANNEL;

static RtkApiRet cvb_drv_otp_indir_retval_get(uint32 unit, uint32 indirCmdAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, indirCmdAddr, 0u, 24u, &regVal));
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

static RtkApiRet cvb_drv_otp_indir_store(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(CVB_OTP_CMD_OP_PGM == op)
    {
        /* [in]: op
         * [in]: addr
         * [in]: len
         * [in]: data[3..0]
         * Write 1/.../16byte data {[7:0] of data[0] / ... / [31:24] of data[3]}
         */
        CHK_FUN_RET(retVal, cvb_reg_write(unit, indirDataAddr[0], data[0]));
        CHK_FUN_RET(retVal, cvb_reg_write(unit, indirDataAddr[1], data[1]));
        CHK_FUN_RET(retVal, cvb_reg_write(unit, indirDataAddr[2], data[2]));
        CHK_FUN_RET(retVal, cvb_reg_write(unit, indirDataAddr[3], data[3]));
    }
    else if(CVB_OTP_CMD_OP_CHKSUM_CAL == op)
    {
        /* [in]: op
         * [in]: addr
         * [in]: len
         * [in]: only sum if [31] of data[0] is 1
         * [out]: calculated checksum in [7:0] of data[0]
         * [reserved]: [30:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cvb_reg_write(unit, indirDataAddr[0], data[0]));
    }
    else if(CVB_OTP_CMD_OP_CHKSUM_PGM == op)
    {
        /* [in]: op
         * [in]: testing
         * [in]: chksum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cvb_reg_write(unit, indirDataAddr[0], data[0]));
    }
    else if(CVB_OTP_CMD_OP_CHKSUM_VERIFY == op)
    {
        /* [in]: op
         * [in]: testing
         * [in]: sum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cvb_reg_write(unit, indirDataAddr[0], data[0]));
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

static RtkApiRet cvb_drv_otp_indir_trigger(const uint32 unit, const uint32 indirCmdAddr)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    indirCmd = 0x0u;
    uint32    regVal   = 0x0u;
    uint32    tryTimes = 0u;

    indirCmd = ((uint32)1u << CVB_OTP_INDIRECT_ACCESS_TRIGGER) | ((uint32)1u << CVB_OTP_INDIRECT_ACCESS_READ) | (CVB_OTP_INDIRECT_ACCESS_TARGET_OTP << CVB_OTP_INDIRECT_ACCESS_TARGET);
    CHK_FUN_RET(retVal, cvb_reg_write(unit, indirCmdAddr, indirCmd));

    /* Polling indir channel done */
    do
    {
        if(tryTimes++ > cvb_g_otpTimeoutCnt)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            break;
        }
        CHK_FUN_RET(retVal, cvb_reg_bit_get(unit, indirCmdAddr, CVB_OTP_INDIRECT_ACCESS_TRIGGER, &regVal));
        CHK_FUN_RET(retVal, rtk_usecond_delay(CVB_OTP_INDIRECT_ACCESS_TRIGGER_DELAY));
    } while(0u != regVal);

    return retVal;
}

static RtkApiRet cvb_drv_otp_indir_load_0(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(CVB_OTP_CMD_OP_LOCKINFO_GET == op)
    {
        /* CVB_OTP_CMD_OP_LOCKINFO_GET
         * [in]: op
         * [in]: addr
         * [out]: otpLockAddr in [15:0] of data[0]
         * [out]: mcuLockAddr in [31:16] of data[0]
         * [out]: otpLockBit#: [5:0] of data[1]
         * [out]: mcuLockBit#: [21:16] of data[1]
         * [reserved]: [15:6]/[31:22] of data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cvb_reg_read(unit, indirDataAddr[0], &data[0]));
        CHK_FUN_RET(retVal, cvb_reg_read(unit, indirDataAddr[1], &data[1]));
    }
    else if((CVB_OTP_CMD_OP_CHKSUM_CAL == op) || (CVB_OTP_CMD_OP_CHKSUM_VERIFY == op))
    {
        /* CVB_OTP_CMD_OP_CHKSUM_CAL
         * [in]: op
         * [in]: addr
         * [in]: len
         * [in]: only sum if [31] of data[0] is 1
         * [out]: calculated checksum in [7:0] of data[0]
         * [reserved]: [30:8] of data[0], data[1], data[2], data[3]
         *
         * CVB_OTP_CMD_OP_CHKSUM_VERIFY
         * [in]: op
         * [in]: testing
         * [in]: sum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        CHK_FUN_RET(retVal, cvb_reg_read(unit, indirDataAddr[0], &data[0]));
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

static RtkApiRet cvb_drv_otp_indir_load(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((CVB_OTP_CMD_OP_MAIN_CHK == op) || ((op >= CVB_OTP_CMD_OP_READ) && (op <= CVB_OTP_CMD_OP_HT_PGM_MARGIN_READ)))
    {
        /*
         * CVB_OTP_CMD_OP_MAIN_CHK
         * [out]: ngFlag in [0] of data[0]
         * [out]: ngKill in [1] of data[0]
         * [out]: ngCnt in data[1]
         * [out]: ngAddr0 in data[2]
         * [out]: ngAddr1 in data[3]
         *
         * CVB_OTP_CMD_OP_READ
         * [in]: op
         * [in]: addr
         * [in]: len
         * [out]: data[3..0]
         * read 1/.../16byte data into {[7:0] of data[0]/.../[31:24] of data[3]}
         *
         * CVB_OTP_CMD_OP_READ/CVB_OTP_CMD_OP_RT_READ / OTP_CMD_OP_RT/HT_INIT_MARGIN_READ / OTP_CMD_OP_RT/HT_PGM_MARGIN_READ
         * [in]: op
         * [in]: addr
         * [out]: data[3..0]
         * read 4byte data into data[3..0]
         *
         * OTP_CMD_OP_CHECKSUM_CAL
         * [out]: calculated checksum in [7:0] of data[0]
         */
        CHK_FUN_RET(retVal, cvb_reg_read(unit, indirDataAddr[0], &data[0]));
        CHK_FUN_RET(retVal, cvb_reg_read(unit, indirDataAddr[1], &data[1]));
        CHK_FUN_RET(retVal, cvb_reg_read(unit, indirDataAddr[2], &data[2]));
        CHK_FUN_RET(retVal, cvb_reg_read(unit, indirDataAddr[3], &data[3]));
    }
    else
    {
        CHK_FUN_RET(retVal, cvb_drv_otp_indir_load_0(unit, op, indirDataAddr, data));
    }

    return retVal;
}

static RtkApiRet cvb_drv_otp_indir_ch_get(uint32 ch, uint32 *indirCmdAddr, uint32 *otpCmdAddr, uint32 *indirDataAddr)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(0u == ch)
    {
        *indirCmdAddr    = CVB_OTP_INDIRECT_ACCESS_COMMAND_0;
        *otpCmdAddr      = CVB_OTP_INDIRECT_ACCESS_ADDR_0;
        indirDataAddr[0] = CVB_OTP_INDIRECT_ACCESS_DATA_0;
        indirDataAddr[1] = CVB_OTP_INDIRECT_ACCESS_OTP_DATA1_0;
        indirDataAddr[2] = CVB_OTP_INDIRECT_ACCESS_OTP_DATA2_0;
        indirDataAddr[3] = CVB_OTP_INDIRECT_ACCESS_OTP_DATA3_0;
    }
    else if(1u == ch)
    {
        *indirCmdAddr    = CVB_OTP_INDIRECT_ACCESS_COMMAND_1;
        *otpCmdAddr      = CVB_OTP_INDIRECT_ACCESS_ADDR_1;
        indirDataAddr[0] = CVB_OTP_INDIRECT_ACCESS_DATA_1;
        indirDataAddr[1] = CVB_OTP_INDIRECT_ACCESS_OTP_DATA1_1;
        indirDataAddr[2] = CVB_OTP_INDIRECT_ACCESS_OTP_DATA2_1;
        indirDataAddr[3] = CVB_OTP_INDIRECT_ACCESS_OTP_DATA3_1;
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
DYNAMIC_SCOPE RtkApiRet cvb_drv_otp_indir_access(const uint32 unit, const uint32 ch, const uint32 cmd, uint32 *data)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    op               = cmd & CVB_OTP_CMD_OP_MASK;
    uint32    indirCmdAddr     = 0x0u;
    uint32    otpCmdAddr       = 0x0u;
    uint32    indirDataAddr[4] = {0};

    if((ch >= CVB_OTP_INDIRECT_CHANNEL_NUM) || (op >= CVB_OTP_CMD_OP_END))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if((NULL == data) && ((op == CVB_OTP_CMD_OP_MAIN_CHK) || (op >= CVB_OTP_CMD_OP_PGM)))
    {
        /* data shall be used */
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get channel register addresses */
    CHK_FUN_RET(retVal, cvb_drv_otp_indir_ch_get(ch, &indirCmdAddr, &otpCmdAddr, indirDataAddr));

    /* Set cmd to OTP command reg */
    CHK_FUN_RET(retVal, cvb_reg_write(unit, otpCmdAddr, cmd));

    /* Store data in indir data reg before trigger indir channel command for program operation*/
    if(NULL != data)
    {
        CHK_FUN_RET(retVal, cvb_drv_otp_indir_store(unit, op, indirDataAddr, data));
    }

    /* Trigger indir channel command */
    CHK_FUN_RET(retVal, cvb_drv_otp_indir_trigger(unit, indirCmdAddr));
    /* Load indir data reg to data after trigger indir channel cmd for read operation */
    if(NULL != data)
    {
        CHK_FUN_RET(retVal, cvb_drv_otp_indir_load(unit, op, indirDataAddr, data));
    }

    /* Get error code from indir cmd reg */
    CHK_FUN_RET(retVal, cvb_drv_otp_indir_retval_get(unit, indirCmdAddr));

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
RtkApiRet cvb_drv_otp_write_err_chk(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((0u == len) || (addr > CVB_OTP_BYTE_END_ADDR) || (len > CVB_OTP_BYTE_CNT) || (addr > (CVB_OTP_BYTE_CNT - len)))
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
RtkApiRet cvb_drv_otp_read_err_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((0u == len) || (addr > CVB_OTP_BYTE_END_ADDR) || (len > CVB_OTP_BYTE_CNT) || (addr > (CVB_OTP_BYTE_CNT - len)))
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
 * @brief Check the firmware status for indirect access channel
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @retval RT_ERR_OK                The firmware status read successfully.
 * @retval -RT_ERR_NULL_POINTER     The input paraemter is NULL.
 * @retval -RT_ERR_OTP_INDIR_ACCESS The indirect access is failed
 */
RtkApiRet cvb_drv_otp_fw_status_chk(uint32 unit)
{
    RtkApiRet retVal;
    uint8     fwStatus;
    uint32    regVal;

    /* Get the dummy register */
    retVal = cvb_reg_read(unit, CVB_CPU_DMY_1, &regVal);

    if(RT_ERR_OK == retVal)
    {
        /* Get the firmware status */
        fwStatus = (uint8)((regVal & CVB_OTP_INDIR_ACCESS_FW_STATUS_MASK) >> CVB_OTP_INDIR_ACCESS_FW_STATUS_STARTBIT);
        /* Check the firmware status */
        if((CVB_OTP_INDIR_ACCESS_FW_STATUS == fwStatus) || (CVB_OTP_FLASHLESS_FW_STATUS0 == fwStatus) || (CVB_OTP_FLASHLESS_FW_STATUS1 == fwStatus))
        {
            /* The indirect access operation is working normally */
        }
        else
        {
            /* The indirect access operation is failed */
            retVal = -RT_ERR_OTP_INDIR_ACCESS;
        }
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
RtkApiRet cvb_drv_otp_indir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    iBuf         = 0u;
    uint32    iIndirData   = 0u;
    uint32    indirCmd     = 0u;
    uint32    indirAddr    = 0u;
    uint32    indirLen     = 0u;
    uint32    indirData[4] = {0u};

    for(iBuf = 0u; iBuf < len; iBuf += CVB_OTP_INDIRECT_ACCESS_BUF_SIZE)
    {
        if(addr <= (CVB_OTP_BYTE_CNT - iBuf))
        {
            indirAddr = addr + iBuf;
            if((iBuf + CVB_OTP_INDIRECT_ACCESS_BUF_SIZE) <= len)
            {
                indirLen = CVB_OTP_INDIRECT_ACCESS_BUF_SIZE;
            }
            else
            {
                /* Write OTP for last round */
                indirLen = len - iBuf;
            }
        }
        else
        {
            retVal = RT_ERR_INPUT;
        }

        /* Cast buf to indirData */
        CHK_FUN_RET(retVal, cvb_drv_drv_memset(indirData, 0, sizeof(indirData)));
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
            indirCmd = CVB_OTP_CMD_OP_PGM | (indirAddr << CVB_OTP_CMD_ADDR_LSP) | (indirLen << CVB_OTP_CMD_LEN_LSP);
            CHK_FUN_RET(retVal, cvb_drv_otp_indir_access(unit, 0u, indirCmd, indirData));
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
RtkApiRet cvb_drv_otp_indir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal;
    uint32    iBuf         = 0u;
    uint32    iIndirData   = 0u;
    uint32    indirCmd     = 0u;
    uint32    indirAddr    = 0u;
    uint32    indirLen     = 0u;
    uint32    indirData[4] = {0u};

    /* Other modules read OTP indirectly in the normal mode */
    for(iBuf = 0u; iBuf < len; iBuf += CVB_OTP_INDIRECT_ACCESS_BUF_SIZE)
    {
        if(addr <= (CVB_OTP_BYTE_CNT - iBuf))
        {
            indirAddr = addr + iBuf;
            if((iBuf + CVB_OTP_INDIRECT_ACCESS_BUF_SIZE) <= len)
            {
                indirLen = CVB_OTP_INDIRECT_ACCESS_BUF_SIZE;
            }
            else
            {
                /* Read OTP for last round */
                indirLen = len - iBuf;
            }

            /* Indir access */
            if(0u == iBuf)
            {
                retVal = cvb_drv_drv_memset(indirData, 0, sizeof(indirData));
            }
            else
            {
                CHK_FUN_RET(retVal, cvb_drv_drv_memset(indirData, 0, sizeof(indirData)));
            }
            indirCmd = CVB_OTP_CMD_OP_READ | (indirAddr << CVB_OTP_CMD_ADDR_LSP) | (indirLen << CVB_OTP_CMD_LEN_LSP);
            CHK_FUN_RET(retVal, cvb_drv_otp_indir_access(unit, 0u, indirCmd, indirData));

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
            retVal = RT_ERR_INPUT;
        }
    }

    return retVal;
}
