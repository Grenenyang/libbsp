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

#ifndef DV1_ASICDRV_CONFIG_H_
#define DV1_ASICDRV_CONFIG_H_

#include "rtkas_common_types.h"

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern uint32 dv1_g_wrapSDSNotPowerUp;

/******************************************************************************
 * Defines
 ******************************************************************************/
#define DV1_CFG_SPI_IDENTIFY           (0x8899u)
#define DV1_FLASH_CONFIG_READ_LENGTH   (0x10U)
#define DV1_FLASH_CONFIG_BUF_LEN       (0x20U)
#define DV1_CONFIG_SGMII_ENTRY_LEN     (6u)
#define DV1_CONFIG_MAC_ENTRY_LEN       (7u)
#define DV1_CONFIG_OCP_ENTRY_LEN       (4u)
#define DV1_CONFIG_FWCFG_BYTE_LEN      (3u)
#define DV1_CONFIG_TABLE0_HEADER_LEN   (4u)
#define DV1_CONFIG_TABLE1_HEADER_LEN   (4u)
#define DV1_CONFIG_TABLE2_HEADER_LEN   (2u)
#define DV1_CONFIG_SDS_ENTRY_LEN       (4u)
#define DV1_CONFIG_ROT_MTBL_LEN        (56u)
#define DV1_CONFIG_ROT_UTBL_LEN        (44u)
#define DV1_CONFIG_ROT_NET_LEN         (84u)
#define DV1_CFG_REG_FIELD_WR_ENTRY_LEN (9u)

#define DV1_EFU_IDENTIFY             (0x80u)
#define DV1_OTP_CFG_START_ADDR       (0x00u)
#define DV1_OTP_INTERNAL_CONFIG_ADDR (0x00C0u)
#define DV1_OTP_EXT_CFG_END          (DV1_OTP_INTERNAL_CONFIG_ADDR - 0x10u) /*for 6665 PHY extend*/

#define DV1_ROT_TRG_ROT_MTBL    (0x1u)
#define DV1_ROT_TRG_ROT_UTBL    (0x2u)
#define DV1_ROT_TRG_ROT_NETWORK (0x3u)
#define DV1_ROT_TRG_IDLE        (0x10u)
#define DV1_ROT_TRG_BUSY        (0x20u)
#define DV1_ROT_TRG_RDY         (0x30u)
#define DV1_ROT_TRG_DONE        (0x40u)

#define DV1_PORT_L2_MISC        (0xF00014u)
#define DV1_PORT_L2_MISC_MASK   (0xF000FFu)
#define DV1_PORT_L2_MISC_OFFSET (0x800u)
#define DV1_PORT_CTRL           (0xC10000u)
#define DV1_PORT_CTRL_MASK      (0xC100FFu)
#define DV1_PORT_CTRL_OFFSET    (0x800u)
#define DV1_RX_EN_BIT           ((uint32)1 << 1u)

/*BR reserved for future, 6764 action as same as RTPG*/
#define DV1_BR_PORT_START   (0u)
#define DV1_BR_PORT_END     (5u)
#define DV1_RTPG_PORT_START (0u)
#define DV1_RTPG_PORT_END   (5u)

#define DV1_ROT_PATCH_SRAM     (0x57800u)
#define DV1_ROT_TRG_STATE_SRAM (DV1_ROT_PATCH_SRAM + DV1_ROT_TRG_OFFSET)
#define DV1_ROT_TRG_OFFSET     (100u)

#define DV1_OTP_FLASHLESS_ENABLE_PORT (DV1_OTP_INTERNAL_CONFIG_ADDR + 0x0Cu)
#define DV1_RTL9075_EXT_PORT_MASK     (0x00001FFCu)

#define DV1_CONFIG_ETH_MODE_BIT (6u)

#define DV1_AESHASH_SHA256_LEN            (32u)
#define DV1_FLASH_HEADER_OFFSET           (0x00000020u)
#define DV1_FLASH_CONFIG_HEADER_LEN       (256u)
#define DV1_FLASH_DOMAIN_LEN              (3u)
#define DV1_CONFIG_BLOCK_ALIGNMENT_LENGTH (16u)
#define DV1_CONFIG_BLOCK_MAX_LENGTH       (1024u)
#define DV1_CONFIG_BLOCK_HASH_LENGTH      (DV1_AESHASH_SHA256_LEN)
#define DV1_MAXIMUM_BLOCK_NUMBER          (1024u)

#define DV1_NUM_OF_PORT_T1PHY (10u)

#define DV1_PAGE_SWR_OCP (0x4A000000u)
#define DV1_PAGE_SYS_MEM (0x01u)
#define DV1_PAGE_SW      (0x4Au)

#define DV1_FE_OCP_OFST_ADDR  (0x0B0000u)
#define DV1_ADDR_BOOTCFG_BASE (0x010C0000u)
#define DV1_ADDR_BOOTCFG_SIZE (0x00003000u)

#define DV1_BR_PHY_EN_MSK_ADDR (0x010C000Cu)

#define DV1_ROUTING_TBL_SIZE (0x4800u)
#define DV1_ROUTING_TBL_ADDR (0x010ECC76u)

#define DV1_ROUTING_INTF_TBL_SIZE (0x800u)
#define DV1_ROUTING_INTF_TBL_ADDR (0x010EC476u)

#define DV1_MKA_CONFIG_SIZE (0x400u)
#define DV1_MKA_CONFIG_ADDR (0x010C01C8u)

#define DV1_PTP_STATUS_SIZE (0x2190u)
#define DV1_PTP_STATUS_ADDR (0x010C05D0u)

#define DV1_CONFIG_TABLE_DATA_LEN (4u)
#define DV1_CONFIG_TABLE_CTRL_BIT (0x80000000u)
#define DV1_MAX_FW_LENGTH         (0xC0000u)

#define DV1_MPY_PAGE                  (0x09u)
#define DV1_MPY_NIC_BUF_ADDR_0        (0x09020000u)
#define DV1_MPY_NIC_BUF_ADDR_1        (0x09030000u)
#define DV1_MPY_NIC_BUF_SIZE          (0x2000u)
#define DV1_MPY_NIC_CONFIG_HEADER_LEN (8u)
#define DV1_MPY_NIC_CONFIG_LIMIT_LEN  ((uint32)DV1_MPY_NIC_BUF_SIZE - (uint32)DV1_MPY_NIC_CONFIG_HEADER_LEN)
#define DV1_MPY_NIC_CONFIG_MAGIC_NUM  (0x88998899u)

/*OPFSM WAKE Output*/
#define DV1_WAKEO_CONFIG_FLASH_ADDRESST (0x80u)
#define DV1_WAKEO_GPIO1_FLASH_ADDRESST  (0x82u)
#define DV1_WAKEO_GPIO2_FLASH_ADDRESST  (0x84u)
#define DV1_ACTIVE_T1PHY_MASK           (0x003Fu)
#define DV1_OFST_OP_FSM_ADDR            (0x0800u)

/*SERDES 5GKR*/
#define DV1_SERDES_PATCH_METHOD_ADDRESS (0x10C2800u)

#define DV1_TABLE_MAC_BASE_ADDR (0x50000000u)
#define DV1_TABLE_ALE_BASE_ADDR (0x51000000u)
#define DV1_TABLE_INDEX_GAP     (0x40000u)
#define DV1_TABLE_ENTRY_MAX     (8192u)

#define DV1_EXT_PHY_PATCH_CODE_ADDR (0x010BF00Cu)
#define DV1_EXT_PHY_PATCH_LEN_ADDR  (0x010BF008u)
#define DV1_EXT_PHY_PATCH_LENGTH    (2048u)
#define DV1_EXT_PHY_PATCH_ENTRY_LEN (3u)

/******************************************************************************
 * Typedef Structures
 ******************************************************************************/
typedef struct
{
    uint32 index;
    uint32 blockIdx;
    uint32 length;
} dv1_ConfigReg_t;

typedef struct
{
    uint32 index;
    uint32 blockIdx;
    uint32 length;
    uint32 configPort;
} dv1_ConfigEth_t;

typedef struct
{
    uint32 unit;
    uint32 index;
    uint32 imgStartAddr;
    uint32 imgParaLen;
    uint32 phyPortEnIdx;
    uint32 configPort;
} dv1_UpdateConfig_t;

typedef struct
{
    uint32  address;
    uint32  lsp;
    uint32  len;
    uint32 *value;
    uint32  num;
    uint32  timeoutValue;
    uint8   isEth;
} dv1_PollingReg_t;

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
typedef RtkApiRet (*dv1_ConfigHandlerReg)(dv1_ConfigReg_t configReg, const uint8 *buffer);
typedef RtkApiRet (*dv1_ConfigHandlerEth)(dv1_ConfigEth_t configEtg, const uint8 *buffer);
EXTERN_API RtkApiRet dv1_drv_config_parse_flash_cfg_header(const int8 *fileName, uint32 spiStartAddr[], uint32 spiParaLen[], uint8 stkIdx);
EXTERN_API RtkApiRet dv1_drv_config_parse_efuse_cfg_header(uint32 unit, uint8 efuStartAddr[], uint8 efuParaLen[]);
EXTERN_API RtkApiRet dv1_drv_config_efuse_cfg_ext_search(uint32 unit, uint8 *ext_start_addr, uint8 *endofOTP);
EXTERN_API RtkApiRet dv1_drv_config_eth_packet_assmeble(uint8 regCmd, uint32 regAddr, uint32 regVal, uint8 lastPacket);
EXTERN_API RtkApiRet dv1_drv_config_eth_tx_packet_assmeble_rx_decode(uint8 regCmd, uint32 regAddr, uint32 *regVal, uint8 lastPacket);
EXTERN_API RtkApiRet dv1_drv_config_check_enable_phy(uint32 *phyPortEnIdx);
EXTERN_API RtkApiRet dv1_drv_config_check_eth_enable_phy(uint32 *phyPortEnIdx);
EXTERN_API RtkApiRet dv1_drv_config_sys_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig);
EXTERN_API RtkApiRet dv1_drv_config_eth_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode);
EXTERN_API RtkApiRet dv1_drv_config_get_hdr_fw_info(void *fp, uint32 *fwAddr, uint32 *fwLength, uint8 *fwKeyType);
EXTERN_API RtkApiRet dv1_drv_config_eth_write_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 value);
EXTERN_API RtkApiRet dv1_drv_config_update_Aes_Ctr_Iv(uint32 unit, void *fp, const uint32 fwKeyType, const uint32 fwLength);
EXTERN_API RtkApiRet dv1_drv_config_eth_update_aes_ctr_iv(void *fp, const uint32 fwKeyType, const uint32 fwLength);
EXTERN_API RtkApiRet dv1_drv_config_polling_status(uint32 unit, dv1_PollingReg_t pollingReg);

#endif /*_RTL906X_ASICDRV_CONFIG_H_*/
