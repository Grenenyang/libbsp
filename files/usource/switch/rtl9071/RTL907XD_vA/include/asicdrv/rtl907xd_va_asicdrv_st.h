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

#ifndef DVA_ASICDRV_ST_H
#define DVA_ASICDRV_ST_H

#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_api_st.h"
#include "rtl907xd_va_api_tsn.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtkas_api_st.h"
EXTERN_API RtkApiRet dva_drv_st_gt_conf_set_intf_chk(dva_RtkPort portId, uint32 baseTimeIsOffset, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension,
                                                     StGateControlListParam_t gateControlList[], uint32 controlListLength);
EXTERN_API RtkApiRet dva_drv_st_gt_conf_get_intf_chk(dva_RtkPort portId, RtkTsnPtpTime_t *baseTime, RtkTsnCycleTime_t *cycleTime,
                                                     uint32 *cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 *controlListLength);
EXTERN_API RtkApiRet dva_drv_st_byte_count_intf_chk(uint32 unit, dva_RtkPort portId, uint32 portType, uint32 macsecIPG);

EXTERN_API RtkApiRet dva_drv_st_preconfig_chk(uint32 unit, dva_RtkPort portId);
EXTERN_API RtkApiRet dva_drv_st_isdone_chk(uint32 unit, dva_RtkPort portId);
/* ieee8021STMaxSDUTable */
EXTERN_API RtkApiRet dva_drv_st_queue_max_sdu_get(uint32 unit, dva_RtkPort portId, uint32 queueId, uint32 *queueMaxSDU);
EXTERN_API RtkApiRet dva_drv_st_queue_max_sdu_set(uint32 unit, dva_RtkPort portId, uint32 queueId, uint32 queueMaxSDU);

EXTERN_API RtkApiRet dva_drv_st_byte_count_xmii_set(uint32 unit, dva_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG);
EXTERN_API RtkApiRet dva_drv_st_byte_count_t1_set(uint32 unit, dva_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG);
EXTERN_API RtkApiRet dva_drv_st_byte_count_fe_set(uint32 unit, dva_RtkPort portId, uint32 speed, uint32 macsecIPG);

/* ieee8021STParametersTable */
EXTERN_API RtkApiRet dva_drv_st_gate_hw_enabled_get(uint32 unit, dva_RtkPort portId, uint32 *gateEnabled);
EXTERN_API RtkApiRet dva_drv_st_gate_hw_enabled_set(uint32 unit, dva_RtkPort portId, uint32 gateEnabled, uint32 isImage);
EXTERN_API RtkApiRet dva_drv_st_gate_enabled_get(uint32 unit, dva_RtkPort portId, uint32 *gateEnabled);
EXTERN_API RtkApiRet dva_drv_st_gate_enabled_set(uint32 unit, dva_RtkPort portId, uint32 gateEnabled, uint32 isImage);

EXTERN_API RtkApiRet dva_drv_st_admin_gate_states_get(uint32 unit, dva_RtkPort portId, uint32 *gateState);
EXTERN_API RtkApiRet dva_drv_st_admin_gate_states_set(uint32 unit, dva_RtkPort portId, uint32 gateState);
EXTERN_API RtkApiRet dva_drv_st_oper_gate_states_get(uint32 unit, dva_RtkPort portId, uint32 *gateState);

EXTERN_API RtkApiRet dva_drv_st_gate_config_set(uint32 unit, dva_RtkPort portId, uint32 baseTimeIsOffset, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 controlListLength);
EXTERN_API RtkApiRet dva_drv_st_ad_ctrl_lst_len_get(uint32 unit, dva_RtkPort portId, uint32 *controlListLength);
EXTERN_API RtkApiRet dva_drv_st_ad_ctrl_lst_len_set(uint32 unit, dva_RtkPort portId, uint32 controlListLength);
EXTERN_API RtkApiRet dva_drv_st_op_ctrl_lst_len_get(uint32 unit, dva_RtkPort portId, uint32 *controlListLength);

EXTERN_API RtkApiRet dva_drv_st_admin_ctrl_list_get(uint32 unit, dva_RtkPort portId, StGateControlListParam_t gateControlList[DVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry);
EXTERN_API RtkApiRet dva_drv_st_admin_ctrl_list_set(uint32 unit, dva_RtkPort portId, StGateControlListParam_t gateControlList[DVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry, uint32 isImage);
EXTERN_API RtkApiRet dva_drv_st_oper_ctrl_list_get(uint32 unit, dva_RtkPort portId, StGateControlListParam_t gateControlList[DVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry);

EXTERN_API RtkApiRet dva_drv_st_admin_cycle_time_get(uint32 unit, dva_RtkPort portId, RtkTsnCycleTime_t *cycleTime);
EXTERN_API RtkApiRet dva_drv_st_admin_cycle_time_set(uint32 unit, dva_RtkPort portId, RtkTsnCycleTime_t cycleTime);
EXTERN_API RtkApiRet dva_drv_st_oper_cycle_time_get(uint32 unit, dva_RtkPort portId, RtkTsnCycleTime_t *cycleTime);

EXTERN_API RtkApiRet dva_drv_st_ad_cycle_time_ext_get(uint32 unit, dva_RtkPort portId, uint32 *cycleTimeExtension);
EXTERN_API RtkApiRet dva_drv_st_ad_cycle_time_ext_set(uint32 unit, dva_RtkPort portId, uint32 cycleTimeExtension);
EXTERN_API RtkApiRet dva_drv_st_op_cycle_time_ext_get(uint32 unit, dva_RtkPort portId, uint32 *cycleTimeExtension);

EXTERN_API RtkApiRet dva_drv_st_admin_base_time_get(uint32 unit, dva_RtkPort portId, RtkTsnPtpTime_t *baseTime);
EXTERN_API RtkApiRet dva_drv_st_admin_base_time_set(uint32 unit, dva_RtkPort portId, RtkTsnPtpTime_t *baseTime, uint32 baseTimeIsOffset, uint32 isImage);
EXTERN_API RtkApiRet dva_drv_st_oper_base_time_get(uint32 unit, dva_RtkPort portId, RtkTsnPtpTime_t *baseTime);

EXTERN_API RtkApiRet dva_drv_st_config_change_set(uint32 unit, dva_RtkPort portId, uint32 configChange, uint32 baseTimeIsOffset);
EXTERN_API RtkApiRet dva_drv_st_current_time_get(uint32 unit, RtkTsnPtpTime_t *currentTime);
EXTERN_API RtkApiRet dva_drv_st_config_pending_get(uint32 unit, dva_RtkPort portId, uint32 *configPending);

/*for cle*/
EXTERN_API RtkApiRet dva_drv_st_transmission_overrun_get(uint32 unit, dva_RtkPort portId, uint32 queueId, uint64 *transmissionOverrunCounter);
EXTERN_API RtkApiRet dva_drv_st_consecutive_gate_open_list_get(uint32 unit, dva_RtkPort portId, uint32 instance, dva_RtkStConsecutiveGateOpenEntry_t consecutiveList[DVA_TSN_NUMBER_OF_QUEUE][DVA_ST_SUPPORT_LIST_MAX / 2u]);
EXTERN_API RtkApiRet dva_drv_st_gate_config_image_set(uint32 unit, dva_RtkPort portId, uint32 baseTimeIsOffset, RtkTsnPtpTime_t baseTime, uint32 cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 controlListLength);
EXTERN_API RtkApiRet dva_drv_st_config_change_get(uint32 unit, dva_RtkPort portId, uint32 *configChange);
EXTERN_API RtkApiRet dva_drv_st_config_change_time_get(uint32 unit, dva_RtkPort portId, RtkTsnPtpTime_t *configChangeTime);
EXTERN_API RtkApiRet dva_drv_st_tick_granularity_get(uint32 *tickGranularity);
EXTERN_API RtkApiRet dva_drv_st_config_change_error_get(uint32 unit, dva_RtkPort portId, uint64 *configChangeErrorCounter);
EXTERN_API RtkApiRet dva_drv_st_supported_list_max_get(uint32 *supportedListMax);

#define DVA_BYTE_PRE_FRAME_21  (21u)
#define DVA_BYTE_PRE_FRAME_23  (23u)
#define DVA_BYTE_PRE_FRAME_24  (24u)
#define DVA_BYTE_PRE_FRAME_25  (25u)
#define DVA_BYTE_PRE_FRAME_26  (26u)
#define DVA_BYTE_PRE_FRAME_29  (29u)
#define DVA_BYTE_PRE_FRAME_34  (34u)
#define DVA_BYTE_PRE_FRAME_38  (38u)
#define DVA_BYTE_PRE_FRAME_45  (45u)
#define DVA_BYTE_PRE_FRAME_47  (47u)
#define DVA_BYTE_PRE_FRAME_49  (49u)
#define DVA_BYTE_PRE_FRAME_53  (53u)
#define DVA_BYTE_PRE_FRAME_55  (55u)
#define DVA_BYTE_PRE_FRAME_56  (56u)
#define DVA_BYTE_PRE_FRAME_57  (57u)
#define DVA_BYTE_PRE_FRAME_61  (61u)
#define DVA_BYTE_PRE_FRAME_65  (65u)
#define DVA_BYTE_PRE_FRAME_67  (67u)
#define DVA_BYTE_PRE_FRAME_78  (78u)
#define DVA_BYTE_PRE_FRAME_82  (82u)
#define DVA_BYTE_PRE_FRAME_83  (83u)
#define DVA_BYTE_PRE_FRAME_101 (101u)
#define DVA_BYTE_PRE_FRAME_111 (111u)
#define DVA_BYTE_PRE_FRAME_112 (112u)
#define DVA_BYTE_PRE_FRAME_116 (116u)
#define DVA_BYTE_PRE_FRAME_126 (126u)
#define DVA_BYTE_PRE_FRAME_127 (127u)
#define DVA_BYTE_PRE_FRAME_128 (128u)
#define DVA_BYTE_PRE_FRAME_132 (132u)
#define DVA_BYTE_PRE_FRAME_134 (134u)
#define DVA_BYTE_PRE_FRAME_209 (209u)
#define DVA_BYTE_PRE_FRAME_214 (214u)
#define DVA_BYTE_PRE_FRAME_217 (217u)
#define DVA_BYTE_PRE_FRAME_219 (219u)
#define DVA_BYTE_PRE_FRAME_222 (222u)
#define DVA_BYTE_PRE_FRAME_233 (233u)
#define DVA_BYTE_PRE_FRAME_236 (236u)
#define DVA_BYTE_PRE_FRAME_238 (238u)
#define DVA_BYTE_PRE_FRAME_245 (245u)
#define DVA_BYTE_PRE_FRAME_265 (265u)
#define DVA_BYTE_PRE_FRAME_270 (270u)
#define DVA_BYTE_PRE_FRAME_362 (362u)
#define DVA_BYTE_PRE_FRAME_367 (367u)
/* BR */
#define DVA_TX_10M   (0u)
#define DVA_TX_100M  (1u)
#define DVA_TX_1000M (2u)
/* RTPG */
#define DVA_RTPG_1GMAC_100M    (3u)
#define DVA_RTPG_1GMAC_1000M   (4u)
#define DVA_RTPG_2_5GMAC_100M  (5u)
#define DVA_RTPG_2_5GMAC_1000M (6u)
/* xMII */
#define DVA_RGMII_1GMAC_1000M   (7u)
#define DVA_RGMII_1GMAC_100M    (8u)
#define DVA_RGMII_1GMAC_10M     (9u)
#define DVA_MII_1GMAC_100M      (10u)
#define DVA_MII_1GMAC_10M       (11u)
#define DVA_RMII_1GMAC_100M     (12u)
#define DVA_RMII_1GMAC_10M      (13u)
#define DVA_RGMII_2_5GMAC_1000M (14u)
#define DVA_RGMII_2_5GMAC_100M  (15u)
#define DVA_RGMII_2_5GMAC_10M   (16u)
#define DVA_MII_2_5GMAC_100M    (17u)
#define DVA_MII_2_5GMAC_10M     (18u)
#define DVA_RMII_2_5GMAC_100M   (19u)
#define DVA_RMII_2_5GMAC_10M    (20u)
/* SGMII */
#define DVA_SGMII_2_5GMAC_1000M (21u)
#define DVA_SGMII_2_5GMAC_100M  (22u)
#define DVA_SGMII_2_5GMAC_10M   (23u)
#define DVA_SGMII_10GMAC_1000M  (24u)
#define DVA_SGMII_10GMAC_100M   (25u)
#define DVA_SGMII_10GMAC_10M    (26u)
/*USXG*/
#define DVA_USXG_10GMAC_10000M (27u)
#define DVA_USXG_10GMAC_5000M  (28u)
#define DVA_USXG_10GMAC_2500M  (29u)
/*HISG*/
#define DVA_HISG_10GMAC_2500M  (30u)
#define DVA_HISG_2_5GMAC_2500M (31u)
/* KR */
#define DVA_KR_10GMAC_5000M  (32u)
#define DVA_KR_10GMAC_10000M (33u)
/* XFI */
#define DVA_XFI_10GMAC_5000M  (34u)
#define DVA_XFI_10GMAC_10000M (35u)

#define DVA_ST_DELAY_TYPE_LIST_END (36u)

typedef struct
{
    uint32 delaywithNoIPG;
    uint32 delaywithIPG24;
    uint32 delaywithIPG32;

    uint32 bcwithNoIPG;
    uint32 bcwithIPG24;
    uint32 bcwithIPG32;
} dva_stDelayVal_t;
/* P7 */
#define DVA_CYCLE_DELAY_TX_10M   (6483u)
#define DVA_CYCLE_DELAY_TX_100M  (667u)
#define DVA_CYCLE_DELAY_TX_1000M (69u)
/* P1/P2/p3 */
#define DVA_CYCLE_DELAY_RTPG_1GMAC_100M            (669u)
#define DVA_CYCLE_DELAY_RTPG_1GMAC_100M_WITH_IPG24 (3120u)
#define DVA_CYCLE_DELAY_RTPG_1GMAC_100M_WITH_IPG32 (3103u)

#define DVA_CYCLE_DELAY_RTPG_1GMAC_1000M            (67u)
#define DVA_CYCLE_DELAY_RTPG_1GMAC_1000M_WITH_IPG24 (314u)
#define DVA_CYCLE_DELAY_RTPG_1GMAC_1000M_WITH_IPG32 (309u)
/* P4/P5/P6 */
#define DVA_CYCLE_DELAY_RTPG_2_5GMAC_100M            (665u)
#define DVA_CYCLE_DELAY_RTPG_2_5GMAC_100M_WITH_IPG24 (3021u)
#define DVA_CYCLE_DELAY_RTPG_2_5GMAC_100M_WITH_IPG32 (3027u)

#define DVA_CYCLE_DELAY_RTPG_2_5GMAC_1000M            (68u)
#define DVA_CYCLE_DELAY_RTPG_2_5GMAC_1000M_WITH_IPG24 (315u)
#define DVA_CYCLE_DELAY_RTPG_2_5GMAC_1000M_WITH_IPG32 (315u)
/* P3 RGMII */
#define DVA_CYCLE_DELAY_RGMII_1GMAC_1000M            (69u)
#define DVA_CYCLE_DELAY_RGMII_1GMAC_1000M_WITH_IPG24 (321u)
#define DVA_CYCLE_DELAY_RGMII_1GMAC_1000M_WITH_IPG32 (307u)

#define DVA_CYCLE_DELAY_RGMII_1GMAC_100M            (669u)
#define DVA_CYCLE_DELAY_RGMII_1GMAC_100M_WITH_IPG24 (3095u)
#define DVA_CYCLE_DELAY_RGMII_1GMAC_100M_WITH_IPG32 (3118u)

#define DVA_CYCLE_DELAY_RGMII_1GMAC_10M            (6279u)
#define DVA_CYCLE_DELAY_RGMII_1GMAC_10M_WITH_IPG24 (29873u)
#define DVA_CYCLE_DELAY_RGMII_1GMAC_10M_WITH_IPG32 (29875u)
/* P3 MII */
#define DVA_CYCLE_DELAY_MII_1GMAC_100M            (669u)
#define DVA_CYCLE_DELAY_MII_1GMAC_100M_WITH_IPG24 (3102u)
#define DVA_CYCLE_DELAY_MII_1GMAC_100M_WITH_IPG32 (3102u)

#define DVA_CYCLE_DELAY_MII_1GMAC_10M            (6329u)
#define DVA_CYCLE_DELAY_MII_1GMAC_10M_WITH_IPG24 (30017u)
#define DVA_CYCLE_DELAY_MII_1GMAC_10M_WITH_IPG32 (30018u)
/* P3 RMII */
#define DVA_CYCLE_DELAY_RMII_1GMAC_100M            (669u)
#define DVA_CYCLE_DELAY_RMII_1GMAC_100M_WITH_IPG24 (3103u)
#define DVA_CYCLE_DELAY_RMII_1GMAC_100M_WITH_IPG32 (3101u)

#define DVA_CYCLE_DELAY_RMII_1GMAC_10M            (6303u)
#define DVA_CYCLE_DELAY_RMII_1GMAC_10M_WITH_IPG24 (29903u)
#define DVA_CYCLE_DELAY_RMII_1GMAC_10M_WITH_IPG32 (30063u)
/* P7/8 xMII */
#define DVA_CYCLE_DELAY_RGMII_2_5GMAC_1000M (68u)
#define DVA_CYCLE_DELAY_RGMII_2_5GMAC_100M  (642u)
#define DVA_CYCLE_DELAY_RGMII_2_5GMAC_10M   (6239u)
#define DVA_CYCLE_DELAY_MII_2_5GMAC_100M    (645u)
#define DVA_CYCLE_DELAY_MII_2_5GMAC_10M     (6355u)
#define DVA_CYCLE_DELAY_RMII_2_5GMAC_100M   (647u)
#define DVA_CYCLE_DELAY_RMII_2_5GMAC_10M    (6271u)
/* P4/5/6/7/8 SGMII */
#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_1000M            (68u)
#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_1000M_WITH_IPG24 (304u)
#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_1000M_WITH_IPG32 (301u)

#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_100M            (669u)
#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_100M_WITH_IPG24 (3041u)
#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_100M_WITH_IPG32 (3032u)

#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_10M            (6284u)
#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_10M_WITH_IPG24 (29844u)
#define DVA_CYCLE_DELAY_SGMII_2_5GMAC_10M_WITH_IPG32 (29826u)
/* P9/10/11/12 SGMII */
#define DVA_CYCLE_DELAY_SGMII_10GMAC_1000M            (40u)
#define DVA_CYCLE_DELAY_SGMII_10GMAC_1000M_WITH_IPG24 (278u)
#define DVA_CYCLE_DELAY_SGMII_10GMAC_1000M_WITH_IPG32 (282u)

#define DVA_CYCLE_DELAY_SGMII_10GMAC_100M            (323u)
#define DVA_CYCLE_DELAY_SGMII_10GMAC_100M_WITH_IPG24 (2717u)
#define DVA_CYCLE_DELAY_SGMII_10GMAC_100M_WITH_IPG32 (2721u)

#define DVA_CYCLE_DELAY_SGMII_10GMAC_10M            (3162u)
#define DVA_CYCLE_DELAY_SGMII_10GMAC_10M_WITH_IPG24 (27208u)
#define DVA_CYCLE_DELAY_SGMII_10GMAC_10M_WITH_IPG32 (27217u)
/* P9/10/11/12 USXG */
#define DVA_CYCLE_DELAY_USXG_10GMAC_10G            (28u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_10G_WITH_IPG24 (174u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_10G_WITH_IPG32 (174u)

#define DVA_CYCLE_DELAY_USXG_10GMAC_5G            (52u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_5G_WITH_IPG24 (341u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_5G_WITH_IPG32 (340u)

#define DVA_CYCLE_DELAY_USXG_10GMAC_2_5G            (97u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_2_5G_WITH_IPG24 (677u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_2_5G_WITH_IPG32 (681u)

#define DVA_CYCLE_DELAY_USXG_10GMAC_1G            (40u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_1G_WITH_IPG24 (277u)
#define DVA_CYCLE_DELAY_USXG_10GMAC_1G_WITH_IPG32 (284u)
/* P9/10/11/12 HISG */
#define DVA_CYCLE_DELAY_HISG_10GMAC_2_5G            (21u)
#define DVA_CYCLE_DELAY_HISG_10GMAC_2_5G_WITH_IPG24 (119u)
#define DVA_CYCLE_DELAY_HISG_10GMAC_2_5G_WITH_IPG32 (120u)

#define DVA_CYCLE_DELAY_HISG_2_5GMAC_2_5G            (33u)
#define DVA_CYCLE_DELAY_HISG_2_5GMAC_2_5G_WITH_IPG24 (134u)
#define DVA_CYCLE_DELAY_HISG_2_5GMAC_2_5G_WITH_IPG32 (126u)

/* P9/10/11/12 KR */
#define DVA_CYCLE_DELAY_KR_10GMAC_5G            (51u)
#define DVA_CYCLE_DELAY_KR_10GMAC_5G_WITH_IPG24 (341u)
#define DVA_CYCLE_DELAY_KR_10GMAC_5G_WITH_IPG32 (340u)

#define DVA_CYCLE_DELAY_KR_10GMAC_10G            (29u)
#define DVA_CYCLE_DELAY_KR_10GMAC_10G_WITH_IPG24 (174u)
#define DVA_CYCLE_DELAY_KR_10GMAC_10G_WITH_IPG32 (174u)

/* P9/10/11/12 XFI */
#define DVA_CYCLE_DELAY_XFI_10GMAC_5G            (51u)
#define DVA_CYCLE_DELAY_XFI_10GMAC_5G_WITH_IPG24 (341u)
#define DVA_CYCLE_DELAY_XFI_10GMAC_5G_WITH_IPG32 (340u)

#define DVA_CYCLE_DELAY_XFI_10GMAC_10G            (29u)
#define DVA_CYCLE_DELAY_XFI_10GMAC_10G_WITH_IPG24 (174u)
#define DVA_CYCLE_DELAY_XFI_10GMAC_10G_WITH_IPG32 (174u)

/* 64 */
#define DVA_ST_MIN_GATE_INTERVAL  (64u)
#define DVA_ST_TICK_IN_NANOSECOND (4u)
#define DVA_ST_TICK_GRANULARITY   (80u)
#define DVA_ST_MAX_CYCLE_TIME     (4000000000u)
/* SGOT */
#define DVA_SGOT_DATAREG_CNT (16u)
/* for st sram address */
#define DVA_ST_CONFIG_DONE_ADDRESS       (0x010FE80Cu)
#define DVA_ST_NEED_CFGMSK_ADDRESS       (0x0113E9C8u)
#define DVA_ST_BASETIME_ISOFFSET_ADDRESS (0x010FEC08u)
/* for base time offset */
#define DVA_ST_OFFSET_BASETIME (1u)
#define DVA_ST_PTP_BASETIME    (0u)
/* DVA_ASICDRV_ST_H */
#endif
