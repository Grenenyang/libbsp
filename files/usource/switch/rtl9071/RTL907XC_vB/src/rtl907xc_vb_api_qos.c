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

#include "rtkas_types.h"
#include "rtkas_api_qos.h"
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_asicdrv.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#include "rtl907xc_vb_asicdrv_qos.h"

/**
 * @defgroup RTK_MODULE_QOS QOS
 * @brief QoS APIs
 * @{
 */

/**
 * @addtogroup QOS_IGR_BW Ingress Bandwidth Control
 * @{
 */

/**
 * @defgroup QOS_IGR_BW_ASIC Low Level Driver
 * @brief QoS Low Level Driver
 * @{
 */

/**
 * @brief Set port ingress bandwidth and flow control ability.
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Ingress port number
 * @param[in] pIgrBwCtrl Ingress bandwidth control (N/A) \n
 *                       The struct includes bwCtrlEn0, bwCtrlEn1, igrBw0, igrBw1, flowCtrlEn0 and flowCtrlEn1. \n
 *                       The bwCtrlEn0 represents the ingress bandwidth control capability of LB0. \n
 *                       The bwCtrlEn1 represents the ingress bandwidth control capability of LB1. \n
 *                       The igrBw0 represents the bandwidth of LB0 in units of 8 Kbps. \n
 *                       The igrBw1 represents the bandwidth of LB1 in units of 8 Kbps. \n
 *                       The flowCtrlEn0 represents the flow control ability of LB0. \n
 *                       The flowCtrlEn1 represents flow control ability of LB1.
 * @param[out] None
 * @retval RT_ERR_OK     The API was executed correctly.
 * @retval Others        Please refer to cvb_drv_qos_igr_bw_ctrl_set_ret_chk(), cvb_reg_field_write(), cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_igr_bw_ctrl_set(uint32 unit, cvb_RtkPort port, const QosIgrBwCtrl_t *pIgrBwCtrl)
{
    RtkApiRet retVal;

    /* Check the input parameters */
    retVal = cvb_drv_qos_igr_bw_ctrl_set_ret_chk(unit, port, pIgrBwCtrl);

    /* Ingress bandwidth includes the preamble and IFG (20 Bytes) */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_INGRESS_BANDWIDTH_CONTROL_DROP_THRESHOLD_GLOBAL_CONTROL,
                                            CVB_INGRESS_BANDWIDTH_CONTROL_DROP_THRESHOLD_GLOBAL_CONTROL_INBW_INC_IFG_LSP,
                                            CVB_INGRESS_BANDWIDTH_CONTROL_DROP_THRESHOLD_GLOBAL_CONTROL_INBW_INC_IFG_LEN,
                                            (uint32)0x1u));
    /* Set the ability of ingress bandwidth control 0 */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_EN_LSP,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_EN_LEN,
                                                       pIgrBwCtrl->bwCtrlEn0));
    /* Set the ability of ingress bandwidth control 1 */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_EN_LSP,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_EN_LEN,
                                                       pIgrBwCtrl->bwCtrlEn1));
    /* Set the bandwidth of the leaky bucket 0 */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_RATE_LSP,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_RATE_LEN,
                                                       pIgrBwCtrl->igrBw0));
    /* Set the bandwidth of the leaky bucket 1 */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_RATE_LSP,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_RATE_LEN,
                                                       pIgrBwCtrl->igrBw1));
    /* Set the flow control ability 0 */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_FC_EN_LSP,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_FC_EN_LEN,
                                                       pIgrBwCtrl->flowCtrlEn0));
    /* Set the flow control ability 1 */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_FC_EN_LSP,
                                                       CVB_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_FC_EN_LEN,
                                                       pIgrBwCtrl->flowCtrlEn1));

    return retVal;
}

/**
 * @brief Get port ingress bandwidth and flow control ability.
 * @param[in] unit        Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port        Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                        Ingress port number
 * @param[out] pIgrBwCtrl Ingress bandwidth control (N/A) \n
 *                        The struct includes bwCtrlEn0, bwCtrlEn1, igrBw0, igrBw1, flowCtrlEn0 and flowCtrlEn1. \n
 *                        The bwCtrlEn0 represents the ingress bandwidth control capability of LB0. \n
 *                        The bwCtrlEn1 represents the ingress bandwidth control capability of LB1. \n
 *                        The igrBw0 represents the bandwidth of LB0 in units of 8 Kbps. \n
 *                        The igrBw1 represents the bandwidth of LB1 in units of 8 Kbps. \n
 *                        The flowCtrlEn0 represents the flow control ability of LB0. \n
 *                        The flowCtrlEn1 represents flow control ability of LB1.
 * @retval RT_ERR_OK      The API was executed correctly.
 * @retval Others         Please refer to cvb_drv_qos_igr_bw_ctrl_get_ret_chk(), cvb_drv_qos_igr_bw_ctrl_get_0(), cvb_drv_qos_igr_bw_ctrl_get_1().
 */
RtkApiRet cvb_qos_igr_bw_ctrl_get(uint32 unit, cvb_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check the input parameters */
    CHK_FUN_RET(retVal, cvb_drv_qos_igr_bw_ctrl_get_ret_chk(unit, port, pIgrBwCtrl));

    /* Get the ability of bandwidth control and the bandwidth of the leaky bucket */
    CHK_FUN_RET(retVal, cvb_drv_qos_igr_bw_ctrl_get_0(unit, port, pIgrBwCtrl));

    /* Get the ability of flow control */
    CHK_FUN_RET(retVal, cvb_drv_qos_igr_bw_ctrl_get_1(unit, port, pIgrBwCtrl));

    return retVal;
}
/**@}*/ /* QOS_IGR_BW_ASIC */
/**@}*/ /* QOS_IGR_BW */

/**
 * @addtogroup QOS_TC Traffic Classification
 * @{
 */

/**
 * @defgroup QOS_TC_ASIC Low Level Driver
 * @brief QoS Low Level Driver
 * @{
 */

/**
 * @brief Set the internal priority for port-based priority source.
 * @param[in] unit				   Stacking unit (1 to 7) \n
 *								   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port				   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *								   Ingress port number
 * @param[in] pri				   Priority (0 to QOS_PRI_MAX) \n
 *								   Internal priority
 * @param[out] None
 * @retval RT_ERR_OK			   The internal priority is set successfully.
 * @retval -RT_ERR_PORT_ID		   The Port number is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY The internal priority is invalid.
 * @retval Others				   Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_port_pri_set(uint32 unit, cvb_RtkPort port, cvb_QosPri pri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(pri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else
    {
        /* do nothing */
    }

    /* Set the internal priority for port-based priority source. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_PORTPRIORITY_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_PORTPRIORITY_LEN,
                                                       pri));

    return retVal;
}

/**
 * @brief Get the internal priority for port-based priority source.
 * @param[in] unit              Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                              Ingress port number
 * @param[out] pPri             Priority (0 to QOS_PRI_MAX) \n
 *                              Internal priority
 * @retval RT_ERR_OK            The internal priority is got successfully.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_NULL_POINTER The internal priority is NULL.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_qos_port_pri_get(uint32 unit, cvb_RtkPort port, cvb_QosPri *pPri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pPri)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the internal priority for port-based priority source. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_PORTPRIORITY_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_PORTPRIORITY_LEN,
                                                       pPri));

    return retVal;
}

/**
 * @brief Set a weight for each priority source.
 * @param[in] unit                    Stacking unit (1 to 7) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group                   Group index (0 to QOS_TBL_IDX_MAX) \n
 *                                    The arbitration index is known as the priority weight group table index.
 * @param[in] pPriDec                 Priority decision (N/A) \n
 *                                    The struct incldues portPri, hybridPri, dscpPri, cvlanPri and svlanPri. \n
 *                                    The portPri is the weight of port priority, the hybridPri is the weight of hybrid priority,
 *                                    the dscpPri is the weight of DSCP priority, the cvlanPri is the weight of CVLAN priority,
 *                                    and the svlanPri is the weight of SVLAN priority.
 * @param[out] None
 * @retval RT_ERR_OK                  The all priority weights are set successfully.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP  The group index is invalid.
 * @retval -RT_ERR_QOS_SEL_PRI_SOURCE The port/hybrid/dscp/cvlan/svlan priority is invalid.
 * @retval Others                     Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_pri_sel_set(uint32 unit, cvb_QosTblIdx group, const QosPriSel_t *pPriDec)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(group > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if((pPriDec->portPri > QOS_PRI_MAX) || (pPriDec->hybridPri > QOS_PRI_MAX) || (pPriDec->dscpPri > QOS_PRI_MAX) || (pPriDec->cvlanPri > QOS_PRI_MAX) || (pPriDec->svlanPri > QOS_PRI_MAX))
    {
        retVal = -RT_ERR_QOS_SEL_PRI_SOURCE;
    }
    else
    {
        /* do nothing */
    }

    /* Set a weight for each priority source. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0 + (group * CVB_QOS_GROUP_OFFSET),
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_OTAG_PRIO_LSP,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_OTAG_PRIO_LEN,
                                                       pPriDec->svlanPri));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0 + (group * CVB_QOS_GROUP_OFFSET),
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_ITAG_PRIO_LSP,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_ITAG_PRIO_LEN,
                                                       pPriDec->cvlanPri));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0 + (group * CVB_QOS_GROUP_OFFSET),
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_DSCP_PRIO_LSP,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_DSCP_PRIO_LEN,
                                                       pPriDec->dscpPri));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0 + (group * CVB_QOS_GROUP_OFFSET),
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_DOT1QBP_PRIO_LSP,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_DOT1QBP_PRIO_LEN,
                                                       pPriDec->hybridPri));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0 + (group * CVB_QOS_GROUP_OFFSET),
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_PBP_PRIO_LSP,
                                                       CVB_PRIORITY_ARBITRATION_CONTROL0_PBP_PRIO_LEN,
                                                       pPriDec->portPri));

    return retVal;
}

/**
 * @brief Get weights of priority sources.
 * @param[in] unit                    Stacking unit (1 to 7) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group                   Group index (0 to QOS_TBL_IDX_MAX) \n
 *                                    The arbitration index is known as the priority weight group table index.
 * @param[out] pPriDec                Priority decision (N/A) \n
 *                                    The struct incldues portPri, hybridPri, dscpPri, cvlanPri and svlanPri. \n
 *                                    The portPri is the weight of port priority, the hybridPri is the weight of hybrid priority,
 *                                    the dscpPri is the weight of DSCP priority, the cvlanPri is the weight of CVLAN priority,
 *                                    and the svlanPri is the weight of SVLAN priority.
 * @retval RT_ERR_OK                  The all priority weights are got successfully.
 * @retval -RT_ERR_NULL_POINTER       The priority weight description is NULL.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP  The group index is invalid.
 * @retval Others                     Please refer to cvb_reg_field_read() and cvb_reg_field_get().
 */
RtkApiRet cvb_qos_pri_sel_get(uint32 unit, cvb_QosTblIdx group, QosPriSel_t *pPriDec)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regAddr;
    uint32    regVal;

    if(group > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(NULL == pPriDec)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        regAddr = CVB_PRIORITY_ARBITRATION_CONTROL0 + (group * CVB_QOS_GROUP_OFFSET);
    }

    /* Get a weight for each priority source. */
    CHK_FUN_RET(retVal, cvb_reg_read(unit, regAddr, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(regAddr, CVB_PRIORITY_ARBITRATION_CONTROL0_OTAG_PRIO_LSP, CVB_PRIORITY_ARBITRATION_CONTROL0_OTAG_PRIO_LEN, &pPriDec->svlanPri, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(regAddr, CVB_PRIORITY_ARBITRATION_CONTROL0_ITAG_PRIO_LSP, CVB_PRIORITY_ARBITRATION_CONTROL0_ITAG_PRIO_LEN, &pPriDec->cvlanPri, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(regAddr, CVB_PRIORITY_ARBITRATION_CONTROL0_DSCP_PRIO_LSP, CVB_PRIORITY_ARBITRATION_CONTROL0_DSCP_PRIO_LEN, &pPriDec->dscpPri, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(regAddr, CVB_PRIORITY_ARBITRATION_CONTROL0_DOT1QBP_PRIO_LSP, CVB_PRIORITY_ARBITRATION_CONTROL0_DOT1QBP_PRIO_LEN, &pPriDec->hybridPri, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(regAddr, CVB_PRIORITY_ARBITRATION_CONTROL0_PBP_PRIO_LSP, CVB_PRIORITY_ARBITRATION_CONTROL0_PBP_PRIO_LEN, &pPriDec->portPri, regVal));

    return retVal;
}

/**
 * @brief Set a specific ingress port to use priority arbitration table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
                                Ingress port number
 * @param[in] idx               Arbitration table index (0 to QOS_TBL_IDX_MAX) \n
 *                              The arbitration index is known as the priority weight group table index.
 * @param[out] None
 * @retval RT_ERR_OK                 The priority weight table was set successfully
 * @retval -RT_ERR_PORT_ID           The Port number is invalid
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The arbitration table index is invalid
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set()
 */
RtkApiRet cvb_qos_pri_wt_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else
    {
        /* do nothing */
    }

    /* Set a specific port to use priority arbitration table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_PRIWGTTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_PRIWGTTBLIDX_LEN,
                                                       idx));

    return retVal;
}

/**
 * @brief Get which priority arbitration table is used for a specific ingress port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port               Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                               Ingress port number
 * @param[out] pIdx              Arbitration table index (0 to QOS_TBL_IDX_MAX) \n
 *                               The arbitration index is known as the priority weight group table index.
 * @retval RT_ERR_OK             The priority weight table was set successfully
 * @retval -RT_ERR_PORT_ID       The Port number is invalid
 * @retval -RT_ERR_NULL_POINTER  The arbitration table index is NULL
 * @retval Others                Please refer to cvb_drv_indirect_reg_field_get()
 */
RtkApiRet cvb_qos_pri_wt_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIdx)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get which priority arbitration table is used for a specific port. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_PRIWGTTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_PRIWGTTBLIDX_LEN,
                                                       pIdx));

    return retVal;
}

/**
 * @brief Set the inner tag remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx        Remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       Remapping table index for CVLAN
 * @param[in] iPri       Inner priority (0 to QOS_PRI_MAX) \n
 *                       Inner priority
 * @param[in] iDei       Inner dei (0 to QOS_DEI_MAX) \n
 *                       Inner dei
 * @param[in] mappingPri Mapping priority (0 to QOS_PRI_MAX) \n
 *                       The mapping priority value to replace the inner priority.
 * @param[out] None
 * @retval RT_ERR_OK                 The priority weight table was set successfully
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The priority remapping table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The priority is invalid.
 * @retval -RT_ERR_QOS_1P_DEI        The dei is invalid.
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set()
 */
RtkApiRet cvb_qos_cvlan_pri_remap_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri iPri, cvb_QosDei iDei, cvb_QosPri mappingPri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if((iPri > QOS_PRI_MAX) || (mappingPri > QOS_PRI_MAX))
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(iDei > QOS_DEI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_DEI;
    }
    else
    {
        /* do nothing */
    }

    /* Set the inner tag remapping table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_INNER_TAG_PRIORITY_MAP_CONTROL0 + (idx * CVB_QOS_QUE_SIZE) + (iDei * CVB_QOS_DEI_SIZE),
                                                       CVB_INNER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LSP + (iPri * CVB_QOS_PRI_SIZE),
                                                       CVB_INNER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LEN,
                                                       mappingPri));

    return retVal;
}

/**
 * @brief Get the inner tag remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx          Remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                         Remapping table index for CVLAN
 * @param[in] iPri         Inner priority (0 to QOS_PRI_MAX) \n
 *                         Inner priority
 * @param[in] iDei         Inner dei (0 to QOS_DEI_MAX) \n
 *                         Inner dei
 * @param[out] pMappingPri Mapping priority (0 to QOS_PRI_MAX) \n
 *                         The mapping priority value to replace the inner priority.
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The cvlan priority remapping table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The priority is invalid.
 * @retval -RT_ERR_QOS_1P_DEI        The dei is invalid.
 * @retval -RT_ERR_NULL_POINTER      The input parameter is NULL.
 * @retval Others                    Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_cvlan_pri_remap_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri iPri, cvb_QosDei iDei, cvb_QosPri *pMappingPri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(iPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(iDei > QOS_DEI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_DEI;
    }
    else if(NULL == pMappingPri)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the inner tag remapping table. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_INNER_TAG_PRIORITY_MAP_CONTROL0 + (idx * CVB_QOS_QUE_SIZE) + (iDei * CVB_QOS_DEI_OFFSET),
                                           CVB_INNER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LSP + (iPri * CVB_QOS_PRI_SIZE),
                                           CVB_INNER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LEN,
                                           pMappingPri));

    return retVal;
}

/**
 * @brief Set the outer tag remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx        Remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       Remapping table index for CVLAN
 * @param[in] oPri       Outer priority (0 to QOS_PRI_MAX) \n
 *                       Outer priority
 * @param[in] oDei       Outer dei (0 to QOS_DEI_MAX) \n
 *                       Outer dei
 * @param[in] mappingPri Mapping priority (0 to QOS_PRI_MAX) \n
 *                       The mapping priority value to replace the outer priority.
 * @param[out] None
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The priority remapping table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The priority is invalid.
 * @retval -RT_ERR_QOS_1P_DEI        The dei is invalid.
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_svlan_pri_remap_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oPri, cvb_QosDei oDei, cvb_QosPri mappingPri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if((oPri > QOS_PRI_MAX) || (mappingPri > QOS_PRI_MAX))
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(oDei > QOS_DEI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_DEI;
    }
    else
    {
        /* do nothing */
    }

    /* Set the outer tag remapping table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_OUTER_TAG_PRIORITY_MAP_CONTROL0 + (idx * CVB_QOS_QUE_SIZE) + (oDei * CVB_QOS_DEI_OFFSET),
                                                       CVB_OUTER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LSP + (oPri * CVB_QOS_PRI_SIZE),
                                                       CVB_OUTER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LEN,
                                                       mappingPri));

    return retVal;
}

/**
 * @brief Get the outer tag remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx          Remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                         Remapping table index for CVLAN
 * @param[in] oPri         Outer priority (0 to QOS_PRI_MAX) \n
 *                         Outer priority
 * @param[in] oDei         Outer dei (0 to QOS_DEI_MAX) \n
 *                         Outer dei
 * @param[out] pMappingPri Mapping priority (0 to QOS_PRI_MAX) \n
 *                         The mapping priority value to replace the outer priority.
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The priority remapping table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The priority is invalid.
 * @retval -RT_ERR_QOS_1P_DEI        The dei is invalid.
 * @retval -RT_ERR_NULL_POINTER      The input parameter is NULL.
 * @retval Others                    Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_svlan_pri_remap_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oPri, cvb_QosDei oDei, cvb_QosPri *pMappingPri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(oPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(oDei > QOS_DEI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_DEI;
    }
    else if(NULL == pMappingPri)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the outer tag remapping table. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_OUTER_TAG_PRIORITY_MAP_CONTROL0 + (idx * CVB_QOS_QUE_SIZE) + (oDei * CVB_QOS_DEI_OFFSET),
                                           CVB_OUTER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LSP + (oPri * CVB_QOS_PRI_SIZE),
                                           CVB_OUTER_TAG_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LEN,
                                           pMappingPri));

    return retVal;
}

/**
 * @brief Set the DSCP remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx        Remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       Remapping table index for CVLAN
 * @param[in] dscp       Differentiated Service Code Point (0 to QOS_DSCP_MAX) \n
 *                       Differentiated Service Code Point
 * @param[in] mappingPri Mapping priority (0 to QOS_PRI_MAX) \n
 *                       The mapping priority value to replace the DSCP value.
 * @param[out] None
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The priority remapping table index is invalid.
 * @retval -RT_ERR_QOS_DSCP_VALUE    The DSCP value is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The priority is invalid.
 * @retval Others                    Please refer to cvb_reg_field_write().
 */
RtkApiRet cvb_qos_dscp_pri_remap_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosDscp dscp, cvb_QosPri mappingPri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(dscp > QOS_DSCP_MAX)
    {
        retVal = -RT_ERR_QOS_DSCP_VALUE;
    }
    else if(mappingPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else
    {
        /* do nothing */
    }

    /* Set the DSCP remapping table. */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_DSCP_PRIORITY_MAP_CONTROL0 + (idx * CVB_QOS_DSCP_RMK_OFFSET) + ((dscp / CVB_QOS_DSCP_SIZE) * CVB_QOS_DSCP_OFFSET),
                                            CVB_DSCP_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LSP + ((dscp % CVB_QOS_DSCP_SIZE) * CVB_QOS_DSCP_OFFSET),
                                            CVB_DSCP_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LEN,
                                            mappingPri));

    return retVal;
}

/**
 * @brief Get the DSCP remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx          Remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                         Remapping table index for CVLAN
 * @param[in] dscp         Differentiated Service Code Point (0 to QOS_DSCP_MAX) \n
 *                         Differentiated Service Code Point
 * @param[out] pMappingPri Mapping priority (0 to QOS_PRI_MAX) \n
 *                         The mapping priority value to replace the DSCP value.
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The table indiex invalid.
 * @retval -RT_ERR_QOS_DSCP_VALUE    The dspc value is invalid.
 * @retval -RT_ERR_NULL_POINTER      The input parameter is NULL.
 * @retval Others                    Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_dscp_pri_remap_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosDscp dscp, cvb_QosPri *pMappingPri)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(dscp > QOS_DSCP_MAX)
    {
        retVal = -RT_ERR_QOS_DSCP_VALUE;
    }
    else if(NULL == pMappingPri)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the DSCP remapping table. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_DSCP_PRIORITY_MAP_CONTROL0 + (idx * CVB_QOS_DSCP_RMK_OFFSET) + ((dscp / CVB_QOS_DSCP_SIZE) * CVB_QOS_DSCP_OFFSET),
                                           CVB_DSCP_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LSP + ((dscp % CVB_QOS_DSCP_SIZE) * CVB_QOS_DSCP_OFFSET),
                                           CVB_DSCP_PRIORITY_MAP_CONTROL0_PRIORITYMAPTO0_LEN,
                                           pMappingPri));

    return retVal;
}

/**
 * @brief Set a specific port to use the inner tag remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Ingress port number
 * @param[in] idx        CVLAN remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       CVLAN remapping table index
 * @param[out] None
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_PORT_ID           The Port number is invalid.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The cvlan remapping table is invalid.
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_cvlan_remap_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else
    {
        /* do nothing */
    }

    /* Set a specific port to use inner tag remapping table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_IPRIDPTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_IPRIDPTBLIDX_LEN,
                                                       idx));

    return retVal;
}

/**
 * @brief Get which inner tag remapping table is used for a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Ingress port number
 * @param[out] pIdx      CVLAN remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       CVLAN remapping table index
 * @retval RT_ERR_OK            The API was executed correctly.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_qos_cvlan_remap_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIdx)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get which inner tag remapping table is used for a specific port. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_IPRIDPTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_IPRIDPTBLIDX_LEN,
                                                       pIdx));

    return retVal;
}

/**
 * @brief Set a specific port to use the outer tag remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Ingress port number
 * @param[in] idx        SVLAN remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       SVLAN remapping table index
 * @param[out] None
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_PORT_ID           The Port number is invalid
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The remapping table is invalid.
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_svlan_remap_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else
    {
        /* do nothing */
    }

    /* Set a specific port to use outer tag remapping table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_OPRIDPTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_OPRIDPTBLIDX_LEN,
                                                       idx));

    return retVal;
}

/**
 * @brief Get which SVLAN remapping table is used for a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Ingress port number
 * @param[out] pIdx      SVLAN remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       SVLAN remapping table index
 * @retval RT_ERR_OK            The API was executed correctly.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_qos_svlan_remap_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIdx)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get which outer tag remapping table is used for a specific port. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_OPRIDPTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_OPRIDPTBLIDX_LEN,
                                                       pIdx));

    return retVal;
}

/**
 * @brief Set a specific port to use the DSCP remapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Ingress port number
 * @param[in] idx        DSCP remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       DSCP remapping table index
 * @param[out] None
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_PORT_ID           The Port number is invalid
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The dscp remapping table is invalid.
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_dscp_remap_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else
    {
        /* do nothing */
    }

    /* Set a specific port to use DSCP remapping table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_DSCPPRIDPTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_DSCPPRIDPTBLIDX_LEN,
                                                       idx));

    return retVal;
}

/**
 * @brief Get which DSCP remapping table is used for a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Ingress port number
 * @param[out] pIdx      DSCP remapping table index (0 to QOS_TBL_IDX_MAX) \n
 *                       DSCP remapping table index
 * @retval RT_ERR_OK            The API was executed correctly.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_qos_dscp_remap_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIdx)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get which DSCP remapping table is used for a specific port. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                       CVB_PORT0_PRIORITY_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_CONTROL_DSCPPRIDPTBLIDX_LSP,
                                                       CVB_PORT0_PRIORITY_CONTROL_DSCPPRIDPTBLIDX_LEN,
                                                       pIdx));

    return retVal;
}

/**
 * @brief Set the internal-priority-to-traffic-class mapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] pri       Priority (0 to QOS_PRI_MAX) \n
 *                      Priority value
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[out] None
 * @retval RT_ERR_OK               The API was executed correctly.
 * @retval -RT_ERR_PORT_ID         The Port number is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY The priority is invalid.
 * @retval -RT_ERR_QUEUE_ID        The queue is invalid.
 * @retval Others                  Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_pri_que_mapping_set(uint32 unit, cvb_RtkPort port, cvb_QosPri pri, cvb_QosQueNum que)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(pri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else
    {
        /* do nothing */
    }

    /* Set the internal-priority-to-traffic-class mapping table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_MAP_TO_QUEUENUMBER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_MAP_TO_QUEUENUMBER_CONTROL_PRI0QNUM_LSP + (pri * CVB_QOS_PRI_SIZE),
                                                       CVB_PORT0_PRIORITY_MAP_TO_QUEUENUMBER_CONTROL_PRI0QNUM_LEN,
                                                       que));

    return retVal;
}

/**
 * @brief Get the internal-priority-to-traffic-class mapping table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] pri       Priority (0 to QOS_PRI_MAX) \n
 *                      Priority value
 * @param[out] pQue     Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @retval RT_ERR_OK               The API was executed correctly.
 * @retval -RT_ERR_PORT_ID         The Port number is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY The priority is invalid.
 * @retval -RT_ERR_NULL_POINTER    The input parameter is NULL.
 * @retval Others                  Please refer to cvb_drv_indirect_reg_field_get().
 * @note This API can get port priority to queue mapping table.
 */
RtkApiRet cvb_qos_pri_que_mapping_get(uint32 unit, cvb_RtkPort port, cvb_QosPri pri, cvb_QosQueNum *pQue)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(pri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(NULL == pQue)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the internal-priority-to-traffic-class mapping table. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                       CVB_PORT0_PRIORITY_MAP_TO_QUEUENUMBER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_MAP_TO_QUEUENUMBER_CONTROL_PRI0QNUM_LSP + (pri * CVB_QOS_PRI_SIZE),
                                                       CVB_PORT0_PRIORITY_MAP_TO_QUEUENUMBER_CONTROL_PRI0QNUM_LEN,
                                                       pQue));

    return retVal;
}
/**@}*/ /* QOS_TC_ASIC */
/**@}*/ /* QOS_TC */

/**
 * @addtogroup QOS_TS Traffic Scheduling
 * @{
 */

/**
 * @defgroup QOS_TS_ASIC Low Level Driver
 * @brief QoS Low Level Driver
 * @{
 */

/**
 * @brief Set a traffic scheduler for a specific queue of a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *			  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port		Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *						Egress port number
 * @param[in] que		Queue (0 to QOS_QUE_MAX) \n
 *						Queue number
 * @param[in] queSch	Queue scheduling (<CHIPID>_QOS_SCH_SP to <CHIPID>_QOS_SCH_END) \n
 *						Queue scheduling algorithm
 * @param[out] None
 * @retval RT_ERR_OK	The API was executed correctly.
 * @retval Others		Please refer to cvb_drv_qos_que_sch_set_ret_chk(), cvb_drv_qos_que_sch_set_info_set() and cvb_reg_field_write().
 * @note For a port, WFQ/WRR/CBS can not exist at the same time.
 */
RtkApiRet cvb_qos_que_sch_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, RtkQoSSch queSch)
{
    RtkApiRet retVal;

    /* Check the input parameters */
    retVal = cvb_drv_qos_que_sch_set_ret_chk(unit, port, que, queSch);

    /* Set a traffic scheduler for a specific queue of a specific port */
    CHK_FUN_RET(retVal, cvb_drv_qos_que_sch_set_info_set(unit, port, que, queSch));

    /* Egress bandwidth includes the preamble and IFG (20 bytes) */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_PORT_LEAKY_BUCKET_CONTROL,
                                            CVB_PORT_LEAKY_BUCKET_CONTROL_PKTLENWITHPIFG_LSP,
                                            CVB_PORT_LEAKY_BUCKET_CONTROL_PKTLENWITHPIFG_LEN,
                                            ENABLE));

    return retVal;
}

/**
 * @brief Get the traffic scheduler for a specific queue of a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[out] pQueSch  Queue scheduling (<CHIPID>_QOS_SCH_SP to <CHIPID>_QOS_SCH_END) \n
 *                      Queue scheduling algorithm
 * @retval RT_ERR_OK    The API was executed correctly.
 * @retval Others       Please refer to cvb_drv_qos_que_sch_get_ret_chk() and cvb_drv_qos_que_sch_get_info_get().
 */
RtkApiRet cvb_qos_que_sch_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, RtkQoSSch *pQueSch)
{
    RtkApiRet retVal;

    /* Check the input parameters */
    retVal = cvb_drv_qos_que_sch_get_ret_chk(unit, port, que, pQueSch);

    /* Get the traffic scheduler for a specific queue of a specific port */
    CHK_FUN_RET(retVal, cvb_drv_qos_que_sch_get_info_get(unit, port, que, pQueSch));

    return retVal;
}

/**
 * @brief Set a weight value of WRR/WFQ scheduler for a specific queue of a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[in] queWt     Queue weight (0 to QOS_WT_MAX) \n
 *                      Queue weight
 * @param[out] None
 * @retval RT_ERR_OK                The API was executed correctly.
 * @retval -RT_ERR_PORT_ID          The Port number is invalid.
 * @retval -RT_ERR_QUEUE_ID         The queue is invalid.
 * @retval -RT_ERR_QOS_QUEUE_WEIGHT The weight is invalid.
 * @retval Others                   Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_que_wfq_wrr_wt_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_QosWt queWt)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(queWt > QOS_WT_MAX)
    {
        retVal = -RT_ERR_QOS_QUEUE_WEIGHT;
    }
    else
    {
        /* do nothing */
    }

    /* Set a weight value of WRR/WFQ scheduler for a specific queue of a specific port. */
    if(que >= CVB_QOS_QUE_BASE)
    {
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + ((que - CVB_QOS_QUE_BASE) * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LSP,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LEN,
                                                           queWt));
    }
    else
    {
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + (que * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LSP,
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LEN,
                                                           queWt));
    }

    return retVal;
}

/**
 * @brief Get the weight value of WRR/WFQ scheduler for a specific queue of a specific egress port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[out] pQueWt   Queue weight (0 to QOS_WT_MAX) \n
 *                      Queue weight
 * @retval RT_ERR_OK                The API was executed correctly.
 * @retval -RT_ERR_PORT_ID          The Port number is invalid.
 * @retval -RT_ERR_QUEUE_ID         The queue is invalid.
 * @retval -RT_ERR_NULL_POINTER     The input parameter is NULL.
 * @retval Others                   Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_qos_que_wfq_wrr_wt_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_QosWt *pQueWt)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(NULL == pQueWt)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the weight value of WRR/WFQ scheduler for a specific queue of a specific port. */
    if(que >= CVB_QOS_QUE_BASE)
    {
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + ((que - CVB_QOS_QUE_BASE) * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LSP,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LEN,
                                                           pQueWt));
    }
    else
    {
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + (que * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LSP,
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTWEIGHT_LEN,
                                                           pQueWt));
    }

    return retVal;
}
/**@}*/ /* QOS_TS_ASIC */
/**@}*/ /* QOS_TS */

/**
 * @addtogroup QOS_EGR_BW Egress Bandwidth Control
 * @{
 */

/**
 * @defgroup QOS_EGR_BW_ASIC Low Level Driver
 * @brief QoS Low Level Driver
 * @{
 */

/**
 * @brief Set a queue bandwidth for a specific queue of a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[in] en        Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *                      Enable value
 * @param[in] bw   Queue bandwidth (0 to CVB_QOS_QUE_BW_MAX) \n
 *                      Queue bandwidth
 * @param[out] None
 * @retval RT_ERR_OK            The egress queue bandwidth is set successfully.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_QUEUE_ID     The queue is invalid.
 * @retval -RT_ERR_ENABLE       The enable value is invalid.
 * @retval -RT_ERR_QOS_BW     The queue bandwidth is invalid.
 * @retval Others               Please refer to cvb_reg_field_write() and cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_egr_que_bw_ctrl_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_RtkEnable en, cvb_QosBw bw)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(en >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else if(bw > CVB_QOS_QUE_BW_MAX)
    {
        retVal = -RT_ERR_QOS_BW;
    }
    else
    {
        /* do nothing */
    }

    /* Egress bandwidth includes the preamble and IFG (20 bytes) */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_PORT_LEAKY_BUCKET_CONTROL,
                                            CVB_PORT_LEAKY_BUCKET_CONTROL_PKTLENWITHPIFG_LSP,
                                            CVB_PORT_LEAKY_BUCKET_CONTROL_PKTLENWITHPIFG_LEN,
                                            (uint32)0x1u));
    if(que >= CVB_QOS_QUE_BASE)
    {
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q4MAXLB_EN_LSP + (que - CVB_QOS_QUE_BASE),
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q4MAXLB_EN_LEN,
                                                           (uint32)en));
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + ((que - CVB_QOS_QUE_BASE) * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LSP,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LEN,
                                                           bw));
    }
    else
    {
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q0MAXLB_EN_LSP + que,
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q0MAXLB_EN_LEN,
                                                           (uint32)en));
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + (que * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LSP,
                                                           CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LEN,
                                                           bw));
    }

    return retVal;
}
/**
 * @brief Get the queue bandwidth for a specific queue of a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                       Egress port number
 * @param[in] que        Queue (0 to QOS_QUE_MAX) \n
 *                       Queue number
 * @param[out] pEn       Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *                       Enable value
 * @param[out] pQueBw  Queue bandwidth (0 to CVB_QOS_QUE_BW_MAX) \n
 *                       Queue bandwidth
 * @retval RT_ERR_OK     The egress queue bandwidth is got successfully.
 * @retval Others        Please refer to cvb_drv_qos_egr_que_bw_ctrl_ret_chk() and cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_qos_egr_que_bw_ctrl_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_RtkEnable *pEn, cvb_QosBw *pQueBw)
{
    RtkApiRet retVal;
    uint32    regVal = 0u;

    /* Check the input parameters */
    retVal = cvb_drv_qos_egr_que_bw_ctrl_ret_chk(unit, port, que, pEn, pQueBw);

    /* Get the queue bandwidth for a specific queue of a specific port. */
    if(que >= CVB_QOS_QUE_BASE)
    {
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q4MAXLB_EN_LSP + (que - CVB_QOS_QUE_BASE),
                                                           CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q4MAXLB_EN_LEN,
                                                           &regVal));
        if(RT_ERR_OK == retVal)
        {
            if(0u == regVal)
            {
                *pEn = CVB_DISABLED;
            }
            else
            {
                *pEn = CVB_ENABLED;
            }
        }
        CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + ((que - CVB_QOS_QUE_BASE) * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LSP,
                                                           CVB_PORT0_Q4_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LEN,
                                                           pQueBw));
    }
    else
    {
        CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                               CVB_PORT0_QUEUE_SCHEDULE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                               CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q0MAXLB_EN_LSP + que,
                                               CVB_PORT0_QUEUE_SCHEDULE_CONTROL_Q0MAXLB_EN_LEN,
                                               &regVal));
        if(RT_ERR_OK == retVal)
        {
            if(0u == regVal)
            {
                *pEn = CVB_DISABLED;
            }
            else
            {
                *pEn = CVB_ENABLED;
            }
        }
        CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                               CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]) + (que * CVB_QOS_PRI_OFFSET * CVB_QOS_PRI_SIZE),
                                               CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LSP,
                                               CVB_PORT0_Q0_SCHEDULER_BUCKET_CONTROL1_MAXBKTAVERAGERATE_LEN,
                                               pQueBw));
    }

    return retVal;
}
/**@}*/ /* QOS_EGR_BW_ASIC */
/**@}*/ /* QOS_EGR_BW */

/**
 * @addtogroup QOS_TPR Traffic Priority Remarking
 * @{
 */

/**
 * @defgroup QOS_TPR_ASIC Low Level Driver
 * @brief QoS Low Level Driver
 * @{
 */

/**
 * @brief Set the inner tag priority remarking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *			  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port		Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *						Egress port number
 * @param[in] en		Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *						Enable value
 * @param[out] None
 * @retval RT_ERR_OK			The remarking enable is set successfully.
 * @retval -RT_ERR_PORT_ID		The Port number is invalid.
 * @retval -RT_ERR_ENABLE		The enable value is invalid.
 * @retval Others				Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_cvlan_rmk_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(en >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* do nothing */
    }

    /* Set the inner tag priority remarking status. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMK_LSP,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMK_LEN,
                                                       (uint32)en));

    return retVal;
}

/**
 * @brief Get the inner tag priority remarking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[out] pEn      Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *                      Enable value
 * @retval RT_ERR_OK            The remarking enable is set successfully.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_NULL_POINTER The input parameter is invalid.
 * @retval Others               Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_cvlan_rmk_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEn)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the inner tag priority remarking status. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMK_LSP,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMK_LEN,
                                           &regVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == regVal)
        {
            *pEn = CVB_DISABLED;
        }
        else
        {
            *pEn = CVB_ENABLED;
        }
    }

    return retVal;
}

/**
 * @brief Set the outer tag priority remarking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] en        Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *                      Enable value
 * @param[out] None
 * @retval RT_ERR_OK            The remarking enable is set successfully.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_ENABLE       The enable value is invalid.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_svlan_rmk_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(en >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* do nothing */
    }

    /* Set the outer tag priority remarking status. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMK_LSP,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMK_LEN,
                                                       (uint32)en));

    return retVal;
}

/**
 * @brief Get the outer tag priority remarking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[out] pEn      Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *                      Enable value
 * @retval RT_ERR_OK            The remarking enable is set successfully.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_NULL_POINTER The input parameter is invalid.
 * @retval Others               Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_svlan_rmk_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEn)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the outer tag priority remarking status. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMK_LSP,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMK_LEN,
                                           &regVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == regVal)
        {
            *pEn = CVB_DISABLED;
        }
        else
        {
            *pEn = CVB_ENABLED;
        }
    }

    return retVal;
}

/**
 * @brief Set the DSCP priority remarking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[in] en        Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *                      Enable value
 * @param[out] None
 * @retval RT_ERR_OK            The dscp remarking enable is set successfully.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_ENABLE       The enable value is invalid.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_dscp_rmk_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(en >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* do nothing */
    }

    /* Set the DSCP priority remarking status. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMK_LSP,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMK_LEN,
                                                       (uint32)en));

    return retVal;
}

/**
 * @brief Get the DSCP priority remarking status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                      Egress port number
 * @param[out] pEn      Enable value (CVB_DISABLE to CVB_ENABLED) \n
 *                      Enable value
 * @retval RT_ERR_OK            The dscp remarking enable is set successfully.
 * @retval -RT_ERR_PORT_ID      The Port number is invalid.
 * @retval -RT_ERR_NULL_POINTER The input parameter is invalid.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_qos_dscp_rmk_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEn)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get the DSCP priority remarking status. */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMK_LSP,
                                                       CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMK_LEN,
                                                       &regVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == regVal)
        {
            *pEn = CVB_DISABLED;
        }
        else
        {
            *pEn = CVB_ENABLED;
        }
    }

    return retVal;
}

/**
 * @brief Set egress port ctag priority and dei remark table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx    CVLAN remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                   CVLAN remarking table index
 * @param[in] oriPri Original priority (0 to QOS_PRI_MAX) \n
 *                   Original priority
 * @param[in] rmkPri Remarking priority (0 to QOS_PRI_MAX) \n
 *                   Remarking priority
 * @param[in] rmkDei Remarking dei (0 to QOS_DEI_MAX) \n
 *                   Remarking dei
 * @param[out] None
 * @retval RT_ERR_OK                 The remarking is set successfully.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The remarking table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The original priority or/and remarking priority is/are invalid.
 * @retval -RT_ERR_QOS_1P_DEI        The remarking dei is invalid.
 * @retval Others                    Please refer to cvb_reg_field_write().
 */
RtkApiRet cvb_qos_cvlan_rmk_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri rmkPri, cvb_QosDei rmkDei)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if((oriPri > QOS_PRI_MAX) || (rmkPri > QOS_PRI_MAX))
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(rmkDei > QOS_DEI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_DEI;
    }
    else
    {
        /* do nothing */
    }

    /* Set egress port ctag priority and dei remark table. */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                            CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_PRITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                            CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_PRITOPRI0_LEN,
                                            rmkPri));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                            CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_IDEITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                            CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_IDEITOPRI0_LEN,
                                            rmkDei));

    return retVal;
}

/**
 * @brief Get egress port ctag priority & dei remark table configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx       CVLAN remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                      CVLAN  remarking table index
 * @param[in] oriPri    Original priority (0 to QOS_PRI_MAX) \n
 *                      Original priority
 * @param[out] pRmkPri  Remarking priority (0 to QOS_PRI_MAX) \n
 *                      Remarking priority
 * @param[out] pRmkDei  Remarking dei (0 to QOS_DEI_MAX) \n
 *                      Remarking dei
 * @retval RT_ERR_OK                 The remarking is got successfully.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The remarking table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The original priority or/and remarking priority is/are invalid.
 * @retval -RT_ERR_NULL_POINTER      The input parameter is invalid.
 * @retval Others                    Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_cvlan_rmk_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri *pRmkPri, cvb_QosDei *pRmkDei)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(oriPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if((NULL == pRmkPri) || (NULL == pRmkDei))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get egress port ctag priority & dei remark table configuration. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                           CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_PRITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                           CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_PRITOPRI0_LEN,
                                           pRmkPri));
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                           CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_IDEITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                           CVB_INNER_TAG_PRIORITY_REMARKING_TABLE_CONTROL0_IDEITOPRI0_LEN,
                                           pRmkDei));

    return retVal;
}

/**
 * @brief Set egress port stag priority and dei remark table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx    SVLAN remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                   SVLAN remarking table index
 * @param[in] oriPri Original priority (0 to QOS_PRI_MAX) \n
 *                   Original priority
 * @param[in] rmkPri Remarking priority (0 to QOS_PRI_MAX) \n
 *                   Remarking priority
 * @param[in] rmkDei Remarking dei (0 to QOS_DEI_MAX) \n
 *                   Remarking dei
 * @param[out] None
 * @retval RT_ERR_OK                 The remarking is set successfully.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The remarking table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The original priority or/and remarking priority is/are invalid.
 * @retval -RT_ERR_QOS_1P_DEI        The remarking dei is invalid.
 * @retval Others                    Please refer to cvb_reg_field_write().
 */
RtkApiRet cvb_qos_svlan_rmk_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri rmkPri, cvb_QosDei rmkDei)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if((oriPri > QOS_PRI_MAX) || (rmkPri > QOS_PRI_MAX))
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(rmkDei > QOS_DEI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_DEI;
    }
    else
    {
        /* do nothing */
    }

    /* Set egress port stag priority and dei remark table. */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                            CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_PRITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                            CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_PRITOPRI0_LEN,
                                            rmkPri));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                            CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_ODEITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                            CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_ODEITOPRI0_LEN,
                                            rmkDei));

    return retVal;
}

/**
 * @brief Get egress port stag priority & dei remark table configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx       SVLAN remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                      SVLAN  remarking table index
 * @param[in] oriPri    Original priority (0 to QOS_PRI_MAX) \n
 *                      Original priority
 * @param[out] pRmkPri  Remarking priority (0 to QOS_PRI_MAX) \n
 *                      Remarking priority
 * @param[out] pRmkDei  Remarking dei (0 to QOS_DEI_MAX) \n
 *                      Remarking dei
 * @retval RT_ERR_OK                 The remarking is got successfully.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The remarking table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The original priority or/and remarking priority is/are invalid.
 * @retval -RT_ERR_NULL_POINTER      The input parameter is invalid.
 * @retval Others                    Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_svlan_rmk_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosPri *pRmkPri, cvb_QosDei *pRmkDei)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(oriPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if((NULL == pRmkPri) || (NULL == pRmkDei))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get egress port stag priority & dei remark table configuration. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                           CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_PRITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                           CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_PRITOPRI0_LEN,
                                           pRmkPri));
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_PRI_OFFSET),
                                           CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_ODEITOPRI0_LSP + (oriPri * CVB_QOS_PRI_OFFSET),
                                           CVB_OUTER_TAG_PRIORITY_AND_DEI_REMARKING_TABLE_CONTROL0_ODEITOPRI0_LEN,
                                           pRmkDei));

    return retVal;
}

/**
 * @brief Set gress port dscp remark table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx     DSCP remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                    DSCP  remarking table index
 * @param[in] oriPri  Original priority (0 to QOS_PRI_MAX) \n
 *                    Original priority
 * @param[in] rmkDscp Remarking DSCP (0 to QOS_DSCP_MAX) \n
 *                    Remarking DSCP
 * @param[out] None
 * @retval RT_ERR_OK                 The dscp remarking is set successfully.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The dscp remarking table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The original priority or/and remarking dscp is/are invalid.
 * @retval -RT_ERR_QOS_DSCP_VALUE    The remarking dscp is invalid.
 * @retval Others                    Please refer to cvb_reg_field_write().
 */
RtkApiRet cvb_qos_dscp_rmk_set(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosDscp rmkDscp)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(oriPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(rmkDscp > QOS_DSCP_MAX)
    {
        retVal = -RT_ERR_QOS_DSCP_VALUE;
    }
    else
    {
        /* do nothing */
    }

    /* Set gress port dscp remark table. */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_DSCP_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_QUE_SIZE) + ((oriPri / CVB_QOS_DSCP_NUM) * CVB_QOS_DSCP_OFFSET),
                                            CVB_DSCP_REMARKING_TABLE_CONTROL0_DSCPTOPRI0_LSP + ((oriPri % CVB_QOS_DSCP_NUM) * CVB_QOS_DSCP_RMK_SIZE),
                                            CVB_DSCP_REMARKING_TABLE_CONTROL0_DSCPTOPRI0_LEN,
                                            rmkDscp));

    return retVal;
}

/**
 * @brief Get egress port dscp remark table configuration.
 * @param[in] unit                   Stacking unit (1 to 7) \n
 *                                   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx                    Remarking table index
 * @param[in] oriPri                 Internal priority
 * @param[out] pRmkDscp              Remarked dscp priority
 * @retval RT_ERR_OK                 The dscp remarking is got successfully.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The dscp remarking table index is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The original priority or/and remarking dscp is/are invalid.
 * @retval -RT_ERR_NULL_POINTER      The input parameter is invalid.
 * @retval Others                    Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_qos_dscp_rmk_get(uint32 unit, cvb_QosTblIdx idx, cvb_QosPri oriPri, cvb_QosDscp *pRmkDscp)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else if(oriPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(NULL == pRmkDscp)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get egress port dscp remark table configuration */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_DSCP_REMARKING_TABLE_CONTROL0 + (idx * CVB_QOS_QUE_SIZE) + ((oriPri / CVB_QOS_DSCP_NUM) * CVB_QOS_DSCP_OFFSET),
                                           CVB_DSCP_REMARKING_TABLE_CONTROL0_DSCPTOPRI0_LSP + ((oriPri % CVB_QOS_DSCP_NUM) * CVB_QOS_DSCP_RMK_SIZE),
                                           CVB_DSCP_REMARKING_TABLE_CONTROL0_DSCPTOPRI0_LEN,
                                           pRmkDscp));

    return retVal;
}

/**
 * @brief Set a specific egress port to use inner tag remarking table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                              Egress port number
 * @param[in] idx               Remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                              Remarking table index
 * @param[out] None
 * @retval RT_ERR_OK                 The remarking is set successfully.
 * @retval -RT_ERR_PORT_ID           The Port number is invalid.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The remarking table index is invalid.
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_cvlan_rmk_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else
    {
        /* do nothing */
    }

    /* Set a specific port to use inner tag remarking table. */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMKTBLIDX_LSP,
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMKTBLIDX_LEN,
                                            idx));

    return retVal;
}

/**
 * @brief Get which inner tag remarking table is used for a specific egress port.
 * @param[in] unit              Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                              Egress port number
 * @param[out] pIdx             Remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                              Remarking table index
 * @retval RT_ERR_OK            The CVLAN remarking table is got successfully
 * @retval -RT_ERR_PORT_ID      The Port number is invalid
 * @retval -RT_ERR_NULL_POINTER The CVLAN remarking table index is NULL
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get()
 */
RtkApiRet cvb_qos_cvlan_rmk_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIdx)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get which inner tag remarking table is used for a specific port. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMKTBLIDX_LSP,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_IPRIRMKTBLIDX_LEN,
                                           pIdx));

    return retVal;
}

/**
 * @brief Set a specific egress port to use outer tag remarking table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                              Egress port number
 * @param[in] idx               Remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                              Remarking table index
 * @param[out] None
 * @retval RT_ERR_OK                 The remarking is set successfully.
 * @retval -RT_ERR_PORT_ID           The Port number is invalid.
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The remarking table index is invalid.
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_qos_svlan_rmk_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else
    {
        /* do nothing */
    }

    /* Set a specific port to use outer tag remarking table. */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMKTBLIDX_LSP,
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMKTBLIDX_LEN,
                                            idx));

    return retVal;
}

/**
 * @brief Get which outer tag remarking table is used for a specific egress port.
 * @param[in] unit              Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                              Egress port number
 * @param[out] pIdx             Remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                              Remarking table index
 * @retval RT_ERR_OK            The SVLAN remarking table is got successfully
 * @retval -RT_ERR_PORT_ID      The Port number is invalid
 * @retval -RT_ERR_NULL_POINTER The SVLAN remarking table index is NULL
 * @retval Others               Please refer to cvb_reg_field_read()
 */
RtkApiRet cvb_qos_svlan_rmk_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIdx)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get which outer tag remarking table is used for a specific port. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMKTBLIDX_LSP,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_OPRIRMKTBLIDX_LEN,
                                           pIdx));

    return retVal;
}

/**
 * @brief Set a specific egress port to use DSCP remarking table.
 * @param[in] unit                   Stacking unit (1 to 7) \n
 *                                   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
                                     Egress port number
 * @param[out] pIdx                  Remarking table index (0 to QOS_TBL_IDX_MAX) \n
 *                                   Remarking table index
 * @retval RT_ERR_OK                 The DSCP remarking table is set successfully
 * @retval -RT_ERR_PORT_ID           The Port number is invalid
 * @retval -RT_ERR_QOS_SEL_PRI_GROUP The DSCP remarking table index is invalid
 * @retval Others                    Please refer to cvb_drv_indirect_reg_field_set()
 */
RtkApiRet cvb_qos_dscp_rmk_tbl_set(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx idx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(idx > QOS_TBL_IDX_MAX)
    {
        retVal = -RT_ERR_QOS_SEL_PRI_GROUP;
    }
    else
    {
        /* do nothing */
    }

    /* Set a specific port to use DSCP remarking table. */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMKTBLIDX_LSP,
                                            CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMKTBLIDX_LEN,
                                            idx));

    return retVal;
}

/**
 * @brief Get which DSCP remarking table is used for a specific egress port.
 * @param[in] unit              Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
                                Egress port number
 * @param[out] pIdx             Remarking table index
 * @retval RT_ERR_OK            The DSCP remarking table is got successfully
 * @retval -RT_ERR_PORT_ID      The Port number is invalid
 * @retval -RT_ERR_NULL_POINTER The DSCP remarking table index is NULL
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get()
 */
RtkApiRet cvb_qos_dscp_rmk_tbl_get(uint32 unit, cvb_RtkPort port, cvb_QosTblIdx *pIdx)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIdx)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get which DSCP remarking table is used for a specific port. */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]),
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMKTBLIDX_LSP,
                                           CVB_PORT0_PRIORITY_REMARKING_CONTROL_DSCPRMKTBLIDX_LEN,
                                           pIdx));

    return retVal;
}
/**@}*/ /* QOS_TPR_ASIC */
/**@}*/ /* QOS_TPR */
/**@}*/ /* QOS */
