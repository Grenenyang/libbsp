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
#include "rtkas_api_acl_ext.h"

/**
 * @addtogroup CB_EXAMPLE CB Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_CB_EXAMPLE RTL907XD_VA CB Example
 * @{
 */

/**
 * @defgroup RTL907XDVA_CB_INGRESS_PASSSIVE_SID_UDP_PORT_RANGE_EXAMPLE CB Ingress Passive Stream Identification UDP Port Range Check Example
 *
 * @brief <b> Duplicate UDP packets within a specific L4 port range. </b> \n
 *            Achieve redundancy of the stream by duplicating UDP packets within a specific L4 port range to the CB destination ports, \n
 *            without altering the content of the frames.
 *
 * @section Purpose
 *          This example demonstrates how to duplicate UDP packets within a specific L4 port range.
 *
 * @section Description
 *          The host sends a stream without R-tag whose Destination UDP port is 1812 and Source UDP port is 500 to switch's port1 \n
 *          to match stream identification, or send a stream without R-tag whose Destination UDP port is 1813 and Source UDP port \n
 *          is 12222 to switch's port1 to match stream identification, The switch will add R-tag to the stream and forward it \n
 *          to port2 and port3, \n
 *          The sequence number of the egress packets increases continuously from 0. \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_cb_ingress_passivesid_udp_port_range(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_cb_ingress_passivesid_udp_port_range(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};
    /* Config information */
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
    /* Stream identification */
    AclL4PortRangeInfo_t aclL4PortRangeInfo0 = {
        .entryIdx = 0u,
        .type     = DVA_L4PORT_RANGE_UDP_SOURCE_PORT,
        .upper    = 12223u,
        .lower    = 500u,
    };
    AclL4PortRangeInfo_t aclL4PortRangeInfo1 = {
        .entryIdx = 1u,
        .type     = DVA_L4PORT_RANGE_UDP_DEST_PORT,
        .upper    = 1813u,
        .lower    = 1812u,
    };
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index         = 3u,
        .sidentryParam = {
            .carebits        = DVA_TSN_SELECTED_FIELD_L4PORTRANGE,
            .l4PortChkTblMap = 0x3,
            .portMask        = DVA_RTK_USER_PMAP_1ST,
            .isValid         = ENABLED,
            .streamHandle    = 7u,
        },
    };
    CbMiscInfo_t CbMiscInfo = {
        .index       = cbIngressPassiveSidInfo.index,
        .lutTabParam = {
            .indRcvEn    = DISABLED,
            .seqRcvEn    = DISABLED,
            .splitEn     = ENABLED,
            .splitDPM    = DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD,
            .genSeqNumEn = ENABLED,
            .activeEn    = DISABLED,
            .encEn       = ENABLED,
        },
    };
    /* Step 1. Set ACL L4 port range entry */
    CHK_FUN_RET(retVal, rtk_acl_l4_port_range_set(unitChip, &aclL4PortRangeInfo0));
    CHK_FUN_RET(retVal, rtk_acl_l4_port_range_set(unitChip, &aclL4PortRangeInfo1));
    /* Step 2. Enable CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step 3. Enable passive stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step 4. Enable R-tag parser */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));

    /* Step 5. Set passive stream identification entry */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step 6. Enable splitting function, and set DPM
     *         Enable sequence number generation
     *         Add R-tag at egress port
     */
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &CbMiscInfo));

    /* Expected Behavior: The switch will forward the same streams to port2 and port3
     *
     */
    return retVal;
}
/**@}*/ /* RTL907XDVA_CB_INGRESS_PASSSIVE_SID_UDP_PORT_RANGE_EXAMPLE */

/**
 * @defgroup RTL907XDVA_CB_INGRESS_PASSSIVE_SID_UDPTCP_PORT_RANGE_EXAMPLE CB Ingress Passive Stream Identification UDP And TCP Port Range Check Example
 *
 * @brief <b> Duplicate UDP or TCP packets within a specific L4 port range. </b> \n
 *           Achieve redundancy of the stream by duplicating UDP or TCP packets within a specific L4 port range to the CB destination ports, \n
 *            without changing the content of the frames.
 *
 * @section Purpose
 *          This example demonstrates how to duplicate UDP or TCP packets within a specific L4 port range.
 *
 * @section Description
 *          The host sends a stream without R-tag whose Destination UDP port is 1812 and Source UDP port is 500 to switch's port1 \n
 *          to match stream identification, or send a stream without R-tag whose Destination TCP port is 995 and Source TCP port \n
 *          is 443 to switch's port1 to match stream identification, The switch will add R-tag to the stream and forward it to \n
 *          port2 and port3, \n
 *          The sequence number of the egress packets increases continuously from 0. \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_cb_ingress_passivesid_udptcp_port_range(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_cb_ingress_passivesid_udptcp_port_range(uint32 unit)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XD_VA};
    /* Config information */
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
    AclL4PortRangeInfo_t aclL4PortRangeInfo0 = {
        .entryIdx = 0u,
        .type     = DVA_L4PORT_RANGE_TCP_OR_UDP_SOURCE_PORT,
        .upper    = 500u,
        .lower    = 400u,
    };
    AclL4PortRangeInfo_t aclL4PortRangeInfo1 = {
        .entryIdx = 1u,
        .type     = DVA_L4PORT_RANGE_TCP_OR_UDP_DEST_PORT,
        .upper    = 2000u,
        .lower    = 900u,
    };
    CbIngressPassiveSidInfo_t cbIngressPassiveSidInfo = {
        .index         = 3u,
        .sidentryParam = {
            .carebits        = DVA_TSN_SELECTED_FIELD_L4PORTRANGE,
            .l4PortChkTblMap = 0x3,
            .portMask        = DVA_RTK_USER_PMAP_1ST,
            .isValid         = ENABLED,
            .streamHandle    = 7u,
        },
    };
    CbMiscInfo_t CbMiscInfo = {
        .index       = cbIngressPassiveSidInfo.index,
        .lutTabParam = {
            .indRcvEn    = DISABLED,
            .seqRcvEn    = DISABLED,
            .splitEn     = ENABLED,
            .splitDPM    = DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD,
            .genSeqNumEn = ENABLED,
            .activeEn    = DISABLED,
            .encEn       = ENABLED,
        },
    };
    /* Step 1. Set ACL L4 port range entry */
    CHK_FUN_RET(retVal, rtk_acl_l4_port_range_set(unitChip, &aclL4PortRangeInfo0));
    CHK_FUN_RET(retVal, rtk_acl_l4_port_range_set(unitChip, &aclL4PortRangeInfo1));
    /* Step 2. Enable CB */
    CHK_FUN_RET(retVal, rtk_cb_enabled_set(unitChip, &cbEnabledInfo));
    /* Step 3. Enable passive stream identification */
    CHK_FUN_RET(retVal, rtk_cb_sid_enabled_set(unitChip, &cbSidEnabledInfo));
    /* Step 4. Enable R-tag parser */
    CHK_FUN_RET(retVal, rtk_cb_parse_rtag_enabled_set(unitChip, &cbParseRtagEnabledInfo));

    /* Step 5. Set passive stream identification entry */
    CHK_FUN_RET(retVal, rtk_cb_ingress_passive_sid_set(unitChip, &cbIngressPassiveSidInfo));
    /* Step 6. Enable splitting function, and set DPM
     *         Enable sequence number generation
     *         Add R-tag at egress port
     */
    CHK_FUN_RET(retVal, rtk_cb_misc_set(unitChip, &CbMiscInfo));

    /* Expected Behavior: The switch will forward the same streams to port2 and port3
     *
     */
    return retVal;
}
/**@}*/ /* RTL907XDVA_CB_INGRESS_PASSSIVE_SID_UDPTCP_PORT_RANGE_EXAMPLE */
/**@}*/ /* RTL907XDVA_CB_EXAMPLE */
/**@}*/ /* CB_EXAMPLE */
