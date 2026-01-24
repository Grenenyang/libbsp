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

#ifndef CVA_ASICDRV_TCAM_H__
#define CVA_ASICDRV_TCAM_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xc_va_api_ext.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* 31 is the max number of dataReg(CVA_STREAM_IDENTIFICATION_TABLE) */
#define CVA_TCAM_DATA_REG_MAX        (31u)
#define CVA_TCAM_VALID_OFFSET        (6u)
#define CVA_TCAM_ACL_PIE_MASK_OFFSET (7u)
/* before valid bit */
#define CVA_TCAM_CB_SID_MASK_OFFSET_BF_VALID (16u)
/* after valid bit */
#define CVA_TCAM_CB_SID_MASK_OFFSET_AF_VALID (15u)
/* before valid bit */
#define CVA_TCAM_RT_NETWORK_MASK_OFFSET_BF_VALID (12u)
/* after valid bit */
#define CVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID (11u)

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
EXTERN_API const cva_table_list_t cva_g_tableTypeList[CVA_TABLE_LIST_END];

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/

#ifdef RTK_INTERNAL_DEV
EXTERN_API RtkApiRet cva_drv_tcam_write_internal(uint32 unit, cva_table_list_t table, uint32 addr, const uint32 *data);
#else
EXTERN_API RtkApiRet cva_drv_tcam_write(uint32 unit, cva_table_list_t table, uint32 addr, const uint32 *data);
#endif /* RTK_INTERNAL_DEV */
EXTERN_API RtkApiRet cva_drv_tcam_check(uint32 unit, const cva_table_list_t table, const uint32 addr, const uint32 *const data);

#endif /* CVA_ASICDRV_TCAM_H__ */