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
#include "rtkas_api_link_aggregation.h"
/**
 * @addtogroup RTK_MODULE_LINK_AGGREGATION Link Aggregation
 * @{
 */

/**
 * @addtogroup LAG_GROUP Port Grouping
 * @{
 */

/**
 * @addtogroup LAG_GROUP_ASIC Low Level Driver
 * @brief Port Grouping Low Level Driver
 * @{
 */

/**
 * @brief Set group members of link aggregation.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for setting member ports.
 * @param[in] portMask Member port mask (0 to DVA_RTK_USER_PMAP_ALL) \n
 *            Indicate member ports in group.
 * @param[out] None
 * @retval RT_ERR_OK Set group members of link aggregation successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval Others Refer to dva_drv_pmap_convert() or dva_reg_field_write() for the error.
 * @note This API sets the port mask corresponding to the group members of link aggregation. \n
 *       The links in the port mask are bundled together to form a single logical link.
 */
RtkApiRet dva_lag_member_set(uint32 unit, uint32 group, RtkPmap portMask)
{
    RtkApiRet ret = RT_ERR_OK;
    RtkPmap   val = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Transform the mapping between user port mask and system port mask. */
        CHK_FUN_CONTINUE(ret, dva_drv_pmap_convert(portMask, &val, DVA_EM_USER_TO_SYS));
        /* Set LAG member register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_MEMBER + ((group - 1u) * 0x8u), DVA_LAG0_MEMBER_LAG0_PORT_MEMBER_LSP, DVA_LAG0_MEMBER_LAG0_PORT_MEMBER_LEN, val));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get group members of link aggregation.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for getting member ports.
 * @param[out] pPortMask Member port mask (0 to DVA_RTK_USER_PMAP_ALL) \n
 *             Indicate member ports in group.
 * @retval RT_ERR_OK Get group members of link aggregation successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_drv_pmap_convert() or dva_reg_field_read() for the error.
 * @note This API gets the port mask corresponding to the group members of link aggregation. \n
 *       The links in the port mask are bundled together to form a single logical link.
 */
RtkApiRet dva_lag_member_get(uint32 unit, uint32 group, RtkPmap *pPortMask)
{
    RtkApiRet ret = RT_ERR_OK;
    RtkPmap   val = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pPortMask)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get LAG member register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_MEMBER + ((group - 1u) * 0x8u), DVA_LAG0_MEMBER_LAG0_PORT_MEMBER_LSP, DVA_LAG0_MEMBER_LAG0_PORT_MEMBER_LEN, &val));
        /* Transform the mapping between user port mask and system port mask. */
        CHK_FUN_CONTINUE(ret, dva_drv_pmap_convert(val, pPortMask, DVA_EM_SYS_TO_USER));
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* LAG_GROUP_ASIC */
/**@}*/ /* LAG_GROUP */

/**
 * @addtogroup LAG_HASH Hash Mode
 * @{
 */

/**
 * @addtogroup LAG_HASH_ASIC Driver
 * @brief Hash Mode Low Level Driver
 * @{
 */

/**
 * @brief Set link aggregation group to hash mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for setting hash mask and shift.
 * @param[in] hashMask Hash Mask (RTK_LAG_HASH_ALL) \n
 *            Indicate hash mask for calculating hash value. Hash mask can contain multiple hash keys. \n
 *            RTK_LAG_HASH_INPORT: Ingress port as hash key \n
 *            RTK_LAG_HASH_DMAC: Destination MAC as hash key \n
 *            RTK_LAG_HASH_SMAC: Source MAC as hash key \n
 *            RTK_LAG_HASH_DIP: Destination IP as hash key \n
 *            RTK_LAG_HASH_SIP: Source IP as hash key \n
 *            RTK_LAG_HASH_DPORT: L4 source port as hash key \n
 *            RTK_LAG_HASH_SPORT: L4 destination port as hash key
 * @param[in] hashShift Hash Shift (RTK_LAG_HASH_SHIFT_OFFSET_ALL) \n
 *            Indicate hash shift for hash key shifting. \n
 *            RTK_LAG_HASH_SHIFT_OFFSET_INPORT: Hash shift for ingress port, 0 to 3 for the offset \n
 *            RTK_LAG_HASH_SHIFT_OFFSET_DMAC: Hash shift for sestination MAC, 0 to 3 for the offset \n
 *            RTK_LAG_HASH_SHIFT_OFFSET_SMAC: Hash shift for source MAC, 0 to 3 for the offset \n
 *            RTK_LAG_HASH_SHIFT_OFFSET_DIP: Hash shift for destination IP, 0 to 3 for the offset \n
 *            RTK_LAG_HASH_SHIFT_OFFSET_SIP: Hash shift for source IP, 0 to 3 for the offset \n
 *            RTK_LAG_HASH_SHIFT_OFFSET_DPORT: Hash shift for L4 source port, 0 to 3 for the offset \n
 *            RTK_LAG_HASH_SHIFT_OFFSET_SPORT: Hash shift for L4 destination port, 0 to 3 for the offset
 * @param[out] None
 * @retval RT_ERR_OK Set link aggregation group to hash mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id, hash mask or hash shift is invalid.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the hash mode to distribute packets by a hash value computed from the specific packet fields.
 */
RtkApiRet dva_lag_hash_set(uint32 unit, uint32 group, uint8 hashMask, uint16 hashShift)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if((hashMask & RTK_LAG_HASH_ALL) != hashMask)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if((hashShift & RTK_LAG_HASH_SHIFT_OFFSET_ALL) != hashShift)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set LAG hash setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_HASH_SETTING + ((group - 1u) * 0x8u), DVA_LAG0_HASH_SETTING_LAG0_HASH_MASK_LSP, DVA_LAG0_HASH_SETTING_LAG0_HASH_MASK_LEN, (uint32)hashMask));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_HASH_SETTING + ((group - 1u) * 0x8u), DVA_LAG0_HASH_SETTING_LAG0_HASH_SHIFT_LSP, DVA_LAG0_HASH_SETTING_LAG0_HASH_SHIFT_LEN, (uint32)hashShift));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get hash mode configuration of link aggregation group.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for getting hash mask and shift. \n
 * @param[out] pHashMask Hash Mask (RTK_LA_ALL_HASH_MASK) \n
 *             Indicate hash mask for calculating hash value. Hash mask can contain multiple hash keys. \n
 *             RTK_LAG_HASH_INPORT: Ingress port as hash key \n
 *             RTK_LAG_HASH_DMAC: Destination MAC as hash key \n
 *             RTK_LAG_HASH_SMAC: Source MAC as hash key \n
 *             RTK_LAG_HASH_DIP: Destination IP as hash key \n
 *             RTK_LAG_HASH_SIP: Source IP as hash key \n
 *             RTK_LAG_HASH_DPORT: L4 source port as hash key \n
 *             RTK_LAG_HASH_SPORT: L4 destination port as hash key
 * @param[out] pHashShift Hash Shift (RTK_LAG_HASH_SHIFT_OFFSET_ALL)
 *             Indicate hash shift for hash key shifting. \n
 *             RTK_LAG_HASH_SHIFT_OFFSET_INPORT: Hash shift for ingress port, 0 to 3 for the offset \n
 *             RTK_LAG_HASH_SHIFT_OFFSET_DMAC: Hash shift for sestination MAC, 0 to 3 for the offset \n
 *             RTK_LAG_HASH_SHIFT_OFFSET_SMAC: Hash shift for source MAC, 0 to 3 for the offset \n
 *             RTK_LAG_HASH_SHIFT_OFFSET_DIP: Hash shift for destination IP, 0 to 3 for the offset \n
 *             RTK_LAG_HASH_SHIFT_OFFSET_SIP: Hash shift for source IP, 0 to 3 for the offset \n
 *             RTK_LAG_HASH_SHIFT_OFFSET_DPORT: Hash shift for L4 source port, 0 to 3 for the offset \n
 *             RTK_LAG_HASH_SHIFT_OFFSET_SPORT: Hash shift for L4 destination port, 0 to 3 for the offset
 * @retval RT_ERR_OK Get hash mode configuration of link aggregation group successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the hash mode configuration of the link aggregation group.
 */
RtkApiRet dva_lag_hash_get(uint32 unit, uint32 group, uint8 *pHashMask, uint16 *pHashShift)
{
    RtkApiRet ret = RT_ERR_OK;
    uint32    val = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pHashMask) || (NULL == pHashShift))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get LAG hash setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_HASH_SETTING + ((group - 1u) * 0x8u), DVA_LAG0_HASH_SETTING_LAG0_HASH_MASK_LSP, DVA_LAG0_HASH_SETTING_LAG0_HASH_MASK_LEN, &val));
        *pHashMask = (uint8)val;
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_HASH_SETTING + ((group - 1u) * 0x8u), DVA_LAG0_HASH_SETTING_LAG0_HASH_SHIFT_LSP, DVA_LAG0_HASH_SETTING_LAG0_HASH_SHIFT_LEN, &val));
        *pHashShift = (uint16)val;
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* LAG_HASH_ASIC */
/**@}*/ /* LAG_HASH */

/**
 * @addtogroup LAG_BALANCE Balance Mode
 * @{
 */

/**
 * @addtogroup LAG_BALANCE_ASIC Driver
 * @brief Balance Mode Low Level Driver
 * @{
 */

/**
 * @brief Set link aggregation group to balance mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for setting balance mode.
 * @param[in] balanceMode Balance Mode (RTK_LAG_BALANCE_DISABLE, RTK_LAG_BALANCE_ETHERTYPE, RTK_LAG_BALANCE_PROTOCOL,
 *            RTK_LAG_BALANCE_L4DPORT, RTK_LAG_BALANCE_ALL)\n
 *            RTK_LAG_BALANCE_DISABLE: Disable balance mode \n
 *            RTK_LAG_BALANCE_ETHERTYPE: Ether type balance mode \n
 *            RTK_LAG_BALANCE_PROTOCOL: Protocol balance mode \n
 *            RTK_LAG_BALANCE_L4DPORT: L4 destination port balance mode \n
 *            RTK_LAG_BALANCE_ALL: All balance mode
 * @param[out] None
 * @retval RT_ERR_OK Set link aggregation group to balance mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id or balance mode is invalid.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the balance mode to distribute packets by round-robin of group members.
 */
RtkApiRet dva_lag_balance_set(uint32 unit, uint32 group, uint32 balanceMode)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if((RTK_LAG_BALANCE_DISABLE != balanceMode) && (RTK_LAG_BALANCE_ETHERTYPE != balanceMode)
           && (RTK_LAG_BALANCE_PROTOCOL != balanceMode) && (RTK_LAG_BALANCE_L4DPORT != balanceMode)
           && (RTK_LAG_BALANCE_ALL != balanceMode))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_BALANCE_MODE_SETTING_2 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_2_LAG0_BALANCE_MODE_MASK_LSP, DVA_LAG0_BALANCE_MODE_SETTING_2_LAG0_BALANCE_MODE_MASK_LEN, balanceMode));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get balance mode status of link aggregation group.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for getting balance mode.
 * @param[out] pBalanceMode Balance Mode (RTK_LAG_BALANCE_DISABLE, RTK_LAG_BALANCE_ETHERTYPE, RTK_LAG_BALANCE_PROTOCOL,
 *             RTK_LAG_BALANCE_L4DPORT, RTK_LAG_BALANCE_ALL)\n
 *             RTK_LAG_BALANCE_DISABLE: Disable balance mode \n
 *             RTK_LAG_BALANCE_ETHERTYPE: Ether type balance mode \n
 *             RTK_LAG_BALANCE_PROTOCOL: Protocol balance mode \n
 *             RTK_LAG_BALANCE_L4DPORT: L4 destination port balance mode \n
 *             RTK_LAG_BALANCE_ALL: All balance mode
 * @retval RT_ERR_OK Get balance mode status of link aggregation group successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the balance mode status of the link aggregation group.
 */
RtkApiRet dva_lag_balance_get(uint32 unit, uint32 group, uint32 *pBalanceMode)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pBalanceMode)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_BALANCE_MODE_SETTING_2 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_2_LAG0_BALANCE_MODE_MASK_LSP, DVA_LAG0_BALANCE_MODE_SETTING_2_LAG0_BALANCE_MODE_MASK_LEN, pBalanceMode));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set ether type value of ether type balance mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for setting balance ether type.
 * @param[in] etherType Ether type value (0 to 65535) \n
 *            Indicate ether type value.
 * @param[out] None
 * @retval RT_ERR_OK Set ether type value of ether type balance mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the ether type value of ether type balance mode to distribute packets by round-robin of group members.
 */
RtkApiRet dva_lag_balance_ethertype_set(uint32 unit, uint32 group, uint16 etherType)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_BALANCE_MODE_SETTING_0 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_ETHERTYPE_LSP, DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_ETHERTYPE_LEN, (uint32)etherType));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get ether type value of ether type balance mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for getting balance ether type.
 * @param[out] pEtherType Ether type value (0 to 65535) \n
 *             Indicate ether type value.
 * @retval RT_ERR_OK Get ether type value of ether type balance mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the ether type value of ether type balance mode.
 */
RtkApiRet dva_lag_balance_ethertype_get(uint32 unit, uint32 group, uint16 *pEtherType)
{
    RtkApiRet ret = RT_ERR_OK;
    uint32    val = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pEtherType)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_BALANCE_MODE_SETTING_0 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_ETHERTYPE_LSP, DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_ETHERTYPE_LEN, &val));
        *pEtherType = (uint16)val;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set protocol value of protocol balance mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for setting balance protocol.
 * @param[in] protocol Protocol Value (0 to 255) \n
 *            Indicate protocol value.
 * @param[out] None
 * @retval RT_ERR_OK Set protocol value of protocol balance mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the protocol value of protocol balance mode to distribute packets by round-robin of group members.
 */
RtkApiRet dva_lag_balance_protocol_set(uint32 unit, uint32 group, uint8 protocol)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_BALANCE_MODE_SETTING_0 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_PROTOCOL_LSP, DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_PROTOCOL_LEN, (uint32)protocol));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get protocol value of protocol balance mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for getting balance protocol.
 * @param[out] pProtocol Protocol Value (0 to 255) \n
 *             Indicate protocol value.
 * @retval RT_ERR_OK Get protocol value of protocol balance mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the protocol value of protocol balance mode.
 */
RtkApiRet dva_lag_balance_protocol_get(uint32 unit, uint32 group, uint8 *pProtocol)
{
    RtkApiRet ret = RT_ERR_OK;
    uint32    val = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == pProtocol)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_BALANCE_MODE_SETTING_0 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_PROTOCOL_LSP, DVA_LAG0_BALANCE_MODE_SETTING_0_LAG0_BALANCE_MODE_PROTOCOL_LEN, &val));
        *pProtocol = (uint8)val;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set L4 destination port value/mask of L4 destination port balance mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for setting balance L4 destination port.
 * @param[in] l4Dport L4 Destination Port Value (0 to 65535) \n
 *            Indicate L4 destination port value.
 * @param[in] l4DportMask L4 destination Port Mask Value (0 to 65535) \n
 *            Indicate L4 destination port mask value.
 * @param[out] None
 * @retval RT_ERR_OK Set L4 destination port value/mask of L4 destination port balance mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval Others Refer to dva_reg_field_write() for the error.
 * @note This API sets the L4 destination port value/mask of L4 destination port balance mode to distribute packets by round-robin of group members.
 */
RtkApiRet dva_lag_balance_l4dport_set(uint32 unit, uint32 group, uint16 l4Dport, uint16 l4DportMask)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_BALANCE_MODE_SETTING_1 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_LSP, DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_LEN, (uint32)l4Dport));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_LAG0_BALANCE_MODE_SETTING_1 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_MASK_LSP, DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_MASK_LEN, (uint32)l4DportMask));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get L4 destination port value of L4 destination port balance mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] group Group Identification (1 to 8) \n
 *            Indicate group ID for getting balance L4 destination port.
 * @param[out] pL4Dport L4 Destination Port Value (0 to 65535) \n
 *             Indicate L4 destination port value.
 * @param[out] pL4DportMask L4 Destination Port Mask Value (0 to 65535) \n
 *             Indicate L4 destination port mask value.
 * @retval RT_ERR_OK Get L4 destination port value of L4 destination port balance mode successfully.
 * @retval -RT_ERR_OUT_OF_RANGE The group id is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_field_read() for the error.
 * @note This API gets the L4 destination port value/mask of L4 destination port balance mode.
 */
RtkApiRet dva_lag_balance_l4dport_get(uint32 unit, uint32 group, uint16 *pL4Dport, uint16 *pL4DportMask)
{
    RtkApiRet ret = RT_ERR_OK;
    uint32    val = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if((RTK_LA_MAX_NUM_OF_GROUP < group) || (RTK_LA_MIN_NUM_OF_GROUP > group))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pL4Dport) || (NULL == pL4DportMask))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get LAG balance mode setting register. */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_BALANCE_MODE_SETTING_1 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_LSP, DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_LEN, &val));
        *pL4Dport = (uint16)val;
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_LAG0_BALANCE_MODE_SETTING_1 + ((group - 1u) * 0xCu), DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_MASK_LSP, DVA_LAG0_BALANCE_MODE_SETTING_1_LAG0_BALANCE_MODE_L4_DPORT_MASK_LEN, &val));
        *pL4DportMask = (uint16)val;
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* LAG_BALANCE_ASIC */
/**@}*/ /* LAG_BALANCE */

/**@}*/
