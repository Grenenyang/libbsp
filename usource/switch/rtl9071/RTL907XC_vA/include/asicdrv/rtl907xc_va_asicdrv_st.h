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

#ifndef CVA_ASICDRV_ST_H
#define CVA_ASICDRV_ST_H

#include "rtl907xc_va_api_st.h"
#include "rtl907xc_va_api_tsn.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_table_struct.h"
#include "rtkas_api_st.h"
EXTERN_API RtkApiRet cva_drv_st_gt_conf_set_intf_chk(cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension,
                                                     const StGateControlListParam_t gateControlList[], uint32 controlListLength);
EXTERN_API RtkApiRet cva_drv_st_gt_conf_get_intf_chk(cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, RtkTsnCycleTime_t *cycleTime,
                                                     uint32 *cycleTimeExtension, StGateControlListParam_t gateControlList[], uint32 *controlListLength);
EXTERN_API RtkApiRet cva_drv_st_byte_count_intf_chk(uint32 unit, cva_RtkPort portId, uint32 portType, uint32 macsecIPG);
EXTERN_API RtkApiRet cva_drv_st_preconfig_chk(uint32 unit);
EXTERN_API RtkApiRet cva_drv_st_isdone_chk(uint32 unit);
/* ieee8021STMaxSDUTable */
EXTERN_API RtkApiRet cva_drv_st_queue_max_sdu_get(uint32 unit, cva_RtkPort portId, uint32 queueId, uint32 *queueMaxSDU);
EXTERN_API RtkApiRet cva_drv_st_queue_max_sdu_set(uint32 unit, cva_RtkPort portId, uint32 queueId, uint32 queueMaxSDU);

EXTERN_API RtkApiRet cva_drv_st_byte_count_xmii_set(uint32 unit, cva_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG);
EXTERN_API RtkApiRet cva_drv_st_byte_count_t1_set(uint32 unit, cva_RtkPort portId, uint32 portType, uint32 speed, uint32 macsecIPG);
EXTERN_API RtkApiRet cva_drv_st_byte_count_fe_set(uint32 unit, cva_RtkPort portId, uint32 speed, uint32 macsecIPG);

/* ieee8021STParametersTable */
EXTERN_API RtkApiRet cva_drv_st_gate_hw_enabled_get(uint32 unit, cva_RtkPort portId, uint32 *gateEnabled);
EXTERN_API RtkApiRet cva_drv_st_gate_hw_enabled_set(uint32 unit, cva_RtkPort portId, uint32 gateEnabled, uint32 isImage);
EXTERN_API RtkApiRet cva_drv_st_gate_enabled_get(uint32 unit, cva_RtkPort portId, uint32 *gateEnabled);
EXTERN_API RtkApiRet cva_drv_st_gate_enabled_set(uint32 unit, cva_RtkPort portId, uint32 gateEnabled, uint32 isImage);

EXTERN_API RtkApiRet cva_drv_st_admin_gate_states_get(uint32 unit, cva_RtkPort portId, uint32 *gateState);
EXTERN_API RtkApiRet cva_drv_st_admin_gate_states_set(uint32 unit, cva_RtkPort portId, uint32 gateState);
EXTERN_API RtkApiRet cva_drv_st_oper_gate_states_get(uint32 unit, cva_RtkPort portId, uint32 *gateState);

EXTERN_API RtkApiRet cva_drv_st_gate_config_set(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime, uint32 cycleTimeExtension, const StGateControlListParam_t gateControlList[], uint32 controlListLength);
EXTERN_API RtkApiRet cva_drv_st_ad_ctrl_lst_len_get(uint32 unit, cva_RtkPort portId, uint32 *controlListLength);
EXTERN_API RtkApiRet cva_drv_st_ad_ctrl_lst_len_set(uint32 unit, cva_RtkPort portId, uint32 controlListLength);
EXTERN_API RtkApiRet cva_drv_st_op_ctrl_lst_len_get(uint32 unit, cva_RtkPort portId, uint32 *controlListLength);

EXTERN_API RtkApiRet cva_drv_st_admin_ctrl_list_get(uint32 unit, cva_RtkPort portId, StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry);
EXTERN_API RtkApiRet cva_drv_st_admin_ctrl_list_set(uint32 unit, cva_RtkPort portId, const StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry, uint32 isImage);
EXTERN_API RtkApiRet cva_drv_st_oper_ctrl_list_get(uint32 unit, cva_RtkPort portId, StGateControlListParam_t gateControlList[CVA_ST_SUPPORT_LIST_MAX], uint32 numberOfEntry);

EXTERN_API RtkApiRet cva_drv_st_admin_cycle_time_get(uint32 unit, cva_RtkPort portId, RtkTsnCycleTime_t *cycleTime);
EXTERN_API RtkApiRet cva_drv_st_admin_cycle_time_set(uint32 unit, cva_RtkPort portId, RtkTsnCycleTime_t cycleTime);
EXTERN_API RtkApiRet cva_drv_st_oper_cycle_time_get(uint32 unit, cva_RtkPort portId, RtkTsnCycleTime_t *cycleTime);

EXTERN_API RtkApiRet cva_drv_st_ad_cycle_time_ext_get(uint32 unit, cva_RtkPort portId, uint32 *cycleTimeExtension);
EXTERN_API RtkApiRet cva_drv_st_ad_cycle_time_ext_set(uint32 unit, cva_RtkPort portId, uint32 cycleTimeExtension);
EXTERN_API RtkApiRet cva_drv_st_op_cycle_time_ext_get(uint32 unit, cva_RtkPort portId, uint32 *cycleTimeExtension);

EXTERN_API RtkApiRet cva_drv_st_admin_base_time_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime);
EXTERN_API RtkApiRet cva_drv_st_admin_base_time_set(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime);
EXTERN_API RtkApiRet cva_drv_st_oper_base_time_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *baseTime);

EXTERN_API RtkApiRet cva_drv_st_config_change_set(uint32 unit, cva_RtkPort portId, uint32 configChange);
EXTERN_API RtkApiRet cva_drv_st_current_time_get(uint32 unit, RtkTsnPtpTime_t *currentTime);
EXTERN_API RtkApiRet cva_drv_st_config_pending_get(uint32 unit, cva_RtkPort portId, uint32 *configPending);

/*for cle*/
EXTERN_API RtkApiRet cva_drv_st_transmission_overrun_get(uint32 unit, cva_RtkPort portId, uint32 queueId, uint64 *transmissionOverrunCounter);
EXTERN_API RtkApiRet cva_drv_st_consecutive_gate_open_list_get(uint32 unit, cva_RtkPort portId, uint32 instance, cva_RtkStConsecutiveGateOpenEntry_t consecutiveList[CVA_TSN_NUMBER_OF_QUEUE][CVA_ST_SUPPORT_LIST_MAX / 2u]);
EXTERN_API RtkApiRet cva_drv_st_gate_config_image_set(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t baseTime, uint32 cycleTimeExtension, const StGateControlListParam_t gateControlList[], uint32 controlListLength);
EXTERN_API RtkApiRet cva_drv_st_config_change_get(uint32 unit, cva_RtkPort portId, uint32 *configChange);
EXTERN_API RtkApiRet cva_drv_st_config_change_time_get(uint32 unit, cva_RtkPort portId, RtkTsnPtpTime_t *configChangeTime);
EXTERN_API RtkApiRet cva_drv_st_tick_granularity_get(uint32 *tickGranularity);
EXTERN_API RtkApiRet cva_drv_st_config_change_error_get(uint32 unit, cva_RtkPort portId, uint64 *configChangeErrorCounter);
EXTERN_API RtkApiRet cva_drv_st_supported_list_max_get(uint32 *supportedListMax);

#define CVA_BYTE_PRE_FRAME_10 (10u)
#define CVA_BYTE_PRE_FRAME_11 (11u)
#define CVA_BYTE_PRE_FRAME_13 (13u)
#define CVA_BYTE_PRE_FRAME_14 (14u)
#define CVA_BYTE_PRE_FRAME_19 (19u)
#define CVA_BYTE_PRE_FRAME_22 (22u)
#define CVA_BYTE_PRE_FRAME_23 (23u)
#define CVA_BYTE_PRE_FRAME_30 (30u)
#define CVA_BYTE_PRE_FRAME_31 (31u)
#define CVA_BYTE_PRE_FRAME_37 (37u)
#define CVA_BYTE_PRE_FRAME_38 (38u)
#define CVA_BYTE_PRE_FRAME_39 (39u)
#define CVA_BYTE_PRE_FRAME_41 (41u)
#define CVA_BYTE_PRE_FRAME_42 (42u)
#define CVA_BYTE_PRE_FRAME_45 (45u)
#define CVA_BYTE_PRE_FRAME_46 (46u)
#define CVA_BYTE_PRE_FRAME_59 (59u)
#define CVA_BYTE_PRE_FRAME_92 (92u)

#define CVA_CYCLE_DELAY_BRPHY_1GMAC_100M            (349u)
#define CVA_CYCLE_DELAY_WITH_IPG24_BRPHY_1GMAC_100M (1689u)
#define CVA_CYCLE_DELAY_WITH_IPG32_BRPHY_1GMAC_100M (1689u)
#define CVA_CYCLE_DELAY_BRPHY_1GMAC_10M             (3108u)
#define CVA_CYCLE_DELAY_WITH_IPG24_BRPHY_1GMAC_10M  (16508u)
#define CVA_CYCLE_DELAY_WITH_IPG32_BRPHY_1GMAC_10M  (16508u)
#define CVA_CYCLE_DELAY_FE_1GMAC_100M               (340u)
#define CVA_CYCLE_DELAY_WITH_IPG24_FE_1GMAC_100M    (1680u)
#define CVA_CYCLE_DELAY_WITH_IPG32_FE_1GMAC_100M    (1680u)
#define CVA_CYCLE_DELAY_FE_1GMAC_10M                (3108u)
#define CVA_CYCLE_DELAY_WITH_IPG24_FE_1GMAC_10M     (16508u)
#define CVA_CYCLE_DELAY_WITH_IPG32_FE_1GMAC_10M     (16508u)

#define CVA_CYCLE_DELAY_MII_1GMAC_1000M            (40u)
#define CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_1000M (174u)
#define CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_1000M (174u)
#define CVA_CYCLE_DELAY_MII_1GMAC_100M             (318u)
#define CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_100M  (1658u)
#define CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_100M  (1658u)
#define CVA_CYCLE_DELAY_MII_1GMAC_10M              (3108u)
#define CVA_CYCLE_DELAY_WITH_IPG24_MII_1GMAC_10M   (16508u)
#define CVA_CYCLE_DELAY_WITH_IPG32_MII_1GMAC_10M   (16508u)

#define CVA_CYCLE_DELAY_MII_5GMAC_1000M            (18u)
#define CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_1000M (138u)
#define CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_1000M (138u)
#define CVA_CYCLE_DELAY_MII_5GMAC_100M             (98u)
#define CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_100M  (1298u)
#define CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_100M  (1298u)
#define CVA_CYCLE_DELAY_MII_5GMAC_10M              (908u)
#define CVA_CYCLE_DELAY_WITH_IPG24_MII_5GMAC_10M   (12908u)
#define CVA_CYCLE_DELAY_WITH_IPG32_MII_5GMAC_10M   (12908u)

#define CVA_CYCLE_DELAY_RTPG_1GMAC_1000M            (69u)
#define CVA_CYCLE_DELAY_WITH_IPG24_RTPG_1GMAC_1000M (203u)
#define CVA_CYCLE_DELAY_WITH_IPG32_RTPG_1GMAC_1000M (203u)
#define CVA_CYCLE_DELAY_RTPG_1GMAC_100M             (351u)
#define CVA_CYCLE_DELAY_WITH_IPG24_RTPG_1GMAC_100M  (1691u)
#define CVA_CYCLE_DELAY_WITH_IPG32_RTPG_1GMAC_100M  (1691u)

#define CVA_CYCLE_DELAY_SGMII_1GMAC_1000M            (60u)
#define CVA_CYCLE_DELAY_WITH_IPG24_SGMII_1GMAC_1000M (194u)
#define CVA_CYCLE_DELAY_WITH_IPG32_SGMII_1GMAC_1000M (194u)
#define CVA_CYCLE_DELAY_SGMII_1GMAC_100M             (342u)
#define CVA_CYCLE_DELAY_WITH_IPG24_SGMII_1GMAC_100M  (1682u)
#define CVA_CYCLE_DELAY_WITH_IPG32_SGMII_1GMAC_100M  (1682u)
#define CVA_CYCLE_DELAY_SGMII_1GMAC_10M              (3108u)
#define CVA_CYCLE_DELAY_WITH_IPG24_SGMII_1GMAC_10M   (16508u)
#define CVA_CYCLE_DELAY_WITH_IPG32_SGMII_1GMAC_10M   (16508u)

#define CVA_CYCLE_DELAY_SGMI_5GMAC_1000M            (38u)
#define CVA_CYCLE_DELAY_WITH_IPG24_SGMI_5GMAC_1000M (158u)
#define CVA_CYCLE_DELAY_WITH_IPG32_SGMI_5GMAC_1000M (158u)
#define CVA_CYCLE_DELAY_SGMI_5GMAC_100M             (153u)
#define CVA_CYCLE_DELAY_WITH_IPG24_SGMI_5GMAC_100M  (1353u)
#define CVA_CYCLE_DELAY_WITH_IPG32_SGMI_5GMAC_100M  (1353u)
#define CVA_CYCLE_DELAY_SGMI_5GMAC_10M              (908u)
#define CVA_CYCLE_DELAY_WITH_IPG24_SGMI_5GMAC_10M   (12908u)
#define CVA_CYCLE_DELAY_WITH_IPG32_SGMI_5GMAC_10M   (12908u)

#define CVA_CYCLE_DELAY_HSG_5GMAC            (38u)
#define CVA_CYCLE_DELAY_WITH_IPG24_HSG_5GMAC (86u)
#define CVA_CYCLE_DELAY_WITH_IPG32_HSG_5GMAC (86u)

#define CVA_CYCLE_DELAY_USXG_5GMAC_5000M            (24u)
#define CVA_CYCLE_DELAY_WITH_IPG24_USXG_5GMAC_5000M (166u)
#define CVA_CYCLE_DELAY_WITH_IPG32_USXG_5GMAC_5000M (166u)
#define CVA_CYCLE_DELAY_USXG_5GMAC_2500M            (40u)
#define CVA_CYCLE_DELAY_WITH_IPG24_USXG_5GMAC_2500M (325u)
#define CVA_CYCLE_DELAY_WITH_IPG32_USXG_5GMAC_2500M (325u)
#define CVA_CYCLE_DELAY_USXG_5GMAC_1000M            (18u)
#define CVA_CYCLE_DELAY_WITH_IPG24_USXG_5GMAC_1000M (138u)
#define CVA_CYCLE_DELAY_WITH_IPG32_USXG_5GMAC_1000M (138u)

/* 64 */
#define CVA_ST_MIN_GATE_INTERVAL  (64u)
#define CVA_ST_TICK_IN_NANOSECOND (8u)
#define CVA_ST_TICK_GRANULARITY   (80u)
#define CVA_ST_MAX_CYCLE_TIME     (4000000000u)
/* for st sram address */
#define CVA_ST_CONFIG_DONE_ADDRESS (0x001B0E58u)
/* CVA_ASICDRV_ST_H */
#endif
