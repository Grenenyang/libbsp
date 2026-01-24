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

#ifndef CVB_ASICDRV_CONFIG_H_
#define CVB_ASICDRV_CONFIG_H_

#include "rtkas_common_types.h"

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern uint32 cvb_g_pureSDSPowerUp;

/******************************************************************************
 * Defines
 ******************************************************************************/
#define CVB_CFG_SPI_IDENTIFY           (0x8899u)
#define CVB_FLASH_CONFIG_READ_LENGTH   (0x10U)
#define CVB_FLASH_CONFIG_BUF_LEN       (0x20U)
#define CVB_CONFIG_SGMII_ENTRY_LEN     (5u)
#define CVB_CONFIG_MAC_ENTRY_LEN       (7u)
#define CVB_CONFIG_OCP_ENTRY_LEN       (4u)
#define CVB_CONFIG_FWCFG_BYTE_LEN      (3u)
#define CVB_CONFIG_TABLE0_HEADER_LEN   (4u)
#define CVB_CONFIG_TABLE1_HEADER_LEN   (4u)
#define CVB_CONFIG_TABLE2_HEADER_LEN   (2u)
#define CVB_CONFIG_SDS_ENTRY_LEN       (4u)
#define CVB_CONFIG_ROT_MTBL_LEN        (56u)
#define CVB_CONFIG_ROT_UTBL_LEN        (44u)
#define CVB_CONFIG_ROT_NET_LEN         (84u)
#define CVB_CFG_REG_FIELD_WR_ENTRY_LEN (9u)

#define CVB_EFU_IDENTIFY             (0x80u)
#define CVB_OTP_CFG_START_ADDR       (0x00u)
#define CVB_OTP_INTERNAL_CONFIG_ADDR (0x00C0u)
#define CVB_OTP_EXT_CFG_END          (CVB_OTP_INTERNAL_CONFIG_ADDR - 0x10u) /*for 6665 PHY extend*/

#define CVB_ROT_TRG_ROT_MTBL    (0x1u)
#define CVB_ROT_TRG_ROT_UTBL    (0x2u)
#define CVB_ROT_TRG_ROT_NETWORK (0x3u)
#define CVB_ROT_TRG_IDLE        (0x10u)
#define CVB_ROT_TRG_BUSY        (0x20u)
#define CVB_ROT_TRG_RDY         (0x30u)
#define CVB_ROT_TRG_DONE        (0x40u)

#define CVB_PORT_L2_MISC        (0xF00000u)
#define CVB_PORT_L2_MISC_MASK   (0xF0000Fu)
#define CVB_PORT_L2_MISC_OFFSET (0x800u)
#define CVB_RX_EN_BIT           ((uint32)1 << 1u)

#define CVB_BR_PORT_START   (2u)
#define CVB_BR_PORT_END     (5u)
#define CVB_RTPG_PORT_START (8u)
#define CVB_RTPG_PORT_END   (9u)

#define CVB_OTP_FLASHLESS_ENABLE_PORT (CVB_OTP_INTERNAL_CONFIG_ADDR + 0x0Cu)
#define CVB_RTL9075_EXT_PORT_MASK     (0x00001FFCu)

#define CVB_CONFIG_ETH_MODE_BIT (6u)

#define CVB_AESHASH_SHA256_LEN            (32u)
#define CVB_FLASH_HEADER_OFFSET           (0x00000020u)
#define CVB_FLASH_CONFIG_HEADER_LEN       (256u)
#define CVB_FLASH_DOMAIN_LEN              (3u)
#define CVB_CONFIG_BLOCK_ALIGNMENT_LENGTH (16u)
#define CVB_CONFIG_BLOCK_MAX_LENGTH       (1024u)
#define CVB_CONFIG_BLOCK_HASH_LENGTH      (CVB_AESHASH_SHA256_LEN)
#define CVB_MAXIMUM_BLOCK_NUMBER          (1024u)

#define CVB_NUM_OF_PORT_T1PHY (10u)

#define CVB_PAGE_SWR_OCP (0x4A000000u)
#define CVB_PAGE_SYS_MEM (0x00u)
#define CVB_PAGE_SW      (0x4Au)

#define CVB_FE_OCP_OFST_ADDR (0x0B0000u)

/*g_rtl9040Config*/
#define CVB_GLOBAL_CONFIG_SIZE (0x260u)
#define CVB_GLOBAL_CONFIG_ADDR (0x1AD5D8u)

/*g_rtl9040Config.g_BRPHYEnMsk*/
#define CVB_BR_PHY_EN_MSK_OFFSET (0x17Au)

/*g_rtl9040Config.serdesPatchMethod*/
#define CVB_SERDES_PATCH_OFFSET (0x1C8u)

/*cvb_g_rtGlbCfg*/
#define CVB_ROUTING_TBL_SIZE (0x8470u)
#define CVB_ROUTING_TBL_ADDR (0x1A254Cu)

/*g_mkaConfig*/
#define CVB_MKA_CONFIG_SIZE (0x400u)
#define CVB_MKA_CONFIG_ADDR (0x19D3C0u)

/*g_ptpStatus*/
#define CVB_PTP_STATUS_SIZE (0x2180u)
#define CVB_PTP_STATUS_ADDR (0x19FB60u)

#define CVB_CONFIG_TABLE_DATA_LEN (4u)
#define CVB_CONFIG_TABLE_CTRL_BIT (0x80000000u)
#define CVB_MAX_FW_LENGTH         (0xC0000u)

#define CVB_MPY_PAGE                  (0x4Eu)
#define CVB_MPY_NIC_BUF_ADDR_0        (0x4E070000u)
#define CVB_MPY_NIC_BUF_ADDR_1        (0x4E080000u)
#define CVB_MPY_NIC_BUF_SIZE          (0x2000u)
#define CVB_MPY_NIC_CONFIG_HEADER_LEN (8u)
#define CVB_MPY_NIC_CONFIG_LIMIT_LEN  ((uint32)CVB_MPY_NIC_BUF_SIZE - (uint32)CVB_MPY_NIC_CONFIG_HEADER_LEN)
#define CVB_MPY_NIC_CONFIG_MAGIC_NUM  (0x88998899u)

/*OPFSM WAKE O*/
#define CVB_WAKEO_CONFIG_FLASH_ADDRESST (0x80u)
#define CVB_WAKEO_GPIO1_FLASH_ADDRESST  (0x82u)
#define CVB_WAKEO_GPIO2_FLASH_ADDRESST  (0x84u)
#define CVB_ACTIVE_T1PHY_MASK           (0x033Cu)
#define CVB_OFST_OP_FSM_ADDR            (0x0800u)

/*g_someIpBufBase*/
#define CVB_SOME_IP_ADDR       (0x1ADBC8u)
#define CVB_SOME_IP_CFG_LENGTH (0x1400u)

#define CVB_TABLE_BASE_ADDR (0x52000000u)
#define CVB_TABLE_INDEX_GAP (0x40000u)

#define CVB_EXT_PHY_PATCH_CODE_ADDR (0x0017F010u)
#define CVB_EXT_PHY_PATCH_LEN_ADDR  (0x0017F00Cu)
#define CVB_EXT_PHY_PATCH_LENGTH    (2048u)
#define CVB_EXT_PHY_PATCH_ENTRY_LEN (3u)

/******************************************************************************
 * Typedef Structures
 ******************************************************************************/
typedef struct
{
    uint32 index;
    uint32 blockIdx;
    uint32 length;
} cvb_ConfigReg_t;

typedef struct
{
    uint32 index;
    uint32 blockIdx;
    uint32 length;
    uint32 configPort;
} cvb_ConfigEth_t;

typedef struct
{
    uint32 unit;
    uint32 index;
    uint32 imgStartAddr;
    uint32 imgParaLen;
    uint32 phyPortEnIdx;
    uint32 configPort;
} cvb_UpdateConfig_t;

typedef struct
{
    uint32  address;
    uint32  lsp;
    uint32  len;
    uint32 *value;
    uint32  num;
    uint32  timeoutValue;
    uint8   isEth;
} cvb_PollingReg_t;

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
typedef RtkApiRet (*cvb_ConfigHandlerReg)(cvb_ConfigReg_t configReg, const uint8 *buffer);
typedef RtkApiRet (*cvb_ConfigHandlerEth)(cvb_ConfigEth_t configEtg, const uint8 *buffer);
EXTERN_API RtkApiRet cvb_drv_config_parse_flash_cfg_header(const int8 *fileName, uint32 spiStartAddr[], uint32 spiParaLen[], uint8 stkIdx);
EXTERN_API RtkApiRet cvb_drv_config_parse_efuse_cfg_header(uint32 unit, uint8 efuStartAddr[], uint8 efuParaLen[]);
EXTERN_API RtkApiRet cvb_drv_config_efuse_cfg_ext_search(uint32 unit, uint8 *ext_start_addr, uint8 *endofOTP);
EXTERN_API RtkApiRet cvb_drv_config_eth_packet_assmeble(uint8 regCmd, uint32 regAddr, uint32 regVal, uint8 lastPacket);
EXTERN_API RtkApiRet cvb_drv_config_eth_tx_packet_assmeble_rx_decode(uint8 regCmd, uint32 regAddr, uint32 *regVal, uint8 lastPacket);
EXTERN_API RtkApiRet cvb_drv_config_check_enable_phy(uint32 *phyPortEnIdx);
EXTERN_API RtkApiRet cvb_drv_config_check_eth_enable_phy(uint32 *phyPortEnIdx);
EXTERN_API RtkApiRet cvb_drv_config_sys_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig);
EXTERN_API RtkApiRet cvb_drv_config_eth_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode);
EXTERN_API RtkApiRet cvb_drv_config_get_hdr_fw_info(void *fp, uint32 *fwAddr, uint32 *fwLength, uint8 *fwKeyType);
EXTERN_API RtkApiRet cvb_drv_config_eth_write_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 value);
EXTERN_API RtkApiRet cvb_drv_config_polling_status(uint32 unit, cvb_PollingReg_t pollingReg);

#endif /*_RTL906X_ASICDRV_CONFIG_H_*/
