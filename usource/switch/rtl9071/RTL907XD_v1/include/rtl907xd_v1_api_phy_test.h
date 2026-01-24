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

#ifndef DV1_RTK_API_PHY_TEST_H__
#define DV1_RTK_API_PHY_TEST_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_api_phy_test.h"
#include "rtl907xd_v1_api.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DV1_PHY_TESTMODE_BIT_MASK      (0xE000u)
#define DV1_PHY_TESTMODE_LSB           (13u)
#define DV1_RTK_RTCT_ARRAY             (3u)
#define DV1_OCR_PORT_SHIFT_LSB         (16u)
#define DV1_PHY_TEST_USER_PM_ALL       (DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH | DV1_RTK_USER_PMAP_5TH | DV1_RTK_USER_PMAP_6TH)
#define DV1_PHY_TEST_USER_PORT_MIN     (DV1_RTK_USER_PORT_1ST)
#define DV1_PHY_TEST_USER_PORT_MAX     (DV1_RTK_USER_PORT_6TH)
#define DV1_PHY_TEST_USER_PORT_END     (DV1_RTK_USER_PORT_6TH + 1)
#define DV1_PHY_TESTMODE_SPEED_100M    (0u)
#define DV1_PHY_TESTMODE_SPEED_1000M   (1u)
#define DV1_PHY_TESTMODE_SPEED_NUM_END (2u)
#define DV1_PHY_TESTMODE_ROLE_NUM_END  (2u)
#define DV1_PHY_TESTMODE_NUM_END       (8u)

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
EXTERN_API RtkApiRet dv1_rtct_run_cable_diag(uint32 unit, RtkRtctPortNumber_e port);
EXTERN_API RtkApiRet dv1_rtct_get_cable_status(uint32 unit, RtkRtctPortNumber_e port, RtkRtctCableStatus_e *rtkRtctCableStatus, RtkRtctCableDefect *rtkRtctCableDefect);
EXTERN_API RtkApiRet dv1_phy_testmode_set(uint32 unit, uint8 port, uint8 const testMode, uint8 speed, uint8 isMaster);
EXTERN_API RtkApiRet dv1_phy_testmode_get(uint32 unit, uint8 port, uint8 speed, uint8 *testMode);
EXTERN_API RtkApiRet dv1_port_sqi_get(uint32 unit, RtkPort port, uint8 speed, uint8 *pSqi);

#endif /* DV1_RTK_API_PHY_TEST_H__ */