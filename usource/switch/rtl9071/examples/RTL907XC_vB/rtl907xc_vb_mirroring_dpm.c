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

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup MIRROR Mirror Example
 * @{
 */
/**
 * @addtogroup RTL907XC_VB_MIRROR_EXAMPLE RTL907XC_VB Mirror Example
 * @{
 */
/**
 * @addtogroup RTL907XC_VB_MIRROR_DPM_EXAMPLE Mirror DPM Example
 * @brief <b>Port-based Mirroring</b> \n
 *           Any packet received from specific ports will be mirrored to a designated mirroring port.
 *
 * @section Purpose
 *          Any packets received on port1, port2 and transmitted from port1, port2 are mirrored to port3.
 *
 * @section Description
 *          Mirrored Traffic: All kinds of traffic are regarded as mirrored traffic, that include broadcast packets, multicast packets, unicast packets, good packets, CRC error packets, and packets across VLAN etc. \n
 *          Mirrored Port and Mirroring Port: Any original packet received and transmitted from Port 1, Port 2 are mirrored to Port 3. \n
 * @image html mirror/mirroring_dpm.png
 *
 * @{
 */

RtkApiRet cvb_mirroring_dpm(void);

/* Function */

RtkApiRet cvb_mirroring_dpm(void)
{
    RtkApiRet    ret           = RT_ERR_OK;
    UnitChip_t   unitChip      = {0};
    MirrorInfo_t mirrorInfoSet = {0};
    MirrorInfo_t mirrorInfoGet = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = CVB_UNIT;
        unitChip.chip = CHIP_907XC_VB;

        /* Step 2. Configure Mirroring Function 1 - mirrored and mirroring port */
        mirrorInfoSet.mirrorNo                = 1u;
        mirrorInfoSet.mirroringPort           = CVB_RTK_USER_PORT_3RD;
        mirrorInfoSet.mirroredIngressPortmask = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND);
        mirrorInfoSet.mirroredEgressPortmask  = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_2ND);
        mirrorInfoSet.morg                    = MIRROR_ORIGINAL_PACKET;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_set(unitChip, &mirrorInfoSet));
        mirrorInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_get(unitChip, &mirrorInfoGet));

        /* Expected Behavior: Any packets received on port1, port2 and transmitted from port1, port2 are mirrored to port3 */
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XC_VB_MIRROR_DPM_EXAMPLE */
/**@}*/ /* RTL907XC_VB_MIRROR_EXAMPLE */
/**@}*/ /* MIRROR */
/**@}*/ /* EXAMPLE */
