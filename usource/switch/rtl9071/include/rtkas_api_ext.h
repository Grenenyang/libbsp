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

#ifndef RTKAS_API_EXT_H__
#define RTKAS_API_EXT_H__

/*
 * Include Files
 */
#include "rtkas_common_types.h"
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"

#if defined(RTL_907XD_V1)
#include <rtl907xd_v1_reg_define.h>
#include <rtl907xd_v1_reg_list.h>
#include <rtl907xd_v1_reg_struct.h>
#include <rtl907xd_v1_regField_list.h>
#include <rtl907xd_v1_table_struct.h>
#include <rtl907xd_v1_tableField_list.h>
#include <rtl907xd_v1_api.h>
#endif
#if defined(RTL_907XD_VA)
#include <rtl907xd_va_reg_define.h>
#include <rtl907xd_va_reg_list.h>
#include <rtl907xd_va_reg_struct.h>
#include <rtl907xd_va_regField_list.h>
#include <rtl907xd_va_table_struct.h>
#include <rtl907xd_va_tableField_list.h>
#include <rtl907xd_va_api.h>
#endif
#if defined(RTL_907XC_VB)
#include <rtl907xc_vb_reg_define.h>
#include <rtl907xc_vb_reg_list.h>
#include <rtl907xc_vb_reg_struct.h>
#include <rtl907xc_vb_regField_list.h>
#include <rtl907xc_vb_table_struct.h>
#include <rtl907xc_vb_tableField_list.h>
#include <rtl907xc_vb_api.h>
#endif
#if defined(RTL_907XC_VA)
#include <rtl907xc_va_reg_define.h>
#include <rtl907xc_va_reg_list.h>
#include <rtl907xc_va_reg_struct.h>
#include <rtl907xc_va_regField_list.h>
#include <rtl907xc_va_table_struct.h>
#include <rtl907xc_va_tableField_list.h>
#include <rtl907xc_va_api.h>
#endif

#endif /* RTKAS_API_EXT_H__ */
