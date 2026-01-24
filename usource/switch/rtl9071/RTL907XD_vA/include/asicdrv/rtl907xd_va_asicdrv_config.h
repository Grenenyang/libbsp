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

#ifndef DVA_ASICDRV_CONFIG_H_
#define DVA_ASICDRV_CONFIG_H_

#include "rtkas_common_types.h"

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Defines
 ******************************************************************************/
#define DVA_CFG_SPI_IDENTIFY           (0x8899u)
#define DVA_CONFIG_SGMII_ENTRY_LEN     (5u)
#define DVA_CONFIG_MAC_ENTRY_LEN       (7u)
#define DVA_CONFIG_OCP_ENTRY_LEN       (4u)
#define DVA_CONFIG_FWCFG_BYTE_LEN      (3u)
#define DVA_CONFIG_TABLE_HEADER_LEN    (4u)
#define DVA_CONFIG_ROT_MTBL_LEN        (56u)
#define DVA_CONFIG_ROT_UTBL_LEN        (44u)
#define DVA_CONFIG_ROT_NET_LEN         (84u)
#define DVA_CFG_REG_FIELD_WR_ENTRY_LEN (9u)

#define DVA_EFU_IDENTIFY             (0x80u)
#define DVA_OTP_CFG_START_ADDR       (0x00u)
#define DVA_OTP_INTERNAL_CONFIG_ADDR (0x00C0u)
#define DVA_OTP_EXT_CFG_END          (DVA_OTP_INTERNAL_CONFIG_ADDR - 0x10u) /*for 6665 PHY extend*/

#define DVA_MAX_FW_LENGTH (0xC0000u)

#define DVA_OTP_FLASHLESS_ENABLE_PORT (DVA_OTP_INTERNAL_CONFIG_ADDR + 0x0Cu)

#define DVA_AESHASH_SHA256_LEN            (32u)
#define DVA_FLASH_HEADER_OFFSET           (0x00000020u)
#define DVA_FLASH_CONFIG_HEADER_LEN       (256u)
#define DVA_FLASH_DOMAIN_LEN              (3u)
#define DVA_CONFIG_BLOCK_ALIGNMENT_LENGTH (16u)
#define DVA_CONFIG_BLOCK_MAX_LENGTH       (1024u)
#define DVA_CONFIG_BLOCK_HASH_LENGTH      (DVA_AESHASH_SHA256_LEN)
#define DVA_MAXIMUM_BLOCK_NUMBER          (1024u)

#define DVA_NUM_OF_PORT_T1PHY (10u)

#define DVA_PAGE_SWR_OCP (0x4A000000u)
#define DVA_PAGE_SYS_MEM (0x01u)
#define DVA_PAGE_SW      (0x4Au)

#define DVA_FIRMWARE_STAGE             (0x1u)
#define DVA_CONFIG_STAGE               (0x2u)
#define DVA_FLASHLESS_DONE_STAGE       (0x3u)
#define DVA_FLASHLESS_DONE_NO_FW_STAGE (0x4u)
#define DVA_FLASHLESS_DONE_JUMP_SRAM   (0x5u)
#define DVA_FLASHLESS_FAIL_STAGE       (0x6u)

#define DVA_EXIT_CMD_LSP   (0u)
#define DVA_EXIT_CMD_LEN   (8u)
#define DVA_STAGE_LSP      (8u)
#define DVA_STAGE_LEN      (3u)
#define DVA_TRIGGER_LSP    (8u)
#define DVA_STATUS_LSP     (13u)
#define DVA_STATUS_LEN     (3u)
#define DVA_CFG_LENGTH_LSP (9u)
#define DVA_CFG_LENGTH_LEN (20u)

/******************************************************************************
 * Typedef Structures
 ******************************************************************************/
typedef struct
{
    uint32  address;
    uint32  lsp;
    uint32  len;
    uint32 *value;
    uint32  num;
    uint32  timeoutValue;
    uint8   isEth;
} dva_PollingReg_t;

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
EXTERN_API RtkApiRet dva_drv_config_parse_flash_cfg_header(const int8 *fileName, uint32 spiStartAddr[], uint32 spiParaLen[], uint8 stkIdx);
EXTERN_API RtkApiRet dva_drv_config_parse_efuse_cfg_header(uint32 unit, uint8 efuStartAddr[], uint8 efuParaLen[]);
EXTERN_API RtkApiRet dva_drv_config_efuse_cfg_ext_search(uint32 unit, uint8 *ext_start_addr, uint8 *endofOTP);
EXTERN_API RtkApiRet dva_drv_config_eth_packet_assmeble(uint8 regCmd, uint32 regAddr, uint32 regVal, uint8 lastPacket);
EXTERN_API RtkApiRet dva_drv_config_eth_tx_packet_assmeble_rx_decode(uint8 regCmd, uint32 regAddr, uint32 *regVal, uint8 lastPacket);
EXTERN_API RtkApiRet dva_drv_config_get_hdr_fw_info(void *fp, uint32 *fwAddr, uint32 *fwLength, uint8 *fwKeyType);
EXTERN_API RtkApiRet dva_drv_config_eth_read_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 *value);
EXTERN_API RtkApiRet dva_drv_config_eth_write_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 value);
EXTERN_API RtkApiRet dva_drv_config_update_aes_ctr_iv(uint32 unit, void *fp, const uint32 fwKeyType, const uint32 fwLength);
EXTERN_API RtkApiRet dva_drv_config_eth_update_aes_ctr_iv(void *fp, const uint32 fwKeyType, const uint32 fwLength);
EXTERN_API RtkApiRet dva_drv_config_load_config(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet dva_drv_config_load_config_eth(uint32 unit, const int8 *fileName);
EXTERN_API RtkApiRet dva_drv_config_polling_status(uint32 unit, dva_PollingReg_t pollingReg);

#endif /*_RTL906X_ASICDRV_CONFIG_H_*/
