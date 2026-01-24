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
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xd_va_api_eee.h"
#include "rtkas_api_eee.h"

/**
 * @defgroup RTK_MODULE_EEE EEE
 * @brief EEE APIs. The EEE Configuration APIs allow users to manage Energy Efficient Ethernet settings on the switch. It supports decision-making for Low Power Idle mode activation and deactivation, as well as retrieving EEE status.
 *
 * @{
 */

/**
 * @defgroup EEE_LPI_ASSERT LPI Assert Functions
 * @brief Including enable or disable the global EEE function, per-port EEE function, and EEE MAC timer setting.
 * @{
 */

/**
 * @brief Disable EEE function for all ports. This configuration has higher priority than port TX or RX EEE ability. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                  Stacking unit and chip ID (N/A) \n
 *                                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeEnableCfgSet The parameters to disable the EEE function of all ports (N/A). \n
 *                                      Including cfg.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_en().
 */
RtkApiRet rtk_eee_set_en(UnitChip_t unitChip, EeeEnableCfg_t *pEeeEnableCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == pEeeEnableCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_en(unitChip.unit, pEeeEnableCfgSet->cfg);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}

/**
 * @brief Get configuration of disabling EEE function for all ports. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip               Stacking unit and chip ID (N/A) \n
 *                                   The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                   Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *peeeEnableCfg The parameters to disable the EEE function of all ports (N/A). \n
 *                                   Including cfg.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_en().
 */
RtkApiRet rtk_eee_get_en(UnitChip_t unitChip, EeeEnableCfg_t *pEeeEnableCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeEnableCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_en(unitChip.unit, &(pEeeEnableCfgGet->cfg));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set configuration of enable EEE RX and enable EEE TX function of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                     Stacking unit and chip ID (N/A) \n
 *                                         The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                         Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeePerPortRxTxEnCfgSet The parameters for enabling EEE RX function and EEE TX function of the addressed port (N/A). \n
 *                                         Including port, and TX and RX enable configuration.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_port_en().
 */
RtkApiRet rtk_eee_set_port_en(UnitChip_t unitChip, EeePerPortEnCfg_t *pEeePerPortRxTxEnCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == pEeePerPortRxTxEnCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_port_en(unitChip.unit, pEeePerPortRxTxEnCfgSet->port, pEeePerPortRxTxEnCfgSet->enPortEee);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}

/**
 * @brief Get configuration of enable EEE RX and enable EEE TX function of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                     Stacking unit and chip ID (N/A) \n
 *                                         The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                         Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeePerPortRxTxEnCfgGet The parameters of getting the EEE RX function enable setting and EEE TX function enable setting of the addressed port (N/A). \n
 *                                         Including port, and TX and RX enable configuration.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_port_en().
 */
RtkApiRet rtk_eee_get_port_en(UnitChip_t unitChip, EeePerPortEnCfg_t *pEeePerPortRxTxEnCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeePerPortRxTxEnCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_port_en(unitChip.unit, pEeePerPortRxTxEnCfgGet->port, &(pEeePerPortRxTxEnCfgGet->enPortEee));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set the unit for timer T_tx_rate and T_wakeup in EEE function. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                 Stacking unit and chip ID (N/A) \n
 *                                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeTimerUnitCfgSet The parameters to configure EEE timer unit (N/A). \n
 *                                     Including type, timerUnit.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_timer_unit().
 */
RtkApiRet rtk_eee_set_timer_unit(UnitChip_t unitChip, EeeTimerUnitCfg_t *pEeeTimerUnitCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeTimerUnitCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_timer_unit(unitChip.unit, pEeeTimerUnitCfgSet->type, pEeeTimerUnitCfgSet->timerUnit);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the unit for timer T_tx_rate and T_wakeup in EEE function. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                 Stacking unit and chip ID (N/A) \n
 *                                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeTimerUnitCfgGet The parameters to configure EEE timer unit (N/A). \n
 *                                     Including type, timerUnit.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_timer_unit().
 */
RtkApiRet rtk_eee_get_timer_unit(UnitChip_t unitChip, EeeTimerUnitCfg_t *pEeeTimerUnitCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeTimerUnitCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_timer_unit(unitChip.unit, pEeeTimerUnitCfgGet->type, &(pEeeTimerUnitCfgGet->timerUnit));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set the timer value of T_pause, T_tx_delay, T_tx_rate, T_wakeup in EEE function. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                  Stacking unit and chip ID (N/A) \n
 *                                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeTimerValueCfgSet The parameters to configure EEE timer value (N/A). \n
 *                                      Including type, tp, td, tr, tw.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_timer_value().
 */
RtkApiRet rtk_eee_set_timer_value(UnitChip_t unitChip, EeeTimerValueCfg_t *pEeeTimerValueCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeTimerValueCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_timer_value(unitChip.unit, pEeeTimerValueCfgSet->type, pEeeTimerValueCfgSet->eeeTimerVal);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the timer value of T_pause, T_tx_delay, T_tx_rate, T_wakeup in EEE function. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                  Stacking unit and chip ID (N/A) \n
 *                                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeTimerValueCfgGet The parameters to get EEE timer value configurations (N/A). \n
 *                                      Including type, tp, td, tr, tw.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_timer_value().
 */
RtkApiRet rtk_eee_get_timer_value(UnitChip_t unitChip, EeeTimerValueCfg_t *pEeeTimerValueCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeTimerValueCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_timer_value(unitChip.unit, pEeeTimerValueCfgGet->type, &(pEeeTimerValueCfgGet->eeeTimerVal));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* EEE_LPI_ASSERT */

/**
 * @defgroup EEE_LPI_DETECT LPI Detect Function
 * @brief Get the EEE function status indication of MAC port and T1-PHY.
 * @{
 */

/**
 * @brief Get EEE function status of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                  Stacking unit and chip ID (N/A) \n
 *                                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeePerPortEeeStsGet The parameters of the EEE function status of the addressed port (N/A). \n
 *                                      Including port, rxPauseOnFrame, rxStatus, txStatus and eeeStatus.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_port_func_sts().
 */
RtkApiRet rtk_eee_get_port_func_sts(UnitChip_t unitChip, EeePerPortStatus_t *pEeePerPortEeeStsGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeePerPortEeeStsGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_port_func_sts(unitChip.unit, pEeePerPortEeeStsGet->port, &(pEeePerPortEeeStsGet->eeeStatus));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get current T1-PHY TX LPI Indication and RX LPI Indication. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeePerPhyStsGet The parameters to return the T1-PHY EEE LPI TX/RX indication of the designated port (N/A). \n
 *                                  Including port and pEeePerPhyStsGet.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_phy_func_sts().
 */
RtkApiRet rtk_eee_get_phy_func_sts(UnitChip_t unitChip, EeePerPhyStatus_t *pEeePerPhyStsGet)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == pEeePerPhyStsGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_phy_func_sts(unitChip.unit, pEeePerPhyStsGet->port, &(pEeePerPhyStsGet->phyLpiStatus));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}
/**@}*/ /* EEE_LPI_DETECT */

/**
 * @defgroup EEE_TX_REQUEST EEE TX Request Decision Maker Configurations
 * @brief Configure MAC decision maker for determining the timing of entering the LPI state.
 * @{
 */

/**
 * @brief Set the port configuration of decision maker for LPI Entry. \n
 *        This functionality allows user to configure the decision-making process for requesting entering the Low-Power Idle (LPI) state. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                           Stacking unit and chip ID (N/A) \n
 *                                               The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                               Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeLpiRegDecisionMakerCfgSet The parameters to configure LPI decision maker configurations (N/A). \n
 *                                               Including port, txRate, queueEmpty, rxPauseOnFrame.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_port_lpi_req().
 */
RtkApiRet rtk_eee_set_port_lpi_req(UnitChip_t unitChip, EeeLpiRegDecisionMakerCfg_t *pEeeLpiRegDecisionMakerCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeLpiRegDecisionMakerCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_port_lpi_req(unitChip.unit, pEeeLpiRegDecisionMakerCfgSet->port, pEeeLpiRegDecisionMakerCfgSet->txRate, pEeeLpiRegDecisionMakerCfgSet->queueEmpty, pEeeLpiRegDecisionMakerCfgSet->rxPauseOnFrame);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the port configuration of decision maker for LPI Entry. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                           Stacking unit and chip ID (N/A) \n
 *                                               The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                               Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeLpiRegDecisionMakerCfgGet The parameters of the current LPI decision maker configurations of the designated port (N/A). \n
 *                                               Including port, txRate, queueEmpty, rxPauseOnFrame.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_port_lpi_req().
 */
RtkApiRet rtk_eee_get_port_lpi_req(UnitChip_t unitChip, EeeLpiRegDecisionMakerCfg_t *pEeeLpiRegDecisionMakerCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeLpiRegDecisionMakerCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_port_lpi_req(unitChip.unit, pEeeLpiRegDecisionMakerCfgGet->port, &(pEeeLpiRegDecisionMakerCfgGet->txRate), &(pEeeLpiRegDecisionMakerCfgGet->queueEmpty), &(pEeeLpiRegDecisionMakerCfgGet->rxPauseOnFrame));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set the threshold for all Tx queue packets in the DSC unit of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                 Stacking unit and chip ID (N/A) \n
 *                                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeLpiQtxThrCfgSet The parameters of the TX queue threshold (0 to 0xFF). \n
 *                                     Configure the TX Queue Threshold for TX request rate LPI decision maker. \n
 *                                     Including port, qTxThr.
 * @param[out] None
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_port_lpi_qtx_thr().
 */
RtkApiRet rtk_eee_set_port_lpi_qtx_thr(UnitChip_t unitChip, EeeQueueTxThrCfg_t *pEeeLpiQtxThrCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeLpiQtxThrCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_port_lpi_qtx_thr(unitChip.unit, pEeeLpiQtxThrCfgSet->port, pEeeLpiQtxThrCfgSet->qTxThr);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the threshold for all Tx queue packets in the DSC unit of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                 Stacking unit and chip ID (N/A) \n
 *                                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeLpiQtxThrCfgGet The parameters of the TX queue threshold (0 to 0xFF). (N/A). \n
 *                                     Including port, qTxThr.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_port_lpi_qtx_thr().
 */
RtkApiRet rtk_eee_get_port_lpi_qtx_thr(UnitChip_t unitChip, EeeQueueTxThrCfg_t *pEeeLpiQtxThrCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeLpiQtxThrCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_port_lpi_qtx_thr(unitChip.unit, pEeeLpiQtxThrCfgGet->port, &(pEeeLpiQtxThrCfgGet->qTxThr));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* EEE_TX_REQUEST */

/**
 * @defgroup EEE_TX_WAKE EEE TX Wake Decision Maker Configurations
 * @brief Configure the wake decision maker for determining the timing for exiting the LPI state.
 * @{
 */
/**
 * @brief Set the high priority TX queue configurations for QoS mode wake decision maker of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                      Stacking unit and chip ID (N/A) \n
 *                                          The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeWakeTxQueuePriCfgSet The parameters of the current TX queue priority configuration of the designated port (N/A). \n
 *                                          Including port, queuePriCfg.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_port_qtx_pri().
 */
RtkApiRet rtk_eee_set_port_qtx_pri(UnitChip_t unitChip, EeeTxQueuePriCfg_t *pEeeWakeTxQueuePriCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeWakeTxQueuePriCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_port_qtx_pri(unitChip.unit, pEeeWakeTxQueuePriCfgSet->port, pEeeWakeTxQueuePriCfgSet->queuePriCfg);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the high priority TX queue configurations for QoS mode wake decision maker of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                      Stacking unit and chip ID (N/A) \n
 *                                          The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeWakeTxQueuePriCfgSet The parameters of the current TX queue priority configuration of the designated port (N/A). \n
 *                                          Including port, queuePriCfg.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_port_lpi_qtx_thr().
 */
RtkApiRet rtk_eee_get_port_qtx_pri(UnitChip_t unitChip, EeeTxQueuePriCfg_t *pEeeWakeTxQueuePriCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeWakeTxQueuePriCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_port_qtx_pri(unitChip.unit, pEeeWakeTxQueuePriCfgGet->port, &(pEeeWakeTxQueuePriCfgGet->queuePriCfg));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set the wake decision maker configurations of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                         Stacking unit and chip ID (N/A) \n
 *                                             The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeWakeDecisionMakerCfgSet The parameters of the current wake decision maker configurations of the designated port (N/A). \n
 *                                             Including port, wakeSet.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_set_port_wake().
 */
RtkApiRet rtk_eee_set_port_wake(UnitChip_t unitChip, EeeWakeDecisionMakerCfg_t *pEeeWakeDecisionMakerCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEeeWakeDecisionMakerCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_set_port_wake(unitChip.unit, pEeeWakeDecisionMakerCfgSet->port, pEeeWakeDecisionMakerCfgSet->wakeSet);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the wake decision maker configurations of the designated port. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip                         Stacking unit and chip ID (N/A) \n
 *                                             The stacking unit ID indicates which SWITCH is being accessed.\n
 *                                             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEeeWakeDecisionMakerCfgGet The parameters of the current wake decision maker configurations of the designated port (N/A). \n
 *                                             Including port, wakeSet.
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the address chip is not supported.
 * @retval Others Please refer to dva_eee_get_port_wake().
 */
RtkApiRet rtk_eee_get_port_wake(UnitChip_t unitChip, EeeWakeDecisionMakerCfg_t *pEeeWakeDecisionMakerCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == pEeeWakeDecisionMakerCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eee_get_port_wake(unitChip.unit, pEeeWakeDecisionMakerCfgGet->port, &(pEeeWakeDecisionMakerCfgGet->wakeSet));
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* EEE_TX_WAKE */

/**@}*/ /* EEE */
