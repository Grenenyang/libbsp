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

#include "rtl907xd_va_asicdrv_storm_ctl.h"

#ifdef RTK_MODULE_STORM_CTRL
/**
 * @brief Set multicast storm control.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sysPort System Port Identification (2 to 15) \n
 *            Indicate the port ID for setting multicast storm control.
 * @param[in] enable Enable State (0 to 1) \n
 *            Indicate the state of multicast storm control. \n
 *            0: Disabled \n
 *            1: Enabled \n
 * @param[in] type Multicast Type (0 to 1) \n
 *            Indicate the type of multicast storm. \n
 *            0: Unknown multicast only \n
 *            1: Both known and unknown multicast
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Refer to dva_reg_field_write() for the error.
 */
RtkApiRet dva_drv_storm_ctl_multicast_set(uint32 unit, dva_RtkPort sysPort, dva_RtkEnable enable, uint8 type)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Set the value based on the state. */
        if(DVA_DISABLED == enable)
        {
            /* Set suppress multicast configure registers. */
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_ENABLE_LEN, 0x0u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_TYPE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_TYPE_LEN, type));
        }
        else
        {
            /* Set suppress multicast configure registers. */
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_MC_ENABLE_LSP,
                                                      DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SYS_MC_ENABLE_LEN, 0x1u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_TRAFFSTORM_SUPPRESS_GLOBAL, DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SCMETER_INC_IFG_LSP,
                                                      DVA_TRAFFSTORM_SUPPRESS_GLOBAL_SCMETER_INC_IFG_LEN, 0x1u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_ENABLE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_ENABLE_LEN, 0x1u));
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE + (sysPort * PERPORT_REG_OFFSET),
                                                      DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_TYPE_LSP, DVA_PORT0_TRAFFSTORM_SUPPRESS_MULTICAST_CONFIGURE_P_MC_TYPE_LEN, type));
        }
    } while(0u == 1u);

    return ret;
}
#endif
