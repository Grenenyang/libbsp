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
 * @defgroup RTL907XC_VA_MIRROR_SWITCHES_EXAMPLE Mirror Switches Example
 * @brief <b>Mirroring RSPAN(Across Switches) </b> \n
 *           The switch with mirrored ports, referred to as the mirrored switch, should define mirrored traffic, mirrored ports, and its mirroring port. The mirrored switch should always set the RSPAN action to ADD, which means all mirrored packets will have an RSPAN tag inserted and then be transmitted from the mirroring port. \n
 *           The switch with the last mirroring port connected to a network sniffer, known as the mirroring switch, must set the RSPAN action to REMOVE. This setting ensures that mirrored packets, without the RSPAN tag, are transmitted from the mirroring port. \n
 *           The switches between the mirrored switch and the mirroring switch, known as intermediate switches, should set the RSPAN action to RETAIN, which means mirrored packets with the RSPAN tag will be transmitted directly from the mirroring port. \n
 *           Note: If you want to switch to the intermediate switch or mirroring switch after setting it to the mirrored switch, you need to first call the rtk_mirror_reset() to clear the previous mirror settings. \n
 *
 * @section Purpose
 *          Usage of RSPAN mirroring.
 *
 * @section Description
 *          The topology for RSPAN mirroring is as follows:
 *
 * @image html mirror/mirroring_switches.png
 *
 * The RSPAN tag for RSPAN mirroring is as follows:
 *
 * |  TPID (16 bits) |  PRI (3 bits) |  CFI (1 bit) |  VID (12 bits) |
 * |:---------------:|:-------------:|:------------:|:--------------:|
 * |      0x8100     |      0x5      |      0x0     |      0x123     |
 * @{
 */

/**
 * @brief <b>Mirroring RSPAN(Across Switches): Mirrored Switch </b> \n
 *
 * @section Purpose
 *          Packets received on port 1 is mirrored to port 3.
 *
 * @section Description
 *          Any original packets received from Port 1 are regarded as mirrored packets, that are mirrored to Port 3, and a RSPAN tag is inserted in front of the packets. \n
 *          The RSPAN tag is defined in the table below. \n
 *
 * @{
 */
RtkApiRet cva_mirroring_for_mirrored_switch(void);

RtkApiRet cva_mirroring_for_mirrored_switch(void)
{
    RtkApiRet           ret                  = RT_ERR_OK;
    uint8               dstMac[MAC_ADDR_LEN] = {0x00u, 0xE0u, 0x4Cu, 0x68u, 0x04u, 0x00u};
    uint8               port                 = 0u;
    UnitChip_t          unitChip             = {0};
    LutUniAddInfo_t     LutUniAddInfo        = {0};
    MirrorInfo_t        mirrorInfoSet        = {0};
    MirrorInfo_t        mirrorInfoGet        = {0};
    MirrorSampleInfo_t  sampleInfoSet        = {0};
    MirrorSampleInfo_t  sampleInfoGet        = {0};
    MirrorFilterInfo_t  filterInfoSet        = {0};
    MirrorFilterInfo_t  filterInfoGet        = {0};
    MirrorRspanTxInfo_t rspanTxInfoSet       = {0};
    MirrorRspanTxInfo_t rspanTxInfoGet       = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = CVA_UNIT;
        unitChip.chip = CHIP_907XC_VA;

        /* Configure environment for the example - initialize vlan */
        CHK_FUN_CONTINUE(ret, rtk_vlan_init(unitChip));

        /* Configure environment for the example - add unicast entries */
        for(port = CVA_RTK_USER_PORT_1ST; port < (uint8)CVA_RTK_USER_PORT_END; port++)
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
        mirrorInfoSet.mirrorNo                = 1u;
        mirrorInfoSet.mirroringPort           = CVA_RTK_USER_PORT_3RD;
        mirrorInfoSet.mirroredIngressPortmask = CVA_RTK_USER_PMAP_1ST;
        mirrorInfoSet.mirroredEgressPortmask  = 0x000u;
        mirrorInfoSet.morg                    = MIRROR_ORIGINAL_PACKET;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_set(unitChip, &mirrorInfoSet));
        mirrorInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_port_based_get(unitChip, &mirrorInfoGet));

        /* Step 3. Configure Mirroring Function 1 - sample rate */
        sampleInfoSet.mirrorNo = 1u;
        sampleInfoSet.samples  = 0x0001u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_set(unitChip, &sampleInfoSet));
        sampleInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_sample_rate_get(unitChip, &sampleInfoGet));

        /* Step 4. Configure Mirroring Function 1 - filter */
        filterInfoSet.mirrorNo = 1u;
        filterInfoSet.enable   = ENABLE;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_set(unitChip, &filterInfoSet));
        filterInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_mirror_filter_get(unitChip, &filterInfoGet));

        /* Step 5. Configure Mirroring Function 1 - insert tag */
        rspanTxInfoSet.mirrorNo = 1u;
        rspanTxInfoSet.action   = EM_RSPAN_TXTAG_ADD;
        rspanTxInfoSet.tag.tpid = 0x8100u;
        rspanTxInfoSet.tag.pri  = 0x5u;
        rspanTxInfoSet.tag.cfi  = 0x0u;
        rspanTxInfoSet.tag.vid  = 0x123u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_set(unitChip, &rspanTxInfoSet));
        rspanTxInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_get(unitChip, &rspanTxInfoGet));

        /* Expected Behavior: Packets received on port 1 is mirrored to port 3 */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XC_VA_MIRROR_SWITCHES_EXAMPLE */

/**
 * @defgroup RTL907XC_VA_MIRROR_SWITCHES_EXAMPLE Mirror Switches Example
 * @{
 */

/**
 * @brief <b>Mirroring RSPAN(Across Switches): Intermediate Switch </b> \n
 *
 * @section Purpose
 *          Packets with an RSPAN tag received on port 5 are transferred to port 7.
 *
 * @section Description
 *          All packets with a RSPAN tag whose VID is 0x123 received on Port 5 are regarded as mirrored packets, that are transmitted from Port 7 directly.
 *
 * @{
 */
RtkApiRet cva_mirroring_for_intermediate_switch(void);

RtkApiRet cva_mirroring_for_intermediate_switch(void)
{
    RtkApiRet           ret            = RT_ERR_OK;
    UnitChip_t          unitChip       = {0};
    MirrorRspanRxInfo_t rspanRxInfoSet = {0};
    MirrorRspanRxInfo_t rspanRxInfoGet = {0};
    MirrorRspanTxInfo_t rspanTxInfoSet = {0};
    MirrorRspanTxInfo_t rspanTxInfoGet = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = CVA_UNIT;
        unitChip.chip = CHIP_907XC_VA;

        /* Step 2. Configure Mirroring Function 1 - check tag */
        rspanRxInfoSet.mirrorNo         = 1u;
        rspanRxInfoSet.mirroredPortmask = CVA_RTK_USER_PMAP_5TH;
        rspanRxInfoSet.enable           = ENABLE;
        rspanRxInfoSet.vid              = 0x123u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_rx_set(unitChip, &rspanRxInfoSet));
        rspanRxInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_rx_get(unitChip, &rspanRxInfoGet));

        /* Step 3. Configure Mirroring Function 1 - retain tag */
        rspanTxInfoSet.mirrorNo      = 1u;
        rspanTxInfoSet.mirroringPort = CVA_RTK_USER_PORT_7TH;
        rspanTxInfoSet.action        = EM_RSPAN_TXTAG_RETAIN;
        rspanTxInfoSet.tag.tpid      = 0x0u;
        rspanTxInfoSet.tag.pri       = 0x0u;
        rspanTxInfoSet.tag.cfi       = 0x0u;
        rspanTxInfoSet.tag.vid       = 0x0u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_set(unitChip, &rspanTxInfoSet));
        rspanTxInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_get(unitChip, &rspanTxInfoGet));

        /* Expected Behavior: Packets with an RSPAN tag received on port 5 are transferred to port 7 */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XC_VA_MIRROR_SWITCHES_EXAMPLE */

/**
 * @defgroup RTL907XC_VA_MIRROR_SWITCHES_EXAMPLE Mirror Switches Example
 * @{
 */

/**
 * @brief <b>Mirroring RSPAN(Across Switches): Mirroring Switch </b> \n
 *
 * @section Purpose
 *          Packets with an RSPAN tag received on port 9 are transferred to port 11 without an RSPAN tag.
 *
 * @section Description
 *           The RSPAN tag whose VID is 0x123 of the packet received on Port 9 is removed and then transmitted from Port 11. \n
 *
 * @{
 */
RtkApiRet cva_mirroring_for_mirroring_switch(void);

RtkApiRet cva_mirroring_for_mirroring_switch(void)
{
    RtkApiRet           ret            = RT_ERR_OK;
    UnitChip_t          unitChip       = {0};
    MirrorRspanRxInfo_t rspanRxInfoSet = {0};
    MirrorRspanRxInfo_t rspanRxInfoGet = {0};
    MirrorRspanTxInfo_t rspanTxInfoSet = {0};
    MirrorRspanTxInfo_t rspanTxInfoGet = {0};

    do
    {
        /* Step 1. Configure the unit and chip id */
        unitChip.unit = CVA_UNIT;
        unitChip.chip = CHIP_907XC_VA;

        /* Step 2. Configure Mirroring Function 1 - check tag */
        rspanRxInfoSet.mirrorNo         = 1u;
        rspanRxInfoSet.mirroredPortmask = CVA_RTK_USER_PMAP_9TH;
        rspanRxInfoSet.enable           = ENABLE;
        rspanRxInfoSet.vid              = 0x123u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_rx_set(unitChip, &rspanRxInfoSet));
        rspanRxInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_rx_get(unitChip, &rspanRxInfoGet));

        /* Step 3. Configure Mirroring Function 1 - remove tag */
        rspanTxInfoSet.mirrorNo      = 1u;
        rspanTxInfoSet.mirroringPort = CVA_RTK_USER_PORT_11TH;
        rspanTxInfoSet.action        = EM_RSPAN_TXTAG_REMOVE;
        rspanTxInfoSet.tag.tpid      = 0x0u;
        rspanTxInfoSet.tag.pri       = 0x0u;
        rspanTxInfoSet.tag.cfi       = 0x0u;
        rspanTxInfoSet.tag.vid       = 0x0u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_set(unitChip, &rspanTxInfoSet));
        rspanTxInfoGet.mirrorNo = 1u;
        CHK_FUN_CONTINUE(ret, rtk_rspan_tx_get(unitChip, &rspanTxInfoGet));

        /* Expected Behavior: Packets with an RSPAN tag received on port 9 are transferred to port 11 without an RSPAN tag */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XC_VA_MIRROR_SWITCHES_EXAMPLE */

/**@}*/ /* RTL907XC_VA_MIRROR_EXAMPLE */
/**@}*/ /* MIRROR */
/**@}*/ /* EXAMPLE */
