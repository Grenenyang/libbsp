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

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_regField_list.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtkas_debug.h"
#include "rtl907xd_v1_asicdrv_svlan.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"

/**
 * @brief Set SVLAN support frame type
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[in] acceptFrameType accept frame type
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @note The API can set the port SVLAN accept frame type.
 *     The accept frame type as following:
 *         EM_ACCEPT_FRAME_TYPE_ALL
 *         EM_ACCEPT_FRAME_TYPE_TAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_NONE
 */
RtkApiRet dv1_drv_svlan_port_acpt_type_set(uint32 unit, RtkPort port, VlanAcceptFrameType_e acceptFrameType)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    uint32    acctag  = 0u;
    uint32    accutag = 0u;

    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    if(RT_ERR_OK == retVal)
    {
        switch(acceptFrameType)
        {
            case EM_ACCEPT_FRAME_TYPE_ALL:
                acctag  = 1u;
                accutag = 1u;
                break;
            case EM_ACCEPT_FRAME_TYPE_TAG_ONLY:
                acctag  = 1u;
                accutag = 0u;
                break;
            case EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY:
                acctag  = 0u;
                accutag = 1u;
                break;
            case EM_ACCEPT_FRAME_TYPE_NONE:
                acctag  = 0u;
                accutag = 0u;
                break;
            default:
                retVal = -RT_ERR_VLAN_ACCEPT_FRAME_TYPE;
                break;
        }
    }
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOTAG_LSP, DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOTAG_LEN, acctag, &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOUTAG_LSP, DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOUTAG_LEN, accutag, &regVal));
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}

/**
 * @brief Get SVLAN support frame type
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[in] pAcceptFrameType accept frame type
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_SMI
 * @note The API can get the port SVLAN accept frame type.
 *     The accept frame type as following:
 *         EM_ACCEPT_FRAME_TYPE_ALL
 *         EM_ACCEPT_FRAME_TYPE_TAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY
 *         EM_ACCEPT_FRAME_TYPE_NONE
 */
RtkApiRet dv1_drv_svlan_port_acpt_type_get(uint32 unit, RtkPort port, VlanAcceptFrameType_e *pAcceptFrameType)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal;
    uint32    acctag;
    uint32    accutag;

    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_get(DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOTAG_LSP, DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOTAG_LEN, &acctag, regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_get(DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOUTAG_LSP, DV1_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTOUTAG_LEN, &accutag, regVal));

    if(RT_ERR_OK == retVal)
    {
        if((1u == acctag) && (1u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_ALL;
        }
        else if((1u == acctag) && (0u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_TAG_ONLY;
        }
        else if((0u == acctag) && (1u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY;
        }
        else if((0u == acctag) && (0u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_NONE;
        }
        else
        {
            /* do nothing */
        }
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] enabled
 * @retval
 */
RtkApiRet dv1_drv_svlan_port_tag_aware_set(uint32 unit, RtkPort port, uint32 enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_IGNORE_VLAN_TAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREOTAGVID_LSP, DV1_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREOTAGVID_LEN, (0u != enabled) ? 0u : 1u));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pEnabled
 * @retval
 */
RtkApiRet dv1_drv_svlan_port_tag_aware_get(uint32 unit, RtkPort port, uint32 *pEnabled)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_IGNORE_VLAN_TAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREOTAGVID_LSP, DV1_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREOTAGVID_LEN, &fieldVal));

    if(RT_ERR_OK == retVal)
    {
        if(0u != fieldVal)
        {
            *pEnabled = 0u;
        }
        else
        {
            *pEnabled = 1u;
        }
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] enabled
 * @retval
 */
RtkApiRet dv1_drv_svlan_pkt_parser_en_set(uint32 unit, RtkPort port, uint32 enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_PACKET_PARSER_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_PACKET_PARSER_CONTROL_OTAGEN_LSP, DV1_PORT0_PACKET_PARSER_CONTROL_OTAGEN_LEN, enabled));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pEnabled
 * @retval
 */
RtkApiRet dv1_drv_svlan_pkt_parser_en_get(uint32 unit, RtkPort port, uint32 *pEnabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_PACKET_PARSER_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_PACKET_PARSER_CONTROL_OTAGEN_LSP, DV1_PORT0_PACKET_PARSER_CONTROL_OTAGEN_LEN, pEnabled));

    return retVal;
}

/**
 * @brief Set SVLAN egress tag mode
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[in] tagRxu the pkts whose souce port is UNI port tx tag status
 * @param[in] tagRxn the pkts whose souce port is NNI port tx tag status
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_INPUT
 * @retval -RT_ERR_SMI
 * @note The API is used for setting SVLAN egress tag mode
 *    The accept frame type as following:
 *    EM_VLAN_TAG_MODE_ORIGINAL
 *    EM_VLAN_TAG_MODE_KEEP_FORMAT
 *    EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT
 */
RtkApiRet dv1_drv_svlan_egr_tag_mode_set(uint32 unit, RtkPort port, VlanTagMode_e tagRxu, VlanTagMode_e tagRxn)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    fieldVal1 = 0u;
    uint32    fieldVal2 = 0u;

    if((tagRxu >= EM_VLAN_TAG_MODE_END) || (tagRxn >= EM_VLAN_TAG_MODE_END))
    {
        retVal = -RT_ERR_INPUT;
    }
    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    if(RT_ERR_OK == retVal)
    {
        switch(tagRxu)
        {
            case EM_VLAN_TAG_MODE_ORIGINAL:
                fieldVal1 = 0u;
                break;
            case EM_VLAN_TAG_MODE_KEEP_FORMAT:
                fieldVal1 = 1u;
                break;
            case EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT:
                fieldVal1 = 2u;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    if(RT_ERR_OK == retVal)
    {
        switch(tagRxn)
        {
            case EM_VLAN_TAG_MODE_ORIGINAL:
                fieldVal2 = 0u;
                break;
            case EM_VLAN_TAG_MODE_KEEP_FORMAT:
                fieldVal2 = 1u;
                break;
            case EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT:
                fieldVal2 = 2u;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_TX_TAG_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_TX_TAG_CONTROL0_RXUTX_OTAG_KEEP_LSP, DV1_PORT0_TX_TAG_CONTROL0_RXUTX_OTAG_KEEP_LEN, fieldVal1));
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_TX_TAG_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_TX_TAG_CONTROL0_RXNTX_OTAG_KEEP_LSP, DV1_PORT0_TX_TAG_CONTROL0_RXNTX_OTAG_KEEP_LEN, fieldVal2));

    return retVal;
}

/**
 * @brief Get SVLAN egress tag mode
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[out] pTagRxu the pkts whose souce port is UNI port tx tag status
 * @param[out] pTagRxn the pkts whose souce port is NNI port tx tag status
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_SMI
 * @note The API is used for setting SVLAN egress tag mode
 *    The accept frame type as following:
 *    EM_VLAN_TAG_MODE_ORIGINAL
 *    EM_VLAN_TAG_MODE_KEEP_FORMAT
 *    EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT
 */
RtkApiRet dv1_drv_svlan_egr_tag_mode_get(uint32 unit, RtkPort port, VlanTagMode_e *pTagRxu, VlanTagMode_e *pTagRxn)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    if((NULL == pTagRxu) || (NULL == pTagRxn))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_TX_TAG_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_TX_TAG_CONTROL0_RXUTX_OTAG_KEEP_LSP, DV1_PORT0_TX_TAG_CONTROL0_RXUTX_OTAG_KEEP_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        switch(fieldVal)
        {
            case 0:
                *pTagRxu = EM_VLAN_TAG_MODE_ORIGINAL;
                break;
            case 1u:
                *pTagRxu = EM_VLAN_TAG_MODE_KEEP_FORMAT;
                break;
            case 2u:
                *pTagRxu = EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT;
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_TX_TAG_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_TX_TAG_CONTROL0_RXNTX_OTAG_KEEP_LSP, DV1_PORT0_TX_TAG_CONTROL0_RXNTX_OTAG_KEEP_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        switch(fieldVal)
        {
            case 0:
                *pTagRxn = EM_VLAN_TAG_MODE_ORIGINAL;
                break;
            case 1u:
                *pTagRxn = EM_VLAN_TAG_MODE_KEEP_FORMAT;
                break;
            case 2u:
                *pTagRxn = EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT;
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get egress port SVLAN tag status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[in] tagRxu tx SVLAN tag status for the pkts received from UNI port
 * @param[in] tagRxn tx SVLAN tag status for the pkts received from NNI port
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_ENABLE
 * @note The API sets egree port transimiting pakcet with or without
 *      SVLAN tag. Its priority is lower than ACL, and equial to VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
RtkApiRet dv1_drv_svlan_port_egr_stat_set(uint32 unit, RtkPort port, VlanTxTagStatus_e tagRxu, VlanTxTagStatus_e tagRxn)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regVal    = 0u;
    uint32    rxuFileld = 0u;
    uint32    rxnFileld = 0u;

    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_get(DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LSP, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LEN, &rxuFileld, regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_get(DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LSP, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LEN, &rxnFileld, regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LSP, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LEN, ((int32)EM_TAG_STATUS_WITH_TAG == (int32)tagRxu) ? (rxuFileld & (~0x2u)) : (rxuFileld | 0x2u), &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LSP, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LEN, ((int32)EM_TAG_STATUS_WITH_TAG == (int32)tagRxn) ? (rxnFileld & (~0x2u)) : (rxnFileld | 0x2u), &regVal));
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}

/**
 * @brief Get egress port SVLAN tag status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[out] pTagRxu the pointer of tx SVLAN tag status for the pkts received from UNI port
 * @param[out] pTagRxn the pointer of tx SVLAN tag status for the pkts received from NNI port
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_ENABLE
 * @note The API gets egree port transimiting pakcet with or without
 *      SVLAN tags. Its priority is lower than ACL, and equial to VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
RtkApiRet dv1_drv_svlan_port_egr_stat_get(uint32 unit, RtkPort port, VlanTxTagStatus_e *pTagRxu, VlanTxTagStatus_e *pTagRxn)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LSP, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LEN, &fieldVal));
    if((RtkApiRet)RT_ERR_OK == retVal)
    {
        if(0x2u == (fieldVal & 0x2u))
        {
            *pTagRxu = EM_TAG_STATUS_WITHOUT_TAG;
        }
        else
        {
            *pTagRxu = EM_TAG_STATUS_WITH_TAG;
        }
    }
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LSP, DV1_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LEN, &fieldVal));
    if((RtkApiRet)RT_ERR_OK == retVal)
    {
        if(0x2u == (fieldVal & 0x2u))
        {
            *pTagRxn = EM_TAG_STATUS_WITHOUT_TAG;
        }
        else
        {
            *pTagRxn = EM_TAG_STATUS_WITH_TAG;
        }
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] enabled
 * @retval
 */
RtkApiRet dv1_drv_svlan_fwd_base_ovid_set(uint32 unit, RtkPort port, uint32 enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_FORWARDING_TAG_SELECT_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_FORWARDING_TAG_SELECT_CONTROL_FWDBASE_OVID_LSP, DV1_PORT0_FORWARDING_TAG_SELECT_CONTROL_FWDBASE_OVID_LEN, enabled));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pEnabled
 * @retval
 */
RtkApiRet dv1_drv_svlan_fwd_base_ovid_get(uint32 unit, RtkPort port, uint32 *pEnabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_FORWARDING_TAG_SELECT_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_FORWARDING_TAG_SELECT_CONTROL_FWDBASE_OVID_LSP, DV1_PORT0_FORWARDING_TAG_SELECT_CONTROL_FWDBASE_OVID_LEN, pEnabled));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] svid
 * @param[in] pri
 * @param[in] dei
 * @retval
 */
RtkApiRet dv1_drv_svlan_port_pvid_set(uint32 unit, RtkPort port, uint32 svid, uint32 pri, uint32 dei)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if(svid > DV1_VLAN_VID_MAX)
    {
        retVal = -RT_ERR_SVLAN_VID;
    }
    else if(pri > DV1_VLAN_PRI_MAX)
    {
        retVal = -RT_ERR_VLAN_PRIORITY;
    }
    else if(dei > DV1_VLAN_DEI_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }
    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_VLAN_CONTROL_POVID_LSP, DV1_PORT0_VLAN_CONTROL_POVID_LEN, svid, &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_VLAN_CONTROL_POPRI_LSP, DV1_PORT0_VLAN_CONTROL_POPRI_LEN, pri, &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_VLAN_CONTROL_PODEI_LSP, DV1_PORT0_VLAN_CONTROL_PODEI_LEN, dei, &regVal));
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pSvid
 * @param[in] pPri
 * @param[in] pDei
 * @retval
 */
RtkApiRet dv1_drv_svlan_port_pvid_get(uint32 unit, RtkPort port, uint32 *pSvid, uint32 *pPri, uint32 *pDei)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if((NULL == pSvid) || (NULL == pPri) || (NULL == pDei))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }
    CHK_FUN_RET(retVal, dv1_drv_svlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_get(unit, DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_get(DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_VLAN_CONTROL_POVID_LSP, DV1_PORT0_VLAN_CONTROL_POVID_LEN, pSvid, regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_get(DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_VLAN_CONTROL_POPRI_LSP, DV1_PORT0_VLAN_CONTROL_POPRI_LEN, pPri, regVal));
    CHK_FUN_RET(retVal, dv1_reg_field_get(DV1_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]), DV1_PORT0_VLAN_CONTROL_PODEI_LSP, DV1_PORT0_VLAN_CONTROL_PODEI_LEN, pDei, regVal));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] tpid
 * @retval
 */
RtkApiRet dv1_drv_svlan_tpid_set(uint32 unit, uint32 tpid)
{
    return dv1_reg_field_write(unit, DV1_PACKET_PARSER_PID_CONTROL_REGIST_0, DV1_PACKET_PARSER_PID_CONTROL_REGIST_0_SPID_LSP, DV1_PACKET_PARSER_PID_CONTROL_REGIST_0_SPID_LEN, tpid & 0xffffu);
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] pTpid
 * @retval
 */
RtkApiRet dv1_drv_svlan_tpid_get(uint32 unit, uint32 *pTpid)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldval = 0u;

    CHK_FUN_RET(retVal, dv1_reg_field_read(unit, DV1_PACKET_PARSER_PID_CONTROL_REGIST_0, DV1_PACKET_PARSER_PID_CONTROL_REGIST_0_SPID_LSP, DV1_PACKET_PARSER_PID_CONTROL_REGIST_0_SPID_LEN, &fieldval));

    if((RtkApiRet)RT_ERR_OK == retVal)
    {
        *pTpid = fieldval & 0xffffu;
    }

    return retVal;
}

/**
 * @brief Check if inputed user port is vaild
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port number
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 */
RtkApiRet dv1_drv_svlan_check_valid_user_port(uint32 unit, const RtkPort port)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port != (RtkPort)DV1_RTK_CPU_PORT) && ((port > (RtkPort)DV1_RTK_USER_PORT_END) || (port < (RtkPort)DV1_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }

    return retVal;
}
