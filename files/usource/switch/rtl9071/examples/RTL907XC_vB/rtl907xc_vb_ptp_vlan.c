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

#include "rtkas_api_ptp_ext.h"
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup PTP PTP Example
 * @{
 */
/**
 * @defgroup RTL907XCVB_PTP_EXAMPLE RTL907XC_VB PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XCVB_PTP_VLAN_EXAMPLE PTP vlan Example
 *  @brief <b>PTP with VLAN Tag</b> \n
 *  @section Purpose
 *              Customization of PTP messages with VLAN tags. \n\n
 *  @section Description
 *              According to 802.1AS-2020, a frame carrying an IEEE 802.1AS message shall not have a VLAN tag nor a priority tag. \n
 *              By default, PTP messages on the switch do not have a VLAN tag. \n
 *              If there is a requirement to support VLAN tags on PTP messages, the switch can support this customization. \n
 *              Note that this PTP message with VLAN tag customization must be applied to all PTP devices (GM, Bridge, End point) in the environment to ensure successful time synchronization. \n
 *              In this example, ports 1 and 2 are configured with VLAN ID 100, and the PTP VLAN tag ID is set to 100. All PTP messages will be transferred with VLAN ID 100.
 *  @{
 */

RtkApiRet cvb_ptp_vlan(void);

RtkApiRet cvb_ptp_vlan(void)
{
    uint8      port             = 0u;
    RtkVlan    vlanId           = 0u;
    uint8      domainid         = 0u;
    RtkApiRet  ret              = RT_ERR_OK;
    RtkApiRet  apiresult        = RT_ERR_OK;
    uint32     callAPIerrorFlag = 0u;
    UnitChip_t unitChip;
    unitChip.chip = CHIP_907XC_VB;
    unitChip.unit = CVB_UNIT;

    PtpGlenable_t         ptpGlenable;
    PtpCustomVlanEnable_t ptpCustomVlanEnable;
    VlanInfo_t            VlanInfo;
    PtpTag_t              ptpTag;

    do
    {
        /* Step 1. Disable PTP to configure. */
        ptpGlenable.glenable = 0;
        CHK_FUN_CONTINUE((ret), rtk_ptp_get_glenable(unitChip, &ptpGlenable));

        for(port = CVB_RTK_USER_PORT_1ST; port <= CVB_RTK_USER_PORT_11TH; port++)
        {
            /* Step 2. Enable RX VLAN filter. */
            ptpCustomVlanEnable.port   = port;
            ptpCustomVlanEnable.enable = 1u;
            apiresult                  = rtk_ptp_custom_set_vlan_enable(unitChip, &ptpCustomVlanEnable);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint32)1u << 0);
            }
        }
        if(0u != callAPIerrorFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
        /* Step 3. Set internal port. */
        ptpCustomVlanEnable.port   = CVB_RTK_CPU_PORT;
        ptpCustomVlanEnable.enable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_custom_set_vlan_enable(unitChip, &ptpCustomVlanEnable));

        /* Step 4. Set VLAN ports, port1 and port2 with VLAN 100. */
        CHK_FUN_CONTINUE((ret), rtk_vlan_init(unitChip));
        vlanId                            = 100u;
        VlanInfo.vid                      = vlanId;
        VlanInfo.vlanEntry.mbrMsk         = CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND;
        VlanInfo.vlanEntry.utagMsk        = 0x3Fu;
        VlanInfo.vlanEntry.statefulMsk    = 0u;
        VlanInfo.vlanEntry.fid            = 0u;
        VlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
        VlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
        VlanInfo.vlanEntry.interfaceId    = 0u;
        VlanInfo.vlanEntry.interfaceValid = 0u;
        CHK_FUN_RET(ret, rtk_vlan_set(unitChip, &VlanInfo));

        /* Step 5. Set PTP VLAN ID to 100. */
        ptpTag.taginfo.fix            = 1u;
        ptpTag.taginfo.qtag[0].enable = 1u;
        ptpTag.taginfo.qtag[0].tci    = (uint16)vlanId;
        ptpTag.taginfo.enable         = 1u;
        for(domainid = 0u; domainid < 3u; domainid++)
        {
            ptpTag.domainid = domainid;
            apiresult       = rtk_ptp_tag_set(unitChip, &ptpTag);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint32)1u << 1);
            }
        }
        if(0u != callAPIerrorFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 6. Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_get_glenable(unitChip, &ptpGlenable));

        /* Step 7. Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), rtk_ptp_reinitprotocol(unitChip));
    } while(0u == 1u);

    /* Expected Behaviour:
     * 1. Check clock stable on DUT. clkstable: > 0 means stable. 0 means unstable.
     * - Check it by API rtk_ptp_clkstable_get(uint32 domain, uint8 *clkstable).
     * 2. Sync/Follow_Up from DUT should have VLAN ID 100.
     * */

    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XCVB_PTP_EXAMPLE */
/**@}*/ /* RTL907XCVB_PTP_VLAN_EXAMPLE */