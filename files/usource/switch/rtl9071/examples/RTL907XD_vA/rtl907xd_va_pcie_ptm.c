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
#include "rtkas_api_pcie.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @defgroup PCIE_EXAMPLE PCIE Example
 * @{
 */
/**
 * @defgroup RTL907XDVA_PCIE_EXAMPLE RTL907XD_VA PCIE Example
 * @{
 */
/**
 * @defgroup RTL907XDVA_PCIE_PTM_EXAMPLE PCIE PTM Sync to the Switch Example
 * @brief <b>Get the PCIE port for PTM Sync to the Switch.</b> \n
 *
 * @section Purpose
 *   Get the PCIE port for PTM Sync to the Switch
 *
 * @section Description
 *   Example of how to get PCIE port for PTM Sync to the Switch
 *
 *  @{
 */

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
RtkApiRet dva_get_pcie_ptm_sync_info(UnitChip_t unitChip);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_get_pcie_ptm_sync_info(UnitChip_t unitChip)
{
    RtkApiRet     retVal = RT_ERR_OK;
    PciePtmInfo_t pciePtmInfoGet;

    /*  Step 1.
     *  Get the PCIE port for PTM Sync to the Switch.
     */
    pciePtmInfoGet.pciePort = DVA_RTK_USER_PORT_9TH;
    CHK_FUN_RET(retVal, rtk_pcie_ptm_sync_info_get(unitChip, &pciePtmInfoGet));

    /* Expected Behaviour:
     *   The user can find out which PCIE port has enabled the PTM Sync to the Switch.
     */

    return retVal;
}
/**@}*/ /* RTL907XDVA_PCIE_PTM_EXAMPLE */
/**@}*/ /* RTL907XDVA_PCIE_EXAMPLE */
/**@}*/ /* PCIE_EXAMPLE */
/**@}*/ /* EXAMPLE */
