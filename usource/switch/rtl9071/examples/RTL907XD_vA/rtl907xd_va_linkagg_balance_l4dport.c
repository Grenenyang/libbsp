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

/** @addtogroup LAG_EXAMPLE Link Aggregation Example
 *  @{
 */

/**
 * @addtogroup RTL907XDVA_LAG_EXAMPLE RTL907XD_VA Link Aggregation Example
 * @{
 */

/**
 * @addtogroup RTL907XDVA_LAG_BALANCE_L4DPORT_EXAMPLE Link Aggregation Balance L4 Destination Port Mode Example
 *
 * @brief <b>Link Aggregation Balance L4 Destination Port Mode</b> \n
 *        When switch1 transmits packets to switch2, switch2 will use the L4 destination port field in the packet content to balance \n
 *        L4 destination port checking. In this case, switch2 receives packets with L4 destination port 0x1234, matching the balance
 *        protocol setting. Therefore, switch2 will transmit the packets via LAG ports in a round-robin manner.\n
 *
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of link aggregation using the L4 destination port for load balancing.
 *
 * @section Description
 * This function configures link aggregation by setting up group members, enabling L4 destination port balance mode, and verifying the settings.
 *
 * @image html lag/lag_balance_l4dport.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_lag_balance_l4dport(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_lag_balance_l4dport(void)
{
    RtkApiRet                 ret                        = RT_ERR_OK;
    uint32                    unit                       = DVA_UNIT;
    UnitChip_t                unitChip                   = {0};
    LagMemberConfig_t         setLagMemberConfig         = {0u};
    LagMemberConfig_t         getLagMemberConfig         = {0u};
    LagBalanceConfig_t        setLagBalanceConfig        = {0u};
    LagBalanceConfig_t        getLagBalanceConfig        = {0u};
    LagBalanceL4DportConfig_t setLagBalanceL4DportConfig = {0u};
    LagBalanceL4DportConfig_t getLagBalanceL4DportConfig = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_VA;
    do
    {
        /* Step 1. Set the port mask corresponding to the group members of link aggregation. Combine ports 1, 3, 4 into group 1. */
        setLagMemberConfig.group    = 1u;
        setLagMemberConfig.portMask = (DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_3RD | DVA_RTK_USER_PMAP_4TH);
        CHK_FUN_CONTINUE(ret, rtk_lag_member_set(unitChip, &setLagMemberConfig));
        /* Expected Behaviour: Ports 1, 3 and 4 are combined into group 1. */

        /* Step 2. Get the group members of link aggregation. */
        getLagMemberConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_member_get(unitChip, &getLagMemberConfig));
        /* Expected Behaviour: Get member ports 1, 3 and 4 of group 1. */

        /* Step 3. Set link aggregation group to balance l4dport mode.*/
        setLagBalanceConfig.group       = 1u;
        setLagBalanceConfig.balanceMode = RTK_LAG_BALANCE_L4DPORT;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_set(unitChip, &setLagBalanceConfig));
        /* Expected Behaviour: Group 1 balance l4dport mode is enabled */

        /* Step 4. Get link aggregation group to balance l4dport mode.*/
        getLagBalanceConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_get(unitChip, &getLagBalanceConfig));
        /* Expected Behaviour: Get group 1 balance l4dport mode which is enabled */

        /* Step 5. Set the value and mask of balance l4dport mode. */
        setLagBalanceL4DportConfig.group       = 1u;
        setLagBalanceL4DportConfig.l4Dport     = 0x1234;
        setLagBalanceL4DportConfig.l4DportMask = 0xFFFF;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_l4dport_set(unitChip, &setLagBalanceL4DportConfig));
        /* Expected Behaviour: Group 1 balance l4dport value and mask are set with 0x1234 and 0xFFFF. */

        /* Step 6. Get the value and mask of balance l4dport mode. */
        getLagBalanceL4DportConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_l4dport_get(unitChip, &getLagBalanceL4DportConfig));
        /* Expected Behaviour: Get group 1 balance l4dport value and mask which are set with 0x1234 and 0xFFFF. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XDVA_LAG_BALANCE_L4DPORT_EXAMPLE */
/**@}*/ /* RTL907XDVA_LAG_EXAMPLE */
/**@}*/ /* LAG_EXAMPLE */
