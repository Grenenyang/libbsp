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

#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_macsec.h"
#include "rtl907xd_v1_asicdrv_macsec.h"

#define DV1_CHK_NULL_POINTER(r, p)      \
    if((int32)RT_ERR_OK == (int32)(r))  \
    {                                   \
        if(NULL == (p))                 \
        {                               \
            (r) = -RT_ERR_NULL_POINTER; \
            continue;                   \
        }                               \
    }

/**
 * @addtogroup RTK_MODULE_MACSEC MACsec
 * @{
 */

/**
 * @addtogroup MACSEC_MATCH_RULE MACsec Match Rule
 * @{
 */

/**
 * @defgroup MACSEC_MATCH_RULE_ASIC Low Level Driver
 * @brief MACsec Match Rule Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get basic parameters of a specific match rule entry.
 *
 * @note This API should be called as part of a series of API calls
 *        (dv1_macsec_get_match_rule_params(), dv1_macsec_get_match_rule_vlan_params(), dv1_macsec_get_match_rule_misc_params()),
 *        to ensure all parameters for a specific match rule entry are retrieved correctly.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[out] params               Parameters. \n
 *                                  The detailed configuration parameters of this match rule entry.
 *
 * @retval RT_ERR_OK                Successfully retrieved the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_mr_mask_get(), dv1_drv_secy_mr_source_port_get(), dv1_drv_secy_mr_sci_get(), dv1_drv_secy_mr_an_get().
 */
RtkApiRet dv1_macsec_get_match_rule_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecSaMatchEntry_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    mask    = 0u;
    uint32    value   = 0u;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get current enable match mask */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_get(unit, sysPort, direction, entryIdx, &mask));

        /* Verify if source MAC data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_SOURCE_MAC_MASK) != 0u)
        {
            /* If so, get source MAC data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mac_sa_get(unit, sysPort, direction, entryIdx, params->macSrcAddr));
            /* Set the source MAC bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_SOURCE_MAC);
        }

        /* Verify if destination MAC data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_DESTINATION_MAC_MASK) != 0u)
        {
            /* If so, get destination MAC data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mac_da_get(unit, sysPort, direction, entryIdx, params->macDestAddr));
            /* Set the destination MAC bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_DESTINATION_MAC);
        }

        /* Verify if EtherType data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_ETHER_TYPE_MASK) != 0u)
        {
            /* If so, get EtherType data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_ethertype_get(unit, sysPort, direction, entryIdx, &value));
            params->etherType = (uint16)value;
            /* Set the EtherType bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_ETHER_TYPE);
        }

        /* Get the index of the flow control entry that defines an action for this match rule */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_flow_ctrl_get(unit, sysPort, direction, entryIdx, &value));
        params->flowCtrlIdx = (uint8)value;

    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get VLAN related parameters of a specific match rule entry.
 *
 * @note This API should be called as part of a series of API calls
 *        (dv1_macsec_get_match_rule_params(), dv1_macsec_get_match_rule_vlan_params(), dv1_macsec_get_match_rule_misc_params()),
 *        to ensure all parameters for a specific match rule entry are retrieved correctly.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[out] params               Parameters. \n
 *                                  The detailed configuration parameters of this match rule entry.
 *
 * @retval RT_ERR_OK                Successfully retrieved the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_mr_mask_get(), dv1_drv_secy_mr_source_port_get(), dv1_drv_secy_mr_sci_get(), dv1_drv_secy_mr_an_get().
 */
RtkApiRet dv1_macsec_get_match_rule_vlan_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecSaMatchEntry_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    mask    = 0u;
    uint32    value   = 0u;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get current enable match mask for this entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_get(unit, sysPort, direction, entryIdx, &mask));

        /* Verify if outer VLAN ID data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_OUTER_VLAN_ID_MASK) != 0u)
        {
            /* If so, get outer VLAN ID data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_vlan_id_get(unit, sysPort, direction, entryIdx, &value));
            params->outerVlanid = (uint16)value;
            /* Set the outer VLAN ID bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_OUTER_VLAN_ID);
        }

        /* Verify if inner VLAN ID data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_INNER_VLAN_ID_MASK) != 0u)
        {
            /* If so, get inner VLAN ID data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_inner_vlan_id_get(unit, sysPort, direction, entryIdx, &value));
            params->innerVlanid = (uint16)value;
            /* Set the inner VLAN ID bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_INNER_VLAN_ID);
        }

        /* Verify if outer VLAN PCP data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_OUTER_VLAN_UP_MASK) != 0u)
        {
            /* If so, get outer VLAN PCP data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_vlan_up_get(unit, sysPort, direction, entryIdx, &value));
            params->outerVlanPcp = (uint8)value;
            /* Set the outer VLAN PCP bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_OUTER_VLAN_PCP);
        }

        /* Verify if inner VLAN PCP data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_INNER_VLAN_UP_MASK) != 0u)
        {
            /* If so, get inner VLAN PCP data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_inner_vlan_up_get(unit, sysPort, direction, entryIdx, &value));
            params->innerVlanPcp = (uint8)value;
            /* Set the inner VLAN PCP bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_INNER_VLAN_PCP);
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get miscellaneous parameters of a specific match rule entry.
 *
 * @note This API should be called as part of a series of API calls
 *        (dv1_macsec_get_match_rule_params(), dv1_macsec_get_match_rule_vlan_params(), dv1_macsec_get_match_rule_misc_params()),
 *        to ensure all parameters for a specific match rule entry are retrieved correctly.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[out] params               Parameters. \n
 *                                  The detailed configuration parameters of this match rule entry.
 *
 * @retval RT_ERR_OK                Successfully retrieved the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_mr_mask_get(), dv1_drv_secy_mr_source_port_get(), dv1_drv_secy_mr_sci_get(), dv1_drv_secy_mr_an_get().
 */
RtkApiRet dv1_macsec_get_match_rule_misc_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecSaMatchEntry_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    mask    = 0u;
    uint32    value   = 0u;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get current enable match mask for this entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_get(unit, sysPort, direction, entryIdx, &mask));

        /* Verify if source port data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_SOURCE_PORT_MASK) != 0u)
        {
            /* If so, get source port data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_source_port_get(unit, sysPort, direction, entryIdx, &value));
            params->sourcePort = (uint8)value;
            /* Set the source port bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_SOURCE_PORT);
        }

        /* Verify if SCI data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_SCI_MASK) != 0u)
        {
            /* If so, get SCI data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_sci_get(unit, sysPort, direction, entryIdx, params->sci));
            /* Set the SCI bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_SCI);
        }

        /* Verify if AN data is enabled to be compared for this entry */
        if((mask & DV1_MACSEC_SECY_SA_MATCH_AN_0_MASK) != 0u)
        {
            /* If so, get AN data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_an_get(unit, sysPort, direction, entryIdx, &value));
            params->an |= (((uint8)((value >> 0u) & 0x01u)) << 0u);
            /* Set the AN_0 bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_AN_0);
        }

        if((mask & DV1_MACSEC_SECY_SA_MATCH_AN_1_MASK) != 0u)
        {
            /* If so, get AN data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_an_get(unit, sysPort, direction, entryIdx, &value));
            params->an |= (((uint8)((value >> 1u) & 0x01u)) << 1u);
            /* Set the AN_1 bit in enableMask */
            params->enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_AN_1);
        }

    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set basic parameters of a specific match rule entry.
 *
 * @note This API should be called as part of a series of API calls
 *        (dv1_macsec_set_match_rule_params(), dv1_macsec_set_match_rule_vlan_params(), dv1_macsec_set_match_rule_misc_params()),
 *        to ensure all parameters for a specific match rule entry are set correctly.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[in] params                Parameters. \n
 *                                  The detailed configuration parameters of this match rule entry.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_mr_mask_get(), dv1_drv_secy_mr_source_port_set(), dv1_drv_secy_mr_sci_set(), dv1_drv_secy_mr_an_set(), dv1_drv_secy_mr_mask_set().
 */
RtkApiRet dv1_macsec_set_match_rule_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecSaMatchEntry_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    mask    = 0u;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Validate basic parameter data */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_match_rule_parameter_check(params));

        /* Get current enable match mask for this entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_get(unit, sysPort, direction, entryIdx, &mask));

        /* Verify if source MAC address is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_SOURCE_MAC))
        {
            /* If so, set source MAC address */
            mask |= DV1_MACSEC_SECY_SA_MATCH_SOURCE_MAC_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mac_sa_set(unit, sysPort, direction, entryIdx, params->macSrcAddr));
        }

        /* Verify if destination MAC address is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_DESTINATION_MAC))
        {
            /* If so, set destination MAC address */
            mask |= DV1_MACSEC_SECY_SA_MATCH_DESTINATION_MAC_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mac_da_set(unit, sysPort, direction, entryIdx, params->macDestAddr));
        }

        /* Verify if EtherType is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_ETHER_TYPE))
        {
            /* If so, set EtherType */
            mask |= DV1_MACSEC_SECY_SA_MATCH_ETHER_TYPE_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_ethertype_set(unit, sysPort, direction, entryIdx, params->etherType));
        }

        /* Verify if flow control entry index is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_FLOW_CTRL))
        {
            /* If so, set flow control entry index */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_flow_ctrl_set(unit, sysPort, direction, entryIdx, params->flowCtrlIdx));
        }

        /* Update compare mask */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_set(unit, sysPort, direction, entryIdx, mask));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set VLAN related parameters of a specific match rule entry.
 *
 * @note This API should be called as part of a series of API calls
 *        (dv1_macsec_set_match_rule_params(), dv1_macsec_set_match_rule_vlan_params(), dv1_macsec_set_match_rule_misc_params()),
 *        to ensure all parameters for a specific match rule entry are set correctly.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[in] params                Parameters. \n
 *                                  The detailed configuration parameters of this match rule entry.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_mr_mask_get(), dv1_drv_secy_mr_source_port_set(), dv1_drv_secy_mr_sci_set(), dv1_drv_secy_mr_an_set(), dv1_drv_secy_mr_mask_set().
 */
RtkApiRet dv1_macsec_set_match_rule_vlan_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecSaMatchEntry_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    mask    = 0u;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Validate VLAN parameter data */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_vlan_match_rule_parameter_check(params));

        /* Get current enable match mask for this entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_get(unit, sysPort, direction, entryIdx, &mask));

        /* Verify if outer VLAN ID is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_OUTER_VLAN_ID))
        {
            /* If so, set outer VLAN ID */
            mask |= DV1_MACSEC_SECY_SA_MATCH_OUTER_VLAN_ID_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_vlan_id_set(unit, sysPort, direction, entryIdx, params->outerVlanid));
        }

        /* Verify if inner VLAN ID is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_INNER_VLAN_ID))
        {
            /* If so, set inner VLAN ID */
            mask |= DV1_MACSEC_SECY_SA_MATCH_INNER_VLAN_ID_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_inner_vlan_id_set(unit, sysPort, direction, entryIdx, params->innerVlanid));
        }

        /* Verify if outer VLAN PCP is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_OUTER_VLAN_PCP))
        {
            /* If so, set outer VLAN PCP */
            mask |= DV1_MACSEC_SECY_SA_MATCH_OUTER_VLAN_UP_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_vlan_up_set(unit, sysPort, direction, entryIdx, params->outerVlanPcp));
        }

        /* Verify if inner VLAN PCP is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_INNER_VLAN_PCP))
        {
            /* If so, set inner VLAN PCP */
            mask |= DV1_MACSEC_SECY_SA_MATCH_INNER_VLAN_UP_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_inner_vlan_up_set(unit, sysPort, direction, entryIdx, params->innerVlanPcp));
        }

        /* Update compare mask */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_set(unit, sysPort, direction, entryIdx, mask));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set miscellaneous parameters of a specific match rule entry.
 *
 * @note This API should be called as part of a series of API calls
 *        (dv1_macsec_set_match_rule_params(), dv1_macsec_set_match_rule_vlan_params(), dv1_macsec_set_match_rule_misc_params()),
 *        to ensure all parameters for a specific match rule entry are set correctly.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[in] params                Parameters. \n
 *                                  The detailed configuration parameters of this match rule entry.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_mr_mask_get(), dv1_drv_secy_mr_source_port_set(), dv1_drv_secy_mr_sci_set(), dv1_drv_secy_mr_an_set(), dv1_drv_secy_mr_mask_set().
 */
RtkApiRet dv1_macsec_set_match_rule_misc_params(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecSaMatchEntry_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    mask    = 0u;
    uint32    an      = 0u;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Validate basic data */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_match_rule_parameter_check(params));

        /* Get current enable match mask for this entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_get(unit, sysPort, direction, entryIdx, &mask));

        /* Verify if source port is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_SOURCE_PORT))
        {
            /* If so, set source port */
            mask |= DV1_MACSEC_SECY_SA_MATCH_SOURCE_PORT_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_source_port_set(unit, sysPort, direction, entryIdx, params->sourcePort));
        }

        /* Verify if SCI is modified */
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_SCI))
        {
            /* If so, set SCI */
            mask |= DV1_MACSEC_SECY_SA_MATCH_SCI_MASK;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_sci_set(unit, sysPort, direction, entryIdx, params->sci));
        }

        /* Verify if AN is modified */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_an_get(unit, sysPort, direction, entryIdx, &an));
        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_AN_0))
        {
            /* If so, set AN_0 */
            mask |= DV1_MACSEC_SECY_SA_MATCH_AN_0_MASK;
            an |= (params->an & (uint32)0x1u);
        }

        if(dv1_drv_secy_helper_is_params_modified(params->modifiedMask, RTK_MACSEC_SA_MATCH_AN_1))
        {
            /* If so, set AN_1 */
            mask |= DV1_MACSEC_SECY_SA_MATCH_AN_1_MASK;
            an |= (params->an & (uint32)0x2u);
        }
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_an_set(unit, sysPort, direction, entryIdx, an));

        /* Update compare mask */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_mask_set(unit, sysPort, direction, entryIdx, mask));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get the status of a specific MACsec match rule.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[out] enable               Enable (false to true). \n
 *                                  The status of a specific MACsec flow.
 *
 * @retval RT_ERR_OK                Successfully retrieved the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "enable" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_mr_entry_enable_get().
 */
RtkApiRet dv1_macsec_get_match_rule_status(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool *enable)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, enable);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get enable or disable status for this match entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_entry_enable_get(unit, sysPort, direction, entryIdx, enable));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to enable or disable a specific MACsec match rule.
 *        Before a MACsec flow is enabled, the match rule, and its corresponding flow control and SA entry MUST be configured properly,
 *        otherwise the behavior is unpredictable.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the match rule entry.
 * @param[in] enable                Enable (false to true). \n
 *                                  The status of a specific MACsec flow.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the match rule entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_mr_entry_enable_set().
 */
RtkApiRet dv1_macsec_set_match_rule_status(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool enable)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Set enable or disable status for this match entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_mr_entry_enable_set(unit, sysPort, direction, entryIdx, enable));
    } while(false);

    return retCode;
}

/** @} */ /* end of MACSEC_MATCH_RULE_ASIC */

/** @} */ /* end of MACSEC_MATCH_RULE */

/**
 * @addtogroup MACSEC_FLOW_CTRL MACsec Flow Control
 * @{
 */

/**
 * @defgroup MACSEC_FLOW_CTRL_ASIC Low Level Driver
 * @brief MACsec Flow Control Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get a flow control entry.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the flow control entry.
 * @param[out] params               Parameters. \n
 *                                  The detailed configuration parameters of this flow control entry.
 *
 * @retval RT_ERR_OK                Successfully retrieved the flow control entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_fc_common_params_get(), dv1_drv_secy_fc_rx_params_get(), dv1_drv_secy_fc_tx_params_get().
 */
RtkApiRet dv1_macsec_get_flow_control_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecFlowControl_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get common parameters of this flow control entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_common_params_get(unit, sysPort, direction, entryIdx, params));

        /* Verify if direction is RX */
        if(EM_RTK_MACSEC_DIRECTION_RX == direction)
        {
            /* If so, get RX specific parameters of this flow control entry */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_rx_params_get(unit, sysPort, entryIdx, params));
        }

        /* Verify if direction is TX */
        if(EM_RTK_MACSEC_DIRECTION_TX == direction)
        {
            /* If so, get TX specific parameters of this flow control entry */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_tx_params_get(unit, sysPort, entryIdx, params));
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set a flow control entry.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the flow control entry.
 * @param[in] params                Parameters. \n
 *                                  The detailed configuration parameters of this flow control entry.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the flow control entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_fc_common_params_set(), dv1_drv_secy_fc_rx_params_set(), dv1_drv_secy_fc_tx_params_set().
 */
RtkApiRet dv1_macsec_set_flow_control_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecFlowControl_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode    = RT_ERR_OK;
    uint32    sysPort    = 0u;
    uint32    flowType   = 0u;
    uint32    sourcePort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Validate flow control parameters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_flow_control_parameter_check(params));

        /* Set common parameters of this flow control entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_common_params_set(unit, sysPort, direction, entryIdx, params));

        /* Verify if direction is RX */
        if(EM_RTK_MACSEC_DIRECTION_RX == direction)
        {
            /* If so, set RX specific parameters of this flow control entry */
            flowType   = DV1_MACSEC_SECY_FLOW_CTRL_FLOW_TYPE_RX;
            sourcePort = DV1_MACSEC_SECY_FLOW_CTRL_DEST_PORT_CONTROLLED;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_rx_params_set(unit, sysPort, entryIdx, params));
        }

        /* Verify if direction is TX */
        if(EM_RTK_MACSEC_DIRECTION_TX == direction)
        {
            /* If so, set TX specific parameters of this flow control entry */
            flowType   = DV1_MACSEC_SECY_FLOW_CTRL_FLOW_TYPE_TX;
            sourcePort = DV1_MACSEC_SECY_FLOW_CTRL_DEST_PORT_COMMON;
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_tx_params_set(unit, sysPort, entryIdx, params));
        }

        /* Set flow type */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_flow_type_set(unit, sysPort, direction, entryIdx, flowType));

        /* Set destination port */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_fc_dest_port_set(unit, sysPort, direction, entryIdx, sourcePort));
    } while(false);

    return retCode;
}

/** @} */ /* end of MACSEC_FLOW_CTRL_ASIC */

/** @} */ /* end of MACSEC_FLOW_CTRL */

/**
 * @addtogroup MACSEC_SA MACsec SA(Secruity Association)
 * @{
 */

/**
 * @defgroup MACSEC_SA_ASIC Low Level Driver
 * @brief MACsec Security Association Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get a security association (SA) entry.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the SA entry.
 * @param[in] cipherSuite           Cipher Suite (EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128 to EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256). \n
 *                                  The cipher suite of this SA.
 * @param[out] params               Parameters. \n
 *                                  The detailed configuration parameters of this SA entry.
 *
 * @retval RT_ERR_OK                Successfully retrieved the SA entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_sa_key_get(), dv1_drv_secy_sa_sci_get(), dv1_drv_secy_sa_seq_get(), dv1_drv_secy_sa_mask_get(), dv1_drv_secy_sa_ctx_salt_get().
 */
RtkApiRet dv1_macsec_get_sa_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, uint32 cipherSuite, RtkMacsecSecurityAssociation_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Validate SA entry parameters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_sa_entry_parameter_check(cipherSuite, params));

        /* Get all SA entry parameters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_an_get(unit, sysPort, direction, entryIdx, &params->an));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_key_get(unit, sysPort, direction, entryIdx, cipherSuite, params->key, params->hkey));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_sci_get(unit, sysPort, direction, entryIdx, cipherSuite, params->sci));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_seq_get(unit, sysPort, direction, entryIdx, cipherSuite, &params->pn));

        /* Verify if direction is RX */
        if(EM_RTK_MACSEC_DIRECTION_RX == direction)
        {
            /* If so, get replay window */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_mask_get(unit, sysPort, direction, entryIdx, cipherSuite, &params->replayWindow));
        }

        /* Verify if cipher suite is XPN type */
        if((RTK_MACSEC_CIPHERSUITE_GCM_XPN_128 == cipherSuite) || (RTK_MACSEC_CIPHERSUITE_GCM_XPN_256 == cipherSuite))
        {
            /* If so, get context salt */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_ctx_salt_get(unit, sysPort, direction, entryIdx, cipherSuite, params->salt));
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set a security association (SA) entry.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of the SA entry.
 * @param[in] cipherSuite           Cipher Suite (EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128 to EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256). \n
 *                                  The cipher suite of this SA.
 * @param[in] params                Parameters. \n
 *                                  The detailed configuration parameters of this SA entry.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the SA entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "params" parameter is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_sa_init(), dv1_drv_secy_sa_key_set(), dv1_drv_secy_sa_sci_set(), dv1_drv_secy_sa_seq_set(), dv1_drv_secy_sa_mask_set(), dv1_drv_secy_sa_ctx_salt_set().
 */
RtkApiRet dv1_macsec_set_sa_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, uint32 cipherSuite, const RtkMacsecSecurityAssociation_t *params)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, params);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Validate SA entry parameters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_sa_entry_parameter_check(cipherSuite, params));

        /* Initialize control parameters based on direction and cipher suite (key length), AN is used for egress */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_init(unit, sysPort, direction, entryIdx, cipherSuite, params->an));

        /* Set all SA entry parameters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_key_set(unit, sysPort, direction, entryIdx, cipherSuite, params->key, params->hkey));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_sci_set(unit, sysPort, direction, entryIdx, cipherSuite, params->sci));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_seq_set(unit, sysPort, direction, entryIdx, cipherSuite, params->pn));

        /* Verify if direction is RX */
        if(EM_RTK_MACSEC_DIRECTION_RX == direction)
        {
            /* If so, set replay window */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_mask_set(unit, sysPort, direction, entryIdx, cipherSuite, params->replayWindow));
        }

        /* Verify if cipher suite is XPN type */
        if((RTK_MACSEC_CIPHERSUITE_GCM_XPN_128 == cipherSuite) || (RTK_MACSEC_CIPHERSUITE_GCM_XPN_256 == cipherSuite))
        {
            /* If so, set context salt and SSCI */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_sa_ctx_salt_set(unit, sysPort, direction, entryIdx, cipherSuite, params->salt, params->ssci));
        }
    } while(false);

    return retCode;
}

/** @} */ /* end of MACSEC_SA_ASIC */

/** @} */ /* end of MACSEC_SA */

/**
 * @addtogroup MACSEC_COUNTER MACsec Counters
 * @{
 */

/**
 * @defgroup MACSEC_COUNTER_ASIC Low Level Driver
 * @brief MACsec Counters Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get all global counters of MACsec egress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[out] txCounters           Tx Counters. \n
 *                                  Per SA entry counters of MACsec egress engine.
 *
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "txCounters" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_counter_global_get().
 */
RtkApiRet dv1_macsec_get_tx_global_counter(uint32 unit, RtkPort port, RtkMacsecTxGlobalCounter_t *txCounters)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, txCounters);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get all tx global counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_TRANSFORM_ERROR_PKTS, &txCounters->transformErrorPkts));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_CTRL, &txCounters->outPktsCtrl));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_UNKNOWNSA, &txCounters->outPktsUnknownSa));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_UNTAGGED, &txCounters->outPktsUntagged));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_OVERSIZE_PKTS, &txCounters->outOversizePkts));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to clear all global counters of MACsec egress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_counter_global_clear().
 */
RtkApiRet dv1_macsec_clear_tx_global_counter(uint32 unit, RtkPort port)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Clear all tx global counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_TRANSFORM_ERROR_PKTS));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_CTRL));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_UNKNOWNSA));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_PKTS_UNTAGGED));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, DV1_MACSEC_SECY_TX_GLOBAL_OUT_OVERSIZE_PKTS));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get all global counters of MACsec ingress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[out] rxCounters           Rx Counters. \n
 *                                  Per SA entry counters of MACsec ingress engine.
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "rxCounters" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_counter_rxgp1_get(), dv1_drv_secy_counter_rxgp2_get().
 */
RtkApiRet dv1_macsec_get_rx_global_counter(uint32 unit, RtkPort port, RtkMacsecRxGlobalCounter_t *rxCounters)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, rxCounters);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get all rx global counters by group (due to CCM requirement) */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_rxgp1_get(unit, sysPort, rxCounters));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_rxgp2_get(unit, sysPort, rxCounters));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to clear all global counters of MACsec ingress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_counter_rxgp1_clear(), dv1_drv_secy_counter_rxgp2_clear().
 */
RtkApiRet dv1_macsec_clear_rx_global_counter(uint32 unit, RtkPort port)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Clear all rx global counters by group (due to CCM requirement) */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_rxgp1_clear(unit, sysPort));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_rxgp2_clear(unit, sysPort));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get all counters of a specific SA entry in MACsec egress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] index                 SA entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of a specific SA entry.
 * @param[out] txCounters           Tx Counters. \n
 *                                  Per SA entry counters of MACsec egress engine.
 *
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "index" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "txCounters" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_counter_sa_get().
 */
RtkApiRet dv1_macsec_get_tx_sa_counter(uint32 unit, RtkPort port, uint8 index, RtkMacsecTxSaCounter_t *txCounters)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, txCounters);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get all tx per sa counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, index, DV1_MACSEC_SECY_RX_SA_IN_OCTETS_DECRYPYED, &txCounters->outOctetsEncrypted));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNCHECKED, &txCounters->outPktsEncrypted));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_DELAYED, &txCounters->outPktsTooLong));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to clear all counters of a specific SA entry in MACsec egress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] index                 SA entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of a specific SA entry.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "index" parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_counter_sa_clear().
 */
RtkApiRet dv1_macsec_clear_tx_sa_counter(uint32 unit, RtkPort port, uint8 index)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Clear all tx per sa counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, index, DV1_MACSEC_SECY_RX_SA_IN_OCTETS_DECRYPYED));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNCHECKED));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_DELAYED));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get all counters of a specific SA entry in MACsec ingress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] index                 SA entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of a specific SA entry.
 * @param[out] rxCounters           Rx Counters. \n
 *                                  Per SA entry counters of MACsec ingress engine.
 *
 * @retval RT_ERR_OK                Successfully retrieved all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "index" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "rxCounters" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_counter_sa_get().
 */
RtkApiRet dv1_macsec_get_rx_sa_counter(uint32 unit, RtkPort port, uint8 index, RtkMacsecRxSaCounter_t *rxCounters)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, rxCounters);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get all rx per sa counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_OCTETS_DECRYPYED, &rxCounters->inOctetsDecrypted));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNCHECKED, &rxCounters->inPktsUnchecked));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_DELAYED, &rxCounters->inPktsDelayed));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_LATE, &rxCounters->inPktsLate));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_OK, &rxCounters->inPktsOk));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_INVALID, &rxCounters->inPktsInvalid));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_NOT_VALID, &rxCounters->inPktsNotValid));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_NOT_USING_SA, &rxCounters->inPktsNotUsingSa));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNUSED_SA, &rxCounters->inPktsUnusedSa));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNTAGGED_HIT, &rxCounters->inPktsUntaggedHit));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to clear all counters of a specific SA entry in MACsec ingress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] index                 SA entry index (RTK_MACSEC_MIN_ENTRY_IDX to RTK_MACSEC_MAX_ENTRY_IDX). \n
 *                                  The index of a specific SA entry.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "index" parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_counter_sa_clear().
 */
RtkApiRet dv1_macsec_clear_rx_sa_counter(uint32 unit, RtkPort port, uint8 index)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Clear all rx per sa counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_OCTETS_DECRYPYED));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNCHECKED));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_DELAYED));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_LATE));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_OK));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_INVALID));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_NOT_VALID));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_NOT_USING_SA));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNUSED_SA));
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_sa_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, index, DV1_MACSEC_SECY_RX_SA_IN_PKTS_UNTAGGED_HIT));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to clear all counters of MACsec egress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_counter_clear_all().
 */
RtkApiRet dv1_macsec_clear_all_tx_counter(uint32 unit, RtkPort port)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Clear all tx counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_clear_all(unit, sysPort, EM_RTK_MACSEC_DIRECTION_TX));
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to clear all counters of MACsec ingress engine.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully cleared all counters.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_counter_clear_all().
 */
RtkApiRet dv1_macsec_clear_all_rx_counter(uint32 unit, RtkPort port)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Clear all rx counters */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_counter_clear_all(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX));
    } while(false);

    return retCode;
}

/** @} */ /* end of MACSEC_COUNTER_ASIC */

/** @} */ /* end of MACSEC_COUNTER */

/**
 * @addtogroup MACSEC_CLASSIFICATION MACsec Static Classification
 * @{
 */

/**
 * @defgroup MACSEC_CLASSIFICATION_ASIC Low Level Driver
 * @brief MACsec Static Classification Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get the destination MAC address of a control packet classification entry.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX to RTK_MACSEC_MAX_CTRL_PACKET_ENTRY_IDX). \n
 *                                  The index of the control packet classification entry.
 * @param[out] enable               Enable (true to false). \n
 *                                  The status of this entry.
 * @param[out] macDa                Destination MAC address (0x0 to 0xFF for 6 elements). \n
 *                                  A specific destination MAC address that will be classified as control packet.
 *
 * @retval RT_ERR_OK                Successfully retrieved the control packet entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "enable" or "macDa" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_cp_da_enable_get(), dv1_drv_secy_cp_da_get().
 */
RtkApiRet dv1_macsec_get_control_packet_mac_da_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool *enable, uint8 *macDa)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, enable);
        DV1_CHK_NULL_POINTER(retCode, macDa);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get status of this entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_da_enable_get(unit, sysPort, direction, entryIdx, enable));

        /* Verify if this entry is enabled */
        if(*enable)
        {
            /* If so, get data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_da_get(unit, sysPort, direction, entryIdx, macDa));
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set a control packet classification entry to match a specific destination MAC address.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX to RTK_MACSEC_MAX_CTRL_PACKET_ENTRY_IDX). \n
 *                                  The index of the control packet classification entry.
 * @param[in] enable                Enable (true to false). \n
 *                                  The status of this entry.
 * @param[in] macDa                 Destination MAC address (0x0 to 0xFF for 6 elements). \n
 *                                  A specific destination MAC address that will be classified as control packet.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the control packet entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_cp_da_enable_set(), dv1_drv_secy_cp_da_enable_set().
 */
RtkApiRet dv1_macsec_set_control_packet_mac_da_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool enable, const uint8 *macDa)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, macDa);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));
        const uint8 macAddrDefaultEmpty[DV1_MAC_ADDR_LEN] = {0u, 0u, 0u, 0u, 0u, 0u};

        /* Configure this entry */
        if(enable)
        {
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_da_set(unit, sysPort, direction, entryIdx, macDa));
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_da_enable_set(unit, sysPort, direction, entryIdx, true));
        }
        else
        {
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_da_set(unit, sysPort, direction, entryIdx, macAddrDefaultEmpty));
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_da_enable_set(unit, sysPort, direction, entryIdx, false));
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get the Ethernet type field of a control packet classification entry.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX to RTK_MACSEC_MAX_CTRL_PACKET_ENTRY_IDX). \n
 *                                  The index of the control packet classification entry.
 * @param[out] enable               Enable (true to false). \n
 *                                  The status of this entry.
 * @param[out] etherType            Ethernet type (0x0 to 0xFFFF). \n
 *                                  A specific EtherType value that will be classified as control packet.
 *
 * @retval RT_ERR_OK                Successfully retrieved the control packet entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "enable" or "etherType" parameter is NULL.
 * @retval Others                   Please refer to dv1_drv_secy_cp_et_enable_get(), dv1_drv_secy_cp_et_get().
 */
RtkApiRet dv1_macsec_get_control_packet_ether_type_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool *enable, uint16 *etherType)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    value   = 0u;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, enable);
        DV1_CHK_NULL_POINTER(retCode, etherType);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get status of this entry */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_et_enable_get(unit, sysPort, direction, entryIdx, enable));

        /* Verify if this entry is enabled */
        if(*enable)
        {
            /* If so, get data */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_et_get(unit, sysPort, direction, entryIdx, &value));
            *etherType = (uint16)value;
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set a control packet classification entry to match a specific Ethernet type field.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] entryIdx              Entry index (RTK_MACSEC_MIN_CTRL_PACKET_ENTRY_IDX to RTK_MACSEC_MAX_CTRL_PACKET_ENTRY_IDX). \n
 *                                  The index of the control packet classification entry.
 * @param[in] enable                Enable (true to false). \n
 *                                  The status of this entry.
 * @param[in] etherType             Ethernet type (0x0 to 0xFFFF). \n
 *                                  A specific EtherType value that will be classified as control packet.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the control packet entry.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_ENTRY_INDEX      The "entryIdx" parameter is out of range.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_cp_et_set(), dv1_drv_secy_cp_et_enable_set().
 */
RtkApiRet dv1_macsec_set_control_packet_ether_type_entry(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, uint8 entryIdx, bool enable, uint16 etherType)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Configure this entry */
        if(enable)
        {
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_et_set(unit, sysPort, direction, entryIdx, etherType));
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_et_enable_set(unit, sysPort, direction, entryIdx, true));
        }
        else
        {
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_et_set(unit, sysPort, direction, entryIdx, 0x0u));
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_cp_et_enable_set(unit, sysPort, direction, entryIdx, false));
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to get VLAN parser configurations.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[out] vlanParser           VLAN Parser settings. (N/A) \n
 *                                  Control VLAN single or double tag parsing and the corresponding TPID.
 *
 * @retval RT_ERR_OK                Successfully retrieved the VLAN parser configuration.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "vlanParser" parameter is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_vp_stag_get(), dv1_drv_secy_vp_qtag_get(), dv1_drv_secy_vp_control_get().
 */
RtkApiRet dv1_macsec_get_vlan_parser_settings(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, RtkMacsecVlanPaser_t *vlanParser)
{
    /* Initialize local variables */
    RtkApiRet retCode   = RT_ERR_OK;
    bool      parseQinQ = false;
    bool      parseStag = false;
    uint32    value     = 0u;
    uint32    sysPort   = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, vlanParser);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        /* Get status of parsing q-in-q and 802.1s tag */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_control_get(unit, sysPort, direction, &parseQinQ, &parseStag));

        if(parseQinQ)
        {
            /* If parseQinQ is enabled, it means the switch is configured to support double tag */
            vlanParser->type = EM_RTK_MACSEC_VLAN_PARSER_TYPE_DOUBLE_TAG;
            /* Get STAG, also known as 802.1s tag internally */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_stag_get(unit, sysPort, direction, &value));
            vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_OUTER_TPID] = (uint16)value;
            /* Get CTAG, also known as 802.1q tag internally */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_qtag_get(unit, sysPort, direction, &value));
            vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_INNER_TPID] = (uint16)value;
        }
        else
        {
            /* If parseQinQ is enabled, it means the switch is configured to support single tag only */
            vlanParser->type = EM_RTK_MACSEC_VLAN_PARSER_TYPE_SINGLE_TAG;
            /* Get TPID of single tag */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_qtag_get(unit, sysPort, direction, &value));
            vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_INNER_TPID] = (uint16)value;
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set VLAN parser to recognize the VLAN tag(s) appended to MAC source address.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port identity (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] direction             Direction (EM_RTK_MACSEC_DIRECTION_TX to EM_RTK_MACSEC_DIRECTION_RX). \n
 *                                  The ingress or egress direction of the MACsec engine.
 * @param[in] vlanParser            VLAN Parser settings. (N/A) \n
 *                                  Control VLAN single or double tag parsing and the corresponding TPID.
 * @param[out] None
 *
 * @retval RT_ERR_OK                Successfully set the VLAN parser configuration.
 * @retval -RT_ERR_PORT_ID          The "port" parameter is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "vlanParser" parameter is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_drv_secy_vp_stag_set(), dv1_drv_secy_vp_qtag_set(), dv1_drv_secy_vp_control_set().
 */
RtkApiRet dv1_macsec_set_vlan_parser_settings(uint32 unit, RtkPort port, RtkMacsecDirection_e direction, const RtkMacsecVlanPaser_t *vlanParser)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check NULL pointer parameters */
        DV1_CHK_NULL_POINTER(retCode, vlanParser);

        /* Convert user port id to sys port id */
        CHK_FUN_CONTINUE(retCode, dv1_drv_secy_helper_verify_user_port_and_covert_to_sys_port(port, &sysPort));

        if(EM_RTK_MACSEC_VLAN_PARSER_TYPE_DOUBLE_TAG == vlanParser->type)
        {
            /* Set STAG, also known as 802.1s tag internally, if it is not default value (0x88a8) */
            if(vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_OUTER_TPID] != RTK_MACSEC_VLAN_PARSER_DEFAULT_OUTER_TPID)
            {
                CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_stag_set(unit, sysPort, direction, vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_OUTER_TPID]));
            }

            /* Set CTAG, also known as 802.1q tag internally, if it is not default value (0x8100) */
            if(vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_INNER_TPID] != RTK_MACSEC_VLAN_PARSER_DEFAULT_INNER_TPID)
            {
                CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_qtag_set(unit, sysPort, direction, vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_INNER_TPID]));
            }

            /* Enable parse q-in-q and 802.1s tag */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_control_set(unit, sysPort, direction, true, true));
        }
        else if(EM_RTK_MACSEC_VLAN_PARSER_TYPE_SINGLE_TAG == vlanParser->type)
        {
            /* Set TPID of single tag if it is not default value (0x8100) */
            if(vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_INNER_TPID] != RTK_MACSEC_VLAN_PARSER_DEFAULT_INNER_TPID)
            {
                CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_qtag_set(unit, sysPort, direction, vlanParser->tpid[RTK_MACSEC_VLAN_PARSER_INNER_TPID]));
            }
            /* Disable parse q-in-q and 802.1s tag */
            CHK_FUN_CONTINUE(retCode, dv1_drv_secy_vp_control_set(unit, sysPort, direction, false, false));
        }
        else
        {
            retCode = RT_ERR_OUT_OF_RANGE;
        }
    } while(false);

    return retCode;
}

/** @} */ /* end of MACSEC_CLASSIFICATION_ASIC */

/** @} */ /* end of MACSEC_CLASSIFICATION */

/** @} */ /* end of RTK_MODULE_MACSEC */
