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
#include "rtl907xc_vb_api_tsn.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @defgroup RTL907XCVB_CB_EXAMPLE RTL907XC_VB CB Example
 * @{
 */

/**
 * @defgroup RTL907XCVB_CB_EGRESS_ACTIVE_SID_EXAMPLE CB Egress Active Stream Identification Example
 *
 * @brief <b> Modify destination MAC address of egress packets. </b> \n
 *        The destination MAC address of a specific stream can be modified by configuring the active stream identification on the egress port.
 *
 * @section Purpose
 *          This example demonstrates how to modify the destination MAC address of egress packets.
 *
 * @section Description
 *          This example can modify the destination MAC address of original ingress packets that match passive stream identification entry 5 \n
 *          from "00-e0-4c-22-00-00" to "00-f0-dc-25-00-02".
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_cb_egress_active_sid(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_cb_egress_active_sid(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};
    /* config information */
    /* Global */
    CbEnabledInfo_t cbEnabledInfo = {
        .status = ENABLED,
    };
    CbSidEnabledInfo_t cbSidEnabledInfo = {
        .status = ENABLED,
    };
    CbParseRtagEnabledInfo_t cbParseRtagEnabledInfo = {
        .port   = CVB_RTK_USER_PORT_1ST,
        .status = ENABLED};
    /* Stream identification */
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index         = 5u,
        .sidentryParam = {
            .carebits     = CVB_TSN_SELECTED_FILED_DSTADR,
            .dstMac       = {0x00, 0xe0, 0x4c, 0x22, 0x00, 0x00},
            .portMask     = CVB_RTK_USER_PMAP_1ST,
            .isValid      = ENABLED,
            .streamHandle = 7u},
    };
    CbMiscInfo_t cbMiscInfo = {
        .index       = cbIngressPassiveSidInfo.index,
        .lutTabParam = {
            .indRcvEn    = DISABLED,
            .seqRcvEn    = DISABLED,
            .splitEn     = DISABLED,
            .genSeqNumEn = DISABLED,
            .activeEn    = ENABLED,
            .activeIdx   = 2u,
            .encEn       = ENABLED,
        },
    };
    /* Active Stream identification */
    CbActsidEgiftblInfo_t cbActsidEgiftblInfo = {
        .index       = 50u,
        .actegifParm = {
            .saReplaceEn  = DISABLED,
            .daReplaceEn  = ENABLED,
            .vidReplaceEn = DISABLED,
            .pcpReplaceEn = DISABLED,
            .mac          = {0x00, 0xf0, 0xdc, 0x25, 0x00, 0x02},
        },
    };

    CbActsidOiltblInfo_t cbActsidOiltblInfo = {
        .index        = 2u,
        .oilEntryParm = {
            .egifIdx      = (uint8)cbActsidEgiftblInfo.index,
            .oilnextValid = 0u,
        },
    };

    CbActsidRhtblInfo_t cbActsidRhtblInfo = {
        .index       = cbMiscInfo.lutTabParam.activeIdx,
        .rhEntryParm = {
            .portRhIdx = {0, 0, ((uint16)1 << 13u) | (uint16)cbActsidOiltblInfo.index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        }};

    /* Step1. Enable CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step2. Enable passive stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step3. Enable R-tag parser to extract the sequence number from ingress packet */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));

    /* Step4. Set the passive stream identification entry at port1 to match ingress packtes */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step5. Enable active stream identification at port2 to modify egress packets */
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &cbMiscInfo));
    /* Step6. Set active stream identification entry to modify egress packets */
    CHK_FUN_RET(retVal, rtk_cb_actsid_rhtbl_set(unitChip, &cbActsidRhtblInfo));
    CHK_FUN_RET(retVal, rtk_cb_actsid_oiltbl_set(unitChip, &cbActsidOiltblInfo));
    CHK_FUN_RET(retVal, rtk_cb_actsid_egiftbl_set(unitChip, &cbActsidEgiftblInfo));
    /* Expected Behaviour: DMAC of egress packets of port2 is 00-f0-dc-25-00-02
     *
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_CB_EGRESS_ACTIVE_SID_EXAMPLE */
/**@}*/ /* RTL907XCVB_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */
/**@}*/ /* EXAMPLE */