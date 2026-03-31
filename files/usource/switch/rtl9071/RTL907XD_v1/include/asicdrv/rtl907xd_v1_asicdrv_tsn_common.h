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

#ifndef DV1_ASICDRV_TSN_COMMON_H__
#define DV1_ASICDRV_TSN_COMMON_H__

#include "rtl907xd_v1_api_tsn.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_table_struct.h"
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_regField_list.h"

EXTERN_API void      dv1_drv_tsn_cov_ns_to_ptptime(dv1_RtkTsnLongNanosecond_t longNs, dv1_RtkTsnPtpTime_t *ptptime);
EXTERN_API void      dv1_drv_tsn_multicycle_est(dv1_RtkTsnPtpTime_t baseTime, dv1_RtkTsnPtpTime_t currentTime, uint32 cycleTime, dv1_RtkTsnLongNanosecond_t *estimatedLongNs);
EXTERN_API RtkApiRet dv1_drv_tsn_current_time_get(uint32 unit, dv1_RtkTsnPtpTime_t *currentTime);
EXTERN_API void      dv1_drv_tsn_bigint_addition(dv1_RtkTsnLongNanosecond_t addened1st, dv1_RtkTsnLongNanosecond_t addened2nd, dv1_RtkTsnLongNanosecond_t *sum);
EXTERN_API int32     dv1_drv_tsn_bigint_comparison(dv1_RtkTsnLongNanosecond_t const *const a, dv1_RtkTsnLongNanosecond_t const *const b);

#endif  // _RTL906X_ASICDRV_TSN_COMMON_H__
