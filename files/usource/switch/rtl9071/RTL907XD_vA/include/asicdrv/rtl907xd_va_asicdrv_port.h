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

#ifndef DVA_ASICDRV_PORTSECURITY_H_
#define DVA_ASICDRV_PORTSECURITY_H_

#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_api_interface.h"
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api_ethernet_access.h"

/*define umac register for serdes*/
#define DVA_USX_CTRL_0_US0   (0x4A0060F4u)
#define DVA_USX_CTRL_1_US0   (0x4A0060F8u)
#define DVA_USX_STATUS_0_US0 (0x4A006100u)
#define DVA_USX_STATUS_1_US0 (0x4A006104u)
#define DVA_USX_CTRL_0_US1   (0x4A006108u)
#define DVA_USX_CTRL_1_US1   (0x4A00610Cu)
#define DVA_USX_STATUS_0_US1 (0x4A006114u)
#define DVA_USX_STATUS_1_US1 (0x4A006118u)
#define DVA_USX_CTRL_0_US2   (0x4A00611Cu)
#define DVA_USX_CTRL_1_US2   (0x4A006120u)
#define DVA_USX_STATUS_0_US2 (0x4A006128u)
#define DVA_USX_STATUS_1_US2 (0x4A00612Cu)

/* pin assignment 1 */
#define DVA_RTK_PIN_1_P4_RXC  (0x00000040u)
#define DVA_RTK_PIN_1_P4_RXER (0x00000080u)
#define DVA_RTK_PIN_1_P4_RXDV (0x00000100u)
#define DVA_RTK_PIN_1_P4_RXD0 (0x00000200u)
#define DVA_RTK_PIN_1_P4_RXD1 (0x00000400u)
#define DVA_RTK_PIN_1_P4_RXD2 (0x00000800u)
#define DVA_RTK_PIN_1_P4_RXD3 (0x00001000u)

/* pin assignment 2 */
#define DVA_RTK_PIN_2_P5_RXD0 (0x00004000u)
#define DVA_RTK_PIN_2_P5_RXD1 (0x00008000u)
#define DVA_RTK_PIN_2_P5_RXD2 (0x00010000u)
#define DVA_RTK_PIN_2_P5_RXD3 (0x00020000u)
#define DVA_RTK_PIN_2_P5_RXDV (0x00040000u)
#define DVA_RTK_PIN_2_P5_RXER (0x00080000u)
#define DVA_RTK_PIN_2_P5_RXC  (0x00100000u)
#define DVA_RTK_PIN_2_P9_RXD0 (0x08000000u)
#define DVA_RTK_PIN_2_P9_RXD1 (0x10000000u)
#define DVA_RTK_PIN_2_P9_RXD2 (0x20000000u)
#define DVA_RTK_PIN_2_P9_RXD3 (0x40000000u)
#define DVA_RTK_PIN_2_P9_RXDV (0x80000000u)

/* pin assignment 3 */
#define DVA_RTK_PIN_3_P9_RXER (0x00000001u)
#define DVA_RTK_PIN_3_P9_RXC  (0x00000002u)

/*define SERDES configuration*/
#define DVA_SDS_MODE_SG         (0x2u)
#define DVA_SDS_MODE_HSG        (0x12u)
#define DVA_SDS_MODE_USXG       (0xDu)
#define DVA_SDS_MODE_KR_XFI_5G  (0x1Du)
#define DVA_SDS_MODE_KR_XFI_10G (0x1Au)

#define DVA_MAC_MODE (0x0u)
#define DVA_PHY_MODE (0x1u)

/*boot status*/
#define DVA_BOOT_UP_FLASHLESS_ETH       (0xB7u)
#define DVA_BOOT_UP_FULL_INITIZED       (0xBFu)
#define DVA_CPU_DMY_1_BOOTUP_STATUS_LSP (24u)
#define DVA_CPU_DMY_1_BOOTUP_STATUS_LEN (8u)
/******************************************************************************/
/* PORT SPEED Defines                                                         */
/******************************************************************************/
#define DVA_SPEED_10000MBPS (10000u)
#define DVA_SPEED_5000MBPS  (5000u)
#define DVA_SPEED_2500MBPS  (2500u)
#define DVA_SPEED_1000MBPS  (1000u)
#define DVA_SPEED_100MBPS   (100u)
#define DVA_SPEED_10MBPS    (10u)

#define RTK_MODE_INTERFACE_COMBO_VALUE                              \
    {                                                               \
        COMBO_INTF_DISB,              /* [00] Pulldown */           \
            COMBO_INTF_XMII_MII,      /* [01] MII PHY  */           \
            COMBO_INTF_T1PHY_MII,     /* [02] Fast Ethernet */      \
            COMBO_INTF_T1PHY_MII,     /* [03] 100BASE-T1 PHY */     \
            COMBO_INTF_SGMII_HISGMII, /* [04] SGMII */              \
            COMBO_INTF_T1PHY_GMII,    /* [05] GMII */               \
            COMBO_INTF_XMII_RGMII,    /* [06] RGMII */              \
            COMBO_INTF_XMII_MII,      /* [07] MII */                \
            COMBO_INTF_XMII_RMII,     /* [08] RMII */               \
            COMBO_INTF_XMII_RMII,     /* [09] RMII PHY */           \
            COMBO_INTF_XMII_RMII,     /* [10] RMII MAC */           \
            COMBO_INTF_XMII_MII,      /* [11] MII MAC */            \
            COMBO_INTF_T1PHY_GMII,    /* [12] 1000BASE-T1 PHY */    \
            COMBO_INTF_DISB,          /* [13] RESERVED Mode */      \
            COMBO_INTF_SGMII_HISGMII, /* [14] HISGMII */            \
            COMBO_INTF_XGMII_USXG,    /* [15] USXGMII */            \
            COMBO_INTF_XGMII_USXG,    /* [16] 5GKR */               \
            COMBO_INTF_XGMII_USXG,    /* [17] 10GKR */              \
            COMBO_INTF_XGMII_USXG,    /* [18] 5GXFI */              \
            COMBO_INTF_XGMII_USXG,    /* [19] 10GXFI */             \
            COMBO_INTF_PCIE_GMII,     /* [20] PCIE 1G MAC GMII */   \
            COMBO_INTF_PCIE_GMII,     /* [21] PCIE 5G MAC GMII */   \
            COMBO_INTF_PCIE_XGMII,    /* [22] PCIE 5G MAC XGMII */  \
            COMBO_INTF_PCIE_XGMII,    /* [23] PCIE 10G MAC XGMII */ \
            COMBO_INTF_DISB,          /* [24] RESERVED Mode */      \
            COMBO_INTF_SPI_MACPHY,    /* [25] MACPHY */             \
            COMBO_INTF_T1PHY_MII,     /* [26] 10BASE-T1 PHY */      \
            COMBO_INTF_T1PHY_MII,     /* [27] 10BASE-T1S PHY */     \
            COMBO_INTF_T1PHY_GMII,    /* [28] Giga Ethernet */      \
            COMBO_INTF_DISB           /*EM_MODE_EXT_END*/           \
    }

EXTERN_API RtkApiRet dva_drv_port_iso_mask_set(uint32 unit, uint32 port, uint32 mask);
EXTERN_API RtkApiRet dva_drv_port_iso_mask_get(uint32 unit, uint32 port, uint32 *pMask);

EXTERN_API RtkApiRet dva_drv_port_rgmii_delay_cell_set(uint32 unit, dva_RtkPort port, uint8 delay);
EXTERN_API RtkApiRet dva_drv_port_rgmii_delay_cell_get(uint32 unit, dva_RtkPort port, uint8 *pDelay);

EXTERN_API RtkApiRet dva_drv_port_rgmii_addll_set(uint32 unit, dva_RtkPort port, RtkPortSpeed_e speed, uint8 delay);
EXTERN_API RtkApiRet dva_drv_port_rgmii_addll_get(uint32 unit, dva_RtkPort port, uint8 *pDelay);

EXTERN_API RtkApiRet dva_drv_port_disable_rx_en(uint32 unit, uint32 portMask);
EXTERN_API RtkApiRet dva_drv_port_eth_disable_rx_en(uint32 unit, uint32 portMask);
EXTERN_API RtkApiRet dva_drv_port_covert_port_ability_to_reg(uint32 *regVal, PortMacAbility_t *pMacAbility);
EXTERN_API RtkApiRet dva_drv_port_sub_port01_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port02_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port03_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port04_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port05_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port06_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port07_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port08_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port09_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port10_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port11_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_sub_port12_mode_get(uint32 unit, RtkModeExt_e *mode);
EXTERN_API RtkApiRet dva_drv_port_eth_force_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility);
EXTERN_API RtkApiRet dva_drv_port_eth_an_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility);
EXTERN_API RtkApiRet dva_drv_port_eth_get_an_ability(uint32 unit, RtkModeExt_e mode, RtkPort userPort, uint16 *autoNegoCapability);
EXTERN_API RtkApiRet dva_drv_port_xmii_ssc_err_chk(RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset);
EXTERN_API RtkApiRet dva_drv_port_mactop_ecoclk_set(uint32 unit, RtkPort userPort, uint32 status);
EXTERN_API RtkApiRet dva_drv_port_combo_intf_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode);
EXTERN_API RtkApiRet dva_drv_port_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 speed, uint32 linkStatus);
EXTERN_API RtkApiRet dva_drv_xmii_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode);
EXTERN_API RtkApiRet dva_drv_port_polling_tx_nopkt(uint32 unit, uint32 userPort, bool *result);
EXTERN_API RtkApiRet dva_drv_xmii_mode_chk(RtkModeExt_e portMode);
EXTERN_API RtkApiRet dva_drv_t1_err_chk(RtkPort userPort, RtkModeExt_e portMode);
EXTERN_API RtkApiRet dva_drv_fe_err_chk(RtkPort userPort, RtkModeExt_e portMode);
EXTERN_API RtkApiRet dva_drv_port_xmii_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 speed, uint32 linkStatus);
#endif /*_RTL906X_ASICDRV_PORTSECURITY_H_*/
