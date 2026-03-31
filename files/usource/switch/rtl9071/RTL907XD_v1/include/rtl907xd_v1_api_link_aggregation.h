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

#ifndef DV1_RTK_API_LINK_AGGREGATION_H__
#define DV1_RTK_API_LINK_AGGREGATION_H__

#include "rtl907xd_v1_api.h"

EXTERN_API RtkApiRet dv1_lag_member_set(uint32 unit, uint32 group, RtkPmap portMask);
EXTERN_API RtkApiRet dv1_lag_member_get(uint32 unit, uint32 group, RtkPmap *pPortMask);
EXTERN_API RtkApiRet dv1_lag_hash_set(uint32 unit, uint32 group, uint8 hashMask, uint16 hashShift);
EXTERN_API RtkApiRet dv1_lag_hash_get(uint32 unit, uint32 group, uint8 *pHashMask, uint16 *pHashShift);
EXTERN_API RtkApiRet dv1_lag_balance_set(uint32 unit, uint32 group, uint32 balanceMode);
EXTERN_API RtkApiRet dv1_lag_balance_get(uint32 unit, uint32 group, uint32 *pBalanceMode);
EXTERN_API RtkApiRet dv1_lag_balance_ethertype_set(uint32 unit, uint32 group, uint16 etherType);
EXTERN_API RtkApiRet dv1_lag_balance_ethertype_get(uint32 unit, uint32 group, uint16 *pEtherType);
EXTERN_API RtkApiRet dv1_lag_balance_protocol_set(uint32 unit, uint32 group, uint8 protocol);
EXTERN_API RtkApiRet dv1_lag_balance_protocol_get(uint32 unit, uint32 group, uint8 *pProtocol);
EXTERN_API RtkApiRet dv1_lag_balance_l4dport_set(uint32 unit, uint32 group, uint16 l4Dport, uint16 l4DportMask);
EXTERN_API RtkApiRet dv1_lag_balance_l4dport_get(uint32 unit, uint32 group, uint16 *pL4Dport, uint16 *pL4DportMask);

#endif /*DV1_RTK_API_LINK_AGGREGATION_H__ */
