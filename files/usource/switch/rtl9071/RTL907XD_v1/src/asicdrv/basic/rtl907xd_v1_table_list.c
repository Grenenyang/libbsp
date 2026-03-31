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

#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_regField_list.h"
#include "rtl907xd_v1_table_struct.h"
#include "rtl907xd_v1_tableField_list.h"
#include "rtkas_debug.h"
#include "rtkas_types.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#include "rtl907xd_v1_asicdrv_ethernet_access.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_v1_api_ext.h"

#define DV1_INCR           DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE
#define DV1_INCR_BURST_LSP DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_I2C_BURST_LSP
#define DV1_INCR_BURST_LEN DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_I2C_BURST_LEN
#define DV1_INCR_EXE_LSP   DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_EXECUTE_EXTERNAL_DEVICE_LSP
#define DV1_INCR_EXE_LEN   DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_EXECUTE_EXTERNAL_DEVICE_LEN
#define DV1_INCR_RW_LSP    DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCMD_EXTERNAL_DEVICE_LSP
#define DV1_INCR_RW_LEN    DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCMD_EXTERNAL_DEVICE_LEN
#define DV1_INCR_TYPE_LSP  DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCTBTP_EXTERNAL_DEVICE_LSP
#define DV1_INCR_TYPE_LEN  DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCTBTP_EXTERNAL_DEVICE_LEN
#define DV1_INCR_ADDR_LSP  DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCADDR_EXTERNAL_DEVICE_LSP
#define DV1_INCR_ADDR_LEN  DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE_ACCADDR_EXTERNAL_DEVICE_LEN
#define DV1_INDR_BASE      DV1_ALE_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE
#define DV1_MAC_INCR       DV1_MAC_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE
#define DV1_MAC_INDR_BASE  DV1_MAC_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE

static RtkApiRet dv1_table_incr_set(uint32 rw, uint32 type, uint32 indirAddr, uint32 addr, uint32 *regData)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Set the table access control registers */
    /* 1 = EXECUTE */
    CHK_FUN_RET(retVal, dv1_reg_field_set(indirAddr, DV1_INCR_EXE_LSP, DV1_INCR_EXE_LEN, 1u, regData));
    /* 1 = READ */
    /* 0 = WRITE */
    CHK_FUN_RET(retVal, dv1_reg_field_set(indirAddr, DV1_INCR_RW_LSP, DV1_INCR_RW_LEN, rw, regData));
    CHK_FUN_RET(retVal, dv1_reg_field_set(indirAddr, DV1_INCR_TYPE_LSP, DV1_INCR_TYPE_LEN, type, regData));
    CHK_FUN_RET(retVal, dv1_reg_field_set(indirAddr, DV1_INCR_ADDR_LSP, DV1_INCR_ADDR_LEN, addr, regData));

    return retVal;
}

void dv1_indirect_reg_get(const dv1_table_list_t table, uint32 *const pControlReg, uint32 *const pDataBaseReg)
{
    /* Qbv: gate control list & SGOT; Routing: RH & OIL & EGIF => Using MAC table access */
    if((DV1_QBV_GATE_CONTROL_LIST_TABLE == table) || (DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE == table) || (DV1_RH_TABLE == table) || (DV1_OIL_TABLE == table) || (DV1_EGIF_TABLE == table))
    {
        *pControlReg  = DV1_MAC_INCR;
        *pDataBaseReg = DV1_MAC_INDR_BASE;
    }
    else
    {
        *pControlReg  = DV1_INCR;
        *pDataBaseReg = DV1_INDR_BASE;
    }
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet dv1_table_read_internal(uint32 unit, dv1_table_list_t table, uint32 addr, uint32 *data)
#else
RtkApiRet dv1_table_read(uint32 unit, dv1_table_list_t table, uint32 addr, uint32 *data)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    i             = 0u;
    uint32    regData       = 0u;
    uint32    busy          = 0u;
    uint32    tryTimes      = 0u;
    uint32    indirCtrlReg  = 0u;
    uint32    indirData0Reg = 0u;

    /* Check the data types of the parameters */
    if((table >= DV1_TABLE_LIST_END) || (addr >= dv1_table_list[table].size))
    {
        /* TODO need modify it into Hook rtlglue_printf("Invalid table type or table size.\n");*/
        retVal = -RT_ERR_FAILED;
    }
    else
    {
        dv1_indirect_reg_get(table, &indirCtrlReg, &indirData0Reg);

        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
#ifdef RTK_MODULE_ETHERNET_ACCESS
            CHK_FUN_RET(retVal, dv1_drv_eth_acc_table_get(unit, table, addr, 1u, data));
#else
            retVal = -RT_ERR_ETH_ACCESS;
#endif /* RTK_MODULE_ETHERNET_ACCESS */
        }
        else
        {
            /* Set the table access control registers */
            CHK_FUN_RET(retVal, dv1_table_incr_set(1u, dv1_table_list[table].type, indirCtrlReg, addr, &regData));
            CHK_FUN_RET(retVal, dv1_reg_write(unit, indirCtrlReg, regData));

            /* Polling the flag register */
            tryTimes = 0u;
            do
            {
                CHK_FUN_RET(retVal, dv1_reg_field_read(unit, indirCtrlReg, DV1_INCR_EXE_LSP, DV1_INCR_EXE_LEN, &busy));
                tryTimes++;
                if(tryTimes > 0xFFFFu)
                {
                    /*TODO need modify it into Hook DEBUG_ERR(DBG_TESTCASE,"read busy: %d\n", busy);*/
                    retVal = -(int32)RT_ERR_FAILED;
                    break;
                }
            } while((retVal == RT_ERR_OK) && (busy == 1u));

            /* Get the table entry from the registers */
            for(i = 0u; i < (uint32)dv1_table_list[table].datareg_num; i++)
            {
                CHK_FUN_RET(retVal, dv1_reg_read(unit, indirData0Reg + (i * 4u), &data[i]));
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
RtkApiRet dv1_table_write_internal(uint32 unit, dv1_table_list_t table, uint32 addr, const uint32 *data)
#else
RtkApiRet dv1_table_write(uint32 unit, dv1_table_list_t table, uint32 addr, const uint32 *data)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    i             = 0u;
    uint32    regData       = 0u;
    uint32    busy          = 0u;
    uint32    tryTimes      = 0u;
    uint32    indirCtrlReg  = 0u;
    uint32    indirData0Reg = 0u;

    /* Check the data types of the parameters */
    if((table >= DV1_TABLE_LIST_END) || (addr >= dv1_table_list[table].size))
    {
        /* TODO need modify it into Hook rtlglue_printf("Invalid table type or table size.\n"); */
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        dv1_indirect_reg_get(table, &indirCtrlReg, &indirData0Reg);

        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
#ifdef RTK_MODULE_ETHERNET_ACCESS
            CHK_FUN_RET(retVal, dv1_drv_eth_acc_table_set(unit, table, addr, 1u, data));
#else
            retVal = -RT_ERR_ETH_ACCESS;
#endif /* RTK_MODULE_ETHERNET_ACCESS */
        }
        else
        {
            /* Set the registers to the table entry  */
            for(i = 0u; i < (uint32)dv1_table_list[table].datareg_num; i++)
            {
                CHK_FUN_RET(retVal, dv1_reg_write(unit, indirData0Reg + (i * 4u), data[i]));
                if(retVal != RT_ERR_OK)
                {
                    break;
                }
            }

            /* Set the table access control registers */
            CHK_FUN_RET(retVal, dv1_table_incr_set(0u, dv1_table_list[table].type, indirCtrlReg, addr, &regData));
            CHK_FUN_RET(retVal, dv1_reg_write(unit, indirCtrlReg, regData));

            /* Polling the flag register */
            tryTimes = 0u;
            do
            {
                CHK_FUN_RET(retVal, dv1_reg_field_read(unit, indirCtrlReg, DV1_INCR_EXE_LSP, DV1_INCR_EXE_LEN, &busy));
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

#ifdef RTK_MODULE_ETHERNET_ACCESS
static RtkApiRet dv1_table_eth_multi_read(uint32 unit, dv1_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    perPktEntry = (RTK_ETHERACCESS_MAX_BURST / dv1_table_list[table].datareg_num);
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
        CHK_FUN_BREAK(retVal, dv1_drv_eth_acc_table_get(unit, table, addr + i, processEntry, &data[i * dv1_table_list[table].datareg_num]));
        i += processEntry;
    }

    return retVal;
}
#endif /* RTK_MODULE_ETHERNET_ACCESS */

static RtkApiRet dv1_table_i2c_multi_read_process(uint32 unit, dv1_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    regData       = 0u;
    uint32    dataRegNum    = 0u;
    uint32    tryTimes      = 0u;
    uint32    indirCtrlReg  = 0u;
    uint32    indirData0Reg = 0u;

    dv1_indirect_reg_get(table, &indirCtrlReg, &indirData0Reg);

    dataRegNum = dv1_table_list[table].datareg_num;
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_ALE_BURST_OPERATION, DV1_ALE_BURST_OPERATION_ENTRYNUM_LSP, DV1_ALE_BURST_OPERATION_ENTRYNUM_LEN, entryNum, &regData));                   /* max num (9047): 512u, (6620) 17bit (1~131071), limit in 1~4096 */
    CHK_FUN_RET(retVal, dv1_reg_field_set(DV1_ALE_BURST_OPERATION, DV1_ALE_BURST_OPERATION_TABLEDATANUM_CPU_LSP, DV1_ALE_BURST_OPERATION_TABLEDATANUM_CPU_LEN, dataRegNum, &regData)); /* datareg nummber */
    CHK_FUN_RET(retVal, dv1_reg_write(unit, DV1_ALE_BURST_OPERATION, regData));

    /* Set the table access control registers */
    regData = 0u;
    CHK_FUN_RET(retVal, dv1_table_incr_set(1u, dv1_table_list[table].type, indirCtrlReg, addr, &regData));
    /* 1 = special burst */
    CHK_FUN_RET(retVal, dv1_reg_field_set(indirCtrlReg, DV1_INCR_BURST_LSP, DV1_INCR_BURST_LEN, 1u, &regData));
    CHK_FUN_RET(retVal, dv1_reg_write(unit, indirCtrlReg, regData));

    /* Send I2C busrt waveform to receive k * 4bytes data*/
    CHK_FUN_RET(retVal, dv1_reg_read_burst(unit, indirData0Reg, data, entryNum * dataRegNum));

    /* disable tbl burst read */
    if(retVal == RT_ERR_OK)
    {
        tryTimes = 0u;
        do
        {
            retVal = dv1_reg_field_read(unit, indirCtrlReg, DV1_INCR_EXE_LSP, DV1_INCR_EXE_LEN, &regData);
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
    CHK_FUN_RET(retVal, dv1_reg_write(unit, indirCtrlReg, regData));

    /* reset to default value */
    CHK_FUN_RET(retVal, dv1_reg_write(unit, DV1_ALE_BURST_OPERATION, regData));

    return retVal;
}

static RtkApiRet dv1_table_i2c_multi_read(uint32 unit, dv1_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    perPktEntry = (DV1_TABLE_MAX_BURST / dv1_table_list[table].datareg_num);
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
        CHK_FUN_BREAK(retVal, dv1_table_i2c_multi_read_process(unit, table, addr + i, &data[i * dv1_table_list[table].datareg_num], processEntry));
        i += processEntry;
    }

    return retVal;
}

static RtkApiRet dv1_table_multi_read(uint32 unit, dv1_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i;

    for(i = 0; i < entryNum; ++i)
    {
        CHK_FUN_BREAK(retVal, dv1_table_read(unit, table, addr + i, &data[i * dv1_table_list[table].datareg_num]));
    }

    return retVal;
}

RtkApiRet dv1_table_burst_read(uint32 unit, dv1_table_list_t table, uint32 addr, uint32 *data, uint32 entryNum)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check the data types of the parameters */
    if((table >= DV1_TABLE_LIST_END) || (addr >= dv1_table_list[table].size))
    {
        /* TODO need modify it into Hook rtlglue_printf("Invalid table type or table size.\n"); */
        retVal = -RT_ERR_FAILED;
    }
    else
    {

        /* enble tbl burst read */
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
#ifdef RTK_MODULE_ETHERNET_ACCESS
            CHK_FUN_RET(retVal, dv1_table_eth_multi_read(unit, table, addr, data, entryNum));
#else
            retVal = -RT_ERR_ETH_ACCESS;
#endif /* RTK_MODULE_ETHERNET_ACCESS */
        }
        else if(g_RegIF == (uint8)REGIF_I2C)
        {
            CHK_FUN_RET(retVal, dv1_table_i2c_multi_read(unit, table, addr, data, entryNum));
        }
        else
        {
            CHK_FUN_RET(retVal, dv1_table_multi_read(unit, table, addr, data, entryNum));
        }
    }

    return retVal;
}

const dv1_table_t dv1_table_list[DV1_TABLE_LIST_END] =
    {
        {
            /* table name               DV1_VLAN */
            /* access table type */ 0u,
            /* table size */ 4096u,
            /* total data registers */ 3u,
            /* total field numbers */ 12u,
        },
        {
            /* table name               DV1_RESERVED */
            /* access table type */ 1u,
            /* table size */ 0u,
            /* total data registers */ 0u,
            /* total field numbers */ 1u,
        },
        {
            /* table name               DV1_L2_TABLE */
            /* access table type */ 2u,
            /* table size */ 4096u,
            /* total data registers */ 3u,
            /* total field numbers */ 6u,
        },
        {
            /* table name               DV1_LUT_BCAM */
            /* access table type */ 3u,
            /* table size */ 256u,
            /* total data registers */ 3u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               DV1_ACTION_TABLE */
            /* access table type */ 4u,
            /* table size */ 576u,
            /* total data registers */ 4u,
            /* total field numbers */ 20u,
        },
        {
            /* table name               DV1_ACL_COUNTER */
            /* access table type */ 5u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               DV1_POLICER */
            /* access table type */ 6u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 6u,
        },
        {
            /* table name               DV1_PIE_TABLE */
            /* access table type */ 7u,
            /* table size */ 576u,
            /* total data registers */ 13u,
            /* total field numbers */ 14u,
        },
        {
            /* table name               DV1_GROUP */
            /* access table type */ 8u,
            /* table size */ 128u,
            /* total data registers */ 2u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               DV1_QBV_GATE_CONTROL_LIST_TABLE */
            /* access table type */ 9u,
            /* table size */ 4096u,
            /* total data registers */ 2u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE */
            /* access table type */ 10u,
            /* table size */ 2048u,
            /* total data registers */ 16u,
            /* total field numbers */ 40u,
        },
        {
            /* table name               DV1_STREAM_IDENTIFICATION_TABLE */
            /* access table type */ 11u,
            /* table size */ 96u,
            /* total data registers */ 31u,
            /* total field numbers */ 47u,
        },
        {
            /* table name               DV1_CB_LOOK_UP_TABLE */
            /* access table type */ 12u,
            /* table size */ 96u,
            /* total data registers */ 3u,
            /* total field numbers */ 14u,
        },
        {
            /* table name               DV1_CB_SEQUENCY_RECOVERY_TABLE */
            /* access table type */ 13u,
            /* table size */ 48u,
            /* total data registers */ 3u,
            /* total field numbers */ 9u,
        },
        {
            /* table name               DV1_CB_HISTORY_TABLE */
            /* access table type */ 14u,
            /* table size */ 48u,
            /* total data registers */ 16u,
            /* total field numbers */ 16u,
        },
        {
            /* table name               DV1_RH_TABLE */
            /* access table type */ 15u,
            /* table size */ 2048u,
            /* total data registers */ 8u,
            /* total field numbers */ 30u,
        },
        {
            /* table name               DV1_OIL_TABLE */
            /* access table type */ 16u,
            /* table size */ 8192u,
            /* total data registers */ 1u,
            /* total field numbers */ 5u,
        },
        {
            /* table name               DV1_EGIF_TABLE */
            /* access table type */ 17u,
            /* table size */ 160u,
            /* total data registers */ 3u,
            /* total field numbers */ 17u,
        },
        {
            /* table name               DV1_INGIF_TABLE */
            /* access table type */ 18u,
            /* table size */ 64u,
            /* total data registers */ 9u,
            /* total field numbers */ 41u,
        },
        {
            /* table name               DV1_HOST_ROUTE_TABLE */
            /* access table type */ 19u,
            /* table size */ 1024u,
            /* total data registers */ 26u,
            /* total field numbers */ 27u,
        },
        {
            /* table name               DV1_NETWORK_ROUTE_TABLE */
            /* access table type */ 20u,
            /* table size */ 256u,
            /* total data registers */ 23u,
            /* total field numbers */ 51u,
        },
        {
            /* table name               DV1_MULTICAST_COL_TABLE */
            /* access table type */ 21u,
            /* table size */ 64u,
            /* total data registers */ 12u,
            /* total field numbers */ 29u,
        },
        {
            /* table name               DV1_NEXT_HOP_TABLE */
            /* access table type */ 22u,
            /* table size */ 4096u,
            /* total data registers */ 1u,
            /* total field numbers */ 3u,
        },
        {
            /* table name               DV1_QCI_HIT_COUNTER */
            /* access table type */ 23u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               DV1_QCI_GATE_PASS_COUNTER */
            /* access table type */ 24u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               DV1_QCI_GATE_DROP_COUNTER */
            /* access table type */ 25u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               DV1_QCI_SDU_PASS_COUNTER */
            /* access table type */ 26u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               DV1_QCI_SDU_DROP_COUNTER */
            /* access table type */ 27u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               DV1_QCI_METER_DROP_COUNTER */
            /* access table type */ 28u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 2u,
        },
        {
            /* table name               DV1_QCI_GATE_FILTER */
            /* access table type */ 29u,
            /* table size */ 192u,
            /* total data registers */ 3u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               DV1_QCI_CONTROL_LIST */
            /* access table type */ 30u,
            /* table size */ 1536u,
            /* total data registers */ 3u,
            /* total field numbers */ 6u,
        },
        {
            /* table name               DV1_STATEFUL_LOOK_UP_TABLE */
            /* access table type */ 31u,
            /* table size */ 0u,
            /* total data registers */ 16u,
            /* total field numbers */ 16u,
        },
        {
            /* table name               DV1_STATEFUL_BCAM */
            /* access table type */ 32u,
            /* table size */ 0u,
            /* total data registers */ 10u,
            /* total field numbers */ 10u,
        },
        {
            /* table name               DV1_STATEFUL_BCAM_SUB */
            /* access table type */ 33u,
            /* table size */ 0u,
            /* total data registers */ 6u,
            /* total field numbers */ 6u,
        },
        {
            /* table name               DV1_ACL_OVER_RATE_CNT */
            /* access table type */ 34u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               DV1_ACL_ABNORMAL_TIMES */
            /* access table type */ 35u,
            /* table size */ 576u,
            /* total data registers */ 2u,
            /* total field numbers */ 4u,
        },
        {
            /* table name               DV1_HEADER_DUMP */
            /* access table type */ 36u,
            /* table size */ 100u,
            /* total data registers */ 31u,
            /* total field numbers */ 59u,
        },
        {
            /* table name               DV1_TIMER_DISCARD */
            /* access table type */ 37u,
            /* table size */ 576u,
            /* total data registers */ 7u,
            /* total field numbers */ 15u,
        },
        {
            /* table name               DV1_HOST_GROUP_TABLE */
            /* access table type */ 38u,
            /* table size */ 1024u,
            /* total data registers */ 15u,
            /* total field numbers */ 15u,
        },
        {
            /* table name               DV1_QCI_STREAM_FILTER */
            /* access table type */ 39u,
            /* table size */ 96u,
            /* total data registers */ 2u,
            /* total field numbers */ 8u,
        },
};
