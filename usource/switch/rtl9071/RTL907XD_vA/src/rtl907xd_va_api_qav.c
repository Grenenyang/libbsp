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
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_qav.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"

/**
 * @defgroup RTK_MODULE_QAV QAV
 * @brief Qav APIs
 * @{
 */

/**
 * @defgroup QAV_TS Traffic Scheduling
 * @{
 */

/**
 * @defgroup QAV_TS_ASIC Low Level Driver
 * @brief QAV Low Level Driver
 * @{
 */

/**
 * @brief Set the CBS bandwidth for specific port and specific queue.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                        Egress port id
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
                        Queue number
 * @param[in] bw        Bandwidth (0 to DVA_QAV_BW_MAX) \n
                        CBS bandwidth in units of bps.
 * @param[out] None
 * @retval RT_ERR_OK         The API was executed correctly.
 * @retval -RT_ERR_PORT_ID   The port is invalid.
 * @retval -RT_ERR_QUEUE_ID  The queue is invalid.
 * @retval Others            Please refer to dv1_drv_indirect_reg_field_set()
 * @note There is a quantization error in the configured bandwidth, so the read value may be slightly different than write value.
 */
RtkApiRet dva_qav_slope_set(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_QavBw bw)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regAddr;
    uint32    u32TokenFloor;
    uint64    tokenFloor;
    uint64    floorError;
    uint64    ceilError;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(bw > DVA_QAV_BW_MAX)
    {
        retVal = -RT_ERR_QAV_INVALID_BW;
    }
    else
    {
        regAddr = DVA_PORT0_Q0_SCHEDULER_BUCKET_CONTROL3 + (DVA_QAV_ADMIN_IDLE_SLOPE_REG_OFFSET * que) + (PERPORT_REG_OFFSET * dva_g_userPort2sysPort[port]);
    }

    if(RT_ERR_OK == retVal)
    {
        /* Calculate token counter */
        tokenFloor = ((bw << DVA_QAV_PRECISION_BITS) / ((uint64)DVA_QAV_CLK_FREQ));
        floorError = bw - ((tokenFloor * DVA_QAV_CLK_FREQ) >> DVA_QAV_PRECISION_BITS);
        ceilError  = (((tokenFloor + 1u) * DVA_QAV_CLK_FREQ) >> DVA_QAV_PRECISION_BITS) - bw;
        if(floorError > ceilError)
        {
            tokenFloor += 1u;
        }

        /* Set idleslope token of queue */
        u32TokenFloor = (uint32)(tokenFloor & 0xFFFFFFFFu);
        retVal        = dva_drv_indirect_reg_field_set(unit, regAddr, DVA_PORT0_Q1_SCHEDULER_BUCKET_CONTROL3_IDLESLOPE_TOKEN_LSP, DVA_PORT0_Q1_SCHEDULER_BUCKET_CONTROL3_IDLESLOPE_TOKEN_LEN, u32TokenFloor);
    }

    return retVal;
}

/**
 * @brief Set the CBS bandwidth for specific port and specific queue.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *                      Egress port id
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[out] bw       Bandwidth (0 to DVA_QAV_BW_MAX) \n
 *                      CBS bandwidth in units of bps.
 * @retval RT_ERR_OK              The API was executed correctly.
 * @retval -RT_ERR_PORT_ID        The port is invalid.
 * @retval -RT_ERR_QUEUE_ID       The queue is invalid.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * Others  Please refer to dv1_drv_indirect_reg_field_get()
 * @note There is a quantization error in the configured bandwidth, so the read value may be slightly different than write value.
 */
RtkApiRet dva_qav_slope_get(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_QavBw *bw)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    idleSlopeToken = 0u;
    uint32    regAddr        = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(NULL == bw)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        regAddr = DVA_PORT0_Q0_SCHEDULER_BUCKET_CONTROL3 + (DVA_QAV_ADMIN_IDLE_SLOPE_REG_OFFSET * que) + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]);
    }

    /* Get idleslope_token for calculating bw */
    CHK_FUN_RET(retVal, (dva_drv_indirect_reg_field_get(unit,
                                                        regAddr,
                                                        DVA_PORT0_Q0_SCHEDULER_BUCKET_CONTROL3_IDLESLOPE_TOKEN_LSP,
                                                        DVA_PORT0_Q0_SCHEDULER_BUCKET_CONTROL3_IDLESLOPE_TOKEN_LEN,
                                                        &idleSlopeToken)));
    if(RT_ERR_OK == retVal)
    {
        *bw = ((((uint64)idleSlopeToken) * DVA_QAV_CLK_FREQ) >> DVA_QAV_PRECISION_BITS);
    }

    return retVal;
}

/**
 * @brief Set the transmission selection algorithm to Strict Priority or Credit Based Shaper and set CBS bandwidth for specific port and specific queue.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *                      Egress port id
 * @param[in] en        Enable (DVA_DISABLED to DVA_ENABLED) \n
 *                      CBS enable value
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[in] bw        Bandwidth (0 to DVA_QAV_BW_MAX) \n
 *                      CBS bandwidth in units of bps.
 * @param[out] None
 * @retval RT_ERR_OK              The API was executed correctly.
 * @retval -RT_ERR_PORT_ID        The port is invalid.
 * @retval -RT_ERR_QUEUE_ID       The queue is invalid.
 * @retval -RT_ERR_ENABLE         The enable is invalid.
 * @retval Others                 Please refer to dv1_qos_que_sch_set() and dv1_qav_slope_set()
 */
RtkApiRet dva_qav_cbs_set(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_RtkEnable en, dva_QavBw bw)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkQoSSch queSch;
    dva_QavBw qavBw;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(en >= DVA_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* do nothing */
    }

    if(RT_ERR_OK == retVal)
    {
        if(en == DVA_DISABLED)
        {
            queSch = DVA_QOS_SCH_SP;
            qavBw  = (uint64)0u;
        }
        else
        {
            queSch = DVA_QOS_SCH_CBS;
            qavBw  = bw;
        }
    }
    /* Set tx algorithm and bandwidth */
    CHK_FUN_RET(retVal, dva_qos_que_sch_set(unit, port, que, queSch));
    CHK_FUN_RET(retVal, dva_qav_slope_set(unit, port, que, qavBw));

    return retVal;
}

/**
 * @brief Get the transmission selection algorithm and read the CBS bandwidth from specific port and specific queue.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *                      Egress port id
 * @param[in] en        Enable (DVA_DISABLED to DVA_ENABLED) \n
 *                      CBS enable value
 * @param[in] que       Queue (0 to QOS_QUE_MAX) \n
 *                      Queue number
 * @param[out] bw       Bandwidth (0 to DVA_QAV_BW_MAX) \n
 *                      CBS bandwidth in units of bps.
 * @retval RT_ERR_OK              The API was executed correctly.
 * @retval -RT_ERR_PORT_ID        The port is invalid.
 * @retval -RT_ERR_QUEUE_ID       The queue is invalid.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dv1_qos_que_sch_set() and dv1_qav_slope_set()
 */
RtkApiRet dva_qav_cbs_get(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_RtkEnable *en, dva_QavBw *bw)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkQoSSch queSch = DVA_QOS_SCH_CBS;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if((NULL == en) || (NULL == bw))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Get tx algorithm and bandwidth */
    CHK_FUN_RET(retVal, dva_qos_que_sch_get(unit, port, que, &queSch));
    if(RT_ERR_OK == retVal)
    {
        if(DVA_QOS_SCH_CBS == queSch)
        {
            *en    = DVA_ENABLED;
            retVal = dva_qav_slope_get(unit, port, que, bw);
        }
        else
        {
            *en = DVA_DISABLED;
            *bw = 0u;
        }
    }

    return retVal;
}
/**@}*/ /* QAV_TS_ASIC */
/**@}*/ /* QAV_TS */

/**
 * @defgroup QAV_TC Traffic Classification
 * @{
 */

/**
 * @defgroup QAV_TC_ASIC Low Level Driver
 * @brief QAV Low Level Driver
 * @{
 */

/**
 * @brief Set the priority regeneration table for the specific priority in the specific egress port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *                      Ingress port id
 * @param[in] oriPri    Original priority (0 to QOS_PRI_MAX) \n
 *                      Original priority
 * @param[in] en        Enable (DVA_DISABLED to DVA_ENABLED) \n
 *                      Regeneration enable value
 * @param[in] regenPri  Regenerated priority (0 to QOS_PRI_MAX) \n
 *                      The regenerated priority value to replace the original priority.
 * @param[out] None
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_PORT_ID           The port is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The priority is invalid.
 * @retval -RT_ERR_ENABLE            The enable value is invalid.
 * @retval Others                    Please refer to dv1_drv_indirect_reg_field_set()
 */
RtkApiRet dva_qav_pri_regen_set(uint32 unit, dva_RtkPort port, dva_QosPri oriPri, dva_RtkEnable en, dva_QosPri regenPri)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    enBit   = 0u;
    uint32    regLsp  = 0u;
    uint32    regAddr = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if((oriPri > QOS_PRI_MAX) || (regenPri > QOS_PRI_MAX))
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if(en >= DVA_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        enBit   = DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0_PRIORITYREGENERATIONENABLE_P0_LSP + oriPri;
        regAddr = DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0 + (DVA_QAV_PRI_REGEN_REG_OFFSET * dva_g_userPort2sysPort[port]);
        regLsp  = DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0_REMAP_EAV_PRI0_REGEN_P0_LSP + (DVA_QAV_PRI_REGEN_LSP_OFFSET * oriPri);
    }

    /* Regenerated Priority */
    CHK_FUN_RET(retVal, (dva_drv_indirect_reg_field_set(unit,
                                                        regAddr,
                                                        regLsp,
                                                        DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0_REMAP_EAV_PRI0_REGEN_P0_LEN,
                                                        regenPri)));
    /* Enable Bit */
    CHK_FUN_RET(retVal, (dva_drv_indirect_reg_field_set(unit, regAddr, enBit, 1u, en)));

    return retVal;
}

/**
 * @brief Get the priority regeneration table for the specific priority in the specific egress port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *                      Ingress port id
 * @param[in] oriPri    Original priority (0 to QOS_PRI_MAX) \n
 *                      Original priority
 * @param[out] en       Enable (DVA_DISABLED to DVA_ENABLED) \n
 *                      Regeneration enable value
 * @param[out] regenPri Regenerated priority (0 to QOS_PRI_MAX) \n
 *                      The regenerated priority value to replace the original priority.
 * @retval RT_ERR_OK                 The API was executed correctly.
 * @retval -RT_ERR_PORT_ID           The port is invalid.
 * @retval -RT_ERR_QOS_1P_PRIORITY   The priority is invalid.
 * @retval -RT_ERR_NULL_POINTER      The input parameter is NULL.
 * @retval Others               Please refer to dv1_reg_field_read() and dv1_reg_bit_get()
 */
RtkApiRet dva_qav_pri_regen_get(uint32 unit, dva_RtkPort port, dva_QosPri oriPri, dva_RtkEnable *en, dva_QosPri *regenPri)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    enBit   = 0u;
    uint32    regLsp  = 0u;
    uint32    regAddr = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(oriPri > QOS_PRI_MAX)
    {
        retVal = -RT_ERR_QOS_1P_PRIORITY;
    }
    else if((NULL == en) || (NULL == regenPri))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        enBit   = DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0_PRIORITYREGENERATIONENABLE_P0_LSP + oriPri;
        regLsp  = DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0_REMAP_EAV_PRI0_REGEN_P0_LSP + (DVA_QAV_PRI_REGEN_LSP_OFFSET * oriPri);
        regAddr = DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0 + (DVA_QAV_PRI_REGEN_REG_OFFSET * dva_g_userPort2sysPort[port]);
    }

    /* Regenerated Priority */
    CHK_FUN_RET(retVal, (dva_reg_field_read(unit,
                                            regAddr,
                                            regLsp,
                                            DVA_PORT_PRIORITY_REGENARATION_MAPPING_TABLE0_REMAP_EAV_PRI0_REGEN_P0_LEN,
                                            regenPri)));
    /* Enable Bit */
    CHK_FUN_RET(retVal, dva_reg_bit_get(unit, regAddr, enBit, en));

    return retVal;
}

/**
 * @brief Set the priority regeneration target vlan tag.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *                      Egress port id
 * @param[in] enIPri    Enable inner priority (DVA_DISABLED to DVA_ENABLED) \n
 *                      Enable the use of inner tag priority for regeneration
 * @param[in] enOPri    Enable outer priority (DVA_DISABLED to DVA_ENABLED) \n
 *                      Enable the use of outer tag priority for regeneration
 * @param[out] None
 * @retval RT_ERR_OK            The API was executed correctly.
 * @retval -RT_ERR_PORT_ID      The port is invalid.
 * @retval -RT_ERR_ENABLE       The enable value is invalid.
 * @retval Others               Please refer to dv1_drv_indirect_reg_field_set()
 * @note  Usually enable only one target tag at specific egress port.
 */
RtkApiRet dva_qav_regen_tag_sel_set(uint32 unit, dva_RtkPort port, dva_RtkEnable enIPri, dva_RtkEnable enOPri)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if((enIPri >= DVA_RTK_ENABLE_END) || (enOPri >= DVA_RTK_ENABLE_END))
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        regAddr = DVA_PORT0_EAV_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]);
    }

    /* Enable/Disable inner/outer priority regeneration */
    CHK_FUN_RET(retVal, (dva_drv_indirect_reg_field_set(unit, regAddr, DVA_PORT0_EAV_CONTROL_IPRI_RMP_LSP, DVA_PORT0_EAV_CONTROL_IPRI_RMP_LEN, enIPri)));
    CHK_FUN_RET(retVal, (dva_drv_indirect_reg_field_set(unit, regAddr, DVA_PORT0_EAV_CONTROL_OPRI_RMP_LSP, DVA_PORT0_EAV_CONTROL_OPRI_RMP_LEN, enOPri)));

    return retVal;
}

/**
 * @brief Get the priority regeneration target vlan tag.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *                      Egress port id
 * @param[out] pEnIPri  Enable inner priority (DVA_DISABLED to DVA_ENABLED) \n
 *                      Enable the use of inner tag priority for regeneration
 * @param[out] pEnOPri  Enable outer priority (DVA_DISABLED to DVA_ENABLED) \n
 *                      Enable the use of outer tag priority for regeneration
 * @retval RT_ERR_OK              The API was executed correctly.
 * @retval -RT_ERR_PORT_ID        The port is invalid.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others                 Please refer to dv1_drv_indirect_reg_field_get()
 */
RtkApiRet dva_qav_regen_tag_sel_get(uint32 unit, dva_RtkPort port, dva_RtkEnable *pEnIPri, dva_RtkEnable *pEnOPri)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if((NULL == pEnIPri) || (NULL == pEnOPri))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        regAddr = DVA_PORT0_EAV_CONTROL + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]);
    }

    /* Get the configuration of inner/outer priority regeneration */
    CHK_FUN_RET(retVal, (dva_drv_indirect_reg_field_get(unit, regAddr, DVA_PORT0_EAV_CONTROL_IPRI_RMP_LSP, DVA_PORT0_EAV_CONTROL_IPRI_RMP_LEN, pEnIPri)));
    CHK_FUN_RET(retVal, (dva_drv_indirect_reg_field_get(unit, regAddr, DVA_PORT0_EAV_CONTROL_OPRI_RMP_LSP, DVA_PORT0_EAV_CONTROL_OPRI_RMP_LEN, pEnOPri)));

    return retVal;
}
/**@}*/ /* QAV_TC_ASIC */
/**@}*/ /* QAV_TC */
/**@}*/ /* QAV */
