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

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtl907xd_va_reg_list.h"
#include "rtkas_debug.h"
#include "rtl907xd_va_asicdrv_stacking.h"
#include "rtl907xd_va_asicdrv_ethernet_access.h"
#include "rtl907xd_va_asicdrv_config.h"
#ifdef RTL906X_CMD_SHOW
#include "rtl906x_glue.h"
#endif
#include "rtl907xd_va_api_esm.h"

#define DVA_STACK_MAX_NUM 4u

uint32 dva_g_stackingTimeout = STACKING_TIMEOUT;

void dva_drv_stacking_eth_table_data(uint32 regNum, uint32 entryRegNum, const uint32 *regValue, uint32 *data);

/**
 * @brief Dump the current status of stacking configuration through register.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] target               The switch stacking information that currently record in stacking status registers
 *                                  0x0: This switch (the switch that directly connected)
 *                                  0x10: 1 hop neighbor on port S0 (for dynamic construct)
 *                                  0x11: 2 hops neighbor on port S0 (for dynamic construct)
 *                                  0x12: 3 hops neighbor on port S0 (for dynamic construct)
 *                                  0x20: 1 hop neighbor on port S1 (for dynamic construct)
 *                                  0x21: 2 hops neighbor on port S1 (for dynamic construct)
 *                                  0x22: 3 hops neighbor on port S1 (for dynamic construct)
 * @param[in] pThisSwitch          The pointer to structure of this switch information
 * @param[out] None
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet dva_drv_stacking_dump_reg(uint32 unit, uint8 target, dva_StackSwitchInfo_t *pThisSwitch)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;

    CHK_FUN_RET(retVal, dva_drv_memset(pThisSwitch, 0, sizeof(dva_StackSwitchInfo_t)));
    tmpVal = (uint32)target;

    /* Set dump command */
    CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, ((tmpVal << 16) | DVA_STACK_COMMAND_DUMP_REG)));
    CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));

    /* Read dump value 1 */
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_3, &tmpVal));
    if(target == 0u)
    {
        pThisSwitch->numOnS0       = (uint8)((tmpVal & 0xFF000000u) >> 24u);
        pThisSwitch->numOnS1       = (uint8)((tmpVal & 0x00FF0000u) >> 16u);
        pThisSwitch->master_locate = (uint8)((tmpVal & 0x0000F000u) >> 12u);
        pThisSwitch->status        = (uint8)(tmpVal & 0x000000FFu);
    }
    pThisSwitch->available    = (uint8)((tmpVal & 0x00000400u) >> 10u);
    pThisSwitch->portS0Enable = (uint8)((tmpVal & 0x00000200u) >> 9u);
    pThisSwitch->portS1Enable = (uint8)((tmpVal & 0x00000100u) >> 8u);

    /* Read dump value 2 */
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_4, &tmpVal));
    pThisSwitch->unitID      = (uint8)(tmpVal & 0x0000000Fu);
    pThisSwitch->masterID    = (uint8)((tmpVal & 0x000000F0u) >> 4u);
    pThisSwitch->portToRelay = (uint8)((tmpVal & 0x00000F00u) >> 8u);
    pThisSwitch->portToInit  = (uint8)((tmpVal & 0x0000F000u) >> 12u);
    pThisSwitch->groupID     = (uint16)((tmpVal & 0xFFFF0000u) >> 16u);

    /* Read dump value 3 */
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_5, &tmpVal));
    pThisSwitch->unitMapId[0]   = (uint8)(tmpVal & 0x0000000Fu);
    pThisSwitch->unitMapPort[0] = (uint8)((tmpVal & 0x000000F0u) >> 4u);
    pThisSwitch->unitMapId[1]   = (uint8)((tmpVal & 0x00000F00u) >> 8u);
    pThisSwitch->unitMapPort[1] = (uint8)((tmpVal & 0x0000F000u) >> 12u);
    pThisSwitch->unitMapId[2]   = (uint8)((tmpVal & 0x000F0000u) >> 16u);
    pThisSwitch->unitMapPort[2] = (uint8)((tmpVal & 0x00F00000u) >> 20u);
    pThisSwitch->unitMapId[3]   = (uint8)((tmpVal & 0x0F000000u) >> 24u);
    pThisSwitch->unitMapPort[3] = (uint8)((tmpVal & 0xF0000000u) >> 28u);

    /* Read dump value 4 */
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_1, &tmpVal));
    pThisSwitch->model         = (uint16)((tmpVal & 0xFFFF0000u) >> 16u);
    pThisSwitch->macAddress[0] = (uint8)((tmpVal & 0x0000FF00u) >> 8u);
    pThisSwitch->macAddress[1] = (uint8)(tmpVal & 0x000000FFu);

    /* Read dump value 5 */
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_2, &tmpVal));
    pThisSwitch->macAddress[2] = (uint8)((tmpVal & 0xFF000000u) >> 24u);
    pThisSwitch->macAddress[3] = (uint8)((tmpVal & 0x00FF0000u) >> 16u);
    pThisSwitch->macAddress[4] = (uint8)((tmpVal & 0x0000FF00u) >> 8u);
    pThisSwitch->macAddress[5] = (uint8)(tmpVal & 0x000000FFu);
    return retVal;
}

static RtkApiRet dva_drv_stacking_eth_reg_read_send(uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue)
{
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, 0u, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(regNum, regValue, rxPkt));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_eth_reg_read_wait(uint32 *value)
{
    uint8                 regCmd[DVA_STACKING_ACCESS_CHK_CMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_READ};
    uint8                 regAddr[DVA_STACKING_ACCESS_CHK_CMD][RTK_REG_ADDR_LEN];
    uint32                regVal[DVA_STACKING_ACCESS_CHK_CMD];
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    uint32                timeout = 0u;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for response */
        stkCmd = DVA_STACK_COMMAND_REG_GET;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        regVal[0]     = 0;
        while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
        {
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, 1, regCmd, &regAddr[0], regVal, 0u, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (2u * 8u)));
            rxPktLen = DVA_MAX_PKT_LEN;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(1, regVal, rxPkt));
            stkCmd = regVal[0];
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
            if(stkCmd == DVA_STACK_COMMAND_REG_GET_FAIL)
            {
                retVal = RT_ERR_STACK_ACCESS_ERROR;
            }
            timeout++;
        }

        /* Timeout handling */
        if(timeout == dva_g_stackingTimeout)
        {
            *value = 0;
            retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
            dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        }
        else
        {
            if(retVal == RT_ERR_OK)
            {
                /* Return value on the register field */
                regCmd[0]     = DVA_ETHERACCESS_REGISTER_RW_MAC_READ;
                regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REG_VALUE_1623;
                regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REG_VALUE_815;
                regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REG_VALUE_07;
                regVal[0]     = 0;
                CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_reg_read_send(1, regCmd, &regAddr[0], regVal));
                *value = regVal[0];
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Read register value on specific unit of stacking switch with eth
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] address              The address of register
 * @param[out] value               Return the register value in this pointer
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_STACK_REMOTE_NO_RESP
 * Others are fail
 */
RtkApiRet dva_drv_stacking_eth_reg_read(uint32 unit, uint32 address, uint32 *value)
{
    uint8                 regCmd[DVA_STACKING_REG_RWCMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[DVA_STACKING_REG_RWCMD][RTK_REG_ADDR_LEN];
    uint32                reg_val[DVA_STACKING_REG_RWCMD];
    uint32                regNum = DVA_STACKING_REG_RWCMD;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    EtherAccessL2Header_t txPkt;

    do
    {
        if(unit == dva_g_stackingMaster)
        {
            retVal = -RT_ERR_INPUT;
        }
        stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_REG_GET);
        /* Set register value field to 0 */
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REG_VALUE_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REG_VALUE_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REG_VALUE_07;
        reg_val[0]    = 0u;
        /* Set register address */
        regAddr[1][0] = (uint8)DVA_STACK_COMMAND_REG_ADDR_1623;
        regAddr[1][1] = (uint8)DVA_STACK_COMMAND_REG_ADDR_815;
        regAddr[1][2] = (uint8)DVA_STACK_COMMAND_REG_ADDR_07;
        reg_val[1]    = address;
        /* Issue stacking register read */
        regAddr[2][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[2][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[2][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        reg_val[2]    = stkCmd;

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], reg_val, 0u, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (((regNum + 1u)) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(regNum, reg_val, rxPkt));

        /* Wait for response */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_reg_read_wait(value));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_eth_reg_write_wait(void)
{
    uint8                 regCmd[DVA_STACKING_ACCESS_CHK_CMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_READ};
    uint8                 regAddr[DVA_STACKING_ACCESS_CHK_CMD][RTK_REG_ADDR_LEN];
    uint32                regVal[DVA_STACKING_ACCESS_CHK_CMD];
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    uint32                timeout = 0u;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for response */
        stkCmd = DVA_STACK_COMMAND_REG_SET;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        regCmd[0]     = DVA_ETHERACCESS_REGISTER_RW_MAC_READ;
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        regVal[0]     = 0;
        while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
        {
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, 1, regCmd, &regAddr[0], regVal, 0u, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (2u * 8u)));
            rxPktLen = DVA_MAX_PKT_LEN;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(1, regVal, rxPkt));
            stkCmd = regVal[0];
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
            if(stkCmd == DVA_STACK_COMMAND_REG_SET_FAIL)
            {
                retVal = RT_ERR_STACK_ACCESS_ERROR;
            }
            timeout++;
        }

        /* Timeout handling */
        if(timeout == dva_g_stackingTimeout)
        {
            retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
            dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Write register on specific unit of stacking switch with eth
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] address              The address of register
 * @param[in] value                The value to write
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * Others are fail
 */
RtkApiRet dva_drv_stacking_eth_reg_write(uint32 unit, uint32 address, uint32 value)
{
    uint8                 regCmd[DVA_STACKING_REG_RWCMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[DVA_STACKING_REG_RWCMD][RTK_REG_ADDR_LEN];
    uint32                reg_val[DVA_STACKING_REG_RWCMD];
    uint32                regNum = DVA_STACKING_REG_RWCMD;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    EtherAccessL2Header_t txPkt;

    do
    {
        if(unit == dva_g_stackingMaster)
        {
            retVal = -RT_ERR_INPUT;
        }
        stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_REG_SET);
        /* Set register value field to 0 */
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REG_VALUE_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REG_VALUE_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REG_VALUE_07;
        reg_val[0]    = value;
        /* Set register address */
        regAddr[1][0] = (uint8)DVA_STACK_COMMAND_REG_ADDR_1623;
        regAddr[1][1] = (uint8)DVA_STACK_COMMAND_REG_ADDR_815;
        regAddr[1][2] = (uint8)DVA_STACK_COMMAND_REG_ADDR_07;
        reg_val[1]    = address;
        /* Issue stacking register write */
        regAddr[2][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[2][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[2][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        reg_val[2]    = stkCmd;

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], reg_val, 0u, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (((regNum + 1u)) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(regNum, reg_val, rxPkt));

        /* Wait for response */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_reg_write_wait());
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_reg_read_data(uint32 timeout, uint32 *value)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(timeout == dva_g_stackingTimeout)
    {
        *value = 0;
        retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
    }
    else
    {
        /* Return value on the register field */
        retVal = reg_direct_read(DVA_STACK_COMMAND_REG_VALUE, value);
    }

    return retVal;
}

/**
 * @brief Read register on specific unit of stacking switch
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] address              The address of register
 * @param[out] value               Return the register value in this pointer
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_STACK_REMOTE_NO_RESP
 * Others are fail
 */
static RtkApiRet dva_drv_stacking_reg_read_set(uint32 stkAddr, uint32 stkCmd)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Set register value field to 0 */
        CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REG_VALUE, 0u));
        /* Set register address */
        CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REG_ADDR, stkAddr));
        /* Issue stacking register read */
        CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REGISTER, stkCmd));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Read register on specific unit of stacking switch
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] address              The address of register
 * @param[out] value               Return the register value in this pointer
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_STACK_REMOTE_NO_RESP
 * Others are fail
 */
#ifdef RTK_INTERNAL_DEV
RtkApiRet dva_drv_stacking_reg_read_internal(uint32 unit, uint32 regAddr, uint32 *value)
#else
RtkApiRet dva_drv_stacking_reg_read(uint32 unit, uint32 regAddr, uint32 *value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    stkAddr = DVA_STACKING_PAGE | (regAddr & DVA_STACKING_ADDR_MSK);
    uint32    stkCmd;
    uint32    timeout = 0u;

    if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((unit <= 0u) || (unit > DVA_STACK_MAX_UNIT_ID))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            retVal = dva_drv_stacking_eth_reg_read(unit, stkAddr, value);
        }
        else
        {
            do
            {
                stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_REG_GET);
                /* Set register */
                CHK_FUN_CONTINUE(retVal, dva_drv_stacking_reg_read_set(stkAddr, stkCmd));

                /* Wait for response */
                while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
                {
                    CHK_FUN_CONTINUE(retVal, reg_direct_read(DVA_STACK_COMMAND_REGISTER, &stkCmd));
                    CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
                    if(stkCmd == DVA_STACK_COMMAND_REG_GET_FAIL)
                    {
                        retVal = RT_ERR_STACK_ACCESS_ERROR;
                    }
                    timeout++;
                }
                if(timeout == dva_g_stackingTimeout)
                {
                    retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
                    dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
                }
                else
                {
                    /* Timeout handling */
                    if(retVal == RT_ERR_OK)
                    {
                        /* Return value on the register field */
                        retVal = dva_drv_stacking_reg_read_data(timeout, value);
                    }
                }
            } while(0u == 1u);
        }
    }

    return retVal;
}

/**
 * @brief Write register on specific unit of stacking switch
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] address              The address of register
 * @param[in] value                The value to write
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * Others are fail
 */
#ifdef RTK_INTERNAL_DEV
RtkApiRet dva_drv_stacking_reg_write_internal(uint32 unit, uint32 regAddr, uint32 value)
#else
RtkApiRet dva_drv_stacking_reg_write(uint32 unit, uint32 regAddr, uint32 value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    stkAddr = DVA_STACKING_PAGE | (regAddr & DVA_STACKING_ADDR_MSK);
    uint32    stkCmd;
    uint32    timeout = 0u;

    if(g_RegIF == (uint8)REGIF_ETHERNET)
    {
        retVal = dva_drv_stacking_eth_reg_write(unit, stkAddr, value);
    }
    else if((unit <= 0u) || (unit > DVA_STACK_MAX_UNIT_ID))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_REG_SET);
            /* Set register value field */
            CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REG_VALUE, value));
            /* Set register address field */
            CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REG_ADDR, stkAddr));
            /* Issue stacking register write */
            CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REGISTER, stkCmd));

            /* Wait for response */
            while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
            {
                CHK_FUN_CONTINUE(retVal, reg_direct_read(DVA_STACK_COMMAND_REGISTER, &stkCmd));
                CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
                if(stkCmd == DVA_STACK_COMMAND_REG_SET_FAIL)
                {
                    retVal = RT_ERR_STACK_ACCESS_ERROR;
                }
                timeout++;
            }
            if(timeout == dva_g_stackingTimeout)
            {
                retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
                dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            }
        } while(0u == 1u);
    }

    return retVal;
}

static RtkApiRet dva_drv_stacking_eth_page_read_send(uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue)
{
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, 0u, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, (DVA_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u))));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(regNum, regValue, rxPkt));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_eth_page_read_wait(uint32 *value)
{
    uint8                 regCmd[DVA_STACKING_ACCESS_CHK_CMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_READ};
    uint8                 regAddr[DVA_STACKING_ACCESS_CHK_CMD][RTK_REG_ADDR_LEN];
    uint32                regVal[DVA_STACKING_ACCESS_CHK_CMD];
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    uint32                timeout = 0u;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for response */
        stkCmd = DVA_STACK_COMMAND_PAGE_GET;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        regVal[0]     = 0;
        while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
        {
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, 1, regCmd, &regAddr[0], regVal, 0u, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (2u * 8u)));
            rxPktLen = DVA_MAX_PKT_LEN;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(1, regVal, rxPkt));
            stkCmd = regVal[0];
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
            if(stkCmd == DVA_STACK_COMMAND_PAGE_GET_FAIL)
            {
                retVal = RT_ERR_STACK_ACCESS_ERROR;
            }
            timeout++;
        }

        /* Timeout handling */
        if(timeout == dva_g_stackingTimeout)
        {
            *value = 0;
            retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
            dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        }
        else
        {
            if(retVal == RT_ERR_OK)
            {
                /* Return value on the register field */
                regCmd[0]     = DVA_ETHERACCESS_REGISTER_RW_MAC_READ;
                regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REG_VALUE_1623;
                regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REG_VALUE_815;
                regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REG_VALUE_07;
                regVal[0]     = 0;
                CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_page_read_send(1, regCmd, &regAddr[0], regVal));
                *value = regVal[0];
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Read register value on specific unit of stacking switch with eth
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] value               Return the register value in this pointer
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_STACK_REMOTE_NO_RESP
 * Others are fail
 */
RtkApiRet dva_drv_stacking_eth_page_read(uint32 unit, uint32 *value)
{
    uint8                 regCmd[DVA_STACKING_PAGE_RWCMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[DVA_STACKING_PAGE_RWCMD][RTK_REG_ADDR_LEN];
    uint32                reg_val[DVA_STACKING_PAGE_RWCMD];
    uint32                regNum = DVA_STACKING_PAGE_RWCMD;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    EtherAccessL2Header_t txPkt;

    do
    {
        if(unit == dva_g_stackingMaster)
        {
            retVal = -RT_ERR_INPUT;
        }
        stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_PAGE_GET);
        /* Set register value field to 0 */
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REG_VALUE_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REG_VALUE_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REG_VALUE_07;
        reg_val[0]    = 0u;
        /* Issue stacking register read */
        regAddr[1][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[1][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[1][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        reg_val[1]    = stkCmd;

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], reg_val, 0u, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (((regNum + 1u)) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(regNum, reg_val, rxPkt));

        /* Wait for response */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_page_read_wait(value));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_eth_page_write_wait(void)
{
    uint8                 regCmd[DVA_STACKING_ACCESS_CHK_CMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[DVA_STACKING_ACCESS_CHK_CMD][RTK_REG_ADDR_LEN];
    uint32                regVal[DVA_STACKING_ACCESS_CHK_CMD];
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    uint32                timeout = 0u;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for response */
        stkCmd = DVA_STACK_COMMAND_PAGE_SET;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        regCmd[0]     = DVA_ETHERACCESS_REGISTER_RW_MAC_READ;
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        regVal[0]     = 0;
        while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
        {
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, 1, regCmd, &regAddr[0], regVal, 0u, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (2u * 8u)));
            rxPktLen = DVA_MAX_PKT_LEN;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(1, regVal, rxPkt));
            stkCmd = regVal[0];
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
            if(stkCmd == DVA_STACK_COMMAND_PAGE_SET_FAIL)
            {
                retVal = RT_ERR_STACK_ACCESS_ERROR;
            }
            timeout++;
        }

        /* Timeout handling */
        if(timeout == dva_g_stackingTimeout)
        {
            retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
            dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Write register on specific unit of stacking switch with eth
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] value                The value to write
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * Others are fail
 */
RtkApiRet dva_drv_stacking_eth_page_write(uint32 unit, uint32 value)
{
    uint8                 regCmd[DVA_STACKING_PAGE_RWCMD] = {DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[DVA_STACKING_PAGE_RWCMD][RTK_REG_ADDR_LEN];
    uint32                reg_val[DVA_STACKING_PAGE_RWCMD];
    uint32                regNum = DVA_STACKING_PAGE_RWCMD;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    EtherAccessL2Header_t txPkt;

    do
    {
        if(unit == dva_g_stackingMaster)
        {
            retVal = -RT_ERR_INPUT;
        }
        stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_PAGE_SET);
        /* Set register value field to 0 */
        regAddr[0][0] = (uint8)DVA_STACK_COMMAND_REG_VALUE_1623;
        regAddr[0][1] = (uint8)DVA_STACK_COMMAND_REG_VALUE_815;
        regAddr[0][2] = (uint8)DVA_STACK_COMMAND_REG_VALUE_07;
        reg_val[0]    = value;
        /* Issue stacking register write */
        regAddr[1][0] = (uint8)DVA_STACK_COMMAND_REGISTER_1623;
        regAddr[1][1] = (uint8)DVA_STACK_COMMAND_REGISTER_815;
        regAddr[1][2] = (uint8)DVA_STACK_COMMAND_REGISTER_07;
        reg_val[1]    = stkCmd;

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], reg_val, 0u, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (((regNum + 1u)) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_burst_rw(regNum, reg_val, rxPkt));

        /* Wait for response */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_page_write_wait());
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Read page on specific unit of stacking switch
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] value               Return the page value in this pointer
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_STACK_REMOTE_NO_RESP
 * Others are fail
 */
#ifdef RTK_INTERNAL_DEV
RtkApiRet dva_drv_stacking_page_read_internal(uint32 unit, uint32 *value)
#else
RtkApiRet dva_drv_stacking_page_read(uint32 unit, uint32 *value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    stkCmd;
    uint32    timeout = 0u;

    if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((unit <= 0u) || (unit > DVA_STACK_MAX_UNIT_ID))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            retVal = dva_drv_stacking_eth_page_read(unit, value);
        }
        else
        {
            do
            {
                stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_PAGE_GET);
                /* Set page value field to 0 */
                CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REG_VALUE, 0u));
                /* Issue stacking page read */
                CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REGISTER, stkCmd));

                /* Wait for response */
                while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
                {
                    CHK_FUN_CONTINUE(retVal, reg_direct_read(DVA_STACK_COMMAND_REGISTER, &stkCmd));
                    CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
                    if(stkCmd == DVA_STACK_COMMAND_PAGE_GET_FAIL)
                    {
                        retVal = RT_ERR_STACK_ACCESS_ERROR;
                    }
                    timeout++;
                }

                /* Timeout handling */
                if(timeout == dva_g_stackingTimeout)
                {
                    *value = 0;
                    retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
                    dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
                }
                else
                {
                    if(retVal == RT_ERR_OK)
                    {
                        /* Return value on the page field */
                        retVal = reg_direct_read(DVA_STACK_COMMAND_REG_VALUE, value);
                        *value = rtl_htonl(*value);
                    }
                }
            } while(0u == 1u);
        }
    }

    return retVal;
}

/**
 * @brief Write page on specific unit of stacking switch
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] value                The value to write
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * Others are fail
 */
#ifdef RTK_INTERNAL_DEV
RtkApiRet dva_drv_stacking_page_write_internal(uint32 unit, uint32 value)
#else
RtkApiRet dva_drv_stacking_page_write(uint32 unit, uint32 value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    stkCmd;
    uint32    timeout = 0u;

    if(g_RegIF == (uint8)REGIF_ETHERNET)
    {
        retVal = dva_drv_stacking_eth_page_write(unit, value);
    }
    else if((unit <= 0u) || (unit > DVA_STACK_MAX_UNIT_ID))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            stkCmd = ((((uint32)unit) << 8u) | DVA_STACK_COMMAND_PAGE_SET);
            /* Set page value field */
            CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REG_VALUE, rtl_htonl(value)));
            /* Issue stacking page write */
            CHK_FUN_CONTINUE(retVal, reg_direct_write(DVA_STACK_COMMAND_REGISTER, stkCmd));

            /* Wait for response */
            while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
            {
                CHK_FUN_CONTINUE(retVal, reg_direct_read(DVA_STACK_COMMAND_REGISTER, &stkCmd));
                CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
                if(stkCmd == DVA_STACK_COMMAND_PAGE_SET_FAIL)
                {
                    retVal = RT_ERR_STACK_ACCESS_ERROR;
                }
                timeout++;
            }
            if(timeout == dva_g_stackingTimeout)
            {
                retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
                dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            }
        } while(0u == 1u);
    }

    return retVal;
}

static RtkApiRet dva_drv_stacking_eth_table_send(dva_StackBurstReg_t *stkHdr, uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue)
{
    EtherAccessMac_t          mac;
    uint32                    txPktLen = DVA_STACK_BURST_READ_WRITE_LEN;
    uint32                    rxPktLen = DVA_MAX_PKT_LEN;
    dva_StackEthBurstHeader_t txPkt;
    uint8                     rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet                 retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    /* packet process */
    CHK_FUN_RET(retVal, dva_drv_eth_acc_stacking_gen_burst(mac, stkHdr, regNum, regCmd, &regAddr[0], regValue, &txPkt));
    CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
    CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

    /* parse data */
    CHK_FUN_RET(retVal, dva_drv_eth_acc_stacking_check_burst(stkHdr, regNum, regCmd, &regAddr[0], regValue, rxPkt));

    return retVal;
}

void dva_drv_stacking_eth_table_data(uint32 regNum, uint32 entryRegNum, const uint32 *regValue, uint32 *data)
{
    uint32 i, j;

    j = 0;
    for(i = 0; i < regNum; i++)
    {
        if((i % entryRegNum) == 0u)
        {
            continue;
        }
        else
        {
            data[j] = regValue[i];
            j++;
        }
    }
}

static RtkApiRet dva_drv_stacking_eth_table_helper(uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, dva_StackBurstReg_t *stkHdr, uint32 rw, uint32 type, uint32 *wTableReg)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* make etherAccess payload */
    CHK_FUN_RET(retVal, dva_drv_memset(regCmd, 0, RTK_ETHERACCESS_MAX_BURST));
    CHK_FUN_RET(retVal, dva_drv_memset(regAddr, 0, RTK_ETHERACCESS_MAX_BURST * RTK_REG_ADDR_LEN));
    CHK_FUN_RET(retVal, dva_drv_memset(regValue, 0, RTK_ETHERACCESS_MAX_BURST * 4u));
    CHK_FUN_RET(retVal, dva_drv_memset(stkHdr, 0, sizeof(dva_StackBurstReg_t)));
    CHK_FUN_RET(retVal, dva_reg_field_set(DVA_ETH_INCR, DVA_ETH_INCR_EXE_LSP, DVA_ETH_INCR_EXE_LEN, 1, wTableReg));
    CHK_FUN_RET(retVal, dva_reg_field_set(DVA_ETH_INCR, DVA_ETH_INCR_RW_LSP, DVA_ETH_INCR_RW_LEN, rw, wTableReg));
    CHK_FUN_RET(retVal, dva_reg_field_set(DVA_ETH_INCR, DVA_ETH_INCR_TYPE_LSP, DVA_ETH_INCR_TYPE_LEN, type, wTableReg));

    return retVal;
}

/**
 * @brief Table dump.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] table Table
 * @param[in] startIndex Start index
 * @param[in] entryNum Entry number
 * @param[out] data Data
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_STACK_TABLE_FAIL
 * Others are fail
 */
RtkApiRet dva_drv_stacking_eth_table_get(uint32 unit, dva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data)
{
    uint32                wTableReg = 0;
    uint32                wTableRegTmp;
    uint32                entryRegNum = dva_table_list[table].datareg_num + 1u;
    uint32                regNum      = entryNum * entryRegNum;
    uint8                 regCmd[RTK_ETHERACCESS_MAX_BURST];
    uint8                 regAddr[RTK_ETHERACCESS_MAX_BURST][RTK_REG_ADDR_LEN];
    uint32                regValue[RTK_ETHERACCESS_MAX_BURST];
    dva_StackBurstReg_t   stkHdr;
    dva_StackSwitchInfo_t pThisSwitch;
    uint32                i, j;
    uint32                indirCtrlReg  = 0u;
    uint32                indirData0Reg = 0u;
    RtkApiRet             retVal        = RT_ERR_OK;

    do
    {
        /* Check input */
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_table_check(table, startIndex, entryNum, regNum, data));
        dva_indirect_reg_get(table, &indirCtrlReg, &indirData0Reg);
        /* make etherAccess payload */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_table_helper(regCmd, regAddr, regValue, &stkHdr, 1, dva_table_list[table].type, &wTableReg));
        for(i = 0; i < entryNum; i++)
        {
            regCmd[i * entryRegNum]     = DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[i * entryRegNum][0] = (uint8)((indirCtrlReg >> 16u) & 0xFFu);
            regAddr[i * entryRegNum][1] = (uint8)((indirCtrlReg >> 8u) & 0xFFu);
            regAddr[i * entryRegNum][2] = (uint8)((indirCtrlReg)&0xFFu);
            wTableRegTmp                = wTableReg;
            CHK_FUN_RET(retVal, dva_reg_field_set(indirCtrlReg, DVA_ETH_INCR_ADDR_LSP, DVA_ETH_INCR_ADDR_LEN, startIndex + i, &wTableRegTmp));
            regValue[i * entryRegNum] = wTableRegTmp;
            for(j = 1; j <= dva_table_list[table].datareg_num; j++)
            {
                regCmd[(i * entryRegNum) + j]     = DVA_ETHERACCESS_REGISTER_RW_MAC_READ;
                regAddr[(i * entryRegNum) + j][0] = (uint8)((indirData0Reg >> 16u) & 0xFFu);
                regAddr[(i * entryRegNum) + j][1] = (uint8)((indirData0Reg >> 8u) & 0xFFu);
                regAddr[(i * entryRegNum) + j][2] = (uint8)(indirData0Reg + ((j - 1u) * 4u)) & 0xFFu;
            }
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_dump_reg(dva_g_stackingMaster, 0u, &pThisSwitch));
        stkHdr.rwNum    = regNum;
        stkHdr.groupId  = pThisSwitch.groupID;
        stkHdr.fromUnit = (uint8)(dva_g_stackingMaster & 0xffu);
        stkHdr.toUnit   = (uint8)(unit & 0xffu);

        /* packet process */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_table_send(&stkHdr, regNum, regCmd, &regAddr[0], regValue));

        if((stkHdr.groupId != pThisSwitch.groupID) || (stkHdr.toUnit != (uint8)(dva_g_stackingMaster & 0xffu)) || (stkHdr.fromUnit != (uint8)(unit & 0xffu)))
        {
            retVal = -RT_ERR_STACK_TABLE_FAIL;
        }
        else
        {
            dva_drv_stacking_eth_table_data(regNum, entryRegNum, regValue, data);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Table set.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] table Table
 * @param[in] startIndex Start index
 * @param[in] entryNum Entry number
 * @param[in] data Data
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_STACK_TABLE_FAIL
 * Others are fail
 */
RtkApiRet dva_drv_stacking_eth_table_set(uint32 unit, dva_table_list_t table, uint32 startIndex, uint32 entryNum, const uint32 *data)
{
    uint32                wTableReg = 0;
    uint32                wTableRegTmp;
    uint32                entryRegNum = dva_table_list[table].datareg_num + 1u;
    uint32                regNum      = entryNum * entryRegNum;
    uint8                 regCmd[RTK_ETHERACCESS_MAX_BURST];
    uint8                 regAddr[RTK_ETHERACCESS_MAX_BURST][RTK_REG_ADDR_LEN];
    uint32                regValue[RTK_ETHERACCESS_MAX_BURST];
    dva_StackBurstReg_t   stkHdr;
    dva_StackSwitchInfo_t pThisSwitch;
    uint32                i, j;
    uint32                indirCtrlReg  = 0u;
    uint32                indirData0Reg = 0u;
    RtkApiRet             retVal        = RT_ERR_OK;

    do
    {
        /* Check input */
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_table_check(table, startIndex, entryNum, regNum, data));
        dva_indirect_reg_get(table, &indirCtrlReg, &indirData0Reg);
        /* make etherAccess payload */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_table_helper(regCmd, regAddr, regValue, &stkHdr, 0, dva_table_list[table].type, &wTableReg));
        for(i = 0; i < entryNum; i++)
        {
            for(j = 0; j < dva_table_list[table].datareg_num; j++)
            {
                regCmd[(i * entryRegNum) + j]     = DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE;
                regAddr[(i * entryRegNum) + j][0] = (uint8)((indirData0Reg >> 16u) & 0xFFu);
                regAddr[(i * entryRegNum) + j][1] = (uint8)((indirData0Reg >> 8u) & 0xFFu);
                regAddr[(i * entryRegNum) + j][2] = (uint8)(indirData0Reg + (j * 4u)) & 0xFFu;
                regValue[(i * entryRegNum) + j]   = data[(i * dva_table_list[table].datareg_num) + j];
            }
            regCmd[(i * entryRegNum) + j]     = DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[(i * entryRegNum) + j][0] = (uint8)((indirCtrlReg >> 16u) & 0xFFu);
            regAddr[(i * entryRegNum) + j][1] = (uint8)((indirCtrlReg >> 8u) & 0xFFu);
            regAddr[(i * entryRegNum) + j][2] = (uint8)((indirCtrlReg)&0xFFu);
            wTableRegTmp                      = wTableReg;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_set(indirCtrlReg, DVA_ETH_INCR_ADDR_LSP, DVA_ETH_INCR_ADDR_LEN, startIndex + i, &wTableRegTmp));
            regValue[(i * entryRegNum) + j] = wTableRegTmp;
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_dump_reg(dva_g_stackingMaster, 0u, &pThisSwitch));
        stkHdr.rwNum    = regNum;
        stkHdr.groupId  = pThisSwitch.groupID;
        stkHdr.fromUnit = (uint8)(dva_g_stackingMaster & 0xffu);
        stkHdr.toUnit   = (uint8)(unit & 0xffu);

        /* packet process */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_eth_table_send(&stkHdr, regNum, regCmd, &regAddr[0], regValue));
        if((stkHdr.groupId != pThisSwitch.groupID) || (stkHdr.toUnit != (uint8)(dva_g_stackingMaster & 0xffu)) || (stkHdr.fromUnit != (uint8)(unit & 0xffu)))
        {
            retVal = -RT_ERR_STACK_TABLE_FAIL;
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_stacking_command_polling(uint32 unit)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    stkCmd  = 0xFFu;
    uint32    timeout = 0u;

    /* Polling command is finished */
    while((stkCmd != 0u) && (timeout < dva_g_stackingTimeout) && (retVal == RT_ERR_OK))
    {
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, DVA_STACK_COMMAND_REGISTER, &stkCmd));
        timeout++;
    }
    if(timeout == dva_g_stackingTimeout)
    {
        retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
        dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
    }

    return retVal;
}

RtkApiRet dva_drv_stacking_check_connection_status(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;
    uint8     status = 0u;

    do
    {
        /* Dump stacking status to register */
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, ((tmpVal << 16u) | DVA_STACK_COMMAND_DUMP_REG)));
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_command_polling(unit));
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_3, &tmpVal));
        status = (uint8)(tmpVal & 0x000000FFu);

        /* Return if stacking is not working */
        if((status == DVA_STACK_STS_DISABLE) || (status == DVA_STACK_STS_ERROR))
        {
            retVal = -RT_ERR_STACK_NOT_INIT;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_load_fw_header(void *pFile, uint32 *fileAddr, uint32 *fwLen, uint32 *stkReg, dva_StackPayloadRemoteLoad_t *stkHdr, uint32 slaveUnit, uint32 slaveLoc)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal;

    do
    {
        if(slaveLoc == 0u)
        {
            retVal = -RT_ERR_STACK_LOAD_SLAVE;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_config_get_hdr_fw_info(pFile, fileAddr, fwLen, &stkHdr->fwAseType));
            CHK_FUN_CONTINUE(retVal, dva_reg_field_get(0, DVA_STACK_DMY1_FW_LOAD_TYPE_LSP, DVA_STACK_DMY1_FW_LOAD_TYPE_LEN, &tmpVal, stkReg[1]));
            stkHdr->fwSrcType = (tmpVal == DVA_STACK_LOAD_FW_BY_FLASH) ? DVA_STACK_REMOTE_LOAD_FLAG_FLASH : DVA_STACK_REMOTE_LOAD_FLAG_SRAM;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_get(0, DVA_STACK_DMY1_GROUP_ID_LSP, DVA_STACK_DMY1_GROUP_ID_LEN, &tmpVal, stkReg[1]));
            stkHdr->groupId = (uint16)tmpVal;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_get(0, DVA_STACK_DMY1_MASTER_ID_LSP, DVA_STACK_DMY1_MASTER_ID_LEN, &tmpVal, stkReg[1]));
            stkHdr->masterId = (uint8)tmpVal;
            stkHdr->destId   = (uint8)slaveUnit;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_get(0, DVA_STACK_DMY2_MAC4_LSP, DVA_STACK_DMY2_MAC4_LEN, &tmpVal, stkReg[slaveLoc + 2u]));
            stkHdr->desMAC4 = (uint8)tmpVal;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_get(0, DVA_STACK_DMY2_MAC5_LSP, DVA_STACK_DMY2_MAC5_LEN, &tmpVal, stkReg[slaveLoc + 2u]));
            stkHdr->desMAC5 = (uint8)tmpVal;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_load_fw_process(const uint8 *sMac, const uint8 dMac[MAC_ADDR_LEN], dva_StackPayloadRemoteLoad_t *stkHdr, uint8 *fwData, uint16 seqId)
{
    uint32                 txPktLen = (uint32)(sizeof(dva_StackEthFwHeader_t) & 0xFFFFFFFFu);
    uint32                 rxPktLen = DVA_MAX_PKT_LEN;
    dva_StackEthFwHeader_t txPkt;
    uint8                  rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet              retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_stk_gen_fw(sMac, dMac, stkHdr, fwData, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

        /* parse data */
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_stk_check_rep(rxPkt, seqId));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_load_fw_ptr_check(void *pFile, uint32 *stkReg)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == pFile) || (NULL == stkReg))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}
RtkApiRet dva_drv_stacking_flashless_update_slave_check(const int8 *fwFileName, const int8 *cfgFileName)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == fwFileName) || (NULL == cfgFileName))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

/**
 * @brief Stacking load flashless fw to slave.
 * @param[in] pFile file opened
 * @param[in] stkReg stacking data
 * @param[in] slaveUnit slave unit
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_FILE_READ
 * Others are fail
 */
RtkApiRet dva_drv_stacking_load_fw(void *pFile, uint32 *stkReg, uint32 slaveUnit, uint32 slaveLoc)
{
    uint8                        sMac[MAC_ADDR_LEN];
    uint8                        dMac[MAC_ADDR_LEN];
    uint8                        fwData[DVA_STACK_REMOTE_LOAD_LENGTH_UNIT];
    dva_StackPayloadRemoteLoad_t stkHdr;
    uint32                       i;
    uint32                       fwLen     = 0;
    uint32                       fileAddr  = 0;
    uint32                       startAddr = DVA_STACK_L2_MEM_BASE;
    uint32                       offset    = 0;
    uint16                       seqId     = 1;
    RtkApiRet                    retVal    = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_fw_ptr_check(pFile, stkReg));
        /* filled stacking header */
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&stkHdr, 0, sizeof(dva_StackPayloadRemoteLoad_t)));
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_fw_header(pFile, &fileAddr, &fwLen, stkReg, &stkHdr, slaveUnit, slaveLoc));
        stkHdr.dataLength  = DVA_AES_IV_LENGTH;
        stkHdr.seqTotal    = 0u;
        stkHdr.oriStAddr   = 0;
        stkHdr.pktTotalLen = DVA_AES_IV_LENGTH;
        stkHdr.seq         = 0;
        stkHdr.destAddr    = DVA_ADDR_SYS_MEMORY_BASE + fwLen;
        if((uint32)rtl_file_read(pFile, fwData, 0x100, DVA_AES_IV_LENGTH) != DVA_AES_IV_LENGTH)
        {
            retVal = -RT_ERR_FILE_READ;
            continue;
        }

        /* packet process */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_fw_process(sMac, dMac, &stkHdr, fwData, 0));
        stkHdr.dataLength  = DVA_STACK_REMOTE_LOAD_LENGTH_UNIT;
        stkHdr.seqTotal    = (uint16)((fwLen + (DVA_STACK_REMOTE_LOAD_LENGTH_UNIT - 1u)) / (DVA_STACK_REMOTE_LOAD_LENGTH_UNIT));
        stkHdr.oriStAddr   = startAddr;
        stkHdr.pktTotalLen = fwLen;
        for(i = 0; i < (fwLen / DVA_STACK_REMOTE_LOAD_LENGTH_UNIT); ++i)
        {
            if((uint32)rtl_file_read(pFile, fwData, fileAddr + offset, DVA_STACK_REMOTE_LOAD_LENGTH_UNIT) != DVA_STACK_REMOTE_LOAD_LENGTH_UNIT)
            {
                retVal = -RT_ERR_FILE_READ;
                break;
            }

            /* packet process */
            stkHdr.seq      = seqId;
            stkHdr.destAddr = startAddr + offset;
            CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_drv_stacking_load_fw_process(sMac, dMac, &stkHdr, fwData, seqId));
            ++seqId;
            offset += DVA_STACK_REMOTE_LOAD_LENGTH_UNIT;
        }
        if((RT_ERR_OK == retVal) && ((fwLen % DVA_STACK_REMOTE_LOAD_LENGTH_UNIT) != 0u))
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_memset(fwData, 0, DVA_STACK_REMOTE_LOAD_LENGTH_UNIT));
            if((uint32)rtl_file_read(pFile, fwData, fileAddr + offset, (fwLen % DVA_STACK_REMOTE_LOAD_LENGTH_UNIT)) != (fwLen % DVA_STACK_REMOTE_LOAD_LENGTH_UNIT))
            {
                retVal = -RT_ERR_FILE_READ;
            }
            else
            {
                /* packet process */
                stkHdr.seq      = seqId;
                stkHdr.destAddr = startAddr + offset;
                retVal          = dva_drv_stacking_load_fw_process(sMac, dMac, &stkHdr, fwData, seqId);
            }
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_load_cfg_process(const uint8 *sMac, const uint8 dMac[MAC_ADDR_LEN], dva_StackPayloadRemoteLoad_t *stkHdr, uint8 *fwData, uint16 seqId)
{
    uint32                 txPktLen = (uint32)(sizeof(dva_StackEthFwHeader_t) & 0xFFFFFFFFu);
    uint32                 rxPktLen = DVA_MAX_PKT_LEN;
    dva_StackEthFwHeader_t txPkt;
    uint8                  rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet              retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_stk_gen_cfg(sMac, dMac, stkHdr, fwData, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

        /* parse data */
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_stk_check_rep(rxPkt, seqId));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_load_cfg_check(void *pFile, uint32 *stkReg, uint32 *spiStartAddr, uint32 *spiParaLen)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == pFile) || (NULL == stkReg) || (NULL == spiStartAddr) || (NULL == spiParaLen))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

/**
 * @brief Stacking load flashless fw to slave.
 * @param[in] pFile file opened
 * @param[in] stkReg stacking data
 * @param[in] slaveUnit slave unit
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_FILE_READ
 * Others are fail
 */
RtkApiRet dva_drv_stacking_load_cfg(void *pFile, uint32 *stkReg, uint32 slaveUnit, uint32 spiStartAddr, uint32 spiParaLen, uint32 slaveLoc)
{
    uint8                        sMac[MAC_ADDR_LEN];
    uint8                        dMac[MAC_ADDR_LEN];
    uint8                        fwData[DVA_STACK_REMOTE_LOAD_LENGTH_UNIT];
    dva_StackPayloadRemoteLoad_t stkHdr;
    uint32                       i;
    uint32                       fwLen     = 0;
    uint32                       fileAddr  = 0;
    uint32                       startAddr = 0;
    uint32                       offset    = 0;
    uint16                       seqId     = 1;
    RtkApiRet                    retVal    = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_fw_ptr_check(pFile, stkReg));
        /* filled stacking header */
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&stkHdr, 0, sizeof(dva_StackPayloadRemoteLoad_t)));
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_fw_header(pFile, &fileAddr, &fwLen, stkReg, &stkHdr, slaveUnit, slaveLoc));
        startAddr          = DVA_ADDR_SYS_MEMORY_BASE - 0x1000u + fileAddr + fwLen;
        stkHdr.dataLength  = DVA_AES_IV_LENGTH;
        stkHdr.seqTotal    = 0u;
        stkHdr.oriStAddr   = 0;
        stkHdr.pktTotalLen = DVA_AES_IV_LENGTH;
        stkHdr.seq         = 0;
        stkHdr.destAddr    = startAddr + spiParaLen;
        if((uint32)rtl_file_read(pFile, fwData, 0x100, DVA_AES_IV_LENGTH) != DVA_AES_IV_LENGTH)
        {
            retVal = -RT_ERR_FILE_READ;
            continue;
        }

        /* packet process */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_fw_process(sMac, dMac, &stkHdr, fwData, 0));
        stkHdr.dataLength  = DVA_STACK_REMOTE_LOAD_LENGTH_UNIT;
        stkHdr.seqTotal    = (uint16)((spiParaLen + (DVA_STACK_REMOTE_LOAD_LENGTH_UNIT - 1u)) / (DVA_STACK_REMOTE_LOAD_LENGTH_UNIT));
        stkHdr.oriStAddr   = startAddr;
        stkHdr.pktTotalLen = spiParaLen;
        for(i = 0; i < (spiParaLen / DVA_STACK_REMOTE_LOAD_LENGTH_UNIT); ++i)
        {
            if((uint32)rtl_file_read(pFile, fwData, spiStartAddr + offset, DVA_STACK_REMOTE_LOAD_LENGTH_UNIT) != DVA_STACK_REMOTE_LOAD_LENGTH_UNIT)
            {
                retVal = -RT_ERR_FILE_READ;
                break;
            }

            /* packet process */
            stkHdr.seq      = seqId;
            stkHdr.destAddr = startAddr + offset;
            CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_drv_stacking_load_cfg_process(sMac, dMac, &stkHdr, fwData, seqId));
            ++seqId;
            offset += DVA_STACK_REMOTE_LOAD_LENGTH_UNIT;
        }
        if((RT_ERR_OK == retVal) && ((spiParaLen % DVA_STACK_REMOTE_LOAD_LENGTH_UNIT) != 0u))
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_memset(fwData, 0, DVA_STACK_REMOTE_LOAD_LENGTH_UNIT));
            if((uint32)rtl_file_read(pFile, fwData, spiStartAddr + offset, (spiParaLen % DVA_STACK_REMOTE_LOAD_LENGTH_UNIT)) != (spiParaLen % DVA_STACK_REMOTE_LOAD_LENGTH_UNIT))
            {
                retVal = -RT_ERR_FILE_READ;
            }
            else
            {
                /* packet process */
                stkHdr.seq        = seqId;
                stkHdr.destAddr   = startAddr + offset;
                stkHdr.dataLength = (uint16)((spiParaLen % DVA_STACK_REMOTE_LOAD_LENGTH_UNIT) & 0xFFFFu);
                retVal            = dva_drv_stacking_load_cfg_process(sMac, dMac, &stkHdr, fwData, seqId);
            }
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dva_drv_stacking_cfg_data_get_chk(const int8 *fileName, uint32 *stkReg, uint32 *slaveLoc)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == fileName) || (NULL == stkReg) || (NULL == slaveLoc))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

RtkApiRet dva_drv_stacking_parse_flash_slave_cfg_header(const int8 *fileName, uint32 *spiStartAddr, uint32 *spiParaLen, uint8 stkIdx)
{
    uint8     configAddrLenBuf[5] = {(uint8)0u};
    void     *fp                  = NULL;
    RtkApiRet retVal              = RT_ERR_OK;

    do
    {
        if((uint8)3u < stkIdx)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(fp == NULL)
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /* Get config Address and Length from header */
        if((int32)0 == rtl_file_read(fp, configAddrLenBuf, DVA_FLASH_HEADER_OFFSET + 9u + (stkIdx * 5u), 5u))
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s read fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }
        *spiParaLen   = ((uint32)configAddrLenBuf[3] << 4u) + ((uint32)configAddrLenBuf[4] << 12u);
        *spiStartAddr = (uint32)configAddrLenBuf[0] | ((uint32)configAddrLenBuf[1] << 8u) | ((uint32)configAddrLenBuf[2] << 16u);
    } while(0u == 1u);

    rtl_file_close(&fp);

    return retVal;
}

RtkApiRet dva_drv_stacking_cfg_data_get(const int8 *fileName, uint32 *stkReg, uint32 slaveUnit, uint32 *slaveLoc)
{
    RtkApiRet retVal                                    = RT_ERR_OK;
    uint32    i                                         = 0u;
    uint32    tmpVal                                    = 0;
    uint32    offset                                    = 3u;
    void     *pFile                                     = NULL;
    uint8     stkRegTmp[DVA_STACK_MAC_DOMAIN_ENTRY_LEN] = {0u};
    uint32    memberNum;
    uint32    spiStartAddr[DVA_CFG_END_IDX];
    uint32    spiParaLen[DVA_CFG_END_IDX];

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_cfg_data_get_chk(fileName, stkReg, slaveLoc));
        CHK_FUN_CONTINUE(retVal, dva_drv_config_parse_flash_cfg_header(fileName, spiStartAddr, spiParaLen, 0u));

        /* Open files */
        retVal = rtl_file_open(&pFile, 0u, fileName);
        if(pFile == NULL)
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /* Get stacking config reg */
        if((int32)0 == rtl_file_read(pFile, stkRegTmp, spiStartAddr[DVA_CFG_MAC_CONF_IDX], DVA_STACK_MAC_DOMAIN_ENTRY_LEN))
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s read fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        for(i = 0; i < DVA_STACK_CFG_SIZE; ++i)
        {
            stkReg[i] = ((uint32)stkRegTmp[offset]) | ((uint32)stkRegTmp[offset + 1u] << 8u) | ((uint32)stkRegTmp[offset + 2u] << 16u) | ((uint32)stkRegTmp[offset + 3u] << 24u);
            offset += 7u;
        }
        CHK_FUN_CONTINUE(retVal, dva_reg_field_get(0, DVA_STACK_DMY1_MEMBER_NUM_LSP, DVA_STACK_DMY1_MEMBER_NUM_LEN, &memberNum, stkReg[1]));
        for(i = 1; i < memberNum; ++i)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_get(0, DVA_STACK_DMY2_UNIT_LSP, DVA_STACK_DMY2_UNIT_LEN, &tmpVal, stkReg[2u + i]));
            if(tmpVal == slaveUnit)
            {
                *slaveLoc = i;
                break;
            }
        }
    } while(0u == 1u);

    rtl_file_close(&pFile);

    return retVal;
}

RtkApiRet dva_drv_stacking_check_alert(void)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint32              rxPktLen                     = DVA_MAX_PKT_LEN;
    uint8               rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet           retVal = RT_ERR_OK;
    uint32              ackId;

    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

        /* Parse packet */
        ptr    = &rxPkt[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        retVal = drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(retVal == RT_ERR_OK)
        {
            ackId = rtl_htonl(command.ack_id);

            if((rtl_memcmp(&rxPkt[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0) || (command.RTL9040_Identity != rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY)))
            {
                retVal = -RT_ERR_FAILED;
            }
            else if((uint32)DVA_CPU_READY != ackId)
            {
                retVal = -RT_ERR_FAILED;
            }
            else
            {
                /* intentionally left blank */
                /* check packet correct */
            }
        }
    } while(0u == 1u);

    return retVal;
}