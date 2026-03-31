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
#include "rtkas_api_l2.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup MIRROR Mirror Example
 * @{
 */
/**
 * @addtogroup RTL907XD_V1_MIRROR_EXAMPLE RTL907XD_V1 Mirror Example
 * @{
 */
/**
 * @addtogroup RTL907XD_V1_MIRROR_FLOWBASED_EXAMPLE Mirror Flow-based Example
 * @brief <b>Flow-based Mirroring</b> \n
 *           Any packet field supported by Access Control Lists (ACL), such as DMAC, SMAC, and VLAN ID, can be used as characteristics of the mirrored traffic.
 *
 * @section Purpose
 *          Packets with DMAC 00:E0:4C:68:04:0C received on port1 and port2 are mirrored to port3.
 *
 * @section Description
 *          Mirrored Traffic: All kinds of traffic are regarded as mirrored traffic, including broadcast packets, multicast packets, unicast packets, good packets, CRC error packets, and packets across VLAN, etc. \n
 *          Mirrored Port and Mirroring Port: Any original packets with DMAC 00:E0:4C:68:04:0C received on Port 1 and Port 2 are mirrored to Port 3. \n
 *          Mirroring Sample Rate: One of ten mirrored packets is copied to the mirroring port. \n
 *          Mirroring Filter: Port 3 only receives mirrored packets. \n
 *
 * @image html mirror/mirroring_flowbased.png
 * @{
 */

RtkApiRet dv1_mirroring_flowbased(void);

/* Function */

RtkApiRet dv1_mirroring_flowbased(void)
{
    RtkApiRet              ret                  = RT_ERR_OK;
    uint8                  dstMac[MAC_ADDR_LEN] = {0x00u, 0xE0u, 0x4Cu, 0x68u, 0x04u, 0x00u};
    uint8                  port                 = 0u;
    AclTemplateId          tps                  = 10;  /* template 10 */
    uint16                 index                = 320; /* rule 320 (i.e. rule 0 of tempalte 10) */
    UnitChip_t             unitChip             = {0};
    MirrorInfo_t           mirrorInfoSet        = {0};
    MirrorInfo_t           mirrorInfoGet        = {0};
    LutUniAddInfo_t        LutUniAddInfo        = {0};
    AclTemplateInfo_t      aclTemplateInfo      = {0};
    AclPieEntryInfo_t      aclPieEntryInfo      = {0};
    AclTblAccessPieParam_t pPieParam            = {0};
    AclActionEntryInfo_t   aclActionEntryInfo   = {0};
    AclTblAccessActParam_t pActParam            = {0};
    AclEnableCtrlInfo_t    aclEnableCtrlInfo    = {0};
    MirrorSampleInfo_t     sampleInfoSet        = {0};
    MirrorSampleInfo_t     sampleInfoGet        = {0};
    MirrorFilterInfo_t     filterInfoSet        = {0};
    MirrorFilterInfo_t     filterInfoGet        = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = DV1_UNIT;
        unitChip.chip = CHIP_907XD_V1;

        /* Configure environment for the example - initialize vlan */
        CHK_FUN_CONTINUE(ret, rtk_vlan_init(unitChip));

        /* Configure environment for the example - add unicast entries */
        for(port = DV1_RTK_USER_PORT_1ST; port <= (uint8)DV1_RTK_USER_PORT_END; port++)
        {
            CHK_FUN_RET(ret, drv_memcpy(LutUniAddInfo.mac, dstMac, sizeof(LutUniAddInfo.mac)));
            LutUniAddInfo.vid   = 1u;
            LutUniAddInfo.port  = port;
            LutUniAddInfo.fid   = 0u;
            LutUniAddInfo.ulfid = 0u;
            (void)rtk_lut_uni_entry_add(unitChip, &LutUniAddInfo);
            dstMac[MAC_ADDR_LEN - 1u]++;
        }

        /* Setp 2. Configure packet template - DMAC address */
        aclTemplateInfo.tpIdx    = tps;
        aclTemplateInfo.fieldIdx = 0u;
        aclTemplateInfo.type     = DV1_IACL_FLD_DMAC2;
        CHK_FUN_CONTINUE(ret, rtk_acl_template_setting(unitChip, &aclTemplateInfo));
        aclTemplateInfo.fieldIdx = 1u;
        aclTemplateInfo.type     = DV1_IACL_FLD_DMAC1;
        CHK_FUN_CONTINUE(ret, rtk_acl_template_setting(unitChip, &aclTemplateInfo));
        aclTemplateInfo.fieldIdx = 2u;
        aclTemplateInfo.type     = DV1_IACL_FLD_DMAC0;
        CHK_FUN_CONTINUE(ret, rtk_acl_template_setting(unitChip, &aclTemplateInfo));

        /* Setp 3. Configure ACL rule - DMAC = 0x00-E0-4C-68-04-0C */
        pPieParam.data0 = 0x00E0u;
        pPieParam.data1 = 0x4C68u;
        pPieParam.data2 = 0x040Cu;
        pPieParam.mask0 = 0xFFFFu;
        pPieParam.mask1 = 0xFFFFu;
        pPieParam.mask2 = 0xFFFFu;
        pPieParam.valid = 0x01u;

        aclPieEntryInfo.aclRuleIndex = index;
        aclPieEntryInfo.pPieParam    = &pPieParam;
        CHK_FUN_CONTINUE(ret, rtk_acl_setting_pie_entry(unitChip, &aclPieEntryInfo));

        /* Setp 4. Configure ACL action - mirroring Function 1*/
        pActParam.mirror0 = 1u;

        aclActionEntryInfo.aclRuleIndex = index;
        aclActionEntryInfo.pActParam    = &pActParam;
        CHK_FUN_CONTINUE(ret, rtk_acl_setting_action_entry(unitChip, &aclActionEntryInfo));

        /* Setp 5. Enable ACL*/
        aclEnableCtrlInfo.portNum = DV1_RTK_USER_PORT_3RD;
        aclEnableCtrlInfo.aclEn   = DV1_ACL_ENABLE;
        aclEnableCtrlInfo.defAct  = DV1_ACL_DEF_ACT_PERMIT;
        CHK_FUN_CONTINUE(ret, rtk_acl_enable_ctrl_set(unitChip, &aclEnableCtrlInfo));

        /* Step 6. Configure Mirroring Function 1 - mirrored and mirroring port */
        mirrorInfoSet.mirrorNo                = 1u;
        mirrorInfoSet.mirroringPort           = DV1_RTK_USER_PORT_3RD;
        mirrorInfoSet.mirroredIngressPortmask = (DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND);
        mirrorInfoSet.mirroredEgressPortmask  = 0x000u;
        mirrorInfoSet.morg                    = MIRROR_ORIGINAL_PACKET;
        CHK_FUN_CONTINUE(ret, rtk_mirror_flow_based_set(unitChip, &mirrorInfoSet));
        mirrorInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_flow_based_get(unitChip, &mirrorInfoGet));

        /* Step 7. Configure Mirroring Function 1 - sample rate */
        sampleInfoSet.mirrorNo = 1u;
        sampleInfoSet.samples  = 0x000Au;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_set(unitChip, &sampleInfoSet));
        sampleInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_get(unitChip, &sampleInfoGet));

        /* Step 8. Configure Mirroring Function 1 - filter */
        filterInfoSet.mirrorNo = 1u;
        filterInfoSet.enable   = ENABLED;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_set(unitChip, &filterInfoSet));
        filterInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_get(unitChip, &filterInfoGet));

        /* Expected Behavior: Packets with DMAC 00:E0:4C:68:04:0C received on port1 and port2 are mirrored to port3 */
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XD_V1_MIRROR_FLOWBASED_EXAMPLE */
/**@}*/ /* RTL907XD_V1_MIRROR_EXAMPLE */
/**@}*/ /* MIRROR */
/**@}*/ /* EXAMPLE */
