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
#include "rtkas_api_storm_ctl.h"

/** @addtogroup EXAMPLE
 *  @{
 */

/** @addtogroup STORM_EXAMPLE Storm Control Example
 *  @{
 */

/**
 * @addtogroup RTL907XDV1_STORM_EXAMPLE RTL907XD_V1 Storm Control Example
 * @{
 */

/**
 * @addtogroup RTL907XDV1_STORM_BROADCAST_EXAMPLE Broadcast Storm Control Example
 *
 * @brief <b>Broadcast Storm Control</b> \n
 *        When host1 transmits broadcast packets to the switch, the switch will monitor the ingress traffic levels and start to drop the broadcast\n
 *        packets when the traffic level is exceeded. In this case, if host1 transmits broadcast packets continuously and the burst size is exceeded,\n
 *        the switch will start to limit the broadcast packets to 10 PPS.
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of broadcast storm control.
 *
 * @section Description
 * This function configures broadcast storm control by enabling it on a specific port, setting the rate, and verifying the settings.
 *
 * @image html stormctl/stormctl_broadcast.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_storm_ctl_broadcast(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_storm_ctl_broadcast(void)
{
    uint32                        unit                           = DV1_UNIT;
    RtkApiRet                     ret                            = RT_ERR_OK;
    StormCtlBroadcastConfig_t     setStormCtlBroadcastConfig     = {0u};
    StormCtlBroadcastConfig_t     getStormCtlBroadcastConfig     = {0u};
    StormCtlBroadcastRateConfig_t setStormCtlBroadcastRateConfig = {0u};
    StormCtlBroadcastRateConfig_t getStormCtlBroadcastRateConfig = {0u};
    UnitChip_t                    unitChip                       = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;
    do
    {
        /* Step 1. Choose port to enable global broadcast storm control. */
        setStormCtlBroadcastConfig.port   = DV1_RTK_USER_PORT_1ST;
        setStormCtlBroadcastConfig.enable = ENABLED;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_broadcast_set(unitChip, &setStormCtlBroadcastConfig));
        /* Expected Behaviour: Enable broadcast storm control for the port 1. */

        /* Step 2. Get global broadcast storm control status. */
        getStormCtlBroadcastConfig.port = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_broadcast_get(unitChip, &getStormCtlBroadcastConfig));
        /* Expected Behaviour: Get broadcast storm control status which is enabled for the port 1. */

        /* Step 3. Set broadcast storm control rate. */
        setStormCtlBroadcastRateConfig.port      = DV1_RTK_USER_PORT_1ST;
        setStormCtlBroadcastRateConfig.stormUnit = EM_STORM_UNIT_PPS;
        setStormCtlBroadcastRateConfig.rate      = 10u;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_broadcast_rate_set(unitChip, &setStormCtlBroadcastRateConfig));
        /* Expected Behaviour: Set the port 1 to filter 10 pps(unit of packets). */

        /* Step 4. Get broadcast storm control rate. */
        getStormCtlBroadcastRateConfig.port = DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_broadcast_rate_get(unitChip, &getStormCtlBroadcastRateConfig));
        /* Expected Behaviour: Get the broadcast storm control rate which is set 10 pps of port 1. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XDV1_STORM_BROADCAST_EXAMPLE */
/**@}*/ /* RTL907XDV1_STORM_EXAMPLE */
/**@}*/ /* STORM_EXAMPLE */
/**@}*/ /* EXAMPLE */
