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

#include "rtl907xc_vb_asicdrv_flash.h"
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_asicdrv_ethernet_access.h"

/**
 * @defgroup RTK_MODULE_ETHERNET_ACCESS Ethernet Access
 * @brief Ethernet Access APIs
 * @{
 */

/**
 * @defgroup ETH_FLASH Ethernet Access Flash
 * @{
 */

/**
 * @addtogroup ETH_FLASH_ASIC Low Level Driver
 * @brief ETHERNET ACCESS FLASH Low Level Driver
 * @{
 */

/**
 * @brief Flash write via Ethernet Access.
 * @param[in] addr Flash start write address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the start write position
 * @param[in] len Data length (0x1 to 0x80000) \n
 *                Data length is the length of flash going to write
 * @param[in] data Data buffer (0x0 to 0xFF for len elements) \n
 *                  Data buffer is flash data going to write
 * @param[out] None
 * @retval RT_ERR_OK The flash write was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_eth_acc_gen_flash_start, cvb_eth_acc_check_rep_write, cvb_drv_eth_acc_flash_write
 * @note Range of len and addr are based on size of flash chip, (len + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_flow_flash_write(uint32 addr, uint32 len, const uint8 *data)
{
    EtherAccessMac_t      mac;
    uint32                txPktLen    = CVB_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = CVB_MAX_PKT_LEN;
    uint32                checksum    = 0;
    uint32                curChecksum = 0;
    uint32                i;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == data)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    /* Check error input */
    else if((len <= 0u) || ((len > CVB_RTK_ETHERACCESS_BYTE_PER_WRITE) && ((len % CVB_RTK_ETHERACCESS_BYTE_PER_WRITE) != 0u)) || (addr >= CVB_ETHERACCESS_MAX_FLASH_SIZE) || ((CVB_ETHERACCESS_MAX_FLASH_SIZE - addr) < len))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
            for(i = 0u; i < len; i++)
            {
                checksum += data[i];
            }

            /* packet process flash start */
            CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_flash_start(mac, addr, len, checksum, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            /* parse data */
            CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));
            CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_flash_write(addr, len, data, checksum));
        } while(0u == 1u);
    }

    return retVal;
}
/**
 * @brief Flash read via Ethernet Access.
 * @param[in] addr Flash start read address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the start read position
 * @param[in] len Data length (1 to 0x400) \n
 *                Data length is the length of flash going to read
 * @param[out] data Data buffer (0x0 to 0xFF for len elements) \n
 *                  Data buffer is flash data going to save read
 * @retval RT_ERR_OK The flash read was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_eth_acc_gen_flash_read, cvb_eth_acc_check_rep_read
 * @note Range of addr is based on size of flash chip, (len + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_flow_flash_read(uint32 addr, uint32 len, uint8 *data)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen = CVB_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == data)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    /* Check error input */
    else if((len <= 0u) || (len > CVB_RTK_ETHERACCESS_BYTE_PER_WRITE))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(dMac));

        /* packet process flash read */
        CHK_FUN_RET(retVal, cvb_eth_acc_gen_flash_read(sMac, dMac, addr, len, &txPkt));
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, cvb_eth_acc_check_rep_read(rxPkt, data, len));
    }

    return retVal;
}

/**
 * @brief Flash sector erase via Ethernet Access.
 * @param[in] addr Flash erase address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the erase position
 * @param[in] num flash erase number (0x1 to 4095) \n
 *                 Flash erase number is the number of sector going to be erased
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_eth_acc_gen_flash_erase_sector, cvb_eth_acc_check_flash_erase_sector
 * @note Range of num and addr are based on size of flash chip, ((num * 0x1000)) + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_flow_flash_sec_erase(uint32 addr, uint32 num)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen = CVB_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    /* Check error input */
    if(num <= 0u)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(dMac));

        /* packet process erase block */
        CHK_FUN_RET(retVal, cvb_eth_acc_gen_flash_erase_sector(sMac, dMac, addr, num, &txPkt));
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, cvb_eth_acc_check_flash_erase_sector(rxPkt));
    }

    return retVal;
}

/**
 * @brief Flash block erase via Ethernet Access.
 * @param[in] addr Flash erase address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the erase position
 * @param[in] num flash erase number (0x1 to 256) \n
 *                 Flash erase number is the number of block going to be erased
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_eth_acc_gen_flash_erase_block, cvb_eth_acc_check_flash_erase_block
 * @note Range of num and addr are based on size of flash chip, ((num * 0x10000)) + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_flow_flash_blk_erase(uint32 addr, uint32 num)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen = CVB_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    /* Check error input */
    if(num <= 0u)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(dMac));

        /* packet process erase block */
        CHK_FUN_RET(retVal, cvb_eth_acc_gen_flash_erase_block(sMac, dMac, addr, num, &txPkt));
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, cvb_eth_acc_check_flash_erase_block(rxPkt));
    }

    return retVal;
}

/**
 * @brief Flash erase all via Ethernet Access.
 * @param[in] None
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_eth_acc_gen_flash_erase_all, cvb_eth_acc_check_flash_erase_all
 */
RtkApiRet cvb_eth_acc_flow_flash_all_erase(void)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen = CVB_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_RET(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_RET(retVal, rtk_etherAccess_dmac_get(dMac));

        /* packet process erase all */
        CHK_FUN_RET(retVal, cvb_eth_acc_gen_flash_erase_all(sMac, dMac, &txPkt));
        CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_RET(retVal, cvb_eth_acc_check_flash_erase_all(rxPkt));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Flash erase sector via Ethernet Access without cpu busy wait.
 * @param[in] addr Flash erase address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the erase position
 * @param[in] num flash erase number (0x1 to 256) \n
 *                 Flash erase number is the number of sector going to be erased
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_ETH_POLLING CPU erase flash fail.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, usecond_delay, cvb_eth_acc_gen_flash_erase_req, cvb_eth_acc_check_flash_erase_req, cvb_drv_eth_acc_flow_flash_erase_check
 * @note Range of num and addr are based on size of flash chip, ((num * CVB_FLASH_4K)) + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_flow_flash_req_sec_erase(uint32 addr, uint32 num)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen = CVB_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;
    uint32                i;

    /* Check error input */
    if(num <= 0u)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));

            /* packet process erase sector */
            for(i = 0; i < num; ++i)
            {
                if(retVal != RT_ERR_OK)
                {
                    break;
                }
                /* Send erase requirement */
                CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_flash_erase_req(sMac, dMac, ((i * CVB_FLASH_4K) + addr), (uint32)CVB_FLASH_REQ_ERASE_SECTOR, &txPkt));
                CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
                CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
                rxPktLen = CVB_MAX_PKT_LEN;
                CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_flash_erase_req(rxPkt));

                retVal = cvb_drv_eth_acc_flow_flash_erase_check(sMac, dMac, RTK_ETH_ACC_FLASH_ERASE_SEC4K_TIME);
            }
        } while(0u == 1u);
    }

    return retVal;
}

/**
 * @brief Flash erase block via Ethernet Access without cpu busy wait.
 * @param[in] addr Flash erase address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the erase position
 * @param[in] num flash erase number (0x1 to 256) \n
 *                 Flash erase number is the number of block going to be erased
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_ETH_POLLING CPU erase flash fail.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, usecond_delay, cvb_eth_acc_gen_flash_erase_req, cvb_eth_acc_check_flash_erase_req, cvb_drv_eth_acc_flow_flash_erase_check
 * @note Range of num and addr are based on size of flash chip, ((num * CVB_FLASH_64K)) + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_flow_flash_req_blk_erase(uint32 addr, uint32 num)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen = CVB_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;
    uint32                i;

    /* Check error input */
    if(num <= 0u)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));

            /* packet process erase block */
            for(i = 0; i < num; ++i)
            {
                if(retVal != RT_ERR_OK)
                {
                    break;
                }
                /* Send erase requirement */
                CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_flash_erase_req(sMac, dMac, ((i * CVB_FLASH_64K) + addr), (uint32)CVB_FLASH_REQ_ERASE_BLOCK, &txPkt));
                CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
                rxPktLen = CVB_MAX_PKT_LEN;
                CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
                CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_flash_erase_req(rxPkt));

                retVal = cvb_drv_eth_acc_flow_flash_erase_check(sMac, dMac, RTK_ETH_ACC_FLASH_ERASE_BLK64K_TIME);
            }
        } while(0u == 1u);
    }

    return retVal;
}

/**
 * @brief Flash erase all via Ethernet Access without cpu busy wait.
 * @param[in] None
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_ETH_POLLING CPU erase flash fail.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, rtk_usecond_delay, cvb_eth_acc_gen_flash_erase_req, cvb_eth_acc_check_flash_erase_req, cvb_drv_eth_acc_flow_flash_erase_check
 */
RtkApiRet cvb_eth_acc_flow_flash_req_all_erase(void)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen = CVB_RTK_ETHERACCESS_HEADER_LEN;
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));

        /* Send erase requirement */
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_flash_erase_req(sMac, dMac, 0u, (uint32)CVB_FLASH_REQ_ERASE_ALL, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_flash_erase_req(rxPkt));

        retVal = cvb_drv_eth_acc_flow_flash_erase_check(sMac, dMac, RTK_ETH_ACC_FLASH_ERASE_ALL_TIME);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Disable all protocols via Ethernet Access.
 * @param[in] None
 * @param[out] None
 * @retval RT_ERR_OK The portocol disable was successful.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_drv_drv_memset, cvb_eth_acc_gen_burst_rw, cvb_eth_acc_check_burst_rw
 */
RtkApiRet cvb_eth_acc_flow_flash_disable_protocol(void)
{
    EtherAccessMac_t mac;
    /* 24 protocol reg + 1 empty */
    uint32                regNum = (CVB_DISABLE_ACL_NUM * 2u) + CVB_DISABLE_IGMP_NUM;
    uint8                 regCmd[(CVB_DISABLE_ACL_NUM * 2u) + CVB_DISABLE_IGMP_NUM + 1u];
    uint8                 regAddr[(CVB_DISABLE_ACL_NUM * 2u) + CVB_DISABLE_IGMP_NUM + 1u][RTK_REG_ADDR_LEN];
    uint32                regValue[(CVB_DISABLE_ACL_NUM * 2u) + CVB_DISABLE_IGMP_NUM + 1u];
    uint32                regRetValue[(CVB_DISABLE_ACL_NUM * 2u) + CVB_DISABLE_IGMP_NUM + 1u];
    uint32                disAclIndex[CVB_DISABLE_ACL_NUM] = {518u, 519u, 521u, 522u, 523u, 524u, 546u, 547u, 548u, 549u, 550u};
    uint32                txPktLen                         = CVB_RTK_ETHERACCESS_HEADER_LEN + ((regNum + 1u) * 8u);
    uint32                rxPktLen                         = CVB_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    uint32                i;
    RtkApiRet             retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));

        /* set all protocol disable */
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regCmd, 0, sizeof(regCmd)));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regAddr, 0, sizeof(regAddr)));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regValue, 0, sizeof(regValue)));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regRetValue, 0, sizeof(regRetValue)));

        for(i = 0u; i < CVB_DISABLE_ACL_NUM; i++)
        {
            regCmd[i * 2u]            = CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[i * 2u][0]        = (CVB_ETH_INDR_BASE >> 16u) & 0xFFu;
            regAddr[i * 2u][1]        = (CVB_ETH_INDR_BASE >> 8u) & 0xFFu;
            regAddr[i * 2u][2]        = (CVB_ETH_INDR_BASE)&0xFFu;
            regValue[i * 2u]          = 0;
            regCmd[(i * 2u) + 1u]     = CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[(i * 2u) + 1u][0] = (CVB_ETH_INCR >> 16u) & 0xFFu;
            regAddr[(i * 2u) + 1u][1] = (CVB_ETH_INCR >> 8u) & 0xFFu;
            regAddr[(i * 2u) + 1u][2] = (CVB_ETH_INCR)&0xFFu;
            regValue[(i * 2u) + 1u]   = CVB_ETH_DISABLE_ACL_BASE + disAclIndex[i];
        }
        regCmd[CVB_DISABLE_ACL_NUM * 2u]            = CVB_ETHERACCESS_PARTIAL_WRITE;
        regAddr[CVB_DISABLE_ACL_NUM * 2u][0]        = (CVB_IGMP_CONTROL_0 >> 16u) & 0xFFu;
        regAddr[CVB_DISABLE_ACL_NUM * 2u][1]        = (CVB_IGMP_CONTROL_0 >> 8u) & 0xFFu;
        regAddr[CVB_DISABLE_ACL_NUM * 2u][2]        = (CVB_IGMP_CONTROL_0)&0xFFu;
        regValue[CVB_DISABLE_ACL_NUM * 2u]          = 0;
        regCmd[(CVB_DISABLE_ACL_NUM * 2u) + 1u]     = CVB_ETHERACCESS_PARTIAL_MASK_WRITE;
        regAddr[(CVB_DISABLE_ACL_NUM * 2u) + 1u][0] = (CVB_IGMP_CONTROL_0 >> 16u) & 0xFFu;
        regAddr[(CVB_DISABLE_ACL_NUM * 2u) + 1u][1] = (CVB_IGMP_CONTROL_0 >> 8u) & 0xFFu;
        regAddr[(CVB_DISABLE_ACL_NUM * 2u) + 1u][2] = (CVB_IGMP_CONTROL_0)&0xFFu;
        regValue[(CVB_DISABLE_ACL_NUM * 2u) + 1u]   = CVB_DISABLE_IGMP_VAL;

        /* packet process */
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        /* parse data */
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_burst_rw(regNum, regRetValue, rxPkt));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Generate a packet for flash erase all.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash erase packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_gen_flash_erase_all(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

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
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)CVB_FLASH_ERASE_ALL);
            command.byte_per_write   = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_BYTE_PER_WRITE);
            command.check_read_back  = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_READ_BACK);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Generate a packet for flash start.
 * @param[in] mac MAC address \n
 *                 The MAC address is used for source/destination MAC address
 * @param[in] addr Start write address (0x0 to 0xFFFFFF) \n
 *                 Start address of flash will be written into the chip
 * @param[in] size Total write size (0x1 to 0x80000) \n
 *                 The size going to write into the chip
 * @param[in] checksum Checksum (0x0 to 0xFFFFFFFF) \n
 *                 Checksum of the total flash context
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash start packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 * @note Range of size and addr are based on size of flash chip, (size + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_gen_flash_start(const EtherAccessMac_t mac, uint32 addr, uint32 size, uint32 checksum, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

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
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity  = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id        = rtl_htonl((uint32)CVB_FLASH_START);
            command.flash_update_addr = rtl_htonl((uint32)addr);
            if(size >= CVB_RTK_ETHERACCESS_BYTE_PER_WRITE)
            {
                command.byte_per_write = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_BYTE_PER_WRITE);
            }
            else
            {
                command.byte_per_write = rtl_htonl((uint32)size);
            }
            command.check_read_back  = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_READ_BACK);
            command.total_write_size = rtl_htonl((uint32)size);
            command.checksum         = rtl_htonl((uint32)checksum);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, mac.sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Generate a packet for flash write.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] addr Start write address (0x0 to 0xFFFFFF) \n
 *                 Start address of flash will be written into the chip
 * @param[in] flash Context of flash (0x0 to 0xFF for len elements) \n
 *                 Pointer of flash will be written into the chip
 * @param[in] len Length of flash (0x1 to 0x80000) \n
 *                 Length of flash will be written into the chip
 * @param[in] seqId Sequence ID (0x1 to 0xFFFFFFFF) \n
 *                 Value of the sequence ID from previous replied packet
 * @param[in] checksum Checksum (0x0 to 0xFFFFFFFF) \n
 *                 Value of the checksum calculated with previous replied packet
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash write packet generation was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy, cvb_drv_eth_acc_gen_write_check
 * @note Range of len and addr are based on size of flash chip, (len + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_gen_flash_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *flash, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check invalid input */
    if(len <= 0u)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Check invalid input */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_eth_acc_gen_write_check(flash, seqId, packet));
            /* Fill in packet */
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity  = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id        = rtl_htonl((uint32)CVB_FLASH_WRITE);
            command.ack_id            = rtl_htonl((uint32)CVB_CPU_ONGOING);
            command.flash_update_addr = rtl_htonl((uint32)addr);
            if(len >= CVB_RTK_ETHERACCESS_BYTE_PER_WRITE)
            {
                command.byte_per_write = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_BYTE_PER_WRITE);
            }
            else
            {
                command.byte_per_write = rtl_htonl((uint32)len);
            }
            command.sequence_id         = rtl_htonl((uint32)seqId);
            command.need_write_position = rtl_htonl((uint32)((seqId - 1u) * len) + addr);
            command.checksum            = rtl_htonl((uint32)checksum);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, mac.sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, flash, len));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash erase all.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] None
 * @retval RT_ERR_OK The flash erase reply packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_flash_erase_all(const uint8 *packet)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id))
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
 * @brief Check if the ethertype, RTL9040_Identity, sequence ID, and ack ID of the packet are correct for flash/patch write.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[in] seqId Sequence ID (0x1 to 0xFFFFFFFF) \n
 *                 Value of the sequence ID from previous transmitted packet + 1
 * @param[out] ackId ACK ID (1 to 2) \n
 *                 Ack ID is used for check the sent command is work. \n
 *                 1: Success, flash write process is finished. \n
 *                 2: Ongoing, flash write process should continue.
 * @param[out] checksum Checksum (0x0 to 0xFFFFFFFF) \n
 *                 Value of the checksum calculated with previous transmitted packet
 * @retval RT_ERR_OK The flash/patch write packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_rep_write(const uint8 *packet, const uint32 seqId, uint32 *ackId, uint32 *checksum)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
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
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            packetSeqId = rtl_htonl(command.sequence_id);
            *ackId      = rtl_htonl(command.ack_id);
            *checksum   = rtl_htonl(command.checksum);

            if((rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0) || (command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY)))
            {
                ret = -RT_ERR_FAILED;
            }
            else if(packetSeqId != seqId)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(((uint32)CVB_CPU_SUCCESS != *ackId) && ((uint32)CVB_CPU_ONGOING != *ackId))
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
 * @brief Generate a packet for sector erase.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] addr Flash erase address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the erase position
 * @param[in] num flash erase number (0x1 to 4095) \n
 *                 Flash erase number is the number of sector going to be erased
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash erase sector packet generation was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 * @note Range of num and addr are based on size of flash chip, ((num * 0x1000) + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_gen_flash_erase_sector(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if((num <= 0u) || (num > CVB_ETHERACCESS_MAX_FLASH_SECTOR) || (addr >= CVB_ETHERACCESS_MAX_FLASH_SIZE) || (((num * 0x1000u) + addr) > CVB_ETHERACCESS_MAX_FLASH_SIZE))
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Fill in packet */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)CVB_FLASH_ERASE_SECTOR);
            command.rw_value         = num;
            command.rw_addr          = addr;
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash sector erase.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] None
 * @retval RT_ERR_OK The flash erase sector packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_flash_erase_sector(const uint8 *packet)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id))
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
 * @brief Generate a packet for block erase.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] addr Flash erase address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the erase position
 * @param[in] num flash erase number (0x1 to 256) \n
 *                 Flash erase number is the number of block going to be erased
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash erase block packet generation was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 * @note Range of num and addr are based on size of flash chip, ((num * 0x10000) + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_gen_flash_erase_block(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if((num <= 0u) || (num > CVB_ETHERACCESS_MAX_FLASH_BLOCK) || (addr >= CVB_ETHERACCESS_MAX_FLASH_SIZE) || (((num * 0x10000u) + addr) > CVB_ETHERACCESS_MAX_FLASH_SIZE))
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Fill in packet */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)CVB_FLASH_ERASE_BLOCK);
            command.rw_value         = num;
            command.rw_addr          = addr;
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash block erase.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_flash_erase_block(const uint8 *packet)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id))
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
 * @brief Generate a packet for erase request.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] addr Flash erase address (0x0 to 0xFFFFFF) \n
 *                 The address is used for the erase position
 * @param[in] eraseCmd flash erase command (31 to 33) \n
 *                 Flash erase command is the size going to be erased
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash erase block packet generation was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_gen_flash_erase_req(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 eraseCmd, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    uint32              offset[3] = {CVB_FLASH_4K, CVB_FLASH_64K, 0u};
    uint32              tmpCmd    = eraseCmd - (uint32)CVB_FLASH_REQ_ERASE_SECTOR;
    RtkApiRet           ret       = RT_ERR_OK;

    do
    {
        /* Check NULL pointer */
        if(NULL == packet)
        {
            ret = -RT_ERR_NULL_POINTER;
        }
        /* Check invalid input */
        else if(tmpCmd > ((uint32)CVB_FLASH_REQ_ERASE_ALL - (uint32)CVB_FLASH_REQ_ERASE_SECTOR))
        {
            ret = -RT_ERR_INPUT;
        }
        /* Check invalid input */
        else if((addr >= CVB_ETHERACCESS_MAX_FLASH_SIZE) || ((offset[tmpCmd] + addr) > CVB_ETHERACCESS_MAX_FLASH_SIZE))
        {
            ret = -RT_ERR_INPUT;
        }
        else
        {
            /* Fill in packet */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)eraseCmd);
            command.rw_addr          = addr;
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash erase request.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_flash_erase_req(const uint8 *packet)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id))
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
 * @brief Generate a packet for flash status check.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash erase block packet generation was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_gen_flash_status_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    do
    {
        /* Check NULL pointer */
        if(NULL == packet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        else
        {
            /* Fill in packet */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)CVB_FLASH_ERASE_CHECK);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash status check.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_flash_status_check(const uint8 *packet)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_FAIL_POOLING == rtl_htonl(command.ack_id))
            {
                ret = -RT_ERR_ETH_POLLING;
            }
            else if((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id))
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
 * @brief Generate a packet for flash read.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] addr Address of flash read (0x0 to 0xFFFFFF) \n
 *                 Address of flash read start position
 * @param[in] len Size of flash read (0x1 to 0x400) \n
 *                 Length of flash will be read
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The flash read packet generation was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 * @note Range of addr is based on size of flash chip, (len + addr) cannot over the size.
 */
RtkApiRet cvb_eth_acc_gen_flash_read(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 len, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if((len <= 0u) || (len > CVB_RTK_ETHERACCESS_BYTE_PER_WRITE))
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Fill in packet */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)CVB_FLASH_READ);
            command.rw_value         = rtl_htonl((uint32)len);
            command.rw_addr          = rtl_htonl((uint32)addr);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash read.
 * @param[in] len Size of flash read (0x1 to 0x400) \n
 *                 Length of flash will be read
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] data Data of flash read (0x0 to 0xFF for len elements) \n
 *                 Data of flash read get from packet
 * @retval RT_ERR_OK The flash read packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_rep_read(const uint8 *packet, uint8 *data, uint32 len)
{
    EtherAccessHeader_t command;
    const uint8        *ptr;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    RtkApiRet           ret                          = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == packet) || (NULL == data))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else if(len > CVB_RTK_ETHERACCESS_BYTE_PER_WRITE)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Parse packet */
            ptr = (const uint8 *)&packet[0];
            ptr = &ptr[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(data, ptr, len));

            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id))
            {
                ret = -RT_ERR_FAILED;
            }
            else
            {
                /* intentionally left blank */
                /* check packet correct */
            }
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Reset system with ethernet access.
 * @param[in] None
 * @param[out] None
 * @retval RT_ERR_OK The system reset was successful.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_eth_acc_gen_reset_system, cvb_eth_acc_check_reset_system
 */
RtkApiRet cvb_eth_acc_flow_reset_system(void)
{
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    uint32                rxPktLen = CVB_MAX_PKT_LEN;
    EtherAccessMac_t      mac;
    int32                 retVal = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;

    do
    {
        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));

        CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_reset_system_check(mac));
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_reset_system(mac.sMac, mac.dMac, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVB_RTK_ETHERACCESS_HEADER_LEN));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_reset_system(rxPkt));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Generate a packet for reset system.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The system reset packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_gen_reset_system(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    uint8               etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

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
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id       = rtl_htonl((uint32)CVB_RESET_SYSTEM);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, &command, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for system reset.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] None
 * @retval RT_ERR_OK The system reset packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_reset_system(uint8 *packet)
{
    EtherAccessHeader_t command;
    uint8              *ptr;
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
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy(&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if(rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0)
            {
                ret = -RT_ERR_FAILED;
            }
            else if(command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id))
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

/**@}*/ /* ETH_FLASH_ASIC */
/**@}*/ /* ETH_FLASH */

/**
 * @defgroup ETH_PATCH Ethernet Access Patch
 * @{
 */

/**
 * @addtogroup ETH_PATCH_ASIC Low Level Driver
 * @brief ETHERNET ACCESS PATCH Low Level Driver
 * @{
 */

/**
 * @brief Patch write via Ethernet Access.
 * @param[in] addr Patch start write address (0x0 to 0xBFFFF) \n
 *                 The address is used for the start write position
 * @param[in] len Data length (1 to 0xC0000) \n
 *                Data length is the length of patch going to write
 * @param[in] data Data buffer (0x0 to 0xFF for len elements) \n
 *                  Data buffer is patch data going to write
 * @param[out] None
 * @retval RT_ERR_OK The patch write was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to rtk_etherAccess_smac_get, rtk_etherAccess_dmac_get, rtk_etherAccess_tx_packet, rtk_etherAccess_rx_packet, cvb_eth_acc_gen_patch_start, cvb_eth_acc_check_rep_write, cvb_drv_eth_acc_patch_write
 */
RtkApiRet cvb_eth_acc_flow_patch_write(uint32 addr, uint32 len, const uint8 *data)
{
    uint8                 sMac[MAC_ADDR_LEN];
    uint8                 dMac[MAC_ADDR_LEN];
    uint32                txPktLen    = CVB_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN;
    uint32                rxPktLen    = CVB_MAX_PKT_LEN;
    uint32                checksum    = 0;
    uint32                curChecksum = 0;
    uint32                i;
    uint32                seqId = 1;
    uint32                ackId = 0;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[CVB_MAX_PKT_LEN];
    RtkApiRet             retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == data)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    /* Check error input */
    else if((len <= 0u) || ((CVB_RTK_WRAP_CHECK_32 - addr) < len))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(sMac));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dMac));
            for(i = 0u; i < len; i++)
            {
                checksum += data[i];
            }

            /* packet process patch start */
            CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_patch_start(sMac, dMac, addr, len, checksum, &txPkt));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            /* parse data */
            CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_rep_write(rxPkt, seqId, &ackId, &curChecksum));

            CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_patch_write(addr, len, data, checksum));
        } while(0u == 1u);
    }

    return retVal;
}

/**
 * @brief Generate a packet for patch start.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] addr Start write address (0x0 to 0xBFFFF) \n
 *                 Start address of patch will be written into the chip
 * @param[in] size Total write size (0x1 to 0xC0000) \n
 *                 The size going to write into the chip
 * @param[in] checksum Checksum (0x0 to 0xFFFFFFFF) \n
 *                 Checksum of the total patch context
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The patch start packet generation was successful.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy, cvb_drv_eth_acc_gen_patch_start_check
 */
RtkApiRet cvb_eth_acc_gen_patch_start(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 size, uint32 checksum, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    do
    {
        /* Check Input */
        CHK_FUN_CONTINUE(ret, cvb_drv_eth_acc_gen_patch_start_check(addr, size, packet));
        /* Fill in packet */
        ptr = (uint8 *)&packet[0];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
        command.RTL9040_Identity  = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
        command.command_id        = rtl_htonl((uint32)CVB_PATCH_START);
        command.flash_update_addr = rtl_htonl((uint32)addr);
        if(size >= CVB_RTK_ETHERACCESS_BYTE_PER_WRITE)
        {
            command.byte_per_write = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_BYTE_PER_WRITE);
        }
        else
        {
            command.byte_per_write = rtl_htonl((uint32)size);
        }
        command.check_read_back  = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_READ_BACK);
        command.total_write_size = rtl_htonl((uint32)size);
        command.checksum         = rtl_htonl((uint32)checksum);
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, sMac, MAC_ADDR_LEN));

        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Generate a packet for patch write.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] addr Start write address (0x0 to 0xBFFFF) \n
 *                 Start address of patch will be written into the chip
 * @param[in] patch Context of patch (0x0 to 0xFF for len elements) \n
 *                 Length of patch will be written into the chip
 * @param[in] len Length of patch (1 to 0xC0000) \n
 *                 Length of patch will be written into the chip
 * @param[in] seqId Sequence ID (0x1 to 0xFFFFFFFF) \n
 *                 Value of the sequence ID from previous replied packet
 * @param[in] checksum Checksum (0x0 to 0xFFFFFFFF) \n
 *                 Value of the checksum calculated with previous replied packet
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The patch write packet generation was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy, cvb_drv_eth_acc_gen_write_check
 */
RtkApiRet cvb_eth_acc_gen_patch_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *patch, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check invalid input */
    if((len <= 0u) || (addr >= CVB_ETHERACCESS_MAX_PATCH_SIZE) || ((addr + len) > CVB_ETHERACCESS_MAX_PATCH_SIZE))
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Check invalid input */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_eth_acc_gen_write_check(patch, seqId, packet));
            /* Fill in packet */
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
            command.RTL9040_Identity  = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            command.command_id        = rtl_htonl((uint32)CVB_PATCH_WRITE);
            command.ack_id            = rtl_htonl((uint32)CVB_CPU_ONGOING);
            command.flash_update_addr = rtl_htonl((uint32)addr);
            if(len >= CVB_RTK_ETHERACCESS_BYTE_PER_WRITE)
            {
                command.byte_per_write = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_BYTE_PER_WRITE);
            }
            else
            {
                command.byte_per_write = rtl_htonl((uint32)len);
            }
            command.sequence_id         = rtl_htonl((uint32)seqId);
            command.need_write_position = rtl_htonl((uint32)((seqId - 1u) * CVB_RTK_ETHERACCESS_BYTE_PER_WRITE) + addr);
            command.checksum            = rtl_htonl((uint32)checksum);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, mac.sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, patch, len));
        } while(0u == 1u);
    }

    return ret;
}

/**@}*/ /* ETH_PATCH_ASIC */
/**@}*/ /* ETH_PATCH */

/**
 * @defgroup ETH_REG Ethernet Access Register
 * @{
 */

/**
 * @addtogroup ETH_REG_ASIC Low Level Driver
 * @brief ETHERNET ACCESS REG Low Level Driver
 * @{
 */

/**
 * @brief Generate a packet for register read/write burst.
 * @param[in] mac MAC address \n
 *                 The MAC address is used for source/destination MAC address
 * @param[in] number Number of register to read/write (1 to 158) \n
 *                 Number of register will be written into the packet
 * @param[in] regCmd Command of register to read/write (1 to 6 for number elements) \n
 *                 Pointer of array which record every register commend will be written into the packet
 * @param[in] regAddr Address of register to read/write (0 to 0xFF for number * 3 elements) \n
 *                 Pointer of array which record every register address will be written into the packet
 * @param[in] regValue Value of register to write (0 to 0xFFFFFFFF for number elements) \n
 *                 Pointer of array which record every register value will be written into the packet
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The register access packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_eth_acc_gen_burst_rw_check, cvb_drv_drv_memset, cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_gen_burst_rw(const EtherAccessMac_t mac, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue, EtherAccessL2Header_t *packet)
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
        CHK_FUN_CONTINUE(ret, cvb_drv_eth_acc_gen_burst_rw_check(number, regCmd, regAddr, regValue));
        ptr = (uint8 *)&packet[0];
        /* Fill in packet */
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2Header_t)));
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&command, 0, sizeof(EtherAccessHeader_t)));
        command.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
        command.command_id       = rtl_htonl((uint32)CVB_REGISTER_RW_BURST);
        command.rw_value         = rtl_htonl(number);
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&command.mac_address_pc_03, mac.sMac, MAC_ADDR_LEN));

        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.dMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, mac.sMac, MAC_ADDR_LEN));
        ptr = &ptr[MAC_ADDR_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
        ptr = &ptr[RTK_ETHERTYPE_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&command, sizeof(EtherAccessHeader_t)));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        for(i = 0u; i < number; i++)
        {
            reg_rw.command = regCmd[i];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&reg_rw.reg_addr, regAddr[i], RTK_REG_ADDR_LEN));
            reg_rw.value = rtl_htonl(regValue[i]);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&reg_rw, sizeof(EtherAccessRegister_t)));
            ptr = &ptr[sizeof(EtherAccessRegister_t)];
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for register read/write burst.
 * @param[in] number Number of register to read/write (1 to 158) \n
 *                 Number of register will be written into the packet
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] regValue Value of register to write (0 to 0xFFFFFFFF for number elements) \n
 *                 Pointer of array which record every register value will be written into the packet
 * @retval RT_ERR_OK The register access reply packet check was successful.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_burst_rw(uint32 number, uint32 *regValue, uint8 *packet)
{
    EtherAccessHeader_t command;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint32              i;
    uint8              *ptr;
    RtkApiRet           ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == regValue) || (NULL == packet))
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
        /* Parse packet */
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&command, ptr, sizeof(EtherAccessHeader_t));
        if(ret == RT_ERR_OK)
        {
            if((rtl_memcmp(&packet[MAC_ADDR_LEN + MAC_ADDR_LEN], etherType, RTK_ETHERTYPE_LEN) != 0) || (command.RTL9040_Identity != rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY)) || ((uint32)CVB_CPU_SUCCESS != rtl_htonl(command.ack_id)))
            {
                ret = -RT_ERR_FAILED;
            }
            else
            {
                ptr = &ptr[sizeof(EtherAccessHeader_t)];

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
            }
        }
    }

    return ret;
}
/**
 * @brief Generate a packet for table dump.
 * @param[in] table Table (0 to 39) \n
 *                 The table type going to get
 * @param[in] startIndex Start index (0 to 4095) \n
 *                 The entry index going to get
 * @param[in] entryNum Entry number (1 to 4096) \n
 *                 The entry number going to get
 * @param[out] data Data (0x0 to 0xFF for entryNum * cvb_table_list[table].datareg_num elements) \n
 *                 The data from the table
 * @retval RT_ERR_OK The table get was successful.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_eth_acc_table_check, cvb_reg_field_set, cvb_drv_eth_acc_table_entry_get
 * @note Range of startIndex and entryNum are based on size of the table and packet, (startIndex + entryNum) cannot over the table size, (entryNum * (cvb_table_list[table].datareg_num + 1)) cannot over 158.
 */
RtkApiRet cvb_eth_acc_flow_table_entry_get(cvb_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data)
{
    uint32    wTableReg = 0;
    uint32    wTableRegTmp;
    uint32    entryRegNum = cvb_table_list[table].datareg_num + 1u;
    uint32    regNum      = entryNum * entryRegNum;
    uint8     regCmd[RTK_ETHERACCESS_MAX_BURST];
    uint8     regAddr[RTK_ETHERACCESS_MAX_BURST][RTK_REG_ADDR_LEN];
    uint32    regValue[RTK_ETHERACCESS_MAX_BURST];
    uint32    i, j;
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Check input */
        CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_table_check(table, startIndex, entryNum, regNum, data));

        /* make etherAccess payload */
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regCmd, 0, sizeof(regCmd)));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regAddr, 0, sizeof(regAddr)));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regValue, 0, sizeof(regValue)));
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_EXE_LSP, CVB_ETH_INCR_EXE_LEN, 1, &wTableReg));
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_RW_LSP, CVB_ETH_INCR_RW_LEN, 1, &wTableReg));
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_TYPE_LSP, CVB_ETH_INCR_TYPE_LEN, cvb_table_list[table].type, &wTableReg));
        for(i = 0; i < entryNum; i++)
        {
            regCmd[i * entryRegNum]     = CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[i * entryRegNum][0] = (CVB_ETH_INCR >> 16u) & 0xFFu;
            regAddr[i * entryRegNum][1] = (CVB_ETH_INCR >> 8u) & 0xFFu;
            regAddr[i * entryRegNum][2] = (CVB_ETH_INCR)&0xFFu;
            wTableRegTmp                = wTableReg;
            CHK_FUN_BREAK(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_ADDR_LSP, CVB_ETH_INCR_ADDR_LEN, startIndex + i, &wTableRegTmp));
            regValue[i * entryRegNum] = wTableRegTmp;
            for(j = 1; j <= cvb_table_list[table].datareg_num; j++)
            {
                regCmd[(i * entryRegNum) + j]     = CVB_ETHERACCESS_REGISTER_RW_MAC_READ;
                regAddr[(i * entryRegNum) + j][0] = (CVB_ETH_INDR_BASE >> 16u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][1] = (CVB_ETH_INDR_BASE >> 8u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][2] = (uint8)(CVB_ETH_INDR_BASE + ((j - 1u) * 4u)) & 0xFFu;
            }
        }

        /* packet process */
        CHK_FUN_RET(retVal, cvb_drv_eth_acc_table_entry_get(entryRegNum, regNum, regCmd, &regAddr[0], regValue, data));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Generate a packet for table set.
 * @param[in] table Table (0 to 39) \n
 *                 The table type going to set
 * @param[in] startIndex Start index (0 to 4095) \n
 *                 The entry index going to set
 * @param[in] entryNum Entry number (1 to 4096) \n
 *                 The entry number going to set
 * @param[in] data Data (0x0 to 0xFF for entryNum * cvb_table_list[table].datareg_num elements) \n
 *                 The data set to the table
 * @param[out] None
 * @retval RT_ERR_OK The table set was successful.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_eth_acc_table_check, cvb_reg_field_set, cvb_drv_eth_acc_table_entry_set
 * @note Range of startIndex and entryNum are based on size of the table and packet, (startIndex + entryNum) cannot over the table size, (entryNum * (cvb_table_list[table].datareg_num + 1)) cannot over 158.
 */
RtkApiRet cvb_eth_acc_flow_table_entry_set(const cvb_table_list_t table, const uint32 startIndex, const uint32 entryNum, const uint32 *data)
{
    uint32    wTableReg = 0;
    uint32    wTableRegTmp;
    uint32    entryRegNum = cvb_table_list[table].datareg_num + 1u;
    uint32    regNum      = entryNum * entryRegNum;
    uint8     regCmd[RTK_ETHERACCESS_MAX_BURST];
    uint8     regAddr[RTK_ETHERACCESS_MAX_BURST][RTK_REG_ADDR_LEN];
    uint32    regValue[RTK_ETHERACCESS_MAX_BURST];
    uint32    i, j;
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Check input */
        CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_table_check(table, startIndex, entryNum, regNum, data));

        /* make etherAccess payload */
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regCmd, 0, sizeof(regCmd)));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regAddr, 0, sizeof(regAddr)));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(regValue, 0, sizeof(regValue)));
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_EXE_LSP, CVB_ETH_INCR_EXE_LEN, 1, &wTableReg));
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_RW_LSP, CVB_ETH_INCR_RW_LEN, 0, &wTableReg));
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_TYPE_LSP, CVB_ETH_INCR_TYPE_LEN, cvb_table_list[table].type, &wTableReg));
        for(i = 0; i < entryNum; i++)
        {
            for(j = 0; j < cvb_table_list[table].datareg_num; j++)
            {
                regCmd[(i * entryRegNum) + j]     = CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE;
                regAddr[(i * entryRegNum) + j][0] = (CVB_ETH_INDR_BASE >> 16u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][1] = (CVB_ETH_INDR_BASE >> 8u) & 0xFFu;
                regAddr[(i * entryRegNum) + j][2] = (uint8)(CVB_ETH_INDR_BASE + (j * 4u)) & 0xFFu;
                regValue[(i * entryRegNum) + j]   = data[(i * cvb_table_list[table].datareg_num) + j];
            }
            regCmd[(i * entryRegNum) + j]     = CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE;
            regAddr[(i * entryRegNum) + j][0] = (CVB_ETH_INCR >> 16u) & 0xFFu;
            regAddr[(i * entryRegNum) + j][1] = (CVB_ETH_INCR >> 8u) & 0xFFu;
            regAddr[(i * entryRegNum) + j][2] = (CVB_ETH_INCR)&0xFFu;
            wTableRegTmp                      = wTableReg;
            CHK_FUN_BREAK(retVal, cvb_reg_field_set(CVB_ETH_INCR, CVB_ETH_INCR_ADDR_LSP, CVB_ETH_INCR_ADDR_LEN, startIndex + i, &wTableRegTmp));
            regValue[(i * entryRegNum) + j] = wTableRegTmp;
        }

        /* packet process */
        CHK_FUN_RET(retVal, cvb_drv_eth_acc_table_entry_set(regNum, regCmd, &regAddr[0], regValue));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* ETH_REG_ASIC */
/**@}*/ /* ETH_REG */

/**
 * @defgroup ETH_L3_OTP Ethernet Access L3 OTP
 * @{
 */

/**
 * @addtogroup ETH_L3_OTP_ASIC Low Level Driver
 * @brief ETHERNET ACCESS L3 OTP Low Level Driver
 * @{
 */

/**
 * @brief Generate a packet for otp ask.
 * @param[in] macAddr MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] sip Source IP (0x0 to 0xFFFFFFFF) \n
 *                 The source IP is used for L3 source IP address
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The otp ask packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy, cvb_drv_eth_acc_checksum
 */
RtkApiRet cvb_eth_acc_gen_l3_otp_ask(const uint8 macAddr[MAC_ADDR_LEN], uint32 sip, EtherAccessL2IpHdr_t *packet)
{
    EtherAccessIpHdr_t header;
    const uint8        broadcastMac[MAC_ADDR_LEN]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const uint8        etherType[RTK_ETHERTYPE_LEN] = {0x08, 0x00};
    uint32             dip                          = 0xFFFFFFFFu;
    uint8             *ptr;
    uint16             tmpChecksum;
    RtkApiRet          ret = RT_ERR_OK;

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
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2IpHdr_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&header, 0, sizeof(EtherAccessIpHdr_t)));
            header.ip_hl      = CVB_ETHERACCESS_IPHDR_HL;
            header.ip_v       = CVB_ETHERACCESS_IPHDR_VER;
            header.ip_tos     = CVB_ETHERACCESS_IPHDR_TOS;
            header.ip_len     = rtl_htons((uint16)(sizeof(EtherAccessIpHdr_t)));
            header.ip_ttl     = 255;
            header.ip_p       = CVB_ETHERACCESS_IPHDR_PROTO;
            header.ip_src     = rtl_htonl(sip);
            header.ip_dst     = rtl_htonl(dip);
            header.command_id = rtl_htonl((uint32)CVB_ETH_OTP_ASK);
            tmpChecksum       = cvb_drv_eth_acc_checksum(&header);
            header.ip_sum     = rtl_htons(tmpChecksum);

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, broadcastMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, macAddr, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&header, sizeof(EtherAccessIpHdr_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Generate a packet for otp check.
 * @param[in] macAddr MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] sip Source IP (0x0 to 0xFFFFFFFF) \n
 *                 The source IP is used for L3 source IP address
 * @param[in] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @param[in] type OTP type (0 to 5) \n
 *                 OTP type is listed in Ethernet Access programming guide
 * @param[in] typeId OTP type ID (0 to 3) \n
 *                 OTP type id is sub-type of OTP type and is listed in Ethernet Access programming guide
 * @param[in] len Length (0 to 0x800) \n
 *                 Len is length of the data of the type
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The otp check packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy, cvb_drv_eth_acc_checksum
 * @note type, typeId and len size is applied in Ethernet Access programming guide.
 */
RtkApiRet cvb_eth_acc_gen_l3_otp_check(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, EtherAccessL2IpHdr_t *packet)
{
    EtherAccessIpHdr_t header;
    const uint8        broadcastMac[MAC_ADDR_LEN]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const uint8        etherType[RTK_ETHERTYPE_LEN] = {0x08, 0x00};
    uint32             dip                          = 0xFFFFFFFFu;
    uint8             *ptr;
    uint16             tmpChecksum;
    RtkApiRet          ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if(len > CVB_RTK_ETHERACCESS_MAX_OTP)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Fill in packet */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2IpHdr_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&header, 0, sizeof(EtherAccessIpHdr_t)));
            header.ip_hl      = CVB_ETHERACCESS_IPHDR_HL;
            header.ip_v       = CVB_ETHERACCESS_IPHDR_VER;
            header.ip_tos     = CVB_ETHERACCESS_IPHDR_TOS;
            header.ip_len     = rtl_htons((uint16)(sizeof(EtherAccessIpHdr_t)));
            header.ip_ttl     = CVB_ETHERACCESS_IPHDR_TTL;
            header.ip_p       = CVB_ETHERACCESS_IPHDR_PROTO;
            header.ip_src     = rtl_htonl(sip);
            header.ip_dst     = rtl_htonl(dip);
            header.command_id = rtl_htonl((uint32)CVB_ETH_OTP_CHECK);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header.uuid, uuid, RTK_ETHERACCESS_UUID_LEN));
            header.type     = type;
            header.type_id  = typeId;
            header.data_len = rtl_htonl(len);
            tmpChecksum     = cvb_drv_eth_acc_checksum(&header);
            header.ip_sum   = rtl_htons(tmpChecksum);

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, broadcastMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, macAddr, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&header, sizeof(EtherAccessIpHdr_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Generate a packet for otp write.
 * @param[in] macAddr MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] sip Source IP (0x0 to 0xFFFFFFFF) \n
 *                 The source IP is used for L3 source IP address
 * @param[in] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @param[in] type OTP type (0 to 5) \n
 *                 OTP type is listed in Ethernet Access programming guide
 * @param[in] typeId OTP type ID (0 to 3) \n
 *                 OTP type id is sub-type of OTP type and is listed in Ethernet Access programming guide
 * @param[in] len Length (0 to 0x800) \n
 *                 Len is length of the data of the type
 * @param[in] checksum Checksum (0x0 to 0xFFFFFFFF) \n
 *                 Checksum of the data. Notice that this item is sum of each byte of data
 * @param[in] data Data of OTP (0x0 to 0xFF for len elements) \n
 *                 Pointer of the data of the type
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The otp write packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval -RT_ERR_INPUT The input value was wrong.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy, cvb_drv_eth_acc_checksum
 * @note type, typeId and len size is applied in Ethernet Access programming guide.
 */
RtkApiRet cvb_eth_acc_gen_l3_otp_write(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const EtherAccessOtpData_t otpInfo, const uint8 *data, EtherAccessL2IpHdr_t *packet)
{
    EtherAccessIpHdr_t header;
    const uint8        broadcastMac[MAC_ADDR_LEN]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const uint8        etherType[RTK_ETHERTYPE_LEN] = {0x08, 0x00};
    uint32             dip                          = 0xFFFFFFFFu;
    uint8             *ptr;
    uint16             tmpChecksum;
    RtkApiRet          ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == packet)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    /* Check invalid input */
    else if(otpInfo.len > CVB_RTK_ETHERACCESS_MAX_OTP)
    {
        ret = -RT_ERR_INPUT;
    }
    else
    {
        do
        {
            /* Fill in packet */
            ptr = (uint8 *)&packet[0];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(EtherAccessL2IpHdr_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&header, 0, sizeof(EtherAccessIpHdr_t)));
            header.ip_hl      = CVB_ETHERACCESS_IPHDR_HL;
            header.ip_v       = CVB_ETHERACCESS_IPHDR_VER;
            header.ip_tos     = CVB_ETHERACCESS_IPHDR_TOS;
            header.ip_len     = rtl_htons((uint16)(sizeof(EtherAccessIpHdr_t) + otpInfo.len));
            header.ip_ttl     = CVB_ETHERACCESS_IPHDR_TTL;
            header.ip_p       = CVB_ETHERACCESS_IPHDR_PROTO;
            header.ip_src     = rtl_htonl(sip);
            header.ip_dst     = rtl_htonl(dip);
            header.command_id = rtl_htonl((uint32)CVB_ETH_OTP_WRITE);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header.uuid, uuid, RTK_ETHERACCESS_UUID_LEN));
            header.type     = otpInfo.type;
            header.type_id  = otpInfo.typeId;
            header.data_len = rtl_htonl(otpInfo.len);
            header.checksum = rtl_htonl(otpInfo.checksum);
            tmpChecksum     = cvb_drv_eth_acc_checksum(&header);
            header.ip_sum   = rtl_htons(tmpChecksum);

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, broadcastMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, macAddr, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&header, sizeof(EtherAccessIpHdr_t)));
            ptr = &ptr[sizeof(EtherAccessIpHdr_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, data, otpInfo.len));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Get ttl in otp packet.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] ttl TTL (0 to 255) \n
 *                 TTL is used for identifying the switch position in the topology
 * @retval RT_ERR_OK The TTL get was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_get_l3_otp_ttl(const uint8 *packet, uint8 *ttl)
{
    EtherAccessIpHdr_t header;
    const uint8       *ptr;
    RtkApiRet          ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == packet) || (NULL == ttl))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        ptr  = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret  = cvb_drv_drv_memcpy((uint8 *)&header, ptr, sizeof(EtherAccessIpHdr_t));
        *ttl = header.ip_ttl;
    }

    return ret;
}

/**
 * @brief Check if the command ID and ack ID of the packet are correct, and get UUID for L3 OTP join.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] ackId ACK ID (1 to 3) \n
 *                 Ack ID is used for check the sent command is work
 * @param[out] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @retval RT_ERR_OK The otp join packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_l3_otp_join(const uint8 *packet, uint32 *ackId, uint8 *uuid)
{
    EtherAccessIpHdr_t header;
    const uint8       *ptr;
    RtkApiRet          ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == packet) || (NULL == ackId) || (NULL == uuid))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Parse packet */
        ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        ret = cvb_drv_drv_memcpy((uint8 *)&header, ptr, sizeof(EtherAccessIpHdr_t));
        if(ret == RT_ERR_OK)
        {
            *ackId = rtl_htonl(header.ack_id);
            if((uint32)CVB_ETH_OTP_JOIN != rtl_htonl(header.command_id))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != *ackId)
            {
                ret = -RT_ERR_FAILED;
            }
            else
            {
                ret = cvb_drv_drv_memcpy(uuid, header.uuid, RTK_ETHERACCESS_UUID_LEN);
            }
        }
    }

    return ret;
}

/**
 * @brief Check if the command ID, UUID, type, type ID, and ack ID of the packet are correct for L3 OTP reply.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[in] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @param[in] type OTP type (0 to 5) \n
 *                 OTP type is listed in Ethernet Access programming guide
 * @param[in] typeId OTP type ID (0 to 3) \n
 *                 OTP type id is sub-type of OTP type and is listed in Ethernet Access programming guide
 * @param[in] len Length (0 to 0x800) \n
 *                 Len is length of the data of the type
 * @param[out] ackId ACK ID (1 to 3) \n
 *                 Ack ID is used for check the sent command is work \n
 *                 1: Success, the switch replied otp_write command without error. \n
 *                 3: Fail, the switch process otp packet fail. \n
 *                 5: Ready, the switch replied otp_check command without error
 * @param[out] data Data (0x0 to 0xFF for len elements) \n
 *                 Pointer of the data replied by the switch
 * @retval RT_ERR_OK The otp check/write was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval Others Please refer to cvb_drv_drv_memcpy, cvb_drv_eth_acc_check_l3_otp_rep_null, cvb_drv_eth_acc_data_checksum
 */
RtkApiRet cvb_eth_acc_check_l3_otp_rep(EtherAccessL2IpHdr_t *packet, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, uint32 *ackId, uint8 *data)
{
    EtherAccessIpHdr_t header;
    uint8             *ptr;
    uint32             checksum = 0;
    RtkApiRet          ret      = RT_ERR_OK;

    do
    {
        /* Check NULL pointer */
        CHK_FUN_CONTINUE(ret, cvb_drv_eth_acc_check_l3_otp_rep_null(packet, uuid, ackId, data));
        ptr = (uint8 *)&packet[0];
        /* Parse packet */
        ptr = &ptr[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header, ptr, sizeof(EtherAccessIpHdr_t)));
        *ackId = rtl_htonl(header.ack_id);

        if(((uint32)CVB_ETH_OTP_REP != rtl_htonl(header.command_id)) || (rtl_memcmp(uuid, header.uuid, RTK_ETHERACCESS_UUID_LEN) != 0))
        {
            ret = -RT_ERR_FAILED;
        }
        else if((header.type != type) || (header.type_id != typeId) || (rtl_htonl(header.data_len) != len))
        {
            ret = -RT_ERR_FAILED;
        }
        else if(((uint32)CVB_CPU_SUCCESS != *ackId) && ((uint32)CVB_CPU_READY != *ackId))
        {
            ret = -RT_ERR_FAILED;
        }
        else if((uint32)CVB_CPU_READY == *ackId)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(data, &ptr[sizeof(EtherAccessIpHdr_t)], len));
            checksum = cvb_drv_eth_acc_data_checksum(data, len, checksum);
            if(rtl_htonl(header.checksum) != checksum)
            {
                ret = -RT_ERR_FAILED;
            }
        }
        else
        {
            /* intentionally left blank */
            /* check packet correct */
        }
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* ETH_L3_OTP_ASIC */
/**@}*/ /* ETH_L3_OTP */

/**
 * @defgroup ETH_L2_OTP Ethernet Access L2 OTP
 * @{
 */

/**
 * @addtogroup ETH_L2_OTP_ASIC Low Level Driver
 * @brief ETHERNET ACCESS L2 OTP Low Level Driver
 * @{
 */

/**
 * @brief Generate a packet for l2 otp ask.
 * @param[in] sMac MAC address of PC \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address \n
 *                 The MAC address is used for destination MAC address
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The l2 otp ask packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_gen_l2_otp_ask(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], RtkEtherAccessL2OtpHdr_t *packet)
{
    EtherAccessHeader_t header;
    const uint8         etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8              *ptr                          = (uint8 *)&packet[0];
    RtkApiRet           ret                          = RT_ERR_OK;

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
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(RtkEtherAccessL2OtpHdr_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&header, 0, sizeof(EtherAccessHeader_t)));
            header.command_id       = rtl_htonl((uint32)CVB_OTP_L2_ASK);
            header.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header.mac_address_pc_03, sMac, MAC_ADDR_LEN));

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&header, sizeof(EtherAccessHeader_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Generate a packet for l2 otp check.
 * @param[in] sMac MAC address of PC \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address \n
 *                 The MAC address is used for destination MAC address
 * @param[in] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @param[in] type OTP type (0 to 5) \n
 *                 OTP type is listed in Ethernet Access programming guide
 * @param[in] typeId OTP type ID (0 to 3) \n
 *                 OTP type id is sub-type of OTP type and is listed in Ethernet Access programming guide
 * @param[in] len Length (0 to 0x800) \n
 *                 Len is length of the data of the type
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The otp check packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 * @note type, typeId and len size is applied in Ethernet Access programming guide.
 */
RtkApiRet cvb_eth_acc_gen_l2_otp_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, RtkEtherAccessL2OtpHdr_t *packet)
{
    EtherAccessHeader_t   header;
    RtkEtherAccessL2Otp_t otpHeader;
    const uint8           etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8                *ptr                          = (uint8 *)&packet[0];
    RtkApiRet             ret                          = RT_ERR_OK;

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
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(RtkEtherAccessL2OtpHdr_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&header, 0, sizeof(EtherAccessHeader_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header.mac_address_pc_03, sMac, MAC_ADDR_LEN));
            header.command_id       = rtl_htonl((uint32)CVB_OTP_L2_CHECK);
            header.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&otpHeader.uuid, uuid, RTK_ETHERACCESS_UUID_LEN));
            otpHeader.type     = type;
            otpHeader.typeId   = typeId;
            otpHeader.dataLen  = rtl_htonl(len);
            otpHeader.checksum = 0u;

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&header, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&otpHeader, sizeof(RtkEtherAccessL2Otp_t)));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Generate a packet for l2 otp write.
 * @param[in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for source MAC address
 * @param[in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) \n
 *                 The MAC address is used for destination MAC address
 * @param[in] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @param[in] otpInfo OTP information (N/A) \n
 *                 OTP access information
 * @param[in] data Data of OTP (0x0 to 0xFF for len elements) \n
 *                 Pointer of the data of the type
 * @param[out] packet Packet (N/A) \n
 *                 Pointer of generated packet
 * @retval RT_ERR_OK The otp write packet generation was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memset, cvb_drv_drv_memcpy
 * @note type, typeId and len size is applied in Ethernet Access programming guide.
 */
RtkApiRet cvb_eth_acc_gen_l2_otp_write(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const EtherAccessOtpData_t otpInfo, const uint8 *data, RtkEtherAccessL2OtpHdr_t *packet)
{
    EtherAccessHeader_t   header;
    RtkEtherAccessL2Otp_t otpHeader;
    const uint8           etherType[RTK_ETHERTYPE_LEN] = {0x88, 0x99};
    uint8                *ptr                          = (uint8 *)&packet[0];
    RtkApiRet             ret                          = RT_ERR_OK;

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
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(packet, 0, sizeof(RtkEtherAccessL2OtpHdr_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(&header, 0, sizeof(EtherAccessHeader_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header.mac_address_pc_03, sMac, MAC_ADDR_LEN));
            header.command_id       = rtl_htonl((uint32)CVB_OTP_L2_WRITE);
            header.RTL9040_Identity = rtl_htonl((uint32)CVB_RTK_ETHERACCESS_IDENTITY);
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&otpHeader.uuid, uuid, RTK_ETHERACCESS_UUID_LEN));
            otpHeader.type     = otpInfo.type;
            otpHeader.typeId   = otpInfo.typeId;
            otpHeader.dataLen  = rtl_htonl(otpInfo.len);
            otpHeader.checksum = rtl_htonl(otpInfo.checksum);

            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, dMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, sMac, MAC_ADDR_LEN));
            ptr = &ptr[MAC_ADDR_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, etherType, RTK_ETHERTYPE_LEN));
            ptr = &ptr[RTK_ETHERTYPE_LEN];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&header, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, (uint8 *)&otpHeader, sizeof(RtkEtherAccessL2Otp_t)));
            ptr = &ptr[sizeof(RtkEtherAccessL2Otp_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(ptr, data, otpInfo.len));
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check if the command ID and ack ID of the packet are correct, and get UUID for L2 OTP join.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[out] ackId ACK ID (1 to 3) \n
 *                 Ack ID is used for check the sent command is work
 * @param[out] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @retval RT_ERR_OK The otp join packet check was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval Others Please refer to cvb_drv_drv_memcpy
 */
RtkApiRet cvb_eth_acc_check_l2_otp_join(const uint8 *packet, uint32 *ackId, uint8 *uuid)
{
    EtherAccessHeader_t   header;
    RtkEtherAccessL2Otp_t otpHeader;
    const uint8          *ptr = &packet[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
    RtkApiRet             ret = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == packet) || (NULL == ackId) || (NULL == uuid))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            /* Parse packet */
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header, ptr, sizeof(EtherAccessHeader_t)));
            ptr = &ptr[sizeof(EtherAccessHeader_t)];
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&otpHeader, ptr, sizeof(RtkEtherAccessL2Otp_t)));
            *ackId = rtl_htonl(header.ack_id);
            if((uint32)CVB_OTP_L2_JOIN != rtl_htonl(header.command_id))
            {
                ret = -RT_ERR_FAILED;
            }
            else if((uint32)CVB_CPU_SUCCESS != *ackId)
            {
                ret = -RT_ERR_FAILED;
            }
            else
            {
                CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(uuid, otpHeader.uuid, RTK_ETHERACCESS_UUID_LEN));
            }
        } while(0u == 1u);
    }

    return ret;
}

/**
 * @brief Check if the command ID, UUID, type, type ID, and ack ID of the packet are correct for L2 OTP reply.
 * @param[in] packet Packet (N/A) \n
 *                 Pointer of received packet
 * @param[in] uuid UUID (0x0 to 0xFF for 10 elements) \n
 *                 Universally Unique Identifier of the switch
 * @param[in] type OTP type (0 to 5) \n
 *                 OTP type is listed in Ethernet Access programming guide
 * @param[in] typeId OTP type ID (0 to 3) \n
 *                 OTP type id is sub-type of OTP type and is listed in Ethernet Access programming guide
 * @param[in] len Length (0 to 0x800) \n
 *                 Len is length of the data of the type
 * @param[out] ackId ACK ID (1 to 3) \n
 *                 Ack ID is used for check the sent command is work \n
 *                 1: Success, the switch replied otp_write command without error. \n
 *                 3: Fail, the switch process otp packet fail. \n
 *                 5: Ready, the switch replied otp_check command without error
 * @param[out] data Data (0x0 to 0xFF for len elements) \n
 *                 Pointer of the data replied by the switch
 * @retval RT_ERR_OK The otp check/write was successful.
 * @retval -RT_ERR_FAILED The packet checked data failed.
 * @retval Others Please refer to cvb_drv_drv_memcpy, cvb_drv_eth_acc_check_l3_otp_rep_null
 */
RtkApiRet cvb_eth_acc_check_l2_otp_rep(RtkEtherAccessL2OtpHdr_t *packet, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, uint32 *ackId, uint8 *data)
{
    EtherAccessHeader_t   header;
    RtkEtherAccessL2Otp_t otpHeader;
    uint8                *ptr = (uint8 *)&packet[0];
    uint32                i;
    uint32                checksum = 0;
    RtkApiRet             ret      = RT_ERR_OK;

    do
    {
        /* Check NULL pointer */
        CHK_FUN_CONTINUE(ret, cvb_drv_eth_acc_check_l3_otp_rep_null(packet, uuid, ackId, data));
        /* Parse packet */
        ptr = &ptr[MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&header, ptr, sizeof(EtherAccessHeader_t)));
        ptr = &ptr[sizeof(EtherAccessHeader_t)];
        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy((uint8 *)&otpHeader, ptr, sizeof(RtkEtherAccessL2Otp_t)));
        *ackId = rtl_htonl(header.ack_id);
        if((uint32)CVB_OTP_L2_REP != rtl_htonl(header.command_id))
        {
            ret = -RT_ERR_FAILED;
        }
        else if((rtl_memcmp(uuid, otpHeader.uuid, RTK_ETHERACCESS_UUID_LEN) != 0) || (otpHeader.type != type) || (otpHeader.typeId != typeId) || (rtl_htonl(otpHeader.dataLen) != len))
        {
            ret = -RT_ERR_FAILED;
        }
        else if(((uint32)CVB_CPU_SUCCESS != *ackId) && ((uint32)CVB_CPU_READY != *ackId))
        {
            ret = -RT_ERR_FAILED;
        }
        else
        {
            if((uint32)CVB_CPU_READY == *ackId)
            {
                CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(data, &ptr[sizeof(RtkEtherAccessL2Otp_t)], len));
                for(i = 0u; i < len; i++)
                {
                    checksum += data[i];
                }
                if(rtl_htonl(otpHeader.checksum) != checksum)
                {
                    ret = -RT_ERR_FAILED;
                }
            }
        }
    } while(0u == 1u);

    return ret;
}

/**@}*/  /* ETH_L2_OTP_ASIC */
/**@}*/  /* ETH_L2_OTP */
/**@}*/  // Ethernet Access
