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
 * @addtogroup RTL907XCVA_LAG_EXAMPLE RTL907XD_V1 Link Aggregation Example
 * @{
 */

/**
 * @addtogroup RTL907XCVA_LAG_HASH_EXAMPLE Link Aggregation Hash Mode Example
 *
 * @brief <b>Link Aggregation Hash Mode</b> \n
 *        When switch1 transmits the packet to switch2, switch2 will use source MAC and destination MAC with hash shift to do \n
 *        the hash calculation. In this case, switch2 will transmit the packet via port 1 after LAG hash calculation. \n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of link aggregation using hash mode for load balancing.
 *
 * @section Description
 * This function configures link aggregation by setting up group members, disabling balance mode, setting hash mask and shift, and verifying the settings.
 *
 * @image html lag/lag_hash.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_lag_hash_mode(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_lag_hash_mode(void)
{
    RtkApiRet          ret                 = RT_ERR_OK;
    uint8              hashMask            = (uint8)RTK_LAG_HASH_DMAC | (uint8)RTK_LAG_HASH_SMAC;
    uint16             hashShift           = (0x0003u << RTK_LAG_HASH_SHIFT_OFFSET_DMAC);
    uint32             unit                = CVA_UNIT;
    UnitChip_t         unitChip            = {0u};
    LagMemberConfig_t  setLagMemberConfig  = {0u};
    LagMemberConfig_t  getLagMemberConfig  = {0u};
    LagBalanceConfig_t setLagBalanceConfig = {0u};
    LagBalanceConfig_t getLagBalanceConfig = {0u};
    LagHashConfig_t    setLagHashConfig    = {0u};
    LagHashConfig_t    getLagHashConfig    = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VA;
    do
    {
        /* Step 1. Set the port mask corresponding to the group members of link aggregation. Combine ports 1, 3 and 4 into group 1. */
        setLagMemberConfig.group    = 1u;
        setLagMemberConfig.portMask = (CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_3RD | CVA_RTK_USER_PMAP_4TH);
        CHK_FUN_CONTINUE(ret, rtk_lag_member_set(unitChip, &setLagMemberConfig));
        /* Expected Behaviour: Ports 1, 3 and 4 are combined into group 1. */

        /* Step 2. Get the group members of link aggregation. */
        getLagMemberConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_member_get(unitChip, &getLagMemberConfig));
        /* Expected Behaviour: Get member ports 1, 3 and 4 of group 1. */

        /* Step 3. Disable balance mode.*/
        setLagBalanceConfig.group       = 1u;
        setLagBalanceConfig.balanceMode = RTK_LAG_BALANCE_DISABLE;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_set(unitChip, &setLagBalanceConfig));
        /* Expected Behaviour: Group 1 balance mode is disabled */

        /* Step 4. Get link aggregation group balance mode.*/
        getLagBalanceConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_balance_get(unitChip, &getLagBalanceConfig));
        /* Expected Behaviour: Get group 1 balance disable mode. */

        /* Step 5. Set hash mask(DMAC SMAC) and set hash shift(DMAC) to 3. */
        setLagHashConfig.group     = 1u;
        setLagHashConfig.hashMask  = hashMask;
        setLagHashConfig.hashShift = hashShift;
        CHK_FUN_CONTINUE(ret, rtk_lag_hash_set(unitChip, &setLagHashConfig));
        /* Expected Behaviour: Group 1 hash mask is set as DMAC and SMAC and hash shift of DMAC is set to 3. */

        /* Step 6. Get hash mask and hash shift.*/
        getLagHashConfig.group = 1u;
        CHK_FUN_CONTINUE(ret, rtk_lag_hash_get(unitChip, &getLagHashConfig));
        /* Expected Behaviour: Get group 1 hash mask which is set as DMAC and SMAC and hash shift of DMAC which is set to 3. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XCVA_LAG_HASH_EXAMPLE */
/**@}*/ /* RTL907XCVA_LAG_EXAMPLE */
/**@}*/ /* LAG_EXAMPLE */
