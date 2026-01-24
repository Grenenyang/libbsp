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
#include "rtl907xc_va_asicdrv_tcam.h"

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
static bool cva_drv_is_tcam_table(cva_table_list_t table)
{
    bool isTcam = false;

    if((CVA_PIE_TABLE == table) || (CVA_STREAM_IDENTIFICATION_TABLE == table) || (CVA_INGIF_TABLE == table) || (CVA_NETWORK_ROUTE_TABLE == table))
    {
        isTcam = true;
    }

    return isTcam;
}

RtkApiRet cva_drv_tcam_check(uint32 unit, const cva_table_list_t table, const uint32 addr, const uint32 *const data)
{
    RtkApiRet ret                               = RT_ERR_OK;
    uint32    goldenData[CVA_TCAM_DATA_REG_MAX] = {0};
    uint32    tcamData[CVA_TCAM_DATA_REG_MAX]   = {0};
    uint32    dataregLen                        = cva_table_list[table].datareg_num * 4u;
    uint32    i                                 = 0u;

    do
    {
        if(true == cva_drv_is_tcam_table(table))
        {
            /* Set golden data */
            CHK_FUN_CONTINUE(ret, cva_drv_drv_memcpy(goldenData, data, dataregLen));
        }
        else
        {
            /* only tcam table need to check the data */
            continue;
        }

        switch(table)
        {
            /* the golden data will be copied in the case, because only tcam table need to check the data */
            case CVA_PIE_TABLE:
                /* Set HW golden data */
                for(i = 0u; i < CVA_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVA_TCAM_ACL_PIE_MASK_OFFSET];
                }
                break;

            case CVA_STREAM_IDENTIFICATION_TABLE:
                /* Set HW golden data */
                for(i = 0u; i < CVA_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVA_TCAM_CB_SID_MASK_OFFSET_BF_VALID];
                }
                /* Set HW golden data */
                for(i = CVA_TCAM_VALID_OFFSET + 1u; i < 16u; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVA_TCAM_CB_SID_MASK_OFFSET_AF_VALID];
                }
                break;

            case CVA_INGIF_TABLE:
                /* golden data = data & mask */
                goldenData[0] &= goldenData[5];
                goldenData[1] &= goldenData[7];
                goldenData[2] &= goldenData[8];
                break;

            default: /*CVA_NETWORK_ROUTE_TABLE*/
                /* Set HW golden data */
                for(i = 0u; i < CVA_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVA_TCAM_RT_NETWORK_MASK_OFFSET_BF_VALID];
                }
                /* Set HW golden data */
                for(i = CVA_TCAM_VALID_OFFSET + 1u; i < 10u; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID];
                }
                /* golden data = data & mask(TYPE[3:1], SPORT_FMT[21:20], DPORT_FMT[23:22]) */
                goldenData[10] &= (goldenData[10u + CVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID] | 0xFF0FFFF1u);
                /* golden data = data & mask(VID[20:9]) */
                goldenData[11] &= (goldenData[11u + CVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID] | 0xFFE001FFu);
                break;
        }

        CHK_FUN_CONTINUE(ret, cva_table_read(unit, table, addr, tcamData));

        if(0 != rtl_memcmp(goldenData, tcamData, dataregLen))
        {
            ret = RT_ERR_TCAM_COMPARE;
        }
    } while(0u == 1u);

    return ret;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cva_drv_tcam_write_internal(uint32 unit, cva_table_list_t table, uint32 addr, const uint32 *data)
#else
RtkApiRet cva_drv_tcam_write(uint32 unit, cva_table_list_t table, uint32 addr, const uint32 *data)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet ret = RT_ERR_OK;

    /* Write data to the TCAM table  */
    ret = cva_table_write(unit, table, addr, data);
    /* Retrieve data from the TCAM table and check data content */
    CHK_FUN_RET(ret, cva_drv_tcam_check(unit, table, addr, data));

    return ret;
}

const cva_table_list_t cva_g_tableTypeList[CVA_TABLE_LIST_END] =
    {
        [CVA_VLAN]                            = CVA_VLAN,
        [CVA_RESERVED]                        = CVA_RESERVED,
        [CVA_L2_TABLE]                        = CVA_L2_TABLE,
        [CVA_LUT_BCAM]                        = CVA_LUT_BCAM,
        [CVA_ACTION_TABLE]                    = CVA_ACTION_TABLE,
        [CVA_ACL_COUNTER]                     = CVA_ACL_COUNTER,
        [CVA_POLICER]                         = CVA_POLICER,
        [CVA_PIE_TABLE]                       = CVA_PIE_TABLE,
        [CVA_GROUP]                           = CVA_GROUP,
        [CVA_QBV_GATE_CONTROL_LIST_TABLE]     = CVA_QBV_GATE_CONTROL_LIST_TABLE,
        [CVA_QBV_SERIAL_GATE_OPEN_TIME_TABLE] = CVA_QBV_SERIAL_GATE_OPEN_TIME_TABLE,
        [CVA_STREAM_IDENTIFICATION_TABLE]     = CVA_STREAM_IDENTIFICATION_TABLE,
        [CVA_CB_LOOK_UP_TABLE]                = CVA_CB_LOOK_UP_TABLE,
        [CVA_CB_SEQUENCY_RECOVERY_TABLE]      = CVA_CB_SEQUENCY_RECOVERY_TABLE,
        [CVA_CB_HISTORY_TABLE]                = CVA_CB_HISTORY_TABLE,
        [CVA_RH_TABLE]                        = CVA_RH_TABLE,
        [CVA_OIL_TABLE]                       = CVA_OIL_TABLE,
        [CVA_EGIF_TABLE]                      = CVA_EGIF_TABLE,
        [CVA_INGIF_TABLE]                     = CVA_INGIF_TABLE,
        [CVA_HOST_ROUTE_TABLE]                = CVA_HOST_ROUTE_TABLE,
        [CVA_NETWORK_ROUTE_TABLE]             = CVA_NETWORK_ROUTE_TABLE,
        [CVA_MULTICAST_COL_TABLE]             = CVA_MULTICAST_COL_TABLE,
        [CVA_NEXT_HOP_TABLE]                  = CVA_NEXT_HOP_TABLE,
        [CVA_QCI_HIT_COUNTER]                 = CVA_QCI_HIT_COUNTER,
        [CVA_QCI_GATE_PASS_COUNTER]           = CVA_QCI_GATE_PASS_COUNTER,
        [CVA_QCI_GATE_DROP_COUNTER]           = CVA_QCI_GATE_DROP_COUNTER,
        [CVA_QCI_SDU_PASS_COUNTER]            = CVA_QCI_SDU_PASS_COUNTER,
        [CVA_QCI_SDU_DROP_COUNTER]            = CVA_QCI_SDU_DROP_COUNTER,
        [CVA_QCI_METER_DROP_COUNTER]          = CVA_QCI_METER_DROP_COUNTER,
        [CVA_QCI_GATE_FILTER]                 = CVA_QCI_GATE_FILTER,
        [CVA_QCI_CONTROL_LIST]                = CVA_QCI_CONTROL_LIST,
        [CVA_STATEFUL_LOOK_UP_TABLE]          = CVA_STATEFUL_LOOK_UP_TABLE,
        [CVA_STATEFUL_BCAM]                   = CVA_STATEFUL_BCAM,
        [CVA_STATEFUL_BCAM_SUB]               = CVA_STATEFUL_BCAM_SUB,
        [CVA_ACL_OVER_RATE_CNT]               = CVA_ACL_OVER_RATE_CNT,
        [CVA_ACL_ABNORMAL_TIMES]              = CVA_ACL_ABNORMAL_TIMES,
        [CVA_HEADER_DUMP]                     = CVA_HEADER_DUMP,
        [CVA_TIMESTAMP_DISCARD]               = CVA_TIMESTAMP_DISCARD,
        [CVA_HOST_GROUP_TABLE]                = CVA_HOST_GROUP_TABLE,
        [CVA_QCI_STREAM_FILTER]               = CVA_QCI_STREAM_FILTER,
};