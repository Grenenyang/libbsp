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

#ifndef RTKAS_API_ETHERNET_ACCESS_EXT_H__
#define RTKAS_API_ETHERNET_ACCESS_EXT_H__

#include "rtkas_common.h"
#define RTK_ETHERACCESS_UUID_LEN  (10u)
#define RTK_ETHERTYPE_LEN         (2u)
#define RTK_REG_ADDR_LEN          (3u)
#define RTK_ETHERACCESS_MAX_BURST (158u)
#define RTK_ETHERACCESS_MAX_FLASH (1248u)

typedef struct
{
    /*01*/ uint32 RTL9040_Identity; /**< Realtek Id (swap of 0x19040000) */
    /*02*/ uint32 command_id;       /**< Command Id (swap of 1 to DVA_FLASH_EVENT_CLEAR) */
    /*03*/ uint32 ack_id;           /**< Ack Id (swap of 0 to DVA_CPU_FAIL_STATE) */
    /*04*/ uint32 hash_code0;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*05*/ uint32 hash_code1;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*06*/ uint32 hash_code2;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*07*/ uint32 hash_code3;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*08*/ uint32 hash_code4;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*09*/ uint32 hash_code5;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*10*/ uint32 hash_code6;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*11*/ uint32 hash_code7;       /**< Hash value (0 to 0xFFFFFFFF) */

    //------------Secure encode------------
    /*12*/ uint32 rand_id;             /**< Random Id (0 to 0xFFFFFFFF) */
    /*13*/ uint32 flash_update_addr;   /**< Flash write start address (0 to 0xFFFFFFFF) */
    /*14*/ uint32 total_write_size;    /**< Flash total write size (0 to 0xFFFFFFFF) */
    /*15*/ uint32 need_write_position; /**< Every step write address (0 to 0xFFFFFFFF) */
    /*16*/ uint32 byte_per_write;      /**< Current flash block write size (0 to 0xFFFFFFFF) */
    /*17*/ uint32 mac_address_pc_03;   /**< The MAC address of PC (0 to 0xFFFFFFFF) */
    /*18*/ uint32 mac_address_pc_45;   /**< The MAC address of PC (0 to 0xFFFFFFFF) */
    /*19*/ uint32 reserved;            /**< Reserved field (0 to 0xFFFFFFFF) */
    /*20*/ uint32 option;              /**< For DVA : Flash status (0 to DVA_SPI_STATE_ERROR), For CVB : Check id to confirm packet reply (0 to 0xFFFFFFFF)*/
    /*21*/ uint32 sequence_id;         /**< Sequence Id (0 to 0xFFFFFFFF) */
    /*22*/ uint32 checksum;            /**< Sum of all read back value (0 to 0xFFFFFFFF) */
    /*23*/ uint32 check_read_back;     /**< Read back check flag (0 to 1) */
    /*24*/ uint32 fw_ver;              /**< Firmware version (0 to 0xFFFFFFFF) */
    /*25*/ uint32 rw_addr;             /**< Read back flash start address (0 to 0xFFFFFFFF) */
    /*26*/ uint32 rw_value;            /**< The read back total length of flash data (0 to 0xFFFFFFFF) */
} EtherAccessHeader_t;

typedef struct
{
    /*01*/ uint16 RTL9040_Identity; /**< Realtek Id (0) */
} EtherAccessHeader1_t;

typedef struct
{
    /*01*/ uint16 RTL9040_Identity; /**< Realtek Id (swap of 0x0419) */
    /*02*/ uint32 command_id;       /**< Command Id (swap of 1 to DVA_FLASH_EVENT_CLEAR) */
    /*03*/ uint32 ack_id;           /**< Ack Id (swap of 0 to DVA_CPU_FAIL_STATE) */
    /*04*/ uint32 hash_code0;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*05*/ uint32 hash_code1;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*06*/ uint32 hash_code2;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*07*/ uint32 hash_code3;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*08*/ uint32 hash_code4;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*09*/ uint32 hash_code5;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*10*/ uint32 hash_code6;       /**< Hash value (0 to 0xFFFFFFFF) */
    /*11*/ uint32 hash_code7;       /**< Hash value (0 to 0xFFFFFFFF) */

    //------------Secure encode------------
    /*12*/ uint32 rand_id;             /**< Random Id (0 to 0xFFFFFFFF) */
    /*13*/ uint32 flash_update_addr;   /**< Flash write start address (0 to 0xFFFFFFFF) */
    /*14*/ uint32 total_write_size;    /**< Flash total write size (0 to 0xFFFFFFFF) */
    /*15*/ uint32 need_write_position; /**< Every step write address (0 to 0xFFFFFFFF) */
    /*16*/ uint32 byte_per_write;      /**< Current flash block write size (0 to 0xFFFFFFFF) */
    /*17*/ uint32 mac_address_pc_03;   /**< The MAC address of PC (0 to 0xFFFFFFFF) */
    /*18*/ uint32 mac_address_pc_45;   /**< The MAC address of PC (0 to 0xFFFFFFFF) */
    /*19*/ uint32 reserved;            /**< Reserved field (0 to 0xFFFFFFFF) */
    /*20*/ uint32 option;              /**< For DVA : Flash status (0 to DVA_SPI_STATE_ERROR), For CVB : Check id to confirm packet reply (0 to 0xFFFFFFFF)*/
    /*21*/ uint32 sequence_id;         /**< Sequence Id (0 to 0xFFFFFFFF) */
    /*22*/ uint32 checksum;            /**< Sum of all read back value (0 to 0xFFFFFFFF) */
    /*23*/ uint32 check_read_back;     /**< Read back check flag (0 to 1) */
    /*24*/ uint32 fw_ver;              /**< Firmware version (0 to 0xFFFFFFFF) */
    /*25*/ uint32 rw_addr;             /**< Read back flash start address (0 to 0xFFFFFFFF) */
    /*26*/ uint32 rw_value;            /**< The read back total length of flash data (0 to 0xFFFFFFFF) */
} EtherAccessHeader2_t;

typedef struct
{
    /*01*/ uint8  command;                    /**< Reg read/write command Id (swap of 1 to 6) */
    /*02*/ uint8  reg_addr[RTK_REG_ADDR_LEN]; /**< Reg read/write address (0x0 to 0xFF for 3 elements) */
    /*03*/ uint32 value;                      /**< Reg read/write value (0 to 0xFFFFFFFF) */
} EtherAccessRegister_t;

typedef struct
{
    uint8                dmac[MAC_ADDR_LEN];                                                     /**< Destination Mac address (0x0 to 0xFF for 6 elements) */
    uint8                smac[MAC_ADDR_LEN];                                                     /**< Source Mac address (0x0 to 0xFF for 6 elements) */
    uint8                ethertype[RTK_ETHERTYPE_LEN];                                           /**< Ethertype (0x8899) */
    EtherAccessHeader1_t header1;                                                                /**< Ethernet Access header (N/A) */
    EtherAccessHeader2_t header2;                                                                /**< Ethernet Access header (N/A) */
    uint8                data[(RTK_ETHERACCESS_MAX_BURST + 1u) * sizeof(EtherAccessRegister_t)]; /**< Ethernet Access payload (0x0 to 0xFF for 1264 elements) */
} EtherAccessL2Header_t;

typedef struct
{
    uint8 ip_hl : 4,                       /**< header length (5) */
        ip_v : 4;                          /**< version (4) */
    uint8  ip_tos;                         /**< type of service (0) */
    uint16 ip_len;                         /**< total length (48) */
    uint16 ip_id;                          /**< identification (0) */
    uint16 ip_off;                         /**< fragment offset field (0) */
    uint8  ip_ttl;                         /**< time to live (0 to 0xFF) */
    uint8  ip_p;                           /**< protocol (0xFF) */
    uint16 ip_sum;                         /**< checksum (0 to 0xFFFF) */
    uint32 ip_src, ip_dst;                 /**< source and dest address (0 to 0xFFFFFFFF) */
    uint32 command_id;                     /**< Command Id (swap of 14 to 18) */
    uint32 ack_id;                         /**< Ack Id (swap of 0 to 3) */
    uint8  uuid[RTK_ETHERACCESS_UUID_LEN]; /**< UUID (0x0 to 0xFF for 10 elements) */
    uint8  type;                           /**< OTP type (0 to 5) */
    uint8  type_id;                        /**< OTP type ID (0 to 3) */
    uint32 data_len;                       /**< Length (swap of 0 to 0x800) */
    uint32 checksum;                       /**< Checksum (0x0 to 0xFFFFFFFF) */
} EtherAccessIpHdr_t;

typedef struct
{
    uint8 ip_hl : 4, /**< header length (5) */
        ip_v : 4;    /**< version (4) */
    uint8 ip_tos;    /**< type of service (0) */
} EtherAccessIpHdr1_t;

typedef struct
{
    uint16 ip_len;                         /**< total length (48) */
    uint16 ip_id;                          /**< identification (0) */
    uint16 ip_off;                         /**< fragment offset field (0) */
    uint8  ip_ttl;                         /**< time to live (0) */
    uint8  ip_p;                           /**< protocol (0xFF) */
    uint16 ip_sum;                         /**< checksum (0 to 0xFFFF) */
    uint32 ip_src, ip_dst;                 /**< source and dest address (0 to 0xFFFFFFFF) */
    uint32 command_id;                     /**< Command Id (swap of 14 to 18) */
    uint32 ack_id;                         /**< Ack Id (swap of 0 to 3) */
    uint8  uuid[RTK_ETHERACCESS_UUID_LEN]; /**< UUID (0x0 to 0xFF for 10 elements) */
    uint8  type;                           /**< OTP type (0 to 5) */
    uint8  type_id;                        /**< OTP type ID (0 to 3) */
    uint32 data_len;                       /**< Length (swap of 0 to 0x800) */
    uint32 checksum;                       /**< Checksum (0x0 to 0xFFFFFFFF) */
} EtherAccessIpHdr2_t;

typedef struct
{
    uint8               dmac[MAC_ADDR_LEN];              /**< Destination Mac address (0x0 to 0xFF for 6 elements) */
    uint8               smac[MAC_ADDR_LEN];              /**< Source Mac address (0x0 to 0xFF for 6 elements) */
    uint8               ethertype[RTK_ETHERTYPE_LEN];    /**< Ethertype (0x0800) */
    EtherAccessIpHdr1_t header1;                         /**< Ethernet Access header (N/A) */
    EtherAccessIpHdr2_t header2;                         /**< Ethernet Access header (N/A) */
    uint8               data[RTK_ETHERACCESS_MAX_FLASH]; /**< Ethernet Access payload (0x0 to 0xFF for 1248 elements) */
} EtherAccessL2IpHdr_t;

typedef struct
{
    uint8  type;     /**< OTP type (0 to 5) */
    uint8  typeId;   /**< OTP type ID (0 to 3) */
    uint32 len;      /**< Length (swap of 0 to 0x800) */
    uint32 checksum; /**< Checksum (0x0 to 0xFFFFFFFF) */
} EtherAccessOtpData_t;

typedef struct
{
    uint8  uuid[RTK_ETHERACCESS_UUID_LEN];
    uint8  type;
    uint8  typeId;
    uint32 dataLen;
    uint32 checksum;
} RtkEtherAccessL2Otp_t;

typedef struct
{
    uint8                 dmac[MAC_ADDR_LEN];
    uint8                 smac[MAC_ADDR_LEN];
    uint8                 ethertype[RTK_ETHERTYPE_LEN];
    EtherAccessHeader1_t  header1;
    EtherAccessHeader2_t  header2;
    RtkEtherAccessL2Otp_t otpHdr;
    uint8                 data[RTK_ETHERACCESS_MAX_FLASH];
} RtkEtherAccessL2OtpHdr_t;

typedef struct
{
    uint16 addr;     /**< Address (0 to 0xFFFF) */
    uint16 len;      /**< Length (swap of 0 to 0x400) */
    uint32 checksum; /**< Checksum (0x0 to 0xFFFFFFFF) */
} EtherAccessAdaptOtpData_t;

typedef struct
{
    uint8  uuid[RTK_ETHERACCESS_UUID_LEN]; /**< UUID (0x0 to 0xFF for 10 elements) */
    uint16 addr;                           /**< OTP address (0 to 0xFFFF) */
    uint16 len;                            /**< OTP Length (0 to OTP_MAX_LENGTH) */
    uint16 reserved;                       /**< reserved (0) */
    uint32 checksum;                       /**< Checksum (0x0 to 0xFFFFFFFF) */
} EtherAccessL2AdaptOtp_t;

typedef struct
{
    uint8                   dmac[MAC_ADDR_LEN];
    uint8                   smac[MAC_ADDR_LEN];
    uint8                   ethertype[RTK_ETHERTYPE_LEN];
    EtherAccessHeader1_t    header1;
    EtherAccessHeader2_t    header2;
    EtherAccessL2AdaptOtp_t otpHdr;
    uint8                   data[RTK_ETHERACCESS_MAX_FLASH];
} RtkEtherAccessL2AdaptOtpHdr_t;

#endif /* RTKAS_API_ETHERNET_ACCESS_EXT_H__ */
