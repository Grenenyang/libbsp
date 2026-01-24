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

#ifndef CVA_ASICDRV_CONFIG_H_
#define CVA_ASICDRV_CONFIG_H_

#include "rtkas_common_types.h"

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern uint32 cva_g_pureSDSPowerUp;

/******************************************************************************
 * Defines
 ******************************************************************************/
#define CVA_CFG_SPI_IDENTIFY           (0x8899u)
#define CVA_FLASH_CONFIG_READ_LENGTH   (0x10U)
#define CVA_FLASH_CONFIG_BUF_LEN       (0x20U)
#define CVA_CONFIG_SGMII_ENTRY_LEN     (5u)
#define CVA_CONFIG_MAC_ENTRY_LEN       (7u)
#define CVA_CONFIG_OCP_ENTRY_LEN       (4u)
#define CVA_CONFIG_FWCFG_BYTE_LEN      (3u)
#define CVA_CONFIG_TABLE0_HEADER_LEN   (4u)
#define CVA_CONFIG_TABLE1_HEADER_LEN   (4u)
#define CVA_CONFIG_TABLE2_HEADER_LEN   (2u)
#define CVA_CONFIG_SDS_ENTRY_LEN       (4u)
#define CVA_CONFIG_ROT_MTBL_LEN        (56u)
#define CVA_CONFIG_ROT_UTBL_LEN        (44u)
#define CVA_CONFIG_ROT_NET_LEN         (84u)
#define CVA_CFG_REG_FIELD_WR_ENTRY_LEN (9u)

#define CVA_EFU_IDENTIFY             (0x80u)
#define CVA_OTP_CFG_START_ADDR       (0x00u)
#define CVA_OTP_INTERNAL_CONFIG_ADDR (0x00C0u)
#define CVA_OTP_EXT_CFG_END          (CVA_OTP_INTERNAL_CONFIG_ADDR - 0x10u) /*for 6665 PHY extend*/

#define CVA_ROT_TRG_ROT_MTBL    (0x1u)
#define CVA_ROT_TRG_ROT_UTBL    (0x2u)
#define CVA_ROT_TRG_ROT_NETWORK (0x3u)
#define CVA_ROT_TRG_IDLE        (0x10u)
#define CVA_ROT_TRG_BUSY        (0x20u)
#define CVA_ROT_TRG_RDY         (0x30u)
#define CVA_ROT_TRG_DONE        (0x40u)

#define CVA_PORT_L2_MISC        (0xF00000u)
#define CVA_PORT_L2_MISC_MASK   (0xF0000Fu)
#define CVA_PORT_L2_MISC_OFFSET (0x800u)
#define CVA_RX_EN_BIT           ((uint32)1 << 1u)

#define CVA_BR_PORT_START   (2u)
#define CVA_BR_PORT_END     (5u)
#define CVA_RTPG_PORT_START (8u)
#define CVA_RTPG_PORT_END   (9u)

#define CVA_ROT_PATCH_SRAM     (0x57800u)
#define CVA_ROT_TRG_STATE_SRAM (CVA_ROT_PATCH_SRAM + CVA_ROT_TRG_OFFSET)
#define CVA_ROT_TRG_OFFSET     (100u)

#define CVA_OTP_FLASHLESS_ENABLE_PORT (CVA_OTP_INTERNAL_CONFIG_ADDR + 0x0Cu)
#define CVA_RTL9075_EXT_PORT_MASK     (0x00001FFCu)

#define CVA_CONFIG_ETH_MODE_BIT (6u)

#define CVA_AESHASH_SHA256_LEN            (32u)
#define CVA_FLASH_HEADER_OFFSET           (0x00000020u)
#define CVA_FLASH_CONFIG_HEADER_LEN       (256u)
#define CVA_FLASH_DOMAIN_LEN              (3u)
#define CVA_CONFIG_BLOCK_ALIGNMENT_LENGTH (16u)
#define CVA_CONFIG_BLOCK_MAX_LENGTH       (1024u)
#define CVA_CONFIG_BLOCK_HASH_LENGTH      (CVA_AESHASH_SHA256_LEN)
#define CVA_MAXIMUM_BLOCK_NUMBER          (1024u)

#define CVA_NUM_OF_PORT_T1PHY (10u)

#define CVA_PAGE_SWR_OCP (0x4A000000u)

#define CVA_FE_OCP_OFST_ADDR (0x0B0000u)

#define CVA_GLOBAL_CONFIG_SIZE   (0x25Cu)
#define CVA_GLOBAL_CONFIG_ADDR   (0x1AC89Cu)
#define CVA_BR_PHY_EN_MSK_OFFSET (0x17Au)
#define CVA_SERDES_PATCH_OFFSET  (0x1C8u)

#define CVA_ROUTING_TBL_SIZE (0x4800u)
#define CVA_ROUTING_TBL_ADDR (0x1A7860u)

#define CVA_ROUTING_INTF_TBL_SIZE (0x800u)
#define CVA_ROUTING_INTF_TBL_ADDR (0x1A7060u)

#define CVA_MKA_CONFIG_SIZE (0x3E0u)
#define CVA_MKA_CONFIG_ADDR (0x19BA20u)

#define CVA_PTP_STATUS_SIZE (0x2180u)
#define CVA_PTP_STATUS_ADDR (0x1A4E88u)

#define CVA_CONFIG_TABLE_DATA_LEN (4u)
#define CVA_CONFIG_TABLE_CTRL_BIT (0x80000000u)
#define CVA_MAX_FW_LENGTH         (0xC0000u)

#define CVA_MPY_PAGE                  (0x4Eu)
#define CVA_MPY_NIC_BUF_ADDR_0        (0x4E070000u)
#define CVA_MPY_NIC_BUF_ADDR_1        (0x4E080000u)
#define CVA_MPY_NIC_BUF_SIZE          (0x2000u)
#define CVA_MPY_NIC_CONFIG_HEADER_LEN (8u)
#define CVA_MPY_NIC_CONFIG_LIMIT_LEN  ((uint32)CVA_MPY_NIC_BUF_SIZE - (uint32)CVA_MPY_NIC_CONFIG_HEADER_LEN)
#define CVA_MPY_NIC_CONFIG_MAGIC_NUM  (0x88998899u)

/*OPFSM WAKE O*/
#define CVA_WAKEO_CONFIG_FLASH_ADDRESST (0x80u)
#define CVA_WAKEO_GPIO1_FLASH_ADDRESST  (0x82u)
#define CVA_WAKEO_GPIO2_FLASH_ADDRESST  (0x84u)
#define CVA_ACTIVE_T1PHY_MASK           (0x033Cu)
#define CVA_OFST_OP_FSM_ADDR            (0x0800u)

#define CVA_SOME_IP_ADDR       (0x0017EC00u)
#define CVA_SOME_IP_CFG_LENGTH (0x1400u)

#define CVA_TABLE_BASE_ADDR (0x52000000u)
#define CVA_TABLE_INDEX_GAP (0x40000u)

#define CVA_EXT_PHY_PATCH_CODE_ADDR (0x0017E00Cu)
#define CVA_EXT_PHY_PATCH_LEN_ADDR  (0x0017E80Cu)
#define CVA_EXT_PHY_PATCH_LENGTH    (2048u)
#define CVA_EXT_PHY_PATCH_ENTRY_LEN (3u)

/******************************************************************************
 * Typedef Structures
 ******************************************************************************/
typedef struct
{
    uint32 index;
    uint32 blockIdx;
    uint32 length;
} cva_ConfigReg_t;

typedef struct
{
    uint32 index;
    uint32 blockIdx;
    uint32 length;
    uint32 configPort;
} cva_ConfigEth_t;

typedef struct
{
    uint32 unit;
    uint32 index;
    uint32 imgStartAddr;
    uint32 imgParaLen;
    uint32 phyPortEnIdx;
    uint32 configPort;
} cva_UpdateConfig_t;

typedef struct
{
    uint32  address;
    uint32  lsp;
    uint32  len;
    uint32 *value;
    uint32  num;
    uint32  timeoutValue;
    uint8   isEth;
} cva_PollingReg_t;

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
typedef RtkApiRet (*ConfigHandlerReg)(cva_ConfigReg_t configReg, const uint8 *buffer);
typedef RtkApiRet (*ConfigHandlerEth)(cva_ConfigEth_t configEtg, const uint8 *buffer);
EXTERN_API RtkApiRet cva_drv_config_parse_flash_cfg_header(const int8 *fileName, uint32 spiStartAddr[], uint32 spiParaLen[], uint8 stkIdx);
EXTERN_API RtkApiRet cva_drv_config_parse_efuse_cfg_header(uint32 unit, uint8 efuStartAddr[], uint8 efuParaLen[]);
EXTERN_API RtkApiRet cva_drv_config_efuse_cfg_ext_search(uint32 unit, uint8 *ext_start_addr, uint8 *endofOTP);
EXTERN_API RtkApiRet cva_drv_config_eth_packet_assmeble(uint8 regCmd, uint32 regAddr, uint32 regVal, uint8 lastPacket);
EXTERN_API RtkApiRet cva_drv_config_eth_tx_packet_assmeble_rx_decode(uint8 regCmd, uint32 regAddr, uint32 *regVal, uint8 lastPacket);
EXTERN_API RtkApiRet cva_drv_config_check_enable_phy(uint32 *phyPortEnIdx);
EXTERN_API RtkApiRet cva_drv_config_check_eth_enable_phy(uint32 *phyPortEnIdx);
EXTERN_API RtkApiRet cva_drv_config_sys_patch(uint32 unit, const int8 *fileName, cva_UpdateConfig_t updateConfig);
EXTERN_API RtkApiRet cva_drv_config_eth_patch(uint32 unit, const int8 *fileName, cva_UpdateConfig_t updateConfig, uint8 mode);
EXTERN_API RtkApiRet cva_drv_config_get_hdr_fw_info(void *fp, uint32 *fwAddr, uint32 *fwLength, uint8 *fwKeyType);
EXTERN_API RtkApiRet cva_drv_config_eth_write_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 value);
EXTERN_API RtkApiRet cva_drv_config_polling_status(uint32 unit, cva_PollingReg_t pollingReg);

#endif /*_RTL906X_ASICDRV_CONFIG_H_*/
