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
 * @addtogroup RTL907XDVA_MIB_EXAMPLE RTL907XD_VA MIB Example
 * @{
 */

/**
 * @addtogroup RTL907XDVA_MIB_PORT_DISABLE_EXAMPLE Disable Specific Port MIB Counters Example
 *
 * @brief <b>Disable Specific Port MIB Counters</b> \n
 *        Stop port MIB counters on port 1.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the disabling of MIB counters on a specific port.
 *
 * @section Description
 * This function stops the MIB counters on port 1 and verifies the expected behavior.
 *
 * @image html mib/mib_per_port_stop.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_mib_per_port_stop(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_mib_per_port_stop(void)
{
    RtkApiRet        retVal            = RT_ERR_OK;
    uint32           unit              = DVA_UNIT;
    StatPortConfig_t setStatPortConfig = {0u};
    UnitChip_t       unitChip          = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;

    /* Step 1. Stop all port MIB counters on port 1 */
    setStatPortConfig.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_stat_port_stop(unitChip, &setStatPortConfig));
    /* Expected Behaviour: All port MIB counters on port 1 will stop counting */

    return retVal;
}
/**@}*/ /* RTL907XDVA_MIB_PORT_DISABLE_EXAMPLE */
/**@}*/ /* RTL907XDVA_MIB_EXAMPLE */
/**@}*/ /* MIB_EXAMPLE */
