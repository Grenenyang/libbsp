/*
 * Copyright (c) 2024 Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * This software component is confidential and proprietary to Realtek
 * Semiconductor Corp. Disclosure, reproduction, redistribution, in whole
 * or in part, of this work and its derivatives without express permission
 * is prohibited.
 */

#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_opfsm.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_opfsm.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_api_opfsm.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_api_opfsm.h"
#endif

#include "rtkas_api_opfsm.h"

/**
 * @defgroup RTK_MODULE_OPFSM Sleep and Wakeup
 * @brief TC10 APIs. The TC10 APIs provides a comprehensive set of functionalities to manage port sleep and wake configurations,
 *  monitor related events, and operate the OP-FSM (Operational Finite State Machine) function.
 * @{
 */

/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_SLPCAP Sleep capability function
 * @brief Sleep capability function.
 * @{
 */

/**
 * @brief Get the port map with the designated PHY sleep capability.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpSlpCapGet Pointer to a struct that holds parameters to get the port map with the designated sleep capability  (N/A) \n
 *                             The struct includes portMap and sleepCap.\n
 *                             - sleepCap = ENABLE: Return portMap with sleep capability enabled.
 *                             - sleepCap = DISABLE: Return portMap with sleep capability disabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_phy_sleep_cap(), dva_op_get_phy_sleep_cap(), cvb_op_get_phy_sleep_cap(), and cva_op_get_phy_sleep_cap().
 */
RtkApiRet rtk_op_phy_sleep_cap_get(UnitChip_t unitChip, OpSlpCap_t *pOpSlpCapGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpSlpCapGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_phy_sleep_cap(unitChip.unit, pOpSlpCapGet->sleepCap, &(pOpSlpCapGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_phy_sleep_cap(unitChip.unit, pOpSlpCapGet->sleepCap, &(pOpSlpCapGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_phy_sleep_cap(unitChip.unit, pOpSlpCapGet->sleepCap, &(pOpSlpCapGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_phy_sleep_cap(unitChip.unit, pOpSlpCapGet->sleepCap, &(pOpSlpCapGet->portMap));
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
 * @brief Set the designated PHY sleep capability for the ports specified by the input port map.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpSlpCapSet Pointer to a struct that holds parameters to set the designated ports to the designated sleep capability  (N/A) \n
 *                         The struct includes portMap and sleepCap.\n
 *                         - sleepCap = ENABLE: Enable the sleep capability of ports in the portMap.
 *                         - sleepCap = DISABLE: Disable the sleep capability of ports in the portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_phy_sleep_cap(), dva_op_set_phy_sleep_cap(), cvb_op_set_phy_sleep_cap(), and cva_op_set_phy_sleep_cap().
 */
RtkApiRet rtk_op_phy_sleep_cap_set(UnitChip_t unitChip, OpSlpCap_t *pOpSlpCapSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpSlpCapSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_phy_sleep_cap(unitChip.unit, pOpSlpCapSet->portMap, pOpSlpCapSet->sleepCap);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_phy_sleep_cap(unitChip.unit, pOpSlpCapSet->portMap, pOpSlpCapSet->sleepCap);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_phy_sleep_cap(unitChip.unit, pOpSlpCapSet->portMap, pOpSlpCapSet->sleepCap);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_phy_sleep_cap(unitChip.unit, pOpSlpCapSet->portMap, pOpSlpCapSet->sleepCap);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_SLPCAP */

/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_ACCEPT_SLP_CAP The accept sleep request function configuration
 * @brief Configuration of accepting sleep request when the link partner initialize a sleep handshake.
 * @{
 */

/**
 * @brief Get the SWITCH firmware accept sleep request port map (AcceptSleepCmdMsk) configuration indicating ports that accept received sleep requests from the link partner.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpAcceptSlpReqCapGet Pointer to a struct that holds parameters to get the chip accept sleep request configuration.\n
 *                                      The struct includes portMap and cfgValue.
 *                                      - cfgValue = ENABLE: Return portMap indicating ports with accept sleep request enabled.
 *                                      - cfgValue = DISABLE:: Return portMap indicating ports with accept sleep request disabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_accept_sleep_req_cap(), dva_op_get_accept_sleep_req_cap(), cvb_op_get_accept_sleep_req_cap(), and cva_op_get_accept_sleep_req_cap().
 */
RtkApiRet rtk_op_accept_sleep_req_cap_get(UnitChip_t unitChip, OpAcceptSlpReqCap_t *pOpAcceptSlpReqCapGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpAcceptSlpReqCapGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapGet->cfgValue, &(pOpAcceptSlpReqCapGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapGet->cfgValue, &(pOpAcceptSlpReqCapGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapGet->cfgValue, &(pOpAcceptSlpReqCapGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapGet->cfgValue, &(pOpAcceptSlpReqCapGet->portMap));
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
 * @brief Set the addressed ports to the SWITCH firmware accept sleep request port map (AcceptSleepCmdMsk) configuration to have the permission to accept received sleep requests.\n
 *        By default, all ports are not allowed to accept received sleep requests, and all ports will reject sleep requests from the link partner.\n
 *        To accept a sleep request, first ensure the port link status is active and the port's sleep capability is enabled.
 *        After a port is enabled to accept sleep capability, it can perform a remote sleep handshake with the link partner after receiving a sleep request from the link partner,
 *        and the port's local OPFSM state will change to OP_LOCAL_SLEEP after the sleep handshake is successfully completed.\n
 *        The accept sleep capability configuration only takes effect on the receive side.
 *        For example, if a port is not configured to accept received sleep requests and the sleep capability is enabled, this port will reject the sleep request from the link partner,\n
 *        but it can still send sleep requests to the link partner and eventually enter local sleep mode.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpAcceptSlpReqCapSet Pointer to a struct that holds parameters to set the chip accept sleep request configuration.\n
 *                                  The struct includes portMap and cfgValue.
 *                                  - cfgValue = ENABLE: Enable the accept sleep capability of ports designated in the portMap.
 *                                  - cfgValue = DISABLE:: Disable the accept sleep capability of ports designated in the portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_accept_sleep_req_cap(), dva_op_set_accept_sleep_req_cap(), cvb_op_set_accept_sleep_req_cap(), and cva_op_set_accept_sleep_req_cap().
 */
RtkApiRet rtk_op_accept_sleep_req_cap_set(UnitChip_t unitChip, OpAcceptSlpReqCap_t *pOpAcceptSlpReqCapSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpAcceptSlpReqCapSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapSet->portMap, pOpAcceptSlpReqCapSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapSet->portMap, pOpAcceptSlpReqCapSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapSet->portMap, pOpAcceptSlpReqCapSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_accept_sleep_req_cap(unitChip.unit, pOpAcceptSlpReqCapSet->portMap, pOpAcceptSlpReqCapSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_ACCEPT_SLP_CAP */

/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_SLP_FWD The Sleep forwarding function configuration
 * @brief Configuration of port forwarding the incoming sleep request to another port.
 * @{
 */

/**
 * @brief Get the SWITCH firmware transmitting sleep request port map (TXSleepPortMsk) configuration.\n
 *        The transmitting sleep request port map (TXSleepPortMsk) shows the ports that will transmit sleep requests when another port receives a sleep request.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA\n
 *        For example, if bit1 and bit2 in the transmit sleep request port map are set:\n
 *        - When port1 receives a sleep request, port2 will transmit the sleep request to its link partner.
 *        - When port3 receives a sleep request, port1 and port2 will transmit the sleep request to their link partners.
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      The stacking unit ID indicates which SWITCH is being accessed.\n
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpTxSleepMskGet Pointer to a struct that holds parameters to get the chip's sleep forwarding configuration.\n
 *                                 The struct includes portMap and cfgValue.
 *                                 - cfgValue = ENABLE: Return portMap indicating ports with sleep request forwarding enabled.
 *                                 - cfgValue = DISABLE:: Return portMap indicating ports with sleep request forwarding disabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_txsleep_msk(), dva_op_txsleep_msk(), cvb_op_get_txsleep_portmap(), and cva_op_get_txsleep_portmap().
 */
RtkApiRet rtk_op_txsleep_portmap_get(UnitChip_t unitChip, OpTxSleepMsk_t *pOpTxSleepMskGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpTxSleepMskGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_txsleep_portmap(unitChip.unit, pOpTxSleepMskGet->cfgValue, &(pOpTxSleepMskGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_txsleep_portmap(unitChip.unit, pOpTxSleepMskGet->cfgValue, &(pOpTxSleepMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_txsleep_portmap(unitChip.unit, pOpTxSleepMskGet->cfgValue, &(pOpTxSleepMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_txsleep_portmap(unitChip.unit, pOpTxSleepMskGet->cfgValue, &(pOpTxSleepMskGet->portMap));
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
 * @brief Set designated ports to the SWITCH transmitting sleep request port map (TXSleepPortMsk) to initiate a sleep request to their link partner when any other port on the SWITCH receives a sleep request.\n
 *        This API only affects the port's capability of transmitting a sleep request.\n
 *        Regardless of whether the port's capability to accept sleep requests is on or off, the ports in the TXSleepPortMsk will transmit sleep requests when any other port receives a sleep request.\n
 *        The port that receives the sleep request will not send the sleep request to its link partner again.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpTxSleepMskSet Pointer to a struct that holds parameters to set the chip's sleep forwarding configuration.\n
 *                             The struct includes portMap and cfgValue.
 *                             - cfgValue = ENABLE: Enable the sleep request forwarding of ports designated in the portMap.
 *                             - cfgValue = DISABLE:: Disable the sleep request forwarding of ports designated in the portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_txsleep_portmap(), dva_op_set_txsleep_portmap(), cvb_op_set_txsleep_portmap(), and cva_op_set_txsleep_portmap().
 */
RtkApiRet rtk_op_txsleep_portmap_set(UnitChip_t unitChip, OpTxSleepMsk_t *pOpTxSleepMskSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpTxSleepMskSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_txsleep_portmap(unitChip.unit, pOpTxSleepMskSet->portMap, pOpTxSleepMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_txsleep_portmap(unitChip.unit, pOpTxSleepMskSet->portMap, pOpTxSleepMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_txsleep_portmap(unitChip.unit, pOpTxSleepMskSet->portMap, pOpTxSleepMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_txsleep_portmap(unitChip.unit, pOpTxSleepMskSet->portMap, pOpTxSleepMskSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**@}*/ /* OPFSM_SLEEP_PRECONFIG_SLP_FWD */

/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_WHOLECHIP_SLP The Configuration related to Whole Chip Sleep Mode
 * @brief Configuration to toggle the whole-chip sleep function and the condition to enter whole-chip sleep mode.
 * @{
 */

/**
 * @brief Get the SWITCH firmware whole-chip sleep enable configuration (WholeChipSleepEnable).\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpWholechipSlpEnGet Pointer to a struct that holds parameters to get the chip's whole chip sleep enabling configuration.\n
 *                                  The struct includes wholeChipOn.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_en_wholechip_sleep(), dva_op_get_en_wholechip_sleep(), cvb_op_get_en_wholechip_sleep(), and cva_op_get_en_wholechip_sleep().
 */
RtkApiRet rtk_op_wholechip_sleep_en_get(UnitChip_t unitChip, OpWholechipSlpEn_t *pOpWholechipSlpEnGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWholechipSlpEnGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_en_wholechip_sleep(unitChip.unit, &(pOpWholechipSlpEnGet->wholeChipOn));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_en_wholechip_sleep(unitChip.unit, &(pOpWholechipSlpEnGet->wholeChipOn));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_en_wholechip_sleep(unitChip.unit, &(pOpWholechipSlpEnGet->wholeChipOn));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_en_wholechip_sleep(unitChip.unit, &(pOpWholechipSlpEnGet->wholeChipOn));
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
 * @brief Set the SWITCH firmware whole-chip sleep enable configuration (WholeChipSleepEnable) to allow or disallow the SWITCH whole chip sleep function.\n
 *        If the SWITCH is allowed to enter WHOLE_CHIP_SLEEP mode, when any port of SWITCH receives a sleep request and enters OP_LOCAL_SLEEP mode,\n
 *        the SWITCH will check if all the ports in the WholeChipSleepMask are in OP_LOCAL_SLEEP mode.\n
 *        If the SWITCH is allowed to enter WHOLE_CHIP_SLEEP mode and all the ports in the WholeChipSleepMask are in OP_LOCAL_SLEEP mode,\n
 *        the SWITCH will then enter WHOLE_CHIP_SLEEP mode.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpWholechipSlpEnSet Pointer to a struct that holds parameters to set the chip's whole chip sleep enabling configuration.\n
 *                                 The struct includes wholeChipOn.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_en_wholechip_sleep(), dva_op_set_en_wholechip_sleep(), cvb_op_set_en_wholechip_sleep(), and cva_op_set_en_wholechip_sleep().
 */
RtkApiRet rtk_op_wholechip_sleep_en_set(UnitChip_t unitChip, OpWholechipSlpEn_t *pOpWholechipSlpEnSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWholechipSlpEnSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_en_wholechip_sleep(unitChip.unit, pOpWholechipSlpEnSet->wholeChipOn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_en_wholechip_sleep(unitChip.unit, pOpWholechipSlpEnSet->wholeChipOn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_en_wholechip_sleep(unitChip.unit, pOpWholechipSlpEnSet->wholeChipOn);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_en_wholechip_sleep(unitChip.unit, pOpWholechipSlpEnSet->wholeChipOn);
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
 * @brief Get the SWITCH firmware whole-chip sleep port map (WholeChipSleepMask) which is a port map referred to by the whole chip sleep feature.\n
 *        The SWITCH will enter WHOLE_CHIP_SLEEP mode if the ports configured in the WholeChipSleepMask are all in OP_LOCAL_SLEEP mode.\n
 *        The default value of WholeChipSleepMask is 0, meaning no port is in the WholeChipSleepMask.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpWholechipSlpMskGet Pointer to a struct that holds parameters to get the chip's whole chip sleep port mask configuration.\n
 *                                   The struct includes portMap and cfgValue.
 *                                   - cfgValue = ENABLE: Return portMap indicating ports included in the whole-chip sleep port map.
 *                                   - cfgValue = DISABLE:: Return portMap indicating ports excluded from the whole-chip sleep port map.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_wholechip_sleep_portmap(), dva_op_get_wholechip_sleep_portmap(), cvb_op_get_wholechip_sleep_portmap(), and cva_op_get_wholechip_sleep_portmap().
 */
RtkApiRet rtk_op_get_wholechip_sleep_portmap(UnitChip_t unitChip, OpWholechipSlpMsk_t *pOpWholechipSlpMskGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWholechipSlpMskGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskGet->cfgValue, &(pOpWholechipSlpMskGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskGet->cfgValue, &(pOpWholechipSlpMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskGet->cfgValue, &(pOpWholechipSlpMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskGet->cfgValue, &(pOpWholechipSlpMskGet->portMap));
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
 * @brief Set the SWITCH firmware whole-chip sleep port map (WholeChipSleepMask), which is the port mask referred to by the whole chip sleep feature.\n
 *        The SWITCH will enter WHOLE_CHIP_SLEEP mode if all the ports in the WholeChipSleepMask are in OP_LOCAL_SLEEP mode.\n
 *        The default value of WholeChipSleepMask is 0, meaning no port is in the WholeChipSleepMask.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpWholechipSlpMskSet Pointer to a struct that holds parameters to set the chip's whole chip sleep port mask configuration.\n
 *                                  The struct includes portMap and cfgValue.
 *                                  - cfgValue = ENABLE: Add the ports designated in the portMap to the WholeChipSleepMask.
 *                                  - cfgValue = DISABLE:: Remove the ports designated in the portMap from the WholeChipSleepMask.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_wholechip_sleep_portmap(), dva_op_set_wholechip_sleep_portmap(), cvb_op_set_wholechip_sleep_portmap(), and cva_op_set_wholechip_sleep_portmap().
 */
RtkApiRet rtk_op_set_wholechip_sleep_portmap(UnitChip_t unitChip, OpWholechipSlpMsk_t *pOpWholechipSlpMskSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWholechipSlpMskSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskSet->portMap, pOpWholechipSlpMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskSet->portMap, pOpWholechipSlpMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskSet->portMap, pOpWholechipSlpMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_wholechip_sleep_portmap(unitChip.unit, pOpWholechipSlpMskSet->portMap, pOpWholechipSlpMskSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**@}*/ /* OPFSM_SLEEP_PRECONFIG_WHOLECHIP_SLP */

/**
 * @defgroup OPFSM_WAKE_PRECONFIG_LWAKE_EDGE Lwake edge detection selection function
 * @brief Functions to configure the wake input (local wakeup) setting.
 * @{
 */

/**
 * @brief Get the signal edge of LWAKE.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpLwakeEdgeSelectGet Pointer to a struct that holds parameters to get the LWAKE edge configuration.\n
 *                                   The struct includes lwakeEdge.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_lwake_edge_sel(), dva_op_get_lwake_edge_sel(), cvb_op_get_lwake_edge_sel(), and cva_op_get_lwake_edge_sel().
 */
RtkApiRet rtk_op_lwake_edge_sel_get(UnitChip_t unitChip, OpLwakeEdgeSelect_t *pOpLwakeEdgeSelectGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpLwakeEdgeSelectGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_lwake_edge_sel(unitChip.unit, &(pOpLwakeEdgeSelectGet->lwakeEdge));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_lwake_edge_sel(unitChip.unit, &(pOpLwakeEdgeSelectGet->lwakeEdge));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_lwake_edge_sel(unitChip.unit, &(pOpLwakeEdgeSelectGet->lwakeEdge));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_lwake_edge_sel(unitChip.unit, &(pOpLwakeEdgeSelectGet->lwakeEdge));
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
 * @brief Set the signal edge of LWAKE.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpLwakeEdgeSelectSet Pointer to a struct that holds parameters to set the LWAKE edge configuration.\n
 *                                  The struct includes lwakeEdge.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_lwake_edge_sel(), dva_op_set_lwake_edge_sel(), cvb_op_set_lwake_edge_sel(), and cva_op_set_lwake_edge_sel().
 */
RtkApiRet rtk_op_lwake_edge_sel_set(UnitChip_t unitChip, OpLwakeEdgeSelect_t *pOpLwakeEdgeSelectSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpLwakeEdgeSelectSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_lwake_edge_sel(unitChip.unit, pOpLwakeEdgeSelectSet->lwakeEdge);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_lwake_edge_sel(unitChip.unit, pOpLwakeEdgeSelectSet->lwakeEdge);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_lwake_edge_sel(unitChip.unit, pOpLwakeEdgeSelectSet->lwakeEdge);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_lwake_edge_sel(unitChip.unit, pOpLwakeEdgeSelectSet->lwakeEdge);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}

/**@}*/ /* OPFSM_WAKE_PRECONFIG_LWAKE_EDGE */

/**
 * @defgroup OPFSM_WAKE_PRECONFIG_LWAKE_SCOPE Lwake scope selection function
 * @brief Functions to determine whether the sleeping PHY should wake up in response to Lwake
 * @{
 */

/**
 * @brief Get the lwake_wake_port port map.\n
 *        If the RTL907x_D detects a local wakeup event, the ports in the lwake_wake_port port map will wake up after detecting an lwake event and might initiate a remote wakeup to its link partner.\n
 *        By default, all ports have the lwake_wake_port functionality enabled.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpLwakeWakePortGet Pointer to a struct that holds parameters to get the ports that will wake up triggered by LWAKE.\n
 *                                The struct includes portMap and cfgValue.
 *                                - cfgValue = ENABLE: Return portMap indicating ports that send remote wakeup when detecting a SWITCH local wakeup event.
 *                                - cfgValue = DISABLE:: Return portMap indicating ports that will not send remote wakeup when detecting a SWITCH local wakeup event.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_lwake_wake_port(), dva_op_get_lwake_wake_port(), cvb_op_get_lwake_wake_port(), and cva_op_get_lwake_wake_port().
 */
RtkApiRet rtk_op_lwake_wake_port_get(UnitChip_t unitChip, OpLwakeWakePort_t *pOpLwakeWakePortGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpLwakeWakePortGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_lwake_wake_port(unitChip.unit, pOpLwakeWakePortGet->cfgValue, &(pOpLwakeWakePortGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_lwake_wake_port(unitChip.unit, pOpLwakeWakePortGet->cfgValue, &(pOpLwakeWakePortGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_lwake_wake_port(unitChip.unit, pOpLwakeWakePortGet->cfgValue, &(pOpLwakeWakePortGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_lwake_wake_port(unitChip.unit, pOpLwakeWakePortGet->cfgValue, &(pOpLwakeWakePortGet->portMap));
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
 * @brief Set Specific ports can be configured to enable or disable wakeup from sleep mode upon detecting a local wakeup event.\n
 *        When the RTL907x_D detects a local wakeup event, the ports in the lwake_wake_port port map will wake up after detecting an lwake event and might initiate a remote wakeup to its link partner.\n
 *        By default, all ports have the lwake_wake_port functionality enabled.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpLwakeWakePortSet Pointer to a struct that holds parameters to set the ports that will wake up triggered by LWAKE.\n
 *                               The struct includes portMap and cfgValue.
 *                               - cfgValue = ENABLE: Enable ports in portMap will wake up from sleep mode upon detecting a SWITCH local wakeup event.
 *                               - cfgValue = DISABLE:: Disable ports in portMap will wake up from sleep mode upon detecting a SWITCH local wakeup event.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_lwake_wake_port(), dva_op_set_lwake_wake_port(), cvb_op_set_lwake_wake_port(), and cva_op_set_lwake_wake_port().
 */
RtkApiRet rtk_op_lwake_wake_port_set(UnitChip_t unitChip, OpLwakeWakePort_t *pOpLwakeWakePortSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpLwakeWakePortSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_lwake_wake_port(unitChip.unit, pOpLwakeWakePortSet->portMap, pOpLwakeWakePortSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_lwake_wake_port(unitChip.unit, pOpLwakeWakePortSet->portMap, pOpLwakeWakePortSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_lwake_wake_port(unitChip.unit, pOpLwakeWakePortSet->portMap, pOpLwakeWakePortSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_lwake_wake_port(unitChip.unit, pOpLwakeWakePortSet->portMap, pOpLwakeWakePortSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}
/**@}*/ /* OPFSM_WAKE_PRECONFIG_LWAKE_SCOPE */

/**
 * @defgroup OPFSM_WAKE_PRECONFIG_RWAKE_TX_CAP Rwake transmission capability function
 * @brief Functions to configure the rwake transmit capability of ports
 * @{
 */

/**
 * @brief Get the port map indicating the current port's capability of transmitting WUP/WUR remote wakeup.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpEnWuprTxGet Pointer to a struct that holds parameters to get the WUP transmit capability of all ports.\n
 *                            The struct includes portMap and cfgValue.
 *                            - cfgValue = ENABLE: Return portMap indicating ports with WUP and WUR transmit enabled.
 *                            - cfgValue = DISABLE:: Return portMap indicating ports with WUP and WUR transmit disabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_en_wupr_tx(), dva_op_get_en_wupr_tx(), cvb_op_get_en_wupr_tx(), and cva_op_get_en_wupr_tx().
 */
RtkApiRet rtk_op_en_wupr_tx_get(UnitChip_t unitChip, OpEnWuprTx_t *pOpEnWuprTxGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpEnWuprTxGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_en_wupr_tx(unitChip.unit, pOpEnWuprTxGet->cfgValue, &(pOpEnWuprTxGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_en_wupr_tx(unitChip.unit, pOpEnWuprTxGet->cfgValue, &(pOpEnWuprTxGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_en_wupr_tx(unitChip.unit, pOpEnWuprTxGet->cfgValue, &(pOpEnWuprTxGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_en_wupr_tx(unitChip.unit, pOpEnWuprTxGet->cfgValue, &(pOpEnWuprTxGet->portMap));
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
 * @brief Enable or disable the capability of specific ports to transmit WUP/WUR.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpEnWuprTxSet Pointer to a struct that holds parameters to set the WUP transmit capability of specific ports.\n
 *                           The struct includes portMap and cfgValue.
 *                           - cfgValue = ENABLE: Enable the WUP and WUR transmit of ports designated in the portMap.
 *                           - cfgValue = DISABLE:: Disable the WUP and WUR transmit of ports designated in the portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_en_wupr_tx(), dva_op_set_en_wupr_tx(), cvb_op_set_en_wupr_tx(), and cva_op_set_en_wupr_tx().
 */
RtkApiRet rtk_op_en_wupr_tx_set(UnitChip_t unitChip, OpEnWuprTx_t *pOpEnWuprTxSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpEnWuprTxSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_en_wupr_tx(unitChip.unit, pOpEnWuprTxSet->portMap, pOpEnWuprTxSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_en_wupr_tx(unitChip.unit, pOpEnWuprTxSet->portMap, pOpEnWuprTxSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_en_wupr_tx(unitChip.unit, pOpEnWuprTxSet->portMap, pOpEnWuprTxSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_en_wupr_tx(unitChip.unit, pOpEnWuprTxSet->portMap, pOpEnWuprTxSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_WAKE_PRECONFIG_RWAKE_TX_CAP */

/**
 * @defgroup OPFSM_WAKE_PRECONFIG_DIS_RWAKE_RX_DET Rwake detect function
 * @brief Functions to configure the rwake detection capability of ports
 * @{
 */

/**
 * @brief Get the port map indicating the current port's remote wakeup detection setting.\n
 *        By default, all ports allow remote wakeup detection, so get_dva_op_disable_rwake_rx will return 0.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpDisRwakeGet Pointer to a struct that holds parameters to get the ports with remote wakeup detection disabled.\n
 *                               The struct includes portMap and cfgValue.
 *                               - cfgValue = OP_DISABLE_RWAKE: Return portMap indicating ports with remote wakeup detection disabled.
 *                               - cfgValue = OP_ENABLE_RWAKE: Return portMap indicating ports with remote wakeup detection enabled.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_disable_rwake_rx(), dva_op_get_disable_rwake_rx(), cvb_op_get_disable_rwake_rx(), and cva_op_get_disable_rwake_rx().
 */
RtkApiRet rtk_op_disable_rwake_get(UnitChip_t unitChip, OpDisRwake_t *pOpDisRwakeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpDisRwakeGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_disable_rwake_rx(unitChip.unit, &(pOpDisRwakeGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_disable_rwake_rx(unitChip.unit, &(pOpDisRwakeGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_disable_rwake_rx(unitChip.unit, &(pOpDisRwakeGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_disable_rwake_rx(unitChip.unit, &(pOpDisRwakeGet->portMap));
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
 * @brief Disable the remote wakeup detection of selected ports.\n
 *        If a port disables remote wakeup detection, it cannot accept remote wakeup requests initialized by the link partner.\n
 *        By default, all ports allow remote wakeup detection.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpDisRwakeSet Pointer to a struct that holds parameters to disable the remote wakeup detection of specific ports.\n
 *                           The struct includes portMap and cfgValue.
 *                           - cfgValue = OP_DISABLE_RWAKE: Disable the remote wakeup detection of ports designated in the portMap.
 *                           - cfgValue = OP_ENABLE_RWAKE: Enable the remote wakeup detection of ports designated in the portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_disable_rwake_rx(), dva_op_set_disable_rwake_rx(), cvb_op_set_disable_rwake_rx(), and cva_op_set_disable_rwake_rx().
 */
RtkApiRet rtk_op_disable_rwake_set(UnitChip_t unitChip, OpDisRwake_t *pOpDisRwakeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpDisRwakeSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_disable_rwake_rx(unitChip.unit, pOpDisRwakeSet->portMap, pOpDisRwakeSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_disable_rwake_rx(unitChip.unit, pOpDisRwakeSet->portMap, pOpDisRwakeSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_disable_rwake_rx(unitChip.unit, pOpDisRwakeSet->portMap, pOpDisRwakeSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_disable_rwake_rx(unitChip.unit, pOpDisRwakeSet->portMap, pOpDisRwakeSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_WAKE_PRECONFIG_DIS_RWAKE_RX_DET */

/**
 * @defgroup OPFSM_WAKEUP_FWD_RWAKE_FORWARD Rwake forwarding function
 * @brief Functions to configure the rwake forwarding function
 * @{
 */

/**
 * @brief Get the port map indicating the current port's remote wakeup detection setting.\n
 *        By default, all ports allow remote wakeup detection, so get_dva_op_disable_rwake_rx will return 0.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpRwakeFwdPxCfgGet Pointer to a struct that holds parameters to get the remote wakeup forwarding configuration of the selected port.\n
 *                                    The struct includes portSel and portMap.\n
 *                                    The portSel indicates a specific port that will be triggered to send a remote wakeup once the ports indicated in the portMap have a WUP or WUR event.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_portmap_fwd_rwake_to_port(), dva_op_get_portmap_fwd_rwake_to_port(), cvb_op_get_portmap_fwd_rwake_to_port(), and cva_op_get_portmap_fwd_rwake_to_port().
 */
RtkApiRet rtk_op_get_portmap_fwd_rwake_to_port(UnitChip_t unitChip, OpRwakeFwdPxCfgGet_t *pOpRwakeFwdPxCfgGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpRwakeFwdPxCfgGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgGet->portSel, &(pOpRwakeFwdPxCfgGet->portmap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgGet->portSel, &(pOpRwakeFwdPxCfgGet->portmap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgGet->portSel, &(pOpRwakeFwdPxCfgGet->portmap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgGet->portSel, &(pOpRwakeFwdPxCfgGet->portmap));
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
 * @brief Set the remote wake-up transmission capability of the selected port.\n
 *        When the ports in the specified port map (portMap) receive a remote wakeup,\n
 *        the selected port (portSel) will wakeup its link partner if the rwake tx capability is enabled.\n
 *        The default port map setting of a port is all zero.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpRwakeFwdPxCfgSet Pointer to a struct that holds parameters to set the remote wakeup forwarding configuration of the selected port.\n
 *                                The struct includes portSel, portMap, and cfgValue.\n
 *                                - cfgValue = ENABLE: Enable the remote wakeup behavior of the port indicated by portSel.\n
 *                                - cfgValue = DISABLE:: Disable the remote wakeup behavior of the port indicated by portSel.\n
 *                                If a port has remote wakeup behavior enabled, it will send a remote wakeup when the ports addressed in the portMap receive a WUP or WUR.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_portmap_fwd_rwake_to_port(), dva_op_set_portmap_fwd_rwake_to_port(), cvb_op_set_portmap_fwd_rwake_to_port(), and cva_op_set_portmap_fwd_rwake_to_port().
 */
RtkApiRet rtk_op_set_portmap_fwd_rwake_to_port(UnitChip_t unitChip, OpRwakeFwdPxCfgSet_t *pOpRwakeFwdPxCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == pOpRwakeFwdPxCfgSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgSet->portSel, pOpRwakeFwdPxCfgSet->portMap, pOpRwakeFwdPxCfgSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgSet->portSel, pOpRwakeFwdPxCfgSet->portMap, pOpRwakeFwdPxCfgSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgSet->portSel, pOpRwakeFwdPxCfgSet->portMap, pOpRwakeFwdPxCfgSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_portmap_fwd_rwake_to_port(unitChip.unit, pOpRwakeFwdPxCfgSet->portSel, pOpRwakeFwdPxCfgSet->portMap, pOpRwakeFwdPxCfgSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}
/**@}*/ /* OPFSM_WAKEUP_FWD_RWAKE_FORWARD */

/**
 * @defgroup OPFSM_WAKEUP_FWD_WAKEO Wake-out forwarding function
 * @brief Functions to configure the wake-out forwarding function
 * @{
 */

/**
 * @brief Get the SWITCH firmware wake-out forwarding port map indicating the ports that will trigger wake-out forwarding on the specific GPIO pin upon receiving a WUP or WUR.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpWakeoPortMapGet Pointer to a struct that holds parameters to get the WakeO port mask configuration of the addressed config index.\n
 *                                   The struct includes configIdx, portMap, and cfgValue.
 *                                   - cfgValue = ENABLE: The portMap indicates ports in the addressed configIdx ENABLE the wake-output function. Once the ports receive WUP or WUR,
 *                                     the specific GPIO bound to configIdx will output a specific wake-output pulse.
 *                                   - cfgValue = DISABLE:: The portMap indicates ports in the addressed configIdx DISABLE the wake-output function. Once the ports receive WUP or WUR,
 *                                     the specific GPIO bound to configIdx will not output a wake-output pulse.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_wakeo_portmap(), dva_op_get_wakeo_portmap(), cvb_op_get_wakeo_portmap(), and cva_op_get_wakeo_portmap().
 */
RtkApiRet rtk_op_wakeo_portmap_get(UnitChip_t unitChip, OpWakeoPortMap_t *pOpWakeoPortMapGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWakeoPortMapGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_wakeo_portmap(unitChip.unit, pOpWakeoPortMapGet->configIdx, pOpWakeoPortMapGet->cfgValue, &(pOpWakeoPortMapGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_wakeo_portmap(unitChip.unit, pOpWakeoPortMapGet->configIdx, pOpWakeoPortMapGet->cfgValue, &(pOpWakeoPortMapGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_wakeo_portmap(unitChip.unit, pOpWakeoPortMapGet->configIdx, pOpWakeoPortMapGet->cfgValue, &(pOpWakeoPortMapGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_wakeo_portmap(unitChip.unit, pOpWakeoPortMapGet->configIdx, pOpWakeoPortMapGet->cfgValue, &(pOpWakeoPortMapGet->portMap));
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
 * @brief Set the SWITCH firmware wake-out forwarding port map indicating the ports that will trigger wake-out forwarding on the specific GPIO pin upon receiving a WUP or WUR.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpWakeoPortMapSet Pointer to a struct that holds parameters to set the WakeO port mask configuration of the addressed config index.\n
 *                               The struct includes configIdx, portMap, and cfgValue.
 *                               - cfgValue = ENABLE: Enable the configIdx wake-output function of ports indicated in the portMap. Once the ports receive WUP or WUR,
 *                                 the specific GPIO bound to configIdx will output a specific wake-output pulse.
 *                               - cfgValue = DISABLE:: Disable the configIdx wake-output function of ports indicated in the portMap. Once the ports receive WUP or WUR,
 *                                 the specific GPIO bound to configIdx will not output a wake-output pulse.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_wakeo_portmap(), dva_op_set_wakeo_portmap(), cvb_op_set_wakeo_portmap(), and cva_op_set_wakeo_portmap().
 */
RtkApiRet rtk_op_wakeo_portmap_set(UnitChip_t unitChip, OpWakeoPortMap_t *pOpWakeoPortMapSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWakeoPortMapSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_wakeo_portmap(unitChip.unit, pOpWakeoPortMapSet->configIdx, pOpWakeoPortMapSet->portMap, pOpWakeoPortMapSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_wakeo_portmap(unitChip.unit, pOpWakeoPortMapSet->configIdx, pOpWakeoPortMapSet->portMap, pOpWakeoPortMapSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_wakeo_portmap(unitChip.unit, pOpWakeoPortMapSet->configIdx, pOpWakeoPortMapSet->portMap, pOpWakeoPortMapSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_wakeo_portmap(unitChip.unit, pOpWakeoPortMapSet->configIdx, pOpWakeoPortMapSet->portMap, pOpWakeoPortMapSet->cfgValue);
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
 * @brief Get the SWITCH firmware wake-out forwarding pulse width setting on the selected GPIO.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpWakeoPulseGet Pointer to a struct that holds parameters to get the WakeO pulse configuration.\n
 *                              The struct includes pulseCfg.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_wakeo_pulse(), dva_op_get_wakeo_pulse(), cvb_op_get_wakeo_pulse(), and cva_op_get_wakeo_pulse().
 */
RtkApiRet rtk_op_wakeo_pulse_get(UnitChip_t unitChip, OpWakeoPulse_t *pOpWakeoPulseGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWakeoPulseGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_wakeo_pulse(unitChip.unit, &(pOpWakeoPulseGet->pulseCfg));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_wakeo_pulse(unitChip.unit, &(pOpWakeoPulseGet->pulseCfg));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_wakeo_pulse(unitChip.unit, &(pOpWakeoPulseGet->pulseCfg));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_wakeo_pulse(unitChip.unit, &(pOpWakeoPulseGet->pulseCfg));
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
 * @brief Set the SWITCH firmware wake-out forwarding pulse width setting on the selected GPIO.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpWakeoPulseSet Pointer to a struct that holds parameters to set the WakeO pulse configuration.\n
 *                             The struct includes pulseCfg.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_wakeo_pulse(), dva_op_set_wakeo_pulse(), cvb_op_set_wakeo_pulse(), and cva_op_set_wakeo_pulse().
 */
RtkApiRet rtk_op_wakeo_pulse_set(UnitChip_t unitChip, OpWakeoPulse_t *pOpWakeoPulseSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWakeoPulseSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_wakeo_pulse(unitChip.unit, pOpWakeoPulseSet->pulseCfg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_wakeo_pulse(unitChip.unit, pOpWakeoPulseSet->pulseCfg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_wakeo_pulse(unitChip.unit, pOpWakeoPulseSet->pulseCfg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_wakeo_pulse(unitChip.unit, pOpWakeoPulseSet->pulseCfg);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}
/**@}*/ /* OPFSM_WAKEUP_FWD_WAKEO */

/**
 * @defgroup OPFSM_OP_STATE_GLOBAL Global state function
 * @brief Set and get global OPFSM state
 * @{
 */

/**
 * @brief Get the global OPFSM state.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpGlobStateGet Pointer to a struct that holds parameters to get the chip global OP state.\n
 *                             The struct includes the state.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_global_state(), dva_op_get_global_state(), cvb_op_get_global_state(), and cva_op_get_global_state().
 */
RtkApiRet rtk_op_global_state_get(UnitChip_t unitChip, OpGlobState_t *pOpGlobStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpGlobStateGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_global_state(unitChip.unit, &(pOpGlobStateGet->state));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_global_state(unitChip.unit, &(pOpGlobStateGet->state));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_global_state(unitChip.unit, &(pOpGlobStateGet->state));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_global_state(unitChip.unit, &(pOpGlobStateGet->state));
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
 * @brief Set the global OPFSM state.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpGlobStateSet Pointer to a struct that holds parameters to set the chip global OP state.\n
 *                            The struct includes the state.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_global_state(), dva_op_set_global_state(), cvb_op_set_global_state(), and cva_op_set_global_state().
 */
RtkApiRet rtk_op_global_state_set(UnitChip_t unitChip, OpGlobState_t *pOpGlobStateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpGlobStateSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_global_state(unitChip.unit, pOpGlobStateSet->state);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_global_state(unitChip.unit, pOpGlobStateSet->state);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_global_state(unitChip.unit, pOpGlobStateSet->state);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_global_state(unitChip.unit, pOpGlobStateSet->state);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_OP_STATE_GLOBAL */

/**
 * @defgroup OPFSM_OP_STATE_LOCAL Local state function
 * @brief Set and get local OPFSM state
 * @{
 */
/**
 * @brief Get the local OPFSM state of the selected port.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpPortLocalStateGet Pointer to a struct that holds parameters to get a port's local OP state.\n
 *                                     The struct includes the port and state.\n
 *                                     The output state value is the local OPFSM state of the designated port.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_port_local_state(), dva_op_get_port_local_state(), cvb_op_get_port_local_state(), and cva_op_get_port_local_state().
 */
RtkApiRet rtk_op_port_local_state_get(UnitChip_t unitChip, OpPortLocalState_t *pOpPortLocalStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpPortLocalStateGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_port_local_state(unitChip.unit, pOpPortLocalStateGet->port, &(pOpPortLocalStateGet->state));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_port_local_state(unitChip.unit, pOpPortLocalStateGet->port, &(pOpPortLocalStateGet->state));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_port_local_state(unitChip.unit, pOpPortLocalStateGet->port, &(pOpPortLocalStateGet->state));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_port_local_state(unitChip.unit, pOpPortLocalStateGet->port, &(pOpPortLocalStateGet->state));
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
 * @brief Set the local OPFSM state of the selected port.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpPortLocalStateSet Pointer to a struct that holds parameters to set a port's local OP state.\n
 *                                 The struct includes the port and state.\n
 *                                 The state value indicates the local OPFSM state to apply to the designated port.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_port_local_state(), dva_op_set_port_local_state(), cvb_op_set_port_local_state(), and cva_op_set_port_local_state().
 */
RtkApiRet rtk_op_port_local_state_set(UnitChip_t unitChip, OpPortLocalState_t *pOpPortLocalStateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpPortLocalStateSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_port_local_state(unitChip.unit, pOpPortLocalStateSet->port, pOpPortLocalStateSet->state);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_port_local_state(unitChip.unit, pOpPortLocalStateSet->port, pOpPortLocalStateSet->state);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_port_local_state(unitChip.unit, pOpPortLocalStateSet->port, pOpPortLocalStateSet->state);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_port_local_state(unitChip.unit, pOpPortLocalStateSet->port, pOpPortLocalStateSet->state);
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
 * @brief Get the ports with the designated local OPFSM state.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pOpPmapLocalStateGet Pointer to a struct that holds parameters to get the port map of the designated local OP state.\n
 *                                     The struct includes portMap and state.\n
 *                                     The output portMap indicates ports with the OPFSM local state addressed by the input state value.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_portmap_local_state(), dva_op_get_portmap_local_state(), cvb_op_get_portmap_local_state(), and cva_op_get_portmap_local_state().
 */
RtkApiRet rtk_op_portmap_local_state_get(UnitChip_t unitChip, OpPmapLocalState_t *pOpPmapLocalStateGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpPmapLocalStateGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_portmap_local_state(unitChip.unit, pOpPmapLocalStateGet->state, &(pOpPmapLocalStateGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_portmap_local_state(unitChip.unit, pOpPmapLocalStateGet->state, &(pOpPmapLocalStateGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_portmap_local_state(unitChip.unit, pOpPmapLocalStateGet->state, &(pOpPmapLocalStateGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_portmap_local_state(unitChip.unit, pOpPmapLocalStateGet->state, &(pOpPmapLocalStateGet->portMap));
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
 * @brief Set the local OPFSM state of the selected ports.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which SWITCH is being accessed.\n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpPmapLocalStateSet Pointer to a struct that holds parameters to set the local OP state of selected ports.\n
 *                                 The struct includes portMap and state.\n
 *                                 Set the local OPFSM state of ports in the input portMap to the input state.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dva_op_set_portmap_local_state(), dv1_op_set_portmap_local_state(), cvb_op_set_portmap_local_state(), and cva_op_set_portmap_local_state().
 */
RtkApiRet rtk_op_portmap_local_state_set(UnitChip_t unitChip, OpPmapLocalState_t *pOpPmapLocalStateSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpPmapLocalStateSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_portmap_local_state(unitChip.unit, pOpPmapLocalStateSet->portMap, pOpPmapLocalStateSet->state);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_portmap_local_state(unitChip.unit, pOpPmapLocalStateSet->portMap, pOpPmapLocalStateSet->state);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_portmap_local_state(unitChip.unit, pOpPmapLocalStateSet->portMap, pOpPmapLocalStateSet->state);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_portmap_local_state(unitChip.unit, pOpPmapLocalStateSet->portMap, pOpPmapLocalStateSet->state);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_OP_STATE_LOCAL */

/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_REQ Sleep request sending function
 * @brief  The sleep request sending function provides sleep.request
 * @{
 */

/**
 * @brief Trigger the selected ports to start a remote sleep handshake with the link partner.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpPortSendSlpReq Pointer to a struct that holds parameters to make some ports initiate a sleep request with their link partner.\n
 *                              The struct includes portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_port_send_sleep_request(), dva_op_port_send_sleep_request(), cvb_op_port_send_sleep_request(), and cva_op_port_send_sleep_request().
 */
RtkApiRet rtk_op_port_send_sleep_request(UnitChip_t unitChip, OpPortSendSlpReq_t *pOpPortSendSlpReq)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpPortSendSlpReq)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_port_send_sleep_request(unitChip.unit, pOpPortSendSlpReq->portMap);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_port_send_sleep_request(unitChip.unit, pOpPortSendSlpReq->portMap);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_port_send_sleep_request(unitChip.unit, pOpPortSendSlpReq->portMap);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_port_send_sleep_request(unitChip.unit, pOpPortSendSlpReq->portMap);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_REQ */

/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_EVENT_STATUS Sleep event status function
 * @brief  The functions to return the receive sleep event, sleep.indication, sleepFail.indicate
 * @{
 */

/**
 * @brief Get the SWITCH firmware receiving sleep request indication flag.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpRxSlpFlagGet Pointer to a struct that holds parameters to get the flag of ports receiving sleep request events.\n
 *                             The struct includes rxSlpFlag.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_rx_sleep_flag(), dva_op_get_rx_sleep_flag(), cvb_op_get_rx_sleep_flag(), and cva_op_get_rx_sleep_flag().
 */
RtkApiRet rtk_op_rx_sleep_flag_get(UnitChip_t unitChip, OpRxSlpFlagGet_t *pOpRxSlpFlagGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpRxSlpFlagGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_rx_sleep_flag(unitChip.unit, &(pOpRxSlpFlagGet->rxSlpFlag));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_rx_sleep_flag(unitChip.unit, &(pOpRxSlpFlagGet->rxSlpFlag));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_rx_sleep_flag(unitChip.unit, &(pOpRxSlpFlagGet->rxSlpFlag));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_rx_sleep_flag(unitChip.unit, &(pOpRxSlpFlagGet->rxSlpFlag));
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
 * @brief Get the SWITCH firmware sleep handshake fail indication flag.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpSlpFailFlagGet Pointer to a struct that holds parameters to get the flag of ports that initiate a sleep request but encounter a sleep fail event.\n
 *                               The struct includes slpFailFlag.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_sleep_fail_flag(), dva_op_get_sleep_fail_flag(), cvb_op_get_sleep_fail_flag(), and cva_op_get_sleep_fail_flag().
 */
RtkApiRet rtk_op_sleep_fail_flag_get(UnitChip_t unitChip, OpSlpFailFlagGet_t *pOpSlpFailFlagGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpSlpFailFlagGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_sleep_fail_flag(unitChip.unit, &(pOpSlpFailFlagGet->slpFailFlag));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_sleep_fail_flag(unitChip.unit, &(pOpSlpFailFlagGet->slpFailFlag));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_sleep_fail_flag(unitChip.unit, &(pOpSlpFailFlagGet->slpFailFlag));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_sleep_fail_flag(unitChip.unit, &(pOpSlpFailFlagGet->slpFailFlag));
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
 * @brief Get the SWITCH firmware sleep handshake success indication flag.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpRxSlpSuccessFlagGet Pointer to a struct that holds parameters to get the flag of ports that achieve a successful sleep event.\n
 *                                    The struct includes slpSuccessFlag.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_sleep_success_flag(), dva_op_get_sleep_success_flag(), cvb_op_get_sleep_success_flag(), and cva_op_get_sleep_success_flag().
 */
RtkApiRet rtk_op_sleep_success_flag_get(UnitChip_t unitChip, OpRxSlpSuccessFlagGet_t *pOpRxSlpSuccessFlagGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpRxSlpSuccessFlagGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_sleep_success_flag(unitChip.unit, &(pOpRxSlpSuccessFlagGet->slpSuccessFlag));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_sleep_success_flag(unitChip.unit, &(pOpRxSlpSuccessFlagGet->slpSuccessFlag));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_sleep_success_flag(unitChip.unit, &(pOpRxSlpSuccessFlagGet->slpSuccessFlag));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_sleep_success_flag(unitChip.unit, &(pOpRxSlpSuccessFlagGet->slpSuccessFlag));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_EVENT_STATUS */

/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_REQ Wake-up request sending function
 * @brief  The wake request sending function provides wakeup.request
 * @{
 */

/**
 * @brief Trigger the selected ports to send a remote wakeup to the link partner.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpPortSendRwake Pointer to a struct that holds parameters for making some ports send a remote wake to their link partner.\n
 *                             The struct includes portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_port_send_rwake(), dva_op_port_send_rwake(), cvb_op_port_send_rwake(), and cva_op_port_send_rwake().
 */
RtkApiRet rtk_op_port_send_rwake(UnitChip_t unitChip, OpPortSendRwake_t *pOpPortSendRwake)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpPortSendRwake)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_port_send_rwake(unitChip.unit, pOpPortSendRwake->portMap);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_port_send_rwake(unitChip.unit, pOpPortSendRwake->portMap);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_port_send_rwake(unitChip.unit, pOpPortSendRwake->portMap);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_port_send_rwake(unitChip.unit, pOpPortSendRwake->portMap);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_REQ */

/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_EVENT_STATUS Wake-up event status function
 * @brief  The wake-up received status function provides the wakeup.indication
 * @{
 */

/**
 * @brief Get the WUP flag.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpWupFlagGet Pointer to a struct that holds parameters to get the WUP flag.\n
 *                           The struct includes portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_wup_flag(), dva_op_get_wup_flag(), cvb_op_get_wup_flag(), and cva_op_get_wup_flag().
 */
RtkApiRet rtk_op_wup_flag_get(UnitChip_t unitChip, OpWupFlagGet_t *pOpWupFlagGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWupFlagGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_wup_flag(unitChip.unit, &(pOpWupFlagGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_wup_flag(unitChip.unit, &(pOpWupFlagGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_wup_flag(unitChip.unit, &(pOpWupFlagGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_wup_flag(unitChip.unit, &(pOpWupFlagGet->portMap));
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
 * @brief Get the WUR flag.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpWurFlagGet Pointer to a struct that holds parameters to get the WUR flag.\n
 *                           The struct includes portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_wur_flag(), dva_op_get_wur_flag(), cvb_op_get_wur_flag(), and cva_op_get_wur_flag().
 */
RtkApiRet rtk_op_wur_flag_get(UnitChip_t unitChip, OpWurFlagGet_t *pOpWurFlagGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWurFlagGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_wur_flag(unitChip.unit, &(pOpWurFlagGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_wur_flag(unitChip.unit, &(pOpWurFlagGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_wur_flag(unitChip.unit, &(pOpWurFlagGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_wur_flag(unitChip.unit, &(pOpWurFlagGet->portMap));
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
 * @brief Clear wake flags including WUP flag, WUR flag, POWN (PowerOn), and LWAKE (LocalWakeup).\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_wakeflag_clearall(), dva_op_wakeflag_clearall(), cvb_op_wakeflag_clearall(), and cva_op_wakeflag_clearall().
 */
RtkApiRet rtk_op_wakeflag_clearall(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_op_wakeflag_clearall(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_op_wakeflag_clearall(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_op_wakeflag_clearall(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_op_wakeflag_clearall(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_EVENT_STATUS */

/**
 * @defgroup OPFSM_INTR_OPFSM_EVENT OPFSM event interrupt function
 * @brief  Pass the OPFSM event interrupt to CPU or externnal interrupt. The OPFSM events including: OPFSM reset, PIN reset, OPFSM Power on and OPFSM detect local wakeup.
 * @{
 */

/**
 * @brief Get the OP-FSM event interrupt mask.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpEventExtIntrMskGet Pointer to a struct that holds parameters to get the interrupt mask configuration of OPFSM events.\n
 *                                   The struct includes cfgValue.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_event_ext_intr_msk(), dva_op_get_event_ext_intr_msk(), cvb_op_get_event_ext_intr_msk(), and cva_op_get_event_ext_intr_msk().
 */
RtkApiRet rtk_op_event_ext_intr_msk_get(UnitChip_t unitChip, OpEventExtIntrMsk_t *pOpEventExtIntrMskGet)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == pOpEventExtIntrMskGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_event_ext_intr_msk(unitChip.unit, &(pOpEventExtIntrMskGet->cfgValue));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_event_ext_intr_msk(unitChip.unit, &(pOpEventExtIntrMskGet->cfgValue));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_event_ext_intr_msk(unitChip.unit, &(pOpEventExtIntrMskGet->cfgValue));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_event_ext_intr_msk(unitChip.unit, &(pOpEventExtIntrMskGet->cfgValue));
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
 * @brief Set the the OP-FSM event interrupt mask.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpEventExtIntrMskSet Pointer to a struct that holds parameters to set the interrupt mask configuration of OPFSM events.\n
 *                                  The struct includes cfgValue.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_event_ext_intr_msk(), dva_op_set_event_ext_intr_msk(), cvb_op_set_event_ext_intr_msk(), and cva_op_set_event_ext_intr_msk().
 */
RtkApiRet rtk_op_event_ext_intr_msk_set(UnitChip_t unitChip, OpEventExtIntrMsk_t *pOpEventExtIntrMskSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpEventExtIntrMskSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_event_ext_intr_msk(unitChip.unit, pOpEventExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_event_ext_intr_msk(unitChip.unit, pOpEventExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_event_ext_intr_msk(unitChip.unit, pOpEventExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_event_ext_intr_msk(unitChip.unit, pOpEventExtIntrMskSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);
    return ret;
}
/**@}*/ /* OPFSM_INTR_OPFSM_EVENT */

/**
 * @defgroup OPFSM_INTR_SLEEP_WAKE_EVENT_INTR Sleep and Wake-up received interrupt function
 * @brief  Wake-up event interrupt mask and interrupt enable and sleep interrupt enable configuration
 * @{
 */

/**
 * @brief Get the configuration of external interrupt triggered by sleep events.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpSlpEventIntrEnGet Pointer to a struct that holds parameters to get the enabling configuration of sleep event interrupts.\n
 *                                  The struct includes extIntrOn.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_sleep_event_intr_en(), dva_op_get_sleep_event_intr_en(), and cvb_op_get_sleep_event_intr_en().
 */
RtkApiRet rtk_op_sleep_event_intr_en_get(UnitChip_t unitChip, OpSlpEventIntrEn_t *pOpSlpEventIntrEnGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpSlpEventIntrEnGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_sleep_event_intr_en(unitChip.unit, &(pOpSlpEventIntrEnGet->extIntrOn));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_sleep_event_intr_en(unitChip.unit, &(pOpSlpEventIntrEnGet->extIntrOn));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_sleep_event_intr_en(unitChip.unit, &(pOpSlpEventIntrEnGet->extIntrOn));
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
 * @brief Allow or disallow the SWITCH to output an interrupt signal when specific sleep events occur.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpSlpEventIntrEnSet Pointer to a struct that holds parameters to set the enabling configuration of sleep event interrupts.\n
 *                                 The struct includes extIntrOn.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_sleep_event_intr_en(), dva_op_set_sleep_event_intr_en(), and cvb_op_set_sleep_event_intr_en().
 */
RtkApiRet rtk_op_sleep_event_intr_en_set(UnitChip_t unitChip, OpSlpEventIntrEn_t *pOpSlpEventIntrEnSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpSlpEventIntrEnSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_sleep_event_intr_en(unitChip.unit, pOpSlpEventIntrEnSet->extIntrOn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_sleep_event_intr_en(unitChip.unit, pOpSlpEventIntrEnSet->extIntrOn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_sleep_event_intr_en(unitChip.unit, pOpSlpEventIntrEnSet->extIntrOn);
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
 * @brief Get the WUR interrupt mask port mask.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpWurExtIntrMskGet Pointer to a struct that holds parameters to get the interrupt mask configuration of the WUR event.\n
 *                                 The struct includes portMap and cfgValue.
 *                                 - cfgValue = 1: Return portMap indicating ports that mask WUR external interrupt.
 *                                 - cfgValue = 0: Return portMap indicating ports that unmask WUR external interrupt.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_wur_ext_intr_msk(), dva_op_get_wur_ext_intr_msk(), cvb_op_get_wur_ext_intr_msk(), and cva_op_get_wur_ext_intr_msk().
 */
RtkApiRet rtk_op_wur_ext_intr_msk_get(UnitChip_t unitChip, OpWurExtIntrMsk_t *pOpWurExtIntrMskGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWurExtIntrMskGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskGet->cfgValue, &(pOpWurExtIntrMskGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskGet->cfgValue, &(pOpWurExtIntrMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskGet->cfgValue, &(pOpWurExtIntrMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskGet->cfgValue, &(pOpWurExtIntrMskGet->portMap));
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
 * @brief Set port mask of sending a signal on the interrupt pin (INTB) upon receiving a WUR.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpWurExtIntrMskSet Pointer to a struct that holds parameters to set the interrupt mask configuration of the WUR event.\n
 *                                The struct includes portMap and cfgValue.
 *                                - cfgValue = 1: Mask the WUR external interrupt of ports designated in the portMap.
 *                                - cfgValue = 0: Unmask the WUR external interrupt of ports designated in the portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_wur_ext_intr_msk(), dva_op_set_wur_ext_intr_msk(), cvb_op_set_wur_ext_intr_msk(), and cva_op_set_wur_ext_intr_msk().
 */
RtkApiRet rtk_op_wur_ext_intr_msk_set(UnitChip_t unitChip, OpWurExtIntrMsk_t *pOpWurExtIntrMskSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWurExtIntrMskSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskSet->portMap, pOpWurExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskSet->portMap, pOpWurExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskSet->portMap, pOpWurExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_wur_ext_intr_msk(unitChip.unit, pOpWurExtIntrMskSet->portMap, pOpWurExtIntrMskSet->cfgValue);
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
 * @brief Get the WUP interrupt port mask.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] pOpWupExtIntrMskGet Pointer to a struct that holds parameters to get the interrupt mask configuration of the WUP event.\n
 *                                 The struct includes portMap and cfgValue.
 *                                 - cfgValue = 1: Return portMap indicating ports that mask WUP external interrupt.
 *                                 - cfgValue = 0: Return portMap indicating ports that unmask WUP external interrupt.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_get_wup_ext_intr_msk(), dva_op_get_wup_ext_intr_msk(), cvb_op_get_wup_ext_intr_msk(), and cva_op_get_wup_ext_intr_msk().
 */
RtkApiRet rtk_op_wup_ext_intr_msk_get(UnitChip_t unitChip, OpWupExtIntrMsk_t *pOpWupExtIntrMskGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pOpWupExtIntrMskGet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_get_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskGet->cfgValue, &(pOpWupExtIntrMskGet->portMap));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_get_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskGet->cfgValue, &(pOpWupExtIntrMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_get_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskGet->cfgValue, &(pOpWupExtIntrMskGet->portMap));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_get_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskGet->cfgValue, &(pOpWupExtIntrMskGet->portMap));
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
 * @brief Set specific port mask of sending a signal on the interrupt pin upon receing a WUP.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which SWITCH is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] pOpWupExtIntrMskSet Pointer to a struct that holds parameters to set the interrupt mask configuration of the WUP event.\n
 *                                The struct includes portMap and cfgValue.
 *                                - cfgValue = 1: Mask the WUP external interrupt of ports designated in the portMap.
 *                                - cfgValue = 0: Unmask the WUP external interrupt of ports designated in the portMap.
 *
 * @retval RT_ERR_OK The function was executed successfully.
 * @retval -RT_ERR_NULL_POINTER if a NULL parameter is passed.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Others Please refer to dv1_op_set_wup_ext_intr_msk(), dva_op_set_wup_ext_intr_msk(), cvb_op_set_wup_ext_intr_msk(), and cva_op_set_wup_ext_intr_msk().
 */
RtkApiRet rtk_op_wup_ext_intr_msk_set(UnitChip_t unitChip, OpWupExtIntrMsk_t *pOpWupExtIntrMskSet)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if(NULL == pOpWupExtIntrMskSet)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_op_set_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskSet->portMap, pOpWupExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_op_set_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskSet->portMap, pOpWupExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_op_set_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskSet->portMap, pOpWupExtIntrMskSet->cfgValue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_op_set_wup_ext_intr_msk(unitChip.unit, pOpWupExtIntrMskSet->portMap, pOpWupExtIntrMskSet->cfgValue);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* OPFSM_INTR_SLEEP_WAKE_EVENT_INTR */

/**@}*/ /* RTK_MODULE_OPFSM */
