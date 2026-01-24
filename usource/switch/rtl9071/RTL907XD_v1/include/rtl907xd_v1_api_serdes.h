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

#ifndef DV1_RTK_API_SERDES_H__
#define DV1_RTK_API_SERDES_H__

#include "rtl907xd_v1_api.h"

/*Maximum value*/
#define DV1_SDS_PCS_MAX_REG_NUM   (31u)
#define DV1_SDS_PCS_MAX_PAGE_NUM  (54u)
#define DV1_SDS_APHY_MAX_REG_ADDR (0xFFFFu)
#define DV1_UMAC_REG_END_BIT      (15u)

/*FW virtual channel*/
#define DV1_SDS_APHY_TYPE (0u)
#define DV1_SDS_PCS_TYPE  (1u)

/**/
#define DV1_BOOT_UP_FULL_INITIZED       (0xBFu)
#define DV1_CPU_DMY_1_BOOTUP_STATUS_LSP (24u)
#define DV1_CPU_DMY_1_BOOTUP_STATUS_LEN (8u)

EXTERN_API RtkApiRet dv1_sys_sds_reg_getbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value);
EXTERN_API RtkApiRet dv1_sys_sds_reg_getbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value);
EXTERN_API RtkApiRet dv1_sys_sds_reg_setbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet dv1_sys_sds_reg_setbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);

#endif /* DV1_RTK_API_SERDES_H__ */