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
#include "rtkas_api_mirror.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup MIRROR Mirror Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_MIRROR_EXAMPLE RTL907XC_VA Mirror Example
 * @{
 */
/**
 * @addtogroup RTL907XD_VA_MIRROR_RESET_EXAMPLE Mirror Reset Example
 * @brief <b>Reset Mirroring</b> \n
 *           Reset the mirroring funcion 1 or 2.
 *
 * @section Purpose
 *          Clear the previous settings for the mirroring.
 *
 * @section Description
 *          Disable mirroring function for mirroring function 1 or mirroring function 2.
 * @{
 */
RtkApiRet dva_mirroring_reset(void);

/* Function */

RtkApiRet dva_mirroring_reset(void)
{
    RtkApiRet         ret       = RT_ERR_OK;
    UnitChip_t        unitChip  = {0};
    MirrorResetInfo_t resetInfo = {0};

    do
    {
        /* Step1. Configure the unit and chip id */
        unitChip.unit = DVA_UNIT;
        unitChip.chip = CHIP_907XD_VA;

        /* Step2. Reset mirror function 1*/
        resetInfo.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_reset(unitChip, &resetInfo));

        /* Step3. Reset mirror function 2*/
        resetInfo.mirrorNo = 2u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_reset(unitChip, &resetInfo));

        /* Expected Behavior: Mirroring function 1 and 2 are all disable */
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XD_VA_MIRROR_RESET_EXAMPLE */
/**@}*/ /* RTL907XD_VA_MIRROR_EXAMPLE */
/**@}*/ /* MIRROR */
/**@}*/ /* EXAMPLE */
