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

#include "rtkas_api_ext.h"
#include "rtkas_api_macsec.h"

/* Declaration */
RtkApiRet cva_classify_control_pkt_by_ether_type(void);
RtkApiRet cva_classify_control_pkt_by_mac_da(void);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup MACSEC_EXAMPLE MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XC_VA_MACSEC_EXAMPLE RTL907XC_VA MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XC_VA_MACSEC_CONTROL_PACKET_ETHER_TYPE MACsec Classify Control Packet By Ethertype Example
 *
 * @brief <b> Specific Ethertype As Control Packet </b> \n
 *        Configure a specific Ethertype packet as control pacekt. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to configurae a specific Ethertype packet as control pacekt to bypass
 *          MACsec hardware engine.
 *
 * @section Description
 *          This example demonstrates how to treat all packets with EtherType 0x0806 as control packets, \n
 *          and they will bypass all following MACsec hardware processing.
 *
 * @{
 */
RtkApiRet cva_classify_control_pkt_by_ether_type(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step 1. configure control packet entry 0 to set etherType 0x0806 as control packet */
    MacsecControlPacketEtherTypeEntry_t etherTypeCtrlPkt = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_RX,
        .entryIdx  = RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX,
        .enable    = true,
        .etherType = 0x0806u,
    };
    CHK_FUN_RET(retVal, rtk_macsec_set_control_packet_ether_type_entry(unitChip, &etherTypeCtrlPkt));

    /* Step 2. get control packet entry 0 to verify if it is expected */
    MacsecControlPacketEtherTypeEntry_t runtimConfig = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_RX,
        .entryIdx  = RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX,
    };
    CHK_FUN_RET(retVal, rtk_macsec_get_control_packet_ether_type_entry(unitChip, &runtimConfig));
    /* Expect Behaviour: get runtime enable and mac data for Tx macsec engine entry 0, the runtimeEnable should be true and runtimeEtherType should be 0x 0806*/

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_CONTROL_PACKET_ETHER_TYPE */

/**
 * @addtogroup RTL907XC_VA_MACSEC_CONTROL_PACKET_MAC_DA MACsec Classify Control Packet By MAC DA Example
 *
 * @brief <b> Specific Destination Mac As Control Packet </b> \n
 *        Configure a packet with specific destination MAC address as control pacekt. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to configurae a packet with specific destination MAC address
 *          as control pacekt to bypass MACsec hardware engine.
 *
 * @section Description
 *          This example demonstrates how to treat all packets with destination MAC address is 00-E0-4C-11-22-33 as control packets, \n
 *          and they will bypass all following MACsec hardware processing.
 *
 * @{
 */
RtkApiRet cva_classify_control_pkt_by_mac_da(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step 1. configure control packet entry 0 to set destination MAC addesss 00-E0-4C-11-22-33 as control packet */
    MacsecControlPacketMacDaEntry_t MacDaCtrlPkt = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_RX,
        .entryIdx  = RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX,
        .enable    = true,
        .mac       = {0x00u, 0xE0u, 0x4Cu, 0x11u, 0x22u, 0x33u},
    };
    CHK_FUN_RET(retVal, rtk_macsec_set_control_packet_mac_da_entry(unitChip, &MacDaCtrlPkt));

    /* Step 2. get control packet entry 0 to verify if it is expected */
    MacsecControlPacketMacDaEntry_t runtimConfig = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_RX,
        .entryIdx  = RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX,
    };

    CHK_FUN_RET(retVal, rtk_macsec_get_control_packet_mac_da_entry(unitChip, &runtimConfig));
    /* Expect Behaviour: get runtime enable and mac data for Rx macsec engine entry 0 */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_CONTROL_PACKET_MAC_DA */
/**@}*/ /* RTL907XC_VA_MACSEC_EXAMPLE */
/**@}*/ /* MACSEC_EXAMPLE */
/**@}*/ /* EXAMPLE */