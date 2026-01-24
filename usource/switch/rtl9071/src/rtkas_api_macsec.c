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

#include "rtkas_api_macsec.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_macsec.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_macsec.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_macsec.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_macsec.h"
#endif

/**
 * @defgroup RTK_MODULE_MACSEC MACsec
 * @brief MACsec APIs. This component is used for control MACsec (Media Access Control Security) hardware.
 * @{
 */
/**
 * @defgroup MACSEC_MATCH_RULE MACsec Match Rule
 * @brief Configure the match rule table of the MACsec hardware module
 * @{
 */
/**
 * @brief This API is used to get basic parameters of a specific match rule entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] matchRuleParams   MACsec get match rule params information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved the match rule entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleParams" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_match_rule_params(UnitChip_t unitChip, MacsecMatchRuleParams_t *matchRuleParams)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleParams)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get VLAN related parameters of a specific match rule entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] matchRuleParams   MACsec get match rule VLAN params information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved the match rule entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleParams" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_match_rule_vlan_params(UnitChip_t unitChip, MacsecMatchRuleParams_t *matchRuleParams)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleParams)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get miscellaneous parameters of a specific match rule entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] matchRuleParams   MACsec get match rule miscellaneous params information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved the match rule entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleParams" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_match_rule_misc_params(UnitChip_t unitChip, MacsecMatchRuleParams_t *matchRuleParams)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleParams)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set basic parameters of a specific match rule entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  matchRuleParams      MACsec set match rule params information.(N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the match rule entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleParams" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_match_rule_params(UnitChip_t unitChip, const MacsecMatchRuleParams_t *matchRuleParams)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleParams)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_match_rule_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set VLAN related parameters of a specific match rule entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  matchRuleParams      MACsec set match rule VLAN params information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the match rule entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleParams" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_match_rule_vlan_params(UnitChip_t unitChip, const MacsecMatchRuleParams_t *matchRuleParams)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleParams)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_match_rule_vlan_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set miscellaneous parameters of a specific match rule entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  matchRuleParams      MACsec set match rule miscellaneous params information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the match rule entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleParams" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_match_rule_misc_params(UnitChip_t unitChip, const MacsecMatchRuleParams_t *matchRuleParams)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleParams)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_match_rule_misc_params(unitChip.unit, matchRuleParams->port, matchRuleParams->direction, matchRuleParams->entryIdx, &matchRuleParams->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get the status of a specific MACsec flow.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] matchRuleStatus   MACsec get match rule status information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved the flow control entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleStatus" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_match_rule_status(UnitChip_t unitChip, MacsecMatchRuleStatus_t *matchRuleStatus)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleStatus)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, &matchRuleStatus->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, &matchRuleStatus->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, &matchRuleStatus->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, &matchRuleStatus->enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to enable or disable a specific MACsec flow.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  matchRuleStatus      MACsec set match rule status information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the flow control entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "matchRuleStatus" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_match_rule_status(UnitChip_t unitChip, const MacsecMatchRuleStatus_t *matchRuleStatus)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != matchRuleStatus)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, matchRuleStatus->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, matchRuleStatus->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, matchRuleStatus->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_match_rule_status(unitChip.unit, matchRuleStatus->port, matchRuleStatus->direction, matchRuleStatus->entryIdx, matchRuleStatus->enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**@}*/  // MACSEC_MATCH_RULE

/**
 * @defgroup MACSEC_FLOW_CTRL MACsec Flow Control
 * @brief Configure the flow control table of the MACsec hardware module
 * @{
 */
/**
 * @brief This API is used to get a flow control entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] flowControlEntry  MACsec get flow control entry information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved the flow control entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "flowControlEntry" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_flow_control_entry(UnitChip_t unitChip, MacsecFlowControlEntry_t *flowControlEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != flowControlEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set a flow control entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  flowControlEntry     MACsec set flow control entry information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the flow control entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "flowControlEntry" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_flow_control_entry(UnitChip_t unitChip, const MacsecFlowControlEntry_t *flowControlEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != flowControlEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_flow_control_entry(unitChip.unit, flowControlEntry->port, flowControlEntry->direction, flowControlEntry->entryIdx, &flowControlEntry->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**@}*/  // MACSEC_FLOW_CTRL

/**
 * @defgroup MACSEC_SA MACsec SA(Secruity Association)
 * @brief Configure the security asociation table of the MACsec hardware module
 * @{
 */
/**
 * @brief This API is used to get a security association (SA) entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] saEntry           MACsec get SA entry information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved the SA entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "saEntry" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_sa_entry(UnitChip_t unitChip, MacsecSaEntry_t *saEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != saEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set a security association (SA) entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  saEntry              MACsec set SA entry information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the SA entry.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "saEntry" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_sa_entry(UnitChip_t unitChip, const MacsecSaEntry_t *saEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != saEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_sa_entry(unitChip.unit, saEntry->port, saEntry->direction, saEntry->entryIdx, saEntry->cipherSuite, &saEntry->params);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**@}*/  // MACSEC_SA

/**
 * @defgroup MACSEC_COUNTER MACsec Counters
 * @brief Fetch/clear the statistic counters of the MACsec hardware module
 * @{
 */
/**
 * @brief This API is used to get all global counters of MACsec egress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] txGlobalCounter   MACsec get tx global counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "txGlobalCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_tx_global_counter(UnitChip_t unitChip, MacsecTxGlobalCounter_t *txGlobalCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != txGlobalCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_tx_global_counter(unitChip.unit, txGlobalCounter->port, &txGlobalCounter->counters);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_tx_global_counter(unitChip.unit, txGlobalCounter->port, &txGlobalCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_tx_global_counter(unitChip.unit, txGlobalCounter->port, &txGlobalCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_tx_global_counter(unitChip.unit, txGlobalCounter->port, &txGlobalCounter->counters);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to clear all global counters of MACsec egress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  txGlobalCounter      MACsec clear tx global counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "txGlobalCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_clear_tx_global_counter(UnitChip_t unitChip, const MacsecClearTxGlobalCounter_t *txGlobalCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != txGlobalCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_clear_tx_global_counter(unitChip.unit, txGlobalCounter->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_clear_tx_global_counter(unitChip.unit, txGlobalCounter->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_clear_tx_global_counter(unitChip.unit, txGlobalCounter->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_clear_tx_global_counter(unitChip.unit, txGlobalCounter->port);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get all global counters of MACsec ingress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] rxGlobalCounter   MACsec get rx global counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "rxGlobalCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_rx_global_counter(UnitChip_t unitChip, MacsecRxGlobalCounter_t *rxGlobalCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != rxGlobalCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_rx_global_counter(unitChip.unit, rxGlobalCounter->port, &rxGlobalCounter->counters);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_rx_global_counter(unitChip.unit, rxGlobalCounter->port, &rxGlobalCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_rx_global_counter(unitChip.unit, rxGlobalCounter->port, &rxGlobalCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_rx_global_counter(unitChip.unit, rxGlobalCounter->port, &rxGlobalCounter->counters);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to clear all global counters of MACsec ingress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  rxGlobalCounter      MACsec clear rx global counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "rxGlobalCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_clear_rx_global_counter(UnitChip_t unitChip, const MacsecClearRxGlobalCounter_t *rxGlobalCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != rxGlobalCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_clear_rx_global_counter(unitChip.unit, rxGlobalCounter->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_clear_rx_global_counter(unitChip.unit, rxGlobalCounter->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_clear_rx_global_counter(unitChip.unit, rxGlobalCounter->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_clear_rx_global_counter(unitChip.unit, rxGlobalCounter->port);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get all counters of a specific SA entry in MACsec egress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] txSaCounter       MACsec get tx SA counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "txSaCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_tx_sa_counter(UnitChip_t unitChip, MacsecTxSaCounter_t *txSaCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != txSaCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index, &txSaCounter->counters);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index, &txSaCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index, &txSaCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index, &txSaCounter->counters);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to clear all counters of a specific SA entry in MACsec egress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  txSaCounter          MACsec clear tx SA counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "txSaCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_clear_tx_sa_counter(UnitChip_t unitChip, const MacsecClearTxSaCounter_t *txSaCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != txSaCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_clear_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_clear_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_clear_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_clear_tx_sa_counter(unitChip.unit, txSaCounter->port, txSaCounter->index);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get all counters of a specific SA entry in MACsec ingress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] rxSaCounter       MACsec get rx SA counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "rxSaCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_rx_sa_counter(UnitChip_t unitChip, MacsecRxSaCounter_t *rxSaCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != rxSaCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index, &rxSaCounter->counters);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index, &rxSaCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index, &rxSaCounter->counters);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index, &rxSaCounter->counters);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to clear all counters of a specific SA entry in MACsec ingress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  rxSaCounter          MACsec clear rx SA counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "rxSaCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_clear_rx_sa_counter(UnitChip_t unitChip, const MacsecClearRxSaCounter_t *rxSaCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != rxSaCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_clear_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_clear_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_clear_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_clear_rx_sa_counter(unitChip.unit, rxSaCounter->port, rxSaCounter->index);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to clear all counters of MACsec egress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  txCounter            MACsec clear all tx counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "txCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_clear_all_tx_counter(UnitChip_t unitChip, const MacsecClearAllTxCounter_t *txCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != txCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_clear_all_tx_counter(unitChip.unit, txCounter->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_clear_all_tx_counter(unitChip.unit, txCounter->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_clear_all_tx_counter(unitChip.unit, txCounter->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_clear_all_tx_counter(unitChip.unit, txCounter->port);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to clear all counters of MACsec ingress engine.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID. (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  rxCounter            MACsec clear all rx counter information. (N/A) \n
 *                                  Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval -RT_ERR_NULL_POINTER     The "rxCounter" parameter is NULL.
 * @retval Others                   Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_clear_all_rx_counter(UnitChip_t unitChip, const MacsecClearAllRxCounter_t *rxCounter)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != rxCounter)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_clear_all_rx_counter(unitChip.unit, rxCounter->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_clear_all_rx_counter(unitChip.unit, rxCounter->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_clear_all_rx_counter(unitChip.unit, rxCounter->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_clear_all_rx_counter(unitChip.unit, rxCounter->port);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**@}*/  // MACSEC_COUNTER

/**
 * @defgroup MACSEC_CLASSIFICATION MACsec Static Classification
 * @brief Configure the control packet classifier and vlan parser rules of the MACsec hardware module
 * @{
 */
/**
 * @brief This API is used to get the destination MAC address of a control packet classification entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                  Stacking unit and chip ID. (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] controlPacketMacEntry     MACsec get control packet MAC DA entry information. (N/A) \n
 *                                          Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                        Successfully retrieved the control packet entry.
 * @retval -RT_ERR_NOT_SUPPORT              Not supported.
 * @retval -RT_ERR_NULL_POINTER             The "controlPacketMacEntry" parameter is NULL.
 * @retval Others                           Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_control_packet_mac_da_entry(UnitChip_t unitChip, MacsecControlPacketMacDaEntry_t *controlPacketMacEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != controlPacketMacEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, &controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, &controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, &controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, &controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set a control packet classification entry to match a specific destination MAC address.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                     Stacking unit and chip ID. (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  controlPacketMacEntry        MACsec set control packet MAC DA entry information. (N/A) \n
 *                                          Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                        Successfully set the control packet entry.
 * @retval -RT_ERR_NOT_SUPPORT              Not supported.
 * @retval -RT_ERR_NULL_POINTER             The "controlPacketMacEntry" parameter is NULL.
 * @retval Others                           Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_control_packet_mac_da_entry(UnitChip_t unitChip, const MacsecControlPacketMacDaEntry_t *controlPacketMacEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != controlPacketMacEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_control_packet_mac_da_entry(unitChip.unit, controlPacketMacEntry->port, controlPacketMacEntry->direction, controlPacketMacEntry->entryIdx, controlPacketMacEntry->enable, controlPacketMacEntry->mac);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get the Ethernet type field of a control packet classification entry.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                    Stacking unit and chip ID. (N/A) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 *                                            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] controlPacketEtherTypeEntry MACsec get control packet EtherType entry information. (N/A) \n
 *                                            Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                          Successfully retrieved the control packet entry.
 * @retval -RT_ERR_NOT_SUPPORT                Not supported.
 * @retval -RT_ERR_NULL_POINTER               The "controlPacketEtherTypeEntry" parameter is NULL.
 * @retval Others                             Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_control_packet_ether_type_entry(UnitChip_t unitChip, MacsecControlPacketEtherTypeEntry_t *controlPacketEtherTypeEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != controlPacketEtherTypeEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, &controlPacketEtherTypeEntry->enable, &controlPacketEtherTypeEntry->etherType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, &controlPacketEtherTypeEntry->enable, &controlPacketEtherTypeEntry->etherType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, &controlPacketEtherTypeEntry->enable, &controlPacketEtherTypeEntry->etherType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, &controlPacketEtherTypeEntry->enable, &controlPacketEtherTypeEntry->etherType);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set a control packet classification entry to match a specific Ethernet type field.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                     Stacking unit and chip ID. (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  controlPacketEtherTypeEntry  MACsec set control packet EtherType entry information. (N/A) \n
 *                                          Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                        Successfully set the control packet entry.
 * @retval -RT_ERR_NOT_SUPPORT              Not supported.
 * @retval -RT_ERR_NULL_POINTER             The "controlPacketEtherTypeEntry" parameter is NULL.
 * @retval Others                           Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_control_packet_ether_type_entry(UnitChip_t unitChip, const MacsecControlPacketEtherTypeEntry_t *controlPacketEtherTypeEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != controlPacketEtherTypeEntry)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, controlPacketEtherTypeEntry->enable, controlPacketEtherTypeEntry->etherType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, controlPacketEtherTypeEntry->enable, controlPacketEtherTypeEntry->etherType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, controlPacketEtherTypeEntry->enable, controlPacketEtherTypeEntry->etherType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_control_packet_ether_type_entry(unitChip.unit, controlPacketEtherTypeEntry->port, controlPacketEtherTypeEntry->direction, controlPacketEtherTypeEntry->entryIdx, controlPacketEtherTypeEntry->enable, controlPacketEtherTypeEntry->etherType);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to get VLAN parser configurations.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                  Stacking unit and chip ID. (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanParserSettings        MACsec get VLAN parser settings information. (N/A) \n
 *                                          Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK                        Successfully retrieved the VLAN parser configuration.
 * @retval -RT_ERR_NOT_SUPPORT              Not supported.
 * @retval -RT_ERR_NULL_POINTER             The "vlanParserSettings" parameter is NULL.
 * @retval Others                           Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_get_vlan_parser_settings(UnitChip_t unitChip, MacsecVlanParserSettings_t *vlanParserSettings)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != vlanParserSettings)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_get_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_get_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_get_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_get_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief This API is used to set VLAN parser to recognize the VLAN tag(s) appended to MAC source address.\n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                     Stacking unit and chip ID. (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanParserSettings           MACsec set VLAN parser settings information. (N/A) \n
 *                                          Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK                        Successfully set the VLAN parser configuration.
 * @retval -RT_ERR_NOT_SUPPORT              Not supported.
 * @retval -RT_ERR_NULL_POINTER             The "vlanParserSettings" parameter is NULL.
 * @retval Others                           Please refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_macsec_set_vlan_parser_settings(UnitChip_t unitChip, const MacsecVlanParserSettings_t *vlanParserSettings)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != vlanParserSettings)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macsec_set_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macsec_set_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macsec_set_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macsec_set_vlan_parser_settings(unitChip.unit, vlanParserSettings->port, vlanParserSettings->direction, &vlanParserSettings->vlanParser);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**@}*/  // MACSEC_CLASSIFICATION

/**@}*/  // macsec