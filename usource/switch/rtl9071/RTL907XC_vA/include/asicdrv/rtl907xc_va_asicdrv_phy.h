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

#ifndef CVA_ASICDRV_PHY_H_
#define CVA_ASICDRV_PHY_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_api_phy_test.h"

#define CVA_MAX_POLLCNT                    (1000u)
#define CVA_OCP_MR1                        (0xB10u)
#define CVA_OCP_BMCR                       (0xA400u)
#define CVA_PHY_RST_LSP                    (15u)
#define CVA_PHY_RST_LEN                    (1u)
#define CVA_PHY_DIGILBK_LSP                (14u)
#define CVA_PHY_DIGILBK_LEN                (1u)
#define CVA_PHY_SPD0_LSP                   (13u)
#define CVA_PHY_SPD0_LEN                   (1u)
#define CVA_PHY_AN_MODE_LSP                (12u)
#define CVA_PHY_AN_MODE_LEN                (1u)
#define CVA_PHY_PWRDN_LSP                  (11u)
#define CVA_PHY_PWRDN_LEN                  (1u)
#define CVA_PHY_MII_ISO_LSP                (10u)
#define CVA_PHY_MII_ISO_LEN                (1u)
#define CVA_PHY_AN_RESTART_LSP             (9u)
#define CVA_PHY_AN_RESTART_LEN             (1u)
#define CVA_PHY_REGDUP_LSP                 (8u)
#define CVA_PHY_REGDUP_LEN                 (1u)
#define CVA_PHY_SPD1_LSP                   (6u)
#define CVA_PHY_SPD1_LEN                   (1u)
#define CVA_OCP_BMSR                       (0xA402u)
#define CVA_PHY_BMSR_LINK_STS_LSP          (2u)
#define CVA_PHY_BMSR_LINK_STS_LEN          (1u)
#define CVA_OCP_PHYID1                     (0xA404u)
#define CVA_PHY_PHYID1_OUI_MSB_LSP         (0u)
#define CVA_PHY_PHYID1_OUI_MSB_LEN         (16u)
#define CVA_OCP_PHYID2                     (0xA406u)
#define CVA_PHY_PHYID2_OUI_LSB_LSP         (10u)
#define CVA_PHY_PHYID2_OUI_LSB_LEN         (6u)
#define CVA_PHY_PHYID2_MODEL_NUM_LSP       (4u)
#define CVA_PHY_PHYID2_MODEL_NUM_LEN       (6u)
#define CVA_PHY_PHYID2_REV_NUM_LSP         (0u)
#define CVA_PHY_PHYID2_REV_NUM_LEN         (4u)
#define CVA_OCP_PHYCR                      (0xA412u)
#define CVA_PHYCR_ROLE_LSP                 (11u)
#define CVA_PHYCR_ROLE_LEN                 (1u)
#define CVA_OCP_PHYSR                      (0xA414u)
#define CVA_PHY_PHYSR_ROLE_CFG_LSP         (14u)
#define CVA_PHY_PHYSR_ROLE_CFG_LEN         (1u)
#define CVA_PHY_PHYSR_LOC_RCV_STS_LSP      (13u)
#define CVA_PHY_PHYSR_LOC_RCV_STS_LEN      (1u)
#define CVA_PHY_PHYSR_REM_RCV_STS_LSP      (12u)
#define CVA_PHY_PHYSR_REM_RCV_STS_LEN      (1u)
#define CVA_OCP_PHYSR2                     (0xA434u)
#define CVA_PHY_PHYSR2_MASTER_MODE_LSP     (11u)
#define CVA_PHY_PHYSR2_MASTER_MODE_LEN     (1u)
#define CVA_PHY_PHYSR2_SPEED_LSP           (4u)
#define CVA_PHY_PHYSR2_SPEED_LEN           (2u)
#define CVA_PHY_PHYSR2_DUPLEX_LSP          (3u)
#define CVA_PHY_PHYSR2_DUPLEX_LEN          (1u)
#define CVA_PHY_PHYSR2_LINK_STS_LSP        (2u)
#define CVA_PHY_PHYSR2_LINK_STS_LEN        (1u)
#define CVA_OCP_PHYSR4                     (0xA64Eu)
#define CVA_PHY_PHYSR4_RX_POLARITY_STS_LSP (13u)
#define CVA_PHY_PHYSR4_RX_POLARITY_STS_LEN (1u)
#define CVA_PHY_PHYSR4_TX_POLARITY_STS_LSP (12u)
#define CVA_PHY_PHYSR4_TX_POLARITY_STS_LEN (1u)
#define CVA_PHY_PHYSR4_REMRCV_STS_LSP      (10u)
#define CVA_PHY_PHYSR4_REMRCV_STS_LEN      (1u)
#define CVA_PHY_PHYSR4_LOCRCV_STS_LSP      (2u)
#define CVA_PHY_PHYSR4_LOCRCV_STS_LEN      (1u)
#define CVA_OCP_PHYSFR                     (0xA420u)
#define CVA_PHY_PHYSFR_STS_SUB_FLAG_LSP    (0u)
#define CVA_PHY_PHYSFR_STS_SUB_FLAG_LEN    (3u)
#define CVA_OCP_PHY_PCS                    (0xA600u)
#define CVA_PHY_PCS_LSP                    (0u)
#define CVA_PHY_PCS_LEN                    (8u)
#define CVA_OCP_ETHPHYCR                   (0xA430u)
#define CVA_PHY_ETHPHYCR_MNL_MDI_EN_LSP    (9u)
#define CVA_PHY_ETHPHYCR_MNL_MDI_EN_LEN    (1u)
#define CVA_PHY_ETHPHYCR_MNL_MDI_VAL_LSP   (8u)
#define CVA_PHY_ETHPHYCR_MNL_MDI_VAL_LEN   (1u)
#define CVA_PHY_ETHPHYCR_PWR_SAVING_LSP    (2u)
#define CVA_PHY_ETHPHYCR_PWR_SAVING_LEN    (1u)
#define CVA_OCP_IPCR1                      (0xC800u)
#define CVA_PHY_IPCR1_REMOTE_LBK_LSP       (4u)
#define CVA_PHY_IPCR1_REMOTE_LBK_LEN       (2u)
#define CVA_PHY_IPCR1_PATH_SEL_LSP         (2u)
#define CVA_PHY_IPCR1_PATH_SEL_LEN         (2u)
#define CVA_PHY_IPCR1_EN_INT_CNT_LSP       (1u)
#define CVA_PHY_IPCR1_EN_INT_CNT_LEN       (1u)
#define CVA_OCP_IPCR2                      (0xC802u)
#define CVA_PHY_IPCR2_CLR_CNTERR_LSP       (6u)
#define CVA_PHY_IPCR2_CLR_CNTERR_LEN       (1u)
#define CVA_PHY_IPCR2_CLR_INTCNT_LSP       (5u)
#define CVA_PHY_IPCR2_CLR_INTCNT_LEN       (1u)
#define CVA_PHY_IPCR2_CLR_RXCNT_LSP        (1u)
#define CVA_PHY_IPCR2_CLR_RXCNT_LEN        (1u)
#define CVA_OCP_IPCR3                      (0xC810u)
#define CVA_OCP_IPCR4                      (0xC812u)
#define CVA_OCP_IEPCR                      (0xC814u)
#define CVA_OCP_AUTOPOLARITY               (0xA58Au)
#define CVA_OCP_PHY_COMBO_100TSTMOD        (0xA592u)
#define CVA_OCP_PHY_100TSTMOD              (0xA5BAu)
#define CVA_OCP_PHY_10TSTMOD               (0xA596u)
#define CVA_OCP_CEN4                       (0xBC48u)
#define CVA_OCP_CEN5                       (0xBC4Au)
#define CVA_OCP_CEN9                       (0xBC52u)
#define CVA_OCP_CEN10                      (0xBC54u)
#define CVA_OCP_PKTGEN_TXNUM0_15           (0xC808u)
#define CVA_OCP_PKTGEN_TXNUM16_31          (0xC80Au)
#define CVA_OCP_PKTGEN_TXCNT0_15           (0xC80Cu)
#define CVA_OCP_PKTGEN_TXCNT16_31          (0xC80Eu)
#define CVA_OCP_GPHYCORE_OE                (0xD000u)
#define CVA_OCP_GPHYCORE_DBGSEL01          (0xD002u)
#define CVA_OCP_SQI_INDEX                  (0xD106u)
#define CVA_OCP_OPFSM_P0_OP_MODE_LOCAL     (0x920u)
#define CVA_OCP_OPFSM_P1_OP_MODE_LOCAL     (0x922u)
#define CVA_OCP_OPFSM_P2_OP_MODE_LOCAL     (0x924u)
#define CVA_OCP_OPFSM_P3_OP_MODE_LOCAL     (0x926u)
#define CVA_OCP_OPFSM_P4_OP_MODE_LOCAL     (0x928u)
#define CVA_OCP_R                          (0xBCE0u)
#define CVA_OCP_RC                         (0xBCDCu)
#define CVA_OCP_RRCCTLREG                  (0xA472u)
#define CVA_OCP_UCACC_ADDR                 (0xA436u)
#define CVA_OCP_UCACC_DATA                 (0xA438u)
#define CVA_OCP_GIGA_CTRL_REG01            (0xBC00u)
#define CVA_OCP_SNRR                       (0xA8C0u)
#define CVA_OCP_SNROR                      (0xACC0u)
#define CVA_OCP_SLP_CAP_100T1              (0xA5A8u)
#define CVA_OCP_SLP_CAP_GIGAPHY            (0xA516u)
#define CVA_OCP_OAM_CAP_GIGAPHY            (0xA598u)
#define CVA_OCP_SLPCR                      (0xA42Au)
#define CVA_OCP_RTCT_CTL                   (0xA422u)

/*Offset for channel
 */
#define CVA_RG_I2C_STATUS             (0X0000u)
#define CVA_RG_I2C_EAAR               (0X0004u)
#define CVA_RG_I2C_CONTFIELD_TYPE_ROL (0X0008u)
#define CVA_REG_CEN0                  (0X000Cu)
#define CVA_REG_CEN1                  (0X0010u)
#define CVA_REG_CEN2                  (0X0014u)
#define CVA_REG0                      (0X0018u)
#define CVA_REG1                      (0X001Cu)
#define CVA_REG2                      (0X0020u)
#define CVA_REG3                      (0X0024u)
#define CVA_REG4                      (0X0028u)
#define CVA_REG5                      (0X002Cu)
#define CVA_REG6                      (0X0030u)
#define CVA_REG7                      (0X0034u)
#define CVA_REG8                      (0X0038u)
#define CVA_REG9                      (0X003Cu)
#define CVA_REG10                     (0X0040u)
#define CVA_REG11                     (0X0044u)
#define CVA_REG12                     (0X0048u)
#define CVA_REG13                     (0X004Cu)
#define CVA_REG14                     (0X0050u)
#define CVA_REG15                     (0X0054u)
#define CVA_REG16                     (0X0058u)
#define CVA_REG17                     (0X005Cu)
#define CVA_REG18                     (0X0060u)
#define CVA_REG19                     (0X0064u)
#define CVA_REG20                     (0X0068u)
#define CVA_REG21                     (0X006Cu)
#define CVA_REG22                     (0X0070u)
#define CVA_REG23                     (0X0074u)
#define CVA_REG24                     (0X0078u)
#define CVA_REG25                     (0X007Cu)
#define CVA_REG26                     (0X0080u)
#define CVA_REG27                     (0X0084u)
#define CVA_REG28                     (0X0088u)
#define CVA_REG29                     (0X008Cu)
#define CVA_REG30                     (0X0090u)
#define CVA_REG31                     (0X0094u)
#define CVA_REG_LDO0                  (0X0098u)
#define CVA_REG_LDO1                  (0X009Cu)
#define CVA_EN_DCO                    (0X00A0u)
#define CVA_EN_GDAC_IBX_CEN           (0X00A4u)
#define CVA_EN_OTPLDO                 (0X00A8u)
#define CVA_POW_FE                    (0X00ACu)
#define CVA_POW_XTAL_LDO              (0X00B0u)
#define CVA_FE_R_CALI_EN              (0X00B4u)
#if 0 /* redeclaration in rtl906x_reg_list */
#define CVA_PLL_PARAM_0  (0X00B8u)
#define CVA_PLL_PARAM_1  (0X00BCu)
#define CVA_PLL_PARAM_2  (0X00C0u)
#define CVA_PLL_PARAM_3  (0X00C4u)
#define CVA_PLL_PARAM_4  (0X00C8u)
#define CVA_PLL_PARAM_5  (0X00CCu)
#define CVA_PLL_PARAM_6  (0X00D0u)
#define CVA_PLL_PARAM_7  (0X00D4u)
#define CVA_PLL_PARAM_8  (0X00D8u)
#define CVA_PLL_PARAM_9  (0X00DCu)
#define CVA_PLL_PARAM_10 (0X00E0u)
#define CVA_PLL_PARAM_11 (0X00E4u)
#define CVA_PLL_PARAM_12 (0X00E8u)
#define CVA_PLL_PARAM_13 (0X00ECu)
#endif
#define CVA_REG_POW_MCK                 (0X00F0u)
#define CVA_REG_FASTON_LDO_L            (0X00F4u)
#define CVA_REG_RSTB_PLL_L              (0X00F8u)
#define CVA_REG_FASTLOCK_CCO_L          (0X00FCu)
#define CVA_REG_ICP_SW_L                (0X0100u)
#define CVA_REG_VC_FORCE_L              (0X0104u)
#define CVA_REG_RSTB_DLL_L              (0X0108u)
#define CVA_REG_START_DUTY_CONSTRAINT_L (0X010Cu)
#define CVA_REG_POW_PI_L                (0X0110u)
#define CVA_REG_PI_OUT_EN_L             (0X0114u)
#define CVA_REG_RSTB_SSCPS_L            (0X0118u)
#define CVA_REG_ANARSTB_RX              (0X011Cu)
#define CVA_REG_ANARSTB_PTP             (0X0120u)
#define CVA_REG_ANARSTB_SYS             (0X0124u)
#define CVA_REG_ANARSTB_RMII            (0X0128u)
#define CVA_P_REG_TRIG                  (0X012Cu)
#define CVA_P_SW_FULL_CTRL              (0X0130u)
#define CVA_P_I2C_ON                    (0X0134u)
#define CVA_REG_RESERVED_0              (0X0138u)
#define CVA_REG_RESERVED_1              (0X013Cu)
#define CVA_REG_RESERVED_2              (0X0140u)
#define CVA_REG_RESERVED_3              (0X0144u)
#define CVA_REG_RESERVED_4              (0X0148u)

#define CVA_PHY_ACCESS (0)
#define CVA_MMD_ACCESS (1)
#define CVA_TICK_100US (100u)
/* us unit */
#define CVA_TIMEOUT_5MS (5000u)

#define CVA_AFE_REG_6_REG_125M_ATP_ATN_LSP (8u)
#define CVA_AFE_REG_6_REG_125M_ATP_ATN_LEN (5u)

#define CVA_SQI_10M_P2P_REG       (0xA50Cu)
#define CVA_SQI_10M_INDEX_REG     (0xA50Eu)
#define CVA_SQI_100M_LINK_STA_REG (0xA600u)

EXTERN_API RtkApiRet cva_drv_phy_reg_read(uint32 unit, uint8 phyAddr, uint8 page, uint8 regAddr, uint16 *regData);
EXTERN_API RtkApiRet cva_drv_phy_reg_write(uint32 unit, uint8 phyAddr, uint8 page, uint8 regAddr, uint16 regData);

EXTERN_API RtkApiRet cva_drv_mmd_reg_read(uint32 unit, uint8 phyAddr, uint8 deviceID, uint16 regAddr, uint16 *regData);
EXTERN_API RtkApiRet cva_drv_mmd_reg_write(uint32 unit, uint8 phyAddr, uint8 deviceID, uint16 regAddr, uint16 regData);

EXTERN_API RtkApiRet cva_drv_giga_phy_ext_read(uint32 unit, uint8 page, uint8 regAddr, uint16 *regData);
EXTERN_API RtkApiRet cva_drv_giga_phy_ext_write(uint32 unit, uint8 page, uint8 regAddr, uint16 regData);

EXTERN_API RtkApiRet cva_drv_b_phy_sqi_get(uint32 unit, uint32 port, uint8 speed, uint8 *pSqi);
EXTERN_API RtkApiRet cva_drv_r_phy_sqi_get(uint32 unit, uint32 port, uint8 speed, uint8 *pSqi);
EXTERN_API RtkApiRet cva_drv_phy_10base_testmode_set(uint32 unit, uint32 port, uint8 const testMode);
EXTERN_API RtkApiRet cva_drv_phy_100base_testmode_set(uint32 unit, uint32 port, uint8 const testMode);
EXTERN_API RtkApiRet cva_drv_phy_1000base_testmode_set(uint32 unit, uint32 port, uint8 const testMode, uint8 isMaster);
EXTERN_API RtkApiRet cva_drv_phy_patch_request(uint32 unit, uint32 port, uint8 mode, uint8 lastPacket);
EXTERN_API RtkApiRet cva_drv_phy_patch_release(uint32 unit, uint32 port, uint8 mode, uint8 lastPacket);
EXTERN_API RtkApiRet cva_drv_wait_op_finished(uint32 unit);
EXTERN_API RtkApiRet cva_drv_userport_to_sysport(uint8 port, uint32 *sysPort);
EXTERN_API RtkApiRet cva_drv_sysport_to_ocpaddr(uint32 sysPort, uint32 *ocpAddr);
EXTERN_API RtkApiRet cva_drv_valid_phytest_userport_check(uint8 port);
EXTERN_API RtkApiRet cva_drv_phy_100tm_tx_clk66_set(uint32 unit, uint32 prtSel);
EXTERN_API RtkApiRet cva_drv_phy_1000tm_tx_clk125_set(uint32 unit, uint32 sysPort, uint32 prtSel, uint8 isMaster);
EXTERN_API RtkApiRet cva_drv_rtct_status_get(uint32 unit, uint32 portNum, uint32 *rtctStatus, uint32 *rtctDetect);

EXTERN_API RtkApiRet cva_drv_phy_channel_get(uint32 unit, uint32 offset, uint32 *value);
EXTERN_API RtkApiRet cva_drv_phy_channel_set(uint32 unit, uint32 offset, uint32 value);

EXTERN_API RtkApiRet cva_drv_is_link_up_for_sqi(uint32 unit, uint8 userPort);

#endif /*#ifndef _RTL906X_ASICDRV_PHY_H_*/
