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
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtkas_api_qos.h"
#include "rtl907xd_v1_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */
/**
 * @addtogroup QOS_EXAMPLE QoS Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_QOS_EXAMPLE RTL907XD_V1 QoS Example
 * @{
 */
/**
 * @defgroup RTL907XD_V1_QOS_RMK_DSCP_EXAMPLE QoS Remarking Dscp Example
 * @brief <b>QoS Remarking Dscp</b>
 *
 * @section Purpose
 *   Optimize traffic treatment by reclassifying DSCP values.
 *
 * @section Description
 *   Modify DSCP field to enforce QoS policies.\n
 *   As shown in figure below, the switch received a IP packet with DSCP 20 at port 3, and then remark the DSCP to 36 at outgoing of port 1.
 *
 * @image html qos/rmk_dscp.jpg
 *
 * @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_qos_dscp_remarking(void);

/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_qos_dscp_remarking(void)
{
    RtkApiRet         retVal;
    UnitChip_t        unitChip;
    QosPriSelInfo_t   wQosPriSelInfo;
    QosPriSelInfo_t   rQosPriSelInfo;
    QosPriWtTbl_t     wQosPriWtTbl;
    QosPriWtTbl_t     rQosPriWtTbl;
    QosDscpPriRemap_t wQosDscpPriRemap;
    QosDscpPriRemap_t rQosDscpPriRemap;
    QosDscpRemapTbl_t wQosDscpRemapTbl;
    QosDscpRemapTbl_t rQosDscpRemapTbl;
    QosDscpRmkEn_t    wQosDscpRmkEn;
    QosDscpRmkEn_t    rQosDscpRmkEn;
    QosDscpRmk_t      wQosDscpRmk;
    QosDscpRmk_t      rQosDscpRmk;
    QosDscpRmkTbl_t   wQosDscpRmkTbl;
    QosDscpRmkTbl_t   rQosDscpRmkTbl;

    /* Initialize struct */
    unitChip.unit = DV1_UNIT;
    unitChip.chip = CHIP_907XD_V1;
    retVal        = drv_memset(&wQosPriSelInfo, 0, sizeof(wQosPriSelInfo));
    CHK_FUN_RET(retVal, drv_memset(&rQosPriSelInfo, 0, sizeof(rQosPriSelInfo)));
    CHK_FUN_RET(retVal, drv_memset(&wQosPriWtTbl, 0, sizeof(wQosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosPriWtTbl, 0, sizeof(rQosPriWtTbl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosDscpPriRemap, 0, sizeof(wQosDscpPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&rQosDscpPriRemap, 0, sizeof(rQosDscpPriRemap)));
    CHK_FUN_RET(retVal, drv_memset(&wQosDscpRemapTbl, 0, sizeof(wQosDscpRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosDscpRemapTbl, 0, sizeof(rQosDscpRemapTbl)));
    CHK_FUN_RET(retVal, drv_memset(&wQosDscpRmkEn, 0, sizeof(wQosDscpRmkEn)));
    CHK_FUN_RET(retVal, drv_memset(&rQosDscpRmkEn, 0, sizeof(rQosDscpRmkEn)));
    CHK_FUN_RET(retVal, drv_memset(&wQosDscpRmk, 0, sizeof(wQosDscpRmk)));
    CHK_FUN_RET(retVal, drv_memset(&rQosDscpRmk, 0, sizeof(rQosDscpRmk)));
    CHK_FUN_RET(retVal, drv_memset(&wQosDscpRmkTbl, 0, sizeof(wQosDscpRmkTbl)));
    CHK_FUN_RET(retVal, drv_memset(&rQosDscpRmkTbl, 0, sizeof(rQosDscpRmkTbl)));

    /* Step 1. Set priority extraction weight table 0 */
    wQosPriSelInfo.group             = 0u;
    wQosPriSelInfo.qosPriSel.dscpPri = 7u;
    rQosPriSelInfo.group             = 0u;
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_set(unitChip, &wQosPriSelInfo));
    CHK_FUN_RET(retVal, rtk_qos_pri_sel_get(unitChip, &rQosPriSelInfo));
    /* Expected Behaviour:  In priority extraction weight table 0, only accept dscp priority as source priority */

    /* Step 2. Set port 3 (ingress port) to use table 0 */
    wQosPriWtTbl.port = DV1_RTK_USER_PORT_3RD;
    wQosPriWtTbl.idx  = 0;
    rQosPriWtTbl.port = DV1_RTK_USER_PORT_3RD;
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_set(unitChip, &wQosPriWtTbl));
    CHK_FUN_RET(retVal, rtk_qos_pri_wt_tbl_get(unitChip, &rQosPriWtTbl));
    /* Expected Behaviour: select priority extraction weight table 0 on port 3 (ingress port). */

    /* Step 3.1. Set DSCP remapping table 0. DSCP 20 mapping to internal priority 2 */
    wQosDscpPriRemap.idx        = 0u;
    wQosDscpPriRemap.dscp       = 20u;
    wQosDscpPriRemap.mappingPri = 2u;
    rQosDscpPriRemap.idx        = 0u;
    rQosDscpPriRemap.dscp       = 20u;
    CHK_FUN_RET(retVal, rtk_qos_dscp_pri_remap_set(unitChip, &wQosDscpPriRemap));
    CHK_FUN_RET(retVal, rtk_qos_dscp_pri_remap_get(unitChip, &rQosDscpPriRemap));

    /* Step 3.2. Using DSCP remapping table 0 on port 3 */
    wQosDscpRemapTbl.port = DV1_RTK_USER_PORT_3RD;
    wQosDscpRemapTbl.idx  = 0u;
    rQosDscpRemapTbl.port = DV1_RTK_USER_PORT_3RD;
    CHK_FUN_RET(retVal, rtk_qos_dscp_remap_tbl_set(unitChip, &wQosDscpRemapTbl));
    CHK_FUN_RET(retVal, rtk_qos_dscp_remap_tbl_get(unitChip, &rQosDscpRemapTbl));

    /* Step 3.3. Enable DSCP remarking for port 1 */
    wQosDscpRmkEn.port = DV1_RTK_USER_PORT_1ST;
    wQosDscpRmkEn.en   = ENABLED;
    rQosDscpRmkEn.port = DV1_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_qos_dscp_rmk_en_set(unitChip, &wQosDscpRmkEn));
    CHK_FUN_RET(retVal, rtk_qos_dscp_rmk_en_get(unitChip, &rQosDscpRmkEn));

    /* Step 3.4. Set DSCP remarking table 0, internal priority 2 remark to DSCP 36 */
    wQosDscpRmk.idx     = 0u;
    wQosDscpRmk.oriPri  = 2u;
    wQosDscpRmk.rmkDscp = 36u;
    rQosDscpRmk.idx     = 0u;
    rQosDscpRmk.oriPri  = 2u;
    CHK_FUN_RET(retVal, rtk_qos_dscp_rmk_set(unitChip, &wQosDscpRmk));
    CHK_FUN_RET(retVal, rtk_qos_dscp_rmk_get(unitChip, &rQosDscpRmk));

    /* Step 3.5. Select DSCP remarking table 0 on port 1 */
    wQosDscpRmkTbl.port = DV1_RTK_USER_PORT_1ST;
    wQosDscpRmkTbl.idx  = 0u;
    rQosDscpRmkTbl.port = DV1_RTK_USER_PORT_1ST;
    CHK_FUN_RET(retVal, rtk_qos_dscp_rmk_tbl_set(unitChip, &wQosDscpRmkTbl));
    CHK_FUN_RET(retVal, rtk_qos_dscp_rmk_tbl_get(unitChip, &rQosDscpRmkTbl));
    /* Expected Behaviour: When received a packet with DSCP 20 at port 3, and then remark the DSCP to 36 at outgoing of port 1. */

    return retVal;
}
/**@}*/ /* RTL907XD_V1_QOS_RMK_DSCP_EXAMPLE */
/**@}*/ /* RTL907XD_V1_QOS_EXAMPLE */
/**@}*/ /* QOS_EXAMPLE */
/**@}*/ /* EXAMPLE */