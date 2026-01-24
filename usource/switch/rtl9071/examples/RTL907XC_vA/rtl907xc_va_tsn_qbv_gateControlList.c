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

/* include file */
#include "rtkas_api_ext.h"
#include "rtkas_api_st.h"
#include "rtkas_api_psfp.h"
#include "rtl907xc_va_api_st.h"
#include "rtl907xc_va_api_qos.h"
#include "rtl907xc_va_api_ptp.h"
#include "rtl907xc_va_api_interface.h"
/* define macro */
#define CVA_QBV_INGRESS_PORT_NUM (CVA_RTK_USER_PORT_1ST)
#define CVA_QBV_EGRESS_PORT_NUM  (CVA_RTK_USER_PORT_2ND)
#define CVA_QBV_100M_INTERVAL    (12720u)
#define CVA_QBV_PENDING          (1u)
/*1000 microseconds*/
#define CVA_TRYTIMEMAX           (1000u)
#define CVA_NANOSECONDMAX        (1000000000u)
#define CVA_QBV_PTPREADY_TIMEOUT (1)
#define CVA_QBV_PENDING_TIMEOUT  (2)

/**
 * @addtogroup ST_EXAMPLE QBV/ST Example
 * @{
 */

/**
 * @addtogroup RTL907XCVA_ST_EXAMPLE RTL907XC_VA QBV/ST Example
 * @{
 */

static RtkApiRet cva_tsn_qbv_gate_config_check(UnitChip_t unitChip, cva_RtkPort portId)
{
    RtkApiRet                result = RT_ERR_OK;
    StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX];

    StGateEnableInfo_t StGateEnableInfo = {
        .portId = portId,
    };
    StByteCountGet_t StByteCountGet = {
        .portId = portId,
    };
    StGateConfigInfo_t StGateConfigAdmin = {
        .portId          = portId,
        .gateControlList = gateControlList,
    };
    StGateConfigInfo_t StGateConfigOper = {
        .portId          = portId,
        .gateControlList = gateControlList,
    };
    do
    {
        CHK_FUN_CONTINUE(result, rtk_st_gate_enable_get(unitChip, &StGateEnableInfo));
        CHK_FUN_CONTINUE(result, rtk_st_byte_count_get(unitChip, &StByteCountGet));
        CHK_FUN_CONTINUE(result, rtk_st_admin_gate_config_get(unitChip, &StGateConfigAdmin));
        CHK_FUN_CONTINUE(result, rtk_st_oper_gate_config_get(unitChip, &StGateConfigOper));
    } while(0u == 1u);

    return result;
}
/**
 * @defgroup RTL907XCVA_ST_GATE_CONTROL_LIST_OPEN_EXAMPLE QBV/ST Gate Control List Open Example
 *
 * @brief <b> Configure the QBV open gate control list. </b>
 *
 * @section Purpose
 *          This example code demonstrates how to set the QBV gate control list configuration.
 *
 * @section Description
 *          In this example, you need to configure PTP, VLAN and QOS first. \n
 *          After all configurations are completed, te behavior is as follows, \n
 *          The host sequentially injects 128-byte VLAN2 packets with VLAN priorities ranging from 0 to 7 into P1. \n
 *          The VLAN priority of the TX-packet sequence should be 7,6,5,4,3,2,1,0. \n
 *          Note: The maximum time(uint:microsecond) for polling in step 5 is equal to baseTime plus CVA_TRYTIMEMAX, \n
 *          CVA_TRYTIMEMAX is determined by the frequency of the platform,  \n
 *          the reference frequency for the value in this example is 125MHz. \n
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_tsn_qbv_gate_control_list_open(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_tsn_qbv_gate_control_list_open(uint32 unit)
{
    RtkApiRet  result   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VA};
    uint32                   i = 0;
    PsfpCurrentTimeGet_t     currentTime;
    uint64                   estSecond = 0;
    StGateControlListParam_t gateControlList[16];
    uint32                   startTime         = 0u;
    uint32                   current           = 0u;
    uint32                   waitNoPendMaxTime = 0u;
    (void)drv_memset(gateControlList, 0, (16u * sizeof(StGateControlListParam_t)));
    (void)drv_memset(&currentTime, 0, sizeof(PsfpCurrentTimeGet_t));
    /* QoS config */

    QosPriSelInfo_t qosPriSel = {
        .qosPriSel = {
            .portPri   = 0,
            .hybridPri = 7,
            .dscpPri   = 0,
            .cvlanPri  = 0,
            .svlanPri  = 0,
        },
    };

    QosCvlanPriRemap_t qosCvlanPriRemap = {
        .idx = 0,
    };
    QosSvlanPriRemap_t qosSvlanPriRemap = {
        .idx = 0,
    };
    /* PTP config */
    PtpMasterEn_t ptpMasterEnInfo = {
        .masterEn = ENABLED,
    };
    PtpSlave0En_t ptpSlave0EnInfo = {
        .slave0En = ENABLED,
    };
    PtpSlave1En_t ptpSlave1EnInfo = {
        .slave1En = ENABLED,
    };
    PtpSlave2En_t ptpSlave2EnInfo = {
        .slave2En = ENABLED,
    };
    PtpHwEnable_t ptpHwEnableInfo = {
        .ptpEnable = ENABLED,
    };
    /* GCL config */
    StConfigPendingGet_t stConfigPendingGet = {
        .portId = CVA_QBV_EGRESS_PORT_NUM,
    };
    StByteCountSet_t stByteCountInfo = {
        .portId    = CVA_QBV_EGRESS_PORT_NUM,
        .portType  = CVA_ST_PORT_TYPE_100BASE_T1,
        .speed     = RTK_PORT_SPEED_100M,
        .macsecIPG = CVA_NO_MACSEC,
    };
    StGateEnableInfo_t stGateEnableInfo = {
        .portId = CVA_QBV_EGRESS_PORT_NUM,
        .enable = ENABLED,

    };
    StGateConfigInfo_t stGateConfig = {
        .portId             = CVA_QBV_EGRESS_PORT_NUM,
        .controlListLength  = 16,
        .cycleTimeExtension = 10000,
    };

    do
    {
        /*
         * Preconfiguration
         *   All ports (P1~11) join the same VLAN (e.g., VLAN2)
         */

        /* Environment configuration
         *   Force to use dot1q priority (QoS)
         *   Preconfigure VLAN and priority to queue mapping (pri:0~7 to que:0~7)
         */

        result = rtk_qos_pri_sel_set(unitChip, &qosPriSel);

        for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
        {
            qosCvlanPriRemap.iPri       = i;
            qosCvlanPriRemap.iDei       = 0;
            qosCvlanPriRemap.mappingPri = i;
            CHK_FUN_RET(result, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
            qosCvlanPriRemap.iDei = 1;
            CHK_FUN_RET(result, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
            qosSvlanPriRemap.oPri       = i;
            qosSvlanPriRemap.oDei       = 0;
            qosSvlanPriRemap.mappingPri = i;
            CHK_FUN_RET(result, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
            qosSvlanPriRemap.oDei = 1;
            CHK_FUN_RET(result, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
        }

        /* Env
         *   Start PTP
         */
        CHK_FUN_RET(result, rtk_ptp_set_master_en(unitChip, &ptpMasterEnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave0_en(unitChip, &ptpSlave0EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave1_en(unitChip, &ptpSlave1EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave2_en(unitChip, &ptpSlave2EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_hw_enable(unitChip, &ptpHwEnableInfo));

        /* [Option] If DUT boots up and doesn't init QBV */
        /* Notice: st_init() can only be called once after DUT is restarted */
        CHK_FUN_RET(result, rtk_st_init(unitChip));

        /* Step 1
         *   Enable QBV on P2
         */
        /* User enable gate */
        CHK_FUN_RET(result, rtk_st_gate_enable_set(unitChip, &stGateEnableInfo));

        /* Step 2
         *   Set the byte_count on P2 (100M)
         */
        CHK_FUN_RET(result, rtk_st_byte_count_set(unitChip, &stByteCountInfo));
        /* Step 3
         *   Get current PTP time
         */
        CHK_FUN_RET(result, rtk_psfp_current_time_get(unitChip, &currentTime));

        /* Step 4
         *   Program GCL to control all queues (7 to 0) on P2 to one-half time open and one-half time close.
         */
        /* This is just an example: Base time delayed by 1.5 seconds compared to PTP time */
        estSecond = ((uint64)currentTime.currentTime.msbSecond << 32) | (uint64)currentTime.currentTime.lsbSecond;
        /* NanoSec overflow */
        if(CVA_NANOSECONDMAX <= (currentTime.currentTime.nanosecond + 500000000u))
        {
            stGateConfig.baseTime.nanosecond = currentTime.currentTime.nanosecond + 500000000u - CVA_NANOSECONDMAX;
            estSecond                        = estSecond + 2u;
        }
        else
        {
            stGateConfig.baseTime.nanosecond = currentTime.currentTime.nanosecond + 500000000u;
            estSecond                        = estSecond + 1u;
        }
        stGateConfig.baseTime.lsbSecond = (uint32)estSecond;
        stGateConfig.baseTime.msbSecond = (uint16)(estSecond >> 32);
        { /* 7 */
            gateControlList[0].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[0].gateState    = 0x80;
            gateControlList[0].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[1].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[1].gateState    = 0x00;
            gateControlList[1].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 6 */
            gateControlList[2].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[2].gateState    = 0x40;
            gateControlList[2].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[3].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[3].gateState    = 0x00;
            gateControlList[3].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 5 */
            gateControlList[4].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[4].gateState    = 0x20;
            gateControlList[4].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[5].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[5].gateState    = 0x00;
            gateControlList[5].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 4 */
            gateControlList[6].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[6].gateState    = 0x10;
            gateControlList[6].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[7].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[7].gateState    = 0x00;
            gateControlList[7].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 3 */
            gateControlList[8].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[8].gateState    = 0x08;
            gateControlList[8].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[9].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[9].gateState    = 0x00;
            gateControlList[9].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 2 */
            gateControlList[10].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[10].gateState    = 0x04;
            gateControlList[10].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[11].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[11].gateState    = 0x00;
            gateControlList[11].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 1 */
            gateControlList[12].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[12].gateState    = 0x02;
            gateControlList[12].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[13].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[13].gateState    = 0x00;
            gateControlList[13].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 0 */
            gateControlList[14].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[14].gateState    = 0x01;
            gateControlList[14].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[15].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[15].gateState    = 0x00;
            gateControlList[15].timeInterval = CVA_QBV_100M_INTERVAL;
        }
        stGateConfig.gateControlList = gateControlList;
        CHK_FUN_RET(result, rtk_st_gate_config_set(unitChip, &stGateConfig));
        /* Step 5
         *   Check the QBV config pending.
         */
        /* Get start time */
        CHK_FUN_RET(result, rtk_timer_get_current(&startTime));

        waitNoPendMaxTime = CVA_TRYTIMEMAX + 1500000u;
        do
        {
            CHK_FUN_RET(result, rtk_st_config_pending_get(unitChip, &stConfigPendingGet));
            /* Get end time */
            CHK_FUN_RET(result, rtk_timer_get_current(&current));
            /* Polling timeout in implementation,
             * CVA_TRYTIMEMAX can be configured according to the user's implementation environment */
        } while((CVA_QBV_PENDING == stConfigPendingGet.configPending) && (waitNoPendMaxTime > (current - startTime)));
        if((current - startTime) == waitNoPendMaxTime)
        {
            /* Timeout processing can be configured according to the user's implementation environment */
            result = CVA_QBV_PENDING_TIMEOUT;
            continue;
        }

        /* [Option] If user wants to check the configuration */
        CHK_FUN_RET(result, cva_tsn_qbv_gate_config_check(unitChip, CVA_QBV_EGRESS_PORT_NUM));

        /* Expected Behaviour
         *   Inject VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially with max throughput
         *   Monitor the number of TX 128-byte VLAN packets on egress-port (P2).
         *   => All packets (with pri 7 to 0) could be transmitted.
         *   => The max throughput of packet rate should be about 50% (or a little lower).
         *   => The VLAN priority of the TX-packet sequence should be 7,6,5,4,3,2,1,0 (without overrun).
         */
    } while(0u == 1u);
    return result;
}
/**@}*/ /* RTL907XCVA_QBV_GATE_CONTROL_LIST_OPEN_EXAMPLE */
/**
 * @defgroup RTL907XCVA_ST_GATE_CONTROL_LIST_HALF_OPEN_EXAMPLE QBV/ST Gate Control List Half Open Example
 *
 * @brief <b> Configure the QBV partially open gate control list. </b>
 *
 * @section Purpose
 *          This example code demonstrates how to set the QBV half-open gate control list configuration.
 *
 * @section Description
 *          In this example, you need to configure PTP, VLAN and QOS first. \n
 *          After all configurations are completed, te behavior is as follows, \n
 *          The host sequentially injects 128-byte VLAN2 packets with VLAN priorities ranging from 0 to 7 into P1. \n
 *          The VLAN priority of the TX-packet sequence should be 7,6,5,4. \n
 *          Note: The maximum time(uint:microsecond) for polling in step 5 is equal to baseTime plus CVA_TRYTIMEMAX, \n
 *          CVA_TRYTIMEMAX is determined by the frequency of the platform,  \n
 *          the reference frequency for the value in this example is 125MHz. \n
 *
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_tsn_qbv_gate_control_list_half_open(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_tsn_qbv_gate_control_list_half_open(uint32 unit)
{
    RtkApiRet  result   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VA};

    PsfpCurrentTimeGet_t     currentTime;
    uint32                   i         = 0;
    uint64                   estSecond = 0;
    StGateControlListParam_t gateControlList[8];
    uint32                   startTime         = 0u;
    uint32                   current           = 0u;
    uint32                   waitNoPendMaxTime = 0u;
    (void)drv_memset(gateControlList, 0, (8u * sizeof(StGateControlListParam_t)));
    (void)drv_memset(&currentTime, 0, sizeof(PsfpCurrentTimeGet_t));
    /* QoS config */

    QosPriSelInfo_t qosPriSel = {
        .qosPriSel = {
            .portPri   = 0,
            .hybridPri = 7,
            .dscpPri   = 0,
            .cvlanPri  = 0,
            .svlanPri  = 0,
        },
    };

    QosCvlanPriRemap_t qosCvlanPriRemap = {
        .idx = 0,
    };
    QosSvlanPriRemap_t qosSvlanPriRemap = {
        .idx = 0,
    };
    /* PTP config */
    PtpMasterEn_t ptpMasterEnInfo = {
        .masterEn = ENABLED,
    };
    PtpSlave0En_t ptpSlave0EnInfo = {
        .slave0En = ENABLED,
    };
    PtpSlave1En_t ptpSlave1EnInfo = {
        .slave1En = ENABLED,
    };
    PtpSlave2En_t ptpSlave2EnInfo = {
        .slave2En = ENABLED,
    };
    PtpHwEnable_t ptpHwEnableInfo = {
        .ptpEnable = ENABLED,
    };
    /* GCL config */
    StConfigPendingGet_t stConfigPendingGet = {
        .portId = CVA_QBV_EGRESS_PORT_NUM,
    };
    StByteCountSet_t stByteCountInfo = {
        .portId    = CVA_QBV_EGRESS_PORT_NUM,
        .portType  = CVA_ST_PORT_TYPE_100BASE_T1,
        .speed     = RTK_PORT_SPEED_100M,
        .macsecIPG = CVA_NO_MACSEC,
    };
    StGateEnableInfo_t stGateEnableInfo = {
        .portId = CVA_QBV_EGRESS_PORT_NUM,
        .enable = ENABLED,

    };
    StGateConfigInfo_t stGateConfig = {
        .portId             = CVA_QBV_EGRESS_PORT_NUM,
        .controlListLength  = 8,
        .cycleTimeExtension = 10000,
    };
    /*
     * Preconfiguration
     *   All ports (P1~11) join the same VLAN (e.g., VLAN2)
     */

    /* Environment configuration
     *   Force to use dot1q priority (QoS)
     *   Preconfigure VLAN and priority to queue mapping (pri:0~7 to que:0~7)
     */
    do
    {

        result = rtk_qos_pri_sel_set(unitChip, &qosPriSel);

        for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
        {
            qosCvlanPriRemap.iPri       = i;
            qosCvlanPriRemap.iDei       = 0;
            qosCvlanPriRemap.mappingPri = i;
            CHK_FUN_RET(result, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
            qosCvlanPriRemap.iDei = 1;
            CHK_FUN_RET(result, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
            qosSvlanPriRemap.oPri       = i;
            qosSvlanPriRemap.oDei       = 0;
            qosSvlanPriRemap.mappingPri = i;
            CHK_FUN_RET(result, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
            qosSvlanPriRemap.oDei = 1;
            CHK_FUN_RET(result, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
        }

        /* Env
         *   Start PTP
         */
        CHK_FUN_RET(result, rtk_ptp_set_master_en(unitChip, &ptpMasterEnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave0_en(unitChip, &ptpSlave0EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave1_en(unitChip, &ptpSlave1EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave2_en(unitChip, &ptpSlave2EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_hw_enable(unitChip, &ptpHwEnableInfo));

        /* Step 1
         *   Enable QBV on P2
         */
        /* User enable gate */
        CHK_FUN_RET(result, rtk_st_gate_enable_set(unitChip, &stGateEnableInfo));

        /* Step 2
         *   Set the byte_count on P2 (100M)
         */
        CHK_FUN_RET(result, rtk_st_byte_count_set(unitChip, &stByteCountInfo));
        /* Step 3
         *   Get current PTP time
         */
        CHK_FUN_RET(result, rtk_psfp_current_time_get(unitChip, &currentTime));

        /* Step 4
         *   Program GCL to control half queues (7,6,5,4) on P2 to one-half open and one-half close.
         */
        /* This is just an example: Base time delayed by 1.5 seconds compared to PTP time */
        estSecond = ((uint64)currentTime.currentTime.msbSecond << 32) | (uint64)currentTime.currentTime.lsbSecond;
        /* NanoSec overflow */
        if(CVA_NANOSECONDMAX <= (currentTime.currentTime.nanosecond + 500000000u))
        {
            stGateConfig.baseTime.nanosecond = currentTime.currentTime.nanosecond + 500000000u - CVA_NANOSECONDMAX;
            estSecond                        = estSecond + 2u;
        }
        else
        {
            stGateConfig.baseTime.nanosecond = currentTime.currentTime.nanosecond + 500000000u;
            estSecond                        = estSecond + 1u;
        }
        stGateConfig.baseTime.lsbSecond = (uint32)estSecond;
        stGateConfig.baseTime.msbSecond = (uint16)(estSecond >> 32);
        { /* 7 */
            gateControlList[0].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[0].gateState    = 0x80;
            gateControlList[0].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[1].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[1].gateState    = 0x00;
            gateControlList[1].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 6 */
            gateControlList[2].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[2].gateState    = 0x40;
            gateControlList[2].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[3].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[3].gateState    = 0x00;
            gateControlList[3].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 5 */
            gateControlList[4].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[4].gateState    = 0x20;
            gateControlList[4].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[5].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[5].gateState    = 0x00;
            gateControlList[5].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 4 */
            gateControlList[6].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[6].gateState    = 0x10;
            gateControlList[6].timeInterval = CVA_QBV_100M_INTERVAL;

            gateControlList[7].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[7].gateState    = 0x00;
            gateControlList[7].timeInterval = CVA_QBV_100M_INTERVAL;
        }
        stGateConfig.gateControlList = gateControlList;
        CHK_FUN_RET(result, rtk_st_gate_config_set(unitChip, &stGateConfig));
        /* Step 5
         *   Check the QBV config pending.
         */
        /* Get start time */
        CHK_FUN_RET(result, rtk_timer_get_current(&startTime));

        waitNoPendMaxTime = CVA_TRYTIMEMAX + 1500000u;
        do
        {
            CHK_FUN_RET(result, rtk_st_config_pending_get(unitChip, &stConfigPendingGet));
            /* Get end time */
            CHK_FUN_RET(result, rtk_timer_get_current(&current));
            /* Polling timeout in implementation,
             * CVA_TRYTIMEMAX can be configured according to the user's implementation environment */
        } while((CVA_QBV_PENDING == stConfigPendingGet.configPending) && (waitNoPendMaxTime > (current - startTime)));
        if((current - startTime) == waitNoPendMaxTime)
        {
            /* Timeout processing can be configured according to the user's implementation environment */
            result = CVA_QBV_PENDING_TIMEOUT;
            continue;
        }
        /* [Option] If user wants to check the configuration */
        CHK_FUN_RET(result, cva_tsn_qbv_gate_config_check(unitChip, CVA_QBV_EGRESS_PORT_NUM));

        /* Expected Behaviour
         *   Inject VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially with max throughput
         *   Monitor the number of TX 128-byte VLAN packets on egress-port (P2).
         *   => Half packets (with pri 7,6,5,4) could be transmitted.
         *   => The max throughput of packet rate should be about 25% (or lower).
         *   => The VLAN priority of the TX-packet sequence should be 7,6,5,4 (without overrun).
         */
    } while(0u == 1u);
    return result;
}
/**@}*/ /* RTL907XCVA_QBV_GATE_CONTROL_LIST_HALF_OPEN_EXAMPLE */
/**
 * @defgroup RTL907XCVA_ST_GATE_CONTROL_LIST_SGOT_EXAMPLE QBV/ST Gate Control List Sgot Example
 *
 * @brief <b> Configure the QBV continuous open gate control list. </b>
 *
 * @section Purpose
 *          This example code demonstrates how to set the QBV continuous open gate control list configuration.
 * @section Description
 *          In this example, you need to configure PTP, VLAN and QOS first. \n
 *          After all configurations are completed, te behavior is as follows, \n
 *          The host sequentially injects 128-byte VLAN2 packets with VLAN priorities ranging from 0 to 7 into P1. \n
 *          The VLAN priority of the TX-packet sequence should be 7,6,5,4,3,2,1,0. \n
 *          Note: The maximum time(uint:microsecond) for polling in step 5 is equal to baseTime plus CVA_TRYTIMEMAX, \n
 *          CVA_TRYTIMEMAX is determined by the frequency of the platform,  \n
 *          the reference frequency for the value in this example is 125MHz \n
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet cva_tsn_qbv_gate_control_list_continuous_open(uint32 unit);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet cva_tsn_qbv_gate_control_list_continuous_open(uint32 unit)
{
    RtkApiRet  result   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = unit,
        .chip = CHIP_907XC_VA};
    PsfpCurrentTimeGet_t     currentTime;
    uint32                   i         = 0;
    uint64                   estSecond = 0;
    StGateControlListParam_t gateControlList[24];
    uint32                   startTime         = 0u;
    uint32                   current           = 0u;
    uint32                   waitNoPendMaxTime = 0u;
    (void)drv_memset(gateControlList, 0, (24u * sizeof(StGateControlListParam_t)));
    (void)drv_memset(&currentTime, 0, sizeof(PsfpCurrentTimeGet_t));
    /* QoS config */

    QosPriSelInfo_t qosPriSel = {
        .qosPriSel = {
            .portPri   = 0,
            .hybridPri = 7,
            .dscpPri   = 0,
            .cvlanPri  = 0,
            .svlanPri  = 0,
        },
    };

    QosCvlanPriRemap_t qosCvlanPriRemap = {
        .idx = 0,
    };
    QosSvlanPriRemap_t qosSvlanPriRemap = {
        .idx = 0,
    };
    /* PTP config */
    PtpMasterEn_t ptpMasterEnInfo = {
        .masterEn = ENABLED,
    };
    PtpSlave0En_t ptpSlave0EnInfo = {
        .slave0En = ENABLED,
    };
    PtpSlave1En_t ptpSlave1EnInfo = {
        .slave1En = ENABLED,
    };
    PtpSlave2En_t ptpSlave2EnInfo = {
        .slave2En = ENABLED,
    };
    PtpHwEnable_t ptpHwEnableInfo = {
        .ptpEnable = ENABLED,
    };
    /* GCL config */
    StConfigPendingGet_t stConfigPendingGet = {
        .portId = CVA_QBV_EGRESS_PORT_NUM,
    };
    StByteCountSet_t stByteCountInfo = {
        .portId    = CVA_QBV_EGRESS_PORT_NUM,
        .portType  = CVA_ST_PORT_TYPE_100BASE_T1,
        .speed     = RTK_PORT_SPEED_100M,
        .macsecIPG = CVA_NO_MACSEC,
    };
    StGateEnableInfo_t stGateEnableInfo = {
        .portId = CVA_QBV_EGRESS_PORT_NUM,
        .enable = ENABLED,

    };
    StGateConfigInfo_t stGateConfig = {
        .portId             = CVA_QBV_EGRESS_PORT_NUM,
        .controlListLength  = 24,
        .cycleTimeExtension = 10000,
    };
    /*
     * Preconfiguration
     *   All ports (P1~11) join the same VLAN (e.g., VLAN2)
     */

    /* Environment configuration
     *   Force to use dot1q priority (QoS)
     *   Preconfigure VLAN and priority to queue mapping (pri:0~7 to que:0~7)
     */
    do
    {

        result = rtk_qos_pri_sel_set(unitChip, &qosPriSel);

        for(i = 0; i != TSN_NUMBER_OF_QUEUE; i++)
        {
            qosCvlanPriRemap.iPri       = i;
            qosCvlanPriRemap.iDei       = 0;
            qosCvlanPriRemap.mappingPri = i;
            CHK_FUN_RET(result, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
            qosCvlanPriRemap.iDei = 1;
            CHK_FUN_RET(result, rtk_qos_cvlan_pri_remap_set(unitChip, &qosCvlanPriRemap));
            qosSvlanPriRemap.oPri       = i;
            qosSvlanPriRemap.oDei       = 0;
            qosSvlanPriRemap.mappingPri = i;
            CHK_FUN_RET(result, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
            qosSvlanPriRemap.oDei = 1;
            CHK_FUN_RET(result, rtk_qos_svlan_pri_remap_set(unitChip, &qosSvlanPriRemap));
        }
        /* Env
         *   Start PTP
         */
        CHK_FUN_RET(result, rtk_ptp_set_master_en(unitChip, &ptpMasterEnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave0_en(unitChip, &ptpSlave0EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave1_en(unitChip, &ptpSlave1EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_slave2_en(unitChip, &ptpSlave2EnInfo));
        CHK_FUN_RET(result, rtk_ptp_set_hw_enable(unitChip, &ptpHwEnableInfo));

        /* Step 1
         *   Enable QBV on P2
         */
        /* User enable gate */
        CHK_FUN_RET(result, rtk_st_gate_enable_set(unitChip, &stGateEnableInfo));

        /* Step 2
         *   Set the byte_count on P2 (100M)
         */
        CHK_FUN_RET(result, rtk_st_byte_count_set(unitChip, &stByteCountInfo));
        /* Step 3
         *   Get current PTP time
         */
        CHK_FUN_RET(result, rtk_psfp_current_time_get(unitChip, &currentTime));

        /* Step 4
         *   Program GCL to control all queues on P2 with one-half sufficient-open-time.
         */
        /* This is just an example: Base time delayed by 1.5 seconds compared to PTP time */
        estSecond = ((uint64)currentTime.currentTime.msbSecond << 32) | (uint64)currentTime.currentTime.lsbSecond;
        /* NanoSec overflow */
        if(CVA_NANOSECONDMAX <= (currentTime.currentTime.nanosecond + 500000000u))
        {
            stGateConfig.baseTime.nanosecond = currentTime.currentTime.nanosecond + 500000000u - CVA_NANOSECONDMAX;
            estSecond                        = estSecond + 2u;
        }
        else
        {
            stGateConfig.baseTime.nanosecond = currentTime.currentTime.nanosecond + 500000000u;
            estSecond                        = estSecond + 1u;
        }
        stGateConfig.baseTime.lsbSecond = (uint32)estSecond;
        stGateConfig.baseTime.msbSecond = (uint16)(estSecond >> 32);

        { /* 7 */
            gateControlList[0].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[0].gateState    = 0x80;
            gateControlList[0].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[1].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[1].gateState    = 0x80;
            gateControlList[1].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[2].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[2].gateState    = 0x00;
            gateControlList[2].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 6 */
            gateControlList[3].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[3].gateState    = 0x40;
            gateControlList[3].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[4].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[4].gateState    = 0x40;
            gateControlList[4].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[5].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[5].gateState    = 0x00;
            gateControlList[5].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 5 */
            gateControlList[6].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[6].gateState    = 0x20;
            gateControlList[6].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[7].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[7].gateState    = 0x20;
            gateControlList[7].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[8].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[8].gateState    = 0x00;
            gateControlList[8].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 4 */
            gateControlList[9].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[9].gateState    = 0x10;
            gateControlList[9].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[10].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[10].gateState    = 0x10;
            gateControlList[10].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[11].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[11].gateState    = 0x00;
            gateControlList[11].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 3 */
            gateControlList[12].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[12].gateState    = 0x08;
            gateControlList[12].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[13].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[13].gateState    = 0x08;
            gateControlList[13].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[14].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[14].gateState    = 0x00;
            gateControlList[14].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 2 */
            gateControlList[15].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[15].gateState    = 0x04;
            gateControlList[15].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[16].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[16].gateState    = 0x04;
            gateControlList[16].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[17].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[17].gateState    = 0x00;
            gateControlList[17].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 1 */
            gateControlList[18].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[18].gateState    = 0x02;
            gateControlList[18].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[19].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[19].gateState    = 0x02;
            gateControlList[19].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[20].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[20].gateState    = 0x00;
            gateControlList[20].timeInterval = CVA_QBV_100M_INTERVAL;

            /* 0 */
            gateControlList[21].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[21].gateState    = 0x01;
            gateControlList[21].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[22].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[22].gateState    = 0x01;
            gateControlList[22].timeInterval = (CVA_QBV_100M_INTERVAL / 2u);

            gateControlList[23].operation    = CVA_ST_GATE_OPERATION_SET_GATE_STATE;
            gateControlList[23].gateState    = 0x00;
            gateControlList[23].timeInterval = CVA_QBV_100M_INTERVAL;
        }
        stGateConfig.gateControlList = gateControlList;
        CHK_FUN_RET(result, rtk_st_gate_config_set(unitChip, &stGateConfig));
        /* Step 5
         *   Check the QBV config pending.
         */
        /* Get start time */
        CHK_FUN_RET(result, rtk_timer_get_current(&startTime));

        waitNoPendMaxTime = CVA_TRYTIMEMAX + 1500000u;
        do
        {
            CHK_FUN_RET(result, rtk_st_config_pending_get(unitChip, &stConfigPendingGet));
            /* Get end time */
            CHK_FUN_RET(result, rtk_timer_get_current(&current));
            /* Polling timeout in implementation,
             * CVA_TRYTIMEMAX can be configured according to the user's implementation environment */
        } while((CVA_QBV_PENDING == stConfigPendingGet.configPending) && (waitNoPendMaxTime > (current - startTime)));
        if((current - startTime) == waitNoPendMaxTime)
        {
            /* Timeout processing can be configured according to the user's implementation environment */
            result = CVA_QBV_PENDING_TIMEOUT;
            continue;
        }
        /* [Option] If user wants to check the configuration */
        CHK_FUN_RET(result, cva_tsn_qbv_gate_config_check(unitChip, CVA_QBV_EGRESS_PORT_NUM));

        /* Expected Behaviour
         *   Inject VLAN2 packets into P1 with VLAN priority 0~7 packets sequentially with max throughput
         *   Monitor the number of TX 128-byte VLAN packets on egress-port (P2).
         *   => All packets (with pri 7 to 0) could be transmitted.
         *   => The max throughput of packet rate should be about 50% (or a little lower).
         *   => The VLAN priority of the TX-packet sequence should be 7,6,5,4,3,2,1,0 (without overrun).
         */
    } while(0u == 1u);
    return result;
}
/**@}*/ /* RTL907XCVA_ST_GATE_CONTROL_LIST_SGOT_EXAMPLE */
/**@}*/ /* RTL907XCVA_ST_EXAMPLE */
/**@}*/ /* ST_EXAMPLE */