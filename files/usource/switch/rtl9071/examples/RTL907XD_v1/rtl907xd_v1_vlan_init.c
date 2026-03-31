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
 * @addtogroup dv1_vlan_initialization Initialization Example
 *
 * @brief This example demonstrates how to initialize global VLAN and per-port VLAN, and then modify them.
 *
 * @section Purpose
 *      To demostrate how to use APIs to initialize global VLAN and per-port VLAN, and then modify them.
 *
 * @section Description
 *      The following table corresponds to the VLAN configurations.
 *
 * @image html vlan/vlan_initialization.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_vlan_initialization(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_vlan_initialization(void)
{
    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t         unitChip         = {0};
    VlanInfo_t         vlanInfo         = {0};
    VlanPortPvidInfo_t vlanPortPvidInfo = {0};

    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;

    /* Step 1: Initialize VLAN */
    CHK_FUN_RET(retVal, rtk_vlan_init(unitChip));
    /* Expected Behavior: Frames with VID 1 will be flooded to all ports. */

    /* Step 2: All the ports are in the default VLAN 1 after VLAN is initialized.
     * Modify it as follows:
     * VLAN 1 members: PORT_1ST, PORT_2ND, PORT_3RD
     * VLAN 2 members: PORT_4TH, PORT_5TH, PORT_6TH
     * Untagged ports: PORT_1ST, PORT_2ND, PORT_3RD, PORT_4TH, PORT_5TH, PORT_6TH
     */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 100u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD);                                                                         /* PORT_1ST, PORT_2ND, PORT_3RD */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH | DV1_RTK_USER_PMAP_5TH | DV1_RTK_USER_PMAP_6TH); /* PORT_1ST, PORT_2ND, PORT_3RD, PORT_4TH, PORT_5TH, PORT_6TH */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 200u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DV1_RTK_USER_PMAP_4TH | DV1_RTK_USER_PMAP_5TH | DV1_RTK_USER_PMAP_6TH);                                                                         /* PORT_4TH, PORT_5TH, PORT_6TH */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH | DV1_RTK_USER_PMAP_5TH | DV1_RTK_USER_PMAP_6TH); /* PORT_1ST, PORT_2ND, PORT_3RD, PORT_4TH, PORT_5TH, PORT_6TH */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behavior:
     *    Frames with VID 100 will be flooded to PORT_1ST, PORT_2ND, and PORT_3RD.
     *    Frames with VID 200 will be flooded to PORT_4TH, PORT_5TH, and PORT_6TH.
     */

    /* Step 3: Set PVID for each port */
    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)DV1_RTK_USER_PORT_1ST;
    vlanPortPvidInfo.pvid     = 100u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)DV1_RTK_USER_PORT_2ND;
    vlanPortPvidInfo.pvid     = 100u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)DV1_RTK_USER_PORT_3RD;
    vlanPortPvidInfo.pvid     = 100u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)DV1_RTK_USER_PORT_4TH;
    vlanPortPvidInfo.pvid     = 200u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)DV1_RTK_USER_PORT_5TH;
    vlanPortPvidInfo.pvid     = 200u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));

    CHK_FUN_RET(retVal, drv_memset(&vlanPortPvidInfo, 0, sizeof(vlanPortPvidInfo)));
    vlanPortPvidInfo.port     = (RtkPort)DV1_RTK_USER_PORT_6TH;
    vlanPortPvidInfo.pvid     = 200u;
    vlanPortPvidInfo.priority = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_pvid_set(unitChip, &vlanPortPvidInfo));
    /* Expected Behavior:
     *    The untagged frames from PORT_1ST, PORT_2ND, and PORT_3RD will be flooded in VLAN 100.
     *    The untagged frames from PORT_4TH, PORT_5TH, and PORT_6TH will be flooded in VLAN 200.
     */

    return retVal;
}
/**@}*/ /* dv1_vlan_initialization */
/**@}*/ /* RTL907XDV1_VLAN_EXAMPLE */
/**@}*/ /* VLAN_EXAMPLE */
/**@}*/ /* EXAMPLE */