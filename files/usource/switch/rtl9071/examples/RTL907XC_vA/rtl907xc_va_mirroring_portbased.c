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
#include "rtkas_api_l2.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
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
 * @defgroup RTL907XC_VA_MIRROR_PORTBASED_EXAMPLE Mirror Port-based Example
 * @brief <b>Port-based Mirroring – Two Mirror Function</b> \n
 *           Any packet received from specific ports will be mirrored to a designated mirroring port, with normal forwarding packets filtered out and packets mirrored at the specified sample rate.
 *
 * @section Purpose
 *          Packets transmitted to port4 and port5 are mirrored to port6
 *
 * @section Description
 *          Mirrored Traffic: Any packet received from specific ports will be mirrored to a designated mirroring port, with normal forwarding packets filtered out and packets mirrored at the specified sample rate. \n
 *          Mirrored Port and Mirroring Port: Any original packets received on Port 1 and Port 2 are mirrored to Port 3. In the meantime, any modified packets transmitted from Port 4 and Port 5 are mirrored to Port 6. \n
 *          Mirroring Sample Rate: One in five mirrored packets is copied to the mirroring port. \n
 *          Mirroring Filter: Port 3 is able to receive normal packets and mirrored packets but Port 6 can only receive mirrored packets. \n
 *
 * @image html mirror/mirroring_portbased.png
 * @{
 */

RtkApiRet cva_mirroring_portbased(void);

RtkApiRet cva_mirroring_portbased(void)
{
    RtkApiRet          ret                  = RT_ERR_OK;
    uint8              dstMac[MAC_ADDR_LEN] = {0x00u, 0xE0u, 0x4Cu, 0x68u, 0x04u, 0x00u};
    uint8              port                 = 0u;
    UnitChip_t         unitChip             = {0};
    LutUniAddInfo_t    LutUniAddInfo        = {0};
    MirrorInfo_t       mirrorInfoSet1       = {0};
    MirrorInfo_t       mirrorInfoGet1       = {0};
    MirrorInfo_t       mirrorInfoSet2       = {0};
    MirrorInfo_t       mirrorInfoGet2       = {0};
    MirrorSampleInfo_t sampleInfoSet1       = {0};
    MirrorSampleInfo_t sampleInfoGet1       = {0};
    MirrorSampleInfo_t sampleInfoSet2       = {0};
    MirrorSampleInfo_t sampleInfoGet2       = {0};
    MirrorFilterInfo_t filterInfoSet1       = {0};
    MirrorFilterInfo_t filterInfoGet1       = {0};
    MirrorFilterInfo_t filterInfoSet2       = {0};
    MirrorFilterInfo_t filterInfoGet2       = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = CVA_UNIT;
        unitChip.chip = CHIP_907XC_VA;

        /* Configure environment for the example - initialize vlan */
        CHK_FUN_CONTINUE(ret, rtk_vlan_init(unitChip));

        /* Configure environment for the example - add unicast entries */
        for(port = CVA_RTK_USER_PORT_1ST; port <= (uint8)CVA_RTK_USER_PORT_END; port++)
        {
            CHK_FUN_RET(ret, drv_memcpy(LutUniAddInfo.mac, dstMac, sizeof(LutUniAddInfo.mac)));
            LutUniAddInfo.vid   = 1u;
            LutUniAddInfo.port  = port;
            LutUniAddInfo.fid   = 0u;
            LutUniAddInfo.ulfid = 0u;
            CHK_FUN_CONTINUE(ret, rtk_lut_uni_entry_add(unitChip, &LutUniAddInfo));
            dstMac[MAC_ADDR_LEN - 1u]++;
        }

        /* Step 2. Configure Mirroring Function 1 - mirrored and mirroring port */
        mirrorInfoSet1.mirrorNo                = 1u;
        mirrorInfoSet1.mirroringPort           = CVA_RTK_USER_PORT_3RD;
        mirrorInfoSet1.mirroredIngressPortmask = (CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_2ND);
        mirrorInfoSet1.mirroredEgressPortmask  = 0x000u;
        mirrorInfoSet1.morg                    = MIRROR_ORIGINAL_PACKET;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_set(unitChip, &mirrorInfoSet1));
        mirrorInfoGet1.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_get(unitChip, &mirrorInfoGet1));

        /* Step 3. Configure Mirroring Function 1 - sample rate */
        sampleInfoSet1.mirrorNo = 1u;
        sampleInfoSet1.samples  = 0x0005u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_set(unitChip, &sampleInfoSet1));
        sampleInfoGet1.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_get(unitChip, &sampleInfoGet1));

        /* Step 4. Configure Mirroring Function 1 - filter */
        filterInfoSet1.mirrorNo = 1u;
        filterInfoSet1.enable   = DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_set(unitChip, &filterInfoSet1));
        filterInfoGet1.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_get(unitChip, &filterInfoGet1));

        /* Expected Behavior: Packets received from port1 and port2 are mirrored to port3 */

        /* Step 5. Configure Mirroring Function 2 - mirrored and mirroring port */
        mirrorInfoSet2.mirrorNo                = 2u;
        mirrorInfoSet2.mirroringPort           = CVA_RTK_USER_PORT_6TH;
        mirrorInfoSet2.mirroredIngressPortmask = 0x000u;
        mirrorInfoSet2.mirroredEgressPortmask  = (CVA_RTK_USER_PMAP_4TH | CVA_RTK_USER_PMAP_5TH);
        mirrorInfoSet2.morg                    = MIRROR_MODIFIED_PACKET;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_set(unitChip, &mirrorInfoSet2));
        mirrorInfoGet2.mirrorNo = 2u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_get(unitChip, &mirrorInfoGet2));

        /* Step 6. Configure Mirroring Function 2 - sample rate */
        sampleInfoSet2.mirrorNo = 2u;
        sampleInfoSet2.samples  = 0x0005u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_set(unitChip, &sampleInfoSet2));
        sampleInfoGet2.mirrorNo = 2u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_get(unitChip, &sampleInfoGet2));

        /* Step 7. Configure Mirroring Function 2 - filter */
        filterInfoSet2.mirrorNo = 2u;
        filterInfoSet2.enable   = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_set(unitChip, &filterInfoSet2));
        filterInfoGet2.mirrorNo = 2u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_get(unitChip, &filterInfoGet2));

        /* Expected Behavior: Packets transmitted to port4 and port5 are mirrored to port6 */
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XC_VA_MIRROR_PORTBASED_EXAMPLE */
/**@}*/ /* RTL907XC_VA_MIRROR_EXAMPLE */
/**@}*/ /* MIRROR */
/**@}*/ /* EXAMPLE */
