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
#include "rtkas_api_cb.h"
#include "rtl907xd_va_api_tsn.h"
#include "rtl907xd_va_api_cb.h"
#include "rtkas_api_vlan.h"

/**
 * @addtogroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_CB_EXAMPLE RTL907XD_VA CB Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_CB_EGRESS_REPLICATION_EXTEND_DPM_EXAMPLE CB Egress Replication Extend DPM Example
 *
 * @brief <b> Replicate the stream to the original destination ports and CB destination ports.</b>  \n
 *            Achieve stream redundancy within the topology by duplicating specific streams to the original destination ports \n
 *            and CB destination ports without changing the content of frames.
 *
 * @section Purpose
 *          This example demonstrates how to replicate streams to original destination port mask \n
 *          and CB destination port mask for redundancy.
 *
 * @section Description
 *          The host sends a stream without an R-tag, with a VID of 10 and a VLAN priority of 2, to the switch's port1 \n
 *          to match the stream identification. The switch will add an R-tag to the stream and replicate it \n
 *          to the original destination port (port 2) and the CB destination port (port 3). \n
 *          The sequence number of the egress packets will increase continuously from 0. \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_cb_egress_replication_extend_dpm(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_cb_egress_replication_extend_dpm(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};
    /* Config information */
    /* VLAN 10 */
    VlanInfo_t vlanInfo;
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(VlanInfo_t)));
    vlanInfo.vid                      = 10u;
    vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    /* Global */
    CbEnabledInfo_t cbEnabledInfo = {
        .status = ENABLED,
    };
    CbSidEnabledInfo_t cbSidEnabledInfo = {
        .status = ENABLED,
    };
    CbParseRtagEnabledInfo_t cbParseRtagEnabledInfo = {
        .port   = DVA_RTK_USER_PORT_1ST,
        .status = ENABLED,
    };
    CbSplitDpmModeInfo_t cbSplitDpmModeInfo = {
        .splitDpmMode = DVA_FRER_SPLIT_DPM_EXTEND,
    };
    /* Stream identification */
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index         = 3u,
        .sidentryParam = {
            .carebits     = DVA_TSN_SELECTED_FILED_VID | DVA_TSN_SELECTED_FIELD_VIDPRI,
            .vlanId       = 10u,
            .vlanPri      = 2,
            .portMask     = DVA_RTK_USER_PMAP_1ST,
            .isValid      = ENABLED,
            .streamHandle = 7u,
        },
    };
    CbMiscInfo_t CbMiscInfo = {
        .index       = cbIngressPassiveSidInfo.index,
        .lutTabParam = {
            .indRcvEn    = DISABLED,
            .seqRcvEn    = DISABLED,
            .splitEn     = ENABLED,
            .splitDPM    = DVA_RTK_USER_PMAP_3RD,
            .genSeqNumEn = ENABLED,
            .activeEn    = DISABLED,
            .encEn       = ENABLED,
        },
    };

    /* Step 1. Set VLAN entry 10 */
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Step 2. Enable CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step 3. Enable passive stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step 4.Enable the R-tag parser to extract the sequence number from ingress packet */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));
    /* Step 5. Set the mode for splitting destination port mask to extend mode */
    CHK_FUN_RET(retVal, rtk_cb_splitdpm_mode_set(unitChip, &cbSplitDpmModeInfo));

    /* Step 6. Set passive stream identification entry at port1 to match ingress packets */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step 7. Enable splitting function, and set destination port mask
     *         Enable sequence number generation
     *         Add R-tag to egress packets
     */
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &CbMiscInfo));

    /* Expected Behavior: The switch will forward the same streams to port2, port3
     *
     */
    /* Get split DPM mode to check */
    CHK_FUN_RET(retVal, drv_memset(&cbSplitDpmModeInfo, 0, sizeof(CbSplitDpmModeInfo_t)));
    CHK_FUN_RET(retVal, rtk_cb_splitdpm_mode_get(unitChip, &cbSplitDpmModeInfo));
    return retVal;
}
/**@}*/ /* RTL907XDVA_CB_EGRESS_REPLICATION_EXTEND_DPM_EXAMPLE */

/**
 * @defgroup RTL907XDVA_CB_EGRESS_REPLICATION_OVERWRITE_DPM_EXAMPLE CB Egress Replication Overwrite DPM Example
 *
 * @brief <b> Replicate the stream to the CB destination ports.</b> \n
 *            Achieve stream redundancy by duplicating specific streams exclusively to the CB destination ports \n
 *            without changing the content of frames.
 *
 * @section Purpose
 *          This example demonstrates how to replicate streams only to CB destination port mask for redundancy.
 *
 * @section Description
 *          The host sends a stream without an R-tag, with a VID of 10 and a VLAN priority of 2, to the switch's port1 \n
 *          to match the stream identification. The switch will add an R-tag to the stream and replicate it only \n
 *          to the CB destination port (port 3). \n
 *          The sequence number of the egress packets will increase continuously from 0. \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_cb_egress_replication_overwrite_dpm(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_cb_egress_replication_overwrite_dpm(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};
    /* Config information */
    /* VLAN 10 */
    VlanInfo_t vlanInfo;
    CHK_FUN_RET(retVal, drv_memset(&vlanInfo, 0, sizeof(VlanInfo_t)));
    vlanInfo.vid                      = 10u;
    vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND;
    vlanInfo.vlanEntry.utagMsk        = 0x0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;

    /* Global */
    CbEnabledInfo_t cbEnabledInfo = {
        .status = ENABLED,
    };
    CbSidEnabledInfo_t cbSidEnabledInfo = {
        .status = ENABLED,
    };
    CbParseRtagEnabledInfo_t cbParseRtagEnabledInfo = {
        .port   = DVA_RTK_USER_PORT_1ST,
        .status = ENABLED,
    };
    CbSplitDpmModeInfo_t cbSplitDpmModeInfo = {
        .splitDpmMode = DVA_FRER_SPLIT_DPM_OVERWRITE,
    };
    /* Stream identification */
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index         = 3u,
        .sidentryParam = {
            .carebits     = DVA_TSN_SELECTED_FILED_VID | DVA_TSN_SELECTED_FIELD_VIDPRI,
            .vlanId       = 10u,
            .vlanPri      = 2,
            .portMask     = DVA_RTK_USER_PMAP_1ST,
            .isValid      = ENABLED,
            .streamHandle = 7u,
        },
    };
    CbMiscInfo_t CbMiscInfo = {
        .index       = cbIngressPassiveSidInfo.index,
        .lutTabParam = {
            .indRcvEn    = DISABLED,
            .seqRcvEn    = DISABLED,
            .splitEn     = ENABLED,
            .splitDPM    = DVA_RTK_USER_PMAP_3RD,
            .genSeqNumEn = ENABLED,
            .activeEn    = DISABLED,
            .encEn       = ENABLED,
        },
    };

    /* Step 1. Set VLAN entry 10 */
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));
    /* Step 2. Enable CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step 3. Enable passive stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step 4.Enable the R-tag parser to extract the sequence number from ingress packet */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));
    /* Step 5. Set the mode for splitting destination port mask to overwrite mode */
    CHK_FUN_RET(retVal, rtk_cb_splitdpm_mode_set(unitChip, &cbSplitDpmModeInfo));

    /* Step 6. Set passive stream identification entry at port1 to match ingress packets */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step 7. Enable splitting function, and set destination port mask
     *         Enable sequence number generation
     *         Add R-tag to egress packets
     */
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &CbMiscInfo));

    /* Expected Behavior: The switch will forward the same streams to port3
     *
     */
    /* Get split DPM mode to check */
    CHK_FUN_RET(retVal, drv_memset(&cbSplitDpmModeInfo, 0, sizeof(CbSplitDpmModeInfo_t)));
    CHK_FUN_RET(retVal, rtk_cb_splitdpm_mode_get(unitChip, &cbSplitDpmModeInfo));
    return retVal;
}
/**@}*/ /* RTL907XDVA_CB_EGRESS_REPLICATION_OVERWRITE_DPM_EXAMPLE */
/**@}*/ /* RTL907XDVA_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */
