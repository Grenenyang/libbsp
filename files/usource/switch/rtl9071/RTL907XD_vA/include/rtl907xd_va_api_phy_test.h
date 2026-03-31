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

#ifndef DVA_RTK_API_PHY_TEST_H__
#define DVA_RTK_API_PHY_TEST_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_api_phy_test.h"
#include "rtl907xd_va_api.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DVA_PHY_TESTMODE_BIT_MASK      (0xE000u)
#define DVA_PHY_TESTMODE_LSB           (13u)
#define DVA_RTK_RTCT_ARRAY             (3u)
#define DVA_OCR_PORT_SHIFT_LSB         (16u)
#define DVA_PHY_TEST_USER_PM_ALL       (DVA_RTK_USER_PMAP_1ST | DVA_RTK_USER_PMAP_2ND | DVA_RTK_USER_PMAP_3RD | DVA_RTK_USER_PMAP_4TH | DVA_RTK_USER_PMAP_5TH | DVA_RTK_USER_PMAP_6TH)
#define DVA_PHY_TEST_USER_PORT_MIN     (DVA_RTK_USER_PORT_1ST)
#define DVA_PHY_TEST_USER_PORT_MAX     (DVA_RTK_USER_PORT_6TH)
#define DVA_PHY_TEST_USER_PORT_END     (DVA_RTK_USER_PORT_6TH + 1)
#define DVA_PHY_TESTMODE_SPEED_100M    (0u)
#define DVA_PHY_TESTMODE_SPEED_1000M   (1u)
#define DVA_PHY_TESTMODE_SPEED_NUM_END (2u)
#define DVA_PHY_TESTMODE_ROLE_NUM_END  (2u)
#define DVA_PHY_TESTMODE_NUM_END       (8u)

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dva_rtct_run_cable_diag(uint32 unit, RtkRtctPortNumber_e port);
EXTERN_API RtkApiRet dva_rtct_get_cable_status(uint32 unit, RtkRtctPortNumber_e port, RtkRtctCableStatus_e *rtkRtctCableStatus, RtkRtctCableDefect *rtkRtctCableDefect);
EXTERN_API RtkApiRet dva_phy_testmode_set(uint32 unit, uint8 port, uint8 const testMode, uint8 speed, uint8 isMaster);
EXTERN_API RtkApiRet dva_phy_testmode_get(uint32 unit, uint8 port, uint8 speed, uint8 *testMode);
EXTERN_API RtkApiRet dva_port_mse_get(uint32 unit, RtkPort port, uint16 *pMse);
EXTERN_API RtkApiRet dva_port_wc_mse_get(uint32 unit, RtkPort port, uint16 *pWcMse);
EXTERN_API RtkApiRet dva_port_peak_mse_get(uint32 unit, RtkPort port, uint8 *pPkMse);
EXTERN_API RtkApiRet dva_port_wc_peak_mse_get(uint32 unit, RtkPort port, uint8 *pWcPkMse);
EXTERN_API RtkApiRet dva_port_sqi_get(uint32 unit, RtkPort port, uint8 speed, uint8 *pSqi);
EXTERN_API RtkApiRet dva_port_ltt_get(uint32 unit, RtkPort port, uint8 *pTime);
EXTERN_API RtkApiRet dva_port_lrt_get(uint32 unit, RtkPort port, uint8 *pTime);
EXTERN_API RtkApiRet dva_port_rrt_get(uint32 unit, RtkPort port, uint8 *pTime);
EXTERN_API RtkApiRet dva_port_lfl_loss_get(uint32 unit, RtkPort port, uint8 *pLoss);
EXTERN_API RtkApiRet dva_port_lfl_fail_get(uint32 unit, RtkPort port, uint16 *pFail);

#endif /* DVA_RTK_API_PHY_TEST_H__ */