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

#ifndef RTKAS_API_SERDES_H__
#define RTKAS_API_SERDES_H__

#include "rtkas_api_interface.h"

/*Maximum value*/
#define SDS_PCS_MAX_REG_NUM   (31u)
#define SDS_PCS_MAX_PAGE_NUM  (54u)
#define SDS_APHY_MAX_REG_ADDR (0xFFFFu)
#define UMAC_REG_END_BIT      (15u)

/*FW virtual channel*/
#define SDS_APHY_TYPE (0u)
#define SDS_PCS_TYPE  (1u)

#define BOOT_UP_FULL_INITIZED       (0xBFu)
#define CPU_DMY_1_BOOTUP_STATUS_LSP (24u)
#define CPU_DMY_1_BOOTUP_STATUS_LEN (8u)

/**
 * @brief This struct is used for setting or getting PCS register.
 */
typedef struct
{
    RtkPort port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  page;     /**< [in] Serdes register page (0 to 54) */
    uint32  reg;      /**< [in] Serdes register (0 to 31) */
    uint32  startBit; /**< [in] Start bit (0 to 15) */
    uint32  endBit;   /**< [in] End bit (0 to 15) */
    uint32  value;    /**< [in, out] Value (0 to 0xFFFF) */
} SysSdsPcsRegConfig_t;

/**
 * @brief This struct is used for setting or gerring APHY register.
 */
typedef struct
{
    RtkPort port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  page;     /**< [in] Serdes register page (0) */
    uint32  reg;      /**< [in] Serdes register (0 to 0xFFFF) */
    uint32  startBit; /**< [in] Start bit (0 to 15) */
    uint32  endBit;   /**< [in] End bit (0 to 15) */
    uint32  value;    /**< [in, out] Value (0 to 0xFFFF) */
} SysSdsAphyRegConfig_t;

typedef struct
{
    RtkPort      userPort;    /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkModeExt_e serdesMode;  /**< [in] SerDes mode (EM_MODE_EXT_SGMII, EM_MODE_EXT_HISGMII, EM_MODE_EXT_USXGMII, EM_MODE_EXT_5G_KR, EM_MODE_EXT_10G_KR, EM_MODE_EXT_5G_XFI, EM_MODE_EXT_10G_XFI) */
    uint32       portRole;    /**< [in] Role of the port (0 = mac role, 1 = phy role) */
    uint32       isForceMode; /**< [in] Force mode (0 = Auto-negotiation mode, 1 = Force mode) */
    uint32       speed;       /**< [in] Serdes speed configuration (DVA_SPEED_10MBPS to DVA_SPEED_10000MBPS) */
    uint32       portSpeed;   /**< [in] Port speed configuration (DVA_SPEED_10MBPS to DVA_SPEED_10000MBPS) */
} SysSdsConfigSet_t;

typedef struct
{
    RtkPort      userPort;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkModeExt_e serdesMode; /**< [in] SerDes mode (EM_MODE_EXT_SGMII, EM_MODE_EXT_HISGMII, EM_MODE_EXT_USXGMII, EM_MODE_EXT_5G_KR, EM_MODE_EXT_10G_KR, EM_MODE_EXT_5G_XFI, EM_MODE_EXT_10G_XFI) */
    uint32       linkStatus; /**< [in] Port Link status (0 = link down, 1 = link up) */
} SysSdsForceLinkSet_t;
EXTERN_API RtkApiRet rtk_sys_sds_reg_getbits_pcs(UnitChip_t unitChip, SysSdsPcsRegConfig_t *sysSdsRegGetbitsPcs);
EXTERN_API RtkApiRet rtk_sys_sds_reg_getbits_aphy(UnitChip_t unitChip, SysSdsAphyRegConfig_t *sysSdsRegGetbitsAphy);
EXTERN_API RtkApiRet rtk_sys_sds_reg_setbits_pcs(UnitChip_t unitChip, SysSdsPcsRegConfig_t *sysSdsRegSetbitsPcs);
EXTERN_API RtkApiRet rtk_sys_sds_reg_setbits_aphy(UnitChip_t unitChip, SysSdsAphyRegConfig_t *sysSdsRegSetbitsAphy);
EXTERN_API RtkApiRet rtk_sys_sds_config_set(UnitChip_t unitChip, SysSdsConfigSet_t *sysSdsConfigSet);
EXTERN_API RtkApiRet rtk_sys_sds_force_link_set(UnitChip_t unitChip, SysSdsForceLinkSet_t *sysSdsForceLinkSet);
#endif /* RTKAS_API_SERDES_H__ */