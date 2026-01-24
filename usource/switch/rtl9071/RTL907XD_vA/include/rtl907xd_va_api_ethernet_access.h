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

#ifndef DVA_RTK_API_ETHERNET_ACCESS_H__
#define DVA_RTK_API_ETHERNET_ACCESS_H__

#include "rtl907xd_va_api.h"
#include "rtl907xd_va_table_struct.h"
#include "rtl907xd_va_asicdrv_flash.h"
#include "rtl907xd_va_asicdrv_config.h"
#include "rtl907xd_va_api_spi_flash.h"
#include "rtkas_api_ethernet_access_ext.h"

#define DVA_RTK_ETHERACCESS_IDENTITY                (0x19040000u)
#define DVA_RTK_LOAD_CFG_HEADER                     (4u)
#define DVA_RTK_ETHERACCESS_BYTE_PER_WRITE          (0x00000400u)
#define DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE      (0x00000100u)
#define DVA_RTK_ETHERACCESS_READ_BACK               (0x00000001u)
#define DVA_RTK_ETHERACCESS_HEADER_LEN              (0x76u)
#define DVA_RTK_ETHERACCESS_MAC_READ_LEN            (1038u)
#define DVA_RTK_ETHERACCESS_MAC_WRITE_LEN           (1038u)
#define DVA_RTK_ETHERACCESS_ETH_FLASH_ERASE_ALL_LEN (1142u)
#define DVA_RTK_ETHERACCESS_ETH_FLASH_WRITE_LEN     (DVA_RTK_ETHERACCESS_HEADER_LEN + DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
#define DVA_RTK_ETHERACCESS_ETH_FLASH_REQ_WRITE_LEN (DVA_RTK_ETHERACCESS_HEADER_LEN + DVA_RTK_ETHERACCESS_REQ_BYTE_PER_WRITE)
#define DVA_RTK_ETHERACCESS_FLASH_START_LEN         (1142u)
#define DVA_RTK_ETHERACCESS_LOAD_CFG_LEN            (DVA_RTK_ETHERACCESS_HEADER_LEN + DVA_RTK_LOAD_CFG_HEADER + DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
#define DVA_RTK_ETHERACCESS_MAX_OTP                 (1160u)
#define DVA_RTK_ETHERACCESS_BURST_READ_WRITE_LEN    (DVA_RTK_ETHERACCESS_HEADER_LEN + (RTK_ETHERACCESS_MAX_BURST * 8u))
#define DVA_RTK_ETHERACCESS_TTL_POSITION            (8u)
#define DVA_MAX_PKT_LEN                             (1492u)
#define DVA_DISABLE_ACL_NUM                         (16u)
#define DVA_RESET_CHECK_NUM                         (2u)
#define DVA_RESET_WRITE_NUM                         (1u)
#define DVA_ETHERACCESS_MAX_PATCH_SIZE              (0xB0000u)
#define DVA_ETHERACCESS_MAX_FLASH_BLOCK             (0x80u)
#define DVA_ETHERACCESS_MAX_FLASH_SECTOR            (0x800u)
#define DVA_ETHERACCESS_MAX_FLASH_SIZE              (0x800000u)

#define DVA_ETHERACCESS_IPHDR_SIZE  (20u)
#define DVA_ETHERACCESS_IPHDR_HL    (5u)
#define DVA_ETHERACCESS_IPHDR_VER   (4u)
#define DVA_ETHERACCESS_IPHDR_TOS   (0u)
#define DVA_ETHERACCESS_IPHDR_TTL   (0xFFu)
#define DVA_ETHERACCESS_IPHDR_PROTO (0xFFu)

#define DVA_ETHERACCESS_REGISTER_RW_MAC_READ   (1u)
#define DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE  (2u)
#define DVA_ETHERACCESS_REGISTER_RW_8BIT_READ  (3u)
#define DVA_ETHERACCESS_REGISTER_RW_8BIT_WRITE (4u)
#define DVA_ETHERACCESS_REGISTER_RW_SRAM_READ  (5u)
#define DVA_ETHERACCESS_REGISTER_RW_SRAM_WRITE (6u)
#define DVA_ETHERACCESS_REGISTER_RW_RESERVED1  (7u)
#define DVA_ETHERACCESS_PARTIAL_WRITE          (8u)
#define DVA_ETHERACCESS_PARTIAL_MASK_WRITE     (9u)
#define DVA_ETHERACCESS_REGISTER_END           DVA_ETHERACCESS_PARTIAL_MASK_WRITE

#define DVA_ETH_DISABLE_ACL_BASE (0x80070000u)
#define DVA_ETH_INCR             DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU
#define DVA_ETH_INCR_EXE_LSP     DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_EXECUTE_INTERNAL_CPU_LSP
#define DVA_ETH_INCR_EXE_LEN     DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_EXECUTE_INTERNAL_CPU_LEN
#define DVA_ETH_INCR_RW_LSP      DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCMD_INTERNAL_CPU_LSP
#define DVA_ETH_INCR_RW_LEN      DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCMD_INTERNAL_CPU_LEN
#define DVA_ETH_INCR_TYPE_LSP    DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCTBTP_INTERNAL_CPU_LSP
#define DVA_ETH_INCR_TYPE_LEN    DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCTBTP_INTERNAL_CPU_LEN
#define DVA_ETH_INCR_ADDR_LSP    DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCADDR_INTERNAL_CPU_LSP
#define DVA_ETH_INCR_ADDR_LEN    DVA_ALE_INDIRECT_CONTROL_FOR_INTERNAL_CPU_ACCADDR_INTERNAL_CPU_LEN
#define DVA_ETH_INDR_BASE        DVA_ALE_INDIRECT_DATA0_FOR_INTERNAL_CPU

#define DVA_ETH_RESET_ENABLE              (0x001a3c70u)
#define DVA_ETH_RESET_ENABLE_BIT          (0x4u)
#define DVA_ETH_RESET_ENABLE_07           (DVA_ETH_RESET_ENABLE & 0xFFu)
#define DVA_ETH_RESET_ENABLE_815          ((DVA_ETH_RESET_ENABLE >> 8u) & 0xFFu)
#define DVA_ETH_RESET_ENABLE_1623         ((DVA_ETH_RESET_ENABLE >> 16u) & 0xFFu)
#define DVA_CPU_DMY_2_07                  (DVA_CPU_DMY_2 & 0xFFu)
#define DVA_CPU_DMY_2_815                 ((DVA_CPU_DMY_2 >> 8u) & 0xFFu)
#define DVA_CPU_DMY_2_1623                ((DVA_CPU_DMY_2 >> 16u) & 0xFFu)
#define DVA_CPU_FWLEN_07                  (DVA_INTERNAL_CPU_VARIABLE19 & 0xFFu)
#define DVA_CPU_FWLEN_815                 ((DVA_INTERNAL_CPU_VARIABLE19 >> 8u) & 0xFFu)
#define DVA_CPU_FWLEN_1623                ((DVA_INTERNAL_CPU_VARIABLE19 >> 16u) & 0xFFu)
#define DVA_ADDR_SHARE_MEMORY_END         (0x010F0000u)
#define DVA_ADDR_SYS_MEMORY_BASE          (0x01000000u)
#define DVA_INDIRECT_ACCESS_HEADER_LENGTH (8u)

#define DVA_COMMAND_NON              (0u)
#define DVA_FLASH_START              (1u)
#define DVA_FLASH_WRITE              (2u)
#define DVA_REP_WRITE                (3u)
#define DVA_REP_READ                 (4u)
#define DVA_FLASH_READ               (5u)
#define DVA_REGISTER_RW_BURST        (6u)
#define DVA_PATCH_START              (7u)
#define DVA_PATCH_WRITE              (8u)
#define DVA_ETH_RESVERED1            (9u)
#define DVA_FLASH_ERASE_SECTOR       (10u)
#define DVA_FLASH_ERASE_BLOCK        (11u)
#define DVA_FLASH_ERASE_ALL          (12u)
#define DVA_PAGE_SELECT              (13u)
#define DVA_RESET_SYSTEM             (14u)
#define DVA_ETH_RESVERED2            (15u)
#define DVA_ETH_RESVERED3            (16u)
#define DVA_ETH_RESVERED4            (17u)
#define DVA_ETH_RESVERED5            (18u)
#define DVA_STK_LOAD_FW              (19u)
#define DVA_STK_LOAD_CFG             (20u)
#define DVA_STK_REG_BURST            (21u)
#define DVA_STK_REPLY                (22u)
#define DVA_OTP_L2_ASK               (23u)
#define DVA_OTP_L2_JOIN              (24u)
#define DVA_OTP_L2_CHECK             (25u)
#define DVA_OTP_L2_WRITE             (26u)
#define DVA_OTP_L2_REP               (27u)
#define DVA_PAGE_READ                (28u)
#define DVA_FLASHLESS_CFG            (29u)
#define DVA_FLASH_STATUS_CHECK       (30u)
#define DVA_FLASH_REQ_ERASE_SECTOR   (31u)
#define DVA_FLASH_REQ_ERASE_BLOCK    (32u)
#define DVA_FLASH_REQ_ERASE_ALL      (33u)
#define DVA_ETH_RESVERED6            (34u)
#define DVA_ETH_RESVERED7            (35u)
#define DVA_FLASH_REQ_WRITE          (36u)
#define DVA_FLASH_EVENT_READ         (37u)
#define DVA_FLASH_CHECK_READ         (38u)
#define DVA_FLASH_EVENT_ERASE_SECTOR (39u)
#define DVA_FLASH_EVENT_ERASE_BLOCK  (40u)
#define DVA_FLASH_EVENT_ERASE_ALL    (41u)
#define DVA_FLASH_EVENT_WRITE        (42u)
#define DVA_FLASH_EVENT_CHECK        (43u)
#define DVA_FLASH_EVENT_CLEAR        (44u)
#define DVA_FLASH_EVENT_REPLY        (45u)
#define DVA_FLASHLESS_ALERT          (46u)
#define DVA_OTP_L2_ADAPT_CHECK       (47u)
#define DVA_OTP_L2_ADAPT_WRITE       (48u)
#define DVA_OTP_L2_ADAPT_REP         (49u)
#define DVA_COMMAND_END              (50u)

#define DVA_ETH_OTP_CMD_NON (0xDu)
#define DVA_ETH_OTP_ASK     (0xEu)
#define DVA_ETH_OTP_JOIN    (0xFu)
#define DVA_ETH_OTP_CHECK   (0x10u)
#define DVA_ETH_OTP_WRITE   (0x11u)
#define DVA_ETH_OTP_REP     (0x12u)
#define DVA_ETH_OTP_CMD_END (0x13u)

#define DVA_CPU_STATUS_NON      (0u)
#define DVA_CPU_SUCCESS         (1u)
#define DVA_CPU_ONGOING         (2u)
#define DVA_CPU_FAIL            (3u)
#define DVA_CPU_FAIL_SECURE_MSG (4u)
#define DVA_CPU_READY           (5u)
#define DVA_CPU_FAIL_FLOW       (6u)
#define DVA_CPU_FAIL_CHECKSUM   (7u)
#define DVA_CPU_FAIL_SEQID      (8u)
#define DVA_CPU_FAIL_NOFLASH    (9u)
#define DVA_CPU_PAGE_LOCK       (10u)
#define DVA_CPU_FAIL_ADDR       (11u)
#define DVA_CPU_FAIL_POOLING    (12u)
#define DVA_CPU_FAIL_AUTH       (13u)
#define DVA_CPU_FAIL_CMD        (14u)
#define DVA_CPU_FAIL_SIZE       (15u)
#define DVA_CPU_FAIL_HDR        (16u)
#define DVA_CPU_FAIL_L2MEM      (17u)
#define DVA_CPU_FAIL_CFG        (18u)
#define DVA_CPU_FAIL_ENCRYPT    (19u)
#define DVA_CPU_FAIL_STATE      (20u)
#define DVA_CPU_STATUS_END      (21u)

#define DVA_SPI_STATE_IDLE        (0)
#define DVA_SPI_STATE_READ        (1)
#define DVA_SPI_STATE_WRITEING    (2)
#define DVA_SPI_STATE_WRITE       (3)
#define DVA_SPI_STATE_ERASING     (4)
#define DVA_SPI_STATE_ERASE       (5)
#define DVA_SPI_STATE_WAIT_FINISH (6)
#define DVA_SPI_STATE_ERROR       (7)
#define DVA_SPI_STATE_END         (8)

EXTERN_API RtkApiRet dva_eth_acc_gen_burst_rw(const EtherAccessMac_t mac, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue, uint32 portMask, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_burst_rw(uint32 number, uint32 *regValue, uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_erase_all(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_erase_all(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_erase_sector(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_erase_sector(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_erase_block(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 num, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_erase_block(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_start(const EtherAccessMac_t mac, uint32 addr, uint32 size, uint32 perLen, uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *flash, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_patch_start(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 size, uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_patch_write(const EtherAccessMac_t mac, const uint32 addr, const uint8 *patch, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_rep_write(const uint8 *packet, const uint32 seqId, uint32 *ackId, uint32 *checksum);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_read(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 len, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_rep_read(const uint8 *packet, uint8 *data, uint32 len);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_erase_req(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 eraseCmd, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_erase_req(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_write_req(const EtherAccessMac_t mac, const uint32 addr, const uint8 *flash, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_status_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_status_check(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_erase_event(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 eraseCmd, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_erase_event(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_write_event(const EtherAccessMac_t mac, const uint32 addr, const uint8 *flash, const uint32 len, const uint32 seqId, const uint32 checksum, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_event(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_event(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_read_event(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 addr, uint32 len, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_rep_read_event(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_read_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], uint32 offset, uint32 len, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_read_check(const uint8 *packet, uint32 offset, uint32 len, uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_gen_flash_clear_event(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_flash_clear_event(const uint8 *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_reset_system(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_reset_system(uint8 *packet);

EXTERN_API RtkApiRet dva_eth_acc_gen_l3_otp_ask(const uint8 macAddr[MAC_ADDR_LEN], uint32 sip, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_l3_otp_check(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_l3_otp_write(const uint8 macAddr[MAC_ADDR_LEN], const uint32 sip, const uint8 *uuid, const EtherAccessOtpData_t otpInfo, const uint8 *data, EtherAccessL2IpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_get_l3_otp_ttl(const uint8 *packet, uint8 *ttl);
EXTERN_API RtkApiRet dva_eth_acc_check_l3_otp_join(const uint8 *packet, uint32 *ackId, uint8 *uuid);
EXTERN_API RtkApiRet dva_eth_acc_check_l3_otp_rep(EtherAccessL2IpHdr_t *packet, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, uint32 *ackId, uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_gen_l2_otp_ask(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], RtkEtherAccessL2OtpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_l2_otp_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, RtkEtherAccessL2OtpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_l2_otp_write(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const EtherAccessOtpData_t otpInfo, const uint8 *data, RtkEtherAccessL2OtpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_l2_otp_join(const uint8 *packet, uint32 *ackId, uint8 *uuid);
EXTERN_API RtkApiRet dva_eth_acc_check_l2_otp_rep(RtkEtherAccessL2OtpHdr_t *packet, const uint8 *uuid, const uint8 type, const uint8 typeId, const uint32 len, uint32 *ackId, uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_gen_l2_adapt_otp_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const uint16 addr, const uint16 len, RtkEtherAccessL2AdaptOtpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_gen_l2_adapt_otp_write(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint8 *uuid, const EtherAccessAdaptOtpData_t otpInfo, const uint8 *data, RtkEtherAccessL2AdaptOtpHdr_t *packet);
EXTERN_API RtkApiRet dva_eth_acc_check_l2_adapt_otp_rep(RtkEtherAccessL2AdaptOtpHdr_t *packet, const uint8 *uuid, const uint32 len, uint32 *ackId, uint8 *data);

EXTERN_API RtkApiRet dva_eth_acc_flow_table_entry_get(dva_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_table_entry_set(const dva_table_list_t table, const uint32 startIndex, const uint32 entryNum, const uint32 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_disable_protocol(void);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_all_erase(void);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_sec_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_blk_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_write(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_patch_write(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_read(uint32 addr, uint32 len, uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_req_all_erase(void);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_req_sec_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_req_blk_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_write_req(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_event_all_erase(void);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_event_sec_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_event_blk_erase(uint32 addr, uint32 num);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_event_write(uint32 addr, uint32 len, const uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_read_event(uint32 addr, uint32 len, uint8 *data);
EXTERN_API RtkApiRet dva_eth_acc_flow_flash_clear_event(void);
EXTERN_API RtkApiRet dva_eth_acc_flow_reset_system(void);

#endif /* DVA_RTK_API_ETHERNET_ACCESS_H__ */
