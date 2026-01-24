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

#include "rtl907xc_vb_api_ext.h"

/**
 * @addtogroup RTK_MODULE_IGMP IGMP/MLD
 * @{
 */
/**
 * @addtogroup IGMP_GFS General Feature Status
 * @{
 */
/**
 * @addtogroup IGMP_GFS_ASIC Low Level Driver
 * @{
 */

/**
 * @brief This API is used to configure IGMP ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] igmpAbility IGMP ability (0 to 1) \n
 *                        1: enable \n
 *                        0: disable
 * @param[out] None
 * @retval SUCCESS       The acceptable igmpAbility was set.
 * @retval -RT_ERR_INPUT The "igmpAbility" was out of range.
 * @retval Others        Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_igmp_ability_set(uint32 unit, uint32 igmpAbility)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(igmpAbility >= (uint32)CVB_RTK_ENABLE_END)
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            /* Load the control register into retVal */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            /* Update igmpAbility in regVal */
            if((uint32)CVB_ENABLED == igmpAbility)
            {
                regVal |= ((uint32)0x1u << (uint32)CVB_IGMP_IGMP_ABILITY_LSB);
            }
            else
            {
                regVal &= (~((uint32)0x1u << (uint32)CVB_IGMP_IGMP_ABILITY_LSB));
            }
            /* Write regVal back into the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_0, regVal));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get IGMP ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] pIgmpAbility The pointer to IGMP ability (0 to 1) \n
 *                          1: enabled \n
 *                          0: disabled
 * @retval SUCCESS              Access IGMP ability successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of the "pIgmpAbility" is NULL.
 * @retval Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_igmp_ability_get(uint32 unit, uint32 *pIgmpAbility)
{
    uint32    fieldVal = 0u;
    uint32    regVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        if(NULL == pIgmpAbility)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Read the control register to check if igmp is enabled or not */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            fieldVal      = (uint32)(((regVal & CVB_IGMP_CTRL_MASK_0) >> CVB_IGMP_IGMP_ABILITY_LSB) & 0x1u);
            *pIgmpAbility = (1u == fieldVal) ? (uint32)CVB_ENABLED : (uint32)CVB_DISABLED;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to configure MLD ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] mldAbility MLD ability (0 to 1) \n
 *                       1: enable \n
 *                       0: disable
 * @param[out] None
 * @retval SUCCESS       The acceptable mldAbility was set.
 * @retval -RT_ERR_INPUT The "mldAbility" was out of range.
 * @retval Others        Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_mld_ability_set(uint32 unit, uint32 mldAbility)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(mldAbility >= (uint32)CVB_RTK_ENABLE_END)
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            /* Load the control register into retVal */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            /* Update mldAbility in regVal */
            if((uint32)CVB_ENABLED == mldAbility)
            {
                regVal |= ((uint32)0x1u << (uint32)CVB_IGMP_MLD_ABILITY_LSB);
            }
            else
            {
                regVal &= (~((uint32)0x1u << (uint32)CVB_IGMP_MLD_ABILITY_LSB));
            }
            /* Write regVal back into the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_0, regVal));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get MLD ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] pMldAbility The pointer to MLD ability (0 to 1) \n
 *                         1: enabled \n
 *                         0: disabled
 * @retval SUCCESS              Access MLD ability successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of the "pMldAbility" is NULL.
 * @retval Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_mld_ability_get(uint32 unit, uint32 *pMldAbility)
{
    uint32    fieldVal = 0u;
    uint32    regVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        if(NULL == pMldAbility)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Read the control register to check if MLD is enabled or not */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            fieldVal     = (uint32)(((regVal & CVB_IGMP_CTRL_MASK_0) >> CVB_IGMP_MLD_ABILITY_LSB) & 0x1u);
            *pMldAbility = (1u == fieldVal) ? (uint32)CVB_ENABLED : (uint32)CVB_DISABLED;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to configure FastLeave ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] fastLeaveEn FastLeave ability (0 to 1) \n
 *                        1: enable \n
 *                        0: disable
 * @param[out] None
 * @retval SUCCESS       The acceptable fastLeaveEn was set.
 * @retval -RT_ERR_INPUT The "fastLeaveEn" was out of range.
 * @retval Others        Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_fastleave_ability_set(uint32 unit, uint32 fastLeaveEn)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(fastLeaveEn >= (uint32)CVB_RTK_ENABLE_END)
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            /* Load the control register into retVal */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            /* Update fastLeaveEn ability in regVal */
            if((uint32)CVB_ENABLED == fastLeaveEn)
            {
                regVal |= ((uint32)0x1u << (uint32)CVB_IGMP_FASTLEAVE_ABILITY_LSB);
            }
            else
            {
                regVal &= (~((uint32)0x1u << (uint32)CVB_IGMP_FASTLEAVE_ABILITY_LSB));
            }
            /* Write regVal back into the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_0, regVal));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get FastLeave ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] pFastLeaveEn The pointer to FastLeave ability (0 to 1) \n
 *                          1: enabled \n
 *                          0: disabled
 * @return SUCCESS              Access FastLeave ability successfully.
 * @return -RT_ERR_NULL_POINTER The pointer of the "pFastLeaveEn" is NULL.
 * @return Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_fastleave_ability_get(uint32 unit, uint32 *pFastLeaveEn)
{
    uint32    fieldVal = 0u;
    uint32    regVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        if(NULL == pFastLeaveEn)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Read the control register to check if fastleave is enabled or not */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            fieldVal      = (uint32)(((regVal & CVB_IGMP_CTRL_MASK_0) >> CVB_IGMP_FASTLEAVE_ABILITY_LSB) & 0x1u);
            *pFastLeaveEn = (1u == fieldVal) ? (uint32)CVB_ENABLED : (uint32)CVB_DISABLED;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to configure Cross VLAN ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] crossVlanEn Cross VLAN ability (0 to 1) \n
 *                        1: enable \n
 *                        0: disable
 * @param[out] None
 * @retval SUCCESS       The acceptable crossVlanEn was set.
 * @retval -RT_ERR_INPUT The "crossVlanEn" was out of range.
 * @retval Others        Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_crossvlan_ability_set(uint32 unit, uint32 crossVlanEn)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(crossVlanEn >= (uint32)CVB_RTK_ENABLE_END)
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            /* Load the control register into retVal */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            /* Update crossVlanEn ability in regVal */
            if((uint32)CVB_ENABLED == crossVlanEn)
            {
                regVal |= ((uint32)0x1u << (uint32)CVB_IGMP_CROSSVLAN_ABILITY_LSB);
            }
            else
            {
                regVal &= (~((uint32)0x1u << (uint32)CVB_IGMP_CROSSVLAN_ABILITY_LSB));
            }
            /* Write regVal back into the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_0, regVal));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get Cross VLAN ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] pCrossVlanEn The pointer to Cross VLAN ability (0 to 1) \n
 *                          1: enabled \n
 *                          0: disabled
 * @retval SUCCESS              Access Cross VLAN ability successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of the "pCrossVlanEn" is NULL.
 * @retval Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_crossvlan_ability_get(uint32 unit, uint32 *pCrossVlanEn)
{
    uint32    fieldVal = 0u;
    uint32    regVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        if(NULL == pCrossVlanEn)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Read the control register to check if cross VLAN is enabled or not */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            fieldVal      = (uint32)(((regVal & CVB_IGMP_CTRL_MASK_0) >> CVB_IGMP_CROSSVLAN_ABILITY_LSB) & 0x1u);
            *pCrossVlanEn = (1u == fieldVal) ? (uint32)CVB_ENABLED : (uint32)CVB_DISABLED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* IGMP_GFS_ASIC */
/**@}*/ /* IGMP_GFS */

/**
 * @addtogroup AGING_TIME IGMP MLD Aging Time
 * @{
 */
/**
 * @addtogroup IGMP_AGING_ASIC Low Level Driver
 * @{
 */

/**
 * @brief This API is used to set the time of aging out for one group address.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] grpMemAgingTime The time setting of aging out for one group address
 * @param[out] None
 * @return SUCCESS The grpMemAgingTime was set.
 * @return Others  Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_grpmemagingtime_set(uint32 unit, uint8 grpMemAgingTime)
{
    uint32    regVal   = 0u;
    uint32    fieldVal = 0u;
    uint32    oldVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_1, &regVal));
        /* keep router portmask */
        oldVal = (regVal & CVB_IGMP_CTRL_MASK_2);
        /* Clear grpMemAgingTime field. */
        fieldVal = (regVal & CVB_IGMP_CTRL_MASK_1) & (~((uint32)0xFFu << (uint32)CVB_IGMP_GRP_MEM_AGE_TIME_LSB));
        /* Update grpMemAgingTime in reg value. */
        regVal = fieldVal | ((uint32)grpMemAgingTime << (uint32)CVB_IGMP_GRP_MEM_AGE_TIME_LSB);

        CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_1, (uint32)(regVal | oldVal)));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get the time of aging out for one group address.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] pGrpMemAgingTime The pointer to the time setting of aging out for one group address
 * @param[out] None
 * @return SUCCESS              Access Group Member Aging Timeout setting successfully.
 * @return -RT_ERR_NULL_POINTER The pointer of the "pGrpMemAgingTime" is NULL.
 * @return Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_grpmemagingtime_get(uint32 unit, uint8 *pGrpMemAgingTime)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(NULL == pGrpMemAgingTime)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Get grpMemAgingTime field from the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_1, &regVal));
            *pGrpMemAgingTime = (uint8)((((regVal & CVB_IGMP_CTRL_MASK_1)) >> CVB_IGMP_GRP_MEM_AGE_TIME_LSB) & 0xFFu);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to set the time duration of deleting group address after receiving LEAVE message in non-fast leave mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] lastMemAgingTime Time duration of deleting group address when the last member leaves
 * @param[out] None
 * @return SUCCESS The lastMemAgingTime was set.
 * @return Others  Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_lastmemagingtime_set(uint32 unit, uint8 lastMemAgingTime)
{
    uint32    regVal   = 0u;
    uint32    fieldVal = 0u;
    uint32    oldVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_1, &regVal));
        /* keep router portmask */
        oldVal = (regVal & CVB_IGMP_CTRL_MASK_2);
        /* Clear field lastMemAgingTime value. */
        fieldVal = (regVal & CVB_IGMP_CTRL_MASK_1) & (~((uint32)0xFFu << (uint32)CVB_IGMP_LAST_MEM_AGE_TIME_LSB));
        /* Replace lastMemAgingTime value in regVal. */
        regVal = fieldVal | ((uint32)lastMemAgingTime << (uint32)CVB_IGMP_LAST_MEM_AGE_TIME_LSB);

        CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_1, (uint32)(regVal | oldVal)));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get the time duration of deleting group address after receiving LEAVE message in non-fast leave mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] pLastMemAgingTime The pointer to time duration of deleting group address when the last member leaves
 * @param[out] None
 * @return SUCCESS              Access Last Member Aging Timeout setting successfully.
 * @return -RT_ERR_NULL_POINTER The pointer of the "pLastMemAgingTime" is NULL.
 * @return Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_lastmemagingtime_get(uint32 unit, uint8 *pLastMemAgingTime)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(NULL == pLastMemAgingTime)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Get lastMemAgingTime field from the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_1, &regVal));
            *pLastMemAgingTime = (uint8)((((regVal & CVB_IGMP_CTRL_MASK_1)) >> CVB_IGMP_LAST_MEM_AGE_TIME_LSB) & 0xFFu);
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* IGMP_AGING_ASIC */
/**@}*/ /* AGING_TIME */

/**
 * @addtogroup L2_DROP L2 Default Drop Setting
 * @{
 */
/**
 * @addtogroup L2_DROP_ASIC Low Level Driver
 * @{
 */

/**
 * @brief This API is to Enable/Disable the default drop operation when layer 2 packets lookup miss.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] l2DefaultDrop Enable/Disable the default drop operation (0 to 1) \n
 *                          1: enable \n
 *                          0: disable
 * @param[out] None
 * @retval SUCCESS       The l2DefaultDrop was set.
 * @retval -RT_ERR_INPUT The "l2DefaultDrop" was out of range.
 * @retval Others        Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_l2defaultdrop_set(uint32 unit, uint32 l2DefaultDrop)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(l2DefaultDrop >= (uint32)CVB_RTK_ENABLE_END)
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            /* Load the control register into retVal */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            /* Update l2DefaultDrop in regVal */
            if((uint32)CVB_ENABLED == l2DefaultDrop)
            {
                regVal |= ((uint32)0x1u << (uint32)CVB_IGMP_L2_DEF_DROP_LSB);
            }
            else
            {
                regVal &= (~((uint32)0x1u << (uint32)CVB_IGMP_L2_DEF_DROP_LSB));
            }
            /* Write regVal back into the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_0, regVal));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is to get the layer 2 default drop setting.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] pl2Operation Pointer to L2 default drop setting (0 to 1) \n
 *                          1: enabled \n
 *                          0: disabled
 * @return SUCCESS              Access L2 Default Drop setting successfully.
 * @return -RT_ERR_NULL_POINTER The pointer of the "pl2DefaultDrop" is NULL.
 * @return Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_l2defaultdrop_get(uint32 unit, uint32 *pl2DefaultDrop)
{
    uint32    fieldVal = 0u;
    uint32    regVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        if(NULL == pl2DefaultDrop)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Read the control register to check if l2defaultdrop is enabled or not */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            fieldVal        = (uint32)(((regVal & CVB_IGMP_CTRL_MASK_0) >> CVB_IGMP_L2_DEF_DROP_LSB) & 0x1u);
            *pl2DefaultDrop = (1u == fieldVal) ? (uint32)CVB_ENABLED : (uint32)CVB_DISABLED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* L2_DROP_ASIC */
/**@}*/ /* L2_DROP */

/**
 * @addtogroup CUSTMZ Customized L2 Lookup Miss Setting
 * @{
 */
/**
 * @addtogroup IGMP_CUSTMZ_ASIC Low Level Driver
 * @{
 */

/**
 * @brief This API is to Enable/Disable the customization setting when layer 2 packets lookup miss.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] l2Customize Enable/Disable the layer 2 customization (0 to 1) \n
 *                        1: enable \n
 *                        0: disable
 * @param[out] None
 * @retval SUCCESS       The l2Customize was set.
 * @retval -RT_ERR_INPUT The "l2Customize" was out of range.
 * @retval Others        Please refer to cvb_reg_read() and cvb_reg_write() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_l2custmzlumiss_set(uint32 unit, uint32 l2Customize)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = SUCCESS;

    do
    {
        if(l2Customize >= (uint32)CVB_RTK_ENABLE_END)
        {
            retVal = -RT_ERR_INPUT;
        }
        else
        {
            /* Load the control register into retVal */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            /* Update l2Customize in regVal */
            if((uint32)CVB_ENABLED == l2Customize)
            {
                regVal |= ((uint32)0x1u << (uint32)CVB_IGMP_L2_CUS_LU_MISS_LSB);
            }
            else
            {
                regVal &= (~((uint32)0x1u << (uint32)CVB_IGMP_L2_CUS_LU_MISS_LSB));
            }
            /* Write regVal back into the control register */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_IGMP_CONTROL_0, regVal));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is to get the layer 2 customize lookup miss setting.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[out] pl2Operation Pointer to L2 customize setting (0 to 1) \n
 *                          1: enabled \n
 *                          0: disabled
 * @retval SUCCESS              Access L2 Customized Lookup Miss setting successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of the "pl2DefaultDrop" is NULL.
 * @retval Others               Please refer to cvb_reg_read() in rtl907xc_vb_reg_list.c.
 */
RtkApiRet cvb_igmpmld_l2custmzlumiss_get(uint32 unit, uint32 *pl2Customize)
{
    uint32    fieldVal = 0u;
    uint32    regVal   = 0u;
    RtkApiRet retVal   = SUCCESS;

    do
    {
        if(NULL == pl2Customize)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        else
        {
            /* Read the control register to check if l2custmzlumiss is enabled or not */
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_IGMP_CONTROL_0, &regVal));
            fieldVal      = (uint32)(((regVal & CVB_IGMP_CTRL_MASK_0) >> CVB_IGMP_L2_CUS_LU_MISS_LSB) & 0x1u);
            *pl2Customize = (1u == fieldVal) ? (uint32)CVB_ENABLED : (uint32)CVB_DISABLED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* IGMP_CUSTMZ_ASIC */
/**@}*/ /* CUSTMZ */
/**@}*/ /* RTK_MODULE_IGMP */
