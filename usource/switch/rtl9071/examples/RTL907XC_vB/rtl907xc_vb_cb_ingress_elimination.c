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
#include "rtl907xc_vb_api_tsn.h"
#include "rtkas_api_cb.h"

/**
 * @addtogroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @addtogroup RTL907XCVB_CB_EXAMPLE RTL907XC_VB CB Example
 * @{
 */

/**
 * @defgroup RTL907XCVB_CB_INGRESS_ELIMINATION_EXAMPLE CB Ingress Elimination Example
 *
 * @brief <b> Duplicate packets elimination at ingress port for a single stream. </b> \n
 *            Eliminate duplicate packets in a single-member stream by utilizing the sequence numbers of the packets. \n
 *            As a result, the switch will forward a non-redundant packet stream.
 *
 * @section Purpose
 *          This example demonstrates how to eliminate duplicate packets at ingress port for a single stream.
 *
 * @section Description
 *          The host sends 4 packets with Rtag to switch port1, whose sequence numbers are 24,24,25,24 \n
 *          and source MAC is "22-11-33-ad-ff-ec" to match the stream identification. \n
 *          The switch will forward 3 packets to the egress port, sequence numbers are 24,25,24. \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_cb_ingress_elimination(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_cb_ingress_elimination(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
    /* config information */
    CbParseRtagEnabledInfo_t cbParseRtagEnabledInfo = {
        .port   = CVB_RTK_USER_PORT_1ST,
        .status = ENABLED,
    };
    /* Global */
    CbEnabledInfo_t cbEnabledInfo = {
        .status = ENABLED,
    };
    CbSidEnabledInfo_t cbSidEnabledInfo = {
        .status = ENABLED,
    };
    /* Stream identification */
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index         = 3u,
        .sidentryParam = {
            .carebits     = CVB_TSN_SELECTED_FILED_SRCADR,
            .srcMac       = {0x22, 0x11, 0x33, 0xad, 0xff, 0xec},
            .portMask     = CVB_RTK_USER_PMAP_1ST,
            .isValid      = ENABLED,
            .streamHandle = 7u,
        },
    };
    CbMiscInfo_t cbMiscInfo = {
        .index       = cbIngressPassiveSidInfo.index,
        .lutTabParam = {
            .indRcvEn    = ENABLED,
            .indRcvIdx   = 67u,
            .seqRcvEn    = DISABLED,
            .splitEn     = DISABLED,
            .genSeqNumEn = DISABLED,
            .activeEn    = DISABLED,
            .encEn       = ENABLED,
        },
    };

    /* Individual recovery */
    CbIngressIndrcvInfo_t cbIngressIndrcvInfo = {
        .index       = cbMiscInfo.lutTabParam.indRcvIdx,
        .indTabParam = {
            .rstload = 5000000u,
            .valid   = ENABLED,
        },
    };

    /* Step1. Enable CB*/
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step2. Enable passive stream identification*/
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step3. Enable the R-tag parser to extract the sequence number from ingress packet */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));

    /* Step4. Set passive stream identification entry at port1 to match ingress packets */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step5. Add an R-tag to egress packets.
     *         Enable individual recovery at ingress port to eliminate duplicate packets
     */
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &cbMiscInfo));
    /* Step6. Set individual recovery entry */
    CHK_FUN_RET(retVal, rtk_cb_ingress_indrcv_set(unitChip, &cbIngressIndrcvInfo));

    /* Expected Behavior: The switch will eliminate duplicate packets at port 1
     *
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_CB_INGRESS_ELIMINATION_EXAMPLE */
/**@}*/ /* RTL907XCVB_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */