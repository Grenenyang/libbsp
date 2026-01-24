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
 * @addtogroup cvb_vlan_fwd_outer_tag Forwarding Based on Outer Tag of Port-based VLAN Example
 *
 * @brief This example demonstrates how to configure settings to forward frames based on outer VLAN tag of port-based VLAN.
 *
 * @section Purpose
 *      To demostrate how to use APIs to configure settings to forward frames based on outer VLAN tag of port-based VLAN.
 *
 * @section Description
 *        1. Enable outer tag parser. \n
 *        2. Accept double tag. \n
 *        3. Set accept frame type. \n
 *        4. Ignore outer tag and use port-based as outer tag. \n
 *        5. Set egress tag status. \n
 *        6. Set VLAN table. \n
 *        7. Set forwarding behavior based on outer tag. \n
 *
 * @image html vlan/vlan_fwd_outer_tag.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_vlan_fwd_outer_tag(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_vlan_fwd_outer_tag(void)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = CVB_RTK_USER_PORT_1ST;

    UnitChip_t                  unitChip                  = {0};
    SvlanPortPktParserInfo_t    svlanPortPktParserInfo    = {0};
    VlanPortAcptDoubleTagInfo_t vlanPortAcptDoubleTagInfo = {0};
    SvlanPortAcptTypeInfo_t     svlanPortAcptTypeInfo     = {0};
    VlanTagAwareInfo_t          vlanTagAwareInfo          = {0};
    SvlanPortPvidInfo_t         svlanPortPvidInfo         = {0};
    SvlanPortEgrTagStatusInfo_t svlanPortEgrTagStatusInfo = {0};
    VlanInfo_t                  vlanInfo                  = {0};
    SvlanFwdBaseOvidInfo_t      svlanFwdBaseOvidInfo      = {0};

    unitChip.unit = CVB_UNIT;
    unitChip.chip = CHIP_907XC_VB;

    /* Step 1: Enable outer tag parser */
    CHK_FUN_RET(retVal, drv_memset(&svlanPortPktParserInfo, 0, sizeof(svlanPortPktParserInfo)));
    svlanPortPktParserInfo.port   = port;
    svlanPortPktParserInfo.enable = ENABLED;
    CHK_FUN_RET(retVal, rtk_svlan_port_pkt_parser_en_set(unitChip, &svlanPortPktParserInfo));

    CHK_FUN_RET(retVal, drv_memset(&svlanPortPktParserInfo, 0, sizeof(svlanPortPktParserInfo)));
    svlanPortPktParserInfo.port = port;
    CHK_FUN_RET(retVal, rtk_svlan_port_pkt_parser_en_get(unitChip, &svlanPortPktParserInfo));

    /* Step 2: Permit double-tagged frame to enter the switch */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortAcptDoubleTagInfo, 0, sizeof(vlanPortAcptDoubleTagInfo)));
    vlanPortAcptDoubleTagInfo.port            = port;
    vlanPortAcptDoubleTagInfo.acceptDoubleTag = EM_ACCEPT_DOUBLE_TAG;
    CHK_FUN_RET(retVal, rtk_vlan_port_acpt_double_tag_set(unitChip, &vlanPortAcptDoubleTagInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortAcptDoubleTagInfo, 0, sizeof(vlanPortAcptDoubleTagInfo)));
    vlanPortAcptDoubleTagInfo.port = port;
    CHK_FUN_RET(retVal, rtk_vlan_port_acpt_double_tag_get(unitChip, &vlanPortAcptDoubleTagInfo));

    /* Step 3: Set and get accept frame type */
    CHK_FUN_RET(retVal, drv_memset(&svlanPortAcptTypeInfo, 0, sizeof(svlanPortAcptTypeInfo)));
    svlanPortAcptTypeInfo.port            = port;
    svlanPortAcptTypeInfo.acceptFrameType = EM_ACCEPT_FRAME_TYPE_ALL;
    CHK_FUN_RET(retVal, rtk_svlan_port_acpt_type_set(unitChip, &svlanPortAcptTypeInfo));

    CHK_FUN_RET(retVal, drv_memset(&svlanPortAcptTypeInfo, 0, sizeof(svlanPortAcptTypeInfo)));
    svlanPortAcptTypeInfo.port = port;
    CHK_FUN_RET(retVal, rtk_svlan_port_acpt_type_get(unitChip, &svlanPortAcptTypeInfo));

    /* Step 4: Ignore outer tag and use port-based VLAN as outer tag */
    CHK_FUN_RET(retVal, drv_memset(&vlanTagAwareInfo, 0, sizeof(vlanTagAwareInfo)));
    vlanTagAwareInfo.port    = port;
    vlanTagAwareInfo.enabled = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_tag_aware_set(unitChip, &vlanTagAwareInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanTagAwareInfo, 0, sizeof(vlanTagAwareInfo)));
    vlanTagAwareInfo.port = port;
    CHK_FUN_RET(retVal, rtk_vlan_tag_aware_get(unitChip, &vlanTagAwareInfo));

    CHK_FUN_RET(retVal, drv_memset(&svlanPortPvidInfo, 0, sizeof(svlanPortPvidInfo)));
    svlanPortPvidInfo.port     = port;
    svlanPortPvidInfo.pvid     = 100u;
    svlanPortPvidInfo.priority = 0u;
    svlanPortPvidInfo.dei      = 0u;
    CHK_FUN_RET(retVal, rtk_svlan_port_pvid_set(unitChip, &svlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&svlanPortPvidInfo, 0, sizeof(svlanPortPvidInfo)));
    svlanPortPvidInfo.port = port;
    CHK_FUN_RET(retVal, rtk_svlan_port_pvid_get(unitChip, &svlanPortPvidInfo));

    /* Step 5: Set egress tag status */
    CHK_FUN_RET(retVal, drv_memset(&svlanPortEgrTagStatusInfo, 0, sizeof(svlanPortEgrTagStatusInfo)));
    svlanPortEgrTagStatusInfo.port      = port;
    svlanPortEgrTagStatusInfo.tagStatus = EM_TAG_STATUS_WITHOUT_TAG;
    CHK_FUN_RET(retVal, rtk_svlan_port_egr_tag_status_set(unitChip, &svlanPortEgrTagStatusInfo));

    CHK_FUN_RET(retVal, drv_memset(&svlanPortEgrTagStatusInfo, 0, sizeof(svlanPortEgrTagStatusInfo)));
    svlanPortEgrTagStatusInfo.port = port;
    CHK_FUN_RET(retVal, rtk_svlan_port_egr_tag_status_get(unitChip, &svlanPortEgrTagStatusInfo));

    /* Step 6: Set VLAN table */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND); /* PORT_1ST, PORT_2ND */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND); /* PORT_1ST, PORT_2ND */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 7: Set and get forwarding behavior based on outer tag */
    CHK_FUN_RET(retVal, drv_memset(&svlanFwdBaseOvidInfo, 0, sizeof(svlanFwdBaseOvidInfo)));
    svlanFwdBaseOvidInfo.port   = port;
    svlanFwdBaseOvidInfo.enable = ENABLED;
    CHK_FUN_RET(retVal, rtk_svlan_fwd_base_ovid_set(unitChip, &svlanFwdBaseOvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&svlanFwdBaseOvidInfo, 0, sizeof(svlanFwdBaseOvidInfo)));
    svlanFwdBaseOvidInfo.port = port;
    CHK_FUN_RET(retVal, rtk_svlan_fwd_base_ovid_get(unitChip, &svlanFwdBaseOvidInfo));

    /* Expected Behavior: The double-tagged frame with <IVID 1, OVID 100> from PORT_1ST will be forwarded to PORT_2ND and only tagged IVID 1. */

    return retVal;
}
/**@}*/ /* cvb_vlan_fwd_outer_tag */
/**@}*/ /* RTL907XCVB_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */