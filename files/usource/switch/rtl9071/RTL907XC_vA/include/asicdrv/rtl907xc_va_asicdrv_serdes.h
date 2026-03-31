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

#ifndef CVA_ASICDRV_SERDES_H
#define CVA_ASICDRV_SERDES_H

#include "rtkas_common_types.h"

#include "rtkas_types.h"
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_api_serdes.h"

extern uint32 cva_g_rtkTimerSerdesMdioChannel;

#define CVA_RESERVE_MODE            (0x3u)
#define CVA_RGMII_MODE              (0x0u)
#define CVA_P10_MAC_FORCE_LINK_DOWN (0x841u)
#define CVA_P11_MAC_FORCE_LINK_DOWN (0x41u)
#define CVA_SDS_OFF_MODE            (0x1Fu)
#define CVA_SDS_MODE_SGMII          (0x2u)
#define CVA_SDS_MODE_HISGMII        (0x12u)
#define CVA_SDS_MODE_USXGMII        (0xDu)
#define CVA_SDS_MODE_5G_KR          (0x1Du)

/* SDS SPEED define */
#define CVA_SDS_SPEED_MASK  (0x3003u)
#define CVA_SDS_SPEED_5000M (0x1002u)
#define CVA_SDS_SPEED_2500M (0x1001u)
#define CVA_SDS_SPEED_1000M (0x2u)
#define CVA_SDS_SPEED_100M  (0x1u)
#define CVA_SDS_SPEED_10M   (0x0u)

/*UMAC Register Address*/
#define CVA_USX_CTRL_0_US0 (0x4A0060F4u)
#define CVA_USX_CTRL_0_US1 (0x4A006108u)
#define CVA_USX_CTRL_0_US2 (0x4A00611Cu)

#define CVA_USX_CTRL_1_US0 (0x4A0060F8u)
#define CVA_USX_CTRL_1_US1 (0x4A00610Cu)
#define CVA_USX_CTRL_1_US2 (0x4A006120u)

#define CVA_PURE_SGMII          (0x4u)
#define CVA_COMBO_SGMII_HISGMII (0x5u)
#define CVA_COMBO_USXGMII       (0x6u)

#define CVA_SDS_IN_ADDR                                                                                                                      \
    {                                                                                                                                        \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_SDS_A_IN_S0_R, 0xFFu, CVA_SDS_A_IN_US0_R, CVA_SDS_A_IN_US1_R, CVA_SDS_A_IN_US2_R \
    }
#define CVA_SDS_CON                                                                                              \
    {                                                                                                            \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_S0_CON, 0xFFu, CVA_US0_CON, CVA_US1_CON, CVA_US2_CON \
    }
#define CVA_DATA_READ_CHANNEL                                                                                                                    \
    {                                                                                                                                            \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_SDS_D_OUT_S0_R, 0xFFu, CVA_SDS_D_OUT_US0_R, CVA_SDS_D_OUT_US1_R, CVA_SDS_D_OUT_US2_R \
    }
#define CVA_DATA_WRITE_CHANNEL                                                                                                               \
    {                                                                                                                                        \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_SDS_D_IN_S0_R, 0xFFu, CVA_SDS_D_IN_US0_R, CVA_SDS_D_IN_US1_R, CVA_SDS_D_IN_US2_R \
    }
#define CVA_SDS_MDIO_ADDR                                                                                                                        \
    {                                                                                                                                            \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_S0_MDIO_ADDR_R, 0xFFu, CVA_US0_MDIO_ADDR_R, CVA_US1_MDIO_ADDR_R, CVA_US2_MDIO_ADDR_R \
    }
#define CVA_SDS_MDIO_CON                                                                                                             \
    {                                                                                                                                \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_S0_MDIO_CON, 0xFFu, CVA_US0_MDIO_CON, CVA_US1_MDIO_CON, CVA_US2_MDIO_CON \
    }
#define CVA_SDS_MDIO_DATA_OUTPUT                                                                                                                         \
    {                                                                                                                                                    \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_S0_MDIO_DATA_O_R, 0xFFu, CVA_US0_MDIO_DATA_O_R, CVA_US1_MDIO_DATA_O_R, CVA_US2_MDIO_DATA_O_R \
    }
#define CVA_SDS_MDIO_DATA_INPUT                                                                                                                          \
    {                                                                                                                                                    \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, CVA_S0_MDIO_DATA_I_R, 0xFFu, CVA_US0_MDIO_DATA_I_R, CVA_US1_MDIO_DATA_I_R, CVA_US2_MDIO_DATA_I_R \
    }

#define CVA_USX_CTRL_1_USX_ADDR                                                                                                 \
    {                                                                                                                           \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFFu, CVA_USX_CTRL_1_US0, CVA_USX_CTRL_1_US1, CVA_USX_CTRL_1_US2 \
    }

#define CVA_USX_CTRL_0_USX_ADDR                                                                                                 \
    {                                                                                                                           \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFFu, CVA_USX_CTRL_0_US0, CVA_USX_CTRL_0_US1, CVA_USX_CTRL_0_US2 \
    }

/* register field entry */
#define CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS_SPEED_LSP (0u)
#define CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS_SPEED_LEN (2u)
#define CVA_SDS_MODE_LSP                                  (0u)
#define CVA_SDS_MODE_LEN                                  (5u)

/* register field value */
#define CVA_NORMAL (0x0u)

EXTERN_API RtkApiRet cva_drv_sds_pcs_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet cva_drv_sds_aphy_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet cva_drv_sds_pcs_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
EXTERN_API RtkApiRet cva_drv_sds_aphy_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
EXTERN_API RtkApiRet cva_drv_sds_is_serdes_port(uint32 unit, RtkPort userPort);
EXTERN_API RtkApiRet cva_drv_sds_rst_otpv3_stk_config(void);
EXTERN_API RtkApiRet cva_drv_sds_eth_rst_otpv3_stk_config(void);
EXTERN_API RtkApiRet cva_drv_sds_p8_init(uint32 unit);
EXTERN_API RtkApiRet cva_drv_sds_p8_init_eth(void);
EXTERN_API RtkApiRet cva_drv_sds_port_speed_get(uint32 unit, RtkPort sysPort, uint32 *portSpeed);
EXTERN_API RtkApiRet cva_drv_sds_chk_port_range(uint32 unit, RtkPort userPort);
#endif
