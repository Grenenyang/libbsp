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
#include "rtkas_api_l2.h"
#include "rtkas_api_acl_ext.h"
#include "rtkas_api_routing.h"

/** @addtogroup EXAMPLE
 *  @{
 */

/** @addtogroup RT_EXAMPLE
 *  @{
 */

/** @addtogroup CVB_RT_EXAMPLE
 *  @{
 */

/** @addtogroup CVB_RT_EXAMPLE_FIR
 *  @{
 */

/** @defgroup CVB_RT_EXAMPLE_STATIC_FIR_RNG_CHK Static Routing Firewall Binding Range Check
 * @brief This example demonstrates how to filter a stream with SIP - 192.168.2.2 / DIP - 192.168.1.x / DP - any / SP - any by range check.
 *
 * @image html routing/static_routing_fir_rng_chk.jpg
 *
 * @section Purpose
 * To demonstrate how to use APIs to configure Static Routing Firewall Binding Range Check.
 * @section Description
 * When using routing as a firewall, the routing feature provides a method to constrain lookup keys within a specific range rather than specifying exact values. This can effectively reduce the number of routing entries required in environments with specific continuous IP addresses or TCP/UDP ports. \n
 * This example demonstrates how to filter a stream with SIP - 192.168.2.2 / DIP - 192.168.1.x / DP - any / SP - any by range check. \n
 * It will be configured by following these steps:
 * 1. Disable & initialize the routing function
 * 2. Set VLANs
 * 3. Set static MACs for routing
 * 4. Set ACL range
 * 5. Set interfaces
 * 6. Set static unicast rules
 * 7. Enable routing
 *
 * @note This feature is only supported on the Host Routing table.
 *  @{
 */

#define VLAN10 (10u)
#define VLAN20 (20u)

/* Declaration */
static RtkApiRet cvb_rng_chk_rule_set(UnitChip_t unitChip);
RtkApiRet        cvb_routing_rng_chk_static_routing_fir(void);
static RtkApiRet cvb_routing_fir_rng_chk_table_dump(void);

/* Function */
RtkApiRet cvb_routing_rng_chk_static_routing_fir(void)
{
    RtkApiRet           ret                      = RT_ERR_OK;
    RtkApiRet           lukRet                   = RT_ERR_OK;
    UnitChip_t          unitChip                 = {CVB_UNIT, CHIP_907XC_VB};
    VlanInfo_t          vlanInfo                 = {0};
    const uint8         dstNodeMac[MAC_ADDR_LEN] = {0x00u, 0x00u, 0x00u, 0x00u, 0x02u, 0x02u};
    LutUniAddInfo_t     lutUniAddInfo            = {0};
    LutSearchInfo_t     lutSearchInfo            = {0};
    RtEnable_t          enable                   = {0};
    RtRngChkSet_t       rngChkSet                = {0};
    RtIntfCfg_t         intfCfg                  = {0};
    RtIntfCfg_t         intfCfgGet               = {0};
    RtTableSearchInfo_t searchInfo               = {0};
    RtUcEntry_t         ucEntry                  = {0};
    RtUcEntry_t         ucEntryGet               = {0};

    do
    {
        /* Disable Routing */
        enable.enable = DISABLED;
        CHK_FUN_RET(ret, rtk_route_enable(unitChip, &enable));

        /* Clear routing tables */
        CHK_FUN_RET(ret, rtk_route_tbl_flush(unitChip));

        /* Enable routing range check */
        rngChkSet.enable = ENABLED;
        CHK_FUN_CONTINUE(ret, rtk_route_rng_chk_set(unitChip, &rngChkSet));

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

        /* Add unicast L2 entries */
        CHK_FUN_RET(ret, drv_memcpy(lutUniAddInfo.mac, dstNodeMac, MAC_ADDR_LEN));
        lutUniAddInfo.vid   = VLAN20;
        lutUniAddInfo.port  = CVB_RTK_USER_PORT_2ND;
        lutUniAddInfo.fid   = 0u;
        lutUniAddInfo.ulfid = ENABLED;
        CHK_FUN_RET(ret, rtk_lut_uni_entry_add(unitChip, &lutUniAddInfo));
        /* Get corresponding L2 index */
        CHK_FUN_RET(ret, drv_memcpy(lutSearchInfo.macAddr, dstNodeMac, MAC_ADDR_LEN));
        CHK_FUN_RET(ret, rtk_lut_entry_search(unitChip, &lutSearchInfo));
        /* Expected Behaviour: The MAC address will be stored in L2 table and we can get its index. */

        /* Set ACL range check */
        CHK_FUN_CONTINUE(ret, cvb_rng_chk_rule_set(unitChip));

        /* Set interfaces 0 */
        intfCfg.intfEntry.index      = 0u;
        intfCfg.intfEntry.valid      = ENABLED;
        intfCfg.intfEntry.firewall   = ENABLED;
        intfCfg.luKeyMode.dipNoCare  = ENABLED;
        intfCfg.luKeyMode.spNoCare   = ENABLED;
        intfCfg.luKeyMode.dpNoCare   = ENABLED;
        intfCfg.luKeyMode.vidNoCare  = ENABLED;
        intfCfg.intfEntry.spFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.dpFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.intfFidVid = VLAN10;
        intfCfg.intfEntry.portId     = CVB_RTK_USER_PORT_1ST;
        intfCfg.intfEntry.ipVer      = IP4_VER;
        intfCfg.intfEntry.gwIp[0]    = 192u;
        intfCfg.intfEntry.gwIp[1]    = 168u;
        intfCfg.intfEntry.gwIp[2]    = 2u;
        intfCfg.intfEntry.gwIp[3]    = 254u;
        intfCfg.intfEntry.mac[1]     = 0xe0u;
        intfCfg.intfEntry.mac[2]     = 0x4cu;
        intfCfg.intfEntry.mac[5]     = 0x01u;
        CHK_FUN_RET(ret, rtk_route_intf_set(unitChip, &intfCfg));

        intfCfgGet.intfEntry.index = intfCfg.intfEntry.index;
        CHK_FUN_RET(ret, rtk_route_intf_get(unitChip, &intfCfgGet));

        if(0 == rtl_memcmp(&intfCfgGet, &intfCfg, sizeof(intfCfg)))
        {
            (void)rtlglue_printf("Set interface %d successfully\n", intfCfgGet.intfEntry.index);
        }
        /*
        Expected Behaviour: following interface will be set
        Interface 0
        IP - 192.168.2.254
        MAC - 00:E0:4C:00:00:01
        Port - 1st
        VID - 10
        */

        /* Set interface 1 */
        intfCfg.intfEntry.index      = 1u;
        intfCfg.intfEntry.valid      = ENABLED;
        intfCfg.intfEntry.firewall   = ENABLED;
        intfCfg.luKeyMode.dipNoCare  = ENABLED;
        intfCfg.luKeyMode.spNoCare   = ENABLED;
        intfCfg.luKeyMode.dpNoCare   = ENABLED;
        intfCfg.luKeyMode.vidNoCare  = ENABLED;
        intfCfg.intfEntry.spFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.dpFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.intfFidVid = VLAN20;
        intfCfg.intfEntry.portId     = CVB_RTK_USER_PORT_2ND;
        intfCfg.intfEntry.ipVer      = IP4_VER;
        intfCfg.intfEntry.gwIp[0]    = 192u;
        intfCfg.intfEntry.gwIp[1]    = 168u;
        intfCfg.intfEntry.gwIp[2]    = 1u;
        intfCfg.intfEntry.gwIp[3]    = 254u;
        intfCfg.intfEntry.mac[1]     = 0xe0u;
        intfCfg.intfEntry.mac[2]     = 0x4cu;
        intfCfg.intfEntry.mac[5]     = 0x01u;
        CHK_FUN_RET(ret, rtk_route_intf_set(unitChip, &intfCfg));

        intfCfgGet.intfEntry.index = intfCfg.intfEntry.index;
        CHK_FUN_RET(ret, rtk_route_intf_get(unitChip, &intfCfgGet));

        if(0 == rtl_memcmp(&intfCfgGet, &intfCfg, sizeof(intfCfg)))
        {
            (void)rtlglue_printf("Set interface %d successfully\n", intfCfgGet.intfEntry.index);
        }
        /*
        Expected Behaviour: following interface will be set
        Interface 1
        IP - 192.168.1.254
        MAC - 00:E0:4C:00:00:01
        Port - 2nd
        VID - 20
        */

        /* Lookup IPv4 unicast */
        CHK_FUN_RET(ret, drv_memset(&searchInfo, 0, sizeof(searchInfo)));
        searchInfo.luKey.sip[0]    = 192u;
        searchInfo.luKey.sip[1]    = 168u;
        searchInfo.luKey.sip[2]    = 2u;
        searchInfo.luKey.sip[3]    = 2u;
        searchInfo.luKey.entryType = EM_RT_UC4FIR;
        searchInfo.luKey.dpFmt     = RT_L4FMT_NOCARE;
        searchInfo.luKey.spFmt     = RT_L4FMT_NOCARE;
        lukRet                     = rtk_route_tbl_lookup(unitChip, &searchInfo);

        if((-RT_ERR_ROUTE_ENTRY_NOT_FOUND == lukRet) && (RT_TBL_INVALID_IDX != searchInfo.availIdx))
        {
            /* If lookup miss, check if an available entry exist */
            if(EM_HOST_TBL == searchInfo.availTblType)
            {
                /* The available entry is in host table, then set the unicast entry into the available entry */
                CHK_FUN_RET(ret, drv_memset(&ucEntry, 0, sizeof(ucEntry)));
                CHK_FUN_RET(ret, drv_memcpy(&ucEntry.hostLuKey, &searchInfo.luKey, sizeof(RtlookUpKey_t)));
                ucEntry.tblInfo.tblType  = searchInfo.availTblType;
                ucEntry.tblInfo.index    = searchInfo.availIdx;
                ucEntry.tblInfo.entryIdx = searchInfo.availEntryIdx;
                ucEntry.hostLuKey.dipGrp = 0x8; /* ACL Group 3 */
                ucEntry.valid            = ENABLED;
                ucEntry.ttl              = ENABLED;
                ucEntry.egrIfIdx         = 1u;
                ucEntry.l2Idx            = lutSearchInfo.lutEntry.index;
                CHK_FUN_RET(ret, rtk_route_uc_set(unitChip, &ucEntry));

                /* Get the unicast entry */
                ucEntryGet.tblInfo.tblType     = ucEntry.tblInfo.tblType;
                ucEntryGet.tblInfo.index       = ucEntry.tblInfo.index;
                ucEntryGet.tblInfo.entryIdx    = ucEntry.tblInfo.entryIdx;
                ucEntryGet.hostLuKey.entryType = ucEntry.hostLuKey.entryType;
                CHK_FUN_RET(ret, rtk_route_uc_get(unitChip, &ucEntryGet));

                if(0 == rtl_memcmp(&ucEntryGet, &ucEntry, sizeof(ucEntry)))
                {
                    (void)rtlglue_printf("Set IPv4 unicast entry successfully\n");
                }
            }
            else if(EM_NET_TBL == searchInfo.availTblType)
            {
                (void)rtlglue_printf("Net table doesn't support range check, no available space to store the entry!!\n");
            }
            else
            {
                (void)rtlglue_printf("No available space to store the entry!!\n");
            }
        }
        /*
        Expected Behaviour: following static host will be set
        SIP - 192.168.2.2
        DIP - 192.168.1.x
        SPort - any
        DPort - any
        L2 index - get from L2 table
        egress intetface ID - 1
        */

        /* Enable Routing */
        enable.enable = ENABLED;
        CHK_FUN_RET(ret, rtk_route_enable(unitChip, &enable));

        /* Diagnostic */
        CHK_FUN_RET(ret, cvb_routing_fir_rng_chk_table_dump());

        /*
        Expected Behaviour:
        The packet from interface 0 with SIP-192.168.2.2 DIP-192.168.1.x DPort-any SPort-any will be routed to interface 1
        VID, SMAC, SIP will be replaced according to egress interface settings, and TTL will be checked and decreased.
        Other L3 packets will be blocked
        */
    } while(1 == 0);

    return ret;
}

/**
 * @brief <b>Set ACL range check</b> \n
 *      Set ACL range check, please refer to \ref RANGE_CHECK and \ref acl_white_list to see the function detail. \n
 *
 */
static RtkApiRet cvb_rng_chk_rule_set(UnitChip_t unitChip)
{
    RtkApiRet        retVal        = RT_ERR_OK;
    const ipaddr_t   upper         = 0xC0A801FFu; /* 192.168.1.255 */
    const ipaddr_t   lower         = 0xC0A80101u; /* 192.168.1.1 */
    AclIpRangeInfo_t aclIpRangeSet = {0};
    AclIpRangeInfo_t aclIpRangeGet = {0};

    /* Configure IPv4 range check index to 3, upper/lower bound set into hardward */
    aclIpRangeSet.entryIdx = 3u;
    aclIpRangeSet.upper    = upper;
    aclIpRangeSet.lower    = lower;
    aclIpRangeSet.type     = CVB_IP_RANGE_DEST_IP;
    CHK_FUN_RET(retVal, rtk_acl_ip_range_set(unitChip, &aclIpRangeSet));

    aclIpRangeGet.entryIdx = 3u;
    CHK_FUN_RET(retVal, rtk_acl_ip_range_get(unitChip, &aclIpRangeGet));

    if(RT_ERR_OK == retVal)
    {
        (void)rtlglue_printf("[Get] pType=%d Upper=0x%x Lower=0x%x\n", aclIpRangeGet.type, aclIpRangeGet.upper, aclIpRangeGet.lower);
    }

    /*
    Expected Behaviour: following interface will be set
    ACL range check index: 3
    IPv4 range: 192.168.1.1 ~ 192.168.1.255
    */
    return retVal;
}

static RtkApiRet cvb_routing_fir_rng_chk_table_dump(void)
{
    RtkApiRet        ret           = RT_ERR_OK;
    UnitChip_t       unitChip      = {CVB_UNIT, CHIP_907XC_VB};
    RtIntfCfg_t      intfCfg       = {0};
    RtUcEntry_t      ucEntry       = {0};
    RtMcEntry_t      mcEntry       = {0};
    RtHostTypeList_t entryTypeList = {0};
    uint16           i             = 0;
    uint8            j             = 0;

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

    /* Dump host table */
    (void)rtlglue_printf("========== %s ==========\n", "Host table");
    for(i = 0u; i < RT_HOST_TBL_MAX_SIZE; i++)
    {
        /* Get entry type info */
        CHK_FUN_CONTINUE(ret, drv_memset(&entryTypeList, 0, sizeof(entryTypeList)));
        entryTypeList.index = i;
        ret                 = rtk_route_host_entry_typelist_get(unitChip, &entryTypeList);

        /* Get entry index info */
        for(j = 0u; j < RT_HOST_ENTRY_MAX_SIZE; j++)
        {
            if(ENABLED == entryTypeList.typeList[j].valid)
            {
                if(0u == ((uint8)entryTypeList.typeList[j].entryType & 1u)) /* Unicast : EM_RT_UC4(0), EM_RT_UC6(2), EM_RT_UC4FIR(4), EM_RT_UC6FIR(6) */
                {
                    CHK_FUN_CONTINUE(ret, drv_memset(&ucEntry, 0, sizeof(ucEntry)));
                    ucEntry.tblInfo.tblType     = EM_HOST_TBL;
                    ucEntry.tblInfo.index       = i;
                    ucEntry.tblInfo.entryIdx    = entryTypeList.typeList[j].entryIdx;
                    ucEntry.hostLuKey.entryType = entryTypeList.typeList[j].entryType;
                    CHK_FUN_CONTINUE(ret, rtk_route_uc_get(unitChip, &ucEntry));

                    (void)rtlglue_printf("[%4d] Valid: %d TTL: %d Type: %d", i, ucEntry.valid, ucEntry.ttl, ucEntry.hostLuKey.entryType);

                    if(0u == (IP6_TYPE_BIT & (uint8)ucEntry.hostLuKey.entryType))
                    {
                        (void)rtlglue_printf(" DIP: %3d.%3d.%3d.%3d SIP: %3d.%3d.%3d.%3d",
                                             ucEntry.hostLuKey.dipAddr[0], ucEntry.hostLuKey.dipAddr[1], ucEntry.hostLuKey.dipAddr[2], ucEntry.hostLuKey.dipAddr[3],
                                             ucEntry.hostLuKey.sipAddr[0], ucEntry.hostLuKey.sipAddr[1], ucEntry.hostLuKey.sipAddr[2], ucEntry.hostLuKey.sipAddr[3]);
                    }
                    else
                    {
                        (void)rtlglue_printf(" DIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x SIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                                             ucEntry.hostLuKey.dipAddr[0], ucEntry.hostLuKey.dipAddr[1], ucEntry.hostLuKey.dipAddr[2], ucEntry.hostLuKey.dipAddr[3],
                                             ucEntry.hostLuKey.dipAddr[4], ucEntry.hostLuKey.dipAddr[5], ucEntry.hostLuKey.dipAddr[6], ucEntry.hostLuKey.dipAddr[7],
                                             ucEntry.hostLuKey.dipAddr[8], ucEntry.hostLuKey.dipAddr[9], ucEntry.hostLuKey.dipAddr[10], ucEntry.hostLuKey.dipAddr[11],
                                             ucEntry.hostLuKey.dipAddr[12], ucEntry.hostLuKey.dipAddr[13], ucEntry.hostLuKey.dipAddr[14], ucEntry.hostLuKey.dipAddr[15],
                                             ucEntry.hostLuKey.sipAddr[0], ucEntry.hostLuKey.sipAddr[1], ucEntry.hostLuKey.sipAddr[2], ucEntry.hostLuKey.sipAddr[3],
                                             ucEntry.hostLuKey.sipAddr[4], ucEntry.hostLuKey.sipAddr[5], ucEntry.hostLuKey.sipAddr[6], ucEntry.hostLuKey.sipAddr[7],
                                             ucEntry.hostLuKey.sipAddr[8], ucEntry.hostLuKey.sipAddr[9], ucEntry.hostLuKey.sipAddr[10], ucEntry.hostLuKey.sipAddr[11],
                                             ucEntry.hostLuKey.sipAddr[12], ucEntry.hostLuKey.sipAddr[13], ucEntry.hostLuKey.sipAddr[14], ucEntry.hostLuKey.sipAddr[15]);
                    }
                    (void)rtlglue_printf(" DP: %5d SP: %5d VID: %d NhAlloc:%d NhIdx:%4d L2Idx: %4d EGRIF: %d\n",
                                         ucEntry.hostLuKey.dp, ucEntry.hostLuKey.sp, ucEntry.hostLuKey.vid,
                                         ucEntry.nhAlloc, ucEntry.nhIdx, ucEntry.l2Idx, ucEntry.egrIfIdx);
                }
                else /* Multicast */
                {
                    CHK_FUN_CONTINUE(ret, drv_memset(&mcEntry, 0, sizeof(mcEntry)));
                    mcEntry.tblInfo.tblType  = EM_HOST_TBL;
                    mcEntry.tblInfo.index    = i;
                    mcEntry.tblInfo.entryIdx = entryTypeList.typeList[j].entryIdx;
                    mcEntry.luKey.entryType  = entryTypeList.typeList[j].entryType;
                    CHK_FUN_CONTINUE(ret, rtk_route_mc_get(unitChip, &mcEntry));

                    (void)rtlglue_printf("[%4d] Valid: %d TTL: %d Type: %d",
                                         i, mcEntry.valid, mcEntry.ttl, mcEntry.luKey.entryType);

                    if(0u == (IP6_TYPE_BIT & (uint8)mcEntry.luKey.entryType))
                    {
                        (void)rtlglue_printf(" DIP: %3d.%3d.%3d.%3d SIP: %3d.%3d.%3d.%3d",
                                             mcEntry.luKey.dipAddr[0], mcEntry.luKey.dipAddr[1], mcEntry.luKey.dipAddr[2], mcEntry.luKey.dipAddr[3], mcEntry.luKey.sipAddr[0], mcEntry.luKey.sipAddr[1], mcEntry.luKey.sipAddr[2], mcEntry.luKey.sipAddr[3]);
                    }
                    else
                    {
                        (void)rtlglue_printf(" DIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x SIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                                             mcEntry.luKey.dipAddr[0], mcEntry.luKey.dipAddr[1], mcEntry.luKey.dipAddr[2], mcEntry.luKey.dipAddr[3],
                                             mcEntry.luKey.dipAddr[4], mcEntry.luKey.dipAddr[5], mcEntry.luKey.dipAddr[6], mcEntry.luKey.dipAddr[7],
                                             mcEntry.luKey.dipAddr[8], mcEntry.luKey.dipAddr[9], mcEntry.luKey.dipAddr[10], mcEntry.luKey.dipAddr[11],
                                             mcEntry.luKey.dipAddr[12], mcEntry.luKey.dipAddr[13], mcEntry.luKey.dipAddr[14], mcEntry.luKey.dipAddr[15],
                                             mcEntry.luKey.sipAddr[0], mcEntry.luKey.sipAddr[1], mcEntry.luKey.sipAddr[2], mcEntry.luKey.sipAddr[3],
                                             mcEntry.luKey.sipAddr[4], mcEntry.luKey.sipAddr[5], mcEntry.luKey.sipAddr[6], mcEntry.luKey.sipAddr[7],
                                             mcEntry.luKey.sipAddr[8], mcEntry.luKey.sipAddr[9], mcEntry.luKey.sipAddr[10], mcEntry.luKey.sipAddr[11],
                                             mcEntry.luKey.sipAddr[12], mcEntry.luKey.sipAddr[13], mcEntry.luKey.sipAddr[14], mcEntry.luKey.sipAddr[15]);
                    }
                    (void)rtlglue_printf(" DP: %5d SP: %5d VID: %d RHALLOC: %d RHIDX: %4d DPM: 0x%0x EGRIFLIST: 0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
                                         mcEntry.luKey.dp, mcEntry.luKey.sp, mcEntry.luKey.vid, mcEntry.rhAlloc, mcEntry.rhTblIdx, mcEntry.dpm,
                                         mcEntry.nextHopIf[7], mcEntry.nextHopIf[6], mcEntry.nextHopIf[5], mcEntry.nextHopIf[4],
                                         mcEntry.nextHopIf[3], mcEntry.nextHopIf[2], mcEntry.nextHopIf[1], mcEntry.nextHopIf[0]);
                }
            }

            if(RT_ERR_OK != ret)
            {
                break;
            }
        }

        if(RT_ERR_OK != ret)
        {
            break;
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

    /* Dump multicast collision table */
    (void)rtlglue_printf("========== %s ==========\n", "Multicast collision table");
    for(i = 0u; i < RT_MCCOL_TBL_MAX_SIZE; i++)
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&mcEntry, 0, sizeof(mcEntry)));
        mcEntry.tblInfo.tblType = EM_MCCOL_TBL;
        mcEntry.tblInfo.index   = i;
        CHK_FUN_CONTINUE(ret, rtk_route_mc_get(unitChip, &mcEntry));

        if(ENABLED == mcEntry.valid)
        {
            (void)rtlglue_printf("[%2d] Valid: %d TTL: %d", i, mcEntry.valid, mcEntry.ttl);

            (void)rtlglue_printf(" Type: %d", mcEntry.luKey.entryType);

            if(0u == (IP6_TYPE_BIT & (uint8)mcEntry.luKey.entryType))
            {
                (void)rtlglue_printf(" DIP: %3d.%3d.%3d.%3d SIP: %3d.%3d.%3d.%3d",
                                     mcEntry.luKey.dipAddr[0], mcEntry.luKey.dipAddr[1], mcEntry.luKey.dipAddr[2], mcEntry.luKey.dipAddr[3],
                                     mcEntry.luKey.sipAddr[0], mcEntry.luKey.sipAddr[1], mcEntry.luKey.sipAddr[2], mcEntry.luKey.sipAddr[3]);
            }
            else
            {
                (void)rtlglue_printf(" DIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x SIP: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                                     mcEntry.luKey.dipAddr[0], mcEntry.luKey.dipAddr[1], mcEntry.luKey.dipAddr[2], mcEntry.luKey.dipAddr[3],
                                     mcEntry.luKey.dipAddr[4], mcEntry.luKey.dipAddr[5], mcEntry.luKey.dipAddr[6], mcEntry.luKey.dipAddr[7],
                                     mcEntry.luKey.dipAddr[8], mcEntry.luKey.dipAddr[9], mcEntry.luKey.dipAddr[10], mcEntry.luKey.dipAddr[11],
                                     mcEntry.luKey.dipAddr[12], mcEntry.luKey.dipAddr[13], mcEntry.luKey.dipAddr[14], mcEntry.luKey.dipAddr[15],
                                     mcEntry.luKey.sipAddr[0], mcEntry.luKey.sipAddr[1], mcEntry.luKey.sipAddr[2], mcEntry.luKey.sipAddr[3],
                                     mcEntry.luKey.sipAddr[4], mcEntry.luKey.sipAddr[5], mcEntry.luKey.sipAddr[6], mcEntry.luKey.sipAddr[7],
                                     mcEntry.luKey.sipAddr[8], mcEntry.luKey.sipAddr[9], mcEntry.luKey.sipAddr[10], mcEntry.luKey.sipAddr[11],
                                     mcEntry.luKey.sipAddr[12], mcEntry.luKey.sipAddr[13], mcEntry.luKey.sipAddr[14], mcEntry.luKey.sipAddr[15]);
            }
            (void)rtlglue_printf(" DP: %5d SP: %5d VID: %d RHALLOC: %d RHIDX: %4d DPM: 0x%0x EGRIFLIST: 0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
                                 mcEntry.luKey.dp, mcEntry.luKey.sp, mcEntry.luKey.vid, mcEntry.rhAlloc, mcEntry.rhTblIdx, mcEntry.dpm,
                                 mcEntry.nextHopIf[7], mcEntry.nextHopIf[6], mcEntry.nextHopIf[5], mcEntry.nextHopIf[4],
                                 mcEntry.nextHopIf[3], mcEntry.nextHopIf[2], mcEntry.nextHopIf[1], mcEntry.nextHopIf[0]);
        }
    }

    /*
    Expected Behaviour:
        Dump all table info
    */
    return ret;
}
/**@}*/ /* CVB_RT_EXAMPLE_STATIC_FIR_RNG_CHK */
/**@}*/ /* CVB_RT_EXAMPLE_FIR */
/**@}*/ /* CVB_RT_EXAMPLE */
/**@}*/ /* RT_EXAMPLE */
/**@}*/ /* EXAMPLE */