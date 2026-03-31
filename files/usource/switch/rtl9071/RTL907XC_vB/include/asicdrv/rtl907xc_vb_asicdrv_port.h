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

#ifndef CVB_ASICDRV_PORTSECURITY_H_
#define CVB_ASICDRV_PORTSECURITY_H_

#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_api_interface.h"
#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_api_ethernet_access.h"

/*define umac register for serdes*/
#define CVB_USX_CTRL_0_US0   (0x4A0060F4u)
#define CVB_USX_CTRL_1_US0   (0x4A0060F8u)
#define CVB_USX_STATUS_0_US0 (0x4A006100u)
#define CVB_USX_STATUS_1_US0 (0x4A006104u)
#define CVB_USX_CTRL_0_US1   (0x4A006108u)
#define CVB_USX_CTRL_1_US1   (0x4A00610Cu)
#define CVB_USX_STATUS_0_US1 (0x4A006114u)
#define CVB_USX_STATUS_1_US1 (0x4A006118u)
#define CVB_USX_CTRL_0_US2   (0x4A00611Cu)
#define CVB_USX_CTRL_1_US2   (0x4A006120u)
#define CVB_USX_STATUS_0_US2 (0x4A006128u)
#define CVB_USX_STATUS_1_US2 (0x4A00612Cu)

/* pin assignment 1 */
#define CVB_RTK_PIN_1_P4_RXC  (0x00000040u)
#define CVB_RTK_PIN_1_P4_RXER (0x00000080u)
#define CVB_RTK_PIN_1_P4_RXDV (0x00000100u)
#define CVB_RTK_PIN_1_P4_RXD0 (0x00000200u)
#define CVB_RTK_PIN_1_P4_RXD1 (0x00000400u)
#define CVB_RTK_PIN_1_P4_RXD2 (0x00000800u)
#define CVB_RTK_PIN_1_P4_RXD3 (0x00001000u)

/* pin assignment 2 */
#define CVB_RTK_PIN_2_P5_RXD0 (0x00004000u)
#define CVB_RTK_PIN_2_P5_RXD1 (0x00008000u)
#define CVB_RTK_PIN_2_P5_RXD2 (0x00010000u)
#define CVB_RTK_PIN_2_P5_RXD3 (0x00020000u)
#define CVB_RTK_PIN_2_P5_RXDV (0x00040000u)
#define CVB_RTK_PIN_2_P5_RXER (0x00080000u)
#define CVB_RTK_PIN_2_P5_RXC  (0x00100000u)
#define CVB_RTK_PIN_2_P9_RXD0 (0x08000000u)
#define CVB_RTK_PIN_2_P9_RXD1 (0x10000000u)
#define CVB_RTK_PIN_2_P9_RXD2 (0x20000000u)
#define CVB_RTK_PIN_2_P9_RXD3 (0x40000000u)
#define CVB_RTK_PIN_2_P9_RXDV (0x80000000u)

/* pin assignment 3 */
#define CVB_RTK_PIN_3_P9_RXER (0x00000001u)
#define CVB_RTK_PIN_3_P9_RXC  (0x00000002u)

EXTERN_API RtkApiRet cvb_drv_port_iso_mask_set(uint32 unit, uint32 port, uint32 mask);
EXTERN_API RtkApiRet cvb_drv_port_iso_mask_get(uint32 unit, uint32 port, uint32 *pMask);

EXTERN_API RtkApiRet cvb_drv_port_rgmii_delay_cell_set(uint32 unit, RtkPort port, uint8 delay);
EXTERN_API RtkApiRet cvb_drv_port_rgmii_delay_cell_get(uint32 unit, RtkPort port, uint8 *pDelay);

EXTERN_API RtkApiRet cvb_drv_port_rgmii_addll_set(uint32 unit, RtkPort port, RtkPortSpeed_e speed, uint8 delay);
EXTERN_API RtkApiRet cvb_drv_port_rgmii_addll_get(uint32 unit, RtkPort port, uint8 *pDelay);

EXTERN_API RtkApiRet cvb_drv_port_disable_rx_en(uint32 unit, uint32 portMask);
EXTERN_API RtkApiRet cvb_drv_port_eth_disable_rx_en(uint32 unit, uint32 portMask);
EXTERN_API RtkApiRet cvb_drv_port_covert_port_ability_to_reg(uint32 *regVal, PortMacAbility_t *pMacAbility);
EXTERN_API RtkApiRet cvb_drv_port_sub_port02_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port03_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port04_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port05_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port06_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port07_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port08_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port09_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port10_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port11_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_sub_port12_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet cvb_drv_port_eth_an_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility);
EXTERN_API RtkApiRet cvb_drv_port_eth_force_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility);
EXTERN_API RtkApiRet cvb_drv_port_xmii_ssc_err_chk(RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset);
EXTERN_API RtkApiRet cvb_drv_port_xmii_ssc_set(uint32 unit, RtkPort sysPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset);
EXTERN_API RtkApiRet cvb_drv_port_xmii_ssc_get(uint32 unit, RtkPort sysPort, RtkEnable *isEnable, uint32 *sequenceModeOffset, uint32 *phaseOffset);

#endif /*_RTL906X_ASICDRV_PORTSECURITY_H_*/
