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

#ifndef CVA_RTK_API_PHY_TEST_H__
#define CVA_RTK_API_PHY_TEST_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_api_phy_test.h"
#include "rtl907xc_va_api.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CVA_PHY_TESTMODE_PORT6                      (6u)
#define CVA_PHY_TESTMODE_PORT7                      (7u)
#define CVA_PHY_TESTMODE_BIT_MASK                   (0xE000u)
#define CVA_PHY_TESTMODE_LSB                        (13u)
#define CVA_PHY_TESTMODE_100_OUTPUT_CLK_EN_REGVALUE (0x4588u)
#define CVA_PHY_100_TESTMODE_3                      (3u)
#define CVA_RTK_RTCT_ARRAY                          (3u)
#define CVA_OCR_PORT_SHIFT_LSB                      (16u)
#define CVA_PHY_RST_USER_PM_1                       (1u << PN_PORT1)
#define CVA_PHY_RST_USER_PM_2                       (1u << PN_PORT2)
#define CVA_PHY_RST_USER_PM_3                       (1u << PN_PORT3)
#define CVA_PHY_RST_USER_PM_4                       (1u << PN_PORT4)
#define CVA_PHY_RST_USER_PM_5                       (1u << PN_PORT5)
#define CVA_PHY_RST_USER_PM_6                       (1u << PN_PORT6)
#define CVA_PHY_RST_USER_PM                         (CVA_PHY_RST_USER_PM_1 | CVA_PHY_RST_USER_PM_2 | CVA_PHY_RST_USER_PM_3 | CVA_PHY_RST_USER_PM_4 | CVA_PHY_RST_USER_PM_5 | CVA_PHY_RST_USER_PM_6)
#define CVA_PHYTEST_BW_10M                          (0u)
#define CVA_PHYTEST_BW_100M                         (1u)
#define CVA_PHYTEST_RTP_100M                        (0u)
#define CVA_PHYTEST_RTP_1000M                       (1u)

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
EXTERN_API RtkApiRet cva_rtct_run_cable_diag(uint32 unit, RtkRtctPortNumber_e port);
EXTERN_API RtkApiRet cva_rtct_get_cable_status(uint32 unit, RtkRtctPortNumber_e port, RtkRtctCableStatus_e *rtkRtctCableStatus, RtkRtctCableDefect *rtkRtctCableDefect);
EXTERN_API RtkApiRet cva_phy_testmode_set(uint32 unit, uint8 port, uint8 const testMode, uint8 speed, uint8 isMaster);
EXTERN_API RtkApiRet cva_phy_testmode_get(uint32 unit, uint8 port, uint8 speed, uint8 *testMode);
EXTERN_API RtkApiRet cva_port_sqi_get(uint32 unit, RtkPort port, uint8 speed, uint8 *pSqi);

#endif /* CVA_RTK_API_PHY_TEST_H__ */