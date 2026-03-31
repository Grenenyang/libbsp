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
#include "rtl907xd_va_api_ethernet_access.h"
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
 * @addtogroup RTL907DVA_ETH_ACC_EXAMPLE RTL907XD_VA Ethernet Access Example
 * @{
 */

/**
 * @addtogroup RTL907DVA_ETH_ACC_OTP_ADAPT_EXAMPLE Ethernet Access L2 Adapt OTP Access Example
 * @brief <b>Ethernet Access L2 OTP Access</b>
 *
 * @section Purpose
 *   Access OTP via L2 Ethernet Access with any address.
 *
 * @section Description
 *   This example shows how to access OTP via Ethernet by using the API. The example tries to write to OTP address = 0, length = 1, and value = 0xEF to the switch via Ethernet Access. \n
 *
 * @{
 */
RtkApiRet dva_etherAccess_l2_adapt_otp_flow(uint32 unit);
RtkApiRet dva_etherAccess_l2_adapt_otp_flow(uint32 unit)
{
    uint8                         srcMac[MAC_ADDR_LEN];
    uint8                         dstMac[MAC_ADDR_LEN];
    uint8                         otpAddr    = 0xC4;
    uint16                        otpLen     = 1;
    uint32                        checksum   = 0;
    uint8                         otpData[1] = {0xEF};
    RtkEtherAccessL2OtpHdr_t      txPkt;
    RtkEtherAccessL2OtpHdr_t      rxPkt;
    RtkEtherAccessL2AdaptOtpHdr_t txOtpPkt;
    RtkEtherAccessL2AdaptOtpHdr_t rxOtpPkt;
    uint32                        rxLen = 0;
    uint8                         checkData[1];
    uint32                        i;
    RtkApiRet                     retVal = RT_ERR_OK;
    /* uuid record numSwitch(2) data */
    uint8                      uuid[RTK_ETHERACCESS_UUID_LEN];
    UnitChip_t                 unitChip;
    EthAccGenL2OtpAsk_t        ethAccGenL2OtpAsk;
    EthAccCheckL2OtpJoin_t     ethAccCheckL2OtpJoin;
    EthAccGenL2AdaptOtpCheck_t ethAccGenL2AdaptOtpCheck;
    EthAccCheckL2AdaptOtpRep_t ethAccCheckL2AdaptOtpRep;
    EthAccGenL2AdaptOtpWrite_t ethAccGenL2AdaptOtpWrite;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        /* Step 1. Initialize the Ethernet interface */
        /* Step 1.1. Set the source MAC address in the API, e.g., rtk_etherAccess_smac_get = etherAccess_smac_get */
        /* Step 1.2. Set the destination MAC address in the API, e.g., rtk_etherAccess_dmac_get = etherAccess_dmac_get */
        /* Step 1.3. Set the TX interface in the API, e.g., rtk_etherAccess_tx_packet = etherAccess_tx_packet */
        /* Step 1.4. Set the RX interface in the API, e.g., rtk_etherAccess_rx_packet = etherAccess_rx_packet */

        /* Step 2. Get the MAC address */
        (void)drv_memset(uuid, 0, RTK_ETHERACCESS_UUID_LEN);
        (void)drv_memset(&ethAccGenL2OtpAsk, 0, sizeof(EthAccGenL2OtpAsk_t));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(srcMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(dstMac));

        /* Step 3. Generate and send a packet to ask the topology */
        (void)drv_memcpy(ethAccGenL2OtpAsk.sMac, srcMac, MAC_ADDR_LEN);
        (void)drv_memcpy(ethAccGenL2OtpAsk.dMac, dstMac, MAC_ADDR_LEN);
        ethAccGenL2OtpAsk.packet = &txPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_l2_otp_ask(unitChip, &ethAccGenL2OtpAsk));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, sizeof(RtkEtherAccessL2OtpHdr_t)));

        /* Step 4. Receive the OTP join packet */
        rxLen = (uint32)sizeof(RtkEtherAccessL2OtpHdr_t);
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxPkt, &rxLen));
        ethAccCheckL2OtpJoin.packet = (uint8 *)&rxPkt;
        ethAccCheckL2OtpJoin.uuid   = uuid;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_l2_otp_join(unitChip, &ethAccCheckL2OtpJoin));

        /* Step 5. Generate and send the OTP check packet for the switch */
        (void)drv_memcpy(ethAccGenL2AdaptOtpCheck.sMac, srcMac, MAC_ADDR_LEN);
        (void)drv_memcpy(ethAccGenL2AdaptOtpCheck.dMac, dstMac, MAC_ADDR_LEN);
        ethAccGenL2AdaptOtpCheck.uuid   = uuid;
        ethAccGenL2AdaptOtpCheck.addr   = otpAddr;
        ethAccGenL2AdaptOtpCheck.len    = otpLen;
        ethAccGenL2AdaptOtpCheck.packet = &txOtpPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_l2_adapt_otp_check(unitChip, &ethAccGenL2AdaptOtpCheck));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txOtpPkt, sizeof(RtkEtherAccessL2AdaptOtpHdr_t)));

        /* Step 6. Receive the OTP reply packets */
        rxLen = (uint32)sizeof(RtkEtherAccessL2OtpHdr_t);
        (void)drv_memset(checkData, 0, otpLen);
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxOtpPkt, &rxLen));
        ethAccCheckL2AdaptOtpRep.packet = &rxOtpPkt;
        ethAccCheckL2AdaptOtpRep.uuid   = uuid;
        ethAccCheckL2AdaptOtpRep.len    = otpLen;
        ethAccCheckL2AdaptOtpRep.data   = checkData;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_l2_adapt_otp_rep(unitChip, &ethAccCheckL2AdaptOtpRep));

        /* Step 7. Calculate OTP data checksum */
        for(i = 0; i < otpLen; i++)
        {
            checksum = checksum + otpData[i];
        }

        /* Step 8. Generate and send the OTP write packet for the switch */
        (void)drv_memcpy(ethAccGenL2AdaptOtpWrite.sMac, srcMac, MAC_ADDR_LEN);
        (void)drv_memcpy(ethAccGenL2AdaptOtpWrite.dMac, dstMac, MAC_ADDR_LEN);
        ethAccGenL2AdaptOtpWrite.uuid             = uuid;
        ethAccGenL2AdaptOtpWrite.otpInfo.addr     = otpAddr;
        ethAccGenL2AdaptOtpWrite.otpInfo.len      = otpLen;
        ethAccGenL2AdaptOtpWrite.otpInfo.checksum = checksum;
        ethAccGenL2AdaptOtpWrite.data             = otpData;
        ethAccGenL2AdaptOtpWrite.packet           = &txOtpPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_l2_adapt_otp_write(unitChip, &ethAccGenL2AdaptOtpWrite));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txOtpPkt, sizeof(RtkEtherAccessL2AdaptOtpHdr_t)));

        /* Step 9. Receive the OTP reply packet */
        rxLen = (uint32)sizeof(RtkEtherAccessL2OtpHdr_t);
        (void)drv_memset(checkData, 0, otpLen);
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxOtpPkt, &rxLen));
        ethAccCheckL2AdaptOtpRep.packet = &rxOtpPkt;
        ethAccCheckL2AdaptOtpRep.uuid   = uuid;
        ethAccCheckL2AdaptOtpRep.len    = otpLen;
        ethAccCheckL2AdaptOtpRep.data   = checkData;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_l2_adapt_otp_rep(unitChip, &ethAccCheckL2AdaptOtpRep));
        /* Expected Behaviour: Write correct OTP value */
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907DVA_ETH_ACC_OTP_ADAPT_EXAMPLE */
/**@}*/ /* RTL907DVA_ETH_ACC_EXAMPLE */
/**@}*/ /* ETH_ACC_EXAMPLE */
/**@}*/ /* EXAMPLE */
