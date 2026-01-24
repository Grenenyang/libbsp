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
#include "rtkas_api_link_aggregation.h"

/** @addtogroup EXAMPLE
 *  @{
 */

/** @addtogroup LAG_EXAMPLE Link Aggregation Example
 *  @{
 */

/**
 * @addtogroup RTL907XDV1_LAG_EXAMPLE RTL907XD_V1 Link Aggregation Example
 * @{
 */

/**
 * @addtogroup RTL907XDV1_LAG_BALANCE_ALL_EXAMPLE Link Aggregation Balance All Mode Example
 *
 * @brief <b>Link Aggregation Balance All Mode</b> \n
 *        When switch1 transmits packets to switch2, switch2 will perform balance all calculation. In this case, \n
 *        switch2 will transmit packets via LAG ports in a round-robin manner.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of link aggregation using the balance all mode for load balancing.
 *
 * @section Description
 * This function configures link aggregation by setting up group members, enabling balance all mode, and verifying the settings.
 *
 * @image html lag/lag_balance_all.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_lag_balance_all(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_lag_balance_all(void)
{
    RtkApiRet          ret                 = RT_ERR_OK;
    uint32             unit                = DV1_UNIT;
    UnitChip_t         unitChip            = {0u};
    LagMemberConfig_t  setLagMemberConfig  = {0u};
    LagMemberConfig_t  getLagMemberConfig  = {0u};
    LagBalanceConfig_t setLagBalanceConfig = {0u};
    LagBalanceConfig_t getLagBalanceConfig = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;
    do
    {
        /* Step 1. Set the port mask corresponding to the group members of link aggregation. Combine ports 1, 3 and 4 into group 1. */
        setLagMemberConfig.group    = 1u;
        setLagMemberConfig.portMask = (DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH);
        CHK_FUN_CONTINUE(ret, rtk_lag_member_set(unitChip, &setLagMemberConfig));
        /* Expected Behaviour: Ports 1, 3 and 4 are combined into group 1. */

        /* Step 2. Get the group members of link aggregation. */
        getLagMemberConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_member_get(unitChip, &getLagMemberConfig));
        /* Expected Behaviour: Get member ports 1, 3 and 4 of group 1. */

        /* Step 3. Set link aggregation group to balance all mode.*/
        setLagBalanceConfig.group       = 1u;
        setLagBalanceConfig.balanceMode = RTK_LAG_BALANCE_ALL;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_set(unitChip, &setLagBalanceConfig));
        /* Expected Behaviour: Group 1 balance all mode is enabled */

        /* Step 4. Get link aggregation group balance mode.*/
        getLagBalanceConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_get(unitChip, &getLagBalanceConfig));
        /* Expected Behaviour: Get group 1 balance all mode. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XDV1_LAG_BALANCE_ALL_EXAMPLE */
/**@}*/ /* RTL907XDV1_LAG_EXAMPLE */
/**@}*/ /* LAG_EXAMPLE */
/**@}*/ /* EXAMPLE */
