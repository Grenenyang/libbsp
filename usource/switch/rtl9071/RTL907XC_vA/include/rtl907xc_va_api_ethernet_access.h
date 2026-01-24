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

#ifndef CVA_RTK_API_ETHERNET_ACCESS_H__
#define CVA_RTK_API_ETHERNET_ACCESS_H__

#include "rtl907xc_va_api.h"
#include "rtl907xc_va_table_struct.h"
#include "rtl907xc_va_asicdrv_flash.h"
#include "rtl907xc_va_asicdrv_config.h"
#include "rtl907xc_va_api_spi_flash.h"
#include "rtkas_api_ethernet_access_ext.h"

#define CVA_RTK_ETHERACCESS_IDENTITY                (0x19040000u)
#define CVA_RTK_ETHERACCESS_BYTE_PER_WRITE          (0x00000400u)
#define CVA_RTK_ETHERACCESS_READ_BACK               (0x00000001u)
#define CVA_RTK_ETHERACCESS_HEADER_LEN              (0x76u)
#define CVA_RTK_ETHERACCESS_MAC_READ_LEN            (1038u)
#define CVA_RTK_ETHERACCESS_MAC_WRITE_LEN           (1038u)
#define CVA_RTK_ETHERACCESS_ETH_FLASH_ERASE_ALL_LEN (1142u)
#define CVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN     (CVA_RTK_ETHERACCESS_HEADER_LEN + CVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
#define CVA_RTK_ETHERACCESS_FLASH_START_LEN         (1142u)
#define CVA_RTK_ETHERACCESS_MAX_OTP                 (800u)
#define CVA_RTK_ETHERACCESS_BURST_READ_WRITE_LEN    (CVA_RTK_ETHERACCESS_HEADER_LEN + (RTK_ETHERACCESS_MAX_BURST * 8u))
#define CVA_RTK_ETHERACCESS_TTL_POSITION            (8u)
#define CVA_MAX_PKT_LEN                             (1492u)
#define CVA_DISABLE_ACL_NUM                         (12u)
#define CVA_RESET_CHECK_NUM                         (2u)
#define CVA_RESET_WRITE_NUM                         (1u)
#define CVA_ETHERACCESS_MAX_PATCH_SIZE              (0x70000u)
#define CVA_ETHERACCESS_MAX_FLASH_BLOCK             (0x100u)
#define CVA_ETHERACCESS_MAX_FLASH_SECTOR            (0x1000u)
#define CVA_ETHERACCESS_MAX_FLASH_SIZE              (0x1000000u)

#define CVA_ETHERACCESS_IPHDR_SIZE  (20u)
#define CVA_ETHERACCESS_IPHDR_HL    (5u)
#define CVA_ETHERACCESS_IPHDR_VER   (4u)
#define CVA_ETHERACCESS_IPHDR_TOS   (0u)
#define CVA_ETHERACCESS_IPHDR_TTL   (0xFFu)
#define CVA_ETHERACCESS_IPHDR_PROTO (0xFFu)

#define CVA_ETHERACCESS_REGISTER_RW_MAC_READ   (1u)
#define CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE  (2u)
#define CVA_ETHERACCESS_REGISTER_RW_8BIT_READ  (3u)
#define CVA_ETHERACCESS_REGISTER_RW_8BIT_WRITE (4u)
#define CVA_ETHERACCESS_REGISTER_RW_SRAM_READ  (5u)
#define CVA_ETHERACCESS_REGISTER_RW_SRAM_WRITE (6u)
#define CVA_ETHERACCESS_REGISTER_RW_RESERVED1  (7u)
#define CVA_ETHERACCESS_PARTIAL_WRITE          (8u)
#define CVA_ETHERACCESS_PARTIAL_MASK_WRITE     (9u)
#define CVA_ETHERACCESS_REGISTER_END           CVA_ETHERACCESS_PARTIAL_MASK_WRITE

#define CVA_ETH_DISABLE_ACL_BASE (0x80070000u)
#define CVA_ETH_INCR             CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU
#define CVA_ETH_INCR_EXE_LSP     CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_EXECUTE_INTERNAL_CPU_LSP
#define CVA_ETH_INCR_EXE_LEN     CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_EXECUTE_INTERNAL_CPU_LEN
#define CVA_ETH_INCR_RW_LSP      CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCMD_INTERNAL_CPU_LSP
#define CVA_ETH_INCR_RW_LEN      CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCMD_INTERNAL_CPU_LEN
#define CVA_ETH_INCR_TYPE_LSP    CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCTBTP_INTERNAL_CPU_LSP
#define CVA_ETH_INCR_TYPE_LEN    CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCTBTP_INTERNAL_CPU_LEN
#define CVA_ETH_INCR_ADDR_LSP    CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCADDR_INTERNAL_CPU_LSP
#define CVA_ETH_INCR_ADDR_LEN    CVA_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCADDR_INTERNAL_CPU_LEN
#define CVA_ETH_INDR_BASE        CVA_INDIRECT_DATA0_FOR_INTERNAL_CPU

#define CVA_ETH_RESET_ENABLE      (0x001a3c70u)
#define CVA_ETH_RESET_ENABLE_BIT  (0x4u)
#define CVA_ETH_RESET_ENABLE_07   (CVA_ETH_RESET_ENABLE & 0xFFu)
#define CVA_ETH_RESET_ENABLE_815  ((CVA_ETH_RESET_ENABLE >> 8u) & 0xFFu)
#define CVA_ETH_RESET_ENABLE_1623 ((CVA_ETH_RESET_ENABLE >> 16u) & 0xFFu)
#define CVA_CPU_DMY_2_07          (CVA_CPU_DMY_2 & 0xFFu)
#define CVA_CPU_DMY_2_815         ((CVA_CPU_DMY_2 >> 8u) & 0xFFu)
#define CVA_CPU_DMY_2_1623        ((CVA_CPU_DMY_2 >> 16u) & 0xFFu)

#define CVA_COMMAND_NON        (0u)
#define CVA_FLASH_START        (1u)
#define CVA_FLASH_WRITE        (2u)
#define CVA_REP_WRITE          (3u)
#define CVA_REP_READ           (4u)
#define CVA_FLASH_READ         (5u)
#define CVA_REGISTER_RW_BURST  (6u)
#define CVA_PATCH_START        (7u)
#define CVA_PATCH_WRITE        (8u)
#define CVA_ETH_RESVERED1      (9u)
#define CVA_FLASH_ERASE_SECTOR (10u)
#define CVA_FLASH_ERASE_BLOCK  (11u)
#define CVA_FLASH_ERASE_ALL    (12u)
#define CVA_PAGE_SELECT        (13u)
#define CVA_RESET_SYSTEM       (14u)
#define CVA_ETH_RESVERED2      (15u)
#define CVA_ETH_RESVERED3      (16u)
#define CVA_ETH_RESVERED4      (17u)
#define CVA_ETH_RESVERED5      (18u)
#define CVA_STK_LOAD_FW        (19u)
#define CVA_STK_LOAD_CFG       (20u)
#define CVA_STK_REG_BURST      (21u)
#define CVA_STK_REPLY          (22u)
#define CVA_COMMAND_END        (23u)

#define CVA_ETH_OTP_CMD_NON (0xDu)
#define CVA_ETH_OTP_ASK     (0xEu)
#define CVA_ETH_OTP_JOIN    (0xFu)
#define CVA_ETH_OTP_CHECK   (0x10u)
#define CVA_ETH_OTP_WRITE   (0x11u)
#define CVA_ETH_OTP_REP     (0x12u)
#define CVA_ETH_OTP_CMD_END (0x13u)

#define CVA_CPU_STATUS_NON      (0u)
#define CVA_CPU_SUCCESS         (1u)
#define CVA_CPU_ONGOING         (2u)
#define CVA_CPU_FAIL            (3u)
#define CVA_CPU_FAIL_SECURE_MSG (4u)
#define CVA_CPU_READY           (5u)
#define CVA_CPU_FAIL_FLOW       (6u)
#define CVA_CPU_FAIL_CHECKSUM   (7u)
#define CVA_CPU_FAIL_SEQID      (8u)
#define CVA_CPU_FAIL_NOFLASH    (9u)
#define CVA_CPU_PAGE_LOCK       (10u)
#define CVA_CPU_FAIL_ADDR       (11u)
#define CVA_CPU_FAIL_POOLING    (12u)
#define CVA_CPU_FAIL_AUTH       (13u)
#define CVA_CPU_FAIL_CMD        (14u)
#define CVA_CPU_FAIL_SIZE       (15u)
#define CVA_CPU_FAIL_HDR        (16u)
#define CVA_CPU_FAIL_L2MEM      (17u)
#define CVA_CPU_STATUS_END      (18u)

EXTERN_API RtkApiRet cva_eth_acc_gen_burst_rw(const EtherAccessMac_t mac, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_check_burst_rw(uint32 number, uint32 *regValue, uint8 *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_flash_erase_all(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_check_flash_erase_all(const uint8 *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_flash_erase_sector(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_check_flash_erase_sector(const uint8 *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_flash_erase_block(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_check_flash_erase_block(const uint8 *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_flash_start(const EtherAccessMac_t mac, uint32 addr, uint32 size, uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_flash_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *flash, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_patch_start(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 size, uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_patch_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *patch, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_check_rep_write(const uint8 *packet, const uint32 seqId, uint32 *ackId, uint32 *checksum);
EXTERN_API RtkApiRet cva_eth_acc_gen_flash_read(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 len, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_check_rep_read(EtherAccessL2Header_t *packet, uint8 *data, uint32 len);
EXTERN_API RtkApiRet cva_eth_acc_gen_l3_otp_ask(const uint8 macAddr[MAC_ADDR_LEN], uint32 sip, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_l3_otp_check(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_gen_l3_otp_write(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const EtherAccessOtpData_t otpInfo, const uint8 *data, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_get_l3_otp_ttl(const uint8 *packet, uint8 *ttl);
EXTERN_API RtkApiRet cva_eth_acc_check_l3_otp_join(const uint8 *packet, uint32 *ackId, uint8 *uuid);
EXTERN_API RtkApiRet cva_eth_acc_check_l3_otp_rep(EtherAccessL2IpHdr_t *packet, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, uint32 *ackId, uint8 *data);
EXTERN_API RtkApiRet cva_eth_acc_gen_reset_system(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cva_eth_acc_check_reset_system(uint8 *packet);

EXTERN_API RtkApiRet cva_eth_acc_flow_table_entry_get(cva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data);
EXTERN_API RtkApiRet cva_eth_acc_flow_table_entry_set(const cva_table_list_t table, const uint32 startIndex, const uint32 entryNum, const uint32 *data);
EXTERN_API RtkApiRet cva_eth_acc_flow_flash_disable_protocol(void);
EXTERN_API RtkApiRet cva_eth_acc_flow_flash_all_erase(void);
EXTERN_API RtkApiRet cva_eth_acc_flow_flash_sec_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet cva_eth_acc_flow_flash_blk_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet cva_eth_acc_flow_flash_write(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet cva_eth_acc_flow_patch_write(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet cva_eth_acc_flow_flash_read(uint32 addr, uint32 len, uint8 *data);
EXTERN_API RtkApiRet cva_eth_acc_flow_reset_system(void);

#endif /* CVA_RTK_API_ETHERNET_ACCESS_H__ */
