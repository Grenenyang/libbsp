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
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_sys_boot.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_sys_boot.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_sys_boot.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_sys_boot.h"
#endif

#include "rtkas_api_sys_boot.h"

/**
 * @defgroup RTK_MODULE_SYS_BOOT Flashless(System Boot-up)
 * @brief Flashless API. This component is used for external loading, referred to as a flashless update.
 * @{
 */

/**
 * @defgroup FL_SYS Update Firmware/Configuration via Register Interface
 * @brief External loading from register interface, I2C/SPI/SMI/PCIE...etc.
 * @{
 */
/**
 * @brief A function that checks if the system is in flashless mode and is ready for an external update. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysCheckFl Pointer to the structure containing the parameters for check flashless mode parameter (N/A) \n
 *            Contains bypassEth.
 * @retval RT_ERR_OK Successfully entered flashless mode.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sys_check_flashless_mode(),dva_sys_check_flashless_mode(),cvb_sys_check_flashless_mode(),cva_sys_check_flashless_mode().
 */
RtkApiRet rtk_sys_check_flashless_mode(UnitChip_t unitChip, SysCheckFl_t *sysCheckFl)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sysCheckFl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sys_check_flashless_mode(unitChip.unit, sysCheckFl->byPassEth);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sys_check_flashless_mode(unitChip.unit, sysCheckFl->byPassEth);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sys_check_flashless_mode(unitChip.unit, sysCheckFl->byPassEth);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sys_check_flashless_mode(unitChip.unit, sysCheckFl->byPassEth);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief A function that transitions from the firmware loading stage to the configuration loading stage. \n
 *        It will trigger firmware authentication if firmware exists. \n
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @retval RT_ERR_OK Update success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_sys_leave_fw_to_cfg().
 */
RtkApiRet rtk_sys_leave_fw_to_cfg(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_leave_fw_to_cfg(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}
/**
 * @brief A function used to leave flashless mode. \n
 *        It will trigger firmware authentication if firmware exists for RTL907XD_V1/RTL907XC_VB/RTL907XC_VA. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysCheckFl Pointer to the structure containing the parameters for check flashless mode parameter (N/A) \n
 *            Contains bypassEth.
 * @retval RT_ERR_OK  Successfully triggered exit from flashless mode.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_sys_leave_flashless(),dva_sys_leave_flashless(),cvb_sys_leave_flashless(),cva_sys_leave_flashless().
 */
RtkApiRet rtk_sys_leave_flashless(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_leave_flashless(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_leave_flashless(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_leave_flashless(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_leave_flashless(unitChip.unit);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief A function to check if leaving flashless is successful. \n
 *        It will check firmware status is "<CHIPID>_SYS_BOOT_UP_FULL_INITIZED" and external interrupt event is "<CHIPID>_EXINT_BOOT_INITZED_CONFIG_SUCCESS". \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @retval RT_ERR_OK Successfully left flashless mode
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_sys_check_leave_flashless(),dva_sys_check_leave_flashless(),cvb_sys_check_leave_flashless(),cva_sys_check_leave_flashless().
 */
RtkApiRet rtk_sys_check_leave_flashless(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_check_leave_flashless(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_check_leave_flashless(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_check_leave_flashless(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_check_leave_flashless(unitChip.unit);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief A function used to update firmware to SRAM in flashless mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysUpdateFw Pointer to the structure containing the parameters for update firmware parameter via register Interface(N/A) \n
 *            Contains filename.
 * @retval RT_ERR_OK Successfully updated firmware.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sys_update_firmware(),dva_sys_update_firmware(),cvb_sys_update_firmware(),cva_sys_update_firmware().
 * @note A binary with firmware should be input for this function.
 */
RtkApiRet rtk_sys_update_firmware(UnitChip_t unitChip, SysUpdateFw_t *sysUpdateFw)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sysUpdateFw)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sys_update_firmware(unitChip.unit, sysUpdateFw->fileName);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sys_update_firmware(unitChip.unit, sysUpdateFw->fileName);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sys_update_firmware(unitChip.unit, sysUpdateFw->fileName);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sys_update_firmware(unitChip.unit, sysUpdateFw->fileName);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**
 * @brief A function used to update configuration in flashless mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysUpdateCfg Pointer to the structure containing the parameters for update config parameter via register Interface (N/A) \n
 *            Contains filename.
 * @retval RT_ERR_OK Successfully updated configuration.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sys_update_config(),dva_sys_update_config(),cvb_sys_update_config(),cva_sys_update_config().
 * @note A binary with no secure boot firmware should be input for this function except RTL907XD_VA.
 */
RtkApiRet rtk_sys_update_config(UnitChip_t unitChip, SysUpdateCfg_t *sysUpdateCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sysUpdateCfg)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sys_update_config(unitChip.unit, sysUpdateCfg->fileName);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sys_update_config(unitChip.unit, sysUpdateCfg->fileName);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sys_update_config(unitChip.unit, sysUpdateCfg->fileName);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sys_update_config(unitChip.unit, sysUpdateCfg->fileName);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /* FL_SYS */

/**
 * @defgroup FL_ETH_PORT_DISABLE Disable Port for Ethernet Access Flashless
 * @brief Disable the port for Ethernet Access under flashess mode.
 * @{
 */

/**
 * @brief Program OTP memory if needed to disable port for flashless over Ethernet Access. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysEthPortOtpDis Pointer to the structure containing the parameters for disable Ethernet Access port otp parameter (N/A) \n
 *            Contain userPort.
 * @retval RT_ERR_OK Successfully disabled the selected port for flashless over Ethernet Access.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sys_flashless_eth_port_otp_disable(),dva_sys_flashless_eth_port_otp_disable(),cvb_sys_flashless_eth_port_otp_disable(),
 *                cva_sys_flashless_eth_port_otp_disable().
 */
RtkApiRet rtk_sys_flashless_eth_port_otp_disable(UnitChip_t unitChip, SysEthPortOtpDis_t *sysEthPortOtpDis)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sysEthPortOtpDis)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sys_flashless_eth_port_otp_disable(unitChip.unit, sysEthPortOtpDis->userPort);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sys_flashless_eth_port_otp_disable(unitChip.unit, sysEthPortOtpDis->userPort);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sys_flashless_eth_port_otp_disable(unitChip.unit, sysEthPortOtpDis->userPort);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sys_flashless_eth_port_otp_disable(unitChip.unit, sysEthPortOtpDis->userPort);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /* FL_ETH_PORT_DISABLE */

/**
 * @defgroup FL_ETH Update Firmware/Configuration via Ethernet Access
 * @brief External loading from Ethernet Access.
 * @{
 */

/**
 * @brief A function that transitions from the firmware loading stage to the configuration loading stage via etherAccess. \n
 *        It will trigger firmware authentication if firmware exists. \n
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @retval RT_ERR_OK Update success.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_sys_eth_leave_fw_to_cfg().
 */
RtkApiRet rtk_sys_eth_leave_fw_to_cfg(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_eth_leave_fw_to_cfg(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief A function used to update firmware into SRAM via Ethernet Access in flashless mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysEthUpdateFw Pointer to the structure containing the parameters for update firmware parameter via Ethernet Access(N/A) \n
 *            Contain fileName.
 * @retval RT_ERR_OK Successfully updated firmware for flashless over Ethernet Access.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sys_eth_fw_update(),dva_sys_eth_fw_update(),cvb_sys_eth_update_firmware(),cva_sys_eth_update_firmware().
 * @note A binary with firmware should be input for this function.
 */
RtkApiRet rtk_sys_eth_update_firmware(UnitChip_t unitChip, SysEthUpdateFw_t *sysEthUpdateFw)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sysEthUpdateFw)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sys_eth_fw_update(unitChip.unit, sysEthUpdateFw->fileName);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sys_eth_fw_update(unitChip.unit, sysEthUpdateFw->fileName);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sys_eth_update_firmware(unitChip.unit, sysEthUpdateFw->fileName);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sys_eth_update_firmware(unitChip.unit, sysEthUpdateFw->fileName);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief A function used to update configuration in flashless mode via Ethernet Access. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysEthUpdateCfg Pointer to the structure containing the parameters for update configuration via Ethernet Access (N/A) \n
 *            Contains fileName and userPort.
 * @retval RT_ERR_OK Successfully updated configuration for flashless over Ethernet Access.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sys_eth_cfg_update(),dva_sys_eth_cfg_update(),cvb_sys_eth_update_config(),cva_sys_eth_update_config().
 * @note A binary with no secure boot firmware should be input for this function except RTL907XD_VA.
 */
RtkApiRet rtk_sys_eth_update_config(UnitChip_t unitChip, SysEthUpdateCfg_t *sysEthUpdateCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sysEthUpdateCfg)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sys_eth_cfg_update(unitChip.unit, sysEthUpdateCfg->userPort, sysEthUpdateCfg->fileName);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sys_eth_cfg_update(unitChip.unit, sysEthUpdateCfg->fileName);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sys_eth_update_config(unitChip.unit, sysEthUpdateCfg->userPort, sysEthUpdateCfg->fileName);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sys_eth_update_config(unitChip.unit, sysEthUpdateCfg->userPort, sysEthUpdateCfg->fileName);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief A function used to disable the RX capability of all ports except the configured port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *sysEthPortRxDis Pointer to the structure containing the parameters for disabling port RX (N/A) \n
 *            Contains userPort.
 * @retval RT_ERR_OK Successfully disabled the RX capability of all ports except the configured port.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_sys_eth_disable_non_configure_port_rx(),dva_sys_eth_disable_non_configure_port_rx(),cvb_sys_eth_disable_non_configure_port_rx(),
 *                                cva_sys_eth_disable_non_configure_port_rx()
 */
RtkApiRet rtk_sys_eth_disable_non_configure_port_rx(UnitChip_t unitChip, SysEthPortRxDis_t *sysEthPortRxDis)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sysEthPortRxDis)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_sys_eth_disable_non_configure_port_rx(unitChip.unit, sysEthPortRxDis->userPort);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_sys_eth_disable_non_configure_port_rx(unitChip.unit, sysEthPortRxDis->userPort);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_sys_eth_disable_non_configure_port_rx(unitChip.unit, sysEthPortRxDis->userPort);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_sys_eth_disable_non_configure_port_rx(unitChip.unit, sysEthPortRxDis->userPort);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /* FL_ETH */
/**@}*/ /* System Bootsup */
