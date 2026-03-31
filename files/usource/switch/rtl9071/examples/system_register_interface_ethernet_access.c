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

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include "rtkas_common.h"
#include "rtkas_error.h"
#include <string.h>

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ethernet_access.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ethernet_access.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ethernet_access.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ethernet_access.h"
#endif
#include "rtkas_api_ethernet_access.h"
#include "rtkas_system_porting.h"

/**
 * @addtogroup EXAMPLE
 * @{
 */

/**
 * @defgroup SYS_PORTING_EX System Porting Example
 * @{
 */

/**
 * @defgroup REGIF_ETHERNET_ACCESS_INIT Ethernet Access Register Interface Init Example
 * @{
 */

/*******************************************************************************
 *   Defines
 *******************************************************************************/
#define RTK_PC_FILTER_PROTOCOL ETH_P_ALL

/*******************************************************************************
 *  Global Variables
 ******************************************************************************/
static int8  nicName[20];
static uint8 g_linuxDstMac[MAC_ADDR_LEN];
static uint8 g_linuxSrcMac[MAC_ADDR_LEN];
static int32 g_ethFd = 0;

/*******************************************************************************
 *  Functions
 ******************************************************************************/
void      linux_eth_default_set(void);
RtkApiRet linux_eth_tx_packet(const uint8 *txPkt, const uint32 len);
RtkApiRet linux_eth_rx_packet(uint8 *rxPkt, uint32 *len);
RtkApiRet linux_eth_smac_get(uint8 *macAddr);
RtkApiRet linux_eth_dmac_get(uint8 *macAddr);
RtkApiRet eth_socket_open(void);
void      eth_socket_close(void);
#if defined(RTL_907XD_VA)
RtkApiRet dva_platform_eth_read(uint32 regAddr, uint32 *value);
RtkApiRet dva_platform_eth_write(uint32 regAddr, uint32 value);
#endif
RtkApiRet sys_reg_interface_eth_open(void);
void      sys_reg_interface_eth_close(void);
RtkApiRet sys_reg_eth_interface_init(void);

void linux_eth_default_set(void)
{
    int8  nic[20]                   = {"enp2s0"};
    uint8 linuxDstMac[MAC_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8 linuxSrcMac[MAC_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    (void)drv_memcpy(nicName, nic, 20);
    (void)drv_memcpy(g_linuxDstMac, linuxDstMac, MAC_ADDR_LEN);
    (void)drv_memcpy(g_linuxSrcMac, linuxSrcMac, MAC_ADDR_LEN);
}

RtkApiRet linux_eth_tx_packet(const uint8 *txPkt, const uint32 len)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == txPkt)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    /* Check error input */
    if(len <= 0u)
    {
        retVal = -RT_ERR_INPUT;
    }

    if(RT_ERR_OK == retVal)
    {
        /* Send packet here */
        uint32 bytes = 0;
        bytes        = (uint32)write(g_ethFd, txPkt, len);
        if(bytes == 0u)
        {
            retVal = -RT_ERR_FAILED;
        }
    }

    return retVal;
}

RtkApiRet linux_eth_rx_packet(uint8 *rxPkt, uint32 *len)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check NULL pointer */
    if((NULL == rxPkt) || (NULL == len))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == retVal)
    {
        (void)drv_memset(rxPkt, 0, *len);
        *len = 0;

        /* Receive packet here */
        int32  readRet = 0;
        uint32 readLen = 1492u;

        while((rxPkt[12] != 0x88u) && (rxPkt[13] != 0x99u))
        {
            readRet = (int32)read(g_ethFd, rxPkt, readLen);
            *len    = (uint32)readRet;
        }
    }

    return retVal;
}

RtkApiRet linux_eth_smac_get(uint8 *macAddr)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == macAddr)
    {
        ret = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == ret)
    {
        (void)drv_memset(macAddr, 0, MAC_ADDR_LEN);
        /* Get MAC address here */
        (void)drv_memcpy(macAddr, g_linuxSrcMac, MAC_ADDR_LEN);
    }

    return ret;
}

RtkApiRet linux_eth_dmac_get(uint8 *macAddr)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Check NULL pointer */
    if(NULL == macAddr)
    {
        ret = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == ret)
    {
        (void)drv_memset(macAddr, 0, MAC_ADDR_LEN);
        /* Get MAC address here */
        (void)drv_memcpy(macAddr, g_linuxDstMac, MAC_ADDR_LEN);
    }

    return ret;
}

RtkApiRet eth_socket_open(void)
{
    struct ifreq       req;
    struct sockaddr_ll addr;
    RtkApiRet          retVal = RT_ERR_OK;

    do
    {
        /* Open socket to communicate the switch. */
        g_ethFd = socket(PF_PACKET, (int)SOCK_RAW, (int)rtl_htons(RTK_PC_FILTER_PROTOCOL));
        if(g_ethFd < 0)
        {
            (void)rtlglue_printf("init: socket\n");
            retVal = RT_ERR_FAILED;
            continue;
        }
        if(fcntl(g_ethFd, F_SETFL, O_NONBLOCK) < 0)
        {
            (void)rtlglue_printf("init: fcntl\n");
            (void)close(g_ethFd);
            retVal = RT_ERR_FAILED;
            continue;
        }

        /* Bind to interface. */
        (void)drv_memset(&req, 0, sizeof(req));
        (void)drv_memcpy((char *)req.ifr_name, (char *)nicName, 16u);
        if(ioctl(g_ethFd, SIOCGIFINDEX, &req) < 0)
        {
            (void)rtlglue_printf("init: ioctl\n");
            (void)close(g_ethFd);
            retVal = RT_ERR_FAILED;
            continue;
        }
        (void)drv_memset(&addr, 0, sizeof(addr));
        addr.sll_family   = PF_PACKET;
        addr.sll_protocol = rtl_htons(RTK_PC_FILTER_PROTOCOL);
        addr.sll_ifindex  = req.ifr_ifindex;
        if(bind(g_ethFd, (const struct sockaddr *)&addr, (socklen_t)sizeof(addr)) < 0)
        {
            (void)rtlglue_printf("init: bind fails\n");
            (void)close(g_ethFd);
            retVal = RT_ERR_FAILED;
            continue;
        }
        if(ioctl(g_ethFd, SIOCGIFHWADDR, &req) < 0)
        {
            (void)rtlglue_printf("init: ioctl SIOCGIFHWADDR\n");
            (void)close(g_ethFd);
            retVal = RT_ERR_FAILED;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_tx_packet_init(&linux_eth_tx_packet));
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_rx_packet_init(&linux_eth_rx_packet));
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_smac_init(&linux_eth_smac_get));
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_dmac_init(&linux_eth_dmac_get));
    } while(0u == 1u);

    return retVal;
}

void eth_socket_close(void)
{
    if(0 < g_ethFd)
    {
        (void)close(g_ethFd);
    }
}

/* ********************************************************** */
/*  platform-depent code                                      */
/* ********************************************************** */

#if defined(RTL_907XD_VA)
/**
 * @brief <b>platform ethernet access read function</b> \n
 *         Note: regAddr MUST be sent in little-endian,
 *               and value will be sent in little-endian.
 */
RtkApiRet dva_platform_eth_read(uint32 regAddr, uint32 *value)
{
    /* implement ethernet access driver here */
    uint8                 addrData[1][RTK_REG_ADDR_LEN];
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint8                 regCmd;
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    int32                 retVal   = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;
    uint32                idx = 0u;
    EthGenBurst_t         ethGenBurst;
    EthChkBurst_t         ethCHkBusrt;
    UnitChip_t            unitChip;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        if(0x0u != (regAddr & 0x3u))
        {
            /* warning: this reg is not permitted */
            value[0] = 0x0u;
            continue;
        }

        regCmd           = DVA_ETHERACCESS_REGISTER_RW_MAC_READ;
        addrData[idx][0] = (uint8)((regAddr >> 16u) & 0xFFu);
        addrData[idx][1] = (uint8)((regAddr >> 8u) & 0xFFu);
        addrData[idx][2] = (uint8)(regAddr & 0xFFu);
        value[0]         = 0u;

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(ethGenBurst.mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(ethGenBurst.mac.sMac));
        ethGenBurst.number   = 1u;
        ethGenBurst.regCmd   = &regCmd;
        ethGenBurst.regAddr  = addrData;
        ethGenBurst.regValue = value;
        ethGenBurst.portMask = 0u;
        ethGenBurst.packet   = &txPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_burst_rw(unitChip, &ethGenBurst));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (1u * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        ethCHkBusrt.number   = 1u;
        ethCHkBusrt.regValue = value;
        ethCHkBusrt.packet   = rxPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_burst_rw(unitChip, &ethCHkBusrt));

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief <b>platform ethernet access write function</b> \n
 *         Note: both regAddr and value MUST be sent in little-endian
 */

RtkApiRet dva_platform_eth_write(uint32 regAddr, uint32 value)
{
    /* implement ethernet access driver here */
    uint8                 addrData[1][RTK_REG_ADDR_LEN];
    uint8                 rxPkt[DVA_MAX_PKT_LEN];
    uint8                 regCmd;
    uint32                rxPktLen = DVA_MAX_PKT_LEN;
    int32                 retVal   = RT_ERR_OK;
    EtherAccessL2Header_t txPkt;
    uint32                idx = 0u;
    EthGenBurst_t         ethGenBurst;
    EthChkBurst_t         ethCHkBusrt;
    UnitChip_t            unitChip;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        if(0x0u != (regAddr & 0x3u))
        {
            /* warning: this reg is not permitted */
            continue;
        }

        regCmd           = DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE;
        addrData[idx][0] = (uint8)((regAddr >> 16u) & 0xFFu);
        addrData[idx][1] = (uint8)((regAddr >> 8u) & 0xFFu);
        addrData[idx][2] = (uint8)(regAddr & 0xFFu);

        /* prepare the data */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(ethGenBurst.mac.dMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(ethGenBurst.mac.sMac));
        ethGenBurst.number   = 1u;
        ethGenBurst.regCmd   = &regCmd;
        ethGenBurst.regAddr  = addrData;
        ethGenBurst.regValue = &value;
        ethGenBurst.portMask = 0u;
        ethGenBurst.packet   = &txPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_burst_rw(unitChip, &ethGenBurst));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (1u * 8u)));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        ethCHkBusrt.number   = 1u;
        ethCHkBusrt.regValue = &value;
        ethCHkBusrt.packet   = rxPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_burst_rw(unitChip, &ethCHkBusrt));
    } while(0u == 1u);

    return retVal;
}
#endif

/* Function */
/**
 * @brief <b>Open ethernet access fd for SDK</b> \n
 *         The ethernet access register interface need to open driver fd before any register access operation\n
 */
RtkApiRet sys_reg_interface_eth_open(void)
{
    return eth_socket_open();
}

/**
 * @brief <b>Close ethernet access fd for SDK</b> \n
 *         The ethernet access register interface need to close driver fd after all register access operation are done\n
 */
void sys_reg_interface_eth_close(void)
{
    if(g_ethFd >= 0)
    {
        eth_socket_close();
    }
}

/* Function */
/**
 * @brief <b>Init register interface for SDK</b> \n
 *         This example demonstrates how register ethernet read/write callback\n
 */
RtkApiRet sys_reg_eth_interface_init(void)
{
    RtkApiRet retVal = RT_ERR_OK;

    RegifCallback_t regifCallback = {
#if defined(RTL_907XD_VA)
        .readFunc  = &dva_platform_eth_read,
        .writeFunc = &dva_platform_eth_write,
#endif
    };

    /* Initialize socket. */
    linux_eth_default_set();
    retVal = sys_reg_interface_eth_open();
    if(retVal == RT_ERR_OK)
    {
        /* Initialize register interface callback. */
        retVal = rtk_regif_init(REGIF_ETHERNET, &regifCallback);
    }

    /* Expected Behaviour:
     *    After calling this API, SDK can access switch by ethernet interface on your platform
     */

    /* If ethernet access is finished, close the socket. */
    sys_reg_interface_eth_close();

    return retVal;
}

#endif /* __linux__ */

/**@}*/ /* REGIF_ETHERNET_ACCESS_INIT */
/**@}*/ /* SYS_PORTING_EX */
/**@}*/ /* EXAMPLE */
