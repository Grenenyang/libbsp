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
#include "rtkas_api_cb.h"

/**
 * @addtogroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_CB_EXAMPLE RTL907XD_VA CB Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_CB_RESET_ENTRY_EXAMPLE CB Reset Entry Example
 *
 * @brief <b> Reset a CB entry. </b>
 *
 * @section Purpose
 *          This example demonstrates how to reset a CB entry.
 * @section Description
 *          This example can reset stream identification entry 1, MISC entry 3, individual recovery entry 5, sequence recovery entry 7, \n
 *          and active stream identification entry to their default values.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_cb_reset_single_entry(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_cb_reset_single_entry(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};

    CbIngressPassiveSidResetInfo_t cbIngressPassiveSidReset = {.index = 1u};
    CbIngressIndrcvResetInfo_t     cbIngressIndrcvReset     = {.index = 5u};
    CbEgressSeqrcvResetInfo_t      cbEgressSeqrcvReset      = {.index = 7u};
    CbMiscResetInfo_t              cbMiscReset              = {.index = 3u};

    CbActsidRhtblInfo_t   cbActsidRhtbl;
    CbActsidOiltblInfo_t  cbActsidOiltbl;
    CbActsidEgiftblInfo_t cbActsidEgiftbl;
    CHK_FUN_RET(retVal, drv_memset(&cbActsidRhtbl, 0, sizeof(CbActsidRhtblInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbActsidOiltbl, 0, sizeof(CbActsidOiltblInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbActsidEgiftbl, 0, sizeof(CbActsidEgiftblInfo_t)));

    /* Step 1. Reset stream identification entry */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_reset(unitChip, &cbIngressPassiveSidReset));
    /* Step 2. Reset individual recovery entry */
    CHK_FUN_RET(retVal, rtk_cb_ingress_indrcv_reset(unitChip, &cbIngressIndrcvReset));
    /* Step 3. Reset sequence recovery entry */
    CHK_FUN_RET(retVal, rtk_cb_egress_seqrcv_reset(unitChip, &cbEgressSeqrcvReset));
    /* Step 4. Reset MISC entry */
    CHK_FUN_RET(retVal, rtk_cb_misc_reset(unitChip, &cbMiscReset));
    /* Step 5. Reset active stream identification entry */
    cbActsidOiltbl.index = 9u;
    CHK_FUN_RET(retVal, rtk_cb_actsid_oiltbl_set(unitChip, &cbActsidOiltbl));
    cbActsidEgiftbl.index = 10u;
    CHK_FUN_RET(retVal, rtk_cb_actsid_egiftbl_set(unitChip, &cbActsidEgiftbl));
    cbActsidRhtbl.index = 11u;
    CHK_FUN_RET(retVal, rtk_cb_actsid_rhtbl_set(unitChip, &cbActsidRhtbl));
    /* Expected Behavior: Stream identification entry 1, MISC entry 3,
     * individual recovery entry 5, sequence recovery entry 7,
     * and active stream identification entry should be set to their default values.
     */
    return retVal;
}
/**@}*/ /* RTL907XDVA_CB_RESET_ENTRY_EXAMPLE */
/**@}*/ /* RTL907XDVA_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */
