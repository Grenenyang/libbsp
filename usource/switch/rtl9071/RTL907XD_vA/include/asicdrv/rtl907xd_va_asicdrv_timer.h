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

#ifndef DVA_ASICDRV_TIMER_H__
#define DVA_ASICDRV_TIMER_H__

#include "rtl907xd_va_api_esm.h"

/******************************************************************************/
/*  Defines                                                                   */
/******************************************************************************/
#define DIRECT_ACCESS_REG                   (0u)
#define INDIRECT_ACCESS_REG_TARGET          (1u)
#define INDIRECT_ACCESS_PER_PORT_REG_TARGET (2u)

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
typedef struct
{
    uint32           triggerRegAddr;
    uint32           triggerRegLsp;
    uint32           triggerRegLen;
    uint32           pollingRegAddr;
    uint32           pollingRegLsp;
    uint32           pollingRegLen;
    uint32           action;
    uint32           expVal;
    uint32           port;
    uint32           chkPortClk;
    dva_EsmEventID_e eventID;
} RegPollingConfig_t;

EXTERN_API RtkApiRet dva_drv_timer_polling_reg_timeout(uint32 unit, RegPollingConfig_t *pollingConf, uint32 maxUs, uint32 minUs);

#endif /* DVA_ASICDRV_TIMER_H__ */
