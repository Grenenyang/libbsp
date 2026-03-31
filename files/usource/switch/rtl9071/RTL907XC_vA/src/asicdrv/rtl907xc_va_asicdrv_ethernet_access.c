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

#include "rtkas_types.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_reg_list.h"

#include "rtl907xc_va_asicdrv_ethernet_access.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"

/**
 * @brief Select page with ethernet access
 * @param[in] page number of page
 * @param[out] None
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_flow_page_select(uint32 page)
{
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    uint32                rxPktLen = CVA_MAX_PKT_LEN;
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    int32                 retVal = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_gen_page_select(sMac, dMac, page, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVA_RTK_ETHERACCESS_HEADER_LEN));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cva_drv_eth_acc_check_page_select(rxPkt));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Generate a packet for page select.
 * @param[in] sMac MAC address of PC
 * @param[in] dMac address of switch
 * @param[in] page Page of register to access
 * @param[out] packet Packet
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_gen_page_select(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], uint32 page, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    uint8               etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr                          = (uint8 *)&packet[0];
    ;
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else if(page > 0xFFu)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Fill in packet */
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVA_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)CVA_PAGE_SELECT);
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));
            command.rw_value = rtl_htonl((uint32)page);

            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, &command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check a packet for page select.
 * @param[in] packet Packet
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_check_page_select(uint8 *packet)
{
    EtherAccessHeader_t command;
    uint8              *ptr                          = (uint8 *)&packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
    uint8               etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        ret = cva_drv_drv_memcpy(&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVA_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVA_CPU_SUCCESS != rtl_htonl(command.ack_id))
            {
                ret = -RT_ERR_FAILED;
            }
            else
            {
                /* intentionally left blank */
                /* check packet correct */
            }
        }
    }

    return ret;
}

/**
 * @brief Calculate ipv4 checksum.
 * @param[in] header Header of the packet
 * @param[out] None
 * @retval ~sum - Value of checksum
 */
uint16 cva_drv_eth_acc_checksum(const EtherAccessIpHdr_t *header)
{
    uint32    sum     = 0;
    uint16    oddbyte = 0;
    uint16    data[sizeof(EtherAccessIpHdr_t)];
    uint16   *ptr    = (uint16 *)data;
    uint32    nbytes = CVA_ETHERACCESS_IPHDR_SIZE;
    RtkApiRet ret    = RT_ERR_OK;

    ret = cva_drv_drv_memcpy((uint8 *)data, (const uint8 *)header, sizeof(EtherAccessIpHdr_t));
    if(ret == RT_ERR_OK)
    {
        while(nbytes > 1u)
        {
            sum += (uint32)rtl_htons(*ptr);
            ptr = &ptr[1];
            nbytes -= 2u;
        }
        if(nbytes == 1u)
        {
            oddbyte = (*ptr & 0xff00u);
            sum += (uint32)rtl_htons(oddbyte);
        }

        sum = (sum >> 16u) + (sum & 0xffffu);
        sum += (sum >> 16u);
        sum = ~sum;
    }

    return (uint16)sum;
}

uint32 cva_drv_eth_acc_data_checksum(const uint8 *data, uint32 len, uint32 checksum)
{
    uint32 i;
    uint32 tmpChecksum = checksum;

    for(i = 0; i < len; ++i)
    {
        tmpChecksum += data[i];
    }

    return tmpChecksum;
}

RtkApiRet cva_drv_eth_acc_gen_burst_rw_check(uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue)
{
    uint32    i;
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == regCmd) || (NULL == regAddr) || (NULL == regValue))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check error input */
    else if((0u >= number) || (RTK_ETHERACCESS_MAX_BURST < number))
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        for(i = 0; i < number; ++i)
        {
            if((regCmd[i] <= 0u) || (regCmd[i] > CVA_ETHERACCESS_REGISTER_END))
            {
                ret = -RT_ERR_INPUT;
                break;
            }
        }
    }

    return ret;
}

RtkApiRet cva_drv_eth_acc_gen_patch_start_check(uint32 addr, uint32 size, EtherAccessL2Header_t *packet)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else if((addr >= CVA_ETHERACCESS_MAX_PATCH_SIZE) || (size <= 0u) || (size > CVA_ETHERACCESS_MAX_PATCH_SIZE) || ((addr + size) > CVA_ETHERACCESS_MAX_PATCH_SIZE))
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        /* intentionally left blank */
        /* check packet correct */
    }

    return ret;
}

RtkApiRet cva_drv_eth_acc_gen_write_check(const uint8 *data, const uint32 seqId, EtherAccessL2Header_t *packet)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == data) || (NULL == packet))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if(seqId <= 0u)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        /* intentionally left blank */
        /* check packet correct */
    }

    return ret;
}

RtkApiRet cva_drv_eth_acc_flash_write(uint32 addr, uint32 len, const uint8 *data, uint32 checksum)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen    = CVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = CVA_MAX_PKT_LEN;
    uint32                curChecksum = 0;
    uint32                i, j;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    if(len <= CVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
    {
        /* packet process flash write */
        CHK_FUN_RET(retVal, cva_eth_acc_gen_flash_write(mac, addr, data, len, seqId, checksum, &txPkt));
        seqId++;
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, cva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
    }
    else
    {
        for(j = 0; j < (len / CVA_RTK_ETHERACCESS_BYTE_PER_WRITE); j++)
        {
            if(retVal != RT_ERR_OK)
            {
                break;
            }
            for(i = 0; i < CVA_RTK_ETHERACCESS_BYTE_PER_WRITE; i++)
            {
                curChecksum += data[(j * CVA_RTK_ETHERACCESS_BYTE_PER_WRITE) + i];
            }

            /* packet process flash write */
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_flash_write(mac, addr, &data[(j * CVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], CVA_RTK_ETHERACCESS_BYTE_PER_WRITE, seqId, curChecksum, &txPkt));
            seqId++;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            /* parse data */
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
            rxPktLen = CVA_MAX_PKT_LEN;
        }
    }

    return retVal;
}

RtkApiRet cva_drv_eth_acc_patch_write(uint32 addr, uint32 len, const uint8 *data, uint32 checksum)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen    = CVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = CVA_MAX_PKT_LEN;
    uint32                remainLen   = 0;
    uint32                curChecksum = 0;
    uint32                j;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));
    for(j = 0; j < (len / CVA_RTK_ETHERACCESS_BYTE_PER_WRITE); j++)
    {
        if(retVal != RT_ERR_OK)
        {
            break;
        }
        curChecksum = cva_drv_eth_acc_data_checksum(&data[(j * CVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], CVA_RTK_ETHERACCESS_BYTE_PER_WRITE, curChecksum);

        /* packet process patch write */
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_patch_write(mac, addr, &data[(j * CVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], CVA_RTK_ETHERACCESS_BYTE_PER_WRITE, seqId, curChecksum, &txPkt));
        seqId++;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
        rxPktLen = CVA_MAX_PKT_LEN;
    }
    remainLen = len % CVA_RTK_ETHERACCESS_BYTE_PER_WRITE;
    if(remainLen != 0u)
    {
        curChecksum = cva_drv_eth_acc_data_checksum(&data[(j * CVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], remainLen, curChecksum);

        /* packet process patch write */
        CHK_FUN_RET(retVal, cva_eth_acc_gen_patch_write(mac, addr, &data[(j * CVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], remainLen, seqId, curChecksum, &txPkt));
        seqId++;
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, cva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
    }

    return retVal;
}

RtkApiRet cva_drv_eth_acc_reset_system_check(const EtherAccessMac_t mac)
{
    /* 2 reg + 1 empty */
    uint32 regNumStep1                                                = CVA_RESET_CHECK_NUM;
    uint8  regCmdStep1[(CVA_RESET_CHECK_NUM) + 1u]                    = {CVA_ETHERACCESS_REGISTER_RW_MAC_READ, CVA_ETHERACCESS_REGISTER_RW_SRAM_READ, 0u};
    uint8  regAddrStep1[(CVA_RESET_CHECK_NUM) + 1u][RTK_REG_ADDR_LEN] = {{CVA_CPU_DMY_2_1623, CVA_CPU_DMY_2_815, CVA_CPU_DMY_2_07},
                                                                         {CVA_ETH_RESET_ENABLE_1623, CVA_ETH_RESET_ENABLE_815, CVA_ETH_RESET_ENABLE_07},
                                                                         {0, 0, 0}};
    uint32 regValueStep1[(CVA_RESET_CHECK_NUM) + 1u]                  = {0};
    uint32 regRetValueStep1[(CVA_RESET_CHECK_NUM) + 1u]               = {0};
    /* 1 reg + 1 empty */
    uint32                regNumStep2                                                = CVA_RESET_WRITE_NUM;
    uint8                 regCmdStep2[(CVA_RESET_WRITE_NUM) + 1u]                    = {CVA_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u};
    uint8                 regAddrStep2[(CVA_RESET_WRITE_NUM) + 1u][RTK_REG_ADDR_LEN] = {{CVA_ETH_RESET_ENABLE_1623, CVA_ETH_RESET_ENABLE_815, CVA_ETH_RESET_ENABLE_07},
                                                                                        {0, 0, 0}};
    uint32                regValueStep2[(CVA_RESET_WRITE_NUM) + 1u]                  = {0};
    uint32                regRetValueStep2[(CVA_RESET_WRITE_NUM) + 1u]               = {0};
    uint32                rxPktLen                                                   = CVA_MAX_PKT_LEN;
    uint32                txPktLen                                                   = 0u;
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    EtherAccessL2Header_t txPkt;
    int32                 retVal = RT_ERR_OK;

    do
    {
        /* packet process (fw version check) */
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_burst_rw(mac, regNumStep1, regCmdStep1, &regAddrStep1[0], regValueStep1, &txPkt));
        txPktLen = CVA_RTK_ETHERACCESS_HEADER_LEN + ((regNumStep1 + 1u) * 8u);
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        rxPktLen = CVA_MAX_PKT_LEN;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_burst_rw(regNumStep1, regRetValueStep1, rxPkt));

        /* packet process (fw version set) */
        if(regRetValueStep1[0] == 0u)
        {
            regValueStep2[0] = regRetValueStep1[1] | ((uint32)0x1u << CVA_ETH_RESET_ENABLE_BIT);
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_gen_burst_rw(mac, regNumStep2, regCmdStep2, &regAddrStep2[0], regValueStep2, &txPkt));
            txPktLen = CVA_RTK_ETHERACCESS_HEADER_LEN + ((regNumStep2 + 1u) * 8u);
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            rxPktLen = CVA_MAX_PKT_LEN;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            /* parse data */
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_check_burst_rw(regNumStep2, regRetValueStep2, rxPkt));
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cva_drv_eth_acc_table_check(cva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 regNum, const uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == data)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    /* Check error input */
    else if((table >= CVA_TABLE_LIST_END) || (startIndex >= cva_table_list[table].size) || (entryNum > cva_table_list[table].size) || ((startIndex + entryNum) > cva_table_list[table].size) || (regNum > RTK_ETHERACCESS_MAX_BURST))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* intentionally left blank */
        /* check packet correct */
    }

    return retVal;
}

RtkApiRet cva_drv_eth_acc_table_entry_get(uint32 entryRegNum, uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, uint32 *data)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen = CVA_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u);
    uint32                rxPktLen = CVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    uint32                i, j;
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    /* packet process */
    CHK_FUN_RET(retVal, cva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, &txPkt));
    CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
    CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

    /* parse data */
    CHK_FUN_RET(retVal, cva_eth_acc_check_burst_rw(regNum, regValue, rxPkt));

    if(retVal == RT_ERR_OK)
    {
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

    return retVal;
}

RtkApiRet cva_drv_eth_acc_table_entry_set(uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen = CVA_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u);
    uint32                rxPktLen = CVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    /* packet process */
    CHK_FUN_RET(retVal, cva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, &txPkt));
    CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
    CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

    /* parse data */
    CHK_FUN_RET(retVal, cva_eth_acc_check_burst_rw(regNum, regValue, rxPkt));

    return retVal;
}

RtkApiRet cva_drv_eth_acc_check_l3_otp_rep_null(EtherAccessL2IpHdr_t *packet, const uint8 *uuid, uint32 *ackId, uint8 *data)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == packet) || (NULL == uuid) || (NULL == ackId) || (NULL == data))
    {
        ret = -RT_ERR_NULL_POINTER;
    }

    return ret;
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
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_table_get(uint32 unit, cva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(true == cva_reg_check_unit(unit))
    {
        retVal = cva_eth_acc_flow_table_entry_get(table, startIndex, entryNum, data);
    }
    else
    {
#ifdef RTK_MODULE_STACKING
        retVal = cva_drv_stacking_eth_table_get(unit, table, startIndex, entryNum, data);
#endif
    }

    return retVal;
}

/**
 * @brief Table dump.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] table Table
 * @param[in] startIndex Start index
 * @param[in] entryNum Entry number
 * @param[in] data Data
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_table_set(uint32 unit, cva_table_list_t table, uint32 startIndex, uint32 entryNum, const uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(true == cva_reg_check_unit(unit))
    {
        retVal = cva_eth_acc_flow_table_entry_set(table, startIndex, entryNum, data);
    }
    else
    {
#ifdef RTK_MODULE_STACKING
        retVal = cva_drv_stacking_eth_table_set(unit, table, startIndex, entryNum, data);
#endif
    }

    return retVal;
}

#ifdef RTK_MODULE_STACKING
/**
 * @brief Generate a packet for register read/write burst.
 * @param[in] sMac MAC address of PC
 * @param[in] dMac Destination MAC address
 * @param[in] stkHdr Stacking header of the packet
 * @param[in] number Number of register to read/write
 * @param[in] regCmd Command of register to read/write
 * @param[in] regAddr Address of register to read/write
 * @param[in] regValue Vlaue of register to write
 * @param[out] packet Packet
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_stacking_gen_burst(const EtherAccessMac_t mac, cva_StackBurstReg_t *stkHdr, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue, cva_StackEthBurstHeader_t *packet)
{
    EtherAccessHeader_t   command;
    EtherAccessRegister_t reg_rw;
    const uint8           etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8                *ptr;
    uint32                i;
    RtkApiRet             ret = RT_ERR_OK;

    do
    {
        /* Check error input */
        if(packet == NULL)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(ret, cva_drv_eth_acc_gen_burst_rw_check(number, regCmd, regAddr, regValue));
        ptr = (uint8 *)&packet[0];
        /* Fill in packet */
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(packet, 0, sizeof(cva_StackEthBurstHeader_t)));
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
        command.RTL9040_Identity = rtl_htonl((uint32)CVA_RTK_ETHERACCESS_IDENTITY);
        command.command_id       = rtl_htonl((uint32)CVA_STK_REG_BURST);
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, mac.sMac, MAC_ADDR_LEN));

        CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, mac.dMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, mac.sMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, (uint8 *)stkHdr, sizeof(cva_StackBurstReg_t)));
        ptr = &ptr[sizeof(cva_StackBurstReg_t)];
        for(i = 0u; i < number; i++)
        {
            reg_rw.command = regCmd[i];
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy((uint8 *)&reg_rw.reg_addr, regAddr[i], RTK_REG_ADDR_LEN));
            reg_rw.value = rtl_htonl(regValue[i]);
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(ptr, (uint8 *)&reg_rw, sizeof(EtherAccessRegister_t)));
            ptr = &ptr[sizeof(EtherAccessRegister_t)];
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check a packet for register read/write burst.
 * @param[in] number Number of register to read/write
 * @param[in] regCmd Command of register to read/write
 * @param[in] regAddr Address of register to read/write
 * @param[in] packet Packet
 * @param[out] stkHdr Stacking header of the packet
 * @param[out] regValue Value of register to write
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_stacking_check_burst(cva_StackBurstReg_t *stkHdr, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, uint8 *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint32              i;
    uint8              *ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
    RtkApiRet           ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == regCmd) || (NULL == regAddr) || (NULL == regValue) || (NULL == packet))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check error input */
    else if((0u >= number) || (RTK_ETHERACCESS_MAX_BURST < number))
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Parse packet */
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy((uint8 *)stkHdr, ptr, sizeof(cva_StackBurstReg_t)));

            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            if(command.RTL9040_Identity != rtl_htonl((uint32)CVA_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            if((uint32)CVA_CPU_SUCCESS != rtl_htonl(command.ack_id))
            {
                ret = -RT_ERR_FAILED;
            }
            ptr = &ptr[sizeof(cva_StackBurstReg_t)];

            for(i = 0u; i < number; i++)
            {
                if(i != 0u)
                {
                    ptr = &ptr[8u];
                }
                else
                {
                    ptr = &ptr[4u];
                }
                regValue[i] = (((uint32)ptr[0]) << 24u) | (((uint32)ptr[1]) << 16u) | (((uint32)ptr[2]) << 8u) | ((uint32)ptr[3]);
            }
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Generate a packet for fw write.
 * @param[in] sMac MAC address of PC
 * @param[in] dMac Destination MAC address
 * @param[in] stkHdr Stacking sub header
 * @param[in] stkHdr Stacking sub header
 * @param[out] packet Packet
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 * @note 1. 1492 byte is necessary to be allocated for the packet.(1142)
 */
RtkApiRet cva_drv_eth_acc_stk_gen_fw(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], cva_StackPayloadRemoteLoad_t *stkHdr, const uint8 *data, cva_StackEthFwHeader_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr                          = (uint8 *)&packet[0];
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == stkHdr) || (NULL == data) || (NULL == packet))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if(stkHdr->seq <= 0u)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        /* Fill in packet */
        (void)cva_drv_drv_memset(packet, 0, sizeof(cva_StackEthFwHeader_t));
        (void)cva_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t));
        command.RTL9040_Identity = rtl_htonl((uint32)CVA_RTK_ETHERACCESS_IDENTITY);
        command.command_id       = rtl_htonl((uint32)CVA_STK_LOAD_FW);
        command.ack_id           = rtl_htonl((uint32)CVA_CPU_ONGOING);
        command.sequence_id      = rtl_htonl((uint32)stkHdr->seq);
        (void)cva_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN);

        (void)cva_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)cva_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)cva_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN);
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        (void)cva_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        (void)cva_drv_drv_memcpy(ptr, (uint8 *)&stkHdr[0], sizeof(cva_StackPayloadRemoteLoad_t));
        ptr = &ptr[sizeof(cva_StackPayloadRemoteLoad_t)];
        (void)cva_drv_drv_memcpy(ptr, data, CVA_STACK_REMOTE_LOAD_LENGTH_UNIT);
    }

    return ret;
}

RtkApiRet cva_drv_eth_acc_stk_gen_cfg(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], cva_StackPayloadRemoteCfg_t *stkHdr, const uint8 *data, cva_StackEthCfgHeader_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr                          = (uint8 *)&packet[0];
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == stkHdr) || (NULL == data) || (NULL == packet))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if(stkHdr->seq <= 0u)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        /* Fill in packet */
        (void)cva_drv_drv_memset(packet, 0, sizeof(cva_StackEthCfgHeader_t));
        (void)cva_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t));
        command.RTL9040_Identity = rtl_htonl((uint32)CVA_RTK_ETHERACCESS_IDENTITY);
        command.command_id       = rtl_htonl((uint32)CVA_STK_LOAD_CFG);
        command.ack_id           = rtl_htonl((uint32)CVA_CPU_ONGOING);
        command.sequence_id      = rtl_htonl((uint32)stkHdr->seq);
        (void)cva_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN);

        (void)cva_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)cva_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)cva_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN);
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        (void)cva_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        (void)cva_drv_drv_memcpy(ptr, (uint8 *)&stkHdr[0], sizeof(cva_StackPayloadRemoteCfg_t));
        ptr = &ptr[sizeof(cva_StackPayloadRemoteCfg_t)];
        (void)cva_drv_drv_memcpy(ptr, data, stkHdr->cfgLength);
    }

    return ret;
}

/**
 * @brief Check the ethertype, RTL9040_Identity, sequence id, and ack id of the packet is correct.
 * @param[in] packet Packet
 * @param[in] seqId Sequence ID
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 */
RtkApiRet cva_drv_eth_acc_stk_check_rep(const uint8 *packet, const uint32 seqId)
{
    EtherAccessHeader_t command;
    const uint8        *ptr                          = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint32              packetSeqId;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if(seqId <= 0u)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        /* Parse packet */
        (void)cva_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        packetSeqId = rtl_htonl(command.sequence_id);
        if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
        {
            ret = -RT_ERR_FAILED;
        }
        if(command.RTL9040_Identity != rtl_htonl((uint32)CVA_RTK_ETHERACCESS_IDENTITY))
        {
            ret = -RT_ERR_FAILED;
        }
        if(packetSeqId != seqId)
        {
            ret = -RT_ERR_FAILED;
        }
        if(rtl_htonl((uint32)CVA_CPU_SUCCESS) != command.ack_id)
        {
            ret = -RT_ERR_FAILED;
        }
    }

    return ret;
}
#endif
