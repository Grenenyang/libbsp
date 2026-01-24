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
#include "rtkas_api_igmp.h"
#include "rtkas_api_vlan.h"

#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_reg_list.h"
#endif

RtkApiRet cvb_get_all_igmp_config(RtkIgmpConfig_t *pIgmpCfg);
RtkApiRet cvb_igmp_mld_disable(uint32 unit);
RtkApiRet cvb_igmp_mld_grpagingtime(uint32 unit);
RtkApiRet cvb_igmp_mld_fastleave(uint32 unit);
RtkApiRet cvb_igmp_mld_lastmemagingtime(uint32 unit);
RtkApiRet cvb_igmp_mld_l2defaultdrop(uint32 unit);
RtkApiRet cvb_igmp_mld_crossvlan(uint32 unit);
RtkApiRet cvb_igmp_mld_l2custmzlumiss(uint32 unit);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup IGMP_EXAMPLE IGMP Example
 * @{
 */
/**
 * @defgroup RTL907CVB_IGMP_EXAMPLE RTL907XC_VB IGMP Example
 * @{
 */
/**
 * @defgroup RTL907CVB_IGMP_GFS_EXAMPLE General Feature Example
 * @{
 */
/**
 * @defgroup RTL907CVB_IGMP_GFS_EXAMPLE_1 Get all IGMP configurations
 * @brief This example retrieves the status of all IGMP features.
 *
 * @section Purpose
 * This demonstrates how to use GET APIs.
 *
 * @section Description
 * The results will be saved in the structure pIgmpCfg.
 *
 * @{
 */
RtkApiRet cvb_get_all_igmp_config(RtkIgmpConfig_t *pIgmpCfg)
{
    RtkApiRet          retVal = RT_ERR_OK;
    UnitChip_t         uintChip;
    IgmpInfo_t         igmpGet;
    MldInfo_t          mldGet;
    CrossVlanInfo_t    crossGet;
    FastLeaveInfo_t    fastGet;
    L2D4DropInfo_t     dropGet;
    LastAgeTimeInfo_t  lastGet;
    GroupAgeTimeInfo_t groupGet;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        if(NULL == pIgmpCfg)
        {
            retVal = -RT_ERR_NULL_POINTER;
            break;
        }
        /* Step 1: Get all IGMP configurations. */
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_igmp_ability_get(uintChip, &igmpGet));
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_mld_ability_get(uintChip, &mldGet));
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_crossvlan_ability_get(uintChip, &crossGet));
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_fastleave_ability_get(uintChip, &fastGet));
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_l2defaultdrop_get(uintChip, &dropGet));
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_lastmemagingtime_get(uintChip, &lastGet));
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_grpmemagingtime_get(uintChip, &groupGet));

        pIgmpCfg->igmpEn           = igmpGet.enable;
        pIgmpCfg->mldEn            = mldGet.enable;
        pIgmpCfg->crosVlanEn       = crossGet.enable;
        pIgmpCfg->fastLeavEn       = fastGet.enable;
        pIgmpCfg->l2DropEn         = dropGet.drop;
        pIgmpCfg->lastMemAgingTime = lastGet.lstTime;
        pIgmpCfg->grpMemAgingTime  = groupGet.grpTime;
    } while(0u == 1u);

    /* Expected Behavior: Check IGMP configurations by the pIgmpCfg. */
    return retVal;
}
/**@}*/ /* RTL907CVB_IGMP_GFS_EXAMPLE_1 */

/**
 * @defgroup RTL907CVB_IGMP_GFS_EXAMPLE_2 Disable IGMP/MLD ability
 * @brief This example disables IGMP and MLD.
 *
 * @section Purpose
 * This shows how to disable IGMP/MLD.
 *
 * @section Description
 * Use SET APIs to disable IGMP/MLD.
 *
 * @{
 */
RtkApiRet cvb_igmp_mld_disable(uint32 unit)
{
    RtkApiRet  retVal = RT_ERR_OK;
    UnitChip_t uintChip;
    IgmpInfo_t igmpSet;
    MldInfo_t  mldSet;
    uintChip.unit = unit;
    uintChip.chip = CHIP_907XC_VB;

    /* Step 1. Disable IGMP and MLD. */
    igmpSet.enable = 0u;
    mldSet.enable  = 0u;
    CHK_FUN_RET(retVal, rtk_igmpmld_igmp_ability_set(uintChip, &igmpSet));
    CHK_FUN_RET(retVal, rtk_igmpmld_mld_ability_set(uintChip, &mldSet));
    /* Expected Behavior: Multicast packets will be flooded to all ports. */

    return retVal;
}
/**@}*/ /* RTL907CVA_IGMP_GFS_EXAMPLE_2 */

/**
 * @defgroup RTL907CVB_IGMP_GFS_EXAMPLE_3 Enable Fast Leave ability
 * @brief This example enables IGMP, MLD, and Fast Leave.
 *
 * @section Purpose
 * This shows how to enable Fast Leave ability.
 *
 * @section Description
 * After this execution, IGMP and MLD will be enabled, and Fast Leave will be enabled as well. \n
 * The multicast group packets will stop forwarding immediately once the last member leaves the port.
 *
 * @{
 */
RtkApiRet cvb_igmp_mld_fastleave(uint32 unit)
{
    RtkApiRet       retVal = RT_ERR_OK;
    UnitChip_t      uintChip;
    IgmpInfo_t      igmpSet;
    MldInfo_t       mldSet;
    FastLeaveInfo_t fastSet;
    uintChip.unit = unit;
    uintChip.chip = CHIP_907XC_VB;

    /* Step 1. Enable IGMP and MLD. */
    igmpSet.enable = 1u;
    mldSet.enable  = 1u;
    CHK_FUN_RET(retVal, rtk_igmpmld_igmp_ability_set(uintChip, &igmpSet));
    CHK_FUN_RET(retVal, rtk_igmpmld_mld_ability_set(uintChip, &mldSet));

    /* Step 2. Enable Fast Leave. */
    fastSet.enable = 1u;
    CHK_FUN_RET(retVal, rtk_igmpmld_fastleave_ability_set(uintChip, &fastSet));
    /* Expected Behavior: Multicast packets will stop forwarding immediately once the last member leaves the port. */

    return retVal;
}
/**@}*/ /* RTL907CVB_IGMP_GFS_EXAMPLE_3 */

/**
 * @defgroup RTL907CVB_IGMP_GFS_EXAMPLE_4 Enable Cross VLAN ability
 * @brief The cross VLAN example requires ports with 2 different VLAN IDs.
 *
 * @image html igmp_fastleave.jpg
 *
 * |           | Port 3    | Port 2    | Port 1    |
 * | :-------- | :-------- | :-------- | :-------- |
 * | VLAN 100  | Tagged    | Tagged    |    x      |
 * | VLAN 200  |    x      |    x      | Tagged    |
 *
 * @section Purpose
 * This shows how to enable Cross VLAN.
 *
 * @section Description
 * In the example, port 3 is configured to tagged VLAN 100, and port 1 and port 2 are configured to tagged VLAN 200. \n
 * Multicast traffic is from port 1 with VLAN 200. Host 1 is connected to port 3, and Host 2 is connected to port 2. \n
 * In this example, with cross VLAN enabled, Host 1 and Host 2 can both receive the multicast traffic.
 *
 * @{
 */
RtkApiRet cvb_igmp_mld_crossvlan(uint32 unit)
{
    RtkApiRet       retVal = RT_ERR_OK;
    UnitChip_t      uintChip;
    IgmpInfo_t      igmpSet;
    MldInfo_t       mldSet;
    VlanInfo_t      vlanSet;
    CrossVlanInfo_t crossSet;
    uintChip.unit = unit;
    uintChip.chip = CHIP_907XC_VB;

    /* Step 1: Enable IGMP and MLD. */
    igmpSet.enable = 1u;
    mldSet.enable  = 1u;
    CHK_FUN_RET(retVal, rtk_igmpmld_igmp_ability_set(uintChip, &igmpSet));
    CHK_FUN_RET(retVal, rtk_igmpmld_mld_ability_set(uintChip, &mldSet));

    /* Step 2: Set VLAN port, port3 with VLAN100, port1 with VLAN200, and port2 with VLAN200. */
    CHK_FUN_RET(retVal, rtk_vlan_init(uintChip));
    CHK_FUN_RET(retVal, drv_memset(&vlanSet, 0, sizeof(vlanSet)));
    if(RT_ERR_OK == retVal)
    {
        vlanSet.vid                      = 100u;
        vlanSet.vlanEntry.mbrMsk         = 0x0C; /* port3, port2 */
        vlanSet.vlanEntry.utagMsk        = 0u;
        vlanSet.vlanEntry.fid            = 0u;
        vlanSet.vlanEntry.ulfid          = EM_VLAN_IVL;
        vlanSet.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
        vlanSet.vlanEntry.interfaceId    = 0u;
        vlanSet.vlanEntry.interfaceValid = 0u;
    }
    CHK_FUN_RET(retVal, rtk_vlan_set(uintChip, &vlanSet));

    CHK_FUN_RET(retVal, drv_memset(&vlanSet, 0, sizeof(vlanSet)));
    if(RT_ERR_OK == retVal)
    {
        vlanSet.vid                      = 200u;
        vlanSet.vlanEntry.mbrMsk         = 0x02; /* port1 */
        vlanSet.vlanEntry.utagMsk        = 0u;
        vlanSet.vlanEntry.fid            = 0u;
        vlanSet.vlanEntry.ulfid          = EM_VLAN_IVL;
        vlanSet.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
        vlanSet.vlanEntry.interfaceId    = 0u;
        vlanSet.vlanEntry.interfaceValid = 0u;
    }
    CHK_FUN_RET(retVal, rtk_vlan_set(uintChip, &vlanSet));

    /* Expected Behavior: Multicast packets with vid 200 will be flooded to port1 and port2 but not port0. */

    /* Step 3: Enable Cross VLAN. */
    crossSet.enable = 1u;
    CHK_FUN_RET(retVal, rtk_igmpmld_crossvlan_ability_set(uintChip, &crossSet));
    /* Expected Behavior: Multicast packets with vid 200 will be flooded to port1, port2, and also port3. */

    return retVal;
}
/**@}*/ /* RTL907CVB_IGMP_GFS_EXAMPLE_4 */
/**@}*/ /* RTL907CVB_IGMP_GFS_EXAMPLE */

/**
 * @defgroup RTL907CVB_IGMP_AGING_EXAMPLE Aging Time Example
 * @{
 */
/**
 * @defgroup RTL907CVB_IGMP_AGING_EXAMPLE_1 Set the group member aging time
 * @brief This example enables IGMP and MLD with a specific group aging time.
 *
 * @section Purpose
 * This shows how to define the group member aging time.
 *
 * @section Description
 * After this execution, IGMP and MLD will be enabled, and the group aging time will be set to 60 seconds.
 *
 * @{
 */
RtkApiRet cvb_igmp_mld_grpagingtime(uint32 unit)
{
    RtkApiRet          retVal = RT_ERR_OK;
    UnitChip_t         uintChip;
    IgmpInfo_t         igmpSet;
    MldInfo_t          mldSet;
    GroupAgeTimeInfo_t groupSet;
    uintChip.unit = unit;
    uintChip.chip = CHIP_907XC_VB;

    /* Step 1: Enable IGMP and MLD. */
    igmpSet.enable = 1u;
    mldSet.enable  = 1u;
    CHK_FUN_RET(retVal, rtk_igmpmld_igmp_ability_set(uintChip, &igmpSet));
    CHK_FUN_RET(retVal, rtk_igmpmld_mld_ability_set(uintChip, &mldSet));
    /* Expected Behavior: Multicast packets will be forwarded to the ports which have IGMP join coming. */

    /* Step 2: Set group member aging time. */
    groupSet.grpTime = 60u;
    CHK_FUN_RET(retVal, rtk_igmpmld_grpmemagingtime_set(uintChip, &groupSet));
    /* Expected Behavior: Multicast packets will stop forwarding after 60 seconds if there is no IGMP report coming on the port. */

    return retVal;
}
/**@}*/ /* RTL907CVB_IGMP_AGING_EXAMPLE_1 */

/**
 * @defgroup RTL907CVB_IGMP_AGING_EXAMPLE_2 Set the last member aging time
 * @brief This example enables IGMP and MLD with a specific last member aging time.
 *
 * @section Purpose
 * This shows how to define the last member aging time.
 *
 * @section Description
 * After this execution, IGMP and MLD will be enabled, and the last member aging time will be set to 5 seconds.
 *
 * @{
 */
RtkApiRet cvb_igmp_mld_lastmemagingtime(uint32 unit)
{
    RtkApiRet         retVal = RT_ERR_OK;
    UnitChip_t        uintChip;
    IgmpInfo_t        igmpSet;
    MldInfo_t         mldSet;
    FastLeaveInfo_t   fastSet;
    LastAgeTimeInfo_t lastSet;
    uintChip.unit = unit;
    uintChip.chip = CHIP_907XC_VB;

    /* Step 1: Enable IGMP and MLD. */
    igmpSet.enable = 1u;
    mldSet.enable  = 1u;
    CHK_FUN_RET(retVal, rtk_igmpmld_igmp_ability_set(uintChip, &igmpSet));
    CHK_FUN_RET(retVal, rtk_igmpmld_mld_ability_set(uintChip, &mldSet));

    /* Step 2: Disable Fast Leave. */
    fastSet.enable = 0u;
    CHK_FUN_RET(retVal, rtk_igmpmld_fastleave_ability_set(uintChip, &fastSet));

    /* Step 3: Set last member aging time. */
    lastSet.lstTime = 5u;
    CHK_FUN_RET(retVal, rtk_igmpmld_lastmemagingtime_set(uintChip, &lastSet));
    /* Expected Behavior: Multicast will stop forwarding after 5 seconds once the last member leaves. */

    return retVal;
}
/**@}*/ /* RTL907CVB_IGMP_AGING_EXAMPLE_2 */
/**@}*/ /* RTL907CVB_IGMP_AGING_EXAMPLE */

/**
 * @defgroup RTL907CVB_IGMP_DROP_EXAMPLE L2 Drop Example
 * @brief This example enables L2 Default Drop.
 *
 * @section Purpose
 * This shows how to enable L2 Default Drop.
 *
 * @section Description
 * Because there is a conflict between enabling IGMP/MLD and L2 Default Drop, \n
 * IGMP/MLD must be disabled first, then L2 Default Drop can be enabled. \n
 * It is not allowed to enable IGMP/MLD and L2 Default Drop at the same time.
 *
 * @{
 */
RtkApiRet cvb_igmp_mld_l2defaultdrop(uint32 unit)
{
    RtkApiRet      retVal = RT_ERR_OK;
    UnitChip_t     uintChip;
    IgmpInfo_t     igmpSet;
    MldInfo_t      mldSet;
    L2D4DropInfo_t dropSet;
    uintChip.unit = unit;
    uintChip.chip = CHIP_907XC_VB;

    /* Step 1: Disable IGMP and MLD. */
    igmpSet.enable = 0u;
    mldSet.enable  = 0u;
    CHK_FUN_RET(retVal, rtk_igmpmld_igmp_ability_set(uintChip, &igmpSet));
    CHK_FUN_RET(retVal, rtk_igmpmld_mld_ability_set(uintChip, &mldSet));
    /* Expected Behavior: Multicast packets will be flooded to all ports. */

    /* Step 2: Enable L2 default drop. */
    dropSet.drop = 1u;
    CHK_FUN_RET(retVal, rtk_igmpmld_l2defaultdrop_set(uintChip, &dropSet));
    /* Expected Behavior: Multicast packets will stop forwarding on all ports. */

    return retVal;
}
/**@}*/ /* RTL907CVB_IGMP_DROP_EXAMPLE */

/**
 * @defgroup RTL907CVB_IGMP_CUST_EXAMPLE Customization Lookup Miss Example
 * @brief This example enables L2 customized lookup miss.
 *
 * @section Purpose
 * This shows how to enable customized L2 lookup miss ability.
 *
 * @section Description
 * Because this conflicts with IGMP/MLD and L2 Default Drop, \n
 * IGMP/MLD and L2 Default Drop must be disabled first, then L2 customized lookup miss can be enabled.
 *
 * @{
 */
RtkApiRet cvb_igmp_mld_l2custmzlumiss(uint32 unit)
{
    RtkApiRet         retVal = RT_ERR_OK;
    UnitChip_t        uintChip;
    IgmpInfo_t        igmpSet;
    MldInfo_t         mldSet;
    L2D4DropInfo_t    dropSet;
    L2CustomizeInfo_t cusSet, cusGet;
    uintChip.unit = unit;
    uintChip.chip = CHIP_907XC_VB;

    do
    {
        /* Step 1: Disable IGMP and MLD. */
        igmpSet.enable = 0u;
        mldSet.enable  = 0u;
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_igmp_ability_set(uintChip, &igmpSet));
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_mld_ability_set(uintChip, &mldSet));

        /* Step 2: Disable L2 default drop. */
        dropSet.drop = 0u;
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_l2defaultdrop_set(uintChip, &dropSet));

        /* Step 3: Enable L2 customized lookup miss. */
        cusSet.enable = 1u;
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_l2custmzlumiss_set(uintChip, &cusSet));

        /* Step 4: Check L2 customized lookup miss ability. */
        CHK_FUN_CONTINUE(retVal, rtk_igmpmld_l2custmzlumiss_get(uintChip, &cusGet));
        if(cusGet.enable != cusSet.enable)
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /* Step 5: Configure actions of ports when lookup miss
           port 1: drop
           port 2: flooding in vlan
           port 3: flooding
         */
        CHK_FUN_CONTINUE(retVal, reg_write(uintChip, CVB_PORT1_ADDRESS_TABLE_LOOKUP_MISS_CONTROL, 0u));
        CHK_FUN_CONTINUE(retVal, reg_write(uintChip, CVB_PORT2_ADDRESS_TABLE_LOOKUP_MISS_CONTROL, 0x20u));
        CHK_FUN_CONTINUE(retVal, reg_write(uintChip, CVB_PORT3_ADDRESS_TABLE_LOOKUP_MISS_CONTROL, 0x30u));

        /* Expected Behavior: When lookup miss occurs:
           port 1 should drop packets
           port 2 should let packets flood in VLAN
           port 3 should let packets flood
         */

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907CVB_IGMP_CUST_EXAMPLE */
/**@}*/ /* RTL907CVB_IGMP_EXAMPLE */
/**@}*/ /* IGMP_EXAMPLE */
/**@}*/ /* EXAMPLE */