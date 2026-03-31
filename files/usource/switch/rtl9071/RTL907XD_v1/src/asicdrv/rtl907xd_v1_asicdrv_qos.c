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

#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_regField_list.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_api_qos.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#include "rtl907xd_v1_asicdrv_qos.h"

/**
 * @brief Check the input parametes for the ingress bandwidth and flow control ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] pIgrBwCtrl Ingress bandwidth control configuration
 * @param[out] None
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_ENABLE
 * @retval -RT_ERR_QOS_BW
 * @retval RT_ERR_OK
 * @note pIgrBwCtrl->bwCtrlEn0 stands for ingress bandwidth control ability, from DV1_DISABLED to DV1_ENABLED. \n
 *      pIgrBwCtrl->bwCtrlEn1 stands for ingress bandwidth control ability, from DV1_DISABLED to DV1_ENABLED. \n
 *      pIgrBwCtrl->igrBw0 stands for the bw of LB0, in units of 8Kbps. \n
 *      pIgrBwCtrl->igrBw1 stands for the bw of LB1, in units of 8Kbps.\n
 *      pIgrBwCtrl->flowCtrlEn0 stands for the flow control ability of LB0. \n
 *      pIgrBwCtrl->flowCtrlEn1 stands for the flow control ability of LB1.
 */
RtkApiRet dv1_drv_qos_igr_bw_ctrl_set_ret_chk(uint32 unit, dv1_RtkPort port, const QosIgrBwCtrl_t *pIgrBwCtrl)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIgrBwCtrl)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((pIgrBwCtrl->bwCtrlEn0 >= DV1_RTK_ENABLE_END) || (pIgrBwCtrl->bwCtrlEn1 >= DV1_RTK_ENABLE_END) || (pIgrBwCtrl->flowCtrlEn0 >= DV1_RTK_ENABLE_END) || (pIgrBwCtrl->flowCtrlEn1 >= DV1_RTK_ENABLE_END))
    {
        retVal = -RT_ERR_ENABLE;
    }
    else if((pIgrBwCtrl->igrBw0 > DV1_QOS_PORT_BW_MAX) || (pIgrBwCtrl->igrBw1 > DV1_QOS_PORT_BW_MAX))
    {
        retVal = -RT_ERR_QOS_BW;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Check the input parametes for the ingress bandwidth and flow control ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] pIgrBwCtrl Ingress bandwidth control configuration
 * @param[out] None
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_ENABLE
 * @retval -RT_ERR_QOS_BW
 * @retval RT_ERR_OK
 * @note pIgrBwCtrl->bwCtrlEn0 stands for ingress bandwidth control ability, from DV1_DISABLED to DV1_ENABLED. \n
 *      pIgrBwCtrl->bwCtrlEn1 stands for ingress bandwidth control ability, from DV1_DISABLED to DV1_ENABLED. \n
 *      pIgrBwCtrl->igrBw0 stands for the bw of LB0, in units of 8Kbps. \n
 *      pIgrBwCtrl->igrBw1 stands for the bw of LB1, in units of 8Kbps.\n
 *      pIgrBwCtrl->flowCtrlEn0 stands for the flow control ability of LB0. \n
 *      pIgrBwCtrl->flowCtrlEn1 stands for the flow control ability of LB1.
 */
RtkApiRet dv1_drv_qos_igr_bw_ctrl_get_ret_chk(uint32 unit, dv1_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIgrBwCtrl)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Get the ingress bandwidth control ability.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[out] pIgrBwCtrl Ingress bandwidth control configuration
 * @retval RT_ERR_OK
 */
RtkApiRet dv1_drv_qos_igr_bw_ctrl_get_0(uint32 unit, dv1_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    /* Get the ability of ingress bandwidth control 0 */
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_EN_LSP,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_EN_LEN,
                                                       &regVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == regVal)
        {
            pIgrBwCtrl->bwCtrlEn0 = DV1_DISABLED;
        }
        else
        {
            pIgrBwCtrl->bwCtrlEn0 = DV1_ENABLED;
        }
    }

    /* Get the ability of ingress bandwidth control 1 */
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_EN_LSP,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_EN_LEN,
                                                       &regVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == regVal)
        {
            pIgrBwCtrl->bwCtrlEn1 = DV1_DISABLED;
        }
        else
        {
            pIgrBwCtrl->bwCtrlEn1 = DV1_ENABLED;
        }
    }

    /* Get the bw of the leaky bucket 0 */
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_RATE_LSP,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_RATE_LEN,
                                                       &pIgrBwCtrl->igrBw0));
    /* Get the bw of the leaky bucket 1 */
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_RATE_LSP,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_RATE_LEN,
                                                       &pIgrBwCtrl->igrBw1));

    return retVal;
}

/**
 * @brief Get the flow control ability of the ingress bandwidth control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[out] pIgrBwCtrl Ingress bandwidth control configuration
 * @retval RT_ERR_OK
 */
RtkApiRet dv1_drv_qos_igr_bw_ctrl_get_1(uint32 unit, dv1_RtkPort port, QosIgrBwCtrl_t *pIgrBwCtrl)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    /* Get the ability of the flow control 0 */
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_FC_EN_LSP,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL0_INBW0_FC_EN_LEN,
                                                       &regVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == regVal)
        {
            pIgrBwCtrl->flowCtrlEn0 = DV1_DISABLED;
        }
        else
        {
            pIgrBwCtrl->flowCtrlEn0 = DV1_ENABLED;
        }
    }

    /* Get the ability of the flow control 1 */
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_FC_EN_LSP,
                                                       DV1_PORT0_INGRESS_BANDWIDTH_RATE_CONTROL1_INBW1_FC_EN_LEN,
                                                       &regVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == regVal)
        {
            pIgrBwCtrl->flowCtrlEn1 = DV1_DISABLED;
        }
        else
        {
            pIgrBwCtrl->flowCtrlEn1 = DV1_ENABLED;
        }
    }

    return retVal;
}

/**
 * @brief Check the input parametes for the traffic scheduler.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] que Queue ID
 * @param[out] queSch Queue schedule algorithm
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_QUEUE_ID
 * @retval -RT_ERR_QOS_SCHEDULE
 * @retval RT_ERR_OK
 */
RtkApiRet dv1_drv_qos_que_sch_set_ret_chk(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, RtkQoSSch queSch)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(queSch > DV1_QOS_SCH_END)
    {
        retVal = -RT_ERR_QOS_SCHEDULE;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Check the input parametes for the traffic scheduler.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] que Queue ID
 * @param[out] pQueSch Queue schedule algorithm
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_QUEUE_ID
 * @retval -RT_ERR_NULL_POINTER
 * @retval RT_ERR_OK
 */
RtkApiRet dv1_drv_qos_que_sch_get_ret_chk(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, RtkQoSSch *pQueSch)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(NULL == pQueSch)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief Get the setting value for the traffic scheduler.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] que Queue ID
 * @param[in] queSch Queue schedule algorithm
 * @param[out] *qosQueSchInfoReg QoS schedule register values
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 */
static RtkApiRet dv1_drv_qos_que_sch_set_info_val_get(RtkQoSSch queSch, QosQueSchInfoReg_t *qosQueSchInfoReg)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(DV1_QOS_SCH_SP == queSch)
    {
        qosQueSchInfoReg->schType = QOS_MINBKTSCKTYPE_SP;
        qosQueSchInfoReg->schSel  = QOS_SCHTYPESEL_WRR;
        qosQueSchInfoReg->schAlgo = DV1_QOS_DOT1QSCKALGTYPE_CBS;
    }
    else if(DV1_QOS_SCH_WRR == queSch)
    {
        qosQueSchInfoReg->schType = QOS_MINBKTSCKTYPE_OTHERS;
        qosQueSchInfoReg->schSel  = QOS_SCHTYPESEL_WRR;
        qosQueSchInfoReg->schAlgo = DV1_QOS_DOT1QSCKALGTYPE_CBS;
    }
    else if(DV1_QOS_SCH_WFQ == queSch)
    {
        qosQueSchInfoReg->schType = QOS_MINBKTSCKTYPE_OTHERS;
        qosQueSchInfoReg->schSel  = QOS_SCHTYPESEL_WFQ;
        qosQueSchInfoReg->schAlgo = DV1_QOS_DOT1QSCKALGTYPE_CBS;
    }
    else if(DV1_QOS_SCH_CBS == queSch)
    {
        qosQueSchInfoReg->schType = QOS_MINBKTSCKTYPE_OTHERS;
        qosQueSchInfoReg->schSel  = QOS_SCHTYPESEL_OTHERS;
        qosQueSchInfoReg->schAlgo = DV1_QOS_DOT1QSCKALGTYPE_CBS;
    }
    else
    {
        /* The ATS is not supported. */
        retVal = -RT_ERR_INPUT;
    }

    return retVal;
}

/**
 * @brief Set the register value for the traffic scheduler.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] que Queue ID
 * @param[in] queSch Queue schedule algorithm
 * @retval RT_ERR_OK
 */
RtkApiRet dv1_drv_qos_que_sch_set_info_set(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, RtkQoSSch queSch)
{
    RtkApiRet          retVal;
    QosQueSchInfoReg_t qosQueSchInfoReg;
    uint32             regAddr;

    retVal = drv_memset(&qosQueSchInfoReg, 0, sizeof(qosQueSchInfoReg));

    if(que >= DV1_QOS_QUE_BASE)
    {
        regAddr = DV1_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]) + ((que - DV1_QOS_QUE_BASE) * DV1_QOS_PRI_OFFSET * DV1_QOS_PRI_SIZE);
    }
    else
    {
        regAddr = DV1_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]) + (que * DV1_QOS_PRI_OFFSET * DV1_QOS_PRI_SIZE);
    }

    CHK_FUN_RET(retVal, dv1_drv_qos_que_sch_set_info_val_get(queSch, &qosQueSchInfoReg));
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit,
                                                       regAddr,
                                                       DV1_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTSCKTYPE_LSP,
                                                       DV1_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTSCKTYPE_LEN,
                                                       qosQueSchInfoReg.schType));

    if((RT_ERR_OK == retVal) && (DV1_QOS_SCH_SP != queSch))
    {
        retVal = dv1_drv_indirect_reg_field_set(unit,
                                                DV1_PORT0_QUEUE_SCHEDULE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                DV1_PORT0_QUEUE_SCHEDULE_CONTROL_SCHEDULER_TYPE_SEL_LSP,
                                                DV1_PORT0_QUEUE_SCHEDULE_CONTROL_SCHEDULER_TYPE_SEL_LEN,
                                                qosQueSchInfoReg.schSel);
    }

    return retVal;
}

/**
 * @brief Get the setting value for the traffic scheduler.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] *queSchInfo QoS tx queue scheduler information
 * @retval RT_ERR_OK
 */
RtkApiRet dv1_drv_qos_que_sch_get_info_get(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, RtkQoSSch *pQueSch)
{
    RtkApiRet retVal;
    uint32    regAddr;
    uint32    schType;
    uint32    schSel;

    if(que >= DV1_QOS_QUE_BASE)
    {
        regAddr = DV1_PORT0_Q4_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]) + ((que - DV1_QOS_QUE_BASE) * DV1_QOS_PRI_OFFSET * DV1_QOS_PRI_SIZE);
    }
    else
    {
        regAddr = DV1_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]) + (que * DV1_QOS_PRI_OFFSET * DV1_QOS_PRI_SIZE);
    }

    /* Get registers */
    retVal = dv1_drv_indirect_reg_field_get(unit,
                                            regAddr,
                                            DV1_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTSCKTYPE_LSP,
                                            DV1_PORT0_Q0_SCHEDULER_BUCKET_CONTROL2_MINBKTSCKTYPE_LEN,
                                            &schType);

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit,
                                                       DV1_PORT0_QUEUE_SCHEDULE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]),
                                                       DV1_PORT0_QUEUE_SCHEDULE_CONTROL_SCHEDULER_TYPE_SEL_LSP,
                                                       DV1_PORT0_QUEUE_SCHEDULE_CONTROL_SCHEDULER_TYPE_SEL_LEN,
                                                       &schSel));

    /* Get a traffic scheduler for a specific queue of a specific port. */
    if(RT_ERR_OK == retVal)
    {
        if(QOS_MINBKTSCKTYPE_SP == schType)
        {
            *pQueSch = DV1_QOS_SCH_SP;
        }
        else
        {
            if(QOS_SCHTYPESEL_WFQ == schSel)
            {
                *pQueSch = DV1_QOS_SCH_WFQ;
            }
            else if(QOS_SCHTYPESEL_WRR == schSel)
            {
                *pQueSch = DV1_QOS_SCH_WRR;
            }
            else
            {
                *pQueSch = DV1_QOS_SCH_CBS;
            }
        }
    }

    return retVal;
}

/**
 * @brief Check the input parameter for the queue bw.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port ID
 * @param[in] que Queue ID
 * @param[out] pEn Enable the egress queue bandwidth control function
 * @param[out] pQueBw Queue bandwidth
 * @retval RT_ERR_OK
 */
RtkApiRet dv1_drv_qos_egr_que_bw_ctrl_ret_chk(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_RtkEnable *pEn, dv1_QosBw *pQueBw)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if((NULL == pEn) || (NULL == pQueBw))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}
