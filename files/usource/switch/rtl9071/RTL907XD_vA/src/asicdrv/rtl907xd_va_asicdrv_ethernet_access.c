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
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_list.h"

#include "rtl907xd_va_asicdrv_ethernet_access.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_api_esm.h"

/**
 * @brief Select page with ethernet access
 * @param[in] page number of page
 * @param[out] None
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet dva_drv_eth_acc_flow_page_select(uint32 page)
{
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    int32                 retVal = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_gen_page_select(sMac, dMac, page, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_check_page_select(rxPkt));
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
RtkApiRet dva_drv_eth_acc_gen_page_select(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], uint32 page, EtherAccessL2Header_t *packet)
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
            CHK_FUN_CONTINUE(ret, dva_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)DVA_PAGE_SELECT);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));
            command.rw_value = rtl_htonl((uint32)page);

            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, &command, sizeof(EtherAccessHeader_t)));
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
RtkApiRet dva_drv_eth_acc_check_page_select(uint8 *packet)
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
        ret = dva_drv_memcpy(&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)DVA_CPU_SUCCESS != rtl_htonl(command.ack_id))
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
 * @brief Read page with ethernet access
 * @param[in] page number of page
 * @param[out] None
 * @retval RT_ERR_OK
 * Others are fail
 */
RtkApiRet dva_drv_eth_acc_flow_page_read(uint32 *page)
{
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    int32                 retVal = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_gen_page_read(sMac, dMac, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_check_page_read(rxPkt, page));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Generate a packet for page read.
 * @param[in] sMac MAC address of PC
 * @param[in] dMac address of switch
 * @param[in] page Page of register to access
 * @param[out] packet Packet
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 */
RtkApiRet dva_drv_eth_acc_gen_page_read(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet)
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
    else
    {
        do
        {
            /* Fill in packet */
            CHK_FUN_CONTINUE(ret, dva_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)DVA_PAGE_READ);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, &command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check a packet for page read.
 * @param[in] packet Packet
 * @param[out] page Page of ethernet access
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_NULL_POINTER
 * Others are fail
 */
RtkApiRet dva_drv_eth_acc_check_page_read(uint8 *packet, uint32 *page)
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
        *page = 0u;
        ret   = dva_drv_memcpy(&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)DVA_CPU_SUCCESS != rtl_htonl(command.ack_id))
            {
                ret = -RT_ERR_FAILED;
            }
            else
            {
                /* check packet correct */
                *page = rtl_htonl((uint32)command.rw_value);
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
uint16 dva_drv_eth_acc_checksum(const EtherAccessIpHdr_t *header)
{
    uint32    sum     = 0;
    uint16    oddbyte = 0;
    uint16    data[sizeof(EtherAccessIpHdr_t)];
    uint16   *ptr    = (uint16 *)data;
    uint32    nbytes = DVA_ETHERACCESS_IPHDR_SIZE;
    RtkApiRet ret    = RT_ERR_OK;

    ret = dva_drv_memcpy((uint8 *)data, (const uint8 *)header, sizeof(EtherAccessIpHdr_t));
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

uint32 dva_drv_eth_acc_data_checksum(const uint8 *data, uint32 len, uint32 checksum)
{
    uint32 i;
    uint32 tmpChecksum = checksum;

    for(i = 0; i < len; ++i)
    {
        tmpChecksum += data[i];
    }

    return tmpChecksum;
}

RtkApiRet dva_drv_eth_acc_gen_burst_rw_check(uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue)
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
            if((regCmd[i] <= 0u) || (regCmd[i] > DVA_ETHERACCESS_REGISTER_END))
            {
                ret = -RT_ERR_INPUT;
                break;
            }
        }
    }

    return ret;
}

RtkApiRet dva_drv_eth_acc_gen_patch_start_check(uint32 addr, uint32 size, EtherAccessL2Header_t *packet)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else if((addr >= DVA_ETHERACCESS_MAX_PATCH_SIZE) || (size <= 0u) || (size > DVA_ETHERACCESS_MAX_PATCH_SIZE) || ((addr + size) > DVA_ETHERACCESS_MAX_PATCH_SIZE))
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

RtkApiRet dva_drv_eth_acc_gen_write_check(const uint8 *data, const uint32 seqId, EtherAccessL2Header_t *packet)
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

RtkApiRet dva_drv_eth_acc_flash_write(uint32 addr, uint32 len, const uint8 *data, uint32 checksum)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen    = DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = DVA_MAX_PKT_LEN;
    uint32                curChecksum = 0;
    uint32                i, j;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    if(len <= DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
    {
        /* packet process flash write */
        CHK_FUN_RET(retVal, dva_eth_acc_gen_flash_write(mac, addr, data, len, seqId, checksum, &txPkt));
        seqId++;
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
    }
    else
    {
        for(j = 0; j < (len / DVA_RTK_ETHERACCESS_BYTE_PER_WRITE); j++)
        {
            if(retVal != RT_ERR_OK)
            {
                break;
            }
            for(i = 0; i < DVA_RTK_ETHERACCESS_BYTE_PER_WRITE; i++)
            {
                curChecksum += data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE) + i];
            }

            /* packet process flash write */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_write(mac, addr, &data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], DVA_RTK_ETHERACCESS_BYTE_PER_WRITE, seqId, curChecksum, &txPkt));
            seqId++;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            /* parse data */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
            rxPktLen = DVA_MAX_PKT_LEN;
        }
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_patch_write(uint32 addr, uint32 len, const uint8 *data, uint32 checksum)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen    = DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = DVA_MAX_PKT_LEN;
    uint32                remainLen   = 0;
    uint32                curChecksum = 0;
    uint32                j;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));
    for(j = 0; j < (len / DVA_RTK_ETHERACCESS_BYTE_PER_WRITE); j++)
    {
        if(retVal != RT_ERR_OK)
        {
            break;
        }
        curChecksum = dva_drv_eth_acc_data_checksum(&data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], DVA_RTK_ETHERACCESS_BYTE_PER_WRITE, curChecksum);

        /* packet process patch write */
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_patch_write(mac, addr, &data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], DVA_RTK_ETHERACCESS_BYTE_PER_WRITE, seqId, curChecksum, &txPkt));
        seqId++;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
        rxPktLen = DVA_MAX_PKT_LEN;
    }
    remainLen = len % DVA_RTK_ETHERACCESS_BYTE_PER_WRITE;
    if(remainLen != 0u)
    {
        curChecksum = dva_drv_eth_acc_data_checksum(&data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], remainLen, curChecksum);

        /* packet process patch write */
        CHK_FUN_RET(retVal, dva_eth_acc_gen_patch_write(mac, addr, &data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], remainLen, seqId, curChecksum, &txPkt));
        seqId++;
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_flow_flash_status_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint32 delay)
{
    uint32                txPktLen = DVA_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal    = -RT_ERR_ETH_POLLING;
    uint32                retryTime = 0;

    if(rtk_usecond_delay(delay) != RT_ERR_OK)
    {
        retVal = -RT_ERR_FAILED;
    }
    else
    {
        while((retVal == -RT_ERR_ETH_POLLING) && (retryTime < RTK_ETH_ACC_RETRY))
        {
            ++retryTime;
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(RTK_ETH_ACC_FLASH_POLLING_TIME));
            /* Send flash status check */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_status_check(sMac, dMac, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            retVal   = dva_eth_acc_check_flash_status_check(rxPkt);
            rxPktLen = DVA_MAX_PKT_LEN;
        }
    }
    if(retVal == -RT_ERR_ETH_POLLING)
    {
        dva_esm_event_log_set(DVA_EM_EVENT_ETHERNET_ACCESS_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_flow_flash_write_req_check(uint32 addr, uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(((addr & 0xFFu) != 0u) || (len <= 0u) || ((len > DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE) && ((len % DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE) != 0u)) || (addr >= DVA_ETHERACCESS_MAX_FLASH_SIZE) || ((DVA_ETHERACCESS_MAX_FLASH_SIZE - addr) < len))
    {
        retVal = -RT_ERR_INPUT;
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_flash_write_req(uint32 addr, uint32 len, const uint8 *data, uint32 checksum)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen    = DVA_RTK_ETHERACCESS_ETH_FLASH_REQ_WRITE_LEN;
    uint32                rxPktLen    = DVA_MAX_PKT_LEN;
    uint32                curChecksum = 0;
    uint32                j;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    if(len <= DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE)
    {
        /* packet process flash write */
        CHK_FUN_RET(retVal, dva_eth_acc_gen_flash_write_req(mac, addr, data, len, seqId, checksum, &txPkt));
        seqId++;
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
        CHK_FUN_RET(retVal, dva_drv_eth_acc_flow_flash_status_check(mac.sMac, mac.dMac, RTK_ETH_ACC_FLASH_WRITE_TIME));
    }
    else
    {
        for(j = 0; j < (len / DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE); j++)
        {
            if(retVal != RT_ERR_OK)
            {
                break;
            }
            curChecksum = dva_drv_eth_acc_data_checksum(&data[j * DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE], DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE, curChecksum);

            /* packet process flash write */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_write_req(mac, addr, &data[(j * DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE)], DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE, seqId, curChecksum, &txPkt));
            seqId++;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            /* parse data */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
            retVal   = dva_drv_eth_acc_flow_flash_status_check(mac.sMac, mac.dMac, RTK_ETH_ACC_FLASH_WRITE_TIME);
            rxPktLen = DVA_MAX_PKT_LEN;
        }
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_check_flash_event(const uint32 option)
{
    RtkApiRet ret = RT_ERR_OK;

    if(((uint32)DVA_SPI_STATE_ERASING == option) || ((uint32)DVA_SPI_STATE_ERASE == option))
    {
        ret = -RT_ERR_ETH_POLLING;
    }
    else if(((uint32)DVA_SPI_STATE_WRITE == option) || ((uint32)DVA_SPI_STATE_WRITEING == option))
    {
        ret = -RT_ERR_ETH_POLLING;
    }
    else
    {
        ret = -RT_ERR_FAILED;
    }

    return ret;
}

RtkApiRet dva_drv_eth_acc_flash_write_event(uint32 addr, uint32 len, const uint8 *data, uint32 checksum)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen    = DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = DVA_MAX_PKT_LEN;
    uint32                curChecksum = 0;
    uint32                i, j;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));

    if(len <= DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
    {
        /* packet process flash write */
        CHK_FUN_RET(retVal, dva_eth_acc_gen_flash_write_event(mac, addr, data, len, seqId, checksum, &txPkt));
        seqId++;
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
    }
    else
    {
        for(j = 0; j < (len / DVA_RTK_ETHERACCESS_BYTE_PER_WRITE); j++)
        {
            if(retVal != RT_ERR_OK)
            {
                break;
            }
            for(i = 0; i < DVA_RTK_ETHERACCESS_BYTE_PER_WRITE; i++)
            {
                curChecksum += data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE) + i];
            }

            /* packet process flash write */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_write_event(mac, addr, &data[(j * DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)], DVA_RTK_ETHERACCESS_BYTE_PER_WRITE, seqId, curChecksum, &txPkt));
            seqId++;
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            /* parse data */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
            rxPktLen = DVA_MAX_PKT_LEN;
        }
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_flow_flash_get_event(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint32 delay)
{
    uint32                txPktLen = DVA_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal    = -RT_ERR_ETH_POLLING;
    uint32                retryTime = 0;

    if(rtk_usecond_delay(delay) != RT_ERR_OK)
    {
        retVal = -RT_ERR_FAILED;
    }
    else
    {
        while((retVal == -RT_ERR_ETH_POLLING) && (retryTime < RTK_ETH_ACC_RETRY))
        {
            ++retryTime;
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(RTK_ETH_ACC_FLASH_POLLING_TIME));
            /* Send flash status check */
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_event(sMac, dMac, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            retVal   = dva_eth_acc_check_flash_event(rxPkt);
            rxPktLen = DVA_MAX_PKT_LEN;
        }
    }
    if(retVal == -RT_ERR_ETH_POLLING)
    {
        dva_esm_event_log_set(DVA_EM_EVENT_ETHERNET_ACCESS_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_flow_flash_event_write(const EtherAccessMac_t mac, uint32 addr, uint32 len, uint32 writePerBurst, const uint8 *data)
{
    uint32                txPktLen    = DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = DVA_MAX_PKT_LEN;
    uint32                checksum    = 0;
    uint32                curChecksum = 0;
    uint32                addrOffset  = 0;
    uint32                processLen  = 0;
    uint32                i;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    while((addrOffset < len) && (retVal == RT_ERR_OK))
    {
        checksum    = 0;
        curChecksum = 0;
        processLen  = ((len - addrOffset) > writePerBurst) ? writePerBurst : (len - addrOffset);
        for(i = 0u; i < processLen; i++)
        {
            checksum += data[addrOffset + i];
        }
        /* packet process flash start */
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_start(mac, addr + addrOffset, processLen, DVA_RTK_ETHERACCESS_BYTE_PER_WRITE, checksum, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_flash_write_event(addr + addrOffset, processLen, &data[addrOffset], checksum));
        CHK_FUN_CONTINUE(retVal, dva_drv_eth_acc_flow_flash_get_event(mac.sMac, mac.dMac, RTK_ETH_ACC_FLASH_WRITE_BASE_TIME * (processLen >> 10u)));
        rxPktLen = DVA_MAX_PKT_LEN;
        addrOffset += processLen;
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_flow_flash_read_event(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 len, uint8 *data)
{
    uint32                packetNum = (len + DVA_RTK_ETHERACCESS_BYTE_PER_WRITE - 1u) / DVA_RTK_ETHERACCESS_BYTE_PER_WRITE;
    uint32                txPktLen  = DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen  = DVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    do
    {
        txPktLen = DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_read_event(sMac, dMac, addr, len, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_check_rep_read_event(rxPkt));
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(RTK_ETH_ACC_FLASH_READ_BASE_TIME * packetNum));
    } while(1u == 0u);

    return retVal;
}

RtkApiRet dva_drv_eth_acc_flow_flash_check_read(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], uint32 len, uint8 *data)
{
    uint32                packetNum = (len + DVA_RTK_ETHERACCESS_BYTE_PER_WRITE - 1u) / DVA_RTK_ETHERACCESS_BYTE_PER_WRITE;
    uint32                offset    = 0;
    uint32                tmpLen;
    uint32                i;
    uint32                retryTime = 0;
    uint32                txPktLen  = DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen  = DVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    tmpLen = len;
    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(RTK_ETH_ACC_FLASH_POLLING_TIME));
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_read_check(sMac, dMac, offset, tmpLen, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxPkt, &rxPktLen));
        /* parse data */
        retVal   = dva_eth_acc_check_flash_read_check(rxPkt, offset, tmpLen, data);
        rxPktLen = DVA_MAX_PKT_LEN;
        ++retryTime;
    } while((retVal == -RT_ERR_ETH_ONGOING) && (retryTime < RTK_ETH_ACC_RETRY));
    if(retVal == -RT_ERR_ETH_ONGOING)
    {
        dva_esm_event_log_set(DVA_EM_EVENT_ETHERNET_ACCESS_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
    }
    tmpLen = (tmpLen > DVA_RTK_ETHERACCESS_BYTE_PER_WRITE) ? (tmpLen - DVA_RTK_ETHERACCESS_BYTE_PER_WRITE) : 0u;
    offset += DVA_RTK_ETHERACCESS_BYTE_PER_WRITE;

    for(i = 1; i < packetNum; ++i)
    {
        if(retVal != RT_ERR_OK)
        {
            break;
        }
        CHK_FUN_CONTINUE(retVal, dva_eth_acc_gen_flash_read_check(sMac, dMac, offset, tmpLen, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxPkt, &rxPktLen));
        /* parse data */
        retVal = dva_eth_acc_check_flash_read_check(rxPkt, offset, tmpLen, data);
        tmpLen = (tmpLen > DVA_RTK_ETHERACCESS_BYTE_PER_WRITE) ? (tmpLen - DVA_RTK_ETHERACCESS_BYTE_PER_WRITE) : 0u;
        offset += DVA_RTK_ETHERACCESS_BYTE_PER_WRITE;
        rxPktLen = DVA_MAX_PKT_LEN;
    }

    return retVal;
}

RtkApiRet dva_drv_eth_acc_table_check(dva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 regNum, const uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == data)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    /* Check error input */
    else if((table >= DVA_TABLE_LIST_END) || (startIndex >= dva_table_list[table].size) || (entryNum > dva_table_list[table].size) || ((startIndex + entryNum) > dva_table_list[table].size) || (regNum > RTK_ETHERACCESS_MAX_BURST))
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

RtkApiRet dva_drv_eth_acc_table_entry_get(uint32 entryRegNum, uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, uint32 *data)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen = DVA_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u);
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint32                i, j;
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));

    /* packet process */
    CHK_FUN_RET(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, 0u, &txPkt));
    CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
    CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

    /* parse data */
    CHK_FUN_RET(retVal, dva_eth_acc_check_burst_rw(regNum, regValue, rxPkt));

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

RtkApiRet dva_drv_eth_acc_table_entry_set(uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen = DVA_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u);
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(mac.dMac));
    CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(mac.sMac));

    /* packet process */
    CHK_FUN_RET(retVal, dva_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, 0u, &txPkt));
    CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
    CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

    /* parse data */
    CHK_FUN_RET(retVal, dva_eth_acc_check_burst_rw(regNum, regValue, rxPkt));

    return retVal;
}

RtkApiRet dva_drv_eth_acc_check_l3_otp_rep_null(void *packet, const uint8 *uuid, uint32 *ackId, uint8 *data)
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
RtkApiRet dva_drv_eth_acc_table_get(uint32 unit, dva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(true == dva_reg_check_unit(unit))
    {
        retVal = dva_eth_acc_flow_table_entry_get(table, startIndex, entryNum, data);
    }
    else
    {
#ifdef RTK_MODULE_STACKING
        retVal = dva_drv_stacking_eth_table_get(unit, table, startIndex, entryNum, data);
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
RtkApiRet dva_drv_eth_acc_table_set(uint32 unit, dva_table_list_t table, uint32 startIndex, uint32 entryNum, const uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(true == dva_reg_check_unit(unit))
    {
        retVal = dva_eth_acc_flow_table_entry_set(table, startIndex, entryNum, data);
    }
    else
    {
#ifdef RTK_MODULE_STACKING
        retVal = dva_drv_stacking_eth_table_set(unit, table, startIndex, entryNum, data);
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
RtkApiRet dva_drv_eth_acc_stacking_gen_burst(const EtherAccessMac_t mac, dva_StackBurstReg_t *stkHdr, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue, dva_StackEthBurstHeader_t *packet)
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
        CHK_FUN_CONTINUE(ret, dva_drv_eth_acc_gen_burst_rw_check(number, regCmd, regAddr, regValue));
        ptr = (uint8 *)&packet[0];
        /* Fill in packet */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(packet, 0, sizeof(dva_StackEthBurstHeader_t)));
        CHK_FUN_CONTINUE(ret, dva_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
        command.RTL9040_Identity = rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY);
        command.command_id       = rtl_htonl((uint32)DVA_STK_REG_BURST);
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy((uint8 *)&command.mac_address_pc_03, mac.sMac, MAC_ADDR_LEN));

        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, mac.dMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, mac.sMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, (uint8 *)stkHdr, sizeof(dva_StackBurstReg_t)));
        ptr = &ptr[sizeof(dva_StackBurstReg_t)];
        for(i = 0u; i < number; i++)
        {
            reg_rw.command = regCmd[i];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy((uint8 *)&reg_rw.reg_addr, regAddr[i], RTK_REG_ADDR_LEN));
            reg_rw.value = rtl_htonl(regValue[i]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(ptr, (uint8 *)&reg_rw, sizeof(EtherAccessRegister_t)));
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
RtkApiRet dva_drv_eth_acc_stacking_check_burst(dva_StackBurstReg_t *stkHdr, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, uint8 *packet)
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
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy((uint8 *)stkHdr, ptr, sizeof(dva_StackBurstReg_t)));

            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            if(command.RTL9040_Identity != rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            if((uint32)DVA_CPU_SUCCESS != rtl_htonl(command.ack_id))
            {
                ret = -RT_ERR_FAILED;
            }
            ptr = &ptr[sizeof(dva_StackBurstReg_t)];

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
RtkApiRet dva_drv_eth_acc_stk_gen_fw(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], dva_StackPayloadRemoteLoad_t *stkHdr, const uint8 *data, dva_StackEthFwHeader_t *packet)
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
    else
    {
        /* Fill in packet */
        (void)dva_drv_memset(packet, 0, sizeof(dva_StackEthFwHeader_t));
        (void)dva_drv_memset(&command, 0, sizeof(EtherAccessHeader_t));
        command.RTL9040_Identity = rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY);
        command.command_id       = rtl_htonl((uint32)DVA_STK_LOAD_FW);
        command.ack_id           = rtl_htonl((uint32)DVA_CPU_ONGOING);
        command.sequence_id      = rtl_htonl((uint32)stkHdr->seq);
        (void)dva_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN);

        (void)dva_drv_memcpy(ptr, dMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)dva_drv_memcpy(ptr, sMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)dva_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN);
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        (void)dva_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        (void)dva_drv_memcpy(ptr, (uint8 *)&stkHdr[0], sizeof(dva_StackPayloadRemoteLoad_t));
        ptr = &ptr[sizeof(dva_StackPayloadRemoteLoad_t)];
        (void)dva_drv_memcpy(ptr, data, DVA_STACK_REMOTE_LOAD_LENGTH_UNIT);
    }

    return ret;
}

RtkApiRet dva_drv_eth_acc_stk_gen_cfg(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], dva_StackPayloadRemoteLoad_t *stkHdr, const uint8 *data, dva_StackEthFwHeader_t *packet)
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
    else
    {
        /* Fill in packet */
        (void)dva_drv_memset(packet, 0, sizeof(dva_StackEthFwHeader_t));
        (void)dva_drv_memset(&command, 0, sizeof(EtherAccessHeader_t));
        command.RTL9040_Identity = rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY);
        command.command_id       = rtl_htonl((uint32)DVA_STK_LOAD_CFG);
        command.ack_id           = rtl_htonl((uint32)DVA_CPU_ONGOING);
        command.sequence_id      = rtl_htonl((uint32)stkHdr->seq);
        (void)dva_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN);

        (void)dva_drv_memcpy(ptr, dMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)dva_drv_memcpy(ptr, sMac, MAC_ADDR_LEN);
        ptr = &ptr[MAC_ADDR_LEN];
        (void)dva_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN);
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        (void)dva_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        (void)dva_drv_memcpy(ptr, (uint8 *)&stkHdr[0], sizeof(dva_StackPayloadRemoteLoad_t));
        ptr = &ptr[sizeof(dva_StackPayloadRemoteLoad_t)];
        (void)dva_drv_memcpy(ptr, data, stkHdr->dataLength);
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
RtkApiRet dva_drv_eth_acc_stk_check_rep(const uint8 *packet, const uint16 seqId)
{
    EtherAccessHeader_t command;
    const uint8        *ptr                          = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint16              packetSeqId;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        (void)dva_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        packetSeqId = (uint16)(rtl_htonl(command.sequence_id) & 0xFFFFu);
        if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
        {
            ret = -RT_ERR_FAILED;
        }
        if(command.RTL9040_Identity != rtl_htonl((uint32)DVA_RTK_ETHERACCESS_IDENTITY))
        {
            ret = -RT_ERR_FAILED;
        }
        if(packetSeqId != seqId)
        {
            ret = -RT_ERR_FAILED;
        }
        if(rtl_htonl((uint32)DVA_CPU_SUCCESS) != command.ack_id)
        {
            ret = -RT_ERR_FAILED;
        }
    }

    return ret;
}
#endif
