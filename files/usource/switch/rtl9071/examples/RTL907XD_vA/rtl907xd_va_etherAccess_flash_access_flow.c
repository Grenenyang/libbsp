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
 * @addtogroup RTL907DVA_ETH_ACC_FLASH_EXAMPLE Ethernet Access Flash Access Example
 * @brief <b>Ethernet Access Flash Access</b>
 *
 * @section Purpose
 *   Access flash via Ethernet.
 *
 * @section Description
 *   This example shows how to access flash directly via Ethernet by using the API. \n
 *   The user can use provided API to directly erase, read, and write flash. \n
 *   The user can use provided API to disable other protocols before an important update to prevent any impact. \n
 *   The user can use provided API to reset system after the update is finished. \n
 *   The user should prepare and read the flash write data they want to update before starting the flash write. \n
 *   Refer to the flash programming guide for the flash update steps. \n
 *
 * @{
 */
RtkApiRet dva_etherAccess_flash_access_flow(uint32 unit);
RtkApiRet dva_etherAccess_flash_access_flow(uint32 unit)
{
    uint32                    addr     = 0x0;
    uint32                    num      = 1;
    uint32                    readLen  = 0x400;
    uint32                    writeLen = DVA_FLASH_ZONE_SIZE_512KB;
    uint8                     readData[0x400];
    uint8                     writeData[DVA_FLASH_ZONE_SIZE_512KB];
    RtkApiRet                 retVal   = RT_ERR_OK;
    void                     *fp       = NULL;
    const int8               *fileName = "image/RTL907XD_va/etherAccess/drvTest_etherAccess_testcase2.bin";
    UnitChip_t                unitChip;
    EthAccFlowFlashSecErase_t ethAccFlowFlashSecErase;
    EthAccFlowFlashBlkErase_t ethAccFlowFlashBlkErase;
    EthAccFlowFlashRead_t     ethAccFlowFlashRead;
    EthAccFlowFlashWrite_t    ethAccFlowFlashWrite;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;

    do
    {
        (void)drv_memset(readData, 0, sizeof(readData));
        (void)drv_memset(writeData, 0, sizeof(writeData));
        /* Open the binary file with the firmware */
        CHK_FUN_CONTINUE(retVal, rtl_file_open(&fp, 0u, fileName));
        if((int32)0u == rtl_file_read(fp, writeData, 0, writeLen))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /* Step 1. Initialize the Ethernet interface */
        /* Step 1.1. Set the source MAC address in the API, e.g., rtk_etherAccess_smac_get = etherAccess_smac_get */
        /* Step 1.2. Set the destination MAC address in the API, e.g., rtk_etherAccess_dmac_get = etherAccess_dmac_get */
        /* Step 1.3. Set the TX interface in the API, e.g., rtk_etherAccess_tx_packet = etherAccess_tx_packet */
        /* Step 1.4. Set the RX interface in the API, e.g., rtk_etherAccess_rx_packet = etherAccess_rx_packet */

        /* Step 2.1. Erase 1 sector of flash */
        ethAccFlowFlashSecErase.addr = addr;
        ethAccFlowFlashSecErase.num  = num;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_sec_erase(unitChip, &ethAccFlowFlashSecErase));

        /* Step 2.2. Erase 1 block of flash */
        ethAccFlowFlashBlkErase.addr = addr;
        ethAccFlowFlashBlkErase.num  = num;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_blk_erase(unitChip, &ethAccFlowFlashBlkErase));

        /* Step 2.3. Read flash */
        ethAccFlowFlashRead.addr = addr;
        ethAccFlowFlashRead.len  = readLen;
        ethAccFlowFlashRead.data = readData;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_read(unitChip, &ethAccFlowFlashRead));

        /* Step 2.4. Erase all flash */
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_all_erase(unitChip));

        /* Step 3. Disable protocol before updating flash to prevent busy traffic from affecting it */
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_disable_protocol(unitChip));

        /* Step 4. Write flash */
        ethAccFlowFlashWrite.addr = addr;
        ethAccFlowFlashWrite.len  = writeLen;
        ethAccFlowFlashWrite.data = writeData;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_flash_write(unitChip, &ethAccFlowFlashWrite));

        /* Step 5. Reset system */
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_reset_system(unitChip));
        /* Expected Behaviour: Content of flash is updated with content of writeData */
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907DVA_ETH_ACC_FLASH_EXAMPLE */
/**@}*/ /* RTL907DVA_ETH_ACC_EXAMPLE */
/**@}*/ /* ETH_ACC_EXAMPLE */
/**@}*/ /* EXAMPLE */