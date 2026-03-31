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
 * @addtogroup RTL907XDVA_MIB_PORT_REASON_RX_EXAMPLE Get RX Drop Reason Example
 *
 * @brief <b>Get RX Drop Reason</b> \n
 *        Get port RX drop reason on port 1, 2, and 3 for the last ingress packet.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate how to retrieve the RX drop reason for specific ports.
 *
 * @section Description
 * This function retrieves the RX drop reason for ports 1, 2, and 3 for the last ingress packet, and verifies the expected behavior.
 *
 * @image html mib/mib_per_port_get_rx_drop_reason.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_mib_per_port_get_rx_drop_reason(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_mib_per_port_get_rx_drop_reason(void)
{
    RtkApiRet                 retVal                          = RT_ERR_OK;
    uint32                    unit                            = DVA_UNIT;
    StatPortGetRxDropReason_t getPort1StatPortGetRxDropReason = {0u};
    StatPortGetRxDropReason_t getPort2StatPortGetRxDropReason = {0u};
    StatPortGetRxDropReason_t getPort3StatPortGetRxDropReason = {0u};
    UnitChip_t                unitChip                        = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;

    /* Step 1. Get port1, port2, port3 RX drop reason for the last ingress packet */
    getPort1StatPortGetRxDropReason.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_stat_port_get_rx_drop_reason(unitChip, &getPort1StatPortGetRxDropReason));
    getPort2StatPortGetRxDropReason.port = DVA_RTK_USER_PORT_2ND;
    CHK_FUN_RET(retVal, rtk_stat_port_get_rx_drop_reason(unitChip, &getPort2StatPortGetRxDropReason));
    getPort3StatPortGetRxDropReason.port = DVA_RTK_USER_PORT_3RD;
    CHK_FUN_RET(retVal, rtk_stat_port_get_rx_drop_reason(unitChip, &getPort3StatPortGetRxDropReason));
    /* Expected Behaviour: The read back values represent RX drop reason; Use the value and refer to the app note to find the reason description  */

    return retVal;
}
/**@}*/ /* RTL907XDVA_MIB_PORT_REASON_RX_EXAMPLE */
/**@}*/ /* RTL907XDVA_MIB_EXAMPLE */
/**@}*/ /* MIB_EXAMPLE */
