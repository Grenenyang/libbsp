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
#include "rtl907xc_va_api_acl.h"
#include "rtl907xc_va_asicdrv_acl.h"
#include "rtl907xc_va_asicdrv_tcam.h"

RtkApiRet cva_drv_acl_userport_2_sysport(RtkPort userPort, RtkPort *pSysPort)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((userPort != (RtkPort)CVA_RTK_CPU_PORT) && ((userPort > (RtkPort)CVA_RTK_USER_PORT_END) || (userPort < (RtkPort)CVA_RTK_USER_PORT_1ST)))
    {
        retVal = -(RT_ERR_PORT_ID);
    }

    *pSysPort = (RtkPort)cva_g_userPort2sysPort[userPort];

    return retVal;
}

RtkApiRet cva_drv_acl_userpmap_2_syspmap(RtkPmap userPmap, RtkPmap *pSysPmap)
{
    RtkApiRet retVal = RT_ERR_OK;
    retVal           = cva_drv_pmap_convert(userPmap, pSysPmap, CVA_EM_USER_TO_SYS);

    return retVal;
}

RtkApiRet cva_drv_acl_internal_rule_id_check(uint32 unit, AclRuleId aclRuleIndex)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* PIE33 cannot use */
        if((aclRuleIndex >= CVA_ACL_INT_RULE_PIE33_LBOUND) && (aclRuleIndex <= CVA_ACL_INT_RULE_PIE33_HBOUND))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        /* PIE35 cannot use */
        if((aclRuleIndex >= CVA_ACL_INT_RULE_PIE35_LBOUND) && (aclRuleIndex <= CVA_ACL_INT_RULE_PIE35_HBOUND))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(aclRuleIndex > CVA_ACL_INT_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_acl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType)
{
    uint32 regAddr  = 0u;
    uint32 regValue = 0u;
    uint32 regField = 0u;

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > CVA_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > CVA_ACL_TEMPLATE_FIELD_BOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_IDX);
            continue;
        }

        if(NULL == pType)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        /* internal TG 16 - 17 */
        if(tpIdx > CVA_ACL_TEMPLATE_BOUND)
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVA_USER_DEFINED_TEMPLATE_CONTROL48 + (uint32)((((tpIdx - (CVA_ACL_TEMPLATE_BOUND + 1u)) * 3u) + (fieldIdx / 4u)) * 4u);
        }
        /* external TG 0 - 15 */
        else
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVA_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        }
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = CVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, regAddr, regField, CVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, &regValue));
        *pType = regValue;
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_acl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type)
{
    uint32    regAddr  = 0u;
    uint32    regField = 0u;
    RtkApiRet ret      = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > CVA_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > CVA_ACL_TEMPLATE_FIELD_BOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_IDX);
            continue;
        }

        if(type > CVA_IACL_FLD_HBOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_TYPE);
            continue;
        }

        /* internal TG 16 - 17 */
        if(tpIdx > CVA_ACL_TEMPLATE_BOUND)
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVA_USER_DEFINED_TEMPLATE_CONTROL48 + (uint32)((((tpIdx - (CVA_ACL_TEMPLATE_BOUND + 1u)) * 3u) + (fieldIdx / 4u)) * 4u);
        }
        /* external TG 0 - 15 */
        else
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVA_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        }
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = CVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, regAddr, regField, CVA_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, type));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_acl_getting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    cva_AclTblAccessPieEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pPieParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, cva_table_read(unit, CVA_PIE_TABLE, aclRuleIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, cva_drv_drv_memset(pPieParam, 0, sizeof(AclTblAccessPieParam_t)));
        pPieParam->data0  = (uint16)((asicEntry.val[0]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data1  = (uint16)((asicEntry.val[0] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data2  = (uint16)((asicEntry.val[1]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data3  = (uint16)((asicEntry.val[1] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data4  = (uint16)((asicEntry.val[2]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data5  = (uint16)((asicEntry.val[2] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data6  = (uint16)((asicEntry.val[3]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data7  = (uint16)((asicEntry.val[3] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data8  = (uint16)((asicEntry.val[4]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data9  = (uint16)((asicEntry.val[4] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data10 = (uint16)((asicEntry.val[5]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->data11 = (uint16)((asicEntry.val[5] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->valid  = asicEntry.val[6] & cva_drv_tbl_fieldlen2mask(CVA_PIE_TABLE_VALID_INDIRECT_LEN);
        pPieParam->mask0  = (uint16)((asicEntry.val[7]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask1  = (uint16)((asicEntry.val[7] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask2  = (uint16)((asicEntry.val[8]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask3  = (uint16)((asicEntry.val[8] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask4  = (uint16)((asicEntry.val[9]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask5  = (uint16)((asicEntry.val[9] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask6  = (uint16)((asicEntry.val[10]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask7  = (uint16)((asicEntry.val[10] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask8  = (uint16)((asicEntry.val[11]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask9  = (uint16)((asicEntry.val[11] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask10 = (uint16)((asicEntry.val[12]) & cva_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask11 = (uint16)((asicEntry.val[12] >> 16u) & cva_drv_tbl_fieldlen2mask(16u));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_acl_setting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    cva_AclTblAccessPieEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pPieParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[0]  = (((uint32)pPieParam->data0 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data1 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[1]  = (((uint32)pPieParam->data2 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data3 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[2]  = (((uint32)pPieParam->data4 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data5 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[3]  = (((uint32)pPieParam->data6 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data7 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[4]  = (((uint32)pPieParam->data8 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data9 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[5]  = (((uint32)pPieParam->data10 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data11 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[6]  = pPieParam->valid & cva_drv_tbl_fieldlen2mask(CVA_PIE_TABLE_VALID_INDIRECT_LEN);
        asicEntry.val[7]  = (((uint32)pPieParam->mask0 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask1 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[8]  = (((uint32)pPieParam->mask2 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask3 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[9]  = (((uint32)pPieParam->mask4 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask5 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[10] = (((uint32)pPieParam->mask6 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask7 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[11] = (((uint32)pPieParam->mask8 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask9 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[12] = (((uint32)pPieParam->mask10 & cva_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask11 & cva_drv_tbl_fieldlen2mask(16u)) << 16u);

        CHK_FUN_CONTINUE(ret, cva_drv_tcam_write(unit, CVA_PIE_TABLE, aclRuleIndex, &(asicEntry.val)[0]));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_acl_copytocpu_action_get(uint32 unit, AclRuleId aclRuleIndex, RtkPmap *copyCpuDpm)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    pieIndex = 0u;
    uint32    regAddr  = 0u;
    uint32    cpuMask  = 0u;
    RtkPmap   srcPmap  = 0u;
    RtkPmap   dstPmap  = 0u;

    do
    {
        pieIndex = aclRuleIndex / CVA_ACL_PIE_RULE_NUM;
        if((pieIndex == 0u) || (pieIndex == 1u))
        {
            /* CVA_ACL_COPYTOCPU_CONTROL0 in the specific register address */
            regAddr = CVA_ACL_COPYTOCPU_CONTROL0;
        }
        else
        {
            /*
                -2u: pieIndex 0 and 1 is belong to CVA_ACL_COPYTOCPU_CONTROL0
                /2u: one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks
                *4u: 4 bytes register
            */
            regAddr = CVA_ACL_COPYTOCPU_CONTROL1 + (((pieIndex - 2u) / 2u) * 4u);
        }
        /* one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks */
        if(0u == (pieIndex % 2u))
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, regAddr, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LSP, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LEN, &cpuMask));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, regAddr, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LSP, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LEN, &cpuMask));
        }
        srcPmap = (cpuMask & (CVA_RTK_SYS_PMAP_ALL | CVA_RTK_CPU_PMAP));
        CHK_FUN_CONTINUE(retVal, cva_drv_pmap_convert(srcPmap, &dstPmap, CVA_EM_SYS_TO_USER));
        *copyCpuDpm = dstPmap;
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cva_drv_acl_copytocpu_action_set(uint32 unit, AclRuleId aclRuleIndex, RtkPmap copyCpuDpm)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    pieIndex = 0u;
    uint32    regAddr  = 0u;
    RtkPmap   dstPmap  = 0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_pmap_convert(copyCpuDpm, &dstPmap, CVA_EM_USER_TO_SYS));
        pieIndex = aclRuleIndex / CVA_ACL_PIE_RULE_NUM;
        if((pieIndex == 0u) || (pieIndex == 1u))
        {
            /* CVA_ACL_COPYTOCPU_CONTROL0 in the specific register address */
            regAddr = CVA_ACL_COPYTOCPU_CONTROL0;
        }
        else
        {
            /*
                -2u: pieIndex 0 and 1 is belong to CVA_ACL_COPYTOCPU_CONTROL0
                /2u: one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks
                *4u: 4 bytes register
            */
            regAddr = CVA_ACL_COPYTOCPU_CONTROL1 + (((pieIndex - 2u) / 2u) * 4u);
        }
        /* one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks */
        if(0u == (pieIndex % 2u))
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, regAddr, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LSP, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LEN, (uint32)dstPmap));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, regAddr, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LSP, CVA_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LEN, (uint32)dstPmap));
        }
    } while(0u == 1u);

    return retVal;
}

static void cva_drv_acl_otag_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessOtagOpParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw   = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
        pActParam->outVidCtl  = ((pAsicEntry->val[*pInfoNum] >> 29u) & cva_drv_tbl_fieldlen2mask(2u));
        pActParam->outVidInfo = ((pAsicEntry->val[*pInfoNum] >> 17u) & cva_drv_tbl_fieldlen2mask(12u));
        pActParam->outTagOp   = ((pAsicEntry->val[*pInfoNum] >> 15u) & cva_drv_tbl_fieldlen2mask(2u));

        (*pInfoNum)++;
    }
}

static void cva_drv_acl_otag_aif_set(uint32 flag, const AclTblAccessOtagOpParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->outVidCtl & cva_drv_tbl_fieldlen2mask(2u)) << 29u)
                                     | ((uint32)(pActParam->outVidInfo & cva_drv_tbl_fieldlen2mask(12u)) << 17u)
                                     | ((uint32)(pActParam->outTagOp & cva_drv_tbl_fieldlen2mask(2u)) << 15u);
        (*pInfoNum)++;
    }
}

static void cva_drv_acl_itag_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessIntagOpParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw  = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
        pActParam->inVidCtl  = ((pAsicEntry->val[*pInfoNum] >> 29u) & cva_drv_tbl_fieldlen2mask(2u));
        pActParam->inVidInfo = ((pAsicEntry->val[*pInfoNum] >> 17u) & cva_drv_tbl_fieldlen2mask(12u));
        pActParam->inTagOp   = ((pAsicEntry->val[*pInfoNum] >> 15u) & cva_drv_tbl_fieldlen2mask(2u));

        (*pInfoNum)++;
    }
}

static void cva_drv_acl_itag_aif_set(uint32 flag, const AclTblAccessIntagOpParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->inVidCtl & cva_drv_tbl_fieldlen2mask(2u)) << 29u)
                                     | ((uint32)(pActParam->inVidInfo & cva_drv_tbl_fieldlen2mask(12u)) << 17u)
                                     | ((uint32)(pActParam->inTagOp & cva_drv_tbl_fieldlen2mask(2u)) << 15u);
        (*pInfoNum)++;
    }
}

static void cva_drv_acl_vlan_prirmk_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessVlanPriorityRmkParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
        pActParam->tagSel   = ((pAsicEntry->val[*pInfoNum] >> 27u) & cva_drv_tbl_fieldlen2mask(4u));
        pActParam->inPri    = ((pAsicEntry->val[*pInfoNum] >> 24u) & cva_drv_tbl_fieldlen2mask(3u));
        pActParam->outPri   = ((pAsicEntry->val[*pInfoNum] >> 21u) & cva_drv_tbl_fieldlen2mask(3u));
        pActParam->odei     = ((pAsicEntry->val[*pInfoNum] >> 20u) & cva_drv_tbl_fieldlen2mask(1u));
        pActParam->idei     = ((pAsicEntry->val[*pInfoNum] >> 19u) & cva_drv_tbl_fieldlen2mask(1u));

        (*pInfoNum)++;
    }
}

static void cva_drv_acl_vlan_prirmk_aif_set(uint32 flag, const AclTblAccessVlanPriorityRmkParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->tagSel & cva_drv_tbl_fieldlen2mask(4u)) << 27u)
                                     | ((uint32)(pActParam->inPri & cva_drv_tbl_fieldlen2mask(3u)) << 24u)
                                     | ((uint32)(pActParam->outPri & cva_drv_tbl_fieldlen2mask(3u)) << 21u)
                                     | ((uint32)(pActParam->odei & cva_drv_tbl_fieldlen2mask(1u)) << 20u)
                                     | ((uint32)(pActParam->idei & cva_drv_tbl_fieldlen2mask(1u)) << 19u);
        (*pInfoNum)++;
    }
}

static RtkApiRet cva_drv_acl_redir_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessRedirectParam_t *pActParam, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
        {
            pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
            pActParam->opcode   = ((pAsicEntry->val[*pInfoNum] >> 29u) & cva_drv_tbl_fieldlen2mask(2u));
            pActParam->crsvlan  = ((pAsicEntry->val[*pInfoNum] >> 28u) & cva_drv_tbl_fieldlen2mask(1u));
            srcPmap             = ((RtkPmap)(pAsicEntry->val[*pInfoNum] >> 12u) & (RtkPmap)(CVA_RTK_SYS_PMAP_ALL | CVA_RTK_CPU_PMAP));
            CHK_FUN_CONTINUE(retVal, cva_drv_pmap_convert(srcPmap, &dstPmap, CVA_EM_SYS_TO_USER));
            pActParam->dpm         = dstPmap;
            pActParam->bypassLaLut = ((pAsicEntry->val[*pInfoNum] >> 11u) & cva_drv_tbl_fieldlen2mask(1u));

            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_acl_redir_aif_set(uint32 flag, const AclTblAccessRedirectParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
        {
            srcPmap = (RtkPmap)pActParam->dpm;
            CHK_FUN_CONTINUE(retVal, cva_drv_pmap_convert(srcPmap, &dstPmap, CVA_EM_USER_TO_SYS));
            pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                         | ((uint32)(pActParam->opcode & cva_drv_tbl_fieldlen2mask(2u)) << 29u)
                                         | ((uint32)(pActParam->crsvlan & cva_drv_tbl_fieldlen2mask(1u)) << 28u)
                                         | ((uint32)(dstPmap & cva_drv_tbl_fieldlen2mask(16u)) << 12u)
                                         | ((uint32)(pActParam->bypassLaLut & cva_drv_tbl_fieldlen2mask(1u)) << 11u);

            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static void cva_drv_acl_dscp_rmk_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessDscpRmkParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->opcode = ((pAsicEntry->val[*pInfoNum] >> 29u) & cva_drv_tbl_fieldlen2mask(2u));

        if(0u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[*pInfoNum] >> 23u) & cva_drv_tbl_fieldlen2mask(6u));
        }
        else if(1u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[*pInfoNum] >> 26u) & cva_drv_tbl_fieldlen2mask(3u));
        }
        else if(2u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[*pInfoNum] >> 23u) & cva_drv_tbl_fieldlen2mask(3u));
        }
        else
        {
            /* do nothing */
        }
        (*pInfoNum)++;
    }
}

static void cva_drv_acl_dscp_rmk_aif_set(uint32 flag, const AclTblAccessDscpRmkParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    uint32 opcode        = 0u;
    uint32 dscpIpPrecDtr = 0u;

    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        opcode = (uint32)pActParam->opcode & cva_drv_tbl_fieldlen2mask(2u);
        if(0u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & cva_drv_tbl_fieldlen2mask(6u)) << 23u;
        }
        else if(1u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & cva_drv_tbl_fieldlen2mask(3u)) << 26u;
        }
        else if(2u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & cva_drv_tbl_fieldlen2mask(3u)) << 23u;
        }
        else
        {
            /* do nothing */
        }

        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(opcode) << 29u)
                                     | (dscpIpPrecDtr);

        (*pInfoNum)++;
    }
}

static void cva_drv_acl_fwd_prias_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessFwdPriorityParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
        pActParam->priority = ((pAsicEntry->val[*pInfoNum] >> 28u) & cva_drv_tbl_fieldlen2mask(3u));
        (*pInfoNum)++;
    }
}

static void cva_drv_acl_fwd_prias_aif_set(uint32 flag, const AclTblAccessFwdPriorityParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->priority & cva_drv_tbl_fieldlen2mask(3u)) << 28u);
        (*pInfoNum)++;
    }
}

static void cva_drv_acl_routing_uc_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessUcParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
        pActParam->nhIdx    = ((pAsicEntry->val[*pInfoNum] >> 19u) & cva_drv_tbl_fieldlen2mask(12u));
        (*pInfoNum)++;
    }
}

static void cva_drv_acl_routing_uc_aif_set(uint32 flag, const AclTblAccessUcParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->nhIdx & cva_drv_tbl_fieldlen2mask(12u)) << 19u);
        (*pInfoNum)++;
    }
}

static RtkApiRet cva_drv_acl_routing_mc_aif_get(uint32 flag, cva_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessMcParam_t *pActParam, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
        {
            pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cva_drv_tbl_fieldlen2mask(1u));
            pActParam->rhIdx    = ((pAsicEntry->val[*pInfoNum] >> 19u) & cva_drv_tbl_fieldlen2mask(12u));
            srcPmap             = ((pAsicEntry->val[*pInfoNum] >> 3u) & cva_drv_tbl_fieldlen2mask(16u));
            CHK_FUN_CONTINUE(retVal, cva_drv_pmap_convert(srcPmap, &dstPmap, CVA_EM_SYS_TO_USER));
            pActParam->eppDpm = dstPmap;
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_acl_routing_mc_aif_set(uint32 flag, const AclTblAccessMcParam_t *pActParam, cva_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVA_ACL_AIF_DATA_REG_BOUND))
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_pmap_convert(pActParam->eppDpm, &dstPmap, CVA_EM_USER_TO_SYS));
            pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cva_drv_tbl_fieldlen2mask(1u)) << 31u)
                                         | ((uint32)(pActParam->rhIdx & cva_drv_tbl_fieldlen2mask(12u)) << 19u)
                                         | ((uint32)(dstPmap & cva_drv_tbl_fieldlen2mask(16u)) << 3u);
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

RtkApiRet cva_drv_acl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessActParam_t *pActParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    cva_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, cva_table_read(unit, CVA_ACTION_TABLE, aclRuleIndex, &(asicEntry.val)[0]));

        pActParam->fno          = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_FLOW_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_FLOW_LEN));
        pActParam->drop         = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_DROP_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_DROP_LEN));
        pActParam->copytocpu    = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_CP2CPU_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_CP2CPU_LEN));
        pActParam->mirror1      = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_MIRROR1_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_MIRROR1_LEN));
        pActParam->mirror0      = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_MIRROR0_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_MIRROR0_LEN));
        pActParam->otag         = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_OTAG_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_OTAG_LEN));
        pActParam->itag         = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_ITAG_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_ITAG_LEN));
        pActParam->priormk      = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_PRIRMK_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_PRIRMK_LEN));
        pActParam->redir        = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_REDIR_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_REDIR_LEN));
        pActParam->dscprmk      = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_DSCPRMK_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_DSCPRMK_LEN));
        pActParam->prioasn      = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_PRIORITY_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_PRIORITY_LEN));
        pActParam->byratelmt    = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_BYRATELMT_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_BYRATELMT_LEN));
        pActParam->uc           = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_UC_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_UC_LEN));
        pActParam->mc           = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_MC_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_MC_LEN));
        pActParam->timerDiscard = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_TIMER_DISCARD_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_TIMER_DISCARD_LEN));
        pActParam->headerDump   = ((asicEntry.val[infoNum] >> CVA_ACTION_TABLE_HEADER_DUMP_LSP) & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_HEADER_DUMP_LEN));

        if(1u == pActParam->copytocpu)
        {
            /* Retrieve copy DPM*/
            CHK_FUN_CONTINUE(ret, cva_drv_acl_copytocpu_action_get(unit, aclRuleIndex, &(pActParam->copyCpuDpm)));
        }

        infoNum++;
        /* Retrieve outer VLAN AIF */
        cva_drv_acl_otag_aif_get(pActParam->otag, &asicEntry, &pActParam->outTagOpInfo, &infoNum);
        /* Retrieve inner VLAN AIF */
        cva_drv_acl_itag_aif_get(pActParam->itag, &asicEntry, &pActParam->inTagOpInfo, &infoNum);
        /* Retrieve VLAN priority remarking AIF */
        cva_drv_acl_vlan_prirmk_aif_get(pActParam->priormk, &asicEntry, &pActParam->priRmkInfo, &infoNum);
        /* Retrieve redirect AIF */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_redir_aif_get(pActParam->redir, &asicEntry, &pActParam->redirInfo, &infoNum));
        /* Retrieve DSCP remarking AIF*/
        cva_drv_acl_dscp_rmk_aif_get(pActParam->dscprmk, &asicEntry, &pActParam->dscpRmkInfo, &infoNum);
        /* Retrieve packet priority AIF*/
        cva_drv_acl_fwd_prias_aif_get(pActParam->prioasn, &asicEntry, &pActParam->prioInfo, &infoNum);
        /* Retrieve routing unicast AIF*/
        cva_drv_acl_routing_uc_aif_get(pActParam->uc, &asicEntry, &pActParam->ucInfo, &infoNum);
        /* Retrieve routing multicast AIF*/
        CHK_FUN_CONTINUE(ret, cva_drv_acl_routing_mc_aif_get(pActParam->mc, &asicEntry, &pActParam->mcInfo, &infoNum));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_acl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessActParam_t *pActParam)
{
    cva_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0u;
    RtkApiRet                  ret       = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[infoNum] = ((uint32)(pActParam->fno & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_FLOW_LEN)) << CVA_ACTION_TABLE_FLOW_LSP)
                                 | ((uint32)(pActParam->drop & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_DROP_LEN)) << CVA_ACTION_TABLE_DROP_LSP)
                                 | ((uint32)(pActParam->copytocpu & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_CP2CPU_LEN)) << CVA_ACTION_TABLE_CP2CPU_LSP)
                                 | ((uint32)(pActParam->mirror1 & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_MIRROR1_LEN)) << CVA_ACTION_TABLE_MIRROR1_LSP)
                                 | ((uint32)(pActParam->mirror0 & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_MIRROR0_LEN)) << CVA_ACTION_TABLE_MIRROR0_LSP)
                                 | ((uint32)(pActParam->otag & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_OTAG_LEN)) << CVA_ACTION_TABLE_OTAG_LSP)
                                 | ((uint32)(pActParam->itag & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_ITAG_LEN)) << CVA_ACTION_TABLE_ITAG_LSP)
                                 | ((uint32)(pActParam->priormk & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_PRIRMK_LEN)) << CVA_ACTION_TABLE_PRIRMK_LSP)
                                 | ((uint32)(pActParam->redir & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_REDIR_LEN)) << CVA_ACTION_TABLE_REDIR_LSP)
                                 | ((uint32)(pActParam->dscprmk & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_DSCPRMK_LEN)) << CVA_ACTION_TABLE_DSCPRMK_LSP)
                                 | ((uint32)(pActParam->prioasn & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_PRIORITY_LEN)) << CVA_ACTION_TABLE_PRIORITY_LSP)
                                 | ((uint32)(pActParam->byratelmt & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_BYRATELMT_LEN)) << CVA_ACTION_TABLE_BYRATELMT_LSP)
                                 | ((uint32)(pActParam->uc & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_UC_LEN)) << CVA_ACTION_TABLE_UC_LSP)
                                 | ((uint32)(pActParam->mc & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_MC_LEN)) << CVA_ACTION_TABLE_MC_LSP)
                                 | ((uint32)(pActParam->timerDiscard & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_TIMER_DISCARD_LEN)) << CVA_ACTION_TABLE_TIMER_DISCARD_LSP)
                                 | ((uint32)(pActParam->headerDump & cva_drv_tbl_fieldlen2mask(CVA_ACTION_TABLE_HEADER_DUMP_LEN)) << CVA_ACTION_TABLE_HEADER_DUMP_LSP);

        infoNum++;
        if(1u == pActParam->copytocpu)
        {
            /* Configure copy DPM*/
            CHK_FUN_CONTINUE(ret, cva_drv_acl_copytocpu_action_set(unit, aclRuleIndex, pActParam->copyCpuDpm));
        }

        /* Configure outer VLAN AIF */
        cva_drv_acl_otag_aif_set(pActParam->otag, &pActParam->outTagOpInfo, &asicEntry, &infoNum);
        /* Configure inner VLAN AIF */
        cva_drv_acl_itag_aif_set(pActParam->itag, &pActParam->inTagOpInfo, &asicEntry, &infoNum);
        /* Configure VLAN priority remarking AIF */
        cva_drv_acl_vlan_prirmk_aif_set(pActParam->priormk, &pActParam->priRmkInfo, &asicEntry, &infoNum);
        /* Configure redirect AIF */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_redir_aif_set(pActParam->redir, &pActParam->redirInfo, &asicEntry, &infoNum));
        /* Configure DSCP remarking AIF*/
        cva_drv_acl_dscp_rmk_aif_set(pActParam->dscprmk, &pActParam->dscpRmkInfo, &asicEntry, &infoNum);
        /* Configure packet priority AIF*/
        cva_drv_acl_fwd_prias_aif_set(pActParam->prioasn, &pActParam->prioInfo, &asicEntry, &infoNum);
        /* Configure routing unicast AIF */
        cva_drv_acl_routing_uc_aif_set(pActParam->uc, &pActParam->ucInfo, &asicEntry, &infoNum);
        /* Configure routing multicast AIF */
        CHK_FUN_CONTINUE(ret, cva_drv_acl_routing_mc_aif_set(pActParam->mc, &pActParam->mcInfo, &asicEntry, &infoNum));

        CHK_FUN_CONTINUE(ret, cva_table_write(unit, CVA_ACTION_TABLE, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}

static uint64 cva_drv_acl_swap64(uint64 value)
{
    return ((value & 0xFF00000000000000u) >> 56u) | ((value & 0x00FF000000000000u) >> 40u) | ((value & 0x0000FF0000000000u) >> 24u) | ((value & 0x000000FF00000000u) >> 8u) | ((value & 0x00000000FF000000u) << 8u) | ((value & 0x0000000000FF0000u) << 24u) | ((value & 0x000000000000FF00u) << 40u) | ((value & 0x00000000000000FFu) << 56u);
}

RtkApiRet cva_drv_acl_ip6_range_check(const AclIpv6 *pUpper, const AclIpv6 *pLower)
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
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memcpy((uint8 *)&tmpVal, &pUpper[0], 8u));
        ipv6_upper_h = cva_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0xffeeddccbbaa9988
            ipv6_upper_l = 0x8899aabbccddeeff
        */
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memcpy((uint8 *)&tmpVal, &pUpper[8], 8u));
        ipv6_upper_l = cva_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0x7766554433221100
            ipv6_lower_h = 0x0011223344556677
        */
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memcpy((uint8 *)&tmpVal, &pLower[0], 8u));
        ipv6_lower_h = cva_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0xffeeddccbbaa9988
            ipv6_lower_l = 0x8899aabbccddeeff
        */
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memcpy((uint8 *)&tmpVal, &pLower[8], 8u));
        ipv6_lower_l = cva_drv_acl_swap64(tmpVal);

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

RtkApiRet cva_drv_acl_ip6_range_set_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, const AclIpv6 *ipv6)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    fieldVal    = 0u;
    uint32    regAddr     = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        if(entryIdx > CVA_FILTER_IP6RANGE_BOUND)
        {
            retVal = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(boundType > CVA_ACL_IP6_RANGE_HBOUND_TYPE)
        {
            retVal = -(RT_ERR_ACL_BOUND_TYPE);
            continue;
        }

        if(NULL == ipv6)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(CVA_ACL_IP6_RANGE_LBOUND_TYPE == boundType)
        {
            regAddr = CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_3;
        }
        else
        {
            regAddr = CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_1_3;
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
            CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, regAddr - (i * 4u) + (entryIdx * 36u), CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LSP, CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LEN, fieldVal));
        }

    } while(0u == 1u);

    return retVal;
}

RtkApiRet cva_drv_acl_ip6_range_get_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, AclIpv6 *ipv6)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    fieldVal    = 0u;
    uint32    regAddr     = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        if(entryIdx > CVA_FILTER_IP6RANGE_BOUND)
        {
            retVal = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(boundType > CVA_ACL_IP6_RANGE_HBOUND_TYPE)
        {
            retVal = -(RT_ERR_ACL_BOUND_TYPE);
            continue;
        }

        if(NULL == ipv6)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(CVA_ACL_IP6_RANGE_LBOUND_TYPE == boundType)
        {
            regAddr = CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_3;
        }
        else
        {
            regAddr = CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_1_3;
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
            CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, regAddr - (i * 4u) + (entryIdx * 36u), CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LSP, CVA_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LEN, &fieldVal));
            ipv6[(i * 4u)]      = (uint8)((fieldVal & 0xff000000u) >> 24u);
            ipv6[(i * 4u) + 1u] = (uint8)((fieldVal & 0xff0000u) >> 16u);
            ipv6[(i * 4u) + 2u] = (uint8)((fieldVal & 0xff00u) >> 8u);
            ipv6[(i * 4u) + 3u] = (uint8)((fieldVal & 0xffu));
        }

    } while(0u == 1u);

    return retVal;
}
