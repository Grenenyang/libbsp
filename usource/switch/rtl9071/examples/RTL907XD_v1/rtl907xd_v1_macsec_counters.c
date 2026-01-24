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
#include "rtkas_api_macsec.h"

/* Declaration */
RtkApiRet dv1_macsec_get_counters(void);
RtkApiRet dv1_macsec_clear_counters(void);
RtkApiRet dv1_macsec_clear_all_counters(void);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup MACSEC_EXAMPLE MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XD_V1_MACSEC_EXAMPLE RTL907XD_V1 MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XD_V1_MACSEC_GET_COUNTERS MACsec Get Counters Example
 *
 * @brief <b>Get All MACsec Counters </b> \n
 *        Get all MACsec counters on port 1. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to get MACsec Counters.
 *
 * @section Description
 *          This example demonstrates how to get port 1's global transmit/receive counters and \n
 *          SA index#0 transmit/receive counters.
 *
 * @{
 */
RtkApiRet dv1_macsec_get_counters(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = DV1_UNIT,
        .chip = CHIP_907XD_V1};

    /* Step 1. get tx global counters */
    MacsecTxGlobalCounter_t txGlobalCounters = {.port = DV1_RTK_USER_PORT_1ST};
    CHK_FUN_RET(retVal, rtk_macsec_get_tx_global_counter(unitChip, &txGlobalCounters));

    /* Step 2. get tx SA counters for first SA */
    MacsecTxSaCounter_t txSaCounters = {.port = DV1_RTK_USER_PORT_1ST, .index = RTK_MACSEC_MIN_ENTRY_IDX};
    CHK_FUN_RET(retVal, rtk_macsec_get_tx_sa_counter(unitChip, &txSaCounters));

    /* Step 3. get rx global counters */
    MacsecRxGlobalCounter_t rxGlobalCounters = {.port = DV1_RTK_USER_PORT_1ST};
    CHK_FUN_RET(retVal, rtk_macsec_get_rx_global_counter(unitChip, &rxGlobalCounters));

    /* Step 2. get rx SA counters for first SA */
    MacsecRxSaCounter_t rxSaCounters = {.port = DV1_RTK_USER_PORT_1ST, .index = RTK_MACSEC_MIN_ENTRY_IDX};
    CHK_FUN_RET(retVal, rtk_macsec_get_rx_sa_counter(unitChip, &rxSaCounters));

    /*  Expected Behaviour: Get all macsec counters on port 1  */

    return retVal;
}

/**@}*/ /* RTL907XD_V1_MACSEC_GET_COUNTERS */

/**
 * @addtogroup RTL907XD_V1_MACSEC_CLEAR_COUNTERS MACsec Clear Counters Example
 *
 * @brief <b>Clear MACsec Counters </b> \n
 *        Clear MACsec counters on port 1. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to clear MACsec Counters.
 *
 * @section Description
 *          This example demonstrates how to clear port 1's global transmit/receive counters and \n
 *          SA index#0 transmit/receive counters.
 *
 * @{
 */
RtkApiRet dv1_macsec_clear_counters(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = DV1_UNIT,
        .chip = CHIP_907XD_V1};

    /* Step 1. clear tx global counters */
    MacsecClearTxGlobalCounter_t clearTxGlobal = {.port = DV1_RTK_USER_PORT_1ST};
    CHK_FUN_RET(retVal, rtk_macsec_clear_tx_global_counter(unitChip, &clearTxGlobal));

    /* Step 2. clear tx per-sa counters */
    MacsecClearTxSaCounter_t clearTxSa = {.port = DV1_RTK_USER_PORT_1ST, .index = RTK_MACSEC_MIN_ENTRY_IDX};
    CHK_FUN_RET(retVal, rtk_macsec_clear_tx_sa_counter(unitChip, &clearTxSa));

    /* Step 3. clear rx global counters */
    MacsecClearRxGlobalCounter_t clearRxGlobal = {.port = DV1_RTK_USER_PORT_1ST};
    CHK_FUN_RET(retVal, rtk_macsec_clear_rx_global_counter(unitChip, &clearRxGlobal));

    /* Step 4. clear rx per-sa counters */
    MacsecClearRxSaCounter_t clearRxSa = {.port = DV1_RTK_USER_PORT_1ST, .index = RTK_MACSEC_MIN_ENTRY_IDX};
    CHK_FUN_RET(retVal, rtk_macsec_clear_rx_sa_counter(unitChip, &clearRxSa));

    /*  Expected Behaviour: Tx/Rx global and/or per sa counters are clear to 0 */
    return retVal;
}

/**@}*/ /* RTL907XD_V1_MACSEC_CLEAR_COUNTERS */

/**
 * @addtogroup RTL907XD_V1_MACSEC_CLEAR_ALL_COUNTERS MACsec Clear All Counters Example
 *
 * @brief <b> Clear all MACsec Counters </b> \n
 *        Clear All MACsec counters on port 1. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to clear all MACsec counters at once.
 *
 * @section Description
 *          This example demonstrates how to clear all port 1's MACsec counters.
 *
 * @{
 *
 */
RtkApiRet dv1_macsec_clear_all_counters(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = DV1_UNIT,
        .chip = CHIP_907XD_V1};

    /* Step 1. clear all tx global counters for port 1 */
    MacsecClearAllTxCounter_t clearAllTxCnts = {.port = DV1_RTK_USER_PORT_1ST};
    CHK_FUN_RET(retVal, rtk_macsec_clear_all_tx_counter(unitChip, &clearAllTxCnts));

    /* Step 2. clear all rx global counters for port 1 */
    MacsecClearAllRxCounter_t clearAllRxCnts = {.port = DV1_RTK_USER_PORT_1ST};
    CHK_FUN_RET(retVal, rtk_macsec_clear_all_rx_counter(unitChip, &clearAllRxCnts));

    /*  Expected Behaviour: All Tx/Rx counters are clear to 0 */
    return retVal;
}

/**@}*/ /* RTL907XD_V1_MACSEC_CLEAR_ALL_COUNTERS */
/**@}*/ /* RTL907XD_V1_MACSEC_EXAMPLE */
/**@}*/ /* MACSEC_EXAMPLE */
/**@}*/ /* EXAMPLE */