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

#ifndef DVA_RTK_API_PCIE_H__
#define DVA_RTK_API_PCIE_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xd_va_api.h"
#include "rtkas_api_pcie.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define DVA_PCIe_OTP_START_ADDR                   0x180u
#define DVA_PCIE_OTP_CONFIG_SIZE                  (256u)
#define DVA_PCIE_OTP_CONFIG_VALID_CTRL            (1u)
#define DVA_PCIE_OTP_CONFIG_BLOCK_NUM             (4u)
#define DVA_PCIE_OTP_CONFIG_BLOCK_SIZE            (DVA_PCIE_OTP_CONFIG_SIZE / DVA_PCIE_OTP_CONFIG_BLOCK_NUM)
#define DVA_PCIE_OTP_P9_OFFSET                    (0u)
#define DVA_PCIE_OTP_P11_OFFSET                   (19u)
#define DVA_PCIE_OTP_1ST_CONFIG_USED_VALID_OFFSET (4u)
#define DVA_PCIE_OTP_1ST_CONFIG_USED_VALID_MASK   (0xFu)

/* PCIe config dummy register */
#define DVA_PCIE_SW_CONFIG_REG               (DVA_HW_MISC_DMY31)
#define DVA_PCIE_P9_DUMP_LINK_STATUS_LSP     19u
#define DVA_PCIE_P9_DUMP_LINK_STATUS_LEN     1u
#define DVA_PCIE_P11_DUMP_LINK_STATUS_LSP    20u
#define DVA_PCIE_P11_DUMP_LINK_STATUS_LEN    1u
#define DVA_PCIE_P9_LINK_STATUS_REG          (DVA_HW_MISC_DMY25)
#define DVA_PCIE_P11_LINK_STATUS_REG         (DVA_HW_MISC_DMY35)
#define DVA_PCIE_SW_CONFIG_LINK_SPEED_MASK   0x30000u
#define DVA_PCIE_SW_CONFIG_LINK_SPEED_OFFSET 16u
#define DVA_PCIE_SW_CONFIG_LINK_STATUS_MASK  0xFFFFu
#define DVA_PCIE_LINK_UP_OK                  0xB183u

#define DVA_PCIE_P9_CTRL_REG      DVA_PORT9_PORT_CTRL1
#define DVA_PCIE_P11_CTRL_REG     DVA_PORT11_PORT_CTRL1
#define DVA_PCIE_LINK_CAP_LSP     23u
#define DVA_PCIE_LINK_CAP_LEN     2u
#define DVA_PCIE_DISABLE_REEQ_LSP 25u
#define DVA_PCIE_DISABLE_REEQ_LEN 1u
#define DVA_PCIE_2LANE_ENABLE_LSP 27u
#define DVA_PCIE_2LANE_ENABLE_LEN 1u

#define DVA_PCIE_PTM_CTRL_REG              0x4A600498u
#define DVA_PCIE_PTM_CTRL_REG_PORT_SEL_LSP (0u)
#define DVA_PCIE_PTM_CTRL_REG_PORT_SEL_LEN (1u)
#define DVA_PCIE_PTM_PORT_SEL_P09          (1u)
#define DVA_PCIE_PTM_PORT_SEL_P11          (0u)

#define DVA_PCIE_TIME_MAX_US (100000u)
#define DVA_PCIE_TIME_MIN_US (0u)

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
EXTERN_API RtkApiRet dva_pcie_parm_get(uint32 unit, RtkPort portId, PcieParm_t *pcieParam);
EXTERN_API RtkApiRet dva_pcie_linkspeed_get(uint32 unit, RtkPort portId, uint32 *speed);
EXTERN_API RtkApiRet dva_pcie_lane_get(uint32 unit, RtkPort portId, uint32 *pcie2LaneEn);
EXTERN_API RtkApiRet dva_pcie_ptm_sync_info_get(uint32 unit, RtkPort portId, uint32 *pciePtmSync);
#endif /* DVA_RTK_API_PCIE_H__ */