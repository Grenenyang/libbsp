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
#include "rtkas_api_dot1x.h"

/**
 * @addtogroup DOT1X_EXAMPLE DOT1X Example
 * @{
 */
/**
 * @addtogroup RTL907XCVB_DOT1X_EXAMPLE RTL907XC_VB DOT1X Example
 * @{
 */

/**
 * @defgroup RTL907XCVB_DOT1X_SET_OUTERTAG_UNAUTHBE_EXAMPLE Dot1x Set Outer-tag Unauthorized Behavior Example
 *
 * @brief <b>Set unauthorized outer-tagged and outer-untagged packet behavior.</b> \n
 *           The switch will trap unauthorized outer-tagged packet to port 11,  \n
 *           and send unauthorized outer-untagged or outer priority-tagged packet to guest VLAN 500.
 *
 * @section Purpose
 *          This example demonstrates how to set unauthorized outer-tagged packet behavior.
 * @section Description
 *          If the switch receives an outer-tagged packet from an unauthenticated port, it will trap these packets to the CPU port. \n
 *          If the switch receives an outer untagged or outer priority-tagged packet from an unauthenticated port, it will send these packets to guest VLAN 500. \n
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_dot1x_set_outertag_unauthbe(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_dot1x_set_outertag_unauthbe(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};

    Dot1xGuestVidEntryInfo_t dot1xGuestVidEntryInfo;
    CHK_FUN_RET(retVal, drv_memset(&dot1xGuestVidEntryInfo, 0, sizeof(Dot1xGuestVidEntryInfo_t)));
    dot1xGuestVidEntryInfo.vid                       = 500;
    dot1xGuestVidEntryInfo.pVlanEntry.mbrMsk         = CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD;
    dot1xGuestVidEntryInfo.pVlanEntry.utagMsk        = 0x0u;
    dot1xGuestVidEntryInfo.pVlanEntry.fid            = 0u;
    dot1xGuestVidEntryInfo.pVlanEntry.ulfid          = EM_VLAN_IVL;
    dot1xGuestVidEntryInfo.pVlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    dot1xGuestVidEntryInfo.pVlanEntry.interfaceId    = 0u;
    dot1xGuestVidEntryInfo.pVlanEntry.interfaceValid = 0u;
    Dot1xEnPortMaskInfo_t dot1xEnPortMaskInfo        = {
               .portMask = 0xFF6u,
    };
    Dot1xCpuPMaskInfo_t dot1xCpuPMaskInfo = {
        .cpuPortMask = CVB_RTK_USER_PMAP_11TH,
    };
    SvlanFwdBaseOvidInfo_t svlanFwdBaseOvidInfo = {
        .port   = CVB_RTK_USER_PORT_1ST,
        .enable = ENABLED,
    };
    SvlanPortPktParserInfo_t svlanPortPktParserInfo = {
        .port   = CVB_RTK_USER_PORT_1ST,
        .enable = ENABLED,
    };
    Dot1xUnauthPktActionInfo_t dot1xUnauthPktActionInfo0 = {
        .pktType      = CVB_DOT1X_PKT_OUTTAG,
        .unauthAction = CVB_DOT1X_ACTION_TRAP2CPU,
    };
    Dot1xUnauthPktActionInfo_t dot1xUnauthPktActionInfo1 = {
        .pktType      = CVB_DOT1X_PKT_OUTUNTAG,
        .unauthAction = CVB_DOT1X_ACTION_GUESTVLAN,
    };
    /* Step 1: Enable DOT1X */
    CHK_FUN_RET(retVal, rtk_dot1x_set_enable_portmask(unitChip, &dot1xEnPortMaskInfo));
    /* Step 2: Set CPU port */
    CHK_FUN_RET(retVal, rtk_dot1x_cpuportmask_set(unitChip, &dot1xCpuPMaskInfo));
    /* Step 3: Set the forwarding VLAN tag as the outer VID */
    CHK_FUN_RET(retVal, rtk_svlan_fwd_base_ovid_set(unitChip, &svlanFwdBaseOvidInfo));
    /* Step 4: Enable the outer-tag parser ability */
    CHK_FUN_RET(retVal, rtk_svlan_port_pkt_parser_en_set(unitChip, &svlanPortPktParserInfo));
    /* Step 5: Set guest VLAN entry */
    CHK_FUN_RET(retVal, rtk_dot1x_guest_vlan_entry_set(unitChip, &dot1xGuestVidEntryInfo));
    /* Step 6: Set unauthorized behavior for outer-tagged packets */
    CHK_FUN_RET(retVal, rtk_dot1x_unauth_packet_oper_set(unitChip, &dot1xUnauthPktActionInfo0));
    /* Step 7: Set unauthorized behavior for outer-untagged or outer priority-tagged frames */
    CHK_FUN_RET(retVal, rtk_dot1x_unauth_packet_oper_set(unitChip, &dot1xUnauthPktActionInfo1));
    /* Step 8: Trigger the dynamic setting flag to set the new configuration to the switch */
    CHK_FUN_RET(retVal, rtk_dot1x_set_dynamic_reset_flag(unitChip));
    /* Expected Behavior: The switch will trap unauthorized outer-tagged packets to the CPU port.
     *                    The switch will send unauthorized outer-untagged packets to Guest VLAN 500.
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_DOT1X_SET_OUTERTAG_UNAUTHBE_EXAMPLE */
/* Function */
/**
 * @defgroup RTL907XCVB_DOT1X_SET_INNERTAG_UNAUTHBE_EXAMPLE Dot1x Set Inner-tag Unauthorized Behavior Example
 *
 * @brief <b>Set unauthorized inner-tagged and inner-untagged packet behavior.</b> \n
 *           The switch will drop unauthorized inner-tagged packet,  \n
 *           and send unauthorized inner-untagged or inner priority-tagged packet to guest VLAN 500.
 *
 * @section Purpose
 *          This example demonstrates how to set unauthorized inner-tagged packet behavior.
 * @section Description
 *          If the switch receives an inner-tagged packet from an unauthenticated port, it will drop these packets. \n
 *          If the switch receives an inner-untagged or inner priority-tagged packet from an unauthenticated port, it will send these packets to guest VLAN 500. \n
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_dot1x_set_innertag_unauthbe(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_dot1x_set_innertag_unauthbe(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VB};

    Dot1xGuestVidEntryInfo_t dot1xGuestVidEntryInfo;
    CHK_FUN_RET(retVal, drv_memset(&dot1xGuestVidEntryInfo, 0, sizeof(Dot1xGuestVidEntryInfo_t)));
    dot1xGuestVidEntryInfo.vid                       = 500;
    dot1xGuestVidEntryInfo.pVlanEntry.mbrMsk         = CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD;
    dot1xGuestVidEntryInfo.pVlanEntry.utagMsk        = 0x0u;
    dot1xGuestVidEntryInfo.pVlanEntry.fid            = 0u;
    dot1xGuestVidEntryInfo.pVlanEntry.ulfid          = EM_VLAN_IVL;
    dot1xGuestVidEntryInfo.pVlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    dot1xGuestVidEntryInfo.pVlanEntry.interfaceId    = 0u;
    dot1xGuestVidEntryInfo.pVlanEntry.interfaceValid = 0u;

    Dot1xEnPortMaskInfo_t dot1xEnPortMaskInfo = {
        .portMask = 0xFF6u,
    };
    SvlanFwdBaseOvidInfo_t svlanFwdBaseOvidInfo = {
        .port   = CVB_RTK_USER_PORT_1ST,
        .enable = DISABLED,
    };
    SvlanPortPktParserInfo_t svlanPortPktParserInfo = {
        .port   = CVB_RTK_USER_PORT_1ST,
        .enable = DISABLED,
    };
    Dot1xUnauthPktActionInfo_t dot1xUnauthPktActionInfo0 = {
        .pktType      = CVB_DOT1X_PKT_INTAG,
        .unauthAction = CVB_DOT1X_ACTION_DROP,
    };
    Dot1xUnauthPktActionInfo_t dot1xUnauthPktActionInfo1 = {
        .pktType      = CVB_DOT1X_PKT_INUNTAG,
        .unauthAction = CVB_DOT1X_ACTION_GUESTVLAN,
    };
    /* Step 1: Enable DOT1X */
    CHK_FUN_RET(retVal, rtk_dot1x_set_enable_portmask(unitChip, &dot1xEnPortMaskInfo));
    /* Step 2: Set the forwarding VLAN tag the innerr VID  */
    CHK_FUN_RET(retVal, rtk_svlan_fwd_base_ovid_set(unitChip, &svlanFwdBaseOvidInfo));
    /* Step 3: Enable the inner-tag parser ability */
    CHK_FUN_RET(retVal, rtk_svlan_port_pkt_parser_en_set(unitChip, &svlanPortPktParserInfo));
    /* Step 4: Set guest VLAN entry */
    CHK_FUN_RET(retVal, rtk_dot1x_guest_vlan_entry_set(unitChip, &dot1xGuestVidEntryInfo));
    /* Step 5: Set unauthorized behavior for inner-tagged packets */
    CHK_FUN_RET(retVal, rtk_dot1x_unauth_packet_oper_set(unitChip, &dot1xUnauthPktActionInfo0));
    /* Step 6: Set unauthorized behavior for inner-untagged or inner priority-tagged frames */
    CHK_FUN_RET(retVal, rtk_dot1x_unauth_packet_oper_set(unitChip, &dot1xUnauthPktActionInfo1));
    /* Step 7: Trigger the dynamic setting flag to set the new configuration to the switch */
    CHK_FUN_RET(retVal, rtk_dot1x_set_dynamic_reset_flag(unitChip));
    /* Expected Behavior: The switch will drop unauthorized inner-tagged packets.
     *                    The switch will send unauthorized inner untagged packets to Guest VLAN 500.
     */
    return retVal;
}
/**@}*/ /* RTL907XCVB_DOT1X_SET_INNERTAG_UNAUTHBE_EXAMPLE */
/**@}*/ /* RTL907XCVB_DOT1X_EXAMPLE */
/**@}*/ /* DOT1X_EXAMPLE */
