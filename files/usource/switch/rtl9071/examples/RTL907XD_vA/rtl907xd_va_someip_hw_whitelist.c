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

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_someip.h"
#endif
#include "rtkas_api_someip.h"
#include "rtl907xd_va_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup SOMEIP_EXAMPLE SOME/IP Firewall Example
 * @{
 */

/**
 * @defgroup RTL907DVA_SOMEIP_EXAMPLE RTL907XD_VA SOME/IP Firewall Example
 * @{
 */

/**
 * @defgroup RTL907DVA_SOMEIP_HW_WHITELIST_EXAMPLE Set and Get the SOME/IP Firewall HW Whitelist Example
 * @{
 */

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
RtkApiRet dva_exam_someip_set_get_hw_whitelist(void);

/******************************************************************************/
/* Functions Definition                                                       */
/******************************************************************************/
/**
 * @brief <b>Set and get the SOME/IP firewall HW whitelist</b>
 *
 * @section Purpose
 *   The switch provides a way to set and get the SOME/IP firewall HW whitelist.
 *
 * @section Description
 *   This example shows how to set and get the SOME/IP firewall HW whitelist.
 */
RtkApiRet dva_exam_someip_set_get_hw_whitelist(void)
{
    RtkApiRet           ret                                = RT_ERR_OK;
    someipHWWhitelist_t someipHWWhitelist                  = {0};
    uint32              whitelist[SOMEIP_HW_WHITELIST_NUM] = {0};
    UnitChip_t          unitChip                           = {0};

    /*Configure the unit and chip ID*/
    unitChip.chip = CHIP_907XD_VA;
    unitChip.unit = DVA_UNIT;

    /* Step 1.  Set SOME/IP firewall HW whitelist information*/
    /* Set index of HW whitelist to 1*/
    someipHWWhitelist.idx = 1u;
    /* Set HW whitelist to 0x13EC0001*/
    someipHWWhitelist.whitelist = 0x13EC0001u;

    /* Set to stacking unit and SOME/IP firewall HW whitelist information*/
    ret = rtk_someip_set_hw_whitelist(unitChip, &someipHWWhitelist);

    /* Step 2.  Get all the SOME/IP firewall HW whitelist*/
    /* Get all the SOME/IP firewall HW whitelist*/
    ret = rtk_someip_get_hw_whitelist(unitChip, whitelist);

    /* Expected Behavior:
     *   Users can set the SOME/IP firewall HW whitelist settings successfully.
     *   After setting, the user can correctly get the SOME/IP firewall HW whitelist settings.
     */

    return ret;
}

/**@}*/ /* RTL907DVA_SOMEIP_HW_WHITELIST_EXAMPLE */
/**@}*/ /* RTL907DVA_SOMEIP_EXAMPLE */
/**@}*/ /* SOMEIP_EXAMPLE */
/**@}*/ /* EXAMPLE */
