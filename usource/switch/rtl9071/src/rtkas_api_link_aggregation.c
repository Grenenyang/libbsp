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
#include "rtl907xd_v1_api_link_aggregation.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_link_aggregation.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_link_aggregation.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_link_aggregation.h"
#endif
#include "rtkas_api_link_aggregation.h"

/**
 * @defgroup RTK_MODULE_LINK_AGGREGATION Link Aggregation
 * @brief Link Aggregation APIs. Link Aggregation is used to combine several physical ports on a switch into one logical port to provide greater bandwidth.
 * @{
 */
/**
 * @defgroup LAG_GROUP Port Grouping
 * @brief Aggregate the member ports into a group.
 * @{
 */
/**
 * @brief Set group members of link aggregation. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lagMemberSet Link aggregation member set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set group members of link aggregation successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_member_set(), dva_lag_member_set(), cvb_lag_member_set(), cva_lag_member_set().
 */
RtkApiRet rtk_lag_member_set(UnitChip_t unitChip, LagMemberConfig_t *lagMemberSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagMemberSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_member_set(unitChip.unit, lagMemberSet->group, lagMemberSet->portMask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_member_set(unitChip.unit, lagMemberSet->group, lagMemberSet->portMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_member_set(unitChip.unit, lagMemberSet->group, lagMemberSet->portMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_member_set(unitChip.unit, lagMemberSet->group, lagMemberSet->portMask);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get group members of link aggregation. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lagMemberGet Link aggregation member get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @retval RT_ERR_OK Get group members of link aggregation successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_member_get(), dva_lag_member_get(), cvb_lag_member_get(), cva_lag_member_get().
 */
RtkApiRet rtk_lag_member_get(UnitChip_t unitChip, LagMemberConfig_t *lagMemberGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagMemberGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_member_get(unitChip.unit, lagMemberGet->group, &lagMemberGet->portMask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_member_get(unitChip.unit, lagMemberGet->group, &lagMemberGet->portMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_member_get(unitChip.unit, lagMemberGet->group, &lagMemberGet->portMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_member_get(unitChip.unit, lagMemberGet->group, &lagMemberGet->portMask);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* LAG_GROUP */

/**
 * @defgroup LAG_HASH Hash Mode
 * @brief Use the packet's hash value calculated based on hash mask and hash shift as a rule for frame distribution.
 * @{
 */
/**
 * @brief Set link aggregation group to hash mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lagHashSet Link aggregation hash set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set link aggregation group to hash mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_hash_set(), dva_lag_hash_set(), cvb_lag_hash_set(), cva_lag_hash_set().
 */
RtkApiRet rtk_lag_hash_set(UnitChip_t unitChip, LagHashConfig_t *lagHashSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagHashSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_hash_set(unitChip.unit, lagHashSet->group, lagHashSet->hashMask, lagHashSet->hashShift);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_hash_set(unitChip.unit, lagHashSet->group, lagHashSet->hashMask, lagHashSet->hashShift);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_hash_set(unitChip.unit, lagHashSet->group, lagHashSet->hashMask, lagHashSet->hashShift);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_hash_set(unitChip.unit, lagHashSet->group, lagHashSet->hashMask, lagHashSet->hashShift);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get hash mode configuration of link aggregation group. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lagHashGet Link aggregation hash get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @retval RT_ERR_OK Get hash mode configuration of link aggregation group successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_hash_get(), dva_lag_hash_get(), cvb_lag_hash_get(), cva_lag_hash_get().
 */
RtkApiRet rtk_lag_hash_get(UnitChip_t unitChip, LagHashConfig_t *lagHashGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagHashGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_hash_get(unitChip.unit, lagHashGet->group, &lagHashGet->hashMask, &lagHashGet->hashShift);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_hash_get(unitChip.unit, lagHashGet->group, &lagHashGet->hashMask, &lagHashGet->hashShift);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_hash_get(unitChip.unit, lagHashGet->group, &lagHashGet->hashMask, &lagHashGet->hashShift);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_hash_get(unitChip.unit, lagHashGet->group, &lagHashGet->hashMask, &lagHashGet->hashShift);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* LAG_HASH */

/**
 * @defgroup LAG_BALANCE Balance Mode
 * @brief Specific packets are transmitted by all member ports of the group in a round-robin manner for frame distribution.
 * @{
 */
/**
 * @brief Set link aggregation group to balance mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lagBalanceSet Link aggregation balance set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set link aggregation group to balance mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_set(), dva_lag_balance_set(), cvb_lag_balance_set(), cva_lag_balance_set().
 */
RtkApiRet rtk_lag_balance_set(UnitChip_t unitChip, LagBalanceConfig_t *lagBalanceSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_set(unitChip.unit, lagBalanceSet->group, lagBalanceSet->balanceMode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_set(unitChip.unit, lagBalanceSet->group, lagBalanceSet->balanceMode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_set(unitChip.unit, lagBalanceSet->group, lagBalanceSet->balanceMode);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_set(unitChip.unit, lagBalanceSet->group, lagBalanceSet->balanceMode);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get balance mode status of link aggregation group. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lagBalanceGet Link aggregation balance get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @retval RT_ERR_OK Get balance mode status of link aggregation group successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_get(), dva_lag_balance_get(), cvb_lag_balance_get(), cva_lag_balance_get().
 */
RtkApiRet rtk_lag_balance_get(UnitChip_t unitChip, LagBalanceConfig_t *lagBalanceGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_get(unitChip.unit, lagBalanceGet->group, &lagBalanceGet->balanceMode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_get(unitChip.unit, lagBalanceGet->group, &lagBalanceGet->balanceMode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_get(unitChip.unit, lagBalanceGet->group, &lagBalanceGet->balanceMode);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_get(unitChip.unit, lagBalanceGet->group, &lagBalanceGet->balanceMode);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Set ether type value of ether type balance mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lagBalanceEthertypeSet Link aggregation balance ether type set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set ether type value of ether type balance mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_ethertype_set(), dva_lag_balance_ethertype_set(), cvb_lag_balance_ethertype_set(), cva_lag_balance_ethertype_set().
 */
RtkApiRet rtk_lag_balance_ethertype_set(UnitChip_t unitChip, LagBalanceEthertypeConfig_t *lagBalanceEthertypeSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceEthertypeSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_ethertype_set(unitChip.unit, lagBalanceEthertypeSet->group, lagBalanceEthertypeSet->etherType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_ethertype_set(unitChip.unit, lagBalanceEthertypeSet->group, lagBalanceEthertypeSet->etherType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_ethertype_set(unitChip.unit, lagBalanceEthertypeSet->group, lagBalanceEthertypeSet->etherType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_ethertype_set(unitChip.unit, lagBalanceEthertypeSet->group, lagBalanceEthertypeSet->etherType);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get ether type value of ether type balance mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lagBalanceEthertypeGet Link aggregation balance ether type get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @retval RT_ERR_OK Get ether type value of ether type balance mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_ethertype_get(), dva_lag_balance_ethertype_get(), cvb_lag_balance_ethertype_get(), cva_lag_balance_ethertype_get().
 */
RtkApiRet rtk_lag_balance_ethertype_get(UnitChip_t unitChip, LagBalanceEthertypeConfig_t *lagBalanceEthertypeGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceEthertypeGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_ethertype_get(unitChip.unit, lagBalanceEthertypeGet->group, &lagBalanceEthertypeGet->etherType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_ethertype_get(unitChip.unit, lagBalanceEthertypeGet->group, &lagBalanceEthertypeGet->etherType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_ethertype_get(unitChip.unit, lagBalanceEthertypeGet->group, &lagBalanceEthertypeGet->etherType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_ethertype_get(unitChip.unit, lagBalanceEthertypeGet->group, &lagBalanceEthertypeGet->etherType);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Set protocol value of protocol balance mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lagBalanceProtocolSet Link aggregation balance protocol set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set protocol value of protocol balance mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_protocol_set(), dva_lag_balance_protocol_set(), cvb_lag_balance_protocol_set(), cva_lag_balance_protocol_set().
 */
RtkApiRet rtk_lag_balance_protocol_set(UnitChip_t unitChip, LagBalanceProtocolConfig_t *lagBalanceProtocolSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceProtocolSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_protocol_set(unitChip.unit, lagBalanceProtocolSet->group, lagBalanceProtocolSet->protocol);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_protocol_set(unitChip.unit, lagBalanceProtocolSet->group, lagBalanceProtocolSet->protocol);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_protocol_set(unitChip.unit, lagBalanceProtocolSet->group, lagBalanceProtocolSet->protocol);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_protocol_set(unitChip.unit, lagBalanceProtocolSet->group, lagBalanceProtocolSet->protocol);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get protocol value of protocol balance mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lagBalanceProtocolGet Link aggregation balance protocol get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @retval RT_ERR_OK Get protocol value of protocol balance mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_protocol_get(), dva_lag_balance_protocol_get(), cvb_lag_balance_protocol_get(), cva_lag_balance_protocol_get().
 */
RtkApiRet rtk_lag_balance_protocol_get(UnitChip_t unitChip, LagBalanceProtocolConfig_t *lagBalanceProtocolGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceProtocolGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_protocol_get(unitChip.unit, lagBalanceProtocolGet->group, &lagBalanceProtocolGet->protocol);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_protocol_get(unitChip.unit, lagBalanceProtocolGet->group, &lagBalanceProtocolGet->protocol);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_protocol_get(unitChip.unit, lagBalanceProtocolGet->group, &lagBalanceProtocolGet->protocol);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_protocol_get(unitChip.unit, lagBalanceProtocolGet->group, &lagBalanceProtocolGet->protocol);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Set L4 destination port value/mask of L4 destination port balance mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lagBalanceL4DportSet Link aggregation balance L4 destination port set information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Set L4 destination port value/mask of L4 destination port balance mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_l4dport_set(), dva_lag_balance_l4dport_set(), cvb_lag_balance_l4dport_set(), cva_lag_balance_l4dport_set().
 */
RtkApiRet rtk_lag_balance_l4dport_set(UnitChip_t unitChip, LagBalanceL4DportConfig_t *lagBalanceL4DportSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceL4DportSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_l4dport_set(unitChip.unit, lagBalanceL4DportSet->group, lagBalanceL4DportSet->l4Dport, lagBalanceL4DportSet->l4DportMask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_l4dport_set(unitChip.unit, lagBalanceL4DportSet->group, lagBalanceL4DportSet->l4Dport, lagBalanceL4DportSet->l4DportMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_l4dport_set(unitChip.unit, lagBalanceL4DportSet->group, lagBalanceL4DportSet->l4Dport, lagBalanceL4DportSet->l4DportMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_l4dport_set(unitChip.unit, lagBalanceL4DportSet->group, lagBalanceL4DportSet->l4Dport, lagBalanceL4DportSet->l4DportMask);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get L4 destination port value of L4 destination port balance mode. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lagBalanceL4DportGet Link aggregation balance L4 destination port get information (N/A) \n
 *             Please refer to the struct field introductions in the rtkas_api_link_aggregation.h files for detailed content.
 * @retval RT_ERR_OK Get L4 destination port value of L4 destination port balance mode successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER Null pointer for parameter.
 * @retval Others Please refer to dv1_lag_balance_l4dport_get(), dva_lag_balance_l4dport_get(), cvb_lag_balance_l4dport_get(), cva_lag_balance_l4dport_get().
 */
RtkApiRet rtk_lag_balance_l4dport_get(UnitChip_t unitChip, LagBalanceL4DportConfig_t *lagBalanceL4DportGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lagBalanceL4DportGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lag_balance_l4dport_get(unitChip.unit, lagBalanceL4DportGet->group, &lagBalanceL4DportGet->l4Dport, &lagBalanceL4DportGet->l4DportMask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lag_balance_l4dport_get(unitChip.unit, lagBalanceL4DportGet->group, &lagBalanceL4DportGet->l4Dport, &lagBalanceL4DportGet->l4DportMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lag_balance_l4dport_get(unitChip.unit, lagBalanceL4DportGet->group, &lagBalanceL4DportGet->l4Dport, &lagBalanceL4DportGet->l4DportMask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lag_balance_l4dport_get(unitChip.unit, lagBalanceL4DportGet->group, &lagBalanceL4DportGet->l4Dport, &lagBalanceL4DportGet->l4DportMask);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* LAG_BALANCE */

/**@}*/