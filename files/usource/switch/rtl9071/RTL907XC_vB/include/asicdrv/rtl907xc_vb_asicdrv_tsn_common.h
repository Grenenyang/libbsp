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

#ifndef CVB_ASICDRV_TSN_COMMON_H__
#define CVB_ASICDRV_TSN_COMMON_H__

#include "rtl907xc_vb_api_tsn.h"
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtl907xc_vb_reg_list.h"
#include "rtl907xc_vb_regField_list.h"
#include "rtkas_api_st.h"
EXTERN_API void      cvb_drv_tsn_cov_ns_to_ptptime(cvb_RtkTsnLongNanosecond_t longNs, RtkTsnPtpTime_t *ptptime);
EXTERN_API void      cvb_drv_tsn_multicycle_est(RtkTsnPtpTime_t baseTime, RtkTsnPtpTime_t currentTime, uint32 cycleTime, cvb_RtkTsnLongNanosecond_t *estimatedLongNs);
EXTERN_API RtkApiRet cvb_drv_tsn_current_time_get(uint32 unit, RtkTsnPtpTime_t *currentTime);
EXTERN_API void      cvb_drv_tsn_bigint_addition(cvb_RtkTsnLongNanosecond_t addened1st, cvb_RtkTsnLongNanosecond_t addened2nd, cvb_RtkTsnLongNanosecond_t *sum);
EXTERN_API int32     cvb_drv_tsn_bigint_comparison(cvb_RtkTsnLongNanosecond_t const *const a, cvb_RtkTsnLongNanosecond_t const *const b);

#endif  // _RTL906X_ASICDRV_TSN_COMMON_H__
