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
#include "rtkas_error.h"
#include "rtkas_types.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_interface.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_interface.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_api_interface.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_api_interface.h"
#endif
#include "rtkas_api_interface.h"

/**
 * @defgroup RTK_MODULE_INTERFACE Interface
 * @brief Interface APIs. The Interface API allows users to manage switch interface configurations and retrieve port link status.
 * @{
 */

/**
 * @defgroup INTF_T1_CFG T1 Interface Configuration
 * @brief Configure the T1 interface
 * @{
 */

/**
 * @brief Switches a specific T1 port between 100Base-T1 and 1000Base-T1.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA.
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] pPortModeSet Pointer to the port mode set information.
 *                             Refer to the struct field descriptions in the header files for detailed information.
 *
 * @retval RT_ERR_OK if the function executes successfully.
 * @retval -RT_ERR_NOT_SUPPORT if the operation is not supported.
 * @retval Other error codes as defined in dv1_port_t1_mode_set(), dva_port_t1_mode_set(),
 *         cvb_port_t1_mode_set(), and cva_port_t1_mode_set().
 */
RtkApiRet rtk_port_t1_mode_set(UnitChip_t unitChip, PortMode_t *pPortModeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* check NULL parameter */
        if(NULL == pPortModeSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_t1_mode_set(unitChip.unit, pPortModeSet->userPort, pPortModeSet->mode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_t1_mode_set(unitChip.unit, pPortModeSet->userPort, pPortModeSet->mode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_t1_mode_set(unitChip.unit, pPortModeSet->userPort, pPortModeSet->mode);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_t1_mode_set(unitChip.unit, pPortModeSet->userPort, pPortModeSet->mode);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Configure the T1 interface for a specified port in flashless over ethernet mode. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip Stacking unit and chip ID. \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortT1ConfigSet Configuration parameters for the T1 interface. \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NULL_POINTER Null pointer error.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_port_t1_config_set().
 */
RtkApiRet rtk_port_t1_config_set(UnitChip_t unitChip, PortT1ConfigSet_t *pPortT1ConfigSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortT1ConfigSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_t1_config_set(unitChip.unit, pPortT1ConfigSet->userPort, pPortT1ConfigSet->portMode, pPortT1ConfigSet->isMaster, pPortT1ConfigSet->speed);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* INTF_T1_CFG */

/**
 * @defgroup INTF_LINK_STS Interface Mode and Link Status
 * @brief Get Interface Mode and Current Link Status
 * @{
 */

/**
 * @brief Get the current port mode type of the addressed port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA.
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortModeGet The port mode get information (N/A). \n
 *             Refer to the struct field descriptions in the .h files for detailed information.
 * @param[out] None
 * @retval RT_ERR_OK If the function executes successfully.
 * @retval -RT_ERR_NOT_SUPPORT If the operation is not supported.
 * @retval Others Refer to the following functions: dv1_port_mode_get(), dva_port_mode_get(),
 *                cvb_port_mode_get(), and cva_port_mode_get().
 */
RtkApiRet rtk_port_mode_get(UnitChip_t unitChip, PortMode_t *pPortModeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* check NULL parameter */
        if(NULL == pPortModeGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_mode_get(unitChip.unit, pPortModeGet->userPort, &(pPortModeGet->mode));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_mode_get(unitChip.unit, pPortModeGet->userPort, &(pPortModeGet->mode));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_mode_get(unitChip.unit, pPortModeGet->userPort, &(pPortModeGet->mode));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_mode_get(unitChip.unit, pPortModeGet->userPort, &(pPortModeGet->mode));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the link status of the addressed port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortLinkStatus Port link status (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_link_status_get(), dva_port_link_status_get(), cvb_port_link_status_get() and cva_port_link_status_get().
 */
RtkApiRet rtk_port_link_status_get(UnitChip_t unitChip, PortLinkStatusGet_t *pPortLinkStatusGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortLinkStatusGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_link_status_get(unitChip.unit, pPortLinkStatusGet->userPort, &pPortLinkStatusGet->status);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_link_status_get(unitChip.unit, pPortLinkStatusGet->userPort, &pPortLinkStatusGet->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_link_status_get(unitChip.unit, pPortLinkStatusGet->userPort, &pPortLinkStatusGet->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_link_status_get(unitChip.unit, pPortLinkStatusGet->userPort, &pPortLinkStatusGet->status);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set the packet maximum length configuration of the addressed port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortPktMaxlenSet The packet maximum length information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_pkt_maxlen_set(), dva_port_pkt_maxlen_set(), cvb_port_pkt_maxlen_set() and cva_port_pkt_maxlen_set().
 */
RtkApiRet rtk_port_pkt_maxlen_set(UnitChip_t unitChip, PortPktMaxlenSet_t *pPortPktMaxlenSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortPktMaxlenSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_pkt_maxlen_set(unitChip.unit, pPortPktMaxlenSet->enable, pPortPktMaxlenSet->userPort);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_pkt_maxlen_set(unitChip.unit, pPortPktMaxlenSet->enable, pPortPktMaxlenSet->userPort);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_pkt_maxlen_set(unitChip.unit, pPortPktMaxlenSet->enable, pPortPktMaxlenSet->userPort);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_pkt_maxlen_set(unitChip.unit, pPortPktMaxlenSet->enable, pPortPktMaxlenSet->userPort);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the packet maximum length configuration of the addressed port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortPktMaxlenGet packet maximum length information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_pkt_maxlen_get(), dva_port_pkt_maxlen_get(), cvb_port_pkt_maxlen_get() and cva_port_pkt_maxlen_get().
 */
RtkApiRet rtk_port_pkt_maxlen_get(UnitChip_t unitChip, PortPktMaxlenGet_t *pPortPktMaxlenGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortPktMaxlenGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_pkt_maxlen_get(unitChip.unit, pPortPktMaxlenGet->userPort, &(pPortPktMaxlenGet->maxLen));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_pkt_maxlen_get(unitChip.unit, pPortPktMaxlenGet->userPort, &(pPortPktMaxlenGet->maxLen));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_pkt_maxlen_get(unitChip.unit, pPortPktMaxlenGet->userPort, &(pPortPktMaxlenGet->maxLen));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_pkt_maxlen_get(unitChip.unit, pPortPktMaxlenGet->userPort, &(pPortPktMaxlenGet->maxLen));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* INTF_LINK_STS */

/**
 * @defgroup ETH_LINK_CFG Ethernet Link Configuration
 * @brief Ethernet interface configuration setting and getting APIs.
 * @{
 */

/**
 * @brief Set the Ethernet port link configuration. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA.
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEthLinkAbilitySet The Ethernet link ability set information (N/A). \n
 *                Refer to the struct field descriptions in the .h files for detailed information.
 * @param[out] None
 * @retval RT_ERR_OK If the function executes successfully.
 * @retval -RT_ERR_NOT_SUPPORT If the operation is not supported.
 * @retval Others Refer to the following functions: dv1_port_eth_link_ability_set(), dva_port_eth_link_ability_set(),
 *                cvb_port_eth_link_ability_set(), and cva_port_eth_link_ability_set().
 */
RtkApiRet rtk_port_eth_link_ability_set(UnitChip_t unitChip, PortEthLinkAbilitySet_t *pEthLinkAbilitySet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEthLinkAbilitySet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_eth_link_ability_set(unitChip.unit, pEthLinkAbilitySet->userPort, pEthLinkAbilitySet->pEthAbility);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_eth_link_ability_set(unitChip.unit, pEthLinkAbilitySet->userPort, pEthLinkAbilitySet->pEthAbility);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_eth_link_ability_set(unitChip.unit, pEthLinkAbilitySet->userPort, pEthLinkAbilitySet->pEthAbility);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_eth_link_status_set(unitChip.unit, pEthLinkAbilitySet->userPort, pEthLinkAbilitySet->pEthAbility);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get the link status of the addressed Ethernet port \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pEthLinkAbility Ethernet link ability get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_eth_link_ability_get(), dva_port_eth_link_ability_get(),
 *                cvb_port_eth_link_ability_get() and cva_port_eth_link_ability_get().
 */
RtkApiRet rtk_port_eth_link_ability_get(UnitChip_t unitChip, PortEthLinkAbilityGet_t *pEthLinkAbilityGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pEthLinkAbilityGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_eth_link_ability_get(unitChip.unit, pEthLinkAbilityGet->userPort, pEthLinkAbilityGet->pStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_eth_link_ability_get(unitChip.unit, pEthLinkAbilityGet->userPort, pEthLinkAbilityGet->pStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_eth_link_ability_get(unitChip.unit, pEthLinkAbilityGet->userPort, pEthLinkAbilityGet->pStatus);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_eth_link_status_get(unitChip.unit, pEthLinkAbilityGet->userPort, pEthLinkAbilityGet->pStatus);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Configure the FE interface for a specified port in flashless over ethernet mode. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip Stacking unit and chip ID. \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortFeConfigSet Configuration parameters for the FE interface. \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NULL_POINTER Null pointer error.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_port_fe_config_set().
 */
RtkApiRet rtk_port_fe_config_set(UnitChip_t unitChip, PortFeConfigSet_t *pPortFeConfigSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortFeConfigSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_fe_config_set(unitChip.unit, pPortFeConfigSet->userPort, pPortFeConfigSet->portMode, pPortFeConfigSet->anEnable, pPortFeConfigSet->speed);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* ETH_LINK_CFG */

/**
 * @defgroup INTF_RGMII_CFG RGMII Interface Configuration
 * @brief RGMII interface configuration setting and getting APIs.
 * @{
 */

/**
 * @brief Set RGMII TX delay mode. The value 0 means disable TX delay mode, and 1 means enable TX delay mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out]  *pPortRgmiiTxdly Set port rgmii tx delay set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_rgmii_txdly_set(), dva_port_rgmii_txdly_set(), cvb_port_rgmii_txdly_set() and cva_port_rgmii_txdly_set().
 */
RtkApiRet rtk_port_rgmii_txdly_set(UnitChip_t unitChip, PortRgmiiTxdly_t *pPortRgmiiTxdlySet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortRgmiiTxdlySet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_rgmii_txdly_set(unitChip.unit, pPortRgmiiTxdlySet->userPort, pPortRgmiiTxdlySet->delay);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_rgmii_txdly_set(unitChip.unit, pPortRgmiiTxdlySet->userPort, pPortRgmiiTxdlySet->delay);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_rgmii_txdly_set(unitChip.unit, pPortRgmiiTxdlySet->userPort, pPortRgmiiTxdlySet->delay);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_rgmii_txdly_set(unitChip.unit, pPortRgmiiTxdlySet->userPort, pPortRgmiiTxdlySet->delay);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get RGMII TX delay mode. The value 0 means disable TX delay mode, and 1 means enable TX delay mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortRgmiiTxdly Get port rgmii txdly get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_rgmii_txdly_get(), dva_port_rgmii_txdly_get(), cvb_port_rgmii_txdly_get() and cva_port_rgmii_txdly_get().
 */
RtkApiRet rtk_port_rgmii_txdly_get(UnitChip_t unitChip, PortRgmiiTxdly_t *pPortRgmiiTxdlyGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortRgmiiTxdlyGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_rgmii_txdly_get(unitChip.unit, pPortRgmiiTxdlyGet->userPort, &(pPortRgmiiTxdlyGet->delay));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_rgmii_txdly_get(unitChip.unit, pPortRgmiiTxdlyGet->userPort, &(pPortRgmiiTxdlyGet->delay));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_rgmii_txdly_get(unitChip.unit, pPortRgmiiTxdlyGet->userPort, &(pPortRgmiiTxdlyGet->delay));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_rgmii_txdly_get(unitChip.unit, pPortRgmiiTxdlyGet->userPort, &(pPortRgmiiTxdlyGet->delay));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set RGMII RX delay mode and the set RX delay time unit is ns. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out]  *pPortRgmiiRxdly Set port rgmii rx delay set information. (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_rgmii_rxdly_set(), dva_port_rgmii_rxdly_set(), cvb_port_rgmii_rxdly_set() and cva_port_rgmii_rxdly_set().
 */
RtkApiRet rtk_port_rgmii_rxdly_set(UnitChip_t unitChip, PortRgmiiRxdlySet_t *pPortRgmiiRxdlySet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortRgmiiRxdlySet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_rgmii_rxdly_set(unitChip.unit, pPortRgmiiRxdlySet->userPort, pPortRgmiiRxdlySet->speed, pPortRgmiiRxdlySet->delay);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_rgmii_rxdly_set(unitChip.unit, pPortRgmiiRxdlySet->userPort, pPortRgmiiRxdlySet->speed, pPortRgmiiRxdlySet->delay);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_rgmii_rxdly_set(unitChip.unit, pPortRgmiiRxdlySet->userPort, pPortRgmiiRxdlySet->speed, pPortRgmiiRxdlySet->delay);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_rgmii_rxdly_set(unitChip.unit, pPortRgmiiRxdlySet->userPort, pPortRgmiiRxdlySet->speed, pPortRgmiiRxdlySet->delay);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get RGMII RX delay mode and the get RX delay time unit is ns. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortRgmiiRxdly Get port rgmii rxdly get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_rgmii_rxdly_get(), dva_port_rgmii_rxdly_get(), cvb_port_rgmii_rxdly_get() and cva_port_rgmii_rxdly_get().
 */
RtkApiRet rtk_port_rgmii_rxdly_get(UnitChip_t unitChip, PortRgmiiRxdlyGet_t *pPortRgmiiRxdlyGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortRgmiiRxdlyGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_rgmii_rxdly_get(unitChip.unit, pPortRgmiiRxdlyGet->userPort, &(pPortRgmiiRxdlyGet->delay));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_rgmii_rxdly_get(unitChip.unit, pPortRgmiiRxdlyGet->userPort, &(pPortRgmiiRxdlyGet->delay));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_rgmii_rxdly_get(unitChip.unit, pPortRgmiiRxdlyGet->userPort, &(pPortRgmiiRxdlyGet->delay));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_rgmii_rxdly_get(unitChip.unit, pPortRgmiiRxdlyGet->userPort, &(pPortRgmiiRxdlyGet->delay));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* INTF_RGMII_CFG */

/**
 * @defgroup INTF_XMII_CFG XMII Interface Configuration
 * @brief Configure the XMII interface
 * @{
 */

/**
 * @brief Set the link configuration of the addressed XMII port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA.
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortLinkCfgSet The port link configuration. (N/A). \n
 *             Refer to the struct field descriptions in the .h files for detailed information.
 * @param[out] None
 * @retval RT_ERR_OK If the function executes successfully.
 * @retval -RT_ERR_NOT_SUPPORT If the operation is not supported.
 * @retval Others Refer to the following functions: dv1_port_link_status_get(), dva_port_link_status_get(),
 *                cvb_port_link_status_get(), and cva_port_link_status_get().
 */
RtkApiRet rtk_port_xmii_link_status_set(UnitChip_t unitChip, PortLinkConfigurationSet_t *pPortLinkCfgSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortLinkCfgSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_xmii_link_status_set(unitChip.unit, pPortLinkCfgSet->userPort, &pPortLinkCfgSet->macAbility);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_link_status_set(unitChip.unit, pPortLinkCfgSet->userPort, &pPortLinkCfgSet->macAbility);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_xmii_link_status_set(unitChip.unit, pPortLinkCfgSet->userPort, &pPortLinkCfgSet->macAbility);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_xmii_link_status_set(unitChip.unit, pPortLinkCfgSet->userPort, &pPortLinkCfgSet->macAbility);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set xMII TX driving strength which includes rising and falling. The bigger value means higher driving strength. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out]  *pPortXmiiDriving Set port xmii driving set information. (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_xmii_driving_set(), dva_port_xmii_driving_set(), cvb_port_xmii_driving_set() and cva_port_xmii_driving_set().
 */
RtkApiRet rtk_port_xmii_driving_set(UnitChip_t unitChip, PortXmiiDriving_t *pPortXmiiDrivingSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiDrivingSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_xmii_driving_set(unitChip.unit, pPortXmiiDrivingSet->userPort, pPortXmiiDrivingSet->base, pPortXmiiDrivingSet->rise, pPortXmiiDrivingSet->fall);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_driving_set(unitChip.unit, pPortXmiiDrivingSet->userPort, pPortXmiiDrivingSet->base, pPortXmiiDrivingSet->rise, pPortXmiiDrivingSet->fall);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_xmii_driving_set(unitChip.unit, pPortXmiiDrivingSet->userPort, pPortXmiiDrivingSet->base, pPortXmiiDrivingSet->rise, pPortXmiiDrivingSet->fall);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_xmii_driving_set(unitChip.unit, pPortXmiiDrivingSet->userPort, pPortXmiiDrivingSet->base, pPortXmiiDrivingSet->rise, pPortXmiiDrivingSet->fall);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get xMII TX driving strength which includes rising and falling. The bigger value means higher driving strength. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortXmiiDriving Get port xmii driving get information. (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_xmii_driving_get(), dva_port_xmii_driving_get(), cvb_port_xmii_driving_get() and cva_port_xmii_driving_get().
 */
RtkApiRet rtk_port_xmii_driving_get(UnitChip_t unitChip, PortXmiiDriving_t *pPortXmiiDrivingGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiDrivingGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_xmii_driving_get(unitChip.unit, pPortXmiiDrivingGet->userPort, &(pPortXmiiDrivingGet->base), &(pPortXmiiDrivingGet->rise), &(pPortXmiiDrivingGet->fall));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_driving_get(unitChip.unit, pPortXmiiDrivingGet->userPort, &(pPortXmiiDrivingGet->base), &(pPortXmiiDrivingGet->rise), &(pPortXmiiDrivingGet->fall));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_xmii_driving_get(unitChip.unit, pPortXmiiDrivingGet->userPort, &(pPortXmiiDrivingGet->base), &(pPortXmiiDrivingGet->rise), &(pPortXmiiDrivingGet->fall));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_xmii_driving_get(unitChip.unit, pPortXmiiDrivingGet->userPort, &(pPortXmiiDrivingGet->base), &(pPortXmiiDrivingGet->rise), &(pPortXmiiDrivingGet->fall));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set xMII RX and TX single sampling edge, user could set as positive latch or negative latch. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out]  *pPortXmiiLatchEdge Set port xmii latch edge set information. (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_xmii_latch_edge_set(), dva_port_xmii_latch_edge_set(), cvb_port_xmii_latch_edge_set() and cva_port_xmii_latch_edge_set().
 */
RtkApiRet rtk_port_xmii_latch_edge_set(UnitChip_t unitChip, PortXmiiLatchEdge_t *pPortXmiiLatchEdgeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiLatchEdgeSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_xmii_latch_edge_set(unitChip.unit, pPortXmiiLatchEdgeSet->userPort, pPortXmiiLatchEdgeSet->isRxNegSample, pPortXmiiLatchEdgeSet->isTxNegSample);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_latch_edge_set(unitChip.unit, pPortXmiiLatchEdgeSet->userPort, pPortXmiiLatchEdgeSet->isRxNegSample, pPortXmiiLatchEdgeSet->isTxNegSample);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_xmii_latch_edge_set(unitChip.unit, pPortXmiiLatchEdgeSet->userPort, pPortXmiiLatchEdgeSet->isRxNegSample, pPortXmiiLatchEdgeSet->isTxNegSample);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_xmii_latch_edge_set(unitChip.unit, pPortXmiiLatchEdgeSet->userPort, pPortXmiiLatchEdgeSet->isRxNegSample, pPortXmiiLatchEdgeSet->isTxNegSample);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get xMII RX and TX single sampling edge, user could set as positive latch or negative latch. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortXmiiLatchEdge Get port xmii latch edge get information. (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_xmii_latch_edge_get(), dva_port_xmii_latch_edge_get(), cvb_port_xmii_latch_edge_get() and cva_port_xmii_latch_edge_get().
 */
RtkApiRet rtk_port_xmii_latch_edge_get(UnitChip_t unitChip, PortXmiiLatchEdge_t *pPortXmiiLatchEdgeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiLatchEdgeGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_xmii_latch_edge_get(unitChip.unit, pPortXmiiLatchEdgeGet->userPort, &(pPortXmiiLatchEdgeGet->isRxNegSample), &(pPortXmiiLatchEdgeGet->isTxNegSample));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_latch_edge_get(unitChip.unit, pPortXmiiLatchEdgeGet->userPort, &(pPortXmiiLatchEdgeGet->isRxNegSample), &(pPortXmiiLatchEdgeGet->isTxNegSample));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_xmii_latch_edge_get(unitChip.unit, pPortXmiiLatchEdgeGet->userPort, &(pPortXmiiLatchEdgeGet->isRxNegSample), &(pPortXmiiLatchEdgeGet->isTxNegSample));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_xmii_latch_edge_get(unitChip.unit, pPortXmiiLatchEdgeGet->userPort, &(pPortXmiiLatchEdgeGet->isRxNegSample), &(pPortXmiiLatchEdgeGet->isTxNegSample));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Sets the signal spread to cover the widest frequency range. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *            The stacking unit ID indicates the switch to access. \n
 *            The chip ID indicates the chip model to access.
 * @param[in,out] *pPortXmiiMaxSscSet Pointer to port XMII max SSC set information. (N/A). \n
 *            Refer to struct field descriptions in .h files for details.
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly.
 * @retval -RT_ERR_PORT_ID      The port ID is out of range.
 * @retval Others               Please refer to dv1_port_xmii_ssc_set(), dva_port_xmii_ssc_set(), cvb_port_xmii_ssc_set() and cva_port_xmii_ssc_set().
 * @note This API set the signal to spread over the widest range of frequencies.
 */
RtkApiRet rtk_port_xmii_ssc_set(UnitChip_t unitChip, PortXmiiSsc_t *pPortXmiiMaxSscSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiMaxSscSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_xmii_ssc_set(unitChip.unit, pPortXmiiMaxSscSet->userPort, pPortXmiiMaxSscSet->isEnable, pPortXmiiMaxSscSet->sequenceModeOffset, pPortXmiiMaxSscSet->phaseOffset);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_ssc_set(unitChip.unit, pPortXmiiMaxSscSet->userPort, pPortXmiiMaxSscSet->isEnable, pPortXmiiMaxSscSet->sequenceModeOffset, pPortXmiiMaxSscSet->phaseOffset);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_xmii_ssc_set(unitChip.unit, pPortXmiiMaxSscSet->userPort, pPortXmiiMaxSscSet->isEnable, pPortXmiiMaxSscSet->sequenceModeOffset, pPortXmiiMaxSscSet->phaseOffset);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_xmii_ssc_set(unitChip.unit, pPortXmiiMaxSscSet->userPort, pPortXmiiMaxSscSet->isEnable, pPortXmiiMaxSscSet->sequenceModeOffset, pPortXmiiMaxSscSet->phaseOffset);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Gets the signal spread configuration for the widest frequency range. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID indicates the switch to access.
 *             Chip ID indicates the chip model to access.
 * @param[in,out] *pPortXmiiMaxSscGet Pointer to port XMII max SSC get information. (N/A). \n
 *            Refer to struct field descriptions in .h files for details.
 * @param[out] None
 * @retval  RT_ERR_OK           To execute this function correctly.
 * @retval -RT_ERR_PORT_ID      The port ID is out of range.
 * @retval Others               Please refer to dv1_port_xmii_ssc_get(), dva_port_xmii_ssc_get(), cvb_port_xmii_ssc_get() and cva_port_xmii_ssc_get().
 * @note This API get the signal to spread over the widest range of frequencies.
 */
RtkApiRet rtk_port_xmii_ssc_get(UnitChip_t unitChip, PortXmiiSsc_t *pPortXmiiSscGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiSscGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_xmii_ssc_get(unitChip.unit, pPortXmiiSscGet->userPort, &(pPortXmiiSscGet->isEnable), &(pPortXmiiSscGet->sequenceModeOffset), &(pPortXmiiSscGet->phaseOffset));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_ssc_get(unitChip.unit, pPortXmiiSscGet->userPort, &(pPortXmiiSscGet->isEnable), &(pPortXmiiSscGet->sequenceModeOffset), &(pPortXmiiSscGet->phaseOffset));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_port_xmii_ssc_get(unitChip.unit, pPortXmiiSscGet->userPort, &(pPortXmiiSscGet->isEnable), &(pPortXmiiSscGet->sequenceModeOffset), &(pPortXmiiSscGet->phaseOffset));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_port_xmii_ssc_get(unitChip.unit, pPortXmiiSscGet->userPort, &(pPortXmiiSscGet->isEnable), &(pPortXmiiSscGet->sequenceModeOffset), &(pPortXmiiSscGet->phaseOffset));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Configure the xMII interface for a specified port in flashless over ethernet mode. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip Stacking unit and chip ID. \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortXmiiConfigSet Configuration parameters for the xMII interface. \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NULL_POINTER Null pointer error.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_port_xmii_config_set().
 */
RtkApiRet rtk_port_xmii_config_set(UnitChip_t unitChip, PortXmiiConfigSet_t *pPortXmiiConfigSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiConfigSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_config_set(unitChip.unit, pPortXmiiConfigSet->userPort, pPortXmiiConfigSet->portMode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set the force link status for the xMII interface on a specified port in flashless over ethernet mode. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip Stacking unit and chip ID. \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortXmiiForceLinkSet Configuration parameters for forcing link status. \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NULL_POINTER Null pointer error.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_port_xmii_force_link_set().
 */
RtkApiRet rtk_port_xmii_force_link_set(UnitChip_t unitChip, PortXmiiForceLinkSet_t *pPortXmiiForceLinkSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortXmiiForceLinkSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_xmii_force_link_set(unitChip.unit, pPortXmiiForceLinkSet->userPort, pPortXmiiForceLinkSet->portMode, pPortXmiiForceLinkSet->speed, pPortXmiiForceLinkSet->linkStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* INTF_XMII_CFG */

/**
 * @defgroup INTF_RMII_CFG RMII Interface Configuration
 * @brief Configure the RMII interface
 * @{
 */

/**
 * @brief Set RMII PHY mode and internal mode refer clock selection, user could set as phy mode and internal mode clock phase. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out]  *pPortRmiiRefclkSel Set port rmii reference clock sel set information. (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_rmii_refclk_sel_set(), dva_port_rmii_refclk_sel_set().
 */
RtkApiRet rtk_port_rmii_refclk_sel_set(UnitChip_t unitChip, PortRmiiRefclkSel_t *pPortRmiiRefclkSelSet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortRmiiRefclkSelSet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_rmii_refclk_sel_set(unitChip.unit, pPortRmiiRefclkSelSet->userPort, pPortRmiiRefclkSelSet->phyModePhase, pPortRmiiRefclkSelSet->internalPhase);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_rmii_refclk_sel_set(unitChip.unit, pPortRmiiRefclkSelSet->userPort, pPortRmiiRefclkSelSet->phyModePhase, pPortRmiiRefclkSelSet->internalPhase);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get RMII PHY mode and internal mode refer clock selection, user could get as phy mode and internal mode clock phase latch or negative latch. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1
 *
 * @param[in]  unitChip Stacking unit and chip ID. (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *pPortRmiiRefclkSel Get port rmii reference clock sel get information. (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK To execute this function correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_port_rmii_refclk_sel_get(), dva_port_rmii_refclk_sel_get().
 */
RtkApiRet rtk_port_rmii_refclk_sel_get(UnitChip_t unitChip, PortRmiiRefclkSel_t *pPortRmiiRefclkSelGet)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == pPortRmiiRefclkSelGet)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_port_rmii_refclk_sel_get(unitChip.unit, pPortRmiiRefclkSelGet->userPort, &(pPortRmiiRefclkSelGet->phyModePhase), &(pPortRmiiRefclkSelGet->internalPhase));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_port_rmii_refclk_sel_get(unitChip.unit, pPortRmiiRefclkSelGet->userPort, &(pPortRmiiRefclkSelGet->phyModePhase), &(pPortRmiiRefclkSelGet->internalPhase));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(1u == 0u);

    return ret;
}
/**@}*/ /* INTF_RMII_CFG */
/**@}*/ /* Interface */