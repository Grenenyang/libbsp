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
#include "rtl907xd_va_api_acl.h"
#include "rtl907xd_va_asicdrv_acl.h"
#include "rtl907xd_va_asicdrv_tcam.h"
#include "rtl907xd_va_asicdrv_tcam_crc.h"
#include "rtl907xd_va_asicdrv_tcam_ctrl.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"

/******************************************************************************/
/* Global Variables Definition                                                */
/******************************************************************************/
static bool   dva_g_AclRuntimeSetStart                                         = false;
static uint64 dva_g_aclRuntimePieBlockEntryFlag[DVA_TCAM_PIE_BLOCK_BOUND + 1u] = {0};
static uint32 dva_g_aclRuntimePieEntrySramAddr[DVA_ACL_INT_RULE_BOUND + 1u]    = {0};
static uint32 dva_g_aclNum                                                     = 0u;
static uint32 dva_g_aclShareMemoryCurAddr                                      = 0u;

/******************************************************************************/
/* Functions Definition                                                       */
/******************************************************************************/
RtkApiRet dva_drv_acl_userport_2_sysport(RtkPort userPort, RtkPort *pSysPort)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((userPort != (RtkPort)DVA_RTK_CPU_PORT) && ((userPort > (RtkPort)DVA_RTK_USER_PORT_END) || (userPort < (RtkPort)DVA_RTK_USER_PORT_1ST)))
    {
        retVal = -(RT_ERR_PORT_ID);
    }

    *pSysPort = (RtkPort)dva_g_userPort2sysPort[userPort];

    return retVal;
}

RtkApiRet dva_drv_acl_userpmap_2_syspmap(RtkPmap userPmap, RtkPmap *pSysPmap)
{
    RtkApiRet retVal = RT_ERR_OK;
    retVal           = dva_drv_pmap_convert(userPmap, pSysPmap, DVA_EM_USER_TO_SYS);

    return retVal;
}

RtkApiRet dva_drv_acl_internal_rule_id_check(uint32 unit, AclRuleId aclRuleIndex)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* PIE33 cannot use */
        if((aclRuleIndex >= DVA_ACL_INT_RULE_PIE33_LBOUND) && (aclRuleIndex <= DVA_ACL_INT_RULE_PIE33_HBOUND))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        /* PIE35 cannot use */
        if((aclRuleIndex >= DVA_ACL_INT_RULE_PIE35_LBOUND) && (aclRuleIndex <= DVA_ACL_INT_RULE_PIE35_HBOUND))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(aclRuleIndex > DVA_ACL_INT_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_partition_get(uint32 unit, AclPartition *pAclPartition)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    fieldVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pAclPartition)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /*
            Retrieve the ACL partition value
            Increase by one means an increase of 64 entries belonging to Ingress ACL
            A value of 0 means all rules are egress
            A value of 1 means the 0th is ingress, and the 1st to the 8th are egress
            A value greater than 9 means all rules are ingress
            All internal protocols are in EACL, so 9 is not allowed
        */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_ACL_PARTITION_NUMBER, DVA_ACL_PARTITION_NUMBER_ACL_PARTITION_NUM_LSP, DVA_ACL_PARTITION_NUMBER_ACL_PARTITION_NUM_LEN, &fieldVal));
        *pAclPartition = fieldVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_partition_set(uint32 unit, AclPartition aclPartition)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(aclPartition > DVA_ACL_PARTITION_BOUND)
        {
            ret = -(RT_ERR_ACL_PARTITION);
            continue;
        }

        /*
            Retrieve the ACL partition value
            Increase by one means an increase of 64 entries belonging to Ingress ACL
            A value of 0 means all rules are egress
            A value of 1 means the 0th is ingress, and the 1st to the 8th are egress
            A value greater than 9 means all rules are ingress
            All internal protocols are in EACL, so 9 is not allowed
        */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ACL_PARTITION_NUMBER, DVA_ACL_PARTITION_NUMBER_ACL_PARTITION_NUM_LSP, DVA_ACL_PARTITION_NUMBER_ACL_PARTITION_NUM_LEN, aclPartition));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_mode_template_index_check(uint32 unit, AclTemplateId tpIdx, AclMode aclMode)
{
    AclPartition aclPartition    = 0u;
    uint32       iaclTpIdxHBound = 0u;
    uint32       eaclTpIdxLBound = 0u;
    /* Increase by one means an increase of 2 template generators belonging to Ingress ACL */
    uint32 incrTpIdx = 2u;

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > DVA_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if((aclMode != DVA_ACL_IS_IACL) && (aclMode != DVA_ACL_IS_EACL))
        {
            ret = -(RT_ERR_ACL_MODE);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_acl_partition_get(unit, &aclPartition));

        /* A value of 0 means all rules are egress */
        if(0u == aclPartition)
        {
            if(DVA_ACL_IS_IACL == aclMode)
            {
                ret = -(RT_ERR_ACL_IACL_PARTITION);
            }
        }
        /* aclPartition = 1 ~ 8 */
        else if(aclPartition <= DVA_ACL_PARTITION_BOUND)
        {
            eaclTpIdxLBound = aclPartition * incrTpIdx;
            iaclTpIdxHBound = eaclTpIdxLBound - 1u;

            if(DVA_ACL_IS_IACL == aclMode)
            {
                if(tpIdx > iaclTpIdxHBound)
                {
                    ret = -(RT_ERR_ACL_IACL_PARTITION);
                }
            }
            else
            {
                if(tpIdx < eaclTpIdxLBound)
                {
                    ret = -(RT_ERR_ACL_EACL_PARTITION);
                }
            }
        }
        /* aclPartition = 9, means all rules are ingress, but 9 is not allowed */
        else
        {
            ret = -(RT_ERR_ACL_PARTITION);
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_mode_rule_index_check(uint32 unit, AclRuleId aclRuleIndex, AclMode aclMode)
{
    AclPartition aclPartition        = 0u;
    uint32       iaclRuleIndexHBound = 0u;
    uint32       eaclRuleIndexLBound = 0u;
    /* Increase by one means an increase of 64 entries belonging to Ingress ACL */
    uint32 incrAclRuleIndex = 64u;

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(aclRuleIndex > DVA_ACL_INT_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if((aclMode != DVA_ACL_IS_IACL) && (aclMode != DVA_ACL_IS_EACL))
        {
            ret = -(RT_ERR_ACL_MODE);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_acl_partition_get(unit, &aclPartition));

        /* A value of 0 means all rules are egress */
        if(0u == aclPartition)
        {
            if(DVA_ACL_IS_IACL == aclMode)
            {
                ret = -(RT_ERR_ACL_IACL_PARTITION);
            }
        }
        /* aclPartition = 1 ~ 8 */
        else if(aclPartition <= DVA_ACL_PARTITION_BOUND)
        {
            eaclRuleIndexLBound = aclPartition * incrAclRuleIndex;
            iaclRuleIndexHBound = eaclRuleIndexLBound - 1u;

            if(DVA_ACL_IS_IACL == aclMode)
            {
                if(aclRuleIndex > iaclRuleIndexHBound)
                {
                    ret = -(RT_ERR_ACL_IACL_PARTITION);
                }
            }
            else
            {
                if(aclRuleIndex < eaclRuleIndexLBound)
                {
                    ret = -(RT_ERR_ACL_EACL_PARTITION);
                }
            }
        }
        /* aclPartition = 9, means all rules are ingress, but 9 is not allowed */
        else
        {
            ret = -(RT_ERR_ACL_PARTITION);
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType)
{
    uint32 regAddr  = 0u;
    uint32 regValue = 0u;
    uint32 regField = 0u;

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > DVA_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > DVA_ACL_TEMPLATE_FIELD_BOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_IDX);
            continue;
        }

        if(NULL == pType)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
        regAddr = DVA_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, regAddr, regField, DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, &regValue));
        *pType = regValue;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type)
{
    uint32    regAddr  = 0u;
    uint32    regField = 0u;
    RtkApiRet ret      = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > DVA_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > DVA_ACL_TEMPLATE_FIELD_BOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_IDX);
            continue;
        }

        if(type > DVA_IACL_FLD_HBOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_TYPE);
            continue;
        }

        /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
        regAddr = DVA_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, regAddr, regField, DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, type));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_eacl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType)
{
    uint32 regAddr  = 0u;
    uint32 regValue = 0u;
    uint32 regField = 0u;

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > DVA_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > DVA_ACL_TEMPLATE_FIELD_BOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_IDX);
            continue;
        }

        if(NULL == pType)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
        regAddr = DVA_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, regAddr, regField, DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, &regValue));
        *pType = regValue;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_eacl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type)
{
    uint32    regAddr  = 0u;
    uint32    regField = 0u;
    RtkApiRet ret      = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > DVA_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > DVA_ACL_TEMPLATE_FIELD_BOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_IDX);
            continue;
        }

        if(type > DVA_EACL_FLD_HBOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_TYPE);
            continue;
        }

        /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
        regAddr = DVA_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, regAddr, regField, DVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, type));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_getting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    dva_AclTblAccessPieEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pPieParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_table_read(unit, DVA_PIE_TABLE, aclRuleIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, dva_drv_memset(pPieParam, 0, sizeof(AclTblAccessPieParam_t)));
        pPieParam->data0  = (uint16)((asicEntry.val[0]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data1  = (uint16)((asicEntry.val[0] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data2  = (uint16)((asicEntry.val[1]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data3  = (uint16)((asicEntry.val[1] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data4  = (uint16)((asicEntry.val[2]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data5  = (uint16)((asicEntry.val[2] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data6  = (uint16)((asicEntry.val[3]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data7  = (uint16)((asicEntry.val[3] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data8  = (uint16)((asicEntry.val[4]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data9  = (uint16)((asicEntry.val[4] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data10 = (uint16)((asicEntry.val[5]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data11 = (uint16)((asicEntry.val[5] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->valid  = asicEntry.val[6] & dva_drv_tbl_fieldlen2mask(DVA_PIE_TABLE_TCAM_VALID_LEN);
        pPieParam->mask0  = (uint16)((asicEntry.val[7]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask1  = (uint16)((asicEntry.val[7] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask2  = (uint16)((asicEntry.val[8]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask3  = (uint16)((asicEntry.val[8] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask4  = (uint16)((asicEntry.val[9]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask5  = (uint16)((asicEntry.val[9] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask6  = (uint16)((asicEntry.val[10]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask7  = (uint16)((asicEntry.val[10] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask8  = (uint16)((asicEntry.val[11]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask9  = (uint16)((asicEntry.val[11] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask10 = (uint16)((asicEntry.val[12]) & dva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask11 = (uint16)((asicEntry.val[12] >> 16u) & dva_drv_tbl_fieldlen2mask(16u));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_setting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    dva_AclTblAccessPieEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pPieParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[0]  = (((uint32)pPieParam->data0 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data1 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[1]  = (((uint32)pPieParam->data2 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data3 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[2]  = (((uint32)pPieParam->data4 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data5 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[3]  = (((uint32)pPieParam->data6 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data7 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[4]  = (((uint32)pPieParam->data8 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data9 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[5]  = (((uint32)pPieParam->data10 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data11 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[6]  = pPieParam->valid & dva_drv_tbl_fieldlen2mask(DVA_PIE_TABLE_TCAM_VALID_LEN);
        asicEntry.val[7]  = (((uint32)pPieParam->mask0 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask1 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[8]  = (((uint32)pPieParam->mask2 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask3 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[9]  = (((uint32)pPieParam->mask4 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask5 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[10] = (((uint32)pPieParam->mask6 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask7 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[11] = (((uint32)pPieParam->mask8 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask9 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[12] = (((uint32)pPieParam->mask10 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask11 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_pie_entry(unit, aclRuleIndex, &(asicEntry.val)[0]));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_acl_start_runtime_set(uint32 unit)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint8     regVal  = 0u;
    uint32    endAddr = 0u;

    do
    {
        if(false == dva_g_AclRuntimeSetStart)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_1byte_get(unit, DVA_ACL_SHARE_MEMORY_IN_USE_ADDR, &regVal));
            if(DVA_ACL_SHARE_MEMORY_IN_USE == regVal)
            {
                ret = -(RT_ERR_ACL_SRAM_NOT_AVL);
                continue;
            }
            else
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_1byte_set(unit, DVA_ACL_SHARE_MEMORY_IN_USE_ADDR, (uint8)DVA_ACL_SHARE_MEMORY_IN_USE));
                dva_g_AclRuntimeSetStart = true;
                /* Get SRAM shared memory start address */
                CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_SRAM_SHARE_START, &dva_g_aclShareMemoryCurAddr));
            }
        }
        /* Get SRAM shared memory end address */
        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_SRAM_SHARE_END, &endAddr));
        if(DVA_ACL_PIE_DATA_MAX_LEN > (endAddr - dva_g_aclShareMemoryCurAddr))
        {
            ret = -(RT_ERR_ACL_SRAM_NO_SPACE);
        }
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_acl_find_runtime_pie_entry(uint32 unit, uint32 index, dva_AclTblAccessPieEntry_t *pPieEntry)
{
    RtkApiRet ret        = RT_ERR_OK;
    uint32    i          = 0u;
    uint32    sramAddr   = dva_g_aclRuntimePieEntrySramAddr[index];
    uint32    sramVal    = 0u;
    uint32    dataRegNum = dva_table_list[DVA_PIE_TABLE].datareg_num;

    do
    {
        if(NULL == pPieEntry)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(0u == sramAddr)
        {
            ret = -(RT_ERR_ACL_PIE_ENTRY_NOT_FOUND);
            continue;
        }

        for(i = 0u; i < dataRegNum; i++)
        {
            CHK_FUN_BREAK(ret, dva_drv_indirect_sram_get(unit, sramAddr, &sramVal));
            sramAddr += 4u;
            pPieEntry->val[i] = sramVal;
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_acl_fill_org_crc(uint32 unit, uint32 block)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;
    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_pie_golden_crc(unit, block, DVA_TCAM_PIE_TCAM0, &regVal));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_set(unit, dva_g_aclShareMemoryCurAddr, regVal));
        dva_g_aclShareMemoryCurAddr += 4u;
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_pie_golden_crc(unit, block, DVA_TCAM_PIE_TCAM1, &regVal));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_set(unit, dva_g_aclShareMemoryCurAddr, regVal));
        dva_g_aclShareMemoryCurAddr += 4u;
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_acl_fill_calc_crc(uint32 unit, uint32 block)
{
    RtkApiRet                  ret             = RT_ERR_OK;
    uint32                     blockEntryIndex = 0u;
    uint32                     entryIndex      = 0u;
    dva_AclTblAccessPieEntry_t pieEntry        = {0};
    dva_drv_TcamPieCrcVal_t    curCrc          = {.val = {~0u, ~0u}};
    dva_drv_TcamPieCrcVal_t    goldenCrc       = {.val = {0u, 0u}};

    for(blockEntryIndex = 0u; blockEntryIndex < DVA_TCAM_PIE_BLOCK_ENTRY_NUM; blockEntryIndex++)
    {
        entryIndex = (block * DVA_TCAM_PIE_BLOCK_ENTRY_NUM) + blockEntryIndex;

        if(RT_ERR_OK != dva_drv_acl_internal_rule_id_check(unit, entryIndex))
        {
            continue;
        }

        if(0u != (dva_g_aclRuntimePieBlockEntryFlag[block] & (1ULL << blockEntryIndex)))
        {
            ret = dva_drv_acl_find_runtime_pie_entry(unit, entryIndex, &pieEntry);
        }
        else
        {
            ret = dva_table_read(unit, DVA_PIE_TABLE, entryIndex, &(pieEntry.val[0]));
        }

        CHK_FUN_RET(ret, dva_drv_tcam_pie_crc_calculate(&(pieEntry.val[0]), 1u, &curCrc, &goldenCrc));
        CHK_FUN_RET(ret, dva_drv_memcpy(&curCrc, &goldenCrc, sizeof(goldenCrc)));

        if(RT_ERR_OK != ret)
        {
            break;
        }
    }
    if(RT_ERR_OK == ret)
    {
        ret = dva_drv_indirect_sram_set(unit, dva_g_aclShareMemoryCurAddr, goldenCrc.val[0]);
        dva_g_aclShareMemoryCurAddr += 4u;
        CHK_FUN_RET(ret, dva_drv_indirect_sram_set(unit, dva_g_aclShareMemoryCurAddr, goldenCrc.val[1]));
        dva_g_aclShareMemoryCurAddr += 4u;
    }

    return ret;
}

static RtkApiRet dva_drv_acl_runtime_per_run_crc_calc(uint32 unit)
{
    RtkApiRet ret   = RT_ERR_OK;
    uint32    block = 0u;

    do
    {
        for(block = 0u; block <= DVA_TCAM_PIE_BLOCK_BOUND; block++)
        {
            if(0u == dva_g_aclRuntimePieBlockEntryFlag[block])
            {
                ret = dva_drv_acl_fill_org_crc(unit, block);
            }
            else
            {
                ret = dva_drv_acl_fill_calc_crc(unit, block);
            }

            if(RT_ERR_OK != ret)
            {
                break;
            }
        }
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_acl_runtime_pie_sram_set(uint32 unit, AclRuleId aclRuleIndex, dva_AclTblAccessPieEntry_t *pPieAsicEntry)
{
    RtkApiRet ret        = RT_ERR_OK;
    uint32    i          = 0u;
    uint32    dmaAddr    = 0u;
    uint32    dataRegNum = dva_table_list[DVA_PIE_TABLE].datareg_num;

    do
    {
        /* Mode 0: DMA address + entry data */
        /* Calculate the DMA address of ACL entry */
        dmaAddr = DVA_ACL_PIE_DMA_BASE_ADDR + (DVA_ACL_PIE_DMA_ACTUAL_DATA_REG_SIZE * aclRuleIndex);
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_set(unit, dva_g_aclShareMemoryCurAddr, dmaAddr));
        dva_g_aclShareMemoryCurAddr += DVA_ACL_CONFIG_TABLE_MODE0_INFO_BYTE;
        /* Recode entry data address */
        dva_g_aclRuntimePieEntrySramAddr[aclRuleIndex] = dva_g_aclShareMemoryCurAddr;

        for(i = 0u; i < dataRegNum; i++)
        {
            CHK_FUN_BREAK(ret, dva_drv_indirect_sram_set(unit, dva_g_aclShareMemoryCurAddr, pPieAsicEntry->val[i]));
            dva_g_aclShareMemoryCurAddr += 4u;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_runtime_add_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet                  ret                 = RT_ERR_OK;
    dva_AclTblAccessPieEntry_t asicEntry           = {0};
    uint32                     belongPieBlockIndex = 0u;
    uint32                     belongPieEntryIndex = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pPieParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /*
            1. Set dva_g_AclRuntimeSetStart to true
            2. Get shared memory start and end address
            3. Confirm whether there is sufficient space in the shared memory
        */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_start_runtime_set(unit));

        belongPieBlockIndex = aclRuleIndex / DVA_TCAM_PIE_BLOCK_ENTRY_NUM;
        belongPieEntryIndex = aclRuleIndex % DVA_TCAM_PIE_BLOCK_ENTRY_NUM;

        /* Entry is exist */
        if(0u != (dva_g_aclRuntimePieBlockEntryFlag[belongPieBlockIndex] & (1ULL << belongPieEntryIndex)))
        {
            ret = -(RT_ERR_ACL_DUP_ENTRYIDX);
            continue;
        }

        asicEntry.val[0]  = (((uint32)pPieParam->data0 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data1 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[1]  = (((uint32)pPieParam->data2 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data3 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[2]  = (((uint32)pPieParam->data4 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data5 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[3]  = (((uint32)pPieParam->data6 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data7 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[4]  = (((uint32)pPieParam->data8 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data9 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[5]  = (((uint32)pPieParam->data10 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data11 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[6]  = pPieParam->valid & dva_drv_tbl_fieldlen2mask(DVA_PIE_TABLE_TCAM_VALID_LEN);
        asicEntry.val[7]  = (((uint32)pPieParam->mask0 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask1 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[8]  = (((uint32)pPieParam->mask2 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask3 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[9]  = (((uint32)pPieParam->mask4 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask5 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[10] = (((uint32)pPieParam->mask6 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask7 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[11] = (((uint32)pPieParam->mask8 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask9 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[12] = (((uint32)pPieParam->mask10 & dva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask11 & dva_drv_tbl_fieldlen2mask(16u)) << 16u);
        CHK_FUN_CONTINUE(ret, dva_drv_acl_runtime_pie_sram_set(unit, aclRuleIndex, &asicEntry));
        /* Record entry index */
        dva_g_aclRuntimePieBlockEntryFlag[belongPieBlockIndex] |= 1ULL << (belongPieEntryIndex);
        /* Increase acl number */
        dva_g_aclNum++;

        /* When dva_g_aclNum is equal to the number per run */
        if(0u == (dva_g_aclNum % DVA_ACL_WRITE_ENTRY_NUM_PER_ROUND))
        {
            /* Calculate per run golden CRC value */
            CHK_FUN_CONTINUE(ret, dva_drv_acl_runtime_per_run_crc_calc(unit));
        }

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_acl_indac_trigger(uint32 unit, uint32 command)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    count   = 0u;
    uint32    trigger = 1u;
    uint32    error   = 0u;

    /* Set indirect access command */
    ret = dva_reg_write(unit, DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE, command);

    if(RT_ERR_OK == ret)
    {
        /* Polling trigger bit */
        while((1u == trigger) && (RT_ERR_OK == ret))
        {
            CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE, DVA_ACL_INDAC_TRIGGER_LSP, &trigger));
            if(0u == trigger)
            {
                continue;
            }
            else if(count >= DVA_ACL_INDAC_TRY_TIMES)
            {
                ret = -(RT_ERR_BUSYWAIT_TIMEOUT);
                continue;
            }
            else
            {
                CHK_FUN_CONTINUE(ret, rtk_usecond_delay(DVA_ACL_INDAC_DELAY_US));
                count++;
            }
        }
        CHK_FUN_RET(ret, dva_reg_bit_get(unit, DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE, DVA_ACL_INDAC_ERROR_LSP, &error));
        if((RT_ERR_OK == ret) && (1u == error))
        {
            ret = -(RT_ERR_ACL_INDAC_TRIGGER_FAIL);
        }
    }

    return ret;
}

static RtkApiRet dva_drv_acl_indac_wait_complete(uint32 unit)
{
    uint32    command = 0u;
    RtkApiRet ret     = RT_ERR_OK;
    uint32    count   = 0u;
    uint32    state   = DVA_ACL_INDAC_CORE_STATE_ONGOING;
    uint32    fail    = 0u;

    /* Polling core state */
    command = ((uint32)1u << DVA_ACL_INDAC_TRIGGER_LSP) | ((uint32)DVA_ACL_INDAC_TARGET_FSM << DVA_ACL_INDAC_FSM_LSP) | DVA_ACL_INDAC_CORE_OP_ONGOING;
    while((0u != state) && (RT_ERR_OK == ret))
    {
        CHK_FUN_CONTINUE(ret, dva_drv_acl_indac_trigger(unit, command));
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_OPCODE, DVA_ACL_INDAC_FSM_STATE_LSP, DVA_ACL_INDAC_FSM_STATE_LEN, &state));
        if(DVA_ACL_INDAC_CORE_STATE_IDLE == state)
        {
            continue;
        }
        else if(count >= DVA_ACL_INDAC_TRY_TIMES)
        {
            ret = -(RT_ERR_BUSYWAIT_TIMEOUT);
            continue;
        }
        else
        {
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(DVA_ACL_INDAC_DELAY_US));
            count++;
        }
    }

    CHK_FUN_RET(ret, dva_reg_bit_get(unit, DVA_ACL_INDAC_PIE_SET_STATUS, DVA_ACL_INDAC_PIE_SET_FAIL_LSP, &fail));

    if(RT_ERR_OK == ret)
    {
        /* Initialize global variables */
        dva_g_aclNum                = 0u;
        dva_g_aclShareMemoryCurAddr = 0u;
        dva_g_AclRuntimeSetStart    = false;
        if(1u == fail)
        {
            ret = -(RT_ERR_ACL_INDAC_EXE_FAIL);
        }
    }
    return ret;
}

RtkApiRet dva_drv_acl_runtime_trigger_pie_entries(uint32 unit)
{
    RtkApiRet   ret     = RT_ERR_OK;
    uint32      command = 0u;
    static bool isAdd   = false;

    do
    {
        if(dva_g_aclNum > 0u)
        {
            /* Add last run golden crc values */
            if((false == isAdd) && (0u != (dva_g_aclNum % DVA_ACL_WRITE_ENTRY_NUM_PER_ROUND)))
            {
                CHK_FUN_CONTINUE(ret, dva_drv_acl_runtime_per_run_crc_calc(unit));
                isAdd = true;
            }
            CHK_FUN_CONTINUE(ret, drv_memset(dva_g_aclRuntimePieBlockEntryFlag, 0, sizeof(dva_g_aclRuntimePieBlockEntryFlag)));
            CHK_FUN_CONTINUE(ret, drv_memset(dva_g_aclRuntimePieEntrySramAddr, 0, sizeof(dva_g_aclRuntimePieEntrySramAddr)));

            /* Trigger PIE entries setting start */
            CHK_FUN_CONTINUE(ret, dva_reg_write(unit, DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_ADDRESS, dva_g_aclNum));
            CHK_FUN_CONTINUE(ret, dva_reg_write(unit, DVA_CPU_INDIRECT_ACCESS_CHANNEL_0_DATA, DVA_ACL_WRITE_ENTRY_NUM_PER_ROUND));
            command = ((uint32)1u << DVA_ACL_INDAC_TRIGGER_LSP) | ((uint32)1u << DVA_ACL_INDAC_LOCK_LSP) | ((uint32)DVA_ACL_INDAC_TARGET_FSM << DVA_ACL_INDAC_FSM_LSP) | DVA_ACL_INDAC_ACL_OP_WRITE;
            CHK_FUN_CONTINUE(ret, dva_drv_acl_indac_trigger(unit, command));
            isAdd = false;
            /* Waiting indirect access complete */
            CHK_FUN_CONTINUE(ret, dva_drv_acl_indac_wait_complete(unit));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_1byte_set(unit, DVA_ACL_SHARE_MEMORY_IN_USE_ADDR, (uint8)DVA_ACL_SHARE_MEMORY_NOT_IN_USE));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_copytocpu_action_get(uint32 unit, AclRuleId aclRuleIndex, RtkPmap *copyCpuDpm)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    pieIndex = 0u;
    uint32    regAddr  = 0u;
    uint32    cpuMask  = 0u;
    RtkPmap   srcPmap  = 0u;
    RtkPmap   dstPmap  = 0u;

    do
    {
        pieIndex = aclRuleIndex / DVA_ACL_PIE_RULE_NUM;
        regAddr  = DVA_ACL_COPYTOCPU_CONTROL0 + (pieIndex * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr, DVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LSP, DVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LEN, &cpuMask));
        srcPmap = (cpuMask & (DVA_RTK_SYS_PMAP_ALL | DVA_RTK_CPU_PMAP));
        CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_SYS_TO_USER));
        *copyCpuDpm = dstPmap;
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_copytocpu_action_set(uint32 unit, AclRuleId aclRuleIndex, RtkPmap copyCpuDpm)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    pieIndex = 0u;
    uint32    regAddr  = 0u;
    RtkPmap   dstPmap  = 0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(copyCpuDpm, &dstPmap, DVA_EM_USER_TO_SYS));
        pieIndex = aclRuleIndex / DVA_ACL_PIE_RULE_NUM;
        regAddr  = DVA_ACL_COPYTOCPU_CONTROL0 + (pieIndex * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regAddr, DVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LSP, DVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LEN, (uint32)dstPmap));
    } while(0u == 1u);

    return retVal;
}

static void dva_drv_acl_otag_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessOtagOpParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw   = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
        pActParam->outVidCtl  = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 29u) & dva_drv_tbl_fieldlen2mask(2u));
        pActParam->outVidInfo = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 17u) & dva_drv_tbl_fieldlen2mask(12u));
        pActParam->outTagOp   = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 15u) & dva_drv_tbl_fieldlen2mask(2u));

        (*pInfoNum)++;
    }
}

static void dva_drv_acl_otag_aif_set(uint32 flag, const AclTblAccessOtagOpParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                    | ((uint32)(pActParam->outVidCtl & dva_drv_tbl_fieldlen2mask(2u)) << 29u)
                                                                    | ((uint32)(pActParam->outVidInfo & dva_drv_tbl_fieldlen2mask(12u)) << 17u)
                                                                    | ((uint32)(pActParam->outTagOp & dva_drv_tbl_fieldlen2mask(2u)) << 15u);
        (*pInfoNum)++;
    }
}

static void dva_drv_acl_itag_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessIntagOpParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw  = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
        pActParam->inVidCtl  = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 29u) & dva_drv_tbl_fieldlen2mask(2u));
        pActParam->inVidInfo = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 17u) & dva_drv_tbl_fieldlen2mask(12u));
        pActParam->inTagOp   = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 15u) & dva_drv_tbl_fieldlen2mask(2u));

        (*pInfoNum)++;
    }
}

static void dva_drv_acl_itag_aif_set(uint32 flag, const AclTblAccessIntagOpParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                    | ((uint32)(pActParam->inVidCtl & dva_drv_tbl_fieldlen2mask(2u)) << 29u)
                                                                    | ((uint32)(pActParam->inVidInfo & dva_drv_tbl_fieldlen2mask(12u)) << 17u)
                                                                    | ((uint32)(pActParam->inTagOp & dva_drv_tbl_fieldlen2mask(2u)) << 15u);
        (*pInfoNum)++;
    }
}

static void dva_drv_acl_vlan_prirmk_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessVlanPriorityRmkParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
        pActParam->tagSel   = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 27u) & dva_drv_tbl_fieldlen2mask(4u));
        pActParam->inPri    = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 24u) & dva_drv_tbl_fieldlen2mask(3u));
        pActParam->outPri   = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 21u) & dva_drv_tbl_fieldlen2mask(3u));
        pActParam->odei     = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 20u) & dva_drv_tbl_fieldlen2mask(1u));
        pActParam->idei     = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 19u) & dva_drv_tbl_fieldlen2mask(1u));

        (*pInfoNum)++;
    }
}

static void dva_drv_acl_vlan_prirmk_aif_set(uint32 flag, const AclTblAccessVlanPriorityRmkParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                    | ((uint32)(pActParam->tagSel & dva_drv_tbl_fieldlen2mask(4u)) << 27u)
                                                                    | ((uint32)(pActParam->inPri & dva_drv_tbl_fieldlen2mask(3u)) << 24u)
                                                                    | ((uint32)(pActParam->outPri & dva_drv_tbl_fieldlen2mask(3u)) << 21u)
                                                                    | ((uint32)(pActParam->odei & dva_drv_tbl_fieldlen2mask(1u)) << 20u)
                                                                    | ((uint32)(pActParam->idei & dva_drv_tbl_fieldlen2mask(1u)) << 19u);
        (*pInfoNum)++;
    }
}

static RtkApiRet dva_drv_acl_redir_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessRedirectParam_t *pActParam, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
        {
            pActParam->withdraw = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->opcode   = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 29u) & dva_drv_tbl_fieldlen2mask(2u));
            srcPmap             = ((RtkPmap)(pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 13u) & (RtkPmap)(DVA_RTK_SYS_PMAP_ALL | DVA_RTK_CPU_PMAP));
            CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_SYS_TO_USER));
            pActParam->dpm         = dstPmap;
            pActParam->bypassLaLut = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 12u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->crsvlan     = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 11u) & dva_drv_tbl_fieldlen2mask(1u));
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet dva_drv_acl_redir_aif_set(uint32 flag, const AclTblAccessRedirectParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
        {
            srcPmap = (RtkPmap)pActParam->dpm;
            CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_USER_TO_SYS));
            pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                        | ((uint32)(pActParam->opcode & dva_drv_tbl_fieldlen2mask(2u)) << 29u)
                                                                        | ((uint32)(dstPmap & dva_drv_tbl_fieldlen2mask(16u)) << 13u)
                                                                        | ((uint32)(pActParam->bypassLaLut & dva_drv_tbl_fieldlen2mask(1u)) << 12u)
                                                                        | ((uint32)(pActParam->crsvlan & dva_drv_tbl_fieldlen2mask(1u)) << 11u);
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet dva_drv_acl_egr_redir_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessEgrRedirectParam_t *pActParam, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
        {
            pActParam->withdraw = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->opcode   = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 29u) & dva_drv_tbl_fieldlen2mask(2u));
            srcPmap             = ((RtkPmap)(pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 13u) & (RtkPmap)(DVA_RTK_SYS_PMAP_ALL | DVA_RTK_CPU_PMAP));
            CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_SYS_TO_USER));
            pActParam->dpm         = dstPmap;
            pActParam->bypassLaLut = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 12u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->recDpm      = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 10u) & dva_drv_tbl_fieldlen2mask(2u));
            pActParam->actOpSel    = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 8u) & dva_drv_tbl_fieldlen2mask(2u));
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet dva_drv_acl_egr_redir_aif_set(uint32 flag, const AclTblAccessEgrRedirectParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(pActParam->dpm, &dstPmap, DVA_EM_USER_TO_SYS));
            pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                        | ((uint32)(pActParam->opcode & dva_drv_tbl_fieldlen2mask(2u)) << 29u)
                                                                        | ((uint32)(dstPmap & dva_drv_tbl_fieldlen2mask(16u)) << 13u)
                                                                        | ((uint32)(pActParam->bypassLaLut & dva_drv_tbl_fieldlen2mask(1u)) << 12u)
                                                                        | ((uint32)(pActParam->recDpm & dva_drv_tbl_fieldlen2mask(2u)) << 10u)
                                                                        | ((uint32)(pActParam->actOpSel & dva_drv_tbl_fieldlen2mask(2u)) << 8u);
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static void dva_drv_acl_dscp_rmk_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessDscpRmkParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->opcode = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 29u) & dva_drv_tbl_fieldlen2mask(2u));

        if(0u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 23u) & dva_drv_tbl_fieldlen2mask(6u));
        }
        else if(1u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 26u) & dva_drv_tbl_fieldlen2mask(3u));
        }
        else if(2u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 23u) & dva_drv_tbl_fieldlen2mask(3u));
        }
        else
        {
            /* do nothing */
        }
        (*pInfoNum)++;
    }
}

static void dva_drv_acl_dscp_rmk_aif_set(uint32 flag, const AclTblAccessDscpRmkParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    uint32 opcode        = 0u;
    uint32 dscpIpPrecDtr = 0u;

    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        opcode = (uint32)pActParam->opcode & dva_drv_tbl_fieldlen2mask(2u);
        if(0u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & dva_drv_tbl_fieldlen2mask(6u)) << 23u;
        }
        else if(1u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & dva_drv_tbl_fieldlen2mask(3u)) << 26u;
        }
        else if(2u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & dva_drv_tbl_fieldlen2mask(3u)) << 23u;
        }
        else
        {
            /* do nothing */
        }

        pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                    | ((uint32)(opcode) << 29u)
                                                                    | (dscpIpPrecDtr);

        (*pInfoNum)++;
    }
}

static void dva_drv_acl_fwd_prias_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessFwdPriorityParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
        pActParam->priority = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 28u) & dva_drv_tbl_fieldlen2mask(3u));
        (*pInfoNum)++;
    }
}

static void dva_drv_acl_fwd_prias_aif_set(uint32 flag, const AclTblAccessFwdPriorityParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                    | ((uint32)(pActParam->priority & dva_drv_tbl_fieldlen2mask(3u)) << 28u);
        (*pInfoNum)++;
    }
}

static void dva_drv_acl_routing_uc_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessUcParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
        pActParam->nhIdx    = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 19u) & dva_drv_tbl_fieldlen2mask(12u));
        (*pInfoNum)++;
    }
}

static void dva_drv_acl_routing_uc_aif_set(uint32 flag, const AclTblAccessUcParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                    | ((uint32)(pActParam->nhIdx & dva_drv_tbl_fieldlen2mask(12u)) << 19u);
        (*pInfoNum)++;
    }
}

static RtkApiRet dva_drv_acl_routing_mc_aif_get(uint32 flag, dva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessMcParam_t *pActParam, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
        {
            pActParam->withdraw = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 31u) & dva_drv_tbl_fieldlen2mask(1u));
            pActParam->rhIdx    = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 19u) & dva_drv_tbl_fieldlen2mask(12u));
            srcPmap             = ((pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] >> 3u) & dva_drv_tbl_fieldlen2mask(16u));
            CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_SYS_TO_USER));
            pActParam->eppDpm = dstPmap;
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet dva_drv_acl_routing_mc_aif_set(uint32 flag, const AclTblAccessMcParam_t *pActParam, dva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= DVA_ACL_AIF_DATA_REG_BOUND))
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_pmap_convert(pActParam->eppDpm, &dstPmap, DVA_EM_USER_TO_SYS));
            pAsicEntry->val[DVA_ACL_AIF_DATA_REG_BOUND - (*pInfoNum)] = ((uint32)(pActParam->withdraw & dva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                                                        | ((uint32)(pActParam->rhIdx & dva_drv_tbl_fieldlen2mask(12u)) << 19u)
                                                                        | ((uint32)(dstPmap & dva_drv_tbl_fieldlen2mask(16u)) << 3u);
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

RtkApiRet dva_drv_acl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessActParam_t *pActParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    dva_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_table_read(unit, DVA_ACTION_TABLE, aclRuleIndex, &(asicEntry.val)[0]));

        pActParam->fno          = ((asicEntry.val[3] >> DVA_ACTION_TABLE_FLOW_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_FLOW_LEN));
        pActParam->drop         = ((asicEntry.val[3] >> DVA_ACTION_TABLE_DROP_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DROP_LEN));
        pActParam->copytocpu    = ((asicEntry.val[3] >> DVA_ACTION_TABLE_CP2CPU_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_CP2CPU_LEN));
        pActParam->mirror1      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_MIRROR1_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR1_LEN));
        pActParam->mirror0      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_MIRROR0_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR0_LEN));
        pActParam->otag         = ((asicEntry.val[3] >> DVA_ACTION_TABLE_OTAG_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_OTAG_LEN));
        pActParam->itag         = ((asicEntry.val[3] >> DVA_ACTION_TABLE_ITAG_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_ITAG_LEN));
        pActParam->priormk      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_PRIRMK_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIRMK_LEN));
        pActParam->redir        = ((asicEntry.val[3] >> DVA_ACTION_TABLE_REDIR_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_REDIR_LEN));
        pActParam->dscprmk      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_DSCPRMK_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DSCPRMK_LEN));
        pActParam->prioasn      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_PRIORITY_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIORITY_LEN));
        pActParam->byratelmt    = ((asicEntry.val[3] >> DVA_ACTION_TABLE_BYRATELMT_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_BYRATELMT_LEN));
        pActParam->uc           = ((asicEntry.val[3] >> DVA_ACTION_TABLE_UC_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_UC_LEN));
        pActParam->mc           = ((asicEntry.val[3] >> DVA_ACTION_TABLE_MC_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MC_LEN));
        pActParam->timerDiscard = ((asicEntry.val[3] >> DVA_ACTION_TABLE_TIMER_DISCARD_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_TIMER_DISCARD_LEN));
        pActParam->headerDump   = ((asicEntry.val[3] >> DVA_ACTION_TABLE_HEADER_DUMP_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_HEADER_DUMP_LEN));

        if(1u == pActParam->copytocpu)
        {
            /* Retrieve copy DPM*/
            CHK_FUN_CONTINUE(ret, dva_drv_acl_copytocpu_action_get(unit, aclRuleIndex, &(pActParam->copyCpuDpm)));
        }

        /* Retrieve outer VLAN AIF */
        dva_drv_acl_otag_aif_get(pActParam->otag, &asicEntry, &pActParam->outTagOpInfo, &infoNum);
        /* Retrieve inner VLAN AIF */
        dva_drv_acl_itag_aif_get(pActParam->itag, &asicEntry, &pActParam->inTagOpInfo, &infoNum);
        /* Retrieve VLAN priority remarking AIF */
        dva_drv_acl_vlan_prirmk_aif_get(pActParam->priormk, &asicEntry, &pActParam->priRmkInfo, &infoNum);
        /* Retrieve redirect AIF */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_redir_aif_get(pActParam->redir, &asicEntry, &pActParam->redirInfo, &infoNum));
        /* Retrieve DSCP remarking AIF*/
        dva_drv_acl_dscp_rmk_aif_get(pActParam->dscprmk, &asicEntry, &pActParam->dscpRmkInfo, &infoNum);
        /* Retrieve packet priority AIF*/
        dva_drv_acl_fwd_prias_aif_get(pActParam->prioasn, &asicEntry, &pActParam->prioInfo, &infoNum);
        /* Retrieve routing unicast AIF*/
        dva_drv_acl_routing_uc_aif_get(pActParam->uc, &asicEntry, &pActParam->ucInfo, &infoNum);
        /* Retrieve routing multicast AIF*/
        CHK_FUN_CONTINUE(ret, dva_drv_acl_routing_mc_aif_get(pActParam->mc, &asicEntry, &pActParam->mcInfo, &infoNum));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_acl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessActParam_t *pActParam)
{
    dva_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0u;
    RtkApiRet                  ret       = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[3] = ((uint32)(pActParam->fno & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_FLOW_LEN)) << DVA_ACTION_TABLE_FLOW_LSP)
                           | ((uint32)(pActParam->drop & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DROP_LEN)) << DVA_ACTION_TABLE_DROP_LSP)
                           | ((uint32)(pActParam->copytocpu & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_CP2CPU_LEN)) << DVA_ACTION_TABLE_CP2CPU_LSP)
                           | ((uint32)(pActParam->mirror1 & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR1_LEN)) << DVA_ACTION_TABLE_MIRROR1_LSP)
                           | ((uint32)(pActParam->mirror0 & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR0_LEN)) << DVA_ACTION_TABLE_MIRROR0_LSP)
                           | ((uint32)(pActParam->otag & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_OTAG_LEN)) << DVA_ACTION_TABLE_OTAG_LSP)
                           | ((uint32)(pActParam->itag & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_ITAG_LEN)) << DVA_ACTION_TABLE_ITAG_LSP)
                           | ((uint32)(pActParam->priormk & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIRMK_LEN)) << DVA_ACTION_TABLE_PRIRMK_LSP)
                           | ((uint32)(pActParam->redir & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_REDIR_LEN)) << DVA_ACTION_TABLE_REDIR_LSP)
                           | ((uint32)(pActParam->dscprmk & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DSCPRMK_LEN)) << DVA_ACTION_TABLE_DSCPRMK_LSP)
                           | ((uint32)(pActParam->prioasn & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIORITY_LEN)) << DVA_ACTION_TABLE_PRIORITY_LSP)
                           | ((uint32)(pActParam->byratelmt & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_BYRATELMT_LEN)) << DVA_ACTION_TABLE_BYRATELMT_LSP)
                           | ((uint32)(pActParam->uc & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_UC_LEN)) << DVA_ACTION_TABLE_UC_LSP)
                           | ((uint32)(pActParam->mc & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MC_LEN)) << DVA_ACTION_TABLE_MC_LSP)
                           | ((uint32)(pActParam->timerDiscard & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_TIMER_DISCARD_LEN)) << DVA_ACTION_TABLE_TIMER_DISCARD_LSP)
                           | ((uint32)(pActParam->headerDump & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_HEADER_DUMP_LEN)) << DVA_ACTION_TABLE_HEADER_DUMP_LSP);

        if(1u == pActParam->copytocpu)
        {
            /* Configure copy DPM*/
            CHK_FUN_CONTINUE(ret, dva_drv_acl_copytocpu_action_set(unit, aclRuleIndex, pActParam->copyCpuDpm));
        }

        /* Configure outer VLAN AIF */
        dva_drv_acl_otag_aif_set(pActParam->otag, &pActParam->outTagOpInfo, &asicEntry, &infoNum);
        /* Configure inner VLAN AIF */
        dva_drv_acl_itag_aif_set(pActParam->itag, &pActParam->inTagOpInfo, &asicEntry, &infoNum);
        /* Configure VLAN priority remarking AIF */
        dva_drv_acl_vlan_prirmk_aif_set(pActParam->priormk, &pActParam->priRmkInfo, &asicEntry, &infoNum);
        /* Configure redirect AIF */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_redir_aif_set(pActParam->redir, &pActParam->redirInfo, &asicEntry, &infoNum));
        /* Configure DSCP remarking AIF*/
        dva_drv_acl_dscp_rmk_aif_set(pActParam->dscprmk, &pActParam->dscpRmkInfo, &asicEntry, &infoNum);
        /* Configure packet priority AIF*/
        dva_drv_acl_fwd_prias_aif_set(pActParam->prioasn, &pActParam->prioInfo, &asicEntry, &infoNum);
        /* Configure routing unicast AIF */
        dva_drv_acl_routing_uc_aif_set(pActParam->uc, &pActParam->ucInfo, &asicEntry, &infoNum);
        /* Configure routing multicast AIF */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_routing_mc_aif_set(pActParam->mc, &pActParam->mcInfo, &asicEntry, &infoNum));

        CHK_FUN_CONTINUE(ret, dva_table_write(unit, DVA_ACTION_TABLE, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_eacl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, EaclTblAccessActParam_t *pActParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    dva_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_table_read(unit, DVA_ACTION_TABLE, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

        pActParam->drop         = ((asicEntry.val[3] >> DVA_ACTION_TABLE_DROP_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DROP_LEN));
        pActParam->copytocpu    = ((asicEntry.val[3] >> DVA_ACTION_TABLE_CP2CPU_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_CP2CPU_LEN));
        pActParam->mirror1      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_MIRROR1_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR1_LEN));
        pActParam->mirror0      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_MIRROR0_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR0_LEN));
        pActParam->otag         = ((asicEntry.val[3] >> DVA_ACTION_TABLE_OTAG_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_OTAG_LEN));
        pActParam->itag         = ((asicEntry.val[3] >> DVA_ACTION_TABLE_ITAG_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_ITAG_LEN));
        pActParam->priormk      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_PRIRMK_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIRMK_LEN));
        pActParam->redir        = ((asicEntry.val[3] >> DVA_ACTION_TABLE_REDIR_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_REDIR_LEN));
        pActParam->dscprmk      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_DSCPRMK_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DSCPRMK_LEN));
        pActParam->prioasn      = ((asicEntry.val[3] >> DVA_ACTION_TABLE_PRIORITY_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIORITY_LEN));
        pActParam->timerDiscard = ((asicEntry.val[3] >> DVA_ACTION_TABLE_TIMER_DISCARD_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_TIMER_DISCARD_LEN));
        pActParam->headerDump   = ((asicEntry.val[3] >> DVA_ACTION_TABLE_HEADER_DUMP_LSP) & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_HEADER_DUMP_LEN));

        if(1u == pActParam->copytocpu)
        {
            /* Retrieve copy DPM*/
            CHK_FUN_CONTINUE(ret, dva_drv_acl_copytocpu_action_get(unit, aclRuleIndex, &(pActParam->copyCpuDpm)));
        }

        /* Retrieve outer VLAN AIF */
        dva_drv_acl_otag_aif_get(pActParam->otag, &asicEntry, &pActParam->outTagOpInfo, &infoNum);
        /* Retrieve inner VLAN AIF */
        dva_drv_acl_itag_aif_get(pActParam->itag, &asicEntry, &pActParam->inTagOpInfo, &infoNum);
        /* Retrieve VLAN priority remarking AIF */
        dva_drv_acl_vlan_prirmk_aif_get(pActParam->priormk, &asicEntry, &pActParam->priRmkInfo, &infoNum);
        /* Retrieve redirect AIF */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_egr_redir_aif_get(pActParam->redir, &asicEntry, &pActParam->egrRedirInfo, &infoNum));
        /* Retrieve DSCP remarking AIF*/
        dva_drv_acl_dscp_rmk_aif_get(pActParam->dscprmk, &asicEntry, &pActParam->dscpRmkInfo, &infoNum);
        /* Retrieve packet priority AIF*/
        dva_drv_acl_fwd_prias_aif_get(pActParam->prioasn, &asicEntry, &pActParam->prioInfo, &infoNum);
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_eacl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const EaclTblAccessActParam_t *pActParam)
{
    dva_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0u;
    RtkApiRet                  ret       = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[3] = ((uint32)(pActParam->drop & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DROP_LEN)) << DVA_ACTION_TABLE_DROP_LSP)
                           | ((uint32)(pActParam->copytocpu & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_CP2CPU_LEN)) << DVA_ACTION_TABLE_CP2CPU_LSP)
                           | ((uint32)(pActParam->mirror1 & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR1_LEN)) << DVA_ACTION_TABLE_MIRROR1_LSP)
                           | ((uint32)(pActParam->mirror0 & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_MIRROR0_LEN)) << DVA_ACTION_TABLE_MIRROR0_LSP)
                           | ((uint32)(pActParam->otag & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_OTAG_LEN)) << DVA_ACTION_TABLE_OTAG_LSP)
                           | ((uint32)(pActParam->itag & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_ITAG_LEN)) << DVA_ACTION_TABLE_ITAG_LSP)
                           | ((uint32)(pActParam->priormk & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIRMK_LEN)) << DVA_ACTION_TABLE_PRIRMK_LSP)
                           | ((uint32)(pActParam->redir & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_REDIR_LEN)) << DVA_ACTION_TABLE_REDIR_LSP)
                           | ((uint32)(pActParam->dscprmk & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_DSCPRMK_LEN)) << DVA_ACTION_TABLE_DSCPRMK_LSP)
                           | ((uint32)(pActParam->prioasn & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_PRIORITY_LEN)) << DVA_ACTION_TABLE_PRIORITY_LSP)
                           | ((uint32)(pActParam->timerDiscard & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_TIMER_DISCARD_LEN)) << DVA_ACTION_TABLE_TIMER_DISCARD_LSP)
                           | ((uint32)(pActParam->headerDump & dva_drv_tbl_fieldlen2mask(DVA_ACTION_TABLE_HEADER_DUMP_LEN)) << DVA_ACTION_TABLE_HEADER_DUMP_LSP);

        if(1u == pActParam->copytocpu)
        {
            /* Configure copy DPM*/
            CHK_FUN_CONTINUE(ret, dva_drv_acl_copytocpu_action_set(unit, aclRuleIndex, pActParam->copyCpuDpm));
        }

        /* Configure outer VLAN AIF */
        dva_drv_acl_otag_aif_set(pActParam->otag, &pActParam->outTagOpInfo, &asicEntry, &infoNum);
        /* Configure inner VLAN AIF */
        dva_drv_acl_itag_aif_set(pActParam->itag, &pActParam->inTagOpInfo, &asicEntry, &infoNum);
        /* Configure VLAN priority remarking AIF */
        dva_drv_acl_vlan_prirmk_aif_set(pActParam->priormk, &pActParam->priRmkInfo, &asicEntry, &infoNum);
        /* Configure redirect AIF */
        CHK_FUN_CONTINUE(ret, dva_drv_acl_egr_redir_aif_set(pActParam->redir, &pActParam->egrRedirInfo, &asicEntry, &infoNum));
        /* Configure DSCP remarking AIF*/
        dva_drv_acl_dscp_rmk_aif_set(pActParam->dscprmk, &pActParam->dscpRmkInfo, &asicEntry, &infoNum);
        /* Configure packet priority AIF*/
        dva_drv_acl_fwd_prias_aif_set(pActParam->prioasn, &pActParam->prioInfo, &asicEntry, &infoNum);

        CHK_FUN_CONTINUE(ret, dva_table_write(unit, DVA_ACTION_TABLE, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}

static uint64 dva_drv_acl_swap64(uint64 value)
{
    return ((value & 0xFF00000000000000u) >> 56u) | ((value & 0x00FF000000000000u) >> 40u) | ((value & 0x0000FF0000000000u) >> 24u) | ((value & 0x000000FF00000000u) >> 8u) | ((value & 0x00000000FF000000u) << 8u) | ((value & 0x0000000000FF0000u) << 24u) | ((value & 0x000000000000FF00u) << 40u) | ((value & 0x00000000000000FFu) << 56u);
}

RtkApiRet dva_drv_acl_ip6_range_check(const AclIpv6 *pUpper, const AclIpv6 *pLower)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint64    tmpVal       = 0u;
    uint64    ipv6_upper_h = 0u;
    uint64    ipv6_upper_l = 0u;
    uint64    ipv6_lower_h = 0u;
    uint64    ipv6_lower_l = 0u;

    do
    {
        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0x7766554433221100
            ipv6_upper_h = 0x0011223344556677
        */
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy((uint8 *)&tmpVal, &pUpper[0], 8u));
        ipv6_upper_h = dva_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0xffeeddccbbaa9988
            ipv6_upper_l = 0x8899aabbccddeeff
        */
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy((uint8 *)&tmpVal, &pUpper[8], 8u));
        ipv6_upper_l = dva_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0x7766554433221100
            ipv6_lower_h = 0x0011223344556677
        */
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy((uint8 *)&tmpVal, &pLower[0], 8u));
        ipv6_lower_h = dva_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0xffeeddccbbaa9988
            ipv6_lower_l = 0x8899aabbccddeeff
        */
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy((uint8 *)&tmpVal, &pLower[8], 8u));
        ipv6_lower_l = dva_drv_acl_swap64(tmpVal);

        if(ipv6_upper_h < ipv6_lower_h)
        {
            retVal = -(RT_ERR_ACL_RANGE_ERROR);
            continue;
        }

        if((ipv6_upper_h == ipv6_lower_h) && (ipv6_upper_l < ipv6_lower_l))
        {
            retVal = -(RT_ERR_ACL_RANGE_ERROR);
            continue;
        }

    } while(0u == 1u);
    return retVal;
}

RtkApiRet dva_drv_acl_ip6_range_set_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, const AclIpv6 *ipv6)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    fieldVal    = 0u;
    uint32    regAddr     = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        if(entryIdx > DVA_FILTER_IP6RANGE_BOUND)
        {
            retVal = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(boundType > DVA_ACL_IP6_RANGE_HBOUND_TYPE)
        {
            retVal = -(RT_ERR_ACL_BOUND_TYPE);
            continue;
        }

        if(NULL == ipv6)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(DVA_ACL_IP6_RANGE_LBOUND_TYPE == boundType)
        {
            regAddr = DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_3;
        }
        else
        {
            regAddr = DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_1_3;
        }

        /*
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_3 <-> ipv6[0]  to ipv6[3]
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_2 <-> ipv6[4]  to ipv6[7]
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_1 <-> ipv6[8]  to ipv6[11]
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_0 <-> ipv6[12] to ipv6[15]
        */
        for(i = 0; i <= rangeRegNum; i++)
        {
            /*
                IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
                ipv6[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_3: 0x00112233
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_2: 0x44556677
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_1: 0x8899AABB
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_0: 0xCCDDEEFF
            */
            fieldVal = (((uint32)ipv6[i * 4u]) << 24u) | (((uint32)ipv6[(i * 4u) + 1u]) << 16u) | (((uint32)ipv6[(i * 4u) + 2u]) << 8u) | (((uint32)ipv6[(i * 4u) + 3u]));
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regAddr - (i * 4u) + (entryIdx * 36u), DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LSP, DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LEN, fieldVal));
        }

    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_ip6_range_get_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, AclIpv6 *ipv6)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    fieldVal    = 0u;
    uint32    regAddr     = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        if(entryIdx > DVA_FILTER_IP6RANGE_BOUND)
        {
            retVal = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(boundType > DVA_ACL_IP6_RANGE_HBOUND_TYPE)
        {
            retVal = -(RT_ERR_ACL_BOUND_TYPE);
            continue;
        }

        if(NULL == ipv6)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(DVA_ACL_IP6_RANGE_LBOUND_TYPE == boundType)
        {
            regAddr = DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_3;
        }
        else
        {
            regAddr = DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_1_3;
        }

        /*
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_3 <-> ipv6[0]  to ipv6[3]
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_2 <-> ipv6[4]  to ipv6[7]
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_1 <-> ipv6[8]  to ipv6[11]
            IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_0 <-> ipv6[12] to ipv6[15]
        */
        for(i = 0; i <= rangeRegNum; i++)
        {
            /*
                IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
                ipv6[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_3: 0x00112233
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_2: 0x44556677
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_1: 0x8899AABB
                IPV6_RANGE_CHECKING_TABLE_ENTRY0_X_0: 0xCCDDEEFF
            */
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr - (i * 4u) + (entryIdx * 36u), DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LSP, DVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LEN, &fieldVal));
            ipv6[(i * 4u)]      = (uint8)((fieldVal & 0xff000000u) >> 24u);
            ipv6[(i * 4u) + 1u] = (uint8)((fieldVal & 0xff0000u) >> 16u);
            ipv6[(i * 4u) + 2u] = (uint8)((fieldVal & 0xff00u) >> 8u);
            ipv6[(i * 4u) + 3u] = (uint8)((fieldVal & 0xffu));
        }

    } while(0u == 1u);

    return retVal;
}

/********* ACL Internal Protocols *********/

/* ** MKA ** */
RtkApiRet dva_drv_acl_config_mka(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MKA;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ether type */
        pieParam.data5 = 0x0005u;
        pieParam.mask5 = 0x00FFu;
        pieParam.data6 = 0x888eu;
        pieParam.mask6 = 0xFFFFu;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}
/* ** MKA END ** */

/* ** Dot1X ** */
RtkApiRet dva_drv_acl_config_dot1x_arp(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_DOT1X_ARP;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ARP and DIP match [15] */
        pieParam.data7 = 0x8000u;
        pieParam.mask7 = 0x8000u;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_dot1x_radius(uint32 unit, uint16 udpPort)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_DOT1X_RADIUS;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting L4 format: UDP, [3:1] l4fmt */
        pieParam.data7 = 0x0002u;
        pieParam.mask7 = 0x000Eu;
        /* Setting L4 sport: RAD port */
        pieParam.data9 = 1812u;
        pieParam.mask9 = 0xFFFEu;
        /* Setting L4 dport */
        pieParam.data10 = udpPort;
        pieParam.mask10 = 0xFFFFu;
        pieParam.valid  = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_dot1x_eap(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_DOT1X_EAP;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ether type */
        pieParam.data6 = 0x888eu;
        pieParam.mask6 = 0xFFFFu;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}
/* ** Dot1X END ** */

/* ** Routing ** */
RtkApiRet dva_drv_acl_config_route_arp(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_ROUTE_ARP;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting L3 format: ARP, [5:4] l3fmt */
        pieParam.data7 = 0x0000u;
        pieParam.mask7 = 0x0030u;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting copy to cpu action */
        actParam.copytocpu  = 1u;
        actParam.copyCpuDpm = 0x8000u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_route_ns(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_ROUTE_ICMP6_NS;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ICMP type, [7:0] hsb_icmp_type */
        pieParam.data8 = 0x0087u;
        pieParam.mask8 = 0x00FFu;
        /* Setting IP header: ICMPv6, [7:0] protocol */
        pieParam.data11 = 0x003Au;
        pieParam.mask11 = 0x00FFu;
        pieParam.valid  = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting copy to cpu action */
        actParam.copytocpu  = 1u;
        actParam.copyCpuDpm = 0x8000u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_route_na(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_ROUTE_ICMP6_NA;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ICMP type, [7:0] hsb_icmp_type */
        pieParam.data8 = 0x0088u;
        pieParam.mask8 = 0x00FFu;
        /* Setting IP header: ICMPv6, [7:0] protocol */
        pieParam.data11 = 0x003Au;
        pieParam.mask11 = 0x00FFu;
        pieParam.valid  = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting copy to cpu action */
        actParam.copytocpu  = 1u;
        actParam.copyCpuDpm = 0x8000u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_route_uc_lk_miss(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_ROUTE_UC_LK_MISS;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_template_setting(unit, DVA_ACL_TEMPLATE16, 3u, DVA_EACL_FLD_PPI_LKMISS));
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_template_setting(unit, DVA_ACL_TEMPLATE16, 4u, DVA_EACL_FLD_PPI_3_ROUTE_UC_MISS));

        /* DMAC is unicast, mac[40] != 1 */
        pieParam.data2 = 0x0000u;
        pieParam.mask2 = 0x0100u;
        /* [0] L2_TBL_DA_LK_MISS L2 Table DA lookup miss or not */
        /* L2 DMAC lookup miss */
        pieParam.data3 = 0x0001u;
        pieParam.mask3 = 0x0001u;
        /* [9] ROUTE_IGTBL_HIT Ingress Interface Table that lookup hit */
        /* Ingress interface not hit */
        pieParam.data4 = 0x0000u;
        pieParam.mask4 = 0x0200u;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting drop action */
        actParam.drop = 1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}
/* ** Routing END ** */

/* ** IGMP ** */
RtkApiRet dva_drv_acl_config_igmp(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_IGMP4;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting IP header: IGMPv4, [7:0] protocol */
        pieParam.data11 = 0x0002u;
        pieParam.mask11 = 0x00FFu;
        pieParam.valid  = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_mld_query(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MLD_ICMP6_QUERY;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ICMP type, [7:0] hsb_icmp_type */
        pieParam.data8 = 0x0082u;
        pieParam.mask8 = 0x00FFu;
        /* Setting IP header: ICMPv6, [7:0] protocol */
        pieParam.data11 = 0x003Au;
        pieParam.mask11 = 0x00FFu;
        pieParam.valid  = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_mld_report(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MLD_ICMP6_REPORT;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ICMP type, [7:0] hsb_icmp_type */
        pieParam.data8 = 0x0083u;
        pieParam.mask8 = 0x00FFu;
        /* Setting IP header: ICMPv6, [7:0] protocol */
        pieParam.data11 = 0x003Au;
        pieParam.mask11 = 0x00FFu;
        pieParam.valid  = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_mld_done(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MLD_ICMP6_DONE;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting ICMP type, [7:0] hsb_icmp_type */
        pieParam.data8 = 0x0084u;
        pieParam.mask8 = 0x00FFu;
        /* Setting IP header: ICMPv6, [7:0] protocol */
        pieParam.data11 = 0x003Au;
        pieParam.mask11 = 0x00FFu;
        pieParam.valid  = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}
/* ** IGMP END ** */

/* ** STP ** */
RtkApiRet dva_drv_acl_config_stp(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_STP;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting DMAC: 01:80:C2:00:00:00 */
        pieParam.data0 = 0x0000u;
        pieParam.mask0 = 0xFFFFu;
        pieParam.data1 = 0xC200u;
        pieParam.mask1 = 0xFFFFu;
        pieParam.data2 = 0x0180u;
        pieParam.mask2 = 0xFFFFu;
        /* Setting ether type, bit [15:11] shall be 0 if ethertype is length (<=0x600) */
        pieParam.data6 = 0x0000u;
        pieParam.mask6 = 0xF800u;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}
/* ** STP END ** */

/* ** SRP ** */
RtkApiRet dva_drv_acl_config_msrp(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MSRP;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting DMAC: 01:80:C2:00:00:0E */
        pieParam.data0 = 0x000Eu;
        pieParam.mask0 = 0xFFFFu;
        pieParam.data1 = 0xC200u;
        pieParam.mask1 = 0xFFFFu;
        pieParam.data2 = 0x0180u;
        pieParam.mask2 = 0xFFFFu;
        /* Setting ether type: 0x22EA */
        pieParam.data6 = 0x22EAu;
        pieParam.mask6 = 0xFFFFu;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_mvrp_customer(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MVRP_CUSTOMER;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting DMAC: 01:80:C2:00:00:21 */
        pieParam.data0 = 0x0021u;
        pieParam.mask0 = 0xFFFFu;
        pieParam.data1 = 0xC200u;
        pieParam.mask1 = 0xFFFFu;
        pieParam.data2 = 0x0180u;
        pieParam.mask2 = 0xFFFFu;
        /* Setting ether type: 0x88F5 */
        pieParam.data6 = 0x88F5u;
        pieParam.mask6 = 0xFFFFu;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_mvrp_provider(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MVRP_PROVIDER;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting DMAC: 01:80:C2:00:00:0D */
        pieParam.data0 = 0x000Du;
        pieParam.mask0 = 0xFFFFu;
        pieParam.data1 = 0xC200u;
        pieParam.mask1 = 0xFFFFu;
        pieParam.data2 = 0x0180u;
        pieParam.mask2 = 0xFFFFu;
        /* Setting ether type: 0x88F5 */
        pieParam.data6 = 0x88F5u;
        pieParam.mask6 = 0xFFFFu;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_acl_config_mmrp(uint32 unit)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    AclRuleId               aclRuleIndex = DVA_ACL_MMRP;
    RtkApiRet               retVal       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));

        /* Setting DMAC: 01:80:C2:00:00:20 */
        pieParam.data0 = 0x0020u;
        pieParam.mask0 = 0xFFFFu;
        pieParam.data1 = 0xC200u;
        pieParam.mask1 = 0xFFFFu;
        pieParam.data2 = 0x0180u;
        pieParam.mask2 = 0xFFFFu;
        /* Setting ether type: 0x88F6 */
        pieParam.data6 = 0x88F6u;
        pieParam.mask6 = 0xFFFFu;
        pieParam.valid = 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));

        /* Setting redirect action */
        actParam.redir = 1u;
        /* Trap to cpu port15 */
        actParam.egrRedirInfo.dpm = 0x8000u;
        /* Trap to CPU (with CPU tag) */
        actParam.egrRedirInfo.opcode   = 1u;
        actParam.egrRedirInfo.withdraw = 0u;
        /* Setting packet priority action */
        actParam.prioasn           = 1u;
        actParam.prioInfo.priority = DVA_ACL_PROTOCOL_PRIORITY;
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}
/* ** SRP END ** */

RtkApiRet dva_drv_acl_config_clear(uint32 unit, AclRuleId aclRuleIndex)
{
    AclTblAccessPieParam_t  pieParam;
    EaclTblAccessActParam_t actParam;
    RtkApiRet               retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&pieParam, 0, sizeof(pieParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&actParam, 0, sizeof(actParam)));
        CHK_FUN_CONTINUE(retVal, dva_drv_acl_setting_pie_entry(unit, aclRuleIndex, &pieParam));
        CHK_FUN_CONTINUE(retVal, dva_drv_eacl_setting_action_entry(unit, aclRuleIndex, &actParam));
    } while(0u == 1u);

    return retVal;
}
