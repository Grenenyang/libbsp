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

/**
 * @addtogroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_CB_EXAMPLE RTL907XD_VA CB Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_CB_RESET_REG_AND_TABLE_EXAMPLE CB Reset Register And Table Example
 *
 * @brief <b> Reset CB control register, passive stream identification table, MISC table, individual recovery table and sequence recovery table. </b>
 *
 * @section Purpose
 *          This example demonstrates how to reset CB control register and all entries.
 *
 * @section Description
 *          This example can reset CB control register, stream identification table (96 entries), \n
 *          MISC table (96 entries), individual recovery table (96 entries), \n
 *          and sequence recovery table (48 entries) to their default values. \n
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_cb_reset_reg_andtbl(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_cb_reset_reg_andtbl(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};

    /* Step 1. Reset CB */
    CHK_FUN_RET(retVal, rtk_cb_reset(unitChip));

    /* Expected Behaviour:
     * Reset the CB control register, stream identification table (96 entries), MISC table (96 entries),
     * individual recovery table (96 entries),
     * and sequence recovery table (48 entries) to their default values.
     */
    return retVal;
}
/**@}*/ /* RTL907XDVA_CB_RESET_REG_AND_TABLE_EXAMPLE */
/**@}*/ /* RTL907XDVA_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */
