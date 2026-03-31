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

#ifndef CVB_ASICDRV_SINGLE_LOOP_H_
#define CVB_ASICDRV_SINGLE_LOOP_H_

#include "rtl907xc_vb_api_ext.h"
/* register list */
#define CVB_SL_CONFIG                (CVB_HW_MISC_DMY_RESIGSER_13)
#define CVB_SL_CONFIG_FIELDNUM       (10u)
#define CVB_SL_CONFIG_DEFAULTVAL     (0x0u)
#define CVB_SL_CONTROL               (CVB_RSTP_GLOBAL)
#define CVB_SL_CONTROL_FIELDNUM      (2u)
#define CVB_SL_CONTROL_DEFAULTVAL    (0x0u)
#define CVB_SL_PORT_STATE            (CVB_SPANNING_TREE_PORT_STATE_CONTROL0)
#define CVB_SL_PORT_STATE_FIELDNUM   (16u)
#define CVB_SL_PORT_STATE_DEFAULTVAL (0xFFFFFFFFu)

/* register field */
#define CVB_SL_CONFIG_CONTROL_PORT_A_LSP        (0u)
#define CVB_SL_CONFIG_CONTROL_PORT_A_LEN        (4u)
#define CVB_SL_CONFIG_CONTROL_PORT_A_TYPE       (CVB_FIELD_TYPE_RW)
#define CVB_SL_CONFIG_CONTROL_PORT_A_STATE_LSP  (4u)
#define CVB_SL_CONFIG_CONTROL_PORT_A_STATE_LEN  (1u)
#define CVB_SL_CONFIG_CONTROL_PORT_A_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_CONFIG_CONTROL_PORT_B_LSP        (8u)
#define CVB_SL_CONFIG_CONTROL_PORT_B_LEN        (4u)
#define CVB_SL_CONFIG_CONTROL_PORT_B_TYPE       (CVB_FIELD_TYPE_RW)
#define CVB_SL_CONFIG_CONTROL_PORT_B_STATE_LSP  (12u)
#define CVB_SL_CONFIG_CONTROL_PORT_B_STATE_LEN  (1u)
#define CVB_SL_CONFIG_CONTROL_PORT_B_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_CONFIG_AUTO_RECOVERY_LSP         (16u)
#define CVB_SL_CONFIG_AUTO_RECOVERY_LEN         (1u)
#define CVB_SL_CONFIG_AUTO_RECOVERY_TYPE        (CVB_FIELD_TYPE_RW)
#define CVB_SL_CONFIG_ENABLE_LSP                (31u)
#define CVB_SL_CONFIG_ENABLE_LEN                (1u)
#define CVB_SL_CONFIG_ENABLE_TYPE               (CVB_FIELD_TYPE_RW)

#define CVB_SL_PORT_STATE_PORT15_STATE_LSP  (30u)
#define CVB_SL_PORT_STATE_PORT15_STATE_LEN  (2u)
#define CVB_SL_PORT_STATE_PORT15_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT14_STATE_LSP  (28u)
#define CVB_SL_PORT_STATE_PORT14_STATE_LEN  (2u)
#define CVB_SL_PORT_STATE_PORT14_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT13_STATE_LSP  (26u)
#define CVB_SL_PORT_STATE_PORT13_STATE_LEN  (2u)
#define CVB_SL_PORT_STATE_PORT13_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT12_STATE_LSP  (24u)
#define CVB_SL_PORT_STATE_PORT12_STATE_LEN  (2u)
#define CVB_SL_PORT_STATE_PORT12_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT11_STATE_LSP  (22u)
#define CVB_SL_PORT_STATE_PORT11_STATE_LEN  (2u)
#define CVB_SL_PORT_STATE_PORT11_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT10_STATE_LSP  (20u)
#define CVB_SL_PORT_STATE_PORT10_STATE_LEN  (2u)
#define CVB_SL_PORT_STATE_PORT10_STATE_TYPE (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT9_STATE_LSP   (18u)
#define CVB_SL_PORT_STATE_PORT9_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT9_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT8_STATE_LSP   (16u)
#define CVB_SL_PORT_STATE_PORT8_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT8_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT7_STATE_LSP   (14u)
#define CVB_SL_PORT_STATE_PORT7_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT7_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT6_STATE_LSP   (12u)
#define CVB_SL_PORT_STATE_PORT6_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT6_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT5_STATE_LSP   (10u)
#define CVB_SL_PORT_STATE_PORT5_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT5_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT4_STATE_LSP   (8u)
#define CVB_SL_PORT_STATE_PORT4_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT4_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT3_STATE_LSP   (6u)
#define CVB_SL_PORT_STATE_PORT3_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT3_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT2_STATE_LSP   (4u)
#define CVB_SL_PORT_STATE_PORT2_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT2_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT1_STATE_LSP   (2u)
#define CVB_SL_PORT_STATE_PORT1_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT1_STATE_TYPE  (CVB_FIELD_TYPE_RW)
#define CVB_SL_PORT_STATE_PORT0_STATE_LSP   (0u)
#define CVB_SL_PORT_STATE_PORT0_STATE_LEN   (2u)
#define CVB_SL_PORT_STATE_PORT0_STATE_TYPE  (CVB_FIELD_TYPE_RW)

#define CVB_SL_CONTROL_REINIT_LSP  (0u)
#define CVB_SL_CONTROL_REINIT_LEN  (1u)
#define CVB_SL_CONTROL_REINIT_TYPE (CVB_FIELD_TYPE_WO)

EXTERN_API RtkApiRet cvb_drv_sl_default_port_state_check(SingleLoopPortState_e portAState, SingleLoopPortState_e portBState);
EXTERN_API RtkApiRet cvb_drv_sl_default_port_state_get(uint32 mode, SingleLoopPortState_e *pPortStateE, uint32 *pPortStateU);
EXTERN_API RtkApiRet cvb_drv_sl_port_state_get(uint32 portState, SingleLoopPortState_e *pPortState);

#endif /* CVB_ASICDRV_SINGLE_LOOP_H_ */
