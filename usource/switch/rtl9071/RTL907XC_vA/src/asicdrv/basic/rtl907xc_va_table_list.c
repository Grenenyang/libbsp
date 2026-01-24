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

#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_table_struct.h"
#include "rtl907xc_va_tableField_list.h"
#include "rtkas_debug.h"
#include "rtkas_types.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_asicdrv_ethernet_access.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_va_api_ext.h"

#define CVA_INCR           CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE
#define CVA_INCR_BURST_LSP CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_I2C_BURST_LSP
#define CVA_INCR_BURST_LEN CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_I2C_BURST_LEN
#define CVA_INCR_EXE_LSP   CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_EXECUTE_EXTERNAL_DEVICE_LSP
#define CVA_INCR_EXE_LEN   CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_EXECUTE_EXTERNAL_DEVICE_LEN
#define CVA_INCR_RW_LSP    CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCMD_EXTERNAL_DEVICE_LSP
#define CVA_INCR_RW_LEN    CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCMD_EXTERNAL_DEVICE_LEN
#define CVA_INCR_TYPE_LSP  CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCTBTP_EXTERNAL_DEVICE_LSP
#define CVA_INCR_TYPE_LEN  CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCTBTP_EXTERNAL_DEVICE_LEN
#define CVA_INCR_ADDR_LSP  CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCADDR_EXTERNAL_DEVICE_LSP
#define CVA_INCR_ADDR_LEN  CVA_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCADDR_EXTERNAL_DEVICE_LEN
#define CVA_INDR_BASE      CVA_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE

static RtkApiRet cva_table_incr_set(uint32 rw, uint32 type, uint32 addr, uint32 *regData)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Set the table access control registers */
    /* 1 = EXECUTE */
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_INCR, CVA_INCR_EXE_LSP, CVA_INCR_EXE_LEN, 1u, regData));
    /* 1 = READ */
    /* 0 = WRITE */
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_INCR, CVA_INCR_RW_LSP, CVA_INCR_RW_LEN, rw, regData));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_INCR, CVA_INCR_TYPE_LSP, CVA_INCR_TYPE_LEN, type, regData));
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_INCR, CVA_INCR_ADDR_LSP, CVA_INCR_ADDR_LEN, addr, regData));

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cva_table_read_internal(uint32 unit, cva_table_list_t table, uint32 addr, uint32 *data)
#else
RtkApiRet cva_table_read(uint32 unit, cva_table_list_t table, uint32 addr, uint32 *data)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    i        = 0u;
    uint32    regData  = 0u;
    uint32    busy     = 0u;
    uint32    tryTimes = 0u;

    /* Check the data types of the parameters */
    if((table >= CVA_TABLE_LIST_END) || (addr >= cva_table_list[table].size))
    {
        /* TODO need modify it into Hook rtlglue_printf("Invalid table type or table size.\n");*/
        retVal = -RT_ERR_FAILED;
    }
    else
    {
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            CHK_FUN_RET(retVal, cva_drv_eth_acc_table_get(unit, table, addr, 1u, data));
        }
        else
        {
            /* Set the table access control registers */
            CHK_FUN_RET(retVal, cva_table_incr_set(1u, cva_table_list[table].type, addr, &regData));
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_INCR, regData));

            /* Polling the flag register */
            tryTimes = 0u;
            do
            {
                CHK_FUN_RET(retVal, cva_reg_field_read(unit, CVA_INCR, CVA_INCR_EXE_LSP, CVA_INCR_EXE_LEN, &busy));
                tryTimes++;
                if(tryTimes > 0xFFFFu)
                {
                    /*TODO need modify it into Hook DEBUG_ERR(DBG_TESTCASE,"read busy: %d\n", busy);*/
                    retVal = -(int32)RT_ERR_FAILED;
                    break;
                }
            } while((retVal == RT_ERR_OK) && (busy == 1u));

            /* Get the table entry from the registers */
            for(i = 0u; i < (uint32)cva_table_list[table].datareg_num; i++)
            {
                CHK_FUN_RET(retVal, cva_reg_read(unit, CVA_INDR_BASE + (i * 4u), &data[i]));
                if(retVal != RT_ERR_OK)
                {
                    break;
                }
            }
        }
    }

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cva_table_write_internal(uint32 unit, cva_table_list_t table, uint32 addr, const uint32 *data)
#else
RtkApiRet cva_table_write(uint32 unit, cva_table_list_t table, uint32 addr, const uint32 *data)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    i        = 0u;
    uint32    regData  = 0u;
    uint32    busy     = 0u;
    uint32    tryTimes = 0u;

    /* Check the data types of the parameters */
    if((table >= CVA_TABLE_LIST_END) || (addr >= cva_table_list[table].size))
    {
        /* TODO need modify it into Hook rtlglue_printf("Invalid table type or table size.\n"); */
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            CHK_FUN_RET(retVal, cva_drv_eth_acc_table_set(unit, table, addr, 1u, data));
        }
        else
        {
            /* Set the registers to the table entry  */
            for(i = 0u; i < (uint32)cva_table_list[table].datareg_num; i++)
            {
                CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_INDR_BASE + (i * 4u), data[i]));
                if(retVal != RT_ERR_OK)
                {
                    break;
                }
            }

            /* Set the table access control registers */
            CHK_FUN_RET(retVal, cva_table_incr_set(0u, cva_table_list[table].type, addr, &regData));
            CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_INCR, regData));

            /* Polling the flag register */
            tryTimes = 0u;
            do
            {
                CHK_FUN_RET(retVal, cva_reg_field_read(unit, CVA_INCR, CVA_INCR_EXE_LSP, CVA_INCR_EXE_LEN, &busy));
                tryTimes++;
                if(tryTimes > 0xFFFFu)
                {
                    /* TODO need modify it into Hook DEBUG_ERR(DBG_TESTCASE,"read busy: %d\n", busy); */
                    retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                    break;
                }
            } while((retVal == RT_ERR_OK) && (busy == 1u));
        }
    }

    return retVal;
}

static RtkApiRet cva_table_eth_multi_read(uint32 unit, cva_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    perPktEntry = (RTK_ETHERACCESS_MAX_BURST / cva_table_list[table].datareg_num);
    uint32    processEntry;
    uint32    i = 0;

    while(i < entryNum)
    {
        if((entryNum - i) < perPktEntry)
        {
            processEntry = entryNum - i;
        }
        else
        {
            processEntry = perPktEntry;
        }
        CHK_FUN_BREAK(retVal, cva_drv_eth_acc_table_get(unit, table, addr + i, processEntry, &data[i * cva_table_list[table].datareg_num]));
        i += processEntry;
    }

    return retVal;
}

static RtkApiRet cva_table_i2c_multi_read_process(uint32 unit, cva_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    regData    = 0u;
    uint32    dataRegNum = 0u;
    uint32    tryTimes   = 0u;

    dataRegNum = cva_table_list[table].datareg_num;
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_BURST_OPERATION, CVA_BURST_OPERATION_ENTRYNUM_LSP, CVA_BURST_OPERATION_ENTRYNUM_LEN, entryNum, &regData));                   /* max num (9047): 512u, (6620) 17bit (1~131071), limit in 1~4096 */
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_BURST_OPERATION, CVA_BURST_OPERATION_TABLEDATANUM_CPU_LSP, CVA_BURST_OPERATION_TABLEDATANUM_CPU_LEN, dataRegNum, &regData)); /* datareg nummber */
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_BURST_OPERATION, regData));

    /* Set the table access control registers */
    regData = 0u;
    CHK_FUN_RET(retVal, cva_table_incr_set(1u, cva_table_list[table].type, addr, &regData));
    /* 1 = special burst */
    CHK_FUN_RET(retVal, cva_reg_field_set(CVA_INCR, CVA_INCR_BURST_LSP, CVA_INCR_BURST_LEN, 1u, &regData));
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_INCR, regData));

    /* Send I2C busrt waveform to receive k * 4bytes data*/
    CHK_FUN_RET(retVal, cva_reg_read_burst(unit, CVA_INDR_BASE, data, entryNum * dataRegNum));

    /* disable tbl burst read */
    if(retVal == RT_ERR_OK)
    {
        tryTimes = 0u;
        do
        {
            retVal = cva_reg_field_read(unit, CVA_INCR, CVA_INCR_EXE_LSP, CVA_INCR_EXE_LEN, &regData);
            tryTimes++;
            if(tryTimes >= 0xFFFFFu)
            {
                /*TODO need modify it into Hook rtlglue_printf("[FAILED] Polling EXECUTE_CPU to 0.\n");*/
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                break;
            }
        } while((retVal == RT_ERR_OK) && (regData != 0u));
    }

    /* reset to default value */
    regData = 0u;
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_INCR, regData));

    /* reset to default value */
    CHK_FUN_RET(retVal, cva_reg_write(unit, CVA_BURST_OPERATION, regData));

    return retVal;
}

static RtkApiRet cva_table_i2c_multi_read(uint32 unit, cva_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    perPktEntry = (CVA_TABLE_MAX_BURST / cva_table_list[table].datareg_num);
    uint32    processEntry;
    uint32    i = 0;

    while(i < entryNum)
    {
        if((entryNum - i) < perPktEntry)
        {
            processEntry = entryNum - i;
        }
        else
        {
            processEntry = perPktEntry;
        }
        CHK_FUN_BREAK(retVal, cva_table_i2c_multi_read_process(unit, table, addr + i, &data[i * cva_table_list[table].datareg_num], processEntry));
        i += processEntry;
    }

    return retVal;
}

static RtkApiRet cva_table_multi_read(uint32 unit, cva_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i;

    for(i = 0; i < entryNum; ++i)
    {
        CHK_FUN_BREAK(retVal, cva_table_read(unit, table, addr + i, &data[i * cva_table_list[table].datareg_num]));
    }

    return retVal;
}

RtkApiRet cva_table_burst_read(uint32 unit, cva_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check the data types of the parameters */
    if((table >= CVA_TABLE_LIST_END) || (addr >= cva_table_list[table].size))
    {
        /* TODO need modify it into Hook rtlglue_printf("Invalid table type or table size.\n"); */
        retVal = -RT_ERR_FAILED;
    }
    else
    {
        /* enble tbl burst read */
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            CHK_FUN_RET(retVal, cva_table_eth_multi_read(unit, table, addr, data, entryNum));
        }
        else if(g_RegIF == (uint8)REGIF_I2C)
        {
            CHK_FUN_RET(retVal, cva_table_i2c_multi_read(unit, table, addr, data, entryNum));
        }
        else
        {
            CHK_FUN_RET(retVal, cva_table_multi_read(unit, table, addr, data, entryNum));
        }
    }

    return retVal;
}

const cva_table_t cva_table_list[] =
    {
        {
            /* table name               CVA_VLAN */
            /* access table type */ 0u,
            /* table size */ 4096u,
            /* total data registers */ 3u,
            /* total field numbers */ 12u,
        },
        {
            /* table name               CVA_RESERVED */
            /* access table type */ 1u,
            /* table size */ 0u,
            /* total data registers */ 0u,
            /* total field numbers */ 1u,
        },
        {
            /* table name               CVA_L2_TABLE */
            /* access table type */ 2u,
            /* table size */ 4096u,
            /* total data registers */ 3u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               CVA_LUT_BCAM */
            /* access table type */ 3u,
            /* table size */ 256u,
            /* total data registers */ 3u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               CVA_ACTION_TABLE */
            /* access table type */ 4u,
            /* table size */ 576u,
            /* total data registers */ 4u,
            /* total field numbers */ 20u,
        },
        {
            /* table name               CVA_ACL_COUNTER */
            /* access table type */ 5u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               CVA_POLICER */
            /* access table type */ 6u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 6u,
        },
        {
            /* table name               CVA_PIE_TABLE */
            /* access table type */ 7u,
            /* table size */ 576u,
            /* total data registers */ 13u,
            /* total field numbers */ 14u,
        },
        {
            /* table name               CVA_GROUP */
            /* access table type */ 8u,
            /* table size */ 128u,
            /* total data registers */ 2u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               CVA_QBV_GATE_CONTROL_LIST_TABLE */
            /* access table type */ 9u,
            /* table size */ 4096u,
            /* total data registers */ 2u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               CVA_QBV_SERIAL_GATE_OPEN_TIME_TABLE */
            /* access table type */ 10u,
            /* table size */ 2048u,
            /* total data registers */ 9u,
            /* total field numbers */ 33u,
        },
        {
            /* table name               CVA_STREAM_IDENTIFICATION_TABLE */
            /* access table type */ 11u,
            /* table size */ 96u,
            /* total data registers */ 31u,
            /* total field numbers */ 47u,
        },
        {
            /* table name               CVA_CB_LOOK_UP_TABLE */
            /* access table type */ 12u,
            /* table size */ 96u,
            /* total data registers */ 3u,
            /* total field numbers */ 14u,
        },
        {
            /* table name               CVA_CB_SEQUENCY_RECOVERY_TABLE */
            /* access table type */ 13u,
            /* table size */ 48u,
            /* total data registers */ 3u,
            /* total field numbers */ 9u,
        },
        {
            /* table name               CVA_CB_HISTORY_TABLE */
            /* access table type */ 14u,
            /* table size */ 48u,
            /* total data registers */ 17u,
            /* total field numbers */ 17u,
        },
        {
            /* table name               CVA_RH_TABLE */
            /* access table type */ 15u,
            /* table size */ 2048u,
            /* total data registers */ 9u,
            /* total field numbers */ 31u,
        },
        {
            /* table name               CVA_OIL_TABLE */
            /* access table type */ 16u,
            /* table size */ 8192u,
            /* total data registers */ 1u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               CVA_EGIF_TABLE */
            /* access table type */ 17u,
            /* table size */ 160u,
            /* total data registers */ 3u,
            /* total field numbers */ 17u,
        },
        {
            /* table name               CVA_INGIF_TABLE */
            /* access table type */ 18u,
            /* table size */ 64u,
            /* total data registers */ 9u,
            /* total field numbers */ 41u,
        },
        {
            /* table name               CVA_HOST_ROUTE_TABLE */
            /* access table type */ 19u,
            /* table size */ 1024u,
            /* total data registers */ 27u,
            /* total field numbers */ 28u,
        },
        {
            /* table name               CVA_NETWORK_ROUTE_TABLE */
            /* access table type */ 20u,
            /* table size */ 256u,
            /* total data registers */ 23u,
            /* total field numbers */ 51u,
        },
        {
            /* table name               CVA_MULTICAST_COL_TABLE */
            /* access table type */ 21u,
            /* table size */ 64u,
            /* total data registers */ 13u,
            /* total field numbers */ 30u,
        },
        {
            /* table name               CVA_NEXT_HOP_TABLE */
            /* access table type */ 22u,
            /* table size */ 4096u,
            /* total data registers */ 1u,
            /* total field numbers */ 3u,
        },
        {
            /* table name               CVA_QCI_HIT_COUNTER */
            /* access table type */ 23u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               CVA_QCI_GATE_PASS_COUNTER */
            /* access table type */ 24u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               CVA_QCI_GATE_DROP_COUNTER */
            /* access table type */ 25u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               CVA_QCI_SDU_PASS_COUNTER */
            /* access table type */ 26u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               CVA_QCI_SDU_DROP_COUNTER */
            /* access table type */ 27u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               CVA_QCI_METER_DROP_COUNTER */
            /* access table type */ 28u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               CVA_QCI_GATE_FILTER */
            /* access table type */ 29u,
            /* table size */ 256u,
            /* total data registers */ 3u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               CVA_QCI_CONTROL_LIST */
            /* access table type */ 30u,
            /* table size */ 2048u,
            /* total data registers */ 3u,
            /* total field numbers */ 6u,
        },
        {
            /* table name               CVA_STATEFUL_LOOK_UP_TABLE */
            /* access table type */ 31u,
            /* table size */ 0u,
            /* total data registers */ 16u,
            /* total field numbers */ 16u,
        },
        {
            /* table name               CVA_STATEFUL_BCAM */
            /* access table type */ 32u,
            /* table size */ 0u,
            /* total data registers */ 10u,
            /* total field numbers */ 10u,
        },
        {
            /* table name               CVA_STATEFUL_BCAM_SUB */
            /* access table type */ 33u,
            /* table size */ 0u,
            /* total data registers */ 6u,
            /* total field numbers */ 6u,
        },
        {
            /* table name               CVA_ACL_OVER_RATE_CNT */
            /* access table type */ 34u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               CVA_ACL_ABNORMAL_TIMES */
            /* access table type */ 35u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               CVA_HEADER_DUMP */
            /* access table type */ 36u,
            /* table size */ 100u,
            /* total data registers */ 27u,
            /* total field numbers */ 58u,
        },
        {
            /* table name               CVA_TIMESTAMP_DISCARD */
            /* access table type */ 37u,
            /* table size */ 576u,
            /* total data registers */ 8u,
            /* total field numbers */ 16u,
        },
        {
            /* table name               CVA_HOST_GROUP_TABLE */
            /* access table type */ 38u,
            /* table size */ 1024u,
            /* total data registers */ 16u,
            /* total field numbers */ 16u,
        },
        {
            /* table name               CVA_QCI_STREAM_FILTER */
            /* access table type */ 39u,
            /* table size */ 96u,
            /* total data registers */ 1u,
            /* total field numbers */ 5u,
        },
};
