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
#include "rtkas_api_mirror.h"
#include "rtkas_api_acl_ext.h"
#include "rtkas_api_vlan.h"

/* Function */

/** @addtogroup EXAMPLE Example
 *  @{
 */
/**
 * @defgroup MIRROR Mirror Example
 * @{
 */
/**
 * @defgroup RTL907XC_VA_MIRROR_EXAMPLE RTL907XC_VA Mirror Example
 * @{
 */
/**
 * @defgroup RTL907XC_VA_MIRROR_FLOWBASED_RSPAN_EXAMPLE Mirror Flow-based RSPAN Example
 * @brief <b>Flow-based Mirroring with RSPAN</b> \n
 *           Any packet field supported by Access Control Lists (ACL), such as DMAC, SMAC, and VLAN ID, can be used as characteristics of the mirrored traffic.
 *           To achieve mirroring across switches using the Remote Switched Port Analyzer(RSPAN).
 *
 * @section Purpose
 *          1. Packets with VLAN100 received on port1 are mirrored to port5 with RSPAN tag.
 *          2. Packets with VLAN200 and DIP 10.0.0.1 received on port2 are mirrored to port4.
 *
 * @section Description
 *          Mirrored Traffic: All kinds of traffic are regarded as mirrored traffic, that include broadcast packets, multicast packets, unicast packets, good packets, CRC error packets, and packets across VLAN etc. \n
 *          Mirrored Port and Mirroring Port:
 *              1.  Any original packets with VLAN100 received on Port 1 are mirrored to Port 5 with RSPAN tag. \n
 *              2.  Any modified packets with VLAN200 and DIP 10.0.0.1 received on Port 2 are mirrored to Port 4. \n
 *
 * @image html mirror/mirroring_flowbased_rspan.png
 * @{
 */

RtkApiRet cva_mirroring_flowbased_rspan(void);

RtkApiRet cva_mirroring_flowbased_rspan(void)
{
    RtkApiRet              ret                = RT_ERR_OK;
    AclTemplateId          tps                = 10;  /* template 10 */
    uint16                 index1             = 320; /* rule 320 */
    uint16                 index2             = 321; /* rule 321 */
    UnitChip_t             unitChip           = {0};
    VlanInfo_t             VlanInfo           = {0};
    AclTemplateInfo_t      aclTemplateInfo    = {0};
    AclPieEntryInfo_t      aclPieEntryInfo    = {0};
    AclTblAccessPieParam_t pPieParam          = {0};
    AclActionEntryInfo_t   aclActionEntryInfo = {0};
    AclTblAccessActParam_t pActParam          = {0};
    AclEnableCtrlInfo_t    aclEnableCtrlInfo  = {0};
    MirrorInfo_t           mirrorInfoSet1     = {0};
    MirrorInfo_t           mirrorInfoSet2     = {0};
    MirrorInfo_t           mirrorInfoGet1     = {0};
    MirrorInfo_t           mirrorInfoGet2     = {0};
    MirrorRspanTxInfo_t    rspanTxInfoSet     = {0};
    MirrorRspanTxInfo_t    rspanTxInfoGet     = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = CVA_UNIT;
        unitChip.chip = CHIP_907XC_VA;

        /* Configure environment for the example - initialize vlan */
        CHK_FUN_CONTINUE(ret, rtk_vlan_init(unitChip));
        {
            VlanInfo.vid                      = 100u;
            VlanInfo.vlanEntry.mbrMsk         = (CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_2ND);
            VlanInfo.vlanEntry.utagMsk        = 0x0u;
            VlanInfo.vlanEntry.fid            = 0u;
            VlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
            VlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
            VlanInfo.vlanEntry.interfaceId    = 0u;
            VlanInfo.vlanEntry.interfaceValid = 0u;
            CHK_FUN_CONTINUE(ret, rtk_vlan_set(unitChip, &VlanInfo));

            VlanInfo.vid                      = 200u;
            VlanInfo.vlanEntry.mbrMsk         = (CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_2ND);
            VlanInfo.vlanEntry.utagMsk        = 0x0u;
            VlanInfo.vlanEntry.fid            = 0u;
            VlanInfo.vlanEntry.ulfid          = EM_VLAN_SVL;
            VlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
            VlanInfo.vlanEntry.interfaceId    = 0u;
            VlanInfo.vlanEntry.interfaceValid = 0u;
            CHK_FUN_CONTINUE(ret, rtk_vlan_set(unitChip, &VlanInfo));
        }

        /* Setp 2. Configure packet template */
        aclTemplateInfo.tpIdx    = tps;
        aclTemplateInfo.fieldIdx = 0u;
        aclTemplateInfo.type     = CVA_IACL_FLD_ITAG;
        CHK_FUN_CONTINUE(ret, rtk_acl_template_setting(unitChip, &aclTemplateInfo));
        aclTemplateInfo.fieldIdx = 1u;
        aclTemplateInfo.type     = CVA_IACL_FLD_DIP0;
        CHK_FUN_CONTINUE(ret, rtk_acl_template_setting(unitChip, &aclTemplateInfo));
        aclTemplateInfo.fieldIdx = 2u;
        aclTemplateInfo.type     = CVA_IACL_FLD_DIP1;
        CHK_FUN_CONTINUE(ret, rtk_acl_template_setting(unitChip, &aclTemplateInfo));

        /* Setp 3. Configure ACL rule */
        pPieParam.data0 = 0x0064u;
        pPieParam.mask0 = 0x0FFFu;
        pPieParam.valid = 0x1;

        aclPieEntryInfo.aclRuleIndex = index1;
        aclPieEntryInfo.pPieParam    = &pPieParam;
        CHK_FUN_CONTINUE(ret, rtk_acl_setting_pie_entry(unitChip, &aclPieEntryInfo));

        pPieParam.data0 = 0x00C8u;
        pPieParam.data1 = 0x0001u;
        pPieParam.data2 = 0x0A00u;
        pPieParam.mask0 = 0x0FFFu;
        pPieParam.mask1 = 0xFFFFu;
        pPieParam.mask2 = 0xFFFFu;
        pPieParam.valid = 0x01u;

        aclPieEntryInfo.aclRuleIndex = index2;
        aclPieEntryInfo.pPieParam    = &pPieParam;
        CHK_FUN_CONTINUE(ret, rtk_acl_setting_pie_entry(unitChip, &aclPieEntryInfo));

        /* Setp 4. Configure ACL action*/
        pActParam.mirror0 = 1u;

        aclActionEntryInfo.aclRuleIndex = index1;
        aclActionEntryInfo.pActParam    = &pActParam;
        CHK_FUN_CONTINUE(ret, rtk_acl_setting_action_entry(unitChip, &aclActionEntryInfo));

        pActParam.mirror0 = 0u;
        pActParam.mirror1 = 1u;

        aclActionEntryInfo.aclRuleIndex = index2;
        aclActionEntryInfo.pActParam    = &pActParam;
        CHK_FUN_CONTINUE(ret, rtk_acl_setting_action_entry(unitChip, &aclActionEntryInfo));

        /* Setp 5. Enable ACL*/
        aclEnableCtrlInfo.portNum = CVA_RTK_USER_PORT_4TH;
        aclEnableCtrlInfo.aclEn   = CVA_ACL_ENABLE;
        aclEnableCtrlInfo.defAct  = CVA_ACL_DEF_ACT_PERMIT;
        CHK_FUN_CONTINUE(ret, rtk_acl_enable_ctrl_set(unitChip, &aclEnableCtrlInfo));
        aclEnableCtrlInfo.portNum = CVA_RTK_USER_PORT_5TH;
        CHK_FUN_CONTINUE(ret, rtk_acl_enable_ctrl_set(unitChip, &aclEnableCtrlInfo));

        /* Step 6. Configure Mirroring Function 1 */
        mirrorInfoSet1.mirrorNo                = 1u;
        mirrorInfoSet1.mirroringPort           = CVA_RTK_USER_PORT_5TH;
        mirrorInfoSet1.mirroredIngressPortmask = CVA_RTK_USER_PMAP_1ST;
        mirrorInfoSet1.mirroredEgressPortmask  = 0x000u;
        mirrorInfoSet1.morg                    = MIRROR_ORIGINAL_PACKET;
        CHK_FUN_CONTINUE(ret, rtk_mirror_flow_based_set(unitChip, &mirrorInfoSet1));
        mirrorInfoGet1.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_flow_based_get(unitChip, &mirrorInfoGet1));

        /* Step 7. Configure Mirroring Function 1 - insert tag */
        rspanTxInfoSet.mirrorNo      = 1u;
        rspanTxInfoSet.mirroringPort = CVA_RTK_USER_PORT_5TH;
        rspanTxInfoSet.action        = EM_RSPAN_TXTAG_ADD;
        rspanTxInfoSet.tag.tpid      = 0x88A8u;
        rspanTxInfoSet.tag.pri       = 0x5u;
        rspanTxInfoSet.tag.cfi       = 0x0u;
        rspanTxInfoSet.tag.vid       = 0x123u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_set(unitChip, &rspanTxInfoSet));
        rspanTxInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_get(unitChip, &rspanTxInfoGet));

        /* Step 8. Configure Mirroring Function 2 */
        mirrorInfoSet2.mirrorNo                = 2u;
        mirrorInfoSet2.mirroringPort           = CVA_RTK_USER_PORT_4TH;
        mirrorInfoSet2.mirroredIngressPortmask = CVA_RTK_USER_PMAP_2ND;
        mirrorInfoSet2.mirroredEgressPortmask  = 0x000u;
        mirrorInfoSet2.morg                    = MIRROR_MODIFIED_PACKET;
        CHK_FUN_CONTINUE(ret, rtk_mirror_flow_based_set(unitChip, &mirrorInfoSet2));
        mirrorInfoGet2.mirrorNo = 2u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_flow_based_get(unitChip, &mirrorInfoGet2));

        /* Expected Behavior:
           1. Packets with VLAN100 received on port1 are mirrored to port5 with RSPAN tag.
           2. Packets with VLAN200 and DIP 10.0.0.1 received on port2 are mirrored to port4.
        */
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XC_VA_MIRROR_FLOWBASED_RSPAN_EXAMPLE */
/**@}*/ /* RTL907XC_VA_MIRROR_EXAMPLE */
/**@}*/ /* MIRROR */
/**@}*/ /* EXAMPLE */
