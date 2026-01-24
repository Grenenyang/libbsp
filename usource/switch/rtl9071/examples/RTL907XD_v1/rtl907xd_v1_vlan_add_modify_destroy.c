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
 * @defgroup VLAN_EXAMPLE VLAN Example
 * @{
 */
/**
 * @defgroup RTL907XDV1_VLAN_EXAMPLE RTL907XD_V1 VLAN Example
 * @{
 */
/**
 * @defgroup dv1_vlan_add_modify_destroy Add/Modify/Destroy the VLAN Example
 *
 * @brief This example demonstrates how to configure VLAN entries.
 *
 * @section Purpose
 *      To demostrate how to use APIs to set/clear/get/delete VLAN entries.
 *
 * @section Description
 *      The following table corresponds to these four actions.
 *
 * @image html vlan/vlan_add_modify_destroy.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_vlan_add_modify_destroy(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_vlan_add_modify_destroy(void)
{
    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t        unitChip        = {0};
    VlanInfo_t        vlanInfo        = {0};
    VlanDestroyInfo_t vlanDestroyInfo = {0};

    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;

    /* Step 1: Set PORT_1ST, PORT_2ND, PORT_3RD, and PORT_4TH as member set and PORT_3RD, PORT_4TH as untag set for VLAN 100 with FID 0 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH);
    vlanInfo.vlanEntry.utagMsk        = (uint32)(DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH);
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behavior: Frames with VID 100 will be flooded to PORT_1ST, PORT_2ND, PORT_3RD, and PORT_4TH. */

    /* Step 2: Clear member portmask and untag portmask for VLAN 100 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = 0x0u;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behavior: Frames with VID 100 will be dropped. */

    /* Step 3: Get VLAN entry of VLAN 100 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid = 100u;
    CHK_FUN_RET(retVal, rtk_vlan_get(unitChip, &vlanInfo));

    /* Step 4: Delete the VLAN with VID 100 */
    CHK_FUN_RET(retVal, drv_memset(&vlanDestroyInfo, 0, sizeof(vlanDestroyInfo)));
    vlanDestroyInfo.vid = 100u;
    CHK_FUN_RET(retVal, rtk_vlan_destroy(unitChip, &vlanDestroyInfo));
    /* Expected Behavior: Frames with VID 100 will be dropped. */

    return retVal;
}
/**@}*/ /* dv1_vlan_add_modify_destroy */
/**@}*/ /* RTL907XDV1_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */