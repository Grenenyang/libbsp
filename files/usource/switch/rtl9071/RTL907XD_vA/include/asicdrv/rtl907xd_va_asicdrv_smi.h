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

#ifndef DVA_ASICDRV_SMI_H_
#define DVA_ASICDRV_SMI_H_
#include "rtkas_common_types.h"
#include "rtkas_error.h"

/******************************************************************************
 *  Defines
 ******************************************************************************/
#define DVA_MDX_POLLING_MAX_US (96u)
#define DVA_MDX_POLLING_MIN_US (3u)

/******************************************************************************
 *  Structure
 ******************************************************************************/
/******************************************************************************
 *  Global Variables
 ******************************************************************************/
/******************************************************************************
 *  Function Prototype
 ******************************************************************************/
EXTERN_API RtkApiRet dva_mdx_init(const uint8 phyID[], uint8 gpio, uint8 baudr, uint8 preamble);
EXTERN_API RtkApiRet dva_mdx_register_set(uint8 phyID_idx, uint16 page, uint8 reg, uint16 mdxData);
EXTERN_API RtkApiRet dva_mdx_register_get(uint8 phyID_idx, uint16 page, uint8 reg, uint16 *mdxData);
EXTERN_API RtkApiRet dva_mdx_polling_cmd_bit(uint32 unit);

#endif /* DVA_ASICDRV_SMI_H_ */
