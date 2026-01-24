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
#include "rtkas_api_dot1x.h"

/**
 * @addtogroup DOT1X_EXAMPLE DOT1X Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_DOT1X_EXAMPLE RTL907XD_VA DOT1X Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_DOT1X_CONTROL_DIRECTION_EXAMPLE Dot1x Control Direction Example
 *
 * @brief <b>Set port-based DOT1X control direction.</b> \n
 *           We can determine whether authentication is enforced only in the ingress direction \n
 *           or in both directions by setting the control direction.
 *
 * @section Purpose
 *          This example demonstrates how to set port-based DOT1X control direction.
 * @section Description
 *          The host sends packets to switch port 1 \n
 *          Port 2 cannot receive packets but can send packets \n
 *          Port 3 can neither receive packets nor send packets
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_dot1x_control_direction(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_dot1x_control_direction(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};

    Dot1xEnPortMaskInfo_t dot1xEnPortMaskInfo = {
        .portMask = 0xFFCu,
    };
    Dot1xDirInfo_t dot1xDirInfo0 = {
        .port          = DVA_RTK_USER_PORT_2ND,
        .portDirection = DVA_DOT1X_DIR_IN,
    };
    Dot1xDirInfo_t dot1xDirInfo1 = {
        .port          = DVA_RTK_USER_PORT_3RD,
        .portDirection = DVA_DOT1X_BOTH,
    };

    /* Step 1: Enable DOT1X */
    CHK_FUN_RET(retVal, rtk_dot1x_set_enable_portmask(unitChip, &dot1xEnPortMaskInfo));

    /* Step 2: Set control direction
     * Set control direction of port 2 to incoming
     * Set control direction of port 3 to both incoming and outgoing
     */
    CHK_FUN_RET(retVal, rtk_dot1x_portbased_direction_set(unitChip, &dot1xDirInfo0));
    CHK_FUN_RET(retVal, rtk_dot1x_portbased_direction_set(unitChip, &dot1xDirInfo1));
    /* Step 3: Trigger the dynamic setting flag to set the new configuration to the switch */
    CHK_FUN_RET(retVal, rtk_dot1x_set_dynamic_reset_flag(unitChip));
    /* Expected Behavior: Port 1 can receive and send packets
     *                    Port 2 can send packets but cannot receive packets
     *                    Port 3 can neither receive nor send packets
     */
    return retVal;
}
/**@}*/ /* RTL907XDVA_DOT1X_CONTROL_DIRECTION_EXAMPLE */
/**@}*/ /* RTL907XDVA_DOT1X_EXAMPLE */
/**@}*/ /* DOT1X_EXAMPLE */
