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
#include "rtkas_api_stp.h"

/** @addtogroup EXAMPLE
 *  @{
 */

/** @addtogroup STP_EXAMPLE STP Example
 *  @{
 */

/**
 * @addtogroup RTL907XCVB_STP_EXAMPLE RTL907XC_VB STP Example
 * @{
 */

/**
 * @addtogroup RTL907XCVB_STP_SET_PORT_STATE_EXAMPLE Configure Port State Example
 *
 * @brief <b>Configure Port State</b> \n
 *        If Spanning Tree Protocol (STP/RSTP/MSTP) is implemented by an external CPU, it can configure the \n
 *        port state with rtk_stp_state_set() and rtk_stp_state_get().\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of port states in STP using an external CPU.
 *
 * @section Description
 * This function configures the port states by setting and getting the global STP state and individual port states.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_stp_set_port_state(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_stp_set_port_state(void)
{
    RtkApiRet            retVal                = RT_ERR_OK;
    uint32               unit                  = CVB_UNIT;
    StpStateConfig_t     setStpStateConfig     = {0};
    StpStateConfig_t     getStpStateConfig     = {0};
    StpPortStateConfig_t setStpPortStateConfig = {0};
    StpPortStateConfig_t getStpPortStateConfig = {0};
    UnitChip_t           unitChip              = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;
    do
    {
        /* Step 1. Set the global state of STP. */
        setStpStateConfig.stpState = DISABLED;
        CHK_FUN_CONTINUE(retVal, rtk_stp_state_set(unitChip, &setStpStateConfig));
        /* Expected Behaviour: The global state of STP is disabled. */

        /* Step 2. Get the global state of STP. */
        CHK_FUN_CONTINUE(retVal, rtk_stp_state_get(unitChip, &getStpStateConfig));
        /* Expected Behaviour: Get the global state of STP which is disabled. */

        /* Step 3. Set the port state based on the implementation. */

        /* Set the port state. */
        setStpPortStateConfig.msti      = 1u;
        setStpPortStateConfig.portId    = CVB_RTK_USER_PORT_1ST;
        setStpPortStateConfig.portState = EM_STP_DISCARDING;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_set(unitChip, &setStpPortStateConfig));
        /* Expected Behaviour: Set the port state of port 1 to discarding. */

        /* Get the port state. */
        getStpPortStateConfig.msti   = 1u;
        getStpPortStateConfig.portId = CVB_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_get(unitChip, &getStpPortStateConfig));
        /* Expected Behaviour: Get the port state of port 1 which is discarding. */

        /* Set the port state. */
        setStpPortStateConfig.msti      = 1u;
        setStpPortStateConfig.portId    = CVB_RTK_USER_PORT_2ND;
        setStpPortStateConfig.portState = EM_STP_BLOCKING;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_set(unitChip, &setStpPortStateConfig));
        /* Expected Behaviour: Set the port state of port 2 to blocking. */

        /* Get the port state. */
        getStpPortStateConfig.msti   = 1u;
        getStpPortStateConfig.portId = CVB_RTK_USER_PORT_2ND;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_get(unitChip, &getStpPortStateConfig));
        /* Expected Behaviour: Get the port state of port 2 which is blocking. */

        /* Set the port state. */
        setStpPortStateConfig.msti      = 1u;
        setStpPortStateConfig.portId    = CVB_RTK_USER_PORT_3RD;
        setStpPortStateConfig.portState = EM_STP_LEARNING;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_set(unitChip, &setStpPortStateConfig));
        /* Expected Behaviour: Set the port state of port 3 to learning. */

        /* Get the port state. */
        getStpPortStateConfig.msti   = 1u;
        getStpPortStateConfig.portId = CVB_RTK_USER_PORT_3RD;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_get(unitChip, &getStpPortStateConfig));
        /* Expected Behaviour: Get the port state of port 3 which is learning. */

        /* Set the port state. */
        setStpPortStateConfig.msti      = 1u;
        setStpPortStateConfig.portId    = CVB_RTK_USER_PORT_4TH;
        setStpPortStateConfig.portState = EM_STP_FORWARDING;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_set(unitChip, &setStpPortStateConfig));
        /* Expected Behaviour: Set the port state of port 4 to forwarding. */

        /* Get the port state. */
        getStpPortStateConfig.msti   = 1u;
        getStpPortStateConfig.portId = CVB_RTK_USER_PORT_4TH;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_state_get(unitChip, &getStpPortStateConfig));
        /* Expected Behaviour: Get the port state of port 4 which is forwarding. */

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907XCVB_STP_SET_PORT_STATE_EXAMPLE */
/**@}*/ /* RTL907XCVB_STP_EXAMPLE */
/**@}*/ /* STP_EXAMPLE */
/**@}*/ /* EXAMPLE */
