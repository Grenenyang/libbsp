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

#include "rtkas_error.h"
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"

#define CVA_SYS_BS_FIRMWARE_VERIFIED_BIT (16u)
#define CVA_SYS_FW_STATUS_ADDR           CVA_CPU_DMY_1
#define CVA_SYS_FW_STATUS_LSP            24u
#define CVA_SYS_FW_STATUS_LEN            8u

/**
 * @addtogroup RTK_MODULE_SYSTEM System
 * @brief System APIs. This component contains various system-wide APIs.
 * @{
 */

/**
 * @addtogroup SYSTEM_INIT System Initialization
 * @brief Initializes the system for the specified chip version.
 * @{
 */

/**
 * @addtogroup SYSTEM_INIT_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief Initializes the system for the specified chip version.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 *
 * @retval RT_ERR_OK                System initialized successfully.
 * @retval -RT_ERR_CHIP_NOT_SUPPORTED The specified chip is not supported.
 * @retval Others                   Please refer to cva_sys_chip_id_get()
 */
RtkApiRet cva_sys_init(uint32 unit)
{
    /* Initialize local variables */
    RtkApiRet ret       = RT_ERR_OK;
    uint32    chipId    = 0u;
    uint32    i         = 0u;
    bool      supported = false;

    /* Define an array of supported chip IDs */
    const uint32 supportChipId[] = {
        0x67600000,
    };

    do
    {
        /* Get the chip ID and store it in chipId */
        CHK_FUN_CONTINUE(ret, cva_sys_chip_id_get(unit, &chipId));

        /* Loop through the supported chip IDs */
        for(i = 0u; i < (sizeof(supportChipId) / sizeof(supportChipId[0])); ++i)
        {
            /* Check if the retrieved chip ID matches a supported chip ID */
            if(chipId == supportChipId[i])
            {
                /* If so, set supported flag to true */
                supported = true;

                /* Exit the loop */
                break;
            }
        }
    } while(false);

    /* Check if the function succeeded but the chip is not supported */
    if((RT_ERR_OK == ret) && (false == supported))
    {
        /* If so, set return value to RT_ERR_CHIP_NOT_SUPPORTED */
        ret = (-RT_ERR_CHIP_NOT_SUPPORTED);
    }

    /* Return the result */
    return ret;
}

/** @} */ /* end of SYSTEM_INIT_ASIC */

/** @} */ /* end of SYSTEM_INIT */

/**
 * @addtogroup SYSTEM_STATUS_GET System Status
 * @brief Retrieves the system status of the specified chip.
 * @{
 */

/**
 * @addtogroup SYSTEM_STATUS_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief Retrieves the system status of the specified chip.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] status The switch status (EM_SYS_STATUS_INIT to EM_SYS_STATUS_UNKNOWN)
 *
 * @retval RT_ERR_OK                Retrieved the system status successfully.
 * @retval Others                   Please refer to cva_reg_field_read()
 */
RtkApiRet cva_sys_status_get(uint32 unit, RtkSysStatus_e *status)
{
    /* Initialize local variables */
    RtkApiRet ret   = RT_ERR_OK;
    uint32    value = 0u;

    /* Read the system firmware status from the register */
    ret = cva_reg_field_read(unit, CVA_SYS_FW_STATUS_ADDR, CVA_SYS_FW_STATUS_LSP, CVA_SYS_FW_STATUS_LEN, &value);

    /* Check if the previous result is OK */
    if(RT_ERR_OK == ret)
    {
        /* If so, translate the internal register value to the corresponding system status */
        switch(value)
        {
            case 0xB0:
            case 0xB1:
            case 0xB2:
            case 0xB6:
            case 0xBE:
                *status = EM_SYS_STATUS_INIT;
                break;
            case 0xB3:
            case 0xB4:
            case 0xB5:
                *status = EM_SYS_STATUS_WAITING;
                break;
            case 0xBF:
                *status = EM_SYS_STATUS_READY;
                break;
            default:
                *status = EM_SYS_STATUS_UNKNOWN;
                break;
        }
    }
    else
    {
        /* If not, set status to EM_SYS_STATUS_UNKNOWN */
        *status = EM_SYS_STATUS_UNKNOWN;
    }

    /* Return the result */
    return ret;
}

/** @} */ /* end of SYSTEM_STATUS_ASIC */

/** @} */ /* end of SYSTEM_STATUS_GET */

/**
 * @addtogroup SYSTEM_OTP_VERSION_GET OTP version
 * @brief Retrieves the OTP version running in the switch.
 * @{
 */

/**
 * @addtogroup SYSTEM_OTP_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief Retrieves the OTP version running in the switch.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] major                Major Version Number(0 to 31) \n
 *                                  The major part of OTP version
 * @param[out] minor                Minor Version Number(0) \n
 *                                  The minor part of OTP version, which should be 0 currently. \n
 *                                  This is reserved for future use
 * @retval RT_ERR_OK                Successfully retrieved the OTP version.
 * @retval -RT_ERR_NULL_POINTER     The "major" or "minor" parameter is NULL
 * @retval Others                   Please refer to cva_otp_read()
 */
RtkApiRet cva_sys_otp_version_get(uint32 unit, uint8 *major, uint8 *minor)
{
    /* Initialize local variables */
    RtkApiRet ret     = RT_ERR_OK;
    uint32    otpAddr = 0x0B07u;
    uint8     otpData = 0u;

    do
    {
        /* Check if the parameter major or minor is NULL */
        if((NULL == major) || (NULL == minor))
        {
            /* If so, set return value to RT_ERR_NULL_POINTER */
            ret = -RT_ERR_NULL_POINTER;
            break;
        }

        /* Read the OTP data from the specified address */
        CHK_FUN_CONTINUE(ret, cva_otp_read(unit, otpAddr, &otpData, 1));

        /* Extract the major version number from the OTP data */
        *major = ((otpData >> 3u) & 0x1Fu);
        /* Reserved for future use */
        *minor = 0u;

    } while(false);

    /* Return the result */
    return ret;
}

/** @} */ /* end of SYSTEM_OTP_ASIC */

/** @} */ /* end of SYSTEM_OTP_VERSION_GET */

/**
 * @addtogroup SYSTEM_FW_VERSION_GET Firmware version
 * @brief Retrieves the firmware version running in the switch.
 * @{
 */

/**
 * @addtogroup SYSTEM_FW_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief Retrieves the firmware version running in the switch.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] version              Version (0x0 to 0xFFFFFFFF) \n
 *                                  The version of the running firmware
 * @retval RT_ERR_OK                Successfully retrieved the firmware version.
 * @retval -RT_ERR_NULL_POINTER     The "version" parameter is NULL
 * @retval Others                   Please refer to cva_reg_read()
 */
RtkApiRet cva_sys_fw_version_get(uint32 unit, FwVersionInfo_t *version)
{
    /* Initialize local variables */
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;
    uint32    page   = 0u;
    uint32    ver    = 0u;

    do
    {
        /* Check if the parameter version or minor is NULL */
        if(NULL == version)
        {
            /* If so, set return value to RT_ERR_NULL_POINTER */
            ret = -RT_ERR_NULL_POINTER;
            break;
        }

        /* Check if the firmware is verified */
        CHK_FUN_CONTINUE(ret, cva_reg_bit_get(unit, CVA_CPU_DMY_0, CVA_SYS_BS_FIRMWARE_VERIFIED_BIT, &regVal));

        if(0UL == regVal)
        {
            /* If not, there is no data in flash, get ROM code version */
            CHK_FUN_CONTINUE(ret, cva_reg_page_read(unit, &page));
            CHK_FUN_CONTINUE(ret, cva_reg_page_write(unit, 0x0u));

            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, 0x203ff0, &ver));
            ver = swapl32(ver);

            CHK_FUN_CONTINUE(ret, cva_reg_page_write(unit, page));
        }
        else
        {
            /* If so, firmware exists in flash, get the version from dummy register */
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, CVA_CPU_DMY_2, &ver));
        }

        /* Extract version information from the retrieved value */
        version->chip    = ((ver & 0xF0000000u) >> 28u);
        version->major   = ((ver & 0x0FF00000u) >> 20u);
        version->minor   = ((ver & 0x000F0000u) >> 16u);
        version->date    = ((ver & 0x0000FF00u) >> 8u);
        version->month   = ((ver & 0x000000F0u) >> 4u);
        version->year    = ((ver & 0x0000000Fu) >> 0u) + 22u;
        version->version = ver;
    } while(false);

    /* Return the result */
    return ret;
}

/** @} */ /* end of SYSTEM_FW_ASIC */

/** @} */ /* end of SYSTEM_FW_VERSION_GET */

/**
 * @addtogroup SYSTEM_CHIP_ID_GET Chip Identity
 * @brief Retrieves the chip ID from the internal register.
 * @{
 */

/**
 * @addtogroup SYSTEM_CHIP_ID_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief Retrieves the chip ID from the internal register.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] value                value (0x0 to 0x0) \n
 *                                  The value of chip id, should be 0x67600000 for RTL907x/6xC family chip
 * @retval RT_ERR_OK                Successfully retrieved the chip ID.
 * @retval -RT_ERR_NULL_POINTER     The "value" parameter is NULL
 */
RtkApiRet cva_sys_chip_id_get(uint32 unit, uint32 *value)
{
    /* Initialize local variables */
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        /* Check if the parameter value or minor is NULL */
        if(NULL == value)
        {
            /* If so, set return value to RT_ERR_NULL_POINTER */
            ret = -RT_ERR_NULL_POINTER;
            break;
        }

        /* Enable the internal read */
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, CVA_INTERNAL_READ, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_INTERNAL_READ, 0x3u));

        /* Get the chip ID */
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, CVA_CHIP_VERSION_ID, value));

        /* Disable the internal read */
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_INTERNAL_READ, regVal));
    } while(false);

    /* Return the result */
    return ret;
}

/** @} */ /* end of SYSTEM_CHIP_ID_ASIC */

/** @} */ /* end of SYSTEM_CHIP_ID_GET */

/**
 * @addtogroup SYSTEM_CHIP_VERSION_GET Chip version
 * @brief Retrieves the chip version from the internal register.
 * @{
 */

/**
 * @addtogroup SYSTEM_CHIP_VERSION_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief Retrieves the chip version from the internal register.
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] version              version (1 to 1) \n
 *                                  The chip version
 * @retval RT_ERR_OK                Successfully retrieved the chip version.
 * @retval -RT_ERR_NULL_POINTER     The "version" parameter is NULL
 * @retval Others                   Please refer to cva_sys_chip_id_get()
 */
RtkApiRet cva_sys_chip_version_get(uint32 unit, uint32 *version)
{
    /* Initialize local variables */
    RtkApiRet ret    = RT_ERR_OK;
    uint32    chipId = 0u;

    do
    {
        /* Check if the parameter version or minor is NULL */
        if(NULL == version)
        {
            /* If so, set return value to RT_ERR_NULL_POINTER */
            ret = -RT_ERR_NULL_POINTER;
            break;
        }

        /* Retrieve the chip ID */
        CHK_FUN_CONTINUE(ret, cva_sys_chip_id_get(unit, &chipId));

        /* Check if the chip ID matches the project number */
        if(chipId == 0x67600000u)
        {
            /* If so, set version to 1 */
            *version = 1;
        }
        else
        {
            /* If not, do nothing */
        }
    } while(false);

    /* Return the result */
    return ret;
}

/** @} */ /* end of SYSTEM_CHIP_VERSION_ASIC */

/** @} */ /* end of SYSTEM_CHIP_VERSION_GET */

/**
 * @addtogroup SYSTEM_INDIRECT_CHANNEL_SET Indirect Access Channel
 * @brief Sets the indirect access channel to be used.
 * @{
 */

/**
 * @addtogroup SYSTEM_INDIRECT_CHANNEL_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief Sets the indirect access channel to be used.
 *
 * @param[in] channel               Channel (CVA_RTK_INDIRECT_CHANNEL_PRIMARY to CVA_RTK_INDIRECT_CHANNEL_SECONDARY) \n
 *                                  specific which indirect channel that all function of this SDK should use
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the indirect access channel number.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range
 */
RtkApiRet cva_sys_indirect_channel_set(uint32 channel)
{
    /* Initialize local variables */
    RtkApiRet ret = RT_ERR_OK;

    /* Validate the channel parameter and set the indirect access channel */
    if((channel == CVA_RTK_INDIRECT_CHANNEL_PRIMARY) || (channel == CVA_RTK_INDIRECT_CHANNEL_SECONDARY))
    {
        /* If so, set the channel */
        ret = cva_drv_indirect_channel_set(channel);
    }
    else
    {
        /* If not, set return value to RT_ERR_OUT_OF_RANGE */
        ret = -RT_ERR_OUT_OF_RANGE;
    }

    /* Return the result */
    return ret;
}

/** @} */ /* end of SYSTEM_INDIRECT_CHANNEL_ASIC */

/** @} */ /* end of SYSTEM_INDIRECT_CHANNEL_SET */

/**
 * @addtogroup SYSTEM_TIMER_INIT System timer
 * @brief Adjusts timer value.
 * @{
 */

/**
 * @addtogroup SYSTEM_TIMER_INIT_ASIC Low Level Driver
 * @brief System Low Level Driver
 * @{
 */

/**
 * @brief    A function used to adjust timer value \n
 *           DO NOT CALL THIS FUNCTION WTHOUT ANY DIRECTIVE FROM REALTEK \n
 *           This API is intentionally marked as static function
 *
 * @param[in] None
 * @param[out] None
 *
 * @retval RT_ERR_OK     Successfully initialized the timer.
 */
static RtkApiRet cva_timer_init(void)
{
    /* Initialize the timer using the driver function */
    cva_drv_timer_init();

    /* Return the result */
    return RT_ERR_OK;
}

/** @} */ /* end of SYSTEM_TIMER_INIT_ASIC */

/** @} */ /* end of SYSTEM_TIMER_INIT */

/** @} */ /* end of RTK_MODULE_SYSTEM */
