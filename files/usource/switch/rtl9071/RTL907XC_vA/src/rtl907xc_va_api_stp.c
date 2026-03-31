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

#include "rtl907xc_va_api_ext.h"

/**
 * @addtogroup RTK_MODULE_STP STP
 * @{
 */

/**
 * @addtogroup STP_BRIDGE_CONFIGURATION STP Bridge Configuration
 * @{
 */

/**
 * @addtogroup STP_BRIDGE_CONFIGURATION_ASIC Low Level Driver
 * @brief Bridge Configuration Low Level Driver
 * @{
 */

/**
 * @brief This API is used to enable or disable STP.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] stpState Spanning Tree Protocol Global State (DISABLED, ENABLED) \n
 *            Indicates whether the global state of STP is enabled or disabled. \n
 *            DISABLED: Disabled \n
 *            ENABLED: Enabled \n
 * @param[out] None
 * @retval -RT_ERR_ENABLE The input value of STP state is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set STP state successfully.
 */
RtkApiRet cva_stp_state_set(uint32 unit, RtkEnable stpState)
{
    uint32    fieldVal = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(RTK_ENABLE_END <= stpState)
    {
        retVal = (-RT_ERR_ENABLE);
    }
    else
    {
        fieldVal = (ENABLED == stpState) ? 0u : 1u;

        /* Set STP global state register. */
        retVal = cva_reg_field_write(unit,
                                     CVA_RSTP_GLOBAL,
                                     CVA_RSTP_GLOBAL_RSTP_DIS_LSP,
                                     CVA_RSTP_GLOBAL_RSTP_DIS_LEN,
                                     fieldVal);
    }

    return retVal;
}

/**
 * @brief This API is used to check whether STP is enabled or disabled.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] pStpState Spanning Tree Protocol Global State (DISABLED, ENABLED) \n
 *             Indicates whether the global state of STP is enabled or disabled. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get STP state successfully.
 */
RtkApiRet cva_stp_state_get(uint32 unit, RtkEnable *pStpState)
{
    uint32    fieldVal = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(NULL == pStpState)
    {
        retVal = (-RT_ERR_NULL_POINTER);
    }
    else
    {
        /* Get STP global state register. */
        retVal = cva_reg_field_read(unit,
                                    CVA_RSTP_GLOBAL,
                                    CVA_RSTP_GLOBAL_RSTP_DIS_LSP,
                                    CVA_RSTP_GLOBAL_RSTP_DIS_LEN,
                                    &fieldVal);

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Transform STP global state. */
            *pStpState = (1u == fieldVal) ? DISABLED : ENABLED;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to switch STP protocol between STP and RSTP.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] stpType Spanning Tree Protocol Type (N/A) \n
 *            Indicates whether the operating protocol is STP or RSTP.
 * @param[out] None
 * @retval -RT_ERR_INPUT The input value of STP type is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set STP type successfully.
 */
RtkApiRet cva_stp_type_set(uint32 unit, RtkStpType_e stpType)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(EM_STP_TYPE_END <= stpType)
    {
        retVal = (-RT_ERR_INPUT);
    }
    else
    {
        /* Set STP global state register. */
        retVal = cva_reg_field_write(unit,
                                     CVA_RSTP_GLOBAL,
                                     CVA_RSTP_GLOBAL_RSTP_TYPE_LSP,
                                     CVA_RSTP_GLOBAL_RSTP_TYPE_LEN,
                                     (uint32)stpType);
    }

    return retVal;
}

/**
 * @brief This API is used to get spanning tree type.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] pStpType Spanning Tree Protocol Type (N/A) \n
 *             Indicates whether the operating protocol is STP or RSTP.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get STP type successfully.
 */
RtkApiRet cva_stp_type_get(uint32 unit, RtkStpType_e *pStpType)
{
    uint32    fieldVal = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(NULL == pStpType)
    {
        retVal = (-RT_ERR_NULL_POINTER);
    }
    else
    {
        /* Get STP global state register. */
        retVal = cva_reg_field_read(unit,
                                    CVA_RSTP_GLOBAL,
                                    CVA_RSTP_GLOBAL_RSTP_TYPE_LSP,
                                    CVA_RSTP_GLOBAL_RSTP_TYPE_LEN,
                                    &fieldVal);

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Transform STP type. */
            *pStpType = ((uint32)EM_STP_TYPE_RSTP == fieldVal) ? EM_STP_TYPE_RSTP : EM_STP_TYPE_STP;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to set switch priority.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] stpBridgePriority Spanning Tree Protocol Bridge Priority (RTK_STP_BRIDGE_PORT_PRIORITY_0
 *            to RTK_STP_BRIDGE_PORT_PRIORITY_61440) \n
 *            Indicates the bridge priority. \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_0: bridge priority 0 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_4096: bridge priority 4096 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_8192: bridge priority 8192 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_12288: bridge priority 12288 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_16384: bridge priority 16384 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_20480: bridge priority 20480 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_24576: bridge priority 24576 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_28672: bridge priority 28672 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_32768: bridge priority 32768 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_36864: bridge priority 36864 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_40960: bridge priority 40960 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_45056: bridge priority 45056 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_49152: bridge priority 49152 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_53248: bridge priority 53248 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_57344: bridge priority 57344 \n
 *            RTK_STP_BRIDGE_PORT_PRIORITY_61440: bridge priority 61440 \n
 * @param[out] None
 * @retval -RT_ERR_STP_BRIDGE_PRIORITY The input value of priority is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set switch priority successfully.
 */
RtkApiRet cva_stp_bridge_priority_set(uint32 unit, RtkStpBdgPri stpBridgePriority)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(STP_BRIDGE_PORT_PRIORITYMAX < stpBridgePriority)
    {
        retVal = (-RT_ERR_STP_BRIDGE_PRIORITY);
    }
    else
    {
        /* Set STP global state register. */
        retVal = cva_reg_field_write(unit,
                                     CVA_RSTP_GLOBAL,
                                     CVA_RSTP_GLOBAL_BRIDGE_PRIORITY_LSP,
                                     CVA_RSTP_GLOBAL_BRIDGE_PRIORITY_LEN,
                                     stpBridgePriority);
    }

    return retVal;
}

/**
 * @brief This API is used to get switch priority.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] pStpBridgePriority Spanning Tree Protocol Bridge Priority. (RTK_STP_BRIDGE_PORT_PRIORITY_0
 *             to RTK_STP_BRIDGE_PORT_PRIORITY_61440) \n
 *             Indicates the bridge priority. \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_0: bridge priority 0 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_4096: bridge priority 4096 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_8192: bridge priority 8192 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_12288: bridge priority 12288 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_16384: bridge priority 16384 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_20480: bridge priority 20480 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_24576: bridge priority 24576 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_28672: bridge priority 28672 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_32768: bridge priority 32768 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_36864: bridge priority 36864 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_40960: bridge priority 40960 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_45056: bridge priority 45056 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_49152: bridge priority 49152 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_53248: bridge priority 53248 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_57344: bridge priority 57344 \n
 *             RTK_STP_BRIDGE_PORT_PRIORITY_61440: bridge priority 61440 \n
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get switch priority successfully.
 */
RtkApiRet cva_stp_bridge_priority_get(uint32 unit, RtkStpBdgPri *pStpBridgePriority)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(NULL == pStpBridgePriority)
    {
        retVal = (-RT_ERR_NULL_POINTER);
    }
    else
    {
        /* Get STP global state register. */
        retVal = cva_reg_field_read(unit,
                                    CVA_RSTP_GLOBAL,
                                    CVA_RSTP_GLOBAL_BRIDGE_PRIORITY_LSP,
                                    CVA_RSTP_GLOBAL_BRIDGE_PRIORITY_LEN,
                                    pStpBridgePriority);
    }

    return retVal;
}

/**
 * @brief This API is used to set forward delay.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] stpForwardDelay Spanning Tree Protocol Forward Delay Time (4 to 30) \n
 *            Indicates the forward delay time.
 * @param[out] None
 * @retval -RT_ERR_STP_FORWARDDELAY The input value of forward delay is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set forward delay successfully.
 */
RtkApiRet cva_stp_forward_delay_set(uint32 unit, RtkStpFowardDelay stpForwardDelay)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if((STP_FORWARD_DELAYMAX < stpForwardDelay) || (STP_FORWARD_DELAYMIN > stpForwardDelay))
    {
        retVal = (-RT_ERR_STP_FORWARDDELAY);
    }
    else
    {
        /* Set STP global state register. */
        retVal = cva_reg_field_write(unit,
                                     CVA_RSTP_GLOBAL,
                                     CVA_RSTP_GLOBAL_FORWARD_DELAY_LSP,
                                     CVA_RSTP_GLOBAL_FORWARD_DELAY_LEN,
                                     stpForwardDelay);
    }

    return retVal;
}

/**
 * @brief This API is used to get forward delay.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] pStpForwardDelay Spanning Tree Protocol Forward Delay Time (4 to 30) \n
 *             Indicates the forward delay time.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get forward delay successfully.
 */
RtkApiRet cva_stp_forward_delay_get(uint32 unit, RtkStpFowardDelay *pStpForwardDelay)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(NULL == pStpForwardDelay)
    {
        retVal = (-RT_ERR_NULL_POINTER);
    }
    else
    {
        /* Get STP global state register. */
        retVal = cva_reg_field_read(unit,
                                    CVA_RSTP_GLOBAL,
                                    CVA_RSTP_GLOBAL_FORWARD_DELAY_LSP,
                                    CVA_RSTP_GLOBAL_FORWARD_DELAY_LEN,
                                    pStpForwardDelay);

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Check if the value is valid. */
            if((STP_FORWARD_DELAYMAX < *pStpForwardDelay) || (STP_FORWARD_DELAYMIN > *pStpForwardDelay))
            {
                retVal = (-RT_ERR_FAILED);
            }
        }
    }

    return retVal;
}

/**
 * @brief This API is used to set max age time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] stpMaxAge Spanning Tree Protocol Max Age Time (6 to 40) \n
 *            Indicates the max age time.
 * @param[out] None
 * @retval -RT_ERR_STP_MAXAGE The input value of max age is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set max age time successfully.
 */
RtkApiRet cva_stp_max_age_set(uint32 unit, RtkStpMaxAge stpMaxAge)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if((STP_AGEMAX < stpMaxAge) || (STP_AGEMIN > stpMaxAge))
    {
        retVal = (-RT_ERR_STP_MAXAGE);
    }
    else
    {
        /* Set STP global state register. */
        retVal = cva_reg_field_write(unit,
                                     CVA_RSTP_GLOBAL,
                                     CVA_RSTP_GLOBAL_MAX_AGE_LSP,
                                     CVA_RSTP_GLOBAL_MAX_AGE_LEN,
                                     stpMaxAge);
    }

    return retVal;
}

/**
 * @brief This API is used to get max age.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] pStpMaxAge Spanning Tree Protocol Max Age Time (6 to 40) \n
 *             Indicates the max age time.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get max age time successfully.
 */
RtkApiRet cva_stp_max_age_get(uint32 unit, RtkStpMaxAge *pStpMaxAge)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(NULL == pStpMaxAge)
    {
        retVal = (-RT_ERR_NULL_POINTER);
    }
    else
    {
        /* Get STP global state register. */
        retVal = cva_reg_field_read(unit,
                                    CVA_RSTP_GLOBAL,
                                    CVA_RSTP_GLOBAL_MAX_AGE_LSP,
                                    CVA_RSTP_GLOBAL_MAX_AGE_LEN,
                                    pStpMaxAge);

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Check if the value is valid. */
            if((STP_AGEMAX < *pStpMaxAge) || (STP_AGEMIN > *pStpMaxAge))
            {
                retVal = (-RT_ERR_FAILED);
            }
        }
    }

    return retVal;
}

/**
 * @brief This API is used to set hello time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] stpHelloTime Spanning Tree Protocol Hello Time (1 to 2) \n
 *            Indicates the hello time.
 * @param[out] None
 * @retval -RT_ERR_STP_HELLOTIME The input value of hello time is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set hello time successfully.
 */
RtkApiRet cva_stp_hello_time_set(uint32 unit, RtkStpHelloTime stpHelloTime)
{
    uint32    fieldVal = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if((STP_HELLOTIMEMAX < stpHelloTime) || (STP_HELLOTIMEMIN > stpHelloTime))
    {
        retVal = (-RT_ERR_STP_HELLOTIME);
    }
    else
    {
        /* Transform the value of hello time for setting to register. */
        fieldVal = (stpHelloTime - 1u);

        /* Set STP global state register. */
        retVal = cva_reg_field_write(unit,
                                     CVA_RSTP_GLOBAL,
                                     CVA_RSTP_GLOBAL_HELLO_TIME_LSP,
                                     CVA_RSTP_GLOBAL_HELLO_TIME_LEN,
                                     fieldVal);
    }

    return retVal;
}

/**
 * @brief This API is used to get hello time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[out] pStpHelloTime Spanning Tree Protocol Hello Time (1 to 2) \n
 *             Indicates the hello time.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get hello time successfully.
 */
RtkApiRet cva_stp_hello_time_get(uint32 unit, RtkStpHelloTime *pStpHelloTime)
{
    uint32    fieldVal = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    /* Check if the parameter is valid. */
    if(NULL == pStpHelloTime)
    {
        retVal = (-RT_ERR_NULL_POINTER);
    }
    else
    {
        /* Get STP global state register. */
        retVal = cva_reg_field_read(unit,
                                    CVA_RSTP_GLOBAL,
                                    CVA_RSTP_GLOBAL_HELLO_TIME_LSP,
                                    CVA_RSTP_GLOBAL_HELLO_TIME_LEN,
                                    &fieldVal);

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Transform the value getting from register to hello time. */
            *pStpHelloTime = (fieldVal + 1u);
        }
    }

    return retVal;
}

/**@}*/ /* STP_BRIDGE_CONFIGURATION_ASIC */
/**@}*/ /* STP_BRIDGE_CONFIGURATION */

/**
 * @addtogroup STP_PORT_CONFIGURATION STP Port Configuration
 * @{
 */

/**
 * @addtogroup STP_PORT_CONFIGURATION_ASIC Low Level Driver
 * @brief Port Configuration Low Level Driver
 * @{
 */

/**
 * @brief This API is used to set spanning tree port states by instance.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] msti Multiple Spanning Tree Instance (0 to 14) \n
 *            Indicates the instance ID for setting port state.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for setting port state.
 * @param[in] portState Port State (N/A) \n
 *            Indicates the port state.
 * @param[out] None
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_MSTI The input value of instance ID is invalid.
 * @retval -RT_ERR_MSTP_STATE The input value of port state is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set port state by instance successfully.
 */
RtkApiRet cva_stp_port_state_set(uint32 unit, RtkStpMstiId msti, RtkPort portId, RtkStpPortState_e portState)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(MSTIMAX <= msti)
        {
            retVal = (-RT_ERR_MSTI);
            continue;
        }

        /* Check if the parameter is valid. */
        if((uint32)EM_STP_END <= (uint32)portState)
        {
            retVal = (-RT_ERR_MSTP_STATE);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Set STP port state register. */
        retVal = cva_reg_field_write(unit,
                                     (CVA_SPANNING_TREE_PORT_STATE_CONTROL0 + (msti * 4u)),
                                     ((uint32)CVA_SPANNING_TREE_PORT_STATE_CONTROL0_PORT0_STATE_LSP + (sysPort * (uint32)CVA_SPANNING_TREE_PORT_STATE_CONTROL0_PORT0_STATE_LEN)),
                                     (uint32)CVA_SPANNING_TREE_PORT_STATE_CONTROL0_PORT0_STATE_LEN,
                                     (uint32)portState);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get spanning tree port states by instance.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] msti Multiple Spanning Tree Instance (0 to 14) \n
 *            Indicates the instance ID for getting port state.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for getting port state.
 * @param[out] pPortState Port State (N/A) \n
 *             Indicates the port state.
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_MSTI The input value of instance ID is invalid.
 * @retval -RT_ERR_MSTP_STATE The port state is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get port state by instance successfully.
 */
RtkApiRet cva_stp_port_state_get(uint32 unit, RtkStpMstiId msti, RtkPort portId, RtkStpPortState_e *pPortState)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(MSTIMAX <= msti)
        {
            retVal = (-RT_ERR_MSTI);
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pPortState)
        {
            retVal = (-RT_ERR_NULL_POINTER);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Get STP port state register. */
        retVal = cva_reg_field_read(unit,
                                    (CVA_SPANNING_TREE_PORT_STATE_CONTROL0 + (msti * 4u)),
                                    ((uint32)CVA_SPANNING_TREE_PORT_STATE_CONTROL0_PORT0_STATE_LSP + (sysPort * (uint32)CVA_SPANNING_TREE_PORT_STATE_CONTROL0_PORT0_STATE_LEN)),
                                    (uint32)CVA_SPANNING_TREE_PORT_STATE_CONTROL0_PORT0_STATE_LEN,
                                    &fieldVal);

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Transform the value getting from register to port state. */
            if((uint32)EM_STP_DISCARDING == fieldVal)
            {
                *pPortState = EM_STP_DISCARDING;
            }
            else if((uint32)EM_STP_BLOCKING == fieldVal)
            {
                *pPortState = EM_STP_BLOCKING;
            }
            else if((uint32)EM_STP_LEARNING == fieldVal)
            {
                *pPortState = EM_STP_LEARNING;
            }
            else if((uint32)EM_STP_FORWARDING == fieldVal)
            {
                *pPortState = EM_STP_FORWARDING;
            }
            else
            {
                *pPortState = EM_STP_END;
                retVal      = (-RT_ERR_MSTP_STATE);
            }
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to set port priority.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for setting port priority.
 * @param[in] stpPortPriority Spanning Tree Protocol Port Priority (RTK_STP_PORT_PRIORITY_0 to RTK_STP_PORT_PRIORITY_240) \n
 *            Indicates the port priority. \n
 *            RTK_STP_PORT_PRIORITY_0: port priority 0 \n
 *            RTK_STP_PORT_PRIORITY_16: port priority 16  \n
 *            RTK_STP_PORT_PRIORITY_32: port priority 32  \n
 *            RTK_STP_PORT_PRIORITY_48: port priority 48  \n
 *            RTK_STP_PORT_PRIORITY_64: port priority 64  \n
 *            RTK_STP_PORT_PRIORITY_80: port priority 80  \n
 *            RTK_STP_PORT_PRIORITY_96: port priority 96  \n
 *            RTK_STP_PORT_PRIORITY_112: port priority 112 \n
 *            RTK_STP_PORT_PRIORITY_128: port priority 128 \n
 *            RTK_STP_PORT_PRIORITY_144: port priority 144 \n
 *            RTK_STP_PORT_PRIORITY_160: port priority 160 \n
 *            RTK_STP_PORT_PRIORITY_176: port priority 176 \n
 *            RTK_STP_PORT_PRIORITY_192: port priority 192 \n
 *            RTK_STP_PORT_PRIORITY_208: port priority 208 \n
 *            RTK_STP_PORT_PRIORITY_224: port priority 224 \n
 *            RTK_STP_PORT_PRIORITY_240: port priority 240 \n
 * @param[out] None
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_STP_PORT_PRIORITY The input value of port priority is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set port priority successfully.
 */
RtkApiRet cva_stp_port_priority_set(uint32 unit, RtkPort portId, RtkStpPortPri stpPortPriority)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(STP_BRIDGE_PORT_PRIORITYMAX < stpPortPriority)
        {
            retVal = (-RT_ERR_STP_PORT_PRIORITY);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P0_PORT_PRIORITY_LSP,
                                         CVA_RSTP_PORT0_P0_PORT_PRIORITY_LEN,
                                         stpPortPriority);
        }
        else
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P1_PORT_PRIORITY_LSP,
                                         CVA_RSTP_PORT0_P1_PORT_PRIORITY_LEN,
                                         stpPortPriority);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get port priority.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for getting port priority.
 * @param[out] pStpPortPriority Spanning Tree Protocol Port Priority (RTK_STP_PORT_PRIORITY_0 to RTK_STP_PORT_PRIORITY_240) \n
 *             Indicates the port priority. \n
 *             RTK_STP_PORT_PRIORITY_0: port priority 0 \n
 *             RTK_STP_PORT_PRIORITY_16: port priority 16  \n
 *             RTK_STP_PORT_PRIORITY_32: port priority 32  \n
 *             RTK_STP_PORT_PRIORITY_48: port priority 48  \n
 *             RTK_STP_PORT_PRIORITY_64: port priority 64  \n
 *             RTK_STP_PORT_PRIORITY_80: port priority 80  \n
 *             RTK_STP_PORT_PRIORITY_96: port priority 96  \n
 *             RTK_STP_PORT_PRIORITY_112: port priority 112 \n
 *             RTK_STP_PORT_PRIORITY_128: port priority 128 \n
 *             RTK_STP_PORT_PRIORITY_144: port priority 144 \n
 *             RTK_STP_PORT_PRIORITY_160: port priority 160 \n
 *             RTK_STP_PORT_PRIORITY_176: port priority 176 \n
 *             RTK_STP_PORT_PRIORITY_192: port priority 192 \n
 *             RTK_STP_PORT_PRIORITY_208: port priority 208 \n
 *             RTK_STP_PORT_PRIORITY_224: port priority 224 \n
 *             RTK_STP_PORT_PRIORITY_240: port priority 240 \n
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get port priority successfully.
 */
RtkApiRet cva_stp_port_priority_get(uint32 unit, RtkPort portId, RtkStpPortPri *pStpPortPriority)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pStpPortPriority)
        {
            retVal = (-RT_ERR_NULL_POINTER);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P0_PORT_PRIORITY_LSP,
                                        CVA_RSTP_PORT0_P0_PORT_PRIORITY_LEN,
                                        pStpPortPriority);
        }
        else
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit, (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P1_PORT_PRIORITY_LSP,
                                        CVA_RSTP_PORT0_P1_PORT_PRIORITY_LEN,
                                        pStpPortPriority);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to set port path cost.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for setting port path cost.
 * @param[in] stpPortPathCost Spanning Tree Protocol Port Path Cost (RTK_STP_PATHCOST_AUTO to RTK_STP_PATHCOST_2) \n
 *            Indicates the port path cost. \n
 *            RTK_STP_PATHCOST_AUTO: stands for auto, STP will decide the path cost according to port's speed and duplex \n
 *            RTK_STP_PATHCOST_200000000: stands for port speed 100Kbps, path cost=200000000 \n
 *            RTK_STP_PATHCOST_20000000: stands for port speed 1Mbps, path cost=20000000 \n
 *            RTK_STP_PATHCOST_2000000: stands for port speed 10Mbps Half duplex, path cost=2000000 \n
 *            RTK_STP_PATHCOST_1999999: stands for port speed 10Mbps Full duplex, path cost=1999999 \n
 *            RTK_STP_PATHCOST_1000000: stands for port speed 10Mbps Link aggregation, path cost=1000000 \n
 *            RTK_STP_PATHCOST_200000: stands for port speed 100Mbps Half duplex, path cost=200000 \n
 *            RTK_STP_PATHCOST_199999: stands for port speed 100Mbps Full duplex, path cost=199999 \n
 *            RTK_STP_PATHCOST_100000: stands for port speed 100Mbps Link aggregation, path cost=100000 \n
 *            RTK_STP_PATHCOST_20000: stands for port speed 1Gbps Full duplex, path cost=20000 \n
 *            RTK_STP_PATHCOST_19999: stands for port speed 1Gbps, path cost=19999 \n
 *            RTK_STP_PATHCOST_10000: stands for port speed 1Gbps Link aggregation, path cost=10000 \n
 *            RTK_STP_PATHCOST_2000: stands for port speed 10Gbps, path cost=2000 \n
 *            RTK_STP_PATHCOST_200: stands for port speed 100Gbps, path cost=200 \n
 *            RTK_STP_PATHCOST_20: stands for port speed 1Tbps, path cost=20 \n
 *            RTK_STP_PATHCOST_2: stands for port speed 10Tbps, path cost=2 \n
 * @param[out] None
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_STP_PORT_PATH_COST The input value of port path cost is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set port path cost successfully.
 */
RtkApiRet cva_stp_port_path_cost_set(uint32 unit, RtkPort portId, RtkStpPortPathCost stpPortPathCost)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(STP_PATHCOSTMAX < stpPortPathCost)
        {
            retVal = (-RT_ERR_STP_PORT_PATH_COST);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P0_PORT_PATH_COST_LSP,
                                         CVA_RSTP_PORT0_P0_PORT_PATH_COST_LEN,
                                         stpPortPathCost);
        }
        else
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P1_PORT_PATH_COST_LSP,
                                         CVA_RSTP_PORT0_P1_PORT_PATH_COST_LEN,
                                         stpPortPathCost);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get port path cost.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for getting port path cost.
 * @param[out] pStpPortPathCost Spanning Tree Protocol Port Path Cost (RTK_STP_PATHCOST_AUTO to RTK_STP_PATHCOST_2) \n
 *             Indicates the port path cost. \n
 *             RTK_STP_PATHCOST_AUTO: stands for auto, STP will decide the path cost according to port's speed and duplex \n
 *             RTK_STP_PATHCOST_200000000: stands for port speed 100Kbps, path cost=200000000 \n
 *             RTK_STP_PATHCOST_20000000: stands for port speed 1Mbps, path cost=20000000 \n
 *             RTK_STP_PATHCOST_2000000: stands for port speed 10Mbps Half duplex, path cost=2000000 \n
 *             RTK_STP_PATHCOST_1999999: stands for port speed 10Mbps Full duplex, path cost=1999999 \n
 *             RTK_STP_PATHCOST_1000000: stands for port speed 10Mbps Link aggregation, path cost=1000000 \n
 *             RTK_STP_PATHCOST_200000: stands for port speed 100Mbps Half duplex, path cost=200000 \n
 *             RTK_STP_PATHCOST_199999: stands for port speed 100Mbps Full duplex, path cost=199999 \n
 *             RTK_STP_PATHCOST_100000: stands for port speed 100Mbps Link aggregation, path cost=100000 \n
 *             RTK_STP_PATHCOST_20000: stands for port speed 1Gbps Full duplex, path cost=20000 \n
 *             RTK_STP_PATHCOST_19999: stands for port speed 1Gbps, path cost=19999 \n
 *             RTK_STP_PATHCOST_10000: stands for port speed 1Gbps Link aggregation, path cost=10000 \n
 *             RTK_STP_PATHCOST_2000: stands for port speed 10Gbps, path cost=2000 \n
 *             RTK_STP_PATHCOST_200: stands for port speed 100Gbps, path cost=200 \n
 *             RTK_STP_PATHCOST_20: stands for port speed 1Tbps, path cost=20 \n
 *             RTK_STP_PATHCOST_2: stands for port speed 10Tbps, path cost=2 \n
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get port path cost successfully.
 */
RtkApiRet cva_stp_port_path_cost_get(uint32 unit, RtkPort portId, RtkStpPortPathCost *pStpPortPathCost)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pStpPortPathCost)
        {
            retVal = (-RT_ERR_NULL_POINTER);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P0_PORT_PATH_COST_LSP,
                                        CVA_RSTP_PORT0_P0_PORT_PATH_COST_LEN,
                                        pStpPortPathCost);
        }
        else
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P1_PORT_PATH_COST_LSP,
                                        CVA_RSTP_PORT0_P1_PORT_PATH_COST_LEN,
                                        pStpPortPathCost);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to set port admin edge.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for setting the state of port admin edge.
 * @param[in] stpPortAdminEdge Spanning Tree Protocol Port Admin Edge State (DISABLED, ENABLED) \n
 *            Indicates the state of port admin edge. \n
 *            DISABLED: Disabled \n
 *            ENABLED: Enabled \n
 * @param[out] None
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_ENABLE The port admin edge state is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set port admin edge successfully.
 */
RtkApiRet cva_stp_port_admin_edge_set(uint32 unit, RtkPort portId, RtkEnable stpPortAdminEdge)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPort   sysPort  = 0u;

    /* Check if the parameter is valid. */
    if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
    {
        retVal = (-RT_ERR_PORT_ID);
    }
    else
    {
        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Set the value based on the state. */
        if(ENABLED == stpPortAdminEdge)
        {
            fieldVal = 1u;
            /* Calculate the offset for setting to STP port register. */
            if(0u == (sysPort % 2u))
            {
                /* Set STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P0_ADMIN_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P0_ADMIN_EDGE_PORT_LEN,
                                             fieldVal);
            }
            else
            {
                /* Set STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P1_ADMIN_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P1_ADMIN_EDGE_PORT_LEN,
                                             fieldVal);
            }
        }
        else if(DISABLED == stpPortAdminEdge)
        {
            fieldVal = 0u;
            /* Calculate the offset for setting to STP port register. */
            if(0u == (sysPort % 2u))
            {
                /* Set STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P0_ADMIN_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P0_ADMIN_EDGE_PORT_LEN,
                                             fieldVal);
            }
            else
            {
                /* Set STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P1_ADMIN_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P1_ADMIN_EDGE_PORT_LEN,
                                             fieldVal);
            }
        }
        else
        {
            /* The parameter is invalid. */
            retVal = (-RT_ERR_ENABLE);
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get port admin edge.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for getting the state of port admin edge.
 * @param[out] pStpPortAdminEdge Spanning Tree Protocol Port Admin Edge State (DISABLED, ENABLED) \n
 *             Indicates the state of port admin edge. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get port admin edge successfully.
 */
RtkApiRet cva_stp_port_admin_edge_get(uint32 unit, RtkPort portId, RtkEnable *pStpPortAdminEdge)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pStpPortAdminEdge)
        {
            retVal = (-RT_ERR_NULL_POINTER);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P0_ADMIN_EDGE_PORT_LSP,
                                        CVA_RSTP_PORT0_P0_ADMIN_EDGE_PORT_LEN,
                                        &fieldVal);
        }
        else
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P1_ADMIN_EDGE_PORT_LSP,
                                        CVA_RSTP_PORT0_P1_ADMIN_EDGE_PORT_LEN,
                                        &fieldVal);
        }

        if(RT_ERR_OK == retVal)
        {
            /* Transform the value getting from register to port admin edge state. */
            *pStpPortAdminEdge = ((uint32)ENABLED == fieldVal) ? ENABLED : DISABLED;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to set port auto edge.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for setting the state of port auto edge.
 * @param[in] stpPortAutoEdge Spanning Tree Protocol Port Auto Edge State (DISABLED, ENABLED) \n
 *            Indicates the state of port auto edge. \n
 *            DISABLED: Disabled \n
 *            ENABLED: Enabled \n
 * @param[out] None
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_ENABLE The port auto edge state is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set port auto edge successfully.
 */
RtkApiRet cva_stp_port_auto_edge_set(uint32 unit, RtkPort portId, RtkEnable stpPortAutoEdge)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPort   sysPort  = 0u;

    /* Check if the parameter is valid. */
    if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
    {
        retVal = (-RT_ERR_PORT_ID);
    }
    else
    {
        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];

        /* Set the value based on the state. */
        if(ENABLED == stpPortAutoEdge)
        {
            fieldVal = 1u;
            /* Calculate the offset for setting to STP port register. */
            if(0u == (sysPort % 2u))
            {
                /* Set STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P0_AUTO_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P0_AUTO_EDGE_PORT_LEN,
                                             fieldVal);
            }
            else
            {
                /* Set STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P1_AUTO_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P1_AUTO_EDGE_PORT_LEN,
                                             fieldVal);
            }
        }
        else if(DISABLED == stpPortAutoEdge)
        {
            fieldVal = 0u;
            /* Calculate the offset for setting to STP port register. */
            if(0u == (sysPort % 2u))
            {
                /* Get STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P0_AUTO_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P0_AUTO_EDGE_PORT_LEN,
                                             fieldVal);
            }
            else
            {
                /* Get STP port register. */
                retVal = cva_reg_field_write(unit,
                                             (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                             CVA_RSTP_PORT0_P1_AUTO_EDGE_PORT_LSP,
                                             CVA_RSTP_PORT0_P1_AUTO_EDGE_PORT_LEN,
                                             fieldVal);
            }
        }
        else
        {
            /* The parameter is invalid. */
            retVal = (-RT_ERR_ENABLE);
        }
    }

    return retVal;
}

/**
 * @brief This API is used to get port auto edge.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for getting the state of port auto edge.
 * @param[out] pStpPortAutoEdge Spanning Tree Protocol Port Auto Edge State (DISABLED, ENABLED) \n
 *             Indicates the state of port auto edge. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get port auto edge successfully.
 */
RtkApiRet cva_stp_port_auto_edge_get(uint32 unit, RtkPort portId, RtkEnable *pStpPortAutoEdge)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pStpPortAutoEdge)
        {
            retVal = (-RT_ERR_NULL_POINTER);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P0_AUTO_EDGE_PORT_LSP,
                                        CVA_RSTP_PORT0_P0_AUTO_EDGE_PORT_LEN,
                                        &fieldVal);
        }
        else
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P1_AUTO_EDGE_PORT_LSP,
                                        CVA_RSTP_PORT0_P1_AUTO_EDGE_PORT_LEN,
                                        &fieldVal);
        }

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Transform the value getting from register to port auto edge state. */
            *pStpPortAutoEdge = ((uint32)ENABLED == fieldVal) ? ENABLED : DISABLED;
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to set port p2p.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for setting p2p.
 * @param[in] stpPortP2p Spanning Tree Protocol Port Point to Point Type (N/A) \n
 *            Indicates the p2p type.
 * @param[out] None
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_INPUT The input value of port p2p is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Set port p2p successfully.
 */
RtkApiRet cva_stp_port_p2p_set(uint32 unit, RtkPort portId, RtkStpPortP2p_e stpPortP2p)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(EM_STP_P2P_END <= stpPortP2p)
        {
            retVal = (-RT_ERR_INPUT);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P0_P2P_PROT_LSP,
                                         CVA_RSTP_PORT0_P0_P2P_PROT_LEN,
                                         (uint32)stpPortP2p);
        }
        else
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P1_P2P_PROT_LSP,
                                         CVA_RSTP_PORT0_P1_P2P_PROT_LEN,
                                         (uint32)stpPortP2p);
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to get port p2p attribution.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for getting p2p.
 * @param[out] pStpPortP2p Spanning Tree Protocol Port Point to Point Type (N/A) \n
 *             Indicates the p2p type.
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval -RT_ERR_FAILED The p2p type is invalid.
 * @retval Others Refer to cva_reg_field_read() for the error.
 * @retval RT_ERR_OK Get port p2p successfully.
 */
RtkApiRet cva_stp_port_p2p_get(uint32 unit, RtkPort portId, RtkStpPortP2p_e *pStpPortP2p)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPort   sysPort  = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
        {
            retVal = (-RT_ERR_PORT_ID);
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pStpPortP2p)
        {
            retVal = (-RT_ERR_NULL_POINTER);
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P0_P2P_PROT_LSP,
                                        CVA_RSTP_PORT0_P0_P2P_PROT_LEN,
                                        &fieldVal);
        }
        else
        {
            /* Get STP port register. */
            retVal = cva_reg_field_read(unit,
                                        (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                        CVA_RSTP_PORT0_P1_P2P_PROT_LSP,
                                        CVA_RSTP_PORT0_P1_P2P_PROT_LEN,
                                        &fieldVal);
        }

        /* Get value of register successfully. */
        if(RT_ERR_OK == retVal)
        {
            /* Transform the value getting from register to p2p type. */
            if((uint32)EM_STP_P2P_AUTO == fieldVal)
            {
                *pStpPortP2p = EM_STP_P2P_AUTO;
            }
            else if((uint32)EM_STP_P2P_TURE == fieldVal)
            {
                *pStpPortP2p = EM_STP_P2P_TURE;
            }
            else if((uint32)EM_STP_P2P_FALSE == fieldVal)
            {
                *pStpPortP2p = EM_STP_P2P_FALSE;
            }
            else
            {
                *pStpPortP2p = EM_STP_P2P_END;
                retVal       = (-RT_ERR_FAILED);
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief This API is used to trigger port migration check.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used to indicate which switch to be accessed.
 * @param[in] portId Port Identification (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *            Indicates the port ID for setting migration check.
 * @param[out] None
 * @retval -RT_ERR_PORT_ID The input value of port is invalid.
 * @retval Others Refer to cva_reg_field_write() for the error.
 * @retval RT_ERR_OK Trigger port migration check successfully.
 * @note When migration check is done, the related register bit will be cleared to 0 automatically.
 */
RtkApiRet cva_stp_port_migration_check_set(uint32 unit, RtkPort portId)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    /* Check if the parameter is valid. */
    if(((uint32)CVA_RTK_USER_PORT_1ST > portId) || ((uint32)CVA_RTK_USER_PORT_END < portId))
    {
        retVal = (-RT_ERR_PORT_ID);
    }
    else
    {
        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)cva_g_userPort2sysPort[portId];
        /* Calculate the offset for setting to STP port register. */
        if(0u == (sysPort % 2u))
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P0_MIGRATION_CHECK_LSP,
                                         CVA_RSTP_PORT0_P0_MIGRATION_CHECK_LEN,
                                         1u);
        }
        else
        {
            /* Set STP port register. */
            retVal = cva_reg_field_write(unit,
                                         (CVA_RSTP_PORT0 + (4u * (sysPort / 2u))),
                                         CVA_RSTP_PORT0_P1_MIGRATION_CHECK_LSP,
                                         CVA_RSTP_PORT0_P1_MIGRATION_CHECK_LEN,
                                         1u);
        }
    }

    return retVal;
}

/**@}*/ /* STP_PORT_CONFIGURATION_ASIC */
/**@}*/ /* STP_PORT_CONFIGURATION */
        /**@}*/
