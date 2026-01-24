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
 * @addtogroup cva_vlan_assign_tag Assign Untagged Frame from Specified Port to the VLAN Example
 *
 * @brief This example demonstrates how to assign untagged frames from specified port to VLAN.
 *
 * @section Purpose
 *      To demostrate how to use APIs to assign the VLAN tag of PORT_1ST/PORT_2ND based on port-based VLAN ID and 802.1p VLAN priority.
 *
 * @section Description
 *      After executing this example, the SWITCH assigns untagged frames from PORT_1ST/PORT_2ND to VLAN 1/2, and tagged frames keep the original VLAN tag format. \n
 *      The following figure/table corresponds to the untagged actions. \n
 *      Before using the API, users should call rtk_vlan_set to set the member portmask and untag portmask for the VLAN. \n
 *
 * @image html vlan/vlan_assign_tag_2.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_vlan_assign_tag(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_vlan_assign_tag(void)
{
    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t         unitChip         = {0};
    VlanInfo_t         vlanInfo         = {0};
    VlanPortPvidInfo_t vlanPortPvidInfo = {0};

    unitChip.unit = CVA_UNIT;
    unitChip.chip = CHIP_907XC_VA;

    /* Step 1: Set PORT_1ST and PORT_3RD as member set to VLAN 1 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 1u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_3RD);
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behavior: Frames with VID 1 will be flooded to PORT_1ST and PORT_3RD. */

    /* Step 2: Set PORT_2ND and PORT_4TH as member set to VLAN 2 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 2u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVA_RTK_USER_PMAP_2ND | CVA_RTK_USER_PMAP_4TH);
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behavior: Frames with VID 2 will be flooded to PORT_2ND and PORT_4TH. */

    /* Step 3: Set PORT_2ND and PORT_4TH as member set to VLAN 10 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 10u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVA_RTK_USER_PMAP_2ND | CVA_RTK_USER_PMAP_4TH);
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behavior: Frames with VID 10 will be flooded to PORT_2ND and PORT_4TH. */

    /* Step 4: Set PORT_1ST PVID to 1 and 802.1p priority of PVID 1 to 0 */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)CVA_RTK_USER_PORT_1ST;
    vlanPortPvidInfo.pvid     = 1u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));
    /* Expected Behavior: The untagged frames from PORT_1ST will be flooded to PORT_1ST and PORT_3RD. */

    /* Step 5: Set PORT_2ND PVID to 2 and 802.1p priority of PVID 2 to 0 */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)CVA_RTK_USER_PORT_2ND;
    vlanPortPvidInfo.pvid     = 2u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));
    /* Expected Behavior: The untagged frames from PORT_2ND will be flooded to PORT_2ND and PORT_4TH. */

    return retVal;
}
/**@}*/ /* cva_vlan_assign_tag */
/**@}*/ /* RTL907XCVA_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */