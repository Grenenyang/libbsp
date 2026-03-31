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
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_l2.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_l2.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_l2.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_vb_api_l2.h"
#endif
#include "rtkas_api_l2.h"
#include "rtl907xc_vb_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @defgroup L2_EXAMPLE L2 Example
 * @{
 */
/**
 * @defgroup RTL907XCVB_L2_EXAMPLE RTL907XC_VB L2 Example
 * @{
 */
/**
 * @defgroup cvb_l2_fwd Forwarding Frames Based on L2 Example
 *
 * @brief This example demonstrates how to configure L2 entries.
 *
 * @section Purpose
 *      To demostrate how to use APIs to configure L2 entries to forward unicast/multicast/IP destination address.
 *
 * @section Description
 *        1. Disable address learning. \n
 *        2. Enable address learning. \n
 *        3. Get hash algorithm. \n
 *        4. Add three L2 entries. \n
 *        5. Set VLAN table. \n
 *        6. Search the unicast entry. \n
 *        7. Get the unicast entry, the multicast entry, and the IP multicast entry. \n
 *        8. Delete L2 entries. \n
 *        9. Set L2 unicast lookup miss action. \n
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_l2_fwd(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_l2_fwd(void)
{
    RtkApiRet   retVal                 = RT_ERR_OK;
    const uint8 uniMac[MAC_ADDR_LEN]   = {0x82, 0x34, 0x56, 0x78, 0x9A, 0xBD};
    const uint8 multiMac[MAC_ADDR_LEN] = {0x81, 0x34, 0x56, 0x78, 0x9A, 0xBD};
    const uint8 sip1[IPV4_ADDR_LEN]    = {192, 168, 0, 1};
    const uint8 dip1[IPV4_ADDR_LEN]    = {224, 0, 0, 1};
    const uint8 sip2[IPV4_ADDR_LEN]    = {192, 168, 0, 2};
    const uint8 dip2[IPV4_ADDR_LEN]    = {224, 0, 0, 2};
    uint8       inPort                 = CVB_RTK_USER_PORT_1ST;
    uint8       outPort                = CVB_RTK_USER_PORT_2ND;
    RtkPmap     rtkPortmask            = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND); /* PORT_1ST, PORT_2ND */

    UnitChip_t              unitChip              = {0};
    LutMacLrnInfo_t         lutMacLrnInfo         = {0};
    LutHashAlgoInfo_t       lutHashAlgoInfo       = {0};
    LutUniAddInfo_t         lutUniAddInfo         = {0};
    LutMultiAddInfo_t       lutMultiAddInfo       = {0};
    LutIpMultiInfo_t        lutIpMultiInfo        = {0};
    VlanInfo_t              vlanInfo              = {0};
    LutSearchInfo_t         lutSearchInfo         = {0};
    LutGetInfo_t            lutGetInfo            = {0};
    LutMultiGetInfo_t       lutMultiGetInfo       = {0};
    LutCamGetInfo_t         lutCamGetInfo         = {0};
    LutDeleteInfo_t         lutDeleteInfo         = {0};
    LutIpMultiDeleteInfo_t  lutIpMultiDeleteInfo  = {0};
    LutLookupMissCtrlInfo_t lutLookupMissCtrlInfo = {0};

    unitChip.unit = CVB_UNIT;
    unitChip.chip = CHIP_907XC_VB;

    /* Step 1: Disable address learning */
    CHK_FUN_RET(retVal, drv_memset(&lutMacLrnInfo, 0, sizeof(lutMacLrnInfo)));
    lutMacLrnInfo.portmask = rtkPortmask;
    lutMacLrnInfo.enable   = 0u;
    CHK_FUN_RET(retVal, rtk_lut_mac_lrn_en(unitChip, &lutMacLrnInfo));

    /* Step 2: Address learning should be disabled and re-enabled to validate the hash algorithm setting. */
    CHK_FUN_RET(retVal, drv_memset(&lutMacLrnInfo, 0, sizeof(lutMacLrnInfo)));
    lutMacLrnInfo.portmask = rtkPortmask;
    lutMacLrnInfo.enable   = 1u;
    CHK_FUN_RET(retVal, rtk_lut_mac_lrn_en(unitChip, &lutMacLrnInfo));

    /* Step 3: Get hash algorithm */
    CHK_FUN_RET(retVal, drv_memset(&lutHashAlgoInfo, 0, sizeof(lutHashAlgoInfo)));
    CHK_FUN_RET(retVal, rtk_lut_hash_algo_get(unitChip, &lutHashAlgoInfo));

    /* Step 4: Add a unicast entry, a multicast entry, and two IP multicast entries */
    CHK_FUN_RET(retVal, drv_memset(&lutUniAddInfo, 0, sizeof(lutUniAddInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutUniAddInfo.mac, uniMac, sizeof(lutUniAddInfo.mac)));
    lutUniAddInfo.vid   = 6u;
    lutUniAddInfo.port  = outPort;
    lutUniAddInfo.fid   = 0u;
    lutUniAddInfo.ulfid = 0u;
    CHK_FUN_RET(retVal, rtk_lut_uni_entry_add(unitChip, &lutUniAddInfo));  // index = 3992

    CHK_FUN_RET(retVal, drv_memset(&lutMultiAddInfo, 0, sizeof(lutMultiAddInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutMultiAddInfo.mac, multiMac, sizeof(lutMultiAddInfo.mac)));
    lutMultiAddInfo.vid      = 0u;
    lutMultiAddInfo.portmask = rtkPortmask;
    lutMultiAddInfo.fid      = 0u;
    CHK_FUN_RET(retVal, rtk_lut_multi_entry_add(unitChip, &lutMultiAddInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutIpMultiInfo, 0, sizeof(lutIpMultiInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiInfo.sip, sip1, sizeof(lutIpMultiInfo.sip)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiInfo.dip, dip1, sizeof(lutIpMultiInfo.dip)));
    lutIpMultiInfo.portmask = rtkPortmask;
    CHK_FUN_RET(retVal, rtk_lut_ip_multi_entry_add(unitChip, &lutIpMultiInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutIpMultiInfo, 0, sizeof(lutIpMultiInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiInfo.sip, sip2, sizeof(lutIpMultiInfo.sip)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiInfo.dip, dip2, sizeof(lutIpMultiInfo.dip)));
    lutIpMultiInfo.portmask = rtkPortmask;
    CHK_FUN_RET(retVal, rtk_lut_ip_multi_entry_add(unitChip, &lutIpMultiInfo));

    /* Step 5: Set VLAN 6 */
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = 6u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND);                                                                                                 /* PORT_1ST, PORT_2ND */
    vlanInfo.vlanEntry.utagMsk        = (uint32)(CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_4TH | CVB_RTK_USER_PMAP_5TH | CVB_RTK_USER_PMAP_6TH); /* PORT_1ST, PORT_2ND, PORT_3RD, PORT_4TH, PORT_5TH, PORT_6TH */
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 6: Search the unicast entry */
    CHK_FUN_RET(retVal, drv_memset(&lutSearchInfo, 0, sizeof(lutSearchInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutSearchInfo.macAddr, uniMac, sizeof(lutSearchInfo.macAddr)));
    CHK_FUN_RET(retVal, rtk_lut_entry_search(unitChip, &lutSearchInfo));

    /* Step 7: Get the unicast entry and the IP multicast entry from the get function */
    CHK_FUN_RET(retVal, drv_memset(&lutGetInfo, 0, sizeof(lutGetInfo)));
    lutGetInfo.index = 3992u;
    CHK_FUN_RET(retVal, rtk_lut_entry_get(unitChip, &lutGetInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutMultiGetInfo, 0, sizeof(lutMultiGetInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutMultiGetInfo.macAddr, multiMac, sizeof(lutMultiGetInfo.macAddr)));
    lutMultiGetInfo.fid = 0u;
    CHK_FUN_RET(retVal, rtk_lut_multi_entry_get(unitChip, &lutMultiGetInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutIpMultiInfo, 0, sizeof(lutIpMultiInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiInfo.sip, sip1, sizeof(lutIpMultiInfo.sip)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiInfo.dip, dip1, sizeof(lutIpMultiInfo.dip)));
    CHK_FUN_RET(retVal, rtk_lut_ip_multi_entry_get(unitChip, &lutIpMultiInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutCamGetInfo, 0, sizeof(lutCamGetInfo)));
    lutCamGetInfo.index = 0u;
    CHK_FUN_RET(retVal, rtk_lut_cam_entry_get(unitChip, &lutCamGetInfo));

    /* Step 8: Delete the entries */
    CHK_FUN_RET(retVal, drv_memset(&lutDeleteInfo, 0, sizeof(lutDeleteInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutDeleteInfo.mac, uniMac, sizeof(lutDeleteInfo.mac)));
    lutDeleteInfo.vid   = 6u;
    lutDeleteInfo.fid   = 0u;
    lutDeleteInfo.ulfid = 0u;
    CHK_FUN_RET(retVal, rtk_lut_entry_delete(unitChip, &lutDeleteInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutIpMultiDeleteInfo, 0, sizeof(lutIpMultiDeleteInfo)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiDeleteInfo.sip, sip2, sizeof(lutIpMultiDeleteInfo.sip)));
    CHK_FUN_RET(retVal, drv_memcpy(lutIpMultiDeleteInfo.dip, dip2, sizeof(lutIpMultiDeleteInfo.dip)));
    CHK_FUN_RET(retVal, rtk_lut_ip_multi_entry_delete(unitChip, &lutIpMultiDeleteInfo));

    /* Step 9: Set L2 unicast lookup miss to drop */
    CHK_FUN_RET(retVal, drv_memset(&lutLookupMissCtrlInfo, 0, sizeof(lutLookupMissCtrlInfo)));
    lutLookupMissCtrlInfo.port   = inPort;
    lutLookupMissCtrlInfo.type   = CVB_LM_L2_UNICAST;
    lutLookupMissCtrlInfo.action = CVB_LM_ACTION_DROP;
    CHK_FUN_RET(retVal, rtk_lut_lookup_miss_ctrl(unitChip, &lutLookupMissCtrlInfo));

    /* Expected Behavior:
     *   The frame with <DA, vid>= <82:34:56:78:9A:BD, 6> will be dropped.
     *   The frame with <DA, vid>= <81:34:56:78:9A:BD, 6> will be forwarded to PORT_1ST and PORT_2ND.
     *   The packet with <SIP, DIP>= <192.168.0.1, 224.0.0.1> will be forwarded to PORT_1ST and PORT_2ND.
     *   The packet with <SIP, DIP>= <192.168.0.2, 224.0.0.2> will be dropped.
     */

    return retVal;
}
/**@}*/ /* cvb_l2_fwd */
/**@}*/ /* RTL907XCVB_L2_EXAMPLE */
/**@}*/ /* L2_EXAMPLE */
/**@}*/ /* EXAMPLE */