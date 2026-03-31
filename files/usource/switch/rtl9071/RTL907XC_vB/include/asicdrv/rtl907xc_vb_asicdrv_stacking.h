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

#ifndef CVB_ASICDRV_STACK_H_
#define CVB_ASICDRV_STACK_H_

/****************************************************************/
/* Header File inclusion                                        */
/****************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_api_stacking.h"
#include "rtl907xc_vb_api_ethernet_access.h"
#include "rtl907xc_vb_table_struct.h"

/****************************************************************/
/* Constant Definition                                          */
/****************************************************************/
#define CVB_STACKING_PAGE                (0x4A000000u)
#define CVB_STACKING_ADDR_MSK            (0x00FFFFFFu)
#define CVB_STACKING_REG_RWCMD           (0x3u)
#define CVB_STACKING_PAGE_RWCMD          (0x2u)
#define CVB_STACKING_ACCESS_CHK_CMD      (0x1u)
#define CVB_STACK_COMMAND_REGISTER_07    (CVB_STACK_COMMAND_REGISTER & 0xFFu)
#define CVB_STACK_COMMAND_REGISTER_815   ((CVB_STACK_COMMAND_REGISTER >> 8u) & 0xFFu)
#define CVB_STACK_COMMAND_REGISTER_1623  ((CVB_STACK_COMMAND_REGISTER >> 16u) & 0xFFu)
#define CVB_STACK_COMMAND_REG_ADDR_07    (CVB_STACK_COMMAND_REG_ADDR & 0xFFu)
#define CVB_STACK_COMMAND_REG_ADDR_815   ((CVB_STACK_COMMAND_REG_ADDR >> 8u) & 0xFFu)
#define CVB_STACK_COMMAND_REG_ADDR_1623  ((CVB_STACK_COMMAND_REG_ADDR >> 16u) & 0xFFu)
#define CVB_STACK_COMMAND_REG_VALUE_07   (CVB_STACK_COMMAND_REG_VALUE & 0xFFu)
#define CVB_STACK_COMMAND_REG_VALUE_815  ((CVB_STACK_COMMAND_REG_VALUE >> 8u) & 0xFFu)
#define CVB_STACK_COMMAND_REG_VALUE_1623 ((CVB_STACK_COMMAND_REG_VALUE >> 16u) & 0xFFu)

#define CVB_STACK_CFG_SIZE             (0x6u)
#define CVB_STACK_MAC_DOMAIN_ENTRY_LEN (0x2Au)
#define CVB_STACK_BURST_HDR_LEN        (0x2Cu)
#define CVB_STACK_BURST_READ_WRITE_LEN (CVB_RTK_ETHERACCESS_HEADER_LEN + CVB_STACK_BURST_HDR_LEN + (RTK_ETHERACCESS_MAX_BURST * 8u))
/* Dummy reg value */
#define CVB_STACK_LOAD_FW_BY_FLASH (0x0u)
#define CVB_STACK_LOAD_FW_BY_SRAM  (0x1u)
/* Packet value value */
#define CVB_STACK_REMOTE_LOAD_FLAG_FLASH (0x0u)
#define CVB_STACK_REMOTE_LOAD_FLAG_SRAM  (0x1u)
#define CVB_STACK_L2_MEM_BASE            (0x100000u)

#define CVB_STACK_DMY1_FW_LOAD_TYPE_LSP (29u)
#define CVB_STACK_DMY1_FW_LOAD_TYPE_LEN (1u)
#define CVB_STACK_DMY1_GROUP_ID_LSP     (16u)
#define CVB_STACK_DMY1_GROUP_ID_LEN     (13u)
#define CVB_STACK_DMY1_MASTER_ID_LSP    (8u)
#define CVB_STACK_DMY1_MASTER_ID_LEN    (4u)
#define CVB_STACK_DMY1_MEMBER_NUM_LSP   (4u)
#define CVB_STACK_DMY1_MEMBER_NUM_LEN   (4u)
#define CVB_STACK_DMY2_MAC4_LSP         (24u)
#define CVB_STACK_DMY2_MAC4_LEN         (8u)
#define CVB_STACK_DMY2_MAC5_LSP         (16u)
#define CVB_STACK_DMY2_MAC5_LEN         (8u)
#define CVB_STACK_DMY2_UNIT_LSP         (8u)
#define CVB_STACK_DMY2_UNIT_LEN         (4u)

/****************************************************************/
/* Type Definition                                              */
/****************************************************************/

typedef struct
{
    /* 48~51 */
    uint32 rwNum;
    /* 52~83 */
    uint8 hash[32];
    /* 84~87 */
    uint32 randomNum;
    /* 88~89 */
    uint16 groupId;
    /* 90 */
    uint8 fromUnit;
    /* 91 */
    uint8 toUnit;
    /* 92~1356 is regAddr and regValue*/
} cvb_StackBurstReg_t;

typedef struct
{
    uint8                dmac[MAC_ADDR_LEN];
    uint8                smac[MAC_ADDR_LEN];
    uint8                ethertype[RTK_ETHERTYPE_LEN];
    EtherAccessHeader1_t header1;
    EtherAccessHeader2_t header2;
    cvb_StackBurstReg_t  stkHeader;
    uint8                data[RTK_ETHERACCESS_MAX_BURST * sizeof(EtherAccessRegister_t)];
} cvb_StackEthBurstHeader_t;

typedef struct
{
    /* 48~49 */
    uint16 groupId;
    /* 50 */
    uint8 masterId;
    /* 51 */
    uint8 destId;
    /* 52 */
    uint8 desMAC4;
    /* 53 */
    uint8 desMAC5;
    /* 54~55 */
    uint16 cfgLength;
    /* 56~57 */
    uint16 seq;
    /* 58~59 */
    uint16 seqTotal;
    /* 60~91 */
    uint8 hash[32];
    /* 92~95 */
    uint32 randomNum;
    /* 96 */
    uint8 index;
    /* 97 */
    uint8 blkIdx1;
    /* 98 */
    uint8 blkIdx2;
    /* 99 */
    uint8 tableMode;
} cvb_StackPayloadRemoteCfg_t;

typedef struct
{
    uint8                       dmac[MAC_ADDR_LEN];
    uint8                       smac[MAC_ADDR_LEN];
    uint8                       ethertype[RTK_ETHERTYPE_LEN];
    EtherAccessHeader1_t        header1;
    EtherAccessHeader2_t        header2;
    cvb_StackPayloadRemoteCfg_t stkHeader;
    /* 100~1387 */
    uint8 fwData[CVB_STACK_REMOTE_LOAD_LENGTH_UNIT];
} cvb_StackEthCfgHeader_t;

typedef struct
{
    /* 48~49 */
    uint16 groupId;
    /* 50 */
    uint8 masterId;
    /* 51 */
    uint8 destId;
    /* 52 */
    uint8 desMAC4;
    /* 53 */
    uint8 desMAC5;
    /* 54~55 */
    uint16 dataLength;
    /* 56~57 */
    uint16 seq;
    /* 58~59 */
    uint16 seqTotal;
    /* 60~63 */
    uint32 oriStAddr;
    /* 64~67 */
    uint32 pktTotalLen;
    /* 68 */
    uint8 fwAseType;
    /* 69 */
    uint8 fwSrcType;
    /* 70~71 */
    uint8 rsvd[2];
    /* 72~75 */
    uint32 destAddr;
    /* 76~107 */
    uint8 hash[32];
    /* 108~111 */
    uint32 randomNum;
} cvb_StackPayloadRemoteLoad_t;

typedef struct
{
    uint8                        dmac[MAC_ADDR_LEN];
    uint8                        smac[MAC_ADDR_LEN];
    uint8                        ethertype[RTK_ETHERTYPE_LEN];
    EtherAccessHeader1_t         header1;
    EtherAccessHeader2_t         header2;
    cvb_StackPayloadRemoteLoad_t stkHeader;
    /* 112~1387 */
    uint8 fwData[CVB_STACK_REMOTE_LOAD_LENGTH_UNIT];
} cvb_StackEthFwHeader_t;

/****************************************************************/
/* Type Definition                                              */
/****************************************************************/
typedef struct
{
    uint8  available;     /* Indicate the information is available (0x0~0x1) */
    uint8  status;        /* The stacking status of this switch (0x0~0x5) */
    uint8  macAddress[6]; /* The MAC address of switch (0x0~0xFF) */
    uint8  portS0Enable;  /* Port S0 enable for stacking (0x0~0x1) */
    uint8  portS1Enable;  /* Port S1 enable for stacking (0x0~0x1) */
    uint8  unitID;        /* The unit ID of this switch (0x0~0xFF) */
    uint8  masterID;      /* The stacking master unit ID of this switch (0x0~0xFF) */
    uint16 groupID;       /* The group ID of this switch (0x0~0xFFFF) */
    uint16 model;         /* The model of this switch (0x0~0xFFFF) */
    uint8  portToInit;    /* The stacking port that this switch receive "device search"
                                  0xF: It is initiator
                                  0x1: on port S0
                                  0x2: on port S1
                              */
    uint8 portToRelay;    /* The stacking port that this switch relay "device search" to other switch
                                  0xF: It is initiator
                                  0x1: port S0
                                  0x2: port S1
                              */
    uint8 master_locate;  /* The master location in stacking member list (0x0~0x3) */
    uint8 numOnS0;        /* The stackable member on port S0 (0x0~0x3) */
    uint8 numOnS1;        /* The stackable member on port S1 (0x0~0x3) */
    uint8 unitMapId[4];   /* Stacking Unit Map ID array (0x0~0xFF) */
    uint8 unitMapPort[4]; /* Stacking Unit Map port array (0x0~0xF) */
} cvb_StackSwitchInfo_t;

/****************************************************************
 *  Global Variables
 ****************************************************************/
extern uint32 cvb_g_stackingTimeout;

/****************************************************************/
/* Driver Proto Type Definition                                 */
/****************************************************************/

EXTERN_API RtkApiRet cvb_drv_stacking_dump_reg(uint32 unit, uint8 target, cvb_StackSwitchInfo_t *pThisSwitch);
EXTERN_API RtkApiRet cvb_drv_stacking_eth_reg_read(uint32 unit, uint32 address, uint32 *value);
EXTERN_API RtkApiRet cvb_drv_stacking_eth_reg_write(uint32 unit, uint32 address, uint32 value);
EXTERN_API RtkApiRet cvb_drv_stacking_eth_page_read(uint32 unit, uint32 *value);
EXTERN_API RtkApiRet cvb_drv_stacking_eth_page_write(uint32 unit, uint32 value);
#ifdef RTK_INTERNAL_DEV
EXTERN_API RtkApiRet cvb_drv_stacking_reg_read_internal(uint32 unit, uint32 regAddr, uint32 *value);
EXTERN_API RtkApiRet cvb_drv_stacking_reg_write_internal(uint32 unit, uint32 regAddr, uint32 value);
EXTERN_API RtkApiRet cvb_drv_stacking_page_read_internal(uint32 unit, uint32 *value);
EXTERN_API RtkApiRet cvb_drv_stacking_page_write_internal(uint32 unit, uint32 value);
#else
EXTERN_API RtkApiRet cvb_drv_stacking_reg_read(uint32 unit, uint32 regAddr, uint32 *value);
EXTERN_API RtkApiRet cvb_drv_stacking_reg_write(uint32 unit, uint32 regAddr, uint32 value);
EXTERN_API RtkApiRet cvb_drv_stacking_page_read(uint32 unit, uint32 *value);
EXTERN_API RtkApiRet cvb_drv_stacking_page_write(uint32 unit, uint32 value);
#endif /* RTK_INTERNAL_DEV */
EXTERN_API RtkApiRet cvb_drv_stacking_eth_table_get(uint32 unit, cvb_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data);
EXTERN_API RtkApiRet cvb_drv_stacking_eth_table_set(uint32 unit, cvb_table_list_t table, uint32 startIndex, uint32 entryNum, const uint32 *data);
EXTERN_API RtkApiRet cvb_drv_stacking_check_connection_status(uint32 unit);
EXTERN_API RtkApiRet cvb_drv_stacking_load_fw(void *pFile, uint32 *stkReg, uint32 slaveUnit, uint32 slaveLoc);
EXTERN_API RtkApiRet cvb_drv_stacking_load_cfg(void *pFile, uint32 *stkReg, uint32 slaveUnit, uint32 *spiStartAddr, uint32 *spiParaLen, uint32 slaveLoc);
EXTERN_API RtkApiRet cvb_drv_stacking_cfg_data_get(const int8 *fileName, uint32 *stkReg, uint32 slaveUnit, uint32 *spiStartAddr, uint32 *spiParaLen, uint32 *slaveLoc);
EXTERN_API RtkApiRet cvb_drv_stacking_flashless_file_check(const int8 *fwFileName, const int8 *cfgFileName);
EXTERN_API RtkApiRet cvb_drv_stacking_flashless_update_flow_check(uint32 value);

#endif /*#ifndef _RTL906X_ASICDRV_STACK_H_*/
