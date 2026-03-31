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
 * @addtogroup cvb_vlan_ingress_retagging Ingress Retagging Example
 *
 * @brief This example demonstrates how to retag inner VLAN tag on ingress port.
 *
 * @section Purpose
 *      To demostrate how to use APIs to retag inner VLAN tag on ingress port.
 *
 * @section Description
 *        1. Enable ingress filter. \n
 *        2. Set VLAN table. \n
 *        3. Configure ingress retagging. \n
 *        4. Reset all ingress rules. \n
 *        5. Enable ingress retagging. \n
 *        6. Set an ingress rule. \n
 *
 * @image html vlan/vlan_ingress_retagging.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_vlan_ingress_retagging(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_vlan_ingress_retagging(void)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   inPort = CVB_RTK_USER_PORT_1ST;
    RtkPmap   pbm    = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND);  // PORT_1ST and PORT_2ND

    UnitChip_t               unitChip               = {0};
    VlanPortIgrFilterInfo_t  VlanPortIgrFilterInfo  = {0};
    VlanInfo_t               VlanInfo               = {0};
    VlanIgrRetaggingEnInfo_t VlanIgrRetaggingEnInfo = {0};
    VlanIgrRetaggingInfo_t   VlanIgrRetaggingInfo   = {0};

    unitChip.unit = CVB_UNIT;
    unitChip.chip = CHIP_907XC_VB;

    /* Step 1: Enable ingress filter on PORT_1ST */
    VlanPortIgrFilterInfo.port      = inPort;
    VlanPortIgrFilterInfo.igrFilter = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_port_igr_filter_en_set(unitChip, &VlanPortIgrFilterInfo));

    CHK_FUN_RET(retVal, drv_memset(&VlanPortIgrFilterInfo, 0, sizeof(VlanPortIgrFilterInfo)));
    VlanPortIgrFilterInfo.port = inPort;
    CHK_FUN_RET(retVal, rtk_vlan_port_igr_filter_en_get(unitChip, &VlanPortIgrFilterInfo));

    /* Step 2: Set VLAN 2 */
    VlanInfo.vid                      = 2u;
    VlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND); /* PORT_1ST, PORT_2ND */
    VlanInfo.vlanEntry.utagMsk        = (uint32)(CVB_RTK_USER_PMAP_1ST);                         /* PORT_1ST */
    VlanInfo.vlanEntry.fid            = 0u;
    VlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    VlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    VlanInfo.vlanEntry.interfaceId    = 0u;
    VlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &VlanInfo));

    /* Step 3: Reset all ingress retagging rules */
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_init(unitChip));

    /* Step 4: Enable the ingress retagging on PORT_1ST */
    VlanIgrRetaggingEnInfo.port   = inPort;
    VlanIgrRetaggingEnInfo.enable = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_en_set(unitChip, &VlanIgrRetaggingEnInfo));

    CHK_FUN_RET(retVal, drv_memset(&VlanIgrRetaggingEnInfo, 0, sizeof(VlanIgrRetaggingEnInfo)));
    VlanIgrRetaggingEnInfo.port = inPort;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_en_get(unitChip, &VlanIgrRetaggingEnInfo));

    /* Step 5: Set and get an ingress retagging rule to retag VID 1 with VID 2 */
    VlanIgrRetaggingInfo.idx         = 0u;
    VlanIgrRetaggingInfo.originalVid = 1u;
    VlanIgrRetaggingInfo.newVid      = 2u;
    VlanIgrRetaggingInfo.pbm         = pbm;
    VlanIgrRetaggingInfo.valid       = 1u;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_set(unitChip, &VlanIgrRetaggingInfo));

    CHK_FUN_RET(retVal, drv_memset(&VlanIgrRetaggingInfo, 0, sizeof(VlanIgrRetaggingInfo)));
    VlanIgrRetaggingInfo.idx = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_get(unitChip, &VlanIgrRetaggingInfo));

    /* Expected Behavior: The frames with VID 1 from PORT_1ST will be retagged with VID 2 and flooded in VLAN member ports. */

    return retVal;
}
/**@}*/ /* cvb_vlan_ingress_retagging */
/**@}*/ /* RTL907XCVB_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */