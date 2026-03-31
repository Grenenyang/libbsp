
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

#ifndef CVA_ASICDRV_FLOW_CTRL_H_
#define CVA_ASICDRV_FLOW_CTRL_H_

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CVA_FLOWCTRL_PORT_OFFSET  (0x800u)
#define CVA_FLOWCTRL_RA_BYPASS_EN (0x1u)

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/

EXTERN_API RtkApiRet cva_drv_flow_ctrl_ra_bypass_chk(uint32 unit, cva_RtkPort port);
EXTERN_API RtkApiRet cva_drv_flow_ctrl_global_set(uint32 unit, cva_RtkPort port, cva_RtkEnable en);

#endif /* CVA_ASICDRV_FLOW_CTRL_H_ */
