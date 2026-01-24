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
#include "rtkas_api_stp.h"

/** @addtogroup STP_EXAMPLE STP Example
 *  @{
 */

/**
 * @addtogroup RTL907XDV1_STP_EXAMPLE RTL907XD_V1 STP Example
 * @{
 */

/**
 * @addtogroup RTL907XDV1_STP_MIGRATION_CHECK_EXAMPLE Migration Check Set Example
 *
 * @brief <b>Migration Check Set</b> \n
 *        Trigger STP migration check.
 *
 * @section Purpose
 * The purpose of this function is to demonstrate how to trigger an STP migration check.
 *
 * @section Description
 * This function triggers an STP migration check on a specified port and verifies the expected behavior.
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_stp_migration_check_set(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_stp_migration_check_set(void)
{
    RtkApiRet                  retVal                      = RT_ERR_OK;
    uint32                     unit                        = DV1_UNIT;
    StpPortMigrationCheckSet_t setStpPortMigrationCheckSet = {0u};
    UnitChip_t                 unitChip                    = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;
    do
    {
        /* Step 1. Trigger STP migratoin check. */
        setStpPortMigrationCheckSet.portId = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(retVal, rtk_stp_port_migration_check_set(unitChip, &setStpPortMigrationCheckSet));
        /* Expected Behaviour: Trigger port migration check of port 1 successfully. */

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907XDV1_STP_MIGRATION_CHECK_EXAMPLE */
/**@}*/ /* RTL907XDV1_STP_EXAMPLE */
/**@}*/ /* STP_EXAMPLE */
