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
#include "rtkas_common.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_sys_boot.h"
#include "rtl907xd_va_asicdrv_otp.h"
#include "rtl907xd_va_asicdrv_timer.h"
#include "rtl907xd_va_api_esm.h"

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
RtkApiRet dva_drv_otp_write_err_chk(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((0u == len) || (addr > DVA_OTP_BYTE_END_ADDR) || (len > DVA_OTP_BYTE_CNT) || (addr > (DVA_OTP_BYTE_CNT - len)))
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
RtkApiRet dva_drv_otp_read_err_chk(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((0u == len) || (addr > DVA_OTP_BYTE_END_ADDR) || (len > DVA_OTP_BYTE_CNT) || (addr > (DVA_OTP_BYTE_CNT - len)))
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
RtkApiRet dva_drv_otp_fw_status_chk(uint32 unit)
{
    RtkApiRet retVal;
    uint8     fwStatus;
    uint32    regVal;

    /* Get the dummy register */
    retVal = dva_reg_read(unit, DVA_CPU_DMY_1, &regVal);

    if(RT_ERR_OK == retVal)
    {
        /* Get the firmware status */
        fwStatus = (uint8)((regVal & DVA_OTP_INDIR_ACCESS_FW_STATUS_MASK) >> DVA_OTP_INDIR_ACCESS_FW_STATUS_STARTBIT);
        /* Check the firmware status */
        if((DVA_OTP_INDIR_ACCESS_FW_STATUS == fwStatus) || (DVA_SYS_BOOT_UP_FLASHLESS == fwStatus) || (DVA_SYS_BOOT_UP_FLASHLESS_ETH == fwStatus))
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

static RtkApiRet dva_drv_otp_indir_err_get(uint32 unit, uint32 indirCmdAddr)
{
    RtkApiRet retVal;
    uint32    regVal;

    retVal = dva_reg_field_read(unit, indirCmdAddr, DVA_OTP_INDIRECT_ACCESS_ERR_LSP, DVA_OTP_INDIRECT_ACCESS_ERR_LEN, &regVal);
    if((RT_ERR_OK == retVal) && (regVal <= DVA_OTP_INDIRECT_ACCESS_ERR_MAX))
    {
        retVal = -(RtkApiRet)regVal;
    }

    return retVal;
}

static RtkApiRet dva_drv_otp_indir_store(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal;

    if(DVA_OTP_CMD_OP_PGM == op)
    {
        /* [in]: op
         * [in]: addr
         * [in]: len
         * [in]: data[3..0]
         * Write 1/.../16byte data {[7:0] of data[0] / ... / [31:24] of data[3]}
         */
        retVal = dva_reg_write(unit, indirDataAddr[0], data[0]);
        CHK_FUN_RET(retVal, dva_reg_write(unit, indirDataAddr[1], data[1]));
        CHK_FUN_RET(retVal, dva_reg_write(unit, indirDataAddr[2], data[2]));
        CHK_FUN_RET(retVal, dva_reg_write(unit, indirDataAddr[3], data[3]));
    }
    else if(DVA_OTP_CMD_OP_CHKSUM_CAL == op)
    {
        /* [in]: op
         * [in]: addr
         * [in]: len
         * [in]: only sum if [31] of data[0] is 1
         * [out]: calculated checksum in [7:0] of data[0]
         * [reserved]: [30:8] of data[0], data[1], data[2], data[3]
         */
        retVal = dva_reg_write(unit, indirDataAddr[0], data[0]);
    }
    else if(DVA_OTP_CMD_OP_CHKSUM_PGM == op)
    {
        /* [in]: op
         * [in]: testing
         * [in]: chksum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        retVal = dva_reg_write(unit, indirDataAddr[0], data[0]);
    }
    else if(DVA_OTP_CMD_OP_CHKSUM_VERIFY == op)
    {
        /* [in]: op
         * [in]: testing
         * [in]: sum in [7:0] of data[0]
         * [reserved]: [31:8] of data[0], data[1], data[2], data[3]
         */
        retVal = dva_reg_write(unit, indirDataAddr[0], data[0]);
    }
    else
    {
        retVal = RT_ERR_OK;
    }

    return retVal;
}

static RtkApiRet dva_drv_otp_indir_trigger(const uint32 unit, const uint32 indirCmdAddr)
{
    RtkApiRet          retVal;
    uint32             indirCmd = ((uint32)1u << DVA_OTP_INDIRECT_ACCESS_READ) | (DVA_OTP_INDIRECT_ACCESS_TARGET_OTP << DVA_OTP_INDIRECT_ACCESS_TARGET);
    RegPollingConfig_t pollingConf;

    /* Initialize struct */
    pollingConf.triggerRegAddr = indirCmdAddr;
    pollingConf.triggerRegLsp  = DVA_OTP_INDIRECT_ACCESS_TRIGGER;
    pollingConf.triggerRegLen  = DVA_OTP_INDIRECT_ACCESS_TRIGGER_LEN;
    pollingConf.pollingRegAddr = indirCmdAddr;
    pollingConf.pollingRegLsp  = DVA_OTP_INDIRECT_ACCESS_TRIGGER;
    pollingConf.pollingRegLen  = DVA_OTP_INDIRECT_ACCESS_TRIGGER_LEN;
    pollingConf.action         = DVA_OTP_INDIRECT_ACCESS_TRIGGER_ACTION;
    pollingConf.expVal         = DVA_OTP_INDIRECT_ACCESS_TRIGGER_EXPVAL;
    pollingConf.port           = 0u;
    pollingConf.chkPortClk     = 0u;
    pollingConf.eventID        = DVA_EM_EVENT_OTP_POLLING_TIMEOUT;

    retVal = dva_reg_write(unit, indirCmdAddr, indirCmd);
    CHK_FUN_RET(retVal, dva_drv_timer_polling_reg_timeout(unit, &pollingConf, DVA_OTP_INDIRECT_ACCESS_TRIGGER_MAXNS, DVA_OTP_INDIRECT_ACCESS_TRIGGER_MINNS));

    return retVal;
}

static RtkApiRet dva_drv_otp_indir_load_0(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal;

    if(DVA_OTP_CMD_OP_CHKSUM_CAL == op)
    {
        /* DVA_OTP_CMD_OP_CHKSUM_CAL
         * [in]: op
         * [in]: addr
         * [in]: len
         * [in]: only sum if [31] of data[0] is 1
         * [out]: calculated checksum in [7:0] of data[0]
         * [reserved]: [30:8] of data[0], data[1], data[2], data[3]
         */
        retVal = dva_reg_read(unit, indirDataAddr[0], &data[0]);
    }
    else
    {
        retVal = RT_ERR_OK;
    }

    return retVal;
}

static RtkApiRet dva_drv_otp_indir_load(const uint32 unit, const uint32 op, const uint32 *indirDataAddr, uint32 *data)
{
    RtkApiRet retVal;

    if((DVA_OTP_CMD_OP_MAIN_CHK == op) || ((op >= DVA_OTP_CMD_OP_READ) && (op <= DVA_OTP_CMD_OP_PGM_MARGIN_READ)))
    {
        /*
         * DVA_OTP_CMD_OP_MAIN_CHK
         * [out]: ngFlag in [0] of data[0]
         * [out]: ngKill in [1] of data[0]
         * [out]: ngCnt in data[1]
         * [out]: ngAddr0 in data[2]
         * [out]: ngAddr1 in data[3]
         *
         * DVA_OTP_CMD_OP_READ
         * [in]: op
         * [in]: addr
         * [in]: len
         * [out]: data[3..0]
         * read 1/.../16byte data into {[7:0] of data[0]/.../[31:24] of data[3]}
         *
         * DVA_OTP_CMD_OP_READ/OTP_CMD_OP_RT_READ / OTP_CMD_OP_RT/HT_INIT_MARGIN_READ / OTP_CMD_OP_RT/HT_PGM_MARGIN_READ
         * [in]: op
         * [in]: addr
         * [out]: data[3..0]
         * read 4byte data into data[3..0]
         *
         * OTP_CMD_OP_CHECKSUM_CAL
         * [out]: calculated checksum in [7:0] of data[0]
         */
        retVal = dva_reg_read(unit, indirDataAddr[0], &data[0]);
        CHK_FUN_RET(retVal, dva_reg_read(unit, indirDataAddr[1], &data[1]));
        CHK_FUN_RET(retVal, dva_reg_read(unit, indirDataAddr[2], &data[2]));
        CHK_FUN_RET(retVal, dva_reg_read(unit, indirDataAddr[3], &data[3]));
    }
    else
    {
        retVal = dva_drv_otp_indir_load_0(unit, op, indirDataAddr, data);
    }

    return retVal;
}

static RtkApiRet dva_drv_otp_indir_ch_get(uint32 ch, uint32 *indirCmdAddr, uint32 *otpCmdAddr, uint32 *indirDataAddr)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(0u == ch)
    {
        *indirCmdAddr    = DVA_OTP_INDIRECT_ACCESS_COMMAND_0;
        *otpCmdAddr      = DVA_OTP_INDIRECT_ACCESS_ADDR_0;
        indirDataAddr[0] = DVA_OTP_INDIRECT_ACCESS_DATA_0;
        indirDataAddr[1] = DVA_OTP_INDIRECT_ACCESS_OTP_DATA1_0;
        indirDataAddr[2] = DVA_OTP_INDIRECT_ACCESS_OTP_DATA2_0;
        indirDataAddr[3] = DVA_OTP_INDIRECT_ACCESS_OTP_DATA3_0;
    }
    else if(1u == ch)
    {
        *indirCmdAddr    = DVA_OTP_INDIRECT_ACCESS_COMMAND_1;
        *otpCmdAddr      = DVA_OTP_INDIRECT_ACCESS_ADDR_1;
        indirDataAddr[0] = DVA_OTP_INDIRECT_ACCESS_DATA_1;
        indirDataAddr[1] = DVA_OTP_INDIRECT_ACCESS_OTP_DATA1_1;
        indirDataAddr[2] = DVA_OTP_INDIRECT_ACCESS_OTP_DATA2_1;
        indirDataAddr[3] = DVA_OTP_INDIRECT_ACCESS_OTP_DATA3_1;
    }
    else
    {
        retVal = -RT_ERR_INPUT;
    }

    return retVal;
}

/**
 * @brief Check the input parameters for the indirect access OTP
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ch indirect channel number
 * @param[in] cmd indirect channel command
 * @param[in] data Read back data for read operation
 * @param[out] data Written data for write operation
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 * @note op = cmd[4:0] / addr = cmd[31:16] / len = cmd[15:11] / testing = cmd[10:9] / reserved = cmd[8:5] / [31] of data[0] = sum(1)/checksum(0)
 */
static RtkApiRet dva_drv_otp_indir_access_err_chk(const uint32 ch, const uint32 cmd, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    op     = cmd & DVA_OTP_CMD_OP_MASK;

    if((ch >= DVA_OTP_INDIRECT_CHANNEL_NUM) || (op > DVA_OTP_CMD_OP_END))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == data)
    {
        if(((op == DVA_OTP_CMD_OP_MAIN_CHK) || ((op >= DVA_OTP_CMD_OP_PGM) && (op <= DVA_OTP_CMD_OP_PGM_MARGIN_READ))) || (op >= DVA_OTP_CMD_OP_CHKSUM_CAL))
        {
            /* data shall be used */
            retVal = -RT_ERR_NULL_POINTER;
        }
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
RtkApiRet dva_drv_otp_indir_access(const uint32 unit, const uint32 ch, const uint32 cmd, uint32 *data)
{
    RtkApiRet retVal;
    uint32    op = cmd & DVA_OTP_CMD_OP_MASK;
    uint32    indirCmdAddr;
    uint32    otpCmdAddr;
    uint32    indirDataAddr[4];

    /* Check the input parameters */
    retVal = dva_drv_otp_indir_access_err_chk(ch, cmd, data);

    /* Get channel register addresses */
    CHK_FUN_RET(retVal, dva_drv_otp_indir_ch_get(ch, &indirCmdAddr, &otpCmdAddr, indirDataAddr));

    /* Set cmd to OTP command reg */
    CHK_FUN_RET(retVal, dva_reg_write(unit, otpCmdAddr, cmd));

    /* Store data in indir data reg before trigger indir channel command for program operation*/
    if(NULL != data)
    {
        CHK_FUN_RET(retVal, dva_drv_otp_indir_store(unit, op, indirDataAddr, data));
    }

    /* Trigger indir channel command */
    CHK_FUN_RET(retVal, dva_drv_otp_indir_trigger(unit, indirCmdAddr));

    /* Load indir data reg to data after trigger indir channel cmd for read operation */
    if(NULL != data)
    {
        CHK_FUN_RET(retVal, dva_drv_otp_indir_load(unit, op, indirDataAddr, data));
    }

    /* Get error code from indir cmd reg */
    CHK_FUN_RET(retVal, dva_drv_otp_indir_err_get(unit, indirCmdAddr));

    return retVal;
}

/**
 * @brief Write OTP memory via the indirectly access
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] addr        Address
 * @param[in] buf         Data buffer
 * @param[in] len         Buffer length
 * @param[out] None * @retval RT_ERR_OK                          The OTP memory write successfully.
 * @retval -RT_ERR_INPUT                      The input paraemters are invalid.
 * @retval -RT_ERR_NULL_POINTER               The input buf is NULL.
 * @retval -RT_ERR_OTP_FSM_MAIN_TIMEOUT       The OTP main FSM polling timeout
 * @retval -RT_ERR_OTP_FSM_IF_TIMEOUT         The OTP intrface FSM polling timeout
 * @retval -RT_ERR_OTP_FSM_READY_TIMEOUT      The OTP ready FSM polling timeout
 * @retval -RT_ERR_OTP_INT                    The OTP interrupt polling timeout
 * @retval -RT_ERR_OTP_MISMATCH               The OTP data mismatch
 * @retval -RT_ERR_OTP_LOCK_ROLE_INVALID      The OTP lock role is invalid
 * @retval -RT_ERR_OTP_LOCK                   The OTP lock failed
 * @retval -RT_ERR_OTP_UNLOCK_ROLE_INVALID    The OTP unlock role is invalid
 * @retval -RT_ERR_OTP_UNLOCK                 The OTP unlock failed
 * @retval -RT_ERR_OTP_INIT_MARGIN_READ_CMP   The OTP initial margin read comparison results are inconsistent
 * @retval -RT_ERR_OTP_PGM_MARGIN_READ_CMP    The OTP program margin read comparison results are inconsistent
 * @retval -RT_ERR_OTP_BASIC_READ_CMP         The OTP initial margin read comparison results are inconsistent
 * @retval -RT_ERR_OTP_READ_LOCKDATA          The OTP read address is locked
 * @retval -RT_ERR_OTP_PGM_SAME_DATA          The programmed OTP data matches the current data
 * @retval -RT_ERR_OTP_PGM_INVALID_DATA       The programmed OTP data is invalid
 * @retval -RT_ERR_OTP_PGM_LOCKDATA           The programmed OTP address is locked
 * @retval -RT_ERR_OTP_DEADLOCK               The OTP resource is in a deadlock
 */
RtkApiRet dva_drv_otp_indir_write(uint32 unit, const uint32 addr, const uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    iBuf         = 0u;
    uint32    iIndirData   = 0u;
    uint32    indirCmd     = 0u;
    uint32    indirAddr    = 0u;
    uint32    indirLen     = 0u;
    uint32    indirData[4] = {0u};

    /* Set the operating temp to the room temp */
    indirCmd = DVA_OTP_CMD_OP_TEMP | (DVA_OTP_CMD_TEMP_RT << DVA_OTP_CMD_TEMP_LSP);
    retVal   = dva_drv_otp_indir_access(unit, 0u, indirCmd, NULL);

    for(iBuf = 0u; iBuf < len; iBuf += DVA_OTP_INDIRECT_ACCESS_BUF_SIZE)
    {
        if(addr <= (DVA_OTP_BYTE_CNT - iBuf))
        {
            indirAddr = addr + iBuf;
            if((iBuf + DVA_OTP_INDIRECT_ACCESS_BUF_SIZE) <= len)
            {
                indirLen = DVA_OTP_INDIRECT_ACCESS_BUF_SIZE;
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
        CHK_FUN_RET(retVal, dva_drv_memset(indirData, 0, sizeof(indirData)));
        if(RT_ERR_OK == retVal)
        {
            for(iIndirData = 0u; iIndirData < indirLen; iIndirData++)
            {
                if(((uint32)buf[iIndirData + iBuf] << (DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BIT * (iIndirData % DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BYTE))) <= DVA_OTP_INDIRECT_ACCESS_BUF_BYTE_DATA_MAX)
                {
                    indirData[iIndirData / 4u] |= (uint32)((uint32)buf[iIndirData + iBuf] << (DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BIT * (iIndirData % DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BYTE)));
                }
            }

            /* Indir access */
            indirCmd = DVA_OTP_CMD_OP_PGM | (indirAddr << DVA_OTP_CMD_ADDR_LSP) | (indirLen << DVA_OTP_CMD_LEN_LSP);
            CHK_FUN_RET(retVal, dva_drv_otp_indir_access(unit, 0u, indirCmd, indirData));
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
RtkApiRet dva_drv_otp_indir_read(uint32 unit, const uint32 addr, uint8 *const buf, const uint32 len)
{
    RtkApiRet retVal;
    uint32    iBuf         = 0u;
    uint32    iIndirData   = 0u;
    uint32    indirCmd     = 0u;
    uint32    indirAddr    = 0u;
    uint32    indirLen     = 0u;
    uint32    indirData[4] = {0u};

    /* Set the operating temp to the room temp */
    indirCmd = DVA_OTP_CMD_OP_TEMP | (DVA_OTP_CMD_TEMP_RT << DVA_OTP_CMD_TEMP_LSP);
    retVal   = dva_drv_otp_indir_access(unit, 0u, indirCmd, NULL);

    /* Other modules read OTP indirectly in the normal mode */
    for(iBuf = 0u; iBuf < len; iBuf += DVA_OTP_INDIRECT_ACCESS_BUF_SIZE)
    {
        if(addr <= (DVA_OTP_BYTE_CNT - iBuf))
        {
            indirAddr = addr + iBuf;
            if((iBuf + DVA_OTP_INDIRECT_ACCESS_BUF_SIZE) <= len)
            {
                indirLen = DVA_OTP_INDIRECT_ACCESS_BUF_SIZE;
            }
            else
            {
                /* Read OTP for last round */
                indirLen = len - iBuf;
            }

            /* Indir access */
            CHK_FUN_RET(retVal, dva_drv_memset(indirData, 0, sizeof(indirData)));
            indirCmd = DVA_OTP_CMD_OP_READ | (indirAddr << DVA_OTP_CMD_ADDR_LSP) | (indirLen << DVA_OTP_CMD_LEN_LSP);
            CHK_FUN_RET(retVal, dva_drv_otp_indir_access(unit, 0u, indirCmd, indirData));

            if(RT_ERR_OK == retVal)
            {
                /* Cast indirData to buf */
                for(iIndirData = 0u; iIndirData < indirLen; iIndirData++)
                {
                    buf[iIndirData + iBuf] = (uint8)((indirData[iIndirData / DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BYTE] >> (DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BIT * (iIndirData % DVA_OTP_INDIRECT_ACCESS_ALIGNMENT_BYTE))) & (uint16)UINT8_MAX);
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
