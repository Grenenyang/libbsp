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
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_someip.h"
#endif

#include "rtkas_api_someip.h"

/**
 * @defgroup RTK_MODULE_SOMEIP SOME/IP Firewall
 * @brief SOME/IP firewall APIs
 * @{
 */

/**
 * @defgroup SOMEIP_SET_L4DPORT Set SOME/IP firewall L4 destination port
 * @brief Set SOME/IP firewall L4 destination port
 * @{
 */

/**
 * @brief This API sets an L4 destination port for the SOME/IP firewall.
 *        Supported chips: RTL_907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used for indicating which switch is to be accessed.
 *                                  Chip ID is used for the chip model.
 * @param[in] someipL4Dport         SOME/IP firewall L4 destination port information (N/A) \n
 *                                  Parameters required to configure the SOME/IP firewall L4 destination port.
 * @param[out] None
 * @retval RT_ERR_OK                The settings of the SOME/IP firewall L4 destination port were configured successfully. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others                   Please refer to dva_someip_set_l4dport()
 */
RtkApiRet rtk_someip_set_l4dport(UnitChip_t unitChip, someipL4Dport_t *someipL4Dport)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == someipL4Dport)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_someip_set_l4dport(unitChip.unit, someipL4Dport->idx, someipL4Dport->l4dport);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SOMEIP_SET_L4DPORT*/

/**
 * @defgroup SOMEIP_GET_L4DPORT Get SOME/IP firewall L4 destination port
 * @brief Get SOME/IP firewall L4 destination port
 * @{
 */

/**
 * @brief This API gets all L4 destination port configurations for the SOME/IP firewall.
 *        Supported chips: RTL_907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used for indicating which switch is to be accessed.
 *                                  Chip ID is used for the chip model.
 * @param[out] l4dport              SOME/IP firewall L4 destination port (N/A) \n
 *                                  All settings information for the SOME/IP firewall L4 destination port.
 * @retval RT_ERR_OK                The settings of the SOME/IP firewall L4 destination port were obtained successfully. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others                   Please refer to dva_someip_get_l4dport()
 */
RtkApiRet rtk_someip_get_l4dport(UnitChip_t unitChip, uint16 *l4dport)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == l4dport)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_someip_get_l4dport(unitChip.unit, l4dport);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SOMEIP_GET_L4DPORT*/

/**
 * @defgroup SOMEIP_SET_HW_WHITELIST Set SOME/IP firewall HW whitelist
 * @brief Set SOME/IP firewall HW whitelist
 * @{
 */

/**
 * @brief This API sets an HW whitelist for the SOME/IP firewall.
 *        Supported chips: RTL_907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used for indicating which switch is to be accessed.
 *                                  Chip ID is used for the chip model.
 * @param[in] someipHWWhitelist     SOME/IP firewall HW whitelist information (N/A) \n
 *                                  Parameters required to configure the SOME/IP firewall HW whitelist.
 * @param[out] None
 * @retval RT_ERR_OK                The settings of the SOME/IP firewall HW whitelist were configured successfully. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others                   Please refer to dva_someip_set_hw_whitelist()
 */
RtkApiRet rtk_someip_set_hw_whitelist(UnitChip_t unitChip, someipHWWhitelist_t *someipHWWhitelist)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == someipHWWhitelist)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_someip_set_hw_whitelist(unitChip.unit, someipHWWhitelist->idx, someipHWWhitelist->whitelist);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SOMEIP_SET_HW_WHITELIST*/

/**
 * @defgroup SOMEIP_GET_HW_WHITELIST Get SOME/IP firewall HW whitelist
 * @brief Get SOME/IP firewall HW whitelist
 * @{
 */

/**
 * @brief This API gets all SOME/IP firewall HW whitelist configurations.
 *        Supported chips: RTL_907XD_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used for indicating which switch is to be accessed.
 *                                  Chip ID is used for the chip model.
 * @param[out] whitelist            SOME/IP firewall HW whitelist (N/A) \n
 *                                  All setting information for the SOME/IP firewall HW whitelist.
 * @retval RT_ERR_OK                The settings of the SOME/IP firewall HW whitelist were obtained successfully. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others                   Please refer to dva_someip_get_hw_whitelist()
 */
RtkApiRet rtk_someip_get_hw_whitelist(UnitChip_t unitChip, uint32 *whitelist)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == whitelist)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_someip_get_hw_whitelist(unitChip.unit, whitelist);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*SOMEIP_GET_HW_WHITELIST*/

/**@}*/ /* SOMEIP */
