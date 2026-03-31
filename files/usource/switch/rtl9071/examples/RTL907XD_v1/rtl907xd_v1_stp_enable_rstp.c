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

/** @addtogroup STP_EXAMPLE STP Example
 *  @{
 */

/**
 * @addtogroup RTL907XDV1_STP_EXAMPLE RTL907XD_V1 STP Example
 * @{
 */

/**
 * @addtogroup RTL907XDV1_STP_ENABLE_RSTP_EXAMPLE Enable RSTP Example
 *
 * @brief <b>Enable RSTP</b> \n
 *        Enable RSTP by internal CPU.
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the enabling of RSTP using an internal CPU.
 *
 * @section Description
 * This function enables RSTP by setting various RSTP parameters such as type, bridge priority, forward delay, max age, hello time, port priority, port path cost, and port states. It then verifies the settings.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_stp_enable_rstp(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_stp_enable_rstp(void)
{
    RtkApiRet                 retVal                     = RT_ERR_OK;
    uint32                    unit                       = DV1_UNIT;
    StpStateConfig_t          setStpStateConfig          = {0};
    StpStateConfig_t          getStpStateConfig          = {0};
    StpTypeConfig_t           setStpTypeConfig           = {0};
    StpTypeConfig_t           getStpTypeConfig           = {0};
    StpBridgePriorityConfig_t setStpBridgePriorityConfig = {0};
    StpBridgePriorityConfig_t getStpBridgePriorityConfig = {0};
    StpForwardDelayConfig_t   setStpForwardDelayConfig   = {0};
    StpForwardDelayConfig_t   getStpForwardDelayConfig   = {0};
    StpMaxAgeConfig_t         setStpMaxAgeConfig         = {0};
    StpMaxAgeConfig_t         getStpMaxAgeConfig         = {0};
    StpHelloTimeConfig_t      setStpHelloTimeConfig      = {0};
    StpHelloTimeConfig_t      getStpHelloTimeConfig      = {0};
    StpPortPriorityConfig_t   setStpPortPriorityConfig   = {0};
    StpPortPriorityConfig_t   getStpPortPriorityConfig   = {0};
    StpPortPathCostConfig_t   setStpPortPathCostConfig   = {0};
    StpPortPathCostConfig_t   getStpPortPathCostConfig   = {0};
    StpPortAdminEdgeConfig_t  setStpPortAdminEdgeConfig  = {0};
    StpPortAdminEdgeConfig_t  getStpPortAdminEdgeConfig  = {0};
    StpPortAutoEdgeConfig_t   setStpPortAutoEdgeConfig   = {0};
    StpPortAutoEdgeConfig_t   getStpPortAutoEdgeConfig   = {0};
    StpPortP2PConfig_t        setStpPortP2PConfig        = {0};
    StpPortP2PConfig_t        getStpPortP2PConfig        = {0};
    UnitChip_t                unitChip                   = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;
    do
    {
        /* Step 1. Set STP type. */
        setStpTypeConfig.stpType = EM_STP_TYPE_RSTP;
        CHK_FUN_CONTINUE(retVal, rtk_stp_type_set(unitChip, &setStpTypeConfig));
        /* Expected Behaviour: Set STP type to RSTP. */

        /* Step 2. Get STP type. */
        CHK_FUN_CONTINUE(retVal, rtk_stp_type_get(unitChip, &getStpTypeConfig));
        /* Expected Behaviour: Get STP type which is RSTP. */

        /* Step 3. Set bridge priority. */
        setStpBridgePriorityConfig.stpBridgePriority = RTK_STP_BRIDGE_PORT_PRIORITY_4096;
        CHK_FUN_CONTINUE(retVal, rtk_stp_bridge_priority_set(unitChip, &setStpBridgePriorityConfig));
        /* Expected Behaviour: Set bridge priority to 4096. */

        /* Step 4. Get bridge priority. */
        CHK_FUN_CONTINUE(retVal, rtk_stp_bridge_priority_get(unitChip, &getStpBridgePriorityConfig));
        /* Expected Behaviour: Get bridge priority which is 4096. */

        /* Step 5. Set forward delay. */
        setStpForwardDelayConfig.stpForwardDelay = 16u;
        CHK_FUN_CONTINUE(retVal, rtk_stp_forward_delay_set(unitChip, &setStpForwardDelayConfig));
        /* Expected Behaviour: Set forward delay to 16 seconds. */

        /* Step 6. Get forward delay. */
        CHK_FUN_CONTINUE(retVal, rtk_stp_forward_delay_get(unitChip, &getStpForwardDelayConfig));
        /* Expected Behaviour: Get forward delay which is 16 seconds. */

        /* Step 7. Set max age. */
        setStpMaxAgeConfig.stpMaxAge = 21u;
        CHK_FUN_CONTINUE(retVal, rtk_stp_max_age_set(unitChip, &setStpMaxAgeConfig));
        /* Expected Behaviour: Set max age to 21 seconds. */

        /* Step 8. Get max age. */
        CHK_FUN_CONTINUE(retVal, rtk_stp_max_age_get(unitChip, &getStpMaxAgeConfig));
        /* Expected Behaviour: Get max age which is 21 seconds. */

        /* Step 9. Set hello time. */
        setStpHelloTimeConfig.stpHelloTime = 1u;
        CHK_FUN_CONTINUE(retVal, rtk_stp_hello_time_set(unitChip, &setStpHelloTimeConfig));
        /* Expected Behaviour: Set hello time to 1 second. */

        /* Step 10. Get hello time. */
        CHK_FUN_CONTINUE(retVal, rtk_stp_hello_time_get(unitChip, &getStpHelloTimeConfig));
        /* Expected Behaviour: Get hello time which is 1 second. */

        /* Step 11. Set port priority. */
        setStpPortPriorityConfig.portId          = DV1_RTK_USER_PORT_1ST;
        setStpPortPriorityConfig.stpPortPriority = RTK_STP_PORT_PRIORITY_144;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_priority_set(unitChip, &setStpPortPriorityConfig));
        /* Expected Behaviour: Set port priority to 144. */

        /* Step 12. Get port priority. */
        getStpPortPriorityConfig.portId = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_priority_get(unitChip, &getStpPortPriorityConfig));
        /* Expected Behaviour: Get port priority which is 144. */

        /* Step 13. Set port path cost. */
        setStpPortPathCostConfig.portId          = DV1_RTK_USER_PORT_1ST;
        setStpPortPathCostConfig.stpPortPathCost = RTK_STP_PATHCOST_199999;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_path_cost_set(unitChip, &setStpPortPathCostConfig));
        /* Expected Behaviour: Set port path cost to 199999. */

        /* Step 14. Get port path cost. */
        getStpPortPathCostConfig.portId = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_path_cost_get(unitChip, &getStpPortPathCostConfig));
        /* Expected Behaviour: Get port path cost which is 199999. */

        /* Step 15. Set the port state of admin edge. */
        setStpPortAdminEdgeConfig.portId           = DV1_RTK_USER_PORT_1ST;
        setStpPortAdminEdgeConfig.stpPortAdminEdge = DISABLED;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_admin_edge_set(unitChip, &setStpPortAdminEdgeConfig));
        /* Expected Behaviour: Set the port state of admin edge to disabled. */

        /* Step 16. Get the port state of admin edge. */
        getStpPortAdminEdgeConfig.portId = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_admin_edge_get(unitChip, &getStpPortAdminEdgeConfig));
        /* Expected Behaviour: Get the port state of admin edge which is disabled. */

        /* Step 17. Set the port state of auto edge. */
        setStpPortAutoEdgeConfig.portId          = DV1_RTK_USER_PORT_1ST;
        setStpPortAutoEdgeConfig.stpPortAutoEdge = DISABLED;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_auto_edge_set(unitChip, &setStpPortAutoEdgeConfig));
        /* Expected Behaviour: Set the port state of auto edge to disabled. */

        /* Step 18. Get the port state of auto edge. */
        getStpPortAutoEdgeConfig.portId = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_auto_edge_get(unitChip, &getStpPortAutoEdgeConfig));
        /* Expected Behaviour: Get the port state of auto edge which is disabled. */

        /* Step 19. Set the port state of p2p. */
        setStpPortP2PConfig.portId     = DV1_RTK_USER_PORT_1ST;
        setStpPortP2PConfig.stpPortP2p = EM_STP_P2P_AUTO;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_p2p_set(unitChip, &setStpPortP2PConfig));
        /* Expected Behaviour: Set the port state of p2p to auto. */

        /* Step 20. Get the port state of p2p. */
        getStpPortP2PConfig.portId = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_p2p_get(unitChip, &getStpPortP2PConfig));
        /* Expected Behaviour: Get the port state of p2p which is auto. */

        /* Step 21. Set the global state of STP. */
        setStpStateConfig.stpState = ENABLED;
        CHK_FUN_CONTINUE(retVal, rtk_stp_state_set(unitChip, &setStpStateConfig));
        /* Expected Behaviour: Set the global state of STP to enabled. */

        /* Step 22. Get the global state of STP. */
        CHK_FUN_CONTINUE(retVal, rtk_stp_state_get(unitChip, &getStpStateConfig));
        /* Expected Behaviour: Get the global state of STP which is enabled. */

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907XDV1_STP_ENABLE_RSTP_EXAMPLE */
/**@}*/ /* RTL907XDV1_STP_EXAMPLE */
/**@}*/ /* STP_EXAMPLE */
