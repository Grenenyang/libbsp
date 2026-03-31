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
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_asicdrv_phy.h"
#include "rtkas_api_phy.h"
#include "rtl907xc_vb_asicdrv_opfsm.h"
#include "rtkas_api_opfsm.h"
#include "rtl907xc_vb_api_opfsm.h"

/* Declaration */
RtkApiRet cvb_tc10_enable_allport_sleep_cap(uint32 unit);
RtkApiRet cvb_tc10_config_sleep_accept_and_forwarding(uint32 unit);
RtkApiRet cvb_tc10_config_wholechip_sleep(uint32 unit);
RtkApiRet cvb_tc10_lwake_cfg(uint32 unit);
RtkApiRet cvb_tc10_rwake_fwd_cfg(uint32 unit);
RtkApiRet cvb_tc10_wakeo_cfg(uint32 unit);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup TC10_OPFSM_EXAMPLE Sleep and Wake Function Example
 * @{
 */

/** @defgroup RTL907XC_VB_SLEEP_WAKE RTL907XC_VB Sleep and Wake Example
 *  @{
 */

/** @defgroup RTL907XC_VB_SLEEP_WAKE_INTF_CFG Sleep and Wake Interface Configuration Example
 *  @{
 */

/* Function */

/** @defgroup RTL907XC_VB_SLEEP Sleep Configuration Example
 *  @brief <b>Configure Port TC10 Sleep Settings</b>
 *
 *  @section Purpose
 *  Providing configuration example of port TC10 sleep, including sleep capability, accept sleep request, and whole chip sleep.
 *
 *  @section Description
 *  This example provides a sample configuration related to sleep settings.
 *  @{
 */

/**
 * @brief <b>Enable the sleep capability of TC10-compatible ports</b>
 *
 * @details
 * This function enables the sleep capability of all TC10-compatible ports on the specified unit.
 *
 * @section Purpose
 * - Activate the sleep capability of all TC10-compatible ports.
 * - Allow ports to perform sleep handshake with their link partners.
 * - Enable ports to initialize a sleep request to their link partners or recognize received sleep requests.
 *
 * @section Description
 * 1. Pre-config: Get the ports that support sleep function according to the chip defines.
 * 2. Get the port map of ports enabled sleep capability.
 * 3. If some ports support sleep/wake but disable its sleep capability, enable those port sleep capability.
 * 4. Verify the set port sleep capability function result.
 * 5. If all ports sleep capability are enabled, write PHY reset register to activate the sleep functionality.
 */
RtkApiRet cvb_tc10_enable_allport_sleep_cap(uint32 unit)
{
    RtkApiRet      rtkRet       = RT_ERR_OK;
    UnitChip_t     unitChip     = {.chip = CHIP_907XD_VA, .unit = unit};
    OpSlpCap_t     opSlpCap     = {0u};
    PhyResetInfo_t phyResetInfo = {0u};
    RtkPmap        tc10Pmap     = 0u;

    do
    {
        /* Pre-config. get the ports support sleep function according to the chip defines */
#ifdef CVB_OP_RTPGE_USER_PORTMAP
        tc10Pmap |= CVB_OP_RTPGE_USER_PORTMAP;
#endif
#ifdef CVB_OP_BWPHY_USER_PORTMAP
        tc10Pmap |= CVB_OP_BWPHY_USER_PORTMAP;
#endif

        /* Step 1. get the port map of ports disabled sleep capability */
        opSlpCap.sleepCap = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_get(unitChip, &opSlpCap));
        /* Expected Behavior: the opSlpCap.portMap returns the portmap of ports disable sleep capability */

        /* Step 2. if opSlpCap.portMap indicates some ports support sleep/wake but disable its sleep capability, enable those port sleep capability */
        if(0u != opSlpCap.portMap)
        {
            /* Step3 enable the sleep capability of the ports indicated in opSlpCap.portMap */
            opSlpCap.sleepCap = ENABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_set(unitChip, &opSlpCap));
            /* Step 4: verify the set port sleep capability function result */
            opSlpCap.portMap  = 0u;
            opSlpCap.sleepCap = ENABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_get(unitChip, &opSlpCap));
            if(tc10Pmap != opSlpCap.portMap)
            {
                rtkRet = -RT_ERR_PORT_MASK;
                continue;
            }
            /* if all ports sleep capability are enabled, it is required to conduct PHY reset to activate the PHY sleep functionality */
            else
            {
                phyResetInfo.portMap = tc10Pmap;
                CHK_FUN_CONTINUE(rtkRet, rtk_phy_bmcr_reset(unitChip, &phyResetInfo));
            }
        }
        /* Expected Behavior: all ports support sleep/wake enable its sleep capability */
    } while(0u == 1u);

    return rtkRet;
}

/**
 * @brief <b>Configure Accept Sleep Port map and Sleep Request Forwarding Port map</b>
 *
 * @details
 * This function configures some ports to allow accepting sleep requests and enables forwarding sleep requests.
 *
 * @section Purpose
 * - Configure ports to allow accepting received sleep requests.
 * - Configure ports to enable forwarding sleep requests.
 *
 * @section Description
 * 1. Configure ports to allow accept received sleep request can reply the received TC10 sleep request.
 * 2. Configure ports to not allow accept received sleep request will not reply the received TC10 sleep request.
 * 3. Configure ports to enable sleep request forwarding. The ports enabled sleep request forwarding will initialize a sleep request to its link partner once another port has receive sleep request event.
 * 4. Configure ports disable sleep request forwarding will not initialize a sleep request to its link partner once another port receives sleep request.
 *
 * Once the link partner switch2 starts a sleep request on port1, the switch1 port1 will reply a sleep request to the link partner, and finally both port1 will enter sleep mode.
 * Upon switch1 port1 receives sleep request, the switch1 port3, port4 and port5 enable sleep forwarding will starts sleep request to its link partner.
 * If a port with enabled sleep capability is configured to not accept sleep requests, it will reject the sleep request upon receiving it.
 * However, other ports with enabled sleep forwarding will still forward the sleep request.
 *
 * @image html opfsm_tc10/accept_sleep.png
 * @image html opfsm_tc10/txsleep_with_accept_sleep.png
 * @image html opfsm_tc10/txsleep_with_reject_sleep.png
 */
RtkApiRet cvb_tc10_config_sleep_accept_and_forwarding(uint32 unit)
{
    RtkApiRet           rtkRet            = RT_ERR_OK;
    UnitChip_t          unitChip          = {.chip = CHIP_907XD_VA, .unit = unit};
    OpAcceptSlpReqCap_t opAcceptSlpReqCap = {0u};
    OpTxSleepMsk_t      opTxSleepMsk      = {0u};
    RtkPmap             cvb_acceptSlpPmap = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD);
    RtkPmap             cvb_slpFwdPmap    = (CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_4TH | CVB_RTK_USER_PMAP_5TH);

    do
    {
        /* Step 1: enable accept sleep request of the user port1, port2 and port3 */
        opAcceptSlpReqCap.portMap  = cvb_acceptSlpPmap;
        opAcceptSlpReqCap.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_accept_sleep_req_cap_set(unitChip, &opAcceptSlpReqCap));
        /* Step 2: check the accept sleep request configuration result */
        opAcceptSlpReqCap.portMap  = 0x0;
        opAcceptSlpReqCap.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_accept_sleep_req_cap_get(unitChip, &opAcceptSlpReqCap));
        if(cvb_acceptSlpPmap != opAcceptSlpReqCap.portMap)
        {
            rtkRet = -RT_ERR_FAILED;
            continue;
        }
        /* Step3 enable sleep request forwarding of the user port1, port2 and port3 */
        opTxSleepMsk.portMap  = cvb_slpFwdPmap;
        opTxSleepMsk.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_txsleep_portmap_set(unitChip, &opTxSleepMsk));
        /* Step 4: check the sleep request forwarding configuration result */
        opTxSleepMsk.portMap  = 0x0;
        opTxSleepMsk.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_txsleep_portmap_get(unitChip, &opTxSleepMsk));
        if(cvb_slpFwdPmap != opTxSleepMsk.portMap)
        {
            rtkRet = -RT_ERR_FAILED;
            continue;
        }
        /* Expect behavior:
         * Once the link partner switch2 starts a sleep request on port1, the switch1 port1 will reply a sleep request to the link partner, and finally both port1 will enter sleep mode.
         * Upon switch1 port1 receives sleep request, the switch1 port3, port4 and port5 enable sleep forwarding will starts sleep request to its link partner.
         */
    } while(0u == 1u);

    return rtkRet;
}

/**
 * @brief <b>Enable the switch to enter global sleep state once the designated ports enter local sleep state</b>
 *
 * @details
 * This function enables the whole chip sleep function and specifies the whole chip sleep port map.
 *
 * @section Purpose
 * - Enable the switch to go to global sleep mode if some designated ports enter local sleep mode.
 * - Specify the whole chip sleep port map.
 *
 * @section Description
 * After enable the whole chip sleep function, it is required to specify the whole chip sleep port map.
 * If the ports belongs to the whole chip sleep port map enter local sleep mode and the global whole chip sleep function is enabled,
 * the switch firmware will command the switch go to global sleep mode.
 *
 * 1. Pre-config: Get the ports that support sleep function according to the chip defines.
 * 2. Enable global whole chip sleep function if it is not enabled.
 * 3. Specify the port1, port3, and port6 as members of the whole chip sleep port map.
 * 4. Set the whole chip sleep port map to 0.
 * 5. Add port1, port3, and port6 to the whole chip sleep port map if no port is in the whole chip sleep port map.
 *
 * @image html opfsm_tc10/enter_wholechip_sleep.png
 * @image html opfsm_tc10/not_enter_wholechip_sleep.png
 */
RtkApiRet cvb_tc10_config_wholechip_sleep(uint32 unit)
{
    RtkApiRet           rtkRet              = RT_ERR_OK;
    UnitChip_t          unitChip            = {.chip = CHIP_907XD_VA, .unit = unit};
    OpWholechipSlpEn_t  opWholechipSlpEn    = {0u};
    OpWholechipSlpMsk_t opWholechipSlpMsk   = {0u};
    RtkPmap             tc10Pmap            = 0u;
    RtkPmap             cvb_wholechipSlpMsk = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_6TH);

    do
    {
        /* Pre-config. get the ports support sleep function according to the chip defines */
#ifdef CVB_OP_RTPGE_USER_PORTMAP
        tc10Pmap |= CVB_OP_RTPGE_USER_PORTMAP;
#endif
#ifdef CVB_OP_BWPHY_USER_PORTMAP
        tc10Pmap |= CVB_OP_BWPHY_USER_PORTMAP;
#endif

        /* Step 1: Enable global whole chip sleep function if the it is not enabled. */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wholechip_sleep_en_get(unitChip, &opWholechipSlpEn));
        if(DISABLE == opWholechipSlpEn.wholeChipOn)
        {
            opWholechipSlpEn.wholeChipOn = ENABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_wholechip_sleep_en_set(unitChip, &opWholechipSlpEn));
        }
        /* Step 2: Add the port1, port3 and port6 to the whole chip sleep port map. */
        opWholechipSlpMsk.portMap  = cvb_wholechipSlpMsk;
        opWholechipSlpMsk.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_wholechip_sleep_portmap(unitChip, &opWholechipSlpMsk));
        /* Step 3: Remove all ports from the whole chip sleep port map except for port1, port3, and port6. */
        opWholechipSlpMsk.portMap  = tc10Pmap ^ cvb_wholechipSlpMsk;
        opWholechipSlpMsk.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_wholechip_sleep_portmap(unitChip, &opWholechipSlpMsk));
        /* Step 4: Get the whole chip sleep port map */
        opWholechipSlpMsk.portMap  = 0u;
        opWholechipSlpMsk.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_get_wholechip_sleep_portmap(unitChip, &opWholechipSlpMsk));
        if(cvb_wholechipSlpMsk != opWholechipSlpMsk.portMap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }
        /* Expect behavior: Once the port1, port3 and port6 are in sleep mode simultaneously, the switch firmware will command the switch go to global sleep mode.
         * After the switch enter global sleep mode, the INH pin transitions from high to low.
         */
    } while(0u == 1u);

    return rtkRet;
}
/**@}*/ /* RTL907XC_VB_SLEEP */

/** @defgroup RTL907XC_VB_WAKE Wake Configuration Example
 *  @brief <b>Configure Port TC10 Wake Settings</b>
 *
 *  @section Purpose
 *  Providing configuration example of local wakeup, remote wakeup, wake-out forwarding.
 *
 *  @section Description
 *  This example provides a sample configuration of local wakeup, remote wakeup, wake-out forwarding.
 *
 *  @{
 */

/**
 * @brief <b>Configure the local-wakeup related function</b>
 *
 * @details
 * This function configures the edge selection for the local wake pin and triggers a wake-up request upon detecting a local wake.
 *
 * @section Purpose
 * - Configure the edge selection for the local wake pin.
 * - Trigger wake-up request upon detecting a local wake.
 *
 * @section Description
 * If the edge selection is raising-edge, the switch will detect a local wakeup event when detect a raising-edge.
 * If the edge selection is falling-edge, the switch will detect a local wakeup event when detect a falling-edge.
 * When an lwake event is detected, a port configured with enable lwake_wake_port will wake up from sleep mode.
 * If the port has enabled wakeup transmit capability, it will send a remote wakeup signal to the counterpart.
 *
 * 1. Pre-config: Get the ports that support sleep function according to the chip defines.
 * 2. Enable all port sleep capability and make all ports accept sleep requests, and not conduct sleep request forwarding.
 * 3. Configure the local wake edge detection to falling edge.
 * 4. Configure local wake-up event trigger for port1, port3, and port6 to send wake-up requests.
 * 5. Read and clear the WUP and WUR flags to allow the switch port to send wake-up requests.
 * 6. Read-clear the lwake flag to allow the switch to detect the local wake-up event.
 *
 * @image html opfsm_tc10/lwake_wake_port.png
 */
RtkApiRet cvb_tc10_lwake_cfg(uint32 unit)
{
    RtkApiRet           rtkRet              = RT_ERR_OK;
    UnitChip_t          unitChip            = {.chip = CHIP_907XD_VA, .unit = unit};
    OpSlpCap_t          opSlpCap            = {0u};
    OpAcceptSlpReqCap_t opAcceptSlpReqCap   = {0u};
    OpTxSleepMsk_t      opTxSleepMsk        = {0u};
    PhyResetInfo_t      phyResetInfo        = {0u};
    OpLwakeEdgeSelect_t opLwakeEdgeSelect   = {0u};
    OpLwakeWakePort_t   opLwakeWakePort     = {0u};
    RtkPmap             tc10Pmap            = 0u;
    uint32              regVal              = 0u;
    RtkPmap             cvb_lwakeToRwakeMsk = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_6TH);

    do
    {
        /* Pre-config. get the ports support sleep function according to the chip defines */
#ifdef CVB_OP_RTPGE_USER_PORTMAP
        tc10Pmap |= CVB_OP_RTPGE_USER_PORTMAP;
#endif
#ifdef CVB_OP_BWPHY_USER_PORTMAP
        tc10Pmap |= CVB_OP_BWPHY_USER_PORTMAP;
#endif

        /* Pre-config. Enable all port sleep capability and make all port accept sleep request, and not conduct sleep request forwarding. */
        opSlpCap.portMap  = tc10Pmap;
        opSlpCap.sleepCap = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_set(unitChip, &opSlpCap));
        phyResetInfo.portMap = tc10Pmap;
        CHK_FUN_CONTINUE(rtkRet, rtk_phy_bmcr_reset(unitChip, &phyResetInfo));
        opAcceptSlpReqCap.portMap  = tc10Pmap;
        opAcceptSlpReqCap.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_accept_sleep_req_cap_set(unitChip, &opAcceptSlpReqCap));
        opTxSleepMsk.portMap  = tc10Pmap;
        opTxSleepMsk.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_txsleep_portmap_set(unitChip, &opTxSleepMsk));

        /* Step 1: Configure the local wake edge detection to falling edge */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_lwake_edge_sel_get(unitChip, &opLwakeEdgeSelect));
        if(OP_LWAKE_RISING_EDGE == opLwakeEdgeSelect.lwakeEdge)
        {
            opLwakeEdgeSelect.lwakeEdge = OP_LWAKE_FALLING_EDGE;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_lwake_edge_sel_set(unitChip, &opLwakeEdgeSelect));
        }

        /* Step 2: Configure local wake up event trigger port1, port3 and port6 to send wake up request */
        opLwakeWakePort.cfgValue = DISABLE;
        opLwakeWakePort.portMap  = tc10Pmap;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_lwake_wake_port_set(unitChip, &opLwakeWakePort));
        opLwakeWakePort.cfgValue = ENABLE;
        opLwakeWakePort.portMap  = cvb_lwakeToRwakeMsk;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_lwake_wake_port_set(unitChip, &opLwakeWakePort));
        opLwakeWakePort.cfgValue = ENABLE;
        opLwakeWakePort.portMap  = 0u;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_lwake_wake_port_get(unitChip, &opLwakeWakePort));
        if(cvb_lwakeToRwakeMsk != opLwakeWakePort.portMap)
        {
            rtkRet = -RT_ERR_FAILED;
            continue;
        }

        /* Step 3: It is required to read and clear the WUP and WUR flag to allow the switch port to send wakeup request,
         *  and read-clear the lwake flag to allow the switch detect the local wake up event */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeflag_clearall(unitChip));
        /* The lwake flag is CVB_OCP_PWON_FLAG bit[10] */
        CHK_FUN_CONTINUE(rtkRet, reg_read(unitChip, (CVB_OP_PWON_FLAG << 1), &regVal));

        /* Expect behavior:
         * Assume the port1, port3 and port6 finish sleep handshake and are in sleep mode,
         * pull down the wake pin and after switch detect a falling edge on wake pin,
         * the lwake flag will will be set and trigger port1, port3 and port6 send WUP to its link partner during port1/3/6 are in local standby mode.
         */
    } while(0u == 1u);

    return rtkRet;
}

/**
 * @brief <b>Configure the wake-up request forwarding function</b>
 *
 * @details
 * This function configures the wake-up request forwarding, allowing some ports to be woken up when designated ports are woken up.
 *
 * @section Purpose
 * - Forward a received wake-up request on a port to another port.
 * - Enable some ports to be woken up when designated ports are woken up.
 *
 * @section Description
 * 1. Pre-config: Get the ports that support sleep function according to the chip defines.
 * 2. Enable all ports to send wake-up requests and detect wake-up requests.
 * 3. Configure the wake-up request forwarding for each port.
 * 4. Verify the wake-up request forwarding configuration for each port.
 *
 * By configuring the wakeup request forwarding, some port can be also waken up when the designated ports are wakened up.
 * In this example, the port2, port3, port4, port6 will forward the detected remote wakeup request ti port1.
 * The port1, port3, port4, port6 will forward the detected remote wakeup request to port2.
 * The port4, port5, port6 will forward the detected remote wakeup request to port3.
 *
 * @image html opfsm_tc10/px_rwake_cfg_p1.png
 * @image html opfsm_tc10/px_rwake_cfg_p2.png
 * @image html opfsm_tc10/px_rwake_cfg_p3.png
 * @image html opfsm_tc10/px_rwake_cfg_p4.png
 * @image html opfsm_tc10/px_rwake_cfg_p5.png
 * @image html opfsm_tc10/px_rwake_cfg_p6.png
 */
RtkApiRet cvb_tc10_rwake_fwd_cfg(uint32 unit)
{
    RtkApiRet            rtkRet             = RT_ERR_OK;
    UnitChip_t           unitChip           = {.chip = CHIP_907XD_VA, .unit = unit};
    OpEnWuprTx_t         opEnWuprTx         = {0};
    OpDisRwake_t         opDisRwake         = {0};
    OpRwakeFwdPxCfgSet_t opRwakeFwdPxCfgSet = {0};
    OpRwakeFwdPxCfgGet_t opRwakeFwdPxCfgGet = {0};
    RtkPmap              tc10Pmap           = 0u;
    RtkPmap              cfgPmap            = 0u;

    do
    {
        /* Pre-config. get the ports support sleep function according to the chip defines */
#ifdef CVB_OP_RTPGE_USER_PORTMAP
        tc10Pmap |= CVB_OP_RTPGE_USER_PORTMAP;
#endif
#ifdef CVB_OP_BWPHY_USER_PORTMAP
        tc10Pmap |= CVB_OP_BWPHY_USER_PORTMAP;
#endif

        /* Pre-config. Enable all port send wakeup request ability and wakeup detection ability. */
        opEnWuprTx.portMap  = tc10Pmap;
        opEnWuprTx.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_en_wupr_tx_set(unitChip, &opEnWuprTx));
        opDisRwake.portMap  = tc10Pmap;
        opDisRwake.cfgValue = 0u;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_disable_rwake_set(unitChip, &opDisRwake));

        /* Step 1: Configure the port1 */
        opRwakeFwdPxCfgSet.portSel = CVB_RTK_USER_PORT_1ST;
        /* Step 1.1: P1 enable remote wakeup forwarding by port2, port3, port4 ,port6  */
        cfgPmap                     = (CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_4TH | CVB_RTK_USER_PMAP_6TH);
        opRwakeFwdPxCfgSet.portMap  = cfgPmap;
        opRwakeFwdPxCfgSet.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgSet));
        /* Step 1.2: P1 disable remote wakeup forwarding by other ports except port2, port3, port4 ,port6 */
        opRwakeFwdPxCfgSet.portMap  = tc10Pmap ^ cfgPmap;
        opRwakeFwdPxCfgSet.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgSet));
        /* Step 1.3: Get P1 remote wakeup forwarding configuration */
        opRwakeFwdPxCfgGet.portSel = CVB_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_get_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgGet));
        if(cfgPmap != opRwakeFwdPxCfgGet.portmap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Step 2: Configure the port2 */
        opRwakeFwdPxCfgSet.portSel = CVB_RTK_USER_PORT_2ND;
        /* Step 2.1: P2 enable remote wakeup forwarding by port1, port3, port4 ,port6  */
        cfgPmap                     = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_4TH | CVB_RTK_USER_PMAP_6TH);
        opRwakeFwdPxCfgSet.portMap  = cfgPmap;
        opRwakeFwdPxCfgSet.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgSet));
        /* Step 2.2: P2 disable remote wakeup forwarding by other ports except port1, port3, port4 ,port6 */
        opRwakeFwdPxCfgSet.portMap  = tc10Pmap ^ cfgPmap;
        opRwakeFwdPxCfgSet.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgSet));
        /* Step 2.3: Get P2 remote wakeup forwarding configuration */
        opRwakeFwdPxCfgGet.portSel = CVB_RTK_USER_PORT_2ND;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_get_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgGet));
        if(cfgPmap != opRwakeFwdPxCfgGet.portmap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Step3 Configure the port3 */
        opRwakeFwdPxCfgSet.portSel = CVB_RTK_USER_PORT_3RD;
        /* Step 3.1: P3 enable remote wakeup forwarding by port4, port5 ,port6  */
        cfgPmap                     = (CVB_RTK_USER_PMAP_4TH | CVB_RTK_USER_PMAP_5TH | CVB_RTK_USER_PMAP_6TH);
        opRwakeFwdPxCfgSet.portMap  = cfgPmap;
        opRwakeFwdPxCfgSet.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgSet));
        /* Step 3.2: P3 disable remote wakeup forwarding by other ports except port4, port5 ,port6 */
        opRwakeFwdPxCfgSet.portMap  = tc10Pmap ^ cfgPmap;
        opRwakeFwdPxCfgSet.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_set_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgSet));
        /* Step 3.3: Get P3 remote wakeup forwarding configuration */
        opRwakeFwdPxCfgGet.portSel = CVB_RTK_USER_PORT_3RD;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_get_portmap_fwd_rwake_to_port(unitChip, &opRwakeFwdPxCfgGet));
        if(cfgPmap != opRwakeFwdPxCfgGet.portmap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Expect behavior:
         * Trigger port2 send wakeup request when port1 receive wakeup request
         * Trigger port1 send wakeup request when port2 receive wakeup request
         * Trigger port1 and port2 send wakeup request when port3 receive wakeup request
         * Trigger port1, port2, port3 send wakeup request when port4 receive wakeup request
         * Trigger port2, port3 send wakeup request when port5 receive wakeup request
         * Trigger port1, port3 send wakeup request when port6 receive wakeup request
         */
    } while(0u == 1u);

    return rtkRet;
}

/**
 * @brief <b>Configure the wake output function</b>
 *
 * @details
 * This function configures the wake-up event forwarding to the specified GPIO.
 *
 * @section Purpose
 * - Forward the wake-up event to the specified wake pin.
 * - Generate specified pulse on the configured pins when the specified ports receive WUP or WUR.
 *
 * @section Description
 * 1. Pre-config: Get the ports that support sleep function according to the chip defines.
 * 2. Configure the wake output forwarding pulse width to 500ms.
 * 3. Configure WUP/WUR detected on port2 and port3 to trigger wake output forwarding pulse on GPIOB1.
 * 4. Configure WUP/WUR detected on port3 and port4 to trigger wake output forwarding pulse on GPIOB2.
 *
 * By enable the wake out forwarding function, if the specified ports received WUP or WUR,
 * the switch will generate specified pulse on the configured pins.
 * In this example, configure port2, port3 receive WUP/WUR trigger Square Wave with 500ms pulse width on GPIOB1.
 * Configure port3, port4 receive WUP/WUR trigger Square Wave with 500ms pulse width on GPIOB2.
 */
RtkApiRet cvb_tc10_wakeo_cfg(uint32 unit)
{
    RtkApiRet        rtkRet         = RT_ERR_OK;
    UnitChip_t       unitChip       = {.chip = CHIP_907XD_VA, .unit = unit};
    OpWakeoPortMap_t opWakeoPortMap = {0};
    OpWakeoPulse_t   opWakeoPulse   = {0};
    RtkPmap          tc10Pmap       = 0u;
    RtkPmap          cfgPmap        = 0u;

    do
    {
        /* Pre-config. get the ports support sleep function according to the chip defines */
#ifdef CVB_OP_RTPGE_USER_PORTMAP
        tc10Pmap |= CVB_OP_RTPGE_USER_PORTMAP;
#endif
#ifdef CVB_OP_BWPHY_USER_PORTMAP
        tc10Pmap |= CVB_OP_BWPHY_USER_PORTMAP;
#endif

        /* Step 1: configure the wake out forwarding pulse width to 500ms */
        opWakeoPulse.pulseCfg = OP_WAKEO_PULSETYPE_500MS;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_pulse_set(unitChip, &opWakeoPulse));
        opWakeoPulse.pulseCfg = OP_WAKEO_PULSETYPE_UNKNOWN;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_pulse_get(unitChip, &opWakeoPulse));
        if(OP_WAKEO_PULSETYPE_500MS != opWakeoPulse.pulseCfg)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Step 2: Make WUP/WUR detected on port2 and port3 trigger wake out forwarding pulse on the GPIOB1 */
        opWakeoPortMap.configIdx = OP_WAKEO_GPIOB1_ID;
        cfgPmap                  = (CVB_RTK_USER_PORT_2ND | CVB_RTK_USER_PORT_3RD);
        opWakeoPortMap.portMap   = cfgPmap;
        opWakeoPortMap.cfgValue  = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_portmap_set(unitChip, &opWakeoPortMap));
        opWakeoPortMap.portMap  = tc10Pmap ^ cfgPmap;
        opWakeoPortMap.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_portmap_set(unitChip, &opWakeoPortMap));
        opWakeoPortMap.portMap  = 0u;
        opWakeoPortMap.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_portmap_get(unitChip, &opWakeoPortMap));
        if(cfgPmap != opWakeoPortMap.portMap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Step 3: Make WUP/WUR detected on port3 and port4 trigger wake out forwarding pulse on the GPIOB2 */
        opWakeoPortMap.configIdx = OP_WAKEO_GPIOB2_ID;
        cfgPmap                  = (CVB_RTK_USER_PORT_3RD | CVB_RTK_USER_PORT_4TH);
        opWakeoPortMap.portMap   = cfgPmap;
        opWakeoPortMap.cfgValue  = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_portmap_set(unitChip, &opWakeoPortMap));
        opWakeoPortMap.portMap  = tc10Pmap ^ cfgPmap;
        opWakeoPortMap.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_portmap_set(unitChip, &opWakeoPortMap));
        opWakeoPortMap.portMap  = 0u;
        opWakeoPortMap.cfgValue = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeo_portmap_get(unitChip, &opWakeoPortMap));
        if(cfgPmap != opWakeoPortMap.portMap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Expect behavior:
         * When the port2, port3 receive WUP/WUR, the switch firmware will trigger square wave with 500ms pulse width on GPIOB1.
         * When the port3, port4 receive WUP/WUR, the switch firmware will trigger square wave with 500ms pulse width on GPIOB2.
         */
    } while(0u == 1u);

    return rtkRet;
}

/**@}*/ /* RTL907XC_VB_WAKE */

/**@}*/ /* RTL907XC_VB_SLEEP_WAKE_INTF_CFG */
/**@}*/ /* RTL907XC_VB_SLEEP_WAKE */
/**@}*/ /* TC10_OPFSM_EXAMPLE */
/**@}*/ /* EXAMPLE */
