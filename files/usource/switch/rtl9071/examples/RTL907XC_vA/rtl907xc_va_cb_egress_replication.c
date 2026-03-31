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
 * @defgroup RTL907XCVA_CB_EGRESS_REPLICATION_EXAMPLE CB Egress Replication Example
 *
 * @brief <b> Stream replication at egress ports. </b> \n
 *            Achieve stream redundancy by duplicating specific streams without changing the content of ingress frames.
 *
 * @section Purpose
 *          This example demonstrates how to replicate streams for redundancy.
 *
 * @section Description
 *          The host sends a stream without R-tag whose source MAC address is 22-11-33-ad-ff-ec to the switch's port1 \n
 *          to match the stream identification, the switch will add R-tag to the stream and replicate it to port2 and port3, \n
 *          the sequence number in the R-tag of the egress packets increases continuously from 0.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_cb_egress_replication(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_cb_egress_replication(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VA};
    /* config information */
    CbEnabledInfo_t cbEnabledInfo = {
        .status = ENABLED,
    };
    CbSidEnabledInfo_t cbSidEnabledInfo = {
        .status = ENABLED,
    };
    CbParseRtagEnabledInfo_t cbParseRtagEnabledInfo = {
        .port   = CVA_RTK_USER_PORT_1ST,
        .status = ENABLED};
    /* Stream identification */
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index         = 3u,
        .sidentryParam = {
            .carebits     = CVA_TSN_SELECTED_FILED_SRCADR,
            .srcMac       = {0x22, 0x11, 0x33, 0xad, 0xff, 0xec},
            .portMask     = CVA_RTK_USER_PMAP_1ST,
            .isValid      = ENABLED,
            .streamHandle = 7u,
        },
    };
    CbMiscInfo_t CbMiscInfo = {
        .index       = cbIngressPassiveSidInfo.index,
        .lutTabParam = {
            .indRcvEn    = DISABLED,
            .seqRcvEn    = DISABLED,
            .splitEn     = ENABLED,
            .splitDPM    = CVA_RTK_USER_PMAP_2ND | CVA_RTK_USER_PMAP_3RD,
            .genSeqNumEn = ENABLED,
            .activeEn    = DISABLED,
            .encEn       = ENABLED,
        },
    };
    /* Step1. Enable CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step2. Enable passive stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step3. Enable the R-tag parser to extract the sequence number from ingress packet */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));

    /* Step4. Set passive stream identification entry at port 1 to match packets */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step5. Enable splitting function and set destination port mask.
     *         Enable sequence number generation.
     *         Add an R-tag to egress packets.
     */
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &CbMiscInfo));

    /* Expected Behaviour: The switch will replicate the same streams to port2 and port3 for redundancy.
     *
     */
    return retVal;
}
/**@}*/ /* RTL907XCVA_CB_EGRESS_REPLICATION_EXAMPLE */
/**@}*/ /* RTL907XCVA_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */