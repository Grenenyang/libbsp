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

#ifndef RTKAS_TYPES_H_
#define RTKAS_TYPES_H_

#include "rtkas_common.h"
#define CHK_FUN_RETVAL(f)                  \
    do                                     \
    {                                      \
        if((int32)RT_ERR_OK != (int32)(f)) \
        {                                  \
            return (RT_ERR_SMI);           \
        }                                  \
    } while(0u == 1u)

#define CHK_FUN_CONTINUE_START \
    do                         \
    {

#define CHK_FUN_CONTINUE_END \
    }                        \
    while(1u == 0u)          \
        ;

#define CHK_FUN_CONTINUE(r, f)             \
    {                                      \
        (r) = (f);                         \
        if((int32)RT_ERR_OK != (int32)(r)) \
        {                                  \
            continue;                      \
        }                                  \
    }

#define CHK_FUN_CONTINUE_RESERVE_ERR(r, f) \
    {                                      \
        if((int32)RT_ERR_OK != (int32)(r)) \
        {                                  \
            continue;                      \
        }                                  \
        (r) = (f);                         \
    }

#define CHK_FUN_BREAK(r, f)                \
    {                                      \
        (r) = (f);                         \
        if((int32)RT_ERR_OK != (int32)(r)) \
        {                                  \
            break;                         \
        }                                  \
    }

#define CHK_FUN_RET(r, f)                  \
    do                                     \
    {                                      \
        if((int32)RT_ERR_OK == (int32)(r)) \
        {                                  \
            (r) = (f);                     \
        }                                  \
    } while(0u == 1u)

#define PN_PORT_TEST_START PN_PORT12
#define PN_PORT_TEST_END   PN_PORT14
#define PERPORT_REG_OFFSET (0x800u)
#define NEXT_REG_OFFSET    (4u)

/* TODO: Deprecated, Port Number */
#define PN_PORT0    (0u)
#define PN_PORT1    (1u)
#define PN_PORT2    (2u)
#define PN_PORT3    (3u)
#define PN_PORT4    (4u)
#define PN_PORT5    (5u)
#define PN_PORT6    (6u)
#define PN_PORT7    (7u)
#define PN_PORT8    (8u)
#define PN_PORT9    (9u)
#define PN_PORT10   (10u)
#define PN_PORT11   (11u)
#define PN_PORT12   (12u)
#define PN_PORT13   (13u)
#define PN_PORT14   (14u)
#define PN_PORT15   (15u)
#define PN_PORT_END (16u)

/* TODO: Deprecated, Port Mask */
#define PM_PORT_0   (1u << PN_PORT0)
#define PM_PORT_1   (1u << PN_PORT1)
#define PM_PORT_2   (1u << PN_PORT2)
#define PM_PORT_3   (1u << PN_PORT3)
#define PM_PORT_4   (1u << PN_PORT4)
#define PM_PORT_5   (1u << PN_PORT5)
#define PM_PORT_6   (1u << PN_PORT6)
#define PM_PORT_7   (1u << PN_PORT7)
#define PM_PORT_8   (1u << PN_PORT8)
#define PM_PORT_9   (1u << PN_PORT9)
#define PM_PORT_10  (1u << PN_PORT10)
#define PM_PORT_11  (1u << PN_PORT11)
#define PM_PORT_12  (1u << PN_PORT12)
#define PM_PORT_13  (1u << PN_PORT13)
#define PM_PORT_14  (1u << PN_PORT14)
#define PM_PORT_15  (1u << PN_PORT15)
#define PM_PORT_ALL (0xFFFFu)

#define DV1_IS_VALID_PORT_ID(portId)            (((((uint32)DV1_RTK_USER_PORT_1ST <= (portId)) && ((uint32)DV1_RTK_USER_PORT_END >= (portId))) || (DV1_RTK_CPU_PORT == (portId))) ? 1u : 0u)
#define DV1_IS_VALID_PORT_ID_WO_CPUPORT(portId) ((((uint32)DV1_RTK_USER_PORT_1ST <= (portId)) && ((uint32)DV1_RTK_USER_PORT_END >= (portId))) ? 1u : 0u)
#define DV1_IS_VALID_T1PHY_PORT_ID(portId)      ((((uint32)DV1_RTK_USER_PORT_1ST <= (portId)) && ((uint32)DV1_RTK_USER_PORT_6TH >= (portId))) ? 1u : 0u)

#define DVA_IS_VALID_PORT_ID(portId)            (((((uint32)DVA_RTK_USER_PORT_1ST <= (portId)) && ((uint32)DVA_RTK_USER_PORT_END >= (portId))) || (DVA_RTK_CPU_PORT == (portId))) ? 1u : 0u)
#define DVA_IS_VALID_PORT_ID_WO_CPUPORT(portId) ((((uint32)DVA_RTK_USER_PORT_1ST <= (portId)) && ((uint32)DVA_RTK_USER_PORT_END >= (portId))) ? 1u : 0u)
#define DVA_IS_VALID_T1PHY_PORT_ID(portId)      ((((uint32)DVA_RTK_USER_PORT_1ST <= (portId)) && ((uint32)DVA_RTK_USER_PORT_6TH >= (portId))) ? 1u : 0u)

#define CVB_IS_VALID_PORT_ID(portId)            (((((uint32)CVB_RTK_USER_PORT_1ST <= (portId)) && ((uint32)CVB_RTK_USER_PORT_END >= (portId))) || (CVB_RTK_CPU_PORT == (portId))) ? 1u : 0u)
#define CVB_IS_VALID_PORT_ID_WO_CPUPORT(portId) ((((uint32)CVB_RTK_USER_PORT_1ST <= (portId)) && ((uint32)CVB_RTK_USER_PORT_END >= (portId))) ? 1u : 0u)
#define CVB_IS_VALID_PORT_MASK(portMask)        (((portMask)&PM_PORT_ALL) ? 1u : 0u)
#define CVB_IS_VALID_PHY_ID(phyId)              ((((phyId) == 4u) || ((phyId) == 6u)) ? 1u : 0u)

#define CVA_IS_VALID_PORT_ID(portId)            (((((uint32)CVA_RTK_USER_PORT_1ST <= (portId)) && ((uint32)CVA_RTK_USER_PORT_END >= (portId))) || (CVA_RTK_CPU_PORT == (portId))) ? 1u : 0u)
#define CVA_IS_VALID_PORT_ID_WO_CPUPORT(portId) ((((uint32)CVA_RTK_USER_PORT_1ST <= (portId)) && ((uint32)CVA_RTK_USER_PORT_END >= (portId))) ? 1u : 0u)
#define CVA_IS_VALID_PORT_MASK(portMask)        (((portMask)&PM_PORT_ALL) ? 1u : 0u)
#define CVA_IS_VALID_PHY_ID(phyId)              ((((phyId) == 4u) || ((phyId) == 6u)) ? 1u : 0u)

/* skip the misra check #define IS_VALID_HEAC_PORT_ID(portId)       ((((portId) >= 0 && (portId) <= 3) || ((portId) == 6)) ? 1 : 0)*/

#define HAS_100BASE_T1PHY(portid) ((PN_PORT1 <= (portid)) && (PN_PORT6 >= (portid)))
#define PORTMASKOF100BASE_T1      0x6FFu

/*802.1X*/
#define RTL906X_SKEY_PORT0_ADDR    0x2ca8u
#define RTL906X_SKEY_PORT1_ADDR    0x2cb2u
#define RTL906X_SKEY_PORT2_ADDR    0x2cbcu
#define RTL906X_SKEY_PORT3_ADDR    0x2cc6u
#define RTL906X_SKEY_PORT4_ADDR    0x2cd0u
#define RTL906X_SKEY_PORT5_ADDR    0x2cdau
#define RTL906X_SWITCH_IP_ADDR     0x2ce4u
#define RTL906X_RADIUS_IP_ADDR     0x2ce8u
#define RTL906X_RADIUS_PORT_ADDR   0x2cedu
#define RTL906X_DOT1X_PORT_MASK_EN 0x2cecu
#define dot1x_string_length_limit  10u

/* VLAN */
#define RTL906X_VIDMAX             0xFFFu
#define RTL906X_FIDMAX             14u
#define RTL906X_VLAN_FLUSH_TIMEOUT 655535u

/* the above macro is generated by genDotH */
#define RTL906X_VALID_REG_NO 3236u

/* MIB Counter -- add by yjhsieh - 20180126 */
#define systemMIB 16u

/* Ethernet Access */
typedef struct
{
    uint8 sMac[MAC_ADDR_LEN]; /**< Destination Mac address (N/A) */
    uint8 dMac[MAC_ADDR_LEN]; /**< Source Mac address (N/A) */
} EtherAccessMac_t;

/* Stacking */

#define STACK_BOOT_MASTER (0u)
#define STACK_BOOT_SLAVE  (1u)
#define PORT_STACKING     (1u)
#define PORT_NON_STACKING (0u)
#endif /* RTKAS_TYPES_H_ */
