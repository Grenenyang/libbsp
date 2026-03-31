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

#include "rtkas_api_ext.h"
#include "rtkas_api_system.h"
#include "rtkas_version.h"

/* Declaration */
RtkApiRet dv1_sys_init_sdk_example(void);
RtkApiRet dv1_sys_set_secondary_channel_example(void);
RtkApiRet dv1_sys_get_chip_firmware_ver_example(void);
RtkApiRet dv1_sys_get_chip_id_example(void);
RtkApiRet dv1_sys_get_chip_version_example(void);
RtkApiRet dv1_sys_get_chip_otp_version_example(void);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup SYSTEM_EXAMPLE System Example
 * @{
 */

/**
 * @addtogroup RTL907XD_V1_SYS_EXAMPLE RTL907XD_V1 System Example
 * @{
 */

/**
 * @addtogroup RTL907XD_V1_SYS_INIT_SDK System Initialize SDK Example
 *
 * @brief <b> Init SDK </b> \n
 *        Initialize SDK
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to initialize the SDK.
 *
 * @section Description
 *          This example demonstrates how to initialize the SDK. \n
 *          The check function's return value "ret" must be RT_ERR_OK if this SDK is used for a supported chip. \n
 *          Otherwise, the (-RT_ERR_CHIP_NOT_SUPPORTED) value will be returned. \n
 *          This example demonstrates how to initialize the SDK before use. \n
 *          If incorrect `unicChip.chip` value is provided, an RT_ERR_CHIP_NOT_SUPPORTED error will be returned. \n
 *          Additionally, in this example, the system will poll the switch’s status until it is ready. \n
 *
 * @{
 */
RtkApiRet dv1_sys_init_sdk_example(void)
{
    UnitChip_t     unitChip  = {.chip = CHIP_907XD_V1, .unit = 1u};
    RtkSysStatus_e sysStatus = EM_SYS_STATUS_UNKNOWN;
    RtkApiRet      ret       = RT_ERR_OK;

    do
    {
        /* Step 1. Call system init function */
        CHK_FUN_CONTINUE(ret, rtk_sys_init(unitChip));

        /* Step 2. Polling system status until it is ready */
        while((ret == RT_ERR_OK) && (sysStatus != EM_SYS_STATUS_READY))
        {
            ret = rtk_sys_status_get(unitChip, &sysStatus);
        }

    } while(0u == 1u);

    /* Expected Behavior: The switch is ready for configuration */
    return ret;
}

/**@}*/ /* RTL907XD_V1_SYS_INIT_SDK */

/**
 * @addtogroup RTL907XD_V1_SYS_GET_CHIP_FW_VER System Get Firmware Version Example
 *
 * @brief   <b>Firmware Version<\b> \n
 *          Get firmware version of chip.
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to retreive the firmware version.
 *
 * @section Description
 *          This example demonstrates how to retreive the firmware version, and \n
 *          it also has a basic validation by comparing if the major value is expected.
 *
 * @{
 */
RtkApiRet dv1_sys_get_chip_firmware_ver_example(void)
{
    UnitChip_t      unitChip      = {.chip = CHIP_907XD_V1, .unit = 1u};
    FwVersionInfo_t fwVersionInfo = {0};
    RtkApiRet       ret           = RT_ERR_OK;

    const uint32 fwVerChk = 1u;

    do
    {
        /* Step 1. Get firmware version */
        CHK_FUN_CONTINUE(ret, rtk_sys_fw_version_get(unitChip, &fwVersionInfo));

        /* Expected Behavior: the value of firmware version is same as expected */
        if(fwVersionInfo.major != fwVerChk)
        {
            ret = RT_ERR_FAILED;
        }

    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XD_V1_SYS_GET_CHIP_FW_VER */

/**
 * @addtogroup RTL907XD_V1_SYS_GET_CHIP_ID System Get Chip ID Example
 *
 * @brief <b> Chip ID </b> \n
 *        Get chip identity.
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to retreive the chip identity.
 *
 * @section Description
 *          This example demonstrates how to retreive retreive the chip's identity and \n
 *          verify if it is matches the expected value.
 *
 * @{
 */
RtkApiRet dv1_sys_get_chip_id_example(void)
{
    UnitChip_t unitChip = {.chip = CHIP_907XD_V1, .unit = 1u};
    ChipId_t   chipId   = {0};
    uint32     idVal    = 0u;
    RtkApiRet  ret      = RT_ERR_OK;

    const uint32 expected = 0x6764u;

    do
    {
        /* Step 1. Get chip ID */
        CHK_FUN_CONTINUE(ret, rtk_sys_chip_id_get(unitChip, &chipId));
        idVal = chipId.value >> 16u;

        /* Expected Behavior: The chip ID matches the expected value */
        if(idVal != expected)
        {
            ret = RT_ERR_CHIP_NOT_FOUND;
        }

    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XD_V1_SYS_GET_CHIP_ID */

/**
 * @addtogroup RTL907XD_V1_SYS_GET_CHIP_VER System Get Chip Version Example
 *
 * @brief <b> Chip Version </b> \n
 *        Get chip version.
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to retreive the chip version.
 *
 * @section Description
 *          This example demonstrates how to retreive retreive the chip's identity and \n
 *          verify if it is matches the expected value (1 for rtl907xd-v1 series).
 *
 * @{
 */
RtkApiRet dv1_sys_get_chip_version_example(void)
{
    UnitChip_t        unitChip        = {.chip = CHIP_907XD_V1, .unit = 1u};
    ChipVersionInfo_t chipVersionInfo = {0};
    RtkApiRet         ret             = RT_ERR_OK;

    const uint32 expected = 1u;

    do
    {
        /* Step 1. Get chip version */
        CHK_FUN_CONTINUE(ret, rtk_sys_chip_version_get(unitChip, &chipVersionInfo));

        /* Expected Behavior: The chip version matches the expected value */
        if(chipVersionInfo.version != expected)
        {
            ret = RT_ERR_CHIP_NOT_FOUND;
        }

    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XD_V1_SYS_GET_CHIP_VER */

/**
 * @addtogroup RTL907XD_V1_SYS_GET_OTP_VER System Get OTP Version Example
 *
 * @brief <b> OTP Version </b> \n
 *        Get OTP version.
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to retreive the OTP version.
 *
 * @section Description
 *          This example demonstrates how to retreive retreive the chip's OTP version.
 *
 * @{
 */
RtkApiRet dv1_sys_get_chip_otp_version_example(void)
{
    UnitChip_t       unitChip       = {.chip = CHIP_907XD_V1, .unit = 1u};
    OtpVersionInfo_t otpVersionInfo = {0};
    RtkApiRet        ret            = RT_ERR_OK;

    do
    {
        /* Step 1. Get chip OTP version */
        CHK_FUN_CONTINUE(ret, rtk_sys_otp_version_get(unitChip, &otpVersionInfo));

    } while(0u == 1u);

    /* Expected Behavior: The OTP version matches the expected value */
    return ret;
}

/**@}*/ /* RTL907XD_V1_SYS_GET_OTP_VER */

/**
 * @addtogroup RTL907XD_V1_SYS_SET_SEC_CHANNEL System Set Indirect Access Channel Example
 *
 * @brief <b> Firmware Indirect Access Channel </b> \n
 *        Change to use the secondary channel for firmware indirect access mechanism.
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to change to use the secondary indirect access channel.
 *
 * @section Description
 *          This example demonstrates how to change to use the secondary indirect access channel for this SDK. \n
 *          After calling this API, all APIs that trigger indirect access will use the secondary indirect access channel.
 *
 * @{
 */
RtkApiRet dv1_sys_set_secondary_channel_example(void)
{
    UnitChip_t     unitChip     = {.chip = CHIP_907XD_V1, .unit = 1u};
    IndirChannel_t indirChannel = {.channel = DV1_RTK_INDIRECT_CHANNEL_SECONDARY};
    RtkApiRet      ret          = RT_ERR_OK;

    do
    {
        /* Step 1. Change to use the secondary indirect access channel */
        CHK_FUN_CONTINUE(ret, rtk_sys_indirect_channel_set(unitChip, &indirChannel));

    } while(0u == 1u);

    /* Expected Behavior: The upcoming indirect access request will be executed on the  secondary channel */
    return ret;
}

/**@}*/ /* RTL907XD_V1_SYS_SET_SEC_CHANNEL */

/**@}*/ /* RTL907XD_V1_SYS_EXAMPLE */
/**@}*/ /* SYSTEM_EXAMPLE */
/**@}*/ /* EXAMPLE */