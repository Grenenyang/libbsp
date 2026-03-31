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

#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_va_api_l2.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_l2.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xd_va_api_l2.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xd_va_api_l2.h"
#endif
#include "rtkas_api_l2.h"
#include "rtl907xd_va_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup L2_EXAMPLE L2 Example
 * @{
 */
/**
 * @addtogroup RTL907XDVA_L2_EXAMPLE RTL907XD_VA L2 Example
 * @{
 */
/**
 * @addtogroup dva_l2_sa_move L2 SA move example
 *
 * @brief This example demonstrates how to configure SA move settings.
 *
 * @section Purpose
 *      To demostrate how to use APIs to configure SA move settings.
 *
 * @section Description
 *        1. Set/Get legal/illegal ports for a specific port. \n
 *        2. Set/Get legal/illegal forward action for a specific port. \n
 *        3. Set SA move CPU ports. \n
 *        4. Enable legal/illegal record function and get legal/illegal records. \n
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_l2_sa_move(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_l2_sa_move(void)
{
    RtkApiRet retVal = RT_ERR_OK;

    UnitChip_t                      unitChip                      = {0};
    LutMacLrnInfo_t                 lutMacLrnInfo                 = {0};
    LutSaMoveLegalPortInfo_t        lutSaMoveLegalPortInfo        = {0};
    LutSaMoveLegalPortActInfo_t     lutSaMoveLegalPortActInfo     = {0};
    LutSaMoveIllegalPortActInfo_t   lutSaMoveIllegalPortActInfo   = {0};
    LutSaMoveCpuPortInfo_t          lutSaMoveCpuPortInfo          = {0};
    LutLegalSaMoveRecordInfo_t      lutLegalSaMoveRecordInfo      = {0};
    LutIllegalSaMoveRecordInfo_t    lutIllegalSaMoveRecordInfo    = {0};
    LutLegalSaMoveRecordGetInfo_t   lutLegalSaMoveRecordGetInfo   = {0};
    LutIllegalSaMoveRecordGetInfo_t lutIllegalSaMoveRecordGetInfo = {0};
    VlanInfo_t                      vlanInfo                      = {0};
    VlanPortEgrFilterInfo_t         vlanPortEgrFilterInfo         = {0};

    unitChip.unit = DVA_UNIT;
    unitChip.chip = CHIP_907XD_VA;

    /* Step 1: Enable the address learning of all ports */
    CHK_FUN_RET(retVal, drv_memset(&lutMacLrnInfo, 0, sizeof(lutMacLrnInfo)));
    lutMacLrnInfo.portmask = (uint32)(DVA_RTK_USER_PMAP_ALL);
    lutMacLrnInfo.enable   = 1u;
    CHK_FUN_RET(retVal, rtk_lut_mac_lrn_en(unitChip, &lutMacLrnInfo));

    /* Step 2: Set/Get PORT_1ST legal port (PORT_2ND) and illegal ports (PORT_3RD to PORT_END) */
    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveLegalPortInfo, 0, sizeof(lutSaMoveLegalPortInfo)));
    lutSaMoveLegalPortInfo.port     = DVA_RTK_USER_PORT_1ST;
    lutSaMoveLegalPortInfo.portmask = (uint32)(DVA_RTK_USER_PMAP_2ND);
    CHK_FUN_RET(retVal, rtk_lut_sa_move_legal_port_set(unitChip, &lutSaMoveLegalPortInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveLegalPortInfo, 0, sizeof(lutSaMoveLegalPortInfo)));
    lutSaMoveLegalPortInfo.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_lut_sa_move_legal_port_get(unitChip, &lutSaMoveLegalPortInfo));

    /* Step 3: Set/Get PORT_1ST legal port action (forward and copy to CPU ports) and illegal action (drop) */
    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveLegalPortActInfo, 0, sizeof(lutSaMoveLegalPortActInfo)));
    lutSaMoveLegalPortActInfo.port         = DVA_RTK_USER_PORT_1ST;
    lutSaMoveLegalPortActInfo.legalPortAct = EM_LEGAL_FWD_N_COPY_CPU;
    CHK_FUN_RET(retVal, rtk_lut_sa_move_legal_port_set_act(unitChip, &lutSaMoveLegalPortActInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveLegalPortActInfo, 0, sizeof(lutSaMoveLegalPortActInfo)));
    lutSaMoveLegalPortActInfo.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_lut_sa_move_legal_port_get_act(unitChip, &lutSaMoveLegalPortActInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveIllegalPortActInfo, 0, sizeof(lutSaMoveIllegalPortActInfo)));
    lutSaMoveIllegalPortActInfo.port           = DVA_RTK_USER_PORT_1ST;
    lutSaMoveIllegalPortActInfo.illegalPortAct = EM_ILLEGAL_DROP;
    CHK_FUN_RET(retVal, rtk_lut_sa_move_illegal_port_set_act(unitChip, &lutSaMoveIllegalPortActInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveIllegalPortActInfo, 0, sizeof(lutSaMoveIllegalPortActInfo)));
    lutSaMoveIllegalPortActInfo.port = DVA_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_lut_sa_move_illegal_port_get_act(unitChip, &lutSaMoveIllegalPortActInfo));

    /* Step 4: Set/Get SA move CPU ports (PORT_4TH) */
    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveCpuPortInfo, 0, sizeof(lutSaMoveCpuPortInfo)));
    lutSaMoveCpuPortInfo.portmask = (uint32)(DVA_RTK_USER_PMAP_4TH);
    CHK_FUN_RET(retVal, rtk_lut_sa_move_cpu_port_set(unitChip, &lutSaMoveCpuPortInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutSaMoveCpuPortInfo, 0, sizeof(lutSaMoveCpuPortInfo)));
    CHK_FUN_RET(retVal, rtk_lut_sa_move_cpu_port_get(unitChip, &lutSaMoveCpuPortInfo));

    /* Step 5: Enable PORT_1ST legal/illegal SA move record */
    lutLegalSaMoveRecordInfo.port   = DVA_RTK_USER_PORT_1ST;
    lutLegalSaMoveRecordInfo.enable = 1u;
    CHK_FUN_RET(retVal, rtk_lut_legal_sa_move_record_en(unitChip, &lutLegalSaMoveRecordInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutIllegalSaMoveRecordInfo, 0, sizeof(lutIllegalSaMoveRecordInfo)));
    lutIllegalSaMoveRecordInfo.port   = DVA_RTK_USER_PORT_1ST;
    lutIllegalSaMoveRecordInfo.enable = 1u;
    CHK_FUN_RET(retVal, rtk_lut_illegal_sa_move_record_en(unitChip, &lutIllegalSaMoveRecordInfo));

    /* Step 6: Get legal/illegal SA move record of index 0 */
    CHK_FUN_RET(retVal, drv_memset(&lutLegalSaMoveRecordGetInfo, 0, sizeof(lutLegalSaMoveRecordGetInfo)));
    lutLegalSaMoveRecordGetInfo.idx = 0u;
    CHK_FUN_RET(retVal, rtk_lut_legal_sa_move_get_record(unitChip, &lutLegalSaMoveRecordGetInfo));

    CHK_FUN_RET(retVal, drv_memset(&lutIllegalSaMoveRecordGetInfo, 0, sizeof(lutIllegalSaMoveRecordGetInfo)));
    lutIllegalSaMoveRecordGetInfo.idx = 0u;
    CHK_FUN_RET(retVal, rtk_lut_illegal_sa_move_get_record(unitChip, &lutIllegalSaMoveRecordGetInfo));

    /*
        Step 7: Set VLAN 6
        Member ports are PORT_1ST, PORT_2ND, PORT_3RD, and PORT_4TH.
        No untagged port.
        Forwarding based on VLAN.
    */
    vlanInfo.vid                      = 6u;
    vlanInfo.vlanEntry.mbrMsk         = (uint32)(DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD);
    vlanInfo.vlanEntry.utagMsk        = 0u;
    vlanInfo.vlanEntry.fid            = 0u;
    vlanInfo.vlanEntry.ulfid          = EM_VLAN_IVL;
    vlanInfo.vlanEntry.vbfwd          = EM_ALE_BASED_FWD;
    vlanInfo.vlanEntry.interfaceId    = 0u;
    vlanInfo.vlanEntry.interfaceValid = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_set(unitChip, &vlanInfo));

    /* Step 8: Disable PORT_4TH (CPU port) egress filter, so PORT_4TH (CPU port) can receive copy packets */
    vlanPortEgrFilterInfo.port      = DVA_RTK_USER_PORT_4TH;
    vlanPortEgrFilterInfo.egrFilter = 0u;
    CHK_FUN_RET(retVal, rtk_vlan_port_egr_filter_en_set(unitChip, &vlanPortEgrFilterInfo));

    /* Expected Behavior:

        1. Send the frame <SA, vid> = <82:34:56:78:9A:BD, 6> to PORT_1ST.
            a. The source MAC will be learned by PORT_1ST.
            b. The frame will be forwarded to PORT_2ND and PORT_3RD.
        2. Send the frame <Source MAC> = <82:34:56:78:9A:BD> to PORT_2ND (legal port).
            a. Because PORT_2ND is PORT_1ST's legal port, the source MAC will be learned by PORT_2ND.
            b. The frame will be forwarded to PORT_1ST and PORT_3RD, and copied to PORT_4TH.
            c. It will be written to SA move legal record registers.
        3. Send the frame <Source MAC> = <82:34:56:78:9A:BD> to PORT_3RD (illegal port).
            a. Because PORT_3RD is PORT_1ST's illegal port, the source MAC will NOT be learned by PORT_3RD.
            b. The frame will be dropped.
            c. It will be written to SA move illegal record registers.
    */

    return retVal;
}
/**@}*/ /* dva_l2_sa_move */
/**@}*/ /* RTL907XDVA_L2_EXAMPLE */
/**@}*/ /* L2_EXAMPLE */
/**@}*/ /* EXAMPLE */