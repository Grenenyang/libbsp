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
#include "rtkas_common.h"
#include "rtl907xd_v1_api_ethernet_access.h"
#include "rtkas_api_ethernet_access.h"
#include "rtkas_system_porting.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup ETH_ACC_EXAMPLE Ethernet Access Example
 * @{
 */

/**
 * @defgroup RTL907DV1_ETH_ACC_EXAMPLE RTL907XD_V1 Ethernet Access Example
 * @{
 */

/**
 * @addtogroup RTL907DV1_ETH_ACC_REG_EXAMPLE Ethernet Access Reg Access Example
 * @brief <b>Ethernet Access Register Access</b>
 *
 * @section Purpose
 *   Access registers via Ethernet.
 *
 * @section Description
 *   This example shows how to access registers via Ethernet by using the API. \n
 *   The example tries to read register 0x180000 and write to register 0x180004 with the value 0x843. \n
 *
 * @{
 */
RtkApiRet dv1_etherAccess_reg_read_write_flow(uint32 unit);
RtkApiRet dv1_etherAccess_reg_read_write_flow(uint32 unit)
{
    uint32                numBurst = 2;
    uint32                txPktLen = DV1_RTK_ETHERACCESS_HEADER_LEN + (numBurst * 8u);
    uint32                rxPktLen = DV1_MAX_PKT_LEN;
    EtherAccessL2Header_t txPkt;
    uint8                 rxPkt[DV1_MAX_PKT_LEN];
    uint8                 burstRegCmd[2]                    = {DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE};
    uint8                 burstRegAddr[2][RTK_REG_ADDR_LEN] = {{0x18, 0x00, 0x00}, {0x18, 0x00, 0x04}};
    uint32                burstRegVal[2]                    = {0, 0x843};
    uint32                regValue[2];
    RtkApiRet             retVal = RT_ERR_OK;
    UnitChip_t            unitChip;
    EthGenBurst_t         ethGenBurst;
    EthChkBurst_t         ethChkBurst;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;

    do
    {
        /* Step 1. Initialize the Ethernet interface */
        /* Step 1.1. Set the source MAC address in the API, e.g., rtk_etherAccess_smac_get = etherAccess_smac_get */
        /* Step 1.2. Set the destination MAC address in the API, e.g., rtk_etherAccess_dmac_get = etherAccess_dmac_get */
        /* Step 1.3. Set the TX interface in the API, e.g., rtk_etherAccess_tx_packet = etherAccess_tx_packet */
        /* Step 1.4. Set the RX interface in the API, e.g., rtk_etherAccess_rx_packet = etherAccess_rx_packet */

        /* Step 2. Get the MAC address */
        (void)drv_memset(&ethGenBurst, 0, sizeof(EthGenBurst_t));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(ethGenBurst.mac.sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(ethGenBurst.mac.dMac));

        /* Step 3. Generate a packet to read the register and write to the register */
        ethGenBurst.number   = numBurst;
        ethGenBurst.regCmd   = burstRegCmd;
        ethGenBurst.regAddr  = &burstRegAddr[0];
        ethGenBurst.regValue = burstRegVal;
        ethGenBurst.packet   = &txPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_burst_rw(unitChip, &ethGenBurst));

        /* Step 4. Send the packet to read the register and write to the register */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, txPktLen));

        /* Step 5. Receive the packet to check if the read/write was successful or failed */
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

        /* Step 6. Parse the data */
        ethChkBurst.number   = numBurst;
        ethChkBurst.regValue = regValue;
        ethChkBurst.packet   = rxPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_burst_rw(unitChip, &ethChkBurst));
        /* Expected Behaviour: Get data is expected */
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907DV1_ETH_ACC_REG_EXAMPLE */
/**@}*/ /* RTL907DV1_ETH_ACC_EXAMPLE */
/**@}*/ /* ETH_ACC_EXAMPLE */
/**@}*/ /* EXAMPLE */