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
 * @defgroup RTL907XCVB_PTP_EXAMPLE RTL907XC_VB PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XCVB_PTP_OVERWRITE_MY_CLOCK_ID_EXAMPLE PTP overwrite clockID Example
 *  @brief <b>Overwrite My Clock Identity</b> \n
 *  @section Purpose
 *              If there is a requirement to change the clockIdentity to that of the GM, the switch can support this customized option. \n\n
 *  @section Description
 *              This is a customized option to change the clockIdentity in sync/follow-up packets. \n
 *              According to 802.1AS 2020, the clockIdentity in sync/follow-up packets is the self clockIdentity. \n
 *  @{
 */

RtkApiRet cvb_ptp_overwrite_myclockID(void);

RtkApiRet cvb_ptp_overwrite_myclockID(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t unitChip;

    unitChip.chip = CHIP_907XC_VB;
    unitChip.unit = CVB_UNIT;

    PtpGlenable_t       ptpGlenable;
    PtpOverwritePtpid_t ptpOverwritePtpid;

    do
    {
        /* Step 1. Disable PTP to configure. */
        ptpGlenable.glenable = 0u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_glenable(unitChip, &ptpGlenable));

        /* Step 2* Enable overwritting clockID with my clockID in sync/follow up. */
        ptpOverwritePtpid.enable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_overwrite_ptpid_set(unitChip, &ptpOverwritePtpid));

        /* Step 3. Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_glenable(unitChip, &ptpGlenable));

        /* Step 4* Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), rtk_ptp_reinitprotocol(unitChip));

    } while(0u == 1u);
    /* Expected Behaviour:
     * 1. The clockID in Sync/Follow_Up sent from DUT's master port is changed to my clockID.
     * */
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XCVB_PTP_EXAMPLE */
/**@}*/ /* RTL907XCVB_PTP_OVERWRITE_MY_CLOCK_ID_EXAMPLE */