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

#ifndef DVA_ASICDRV_SERDES_H
#define DVA_ASICDRV_SERDES_H

#include "rtkas_common_types.h"

#include "rtkas_types.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_serdes.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtkas_api_interface.h"
/******************************************************************************/
/* SERDES mode Defines                                                        */
/******************************************************************************/
#define DVA_SDS_MODE_RESERVE (0x0u)

#define DVA_SDS_MODE_RESERVE    (0x0u)
#define DVA_SDS_MODE_SG         (0x2u)
#define DVA_SDS_MODE_HSG        (0x12u)
#define DVA_SDS_MODE_USXG       (0xDu)
#define DVA_SDS_MODE_KR_XFI_5G  (0x1Du)
#define DVA_SDS_MODE_KR_XFI_10G (0x1Au)
#define DVA_SDS_OFF_MODE        (0x1Fu)

#define DVA_AN_MODE    (0x0u)
#define DVA_FORCE_MODE (0x1u)

#define DVA_ANALOG_SDS_10G (0x0u)
#define DVA_ANALOG_SDS_5G  (0x3u)
/******************************************************************************/
/* SERDES Condition Defines                                                   */
/******************************************************************************/
#define DVA_IS_WRAP_SDS          (0x1u)
#define DVA_IS_SDS_PORT          (0x1u)
#define DVA_IS_NOT_SDS           (0x0u)
#define DVA_IS_NOT_ETH_ACCESS    (0x0u)
#define DVA_IS_ETH_ACCESS        (0x1u)
#define DVA_WORKAROUND_READY     (0x1u)
#define DVA_WORKAROUND_NOT_READY (0x0u)
/******************************************************************************/
/* SERDES Register Field Defines                                              */
/******************************************************************************/
#define DVA_INDIRECT_ACCESS_SDS_INDEX_LSP   (21u)
#define DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP (16u)

/******************************************************************************/
/* Register Value Defines                                                     */
/******************************************************************************/
/* PORT#_INDIRECT_ACCESS_SDS_PCS_CTL */
#define DVA_WRITE_DATA (0x1u)
#define DVA_READ_DATA  (0x0u)

/******************************************************************************/
/* SERDES Count Defines                                                       */
/******************************************************************************/
#define DVA_POLLING_SDS_CHANNEL_MIN_UNIT_MICRO_SEC (1u)
#define DVA_POLLING_SDS_CHANNEL_MAX_UNIT_MICRO_SEC (10u)
#define DVA_SGMII_DOMAIN4_SDS_PORT_BITMASK         (0x7FFFu)
#define DVA_WRAP_SDS_PORT_BITMASK                  (0x1F0u)
#define DVA_SDS_START_PORT_OFFSET                  (4u)
#define DVA_WRAP_SDS_START_PORT                    (4u)
#define DVA_WRAP_SDS_END_PORT                      (8u)

/******************************************************************************/
/* SERDES SPEED Defines                                                       */
/******************************************************************************/
#define DVA_SDS_SPEED_MASK  (0x3003u)
#define DVA_SDS_SPEED_10G   (0x1000u)
#define DVA_SDS_SPEED_5G    (0x1002u)
#define DVA_SDS_SPEED_2500M (0x1001u)
#define DVA_SDS_SPEED_1000M (0x2u)
#define DVA_SDS_SPEED_100M  (0x1u)
#define DVA_SDS_SPEED_10M   (0x0u)

#define DVA_SDS_SPEED_10000M_FULL (0x1014u)
#define DVA_SDS_SPEED_5000M_FULL  (0x1016u)
#define DVA_SDS_SPEED_2500M_FULL  (0x1015u)
#define DVA_SDS_SPEED_1000M_FULL  (0x16u)
#define DVA_SDS_SPEED_100M_FULL   (0x15u)
#define DVA_SDS_SPEED_10M_FULL    (0x14u)

#define DVA_SDS_INDIRECT_ACCESS_PCS_CTRL                                                                                           \
    {                                                                                                                              \
        0xFF, 0xFF, 0xFF, 0xFF, DVA_PORT4_INDIRECT_ACCESS_SDS_PCS_CTL, DVA_PORT5_INDIRECT_ACCESS_SDS_PCS_CTL,                      \
            DVA_PORT6_INDIRECT_ACCESS_SDS_PCS_CTL, DVA_PORT7_INDIRECT_ACCESS_SDS_PCS_CTL, DVA_PORT8_INDIRECT_ACCESS_SDS_PCS_CTL,   \
            DVA_PORT9_INDIRECT_ACCESS_WTG_PCS_CTL, DVA_PORT10_INDIRECT_ACCESS_WTG_PCS_CTL, DVA_PORT11_INDIRECT_ACCESS_WTG_PCS_CTL, \
            DVA_PORT12_INDIRECT_ACCESS_WTG_PCS_CTL, 0xFF, 0xFF                                                                     \
    }
#define DVA_PCS_DATA_CHANNEL                                                                                                          \
    {                                                                                                                                 \
        0xFF, 0xFF, 0xFF, 0xFF, DVA_PORT4_INDIRECT_ACCESS_SDS_PCS_DATA, DVA_PORT5_INDIRECT_ACCESS_SDS_PCS_DATA,                       \
            DVA_PORT6_INDIRECT_ACCESS_SDS_PCS_DATA, DVA_PORT7_INDIRECT_ACCESS_SDS_PCS_DATA, DVA_PORT8_INDIRECT_ACCESS_SDS_PCS_DATA,   \
            DVA_PORT9_INDIRECT_ACCESS_WTG_PCS_DATA, DVA_PORT10_INDIRECT_ACCESS_WTG_PCS_DATA, DVA_PORT11_INDIRECT_ACCESS_WTG_PCS_DATA, \
            DVA_PORT12_INDIRECT_ACCESS_WTG_PCS_DATA, 0xFF, 0xFF                                                                       \
    }
#define DVA_SDS_INDIRECT_ACCESS_DPHY_CTRL                                                                                             \
    {                                                                                                                                 \
        0xFF, 0xFF, 0xFF, 0xFF, DVA_PORT4_INDIRECT_ACCESS_SDS_DPHY_CTL, DVA_PORT5_INDIRECT_ACCESS_SDS_DPHY_CTL,                       \
            DVA_PORT6_INDIRECT_ACCESS_SDS_DPHY_CTL, DVA_PORT7_INDIRECT_ACCESS_SDS_DPHY_CTL, DVA_PORT8_INDIRECT_ACCESS_SDS_DPHY_CTL,   \
            DVA_PORT9_INDIRECT_ACCESS_WTG_DPHY_CTL, DVA_PORT10_INDIRECT_ACCESS_WTG_DPHY_CTL, DVA_PORT11_INDIRECT_ACCESS_WTG_DPHY_CTL, \
            DVA_PORT12_INDIRECT_ACCESS_WTG_DPHY_CTL, 0xFF, 0xFF                                                                       \
    }
#define DVA_DPHY_DATA_CHANNEL                                                                                                            \
    {                                                                                                                                    \
        0xFF, 0xFF, 0xFF, 0xFF, DVA_PORT4_INDIRECT_ACCESS_SDS_DPHY_DATA, DVA_PORT5_INDIRECT_ACCESS_SDS_DPHY_DATA,                        \
            DVA_PORT6_INDIRECT_ACCESS_SDS_DPHY_DATA, DVA_PORT7_INDIRECT_ACCESS_SDS_DPHY_DATA, DVA_PORT8_INDIRECT_ACCESS_SDS_DPHY_DATA,   \
            DVA_PORT9_INDIRECT_ACCESS_WTG_DPHY_DATA, DVA_PORT10_INDIRECT_ACCESS_WTG_DPHY_DATA, DVA_PORT11_INDIRECT_ACCESS_WTG_DPHY_DATA, \
            DVA_PORT12_INDIRECT_ACCESS_WTG_DPHY_DATA, 0xFF, 0xFF                                                                         \
    }

/******************************************************************************/
/* Global Variable                                                            */
/******************************************************************************/
EXTERN_API RtkApiRet dva_drv_sds_pcs_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet dva_drv_sds_aphy_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet dva_drv_sds_pcs_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
EXTERN_API RtkApiRet dva_drv_sds_aphy_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
EXTERN_API RtkApiRet dva_drv_sds_is_serdes_port(uint32 unit, RtkPort userPort);
EXTERN_API RtkApiRet dva_drv_sds_port_speed_get(uint32 unit, RtkPort sysPort, uint32 *portSpeed);
EXTERN_API RtkApiRet dva_drv_sds_aphy_covert_port(uint32 portType, RtkPort *sysPort);
EXTERN_API RtkApiRet dva_drv_sds_chk_xfi_or_kr(uint32 unit, RtkPort userPort, RtkModeExt_e *result);
EXTERN_API RtkApiRet dva_drv_sds_chk_port_range(RtkPort userPort);
EXTERN_API RtkApiRet dva_drv_sds_wrap_serdes_init(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet dva_drv_sds_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e serdesMode, uint32 portRole, uint32 isForceMode, uint32 speed, uint32 portSpeed);
EXTERN_API RtkApiRet dva_drv_sds_1gmac_link_set(uint32 unit, uint32 userPort, RtkModeExt_e serdesMode, uint32 linkStatus);
EXTERN_API RtkApiRet dva_drv_sds_5gmac_link_set(uint32 unit, uint32 userPort, RtkModeExt_e serdesMode, uint32 linkStatus);
EXTERN_API RtkApiRet dva_drv_sds_mode_chk(RtkModeExt_e serdesMode);
#endif
