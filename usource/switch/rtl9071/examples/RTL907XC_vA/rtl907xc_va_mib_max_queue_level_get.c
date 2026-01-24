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
#include "rtkas_api_mib.h"

/** @addtogroup MIB_EXAMPLE MIB Example
 *  @{
 */

/**
 * @addtogroup RTL907XCVA_MIB_EXAMPLE RTL907XC_VA MIB Example
 * @{
 */

/**
 * @addtogroup RTL907XCVA_MIB_PORT_QUEUE_EXAMPLE Get Specific Port RX Max Queue Level Example
 *
 * @brief <b>Get Specific Port RX Max Queue Level</b> \n
 *        Get max queue level on port 1, 2, and 3.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate how to retrieve the RX max queue level for specific ports.
 *
 * @section Description
 * This function retrieves the RX max queue level for ports 1, 2, and 3, and verifies the expected behavior.
 *
 * @image html mib/mib_max_queue_level_get.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_mib_max_queue_level_get(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_mib_max_queue_level_get(void)
{
    RtkApiRet                  retVal                           = RT_ERR_OK;
    uint32                     unit                             = CVA_UNIT;
    StatPortMaxQueueLevelGet_t getPort1StatPortMaxQueueLevelGet = {0u};
    StatPortMaxQueueLevelGet_t getPort2StatPortMaxQueueLevelGet = {0u};
    StatPortMaxQueueLevelGet_t getPort3StatPortMaxQueueLevelGet = {0u};
    UnitChip_t                 unitChip                         = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;

    /* Step 1. Get port1, port2, port3 RX max queue level (unit = page; 1 page = 128Byte) */
    getPort1StatPortMaxQueueLevelGet.port = CVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_stat_port_max_queue_level_get(unitChip, &getPort1StatPortMaxQueueLevelGet));
    getPort2StatPortMaxQueueLevelGet.port = CVA_RTK_USER_PORT_2ND;
    CHK_FUN_RET(retVal, rtk_stat_port_max_queue_level_get(unitChip, &getPort2StatPortMaxQueueLevelGet));
    getPort3StatPortMaxQueueLevelGet.port = CVA_RTK_USER_PORT_3RD;
    CHK_FUN_RET(retVal, rtk_stat_port_max_queue_level_get(unitChip, &getPort3StatPortMaxQueueLevelGet));
    /* Expected Behaviour: The read back values represents RX max queue level (unit = page; 1 page = 128Byte) */

    return retVal;
}
/**@}*/ /* RTL907XCVA_MIB_PORT_QUEUE_EXAMPLE */
/**@}*/ /* RTL907XCVA_MIB_EXAMPLE */
/**@}*/ /* MIB_EXAMPLE */
