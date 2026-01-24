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
#include "rtkas_api_acl.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_acl.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_acl.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_acl.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_acl.h"
#endif

/**
 * @defgroup RTK_MODULE_ACL ACL
 * @brief ACL APIs. ACL (Access Control List) provide a comprehensive set of functions to manage and control network packet access permissions, including enabling/disabling ACL, template generation, packet inspection, and various range and member checks.
 * @{
 */
/**
 * @defgroup ACL_ENABLE ACL Enable & Lookup Miss Default Action
 * @brief "ACL Enable & Lookup Miss Default Action" allows each port to individually enable or disable ACL and decide whether to drop or continue processing unmatched packets.
 * @{
 */
/**
 * @brief Get the ingress ACL enable control
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]       unitChip Stacking unit and chip ID (N/A) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out]  aclEnableCtrlInfo Ingress ACL enable control information (N/A) \n
 *                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ingress ACL enable control successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_enable_ctrl_get(), dva_acl_enable_ctrl_get(), cvb_acl_enable_ctrl_get(), cva_acl_enable_ctrl_get().
 */
RtkApiRet rtk_acl_enable_ctrl_get(UnitChip_t unitChip, AclEnableCtrlInfo_t *aclEnableCtrlInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclEnableCtrlInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_enable_ctrl_get(unitChip.unit, aclEnableCtrlInfo->portNum, &(aclEnableCtrlInfo->aclEn), &(aclEnableCtrlInfo->defAct));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_enable_ctrl_get(unitChip.unit, aclEnableCtrlInfo->portNum, &(aclEnableCtrlInfo->aclEn), &(aclEnableCtrlInfo->defAct));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_enable_ctrl_get(unitChip.unit, aclEnableCtrlInfo->portNum, &(aclEnableCtrlInfo->aclEn), &(aclEnableCtrlInfo->defAct));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_enable_ctrl_get(unitChip.unit, aclEnableCtrlInfo->portNum, &(aclEnableCtrlInfo->aclEn), &(aclEnableCtrlInfo->defAct));
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
 * @brief Set the ingress ACL enable control
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclEnableCtrlInfo Ingress ACL enable control information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ingress ACL enable control successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_enable_ctrl_set(), dva_acl_enable_ctrl_set(), cvb_acl_enable_ctrl_set(), cva_acl_enable_ctrl_set().
 */
RtkApiRet rtk_acl_enable_ctrl_set(UnitChip_t unitChip, AclEnableCtrlInfo_t *aclEnableCtrlInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclEnableCtrlInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_enable_ctrl_set(unitChip.unit, aclEnableCtrlInfo->portNum, aclEnableCtrlInfo->aclEn, aclEnableCtrlInfo->defAct);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_enable_ctrl_set(unitChip.unit, aclEnableCtrlInfo->portNum, aclEnableCtrlInfo->aclEn, aclEnableCtrlInfo->defAct);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_enable_ctrl_set(unitChip.unit, aclEnableCtrlInfo->portNum, aclEnableCtrlInfo->aclEn, aclEnableCtrlInfo->defAct);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_enable_ctrl_set(unitChip.unit, aclEnableCtrlInfo->portNum, aclEnableCtrlInfo->aclEn, aclEnableCtrlInfo->defAct);
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
 * @brief Get the egress ACL enable control
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]       unitChip Stacking unit and chip ID (N/A) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out]  eaclEnableCtrlInfo Egress ACL enable control information (N/A) \n
 *                  Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the egress ACL enable control successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_enable_ctrl_get().
 */
RtkApiRet rtk_eacl_enable_ctrl_get(UnitChip_t unitChip, EaclEnableCtrlInfo_t *eaclEnableCtrlInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclEnableCtrlInfo)
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
                ret = dva_eacl_enable_ctrl_get(unitChip.unit, eaclEnableCtrlInfo->portNum, &(eaclEnableCtrlInfo->aclEn), &(eaclEnableCtrlInfo->defAct));
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
 * @brief Set the egress ACL enable control
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  eaclEnableCtrlInfo Egress ACL enable control information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the egress ACL enable control successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_enable_ctrl_set().
 */
RtkApiRet rtk_eacl_enable_ctrl_set(UnitChip_t unitChip, EaclEnableCtrlInfo_t *eaclEnableCtrlInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclEnableCtrlInfo)
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
                ret = dva_eacl_enable_ctrl_set(unitChip.unit, eaclEnableCtrlInfo->portNum, eaclEnableCtrlInfo->aclEn, eaclEnableCtrlInfo->defAct);
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

/**@}*/ /*ACL_ENABLE*/

/**
 * @defgroup TEMPLATE_GENERATOR Template Generator
 * @brief The user can define which combination of packet fields need to be filtered.
 * @{
 */
/**
 * @brief Get the ingress ACL template generator configuration
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclTemplateInfo Ingress ACL template getting information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ingress ACL template successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_template_getting(), dva_acl_template_getting(), cvb_acl_template_getting(), cva_acl_template_getting().
 */
RtkApiRet rtk_acl_template_getting(UnitChip_t unitChip, AclTemplateInfo_t *aclTemplateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTemplateInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_template_getting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, &(aclTemplateInfo->type));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_template_getting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, &(aclTemplateInfo->type));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_template_getting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, &(aclTemplateInfo->type));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_template_getting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, &(aclTemplateInfo->type));
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
 * @brief Set the ingress ACL template generator configuration
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclTemplateInfo Ingress ACL template setting information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ingress ACL template successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_template_setting(), dva_acl_template_setting(), cvb_acl_template_setting(), cva_acl_template_setting().
 */
RtkApiRet rtk_acl_template_setting(UnitChip_t unitChip, AclTemplateInfo_t *aclTemplateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTemplateInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_template_setting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, aclTemplateInfo->type);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_template_setting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, aclTemplateInfo->type);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_template_setting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, aclTemplateInfo->type);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_template_setting(unitChip.unit, aclTemplateInfo->tpIdx, aclTemplateInfo->fieldIdx, aclTemplateInfo->type);
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
 * @brief Get the egress ACL template generator configuration
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] eaclTemplateInfo Egress ACL template getting information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the egress ACL template successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_template_getting().
 */
RtkApiRet rtk_eacl_template_getting(UnitChip_t unitChip, EaclTemplateInfo_t *eaclTemplateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclTemplateInfo)
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
                ret = dva_eacl_template_getting(unitChip.unit, eaclTemplateInfo->tpIdx, eaclTemplateInfo->fieldIdx, &(eaclTemplateInfo->type));
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
 * @brief Set the egress ACL template generator configuration
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  eaclTemplateInfo Egress ACL template setting information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the egress ACL template successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_template_setting().
 */
RtkApiRet rtk_eacl_template_setting(UnitChip_t unitChip, EaclTemplateInfo_t *eaclTemplateInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclTemplateInfo)
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
                ret = dva_eacl_template_setting(unitChip.unit, eaclTemplateInfo->tpIdx, eaclTemplateInfo->fieldIdx, eaclTemplateInfo->type);
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

/**@}*/ /*TEMPLATE_GENERATOR*/

/**
 * @defgroup PACKET_INSPECTION_ENGINE Packet Inspection Engine
 * @brief PIE (Packet Inspection Engine) is a module that inspects network packets against ACL rules and outputs hit or miss results.
 * @{
 */
/**
 * @brief Get the ACL PIE entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclPieEntryInfo ACL getting PIE entry information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL PIE entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_getting_pie_entry(), dva_acl_getting_pie_entry(), cvb_acl_getting_pie_entry(), cva_acl_getting_pie_entry().
 */
RtkApiRet rtk_acl_getting_pie_entry(UnitChip_t unitChip, AclPieEntryInfo_t *aclPieEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPieEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_getting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_getting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_getting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_getting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
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
 * @brief Set the ACL PIE entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclPieEntryInfo ACL setting PIE entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL PIE entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_setting_pie_entry(), dva_acl_setting_pie_entry(), cvb_acl_setting_pie_entry(), cva_acl_setting_pie_entry().
 */
RtkApiRet rtk_acl_setting_pie_entry(UnitChip_t unitChip, AclPieEntryInfo_t *aclPieEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPieEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_setting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_setting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_setting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_setting_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
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
 * @brief Add the ACL PIE entry at runtime
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclPieEntryInfo ACL setting PIE entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Add the ACL PIE entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_acl_runtime_add_pie_entry()
 * @note Please note that aclRuleIndex in AclPieEntryInfo_t must not be duplicated. \n
 *       When adding entries in increments of 128, data preprocessing will occur, necessitating a brief waiting period. This is a normal part of the process, so please rest assured that packet transmission will continue without interruption. \n
 *       After adding all entries, please execute rtk_acl_runtime_trigger_pie_entries to load all entries into the switch. \n
 */
RtkApiRet rtk_acl_runtime_add_pie_entry(UnitChip_t unitChip, AclPieEntryInfo_t *aclPieEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPieEntryInfo)
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
                ret = dva_acl_runtime_add_pie_entry(unitChip.unit, aclPieEntryInfo->aclRuleIndex, aclPieEntryInfo->pPieParam);
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
 * @brief Trigger PIE entries at runtime
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Trigger PIE entries successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_acl_runtime_trigger_pie_entries()
 * @note Upon initial execution of this function, data preprocessing occurs, requiring a brief wait. Packet transmission remains uninterrupted, so please rest assured.
 *       Following this, the Switch's transmission will briefly pause for no more than 2ms. Once completed, the ACL's PIE will be activated, and packet transmission will continue.
 */
RtkApiRet rtk_acl_runtime_trigger_pie_entries(UnitChip_t unitChip)
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
            ret = dva_acl_runtime_trigger_pie_entries(unitChip.unit);
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
    return ret;
}

/**
 * @brief Get the ACL PIE priority value
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclPriorityPieInfo ACL priority PIE get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL PIE priority value successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_priority_pie_get(), dva_acl_priority_pie_get(), cvb_acl_priority_pie_get(), cva_acl_priority_pie_get().
 */
RtkApiRet rtk_acl_priority_pie_get(UnitChip_t unitChip, AclPriorityPieInfo_t *aclPriorityPieInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPriorityPieInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_priority_pie_get(unitChip.unit, aclPriorityPieInfo->pieBlock, &(aclPriorityPieInfo->priVal));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_priority_pie_get(unitChip.unit, aclPriorityPieInfo->pieBlock, &(aclPriorityPieInfo->priVal));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_priority_pie_get(unitChip.unit, aclPriorityPieInfo->pieBlock, &(aclPriorityPieInfo->priVal));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_priority_pie_get(unitChip.unit, aclPriorityPieInfo->pieBlock, &(aclPriorityPieInfo->priVal));
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
 * @brief Set the ACL PIE priority value
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclPriorityPieInfo ACL priority PIE set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL PIE priority value successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_priority_pie_set(), dva_acl_priority_pie_set(), cvb_acl_priority_pie_set(), cva_acl_priority_pie_set().
 */
RtkApiRet rtk_acl_priority_pie_set(UnitChip_t unitChip, AclPriorityPieInfo_t *aclPriorityPieInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPriorityPieInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_priority_pie_set(unitChip.unit, aclPriorityPieInfo->pieBlock, aclPriorityPieInfo->priVal);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_priority_pie_set(unitChip.unit, aclPriorityPieInfo->pieBlock, aclPriorityPieInfo->priVal);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_priority_pie_set(unitChip.unit, aclPriorityPieInfo->pieBlock, aclPriorityPieInfo->priVal);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_priority_pie_set(unitChip.unit, aclPriorityPieInfo->pieBlock, aclPriorityPieInfo->priVal);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*PACKET_INSPECTION_ENGINE*/

/**
 * @defgroup RESULT_REVERSE Result Reverse
 * @brief "Result Reverse" inverts the PIE output, swapping "match" results with "mismatch" results and vice versa.
 * @{
 */
/**
 * @brief Get the ACL result reverse value
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclReverseInfo ACL reverse getting information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL reverse value successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_reverse_getting(), dva_acl_reverse_getting(), cvb_acl_reverse_getting(), cva_acl_reverse_getting().
 */
RtkApiRet rtk_acl_reverse_getting(UnitChip_t unitChip, AclReverseInfo_t *aclReverseInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclReverseInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_reverse_getting(unitChip.unit, aclReverseInfo->aclRuleIndex, &(aclReverseInfo->isReverse));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_reverse_getting(unitChip.unit, aclReverseInfo->aclRuleIndex, &(aclReverseInfo->isReverse));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_reverse_getting(unitChip.unit, aclReverseInfo->aclRuleIndex, &(aclReverseInfo->isReverse));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_reverse_getting(unitChip.unit, aclReverseInfo->aclRuleIndex, &(aclReverseInfo->isReverse));
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
 * @brief Set the ACL result reverse value
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclReverseInfo ACL reverse setting information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL reverse value successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_reverse_setting(), dva_acl_reverse_setting(), cvb_acl_reverse_setting(), cva_acl_reverse_setting().
 */
RtkApiRet rtk_acl_reverse_setting(UnitChip_t unitChip, AclReverseInfo_t *aclReverseInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclReverseInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_reverse_setting(unitChip.unit, aclReverseInfo->aclRuleIndex, aclReverseInfo->isReverse);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_reverse_setting(unitChip.unit, aclReverseInfo->aclRuleIndex, aclReverseInfo->isReverse);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_reverse_setting(unitChip.unit, aclReverseInfo->aclRuleIndex, aclReverseInfo->isReverse);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_reverse_setting(unitChip.unit, aclReverseInfo->aclRuleIndex, aclReverseInfo->isReverse);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*RESULT_REVERSE*/

/**
 * @defgroup RESULT_AGGREGATION Result Aggregation
 * @brief "Result Aggregation" merges two ACL entries to handle more filters simultaneously, increasing field count but reducing total entries.
 * @{
 */
/**
 * @brief Get the ACL result aggregation status
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclAggregatorInfo ACL aggregator get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL result aggregation status successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_aggregator_get(), dva_acl_aggregator_get(), cvb_acl_aggregator_get(), cva_acl_aggregator_get().
 */
RtkApiRet rtk_acl_aggregator_get(UnitChip_t unitChip, AclAggregatorInfo_t *aclAggregatorInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclAggregatorInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_aggregator_get(unitChip.unit, aclAggregatorInfo->aclRuleIndex, &(aclAggregatorInfo->aggOp));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_aggregator_get(unitChip.unit, aclAggregatorInfo->aclRuleIndex, &(aclAggregatorInfo->aggOp));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_aggregator_get(unitChip.unit, aclAggregatorInfo->aclRuleIndex, &(aclAggregatorInfo->aggOp));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_aggregator_get(unitChip.unit, aclAggregatorInfo->aclRuleIndex, &(aclAggregatorInfo->aggOp));
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
 * @brief Set the ACL result aggregation status
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclAggregatorInfo ACL aggregator set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL result aggregation status successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_aggregator_set(), dva_acl_aggregator_set(), cvb_acl_aggregator_set(), cva_acl_aggregator_set().
 */
RtkApiRet rtk_acl_aggregator_set(UnitChip_t unitChip, AclAggregatorInfo_t *aclAggregatorInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclAggregatorInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_aggregator_set(unitChip.unit, aclAggregatorInfo->aclRuleIndex, aclAggregatorInfo->aggOp);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_aggregator_set(unitChip.unit, aclAggregatorInfo->aclRuleIndex, aclAggregatorInfo->aggOp);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_aggregator_set(unitChip.unit, aclAggregatorInfo->aclRuleIndex, aclAggregatorInfo->aggOp);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_aggregator_set(unitChip.unit, aclAggregatorInfo->aclRuleIndex, aclAggregatorInfo->aggOp);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*RESULT_AGGREGATION*/

/**
 * @defgroup ACL_ACTION ACL Action
 * @brief "ACL Action" manage packet handling by redirecting, allowing or dropping packets, modifying VLAN tags, and adjusting priorities.
 * @{
 */
/**
 * @brief Get the ingress ACL action entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclActionEntryInfo Ingress ACL getting action entry information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ingress ACL action entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_getting_action_entry(), dva_acl_getting_action_entry(), cvb_acl_getting_action_entry(), cva_acl_getting_action_entry().
 */
RtkApiRet rtk_acl_getting_action_entry(UnitChip_t unitChip, AclActionEntryInfo_t *aclActionEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclActionEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_getting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_getting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_getting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_getting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
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
 * @brief Set the ingress ACL action entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclActionEntryInfo Ingress ACL setting action entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ingress ACL action entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_setting_action_entry(), dva_acl_setting_action_entry(), cvb_acl_setting_action_entry(), cva_acl_setting_action_entry().
 */
RtkApiRet rtk_acl_setting_action_entry(UnitChip_t unitChip, AclActionEntryInfo_t *aclActionEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclActionEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_setting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_setting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_setting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_setting_action_entry(unitChip.unit, aclActionEntryInfo->aclRuleIndex, aclActionEntryInfo->pActParam);
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
 * @brief Get the egress ACL action entry
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]    unitChip Stacking unit and chip ID (N/A) \n
 *               Stacking unit ID is used to indicate which switch is to be accessed.
 *               Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] eaclActionEntryInfo Egress ACL getting action entry information (N/A) \n
 *               Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the egress ACL action entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_getting_action_entry().
 */
RtkApiRet rtk_eacl_getting_action_entry(UnitChip_t unitChip, EaclActionEntryInfo_t *eaclActionEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclActionEntryInfo)
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
                ret = dva_eacl_getting_action_entry(unitChip.unit, eaclActionEntryInfo->aclRuleIndex, eaclActionEntryInfo->pEactParam);
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
 * @brief Set the egress ACL action entry
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  eaclActionEntryInfo Egress ACL setting action entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the egress action entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_setting_action_entry().
 */
RtkApiRet rtk_eacl_setting_action_entry(UnitChip_t unitChip, EaclActionEntryInfo_t *eaclActionEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclActionEntryInfo)
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
                ret = dva_eacl_setting_action_entry(unitChip.unit, eaclActionEntryInfo->aclRuleIndex, eaclActionEntryInfo->pEactParam);
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

/**@}*/ /*ACL_ACTION*/

/**
 * @defgroup ANY_OFFSET Any Offset / User Define Field (UDF)
 * @brief "User Defined Field" allows users to filter any 2-byte field within a packet by specifying base and offset values.
 * @{
 */
/**
 * @brief Get the ACL any offset configuration
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclOffsetInfo ACL offset get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL any offset configuration successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_offset_get(), dva_acl_offset_get(), cvb_acl_offset_get(), cva_acl_offset_get().
 */
RtkApiRet rtk_acl_offset_get(UnitChip_t unitChip, AclOffsetInfo_t *aclOffsetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclOffsetInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_offset_get(unitChip.unit, aclOffsetInfo->offsetIndex, aclOffsetInfo->portNum, &(aclOffsetInfo->offsetType), &(aclOffsetInfo->offsetPosition));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_offset_get(unitChip.unit, aclOffsetInfo->offsetIndex, aclOffsetInfo->portNum, &(aclOffsetInfo->offsetType), &(aclOffsetInfo->offsetPosition));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_offset_get(unitChip.unit, aclOffsetInfo->offsetIndex, &(aclOffsetInfo->offsetType), &(aclOffsetInfo->offsetPosition));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_offset_get(unitChip.unit, aclOffsetInfo->offsetIndex, &(aclOffsetInfo->offsetType), &(aclOffsetInfo->offsetPosition));
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
 * @brief Set the ACL any offset configuration
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclOffsetInfo ACL offset set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL any offset configuration successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_offset_set(), dva_acl_offset_set(), cvb_acl_offset_set(), cva_acl_offset_set().
 */
RtkApiRet rtk_acl_offset_set(UnitChip_t unitChip, AclOffsetInfo_t *aclOffsetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclOffsetInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_offset_set(unitChip.unit, aclOffsetInfo->offsetIndex, aclOffsetInfo->portNum, aclOffsetInfo->offsetType, aclOffsetInfo->offsetPosition);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_offset_set(unitChip.unit, aclOffsetInfo->offsetIndex, aclOffsetInfo->portNum, aclOffsetInfo->offsetType, aclOffsetInfo->offsetPosition);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_offset_set(unitChip.unit, aclOffsetInfo->offsetIndex, aclOffsetInfo->offsetType, aclOffsetInfo->offsetPosition);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_offset_set(unitChip.unit, aclOffsetInfo->offsetIndex, aclOffsetInfo->offsetType, aclOffsetInfo->offsetPosition);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*ANY_OFFSET*/

/**
 * @defgroup RANGE_CHECK Range Check
 * @brief "Range Check" allows defining a range of continuous values for various criteria such as Layer4 TCP/UDP port ranges, VLAN ID ranges, IPv4/IPv6 address ranges, and packet length ranges. This feature enables efficient filtering and matching of network packets.
 * @{
 */
/**
 * @brief Get the ACL Layer4 TCP/UDP port range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclL4PortRangeInfo ACL Layer4 port range get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL Layer4 port range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_l4_port_range_get(), dva_acl_l4_port_range_get(), cvb_acl_l4_port_range_get(), cva_acl_l4_port_range_get().
 */
RtkApiRet rtk_acl_l4_port_range_get(UnitChip_t unitChip, AclL4PortRangeInfo_t *aclL4PortRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclL4PortRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_l4_port_range_get(unitChip.unit, aclL4PortRangeInfo->entryIdx, &(aclL4PortRangeInfo->type), &(aclL4PortRangeInfo->upper), &(aclL4PortRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_l4_port_range_get(unitChip.unit, aclL4PortRangeInfo->entryIdx, &(aclL4PortRangeInfo->type), &(aclL4PortRangeInfo->upper), &(aclL4PortRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_l4_port_range_get(unitChip.unit, aclL4PortRangeInfo->entryIdx, &(aclL4PortRangeInfo->type), &(aclL4PortRangeInfo->upper), &(aclL4PortRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_l4_port_range_get(unitChip.unit, aclL4PortRangeInfo->entryIdx, &(aclL4PortRangeInfo->type), &(aclL4PortRangeInfo->upper), &(aclL4PortRangeInfo->lower));
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
 * @brief Set the ACL Layer4 TCP/UDP port range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclL4PortRangeInfo ACL Layer4 port range set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL Layer4 port range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_l4_port_range_set(), dva_acl_l4_port_range_set(), cvb_acl_l4_port_range_set(), cva_acl_l4_port_range_set().
 */
RtkApiRet rtk_acl_l4_port_range_set(UnitChip_t unitChip, AclL4PortRangeInfo_t *aclL4PortRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclL4PortRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_l4_port_range_set(unitChip.unit, aclL4PortRangeInfo->entryIdx, aclL4PortRangeInfo->type, aclL4PortRangeInfo->upper, aclL4PortRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_l4_port_range_set(unitChip.unit, aclL4PortRangeInfo->entryIdx, aclL4PortRangeInfo->type, aclL4PortRangeInfo->upper, aclL4PortRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_l4_port_range_set(unitChip.unit, aclL4PortRangeInfo->entryIdx, aclL4PortRangeInfo->type, aclL4PortRangeInfo->upper, aclL4PortRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_l4_port_range_set(unitChip.unit, aclL4PortRangeInfo->entryIdx, aclL4PortRangeInfo->type, aclL4PortRangeInfo->upper, aclL4PortRangeInfo->lower);
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
 * @brief Get the ACL VLAN ID range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclVidRangeInfo ACL VLAN ID range get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL VLAN ID range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_vid_range_get(), dva_acl_vid_range_get(), cvb_acl_vid_range_get(), cva_acl_vid_range_get().
 */
RtkApiRet rtk_acl_vid_range_get(UnitChip_t unitChip, AclVidRangeInfo_t *aclVidRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclVidRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_vid_range_get(unitChip.unit, aclVidRangeInfo->entryIdx, &(aclVidRangeInfo->type), &(aclVidRangeInfo->upper), &(aclVidRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_vid_range_get(unitChip.unit, aclVidRangeInfo->entryIdx, &(aclVidRangeInfo->type), &(aclVidRangeInfo->upper), &(aclVidRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_vid_range_get(unitChip.unit, aclVidRangeInfo->entryIdx, &(aclVidRangeInfo->type), &(aclVidRangeInfo->upper), &(aclVidRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_vid_range_get(unitChip.unit, aclVidRangeInfo->entryIdx, &(aclVidRangeInfo->type), &(aclVidRangeInfo->upper), &(aclVidRangeInfo->lower));
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
 * @brief Set the ACL VLAN ID range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclVidRangeInfo ACL VLAN ID range set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL VLAN ID range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_vid_range_set(), dva_acl_vid_range_set(), cvb_acl_vid_range_set(), cva_acl_vid_range_set().
 */
RtkApiRet rtk_acl_vid_range_set(UnitChip_t unitChip, AclVidRangeInfo_t *aclVidRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclVidRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_vid_range_set(unitChip.unit, aclVidRangeInfo->entryIdx, aclVidRangeInfo->type, aclVidRangeInfo->upper, aclVidRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_vid_range_set(unitChip.unit, aclVidRangeInfo->entryIdx, aclVidRangeInfo->type, aclVidRangeInfo->upper, aclVidRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_vid_range_set(unitChip.unit, aclVidRangeInfo->entryIdx, aclVidRangeInfo->type, aclVidRangeInfo->upper, aclVidRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_vid_range_set(unitChip.unit, aclVidRangeInfo->entryIdx, aclVidRangeInfo->type, aclVidRangeInfo->upper, aclVidRangeInfo->lower);
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
 * @brief Get the ACL IPv4 range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclIpRangeInfo ACL IPv4 range get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL IPv4 range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip_range_get(), dva_acl_ip_range_get(), cvb_acl_ip_range_get(), cva_acl_ip_range_get().
 */
RtkApiRet rtk_acl_ip_range_get(UnitChip_t unitChip, AclIpRangeInfo_t *aclIpRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIpRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip_range_get(unitChip.unit, aclIpRangeInfo->entryIdx, &(aclIpRangeInfo->type), &(aclIpRangeInfo->upper), &(aclIpRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip_range_get(unitChip.unit, aclIpRangeInfo->entryIdx, &(aclIpRangeInfo->type), &(aclIpRangeInfo->upper), &(aclIpRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip_range_get(unitChip.unit, aclIpRangeInfo->entryIdx, &(aclIpRangeInfo->type), &(aclIpRangeInfo->upper), &(aclIpRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip_range_get(unitChip.unit, aclIpRangeInfo->entryIdx, &(aclIpRangeInfo->type), &(aclIpRangeInfo->upper), &(aclIpRangeInfo->lower));
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
 * @brief Set the ACL IPv4 range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclIpRangeInfo ACL IPv4 range set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv4 range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip_range_set(), dva_acl_ip_range_set(), cvb_acl_ip_range_set(), cva_acl_ip_range_set().
 */
RtkApiRet rtk_acl_ip_range_set(UnitChip_t unitChip, AclIpRangeInfo_t *aclIpRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIpRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip_range_set(unitChip.unit, aclIpRangeInfo->entryIdx, aclIpRangeInfo->type, aclIpRangeInfo->upper, aclIpRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip_range_set(unitChip.unit, aclIpRangeInfo->entryIdx, aclIpRangeInfo->type, aclIpRangeInfo->upper, aclIpRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip_range_set(unitChip.unit, aclIpRangeInfo->entryIdx, aclIpRangeInfo->type, aclIpRangeInfo->upper, aclIpRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip_range_set(unitChip.unit, aclIpRangeInfo->entryIdx, aclIpRangeInfo->type, aclIpRangeInfo->upper, aclIpRangeInfo->lower);
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
 * @brief Get the ACL packet length range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclPktLenRangeInfo ACL packet length range get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL packet length range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_pkt_len_range_get(), dva_acl_pkt_len_range_get(), cvb_acl_pkt_len_range_get(), cva_acl_pkt_len_range_get().
 */
RtkApiRet rtk_acl_pkt_len_range_get(UnitChip_t unitChip, AclPktLenRangeInfo_t *aclPktLenRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPktLenRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_pkt_len_range_get(unitChip.unit, aclPktLenRangeInfo->entryIdx, &(aclPktLenRangeInfo->upper), &(aclPktLenRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_pkt_len_range_get(unitChip.unit, aclPktLenRangeInfo->entryIdx, &(aclPktLenRangeInfo->upper), &(aclPktLenRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_pkt_len_range_get(unitChip.unit, aclPktLenRangeInfo->entryIdx, &(aclPktLenRangeInfo->upper), &(aclPktLenRangeInfo->lower));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_pkt_len_range_get(unitChip.unit, aclPktLenRangeInfo->entryIdx, &(aclPktLenRangeInfo->upper), &(aclPktLenRangeInfo->lower));
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
 * @brief Set the ACL packet length range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclPktLenRangeInfo ACL packet length range set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL packet length range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_pkt_len_range_set(), dva_acl_pkt_len_range_set(), cvb_acl_pkt_len_range_set(), cva_acl_pkt_len_range_set().
 */
RtkApiRet rtk_acl_pkt_len_range_set(UnitChip_t unitChip, AclPktLenRangeInfo_t *aclPktLenRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPktLenRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_pkt_len_range_set(unitChip.unit, aclPktLenRangeInfo->entryIdx, aclPktLenRangeInfo->upper, aclPktLenRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_pkt_len_range_set(unitChip.unit, aclPktLenRangeInfo->entryIdx, aclPktLenRangeInfo->upper, aclPktLenRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_pkt_len_range_set(unitChip.unit, aclPktLenRangeInfo->entryIdx, aclPktLenRangeInfo->upper, aclPktLenRangeInfo->lower);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_pkt_len_range_set(unitChip.unit, aclPktLenRangeInfo->entryIdx, aclPktLenRangeInfo->upper, aclPktLenRangeInfo->lower);
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
 * @brief Get the ACL IPv6 range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclIp6RangeInfo ACL IPv6 range get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL IPv6 range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip6_range_get(), dva_acl_ip6_range_get(), cvb_acl_ip6_range_get(), cva_acl_ip6_range_get().
 */
RtkApiRet rtk_acl_ip6_range_get(UnitChip_t unitChip, AclIp6RangeInfo_t *aclIp6RangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIp6RangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip6_range_get(unitChip.unit, aclIp6RangeInfo->entryIdx, &(aclIp6RangeInfo->type), &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip6_range_get(unitChip.unit, aclIp6RangeInfo->entryIdx, &(aclIp6RangeInfo->type), &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip6_range_get(unitChip.unit, aclIp6RangeInfo->entryIdx, &(aclIp6RangeInfo->type), &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip6_range_get(unitChip.unit, aclIp6RangeInfo->entryIdx, &(aclIp6RangeInfo->type), &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
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
 * @brief Set the ACL IPv6 range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclIp6RangeInfo ACL IPv6 range set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv6 range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip6_range_set(), dva_acl_ip6_range_set(), cvb_acl_ip6_range_set(), cva_acl_ip6_range_set().
 */
RtkApiRet rtk_acl_ip6_range_set(UnitChip_t unitChip, AclIp6RangeInfo_t *aclIp6RangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIp6RangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip6_range_set(unitChip.unit, aclIp6RangeInfo->entryIdx, aclIp6RangeInfo->type, &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip6_range_set(unitChip.unit, aclIp6RangeInfo->entryIdx, aclIp6RangeInfo->type, &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip6_range_set(unitChip.unit, aclIp6RangeInfo->entryIdx, aclIp6RangeInfo->type, &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip6_range_set(unitChip.unit, aclIp6RangeInfo->entryIdx, aclIp6RangeInfo->type, &(aclIp6RangeInfo->upper)[0], &(aclIp6RangeInfo->lower)[0]);
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
 * @brief Get the ACL MAC address range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclMacRangeInfo ACL MAC range get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL MAC address range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_mac_range_get(), dva_acl_mac_range_get(), cvb_acl_mac_range_get(), cva_acl_mac_range_get().
 */
RtkApiRet rtk_acl_mac_range_get(UnitChip_t unitChip, AclMacRangeInfo_t *aclMacRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclMacRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_mac_range_get(unitChip.unit, aclMacRangeInfo->entryIdx, &(aclMacRangeInfo->type), &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_mac_range_get(unitChip.unit, aclMacRangeInfo->entryIdx, &(aclMacRangeInfo->type), &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_mac_range_get(unitChip.unit, aclMacRangeInfo->entryIdx, &(aclMacRangeInfo->type), &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_mac_range_get(unitChip.unit, aclMacRangeInfo->entryIdx, &(aclMacRangeInfo->type), &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
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
 * @brief Set the ACL MAC address range
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclMacRangeInfo ACL MAC range set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL MAC address range successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_mac_range_set(), dva_acl_mac_range_set(), cvb_acl_mac_range_set(), cva_acl_mac_range_set().
 */
RtkApiRet rtk_acl_mac_range_set(UnitChip_t unitChip, AclMacRangeInfo_t *aclMacRangeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclMacRangeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_mac_range_set(unitChip.unit, aclMacRangeInfo->entryIdx, aclMacRangeInfo->type, &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_mac_range_set(unitChip.unit, aclMacRangeInfo->entryIdx, aclMacRangeInfo->type, &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_mac_range_set(unitChip.unit, aclMacRangeInfo->entryIdx, aclMacRangeInfo->type, &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_mac_range_set(unitChip.unit, aclMacRangeInfo->entryIdx, aclMacRangeInfo->type, &(aclMacRangeInfo->upper)[0], &(aclMacRangeInfo->lower)[0]);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*RANGE_CHECK*/

/**
 * @defgroup GROUP_MEMBER_CHECK Group Member Check
 * @brief "Group Member Check" allows defining multiple discontinuous values for various criteria such as Layer4 TCP/UDP port members, VLAN ID members, IPv4/IPv6 members, and MAC address members. This feature enables efficient filtering and matching of network packets based on non-continuous value sets.
 * @{
 */
/**
 * @brief Get the ACL Layer4 TCP/UDP port member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclL4PortMemberInfo ACL Layer4 port member get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL Layer4 TCP/UDP port member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_l4_port_member_get(), dva_acl_l4_port_member_get(), cvb_acl_l4_port_member_get(), cva_acl_l4_port_member_get().
 */
RtkApiRet rtk_acl_l4_port_member_get(UnitChip_t unitChip, AclL4PortMemberInfo_t *aclL4PortMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclL4PortMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_l4_port_member_get(unitChip.unit, aclL4PortMemberInfo->entryIdx, &(aclL4PortMemberInfo->type), &(aclL4PortMemberInfo->member));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_l4_port_member_get(unitChip.unit, aclL4PortMemberInfo->entryIdx, &(aclL4PortMemberInfo->type), &(aclL4PortMemberInfo->member));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_l4_port_member_get(unitChip.unit, aclL4PortMemberInfo->entryIdx, &(aclL4PortMemberInfo->type), &(aclL4PortMemberInfo->member));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_l4_port_member_get(unitChip.unit, aclL4PortMemberInfo->entryIdx, &(aclL4PortMemberInfo->type), &(aclL4PortMemberInfo->member));
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
 * @brief Set the ACL Layer4 TCP/UDP port member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclL4PortMemberInfo ACL Layer4 port member set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL Layer4 TCP/UDP port member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_l4_port_member_set(), dva_acl_l4_port_member_set(), cvb_acl_l4_port_member_set(), cva_acl_l4_port_member_set().
 */
RtkApiRet rtk_acl_l4_port_member_set(UnitChip_t unitChip, AclL4PortMemberInfo_t *aclL4PortMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclL4PortMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_l4_port_member_set(unitChip.unit, aclL4PortMemberInfo->entryIdx, aclL4PortMemberInfo->type, aclL4PortMemberInfo->member);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_l4_port_member_set(unitChip.unit, aclL4PortMemberInfo->entryIdx, aclL4PortMemberInfo->type, aclL4PortMemberInfo->member);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_l4_port_member_set(unitChip.unit, aclL4PortMemberInfo->entryIdx, aclL4PortMemberInfo->type, aclL4PortMemberInfo->member);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_l4_port_member_set(unitChip.unit, aclL4PortMemberInfo->entryIdx, aclL4PortMemberInfo->type, aclL4PortMemberInfo->member);
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
 * @brief Get the ACL VLAN ID member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclVidMemberInfo ACL VLAN ID member get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL VLAN ID member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_vid_member_get(), dva_acl_vid_member_get(), cvb_acl_vid_member_get(), cva_acl_vid_member_get().
 */
RtkApiRet rtk_acl_vid_member_get(UnitChip_t unitChip, AclVidMemberInfo_t *aclVidMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclVidMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_vid_member_get(unitChip.unit, aclVidMemberInfo->entryIdx, &(aclVidMemberInfo->type), &(aclVidMemberInfo->member));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_vid_member_get(unitChip.unit, aclVidMemberInfo->entryIdx, &(aclVidMemberInfo->type), &(aclVidMemberInfo->member));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_vid_member_get(unitChip.unit, aclVidMemberInfo->entryIdx, &(aclVidMemberInfo->type), &(aclVidMemberInfo->member));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_vid_member_get(unitChip.unit, aclVidMemberInfo->entryIdx, &(aclVidMemberInfo->type), &(aclVidMemberInfo->member));
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
 * @brief Set the ACL VLAN ID member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclVidMemberInfo ACL VLAN ID member set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL VLAN ID member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_vid_member_set(), dva_acl_vid_member_set(), cvb_acl_vid_member_set(), cva_acl_vid_member_set().
 */
RtkApiRet rtk_acl_vid_member_set(UnitChip_t unitChip, AclVidMemberInfo_t *aclVidMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclVidMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_vid_member_set(unitChip.unit, aclVidMemberInfo->entryIdx, aclVidMemberInfo->type, aclVidMemberInfo->member);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_vid_member_set(unitChip.unit, aclVidMemberInfo->entryIdx, aclVidMemberInfo->type, aclVidMemberInfo->member);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_vid_member_set(unitChip.unit, aclVidMemberInfo->entryIdx, aclVidMemberInfo->type, aclVidMemberInfo->member);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_vid_member_set(unitChip.unit, aclVidMemberInfo->entryIdx, aclVidMemberInfo->type, aclVidMemberInfo->member);
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
 * @brief Get the ACL source port mask
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclSpmMemberInfo ACL source port mask member get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL source port mask successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_spm_member_get(), dva_acl_spm_member_get(), cvb_acl_spm_member_get(), cva_acl_spm_member_get().
 */
RtkApiRet rtk_acl_spm_member_get(UnitChip_t unitChip, AclSpmMemberInfo_t *aclSpmMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclSpmMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_spm_member_get(unitChip.unit, aclSpmMemberInfo->entryIdx, &(aclSpmMemberInfo->spm));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_spm_member_get(unitChip.unit, aclSpmMemberInfo->entryIdx, &(aclSpmMemberInfo->spm));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_spm_member_get(unitChip.unit, aclSpmMemberInfo->entryIdx, &(aclSpmMemberInfo->spm));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_spm_member_get(unitChip.unit, aclSpmMemberInfo->entryIdx, &(aclSpmMemberInfo->spm));
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
 * @brief Set the ACL source port mask
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclSpmMemberInfo ACL source port mask member set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL source port mask successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_spm_member_set(), dva_acl_spm_member_set(), cvb_acl_spm_member_set(), cva_acl_spm_member_set().
 */
RtkApiRet rtk_acl_spm_member_set(UnitChip_t unitChip, AclSpmMemberInfo_t *aclSpmMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclSpmMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_spm_member_set(unitChip.unit, aclSpmMemberInfo->entryIdx, aclSpmMemberInfo->spm);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_spm_member_set(unitChip.unit, aclSpmMemberInfo->entryIdx, aclSpmMemberInfo->spm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_spm_member_set(unitChip.unit, aclSpmMemberInfo->entryIdx, aclSpmMemberInfo->spm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_spm_member_set(unitChip.unit, aclSpmMemberInfo->entryIdx, aclSpmMemberInfo->spm);
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
 * @brief Get the ACL IPv4 member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclIpMemberInfo ACL IPv4 member get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL IPv4 member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip_member_get(), dva_acl_ip_member_get(), cvb_acl_ip_member_get(), cva_acl_ip_member_get().
 */
RtkApiRet rtk_acl_ip_member_get(UnitChip_t unitChip, AclIpMemberInfo_t *aclIpMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIpMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip_member_get(unitChip.unit, aclIpMemberInfo->entryIdx, &(aclIpMemberInfo->type), &(aclIpMemberInfo->member));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip_member_get(unitChip.unit, aclIpMemberInfo->entryIdx, &(aclIpMemberInfo->type), &(aclIpMemberInfo->member));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip_member_get(unitChip.unit, aclIpMemberInfo->entryIdx, &(aclIpMemberInfo->type), &(aclIpMemberInfo->member));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip_member_get(unitChip.unit, aclIpMemberInfo->entryIdx, &(aclIpMemberInfo->type), &(aclIpMemberInfo->member));
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
 * @brief Set the ACL IPv4 member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclIpMemberInfo ACL IPv4 member set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv4 member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip_member_set(), dva_acl_ip_member_set(), cvb_acl_ip_member_set(), cva_acl_ip_member_set().
 */
RtkApiRet rtk_acl_ip_member_set(UnitChip_t unitChip, AclIpMemberInfo_t *aclIpMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIpMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip_member_set(unitChip.unit, aclIpMemberInfo->entryIdx, aclIpMemberInfo->type, aclIpMemberInfo->member);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip_member_set(unitChip.unit, aclIpMemberInfo->entryIdx, aclIpMemberInfo->type, aclIpMemberInfo->member);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip_member_set(unitChip.unit, aclIpMemberInfo->entryIdx, aclIpMemberInfo->type, aclIpMemberInfo->member);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip_member_set(unitChip.unit, aclIpMemberInfo->entryIdx, aclIpMemberInfo->type, aclIpMemberInfo->member);
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
 * @brief Get the ACL IPv6 member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclIp6MemberInfo ACL IPv6 member get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL IPv6 member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip6_member_get(), dva_acl_ip6_member_get(), cvb_acl_ip6_member_get(), cva_acl_ip6_member_get().
 */
RtkApiRet rtk_acl_ip6_member_get(UnitChip_t unitChip, AclIp6MemberInfo_t *aclIp6MemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIp6MemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip6_member_get(unitChip.unit, aclIp6MemberInfo->entryIdx, &(aclIp6MemberInfo->type), &(aclIp6MemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip6_member_get(unitChip.unit, aclIp6MemberInfo->entryIdx, &(aclIp6MemberInfo->type), &(aclIp6MemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip6_member_get(unitChip.unit, aclIp6MemberInfo->entryIdx, &(aclIp6MemberInfo->type), &(aclIp6MemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip6_member_get(unitChip.unit, aclIp6MemberInfo->entryIdx, &(aclIp6MemberInfo->type), &(aclIp6MemberInfo->member)[0]);
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
 * @brief Set the ACL IPv6 member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclIp6MemberInfo ACL IPv6 member set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv6 member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_ip6_member_set(), dva_acl_ip6_member_set(), cvb_acl_ip6_member_set(), cva_acl_ip6_member_set().
 */
RtkApiRet rtk_acl_ip6_member_set(UnitChip_t unitChip, AclIp6MemberInfo_t *aclIp6MemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclIp6MemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_ip6_member_set(unitChip.unit, aclIp6MemberInfo->entryIdx, aclIp6MemberInfo->type, &(aclIp6MemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_ip6_member_set(unitChip.unit, aclIp6MemberInfo->entryIdx, aclIp6MemberInfo->type, &(aclIp6MemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_ip6_member_set(unitChip.unit, aclIp6MemberInfo->entryIdx, aclIp6MemberInfo->type, &(aclIp6MemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_ip6_member_set(unitChip.unit, aclIp6MemberInfo->entryIdx, aclIp6MemberInfo->type, &(aclIp6MemberInfo->member)[0]);
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
 * @brief Get the ACL MAC address member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclMacMemberInfo ACL MAC member get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL MAC address member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_mac_member_get(), dva_acl_mac_member_get(), cvb_acl_mac_member_get(), cva_acl_mac_member_get().
 */
RtkApiRet rtk_acl_mac_member_get(UnitChip_t unitChip, AclMacMemberInfo_t *aclMacMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclMacMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_mac_member_get(unitChip.unit, aclMacMemberInfo->entryIdx, &(aclMacMemberInfo->type), &(aclMacMemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_mac_member_get(unitChip.unit, aclMacMemberInfo->entryIdx, &(aclMacMemberInfo->type), &(aclMacMemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_mac_member_get(unitChip.unit, aclMacMemberInfo->entryIdx, &(aclMacMemberInfo->type), &(aclMacMemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_mac_member_get(unitChip.unit, aclMacMemberInfo->entryIdx, &(aclMacMemberInfo->type), &(aclMacMemberInfo->member)[0]);
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
 * @brief Set the ACL MAC address member
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclMacMemberInfo ACL MAC member set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL MAC address member successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_mac_member_set(), dva_acl_mac_member_set(), cvb_acl_mac_member_set(), cva_acl_mac_member_set().
 */
RtkApiRet rtk_acl_mac_member_set(UnitChip_t unitChip, AclMacMemberInfo_t *aclMacMemberInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclMacMemberInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_mac_member_set(unitChip.unit, aclMacMemberInfo->entryIdx, aclMacMemberInfo->type, &(aclMacMemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_mac_member_set(unitChip.unit, aclMacMemberInfo->entryIdx, aclMacMemberInfo->type, &(aclMacMemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_mac_member_set(unitChip.unit, aclMacMemberInfo->entryIdx, aclMacMemberInfo->type, &(aclMacMemberInfo->member)[0]);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_mac_member_set(unitChip.unit, aclMacMemberInfo->entryIdx, aclMacMemberInfo->type, &(aclMacMemberInfo->member)[0]);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*GROUP_MEMBER_CHECK*/

/**
 * @defgroup COUNTER_POLICER_ABNORMAL ACL Counter & Policer & Abnormal Times
 * @brief  "ACL Counter & Policer" provide functions to monitor and limit packet traffic for each ACL entry, and record the number or size of packets.\n
 *         "ACL Abnormal" times counter increases when the ACL policer rate limit is exceeded and triggers an interrupt to notify the CPU when the limit is reached.
 * @{
 */
/**
 * @brief Get the ACL lookup hit counter entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclCounterEntryInfo ACL rule hit counter get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL lookup hit counter entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_counter_entry_get(), dva_acl_counter_entry_get(), cvb_acl_counter_entry_get(), cva_acl_counter_entry_get().
 */
RtkApiRet rtk_acl_counter_entry_get(UnitChip_t unitChip, AclCounterEntryInfo_t *aclCounterEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclCounterEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_counter_entry_get(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_counter_entry_get(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_counter_entry_get(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_counter_entry_get(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
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
 * @brief Set the ACL lookup hit counter entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclCounterEntryInfo ACL rule hit counter set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL lookup hit counter entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_counter_entry_set(), dva_acl_counter_entry_set(), cvb_acl_counter_entry_set(), cva_acl_counter_entry_set().
 */
RtkApiRet rtk_acl_counter_entry_set(UnitChip_t unitChip, AclCounterEntryInfo_t *aclCounterEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclCounterEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_counter_entry_set(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_counter_entry_set(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_counter_entry_set(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_counter_entry_set(unitChip.unit, aclCounterEntryInfo->aclRuleIndex, aclCounterEntryInfo->pCounterParam);
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
 * @brief Get the ACL lookup hit policer entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclPolicerEntryInfo ACL rule hit policer get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL lookup hit policer entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_policer_entry_get(), dva_acl_policer_entry_get(), cvb_acl_policer_entry_get(), cva_acl_policer_entry_get().
 */
RtkApiRet rtk_acl_policer_entry_get(UnitChip_t unitChip, AclPolicerEntryInfo_t *aclPolicerEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPolicerEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_policer_entry_get(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_policer_entry_get(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_policer_entry_get(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_policer_entry_get(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
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
 * @brief Set the ACL lookup hit policer entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclPolicerEntryInfo ACL rule hit policer set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL lookup hit policer entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_policer_entry_set(), dva_acl_policer_entry_set(), cvb_acl_policer_entry_set(), cva_acl_policer_entry_set().
 */
RtkApiRet rtk_acl_policer_entry_set(UnitChip_t unitChip, AclPolicerEntryInfo_t *aclPolicerEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclPolicerEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_policer_entry_set(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_policer_entry_set(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_policer_entry_set(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_policer_entry_set(unitChip.unit, aclPolicerEntryInfo->aclRuleIndex, aclPolicerEntryInfo->pPolicerParam);
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
 * @brief Get the ACL abnormal times entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclAbnormalTimesEntryInfo ACL getting abnormal times entry information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL abnormal times entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_getting_abnormal_times_entry(), dva_acl_getting_abnormal_times_entry(), cvb_acl_getting_abnormal_times_entry(), cva_acl_getting_abnormal_times_entry().
 */
RtkApiRet rtk_acl_getting_abnormal_times_entry(UnitChip_t unitChip, AclAbnormalTimesEntryInfo_t *aclAbnormalTimesEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclAbnormalTimesEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_getting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_getting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_getting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_getting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
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
 * @brief Set the ACL abnormal times entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclAbnormalTimesEntryInfo ACL setting abnormal times entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL abnormal times entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_setting_abnormal_times_entry(), dva_acl_setting_abnormal_times_entry(), cvb_acl_setting_abnormal_times_entry(), cva_acl_setting_abnormal_times_entry().
 */
RtkApiRet rtk_acl_setting_abnormal_times_entry(UnitChip_t unitChip, AclAbnormalTimesEntryInfo_t *aclAbnormalTimesEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclAbnormalTimesEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_setting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_setting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_setting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_setting_abnormal_times_entry(unitChip.unit, aclAbnormalTimesEntryInfo->aclRuleIndex, aclAbnormalTimesEntryInfo->pAbnormalTimesParam);
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
 * @brief Get the enable status of the ingress ACL time slot function
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclTimeSlotEnInfo Ingress ACL time slot enable status get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the enable status of the ingress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_time_slot_en_get(), dva_acl_time_slot_en_get(), cvb_acl_time_slot_en_get(), cva_acl_time_slot_en_get().
 */
RtkApiRet rtk_acl_time_slot_en_get(UnitChip_t unitChip, AclTimeSlotEnInfo_t *aclTimeSlotEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTimeSlotEnInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_time_slot_en_get(unitChip.unit, &(aclTimeSlotEnInfo->enable));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_time_slot_en_get(unitChip.unit, &(aclTimeSlotEnInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_time_slot_en_get(unitChip.unit, &(aclTimeSlotEnInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_time_slot_en_get(unitChip.unit, &(aclTimeSlotEnInfo->enable));
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
 * @brief Enable/Disable the ingress ACL time slot function
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclTimeSlotEnInfo Ingress ACL time slot enable status set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Enable/disable the ingress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_time_slot_en_set(), dva_acl_time_slot_en_set(), cvb_acl_time_slot_en_set(), cva_acl_time_slot_en_set().
 */
RtkApiRet rtk_acl_time_slot_en_set(UnitChip_t unitChip, AclTimeSlotEnInfo_t *aclTimeSlotEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTimeSlotEnInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_time_slot_en_set(unitChip.unit, aclTimeSlotEnInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_time_slot_en_set(unitChip.unit, aclTimeSlotEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_time_slot_en_set(unitChip.unit, aclTimeSlotEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_time_slot_en_set(unitChip.unit, aclTimeSlotEnInfo->enable);
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
 * @brief Get the enable status of the egress ACL time slot function
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] eaclTimeSlotEnInfo Egress ACL time slot enable status get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the enable status of the egress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_time_slot_en_get().
 */
RtkApiRet rtk_eacl_time_slot_en_get(UnitChip_t unitChip, EaclTimeSlotEnInfo_t *eaclTimeSlotEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclTimeSlotEnInfo)
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
                ret = dva_eacl_time_slot_en_get(unitChip.unit, &(eaclTimeSlotEnInfo->enable));
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
 * @brief Enable/Disable the egress ACL time slot function
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  eaclTimeSlotEnInfo Egress ACL time slot enable status set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK enable/disable the egress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to rtk_eacl_time_slot_en_set().
 */
RtkApiRet rtk_eacl_time_slot_en_set(UnitChip_t unitChip, EaclTimeSlotEnInfo_t *eaclTimeSlotEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclTimeSlotEnInfo)
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
                ret = dva_eacl_time_slot_en_set(unitChip.unit, eaclTimeSlotEnInfo->enable);
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
 * @brief Get the interval of the ingress ACL time slot function
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclTimeSlotThresholdInfo Ingress ACL time slot threshold get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the interval of the ingress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_time_slot_threshold_get(), dva_acl_time_slot_threshold_get(), cvb_acl_time_slot_threshold_get(), cva_acl_time_slot_threshold_get().
 */
RtkApiRet rtk_acl_time_slot_threshold_get(UnitChip_t unitChip, AclTimeSlotThresholdInfo_t *aclTimeSlotThresholdInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTimeSlotThresholdInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_time_slot_threshold_get(unitChip.unit, &(aclTimeSlotThresholdInfo->threshold));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_time_slot_threshold_get(unitChip.unit, &(aclTimeSlotThresholdInfo->threshold));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_time_slot_threshold_get(unitChip.unit, &(aclTimeSlotThresholdInfo->threshold));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_time_slot_threshold_get(unitChip.unit, &(aclTimeSlotThresholdInfo->threshold));
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
 * @brief Set the interval of the ingress ACL time slot function
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclTimeSlotThresholdInfo Ingress ACL time slot threshold set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the interval of the ingress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_time_slot_threshold_set(), dva_acl_time_slot_threshold_set(), cvb_acl_time_slot_threshold_set(), cva_acl_time_slot_threshold_set().
 */
RtkApiRet rtk_acl_time_slot_threshold_set(UnitChip_t unitChip, AclTimeSlotThresholdInfo_t *aclTimeSlotThresholdInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTimeSlotThresholdInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_time_slot_threshold_set(unitChip.unit, aclTimeSlotThresholdInfo->threshold);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_time_slot_threshold_set(unitChip.unit, aclTimeSlotThresholdInfo->threshold);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_time_slot_threshold_set(unitChip.unit, aclTimeSlotThresholdInfo->threshold);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_time_slot_threshold_set(unitChip.unit, aclTimeSlotThresholdInfo->threshold);
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
 * @brief Get the interval of the egress ACL time slot function
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] eaclTimeSlotThresholdInfo Egress ACL time slot threshold get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the interval of the egress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_time_slot_threshold_get().
 */
RtkApiRet rtk_eacl_time_slot_threshold_get(UnitChip_t unitChip, EaclTimeSlotThresholdInfo_t *eaclTimeSlotThresholdInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclTimeSlotThresholdInfo)
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
                ret = dva_eacl_time_slot_threshold_get(unitChip.unit, &(eaclTimeSlotThresholdInfo->threshold));
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
 * @brief Set the interval of the egress ACL time slot function
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  eaclTimeSlotThresholdInfo Egress ACL time slot threshold set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the interval of the egress ACL time slot function successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_time_slot_threshold_set().
 */
RtkApiRet rtk_eacl_time_slot_threshold_set(UnitChip_t unitChip, EaclTimeSlotThresholdInfo_t *eaclTimeSlotThresholdInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclTimeSlotThresholdInfo)
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
                ret = dva_eacl_time_slot_threshold_set(unitChip.unit, eaclTimeSlotThresholdInfo->threshold);
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
 * @brief Get the ingress ACL abnormal destination port mask
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclAbnormalDpmInfo Ingress ACL abnormal destination port mask get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ingress ACL abnormal destination port mask successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_abnormal_dpm_get(), dva_acl_abnormal_dpm_get(), cvb_acl_abnormal_dpm_get(), cva_acl_abnormal_dpm_get().
 */
RtkApiRet rtk_acl_abnormal_dpm_get(UnitChip_t unitChip, AclAbnormalDpmInfo_t *aclAbnormalDpmInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclAbnormalDpmInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_abnormal_dpm_get(unitChip.unit, &(aclAbnormalDpmInfo->dpm));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_abnormal_dpm_get(unitChip.unit, &(aclAbnormalDpmInfo->dpm));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_abnormal_dpm_get(unitChip.unit, &(aclAbnormalDpmInfo->dpm));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_abnormal_dpm_get(unitChip.unit, &(aclAbnormalDpmInfo->dpm));
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
 * @brief Set the ingress ACL abnormal destination port mask
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclAbnormalDpmInfo Ingress ACL abnormal destination port mask set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ingress ACL abnormal destination port mask successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_abnormal_dpm_set(), dva_acl_abnormal_dpm_set(), cvb_acl_abnormal_dpm_set(), cva_acl_abnormal_dpm_set().
 */
RtkApiRet rtk_acl_abnormal_dpm_set(UnitChip_t unitChip, AclAbnormalDpmInfo_t *aclAbnormalDpmInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclAbnormalDpmInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_abnormal_dpm_set(unitChip.unit, aclAbnormalDpmInfo->dpm);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_abnormal_dpm_set(unitChip.unit, aclAbnormalDpmInfo->dpm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_abnormal_dpm_set(unitChip.unit, aclAbnormalDpmInfo->dpm);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_abnormal_dpm_set(unitChip.unit, aclAbnormalDpmInfo->dpm);
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
 * @brief Get the egress ACL abnormal destination port mask
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] eaclAbnormalDpmInfo Egress ACL abnormal destination port mask get information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the egress ACL abnormal destination port mask successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_abnormal_dpm_get().
 */
RtkApiRet rtk_eacl_abnormal_dpm_get(UnitChip_t unitChip, EaclAbnormalDpmInfo_t *eaclAbnormalDpmInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclAbnormalDpmInfo)
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
                ret = dva_eacl_abnormal_dpm_get(unitChip.unit, &(eaclAbnormalDpmInfo->dpm));
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
 * @brief Set the egress ACL abnormal destination port mask
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  eaclAbnormalDpmInfo Egress ACL abnormal destination port mask set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the egress ACL abnormal destination port mask successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eacl_abnormal_dpm_set().
 */
RtkApiRet rtk_eacl_abnormal_dpm_set(UnitChip_t unitChip, EaclAbnormalDpmInfo_t *eaclAbnormalDpmInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == eaclAbnormalDpmInfo)
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
                ret = dva_eacl_abnormal_dpm_set(unitChip.unit, eaclAbnormalDpmInfo->dpm);
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

/**@}*/ /*COUNTER_POLICER_ABNORMAL*/

/**
 * @defgroup ACL_TIMER_DISCARD ACL Timer Discard
 * @brief "ACL Timer Discard" drops packets that hit the ACL rule if a threshold is exceeded during the monitoring period, with configurable timings.
 * @{
 */
/**
 * @brief Get the ACL timer discard entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclTimerDiscardEntryInfo ACL getting timer discard entry information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL timer discard entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_getting_timer_discard_entry(), dva_acl_getting_timer_discard_entry(), cvb_acl_getting_timer_discard_entry(), cva_acl_getting_timer_discard_entry().
 */
RtkApiRet rtk_acl_getting_timer_discard_entry(UnitChip_t unitChip, AclTimerDiscardEntryInfo_t *aclTimerDiscardEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTimerDiscardEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_getting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_getting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_getting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_getting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
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
 * @brief Set the ACL timer discard entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  aclTimerDiscardEntryInfo ACL setting timer discard entry information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL timer discard entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_setting_timer_discard_entry(), dva_acl_setting_timer_discard_entry(), cvb_acl_setting_timer_discard_entry(), cva_acl_setting_timer_discard_entry().
 */
RtkApiRet rtk_acl_setting_timer_discard_entry(UnitChip_t unitChip, AclTimerDiscardEntryInfo_t *aclTimerDiscardEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclTimerDiscardEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_setting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_setting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_setting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_setting_timer_discard_entry(unitChip.unit, aclTimerDiscardEntryInfo->aclRuleIndex, aclTimerDiscardEntryInfo->pTimerDiscardParam);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*ACL_TIMER_DISCARD*/

/**
 * @defgroup ACL_HEADER_DUMP ACL Header Dump
 * @brief "ACL Header Dump" supports dumping the contents of packets that hit the ACL rules
 * @{
 */
/**
 * @brief Get the ACL header dump entry
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]      unitChip Stacking unit and chip ID (N/A) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in, out] aclHeaderDumpEntryInfo ACL getting header dump entry information (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK Get the ACL header dump entry successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_acl_getting_header_dump_entry(), dva_acl_getting_header_dump_entry(), cvb_acl_getting_header_dump_entry(), cva_acl_getting_header_dump_entry().
 */
RtkApiRet rtk_acl_getting_header_dump_entry(UnitChip_t unitChip, AclHeaderDumpEntryInfo_t *aclHeaderDumpEntryInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == aclHeaderDumpEntryInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_acl_getting_header_dump_entry(unitChip.unit, aclHeaderDumpEntryInfo->aclHeaderDumpIndex, aclHeaderDumpEntryInfo->pHeaderDumpParam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_acl_getting_header_dump_entry(unitChip.unit, aclHeaderDumpEntryInfo->aclHeaderDumpIndex, aclHeaderDumpEntryInfo->pHeaderDumpParam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_acl_getting_header_dump_entry(unitChip.unit, aclHeaderDumpEntryInfo->aclHeaderDumpIndex, aclHeaderDumpEntryInfo->pHeaderDumpParam);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_acl_getting_header_dump_entry(unitChip.unit, aclHeaderDumpEntryInfo->aclHeaderDumpIndex, aclHeaderDumpEntryInfo->pHeaderDumpParam);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /*ACL_HEADER_DUMP*/

/**@}*/ /*ACL*/
