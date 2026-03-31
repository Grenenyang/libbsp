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

#ifndef DVA_ASICDRV_FLASH_H
#define DVA_ASICDRV_FLASH_H

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api_spi_flash.h"

#define DVA_DEBUG_FLASH (0)
/********************************************************************************
 * Dummy Register
 ********************************************************************************/
#define DVA_SPIC_DMY_REG_0 (DVA_INTERNAL_CPU_VARIABLE50) /* record flash zone selection and dummy cycle */
#define DVA_SPIC_DMY_REG_1 (DVA_INTERNAL_CPU_VARIABLE23) /* !!DEPRECATED!! Bit0 of Byte3 indicates whether occupied by internal CPU. Bit1 of Byte3 indicates whether occupied by external CPU.*/
#define DVA_SPIC_DMY_REG_2 (DVA_CPU_SPECIAL2)            /* Bit[0:1] indicates flash layout. Bit2 indicates length of each configuration */
                                                         /* Register name of address 0x4A1F0008 is DVA_CPU_SPECIAL2 */
#if 0                                                    // for debug
#define DVA_SPIC_DMY_REG_2 DVA_I2C_DMY0
#endif

/********************************************************************************
 * Flash Controller Register
 ********************************************************************************/
/*flash control register*/
#define DVA_FLASH_CTRLR0      (0x00u)
#define DVA_FLASH_RX_NDF      (0x04u)
#define DVA_FLASH_SSIENR      (0x08u)
#define DVA_FLASH_SER         (0x10u)
#define DVA_FLASH_BAUDR       (0x14u)
#define DVA_FLASH_TXFLR       (0x20u)
#define DVA_FLASH_RXFLR       (0x24u)
#define DVA_FLASH_SR          (0x28u)
#define DVA_FLASH_ISR         (0x30u)
#define DVA_FLASH_ICR         (0x48u)
#define DVA_FLASH_DR          (0x60u)
#define DVA_FLASH_ADDR_CTRLR2 (0x110u)
#define DVA_FLASH_FBAUDR      (0x114u)
#define DVA_FLASH_USER_LENGTH (0x118u)
#define DVA_FLASH_AUTO_LENGTH (0x11Cu)
#define DVA_FLASH_VALID_CMD   (0x120u)
#define DVA_FLASH_DUM_BYTE    (0x12Cu)
#define DVA_FLASH_FLUSH_FIFO  (0x128u)
#define DVA_FLASH_TX_NDF      (0x130u)

#define DVA_FLASH_USER_LENGTH_RD_DUMMY_LENGTH_LSP (0u)
#define DVA_FLASH_USER_LENGTH_RD_DUMMY_LENGTH_LEN (11u)

#define DVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP (0u)
#define DVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN (11u)

/*flash command*/
#define DVA_FLASH_READ_COM      (0x03u)
#define DVA_FLASH_FAST_READ_COM (0x0Bu)
#define DVA_FLASH_DREAD_COM     (0x3Bu)
#define DVA_FLASH_2READ_COM     (0xBBu)
#define DVA_FLASH_QREAD_COM     (0x6Bu)
#define DVA_FLASH_4READ_COM     (0xEBu)
#define DVA_FLASH_PP_COM        (0x02u)
#define DVA_FLASH_QPP_COM       (0x32u)
#define DVA_FLASH_4PP_COM       (0x38u)

#define DVA_FLASH_SE_COM    (0x20u) /*erase 4K block command*/
#define DVA_FLASH_BE32K_COM (0x52u) /*erase 32K block command*/
#define DVA_FLASH_BE_COM    (0xD8u) /*erase 64K block command*/
#define DVA_FLASH_CE_COM    (0xC7u) /*erase all*/

#define DVA_FLASH_WREN_COM          (0x06u)
#define DVA_FLASH_WRDI_COM          (0x04u)
#define DVA_FLASH_WRSR_COM          (0x01u)
#define DVA_FLASH_RDID_COM          (0x9Fu)
#define DVA_FLASH_RDSR_COM          (0x05u)
#define DVA_FLASH_RDCR_COM          (0x35u)
#define DVA_FLASH_RDCR_COM_FOR_MXIC (0x15u)
/********************************************************************************
 * Binary FILE
 ********************************************************************************/
/*6665 START !!DEPRECATED!!*/
#define DVA_ADDR_BIN_FW                  (0x000000u)
#define DVA_ADDR_FLASH_PATCH_POINT       (0x003F80u)
#define DVA_ADDR_BIN_SETTING             (0x040000u)
#define DVA_ADDR_BIN_CALI                (0x040010u)
#define DVA_ADDR_BIN_HEADER              (0x040020u)
#define DVA_ADDR_BIN_RESERVED            (0x040040u)
#define DVA_ADDR_BIN_CONFIG              (0x040060u)
#define DVA_ADDR_BIN_FLASH_PROTECT       (0x00040036u)
#define DVA_ADDR_BIN_FLASH_PROTECT_ZoneB (0x000A0036u)
/* For 128KB configuration */
#define DVA_ADDR_BIN_CONFIG128KB_FOR_STACKING_0 0x060060u
#define DVA_ADDR_BIN_CONFIG128KB_FOR_STACKING_1 0x080060u
#define DVA_ADDR_BIN_CONFIG128KB_FOR_STACKING_2 0x0A0060u
/* For 256KB configuration */
#define DVA_ADDR_BIN_CONFIG256KB_FOR_STACKING_0 0x080060u
#define DVA_ADDR_BIN_CONFIG256KB_FOR_STACKING_1 0x0C0060u
#define DVA_ADDR_BIN_CONFIG256KB_FOR_STACKING_2 0x100060u
/* For 512KB configuration */
#define DVA_ADDR_BIN_CONFIG512KB_FOR_STACKING_0 0x0C0060u
#define DVA_ADDR_BIN_CONFIG512KB_FOR_STACKING_1 0x140060u
#define DVA_ADDR_BIN_CONFIG512KB_FOR_STACKING_2 0x1C0060u
/*6665 END*/

/********************************************************************************
 * Flash Memory
 ********************************************************************************/
/*6665 START !!DEPRECATED!!*/
#define DVA_ADDR_FLASH_FW_1_FOR_CONFIG128KB (0x060000u)
#define DVA_ADDR_FLASH_FW_1_FOR_CONFIG256KB (0x080000u)
#define DVA_ADDR_FLASH_FW_1_FOR_CONFIG512KB (0x0C0000u)
/* For 128KB configuration */
#define DVA_ADDR_FLASH_STACKING_OFST_FOR_CONFIG128KB (0x100060u)
/* For 256KB configuration */
#define DVA_ADDR_FLASH_STACKING_OFST_FOR_CONFIG256KB (0x140060u)
/* For 512KB configuration */
#define DVA_ADDR_FLASH_STACKING_OFST_FOR_CONFIG512KB (0x200060u)
/*6665 END*/

/*6760 Flash*/
#define DVA_FLASH_HEADE_IDENTIFY         (0x8899u)
#define DVA_FLASH_FW_LENGTH_MAX          (0x40000u)
#define DVA_FLASH_CONFIG128KB_LENGTH_MAX (0x20000u)
#define DVA_FLASH_CONFIG256KB_LENGTH_MAX (0x40000u)
#define DVA_FLASH_CONFIG512KB_LENGTH_MAX (0x80000u)
#define DVA_FLASH_ZONE_SIZE_512KB        (0x00080000u)
#define DVA_FLASH_ZONE_SIZE_1MB          (0x00100000u)
#define DVA_FLASH_ZONE_SIZE_2MB          (0x00200000u)
#define DVA_FLASH_ZONE_SIZE_4MB          (0x00400000u)
#define DVA_FLASH_ZONE_SIZE_8MB          (0x00800000u)

/* Flash and Address Offset */
#define DVA_FLASH_SETTING_OFFSET       (0x00000000u)
#define DVA_FLASH_CALIB_PATTERN_OFFSET (0x00000010u)
#define DVA_FLASH_HEADER_OFFSET        (0x00000020u)
#define DVA_FLASH_RESERVE_AREA_OFFSET  (0x00000060u)
#define DVA_FLASH_AES_CTR_IV_OFFSET    (0x000000100u)

/*header 0x5c = 0x20+60bytes*/
#define DVA_FLASH_HEADER_LOCK_OFFSET (0x0000005cu)

/*Force update*/
#define DVA_FLASH_FORCE_UPDATE_ALL (0x1u)
#define DVA_FLASH_UPDATE_PARTIAL   (0x0u)

/*For CRC*/
#define DVA_DATALENMAX (4096U)

#define DVA_FLASH_64K (0x10000u)
#define DVA_FLASH_4K  (0x1000u)

/*Content Ready bit*/
#define DVA_FLASH_STATUS_READY     (0x0u)
#define DVA_FLASH_STATUS_NOT_READY (0x1u)  // need program in image

/*Flash size*/
#define DVA_FLASH_SIZE_TOATL_1M   (7u)
#define DVA_FLASH_SIZE_TOATL_2M   (6u)
#define DVA_FLASH_SIZE_TOATL_4M   (5u)
#define DVA_FLASH_SIZE_TOATL_8M   (4u)
#define DVA_FLASH_SIZE_TOATL_16M  (3u)
#define DVA_FLASH_SIZE_TOATL_RES1 (2u)
#define DVA_FLASH_SIZE_TOATL_RES2 (1u)
#define DVA_FLASH_SIZE_TOATL_RES3 (0u)

#define DVA_CALIB_RESULT_COMPESANTE_NO_NEED         (0u)
#define DVA_CALIB_RESULT_COMPESANTE_SUCCESS         (1u)
#define DVA_CALIB_RESULT_COMPESANTE_FAIL            (2u)
#define DVA_CALIB_RESULT_COMPESANTE_FOR_1_CANDIDATE (3u)

/* Register lock status */
#define DVA_REGISTER_LOCK_MASK         (0x00000003u)
#define DVA_REGISTER_LOCK_FREE         (0u)
#define DVA_REGISTER_LOCK_CPU_EXTERNAL (1u)
#define DVA_REGISTER_LOCK_CPU_INTERNAL (2u)

/*SYS CLK*/
#define DVA_SYS_CLK_REG_300MHZ     (0x0u)
#define DVA_SYS_CLK_REG_400MHZ_AFE (0x1u)
#define DVA_SYS_CLK_REG_333MHZ     (0x2u)
#define DVA_SYS_CLK_REG_400MHZ_DIV (0x3u)
#define DVA_SYS_CLK_REG_500MHZ_1   (0x4u)
#define DVA_SYS_CLK_REG_500MHZ_2   (0x5u)
#define DVA_SYS_CLK_REG_500MHZ_3   (0x6u)
#define DVA_SYS_CLK_REG_500MHZ_4   (0x7u)

#define DVA_SYS_CLK_300MHZ (300u)
#define DVA_SYS_CLK_333MHZ (333u)
#define DVA_SYS_CLK_400MHZ (400u)
#define DVA_SYS_CLK_500MHZ (500u)

/*Default unlock*/
#define DVA_FLASH_ENABLE_LOCK      (0x0u)
#define DVA_FLASH_DISABLE_LOCK     (0x1u)
#define DVA_FLASH_SUPPORT_LOCK     (0x0u)
#define DVA_FLASH_NOT_SUPPORT_LOCK (0x1u)

/*Default Protect all*/
#define DVA_FLASH_LOCK_BIT_PROTECT_HIGH (0x1u)
#define DVA_FLASH_LOCK_BIT_PROTECT_LOW  (0x0u)
#define DVA_FLASH_LOCK_BIT_MASK_DEFAULT (0xFFu)

/*FLASH CHIP ID*/
#define DVA_FLASH_VENDOR_MXIC       (0xC2u)
#define DVA_FLASH_VENDOR_GIGADEVICE (0xC8u)
#define DVA_FLASH_VENDOR_ISSI       (0x9Du)
#define DVA_FLASH_VENDOR_WINBOND    (0xEFu)

/*IC part number*/
#define DVA_IC_PART_RTL9072DA  (0x0u)
#define DVA_IC_PART_RTL9070DC  (0x1u)
#define DVA_IC_PART_RTL9072DC  (0x2u)
#define DVA_IC_PART_RTL9068DZB (0x3u)

#define DVA_IC_VERSION_VA (0x2)

/* Flash FIFO size*/
#define DVA_FIFO_NUM              (256u)
#define DVA_FLASH_PAGE_NUM        (256u)
#define DVA_SPIC_SYSDELAY_DEFAULT (0x3u)
#define DVA_PAGE_SWITCH_REG       (0xFFFF01u)
#define DVA_SPIC_PAGE             (0x07u)
#define DVA_CPU_PAGE              (0x42u)
#define DVA_SW_PAGE               (0x4Au)

#define DVA_SPI_FLASH_WIP (0x1u)
#define DVA_SPI_FLASH_WEL (0x2u)
#define DVA_SPIC_BUSY     (0x1u)
#define DVA_SPIC_EN       (0x1u)

#define DVA_SPIC_HIGH_SPEED_TH (25u)

/*6665 Start !!DEPRECATED!!*/
/* spic_layout_type_t */
#define DVA_FLASH_CONFIGURATION_RESERVED (0u)
#define DVA_FLASH_CONFIGURATION_512KB    (1u)
#define DVA_FLASH_CONFIGURATION_256KB    (2u)
#define DVA_FLASH_CONFIGURATION_128KB    (3u)
#define DVA_FLASH_CONFIGURATION_END      (4u)
/*6665 End*/

/* spic_addr_type_t */
#define DVA_SPIC_ADDR_MULTI  (0u)
#define DVA_SPIC_ADDR_SINGLE (1u)

/* spic_QE_loc_t */
#define DVA_SPIC_QE_SR1 (0u)
#define DVA_SPIC_QE_SR2 (1u)

/* flash_config_location_t */
#define DVA_FLASH_ZONE_A (0u)
#define DVA_FLASH_ZONE_B (1u)

/* spic_erase_cmd_type_t */
#define DVA_SPIC_CHIP_ERASE   (0u)
#define DVA_SPIC_SECTOR_ERASE (1u)
#define DVA_SPIC_BLK64K_ERASE (2u)
#define DVA_SPIC_ERASE_END    (3u)

#define DVA_FLASH_ZONE_SIZE_NUMBER (5u)

#define DVA_SPI_UPDATE_ETH   (1u)
#define DVA_SPI_UPDATE_REGIF (0u)

#define DVA_SPI_CALIBRTION       (0u)
#define DVA_SPI_CALIBRATION_FREE (1u)

#define DVA_WAIT_FLASH_STATUS_TIME (240000000u)

#define DVA_NEED_REVERT     (1u)
#define DVA_NOT_NEED_REVERT (0u)

typedef struct
{
    uint8  updateMode; /*1:update all, 0:partial*/
    uint32 updateMask;
    uint32 fwLen;
    uint32 configLen;
    uint32 stk0Len;   /* option */
    uint32 stk1Len;   /* option */
    uint32 stk2Len;   /* option */
    uint32 macsecLen; /* option */
    uint32 fwAddr;
    uint32 configAddr;
    uint32 stk0Addr;   /* option */
    uint32 stk1Addr;   /* option */
    uint32 stk2Addr;   /* option */
    uint32 macsecAddr; /* option */
    uint32 fwAddrOri;
    uint32 configAddrOri;
    uint32 stk0AddrOri;   /* option */
    uint32 stk1AddrOri;   /* option */
    uint32 stk2AddrOri;   /* option */
    uint32 macsecAddrOri; /* option */
    uint32 fwLenOri;
    uint32 configLenOri;
    uint32 stk0LenOri;   /* option */
    uint32 stk1LenOri;   /* option */
    uint32 stk2LenOri;   /* option */
    uint32 macsecLenOri; /* option */
} dva_SpiFlashUpdate_t;

typedef struct
{
    uint32 fpOffset;
    uint32 addr;
    uint32 programLen;
    uint32 dividBase;
} dva_SpiWriteSection_t;

extern uint8  dva_g_spiInit;
extern uint16 dva_g_spiSysDelay;
extern uint8  dva_g_spiCh;

extern uint32 dva_g_flashId;
extern uint8  dva_g_spicProgramCmd;
extern uint8  dva_g_spicReadCmd;
extern uint8  dva_g_spicRdcrCmd;
extern uint8  dva_g_spicModeBitValue;
extern uint8  dva_g_spicProgramAddrChannel;
extern uint8  dva_g_spicReadAddrChannel;
extern uint8  dva_g_spicCmdDelay;
extern uint8  dva_g_spicQeLocation;
extern uint8  dva_g_spicClk;
extern uint32 dva_g_spicBaudr;

extern uint8  dva_g_spiChForUpdate;
extern uint8  dva_g_spicProgramCmdForUpdate;
extern uint8  dva_g_spicReadCmdForUpdate;
extern uint8  dva_g_spicModeBitValueForUpdate;
extern uint8  dva_g_spicProgramAddrChannelForUpdate;
extern uint8  dva_g_spicReadAddrChannelForUpdate;
extern uint8  dva_g_spicCmdDelayForUpdate;
extern uint8  dva_g_spicClkForUpdate;
extern uint32 dva_g_spicBaudrForUpdate;

///////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_API RtkApiRet dva_drv_spi_flash_wren(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_write_sr(uint32 unit, uint8 statusData, uint8 configData, uint8 mode);
EXTERN_API RtkApiRet dva_drv_spi_flash_read_sr(uint32 unit, uint8 *flashStatus);
EXTERN_API RtkApiRet dva_drv_spi_flash_read_cr(uint32 unit, uint8 *flashConfig);
EXTERN_API RtkApiRet dva_drv_spi_flash_read_id(uint32 unit, uint32 *flashId);
EXTERN_API RtkApiRet dva_drv_spi_flash_write(uint32 unit, uint32 startAddr, const uint8 *buf, uint32 writeLen);
EXTERN_API RtkApiRet dva_drv_spi_flash_erase(uint32 unit, uint32 eraseAddr, uint8 flag);
EXTERN_API RtkApiRet dva_drv_spi_flash_read(uint32 unit, uint32 startAddr, uint8 *buf, uint32 readLen);
EXTERN_API RtkApiRet dva_drv_spi_flash_init(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_calibration(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_get_pattern(uint32 unit, SpicPattern_t *spicPattern);
EXTERN_API RtkApiRet dva_drv_spi_flash_get_spi_speed(uint32 unit, uint8 *speed);
EXTERN_API RtkApiRet dva_drv_spi_flash_sel_and_set_spi_for_update(uint32 unit, uint8 ch, uint8 speed);
EXTERN_API RtkApiRet dva_drv_spi_flash_sel_spi_speed_for_update(uint32 unit, uint8 speed);
EXTERN_API RtkApiRet dva_drv_spi_flash_sel_spi_channel_for_update(uint32 unit, uint8 ch);
EXTERN_API RtkApiRet dva_drv_spi_flash_sel_addr_channel_for_update(uint32 unit, uint8 ch);
EXTERN_API RtkApiRet dva_drv_spi_flash_set_spi_config_for_update(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_update(uint32 unit, const char *fileName);
EXTERN_API RtkApiRet dva_drv_spi_flash_update_for_ethernet(const char *fileName);
EXTERN_API RtkApiRet dva_drv_spi_flash_set_spi_channel_for_update(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_spic_lock(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_spic_unlock(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_init_get(uint32 unit, uint32 *outSpiCH, uint32 *outSpicClk);
EXTERN_API RtkApiRet dva_drv_spi_flash_chip_unlock(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_chip_lock(uint32 unit);
EXTERN_API RtkApiRet dva_drv_spi_flash_crc_16_calculate(uint32 unit, const uint8 *data, uint16 len, uint16 *crc16Val);
EXTERN_API RtkApiRet dva_drv_spi_flash_check_spic_busy(uint32 unit, uint32 minUs, uint32 maxUs);
EXTERN_API RtkApiRet dva_drv_spi_flash_check_spic_en_down(uint32 unit, uint32 minUs, uint32 maxUs);
EXTERN_API RtkApiRet dva_drv_spi_revert_dphy_spd_delay(uint32 unit);

///////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /*_RTL906X_ASICDRV_FLASH_H_*/
