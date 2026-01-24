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
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api_dot1x.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_dot1x.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_dot1x.h"
#endif

#include "rtkas_api_dot1x.h"
/**
 * @defgroup RTK_MODULE_DOT1X DOT1X
 * @brief DOT1X APIs.  This section details the functions used to configure various settings for port-based DOT1X, including:
 *                      - Handshake configuration
 *                      - Aging settings
 *                      - Forced unauthorized access handling
 *                      - Register reset
 *                      - Unauthorized packet filtering
 *                      - Control direction settings
 * @{
 */

/**
 * @defgroup HANDSHAKE_CFG Handshake information configuration
 * @brief The configuration of port-based DOT1X handshake information.
 * @{
 */

/**
 * @brief Get port-based DOT1X enable port mask. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xEnPortMaskInfo   Port mask (xd_VA_RTK_USER_PMAP_1ST to xd_VA_RTK_USER_PMAP_ALL) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK                Port-based DOT1X enable port mask was gotten.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_get_enable_portmask(), cvb_dot1x_get_enable_portmask(), cva_dot1x_get_enable_portmask()
 */
RtkApiRet rtk_dot1x_get_enable_portmask(UnitChip_t unitChip, Dot1xEnPortMaskInfo_t *dot1xEnPortMaskInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xEnPortMaskInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_enable_portmask(unitChip.unit, &dot1xEnPortMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_enable_portmask(unitChip.unit, &dot1xEnPortMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_enable_portmask(unitChip.unit, &dot1xEnPortMaskInfo->portMask);
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
 * @brief Set port-based DOT1X enable port mask. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xEnPortMaskInfo    Port mask (xd_VA_RTK_USER_PMAP_1ST to xd_VA_RTK_USER_PMAP_ALL) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK                The DOT1X enable port mask was set.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_set_enable_portmask(), cvb_dot1x_set_enable_portmask(), cva_dot1x_set_enable_portmask()
 */
RtkApiRet rtk_dot1x_set_enable_portmask(UnitChip_t unitChip, Dot1xEnPortMaskInfo_t *dot1xEnPortMaskInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xEnPortMaskInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_enable_portmask(unitChip.unit, dot1xEnPortMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_enable_portmask(unitChip.unit, dot1xEnPortMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_enable_portmask(unitChip.unit, dot1xEnPortMaskInfo->portMask);
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
 * @brief Get the secret key from the switch. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] dot1xSecretKeyInfo    DOT1X secret key parameters (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK                The secret key was gotten.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_get_secret_key(), cvb_dot1x_get_secret_key(), cva_dot1x_get_secret_key()
 */
RtkApiRet rtk_dot1x_get_secret_key(UnitChip_t unitChip, Dot1xSecretKeyInfo_t *dot1xSecretKeyInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xSecretKeyInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_secret_key(unitChip.unit, dot1xSecretKeyInfo->port, dot1xSecretKeyInfo->secretKeyLength, dot1xSecretKeyInfo->pSecretKey);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_secret_key(unitChip.unit, dot1xSecretKeyInfo->port, dot1xSecretKeyInfo->secretKeyLength, dot1xSecretKeyInfo->pSecretKey);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_secret_key(unitChip.unit, dot1xSecretKeyInfo->port, dot1xSecretKeyInfo->secretKeyLength, dot1xSecretKeyInfo->pSecretKey);
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
 * @brief Set the secret key in the switch. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xSecretKeyInfo    DOT1X secret key parameters (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK                The secret key was set.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_set_secret_key(), cvb_dot1x_set_secret_key(), cva_dot1x_set_secret_key()
 */
RtkApiRet rtk_dot1x_set_secret_key(UnitChip_t unitChip, Dot1xSecretKeyInfo_t *dot1xSecretKeyInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xSecretKeyInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_secret_key(unitChip.unit, dot1xSecretKeyInfo->port, dot1xSecretKeyInfo->secretKeyLength, dot1xSecretKeyInfo->pSecretKey);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_secret_key(unitChip.unit, dot1xSecretKeyInfo->port, dot1xSecretKeyInfo->secretKeyLength, dot1xSecretKeyInfo->pSecretKey);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_secret_key(unitChip.unit, dot1xSecretKeyInfo->port, dot1xSecretKeyInfo->secretKeyLength, dot1xSecretKeyInfo->pSecretKey);
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
 * @brief Get the switch IP Address. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xSwitchIpInfo   DOT1X switch IP parameters (N/A) \n
 *                             Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK                DOT1X switch IP Address was gotten.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_get_switch_ip(), cvb_dot1x_get_switch_ip(), cva_dot1x_get_switch_ip()
 *
 */
RtkApiRet rtk_dot1x_get_switch_ip(UnitChip_t unitChip, Dot1xSwitchIpInfo_t *dot1xSwitchIpInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xSwitchIpInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_switch_ip(unitChip.unit, dot1xSwitchIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_switch_ip(unitChip.unit, dot1xSwitchIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_switch_ip(unitChip.unit, dot1xSwitchIpInfo->pIpAddr);
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
 * @brief Set the switch IP address. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xSwitchIpInfo        DOT1X switch IP parameters (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK               DOT1X switch IP address was set.
 * @retval -RT_ERR_NOT_SUPPORT     Not supported.
 * @retval -RT_ERR_NULL_POINTER    The input parameter is NULL.
 * @retval Others                  Please refer to dva_dot1x_get_switch_ip(), cvb_dot1x_set_switch_ip(), cva_dot1x_set_switch_ip()
 */
RtkApiRet rtk_dot1x_set_switch_ip(UnitChip_t unitChip, Dot1xSwitchIpInfo_t *dot1xSwitchIpInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xSwitchIpInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_switch_ip(unitChip.unit, dot1xSwitchIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_switch_ip(unitChip.unit, dot1xSwitchIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_switch_ip(unitChip.unit, dot1xSwitchIpInfo->pIpAddr);
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
 * @brief Get the RADIUS server IP address from the switch. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xRadiusIpInfo   RADIUS server IP parameters (N/A) \n
 *                             Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK                RADIUS server IP address was gotten.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_get_radius_server_ip(), cvb_dot1x_get_radius_server_ip(), cva_dot1x_get_radius_server_ip()
 */
RtkApiRet rtk_dot1x_get_radius_server_ip(UnitChip_t unitChip, Dot1xRadiusIpInfo_t *dot1xRadiusIpInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xRadiusIpInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_radius_server_ip(unitChip.unit, dot1xRadiusIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_radius_server_ip(unitChip.unit, dot1xRadiusIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_radius_server_ip(unitChip.unit, dot1xRadiusIpInfo->pIpAddr);
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
 * @brief Set the RADIUS server IP address in the switch. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xRadiusIpInfo   RADIUS server IP parameters (N/A) \n
 *                             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK                RADIUS server IP address was set.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_set_radius_server_ip(), cvb_dot1x_set_radius_server_ip(), cva_dot1x_set_radius_server_ip()
 */
RtkApiRet rtk_dot1x_set_radius_server_ip(UnitChip_t unitChip, Dot1xRadiusIpInfo_t *dot1xRadiusIpInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xRadiusIpInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_radius_server_ip(unitChip.unit, dot1xRadiusIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_radius_server_ip(unitChip.unit, dot1xRadiusIpInfo->pIpAddr);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_radius_server_ip(unitChip.unit, dot1xRadiusIpInfo->pIpAddr);
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
 * @brief Get the RADIUS UDP port. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xUdpPortInfo           Port number parameters (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK                RADIUS UDP port was gotten.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_get_radius_udp_port(), cvb_dot1x_get_radius_udp_port(), cva_dot1x_get_radius_udp_port()
 */
RtkApiRet rtk_dot1x_get_radius_udp_port(UnitChip_t unitChip, Dot1xUdpPortInfo_t *dot1xUdpPortInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xUdpPortInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_radius_udp_port(unitChip.unit, &dot1xUdpPortInfo->portNum);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_radius_udp_port(unitChip.unit, &dot1xUdpPortInfo->portNum);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_radius_udp_port(unitChip.unit, &dot1xUdpPortInfo->portNum);
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
 * @brief Set the RADIUS UDP port. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xUdpPortInfo           Port number parameters (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK                RADIUS UDP port was set.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_set_radius_udp_port(), cvb_dot1x_set_radius_udp_port(), cva_dot1x_set_radius_udp_port()
 */
RtkApiRet rtk_dot1x_set_radius_udp_port(UnitChip_t unitChip, Dot1xUdpPortInfo_t *dot1xUdpPortInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xUdpPortInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_radius_udp_port(unitChip.unit, dot1xUdpPortInfo->portNum);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_radius_udp_port(unitChip.unit, dot1xUdpPortInfo->portNum);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_radius_udp_port(unitChip.unit, dot1xUdpPortInfo->portNum);
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
 * @brief Get the switch port mask connected to the RADIUS server. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xradiusPMaskInfo           Port mask parameters (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK                Port mask was gotten.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_get_radius_server_portmask(), cvb_dot1x_get_radius_server_portmask(), cva_dot1x_get_radius_server_portmask()
 */
RtkApiRet rtk_dot1x_get_radius_server_portmask(UnitChip_t unitChip, Dot1xRadiusPMaskInfo_t *dot1xradiusPMaskInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xradiusPMaskInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_radius_server_portmask(unitChip.unit, &dot1xradiusPMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_radius_server_portmask(unitChip.unit, &dot1xradiusPMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_radius_server_portmask(unitChip.unit, &dot1xradiusPMaskInfo->portMask);
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
 * @brief Set the switch port mask connected to the RADIUS server. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xradiusPMaskInfo          Port mask parameters (N/A) \n
 *                              Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK            Port mask was set.
 * @retval -RT_ERR_NOT_SUPPORT  Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others               Please refer to dva_dot1x_get_radius_server_portmask(), cvb_dot1x_set_radius_server_portmask(), cva_dot1x_set_radius_server_portmask()
 */
RtkApiRet rtk_dot1x_set_radius_server_portmask(UnitChip_t unitChip, Dot1xRadiusPMaskInfo_t *dot1xradiusPMaskInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xradiusPMaskInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_radius_server_portmask(unitChip.unit, dot1xradiusPMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_radius_server_portmask(unitChip.unit, dot1xradiusPMaskInfo->portMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_radius_server_portmask(unitChip.unit, dot1xradiusPMaskInfo->portMask);
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
 * @brief Set the method for searching the RADIUS server. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xArpMethodInfo             The method parameters for searching the RADIUS server (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK                The method for searching the RADIUS server was set.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_set_arp_port_select(), cvb_dot1x_set_arp_port_select(), cva_dot1x_set_arp_port_select()
 */
RtkApiRet rtk_dot1x_set_arp_port_select(UnitChip_t unitChip, Dot1xArpMethodInfo_t *dot1xArpMethodInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xArpMethodInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_arp_port_select(unitChip.unit, dot1xArpMethodInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_arp_port_select(unitChip.unit, dot1xArpMethodInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_arp_port_select(unitChip.unit, dot1xArpMethodInfo->status);
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
 * @brief Get the method used for searching the RADIUS server. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xArpMethodInfo               The method parameters for searching the RADIUS server (N/A) \n
 *                                      Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK                    The method for searching the RADIUS server was gotten.
 * @retval -RT_ERR_NOT_SUPPORT          Not supported.
 * @retval -RT_ERR_NULL_POINTER         The input parameter is NULL.
 * @retval Others                       Please refer to dva_dot1x_get_arp_port_select(), cvb_dot1x_get_arp_port_select(), cva_dot1x_get_arp_port_select()
 */
RtkApiRet rtk_dot1x_get_arp_port_select(UnitChip_t unitChip, Dot1xArpMethodInfo_t *dot1xArpMethodInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xArpMethodInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_arp_port_select(unitChip.unit, &dot1xArpMethodInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_arp_port_select(unitChip.unit, &dot1xArpMethodInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_arp_port_select(unitChip.unit, &dot1xArpMethodInfo->status);
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
 * @brief Set the dynamic flag to trigger the DOT1X configuration. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK                The dynamic flag was set.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval Others                   Please refer to dva_dot1x_set_dynamic_reset_flag(), cvb_dot1x_set_dynamic_reset_flag(), cva_dot1x_set_dynamic_reset_flag()
 */
RtkApiRet rtk_dot1x_set_dynamic_reset_flag(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_dot1x_set_dynamic_reset_flag(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_dot1x_set_dynamic_reset_flag(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_dot1x_set_dynamic_reset_flag(unitChip.unit);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}
/**@}*/ /* HANDSHAKE_CFG */

/**
 * @defgroup DOT1X_AGING_TIME DOT1X Aging Time
 * @brief The configuration of the port-based DOT1X aging out function.
 * @{
 */

/**
 * @brief Get the enable status of the aging function. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xAgeStatusInfo The enable status parameters of the aging time function (N/A) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK              The enable status of the aging function was gotten.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_get_aging_time_enable(), cvb_dot1x_get_aging_time_enable(), cva_dot1x_get_aging_time_enable()
 */
RtkApiRet rtk_dot1x_get_aging_time_enable(UnitChip_t unitChip, Dot1xAgeStatusInfo_t *dot1xAgeStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xAgeStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_aging_time_enable(unitChip.unit, &dot1xAgeStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_aging_time_enable(unitChip.unit, &dot1xAgeStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_aging_time_enable(unitChip.unit, &dot1xAgeStatusInfo->status);
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
 * @brief Set the enable status of the aging function. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xAgeStatusInfo The enable status parameters of the aging time function (N/A) \n
 *                               Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK              The enable status of the aging function was set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_set_aging_time_enable(), cvb_dot1x_set_aging_time_enable(), cva_dot1x_set_aging_time_enable()
 */
RtkApiRet rtk_dot1x_set_aging_time_enable(UnitChip_t unitChip, Dot1xAgeStatusInfo_t *dot1xAgeStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xAgeStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_aging_time_enable(unitChip.unit, dot1xAgeStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_aging_time_enable(unitChip.unit, dot1xAgeStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_aging_time_enable(unitChip.unit, dot1xAgeStatusInfo->status);
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
 * @brief Get the value of the aging time. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xAgeOutValInfo Aging out timer parameters (0 to 0xFFFF) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK              The value of the aging time was gotten.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_get_aging_time(), cvb_dot1x_get_aging_time(), cva_dot1x_get_aging_time()
 */
RtkApiRet rtk_dot1x_get_aging_time(UnitChip_t unitChip, Dot1xAgeOutValInfo_t *dot1xAgeOutValInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xAgeOutValInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_aging_time(unitChip.unit, (uint16 *)&dot1xAgeOutValInfo->agingTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_aging_time(unitChip.unit, (uint16 *)&dot1xAgeOutValInfo->agingTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_aging_time(unitChip.unit, (uint16 *)&dot1xAgeOutValInfo->agingTime);
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
 * @brief Set the value of the aging time. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xAgeOutValInfo Aging out timer parameters (0 to 0xFFFF) \n
 *                               Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK              The value of the aging time was set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_set_aging_time(), cvb_dot1x_set_aging_time(), cva_dot1x_set_aging_time()
 */
RtkApiRet rtk_dot1x_set_aging_time(UnitChip_t unitChip, Dot1xAgeOutValInfo_t *dot1xAgeOutValInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xAgeOutValInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_aging_time(unitChip.unit, (uint16)dot1xAgeOutValInfo->agingTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_aging_time(unitChip.unit, (uint16)dot1xAgeOutValInfo->agingTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_aging_time(unitChip.unit, (uint16)dot1xAgeOutValInfo->agingTime);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* DOT1X_AGING_TIME */

/**
 * @defgroup FORCE_UNAUTH Force unauth
 * @brief The configuration of the port-based DOT1X force unauthorized function.
 * @{
 */

/**
 * @brief Set the enable status of the force unauthorized function. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xForceStatusInfo The enable status parameters of the force unauthorized function (N/A) \n
 *                                 Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK              The enable status of the force unauthorized function was set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_set_force_unauth_enable(), cvb_dot1x_set_force_unauth_enable(), cva_dot1x_set_force_unauth_enable()
 */
RtkApiRet rtk_dot1x_set_force_unauth_enable(UnitChip_t unitChip, Dot1xForceStatusInfo_t *dot1xForceStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xForceStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_set_force_unauth_enable(unitChip.unit, dot1xForceStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_set_force_unauth_enable(unitChip.unit, dot1xForceStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_set_force_unauth_enable(unitChip.unit, dot1xForceStatusInfo->status);
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
 * @brief Get the enable status of the force unauthorized function. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xForceStatusInfo The enable status parameters of the force unauthorized function (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK                The enable status of the force unauthorized function was gotten.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to dva_dot1x_get_force_unauth_enable(), cvb_dot1x_get_force_unauth_enable(), cva_dot1x_get_force_unauth_enable()
 */
RtkApiRet rtk_dot1x_get_force_unauth_enable(UnitChip_t unitChip, Dot1xForceStatusInfo_t *dot1xForceStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xForceStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_get_force_unauth_enable(unitChip.unit, &dot1xForceStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_get_force_unauth_enable(unitChip.unit, &dot1xForceStatusInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_get_force_unauth_enable(unitChip.unit, &dot1xForceStatusInfo->status);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* FORCE_UNAUTH */

/**
 * @defgroup DOT1X_RESET DOT1X register reset
 * @brief Reset the registers of port-based DOT1X to their default values.
 * @{
 */

/**
 * @brief Reset the DOT1X hardware registers to the default values. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK              The DOT1X hardware registers were reset to the default values.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval Others                 Please refer to dva_dot1x_hw_reset(), cvb_dot1x_hw_reset(), cva_dot1x_hw_reset()
 */
RtkApiRet rtk_dot1x_hw_reset(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_dot1x_hw_reset(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_dot1x_hw_reset(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_dot1x_hw_reset(unitChip.unit);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**@}*/ /* DOT1X_RESET */

/**
 * @defgroup INGRESS_FILTER Ingress Port-based DOT1X Filter
 * @brief The configuration of unauthenticated packet behavior at the ingress port.
 * @{
 */

/**
 * @brief Set the action configuration for unauthorized packets. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xUnauthInfo DOT1X unauthorized action parameters (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK              DOT1X unauthorized action configuration was set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_unauth_packet_oper_set(), cvb_dot1x_unauth_packet_oper_set(), cva_dot1x_unauth_packet_oper_set()
 */
RtkApiRet rtk_dot1x_unauth_packet_oper_set(UnitChip_t unitChip, Dot1xUnauthPktActionInfo_t *dot1xUnauthInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xUnauthInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_unauth_packet_oper_set(unitChip.unit, dot1xUnauthInfo->pktType, dot1xUnauthInfo->unauthAction);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_unauth_packet_oper_set(unitChip.unit, dot1xUnauthInfo->pktType, dot1xUnauthInfo->unauthAction);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_unauth_packet_oper_set(unitChip.unit, dot1xUnauthInfo->pktType, dot1xUnauthInfo->unauthAction);
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
 * @brief Get the action configuration for unauthorized packets. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] dot1xUnauthInfo DOT1X unauthorized action parameters (N/A) \n
 *                                Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK              DOT1X unauthorized action configuration was gotten.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_unauth_packet_oper_get(), cvb_dot1x_unauth_packet_oper_get(), cva_dot1x_unauth_packet_oper_get()
 */
RtkApiRet rtk_dot1x_unauth_packet_oper_get(UnitChip_t unitChip, Dot1xUnauthPktActionInfo_t *dot1xUnauthInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xUnauthInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_unauth_packet_oper_get(unitChip.unit, dot1xUnauthInfo->pktType, &dot1xUnauthInfo->unauthAction);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_unauth_packet_oper_get(unitChip.unit, dot1xUnauthInfo->pktType, &dot1xUnauthInfo->unauthAction);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_unauth_packet_oper_get(unitChip.unit, dot1xUnauthInfo->pktType, &dot1xUnauthInfo->unauthAction);
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
 * @brief Get port-based DOT1X authorized status. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] dot1xAuthStatusInfo DOT1X authorized status parameters (N/A) \n
 *                                    Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK                  Port-based DOT1X authorized status was gotten.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval -RT_ERR_NULL_POINTER       The input parameter is NULL.
 * @retval Others                     Please refer to dva_dot1x_portbased_auth_get(), cvb_dot1x_portbased_auth_get(), cva_dot1x_portbased_auth_get()
 */
RtkApiRet rtk_dot1x_portbased_auth_get(UnitChip_t unitChip, Dot1xGetAuthStatus_t *dot1xAuthStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xAuthStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_portbased_auth_get(unitChip.unit, dot1xAuthStatusInfo->port, &dot1xAuthStatusInfo->portAuth);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_portbased_auth_get(unitChip.unit, dot1xAuthStatusInfo->port, &dot1xAuthStatusInfo->portAuth);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_portbased_auth_get(unitChip.unit, dot1xAuthStatusInfo->port, &dot1xAuthStatusInfo->portAuth);
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
 * @brief Set DOT1X guest VLAN entry. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xGuestVidEntryInfo DOT1X guest VLAN configuration parameters (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK              DOT1X guest VLAN entry was set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_guest_vlan_set(), cvb_dot1x_guest_vlan_entry_set(), cva_dot1x_guest_vlan_entry_set()
 */
RtkApiRet rtk_dot1x_guest_vlan_entry_set(UnitChip_t unitChip, Dot1xGuestVidEntryInfo_t *dot1xGuestVidEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xGuestVidEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_guest_vlan_entry_set(unitChip.unit, dot1xGuestVidEntryInfo->vid, &dot1xGuestVidEntryInfo->pVlanEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_guest_vlan_entry_set(unitChip.unit, dot1xGuestVidEntryInfo->vid, &dot1xGuestVidEntryInfo->pVlanEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_guest_vlan_entry_set(unitChip.unit, dot1xGuestVidEntryInfo->vid, &dot1xGuestVidEntryInfo->pVlanEntry);
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
 * @brief Get DOT1X guest VLAN entry. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xGuestVidEntryInfo DOT1X guest VLAN configuration parameters (N/A) \n
 *                                    Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK                  DOT1X guest VLAN entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval -RT_ERR_NULL_POINTER       The input parameter is NULL.
 * @retval Others                     Please refer to dva_dot1x_guest_vid_get(), cvb_dot1x_guest_vlan_entry_get(), cva_dot1x_guest_vlan_entry_get()
 */
RtkApiRet rtk_dot1x_guest_vlan_entry_get(UnitChip_t unitChip, Dot1xGuestVidEntryInfo_t *dot1xGuestVidEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xGuestVidEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_guest_vlan_entry_get(unitChip.unit, &dot1xGuestVidEntryInfo->vid, &dot1xGuestVidEntryInfo->pVlanEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_guest_vlan_entry_get(unitChip.unit, &dot1xGuestVidEntryInfo->vid, &dot1xGuestVidEntryInfo->pVlanEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_guest_vlan_entry_get(unitChip.unit, &dot1xGuestVidEntryInfo->vid, &dot1xGuestVidEntryInfo->pVlanEntry);
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
 * @brief Get DOT1X CPU port mask. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dot1xCpuPMaskInfo CPU port mask parameters (N/A) \n
 *                               Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK             DOT1X CPU port mask was gotten.
 * @retval -RT_ERR_NOT_SUPPORT   Not supported.
 * @retval -RT_ERR_NULL_POINTER  The input parameter is NULL.
 * @retval Others                Please refer to dva_dot1x_cpuportmask_get(), cvb_dot1x_cpuportmask_get(), cva_dot1x_cpuportmask_get()
 */
RtkApiRet rtk_dot1x_cpuportmask_get(UnitChip_t unitChip, Dot1xCpuPMaskInfo_t *dot1xCpuPMaskInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xCpuPMaskInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_cpuportmask_get(unitChip.unit, &dot1xCpuPMaskInfo->cpuPortMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_cpuportmask_get(unitChip.unit, &dot1xCpuPMaskInfo->cpuPortMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_cpuportmask_get(unitChip.unit, &dot1xCpuPMaskInfo->cpuPortMask);
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
 * @brief Set DOT1X CPU port mask. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dot1xCpuPMaskInfo CPU port mask parameters (N/A) \n
 *                              Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK              DOT1X CPU port mask was set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_cpuportmask_set(), cvb_dot1x_cpuportmask_set(), cva_dot1x_cpuportmask_set()
 *
 */
RtkApiRet rtk_dot1x_cpuportmask_set(UnitChip_t unitChip, Dot1xCpuPMaskInfo_t *dot1xCpuPMaskInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xCpuPMaskInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_cpuportmask_set(unitChip.unit, dot1xCpuPMaskInfo->cpuPortMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_cpuportmask_set(unitChip.unit, dot1xCpuPMaskInfo->cpuPortMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_cpuportmask_set(unitChip.unit, dot1xCpuPMaskInfo->cpuPortMask);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* INGRESS_FILTER */

/**
 * @defgroup CONTROL_DIRECTION Port-based DOT1X control direction
 * @brief The configuration of port-based DOT1X control direction.
 * @{
 */

/**
 * @brief Set port-based DOT1X control direction configuration. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] direction port-based DOT1X  control direction parameters (N/A) \n
 *                      Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK              port-based DOT1X  operational direction configuration was set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dva_dot1x_portbased_direction_set(), cvb_dot1x_portbased_direction_set(), cva_dot1x_portbased_direction_set()
 */
RtkApiRet rtk_dot1x_portbased_direction_set(UnitChip_t unitChip, Dot1xDirInfo_t *dot1xDirInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xDirInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_portbased_direction_set(unitChip.unit, dot1xDirInfo->port, dot1xDirInfo->portDirection);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_portbased_direction_set(unitChip.unit, dot1xDirInfo->port, dot1xDirInfo->portDirection);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_portbased_direction_set(unitChip.unit, dot1xDirInfo->port, dot1xDirInfo->portDirection);
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
 * @brief Get port-based DOT1X control direction configuration. \n
 *        Supported chips: RTL907XD_VA/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] dot1xDirInfo port-based DOT1X  control direction parameters (N/A) \n
 *                             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK            Port-based DOT1X control direction configuration was gotten.
 * @retval -RT_ERR_NOT_SUPPORT  Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others               Please refer to dva_dot1x_portbased_direction_get(), cvb_dot1x_portbased_direction_get(), cva_dot1x_portbased_direction_get()
 */
RtkApiRet rtk_dot1x_portbased_direction_get(UnitChip_t unitChip, Dot1xDirInfo_t *dot1xDirInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == dot1xDirInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_dot1x_portbased_direction_get(unitChip.unit, dot1xDirInfo->port, (uint32 *)&dot1xDirInfo->portDirection);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_dot1x_portbased_direction_get(unitChip.unit, dot1xDirInfo->port, (uint32 *)&dot1xDirInfo->portDirection);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_dot1x_portbased_direction_get(unitChip.unit, dot1xDirInfo->port, (uint32 *)&dot1xDirInfo->portDirection);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* CONTROL_DIRECTION */
/**@}*/ /* DOT1X */
