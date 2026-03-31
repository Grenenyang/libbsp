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
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtkas_api_igmp.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ext.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ext.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ext.h"
#endif

/**
 * @defgroup RTK_MODULE_IGMP IGMP/MLD
 * @brief IGMP/MLD APIs. This component is for IGMP/MLD snooping.
 * @{
 */
/**
 * @defgroup IGMP_GFS General Feature Status
 * @brief IGMP/MLD APIs for general features
 * @{
 */

/**
 * @brief This API is used to configure IGMP ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] igmpInfo IGMP set information (N/A). \n
 *                     Parameters required to configure IGMP ability.
 * @param[out] None
 * @retval RT_ERR_OK           IGMP ability was configured.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others              Please refer to dv1_igmpmld_igmp_ability_set(), dva_igmpmld_igmp_ability_set(), \n
 *                             cvb_igmpmld_igmp_ability_set(), cva_igmpmld_igmp_ability_set().
 */
RtkApiRet rtk_igmpmld_igmp_ability_set(UnitChip_t unitChip, IgmpInfo_t *igmpInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == igmpInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_igmp_ability_set(unitChip.unit, igmpInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_igmp_ability_set(unitChip.unit, igmpInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_igmp_ability_set(unitChip.unit, igmpInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_igmp_ability_set(unitChip.unit, igmpInfo->enable);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get IGMP ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[out] igmpInfo IGMP get information (N/A). \n
 *                      Parameters required to obtain IGMP ability.
 * @retval RT_ERR_OK            IGMP ability was obtained.
 * @retval -RT_ERR_NULL_POINTER Null pointer.
 * @retval -RT_ERR_NOT_SUPPORT  Not supported.
 * @retval Others               Please refer to dv1_igmpmld_igmp_ability_get() or dva_igmpmld_igmp_ability_get(), \n
 *                              cvb_igmpmld_igmp_ability_get(), cvb_igmpmld_igmp_ability_get().
 */
RtkApiRet rtk_igmpmld_igmp_ability_get(UnitChip_t unitChip, IgmpInfo_t *igmpInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == igmpInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_igmp_ability_get(unitChip.unit, &(igmpInfo->enable));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_igmp_ability_get(unitChip.unit, &(igmpInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_igmp_ability_get(unitChip.unit, &(igmpInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_igmp_ability_get(unitChip.unit, &(igmpInfo->enable));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to configure MLD ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] mldInfo MLD set information (N/A) \n
 *                    Parameters required to configure MLD ability.
 * @param[out] None
 * @retval RT_ERR_OK            MLD ability was configured.
 * @retval -RT_ERR_NULL_POINTER Null pointer.
 * @retval -RT_ERR_NOT_SUPPORT  Not supported.
 * @retval Others               Please refer to dv1_igmpmld_mld_ability_set() or dva_igmpmld_mld_ability_set(), \n
 *                              cvb_igmpmld_mld_ability_set(), cva_igmpmld_mld_ability_set().
 */
RtkApiRet rtk_igmpmld_mld_ability_set(UnitChip_t unitChip, MldInfo_t *mldInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == mldInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_mld_ability_set(unitChip.unit, mldInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_mld_ability_set(unitChip.unit, mldInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_mld_ability_set(unitChip.unit, mldInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_mld_ability_set(unitChip.unit, mldInfo->enable);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get MLD ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] mldInfo MLD get information (N/A) \n
 *                    Parameters required to obtain MLD ability.
 * @retval RT_ERR_OK            MLD ability was obtained.
 * @retval -RT_ERR_NULL_POINTER Null pointer.
 * @retval -RT_ERR_NOT_SUPPORT  Not supported.
 * @retval Others               Please refer to dv1_igmpmld_mld_ability_get() or dva_igmpmld_mld_ability_get(), \n
 *                              cvb_igmpmld_mld_ability_get(), cva_igmpmld_mld_ability_get().
 */
RtkApiRet rtk_igmpmld_mld_ability_get(UnitChip_t unitChip, MldInfo_t *mldInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == mldInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_mld_ability_get(unitChip.unit, &(mldInfo->enable));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_mld_ability_get(unitChip.unit, &(mldInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_mld_ability_get(unitChip.unit, &(mldInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_mld_ability_get(unitChip.unit, &(mldInfo->enable));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to configure FastLeave ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] fastLeaveInfo FastLeave set information (N/A) \n
 *                          Parameters required to configure IGMP/MLD fastleave ability. \n
 *                          If enabled, the related entries will be updated or deleted as soon as IGMP/MLD leave is received. \n
 *                          If disabled, the update or the delete of related entries will be delayed for a period depending on \n
 *                          the last member aging time to make sure there is no one interested in the group.
 * @param[out] None
 * @return RT_ERR_OK            fastleave ability was configured.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_fastleave_ability_set() or dva_igmpmld_fastleave_ability_set(), \n
 *                              cvb_igmpmld_fastleave_ability_set(), cva_igmpmld_fastleave_ability_set().
 */
RtkApiRet rtk_igmpmld_fastleave_ability_set(UnitChip_t unitChip, FastLeaveInfo_t *fastLeaveInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == fastLeaveInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_fastleave_ability_set(unitChip.unit, fastLeaveInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_fastleave_ability_set(unitChip.unit, fastLeaveInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_fastleave_ability_set(unitChip.unit, fastLeaveInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_fastleave_ability_set(unitChip.unit, fastLeaveInfo->enable);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get FastLeave ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[out] fastLeaveInfo FastLeave get information (N/A) \n
 *                           Parameters required to obtain IGMP/MLD fastleave ability. \n
 *                           If enabled, the related entries will be updated or deleted as soon as IGMP/MLD leave is received. \n
 *                           If disabled, the update or the delete of related entries will be delayed for a period depending on \n
 *                           the last member aging time to make sure there is no one interested in the group.
 * @return RT_ERR_OK            FastLeave ability was obtained.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_fastleave_ability_get() or dva_igmpmld_fastleave_ability_get(), \n
 *                              cvb_igmpmld_fastleave_ability_get(), cva_igmpmld_fastleave_ability_get().
 */
RtkApiRet rtk_igmpmld_fastleave_ability_get(UnitChip_t unitChip, FastLeaveInfo_t *fastLeaveInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == fastLeaveInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_fastleave_ability_get(unitChip.unit, &(fastLeaveInfo->enable));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_fastleave_ability_get(unitChip.unit, &(fastLeaveInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_fastleave_ability_get(unitChip.unit, &(fastLeaveInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_fastleave_ability_get(unitChip.unit, &(fastLeaveInfo->enable));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to configure Cross VLAN ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] crossVlanInfo CrossVLAN set information (N/A) \n
 *                          Parameters required to configure IGMP/MLD cross VLAN ability. \n
 *                          If enabled, IGMP/MLD packets and IP multicast data packets will be forwarded without VLAN limitations.
 * @param[out] None
 * @return RT_ERR_OK            Cross VLAN ability was configured.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_crossvlan_ability_set() or dva_igmpmld_crossvlan_ability_set(), \n
 *                              cvb_igmpmld_crossvlan_ability_set(), cva_igmpmld_crossvlan_ability_set().
 */
RtkApiRet rtk_igmpmld_crossvlan_ability_set(UnitChip_t unitChip, CrossVlanInfo_t *crossVlanInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == crossVlanInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_crossvlan_ability_set(unitChip.unit, crossVlanInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_crossvlan_ability_set(unitChip.unit, crossVlanInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_crossvlan_ability_set(unitChip.unit, crossVlanInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_crossvlan_ability_set(unitChip.unit, crossVlanInfo->enable);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get Cross VLAN ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[out] crossVlanInfo CrossVLAN get information (N/A) \n
 *                           Parameters required to obtain IGMP/MLD cross VLAN ability. \n
 *                           If enabled, IGMP/MLD packets and IP multicast data packets will be forwarded without VLAN limitations.
 * @return RT_ERR_OK            Cross VLAN ability was obtained.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_crossvlan_ability_get() or dva_igmpmld_crossvlan_ability_get(), \n
 *                              cvb_igmpmld_crossvlan_ability_get(), cva_igmpmld_crossvlan_ability_get().
 */
RtkApiRet rtk_igmpmld_crossvlan_ability_get(UnitChip_t unitChip, CrossVlanInfo_t *crossVlanInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == crossVlanInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_crossvlan_ability_get(unitChip.unit, &(crossVlanInfo->enable));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_crossvlan_ability_get(unitChip.unit, &(crossVlanInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_crossvlan_ability_get(unitChip.unit, &(crossVlanInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_crossvlan_ability_get(unitChip.unit, &(crossVlanInfo->enable));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* IGMP_GFS */

/**
 * @defgroup AGING_TIME IGMP/MLD Aging Time
 * @brief IGMP/MLD APIs for group/last member aging time
 * @{
 */
/**
 * @brief This API is used to set the group member aging time of IGMP/MLD. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] grpAgeTimeInfo Group member aging time set information (N/A) \n
 *                           Parameters required to configure IGMP/MLD group member aging time. \n
 *                           If the group address does not receive any IGMP/MLD report in this duration, it would be expired and deleted.
 * @param[out] None
 * @retval RT_ERR_OK            The group member aging time was configured.
 * @retval -RT_ERR_NULL_POINTER Null pointer.
 * @retval -RT_ERR_NOT_SUPPORT  Not supported.
 * @retval Others               Please refer to dv1_igmpmld_grpmemagingtime_set() or dva_igmpmld_grpmemagingtime_set(), \n
 *                              cvb_igmpmld_grpmemagingtime_set(), cva_igmpmld_grpmemagingtime_set()
 */
RtkApiRet rtk_igmpmld_grpmemagingtime_set(UnitChip_t unitChip, GroupAgeTimeInfo_t *grpAgeTimeInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == grpAgeTimeInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_grpmemagingtime_set(unitChip.unit, grpAgeTimeInfo->grpTime);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_grpmemagingtime_set(unitChip.unit, grpAgeTimeInfo->grpTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_grpmemagingtime_set(unitChip.unit, grpAgeTimeInfo->grpTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_grpmemagingtime_set(unitChip.unit, grpAgeTimeInfo->grpTime);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get the group member aging time of IGMP/MLD. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[out] grpAgeTimeInfo Group member aging time get information (N/A) \n
 *                            Parameters required to obtain IGMP/MLD group member aging time. \n
 *                            If the group address does not receive any IGMP/MLD report in this duration, it would be expired and deleted.
 * @retval RT_ERR_OK            The group member aging time was obtained.
 * @retval -RT_ERR_NULL_POINTER Null pointer.
 * @retval -RT_ERR_NOT_SUPPORT  Not supported.
 * @retval Others               Please refer to dv1_igmpmld_grpmemagingtime_get() or dva_igmpmld_grpmemagingtime_get(), \n
 *                              cvb_igmpmld_grpmemagingtime_get(), cva_igmpmld_grpmemagingtime_get().
 */
RtkApiRet rtk_igmpmld_grpmemagingtime_get(UnitChip_t unitChip, GroupAgeTimeInfo_t *grpAgeTimeInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == grpAgeTimeInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_grpmemagingtime_get(unitChip.unit, &(grpAgeTimeInfo->grpTime));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_grpmemagingtime_get(unitChip.unit, &(grpAgeTimeInfo->grpTime));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_grpmemagingtime_get(unitChip.unit, &(grpAgeTimeInfo->grpTime));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_grpmemagingtime_get(unitChip.unit, &(grpAgeTimeInfo->grpTime));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to set the last member aging time of IGMP/MLD. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] lastAgeTimeInfo Last member aging time set information (N/A) \n
 *                            Parameters required to configure IGMP/MLD last member aging time. \n
 *                            This specifies the waiting time for the group address to be deleted since the last member leaves.
 * @param[out] None
 * @return RT_ERR_OK            The last member aging time was configured.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_lastmemagingtime_set() or dva_igmpmld_lastmemagingtime_set(), \n
 *                              cvb_igmpmld_lastmemagingtime_set(), cva_igmpmld_lastmemagingtime_set().
 */
RtkApiRet rtk_igmpmld_lastmemagingtime_set(UnitChip_t unitChip, LastAgeTimeInfo_t *lastAgeTimeInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == lastAgeTimeInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_lastmemagingtime_set(unitChip.unit, lastAgeTimeInfo->lstTime);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_lastmemagingtime_set(unitChip.unit, lastAgeTimeInfo->lstTime);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_lastmemagingtime_set(unitChip.unit, lastAgeTimeInfo->lstTime);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_lastmemagingtime_set(unitChip.unit, lastAgeTimeInfo->lstTime);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get the last member aging time of IGMP/MLD. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[out] lastAgeTimeInfo Last member aging time get information (N/A) \n
 *                             Parameters required to obtain IGMP/MLD last member aging time. \n
 *                             This specifies the waiting time for the group address to be deleted since the last member leaves.
 * @return RT_ERR_OK            The last member aging time was obtained.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_lastmemagingtime_get() or dva_igmpmld_lastmemagingtime_get(), \n
 *                              cvb_igmpmld_lastmemagingtime_get(), cva_igmpmld_lastmemagingtime_get().
 */
RtkApiRet rtk_igmpmld_lastmemagingtime_get(UnitChip_t unitChip, LastAgeTimeInfo_t *lastAgeTimeInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == lastAgeTimeInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_lastmemagingtime_get(unitChip.unit, &(lastAgeTimeInfo->lstTime));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_lastmemagingtime_get(unitChip.unit, &(lastAgeTimeInfo->lstTime));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_lastmemagingtime_get(unitChip.unit, &(lastAgeTimeInfo->lstTime));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_lastmemagingtime_get(unitChip.unit, &(lastAgeTimeInfo->lstTime));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* AGING_TIME */

/**
 * @defgroup L2_DROP L2 Default Drop Setting
 * @brief IGMP/MLD APIs for L2 default drop setting
 * @{
 */

/**
 * @brief This API is to configure the default drop when layer 2 packets lookup miss. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used for indicating which switch to be accessed. \n
 *                     Chip ID is used for chip model.
 * @param[in] l2D4DropInfo L2 default drop set information (N/A) \n
 *                         Parameters required to configure L2 default drop. \n
 *                         When this ability is enabled, packets will be dropped if the destination address is not in the L2 table. \n
 *                         This conflicts with IGMP/MLD ability.
 * @param[out] None
 * @return RT_ERR_OK            The L2 default drop was configured.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_l2defaultdrop_set(), dva_igmpmld_l2defaultdrop_set(), \n
 *                              cvb_igmpmld_l2defaultdrop_set(), or cva_igmpmld_l2defaultdrop_set().
 */
RtkApiRet rtk_igmpmld_l2defaultdrop_set(UnitChip_t unitChip, L2D4DropInfo_t *l2D4DropInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == l2D4DropInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_l2defaultdrop_set(unitChip.unit, l2D4DropInfo->drop);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_l2defaultdrop_set(unitChip.unit, l2D4DropInfo->drop);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_l2defaultdrop_set(unitChip.unit, l2D4DropInfo->drop);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_l2defaultdrop_set(unitChip.unit, l2D4DropInfo->drop);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API obtains the L2 default drop setting. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[out] l2D4DropInfo L2 default drop set information (N/A) \n
 *                          Parameters required to obtain the L2 default drop setting. \n
 *                          When this feature is enabled, packets will be dropped if the destination address is not in the L2 table. \n
 *                          This conflicts with the IGMP/MLD feature.
 * @return RT_ERR_OK            The L2 default drop setting was obtained.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_l2defaultdrop_get(), dva_igmpmld_l2defaultdrop_get(), \n
 *                              cvb_igmpmld_l2defaultdrop_get(), or cva_igmpmld_l2defaultdrop_get().
 */
RtkApiRet rtk_igmpmld_l2defaultdrop_get(UnitChip_t unitChip, L2D4DropInfo_t *l2D4DropInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == l2D4DropInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_l2defaultdrop_get(unitChip.unit, &(l2D4DropInfo->drop));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_l2defaultdrop_get(unitChip.unit, &(l2D4DropInfo->drop));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_l2defaultdrop_get(unitChip.unit, &(l2D4DropInfo->drop));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_l2defaultdrop_get(unitChip.unit, &(l2D4DropInfo->drop));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* L2_DROP */

/**
 * @defgroup CUSTMZ Customized L2 Lookup Miss Setting
 * @brief IGMP/MLD APIs for customized L2 lookup miss setting
 * @{
 */
/**
 * @brief This API configures the customization when layer 2 packets lookup miss. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in] l2CustomizeInfo L2 customization set information (N/A) \n
 *                            Parameters required to configure L2 customization when a lookup miss happens. \n
 *                            If this feature is enabled, the customized operation is allowed when a lookup miss happens. \n
 *                            The customized operation includes drop, trap to CPU, and flood in VLAN. \n
 *                            This conflicts with Default Drop and IGMP/MLD.
 * @param[out] None
 * @return RT_ERR_OK            The L2 customization was configured.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_l2custmzlumiss_set(), dva_igmpmld_l2custmzlumiss_set(), \n
 *                              cvb_igmpmld_l2custmzlumiss_set(), or cva_igmpmld_l2custmzlumiss_set().
 */
RtkApiRet rtk_igmpmld_l2custmzlumiss_set(UnitChip_t unitChip, L2CustomizeInfo_t *l2CustomizeInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == l2CustomizeInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_l2custmzlumiss_set(unitChip.unit, l2CustomizeInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_l2custmzlumiss_set(unitChip.unit, l2CustomizeInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_l2custmzlumiss_set(unitChip.unit, l2CustomizeInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_l2custmzlumiss_set(unitChip.unit, l2CustomizeInfo->enable);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API obtains the layer 2 customization setting. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[out] l2CustomizeInfo L2 customization get information (N/A) \n
 *                             Parameters required to obtain the L2 customization setting. \n
 *                             If this feature is enabled, the customized operation is allowed when a lookup miss happens. \n
 *                             The customized operation includes drop, trap to CPU, and flood in VLAN. \n
 *                             This conflicts with Default Drop and IGMP/MLD.
 * @return RT_ERR_OK            The L2 customization setting was obtained.
 * @return -RT_ERR_NULL_POINTER Null pointer.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dv1_igmpmld_l2custmzlumiss_get(), dva_igmpmld_l2custmzlumiss_get(), \n
 *                              cvb_igmpmld_l2custmzlumiss_get(), or cva_igmpmld_l2custmzlumiss_get().
 */
RtkApiRet rtk_igmpmld_l2custmzlumiss_get(UnitChip_t unitChip, L2CustomizeInfo_t *l2CustomizeInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == l2CustomizeInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_igmpmld_l2custmzlumiss_get(unitChip.unit, &(l2CustomizeInfo->enable));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_igmpmld_l2custmzlumiss_get(unitChip.unit, &(l2CustomizeInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_igmpmld_l2custmzlumiss_get(unitChip.unit, &(l2CustomizeInfo->enable));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_igmpmld_l2custmzlumiss_get(unitChip.unit, &(l2CustomizeInfo->enable));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* CUSTMZ */
/**@}*/ /* RTK_MODULE_IGMP */
