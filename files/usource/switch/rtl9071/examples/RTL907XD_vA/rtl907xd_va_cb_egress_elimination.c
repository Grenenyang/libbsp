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
#include "rtl907xd_va_api_tsn.h"

/**
 * @addtogroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_CB_EXAMPLE RTL907XD_VA CB Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_CB_EGRESS_ELIMINATION_EXAMPLE CB Egress Elimination Example
 *
 * @brief <b> Duplicate streams elimination at egress port. </b> \n
 *            Eliminate duplicate packets in a multiple-member stream by utilizing the sequence numbers of the packets. \n
 *            As a result, the egress port will have only a single, non-redundant packet stream.
 *
 * @section Purpose
 *          This example demonstrates how to discard duplicate packets at the egress port. \n
 *
 * @section Description
 *          The host sends a stream with Rtag to switch port1, whose sequence numbers are 0,1,2,3,4,5,6,7,8,9 \n
 *          and destination MAC is "00-e0-4c-22-00-00" to match the stream identification. \n
 *          Host sends another stream with Rtag to switch port1, whose sequence numbers are 0,1,2,3,4 \n
 *          and source MAC is "02-33-44-55-66-88" to match the stream identification. \n
 *          The switch will drop 5 packets whose sequence numbers are 0,1,2,3,4. \n
 *          A latent error will occur if the deviation between the current deviation value and the baseline deviation value \n
 *          exceeds expected difference (set in the parameter cbEgressSeqrcvInfo.seqTabParamDva->expDiff). \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_cb_egress_elimination(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_cb_egress_elimination(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};
    /* Config information */
    /* Global */
    CbLatenTimerInfo_t cbLatenTimerInfo = {
        .lattimvalue = 1000u,
    };
    CbLatenResetTimerInfo_t cbLatenResetTimerInfo = {
        .latrsttimvalue = 5000u,
    };
    CbParseRtagEnabledInfo_t cbParseRtagEnabledInfo = {
        .port   = DVA_RTK_USER_PORT_1ST,
        .status = ENABLED,
    };
    CbEnabledInfo_t cbEnabledInfo = {
        .status = ENABLED,
    };
    CbSidEnabledInfo_t cbSidEnabledInfo = {
        .status = ENABLED,
    };
    CbInterruptEnableInfo_t cbInterruptEnableInfo = {
        .status = ENABLED,
    };
    /* Stream identification */
    uint8 dstmac[MAC_ADDR_LEN] = {0x00, 0xe0, 0x4c, 0x22, 0x00, 0x00};
    uint8 srcmac[MAC_ADDR_LEN] = {0x02, 0x33, 0x44, 0x55, 0x66, 0x88};

    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo;
    CbMiscInfo_t              cbMiscInfo;
    CbEgressSeqrcvInfo_t      cbEgressSeqrcvInfo;
    CHK_FUN_RET(retVal, drv_memset(&cbIngressPassiveSidInfo, 0, sizeof(CbIngressPassiveSidInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbMiscInfo, 0, sizeof(CbMiscInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbEgressSeqrcvInfo, 0, sizeof(CbEgressSeqrcvInfo_t)));

    /* For checking */
    CbIngressPerstreamIdcntInfo_t cbIngressPerstreamIdcntInfo;
    CbIngressPerportIdcntInfo_t   cbIngressPerportIdcntInfo;
    CHK_FUN_RET(retVal, drv_memset(&cbIngressPerstreamIdcntInfo, 0, sizeof(CbIngressPerstreamIdcntInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbIngressPerportIdcntInfo, 0, sizeof(CbIngressPerportIdcntInfo_t)));

    CbEgressSeqrcvDropcntInfo_t cbEgressSeqrcvDropcntInfo;
    CbEgressSeqrcvPasscntInfo_t cbEgressSeqrcvPasscntInfo;
    CbEgressSeqhistoryInfo_t    cbEgressSeqhistoryInfo;
    CHK_FUN_RET(retVal, drv_memset(&cbEgressSeqrcvDropcntInfo, 0, sizeof(CbEgressSeqrcvDropcntInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbEgressSeqrcvPasscntInfo, 0, sizeof(CbEgressSeqrcvPasscntInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbEgressSeqhistoryInfo, 0, sizeof(CbEgressSeqhistoryInfo_t)));

    CbInterruptFlagInfo_t     cbInterruptFlagInfo;
    CbInterruptMaskInfo_t     cbInterruptMaskInfo;
    CbInterruptPerStateInfo_t cbInterruptStateInfo;
    CbInterruptPerStateInfo_t cbInterruptStateClearInfo;
    CHK_FUN_RET(retVal, drv_memset(&cbInterruptFlagInfo, 0, sizeof(CbInterruptFlagInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbInterruptMaskInfo, 0, sizeof(CbInterruptMaskInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbInterruptStateInfo, 0, sizeof(CbInterruptPerStateInfo_t)));
    CHK_FUN_RET(retVal, drv_memset(&cbInterruptStateClearInfo, 0, sizeof(CbInterruptPerStateInfo_t)));

    /* Step1. Enable CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step2. Enable passive stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step3. Set latent error test timer */
    CHK_FUN_RET(retVal, rtk_cb_laten_timer_set(unitChip, &cbLatenTimerInfo));
    /* Step4.Set latent error reset timer */
    CHK_FUN_RET(retVal, rtk_cb_laten_reset_timer_set(unitChip, &cbLatenResetTimerInfo));
    /* Step5. Enable cb global interrupt */
    CHK_FUN_RET(retVal, rtk_cb_interrupt_enable_set(unitChip, &cbInterruptEnableInfo));
    /* Step6. Enable R-tag parser to extract the sequence number from ingress packet */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));

    /* Step7. Set passive stream identification entry at port 1 to match ingress packets */
    cbIngressPassiveSidInfo.index                      = 5;
    cbIngressPassiveSidInfo.sidentryParam.carebits     = DVA_TSN_SELECTED_FILED_DSTADR;
    cbIngressPassiveSidInfo.sidentryParam.portMask     = DVA_RTK_USER_PMAP_1ST;
    cbIngressPassiveSidInfo.sidentryParam.isValid      = ENABLED;
    cbIngressPassiveSidInfo.sidentryParam.streamHandle = 7;
    CHK_FUN_RET(retVal, drv_memcpy(cbIngressPassiveSidInfo.sidentryParam.dstMac, dstmac, MAC_ADDR_LEN));
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));

    cbIngressPassiveSidInfo.index                      = 6;
    cbIngressPassiveSidInfo.sidentryParam.carebits     = DVA_TSN_SELECTED_FILED_SRCADR;
    cbIngressPassiveSidInfo.sidentryParam.portMask     = DVA_RTK_USER_PMAP_1ST;
    cbIngressPassiveSidInfo.sidentryParam.isValid      = ENABLED;
    cbIngressPassiveSidInfo.sidentryParam.streamHandle = 8;
    CHK_FUN_RET(retVal, drv_memcpy(cbIngressPassiveSidInfo.sidentryParam.srcMac, srcmac, MAC_ADDR_LEN));
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step8.  Copy packets to port 2 and add an R-tag to those packets.
     *         Enable sequence recovery at egress port
     *
     */
    cbMiscInfo.index                   = 5;
    cbMiscInfo.lutTabParam.indRcvEn    = DISABLED;
    cbMiscInfo.lutTabParam.seqRcvIdx   = 40u;
    cbMiscInfo.lutTabParam.seqRcvEn    = ENABLED;
    cbMiscInfo.lutTabParam.splitEn     = ENABLED;
    cbMiscInfo.lutTabParam.splitDPM    = DVA_RTK_USER_PMAP_2ND;
    cbMiscInfo.lutTabParam.genSeqNumEn = DISABLED;
    cbMiscInfo.lutTabParam.activeEn    = DISABLED;
    cbMiscInfo.lutTabParam.encEn       = ENABLED;
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &cbMiscInfo));
    cbMiscInfo.index                   = 6;
    cbMiscInfo.lutTabParam.indRcvEn    = DISABLED;
    cbMiscInfo.lutTabParam.seqRcvIdx   = 40u;
    cbMiscInfo.lutTabParam.seqRcvEn    = ENABLED;
    cbMiscInfo.lutTabParam.splitEn     = ENABLED;
    cbMiscInfo.lutTabParam.splitDPM    = DVA_RTK_USER_PMAP_2ND;
    cbMiscInfo.lutTabParam.genSeqNumEn = DISABLED;
    cbMiscInfo.lutTabParam.activeEn    = DISABLED;
    cbMiscInfo.lutTabParam.encEn       = ENABLED;
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &cbMiscInfo));
    /* Step9. Set the sequence recovery entry to eliminate duplicate packets */
    cbEgressSeqrcvInfo.index                = 40;
    cbEgressSeqrcvInfo.seqTabParam.hisLen   = 3u;
    cbEgressSeqrcvInfo.seqTabParam.takeno   = 0u;
    cbEgressSeqrcvInfo.seqTabParam.alg      = 0u;
    cbEgressSeqrcvInfo.seqTabParam.expDiff  = 3u;
    cbEgressSeqrcvInfo.seqTabParam.rstLoad  = 10000000u;
    cbEgressSeqrcvInfo.seqTabParam.latPaths = 2u;
    cbEgressSeqrcvInfo.seqTabParam.laten    = 1u;
    cbEgressSeqrcvInfo.seqTabParam.valid    = ENABLED;
    CHK_FUN_RET(retVal, rtk_cb_egress_seqrcv_set(unitChip, &cbEgressSeqrcvInfo));
    /* Expected Behavior: The switch will eliminate duplicate packets, and there is a latent error
     *
     */
    /* Step 10. Check counter */
    cbIngressPerstreamIdcntInfo.index = 5u;
    CHK_FUN_RET(retVal, rtk_cb_ingress_perstream_idcnt_get(unitChip, &cbIngressPerstreamIdcntInfo));
    cbIngressPerstreamIdcntInfo.index = 6u;
    CHK_FUN_RET(retVal, rtk_cb_ingress_perstream_idcnt_get(unitChip, &cbIngressPerstreamIdcntInfo));
    cbIngressPerportIdcntInfo.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_cb_ingress_perport_idcnt_get(unitChip, &cbIngressPerportIdcntInfo));
    cbEgressSeqrcvDropcntInfo.index = cbEgressSeqrcvInfo.index;
    CHK_FUN_RET(retVal, rtk_cb_egress_seqrcv_dropcnt_get(unitChip, &cbEgressSeqrcvDropcntInfo));
    cbEgressSeqrcvPasscntInfo.index = cbEgressSeqrcvInfo.index;
    CHK_FUN_RET(retVal, rtk_cb_egress_seqrcv_passcnt_get(unitChip, &cbEgressSeqrcvPasscntInfo));
    /* Step 11. Check sequence history */
    cbEgressSeqhistoryInfo.index = cbEgressSeqrcvInfo.index;
    CHK_FUN_RET(retVal, rtk_cb_egress_seqhistory_get(unitChip, &cbEgressSeqhistoryInfo));
    /* Expected Behavior: seqHistoryEntry.val[0] = 0x380
     *
     */
    /* Step 12. Wait 5s, check interrupt state */
    CHK_FUN_RET(retVal, rtk_cb_interrupt_flag_get(unitChip, &cbInterruptFlagInfo));
    /* Expected Behavior: flag = 1
     *
     */
    CHK_FUN_RET(retVal, rtk_cb_interrupt_mask_get(unitChip, &cbInterruptMaskInfo));
    /* Expected Behavior: statusmask = 0x10000000000u
     *
     */
    cbInterruptStateInfo.index = cbEgressSeqrcvInfo.index;
    CHK_FUN_RET(retVal, rtk_cb_interrupt_state_get(unitChip, &cbInterruptStateInfo));
    /* Expected Behavior: status = 0x1u
     *
     */
    /* Step 13. Clear interrupt state */
    cbInterruptStateClearInfo.index = cbEgressSeqrcvInfo.index;
    CHK_FUN_RET(retVal, rtk_cb_interrupt_state_clear(unitChip, &cbInterruptStateClearInfo));
    /* Step 14. Check interrupt state */
    CHK_FUN_RET(retVal, rtk_cb_interrupt_flag_get(unitChip, &cbInterruptFlagInfo));
    /* Expected Behavior: flag = 0
     *
     */
    CHK_FUN_RET(retVal, rtk_cb_interrupt_mask_get(unitChip, &cbInterruptMaskInfo));
    /* Expected Behavior: statusmask = 0x0u
     *
     */
    cbInterruptStateInfo.index = cbEgressSeqrcvInfo.index;
    CHK_FUN_RET(retVal, rtk_cb_interrupt_state_get(unitChip, &cbInterruptStateInfo));
    /* Expected Behavior: status = 0x0u
     *
     */
    return retVal;
}
/**@}*/ /* RTL907XDVA_CB_EGRESS_ELIMINATION_EXAMPLE */
/**@}*/ /* RTL907XDVA_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */
