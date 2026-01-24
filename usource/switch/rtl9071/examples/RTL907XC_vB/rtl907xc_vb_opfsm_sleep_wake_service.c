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
#include "rtkas_api_opfsm.h"
#include "rtkas_api_phy.h"
#include "rtl907xc_vb_api.h"
#include "rtkas_api_reset.h"
#include "rtl907xc_vb_api_opfsm.h"

/* Declaration */
RtkApiRet cvb_tc10_global_op_cmd(uint32 unit);
RtkApiRet cvb_tc10_local_op_cmd(uint32 unit);
RtkApiRet cvb_tc10_send_sleep_req(uint32 unit);
RtkApiRet cvb_tc10_sleep_event_indication(uint32 unit);
RtkApiRet cvb_tc10_wake_req_and_indication(uint32 unit);
RtkApiRet cvb_tc10_interrupt_cfg(uint32 unit);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup TC10_OPFSM_EXAMPLE Sleep and Wake Function Example
 * @{
 */

/** @defgroup RTL907XC_VB_SLEEP_WAKE RTL907XC_VB Sleep and Wake Example
 *  @{
 */

/** @defgroup RTL907XC_VB_SLEEP_WAKE_SERVICE RTL907XC_VB Sleep and Wake Service Primitives Example
 *  @{
 */

/* Function */
/** @defgroup RTL907XC_VB_OPFSM OPFSM Configuration Example
 *  @brief <b>OPFSM Configuration Example</b>
 *
 *  @section Purpose
 *  Providing setting and getting OP-FSM Example
 *
 *  @section Description
 *  This example provides a sample to command OP-FSM and get OP-FSM status.
 *  @{
 */

/**
 * @brief <b>Global OPFSM State Set and Get</b>
 *
 * @details
 * This function uses host commands to transition the switch to a specific global operation mode.
 *
 * @section Purpose
 * Use host command to command the switch to go to a specific global operation mode.
 *
 * @section Description
 * There are various modes in the switch, including:
 * - Power-off
 * - Sleep
 * - Safety
 * - Normal
 * - Standby
 *
 * Each operation mode has differences in:
 * - Power dissipation
 * - Availability of establishing the link
 * - Reg accessibility
 *
 * The SDK API supports command switch transitions to Normal, Standby, and Sleep modes.
 *
 * For detailed operation mode transitions, please refer to the Operation Modes (OP) section in the application note.
 */
RtkApiRet cvb_tc10_global_op_cmd(uint32 unit)
{
    RtkApiRet     rtkRet      = RT_ERR_OK;
    UnitChip_t    unitChip    = {.chip = CHIP_907XD_VA, .unit = unit};
    OpGlobState_t opGlobState = {0};

    do
    {
        /* Pre-config. reset switch */
        CHK_FUN_CONTINUE(rtkRet, rtk_reset_chip_rst(unitChip));

        /* Step 1: get the current global OP state of switch */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_global_state_get(unitChip, &opGlobState));
        /* Expected Behavior: Return the current global OP state of switch */

        /* Step 2: If the current global mode is NORMAL mode, command the switch go to STANDBY mode */
        if(OP_GLOB_NORMAL == opGlobState.state)
        {
            opGlobState.state = OP_GLOB_STANDBY;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_global_state_set(unitChip, &opGlobState));
        }
        /* Expected Behavior: The switch global OP state transition to STANDBY mode. All T1 port can not link-up and transmit packets */

        /* Step 3: If the current global mode is in STANDBY mode, command the switch go to NORMAL mode */
        if(OP_GLOB_STANDBY == opGlobState.state)
        {
            opGlobState.state = OP_GLOB_NORMAL;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_global_state_set(unitChip, &opGlobState));
        }
        /* Expected Behavior: The switch global OP state transition to NORMAL mode. All T1 port can link-up and transmit packets */

        /* Step 4: If the current global mode is in NORMAL mode, command the switch go to SLEEP mode */
        if(OP_GLOB_NORMAL == opGlobState.state)
        {
            opGlobState.state = OP_GLOB_WHOLE_CHIP_SLEEP;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_global_state_set(unitChip, &opGlobState));
        }
        /* Expected Behavior: The switch global OP state transition to SLEEP mode. All T1 port can link-up and transmit packets, the register cannot be accessed. */
    } while(0u == 1u);

    return rtkRet;
}

/**
 * @brief <b>Local OPFSM State Set and Get</b>
 *
 * @details
 * This function uses host commands to transition a specific switch port to a local operation mode.
 *
 * @section Purpose
 * Use host command to command the switch port to go to a specific local operation mode.
 *
 * @section Description
 * The SDK API supports command port local state transitions to:
 * - Normal mode
 * - Standby mode
 * - Sleep mode
 *
 * For detailed operation mode transitions, please refer to the Operation Modes (OP) section in the application note.
 *
 * @image html opfsm_tc10/local_state.png
 */
RtkApiRet cvb_tc10_local_op_cmd(uint32 unit)
{
    RtkApiRet          rtkRet           = RT_ERR_OK;
    UnitChip_t         unitChip         = {.chip = CHIP_907XD_VA, .unit = unit};
    OpPortLocalState_t opPortLocalState = {0};
    OpPmapLocalState_t opPmapLocalState = {0};

    do
    {
        /* Pre-config. reset switch */
        CHK_FUN_CONTINUE(rtkRet, rtk_reset_chip_rst(unitChip));

        /* Step 1: get the current local OP state of switch port1 */
        opPortLocalState.port = CVB_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_port_local_state_get(unitChip, &opPortLocalState));
        /* Expected Behavior: Return the current local OP state of port1 */

        /* Step 2: If the current local mode of port1 is NORMAL mode, command the port1 go to STANDBY mode */
        if(OP_LOCAL_NORMAL == opPortLocalState.state)
        {
            opPortLocalState.state = OP_LOCAL_STANDBY;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_port_local_state_set(unitChip, &opPortLocalState));
        }
        /* Expected Behavior: The switch port1 local OP state transition to STANDBY mode */

        /* Step 3: Get the ports with local state is in NORMAL mode, at this time all T1 ports expect port1 are in NORMAL mode. Command these ports transition to SLEEP mode */
        opPmapLocalState.state = OP_LOCAL_NORMAL;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_portmap_local_state_get(unitChip, &opPmapLocalState));
        opPmapLocalState.state = OP_LOCAL_SLEEP;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_portmap_local_state_set(unitChip, &opPmapLocalState));
        /* Expected Behavior: The ports except port1 will transition to sleep mode */
    } while(0u == 1u);

    return rtkRet;
}
/**@}*/ /* RTL907XC_VB_OPFSM */

/** @defgroup RTL907XC_VB_SLEEP_REQ_AND_INDICATION Initialize Sleep Request and Get Sleep Indications Example
 *  @brief <b>Initialize Sleep Request and Get Sleep Indications Example</b>
 *
 *  @section Purpose
 *  Providing example of initializing a sleep handshake and get the sleep event indications
 *
 *  @section Description
 *  This example provides a example to command a port to start a sleep request and get the sleep event indications including receive sleep request, sleep handshake fail and sleep success.
 *  @{
 */

/**
 * @brief <b>Sleep Request and Sleep Indication</b>
 *
 * @details
 * This function uses the SDK API to initialize a TC10 sleep handshake with its link partner.
 * The port local state will enter local sleep state and the Ethernet link will be passive after finishing the sleep handshaking.
 * It also retrieves various sleep indication flags during the sleep handshake.
 *
 * @section Purpose
 * Use SDK API to initialize a sleep handshake with the link partner and get the sleep indication.
 *
 * @section Description
 * The switch supports initializing the TC10 sleep handshake with its link partner by SDK API.
 * The port local state will enter local sleep state and the Ethernet link will be passive after finishing the sleep handshaking.
 *
 * Use SDK API to get the following indication flags during sleep handshake:
 * - Step 1: Enable sleep capability on all ports and check if all ports are in local NORMAL mode.
 * - Step 2: Send sleep request on port1 and port3. The link partner of port1 is configured to accept sleep requests, while the link partner of port3 is not.
 * - Step 3: The link partner sends a sleep request on port2.
 * - Step 4: Retrieve sleep indication flags:
 *   - Receive sleep request indication
 *   - Sleep fail indication
 *   - Sleep success indication
 *
 * @image html opfsm_tc10/rx_sleep_success_indication.png
 * @image html opfsm_tc10/sleep_fail_indication.png
 */
RtkApiRet cvb_tc10_send_sleep_req(uint32 unit)
{
    RtkApiRet               rtkRet                = RT_ERR_OK;
    UnitChip_t              unitChip              = {.chip = CHIP_907XD_VA, .unit = unit};
    OpSlpCap_t              opSlpCap              = {0u};
    OpPortSendSlpReq_t      opPortSendSlpReq      = {0u};
    PhyResetInfo_t          phyResetInfo          = {0u};
    OpRxSlpFlagGet_t        opRxSlpFlagGet        = {0u};
    OpSlpFailFlagGet_t      opSlpFailFlagGet      = {0u};
    OpRxSlpSuccessFlagGet_t opRxSlpSuccessFlagGet = {0u};

    do
    {
        /* Pre-config. reset switch */
        CHK_FUN_CONTINUE(rtkRet, rtk_reset_chip_rst(unitChip));

        /* Step 1: enable all port sleep capability and check if all port in local NORMAL mode */
        opSlpCap.sleepCap = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_get(unitChip, &opSlpCap));
        if(0u != opSlpCap.portMap)
        {
            opSlpCap.sleepCap = ENABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_set(unitChip, &opSlpCap));
            CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_get(unitChip, &opSlpCap));
            /* Do PHY reset to activate PHY sleep capability */
            phyResetInfo.portMap = opSlpCap.portMap;
            CHK_FUN_CONTINUE(rtkRet, rtk_phy_bmcr_reset(unitChip, &phyResetInfo));
        }
        /* Expected Behavior: All TC10-compatible port enable sleep capability. */

        /* Step 2: send sleep request on port1, port3.
         * The link partner of port1 is configured to accept sleep request.
         * The link partner of port3 is configured to not accept sleep request.
         */
        opPortSendSlpReq.portMap = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD);
        CHK_FUN_CONTINUE(rtkRet, rtk_op_port_send_sleep_request(unitChip, &opPortSendSlpReq));
        /* Expected Behavior: The port1, port2, port3 send sleep request. */

        /* Step 3: The link partner sends sleep request on port2 */

        /* Step 4: If link partner of port1 and port2 are configured to accept sleep request, bit port3 is not */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_rx_sleep_flag_get(unitChip, &opRxSlpFlagGet));
        CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_fail_flag_get(unitChip, &opSlpFailFlagGet));
        CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_success_flag_get(unitChip, &opRxSlpSuccessFlagGet));
        /* Expected Behavior:
         * opRxSlpFlagGet.rxSlpFlag will return port map involves port2
         * opSlpFailFlagGet.slpFailFlag will return port map involves port3
         * opRxSlpSuccessFlagGet.slpSuccessFlag will return port map involves port1 and port2.
         */
    } while(0u == 1u);

    return rtkRet;
}

/**
 * @brief <b>Indicate sleep events for the switch</b>
 *
 * @details
 * This example get indications of sleep events, including receiving sleep requests, sleep handshake failures, and entering sleep mode.
 *
 * @section Purpose
 * - Indicate sleep events such as receiving sleep requests, sleep handshake failures, and entering sleep mode.
 * - Enable external interrupts for these sleep events.
 *
 * @section Description
 * This function retrieves and clears the flags for various sleep events and enables external interrupts for these events.
 *
 * 1. Get the per-port flag indicating the reception of a sleep request.
 * 2. Get the per-port flag indicating an unsuccessful sleep request.
 * 3. Get the per-port flag indicating the port has finished the sleep handshake and entered sleep mode.
 * 4. Enable external interrupts for sleep events, including receiving sleep requests, sleep handshake failures, and entering sleep mode.
 */
RtkApiRet cvb_tc10_sleep_event_indication(uint32 unit)
{
    RtkApiRet               rtkRet                = RT_ERR_OK;
    UnitChip_t              unitChip              = {.chip = CHIP_907XD_VA, .unit = unit};
    OpRxSlpFlagGet_t        opRxSlpFlagGet        = {0u};
    OpSlpFailFlagGet_t      opSlpFailFlagGet      = {0u};
    OpRxSlpSuccessFlagGet_t opRxSlpSuccessFlagGet = {0u};
    OpSlpEventIntrEn_t      opSlpEventIntrEn      = {0u};

    do
    {
        /* Step 1: Get the per-port flag indicating the reception of a sleep request. */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_rx_sleep_flag_get(unitChip, &opRxSlpFlagGet));
        /* Expected behavior: The opRxSlpFlagGet.rxSlpFlag contains the ports that have received a sleep request.
         * The opRxSlpFlagGet.rxSlpFlag is read and cleared.
         */

        /* Step 2: Get the per-port flag indicating the port has an unsuccessful sleep request. */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_fail_flag_get(unitChip, &opSlpFailFlagGet));
        /* Expected behavior: The opSlpFailFlagGet.slpFailFlag contains the ports that initiated an unsuccessful sleep request.
         * The opSlpFailFlagGet.slpFailFlag is read and cleared.
         */

        /* Step 3: Get the per-port flag indicating the ports that have entered sleep mode after completing a successful sleep handshake. */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_success_flag_get(unitChip, &opRxSlpSuccessFlagGet));
        /* Expected behavior: The opRxSlpSuccessFlagGet.slpSuccessFlag contains the ports that have entered sleep mode after completing a successful sleep handshake.
         * The opRxSlpSuccessFlagGet.slpSuccessFlag is read and cleared.
         */

        /* Step 4: Enable external interrupts for sleep events, including:
         * 1. Receiving a sleep request
         * 2. Sleep handshake failure
         * 3. Entering sleep mode
         */
        opSlpEventIntrEn.extIntrOn = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_event_intr_en_set(unitChip, &opSlpEventIntrEn));
        opSlpEventIntrEn.extIntrOn = 0u;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_event_intr_en_get(unitChip, &opSlpEventIntrEn));
        /* Expected behavior: Trigger external interrupts for sleep events. */

    } while(0u == 1u);

    return rtkRet;
}
/**@}*/ /* RTL907XC_VB_SLEEP_REQ_AND_INDICATION */

/** @defgroup RTL907XC_VB_WAKE_REQ_AND_INDICATION Initialize Wakeup Request and Get Wake Indications Example
 *  @brief <b>Initialize Wakeup Request and Get Wake Indications Example</b>
 *
 *  @section Purpose
 *  Providing example of initializing wakeup request and get wake indications
 *
 *  @section Description
 *  This example provides a example to command a port to initialize a wakeup to its counterpart and get wake event indications.
 *  @{
 */
/**
 * @brief Wake-up Request and Wake Indication
 *
 * @details
 * This function handles the wake-up request and wake indication process. It enables or disables wakeup request detection capability and wakeup request transmit capability, retrieves wakeup indication flags, clears wakeup flags, and sends remote wake-up requests.
 *
 * @section Purpose
 * - Enable or disable wakeup request detection capability and wakeup request transmit capability.
 * - Get the wakeup indication flag of WUP and WUR if any wakeup request is received.
 * - Use wakeup flag clear API to clear all wakeup flags. It is required to read-clear the wakeup flag before sending a wakeup request.
 * - Send remote wake-up request.
 *
 * @section Description
 * 1. Enable or disable wakeup request detection capability and wakeup request transmit capability.
 * 2. Get the wakeup indication flag of WUP and WUR if any wakeup request is received.
 * 3. Use wakeup flag clear API to clear all wakeup flags. It is required to read-clear the wakeup flag before sending a wakeup request.
 * 4. Send remote wake-up request.
 */
RtkApiRet cvb_tc10_wake_req_and_indication(uint32 unit)
{
    RtkApiRet          rtkRet           = RT_ERR_OK;
    UnitChip_t         unitChip         = {.chip = CHIP_907XD_VA, .unit = unit};
    OpEnWuprTx_t       opEnWuprTx       = {0u};
    OpSlpCap_t         opSlpCap         = {0u};
    OpPmapLocalState_t opPmapLocalState = {0u};
    OpDisRwake_t       opDisRwake       = {0u};
    OpPortSendSlpReq_t opPortSendSlpReq = {0u};
    OpPortSendRwake_t  opPortSendRwake  = {0u};
    OpWupFlagGet_t     opWupFlagGet     = {0u};
    OpWurFlagGet_t     opWurFlagGet     = {0u};

    do
    {
        /* Pre-config. reset switch */
        CHK_FUN_CONTINUE(rtkRet, rtk_reset_chip_rst(unitChip));

        /* Step 1: Clear all wakeup flag after switch reboot. */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wakeflag_clearall(unitChip));

        /* Step 2: Enable all port wakeup detection and transmit capability */
        opEnWuprTx.cfgValue = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_en_wupr_tx_get(unitChip, &opEnWuprTx));
        if(0u != opEnWuprTx.portMap)
        {
            opEnWuprTx.cfgValue = ENABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_en_wupr_tx_get(unitChip, &opEnWuprTx));
        }
        opDisRwake.cfgValue = 1u;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_disable_rwake_get(unitChip, &opDisRwake));
        if(0u != opDisRwake.portMap)
        {
            opDisRwake.cfgValue = 0u;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_disable_rwake_set(unitChip, &opDisRwake));
        }
        /* Expected Behavior: All port enable wakeup request detection and transmitting */

        /* Step 3: Send sleep request on all TC10 compatible ports.
         * note. if the PHY role of the link partner is master and the link is passive,
         * the link partner will try to do link training and the training pattern will be treat as WUP.
         * So let the PHY role of the link partner configured as slave to avoid receive unexpected wakeup request.
         */
        opSlpCap.sleepCap = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_phy_sleep_cap_get(unitChip, &opSlpCap));
        /* send sleep request on all ports enabled sleep capability */
        opPortSendSlpReq.portMap = opSlpCap.portMap;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_port_send_sleep_request(unitChip, &opPortSendSlpReq));
        opPmapLocalState.state = OP_LOCAL_SLEEP;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_portmap_local_state_get(unitChip, &opPmapLocalState));
        if(opPortSendSlpReq.portMap != opPmapLocalState.portMap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }
        /* Expected Behavior: all TC10-compatible ports are in local sleep mode */

        /* Step 4: Command the port1 and port2 going to send WUP transition to local STANDBY mode.
         * Because only the link partner in sleep mode is waken-up by WUP, not WUR, so port1 and port2 should be in STANDBY mode to send WUP.
         */
        opPmapLocalState.portMap = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND);
        opPmapLocalState.state   = OP_LOCAL_STANDBY;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_portmap_local_state_set(unitChip, &opPmapLocalState));
        opPmapLocalState.portMap = 0u;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_portmap_local_state_get(unitChip, &opPmapLocalState));
        if((CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND) != opPmapLocalState.portMap)
        {
            rtkRet = -RT_ERR_PORT_MASK;
            continue;
        }
        /* Expected Behavior: port1 and port2 are in local STANDBY mode */

        /* Step 5: Let port1 and port2 send a wakeup request.
         * Ports 1 and 2 are in STANDBY mode and have the capability to send WUP.
         */
        opPortSendRwake.portMap = opPmapLocalState.portMap;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_port_send_rwake(unitChip, &opPortSendRwake));
        /* Expected Behavior: The link partner of port1 and port2 will be waken-up by WUP */

        /* Step 6: Command the port1 and port2 go to local sleep mode and waken-up by the WUP from the link-partner */
        opPmapLocalState.portMap = opPortSendRwake.portMap;
        opPmapLocalState.state   = OP_LOCAL_SLEEP;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_portmap_local_state_set(unitChip, &opPmapLocalState));
        /* Expected Behavior: The port1 and port2 are in local sleep mode */
        /* Make the link partner of port1 and port2 send WUP, and check the WUP flag of port1 and port2 */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wup_flag_get(unitChip, &opWupFlagGet));
        /* Expected Behavior: The WUP flags indicates port1 and port2 have received WUP */
        /* Make the link partner of port1 and port2 send WUR, and check the WUR flag of port1 and port2 */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wur_flag_get(unitChip, &opWurFlagGet));
        /* Expected Behavior: The WUR flags indicates port1 and port2 have received WUR */
    } while(0u == 1u);

    return rtkRet;
}
/**@}*/ /* RTL907XC_VB_WAKE_REQ_AND_INDICATION */

/** @defgroup RTL907XC_VB_SLEEP_WAKE_INTERRUPT_CFG Sleep and Wakeup Interrupt Configuration Example
 *  @brief <b>Sleep and Wakeup Interrupt Configuration Example</b>
 *
 *  @section Purpose
 *  Providing example of sleep and wakeup interrupt configuration example
 *
 *  @section Description
 *  This example provides a example to configure the sleep and wakeup interrupt mask.
 *  @{
 */
/**
 * @brief Configure TC10 Interrupts
 *
 * @details
 * This function configures the sleep and wakeup interrupts for TC10-compatible ports. It sets the wakeup request interrupt mask, OPFSM event interrupt mask, and enables or disables the sleep event interrupt.
 *
 * @section Purpose
 * - Set the wakeup request interrupt mask for TC10-compatible ports.
 * - Set the OPFSM event interrupt mask.
 * - Enable or disable the sleep event interrupt.
 *
 * @section Description
 * 1. Set the wakeup request interrupt mask for specific ports.
 * 2. Set the wakeup request interrupt mask for other ports.
 * 3. Set the wakeup request interrupt mask for all ports.
 * 4. Set the wakeup request interrupt mask for other ports.
 * 5. Set the OPFSM event interrupt mask.
 */
RtkApiRet cvb_tc10_interrupt_cfg(uint32 unit)
{
    RtkApiRet           rtkRet            = RT_ERR_OK;
    UnitChip_t          unitChip          = {.chip = CHIP_907XD_VA, .unit = unit};
    OpWurExtIntrMsk_t   opWurExtIntrMsk   = {0u};
    OpWupExtIntrMsk_t   opWupExtIntrMsk   = {0u};
    OpSlpEventIntrEn_t  opSlpEventIntrEn  = {0u};
    OpEventExtIntrMsk_t opEventExtIntrMsk = {0u};
    RtkPmap             tc10Pmap          = 0u;
    RtkPmap             pmapCfg           = 0u;
    uint16              eventMskCfg       = (uint16)(OP_REC_RG_RST_INTR_MSK | OP_REC_PIN_RST_INTR_MSK | OP_PWON_INTR_MSK | OP_LWAKE_INTR_MSK);

    do
    {
        /* Pre-config. get the ports support sleep function according to the chip defines */
#ifdef CVB_OP_RTPGE_USER_PORTMAP
        tc10Pmap |= CVB_OP_RTPGE_USER_PORTMAP;
#endif
#ifdef CVB_OP_BWPHY_USER_PORTMAP
        tc10Pmap |= CVB_OP_BWPHY_USER_PORTMAP;
#endif

        /* Step 1: Set port1 and port2 the WUP Interrupt mask.
         * Receiving WUP on port1 and port2 will trigger external interrupt.
         */
        pmapCfg                  = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND);
        opWupExtIntrMsk.cfgValue = 0;
        opWupExtIntrMsk.portMap  = pmapCfg;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wup_ext_intr_msk_set(unitChip, &opWupExtIntrMsk));
        /* Expected Behavior: Receiving WUP on port1 and port2 will trigger external interrupt. */
        opWupExtIntrMsk.cfgValue = 1;
        opWupExtIntrMsk.portMap  = tc10Pmap ^ pmapCfg;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wup_ext_intr_msk_set(unitChip, &opWupExtIntrMsk));
        /* Expected Behavior: Receiving WUP on ports excluding port1 and port2 will not trigger external interrupt. */
        opWupExtIntrMsk.cfgValue = 0;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wup_ext_intr_msk_get(unitChip, &opWupExtIntrMsk));
        /* Expected Behavior: returns opWupExtIntrMsk.portMap involving port1 and port2 */

        /* Step 2: Set port3 and port4 the WUR Interrupt mask.
         * Receiving WUR on port3 and port4 will trigger external interrupt.
         */
        pmapCfg                  = (CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_4TH);
        opWurExtIntrMsk.cfgValue = 0;
        opWurExtIntrMsk.portMap  = pmapCfg;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wur_ext_intr_msk_set(unitChip, &opWurExtIntrMsk));
        /* Expected Behavior: Receiving WUR on port3 and port4 will trigger external interrupt. */
        opWurExtIntrMsk.cfgValue = 1;
        opWurExtIntrMsk.portMap  = tc10Pmap ^ pmapCfg;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wur_ext_intr_msk_set(unitChip, &opWurExtIntrMsk));
        /* Expected Behavior: Receiving WUR on ports excluding port3 and port4 will not trigger external interrupt. */
        opWurExtIntrMsk.cfgValue = 0;
        CHK_FUN_CONTINUE(rtkRet, rtk_op_wur_ext_intr_msk_get(unitChip, &opWurExtIntrMsk));
        /* Expected Behavior: returns opWurExtIntrMsk.portMap involving port3 and port4 */

        /* Step 3: Enable sleep event trigger interrupt. */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_event_intr_en_get(unitChip, &opSlpEventIntrEn));
        if(DISABLE == opSlpEventIntrEn.extIntrOn)
        {
            opSlpEventIntrEn.extIntrOn = ENABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_sleep_event_intr_en_set(unitChip, &opSlpEventIntrEn));
        }
        /* Expected Behavior: all sleep events including RX sleep/Sleep Fail/Sleep Success will trigger external interrupt */

        /* Step 4: Set OPFSM Interrupt mask.
         * (OP_REC_RG_RST_INTR_MSK | OP_REC_PIN_RST_INTR_MSK | OP_PWON_INTR_MSK | OP_LWAKE_INTR_MSK)
         */
        CHK_FUN_CONTINUE(rtkRet, rtk_op_event_ext_intr_msk_get(unitChip, &opEventExtIntrMsk));
        if(eventMskCfg != opEventExtIntrMsk.cfgValue)
        {
            opEventExtIntrMsk.cfgValue = eventMskCfg;
            CHK_FUN_CONTINUE(rtkRet, rtk_op_event_ext_intr_msk_set(unitChip, &opEventExtIntrMsk));
        }
        /* Expected Behavior: following events will not trigger external interrupt:
         * 1. switch reset by register
         * 2. switch reset by HW reset PIN
         * 3. switch VBAT power on
         * 4. local wakeup event
         */
    } while(0u == 1u);

    return rtkRet;
}
/**@}*/ /* RTL907XC_VB_SLEEP_WAKE_INTERRUPT_CFG */

/**@}*/ /* RTL907XC_VB_SLEEP_WAKE_SERVICE */
/**@}*/ /* RTL907XC_VB_SLEEP_WAKE */
/**@}*/ /* TC10_OPFSM_EXAMPLE */
/**@}*/ /* EXAMPLE */
