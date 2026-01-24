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

#ifndef DVA_RTK_API_SERDES_H__
#define DVA_RTK_API_SERDES_H__

#include "rtl907xd_va_api.h"
#include "rtkas_api_interface.h"

/*Maximum value*/
#define DVA_SDS_PCS_MAX_REG_NUM   (31u)
#define DVA_SDS_PCS_MAX_PAGE_NUM  (54u)
#define DVA_SDS_APHY_MAX_REG_ADDR (0xFFFFu)
#define DVA_UMAC_REG_END_BIT      (15u)

/*FW virtual channel*/
#define DVA_SDS_APHY_TYPE (0u)
#define DVA_SDS_PCS_TYPE  (1u)

EXTERN_API RtkApiRet dva_sys_sds_reg_getbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value);
EXTERN_API RtkApiRet dva_sys_sds_reg_getbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value);
EXTERN_API RtkApiRet dva_sys_sds_reg_setbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet dva_sys_sds_reg_setbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
EXTERN_API RtkApiRet dva_sys_sds_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e serdesMode, uint32 portRole, uint32 isForceMode, uint32 speed, uint32 portSpeed);
EXTERN_API RtkApiRet dva_sds_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e serdesMode, uint32 status);
#endif /* DVA_RTK_API_SERDES_H__ */