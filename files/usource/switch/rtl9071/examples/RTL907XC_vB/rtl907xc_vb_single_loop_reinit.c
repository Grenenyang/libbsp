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
#include "rtkas_api_single_loop.h"
#include "rtkas_api_spi_flash.h"
#include "rtkas_api_reset.h"

/** @addtogroup SINGLE_LOOP_EXAMPLE Single Loop Example
 *  @{
 */

/**
 * @addtogroup RTL907XCVB_SINGLE_LOOP_EXAMPLE RTL907XC_VB Single Loop Example
 * @{
 */

/**
 * @defgroup RTL907XCVB_SINGLE_LOOP_REINIT_EXAMPLE Single Loop Re-initialization Example
 *
 * @brief <b>Single Loop Re-initialization</b> \n
 *        When the recovery state of all switches in Single Loop is not set to enabled, it will not recover the topology to the default \n
 *        state automatically once the topology resumes. In this case, you can use this API to re-initialize Single Loop to recover \n
 *        all the ports to the default state.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the re-initialization of Single Loop to recover all ports to their default state.
 *
 * @section Description
 * This function re-initializes Single Loop by calling the appropriate API and verifies the expected behavior.
 *
 * @image html sl/single_loop_reinit.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_single_loop_reinit(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_single_loop_reinit(void)
{
    RtkApiRet  ret      = RT_ERR_OK;
    uint32     unit     = CVB_UNIT;
    UnitChip_t unitChip = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VB;
    do
    {
        /** Step 1. Re-initialize Single Loop */
        CHK_FUN_CONTINUE(ret, rtk_sl_reinit(unitChip));
        /** Expected Behaviour: Single Loop will be re-initialized to recover the ports to default state. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XCVB_SINGLE_LOOP_REINIT_EXAMPLE */
/**@}*/ /* RTL907XCVB_SINGLE_LOOP_EXAMPLE */
/**@}*/ /* SINGLE_LOOP_EXAMPLE */