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

#ifndef DV1_RTK_API_H__
#define DV1_RTK_API_H__

/*
 * Include Files
 */
#include "rtkas_endian.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_timer.h"
#include "rtl907xd_v1_asicdrv.h"

#define DV1_RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST 1
#define DV1_RTK_MAX_NUM_OF_PROTO_TYPE                0xFFFF
#define DV1_RTK_MAX_PORT_MASK                        0xFFFFu

#define DV1_RTK_WRAP_CHECK_32 (0xFFFFFFFFu)

/* indirect channel type */
#define DV1_RTK_INDIRECT_CHANNEL_PRIMARY   (0u)
#define DV1_RTK_INDIRECT_CHANNEL_SECONDARY (1u)

/* wol event */
#define DV1_WOL_EVENT_LINKUP        (0x1)
#define DV1_WOL_EVENT_MAGIC_PKT     (0x2)
#define DV1_WOL_EVENT_WAKEUP_PKT    (0x4)
#define DV1_WOL_EVENT_ANY_PKT_PORT5 (0x8)  /* Valid only when port 5 is configured as cpu port */
#define DV1_WOL_EVENT_ANY_PKT_PORT6 (0x10) /* Valid only when port 6 is configured as cpu port */

#ifndef DV1_MAC_ADDR_LEN
#define DV1_MAC_ADDR_LEN (MAC_ADDR_LEN)
#endif

#define DV1_IPV6_ADDR_LEN (16u)
#define DV1_IPV4_ADDR_LEN (4u)

#define DV1_ALLPORT 0xFF

typedef uint32 dv1_RtkEnable;
#define DV1_DISABLED       (0u) /* Disable function */
#define DV1_ENABLED        (1u) /* Enable function */
#define DV1_RTK_ENABLE_END (2u)

typedef uint32 dv1_RtkPort;

/* unauth pkt action */

/* cfg_spi_para_idx */
#define DV1_CFG_25UA_IDX        (0u)
#define DV1_CFG_OPFSM_IDX       (1u)
#define DV1_CFG_BR_PATCH_1_IDX  (2u)
#define DV1_CFG_BR_PATCH_2_IDX  (3u)
#define DV1_CFG_SGMII_PATCH_IDX (4u)
#define DV1_CFG_FE_PATCH_IDX    (5u)
#define DV1_CFG_MAC_CONF_IDX    (6u)
#define DV1_CFG_SOMEIP_IDX      (7u)
/* domain 8 : CFG_FWCFG_IDX is deprecrated */
#define DV1_CFG_RESERVED_1          (8u)
#define DV1_CFG_RTPG_PATCH_1_IDX    (9u)
#define DV1_CFG_RTPG_PATCH_2_IDX    (10u)
#define DV1_CFG_STACKING_IDX        (11u)
#define DV1_CFG_PCIE_IDX            (12u)
#define DV1_CFG_FWCFG_BYTE_IDX      (13u)
#define DV1_CFG_REG_FIELD_WR_IDX    (14u)
#define DV1_CFG_MKA_IDX             (15u)
#define DV1_CFG_ROT_SW_TABLE        (16u)
#define DV1_CFG_ROT_SW_INTF_TABLE   (17u)
#define DV1_CFG_MACPHY_0_CONFIG_IDX (18u)
#define DV1_CFG_MACPHY_1_CONFIG_IDX (19u)
#define DV1_CFG_PTP_IDX             (20u)
#define DV1_CFG_TBL_BASE_IDX        (21u)
#define DV1_CFG_TBL_END_IDX         (60u)
#define DV1_CFG_QCC_PRE_TBL_IDX     (61u)
#define DV1_CFG_5KR_PATCH_IDX       (62u)
/* domain 63: reserved for patch code */
#define DV1_CFG_RESERVED_2 (63u)
#define DV1_CFG_END_IDX    (64u)

#define DV1_EFU_25UA_IDX         (0u)
#define DV1_EFU_OPFSM_IDX        (1u)
#define DV1_EFU_FUSEOUT_IDX      (2u)
#define DV1_EFU_BR_PATCH_1_IDX   (3u)
#define DV1_EFU_BR_PATCH_2_IDX   (4u)
#define DV1_EFU_SGMII_PATCH_IDX  (5u)
#define DV1_EFU_FE_PATCH_IDX     (6u)
#define DV1_EFU_MAC_CONF_IDX     (7u)
#define DV1_EFU_RTPG_PATCH_1_IDX (8u)
#define DV1_EFU_RTPG_PATCH_2_IDX (9u)
#define DV1_EFU_SGMII_FIX_IDX    (10u)
#define DV1_EFU_PCIE_IDX         (11u)
#define DV1_EFU_STACKING_IDX     (12u)
#define DV1_EFU_FWCFG_BYTE_IDX   (13u)
#define DV1_EFU_END_IDX          (14u)

#define DV1_EFUSE_END_IDX DV1_EFU_END_IDX

#define DV1_RT_INIT_OK  0u
#define DV1_RT_RESET_OK 1u
#ifdef AUTOSAR_PORT
#define DV1_RTK_USER_PORT_1ST  (0u)
#define DV1_RTK_USER_PORT_2ND  (1u)
#define DV1_RTK_USER_PORT_3RD  (2u)
#define DV1_RTK_USER_PORT_4TH  (3u)
#define DV1_RTK_USER_PORT_5TH  (4u)
#define DV1_RTK_USER_PORT_6TH  (5u)
#define DV1_RTK_USER_PORT_7TH  (6u)
#define DV1_RTK_USER_PORT_8TH  (7u)
#define DV1_RTK_USER_PORT_9TH  (8u)
#define DV1_RTK_USER_PORT_10TH (9u)
#define DV1_RTK_USER_PORT_11TH (10u)
#define DV1_RTK_USER_PORT_12TH (11u)
#define DV1_RTK_USER_PORT_END  (DV1_RTK_USER_PORT_12TH)
#else
#define DV1_RTK_USER_PORT_1ST  (1u)
#define DV1_RTK_USER_PORT_2ND  (2u)
#define DV1_RTK_USER_PORT_3RD  (3u)
#define DV1_RTK_USER_PORT_4TH  (4u)
#define DV1_RTK_USER_PORT_5TH  (5u)
#define DV1_RTK_USER_PORT_6TH  (6u)
#define DV1_RTK_USER_PORT_7TH  (7u)
#define DV1_RTK_USER_PORT_8TH  (8u)
#define DV1_RTK_USER_PORT_9TH  (9u)
#define DV1_RTK_USER_PORT_10TH (10u)
#define DV1_RTK_USER_PORT_11TH (11u)
#define DV1_RTK_USER_PORT_12TH (12u)
#define DV1_RTK_USER_PORT_END  (DV1_RTK_USER_PORT_12TH)
#endif
#define DV1_RTK_USER_PMAP_1ST  ((uint32)1u << DV1_RTK_USER_PORT_1ST)
#define DV1_RTK_USER_PMAP_2ND  ((uint32)1u << DV1_RTK_USER_PORT_2ND)
#define DV1_RTK_USER_PMAP_3RD  ((uint32)1u << DV1_RTK_USER_PORT_3RD)
#define DV1_RTK_USER_PMAP_4TH  ((uint32)1u << DV1_RTK_USER_PORT_4TH)
#define DV1_RTK_USER_PMAP_5TH  ((uint32)1u << DV1_RTK_USER_PORT_5TH)
#define DV1_RTK_USER_PMAP_6TH  ((uint32)1u << DV1_RTK_USER_PORT_6TH)
#define DV1_RTK_USER_PMAP_7TH  ((uint32)1u << DV1_RTK_USER_PORT_7TH)
#define DV1_RTK_USER_PMAP_8TH  ((uint32)1u << DV1_RTK_USER_PORT_8TH)
#define DV1_RTK_USER_PMAP_9TH  ((uint32)1u << DV1_RTK_USER_PORT_9TH)
#define DV1_RTK_USER_PMAP_10TH ((uint32)1u << DV1_RTK_USER_PORT_10TH)
#define DV1_RTK_USER_PMAP_11TH ((uint32)1u << DV1_RTK_USER_PORT_11TH)
#define DV1_RTK_USER_PMAP_12TH ((uint32)1u << DV1_RTK_USER_PORT_12TH)
#define DV1_RTK_USER_PMAP_ALL  (uint32)(DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH | DV1_RTK_USER_PMAP_5TH | DV1_RTK_USER_PMAP_6TH | DV1_RTK_USER_PMAP_7TH | DV1_RTK_USER_PMAP_8TH | DV1_RTK_USER_PMAP_9TH | DV1_RTK_USER_PMAP_10TH | DV1_RTK_USER_PMAP_11TH | DV1_RTK_USER_PMAP_12TH)

#endif /* DV1_RTK_API_H__ */
