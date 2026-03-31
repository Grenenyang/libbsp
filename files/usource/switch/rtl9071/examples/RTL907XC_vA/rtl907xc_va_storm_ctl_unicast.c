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
 * @addtogroup RTL907XCVA_STORM_EXAMPLE RTL907XC_VA Storm Control Example
 * @{
 */

/**
 * @addtogroup RTL907XCVA_STORM_UNICAST_EXAMPLE Unicast Storm Control Example
 *
 * @brief <b>Unicast Storm Control</b> \n
 *        When host1 transmits unicast packets to the switch, the switch will monitor the ingress traffic levels and start to drop the unicast\n
 *        packets when the traffic level is exceeded. In this case, if host1 transmits unicast packets continuously and the burst size is exceeded,\n
 *        the switch will start to limit the unicast packets to 10 PPS.
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of unicast storm control.
 *
 * @section Description
 * This function configures unicast storm control by enabling it on a specific port, setting the rate, and verifying the settings.
 *
 * @image html stormctl/stormctl_unicast.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_storm_ctl_unicast(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_storm_ctl_unicast(void)
{
    uint32                      unit                         = CVA_UNIT;
    RtkApiRet                   ret                          = RT_ERR_OK;
    StormCtlUnicastConfig_t     setStormCtlUnicastConfig     = {0u};
    StormCtlUnicastConfig_t     getStormCtlUnicastConfig     = {0u};
    StormCtlUnicastRateConfig_t setStormCtlUnicastRateConfig = {0u};
    StormCtlUnicastRateConfig_t getStormCtlUnicastRateConfig = {0u};
    UnitChip_t                  unitChip                     = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;
    do
    {
        /* Step 1. Choose port to enable global unicast storm control. */
        setStormCtlUnicastConfig.port   = CVA_RTK_USER_PORT_1ST;
        setStormCtlUnicastConfig.enable = ENABLED;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_unicast_set(unitChip, &setStormCtlUnicastConfig));
        /* Expected Behaviour: Enable unicast storm control for the port 1 */

        /* Step 2. Get global unicast storm control status. */
        getStormCtlUnicastConfig.port = CVA_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_unicast_get(unitChip, &getStormCtlUnicastConfig));
        /* Expected Behaviour: Get broadcast storm control status which is enabled for the port 1. */

        /* Step 3. Set unicast storm control rate. */
        setStormCtlUnicastRateConfig.port      = CVA_RTK_USER_PORT_1ST;
        setStormCtlUnicastRateConfig.stormUnit = EM_STORM_UNIT_PPS;
        setStormCtlUnicastRateConfig.rate      = 10u;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_unicast_rate_set(unitChip, &setStormCtlUnicastRateConfig));
        /* Expected Behaviour: Set the port 1 to filter 10 pps(unit of packets) */

        /* Step 4. Get unicast storm control rate. */
        getStormCtlUnicastRateConfig.port = CVA_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_storm_ctl_unicast_rate_get(unitChip, &getStormCtlUnicastRateConfig));
        /* Expected Behaviour: Get the unicast storm control rate which is set 10 pps of port 1. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XCVA_STORM_UNICAST_EXAMPLE */
/**@}*/ /* RTL907XCVA_STORM_EXAMPLE */
/**@}*/ /* STORM_EXAMPLE */
