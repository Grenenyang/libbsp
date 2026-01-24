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
#include "rtkas_api_vlan.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_vlan.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_vlan.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_vlan.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_vb_api_vlan.h"
#endif
#include "rtl907xc_vb_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup VLAN_EXAMPLE VLAN Example
 * @{
 */
/**
 * @addtogroup RTL907XCVB_VLAN_EXAMPLE RTL907XC_VB VLAN Example
 * @{
 */
/**
 * @addtogroup cvb_vlan_egress_retagging Egress Retagging Example
 *
 * @brief This example demonstrates how to retag inner VLAN tag on egress port.
 *
 * @section Purpose
 *      To demostrate how to use APIs to retag inner VLAN tag on egress port.
 *
 * @section Description
 *        1. Enable egress filter. \n
 *        2. Set VLAN table. \n
 *        3. Configure egress retagging. \n
 *        4. Reset all egress rules. \n
 *        5. Enable egress retagging. \n
 *        6. Set an egress rule. \n
 *
 * @image html vlan/vlan_egress_retagging.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_vlan_egress_retagging(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_vlan_egress_retagging(void)
{
    RtkApiRet retVal   = RT_ERR_OK;
    RtkPort   outPort1 = CVB_RTK_USER_PORT_2ND;
    RtkPort   outPort2 = CVB_RTK_USER_PORT_3RD;
    RtkPmap   pbm      = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND); /* PORT_1ST, PORT_2ND */

    UnitChip_t               unitChip               = {0};
    VlanPortEgrFilterInfo_t  vlanPortEgrFilterInfo  = {0};
    VlanInfo_t               vlanInfo               = {0};
    VlanEgrRetaggingEnInfo_t vlanEgrRetaggingEnInfo = {0};
    VlanEgrRetaggingInfo_t   vlanEgrRetaggingInfo   = {0};

    unitChip.unit = CVB_UNIT;
    unitChip.chip = CHIP_907XC_VB;

    /* Step 1: Enable egress filter on PORT_2ND and PORT_3RD */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrFilterInfo, 0, sizeof(vlanPortEgrFilterInfo)));
    vlanPortEgrFilterInfo.port      = outPort1;
    vlanPortEgrFilterInfo.egrFilter = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_filter_en_set(unitChip, &vlanPortEgrFilterInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrFilterInfo, 0, sizeof(vlanPortEgrFilterInfo)));
    vlanPortEgrFilterInfo.port      = outPort2;
    vlanPortEgrFilterInfo.egrFilter = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_filter_en_set(unitChip, &vlanPortEgrFilterInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrFilterInfo, 0, sizeof(vlanPortEgrFilterInfo)));
    vlanPortEgrFilterInfo.port = outPort2;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_filter_en_get(unitChip, &vlanPortEgrFilterInfo));

    /* Step 2: Set VLAN 1 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 1u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD); /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(CVB_RTK_USER_PMAP_1ST);                                                 /* PORT_1ST */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 3: Reset all egress retagging rules */
    CHK_FUN_RET(retVal, rtk_vlan_egr_retagging_init(unitChip));

    /* Step 4: Enable the egress retagging on PORT_2ND */
    CHK_FUN_RET(retVal, drv_memset(&vlanEgrRetaggingEnInfo, 0, sizeof(vlanEgrRetaggingEnInfo)));
    vlanEgrRetaggingEnInfo.port   = outPort1;
    vlanEgrRetaggingEnInfo.enable = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_egr_retagging_en_set(unitChip, &vlanEgrRetaggingEnInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanEgrRetaggingEnInfo, 0, sizeof(vlanEgrRetaggingEnInfo)));
    vlanEgrRetaggingEnInfo.port = outPort1;
    CHK_FUN_RET(retVal, rtk_vlan_egr_retagging_en_get(unitChip, &vlanEgrRetaggingEnInfo));

    /* Step 5: Set and get an egress retagging rule to retag VID 1 with VID 2 */
    CHK_FUN_RET(retVal, drv_memset(&vlanEgrRetaggingInfo, 0, sizeof(vlanEgrRetaggingInfo)));
    vlanEgrRetaggingInfo.idx         = 0u;
    vlanEgrRetaggingInfo.originalVid = 1u;
    vlanEgrRetaggingInfo.newVid      = 2u;
    vlanEgrRetaggingInfo.pbm         = pbm;
    vlanEgrRetaggingInfo.valid       = 1u;
    CHK_FUN_RET(retVal, rtk_vlan_egr_retagging_set(unitChip, &vlanEgrRetaggingInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanEgrRetaggingInfo, 0, sizeof(vlanEgrRetaggingInfo)));
    vlanEgrRetaggingInfo.idx = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_egr_retagging_get(unitChip, &vlanEgrRetaggingInfo));

    /* Expected Behavior:
     *   The frames with VID 1 from PORT_1ST will be forwarded to PORT_2ND and tagged VID 2.
     *   The frames with VID 1 from PORT_1ST will be forwarded to PORT_3RD and tagged VID 1.
     */

    return retVal;
}
/**@}*/ /* cvb_vlan_egress_retagging */
/**@}*/ /* RTL907XCVB_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */