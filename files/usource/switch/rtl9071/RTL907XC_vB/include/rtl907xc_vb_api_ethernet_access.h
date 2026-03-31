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

#ifndef CVB_RTK_API_ETHERNET_ACCESS_H__
#define CVB_RTK_API_ETHERNET_ACCESS_H__

#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtl907xc_vb_asicdrv_flash.h"
#include "rtl907xc_vb_asicdrv_config.h"
#include "rtl907xc_vb_api_spi_flash.h"
#include "rtkas_api_ethernet_access_ext.h"

#define CVB_RTK_ETHERACCESS_IDENTITY                (0x19040000u)
#define CVB_RTK_LOAD_CFG_HEADER                     (4u)
#define CVB_RTK_ETHERACCESS_BYTE_PER_WRITE          (0x00000400u)
#define CVB_RTK_ETHERACCESS_READ_BACK               (0x00000001u)
#define CVB_RTK_ETHERACCESS_HEADER_LEN              (0x76u)
#define CVB_RTK_ETHERACCESS_MAC_READ_LEN            (1038u)
#define CVB_RTK_ETHERACCESS_MAC_WRITE_LEN           (1038u)
#define CVB_RTK_ETHERACCESS_ETH_FLASH_ERASE_ALL_LEN (1142u)
#define CVB_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN     (CVB_RTK_ETHERACCESS_HEADER_LEN + CVB_RTK_ETHERACCESS_BYTE_PER_WRITE)
#define CVB_RTK_ETHERACCESS_FLASH_START_LEN         (1142u)
#define CVB_RTK_ETHERACCESS_LOAD_CFG_LEN            (CVB_RTK_ETHERACCESS_HEADER_LEN + CVB_RTK_LOAD_CFG_HEADER + CVB_RTK_ETHERACCESS_BYTE_PER_WRITE)
#define CVB_RTK_ETHERACCESS_MAX_OTP                 (800u)
#define CVB_RTK_ETHERACCESS_BURST_READ_WRITE_LEN    (CVB_RTK_ETHERACCESS_HEADER_LEN + (RTK_ETHERACCESS_MAX_BURST * 8u))
#define CVB_RTK_ETHERACCESS_TTL_POSITION            (8u)
#define CVB_MAX_PKT_LEN                             (1492u)
#define CVB_DISABLE_ACL_NUM                         (11u)
#define CVB_DISABLE_IGMP_NUM                        (2u)
#define CVB_DISABLE_IGMP_VAL                        (0x3000u)
#define CVB_RESET_CHECK_NUM                         (2u)
#define CVB_RESET_WRITE_NUM                         (1u)
#define CVB_ETHERACCESS_MAX_PATCH_SIZE              (0x70000u)
#define CVB_ETHERACCESS_MAX_FLASH_BLOCK             (0x100u)
#define CVB_ETHERACCESS_MAX_FLASH_SECTOR            (0x1000u)
#define CVB_ETHERACCESS_MAX_FLASH_SIZE              (0x1000000u)

#define CVB_ETHERACCESS_IPHDR_SIZE  (20u)
#define CVB_ETHERACCESS_IPHDR_HL    (5u)
#define CVB_ETHERACCESS_IPHDR_VER   (4u)
#define CVB_ETHERACCESS_IPHDR_TOS   (0u)
#define CVB_ETHERACCESS_IPHDR_TTL   (0xFFu)
#define CVB_ETHERACCESS_IPHDR_PROTO (0xFFu)

#define CVB_ETHERACCESS_REGISTER_RW_MAC_READ   (1u)
#define CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE  (2u)
#define CVB_ETHERACCESS_REGISTER_RW_8BIT_READ  (3u)
#define CVB_ETHERACCESS_REGISTER_RW_8BIT_WRITE (4u)
#define CVB_ETHERACCESS_REGISTER_RW_SRAM_READ  (5u)
#define CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE (6u)
#define CVB_ETHERACCESS_REGISTER_RW_RESERVED1  (7u)
#define CVB_ETHERACCESS_PARTIAL_WRITE          (8u)
#define CVB_ETHERACCESS_PARTIAL_MASK_WRITE     (9u)
#define CVB_ETHERACCESS_REGISTER_END           CVB_ETHERACCESS_PARTIAL_MASK_WRITE

#define CVB_ETH_DISABLE_ACL_BASE (0x80070000u)
#define CVB_ETH_INCR             CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU
#define CVB_ETH_INCR_EXE_LSP     CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_EXECUTE_INTERNAL_CPU_LSP
#define CVB_ETH_INCR_EXE_LEN     CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_EXECUTE_INTERNAL_CPU_LEN
#define CVB_ETH_INCR_RW_LSP      CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCMD_INTERNAL_CPU_LSP
#define CVB_ETH_INCR_RW_LEN      CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCMD_INTERNAL_CPU_LEN
#define CVB_ETH_INCR_TYPE_LSP    CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCTBTP_INTERNAL_CPU_LSP
#define CVB_ETH_INCR_TYPE_LEN    CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCTBTP_INTERNAL_CPU_LEN
#define CVB_ETH_INCR_ADDR_LSP    CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCADDR_INTERNAL_CPU_LSP
#define CVB_ETH_INCR_ADDR_LEN    CVB_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCADDR_INTERNAL_CPU_LEN
#define CVB_ETH_INDR_BASE        CVB_INDIRECT_DATA0_FOR_INTERNAL_CPU

#define CVB_ETH_RESET_ENABLE      (0x001a3c70u)
#define CVB_ETH_RESET_ENABLE_BIT  (0x4u)
#define CVB_ETH_RESET_ENABLE_07   (CVB_ETH_RESET_ENABLE & 0xFFu)
#define CVB_ETH_RESET_ENABLE_815  ((CVB_ETH_RESET_ENABLE >> 8u) & 0xFFu)
#define CVB_ETH_RESET_ENABLE_1623 ((CVB_ETH_RESET_ENABLE >> 16u) & 0xFFu)
#define CVB_CPU_DMY_2_07          (CVB_CPU_DMY_2 & 0xFFu)
#define CVB_CPU_DMY_2_815         ((CVB_CPU_DMY_2 >> 8u) & 0xFFu)
#define CVB_CPU_DMY_2_1623        ((CVB_CPU_DMY_2 >> 16u) & 0xFFu)

#define CVB_COMMAND_NON            (0u)
#define CVB_FLASH_START            (1u)
#define CVB_FLASH_WRITE            (2u)
#define CVB_REP_WRITE              (3u)
#define CVB_REP_READ               (4u)
#define CVB_FLASH_READ             (5u)
#define CVB_REGISTER_RW_BURST      (6u)
#define CVB_PATCH_START            (7u)
#define CVB_PATCH_WRITE            (8u)
#define CVB_ETH_RESVERED1          (9u)
#define CVB_FLASH_ERASE_SECTOR     (10u)
#define CVB_FLASH_ERASE_BLOCK      (11u)
#define CVB_FLASH_ERASE_ALL        (12u)
#define CVB_PAGE_SELECT            (13u)
#define CVB_RESET_SYSTEM           (14u)
#define CVB_ETH_RESVERED2          (15u)
#define CVB_ETH_RESVERED3          (16u)
#define CVB_ETH_RESVERED4          (17u)
#define CVB_ETH_RESVERED5          (18u)
#define CVB_STK_LOAD_FW            (19u)
#define CVB_STK_LOAD_CFG           (20u)
#define CVB_STK_REG_BURST          (21u)
#define CVB_STK_REPLY              (22u)
#define CVB_OTP_L2_ASK             (23u)
#define CVB_OTP_L2_JOIN            (24u)
#define CVB_OTP_L2_CHECK           (25u)
#define CVB_OTP_L2_WRITE           (26u)
#define CVB_OTP_L2_REP             (27u)
#define CVB_PAGE_READ              (28u)
#define CVB_FLASHLESS_CFG          (29u)
#define CVB_FLASH_ERASE_CHECK      (30u)
#define CVB_FLASH_REQ_ERASE_SECTOR (31u)
#define CVB_FLASH_REQ_ERASE_BLOCK  (32u)
#define CVB_FLASH_REQ_ERASE_ALL    (33u)
#define CVB_ETH_CHECK_ID           (34u)
#define CVB_ETH_REP_CHECK_ID       (35u)
#define CVB_COMMAND_END            (36u)

#define CVB_ETH_OTP_CMD_NON (0xDu)
#define CVB_ETH_OTP_ASK     (0xEu)
#define CVB_ETH_OTP_JOIN    (0xFu)
#define CVB_ETH_OTP_CHECK   (0x10u)
#define CVB_ETH_OTP_WRITE   (0x11u)
#define CVB_ETH_OTP_REP     (0x12u)
#define CVB_ETH_OTP_CMD_END (0x13u)

#define CVB_CPU_STATUS_NON      (0u)
#define CVB_CPU_SUCCESS         (1u)
#define CVB_CPU_ONGOING         (2u)
#define CVB_CPU_FAIL            (3u)
#define CVB_CPU_FAIL_SECURE_MSG (4u)
#define CVB_CPU_READY           (5u)
#define CVB_CPU_FAIL_FLOW       (6u)
#define CVB_CPU_FAIL_CHECKSUM   (7u)
#define CVB_CPU_FAIL_SEQID      (8u)
#define CVB_CPU_FAIL_NOFLASH    (9u)
#define CVB_CPU_PAGE_LOCK       (10u)
#define CVB_CPU_FAIL_ADDR       (11u)
#define CVB_CPU_FAIL_POOLING    (12u)
#define CVB_CPU_FAIL_AUTH       (13u)
#define CVB_CPU_FAIL_CMD        (14u)
#define CVB_CPU_FAIL_SIZE       (15u)
#define CVB_CPU_FAIL_HDR        (16u)
#define CVB_CPU_FAIL_L2MEM      (17u)
#define CVB_CPU_FAIL_CFG        (18u)
#define CVB_CPU_STATUS_END      (19u)

EXTERN_API RtkApiRet cvb_eth_acc_gen_burst_rw(const EtherAccessMac_t mac, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_burst_rw(uint32 number, uint32 *regValue, uint8 *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_erase_all(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_flash_erase_all(const uint8 *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_erase_sector(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_flash_erase_sector(const uint8 *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_erase_block(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_flash_erase_block(const uint8 *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_erase_req(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 eraseCmd, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_flash_erase_req(const uint8 *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_status_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_flash_status_check(const uint8 *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_start(const EtherAccessMac_t mac, uint32 addr, uint32 size, uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *flash, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_patch_start(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 size, uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_patch_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *patch, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_rep_write(const uint8 *packet, const uint32 seqId, uint32 *ackId, uint32 *checksum);
EXTERN_API RtkApiRet cvb_eth_acc_gen_flash_read(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 len, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_rep_read(const uint8 *packet, uint8 *data, uint32 len);
EXTERN_API RtkApiRet cvb_eth_acc_gen_l3_otp_ask(const uint8 macAddr[MAC_ADDR_LEN], uint32 sip, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_l3_otp_check(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_l3_otp_write(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const EtherAccessOtpData_t otpInfo, const uint8 *data, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_get_l3_otp_ttl(const uint8 *packet, uint8 *ttl);
EXTERN_API RtkApiRet cvb_eth_acc_check_l3_otp_join(const uint8 *packet, uint32 *ackId, uint8 *uuid);
EXTERN_API RtkApiRet cvb_eth_acc_check_l3_otp_rep(EtherAccessL2IpHdr_t *packet, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, uint32 *ackId, uint8 *data);
EXTERN_API RtkApiRet cvb_eth_acc_gen_reset_system(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_reset_system(uint8 *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_l2_otp_ask(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], RtkEtherAccessL2OtpHdr_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_l2_otp_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, RtkEtherAccessL2OtpHdr_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_gen_l2_otp_write(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const EtherAccessOtpData_t otpInfo, const uint8 *data, RtkEtherAccessL2OtpHdr_t *packet);
EXTERN_API RtkApiRet cvb_eth_acc_check_l2_otp_join(const uint8 *packet, uint32 *ackId, uint8 *uuid);
EXTERN_API RtkApiRet cvb_eth_acc_check_l2_otp_rep(RtkEtherAccessL2OtpHdr_t *packet, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, uint32 *ackId, uint8 *data);

EXTERN_API RtkApiRet cvb_eth_acc_flow_table_entry_get(cvb_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data);
EXTERN_API RtkApiRet cvb_eth_acc_flow_table_entry_set(const cvb_table_list_t table, const uint32 startIndex, const uint32 entryNum, const uint32 *data);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_disable_protocol(void);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_all_erase(void);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_sec_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_blk_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_req_sec_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_req_blk_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_req_all_erase(void);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_write(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet cvb_eth_acc_flow_patch_write(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet cvb_eth_acc_flow_flash_read(uint32 addr, uint32 len, uint8 *data);
EXTERN_API RtkApiRet cvb_eth_acc_flow_reset_system(void);

#endif /* CVB_RTK_API_ETHERNET_ACCESS_H__ */
