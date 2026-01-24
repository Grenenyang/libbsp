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

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_api_ext.h"
#include "rtl907xd_va_api_acl.h"
#include "rtl907xd_va_asicdrv_acl.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_tcam.h"
#include "rtl907xd_va_asicdrv_tcam_ctrl.h"
#include "rtl907xd_va_asicdrv_tcam_crc.h"

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
static bool dva_drv_is_tcam_table(dva_table_list_t table)
{
    bool isTcam = false;

    if((DVA_PIE_TABLE == table) || (DVA_STREAM_IDENTIFICATION_TABLE == table) || (DVA_INGIF_TABLE == table) || (DVA_NETWORK_ROUTE_TABLE == table))
    {
        isTcam = true;
    }

    return isTcam;
}

RtkApiRet dva_drv_tcam_check(uint32 unit, const dva_table_list_t table, const uint32 addr, const uint32 *const data)
{
    RtkApiRet ret                               = RT_ERR_OK;
    uint32    goldenData[DVA_TCAM_DATA_REG_MAX] = {0};
    uint32    tcamData[DVA_TCAM_DATA_REG_MAX]   = {0};
    uint32    dataregLen                        = dva_table_list[table].datareg_num * 4u;
    uint32    i                                 = 0u;

    do
    {
        if(true == dva_drv_is_tcam_table(table))
        {
            /* Set golden data */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(goldenData, data, dataregLen));
        }
        else
        {
            /* only tcam table need to check the data */
            continue;
        }

        switch(table)
        {
            /* the golden data will be copied in the case, because only tcam table need to check the data */
            case DVA_PIE_TABLE:
                /* Set HW golden data */
                for(i = 0u; i < DVA_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + DVA_TCAM_ACL_PIE_MASK_OFFSET];
                }
                break;

            case DVA_STREAM_IDENTIFICATION_TABLE:
                /* Set HW golden data */
                for(i = 0u; i < DVA_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + DVA_TCAM_CB_SID_MASK_OFFSET_BF_VALID];
                }
                /* Set HW golden data */
                for(i = DVA_TCAM_VALID_OFFSET + 1u; i < 16u; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + DVA_TCAM_CB_SID_MASK_OFFSET_AF_VALID];
                }
                break;

            case DVA_INGIF_TABLE:
                /* golden data = data & mask */
                goldenData[0] &= goldenData[5];
                goldenData[1] &= goldenData[7];
                goldenData[2] &= goldenData[8];
                break;

            default: /*DVA_NETWORK_ROUTE_TABLE*/
                /* Set HW golden data */
                for(i = 0u; i < DVA_TCAM_NETROUTE_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + DVA_TCAM_RT_NETWORK_MASK_OFFSET_BF_VALID];
                }
                /* Set HW golden data */
                for(i = DVA_TCAM_NETROUTE_VALID_OFFSET + 1u; i < 10u; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + DVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID];
                }
                /* golden data = data & mask(TYPE[3:1], SPORT_FMT[21:20], DPORT_FMT[23:22]) */
                goldenData[10] &= (goldenData[10u + DVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID] | 0xFF0FFFF1u);
                /* golden data = data & mask(VID[20:9]) */
                goldenData[11] &= (goldenData[11u + DVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID] | 0xFFE001FFu);
                break;
        }

        CHK_FUN_CONTINUE(ret, dva_table_read(unit, table, addr, tcamData));

        if(0 != rtl_memcmp(goldenData, tcamData, dataregLen))
        {
            ret = RT_ERR_TCAM_COMPARE;
        }
    } while(0u == 1u);

    return ret;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet dva_drv_tcam_write_internal(uint32 unit, dva_table_list_t table, uint32 addr, const uint32 *data)
#else
RtkApiRet dva_drv_tcam_write(uint32 unit, dva_table_list_t table, uint32 addr, const uint32 *data)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet ret   = RT_ERR_OK;
    uint8     retry = 0u;

    do
    {
        /* Write data to the TCAM table  */
        ret = dva_table_write(unit, table, addr, data);
        /* Retrieve data from the TCAM table and check data content */
        CHK_FUN_RET(ret, dva_drv_tcam_check(unit, table, addr, data));
        if(RT_ERR_OK != ret)
        {
            retry += 1u;
        }
        else
        {
            break;
        }
    } while(retry < DVA_TCAM_WRITE_RETRY_TIME);

    return ret;
}

RtkApiRet dva_drv_tcam_set_pie_entry(uint32 unit, uint32 addr, const uint32 *data)
{
    dva_table_list_t        tableType   = DVA_PIE_TABLE;
    dva_drv_TcamEntryInfo   updateEntry = {.addr = addr, .data = data, .valid = 1u};
    dva_drv_TcamPieCrcVal_t crcVal      = {.val = {0}};
    uint32                  block       = addr / DVA_TCAM_PIE_BLOCK_ENTRY_NUM;
    uint32                  enable      = 0u;
    RtkApiRet               ret         = RT_ERR_OK;

    do
    {
        if(NULL == data)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }
        CHK_FUN_CONTINUE(ret, dva_drv_acl_internal_rule_id_check(unit, addr));

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_safety(unit, &enable));

        if(ENABLE == enable)
        {
            /* Read table entries and calculate the CRC value with the updated entry */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_pie_table_crc(unit, block, &updateEntry, &crcVal));
            /* Disable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_pie_all_check_en(unit, DISABLE));
            /* Write the updated entry into the Table */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
            /* Write the golden CRC value into the golden register */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_pie_block_golden_crc(unit, block, &crcVal));
            /* Enable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_pie_all_check_en(unit, ENABLE));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_netroute_entry(uint32 unit, uint32 addr, const uint32 *data)
{
    dva_table_list_t             tableType   = DVA_NETWORK_ROUTE_TABLE;
    uint32                       tableSize   = dva_table_list[tableType].size;
    dva_drv_TcamEntryInfo        updateEntry = {.addr = addr, .data = data, .valid = 1u};
    dva_drv_TcamNetRouteCrcVal_t crcVal      = {.val = {0}};
    uint32                       enable      = 0u;
    RtkApiRet                    ret         = RT_ERR_OK;

    do
    {
        if(addr >= tableSize)
        {
            ret = -(RT_ERR_TCAM_ADDR);
            continue;
        }

        if(NULL == data)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_safety(unit, &enable));

        if(ENABLE == enable)
        {
            /* Read table entries and calculate the CRC value with the updated entry */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_netroute_table_crc(unit, &updateEntry, &crcVal));
            /* Disable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_all_check_en(unit, DISABLE));
            /* Write the updated entry into the Table */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
            /* Set g_tcamNetRouteAddr to 0 */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_sram_set(unit, DVA_TCAM_SRAM_NETROUTE_ADDR, 0u));
            /* Write the golden CRC value into the golden register */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_all_golden_crc(unit, &crcVal));
            /* Enable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_all_check_en(unit, ENABLE));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_ingif_entry(uint32 unit, uint32 addr, const uint32 *data)
{
    dva_table_list_t          tableType   = DVA_INGIF_TABLE;
    uint32                    tableSize   = dva_table_list[tableType].size;
    dva_drv_TcamEntryInfo     updateEntry = {.addr = addr, .data = data, .valid = 1u};
    dva_drv_TcamIngifCrcVal_t crcVal      = {.val = {0}};
    uint32                    enable      = 0u;
    RtkApiRet                 ret         = RT_ERR_OK;

    do
    {
        if(addr >= tableSize)
        {
            ret = -(RT_ERR_TCAM_ADDR);
            continue;
        }

        if(NULL == data)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_safety(unit, &enable));

        if(ENABLE == enable)
        {
            /* Read table entries and calculate the CRC value with the updated entry */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_ingif_table_crc(unit, &updateEntry, &crcVal));
            /* Disable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_ingif_all_check_en(unit, DISABLE));
            /* Write the updated entry into the Table */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
            /* Write the golden CRC value into the golden register */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_ingif_all_golden_crc(unit, &crcVal));
            /* Enable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_ingif_all_check_en(unit, ENABLE));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_sid_entry(uint32 unit, uint32 addr, const uint32 *data)
{
    dva_table_list_t        tableType   = DVA_STREAM_IDENTIFICATION_TABLE;
    uint32                  tableSize   = dva_table_list[tableType].size;
    dva_drv_TcamEntryInfo   updateEntry = {.addr = addr, .data = data, .valid = 1u};
    dva_drv_TcamSidCrcVal_t crcVal      = {.val = {0}};
    uint32                  enable      = 0u;
    RtkApiRet               ret         = RT_ERR_OK;

    do
    {
        if(addr >= tableSize)
        {
            ret = -(RT_ERR_TCAM_ADDR);
            continue;
        }

        if(NULL == data)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_safety(unit, &enable));

        if(ENABLE == enable)
        {
            /* Read table entries and calculate the CRC value with the updated entry */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_get_sid_table_crc(unit, &updateEntry, &crcVal));
            /* Disable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_all_check_en(unit, DISABLE));
            /* Write the updated entry into the Table */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
            /* Write the golden CRC value into the golden register */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_all_golden_crc(unit, &crcVal));
            /* Enable TCAM check enable */
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_all_check_en(unit, ENABLE));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_tcam_write(unit, tableType, addr, data));
        }
    } while(0u == 1u);

    return ret;
}
