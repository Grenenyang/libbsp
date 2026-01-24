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

#ifndef CVA_RTK_API_H__
#define CVA_RTK_API_H__

/*
 * Include Files
 */
#include "rtkas_endian.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_timer.h"
#include "rtl907xc_va_asicdrv.h"

#define CVA_RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST 1
#define CVA_RTK_MAX_NUM_OF_PROTO_TYPE                0xFFFF
#define CVA_RTK_MAX_PORT_MASK                        0xFFFFu

#define CVA_RTK_WRAP_CHECK_32 (0xFFFFFFFFu)

/* indirect channel type */
#define CVA_RTK_INDIRECT_CHANNEL_PRIMARY   (0u)
#define CVA_RTK_INDIRECT_CHANNEL_SECONDARY (1u)

/* wol event */
#define CVA_WOL_EVENT_LINKUP        (0x1)
#define CVA_WOL_EVENT_MAGIC_PKT     (0x2)
#define CVA_WOL_EVENT_WAKEUP_PKT    (0x4)
#define CVA_WOL_EVENT_ANY_PKT_PORT5 (0x8)  /* Valid only when port 5 is configured as cpu port */
#define CVA_WOL_EVENT_ANY_PKT_PORT6 (0x10) /* Valid only when port 6 is configured as cpu port */

#ifndef CVA_MAC_ADDR_LEN
#define CVA_MAC_ADDR_LEN (6u)
#endif

#define CVA_IPV6_ADDR_LEN (16u)
#define CVA_IPV4_ADDR_LEN (4u)

#define CVA_ALLPORT 0xFF

typedef uint32 cva_RtkEnable;
#define CVA_DISABLED       (0u) /* Disable function */
#define CVA_ENABLED        (1u) /* Enable function */
#define CVA_RTK_ENABLE_END (2u)

typedef uint32 cva_RtkPort;

/* unauth pkt action */

/* cfg_spi_para_idx */
#define CVA_CFG_25UA_IDX            (0u)
#define CVA_CFG_OPFSM_IDX           (1u)
#define CVA_CFG_BR_PATCH_1_IDX      (2u)
#define CVA_CFG_BR_PATCH_2_IDX      (3u)
#define CVA_CFG_SGMII_PATCH_IDX     (4u)
#define CVA_CFG_FE_PATCH_IDX        (5u)
#define CVA_CFG_MAC_CONF_IDX        (6u)
#define CVA_CFG_TABLE_IDX           (7u)
#define CVA_CFG_FWCFG_IDX           (8u)
#define CVA_CFG_RTPG_PATCH_1_IDX    (9u)
#define CVA_CFG_RTPG_PATCH_2_IDX    (10u)
#define CVA_CFG_STACKING_IDX        (11u)
#define CVA_CFG_PCIE_IDX            (12u)
#define CVA_CFG_FWCFG_BYTE_IDX      (13u)
#define CVA_CFG_REG_FIELD_WR_IDX    (14u)
#define CVA_CFG_MKA_IDX             (15u)
#define CVA_CFG_ROT_SW_TABLE        (16u)
#define CVA_CFG_ROT_SW_INTF_TABLE   (17u)
#define CVA_CFG_MACPHY_0_CONFIG_IDX (18u)
#define CVA_CFG_MACPHY_1_CONFIG_IDX (19u)
#define CVA_CFG_PTP_IDX             (20u)
#define CVA_CFG_TBL_BASE_IDX        (21u)
#define CVA_CFG_TBL_END_IDX         (CVA_CFG_TBL_BASE_IDX + (uint8)CVA_TABLE_LIST_END - 1u)
#define CVA_CFG_QCC_PRE_TBL_IDX     (61u)
#define CVA_CFG_5KR_PATCH_IDX       (62u)
#define CVA_CFG_END_IDX             (63u)

#define CVA_EFU_25UA_IDX         (0u)
#define CVA_EFU_OPFSM_IDX        (1u)
#define CVA_EFU_FUSEOUT_IDX      (2u)
#define CVA_EFU_BR_PATCH_1_IDX   (3u)
#define CVA_EFU_BR_PATCH_2_IDX   (4u)
#define CVA_EFU_SGMII_PATCH_IDX  (5u)
#define CVA_EFU_FE_PATCH_IDX     (6u)
#define CVA_EFU_MAC_CONF_IDX     (7u)
#define CVA_EFU_RTPG_PATCH_1_IDX (8u)
#define CVA_EFU_RTPG_PATCH_2_IDX (9u)
#define CVA_EFU_SGMII_FIX_IDX    (10u)
#define CVA_EFU_PCIE_IDX         (11u)
#define CVA_EFU_STACKING_IDX     (12u)
#define CVA_EFU_FWCFG_BYTE_IDX   (13u)
#define CVA_EFU_END_IDX          (14u)

#define CVA_EFUSE_END_IDX CVA_EFU_END_IDX

#define CVA_RT_INIT_OK  0u
#define CVA_RT_RESET_OK 1u
#ifdef AUTOSAR_PORT
#define CVA_RTK_USER_PORT_1ST  (0u)
#define CVA_RTK_USER_PORT_2ND  (1u)
#define CVA_RTK_USER_PORT_3RD  (2u)
#define CVA_RTK_USER_PORT_4TH  (3u)
#define CVA_RTK_USER_PORT_5TH  (4u)
#define CVA_RTK_USER_PORT_6TH  (5u)
#define CVA_RTK_USER_PORT_7TH  (6u)
#define CVA_RTK_USER_PORT_8TH  (7u)
#define CVA_RTK_USER_PORT_9TH  (8u)
#define CVA_RTK_USER_PORT_10TH (9u)
#define CVA_RTK_USER_PORT_11TH (10u)
#define CVA_RTK_USER_PORT_END  (CVA_RTK_USER_PORT_11TH)
#else
#define CVA_RTK_USER_PORT_1ST  (1u)
#define CVA_RTK_USER_PORT_2ND  (2u)
#define CVA_RTK_USER_PORT_3RD  (3u)
#define CVA_RTK_USER_PORT_4TH  (4u)
#define CVA_RTK_USER_PORT_5TH  (5u)
#define CVA_RTK_USER_PORT_6TH  (6u)
#define CVA_RTK_USER_PORT_7TH  (7u)
#define CVA_RTK_USER_PORT_8TH  (8u)
#define CVA_RTK_USER_PORT_9TH  (9u)
#define CVA_RTK_USER_PORT_10TH (10u)
#define CVA_RTK_USER_PORT_11TH (11u)
#define CVA_RTK_USER_PORT_END  (CVA_RTK_USER_PORT_11TH)
#endif

#define CVA_RTK_USER_PMAP_1ST  ((uint32)1u << CVA_RTK_USER_PORT_1ST)
#define CVA_RTK_USER_PMAP_2ND  ((uint32)1u << CVA_RTK_USER_PORT_2ND)
#define CVA_RTK_USER_PMAP_3RD  ((uint32)1u << CVA_RTK_USER_PORT_3RD)
#define CVA_RTK_USER_PMAP_4TH  ((uint32)1u << CVA_RTK_USER_PORT_4TH)
#define CVA_RTK_USER_PMAP_5TH  ((uint32)1u << CVA_RTK_USER_PORT_5TH)
#define CVA_RTK_USER_PMAP_6TH  ((uint32)1u << CVA_RTK_USER_PORT_6TH)
#define CVA_RTK_USER_PMAP_7TH  ((uint32)1u << CVA_RTK_USER_PORT_7TH)
#define CVA_RTK_USER_PMAP_8TH  ((uint32)1u << CVA_RTK_USER_PORT_8TH)
#define CVA_RTK_USER_PMAP_9TH  ((uint32)1u << CVA_RTK_USER_PORT_9TH)
#define CVA_RTK_USER_PMAP_10TH ((uint32)1u << CVA_RTK_USER_PORT_10TH)
#define CVA_RTK_USER_PMAP_11TH ((uint32)1u << CVA_RTK_USER_PORT_11TH)
#define CVA_RTK_USER_PMAP_ALL  (uint32)(CVA_RTK_USER_PMAP_1ST | CVA_RTK_USER_PMAP_2ND | CVA_RTK_USER_PMAP_3RD | CVA_RTK_USER_PMAP_4TH | CVA_RTK_USER_PMAP_5TH | CVA_RTK_USER_PMAP_6TH | CVA_RTK_USER_PMAP_7TH | CVA_RTK_USER_PMAP_8TH | CVA_RTK_USER_PMAP_9TH | CVA_RTK_USER_PMAP_10TH | CVA_RTK_USER_PMAP_11TH)

#define CVA_RTK_PORT_SPEED_10M   (0u)
#define CVA_RTK_PORT_SPEED_100M  (1u)
#define CVA_RTK_PORT_SPEED_1000M (2u)
#define CVA_RTK_PORT_SPEED_2500M (3u)
#define CVA_RTK_PORT_SPEED_5000M (4u)
#define CVA_RTK_PORT_SPEED_END   (CVA_RTK_PORT_SPEED_5000M)

#endif /* CVA_RTK_API_H__ */
