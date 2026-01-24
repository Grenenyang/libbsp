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
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_table_struct.h"
#include "rtkas_system_porting.h"
#include "rtl907xc_va_asicdrv_routing.h"

DYNAMIC_SCOPE void cva_drv_rt_tbl_indr_6_sft(uint32 *data, uint64 dataLen)
{
    uint64 i;

    if(0u < dataLen)
    {
        /* dataLen=16
         * i = 15 14 13 12 11 10  9  8  7  6
         *     14 13 12 11 10  9  8  7  6 (1)
         */
        for(i = (dataLen - 1u); i > CVA_INDR_NUM_RESERVED; i--)
        {
            data[i] = data[(i - 1u)];
        }
        data[CVA_INDR_NUM_RESERVED] = ENABLED; /* READ-ONLY but it should be assigned to 1u for valid to avoid any risk. */
    }
}

DYNAMIC_SCOPE void cva_drv_rt_tbl_indr_6_sft_restore(uint32 *data, uint64 dataLen)
{
    uint32 i;

    if(0u < dataLen)
    {
        /* dataLen=16
         * i =  6  7  8  9 10 11 12 13 14 15
         *      7  8  9 10 11 12 13 14 15 (0)
         */
        for(i = CVA_INDR_NUM_RESERVED; i < (dataLen - 1u); i++)
        {
            data[i] = data[(i + 1u)];
        }
        data[(dataLen - 1u)] = 0u;
    }
}

DYNAMIC_SCOPE void cva_drv_rt_array_bit32_get(const uint32 *src, uint32 *dest, uint32 lsb, uint32 msb)
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

DYNAMIC_SCOPE void cva_drv_rt_array_bit32_set(uint32 *src, uint32 value, uint32 lsb, uint32 msb)
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

RtkApiRet cva_drv_rt_rhtbl_get(uint32 unit, uint32 entryNum, cva_RtRhTblParm_t *rhTblParm)
{
    cva_RtRhTbl_t rhTblEntry;
    uint32        val    = 0u;
    RtkApiRet     retVal = RT_ERR_OK;

    if(entryNum >= CVA_RH_TABLE_MAX)
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
            CHK_FUN_CONTINUE(retVal, cva_table_read(unit, CVA_RH_TABLE, entryNum, rhTblEntry.val));

            cva_drv_rt_tbl_indr_6_sft_restore(rhTblEntry.val, (uint32)(sizeof(rhTblEntry.val) / sizeof(rhTblEntry.val[0])));

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[0], &val, 0u, 13u);
            rhTblParm->portRhIdx[0] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[0], &val, 16u, 29u);
            rhTblParm->portRhIdx[1] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[1], &val, 0u, 13u);
            rhTblParm->portRhIdx[2] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[1], &val, 16u, 29u);
            rhTblParm->portRhIdx[3] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[2], &val, 0u, 13u);
            rhTblParm->portRhIdx[4] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[2], &val, 16u, 29u);
            rhTblParm->portRhIdx[5] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[3], &val, 0u, 13u);
            rhTblParm->portRhIdx[6] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[3], &val, 16u, 29u);
            rhTblParm->portRhIdx[7] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[4], &val, 0u, 13u);
            rhTblParm->portRhIdx[8] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[4], &val, 16u, 29u);
            rhTblParm->portRhIdx[9] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[5], &val, 0u, 13u);
            rhTblParm->portRhIdx[10] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[5], &val, 16u, 29u);
            rhTblParm->portRhIdx[11] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[6], &val, 0u, 13u);
            rhTblParm->portRhIdx[12] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[6], &val, 16u, 29u);
            rhTblParm->portRhIdx[13] = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&rhTblEntry.val[7], &val, 0u, 13u);
            rhTblParm->portRhIdx[14] = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cva_drv_rt_rhtbl_set(uint32 unit, uint32 entryNum, const cva_RtRhTblParm_t *rhTblParm)
{
    cva_RtRhTbl_t rhTblEntry;
    RtkApiRet     retVal = RT_ERR_OK;

    if(entryNum >= CVA_RH_TABLE_MAX)
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

            cva_drv_rt_array_bit32_set(&rhTblEntry.val[0], rhTblParm->portRhIdx[0], 0u, 13u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[0], rhTblParm->portRhIdx[1], 16u, 29u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[1], rhTblParm->portRhIdx[2], 0u, 13u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[1], rhTblParm->portRhIdx[3], 16u, 29u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[2], rhTblParm->portRhIdx[4], 0u, 13u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[2], rhTblParm->portRhIdx[5], 16u, 29u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[3], rhTblParm->portRhIdx[6], 0u, 13u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[3], rhTblParm->portRhIdx[7], 16u, 29u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[4], rhTblParm->portRhIdx[8], 0u, 13u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[4], rhTblParm->portRhIdx[9], 16u, 29u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[5], rhTblParm->portRhIdx[10], 0u, 13u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[5], rhTblParm->portRhIdx[11], 16u, 29u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[6], rhTblParm->portRhIdx[12], 0u, 13u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[6], rhTblParm->portRhIdx[13], 16u, 29u);
            cva_drv_rt_array_bit32_set(&rhTblEntry.val[7], rhTblParm->portRhIdx[14], 0u, 13u);

            cva_drv_rt_tbl_indr_6_sft(rhTblEntry.val, sizeof(rhTblEntry.val) / sizeof(rhTblEntry.val[0]));

            CHK_FUN_CONTINUE(retVal, cva_table_write(unit, CVA_RH_TABLE, entryNum, rhTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cva_drv_rt_oiltbl_get(uint32 unit, uint32 entryNum, cva_RtOilTblParm_t *oilTblParm)
{
    cva_tblAccess_OIL_TABLE_t oilTblEntry;
    uint32                    val    = 0u;
    RtkApiRet                 retVal = RT_ERR_OK;

    if(entryNum >= CVA_OIL_TABLE_MAX)
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
            CHK_FUN_CONTINUE(retVal, cva_table_read(unit, CVA_OIL_TABLE, entryNum, oilTblEntry.val));

            val = 0u;
            cva_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 0u, 0u);
            oilTblParm->ttlChk = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 1u, 1u);
            oilTblParm->ttlDec = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 2u, 9u);
            oilTblParm->egrIfIdx = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&oilTblEntry.val[0], &val, 10u, 23u);
            oilTblParm->oilNext = (uint16)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cva_drv_rt_oiltbl_set(uint32 unit, uint32 entryNum, const cva_RtOilTblParm_t *oilTblParm)
{
    cva_tblAccess_OIL_TABLE_t oilTblEntry;
    RtkApiRet                 retVal = RT_ERR_OK;

    if(entryNum >= CVA_OIL_TABLE_MAX)
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

            cva_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->ttlChk, 0u, 0u);
            cva_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->ttlDec, 1u, 1u);
            cva_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->egrIfIdx, 2u, 9u);
            cva_drv_rt_array_bit32_set(&oilTblEntry.val[0], oilTblParm->oilNext, 10u, 23u);

            CHK_FUN_CONTINUE(retVal, cva_table_write(unit, CVA_OIL_TABLE, entryNum, oilTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cva_drv_rt_egriftbl_get(uint32 unit, uint32 entryNum, cva_RtEgIfTblParm_t *egIfTblParm)
{
    cva_RtEgIfTbl_t egIfTblEntry;
    uint32          val    = 0u;
    RtkApiRet       retVal = RT_ERR_OK;

    if(entryNum >= CVA_EGIF_TABLE_MAX)
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
            CHK_FUN_CONTINUE(retVal, cva_table_read(unit, CVA_EGIF_TABLE, entryNum, egIfTblEntry.val));

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[0], &val, 0u, 31u);
            egIfTblParm->macId31to0 = val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 0u, 15u);
            egIfTblParm->macId47to32 = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 16u, 16u);
            egIfTblParm->macFmtSel = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 17u, 17u);
            egIfTblParm->macReplace = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 18u, 29u);
            egIfTblParm->vid = (uint16)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[1], &val, 30u, 30u);
            egIfTblParm->vidReplace = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 0u, 2u);
            egIfTblParm->pcp = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 3u, 3u);
            egIfTblParm->pcpReplace = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 4u, 6u);
            egIfTblParm->ipMtuIdx = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 7u, 9u);
            egIfTblParm->ip6MtuIdx = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 10u, 17u);
            egIfTblParm->ipMcTtl = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 18u, 25u);
            egIfTblParm->ip6McHl = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 26u, 26u);
            egIfTblParm->icmpRedirChkEn = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 27u, 28u);
            egIfTblParm->icmpRedirAct = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 29u, 29u);
            egIfTblParm->rpfChkEn = (uint8)val;

            val = 0u;
            cva_drv_rt_array_bit32_get(&egIfTblEntry.val[2], &val, 30u, 31u);
            egIfTblParm->rpfFailAct = (uint8)val;
        } while(1 == 0);
    }

    return retVal;
}

RtkApiRet cva_drv_rt_egriftbl_set(uint32 unit, uint32 entryNum, const cva_RtEgIfTblParm_t *egIfTblParm)
{
    cva_RtEgIfTbl_t egIfTblEntry;
    RtkApiRet       retVal = RT_ERR_OK;

    if(entryNum >= CVA_EGIF_TABLE_MAX)
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

            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[0], egIfTblParm->macId31to0, 0u, 31u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->macId47to32, 0u, 15u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->macFmtSel, 16u, 16u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->macReplace, 17u, 17u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->vid, 18u, 29u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[1], egIfTblParm->vidReplace, 30u, 30u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->pcp, 0u, 2u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->pcpReplace, 3u, 3u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ipMtuIdx, 4u, 6u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ip6MtuIdx, 7u, 9u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ipMcTtl, 10u, 17u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->ip6McHl, 18u, 25u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->icmpRedirChkEn, 26u, 26u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->icmpRedirAct, 27u, 28u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->rpfChkEn, 29u, 29u);
            cva_drv_rt_array_bit32_set(&egIfTblEntry.val[2], egIfTblParm->rpfFailAct, 30u, 31u);

            CHK_FUN_CONTINUE(retVal, cva_table_write(unit, CVA_EGIF_TABLE, entryNum, egIfTblEntry.val));
        } while(1 == 0);
    }

    return retVal;
}