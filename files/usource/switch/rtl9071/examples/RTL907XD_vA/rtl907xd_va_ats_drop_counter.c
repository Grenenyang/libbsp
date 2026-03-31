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
#include "rtkas_api_ats.h"
#include "rtkas_api_ptp.h"
#include "rtkas_api_psfp.h"
#include "rtkas_api_qos.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup PSCM_EXAMPLE PSCM(QCI/PSFP & QCR/ATS) Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_ATS_EXAMPLE RTL907XD_VA ATS Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_ATS_DROP_COUNTER_EXAMPLE ATS Drop Counter Example
 * @brief <b>ATS drop counter</b> \n
 *           Get the ats packet drop counter for a specific port.
 *
 * @section Purpose
 *          In order to get the ATS packet drop counter for a specific port when ATS packets are dropped due to traffic limitations on that port.
 *
 * @section Description
 *          When a specific port receives ATS packets that are dropped due to traffic limitations, the user can use the corresponding SDK to obtain the count of dropped packets.
 * @{
 */

RtkApiRet dva_ats_drop_counter(void);

/* Function */

RtkApiRet dva_ats_drop_counter(void)
{
    RtkApiRet             retValue            = RT_ERR_OK;
    UnitChip_t            unitChip            = {0};
    AtsDropCounterGet_t   atsDropCounterGet   = {0};
    AtsDropCounterClear_t atsDropCounterClear = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = DVA_UNIT;
        unitChip.chip = CHIP_907XD_VA;

        /* Step 2. Get the rx port ats packet drop counter */
        atsDropCounterGet.port = DVA_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retValue, rtk_ats_drop_counter_get(unitChip, &atsDropCounterGet));

        /* Step 3. Clear the rx port ats packet drop counter */
        atsDropCounterGet.port = DVA_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retValue, rtk_ats_drop_counter_clear(unitChip, &atsDropCounterClear));

        /* Expected Behavior: Obtain the count of ATS packets dropped on port 1 and reset the counter to zero. */
    } while(0u == 1u);

    return retValue;
}

/**@}*/ /* RTL907XD_VA_ATS_DROP_COUNTER_EXAMPLE */
/**@}*/ /* RTL907XD_VA_ATS_EXAMPLE */
/**@}*/ /* PSCM_EXAMPLE */
/**@}*/ /* EXAMPLE */
