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

#ifndef DV1_ASICDRV_SERDES_H
#define DV1_ASICDRV_SERDES_H

#include "rtkas_common_types.h"

#include "rtkas_types.h"
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_serdes.h"
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_regField_list.h"

/******************************************************************************/
/* SERDES mode Defines                                                        */
/******************************************************************************/
/* PORT#_PORT_CTRL0 */
#define DV1_INTF_SG_HSG (0x6u)
#define DV1_INTF_USXG   (0x7u)

#define DV1_SDS_MODE_RESERVE    (0x0u)
#define DV1_SDS_MODE_SG         (0x2u)
#define DV1_SDS_MODE_HSG        (0x12u)
#define DV1_SDS_MODE_USXG       (0xDu)
#define DV1_SDS_MODE_KR_XFI_5G  (0x1Du)
#define DV1_SDS_MODE_KR_XFI_10G (0x1Au)
#define DV1_SDS_OFF_MODE        (0x1Fu)
/******************************************************************************/
/* SERDES Condition Defines                                                   */
/******************************************************************************/
#define DV1_IS_WRAP_SDS          (0x1u)
#define DV1_IS_SDS_PORT          (0x1u)
#define DV1_IS_NOT_SDS           (0x0u)
#define DV1_IS_NOT_ETH_ACCESS    (0x0u)
#define DV1_IS_ETH_ACCESS        (0x1u)
#define DV1_WORKAROUND_READY     (0x1u)
#define DV1_WORKAROUND_NOT_READY (0x0u)
/******************************************************************************/
/* SERDES Register Field Defines                                              */
/******************************************************************************/
#define DV1_INDIRECT_ACCESS_SDS_INDEX_LSP   (21u)
#define DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP (16u)

/******************************************************************************/
/* Register Value Defines                                                     */
/******************************************************************************/
/* PORT#_INDIRECT_ACCESS_SDS_PCS_CTL */
#define DV1_WRITE_DATA (0x1u)
#define DV1_READ_DATA  (0x0u)

/******************************************************************************/
/* SERDES Count Defines                                                       */
/******************************************************************************/
#define DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC (10u)
#define DV1_SGMII_DOMAIN4_SDS_PORT_BITMASK (0x7FFFu)
#define DV1_WRAP_SDS_PORT_BITMASK          (0x1F0u)
#define DV1_SDS_START_PORT_OFFSET          (4u)
#define DV1_WRAP_SDS_START_PORT            (4u)
#define DV1_WRAP_SDS_END_PORT              (8u)

/******************************************************************************/
/* SERDES Direct Access Channel Defines                                       */
/******************************************************************************/
#define DV1_SDS_INDIRECT_ACCESS_PCS_CTRL                                                                                           \
    {                                                                                                                              \
        0xFF, 0xFF, 0xFF, 0xFF, DV1_PORT4_INDIRECT_ACCESS_SDS_PCS_CTL, DV1_PORT5_INDIRECT_ACCESS_SDS_PCS_CTL,                      \
            DV1_PORT6_INDIRECT_ACCESS_SDS_PCS_CTL, DV1_PORT7_INDIRECT_ACCESS_SDS_PCS_CTL, DV1_PORT8_INDIRECT_ACCESS_SDS_PCS_CTL,   \
            DV1_PORT9_INDIRECT_ACCESS_WTG_PCS_CTL, DV1_PORT10_INDIRECT_ACCESS_WTG_PCS_CTL, DV1_PORT11_INDIRECT_ACCESS_WTG_PCS_CTL, \
            DV1_PORT12_INDIRECT_ACCESS_WTG_PCS_CTL, 0xFF, 0xFF                                                                     \
    }
#define DV1_PCS_DATA_CHANNEL                                                                                                          \
    {                                                                                                                                 \
        0xFF, 0xFF, 0xFF, 0xFF, DV1_PORT4_INDIRECT_ACCESS_SDS_PCS_DATA, DV1_PORT5_INDIRECT_ACCESS_SDS_PCS_DATA,                       \
            DV1_PORT6_INDIRECT_ACCESS_SDS_PCS_DATA, DV1_PORT7_INDIRECT_ACCESS_SDS_PCS_DATA, DV1_PORT8_INDIRECT_ACCESS_SDS_PCS_DATA,   \
            DV1_PORT9_INDIRECT_ACCESS_WTG_PCS_DATA, DV1_PORT10_INDIRECT_ACCESS_WTG_PCS_DATA, DV1_PORT11_INDIRECT_ACCESS_WTG_PCS_DATA, \
            DV1_PORT12_INDIRECT_ACCESS_WTG_PCS_DATA, 0xFF, 0xFF                                                                       \
    }
#define DV1_SDS_INDIRECT_ACCESS_DPHY_CTRL                                                                                             \
    {                                                                                                                                 \
        0xFF, 0xFF, 0xFF, 0xFF, DV1_PORT4_INDIRECT_ACCESS_SDS_DPHY_CTL, DV1_PORT5_INDIRECT_ACCESS_SDS_DPHY_CTL,                       \
            DV1_PORT6_INDIRECT_ACCESS_SDS_DPHY_CTL, DV1_PORT7_INDIRECT_ACCESS_SDS_DPHY_CTL, DV1_PORT8_INDIRECT_ACCESS_SDS_DPHY_CTL,   \
            DV1_PORT9_INDIRECT_ACCESS_WTG_DPHY_CTL, DV1_PORT10_INDIRECT_ACCESS_WTG_DPHY_CTL, DV1_PORT11_INDIRECT_ACCESS_WTG_DPHY_CTL, \
            DV1_PORT12_INDIRECT_ACCESS_WTG_DPHY_CTL, 0xFF, 0xFF                                                                       \
    }
#define DV1_DPHY_DATA_CHANNEL                                                                                                            \
    {                                                                                                                                    \
        0xFF, 0xFF, 0xFF, 0xFF, DV1_PORT4_INDIRECT_ACCESS_SDS_DPHY_DATA, DV1_PORT5_INDIRECT_ACCESS_SDS_DPHY_DATA,                        \
            DV1_PORT6_INDIRECT_ACCESS_SDS_DPHY_DATA, DV1_PORT7_INDIRECT_ACCESS_SDS_DPHY_DATA, DV1_PORT8_INDIRECT_ACCESS_SDS_DPHY_DATA,   \
            DV1_PORT9_INDIRECT_ACCESS_WTG_DPHY_DATA, DV1_PORT10_INDIRECT_ACCESS_WTG_DPHY_DATA, DV1_PORT11_INDIRECT_ACCESS_WTG_DPHY_DATA, \
            DV1_PORT12_INDIRECT_ACCESS_WTG_DPHY_DATA, 0xFF, 0xFF                                                                         \
    }

/******************************************************************************/
/* OFFSET Defines                                                             */
/******************************************************************************/
#define DV1_SDS_PER_PORT_OFFSET (0x800u)

/******************************************************************************/
/* SERDES SPEED Defines                                                       */
/******************************************************************************/
#define DV1_SDS_SPEED_MASK  (0x3003u)
#define DV1_SDS_SPEED_10G   (0x1000u)
#define DV1_SDS_SPEED_5G    (0x1002u)
#define DV1_SDS_SPEED_2500M (0x1001u)
#define DV1_SDS_SPEED_1000M (0x2u)
#define DV1_SDS_SPEED_100M  (0x1u)
#define DV1_SDS_SPEED_10M   (0x0u)

/******************************************************************************/
/* Global Variable                                                            */
/******************************************************************************/
extern uint32 dv1_g_wrapSDSWorkAround;

EXTERN_API RtkApiRet dv1_drv_sds_pcs_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet dv1_drv_sds_aphy_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet dv1_drv_sds_pcs_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
EXTERN_API RtkApiRet dv1_drv_sds_aphy_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
EXTERN_API RtkApiRet dv1_drv_sds_is_serdes_port(uint32 unit, RtkPort userPort);
EXTERN_API RtkApiRet dv1_drv_sds_aphy_covert_port(uint32 portType, RtkPort *sysPort);
EXTERN_API RtkApiRet dv1_drv_sds_wrap_serdes_init(uint32 unit, RtkPort sysPort);
EXTERN_API RtkApiRet dv1_drv_sds_wrap_serdes_init_eth(uint32 unit, RtkPort sysPort);
EXTERN_API RtkApiRet dv1_drv_sds_port_speed_get(uint32 unit, RtkPort sysPort, uint32 *portSpeed);
EXTERN_API RtkApiRet dv1_drv_sds_chk_xfi_or_kr(uint32 unit, RtkPort userPort, RtkModeExt_e *result);
EXTERN_API RtkApiRet dv1_drv_sds_chk_port_range(RtkPort userPort);
EXTERN_API RtkApiRet dv1_drv_port_xmii_ssc_err_chk(RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset);
#endif
