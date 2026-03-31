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
#include "rtl907xd_va_api_cb.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_cb.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_cb.h"
#endif

#include "rtkas_api_cb.h"
/**
 * @defgroup RTK_MODULE_CB CB
 * @brief CB APIs.  This section details the functions used to configure various settings for CB, including:
 *                      - CB decode function
 *                      - CB global configuration
 *                      - CB function reset
 *                      - Passive stream identification function
 *                      - MISC function
 *                      - Individual recovery function
 *                      - Sequence recovery function
 *                      - Active stream identification function
 *                      - CB counters
 * @{
 */
/**
 * @defgroup CB_DECODE Decode function
 * @brief The APIs are used to configure the enable status of CB R-tag parser.
 * @{
 */
/**
 * @brief Set the R-tag parser enable state for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbParseRtagEnabledInfo CB R-tag parser enabled status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The parse R-tag status was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_parse_rtag_enabled_set(), cvb_cb_parse_rtag_enabled_set(), cva_cb_parse_rtag_enabled_set().
 */
RtkApiRet rtk_cb_parse_rtag_enabled_set(UnitChip_t unitChip, CbParseRtagEnabledInfo_t *cbParseRtagEnabledInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbParseRtagEnabledInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_parse_rtag_enabled_set(unitChip.unit, cbParseRtagEnabledInfo->port, cbParseRtagEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_parse_rtag_enabled_set(unitChip.unit, cbParseRtagEnabledInfo->port, cbParseRtagEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_parse_rtag_enabled_set(unitChip.unit, cbParseRtagEnabledInfo->port, cbParseRtagEnabledInfo->status);
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
 * @brief Get the R-tag parser enable state for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbParseRtagEnabledInfo CB R-tag parser enabled status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The parse R-tag status was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_parse_rtag_enabled_get(), cvb_cb_parse_rtag_enabled_get(), cva_cb_parse_rtag_enabled_get().
 */
RtkApiRet rtk_cb_parse_rtag_enabled_get(UnitChip_t unitChip, CbParseRtagEnabledInfo_t *cbParseRtagEnabledInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbParseRtagEnabledInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_parse_rtag_enabled_get(unitChip.unit, cbParseRtagEnabledInfo->port, &cbParseRtagEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_parse_rtag_enabled_get(unitChip.unit, cbParseRtagEnabledInfo->port, &cbParseRtagEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_parse_rtag_enabled_get(unitChip.unit, cbParseRtagEnabledInfo->port, &cbParseRtagEnabledInfo->status);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* CB_DECODE */
/**
 * @defgroup CB_GLB CB global configuration
 * @brief The APIs are used to configure the enable status of the CB or stream identification.
 * @{
 */
/**
 * @brief Set the enable status of CB function. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbEnabledInfo CB enabled status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The CB status was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_enabled_set(), cvb_cb_enabled_set(), cva_cb_enabled_set.
 */
RtkApiRet rtk_cb_enabled_set(UnitChip_t unitChip, CbEnabledInfo_t *cbEnabledInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbEnabledInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_enabled_set(unitChip.unit, cbEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_enabled_set(unitChip.unit, cbEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_enabled_set(unitChip.unit, cbEnabledInfo->status);
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
 * @brief Get the enable status of CB function. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] cbEnabledInfo CB enabled status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The CB status was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_enabled_get(), cvb_cb_enabled_get(), cva_cb_enabled_get().
 */
RtkApiRet rtk_cb_enabled_get(UnitChip_t unitChip, CbEnabledInfo_t *cbEnabledInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbEnabledInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_enabled_get(unitChip.unit, &cbEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_enabled_get(unitChip.unit, &cbEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_enabled_get(unitChip.unit, &cbEnabledInfo->status);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* CB_GLB */

/**
 * @defgroup CB_RESET CB function reset
 * @brief If necessary, the APIs are used to reset CB configuration to its default value before use.
 * @{
 */

/**
 * @brief Reset CB passive stream identification entry to default value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbIngressPassiveSidReset CB ingress passive stream identification reset information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The passive stream identification entry was reset
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_passive_sid_reset(), cvb_cb_ingress_passive_sid_reset(), cva_cb_ingress_passive_sid_reset().
 */
RtkApiRet rtk_cb_ingress_passive_sid_reset(UnitChip_t unitChip, CbIngressPassiveSidResetInfo_t *cbIngressPassiveSidReset)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressPassiveSidReset)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_passive_sid_reset(unitChip.unit, cbIngressPassiveSidReset->index);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_passive_sid_reset(unitChip.unit, cbIngressPassiveSidReset->index);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_passive_sid_reset(unitChip.unit, cbIngressPassiveSidReset->index);
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
 * @brief Reset CB individual recovery entry to default value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbIngressIndrcvReset CB ingress individual recovery reset information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The individual entry was reset
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_indrcv_reset(), cvb_cb_ingress_indrcv_reset(), cva_cb_ingress_indrcv_reset().
 */
RtkApiRet rtk_cb_ingress_indrcv_reset(UnitChip_t unitChip, CbIngressIndrcvResetInfo_t *cbIngressIndrcvReset)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressIndrcvReset)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_indrcv_reset(unitChip.unit, cbIngressIndrcvReset->index);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_indrcv_reset(unitChip.unit, cbIngressIndrcvReset->index);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_indrcv_reset(unitChip.unit, cbIngressIndrcvReset->index);
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
 * @brief Reset CB sequence recovery entry to default value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbEgressSeqrcvReset CB egress sequence recovery reset information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The sequence entry was reset
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_egress_seqrcv_reset(), cvb_cb_egress_seqrcv_reset(), cva_cb_egress_seqrcv_reset().
 */
RtkApiRet rtk_cb_egress_seqrcv_reset(UnitChip_t unitChip, CbEgressSeqrcvResetInfo_t *cbEgressSeqrcvReset)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbEgressSeqrcvReset)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_egress_seqrcv_reset(unitChip.unit, cbEgressSeqrcvReset->index);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_egress_seqrcv_reset(unitChip.unit, cbEgressSeqrcvReset->index);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_egress_seqrcv_reset(unitChip.unit, cbEgressSeqrcvReset->index);
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
 * @brief Reset CB Look up table entry to default value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbMiscReset CB misc reset information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The CB look up entry was reset
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_misc_reset(), cvb_cb_misc_reset(), cva_cb_misc_reset().
 */
RtkApiRet rtk_cb_misc_reset(UnitChip_t unitChip, CbMiscResetInfo_t *cbMiscReset)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbMiscReset)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_misc_reset(unitChip.unit, cbMiscReset->index);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_misc_reset(unitChip.unit, cbMiscReset->index);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_misc_reset(unitChip.unit, cbMiscReset->index);
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
 * @brief Reset CB function to default value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK CB all function was reset to default value
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_cb_reset(), cvb_cb_reset(), cva_cb_reset().
 */
RtkApiRet rtk_cb_reset(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_cb_reset(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_cb_reset(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_cb_reset(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**@}*/ /* CB_RESET */

/**
 * @defgroup PASSIVE_SID Passive stream identification function
 * @brief The APIs are used to configure stream identification match rules.
 * @{
 */
/**
 * @brief Set the enable status of passive stream identification. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip id (N/A) \n
 *             Stacking unit id is used for indicating which switch to be accessed.
 *             Chip id is used for chip model.
 * @param[in]  cbSidEnabledInfo CB stream identification enabled status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The passive stream identification status was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_sid_enabled_set(), cvb_cb_sid_enabled_set(), cva_cb_sid_enabled_set().
 */
RtkApiRet rtk_cb_sid_enabled_set(UnitChip_t unitChip, CbSidEnabledInfo_t *cbSidEnabledInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbSidEnabledInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_sid_enabled_set(unitChip.unit, cbSidEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_sid_enabled_set(unitChip.unit, cbSidEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_sid_enabled_set(unitChip.unit, cbSidEnabledInfo->status);
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
 * @brief Get the enable status of passive stream identification. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip id (N/A) \n
 *             Stacking unit id is used for indicating which switch to be accessed.
 *             Chip id is used for chip model.
 * @param[out] cbSidEnabledInfo CB stream identification enabled status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The passive stream identification status was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_sid_enabled_get(), cvb_cb_sid_enabled_get(), cva_cb_sid_enabled_get().
 */
RtkApiRet rtk_cb_sid_enabled_get(UnitChip_t unitChip, CbSidEnabledInfo_t *cbSidEnabledInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbSidEnabledInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_sid_enabled_get(unitChip.unit, &cbSidEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_sid_enabled_get(unitChip.unit, &cbSidEnabledInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_sid_enabled_get(unitChip.unit, &cbSidEnabledInfo->status);
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
 * @brief Set the passive stream identification entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbIngressPassiveSidInfo CB ingress passive stream identification entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The passive stream identification entry was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_passive_sid_set(), cvb_cb_ingress_passive_sid_set(), cva_cb_ingress_passive_sid_set().
 */
RtkApiRet rtk_cb_ingress_passive_sid_set(UnitChip_t unitChip, CbIngressPassiveSidInfo_t *cbIngressPassiveSidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressPassiveSidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_passive_sid_set(unitChip.unit, cbIngressPassiveSidInfo->index, &cbIngressPassiveSidInfo->sidentryParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_passive_sid_set(unitChip.unit, cbIngressPassiveSidInfo->index, &cbIngressPassiveSidInfo->sidentryParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_passive_sid_set(unitChip.unit, cbIngressPassiveSidInfo->index, &cbIngressPassiveSidInfo->sidentryParam);
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
 * @brief Get the passive stream identification entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbIngressPassiveSidInfo CB ingress passive stream identification entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The passive stream identification entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_passive_sid_get(), cvb_cb_ingress_passive_sid_get(), cva_cb_ingress_passive_sid_get().
 */
RtkApiRet rtk_cb_ingress_passive_sid_get(UnitChip_t unitChip, CbIngressPassiveSidInfo_t *cbIngressPassiveSidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressPassiveSidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_passive_sid_get(unitChip.unit, cbIngressPassiveSidInfo->index, &cbIngressPassiveSidInfo->sidentryParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_passive_sid_get(unitChip.unit, cbIngressPassiveSidInfo->index, &cbIngressPassiveSidInfo->sidentryParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_passive_sid_get(unitChip.unit, cbIngressPassiveSidInfo->index, &cbIngressPassiveSidInfo->sidentryParam);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* PASSIVE_SID */
/**
 * @defgroup MISC CB MISC function
 * @brief The APIs are used to configure MISC information, including the recovery function, sequence number generation function, \n
 *        stream splitting function, active stream identification function, and encode function.
 * @{
 */

/**
 * @brief Set the mode for splitting destination port mask. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] CbSplitDpmModeSet   Splitting DPM mode (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK            The mode for splitting destination port mask was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others               Please refer to dva_cb_splitdpm_mode_set()
 *
 */
RtkApiRet rtk_cb_splitdpm_mode_set(UnitChip_t unitChip, CbSplitDpmModeInfo_t *cbSplitDpmModeInfo)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == cbSplitDpmModeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_splitdpm_mode_set(unitChip.unit, cbSplitDpmModeInfo->splitDpmMode);
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
    }
    return ret;
}
/**
 * @brief Get the mode for splitting destination port mask. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] CbSplitDpmModeGet   Splitting DPM mode (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK            The mode for splitting destination port mask was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others               Please refer to dva_cb_splitdpm_mode_get()
 *
 */
RtkApiRet rtk_cb_splitdpm_mode_get(UnitChip_t unitChip, CbSplitDpmModeInfo_t *cbSplitDpmModeInfo)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == cbSplitDpmModeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_splitdpm_mode_get(unitChip.unit, &cbSplitDpmModeInfo->splitDpmMode);
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
    }
    return ret;
}
/**
 * @brief Set the CB MISC entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbMiscInfo CB misc entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The CB MISC table was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_misc_set(), cvb_cb_misc_set(), cva_cb_misc_set()
 */
RtkApiRet rtk_cb_misc_set(UnitChip_t unitChip, CbMiscInfo_t *cbMiscInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbMiscInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_misc_set(unitChip.unit, cbMiscInfo->index, &cbMiscInfo->lutTabParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_misc_set(unitChip.unit, cbMiscInfo->index, &cbMiscInfo->lutTabParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_misc_set(unitChip.unit, cbMiscInfo->index, &cbMiscInfo->lutTabParam);
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
 * @brief Get the CB MISC entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbMiscInfo CB misc entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK CB MISC entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_misc_get(), cvb_cb_misc_get(), cva_cb_misc_get()
 */
RtkApiRet rtk_cb_misc_get(UnitChip_t unitChip, CbMiscInfo_t *cbMiscInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbMiscInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_misc_get(unitChip.unit, cbMiscInfo->index, &cbMiscInfo->lutTabParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_misc_get(unitChip.unit, cbMiscInfo->index, &cbMiscInfo->lutTabParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_misc_get(unitChip.unit, cbMiscInfo->index, &cbMiscInfo->lutTabParam);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* MISC */

/**
 * @defgroup INDIVIDUAL_RECOVERY Individual recovery function
 * @brief The APIs are used to configure individual recovery function, which can drop duplicate packets for ingress single stream.
 * @{
 */
/**
 * @brief Set the CB individual recovery entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbIngressIndrcvInfo CB ingress individual recovery entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK CB individual recovery entry was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_indrcv_set(), cvb_cb_ingress_indrcv_set(), cva_cb_ingress_indrcv_set()
 */
RtkApiRet rtk_cb_ingress_indrcv_set(UnitChip_t unitChip, CbIngressIndrcvInfo_t *cbIngressIndrcvInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressIndrcvInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_indrcv_set(unitChip.unit, cbIngressIndrcvInfo->index, &cbIngressIndrcvInfo->indTabParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_indrcv_set(unitChip.unit, cbIngressIndrcvInfo->index, &cbIngressIndrcvInfo->indTabParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_indrcv_set(unitChip.unit, cbIngressIndrcvInfo->index, &cbIngressIndrcvInfo->indTabParam);
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
 * @brief Get the CB individual recovery entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbIngressIndrcvInfo CB ingress individual recovery entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK CB individual recovery entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_indrcv_get(), cvb_cb_ingress_indrcv_get(), cva_cb_ingress_indrcv_get()
 */
RtkApiRet rtk_cb_ingress_indrcv_get(UnitChip_t unitChip, CbIngressIndrcvInfo_t *cbIngressIndrcvInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressIndrcvInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_indrcv_get(unitChip.unit, cbIngressIndrcvInfo->index, &cbIngressIndrcvInfo->indTabParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_indrcv_get(unitChip.unit, cbIngressIndrcvInfo->index, &cbIngressIndrcvInfo->indTabParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_indrcv_get(unitChip.unit, cbIngressIndrcvInfo->index, &cbIngressIndrcvInfo->indTabParam);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* INDIVIDUAL_RECOVERY */
/**
 * @defgroup SEQUENCE_RECOVERY Sequence recovery function
 * @brief The APIs are used to configure sequence recovery function, which can drop duplicate packets for multi-stream.
 * @{
 */
/**
 * @brief Set the CB sequence recovery entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbEgressSeqrcvInfo CB egress sequence recovery entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK CB sequence recovery entry was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_egress_seqrcv_set(), cvb_cb_egress_seqrcv_set(), cva_cb_egress_seqrcv_set().
 */
RtkApiRet rtk_cb_egress_seqrcv_set(UnitChip_t unitChip, CbEgressSeqrcvInfo_t *cbEgressSeqrcvInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbEgressSeqrcvInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_egress_seqrcv_set(unitChip.unit, cbEgressSeqrcvInfo->index, &cbEgressSeqrcvInfo->seqTabParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_egress_seqrcv_set(unitChip.unit, cbEgressSeqrcvInfo->index, &cbEgressSeqrcvInfo->seqTabParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_egress_seqrcv_set(unitChip.unit, cbEgressSeqrcvInfo->index, &cbEgressSeqrcvInfo->seqTabParam);
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
 * @brief Get the CB sequence recovery entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] cbEgressSeqrcvInfo CB egress sequence recovery entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK CB sequence recovery entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_egress_seqrcv_get(), cvb_cb_egress_seqrcv_get(), cva_cb_egress_seqrcv_get().
 */
RtkApiRet rtk_cb_egress_seqrcv_get(UnitChip_t unitChip, CbEgressSeqrcvInfo_t *cbEgressSeqrcvInfo)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == cbEgressSeqrcvInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_egress_seqrcv_get(unitChip.unit, cbEgressSeqrcvInfo->index, &cbEgressSeqrcvInfo->seqTabParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_egress_seqrcv_get(unitChip.unit, cbEgressSeqrcvInfo->index, &cbEgressSeqrcvInfo->seqTabParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_egress_seqrcv_get(unitChip.unit, cbEgressSeqrcvInfo->index, &cbEgressSeqrcvInfo->seqTabParam);
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
 * @brief Set the value of latent error test timer. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbLatenTimerInfo CB laten timer information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The value of CB latent error test timer was set.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_laten_timer_set(), cvb_cb_laten_timer_set(), cva_cb_laten_timer_set().
 */
RtkApiRet rtk_cb_laten_timer_set(UnitChip_t unitChip, CbLatenTimerInfo_t *cbLatenTimerInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbLatenTimerInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_laten_timer_set(unitChip.unit, cbLatenTimerInfo->lattimvalue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_laten_timer_set(unitChip.unit, cbLatenTimerInfo->lattimvalue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_laten_timer_set(unitChip.unit, cbLatenTimerInfo->lattimvalue);
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
 * @brief Get the value of latent error test timer. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] cbLatenTimerInfo CB latentimer information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The value of CB latent error test timer was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_laten_timer_get(), cvb_cb_laten_timer_get(), cva_cb_laten_timer_get().
 */
RtkApiRet rtk_cb_laten_timer_get(UnitChip_t unitChip, CbLatenTimerInfo_t *cbLatenTimerInfo)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == cbLatenTimerInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_laten_timer_get(unitChip.unit, &cbLatenTimerInfo->lattimvalue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_laten_timer_get(unitChip.unit, &cbLatenTimerInfo->lattimvalue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_laten_timer_get(unitChip.unit, &cbLatenTimerInfo->lattimvalue);
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
 * @brief Set the value of latent error reset timer. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbLatenResetTimerInfo CB laten reset timer information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The value of CB latent error reset timer was set.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_laten_reset_timer_set(), cvb_cb_laten_reset_timer_set(), cva_cb_laten_reset_timer_set().
 */
RtkApiRet rtk_cb_laten_reset_timer_set(UnitChip_t unitChip, CbLatenResetTimerInfo_t *cbLatenResetTimerInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbLatenResetTimerInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_laten_reset_timer_set(unitChip.unit, cbLatenResetTimerInfo->latrsttimvalue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_laten_reset_timer_set(unitChip.unit, cbLatenResetTimerInfo->latrsttimvalue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_laten_reset_timer_set(unitChip.unit, cbLatenResetTimerInfo->latrsttimvalue);
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
 * @brief Get the value of latent error reset timer. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] cbLatenResetTimerInfo CB laten reset timer information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The value of CB latent error reset timer was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_laten_reset_timer_get(), cvb_cb_laten_reset_timer_get(), cva_cb_laten_reset_timer_get().
 */
RtkApiRet rtk_cb_laten_reset_timer_get(UnitChip_t unitChip, CbLatenResetTimerInfo_t *cbLatenResetTimerInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbLatenResetTimerInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_laten_reset_timer_get(unitChip.unit, &cbLatenResetTimerInfo->latrsttimvalue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_laten_reset_timer_get(unitChip.unit, &cbLatenResetTimerInfo->latrsttimvalue);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_laten_reset_timer_get(unitChip.unit, &cbLatenResetTimerInfo->latrsttimvalue);
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
 * @brief Get the CB global interrupt enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] cbInterruptEnableInfo CB interrupt enable status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK CB latent error interrupt enable status was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_interrupt_enable_get(), cvb_cb_interrupt_enable_get(), cva_cb_interrupt_enable_get().
 */
RtkApiRet rtk_cb_interrupt_enable_get(UnitChip_t unitChip, CbInterruptEnableInfo_t *cbInterruptEnableInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbInterruptEnableInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_interrupt_enable_get(unitChip.unit, &cbInterruptEnableInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_interrupt_enable_get(unitChip.unit, &cbInterruptEnableInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_interrupt_enable_get(unitChip.unit, &cbInterruptEnableInfo->status);
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
 * @brief Set the CB global interrupt enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbInterruptEnableInfo CB interrupt enable status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The CB latent error reset timer was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_interrupt_enable_set(), cvb_cb_interrupt_enable_set(), cva_cb_interrupt_enable_set().
 */
RtkApiRet rtk_cb_interrupt_enable_set(UnitChip_t unitChip, CbInterruptEnableInfo_t *cbInterruptEnableInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbInterruptEnableInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_interrupt_enable_set(unitChip.unit, cbInterruptEnableInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_interrupt_enable_set(unitChip.unit, cbInterruptEnableInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_interrupt_enable_set(unitChip.unit, cbInterruptEnableInfo->status);
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
 * @brief Get the CB global interrupt flag. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] cbInterruptFlagInfo CB interrupt flag information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Global interrupt flag of CB latent error was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_interrupt_flag_get(), cvb_cb_interrupt_flag_get(), cva_cb_interrupt_flag_get().
 */
RtkApiRet rtk_cb_interrupt_flag_get(UnitChip_t unitChip, CbInterruptFlagInfo_t *cbInterruptFlagInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbInterruptFlagInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_interrupt_flag_get(unitChip.unit, &cbInterruptFlagInfo->flag);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_interrupt_flag_get(unitChip.unit, &cbInterruptFlagInfo->flag);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_interrupt_flag_get(unitChip.unit, &cbInterruptFlagInfo->flag);
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
 * @brief Get 48-bits latent error mask for all entries. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] cbInterruptMaskInfo CB interrupt mask bits information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK CB 48-bits interrupt mask was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_interrupt_mask_get(), cvb_cb_interrupt_mask_get(), cva_cb_interrupt_mask_get().
 */
RtkApiRet rtk_cb_interrupt_mask_get(UnitChip_t unitChip, CbInterruptMaskInfo_t *cbInterruptMaskInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbInterruptMaskInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_interrupt_mask_get(unitChip.unit, &cbInterruptMaskInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_interrupt_mask_get(unitChip.unit, &cbInterruptMaskInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_interrupt_mask_get(unitChip.unit, &cbInterruptMaskInfo->status);
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
 * @brief Clear latent error state for an entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbInterruptStateClear CB interrupt state clear information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The latent error state of this sequence recovery entry was cleared up
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_interrupt_state_clear(), cvb_cb_interrupt_state_clear(), cva_cb_interrupt_state_clear().
 */
RtkApiRet rtk_cb_interrupt_state_clear(UnitChip_t unitChip, CbInterruptPerStateInfo_t *cbInterruptStateClear)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbInterruptStateClear)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_interrupt_state_clear(unitChip.unit, cbInterruptStateClear->index);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_interrupt_state_clear(unitChip.unit, cbInterruptStateClear->index);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_interrupt_state_clear(unitChip.unit, cbInterruptStateClear->index);
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
 * @brief Get latent error status for an entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbInterruptStateInfo CB interrupt state information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The latent error state of this sequence recovery entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_interrupt_state_get(), cvb_cb_interrupt_state_get(), cva_cb_interrupt_state_get().
 */
RtkApiRet rtk_cb_interrupt_state_get(UnitChip_t unitChip, CbInterruptPerStateInfo_t *cbInterruptStateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbInterruptStateInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_interrupt_state_get(unitChip.unit, cbInterruptStateInfo->index, &cbInterruptStateInfo->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_interrupt_state_get(unitChip.unit, cbInterruptStateInfo->index, &cbInterruptStateInfo->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_interrupt_state_get(unitChip.unit, cbInterruptStateInfo->index, &cbInterruptStateInfo->status);
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
 * @brief Get the CB sequence recovery history entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbEgressSeqhistoryInfo CB egress sequence recovery history entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK CB sequence recovery history entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_egress_seqhistory_get(), cvb_cb_egress_seqhistory_get(), cva_cb_egress_seqhistory_get().
 */
RtkApiRet rtk_cb_egress_seqhistory_get(UnitChip_t unitChip, CbEgressSeqhistoryInfo_t *cbEgressSeqhistoryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbEgressSeqhistoryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_egress_seqhistory_get(unitChip.unit, cbEgressSeqhistoryInfo->index, &cbEgressSeqhistoryInfo->seqHistoryEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_egress_seqhistory_get(unitChip.unit, cbEgressSeqhistoryInfo->index, &cbEgressSeqhistoryInfo->seqHistoryEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_egress_seqhistory_get(unitChip.unit, cbEgressSeqhistoryInfo->index, &cbEgressSeqhistoryInfo->seqHistoryEntry);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* SEQUENCE_RECOVERY */

/**
 * @defgroup ACTIVE_SID Active stream identification function
 * @brief The APIs are used to configure the replication header entry, outgoing interface list entry, and egress interface entry. \n
 *        After that, the packet content can be modified, including the destination MAC address, source MAC address, VLAN ID, and VLAN PCP field.
 * @{
 */
/**
 * @brief Set the replication header entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbActsidRhtblInfo Replication header entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK  Replication header entry was set.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_actsid_rhtbl_set(), cvb_cb_actsid_rhtbl_set(), cva_cb_actsid_rhtbl_set().
 */
RtkApiRet rtk_cb_actsid_rhtbl_set(UnitChip_t unitChip, CbActsidRhtblInfo_t *cbActsidRhtblInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbActsidRhtblInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_actsid_rhtbl_set(unitChip.unit, cbActsidRhtblInfo->index, &cbActsidRhtblInfo->rhEntryParm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_actsid_rhtbl_set(unitChip.unit, cbActsidRhtblInfo->index, &cbActsidRhtblInfo->rhEntryParm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_actsid_rhtbl_set(unitChip.unit, cbActsidRhtblInfo->index, &cbActsidRhtblInfo->rhEntryParm);
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
 * @brief Get the replication header entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbActsidRhtblInfo Replication header entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Replication header entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_actsid_rhtbl_get(), cvb_cb_actsid_rhtbl_get(), cva_cb_actsid_rhtbl_get().
 */
RtkApiRet rtk_cb_actsid_rhtbl_get(UnitChip_t unitChip, CbActsidRhtblInfo_t *cbActsidRhtblInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbActsidRhtblInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_actsid_rhtbl_get(unitChip.unit, cbActsidRhtblInfo->index, &cbActsidRhtblInfo->rhEntryParm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_actsid_rhtbl_get(unitChip.unit, cbActsidRhtblInfo->index, &cbActsidRhtblInfo->rhEntryParm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_actsid_rhtbl_get(unitChip.unit, cbActsidRhtblInfo->index, &cbActsidRhtblInfo->rhEntryParm);
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
 * @brief Set the outgoing interface list entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbActsidOiltblInfo Outgoing interface list entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Outgoing interface list entry was set.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_actsid_oiltbl_set(), cvb_cb_actsid_oiltbl_set(), cva_cb_actsid_oiltbl_set().
 */
RtkApiRet rtk_cb_actsid_oiltbl_set(UnitChip_t unitChip, CbActsidOiltblInfo_t *cbActsidOiltblInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbActsidOiltblInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_actsid_oiltbl_set(unitChip.unit, cbActsidOiltblInfo->index, &cbActsidOiltblInfo->oilEntryParm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_actsid_oiltbl_set(unitChip.unit, cbActsidOiltblInfo->index, &cbActsidOiltblInfo->oilEntryParm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_actsid_oiltbl_set(unitChip.unit, cbActsidOiltblInfo->index, &cbActsidOiltblInfo->oilEntryParm);
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
 * @brief Get the outgoing interface list entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbActsidOiltblInfo  Outgoing interface list entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Outgoing interface list entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_actsid_oiltbl_get(), cvb_cb_actsid_oiltbl_get(), cva_cb_actsid_oiltbl_get().
 */
RtkApiRet rtk_cb_actsid_oiltbl_get(UnitChip_t unitChip, CbActsidOiltblInfo_t *cbActsidOiltblInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbActsidOiltblInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_actsid_oiltbl_get(unitChip.unit, cbActsidOiltblInfo->index, &cbActsidOiltblInfo->oilEntryParm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_actsid_oiltbl_get(unitChip.unit, cbActsidOiltblInfo->index, &cbActsidOiltblInfo->oilEntryParm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_actsid_oiltbl_get(unitChip.unit, cbActsidOiltblInfo->index, &cbActsidOiltblInfo->oilEntryParm);
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
 * @brief Set the egress interface entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  cbActsidEgiftblInfo Egress interface entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK Egress interface entry was set
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_actsid_egiftbl_set(), cvb_cb_actsid_egiftbl_set(), cva_cb_actsid_egiftbl_set().
 */
RtkApiRet rtk_cb_actsid_egiftbl_set(UnitChip_t unitChip, CbActsidEgiftblInfo_t *cbActsidEgiftblInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbActsidEgiftblInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_actsid_egiftbl_set(unitChip.unit, cbActsidEgiftblInfo->index, &cbActsidEgiftblInfo->actegifParm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_actsid_egiftbl_set(unitChip.unit, cbActsidEgiftblInfo->index, &cbActsidEgiftblInfo->actegifParm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_actsid_egiftbl_set(unitChip.unit, cbActsidEgiftblInfo->index, &cbActsidEgiftblInfo->actegifParm);
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
 * @brief Get the egress interface entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbActsidEgiftblInfo Egress interface entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Egress interface entry was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_actsid_egiftbl_get(), cvb_cb_actsid_egiftbl_get(), cva_cb_actsid_egiftbl_get().
 */
RtkApiRet rtk_cb_actsid_egiftbl_get(UnitChip_t unitChip, CbActsidEgiftblInfo_t *cbActsidEgiftblInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbActsidEgiftblInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_actsid_egiftbl_get(unitChip.unit, cbActsidEgiftblInfo->index, &cbActsidEgiftblInfo->actegifParm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_actsid_egiftbl_get(unitChip.unit, cbActsidEgiftblInfo->index, &cbActsidEgiftblInfo->actegifParm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_actsid_egiftbl_get(unitChip.unit, cbActsidEgiftblInfo->index, &cbActsidEgiftblInfo->actegifParm);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* ACTIVE_SID */
/**
 * @defgroup CB_COUNTER CB counters
 * @brief The APIs are used to get CB counters, including per-entry stream identification match counter value, per-port stream identification match counter value, \n
 *        sequence recovery pass counter value, and sequence recovery drop counter value.
 * @{
 */
/**
 * @brief Get passive per-stream identification counter value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbIngressPerstreamIdcntInfo CB ingress per-stream identification counter information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Per-stream identification counter value was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_perstream_idcnt_get(), cvb_cb_ingress_perstream_idcnt_get(), cva_cb_ingress_perstream_idcnt_get().
 */
RtkApiRet rtk_cb_ingress_perstream_idcnt_get(UnitChip_t unitChip, CbIngressPerstreamIdcntInfo_t *cbIngressPerstreamIdcntInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressPerstreamIdcntInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_perstream_idcnt_get(unitChip.unit, cbIngressPerstreamIdcntInfo->index, &cbIngressPerstreamIdcntInfo->identityNum);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_perstream_idcnt_get(unitChip.unit, cbIngressPerstreamIdcntInfo->index, &cbIngressPerstreamIdcntInfo->identityNum);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_perstream_idcnt_get(unitChip.unit, cbIngressPerstreamIdcntInfo->index, &cbIngressPerstreamIdcntInfo->identityNum);
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
 * @brief Get passive per-port identification counter value. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbIngressPerportIdcntInfo CB ingress per-port identification counter information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK Per-port identification counter value was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_ingress_perport_idcnt_get(), cvb_cb_ingress_perport_idcnt_get(), cva_cb_ingress_perport_idcnt_get().
 */
RtkApiRet rtk_cb_ingress_perport_idcnt_get(UnitChip_t unitChip, CbIngressPerportIdcntInfo_t *cbIngressPerportIdcntInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbIngressPerportIdcntInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_ingress_perport_idcnt_get(unitChip.unit, cbIngressPerportIdcntInfo->port, &cbIngressPerportIdcntInfo->identityNum);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_ingress_perport_idcnt_get(unitChip.unit, cbIngressPerportIdcntInfo->port, &cbIngressPerportIdcntInfo->identityNum);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_ingress_perport_idcnt_get(unitChip.unit, cbIngressPerportIdcntInfo->port, &cbIngressPerportIdcntInfo->identityNum);
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
 * @brief Get the counter value for packets that have dropped by the sequence recovery entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbEgressSeqrcvDropcntInfo CB egress sequence recovery entry dropped counter information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The counter value of dropped packets was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_egress_seqrcv_dropcnt_get(), cvb_cb_egress_seqrcv_dropcnt_get(), cva_cb_egress_seqrcv_dropcnt_get().
 */
RtkApiRet rtk_cb_egress_seqrcv_dropcnt_get(UnitChip_t unitChip, CbEgressSeqrcvDropcntInfo_t *cbEgressSeqrcvDropcntInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbEgressSeqrcvDropcntInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_egress_seqrcv_dropcnt_get(unitChip.unit, cbEgressSeqrcvDropcntInfo->index, &cbEgressSeqrcvDropcntInfo->dropcnt);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_egress_seqrcv_dropcnt_get(unitChip.unit, cbEgressSeqrcvDropcntInfo->index, &cbEgressSeqrcvDropcntInfo->dropcnt);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_egress_seqrcv_dropcnt_get(unitChip.unit, cbEgressSeqrcvDropcntInfo->index, &cbEgressSeqrcvDropcntInfo->dropcnt);
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
 * @brief Get the counter value for packets that have passed through the sequence recovery entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] cbEgressSeqrcvPasscntInfo CB egress sequence recovery entry passed counter information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @retval RT_ERR_OK The counter value of passed packets was gotten.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_cb_egress_seqrcv_passcnt_get(), cvb_cb_egress_seqrcv_passcnt_get(), cva_cb_egress_seqrcv_passcnt_get().
 */
RtkApiRet rtk_cb_egress_seqrcv_passcnt_get(UnitChip_t unitChip, CbEgressSeqrcvPasscntInfo_t *cbEgressSeqrcvPasscntInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == cbEgressSeqrcvPasscntInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_cb_egress_seqrcv_passcnt_get(unitChip.unit, cbEgressSeqrcvPasscntInfo->index, &cbEgressSeqrcvPasscntInfo->passcnt);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_cb_egress_seqrcv_passcnt_get(unitChip.unit, cbEgressSeqrcvPasscntInfo->index, &cbEgressSeqrcvPasscntInfo->passcnt);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_cb_egress_seqrcv_passcnt_get(unitChip.unit, cbEgressSeqrcvPasscntInfo->index, &cbEgressSeqrcvPasscntInfo->passcnt);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* CB_COUNTER */
/**@}*/ /* CB */
