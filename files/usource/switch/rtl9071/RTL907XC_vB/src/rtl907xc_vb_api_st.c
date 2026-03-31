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

#include "rtl907xc_vb_asicdrv_st.h"
#include "rtl907xc_vb_api_tsn.h"
#include "rtl907xc_vb_api_ext.h"
/**
 * @addtogroup RTK_MODULE_ST_QBV ST/QBV
 * @{
 */

/**
 * @addtogroup TRANSMISSION_GATE Transmission Gate
 * @{
 */
/**
 * @addtogroup TRANSMISSION_GATE_ASIC Driver
 * @brief ST Low Level Driver
 * @{
 */
/**
 * @brief Initialize the ST to IEEE 802.1Q recommended value.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully initializes the ST to IEEE 802.1Q recommended value.
 * Others Please refer to cvb_drv_st_queue_max_sdu_set()  \n
 *                      cvb_drv_st_admin_gate_states_set() and cvb_drv_st_gate_hw_enabled_set().
 */
RtkApiRet cvb_st_init(uint32 unit)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    iterPort;
    uint32    iterQueue;

    do
    {
        /* Initialize Gate Parameter Table */
        /* queueMaxSDUTable */
        /* GateEnabled */
        /* AdminGateStates */
        for(iterPort = CVB_RTK_USER_PORT_1ST; iterPort <= CVB_RTK_USER_PORT_END; ++iterPort)
        {
            if(RT_ERR_OK != result)
            {
                break;
            }
            for(iterQueue = 0; iterQueue <= (uint32)CVB_TSN_MAX_QUEUE; ++iterQueue)
            {
                if(RT_ERR_OK != result)
                {
                    break;
                }
                /* Set it to max value. The MAC max SDU or stream max SDU will limit the SDU size. */
                CHK_FUN_CONTINUE(result, cvb_drv_st_queue_max_sdu_set(unit, cvb_g_userPort2sysPort[iterPort], iterQueue, 0x3FFFu));
            }

            /* Gate all open when disabled. */
            CHK_FUN_CONTINUE(result, cvb_drv_st_admin_gate_states_set(unit, cvb_g_userPort2sysPort[iterPort], 0xFFu));

            CHK_FUN_CONTINUE(result, cvb_drv_st_gate_hw_enabled_set(unit, cvb_g_userPort2sysPort[iterPort], DISABLE, 0u));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Set the Byte-Count related to the internal TX-interval parameter.  \n
 *      Note: When setting 5G as the port type, the CBS (Credit-Based Shaper) should be enabled on the port too.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[in] portType Port type (CVB_ST_PORT_TYPE_100BASE_T1 to CVB_ST_PORT_TYPE_END) \n
 *                     Port type of the switch.
 * @param[in] speed Port speed (CVB_RTK_PORT_SPEED_10M to CVB_RTK_PORT_SPEED_END) \n
 *                  Port speed of the switch.
 * @param[in] macsecIPG Macsec IPG (CVB_NO_MACSEC to CVB_MACSEC_IPG32)
 *                      Macsec configuration.
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the Byte-Count related to the internal TX-interval parameter.
 * Others Please refer to cvb_drv_st_byte_count_intf_chk(), cvb_drv_st_byte_count_t1_set(), cvb_drv_st_byte_count_fe_set(), \n
 *                 and cvb_drv_st_byte_count_xmii_set().
 */
RtkApiRet cvb_st_byte_count_set(uint32 unit, cvb_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, cvb_drv_st_byte_count_intf_chk(unit, portId, portType, macsecIPG));

        if((CVB_ST_PORT_TYPE_RTPG_1000BASE_T1 == portType) || (CVB_ST_PORT_TYPE_RTPG_100BASE_T1 == portType) || (CVB_ST_PORT_TYPE_100BASE_T1 == portType) || (CVB_ST_PORT_TYPE_10BASE_T1 == portType))
        {
            /* Set byte count value for port type t1 interface */
            CHK_FUN_CONTINUE(result, cvb_drv_st_byte_count_t1_set(unit, portId, portType, speed, macsecIPG));
        }
        else if(CVB_ST_PORT_TYPE_FAST_ETHERNET == portType)
        {
            /* Set byte count value for port type fast ethernet interface */
            CHK_FUN_CONTINUE(result, cvb_drv_st_byte_count_fe_set(unit, portId, speed, macsecIPG));
        }
        else
        {
            /* Set byte count value for port type xmii interface */
            CHK_FUN_CONTINUE(result, cvb_drv_st_byte_count_xmii_set(unit, portId, portType, speed, macsecIPG));
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief Get the Byte-Count related to the internal TX-interval parameter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] byteCount Byte count (CVB_BYTE_PRE_FRAME_10 to CVB_BYTE_PRE_FRAME_92)  \n
 *                       Byte count value.
 * @retval RT_ERR_OK Successfully gets the Byte-Count related to the internal TX-interval parameter.
 * @retval -RT_ERR_PORT_ID The port was out of range.
 * @retval -RT_ERR_NULL_POINTER Parameter byteCount is a NULL pointer.
 * Others Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_st_byte_count_get(uint32 unit, cvb_RtkPort portId, uint32 *byteCount)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == byteCount)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get byte count value for specified port number */
        CHK_FUN_CONTINUE(result, cvb_reg_field_read(unit, CVB_PORT0_QBV_BC0 + (uint32)(PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[portId]), 0, 14, byteCount));

    } while(0u == 1u);

    return result;
}

/**
 * @brief To enable/disable the transmission gate on a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[in] enable Enable (0 to 1)  \n
 *                   Enable/disable transmission gate on a specific port. (1: enable, 0: disable)
 * @param[in] isImage Configure mode (0 to 1) \n
 *                    Image configure mode or runtime config mode. (1: image mode, 0: runtime mode)
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the transmission gate enable state on a specific port.
 * @retval -RT_ERR_PORT_ID The portId was out of range.
 * @retval -RT_ERR_INPUT Parameter enable is neither 0 nor 1.
 * Others Please refer to cvb_drv_st_preconfig_chk(), cvb_drv_st_gate_enabled_set().
 */
RtkApiRet cvb_st_gate_enable_set(uint32 unit, cvb_RtkPort portId, uint32 enable)
{
    RtkApiRet result  = RT_ERR_OK;
    uint32    isImage = CVB_DISABLED;
    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(((uint32)DISABLE != enable) && ((uint32)ENABLE != enable))
        {
            result = -RT_ERR_INPUT;
            continue;
        }
        if(1u == (enable & 0x1u))
        {
            /* Check port preconfig at runtime */
            CHK_FUN_CONTINUE(result, cvb_drv_st_preconfig_chk(unit));
        }
        else
        {
            /* Check if the configuration is complete. */
            CHK_FUN_CONTINUE(result, cvb_drv_st_isdone_chk(unit));
        }
        /* Enable or disable the gate for the specified port */
        CHK_FUN_CONTINUE(result, cvb_drv_st_gate_enabled_set(unit, portId, enable, isImage));
    } while(0u == 1u);

    return result;
}

/**
 * @brief Get the transmission gate state on a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] enable Enable (0 to 1)  \n
 *                    Transmission gate state is enabled (1) or disabled (0).
 * @retval RT_ERR_OK Successfully gets the transmission gate state on a specific port
 * @retval -RT_ERR_PORT_ID The portId was out of range.
 * @retval -RT_ERR_NULL_POINTER Parameter enable is a NULL pointer.
 * Others Please refer to cvb_drv_st_gate_enabled_get().
 */
RtkApiRet cvb_st_gate_enable_get(uint32 unit, cvb_RtkPort portId, uint32 *enable)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == enable)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get the gate enable state or disable state for the specified port */
        CHK_FUN_CONTINUE(result, cvb_drv_st_gate_enabled_get(unit, cvb_g_userPort2sysPort[portId], enable));

    } while(0u == 1u);

    return result;
}

/**
 * @brief Configure the transmission gate on a specific port using the provided input parameter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[in] baseTime Base time (N/A) \n
 *                     Admin base time as defined in the standard.
 * @param[in] cycleTimeExtension Cycle time extension (0 to 2^32)  \n
 *                               Admin cycle time extension as defined in the standard.
 * @param[in] gateControlList Gate control list (N/A) \n
 *                            Admin gate control list, consists of 128 entries, each includes gate state and interval.
 * @param[in] controlListLength Control list length (2 to 128)  \n
 *                              Admin gate control list length.
 * @param[out] None
 * @retval RT_ERR_OK Successfully configures the transmission gate on a specific port using the provided input parameter.
 * Others Please refer to cvb_drv_st_gt_conf_set_intf_chk(), cvb_drv_st_preconfig_chk(), cvb_drv_st_gate_config_set().
 */
RtkApiRet cvb_st_gate_config_set(uint32 unit, cvb_RtkPort portId, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension,
                                 const StGateControlListParam_t gateControlList[], uint32 controlListLength)
{
    RtkApiRet result;
    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, cvb_drv_st_gt_conf_set_intf_chk(portId, baseTime, cycleTimeExtension, gateControlList, controlListLength));
        /* Check port preconfig at runtime */
        CHK_FUN_CONTINUE(result, cvb_drv_st_preconfig_chk(unit));
        /* Set transmission gate config */
        CHK_FUN_CONTINUE(result, cvb_drv_st_gate_config_set(unit, cvb_g_userPort2sysPort[portId], baseTime, cycleTimeExtension, gateControlList, controlListLength));

    } while(0u == 1u);

    return result;
}
/**
 * @brief Get the admin configuration parameters.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] baseTime Base time (N/A) \n
 *                      Admin base time as defined in the standard.
 * @param[out] cycleTime Cycle time (2 to 2^32)  \n
 *                       Admin cycle time as defined in the standard.
 * @param[out] cycleTimeExtension Cycle time extension (2 to 2^32)  \n
 *                                Admin cycle time extension as defined in the standard.
 * @param[out] gateControlList Gate control list (N/A) \n
 *                             Admin gate control list.
 * @param[out] controlListLength Control list length (2 to 128)  \n
 *                               Admin gate control list length.
 * @retval RT_ERR_OK The function is done.
 * Others Please refer to cvb_drv_st_gt_conf_get_intf_chk(), cvb_drv_st_admin_base_time_get(), cvb_drv_st_ad_cycle_time_ext_get()  \n
 *                 cvb_drv_st_ad_ctrl_lst_len_get(), cvb_drv_st_admin_ctrl_list_get()  \n
 *                 and cvb_drv_st_admin_cycle_time_get().
 */
RtkApiRet cvb_st_admin_gate_config_get(uint32 unit, cvb_RtkPort portId, RtkTsnPtpTime_t *baseTime, RtkTsnCycleTime_t *cycleTime,
                                       uint32 *cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 *controlListLength)
{
    RtkApiRet result;

    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, cvb_drv_st_gt_conf_get_intf_chk(portId, baseTime, cycleTime, cycleTimeExtension, gateControlList, controlListLength));

        /* Get admin base time */
        CHK_FUN_CONTINUE(result, cvb_drv_st_admin_base_time_get(unit, cvb_g_userPort2sysPort[portId], baseTime));

        /* Get admin cycle time extension */
        CHK_FUN_CONTINUE(result, cvb_drv_st_ad_cycle_time_ext_get(unit, cvb_g_userPort2sysPort[portId], cycleTimeExtension));

        /* Get admin control list length */
        CHK_FUN_CONTINUE(result, cvb_drv_st_ad_ctrl_lst_len_get(unit, cvb_g_userPort2sysPort[portId], controlListLength));

        /* We might retrieve the incorrect GCL length (0), but we can't skip processing get-procedure */
        if((*controlListLength >= CVB_ST_SUPPORT_LIST_MIN) && (*controlListLength <= (uint32)CVB_ST_SUPPORT_LIST_MAX))
        {
            /* Get admin gate control list */
            CHK_FUN_CONTINUE(result, cvb_drv_st_admin_ctrl_list_get(unit, cvb_g_userPort2sysPort[portId], gateControlList, *controlListLength));
        }
        /* Get admin cycle time */
        CHK_FUN_CONTINUE(result, cvb_drv_st_admin_cycle_time_get(unit, cvb_g_userPort2sysPort[portId], cycleTime));

    } while(0u == 1u);

    return result;
}

/**
 * @brief Get the operate configuration  parameters.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] baseTime Base time (N/A)  \n
 *                      Operate base time as defined in the standard.
 * @param[out] cycleTime Cycle time (2 to 2^32)  \n
 *                       Operate cycle time as defined in the standard.
 * @param[out] cycleTimeExtension Cycle time extension (2 to 2^32)  \n
 *                                Operate cycle time extension as defined in the standard.
 * @param[out] gateControlList Gate control list (N/A) \n
 *                             Operate gate control list.
 * @param[out] controlListLength Control list length (2 to 128)  \n
 *                               Operate gate control list length.
 * @retval RT_ERR_OK Successfully gets the operate configuration  parameters.
 * Others Please refer to cvb_drv_st_gt_conf_get_intf_chk(), cvb_drv_st_oper_base_time_get(), cvb_drv_st_op_cycle_time_ext_get(),  \n
 *                 cvb_drv_st_op_ctrl_lst_len_get(), cvb_drv_st_oper_ctrl_list_get and  \n
 *                 cvb_drv_st_oper_cycle_time_get().
 */
RtkApiRet cvb_st_oper_gate_config_get(uint32 unit, cvb_RtkPort portId, RtkTsnPtpTime_t *baseTime, RtkTsnCycleTime_t *cycleTime,
                                      uint32 *cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 *controlListLength)
{
    RtkApiRet result;

    do
    {
        /* Interface error check */
        CHK_FUN_CONTINUE(result, cvb_drv_st_gt_conf_get_intf_chk(portId, baseTime, cycleTime, cycleTimeExtension, gateControlList, controlListLength));

        /* Get oper base time */
        CHK_FUN_CONTINUE(result, cvb_drv_st_oper_base_time_get(unit, cvb_g_userPort2sysPort[portId], baseTime));

        /* Get oper cycle time extension */
        CHK_FUN_CONTINUE(result, cvb_drv_st_op_cycle_time_ext_get(unit, cvb_g_userPort2sysPort[portId], cycleTimeExtension));

        /* Get oper control list length */
        CHK_FUN_CONTINUE(result, cvb_drv_st_op_ctrl_lst_len_get(unit, cvb_g_userPort2sysPort[portId], controlListLength));

        /* We might retrieve the incorrect GCL length (0), but we can't skip processing get-procedure */
        if((*controlListLength >= CVB_ST_SUPPORT_LIST_MIN) && (*controlListLength <= (uint32)CVB_ST_SUPPORT_LIST_MAX))
        {
            /* Get oper gate control list */
            CHK_FUN_CONTINUE(result, cvb_drv_st_oper_ctrl_list_get(unit, cvb_g_userPort2sysPort[portId], gateControlList, *controlListLength));
        }
        /* Get oper cycle time */
        CHK_FUN_CONTINUE(result, cvb_drv_st_oper_cycle_time_get(unit, cvb_g_userPort2sysPort[portId], cycleTime));

    } while(0u == 1u);

    return result;
}
/**
 * @brief Set the admin gate state on a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[in] adminGateState Admin gate state (0 to 0xFF)  \n
 *                           Admin gate state as defined in the standard.
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the admin gate state on a specific port.
 * @retval -RT_ERR_PORT_ID The port ID was out of range.
 * @retval -RT_ERR_OUT_OF_RANGE Input is out of range or invalid.
 * Others Please refer to cvb_drv_st_admin_gate_states_set().
 */
RtkApiRet cvb_st_admin_gate_state_set(uint32 unit, cvb_RtkPort portId, uint32 adminGateState)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(adminGateState > 0xFFu)
        {
            result = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set admin gate state */
        CHK_FUN_CONTINUE(result, cvb_drv_st_admin_gate_states_set(unit, cvb_g_userPort2sysPort[portId], adminGateState));

    } while(0u == 1u);

    return result;
}

/**
 * @brief Get the admin gate state on a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] adminGateState Admin gate state (0 to 0xFF)  \n
 *                            Admin gate state as defined in the standard.
 * @retval RT_ERR_OK Successfully gets the admin gate state on a specific port.
 * @retval -RT_ERR_PORT_ID The port ID was out of range.
 * @retval -RT_ERR_NULL_POINTER Parameter adminGateState is a NULL pointer.
 * Others Please refer to cvb_drv_st_admin_gate_states_get().
 */
RtkApiRet cvb_st_admin_gate_state_get(uint32 unit, cvb_RtkPort portId, uint32 *adminGateState)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(adminGateState == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get admin gate state */
        CHK_FUN_CONTINUE(result, cvb_drv_st_admin_gate_states_get(unit, cvb_g_userPort2sysPort[portId], adminGateState));

    } while(0u == 1u);

    return result;
}

/**
 * @brief Get the operate gate state on a specific port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] operGateState Oper gate state (0 to 0xFF)  \n
 *                           Operate gate state as defined in the standard.
 * @retval RT_ERR_OK Successfully gets the operate gate state on a specific port.
 * @retval -RT_ERR_PORT_ID The port ID was out of range.
 * @retval -RT_ERR_NULL_POINTER Parameter operGateState is a NULL pointer.
 * @retval Others Please refer to cvb_drv_st_oper_gate_states_get().
 */
RtkApiRet cvb_st_oper_gate_state_get(uint32 unit, cvb_RtkPort portId, uint32 *operGateState)
{
    RtkApiRet result = RT_ERR_OK;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(operGateState == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get oper gate state */
        CHK_FUN_CONTINUE(result, cvb_drv_st_oper_gate_states_get(unit, cvb_g_userPort2sysPort[portId], operGateState));

    } while(0u == 1u);

    return result;
}
/**
 * @brief Get the configuration pending flag.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] configPending Config pending (0 to 1)  \n
 *                           ConfigPending as defined in the standard.
 * @retval RT_ERR_OK Successfully gets the configuration pending flag.
 * @retval -RT_ERR_PORT_ID The port ID was out of range.
 * @retval -RT_ERR_NULL_POINTER Parameter configPending is a NULL pointer.
 * @retval Others Please refer to cvb_drv_st_config_pending_get().
 */
RtkApiRet cvb_st_config_pending_get(uint32 unit, cvb_RtkPort portId, uint32 *configPending)
{
    RtkApiRet result;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(configPending == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get config pending value */
        CHK_FUN_CONTINUE(result, cvb_drv_st_config_pending_get(unit, cvb_g_userPort2sysPort[portId], configPending));

    } while(0u == 1u);

    return result;
}
/**@}*/ /* TRANSMISSION_GATE_ASIC */
/**@}*/ /* TRANSMISSION_GATE */

/**
 * @addtogroup QUEUEMAXSDU QueueMaxSDU
 * @{
 */
/**
 * @addtogroup QUEUEMAXSDU_ASIC Low Level Driver
 * @brief ST Low Level Driver
 * @{
 */
/**
 * @brief Set the maximum queue SDU on a specific port with the given input parameter list.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[in] queueMaxSdu Queue max SDU (0~16383)  \n
 *                        Queue max SDU value for each queue.
 * @param[out] None
 * @retval RT_ERR_OK Successfully sets the maximum queue SDU on a specific port using the provided input parameters.
 * @retval -RT_ERR_PORT_ID The port ID was out of range.
 * @retval -RT_ERR_NULL_POINTER Parameter queueMaxSdu is a NULL pointer.
 * @retval Others Please refer to cvb_drv_st_queue_max_sdu_set().
 */
RtkApiRet cvb_st_queue_max_sdu_set(uint32 unit, cvb_RtkPort portId, const uint32 *queueMaxSdu)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    iteratorQueue;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        if(queueMaxSdu == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        for(iteratorQueue = 0; iteratorQueue <= (uint32)CVB_TSN_MAX_QUEUE; ++iteratorQueue)
        {
            /* Set max SDU for each queue */
            CHK_FUN_BREAK(result, cvb_drv_st_queue_max_sdu_set(unit, cvb_g_userPort2sysPort[portId], iteratorQueue, queueMaxSdu[iteratorQueue]));
        }
    } while(0u == 1u);

    return result;
}

/**
 * @brief Get the maximum queue SDU on a specific port with the given input parameter list.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Switch port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH)  \n
 *                   Egress port ID.
 * @param[out] queueMaxSdu Queue max SDU (0~16383)  \n
 *                         Queue max SDU value for each queue.
 * @retval RT_ERR_OK Successfully gets the maximum queue SDU on a specific port using the provided input parameters.
 * @retval -RT_ERR_PORT_ID The port ID was out of range.
 * @retval -RT_ERR_NULL_POINTER Parameter queueMaxSdu is a NULL pointer.
 * @retval Others Please refer to cvb_drv_st_queue_max_sdu_get().
 */
RtkApiRet cvb_st_queue_max_sdu_get(uint32 unit, cvb_RtkPort portId, uint32 *queueMaxSdu)
{
    RtkApiRet result = RT_ERR_OK;
    uint32    iteratorQueue;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > portId) || (CVB_RTK_USER_PORT_END < portId))
        {
            result = -RT_ERR_PORT_ID;
            continue;
        }

        for(iteratorQueue = 0; iteratorQueue < (uint32)CVB_TSN_NUMBER_OF_QUEUE; ++iteratorQueue)
        {
            /* Get max SDU for each queue */
            CHK_FUN_BREAK(result, cvb_drv_st_queue_max_sdu_get(unit, cvb_g_userPort2sysPort[portId], iteratorQueue, &queueMaxSdu[iteratorQueue]));
        }
    } while(0u == 1u);

    return result;
}
/**@}*/ /* QUEUEMAXSDU_ASIC */
/**@}*/ /* QUEUEMAXSDU */
/**@}*/ /* ST/QBV */
