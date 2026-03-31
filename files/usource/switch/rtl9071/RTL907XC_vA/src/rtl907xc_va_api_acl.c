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
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtkas_api_acl_ext.h"
#include "rtl907xc_va_asicdrv_acl.h"

const uint8 cva_g_aclUserPort2sysPort[CVA_RTL9075_INTERFACE_NUM] =
    {0xFFu, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 0xFFu, 0xFFu, 0xFFu, 15u};

/**
 * @addtogroup RTK_MODULE_ACL ACL
 * @{
 */
/**
 * @addtogroup ACL_ENABLE ACL Enable & Lookup Miss Default Action
 * @{
 */
/**
 * @addtogroup ACL_ENABLE_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ingress ACL enable control
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portNum Port number (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                     The physical port number of Switch
 * @param[out] pAclEn  Ingress ACL enable (N/A) \n
 *                     Enable/Disable the ingress ACL lookup for the physical port
 * @param[out] pDefAct The default action of ingress ACL lookup miss (N/A) \n
 *                     When the ACL function on a port is enabled and a received packet is ACL lookup miss, the switch will execute the default action
 * @retval RT_ERR_OK Get the ingress ACL enable control successfully
 * @retval -RT_ERR_PORT_ID The port ID is out of range
 * @retval -RT_ERR_NULL_POINTER The "pAclEn" or "pDefAct" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_enable_ctrl_get(uint32 unit, RtkPort portNum, AclEnable *pAclEn, AclDefAct *pDefAct)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if((portNum > (RtkPort)CVA_RTK_USER_PORT_END) || (portNum < (RtkPort)CVA_RTK_USER_PORT_1ST))
        {
            ret = -(RT_ERR_PORT_ID);
            continue;
        }

        if((NULL == pAclEn) || (NULL == pDefAct))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the ACL status for each port */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_PORT0_IACL_LOOKUP_CONTROL + ((uint32)cva_g_userPort2sysPort[portNum] * PERPORT_REG_OFFSET), CVA_PORT0_IACL_LOOKUP_CONTROL_IACL_INACLLUEN_LSP, CVA_PORT0_IACL_LOOKUP_CONTROL_IACL_INACLLUEN_LEN, &fieldVal));
        *pAclEn = fieldVal;

        /* Retrieve the ACL enable control for each port */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_PORT0_IACL_LOOKUP_CONTROL + ((uint32)cva_g_userPort2sysPort[portNum] * PERPORT_REG_OFFSET), CVA_PORT0_IACL_LOOKUP_CONTROL_IACL_DEFACT_LSP, CVA_PORT0_IACL_LOOKUP_CONTROL_IACL_DEFACT_LEN, &fieldVal));
        *pDefAct = fieldVal;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ingress ACL enable control
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portNum Port number (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                     The physical port number of Switch
 * @param[in]  aclEn   Ingress ACL enable (CVA_ACL_DISABLE to CVA_ACL_ENABLE) \n
 *                     Enable/Disable the ingress ACL lookup for the physical port
 * @param[in]  defAct  The default action of ingress ACL lookup miss (CVA_ACL_DEF_ACT_PERMIT to CVA_ACL_DEF_ACT_DROP) \n
 *                     When the ACL function on a port is enabled and a received packet is ACL lookup miss, the switch will execute the default action
 * @param[out] None
 * @retval RT_ERR_OK Set the ingress ACL enable control successfully
 * @retval -RT_ERR_PORT_ID The port ID is out of range
 * @retval -RT_ERR_ACL_ENABLE The "enable" is out of range
 * @retval -RT_ERR_ACL_DEF_ACT The "defAct" is out of range
 * @retval Others Please refer to cva_drv_indirect_reg_field_set(), cva_drv_indirect_sram_1byte_set()
 */
RtkApiRet cva_acl_enable_ctrl_set(uint32 unit, RtkPort portNum, AclEnable aclEn, AclDefAct defAct)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if((portNum > (RtkPort)CVA_RTK_USER_PORT_END) || (portNum < (RtkPort)CVA_RTK_USER_PORT_1ST))
        {
            ret = -(RT_ERR_PORT_ID);
            continue;
        }
        if(aclEn > CVA_ACL_ENABLE)
        {
            ret = -(RT_ERR_ACL_ENABLE);
            continue;
        }
        if(defAct > CVA_ACL_DEF_ACT_DROP)
        {
            ret = -(RT_ERR_ACL_DEF_ACT);
            continue;
        }

        /* Enable/Disable the ingress ACL lookup and configure the default action */
        CHK_FUN_CONTINUE(ret, cva_drv_indirect_sram_1byte_set(unit, CVA_ACL_SRAM_ACT_EN, ((uint8)(defAct & 0x1u) << 1u) | (uint8)(aclEn & 0x1u)));
        CHK_FUN_CONTINUE(ret, cva_drv_indirect_reg_field_set(unit, CVA_PORT0_IACL_LOOKUP_CONTROL + ((uint32)cva_g_userPort2sysPort[portNum] * PERPORT_REG_OFFSET), 0u, 32u, ((uint32)defAct << 1u) | (uint32)aclEn));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*ACL_ENABLE_ASIC*/
/**@}*/ /*ACL_ENABLE*/

/**
 * @addtogroup TEMPLATE_GENERATOR Template Generator
 * @{
 */
/**
 * @addtogroup TEMPLATE_GENERATOR_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ingress ACL template generator configuration
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  tpIdx    Template index of ACL (0 to CVA_ACL_TEMPLATE_BOUND) \n
 *                      The template generator index
 * @param[in]  fieldIdx Field index (0 to CVA_ACL_TEMPLATE_FIELD_BOUND) \n
 *                      The field index of template generator
 * @param[out] pType    Filter key type (N/A) \n
 *                      The filter key type of template generator field
 * @retval RT_ERR_OK Get the ingress ACL template successfully
 * @retval -RT_ERR_ACL_TGIDX The ACL template index is out of range
 * @retval Others Please refer to cva_drv_acl_template_getting()
 */
RtkApiRet cva_acl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType)
{

    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > CVA_ACL_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        CHK_FUN_CONTINUE(ret, cva_drv_acl_template_getting(unit, tpIdx, fieldIdx, pType));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ingress ACL template generator configuration
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  tpIdx    Template index of ACL (0 to CVA_ACL_TEMPLATE_BOUND) \n
 *                      The template generator index
 * @param[in]  fieldIdx Field index (0 to CVA_ACL_TEMPLATE_FIELD_BOUND) \n
 *                      The field index of template generator
 * @param[in]  type     Filter key type (CVA_IACL_FLD_LBOUND to CVA_IACL_FLD_HBOUND) \n
 *                      The filter key type of template generator field
 * @param[out] None
 * @retval RT_ERR_OK Set the ingress ACL template successfully
 * @retval -RT_ERR_ACL_TGIDX The ACL template index is out of range
 * @retval Others Please refer to cva_drv_acl_template_setting()
 */
RtkApiRet cva_acl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > CVA_ACL_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        CHK_FUN_CONTINUE(ret, cva_drv_acl_template_setting(unit, tpIdx, fieldIdx, type));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*TEMPLATE_GENERATOR_ASIC*/
/**@}*/ /*TEMPLATE_GENERATOR*/

/**
 * @addtogroup PACKET_INSPECTION_ENGINE Packet Inspection Engine
 * @{
 */
/**
 * @addtogroup PACKET_INSPECTION_ENGINE_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL PIE entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[out] pPieParam    ACL PIE param (N/A) \n
 *                          The parameters of ACL PIE table
 * @retval RT_ERR_OK Get the ACL PIE entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval Others Please refer to cva_drv_acl_getting_pie_entry()
 */
RtkApiRet cva_acl_getting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        /* Retrieve the entry from the ACL PIE table */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_getting_pie_entry(unit, aclRuleIndex, pPieParam));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL PIE entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[in]  pPieParam    ACL PIE param (N/A) \n
 *                          The parameters of ACL PIE table
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL PIE entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval Others Please refer to cva_drv_acl_setting_pie_entry()
 */
RtkApiRet cva_acl_setting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        /* Configure the entry in the ACL PIE table */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_setting_pie_entry(unit, aclRuleIndex, pPieParam));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL PIE priority value
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  pieBlock PIE index (0 to CVA_ACL_PIE_BLOCK_BOUND) \n
 *                      The Packet Inspection Engine block index
 * @param[out] pPriVal  PIE priority value (N/A) \n
 *                      The Packet Inspection Engine block priority
 * @retval RT_ERR_OK Get the ACL PIE priority value successfully
 * @retval -RT_ERR_ACL_BLOCKNUM the ACL PIE index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pPriVal" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_priority_pie_get(uint32 unit, AclPieBlockId pieBlock, AclPriVal *pPriVal)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(pieBlock > CVA_ACL_PIE_BLOCK_BOUND)
        {
            ret = -RT_ERR_ACL_BLOCKNUM;
            continue;
        }

        if(NULL == pPriVal)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the ACL PIE priority value */
        /* /6u: 1 register stores 6 pie priority, *4u: 4 bytes register */
        offset = CVA_PIE_BLOCK_PRIORITY_CONTROL0 + ((pieBlock / 6u) * 4u);
        /* %6u: 1 register stores 6 pie priority, *5u: 5 bit interval */
        lsp = CVA_PIE_BLOCK_PRIORITY_CONTROL0_PIEBPRI0_LSP + ((pieBlock % 6u) * 5u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, offset, lsp, CVA_PIE_BLOCK_PRIORITY_CONTROL0_PIEBPRI0_LEN, pPriVal));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL PIE priority value
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  pieBlock PIE index (0 to CVA_ACL_PIE_BLOCK_BOUND) \n
 *                      The Packet Inspection Engine block index
 * @param[in]  priVal   PIE priority value (0 to CVA_ACL_PIE_PRIORITY_BOUND) \n
 *                      The Packet Inspection Engine block priority
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL PIE priority value successfully
 * @retval -RT_ERR_ACL_BLOCKNUM the ACL PIE index is out of range
 * @retval -RT_ERR_ACL_PRIORITY_ERRPRI the ACL PIE priority value is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_priority_pie_set(uint32 unit, AclPieBlockId pieBlock, AclPriVal priVal)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(pieBlock > CVA_ACL_PIE_BLOCK_BOUND)
        {
            ret = -RT_ERR_ACL_BLOCKNUM;
            continue;
        }

        if(priVal > CVA_ACL_PIE_PRIORITY_BOUND)
        {
            ret = -RT_ERR_ACL_PRIORITY_ERRPRI;
            continue;
        }

        /* Configure the ACL PIE priority value */
        /* /6u: 1 register stores 6 pie priority, *4u: 4 bytes register */
        offset = CVA_PIE_BLOCK_PRIORITY_CONTROL0 + ((pieBlock / 6u) * 4u);
        /* %6u: 1 register stores 6 pie priority, *5u: 5 bit interval */
        lsp = CVA_PIE_BLOCK_PRIORITY_CONTROL0_PIEBPRI0_LSP + ((pieBlock % 6u) * 5u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, lsp, CVA_PIE_BLOCK_PRIORITY_CONTROL0_PIEBPRI0_LEN, priVal));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*PACKET_INSPECTION_ENGINE_ASIC*/
/**@}*/ /*PACKET_INSPECTION_ENGINE*/

/**
 * @addtogroup RESULT_REVERSE Result Reverse
 * @{
 */
/**
 * @addtogroup RESULT_REVERSE_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL result reverse value
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[out] pIsReverse   Reverse configuration (N/A) \n
 *                          The ACL result reverse configuration
 * @retval RT_ERR_OK Get the ACL result reverse value successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pIsReverse" is NULL
 * @retval Others Please refer to cva_reg_read()
 */
RtkApiRet cva_acl_reverse_getting(uint32 unit, AclRuleId aclRuleIndex, AclReverse *pIsReverse)
{
    uint32    value = 0u;
    RtkApiRet ret   = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pIsReverse)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the ACL result reverse value */
        /* *2u: 1 register stores 2 pie reverse enties, *4u: 4 bytes register */
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, CVA_PIE_RESULT_REVERSE_CONTROL0 + (uint32)(((uint32)aclRuleIndex / CVA_ACL_TEMPLATE_RULE_NUM) * 4u), &value));
        *pIsReverse = ((value >> (uint32)((uint32)aclRuleIndex % CVA_ACL_TEMPLATE_RULE_NUM)) & 0x1u);
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL result reverse value
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[in]  isReverse    Reverse configuration (CVA_ACL_NORMAL to CVA_ACL_REVERSE) \n
 *                          The ACL result reverse configuration
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL result reverse value successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_ACL_REVERSE The "isReverse" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_reverse_setting(uint32 unit, AclRuleId aclRuleIndex, AclReverse isReverse)
{
    uint32    lsp    = 0u;
    uint32    offset = 0u;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(isReverse > CVA_ACL_REVERSE)
        {
            ret = -(RT_ERR_ACL_REVERSE);
            continue;
        }

        /* Configure the ACL result reverse value */
        /* *2u: 1 register stores 2 pie reverse enties, *4u: 4 bytes register */
        offset = (uint32)(CVA_PIE_RESULT_REVERSE_CONTROL0 + (((uint32)aclRuleIndex / CVA_ACL_TEMPLATE_RULE_NUM) * 4u));
        lsp    = (uint32)(aclRuleIndex % CVA_ACL_TEMPLATE_RULE_NUM);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, lsp, CVA_PIE_RESULT_REVERSE_CONTROL0_PIERR0_0_LEN, isReverse));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*RESULT_REVERSE_ASIC*/
/**@}*/ /*RESULT_REVERSE*/

/**
 * @addtogroup RESULT_AGGREGATION Result Aggregation
 * @{
 */
/**
 * @addtogroup RESULT_AGGREGATION_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL result aggregation status
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (even-numbered TG's entry index, e.g. 0-31, 64-95, 128-159 and so on) \n
 *                          The entry index of the ACL table
 * @param[out] pAggOp       Aggregator operation code (N/A) \n
 *                          The ACL aggregator operation code
 * @retval RT_ERR_OK Get the ACL result aggregation status successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pAggOp" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_aggregator_get(uint32 unit, AclRuleId aclRuleIndex, AclAggOp *pAggOp)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if((aclRuleIndex > CVA_ACL_RULE_BOUND) || (((aclRuleIndex / CVA_ACL_TEMPLATE_RULE_NUM) % 2u) != 0u))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pAggOp)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the ACL result aggregation status */
        offset = CVA_PIE_RESULT_AGGREGATOR_CONTROL0 + ((((uint32)aclRuleIndex / CVA_ACL_TEMPLATE_RULE_NUM) + (((uint32)aclRuleIndex % CVA_ACL_TEMPLATE_RULE_NUM) / CVA_ACL_PIE_RULE_NUM)) * 4u);
        lsp    = (((uint32)aclRuleIndex % CVA_ACL_PIE_RULE_NUM) * 2u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, offset, lsp, CVA_PIE_RESULT_AGGREGATOR_CONTROL0_PIERRAR_0_0_LEN, pAggOp));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL result aggregation status
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0-31, 64-95, 128-159, 192-223, 256-287, 320-351, 384-415, 448-479) \n
 *                          The entry index of the ACL table
 * @param[in]  aggOp        Aggregator operation code (CVA_ACL_NOT_AGG to CVA_ACL_SEL_DOWN_PIE_AGG) \n
 *                          The ACL aggregator operation code
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL result aggregation status successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_ACL_AGG_OP The "aggOp" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_aggregator_set(uint32 unit, AclRuleId aclRuleIndex, AclAggOp aggOp)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if((aclRuleIndex > CVA_ACL_RULE_BOUND) || (((aclRuleIndex / CVA_ACL_TEMPLATE_RULE_NUM) % 2u) != 0u))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(aggOp > CVA_ACL_SEL_DOWN_PIE_AGG)
        {
            ret = -(RT_ERR_ACL_AGG_OP);
            continue;
        }

        /* Configure the ACL result aggregation status */
        offset = CVA_PIE_RESULT_AGGREGATOR_CONTROL0 + ((((uint32)aclRuleIndex / CVA_ACL_TEMPLATE_RULE_NUM) + (((uint32)aclRuleIndex % CVA_ACL_TEMPLATE_RULE_NUM) / CVA_ACL_PIE_RULE_NUM)) * 4u);
        lsp    = (((uint32)aclRuleIndex % CVA_ACL_PIE_RULE_NUM) * 2u);
        /* 0x2u: aggregator operation length */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, lsp, CVA_PIE_RESULT_AGGREGATOR_CONTROL0_PIERRAR_0_0_LEN, aggOp));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*RESULT_AGGREGATION_ASIC*/
/**@}*/ /*RESULT_AGGREGATION*/

/**
 * @addtogroup ACL_ACTION ACL Action
 * @{
 */
/**
 * @addtogroup ACTION_ARBITRATION_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL action entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[out] pActParam    ACL action param (N/A) \n
 *                          The ingress ACL parameters of ACL action table
 * @retval RT_ERR_OK Get the ACL action entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval Others Please refer to cva_drv_drv_memset(), cva_drv_acl_getting_action_entry()
 */
RtkApiRet cva_acl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessActParam_t *pActParam)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        /* Retrieve the entry from the ACL action table */
        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(pActParam, 0, sizeof(AclTblAccessActParam_t)));
        CHK_FUN_CONTINUE(ret, cva_drv_acl_getting_action_entry(unit, aclRuleIndex, pActParam));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL action entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[in]  pActParam    ACL action param (N/A) \n
 *                          The ingress ACL parameters of ACL action table
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL action entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval Others Please refer to cva_drv_acl_setting_action_entry()
 */
RtkApiRet cva_acl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessActParam_t *pActParam)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        /* Configure the entry in the ACL Action table */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_setting_action_entry(unit, aclRuleIndex, pActParam));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*ACTION_ARBITRATION_ASIC*/
/**@}*/ /*ACL_ACTION*/

/**
 * @addtogroup ANY_OFFSET Any Offset / User Define Field (UDF)
 * @{
 */
/**
 * @addtogroup ANY_OFFSET_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL any offset configuration
 * @param[in]  unit            Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  offsetIndex     Any offset parser index (0 to CVA_ACL_OFFSET_BOUND) \n
 *                             The ACL any offset parser index
 * @param[out] pOffsetType     Any offset parser type (N/A) \n
 *                             The ACL any offset parser type
 * @param[out] pOffsetPosition Shift position of any offset parser (N/A) \n
 *                             The Shift position of ACL any offset parser
 * @retval RT_ERR_OK Get the ACL any offset configuration successfully
 * @retval -RT_ERR_ACL_ANY_OFFSET_IDX The "offsetIndex" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pOffsetType" or "pOffsetPosition" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_offset_get(uint32 unit, AclOffsetIndex offsetIndex, AclOffsetType *pOffsetType, AclOffsetPosition *pOffsetPosition)
{
    uint32    val    = 0;
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(offsetIndex > CVA_ACL_OFFSET_BOUND)
        {
            ret = -(RT_ERR_ACL_ANY_OFFSET_IDX);
            continue;
        }

        if((NULL == pOffsetType) || (NULL == pOffsetPosition))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the ACL any offset type */
        offset = CVA_PACKET_OFFSET_MATCH_CONTROL0 + ((offsetIndex / 2u) * 4u);
        lsp    = CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_TYPE_LSP + ((offsetIndex % 2u) * 16u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, offset, lsp, CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_TYPE_LEN, &val));
        *pOffsetType = val;

        /* Retrieve the shift position of ACL any offset */
        lsp = CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_VALUE_LSP + ((offsetIndex % 2u) * 16u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, offset, lsp, CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_VALUE_LEN, &val));
        *pOffsetPosition = (uint16)(val & 0xffffu);
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL any offset configuration
 * @param[in]  unit            Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  offsetIndex     Any offset parser index (0 to CVA_ACL_OFFSET_BOUND) \n
 *                             The ACL any offset parser index
 * @param[in]  offsetType      Any offset parser type (CVA_ACL_OFFSET_TYPE_LBOUND to CVA_ACL_OFFSET_TYPE_HBOUND) \n
 *                             The ACL any offset parser type
 * @param[in]  offsetPosition  Shift position of any offset parser (0 to CVA_ACL_OFFSET_POSITION_BOUND) \n
 *                             The Shift position of ACL any offset parser
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL offset configuration successfully
 * @retval -RT_ERR_ACL_ANY_OFFSET_IDX The "offsetIndex" is out of range
 * @retval -RT_ERR_ACL_ANY_OFFSET_TYPE The "offsetType" is out of range
 * @retval -RT_ERR_ACL_ANY_OFFSET_POSITION The "offsetPosition" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_offset_set(uint32 unit, AclOffsetIndex offsetIndex, AclOffsetType offsetType, AclOffsetPosition offsetPosition)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(offsetIndex > CVA_ACL_OFFSET_BOUND)
        {
            ret = -(RT_ERR_INPUT);
            continue;
        }

        if(offsetType > CVA_ACL_OFFSET_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_ANY_OFFSET_TYPE);
            continue;
        }

        if(offsetPosition > CVA_ACL_OFFSET_POSITION_BOUND)
        {
            ret = -(RT_ERR_ACL_ANY_OFFSET_POSITION);
            continue;
        }

        /* Configure the shift position of ACL any offset */
        offset = CVA_PACKET_OFFSET_MATCH_CONTROL0 + ((offsetIndex / 2u) * 4u);
        lsp    = CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_VALUE_LSP + ((offsetIndex % 2u) * 16u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, lsp, CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_VALUE_LEN, (uint32)offsetPosition));

        /* Configure the ACL any offset type */
        lsp = CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_TYPE_LSP + ((offsetIndex % 2u) * 16u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, lsp, CVA_PACKET_OFFSET_MATCH_CONTROL0_OFFSET0_TYPE_LEN, (uint32)offsetType));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*ANY_OFFSET_ASIC*/
/**@}*/ /*ANY_OFFSET*/

/**
 * @addtogroup RANGE_CHECK Range Check
 * @{
 */
/**
 * @addtogroup RANGE_CHECK_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL Layer4 TCP/UDP port range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Layer4 port range check index (0 to CVA_FILTER_L4PORTRANGE_BOUND) \n
 *                      The range check index of Layer4 port
 * @param[out] pType    Range check types of Layer4 port (N/A) \n
 *                      The ACL range check types of Layer4 port
 * @param[out] pUpper   Layer4 port upper bound (N/A) \n
 *                      The upper bound of L4 TCP/UDP port number
 * @param[out] pLower   Layer4 port lower bound (N/A) \n
 *                      The lower bound of L4 TCP/UDP port number
 * @retval RT_ERR_OK Get the ACL Layer4 TCP/UDP port range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pUpper" or "pLower" is NULL
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_get()
 */
RtkApiRet cva_acl_l4_port_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclFilterBound *pUpper, AclFilterBound *pLower)
{
    uint32    regAddr  = 0u;
    uint32    regVal   = 0u;
    uint32    fieldVal = 0u;
    RtkApiRet ret      = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_L4PORTRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve upper and lower bound */
        regAddr = (uint32)CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0 + (entryIdx * (2u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_LOWER_BOUND_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_LOWER_BOUND_LEN, &fieldVal, regVal));
        *pLower = (AclFilterBound)fieldVal;
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_UPPER_BOUND_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_UPPER_BOUND_LEN, &fieldVal, regVal));
        *pUpper = (AclFilterBound)fieldVal;

        /* Retrieve range check type */
        regAddr = (uint32)CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1 + (entryIdx * (2u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_TYPE_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_TYPE_LEN, &fieldVal, regVal));
        *pType = (AclRangeType)fieldVal;
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_DESTPORT_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_DESTPORT_LEN, &fieldVal, regVal));
        *pType = (AclRangeType)((uint32)*pType | (fieldVal << 2u));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL Layer4 TCP/UDP port range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Layer4 port range check index (0 to CVA_FILTER_L4PORTRANGE_BOUND) \n
 *                      The range check index of Layer4 port
 * @param[in]  type     Range check types of Layer4 port (CVA_L4PORT_RANGE_TYPE_LBOUND to CVA_L4PORT_RANGE_TYPE_HBOUND) \n
 *                      The ACL range check types of Layer4 port
 * @param[in]  upper    Layer4 port upper bound (0 to CVA_L4PORT_RANGE_BOUND) \n
 *                      The upper bound of L4 TCP/UDP port number
 * @param[in]  lower    Layer4 port lower bound (0 to CVA_L4PORT_RANGE_BOUND) \n
 *                      The lower bound of L4 TCP/UDP port number
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL Layer4 TCP/UDP port range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_BOUND The "upper" or "lower" is out of range
 * @retval -RT_ERR_ACL_RANGE_ERROR The "lower" is greater than or equal to the "upper"
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_set(), cva_reg_write()
 */
RtkApiRet cva_acl_l4_port_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, AclFilterBound upper, AclFilterBound lower)
{
    uint32    regAddr = 0u;
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_L4PORTRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(type > CVA_L4PORT_RANGE_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_TYPE);
            continue;
        }

        if((upper > CVA_L4PORT_RANGE_BOUND) || (lower > CVA_L4PORT_RANGE_BOUND))
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_BOUND);
            continue;
        }

        if(upper < lower)
        {
            ret = -(RT_ERR_ACL_RANGE_ERROR);
            continue;
        }

        /* Configure upper and lower bound */
        regAddr = CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0 + (entryIdx * (2u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_LOWER_BOUND_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_LOWER_BOUND_LEN, lower, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_UPPER_BOUND_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_0_PORT_UPPER_BOUND_LEN, upper, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));

        /* Configure range check type */
        regAddr = CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1 + (entryIdx * (2u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_DESTPORT_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_DESTPORT_LEN, ((uint32)type >> 2u) & 0x1u, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_TYPE_LSP, CVA_L4PORT_RANGE_CHECKING_TABLE_ENTRY0_1_TYPE_LEN, (uint32)type & 0x3u, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL VLAN ID range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx VLAN ID range check index (0 to CVA_FILTER_VIDRANGE_BOUND) \n
 *                      The range check index of VLAN ID
 * @param[out] pType    Range check types of VLAN ID (N/A) \n
 *                      The ACL range check types of VLAN ID
 * @param[out] pUpper   VLAN ID upper bound (N/A) \n
 *                      The upper bound of VLAN ID
 * @param[out] pLower   VLAN ID lower bound (N/A) \n
 *                      The lower bound of VLAN ID
 * @retval RT_ERR_OK Get the ACL VLAN ID range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is invalid
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pUpper" or "pLower" is NULL
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_get()
 */
RtkApiRet cva_acl_vid_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclFilterBound *pUpper, AclFilterBound *pLower)
{
    uint32    regAddr  = 0u;
    uint32    regVal   = 0u;
    uint32    fieldVal = 0u;
    RtkApiRet ret      = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_VIDRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve upper and lower bound */
        regAddr = CVA_VID_RANGE_CHECKING_TABLE_ENTRY0 + (entryIdx * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_LOWER_BOUND_LSP, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_LOWER_BOUND_LEN, &fieldVal, regVal));
        *pLower = (AclFilterBound)(fieldVal);
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_UPPER_BOUND_LSP, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_UPPER_BOUND_LEN, &fieldVal, regVal));
        *pUpper = (AclFilterBound)(fieldVal);

        /* Retrieve range check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_TYPE_LSP, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_TYPE_LEN, &fieldVal, regVal));
        *pType = (AclRangeType)(fieldVal);
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL VLAN ID range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx VLAN ID range check index (0 to CVA_FILTER_VIDRANGE_BOUND) \n
 *                      The range check index of VLAN ID
 * @param[in]  type     Range check types of VLAN ID (CVA_VID_RANGE_TYPE_LBOUND to CVA_VID_RANGE_TYPE_HBOUND) \n
 *                      The ACL range check types of VLAN ID
 * @param[in]  upper    VLAN ID upper bound (0 to CVA_VID_RANGE_BOUND) \n
 *                      The upper bound of VLAN ID
 * @param[in]  lower    VLAN ID lower bound (0 to CVA_VID_RANGE_BOUND) \n
 *                      The lower bound of VLAN ID
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL Layer4 TCP/UDP port range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_BOUND The "upper" or "lower" is out of range
 * @retval -RT_ERR_ACL_RANGE_ERROR The "lower" is greater than or equal to the "upper"
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_set(), cva_reg_write()
 */
RtkApiRet cva_acl_vid_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, AclFilterBound upper, AclFilterBound lower)
{
    uint32    regAddr = 0u;
    uint32    regVal  = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_VIDRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(type > CVA_VID_RANGE_TYPE_HBOUND)
        {
            ret = -(RT_ERR_INPUT);
            continue;
        }

        if((upper > CVA_VID_RANGE_BOUND) || (lower > CVA_VID_RANGE_BOUND))
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_BOUND);
            continue;
        }

        if(upper < lower)
        {
            ret = -(RT_ERR_ACL_RANGE_ERROR);
            continue;
        }

        /* Configure upper and lower bound */
        regAddr = CVA_VID_RANGE_CHECKING_TABLE_ENTRY0 + (entryIdx * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_LOWER_BOUND_LSP, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_LOWER_BOUND_LEN, lower, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_UPPER_BOUND_LSP, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_VID_UPPER_BOUND_LEN, upper, &regVal));

        /* Configure range check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_TYPE_LSP, CVA_VID_RANGE_CHECKING_TABLE_ENTRY0_TYPE_LEN, (uint32)type, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL IPv4 range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv4 range check index (0 to CVA_FILTER_IPRANGE_BOUND) \n
 *                      The range check index of IPv4
 * @param[out] pType    Range check types of IPv4 (N/A) \n
 *                      The ACL range check types of IPv4
 * @param[out] pUpper   IPv4 upper bound (N/A) \n
 *                      The upper bound of IPv4 address
 * @param[out] pLower   Layer4 port lower bound (N/A) \n
 *                      The lower bound of IPv4 address
 * @retval RT_ERR_OK Get the ACL IPv4 range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is invalid
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pUpper" or "pLower" is NULL
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_read()
 */
RtkApiRet cva_acl_ip_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclIpv4 *pUpper, AclIpv4 *pLower)
{
    uint32    regAddr  = 0u;
    uint32    fieldVal = 0u;
    RtkApiRet ret      = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IPRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve upper and lower bound */
        regAddr = (uint32)CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_0 + (entryIdx * (3u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, pLower));

        regAddr = (uint32)CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_1 + (entryIdx * (3u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, pUpper));

        /* Retrieve range check type */
        regAddr = (uint32)CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_2 + (entryIdx * (3u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, regAddr, CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LSP, CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LEN, &fieldVal));
        *pType = (AclRangeType)fieldVal;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL IPv4 range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv4 range check index (0 to CVA_FILTER_IPRANGE_BOUND) \n
 *                      The range check index of IPv4
 * @param[in]  type     Range check types of IPv4 (CVA_IP_RANGE_TYPE_LBOUND to CVA_IP_RANGE_TYPE_HBOUND) \n
 *                      The ACL range check types of IPv4
 * @param[in]  upper    IPv4 upper bound (0 to CVA_IP_RANGE_BOUND) \n
 *                      The upper bound of IPv4 address
 * @param[in]  lower    Layer4 port lower bound (0 to CVA_IP_RANGE_BOUND) \n
 *                      The lower bound of IPv4 address
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv4 range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_ACL_RANGE_ERROR The "lower" is greater than or equal to the "upper"
 * @retval Others Please refer to cva_reg_write(), cva_reg_field_write()
 */
RtkApiRet cva_acl_ip_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, AclIpv4 upper, AclIpv4 lower)
{
    uint32    regAddr = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IPRANGE_BOUND)
        {
            ret = -(RT_ERR_INPUT);
            continue;
        }

        if(type > CVA_IP_RANGE_TYPE_HBOUND)
        {
            ret = -(RT_ERR_INPUT);
            continue;
        }

        if(upper < lower)
        {
            ret = -(RT_ERR_ACL_RANGE_ERROR);
            continue;
        }

        /* Configure upper and lower bound */
        regAddr = (uint32)CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_0 + (entryIdx * (3u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, lower));
        regAddr = CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_1 + (entryIdx * (3u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, upper));

        /* Configure range check type */
        regAddr = CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_2 + (entryIdx * (3u * 4u));
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, regAddr, CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LSP, CVA_IP_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LEN, (uint32)type));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL packet length range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Packet length range check index (0 to CVA_FILTER_PKTLENRANGE_BOUND) \n
 *                      The range check index of packet length
 * @param[out] pUpper   Packet length upper bound (N/A) \n
 *                      The upper bound of packet length
 * @param[out] pLower   Packet length lower bound (N/A) \n
 *                      The lower bound of packet length
 * @retval RT_ERR_OK Get the ACL packet length range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is invalid
 * @retval -RT_ERR_NULL_POINTER The "pUpper" or "pLower" is NULL
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_get()
 */
RtkApiRet cva_acl_pkt_len_range_get(uint32 unit, AclFilterIndex entryIdx, AclFilterBound *pUpper, AclFilterBound *pLower)
{
    uint32    regAddr  = 0u;
    RtkApiRet ret      = RT_ERR_OK;
    uint32    regVal   = 0u;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_PKTLENRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if((NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve upper and lower bound */
        regAddr = CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0 + (entryIdx * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_LOWER_BOUND_LSP, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_LOWER_BOUND_LEN, &fieldVal, regVal));
        *pLower = fieldVal;
        CHK_FUN_CONTINUE(ret, cva_reg_field_get(regAddr, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_UPPER_BOUND_LSP, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_UPPER_BOUND_LEN, &fieldVal, regVal));
        *pUpper = fieldVal;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL packet length range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Packet length range check index (0 to CVA_FILTER_PKTLENRANGE_BOUND) \n
 *                      The range check index of packet length
 * @param[in]  upper    Packet length upper bound (0 to CVA_PKTLEN_RANGE_BOUND) \n
 *                      The upper bound of packet length
 * @param[in]  lower    Packet length lower bound (0 to CVA_PKTLEN_RANGE_BOUND) \n
 *                      The lower bound of packet length
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL packet length range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_BOUND The "upper" or "lower" is out of range
 * @retval -RT_ERR_ACL_RANGE_ERROR The "lower" is greater than or equal to the "upper"
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_set(), cva_reg_write()
 */
RtkApiRet cva_acl_pkt_len_range_set(uint32 unit, AclFilterIndex entryIdx, AclFilterBound upper, AclFilterBound lower)
{
    uint32    regAddr = 0u;
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_PKTLENRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if((upper > CVA_PKTLEN_RANGE_BOUND) || (lower > CVA_PKTLEN_RANGE_BOUND))
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_BOUND);
            continue;
        }

        if(upper < lower)
        {
            ret = -(RT_ERR_ACL_RANGE_ERROR);
            continue;
        }

        /* Configure upper and lower bound */
        regAddr = CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0 + (entryIdx * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, regAddr, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_LOWER_BOUND_LSP, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_LOWER_BOUND_LEN, lower, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(regAddr, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_UPPER_BOUND_LSP, CVA_PACKET_LENGTH_RANGE_CHECKING_TABLE_ENTRY0_LENGTH_UPPER_BOUND_LEN, upper, &regVal));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, regAddr, regVal));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL IPv6 range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv6 range check index (0 to CVA_FILTER_IP6RANGE_BOUND) \n
 *                      The range check index of IPv6
 * @param[out] pType    Range check types of IPv6 (N/A) \n
 *                      The ACL range check types of IPv6
 * @param[out] pUpper   IPv6 upper bound (N/A) \n
 *                      The upper bound of IPv6 address
 * @param[out] pLower   IPv6 lower bound (N/A) \n
 *                      The lower bound of IPv6 address
 * @retval RT_ERR_OK Get the ACL IPv6 range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is invalid
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pUpper" or "pLower" is NULL
 * @retval Others Please refer to cva_drv_acl_ip6_range_get_boundary(), cva_reg_field_read()
 */
RtkApiRet cva_acl_ip6_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclIpv6 *pUpper, AclIpv6 *pLower)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IP6RANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve upper and lower bound */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_ip6_range_get_boundary(unit, entryIdx, CVA_ACL_IP6_RANGE_LBOUND_TYPE, pLower));
        CHK_FUN_CONTINUE(ret, cva_drv_acl_ip6_range_get_boundary(unit, entryIdx, CVA_ACL_IP6_RANGE_HBOUND_TYPE, pUpper));
        /* Retrieve range check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_2 + (entryIdx * 36u), CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LSP, CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LEN, &fieldVal));
        *pType = fieldVal;

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL IPv6 range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv6 range check index (0 to CVA_FILTER_IP6RANGE_BOUND) \n
 *                      The range check index of IPv6
 * @param[in]  type     Range check types of IPv6 (CVA_IP6_RANGE_TYPE_LBOUND to CVA_IP6_RANGE_TYPE_HBOUND) \n
 *                      The ACL range check types of IPv6
 * @param[in]  pUpper   IPv6 upper bound (N/A) \n
 *                      The upper bound of IPv6 address
 * @param[in]  pLower   IPv6 lower bound (N/A) \n
 *                      The lower bound of IPv6 address
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv6 range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pUpper" or "pLower" is NULL
 * @retval Others Please refer to cva_drv_acl_ip6_range_check(), cva_drv_acl_ip6_range_set_boundary(), cva_reg_field_write()
 */
RtkApiRet cva_acl_ip6_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, const AclIpv6 *pUpper, const AclIpv6 *pLower)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IP6RANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(type > CVA_IP6_RANGE_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_TYPE);
            continue;
        }

        if((NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Check if the inputed IPv6 param are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_ip6_range_check(pUpper, pLower));

        /* Configure upper and lower bound */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_ip6_range_set_boundary(unit, entryIdx, CVA_ACL_IP6_RANGE_LBOUND_TYPE, pLower));
        CHK_FUN_CONTINUE(ret, cva_drv_acl_ip6_range_set_boundary(unit, entryIdx, CVA_ACL_IP6_RANGE_HBOUND_TYPE, pUpper));
        /* Configure range check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_2 + (entryIdx * 36u), CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LSP, CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LEN, (uint32)type));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL MAC address range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx MAC address range check index (0 to CVA_FILTER_MACRANGE_BOUND) \n
 *                      The range check index of MAC address
 * @param[out] pType    Range check types of MAC address (N/A) \n
 *                      The ACL range check types of MAC address
 * @param[out] pUpper   MAC address upper bound (N/A) \n
 *                      The upper bound of MAC address
 * @param[out] pLower   MAC address lower bound (N/A) \n
 *                      The lower bound of MAC address
 * @retval RT_ERR_OK Get the ACL MAC address range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is invalid
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pUpper" or "pLower" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_mac_range_get(uint32 unit, AclFilterIndex entryIdx, AclRangeType *pType, AclMac *pUpper, AclMac *pLower)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_MACRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve upper and lower bound */
        /*
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_0 <-> mac[2]  to mac[5]
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_1 <-> mac[0]  to mac[1]
        */
        /*
            MAC address: 00:11:22:33:44:55
            mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_0: 0x22334455
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_1: 0x0011
        */
        /* *5u: 5 register intervals, *4u: 4 bytes register */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_0 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_0_MAC_LOWER_BOUND_0_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_0_MAC_LOWER_BOUND_0_LEN, &fieldVal));
        pLower[5] = (uint8)(fieldVal & 0xffu);
        pLower[4] = (uint8)((fieldVal & 0xff00u) >> 8u);
        pLower[3] = (uint8)((fieldVal & 0xff0000u) >> 16u);
        pLower[2] = (uint8)((fieldVal & 0xff000000u) >> 24u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_1 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_1_MAC_LOWER_BOUND_1_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_1_MAC_LOWER_BOUND_1_LEN, &fieldVal));
        pLower[1] = (uint8)(fieldVal & 0xffu);
        pLower[0] = (uint8)((fieldVal & 0xff00u) >> 8u);

        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_0 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_0_MAC_UPPER_BOUND_0_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_0_MAC_UPPER_BOUND_0_LEN, &fieldVal));
        pUpper[5] = (uint8)(fieldVal & 0xffu);
        pUpper[4] = (uint8)((fieldVal & 0xff00u) >> 8u);
        pUpper[3] = (uint8)((fieldVal & 0xff0000u) >> 16u);
        pUpper[2] = (uint8)((fieldVal & 0xff000000u) >> 24u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_1 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_1_MAC_UPPER_BOUND_1_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_1_MAC_UPPER_BOUND_1_LEN, &fieldVal));
        pUpper[1] = (uint8)(fieldVal & 0xffu);
        pUpper[0] = (uint8)((fieldVal & 0xff00u) >> 8u);

        /* Retrieve range check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_2 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LEN, &fieldVal));
        *pType = fieldVal;

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL MAC address range
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx MAC address range check index (0 to CVA_FILTER_MACRANGE_BOUND) \n
 *                      The range check index of MAC address
 * @param[in]  type     Range check types of MAC address (CVA_MAC_RANGE_TYPE_LBOUND to CVA_MAC_RANGE_TYPE_HBOUND) \n
 *                      The ACL range check types of MAC address
 * @param[in]  pUpper   MAC address upper bound (N/A) \n
 *                      The upper bound of MAC address
 * @param[in]  pLower   MAC address lower bound (N/A) \n
 *                      The lower bound of MAC address
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL MAC address range successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_RANGE_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pUpper" or "pLower" is NULL
 * @retval -RT_ERR_ACL_RANGE_ERROR The "lower" is greater than or equal to the "upper"
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_mac_range_set(uint32 unit, AclFilterIndex entryIdx, AclRangeType type, const AclMac *pUpper, const AclMac *pLower)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;
    uint64    macUpper = 0u;
    uint64    macLower = 0u;
    uint8     i        = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_MACRANGE_BOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(type > CVA_MAC_RANGE_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_RANGE_CHK_TYPE);
            continue;
        }

        if((NULL == pUpper) || (NULL == pLower))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve MAC upper/lower value from MAC struct */
        for(i = 0; i < MAC_ADDR_LEN; i++)
        {
            macUpper |= ((uint64)pUpper[i] << (40u - (i * 8u)));
            macLower |= ((uint64)pLower[i] << (40u - (i * 8u)));
        }

        if(macUpper <= macLower)
        {
            ret = -(RT_ERR_ACL_RANGE_ERROR);
            continue;
        }

        /* Configure upper and lower bound */
        /*
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_0 <-> mac[2]  to mac[5]
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_1 <-> mac[0]  to mac[1]
        */
        /*
            MAC address: 00:11:22:33:44:55
            mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_0: 0x22334455
            CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_X_1: 0x0011
        */
        /* *5u: 5 register intervals, *4u: 4 bytes register */
        fieldVal = (uint32)(macLower & 0xffffffffu);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_0 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_0_MAC_LOWER_BOUND_0_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_0_MAC_LOWER_BOUND_0_LEN, fieldVal));
        fieldVal = (uint32)(macLower >> 32u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_1 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_1_MAC_LOWER_BOUND_1_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_0_1_MAC_LOWER_BOUND_1_LEN, fieldVal));

        fieldVal = (uint32)(macUpper & 0xffffffffu);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_0 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_0_MAC_UPPER_BOUND_0_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_0_MAC_UPPER_BOUND_0_LEN, fieldVal));
        fieldVal = (uint32)(macUpper >> 32u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_1 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_1_MAC_UPPER_BOUND_1_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_1_1_MAC_UPPER_BOUND_1_LEN, fieldVal));

        /* Retrieve range check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_2 + (entryIdx * 5u * 4u), CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LSP, CVA_MAC_RANGE_CHECKING_TABLE_ENTRY0_2_TYPE_LEN, (uint32)type));

    } while(0u == 1u);
    return ret;
}
/**@}*/ /*RANGE_CHECK_ASIC*/
/**@}*/ /*RANGE_CHECK*/

/**
 * @addtogroup GROUP_MEMBER_CHECK Group Member Check
 * @{
 */
/**
 * @addtogroup GROUP_MEMBER_CHECK_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL Layer4 TCP/UDP port member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Layer4 port member check index (0 to CVA_FILTER_L4PORTMEMBER_BOUND) \n
 *                      The member check index of Layer4 port
 * @param[out] pType    Member check types of Layer4 port (N/A) \n
 *                      The ACL member check types of Layer4 port
 * @param[out] pMember  Layer4 port member (N/A) \n
 *                      L4 TCP/UDP port number member
 * @retval RT_ERR_OK Get the ACL Layer4 TCP/UDP port member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pMember" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_l4_port_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclMember *pMember)
{
    uint32    val    = 0u;
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_L4PORTMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pMember))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve member check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_L4PORT_TABLE_CONTROL, CVA_L4PORT_TABLE_CONTROL_L4PORT_SEL_LSP, CVA_L4PORT_TABLE_CONTROL_L4PORT_SEL_LEN, &val));
        *pType = (AclMemberType)((val >> entryIdx) & 0x1u);

        /* Retrieve member value */
        if((entryIdx % 2u) == 0u)
        {
            lsp = CVA_L4PORT_TABLE_0_L4PORT0_LSP;
        }
        else
        {
            lsp = CVA_L4PORT_TABLE_0_L4PORT1_LSP;
        }
        offset = CVA_L4PORT_TABLE_0 + ((entryIdx / 2u) * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, offset, lsp, CVA_L4PORT_TABLE_0_L4PORT0_LEN, &val));
        *pMember = (AclMember)val;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL Layer4 TCP/UDP port member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Layer4 port member check index (0 to CVA_FILTER_L4PORTMEMBER_BOUND) \n
 *                      The member check index of Layer4 port
 * @param[in]  type     Member check types of Layer4 port (CVA_L4PORT_MEMBER_TYPE_LBOUND to CVA_L4PORT_MEMBER_TYPE_HBOUND) \n
 *                      The ACL member check types of Layer4 port
 * @param[in]  member   Layer4 port member (0 to CVA_L4PORT_MEMBER_BOUND) \n
 *                      L4 TCP/UDP port number member
 * @param[out] None
 * @retval RT_ERR_OK Get the ACL Layer4 TCP/UDP port member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_BOUND The "member" is out of range
 * @retval Others Please refer to cva_reg_field_read(), cva_reg_field_write()
 */
RtkApiRet cva_acl_l4_port_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, AclMember member)
{
    uint32    val    = 0u;
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_L4PORTMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if(type > CVA_L4PORT_MEMBER_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_TYPE);
            continue;
        }

        if(member > CVA_L4PORT_MEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_BOUND);
            continue;
        }

        /* Configure member check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_L4PORT_TABLE_CONTROL, CVA_L4PORT_TABLE_CONTROL_L4PORT_SEL_LSP, CVA_L4PORT_TABLE_CONTROL_L4PORT_SEL_LEN, &val));
        val &= ~((uint32)0x1u << entryIdx);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_L4PORT_TABLE_CONTROL, CVA_L4PORT_TABLE_CONTROL_L4PORT_SEL_LSP, CVA_L4PORT_TABLE_CONTROL_L4PORT_SEL_LEN, val | ((uint32)type << entryIdx)));

        /* Configure member value */
        if((entryIdx % 2u) == 0u)
        {
            lsp = CVA_L4PORT_TABLE_0_L4PORT0_LSP;
        }
        else
        {
            lsp = CVA_L4PORT_TABLE_0_L4PORT1_LSP;
        }
        offset = CVA_L4PORT_TABLE_0 + ((entryIdx / 2u) * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, lsp, CVA_L4PORT_TABLE_0_L4PORT0_LEN, member));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL VLAN ID member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx VLAN ID member check index (0 to CVA_FILTER_VIDMEMBER_BOUND) \n
 *                      The member check index of VLAN ID
 * @param[out] pType    Member check types of VLAN ID (N/A) \n
 *                      The ACL member check types of VLAN ID
 * @param[out] pMember  VLAN ID member (N/A) \n
 *                      The VLAN ID member
 * @retval RT_ERR_OK Get the ACL VLAN ID member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pMember" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_vid_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclMember *pMember)
{
    uint32    val    = 0u;
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_VIDMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pMember))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve member check type */
        lsp = CVA_VLAN_ID_TABLE_CONTROL_VLAN_TYPE0_LSP + (entryIdx * 2u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_VLAN_ID_TABLE_CONTROL, lsp, CVA_VLAN_ID_TABLE_CONTROL_VLAN_TYPE0_LEN, &val));
        *pType = val;

        /* Retrieve member value */
        if((entryIdx % 2u) == 0u)
        {
            lsp = CVA_VLAN_ID_TABLE_0_VLAN_ID_0_LSP;
        }
        else
        {
            lsp = CVA_VLAN_ID_TABLE_0_VLAN_ID_1_LSP;
        }
        offset = CVA_VLAN_ID_TABLE_0 + ((entryIdx / 2u) * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, offset, lsp, CVA_VLAN_ID_TABLE_0_VLAN_ID_0_LEN, &val));
        *pMember = val;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL VLAN ID member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx VLAN ID member check index (0 to CVA_FILTER_VIDMEMBER_BOUND) \n
 *                      The member check index of VLAN ID
 * @param[in]  type     Member check types of VLAN ID (CVA_VID_MEMBER_TYPE_LBOUND to CVA_VID_MEMBER_TYPE_HBOUND) \n
 *                      The ACL member check types of VLAN ID
 * @param[in]  member   VLAN ID member (0 to CVA_VID_MEMBER_BOUND) \n
 *                      The VLAN ID member
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL VLAN ID member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_BOUND The "member" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_vid_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, AclMember member)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_VIDMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if(type > CVA_VID_MEMBER_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_TYPE);
            continue;
        }

        if(member > CVA_VID_MEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_BOUND);
            continue;
        }

        /* Configure member check type */
        lsp = CVA_VLAN_ID_TABLE_CONTROL_VLAN_TYPE0_LSP + (entryIdx * 2u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_VLAN_ID_TABLE_CONTROL, lsp, CVA_VLAN_ID_TABLE_CONTROL_VLAN_TYPE0_LEN, (uint32)type));

        /* Configure member value */
        if((entryIdx % 2u) == 0u)
        {
            lsp = CVA_VLAN_ID_TABLE_0_VLAN_ID_0_LSP;
        }
        else
        {
            lsp = CVA_VLAN_ID_TABLE_0_VLAN_ID_1_LSP;
        }
        offset = CVA_VLAN_ID_TABLE_0 + ((entryIdx / 2u) * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, lsp, CVA_VLAN_ID_TABLE_0_VLAN_ID_0_LEN, member));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL source port mask
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Source port mask member check index (0 to CVA_FILTER_SRCPORTMASK_BOUND) \n
 *                      The member check index of source port mask
 * @param[out] pSpm     Source port mask (N/A) \n
 *                      The Source port mask member
 * @retval RT_ERR_OK Get the ACL source port mask successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is invalid
 * @retval -RT_ERR_NULL_POINTER The "pSpm" is NULL
 * @retval Others Please refer to cva_reg_read(), cva_drv_pmap_convert()
 */
RtkApiRet cva_acl_spm_member_get(uint32 unit, AclFilterIndex entryIdx, RtkPmap *pSpm)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_SRCPORTMASK_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if(NULL == pSpm)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve source port mask range */
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, (uint32)CVA_SOURCE_PORT_MASK_TABLE_ENTRY0 + (entryIdx * 4u), &srcPmap));
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert((RtkPmap)srcPmap, pSpm, CVA_EM_SYS_TO_USER));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL source port mask
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx Source port mask index (0 to CVA_FILTER_SRCPORTMASK_BOUND) \n
 *                      The member check index of source port mask
 * @param[in]  spm      Source port mask (CVA_RTK_USER_PMAP_1ST to CVA_RTK_USER_PMAP_ALL) \n
 *                      The Source port mask member
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL source port mask successfully
 * @retval -RT_ERR_ACL_RANGE_CHK_IDX The "entryIdx" is invalid
 * @retval Others Please refer to cva_drv_pmap_convert(), cva_reg_write()
 */
RtkApiRet cva_acl_spm_member_set(uint32 unit, AclFilterIndex entryIdx, RtkPmap spm)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPmap   dstPmap = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_SRCPORTMASK_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        /* Configure source port mask range */
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(spm, &dstPmap, CVA_EM_USER_TO_SYS));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, (uint32)CVA_SOURCE_PORT_MASK_TABLE_ENTRY0 + (entryIdx * 4u), (uint32)dstPmap));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL IPv4 member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv4 member check index (0 to CVA_FILTER_IPMEMBER_BOUND) \n
 *                      The member check index of IPv4
 * @param[out] pType    Member check types of IPv4 (N/A) \n
 *                      The ACL member check types of IPv4
 * @param[out] pMember  IPv4 member (N/A) \n
 *                      The IPv4 address member
 * @retval RT_ERR_OK Get the ACL IPv4 member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pMember" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_ip_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclIpv4 *pMember)
{
    uint32    val = 0;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IPMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pMember))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve member check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_IPV4_MASK_TABLE_CONTROL, CVA_IPV4_MASK_TABLE_CONTROL_IPV4_MASK_SEL_0_LSP + entryIdx, CVA_IPV4_MASK_TABLE_CONTROL_IPV4_MASK_SEL_0_LEN, &val));
        *pType = val;

        /* Retrieve member value */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_IPV4_MASK_TABLE_0 + (entryIdx * 4u), CVA_IPV4_MASK_TABLE_0_IPV4_MASK_0_LSP, CVA_IPV4_MASK_TABLE_0_IPV4_MASK_0_LEN, &val));
        *pMember = val;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL IPv4 member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv4 member check index (0 to CVA_FILTER_IPMEMBER_BOUND) \n
 *                      The member check index of IPv4
 * @param[in]  type     Member check types of IPv4 (CVA_IP_MEMBER_TYPE_LBOUND to CVA_IP_MEMBER_TYPE_HBOUND) \n
 *                      The ACL member check types of IPv4
 * @param[in]  member   IPv4 member (0 to CVA_IP_MEMBER_BOUND) \n
 *                      The IPv4 address member
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv4 member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_TYPE The "type" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_ip_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, AclIpv4 member)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    lsp    = 0u;
    uint32    offset = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IPMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if(type > CVA_IP_MEMBER_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_TYPE);
            continue;
        }

        /* Configure member check type */
        lsp = CVA_IPV4_MASK_TABLE_CONTROL_IPV4_MASK_SEL_0_LSP + entryIdx;
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_IPV4_MASK_TABLE_CONTROL, lsp, CVA_IPV4_MASK_TABLE_CONTROL_IPV4_MASK_SEL_0_LEN, (uint32)type));

        /* Configure member value */
        offset = CVA_IPV4_MASK_TABLE_0 + (entryIdx * 4u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, offset, CVA_IPV4_MASK_TABLE_0_IPV4_MASK_0_LSP, CVA_IPV4_MASK_TABLE_0_IPV4_MASK_0_LEN, member));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL IPv6 member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv6 member check index (0 to CVA_FILTER_IP6MEMBER_BOUND) \n
 *                      The member check index of IPv6
 * @param[out] pType    Member check types of IPv6 (N/A) \n
 *                      The ACL member check types of IPv6
 * @param[out] pMember  IPv6 member (N/A) \n
 *                      The IPv6 address member
 * @retval RT_ERR_OK Get the ACL IPv6 member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pMember" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_ip6_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclIpv6 *pMember)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    lsp         = 0u;
    uint32    regAddr     = 0u;
    uint32    fieldVal    = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IP6MEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pMember))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve member check type */
        lsp = CVA_IPV6_MASK_TABLE_CONTROL_IPV6_MASK_SEL_0_LSP + entryIdx;
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_IPV6_MASK_TABLE_CONTROL, lsp, CVA_IPV6_MASK_TABLE_CONTROL_IPV6_MASK_SEL_0_LEN, (uint32 *)pType));

        /* Retrieve member value */
        /*
            CVA_IPV6_MASK_TABLE_0_3 <-> ipv6[0]  to ipv6[3]
            CVA_IPV6_MASK_TABLE_0_2 <-> ipv6[4]  to ipv6[7]
            CVA_IPV6_MASK_TABLE_0_1 <-> ipv6[8]  to ipv6[11]
            CVA_IPV6_MASK_TABLE_0_0 <-> ipv6[12] to ipv6[15]
        */
        regAddr = CVA_IPV6_MASK_TABLE_0_3;
        for(i = 0; i <= rangeRegNum; i++)
        {
            /*
                IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
                ipv6[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
                CVA_IPV6_MASK_TABLE_0_3: 0x00112233
                CVA_IPV6_MASK_TABLE_0_2: 0x44556677
                CVA_IPV6_MASK_TABLE_0_1: 0x8899AABB
                CVA_IPV6_MASK_TABLE_0_0: 0xCCDDEEFF
            */
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, regAddr - (i * 4u) + (entryIdx * 16u), CVA_IPV6_MASK_TABLE_0_0_IPV6_MASK_0_0_LSP, CVA_IPV6_MASK_TABLE_0_0_IPV6_MASK_0_0_LEN, &fieldVal));
            pMember[i * 4u]        = (uint8)((fieldVal & 0xff000000u) >> 24u);
            pMember[(i * 4u) + 1u] = (uint8)((fieldVal & 0xff0000u) >> 16u);
            pMember[(i * 4u) + 2u] = (uint8)((fieldVal & 0xff00u) >> 8u);
            pMember[(i * 4u) + 3u] = (uint8)((fieldVal & 0xffu));
            ;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL IPv6 member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx IPv6 member check index (0 to CVA_FILTER_IP6MEMBER_BOUND) \n
 *                      The member check index of IPv6
 * @param[in]  type     Member check types of IPv6 (CVA_IP_MEMBER_TYPE_LBOUND to CVA_IP_MEMBER_TYPE_HBOUND) \n
 *                      The ACL member check types of IPv6
 * @param[in]  pMember  IPv6 member (N/A) \n
 *                      The IPv6 address member
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL IPv6 member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_BOUND The "member" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_ip6_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, const AclIpv6 *pMember)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    lsp         = 0u;
    uint32    regAddr     = 0u;
    uint32    fieldVal    = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_IP6MEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if(type > CVA_IP_MEMBER_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_TYPE);
            continue;
        }

        if(NULL == pMember)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Configure member check type */
        lsp = CVA_IPV6_MASK_TABLE_CONTROL_IPV6_MASK_SEL_0_LSP + entryIdx;
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_IPV6_MASK_TABLE_CONTROL, lsp, CVA_IPV6_MASK_TABLE_CONTROL_IPV6_MASK_SEL_0_LEN, (uint32)type));

        /* Configure member value */
        /*
            CVA_IPV6_MASK_TABLE_0_3 <-> ipv6[0]  to ipv6[3]
            CVA_IPV6_MASK_TABLE_0_2 <-> ipv6[4]  to ipv6[7]
            CVA_IPV6_MASK_TABLE_0_1 <-> ipv6[8]  to ipv6[11]
            CVA_IPV6_MASK_TABLE_0_0 <-> ipv6[12] to ipv6[15]
        */
        regAddr = CVA_IPV6_MASK_TABLE_0_3;
        for(i = 0; i <= rangeRegNum; i++)
        {
            /*
                IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
                ipv6[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
                CVA_IPV6_MASK_TABLE_0_3: 0x00112233
                CVA_IPV6_MASK_TABLE_0_2: 0x44556677
                CVA_IPV6_MASK_TABLE_0_1: 0x8899AABB
                CVA_IPV6_MASK_TABLE_0_0: 0xCCDDEEFF
            */
            fieldVal = (((uint32)pMember[i * 4u]) << 24u) | (((uint32)pMember[(i * 4u) + 1u]) << 16u) | (((uint32)pMember[(i * 4u) + 2u]) << 8u) | (((uint32)pMember[(i * 4u) + 3u]));
            CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, regAddr - (i * 4u) + (entryIdx * 16u), CVA_IPV6_MASK_TABLE_0_0_IPV6_MASK_0_0_LSP, CVA_IPV6_MASK_TABLE_0_0_IPV6_MASK_0_0_LEN, fieldVal));
        }

    } while(0u == 1u);
    return ret;
}

/**
 * @brief Get the ACL MAC address member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx MAC address member check index (0 to CVA_FILTER_MACMEMBER_BOUND) \n
 *                      The member check index of MAC address
 * @param[out] pType    Member Member check types of MAC address (N/A) \n
 *                      The ACL member check types of MAC address
 * @param[out] pMember  MAC address member (N/A) \n
 *                      The MAC address member
 * @retval RT_ERR_OK Get the ACL MAC address member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_NULL_POINTER The "pType" or "pMember" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_mac_member_get(uint32 unit, AclFilterIndex entryIdx, AclMemberType *pType, AclMac *pMember)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_MACMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if((NULL == pType) || (NULL == pMember))
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve member check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_MASK_TABLE_CONTROL, CVA_MAC_MASK_TABLE_CONTROL_MAC_MASK_SEL_0_LSP + entryIdx, CVA_MAC_MASK_TABLE_CONTROL_MAC_MASK_SEL_0_LEN, &fieldVal));
        *pType = fieldVal;

        /* Retrieve member value */
        /*
            CVA_MAC_MASK_TABLE_0_0 <-> mac[2]  to mac[5]
            CVA_MAC_MASK_TABLE_0_1 <-> mac[0]  to mac[1]
        */
        /*
            MAC address: 00:11:22:33:44:55
            mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
            CVA_MAC_MASK_TABLE_0_0: 0x22334455
            CVA_MAC_MASK_TABLE_0_1: 0x0011
        */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_MASK_TABLE_0_0 + (entryIdx * 8u), CVA_MAC_MASK_TABLE_0_0_MAC_MASK_0_0_LSP, CVA_MAC_MASK_TABLE_0_0_MAC_MASK_0_0_LEN, &fieldVal));
        pMember[5] = (uint8)(fieldVal & 0xffu);
        pMember[4] = (uint8)((fieldVal & 0xff00u) >> 8u);
        pMember[3] = (uint8)((fieldVal & 0xff0000u) >> 16u);
        pMember[2] = (uint8)((fieldVal & 0xff000000u) >> 24u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_MASK_TABLE_0_1 + (entryIdx * 8u), CVA_MAC_MASK_TABLE_0_1_MAC_MASK_0_1_LSP, CVA_MAC_MASK_TABLE_0_1_MAC_MASK_0_1_LEN, &fieldVal));
        pMember[1] = (uint8)(fieldVal & 0xffu);
        pMember[0] = (uint8)((fieldVal & 0xff00u) >> 8u);

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL MAC address member
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  entryIdx MAC address member check index (0 to CVA_FILTER_MACMEMBER_BOUND) \n
 *                      The member check index of MAC address
 * @param[in]  type     Member Member check types of MAC address (CVA_MAC_MEMBER_TYPE_LBOUND to CVA_MAC_MEMBER_TYPE_HBOUND) \n
 *                      The ACL member check types of MAC address
 * @param[in]  pMember  MAC address member (N/A) \n
 *                      The MAC address member
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL MAC address member successfully
 * @retval -RT_ERR_ACL_MEMBER_CHK_IDX The "entryIdx" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_TYPE The "type" is out of range
 * @retval -RT_ERR_ACL_MEMBER_CHK_BOUND The "member" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_mac_member_set(uint32 unit, AclFilterIndex entryIdx, AclMemberType type, const AclMac *pMember)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;
    uint64    mac      = 0u;
    uint8     i        = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(entryIdx > CVA_FILTER_MACMEMBER_BOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_IDX);
            continue;
        }

        if(type > CVA_MAC_MEMBER_TYPE_HBOUND)
        {
            ret = -(RT_ERR_ACL_MEMBER_CHK_TYPE);
            continue;
        }

        if(NULL == pMember)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        for(i = 0; i < MAC_ADDR_LEN; i++)
        {
            mac |= ((uint64)pMember[i] << (40u - (i * 8u)));
        }

        /* Configure member check type */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_MASK_TABLE_CONTROL, CVA_MAC_MASK_TABLE_CONTROL_MAC_MASK_SEL_0_LSP + entryIdx, CVA_MAC_MASK_TABLE_CONTROL_MAC_MASK_SEL_0_LEN, (uint32)type));

        /* Configure MAC member value */
        /*
            CVA_MAC_MASK_TABLE_0_0 <-> mac[2]  to mac[5]
            CVA_MAC_MASK_TABLE_0_1 <-> mac[0]  to mac[1]
        */
        /*
            MAC address: 00:11:22:33:44:55
            mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
            CVA_MAC_MASK_TABLE_0_0: 0x22334455
            CVA_MAC_MASK_TABLE_0_1: 0x0011
        */
        fieldVal = (uint32)(mac & 0xffffffffu);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_MASK_TABLE_0_0 + (entryIdx * 8u), CVA_MAC_MASK_TABLE_0_0_MAC_MASK_0_0_LSP, CVA_MAC_MASK_TABLE_0_0_MAC_MASK_0_0_LEN, fieldVal));
        fieldVal = (uint32)(mac >> 32u);
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_MASK_TABLE_0_1 + (entryIdx * 8u), CVA_MAC_MASK_TABLE_0_1_MAC_MASK_0_1_LSP, CVA_MAC_MASK_TABLE_0_1_MAC_MASK_0_1_LEN, fieldVal));

    } while(0u == 1u);

    return ret;
}
/**@}*/ /*GROUP_MEMBER_CHECK_ASIC*/
/**@}*/ /*GROUP_MEMBER_CHECK*/

/**
 * @addtogroup COUNTER_POLICER_ABNORMAL ACL Counter & Policer & Abnormal Times
 * @{
 */
/**
 * @addtogroup COUNTER_POLICER_ABNORMAL_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL lookup hit counter entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[out] pCounter     ACL counter param (N/A) \n
 *                          The parameters of ACL counter table
 * @retval RT_ERR_OK Get the ACL lookup hit counter entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pCounter" is NULL
 * @retval Others Please refer to cva_table_read(), cva_drv_drv_memset()
 */
RtkApiRet cva_acl_counter_entry_get(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessCounterParam_t *pCounter)
{
    RtkApiRet                      ret       = RT_ERR_OK;
    cva_AclTblAccessCounterEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pCounter)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the entry from the ACL counter table */
        CHK_FUN_CONTINUE(ret, cva_table_read(unit, CVA_ACL_COUNTER, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(pCounter, 0, sizeof(AclTblAccessCounterParam_t)));
        pCounter->counterH = ((asicEntry.val[0] >> CVA_ACL_COUNTER_COUNTER_47_32_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_COUNTER_47_32_LEN));
        pCounter->cntmode  = ((asicEntry.val[0] >> CVA_ACL_COUNTER_CNTMODE_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_CNTMODE_LEN));
        pCounter->enable   = ((asicEntry.val[0] >> CVA_ACL_COUNTER_TYPE_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_TYPE_LEN));
        pCounter->counterL = ((asicEntry.val[1] >> CVA_ACL_COUNTER_COUNTER_31_0_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_COUNTER_31_0_LEN));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL lookup hit counter entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The ACL rule index
 * @param[in]  pCounter     ACL counter param (N/A) \n
 *                          The parameters of ACL counter table
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL lookup hit counter entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pCounter" is NULL
 * @retval Others Please refer to cva_table_write()
 */
RtkApiRet cva_acl_counter_entry_set(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessCounterParam_t *pCounter)
{
    RtkApiRet                      ret       = RT_ERR_OK;
    cva_AclTblAccessCounterEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pCounter)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[0] = ((pCounter->counterH & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_COUNTER_47_32_LEN)) << CVA_ACL_COUNTER_COUNTER_47_32_LSP)
                           | ((pCounter->cntmode & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_CNTMODE_LEN)) << CVA_ACL_COUNTER_CNTMODE_LSP)
                           | ((pCounter->enable & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_TYPE_LEN)) << CVA_ACL_COUNTER_TYPE_LSP);
        asicEntry.val[1] = (pCounter->counterL & cva_drv_tbl_fieldlen2mask(CVA_ACL_COUNTER_COUNTER_31_0_LEN)) << CVA_ACL_COUNTER_COUNTER_31_0_LSP;

        /* Configure the entry in the ACL counter table */
        CHK_FUN_CONTINUE(ret, cva_table_write(unit, CVA_ACL_COUNTER, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL lookup hit policer entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[out] pPolicer     ACL policer param (N/A) \n
 *                          The parameters of ACL policer table
 * @retval RT_ERR_OK Get the ACL lookup hit policer entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pPolicer" is NULL
 * @retval Others Please refer to cva_table_read(), cva_drv_drv_memset()
 */
RtkApiRet cva_acl_policer_entry_get(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPolicerParam_t *pPolicer)
{
    RtkApiRet                      ret       = RT_ERR_OK;
    cva_AclTblAccessPolicerEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pPolicer)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the entry from the ACL policer table */
        CHK_FUN_CONTINUE(ret, cva_table_read(unit, CVA_POLICER, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(pPolicer, 0, sizeof(AclTblAccessCounterParam_t)));
        pPolicer->threshold = ((asicEntry.val[0] >> CVA_POLICER_THRESHOLD_LSP) & cva_drv_tbl_fieldlen2mask(CVA_POLICER_THRESHOLD_LEN));
        pPolicer->tokencnt  = ((asicEntry.val[0] >> CVA_POLICER_TOKEN_COUNTER_LSP) & cva_drv_tbl_fieldlen2mask(CVA_POLICER_TOKEN_COUNTER_LEN));
        pPolicer->enable    = ((asicEntry.val[0] >> CVA_POLICER_TYPE_LSP) & cva_drv_tbl_fieldlen2mask(CVA_POLICER_TYPE_LEN));
        /* Shift 1, because policer table read rate = 0, it means the rate is 8Kbps */
        pPolicer->rate = ((asicEntry.val[1] >> CVA_POLICER_RATE_LSP) & cva_drv_tbl_fieldlen2mask(CVA_POLICER_RATE_LEN)) + 1u;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL lookup hit policer entry
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                          The entry index of the ACL table
 * @param[in]  pPolicer     ACL policer param (N/A) \n
 *                          The parameters of ACL policer table
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL lookup hit policer entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pPolicer" is NULL
 * @retval -RT_ERR_ACL_POLICER_RATE The ACL policer rate is out of range
 * @retval Others Please refer to cva_table_write()
 */
RtkApiRet cva_acl_policer_entry_set(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPolicerParam_t *pPolicer)
{
    RtkApiRet                      ret       = RT_ERR_OK;
    cva_AclTblAccessPolicerEntry_t asicEntry = {0};
    uint32                         rate      = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pPolicer)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(pPolicer->rate < 1u)
        {
            ret = -(RT_ERR_ACL_POLICER_RATE);
            continue;
        }
        else
        {
            /* Shift 1, because policer table write rate = 0, it means the rate is 8Kbps */
            rate = pPolicer->rate - 1u;
        }

        asicEntry.val[0] = ((pPolicer->threshold & cva_drv_tbl_fieldlen2mask(CVA_POLICER_THRESHOLD_LEN)) << CVA_POLICER_THRESHOLD_LSP)
                           | ((pPolicer->tokencnt & cva_drv_tbl_fieldlen2mask(CVA_POLICER_TOKEN_COUNTER_LEN)) << CVA_POLICER_TOKEN_COUNTER_LSP)
                           | ((pPolicer->enable & cva_drv_tbl_fieldlen2mask(CVA_POLICER_TYPE_LEN)) << CVA_POLICER_TYPE_LSP);
        asicEntry.val[1] = (rate & cva_drv_tbl_fieldlen2mask(CVA_POLICER_RATE_LEN)) << CVA_POLICER_RATE_LSP;

        /* Configure the entry in the ACL policer table */
        CHK_FUN_CONTINUE(ret, cva_table_write(unit, CVA_POLICER, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ACL abnormal times entry
 * @param[in]  unit                Stacking unit (1 to 7) \n
 *                                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex        ACL rule index (0 to CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND, CVA_ACL_RULE_BOUND-CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND to CVA_ACL_RULE_BOUND) \n
 *                                 The entry index of the ACL table
 * @param[out] pAbnormalTimesParam ACL abnormal times param (N/A) \n
 *                                 The parameters of ACL abnormal times table
 * @retval RT_ERR_OK Get the ACL abnormal times entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pAbnormalTimesParam" is NULL
 * @retval Others Please refer to cva_table_read(), cva_drv_drv_memset()
 */
RtkApiRet cva_acl_getting_abnormal_times_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessAbnormalTimesParam_t *pAbnormalTimesParam)
{
    RtkApiRet                            ret           = RT_ERR_OK;
    cva_AclTblAccessAbnormalTimesEntry_t asicEntry     = {0};
    uint32                               unavailMinNum = CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND + 1u;                      /* 64 */
    uint32                               unavailMaxNum = CVA_ACL_RULE_BOUND - CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND - 1u; /* 447 */

    do
    {
        /* Check if the input parameters are valid */
        if((aclRuleIndex > CVA_ACL_RULE_BOUND) || ((aclRuleIndex >= unavailMinNum) && (aclRuleIndex <= unavailMaxNum)))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pAbnormalTimesParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the entry from the ACL abnormal times table */
        CHK_FUN_CONTINUE(ret, cva_table_read(unit, CVA_ACL_ABNORMAL_TIMES, aclRuleIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(pAbnormalTimesParam, 0, sizeof(AclTblAccessAbnormalTimesParam_t)));
        pAbnormalTimesParam->enable    = (uint8)((asicEntry.val[0] >> CVA_ACL_ABNORMAL_TIMES_ENABLE_ABNORMAL_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACL_ABNORMAL_TIMES_ENABLE_ABNORMAL_LEN));
        pAbnormalTimesParam->counter   = (uint32)((asicEntry.val[1] >> CVA_ACL_ABNORMAL_TIMES_COUNTER_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACL_ABNORMAL_TIMES_COUNTER_LEN));
        pAbnormalTimesParam->threshold = (uint32)((asicEntry.val[1] >> CVA_ACL_ABNORMAL_TIMES_MITIGATION_THRESHOLD_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACL_ABNORMAL_TIMES_MITIGATION_THRESHOLD_LEN));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL abnormal times entry
 * @param[in]  unit                Stacking unit (1 to 7) \n
 *                                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex        ACL rule index (0 to CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND, CVA_ACL_RULE_BOUND-CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND to CVA_ACL_RULE_BOUND) \n
 *                                 The entry index of the ACL table
 * @param[in]  pAbnormalTimesParam ACL abnormal times param (N/A) \n
 *                                 The parameters of ACL abnormal times table
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL abnormal times entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pAbnormalTimesParam" is NULL
 * @retval Others Please refer to cva_table_write()
 */
RtkApiRet cva_acl_setting_abnormal_times_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessAbnormalTimesParam_t *pAbnormalTimesParam)
{
    RtkApiRet                            ret           = RT_ERR_OK;
    cva_AclTblAccessAbnormalTimesEntry_t asicEntry     = {0};
    uint32                               unavailMinNum = CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND + 1u;                      /* 64 */
    uint32                               unavailMaxNum = CVA_ACL_RULE_BOUND - CVA_ACL_ABNORMAL_TIMES_ENTRY_BOUND - 1u; /* 447 */

    do
    {
        /* Check if the input parameters are valid */
        if((aclRuleIndex > CVA_ACL_RULE_BOUND) || ((aclRuleIndex >= unavailMinNum) && (aclRuleIndex <= unavailMaxNum)))
        {

            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pAbnormalTimesParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }
        asicEntry.val[0] = (((uint32)pAbnormalTimesParam->enable & cva_drv_tbl_fieldlen2mask(CVA_ACL_ABNORMAL_TIMES_ENABLE_ABNORMAL_LEN)) << CVA_ACL_ABNORMAL_TIMES_ENABLE_ABNORMAL_LSP);
        asicEntry.val[1] = (((uint32)pAbnormalTimesParam->counter & cva_drv_tbl_fieldlen2mask(CVA_ACL_ABNORMAL_TIMES_COUNTER_LEN)) << CVA_ACL_ABNORMAL_TIMES_COUNTER_LSP) | (((uint32)pAbnormalTimesParam->threshold & cva_drv_tbl_fieldlen2mask(CVA_ACL_ABNORMAL_TIMES_MITIGATION_THRESHOLD_LEN)) << CVA_ACL_ABNORMAL_TIMES_MITIGATION_THRESHOLD_LSP);

        /* Configure the entry in the ACL abnormal times table */
        CHK_FUN_CONTINUE(ret, cva_table_write(unit, CVA_ACL_ABNORMAL_TIMES, aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the enable status of the ingress ACL time slot function
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pEnable Enable status for ingress ACL (N/A) \n
 *                     The Enable status of the ingress ACL time slot function
 * @retval RT_ERR_OK Get the enable status of the ingress ACL time slot function successfully
 * @retval -RT_ERR_NULL_POINTER The "pEnable" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_time_slot_en_get(uint32 unit, AclEnable *pEnable)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pEnable)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /*get the enable status of the time slot*/
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_ACL_TIME_SLOT_COUNTER_0, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_ENABLE_0_LSP, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_ENABLE_0_LEN, &fieldVal));
        *pEnable = fieldVal;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Enable/Disable the ingress ACL time slot function
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  enable  Enable status for ingress ACL (CVA_ACL_DISABLE to CVA_ACL_ENABLE) \n
 *                     The Enable status of the ingress ACL time slot function
 * @param[out] None
 * @retval RT_ERR_OK Enable/Disable the ingress ACL time slot function successfully
 * @retval -RT_ERR_ACL_ENABLE The "enable" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_time_slot_en_set(uint32 unit, AclEnable enable)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(enable > CVA_ACL_ENABLE)
        {
            ret = -(RT_ERR_ACL_ENABLE);
            continue;
        }

        /* Enable/Disable the time slot function */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_ACL_TIME_SLOT_COUNTER_0, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_ENABLE_0_LSP, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_ENABLE_0_LEN, enable));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the interval of the ingress ACL time slot function
 * @param[in]  unit       Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pThreshold Time slot interval for ingress ACL (N/A) \n
 *                        The interval of the ingress ACL time slot function, the unit is millisecond, means 1ms per unit cycle
 * @retval RT_ERR_OK Get the interval of the ingress ACL time slot function successfully
 * @retval -RT_ERR_NULL_POINTER The "pThreshold" is NULL
 * @retval Others Please refer to cva_reg_field_read()
 */
RtkApiRet cva_acl_time_slot_threshold_get(uint32 unit, AclThreshold *pThreshold)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pThreshold)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the interval of the time slot function */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_ACL_TIME_SLOT_COUNTER_0, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_THRESHOLD_0_LSP, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_THRESHOLD_0_LEN, &fieldVal));
        *pThreshold = fieldVal;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the interval of the ingress ACL time slot function
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  threshold Time slot interval for ingress ACL (CVA_ACL_TIME_SLOT_THRESHOLD_LBOUND to CVA_ACL_TIME_SLOT_THRESHOLD_HBOUND) \n
 *                       The interval of the ingress ACL time slot function, the unit is millisecond, means 1ms per unit cycle
 * @param[out] None
 * @retval RT_ERR_OK Set the interval of the ingress ACL time slot function successfully
 * @retval -RT_ERR_ACL_TIME_SLOT_THRESHOLD The "threshold" is out of range
 * @retval Others Please refer to cva_reg_field_write()
 */
RtkApiRet cva_acl_time_slot_threshold_set(uint32 unit, AclThreshold threshold)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if((threshold < CVA_ACL_TIME_SLOT_THRESHOLD_LBOUND) || (threshold > CVA_ACL_TIME_SLOT_THRESHOLD_HBOUND))
        {
            ret = -(RT_ERR_ACL_TIME_SLOT_THRESHOLD);
            continue;
        }

        /* Configure the interval of the time slot function */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_ACL_TIME_SLOT_COUNTER_0, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_THRESHOLD_0_LSP, CVA_ACL_TIME_SLOT_COUNTER_0_TIME_SLOT_THRESHOLD_0_LEN, threshold));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get the ingress ACL abnormal destination port mask
 * @param[in]  unit Stacking unit (1 to 7) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pDpm The port mask for ingress ACL (N/A) \n
 *                  The abnormal destination port mask for ingress ACL
 * @retval RT_ERR_OK Get the ingress ACL abnormal destination port mask successfully
 * @retval -RT_ERR_NULL_POINTER The "pDpm" is NULL
 * @retval Others Please refer to cva_reg_field_read(), cva_drv_pmap_convert()
 */
RtkApiRet cva_acl_abnormal_dpm_get(uint32 unit, RtkPmap *pDpm)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPmap   srcPmap  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pDpm)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the abnormal destination port mask */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_ACL_ABNORMAL_COUNTROL_0, CVA_ACL_ABNORMAL_COUNTROL_0_ABNORMAL_DPM_0_LSP, CVA_ACL_ABNORMAL_COUNTROL_0_ABNORMAL_DPM_0_LEN, &fieldVal));
        srcPmap = (RtkPmap)(fieldVal & (CVA_RTK_SYS_PMAP_ALL | CVA_RTK_CPU_PMAP));
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(srcPmap, pDpm, CVA_EM_SYS_TO_USER));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ingress ACL abnormal destination port mask
 * @param[in]  unit Stacking unit (1 to 7) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  dpm  The port mask for ingress ACL (CVA_RTK_USER_PMAP_1ST to CVA_RTK_USER_PMAP_ALL, CVA_RTK_CPU_PMAP) \n
 *                  The abnormal destination port mask for ingress ACL
 * @param[out] None
 * @retval RT_ERR_OK Set the ingress ACL abnormal destination port mask successfully
 * @retval Others Please refer to cva_drv_pmap_convert(), cva_reg_field_write()
 */
RtkApiRet cva_acl_abnormal_dpm_set(uint32 unit, RtkPmap dpm)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPmap   dstPmap = 0u;

    do
    {
        /* Configure the abnormal destination port mask */
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(dpm, &dstPmap, CVA_EM_USER_TO_SYS));
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_ACL_ABNORMAL_COUNTROL_0, CVA_ACL_ABNORMAL_COUNTROL_0_ABNORMAL_DPM_0_LSP, CVA_ACL_ABNORMAL_COUNTROL_0_ABNORMAL_DPM_0_LEN, (uint32)dstPmap));
    } while(0u == 1u);

    return ret;
}
/**@}*/ /*COUNTER_POLICER_ABNORMAL_ASIC*/
/**@}*/ /*COUNTER_POLICER_ABNORMAL*/

/**
 * @addtogroup ACL_TIMER_DISCARD ACL Timer Discard
 * @{
 */
/**
 * @addtogroup ACL_TIMER_DISCARD_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL timer discard entry
 * @param[in]  unit                Stacking unit (1 to 7) \n
 *                                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex        ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                                 The ACL rule index
 * @param[out] pTimerDiscardParam  ACL timer discard param (N/A) \n
 *                                 The parameters of ACL timer discard table
 * @retval RT_ERR_OK Get the ACL timer discard entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pTimerDiscardParam" is NULL
 * @retval Others Please refer to cva_table_read(), cva_drv_drv_memset()
 */
RtkApiRet cva_acl_getting_timer_discard_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessTimerDiscardParam_t *pTimerDiscardParam)
{
    RtkApiRet                           ret       = RT_ERR_OK;
    cva_AclTblAccessTimerDiscardEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pTimerDiscardParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the entry from the ACL timer discard table */
        CHK_FUN_CONTINUE(ret, cva_table_read(unit, CVA_TIMESTAMP_DISCARD, aclRuleIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(pTimerDiscardParam, 0, sizeof(AclTblAccessTimerDiscardParam_t)));
        pTimerDiscardParam->withdraw        = (uint8)((asicEntry.val[0] >> CVA_TIMESTAMP_DISCARD_ENABLE_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_ENABLE_LEN));
        pTimerDiscardParam->monitorRun      = (uint8)((asicEntry.val[0] >> CVA_TIMESTAMP_DISCARD_MONITOR_RUN_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_RUN_LEN));
        pTimerDiscardParam->guardRun        = (uint8)((asicEntry.val[0] >> CVA_TIMESTAMP_DISCARD_GUARD_RUN_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_RUN_LEN));
        pTimerDiscardParam->threshold       = (uint8)((asicEntry.val[0] >> CVA_TIMESTAMP_DISCARD_THRESHOLD_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_THRESHOLD_LEN));
        pTimerDiscardParam->monitorTime     = (uint32)((asicEntry.val[1] >> CVA_TIMESTAMP_DISCARD_MONITOR_TIME_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_TIME_LEN));
        pTimerDiscardParam->guardTime       = (uint32)((asicEntry.val[2] >> CVA_TIMESTAMP_DISCARD_GUARD_TIME_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_GUARD_TIME_LEN));
        pTimerDiscardParam->monitorEnd      = (uint32)((asicEntry.val[3] >> CVA_TIMESTAMP_DISCARD_MONITOR_END_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_END_LEN));
        pTimerDiscardParam->monitorOverflow = (uint8)((asicEntry.val[4] >> CVA_TIMESTAMP_DISCARD_MONITOR_OF_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_OF_LEN));
        pTimerDiscardParam->guardEnd        = (uint32)((asicEntry.val[5] >> CVA_TIMESTAMP_DISCARD_GUARD_END_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_GUARD_END_LEN));
        /* asicEntry.val[6] is a reserved field */
        pTimerDiscardParam->guardOverflow = (uint8)((asicEntry.val[7] >> CVA_TIMESTAMP_DISCARD_GUARD_OF_LSP) & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_GUARD_OF_LEN));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set the ACL timer discard entry
 * @param[in]  unit                Stacking unit (1 to 7) \n
 *                                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclRuleIndex        ACL rule index (0 to CVA_ACL_RULE_BOUND) \n
 *                                 The entry index of the ACL table
 * @param[in]  pTimerDiscardParam  ACL timer discard param (N/A) \n
 *                                 The parameters of ACL timer discard table
 * @param[out] None
 * @retval RT_ERR_OK Set the ACL timer discard entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL rule index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pTimerDiscardParam" is NULL
 * @retval Others Please refer to cva_table_write()
 */
RtkApiRet cva_acl_setting_timer_discard_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessTimerDiscardParam_t *pTimerDiscardParam)
{
    RtkApiRet                           ret       = RT_ERR_OK;
    cva_AclTblAccessTimerDiscardEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > CVA_ACL_RULE_BOUND)
        {

            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        asicEntry.val[0] = (((uint32)pTimerDiscardParam->withdraw & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_ENABLE_LEN)) << CVA_TIMESTAMP_DISCARD_ENABLE_LSP) | (((uint32)pTimerDiscardParam->monitorRun & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_RUN_LEN)) << CVA_TIMESTAMP_DISCARD_MONITOR_RUN_LSP) | (((uint32)pTimerDiscardParam->guardRun & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_GUARD_RUN_LEN)) << CVA_TIMESTAMP_DISCARD_GUARD_RUN_LSP) | (((uint32)pTimerDiscardParam->threshold & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_THRESHOLD_LEN)) << CVA_TIMESTAMP_DISCARD_THRESHOLD_LSP);
        asicEntry.val[1] = ((uint32)(pTimerDiscardParam->monitorTime & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_TIME_LEN)) << CVA_TIMESTAMP_DISCARD_MONITOR_TIME_LSP);
        asicEntry.val[2] = ((uint32)(pTimerDiscardParam->guardTime & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_GUARD_TIME_LEN)) << CVA_TIMESTAMP_DISCARD_GUARD_TIME_LSP);
        asicEntry.val[3] = ((uint32)(pTimerDiscardParam->monitorEnd & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_END_LEN)) << CVA_TIMESTAMP_DISCARD_MONITOR_END_LSP);
        asicEntry.val[4] = (((uint32)pTimerDiscardParam->monitorOverflow & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_MONITOR_OF_LEN)) << CVA_TIMESTAMP_DISCARD_MONITOR_OF_LSP);
        asicEntry.val[5] = ((uint32)(pTimerDiscardParam->guardEnd & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_GUARD_END_LEN)) << CVA_TIMESTAMP_DISCARD_GUARD_END_LSP);
        /* asicEntry.val[6] is a reserved field */
        asicEntry.val[7] = (((uint32)pTimerDiscardParam->guardOverflow & cva_drv_tbl_fieldlen2mask(CVA_TIMESTAMP_DISCARD_GUARD_OF_LEN)) << CVA_TIMESTAMP_DISCARD_GUARD_OF_LSP);

        /* Configure the entry in the ACL timer discard table */
        CHK_FUN_CONTINUE(ret, cva_table_write(unit, CVA_TIMESTAMP_DISCARD, aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}
/**@}*/ /*ACL_TIMER_DISCARD_ASIC*/
/**@}*/ /*ACL_TIMER_DISCARD*/

/**
 * @addtogroup ACL_HEADER_DUMP ACL Header Dump
 * @{
 */
/**
 * @addtogroup ACL_HEADER_DUMP_ASIC Low Level Driver
 * @brief ACL Low Level Driver
 * @{
 */
/**
 * @brief Get the ACL header dump entry
 * @param[in]  unit               Stacking unit (1 to 7) \n
 *                                Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  aclHeaderDumpIndex ACL Header dump entry index (0 to CVA_ACL_HEADER_DUMP_BOUND) \n
 *                                The entry index of ACL header dump Table
 * @param[out] pHeaderDumpParam   ACL header dump param (N/A) \n
 *                                The parameters of ACL header dump table
 * @retval RT_ERR_OK Get the ACL header dump entry successfully
 * @retval -RT_ERR_ACL_ENTRYIDX The ACL header dump entry index is out of range
 * @retval -RT_ERR_NULL_POINTER The "pHeaderDumpParam" is NULL
 * @retval Others Please refer to cva_table_read(), cva_drv_drv_memset()
 */
RtkApiRet cva_acl_getting_header_dump_entry(uint32 unit, AclHeaderDumpId aclHeaderDumpIndex, AclTblAccessHeaderDumpParam_t *pHeaderDumpParam)
{
    RtkApiRet                         ret       = RT_ERR_OK;
    cva_AclTblAccessHeaderDumpEntry_t asicEntry = {0};
    uint8                             sysPort   = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(aclHeaderDumpIndex > CVA_ACL_HEADER_DUMP_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(NULL == pHeaderDumpParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* Retrieve the entry from the ACL header dump table */
        CHK_FUN_CONTINUE(ret, cva_table_read(unit, CVA_HEADER_DUMP, (uint32)aclHeaderDumpIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(pHeaderDumpParam, 0, sizeof(AclTblAccessHeaderDumpParam_t)));
        sysPort                     = (uint8)((asicEntry.val[0] >> CVA_HEADER_DUMP_SPHY_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_SPHY_LEN));
        pHeaderDumpParam->sphy      = cva_g_sysPort2userPort[sysPort];
        pHeaderDumpParam->timestamp = (((uint64)asicEntry.val[1] >> CVA_HEADER_DUMP_TIMESTAMP_31_0_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_TIMESTAMP_31_0_LEN));
        pHeaderDumpParam->timestamp |= ((((uint64)asicEntry.val[2] >> CVA_HEADER_DUMP_TIMESTAMP_47_32_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_TIMESTAMP_47_32_LEN)) << 32u);
        pHeaderDumpParam->iaclidx = (uint16)((asicEntry.val[3] >> CVA_HEADER_DUMP_IACLIDX_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_IACLIDX_LEN));
        pHeaderDumpParam->eaclidx = (uint16)((asicEntry.val[3] >> CVA_HEADER_DUMP_EACLIDX_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_EACLIDX_LEN));

        pHeaderDumpParam->da[0] = (uint8)((asicEntry.val[5] >> 8u) & 0xFFu);
        pHeaderDumpParam->da[1] = (uint8)((asicEntry.val[5] >> 0u) & 0xFFu);
        pHeaderDumpParam->da[2] = (uint8)((asicEntry.val[4] >> 24u) & 0xFFu);
        pHeaderDumpParam->da[3] = (uint8)((asicEntry.val[4] >> 16u) & 0xFFu);
        pHeaderDumpParam->da[4] = (uint8)((asicEntry.val[4] >> 8u) & 0xFFu);
        pHeaderDumpParam->da[5] = (uint8)((asicEntry.val[4] >> 0u) & 0xFFu);
        /* asicEntry.val[6] is a reserved field */
        pHeaderDumpParam->sa[0] = (uint8)((asicEntry.val[8] >> 8u) & 0xFFu);
        pHeaderDumpParam->sa[1] = (uint8)((asicEntry.val[8] >> 0u) & 0xFFu);
        pHeaderDumpParam->sa[2] = (uint8)((asicEntry.val[7] >> 24u) & 0xFFu);
        pHeaderDumpParam->sa[3] = (uint8)((asicEntry.val[7] >> 16u) & 0xFFu);
        pHeaderDumpParam->sa[4] = (uint8)((asicEntry.val[7] >> 8u) & 0xFFu);
        pHeaderDumpParam->sa[5] = (uint8)((asicEntry.val[7] >> 0u) & 0xFFu);

        pHeaderDumpParam->otagExist = (uint8)((asicEntry.val[9] >> CVA_HEADER_DUMP_OTAG_EXIST_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OTAG_EXIST_LEN));
        pHeaderDumpParam->otagPcp   = (uint8)((asicEntry.val[9] >> CVA_HEADER_DUMP_OTAG_PCP_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OTAG_PCP_LEN));
        pHeaderDumpParam->otagCfi   = (uint8)((asicEntry.val[9] >> CVA_HEADER_DUMP_OTAG_CFI_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OTAG_CFI_LEN));
        pHeaderDumpParam->otagVid   = (uint16)((asicEntry.val[9] >> CVA_HEADER_DUMP_OTAG_VID_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OTAG_VID_LEN));
        pHeaderDumpParam->itagExist = (uint8)((asicEntry.val[10] >> CVA_HEADER_DUMP_ITAG_EXIST_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_ITAG_EXIST_LEN));
        pHeaderDumpParam->itagPcp   = (uint8)((asicEntry.val[10] >> CVA_HEADER_DUMP_ITAG_PCP_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_ITAG_PCP_LEN));
        pHeaderDumpParam->itagCfi   = (uint8)((asicEntry.val[10] >> CVA_HEADER_DUMP_ITAG_CFI_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_ITAG_CFI_LEN));
        pHeaderDumpParam->itagVid   = (uint16)((asicEntry.val[10] >> CVA_HEADER_DUMP_ITAG_VID_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_ITAG_VID_LEN));
        pHeaderDumpParam->ethertype = (uint16)((asicEntry.val[11] >> CVA_HEADER_DUMP_ETHERTYPE_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_ETHERTYPE_LEN));

        pHeaderDumpParam->offsetField0  = (uint16)((asicEntry.val[12] >> CVA_HEADER_DUMP_OFFSET_FIELD0_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD0_LEN));
        pHeaderDumpParam->offsetField1  = (uint16)((asicEntry.val[12] >> CVA_HEADER_DUMP_OFFSET_FIELD1_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD1_LEN));
        pHeaderDumpParam->offsetField2  = (uint16)((asicEntry.val[13] >> CVA_HEADER_DUMP_OFFSET_FIELD2_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD2_LEN));
        pHeaderDumpParam->offsetField3  = (uint16)((asicEntry.val[13] >> CVA_HEADER_DUMP_OFFSET_FIELD3_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD3_LEN));
        pHeaderDumpParam->offsetField4  = (uint16)((asicEntry.val[14] >> CVA_HEADER_DUMP_OFFSET_FIELD4_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD4_LEN));
        pHeaderDumpParam->offsetField5  = (uint16)((asicEntry.val[14] >> CVA_HEADER_DUMP_OFFSET_FIELD5_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD5_LEN));
        pHeaderDumpParam->offsetField6  = (uint16)((asicEntry.val[15] >> CVA_HEADER_DUMP_OFFSET_FIELD6_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD6_LEN));
        pHeaderDumpParam->offsetField7  = (uint16)((asicEntry.val[15] >> CVA_HEADER_DUMP_OFFSET_FIELD7_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD7_LEN));
        pHeaderDumpParam->offsetField8  = (uint16)((asicEntry.val[16] >> CVA_HEADER_DUMP_OFFSET_FIELD8_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD8_LEN));
        pHeaderDumpParam->offsetField9  = (uint16)((asicEntry.val[16] >> CVA_HEADER_DUMP_OFFSET_FIELD9_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD9_LEN));
        pHeaderDumpParam->offsetField10 = (uint16)((asicEntry.val[17] >> CVA_HEADER_DUMP_OFFSET_FIELD10_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD10_LEN));
        pHeaderDumpParam->offsetField11 = (uint16)((asicEntry.val[17] >> CVA_HEADER_DUMP_OFFSET_FIELD11_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD11_LEN));
        pHeaderDumpParam->offsetField12 = (uint16)((asicEntry.val[18] >> CVA_HEADER_DUMP_OFFSET_FIELD12_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD12_LEN));
        pHeaderDumpParam->offsetField13 = (uint16)((asicEntry.val[18] >> CVA_HEADER_DUMP_OFFSET_FIELD13_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD13_LEN));
        pHeaderDumpParam->offsetField14 = (uint16)((asicEntry.val[19] >> CVA_HEADER_DUMP_OFFSET_FIELD14_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD14_LEN));
        pHeaderDumpParam->offsetField15 = (uint16)((asicEntry.val[19] >> CVA_HEADER_DUMP_OFFSET_FIELD15_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD15_LEN));
        pHeaderDumpParam->offsetField16 = (uint16)((asicEntry.val[20] >> CVA_HEADER_DUMP_OFFSET_FIELD16_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD16_LEN));
        pHeaderDumpParam->offsetField17 = (uint16)((asicEntry.val[20] >> CVA_HEADER_DUMP_OFFSET_FIELD17_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD17_LEN));
        pHeaderDumpParam->offsetField18 = (uint16)((asicEntry.val[21] >> CVA_HEADER_DUMP_OFFSET_FIELD18_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD18_LEN));
        pHeaderDumpParam->offsetField19 = (uint16)((asicEntry.val[21] >> CVA_HEADER_DUMP_OFFSET_FIELD19_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD19_LEN));
        pHeaderDumpParam->offsetField20 = (uint16)((asicEntry.val[22] >> CVA_HEADER_DUMP_OFFSET_FIELD20_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD20_LEN));
        pHeaderDumpParam->offsetField21 = (uint16)((asicEntry.val[22] >> CVA_HEADER_DUMP_OFFSET_FIELD21_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD21_LEN));
        pHeaderDumpParam->offsetField22 = (uint16)((asicEntry.val[23] >> CVA_HEADER_DUMP_OFFSET_FIELD22_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD22_LEN));
        pHeaderDumpParam->offsetField23 = (uint16)((asicEntry.val[23] >> CVA_HEADER_DUMP_OFFSET_FIELD23_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD23_LEN));
        pHeaderDumpParam->offsetField24 = (uint16)((asicEntry.val[24] >> CVA_HEADER_DUMP_OFFSET_FIELD24_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD24_LEN));
        pHeaderDumpParam->offsetField25 = (uint16)((asicEntry.val[24] >> CVA_HEADER_DUMP_OFFSET_FIELD25_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD25_LEN));
        pHeaderDumpParam->offsetField26 = (uint16)((asicEntry.val[25] >> CVA_HEADER_DUMP_OFFSET_FIELD26_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD26_LEN));
        pHeaderDumpParam->offsetField27 = (uint16)((asicEntry.val[25] >> CVA_HEADER_DUMP_OFFSET_FIELD27_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD27_LEN));
        pHeaderDumpParam->offsetField28 = (uint16)((asicEntry.val[26] >> CVA_HEADER_DUMP_OFFSET_FIELD28_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD28_LEN));
        pHeaderDumpParam->offsetField29 = (uint16)((asicEntry.val[26] >> CVA_HEADER_DUMP_OFFSET_FIELD29_LSP) & cva_drv_tbl_fieldlen2mask(CVA_HEADER_DUMP_OFFSET_FIELD29_LEN));

    } while(0u == 1u);

    return ret;
}
/**@}*/ /*ACL_HEADER_DUMP_ASIC*/
/**@}*/ /*ACL_HEADER_DUMP*/

/**@}*/ /* ACL */