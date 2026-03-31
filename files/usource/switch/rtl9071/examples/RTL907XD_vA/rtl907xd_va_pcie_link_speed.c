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
 * @defgroup RTL907XDVA_PCIE_SPEED_EXAMPLE PCIE Link Speed Example
 * @brief <b>Get PCIE Link Speed</b> \n
 *
 * @section Purpose
 *   Get the PCIE link speed
 *
 * @section Description
 *   Example of how to get the PCIE link speed on Port 9
 *
 *  @{
 */

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
RtkApiRet dva_get_pcie_linkspeed(UnitChip_t unitChip);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_get_pcie_linkspeed(UnitChip_t unitChip)
{
    RtkApiRet       retVal = RT_ERR_OK;
    PcieLinkspeed_t pcieLinkspeed;

    CHK_FUN_RET(retVal, dva_drv_memset(&pcieLinkspeed, 0, sizeof(PcieLinkspeed_t)));
    /*  Step 1.
     *  Get the PCIE link speed with the host.
     */
    pcieLinkspeed.pciePort = DVA_RTK_USER_PORT_9TH;
    CHK_FUN_RET(retVal, rtk_pcie_linkspeed_get(unitChip, &pcieLinkspeed));

    /* Expected Behaviour:
     *   The user can obtain information about the PCIE link speed with the host.
     */

    return retVal;
}
/**@}*/ /* RTL907XDVA_PCIE_SPEED_EXAMPLE */
/**@}*/ /* RTL907XDVA_PCIE_EXAMPLE */
/**@}*/ /* PCIE_EXAMPLE */
/**@}*/ /* EXAMPLE */
