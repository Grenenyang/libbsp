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
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtl907xc_vb_asicdrv_routing.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#include "rtl907xc_vb_asicdrv_lut.h"
#include "rtl907xc_vb_asicdrv_tcam.h"
#include "rtl907xc_vb_table_struct.h"

void cvb_drv_rt_max_host_entry_idx_num_get(RtHostEntryType_e entryType, uint8 *maxSlotNum)
{
    switch(entryType)
    {
        case EM_RT_UC4:
            *maxSlotNum = CVB_RT_MAX_UC4_ENTRY_NUM;
            break;
        case EM_RT_MC4:
            *maxSlotNum = CVB_RT_MAX_MC4_ENTRY_NUM;
            break;
        case EM_RT_UC6:
            *maxSlotNum = CVB_RT_MAX_UC6_ENTRY_NUM;
            break;
        case EM_RT_MC6:
            *maxSlotNum = CVB_RT_MAX_MC6_ENTRY_NUM;
            break;
        case EM_RT_UC4FIR:
            *maxSlotNum = CVB_RT_MAX_UC4FIR_ENTRY_NUM;
            break;
        case EM_RT_MC4FIR:
            *maxSlotNum = CVB_RT_MAX_MC4FIR_ENTRY_NUM;
            break;
        case EM_RT_UC6FIR:
            *maxSlotNum = CVB_RT_MAX_UC6FIR_ENTRY_NUM;
            break;
        case EM_RT_MC6FIR:
            *maxSlotNum = CVB_RT_MAX_MC6FIR_ENTRY_NUM;
            break;
        default:
            /* Do nothing */
            break;
    }
}

DYNAMIC_SCOPE void cvb_drv_rt_host_entry_size_get(RtHostEntryType_e entryType, uint16 *slotSize)
{
    switch(entryType)
    {
        case EM_RT_UC4:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_UC4;
            break;
        case EM_RT_MC4:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_MC4;
            break;
        case EM_RT_UC6:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_UC6;
            break;
        case EM_RT_MC6:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_MC6; /* byte-alignment */
            break;
        case EM_RT_UC4FIR:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_UC4FIR; /* byte-alignment */
            break;
        case EM_RT_MC4FIR:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_MC4FIR;
            break;
        case EM_RT_UC6FIR:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_UC6FIR; /* byte-alignment */
            break;
        case EM_RT_MC6FIR:
            *slotSize = CVB_RT_HOST_ENTRY_SIZE_MC6FIR; /* byte-alignment */
            break;
        default:
            /* Do nothing */
            break;
    }
}

void cvb_drv_rt_occupied_slot_num_get(RtHostEntryType_e entryType, uint8 *slotLen)
{
    switch(entryType)
    {
        case EM_RT_UC4:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_UC4;
            break;
        case EM_RT_MC4:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_MC4;
            break;
        case EM_RT_UC6:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_UC6;
            break;
        case EM_RT_MC6:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_MC6;
            break;
        case EM_RT_UC4FIR:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_UC4FIR;
            break;
        case EM_RT_MC4FIR:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_MC4FIR;
            break;
        case EM_RT_UC6FIR:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_UC6FIR;
            break;
        case EM_RT_MC6FIR:
            *slotLen = CVB_OCCUPIED_SLOT_NUM_MC6FIR;
            break;
        default:
            /* Do nothing */
            break;
    }
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_uc_param_chk_runtime(const RtUcEntry_t *ucEntry)
{
    RtkApiRet         ret = RT_ERR_OK;
    RtHostEntryType_e entryType;

    do
    {
        if(NULL == ucEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(ENABLED != ucEntry->nhAlloc)
        {
            ret = -RT_ERR_NOT_INIT;
            continue;
        }

        if(CVB_HOST_ROUTE_TABLE_MAX <= ucEntry->tblInfo.index)
        {
            ret = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        if(EM_HOST_TBL == ucEntry->tblInfo.tblType)
        {
            entryType = ucEntry->hostLuKey.entryType;
        }
        else if(EM_NET_TBL == ucEntry->tblInfo.tblType)
        {
            entryType = ucEntry->netLuKey.entryType;
        }
        else
        {
            ret = -RT_ERR_ROUTE_INVALID_TBL_TYPE;
            continue;
        }

        /* Check if is unicast type */
        if(0u != ((uint8)entryType % 2u))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mac_endian_covert(uint8 dst[MAC_ADDR_LEN], const uint8 src[MAC_ADDR_LEN])
{
    RtkApiRet ret = RT_ERR_OK;

    if((NULL == dst) || (NULL == src))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        dst[0] = src[5];
        dst[1] = src[4];
        dst[2] = src[3];
        dst[3] = src[2];
        dst[4] = src[1];
        dst[5] = src[0];
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_ip_endian_covert(uint8 dst[IPV6_ADDR_LEN], const uint8 src[IPV6_ADDR_LEN], uint8 ipVer)
{
    RtkApiRet ret = RT_ERR_OK;

    if((NULL == dst) || (NULL == src))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        if(IP6_VER == ipVer)
        {
            dst[0]  = src[15];
            dst[1]  = src[14];
            dst[2]  = src[13];
            dst[3]  = src[12];
            dst[4]  = src[11];
            dst[5]  = src[10];
            dst[6]  = src[9];
            dst[7]  = src[8];
            dst[8]  = src[7];
            dst[9]  = src[6];
            dst[10] = src[5];
            dst[11] = src[4];
            dst[12] = src[3];
            dst[13] = src[2];
            dst[14] = src[1];
            dst[15] = src[0];
        }
        else if(IP4_VER == ipVer)
        {
            dst[0] = src[3];
            dst[1] = src[2];
            dst[2] = src[1];
            dst[3] = src[0];
        }
        else
        {
            /* do nothing */
        }
    }

    return ret;
}

/* INDR_NUM6 is reserved for valid, so shift */
DYNAMIC_SCOPE void cvb_drv_rt_tbl_indr_6_sft(uint32 *data, uint64 dataLen)
{
    uint64 i;

    if(0u < dataLen)
    {
        /* dataLen=16
         * i = 15 14 13 12 11 10  9  8  7  6
         *     14 13 12 11 10  9  8  7  6 (1)
         */
        for(i = (dataLen - 1u); i > CVB_INDR_NUM_RESERVED; i--)
        {
            data[i] = data[(i - 1u)];
        }
        data[CVB_INDR_NUM_RESERVED] = ENABLED; /* READ-ONLY but it should be assigned to 1u for valid to avoid any risk. */
    }
}

DYNAMIC_SCOPE void cvb_drv_rt_tbl_indr_6_sft_restore(uint32 *data, uint64 dataLen)
{
    uint32 i;

    if(0u < dataLen)
    {
        /* dataLen=16
         * i =  6  7  8  9 10 11 12 13 14 15
         *      7  8  9 10 11 12 13 14 15 (0)
         */
        for(i = CVB_INDR_NUM_RESERVED; i < (dataLen - 1u); i++)
        {
            data[i] = data[(i + 1u)];
        }
        data[(dataLen - 1u)] = 0u;
    }
}

void cvb_drv_rt_array_bit32_get(const uint32 *src, uint32 *dest, uint32 lsb, uint32 msb)
{
    uint32        i;
    const uint32 *ptr;
    const uint32 *ptr1;
    uint32        val = 0u, mask = 0u, first_part_len = 0u;
    uint32        lsb_32, msb_32;

    ptr  = &src[lsb / 32u];
    ptr1 = &src[msb / 32u];

    if(ptr == ptr1)
    {
        lsb_32 = lsb % 32u;
        msb_32 = msb % 32u;
        for(i = lsb_32; i <= msb_32; i++)
        {
            mask |= ((uint32)0x1u << i);
        }

        val = *ptr;
        val &= mask;
        val   = val >> lsb_32;
        *dest = val;
    }
    else
    {
        lsb_32         = lsb % 32u;
        msb_32         = 31u;
        first_part_len = msb_32 - lsb_32 + 1u;
        for(i = lsb_32; i <= msb_32; i++)
        {
            mask |= ((uint32)0x1u << i);
        }

        val = *ptr;
        val &= mask;
        val   = val >> lsb_32;
        *dest = val;

        /* clear parameter */
        mask = 0u;

        lsb_32 = 0u;
        msb_32 = msb % 32u;
        for(i = lsb_32; i <= msb_32; i++)
        {
            mask |= ((uint32)0x1u << i);
        }

        val = *ptr1;
        val &= mask;
        val = val >> lsb_32;
        *dest |= (val << first_part_len);
    }
}

void cvb_drv_rt_array_bit32_set(uint32 *src, uint32 value, uint32 lsb, uint32 msb)
{
    uint32  i;
    uint32 *ptr, *ptr1;
    uint32  val = 0u, mask = 0u, mask_value_len = 0u, value2 = 0u;
    uint32  lsb_32, msb_32, len = 0u;

    ptr  = &src[lsb / 32u];
    ptr1 = &src[msb / 32u];

    if(ptr == ptr1)
    {
        lsb_32 = lsb % 32u;
        msb_32 = msb % 32u;
        if(msb_32 >= lsb_32)
        {
            len = msb_32 - lsb_32 + 1u;
        }
        for(i = lsb_32; i <= msb_32; i++)
        {
            mask |= ((uint32)0x1u << i);
        }
        for(i = 0u; i < len; i++)
        {
            mask_value_len |= ((uint32)0x1u << i);
        }

        val = *ptr;
        val &= (~mask);
        val |= ((value & mask_value_len) << lsb_32);
        *ptr = val;
    }
    else
    {
        lsb_32 = lsb % 32u;
        msb_32 = 31u;
        len    = msb_32 - lsb_32 + 1u;
        for(i = lsb_32; i <= msb_32; i++)
        {
            mask |= ((uint32)0x1u << i);
        }
        for(i = 0u; i < len; i++)
        {
            mask_value_len |= ((uint32)0x1u << i);
        }

        val = *ptr;
        val &= (~mask);
        val |= ((value & mask_value_len) << lsb_32);
        *ptr = val;

        /* clear parameter */
        mask           = 0u;
        mask_value_len = 0u;

        value2 = (value >> len);
        lsb_32 = 0u;
        msb_32 = msb % 32u;
        len    = msb_32 - lsb_32 + 1u;
        for(i = lsb_32; i <= msb_32; i++)
        {
            mask |= ((uint32)0x1u << i);
        }
        for(i = 0u; i < len; i++)
        {
            mask_value_len |= ((uint32)0x1u << i);
        }

        val = *ptr1;
        val &= (~mask);
        val |= ((value2 & mask_value_len) << lsb_32);
        *ptr1 = val;
    }
}

RtkApiRet cvb_drv_rt_rhtbl_get(uint32 unit, uint32 entryNum, cvb_RtRhTblParm_t *rhTblParm)
{
    cvb_RtRhTbl_t rhTblEntry;
    uint32        val    = 0u;
    RtkApiRet     retVal = RT_ERR_OK;

    if(entryNum >= CVB_RH_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(rhTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&rhTblEntry, 0, sizeof(rhTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_RH_TABLE, entryNum, rhTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(rhTblEntry.val, (uint32)(sizeof(rhTblEntry.val) / sizeof(rhTblEntry.val[0])));

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[0], &val, 0u, 13u);
            rhTblParm->portRhIdx[0] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[0], &val, 16u, 29u);
            rhTblParm->portRhIdx[1] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[1], &val, 0u, 13u);
            rhTblParm->portRhIdx[2] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[1], &val, 16u, 29u);
            rhTblParm->portRhIdx[3] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[2], &val, 0u, 13u);
            rhTblParm->portRhIdx[4] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[2], &val, 16u, 29u);
            rhTblParm->portRhIdx[5] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[3], &val, 0u, 13u);
            rhTblParm->portRhIdx[6] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[3], &val, 16u, 29u);
            rhTblParm->portRhIdx[7] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[4], &val, 0u, 13u);
            rhTblParm->portRhIdx[8] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[4], &val, 16u, 29u);
            rhTblParm->portRhIdx[9] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[5], &val, 0u, 13u);
            rhTblParm->portRhIdx[10] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[5], &val, 16u, 29u);
            rhTblParm->portRhIdx[11] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[6], &val, 0u, 13u);
            rhTblParm->portRhIdx[12] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[6], &val, 16u, 29u);
            rhTblParm->portRhIdx[13] = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&rhTblEntry.val[7], &val, 0u, 13u);
            rhTblParm->portRhIdx[14] = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_rhtbl_set(uint32 unit, uint32 entryNum, const cvb_RtRhTblParm_t *rhTblParm)
{
    cvb_RtRhTbl_t rhTblEntry;
    RtkApiRet     retVal = RT_ERR_OK;

    if(entryNum >= CVB_RH_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(rhTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&rhTblEntry, 0, sizeof(rhTblEntry)));

            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[0], rhTblParm->portRhIdx[0], 0u, 13u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[0], rhTblParm->portRhIdx[1], 16u, 29u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[1], rhTblParm->portRhIdx[2], 0u, 13u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[1], rhTblParm->portRhIdx[3], 16u, 29u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[2], rhTblParm->portRhIdx[4], 0u, 13u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[2], rhTblParm->portRhIdx[5], 16u, 29u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[3], rhTblParm->portRhIdx[6], 0u, 13u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[3], rhTblParm->portRhIdx[7], 16u, 29u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[4], rhTblParm->portRhIdx[8], 0u, 13u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[4], rhTblParm->portRhIdx[9], 16u, 29u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[5], rhTblParm->portRhIdx[10], 0u, 13u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[5], rhTblParm->portRhIdx[11], 16u, 29u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[6], rhTblParm->portRhIdx[12], 0u, 13u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[6], rhTblParm->portRhIdx[13], 16u, 29u);
            cvb_drv_rt_array_bit32_set(&rhTblEntry.val[7], rhTblParm->portRhIdx[14], 0u, 13u);

            cvb_drv_rt_tbl_indr_6_sft(rhTblEntry.val, sizeof(rhTblEntry.val) / sizeof(rhTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_RH_TABLE, entryNum, rhTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_oiltbl_get(uint32 unit, uint32 entryNum, cvb_RtOilTblParm_t *oilTblParm)
{
    cvb_tblAccess_OIL_TABLE_t oilTblEntry;
    uint32                    val    = 0u;
    RtkApiRet                 retVal = RT_ERR_OK;

    if(entryNum >= CVB_OIL_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(oilTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&oilTblEntry, 0, sizeof(oilTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_OIL_TABLE, entryNum, oilTblEntry.val));

            val = 0u;
            cvb_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 0u, 0u);
            oilTblParm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 1u, 1u);
            oilTblParm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 2u, 9u);
            oilTblParm->egrIfIdx = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 10u, 23u);
            oilTblParm->oilNext = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_oiltbl_set(uint32 unit, uint32 entryNum, const cvb_RtOilTblParm_t *oilTblParm)
{
    cvb_tblAccess_OIL_TABLE_t oilTblEntry;
    RtkApiRet                 retVal = RT_ERR_OK;

    if(entryNum >= CVB_OIL_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(oilTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&oilTblEntry, 0, sizeof(oilTblEntry)));

            cvb_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->ttlChk, 0u, 0u);
            cvb_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->ttlDec, 1u, 1u);
            cvb_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->egrIfIdx, 2u, 9u);
            cvb_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->oilNext, 10u, 23u);

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_OIL_TABLE, entryNum, oilTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_egriftbl_get(uint32 unit, uint32 entryNum, cvb_RtEgIfTblParm_t *egIfTblParm)
{
    cvb_RtEgIfTbl_t egIfTblEntry;
    uint32          val    = 0u;
    RtkApiRet       retVal = RT_ERR_OK;

    if(entryNum >= CVB_EGIF_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(egIfTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&egIfTblEntry, 0, sizeof(egIfTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_EGIF_TABLE, entryNum, egIfTblEntry.val));

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[0], &val, 0u, 31u);
            egIfTblParm->macId31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 0u, 15u);
            egIfTblParm->macId47to32 = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 16u, 16u);
            egIfTblParm->macFmtSel = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 17u, 17u);
            egIfTblParm->macReplace = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 18u, 29u);
            egIfTblParm->vid = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 30u, 30u);
            egIfTblParm->vidReplace = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 0u, 2u);
            egIfTblParm->pcp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 3u, 3u);
            egIfTblParm->pcpReplace = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 4u, 6u);
            egIfTblParm->ipMtuIdx = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 7u, 9u);
            egIfTblParm->ip6MtuIdx = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 10u, 17u);
            egIfTblParm->ipMcTtl = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 18u, 25u);
            egIfTblParm->ip6McHl = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 26u, 26u);
            egIfTblParm->icmpRedirChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 27u, 28u);
            egIfTblParm->icmpRedirAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 29u, 29u);
            egIfTblParm->rpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 30u, 31u);
            egIfTblParm->rpfFailAct = (uint8)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_egriftbl_set(uint32 unit, uint32 entryNum, const cvb_RtEgIfTblParm_t *egIfTblParm)
{
    cvb_RtEgIfTbl_t egIfTblEntry;
    RtkApiRet       retVal = RT_ERR_OK;

    if(entryNum >= CVB_EGIF_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(egIfTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&egIfTblEntry, 0, sizeof(egIfTblEntry)));

            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[0], egIfTblParm->macId31to0, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->macId47to32, 0u, 15u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->macFmtSel, 16u, 16u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->macReplace, 17u, 17u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->vid, 18u, 29u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->vidReplace, 30u, 30u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->pcp, 0u, 2u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->pcpReplace, 3u, 3u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ipMtuIdx, 4u, 6u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ip6MtuIdx, 7u, 9u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ipMcTtl, 10u, 17u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ip6McHl, 18u, 25u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->icmpRedirChkEn, 26u, 26u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->icmpRedirAct, 27u, 28u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->rpfChkEn, 29u, 29u);
            cvb_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->rpfFailAct, 30u, 31u);

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_EGIF_TABLE, entryNum, egIfTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_igriftbl_get(uint32 unit, uint32 entryNum, cvb_RtIngIfTblParm_t *ingIfTblParm)
{
    cvb_RtIngIfTbl_t ingIfTblEntry;
    uint32           val    = 0u;
    RtkApiRet        retVal = RT_ERR_OK;

    if(entryNum >= CVB_INGIF_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(ingIfTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&ingIfTblEntry, 0, sizeof(ingIfTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_INGIF_TABLE, entryNum, ingIfTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(ingIfTblEntry.val, sizeof(ingIfTblEntry.val) / sizeof(ingIfTblEntry.val[0]));

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[0], &val, 0u, 3u);
            ingIfTblParm->portId = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[0], &val, 4u, 15u);
            ingIfTblParm->ivid = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[0], &val, 16u, 31u);
            ingIfTblParm->macId15to0 = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[1], &val, 0u, 31u);
            ingIfTblParm->macId47to16 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[2], &val, 0u, 15u);
            ingIfTblParm->sp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[2], &val, 16u, 31u);
            ingIfTblParm->dp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 0u, 0u);
            ingIfTblParm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 1u, 1u);
            ingIfTblParm->intfIdVidKeySel = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 2u, 8u);
            ingIfTblParm->intfId = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 9u, 10u);
            ingIfTblParm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 11u, 12u);
            ingIfTblParm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 13u, 14u);
            ingIfTblParm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 15u, 15u);
            ingIfTblParm->ipUcRouteEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 16u, 16u);
            ingIfTblParm->ip6UcRouteEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 17u, 17u);
            ingIfTblParm->ipMcRouteEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 18u, 18u);
            ingIfTblParm->ip6McRouteEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 21u, 22u);
            ingIfTblParm->ipUrpfFailAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 23u, 23u);
            ingIfTblParm->ipUrpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 24u, 24u);
            ingIfTblParm->ipUrpfDftRouteEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 25u, 25u);
            ingIfTblParm->ipUrpfChkMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 26u, 27u);
            ingIfTblParm->ip6UrpfFailAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 28u, 28u);
            ingIfTblParm->ip6UrpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 29u, 29u);
            ingIfTblParm->ip6UrpfDftRouteEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 30u, 30u);
            ingIfTblParm->ip6UrpfChkMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[3], &val, 31u, 31u);
            ingIfTblParm->firMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[4], &val, 0u, 1u);
            ingIfTblParm->ipMcLuMisAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[4], &val, 2u, 3u);
            ingIfTblParm->ip6McLuMisAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[4], &val, 4u, 4u);
            ingIfTblParm->cmp0Gip = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[4], &val, 5u, 5u);
            ingIfTblParm->dipNoCare = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[4], &val, 6u, 6u);
            ingIfTblParm->sipNoCare = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[4], &val, 7u, 7u);
            ingIfTblParm->vidNoCare = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[5], &val, 0u, 3u);
            ingIfTblParm->portIdCare = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[5], &val, 4u, 15u);
            ingIfTblParm->ividCare = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[5], &val, 16u, 31u);
            ingIfTblParm->macId15to0Care = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[6], &val, 0u, 31u);
            ingIfTblParm->macId47to16Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[7], &val, 0u, 15u);
            ingIfTblParm->spCare = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&ingIfTblEntry.val[7], &val, 16u, 31u);
            ingIfTblParm->dpCare = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_igriftbl_set(uint32 unit, uint32 entryNum, const cvb_RtIngIfTblParm_t *ingIfTblParm)
{
    cvb_RtIngIfTbl_t ingIfTblEntry;
    RtkApiRet        retVal = RT_ERR_OK;

    if(entryNum >= CVB_INGIF_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(ingIfTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&ingIfTblEntry, 0, sizeof(ingIfTblEntry)));

            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[0], ingIfTblParm->portId, 0u, 3u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[0], ingIfTblParm->ivid, 4u, 15u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[0], ingIfTblParm->macId15to0, 16u, 31u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[1], ingIfTblParm->macId47to16, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[2], ingIfTblParm->sp, 0u, 15u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[2], ingIfTblParm->dp, 16u, 31u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->valid, 0u, 0u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->intfIdVidKeySel, 1u, 1u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->intfId, 2u, 8u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->spFmt, 9u, 10u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->dpFmt, 11u, 12u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->act, 13u, 14u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ipUcRouteEn, 15u, 15u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ip6UcRouteEn, 16u, 16u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ipMcRouteEn, 17u, 17u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ip6McRouteEn, 18u, 18u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ipUrpfFailAct, 21u, 22u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ipUrpfChkEn, 23u, 23u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ipUrpfDftRouteEn, 24u, 24u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ipUrpfChkMode, 25u, 25u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ip6UrpfFailAct, 26u, 27u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ip6UrpfChkEn, 28u, 28u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ip6UrpfDftRouteEn, 29u, 29u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->ip6UrpfChkMode, 30u, 30u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[3], ingIfTblParm->firMode, 31u, 31u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[4], ingIfTblParm->ipMcLuMisAct, 0u, 1u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[4], ingIfTblParm->ip6McLuMisAct, 2u, 3u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[4], ingIfTblParm->cmp0Gip, 4u, 4u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[4], ingIfTblParm->dipNoCare, 5u, 5u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[4], ingIfTblParm->sipNoCare, 6u, 6u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[4], ingIfTblParm->vidNoCare, 7u, 7u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[5], ingIfTblParm->portIdCare, 0u, 3u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[5], ingIfTblParm->ividCare, 4u, 15u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[5], ingIfTblParm->macId15to0Care, 16u, 31u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[6], ingIfTblParm->macId47to16Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[7], ingIfTblParm->spCare, 0u, 15u);
            cvb_drv_rt_array_bit32_set(&ingIfTblEntry.val[7], ingIfTblParm->dpCare, 16u, 31u);

            cvb_drv_rt_tbl_indr_6_sft(ingIfTblEntry.val, sizeof(ingIfTblEntry.val) / sizeof(ingIfTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_drv_tcam_write(unit, CVB_INGIF_TABLE, entryNum, ingIfTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc4_get(uint32                  unit,
                                  uint32                  entryNum,
                                  cvb_RtHostTblUc4Parm_t *hostTblUc4Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val       = 0u;
    uint32          offset    = 0u;
    uint32          dataRegNo = 0u;
    RtkApiRet       retVal    = RT_ERR_OK;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc4Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC4;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->nhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->arbMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4Parm->ipv4Dip = val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc4_set(uint32                        unit,
                                  uint32                        entryNum,
                                  const cvb_RtHostTblUc4Parm_t *hostTblUc4Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc4Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC4;

            dataRegNo = 0u;

            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->nhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4Parm->ipv4Dip, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc4fir_get(uint32                     unit,
                                     uint32                     entryNum,
                                     cvb_RtHostTblUc4FirParm_t *hostTblUc4FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val = 0u, offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC4FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->nhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->arbMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->ipv4Dip = val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->ipv4Sip = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->sp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc4FirParm->dp = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc4fir_set(uint32                           unit,
                                     uint32                           entryNum,
                                     const cvb_RtHostTblUc4FirParm_t *hostTblUc4FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC4FIR;

            dataRegNo = 0u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->nhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->ipv4Dip, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->ipv4Sip, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->sp, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc4FirParm->dp, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc6_get(uint32                  unit,
                                  uint32                  entryNum,
                                  cvb_RtHostTblUc6Parm_t *hostTblUc6Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val    = 0u;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc6Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC6;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->nhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->arbMode = (uint8)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->ipv6Dip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->ipv6Dip63to32 = val;

            dataRegNo = 2u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->ipv6Dip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6Parm->ipv6Dip127to96 = val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc6_set(uint32                        unit,
                                  uint32                        entryNum,
                                  const cvb_RtHostTblUc6Parm_t *hostTblUc6Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc6Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC6;

            dataRegNo = 0u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->valid, (68u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (68u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->type, (69u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (71u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->valid, (136u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (136u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->type, (137u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (139u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->nhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->ipv6Dip31to0, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->ipv6Dip63to32, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 2u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->ipv6Dip95to64, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6Parm->ipv6Dip127to96, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc6fir_get(uint32                     unit,
                                     uint32                     entryNum,
                                     cvb_RtHostTblUc6FirParm_t *hostTblUc6FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val = 0u, offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC6FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->nhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->arbMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->sp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->dp = (uint16)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Dip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Dip63to32 = val;

            dataRegNo = 2u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Dip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Dip127to96 = val;

            dataRegNo = 3u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Sip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Sip63to32 = val;

            dataRegNo = 4u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Sip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblUc6FirParm->ipv6Sip127to96 = val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_uc6fir_set(uint32                           unit,
                                     uint32                           entryNum,
                                     const cvb_RtHostTblUc6FirParm_t *hostTblUc6FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblUc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_UC6FIR;

            dataRegNo = 0u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (68u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (68u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (69u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (71u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (136u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (136u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (137u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (139u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (204u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (204u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (205u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (207u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (272u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (272u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (273u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (275u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (340u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (340u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (341u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (343u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->nhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->sp, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->dp, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Dip31to0, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Dip63to32, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 2u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Dip95to64, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Dip127to96, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 3u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Sip31to0, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Sip63to32, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 4u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Sip95to64, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblUc6FirParm->ipv6Sip127to96, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc4_get(uint32                  unit,
                                  uint32                  entryNum,
                                  cvb_RtHostTblMc4Parm_t *hostTblMc4Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val = 0u, offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc4Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC4;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->rhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->arbMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->ipv4Dip = val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->ipv4Sip = val;

            dataRegNo = 2u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->dpm = (cvb_RtkPmap)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->rpfFailAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->rpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->ttlMin = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4Parm->mtuMax = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc4_set(uint32                        unit,
                                  uint32                        entryNum,
                                  const cvb_RtHostTblMc4Parm_t *hostTblMc4Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc4Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC4;

            dataRegNo = 0u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->rhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->ipv4Dip, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->ipv4Sip, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 2u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->dpm, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->rpfFailAct, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->rpfChkEn, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->ttlMin, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4Parm->mtuMax, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc4fir_get(uint32                     unit,
                                     uint32                     entryNum,
                                     cvb_RtHostTblMc4FirParm_t *hostTblMc4FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val = 0u, offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC4FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->rhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->arbMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->ipv4Dip = val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->ipv4Sip = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->sp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->dp = (uint16)val;

            dataRegNo = 2u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->dpm = (cvb_RtkPmap)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->rpfFailAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->rpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->ttlMin = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc4FirParm->mtuMax = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc4fir_set(uint32                           unit,
                                     uint32                           entryNum,
                                     const cvb_RtHostTblMc4FirParm_t *hostTblMc4FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC4FIR;

            dataRegNo = 0u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->rhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->ipv4Dip, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->ipv4Sip, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->sp, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->dp, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 2u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->dpm, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->rpfFailAct, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->rpfChkEn, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->ttlMin, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc4FirParm->mtuMax, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc6_get(uint32                  unit,
                                  uint32                  entryNum,
                                  cvb_RtHostTblMc6Parm_t *hostTblMc6Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val = 0u, offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc6Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC6;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->rhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->arbMode = (uint8)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Dip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Dip63to32 = val;

            dataRegNo = 2u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Dip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Dip127to96 = val;

            dataRegNo = 3u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Sip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Sip63to32 = val;

            dataRegNo = 4u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Sip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ipv6Sip127to96 = val;

            dataRegNo = 5u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->dpm = (cvb_RtkPmap)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->rpfFailAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->rpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->ttlMin = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6Parm->mtuMax = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc6_set(uint32                        unit,
                                  uint32                        entryNum,
                                  const cvb_RtHostTblMc6Parm_t *hostTblMc6Parm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc6Parm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC6;

            dataRegNo = 0u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->rhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Dip31to0, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Dip63to32, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 2u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Dip95to64, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Dip127to96, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 3u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Sip31to0, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Sip63to32, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 4u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Sip95to64, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ipv6Sip127to96, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 5u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->dpm, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->rpfFailAct, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->rpfChkEn, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->ttlMin, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6Parm->mtuMax, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc6fir_get(uint32                     unit,
                                     uint32                     entryNum,
                                     cvb_RtHostTblMc6FirParm_t *hostTblMc6FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          val = 0u, offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC6FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->rhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->arbMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->sp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->dp = (uint16)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Dip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Dip63to32 = val;

            dataRegNo = 2u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Dip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Dip127to96 = val;

            dataRegNo = 3u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Sip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Sip63to32 = val;

            dataRegNo = 4u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Sip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ipv6Sip127to96 = val;

            dataRegNo = 5u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->dpm = (cvb_RtkPmap)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->rpfFailAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->rpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->ttlMin = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry.val[0], &val, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            hostTblMc6FirParm->mtuMax = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_host_mc6fir_set(uint32                           unit,
                                     uint32                           entryNum,
                                     const cvb_RtHostTblMc6FirParm_t *hostTblMc6FirParm, uint32 slotNum)
{
    cvb_RtHostTbl_t hostTblEntry;
    uint32          offset = 0u;
    RtkApiRet       retVal = RT_ERR_OK;
    uint32          dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblMc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostTblEntry, 0, sizeof(hostTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_DATA * CVB_OCCUPIED_SLOT_NUM_MC6FIR;

            dataRegNo = 0u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->qosPri, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (6u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->qosEn, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ttlChk, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (8u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ttlDec, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->dstNullIntf, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (10u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->hit, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->act, (12u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->rhIdx, (15u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (26u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->spFmt, (27u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (28u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->dpFmt, (29u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (30u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->arbValid, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->arbMode, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->sp, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (51u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->dp, (52u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Dip31to0, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Dip63to32, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 2u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Dip95to64, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Dip127to96, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 3u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Sip31to0, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Sip63to32, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 4u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Sip95to64, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ipv6Sip127to96, (36u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (67u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            dataRegNo = 5u;
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->valid, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (0u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->type, (1u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (3u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->dpm, (4u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->rpfFailAct, (20u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (21u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->rpfChkEn, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (22u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->ttlMin, (24u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (31u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostTblEntry.val[0], (uint32)hostTblMc6FirParm->mtuMax, (32u + (CVB_BITS_PER_DATA * dataRegNo)) + offset, (45u + (CVB_BITS_PER_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostTblEntry.val, sizeof(hostTblEntry.val) / sizeof(hostTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_nettbl_get(uint32 unit, uint32 entryNum, cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    cvb_RtNetRouteTbl_t netRouteTblEntry;
    uint32              val    = 0u;
    RtkApiRet           retVal = RT_ERR_OK;

    if(entryNum >= CVB_NETWORK_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(netRouteTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&netRouteTblEntry, 0, sizeof(netRouteTblEntry)));

            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_NETWORK_ROUTE_TABLE, entryNum, netRouteTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(netRouteTblEntry.val, sizeof(netRouteTblEntry.val) / sizeof(netRouteTblEntry.val[0]));

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[0], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[1], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip63to32 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[2], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[3], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip127to96 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[4], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[5], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip63to32 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[6], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[7], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip127to96 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[8], &val, 0u, 15u);
            netRouteTblParm->sp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[8], &val, 16u, 31u);
            netRouteTblParm->dp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 0u, 0u);
            netRouteTblParm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 1u, 3u);
            netRouteTblParm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 4u, 4u);
            netRouteTblParm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 5u, 16u);
            netRouteTblParm->nhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 17u, 17u);
            netRouteTblParm->hostRoute = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 18u, 18u);
            netRouteTblParm->dftRoute = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 19u, 19u);
            netRouteTblParm->dstNullIntf = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 20u, 21u);
            netRouteTblParm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 22u, 23u);
            netRouteTblParm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 24u, 26u);
            netRouteTblParm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 27u, 27u);
            netRouteTblParm->ttlDec = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 28u, 28u);
            netRouteTblParm->ttlChk = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[9], &val, 29u, 29u);
            netRouteTblParm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[10], &val, 0u, 2u);
            netRouteTblParm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[10], &val, 3u, 8u);
            netRouteTblParm->eppIdx = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[10], &val, 9u, 20u);
            netRouteTblParm->vid = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[10], &val, 21u, 21u);
            netRouteTblParm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[10], &val, 22u, 22u);
            netRouteTblParm->arbMode = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[11], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip31to0Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[12], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip63to32Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[13], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip95to64Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[14], &val, 0u, 31u);
            netRouteTblParm->ipv6Sip127to96Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[15], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip31to0Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[16], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip63to32Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[17], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip95to64Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[18], &val, 0u, 31u);
            netRouteTblParm->ipv6Dip127to96Care = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[19], &val, 0u, 15u);
            netRouteTblParm->spCare = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[19], &val, 16u, 31u);
            netRouteTblParm->dpCare = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[20], &val, 1u, 3u);
            netRouteTblParm->typeCare = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[20], &val, 20u, 21u);
            netRouteTblParm->spFmtCare = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[20], &val, 22u, 23u);
            netRouteTblParm->dpFmtCare = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&netRouteTblEntry.val[21], &val, 9u, 20u);
            netRouteTblParm->vidCare = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_nettbl_set(uint32 unit, uint32 entryNum, const cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    cvb_RtNetRouteTbl_t netRouteTblEntry;
    RtkApiRet           retVal = RT_ERR_OK;

    if(entryNum >= CVB_NETWORK_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(netRouteTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&netRouteTblEntry, 0, sizeof(netRouteTblEntry)));

            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[0], netRouteTblParm->ipv6Sip31to0, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[1], netRouteTblParm->ipv6Sip63to32, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[2], netRouteTblParm->ipv6Sip95to64, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[3], netRouteTblParm->ipv6Sip127to96, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[4], netRouteTblParm->ipv6Dip31to0, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[5], netRouteTblParm->ipv6Dip63to32, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[6], netRouteTblParm->ipv6Dip95to64, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[7], netRouteTblParm->ipv6Dip127to96, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[8], netRouteTblParm->sp, 0u, 15u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[8], netRouteTblParm->dp, 16u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->valid, 0u, 0u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->type, 1u, 3u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->hit, 4u, 4u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->nhIdx, 5u, 16u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->hostRoute, 17u, 17u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->dftRoute, 18u, 18u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->dstNullIntf, 19u, 19u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->spFmt, 20u, 21u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->dpFmt, 22u, 23u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->act, 24u, 26u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->ttlDec, 27u, 27u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->ttlChk, 28u, 28u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[9], netRouteTblParm->qosEn, 29u, 29u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[10], netRouteTblParm->qosPri, 0u, 2u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[10], netRouteTblParm->eppIdx, 3u, 8u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[10], netRouteTblParm->vid, 9u, 20u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[10], netRouteTblParm->arbValid, 21u, 21u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[10], netRouteTblParm->arbMode, 22u, 22u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[11], netRouteTblParm->ipv6Sip31to0Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[12], netRouteTblParm->ipv6Sip63to32Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[13], netRouteTblParm->ipv6Sip95to64Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[14], netRouteTblParm->ipv6Sip127to96Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[15], netRouteTblParm->ipv6Dip31to0Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[16], netRouteTblParm->ipv6Dip63to32Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[17], netRouteTblParm->ipv6Dip95to64Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[18], netRouteTblParm->ipv6Dip127to96Care, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[19], netRouteTblParm->spCare, 0u, 15u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[19], netRouteTblParm->dpCare, 16u, 31u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[20], netRouteTblParm->typeCare, 1u, 3u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[20], netRouteTblParm->spFmtCare, 20u, 21u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[20], netRouteTblParm->dpFmtCare, 22u, 23u);
            cvb_drv_rt_array_bit32_set(&netRouteTblEntry.val[21], netRouteTblParm->vidCare, 9u, 20u);

            cvb_drv_rt_tbl_indr_6_sft(netRouteTblEntry.val, sizeof(netRouteTblEntry.val) / sizeof(netRouteTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_drv_tcam_write(unit, CVB_NETWORK_ROUTE_TABLE, entryNum, netRouteTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_mccol_get(uint32                unit,
                               uint32                entryNum,
                               cvb_RtMcColTblParm_t *mcColTblParm)
{
    cvb_RtMcColTbl_t mcColTblEntry;
    uint32           val    = 0u;
    RtkApiRet        retVal = RT_ERR_OK;

    if(entryNum >= CVB_MULTICAST_COL_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(mcColTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&mcColTblEntry, 0, sizeof(mcColTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_MULTICAST_COL_TABLE, entryNum, mcColTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(mcColTblEntry.val, sizeof(mcColTblEntry.val) / sizeof(mcColTblEntry.val[0]));

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[0], &val, 0u, 31u);
            mcColTblParm->ipv6Sip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[1], &val, 0u, 31u);
            mcColTblParm->ipv6Sip63to32 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[2], &val, 0u, 31u);
            mcColTblParm->ipv6Sip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[3], &val, 0u, 31u);
            mcColTblParm->ipv6Sip127to96 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[4], &val, 0u, 31u);
            mcColTblParm->ipv6Dip31to0 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[5], &val, 0u, 31u);
            mcColTblParm->ipv6Dip63to32 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[6], &val, 0u, 31u);
            mcColTblParm->ipv6Dip95to64 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[7], &val, 0u, 31u);
            mcColTblParm->ipv6Dip127to96 = val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[8], &val, 0u, 15u);
            mcColTblParm->sp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[8], &val, 16u, 31u);
            mcColTblParm->dp = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 0u, 0u);
            mcColTblParm->valid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 1u, 3u);
            mcColTblParm->type = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 4u, 5u);
            mcColTblParm->spFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 6u, 7u);
            mcColTblParm->dpFmt = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 8u, 8u);
            mcColTblParm->hit = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 9u, 20u);
            mcColTblParm->rhIdx = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 21u, 22u);
            mcColTblParm->act = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[9], &val, 23u, 30u);
            mcColTblParm->ttlMin = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[10], &val, 0u, 15u);
            mcColTblParm->dpm = (cvb_RtkPmap)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[10], &val, 16u, 29u);
            mcColTblParm->mtuMax = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[10], &val, 30u, 30u);
            mcColTblParm->rpfChkEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[10], &val, 31u, 31u);
            mcColTblParm->qosEn = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[11], &val, 0u, 2u);
            mcColTblParm->qosPri = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[11], &val, 3u, 4u);
            mcColTblParm->rpfFailAct = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[11], &val, 5u, 16u);
            mcColTblParm->vid = (uint16)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[11], &val, 17u, 17u);
            mcColTblParm->arbValid = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&mcColTblEntry.val[11], &val, 18u, 18u);
            mcColTblParm->arbMode = (uint8)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_mccol_set(uint32                      unit,
                               uint32                      entryNum,
                               const cvb_RtMcColTblParm_t *mcColTblParm)
{
    cvb_RtMcColTbl_t mcColTblEntry;
    RtkApiRet        retVal = RT_ERR_OK;

    if(entryNum >= CVB_MULTICAST_COL_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(mcColTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&mcColTblEntry, 0, sizeof(mcColTblEntry)));

            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[0], mcColTblParm->ipv6Sip31to0, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[1], mcColTblParm->ipv6Sip63to32, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[2], mcColTblParm->ipv6Sip95to64, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[3], mcColTblParm->ipv6Sip127to96, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[4], mcColTblParm->ipv6Dip31to0, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[5], mcColTblParm->ipv6Dip63to32, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[6], mcColTblParm->ipv6Dip95to64, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[7], mcColTblParm->ipv6Dip127to96, 0u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[8], mcColTblParm->sp, 0u, 15u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[8], mcColTblParm->dp, 16u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->valid, 0u, 0u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->type, 1u, 3u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->spFmt, 4u, 5u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->dpFmt, 6u, 7u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->hit, 8u, 8u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->rhIdx, 9u, 20u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->act, 21u, 22u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[9], mcColTblParm->ttlMin, 23u, 30u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[10], mcColTblParm->dpm, 0u, 15u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[10], mcColTblParm->mtuMax, 16u, 29u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[10], mcColTblParm->rpfChkEn, 30u, 30u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[10], mcColTblParm->qosEn, 31u, 31u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[11], mcColTblParm->qosPri, 0u, 2u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[11], mcColTblParm->rpfFailAct, 3u, 4u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[11], mcColTblParm->vid, 5u, 16u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[11], mcColTblParm->arbValid, 17u, 17u);
            cvb_drv_rt_array_bit32_set(&mcColTblEntry.val[11], mcColTblParm->arbMode, 18u, 18u);

            cvb_drv_rt_tbl_indr_6_sft(mcColTblEntry.val, sizeof(mcColTblEntry.val) / sizeof(mcColTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_MULTICAST_COL_TABLE, entryNum, mcColTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_nhtbl_get(uint32             unit,
                               uint16             entryNum,
                               cvb_RtNhTblParm_t *nhTblParm)
{
    cvb_RtNhTbl_t nhTblEntry;
    uint32        val    = 0u;
    RtkApiRet     retVal = RT_ERR_OK;

    if(entryNum >= CVB_NEXT_HOP_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(nhTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&nhTblEntry, 0, sizeof(nhTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_NEXT_HOP_TABLE, entryNum, nhTblEntry.val));

            val = 0u;
            cvb_drv_rt_array_bit32_get(&nhTblEntry.val[0], &val, 0u, 5u);
            nhTblParm->egrIfIdx = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&nhTblEntry.val[0], &val, 6u, 18u);
            nhTblParm->l2Idx = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_nhtbl_set(uint32                   unit,
                               uint16                   entryNum,
                               const cvb_RtNhTblParm_t *nhTblParm)
{
    cvb_RtNhTbl_t nhTblEntry;
    RtkApiRet     retVal = RT_ERR_OK;

    if(entryNum >= CVB_NEXT_HOP_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(nhTblParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&nhTblEntry, 0, sizeof(nhTblEntry)));
            cvb_drv_rt_array_bit32_set(&nhTblEntry.val[0], nhTblParm->egrIfIdx, 0u, 5u);
            cvb_drv_rt_array_bit32_set(&nhTblEntry.val[0], nhTblParm->l2Idx, 6u, 18u);

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_NEXT_HOP_TABLE, entryNum, nhTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_uc4fir_get(uint32                  unit,
                                        uint32                  entryNum,
                                        cvb_RtHostGrpTblParm_t *hostGrpTblUc4FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             val = 0u, offset = 0u;
    RtkApiRet          retVal = SUCCESS;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblUc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_UC4FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->sipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->spGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->dipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->dpGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->vid = (uint16)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx0 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx1 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx4 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx5 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx8 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx9 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx12 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc4FirParm->aclBlkIdx13 = (uint8)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_uc4fir_set(uint32                        unit,
                                        uint32                        entryNum,
                                        const cvb_RtHostGrpTblParm_t *hostGrpTblUc4FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             offset = 0u;
    RtkApiRet          retVal = SUCCESS;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblUc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_UC4FIR;

            dataRegNo = 0u;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->sipGrp, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->spGrp, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->dipGrp, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->dpGrp, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->vid, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx0, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx1, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx4, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx5, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx8, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx9, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx12, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc4FirParm->aclBlkIdx13, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_uc6fir_get(uint32                  unit,
                                        uint32                  entryNum,
                                        cvb_RtHostGrpTblParm_t *hostGrpTblUc6FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             val = 0u, offset = 0u;
    RtkApiRet          retVal = SUCCESS;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblUc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_UC6FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->sipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->spGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->dipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->dpGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->vid = (uint16)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx0 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx1 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx4 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx5 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx8 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx9 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx12 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblUc6FirParm->aclBlkIdx13 = (uint8)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_uc6fir_set(uint32                        unit,
                                        uint32                        entryNum,
                                        const cvb_RtHostGrpTblParm_t *hostGrpTblUc6FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             offset = 0u;
    RtkApiRet          retVal = RT_ERR_OK;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblUc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_UC6FIR;

            dataRegNo = 0u;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->sipGrp, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->spGrp, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->dipGrp, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->dpGrp, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->vid, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx0, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx1, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx4, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx5, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx8, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx9, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx12, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblUc6FirParm->aclBlkIdx13, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_mc4fir_get(uint32                  unit,
                                        uint32                  entryNum,
                                        cvb_RtHostGrpTblParm_t *hostGrpTblMc4FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             val = 0u, offset = 0u;
    RtkApiRet          retVal = RT_ERR_OK;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblMc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_MC4FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->sipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->spGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->dipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->dpGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->vid = (uint16)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx0 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx1 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx4 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx5 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx8 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx9 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx12 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc4FirParm->aclBlkIdx13 = (uint8)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_mc4fir_set(uint32                        unit,
                                        uint32                        entryNum,
                                        const cvb_RtHostGrpTblParm_t *hostGrpTblMc4FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             offset = 0u;
    RtkApiRet          retVal = RT_ERR_OK;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblMc4FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_MC4FIR;

            dataRegNo = 0u;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->sipGrp, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->spGrp, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->dipGrp, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->dpGrp, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->vid, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            dataRegNo = 1u;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx0, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx1, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx4, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx5, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx8, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx9, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx12, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc4FirParm->aclBlkIdx13, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_mc6fir_get(uint32                  unit,
                                        uint32                  entryNum,
                                        cvb_RtHostGrpTblParm_t *hostGrpTblMc6FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             val = 0u, offset = 0u;
    RtkApiRet          retVal = RT_ERR_OK;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)  // CVB_HOST_ROUTE_TABLE_MAX could be accepted.
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblMc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_MC6FIR;

            dataRegNo = 0u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->sipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->spGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->dipGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->dpGrp = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->vid = (uint16)val;

            dataRegNo = 1u;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx0 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx1 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx4 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx5 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx8 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx9 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx12 = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            hostGrpTblMc6FirParm->aclBlkIdx13 = (uint8)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_hostgrp_mc6fir_set(uint32                        unit,
                                        uint32                        entryNum,
                                        const cvb_RtHostGrpTblParm_t *hostGrpTblMc6FirParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry;
    uint32             offset = 0u;
    RtkApiRet          retVal = RT_ERR_OK;
    uint32             dataRegNo;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTblMc6FirParm == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, drv_memset(&hostGrpTblEntry, 0, sizeof(hostGrpTblEntry)));
            CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

            cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            offset = slotNum * CVB_BITS_PER_GROUP_DATA * CVB_OCCUPIED_SLOT_NUM_MC6FIR;

            dataRegNo = 0;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->sipGrp, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->spGrp, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->dipGrp, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->dpGrp, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->vid, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            dataRegNo = 1;

            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx0, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx1, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx4, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx5, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx8, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx9, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx12, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
            cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblMc6FirParm->aclBlkIdx13, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

            cvb_drv_rt_tbl_indr_6_sft(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cvb_drv_rt_glb_reg_cfg_on(uint32 unit)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_IPUC_ROUTE_CTRL, ((uint32)1u << CVB_L3_IPUC_ROUTE_CTRL_NON_IP_ACT_LSP) | ((uint32)1u << CVB_L3_IPUC_ROUTE_CTRL_GLB_EN_LSP)));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_IP6UC_ROUTE_CTRL, ((uint32)1u << CVB_L3_IP6UC_ROUTE_CTRL_NON_IP_ACT_LSP) | ((uint32)1u << CVB_L3_IP6UC_ROUTE_CTRL_GLB_EN_LSP)));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TRAP_CPU_DPM, ((uint32)1u << 15u)));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_COPY_CPU_DPM, ((uint32)1u << 15u)));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_IPMC_ROUTE_CTRL, CVB_L3_IPMC_ROUTE_CTRL_GLB_EN_LSP, ENABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_IP6MC_ROUTE_CTRL, CVB_L3_IP6MC_ROUTE_CTRL_GLB_EN_LSP, ENABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_HOST_TBL_CTRL, CVB_L3_HOST_TBL_CTRL_UC_TBL1_ALG_LSP, ENABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3NH_L2DA_CONTROL, CVB_L3NH_L2DA_CONTROL_L2MC_DA_MISS_BYPASS_EN_LSP, ENABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, CVB_L3_GLOBAL_CONTROL0, &regVal));
        regVal |= (regVal | ((uint32)1u << CVB_L3_GLOBAL_CONTROL0_ROUTE_DPM_SEL_LSP) | ((uint32)1u << CVB_L3_GLOBAL_CONTROL0_ACL_UC_TTLDEC_LSP));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_GLOBAL_CONTROL0, regVal));
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_glb_reg_cfg_off(uint32 unit)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_IPUC_ROUTE_CTRL, CVB_L3_IPUC_ROUTE_CTRL_GLB_EN_LSP, DISABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_IP6UC_ROUTE_CTRL, CVB_L3_IP6UC_ROUTE_CTRL_GLB_EN_LSP, DISABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_IPMC_ROUTE_CTRL, CVB_L3_IPMC_ROUTE_CTRL_GLB_EN_LSP, DISABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_IP6MC_ROUTE_CTRL, CVB_L3_IP6MC_ROUTE_CTRL_GLB_EN_LSP, DISABLED));
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_HOST_TBL_CTRL, CVB_L3_HOST_TBL_CTRL_UC_TBL1_ALG_LSP, DISABLED));
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_dummy_intf_chk(uint32 unit, bool *status)
{
    RtkApiRet           ret          = RT_ERR_OK;
    cvb_RtEgIfTblParm_t egIfTblEntry = {0};

    ret = cvb_drv_rt_egriftbl_get(unit, 0u, &egIfTblEntry);

    if(ENABLED == egIfTblEntry.macReplace)
    {
        *status = false;
    }
    else
    {
        *status = true;
    }

    return ret;
}

RtkApiRet cvb_drv_rt_route_mode_set(uint32 unit, RtRouteMode_e *rtMode)
{
    RtkApiRet ret     = RT_ERR_OK;
    bool      status  = false;
    RtTbl_t   rtEntry = {0};

    do
    {
        /* Check if the interface is exist */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_dummy_intf_chk(unit, &status));
        if(true == status)
        {
            *rtMode = EM_FILTER_MODE;
        }
        else
        {
            rtEntry.index = 0u;
            CHK_FUN_CONTINUE(ret, cvb_route_rttbl_get(unit, &rtEntry));
            if(ENABLED == rtEntry.valid)
            {
                *rtMode = EM_ROUTE_DYNAMIC_MODE;
            }
            else
            {
                *rtMode = EM_ROUTE_STATIC_MODE;
            }
        }
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_grp_tbl_set(uint32                        unit,
                                                    RtHostEntryType_e             entryType,
                                                    uint32                        entryNum,
                                                    const cvb_RtHostGrpTblParm_t *hostGrpTblParm, uint32 slotNum)
{
    cvb_RtHostGrpTbl_t hostGrpTblEntry = {0};
    uint32             offset          = 0u;
    RtkApiRet          retVal          = RT_ERR_OK;
    uint8              maxSlotNum      = 0;
    uint8              slotOccupiedNo  = 0;
    uint32             dataRegNo;

    cvb_drv_rt_max_host_entry_idx_num_get(entryType, &maxSlotNum);
    cvb_drv_rt_occupied_slot_num_get(entryType, &slotOccupiedNo);

    do
    {
        if((entryNum >= cvb_table_list[CVB_HOST_GROUP_TABLE].size) || (slotNum >= maxSlotNum))
        {
            retVal = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        if(hostGrpTblParm == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));

        cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

        offset = slotNum * CVB_BITS_PER_GROUP_DATA * slotOccupiedNo;

        dataRegNo = 0u;
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->sipGrp, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->spGrp, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->dipGrp, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->dpGrp, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->vid, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

        dataRegNo = 1u;
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx0, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx1, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx4, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx5, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx8, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx9, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx12, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        cvb_drv_rt_array_bit32_set(&hostGrpTblEntry.val[0], (uint32)hostGrpTblParm->aclBlkIdx13, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);

        cvb_drv_rt_tbl_indr_6_sft(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

        CHK_FUN_CONTINUE(retVal, cvb_table_write(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val));
    } while(1 == 0);

    return retVal;
}

RtkApiRet cvb_drv_rt_host_grp_tbl_get(uint32 unit, RtHostEntryType_e entryType, uint32 entryNum, cvb_RtHostGrpTblParm_t *hostGrpTableParm, uint32 slotNum)
{
    RtkApiRet          ret             = RT_ERR_OK;
    cvb_RtHostGrpTbl_t hostGrpTblEntry = {0};
    uint32             offset          = 0;
    uint32             val             = 0;
    uint8              maxSlotNum      = 0;
    uint8              slotOccupiedNo  = 0;
    uint32             dataRegNo;

    cvb_drv_rt_max_host_entry_idx_num_get(entryType, &maxSlotNum);
    cvb_drv_rt_occupied_slot_num_get(entryType, &slotOccupiedNo);

    if((entryNum >= cvb_table_list[CVB_HOST_GROUP_TABLE].size) || (slotNum >= maxSlotNum))
    {
        ret = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostGrpTableParm == NULL)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        ret = cvb_table_read(unit, CVB_HOST_GROUP_TABLE, entryNum, hostGrpTblEntry.val);
        cvb_drv_rt_tbl_indr_6_sft_restore(hostGrpTblEntry.val, sizeof(hostGrpTblEntry.val) / sizeof(hostGrpTblEntry.val[0]));

        offset = slotNum * CVB_BITS_PER_GROUP_DATA * slotOccupiedNo;

        dataRegNo = 0u;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (7u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->sipGrp = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (8u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (11u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->spGrp = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (12u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->dipGrp = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (23u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->dpGrp = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->vid = (uint16)val;

        dataRegNo = 1u;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (0u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (4u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx0 = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (5u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (9u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx1 = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (10u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (14u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx4 = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (15u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (19u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx5 = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (20u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (24u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx8 = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (25u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (29u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx9 = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (30u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (34u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx12 = (uint8)val;

        cvb_drv_rt_array_bit32_get(&hostGrpTblEntry.val[0], &val, (35u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset, (39u + (CVB_BITS_PER_GROUP_DATA * dataRegNo)) + offset);
        hostGrpTableParm->aclBlkIdx13 = (uint8)val;
    }

    return ret;
}

RtkApiRet cvb_drv_rt_hw_igr_intf_set_runtime(uint32 unit, RtIntfCfg_t *intfCfg)
{
    RtkApiRet            ret               = RT_ERR_OK;
    cvb_RtIngIfTblParm_t igrIfEntry        = {0};
    uint8                mac[MAC_ADDR_LEN] = {0u};

    do
    {
        if(NULL == intfCfg)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(ENABLED == intfCfg->intfEntry.valid)
        {
            igrIfEntry.valid           = ENABLED;
            igrIfEntry.intfIdVidKeySel = CVB_IGRIF_SEL_VID;
            igrIfEntry.act             = CVB_IGRIF_ACT_ROUTE;
            igrIfEntry.ipUcRouteEn     = ENABLED;
            igrIfEntry.ip6UcRouteEn    = ENABLED;
            igrIfEntry.ipMcRouteEn     = ENABLED;
            igrIfEntry.ip6McRouteEn    = ENABLED;
            igrIfEntry.ipMcLuMisAct    = CVB_IGRIF_MC_LU_MISS_ACT_BRIDGE;
            igrIfEntry.ip6McLuMisAct   = CVB_IGRIF_MC_LU_MISS_ACT_BRIDGE;
            igrIfEntry.firMode         = intfCfg->intfEntry.firewall;
            igrIfEntry.cmp0Gip         = ENABLED;

            if(ENABLED != intfCfg->intfEntry.filter)
            {
                igrIfEntry.portId     = intfCfg->intfEntry.portId;
                igrIfEntry.portIdCare = 0xFu;

                igrIfEntry.intfId   = intfCfg->intfEntry.intfFidVid;
                igrIfEntry.ivid     = intfCfg->intfEntry.intfFidVid;
                igrIfEntry.ividCare = 0xFFFu;

                CHK_FUN_CONTINUE(ret, drv_endian_change(intfCfg->intfEntry.mac, mac, MAC_ADDR_LEN));
                CHK_FUN_CONTINUE(ret, drv_memcpy(&igrIfEntry.macId15to0, mac, 2u));
                CHK_FUN_CONTINUE(ret, drv_memcpy(&igrIfEntry.macId47to16, &mac[2], 4u));
                igrIfEntry.macId15to0Care  = 0xFFFFu;
                igrIfEntry.macId47to16Care = 0xFFFFFFFFu;
            }

            if(ENABLED == intfCfg->intfEntry.firewall)
            {
                igrIfEntry.vidNoCare = intfCfg->luKeyMode.vidNoCare;
                igrIfEntry.dipNoCare = intfCfg->luKeyMode.dipNoCare;
                igrIfEntry.sipNoCare = intfCfg->luKeyMode.sipNoCare;
                igrIfEntry.dpFmt     = (ENABLED == intfCfg->luKeyMode.dpNoCare) ? CVB_RT_L4FMT_NOCARE : intfCfg->intfEntry.dpFmt;
                igrIfEntry.spFmt     = (ENABLED == intfCfg->luKeyMode.spNoCare) ? CVB_RT_L4FMT_NOCARE : intfCfg->intfEntry.spFmt;
            }
        }
        ret = cvb_drv_rt_igriftbl_set(unit, intfCfg->intfEntry.index, &igrIfEntry);
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_hw_egr_intf_set(uint32 unit, const RtIntfTbl_t *intfEntry)
{
    RtkApiRet           ret               = RT_ERR_OK;
    cvb_RtEgIfTblParm_t egrIfEntry        = {0};
    uint8               mac[MAC_ADDR_LEN] = {0u};

    do
    {
        if(NULL == intfEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if((CVB_EGIF_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM) <= intfEntry->index)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if((ENABLED != intfEntry->filter) && (ENABLED == intfEntry->valid))
        {
            /* Set routing mode egress interface config */
            egrIfEntry.macReplace = ENABLED;
            egrIfEntry.macFmtSel  = CVB_MAC_FMT_DA;
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_mac_endian_covert(mac, intfEntry->mac));
            CHK_FUN_CONTINUE(ret, drv_memcpy(&egrIfEntry.macId31to0, mac, 4u));
            CHK_FUN_CONTINUE(ret, drv_memcpy(&egrIfEntry.macId47to32, &mac[4], 2u));
            egrIfEntry.vidReplace = ENABLED;
            egrIfEntry.vid        = intfEntry->intfFidVid;
            egrIfEntry.ipMtuIdx   = intfEntry->ipv4MtuIdx;
            egrIfEntry.ip6MtuIdx  = intfEntry->ipv6MtuIdx;
        }
        ret = cvb_drv_rt_egriftbl_set(unit, intfEntry->index, &egrIfEntry);
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc4_parm_set_runtime(const RtUcEntry_t *hostEntry, cvb_RtHostTblUc4Parm_t *hostTblParmUc4)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == hostEntry) || (NULL == hostTblParmUc4))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Change to Little-endian */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_ip_endian_covert(dip, hostEntry->hostLuKey.dipAddr, IP4_VER));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmUc4->ipv4Dip, dip, IPV4_ADDR_LEN));
        hostTblParmUc4->type   = (uint8)hostEntry->hostLuKey.entryType;
        hostTblParmUc4->valid  = hostEntry->valid;
        hostTblParmUc4->ttlChk = hostEntry->ttl;
        hostTblParmUc4->ttlDec = hostEntry->ttl;
        hostTblParmUc4->nhIdx  = hostEntry->nhIdx;
        hostTblParmUc4->act    = CVB_ACT_ROUTE;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc6_parm_set_runtime(const RtUcEntry_t *hostEntry, cvb_RtHostTblUc6Parm_t *hostTblParmUc6)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == hostEntry) || (NULL == hostTblParmUc6))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Change to Little-endian */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_ip_endian_covert(dip, hostEntry->hostLuKey.dipAddr, IP6_VER));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmUc6->ipv6Dip31to0, dip, IPV6_ADDR_LEN));
        hostTblParmUc6->type   = (uint8)hostEntry->hostLuKey.entryType;
        hostTblParmUc6->ttlChk = hostEntry->ttl;
        hostTblParmUc6->ttlDec = hostEntry->ttl;
        hostTblParmUc6->valid  = hostEntry->valid;
        hostTblParmUc6->nhIdx  = hostEntry->nhIdx;
        hostTblParmUc6->act    = CVB_ACT_ROUTE;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc4fir_parm_set_runtime(const RtUcEntry_t *hostEntry, cvb_RtHostTblUc4FirParm_t *hostTblParmUc4Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    if((NULL == hostEntry) || (NULL == hostTblParmUc4Fir))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            /* Change to Little-endian */
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_ip_endian_covert(dip, hostEntry->hostLuKey.dipAddr, IP4_VER));
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_ip_endian_covert(sip, hostEntry->hostLuKey.sipAddr, IP4_VER));
            CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmUc4Fir->ipv4Dip, dip, IPV4_ADDR_LEN));
            CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmUc4Fir->ipv4Sip, sip, IPV4_ADDR_LEN));
            if(CVB_RT_L4FMT_NOCARE != hostEntry->hostLuKey.dpFmt)
            {
                hostTblParmUc4Fir->dp = hostEntry->hostLuKey.dp;
            }
            if(CVB_RT_L4FMT_NOCARE != hostEntry->hostLuKey.spFmt)
            {
                hostTblParmUc4Fir->sp = hostEntry->hostLuKey.sp;
            }
            hostTblParmUc4Fir->spFmt    = hostEntry->hostLuKey.spFmt;
            hostTblParmUc4Fir->dpFmt    = hostEntry->hostLuKey.dpFmt;
            hostTblParmUc4Fir->type     = (uint8)hostEntry->hostLuKey.entryType;
            hostTblParmUc4Fir->valid    = hostEntry->valid;
            hostTblParmUc4Fir->ttlChk   = hostEntry->ttl;
            hostTblParmUc4Fir->ttlDec   = hostEntry->ttl;
            hostTblParmUc4Fir->nhIdx    = hostEntry->nhIdx;
            hostTblParmUc4Fir->arbValid = hostEntry->arbValid;
            hostTblParmUc4Fir->arbMode  = hostEntry->arbMode;
            hostTblParmUc4Fir->act      = CVB_ACT_ROUTE;
        } while(1 == 0);
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc6fir_parm_set_runtime(const RtUcEntry_t *hostEntry, cvb_RtHostTblUc6FirParm_t *hostTblParmUc6Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    if((NULL == hostEntry) || (NULL == hostTblParmUc6Fir))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            /* Change to Little-endian */
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_ip_endian_covert(dip, hostEntry->hostLuKey.dipAddr, IP6_VER));
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_ip_endian_covert(sip, hostEntry->hostLuKey.sipAddr, IP6_VER));
            CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmUc6Fir->ipv6Dip31to0, dip, IPV6_ADDR_LEN));
            CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmUc6Fir->ipv6Sip31to0, sip, IPV6_ADDR_LEN));
            if(CVB_RT_L4FMT_NOCARE != hostEntry->hostLuKey.dpFmt)
            {
                hostTblParmUc6Fir->dp = hostEntry->hostLuKey.dp;
            }
            if(CVB_RT_L4FMT_NOCARE != hostEntry->hostLuKey.spFmt)
            {
                hostTblParmUc6Fir->sp = hostEntry->hostLuKey.sp;
            }
            hostTblParmUc6Fir->type     = (uint8)hostEntry->hostLuKey.entryType;
            hostTblParmUc6Fir->valid    = hostEntry->valid;
            hostTblParmUc6Fir->ttlChk   = hostEntry->ttl;
            hostTblParmUc6Fir->ttlDec   = hostEntry->ttl;
            hostTblParmUc6Fir->nhIdx    = hostEntry->nhIdx;
            hostTblParmUc6Fir->spFmt    = hostEntry->hostLuKey.spFmt;
            hostTblParmUc6Fir->dpFmt    = hostEntry->hostLuKey.dpFmt;
            hostTblParmUc6Fir->arbValid = hostEntry->arbValid;
            hostTblParmUc6Fir->arbMode  = hostEntry->arbMode;
            hostTblParmUc6Fir->act      = CVB_ACT_ROUTE;
        } while(1 == 0);
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_hostgrp_parm_set_runtime(const RtHostlookUpKey_t *luKey, cvb_RtHostGrpTblParm_t *hostGrpTblParm)
{
    RtkApiRet ret = RT_ERR_OK;

    if((NULL == luKey) || (NULL == hostGrpTblParm))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        hostGrpTblParm->vid         = luKey->vid;
        hostGrpTblParm->aclBlkIdx0  = luKey->aclBlkIdx0;
        hostGrpTblParm->aclBlkIdx1  = luKey->aclBlkIdx1;
        hostGrpTblParm->aclBlkIdx4  = luKey->aclBlkIdx4;
        hostGrpTblParm->aclBlkIdx5  = luKey->aclBlkIdx5;
        hostGrpTblParm->aclBlkIdx8  = luKey->aclBlkIdx8;
        hostGrpTblParm->aclBlkIdx9  = luKey->aclBlkIdx9;
        hostGrpTblParm->aclBlkIdx12 = luKey->aclBlkIdx12;
        hostGrpTblParm->aclBlkIdx13 = luKey->aclBlkIdx13;
        hostGrpTblParm->dipGrp      = luKey->dipGrp;
        hostGrpTblParm->sipGrp      = luKey->sipGrp;
        hostGrpTblParm->dpGrp       = luKey->dpGrp;
        hostGrpTblParm->spGrp       = luKey->spGrp;
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_uc_host_tbl_entry_set_nonfir_runtime(uint32 unit, RtUcEntry_t *hostEntry)
{
    RtkApiRet              ret            = RT_ERR_OK;
    cvb_RtHostTblUc4Parm_t hostTblParmUc4 = {0};
    cvb_RtHostTblUc6Parm_t hostTblParmUc6 = {0};

    do
    {
        switch(hostEntry->hostLuKey.entryType)
        {
            case EM_RT_UC4:
                /* Set host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4_parm_set_runtime(hostEntry, &hostTblParmUc4));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4_set(unit, hostEntry->tblInfo.index, &hostTblParmUc4, hostEntry->tblInfo.entryIdx));
                break;
            case EM_RT_UC6:
                /* Set host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6_parm_set_runtime(hostEntry, &hostTblParmUc6));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6_set(unit, hostEntry->tblInfo.index, &hostTblParmUc6, hostEntry->tblInfo.entryIdx));
                break;
            default:
                /* Do nothing */
                break;
        };
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_uc_host_tbl_entry_set_fir_runtime(uint32 unit, RtUcEntry_t *hostEntry)
{
    RtkApiRet                 ret               = RT_ERR_OK;
    cvb_RtHostTblUc4FirParm_t hostTblParmUc4Fir = {0};
    cvb_RtHostTblUc6FirParm_t hostTblParmUc6Fir = {0};
    cvb_RtHostGrpTblParm_t    hostGrpTblParm    = {0};

    do
    {
        switch(hostEntry->hostLuKey.entryType)
        {
            case EM_RT_UC4FIR:
                /* Set host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4fir_parm_set_runtime(hostEntry, &hostTblParmUc4Fir));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4fir_set(unit, hostEntry->tblInfo.index, &hostTblParmUc4Fir, hostEntry->tblInfo.entryIdx));
                /* Set grp parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_hostgrp_parm_set_runtime(&hostEntry->hostLuKey, &hostGrpTblParm));
                /* Set Grp tbl */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_grp_tbl_set(unit, hostEntry->hostLuKey.entryType, hostEntry->tblInfo.index, &hostGrpTblParm, hostEntry->tblInfo.entryIdx));
                break;
            case EM_RT_UC6FIR:
                /* Set host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6fir_parm_set_runtime(hostEntry, &hostTblParmUc6Fir));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6fir_set(unit, hostEntry->tblInfo.index, &hostTblParmUc6Fir, hostEntry->tblInfo.entryIdx));
                /* Set grp parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_hostgrp_parm_set_runtime(&hostEntry->hostLuKey, &hostGrpTblParm));
                /* Set Grp tbl */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_grp_tbl_set(unit, hostEntry->hostLuKey.entryType, hostEntry->tblInfo.index, &hostGrpTblParm, hostEntry->tblInfo.entryIdx));
                break;
            default:
                /* Do nothing */
                break;
        };
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_rt_uc_host_tbl_entry_runtime_set(uint32 unit, RtUcEntry_t *hostEntry)
{
    RtkApiRet         ret        = RT_ERR_OK;
    uint8             maxSlotNum = 0;
    cvb_RtNhTblParm_t nhTblParm  = {0};

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_uc_param_chk_runtime(hostEntry));

        cvb_drv_rt_max_host_entry_idx_num_get(hostEntry->hostLuKey.entryType, &maxSlotNum);

        if(maxSlotNum <= hostEntry->tblInfo.entryIdx)
        {
            ret = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        switch(hostEntry->hostLuKey.entryType)
        {
            case EM_RT_UC4:
            case EM_RT_UC6:
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_uc_host_tbl_entry_set_nonfir_runtime(unit, hostEntry));
                break;
            case EM_RT_UC4FIR:
            case EM_RT_UC6FIR:
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_uc_host_tbl_entry_set_fir_runtime(unit, hostEntry));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        };

        /* Set nh parm */
        nhTblParm.egrIfIdx = hostEntry->egrIfIdx;
        nhTblParm.l2Idx    = hostEntry->l2Idx;
        /* Set nh */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nhtbl_set(unit, hostEntry->nhIdx, &nhTblParm));
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_net_nonfir_parm_set_runtime(RtUcEntry_t *ucEntry, cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    RtkApiRet ret                    = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN]     = {0};
    uint8     dipCare[IPV6_ADDR_LEN] = {0};
    uint8     ipVer;

    do
    {
        if((NULL == ucEntry) || (NULL == netRouteTblParm))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        ipVer = (IP6_TYPE_BIT == ((uint8)ucEntry->netLuKey.entryType & IP6_TYPE_BIT)) ? IP6_VER : IP4_VER;
        /* Change to Little-endian */
        CHK_FUN_CONTINUE(ret, drv_endian_change(ucEntry->netLuKey.dipAddr, dip, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(ucEntry->netLuKey.dipCare, dipCare, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&netRouteTblParm->ipv6Dip31to0, dip, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&netRouteTblParm->ipv6Dip31to0Care, dipCare, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        netRouteTblParm->type     = (uint8)ucEntry->netLuKey.entryType;
        netRouteTblParm->typeCare = (ENABLED == ucEntry->netLuKey.typeCare) ? 0xFFu : 0u;
        netRouteTblParm->valid    = ucEntry->valid;
        netRouteTblParm->ttlChk   = ucEntry->ttl;
        netRouteTblParm->ttlDec   = ucEntry->ttl;
        netRouteTblParm->nhIdx    = ucEntry->nhIdx;
        netRouteTblParm->act      = CVB_ACT_ROUTE;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_net_ip_set(RtUcEntry_t *ucEntry, cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    RtkApiRet ret                    = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN]     = {0};
    uint8     dipCare[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN]     = {0};
    uint8     sipCare[IPV6_ADDR_LEN] = {0};
    uint32    ipLen;

    if((NULL == ucEntry) || (NULL == netRouteTblParm))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        ipLen = (IP6_TYPE_BIT == ((uint8)ucEntry->netLuKey.entryType & IP6_TYPE_BIT)) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN;

        CHK_FUN_RET(ret, drv_endian_change(ucEntry->netLuKey.dipAddr, dip, ipLen));
        CHK_FUN_RET(ret, drv_endian_change(ucEntry->netLuKey.dipCare, dipCare, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&netRouteTblParm->ipv6Dip31to0, dip, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&netRouteTblParm->ipv6Dip31to0Care, dipCare, ipLen));

        CHK_FUN_RET(ret, drv_endian_change(ucEntry->netLuKey.sipAddr, sip, ipLen));
        CHK_FUN_RET(ret, drv_endian_change(ucEntry->netLuKey.sipCare, sipCare, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&netRouteTblParm->ipv6Sip31to0, sip, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&netRouteTblParm->ipv6Sip31to0Care, sipCare, ipLen));
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_net_fir_parm_set_runtime(RtUcEntry_t *ucEntry, cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((NULL == ucEntry) || (NULL == netRouteTblParm))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, cvb_drv_rt_net_ip_set(ucEntry, netRouteTblParm));

        netRouteTblParm->dp        = ucEntry->netLuKey.dp;
        netRouteTblParm->dpCare    = ucEntry->netLuKey.dpCare;
        netRouteTblParm->sp        = ucEntry->netLuKey.sp;
        netRouteTblParm->spCare    = ucEntry->netLuKey.spCare;
        netRouteTblParm->vid       = ucEntry->netLuKey.vid;
        netRouteTblParm->vidCare   = ucEntry->netLuKey.vidCare;
        netRouteTblParm->type      = (uint8)ucEntry->netLuKey.entryType;
        netRouteTblParm->typeCare  = (ENABLED == ucEntry->netLuKey.typeCare) ? 0xFFu : 0u;
        netRouteTblParm->spFmt     = ucEntry->netLuKey.spFmt;
        netRouteTblParm->spFmtCare = (ENABLED == ucEntry->netLuKey.spFmtCare) ? 0xFFu : 0u;
        netRouteTblParm->dpFmt     = ucEntry->netLuKey.dpFmt;
        netRouteTblParm->dpFmtCare = (ENABLED == ucEntry->netLuKey.dpFmtCare) ? 0xFFu : 0u;
        netRouteTblParm->valid     = ucEntry->valid;
        netRouteTblParm->ttlChk    = ucEntry->ttl;
        netRouteTblParm->ttlDec    = ucEntry->ttl;
        netRouteTblParm->nhIdx     = ucEntry->nhIdx;
        netRouteTblParm->arbValid  = ucEntry->arbValid;
        netRouteTblParm->arbMode   = ucEntry->arbMode;
        netRouteTblParm->act       = CVB_ACT_ROUTE;
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_net_tbl_entry_set_runtime(uint32 unit, RtUcEntry_t *ucEntry)
{
    RtkApiRet               ret             = RT_ERR_OK;
    cvb_RtNetRouteTblParm_t netRouteTblParm = {0};
    cvb_RtNhTblParm_t       nhTblParm       = {0};

    do
    {
        /* Check param */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_uc_param_chk_runtime(ucEntry));

        /* Set net param */
        switch(ucEntry->netLuKey.entryType)
        {
            case EM_RT_UC4:
            case EM_RT_UC6:
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_net_nonfir_parm_set_runtime(ucEntry, &netRouteTblParm));
                break;
            case EM_RT_UC4FIR:
            case EM_RT_UC6FIR:
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_net_fir_parm_set_runtime(ucEntry, &netRouteTblParm));
                break;
            default:
                /* Do nothing */
                break;
        };
        /* Set net table */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nettbl_set(unit, ucEntry->tblInfo.index, &netRouteTblParm));

        /* Set nh parm */
        nhTblParm.egrIfIdx = ucEntry->egrIfIdx;
        nhTblParm.l2Idx    = ucEntry->l2Idx;
        /* Set nh */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nhtbl_set(unit, ucEntry->nhIdx, &nhTblParm));
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_rt_nhtbl_avail_set_runtime(uint32 unit, uint16 nhIdx, uint8 valid)
{
    RtkApiRet ret = RT_ERR_OK;
    uint16    index;
    uint32    val;

    do
    {
        if(CVB_NEXT_HOP_TABLE_MAX <= nhIdx)
        {
            ret = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        index = nhIdx / 32u;

        CHK_FUN_CONTINUE(ret, cvb_drv_indirect_sram_get(unit, CVB_NH_POOL_ADDR + ((uint32)index * 4u), &val));

        if(ENABLED == valid)
        {

            val |= ((uint32)ENABLED << (nhIdx % 32u));
        }
        else
        {

            val &= ~((uint32)ENABLED << (nhIdx % 32u));
        }

        CHK_FUN_CONTINUE(ret, cvb_drv_indirect_sram_set(unit, CVB_NH_POOL_ADDR + ((uint32)index * 4u), val));
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_rt_nhtbl_avail_get_runtime(uint32 unit, uint16 *nhIdx)
{
    uint16    index                                = 0u;
    RtkApiRet ret                                  = RT_ERR_OK;
    uint32    nhPool[CVB_NEXT_HOP_TABLE_MAX / 32u] = {0};
    uint32    nhPoolDataRegNo                      = CVB_NEXT_HOP_TABLE_MAX / 32u;

    do
    {
        if(NULL == nhIdx)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Read nhPool */
        for(index = 0u; index < nhPoolDataRegNo; index++)
        {
            CHK_FUN_BREAK(ret, cvb_drv_indirect_sram_get(unit, CVB_NH_POOL_ADDR + ((uint32)index * 4u), &nhPool[index]));
        }

        if(RT_ERR_OK == ret)
        {
            /* Search avail nh idx */
            for(index = 0u; index < CVB_NEXT_HOP_TABLE_MAX; index++)
            {
                if(((nhPool[index / 32u] >> (index % 32u)) & ENABLED) == DISABLED)
                {
                    *nhIdx = index;
                    break;
                }
            }

            if(CVB_NEXT_HOP_TABLE_MAX == index)
            {
                ret = -RT_ERR_TBL_FULL;
            }
        }
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_param_chk_runtime(const RtMcEntry_t *mcEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == mcEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(CVB_HOST_ROUTE_TABLE_MAX <= mcEntry->tblInfo.index)
        {
            ret = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        /* Check if is multicast type */
        if(0u == ((uint8)mcEntry->luKey.entryType % 2u))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc4_parm_set_runtime(RtMcEntry_t *mcEntry, cvb_RtHostTblMc4Parm_t *hostTblParmMc4)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc4))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.dipAddr, dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.sipAddr, sip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc4->ipv4Dip, dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc4->ipv4Sip, sip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(mcEntry->dpm, &hostTblParmMc4->dpm, CVB_EM_USER_TO_SYS));
        hostTblParmMc4->type   = (uint8)mcEntry->luKey.entryType;
        hostTblParmMc4->valid  = mcEntry->valid;
        hostTblParmMc4->rhIdx  = mcEntry->rhTblIdx;
        hostTblParmMc4->ttlMin = 1u;
        hostTblParmMc4->act    = CVB_ACT_ROUTE;
        hostTblParmMc4->mtuMax = CVB_RT_DFT_MTU_MAX_SIZE;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc6_parm_set_runtime(RtMcEntry_t *mcEntry, cvb_RtHostTblMc6Parm_t *hostTblParmMc6)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc6))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.dipAddr, dip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.sipAddr, sip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc6->ipv6Dip31to0, dip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc6->ipv6Sip31to0, sip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(mcEntry->dpm, &hostTblParmMc6->dpm, CVB_EM_USER_TO_SYS));
        hostTblParmMc6->type   = (uint8)mcEntry->luKey.entryType;
        hostTblParmMc6->valid  = mcEntry->valid;
        hostTblParmMc6->rhIdx  = mcEntry->rhTblIdx;
        hostTblParmMc6->ttlMin = 1u;
        hostTblParmMc6->act    = CVB_ACT_ROUTE;
        hostTblParmMc6->mtuMax = CVB_RT_DFT_MTU_MAX_SIZE;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc4fir_parm_set_runtime(RtMcEntry_t *mcEntry, cvb_RtHostTblMc4FirParm_t *hostTblParmMc4Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc4Fir))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.dipAddr, dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.sipAddr, sip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc4Fir->ipv4Dip, dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc4Fir->ipv4Sip, sip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(mcEntry->dpm, &hostTblParmMc4Fir->dpm, CVB_EM_USER_TO_SYS));
        if(CVB_RT_L4FMT_NOCARE != mcEntry->luKey.dpFmt)
        {
            hostTblParmMc4Fir->dp = mcEntry->luKey.dp;
        }
        if(CVB_RT_L4FMT_NOCARE != mcEntry->luKey.spFmt)
        {
            hostTblParmMc4Fir->sp = mcEntry->luKey.sp;
        }
        hostTblParmMc4Fir->spFmt    = mcEntry->luKey.spFmt;
        hostTblParmMc4Fir->dpFmt    = mcEntry->luKey.dpFmt;
        hostTblParmMc4Fir->type     = (uint8)mcEntry->luKey.entryType;
        hostTblParmMc4Fir->valid    = mcEntry->valid;
        hostTblParmMc4Fir->rhIdx    = mcEntry->rhTblIdx;
        hostTblParmMc4Fir->arbValid = mcEntry->arbValid;
        hostTblParmMc4Fir->arbMode  = mcEntry->arbMode;
        hostTblParmMc4Fir->ttlMin   = 1u;
        hostTblParmMc4Fir->act      = CVB_ACT_ROUTE;
        hostTblParmMc4Fir->mtuMax   = CVB_RT_DFT_MTU_MAX_SIZE;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc6fir_parm_set_runtime(RtMcEntry_t *mcEntry, cvb_RtHostTblMc6FirParm_t *hostTblParmMc6Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc6Fir))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.dipAddr, dip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(mcEntry->luKey.sipAddr, sip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc6Fir->ipv6Dip31to0, dip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&hostTblParmMc6Fir->ipv6Sip31to0, sip, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(mcEntry->dpm, &hostTblParmMc6Fir->dpm, CVB_EM_USER_TO_SYS));
        if(CVB_RT_L4FMT_NOCARE != mcEntry->luKey.dpFmt)
        {
            hostTblParmMc6Fir->dp = mcEntry->luKey.dp;
        }
        if(CVB_RT_L4FMT_NOCARE != mcEntry->luKey.spFmt)
        {
            hostTblParmMc6Fir->sp = mcEntry->luKey.sp;
        }
        hostTblParmMc6Fir->spFmt    = mcEntry->luKey.spFmt;
        hostTblParmMc6Fir->dpFmt    = mcEntry->luKey.dpFmt;
        hostTblParmMc6Fir->type     = (uint8)mcEntry->luKey.entryType;
        hostTblParmMc6Fir->valid    = mcEntry->valid;
        hostTblParmMc6Fir->rhIdx    = mcEntry->rhTblIdx;
        hostTblParmMc6Fir->arbValid = mcEntry->arbValid;
        hostTblParmMc6Fir->arbMode  = mcEntry->arbMode;
        hostTblParmMc6Fir->ttlMin   = 1u;
        hostTblParmMc6Fir->act      = CVB_ACT_ROUTE;
        hostTblParmMc6Fir->mtuMax   = CVB_RT_DFT_MTU_MAX_SIZE;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_rhtbl_avail_idx_get_runtime(uint32 unit, uint16 *rhIdx)
{
    RtkApiRet               ret           = RT_ERR_OK;
    cvb_RtRhTblParm_t       rhTblParm     = {0};
    const cvb_RtRhTblParm_t zeroRhTblParm = {0};
    uint16                  idx           = 0;
    bool                    breakFlag     = false;

    if(NULL == rhIdx)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        for(idx = 0u; idx < (CVB_RH_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM); idx++)
        {
            ret = cvb_drv_rt_rhtbl_get(unit, idx, &rhTblParm);

            if(0 == rtl_memcmp(&rhTblParm, &zeroRhTblParm, sizeof(cvb_RtRhTblParm_t)))
            {
                *rhIdx    = idx;
                breakFlag = true;
            }

            if((RT_ERR_OK != ret) || breakFlag)
            {
                break;
            }
        }

        if(idx == (CVB_RH_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM))
        {
            ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_oiltbl_avail_idx_get_runtime(uint32 unit, uint32 *oilList)
{
    RtkApiRet          ret          = RT_ERR_OK;
    cvb_RtRhTblParm_t  rhTblParm    = {0};
    cvb_RtOilTblParm_t oilTableParm = {0};
    uint16             oilCnt       = 0u;
    uint16             idx;
    uint8              j;
    uint16             tmpOilIdx;

    do
    {
        /* Collect rh indicating OIL */
        for(idx = 0u; idx < (CVB_RH_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM); idx++)
        {
            /* Traverse rh entry */
            ret = cvb_drv_rt_rhtbl_get(unit, idx, &rhTblParm);
            for(j = 0u; j < 15u; j++)
            {
                /* Traverse OIL */
                tmpOilIdx = (uint16)rhTblParm.portRhIdx[j] & CVB_OIL_IDX_MASK;
                do
                {
                    oilList[tmpOilIdx / 32u] |= ((uint32)ENABLED << (tmpOilIdx % 32u));
                    ret       = cvb_drv_rt_oiltbl_get(unit, tmpOilIdx, &oilTableParm);
                    tmpOilIdx = (uint16)oilTableParm.oilNext & CVB_OIL_IDX_MASK;
                    oilCnt++;
                } while((0u != (oilTableParm.oilNext & CVB_OIL_VALID_FLAG)) && (CVB_OIL_TABLE_MAX > oilCnt));
            }
        }

        if((CVB_OIL_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM) == idx)
        {
            ret = -RT_ERR_TBL_FULL;
        }

    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_port_egrif_list_get(uint32 unit, const uint8 *nextHopIf, uint32 portEgrIfList[][2])
{
    RtkApiRet            ret          = RT_ERR_OK;
    cvb_RtIngIfTblParm_t ingIfTblParm = {0};
    uint8                ifIdx;

    for(ifIdx = 0u; ifIdx < (CVB_EGIF_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM); ifIdx++)
    {
        if(0u != (nextHopIf[ifIdx / 8u] & (ENABLED << (ifIdx % 8u))))
        {
            ret = cvb_drv_rt_igriftbl_get(unit, ifIdx, &ingIfTblParm);
            portEgrIfList[ingIfTblParm.portId][ifIdx / 32u] |= ((uint32)ENABLED << (ifIdx % 32u));
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_avail_oil_idx_get(uint16 startIdx, uint16 *availIdx, const uint32 *oilList)
{
    RtkApiRet ret = RT_ERR_OK;
    uint16    idx;

    if((NULL == availIdx) || (NULL == oilList))
    {
        ret = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == ret)
    {
        /* Get 1st avail OIL index */
        for(idx = startIdx; idx < (CVB_OIL_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM); idx++)
        {
            if(DISABLED == (oilList[idx / 32u] & ((uint32)ENABLED << (idx % 32u))))
            {
                *availIdx = idx;
                break;
            }
        }

        if((CVB_OIL_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM) == idx)
        {
            ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_avail_oil_idx_set(uint16 idx, uint32 *oilList)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == oilList)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        oilList[idx / 32u] |= (uint32)ENABLED << (idx % 32u);
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_oil_list_set(uint32 unit, cvb_RtRhTblParm_t *rhTblParm, cvb_RtOilListInfo_t *oilListInfo, uint32 *oilList)
{
    RtkApiRet          ret       = RT_ERR_OK;
    bool               isFirstIf = true;
    uint16             oilIdx    = 0u;
    cvb_RtOilTblParm_t oilTableParm;
    uint8              ifIdx;

    for(ifIdx = 0u; ifIdx < (CVB_EGIF_TABLE_MAX - CVB_CB_EGR_ENTRY_NUM); ifIdx++)
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&oilTableParm, 0, sizeof(cvb_RtOilTblParm_t)));

        if(0u != (oilListInfo->portEgrIfList[oilListInfo->port][ifIdx / 32u] & ((uint32)ENABLED << (ifIdx % 32u))))
        {
            CHK_FUN_BREAK(ret, cvb_drv_avail_oil_idx_get(oilIdx, &oilIdx, oilList));

            if(isFirstIf)
            {
                /* Set Rh param */
                rhTblParm->portRhIdx[oilListInfo->port] = CVB_OIL_VALID_FLAG | oilIdx;
                /* Set OIL tbl */
                oilTableParm.ttlChk   = oilListInfo->ttl;
                oilTableParm.ttlDec   = oilListInfo->ttl;
                oilTableParm.egrIfIdx = ifIdx;
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_oiltbl_set(unit, oilIdx, &oilTableParm));
                isFirstIf = false;
                CHK_FUN_CONTINUE(ret, cvb_drv_avail_oil_idx_set(oilIdx, oilList));
            }
            else
            {
                /* Set last OIL tbl */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_oiltbl_get(unit, (uint32)oilIdx - 1u, &oilTableParm));
                oilTableParm.oilNext = CVB_OIL_VALID_FLAG | oilIdx;
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_oiltbl_set(unit, (uint32)oilIdx - 1u, &oilTableParm));
                /* Set cur OIL tbl */
                oilTableParm.ttlChk   = oilListInfo->ttl;
                oilTableParm.ttlDec   = oilListInfo->ttl;
                oilTableParm.egrIfIdx = ifIdx;
                oilTableParm.oilNext  = 0u;
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_oiltbl_set(unit, (uint32)oilIdx, &oilTableParm));
                CHK_FUN_CONTINUE(ret, cvb_drv_avail_oil_idx_set(oilIdx, oilList));
            }
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_egr_info_set_runtime(uint32 unit, const RtMcEntry_t *mcEntry)
{
    RtkApiRet           ret                               = RT_ERR_OK;
    cvb_RtkPmap         sysDpm                            = 0;
    uint8               zeroNextHopIf[8]                  = {0};
    cvb_RtRhTblParm_t   rhTblParm                         = {0};
    cvb_RtOilTblParm_t  oilTableParm                      = {0};
    cvb_RtOilListInfo_t oilListInfo                       = {0};
    const uint32        zeroEgrIfList[2]                  = {0};
    uint32              oilList[CVB_RT_MAX_OIL_NUM / 32u] = {0};
    uint8               i;

    do
    {
        if(NULL == mcEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(mcEntry->dpm, &sysDpm, CVB_EM_USER_TO_SYS));
        /* Routing mode */
        if(0 != rtl_memcmp(mcEntry->nextHopIf, zeroNextHopIf, sizeof(zeroNextHopIf)))
        {
            /* Get per port interface list */
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_port_egrif_list_get(unit, mcEntry->nextHopIf, oilListInfo.portEgrIfList));
            /* Get OIL list */
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_oiltbl_avail_idx_get_runtime(unit, oilList));
            /* Get ttl */
            oilListInfo.ttl = mcEntry->ttl;

            for(i = CVB_RTK_SYS_PORT_1ST; i <= CVB_RTK_SYS_PORT_END; i++)
            {
                if(0 != rtl_memcmp(oilListInfo.portEgrIfList[i], zeroEgrIfList, sizeof(zeroEgrIfList)))
                {
                    oilListInfo.port = i;
                    /* Set OIL list */
                    CHK_FUN_CONTINUE(ret, cvb_drv_oil_list_set(unit, &rhTblParm, &oilListInfo, oilList));
                }
            }
            /* Set Rh table */
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_rhtbl_set(unit, mcEntry->rhTblIdx, &rhTblParm));
        }
        /* Filter mode */
        else
        {
            /* Set Rh entry */
            for(i = CVB_RTK_SYS_PORT_1ST; i <= CVB_RTK_SYS_PORT_END; i++)
            {
                if(0u != (((uint32)ENABLED << i) & sysDpm))
                {
                    rhTblParm.portRhIdx[i] = CVB_OIL_VALID_FLAG;
                }
            }
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_rhtbl_set(unit, mcEntry->rhTblIdx, &rhTblParm));
            /* Set dummy OIL */
            oilTableParm.ttlChk = mcEntry->ttl;
            oilTableParm.ttlDec = mcEntry->ttl;
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_oiltbl_set(unit, 0u, &oilTableParm));
        }
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_host_tbl_entry_set_nonfir_runtime(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet              ret            = RT_ERR_OK;
    cvb_RtHostTblMc4Parm_t hostTblParmMc4 = {0};
    cvb_RtHostTblMc6Parm_t hostTblParmMc6 = {0};

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_param_chk_runtime(mcEntry));
        /* Get avail rh index */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_rhtbl_avail_idx_get_runtime(unit, &mcEntry->rhTblIdx));
        mcEntry->rhAlloc = ENABLED;

        switch(mcEntry->luKey.entryType)
        {
            case EM_RT_MC4:
                /* Set Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4_parm_set_runtime(mcEntry, &hostTblParmMc4));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4_set(unit, mcEntry->tblInfo.index, &hostTblParmMc4, mcEntry->tblInfo.entryIdx));
                break;
            case EM_RT_MC6:
                /* Set Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6_parm_set_runtime(mcEntry, &hostTblParmMc6));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6_set(unit, mcEntry->tblInfo.index, &hostTblParmMc6, mcEntry->tblInfo.entryIdx));
                break;
            default:
                /* Do nothing */
                break;
        };

        /* Set Egress related info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_egr_info_set_runtime(unit, mcEntry));
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_host_tbl_entry_set_fir_runtime(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet                 ret               = RT_ERR_OK;
    cvb_RtHostTblMc4FirParm_t hostTblParmMc4Fir = {0};
    cvb_RtHostTblMc6FirParm_t hostTblParmMc6Fir = {0};
    cvb_RtHostGrpTblParm_t    hostGrpTblParm    = {0};

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_param_chk_runtime(mcEntry));
        /* Get avail rh index */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_rhtbl_avail_idx_get_runtime(unit, &mcEntry->rhTblIdx));
        mcEntry->rhAlloc = ENABLED;

        switch(mcEntry->luKey.entryType)
        {
            case EM_RT_MC4FIR:
                /* Set Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4fir_parm_set_runtime(mcEntry, &hostTblParmMc4Fir));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4fir_set(unit, mcEntry->tblInfo.index, &hostTblParmMc4Fir, mcEntry->tblInfo.entryIdx));
                break;
            case EM_RT_MC6FIR:
                /* Set Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6fir_parm_set_runtime(mcEntry, &hostTblParmMc6Fir));
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6fir_set(unit, mcEntry->tblInfo.index, &hostTblParmMc6Fir, mcEntry->tblInfo.entryIdx));
                break;
            default:
                /* Do nothing */
                break;
        };

        /* Set grp parm */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_hostgrp_parm_set_runtime(&mcEntry->luKey, &hostGrpTblParm));
        /* Set Grp tbl */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_grp_tbl_set(unit, mcEntry->luKey.entryType, mcEntry->tblInfo.index, &hostGrpTblParm, mcEntry->tblInfo.entryIdx));
        /* Set Egress related info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_egr_info_set_runtime(unit, mcEntry));
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_rt_mc_host_tbl_entry_runtime_set(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(mcEntry->luKey.entryType)
    {
        case EM_RT_MC4:
        case EM_RT_MC6:
            ret = cvb_drv_rt_mc_host_tbl_entry_set_nonfir_runtime(unit, mcEntry);
            break;
        case EM_RT_MC4FIR:
        case EM_RT_MC6FIR:
            ret = cvb_drv_rt_mc_host_tbl_entry_set_fir_runtime(unit, mcEntry);
            break;
        default:
            ret = -RT_ERR_INPUT;
            break;
    };

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mccol_nonfir_parm_set_runtime(RtMcEntry_t *mcEntry, cvb_RtMcColTblParm_t *mcColParm)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};
    uint8     ipLen;

    if((NULL == mcEntry) || (NULL == mcColParm))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        ipLen = (IP6_TYPE_BIT == ((uint8)mcEntry->luKey.entryType & IP6_TYPE_BIT)) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN;

        CHK_FUN_RET(ret, drv_endian_change(mcEntry->luKey.dipAddr, dip, ipLen));
        CHK_FUN_RET(ret, drv_endian_change(mcEntry->luKey.sipAddr, sip, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&mcColParm->ipv6Dip31to0, dip, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&mcColParm->ipv6Sip31to0, sip, ipLen));
        mcColParm->rhIdx  = mcEntry->rhTblIdx;
        mcColParm->valid  = mcEntry->valid;
        mcColParm->type   = (uint8)mcEntry->luKey.entryType;
        mcColParm->ttlMin = 1u;
        mcColParm->mtuMax = CVB_RT_DFT_MTU_MAX_SIZE;
        CHK_FUN_RET(ret, cvb_drv_pmap_convert(mcEntry->dpm, &mcColParm->dpm, CVB_EM_USER_TO_SYS));
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mccol_fir_ip_set_fir(RtMcEntry_t *mcEntry, cvb_RtMcColTblParm_t *mcColParm)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};
    uint8     ipLen;

    if((NULL == mcEntry) || (NULL == mcColParm))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        ipLen = (IP6_TYPE_BIT == ((uint8)mcEntry->luKey.entryType & IP6_TYPE_BIT)) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN;

        /* Change to Little-endian */
        CHK_FUN_RET(ret, drv_endian_change(mcEntry->luKey.dipAddr, dip, ipLen));
        CHK_FUN_RET(ret, drv_endian_change(mcEntry->luKey.sipAddr, sip, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&mcColParm->ipv6Dip31to0, dip, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(&mcColParm->ipv6Sip31to0, sip, ipLen));
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mccol_fir_parm_set_fir_runtime(RtMcEntry_t *mcEntry, cvb_RtMcColTblParm_t *mcColParm)
{
    RtkApiRet ret = RT_ERR_OK;

    if((NULL == mcEntry) || (NULL == mcColParm))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        CHK_FUN_RET(ret, cvb_drv_rt_mccol_fir_ip_set_fir(mcEntry, mcColParm));

        if(CVB_RT_L4FMT_NOCARE != mcEntry->luKey.dpFmt)
        {
            mcColParm->dp = mcEntry->luKey.dp;
        }

        if(CVB_RT_L4FMT_NOCARE != mcEntry->luKey.spFmt)
        {
            mcColParm->sp = mcEntry->luKey.sp;
        }

        if(0u != mcEntry->luKey.vid)
        {
            mcColParm->vid = mcEntry->luKey.vid;
        }

        mcColParm->dpFmt    = mcEntry->luKey.dpFmt;
        mcColParm->spFmt    = mcEntry->luKey.spFmt;
        mcColParm->type     = (uint8)mcEntry->luKey.entryType;
        mcColParm->valid    = mcEntry->valid;
        mcColParm->rhIdx    = mcEntry->rhTblIdx;
        mcColParm->arbMode  = mcEntry->arbMode;
        mcColParm->arbValid = mcEntry->arbValid;
        mcColParm->ttlMin   = 1u;
        mcColParm->mtuMax   = CVB_RT_DFT_MTU_MAX_SIZE;
        CHK_FUN_RET(ret, cvb_drv_pmap_convert(mcEntry->dpm, &mcColParm->dpm, CVB_EM_USER_TO_SYS));
    }

    return ret;
}

RtkApiRet cvb_drv_rt_mc_col_tbl_entry_set_runtime(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet            ret       = RT_ERR_OK;
    cvb_RtMcColTblParm_t mcColParm = {0u};

    do
    {
        /* Mccol table doesn't support acl index combination */
        if(ENABLED == cvb_drv_rt_host_only_key_exist(mcEntry->luKey))
        {
            ret = -RT_ERR_ROUTE_NOT_SUPPORT;
            continue;
        }

        /* Check mc param */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_param_chk_runtime(mcEntry));
        /* Get avail rh index */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_rhtbl_avail_idx_get_runtime(unit, &mcEntry->rhTblIdx));
        mcEntry->rhAlloc = ENABLED;

        switch(mcEntry->luKey.entryType)
        {
            case EM_RT_MC4:
            case EM_RT_MC6:
                /* Set mccol param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_mccol_nonfir_parm_set_runtime(mcEntry, &mcColParm));
                break;
            case EM_RT_MC4FIR:
            case EM_RT_MC6FIR:
                /* Set mccol param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_mccol_fir_parm_set_fir_runtime(mcEntry, &mcColParm));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        };

        /* Set Egress related info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_egr_info_set_runtime(unit, mcEntry));
        /* Set mccol */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mccol_set(unit, mcEntry->tblInfo.index, &mcColParm));
    } while(false);

    return ret;
}

uint8 cvb_drv_rt_host_only_key_exist(RtHostlookUpKey_t hostLuKey)
{
    uint8 enable = DISABLED;

    if((0u != hostLuKey.aclBlkIdx0) || (0u != hostLuKey.aclBlkIdx1) || (0u != hostLuKey.aclBlkIdx4) || (0u != hostLuKey.aclBlkIdx5) || (0u != hostLuKey.aclBlkIdx8) || (0u != hostLuKey.aclBlkIdx9) || (0u != hostLuKey.aclBlkIdx12) || (0u != hostLuKey.aclBlkIdx13) || (0u != hostLuKey.dipGrp) || (0u != hostLuKey.sipGrp)
       || (0u != hostLuKey.dpGrp) || (0u != hostLuKey.spGrp))
    {
        enable = ENABLED;
    }

    return enable;
}

RtkApiRet cvb_drv_rt_intf_param_chk_runtime(const RtIntfCfg_t *intfCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == intfCfg)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(intfCfg->intfEntry.index >= CVB_RT_INTF_SIZE)
        {
            ret = -RT_ERR_ENTRY_INDEX;
            continue;
        }
        if(ENABLED == intfCfg->intfEntry.valid)
        {
            if(ENABLED != intfCfg->intfEntry.filter)
            {
                if((IP4_VER != intfCfg->intfEntry.ipVer) && (IP6_VER != intfCfg->intfEntry.ipVer))
                {
                    if(0u != intfCfg->intfEntry.ipVer)
                    {
                        ret = -RT_ERR_INPUT;
                        continue;
                    }
                }

                if((CVB_RTK_USER_PORT_1ST > intfCfg->intfEntry.portId) || (CVB_RTK_USER_PORT_END < intfCfg->intfEntry.portId))
                {
                    ret = -RT_ERR_PORT_ID;
                }
            }
        }
        else if(DISABLED != intfCfg->intfEntry.valid)
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
        else
        {
            /* Do nothing */
        }
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_hosttbl_clear(uint32 unit)
{
    RtkApiRet       ret   = RT_ERR_OK;
    uint32          index = 0u;
    cvb_RtHostTbl_t hostRouteTableEnrty;

    ret = drv_memset(&hostRouteTableEnrty, 0, sizeof(hostRouteTableEnrty));

    if(RT_ERR_OK == ret)
    {
        for(index = 0u; index < CVB_HOST_ROUTE_TABLE_MAX; index++)
        {
            ret = cvb_table_write(unit, CVB_HOST_ROUTE_TABLE, index, hostRouteTableEnrty.val);
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_tbl_ip_get(uint8 dip[IPV6_ADDR_LEN], uint32 *tblDip, uint8 sip[IPV6_ADDR_LEN], uint32 *tblSip, uint32 len)
{
    RtkApiRet ret = RT_ERR_OK;

    if((NULL != dip) && (NULL != tblDip))
    {
        CHK_FUN_RET(ret, drv_memcpy(dip, tblDip, len));
        CHK_FUN_RET(ret, drv_endian_change(dip, NULL, len));
    }

    if((NULL != sip) && (NULL != tblSip))
    {
        CHK_FUN_RET(ret, drv_memcpy(sip, tblSip, len));
        CHK_FUN_RET(ret, drv_endian_change(sip, NULL, len));
    }

    return ret;
}

DYNAMIC_SCOPE RtHostEntryType_e cvb_drv_rt_host_type_num_to_em(uint8 oriType)
{
    RtHostEntryType_e entryType = EM_RT_MAX;

    switch(oriType)
    {
        case(uint8)EM_RT_UC4:
            entryType = EM_RT_UC4;
            break;
        case(uint8)EM_RT_MC4:
            entryType = EM_RT_MC4;
            break;
        case(uint8)EM_RT_UC6:
            entryType = EM_RT_UC6;
            break;
        case(uint8)EM_RT_MC6:
            entryType = EM_RT_MC6;
            break;
        case(uint8)EM_RT_UC4FIR:
            entryType = EM_RT_UC4FIR;
            break;
        case(uint8)EM_RT_MC4FIR:
            entryType = EM_RT_MC4FIR;
            break;
        case(uint8)EM_RT_UC6FIR:
            entryType = EM_RT_UC6FIR;
            break;
        case(uint8)EM_RT_MC6FIR:
            entryType = EM_RT_MC6FIR;
            break;
        default:
            /* Do nothing */
            break;
    }

    return entryType;
}

DYNAMIC_SCOPE void cvb_drv_rt_change_endian(uint8 *src, uint32 len)
{
    uint32 right = 0u;
    uint32 left  = len - 1u;
    uint8  tmp;

    while(right < left)
    {
        tmp        = src[right];
        src[right] = src[left];
        src[left]  = tmp;

        right++;
        left--;
    }
}

DYNAMIC_SCOPE uint8 cvb_drv_quick_search_l4_fmt_get(uint8 spFmt, uint8 dpFmt)
{
    uint8 l4Fmt = 0;

    /* if entry's spFmt[1] / dpFmtp[1] exists 1, then HW will bypass comparing L4 format */
    if((0x2u != (spFmt & 0x2u)) && (0x2u != (dpFmt & 0x2u)))
    {
        /* Others, HW will compare spFmt */
        l4Fmt = spFmt & 0x1u;
    }

    return l4Fmt;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_lukey_assign(uint32 unit, const RtlookUpKey_t *luKey)
{
    RtkApiRet ret   = RT_ERR_OK;
    uint8     l4Fmt = 0;

    do
    {
        /* This part is for getting hash index */
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL1, (((uint32)luKey->sip[0]) | ((uint32)luKey->sip[1] << 8u) | ((uint32)luKey->sip[2] << 16u) | ((uint32)luKey->sip[3] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL2, (((uint32)luKey->sip[4]) | ((uint32)luKey->sip[5] << 8u) | ((uint32)luKey->sip[6] << 16u) | ((uint32)luKey->sip[7] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL3, (((uint32)luKey->sip[8]) | ((uint32)luKey->sip[9] << 8u) | ((uint32)luKey->sip[10] << 16u) | ((uint32)luKey->sip[11] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL4, (((uint32)luKey->sip[12]) | ((uint32)luKey->sip[13] << 8u) | ((uint32)luKey->sip[14] << 16u) | ((uint32)luKey->sip[15] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL5, (((uint32)luKey->dip[0]) | ((uint32)luKey->dip[1] << 8u) | ((uint32)luKey->dip[2] << 16u) | ((uint32)luKey->dip[3] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL6, (((uint32)luKey->dip[4]) | ((uint32)luKey->dip[5] << 8u) | ((uint32)luKey->dip[6] << 16u) | ((uint32)luKey->dip[7] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL7, (((uint32)luKey->dip[8]) | ((uint32)luKey->dip[9] << 8u) | ((uint32)luKey->dip[10] << 16u) | ((uint32)luKey->dip[11] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL8, (((uint32)luKey->dip[12]) | ((uint32)luKey->dip[13] << 8u) | ((uint32)luKey->dip[14] << 16u) | ((uint32)luKey->dip[15] << 24u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL9, ((uint32)luKey->sp | ((uint32)luKey->dp << 16u))));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL10, ((uint32)luKey->vid & CVB_VID_MASK)));
        /* This part is for looking up entry (only supports unicast) */
        l4Fmt = cvb_drv_quick_search_l4_fmt_get(luKey->spFmt, luKey->dpFmt);
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, CVB_L3_TABLE_SEARCH_CONTROL0, ((((uint32)luKey->entryType >> 1u) & 0x3u) << CVB_L3_TABLE_SEARCH_CONTROL0_TYPE_LSP) | (((uint32)l4Fmt << CVB_L3_TABLE_SEARCH_CONTROL0_PORT_FMT_LSP))));
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_hw_search_result_get(uint32 unit, cvb_RtHwSearchInfo_t *info)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_L3_TABLE_SEARCH_STATUS0, CVB_L3_TABLE_SEARCH_STATUS0_FOUND_LSP, CVB_L3_TABLE_SEARCH_STATUS0_FOUND_LEN, &info->found));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_L3_TABLE_SEARCH_STATUS0, CVB_L3_TABLE_SEARCH_STATUS0_HASH_IDX_TOP_LSP, CVB_L3_TABLE_SEARCH_STATUS0_HASH_IDX_TOP_LEN, &info->topIdx));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_L3_TABLE_SEARCH_STATUS0, CVB_L3_TABLE_SEARCH_STATUS0_HASH_IDX_BTM_LSP, CVB_L3_TABLE_SEARCH_STATUS0_HASH_IDX_BTM_LEN, &info->btmIdx));
        info->btmIdx += CVB_RT_HOST_BTM_IDX_OFFSET;
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_L3_TABLE_SEARCH_STATUS1, CVB_L3_TABLE_SEARCH_STATUS1_VALID_MASK_TOP_LSP, CVB_L3_TABLE_SEARCH_STATUS1_VALID_MASK_TOP_LEN, &info->topValidMsk));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_L3_TABLE_SEARCH_STATUS1, CVB_L3_TABLE_SEARCH_STATUS1_VALID_MASK_BTM_LSP, CVB_L3_TABLE_SEARCH_STATUS1_VALID_MASK_BTM_LEN, &info->btmValidMsk));

        if(DISABLED == info->found)
        {
            if((CVB_RT_SLOT_VLAID_MASK == info->topValidMsk) && (CVB_RT_SLOT_VLAID_MASK == info->btmValidMsk))
            {
                ret = -RT_ERR_TBL_FULL;
            }
            else
            {
                ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;
            }
        }
        else
        {
            ret = -RT_ERR_ROUTE_ENTRY_FOUND;
        }
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_hw_hash_ip_idx_get(uint32 unit, const RtlookUpKey_t *luKey, cvb_RtHwSearchInfo_t *info)
{
    bool          isIp4     = (2u > ((uint8)luKey->entryType % 4u));
    uint32        isBusy    = DISABLED;
    uint8         tries     = 0u;
    RtlookUpKey_t searchKey = {0};
    RtkApiRet     ret       = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, drv_memcpy(&searchKey, luKey, sizeof(RtlookUpKey_t)));
        cvb_drv_rt_change_endian(searchKey.dip, isIp4 ? IPV4_ADDR_LEN : IPV6_ADDR_LEN);
        cvb_drv_rt_change_endian(searchKey.sip, isIp4 ? IPV4_ADDR_LEN : IPV6_ADDR_LEN);

        /* Assign lookup key */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_lukey_assign(unit, &searchKey));
        /* Force btm index use alg 1 */
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_HOST_TBL_CTRL, CVB_L3_HOST_TBL_CTRL_UC_TBL1_ALG_LSP, ENABLED));
        /* Lookup */
        CHK_FUN_CONTINUE(ret, cvb_reg_bit_set(unit, CVB_L3_TABLE_SEARCH_CONTROL0, CVB_L3_TABLE_SEARCH_CONTROL0_L3_SEARCH_TRIGGER_LSP, ENABLED));

        do
        {
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(1u));
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, CVB_L3_TABLE_SEARCH_CONTROL0, CVB_L3_TABLE_SEARCH_CONTROL0_L3_SEARCH_TRIGGER_LSP, &isBusy));
            tries++;
        } while((ENABLED == isBusy) && (CVB_QUCIK_SEARCH_TIMEOUT > tries));

        if(DISABLED == isBusy)
        {
            ret = cvb_drv_rt_hw_search_result_get(unit, info);
        }
        else
        {
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
        }
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE void cvb_drv_rt_host_avail_idx_get(RtTableSearchInfo_t *searchInfo, const cvb_RtHwSearchInfo_t *hwInfo)
{
    uint8 top = 0u;
    uint8 btm = 0u;
    /* clang-format off */
    const uint8   typeNum[8] = {CVB_OCCUPIED_SLOT_NUM_UC4, CVB_OCCUPIED_SLOT_NUM_MC4, CVB_OCCUPIED_SLOT_NUM_UC6, CVB_OCCUPIED_SLOT_NUM_MC6,
                                CVB_OCCUPIED_SLOT_NUM_UC4FIR, CVB_OCCUPIED_SLOT_NUM_MC4FIR, CVB_OCCUPIED_SLOT_NUM_UC6FIR, CVB_OCCUPIED_SLOT_NUM_MC6FIR};
    /* clang-format on */

    /* Search top available slot */
    for(top = 0u; top < (CVB_RT_MAX_SLOT_NUM / typeNum[(uint8)searchInfo->luKey.entryType]); top++)
    {
        if(((hwInfo->topValidMsk >> (typeNum[(uint8)searchInfo->luKey.entryType] * top)) & ENABLED) == DISABLED)
        {
            break;
        }
    }

    /* Search btm available slot */
    for(btm = 0u; btm < (CVB_RT_MAX_SLOT_NUM / typeNum[(uint8)searchInfo->luKey.entryType]); btm++)
    {
        if(((hwInfo->btmValidMsk >> (typeNum[(uint8)searchInfo->luKey.entryType] * btm)) & ENABLED) == DISABLED)
        {
            break;
        }
    }

    /* Get available index & slot */
    searchInfo->availEntryIdx = (top > btm) ? btm : top;
    searchInfo->availIdx      = (uint16)((top > btm) ? hwInfo->btmIdx : (hwInfo->topIdx));
    searchInfo->availTblType  = EM_HOST_TBL;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_tbl_nonfir_get(uint32 unit, uint32 entryNum, uint32 slotNum, RtHostEntryType_e entryType, uint32 *hostTblEntry)
{
    RtkApiRet              ret             = RT_ERR_OK;
    cvb_RtHostTblUc4Parm_t hostTblUc4Entry = {0};
    cvb_RtHostTblMc4Parm_t hostTblMc4Entry = {0};
    cvb_RtHostTblUc6Parm_t hostTblUc6Entry = {0};
    cvb_RtHostTblMc6Parm_t hostTblMc6Entry = {0};

    if(hostTblEntry == NULL)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(entryType)
        {
            case EM_RT_UC4:
                ret = cvb_drv_rt_host_uc4_get(unit, entryNum, &hostTblUc4Entry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblUc4Entry, sizeof(hostTblUc4Entry)));
                break;
            case EM_RT_MC4:
                ret = cvb_drv_rt_host_mc4_get(unit, entryNum, &hostTblMc4Entry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblMc4Entry, sizeof(hostTblMc4Entry)));
                break;
            case EM_RT_UC6:
                ret = cvb_drv_rt_host_uc6_get(unit, entryNum, &hostTblUc6Entry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblUc6Entry, sizeof(hostTblUc6Entry)));
                break;
            case EM_RT_MC6:
                ret = cvb_drv_rt_host_mc6_get(unit, entryNum, &hostTblMc6Entry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblMc6Entry, sizeof(hostTblMc6Entry)));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_tbl_fir_get(uint32 unit, uint32 entryNum, uint32 slotNum, RtHostEntryType_e entryType, uint32 *hostTblEntry)
{
    RtkApiRet                 ret                = RT_ERR_OK;
    cvb_RtHostTblUc4FirParm_t hostTblUc4FirEntry = {0};
    cvb_RtHostTblMc4FirParm_t hostTblMc4FirEntry = {0};
    cvb_RtHostTblUc6FirParm_t hostTblUc6FirEntry = {0};
    cvb_RtHostTblMc6FirParm_t hostTblMc6FirEntry = {0};

    if(hostTblEntry == NULL)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(entryType)
        {
            case EM_RT_UC4FIR:
                ret = cvb_drv_rt_host_uc4fir_get(unit, entryNum, &hostTblUc4FirEntry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblUc4FirEntry, sizeof(hostTblUc4FirEntry)));
                break;
            case EM_RT_MC4FIR:
                ret = cvb_drv_rt_host_mc4fir_get(unit, entryNum, &hostTblMc4FirEntry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblMc4FirEntry, sizeof(hostTblMc4FirEntry)));
                break;
            case EM_RT_UC6FIR:
                ret = cvb_drv_rt_host_uc6fir_get(unit, entryNum, &hostTblUc6FirEntry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblUc6FirEntry, sizeof(hostTblUc6FirEntry)));
                break;
            case EM_RT_MC6FIR:
                ret = cvb_drv_rt_host_mc6fir_get(unit, entryNum, &hostTblMc6FirEntry, slotNum);
                CHK_FUN_RET(ret, drv_memcpy(hostTblEntry, &hostTblMc6FirEntry, sizeof(hostTblMc6FirEntry)));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_tbl_get(uint32 unit, uint32 entryNum, uint32 slotNum, RtHostEntryType_e entryType, uint32 *hostTblEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(hostTblEntry == NULL)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(entryType)
        {
            case EM_RT_UC4:
            case EM_RT_MC4:
            case EM_RT_UC6:
            case EM_RT_MC6:
                ret = cvb_drv_rt_host_tbl_nonfir_get(unit, entryNum, slotNum, entryType, hostTblEntry);
                break;
            case EM_RT_UC4FIR:
            case EM_RT_MC4FIR:
            case EM_RT_UC6FIR:
            case EM_RT_MC6FIR:
                ret = cvb_drv_rt_host_tbl_fir_get(unit, entryNum, slotNum, entryType, hostTblEntry);
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_get_host_tbl_uc_nonfir_lookup_key(uint32 entryType, const uint32 *hostEntry, RtlookUpKey_t *tblKey)
{
    RtkApiRet              ret          = RT_ERR_OK;
    cvb_RtHostTblUc4Parm_t uc4HostEntry = {0};
    cvb_RtHostTblUc6Parm_t uc6HostEntry = {0};

    if((NULL == hostEntry) || (NULL == tblKey))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(entryType)
        {
            case(uint32)EM_RT_UC4:
                CHK_FUN_RET(ret, drv_memcpy(&uc4HostEntry, hostEntry, sizeof(uc4HostEntry)));
                CHK_FUN_RET(ret, drv_memcpy(tblKey->dip, &uc4HostEntry.ipv4Dip, IPV4_ADDR_LEN));
                cvb_drv_rt_change_endian(tblKey->dip, IPV4_ADDR_LEN);
                break;
            case(uint32)EM_RT_UC6:
                CHK_FUN_RET(ret, drv_memcpy(&uc6HostEntry, hostEntry, sizeof(uc6HostEntry)));
                CHK_FUN_RET(ret, drv_memcpy(tblKey->dip, &uc6HostEntry.ipv6Dip31to0, IPV6_ADDR_LEN));
                cvb_drv_rt_change_endian(tblKey->dip, IPV6_ADDR_LEN);
                break;
            default:
                /* Do nothing */
                break;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_get_host_tbl_mc_nonfir_lookup_key(uint32 entryType, const uint32 *hostEntry, RtlookUpKey_t *tblKey)
{
    RtkApiRet              ret          = RT_ERR_OK;
    cvb_RtHostTblMc4Parm_t mc4HostEntry = {0};
    cvb_RtHostTblMc6Parm_t mc6HostEntry = {0};

    if((NULL == hostEntry) || (NULL == tblKey))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(entryType)
        {
            case(uint32)EM_RT_MC4:
                CHK_FUN_RET(ret, drv_memcpy(&mc4HostEntry, hostEntry, sizeof(mc4HostEntry)));
                CHK_FUN_RET(ret, drv_memcpy(tblKey->dip, &mc4HostEntry.ipv4Dip, IPV4_ADDR_LEN));
                cvb_drv_rt_change_endian(tblKey->dip, IPV4_ADDR_LEN);
                CHK_FUN_RET(ret, drv_memcpy(tblKey->sip, &mc4HostEntry.ipv4Sip, IPV4_ADDR_LEN));
                cvb_drv_rt_change_endian(tblKey->sip, IPV4_ADDR_LEN);
                break;
            case(uint32)EM_RT_MC6:
                CHK_FUN_RET(ret, drv_memcpy(&mc6HostEntry, hostEntry, sizeof(mc6HostEntry)));
                CHK_FUN_RET(ret, drv_memcpy(tblKey->dip, &mc6HostEntry.ipv6Dip31to0, IPV6_ADDR_LEN));
                cvb_drv_rt_change_endian(tblKey->dip, IPV6_ADDR_LEN);
                CHK_FUN_RET(ret, drv_memcpy(tblKey->sip, &mc6HostEntry.ipv6Sip31to0, IPV6_ADDR_LEN));
                cvb_drv_rt_change_endian(tblKey->sip, IPV6_ADDR_LEN);
                break;
            default:
                /* Do nothing */
                break;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_get_host_tbl_nonfir_lookup_key(uint32 entryType, const uint32 *hostEntry, RtlookUpKey_t *tblKey)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(entryType)
    {
        case(uint32)EM_RT_UC4:
        case(uint32)EM_RT_UC6:
            ret = cvb_drv_rt_get_host_tbl_uc_nonfir_lookup_key(entryType, hostEntry, tblKey);
            break;
        case(uint32)EM_RT_MC4:
        case(uint32)EM_RT_MC6:
            ret = cvb_drv_rt_get_host_tbl_mc_nonfir_lookup_key(entryType, hostEntry, tblKey);
            break;
        default:
            /* Do nothing */
            break;
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_get_host_tbl_ucfir_lookup_key(uint32 entryType, const uint32 *hostEntry, const cvb_RtHostGrpTblParm_t *hostGrpEntry, RtlookUpKey_t *tblKey)
{
    RtkApiRet                 ret             = RT_ERR_OK;
    cvb_RtHostTblUc4FirParm_t uc4FirHostEntry = {0};
    cvb_RtHostTblUc6FirParm_t uc6FirHostEntry = {0};

    if((NULL == hostEntry) || (NULL == tblKey))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(entryType)
        {
            case(uint32)EM_RT_UC4FIR:
                CHK_FUN_RET(ret, drv_memcpy(&uc4FirHostEntry, hostEntry, sizeof(cvb_RtHostTblUc4FirParm_t)));
                CHK_FUN_RET(ret, cvb_drv_rt_tbl_ip_get(tblKey->dip, &uc4FirHostEntry.ipv4Dip, tblKey->sip, &uc4FirHostEntry.ipv4Sip, IPV4_ADDR_LEN));
                tblKey->dp = uc4FirHostEntry.dp;
                tblKey->sp = uc4FirHostEntry.sp;
                if(NULL != hostGrpEntry)
                {
                    tblKey->vid = hostGrpEntry->vid;
                }
                break;
            case(uint32)EM_RT_UC6FIR:
                CHK_FUN_RET(ret, drv_memcpy(&uc6FirHostEntry, hostEntry, sizeof(cvb_RtHostTblUc6FirParm_t)));
                CHK_FUN_RET(ret, cvb_drv_rt_tbl_ip_get(tblKey->dip, &uc6FirHostEntry.ipv6Dip31to0, tblKey->sip, &uc6FirHostEntry.ipv6Sip31to0, IPV6_ADDR_LEN));
                tblKey->dp = uc6FirHostEntry.dp;
                tblKey->sp = uc6FirHostEntry.sp;
                if(NULL != hostGrpEntry)
                {
                    tblKey->vid = hostGrpEntry->vid;
                }
                break;
            default:
                /* Do nothing */
                break;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_get_host_tbl_mcfir_lookup_key(uint32 entryType, const uint32 *hostEntry, const cvb_RtHostGrpTblParm_t *hostGrpEntry, RtlookUpKey_t *tblKey)
{
    RtkApiRet                 ret             = RT_ERR_OK;
    cvb_RtHostTblMc4FirParm_t mc4FirHostEntry = {0};
    cvb_RtHostTblMc6FirParm_t mc6FirHostEntry = {0};

    if((NULL == hostEntry) || (NULL == tblKey))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(entryType)
        {
            case(uint32)EM_RT_MC4FIR:
                CHK_FUN_RET(ret, drv_memcpy(&mc4FirHostEntry, hostEntry, sizeof(cvb_RtHostTblMc4FirParm_t)));
                CHK_FUN_RET(ret, cvb_drv_rt_tbl_ip_get(tblKey->dip, &mc4FirHostEntry.ipv4Dip, tblKey->sip, &mc4FirHostEntry.ipv4Sip, IPV4_ADDR_LEN));
                tblKey->dp = mc4FirHostEntry.dp;
                tblKey->sp = mc4FirHostEntry.sp;
                if(NULL != hostGrpEntry)
                {
                    tblKey->vid = hostGrpEntry->vid;
                }
                break;
            case(uint32)EM_RT_MC6FIR:
                CHK_FUN_RET(ret, drv_memcpy(&mc6FirHostEntry, hostEntry, sizeof(cvb_RtHostTblMc6FirParm_t)));
                CHK_FUN_RET(ret, cvb_drv_rt_tbl_ip_get(tblKey->dip, &mc6FirHostEntry.ipv6Dip31to0, tblKey->sip, &mc6FirHostEntry.ipv6Sip31to0, IPV6_ADDR_LEN));
                tblKey->dp = mc6FirHostEntry.dp;
                tblKey->sp = mc6FirHostEntry.sp;
                if(NULL != hostGrpEntry)
                {
                    tblKey->vid = hostGrpEntry->vid;
                }
                break;
            default:
                /* Do nothing */
                break;
        }
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_get_host_tbl_fir_lookup_key(uint32 entryType, const uint32 *hostEntry, const cvb_RtHostGrpTblParm_t *hostGrpEntry, RtlookUpKey_t *tblKey)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(entryType)
    {
        case(uint32)EM_RT_UC4FIR:
        case(uint32)EM_RT_UC6FIR:
            ret = cvb_drv_rt_get_host_tbl_ucfir_lookup_key(entryType, hostEntry, hostGrpEntry, tblKey);
            break;
        case(uint32)EM_RT_MC4FIR:
        case(uint32)EM_RT_MC6FIR:
            ret = cvb_drv_rt_get_host_tbl_mcfir_lookup_key(entryType, hostEntry, hostGrpEntry, tblKey);
            break;
        default:
            /* Do nothing */
            break;
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_get_host_tbl_lookup_key(uint32 entryType, const uint32 *hostEntry, const cvb_RtHostGrpTblParm_t *hostGrpEntry, RtlookUpKey_t *tblKey)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(entryType)
    {
        case(uint32)EM_RT_UC4:
        case(uint32)EM_RT_UC6:
        case(uint32)EM_RT_MC4:
        case(uint32)EM_RT_MC6:
            ret = cvb_drv_rt_get_host_tbl_nonfir_lookup_key(entryType, hostEntry, tblKey);
            break;
        case(uint32)EM_RT_UC4FIR:
        case(uint32)EM_RT_MC4FIR:
        case(uint32)EM_RT_UC6FIR:
        case(uint32)EM_RT_MC6FIR:
            ret = cvb_drv_rt_get_host_tbl_fir_lookup_key(entryType, hostEntry, hostGrpEntry, tblKey);
            break;
        default:
            /* Do nothing */
            break;
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_lookup_key_cmp(const RtlookUpKey_t *lookupKey, const RtlookUpKey_t *tblKey)
{
    RtkApiRet ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;

    switch(lookupKey->entryType)
    {
        case EM_RT_UC4:
        case EM_RT_UC6:
            if(0 == rtl_memcmp(tblKey->dip, lookupKey->dip, IPV6_ADDR_LEN))
            {
                ret = -RT_ERR_ROUTE_ENTRY_FOUND;
            }
            break;
        case EM_RT_MC4:
        case EM_RT_MC6:
            if((0 == rtl_memcmp(tblKey->dip, lookupKey->dip, IPV6_ADDR_LEN)) && (0 == rtl_memcmp(tblKey->sip, lookupKey->sip, IPV6_ADDR_LEN)))
            {
                ret = -RT_ERR_ROUTE_ENTRY_FOUND;
            }
            break;
        case EM_RT_UC4FIR:
        case EM_RT_MC4FIR:
        case EM_RT_UC6FIR:
        case EM_RT_MC6FIR:
            if((0 == rtl_memcmp(tblKey->dip, lookupKey->dip, sizeof(IPV6_ADDR_LEN))) && (0 == rtl_memcmp(tblKey->sip, lookupKey->sip, sizeof(IPV6_ADDR_LEN)))
               && (tblKey->dp == lookupKey->dp) && (tblKey->sp == lookupKey->sp) && (tblKey->vid == lookupKey->vid))
            {
                ret = -RT_ERR_ROUTE_ENTRY_FOUND;
            }
            break;
        default:
            ret = -RT_ERR_INPUT;
            break;
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_entry_cmp_and_get(uint32 unit, RtTableSearchInfo_t *searchInfo, uint32 *hostEntry, cvb_RtHostGrpTblParm_t *hostGrpEntry, uint32 idx)
{
    RtkApiRet     ret    = RT_ERR_OK;
    RtlookUpKey_t tblKey = {0};
    bool          isValid;
    uint32        type;
    uint8         slot;
    uint8         maxSlotNum = 0;

    cvb_drv_rt_max_host_entry_idx_num_get(searchInfo->luKey.entryType, &maxSlotNum);

    for(slot = 0u; slot < maxSlotNum; slot++)
    {
        /* Get host entry */
        ret = cvb_drv_rt_host_tbl_get(unit, idx, slot, searchInfo->luKey.entryType, hostEntry);
        /* Get host grp entry */
        if(EM_RT_UC4FIR <= searchInfo->luKey.entryType)
        {
            CHK_FUN_RET(ret, cvb_drv_rt_host_grp_tbl_get(unit, searchInfo->luKey.entryType, idx, hostGrpEntry, slot));
        }

        /* Get entry type and valid */
        isValid = (CVB_HOST_ENTRY_VALID_MASK == (((uint8 *)hostEntry)[0] & CVB_HOST_ENTRY_VALID_MASK));
        type    = (uint32)((uint8 *)hostEntry)[1] & CVB_HOST_ENTRY_TYPE_MASK;

        if(isValid && (type == (uint32)searchInfo->luKey.entryType) && (RT_ERR_OK == ret))
        {
            /* Get tbl lookup key */
            CHK_FUN_RET(ret, cvb_drv_rt_get_host_tbl_lookup_key(type, hostEntry, hostGrpEntry, &tblKey));
            /* Compare lookup key */
            CHK_FUN_RET(ret, cvb_drv_rt_lookup_key_cmp(&tblKey, &searchInfo->luKey));

            if(-RT_ERR_ROUTE_ENTRY_FOUND == ret)
            {
                searchInfo->hitIdx      = (uint16)idx;
                searchInfo->hitEntryIdx = slot;
                searchInfo->hitTblType  = EM_HOST_TBL;
                break;
            }
        }
    }

    if(RT_ERR_OK == ret)
    {
        ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_lookup_result_get(bool hit, bool availFound, uint16 index, cvb_table_list_t tblType)
{
    RtkApiRet ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;

    if(hit)
    {
        ret = -RT_ERR_ROUTE_ENTRY_FOUND;
    }
    else if(availFound)
    {
        ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;
    }
    else if(cvb_table_list[tblType].size == index)
    {
        ret = -RT_ERR_TBL_FULL;
    }
    else
    {
        /* Do nothing */
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_tbl_lukey_get(RtlookUpKey_t *tblKey, cvb_RtNetRouteTblParm_t *netTblParm, uint32 ipLen)
{
    RtkApiRet ret = RT_ERR_OK;

    if((NULL == tblKey) || (NULL == netTblParm))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {

        CHK_FUN_RET(ret, drv_memcpy(tblKey->sip, &netTblParm->ipv6Sip31to0, ipLen));
        CHK_FUN_RET(ret, drv_endian_change(tblKey->sip, NULL, ipLen));
        CHK_FUN_RET(ret, drv_memcpy(tblKey->dip, &netTblParm->ipv6Dip31to0, ipLen));
        CHK_FUN_RET(ret, drv_endian_change(tblKey->dip, NULL, ipLen));
        tblKey->sp  = netTblParm->sp;
        tblKey->dp  = netTblParm->dp;
        tblKey->vid = netTblParm->vid;
    }

    return ret;
}

RtkApiRet cvb_drv_rt_net_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo)
{
    RtkApiRet               ret        = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;
    uint32                  ipLen      = (2u > ((uint8)searchInfo->luKey.entryType % 4u)) ? IPV4_ADDR_LEN : IPV6_ADDR_LEN;
    bool                    hit        = (RT_TBL_INVALID_IDX == searchInfo->hitIdx) ? false : true;
    bool                    availFound = (RT_TBL_INVALID_IDX == searchInfo->availIdx) ? false : true;
    uint16                  index;
    cvb_RtNetRouteTblParm_t netTblParm;
    RtlookUpKey_t           tblKey;

    for(index = 0u; index < cvb_table_list[CVB_NETWORK_ROUTE_TABLE].size; index++)
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&netTblParm, 0, sizeof(netTblParm)));
        CHK_FUN_CONTINUE(ret, drv_memset(&tblKey, 0, sizeof(tblKey)));
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nettbl_get(unit, (uint16)index, &netTblParm));
        if(ENABLED == netTblParm.valid)
        {
            if(!hit)
            {
                /* Get tbl lookup key */
                CHK_FUN_CONTINUE(ret, cvb_drv_tbl_lukey_get(&tblKey, &netTblParm, ipLen));
                /* Compare lookup key */
                ret = cvb_drv_rt_lookup_key_cmp(&tblKey, &searchInfo->luKey);

                if(-RT_ERR_ROUTE_ENTRY_FOUND == ret)
                {
                    searchInfo->hitIdx     = index;
                    searchInfo->hitTblType = EM_NET_TBL;
                    hit                    = true;
                }
            }
        }
        else if(!availFound)
        {
            searchInfo->availIdx     = index;
            searchInfo->availTblType = EM_NET_TBL;
            availFound               = true;
        }
        else
        {
            /* Keep lookup, do nothing */
        }

        if(hit && availFound)
        {
            break;
        }
    }

    return cvb_drv_rt_lookup_result_get(hit, availFound, index, CVB_NETWORK_ROUTE_TABLE);
}

RtkApiRet cvb_drv_rt_mccol_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo)
{
    RtkApiRet            ret = -RT_ERR_ROUTE_ENTRY_NOT_FOUND;
    uint16               index;
    cvb_RtMcColTblParm_t mcColTblParm;
    RtlookUpKey_t        tblKey;
    uint32               ipLen      = (2u > ((uint8)searchInfo->luKey.entryType % 4u)) ? IPV4_ADDR_LEN : IPV6_ADDR_LEN;
    bool                 hit        = (RT_TBL_INVALID_IDX == searchInfo->hitIdx) ? false : true;
    bool                 availFound = (RT_TBL_INVALID_IDX == searchInfo->availIdx) ? false : true;

    for(index = 0u; index < cvb_table_list[CVB_MULTICAST_COL_TABLE].size; index++)
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&mcColTblParm, 0, sizeof(mcColTblParm)));
        CHK_FUN_CONTINUE(ret, drv_memset(&tblKey, 0, sizeof(tblKey)));
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mccol_get(unit, (uint32)index, &mcColTblParm));
        if(ENABLED == mcColTblParm.valid)
        {
            /* Get tbl lookup key */
            CHK_FUN_CONTINUE(ret, drv_memcpy(tblKey.sip, &mcColTblParm.ipv6Sip31to0, ipLen));
            cvb_drv_rt_change_endian(tblKey.sip, ipLen);
            CHK_FUN_CONTINUE(ret, drv_memcpy(tblKey.dip, &mcColTblParm.ipv6Dip31to0, ipLen));
            cvb_drv_rt_change_endian(tblKey.dip, ipLen);
            tblKey.sp  = mcColTblParm.sp;
            tblKey.dp  = mcColTblParm.dp;
            tblKey.vid = mcColTblParm.vid;

            /* Compare lookup key */
            ret = cvb_drv_rt_lookup_key_cmp(&tblKey, &searchInfo->luKey);

            if(-RT_ERR_ROUTE_ENTRY_FOUND == ret)
            {
                searchInfo->hitIdx     = index;
                searchInfo->hitTblType = EM_MCCOL_TBL;
                hit                    = true;
            }
        }
        else if(!availFound)
        {

            searchInfo->availIdx     = index;
            searchInfo->availTblType = EM_MCCOL_TBL;
            availFound               = true;
        }
        else
        {
            /* Keep lookup, do nothing */
        }

        if(hit && availFound)
        {
            break;
        }
    }

    return cvb_drv_rt_lookup_result_get(hit, availFound, index, CVB_MULTICAST_COL_TABLE);
}

RtkApiRet cvb_drv_rt_host_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo)
{
    RtkApiRet              ret          = RT_ERR_OK;
    cvb_RtHwSearchInfo_t   hwInfo       = {0};
    cvb_RtHostTbl_t        tableEntry   = {0};
    cvb_RtHostGrpTblParm_t hostGrpEntry = {0};
    uint8                  maxSlotNum   = 0u;
    bool                   isUc         = false;

    do
    {
        if(NULL == searchInfo)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Initialize table index */
        searchInfo->hitTblType    = EM_TBL_MAX;
        searchInfo->hitIdx        = RT_TBL_INVALID_IDX;
        searchInfo->hitEntryIdx   = RT_HOST_ENTRY_INVALID_IDX;
        searchInfo->availTblType  = EM_TBL_MAX;
        searchInfo->availIdx      = RT_TBL_INVALID_IDX;
        searchInfo->availEntryIdx = RT_HOST_ENTRY_INVALID_IDX;

        isUc = (0u == ((uint32)searchInfo->luKey.entryType % 2u));

        /* Host table quick search */
        ret = cvb_drv_rt_hw_hash_ip_idx_get(unit, &searchInfo->luKey, &hwInfo);

        if((-RT_ERR_ROUTE_ENTRY_FOUND == ret) || !isUc)
        {
            /* Lookup top entry */
            ret = cvb_drv_rt_host_entry_cmp_and_get(unit, searchInfo, tableEntry.val, &hostGrpEntry, hwInfo.topIdx);

            if(-RT_ERR_ROUTE_ENTRY_NOT_FOUND == ret)
            {
                /* Lookup btm entry */
                ret = cvb_drv_rt_host_entry_cmp_and_get(unit, searchInfo, tableEntry.val, &hostGrpEntry, hwInfo.btmIdx);
            }
        }

        if((-RT_ERR_ROUTE_ENTRY_FOUND == ret) || (-RT_ERR_ROUTE_ENTRY_NOT_FOUND == ret))
        {
            /* Get available index & slot */
            cvb_drv_rt_host_avail_idx_get(searchInfo, &hwInfo);

            cvb_drv_rt_max_host_entry_idx_num_get(searchInfo->luKey.entryType, &maxSlotNum);
            if(searchInfo->availEntryIdx >= maxSlotNum)
            {
                searchInfo->availIdx      = RT_TBL_INVALID_IDX;
                searchInfo->availEntryIdx = RT_HOST_ENTRY_INVALID_IDX;
                ret                       = -RT_ERR_TBL_FULL;
            }
        }

    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_rttbl_param_chk(const RtTbl_t *rtEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == rtEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if((IP4_VER != rtEntry->ipVer) && (IP6_VER != rtEntry->ipVer))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_rttbl_sdk_to_fw(const RtTbl_t *rtEntry, cvb_RtFwRtTbl_t *fwRtEntry)
{
    RtkApiRet   ret                   = RT_ERR_OK;
    const uint8 zeroIp[IPV6_ADDR_LEN] = {0};
    uint8       i;

    CHK_FUN_RET(ret, drv_memcpy(fwRtEntry->destIpAddr, rtEntry->destIpAddr, IPV6_ADDR_LEN));
    /* Fill mask */
    for(i = 0u; i < rtEntry->subnetMask; i++)
    {
        fwRtEntry->subnetMask[i / 8u] |= (1u << (i % 8u));
    }
    if(0 != rtl_memcmp(zeroIp, rtEntry->nextHopIp, IPV6_ADDR_LEN))
    {
        fwRtEntry->nhIntfId = RT_INTF_TBL_MAX_SIZE;
    }
    else
    {
        fwRtEntry->nhIntfId = rtEntry->nhIntfId;
    }
    fwRtEntry->ipVer = rtEntry->ipVer;
    fwRtEntry->valid = rtEntry->valid;
    CHK_FUN_RET(ret, drv_memcpy(fwRtEntry->nextHopIp, rtEntry->nextHopIp, IPV6_ADDR_LEN));

    return ret;
}

RtkApiRet cvb_drv_rt_rttbl_fw_to_sdk(const cvb_RtFwRtTbl_t *fwRtEntry, RtTbl_t *rtEntry)
{
    RtkApiRet ret = RT_ERR_OK;
    uint8     i;
    uint8     maskNum                   = 0;
    uint8     subnetMask[IPV6_ADDR_LEN] = {0};

    CHK_FUN_RET(ret, drv_memcpy(rtEntry->destIpAddr, fwRtEntry->destIpAddr, IPV6_ADDR_LEN));
    CHK_FUN_RET(ret, drv_memcpy(subnetMask, fwRtEntry->subnetMask, IPV6_ADDR_LEN));
    /* Fill mask */
    for(i = 0u; i < IPV6_ADDR_LEN; i++)
    {
        while(0u != subnetMask[i])
        {
            if(1u == (subnetMask[i] & 1u))
            {
                maskNum++;
            }
            subnetMask[i] = subnetMask[i] >> 1u;
        }
    }
    rtEntry->subnetMask = maskNum;
    rtEntry->nhIntfId   = fwRtEntry->nhIntfId;
    rtEntry->ipVer      = fwRtEntry->ipVer;
    rtEntry->valid      = fwRtEntry->valid;
    CHK_FUN_RET(ret, drv_memcpy(rtEntry->nextHopIp, fwRtEntry->nextHopIp, IPV6_ADDR_LEN));

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_subnet_hw_parm_set(uint32 unit, RtTbl_t *rtEntry, cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    RtkApiRet ret                   = RT_ERR_OK;
    uint8     ip[IPV6_ADDR_LEN]     = {0u};
    uint8     ipCare[IPV6_ADDR_LEN] = {0u};
    uint8     ipLen                 = (rtEntry->ipVer == IP4_VER) ? IPV4_ADDR_LEN : IPV6_ADDR_LEN;
    uint8     i;

    do
    {
        CHK_FUN_CONTINUE(ret, drv_endian_change(rtEntry->destIpAddr, ip, ipLen));
        /* Fill mask */
        for(i = 0u; i < rtEntry->subnetMask; i++)
        {
            ipCare[i / 8u] |= (1u << (i % 8u));
        }
        CHK_FUN_CONTINUE(ret, drv_endian_change(ipCare, NULL, ipLen));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&netRouteTblParm->ipv6Dip31to0, ip, ipLen));
        CHK_FUN_CONTINUE(ret, drv_memcpy(&netRouteTblParm->ipv6Dip31to0Care, ipCare, ipLen));
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nhtbl_avail_get_runtime(unit, &netRouteTblParm->nhIdx));
        netRouteTblParm->valid    = ENABLED;
        netRouteTblParm->type     = (uint8)((rtEntry->ipVer == IP4_VER) ? EM_RT_UC4 : EM_RT_UC6);
        netRouteTblParm->typeCare = 0xFFu;
        netRouteTblParm->act      = CVB_ACT_TRAP;
        netRouteTblParm->qosEn    = ENABLED;
        netRouteTblParm->qosPri   = 0u;
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_fw_intf_param_get(RtIntfTbl_t *intfEntry, const cvb_RtFwIntfTbl_t *fwIntfEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((NULL == fwIntfEntry) || (NULL == intfEntry))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        intfEntry->intfFidVid = fwIntfEntry->intFidIvid;
        CHK_FUN_CONTINUE(ret, drv_memcpy(intfEntry->mac, fwIntfEntry->mac, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(intfEntry->gwIp, fwIntfEntry->gwIp, IPV6_ADDR_LEN));
        intfEntry->portId = fwIntfEntry->physicalPn;
        if(CVB_MTU_IDX_IPV6 == fwIntfEntry->mtuIdx)
        {
            intfEntry->ipVer = IP6_VER;
        }
        else if(CVB_MTU_IDX_IPV4 == fwIntfEntry->mtuIdx)
        {
            intfEntry->ipVer = IP4_VER;
        }
        else
        {
            intfEntry->ipVer = INVALID_VER;
        }
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_sw_intf_clear(uint32 unit)
{
    const uint32 base          = CVB_SW_INTF_TBL_BASE;
    const uint32 fwIntfTblSize = (uint32)sizeof(cvb_RtFwIntfTbl_t) * RT_INTF_TBL_MAX_SIZE;
    RtkApiRet    ret           = RT_ERR_OK;
    uint32       i;

    for(i = 0u; i < (fwIntfTblSize / 4u); i++)
    {
        CHK_FUN_BREAK(ret, cvb_drv_indirect_sram_set(unit, base + (i * 4u), 0u));
    }

    return ret;
}

RtkApiRet cvb_drv_rt_rttbl_clear(uint32 unit)
{
    const uint32 base      = CVB_SW_RT_TBL_BASE;
    const uint32 rtTblSize = (uint32)sizeof(cvb_RtFwRtTbl_t) * RT_NET_TBL_MAX_SIZE;
    RtkApiRet    ret       = RT_ERR_OK;
    uint32       i;

    for(i = 0u; i < (rtTblSize / 4u); i++)
    {
        CHK_FUN_RET(ret, cvb_drv_indirect_sram_set(unit, base + (i * 4u), 0u));
    }

    return ret;
}

RtkApiRet cvb_drv_rt_sw_intf_get(uint32 unit, RtIntfTbl_t *intfEntry)
{
    uint32            base                            = 0u;
    uint32            i                               = 0u;
    uint32            fwIntfEntrySize                 = (uint32)sizeof(cvb_RtFwIntfTbl_t);
    uint32            data[sizeof(cvb_RtFwIntfTbl_t)] = {0u};
    RtkApiRet         ret                             = RT_ERR_OK;
    cvb_RtFwIntfTbl_t fwIntfEntry;

    if(NULL == intfEntry)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else if(CVB_RT_INTF_SIZE <= intfEntry->index)
    {
        ret = -RT_ERR_ENTRY_INDEX;
    }
    else
    {
        do
        {
            /* Set SW interface table */
            base = CVB_SW_INTF_TBL_BASE + (fwIntfEntrySize * intfEntry->index);

            for(i = 0u; i < (fwIntfEntrySize / 4u); i++)
            {
                CHK_FUN_CONTINUE(ret, cvb_drv_indirect_sram_get(unit, base + (i * 4u), &data[i]));
            }

            CHK_FUN_CONTINUE(ret, drv_memcpy((uint8 *)&fwIntfEntry, (uint8 *)data, sizeof(cvb_RtFwIntfTbl_t)));
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_fw_intf_param_get(intfEntry, &fwIntfEntry));
        } while(1 == 0);
    }

    return ret;
}

RtkApiRet cvb_drv_rt_nhpool_clear(uint32 unit)
{
    const uint32 base       = CVB_NH_POOL_ADDR;
    const uint32 nhPoolSize = CVB_NEXT_HOP_TABLE_MAX / 32u;
    RtkApiRet    ret        = RT_ERR_OK;
    uint32       i;

    for(i = 0u; i < nhPoolSize; i++)
    {
        CHK_FUN_RET(ret, cvb_drv_indirect_sram_set(unit, base + (i * 4u), 0u));
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_gw_mac_set(uint32 unit, const RtTbl_t *rtEntry, uint16 *l2Idx)
{
    RtkApiRet          ret       = RT_ERR_OK;
    RtIntfTbl_t        intfEntry = {0};
    cvb_LutAsicParam_t camParam  = {0};

    if((NULL == rtEntry) || (NULL == l2Idx))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        do
        {
            intfEntry.index = rtEntry->nhIntfId;
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_sw_intf_get(unit, &intfEntry));

            *l2Idx               = ((uint16)CVB_LUTCAM_INDEX_END - 1u) - intfEntry.index;
            camParam.type        = CVB_EM_LUT_ASIC_UNICAST;
            camParam.uni.fid     = CVB_LUT_RESERVED_FID;
            camParam.uni.spa     = CVB_RTK_CPU_PORT;
            camParam.uni.bstatic = ENABLED;
            CHK_FUN_CONTINUE(ret, drv_memcpy(camParam.uni.mac, intfEntry.mac, MAC_ADDR_LEN));
            CHK_FUN_CONTINUE(ret, cvb_drv_lut_cam_entry_set(unit, *l2Idx, &camParam));

        } while(1u == 0u);
    }

    return ret;
}

RtkApiRet cvb_drv_rt_subnet_set(uint32 unit, RtTbl_t *rtEntry, uint32 index)
{
    RtkApiRet               ret             = RT_ERR_OK;
    cvb_RtNetRouteTblParm_t netRouteTblParm = {0};

    do
    {
        /* Set net tbl entry parm */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_subnet_hw_parm_set(unit, rtEntry, &netRouteTblParm));
        /* Set HW net tbl */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nettbl_set(unit, index, &netRouteTblParm));
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_host_raw_get(uint32 unit, uint16 entryNum, cvb_RtHostTbl_t *hostTblEntry)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(entryNum >= CVB_HOST_ROUTE_TABLE_MAX)
    {
        retVal = -RT_ERR_ENTRY_INDEX;
    }
    else if(hostTblEntry == NULL)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        retVal = drv_memset(hostTblEntry, 0, sizeof(cvb_RtHostTbl_t));
        CHK_FUN_RET(retVal, cvb_table_read(unit, CVB_HOST_ROUTE_TABLE, entryNum, hostTblEntry->val));
        cvb_drv_rt_tbl_indr_6_sft_restore(hostTblEntry->val, sizeof(hostTblEntry->val) / sizeof(hostTblEntry->val[0]));
    }

    return retVal;
}

RtkApiRet cvb_drv_host_entry_type_valid_get(const cvb_RtHostTbl_t *hostTblEntry, cvb_RtHostValidType_t *entryValidType)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    val    = 0u;
    uint32    offset = 0u;
    uint8     i;

    if((NULL == hostTblEntry) || (NULL == entryValidType))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        for(i = 0u; i < RT_HOST_ENTRY_MAX_SIZE; i++)
        {
            offset = (uint32)i * CVB_BITS_PER_DATA;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry->val[0], &val, 0u + offset, 0u + offset);
            entryValidType->valid[i] = (uint8)val;

            val = 0u;
            cvb_drv_rt_array_bit32_get(&hostTblEntry->val[0], &val, 1u + offset, 3u + offset);
            entryValidType->type[i] = cvb_drv_rt_host_type_num_to_em((uint8)val);
        }
    }

    return ret;
}

RtkApiRet cvb_drv_host_entry_typelist_get(const cvb_RtHostValidType_t *entryValidType, RtHostTypeList_t *hostTypeList)
{
    RtkApiRet ret          = RT_ERR_OK;
    uint8     i            = 0;
    uint8     j            = 0;
    uint8     slotOccupied = 0;

    if((NULL == hostTypeList) || (NULL == entryValidType))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        while(RT_HOST_ENTRY_MAX_SIZE > i)
        {
            if(DISABLED == entryValidType->valid[i])
            {
                i++;
            }
            else
            {
                slotOccupied = 0u;

                hostTypeList->typeList[j].entryType = entryValidType->type[i];
                hostTypeList->typeList[j].valid     = entryValidType->valid[i];
                cvb_drv_rt_occupied_slot_num_get(hostTypeList->typeList[j].entryType, &slotOccupied);
                if(0u != slotOccupied)
                {
                    hostTypeList->typeList[j].entryIdx = i / slotOccupied;
                    i += slotOccupied;
                    j++;
                }
            }
        }
    }

    return ret;
}

RtkApiRet cvb_drv_rt_hw_igr_intf_get(uint32 unit, RtIntfCfg_t *intfCfg)
{
    RtkApiRet            ret        = RT_ERR_OK;
    cvb_RtIngIfTblParm_t igrIfEntry = {0};

    ret                          = cvb_drv_rt_igriftbl_get(unit, intfCfg->intfEntry.index, &igrIfEntry);
    intfCfg->intfEntry.valid     = igrIfEntry.valid;
    intfCfg->intfEntry.firewall  = igrIfEntry.firMode;
    intfCfg->intfEntry.dpFmt     = igrIfEntry.dpFmt;
    intfCfg->intfEntry.spFmt     = igrIfEntry.spFmt;
    intfCfg->luKeyMode.dipNoCare = igrIfEntry.dipNoCare;
    intfCfg->luKeyMode.sipNoCare = igrIfEntry.sipNoCare;
    intfCfg->luKeyMode.vidNoCare = igrIfEntry.vidNoCare;
    intfCfg->luKeyMode.dpNoCare  = (RT_L4FMT_NOCARE == igrIfEntry.dpFmt) ? ENABLED : DISABLED;
    intfCfg->luKeyMode.spNoCare  = (RT_L4FMT_NOCARE == igrIfEntry.spFmt) ? ENABLED : DISABLED;

    return ret;
}

RtkApiRet cvb_drv_rt_hw_egr_intf_get(uint32 unit, RtIntfTbl_t *intfEntry)
{
    RtkApiRet           ret        = RT_ERR_OK;
    cvb_RtEgIfTblParm_t egrIfEntry = {0};

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_egriftbl_get(unit, intfEntry->index, &egrIfEntry));
        intfEntry->filter     = (DISABLED == egrIfEntry.macReplace) ? ENABLED : DISABLED;
        intfEntry->ipv4MtuIdx = egrIfEntry.ipMtuIdx;
        intfEntry->ipv6MtuIdx = egrIfEntry.ip6MtuIdx;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc4_parm_get(RtUcEntry_t *ucEntry, const cvb_RtHostTblUc4Parm_t *hostTblParmUc4)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == ucEntry) || (NULL == hostTblParmUc4))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmUc4->ipv4Dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, ucEntry->hostLuKey.dipAddr, IPV4_ADDR_LEN));
        ucEntry->hostLuKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmUc4->type);
        ucEntry->valid               = hostTblParmUc4->valid;
        ucEntry->ttl                 = hostTblParmUc4->ttlChk;
        ucEntry->nhIdx               = hostTblParmUc4->nhIdx;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc6_parm_get(RtUcEntry_t *ucEntry, const cvb_RtHostTblUc6Parm_t *hostTblParmUc6)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == ucEntry) || (NULL == hostTblParmUc6))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmUc6->ipv6Dip31to0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, ucEntry->hostLuKey.dipAddr, IPV6_ADDR_LEN));
        ucEntry->hostLuKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmUc6->type);
        ucEntry->ttl                 = hostTblParmUc6->ttlChk;
        ucEntry->valid               = hostTblParmUc6->valid;
        ucEntry->nhIdx               = hostTblParmUc6->nhIdx;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc4fir_parm_get(RtUcEntry_t *ucEntry, const cvb_RtHostTblUc4FirParm_t *hostTblParmUc4Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == ucEntry) || (NULL == hostTblParmUc4Fir))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmUc4Fir->ipv4Dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &hostTblParmUc4Fir->ipv4Sip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, ucEntry->hostLuKey.dipAddr, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, ucEntry->hostLuKey.sipAddr, IPV4_ADDR_LEN));
        ucEntry->hostLuKey.dp        = hostTblParmUc4Fir->dp;
        ucEntry->hostLuKey.sp        = hostTblParmUc4Fir->sp;
        ucEntry->hostLuKey.spFmt     = hostTblParmUc4Fir->spFmt;
        ucEntry->hostLuKey.dpFmt     = hostTblParmUc4Fir->dpFmt;
        ucEntry->hostLuKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmUc4Fir->type);
        ucEntry->valid               = hostTblParmUc4Fir->valid;
        ucEntry->ttl                 = hostTblParmUc4Fir->ttlChk;
        ucEntry->nhIdx               = hostTblParmUc4Fir->nhIdx;
        ucEntry->arbValid            = hostTblParmUc4Fir->arbValid;
        ucEntry->arbMode             = hostTblParmUc4Fir->arbMode;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_uc6fir_parm_get(RtUcEntry_t *ucEntry, const cvb_RtHostTblUc6FirParm_t *hostTblParmUc6Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == ucEntry) || (NULL == hostTblParmUc6Fir))
        {
            ret = -RT_ERR_NULL_POINTER;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmUc6Fir->ipv6Dip31to0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &hostTblParmUc6Fir->ipv6Sip31to0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, ucEntry->hostLuKey.dipAddr, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, ucEntry->hostLuKey.sipAddr, IPV6_ADDR_LEN));
        ucEntry->hostLuKey.dp        = hostTblParmUc6Fir->dp;
        ucEntry->hostLuKey.sp        = hostTblParmUc6Fir->sp;
        ucEntry->hostLuKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmUc6Fir->type);
        ucEntry->valid               = hostTblParmUc6Fir->valid;
        ucEntry->ttl                 = hostTblParmUc6Fir->ttlChk;
        ucEntry->nhIdx               = hostTblParmUc6Fir->nhIdx;
        ucEntry->hostLuKey.spFmt     = hostTblParmUc6Fir->spFmt;
        ucEntry->hostLuKey.dpFmt     = hostTblParmUc6Fir->dpFmt;
        ucEntry->arbValid            = hostTblParmUc6Fir->arbValid;
        ucEntry->arbMode             = hostTblParmUc6Fir->arbMode;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc4_parm_get(RtMcEntry_t *mcEntry, cvb_RtHostTblMc4Parm_t *hostTblParmMc4)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc4))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmMc4->ipv4Dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &hostTblParmMc4->ipv4Sip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, mcEntry->luKey.dipAddr, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, mcEntry->luKey.sipAddr, IPV4_ADDR_LEN));
        /* To avoid getting unexpected value of invalid entry */
        hostTblParmMc4->dpm &= (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP);
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(hostTblParmMc4->dpm, &mcEntry->dpm, CVB_EM_SYS_TO_USER));
        mcEntry->luKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmMc4->type);
        mcEntry->valid           = hostTblParmMc4->valid;
        mcEntry->rhTblIdx        = hostTblParmMc4->rhIdx;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc6_parm_get(RtMcEntry_t *mcEntry, cvb_RtHostTblMc6Parm_t *hostTblParmMc6)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc6))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmMc6->ipv6Dip31to0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &hostTblParmMc6->ipv6Sip31to0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, mcEntry->luKey.dipAddr, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, mcEntry->luKey.sipAddr, IPV6_ADDR_LEN));
        /* To avoid getting unexpected value of invalid entry */
        hostTblParmMc6->dpm &= (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP);
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(hostTblParmMc6->dpm, &mcEntry->dpm, CVB_EM_SYS_TO_USER));
        mcEntry->luKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmMc6->type);
        mcEntry->valid           = hostTblParmMc6->valid;
        mcEntry->rhTblIdx        = hostTblParmMc6->rhIdx;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc4fir_parm_get(RtMcEntry_t *mcEntry, cvb_RtHostTblMc4FirParm_t *hostTblParmMc4Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc4Fir))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmMc4Fir->ipv4Dip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &hostTblParmMc4Fir->ipv4Sip, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, mcEntry->luKey.dipAddr, IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, mcEntry->luKey.sipAddr, IPV4_ADDR_LEN));
        /* To avoid getting unexpected value of invalid entry */
        hostTblParmMc4Fir->dpm &= (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP);
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(hostTblParmMc4Fir->dpm, &mcEntry->dpm, CVB_EM_SYS_TO_USER));
        mcEntry->luKey.dp        = hostTblParmMc4Fir->dp;
        mcEntry->luKey.sp        = hostTblParmMc4Fir->sp;
        mcEntry->luKey.spFmt     = hostTblParmMc4Fir->spFmt;
        mcEntry->luKey.dpFmt     = hostTblParmMc4Fir->dpFmt;
        mcEntry->luKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmMc4Fir->type);
        mcEntry->valid           = hostTblParmMc4Fir->valid;
        mcEntry->rhTblIdx        = hostTblParmMc4Fir->rhIdx;
        mcEntry->arbValid        = hostTblParmMc4Fir->arbValid;
        mcEntry->arbMode         = hostTblParmMc4Fir->arbMode;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_host_mc6fir_parm_get(RtMcEntry_t *mcEntry, cvb_RtHostTblMc6FirParm_t *hostTblParmMc6Fir)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};

    do
    {
        if((NULL == mcEntry) || (NULL == hostTblParmMc6Fir))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &hostTblParmMc6Fir->ipv6Dip31to0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &hostTblParmMc6Fir->ipv6Sip31to0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, mcEntry->luKey.dipAddr, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, mcEntry->luKey.sipAddr, IPV6_ADDR_LEN));
        /* To avoid getting unexpected value of invalid entry */
        hostTblParmMc6Fir->dpm &= (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP);
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(hostTblParmMc6Fir->dpm, &mcEntry->dpm, CVB_EM_SYS_TO_USER));
        mcEntry->luKey.dp        = hostTblParmMc6Fir->dp;
        mcEntry->luKey.sp        = hostTblParmMc6Fir->sp;
        mcEntry->luKey.spFmt     = hostTblParmMc6Fir->spFmt;
        mcEntry->luKey.dpFmt     = hostTblParmMc6Fir->dpFmt;
        mcEntry->luKey.entryType = cvb_drv_rt_host_type_num_to_em(hostTblParmMc6Fir->type);
        mcEntry->valid           = hostTblParmMc6Fir->valid;
        mcEntry->rhTblIdx        = hostTblParmMc6Fir->rhIdx;
        mcEntry->arbValid        = hostTblParmMc6Fir->arbValid;
        mcEntry->arbMode         = hostTblParmMc6Fir->arbMode;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_hostgrp_parm_get(RtHostlookUpKey_t *luKey, const cvb_RtHostGrpTblParm_t *hostGrpTblParm)
{
    RtkApiRet ret = RT_ERR_OK;

    if((NULL == luKey) || (NULL == hostGrpTblParm))
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        luKey->vid         = hostGrpTblParm->vid;
        luKey->aclBlkIdx0  = hostGrpTblParm->aclBlkIdx0;
        luKey->aclBlkIdx1  = hostGrpTblParm->aclBlkIdx1;
        luKey->aclBlkIdx4  = hostGrpTblParm->aclBlkIdx4;
        luKey->aclBlkIdx5  = hostGrpTblParm->aclBlkIdx5;
        luKey->aclBlkIdx8  = hostGrpTblParm->aclBlkIdx8;
        luKey->aclBlkIdx9  = hostGrpTblParm->aclBlkIdx9;
        luKey->aclBlkIdx12 = hostGrpTblParm->aclBlkIdx12;
        luKey->aclBlkIdx13 = hostGrpTblParm->aclBlkIdx13;
        luKey->dipGrp      = hostGrpTblParm->dipGrp;
        luKey->sipGrp      = hostGrpTblParm->sipGrp;
        luKey->dpGrp       = hostGrpTblParm->dpGrp;
        luKey->spGrp       = hostGrpTblParm->spGrp;
    }

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_net_ip_get(RtUcEntry_t *ucEntry, const cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    RtkApiRet ret                    = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN]     = {0};
    uint8     dipCare[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN]     = {0};
    uint8     sipCare[IPV6_ADDR_LEN] = {0};
    uint8     ipLen;

    do
    {
        if((NULL == ucEntry) || (NULL == netRouteTblParm))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(0u == netRouteTblParm->typeCare)
        {
            ipLen = (0u == ((uint8)ucEntry->netLuKey.entryType & IP6_TYPE_BIT)) ? IPV4_ADDR_LEN : IPV6_ADDR_LEN;
        }
        else
        {
            ipLen = (0u == (netRouteTblParm->type & IP6_TYPE_BIT)) ? IPV4_ADDR_LEN : IPV6_ADDR_LEN;
        }

        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &netRouteTblParm->ipv6Dip31to0, ipLen));
        CHK_FUN_CONTINUE(ret, drv_memcpy(dipCare, &netRouteTblParm->ipv6Dip31to0Care, ipLen));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, ucEntry->netLuKey.dipAddr, ipLen));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dipCare, ucEntry->netLuKey.dipCare, ipLen));

        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &netRouteTblParm->ipv6Sip31to0, ipLen));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sipCare, &netRouteTblParm->ipv6Sip31to0Care, ipLen));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, ucEntry->netLuKey.sipAddr, ipLen));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sipCare, ucEntry->netLuKey.sipCare, ipLen));
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_net_parm_get(RtUcEntry_t *ucEntry, const cvb_RtNetRouteTblParm_t *netRouteTblParm)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((NULL == ucEntry) || (NULL == netRouteTblParm))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, cvb_drv_rt_net_ip_get(ucEntry, netRouteTblParm));

        ucEntry->netLuKey.dp        = netRouteTblParm->dp;
        ucEntry->netLuKey.dpCare    = netRouteTblParm->dpCare;
        ucEntry->netLuKey.sp        = netRouteTblParm->sp;
        ucEntry->netLuKey.spCare    = netRouteTblParm->spCare;
        ucEntry->netLuKey.vid       = netRouteTblParm->vid;
        ucEntry->netLuKey.vidCare   = netRouteTblParm->vidCare;
        ucEntry->netLuKey.entryType = cvb_drv_rt_host_type_num_to_em(netRouteTblParm->type);
        ucEntry->netLuKey.typeCare  = (0u != netRouteTblParm->typeCare) ? ENABLED : DISABLED;
        ucEntry->netLuKey.spFmt     = netRouteTblParm->spFmt;
        ucEntry->netLuKey.spFmtCare = (0u != netRouteTblParm->spFmtCare) ? ENABLED : DISABLED;
        ucEntry->netLuKey.dpFmt     = netRouteTblParm->dpFmt;
        ucEntry->netLuKey.dpFmtCare = (0u != netRouteTblParm->dpFmtCare) ? ENABLED : DISABLED;
        ucEntry->valid              = netRouteTblParm->valid;
        ucEntry->ttl                = netRouteTblParm->ttlChk;
        ucEntry->nhIdx              = netRouteTblParm->nhIdx;
        ucEntry->arbValid           = netRouteTblParm->arbValid;
        ucEntry->arbMode            = netRouteTblParm->arbMode;
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mccol_parm_get(RtMcEntry_t *mcEntry, cvb_RtMcColTblParm_t *mcColParm)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     dip[IPV6_ADDR_LEN] = {0};
    uint8     sip[IPV6_ADDR_LEN] = {0};
    uint8     ipVer;

    do
    {
        if((NULL == mcEntry) || (NULL == mcColParm))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        ipVer = (0u == (mcColParm->type & IP6_TYPE_BIT)) ? IP4_VER : IP6_VER;

        /* Change to Little-endian */
        CHK_FUN_CONTINUE(ret, drv_memcpy(dip, &mcColParm->ipv6Dip31to0, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(sip, &mcColParm->ipv6Sip31to0, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(dip, mcEntry->luKey.dipAddr, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_endian_change(sip, mcEntry->luKey.sipAddr, (IP6_VER == ipVer) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN));
        mcEntry->luKey.dp        = mcColParm->dp;
        mcEntry->luKey.sp        = mcColParm->sp;
        mcEntry->luKey.vid       = mcColParm->vid;
        mcEntry->luKey.dpFmt     = mcColParm->dpFmt;
        mcEntry->luKey.spFmt     = mcColParm->spFmt;
        mcEntry->luKey.entryType = cvb_drv_rt_host_type_num_to_em(mcColParm->type);
        mcEntry->valid           = mcColParm->valid;
        mcEntry->rhTblIdx        = mcColParm->rhIdx;
        mcEntry->arbMode         = mcColParm->arbMode;
        mcEntry->arbValid        = mcColParm->arbValid;
        /* To avoid getting unexpected value of invalid entry */
        mcColParm->dpm &= (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP);
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(mcColParm->dpm, &mcEntry->dpm, CVB_EM_SYS_TO_USER));
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_uc_host_tbl_entry_nonfir_get(uint32 unit, RtUcEntry_t *ucEntry)
{
    RtkApiRet              ret            = RT_ERR_OK;
    cvb_RtHostTblUc4Parm_t hostTblParmUc4 = {0};
    cvb_RtHostTblUc6Parm_t hostTblParmUc6 = {0};

    do
    {
        switch(ucEntry->hostLuKey.entryType)
        {
            case EM_RT_UC4:
                /* Get static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4_get(unit, ucEntry->tblInfo.index, &hostTblParmUc4, ucEntry->tblInfo.entryIdx));
                /* Get host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4_parm_get(ucEntry, &hostTblParmUc4));
                break;
            case EM_RT_UC6:
                /* Get static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6_get(unit, ucEntry->tblInfo.index, &hostTblParmUc6, ucEntry->tblInfo.entryIdx));
                /* Get host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6_parm_get(ucEntry, &hostTblParmUc6));
                break;
            default:
                /* Do nothing */
                break;
        };
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_uc_host_tbl_entry_fir_get(uint32 unit, RtUcEntry_t *ucEntry)
{
    RtkApiRet                 ret               = RT_ERR_OK;
    cvb_RtHostTblUc4FirParm_t hostTblParmUc4Fir = {0};
    cvb_RtHostTblUc6FirParm_t hostTblParmUc6Fir = {0};
    cvb_RtHostGrpTblParm_t    hostGrpTblParm    = {0};

    do
    {
        switch(ucEntry->hostLuKey.entryType)
        {
            case EM_RT_UC4FIR:
                /* Get static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4fir_get(unit, ucEntry->tblInfo.index, &hostTblParmUc4Fir, ucEntry->tblInfo.entryIdx));
                /* Get host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc4fir_parm_get(ucEntry, &hostTblParmUc4Fir));
                /* Get Grp tbl */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_grp_tbl_get(unit, ucEntry->hostLuKey.entryType, ucEntry->tblInfo.index, &hostGrpTblParm, ucEntry->tblInfo.entryIdx));
                /* Get grp parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_hostgrp_parm_get(&ucEntry->hostLuKey, &hostGrpTblParm));
                break;
            case EM_RT_UC6FIR:
                /* Get static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6fir_get(unit, ucEntry->tblInfo.index, &hostTblParmUc6Fir, ucEntry->tblInfo.entryIdx));
                /* Get host parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_uc6fir_parm_get(ucEntry, &hostTblParmUc6Fir));
                /* Get Grp tbl */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_grp_tbl_get(unit, ucEntry->hostLuKey.entryType, ucEntry->tblInfo.index, &hostGrpTblParm, ucEntry->tblInfo.entryIdx));
                /* Get grp parm */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_hostgrp_parm_get(&ucEntry->hostLuKey, &hostGrpTblParm));
                break;
            default:
                /* Do nothing */
                break;
        };
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_rt_uc_host_tbl_entry_get(uint32 unit, RtUcEntry_t *ucEntry)
{
    RtkApiRet         ret                   = RT_ERR_OK;
    uint8             maxSlotNum            = 0;
    cvb_RtNhTblParm_t nhTblParm             = {0};
    LutParam_t        lutEntry              = {0};
    const uint8       zeroMac[MAC_ADDR_LEN] = {0};

    do
    {
        cvb_drv_rt_max_host_entry_idx_num_get(ucEntry->hostLuKey.entryType, &maxSlotNum);

        if(maxSlotNum <= ucEntry->tblInfo.entryIdx)
        {
            ret = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        switch(ucEntry->hostLuKey.entryType)
        {
            case EM_RT_UC4:
            case EM_RT_UC6:
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_uc_host_tbl_entry_nonfir_get(unit, ucEntry));
                break;
            case EM_RT_UC4FIR:
            case EM_RT_UC6FIR:
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_uc_host_tbl_entry_fir_get(unit, ucEntry));
                break;
            default:
                ret = -RT_ERR_INPUT;
                break;
        };

        /* Get nh info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nhtbl_get(unit, ucEntry->nhIdx, &nhTblParm));
        ucEntry->l2Idx    = nhTblParm.l2Idx;
        ucEntry->egrIfIdx = nhTblParm.egrIfIdx;
        CHK_FUN_CONTINUE(ret, cvb_lut_entry_get(unit, ucEntry->l2Idx, &lutEntry));
        if(0 != rtl_memcpy(lutEntry.uni.mac, zeroMac, MAC_ADDR_LEN))
        {
            ucEntry->nhAlloc = ENABLED;
        }
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_rt_net_tbl_entry_get(uint32 unit, RtUcEntry_t *ucEntry)
{
    RtkApiRet               ret                   = RT_ERR_OK;
    cvb_RtNetRouteTblParm_t netRouteTblParm       = {0};
    cvb_RtNhTblParm_t       nhTblParm             = {0};
    LutParam_t              lutEntry              = {0};
    const uint8             zeroMac[MAC_ADDR_LEN] = {0};

    do
    {
        /* Get net table */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nettbl_get(unit, ucEntry->tblInfo.index, &netRouteTblParm));

        /* Get net param */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_net_parm_get(ucEntry, &netRouteTblParm));

        /* Get nh info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nhtbl_get(unit, ucEntry->nhIdx, &nhTblParm));
        ucEntry->l2Idx    = nhTblParm.l2Idx;
        ucEntry->egrIfIdx = nhTblParm.egrIfIdx;
        CHK_FUN_CONTINUE(ret, cvb_lut_entry_get(unit, ucEntry->l2Idx, &lutEntry));
        if(0 != rtl_memcpy(lutEntry.uni.mac, zeroMac, MAC_ADDR_LEN))
        {
            ucEntry->nhAlloc = ENABLED;
        }
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_egrif_parse(uint32 unit, uint16 startOilIdx, uint8 *nextHopIf)
{
    RtkApiRet          ret          = RT_ERR_OK;
    cvb_RtOilTblParm_t oilTableParm = {0};
    uint16             oilIdx       = startOilIdx;
    uint8              breakFlag    = DISABLED;

    do
    {
        oilIdx &= ~(uint16)CVB_OIL_VALID_FLAG;

        if(CVB_OIL_TABLE_MAX <= oilIdx)
        {
            ret       = -RT_ERR_ENTRY_INDEX;
            breakFlag = ENABLED;
        }
        else
        {
            ret = cvb_drv_rt_oiltbl_get(unit, oilIdx, &oilTableParm);

            if(RT_ERR_OK == ret)
            {
                nextHopIf[oilTableParm.egrIfIdx / 8u] |= (ENABLED << (oilTableParm.egrIfIdx % 8u));
                oilIdx = oilTableParm.oilNext;
            }
            else
            {
                breakFlag = ENABLED;
            }
        }

        if(ENABLED == breakFlag)
        {
            break;
        }
    } while(0u != oilTableParm.oilNext);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_egriflist_get(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet         ret       = RT_ERR_OK;
    cvb_RtRhTblParm_t rhTblParm = {0};
    uint8             i;

    do
    {
        if(NULL == mcEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get Rh table */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_rhtbl_get(unit, mcEntry->rhTblIdx, &rhTblParm));

        /* Traverse OIL table */
        for(i = CVB_RTK_SYS_PORT_1ST; i <= CVB_RTK_SYS_PORT_END; i++)
        {
            if(0u != (rhTblParm.portRhIdx[i] & CVB_OIL_VALID_FLAG))
            {
                CHK_FUN_BREAK(ret, cvb_drv_egrif_parse(unit, rhTblParm.portRhIdx[i], mcEntry->nextHopIf))
            }
        }
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_egr_info_get(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet           ret         = RT_ERR_OK;
    cvb_RtEgIfTblParm_t egIfTblParm = {0};
    cvb_RtRhTblParm_t   rhTblParm   = {0};
    cvb_RtOilTblParm_t  oilTblParm  = {0};
    uint16              oilIdx      = 0u;
    uint8               i;

    do
    {
        /* Get Rh table */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_rhtbl_get(unit, mcEntry->rhTblIdx, &rhTblParm));
        for(i = CVB_RTK_SYS_PORT_1ST; i <= CVB_RTK_SYS_PORT_END; i++)
        {
            if(0u != (rhTblParm.portRhIdx[i] & CVB_OIL_VALID_FLAG))
            {
                mcEntry->rhAlloc = ENABLED;
                oilIdx           = rhTblParm.portRhIdx[i] & ~(uint16)CVB_OIL_VALID_FLAG;
                break;
            }
        }

        /* Get TTL */
        if(ENABLED == mcEntry->rhAlloc)
        {
            /* Get oil info */
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_oiltbl_get(unit, oilIdx, &oilTblParm));
            mcEntry->ttl = oilTblParm.ttlChk & oilTblParm.ttlDec;
        }

        /* Routing mode */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_egriftbl_get(unit, 0u, &egIfTblParm));
        if(DISABLED != egIfTblParm.macReplace)
        {
            /* Get next hop interface list */
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_egriflist_get(unit, mcEntry));
        }
    } while(1 == 0);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_host_tbl_entry_nonfir_get(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet              ret            = RT_ERR_OK;
    cvb_RtHostTblMc4Parm_t hostTblParmMc4 = {0};
    cvb_RtHostTblMc6Parm_t hostTblParmMc6 = {0};

    do
    {
        switch(mcEntry->luKey.entryType)
        {
            case EM_RT_MC4:
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4_get(unit, mcEntry->tblInfo.index, &hostTblParmMc4, mcEntry->tblInfo.entryIdx));
                /* Set Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4_parm_get(mcEntry, &hostTblParmMc4));
                break;
            case EM_RT_MC6:
                /* Set static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6_get(unit, mcEntry->tblInfo.index, &hostTblParmMc6, mcEntry->tblInfo.entryIdx));
                /* Set Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6_parm_get(mcEntry, &hostTblParmMc6));
                break;
            default:
                /* Do nothing */
                break;
        };

        /* Get egr info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_egr_info_get(unit, mcEntry));
    } while(1u == 0u);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_mc_host_tbl_entry_fir_get(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet                 ret               = RT_ERR_OK;
    cvb_RtHostTblMc4FirParm_t hostTblParmMc4Fir = {0};
    cvb_RtHostTblMc6FirParm_t hostTblParmMc6Fir = {0};
    cvb_RtHostGrpTblParm_t    hostGrpTblParm    = {0};

    do
    {
        switch(mcEntry->luKey.entryType)
        {
            case EM_RT_MC4FIR:
                /* Get static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4fir_get(unit, mcEntry->tblInfo.index, &hostTblParmMc4Fir, mcEntry->tblInfo.entryIdx));
                /* Get Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc4fir_parm_get(mcEntry, &hostTblParmMc4Fir));
                break;
            case EM_RT_MC6FIR:
                /* Get static host */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6fir_get(unit, mcEntry->tblInfo.index, &hostTblParmMc6Fir, mcEntry->tblInfo.entryIdx));
                /* Get Host param */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_mc6fir_parm_get(mcEntry, &hostTblParmMc6Fir));
                break;
            default:
                /* Do nothing */
                break;
        };

        /* Get Grp tbl */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_host_grp_tbl_get(unit, mcEntry->luKey.entryType, mcEntry->tblInfo.index, &hostGrpTblParm, mcEntry->tblInfo.entryIdx));
        /* Get grp parm */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_hostgrp_parm_get(&mcEntry->luKey, &hostGrpTblParm));
        /* Get egr info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_egr_info_get(unit, mcEntry));
    } while(1u == 0u);

    return ret;
}

RtkApiRet cvb_drv_rt_mc_host_tbl_entry_get(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(mcEntry->luKey.entryType)
    {
        case EM_RT_MC4:
        case EM_RT_MC6:
            ret = cvb_drv_rt_mc_host_tbl_entry_nonfir_get(unit, mcEntry);
            break;
        case EM_RT_MC4FIR:
        case EM_RT_MC6FIR:
            ret = cvb_drv_rt_mc_host_tbl_entry_fir_get(unit, mcEntry);
            break;
        default:
            ret = -RT_ERR_INPUT;
            break;
    };

    return ret;
}

RtkApiRet cvb_drv_rt_mc_col_tbl_entry_get(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet            ret       = RT_ERR_OK;
    cvb_RtMcColTblParm_t mcColParm = {0u};

    do
    {
        /* Get mccol */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mccol_get(unit, mcEntry->tblInfo.index, &mcColParm));

        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mccol_parm_get(mcEntry, &mcColParm));

        /* Get egr info */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_egr_info_get(unit, mcEntry));

    } while(false);

    return ret;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_rt_fw_intf_param_set(cvb_RtFwIntfTbl_t *fwIntfEntry, const RtIntfTbl_t *intfEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((NULL == fwIntfEntry) || (NULL == intfEntry))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, drv_memset(fwIntfEntry, 0, sizeof(cvb_RtFwIntfTbl_t)));
        fwIntfEntry->intFidIvid = intfEntry->intfFidVid;
        CHK_FUN_CONTINUE(ret, drv_memcpy(fwIntfEntry->mac, intfEntry->mac, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(ret, drv_memcpy(fwIntfEntry->gwIp, intfEntry->gwIp, IPV6_ADDR_LEN));
        fwIntfEntry->valid      = intfEntry->valid;
        fwIntfEntry->physicalPn = intfEntry->portId;
        fwIntfEntry->mtuIdx     = (IP4_VER == intfEntry->ipVer) ? CVB_MTU_IDX_IPV4 : CVB_MTU_IDX_IPV6;
    } while(1 == 0);

    return ret;
}

RtkApiRet cvb_drv_rt_sw_intf_set(uint32 unit, const RtIntfTbl_t *intfEntry)
{
    uint32            base            = 0u;
    uint32            i               = 0u;
    uint32            fwIntfEntrySize = (uint32)sizeof(cvb_RtFwIntfTbl_t);
    uint32            data[sizeof(cvb_RtFwIntfTbl_t)];
    cvb_RtFwIntfTbl_t fwIntfEntry = {0};
    RtkApiRet         ret         = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_fw_intf_param_set(&fwIntfEntry, intfEntry));

        CHK_FUN_CONTINUE(ret, drv_memcpy((uint8 *)data, (const uint8 *)&fwIntfEntry, sizeof(cvb_RtFwIntfTbl_t)));

        /* Set SW interface table */
        base = CVB_SW_INTF_TBL_BASE + (fwIntfEntrySize * intfEntry->index);

        for(i = 0u; i < (fwIntfEntrySize / 4u); i++)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_indirect_sram_set(unit, base + (i * 4u), data[i]));
        }
    } while(1 == 0);

    return ret;
}