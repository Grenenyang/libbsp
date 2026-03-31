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
#include "rtl907xc_vb_api_ethernet_access.h"
#include "rtkas_api_ethernet_access.h"
#include "rtkas_system_porting.h"
#include "rtl907xc_vb_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup ETH_ACC_EXAMPLE Ethernet Access Example
 * @{
 */

/**
 * @addtogroup RTL907CVB_ETH_ACC_EXAMPLE RTL907XC_VB Ethernet Access Example
 * @{
 */

/**
 * @addtogroup RTL907CVB_ETH_ACC_FLASH_REQ_EXAMPLE Ethernet Access Flash Request Access Example
 * @brief <b>Ethernet Access Flash Access with Req</b>
 *
 * @section Purpose
 *   Access flash via Ethernet with requests.
 *
 * @section Description
 *   This example shows how to access flash with requirement in the background via Ethernet by using the API. \n
 *   The user can use provided API to erase flash. \n
 *   Refer to the flash programming guide for the flash update steps. \n
 *
 * @{
 */
RtkApiRet cvb_etherAccess_flash_access_req_flow(uint32 unit);
RtkApiRet cvb_etherAccess_flash_access_req_flow(uint32 unit)
{
    uint32                       addr   = 0x0;
    uint32                       num    = 1;
    RtkApiRet                    retVal = RT_ERR_OK;
    UnitChip_t                   unitChip;
    EthAccFlowFlashSecEraseReq_t ethAccFlowFlashSecEraseReq;
    EthAccFlowFlashBlkEraseReq_t ethAccFlowFlashBlkEraseReq;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1. Initialize the Ethernet interface */
        /* Step 1.1. Set the source MAC address in the API, e.g., rtk_etherAccess_smac_get = etherAccess_smac_get */
        /* Step 1.2. Set the destination MAC address in the API, e.g., rtk_etherAccess_dmac_get = etherAccess_dmac_get */
        /* Step 1.3. Set the TX interface in the API, e.g., rtk_etherAccess_tx_packet = etherAccess_tx_packet */
        /* Step 1.4. Set the RX interface in the API, e.g., rtk_etherAccess_rx_packet = etherAccess_rx_packet */

        /* Step 2.1. Erase 1 sector of flash */
        ethAccFlowFlashSecEraseReq.addr = addr;
        ethAccFlowFlashSecEraseReq.num  = num;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_req_sec_erase(unitChip, &ethAccFlowFlashSecEraseReq));

        /* Step 2.2. Erase 1 block of flash */
        ethAccFlowFlashBlkEraseReq.addr = addr;
        ethAccFlowFlashBlkEraseReq.num  = num;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_req_blk_erase(unitChip, &ethAccFlowFlashBlkEraseReq));

        /* Step 2.3. Erase all flash */
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_req_all_erase(unitChip));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907CVB_ETH_ACC_FLASH_REQ_EXAMPLE */
/**@}*/ /* RTL907CVB_ETH_ACC_EXAMPLE */
/**@}*/ /* ETH_ACC_EXAMPLE */
/**@}*/ /* EXAMPLE */