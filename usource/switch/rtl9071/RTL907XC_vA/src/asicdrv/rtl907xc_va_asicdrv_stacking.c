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
#include "rtl907xc_va_api.h"
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtkas_types.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_table_struct.h"
#include "rtl907xc_va_reg_list.h"
#include "rtkas_debug.h"
#include "rtl907xc_va_asicdrv_stacking.h"
#include "rtl907xc_va_asicdrv_ethernet_access.h"
#include "rtl907xc_va_asicdrv_config.h"
#ifdef RTL906X_CMD_SHOW
#include "rtl906x_glue.h"
#endif

#define CVA_STACK_MAX_NUM 4u

uint32 cva_g_stackingTimeout = STACKING_TIMEOUT;

void cva_drv_stacking_eth_table_data(uint32 regNum, uint32 entryRegNum, const uint32 *regValue, uint32 *data);

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
RtkApiRet cva_drv_stacking_dump_reg(uint32 unit, uint8 target, cva_StackSwitchInfo_t *pThisSwitch)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;

    CHK_FUN_RET(retVal, cva_drv_drv_memset(pThisSwitch, 0, sizeof(cva_StackSwitchInfo_t)));
    tmpVal = (uint32)target;

    /* Set dump command */
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_STACK_COMMAND_REGISTER, ((tmpVal << 16) | CVA_STACK_COMMAND_DUMP_REG)));

    /* Read dump value 1 */
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_STACK_STATUS_REG_3, &tmpVal));
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
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_STACK_STATUS_REG_4, &tmpVal));
    pThisSwitch->unitID      = (uint8)(tmpVal & 0x0000000Fu);
    pThisSwitch->masterID    = (uint8)((tmpVal & 0x000000F0u) >> 4u);
    pThisSwitch->portToRelay = (uint8)((tmpVal & 0x00000F00u) >> 8u);
    pThisSwitch->portToInit  = (uint8)((tmpVal & 0x0000F000u) >> 12u);
    pThisSwitch->groupID     = (uint16)((tmpVal & 0xFFFF0000u) >> 16u);

    /* Read dump value 3 */
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_STACK_STATUS_REG_5, &tmpVal));
    pThisSwitch->unitMapId[0]   = (uint8)(tmpVal & 0x0000000Fu);
    pThisSwitch->unitMapPort[0] = (uint8)((tmpVal & 0x000000F0u) >> 4u);
    pThisSwitch->unitMapId[1]   = (uint8)((tmpVal & 0x00000F00u) >> 8u);
    pThisSwitch->unitMapPort[1] = (uint8)((tmpVal & 0x0000F000u) >> 12u);
    pThisSwitch->unitMapId[2]   = (uint8)((tmpVal & 0x000F0000u) >> 16u);
    pThisSwitch->unitMapPort[2] = (uint8)((tmpVal & 0x00F00000u) >> 20u);
    pThisSwitch->unitMapId[3]   = (uint8)((tmpVal & 0x0F000000u) >> 24u);
    pThisSwitch->unitMapPort[3] = (uint8)((tmpVal & 0xF0000000u) >> 28u);

    /* Read dump value 4 */
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_STACK_STATUS_REG_1, &tmpVal));
    pThisSwitch->model         = (uint16)((tmpVal & 0xFFFF0000u) >> 16u);
    pThisSwitch->macAddress[0] = (uint8)((tmpVal & 0x0000FF00u) >> 8u);
    pThisSwitch->macAddress[1] = (uint8)(tmpVal & 0x000000FFu);

    /* Read dump value 5 */
    CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_STACK_STATUS_REG_2, &tmpVal));
    pThisSwitch->macAddress[2] = (uint8)((tmpVal & 0xFF000000u) >> 24u);
    pThisSwitch->macAddress[3] = (uint8)((tmpVal & 0x00FF0000u) >> 16u);
    pThisSwitch->macAddress[4] = (uint8)((tmpVal & 0x0000FF00u) >> 8u);
    pThisSwitch->macAddress[5] = (uint8)(tmpVal & 0x000000FFu);
    return retVal;
}

static RtkApiRet cva_drv_stacking_eth_reg_read_send(uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue)
{
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    uint32                rxPktLen = CVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVA_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_burst_rw(regNum, regValue, rxPkt));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cva_drv_stacking_eth_reg_read_wait(uint32 unit, uint32 *value)
{
    uint8                 regCmd[CVA_STACKING_REG_RWCMD] = {CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[CVA_STACKING_REG_RWCMD][RTK_REG_ADDR_LEN];
    uint32                regVal[CVA_STACKING_REG_RWCMD];
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    uint32                rxPktLen = CVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    uint32                timeout = 0u;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* Wait for response */
        stkCmd = ((((uint32)unit) << 8u) | 0x30u);
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        regCmd[0]     = CVA_ETHERACCESS_REGISTER_RW_MAC_READ;
        regAddr[0][0] = (uint8)CVA_STACK_COMMAND_REGISTER_1623;
        regAddr[0][1] = (uint8)CVA_STACK_COMMAND_REGISTER_815;
        regAddr[0][2] = (uint8)CVA_STACK_COMMAND_REGISTER_07;
        regVal[0]     = 0;
        while((stkCmd != 0u) && (timeout < cva_g_stackingTimeout) && (retVal == RT_ERR_OK))
        {
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_burst_rw(mac, 1, regCmd, &regAddr[0], regVal, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVA_RTK_ETHERACCESS_HEADER_LEN + (2u * 8u)));
            rxPktLen = CVA_MAX_PKT_LEN;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_burst_rw(1, regVal, rxPkt));
            stkCmd = regVal[0];
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
            timeout++;
        }

        /* Timeout handling */
        if(timeout == cva_g_stackingTimeout)
        {
            *value = 0;
            retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
        }
        else
        {
            /* Return value on the register field */
            regCmd[0]     = CVA_ETHERACCESS_REGISTER_RW_MAC_READ;
            regAddr[0][0] = (uint8)CVA_STACK_COMMAND_REG_VALUE_1623;
            regAddr[0][1] = (uint8)CVA_STACK_COMMAND_REG_VALUE_815;
            regAddr[0][2] = (uint8)CVA_STACK_COMMAND_REG_VALUE_07;
            regVal[0]     = 0;
            CHK_FUN_CONTINUE(retVal, cva_drv_stacking_eth_reg_read_send(1, regCmd, &regAddr[0], regVal));
            *value = regVal[0];
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
RtkApiRet cva_drv_stacking_eth_reg_read(uint32 unit, uint32 address, uint32 *value)
{
    uint8                 regCmd[CVA_STACKING_REG_RWCMD] = {CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[CVA_STACKING_REG_RWCMD][RTK_REG_ADDR_LEN];
    uint32                reg_val[CVA_STACKING_REG_RWCMD];
    uint32                regNum = CVA_STACKING_REG_RWCMD;
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    uint32                rxPktLen = CVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    EtherAccessL2Header_t txPkt;

    do
    {
        if(unit == cva_g_stackingMaster)
        {
            retVal = -RT_ERR_INPUT;
        }
        stkCmd = ((((uint32)unit) << 8u) | 0x30u);
        /* Set register value field to 0 */
        regAddr[0][0] = (uint8)CVA_STACK_COMMAND_REG_VALUE_1623;
        regAddr[0][1] = (uint8)CVA_STACK_COMMAND_REG_VALUE_815;
        regAddr[0][2] = (uint8)CVA_STACK_COMMAND_REG_VALUE_07;
        reg_val[0]    = 0u;
        /* Set register address */
        regAddr[1][0] = (uint8)CVA_STACK_COMMAND_REG_ADDR_1623;
        regAddr[1][1] = (uint8)CVA_STACK_COMMAND_REG_ADDR_815;
        regAddr[1][2] = (uint8)CVA_STACK_COMMAND_REG_ADDR_07;
        reg_val[1]    = address;
        /* Issue stacking register read */
        regAddr[2][0] = (uint8)CVA_STACK_COMMAND_REGISTER_1623;
        regAddr[2][1] = (uint8)CVA_STACK_COMMAND_REGISTER_815;
        regAddr[2][2] = (uint8)CVA_STACK_COMMAND_REGISTER_07;
        reg_val[2]    = stkCmd;

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], reg_val, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVA_RTK_ETHERACCESS_HEADER_LEN + (((regNum + 1u)) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_burst_rw(regNum, reg_val, rxPkt));

        /* Wait for response */
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_eth_reg_read_wait(unit, value));
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
RtkApiRet cva_drv_stacking_eth_reg_write(uint32 unit, uint32 address, uint32 value)
{
    uint8                 regCmd[CVA_STACKING_REG_RWCMD] = {CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 regAddr[CVA_STACKING_REG_RWCMD][RTK_REG_ADDR_LEN];
    uint32                reg_val[CVA_STACKING_REG_RWCMD];
    uint32                regNum = CVA_STACKING_REG_RWCMD;
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    uint32                rxPktLen = CVA_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    uint32                stkCmd;
    EtherAccessL2Header_t txPkt;

    do
    {
        if(unit == cva_g_stackingMaster)
        {
            retVal = -RT_ERR_INPUT;
        }
        stkCmd = ((((uint32)unit) << 8u) | 0x40u);
        /* Set register value field to 0 */
        regAddr[0][0] = (uint8)CVA_STACK_COMMAND_REG_VALUE_1623;
        regAddr[0][1] = (uint8)CVA_STACK_COMMAND_REG_VALUE_815;
        regAddr[0][2] = (uint8)CVA_STACK_COMMAND_REG_VALUE_07;
        reg_val[0]    = value;
        /* Set register address */
        regAddr[1][0] = (uint8)CVA_STACK_COMMAND_REG_ADDR_1623;
        regAddr[1][1] = (uint8)CVA_STACK_COMMAND_REG_ADDR_815;
        regAddr[1][2] = (uint8)CVA_STACK_COMMAND_REG_ADDR_07;
        reg_val[1]    = address;
        /* Issue stacking register write */
        regAddr[2][0] = (uint8)CVA_STACK_COMMAND_REGISTER_1623;
        regAddr[2][1] = (uint8)CVA_STACK_COMMAND_REGISTER_815;
        regAddr[2][2] = (uint8)CVA_STACK_COMMAND_REGISTER_07;
        reg_val[2]    = stkCmd;

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], reg_val, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVA_RTK_ETHERACCESS_HEADER_LEN + (((regNum + 1u)) * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_burst_rw(regNum, reg_val, rxPkt));
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
RtkApiRet cva_drv_stacking_reg_read_internal(uint32 unit, uint32 regAddr, uint32 *value)
#else
RtkApiRet cva_drv_stacking_reg_read(uint32 unit, uint32 regAddr, uint32 *value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    stkAddr = CVA_STACKING_PAGE | (regAddr & CVA_STACKING_ADDR_MSK);
    uint32    stkCmd;
    uint32    timeout = 0u;

    if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((unit <= 0u) || (unit > CVA_STACK_MAX_UNIT_ID))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            retVal = cva_drv_stacking_eth_reg_read(unit, stkAddr, value);
        }
        else
        {
            do
            {
                stkCmd = ((((uint32)unit) << 8u) | 0x30u);
                /* Set register value field to 0 */
                CHK_FUN_CONTINUE(retVal, reg_direct_write(CVA_STACK_COMMAND_REG_VALUE, 0u));
                /* Set register address */
                CHK_FUN_CONTINUE(retVal, reg_direct_write(CVA_STACK_COMMAND_REG_ADDR, stkAddr));
                /* Issue stacking register read */
                CHK_FUN_CONTINUE(retVal, reg_direct_write(CVA_STACK_COMMAND_REGISTER, stkCmd));

                /* Wait for response */
                while((stkCmd != 0u) && (timeout < cva_g_stackingTimeout) && (retVal == RT_ERR_OK))
                {
                    CHK_FUN_CONTINUE(retVal, reg_direct_read(CVA_STACK_COMMAND_REGISTER, &stkCmd));
                    CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
                    timeout++;
                }

                /* Timeout handling */
                if(timeout == cva_g_stackingTimeout)
                {
                    *value = 0;
                    retVal = -RT_ERR_STACK_REMOTE_NO_RESP;
                }
                else
                {
                    /* Return value on the register field */
                    CHK_FUN_CONTINUE(retVal, reg_direct_read(CVA_STACK_COMMAND_REG_VALUE, value));
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
RtkApiRet cva_drv_stacking_reg_write_internal(uint32 unit, uint32 regAddr, uint32 value)
#else
RtkApiRet cva_drv_stacking_reg_write(uint32 unit, uint32 regAddr, uint32 value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    stkAddr = CVA_STACKING_PAGE | (regAddr & CVA_STACKING_ADDR_MSK);
    uint32    stkCmd;

    if(g_RegIF == (uint8)REGIF_ETHERNET)
    {
        retVal = cva_drv_stacking_eth_reg_write(unit, stkAddr, value);
    }
    else if((unit <= 0u) || (unit > CVA_STACK_MAX_UNIT_ID))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        stkCmd = ((((uint32)unit) << 8u) | 0x40u);
        /* Set register value field */
        CHK_FUN_RET(retVal, reg_direct_write(CVA_STACK_COMMAND_REG_ADDR, stkAddr));
        /* Set register address field */
        CHK_FUN_RET(retVal, reg_direct_write(CVA_STACK_COMMAND_REG_VALUE, value));
        /* Issue stacking register write */
        CHK_FUN_RET(retVal, reg_direct_write(CVA_STACK_COMMAND_REGISTER, stkCmd));
    }

    return retVal;
}

static RtkApiRet cva_drv_stacking_eth_table_send(cva_StackBurstReg_t *stkHdr, uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue)
{
    EtherAccessMac_t          mac;
    uint32                    txPktLen = CVA_STACK_BURST_READ_WRITE_LEN;
    uint32                    rxPktLen = CVA_MAX_PKT_LEN;
    cva_StackEthBurstHeader_t txPkt;
    uint8                     rxPkt[CVA_MAX_PKT_LEN];
    RtkApiRet                 retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    /* packet process */
    CHK_FUN_RET(retVal, cva_drv_eth_acc_stacking_gen_burst(mac, stkHdr, regNum, regCmd, &regAddr[0], regValue, &txPkt));
    CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
    CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

    /* parse data */
    CHK_FUN_RET(retVal, cva_drv_eth_acc_stacking_check_burst(stkHdr, regNum, regCmd, &regAddr[0], regValue, rxPkt));

    return retVal;
}

void cva_drv_stacking_eth_table_data(uint32 regNum, uint32 entryRegNum, const uint32 *regValue, uint32 *data)
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

static RtkApiRet cva_drv_stacking_eth_table_helper(uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, cva_StackBurstReg_t *stkHdr, uint32 rw, uint32 type, uint32 *wTableReg)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* make etherAccess payload */
    CHK_FUN_RET(retVal, cva_drv_drv_memset(regCmd, 0, RTK_ETHERACCESS_MAX_BURST));
    CHK_FUN_RET(retVal, cva_drv_drv_memset(regAddr, 0, RTK_ETHERACCESS_MAX_BURST * RTK_REG_ADDR_LEN));
    CHK_FUN_RET(retVal, cva_drv_drv_memset(regValue, 0, RTK_ETHERACCESS_MAX_BURST * 4u));
    CHK_FUN_RET(retVal, cva_drv_drv_memset(stkHdr, 0, sizeof(cva_StackBurstReg_t)));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_ETH_INCR, CVA_ETH_INCR_EXE_LSP, CVA_ETH_INCR_EXE_LEN, 1, wTableReg));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_ETH_INCR, CVA_ETH_INCR_RW_LSP, CVA_ETH_INCR_RW_LEN, rw, wTableReg));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_ETH_INCR, CVA_ETH_INCR_TYPE_LSP, CVA_ETH_INCR_TYPE_LEN, type, wTableReg));

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
RtkApiRet cva_drv_stacking_eth_table_get(uint32 unit, cva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data)
{
    uint32                wTableReg = 0;
    uint32                wTableRegTmp;
    uint32                entryRegNum = cva_table_list[table].datareg_num + 1u;
    uint32                regNum      = entryNum * entryRegNum;
    uint8                 regCmd[RTK_ETHERACCESS_MAX_BURST];
    uint8                 regAddr[RTK_ETHERACCESS_MAX_BURST][RTK_REG_ADDR_LEN];
    uint32                regValue[RTK_ETHERACCESS_MAX_BURST];
    cva_StackBurstReg_t   stkHdr;
    cva_StackSwitchInfo_t pThisSwitch;
    uint32                i, j;
    RtkApiRet             retVal = RT_ERR_OK;

    do
    {
        /* Check input */
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_table_check(table, startIndex, entryNum, regNum, data));
        /* make etherAccess payload */
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_eth_table_helper(regCmd, regAddr, regValue, &stkHdr, 1, cva_table_list[table].type, &wTableReg));
        for(i = 0; i < entryNum; i++)
        {
            regCmd[i * entryRegNum]     = CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[i * entryRegNum][0] = (CVA_ETH_INCR >> 16u) & 0xFFu;
            regAddr[i * entryRegNum][1] = (CVA_ETH_INCR >> 8u) & 0xFFu;
            regAddr[i * entryRegNum][2] = (CVA_ETH_INCR)&0xFFu;
            wTableRegTmp                = wTableReg;
            CHK_FUN_RET(retVal, cva_reg_field_set(CVA_ETH_INCR, CVA_ETH_INCR_ADDR_LSP, CVA_ETH_INCR_ADDR_LEN, startIndex + i, &wTableRegTmp));
            regValue[i * entryRegNum] = wTableRegTmp;
            for(j = 1; j <= cva_table_list[table].datareg_num; j++)
            {
                regCmd[(i * entryRegNum) + j]     = CVA_ETHERACCESS_REGISTER_RW_MAC_READ;
                regAddr[(i * entryRegNum) + j][0] = (CVA_ETH_INDR_BASE >> 16u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][1] = (CVA_ETH_INDR_BASE >> 8u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][2] = (uint8)(CVA_ETH_INDR_BASE + ((j - 1u) * 4u)) & 0xFFu;
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_dump_reg(cva_g_stackingMaster, 0u, &pThisSwitch));
        stkHdr.rwNum    = regNum;
        stkHdr.groupId  = pThisSwitch.groupID;
        stkHdr.fromUnit = (uint8)(cva_g_stackingMaster & 0xffu);
        stkHdr.toUnit   = (uint8)(unit & 0xffu);

        /* packet process */
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_eth_table_send(&stkHdr, regNum, regCmd, &regAddr[0], regValue));

        if((stkHdr.groupId != pThisSwitch.groupID) || (stkHdr.toUnit != (uint8)(cva_g_stackingMaster & 0xffu)) || (stkHdr.fromUnit != (uint8)(unit & 0xffu)))
        {
            retVal = -RT_ERR_STACK_TABLE_FAIL;
        }
        else
        {
            cva_drv_stacking_eth_table_data(regNum, entryRegNum, regValue, data);
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
RtkApiRet cva_drv_stacking_eth_table_set(uint32 unit, cva_table_list_t table, uint32 startIndex, uint32 entryNum, const uint32 *data)
{
    uint32                wTableReg = 0;
    uint32                wTableRegTmp;
    uint32                entryRegNum = cva_table_list[table].datareg_num + 1u;
    uint32                regNum      = entryNum * entryRegNum;
    uint8                 regCmd[RTK_ETHERACCESS_MAX_BURST];
    uint8                 regAddr[RTK_ETHERACCESS_MAX_BURST][RTK_REG_ADDR_LEN];
    uint32                regValue[RTK_ETHERACCESS_MAX_BURST];
    cva_StackBurstReg_t   stkHdr;
    cva_StackSwitchInfo_t pThisSwitch;
    uint32                i, j;
    RtkApiRet             retVal = RT_ERR_OK;

    do
    {
        /* Check input */
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_table_check(table, startIndex, entryNum, regNum, data));
        /* make etherAccess payload */
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_eth_table_helper(regCmd, regAddr, regValue, &stkHdr, 0, cva_table_list[table].type, &wTableReg));
        for(i = 0; i < entryNum; i++)
        {
            for(j = 0; j < cva_table_list[table].datareg_num; j++)
            {
                regCmd[(i * entryRegNum) + j]     = CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE;
                regAddr[(i * entryRegNum) + j][0] = (CVA_ETH_INDR_BASE >> 16u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][1] = (CVA_ETH_INDR_BASE >> 8u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][2] = (uint8)(CVA_ETH_INDR_BASE + (j * 4u)) & 0xFFu;
                regValue[(i * entryRegNum) + j]   = data[(i * cva_table_list[table].datareg_num) + j];
            }
            regCmd[(i * entryRegNum) + j]     = CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[(i * entryRegNum) + j][0] = (CVA_ETH_INCR >> 16u) & 0xFFu;
            regAddr[(i * entryRegNum) + j][1] = (CVA_ETH_INCR >> 8u) & 0xFFu;
            regAddr[(i * entryRegNum) + j][2] = (CVA_ETH_INCR)&0xFFu;
            wTableRegTmp                      = wTableReg;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_set(CVA_ETH_INCR, CVA_ETH_INCR_ADDR_LSP, CVA_ETH_INCR_ADDR_LEN, startIndex + i, &wTableRegTmp));
            regValue[(i * entryRegNum) + j] = wTableRegTmp;
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_dump_reg(cva_g_stackingMaster, 0u, &pThisSwitch));
        stkHdr.rwNum    = regNum;
        stkHdr.groupId  = pThisSwitch.groupID;
        stkHdr.fromUnit = (uint8)(cva_g_stackingMaster & 0xffu);
        stkHdr.toUnit   = (uint8)(unit & 0xffu);

        /* packet process */
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_eth_table_send(&stkHdr, regNum, regCmd, &regAddr[0], regValue));
        if((stkHdr.groupId != pThisSwitch.groupID) || (stkHdr.toUnit != (uint8)(cva_g_stackingMaster & 0xffu)) || (stkHdr.fromUnit != (uint8)(unit & 0xffu)))
        {
            retVal = -RT_ERR_STACK_TABLE_FAIL;
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cva_drv_stacking_check_connection_status(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;
    uint8     status = 0u;

    do
    {
        /* Dump stacking status to register */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_STACK_COMMAND_REGISTER, ((tmpVal << 16u) | CVA_STACK_COMMAND_DUMP_REG)));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_STACK_STATUS_REG_3, &tmpVal));
        status = (uint8)(tmpVal & 0x000000FFu);

        /* Return if stacking is not working */
        if((status == CVA_STACK_STS_DISABLE) || (status == CVA_STACK_STS_ERROR))
        {
            retVal = -RT_ERR_STACK_NOT_INIT;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cva_drv_stacking_load_fw_header(void *pFile, uint32 *fileAddr, uint32 *fwLen, uint32 *stkReg, cva_StackPayloadRemoteLoad_t *stkHdr, uint32 slaveUnit, uint32 slaveLoc)
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
            CHK_FUN_CONTINUE(retVal, cva_drv_config_get_hdr_fw_info(pFile, fileAddr, fwLen, &stkHdr->fwAseType));
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY1_FW_LOAD_TYPE_LSP, CVA_STACK_DMY1_FW_LOAD_TYPE_LEN, &tmpVal, stkReg[1]));
            stkHdr->fwSrcType = (tmpVal == CVA_STACK_LOAD_FW_BY_FLASH) ? CVA_STACK_REMOTE_LOAD_FLAG_FLASH : CVA_STACK_REMOTE_LOAD_FLAG_SRAM;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY1_GROUP_ID_LSP, CVA_STACK_DMY1_GROUP_ID_LEN, &tmpVal, stkReg[1]));
            stkHdr->groupId = (uint16)tmpVal;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY1_MASTER_ID_LSP, CVA_STACK_DMY1_MASTER_ID_LEN, &tmpVal, stkReg[1]));
            stkHdr->masterId = (uint8)tmpVal;
            stkHdr->destId   = (uint8)slaveUnit;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY2_MAC4_LSP, CVA_STACK_DMY2_MAC4_LEN, &tmpVal, stkReg[slaveLoc + 2u]));
            stkHdr->desMAC4 = 0u;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY2_MAC5_LSP, CVA_STACK_DMY2_MAC5_LEN, &tmpVal, stkReg[slaveLoc + 2u]));
            stkHdr->desMAC5 = 0u;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cva_drv_stacking_load_fw_process(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], cva_StackPayloadRemoteLoad_t *stkHdr, uint8 *fwData, uint16 seqId)
{
    uint32                 txPktLen = (uint32)sizeof(cva_StackEthFwHeader_t);
    uint32                 rxPktLen = CVA_MAX_PKT_LEN;
    cva_StackEthFwHeader_t txPkt;
    uint8                  rxPkt[CVA_MAX_PKT_LEN];
    RtkApiRet              retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_stk_gen_fw(sMac, dMac, stkHdr, fwData, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

        /* parse data */
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_stk_check_rep(rxPkt, seqId));
    } while(0u == 1u);

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
RtkApiRet cva_drv_stacking_load_fw(void *pFile, uint32 *stkReg, uint32 slaveUnit, uint32 slaveLoc)
{
    uint8                        sMac[MAC_ADDR_LEN];
    uint8                        dMac[MAC_ADDR_LEN];
    uint8                        fwData[CVA_STACK_REMOTE_LOAD_LENGTH_UNIT];
    cva_StackPayloadRemoteLoad_t stkHdr;
    uint32                       i;
    uint32                       fwLen     = 0;
    uint32                       fileAddr  = 0;
    uint32                       startAddr = CVA_STACK_L2_MEM_BASE;
    uint32                       offset    = 0;
    uint16                       seqId     = 1;
    RtkApiRet                    retVal    = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == pFile) || (NULL == stkReg))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            /* filled stacking header */
            CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(&stkHdr, 0, sizeof(cva_StackPayloadRemoteLoad_t)));
            CHK_FUN_CONTINUE(retVal, cva_drv_stacking_load_fw_header(pFile, &fileAddr, &fwLen, stkReg, &stkHdr, slaveUnit, slaveLoc));
            if(stkHdr.fwSrcType == CVA_STACK_REMOTE_LOAD_FLAG_FLASH)
            {
                CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
                CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
                stkHdr.dataLength  = CVA_STACK_REMOTE_LOAD_LENGTH_UNIT;
                stkHdr.seqTotal    = (uint16)((fwLen + (CVA_STACK_REMOTE_LOAD_LENGTH_UNIT - 1u)) / (CVA_STACK_REMOTE_LOAD_LENGTH_UNIT));
                stkHdr.oriStAddr   = startAddr;
                stkHdr.pktTotalLen = fwLen;
                for(i = 0; i < (fwLen / CVA_STACK_REMOTE_LOAD_LENGTH_UNIT); ++i)
                {
                    if((uint32)rtl_file_read(pFile, fwData, fileAddr + offset, CVA_STACK_REMOTE_LOAD_LENGTH_UNIT) != CVA_STACK_REMOTE_LOAD_LENGTH_UNIT)
                    {
                        retVal = -RT_ERR_FILE_READ;
                        break;
                    }

                    /* packet process */
                    stkHdr.seq      = seqId;
                    stkHdr.destAddr = startAddr + offset;
                    CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cva_drv_stacking_load_fw_process(sMac, dMac, &stkHdr, fwData, seqId));
                    ++seqId;
                    offset += CVA_STACK_REMOTE_LOAD_LENGTH_UNIT;
                }
                if((RT_ERR_OK == retVal) && ((fwLen % CVA_STACK_REMOTE_LOAD_LENGTH_UNIT) != 0u))
                {
                    CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(fwData, 0, CVA_STACK_REMOTE_LOAD_LENGTH_UNIT));
                    if((uint32)rtl_file_read(pFile, fwData, fileAddr + offset, (fwLen % CVA_STACK_REMOTE_LOAD_LENGTH_UNIT)) != (fwLen % CVA_STACK_REMOTE_LOAD_LENGTH_UNIT))
                    {
                        retVal = -RT_ERR_FILE_READ;
                    }
                    else
                    {
                        /* packet process */
                        stkHdr.seq      = seqId;
                        stkHdr.destAddr = startAddr + offset;
                        retVal          = cva_drv_stacking_load_fw_process(sMac, dMac, &stkHdr, fwData, seqId);
                    }
                }
            }
        } while(0u == 1u);
    }

    return retVal;
}

/**
 * @brief Parse stacking config
 * @param[in] stkReg stacking config reg
 * @param[in] slaveUnit slave unit
 * @param[out] stkHdr stacking data
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_STACK_LOAD_SLAVE
 * Others are fail
 */
static RtkApiRet cva_drv_stacking_load_cfg_header(uint32 *stkReg, cva_StackPayloadRemoteCfg_t *stkHdr, uint32 slaveUnit, uint32 slaveLoc)
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
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY1_GROUP_ID_LSP, CVA_STACK_DMY1_GROUP_ID_LEN, &tmpVal, stkReg[1]));
            stkHdr->groupId = (uint16)tmpVal;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY1_MASTER_ID_LSP, CVA_STACK_DMY1_MASTER_ID_LEN, &tmpVal, stkReg[1]));
            stkHdr->masterId = (uint8)tmpVal;
            stkHdr->destId   = (uint8)slaveUnit;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY2_MAC4_LSP, CVA_STACK_DMY2_MAC4_LEN, &tmpVal, stkReg[slaveLoc + 2u]));
            stkHdr->desMAC4 = (uint8)tmpVal;
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY2_MAC5_LSP, CVA_STACK_DMY2_MAC5_LEN, &tmpVal, stkReg[slaveLoc + 2u]));
            stkHdr->desMAC5 = (uint8)tmpVal;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cva_drv_stacking_load_cfg_process(void *pFile, const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], cva_StackPayloadRemoteCfg_t *stkHdr, uint32 startAddr, uint32 paraLen, uint16 *seqId)
{
    uint32                  txPktLen = (uint32)sizeof(cva_StackEthCfgHeader_t);
    uint32                  rxPktLen = CVA_MAX_PKT_LEN;
    cva_StackEthCfgHeader_t txPkt;
    uint8                   rxPkt[CVA_MAX_PKT_LEN];
    uint8                   cfgData[CVA_STACK_REMOTE_LOAD_LENGTH_UNIT];
    RtkApiRet               retVal       = RT_ERR_OK;
    uint32                  dataLen      = 0u;
    uint32                  readLen      = 0u;
    uint16                  blockIdx     = 0;
    uint32                  paraLenTmp   = paraLen;
    uint32                  startAddrTmp = startAddr;

    while((RT_ERR_OK == retVal) && (paraLenTmp > 0u))
    {
        /* the readLen should include padding len if exist */
        dataLen           = ((paraLenTmp > CVA_CONFIG_BLOCK_MAX_LENGTH) ? CVA_CONFIG_BLOCK_MAX_LENGTH : paraLenTmp);
        readLen           = ((dataLen + CVA_CONFIG_BLOCK_ALIGNMENT_LENGTH - 1u) & 0xFFF0u);
        stkHdr->cfgLength = (uint16)(readLen & 0xFFFFu);
        stkHdr->seq       = *seqId;
        stkHdr->blkIdx1   = (uint8)((blockIdx >> 8u) & 0xFFu);
        stkHdr->blkIdx2   = (uint8)(blockIdx & 0xFFu);
        ++blockIdx;

        /* load config-block data */
        if((uint32)rtl_file_read(pFile, cfgData, startAddrTmp, readLen) != readLen)
        {
            retVal = -RT_ERR_FILE_READ;
            break;
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_stk_gen_cfg(sMac, dMac, stkHdr, cfgData, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_stk_check_rep(rxPkt, *seqId));
        *seqId += 1u;
        paraLenTmp   = paraLenTmp - dataLen;
        startAddrTmp = startAddrTmp + readLen + CVA_CONFIG_BLOCK_HASH_LENGTH;
        rxPktLen     = CVA_MAX_PKT_LEN;
    }

    return retVal;
}

static RtkApiRet cva_drv_stacking_load_cfg_check(void *pFile, uint32 *stkReg, uint32 *spiStartAddr, uint32 *spiParaLen)
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
RtkApiRet cva_drv_stacking_load_cfg(void *pFile, uint32 *stkReg, uint32 slaveUnit, uint32 *spiStartAddr, uint32 *spiParaLen, uint32 slaveLoc)
{
    uint8                       sMac[MAC_ADDR_LEN];
    uint8                       dMac[MAC_ADDR_LEN];
    cva_StackPayloadRemoteCfg_t stkHdr;
    uint16                      seqId            = 1;
    RtkApiRet                   retVal           = RT_ERR_OK;
    const uint8                 cfgDomainOrder[] = {CVA_CFG_PTP_IDX,
                                                    CVA_CFG_5KR_PATCH_IDX,
                                                    CVA_CFG_STACKING_IDX,
                                                    CVA_CFG_PCIE_IDX,
                                                    CVA_CFG_REG_FIELD_WR_IDX,
                                                    CVA_CFG_MKA_IDX,
                                                    CVA_CFG_ROT_SW_TABLE,
                                                    CVA_CFG_ROT_SW_INTF_TABLE,
                                                    CVA_CFG_MACPHY_0_CONFIG_IDX,
                                                    CVA_CFG_MACPHY_1_CONFIG_IDX,
                                                    CVA_CFG_QCC_PRE_TBL_IDX};
    uint8                       cfgDomainLength  = (uint8)((sizeof(cfgDomainOrder) / sizeof(cfgDomainOrder[0])) & 0xFFu);
    uint8                       domain           = 0u;
    uint32                      i                = 0u;
    uint32                      startAddr        = 0;
    uint32                      paraLen          = 0u;

    do
    {
        /* Check NULL pointer */
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_load_cfg_check(pFile, stkReg, spiStartAddr, spiParaLen));
        /* filled stacking header */
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(&stkHdr, 0, sizeof(cva_StackPayloadRemoteCfg_t)));
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_load_cfg_header(stkReg, &stkHdr, slaveUnit, slaveLoc));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
        stkHdr.seqTotal  = 0xFFFFu;
        stkHdr.randomNum = 0xFFFFFFFFu;
    } while(0u == 1u);

    for(i = 0u; i < cfgDomainLength; ++i)
    {
        if(retVal != RT_ERR_OK)
        {
            break;
        }
        domain       = cfgDomainOrder[i];
        paraLen      = spiParaLen[domain] & 0x0FFFFFu;
        startAddr    = spiStartAddr[domain];
        stkHdr.index = domain;
        retVal       = cva_drv_stacking_load_cfg_process(pFile, sMac, dMac, &stkHdr, startAddr, paraLen, &seqId);
    }

    /* Table cfg */
    for(domain = CVA_CFG_TBL_BASE_IDX; domain <= CVA_CFG_TBL_END_IDX; ++domain)
    {
        if(retVal != RT_ERR_OK)
        {
            break;
        }
        paraLen      = spiParaLen[domain] & 0x0FFFFFu;
        startAddr    = spiStartAddr[domain];
        stkHdr.index = domain;
        if((stkHdr.index >= CVA_CFG_TBL_BASE_IDX) && (stkHdr.index <= CVA_CFG_TBL_END_IDX))
        {
            stkHdr.tableMode = (uint8)((spiParaLen[domain] & 0xF00000u) >> 20u);
        }
        else
        {
            stkHdr.tableMode = 0u;
        }
        retVal = cva_drv_stacking_load_cfg_process(pFile, sMac, dMac, &stkHdr, startAddr, paraLen, &seqId);
    }

    return retVal;
}

static RtkApiRet cva_drv_stacking_cfg_data_get_chk(const int8 *fileName, uint32 *stkReg, uint32 *spiStartAddr, uint32 *spiParaLen, uint32 *slaveLoc)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == fileName) || (NULL == stkReg) || (NULL == spiStartAddr) || (NULL == spiParaLen) || (NULL == slaveLoc))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

RtkApiRet cva_drv_stacking_cfg_data_get(const int8 *fileName, uint32 *stkReg, uint32 slaveUnit, uint32 *spiStartAddr, uint32 *spiParaLen, uint32 *slaveLoc)
{
    RtkApiRet retVal                                    = RT_ERR_OK;
    uint32    i                                         = 0u;
    uint32    tmpVal                                    = 0;
    uint32    offset                                    = 3u;
    void     *pFile                                     = NULL;
    uint8     stkRegTmp[CVA_STACK_MAC_DOMAIN_ENTRY_LEN] = {0u};
    uint32    memberNum;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_stacking_cfg_data_get_chk(fileName, stkReg, spiStartAddr, spiParaLen, slaveLoc));
        CHK_FUN_CONTINUE(retVal, cva_drv_config_parse_flash_cfg_header(fileName, spiStartAddr, spiParaLen, 0u));

        /* Open files */
        CHK_FUN_CONTINUE(retVal, rtl_file_open(&pFile, 0, fileName));

        /* Get stacking config reg */
        if((int32)0 == rtl_file_read(pFile, stkRegTmp, spiStartAddr[CVA_CFG_MAC_CONF_IDX], CVA_STACK_MAC_DOMAIN_ENTRY_LEN))
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s read fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        tmpVal = ((uint32)stkRegTmp[0u]) | ((uint32)stkRegTmp[1u] << 8u) | ((uint32)stkRegTmp[2u] << 16u);
        /* Check stacking config reg */
        if(tmpVal != (CVA_STACK_COMMAND_REGISTER & 0xFFFFFFu))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        for(i = 0; i < CVA_STACK_CFG_SIZE; ++i)
        {
            stkReg[i] = ((uint32)stkRegTmp[offset]) | ((uint32)stkRegTmp[offset + 1u] << 8u) | ((uint32)stkRegTmp[offset + 2u] << 16u) | ((uint32)stkRegTmp[offset + 3u] << 24u);
            offset += 7u;
        }
        CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY1_MEMBER_NUM_LSP, CVA_STACK_DMY1_MEMBER_NUM_LEN, &memberNum, stkReg[1]));
        for(i = 1; i < memberNum; ++i)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_get(0, CVA_STACK_DMY2_UNIT_LSP, CVA_STACK_DMY2_UNIT_LEN, &tmpVal, stkReg[2u + i]));
            if(tmpVal == slaveUnit)
            {
                *slaveLoc = i;
                break;
            }
        }
    } while(0u == 1u);

    rtl_file_close(&pFile);

    if((0u != *slaveLoc) && (retVal == RT_ERR_OK))
    {
        retVal = cva_drv_config_parse_flash_cfg_header(fileName, spiStartAddr, spiParaLen, (uint8)(*slaveLoc & 0xFFu));
    }

    return retVal;
}

RtkApiRet cva_drv_stacking_flashless_file_check(const int8 *fwFileName, const int8 *cfgFileName)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == fwFileName) || (NULL == cfgFileName))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

RtkApiRet cva_drv_stacking_flashless_update_flow_check(uint32 value)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    unit      = CVA_UNIT;
    uint32    data      = 0u;
    uint32    retryTime = 0u;

    /* Check DUT is ready to start load firmware to slave */
    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
        retVal = cva_reg_field_read(unit, CVA_CPU_SPECIAL0, 0u, 8u, &data);
        retryTime++;
    } while((value != data) && (CVA_TIMEOUT_VALUE > retryTime) && (retVal == RT_ERR_OK));
    if(value != data)
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}
