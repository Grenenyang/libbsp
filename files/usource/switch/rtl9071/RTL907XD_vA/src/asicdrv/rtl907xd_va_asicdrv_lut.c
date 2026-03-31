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
#include "rtl907xd_va_api.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xd_va_asicdrv.h"
#include "rtl907xd_va_asicdrv_lut.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtkas_timer.h"
#include "rtl907xd_va_asicdrv_timer.h"
#include "rtl907xd_va_api_esm.h"

/* calculate index using hash algorithm */
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @param[in] index
 * @retval
 */
RtkApiRet dva_drv_lut_hash0_unicast_index_get(uint32 unit, const dva_LutAsicUniParam_t *const param, const uint8 idx, uint16 *const index)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint16    tmp    = 0u;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    /*mac[5] is mapping to MAC07-MAC00*/
    /*mac[4] is mapping to MAC15-MAC08*/
    /*mac[3] is mapping to MAC23-MAC16*/
    /*mac[2] is mapping to MAC31-MAC24*/
    /*mac[1] is mapping to MAC39-MAC32*/
    /*mac[0] is mapping to MAC47-MAC40*/
    if(RT_ERR_OK == retVal)
    {
        key[0] = (((uint16)param->mac[4] & 0x01u) << 8u) | ((uint16)param->mac[5]);
        key[1] = (((uint16)param->mac[3] & 0x03u) << 7u) | ((uint16)param->mac[4] >> 1u);
        key[2] = (((uint16)param->mac[2] & 0x07u) << 6u) | ((uint16)param->mac[3] >> 2u);
        key[3] = (((uint16)param->mac[1] & 0x0Fu) << 5u) | ((uint16)param->mac[2] >> 3u);
        key[4] = (((uint16)param->mac[0] & 0x1Fu) << 4u) | ((uint16)param->mac[1] >> 4u);
        /*FID: 4-bit width, CVID: 12-bit width*/
        tmp    = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? (uint16)param->fid : ((uint16)param->vid & 0x3Fu);
        key[5] = (tmp << 3u) | ((uint16)param->mac[0] >> 5u);
        key[6] = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? 0u : (param->vid >> 6u);
        *index = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        *index = (uint16)((uint16)(*index << 3u) | (uint16)idx);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @param[in] index
 * @retval
 */
RtkApiRet dva_drv_lut_hash0_multicast_index_get(uint32 unit, const dva_LutAsicMultiParam_t *const param, const uint8 idx, uint16 *const index)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    ulfid    = 0u;
    uint32    fixmcfid = 0u;
    uint32    mcfid    = 0u;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_FIXMCFID_LSP, DVA_VLAN_CONTROL0_FIXMCFID_LEN, &fixmcfid));
    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MCFID_LSP, DVA_VLAN_CONTROL0_MCFID_LEN, &mcfid));
    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MLFID_LSP, DVA_VLAN_CONTROL0_MLFID_LEN, &ulfid));
    if(RT_ERR_OK == retVal)
    {
        key[0] = (((uint16)param->mac[4] & 0x01u) << 8u) | ((uint16)param->mac[5]);
        key[1] = (((uint16)param->mac[3] & 0x03u) << 7u) | ((uint16)param->mac[4] >> 1u);
        key[2] = (((uint16)param->mac[2] & 0x07u) << 6u) | ((uint16)param->mac[3] >> 2u);
        key[3] = (((uint16)param->mac[1] & 0x0Fu) << 5u) | ((uint16)param->mac[2] >> 3u);
        key[4] = (((uint16)param->mac[0] & 0x1Fu) << 4u) | ((uint16)param->mac[1] >> 4u);

        if(1u == fixmcfid)
        {
            key[5] = (((uint16)(mcfid & 0xffffu) & 0x3Fu) << 3u) | ((uint16)param->mac[0] >> 5u);
            key[6] = ((uint16)(mcfid & 0xffffu)) >> 6u;
        }
        else if(0u == fixmcfid)
        {
            if((uint32)DVA_HASH_WITH_CVID == ulfid)
            {
                key[5] = (((uint16)param->vid & 0x3Fu) << 3u) | ((uint16)param->mac[0] >> 5u);
                key[6] = param->vid >> 6u;
            }
            else if((uint32)DVA_HASH_WITH_FID == ulfid)
            {
                key[5] = (((uint16)param->fid & 0xFu) << 3u) | ((uint16)param->mac[0] >> 5u);
                key[6] = 0;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* do nothing */
        }

        *index = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        *index = (uint16)((uint16)(*index << 3u) | (uint16)idx);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @param[in] index
 * @retval
 */
RtkApiRet dva_drv_lut_hash0_ip_index_get(uint32 unit, const dva_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const index)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        key[0] = (((uint16)param->sip[2] & 0x01u) << 8u) | ((uint16)param->sip[3]);
        key[1] = (((uint16)param->sip[1] & 0x03u) << 7u) | ((uint16)param->sip[2] >> 1u);
        key[2] = (((uint16)param->sip[0] & 0x07u) << 6u) | ((uint16)param->sip[1] >> 2u);
        key[3] = (((uint16)param->dip[3] & 0x0Fu) << 5u) | ((uint16)param->sip[0] >> 3u);
        key[4] = (((uint16)param->dip[2] & 0x1Fu) << 4u) | ((uint16)param->dip[3] >> 4u);
        key[5] = (((uint16)param->dip[1] & 0x3Fu) << 3u) | ((uint16)param->dip[2] >> 5u);
        key[6] = (((uint16)param->dip[0] & 0x0Fu) << 2u) | ((uint16)param->dip[1] >> 6u);

        *index = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        *index = (uint16)((uint16)(*index << 3u) | (uint16)idx);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @param[in] index
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_hash_group_index_get(uint32 unit, const dva_LutAsicGroupParam_t *const param, const uint8 idx, uint16 *const index)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        key[0] = ((uint16)param->dip[3] & 0x0Fu);
        key[1] = (((uint16)param->dip[3] >> 4u) & 0x0Fu);
        key[2] = ((uint16)param->dip[2] & 0x0Fu);
        key[3] = (((uint16)param->dip[2] >> 4u) & 0x0Fu);
        key[4] = ((uint16)param->dip[1] & 0x0Fu);
        key[5] = (((uint16)param->dip[1] >> 4u) & 0x0Fu);
        key[6] = ((uint16)param->dip[0] & 0x0Fu);

        *index = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        *index = (uint16)((uint16)(*index << 3u) | (uint16)idx);
    }

    return retVal;
}

/* restore the bits in LUT using reverse hash algorithm */
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_hash0_unicast_restore(uint32 unit, const uint16 index, dva_LutAsicUniParam_t *const param)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint16    restoreBits = 0x0u;
    uint16    tmp         = 0;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        key[0] = (index >> 3);
        key[1] = (((uint16)param->mac[3] & 0x03u) << 7u) | ((uint16)param->mac[4] >> 1u);
        key[2] = (((uint16)param->mac[2] & 0x07u) << 6u) | ((uint16)param->mac[3] >> 2u);
        key[3] = (((uint16)param->mac[1] & 0x0Fu) << 5u) | ((uint16)param->mac[2] >> 3u);
        key[4] = (((uint16)param->mac[0] & 0x1Fu) << 4u) | ((uint16)param->mac[1] >> 4u);

        tmp    = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? (uint16)param->fid : ((uint16)param->vid & 0x3Fu);
        key[5] = (tmp << 3u) | ((uint16)param->mac[0] >> 5u);
        key[6] = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? 0u : (param->vid >> 6u);

        restoreBits = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        param->mac[4] &= 0xFEu;
        param->mac[4] |= (uint8)((restoreBits >> 8u) & 0x01u);
        param->mac[5] = (uint8)(restoreBits & 0xFFu);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_hash0_multi_restore(uint32 unit, const uint16 index, dva_LutAsicMultiParam_t *const param)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint16    restoreBits = 0x0u;
    uint32    fixmcfid    = 0;
    uint32    mcfid       = 0;
    uint32    ulfid       = 0;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_FIXMCFID_LSP, DVA_VLAN_CONTROL0_FIXMCFID_LEN, &fixmcfid));
    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MCFID_LSP, DVA_VLAN_CONTROL0_MCFID_LEN, &mcfid));
    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MLFID_LSP, DVA_VLAN_CONTROL0_MLFID_LEN, &ulfid));

    if(RT_ERR_OK == retVal)
    {
        key[0] = (index >> 3u);
        key[1] = (((uint16)param->mac[3] & 0x03u) << 7u) | ((uint16)param->mac[4] >> 1u);
        key[2] = (((uint16)param->mac[2] & 0x07u) << 6u) | ((uint16)param->mac[3] >> 2u);
        key[3] = (((uint16)param->mac[1] & 0x0Fu) << 5u) | ((uint16)param->mac[2] >> 3u);
        key[4] = (((uint16)param->mac[0] & 0x1Fu) << 4u) | ((uint16)param->mac[1] >> 4u);

        if(fixmcfid == 1u)
        {
            key[5] = (uint16)((mcfid & 0xFu) << 3u) | ((uint16)param->mac[0] >> 5u);
            key[6] = 0u;
        }
        else if(fixmcfid == 0u)
        {
            if((uint32)DVA_HASH_WITH_CVID == ulfid)
            {
                key[5] = ((param->vid & 0x3Fu) << 3u) | ((uint16)param->mac[0] >> 5u);
                key[6] = param->vid >> 6u;
            }
            else if((uint32)DVA_HASH_WITH_FID == ulfid)
            {
                key[5] = (((uint16)param->fid & 0xFu) << 3u) | ((uint16)param->mac[0] >> 5u);
                key[6] = 0u;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* do nothing */
        }

        restoreBits = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        param->mac[4] &= 0xFEu;
        param->mac[4] |= (uint8)((restoreBits >> 8u) & 0x01u);
        param->mac[5] = (uint8)(restoreBits & 0xFFu);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_hash0_ip_restore(uint32 unit, const uint16 index, dva_LutAsicIpMultiParam_t *const param)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint16    restoreBits = 0x0u;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        key[0] = index >> 3u;
        key[1] = (((uint16)param->sip[1] & 0x03u) << 7u) | ((uint16)param->sip[2] >> 1u);
        key[2] = (((uint16)param->sip[0] & 0x07u) << 6u) | ((uint16)param->sip[1] >> 2u);
        key[3] = (((uint16)param->dip[3] & 0x0Fu) << 5u) | ((uint16)param->sip[0] >> 3u);
        key[4] = (((uint16)param->dip[2] & 0x1Fu) << 4u) | ((uint16)param->dip[3] >> 4u);
        key[5] = (((uint16)param->dip[1] & 0x3Fu) << 3u) | ((uint16)param->dip[2] >> 5u);
        key[6] = (((uint16)param->dip[0] & 0x0Fu) << 2u) | ((uint16)param->dip[1] >> 6u);

        restoreBits = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);

        param->sip[2] &= 0xFEu;
        param->sip[2] |= (uint8)((restoreBits >> 8u) & 0x01u);
        param->sip[3] = (uint8)(restoreBits & 0xFFu);
    }

    return retVal;
}

/* calculate index using hash algorithm */
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @param[in] index
 * @retval
 */
RtkApiRet dva_drv_lut_hash1_unicast_index_get(uint32 unit, const dva_LutAsicUniParam_t *const param, const uint8 idx, uint16 *const index)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint16    tmp    = 0;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));
    if(RT_ERR_OK == retVal)
    {
        key[0] = (uint16)param->mac[5] | (((uint16)param->mac[4] & 0x01u) << 8u);
        key[1] = (((uint16)param->mac[4] >> 3u) & 0x1Fu) | (((uint16)param->mac[3] & 0x03u) << 5u) | ((((uint16)param->mac[4] >> 1u) & 0x03u) << 7u);
        key[2] = (((uint16)param->mac[3] >> 5u) & 0x07u) | (((uint16)param->mac[2] & 0x07u) << 3u) | ((((uint16)param->mac[3] >> 2u) & 0x07u) << 6u);
        key[3] = (((uint16)param->mac[2] >> 7u) & 0x01u) | (((uint16)param->mac[1] & 0x0Fu) << 1u) | ((((uint16)param->mac[2] >> 3u) & 0x0Fu) << 5u);
        key[4] = (((uint16)param->mac[0] >> 1u) & 0x0Fu) | ((uint16)param->mac[1] & 0xF0u) | (((uint16)param->mac[0] & 0x01u) << 8u);

        tmp    = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? (uint16)param->fid : ((uint16)param->vid & 0x3Fu);
        key[5] = (tmp >> 3u) | ((((uint16)param->mac[0] >> 5u) & 0x07u) << 3u) | ((tmp & 0x07u) << 6u);
        key[6] = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? 0u : ((param->vid >> 11u) | (((param->vid >> 6u) & 0x1Fu) << 1u));

        *index = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        *index = (uint16)((uint16)(*index << 3u) | (uint16)idx);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @param[in] index
 * @retval
 */
RtkApiRet dva_drv_lut_hash1_multicast_index_get(uint32 unit, const dva_LutAsicMultiParam_t *const param, const uint8 idx, uint16 *const index)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    ulfid    = 0;
    uint32    fixmcfid = 0;
    uint32    mcfid    = 0;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_FIXMCFID_LSP, DVA_VLAN_CONTROL0_FIXMCFID_LEN, &fixmcfid));
    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MCFID_LSP, DVA_VLAN_CONTROL0_MCFID_LEN, &mcfid));
    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MLFID_LSP, DVA_VLAN_CONTROL0_MLFID_LEN, &ulfid));

    if(RT_ERR_OK == retVal)
    {
        key[0] = (uint16)param->mac[5] | (((uint16)param->mac[4] & 0x01u) << 8u);
        key[1] = (((uint16)param->mac[4] >> 3u) & 0x1Fu) | (((uint16)param->mac[3] & 0x03u) << 5u) | ((((uint16)param->mac[4] >> 1u) & 0x03u) << 7u);
        key[2] = (((uint16)param->mac[3] >> 5u) & 0x07u) | (((uint16)param->mac[2] & 0x07u) << 3u) | ((((uint16)param->mac[3] >> 2u) & 0x07u) << 6u);
        key[3] = (((uint16)param->mac[2] >> 7u) & 0x01u) | (((uint16)param->mac[1] & 0x0Fu) << 1u) | ((((uint16)param->mac[2] >> 3u) & 0x0Fu) << 5u);
        key[4] = (((uint16)param->mac[0] >> 1u) & 0x0Fu) | (((uint16)param->mac[1] & 0xF0u)) | (((uint16)param->mac[0] & 0x01u) << 8u);

        if(1u == fixmcfid)
        {
            key[5] = ((((uint16)(mcfid & 0xffffu)) >> 3) & 0x01u) | ((((uint16)param->mac[0] >> 5u) & 0x07u) << 3u) | ((((uint16)(mcfid & 0xffffu)) & 0x07u) << 6u);
            key[6] = 0;
        }
        else if(0u == fixmcfid)
        {
            if((uint32)DVA_HASH_WITH_CVID == ulfid)
            {
                key[5] = ((param->vid >> 3u) & 0x07u) | (((param->mac[0] >> 5u) & 0x07u) << 3u) | ((param->vid & 0x07u) << 6u);
                key[6] = ((param->vid >> 11u) | (((param->vid >> 6) & 0x1Fu) << 1u));
            }
            else if((uint32)DVA_HASH_WITH_FID == ulfid)
            {
                key[5] = (((uint16)param->fid >> 3u) & 0x01u) | ((((uint16)param->mac[0] >> 5u) & 0x07u) << 3u) | (((uint16)param->fid & 0x07u) << 6u);
                key[6] = 0;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* do nothing */
        }

        *index = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        *index = (uint16)((uint16)(*index << 3u) | (uint16)idx);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @param[in] index
 * @retval
 */
RtkApiRet dva_drv_lut_hash1_ip_index_get(uint32 unit, const dva_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const index)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        key[0] = (uint16)param->sip[3] | (((uint16)param->sip[2] & 0x01u) << 8u);
        key[1] = (((uint16)param->sip[2] >> 3u) & 0x1Fu) | (((uint16)param->sip[1] & 0x03u) << 5u) | ((((uint16)param->sip[2] >> 1u) & 0x03u) << 7u);
        key[2] = (((uint16)param->sip[1] >> 5u) & 0x07u) | (((uint16)param->sip[0] & 0x07u) << 3u) | ((((uint16)param->sip[1] >> 2u) & 0x07u) << 6u);
        key[3] = (((uint16)param->sip[0] >> 7u) & 0x01u) | (((uint16)param->dip[3] & 0x0Fu) << 1u) | ((((uint16)param->sip[0] >> 3u) & 0x0Fu) << 5u);
        key[4] = (((uint16)param->dip[2] >> 1u) & 0x0Fu) | ((uint16)param->dip[3] & 0xF0u) | (((uint16)param->dip[2] & 0x01u) << 8u);
        key[5] = (((uint16)param->dip[1] >> 3u) & 0x07u) | ((((uint16)param->dip[2] >> 5u) & 0x07u) << 3u) | (((uint16)param->dip[1] & 0x07u) << 6u);
        key[6] = (((uint16)param->dip[0] >> 3u) & 0x01u) | ((((uint16)param->dip[1] >> 6u) & 0x03u) << 1u) | (((uint16)param->dip[0] & 0x07u) << 3u);

        *index = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);
        *index = (uint16)((uint16)(*index << 3u) | (uint16)idx);
    }

    return retVal;
}

/* restore the bits in LUT using reverse hash algorithm */
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_hash1_unicast_restore(uint32 unit, const uint16 index, dva_LutAsicUniParam_t *param)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint16    restoreBits = 0x0u;
    uint16    tmp;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        key[0] = (index >> 3u);
        key[1] = (((uint16)param->mac[4] >> 3u) & 0x1Fu) | (((uint16)param->mac[3] & 0x03u) << 5u) | ((((uint16)param->mac[4] >> 1u) & 0x03u) << 7u);
        key[2] = (((uint16)param->mac[3] >> 5u) & 0x07u) | (((uint16)param->mac[2] & 0x07u) << 3u) | ((((uint16)param->mac[3] >> 2u) & 0x07u) << 6u);
        key[3] = (((uint16)param->mac[2] >> 7u) & 0x01u) | (((uint16)param->mac[1] & 0x0Fu) << 1u) | ((((uint16)param->mac[2] >> 3u) & 0x0Fu) << 5u);
        key[4] = (((uint16)param->mac[0] >> 1u) & 0x0Fu) | ((uint16)param->mac[1] & 0xF0u) | (((uint16)param->mac[0] & 0x01u) << 8u);
        tmp    = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? (uint16)param->fid : ((uint16)param->vid & 0x3Fu);
        key[5] = (tmp >> 3u) | (((param->mac[0] >> 5u) & 0x07u) << 3u) | ((tmp & 0x07u) << 6u);
        key[6] = (param->ulfid == (uint32)DVA_HASH_WITH_FID) ? 0u : ((param->vid >> 11u) | (((param->vid >> 6u) & 0x1Fu) << 1u));

        restoreBits = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);

        param->mac[4] &= 0xFEu;
        param->mac[4] |= (uint8)((restoreBits >> 8u) & 0x01u);
        param->mac[5] = (uint8)(restoreBits & 0xFFu);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_hash1_multi_restore(uint32 unit, const uint16 index, dva_LutAsicMultiParam_t *param)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint16    restoreBits = 0x0u;
    uint32    fixmcfid    = 0;
    uint32    mcfid       = 0;
    uint32    ulfid       = 0;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_FIXMCFID_LSP, DVA_VLAN_CONTROL0_FIXMCFID_LEN, &fixmcfid));
        CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MCFID_LSP, DVA_VLAN_CONTROL0_MCFID_LEN, &mcfid));
        CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MLFID_LSP, DVA_VLAN_CONTROL0_MLFID_LEN, &ulfid));

        key[0] = index >> 3u;
        key[1] = (((uint16)param->mac[4] >> 3u) & 0x1Fu) | (((uint16)param->mac[3] & 0x03u) << 5u) | ((((uint16)param->mac[4] >> 1u) & 0x03u) << 7u);
        key[2] = (((uint16)param->mac[3] >> 5u) & 0x07u) | (((uint16)param->mac[2] & 0x07u) << 3u) | ((((uint16)param->mac[3] >> 2u) & 0x07u) << 6u);
        key[3] = (((uint16)param->mac[2] >> 7u) & 0x01u) | (((uint16)param->mac[1] & 0x0Fu) << 1u) | ((((uint16)param->mac[2] >> 3u) & 0x0Fu) << 5u);
        key[4] = (((uint16)param->mac[0] >> 1u) & 0x0Fu) | ((uint16)param->mac[1] & 0xF0u) | (((uint16)param->mac[0] & 0x01u) << 8u);

        if(1u == fixmcfid)
        {
            key[5] = (((uint16)param->fid >> 3u) & 0x01u) | (((uint16)param->fid & 0x07u) << 6u);
            key[6] = 0u;
        }
        else if(0u == fixmcfid)
        {
            if((uint32)DVA_HASH_WITH_CVID == ulfid)
            {
                key[5] = ((param->vid >> 3u) & 0x07u) | ((((uint16)param->mac[0] >> 5u) & 0x07u) << 3u) | (((uint16)param->vid & 0x07u) << 6u);
                key[6] = (param->vid >> 11u) | (((param->vid >> 6u) & 0x1Fu) << 1u);
            }
            else if((uint32)DVA_HASH_WITH_FID == ulfid)
            {
                key[5] = (((uint16)param->fid >> 3u) & 0x07u) | ((((uint16)param->mac[0] >> 5u) & 0x07u) << 3u) | ((param->fid & 0x07u) << 6u);
                key[6] = (((uint16)param->fid >> 6u) & 0x1Fu) << 1u;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* do nothing */
        }

        restoreBits = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);

        param->mac[4] &= 0xFEu;
        param->mac[4] |= (uint8)((restoreBits >> 8u) & 0x01u);
        param->mac[5] = (uint8)(restoreBits & 0xFFu);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_hash1_ip_restore(uint32 unit, const uint16 index, dva_LutAsicIpMultiParam_t *const param)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint16    restoreBits = 0x0u;
    uint16    key[DVA_LUT_BUCKET_KEY_LEN];

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(key, 0, sizeof(key)));

    if(RT_ERR_OK == retVal)
    {
        key[0] = (index >> 3u);
        key[1] = (((uint16)param->sip[2] >> 3u) & 0x1Fu) | (((uint16)param->sip[1] & 0x03u) << 5u) | ((((uint16)param->sip[2] >> 1u) & 0x03u) << 7u);
        key[2] = (((uint16)param->sip[1] >> 5u) & 0x07u) | (((uint16)param->sip[0] & 0x07u) << 3u) | ((((uint16)param->sip[1] >> 2u) & 0x07u) << 6u);
        key[3] = (((uint16)param->sip[0] >> 7u) & 0x01u) | (((uint16)param->dip[3] & 0x0Fu) << 1u) | ((((uint16)param->sip[0] >> 3u) & 0x0Fu) << 5u);
        key[4] = (((uint16)param->dip[2] >> 1u) & 0x0Fu) | ((uint16)param->dip[3] & 0xF0u) | (((uint16)param->dip[2] & 0x01u) << 8u);
        key[5] = (((uint16)param->dip[1] >> 3u) & 0x07u) | ((((uint16)param->dip[2] >> 5u) & 0x07u) << 3u) | (((uint16)param->dip[1] & 0x07u) << 6u);
        key[6] = (((uint16)param->dip[0] >> 3u) & 0x01u) | ((((uint16)param->dip[1] >> 6u) & 0x03u) << 1u) | (((uint16)param->dip[0] & 0x07u) << 3u);

        restoreBits = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);

        restoreBits = (uint16)(((key[0] & 0xffffu) ^ (key[1] & 0xffffu) ^ (key[2] & 0xffffu) ^ (key[3] & 0xffffu) ^ (key[4] & 0xffffu) ^ (key[5] & 0xffffu) ^ (key[6] & 0xffffu)) & 0xffffu);

        param->sip[2] &= 0xFEu;
        param->sip[2] |= (uint8)((restoreBits >> 8u) & 0x01u);
        param->sip[3] = (uint8)(restoreBits & 0xFFu);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] table
 * @param[in] entry
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_unicast_param2entry(uint32 unit, const dva_table_list_t table, dva_LutTblAsicEntry_t *const entry, const dva_LutAsicUniParam_t *const param)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     fid    = 0u;

    if((uint32)DVA_HASH_WITH_CVID == param->ulfid)
    {
        fid = DVA_LUT_INVALID_FID;
    }
    else
    {
        fid = param->fid;
    }

    retVal = dva_drv_memset(entry, 0, sizeof(*entry));
    if(RT_ERR_OK == retVal)
    {
        entry->val[0] = (((uint32)param->nh & 0x1u) << 3u) | (((uint32)fid & 0xFu) << 4u) | (((uint32)param->bstatic & 0x1u) << 8u) | (((uint32)param->auth & 0x1u) << 9u) | (((uint32)param->dablk & 0x1u) << 10u) | (((uint32)param->sablk & 0x1u) << 11u) | (((uint32)param->spa & 0xFu) << 12u) | (((uint32)param->age & 0x3FFu) << 16u);
        entry->val[1] = (uint32)0 << 0u; /* L3Entry = 0 */

        switch(table)
        {
            case DVA_L2_TABLE:
                entry->val[1] = entry->val[1] | ((uint32)param->mac[4] & 0xFEu) | ((uint32)param->mac[3] << 8u) | ((uint32)param->mac[2] << 16u) | ((uint32)param->mac[1] << 24u);
                entry->val[2] = (uint32)param->mac[0] | (((uint32)param->vid & 0xFFFu) << 8u); /*bit66*/
                break;
            case DVA_LUT_BCAM:
                entry->val[1] = entry->val[1] | ((uint32)param->mac[5] << 1u) | ((uint32)param->mac[4] << 9u) | ((uint32)param->mac[3] << 17u) | (((uint32)param->mac[2] & 0x7Fu) << 25u);

                entry->val[2] = (((uint32)param->mac[2] & 0x80u) >> 7u) | ((uint32)param->mac[1] << 1u) | ((uint32)param->mac[0] << 9u) | (((uint32)param->vid & 0xFFFu) << 17u); /*bit75*/
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] table
 * @param[in] index
 * @param[in] param
 * @param[in] entry
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_unicast_entry2param(uint32 unit, const uint32 hashAlg, const dva_table_list_t table, const uint32 index, dva_LutAsicUniParam_t *const param, const dva_LutTblAsicEntry_t *const entry)
{
    RtkApiRet retVal = RT_ERR_OK;

    retVal = dva_drv_memset(param, 0, sizeof(dva_LutAsicUniParam_t));
    if(RT_ERR_OK == retVal)
    {
        param->nh  = (uint8)((entry->val[0] >> 3u) & 0x01u);
        param->fid = (uint8)((entry->val[0] >> 4u) & 0x0Fu);
        if(DVA_LUT_INVALID_FID == param->fid) /* hash with CVID */
        {
            param->ulfid = 0u;
        }
        else
        {
            param->ulfid = 1u;
        }

        param->bstatic = (uint8)((entry->val[0] >> 8u) & 0x01u);
        param->auth    = (uint8)((entry->val[0] >> 9u) & 0x01u);
        param->dablk   = (uint8)((entry->val[0] >> 10u) & 0x01u);
        param->sablk   = (uint8)((entry->val[0] >> 11u) & 0x01u);
        param->spa     = (uint8)((entry->val[0] >> 12u) & 0xFu);
        param->age     = (uint16)((entry->val[0] >> 16u) & 0x3FFu);
        param->l3entry = (uint8)((entry->val[1] >> 0u) & 0x01u);

        if(table == DVA_L2_TABLE)
        {
            param->mac[4] = (uint8)((entry->val[1] << 0u) & 0xFEu);
            param->mac[3] = (uint8)((entry->val[1] >> 8u) & 0xFFu);
            param->mac[2] = (uint8)((entry->val[1] >> 16u) & 0xFFu);
            param->mac[1] = (uint8)((entry->val[1] >> 24u) & 0xFFu);
            param->mac[0] = (uint8)(entry->val[2] & 0xFFu);
            param->vid    = (uint16)((entry->val[2] >> 8u) & 0xFFFu);

            if(hashAlg == 0u)
            {
                CHK_FUN_RET(retVal, dva_drv_lut_hash0_unicast_restore(unit, (uint16)(index & 0xffffu), param)); /*restore MAC08-MAC00*/
            }
            else
            {
                CHK_FUN_RET(retVal, dva_drv_lut_hash1_unicast_restore(unit, (uint16)(index & 0xffffu), param));
            }
        }
        else if(table == DVA_LUT_BCAM)
        {
            param->mac[5] = (uint8)((entry->val[1] >> 1u) & 0xFFu);
            param->mac[4] = (uint8)((entry->val[1] >> 9u) & 0xFFu);
            param->mac[3] = (uint8)((entry->val[1] >> 17u) & 0xFFu);
            param->mac[2] = (uint8)(((entry->val[1] >> 25u) & 0x7Fu) | ((entry->val[2] << 7u) & 0x80u));
            param->mac[1] = (uint8)((entry->val[2] >> 1u) & 0xFFu);
            param->mac[0] = (uint8)((entry->val[2] >> 9u) & 0xFFu);
            param->vid    = (uint16)((entry->val[2] >> 17u) & 0xFFFu);
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
 *
 * @param[in] unit
 * @param[in] table
 * @param[in] entry
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_multicast_param2entry(uint32 unit, const dva_table_list_t table, dva_LutTblAsicEntry_t *const entry, const dva_LutAsicMultiParam_t *const param)
{
    RtkApiRet retVal = RT_ERR_OK;

    retVal = dva_drv_memset(entry, 0, sizeof(*entry));
    if(RT_ERR_OK == retVal)
    {
        entry->val[0] = (((uint32)param->fid & 0xFu) << 4u) | ((param->mbr & 0x1FFFFu) << 8u) | (((uint32)param->vleaky & 0x1u) << 25u);
        entry->val[1] = 0; /* L3Entry = 0 */

        switch(table)
        {
            case DVA_L2_TABLE:
                entry->val[1] = entry->val[1] | ((uint32)param->mac[4] & 0xFEu) | ((uint32)param->mac[3] << 8u) | ((uint32)param->mac[2] << 16u) | ((uint32)param->mac[1] << 24u);
                entry->val[2] = (uint32)param->mac[0] | (((uint32)param->vid & 0xFFFu) << 8u); /*bit66*/
                break;
            case DVA_LUT_BCAM:
                entry->val[1] = entry->val[1] | ((uint32)param->mac[5] << 1u) | ((uint32)param->mac[4] << 9u) | ((uint32)param->mac[3] << 17u) | (((uint32)param->mac[2] & 0x7Fu) << 25u);

                entry->val[2] = (((uint32)param->mac[2] & 0x80u) >> 7u) | ((uint32)param->mac[1] << 1u) | ((uint32)param->mac[0] << 9u) | (((uint32)param->vid & 0xFFFu) << 17u); /*bit75*/
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] table
 * @param[in] index
 * @param[in] param
 * @param[in] entry
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_multicast_entry2param(uint32 unit, const uint32 hashAlg, const dva_table_list_t table, const uint16 index, dva_LutAsicMultiParam_t *const param, const dva_LutTblAsicEntry_t *const entry)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param)));
    if(RT_ERR_OK == retVal)
    {
        param->fid     = (uint8)((entry->val[0] >> 4u) & 0x0Fu);
        param->mbr     = (entry->val[0] >> 8u) & 0x1FFFFu;
        param->vleaky  = (uint8)((entry->val[0] >> 25u) & 0x01u);
        param->l3entry = (uint8)(entry->val[1] & 0x01u);

        switch(table)
        {
            case DVA_L2_TABLE:
                param->mac[4] = (uint8)(entry->val[1] & 0xFEu);
                param->mac[3] = (uint8)((entry->val[1] >> 8u) & 0xFFu);
                param->mac[2] = (uint8)((entry->val[1] >> 16u) & 0xFFu);
                param->mac[1] = (uint8)((entry->val[1] >> 24u) & 0xFFu);
                param->mac[0] = (uint8)(entry->val[2] & 0xFFu);
                param->vid    = (uint16)((entry->val[2] >> 8u) & 0xFFFu);

                if(0u == hashAlg)
                {
                    CHK_FUN_RET(retVal, dva_drv_lut_hash0_multi_restore(unit, index, param)); /*restore MAC08-MAC00*/
                }
                else if(1u == hashAlg)
                {
                    CHK_FUN_RET(retVal, dva_drv_lut_hash1_multi_restore(unit, index, param));
                }
                else
                {
                    /* do nothing */
                }
                break;
            case DVA_LUT_BCAM:
                param->mac[5] = (uint8)((entry->val[1] >> 1u) & 0xFFu);
                param->mac[4] = (uint8)((entry->val[1] >> 9u) & 0xFFu);
                param->mac[3] = (uint8)((entry->val[1] >> 17u) & 0xFFu);
                param->mac[2] = (uint8)(((entry->val[1] >> 25u) & 0x7Fu) | ((entry->val[2] << 7u) & 0x80u));
                param->mac[1] = (uint8)((entry->val[2] >> 1u) & 0xFFu);
                param->mac[0] = (uint8)((entry->val[2] >> 9u) & 0xFFu);
                param->vid    = (uint16)((entry->val[2] >> 17u) & 0xFFFu);
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] table
 * @param[in] entry
 * @param[in] param
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_ip_param2entry(uint32 unit, const dva_table_list_t table, dva_LutTblAsicEntry_t *const entry, const dva_LutAsicIpMultiParam_t *const param)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((table != DVA_L2_TABLE) && (table != DVA_LUT_BCAM))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if((NULL == param) || (NULL == entry))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(entry, 0, sizeof(*entry)));
    if(RT_ERR_OK == retVal)
    {
        entry->val[0] = (((uint32)param->fid & 0xFu) << 4u) | ((param->mbr & 0x1FFFFu) << 8u) | (((uint32)param->vleaky & 0x1u) << 25u);
        entry->val[1] = (uint32)1; /* L3Entry = 1 */

        if(DVA_L2_TABLE == table)
        {
            entry->val[1] = entry->val[1] | ((uint32)param->sip[2] & 0xFEu) | ((uint32)param->sip[1] << 8u) | ((uint32)param->sip[0] << 16u) | ((uint32)param->dip[3] << 24u);
            entry->val[2] = (uint32)param->dip[2] | ((uint32)param->dip[1] << 8u) | (((uint32)param->dip[0] & 0x0Fu) << 16u);
        }
        else if(DVA_LUT_BCAM == table)
        {
            entry->val[1] = entry->val[1] | ((uint32)param->sip[3] << 1u) | ((uint32)param->sip[2] << 9u) | ((uint32)param->sip[1] << 17u) | (((uint32)param->sip[0] & 0x7Fu) << 25u);
            entry->val[2] = (((uint32)param->sip[0] & 0x80u) >> 7u) | ((uint32)param->dip[3] << 1u) | ((uint32)param->dip[2] << 9u) | ((uint32)param->dip[1] << 17u) | (((uint32)param->dip[0] & 0x0Fu) << 25u);
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
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] table
 * @param[in] index
 * @param[in] param
 * @param[in] entry
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_ip_entry2param(uint32 unit, const uint32 hashAlg, const dva_table_list_t table, const uint16 index, dva_LutAsicIpMultiParam_t *const param, const dva_LutTblAsicEntry_t *const entry)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((table != DVA_L2_TABLE) && (table != DVA_LUT_BCAM))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if((NULL == param) || (NULL == entry))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param)));
    if(RT_ERR_OK == retVal)
    {
        param->fid     = (uint8)((entry->val[0] >> 4u) & 0x0Fu);
        param->mbr     = (entry->val[0] >> 8u) & 0x1FFFFu;
        param->vleaky  = (uint8)((entry->val[0] >> 25u) & 0x01u);
        param->l3entry = (uint8)(entry->val[1] & 0x01u);

        if(DVA_L2_TABLE == table)
        {
            param->sip[2] = (uint8)(entry->val[1] & 0xFEu);
            param->sip[1] = (uint8)((entry->val[1] >> 8u) & 0xFFu);
            param->sip[0] = (uint8)((entry->val[1] >> 16u) & 0xFFu);
            param->dip[3] = (uint8)((entry->val[1] >> 24u) & 0xFFu);
            param->dip[2] = (uint8)(entry->val[2] & 0xFFu);
            param->dip[1] = (uint8)((entry->val[2] >> 8u) & 0xFFu);
            param->dip[0] = (uint8)((entry->val[2] >> 16u) & 0x0Fu);
            param->dip[0] |= 0xE0u; /*224*/

            if(0u == hashAlg)
            {
                CHK_FUN_RET(retVal, dva_drv_lut_hash0_ip_restore(unit, index, param));
            }
            else if(hashAlg == 1u)
            {
                CHK_FUN_RET(retVal, dva_drv_lut_hash1_ip_restore(unit, index, param));
            }
            else
            {
                /* do nothing */
            }
        }
        else if(DVA_LUT_BCAM == table)
        {
            param->sip[3] = (uint8)((entry->val[1] >> 1u) & 0xFFu);
            param->sip[2] = (uint8)((entry->val[1] >> 9u) & 0xFFu);
            param->sip[1] = (uint8)((entry->val[1] >> 17u) & 0xFFu);
            param->sip[0] = (uint8)(((entry->val[1] >> 25u) & 0x7Fu) | ((entry->val[2] << 7u) & 0x80u));
            param->dip[3] = (uint8)((entry->val[2] >> 1u) & 0xFFu);
            param->dip[2] = (uint8)((entry->val[2] >> 9u) & 0xFFu);
            param->dip[1] = (uint8)((entry->val[2] >> 17u) & 0xFFu);
            param->dip[0] = (uint8)((entry->val[2] >> 25u) & 0x0Fu);
        }
        else
        {
            /* do nothing */
        }
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_param_type_decision(const dva_table_list_t tbl_type, const dva_LutTblAsicEntry_t *const entry, dva_LutAsicEntryType_e *type)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     l3entry   = 0;
    uint8     multicast = 0;

    if((NULL == entry) || (NULL == type))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == retVal)
    {
        l3entry = (uint8)(entry->val[1] & 0x01u);
        if(tbl_type == DVA_L2_TABLE) /*DA bit 40*/
        {
            multicast = (uint8)((entry->val[2]) & 0x01u);
        }
        else
        {
            multicast = (uint8)((entry->val[2] >> 9u) & 0x01u);
        }

        if(l3entry == 1u)
        {
            *type = DVA_EM_LUT_ASIC_IPMULTI;
        }
        else if(multicast == 1u)
        {
            *type = DVA_EM_LUT_ASIC_MULTI;
        }
        else
        {
            if((1u == ((entry->val[0] >> 8u) & 0x01u)) ||  /* static entry    */
               (1u == ((entry->val[0] >> 9u) & 0x01u)) ||  /* auth = 1        */
               (1u == ((entry->val[0] >> 10u) & 0x01u)) || /* dablk= 1        */
               (1u == ((entry->val[0] >> 11u) & 0x01u)) || /* sablk= 1        */
               (0u != ((entry->val[0] >> 16u) & 0x3FFu)))  /* age != 0        */
            {
                *type = DVA_EM_LUT_ASIC_UNICAST;
            }
            else
            {
                *type = DVA_EM_LUT_ASIC_INVALID_ENTRY;
            }
        }
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] tbl_type
 * @param[in] index
 * @param[in] param
 * @param[in] entry
 * @retval
 */
DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_entry2param(uint32 unit, const uint32 hashAlg, const dva_table_list_t tbl_type, const uint16 index, dva_LutAsicParam_t *const param, const dva_LutTblAsicEntry_t *const entry)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((tbl_type != DVA_L2_TABLE) && (tbl_type != DVA_LUT_BCAM))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if((NULL == param) || (NULL == entry))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param)));
    CHK_FUN_RET(retVal, dva_drv_lut_param_type_decision(tbl_type, entry, &param->type));

    if(RT_ERR_OK == retVal)
    {
        switch(param->type)
        {
            case DVA_EM_LUT_ASIC_IPMULTI:
                CHK_FUN_RET(retVal, dva_drv_lut_ip_entry2param(unit, hashAlg, tbl_type, index, &param->ip, entry));
                break;
            case DVA_EM_LUT_ASIC_MULTI:
                CHK_FUN_RET(retVal, dva_drv_lut_multicast_entry2param(unit, hashAlg, tbl_type, index, &param->multi, entry));
                break;
            case DVA_EM_LUT_ASIC_UNICAST:
                CHK_FUN_RET(retVal, dva_drv_lut_unicast_entry2param(unit, hashAlg, tbl_type, index, &param->uni, entry));
                break;
            default: /* DVA_EM_LUT_ASIC_INVALID_ENTRY, and others */
                /* do nothing */
                break;
        }

        if(RT_ERR_OK == retVal)
        {
            param->index = index;
        }
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_uni_index_get(uint32 unit, const uint32 hashAlg, const dva_LutAsicUniParam_t *const param, const uint8 idx, uint16 *const pIndex)
{
    RtkApiRet retVal = RT_ERR_OK;
    if((NULL == param) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(0u == hashAlg)
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash0_unicast_index_get(unit, param, idx, pIndex));
    }
    else
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash1_unicast_index_get(unit, param, idx, pIndex));
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_uni_entry_get_for_add(uint32 unit, const uint8 idx, const uint32 hashAlg, const dva_LutAsicUniParam_t *const param, dva_LutAsicParam_t *const pRetparam, uint16 *pIndex)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == param) || (NULL == pRetparam) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_lut_uni_index_get(unit, hashAlg, param, idx, pIndex));
    CHK_FUN_RET(retVal, dva_drv_memset(pRetparam, 0, sizeof(*pRetparam)));
    CHK_FUN_RET(retVal, dva_drv_lut_entry_get(unit, hashAlg, *pIndex, pRetparam));

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_uni_param_cmp(const dva_LutAsicUniParam_t *const param, const dva_LutAsicParam_t *const pRetparam, uint8 *pFlag)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((rtl_memcmp(pRetparam->uni.mac, param->mac, sizeof(param->mac)) == 0) && (pRetparam->uni.vid == param->vid) && (pRetparam->uni.ulfid == param->ulfid))
    {
        if((0u == param->bstatic) && (1u == pRetparam->uni.bstatic))
        {
            /* A static entry can not be replaced by a non-static entry */
            retVal = -RT_ERR_FAILED;
        }
        else
        {
            *pFlag = 1u;
        }
    }

    return retVal;
}

RtkApiRet dva_drv_lut_uni_find_index(uint32 unit, const uint32 hashAlg, const dva_LutAsicUniParam_t *const param, uint16 *pIndex)
{
    RtkApiRet          retVal = RT_ERR_OK;
    uint16             index  = 0u;
    uint8              flag   = 0u;
    uint8              idx    = 0u;
    dva_LutAsicParam_t retparam;

    if((NULL == param) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == retVal)
    {
        for(idx = DVA_LUT_8WAY_FIRST; idx < DVA_LUT_8WAY_END; idx++)
        {
            CHK_FUN_RET(retVal, dva_drv_lut_uni_entry_get_for_add(unit, idx, hashAlg, param, &retparam, &index));
            if(RT_ERR_OK == retVal)
            {
                if(retparam.type == DVA_EM_LUT_ASIC_INVALID_ENTRY)
                {
                    flag = 1u; /* found an empty entry */
                }
                else if(retparam.type == DVA_EM_LUT_ASIC_UNICAST)
                {
                    retVal = dva_drv_lut_uni_param_cmp(param, &retparam, &flag);
                }
                else
                {
                    /* do nothing */
                }

                if(1u == flag)
                {
                    *pIndex = index;
                    break;
                }
            }
        }
    }

    if((RT_ERR_OK == retVal) && (0u == flag))
    {
        retVal = -RT_ERR_L2_NO_EMPTY_ENTRY;
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_uni_entry_add(uint32 unit, const dva_LutAsicUniParam_t *const param)
{
    RtkApiRet             retVal  = RT_ERR_OK;
    uint32                hashAlg = 0u;
    uint16                index   = 0u;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(1u == (param->mac[0] & 0x01u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_drv_lut_hash_algo_get(unit, &hashAlg));
    CHK_FUN_RET(retVal, dva_drv_lut_uni_find_index(unit, hashAlg, param, &index));
    CHK_FUN_RET(retVal, dva_drv_lut_unicast_param2entry(unit, DVA_L2_TABLE, &entry, param));
    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_L2_TABLE, index, entry.val));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] param
 * @param[in] idx
 * @retval
 */
RtkApiRet dva_drv_lut_uni_entry_set(uint32 unit, const uint32 hashAlg, const dva_LutAsicUniParam_t *const param, const uint8 idx)
{
    RtkApiRet             retVal = RT_ERR_OK;
    uint16                index  = 0u;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(0u == hashAlg)
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash0_unicast_index_get(unit, param, idx, &index));
    }
    else
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash1_unicast_index_get(unit, param, idx, &index));
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_drv_lut_unicast_param2entry(unit, DVA_L2_TABLE, &entry, param));
    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_L2_TABLE, (uint32)index, entry.val));

    return retVal;
}

/*This function is used to test fpga */
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_uni_entry_force_set(uint32 unit, const uint16 index, const dva_LutAsicUniParam_t *const param)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_drv_lut_unicast_param2entry(unit, DVA_L2_TABLE, &entry, param));
    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_L2_TABLE, index, entry.val));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] ageunit
 * @retval
 */
RtkApiRet dva_drv_lut_ageunit_get(uint32 unit, uint32 *const ageunit)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_SOURCE_MAC_LEARNING_CONTROL0, DVA_SOURCE_MAC_LEARNING_CONTROL0_AGEUNIT_LSP, DVA_SOURCE_MAC_LEARNING_CONTROL0_AGEUNIT_LEN, ageunit));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] ageunit
 * @retval
 */
RtkApiRet dva_drv_lut_ageunit_set(uint32 unit, const uint32 ageunit)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_reg_field_write(unit, DVA_SOURCE_MAC_LEARNING_CONTROL0, DVA_SOURCE_MAC_LEARNING_CONTROL0_AGEUNIT_LSP, DVA_SOURCE_MAC_LEARNING_CONTROL0_AGEUNIT_LEN, ageunit));

    return retVal;
}

/*This function is used to set age*/
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] macAddr
 * @param[in] agecnt
 * @retval
 */
RtkApiRet dva_drv_lut_entry_update_age(uint32 unit, const uint8 *const macAddr, const uint32 agecnt)
{
    RtkApiRet             retVal  = RT_ERR_OK;
    uint32                found   = 0u;
    uint16                hitaddr = 0u;
    dva_LutSearchParam_t  searchParam;
    dva_LutTblAsicEntry_t entry;

    if(NULL == macAddr)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(1u == (macAddr[0] & 0x01u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&searchParam, 0, sizeof(searchParam)));
    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));

    if(RT_ERR_OK == retVal)
    {
        searchParam.defMac   = 1;
        retVal               = dva_drv_memcpy(&searchParam.mac, macAddr, sizeof(searchParam.mac));
        searchParam.startIdx = 0;
    }

    CHK_FUN_RET(retVal, dva_drv_lut_search(unit, &searchParam, &found, &hitaddr));
    if(RT_ERR_OK == retVal)
    {
        if(1u == found)
        {
            CHK_FUN_RET(retVal, dva_table_read(unit, DVA_L2_TABLE, hitaddr, entry.val));
            entry.val[0] = (entry.val[0] & 0xFFC0FFFFu) | ((agecnt & 0x3FFu) << 16u);
            CHK_FUN_RET(retVal, dva_table_write(unit, DVA_L2_TABLE, hitaddr, entry.val));
        }
        else
        {
            retVal = -RT_ERR_L2_ENTRY_NOTFOUND;
        }
    }

    return retVal;
}

RtkApiRet dva_drv_lut_entry_delete_input_check(const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid)
{
    RtkApiRet retVal = RT_ERR_OK;
    if(NULL == mac)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((vid > DVA_LUT_VID_MAX) || (ulfid >= (uint8)EM_VLAN_IVLSVL_END))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    /* L2 Lookup by fid */
    if((uint8)DVA_HASH_WITH_FID == ulfid)
    {
        if(fid > DVA_LUT_FID_MAX)
        {
            retVal = -RT_ERR_L2_FID;
        }
    }
    /* L2 Lookup by vid, fid can be set to 15 */
    else
    {
        if(fid > DVA_LUT_INVALID_FID)
        {
            retVal = -RT_ERR_L2_FID;
        }
    }

    return retVal;
}

RtkApiRet dva_drv_lut_entry_delete_param_init(const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid, dva_LutSearchParam_t *const pParam, dva_LutAsicMultiParam_t *const pMultiParam, dva_LutAsicParam_t *const pLutParam)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == pParam) || (NULL == pMultiParam) || (NULL == pLutParam))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, dva_drv_lut_entry_delete_input_check(mac, vid, fid, ulfid));
    CHK_FUN_RET(retVal, dva_drv_memset(pParam, 0, sizeof(*pParam)));
    CHK_FUN_RET(retVal, dva_drv_memset(pMultiParam, 0, sizeof(*pMultiParam)));
    CHK_FUN_RET(retVal, dva_drv_memset(pLutParam, 0, sizeof(*pLutParam)));

    if(RT_ERR_OK == retVal)
    {
        if(0u == (mac[0] & 0x01u))
        {
            pParam->defMac = 1u;
            CHK_FUN_RET(retVal, dva_drv_memcpy(pParam->mac, mac, sizeof(pParam->mac)));
            pParam->ulfid  = ulfid;
            pParam->defFid = 1u;
            if((uint8)DVA_HASH_WITH_CVID == ulfid)
            {
                pParam->fid = (uint32)DVA_LUT_INVALID_FID;
            }
            else
            {
                pParam->fid = (uint32)fid;
            }
            pParam->vid      = vid;
            pParam->startIdx = 0u;
        }
        else
        {
            CHK_FUN_RET(retVal, dva_drv_memcpy(pMultiParam->mac, mac, sizeof(pMultiParam->mac)));
            pMultiParam->fid = (uint8)fid;
            pMultiParam->vid = vid;
        }
    }

    return retVal;
}

uint32 dva_drv_lut_multi_entry_check(const uint8 *const mac, const uint16 vid, const uint16 fid, const dva_LutAsicParam_t *const pLutParam)
{
    int32  cmpMac = 0;
    uint32 found  = 0u;

    if(DVA_EM_LUT_ASIC_MULTI == pLutParam->type)
    {
        cmpMac = rtl_memcmp(mac, pLutParam->multi.mac, sizeof(pLutParam->multi.mac));

        if((0 == cmpMac) && (vid == pLutParam->multi.vid) && (fid == pLutParam->multi.fid))
        {
            found = 1u;
        }
    }

    return found;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @retval
 */
RtkApiRet dva_drv_lut_entry_delete(uint32 unit, const uint16 index)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_LutTblAsicEntry_t entry;

    retVal = dva_drv_memset(&entry, 0, sizeof(entry));

    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_L2_TABLE, (uint32)index, entry.val));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] index
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_uni_entry_get(uint32 unit, const uint32 hashAlg, const uint16 index, dva_LutAsicUniParam_t *const param)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_table_read(unit, DVA_L2_TABLE, index, entry.val));
    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param)));
    CHK_FUN_RET(retVal, dva_drv_lut_unicast_entry2param(unit, hashAlg, DVA_L2_TABLE, index, param, &entry));

    return retVal;
}

RtkApiRet dva_drv_lut_multi_entry_input_check(const uint8 *const pMacAddr, const dva_VlanFid fid, const LutParam_t *const pLutEntry)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == pMacAddr) || (NULL == pLutEntry))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(0x01u != (pMacAddr[0] & 0x01u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(fid > DVA_LUT_FID_MAX)
    {
        retVal = -RT_ERR_L2_FID;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

RtkApiRet dva_drv_lut_multi_entry_param_init(const uint8 *const pMacAddr, const dva_VlanFid fid, dva_LutAsicMultiParam_t *const pMultiParam)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == pMacAddr) || (NULL == pMultiParam))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(0x01u != (pMacAddr[0] & 0x01u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(fid > DVA_LUT_FID_MAX)
    {
        retVal = -RT_ERR_L2_FID;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(pMultiParam, 0, sizeof(*pMultiParam)));
    CHK_FUN_RET(retVal, dva_drv_memcpy(pMultiParam->mac, pMacAddr, 6));

    if(RT_ERR_OK == retVal)
    {
        pMultiParam->vid = (uint16)(fid & 0xffffu);
        pMultiParam->fid = (uint8)(fid & 0xffu);
    }

    return retVal;
}

RtkApiRet dva_drv_lut_multicast_index_get(uint32 unit, const uint32 hashAlg, const dva_LutAsicMultiParam_t *const pMultiParam, const uint8 idx, uint16 *const pIndex)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == pMultiParam) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(0u == hashAlg)
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash0_multicast_index_get(unit, pMultiParam, idx, pIndex));
    }
    else
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash1_multicast_index_get(unit, pMultiParam, idx, pIndex));
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_multi_entry_get(uint32 unit, const uint8 idx, const uint32 hashAlg, const dva_LutAsicMultiParam_t *const param, dva_LutAsicParam_t *const pRetparam, uint16 *pIndex)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == param) || (NULL == pRetparam) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_lut_multicast_index_get(unit, hashAlg, param, idx, pIndex));
    CHK_FUN_RET(retVal, dva_drv_memset(pRetparam, 0, sizeof(*pRetparam)));
    CHK_FUN_RET(retVal, dva_drv_lut_entry_get(unit, hashAlg, *pIndex, pRetparam));

    return retVal;
}

DVA_ASIC_INTERNAL uint8 dva_drv_lut_multi_param_cmp(const uint32 ulfid, const dva_LutAsicMultiParam_t *const param, const dva_LutAsicParam_t *const pRetparam)
{
    uint8 retVal = RT_ERR_OK;

    if(ulfid == (uint32)DVA_HASH_WITH_CVID)
    {
        if((rtl_memcmp(pRetparam->multi.mac, param->mac, sizeof(param->mac)) == 0) && (pRetparam->multi.vid == param->vid))
        {
            retVal = 1u;
        }
    }
    else if(ulfid == (uint32)DVA_HASH_WITH_FID)
    {
        if((rtl_memcmp(pRetparam->multi.mac, param->mac, sizeof(param->mac)) == 0) && (pRetparam->multi.fid == param->fid))
        {
            retVal = 1u;
        }
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

RtkApiRet dva_drv_lut_multi_find_index(uint32 unit, const uint32 ulfid, const uint32 hashAlg, const dva_LutAsicMultiParam_t *const param, uint16 *pIndex)
{
    RtkApiRet          retVal = RT_ERR_OK;
    uint16             index  = 0u;
    uint8              flag   = 0u;
    uint8              idx    = 0u;
    dva_LutAsicParam_t retparam;

    if((NULL == param) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == retVal)
    {
        for(idx = DVA_LUT_8WAY_FIRST; idx < DVA_LUT_8WAY_END; idx++)
        {
            CHK_FUN_RET(retVal, dva_drv_lut_multi_entry_get(unit, idx, hashAlg, param, &retparam, &index));
            if(RT_ERR_OK == retVal)
            {
                if(retparam.type == DVA_EM_LUT_ASIC_INVALID_ENTRY)
                {
                    flag = 1u; /* found an empty entry */
                }
                else if(retparam.type == DVA_EM_LUT_ASIC_MULTI)
                {
                    flag = dva_drv_lut_multi_param_cmp(ulfid, param, &retparam);
                }
                else
                {
                    /* do nothing */
                }

                if(1u == flag)
                {
                    *pIndex = index;
                    break;
                }
            }
        }
    }

    if((RT_ERR_OK == retVal) && (0u == flag))
    {
        retVal = -RT_ERR_L2_NO_EMPTY_ENTRY;
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_multi_entry_add(uint32 unit, const dva_LutAsicMultiParam_t *const param)
{
    RtkApiRet             retVal  = RT_ERR_OK;
    uint32                hashAlg = 0u;
    uint16                index   = 0u;
    uint32                ulfid   = 0u;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(1u != (param->mac[0] & 0x01u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MLFID_LSP, DVA_VLAN_CONTROL0_MLFID_LEN, &ulfid));
    CHK_FUN_RET(retVal, dva_drv_lut_hash_algo_get(unit, &hashAlg));
    CHK_FUN_RET(retVal, dva_drv_lut_multi_find_index(unit, ulfid, hashAlg, param, &index));
    CHK_FUN_RET(retVal, dva_drv_lut_multicast_param2entry(unit, DVA_L2_TABLE, &entry, param));
    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_L2_TABLE, index, entry.val));

    return retVal;
}

RtkApiRet dva_drv_lut_ip_multi_index_get(uint32 unit, const uint32 hashAlg, const dva_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const pIndex)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == param) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(0u == hashAlg)
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash0_ip_index_get(unit, param, idx, pIndex));
    }
    else
    {
        CHK_FUN_RET(retVal, dva_drv_lut_hash1_ip_index_get(unit, param, idx, pIndex));
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_ip_multi_entry_get(uint32 unit, const uint8 idx, const uint32 hashAlg, const dva_LutAsicIpMultiParam_t *const param, dva_LutAsicParam_t *const pRetparam, uint16 *pIndex)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == param) || (NULL == pRetparam) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_lut_ip_multi_index_get(unit, hashAlg, param, idx, pIndex));
    CHK_FUN_RET(retVal, dva_drv_memset(pRetparam, 0, sizeof(*pRetparam)));
    CHK_FUN_RET(retVal, dva_drv_lut_entry_get(unit, hashAlg, *pIndex, pRetparam));

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_ip_multi_find_index(uint32 unit, const uint32 hashAlg, const dva_LutAsicIpMultiParam_t *const param, uint16 *pIndex)
{
    RtkApiRet          retVal = RT_ERR_OK;
    uint16             index  = 0u;
    uint8              flag   = 0u;
    uint8              idx    = 0u;
    dva_LutAsicParam_t retparam;

    if((NULL == param) || (NULL == pIndex))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == retVal)
    {
        for(idx = DVA_LUT_8WAY_FIRST; idx < DVA_LUT_8WAY_END; idx++)
        {
            CHK_FUN_RET(retVal, dva_drv_lut_ip_multi_entry_get(unit, idx, hashAlg, param, &retparam, &index));

            if(RT_ERR_OK == retVal)
            {
                if(retparam.type == DVA_EM_LUT_ASIC_INVALID_ENTRY)
                {
                    flag = 1u; /* found an empty entry */
                }
                else if(retparam.type == DVA_EM_LUT_ASIC_IPMULTI)
                {
                    if((rtl_memcmp(retparam.ip.sip, param->sip, sizeof(param->sip)) == 0) && (rtl_memcmp(retparam.ip.dip, param->dip, sizeof(param->dip)) == 0))
                    {
                        flag = 1u; /* hit the entry */
                    }
                }
                else
                {
                    /* do nothing */
                }

                if(1u == flag)
                {
                    *pIndex = index;
                    break;
                }
            }
        }
    }

    if((RT_ERR_OK == retVal) && (0u == flag))
    {
        retVal = -RT_ERR_L2_NO_EMPTY_ENTRY;
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_ip_multi_entry_add(uint32 unit, const dva_LutAsicIpMultiParam_t *const param)
{
    RtkApiRet             retVal  = RT_ERR_OK;
    uint32                hashAlg = 0u;
    uint16                index   = 0u;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(0xE0u != (param->dip[0] & 0xF0u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_LOOKUP_CONTROL0, DVA_ADDRESS_TABLE_LOOKUP_CONTROL0_IPMC_LSP, DVA_ADDRESS_TABLE_LOOKUP_CONTROL0_IPMC_LEN, 1));
    CHK_FUN_RET(retVal, dva_drv_lut_hash_algo_get(unit, &hashAlg));
    CHK_FUN_RET(retVal, dva_drv_lut_ip_multi_find_index(unit, hashAlg, param, &index));
    CHK_FUN_RET(retVal, dva_drv_lut_ip_param2entry(unit, DVA_L2_TABLE, &entry, param));
    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_L2_TABLE, (uint32)index, entry.val));

    return retVal;
}

RtkApiRet dva_drv_lut_ip_multi_entry_input_check(const uint8 *const sip, const uint8 *const dip)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == sip) || (NULL == dip))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(0xE0u != (dip[0] & 0xF0u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

RtkApiRet dva_drv_lut_ip_multi_entry_param_init(const uint8 *const sip, const uint8 *const dip, dva_LutAsicIpMultiParam_t *const pIpMultiParam, dva_LutAsicParam_t *const pLutParam)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == pIpMultiParam) || (NULL == pLutParam))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, dva_drv_lut_ip_multi_entry_input_check(sip, dip));

    CHK_FUN_RET(retVal, dva_drv_memset(pIpMultiParam, 0, sizeof(*pIpMultiParam)));
    CHK_FUN_RET(retVal, dva_drv_memset(pLutParam, 0, sizeof(*pLutParam)));
    CHK_FUN_RET(retVal, dva_drv_memcpy(&(pIpMultiParam->sip), sip, sizeof(pIpMultiParam->sip)));
    CHK_FUN_RET(retVal, dva_drv_memcpy(&(pIpMultiParam->dip), dip, sizeof(pIpMultiParam->dip)));

    return retVal;
}

uint32 dva_drv_lut_ip_multi_param_cmp(const dva_LutAsicIpMultiParam_t *const pIpMultiParam, const dva_LutAsicParam_t *const pRetparam)
{
    uint32 found = 0;

    if(pRetparam->type == DVA_EM_LUT_ASIC_IPMULTI)
    {
        if((0 == rtl_memcmp(&pRetparam->ip.sip, &pIpMultiParam->sip, sizeof(pIpMultiParam->sip))) && (0 == rtl_memcmp(&pRetparam->ip.dip, &pIpMultiParam->dip, sizeof(pIpMultiParam->dip))))
        {
            found = 1;
        }
    }

    return found;
}

RtkApiRet dva_drv_lut_lookup_miss_ctrl_input_check(const RtkPort port, const uint8 type, const uint8 action)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port != (RtkPort)DVA_RTK_CPU_PORT) && ((port > (RtkPort)DVA_RTK_USER_PORT_END) || (port < (RtkPort)DVA_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    if(type >= (uint8)DVA_LM_TYPE_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(action >= (uint8)DVA_LM_ACTION_END)
    {
        retVal = -RT_ERR_NOT_ALLOWED;
    }
    else
    {
        /* do nothing */
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] idx
 * @retval
 */
RtkApiRet dva_drv_lut_group_entry_set(uint32 unit, const dva_LutAsicGroupParam_t *const param, const uint8 idx)
{
    RtkApiRet              retVal = RT_ERR_OK;
    uint16                 index  = 0u;
    dva_LutTblGroupEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_drv_lut_hash_group_index_get(unit, param, idx, &index));

    if(RT_ERR_OK == retVal)
    {
        entry.val[0] = param->portmask & (DVA_RTK_SYS_PMAP_ALL | DVA_RTK_CPU_PMAP);
        entry.val[1] = (uint32)param->dip[3] | ((uint32)param->dip[2] << 8u) | ((uint32)param->dip[1] << 16u) | (((uint32)param->dip[0] & 0x0Fu) << 24u);
    }

    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_GROUP, (uint32)index, entry.val));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_group_entry_get(uint32 unit, const uint16 index, dva_LutAsicGroupParam_t *const param)
{
    RtkApiRet              retVal = RT_ERR_OK;
    dva_LutTblGroupEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_table_read(unit, DVA_GROUP, (uint32)index, entry.val));
    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param)));

    if(RT_ERR_OK == retVal)
    {
        param->portmask = entry.val[0] & (DVA_RTK_SYS_PMAP_ALL | DVA_RTK_CPU_PMAP);
        param->dip[3]   = (uint8)(entry.val[1] & 0xFFu);
        param->dip[2]   = (uint8)((entry.val[1] >> 8u) & 0xFFu);
        param->dip[1]   = (uint8)((entry.val[1] >> 16u) & 0xFFu);
        param->dip[0]   = (uint8)(((entry.val[1] >> 24u) & 0x0Fu) | 0xE0u);
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_cam_entry_set(uint32 unit, const uint16 index, const dva_LutAsicParam_t *const param)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));

    if(RT_ERR_OK == retVal)
    {
        switch(param->type)
        {
            case DVA_EM_LUT_ASIC_UNICAST:
                CHK_FUN_RET(retVal, dva_drv_lut_unicast_param2entry(unit, DVA_LUT_BCAM, &entry, &(param->uni)));
                break;
            case DVA_EM_LUT_ASIC_MULTI:
                CHK_FUN_RET(retVal, dva_drv_lut_multicast_param2entry(unit, DVA_LUT_BCAM, &entry, &(param->multi)));
                break;
            case DVA_EM_LUT_ASIC_IPMULTI:
                CHK_FUN_RET(retVal, dva_drv_lut_ip_param2entry(unit, DVA_LUT_BCAM, &entry, &(param->ip)));
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }
    CHK_FUN_RET(retVal, dva_table_write(unit, DVA_LUT_BCAM, index, entry.val));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] index
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_cam_entry_get(uint32 unit, const uint16 index, dva_LutAsicParam_t *const param)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_table_read(unit, DVA_LUT_BCAM, index, entry.val));
    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param)));
    CHK_FUN_RET(retVal, dva_drv_lut_entry2param(unit, 0, DVA_LUT_BCAM, index, param, &entry));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] mode
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_flush(uint32 unit, const dva_LutFlushMode_e mode, const dva_LutFlushParam_t *const param)
{
    RtkApiRet          retVal     = RT_ERR_OK;
    RegPollingConfig_t pollingCfg = {0};

    /* Set pollingCfg structure members */
    pollingCfg.triggerRegAddr = DVA_ADDRESS_TABLE_FLUSH_CONTROL0;
    pollingCfg.triggerRegLsp  = DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_TRIGGER_LSP;
    pollingCfg.triggerRegLen  = DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_TRIGGER_LEN;
    pollingCfg.action         = 1u;
    pollingCfg.pollingRegAddr = DVA_ADDRESS_TABLE_FLUSH_CONTROL0;
    pollingCfg.pollingRegLsp  = DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_TRIGGER_LSP;
    pollingCfg.pollingRegLen  = DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_TRIGGER_LEN;
    pollingCfg.expVal         = 0u;
    pollingCfg.eventID        = DVA_EM_EVENT_L2_POLLING_TIMEOUT;

    if(mode >= DVA_EM_LUT_FLUSH_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_FLUSH_CONTROL0, DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_MODE_LSP, DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_MODE_LEN, (uint32)mode));

    if(RT_ERR_OK == retVal)
    {
        switch(mode)
        {
            case DVA_EM_LUT_FLUSH_ALL:
                break;
            case DVA_EM_LUT_FLUSH_PORT_ONLY:
                CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_FLUSH_CONTROL1, DVA_ADDRESS_TABLE_FLUSH_CONTROL1_LUT_FLUSH_PMSK_LSP, DVA_ADDRESS_TABLE_FLUSH_CONTROL1_LUT_FLUSH_PMSK_LEN, param->portmask));
                break;
            case DVA_EM_LUT_FLUSH_PORT_CVID:
                CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_FLUSH_CONTROL1, DVA_ADDRESS_TABLE_FLUSH_CONTROL1_LUT_FLUSH_PMSK_LSP, DVA_ADDRESS_TABLE_FLUSH_CONTROL1_LUT_FLUSH_PMSK_LEN, param->portmask));
                CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_FLUSH_CONTROL0, DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_VID_LSP, DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_VID_LEN, param->vid));
                break;
            case DVA_EM_LUT_FLUSH_PORT_FID:
                CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_FLUSH_CONTROL1, DVA_ADDRESS_TABLE_FLUSH_CONTROL1_LUT_FLUSH_PMSK_LSP, DVA_ADDRESS_TABLE_FLUSH_CONTROL1_LUT_FLUSH_PMSK_LEN, param->portmask));
                CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_FLUSH_CONTROL0, DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_FID_LSP, DVA_ADDRESS_TABLE_FLUSH_CONTROL0_LUT_FLUSH_FID_LEN, param->fid));
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    /* Trigger and polling for L2 flush action */
    CHK_FUN_RET(retVal, dva_drv_timer_polling_reg_timeout(unit, &pollingCfg, DVA_LUT_FLUSH_TIME_MAX_US, DVA_LUT_FLUSH_TIME_MIN_US));

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_search_use_mac(uint32 unit, const uint8 *mac)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    mac31To0  = 0;
    uint32    mac47To32 = 0;

    if(NULL == mac)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        mac47To32 = ((uint32)mac[0] << 8u) | (uint32)mac[1];
        mac31To0  = ((uint32)mac[2] << 24u) | ((uint32)mac[3] << 16u) | ((uint32)mac[4] << 8u) | (uint32)mac[5];
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL0, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_MAC_47_32_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_MAC_47_32_LEN, mac47To32));
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL1, DVA_ADDRESS_TABLE_SEARCH_CONTROL1_LUT_SEARCH_MAC_31_0_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL1_LUT_SEARCH_MAC_31_0_LEN, mac31To0));
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL0, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_SPECIFY_MAC_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_SPECIFY_MAC_LEN, 1));
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_search_use_portNum(uint32 unit, uint32 portNum)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL0, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_PORT_NUM_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_PORT_NUM_LEN, portNum));
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL0, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_SPECIFY_PORT_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL0_LUT_SEARCH_SPECIFY_PORT_LEN, 1));

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_search_use_fid_vid(uint32 unit, uint32 fid, uint32 vid, uint8 ulfid)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_SPECIFY_FID_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_SPECIFY_FID_LEN, 1));
    if(0u == ulfid) /* 0: hash with vid, 1: hash with fid */
    {
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_HASH_ALG_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_HASH_ALG_LEN, 0));
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_VID_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_VID_LEN, vid));
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_FID_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_FID_LEN, 0));
    }
    else
    {
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_HASH_ALG_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_HASH_ALG_LEN, 1));
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_VID_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_VID_LEN, 0));
        CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_FID_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_FID_LEN, fid));
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_search_decision(uint32 unit, const dva_LutSearchParam_t *const param)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(1u == param->defMac)
    {
        CHK_FUN_RET(retVal, dva_drv_lut_search_use_mac(unit, param->mac));
    }
    if(1u == param->defPortNum)
    {
        CHK_FUN_RET(retVal, dva_drv_lut_search_use_portNum(unit, param->portNum));
    }
    if(1u == param->defFid)
    {
        CHK_FUN_RET(retVal, dva_drv_lut_search_use_fid_vid(unit, param->fid, param->vid, param->ulfid));
    }

    return retVal;
}

DVA_ASIC_INTERNAL RtkApiRet dva_drv_lut_clear_search_ctrl_reg(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL0, 0u, 32u, 0u));
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL1, 0u, 32u, 0u));
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, 0u, 32u, 0u));

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit
 * @param[in] param
 * @param[in] found
 * @param[in] hitaddr
 * @retval
 */
RtkApiRet dva_drv_lut_search(uint32 unit, const dva_LutSearchParam_t *const param, uint32 *const found, uint16 *const hitaddr)
{
    RtkApiRet          retVal     = RT_ERR_OK;
    uint32             regVal     = 0u;
    RegPollingConfig_t pollingCfg = {0};

    /* Set pollingCfg structure members */
    pollingCfg.triggerRegAddr = DVA_ADDRESS_TABLE_SEARCH_CONTROL3;
    pollingCfg.triggerRegLsp  = DVA_ADDRESS_TABLE_SEARCH_CONTROL3_LUT_SEARCH_TRIGGER_LSP;
    pollingCfg.triggerRegLen  = DVA_ADDRESS_TABLE_SEARCH_CONTROL3_LUT_SEARCH_TRIGGER_LEN;
    pollingCfg.action         = 1u;
    pollingCfg.pollingRegAddr = DVA_ADDRESS_TABLE_SEARCH_CONTROL3;
    pollingCfg.pollingRegLsp  = DVA_ADDRESS_TABLE_SEARCH_CONTROL3_LUT_SEARCH_TRIGGER_LSP;
    pollingCfg.pollingRegLen  = DVA_ADDRESS_TABLE_SEARCH_CONTROL3_LUT_SEARCH_TRIGGER_LEN;
    pollingCfg.expVal         = 0u;
    pollingCfg.eventID        = DVA_EM_EVENT_L2_POLLING_TIMEOUT;

    if((NULL == param) || (NULL == found) || (NULL == hitaddr))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_lut_clear_search_ctrl_reg(unit));
    CHK_FUN_RET(retVal, dva_drv_lut_search_decision(unit, param));
    /* start */
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_SEARCH_CONTROL2, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_IDX_START_LSP, DVA_ADDRESS_TABLE_SEARCH_CONTROL2_LUT_SEARCH_IDX_START_LEN, param->startIdx));

    /* Trigger and polling for L2 search action */
    CHK_FUN_RET(retVal, dva_drv_timer_polling_reg_timeout(unit, &pollingCfg, DVA_LUT_SEARCH_TIME_MAX_US, DVA_LUT_SEARCH_TIME_MIN_US));

    if(RT_ERR_OK == retVal)
    {
        CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_ADDRESS_TABLE_SEARCH_RESULT, DVA_ADDRESS_TABLE_SEARCH_RESULT_LUT_SEARCH_FOUND_LSP, DVA_ADDRESS_TABLE_SEARCH_RESULT_LUT_SEARCH_FOUND_LEN, found));
        if(1u == *found)
        {
            CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_ADDRESS_TABLE_SEARCH_RESULT, DVA_ADDRESS_TABLE_SEARCH_RESULT_LUT_SEARCH_HIT_ADDR_LSP, DVA_ADDRESS_TABLE_SEARCH_RESULT_LUT_SEARCH_HIT_ADDR_LEN, &regVal));
            *hitaddr = (uint16)(regVal & 0xffffu);
        }
        else
        {
            *hitaddr = 0u;
        }
    }

    return retVal;
}

/*This function is used to replace function asicdrv_lut_getAsicEntry*/
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] index
 * @param[in] param
 * @retval
 */
RtkApiRet dva_drv_lut_entry_get(uint32 unit, const uint32 hashAlg, const uint16 index, dva_LutAsicParam_t *const param)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_LutTblAsicEntry_t entry;

    if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_table_read(unit, DVA_L2_TABLE, index, entry.val));
    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param)));
    CHK_FUN_RET(retVal, dva_drv_lut_entry2param(unit, hashAlg, DVA_L2_TABLE, index, param, &entry));

    return retVal;
}

/*This function is used to replace function asicdrv_lut_getAsicEntry*/
/**
 * @brief
 *
 * @param[in] unit
 * @param[in] hashAlg
 * @param[in] index
 * @param[in] param
 * @param[in] entryNum
 * @retval
 */
RtkApiRet dva_drv_lut_entry_burst_get(uint32 unit, const uint32 hashAlg, const uint16 index, dva_LutAsicParam_t *const param, uint32 entryNum)
{
    RtkApiRet             retVal = RT_ERR_OK;
    uint16                i      = 0u;
    uint16                j      = 0u;
    uint32                entry[sizeof(dva_LutTblAsicEntry_t) * DVA_TBL_MAX_BURST_ENTRYNUM]; /*Read from INDIRECT_DATA0_FOR_CPU~INDIRECT_DATA12_FOR_CPU (+INDIRECT_DATA13_FOR_CPU~INDIRECT_DATA28_FOR_CPU)*/
    dva_LutTblAsicEntry_t lutEntry[DVA_TBL_MAX_BURST_ENTRYNUM];

    if((uint32)REGIF_I2C != g_RegIF)
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == param)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((entryNum > (uint16)DVA_TBL_MAX_BURST_ENTRYNUM) || (0u == entryNum))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, dva_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, dva_drv_memset(&lutEntry, 0, sizeof(lutEntry)));
    CHK_FUN_RET(retVal, dva_table_burst_read(unit, DVA_L2_TABLE, index, entry, entryNum));
    CHK_FUN_RET(retVal, dva_drv_memset(param, 0, sizeof(*param) * entryNum));

    if(RT_ERR_OK == retVal)
    {
        for(i = 0u; i < (uint16)entryNum; i++)
        {
            for(j = 0u; j < 3u; j++)
            {
                lutEntry[i].val[j] = entry[(3u * i) + j];
            }
            CHK_FUN_RET(retVal, dva_drv_lut_entry2param(unit, hashAlg, DVA_L2_TABLE, (uint16)((index + i) & 0xffffu), &(param[i]), &(lutEntry[i])));
            if(RT_ERR_OK != retVal)
            {
                break;
            }
        }
    }

    return retVal;
}

/*
@func uint32 | dva_drv_lut_hash_algo_get | get address table hash algorithm
@parm  void |
@rvalue uint32 |hash algorithm  |
@comm
*/
/**
 * @brief Get address table hash algorithm
 * @param[in] unit
 * @param[out] hashAlg
 * @retval
 */
RtkApiRet dva_drv_lut_hash_algo_get(uint32 unit, uint32 *const hashAlg)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == hashAlg)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_ADDRESS_TABLE_LOOKUP_CONTROL1, DVA_ADDRESS_TABLE_LOOKUP_CONTROL1_HASHALG_LSP, DVA_ADDRESS_TABLE_LOOKUP_CONTROL1_HASHALG_LEN, hashAlg));

    return retVal;
}

/**
 * @brief Set address table hash algorithm
 * @param[in] unit
 * @param[in] hashAlg algorithm, 0 or 1
 * @retval FAILED
 * @retval SUCCESS
 */
RtkApiRet dva_drv_lut_hash_algo_set(uint32 unit, const uint32 hashAlg)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_ADDRESS_TABLE_LOOKUP_CONTROL1, DVA_ADDRESS_TABLE_LOOKUP_CONTROL1_HASHALG_LSP, DVA_ADDRESS_TABLE_LOOKUP_CONTROL1_HASHALG_LEN, hashAlg));

    CHK_FUN_RET(retVal, dva_drv_indirect_sram_1byte_set(unit, DVA_GLOBAL_HASHALG_ADDR, (uint8)(hashAlg & 0xFFu)));

    return retVal;
}

/**
 * @brief Set address table hash algorithm
 * @param[in] unit
 * @param[in] ulfid 0(DVA_HASH_WITH_CVID) or 1((uint32)DVA_HASH_WITH_FID)
 * @retval FAILED
 * @retval SUCCESS
 */
RtkApiRet dva_drv_lut_multi_ulfid_set(uint32 unit, const uint32 ulfid)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_FIXMCFID_LSP, DVA_VLAN_CONTROL0_FIXMCFID_LEN, 0));
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MLFID_LSP, DVA_VLAN_CONTROL0_MLFID_LEN, ulfid));

    return retVal;
}

/*
@func uint32 | dva_drv_lut_multi_ulfid_get | get the ulfid of multicast entry in hash Algo
@parm  void |
@rvalue uint32 |the ulfid of multicast entry  |
@comm
*/
/**
 * @brief Get the ulfid of multicast entry in hash algorithm
 *
 * @param[in] unit
 * @param[out] ulfid the ulfid of multicast entry
 * @retval
 */
RtkApiRet dva_drv_lut_multi_ulfid_get(uint32 unit, uint32 *const ulfid)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_reg_field_read(unit, DVA_VLAN_CONTROL0, DVA_VLAN_CONTROL0_MLFID_LSP, DVA_VLAN_CONTROL0_MLFID_LEN, ulfid));

    return retVal;
}

/**
 * @brief Configure action for packets when address table bucket full
 * @param[in] unit
 * @param[in] enable 1: trap, 0: forward
 * @retval SUCCESS
 * @retval FAILED
 * @note When four ways bucket full, address will not be auto learned by ASIC
 */
RtkApiRet dva_drv_lut_bkt_full_trap(uint32 unit, const uint32 enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_reg_field_write(unit, DVA_SOURCE_MAC_LEARNING_CONTROL0, DVA_SOURCE_MAC_LEARNING_CONTROL0_BKTFULLACT_LSP, DVA_SOURCE_MAC_LEARNING_CONTROL0_BKTFULLACT_LEN, enable));

    return retVal;
}

/**
 * @brief Enable LRU when bucket full
 * @param[in] unit
 * @param[in] enable 1:enable, 0: disable
 * @retval SUCCESS
 * @retval FAILED
 * @note L2LRU enable, the least recently used entry would be overwritten by new un-learned source MAC
 */
RtkApiRet dva_drv_lut_lru_enable(uint32 unit, const uint32 enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, dva_reg_field_write(unit, DVA_SOURCE_MAC_LEARNING_CONTROL0, DVA_SOURCE_MAC_LEARNING_CONTROL0_L2LRU_LSP, DVA_SOURCE_MAC_LEARNING_CONTROL0_L2LRU_LEN, enable));

    return retVal;
}

RtkApiRet dva_drv_lut_sys_param2user_param(const dva_LutAsicParam_t *const sysParam, LutParam_t *const userParam)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;
    uint8     spa     = 0u;

    CHK_FUN_RET(retVal, dva_drv_memcpy(userParam, sysParam, sizeof(*userParam)));

    switch(sysParam->type)
    {
        case DVA_EM_LUT_ASIC_IPMULTI:

            srcPmap = (RtkPmap)sysParam->ip.mbr;
            CHK_FUN_RET(retVal, dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_SYS_TO_USER));
            if(RT_ERR_OK == retVal)
            {
                userParam->ip.mbr = (uint32)dstPmap;
            }
            break;
        case DVA_EM_LUT_ASIC_MULTI:

            srcPmap = (RtkPmap)sysParam->multi.mbr;
            CHK_FUN_RET(retVal, dva_drv_pmap_convert(srcPmap, &dstPmap, DVA_EM_SYS_TO_USER));
            if(RT_ERR_OK == retVal)
            {
                userParam->multi.mbr = (uint32)dstPmap;
            }
            break;
        case DVA_EM_LUT_ASIC_UNICAST:

            spa = (uint8)sysParam->uni.spa;
            if((spa != (RtkPort)DVA_RTK_CPU_PORT) && ((spa > (RtkPort)DVA_RTK_SYS_PORT_ID_MAX) || (spa < (RtkPort)DVA_RTK_SYS_PORT_1ST)))
            {
                retVal = -RT_ERR_PORT_ID;
            }
            else
            {
                userParam->uni.spa = (uint8)dva_g_sysPort2userPort[spa];
            }
            break;
        default:
            /* do nothing */
            break;
    }

    return retVal;
}
