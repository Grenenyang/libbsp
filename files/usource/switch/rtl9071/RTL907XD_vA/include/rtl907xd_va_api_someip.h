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

#ifndef DVA_RTK_API_SOMEIP_H__
#define DVA_RTK_API_SOMEIP_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xd_va_api.h"
#include "rtkas_api_someip.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define SOMEIP_L4DPORT_NUM      (16u)
#define SOMEIP_HW_WHITELIST_NUM (32u)

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dva_someip_set_l4dport(uint32 unit, uint8 idx, uint16 l4dport);
EXTERN_API RtkApiRet dva_someip_get_l4dport(uint32 unit, uint16 *l4dport);
EXTERN_API RtkApiRet dva_someip_set_hw_whitelist(uint32 unit, uint8 idx, uint32 whitelist);
EXTERN_API RtkApiRet dva_someip_get_hw_whitelist(uint32 unit, uint32 *whitelist);

#endif /* DVA_RTK_API_SOMEIP_H__ */
