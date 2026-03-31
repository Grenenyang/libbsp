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
#include "rtl907xd_v1_api_vlan.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xd_v1_api_vlan.h"
#endif
#include "rtl907xd_v1_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup VLAN_EXAMPLE VLAN Example
 * @{
 */
/**
 * @addtogroup RTL907XDV1_VLAN_EXAMPLE RTL907XD_V1 VLAN Example
 * @{
 */
/**
 * @addtogroup dv1_vlan_ingress_retagging Ingress Retagging Example
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
RtkApiRet dv1_vlan_ingress_retagging(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_vlan_ingress_retagging(void)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   inPort = DV1_RTK_USER_PORT_1ST;
    RtkPmap   pbm    = (uint32)(DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND);  // PORT_1ST and PORT_2ND

    UnitChip_t               unitChip               = {0};
    VlanPortIgrFilterInfo_t  vlanPortIgrFilterInfo  = {0};
    VlanInfo_t               vlanInfo               = {0};
    VlanIgrRetaggingEnInfo_t vlanIgrRetaggingEnInfo = {0};
    VlanIgrRetaggingInfo_t   vlanIgrRetaggingInfo   = {0};

    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;

    /* Step 1: Enable ingress filter on PORT_1ST */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortIgrFilterInfo, 0, sizeof(vlanPortIgrFilterInfo)));
    vlanPortIgrFilterInfo.port      = inPort;
    vlanPortIgrFilterInfo.igrFilter = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_port_igr_filter_en_set(unitChip, &vlanPortIgrFilterInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortIgrFilterInfo, 0, sizeof(vlanPortIgrFilterInfo)));
    vlanPortIgrFilterInfo.port = inPort;
    CHK_FUN_RET(retVal, rtk_vlan_port_igr_filter_en_get(unitChip, &vlanPortIgrFilterInfo));

    /* Step 2: Set VLAN 2 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 2u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND); /* PORT_1ST, PORT_2ND */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(DV1_RTK_USER_PMAP_1ST);                         /* PORT_1ST */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 3: Reset all ingress retagging rules */
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_init(unitChip));

    /* Step 4: Enable the ingress retagging on PORT_1ST */
    CHK_FUN_RET(retVal, drv_memset(&vlanIgrRetaggingEnInfo, 0, sizeof(vlanIgrRetaggingEnInfo)));
    vlanIgrRetaggingEnInfo.port   = inPort;
    vlanIgrRetaggingEnInfo.enable = ENABLED;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_en_set(unitChip, &vlanIgrRetaggingEnInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanIgrRetaggingEnInfo, 0, sizeof(vlanIgrRetaggingEnInfo)));
    vlanIgrRetaggingEnInfo.port = inPort;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_en_get(unitChip, &vlanIgrRetaggingEnInfo));

    /* Step 5: Set and get an ingress retagging rule to retag VID 1 with VID 2 */
    CHK_FUN_RET(retVal, drv_memset(&vlanIgrRetaggingInfo, 0, sizeof(vlanIgrRetaggingInfo)));
    vlanIgrRetaggingInfo.idx         = 0u;
    vlanIgrRetaggingInfo.originalVid = 1u;
    vlanIgrRetaggingInfo.newVid      = 2u;
    vlanIgrRetaggingInfo.pbm         = pbm;
    vlanIgrRetaggingInfo.valid       = 1u;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_set(unitChip, &vlanIgrRetaggingInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanIgrRetaggingInfo, 0, sizeof(vlanIgrRetaggingInfo)));
    vlanIgrRetaggingInfo.idx = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_igr_retagging_get(unitChip, &vlanIgrRetaggingInfo));

    /* Expected Behavior: The frames with VID 1 from PORT_1ST will be retagged with VID 2 and flooded in VLAN member ports. */

    return retVal;
}
/**@}*/ /* dv1_vlan_ingress_retagging */
/**@}*/ /* RTL907XDV1_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */