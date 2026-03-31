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
 * @defgroup RTL907XCVA_PTP_EXAMPLE RTL907XC_VA PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XCVA_PTP_NO_SYNC_RECOVERY_EXAMPLE PTP no sync recovery Example
 *  @brief <b>PTP No Sync Recovery</b> \n
 *  @section Purpose
 *              Use a default time record and begin sending Sync messages with a constant preciseOriginTimestamp and advancing correction on its Master role ports. \n\n
 *  @section Description
 *              According to the “Automotive Ethernet AVB Functional and Interoperability Specification,” \n
 *              it defines a behavior: “On startup, if an AED-B (Automotive Ethernet Bridge device) does not receive a Sync message on a slave port within a configurable time, \n
 *              then it shall use a default time record and begin sending Sync messages with a constant preciseOriginTimestamp and advancing correction on its Master role ports.” \n
 *              The switch supports this feature to meet the standard. \n
 *              However, if there is a requirement to disable this feature, so that no Sync messages are sent in the absence of a Sync, the switch also supports customization to disable this feature.
 *  @{
 */

RtkApiRet cva_ptp_no_sync_recovery(void);

RtkApiRet cva_ptp_no_sync_recovery(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t unitChip;

    unitChip.chip = CHIP_907XC_VA;
    unitChip.unit = CVA_UNIT;

    PtpGlenable_t              ptpGlenable;
    PtpSkipfirstbootrecovery_t ptpSkipfirstbootrecovery;
    PtpSilenceifneversynced_t  ptpSilenceifneversynced;

    do
    {
        /* Step 1. Disable PTP to configure. */
        ptpGlenable.glenable = 0u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 2. Enable no recovery after firstbootuptimetarget */
        ptpSkipfirstbootrecovery.enable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_skipfirstbootrecovery(unitChip, &ptpSkipfirstbootrecovery)));
        ptpSilenceifneversynced.domainid = 0u;
        ptpSilenceifneversynced.enable   = 1u;
        /* Step 3. Enable if dut never get sync from slave port, no recovery on domain 0. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));
        ptpSilenceifneversynced.domainid = 1u;
        ptpSilenceifneversynced.enable   = 1u;
        /* Step 4. Enable if dut never get sync from slave port, no recovery on domain 1. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));
        ptpSilenceifneversynced.domainid = 2u;
        ptpSilenceifneversynced.enable   = 1u;
        /* Step 5. Enable if dut never get sync from slave port, no recovery on domain 2. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_silenceifneversynced(unitChip, &ptpSilenceifneversynced)));

        /* Step 6. Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), (rtk_ptp_set_glenable(unitChip, &ptpGlenable)));

        /* Step 7. Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), (rtk_ptp_reinitprotocol(unitChip)));

    } while(0u == 1u);

    /* Expected Behaviour:
     * 1. If no sync/follow_up received from DUT slave port there is no sync/follow_up sent from DUT master port.
     * */
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XCVA_PTP_EXAMPLE */
/**@}*/ /* RTL907XCVA_PTP_NO_SYNC_RECOVERY_EXAMPLE */