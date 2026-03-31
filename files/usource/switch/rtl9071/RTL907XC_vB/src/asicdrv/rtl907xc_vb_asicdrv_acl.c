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
#include "rtl907xc_vb_api_acl.h"
#include "rtl907xc_vb_asicdrv_acl.h"
#include "rtl907xc_vb_asicdrv_tcam.h"

RtkApiRet cvb_drv_acl_userport_2_sysport(RtkPort userPort, RtkPort *pSysPort)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((userPort != (RtkPort)CVB_RTK_CPU_PORT) && ((userPort > (RtkPort)CVB_RTK_USER_PORT_END) || (userPort < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -(RT_ERR_PORT_ID);
    }

    *pSysPort = (RtkPort)cvb_g_userPort2sysPort[userPort];

    return retVal;
}

RtkApiRet cvb_drv_acl_userpmap_2_syspmap(RtkPmap userPmap, RtkPmap *pSysPmap)
{
    RtkApiRet retVal = RT_ERR_OK;
    retVal           = cvb_drv_pmap_convert(userPmap, pSysPmap, CVB_EM_USER_TO_SYS);

    return retVal;
}

RtkApiRet cvb_drv_acl_internal_rule_id_check(uint32 unit, AclRuleId aclRuleIndex)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* PIE33 cannot use */
        if((aclRuleIndex >= CVB_ACL_INT_RULE_PIE33_LBOUND) && (aclRuleIndex <= CVB_ACL_INT_RULE_PIE33_HBOUND))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        /* PIE35 cannot use */
        if((aclRuleIndex >= CVB_ACL_INT_RULE_PIE35_LBOUND) && (aclRuleIndex <= CVB_ACL_INT_RULE_PIE35_HBOUND))
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

        if(aclRuleIndex > CVB_ACL_INT_RULE_BOUND)
        {
            ret = -(RT_ERR_ACL_ENTRYIDX);
            continue;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_acl_template_getting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType *pType)
{
    uint32 regAddr  = 0u;
    uint32 regValue = 0u;
    uint32 regField = 0u;

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > CVB_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > CVB_ACL_TEMPLATE_FIELD_BOUND)
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
        if(tpIdx > CVB_ACL_TEMPLATE_BOUND)
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVB_USER_DEFINED_TEMPLATE_CONTROL48 + (uint32)((((tpIdx - (CVB_ACL_TEMPLATE_BOUND + 1u)) * 3u) + (fieldIdx / 4u)) * 4u);
        }
        /* external TG 0 - 15 */
        else
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVB_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        }
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = CVB_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, regAddr, regField, CVB_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, &regValue));
        *pType = regValue;
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_acl_template_setting(uint32 unit, AclTemplateId tpIdx, AclTemplateFieldId fieldIdx, AclFilterFieldType type)
{
    uint32    regAddr  = 0u;
    uint32    regField = 0u;
    RtkApiRet ret      = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(tpIdx > CVB_ACL_INT_TEMPLATE_BOUND)
        {
            ret = -(RT_ERR_ACL_TGIDX);
            continue;
        }

        if(fieldIdx > CVB_ACL_TEMPLATE_FIELD_BOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_IDX);
            continue;
        }

        if(type > CVB_IACL_FLD_HBOUND)
        {
            ret = -(RT_ERR_ACL_TG_FIELD_TYPE);
            continue;
        }

        /* internal TG 16 - 17 */
        if(tpIdx > CVB_ACL_TEMPLATE_BOUND)
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVB_USER_DEFINED_TEMPLATE_CONTROL48 + (uint32)((((tpIdx - (CVB_ACL_TEMPLATE_BOUND + 1u)) * 3u) + (fieldIdx / 4u)) * 4u);
        }
        /* external TG 0 - 15 */
        else
        {
            /* *3u: each TG has 3 registers to setting 12 templates, /4u: 1 register stores 4 templates, *4u: 4 bytes register */
            regAddr = CVB_USER_DEFINED_TEMPLATE_CONTROL0 + ((((uint32)tpIdx * 3u) + ((uint32)fieldIdx / 4u)) * 4u);
        }
        /* %4u: 1 register stores 4 templates, *7u: 1 field has 7 bits */
        regField = CVB_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LSP + (((uint32)fieldIdx % 4u) * 7u);

        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, regAddr, regField, CVB_USER_DEFINED_TEMPLATE_CONTROL0_FIELD0_LEN, type));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_acl_getting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    cvb_AclTblAccessPieEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pPieParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, cvb_table_read(unit, CVB_PIE_TABLE, aclRuleIndex, &(asicEntry.val)[0]));

        CHK_FUN_CONTINUE(ret, cvb_drv_drv_memset(pPieParam, 0, sizeof(AclTblAccessPieParam_t)));
        pPieParam->data0  = (uint16)((asicEntry.val[0]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data1  = (uint16)((asicEntry.val[0] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data2  = (uint16)((asicEntry.val[1]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data3  = (uint16)((asicEntry.val[1] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data4  = (uint16)((asicEntry.val[2]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data5  = (uint16)((asicEntry.val[2] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data6  = (uint16)((asicEntry.val[3]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data7  = (uint16)((asicEntry.val[3] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data8  = (uint16)((asicEntry.val[4]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data9  = (uint16)((asicEntry.val[4] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data10 = (uint16)((asicEntry.val[5]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->data11 = (uint16)((asicEntry.val[5] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->valid  = asicEntry.val[6] & cvb_drv_tbl_fieldlen2mask(CVB_PIE_TABLE_VALID_INDIRECT_LEN);
        pPieParam->mask0  = (uint16)((asicEntry.val[7]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask1  = (uint16)((asicEntry.val[7] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask2  = (uint16)((asicEntry.val[8]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask3  = (uint16)((asicEntry.val[8] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask4  = (uint16)((asicEntry.val[9]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask5  = (uint16)((asicEntry.val[9] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask6  = (uint16)((asicEntry.val[10]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask7  = (uint16)((asicEntry.val[10] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask8  = (uint16)((asicEntry.val[11]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask9  = (uint16)((asicEntry.val[11] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask10 = (uint16)((asicEntry.val[12]) & cvb_drv_tbl_fieldlen2mask(16u));
        pPieParam->mask11 = (uint16)((asicEntry.val[12] >> 16u) & cvb_drv_tbl_fieldlen2mask(16u));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_acl_setting_pie_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessPieParam_t *pPieParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    cvb_AclTblAccessPieEntry_t asicEntry = {0};

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pPieParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[0]  = (((uint32)pPieParam->data0 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data1 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[1]  = (((uint32)pPieParam->data2 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data3 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[2]  = (((uint32)pPieParam->data4 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data5 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[3]  = (((uint32)pPieParam->data6 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data7 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[4]  = (((uint32)pPieParam->data8 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data9 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[5]  = (((uint32)pPieParam->data10 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->data11 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[6]  = pPieParam->valid & cvb_drv_tbl_fieldlen2mask(CVB_PIE_TABLE_VALID_INDIRECT_LEN);
        asicEntry.val[7]  = (((uint32)pPieParam->mask0 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask1 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[8]  = (((uint32)pPieParam->mask2 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask3 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[9]  = (((uint32)pPieParam->mask4 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask5 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[10] = (((uint32)pPieParam->mask6 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask7 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[11] = (((uint32)pPieParam->mask8 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask9 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);
        asicEntry.val[12] = (((uint32)pPieParam->mask10 & cvb_drv_tbl_fieldlen2mask(16u))) | (((uint32)pPieParam->mask11 & cvb_drv_tbl_fieldlen2mask(16u)) << 16u);

        CHK_FUN_CONTINUE(ret, cvb_drv_tcam_write(unit, CVB_PIE_TABLE, aclRuleIndex, &(asicEntry.val)[0]));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_acl_copytocpu_action_get(uint32 unit, AclRuleId aclRuleIndex, RtkPmap *copyCpuDpm)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    pieIndex = 0u;
    uint32    regAddr  = 0u;
    uint32    cpuMask  = 0u;
    RtkPmap   srcPmap  = 0u;
    RtkPmap   dstPmap  = 0u;

    do
    {
        pieIndex = aclRuleIndex / CVB_ACL_PIE_RULE_NUM;
        if((pieIndex == 0u) || (pieIndex == 1u))
        {
            /* CVB_ACL_COPYTOCPU_CONTROL0 in the specific register address */
            regAddr = CVB_ACL_COPYTOCPU_CONTROL0;
        }
        else
        {
            /*
                -2u: pieIndex 0 and 1 is belong to CVB_ACL_COPYTOCPU_CONTROL0
                /2u: one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks
                *4u: 4 bytes register
            */
            regAddr = CVB_ACL_COPYTOCPU_CONTROL1 + (((pieIndex - 2u) / 2u) * 4u);
        }
        /* one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks */
        if(0u == (pieIndex % 2u))
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regAddr, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LSP, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LEN, &cpuMask));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regAddr, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LSP, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LEN, &cpuMask));
        }
        srcPmap = (cpuMask & (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP));
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER));
        *copyCpuDpm = dstPmap;
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cvb_drv_acl_copytocpu_action_set(uint32 unit, AclRuleId aclRuleIndex, RtkPmap copyCpuDpm)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    pieIndex = 0u;
    uint32    regAddr  = 0u;
    RtkPmap   dstPmap  = 0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(copyCpuDpm, &dstPmap, CVB_EM_USER_TO_SYS));
        pieIndex = aclRuleIndex / CVB_ACL_PIE_RULE_NUM;
        if((pieIndex == 0u) || (pieIndex == 1u))
        {
            /* CVB_ACL_COPYTOCPU_CONTROL0 in the specific register address */
            regAddr = CVB_ACL_COPYTOCPU_CONTROL0;
        }
        else
        {
            /*
                -2u: pieIndex 0 and 1 is belong to CVB_ACL_COPYTOCPU_CONTROL0
                /2u: one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks
                *4u: 4 bytes register
            */
            regAddr = CVB_ACL_COPYTOCPU_CONTROL1 + (((pieIndex - 2u) / 2u) * 4u);
        }
        /* one ACL_COPYTOCPU_CONTROLX register has 2 copytocpu masks */
        if(0u == (pieIndex % 2u))
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, regAddr, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LSP, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_0_LEN, (uint32)dstPmap));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, regAddr, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LSP, CVB_ACL_COPYTOCPU_CONTROL0_CPTOCPUMSK_1_LEN, (uint32)dstPmap));
        }
    } while(0u == 1u);

    return retVal;
}

static void cvb_drv_acl_otag_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessOtagOpParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw   = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
        pActParam->outVidCtl  = ((pAsicEntry->val[*pInfoNum] >> 29u) & cvb_drv_tbl_fieldlen2mask(2u));
        pActParam->outVidInfo = ((pAsicEntry->val[*pInfoNum] >> 17u) & cvb_drv_tbl_fieldlen2mask(12u));
        pActParam->outTagOp   = ((pAsicEntry->val[*pInfoNum] >> 15u) & cvb_drv_tbl_fieldlen2mask(2u));

        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_otag_aif_set(uint32 flag, const AclTblAccessOtagOpParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->outVidCtl & cvb_drv_tbl_fieldlen2mask(2u)) << 29u)
                                     | ((uint32)(pActParam->outVidInfo & cvb_drv_tbl_fieldlen2mask(12u)) << 17u)
                                     | ((uint32)(pActParam->outTagOp & cvb_drv_tbl_fieldlen2mask(2u)) << 15u);
        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_itag_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessIntagOpParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw  = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
        pActParam->inVidCtl  = ((pAsicEntry->val[*pInfoNum] >> 29u) & cvb_drv_tbl_fieldlen2mask(2u));
        pActParam->inVidInfo = ((pAsicEntry->val[*pInfoNum] >> 17u) & cvb_drv_tbl_fieldlen2mask(12u));
        pActParam->inTagOp   = ((pAsicEntry->val[*pInfoNum] >> 15u) & cvb_drv_tbl_fieldlen2mask(2u));

        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_itag_aif_set(uint32 flag, const AclTblAccessIntagOpParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->inVidCtl & cvb_drv_tbl_fieldlen2mask(2u)) << 29u)
                                     | ((uint32)(pActParam->inVidInfo & cvb_drv_tbl_fieldlen2mask(12u)) << 17u)
                                     | ((uint32)(pActParam->inTagOp & cvb_drv_tbl_fieldlen2mask(2u)) << 15u);
        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_vlan_prirmk_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessVlanPriorityRmkParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
        pActParam->tagSel   = ((pAsicEntry->val[*pInfoNum] >> 27u) & cvb_drv_tbl_fieldlen2mask(4u));
        pActParam->inPri    = ((pAsicEntry->val[*pInfoNum] >> 24u) & cvb_drv_tbl_fieldlen2mask(3u));
        pActParam->outPri   = ((pAsicEntry->val[*pInfoNum] >> 21u) & cvb_drv_tbl_fieldlen2mask(3u));
        pActParam->odei     = ((pAsicEntry->val[*pInfoNum] >> 20u) & cvb_drv_tbl_fieldlen2mask(1u));
        pActParam->idei     = ((pAsicEntry->val[*pInfoNum] >> 19u) & cvb_drv_tbl_fieldlen2mask(1u));

        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_vlan_prirmk_aif_set(uint32 flag, const AclTblAccessVlanPriorityRmkParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->tagSel & cvb_drv_tbl_fieldlen2mask(4u)) << 27u)
                                     | ((uint32)(pActParam->inPri & cvb_drv_tbl_fieldlen2mask(3u)) << 24u)
                                     | ((uint32)(pActParam->outPri & cvb_drv_tbl_fieldlen2mask(3u)) << 21u)
                                     | ((uint32)(pActParam->odei & cvb_drv_tbl_fieldlen2mask(1u)) << 20u)
                                     | ((uint32)(pActParam->idei & cvb_drv_tbl_fieldlen2mask(1u)) << 19u);
        (*pInfoNum)++;
    }
}

static RtkApiRet cvb_drv_acl_redir_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessRedirectParam_t *pActParam, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
        {
            pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
            pActParam->opcode   = ((pAsicEntry->val[*pInfoNum] >> 29u) & cvb_drv_tbl_fieldlen2mask(2u));
            pActParam->crsvlan  = ((pAsicEntry->val[*pInfoNum] >> 28u) & cvb_drv_tbl_fieldlen2mask(1u));
            srcPmap             = ((RtkPmap)(pAsicEntry->val[*pInfoNum] >> 12u) & (RtkPmap)(CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP));
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER));
            pActParam->dpm         = dstPmap;
            pActParam->bypassLaLut = ((pAsicEntry->val[*pInfoNum] >> 11u) & cvb_drv_tbl_fieldlen2mask(1u));

            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cvb_drv_acl_redir_aif_set(uint32 flag, const AclTblAccessRedirectParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
        {
            srcPmap = (RtkPmap)pActParam->dpm;
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS));
            pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                         | ((uint32)(pActParam->opcode & cvb_drv_tbl_fieldlen2mask(2u)) << 29u)
                                         | ((uint32)(pActParam->crsvlan & cvb_drv_tbl_fieldlen2mask(1u)) << 28u)
                                         | ((uint32)(dstPmap & cvb_drv_tbl_fieldlen2mask(16u)) << 12u)
                                         | ((uint32)(pActParam->bypassLaLut & cvb_drv_tbl_fieldlen2mask(1u)) << 11u);

            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static void cvb_drv_acl_dscp_rmk_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessDscpRmkParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->opcode = ((pAsicEntry->val[*pInfoNum] >> 29u) & cvb_drv_tbl_fieldlen2mask(2u));

        if(0u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[*pInfoNum] >> 23u) & cvb_drv_tbl_fieldlen2mask(6u));
        }
        else if(1u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[*pInfoNum] >> 26u) & cvb_drv_tbl_fieldlen2mask(3u));
        }
        else if(2u == pActParam->opcode)
        {
            pActParam->withdraw      = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
            pActParam->dscpIpPrecDtr = ((pAsicEntry->val[*pInfoNum] >> 23u) & cvb_drv_tbl_fieldlen2mask(3u));
        }
        else
        {
            /* do nothing */
        }
        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_dscp_rmk_aif_set(uint32 flag, const AclTblAccessDscpRmkParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    uint32 opcode        = 0u;
    uint32 dscpIpPrecDtr = 0u;

    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        opcode = (uint32)pActParam->opcode & cvb_drv_tbl_fieldlen2mask(2u);
        if(0u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & cvb_drv_tbl_fieldlen2mask(6u)) << 23u;
        }
        else if(1u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & cvb_drv_tbl_fieldlen2mask(3u)) << 26u;
        }
        else if(2u == opcode)
        {
            dscpIpPrecDtr = (uint32)(pActParam->dscpIpPrecDtr & cvb_drv_tbl_fieldlen2mask(3u)) << 23u;
        }
        else
        {
            /* do nothing */
        }

        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(opcode) << 29u)
                                     | (dscpIpPrecDtr);

        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_fwd_prias_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessFwdPriorityParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
        pActParam->priority = ((pAsicEntry->val[*pInfoNum] >> 28u) & cvb_drv_tbl_fieldlen2mask(3u));
        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_fwd_prias_aif_set(uint32 flag, const AclTblAccessFwdPriorityParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->priority & cvb_drv_tbl_fieldlen2mask(3u)) << 28u);
        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_routing_uc_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessUcParam_t *pActParam, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
        pActParam->nhIdx    = ((pAsicEntry->val[*pInfoNum] >> 19u) & cvb_drv_tbl_fieldlen2mask(12u));
        (*pInfoNum)++;
    }
}

static void cvb_drv_acl_routing_uc_aif_set(uint32 flag, const AclTblAccessUcParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
    {
        pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                     | ((uint32)(pActParam->nhIdx & cvb_drv_tbl_fieldlen2mask(12u)) << 19u);
        (*pInfoNum)++;
    }
}

static RtkApiRet cvb_drv_acl_routing_mc_aif_get(uint32 flag, cvb_AclTblAccessActEntry_t *pAsicEntry, AclTblAccessMcParam_t *pActParam, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
        {
            pActParam->withdraw = ((pAsicEntry->val[*pInfoNum] >> 31u) & cvb_drv_tbl_fieldlen2mask(1u));
            pActParam->rhIdx    = ((pAsicEntry->val[*pInfoNum] >> 19u) & cvb_drv_tbl_fieldlen2mask(12u));
            srcPmap             = ((pAsicEntry->val[*pInfoNum] >> 3u) & cvb_drv_tbl_fieldlen2mask(16u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER));
            pActParam->eppDpm = dstPmap;
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cvb_drv_acl_routing_mc_aif_set(uint32 flag, const AclTblAccessMcParam_t *pActParam, cvb_AclTblAccessActEntry_t *pAsicEntry, uint32 *pInfoNum)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   dstPmap = 0u;

    do
    {
        if((1u == (flag & 1u)) && (*pInfoNum <= CVB_ACL_AIF_DATA_REG_BOUND))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_pmap_convert(pActParam->eppDpm, &dstPmap, CVB_EM_USER_TO_SYS));
            pAsicEntry->val[*pInfoNum] = ((uint32)(pActParam->withdraw & cvb_drv_tbl_fieldlen2mask(1u)) << 31u)
                                         | ((uint32)(pActParam->rhIdx & cvb_drv_tbl_fieldlen2mask(12u)) << 19u)
                                         | ((uint32)(dstPmap & cvb_drv_tbl_fieldlen2mask(16u)) << 3u);
            (*pInfoNum)++;
        }
    } while(0u == 1u);
    return retVal;
}

RtkApiRet cvb_drv_acl_getting_action_entry(uint32 unit, AclRuleId aclRuleIndex, AclTblAccessActParam_t *pActParam)
{
    RtkApiRet                  ret       = RT_ERR_OK;
    cvb_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, cvb_table_read(unit, CVB_ACTION_TABLE, aclRuleIndex, &(asicEntry.val)[0]));

        pActParam->fno          = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_FLOW_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_FLOW_LEN));
        pActParam->drop         = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_DROP_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_DROP_LEN));
        pActParam->copytocpu    = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_CP2CPU_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_CP2CPU_LEN));
        pActParam->mirror1      = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_MIRROR1_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_MIRROR1_LEN));
        pActParam->mirror0      = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_MIRROR0_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_MIRROR0_LEN));
        pActParam->otag         = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_OTAG_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_OTAG_LEN));
        pActParam->itag         = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_ITAG_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_ITAG_LEN));
        pActParam->priormk      = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_PRIRMK_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_PRIRMK_LEN));
        pActParam->redir        = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_REDIR_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_REDIR_LEN));
        pActParam->dscprmk      = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_DSCPRMK_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_DSCPRMK_LEN));
        pActParam->prioasn      = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_PRIORITY_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_PRIORITY_LEN));
        pActParam->byratelmt    = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_BYRATELMT_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_BYRATELMT_LEN));
        pActParam->uc           = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_UC_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_UC_LEN));
        pActParam->mc           = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_MC_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_MC_LEN));
        pActParam->timerDiscard = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_TIMER_DISCARD_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_TIMER_DISCARD_LEN));
        pActParam->headerDump   = ((asicEntry.val[infoNum] >> CVB_ACTION_TABLE_HEADER_DUMP_LSP) & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_HEADER_DUMP_LEN));

        if(1u == pActParam->copytocpu)
        {
            /* Retrieve copy DPM*/
            CHK_FUN_CONTINUE(ret, cvb_drv_acl_copytocpu_action_get(unit, aclRuleIndex, &(pActParam->copyCpuDpm)));
        }

        infoNum++;
        /* Retrieve outer VLAN AIF */
        cvb_drv_acl_otag_aif_get(pActParam->otag, &asicEntry, &pActParam->outTagOpInfo, &infoNum);
        /* Retrieve inner VLAN AIF */
        cvb_drv_acl_itag_aif_get(pActParam->itag, &asicEntry, &pActParam->inTagOpInfo, &infoNum);
        /* Retrieve VLAN priority remarking AIF */
        cvb_drv_acl_vlan_prirmk_aif_get(pActParam->priormk, &asicEntry, &pActParam->priRmkInfo, &infoNum);
        /* Retrieve redirect AIF */
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_redir_aif_get(pActParam->redir, &asicEntry, &pActParam->redirInfo, &infoNum));
        /* Retrieve DSCP remarking AIF*/
        cvb_drv_acl_dscp_rmk_aif_get(pActParam->dscprmk, &asicEntry, &pActParam->dscpRmkInfo, &infoNum);
        /* Retrieve packet priority AIF*/
        cvb_drv_acl_fwd_prias_aif_get(pActParam->prioasn, &asicEntry, &pActParam->prioInfo, &infoNum);
        /* Retrieve routing unicast AIF*/
        cvb_drv_acl_routing_uc_aif_get(pActParam->uc, &asicEntry, &pActParam->ucInfo, &infoNum);
        /* Retrieve routing multicast AIF*/
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_routing_mc_aif_get(pActParam->mc, &asicEntry, &pActParam->mcInfo, &infoNum));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_acl_setting_action_entry(uint32 unit, AclRuleId aclRuleIndex, const AclTblAccessActParam_t *pActParam)
{
    cvb_AclTblAccessActEntry_t asicEntry = {0};
    uint32                     infoNum   = 0u;
    RtkApiRet                  ret       = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_internal_rule_id_check(unit, aclRuleIndex));

        if(NULL == pActParam)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        asicEntry.val[infoNum] = ((uint32)(pActParam->fno & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_FLOW_LEN)) << CVB_ACTION_TABLE_FLOW_LSP)
                                 | ((uint32)(pActParam->drop & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_DROP_LEN)) << CVB_ACTION_TABLE_DROP_LSP)
                                 | ((uint32)(pActParam->copytocpu & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_CP2CPU_LEN)) << CVB_ACTION_TABLE_CP2CPU_LSP)
                                 | ((uint32)(pActParam->mirror1 & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_MIRROR1_LEN)) << CVB_ACTION_TABLE_MIRROR1_LSP)
                                 | ((uint32)(pActParam->mirror0 & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_MIRROR0_LEN)) << CVB_ACTION_TABLE_MIRROR0_LSP)
                                 | ((uint32)(pActParam->otag & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_OTAG_LEN)) << CVB_ACTION_TABLE_OTAG_LSP)
                                 | ((uint32)(pActParam->itag & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_ITAG_LEN)) << CVB_ACTION_TABLE_ITAG_LSP)
                                 | ((uint32)(pActParam->priormk & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_PRIRMK_LEN)) << CVB_ACTION_TABLE_PRIRMK_LSP)
                                 | ((uint32)(pActParam->redir & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_REDIR_LEN)) << CVB_ACTION_TABLE_REDIR_LSP)
                                 | ((uint32)(pActParam->dscprmk & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_DSCPRMK_LEN)) << CVB_ACTION_TABLE_DSCPRMK_LSP)
                                 | ((uint32)(pActParam->prioasn & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_PRIORITY_LEN)) << CVB_ACTION_TABLE_PRIORITY_LSP)
                                 | ((uint32)(pActParam->byratelmt & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_BYRATELMT_LEN)) << CVB_ACTION_TABLE_BYRATELMT_LSP)
                                 | ((uint32)(pActParam->uc & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_UC_LEN)) << CVB_ACTION_TABLE_UC_LSP)
                                 | ((uint32)(pActParam->mc & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_MC_LEN)) << CVB_ACTION_TABLE_MC_LSP)
                                 | ((uint32)(pActParam->timerDiscard & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_TIMER_DISCARD_LEN)) << CVB_ACTION_TABLE_TIMER_DISCARD_LSP)
                                 | ((uint32)(pActParam->headerDump & cvb_drv_tbl_fieldlen2mask(CVB_ACTION_TABLE_HEADER_DUMP_LEN)) << CVB_ACTION_TABLE_HEADER_DUMP_LSP);

        infoNum++;
        if(1u == pActParam->copytocpu)
        {
            /* Configure copy DPM*/
            CHK_FUN_CONTINUE(ret, cvb_drv_acl_copytocpu_action_set(unit, aclRuleIndex, pActParam->copyCpuDpm));
        }

        /* Configure outer VLAN AIF */
        cvb_drv_acl_otag_aif_set(pActParam->otag, &pActParam->outTagOpInfo, &asicEntry, &infoNum);
        /* Configure inner VLAN AIF */
        cvb_drv_acl_itag_aif_set(pActParam->itag, &pActParam->inTagOpInfo, &asicEntry, &infoNum);
        /* Configure VLAN priority remarking AIF */
        cvb_drv_acl_vlan_prirmk_aif_set(pActParam->priormk, &pActParam->priRmkInfo, &asicEntry, &infoNum);
        /* Configure redirect AIF */
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_redir_aif_set(pActParam->redir, &pActParam->redirInfo, &asicEntry, &infoNum));
        /* Configure DSCP remarking AIF*/
        cvb_drv_acl_dscp_rmk_aif_set(pActParam->dscprmk, &pActParam->dscpRmkInfo, &asicEntry, &infoNum);
        /* Configure packet priority AIF*/
        cvb_drv_acl_fwd_prias_aif_set(pActParam->prioasn, &pActParam->prioInfo, &asicEntry, &infoNum);
        /* Configure routing unicast AIF */
        cvb_drv_acl_routing_uc_aif_set(pActParam->uc, &pActParam->ucInfo, &asicEntry, &infoNum);
        /* Configure routing multicast AIF */
        CHK_FUN_CONTINUE(ret, cvb_drv_acl_routing_mc_aif_set(pActParam->mc, &pActParam->mcInfo, &asicEntry, &infoNum));

        CHK_FUN_CONTINUE(ret, cvb_table_write(unit, CVB_ACTION_TABLE, (uint32)aclRuleIndex, &(asicEntry.val)[0]));

    } while(0u == 1u);

    return ret;
}

static uint64 cvb_drv_acl_swap64(uint64 value)
{
    return ((value & 0xFF00000000000000u) >> 56u) | ((value & 0x00FF000000000000u) >> 40u) | ((value & 0x0000FF0000000000u) >> 24u) | ((value & 0x000000FF00000000u) >> 8u) | ((value & 0x00000000FF000000u) << 8u) | ((value & 0x0000000000FF0000u) << 24u) | ((value & 0x000000000000FF00u) << 40u) | ((value & 0x00000000000000FFu) << 56u);
}

RtkApiRet cvb_drv_acl_ip6_range_check(const AclIpv6 *pUpper, const AclIpv6 *pLower)
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
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memcpy((uint8 *)&tmpVal, &pUpper[0], 8u));
        ipv6_upper_h = cvb_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0xffeeddccbbaa9988
            ipv6_upper_l = 0x8899aabbccddeeff
        */
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memcpy((uint8 *)&tmpVal, &pUpper[8], 8u));
        ipv6_upper_l = cvb_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0x7766554433221100
            ipv6_lower_h = 0x0011223344556677
        */
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memcpy((uint8 *)&tmpVal, &pLower[0], 8u));
        ipv6_lower_h = cvb_drv_acl_swap64(tmpVal);

        /*
            IPv6 address: 0011:2233:4455:6677:8899:AABB:CCDD:EEFF
            tmpVal = 0xffeeddccbbaa9988
            ipv6_lower_l = 0x8899aabbccddeeff
        */
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memcpy((uint8 *)&tmpVal, &pLower[8], 8u));
        ipv6_lower_l = cvb_drv_acl_swap64(tmpVal);

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

RtkApiRet cvb_drv_acl_ip6_range_set_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, const AclIpv6 *ipv6)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    fieldVal    = 0u;
    uint32    regAddr     = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        if(entryIdx > CVB_FILTER_IP6RANGE_BOUND)
        {
            retVal = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(boundType > CVB_ACL_IP6_RANGE_HBOUND_TYPE)
        {
            retVal = -(RT_ERR_ACL_BOUND_TYPE);
            continue;
        }

        if(NULL == ipv6)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(CVB_ACL_IP6_RANGE_LBOUND_TYPE == boundType)
        {
            regAddr = CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_3;
        }
        else
        {
            regAddr = CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_1_3;
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
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, regAddr - (i * 4u) + (entryIdx * 36u), CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LSP, CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LEN, fieldVal));
        }

    } while(0u == 1u);

    return retVal;
}

RtkApiRet cvb_drv_acl_ip6_range_get_boundary(uint32 unit, AclFilterIndex entryIdx, AclBoundType boundType, AclIpv6 *ipv6)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    fieldVal    = 0u;
    uint32    regAddr     = 0u;
    uint32    i           = 0u;
    uint32    rangeRegNum = 3u;

    do
    {
        if(entryIdx > CVB_FILTER_IP6RANGE_BOUND)
        {
            retVal = -(RT_ERR_ACL_RANGE_CHK_IDX);
            continue;
        }

        if(boundType > CVB_ACL_IP6_RANGE_HBOUND_TYPE)
        {
            retVal = -(RT_ERR_ACL_BOUND_TYPE);
            continue;
        }

        if(NULL == ipv6)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(CVB_ACL_IP6_RANGE_LBOUND_TYPE == boundType)
        {
            regAddr = CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_3;
        }
        else
        {
            regAddr = CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_1_3;
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
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_read(unit, regAddr - (i * 4u) + (entryIdx * 36u), CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LSP, CVB_IPV6_RANGE_CHECKING_TABLE_ENTRY0_0_0_IP_LOWER_BOUND_0_LEN, &fieldVal));
            ipv6[(i * 4u)]      = (uint8)((fieldVal & 0xff000000u) >> 24u);
            ipv6[(i * 4u) + 1u] = (uint8)((fieldVal & 0xff0000u) >> 16u);
            ipv6[(i * 4u) + 2u] = (uint8)((fieldVal & 0xff00u) >> 8u);
            ipv6[(i * 4u) + 3u] = (uint8)((fieldVal & 0xffu));
        }

    } while(0u == 1u);

    return retVal;
}
