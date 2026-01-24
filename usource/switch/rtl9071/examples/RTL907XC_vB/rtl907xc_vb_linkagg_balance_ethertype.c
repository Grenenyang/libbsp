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
 * @addtogroup RTL907XCVB_LAG_EXAMPLE RTL907XD_V1 Link Aggregation Example
 * @{
 */

/**
 * @addtogroup RTL907XCVB_LAG_BALANCE_ETHERTYPE_EXAMPLE Link Aggregation Balance Ether Type Mode Example
 *
 * @brief <b>Link Aggregation Balance Ether Type Mode</b> \n
 *        When switch1 transmits packets to switch2, switch2 will use the ether type field in the packet content to balance \n
 *        ether type checking. In this case, switch2 receives packets with ether type 0x1234, matching the balance\n
 *        ether type setting. Therefore, switch2 will transmit the packets via LAG ports in a round-robin manner.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of link aggregation using the ether type for load balancing.
 *
 * @section Description
 * This function configures link aggregation by setting up group members, enabling ether type balance mode, and verifying the settings.
 *
 * @image html lag/lag_balance_ethertype.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cvb_lag_balance_ethertype(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cvb_lag_balance_ethertype(void)
{
    RtkApiRet                   ret                             = RT_ERR_OK;
    uint32                      unit                            = CVB_UNIT;
    UnitChip_t                  unitChip                        = {0u};
    LagMemberConfig_t           setLagMemberConfig              = {0u};
    LagMemberConfig_t           getLagMemberConfig              = {0u};
    LagBalanceConfig_t          setLagBalanceConfig             = {0u};
    LagBalanceConfig_t          getLagBalanceConfig             = {0u};
    LagBalanceEthertypeConfig_t setLagBalanceEthertypeConfig    = {0u};
    LagBalanceEthertypeConfig_t getLagBalanceEthertypeGetConfig = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VB;
    do
    {
        /* Step 1. Set the port mask corresponding to the group members of link aggregation. Combine ports 1, 3 and 4 into group 1. */
        setLagMemberConfig.group    = 1u;
        setLagMemberConfig.portMask = (CVB_RTK_USER_PMAP_1ST | CVB_RTK_USER_PMAP_3RD | CVB_RTK_USER_PMAP_4TH);
        CHK_FUN_CONTINUE(ret, rtk_lag_member_set(unitChip, &setLagMemberConfig));
        /* Expected Behaviour: Ports 1, 3 and 4 are combined into group 1. */

        /* Step 2. Get the group members of link aggregation. */
        getLagMemberConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_member_get(unitChip, &getLagMemberConfig));
        /* Expected Behaviour: Get member ports 1, 3 and 4 of group 1. */

        /* Step 3. Set link aggregation group to balance ether type mode.*/
        setLagBalanceConfig.group       = 1u;
        setLagBalanceConfig.balanceMode = RTK_LAG_BALANCE_ETHERTYPE;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_set(unitChip, &setLagBalanceConfig));
        /* Expected Behaviour: Group 1 balance ether type mode is enabled */

        /* Step 4. Get link aggregation group balance mode.*/
        getLagBalanceConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_get(unitChip, &getLagBalanceConfig));
        /* Expected Behaviour: Get group 1 balance ether type mode which is enabled */

        /* Step 5. Set the value of balance ether type mode. */
        setLagBalanceEthertypeConfig.group     = 1u;
        setLagBalanceEthertypeConfig.etherType = 0x1234u;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_ethertype_set(unitChip, &setLagBalanceEthertypeConfig));
        /* Expected Behaviour: Group 1 balance ether type value is set with 0x1234. */

        /* Step 6. Get the value of balance ether type mode. */
        getLagBalanceEthertypeGetConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_ethertype_get(unitChip, &getLagBalanceEthertypeGetConfig));
        /* Expected Behaviour: Get group 1 balance ether type value which is set with 0x1234. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XCVB_LAG_BALANCE_ETHERTYPE_EXAMPLE */
/**@}*/ /* RTL907XCVB_LAG_EXAMPLE */
/**@}*/ /* LAG_EXAMPLE */
