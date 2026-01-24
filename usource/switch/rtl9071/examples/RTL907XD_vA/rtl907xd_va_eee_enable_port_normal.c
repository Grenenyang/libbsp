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

/** @defgroup EEE_EXAMPLE EEE Example
 *  @{
 */

/** @defgroup RTL907XD_VA_EEE_EXAMPLE RTL907XD_VA EEE Example
 *  @{
 */

/** @defgroup EEE_NORMAL_FUNC Enable TX Queue Empty LPI Request Decision Maker and Normal Mode Wake Decision Maker Example
 *  @brief <b>Enable TX Queue Empty LPI Request Decision Maker and Normal Mode Wake decision Maker Example</b>
 *
 *  @section Purpose
 *  Providing example of TX Queue Empty LPI Request Decision Maker and Normal Mode Wake decision Maker.
 *
 *  @section Description
 *  This example provides configuration of a port enabling TX Queue Empty LPI Request Decision Maker and Normal Mode Wake decision Maker.
 *  @{
 */

/* Function */
/* Declaration */
RtkApiRet dva_eee_enable_port_lpi_txq_empty(UnitChip_t unitChip, RtkPort userPortIndex);

/**
 * @brief Enable Port EEE Function with Enabling TX Queue Empty LPI Request Decision Maker and Normal Mode Wake Decision Maker
 *
 * @details
 * This example uses SDK API to configure the specified port to enable TX Queue Empty LPI Request Decision Maker and Normal Mode Wake Decision Maker.
 *
 * @section Purpose
 * Providing configuration example of enabling the EEE function for a port with TX queue empty LPI request decision maker and normal mode wake decision maker.
 *
 * @section Description
 * This example provides a sample configuration related to enabling the EEE function of the user addressed port with interface type 1000BASE-T1.
 * The EEE configuration of this example code is as follows:
 * - Configure the 1000M timer unit to 1us.
 * - Configure the 1000M timer. Set T_wakeup to 12(us).
 * - Enable the LPI TX request decision maker of addressed port.
 * - Enable LPI TX high priority packet Wakeup decision maker of addressed port.
 *
 * After enabling the TX queue empty LPI request decision maker, addressed port will enter LPI when all TX queues are empty.
 * If the TX queue is not empty, addressed port will leave LPI mode.
 */
RtkApiRet dva_eee_enable_port_lpi_txq_empty(UnitChip_t unitChip, RtkPort userPortIndex)
{
    RtkApiRet                   rtkRet     = RT_ERR_OK;
    RtkPort                     userPortId = userPortIndex;
    EeeEnableCfg_t              eeeEnableCfg;
    EeePerPortEnCfg_t           eeePerPortEnCfg;
    EeeTimerUnitCfg_t           eeeTimerUnitCfg;
    EeeTimerValueCfg_t          eeeTimerValueCfg;
    EeeLpiRegDecisionMakerCfg_t eeeLpiRegDecisionMakerCfg;
    EeeWakeDecisionMakerCfg_t   eeeWakeDecisionMakerCfg;

    (void)drv_memset(&eeeEnableCfg, 0, sizeof(eeeEnableCfg));
    (void)drv_memset(&eeePerPortEnCfg, 0, sizeof(eeePerPortEnCfg));
    (void)drv_memset(&eeeTimerUnitCfg, 0, sizeof(eeeTimerUnitCfg));
    (void)drv_memset(&eeeTimerValueCfg, 0, sizeof(eeeTimerValueCfg));
    (void)drv_memset(&eeeLpiRegDecisionMakerCfg, 0, sizeof(eeeLpiRegDecisionMakerCfg));
    (void)drv_memset(&eeeWakeDecisionMakerCfg, 0, sizeof(eeeWakeDecisionMakerCfg));

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
        /* Set T_wakeup to 12us for interface type 1000BASE-T1 */
        eeeTimerValueCfg.eeeTimerVal.tw = 12u;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_timer_value(unitChip, &eeeTimerValueCfg));
        /* Expected Behavior: The 1000M timer is well configured */

        /* Step 5: Enable LPI TX request decision maker of port 1 */
        eeeLpiRegDecisionMakerCfg.port           = userPortId;
        eeeLpiRegDecisionMakerCfg.txRate         = DISABLE;
        eeeLpiRegDecisionMakerCfg.queueEmpty     = ENABLE;
        eeeLpiRegDecisionMakerCfg.rxPauseOnFrame = DISABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_lpi_req(unitChip, &eeeLpiRegDecisionMakerCfg));
        /* Expected Behavior: The addressed port enabled LPI TX request queue empty decision maker for EEE. */

        /* Step 6: Enable the Wake decision maker of port 1 */
        eeeWakeDecisionMakerCfg.port    = userPortId;
        eeeWakeDecisionMakerCfg.wakeSet = EM_EEE_WAKE_NORMAL_MODE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_wake(unitChip, &eeeWakeDecisionMakerCfg));
        /* Expected Behavior: Enable LPI TX transmit wakeup decision maker for addressed port */

        /* Step 7: Enabled the T1 PHY EEE function of the addressed port */
        eeePerPortEnCfg.enPortEee = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_port_en(unitChip, &eeePerPortEnCfg));
        /* Expected Behavior: T1 PHY EEE function of the addressed port is enabled. */

        /* Step 8: Configure EEE MAC control register to enable EEE function for all port  */
        eeeEnableCfg.cfg = ENABLE;
        CHK_FUN_CONTINUE(rtkRet, rtk_eee_set_en(unitChip, &eeeEnableCfg));
        /* Expected Behavior: EEE function of all port is enabled. */
    } while(0u == 1u);

    return rtkRet;
}

/**@}*/ /* EEE_NORMAL_FUNC */
/**@}*/ /* RTL907XD_VA_EEE_EXAMPLE */
/**@}*/ /* EEE_EXAMPLE */
/**@}*/ /* EXAMPLE */