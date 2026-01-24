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

#ifndef CVB_RTK_API_PCIE_H__
#define CVB_RTK_API_PCIE_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xc_vb_api.h"
#include "rtkas_api_pcie.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define CVB_PCIe_OTP_START_ADDR        0x1200u
#define CVB_PCIE_OTP_CONFIG_SIZE       (576u)
#define CVB_PCIE_OTP_CONFIG_VALID_CTRL (4u)
#define CVB_PCIE_OTP_CONFIG_BLOCK_NUM  (16u)
#define CVB_PCIE_OTP_CONFIG_BLOCK_SIZE (CVB_PCIE_OTP_CONFIG_SIZE / CVB_PCIE_OTP_CONFIG_BLOCK_NUM)

/* PCIe config dummy register */
#define CVB_PCIE_DUMP_LINK_STATUS_REG  (CVB_HW_MISC_DMY_RESIGSER_25)
#define CVB_PCIE_SW_CONFIG_LINK_SPEED  0x30000
#define CVB_PCIE_SW_CONFIG_LINK_STATUS 0x0FFFF
#define CVB_PCIE_LINK_UP_OK            0xB183

#define CVB_PCIE_SW_CONFIG_REG        (CVB_HW_MISC_DMY_RESIGSER_31)
#define CVB_PCIE_DUMP_LINK_STATUS_LSP 19u
#define CVB_PCIE_DUMP_LINK_STATUS_LEN 1u
#define CVB_PCIE_DISABLE_REEQ_LSP     16u
#define CVB_PCIE_DISABLE_REEQ_LEN     1u
#define CVB_PCIE_LINK_CAP_LSP         0u
#define CVB_PCIE_LINK_CAP_LEN         2u

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet cvb_pcie_parm_get(uint32 unit, cvb_RtkPort portId, PcieParm_t *pcieParam);
EXTERN_API RtkApiRet cvb_pcie_linkspeed_get(uint32 unit, cvb_RtkPort portId, uint32 *speed);

#endif /* CVB_RTK_API_PCIE_H__ */