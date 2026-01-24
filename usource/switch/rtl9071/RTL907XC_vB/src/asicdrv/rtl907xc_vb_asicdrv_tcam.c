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
#include "rtl907xc_vb_asicdrv_tcam.h"

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
static bool cvb_drv_is_tcam_table(cvb_table_list_t table)
{
    bool isTcam = false;

    if((CVB_PIE_TABLE == table) || (CVB_STREAM_IDENTIFICATION_TABLE == table) || (CVB_INGIF_TABLE == table) || (CVB_NETWORK_ROUTE_TABLE == table))
    {
        isTcam = true;
    }

    return isTcam;
}

RtkApiRet cvb_drv_tcam_check(uint32 unit, const cvb_table_list_t table, const uint32 addr, const uint32 *const data)
{
    RtkApiRet ret                               = RT_ERR_OK;
    uint32    goldenData[CVB_TCAM_DATA_REG_MAX] = {0};
    uint32    tcamData[CVB_TCAM_DATA_REG_MAX]   = {0};
    uint32    dataregLen                        = cvb_table_list[table].datareg_num * 4u;
    uint32    i                                 = 0u;

    do
    {
        if(true == cvb_drv_is_tcam_table(table))
        {
            /* Set golden data */
            CHK_FUN_CONTINUE(ret, cvb_drv_drv_memcpy(goldenData, data, dataregLen));
        }
        else
        {
            /* only tcam table need to check the data */
            continue;
        }

        switch(table)
        {
            /* the golden data will be copied in the case, because only tcam table need to check the data */
            case CVB_PIE_TABLE:
                /* Set HW golden data */
                for(i = 0u; i < CVB_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVB_TCAM_ACL_PIE_MASK_OFFSET];
                }
                break;

            case CVB_STREAM_IDENTIFICATION_TABLE:
                /* Set HW golden data */
                for(i = 0u; i < CVB_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVB_TCAM_CB_SID_MASK_OFFSET_BF_VALID];
                }
                /* Set HW golden data */
                for(i = CVB_TCAM_VALID_OFFSET + 1u; i < 16u; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVB_TCAM_CB_SID_MASK_OFFSET_AF_VALID];
                }
                break;

            case CVB_INGIF_TABLE:
                /* golden data = data & mask */
                goldenData[0] &= goldenData[5];
                goldenData[1] &= goldenData[7];
                goldenData[2] &= goldenData[8];
                break;

            default: /*CVB_NETWORK_ROUTE_TABLE*/
                /* Set HW golden data */
                for(i = 0u; i < CVB_TCAM_VALID_OFFSET; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVB_TCAM_RT_NETWORK_MASK_OFFSET_BF_VALID];
                }
                /* Set HW golden data */
                for(i = CVB_TCAM_VALID_OFFSET + 1u; i < 10u; i++)
                {
                    /* golden data = data & mask */
                    goldenData[i] &= goldenData[i + CVB_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID];
                }
                /* golden data = data & mask(TYPE[3:1], SPORT_FMT[21:20], DPORT_FMT[23:22]) */
                goldenData[10] &= (goldenData[10u + CVB_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID] | 0xFF0FFFF1u);
                /* golden data = data & mask(VID[20:9]) */
                goldenData[11] &= (goldenData[11u + CVB_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID] | 0xFFE001FFu);
                break;
        }

        CHK_FUN_CONTINUE(ret, cvb_table_read(unit, table, addr, tcamData));

        if(0 != rtl_memcmp(goldenData, tcamData, dataregLen))
        {
            ret = RT_ERR_TCAM_COMPARE;
        }
    } while(0u == 1u);

    return ret;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cvb_drv_tcam_write_internal(uint32 unit, cvb_table_list_t table, uint32 addr, const uint32 *data)
#else
RtkApiRet cvb_drv_tcam_write(uint32 unit, cvb_table_list_t table, uint32 addr, const uint32 *data)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet ret = RT_ERR_OK;

    /* Write data to the TCAM table  */
    ret = cvb_table_write(unit, table, addr, data);
    /* Retrieve data from the TCAM table and check data content */
    CHK_FUN_RET(ret, cvb_drv_tcam_check(unit, table, addr, data));

    return ret;
}

const cvb_table_list_t cvb_g_tableTypeList[CVB_TABLE_LIST_END] =
    {
        [CVB_VLAN]                            = CVB_VLAN,
        [CVB_RESERVED]                        = CVB_RESERVED,
        [CVB_L2_TABLE]                        = CVB_L2_TABLE,
        [CVB_LUT_BCAM]                        = CVB_LUT_BCAM,
        [CVB_ACTION_TABLE]                    = CVB_ACTION_TABLE,
        [CVB_ACL_COUNTER]                     = CVB_ACL_COUNTER,
        [CVB_POLICER]                         = CVB_POLICER,
        [CVB_PIE_TABLE]                       = CVB_PIE_TABLE,
        [CVB_GROUP]                           = CVB_GROUP,
        [CVB_QBV_GATE_CONTROL_LIST_TABLE]     = CVB_QBV_GATE_CONTROL_LIST_TABLE,
        [CVB_QBV_SERIAL_GATE_OPEN_TIME_TABLE] = CVB_QBV_SERIAL_GATE_OPEN_TIME_TABLE,
        [CVB_STREAM_IDENTIFICATION_TABLE]     = CVB_STREAM_IDENTIFICATION_TABLE,
        [CVB_CB_LOOK_UP_TABLE]                = CVB_CB_LOOK_UP_TABLE,
        [CVB_CB_SEQUENCY_RECOVERY_TABLE]      = CVB_CB_SEQUENCY_RECOVERY_TABLE,
        [CVB_CB_HISTORY_TABLE]                = CVB_CB_HISTORY_TABLE,
        [CVB_RH_TABLE]                        = CVB_RH_TABLE,
        [CVB_OIL_TABLE]                       = CVB_OIL_TABLE,
        [CVB_EGIF_TABLE]                      = CVB_EGIF_TABLE,
        [CVB_INGIF_TABLE]                     = CVB_INGIF_TABLE,
        [CVB_HOST_ROUTE_TABLE]                = CVB_HOST_ROUTE_TABLE,
        [CVB_NETWORK_ROUTE_TABLE]             = CVB_NETWORK_ROUTE_TABLE,
        [CVB_MULTICAST_COL_TABLE]             = CVB_MULTICAST_COL_TABLE,
        [CVB_NEXT_HOP_TABLE]                  = CVB_NEXT_HOP_TABLE,
        [CVB_QCI_HIT_COUNTER]                 = CVB_QCI_HIT_COUNTER,
        [CVB_QCI_GATE_PASS_COUNTER]           = CVB_QCI_GATE_PASS_COUNTER,
        [CVB_QCI_GATE_DROP_COUNTER]           = CVB_QCI_GATE_DROP_COUNTER,
        [CVB_QCI_SDU_PASS_COUNTER]            = CVB_QCI_SDU_PASS_COUNTER,
        [CVB_QCI_SDU_DROP_COUNTER]            = CVB_QCI_SDU_DROP_COUNTER,
        [CVB_QCI_METER_DROP_COUNTER]          = CVB_QCI_METER_DROP_COUNTER,
        [CVB_QCI_GATE_FILTER]                 = CVB_QCI_GATE_FILTER,
        [CVB_QCI_CONTROL_LIST]                = CVB_QCI_CONTROL_LIST,
        [CVB_STATEFUL_LOOK_UP_TABLE]          = CVB_STATEFUL_LOOK_UP_TABLE,
        [CVB_STATEFUL_BCAM]                   = CVB_STATEFUL_BCAM,
        [CVB_STATEFUL_BCAM_SUB]               = CVB_STATEFUL_BCAM_SUB,
        [CVB_ACL_OVER_RATE_CNT]               = CVB_ACL_OVER_RATE_CNT,
        [CVB_ACL_ABNORMAL_TIMES]              = CVB_ACL_ABNORMAL_TIMES,
        [CVB_HEADER_DUMP]                     = CVB_HEADER_DUMP,
        [CVB_TIMESTAMP_DISCARD]               = CVB_TIMESTAMP_DISCARD,
        [CVB_HOST_GROUP_TABLE]                = CVB_HOST_GROUP_TABLE,
        [CVB_QCI_STREAM_FILTER]               = CVB_QCI_STREAM_FILTER,
};