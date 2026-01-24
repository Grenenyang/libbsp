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
#include "rtl907xc_va_api_ethernet_access.h"
#include "rtkas_api_ethernet_access.h"
#include "rtkas_system_porting.h"

typedef struct
{
    uint8 uuid[RTK_ETHERACCESS_UUID_LEN];
} cva_etherAccess_map_uuid_t;

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup ETH_ACC_EXAMPLE Ethernet Access Example
 * @{
 */

/**
 * @addtogroup RTL907CVA_ETH_ACC_EXAMPLE RTL907XC_VA Ethernet Access Example
 * @{
 */

/**
 * @addtogroup RTL907CVA_ETH_ACC_OTP_EXAMPLE Ethernet Access L3 OTP Access Example
 * @brief <b>Ethernet Access L3 OTP Access</b>
 *
 * @section Purpose
 *   Access OTP via L3 Ethernet Access with a specified field.
 *
 * @section Description
 *   This example shows how to access OTP with a specified field via Ethernet by using the API. \n
 *   The example tries to write OTP type = 1, type_id = 0, and value = 0xEF to all switches (2 switches are used) on the topology via Ethernet Access with source IP = 17.34.51.68. \n
 *
 * @{
 */

RtkApiRet cva_etherAccess_otp_flow(uint32 unit);
RtkApiRet cva_etherAccess_otp_flow(uint32 unit)
{
    uint8                      srcMac[CVA_MAC_ADDR_LEN];
    uint32                     sip        = 0x11223344;
    uint32                     numSwitch  = 2;
    uint8                      type       = 1;
    uint8                      typeId     = 0;
    uint32                     otpLen     = 1;
    uint8                      otpData[1] = {0xEF};
    EtherAccessL2IpHdr_t       txPkt;
    EtherAccessL2IpHdr_t       rxPkt;
    uint32                     rxLen = 0;
    uint8                      checkData[1];
    uint32                     checksum = 0;
    uint32                     i;
    RtkApiRet                  retVal = RT_ERR_OK;
    cva_etherAccess_map_uuid_t uuidTable[2];
    UnitChip_t                 unitChip;
    EthAccGenL3OtpAsk_t        ethAccGenL3OtpAsk;
    EthAccGetL3OtpTtl_t        ethAccGetL3OtpTtl;
    EthAccCheckL3OtpJoin_t     ethAccCheckL3OtpJoin;
    EthAccGenL3OtpCheck_t      ethAccGenL3OtpCheck;
    EthAccCheckL3OtpRep_t      ethAccCheckL3OtpRep;
    EthAccGenL3OtpWrite_t      ethAccGenL3OtpWrite;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;

    do
    {
        /* Step 1. Initialize the Ethernet interface */
        /* Step 1.1. Set the source MAC address in the API, e.g., rtk_etherAccess_smac_get = etherAccess_smac_get */
        /* Step 1.2. Set the TX interface in the API, e.g., rtk_etherAccess_tx_packet = etherAccess_tx_packet */
        /* Step 1.3. Set the RX interface in the API, e.g., rtk_etherAccess_rx_packet = etherAccess_rx_packet */

        /* Step 2. Get the MAC address */
        (void)drv_memset(uuidTable, 0, sizeof(cva_etherAccess_map_uuid_t) * numSwitch);
        (void)drv_memset(&ethAccGenL3OtpAsk, 0, sizeof(EthAccGenL3OtpAsk_t));
        (void)drv_memset(&ethAccGenL3OtpCheck, 0, sizeof(EthAccGenL3OtpCheck_t));
        (void)drv_memset(&ethAccCheckL3OtpRep, 0, sizeof(EthAccCheckL3OtpRep_t));
        (void)drv_memset(&ethAccGenL3OtpWrite, 0, sizeof(EthAccGenL3OtpWrite_t));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(srcMac));

        /* Step 3. Generate and send a packet to ask the topology */
        (void)drv_memcpy(ethAccGenL3OtpAsk.macAddr, srcMac, CVA_MAC_ADDR_LEN);
        ethAccGenL3OtpAsk.sip    = sip;
        ethAccGenL3OtpAsk.packet = &txPkt;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_l3_otp_ask(unitChip, &ethAccGenL3OtpAsk));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, sizeof(EtherAccessL2IpHdr_t)));

        /* Step 4. Receive the OTP join packets */
        for(i = 0; i < numSwitch; i++)
        {
            rxLen = (uint32)sizeof(EtherAccessL2IpHdr_t);
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxPkt, &rxLen));
            /* Step 4.1 Get TTL */
            ethAccGetL3OtpTtl.packet = (uint8 *)&rxPkt;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_get_l3_otp_ttl(unitChip, &ethAccGetL3OtpTtl));
            /* Step 4.2 Check if the packet is correct and detect the joined UUID */
            ethAccCheckL3OtpJoin.packet = (uint8 *)&rxPkt;
            ethAccCheckL3OtpJoin.uuid   = uuidTable[255u - ethAccGetL3OtpTtl.ttl].uuid;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_l3_otp_join(unitChip, &ethAccCheckL3OtpJoin));
        }

        /* Step 5. Generate and send the OTP check packet for all switches */
        for(i = 0; i < numSwitch; i++)
        {
            (void)drv_memcpy(ethAccGenL3OtpCheck.macAddr, srcMac, CVA_MAC_ADDR_LEN);
            ethAccGenL3OtpCheck.sip    = sip;
            ethAccGenL3OtpCheck.uuid   = uuidTable[i].uuid;
            ethAccGenL3OtpCheck.type   = type;
            ethAccGenL3OtpCheck.typeId = typeId;
            ethAccGenL3OtpCheck.len    = otpLen;
            ethAccGenL3OtpCheck.packet = &txPkt;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_l3_otp_check(unitChip, &ethAccGenL3OtpCheck));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVA_MAX_PKT_LEN));
        }

        /* Step 6. Receive the OTP reply packets */
        for(i = 0; i < numSwitch; i++)
        {
            rxLen = (uint32)sizeof(EtherAccessL2IpHdr_t);
            (void)drv_memset(checkData, 0, otpLen);
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxPkt, &rxLen));
            /* Step 6.1 Get TTL */
            ethAccGetL3OtpTtl.packet = (uint8 *)&rxPkt;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_get_l3_otp_ttl(unitChip, &ethAccGetL3OtpTtl));
            /* Step 6.2 Check if the packet is correct */
            ethAccCheckL3OtpRep.packet = &rxPkt;
            ethAccCheckL3OtpRep.uuid   = uuidTable[255u - ethAccGetL3OtpTtl.ttl].uuid;
            ethAccCheckL3OtpRep.type   = type;
            ethAccCheckL3OtpRep.typeId = typeId;
            ethAccCheckL3OtpRep.len    = otpLen;
            ethAccCheckL3OtpRep.data   = checkData;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_l3_otp_rep(unitChip, &ethAccCheckL3OtpRep));
        }

        /* Step 7. Calculate the OTP data checksum. The user should decide how to overwrite the original data */
        for(i = 0; i < otpLen; i++)
        {
            checksum = checksum + otpData[i];
        }

        /* Step 8. Generate and send the OTP write packet for all switches */
        for(i = 0; i < numSwitch; i++)
        {
            (void)drv_memcpy(ethAccGenL3OtpWrite.macAddr, srcMac, CVA_MAC_ADDR_LEN);
            ethAccGenL3OtpWrite.otpInfo.type     = type;
            ethAccGenL3OtpWrite.otpInfo.typeId   = typeId;
            ethAccGenL3OtpWrite.otpInfo.len      = otpLen;
            ethAccGenL3OtpWrite.otpInfo.checksum = checksum;
            ethAccGenL3OtpWrite.sip              = sip;
            ethAccGenL3OtpWrite.uuid             = uuidTable[i].uuid;
            ethAccGenL3OtpWrite.data             = otpData;
            ethAccGenL3OtpWrite.packet           = &txPkt;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_gen_l3_otp_write(unitChip, &ethAccGenL3OtpWrite));
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVA_MAX_PKT_LEN));
        }

        /* Step 9. Receive the OTP reply packets */
        for(i = 0; i < numSwitch; i++)
        {
            rxLen = (uint32)sizeof(EtherAccessL2IpHdr_t);
            (void)drv_memset(checkData, 0, otpLen);
            CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet((uint8 *)&rxPkt, &rxLen));
            /* Step 9.1 Get TTL */
            ethAccGetL3OtpTtl.packet = (uint8 *)&rxPkt;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_get_l3_otp_ttl(unitChip, &ethAccGetL3OtpTtl));
            /* Step 9.2 Check if the packet is correct */
            ethAccCheckL3OtpRep.packet = &rxPkt;
            ethAccCheckL3OtpRep.uuid   = uuidTable[255u - ethAccGetL3OtpTtl.ttl].uuid;
            ethAccCheckL3OtpRep.type   = type;
            ethAccCheckL3OtpRep.typeId = typeId;
            ethAccCheckL3OtpRep.len    = otpLen;
            ethAccCheckL3OtpRep.data   = checkData;
            CHK_FUN_CONTINUE(retVal, rtk_eth_acc_check_l3_otp_rep(unitChip, &ethAccCheckL3OtpRep));
        }
        /* Expected Behaviour: Write correct OTP value */
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907CVA_ETH_ACC_OTP_EXAMPLE */
/**@}*/ /* RTL907CVA_ETH_ACC_EXAMPLE */
/**@}*/ /* ETH_ACC_EXAMPLE */
/**@}*/ /* EXAMPLE */