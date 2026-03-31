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
#include "rtl907xc_va_api_tsn.h"

/**
 * @addtogroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @addtogroup RTL907XCVA_CB_EXAMPLE RTL907XC_VA CB Example
 * @{
 */

/**
 * @defgroup RTL907XCVA_CB_GET_ALLCONFIG_EXAMPLE CB Get All Configurations Example
 *
 * @brief <b> Get CB all configurations. </b>
 *
 * @section Purpose
 *          This example demonstrates how to get CB all configurations.
 *
 * @section Description
 *          This example can get the enable status of the R-tag parser, passive stream identification entry, individual recovery entry, \n
 *          sequence recovery entry, misc entry, active stream identification entry and CB control register values.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_cb_get_allconfig(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_cb_get_allconfig(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VA};
    CbParseRtagEnabledInfo_t cbParseRtagEnabledInfo = {.port = CVA_RTK_USER_PORT_1ST};
    CbEnabledInfo_t          cbEnabledInfo;
    CbSidEnabledInfo_t       cbSidEnabledInfo;
    CbInterruptEnableInfo_t  cbInterruptEnableInfo;
    CHK_FUN_RET(retVal, drv_memset(&cbEnabledInfo, 0, sizeof(CbEnabledInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbSidEnabledInfo, 0, sizeof(CbSidEnabledInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbInterruptEnableInfo, 0, sizeof(CbInterruptEnableInfo_t)));

    CbLatenTimerInfo_t      cbLatenTimerInfo;
    CbLatenResetTimerInfo_t cbLatenResetTimerInfo;
    CHK_FUN_RET(retVal, drv_memset(&cbLatenTimerInfo, 0, sizeof(CbLatenTimerInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbLatenResetTimerInfo, 0, sizeof(CbLatenResetTimerInfo_t)));

    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index = 1u,
    };
    CbMiscInfo_t cbMiscInfo = {
        .index = 3u,
    };
    CbIngressIndrcvInfo_t cbIngressIndrcvInfo = {
        .index = 5u,
    };
    CbEgressSeqrcvInfo_t cbEgressSeqrcvInfo = {
        .index = 7u,
    };
    CbActsidRhtblInfo_t cbActsidRhtblInfo = {
        .index = 9u,
    };
    CbActsidOiltblInfo_t cbActsidOiltblInfo = {
        .index = 10u,
    };
    CbActsidEgiftblInfo_t cbActsidEgiftblInfo = {
        .index = 11u,
    };

    /* Step1. Get the status of R-tag parser */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_get(unitChip, &cbParseRtagEnabledInfo));
    /* Step2. Get the status of stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_get(unitChip, &cbSidEnabledInfo));
    /* Step3. Get the status of CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_get(unitChip, &cbEnabledInfo));
    /* Step4. Get the status of CB global interrupt */
    CHK_FUN_RET(retVal, rtk_cb_interrupt_enable_get(unitChip, &cbInterruptEnableInfo));
    /* Step5. Get the latent error test timer value */
    CHK_FUN_RET(retVal, rtk_cb_laten_timer_get(unitChip, &cbLatenTimerInfo));
    /* Step6. Get the latent error reset timer value */
    CHK_FUN_RET(retVal, rtk_cb_laten_reset_timer_get(unitChip, &cbLatenResetTimerInfo));
    /* Step7. Get the MISC entry */
    CHK_FUN_RET(retVal, rtk_cb_misc_get(unitChip, &cbMiscInfo));
    /* Step8. Get the passive stream identification entry */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_get(unitChip, &cbIngressPassiveSidInfo));
    /* Step9. Get the individual recovery entry */
    CHK_FUN_RET(retVal, rtk_cb_ingress_indrcv_get(unitChip, &cbIngressIndrcvInfo));
    /* Step10. Get the sequence recovery entry */
    CHK_FUN_RET(retVal, rtk_cb_egress_seqrcv_get(unitChip, &cbEgressSeqrcvInfo));
    /* Step11. Get the replication header entry */
    CHK_FUN_RET(retVal, rtk_cb_actsid_rhtbl_get(unitChip, &cbActsidRhtblInfo));
    /* Step12. Get the outgoing interface list entry */
    CHK_FUN_RET(retVal, rtk_cb_actsid_oiltbl_get(unitChip, &cbActsidOiltblInfo));
    /* Step13. Get the egress interface entry */
    CHK_FUN_RET(retVal, rtk_cb_actsid_egiftbl_get(unitChip, &cbActsidEgiftblInfo));
    /* Expected Behaviour: Get the status of R-tag parser, get passive stream identification entry, individual recovery entry,
     * sequence recovery entry, misc entry, active stream identification entry and CB control register values */
    return retVal;
}
/**@}*/ /* RTL907XCVA_CB_GET_ALLCONFIG_EXAMPLE */
/**@}*/ /* RTL907XCVA_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */
