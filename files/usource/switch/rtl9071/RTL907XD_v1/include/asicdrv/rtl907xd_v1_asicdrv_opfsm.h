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

#ifndef DV1_ASICDRV_OPFSM_H_
#define DV1_ASICDRV_OPFSM_H_

#include "rtkas_common_types.h"
#include "rtl907xd_v1_asicdrv.h"
#ifdef AUTOSAR_PORT
#define DV1_MAP_USERPORT_TO_T1PHY                                        \
    {                                                                    \
        0u, 1u, 2u, 3u, 4u, 5u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu \
    }
#else
#define DV1_MAP_USERPORT_TO_T1PHY                                        \
    {                                                                    \
        0xFFu, 0u, 1u, 2u, 3u, 4u, 5u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu \
    }
#endif
/* user port to T1-PHY index mapping */
extern const uint8 dv1_g_userPort2T1phy[DV1_RTK_USER_PORT_END];

#define DV1_RTK_USER_PORT_WITHT1PHY_ID_MAX ((RtkPort)DV1_RTK_USER_PORT_6TH)
#define DV1_RTK_SYS_PORT_WITHT1PHY_ID_MAX  ((RtkPort)dv1_g_userPort2sysPort[DV1_RTK_USER_PORT_WITHT1PHY_ID_MAX])

#define DV1_RTK_T1PHY_PORT_1ST ((RtkPort)dv1_g_userPort2T1phy[DV1_RTK_USER_PORT_1ST])
#define DV1_RTK_T1PHY_PORT_2ND ((RtkPort)dv1_g_userPort2T1phy[DV1_RTK_USER_PORT_2ND])
#define DV1_RTK_T1PHY_PORT_3RD ((RtkPort)dv1_g_userPort2T1phy[DV1_RTK_USER_PORT_3RD])
#define DV1_RTK_T1PHY_PORT_4TH ((RtkPort)dv1_g_userPort2T1phy[DV1_RTK_USER_PORT_4TH])
#define DV1_RTK_T1PHY_PORT_5TH ((RtkPort)dv1_g_userPort2T1phy[DV1_RTK_USER_PORT_5TH])
#define DV1_RTK_T1PHY_PORT_6TH ((RtkPort)dv1_g_userPort2T1phy[DV1_RTK_USER_PORT_6TH])

#define DV1_RTK_T1PHY_PMAP_1ST ((RtkPmap)1u << DV1_RTK_T1PHY_PORT_1ST)
#define DV1_RTK_T1PHY_PMAP_2ND ((RtkPmap)1u << DV1_RTK_T1PHY_PORT_2ND)
#define DV1_RTK_T1PHY_PMAP_3RD ((RtkPmap)1u << DV1_RTK_T1PHY_PORT_3RD)
#define DV1_RTK_T1PHY_PMAP_4TH ((RtkPmap)1u << DV1_RTK_T1PHY_PORT_4TH)
#define DV1_RTK_T1PHY_PMAP_5TH ((RtkPmap)1u << DV1_RTK_T1PHY_PORT_5TH)
#define DV1_RTK_T1PHY_PMAP_6TH ((RtkPmap)1u << DV1_RTK_T1PHY_PORT_6TH)
#define DV1_RTK_T1PHY_PMAP_ALL (DV1_RTK_T1PHY_PMAP_1ST | DV1_RTK_T1PHY_PMAP_2ND | DV1_RTK_T1PHY_PMAP_3RD | DV1_RTK_T1PHY_PMAP_4TH | DV1_RTK_T1PHY_PMAP_5TH | DV1_RTK_T1PHY_PMAP_6TH)

#define DV1_RTK_USER_PMAP_WITHT1PHY (DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH | DV1_RTK_USER_PMAP_5TH | DV1_RTK_USER_PMAP_6TH)
#define DV1_RTK_SYS_PMAP_WITHT1PHY  (((RtkPmap)1u << DV1_RTK_SYS_PORT_1ST) | ((RtkPmap)1u << DV1_RTK_SYS_PORT_2ND) | ((RtkPmap)1u << DV1_RTK_SYS_PORT_3RD) | ((RtkPmap)1u << DV1_RTK_SYS_PORT_4TH) | ((RtkPmap)1u << DV1_RTK_SYS_PORT_5TH) | ((RtkPmap)1u << DV1_RTK_SYS_PORT_6TH))

#define DV1_OP_WUP_FLAG                 (0xA60u)
#define DV1_OP_WUR_FLAG                 (0xA72u)
#define DV1_OP_REC_FLAG                 (0x910u)
#define DV1_OP_PWON_FLAG                (0x918u)
#define DV1_OP_DIS_RWAKE                (0x822u)
#define DV1_OP_LWAKE_WAKE_PORT          (0x814u)
#define DV1_OP_EN_WUPR_TX               (0x824u)
#define DV1_OP_WUR_INTR_MSK             (0x91Eu)
#define DV1_OP_WUP_INTR_MSK             (0xA10u)
#define DV1_OP_OTHER_INTR_MSK           (0x91Eu)
#define DV1_OP_GLOB_CMD                 (0x900u)
#define DV1_OP_LWAKE_EDGE               (0x81Cu)
#define DV1_OP_WHOLE_CHIP_SLEEP_MASK    (0x4A1B000Cu)
#define DV1_OP_WUR_WUP_FLAG_REG         (0x4A1B0010u) /* For get WUP/WUR flag under wake-out forwarding is enabled */
#define DV1_OP_SLEEP_EVENT_FLAG         (0x4A1B0040u) /* Indicating sleep event including receiving sleep request, sleep handshake fail and sleep handshake success */
#define DV1_OP_SLP_SUCCESS_EVENT_MASK   (0x3FF00000u) /* bit[29:20] Sleep event flag of enter sleep mode of port1 to port10. */
#define DV1_OP_SLP_SUCCESS_EVENT_OFFSET (20u)         /* Offset of sleep event flag of sleep handshake success */
#define DV1_OP_SLP_FAIL_EVENT_MASK      (0x000FFC00u) /* bit[19:10] Sleep event flag of sleep handshake fail of port1 to port10. */
#define DV1_OP_SLP_FAIL_EVENT_OFFSET    (10u)         /* Offset of sleep event flag of sleep handshake fail */
#define DV1_OP_RX_SLP_EVENT_MASK        (0x000003FFu) /* bit[9:0]   Sleep event flag of receiving sleep request(LPS) of port1 to port10. */
#define DV1_OP_RX_SLP_EVENT_OFFSET      (0u)          /* Offset of sleep event flag of receiving sleep request */
#define DV1_OP_ACCEPT_SLEEP_CAP         (0x010C05C8u) /* bit[9:0] p1 to p10 accept sleep capability, bit10 enable deep sleep mode, bit12 enable external interrupt by sleep event */
#define DV1_OP_TXSLEEP_PORTMASK         (0x010C05CAu)
#define DV1_OP_VIR_INTR_MODE            (0x010C05CCu) /* g_Opfsm_VirInterrupt_Mode, 8bit */
#define DV1_OP_WAKEO_PULSE_CFG          (0x010C362Au) /* wake-out forwarding pulse setting */
#define DV1_OP_WAKEO_GPIOB1_PORTS       (0x010F534Cu) /* wake-out forwarding setting on GPIOB1 */
#define DV1_OP_WAKEO_GPIOB2_PORTS       (0x010F534Eu) /* wake-out forwarding setting on GPIOB2 */

typedef enum
{
    DV1_EM_USER_TO_T1PHY = 0, /**< To convert from user port mask to T1-PHY index mask */
    DV1_EM_T1PHY_TO_USER,     /**< To convert from T1-PHY index mask to user port mask */
    DV1_EM_T1PHY_CONVERT_END  /* The last mode. */
} dv1_RtkT1PHYMapConvertMode_e;

EXTERN_API RtkApiRet dv1_drv_phy_1000mt1_phy_sleep_cap_set(uint32 unit, uint32 regOffset, uint32 regOAMOffset, RtkEnable sleepCapOn);
EXTERN_API RtkApiRet dv1_drv_phy_100mt1_phy_sleep_cap_set(uint32 unit, uint32 regOffset, RtkEnable sleepCapOn);
EXTERN_API RtkApiRet dv1_drv_op_disable_wup_transmit(uint32 unit, RtkPmap *wupPortMask);
EXTERN_API RtkApiRet dv1_drv_op_enable_wup_transmit(uint32 unit, RtkPmap wupPortMask);
EXTERN_API RtkApiRet dv1_drv_op_eth_disable_wup_transmit(uint32 unit, RtkPmap *wupPortMask);
EXTERN_API RtkApiRet dv1_drv_op_eth_enable_wup_transmit(uint32 unit, RtkPmap wupPortMask);
EXTERN_API RtkApiRet dv1_drv_op_fetch_wakeo_config(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet dv1_drv_op_eth_fetch_wakeo_config(const int8 *fileName);

RtkApiRet dv1_drv_t1phy_pmap_convert(RtkPmap srcPmap, RtkPmap *dstPmap, dv1_RtkT1PHYMapConvertMode_e mode);

#endif /*#ifndef DV1_ASICDRV_OPFSM_H_*/
