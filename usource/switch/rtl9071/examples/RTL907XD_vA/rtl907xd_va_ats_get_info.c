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
 * @addtogroup RTL907XD_VA_ATS_GET_INFO_EXAMPLE ATS Get Info Example
 * @brief <b>ATS information</b> \n
 *           Get the ATS information.
 *
 * @section Purpose
 *          In order to get the ATS information.
 *
 * @section Description
 *          Gets the ATS informations including receive port number, transmit port number, maximum variation of clock offset, maximum deviation of clock rate,
 *          maximum recognition delay of arrival time, maximum processing delay, minimum processing delay for the ATS scheduler.
 * @{
 */

RtkApiRet dva_ats_get_info_example(void);

/* Function */

RtkApiRet dva_ats_get_info_example(void)
{
    RtkApiRet    retValue   = RT_ERR_OK;
    UnitChip_t   unitChip   = {0};
    AtsGetInfo_t atsGetInfo = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = DVA_UNIT;
        unitChip.chip = CHIP_907XD_VA;

        /* Step 2. Gets the ATS informations */
        CHK_FUN_CONTINUE(retValue, rtk_ats_get_info(unitChip, &atsGetInfo));

        /* Expected Behavior: Successfully obtained the information of ATS. */
    } while(0u == 1u);

    return retValue;
}

/**@}*/ /* RTL907XD_VA_ATS_GET_INFO_EXAMPLE */
/**@}*/ /* RTL907XD_VA_ATS_EXAMPLE */
/**@}*/ /* PSCM_EXAMPLE */
/**@}*/ /* EXAMPLE */
