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

#ifndef DVA_RTK_API_ATS_H__
#define DVA_RTK_API_ATS_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xd_va_api.h"
#include "rtkas_api_ats.h"

#define PORT_OFFSET            (0x800u)
#define PROCESS_DELAY_MIN_TICK (52u)
#define PROCESS_DELAY_MAX_TICK (52u)
#define RTL9072D_PORT_NUMBER   (12u)
#define RTL9070D_PORT_NUMBER   (10u)
#define RTL9072D_VA_PACKAGE    (0xE0u)
#define RTL9070D_VA_PACKAGE    (0xE1u)
#define PORT_OFFSET            (0x800u)

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet dva_ats_init(uint32 unit);
EXTERN_API RtkApiRet dva_ats_get_info(uint32 unit, AtsGetInfo_t *atsGetInfo);
EXTERN_API RtkApiRet dva_ats_cbs_cir_set(uint32 unit, uint8 schedulerId, uint8 groupId, uint32 cbs, uint64 cirKbps);
EXTERN_API RtkApiRet dva_ats_cbs_cir_get(uint32 unit, uint8 schedulerId, uint8 *pGroupId, uint32 *pCbs, uint64 *pCirKbps);
EXTERN_API RtkApiRet dva_ats_group_residence_time_set(uint32 unit, uint8 groupId, uint32 maxResidenceTimeNs);
EXTERN_API RtkApiRet dva_ats_group_residence_time_get(uint32 unit, uint8 groupId, uint32 *pMaxResidenceTimeNs);
EXTERN_API RtkApiRet dva_ats_drop_counter_get(uint32 unit, RtkPort port, uint64 *pCounter);
EXTERN_API RtkApiRet dva_ats_drop_counter_clear(uint32 unit, RtkPort port);
EXTERN_API RtkApiRet dva_ats_scheduler_set(uint32 unit, RtkPort port, QosQueNum queue, RtkEnable mode);

#endif /* DVA_RTK_API_ATS_H__ */
