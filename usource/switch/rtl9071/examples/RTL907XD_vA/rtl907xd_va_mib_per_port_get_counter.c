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
 * @addtogroup RTL907XDVA_MIB_PORT_GET_EXAMPLE Get Specific Port MIB Counters Example
 *
 * @brief <b>Get Specific Port MIB Counters</b> \n
 *        Get port MIB counters on port 1, 2, and 3.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate how to retrieve MIB counters for specific ports.
 *
 * @section Description
 * This function retrieves the MIB counters for ports 1, 2, and 3, and verifies the expected behavior.
 *
 * @image html mib/mib_per_port_get_counter.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_mib_per_port_get_counter(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_mib_per_port_get_counter(void)
{
    RtkApiRet     retVal              = RT_ERR_OK;
    uint32        unit                = DVA_UNIT;
    StatPortGet_t getPort1StatPortGet = {0u};
    StatPortGet_t getPort2StatPortGet = {0u};
    StatPortGet_t getPort3StatPortGet = {0u};
    UnitChip_t    unitChip            = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;

    /* Step 1. Get port1, port2, port3 "EM_DOT1DTPPORTINFRAMES" counter */
    getPort1StatPortGet.port        = DVA_RTK_USER_PORT_1ST;
    getPort1StatPortGet.counterType = EM_DOT1DTPPORTINFRAMES;
    CHK_FUN_RET(retVal, rtk_stat_port_get(unitChip, &getPort1StatPortGet));
    getPort2StatPortGet.port        = DVA_RTK_USER_PORT_2ND;
    getPort2StatPortGet.counterType = EM_DOT1DTPPORTINFRAMES;
    CHK_FUN_RET(retVal, rtk_stat_port_get(unitChip, &getPort2StatPortGet));
    getPort3StatPortGet.port        = DVA_RTK_USER_PORT_3RD;
    getPort3StatPortGet.counterType = EM_DOT1DTPPORTINFRAMES;
    CHK_FUN_RET(retVal, rtk_stat_port_get(unitChip, &getPort3StatPortGet));
    /* Expected Behaviour: The read back values represent port0, port1, port2 "EM_DOT1DTPPORTINFRAMES" counter */

    return retVal;
}
/**@}*/ /* RTL907XDVA_MIB_PORT_GET_EXAMPLE */
/**@}*/ /* RTL907XDVA_MIB_EXAMPLE */
/**@}*/ /* MIB_EXAMPLE */
