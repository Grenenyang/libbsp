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
 * @addtogroup RTL907CVB_ETH_ACC_PATCH_EXAMPLE Ethernet Access Patch Access Example
 * @brief <b>Ethernet Access Patch Access</b>
 *
 * @section Purpose
 *   Access Patch via Ethernet.
 *
 * @section Description
 *   This example shows how to access patches via Ethernet by using the API. \n
 *   The user should prepare and read the patch write data they want to update before starting the flash write. \n
 *   Refer to the flashless programming guide for the flashless update steps. \n
 *   Note that patch access can only work with flashless updates. \n
 *
 * @{
 */
RtkApiRet cvb_etherAccess_patch_access_flow(uint32 unit);
RtkApiRet cvb_etherAccess_patch_access_flow(uint32 unit)
{
    uint32                 addr     = 0x0;
    uint32                 fwLength = 0x800u;
    uint32                 fwAddr   = 0u;
    uint8                  fwBuf[0x800];
    RtkApiRet              retVal   = RT_ERR_OK;
    void                  *fp       = NULL;
    const int8            *fileName = "image/RTL907XC_vB/etherAccess/drvTest_etherAccess_testcase2.bin";
    UnitChip_t             unitChip;
    EthAccFlowPatchWrite_t ethAccGenPatchWrite;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VB;

    do
    {
        /* Assume fwLength and fwAddr is known */
        /* Open the binary file with the firmware */
        CHK_FUN_CONTINUE(retVal, rtl_file_open(&fp, 0u, fileName));
        if((int32)0u == rtl_file_read(fp, fwBuf, fwAddr, fwLength))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /* Step 1 initial ethernet interface */
        /* Step 1.1 set source MAC address in API, e.g rtk_etherAccess_smac_get = etherAccess_smac_get */
        /* Step 1.2 set destination MAC address in API, e.g rtk_etherAccess_dmac_get = etherAccess_dmac_get */
        /* Step 1.3 set tx interface in API, e.g rtk_etherAccess_tx_packet = etherAccess_tx_packet */
        /* Step 1.4 set rx interface in API, e.g rtk_etherAccess_rx_packet = etherAccess_rx_packet */

        /* Step 2. Write patch */
        ethAccGenPatchWrite.addr = addr;
        ethAccGenPatchWrite.len  = fwLength;
        ethAccGenPatchWrite.data = fwBuf;
        CHK_FUN_CONTINUE(retVal, rtk_eth_acc_flow_patch_write(unitChip, &ethAccGenPatchWrite));
        /* Expected Behaviour: Content of patch is updated with content of writeData */
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* RTL907CVB_ETH_ACC_PATCH_EXAMPLE */
/**@}*/ /* RTL907CVB_ETH_ACC_EXAMPLE */
/**@}*/ /* ETH_ACC_EXAMPLE */
/**@}*/ /* EXAMPLE */