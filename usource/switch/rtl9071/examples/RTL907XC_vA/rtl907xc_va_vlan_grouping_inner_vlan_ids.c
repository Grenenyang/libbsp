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
#include "rtl907xc_va_api_vlan.h"
#endif
#include "rtl907xc_va_reg_struct.h"
/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup VLAN_EXAMPLE VLAN Example
 * @{
 */
/**
 * @addtogroup RTL907XCVA_VLAN_EXAMPLE RTL907XC_VA VLAN Example
 * @{
 */
/**
 * @addtogroup cva_vlan_grouping_inner_vlan_ids Grouping Inner VLAN IDs Example
 *
 * @brief This example demonstrates how to setup group inner VLAN IDs.
 *
 * @section Purpose
 *      To demostrate how to use APIs to setup group inner VLAN IDs.
 *
 * @section Description
 *      The following figure/table corresponds to the VLAN configurations.
 *
 * @image html vlan/vlan_grouping_inner_vlan_ids_1.jpg
 * @image html vlan/vlan_grouping_inner_vlan_ids_2.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_vlan_grouping_inner_vlan_ids(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_vlan_grouping_inner_vlan_ids(void)
{
    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t                 unitChip                 = {0};
    VlanInfo_t                 vlanInfo                 = {0};
    VlanPortPvidInfo_t         vlanPortPvidInfo         = {0};
    VlanPortAcptTypeInfo_t     vlanPortAcptTypeInfo     = {0};
    VlanPortEgrTagStatusInfo_t vlanPortEgrTagStatusInfo = {0};

    unitChip.unit = CVA_UNIT;
    unitChip.chip = CHIP_907XC_VA;

    /* Step 1: Create VLANs */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 10u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_2ND); /* PORT_1ST, PORT_2ND */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(CVA_RTK_USER_PMAP_1ST);                         /* PORT_1ST */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 20u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVA_RTK_USER_PMAP_2ND | CVA_RTK_USER_PMAP_3RD); /* PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(CVA_RTK_USER_PMAP_3RD);                         /* PORT_3RD */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_2ND | CVA_RTK_USER_PMAP_3RD | CVA_RTK_USER_PMAP_4TH); /* PORT_1ST, PORT_2ND, PORT_3RD, PORT_4TH */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(CVA_RTK_USER_PMAP_4TH);                                                                         /* PORT_4TH */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 2: Set and get default VID */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)CVA_RTK_USER_PORT_1ST;
    vlanPortPvidInfo.pvid     = 10u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port = (RtkPort)CVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_get(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)CVA_RTK_USER_PORT_2ND;
    vlanPortPvidInfo.pvid     = 1u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)CVA_RTK_USER_PORT_3RD;
    vlanPortPvidInfo.pvid     = 20u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)CVA_RTK_USER_PORT_4TH;
    vlanPortPvidInfo.pvid     = 100u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    /* Step 3: Set and get accept frame type */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortAcptTypeInfo, 0, sizeof(vlanPortAcptTypeInfo)));
    vlanPortAcptTypeInfo.port            = (RtkPort)CVA_RTK_USER_PORT_1ST;
    vlanPortAcptTypeInfo.acceptFrameType = EM_ACCEPT_FRAME_TYPE_ALL;
    CHK_FUN_RET(retVal, rtk_vlan_port_acpt_type_set(unitChip, &vlanPortAcptTypeInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortAcptTypeInfo, 0, sizeof(vlanPortAcptTypeInfo)));
    vlanPortAcptTypeInfo.port = (RtkPort)CVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_vlan_port_acpt_type_get(unitChip, &vlanPortAcptTypeInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortAcptTypeInfo, 0, sizeof(vlanPortAcptTypeInfo)));
    vlanPortAcptTypeInfo.port            = (RtkPort)CVA_RTK_USER_PORT_2ND;
    vlanPortAcptTypeInfo.acceptFrameType = EM_ACCEPT_FRAME_TYPE_TAG_ONLY;
    CHK_FUN_RET(retVal, rtk_vlan_port_acpt_type_set(unitChip, &vlanPortAcptTypeInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortAcptTypeInfo, 0, sizeof(vlanPortAcptTypeInfo)));
    vlanPortAcptTypeInfo.port            = (RtkPort)CVA_RTK_USER_PORT_3RD;
    vlanPortAcptTypeInfo.acceptFrameType = EM_ACCEPT_FRAME_TYPE_ALL;
    CHK_FUN_RET(retVal, rtk_vlan_port_acpt_type_set(unitChip, &vlanPortAcptTypeInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortAcptTypeInfo, 0, sizeof(vlanPortAcptTypeInfo)));
    vlanPortAcptTypeInfo.port            = (RtkPort)CVA_RTK_USER_PORT_4TH;
    vlanPortAcptTypeInfo.acceptFrameType = EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY;
    CHK_FUN_RET(retVal, rtk_vlan_port_acpt_type_set(unitChip, &vlanPortAcptTypeInfo));

    /* Step 4: Set and get egress tag status */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrTagStatusInfo, 0, sizeof(vlanPortEgrTagStatusInfo)));
    vlanPortEgrTagStatusInfo.port      = (RtkPort)CVA_RTK_USER_PORT_1ST;
    vlanPortEgrTagStatusInfo.tagStatus = EM_TAG_STATUS_WITH_TAG;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_tag_status_set(unitChip, &vlanPortEgrTagStatusInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrTagStatusInfo, 0, sizeof(vlanPortEgrTagStatusInfo)));
    vlanPortEgrTagStatusInfo.port = (RtkPort)CVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_tag_status_get(unitChip, &vlanPortEgrTagStatusInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrTagStatusInfo, 0, sizeof(vlanPortEgrTagStatusInfo)));
    vlanPortEgrTagStatusInfo.port      = (RtkPort)CVA_RTK_USER_PORT_2ND;
    vlanPortEgrTagStatusInfo.tagStatus = EM_TAG_STATUS_WITH_TAG;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_tag_status_set(unitChip, &vlanPortEgrTagStatusInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrTagStatusInfo, 0, sizeof(vlanPortEgrTagStatusInfo)));
    vlanPortEgrTagStatusInfo.port      = (RtkPort)CVA_RTK_USER_PORT_3RD;
    vlanPortEgrTagStatusInfo.tagStatus = EM_TAG_STATUS_WITH_TAG;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_tag_status_set(unitChip, &vlanPortEgrTagStatusInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortEgrTagStatusInfo, 0, sizeof(vlanPortEgrTagStatusInfo)));
    vlanPortEgrTagStatusInfo.port      = (RtkPort)CVA_RTK_USER_PORT_4TH;
    vlanPortEgrTagStatusInfo.tagStatus = EM_TAG_STATUS_WITHOUT_TAG;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_tag_status_set(unitChip, &vlanPortEgrTagStatusInfo));

    /* Expected Behavior:
     *   The untagged frame from PORT_1ST will be added an inner tag <IVID 10>, and then forwarded to PORT_2ND.
     *   The tagged frame with IVID 10 from PORT_1ST will be forwarded to PORT_2ND.
     *   The untagged frame from PORT_2ND will be dropped.
     *   The tagged frame with IVID 10 from PORT_2ND will be untagged, and then forwarded to PORT_1ST.
     */

    return retVal;
}
/**@}*/ /* cva_vlan_grouping_inner_vlan_ids */
/**@}*/ /* RTL907XCVA_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */