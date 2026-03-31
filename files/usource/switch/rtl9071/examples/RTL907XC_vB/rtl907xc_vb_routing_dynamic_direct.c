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
#include "rtkas_api_vlan.h"
#include "rtkas_api_routing.h"

/** @defgroup EXAMPLE Example
 * @brief Example Code
 *  @{
 */

/** @defgroup RT_EXAMPLE Routing Example
 *  @{
 */

/** @defgroup CVB_RT_EXAMPLE RTL907XC_VB Routing Example
 *  @{
 */

/** @defgroup CVB_RT_EXAMPLE_RT Examples for routing application
 *  @{
 */

/**
 * @defgroup CVB_RT_EXAMPLE_D_CONNECT Network Routing - Unicast Direct Connection
 * @brief This example demonstrates the steps required to configure network routing between different subnets.
 *
 * @image html routing/network_routing_direct.jpg
 *
 * @section Purpose
 * To demonstrate how to use APIs to configure Network Routing for Unicast Direct Connection.
 * @section Description
 * Host A and Host B/C are in different subnets. If they want to ping each other, they must establish a routing entry and transform the L2 header across the different LANs.
 *      In order to communicate between different hosts on different LANs, several routing rules and router interfaces need to be configured.
 *      In the above case, two interfaces need to be configured, and each interface shall have a corresponding routing rule. \n
 * This example will be configured by following these steps:
 * 1. Disable & initialize the routing function
 * 2. Set VLANs
 * 3. Set interfaces
 * 4. Set network routes
 * 5. Enable routing
 *
 * @{
 */

#define VLAN10 (10u)
#define VLAN20 (20u)

/* Declaration */
RtkApiRet        cvb_routing_dynamic_direct(void);
static RtkApiRet cvb_dynamic_direct_table_dump(void);

/* Function */
RtkApiRet cvb_routing_dynamic_direct(void)
{
    RtkApiRet   ret        = RT_ERR_OK;
    UnitChip_t  unitChip   = {CVB_UNIT, CHIP_907XC_VB};
    VlanInfo_t  vlanInfo   = {0};
    RtIntfCfg_t intfCfg    = {0};
    RtIntfCfg_t intfCfgGet = {0};
    RtTbl_t     rtEntry    = {0};
    RtTbl_t     rtEntryGet = {0};
    RtEnable_t  enable     = {0};

    /* Disable Routing */
    enable.enable = DISABLED;
    CHK_FUN_RET(ret, rtk_route_enable(unitChip, &enable));

    /* Clear routing tables */
    CHK_FUN_RET(ret, rtk_route_tbl_flush(unitChip));

    /* Initialize VLAN */
    CHK_FUN_RET(ret, rtk_vlan_init(unitChip));

    /*
    All the ports are in the default VLAN 1 after VLAN is initialized.
    Modify it as follows:
    VLAN10 member: 1st port, 3rd port
    VLAN20 member: 2nd port, 4th port
    */
    CHK_FUN_RET(ret, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = VLAN10;
    vlanInfo.vlanEntry.mbrMsk         = CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD; /* 1st port, 3rd port */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(ret, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behaviour: The frames with vid 10 will be flooded to 1st port/3rd port port. */

    CHK_FUN_RET(ret, drv_memset(&vlanInfo, 0, sizeof(vlanInfo)));
    vlanInfo.vid                      = VLAN20;
    vlanInfo.vlanEntry.mbrMsk         = CVB_RTK_USER_PMAP_2ND | CVB_RTK_USER_PMAP_4TH; /* 2nd port, 4th port */
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(ret, rtk_vlan_set(unitChip, &vlanInfo));
    /* Expected Behaviour: The frames with vid 20 will be flooded to 2nd port/4th port. */

    /* Set interface 0 */
    CHK_FUN_RET(ret, drv_memset(&intfCfg, 0, sizeof(intfCfg)));
    intfCfg.intfEntry.index      = 0u;
    intfCfg.intfEntry.valid      = ENABLED;
    intfCfg.intfEntry.intfFidVid = VLAN10;
    intfCfg.intfEntry.portId     = CVB_RTK_USER_PORT_1ST;
    intfCfg.intfEntry.ipVer      = IP4_VER;
    intfCfg.intfEntry.gwIp[0]    = 192u;
    intfCfg.intfEntry.gwIp[1]    = 168u;
    intfCfg.intfEntry.gwIp[2]    = 1u;
    intfCfg.intfEntry.gwIp[3]    = 254u;
    intfCfg.intfEntry.mac[1]     = 0xe0u;
    intfCfg.intfEntry.mac[2]     = 0x4cu;
    intfCfg.intfEntry.mac[5]     = 0x01u;
    CHK_FUN_RET(ret, rtk_route_intf_set(unitChip, &intfCfg));

    /* Get interface 0 */
    CHK_FUN_RET(ret, drv_memset(&intfCfgGet, 0, sizeof(intfCfgGet)));
    intfCfgGet.intfEntry.index = intfCfg.intfEntry.index;
    CHK_FUN_RET(ret, rtk_route_intf_get(unitChip, &intfCfgGet));

    if(0 == rtl_memcmp(&intfCfgGet, &intfCfg, sizeof(intfCfg)))
    {
        (void)rtlglue_printf("Set interface %d successfully\n", intfCfgGet.intfEntry.index);
    }
    /*
    Expected Behaviour: following interface will be set
    Interface 0
    IP - 192.168.1.254
    MAC - 00:E0:4C:00:00:01
    Port - 1st
    VID - 10
    */

    /* Set interface 1 */
    CHK_FUN_RET(ret, drv_memset(&intfCfg, 0, sizeof(intfCfg)));
    intfCfg.intfEntry.index      = 1u;
    intfCfg.intfEntry.valid      = ENABLED;
    intfCfg.intfEntry.intfFidVid = VLAN20;
    intfCfg.intfEntry.portId     = CVB_RTK_USER_PORT_2ND;
    intfCfg.intfEntry.ipVer      = IP4_VER;
    intfCfg.intfEntry.gwIp[0]    = 192u;
    intfCfg.intfEntry.gwIp[1]    = 168u;
    intfCfg.intfEntry.gwIp[2]    = 2u;
    intfCfg.intfEntry.gwIp[3]    = 254u;
    intfCfg.intfEntry.mac[1]     = 0xe0u;
    intfCfg.intfEntry.mac[2]     = 0x4cu;
    intfCfg.intfEntry.mac[5]     = 0x01u;
    CHK_FUN_RET(ret, rtk_route_intf_set(unitChip, &intfCfg));

    /* Get interface 1 */
    CHK_FUN_RET(ret, drv_memset(&intfCfgGet, 0, sizeof(intfCfgGet)));
    intfCfgGet.intfEntry.index = intfCfg.intfEntry.index;
    CHK_FUN_RET(ret, rtk_route_intf_get(unitChip, &intfCfgGet));

    if(0 == rtl_memcmp(&intfCfgGet, &intfCfg, sizeof(intfCfg)))
    {
        (void)rtlglue_printf("Set interface %d successfully\n", intfCfgGet.intfEntry.index);
    }
    /*
    Expected Behaviour: following interface will be set
    Interface 1
    IP - 192.168.2.254
    MAC - 00:E0:4C:00:00:01
    Port - 2nd
    VID - 20
    */

    /* Set interface 2 */
    CHK_FUN_RET(ret, drv_memset(&intfCfg, 0, sizeof(intfCfg)));
    intfCfg.intfEntry.index      = 2u;
    intfCfg.intfEntry.valid      = ENABLED;
    intfCfg.intfEntry.intfFidVid = VLAN10;
    intfCfg.intfEntry.portId     = CVB_RTK_USER_PORT_1ST;
    intfCfg.intfEntry.ipVer      = IP6_VER;
    intfCfg.intfEntry.gwIp[0]    = 0xFDu;
    intfCfg.intfEntry.gwIp[1]    = 0x53u;
    intfCfg.intfEntry.gwIp[2]    = 0x7Cu;
    intfCfg.intfEntry.gwIp[3]    = 0xB8u;
    intfCfg.intfEntry.gwIp[4]    = 0x03u;
    intfCfg.intfEntry.gwIp[5]    = 0x83u;
    intfCfg.intfEntry.gwIp[7]    = 0x01u;
    intfCfg.intfEntry.gwIp[14]   = 0xFFu;
    intfCfg.intfEntry.gwIp[15]   = 0xFEu;
    intfCfg.intfEntry.mac[1]     = 0xE0u;
    intfCfg.intfEntry.mac[2]     = 0x4Cu;
    intfCfg.intfEntry.mac[5]     = 0x01u;
    CHK_FUN_RET(ret, rtk_route_intf_set(unitChip, &intfCfg));

    /* Get interface 2 */
    CHK_FUN_RET(ret, drv_memset(&intfCfgGet, 0, sizeof(intfCfgGet)));
    intfCfgGet.intfEntry.index = intfCfg.intfEntry.index;
    CHK_FUN_RET(ret, rtk_route_intf_get(unitChip, &intfCfgGet));

    if(0 == rtl_memcmp(&intfCfgGet, &intfCfg, sizeof(intfCfg)))
    {
        (void)rtlglue_printf("Set interface %d successfully\n", intfCfgGet.intfEntry.index);
    }
    /*
    Expected Behaviour: following interface will be set
    Interface 2
    IP - FD53:7CB8:383:1::FFFE
    MAC - 00:E0:4C:00:00:01
    Port - 1st
    VID - 10
    */

    /* Set interface 3 */
    CHK_FUN_RET(ret, drv_memset(&intfCfg, 0, sizeof(intfCfg)));
    intfCfg.intfEntry.index      = 3u;
    intfCfg.intfEntry.valid      = ENABLED;
    intfCfg.intfEntry.intfFidVid = VLAN20;
    intfCfg.intfEntry.portId     = CVB_RTK_USER_PORT_2ND;
    intfCfg.intfEntry.ipVer      = IP6_VER;
    intfCfg.intfEntry.gwIp[0]    = 0xFDu;
    intfCfg.intfEntry.gwIp[1]    = 0x53u;
    intfCfg.intfEntry.gwIp[2]    = 0x7Cu;
    intfCfg.intfEntry.gwIp[3]    = 0xB8u;
    intfCfg.intfEntry.gwIp[4]    = 0x03u;
    intfCfg.intfEntry.gwIp[5]    = 0x83u;
    intfCfg.intfEntry.gwIp[7]    = 0x02u;
    intfCfg.intfEntry.gwIp[14]   = 0xFFu;
    intfCfg.intfEntry.gwIp[15]   = 0xFEu;
    intfCfg.intfEntry.mac[1]     = 0xE0u;
    intfCfg.intfEntry.mac[2]     = 0x4Cu;
    intfCfg.intfEntry.mac[5]     = 0x01u;
    CHK_FUN_RET(ret, rtk_route_intf_set(unitChip, &intfCfg));

    /* Get interface 3 */
    CHK_FUN_RET(ret, drv_memset(&intfCfgGet, 0, sizeof(intfCfgGet)));
    intfCfgGet.intfEntry.index = intfCfg.intfEntry.index;
    CHK_FUN_RET(ret, rtk_route_intf_get(unitChip, &intfCfgGet));

    if(0 == rtl_memcmp(&intfCfgGet, &intfCfg, sizeof(intfCfg)))
    {
        (void)rtlglue_printf("Set interface %d successfully\n", intfCfgGet.intfEntry.index);
    }
    /*
    Expected Behaviour: following interface will be set
    Interface 3
    IP - FD53:7CB8:383:2::FFFE
    MAC - 00:E0:4C:00:00:01
    Port - 2nd
    VID - 20
    */

    /* Set IPv4 network routing entry 0 */
    CHK_FUN_RET(ret, drv_memset(&rtEntry, 0, sizeof(rtEntry)));
    rtEntry.destIpAddr[0] = 192u;
    rtEntry.destIpAddr[1] = 168u;
    rtEntry.destIpAddr[2] = 1u;
    rtEntry.subnetMask    = 24u;
    rtEntry.valid         = ENABLED;
    rtEntry.ipVer         = IP4_VER;
    rtEntry.nhIntfId      = 0u;
    rtEntry.index         = 0u;
    CHK_FUN_RET(ret, rtk_route_rttbl_set(unitChip, &rtEntry));

    CHK_FUN_RET(ret, drv_memset(&rtEntryGet, 0, sizeof(rtEntryGet)));
    rtEntryGet.index = rtEntry.index;
    CHK_FUN_RET(ret, rtk_route_rttbl_get(unitChip, &rtEntryGet));

    if(0 == rtl_memcmp(&rtEntryGet, &rtEntry, sizeof(rtEntryGet)))
    {
        (void)rtlglue_printf("Set subnet %d successfully\n", rtEntryGet.index);
    }
    /*
    Expected Behaviour: following network routing entry will be set
    dynamic rouitng 0
    DIP - 192.168.1.0
    IP mask - 24
    egress interface ID - 0
    */

    /* Set IPv4 network routing entry 1 */
    CHK_FUN_RET(ret, drv_memset(&rtEntry, 0, sizeof(rtEntry)));
    rtEntry.destIpAddr[0] = 192u;
    rtEntry.destIpAddr[1] = 168u;
    rtEntry.destIpAddr[2] = 2u;
    rtEntry.subnetMask    = 24u;
    rtEntry.valid         = ENABLED;
    rtEntry.ipVer         = IP4_VER;
    rtEntry.nhIntfId      = 1u;
    rtEntry.index         = 1u;
    CHK_FUN_RET(ret, rtk_route_rttbl_set(unitChip, &rtEntry));

    CHK_FUN_RET(ret, drv_memset(&rtEntryGet, 0, sizeof(rtEntryGet)));
    rtEntryGet.index = rtEntry.index;
    CHK_FUN_RET(ret, rtk_route_rttbl_get(unitChip, &rtEntryGet));

    if(0 == rtl_memcmp(&rtEntryGet, &rtEntry, sizeof(rtEntryGet)))
    {
        (void)rtlglue_printf("Set subnet %d successfully\n", rtEntryGet.index);
    }
    /*
    Expected Behaviour: following network routing entry will be set
    dynamic rouitng 1
    DIP - 192.168.2.0
    IP mask - 24
    egress interface ID - 1
    */

    /* Set IPv6 network routing entry 2 */
    CHK_FUN_RET(ret, drv_memset(&rtEntry, 0, sizeof(rtEntry)));
    rtEntry.destIpAddr[0] = 0xFDu;
    rtEntry.destIpAddr[1] = 0x53u;
    rtEntry.destIpAddr[2] = 0x7Cu;
    rtEntry.destIpAddr[3] = 0xB8u;
    rtEntry.destIpAddr[4] = 0x03u;
    rtEntry.destIpAddr[5] = 0x83u;
    rtEntry.destIpAddr[7] = 0x01u;
    rtEntry.subnetMask    = 64u;
    rtEntry.valid         = ENABLED;
    rtEntry.ipVer         = IP6_VER;
    rtEntry.nhIntfId      = 2u;
    rtEntry.index         = 2u;
    CHK_FUN_RET(ret, rtk_route_rttbl_set(unitChip, &rtEntry));

    CHK_FUN_RET(ret, drv_memset(&rtEntryGet, 0, sizeof(rtEntryGet)));
    rtEntryGet.index = rtEntry.index;
    CHK_FUN_RET(ret, rtk_route_rttbl_get(unitChip, &rtEntryGet));

    if(0 == rtl_memcmp(&rtEntryGet, &rtEntry, sizeof(rtEntryGet)))
    {
        (void)rtlglue_printf("Set subnet %d successfully\n", rtEntryGet.index);
    }
    /*
    Expected Behaviour: following network routing entry will be set
    dynamic rouitng 2
    DIP - FD53:7CB8:383:1::
    IP mask - 64
    egress interface ID - 2
    */

    /* Set IPv6 network routing entry 3 */
    rtEntry.destIpAddr[0] = 0xFDu;
    rtEntry.destIpAddr[1] = 0x53u;
    rtEntry.destIpAddr[2] = 0x7Cu;
    rtEntry.destIpAddr[3] = 0xB8u;
    rtEntry.destIpAddr[4] = 0x03u;
    rtEntry.destIpAddr[5] = 0x83u;
    rtEntry.destIpAddr[7] = 0x02u;
    rtEntry.subnetMask    = 64u;
    rtEntry.valid         = ENABLED;
    rtEntry.ipVer         = IP6_VER;
    rtEntry.nhIntfId      = 3u;
    rtEntry.index         = 3u;
    CHK_FUN_RET(ret, rtk_route_rttbl_set(unitChip, &rtEntry));

    CHK_FUN_RET(ret, drv_memset(&rtEntryGet, 0, sizeof(rtEntryGet)));
    rtEntryGet.index = rtEntry.index;
    CHK_FUN_RET(ret, rtk_route_rttbl_get(unitChip, &rtEntryGet));

    if(0 == rtl_memcmp(&rtEntryGet, &rtEntry, sizeof(rtEntryGet)))
    {
        (void)rtlglue_printf("Set subnet %d successfully\n", rtEntryGet.index);
    }
    /*
    Expected Behaviour: following network routing entry will be set
    dynamic rouitng 3
    DIP - FD53:7CB8:383:2::
    IP mask - 64
    egress interface ID - 3
    */

    /* Enable Routing */
    enable.enable = ENABLED;
    CHK_FUN_RET(ret, rtk_route_enable(unitChip, &enable));

    /* Diagnostic */
    CHK_FUN_RET(ret, cvb_dynamic_direct_table_dump());

    /*
    Expected Behaviour:
    192.168.1.x can ping to 192.168.2.x
    192.168.2.x can ping to 192.168.1.x
    FD53:7CB8:383:1:: can ping to FD53:7CB8:383:2::
    FD53:7CB8:383:2:: can ping to FD53:7CB8:383:1::
    Other routing packets will be blocked
    */
    return ret;
}

static RtkApiRet cvb_dynamic_direct_table_dump(void)
{
    RtkApiRet   ret      = RT_ERR_OK;
    UnitChip_t  unitChip = {CVB_UNIT, CHIP_907XC_VB};
    RtIntfCfg_t intfCfg  = {0};
    RtUcEntry_t ucEntry  = {0};
    RtTbl_t     rtEntry  = {0};
    uint16      i        = 0;

    /* Dump interface table */
    (void)rtlglue_printf("========== %s ==========\n", "Interface table");
    for(i = 0u; i < RT_INTF_TBL_MAX_SIZE; i++)
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&intfCfg, 0, sizeof(intfCfg)));
        intfCfg.intfEntry.index = (uint8)i;
        CHK_FUN_CONTINUE(ret, rtk_route_intf_get(unitChip, &intfCfg));

        if(ENABLED == intfCfg.intfEntry.valid)
        {
            (void)rtlglue_printf("[%2d] Valid: %d Firewall: %d Filter: %d",
                                 i, intfCfg.intfEntry.valid, intfCfg.intfEntry.firewall,
                                 intfCfg.intfEntry.filter);

            (void)rtlglue_printf(" IP ver: %d", intfCfg.intfEntry.ipVer);

            if(IP4_VER == intfCfg.intfEntry.ipVer)
            {
                (void)rtlglue_printf(" IP: %3d.%3d.%3d.%3d", intfCfg.intfEntry.gwIp[0], intfCfg.intfEntry.gwIp[1], intfCfg.intfEntry.gwIp[2], intfCfg.intfEntry.gwIp[3]);
            }
            else
            {
                (void)rtlglue_printf(" IP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                                     intfCfg.intfEntry.gwIp[0], intfCfg.intfEntry.gwIp[1], intfCfg.intfEntry.gwIp[2], intfCfg.intfEntry.gwIp[3],
                                     intfCfg.intfEntry.gwIp[4], intfCfg.intfEntry.gwIp[5], intfCfg.intfEntry.gwIp[6], intfCfg.intfEntry.gwIp[7],
                                     intfCfg.intfEntry.gwIp[8], intfCfg.intfEntry.gwIp[9], intfCfg.intfEntry.gwIp[10], intfCfg.intfEntry.gwIp[11],
                                     intfCfg.intfEntry.gwIp[12], intfCfg.intfEntry.gwIp[13], intfCfg.intfEntry.gwIp[14], intfCfg.intfEntry.gwIp[15]);
            }

            (void)rtlglue_printf(" MAC: %02x:%02x:%02x:%02x:%02x:%02x VID: %d",
                                 intfCfg.intfEntry.mac[0], intfCfg.intfEntry.mac[1], intfCfg.intfEntry.mac[2], intfCfg.intfEntry.mac[3], intfCfg.intfEntry.mac[4], intfCfg.intfEntry.mac[5], intfCfg.intfEntry.intfFidVid);

            if(ENABLED == intfCfg.intfEntry.firewall)
            {
                (void)rtlglue_printf(" dpFmt: %d spFmt: %d [Lookup Key] DIP: %d SIP: %d DPort: %d SPort: %d VID: %d\n",
                                     intfCfg.intfEntry.dpFmt, intfCfg.intfEntry.spFmt,
                                     (ENABLED == intfCfg.luKeyMode.dipNoCare) ? DISABLED : ENABLED, (ENABLED == intfCfg.luKeyMode.sipNoCare) ? DISABLED : ENABLED,
                                     (ENABLED == intfCfg.luKeyMode.dpNoCare) ? DISABLED : ENABLED, (ENABLED == intfCfg.luKeyMode.spNoCare) ? DISABLED : ENABLED,
                                     (ENABLED == intfCfg.luKeyMode.vidNoCare) ? DISABLED : ENABLED);
            }
            else
            {
                (void)rtlglue_printf("\n");
            }
        }
    }

    /* Dump net table */
    (void)rtlglue_printf("========== %s ==========\n", "Network table");
    for(i = 0u; i < RT_NET_TBL_MAX_SIZE; i++)
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&ucEntry, 0, sizeof(ucEntry)));
        ucEntry.tblInfo.tblType = EM_NET_TBL;
        ucEntry.tblInfo.index   = i;
        CHK_FUN_CONTINUE(ret, rtk_route_uc_get(unitChip, &ucEntry));

        if(ENABLED == ucEntry.valid)
        {
            (void)rtlglue_printf("[%3d] Valid: %d TTL: %d NhAlloc:%d NhIdx:%4d L2Idx: %4d EGRIF: %d",
                                 i, ucEntry.valid, ucEntry.ttl, ucEntry.nhAlloc, ucEntry.nhIdx, ucEntry.l2Idx, ucEntry.egrIfIdx);

            (void)rtlglue_printf(" Type: %d", ucEntry.netLuKey.entryType);

            if(0u == (IP6_TYPE_BIT & (uint8)ucEntry.netLuKey.entryType))
            {
                (void)rtlglue_printf(" DIP: %3d.%3d.%3d.%3d SIP: %3d.%3d.%3d.%3d",
                                     ucEntry.netLuKey.dipAddr[0], ucEntry.netLuKey.dipAddr[1], ucEntry.netLuKey.dipAddr[2], ucEntry.netLuKey.dipAddr[3],
                                     ucEntry.netLuKey.sipAddr[0], ucEntry.netLuKey.sipAddr[1], ucEntry.netLuKey.sipAddr[2], ucEntry.netLuKey.sipAddr[3]);
            }
            else
            {
                (void)rtlglue_printf(" DIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x SIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                                     ucEntry.netLuKey.dipAddr[0], ucEntry.netLuKey.dipAddr[1], ucEntry.netLuKey.dipAddr[2], ucEntry.netLuKey.dipAddr[3],
                                     ucEntry.netLuKey.dipAddr[4], ucEntry.netLuKey.dipAddr[5], ucEntry.netLuKey.dipAddr[6], ucEntry.netLuKey.dipAddr[7],
                                     ucEntry.netLuKey.dipAddr[8], ucEntry.netLuKey.dipAddr[9], ucEntry.netLuKey.dipAddr[10], ucEntry.netLuKey.dipAddr[11],
                                     ucEntry.netLuKey.dipAddr[12], ucEntry.netLuKey.dipAddr[13], ucEntry.netLuKey.dipAddr[14], ucEntry.netLuKey.dipAddr[15],
                                     ucEntry.netLuKey.sipAddr[0], ucEntry.netLuKey.sipAddr[1], ucEntry.netLuKey.sipAddr[2], ucEntry.netLuKey.sipAddr[3],
                                     ucEntry.netLuKey.sipAddr[4], ucEntry.netLuKey.sipAddr[5], ucEntry.netLuKey.sipAddr[6], ucEntry.netLuKey.sipAddr[7],
                                     ucEntry.netLuKey.sipAddr[8], ucEntry.netLuKey.sipAddr[9], ucEntry.netLuKey.sipAddr[10], ucEntry.netLuKey.sipAddr[11],
                                     ucEntry.netLuKey.sipAddr[12], ucEntry.netLuKey.sipAddr[13], ucEntry.netLuKey.sipAddr[14], ucEntry.netLuKey.sipAddr[15]);
            }
            (void)rtlglue_printf(" DP: %5d SP: %5d VID: %d\n", ucEntry.netLuKey.dp, ucEntry.netLuKey.sp, ucEntry.netLuKey.vid);
        }
    }

    /* Dump subnet */
    (void)rtlglue_printf("========== %s ==========\n", "Subnet configuration");
    for(i = 0u; i < RT_NET_TBL_MAX_SIZE; i++)
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&rtEntry, 0, sizeof(rtEntry)));
        rtEntry.index = (uint8)i;
        CHK_FUN_CONTINUE(ret, rtk_route_rttbl_get(unitChip, &rtEntry));

        if(ENABLED == rtEntry.valid)
        {
            (void)rtlglue_printf("[%3d] Valid: %d IP version: %d subnet mask: %d next hop interface: %d", i, rtEntry.valid, rtEntry.ipVer, rtEntry.subnetMask, rtEntry.nhIntfId);

            if(IP4_VER == rtEntry.ipVer)
            {
                (void)rtlglue_printf(" DIP: %3d.%3d.%3d.%3d NEXT HOP: %3d.%3d.%3d.%3d\n",
                                     rtEntry.destIpAddr[0], rtEntry.destIpAddr[1], rtEntry.destIpAddr[2], rtEntry.destIpAddr[3], rtEntry.nextHopIp[0], rtEntry.nextHopIp[1], rtEntry.nextHopIp[2], rtEntry.nextHopIp[3]);
            }
            else
            {
                (void)rtlglue_printf(" DIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x NEXT HOP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
                                     rtEntry.destIpAddr[0], rtEntry.destIpAddr[1], rtEntry.destIpAddr[2], rtEntry.destIpAddr[3],
                                     rtEntry.destIpAddr[4], rtEntry.destIpAddr[5], rtEntry.destIpAddr[6], rtEntry.destIpAddr[7],
                                     rtEntry.destIpAddr[8], rtEntry.destIpAddr[9], rtEntry.destIpAddr[10], rtEntry.destIpAddr[11],
                                     rtEntry.destIpAddr[12], rtEntry.destIpAddr[13], rtEntry.destIpAddr[14], rtEntry.destIpAddr[15],
                                     rtEntry.nextHopIp[0], rtEntry.nextHopIp[1], rtEntry.nextHopIp[2], rtEntry.nextHopIp[3],
                                     rtEntry.nextHopIp[4], rtEntry.nextHopIp[5], rtEntry.nextHopIp[6], rtEntry.nextHopIp[7],
                                     rtEntry.nextHopIp[8], rtEntry.nextHopIp[9], rtEntry.nextHopIp[10], rtEntry.nextHopIp[11],
                                     rtEntry.nextHopIp[12], rtEntry.nextHopIp[13], rtEntry.nextHopIp[14], rtEntry.nextHopIp[15]);
            }
        }
    }

    /*
    Expected Behaviour:
        Dump all table info
    */
    return ret;
}
/**@}*/ /* CVB_RT_EXAMPLE_D_CONNECT */
/**@}*/ /* CVB_RT_EXAMPLE_RT */
/**@}*/ /* CVB_RT_EXAMPLE */
/**@}*/ /* RT_EXAMPLE */
/**@}*/ /* EXAMPLE */