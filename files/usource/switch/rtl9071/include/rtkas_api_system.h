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

#ifndef RTKAS_API_SYSTEM_H__
#define RTKAS_API_SYSTEM_H__

#include "rtkas_common.h"
#include "rtkas_common_types.h"

/**
 * @brief Enumeration for system status.
 */
typedef enum
{
    EM_SYS_STATUS_INIT,    /**< The switch is initializing */
    EM_SYS_STATUS_WAITING, /**< The switch is waiting for external */
    EM_SYS_STATUS_READY,   /**< The switch is ready to access */
    EM_SYS_STATUS_UNKNOWN, /**< The switch is in unknown status */
    EM_SYS_STATUS_END
} RtkSysStatus_e;

/**
 * @brief Structure to hold OTP version information.
 *
 * This structure contains the major and minor version numbers of the OTP.
 */
typedef struct
{
    uint8 major; /**< [out] Major version number (0x00 ~ 0xFF) */
    uint8 minor; /**< [out] Minor version number (0x00 ~ 0xFF) */
} OtpVersionInfo_t;

/**
 * @brief Structure to hold SDK version information.
 *
 * This structure contains the major, minor, and build version numbers of the SDK.
 */
typedef struct
{
    uint32 major; /**< [out] Major version number (0x00000000 ~ 0xFFFFFFFF) */
    uint32 minor; /**< [out] Minor version number (0x00000000 ~ 0xFFFFFFFF) */
    uint32 build; /**< [out] Build date (0x00000000 ~ 0xFFFFFFFF) */
} SdkVersionInfo_t;

/**
 * @brief Structure to hold firmware version information.
 *
 * This structure contains detailed version information of the firmware, including
 * the chip identifier, major and minor version numbers, and the release date.
 */
typedef struct
{
    uint32 version; /**< [out] Full version number (0x00000000 ~ 0xFFFFFFFF) */
    uint32 chip;    /**< [out] Chip identifier (1 ~ 15) */
    uint32 major;   /**< [out] Major version number (0 ~ 255) */
    uint32 minor;   /**< [out] Minor version number (0 ~ 15) */
    uint32 date;    /**< [out] Release date (0 ~ 31) */
    uint32 month;   /**< [out] Release month (1 ~ 12) */
    uint32 year;    /**< [out] Release year (20 ~ 24) */
} FwVersionInfo_t;

/**
 * @brief Structure to hold chip version information.
 *
 * This structure contains the version number of the chip.
 */
typedef struct
{
    uint32 version; /**< [out] Chip version number (1 ~ 2) */
} ChipVersionInfo_t;

/**
 * @brief Structure to hold chip ID information.
 *
 * This structure contains the ID value of the chip.
 */
typedef struct
{
    uint32 value; /**< [out] Chip ID value (0x00000000 ~ 0xFFFFFFFF) */
} ChipId_t;

/**
 * @brief Structure to hold indirect channel information.
 *
 * This structure contains the number of the indirect channel to be used.
 */
typedef struct
{
    uint32 channel; /**< [in] Indirect channel number (0 ~ 1) */
} IndirChannel_t;

EXTERN_API RtkApiRet rtk_sys_init(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_sys_status_get(UnitChip_t unitChip, RtkSysStatus_e *sysStatus);
EXTERN_API RtkApiRet rtk_sys_sdk_version_get(UnitChip_t unitChip, SdkVersionInfo_t *sdkVersionInfo);
EXTERN_API RtkApiRet rtk_sys_otp_version_get(UnitChip_t unitChip, OtpVersionInfo_t *otpVersionInfo);
EXTERN_API RtkApiRet rtk_sys_fw_version_get(UnitChip_t unitChip, FwVersionInfo_t *fwVersionInfo);
EXTERN_API RtkApiRet rtk_sys_chip_version_get(UnitChip_t unitChip, ChipVersionInfo_t *chipVersionInfo);
EXTERN_API RtkApiRet rtk_sys_chip_id_get(UnitChip_t unitChip, ChipId_t *chipId);
EXTERN_API RtkApiRet rtk_sys_indirect_channel_set(UnitChip_t unitChip, const IndirChannel_t *indirChannel);

#endif /* RTKAS_API_SYSTEM_H__ */