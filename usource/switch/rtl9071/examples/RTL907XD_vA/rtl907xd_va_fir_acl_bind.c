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

/** @addtogroup FIR_EXAMPLE
 *  @{
 */

/** @addtogroup DVA_FIR_EXAMPLE
 *  @{
 */

/** @defgroup DVA_FIR_EXAMPLE_STATIC_FIR_ACL_BIND Static Routing Firewall Binding ACL Rule
 * @brief This example demonstrates how to set a whitelist so that only packets sent from SAMC-02:7D:FA:00:10:01 / VID-10 to DIP-FD53:7CB8:383:2::2 can be routed.
 *
 * @image html fir/static_routing_fir_bind_acl.jpg
 *
 * @section Purpose
 * To demonstrate how to use APIs to configure Static Routing Firewall Binding ACL Rule.
 * @section Description
 * When the routing feature is used as a firewall application, the routing feature provides a method to extend the ACL filter key by treating routing entries as part of the ACL filter key. This allows the ACL to offload the lookup keys supported by routing to the routing table, thereby reducing the usage of ACL filter keys, increasing the effective length of ACL filter keys, and minimizing the number of ACL entries required. \n
 * This example demonstrates how to set a whitelist so that only packets sent from SAMC-02:7D:FA:00:10:01 / VID-10 to DIP-FD53:7CB8:383:2::2 can be routed. \n
 * It will be configured by following these steps:
 * 1. Disable & initialize the routing function
 * 2. Set VLANs
 * 3. Set static MACs for staic unicast
 * 4. Set ACL to bind
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
RtkApiRet        dva_routing_acl_bind_static_routing_fir(void);
static RtkApiRet dva_acl_bind_rule_set(UnitChip_t unitChip);
static RtkApiRet dva_routing_fir_acl_bind_table_dump(void);

/* Function */
RtkApiRet dva_routing_acl_bind_static_routing_fir(void)
{
    RtkApiRet           ret                      = RT_ERR_OK;
    RtkApiRet           lukRet                   = RT_ERR_OK;
    UnitChip_t          unitChip                 = {DVA_UNIT, CHIP_907XD_VA};
    VlanInfo_t          vlanInfo                 = {0};
    const uint8         aclIdx                   = 0u;
    const uint8         dstNodeMac[MAC_ADDR_LEN] = {0x00u, 0x00u, 0x00u, 0x00u, 0x02u, 0x02u};
    LutUniAddInfo_t     lutUniAddInfo            = {0};
    LutSearchInfo_t     lutSearchInfo            = {0};
    RtEnable_t          enable                   = {0};
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
        vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_3RD; /* 1st port, 3rd port */
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
        vlanInfo.vlanEntry.mbrMsk         = DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_4TH; /* 2nd port, 4th port */
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
        lutUniAddInfo.port  = DVA_RTK_USER_PORT_2ND;
        lutUniAddInfo.fid   = 0u;
        lutUniAddInfo.ulfid = ENABLED;
        lutUniAddInfo.nh    = ENABLED;
        CHK_FUN_RET(ret, rtk_lut_uni_entry_add(unitChip, &lutUniAddInfo));
        /* Get corresponding L2 index */
        CHK_FUN_RET(ret, drv_memcpy(lutSearchInfo.macAddr, dstNodeMac, MAC_ADDR_LEN));
        CHK_FUN_RET(ret, rtk_lut_entry_search(unitChip, &lutSearchInfo));
        /* Expected Behaviour: The MAC address will be stored in L2 table and we can get its index. */

        /* Step 3.1 Set ACL range check */
        CHK_FUN_CONTINUE(ret, dva_acl_bind_rule_set(unitChip));

        /* Set interfaces */
        intfCfg.intfEntry.index      = 0u;
        intfCfg.intfEntry.valid      = ENABLED;
        intfCfg.intfEntry.firewall   = ENABLED;
        intfCfg.luKeyMode.sipNoCare  = ENABLED;
        intfCfg.luKeyMode.spNoCare   = ENABLED;
        intfCfg.luKeyMode.dpNoCare   = ENABLED;
        intfCfg.luKeyMode.vidNoCare  = ENABLED;
        intfCfg.intfEntry.spFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.dpFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.intfFidVid = VLAN10;
        intfCfg.intfEntry.ipVer      = IP6_VER;
        intfCfg.intfEntry.portId     = DVA_RTK_USER_PORT_1ST;
        intfCfg.intfEntry.gwIp[0]    = 0xFDu;
        intfCfg.intfEntry.gwIp[1]    = 0x53u;
        intfCfg.intfEntry.gwIp[2]    = 0x7Cu;
        intfCfg.intfEntry.gwIp[3]    = 0xB8u;
        intfCfg.intfEntry.gwIp[4]    = 0x03u;
        intfCfg.intfEntry.gwIp[5]    = 0x83u;
        intfCfg.intfEntry.gwIp[7]    = 0x01u;
        intfCfg.intfEntry.gwIp[14]   = 0xFFu;
        intfCfg.intfEntry.gwIp[15]   = 0xFEu;
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
        IP - FD53:7CB8:383:1::FFFE
        MAC - 00:E0:4C:00:00:01
        Port - 1st
        VID - 10
        */

        intfCfg.intfEntry.index      = 1u;
        intfCfg.intfEntry.valid      = ENABLED;
        intfCfg.intfEntry.firewall   = ENABLED;
        intfCfg.luKeyMode.sipNoCare  = ENABLED;
        intfCfg.luKeyMode.spNoCare   = ENABLED;
        intfCfg.luKeyMode.dpNoCare   = ENABLED;
        intfCfg.luKeyMode.vidNoCare  = ENABLED;
        intfCfg.intfEntry.spFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.dpFmt      = RT_L4FMT_NOCARE;
        intfCfg.intfEntry.intfFidVid = VLAN20;
        intfCfg.intfEntry.portId     = DVA_RTK_USER_PORT_2ND;
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
        IP - FD53:7CB8:383:1::FFFE
        MAC - 00:E0:4C:00:00:01
        Port - 2nd
        VID - 20
        */

        /* Set unicast */
        searchInfo.luKey.dip[0]    = 0xFDu;
        searchInfo.luKey.dip[1]    = 0x53u;
        searchInfo.luKey.dip[2]    = 0x7Cu;
        searchInfo.luKey.dip[3]    = 0xB8u;
        searchInfo.luKey.dip[4]    = 0x03u;
        searchInfo.luKey.dip[5]    = 0x83u;
        searchInfo.luKey.dip[7]    = 0x02u;
        searchInfo.luKey.dip[15]   = 0x02u;
        searchInfo.luKey.entryType = EM_RT_UC6FIR;
        searchInfo.luKey.dpFmt     = RT_L4FMT_NOCARE;
        searchInfo.luKey.spFmt     = RT_L4FMT_NOCARE;
        lukRet                     = rtk_route_tbl_lookup(unitChip, &searchInfo);

        if((-RT_ERR_ROUTE_ENTRY_NOT_FOUND == lukRet) && (RT_TBL_INVALID_IDX != searchInfo.availIdx))
        {
            if(EM_HOST_TBL == searchInfo.availTblType)
            {
                CHK_FUN_RET(ret, drv_memcpy(&ucEntry.hostLuKey, &searchInfo.luKey, sizeof(RtlookUpKey_t)));
                ucEntry.tblInfo.tblType      = searchInfo.availTblType;
                ucEntry.tblInfo.index        = searchInfo.availIdx;
                ucEntry.tblInfo.entryIdx     = searchInfo.availEntryIdx;
                ucEntry.hostLuKey.aclBlkIdx0 = ACL_BLK_IDX_VALID | aclIdx; /* Bind to PIE index 0 */
                ucEntry.valid                = ENABLED;
                ucEntry.ttl                  = ENABLED;
                ucEntry.egrIfIdx             = 1u;
                ucEntry.l2Idx                = lutSearchInfo.lutEntry.index;
                CHK_FUN_RET(ret, rtk_route_uc_set(unitChip, &ucEntry));

                ucEntryGet.tblInfo.tblType     = ucEntry.tblInfo.tblType;
                ucEntryGet.tblInfo.index       = ucEntry.tblInfo.index;
                ucEntryGet.tblInfo.entryIdx    = ucEntry.tblInfo.entryIdx;
                ucEntryGet.hostLuKey.entryType = ucEntry.hostLuKey.entryType;
                CHK_FUN_RET(ret, rtk_route_uc_get(unitChip, &ucEntryGet));

                if(0 == rtl_memcmp(&ucEntryGet, &ucEntry, sizeof(ucEntry)))
                {
                    (void)rtlglue_printf("Set IPv6 unicast entry successfully\n");
                }
            }
            else
            {
                (void)rtlglue_printf("No available space to store the entry!!\n");
            }
        }
        else
        {
            (void)rtlglue_printf("No available space to store the entry!!\n");
        }
        /*
        Expected Behaviour: following static host will be set
        SIP - any
        DIP - FD53:7CB8:383:2::2
        SPort - any
        DPort - any
        Binded PIE index - 0
        L2 index - get from L2 table
        egress intetface ID - 1
        */

        /* Set unicast */
        searchInfo.luKey.dip[0]    = 0xFDu;
        searchInfo.luKey.dip[1]    = 0x53u;
        searchInfo.luKey.dip[2]    = 0x7Cu;
        searchInfo.luKey.dip[3]    = 0xB8u;
        searchInfo.luKey.dip[4]    = 0x03u;
        searchInfo.luKey.dip[5]    = 0x83u;
        searchInfo.luKey.dip[7]    = 0x02u;
        searchInfo.luKey.dip[15]   = 0x1Eu;
        searchInfo.luKey.entryType = EM_RT_UC6FIR;
        searchInfo.luKey.dpFmt     = RT_L4FMT_NOCARE;
        searchInfo.luKey.spFmt     = RT_L4FMT_NOCARE;
        lukRet                     = rtk_route_tbl_lookup(unitChip, &searchInfo);

        if((-RT_ERR_ROUTE_ENTRY_NOT_FOUND == lukRet) && (RT_TBL_INVALID_IDX != searchInfo.availIdx))
        {
            if(EM_HOST_TBL == searchInfo.availTblType)
            {
                CHK_FUN_RET(ret, drv_memcpy(&ucEntry.hostLuKey, &searchInfo.luKey, sizeof(RtlookUpKey_t)));
                ucEntry.tblInfo.tblType      = searchInfo.availTblType;
                ucEntry.tblInfo.index        = searchInfo.availIdx;
                ucEntry.tblInfo.entryIdx     = searchInfo.availEntryIdx;
                ucEntry.hostLuKey.aclBlkIdx0 = ACL_BLK_IDX_VALID | aclIdx; /* Bind to PIE index 0 */
                ucEntry.valid                = ENABLED;
                ucEntry.ttl                  = ENABLED;
                ucEntry.egrIfIdx             = 1u;
                ucEntry.l2Idx                = lutSearchInfo.lutEntry.index;
                CHK_FUN_RET(ret, rtk_route_uc_set(unitChip, &ucEntry));

                ucEntryGet.tblInfo.tblType     = ucEntry.tblInfo.tblType;
                ucEntryGet.tblInfo.index       = ucEntry.tblInfo.index;
                ucEntryGet.tblInfo.entryIdx    = ucEntry.tblInfo.entryIdx;
                ucEntryGet.hostLuKey.entryType = ucEntry.hostLuKey.entryType;
                CHK_FUN_RET(ret, rtk_route_uc_get(unitChip, &ucEntryGet));

                if(0 == rtl_memcmp(&ucEntryGet, &ucEntry, sizeof(ucEntry)))
                {
                    (void)rtlglue_printf("Set IPv6 unicast entry successfully\n");
                }
            }
            else
            {
                (void)rtlglue_printf("No available space to store the entry!!\n");
            }
        }
        else
        {
            (void)rtlglue_printf("No available space to store the entry!!\n");
        }
        /*
        Expected Behaviour: following static host will be set
        SIP - any
        DIP - FD53:7CB8:383:2::1E
        SPort - any
        DPort - any
        Binded PIE index - 0
        L2 index - get from L2 table
        egress intetface ID - 1
        */

        /* Set unicast */
        searchInfo.luKey.dip[0]    = 0xFDu;
        searchInfo.luKey.dip[1]    = 0x53u;
        searchInfo.luKey.dip[2]    = 0x7Cu;
        searchInfo.luKey.dip[3]    = 0xB8u;
        searchInfo.luKey.dip[4]    = 0x03u;
        searchInfo.luKey.dip[5]    = 0x83u;
        searchInfo.luKey.dip[7]    = 0x02u;
        searchInfo.luKey.dip[15]   = 0x1Eu;
        searchInfo.luKey.entryType = EM_RT_UC6FIR;
        searchInfo.luKey.dpFmt     = RT_L4FMT_NOCARE;
        searchInfo.luKey.spFmt     = RT_L4FMT_NOCARE;
        lukRet                     = rtk_route_tbl_lookup(unitChip, &searchInfo);

        if((-RT_ERR_ROUTE_ENTRY_NOT_FOUND == lukRet) && (RT_TBL_INVALID_IDX != searchInfo.availIdx))
        {
            /* If lookup miss, check if an available entry exist */
            if(EM_HOST_TBL == searchInfo.availTblType)
            {
                CHK_FUN_RET(ret, drv_memset(&ucEntry, 0, sizeof(ucEntry)));
                CHK_FUN_RET(ret, drv_memcpy(&ucEntry.hostLuKey, &searchInfo.luKey, sizeof(RtlookUpKey_t)));
                ucEntry.tblInfo.tblType  = searchInfo.availTblType;
                ucEntry.tblInfo.index    = searchInfo.availIdx;
                ucEntry.tblInfo.entryIdx = searchInfo.availEntryIdx;
                ucEntry.valid            = ENABLED;
                ucEntry.ttl              = ENABLED;
                ucEntry.egrIfIdx         = 1u;
                ucEntry.l2Idx            = lutSearchInfo.lutEntry.index;
                CHK_FUN_RET(ret, rtk_route_uc_set(unitChip, &ucEntry));

                ucEntryGet.tblInfo.tblType     = ucEntry.tblInfo.tblType;
                ucEntryGet.tblInfo.index       = ucEntry.tblInfo.index;
                ucEntryGet.tblInfo.entryIdx    = ucEntry.tblInfo.entryIdx;
                ucEntryGet.hostLuKey.entryType = ucEntry.hostLuKey.entryType;
                CHK_FUN_RET(ret, rtk_route_uc_get(unitChip, &ucEntryGet));

                if(0 == rtl_memcmp(&ucEntryGet, &ucEntry, sizeof(ucEntry)))
                {
                    (void)rtlglue_printf("Set IPv6 unicast entry successfully\n");
                }
            }
            else if(EM_NET_TBL == searchInfo.availTblType)
            {
                /* The available entry is in host table, then set the unicast entry into the available entry */
                CHK_FUN_RET(ret, drv_memset(&ucEntry, 0, sizeof(ucEntry)));
                CHK_FUN_RET(ret, drv_memcpy(&ucEntry.netLuKey, &searchInfo.luKey, sizeof(RtlookUpKey_t)));
                CHK_FUN_RET(ret, drv_memset(ucEntry.netLuKey.dipCare, 0xff, IPV6_ADDR_LEN));
                CHK_FUN_RET(ret, drv_memset(ucEntry.netLuKey.sipCare, 0xff, IPV6_ADDR_LEN));
                ucEntry.netLuKey.typeCare  = ENABLED;
                ucEntry.tblInfo.tblType    = searchInfo.availTblType;
                ucEntry.tblInfo.index      = searchInfo.availIdx;
                ucEntry.valid              = ENABLED;
                ucEntry.netLuKey.dpFmtCare = ENABLED;
                ucEntry.netLuKey.spFmtCare = ENABLED;
                ucEntry.netLuKey.dpCare    = 0xFFFFu;
                ucEntry.netLuKey.spCare    = 0xFFFFu;
                ucEntry.ttl                = ENABLED;
                ucEntry.egrIfIdx           = 1u;
                ucEntry.l2Idx              = lutSearchInfo.lutEntry.index;
                CHK_FUN_RET(ret, rtk_route_uc_set(unitChip, &ucEntry));

                CHK_FUN_RET(ret, drv_memset(&ucEntryGet, 0, sizeof(ucEntryGet)));
                ucEntryGet.tblInfo.tblType    = ucEntry.tblInfo.tblType;
                ucEntryGet.tblInfo.index      = ucEntry.tblInfo.index;
                ucEntryGet.tblInfo.entryIdx   = ucEntry.tblInfo.entryIdx;
                ucEntryGet.netLuKey.entryType = ucEntry.netLuKey.entryType;
                CHK_FUN_RET(ret, rtk_route_uc_get(unitChip, &ucEntryGet));

                if(0 == rtl_memcmp(&ucEntryGet, &ucEntry, sizeof(ucEntry)))
                {
                    (void)rtlglue_printf("Set IPv6 unicast entry successfully\n");
                }
            }
            else
            {
                (void)rtlglue_printf("No available space to store the entry!!\n");
            }
            /*
            Expected Behaviour: following static host will be set
            SIP - any
            DIP - FD53:7CB8:383:2::1E
            SPort - any
            DPort - any
            L2 index - get from L2 table
            egress intetface ID - 1
            */
        }

        /* Enable Routing */
        enable.enable = ENABLED;
        CHK_FUN_RET(ret, rtk_route_enable(unitChip, &enable));

        /* Diagnostic */
        CHK_FUN_RET(ret, dva_routing_fir_acl_bind_table_dump());

        /*
        Expected Behaviour:
        The packet from interface 0 with SIP-any DIP-FD53:7CB8:383:2::2 DPort-any SPort-any with hitting PIE 0 will be routed to interface 1
        The packet from interface 0 with SIP-any DIP-FD53:7CB8:383:2::1E DPort-any SPort-any with hitting PIE 0 will be routed to interface 1
        The packet from interface 0 with SIP-any DIP-FD53:7CB8:383:2::1E DPort-any SPort-any without hitting PIE 0 will be routed to interface 1
        VID, SMAC, SIP will be replaced according to egress interface settings, and TTL will be checked and decreased.
        Other L3 packets will be blocked
        */
    } while(1 == 0);

    return ret;
}

/**
 * @brief <b>Set ACL rule</b> \n
 *      Set ACL rule, please refer to \ref acl_white_list to see the function detail. \n
 *
 */
static RtkApiRet dva_acl_bind_rule_set(UnitChip_t unitChip)
{
    RtkApiRet              retVal             = RT_ERR_OK;
    AclTemplateInfo_t      aclTemplateInfo    = {0};
    AclPieEntryInfo_t      aclPieEntryInfo    = {0};
    AclTblAccessPieParam_t dvaPieParam        = {0};
    AclActionEntryInfo_t   aclActionEntryInfo = {0};
    AclTblAccessActParam_t dvaActParam        = {0};
    AclEnableCtrlInfo_t    enable             = {0};
    uint32                 portN;

    /*
    Configure filter key of field 0~1 of template 0
        field 0: Source MAC[15:0]
        field 1: Source MAC[31:16]
        field 2: Source MAC[47:32]
        field 3: inner VID
    */
    aclTemplateInfo.tpIdx    = 0u;
    aclTemplateInfo.fieldIdx = 0u;
    aclTemplateInfo.type     = DVA_IACL_FLD_SMAC0;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &aclTemplateInfo));
    aclTemplateInfo.tpIdx    = 0u;
    aclTemplateInfo.fieldIdx = 1u;
    aclTemplateInfo.type     = DVA_IACL_FLD_SMAC1;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &aclTemplateInfo));
    aclTemplateInfo.tpIdx    = 0u;
    aclTemplateInfo.fieldIdx = 2u;
    aclTemplateInfo.type     = DVA_IACL_FLD_SMAC2;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &aclTemplateInfo));
    aclTemplateInfo.tpIdx    = 0u;
    aclTemplateInfo.fieldIdx = 3u;
    aclTemplateInfo.type     = DVA_IACL_FLD_ITAG;
    CHK_FUN_RET(retVal, rtk_acl_template_setting(unitChip, &aclTemplateInfo));

    /* Configure below filter rule in field 0 of entry 0, SMAC: 02:7D:FA:00:10:01, ITAG: 10 */
    dvaPieParam.data0 = 0x1001u;
    dvaPieParam.mask0 = 0xFFFFu;
    dvaPieParam.data1 = 0xFA00u;
    dvaPieParam.mask1 = 0xFFFFu;
    dvaPieParam.data2 = 0x027Du;
    dvaPieParam.mask2 = 0xFFFFu;
    dvaPieParam.data3 = VLAN10;
    dvaPieParam.mask3 = 0x0FFFu;
    dvaPieParam.valid = ENABLED;
    /* Configure the ACL entry 0 (index 0 of TG0) and use the field 0~3 */
    aclPieEntryInfo.aclRuleIndex = 0u;
    aclPieEntryInfo.pPieParam    = &dvaPieParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_pie_entry(unitChip, &aclPieEntryInfo));

    /* Configure action permit */
    dvaActParam.drop = 0x0u;
    /* Configure prepared action to hardware table */
    aclActionEntryInfo.aclRuleIndex = 0u;
    aclActionEntryInfo.pActParam    = &dvaActParam;
    CHK_FUN_RET(retVal, rtk_acl_setting_action_entry(unitChip, &aclActionEntryInfo));

    /* Configure each port and each rule to ACL ON and lookup miss in ACL function is permitted for keep process by L2 and VLAN function */
    for(portN = DVA_RTK_USER_PORT_1ST; portN <= DVA_RTK_USER_PORT_END; portN++)
    {
        enable.aclEn   = DVA_ACL_ENABLE;
        enable.defAct  = DVA_ACL_DEF_ACT_PERMIT;
        enable.portNum = portN;
        CHK_FUN_CONTINUE(retVal, rtk_acl_enable_ctrl_set(unitChip, &enable));
    }

    /* Expected Behaviour:
        The inner tag of all packet with SMAC: 02:7D:FA:00:10:01, ITAG: 10 will be permited by ACL
    */
    return retVal;
}

static RtkApiRet dva_routing_fir_acl_bind_table_dump(void)
{
    RtkApiRet        ret           = RT_ERR_OK;
    UnitChip_t       unitChip      = {DVA_UNIT, CHIP_907XD_VA};
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
/**@}*/ /* DVA_FIR_EXAMPLE_STATIC_FIR_ACL_BIND */
/**@}*/ /* DVA_FIR_EXAMPLE */
/**@}*/ /* FIR_EXAMPLE */
/**@}*/ /* EXAMPLE */