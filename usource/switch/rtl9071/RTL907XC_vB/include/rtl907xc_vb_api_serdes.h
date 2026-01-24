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

#ifndef CVB_RTK_API_SERDES_H__
#define CVB_RTK_API_SERDES_H__

#include "rtl907xc_vb_api.h"

/*Maximum value*/
#define CVB_SDS_PCS_MAX_REG_NUM   (31u)
#define CVB_SDS_PCS_MAX_PAGE_NUM  (54u)
#define CVB_SDS_APHY_MAX_REG_ADDR (0xFFFFu)
#define CVB_UMAC_REG_END_BIT      (15u)

/*FW virtual channel*/
#define CVB_SDS_APHY_TYPE (0u)
#define CVB_SDS_PCS_TYPE  (1u)

/*UMAC Register Address*/
#define CVB_PIPE_PHYMODE      (0x4A006130u)
#define CVB_S0_CON            (0x4A006020u)
#define CVB_SDS_A_IN_S0_R     (0x4A006024u)
#define CVB_SDS_D_OUT_S0_R    (0x4A006028u)
#define CVB_SDS_D_IN_S0_R     (0x4A00602Cu)
#define CVB_US0_CON           (0x4A006090u)
#define CVB_SDS_A_IN_US0_R    (0x4A006094u)
#define CVB_SDS_D_OUT_US0_R   (0x4A006098u)
#define CVB_SDS_D_IN_US0_R    (0x4A00609Cu)
#define CVB_US1_CON           (0x4A0060A0u)
#define CVB_SDS_A_IN_US1_R    (0x4A0060A4u)
#define CVB_SDS_D_OUT_US1_R   (0x4A0060A8u)
#define CVB_SDS_D_IN_US1_R    (0x4A0060ACu)
#define CVB_US2_CON           (0x4A0060B0u)
#define CVB_SDS_A_IN_US2_R    (0x4A0060B4u)
#define CVB_SDS_D_OUT_US2_R   (0x4A0060B8u)
#define CVB_SDS_D_IN_US2_R    (0x4A0060BCu)
#define CVB_S0_MDIO_CON       (0x4A006070u)
#define CVB_S0_MDIO_ADDR_R    (0x4A006074u)
#define CVB_S0_MDIO_DATA_O_R  (0x4A006078u)
#define CVB_S0_MDIO_DATA_I_R  (0x4A00607Cu)
#define CVB_US0_MDIO_CON      (0x4A0060C0u)
#define CVB_US0_MDIO_ADDR_R   (0x4A0060C4u)
#define CVB_US0_MDIO_DATA_O_R (0x4A0060C8u)
#define CVB_US0_MDIO_DATA_I_R (0x4A0060CCu)
#define CVB_US1_MDIO_CON      (0x4A0060D0u)
#define CVB_US1_MDIO_ADDR_R   (0x4A0060D4u)
#define CVB_US1_MDIO_DATA_O_R (0x4A0060D8u)
#define CVB_US1_MDIO_DATA_I_R (0x4A0060DCu)
#define CVB_US2_MDIO_CON      (0x4A0060E0u)
#define CVB_US2_MDIO_ADDR_R   (0x4A0060E4u)
#define CVB_US2_MDIO_DATA_O_R (0x4A0060E8u)
#define CVB_US2_MDIO_DATA_I_R (0x4A0060ECu)
#define CVB_WTG_REG_RDY       (0x4A0060F0u)

/**/
#define CVB_BOOT_UP_FULL_INITIZED       (0xBFu)
#define CVB_CPU_DMY_1_BOOTUP_STATUS_LSP (24u)
#define CVB_CPU_DMY_1_BOOTUP_STATUS_LEN (8u)

EXTERN_API RtkApiRet cvb_sys_sds_reg_getbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value);

EXTERN_API RtkApiRet cvb_sys_sds_reg_getbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value);

EXTERN_API RtkApiRet cvb_sys_sds_reg_setbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);

EXTERN_API RtkApiRet cvb_sys_sds_reg_setbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);

#endif /* CVB_RTK_API_SERDES_H__ */