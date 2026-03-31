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
 * @addtogroup RTL907CVA_ETH_ACC_TABLE_EXAMPLE Ethernet Access Table Access Example
 * @brief <b>Ethernet Access Table Access</b>
 *
 * @section Purpose
 *   Access table via Ethernet.
 *
 * @section Description
 *   This example shows how to access a table via Ethernet by using the API. \n
 *   The example tries to write/read 2 entries starting from entry index 3 in the CVA_VLAN table. \n
 *
 * @{
 */
RtkApiRet cva_etherAccess_table_read_write_flow(uint32 unit);
RtkApiRet cva_etherAccess_table_read_write_flow(uint32 unit)
{
    cva_table_list_t table = CVA_VLAN;
    /* Set/Get CVA_VLAN 3 and 4 */
    uint32 startIndex = 3;
    uint32 entryNum   = 2;
    /* CVA_VLAN entry number is enteyNum(2)*/
    /* CVA_VLAN entry reg number is cva_table_list[table].datareg_num(3)*/
    uint32                    setData[2][3];
    uint32                    getData[2][3];
    RtkApiRet                 retVal = RT_ERR_OK;
    UnitChip_t                unitChip;
    EthAccFlowTableEntrySet_t ethAccFlowTableEntrySet;
    EthAccFlowTableEntryGet_t ethAccFlowTableEntryGet;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;

    do
    {
        /* Initialize the data which is going to be set into the CVA_VLAN table */
        (void)drv_memset(setData, 0, sizeof(setData));
        (void)drv_memset(&ethAccFlowTableEntrySet, 0, sizeof(EthAccFlowTableEntrySet_t));
        setData[0][0]                      = 0xc03;
        setData[0][1]                      = 0x3c;
        setData[0][2]                      = 0;
        setData[1][0]                      = 0xc03;
        setData[1][1]                      = 0x30;
        setData[1][2]                      = 0;
        ethAccFlowTableEntrySet.tableCVa   = table;
        ethAccFlowTableEntrySet.startIndex = startIndex;
        ethAccFlowTableEntrySet.entryNum   = entryNum;
        ethAccFlowTableEntrySet.data       = &setData[0][0];
        ethAccFlowTableEntryGet.tableCVa   = table;
        ethAccFlowTableEntryGet.startIndex = startIndex;
        ethAccFlowTableEntryGet.entryNum   = entryNum;
        ethAccFlowTableEntryGet.data       = &getData[0][0];
        /* Step 1. Initialize the Ethernet interface */
        /* Step 1.1. Set the source MAC address in the API, e.g., rtk_etherAccess_smac_get = etherAccess_smac_get */
        /* Step 1.2. Set the destination MAC address in the API, e.g., rtk_etherAccess_dmac_get = etherAccess_dmac_get */
        /* Step 1.3. Set the TX interface in the API, e.g., rtk_etherAccess_tx_packet = etherAccess_tx_packet */
        /* Step 1.4. Set the RX interface in the API, e.g., rtk_etherAccess_rx_packet = etherAccess_rx_packet */

        /* Step 2. Set the table entry */
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_table_entry_set(unitChip, &ethAccFlowTableEntrySet));

        /* Step 3. Get the table entry */
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_table_entry_get(unitChip, &ethAccFlowTableEntryGet));
        /* Expected Behaviour: Table read/write success */
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907CVA_ETH_ACC_TABLE_EXAMPLE */
/**@}*/ /* RTL907CVA_ETH_ACC_EXAMPLE */
/**@}*/ /* ETH_ACC_EXAMPLE */
/**@}*/ /* EXAMPLE */