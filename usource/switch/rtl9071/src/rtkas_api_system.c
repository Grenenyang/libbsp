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

#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_api_system.h"
#include "rtkas_version.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_system.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_system.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_system.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_system.h"
#endif

/**
 * @addtogroup RTK_MODULE_SYSTEM System
 * @brief System APIs. This component contains various system-wide APIs.
 * @{
 */

/**
 * @addtogroup SYSTEM_SDK_VERSION_GET SDK version
 * @brief Retrieves the SDK version.
 * @{
 */

/**
 * @brief This function retrieves the SDK version.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] sdkVersionInfo       A pointer to a structure where the SDK version information will be stored. (N/A) \n
 *                                  This structure contains the major, minor, and build date numbers of the SDK.
 *
 * @retval RT_ERR_OK                Successfully retrieved the SDK version.
 * @retval -RT_ERR_NULL_POINTER     The sdkVersionInfo pointer is NULL.
 */
RtkApiRet rtk_sys_sdk_version_get(UnitChip_t unitChip, SdkVersionInfo_t *sdkVersionInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == sdkVersionInfo)
        {
            ret = -RT_ERR_NULL_POINTER;
            break;
        }

        sdkVersionInfo->major = (uint32)RTKAS_SDK_VERSION_MAJOR;
        sdkVersionInfo->minor = (uint32)RTKAS_SDK_VERSION_MINOR;
        sdkVersionInfo->build = (uint32)RTKAS_SDK_VERSION_BUILD;

    } while(false);

    return ret;
}

/** @} */ /* end of SYSTEM_SDK_VERSION_GET */

/**
 * @addtogroup SYSTEM_INIT System Initialization
 * @brief Initializes the system for the specified chip version.
 * @{
 */

/**
 * @brief Initializes the system for the specified chip version.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 *
 * @retval RT_ERR_OK                System initialized successfully.
 * @retval -RT_ERR_NOT_SUPPORT      The specified chip is not supported.
 * @retval Others                   Please refer to dv1_sys_init(), dva_sys_init(), cvb_sys_init(), cva_sys_init().
 */
RtkApiRet rtk_sys_init(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_init(unitChip.unit);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/** @} */ /* end of SYSTEM_INIT */

/**
 * @addtogroup SYSTEM_STATUS_GET System Status
 * @brief Retrieves the system status of the specified chip.
 * @{
 */

/**
 * @brief Retrieves the system status of the specified chip.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] sysStatus            A pointer to a variable where the system status will be stored. (N/A)
 *
 * @retval RT_ERR_OK                Retrieved the system status successfully.
 * @retval -RT_ERR_NOT_SUPPORT      The specified chip is not supported.
 * @retval Others                   Please refer to dv1_sys_status_get(), dva_sys_status_get(), cvb_sys_status_get(), cva_sys_status_get().
 */
RtkApiRet rtk_sys_status_get(UnitChip_t unitChip, RtkSysStatus_e *sysStatus)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_status_get(unitChip.unit, sysStatus);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_status_get(unitChip.unit, sysStatus);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_status_get(unitChip.unit, sysStatus);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_status_get(unitChip.unit, sysStatus);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/** @} */ /* end of SYSTEM_STATUS_GET */

/**
 * @addtogroup SYSTEM_OTP_VERSION_GET OTP version
 * @brief Retrieves the OTP version running in the switch.
 * @{
 */

/**
 * @brief This function retrieves the OTP version running in the switch.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed..
 * @param[out] otpVersionInfo       A pointer to a structure where the OTP version information will be stored. (N/A) \n
 *                                  This structure contains the major and minor OTP version numbers.
 *
 * @retval RT_ERR_OK                Successfully retrieved the OTP version.
 * @retval -RT_ERR_NOT_SUPPORT      The specified chip is not supported.
 * @retval Others                   Please refer to dv1_sys_otp_version_get(), dva_sys_otp_version_get(), cvb_sys_otp_version_get(), cva_sys_otp_version_get().
 */
RtkApiRet rtk_sys_otp_version_get(UnitChip_t unitChip, OtpVersionInfo_t *otpVersionInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_otp_version_get(unitChip.unit, &otpVersionInfo->major, &otpVersionInfo->minor);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_otp_version_get(unitChip.unit, &otpVersionInfo->major, &otpVersionInfo->minor);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_otp_version_get(unitChip.unit, &otpVersionInfo->major, &otpVersionInfo->minor);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_otp_version_get(unitChip.unit, &otpVersionInfo->major, &otpVersionInfo->minor);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/** @} */ /* end of SYSTEM_OTP_VERSION_GET */

/**
 * @addtogroup SYSTEM_FW_VERSION_GET Firmware version
 * @brief Retrieves the firmware version running in the switch.
 * @{
 */

/**
 * @brief This function retrieves the firmware version running in the switch.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed..
 * @param[out] fwVersionInfo        A pointer to a structure where the firmware version information will be stored. (N/A) \n
 *                                  This structure contains the firmware version.
 *
 * @retval RT_ERR_OK                Successfully retrieved the firmware version.
 * @retval -RT_ERR_NOT_SUPPORT      The specified chip is not supported.
 * @retval Others                   Please refer to dv1_sys_fw_version_get(), dva_sys_fw_version_get(), cvb_sys_fw_version_get(), cva_sys_fw_version_get().
 */
RtkApiRet rtk_sys_fw_version_get(UnitChip_t unitChip, FwVersionInfo_t *fwVersionInfo)
{
    RtkApiRet ret = RT_ERR_OK;
    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_fw_version_get(unitChip.unit, fwVersionInfo);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_fw_version_get(unitChip.unit, fwVersionInfo);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_fw_version_get(unitChip.unit, fwVersionInfo);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_fw_version_get(unitChip.unit, fwVersionInfo);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/** @} */ /* end of SYSTEM_FW_VERSION_GET */

/**
 * @addtogroup SYSTEM_CHIP_ID_GET Chip Identity
 * @brief Retrieves the chip ID from the internal register.
 * @{
 */

/**
 * @brief This function retrieves the chip ID from the internal register.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed..
 * @param[out] chipId               A pointer to a structure where the chip ID will be stored.
 *                                  This structure contains the chip ID.
 *
 * @retval RT_ERR_OK                Successfully retrieved the chip ID.
 * @retval -RT_ERR_NOT_SUPPORT      The specified chip is not supported.
 * @retval Others                   Please refer to dv1_sys_chip_id_get(), dva_sys_chip_id_get(), cvb_sys_chip_id_get(), cva_sys_chip_id_get().
 */
RtkApiRet rtk_sys_chip_id_get(UnitChip_t unitChip, ChipId_t *chipId)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_chip_id_get(unitChip.unit, &chipId->value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_chip_id_get(unitChip.unit, &chipId->value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_chip_id_get(unitChip.unit, &chipId->value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_chip_id_get(unitChip.unit, &chipId->value);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/** @} */ /* end of SYSTEM_CHIP_ID_GET */

/**
 * @addtogroup SYSTEM_CHIP_VERSION_GET Chip version
 * @brief Retrieves the chip version from the internal register.
 * @{
 */

/**
 * @brief This function retrieves the chip version from the internal register.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed..
 * @param[out] chipVersionInfo      A pointer to a structure where the chip version information will be stored. (N/A) \n
 *                                  This structure contains the chip version.
 *
 * @retval RT_ERR_OK                Successfully retrieved the chip version.
 * @retval -RT_ERR_NOT_SUPPORT      The specified chip is not supported.
 * @retval Others                   Please refer to dv1_sys_chip_version_get(), dva_sys_chip_version_get(), cvb_sys_chip_version_get(), cva_sys_chip_version_get().
 */
RtkApiRet rtk_sys_chip_version_get(UnitChip_t unitChip, ChipVersionInfo_t *chipVersionInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_chip_version_get(unitChip.unit, &chipVersionInfo->version);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_chip_version_get(unitChip.unit, &chipVersionInfo->version);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_chip_version_get(unitChip.unit, &chipVersionInfo->version);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_chip_version_get(unitChip.unit, &chipVersionInfo->version);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/** @} */ /* end of SYSTEM_CHIP_VERSION_GET */

/**
 * @addtogroup SYSTEM_INDIRECT_CHANNEL_SET Indirect Access Channel
 * @brief Sets the indirect access channel to be used.
 * @{
 */

/**
 * @brief Sets the indirect access channel to be used.
 *        Supported chips: RTL907XD_VA, RTL907XD_V1, RTL907XC_VB, RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed..
 * @param[in]  indirChannel         A pointer to a structure containing the indirect channel parameter. (N/A) \n
 *                                  This structure contains the channel number to be set.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the indirect access channel number.
 * @retval -RT_ERR_NOT_SUPPORT      The specified chip is not supported.
 * @retval Others                   Please refer to dv1_sys_indirect_channel_set(), dva_sys_indirect_channel_set(), cvb_sys_indirect_channel_set(), cva_sys_indirect_channel_set().
 */
RtkApiRet rtk_sys_indirect_channel_set(UnitChip_t unitChip, const IndirChannel_t *indirChannel)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_indirect_channel_set(indirChannel->channel);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_indirect_channel_set(indirChannel->channel);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_indirect_channel_set(indirChannel->channel);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_indirect_channel_set(indirChannel->channel);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/** @} */ /* end of SYSTEM_INDIRECT_CHANNEL_SET */

/** @} */ /* System */
