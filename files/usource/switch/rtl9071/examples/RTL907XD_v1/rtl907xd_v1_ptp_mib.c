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

#include "rtkas_api_ptp_ext.h"
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup PTP PTP Example
 * @{
 */
/**
 * @defgroup RTL907XDV1_PTP_EXAMPLE RTL907XD_V1 PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XDV1_PTP_MIB_EXAMPLE PTP dump mib Example
 *  @brief <b>PTP Message MIB Counter</b> \n
 *  @section Purpose
 *              Dump PTP message MIB counters. \n\n
 *  @section Description
 *              There are many PTP messages. This example demonstrates how to get different PTP message MIB counters for transmission or reception.
 *  @{
 */

RtkApiRet dv1_ptp_mib(void);

RtkApiRet dv1_ptp_mib(void)
{
    RtkApiRet  ret       = RT_ERR_OK;
    RtkApiRet  apiresult = RT_ERR_OK;
    RtkApiRet  retPrint  = RT_ERR_OK;
    uint32     mib       = 0u;
    UnitChip_t unitChip;
    RtkPort    slavPort   = DV1_RTK_USER_PORT_1ST;
    RtkPort    masterPort = DV1_RTK_USER_PORT_2ND;
    uint32     domainIdx  = 0u;

    unitChip.chip = CHIP_907XD_V1;
    unitChip.unit = DV1_UNIT;

    PtpMib_t PtpMib;

    do
    {
        PtpMib.mib = 0u;
        /* Step 1. Get the different messages RX count */
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_rx_sync_mib_get_count(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" rx_sync_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_rx_follow_up_mib_get_count(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" rx_follow_up_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_rx_pdelay_request_mib_get_cont(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" rx_pdelay_request_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_rx_pdelay_response_mib_get_cont(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" rx_pdelay_response_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_rx_pdelay_resp_follow_up_mib_get(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" rx_pdelay_resp_follow_up_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 2. Get the different messages TX count */
        PtpMib.port     = masterPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_tx_sync_mib_get_count(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" tx_sync_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = masterPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_tx_follow_up_mib_get_count(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" tx_follow_up_mib_get: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_tx_pdelay_request_mib_get_cont(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" tx_pdelay_request_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_tx_pdelay_response_mib_get_cont(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" tx_pdelay_response_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_tx_pdelay_resp_follow_up_mib_get(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" tx_pdelay_resp_follow_up_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 3. Get the error count */
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_rx_ptp_packet_discard_mib_get_cont(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" rx_ptp_packet_discard_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_sync_receipt_timeout_mib_get_count(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" sync_receipt_timeout_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        PtpMib.port     = slavPort;
        PtpMib.domainid = domainIdx;
        apiresult       = rtk_ptp_pdelay_allowed_lost_response_mib_get_count(unitChip, &PtpMib);
        mib             = PtpMib.mib;
        CHK_FUN_CONTINUE((retPrint), (rtlglue_printf(" pdelay_allowed_lost_response_mib: %u\n" PTPMSGCOLOR, mib)));
        if(RT_ERR_OK != apiresult)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Expected Behaviour: Port 1 is slave port. Others are master port. Connect slave port(port1) to GM.
         * Get MIB from slave/master port success
         * */

    } while(0u == 1u);
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XDV1_PTP_EXAMPLE */
/**@}*/ /* RTL907XDV1_PTP_MIB_EXAMPLE */