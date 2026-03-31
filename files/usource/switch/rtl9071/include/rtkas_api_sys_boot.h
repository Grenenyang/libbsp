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

#ifndef RTKAS_API_SYS_BOOT_H__
#define RTKAS_API_SYS_BOOT_H__
/**
 * @brief This struct is used to disable the port to load configuration over ethernet, containing
 * the userPort as input.
 */
typedef struct
{
    RtkPort userPort; /**< [in] User port (0x0 to 0xFFFFFFFF) */
} SysEthPortOtpDis_t;

/**
 * @brief This struct is used for externally loading firmware, containing
 * the fileName as input.
 */
typedef struct
{
    const int8 *fileName; /**< [in] Update file name (0x0 to 0xFF each) */
} SysUpdateFw_t;

/**
 * @brief This struct is used for externally loading configuration, containing
 * the fileName as input.
 */
typedef struct
{
    const int8 *fileName; /**< [in] Update file name (0x0 to 0xFF each) */
} SysUpdateCfg_t;

/**
 * @brief This struct is used for externally loading firmware via ethernet, containing
 * the fileName as input.
 */
typedef struct
{
    const int8 *fileName; /**< [in] Update file name (0x0 to 0xFF each) */
} SysEthUpdateFw_t;

/**
 * @brief This struct is used for externally loading configuration, containing
 * the fileName as input. For RTL_907XD_V1, RLT907XC_VB and RTL907XC_VA, containing userPort as input.
 */
typedef struct
{
#if(defined(RTL_907XD_V1)) || (defined(RTL_907XC_VB)) || (defined(RTL_907XC_VA))
    RtkPort userPort; /**< [in] User port (0x0 to 0xFFFFFFFF) */
#endif
    const int8 *fileName; /**< [in] Update file name (0x0 to 0xFF each) */
} SysEthUpdateCfg_t;

/**
 * @brief This struct is used to disable port rx, containing
 * the userPort as input.
 */
typedef struct
{
    RtkPort userPort; /**< [in] User port (0x0 to 0xFFFFFFFF) */
} SysEthPortRxDis_t;

/**
 * @brief This struct is used to check flashless mode entering, containing
 * the byPassEth as input.
 */
typedef struct
{
    uint8 byPassEth; /**< [in] Bypass ethernet link check (0x0 to 0x1) */
} SysCheckFl_t;

EXTERN_API RtkApiRet rtk_sys_check_flashless_mode(UnitChip_t unitChip, SysCheckFl_t *sysCheckFl);
EXTERN_API RtkApiRet rtk_sys_leave_fw_to_cfg(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_sys_leave_flashless(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_sys_check_leave_flashless(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_sys_flashless_eth_port_otp_disable(UnitChip_t unitChip, SysEthPortOtpDis_t *sysEthPortOtpDis);
EXTERN_API RtkApiRet rtk_sys_update_firmware(UnitChip_t unitChip, SysUpdateFw_t *sysUpdateFw);
EXTERN_API RtkApiRet rtk_sys_update_config(UnitChip_t unitChip, SysUpdateCfg_t *sysUpdateCfg);
EXTERN_API RtkApiRet rtk_sys_eth_leave_fw_to_cfg(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_sys_eth_update_config(UnitChip_t unitChip, SysEthUpdateCfg_t *sysEthUpdateCfg);
EXTERN_API RtkApiRet rtk_sys_eth_update_firmware(UnitChip_t unitChip, SysEthUpdateFw_t *sysEthUpdateFw);
EXTERN_API RtkApiRet rtk_sys_eth_disable_non_configure_port_rx(UnitChip_t unitChip, SysEthPortRxDis_t *sysEthPortRxDis);

#endif /* RTKAS_API_SYS_BOOT_H__ */
