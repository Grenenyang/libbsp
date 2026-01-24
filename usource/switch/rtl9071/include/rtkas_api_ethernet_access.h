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

#ifndef RTKAS_API_ETHERNET_ACCESS_H__
#define RTKAS_API_ETHERNET_ACCESS_H__

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ethernet_access.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ethernet_access.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ethernet_access.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ethernet_access.h"
#endif

/**
 * @brief This struct is used for flash write flow.
 */
typedef struct
{
    uint32 addr; /**< addr Flash start write address (0x0 to 0xFFFFFF) */
    uint32 len;  /**< len Data length (0x1 to 0x80000) */
    uint8 *data; /**< data Data buffer (0x0 to 0xFF for len elements) */
} EthAccFlowFlashWrite_t;

/**
 * @brief This struct is used for flash read flow.
 */
typedef struct
{
    uint32 addr; /**< addr Flash start read address (0x0 to 0xFFFFFF) */
    uint32 len;  /**< len Data length (1 to 0x400) */
    uint8 *data; /**< data Data buffer (0x0 to 0xFF for len elements) */
} EthAccFlowFlashRead_t;

/**
 * @brief This struct is used for generating flash erase sector packet.
 */
typedef struct
{
    uint32 addr; /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32 num;  /**< [in] num flash erase number (0x1 to 4095) */
} EthAccFlowFlashSecErase_t;

/**
 * @brief This struct is used for generating flash erase block packet.
 */
typedef struct
{
    uint32 addr; /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32 num;  /**< [in] num flash erase number (0x1 to 256) */
} EthAccFlowFlashBlkErase_t;

/**
 * @brief This struct is used for generating flash erase all packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashEraseAll_t;

/**
 * @brief This struct is used for generating flash start packet.
 */
typedef struct
{
    EtherAccessMac_t mac;      /**< [in] mac MAC address (N/A) */
    uint32           addr;     /**< [in] addr Start write address (0x0 to 0xFFFFFF) */
    uint32           size;     /**< [in] size Total write size (0x1 to 0x80000) */
    uint32           checksum; /**< [in] checksum Checksum (0x0 to 0xFFFFFFFF) */
#if defined(RTL_907XD_VA)
    uint32 perLen; /**< [in] perLen Write size per packet (0x1 to 0x400) */
#endif
    EtherAccessL2Header_t *packet; /**< [out] packet Packet (N/A) */
} EthAccGenFlashStart_t;

/**
 * @brief This struct is used for generating flash write packet.
 */
typedef struct
{
    EtherAccessMac_t       mac;      /**< [in] mac MAC address (N/A) */
    uint32                 addr;     /**< [in] addr Start write address (0x0 to 0xFFFFFF) */
    uint8                 *flash;    /**< [in] flash Context of flash (0x0 to 0xFF for len elements) */
    uint32                 len;      /**< [in] len Length of flash (0x1 to 0x80000) */
    uint32                 seqId;    /**< [in] seqId Sequence ID (0x1 to 0xFFFFFFFF) */
    uint32                 checksum; /**< [in] checksum Checksum (0x0 to 0xFFFFFFFF) */
    EtherAccessL2Header_t *packet;   /**< [out] packet Packet (N/A) */
} EthAccGenFlashWrite_t;

/**
 * @brief This struct is used for checking flash erase all packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckFlashEraseAll_t;

/**
 * @brief This struct is used for checking flash write reply packet.
 */
typedef struct
{
    uint8 *packet;   /**< [in] packet Packet (N/A) */
    uint32 seqId;    /**< [in] seqId Sequence ID (0x1 to 0xFFFFFFFF) */
    uint32 ackId;    /**< [out] ackId ACK ID (1 to 2) */
    uint32 checksum; /**< [out] checksum Checksum (0x0 to 0xFFFFFFFF) */
} EthAccCheckRepWrite_t;

/**
 * @brief This struct is used for generating flash erase sector packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 addr;               /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32                 num;                /**< [in] num flash erase number (0x1 to 4095) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashEraseSector_t;

/**
 * @brief This struct is used for checking flash erase sector packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckFlashEraseSector_t;

/**
 * @brief This struct is used for generating flash erase block packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 addr;               /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32                 num;                /**< [in] num flash erase number (0x1 to 256) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashEraseBlock_t;

typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckFlashEraseBlock_t;

/**
 * @brief This struct is used for generating flash read packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 addr;               /**< [in] addr Address of flash read (0x0 to 0xFFFFFF) */
    uint32                 len;                /**< [in] len Size of flash read (0x1 to 0x400) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashRead_t;

/**
 * @brief This struct is used for checking flash read reply packet.
 */
typedef struct
{
#if defined(RTL_907XD_V1) || (RTL_907XC_VA)
    EtherAccessL2Header_t *packetStruct; /**< [out] packet Packet (N/A) */
#endif
#if defined(RTL_907XD_VA) || (RTL_907XC_VB)
    uint8 *packetArr; /**< [out] packet Packet (N/A) */
#endif
    uint8 *data; /**< [out] data Data of flash read (0x0 to 0xFF for len elements) */
    uint32 len;  /**< [in] len Size of flash read (0x1 to 0x400) */
} EthAccCheckRepRead_t;

/**
 * @brief This struct is used for generating flash erase request packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 addr;               /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32                 eraseCmd;           /**< [in] num flash erase number (0x1 to 0x3) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashEraseReq_t;

/**
 * @brief This struct is used for checking flash erase request reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckFlashEraseReq_t;

/**
 * @brief This struct is used for generating flash check request status packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashStatusCheck_t;

/**
 * @brief This struct is used for checking flash status request reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckFlashStatusCheck_t;

/**
 * @brief This struct is used for flash erase sector request flow.
 */
typedef struct
{
    uint32 addr; /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32 num;  /**< [in] flash erase number (0x1 to 256) */
} EthAccFlowFlashSecEraseReq_t;

/**
 * @brief This struct is used for flash erase block request flow.
 */
typedef struct
{
    uint32 addr; /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32 num;  /**< [in] flash erase number (0x1 to 256) */
} EthAccFlowFlashBlkEraseReq_t;

/**
 * @brief This struct is used for generating flash write request packet.
 */
typedef struct
{
    EtherAccessMac_t       mac;      /**< [in] mac MAC address (N/A) */
    uint32                 addr;     /**< [in] addr Start write address (0x0 to 0xFFFFFF) */
    uint8                 *flash;    /**< [in] flash Context of flash (0x0 to 0xFF for len elements) */
    uint32                 len;      /**< [in] len Length of flash (0x1 to 0x80000) */
    uint32                 seqId;    /**< [in] seqId Sequence ID (0x1 to 0xFFFFFFFF) */
    uint32                 checksum; /**< [in] checksum Checksum (0x0 to 0xFFFFFFFF) */
    EtherAccessL2Header_t *packet;   /**< [out] packet Packet (N/A) */
} EthAccGenFlashWriteReq_t;

/**
 * @brief This struct is used for flash write request flow.
 */
typedef struct
{
    uint32 addr; /**< [in] Flash start write address (0x0 to 0xFFFFFF) */
    uint32 len;  /**< [in] Data length (0x1 to 0x80000) */
    uint8 *data; /**< [in] Data buffer (0x0 to 0xFF for len elements) */
} EthAccFlowFlashWriteReq_t;

/**
 * @brief This struct is used for generating flash erase event packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 addr;               /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32                 eraseCmd;           /**< [in] num flash erase number (0x1 to 0x3) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashEraseEvent_t;

/**
 * @brief This struct is used for checking flash erase event reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckFlashEraseEvent_t;

/**
 * @brief This struct is used for generating flash write event packet.
 */
typedef struct
{
    EtherAccessMac_t       mac;      /**< [in] mac MAC address (N/A) */
    uint32                 addr;     /**< [in] addr Start write address (0x0 to 0xFFFFFF) */
    uint8                 *flash;    /**< [in] flash Context of flash (0x0 to 0xFF for len elements) */
    uint32                 len;      /**< [in] len Length of flash (0x1 to 0x80000) */
    uint32                 seqId;    /**< [in] seqId Sequence ID (0x1 to 0xFFFFFFFF) */
    uint32                 checksum; /**< [in] checksum Checksum (0x0 to 0xFFFFFFFF) */
    EtherAccessL2Header_t *packet;   /**< [out] packet Packet (N/A) */
} EthAccGenFlashWriteEvent_t;

/**
 * @brief This struct is used for generating flash check event packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashEvent_t;

/**
 * @brief This struct is used for checking flash event reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckFlashEvent_t;

/**
 * @brief This struct is used for flash erase sector event flow.
 */
typedef struct
{
    uint32 addr; /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32 num;  /**< [in] flash erase number (0x1 to 256) */
} EthAccFlowFlashSecEraseEvent_t;

/**
 * @brief This struct is used for flash erase block event flow.
 */
typedef struct
{
    uint32 addr; /**< [in] addr Flash erase address (0x0 to 0xFFFFFF) */
    uint32 num;  /**< [in] flash erase number (0x1 to 256) */
} EthAccFlowFlashBlkEraseEvent_t;

/**
 * @brief This struct is used for flash write event flow.
 */
typedef struct
{
    uint32 addr; /**< [in] Flash start write address (0x0 to 0xFFFFFF) */
    uint32 len;  /**< [in] Data length (0x1 to 0x80000) */
    uint8 *data; /**< [in] Data buffer (0x0 to 0xFF for len elements) */
} EthAccFlowFlashWriteEvent_t;

/**
 * @brief This struct is used for generating flash read event packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 addr;               /**< [in] addr Address of flash read (0x0 to 0xFFFFFF) */
    uint32                 len;                /**< [in] len Size of flash read (0x1 to 0x400) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashReadEvent_t;

/**
 * @brief This struct is used for checking flash read event reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [out] packet Packet (N/A) */
} EthAccCheckRepReadEvent_t;

/**
 * @brief This struct is used for generating flash read check event packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 offset;             /**< [in] Offset of flash read (0x1 to sram access size) */
    uint32                 len;                /**< [in] len Size of flash read (0x1 to 0x400) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashReadCheck_t;

/**
 * @brief This struct is used for checking flash read check event reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [out] packet Packet (N/A) */
    uint32 offset; /**< [in] Offset of flash read (0x1 to sram access size) */
    uint32 len;    /**< [in] len Size of flash read (0x1 to 0x400) */
    uint8 *data;   /**< [out] data Data of flash read (0x0 to 0xFF for len elements) */
} EthAccCheckFlashReadCheck_t;

/**
 * @brief This struct is used for flash read event flow.
 */
typedef struct
{
    uint32 addr; /**< [in] Flash start read address (0x0 to 0xFFFFFF) */
    uint32 len;  /**< [in] Data length (0x1 to 0x80000) */
    uint8 *data; /**< [out] Data buffer (0x0 to 0xFF for len elements) */
} EthAccFlowFlashReadEvent_t;

/**
 * @brief This struct is used for generating flash clear status event packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenFlashClearEvent_t;

/**
 * @brief This struct is used for checking flash clear event reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [out] packet Packet (N/A) */
} EthAccCheckFlashClearEvent_t;

/**
 * @brief This struct is used for generating reset system packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenResetSystem_t;

/**
 * @brief This struct is used for checking reset system reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
} EthAccCheckResetSystem_t;

/**
 * @brief This struct is used for patch write flow.
 */
typedef struct
{
    uint32 addr; /**< [in] addr Patch start write address (0x0 to 0xBFFFF) */
    uint32 len;  /**< [in] len Data length (1 to 0xC0000) */
    uint8 *data; /**< [in] data Data buffer (0x0 to 0xFF for len elements) */
} EthAccFlowPatchWrite_t;

/**
 * @brief This struct is used for generating patch start packet.
 */
typedef struct
{
    uint8                  sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                  dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint32                 addr;               /**< [in] addr Start write address (0x0 to 0xBFFFF) */
    uint32                 size;               /**< [in] size Total write size (0x1 to 0xC0000) */
    uint32                 checksum;           /**< [in] checksum Checksum (0x0 to 0xFFFFFFFF) */
    EtherAccessL2Header_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenPatchStart_t;

/**
 * @brief This struct is used for generating patch write packet.
 */
typedef struct
{
    EtherAccessMac_t       mac;      /**< [in] mac MAC address (N/A) */
    uint32                 addr;     /**< [in] addr Start write address (0x0 to 0xBFFFF) */
    uint8                 *patch;    /**< [in] patch Context of patch (0x0 to 0xFF for len elements) */
    uint32                 len;      /**< [in] len Length of patch (1 to 0xC0000) */
    uint32                 seqId;    /**< [in] seqId Sequence ID (0x1 to 0xFFFFFFFF) */
    uint32                 checksum; /**< [in] checksum Checksum (0x0 to 0xFFFFFFFF) */
    EtherAccessL2Header_t *packet;   /**< [out] packet Packet (N/A) */
} EthAccGenPatchWrite_t;

/**
 * @brief This struct is used for generating reg access reply packet.
 */
typedef struct
{
    EtherAccessMac_t mac;    /**< [in] mac MAC address (N/A) */
    uint32           number; /**< [in] number Number of register to read/write (1 to 158) */
    uint8           *regCmd; /**< [in] regCmd Command of register to read/write (1 to 6 for number elements) */
    uint8 (*regAddr)[3];     /**< [in] regAddr Address of register to read/write (0 to 0xFF for number * 3 elements) */
    uint32 *regValue;        /**< [in] regValue Value of register to write (0 to 0xFFFFFFFF for number elements) */
#if defined(RTL_907XD_VA)
    uint32 portMask; /**< [in] port mask for port clock check (0 to 0xFFFF) */
#endif
    EtherAccessL2Header_t *packet; /**< [out] packet Packet (N/A) */
} EthGenBurst_t;

/**
 * @brief This struct is used for checking reg access reply packet.
 */
typedef struct
{
    uint32  number;   /**< [in] number Number of register to read/write (1 to 158) */
    uint32 *regValue; /**< [out] regValue Value of register to write (0 to 0xFFFFFFFF for number elements) */
    uint8  *packet;   /**< [in] packet Packet (N/A) */
} EthChkBurst_t;

/**
 * @brief This struct is used for table get flow.
 */
typedef struct
{
#if defined(RTL_907XD_V1)
    dv1_table_list_t tableDV1; /**< [in] table Table (0 to 39) */
#endif
#if defined(RTL_907XD_VA)
    dva_table_list_t tableDVa; /**< [in] table Table (0 to 39) */
#endif
#if defined(RTL_907XC_VB)
    cvb_table_list_t tableCVb; /**< [in] table Table (0 to 39) */
#endif
#if defined(RTL_907XC_VA)
    cva_table_list_t tableCVa; /**< [in] table Table (0 to 39) */
#endif
    uint32  startIndex; /**< [in] startIndex Start index (0 to 4095) */
    uint32  entryNum;   /**< [in] entryNum Entry number (1 to 4096) */
    uint32 *data;       /**< [out] data Data (0x0 to 0xFF for entryNum * rtl906x_table_list[table].datareg_num elements) */
} EthAccFlowTableEntryGet_t;

/**
 * @brief This struct is used for table set flow.
 */
typedef struct
{
#if defined(RTL_907XD_V1)
    dv1_table_list_t tableDV1; /**< [in] table Table (0 to 39) */
#endif
#if defined(RTL_907XD_VA)
    dva_table_list_t tableDVa; /**< [in] table Table (0 to 39) */
#endif
#if defined(RTL_907XC_VB)
    cvb_table_list_t tableCVb; /**< [in] table Table (0 to 39) */
#endif
#if defined(RTL_907XC_VA)
    cva_table_list_t tableCVa; /**< [in] table Table (0 to 39) */
#endif
    uint32  startIndex; /**< [in] startIndex Start index (0 to 4095) */
    uint32  entryNum;   /**< [in] entryNum Entry number (1 to 4096) */
    uint32 *data;       /**< [out] data Data (0x0 to 0xFF for entryNum * rtl906x_table_list[table].datareg_num elements) */
} EthAccFlowTableEntrySet_t;

/**
 * @brief This struct is used for generating L3 OTP ask packet.
 */
typedef struct
{
    uint8                 macAddr[MAC_ADDR_LEN]; /**< [in] macAddr MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint32                sip;                   /**< [in] sip Source IP (0x0 to 0xFFFFFFFF) */
    EtherAccessL2IpHdr_t *packet;                /**< [out] packet Packet (N/A) */
} EthAccGenL3OtpAsk_t;

/**
 * @brief This struct is used for generating L3 OTP check packet.
 */
typedef struct
{
    uint8                 macAddr[MAC_ADDR_LEN]; /**< [in] macAddr MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint32                sip;                   /**< [in] sip Source IP (0x0 to 0xFFFFFFFF) */
    uint8                *uuid;                  /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint8                 type;                  /**< [in] type OTP type (0 to 5) */
    uint8                 typeId;                /**< [in] typeId OTP type ID (0 to 3) */
    uint32                len;                   /**< [in] len Length (0 to 0x800) */
    EtherAccessL2IpHdr_t *packet;                /**< [out] packet Packet (N/A) */
} EthAccGenL3OtpCheck_t;

/**
 * @brief This struct is used for generating L3 OTP write packet.
 */
typedef struct
{
    uint8                 macAddr[MAC_ADDR_LEN]; /**< [in] macAddr MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint32                sip;                   /**< [in] sip Source IP (0x0 to 0xFFFFFFFF) */
    uint8                *uuid;                  /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint8                *data;                  /**< [in] data Data of OTP (0x0 to 0xFF for len elements) */
    EtherAccessOtpData_t  otpInfo;               /**< [in] otpInfo OTP information (N/A) */
    EtherAccessL2IpHdr_t *packet;                /**< [out] packet Packet (N/A) */
} EthAccGenL3OtpWrite_t;

/**
 * @brief This struct is used for checking L3 packet ttl.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
    uint8  ttl;    /**< [out] ttl TTL (0 to 255) */
} EthAccGetL3OtpTtl_t;

/**
 * @brief This struct is used for checking L3 OTP join reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
    uint32 ackId;  /**< [out] ackId ACK ID (1 to 3) */
    uint8 *uuid;   /**< [out] uuid UUID (0x0 to 0xFF for 10 elements) */
} EthAccCheckL3OtpJoin_t;

/**
 * @brief This struct is used for checking L3 OTP check reply packet.
 */
typedef struct
{
    EtherAccessL2IpHdr_t *packet; /**< [out] packet Packet (N/A) */
    uint8                *uuid;   /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint8                 type;   /**< [in] type OTP type (0 to 5) */
    uint8                 typeId; /**< [in] typeId OTP type ID (0 to 3) */
    uint32                len;    /**< [in] len Length (0 to 0x800) */
    uint32                ackId;  /**< [out] ackId ACK ID (1 to 3) */
    uint8                *data;   /**< [out] data Data (0x0 to 0xFF for len elements) */
} EthAccCheckL3OtpRep_t;

/**
 * @brief This struct is used for generating L2 OTP ask packet.
 */
typedef struct
{
    uint8                     sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC */
    uint8                     dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address */
    RtkEtherAccessL2OtpHdr_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenL2OtpAsk_t;

/**
 * @brief This struct is used for generating L2 OTP check packet.
 */
typedef struct
{
    uint8                     sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC */
    uint8                     dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address */
    uint8                    *uuid;               /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint8                     type;               /**< [in] type OTP type (0 to 5) */
    uint8                     typeId;             /**< [in] typeId OTP type ID (0 to 3) */
    uint32                    len;                /**< [in] len Length (0 to 0x800) */
    RtkEtherAccessL2OtpHdr_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenL2OtpCheck_t;

/**
 * @brief This struct is used for generating L2 OTP write packet.
 */
typedef struct
{
    uint8                     sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                     dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint8                    *uuid;               /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint8                    *data;               /**< [in] data Data of OTP (0x0 to 0xFF for len elements) */
    EtherAccessOtpData_t      otpInfo;            /**< [in] otpInfo OTP information (N/A) */
    RtkEtherAccessL2OtpHdr_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenL2OtpWrite_t;

/**
 * @brief This struct is used for checking L2 OTP join reply packet.
 */
typedef struct
{
    uint8 *packet; /**< [in] packet Packet (N/A) */
    uint32 ackId;  /**< [out] ackId ACK ID (1 to 3) */
    uint8 *uuid;   /**< [out] uuid UUID (0x0 to 0xFF for 10 elements) */
} EthAccCheckL2OtpJoin_t;

/**
 * @brief This struct is used for checking L2 OTP check reply packet.
 */
typedef struct
{
    RtkEtherAccessL2OtpHdr_t *packet; /**< [out] packet Packet (N/A) */
    uint8                    *uuid;   /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint8                     type;   /**< [in] type OTP type (0 to 5) */
    uint8                     typeId; /**< [in] typeId OTP type ID (0 to 3) */
    uint32                    len;    /**< [in] len Length (0 to 0x800) */
    uint32                    ackId;  /**< [out] ackId ACK ID (1 to 3) */
    uint8                    *data;   /**< [out] data Data (0x0 to 0xFF for len elements) */
} EthAccCheckL2OtpRep_t;

/**
 * @brief This struct is used for generating L2 adapt OTP check packet.
 */
typedef struct
{
    uint8                          sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC */
    uint8                          dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address */
    uint8                         *uuid;               /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint16                         addr;               /**< [in] Address (0 to 0xFFFF) */
    uint16                         len;                /**< [in] Length (swap of 0 to 0x400) */
    RtkEtherAccessL2AdaptOtpHdr_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenL2AdaptOtpCheck_t;

/**
 * @brief This struct is used for generating L2 adapt OTP write packet.
 */
typedef struct
{
    uint8                          sMac[MAC_ADDR_LEN]; /**< [in] sMac MAC address of PC (0x0 to 0xFF for 6 elements) */
    uint8                          dMac[MAC_ADDR_LEN]; /**< [in] dMac Destination MAC address (0x0 to 0xFF for 6 elements) */
    uint8                         *uuid;               /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint8                         *data;               /**< [in] data Data of OTP (0x0 to 0xFF for len elements) */
    EtherAccessAdaptOtpData_t      otpInfo;            /**< [in] otpInfo OTP information (N/A) */
    RtkEtherAccessL2AdaptOtpHdr_t *packet;             /**< [out] packet Packet (N/A) */
} EthAccGenL2AdaptOtpWrite_t;

/**
 * @brief This struct is used for checking L2 adapt OTP check reply packet.
 */
typedef struct
{
    RtkEtherAccessL2AdaptOtpHdr_t *packet; /**< [out] packet Packet (N/A) */
    uint8                         *uuid;   /**< [in] uuid UUID (0x0 to 0xFF for 10 elements) */
    uint16                         len;    /**< [in] Length (swap of 0 to 0x400) */
    uint32                         ackId;  /**< [out] ackId ACK ID (1 to 3) */
    uint8                         *data;   /**< [out] data Data (0x0 to 0xFF for len elements) */
} EthAccCheckL2AdaptOtpRep_t;

EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_write(UnitChip_t unitChip, EthAccFlowFlashWrite_t *ethAccFlowFlashWrite);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_read(UnitChip_t unitChip, EthAccFlowFlashRead_t *ethAccFlowFlashRead);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_sec_erase(UnitChip_t unitChip, EthAccFlowFlashSecErase_t *ethAccFlowFlashSecErase);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_blk_erase(UnitChip_t unitChip, EthAccFlowFlashBlkErase_t *ethAccFlowFlashBlkErase);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_all_erase(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_disable_protocol(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_erase_all(UnitChip_t unitChip, EthAccGenFlashEraseAll_t *ethAccGenFlashEraseAll);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_start(UnitChip_t unitChip, EthAccGenFlashStart_t *ethAccGenFlashStart);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_write(UnitChip_t unitChip, EthAccGenFlashWrite_t *ethAccGenFlashWrite);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_erase_all(UnitChip_t unitChip, EthAccCheckFlashEraseAll_t *ethAccCheckFlashEraseAll);
EXTERN_API RtkApiRet rtk_eth_acc_check_rep_write(UnitChip_t unitChip, EthAccCheckRepWrite_t *ethAccCheckRepWrite);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_erase_sector(UnitChip_t unitChip, EthAccGenFlashEraseSector_t *ethAccGenFlashEraseSector);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_erase_sector(UnitChip_t unitChip, EthAccCheckFlashEraseSector_t *ethAccCheckFlashEraseSector);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_erase_block(UnitChip_t unitChip, EthAccGenFlashEraseBlock_t *ethAccGenFlashEraseBlock);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_erase_block(UnitChip_t unitChip, EthAccCheckFlashEraseBlock_t *ethAccCheckFlashEraseBlock);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_read(UnitChip_t unitChip, EthAccGenFlashRead_t *ethAccGenFlashRead);
EXTERN_API RtkApiRet rtk_eth_acc_check_rep_read(UnitChip_t unitChip, EthAccCheckRepRead_t *ethAccCheckRepRead);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_erase_req(UnitChip_t unitChip, EthAccGenFlashEraseReq_t *ethAccGenFlashEraseReq);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_erase_req(UnitChip_t unitChip, EthAccCheckFlashEraseReq_t *ethAccCheckFlashEraseReq);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_status_check(UnitChip_t unitChip, EthAccGenFlashStatusCheck_t *ethAccGenFlashStatusCheck);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_status_check(UnitChip_t unitChip, EthAccCheckFlashStatusCheck_t *ethAccCheckFlashStatusCheck);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_req_sec_erase(UnitChip_t unitChip, EthAccFlowFlashSecEraseReq_t *ethAccFlowFlashSecEraseReq);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_req_blk_erase(UnitChip_t unitChip, EthAccFlowFlashBlkEraseReq_t *ethAccFlowFlashBlkEraseReq);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_req_all_erase(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_write_req(UnitChip_t unitChip, EthAccGenFlashWriteReq_t *ethAccGenFlashWriteReq);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_write_req(UnitChip_t unitChip, EthAccFlowFlashWriteReq_t *ethAccFlowFlashWriteReq);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_erase_event(UnitChip_t unitChip, EthAccGenFlashEraseEvent_t *ethAccGenFlashEraseEvent);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_erase_event(UnitChip_t unitChip, EthAccCheckFlashEraseEvent_t *ethAccCheckFlashEraseEvent);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_write_event(UnitChip_t unitChip, EthAccGenFlashWriteEvent_t *ethAccGenFlashWriteEvent);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_event(UnitChip_t unitChip, EthAccGenFlashEvent_t *ethAccGenFlashEvent);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_event(UnitChip_t unitChip, EthAccCheckFlashEvent_t *ethAccCheckFlashEvent);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_event_sec_erase(UnitChip_t unitChip, EthAccFlowFlashSecEraseEvent_t *ethAccFlowFlashSecEraseEvent);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_event_blk_erase(UnitChip_t unitChip, EthAccFlowFlashBlkEraseEvent_t *ethAccFlowFlashBlkEraseEvent);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_event_all_erase(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_event_write(UnitChip_t unitChip, EthAccFlowFlashWriteEvent_t *ethAccFlowFlashWriteEvent);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_read_event(UnitChip_t unitChip, EthAccGenFlashReadEvent_t *ethAccGenFlashReadEvent);
EXTERN_API RtkApiRet rtk_eth_acc_check_rep_read_event(UnitChip_t unitChip, EthAccCheckRepReadEvent_t *ethAccCheckRepReadEvent);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_read_check(UnitChip_t unitChip, EthAccGenFlashReadCheck_t *ethAccGenFlashReadCheck);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_read_check(UnitChip_t unitChip, EthAccCheckFlashReadCheck_t *ethAccCheckFlashReadCheck);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_read_event(UnitChip_t unitChip, EthAccFlowFlashReadEvent_t *ethAccFlowFlashReadEvent);
EXTERN_API RtkApiRet rtk_eth_acc_gen_flash_clear_event(UnitChip_t unitChip, EthAccGenFlashClearEvent_t *ethAccGenFlashClearEvent);
EXTERN_API RtkApiRet rtk_eth_acc_check_flash_clear_event(UnitChip_t unitChip, EthAccCheckFlashClearEvent_t *ethAccGenCheckFlashClearEvent);
EXTERN_API RtkApiRet rtk_eth_acc_flow_flash_clear_event(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_eth_acc_flow_reset_system(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_eth_acc_gen_reset_system(UnitChip_t unitChip, EthAccGenResetSystem_t *ethAccGenResetSystem);
EXTERN_API RtkApiRet rtk_eth_acc_check_reset_system(UnitChip_t unitChip, EthAccCheckResetSystem_t *ethAccCheckResetSystem);
EXTERN_API RtkApiRet rtk_eth_acc_gen_burst_rw(UnitChip_t unitChip, EthGenBurst_t *ethGenBurst);
EXTERN_API RtkApiRet rtk_eth_acc_check_burst_rw(UnitChip_t unitChip, EthChkBurst_t *ethChkBurst);
EXTERN_API RtkApiRet rtk_eth_acc_flow_patch_write(UnitChip_t unitChip, EthAccFlowPatchWrite_t *ethAccFlowPatchWrite);
EXTERN_API RtkApiRet rtk_eth_acc_gen_patch_start(UnitChip_t unitChip, EthAccGenPatchStart_t *ethAccGenPatchStart);
EXTERN_API RtkApiRet rtk_eth_acc_gen_patch_write(UnitChip_t unitChip, EthAccGenPatchWrite_t *ethAccGenPatchWrite);
EXTERN_API RtkApiRet rtk_eth_acc_flow_table_entry_get(UnitChip_t unitChip, EthAccFlowTableEntryGet_t *ethAccFlowTableEntryGet);
EXTERN_API RtkApiRet rtk_eth_acc_flow_table_entry_set(UnitChip_t unitChip, EthAccFlowTableEntrySet_t *ethAccFlowTableEntrySet);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l3_otp_ask(UnitChip_t unitChip, EthAccGenL3OtpAsk_t *ethAccGenL3OtpAsk);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l3_otp_check(UnitChip_t unitChip, EthAccGenL3OtpCheck_t *ethAccGenL3OtpCheck);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l3_otp_write(UnitChip_t unitChip, EthAccGenL3OtpWrite_t *ethAccGenL3OtpWrite);
EXTERN_API RtkApiRet rtk_eth_acc_get_l3_otp_ttl(UnitChip_t unitChip, EthAccGetL3OtpTtl_t *ethAccGetL3OtpTtl);
EXTERN_API RtkApiRet rtk_eth_acc_check_l3_otp_join(UnitChip_t unitChip, EthAccCheckL3OtpJoin_t *ethAccCheckL3OtpJoin);
EXTERN_API RtkApiRet rtk_eth_acc_check_l3_otp_rep(UnitChip_t unitChip, EthAccCheckL3OtpRep_t *ethAccCheckL3OtpRep);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l2_otp_ask(UnitChip_t unitChip, EthAccGenL2OtpAsk_t *ethAccGenL2OtpAsk);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l2_otp_check(UnitChip_t unitChip, EthAccGenL2OtpCheck_t *ethAccGenL2OtpCheck);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l2_otp_write(UnitChip_t unitChip, EthAccGenL2OtpWrite_t *ethAccGenL2OtpWrite);
EXTERN_API RtkApiRet rtk_eth_acc_check_l2_otp_join(UnitChip_t unitChip, EthAccCheckL2OtpJoin_t *ethAccCheckL2OtpJoin);
EXTERN_API RtkApiRet rtk_eth_acc_check_l2_otp_rep(UnitChip_t unitChip, EthAccCheckL2OtpRep_t *ethAccCheckL2OtpRep);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l2_adapt_otp_check(UnitChip_t unitChip, EthAccGenL2AdaptOtpCheck_t *ethAccGenL2AdaptOtpCheck);
EXTERN_API RtkApiRet rtk_eth_acc_gen_l2_adapt_otp_write(UnitChip_t unitChip, EthAccGenL2AdaptOtpWrite_t *ethAccGenL2AdaptOtpWrite);
EXTERN_API RtkApiRet rtk_eth_acc_check_l2_adapt_otp_rep(UnitChip_t unitChip, EthAccCheckL2AdaptOtpRep_t *ethAccCheckL2AdaptOtpRep);
#endif /* RTKAS_API_ETHERNET_ACCESS_H__ */
