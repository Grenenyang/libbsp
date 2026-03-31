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

#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_asicdrv_storm_ctl.h"
#include "rtkas_api_ext.h"

/**
 * @addtogroup RTK_MODULE_STORM_CTRL Storm Control
 * @{
 */

/**
 * @addtogroup STORM_BROADCAST Storm Control Broadcast
 * @{
 */

/**
 * @addtogroup STORM_BROADCAST_ASIC Low Level Driver
 * @brief Storm Control Broadcast Low Level Driver
 * @{
 */

/**
 * @brief Set broadcast storm control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for setting the state of broadcast storm control.
 * @param[in] enable Enable State (DISABLED, ENABLED) \n
 *            Indicate the state of broadcast storm control. \n
 *            DISABLED: Disabled \n
 *            ENABLED: Enabled \n
 * @param[out] None
 * @retval RT_ERR_OK Set broadcast storm control successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_ENABLE The enable state is invalid.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API enables or disables the storm control function for broadcast traffic.
 */
RtkApiRet dva_storm_ctl_broadcast_set(uint32 unit, RtkPort port, RtkEnable enable)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(RTK_ENABLE_END <= enable)
        {
            ret = -RT_ERR_ENABLE;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Set the value based on the state. */
        if(DISABLED == enable)
        {
            /* Set suppress broadcast configure registers. */
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_ENABLE_LEN, 0x0u));
        }
        else
        {
            /* Set suppress broadcast configure registers. */
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_BC_ENABLE_LSP,
                                                      DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_BC_ENABLE_LEN, 0x1u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SCMETER_INC_IFG_LSP,
                                                      DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SCMETER_INC_IFG_LEN, 0x1u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_ENABLE_LEN, 0x1u));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get broadcast storm control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for getting the state of broadcast storm control.
 * @param[out] pEnable Enable State (DISABLED, ENABLED) \n
 *             Indicate the state of broadcast storm control. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @retval RT_ERR_OK Get broadcast storm control successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets status of the storm control function for broadcast traffic.
 */
RtkApiRet dva_storm_ctl_broadcast_get(uint32 unit, RtkPort port, RtkEnable *pEnable)
{
    uint32    sys     = 0u;
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pEnable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get suppress broadcast configure registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_BC_ENABLE_LSP,
                                                 DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_BC_ENABLE_LEN, &sys));
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_ENABLE_LEN, &val));

        /* Transform the value getting from register to broadcast storm state. */
        *pEnable = (0x1u == (sys & val)) ? ENABLED : DISABLED;
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* STORM_BROADCAST_ASIC */
/**@}*/ /* STORM_BROADCAST */

/**
 * @addtogroup STORM_BROADCAST_RATE Storm Control Broadcast Rate
 * @{
 */

/**
 * @addtogroup STORM_BROADCAST_RATE_ASIC Low Level Driver
 * @brief Storm Control Broadcast Rate Low Level Driver
 * @{
 */

/**
 * @brief Set broadcast storm control rate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for setting the broadcast storm unit and rate.
 * @param[in] stormUnit Storm Unit of Rate (N/A) \n
 *            Indicate the broadcast storm unit of rate. \n
 * @param[in] rate Limited Rate (0 to 67108863) \n
 *            Indicate the limited rate for broadcast storm.
 * @param[out] None
 * @retval RT_ERR_OK Set broadcast storm control rate successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_OUT_OF_RANGE The storm unit or rate is out of range.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the limited traffic rate for broadcast storm.
 *       The broadcast traffic is dropped when its traffic level is exceeded.
 */
RtkApiRet dva_storm_ctl_broadcast_rate_set(uint32 unit, RtkPort port, RtkStormUnit_e stormUnit, uint32 rate)
{
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(EM_STORM_UNIT_END <= stormUnit)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(DVA_STORM_MAX_RATE < rate)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Set suppress broadcast configure and rate registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                  DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_BUNIT_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_BUNIT_LEN, (uint32)stormUnit));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_RATE + (sysPort * PERPORT_REG_OFFSET),
                                                  DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_RATE_P_BC_RATE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_RATE_P_BC_RATE_LEN, rate));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get broadcast storm control rate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for getting the broadcast storm unit and rate.
 * @param[out] pStormUnit Storm Unit of Rate (N/A) \n
 *             Indicate the broadcast storm unit of rate.
 * @param[out] pRate Limited Rate (0 to 67108863) \n
 *             Indicate the limited rate for broadcast storm.
 * @retval RT_ERR_OK Get broadcast storm control rate successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the limited traffic rate for broadcast storm.
 *       The broadcast traffic is dropped when its traffic level is exceeded.
 */
RtkApiRet dva_storm_ctl_broadcast_rate_get(uint32 unit, RtkPort port, RtkStormUnit_e *pStormUnit, uint32 *pRate)
{
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pStormUnit) || (NULL == pRate))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get suppress broadcast configure and rate registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_BUNIT_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_CONFIGURE_P_BC_BUNIT_LEN, &val));
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_RATE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_RATE_P_BC_RATE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_BROADCAST_RATE_P_BC_RATE_LEN, pRate));

        /* Transform the value getting from register to broadcast storm unit. */
        *pStormUnit = (val == 0u) ? EM_STORM_UNIT_PPS : EM_STORM_UNIT_BPS;
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* STORM_BROADCAST_RATE_ASIC */
/**@}*/ /* STORM_BROADCAST_RATE */

/**
 * @addtogroup STORM_MULTICAST Storm Control Multicast
 * @{
 */

/**
 * @addtogroup STORM_MULTICAST_ASIC Low Level Driver
 * @brief Storm Control Multicast Low Level Driver
 * @{
 */

/**
 * @brief Set multicast storm control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for setting the state of multicast storm control.
 * @param[in] enable Enable State (DISABLED, ENABLED) \n
 *            Indicate the state of multicast storm control. \n
 *            DISABLED: Disabled \n
 *            ENABLED: Enabled \n
 * @param[in] type Multicast Type (MC_TYPE_UNKNOWN, MC_TYPE_BOTH) \n
 *            Indicate the type of multicast storm. \n
 *            MC_TYPE_UNKNOWN: Unknown multicast only \n
 *            MC_TYPE_BOTH: Both known and unknown multicast
 * @param[out] None
 * @retval RT_ERR_OK Set multicast storm control successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_ENABLE The enable state is invalid.
 * @retval -RT_ERR_OUT_OF_RANGE The multicast type is invalid.
 * @retval Others Refer to dva_drv_storm_ctl_multicast_set() for the error.
 * @note This API enables and disables the storm control function for multicast traffic.
 */
RtkApiRet dva_storm_ctl_multicast_set(uint32 unit, RtkPort port, RtkEnable enable, uint8 type)
{
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(RTK_ENABLE_END <= enable)
        {
            ret = -RT_ERR_ENABLE;
            continue;
        }

        /* Check if the parameter is valid. */
        if((MC_TYPE_UNKNOWN != type) && (MC_TYPE_BOTH != type))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Set suppress multicast configure and rate configurations. */
        CHK_FUN_CONTINUE(ret, dva_drv_storm_ctl_multicast_set(unit, sysPort, enable, type));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get multicast storm control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for getting the state of multicast storm control.
 * @param[out] pEnable Enable State (DISABLED, ENABLED) \n
 *             Indicate the state of multicast storm control. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @param[out] pType Multicast Type (MC_TYPE_UNKNOWN, MC_TYPE_BOTH) \n
 *             Indicate the type of multicast storm. \n
 *             MC_TYPE_UNKNOWN: Unknown multicast only \n
 *             MC_TYPE_BOTH: Both known and unknown multicast
 * @retval RT_ERR_OK Get multicast storm control successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets status of the storm control function for multicast traffic.
 */
RtkApiRet dva_storm_ctl_multicast_get(uint32 unit, RtkPort port, RtkEnable *pEnable, uint8 *pType)
{
    uint32    sys     = 0u;
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pEnable) || (NULL == pType))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get suppress multicast configure registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_MC_ENABLE_LSP,
                                                 DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_MC_ENABLE_LEN, &sys));
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_ENABLE_LEN, &val));
        /* Transform the value getting from register to multicast storm state. */
        *pEnable = (0x1u == (sys & val)) ? ENABLED : DISABLED;

        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_TYPE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_TYPE_LEN, &val));
        *pType = (uint8)val;
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* STORM_MULTICAST_ASIC */
/**@}*/ /* STORM_MULTICAST */

/**
 * @addtogroup STORM_MULTICAST_RATE Storm Control Multicast Rate
 * @{
 */

/**
 * @addtogroup STORM_MULTICAST_RATE_ASIC Low Level Driver
 * @brief Storm Control Multicast Rate Low Level Driver
 * @{
 */
/**
 * @brief Set multicast storm control rate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for setting the multicast storm unit and rate.
 * @param[in] stormUnit Storm Unit of Rate (N/A) \n
 *            Indicate the multicast storm unit of rate.
 * @param[in] rate Limited Rate (0 to 67108863) \n
 *            Indicate the limited rate for multicast storm.
 * @param[out] None
 * @retval RT_ERR_OK Set multicast storm control rate successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_OUT_OF_RANGE The storm unit or rate is out of range.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the limited traffic rate for multicast storm.
 *       The multicast traffic is dropped when its traffic level is exceeded.
 */
RtkApiRet dva_storm_ctl_multicast_rate_set(uint32 unit, RtkPort port, RtkStormUnit_e stormUnit, uint32 rate)
{
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(EM_STORM_UNIT_END <= stormUnit)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(DVA_STORM_MAX_RATE < rate)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Set suppress multicast configure and rate registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                  DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_BUNIT_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_BUNIT_LEN, (uint32)stormUnit));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_RATE + (sysPort * PERPORT_REG_OFFSET),
                                                  DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_RATE_P_MC_RATE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_RATE_P_MC_RATE_LEN, rate));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get multicast storm control rate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for getting the broadcast storm unit and rate.
 * @param[out] pStormUnit Storm Unit of Rate (N/A)
 *             Indicate the multicast storm unit of rate. \n
 * @param[out] pRate Limited Rate (0 to 67108863) \n
 *             Indicate the limited rate for multicast storm.
 * @retval RT_ERR_OK Get multicast storm control rate successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the limited traffic rate for multicast storm.
 *       The multicast traffic is dropped when its traffic level is exceeded.
 */
RtkApiRet dva_storm_ctl_multicast_rate_get(uint32 unit, RtkPort port, RtkStormUnit_e *pStormUnit, uint32 *pRate)
{
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pStormUnit) || (NULL == pRate))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get suppress multicast configure and rate registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_BUNIT_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_BUNIT_LEN, &val));
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_RATE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_RATE_P_MC_RATE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_RATE_P_MC_RATE_LEN, pRate));

        /* Transform the value getting from register to multicast storm unit. */
        *pStormUnit = (val == 0u) ? EM_STORM_UNIT_PPS : EM_STORM_UNIT_BPS;
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* STORM_MULTICAST_RATE_ASIC */
/**@}*/ /* STORM_MULTICAST_RATE */

/**
 * @addtogroup STORM_UNICAST Storm Control Unicast
 * @{
 */

/**
 * @addtogroup STORM_UNICAST_ASIC Low Level Driver
 * @brief Storm Control Unicast Low Level Driver
 * @{
 */

/**
 * @brief Set unicast storm control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for setting the state of unicast storm control.
 * @param[in] enable Enable State (DISABLED, ENABLED) \n
 *            Indicate the state of unicast storm control. \n
 *            DISABLED: Disabled \n
 *            ENABLED: Enabled \n
 * @param[out] None
 * @retval RT_ERR_OK Set unicast storm control successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_ENABLE The enable state is invalid.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API enables and disables the storm control function for unknown unicast traffic.
 */
RtkApiRet dva_storm_ctl_unicast_set(uint32 unit, RtkPort port, RtkEnable enable)
{
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(RTK_ENABLE_END <= enable)
        {
            ret = -RT_ERR_ENABLE;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Set the value based on the state. */
        if(DISABLED == enable)
        {
            /* Set suppress unicast configure registers. */
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_ENABLE_LEN, 0x0u));
        }
        else
        {
            /* Set suppress unicast configure registers. */
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_UNUA_ENABLE_LSP,
                                                      DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_UNUA_ENABLE_LEN, 0x1u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SCMETER_INC_IFG_LSP,
                                                      DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SCMETER_INC_IFG_LEN, 0x1u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_ENABLE_LEN, 0x1u));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get unicast storm control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for getting the state of unicast storm control.
 * @param[out] pEnable Enable State (DISABLED, ENABLED) \n
 *             Indicate the state of unicast storm control. \n
 *             DISABLED: Disabled \n
 *             ENABLED: Enabled \n
 * @retval RT_ERR_OK Get unicast storm control successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets status of the storm control function for unknown unicast traffic.
 */
RtkApiRet dva_storm_ctl_unicast_get(uint32 unit, RtkPort port, RtkEnable *pEnable)
{
    uint32    sys     = 0u;
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pEnable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get suppress unicast configure registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_UNUA_ENABLE_LSP,
                                                 DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_UNUA_ENABLE_LEN, &sys));
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_ENABLE_LEN, &val));

        /* Transform the value getting from register to unicast storm state. */
        *pEnable = (0x1u == (sys & val)) ? ENABLED : DISABLED;
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* STORM_UNICAST_ASIC */
/**@}*/ /* STORM_UNICAST */

/**
 * @addtogroup STORM_UNICAST_RATE Storm Control Unicast Rate
 * @{
 */

/**
 * @addtogroup STORM_UNICAST_RATE_ASIC Low Level Driver
 * @brief Storm Control Unicast Rate Low Level Driver
 * @{
 */

/**
 * @brief Set unicast storm control rate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for setting the unicast storm unit and rate.
 * @param[in] stormUnit Storm Unit of Rate (N/A) \n
 *            Indicate the unicast storm unit of rate.
 * @param[in] rate Limited Rate (0 to 67108863) \n
 *            Indicate the limited rate for unicast storm.
 * @param[out] None
 * @retval RT_ERR_OK Set unicast storm control rate successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_OUT_OF_RANGE The storm unit or rate is out of range.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the limited traffic rate for unknown unicast storm.
 *       The unknown unicast traffic is dropped when its traffic level is exceeded.
 */
RtkApiRet dva_storm_ctl_unicast_rate_set(uint32 unit, RtkPort port, RtkStormUnit_e stormUnit, uint32 rate)
{
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(EM_STORM_UNIT_END <= stormUnit)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(DVA_STORM_MAX_RATE < rate)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Set suppress unicast configure and rate registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                  DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_BUNIT_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_BUNIT_LEN, (uint32)stormUnit));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_RATE + (sysPort * PERPORT_REG_OFFSET),
                                                  DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_RATE_P_UNUA_RATE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_RATE_P_UNUA_RATE_LEN, rate));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get unicast storm control rate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identication (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
 *            Indicate the port ID for getting the unicast storm unit and rate.
 * @param[out] pStormUnit Storm Unit of Rate (N/A)
 *             Indicate the unicast storm unit of rate. \n
 * @param[out] pRate Limited Rate (0 to 67108863) \n
 *             Indicate the limited rate for unicast storm.
 * @retval RT_ERR_OK Get unicast storm control rate successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the limited traffic rate for unknown unicast storm.
 *       The unknown unicast traffic is dropped when its traffic level is exceeded.
 */
RtkApiRet dva_storm_ctl_unicast_rate_get(uint32 unit, RtkPort port, RtkStormUnit_e *pStormUnit, uint32 *pRate)
{
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;
    RtkApiRet ret     = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || ((uint32)DVA_RTK_USER_PORT_END < port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pStormUnit) || (NULL == pRate))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get suppress unicast configure and rate registers. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_BUNIT_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_CONFIGURE_P_UNUA_BUNIT_LEN, &val));
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_RATE + (sysPort * PERPORT_REG_OFFSET),
                                                 DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_RATE_P_UNUA_RATE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_UNKNOWN_UNICAST_RATE_P_UNUA_RATE_LEN, pRate));

        /* Transform the value getting from register to unicast storm unit. */
        *pStormUnit = (val == 0u) ? EM_STORM_UNIT_PPS : EM_STORM_UNIT_BPS;
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* STORM_UNICAST_RATE_ASIC */
/**@}*/ /* STORM_UNICAST_RATE */

/**@}*/
