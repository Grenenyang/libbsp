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

#include "rtl907xd_va_api_ext.h"
#include "rtkas_api_eee.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup EEE_EXAMPLE EEE Example
 * @{
 */

/**
 * @addtogroup RTL907XD_VA_EEE_EXAMPLE RTL907XD_VA EEE Example
 * @{
 */

/** @defgroup EEE_QOS_CONFIa Enable TX Rate LPI Decision Maker and QoS Mode Wake Decision Maker Example
 *  @brief <b>Enable TX Rate LPI Decision Maker and QoS Mode Wake Decision Maker Example</b>
 *
 *  @section Purpose
 *  Providing example of enabling TX Rate LPI Decision Maker and QoS Mode Wake Decision Maker of the addressed port.
 *
 *  @section Description
 *  This example provides configuration of a port enabling TX Rate LPI Decision Maker and QoS Mode Wake Decision Maker.
 *  @{
 */

/* Function */
/* Declaration */
RtkApiRet dva_eee_enable_port_lpi_txrate_wake_qosmode(UnitChip_t unitChip, RtkPort userPortIndex);

/**
 * @brief Enable Port EEE Function with Enabling TX Rate LPI Request Decision Maker and QoS Mode Wake Decision Maker
 *
 * @details
 * This example uses SDK API to configure the specified port to enabling TX Rate LPI Request Decision Maker and QoS Mode Wake Decision Maker.
 *
 * @section Purpose
 * Providing configuration example of enabling the EEE function for a port with TX rate LPI request decision maker and QoS mode wake decision maker.
 *
 * @section Description
 * This example provides a sample configuration related to enabling the EEE function of the user addressed port with interface type 1000BASE-T1.
 * The EEE configuration of this example code is as follows:
 * - Configure the 1000M timer unit to 1us.
 * - Configure the 1000M timer. Set T_tx_rate to 10(us). T_pause timer to 2(us). T_tx_delay to 20(us). T_wakeup to 12(us).
 * - Enable LPI TX request rate decision maker of the addressed port.
 * - Set the TX threshold value to 0x5, unit is 128 bytes.
 * - Configure high priority queues for the addressed port.
 * - Enable QoS mode LPI wake decision maker for the addressed port.
 *
 * After enabling the TX rate LPI request decision maker, if the packet bytes in the TX queue do not exceed the TX threshold (5 * 128 bytes) after the T_tx_rate timer expires (10us),
 * the MAC will request the PHY to enter LPI mode.
 * If the PHY receives a pause frame from the link partner, the PHY enters LPI mode.
 *
 * If there is a control packet or a high-priority packet to transmit, the MAC will wake the PHY immediately.
 * For the packets in the low-priority queue, those packets first accumulate in TX queues. The PHY will be woken from LPI when the T_tx_delay timer expires or the packets in DSC unit (128 bytes) reach a certain threshold (255).
 * After the PHY wakes up, the accumulated packets will be transmitted in burst mode.
 */
RtkApiRet dva_eee_enable_port_lpi_txrate_wake_qosmode(UnitChip_t unitChip, RtkPort userPortIndex)
{
    RtkApiRet                   rtkRet     = RT_ERR_OK;
    RtkPort                     userPortId = userPortIndex;
    EeeEnableCfg_t              eeeEnableCfg;
    EeePerPortEnCfg_t           eeePerPortEnCfg;
    EeeTimerUnitCfg_t           eeeTimerUnitCfg;
    EeeTimerValueCfg_t          eeeTimerValueCfg;
    EeeLpiRegDecisionMakerCfg_t eeeLpiRegDecisionMakerCfg;
    EeeQueueTxThrCfg_t          eeeQueueTxThrCfg;
    EeeWakeDecisionMakerCfg_t   eeeWakeDecisionMakerCfg;
    EeeTxQueuePriCfg_t          eeeTxQueuePriCfg;

    (void)drv_memset(&eeeEnableCfg, 0, sizeof(eeeEnableCfg));
    (void)drv_memset(&eeePerPortEnCfg, 0, sizeof(eeePerPortEnCfg));
    (void)drv_memset(&eeeTimerUnitCfg, 0, sizeof(eeeTimerUnitCfg));
    (void)drv_memset(&eeeTimerValueCfg, 0, sizeof(eeeTimerValueCfg));
    (void)drv_memset(&eeeLpiRegDecisionMakerCfg, 0, sizeof(eeeLpiRegDecisionMakerCfg));
    (void)drv_memset(&eeeQueueTxThrCfg, 0, sizeof(eeeQueueTxThrCfg));
    (void)drv_memset(&eeeWakeDecisionMakerCfg, 0, sizeof(eeeWakeDecisionMakerCfg));
    (void)drv_memset(&eeeTxQueuePriCfg, 0, sizeof(eeeTxQueuePriCfg));

    do
    {
        /* Step 1: Configure EEE MAC control register to disable EEE function for all port  */
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_get_en(unitChip, &eeeEnableCfg));
        /* If all port EEE function is enabled, disable all port EEE function */
        if(ENABLE == eeeEnableCfg.cfg)
        {
            eeeEnableCfg.cfg = DISABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_en(unitChip, &eeeEnableCfg));
        }
        /* Expected Behavior: EEE function of all port is disabled. */

        /* Step 2: Disabled the EEE function of the addressed port */
        eeePerPortEnCfg.port = userPortId;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_get_port_en(unitChip, &eeePerPortEnCfg));
        if(ENABLED == eeePerPortEnCfg.enPortEee)
        {
            eeePerPortEnCfg.enPortEee = DISABLE;
            CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_en(unitChip, &eeePerPortEnCfg));
        }
        /* Expected Behavior: EEE function of the addressed port is disabled. */

        /* Step 3: Configure the timer unit of timer type: 1000M */
        eeeTimerUnitCfg.type = EM_EEE_TIMER_1000M;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_get_timer_unit(unitChip, &eeeTimerUnitCfg));
        if(EM_EEE_TIMER_UNIT_1US != eeeTimerUnitCfg.timerUnit)
        {
            eeeTimerUnitCfg.timerUnit = EM_EEE_TIMER_UNIT_1US;
            CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_timer_unit(unitChip, &eeeTimerUnitCfg));
        }
        /* Expected Behavior: The timer unit configuration is 1us */

        /* Step 4: Configure the timer value of timer type: 1000M */
        eeeTimerValueCfg.type = EM_EEE_TIMER_1000M;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_get_timer_value(unitChip, &eeeTimerValueCfg));
        /* Set T_tx_rate to 10us of 1000M */
        eeeTimerValueCfg.eeeTimerVal.tr = 10u;
        /* Set T_wakeup to 12us for interface type 1000BASE-T1 */
        eeeTimerValueCfg.eeeTimerVal.tw = 12u;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_timer_value(unitChip, &eeeTimerValueCfg));
        /* Expected Behavior: The 1000M timer is well configured */

        /* Step 5: Enable LPI TX request Rate decision maker of port 1 */
        eeeLpiRegDecisionMakerCfg.port           = userPortId;
        eeeLpiRegDecisionMakerCfg.txRate         = ENABLE;
        eeeLpiRegDecisionMakerCfg.queueEmpty     = DISABLE;
        eeeLpiRegDecisionMakerCfg.rxPauseOnFrame = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_lpi_req(unitChip, &eeeLpiRegDecisionMakerCfg));
        /* Expected Behavior: The addressed port enabled LPI TX rate decision maker */

        /* Step 6: Set the TX threshold value to 0x5 */
        eeeQueueTxThrCfg.port   = userPortId;
        eeeQueueTxThrCfg.qTxThr = 0x5u;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_lpi_qtx_thr(unitChip, &eeeQueueTxThrCfg));
        /* Expected Behavior: configure the addressed port TX rate threshold value to 5 */

        /* Step 7: configure high priority queue of addressed port */
        eeeTxQueuePriCfg.port        = userPortId;
        eeeTxQueuePriCfg.queuePriCfg = 0u;
        eeeTxQueuePriCfg.queuePriCfg |= (1u << 7u);
        eeeTxQueuePriCfg.queuePriCfg |= (1u << 5u);
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_qtx_pri(unitChip, &eeeTxQueuePriCfg));
        /* Expected Behavior: configure TX queue 7 and queue 5 as high priority queue of addressed port */

        /* Step 8: Enable addressed port QoS mode LPI Wake decision maker */
        eeeWakeDecisionMakerCfg.port    = userPortId;
        eeeWakeDecisionMakerCfg.wakeSet = EM_EEE_WAKE_QOS_MODE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_wake(unitChip, &eeeWakeDecisionMakerCfg));
        /* Expected Behavior: Enable LPI TX high priority packet Wakeup decision maker for EEE of addressed port */

        /* Step 9: Enabled the EEE function of the addressed port */
        eeePerPortEnCfg.enPortEee = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_en(unitChip, &eeePerPortEnCfg));
        /* Expected Behavior: EEE function of the addressed port is enabled. */

        /* Step 10: Configure EEE MAC control register to enable EEE function for all port  */
        eeeEnableCfg.cfg = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_en(unitChip, &eeeEnableCfg));
        /* Expected Behavior: EEE function of all port is enabled. */
    } while(0u == 1u);

    return rtkRet;
}
/**@}*/ /* EEE_QOS_CONFIG */
/**@}*/ /* RTL907XD_VA_EEE_EXAMPLE */
/**@}*/ /* EEE_EXAMPLE */
/**@}*/ /* EXAMPLE */