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

/** @addtogroup STORM_EXAMPLE Storm Control Example
 *  @{
 */

/**
 * @addtogroup RTL907XCVB_STORM_EXAMPLE RTL907XC_VB Storm Control Example
 * @{
 */

/**
 * @addtogroup RTL907XCVB_STORM_MULTICAST_EXAMPLE Multicast Storm Control Example
 *
 * @brief <b>Multicast Storm Control</b> \n
 *        When host1 transmits multicast packets to the switch, the switch will monitor the ingress traffic levels and start to drop the multicast\n
 *        packets when the traffic level is exceeded. In this case, if host1 transmits multicast packets continuously and the burst size is exceeded,\n
 *        the switch will start to limit the multicast packets to 10 PPS.
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of multicast storm control.
 *
 * @section Description
 * This function configures multicast storm control by enabling it on a specific port, setting the rate, and verifying the settings.
 *
 * @image html stormctl/stormctl_multicast.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_storm_ctl_multicast(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_storm_ctl_multicast(void)
{
    uint32                        unit                           = CVB_UNIT;
    RtkApiRet                     ret                            = RT_ERR_OK;
    StormCtlMulticastConfig_t     setStormCtlMulticastConfig     = {0u};
    StormCtlMulticastConfig_t     getStormCtlMulticastConfig     = {0u};
    StormCtlMulticastRateConfig_t setStormCtlMulticastRateConfig = {0u};
    StormCtlMulticastRateConfig_t getStormCtlMulticastRateConfig = {0u};
    UnitChip_t                    unitChip                       = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VB;
    do
    {
        /* Step 1. Choose port to enable global multicast storm control with both multicast type. */
        setStormCtlMulticastConfig.port   = CVB_RTK_USER_PORT_1ST;
        setStormCtlMulticastConfig.enable = ENABLED;
        setStormCtlMulticastConfig.type   = MC_TYPE_BOTH;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_multicast_set(unitChip, &setStormCtlMulticastConfig));
        /* Expected Behaviour: Enable multicast storm control for the port 1 with both multicast type. */

        /* Step 2. Get global multicast storm control status. */
        getStormCtlMulticastConfig.port = CVB_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_multicast_get(unitChip, &getStormCtlMulticastConfig));
        /* Expected Behaviour: Get multicast storm control status which is enabled for the port 1 with both multicast type. */

        /* Step 3. Set multicast storm control rate. */
        setStormCtlMulticastRateConfig.port      = CVB_RTK_USER_PORT_1ST;
        setStormCtlMulticastRateConfig.stormUnit = EM_STORM_UNIT_PPS;
        setStormCtlMulticastRateConfig.rate      = 10u;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_multicast_rate_set(unitChip, &setStormCtlMulticastRateConfig));
        /* Expected Behaviour: Set the port 1 to filter 10 pps(unit of packets). */

        /* Step 4. Get multicast storm control rate. */
        getStormCtlMulticastRateConfig.port = CVB_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_multicast_rate_get(unitChip, &getStormCtlMulticastRateConfig));
        /* Expected Behaviour: Get the multicast storm control rate which is set 10 pps of port 1. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XCVB_STORM_MULTICAST_EXAMPLE */
/**@}*/ /* RTL907XCVB_STORM_EXAMPLE */
/**@}*/ /* STORM_EXAMPLE */
