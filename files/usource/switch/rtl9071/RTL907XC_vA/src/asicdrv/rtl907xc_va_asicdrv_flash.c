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

#include "rtkas_types.h"
#include "rtkas_debug.h"
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_asicdrv_flash.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_api.h"
#include "rtl907xc_va_api_otp.h"
#include "rtkas_endian.h"
#include "rtkas_system_porting.h"
#include "rtl907xc_va_asicdrv_otp.h"
#include "rtl907xc_va_asicdrv.h"

uint16 cva_g_spiSysDelay = (uint16)CVA_SPIC_SYSDELAY_DEFAULT;
uint8  cva_g_spiInit     = (uint8)0u; /* 1 = Disable the operation to read flash status */

uint8  cva_g_spiCh                  = (uint8)RTK_SPI_FLASH_SINGLE_CHANNEL;
uint32 cva_g_spiTimeout             = (uint32)152000000u;
uint32 cva_g_flashId                = (uint32)0u;
uint8  cva_g_spicProgramCmd         = (uint8)CVA_FLASH_PP_COM;
uint8  cva_g_spicReadCmd            = (uint8)CVA_FLASH_READ_COM;
uint8  cva_g_spicRdcrCmd            = (uint8)CVA_FLASH_RDCR_COM;
uint8  cva_g_spicModeBitValue       = (uint8)0xFFu;
uint8  cva_g_spicProgramAddrChannel = (uint8)CVA_SPIC_ADDR_SINGLE;
uint8  cva_g_spicReadAddrChannel    = (uint8)CVA_SPIC_ADDR_SINGLE;
uint8  cva_g_spicCmdDelay           = (uint8)0u;
uint8  cva_g_spicQeLocation         = (uint8)CVA_SPIC_QE_SR2;
uint8  cva_g_spicClk                = (uint8)12u;
uint32 cva_g_spicBaudr              = (uint32)8u;

/* pre-config for update*/
uint8  cva_g_spiChForUpdate                  = (uint8)RTK_SPI_FLASH_SINGLE_CHANNEL;
uint8  cva_g_spicProgramCmdForUpdate         = (uint8)CVA_FLASH_PP_COM;
uint8  cva_g_spicReadCmdForUpdate            = (uint8)CVA_FLASH_READ_COM;
uint8  cva_g_spicModeBitValueForUpdate       = (uint8)0xFFu;
uint8  cva_g_spicProgramAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
uint8  cva_g_spicReadAddrChannelForUpdate    = (uint8)CVA_SPIC_ADDR_SINGLE;
uint8  cva_g_spicCmdDelayForUpdate           = (uint8)0u;
uint8  cva_g_spicClkForUpdate                = (uint8)12u;
uint32 cva_g_spicBaudrForUpdate              = (uint32)8u;

/*flash chip lock config*/
static uint8 cva_g_protectMode   = (uint8)CVA_FLASH_LOCK_BIT_PROTECT_HIGH;
static uint8 cva_g_lockBit       = (uint8)CVA_FLASH_LOCK_BIT_MASK_DEFAULT;
static uint8 cva_g_lockMode      = (uint8)CVA_FLASH_DISABLE_LOCK;
static uint8 cva_g_otpLockEnable = (uint8)CVA_FLASH_NOT_SUPPORT_LOCK;

DYNAMIC_SCOPE uint8 cva_g_lockForUpdate = (uint8)0u; /* 1 = for update, don't lock/unlock at every write/erase function*/

static RtkApiRet cva_drv_spi_flash_write_sr_prepare(uint32 unit);

static RtkApiRet cva_drv_spi_flash_check_spic_busy(uint32 unit);
static RtkApiRet cva_drv_spi_flash_wait_rdy(uint32 unit);
static RtkApiRet cva_drv_spi_flash_config_flash_write(uint32 unit);
static RtkApiRet cva_drv_spi_flash_internal_write_prepare(uint32 unit, uint32 startAddr);
static RtkApiRet cva_drv_spi_flash_internal_write_to_dr_left(uint32 unit, const uint8 *buf, uint32 writeLen);
static RtkApiRet cva_drv_spi_flash_internal_write(uint32 unit, uint32 startAddr, const uint8 *buf, uint32 writeLen);
static RtkApiRet cva_drv_spi_flash_config_flash_read_dual(uint32 unit);
static RtkApiRet cva_drv_spi_flash_config_flash_read_quad(uint32 unit);
static RtkApiRet cva_drv_spi_flash_config_flash_read_single(uint32 unit);
static RtkApiRet cva_drv_spi_flash_internal_read_prepare(uint32 unit, uint32 startAddr, uint32 readLen);
static RtkApiRet cva_drv_spi_flash_config_flash_read(uint32 unit);
static RtkApiRet cva_drv_spi_flash_internal_read(uint32 unit, uint32 startAddr, uint8 *buf, uint32 readLen);
static RtkApiRet cva_drv_spi_flash_internal_calibration(uint32 unit);
static RtkApiRet cva_drv_spi_flash_erase_precheck_flag(uint32 unit, uint32 eraseAddr, uint8 flag);
static RtkApiRet cva_drv_spi_flash_erase_prepare(uint32 unit, uint32 eraseAddr, uint8 flag);
static RtkApiRet cva_drv_spi_flash_internal_calibration_retry(uint32 unit);
static RtkApiRet cva_drv_spi_flash_sysclk_set_reg(uint32 unit, uint8 flag);
static RtkApiRet cva_drv_spi_flash_sysclk_set(uint32 unit, uint8 flag);
static RtkApiRet cva_drv_spi_flash_baudrate_set(uint32 unit, uint32 baudrate);
static RtkApiRet cva_drv_spi_flash_set_spi_speed_for_update_baud(uint32 unit);
static RtkApiRet cva_drv_spi_flash_set_spi_speed_for_update(uint32 unit);
static RtkApiRet cva_drv_spi_flash_write_section(uint32 unit, void *fp, cva_SpiWriteSection_t writeSecParam, uint32 *progress, uint32 *dividCurr);
static RtkApiRet cva_drv_spi_flash_speed_revert_baud(uint32 unit, uint32 baudrate, uint32 fbaudrate);
static RtkApiRet cva_drv_spi_flash_set_qe(uint32 unit);
static RtkApiRet cva_drv_spi_flash_init_get_op_and_layout(uint32 unit, uint32 *val, uint8 *op, uint32 *flashLayout);
static RtkApiRet cva_drv_spi_flash_init_get_dmycycle_and_zonesel(uint32 unit, uint32 val, uint8 op, uint8 *zoneSel);
static RtkApiRet cva_drv_spi_flash_init_back_to_1ch(uint32 unit);
static RtkApiRet cva_drv_spi_flash_init_get_ch_baud_clk(uint32 unit, uint8 *flashBuf);
static RtkApiRet cva_drv_spi_flash_init_get_rdcmd_pgcmd(uint32 unit, uint8 *flashBuf);
static RtkApiRet cva_drv_spi_flash_init_get_setting(uint32 unit, uint8 *flashBuf);
static RtkApiRet cva_drv_spi_flash_cal_time_compensation_success(uint32 unit);
static RtkApiRet cva_drv_spi_flash_cal_time_compensation_fail(uint32 unit);
static RtkApiRet cva_drv_spi_flash_internal_calibration_dmy_candidate(uint32 unit, uint8 dmyCandidateCnt);
static RtkApiRet cva_drv_spi_flash_internal_calibration_set_sysdelay(uint32 unit, uint8 dmyCandidateCnt, uint8 *dmyCandidate);
static RtkApiRet cva_drv_spi_flash_cal_sysdelay_find(uint32 unit, uint8 *candidateArr, uint8 *candidateCnt, const uint32 patternAddress);
static RtkApiRet cva_drv_spi_flash_cal_time_compensation(uint32 unit, uint8 *candidateArr, uint8 *candidateCnt, const uint32 patternAddress);
static RtkApiRet cva_drv_spi_flash_write_setting_header(uint32 unit, const uint8 sel, const uint8 prebuf[], uint8 flashLayout);
static RtkApiRet cva_drv_spi_flash_update_check_zone_a(uint32 unit, uint8 *prebuf, uint32 updateMask, uint8 ethMode);
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_fw(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout);
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_macsec(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout);
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config_stk2(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout);
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config_stk1(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout);
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config_stk(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout);
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout);
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_set_length(uint32 unit, uint8 *prebuf, uint8 *readbuf, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_program(uint32 unit, void *fp, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam, const uint32 dividBase);
static RtkApiRet cva_drv_spi_flash_update_erase_sub_function(uint32 unit, uint32 len, uint32 addr, uint8 eraseFlag);
static RtkApiRet cva_drv_spi_flash_update_erase_fw_config(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_stk1(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_stk2(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_macsec(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_stk(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_get_update_block_for_ethernet(uint32 unit, uint8 *prebuf, cva_SpiFlashUpdate_t *updateParam);
static RtkApiRet cva_drv_spi_flash_erase_for_ethernet(uint32 unit, uint32 eraseAddr, uint8 flag);
static RtkApiRet cva_drv_spi_flash_update_erase_sub_function_for_ethernet(uint32 unit, uint32 len, uint32 addr, uint8 eraseFlag);
static RtkApiRet cva_drv_spi_flash_update_erase_fw_config_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_stk2_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_stk1_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_stk_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_macsec_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_update_erase_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_write_section_for_ethernet(uint32 unit, void *fp, cva_SpiWriteSection_t writeSecParam);
static RtkApiRet cva_drv_spi_flash_update_program_for_ethernet(uint32 unit, void *fp, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
static RtkApiRet cva_drv_spi_flash_get_lock_mode_bit_set(uint32 unit);
static RtkApiRet cva_drv_spi_flash_flush_fifo_and_clr_isr(uint32 unit);

DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_fill_calibration_header(uint32 unit, uint8 *buf);
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_check_ver(uint32 unit, void *fp);
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_check_valid(uint32 unit, uint8 updateMode, uint32 fwLen, uint32 configLen, const uint8 prebuf[]);
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_get_update_block(uint32 unit, uint8 *prebuf, cva_SpiFlashUpdate_t *updateParam, uint32 *dividBase);
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_get_update_partial_check(uint32 unit, uint8 *prebuf, uint8 sel, cva_SpiFlashUpdate_t *updateParam, uint8 flashLayout, uint8 ethMode);
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_erase(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam);
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_get_lock_otp_enable(uint32 unit);

static RtkApiRet cva_drv_spi_flash_flush_fifo_and_clr_isr(uint32 unit)
{
    uint32    regVal = 0u;
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_ICR, &regVal));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_FLUSH_FIFO, 0x1u));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the lock mechanism from OTP.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write().
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_get_lock_otp_enable(uint32 unit)
{
    RtkApiRet retVal        = RT_ERR_OK;
    RtkApiRet retTmp        = RT_ERR_OK;
    uint8     otpLockEnable = (uint8)0u;
    do
    {
        /* change page 0x4A*/
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SW_PAGE));
        /*get otp flash lock enable bit 0xb06[7]*/
        retTmp = cva_otp_read(unit, (uint32)0xB06u, &otpLockEnable, (uint32)1u);
        if(RT_ERR_OK == retTmp)
        {
            cva_g_otpLockEnable = (otpLockEnable >> 7u) & (uint8)0x1u;
        }
        /* change SPIC page*/
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the lock mode and lock bit.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_read().
 */
static RtkApiRet cva_drv_spi_flash_get_lock_mode_bit_set(uint32 unit)
{
    RtkApiRet    retVal                                  = RT_ERR_OK;
    uint32       startAddr                               = (uint32)0u;
    uint32       i                                       = (uint32)0u;
    uint16       crcVal                                  = (uint16)0u;
    uint8        rBufBin[0x60]                           = {(uint8)0u};
    uint8        lockBuf[2]                              = {(uint8)0xff, (uint8)0xff};
    const uint32 fwSizeArray[CVA_FLASH_ZONE_SIZE_NUMBER] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                                            (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_8MB};
    do
    {
        /*otp lock enable*/
        if(CVA_FLASH_SUPPORT_LOCK == cva_g_otpLockEnable)
        {
            cva_g_lockBit     = (uint8)CVA_FLASH_LOCK_BIT_MASK_DEFAULT;
            cva_g_lockMode    = (uint8)CVA_FLASH_DISABLE_LOCK;
            cva_g_protectMode = (uint8)CVA_FLASH_LOCK_BIT_PROTECT_HIGH;
            startAddr         = (uint32)CVA_FLASH_SETTING_OFFSET;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, startAddr, rBufBin, (uint32)0x60u));
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_crc_16_calculate(unit, &rBufBin[0x20], 62, &crcVal));
            if(crcVal == (uint16)(((uint16)rBufBin[0x5F] << 8) | ((uint16)rBufBin[0x5E])))
            {
                startAddr = (uint32)CVA_FLASH_HEADER_LOCK_OFFSET;
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, startAddr, lockBuf, (uint32)2u));
                if((uint8)0xffu != lockBuf[1])
                {
                    cva_g_lockBit     = lockBuf[1];
                    cva_g_lockMode    = lockBuf[0] & 0x01u;
                    cva_g_protectMode = (lockBuf[0] >> 1u) & 0x01u;
                    continue;
                }
                for(i = (uint32)0u; i < (uint32)CVA_FLASH_ZONE_SIZE_NUMBER; i++)
                {
                    crcVal    = 0u;
                    startAddr = ((uint32)CVA_FLASH_ZONE_B * fwSizeArray[i]) + (uint32)CVA_FLASH_SETTING_OFFSET;
                    CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, startAddr, rBufBin, (uint32)0x60u));
                    CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_crc_16_calculate(unit, &rBufBin[0x20], 62, &crcVal));
                    if(crcVal == (uint16)(((uint16)rBufBin[0x5F] << 8) | ((uint16)rBufBin[0x5E])))
                    {
                        startAddr = ((uint32)CVA_FLASH_ZONE_B * fwSizeArray[i]) + (uint32)CVA_FLASH_HEADER_LOCK_OFFSET;
                        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, startAddr, lockBuf, (uint32)2));
                        if((uint8)0xffu != lockBuf[1])
                        {
                            cva_g_lockBit     = lockBuf[1];
                            cva_g_lockMode    = lockBuf[0] & 0x01u;
                            cva_g_protectMode = (lockBuf[0] >> 1u) & 0x01u;
                            break;
                        }
                    }
                }
            }
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Unlock the flash chip
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_spi_flash_wait_rdy(),cva_drv_spi_flash_read_sr(),cva_drv_spi_flash_get_lock_mode_bit_set(),
 *                                cva_drv_spi_flash_write_sr(),
 */
RtkApiRet cva_drv_spi_flash_chip_unlock(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     flashStatus;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_wait_rdy(unit));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
#if 0
        DEBUG_INFO(DBG_MEMCTRL, "\nThe status word before trigger write status command: 0x%x", flashStatus);
#endif
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_lock_mode_bit_set(unit));
        if(CVA_FLASH_LOCK_BIT_MASK_DEFAULT == cva_g_lockBit)
        {
            /*No lock information*/
            continue;
        }
        if(CVA_FLASH_LOCK_BIT_PROTECT_HIGH == cva_g_protectMode)
        {
            if((uint8)0u != (flashStatus & (uint8)cva_g_lockBit))
            {
                flashStatus = flashStatus & ~(cva_g_lockBit);
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(unit, flashStatus, (uint8)0u, (uint8)0u));
#if 0
                DEBUG_INFO(DBG_MEMCTRL, "Detecting protection....");
#endif
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
#if 0

                DEBUG_INFO(DBG_MEMCTRL, "\nThe status word after trigger write status command: 0x%x", flashStatus);
#endif
                if((uint8)0u != (flashStatus & (uint8)cva_g_lockBit))
                {
#if 0

                    DEBUG_INFO(DBG_MEMCTRL, "SPI Protection Cleaning Failed");
#endif
                    retVal = -RT_ERR_FAILED;
                }
            }
        }
        else
        {
            if(cva_g_lockBit != (flashStatus & (uint8)cva_g_lockBit))
            {
                flashStatus = flashStatus | cva_g_lockBit;
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(unit, flashStatus, (uint8)0u, (uint8)0u));
#if 0

                DEBUG_INFO(DBG_MEMCTRL, "Detecting protection....");
#endif
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
#if 0

                DEBUG_INFO(DBG_MEMCTRL, "\nThe status word after trigger write status command: 0x%x", flashStatus);
#endif
                if(cva_g_lockBit != (flashStatus & (uint8)cva_g_lockBit))
                {
#if 0

                    DEBUG_INFO(DBG_MEMCTRL, "SPI Protection Cleaning Failed");
#endif
                    retVal = -RT_ERR_FAILED;
                }
            }
        }
#if 0

        DEBUG_INFO(DBG_MEMCTRL, "SPI hasn't been protected");
#endif
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Lock the flash chip
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_spi_flash_wait_rdy(),cva_drv_spi_flash_read_sr(),cva_drv_spi_flash_get_lock_mode_bit_set(),
 *                                cva_drv_spi_flash_write_sr(),
 */
RtkApiRet cva_drv_spi_flash_chip_lock(uint32 unit)
{
    RtkApiRet retVal = (int32)RT_ERR_OK;
    uint8     flashStatus;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_wait_rdy(unit));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
#if 0
        DEBUG_INFO(DBG_MEMCTRL, "\nThe status word before trigger write status command: 0x%x", flashStatus);
#endif
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_lock_mode_bit_set(unit));
        if(CVA_FLASH_ENABLE_LOCK == cva_g_lockMode)
        {
            if(CVA_FLASH_LOCK_BIT_PROTECT_HIGH == cva_g_protectMode)
            {
                if(cva_g_lockBit != (flashStatus & (uint8)cva_g_lockBit))
                {
                    flashStatus = flashStatus | cva_g_lockBit;
                    CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(unit, flashStatus, (uint8)0u, (uint8)0u));
#if 0

                    DEBUG_INFO(DBG_MEMCTRL, "Detecting protection....");
#endif
                    CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
#if 0

                    DEBUG_INFO(DBG_MEMCTRL, "\nThe status word after trigger write status command: 0x%x", flashStatus);
#endif
                    if(cva_g_lockBit != (flashStatus & (uint8)cva_g_lockBit))
                    {
#if 0

                        DEBUG_INFO(DBG_MEMCTRL, "SPI Protection Failed");
#endif
                        retVal = -RT_ERR_FAILED;
                    }
                }
            }
            else
            {
                if((uint8)0u != (flashStatus & (uint8)cva_g_lockBit))
                {

                    flashStatus = flashStatus & ~(cva_g_lockBit);
                    CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(unit, flashStatus, (uint8)0u, (uint8)0u));
#if 0

                    DEBUG_INFO(DBG_MEMCTRL, "Detecting protection....");
#endif
                    CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
#if 0

                    DEBUG_INFO(DBG_MEMCTRL, "\nThe status word after trigger write status command: 0x%x", flashStatus);
#endif
                    if((uint8)0u != (flashStatus & (uint8)cva_g_lockBit))
                    {
#if 0

                        DEBUG_INFO(DBG_MEMCTRL, "SPI Protection Failed");
#endif
                        retVal = -RT_ERR_FAILED;
                    }
                }
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Check the SPIC is busy or not
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT
 * @retval Others Please refer to cva_reg_read().
 */
static RtkApiRet cva_drv_spi_flash_check_spic_busy(uint32 unit)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = (uint32)0u;
    uint32    timeout = (uint32)cva_g_spiTimeout;

    /* Check Controller Ready */
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_FLASH_SR, &regVal));
        timeout--;
    } while(((uint32)0u != (regVal & (uint32)CVA_SPIC_BUSY)) && ((uint32)0u != (timeout)));

    if((uint32)0u == timeout)
    {
        retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
    }
    return retVal;
}

/**
 * @brief Prepare the configuration to write status register.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_check_spic_busy(),cva_reg_write(),cva_reg_field_write(),
 *                                reg_direct_write_bytes(),
 */
static RtkApiRet cva_drv_spi_flash_write_sr_prepare(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check Controller Ready */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));

        /* set cmd type */
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        /* Tx mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0u));
        /* Enable spi_slave0*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SER, (uint32)0x1u));
        /* read one byte back, for RDSR*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR1, (uint32)0x1u));
        /* SPI read dummy cycle = cva_g_spiSysDelay*/
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN, (uint32)cva_g_spiSysDelay));
        /* SPI Address byte = 1 in user mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x1u));
        /* WREN cmd*/
        CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_WREN_COM));
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Write the status and config register to flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] statusData Status data (0x0 to 0xFF)\n
 *            The data to write in status register in flash.
 * @param[in] configData Config data (0x0 to 0xFF)\n
 *            The  data to write in config register in flash.
 * @param[in] mode Mode (0 to 1) \n
 *            Specific the mode to write register.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_write_sr_prepare(),reg_direct_write_bytes(),cva_reg_write(),
 *                                cva_drv_spi_flash_check_spic_busy(),cva_drv_spi_flash_wait_rdy().
 */
RtkApiRet cva_drv_spi_flash_write_sr(uint32 unit, uint8 statusData, uint8 configData, uint8 mode)
{
    /* Mode 0: transmit 1-byte data */
    /* Mode 1: transmit 2-byte data */
    RtkApiRet retVal = RT_ERR_OK;

    do
    {

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr_prepare(unit));
        if((uint8)0u == mode)
        {
            /* WRSR cmd*/
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_WRSR_COM));
            /* Write status register*/
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)statusData));
        }
        else if((uint8)1u == mode)
        {
            /* SPI Address byte = 2 in user mode*/
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x2u));
            /* WRSR cmd*/
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_WRSR_COM));
            /* Write status register*/
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)statusData));
            /* Write configuration register*/
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)configData));
        }
        else
        {
            /* do nothing */
        }
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        /* check flash busy?*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_wait_rdy(unit));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Read the status register from flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] flashStatus Status register data (0x0 to 0xFF) \n
 *            The status register read from flash.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_check_spic_busy(),cva_reg_write(),,cva_reg_read(),cva_reg_field_write(),
 *                                reg_direct_write_bytes(),cva_drv_spi_flash_check_spic_busy().
 */
RtkApiRet cva_drv_spi_flash_read_sr(uint32 unit, uint8 *flashStatus)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    *flashStatus = (uint8)0u;

    do
    {
        /* Check Controller Ready */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* set cmd type*/
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        /* Rx mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x300u));
        /* Enable spi_slave0*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SER, (uint32)0x1u));
        /* read one byte back*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR1, (uint32)0x1u));
        /* SPI read dummy cycle = cva_g_spiSysDelay*/
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN, (uint32)cva_g_spiSysDelay));
        /* SPI Address byte = 3 in user mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x03u));
        /* RDSR cmd*/
        CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_RDSR_COM));
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* get SR from DR*/
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_DR, &regVal));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        *flashStatus = (uint8)(regVal & (uint32)0xffu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Read the config register from flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] flashConfig Config register data (0x0 to 0xFF) \n
 *            The config register read from flash.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_check_spic_busy(),cva_reg_write(),,cva_reg_read(),cva_reg_field_write(),
 *                                reg_direct_write_bytes(),cva_drv_spi_flash_check_spic_busy().
 */
RtkApiRet cva_drv_spi_flash_read_cr(uint32 unit, uint8 *flashConfig)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        /* Check Controller Ready */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));

        /* set cmd type */
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        /* Rx mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x300u));
        /* Enable spi_slave0*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SER, (uint32)0x1u));
        /* read one byte back*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR1, (uint32)0x1u));
        /* SPI read dummy cycle = cva_g_spiSysDelay*/
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN, (uint32)cva_g_spiSysDelay));
        /* SPI Address byte = 3 in user mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x3u));
        /* RDCR cmd*/
        CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)cva_g_spicRdcrCmd));
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* get ID from DR*/
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_DR, &regVal));
        *flashConfig = (uint8)(regVal & (uint32)0xffu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Read the JEDEC ID from flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] flashId Flash ID (0x0 to 0xFFFFFF)\n
 *            The ID read from flash.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_check_spic_busy(),cva_reg_write(),,cva_reg_read(),cva_reg_field_write(),
 *                                reg_direct_write_bytes(),cva_drv_spi_flash_check_spic_busy().
 */
RtkApiRet cva_drv_spi_flash_read_id(uint32 unit, uint32 *flashId)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        /* Check Controller Ready */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));

        /* set cmd type */
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        /* Rx mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x300u));
        /* Enable spi_slave0*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SER, (uint32)0x1u));
        /* read 3 byte back*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR1, (uint32)0x3u));
        /* SPI read dummy cycle = cva_g_spiSysDelay*/
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN, (uint32)cva_g_spiSysDelay));
        /* SPI Address byte = 3 in user mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0X3u));
        /* Read ID cmd*/
        CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_RDID_COM));
        /* Enable SPIC*/ /* RDID cmd*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* get ID from DR*/
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_DR, &regVal));
        *flashId = regVal & (uint32)0x00FFFFFFu;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Wait the ready bit from flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT
 * @retval Others Please refer to cva_drv_spi_flash_read_sr().
 */
static RtkApiRet cva_drv_spi_flash_wait_rdy(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     flashStatus;
    uint32    timeout = cva_g_spiTimeout;
    if((uint8)0u == cva_g_spiInit)
    {
        do
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
            timeout--;
        } while(((uint8)CVA_SPI_FLASH_WIP == (flashStatus & (uint8)CVA_SPI_FLASH_WIP)) && ((uint32)0u != timeout));

        if((uint32)0u == timeout)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
        }
    }
    return retVal;
}

/**
 * @brief Configure the register for flash writing.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_write(),cva_reg_field_write(),
 */
static RtkApiRet cva_drv_spi_flash_config_flash_write(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* config reg */
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        /* Set SER*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SER, (uint32)0x1u));
        /* read one byte back, for RDSR*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR1, (uint32)0x1u));
        /* SPI read dummy cycle = cva_g_spiSysDelay*/
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN, (uint32)cva_g_spiSysDelay));
        /* SPI Address byte = 3 in user mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x3u));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Configure the register for flash writing.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] startAddr Start address (0x0 to 0xFFFFFF)\n
 *            The start address in flash to write.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_write(),reg_direct_write_bytes(),cva_drv_spi_flash_check_spic_busy().
 */
static RtkApiRet cva_drv_spi_flash_internal_write_prepare(uint32 unit, uint32 startAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addressTmp;
    do
    {
        /* Tx mode & number of channel*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        if((uint8)RTK_SPI_FLASH_QUAD_CHANNEL == cva_g_spiCh)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x80000));
            if((uint8)CVA_SPIC_ADDR_MULTI == cva_g_spicProgramAddrChannel)
            {
                CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0xA0000));
            }
        }
        /* WREN cmd*/
        CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_WREN_COM));
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));

        addressTmp = (startAddr >> 16u) + (startAddr & (uint32)0xff00u) + ((startAddr << 16u) & (uint32)0xff0000u);
        /* Write cmd + address*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_DR, (uint32)cva_g_spicProgramCmd | (addressTmp << 8u)));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Write the data to CVA_FLASH_DR.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] buf Data buffer (0x0 to 0xFF)\n
 *            The data buffer to write in flash.
 * @param[in] writeLen Write length (1 to 256)\n
 *            The length to write.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to reg_direct_write_bytes().
 */
static RtkApiRet cva_drv_spi_flash_internal_write_to_dr_left(uint32 unit, const uint8 *buf, uint32 writeLen)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* < 4 bytes*/
        if((writeLen % ((uint32)4u)) == (uint32)1u)
        {
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)buf[writeLen - 1u]));
        }
        else if((writeLen % ((uint32)4u)) == (uint32)2u)
        {
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)buf[writeLen - 2u]));
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)buf[writeLen - 1u]));
        }
        else if((writeLen % ((uint32)4u)) == (uint32)3u)
        {
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)buf[writeLen - 3u]));
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)buf[writeLen - 2u]));
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)buf[writeLen - 1u]));
        }
        else
        {
            /* do nothing */
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The subfuntion to write flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] startAddr Start address (0x0 to 0xFFFFFF) \n
 *            The start address to write in flash.
 * @param[in] buf Start address (0x0 to 0xFF) \n
 *            The data buffer to write in flash.
 * @param[in] writeLen Write length (1 to 256) \n
 *            The length to write.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_drv_memcpy(),cva_drv_spi_flash_internal_write_prepare(),cva_reg_write(),
 *                                cva_drv_spi_flash_internal_write_to_dr_left(),cva_drv_spi_flash_check_spic_busy(),
 *                                cva_drv_spi_flash_wait_rdy().
 */
static RtkApiRet cva_drv_spi_flash_internal_write(uint32 unit, uint32 startAddr, const uint8 *buf, uint32 writeLen)
{
    RtkApiRet     retVal = RT_ERR_OK;
    uint32        i;
    const uint32 *pbuf;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memcpy((void *)(&pbuf), (void *)(&buf), sizeof(pbuf)));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_internal_write_prepare(unit, startAddr));

        /* write data */
        /* >= 4 bytes*/
        for(i = 0u; i < (writeLen / ((uint32)4u)); i++)
        {
#ifdef RTK_LITTLE_ENDIAN
            /* Little Endian */
            retVal = cva_reg_write(unit, (uint32)CVA_FLASH_DR, pbuf[i]);
#else
            /* Big Endian */
            retVal = cva_reg_write(unit, (uint32)CVA_FLASH_DR, swapl32(pbuf[i]));
#endif
            if(RT_ERR_OK != retVal)
            {
                continue;
            }
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_internal_write_to_dr_left(unit, buf, writeLen));
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x01u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));

        /* check flash busy?*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_wait_rdy(unit));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to write flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] startAddr Start address (0x0 to 0xFFFFFF) \n
 *            The start address to write in flash.
 * @param[in] buf Start address (0x0 to 0xFF) \n
 *            The data buffer to write in flash.
 * @param[in] writeLen Write length (0x0 to 0xFFFFFF) \n
 *            The length to write.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_spi_flash_check_spic_busy(),cva_drv_spi_flash_chip_unlock(),cva_drv_spi_flash_config_flash_write(),
 *                                cva_drv_spi_flash_internal_write(),cva_drv_spi_flash_chip_lock().
 */
RtkApiRet cva_drv_spi_flash_write(uint32 unit, uint32 startAddr, const uint8 *buf, uint32 writeLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i;
    uint32    inFifoNum;
    uint32    lenLeft;
    uint32    cnt;
    do
    {
        if(((uint8)RTK_SPI_FLASH_SINGLE_CHANNEL != cva_g_spiCh) && ((uint8)RTK_SPI_FLASH_DUAL_CHANNEL != cva_g_spiCh) && ((uint8)RTK_SPI_FLASH_QUAD_CHANNEL != cva_g_spiCh))
        {
#if 0
            DEBUG_INFO(DBG_MEMCTRL, "Channel must be set 1, 2 or 4!\n");
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }
        if(0u == writeLen)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));

        if((uint8)0u == cva_g_lockForUpdate)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_chip_unlock(unit));
        }

#if 0
        if((uint8)0u == cva_g_spiInit)
        {
            /*set 4 channel*/
            if((uint8)SPIC_QUAD_CHANNEL == cva_g_spiCh)
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(0x40u, 0u, 0u));
            }
        }
#endif
        /* config reg */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_config_flash_write(unit));
        i = (uint32)0u;
        while(i < writeLen)
        {
            lenLeft = writeLen - i;
            if(lenLeft > (uint32)CVA_FIFONum)
            {
                inFifoNum = (uint32)CVA_FIFONum;
            }
            else
            {
                inFifoNum = lenLeft;
            }

            if((((startAddr + i) % (uint32)256u) + inFifoNum) > (uint32)256u)
            {
                cnt = (uint32)256u - ((startAddr + i) % (uint32)256u);
            }
            else
            {
                cnt = inFifoNum;
            }
            retVal = cva_drv_spi_flash_internal_write(unit, startAddr + i, &buf[i], cnt);
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            i = i + cnt;
        }
        if((uint8)0u == cva_g_lockForUpdate)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_chip_lock(unit));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to check erasing flag.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] eraseAddr Erase address (0x0 to 0xFFFFFF) \n
 *            The  address to erase in flash.
 * @param[in] flag Flag (CVA_SPIC_CHIP_ERASE, CVA_SPIC_SECTOR_ERASE, CVA_SPIC_BLK64K_ERASE) \n
 *            The erase flag.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_check_spic_busy(),cva_drv_spi_flash_chip_unlock().
 */
static RtkApiRet cva_drv_spi_flash_erase_precheck_flag(uint32 unit, uint32 eraseAddr, uint8 flag)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        if((uint8)CVA_SPIC_ERASE_END <= flag)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));

        if((uint8)0u == cva_g_lockForUpdate)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_chip_unlock(unit));
        }
        if((uint8)CVA_SPIC_CHIP_ERASE == flag)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Chip Erase... ...");
#endif
        }

        else if((uint8)CVA_SPIC_SECTOR_ERASE == flag)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Sector Erase %d", (eraseAddr >> 12u));
#endif
        }
        else if((uint8)CVA_SPIC_BLK64K_ERASE == flag)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Block64K Erase %d", (eraseAddr >> 16u));
#endif
        }
        else
        {
            /* do nothing */
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The sub funtion to erase the flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] eraseAddr Erase address (0x0 to 0xFFFFFF) \n
 *            The  address to erase in flash.
 * @param[in] flag Flag (CVA_SPIC_CHIP_ERASE, CVA_SPIC_SECTOR_ERASE, CVA_SPIC_BLK64K_ERASE) \n
 *            The erase flag.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_erase_precheck_flag(),cva_reg_write(),cva_reg_field_write(),
 *                                reg_direct_write_bytes(),cva_drv_spi_flash_check_spic_busy().
 */
static RtkApiRet cva_drv_spi_flash_erase_prepare(uint32 unit, uint32 eraseAddr, uint8 flag)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase_precheck_flag(unit, eraseAddr, flag));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN, (uint32)cva_g_spiSysDelay)); /* SPI read dummy cycle = cva_g_spiSysDelay*/
        if((uint8)CVA_SPIC_CHIP_ERASE == flag)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x1u));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x3u));
        }
        /* Set SER*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SER, (uint32)0x1u));
        /* read one byte back, for RDSR*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR1, (uint32)0x1u));
        /* Tx mode*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0u));
        /* WREN cmd*/
        CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes(CVA_FLASH_DR, (uint32)CVA_FLASH_WREN_COM));
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to erase the flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] eraseAddr Erase address (0x0 to 0xFFFFFF) \n
 *            The address to erase in flash.
 * @param[in] flag Flag (CVA_SPIC_CHIP_ERASE, CVA_SPIC_SECTOR_ERASE, CVA_SPIC_BLK64K_ERASE) \n
 *            The erase flag.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_erase_prepare(),reg_direct_write_bytes(),cva_reg_write(),
 *                                cva_drv_spi_flash_check_spic_busy(),cva_drv_spi_flash_wait_rdy(),cva_drv_spi_flash_chip_lock().
 */
RtkApiRet cva_drv_spi_flash_erase(uint32 unit, uint32 eraseAddr, uint8 flag)
{
    /* Flag 0 = chip erase
       Flag 1 = sector erase
       Flag 2 = block erase (64K)*/
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addressTmp;

    do
    {

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase_prepare(unit, eraseAddr, flag));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        if((uint8)CVA_SPIC_CHIP_ERASE == flag)
        {
            /* Erase Chip*/
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_CE_COM));
        }
        else if((uint8)CVA_SPIC_SECTOR_ERASE == flag)
        {
            addressTmp = (eraseAddr >> 16u) + (eraseAddr & (uint32)0xff00u) + ((eraseAddr << 16u) & (uint32)0xff0000u);
            /*Erase Sector*/
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_SE_COM | (addressTmp << 8u)));
        }
        else if((uint8)CVA_SPIC_BLK64K_ERASE == flag)
        {
            addressTmp = (eraseAddr >> 16u) + (eraseAddr & (uint32)0xff00u) + ((eraseAddr << 16u) & (uint32)0xff0000u);
            /* Erase Block*/
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_DR, (uint32)CVA_FLASH_BE_COM | (addressTmp << 8u)));
        }
        else
        {
            /* do nothing */
        }
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));

        /* check flash busy?*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_wait_rdy(unit));

        if((uint8)0u == cva_g_lockForUpdate)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_chip_lock(unit));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Configure the reading config for dual mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_reg_write(),cva_reg_field_write(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_config_flash_read_dual(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x40300u));
        if((uint8)CVA_SPIC_ADDR_MULTI == cva_g_spicReadAddrChannel)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x50300u));
            if((uint8)4u <= cva_g_spicCmdDelay)
            {
                /* SPI read dummy cycle = 4*/
                CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH,
                                                             CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN,
                                                             ((((uint32)cva_g_spicCmdDelay - ((uint32)8u >> 1u)) * cva_g_spicBaudr * (uint32)2u) + (uint32)cva_g_spiSysDelay)));
            }
            else
            {
                retVal = -RT_ERR_FAILED;
                continue;
            }
        }
        else
        {
            if((uint8)8u <= cva_g_spicCmdDelay)
            {
                /* read dummy cycle = 8*/
                CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH,
                                                             CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN,
                                                             ((((uint32)cva_g_spicCmdDelay - (uint32)8u) * cva_g_spicBaudr * (uint32)2u) + (uint32)cva_g_spiSysDelay)));
            }
            else
            {
                retVal = -RT_ERR_FAILED;
                continue;
            }
        }

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Configure the reading config for quad mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_reg_write(),cva_reg_field_write(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_config_flash_read_quad(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x80300u));
        if((uint8)CVA_SPIC_ADDR_MULTI == cva_g_spicReadAddrChannel)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0xA0300u));
            if((uint8)2u <= cva_g_spicCmdDelay)
            {
                /* SPI read dummy cycle = 6*/
                CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH,
                                                             CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN,
                                                             ((((uint32)cva_g_spicCmdDelay - ((uint32)8u >> 2u)) * cva_g_spicBaudr * (uint32)2u) + (uint32)cva_g_spiSysDelay)));
            }
            else
            {
                retVal = -RT_ERR_FAILED;
                continue;
            }
        }
        else
        {
            if((uint8)8u <= cva_g_spicCmdDelay)
            {
                /* SPI read dummy cycle = 8*/
                CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FLASH_AUTO_LENGTH,
                                                             CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN,
                                                             ((((uint32)cva_g_spicCmdDelay - (uint32)8u) * cva_g_spicBaudr * (uint32)2u) + (uint32)cva_g_spiSysDelay)));
            }
            else
            {
                retVal = -RT_ERR_FAILED;
                continue;
            }
        }

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Configure the reading config for single mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_write(),cva_reg_field_write(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_config_flash_read_single(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x300u));
        if((uint8)CVA_SPIC_HIGH_SPEED_TH < cva_g_spicClk)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR0, (uint32)0x400300u));
        }
        /* SPI read dummy cycle = 0*/
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, (uint32)CVA_FLASH_AUTO_LENGTH,
                                                     CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LSP, CVA_FLASH_AUTO_LENGTH_RD_DUMMY_LENGTH_LEN,
                                                     ((uint32)cva_g_spiSysDelay)));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Configure the config for flash reading.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_write(),cva_drv_spi_flash_config_flash_read_dual(),cva_drv_spi_flash_config_flash_read_quad(),
 *                                cva_drv_spi_flash_config_flash_read_single().
 */
static RtkApiRet cva_drv_spi_flash_config_flash_read(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* config reg */
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        /* Set SER*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SER, (uint32)0x1u));
        if((uint8)0u < cva_g_spicCmdDelay)
        {
            /* SPI Address byte = 4 in user mode*/
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0u));
        }
        else
        {
            /* SPI Address byte = 3 in user mode*/
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_ADDR_LENGTH, (uint32)0x3u));
        }

        /* Rx mode & number of channel*/
        if((uint8)RTK_SPI_FLASH_DUAL_CHANNEL == cva_g_spiCh)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_config_flash_read_dual(unit));
        }
        else if((uint8)RTK_SPI_FLASH_QUAD_CHANNEL == cva_g_spiCh)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_config_flash_read_quad(unit));
        }
        else
        {
            /* RTK_SPI_FLASH_SINGLE_CHANNEL*/
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_config_flash_read_single(unit));
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Prepare the register for flash reading.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.

 * @param[in] startAddr Start address (0x0 to 0xFFFFFF) \n
 *            The start address to read from flash.
 * @param[in] readLen Read length (0x0 to 0xFFFFFF) \n
 *            The length to read from flash.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_write(),reg_direct_write_bytes(),cva_drv_spi_flash_check_spic_busy().
 */
static RtkApiRet cva_drv_spi_flash_internal_read_prepare(uint32 unit, uint32 startAddr, uint32 readLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addressTmp;
    do
    {
        /* read data back*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_CTRLR1, (uint32)readLen));
        addressTmp = (startAddr >> 16u) + (startAddr & (uint32)0xff00u) + ((startAddr << 16u) & (uint32)0xff0000u);

        /* cmd + address*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_FLASH_DR, (uint32)cva_g_spicReadCmd | (addressTmp << 8u)));

        if((uint8)0u < cva_g_spicCmdDelay)
        {
            /* cmd + address*/
            CHK_FUN_CONTINUE(retVal, reg_direct_write_bytes((uint32)CVA_FLASH_DR, (uint32)cva_g_spicModeBitValue)); /* read data back*/
        }
        /* Enable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0x1));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        /* Disable SPIC*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The sub function read the flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] startAddr Start address (0x0 to 0xFFFFFF) \n
 *            The start address to read from flash.
 * @param[in] readLen Read length (0x0 to 0xFFFFFF) \n
 *            The length to read from flash.
 * @param[out] buf Data buffer (0x0 to 0xFF) \n
 *            The data buffer to read from flash.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_internal_read_prepare(),cva_reg_read().
 */
static RtkApiRet cva_drv_spi_flash_internal_read(uint32 unit, uint32 startAddr, uint8 *buf, uint32 readLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal;
    uint32    i;
    uint8    *tmpBuf = buf;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_internal_read_prepare(unit, startAddr, readLen));

        /* get data */
        /* Read Data >= 4 bytes*/
        for(i = 0; i < (readLen / ((uint32)4u)); i++)
        {
            retVal = cva_reg_read(unit, CVA_FLASH_DR, &regVal);
            if(RT_ERR_OK != retVal)
            {
                break;
            }

            *tmpBuf = (uint8)(regVal & (uint32)0xffu);
            tmpBuf++;
            *tmpBuf = (uint8)((regVal & (uint32)0xff00u) >> 8u);
            tmpBuf++;
            *tmpBuf = (uint8)((regVal & (uint32)0xff0000u) >> 16u);
            tmpBuf++;
            *tmpBuf = (uint8)((regVal & (uint32)0xff000000u) >> 24u);
            tmpBuf++;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        /*<4 bytes*/
        if((uint32)1u == (readLen % ((uint32)4u)))
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_DR, &regVal));
            *tmpBuf = (uint8)(regVal & (uint32)0xffu);
            tmpBuf++;
        }
        else if((uint32)2u == (readLen % ((uint32)4u)))
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_FLASH_DR, &regVal));
            *tmpBuf = (uint8)(regVal & (uint32)0xffu);
            tmpBuf++;
            *tmpBuf = (uint8)((regVal & (uint32)0xff00u) >> 8u);
            tmpBuf++;
        }
        else if((uint32)3u == (readLen % ((uint32)4u)))
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_FLASH_DR, &regVal));
            *tmpBuf = (uint8)(regVal & (uint32)0xffu);
            tmpBuf++;
            *tmpBuf = (uint8)((regVal & (uint32)0xff00u) >> 8u);
            tmpBuf++;
            *tmpBuf = (uint8)((regVal & (uint32)0xff0000u) >> 16u);
            tmpBuf++;
        }
        else
        {
            /* do nothing */
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The function read the flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] startAddr Start address (0x0 to 0xFFFFFF) \n
 *            The start address to read from flash.
 * @param[in] readLen Read length (0x0 to 0xFFFFFF) \n
 *            The length to read from flash.
 * @param[out] buf Data buffer (0x0 to 0xFF) \n
 *            The data buffer to read from flash.
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_spi_flash_check_spic_busy(),cva_drv_spi_flash_wait_rdy(),
 *                                cva_drv_spi_flash_config_flash_read(),cva_drv_spi_flash_internal_read().
 */
RtkApiRet cva_drv_spi_flash_read(uint32 unit, uint32 startAddr, uint8 *buf, uint32 readLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i;

    do
    {
        if(((uint8)RTK_SPI_FLASH_SINGLE_CHANNEL != cva_g_spiCh) && ((uint8)RTK_SPI_FLASH_DUAL_CHANNEL != cva_g_spiCh) && ((uint8)RTK_SPI_FLASH_QUAD_CHANNEL != cva_g_spiCh))
        {
#if 0
            DEBUG_INFO(DBG_MEMCTRL, "Channel must be set 1, 2 or 4!\n");
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }
        if(0u == readLen)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_check_spic_busy(unit));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_wait_rdy(unit));

#if 0
        /*set 4 channel*/
        if(((uint8)RTK_SPI_FLASH_QUAD_CHANNEL == cva_g_spiCh) && ((uint8)0u==cva_g_spiInit))
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(0x40,0,0));
        }
#endif

        /* config reg */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_config_flash_read(unit));
        for(i = (uint32)0u; i < readLen; i += (uint32)CVA_FIFONum)
        {
            if((i + (uint32)CVA_FIFONum) > readLen)
            {
                retVal = cva_drv_spi_flash_internal_read(unit, startAddr + i, &buf[i], readLen - i);
            }
            else
            {
                retVal = cva_drv_spi_flash_internal_read(unit, startAddr + i, &buf[i], CVA_FIFONum);
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The function to get op and layout.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] val Value (0x0 to 0xFF) \n
 *            The value of dummy register.
 * @param[out] op OP (0 to 3) \n
 *            The register of zone select.
 * @param[out] flashLayout Flash layout (0 to 7) \n
 *            The information of flash layout.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_init_get_op_and_layout(uint32 unit, uint32 *val, uint8 *op, uint32 *flashLayout)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SW_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DMY_REG_0, val));
        *op = (uint8)(*val & (uint32)0xC0u) >> 6u;
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DMY_REG_2, flashLayout));
        *flashLayout = *flashLayout & (uint32)0x7u;
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The function to get dummy cycle and zone select.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] val Value(0x0 to 0xFF) \n
 *            The value of dummy register.
 * @param[in] op OP (0 to 3)\n
 *            The register of zone select.
 * @param[out] zoneSel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B) \n
 *            The information of zone select.
 * @retval RT_ERR_OK
 */
static RtkApiRet cva_drv_spi_flash_init_get_dmycycle_and_zonesel(uint32 unit, uint32 val, uint8 op, uint8 *zoneSel)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* get dummy cycle*/
        if((uint32)0x20u == (val & (uint32)0x20u))
        {
            cva_g_spiSysDelay = (uint16)(val & (uint32)0x1Fu);
        }
        /* get zoneSel*/
        if((uint8)1u == op)
        {
            *zoneSel = (uint8)CVA_FLASH_ZONE_A;
        }
        else if((uint8)2u == op)
        {
            *zoneSel = (uint8)CVA_FLASH_ZONE_B;
        }
        else
        {
            /* do nothing */
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the 1 channel setting for spi flash init.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_get_spi_speed(),cva_reg_read().
 */
static RtkApiRet cva_drv_spi_flash_init_back_to_1ch(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* back to 1 channel*/
        /* get clk*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_spi_speed(unit, &cva_g_spicClk));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_BAUDR, &cva_g_spicBaudr));
        cva_g_spiCh                  = (uint8)RTK_SPI_FLASH_SINGLE_CHANNEL;
        cva_g_spicProgramAddrChannel = (uint8)CVA_SPIC_ADDR_SINGLE;
        cva_g_spicReadAddrChannel    = (uint8)CVA_SPIC_ADDR_SINGLE;
        cva_g_spicProgramCmd         = (uint8)CVA_FLASH_PP_COM;
        cva_g_spicModeBitValue       = (uint8)0xFFu;
        if((uint8)CVA_SPIC_HIGH_SPEED_TH >= cva_g_spicClk)
        {
            /* Normal Read*/
            cva_g_spicReadCmd  = (uint8)CVA_FLASH_READ_COM;
            cva_g_spicCmdDelay = (uint8)0u;
        }
        else
        {
            /* Fast Read*/
            cva_g_spicReadCmd  = (uint8)CVA_FLASH_FAST_READ_COM;
            cva_g_spicCmdDelay = (uint8)8u;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the channel, baudrate and clock.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] flashBuf Flash buffer (0x0 to 0xFF)\n
 *            The data buffer read from flash header.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_read(),cva_reg_write(),cva_drv_spi_flash_get_spi_speed().
 */
static RtkApiRet cva_drv_spi_flash_init_get_ch_baud_clk(uint32 unit, uint8 *flashBuf)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    baudrate = (uint32)0u;
    do
    {
        /* get channel*/
        if((uint8)0x3u == (flashBuf[0] & (uint8)0x3u))
        {
            cva_g_spiCh = (uint8)RTK_SPI_FLASH_SINGLE_CHANNEL;
        }
        else if((uint8)0x2u == (flashBuf[0] & (uint8)0x3u))
        {
            cva_g_spiCh = (uint8)RTK_SPI_FLASH_DUAL_CHANNEL;
        }
        else if((uint8)0x1u == (flashBuf[0] & (uint8)0x3u))
        {
            cva_g_spiCh = (uint8)RTK_SPI_FLASH_QUAD_CHANNEL;
        }
        else
        {
            cva_g_spiCh = (uint8)RTK_SPI_FLASH_SINGLE_CHANNEL;
        }
        /* get BAUD rate*/
        if((uint8)0x0u == (flashBuf[0] & (uint8)0x04u))
        {
            cva_g_spicBaudr = ((((uint32)flashBuf[0]) & (uint32)0x78u) >> 3u);
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_BAUDR, &baudrate));
            if(baudrate != cva_g_spicBaudr)
            {
                CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_BAUDR, cva_g_spicBaudr));
                CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_FBAUDR, cva_g_spicBaudr));
            }
        }

        /* get clk*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_spi_speed(unit, &cva_g_spicClk));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the read command and program command.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] flashBuf Flash buffer (0x0 to 0xFF) \n
 *            The data buffer read from flash header.
 * @param[out] None
 * @retval RT_ERR_OK
 */
static RtkApiRet cva_drv_spi_flash_init_get_rdcmd_pgcmd(uint32 unit, uint8 *flashBuf)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* get command*/
        if((uint8)0xFFu != flashBuf[3])
        {
            cva_g_spicProgramCmd = flashBuf[3];
        }
        else
        {
            if(((uint8)RTK_SPI_FLASH_SINGLE_CHANNEL == cva_g_spiCh) || ((uint8)RTK_SPI_FLASH_DUAL_CHANNEL == cva_g_spiCh))
            {
                cva_g_spicProgramCmd = (uint8)CVA_FLASH_PP_COM;
            }
            else if(cva_g_spiCh == (uint8)RTK_SPI_FLASH_QUAD_CHANNEL)
            {
                cva_g_spicProgramCmd = (uint8)CVA_FLASH_QPP_COM;
            }
            else
            {
                /* do nothing */
            }
        }

        if((uint8)0xFFu != flashBuf[4])
        {
            cva_g_spicReadCmd = flashBuf[4];
        }
        else
        {
            if((uint8)RTK_SPI_FLASH_SINGLE_CHANNEL == cva_g_spiCh)
            {
                if((uint8)CVA_SPIC_HIGH_SPEED_TH >= cva_g_spicClk)
                {
                    /* Normal Read*/
                    cva_g_spicReadCmd = (uint8)CVA_FLASH_READ_COM;
                }
                else
                {
                    /* Fast Read*/
                    cva_g_spicReadCmd = (uint8)CVA_FLASH_FAST_READ_COM;
                }
            }
            else if((uint8)RTK_SPI_FLASH_DUAL_CHANNEL == cva_g_spiCh)
            {
                /* DRead*/
                cva_g_spicReadCmd = (uint8)CVA_FLASH_DREAD_COM;
            }
            else if((uint8)RTK_SPI_FLASH_QUAD_CHANNEL == cva_g_spiCh)
            {
                /* QRead*/
                cva_g_spicReadCmd = (uint8)CVA_FLASH_QREAD_COM;
            }
            else
            {
                /* do nothing */
            }
        }

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the channel, baudrate and clock.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] flashBuf Flash buffer (0x0 to 0xFF) \n
 *            The data buffer read from flash header.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_init_get_ch_baud_clk(),cva_drv_spi_flash_init_get_rdcmd_pgcmd().
 */
static RtkApiRet cva_drv_spi_flash_init_get_setting(uint32 unit, uint8 *flashBuf)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /*get channel baudrate clock*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_init_get_ch_baud_clk(unit, flashBuf));

        /* get QE location*/
        cva_g_spicQeLocation = (flashBuf[0] >> 7u) & (uint8)0x1u;

        /* get number of channel*/
        cva_g_spicProgramAddrChannel = (flashBuf[2] >> 6u) & (uint8)0x1u;
        cva_g_spicReadAddrChannel    = (flashBuf[2] >> 7u) & (uint8)0x1u;

        /* get command*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_init_get_rdcmd_pgcmd(unit, flashBuf));
        if((uint8)0xFFu != flashBuf[5])
        {
            cva_g_spicRdcrCmd = flashBuf[5];
        }
        else
        {
            cva_g_spicRdcrCmd = (uint8)CVA_FLASH_RDCR_COM;
        }
        /* get mode bit*/
        cva_g_spicModeBitValue = flashBuf[6];
        /* get dummy for command*/
        if((uint8)0x0Fu == (flashBuf[7] & (uint8)0x0Fu))
        {
            switch(cva_g_spicReadCmd)
            {
                case CVA_FLASH_READ_COM:
                    cva_g_spicCmdDelay = (uint8)0u;
                    break;
                case CVA_FLASH_FAST_READ_COM:
                    cva_g_spicCmdDelay = (uint8)8u;
                    break;
                case CVA_FLASH_DREAD_COM:
                    cva_g_spicCmdDelay = (uint8)8u;
                    break;
                case CVA_FLASH_2READ_COM:
                    cva_g_spicCmdDelay = (uint8)4u;
                    break;
                case CVA_FLASH_QREAD_COM:
                    cva_g_spicCmdDelay = (uint8)8u;
                    break;
                case CVA_FLASH_4READ_COM:
                    cva_g_spicCmdDelay = (uint8)6u;
                    break;
                default:
                    cva_g_spicCmdDelay = (uint8)0u;
                    break;
            }
        }
        else
        {
            cva_g_spicCmdDelay = flashBuf[7] & (uint8)0x0Fu;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The SPI flash init function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_get_lock_otp_enable(),cva_drv_spi_flash_init_get_op_and_layout(),
 *                                cva_drv_spi_flash_init_get_dmycycle_and_zonesel(),cva_drv_spi_flash_read(),cva_drv_spi_flash_init_get_setting,
 *                                cva_drv_spi_flash_init_back_to_1ch().
 */
RtkApiRet cva_drv_spi_flash_init(uint32 unit)
{
    RtkApiRet    retVal = RT_ERR_OK;
    uint8        i;
    uint8        zoneSel = (uint8)CVA_FLASH_ZONE_A;
    uint32       startAddr;
    uint8        flashBuf[16];
    uint32       val;
    uint8        op              = (uint8)0u;
    uint32       flashLayout     = (uint32)0;
    const uint32 secondFwAddr[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                    (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                    (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};

    do
    {
        /* Initialize array*/
        for(i = 0u; i < (uint8)sizeof(flashBuf); i++)
        {
            flashBuf[i] = (uint8)0xFFu;
        }
        /*get lock support from otp*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_lock_otp_enable(unit));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_flush_fifo_and_clr_isr(unit));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_init_get_op_and_layout(unit, &val, &op, &flashLayout));
        /* op = 2b00: didn't execute boot code
                2b01: zone A
                2b10: zone B
                2b11: flash no data            */
        if(((uint8)1u == op) || ((uint8)2u == op))
        {

            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_init_get_dmycycle_and_zonesel(unit, val, op, &zoneSel));

            startAddr = ((uint32)zoneSel * secondFwAddr[flashLayout]) + (uint32)CVA_FLASH_SETTING_OFFSET;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, startAddr, flashBuf, (uint32)16u));

            if(((uint8)0x99u == flashBuf[13]) && ((uint8)0x88u == flashBuf[14]) && ((uint8)0x0u == (flashBuf[15] & (uint8)0x01u)))
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_init_get_setting(unit, flashBuf));
            }
            else
            {
                /* back to 1 channel*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_init_back_to_1ch(unit));
            }
        }
        else
        {
            /* set the variables to default value*/
            cva_g_spiSysDelay            = (uint16)CVA_SPIC_SYSDELAY_DEFAULT;
            cva_g_spiInit                = (uint8)0u;
            cva_g_spiCh                  = (uint8)RTK_SPI_FLASH_SINGLE_CHANNEL;
            cva_g_spicProgramCmd         = (uint8)CVA_FLASH_PP_COM;
            cva_g_spicReadCmd            = (uint8)CVA_FLASH_READ_COM;
            cva_g_spicRdcrCmd            = (uint8)CVA_FLASH_RDCR_COM;
            cva_g_spicModeBitValue       = (uint8)0xFFu;
            cva_g_spicProgramAddrChannel = (uint8)CVA_SPIC_ADDR_SINGLE;
            cva_g_spicReadAddrChannel    = (uint8)CVA_SPIC_ADDR_SINGLE;
            cva_g_spicCmdDelay           = (uint8)0u;
            cva_g_spicQeLocation         = (uint8)CVA_SPIC_QE_SR2;
            cva_g_spicClk                = (uint8)12u;
            cva_g_spicBaudr              = (uint32)8u;
        }
#if 0
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spiSysDelay %d\n", cva_g_spiSysDelay);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spiCh %d\n", cva_g_spiCh);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicProgramCmd 0x%x\n", cva_g_spicProgramCmd);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicReadCmd 0x%x\n", cva_g_spicReadCmd);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicRdcrCmd 0x%x\n", cva_g_spicRdcrCmd);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicModeBitValue 0x%x\n", cva_g_spicModeBitValue);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicProgramAddrChannel 0x%d\n", cva_g_spicProgramAddrChannel);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicReadAddrChannel %d\n", cva_g_spicReadAddrChannel);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicCmdDelay %d\n", cva_g_spicCmdDelay);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicQeLocation %d\n", cva_g_spicQeLocation);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicClk %d\n", cva_g_spicClk);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_spicBaudr %d\n", cva_g_spicBaudr);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_lockMode %d\n", cva_g_lockMode);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_protectMode %d\n", cva_g_protectMode);
        DEBUG_INFO(DBG_MEMCTRL, "cva_g_lockBit 0x%x\n", cva_g_lockBit);
#endif
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The function to set the QE bit to flash.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_read_sr(),cva_drv_spi_flash_init_get_op_and_layout(),
 *                                cva_drv_spi_flash_read_cr().
 */
static RtkApiRet cva_drv_spi_flash_set_qe(uint32 unit)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     flashStatus = (uint8)0u;
    uint8     flashConfig = (uint8)0u;
    do
    {
        if((uint8)RTK_SPI_FLASH_QUAD_CHANNEL == cva_g_spiChForUpdate)
        {
            if((uint8)CVA_SPIC_QE_SR1 == cva_g_spicQeLocation)
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
                flashStatus = (flashStatus & (uint8)0xBFu) | (uint8)0x40u;
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(unit, flashStatus, (uint8)0u, (uint8)CVA_SPIC_QE_SR1));
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_sr(unit, &flashStatus));
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_cr(unit, &flashConfig));
                flashConfig = flashConfig | (uint8)0x02u;
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_sr(unit, flashStatus, flashConfig, (uint8)CVA_SPIC_QE_SR2));
            }
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Retry the calibration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_erase(),rtk_usecond_delay(),cva_drv_spi_flash_write(),
 *                                cva_drv_spi_flash_internal_calibration().
 */
static RtkApiRet cva_drv_spi_flash_internal_calibration_retry(uint32 unit)
{
    RtkApiRet   retVal      = RT_ERR_OK;
    const uint8 pattern[16] = {(uint8)0x99u, (uint8)0x99u, (uint8)0x96u, (uint8)0x96u,
                               (uint8)0x3Fu, (uint8)0xCCu, (uint8)0x66u, (uint8)0xFCu,
                               (uint8)0xC0u, (uint8)0x33u, (uint8)0xCCu, (uint8)0x03u,
                               (uint8)0xE5u, (uint8)0xDCu, (uint8)0x31u, (uint8)0x62u};
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase(unit, (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET, (uint8)CVA_SPIC_SECTOR_ERASE));
        /* Wait 0.5 second after sector erase */
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay((uint32)500000u));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write(unit, (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET, (const uint8 *)pattern, (uint32)16u));
        /* Wait 0.05 second after writing pattern into flash */
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay((uint32)50000u));
        /* compare*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_internal_calibration(unit));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The calibration function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),
 *                                cva_drv_spi_flash_internal_calibration(),cva_drv_spi_flash_internal_calibration_retry().
 */
RtkApiRet cva_drv_spi_flash_calibration(uint32 unit)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint16    g_spiSysDelayTmp = 0;
    uint32    regValTmp;

    cva_g_spiInit = 1;
    do
    {
#if 0
        DEBUG_INFO(DBG_MEMCTRL, "calibrating...");
#endif
        /*calibrate*/
        /* change page */
        CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_CPU_PAGE));

        /* store temporary value */
        g_spiSysDelayTmp = cva_g_spiSysDelay;
        CHK_FUN_RET(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, &regValTmp));

        /* Initialize */
        cva_g_spiSysDelay = (uint16)0u;
        CHK_FUN_RET(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, (uint32)0u));
        /* back to the original page.*/
        CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
        CHK_FUN_RET(retVal, cva_drv_spi_flash_internal_calibration(unit));
        if(RT_ERR_OK == retVal)
        {
            continue;
        }
/*write pattern and re-calibrate*/
#if 0
        DEBUG_INFO(DBG_MEMCTRL, "...");
#endif
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_internal_calibration_retry(unit));
    } while(0u == 1u);

    if(RT_ERR_OK != retVal)
    {
        cva_g_spiSysDelay = g_spiSysDelayTmp;
        /* change page */
        CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_CPU_PAGE));

        CHK_FUN_RET(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, regValTmp));

        /* back to the original page.*/
        CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
    }
    cva_g_spiInit = (uint8)0u;

    return retVal;
}

/**
 * @brief The function to find the sysdelay.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] patternAddress Pattern address (0x0 to 0xFFFFFF)\n
 *            The address of pattern in flash.
 * @param[out] candidateArr Candidate array (0x0 to 0x1f) \n
 *             The array of candidate.
 * @param[out] candidateCnt Candidate count (0 to 5) \n
 *             The number of candidate.
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_spi_flash_read().
 */
static RtkApiRet cva_drv_spi_flash_cal_sysdelay_find(uint32 unit, uint8 *candidateArr, uint8 *candidateCnt, const uint32 patternAddress)
{
    RtkApiRet   retVal             = RT_ERR_OK;
    uint8       readBuf[16]        = {(uint8)0u};
    int32       cmpFlag            = (int32)0;
    uint8       sysDlyCandidateCnt = (uint8)0u;
    uint8       sysDlyCandidate[5] = {(uint8)0u};
    uint8       ret                = (uint8)0u;
    const uint8 pattern[16]        = {(uint8)0x99u, (uint8)0x99u, (uint8)0x96u, (uint8)0x96u,
                                      (uint8)0x3Fu, (uint8)0xCCu, (uint8)0x66u, (uint8)0xFCu,
                                      (uint8)0xC0u, (uint8)0x33u, (uint8)0xCCu, (uint8)0x03u,
                                      (uint8)0xE5u, (uint8)0xDCu, (uint8)0x31u, (uint8)0x62u};

    do
    {
        for(cva_g_spiSysDelay = (uint16)1u; cva_g_spiSysDelay < (uint16)0x20u; cva_g_spiSysDelay += (uint16)1u)
        {
            CHK_FUN_RET(retVal, cva_drv_spi_flash_read(unit, patternAddress, readBuf, (uint32)16u));
            if(RT_ERR_OK == retVal)
            {
                cmpFlag = rtl_memcmp(pattern, readBuf, (size_t)16);
                if((int32)0 == cmpFlag)
                {
                    sysDlyCandidate[sysDlyCandidateCnt] = (uint8)cva_g_spiSysDelay;
                    sysDlyCandidateCnt += 1u;
                }
                else
                {
                    if((uint8)0u < sysDlyCandidateCnt)
                    {
                        ret = 1u;
                    }
                    if((uint16)0x1Fu == cva_g_spiSysDelay)
                    {
                        retVal = -RT_ERR_FAILED;
                    }
                }
            }

            if((RT_ERR_OK != retVal) || ((uint8)1u == ret) || ((uint8)5u <= sysDlyCandidateCnt))
            {
                break;
            }
        }

        if((RT_ERR_OK != retVal) || ((uint8)0u == sysDlyCandidateCnt))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        else
        {
            *candidateCnt   = sysDlyCandidateCnt;
            candidateArr[0] = sysDlyCandidate[0];
            candidateArr[1] = sysDlyCandidate[1];
            candidateArr[2] = sysDlyCandidate[2];
            candidateArr[3] = sysDlyCandidate[3];
            candidateArr[4] = sysDlyCandidate[4];
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The action for time compensation success.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_cal_time_compensation_success(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint8)0u;

    CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SW_PAGE));
    CHK_FUN_RET(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DMY_REG_0, &regVal));
    regVal = regVal | ((uint32)CVA_CALIB_RESULT_COMPESANTE_SUCCESS << 8u);
    CHK_FUN_RET(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DMY_REG_0, regVal));
    CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));

    return retVal;
}
/**
 * @brief The action for time compensation fail.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_cal_time_compensation_fail(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint8)0u;

    CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SW_PAGE));
    CHK_FUN_RET(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DMY_REG_0, &regVal));
    regVal = regVal | ((uint32)CVA_CALIB_RESULT_COMPESANTE_FAIL << 8u);
    CHK_FUN_RET(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DMY_REG_0, regVal));

    CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_CPU_PAGE));
    CHK_FUN_RET(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, &regVal));
    regVal = regVal & ~(0x0fffffffu);
    CHK_FUN_RET(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, regVal));
    CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));

    return retVal;
}

/**
 * @brief The function to do time compensation.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] patternAddress Pattern address (0x0 to 0xFFFFFF)\n
 *            The address of pattern in flash.
 * @param[out] candidateArr Candidate array (0x0 to 0x1f)\n
 *             The array of candidate.
 * @param[out] candidateCnt Candidate count (0 to 5)\n
 *             The number of candidate.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_delaychain_revert(),cva_drv_spi_flash_cal_time_compensation_success(),cva_drv_spi_flash_cal_time_compensation_fail().
 */
static RtkApiRet cva_drv_spi_flash_cal_time_compensation(uint32 unit, uint8 *candidateArr, uint8 *candidateCnt, const uint32 patternAddress)
{
    RtkApiRet retVal          = RT_ERR_OK;
    RtkApiRet tmpRet          = RT_ERR_OK;
    uint8     dataDelay       = (uint8)0;
    uint8     tmpCandidate[5] = {(uint8)0u};
    uint8     tmpCandidateCnt = (uint8)0u;
    uint8     rightVal        = (uint8)127u;
    uint8     leftVal         = (uint8)0u;
    uint8     loopExitFlag    = (uint8)0u;
    uint8     cmpResultFlag   = (uint8)0u;
    uint32    regVal          = (uint8)0u;

    do
    {
        dataDelay = (leftVal + rightVal) >> 1;
        if((dataDelay <= leftVal) || (dataDelay >= rightVal))
        {
            cmpResultFlag = (uint8)0u;
            loopExitFlag  = (uint8)1u;
            continue;
        }

        /* Setting delay chain */
        regVal = (((uint32)dataDelay & 0x7Fu)
                  | (((uint32)dataDelay & 0x7Fu) << 7u)
                  | (((uint32)dataDelay & 0x7Fu) << 14u)
                  | (((uint32)dataDelay & 0x7Fu) << 21u));

        CHK_FUN_RET(retVal, cva_drv_spi_flash_delaychain_revert(unit, regVal));

        tmpRet = cva_drv_spi_flash_cal_sysdelay_find(unit, &tmpCandidate[0], &tmpCandidateCnt, patternAddress);

        if(RT_ERR_OK != tmpRet)
        {
            /* Compensate too much */
            rightVal = dataDelay;
            continue;
        }

        if((uint8)3u <= tmpCandidateCnt)
        {
            /* Compensate success, break the loop */
            cmpResultFlag = (uint8)1u;
            loopExitFlag  = (uint8)1u;
            continue;
        }

        if((*candidateCnt) == tmpCandidateCnt)
        {
            if(0 == rtl_memcmp(tmpCandidate, candidateArr, sizeof(uint8) * (*candidateCnt)))
            {
                /* Compensate not enough */
                leftVal = dataDelay;
                continue;
            }
            else
            {
                /* Compensate too much */
                rightVal = dataDelay;
                continue;
            }
        }
        else if((*candidateCnt) > tmpCandidateCnt)
        {
            /* Compensation make situation worse */
            cmpResultFlag = (uint8)0u;
            loopExitFlag  = (uint8)1u;
            continue;
        }
        else
        {
            /* Do nothing */
        }
    } while(0u == loopExitFlag);

    if((uint8)1u == cmpResultFlag)
    {

        CHK_FUN_RET(retVal, cva_drv_spi_flash_cal_time_compensation_success(unit));
        candidateArr[0] = tmpCandidate[0];
        candidateArr[1] = tmpCandidate[1];
        candidateArr[2] = tmpCandidate[2];
        candidateArr[3] = tmpCandidate[3];
        candidateArr[4] = tmpCandidate[4];
        *candidateCnt   = tmpCandidateCnt;
    }
    else
    {
        CHK_FUN_RET(retVal, cva_drv_spi_flash_cal_time_compensation_fail(unit));
    }
    return retVal;
}

/**
 * @brief The action for dummy candidate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dmyCandidateCnt Dummy candidate count (1 to 5)\n
 *            The number of dummy candidate.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_internal_calibration_dmy_candidate(uint32 unit, uint8 dmyCandidateCnt)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regValue = (uint32)0u;

    CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SW_PAGE));
    CHK_FUN_RET(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DMY_REG_0, &regValue));

    if(1u == dmyCandidateCnt)
    {
        regValue = regValue | ((uint32)CVA_CALIB_RESULT_COMPESANTE_NO_NEED_FOR_1_CANDIDATE << 8u);
#if 0 /*DEBUG_INFO*/
        DEBUG_INFO(DBG_MEMCTRL, "CVA_CALIB_RESULT_COMPESANTE_NO_NEED_FOR_1_CANDIDATE\n");
#endif
    }
    else
    {
        regValue = regValue | ((uint32)CVA_CALIB_RESULT_COMPESANTE_NO_NEED << 8u);
#if 0 /*DEBUG_INFO*/
        DEBUG_INFO(DBG_MEMCTRL, "CVA_CALIB_RESULT_COMPESANTE_NO_NEED\n");
#endif
    }
    CHK_FUN_RET(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DMY_REG_0, regValue));
    CHK_FUN_RET(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));

    return retVal;
}

/**
 * @brief The function to set sysdelay.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] dmyCandidateCnt Dummy candidate count (1 to 5)\n
 *            The number of dummy candidate.
 * @param[in] dmyCandidate Dummy candidate (0x0 to 0x1f)\n
 *            The dummy candidate.
 * @param[out] None
 * @retval RT_ERR_OK
 */
static RtkApiRet cva_drv_spi_flash_internal_calibration_set_sysdelay(uint32 unit, uint8 dmyCandidateCnt, uint8 *dmyCandidate)
{
    RtkApiRet retVal = RT_ERR_OK;
    if(0u == dmyCandidateCnt)
    {
        /* Do nothing */
    }
    else if(2u >= dmyCandidateCnt)
    {
        cva_g_spiSysDelay = dmyCandidate[0];
    }
    else if(5u > dmyCandidateCnt)
    {
        cva_g_spiSysDelay = dmyCandidate[1];
    }
    else if(5u == dmyCandidateCnt)
    {
        cva_g_spiSysDelay = dmyCandidate[2];
    }
    else
    {
        /* Do nothing */
    }

    return retVal;
}

/**
 * @brief The sub funtion to do calibration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_reg_read(),cva_drv_spi_flash_cal_sysdelay_find(),cva_drv_spi_flash_cal_time_compensation(),
 *                                cva_drv_spi_flash_internal_calibration_dmy_candidate(),cva_drv_spi_flash_internal_calibration_set_sysdelay().
 */
static RtkApiRet cva_drv_spi_flash_internal_calibration(uint32 unit)
{
    RtkApiRet    retVal = RT_ERR_OK;
    uint32       i;
    uint32       baudrate          = (uint32)0u;
    const uint32 patternAddress[6] = {(uint32)((CVA_FLASH_ZONE_A * CVA_FLASH_ZONE_SIZE_512KB) + (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET),
                                      (uint32)((CVA_FLASH_ZONE_B * CVA_FLASH_ZONE_SIZE_512KB) + (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET),
                                      (uint32)((CVA_FLASH_ZONE_B * CVA_FLASH_ZONE_SIZE_1MB) + (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET),
                                      (uint32)((CVA_FLASH_ZONE_B * CVA_FLASH_ZONE_SIZE_2MB) + (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET),
                                      (uint32)((CVA_FLASH_ZONE_B * CVA_FLASH_ZONE_SIZE_4MB) + (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET),
                                      (uint32)((CVA_FLASH_ZONE_B * CVA_FLASH_ZONE_SIZE_8MB) + (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET)};
    uint8        dmyCandidate[5]   = {(uint8)0u};
    uint8        dmyCandidateCnt   = (uint8)0u;
    uint32       regValue          = (uint32)0u;

    /* scan 6 startaddr
       scan dummy
       dummy hit
       case 1: only one hit -> select one
       case 2: two hit -> do time compensation and selct first dummy
       case 3: 3-4 hit -> select second dummy
       case 5: 5 hit -> select third dummy
       one return*/
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_BAUDR, &regValue));
        baudrate = regValue;
        if((uint32)0u == baudrate)
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }

        for(i = 0u; i < (uint8)6u; ++i)
        {
            retVal = RT_ERR_OK;
            CHK_FUN_RET(retVal, cva_drv_spi_flash_cal_sysdelay_find(unit, &dmyCandidate[0], &dmyCandidateCnt, patternAddress[i]));
            if(RT_ERR_OK != retVal)
            {
                continue;
            }

            /*Delay time compensation*/
            if(2u == dmyCandidateCnt)
            {
                CHK_FUN_RET(retVal, cva_drv_spi_flash_cal_time_compensation(unit, &dmyCandidate[0], &dmyCandidateCnt, patternAddress[i]));
            }
            else
            {
                CHK_FUN_RET(retVal, cva_drv_spi_flash_internal_calibration_dmy_candidate(unit, dmyCandidateCnt));
            }

            CHK_FUN_RET(retVal, cva_drv_spi_flash_internal_calibration_set_sysdelay(unit, dmyCandidateCnt, &dmyCandidate[0]));

            if(RT_ERR_OK == retVal)
            {
                break;
            }
        }
    } while(0u == 1u);

#if 0 /* DEBUG_INFO*/
    if(RT_ERR_OK == retVal)
    {
        DEBUG_INFO(DBG_MEMCTRL, "final cva_g_spiSysDelay = %d\n", cva_g_spiSysDelay);
    }

#endif
    return retVal;
}
/**
 * @brief The funtion fill the calibration header.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] buf Data buffer (0x0 to 0xFf)\n
 *             The data buffer of header.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read().
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_fill_calibration_header(uint32 unit, uint8 *buf)
{
    RtkApiRet   retVal           = RT_ERR_OK;
    uint8       dataDelay[4]     = {(uint8)0u};
    uint32      regVal           = (uint32)0u;
    const uint8 channelTable[5]  = {(uint8)0x0u, (uint8)0x3u, (uint8)0x2u, (uint8)0x0u, (uint8)0x1u};
    const uint8 checkFlashBuf[3] = {(uint8)0x99u, (uint8)0x88u, (uint8)0xFEu};
    do
    {
        /* switch the page. */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_CPU_PAGE));
        /* get the data delay*/
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, &regVal));
        dataDelay[0] = (uint8)(regVal & (uint32)0x7f);
        dataDelay[1] = (uint8)((regVal >> 7u) & (uint32)0x7f);
        dataDelay[2] = (uint8)((regVal >> 14u) & (uint32)0x7f);
        dataDelay[3] = (uint8)((regVal >> 21u) & (uint32)0x7f);
        /* back to the original page. */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));

        /* Flash setting*/
        buf[0] = (uint8)((buf[0] & 0xFFu) | ((channelTable[cva_g_spiCh] & (uint8)0x3u) & 0xFFu) | (((uint8)0x0u << 2u) & 0xFFu) | (((uint8)(cva_g_spicBaudr & (uint8)0xFu) << 3u) & 0xFFu) | (((cva_g_spicQeLocation & (uint8)0x1u) << 7u) & 0xFFu));
        buf[1] = (uint8)((buf[1] & 0xFFu) | (uint8)(((cva_g_spiSysDelay & (uint16)0x1Fu) << 2u) & 0xFFu));
        if(((uint32)RTK_SPI_FLASH_100MHz == cva_g_spicClk) || ((uint32)RTK_SPI_FLASH_50MHz == cva_g_spicClk))
        {
            buf[1] = (uint8)((buf[1] & 0xFFu) | ((uint8)(0x3u) & 0xFFu));
        }
        else if((uint32)RTK_SPI_FLASH_83MHz == cva_g_spicClk)
        {
            buf[1] = (uint8)((buf[1] & 0xFFu) | ((uint8)(0x2u) & 0xFFu));
        }
        else
        {
            buf[1] = (uint8)((buf[1] & 0xFFu) | ((uint8)(0x1u) & 0xFFu));
        }
        buf[2] = (uint8)((buf[2] & 0xFFu) | ((cva_g_spicProgramAddrChannel << 6u) & 0xFFu) | ((cva_g_spicReadAddrChannel << 7u) & 0xFFu));
        buf[3] = cva_g_spicProgramCmd;
        buf[4] = cva_g_spicReadCmd;
        buf[5] = cva_g_spicRdcrCmd;
        buf[6] = cva_g_spicModeBitValue;
        buf[7] = (uint8)((buf[7] & 0xFFu) | ((cva_g_spicCmdDelay & (uint8)0x0Fu) & 0xFFu));
        buf[8] = (uint8)0xFFu;
        if(0u != dataDelay[0])
        {
            buf[8] = (uint8)((uint8)dataDelay[0] | ((uint8)0x0u << 7u));
        }
        buf[9] = (uint8)0xFFu;
        if(0u != dataDelay[1])
        {
            buf[9] = (uint8)((uint8)dataDelay[1] | ((uint8)0x0u << 7u));
        }
        buf[10] = (uint8)0xFFu;
        if(0u != dataDelay[2])
        {
            buf[10] = (uint8)((uint8)dataDelay[2] | ((uint8)0x0u << 7u));
        }
        buf[11] = (uint8)0xFFu;
        if(0u != dataDelay[3])
        {
            buf[11] = (uint8)((uint8)dataDelay[3] | ((uint8)0x0u << 7u));
        }
        buf[12] = (uint8)0xFFu;
        buf[13] = checkFlashBuf[0];
        buf[14] = checkFlashBuf[1];
        buf[15] = checkFlashBuf[2];
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The funtion to get the pattern of header.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] spicPattern SPI controller pattern 32 byte each (0x0 to 0xFF) \n
 *             The flash setting header for image.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_drv_memset(),cva_drv_spi_flash_fill_calibration_header().
 */
RtkApiRet cva_drv_spi_flash_get_pattern(uint32 unit, SpicPattern_t *spicPattern)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(spicPattern, (int32)0, sizeof(SpicPattern_t)));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_fill_calibration_header(unit, spicPattern->pattern));

        /* calibration pattern*/
        spicPattern->pattern[16] = (uint8)0x99u;
        spicPattern->pattern[17] = (uint8)0x99u;
        spicPattern->pattern[18] = (uint8)0x96u;
        spicPattern->pattern[19] = (uint8)0x96u;

        spicPattern->pattern[20] = (uint8)0x3Fu;
        spicPattern->pattern[21] = (uint8)0xCCu;
        spicPattern->pattern[22] = (uint8)0x66u;
        spicPattern->pattern[23] = (uint8)0xFCu;

        spicPattern->pattern[24] = (uint8)0xC0u;
        spicPattern->pattern[25] = (uint8)0x33u;
        spicPattern->pattern[26] = (uint8)0xCCu;
        spicPattern->pattern[27] = (uint8)0x03u;

        spicPattern->pattern[28] = (uint8)0xE5u;
        spicPattern->pattern[29] = (uint8)0xDCu;
        spicPattern->pattern[30] = (uint8)0x31u;
        spicPattern->pattern[31] = (uint8)0x62u;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to set the register of sys clock.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] flag Flag (CVA_SYS_CLK_REG_150MHZ, CVA_SYS_CLK_REG_200MHZ, CVA_SYS_CLK_REG_333MHZ, CVA_SYS_CLK_REG_400MHZ)\n
 *            The sys clock flag.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_read(),cva_reg_field_set(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_sysclk_set_reg(uint32 unit, uint8 flag)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regValue;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_CLK_RST_CLOCK_SELECTION, &regValue));
        if((uint8)CVA_SYS_CLK_REG_150MHZ == flag)
        {
            /* 150MHz*/
            CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_CLOCK_SELECTION, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LSP, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LEN, (uint32)CVA_SYS_CLK_REG_150MHZ, &regValue)); /* set system clock */
        }
        else if((uint8)CVA_SYS_CLK_REG_200MHZ == flag)
        {
            /* 200MHz*/
            CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_CLOCK_SELECTION, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LSP, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LEN, (uint32)CVA_SYS_CLK_REG_200MHZ, &regValue)); /* set system clock */
        }
        else if((uint8)CVA_SYS_CLK_REG_333MHZ == flag)
        {
            /* 333MHz*/
            CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_CLOCK_SELECTION, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LSP, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LEN, (uint32)CVA_SYS_CLK_REG_333MHZ, &regValue)); /* set system clock */
        }
        else if((uint8)CVA_SYS_CLK_REG_400MHZ == flag)
        {
            /* 400MHz*/
            CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_CLOCK_SELECTION, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LSP, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LEN, (uint32)CVA_SYS_CLK_REG_400MHZ, &regValue)); /* set system clock */
        }
        else
        {
            /* do nothing */
        }
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_CLK_RST_CLOCK_SELECTION, regValue));

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to set the sys clock.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] flag Flag (CVA_SYS_CLK_REG_150MHZ, CVA_SYS_CLK_REG_200MHZ, CVA_SYS_CLK_REG_333MHZ, CVA_SYS_CLK_REG_400MHZ)\n
 *            The sys clock flag.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_field_set(),cva_drv_spi_flash_sysclk_set_reg(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_sysclk_set(uint32 unit, uint8 flag)
{
    /* Flag 0 is 150MHz
       Flag 1 is 200MHz
       Flag 2 is 333MHz
       Flag 3 is 400MHz*/
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regValue;

    do
    {
        if((uint8)CVA_SYS_CLK_REG_400MHZ >= flag)
        {
            /* disable spic clock*/
            CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_CPU_PAGE));
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_CLK_RST_IP_ENABLE, &regValue));
            CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_IP_ENABLE, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LSP, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LEN, (uint32)0u, &regValue));
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_CLK_RST_IP_ENABLE, regValue));

            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_sysclk_set_reg(unit, flag));

            /* enable spic clock*/
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_CLK_RST_IP_ENABLE, &regValue));
            CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_IP_ENABLE, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LSP, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LEN, (uint32)0x1, &regValue));
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_CLK_RST_IP_ENABLE, regValue));
            /* back to the original page.*/
            CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
        }
        else
        {
            retVal = -RT_ERR_INPUT;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to set the baudrate .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] baudrate Baudrate (0x1 to 0xf)\n
 *            The sys clock flag.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_baudrate_set(uint32 unit, uint32 baudrate)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if((uint32)0xF < baudrate)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* switch the page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* Disable SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        /* Set the baud rate*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_BAUDR, baudrate));
        /* Set the baud rate*/
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_FBAUDR, baudrate));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to get the spi speed .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] speed Speed (RTK_SPI_FLASH_100MHz,RTK_SPI_FLASH_83MHz,RTK_SPI_FLASH_50MHz,RTK_SPI_FLASH_33MHz,RTK_SPI_FLASH_12MHz)\n
 *            The spi speed.
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_field_read().
 */
RtkApiRet cva_drv_spi_flash_get_spi_speed(uint32 unit, uint8 *speed)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    baudrate = (uint32)8u;
    uint32    regValue;
    uint32    clk = (uint32)CVA_SYS_CLK_200MHZ;

    do
    {
        /* switch the page. */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* get SPIC baud rate*/
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_BAUDR, &baudrate));
        /* switch the page. */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_CPU_PAGE));
        /* get SPIC clock source */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, (uint32)CVA_CLK_RST_CLOCK_SELECTION, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LSP, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LEN, &regValue));
        /* back to the original page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
        if((uint32)CVA_SYS_CLK_REG_150MHZ == regValue)
        {
            clk = (uint32)CVA_SYS_CLK_150MHZ;
        }
        else if((uint32)CVA_SYS_CLK_REG_200MHZ == regValue)
        {
            clk = (uint32)CVA_SYS_CLK_200MHZ;
        }
        else if((uint32)CVA_SYS_CLK_REG_333MHZ == regValue)
        {
            clk = (uint32)CVA_SYS_CLK_333MHZ;
        }
        else if((uint32)CVA_SYS_CLK_REG_400MHZ == regValue)
        {
            clk = (uint32)CVA_SYS_CLK_400MHZ;
        }
        else
        {
            /* do nothing */
        }

        if((uint32)0u != baudrate)
        {
            *speed = (uint8)((clk / ((uint32)2u * baudrate)) & 0xFFu);
        }
        else
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to select the spi speed for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ch Channel (RTK_SPI_FLASH_SINGLE_CHANNEL,RTK_SPI_FLASH_DUAL_CHANNEL,RTK_SPI_FLASH_QUAD_CHANNEL)\n
 *            The spi channel.
 * @param[in] speed Speed (RTK_SPI_FLASH_100MHz,RTK_SPI_FLASH_83MHz,RTK_SPI_FLASH_50MHz,RTK_SPI_FLASH_33MHz,RTK_SPI_FLASH_12MHz)\n
 *            The spi speed.
 * @param[out]None
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_spi_flash_sel_spi_speed_for_update(),cva_drv_spi_flash_sel_spi_channel_for_update(),cva_drv_spi_flash_set_spi_config_for_update().
 */
RtkApiRet cva_drv_spi_flash_sel_and_set_spi_for_update(uint32 unit, uint8 ch, uint8 speed)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_sel_spi_speed_for_update(unit, speed));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_sel_spi_channel_for_update(unit, ch));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_set_spi_config_for_update(unit));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to select spped for updating .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] speed Speed (RTK_SPI_FLASH_100MHz,RTK_SPI_FLASH_83MHz,RTK_SPI_FLASH_50MHz,RTK_SPI_FLASH_33MHz,RTK_SPI_FLASH_12MHz)\n
 *            The spi speed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 */
RtkApiRet cva_drv_spi_flash_sel_spi_speed_for_update(uint32 unit, uint8 speed)
{
    RtkApiRet retVal = RT_ERR_OK;
    if(((uint8)RTK_SPI_FLASH_100MHz == speed) || ((uint8)RTK_SPI_FLASH_83MHz == speed) || ((uint8)RTK_SPI_FLASH_50MHz == speed) || ((uint8)RTK_SPI_FLASH_33MHz == speed) || ((uint8)RTK_SPI_FLASH_12MHz == speed))
    {
        cva_g_spicClkForUpdate = speed;
    }
    else
    {
        retVal = -RT_ERR_INPUT;
    }
    return retVal;
}
/**
 * @brief The funtion to set baudrate for updating .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read().
 */
static RtkApiRet cva_drv_spi_flash_set_spi_speed_for_update_baud(uint32 unit)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regValue = 0u;
    do
    {
        /* switch the page. */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        /* get SPIC baud rate*/
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_BAUDR, &regValue));
        cva_g_spicBaudrForUpdate = regValue & (uint32)0xFFu;
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to set spped for updating .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_spi_flash_sysclk_set(),cva_drv_spi_flash_baudrate_set(),cva_drv_spi_flash_set_spi_speed_for_update_baud().
 */
static RtkApiRet cva_drv_spi_flash_set_spi_speed_for_update(uint32 unit)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint8     flag     = (uint8)0u;
    uint32    baudrate = (uint32)0u;

    do
    {
        switch(cva_g_spicClkForUpdate)
        {
            case RTK_SPI_FLASH_100MHz:
                flag     = CVA_SYS_CLK_REG_400MHZ;
                baudrate = 0x2u;
                break;
            case RTK_SPI_FLASH_83MHz:
                flag     = CVA_SYS_CLK_REG_333MHZ;
                baudrate = 0x2u;
                break;
            case RTK_SPI_FLASH_50MHz:
                flag     = CVA_SYS_CLK_REG_400MHZ;
                baudrate = 0x4u;
                break;
            case RTK_SPI_FLASH_33MHz:
                flag     = CVA_SYS_CLK_REG_200MHZ;
                baudrate = 0x3u;
                break;
            case RTK_SPI_FLASH_12MHz:
                flag     = CVA_SYS_CLK_REG_200MHZ;
                baudrate = 0x8u;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cva_drv_spi_flash_sysclk_set(unit, flag));
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cva_drv_spi_flash_baudrate_set(unit, baudrate));
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cva_drv_spi_flash_set_spi_speed_for_update_baud(unit));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to select channel for updating .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ch Channel (RTK_SPI_FLASH_SINGLE_CHANNEL,RTK_SPI_FLASH_DUAL_CHANNEL,RTK_SPI_FLASH_QUAD_CHANNEL)\n
 *            The spi channel.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 */
RtkApiRet cva_drv_spi_flash_sel_spi_channel_for_update(uint32 unit, uint8 ch)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(((uint8)RTK_SPI_FLASH_SINGLE_CHANNEL == ch) || ((uint8)RTK_SPI_FLASH_DUAL_CHANNEL == ch) || ((uint8)RTK_SPI_FLASH_QUAD_CHANNEL == ch))
    {
        cva_g_spiChForUpdate = ch;
    }
    else
    {
        retVal = -RT_ERR_INPUT;
    }
    return retVal;
}

/**
 * @brief The funtion to set channel for updating .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ch Channel (RTK_SPI_FLASH_SINGLE_CHANNEL,RTK_SPI_FLASH_DUAL_CHANNEL,RTK_SPI_FLASH_QUAD_CHANNEL)\n
 *            The spi channel.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 */
RtkApiRet cva_drv_spi_flash_set_spi_channel_for_update(uint32 unit)
{
    RtkApiRet retVal                = RT_ERR_OK;
    cva_g_spicModeBitValueForUpdate = (uint8)0xFFu;
    if((uint8)RTK_SPI_FLASH_SINGLE_CHANNEL == cva_g_spiChForUpdate)
    {
        cva_g_spicProgramCmdForUpdate         = (uint8)CVA_FLASH_PP_COM;
        cva_g_spicProgramAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
#if 1
        if((uint8)CVA_SPIC_HIGH_SPEED_TH >= cva_g_spicClkForUpdate)
        {
            cva_g_spicReadCmdForUpdate         = (uint8)CVA_FLASH_READ_COM;
            cva_g_spicReadAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
            cva_g_spicCmdDelayForUpdate        = (uint8)0u;
        }
        else
        {
            cva_g_spicReadCmdForUpdate         = (uint8)CVA_FLASH_FAST_READ_COM;
            cva_g_spicReadAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
            cva_g_spicCmdDelayForUpdate        = (uint8)0x8u;
        }
#else
        cva_g_spicReadCmdForUpdate         = (uint8)CVA_FLASH_FAST_READ_COM;
        cva_g_spicReadAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
        cva_g_spicCmdDelayForUpdate        = (uint8)8u;
#endif
    }
    else if((uint8)RTK_SPI_FLASH_DUAL_CHANNEL == cva_g_spiChForUpdate)
    {
        cva_g_spicProgramCmdForUpdate         = (uint8)CVA_FLASH_PP_COM;
        cva_g_spicProgramAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
        cva_g_spicReadCmdForUpdate            = (uint8)CVA_FLASH_DREAD_COM;
        cva_g_spicReadAddrChannelForUpdate    = (uint8)CVA_SPIC_ADDR_SINGLE;
        cva_g_spicCmdDelayForUpdate           = (uint8)0x8u;
    }
    else if((uint8)RTK_SPI_FLASH_QUAD_CHANNEL == cva_g_spiChForUpdate)
    {
        if((uint32)CVA_FLASH_VENDOR_MXIC == cva_g_flashId)
        {
            cva_g_spicProgramCmdForUpdate         = (uint8)CVA_FLASH_4PP_COM;
            cva_g_spicProgramAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_MULTI;
        }
        else
        {
            /*ISSI/WINBOND/GIGADEVICE*/
            cva_g_spicProgramCmdForUpdate         = (uint8)CVA_FLASH_QPP_COM;
            cva_g_spicProgramAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
        }

        cva_g_spicReadCmdForUpdate         = (uint8)CVA_FLASH_QREAD_COM;
        cva_g_spicReadAddrChannelForUpdate = (uint8)CVA_SPIC_ADDR_SINGLE;
        cva_g_spicCmdDelayForUpdate        = (uint8)0x8u;
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}

/**
 * @brief The funtion to set config for updating .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_write(),cva_reg_page_write(),cva_drv_spi_flash_read_id(),
 *                                cva_drv_spi_flash_set_qe(),cva_drv_spi_flash_set_spi_speed_for_update(),cva_drv_spi_flash_set_spi_channel_for_update().
 */
RtkApiRet cva_drv_spi_flash_set_spi_config_for_update(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {

        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
        /* Disable SPIC */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        /* Set SPIC baud rate */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_BAUDR, (uint32)0x8u));
        /* Set SPIC baud rate */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_FBAUDR, (uint32)0x8u));
        cva_g_spiSysDelay = (uint16)CVA_SPIC_SYSDELAY_DEFAULT;
        /* SPIC delay chain is Initialized to 0*/
        /* Switch the page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_CPU_PAGE));
        /* Set SPIC data delay to 0 */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, (uint32)0u));
        /* Back to the original page */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
        /* Get flash ID */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read_id(unit, &cva_g_flashId));
        cva_g_flashId = cva_g_flashId & (uint32)0xFFu;
        if((uint32)CVA_FLASH_VENDOR_MXIC == cva_g_flashId)
        {
            cva_g_spicRdcrCmd = (uint8)CVA_FLASH_RDCR_COM_FOR_MXIC;
        }
        else
        {
            cva_g_spicRdcrCmd = (uint8)CVA_FLASH_RDCR_COM;
        }
        if(((uint32)CVA_FLASH_VENDOR_MXIC == cva_g_flashId) || ((uint32)CVA_FLASH_VENDOR_ISSI == cva_g_flashId))
        {
            cva_g_spicQeLocation = (uint8)CVA_SPIC_QE_SR1;
        }
        else
        {
            cva_g_spicQeLocation = (uint8)CVA_SPIC_QE_SR2;
        }
        /* Set the QE bit*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_set_qe(unit));
        /* Set SPIC speed */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_set_spi_speed_for_update(unit));
        cva_g_spicClk   = cva_g_spicClkForUpdate;
        cva_g_spicBaudr = cva_g_spicBaudrForUpdate;
        /* Set SPIC channel */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_set_spi_channel_for_update(unit));
        cva_g_spiCh          = cva_g_spiChForUpdate;
        cva_g_spicProgramCmd = cva_g_spicProgramCmdForUpdate;
        cva_g_spicReadCmd    = cva_g_spicReadCmdForUpdate;

        cva_g_spicModeBitValue       = cva_g_spicModeBitValueForUpdate;
        cva_g_spicProgramAddrChannel = cva_g_spicProgramAddrChannelForUpdate;
        cva_g_spicReadAddrChannel    = cva_g_spicReadAddrChannelForUpdate;
        cva_g_spicCmdDelay           = cva_g_spicCmdDelayForUpdate;

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to write section for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fp File pointer (0x0 to 0xffffffff)\n
 *            The file pointer.
 * @param[in] writeSecParam Write section parameter \n
 *            The structure of writing section.
 * @param[out] progress Progress (0x0 to 0xFFFFFFFF)\n
 *            The progress number.
 * @param[out] dividCurr Current divide (0x0 to 0xFFFFFFFF)\n
 *            The current divide
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_write().
 */
static RtkApiRet cva_drv_spi_flash_write_section(uint32 unit, void *fp, cva_SpiWriteSection_t writeSecParam, uint32 *progress, uint32 *dividCurr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     buf[300];
    int32     len;
    uint32    rnd     = (uint32)0u;
    uint32    lenCnt  = (uint32)0u;
    uint32    tmpAddr = writeSecParam.addr;
    do
    {
        if((lenCnt + (uint32)256u) <= writeSecParam.programLen)
        {
            len = rtl_file_read(fp, buf, writeSecParam.fpOffset + ((uint32)256u * rnd), 256u);
            rnd = rnd + (uint32)1u;
        }
        else
        {
            /* the last*/
            len = rtl_file_read(fp, buf, writeSecParam.fpOffset + ((uint32)256u * rnd), (writeSecParam.programLen % 256u));
        }
        if((int32)0 == len)
        {
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write(unit, tmpAddr, buf, (uint32)len));

        lenCnt += (uint32)len;
        tmpAddr += (uint32)len;
        *dividCurr += (uint32)len;
        /* Show progress */
        while(*dividCurr >= writeSecParam.dividBase)
        {
            *progress += (uint32)1u;
            *dividCurr -= writeSecParam.dividBase;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Flash progress: %02d", *progress);
#endif
        }
    } while((RT_ERR_OK == retVal) && ((int32)256 == len));

    return retVal;
}

/**
 * @brief The funtion to write header for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[in] prebuf Data buffer (0x0 to 0xFF)\n
 *            The data buffer.
 * @param[out] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_fill_calibration_header(),cva_drv_spi_flash_write(),cva_reg_page_read,
 *                                cva_reg_page_write(),cva_reg_read(),cva_drv_spi_flash_chip_lock().
 */
static RtkApiRet cva_drv_spi_flash_write_setting_header(uint32 unit, const uint8 sel, const uint8 prebuf[], uint8 flashLayout)
{
    /* length of prebuf must be 96 bytes*/
    RtkApiRet    retVal        = RT_ERR_OK;
    uint8        flashBuf[134] = {(uint8)0u};
    uint32       regVal;
    uint32       i;
    uint32       addr             = 0;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_fill_calibration_header(unit, flashBuf));

        for(i = 0; i < (uint32)118u; i++)
        {
            flashBuf[16u + i] = prebuf[i];
        }

        /* program flash setting + calibration pattern*/
        addr = (uint32)CVA_FLASH_SETTING_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout]);

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write(unit, addr, flashBuf, (uint32)32u));

        /* program flash header (except binary address 0x00020~0x00021) + reservation*/
        addr = (uint32)CVA_FLASH_HEADER_OFFSET + ((uint32)0x2u + ((uint32)sel * fwSizeMapping[flashLayout]));

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write(unit, addr, &flashBuf[34], (uint32)100u));

        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SW_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DMY_REG_0, &regVal));
        regVal = (((regVal & (uint32)0xFFFFFF00u) | (((uint32)sel + (uint32)1u) << 6u)) | ((uint32)0x1u << 5u)) | ((uint32)cva_g_spiSysDelay & (uint32)0x1F);
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DMY_REG_0, regVal));
        /* back to the original page.*/
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));

        /* program binary address 0x00020~0x00021. These data must be written in the last step.*/
        addr = (uint32)CVA_FLASH_HEADER_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout]);

        CHK_FUN_CONTINUE(retVal, (int32)cva_drv_spi_flash_write(unit, addr, &flashBuf[32], (uint32)2u));

        /*lock*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_chip_lock(unit));
        cva_g_lockForUpdate = (uint8)0u;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to calculate crc16 value.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] data Data (0x0 to 0xFF)\n
 *            The data.
 * @param[in] len Length (1 to 4095)\n
 *            The legnth.
 * @param[out] crc16Val Crc value (0x0 to 0xFFFF) \n
 *            The crc value.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_drv_memset().
 */
RtkApiRet cva_drv_spi_flash_crc_16_calculate(uint32 unit, const uint8 *data, uint16 len, uint16 *crc16Val)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint16    initCrc = (uint16)0u;
    uint8     tmp     = (uint8)0u;
    uint16    i       = (uint16)0u;
    uint8     j       = (uint8)0u;
    uint16    ploy    = (uint16)0x1021u;
    uint8     reserveData[CVA_DATALENMAX];
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(reserveData, (int32)0, CVA_DATALENMAX));
        /* check input */
        if((CVA_DATALENMAX < len) || (0u == len))
        {
            continue;
        }

        /*reverse original data byte by byte*/
        for(i = 0u; i < len; i++)
        {
            tmp = 0u;
            for(j = 0u; j < 8u; j++)
            {
                if(0u != (data[i] & ((uint8)1u << (7u - j))))
                {
                    tmp |= ((uint8)1u << j);
                }
            }

            reserveData[i] = (uint8)(tmp & 0xFFu);
        }
        /* calculate crc */
        for(i = 0u; i < len; i++)
        {
            initCrc ^= ((uint16)reserveData[i] << 8u);
            for(j = 0u; j < 8u; j++)
            {
                if(1u == ((initCrc >> 15u) & 0x1u))
                {
                    initCrc = (uint16)(((((uint16)initCrc << (uint16)1u) & 0xFFFFu) ^ (ploy & 0xFFFFu)) & 0xFFFFu);
                }
                else
                {
                    initCrc <<= 1u;
                }
            }
        }
        /* reverse key */
        for(j = 0u; j < 16u; j++)
        {
            if(0u != (initCrc & ((uint16)1u << (15u - j))))
            {
                *crc16Val |= ((uint16)1u << j);
            }
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check version.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fp File pointer (0x0 to 0xffffffff)\n
 *            The file pointer.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_NOT_ALLOWED
 * @retval -RT_ERR_CHIP_NOT_SUPPORTED
 * @retval -RT_ERR_IC_PART_NUM
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),rtl906x_otp_read().
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_check_ver(uint32 unit, void *fp)
{
    RtkApiRet retVal = RT_ERR_OK;
    int32     len;
    uint8     imgModel     = (uint8)0u;
    uint8     imgIcVersion = (uint8)0u;
    uint8     imgIcPartNum = (uint8)0u;
    uint32    regVal       = (uint32)0u;
    uint8     icPartNum    = (uint8)0u;
    do
    {
        /*get reserve area byte 14,
        bit[2:0] is ASIC version
        bit[6:3] is IC part number*/
        len = rtl_file_read(fp, &imgModel, ((uint32)CVA_FLASH_RESERVE_AREA_OFFSET + (uint32)0xEu), 1u);
        if((int32)0 == len)
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        imgIcVersion = imgModel & (uint8)0x7u;
        imgIcPartNum = (imgModel >> 3u) & (uint8)0xFu;

        /* Check image version 0x1=VA*/
        if((uint8)0x1u != imgIcVersion)
        {
            retVal = -RT_ERR_NOT_ALLOWED;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "The image is inconsistent with SDK version.");
#endif
            continue;
        }
        /* Check ASIC version */
        /* change page 0x4a*/
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SW_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_CHIP_VERSION_ID, &regVal));
        /* change SPIC page*/
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        if((uint32)CVA_FLASH_CHIP_VERSION != (regVal & 0xFFFFF000u))
        {
            retVal = -RT_ERR_CHIP_NOT_SUPPORTED;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "The IC version is inconsistent with SDK version.");
#endif
            continue;
        }

        /* Check IC part number*/
        /* change page 0x4A*/
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SW_PAGE));
        retVal = cva_otp_read(unit, (uint32)0x166u, &icPartNum, (uint32)1u);
        if(RT_ERR_OK != retVal)
        {

#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Failed to read otp data.");
#endif
            retVal = -RT_ERR_IC_PART_NUM;
            continue;
        }
        icPartNum = icPartNum & (uint8)0xFu;
        /* change SPIC page*/
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        if((uint8)CVA_IC_PART_RTL9068CL >= imgIcPartNum)
        {
            if(imgIcPartNum != icPartNum)
            {
                retVal = -RT_ERR_INPUT;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "The image is inconsistent with IC part number.");
#endif
                continue;
            }
        }
        else
        {
            retVal = -RT_ERR_INPUT;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "The image is inconsistent with IC part number.");
#endif
            continue;
        }

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check image validity.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateMode Update mode (CVA_FLASH_FORCE_UPDATE_ALL,CVA_FLASH_UPDATE_PARTIAL)\n
 *            The update mode.
 * @param[in] fwLen Firmware length (0x1 to 0xFFFE0) \n
 *            The firmware length.
 * @param[in] configLen Config length (0x1 to 0xFFFE0) \n
 *            The config length.
 * @param[in] prebuf Header buffer (0x0 to 0xFF)\n
 *            The header pointer.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_crc_16_calculate().
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_check_valid(uint32 unit, uint8 updateMode, uint32 fwLen, uint32 configLen, const uint8 prebuf[])
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint16    crcVal    = (uint16)0u;
    uint16    crcValBin = (uint16)0u;
    do
    {
        if(CVA_FLASH_FORCE_UPDATE_ALL == updateMode)
        {
            if(((uint32)0xffff0u == fwLen) || ((uint32)0xffff0u == configLen))
            {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "No fw and config\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
        }
        if(((uint8)0x99u != prebuf[0x10]) || ((uint8)0x88u != prebuf[0x11]))
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Image not valid\n");
#endif
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /*Calculate CRC*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_crc_16_calculate(unit, &prebuf[0x10], (uint16)62u, &crcVal));
        crcValBin = (uint16)(((uint16)prebuf[0x4E] & 0xFFu) | (((uint16)prebuf[0x4F] << 8u) & 0xFF00u));
        if(crcValBin != crcVal)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Image CRC check failed crcValBin : 0x%x, crcVal : 0x%x\n", crcValBin, crcVal);
#endif
            retVal = -RT_ERR_INPUT;
            continue;
        }

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The funtion to get the information of zone A is exist or not.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] prebuf Header buffer (0x0 to 0xFF)\n
 *            The header buffer.
 * @param[in] updateMask Update Mask \n
 *            The update mask.
 * @param[out] None
 * @retval RT_ERR_OK
 */
static RtkApiRet cva_drv_spi_flash_update_check_zone_a(uint32 unit, uint8 *prebuf, uint32 updateMask, uint8 ethMode)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint8     fwIdentify[3] = {0u};
    uint16    headerIdent   = 0u;

    do
    {
        if(CVA_SPI_UPDATE_REGIF == ethMode)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, CVA_FLASH_HEADER_OFFSET, fwIdentify, 3u));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_read(CVA_FLASH_HEADER_OFFSET, 3u, fwIdentify));
        }
        headerIdent = ((uint16)fwIdentify[0] & 0xFFu) | (((uint16)fwIdentify[1] << 8) & 0xFF00u);
        if((CVA_FLASH_HEADE_IDENTIFY == headerIdent) && (0x0u == (fwIdentify[2] & 0x3u)))
        {
            /*set fw status bit = 1 if zoneA exist*/
            if(0x0u != ((updateMask >> 0u) & (uint32)0x1u))
            {
                prebuf[0x12] |= (uint8)0x1;
            }
            /*set config status bit = 1 if zoneA exist*/
            if(0x0u != ((updateMask >> 1u) & (uint32)0x1u))
            {
                prebuf[0x12] |= (uint8)0x2;
            }
            /*set macsec status bit = 1 if zoneA exist*/
            if(0x0u != ((updateMask >> 5u) & (uint32)0x1u))
            {
                prebuf[0x13] |= (uint8)0x10;
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to get the infromation of updaing block.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] prebuf Header buffer (0x0 to 0xFF)\n
 *            The header buffer.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[out] dividBase Divide base (0x0 to 0xFFFFFFFF)\n
 *            The base divide .
 * @retval RT_ERR_OK
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_get_update_block(uint32 unit, uint8 *prebuf, cva_SpiFlashUpdate_t *updateParam, uint32 *dividBase)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /*Need update fw*/
        if(((uint32)0xffff0u != updateParam->fwLen) && ((uint32)0x0u != updateParam->fwLen))
        {
            updateParam->fwAddr    = ((uint32)prebuf[0x14]) | ((uint32)prebuf[0x15] << 8u) | ((uint32)prebuf[0x16] << 16u);
            updateParam->fwAddrOri = updateParam->fwAddr;
            *dividBase += updateParam->fwLen;
            updateParam->updateMask |= (uint32)1u << 0u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update fw\n");
#endif
        }
        /*Need update config*/
        if(((uint32)0xffff0u != updateParam->configLen) && ((uint32)0x0u != updateParam->configLen))
        {
            updateParam->configAddr    = ((uint32)prebuf[0x19]) | ((uint32)prebuf[0x1a] << 8u) | ((uint32)prebuf[0x1b] << 16u);
            updateParam->configAddrOri = updateParam->configAddr;
            *dividBase += updateParam->configLen;
            updateParam->updateMask |= (uint32)1u << 1u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update config\n");
#endif
        }
        /*Need update stk0*/
        if(((uint32)0xffff0u != updateParam->stk0Len) && ((uint32)0x0u != updateParam->stk0Len))
        {
            updateParam->stk0Addr    = ((uint32)prebuf[0x1e]) | ((uint32)prebuf[0x1f] << 8u) | ((uint32)prebuf[0x20] << 16u);
            updateParam->stk0AddrOri = updateParam->stk0Addr;
            *dividBase += updateParam->stk0Len;
            updateParam->updateMask |= (uint32)1u << 2u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update stk0\n");
#endif
        }
        /*Need update stk1*/
        if(((uint32)0xffff0u != updateParam->stk1Len) && ((uint32)0x0u != updateParam->stk1Len))
        {
            updateParam->stk1Addr    = ((uint32)prebuf[0x23]) | ((uint32)prebuf[0x24] << 8u) | ((uint32)prebuf[0x25] << 16u);
            updateParam->stk1AddrOri = updateParam->stk1Addr;
            *dividBase += updateParam->stk1Len;
            updateParam->updateMask |= (uint32)1u << 3u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update stk1\n");
#endif
        }
        /*Need update stk2*/
        if(((uint32)0xffff0u != updateParam->stk2Len) && ((uint32)0x0u != updateParam->stk2Len))
        {
            updateParam->stk2Addr    = ((uint32)prebuf[0x28]) | ((uint32)prebuf[0x29] << 8u) | ((uint32)prebuf[0x2a] << 16u);
            updateParam->stk2AddrOri = updateParam->stk2Addr;
            *dividBase += updateParam->stk2Len;
            updateParam->updateMask |= (uint32)1u << 4u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update stk2\n");
#endif
        }
        /*Need update macsec*/
        if(((uint32)0xffff0u != updateParam->macsecLen) && ((uint32)0x0u != updateParam->macsecLen))
        {
            updateParam->macsecAddr    = ((uint32)prebuf[0x2d]) | ((uint32)prebuf[0x2e] << 8u) | ((uint32)prebuf[0x2f] << 16u);
            updateParam->macsecAddrOri = updateParam->macsecAddr;
            *dividBase += updateParam->macsecLen;
            updateParam->updateMask |= (uint32)1u << 5u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update macsec\n");
#endif
        }

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_check_zone_a(unit, prebuf, updateParam->updateMask, CVA_SPI_UPDATE_REGIF));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The funtion to check the image for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[out] dividBase Divide base (0x0 to 0xFFFFFFFF)\n
 *            The base divide .
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 */
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_fw(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /*Need update fw*/
        if((uint32)0x1u == (updateParam.updateMask & (uint32)0x1u))
        {
            /*check if fw exist*/
            if((uint32)0xffff0u == updateParam.fwLenOri)
            {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "fw not exist\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
            /*config exist*/
            if((uint32)0xffff0u != updateParam.configLenOri)
            {
                /*check if overlap to config*/
                if((uint32)(updateParam.fwAddrOri + updateParam.fwLen) > updateParam.configAddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "fw overlap to config\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else
            {
                /*config must exist in original flash for partial update*/
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "fw overlap to zone\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
        }

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check macsec for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 */
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_macsec(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*Need update macsec*/
        if((uint32)0x1u == ((updateParam.updateMask >> 5u) & (uint32)0x1u))
        {
            /*check if macsec exist*/
            if((uint32)0xffff0u == updateParam.macsecLenOri)
            {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "macsec not exist\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
            /*check if exceed the range*/
            if((uint32)(updateParam.macsecAddrOri + updateParam.macsecLen) > fwSizeMapping[flashLayout])
            {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "macsec overlap to zone\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check stacking unit 2 for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_update_get_update_partial_check_macsec().
 */
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config_stk2(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*Need update stk2*/
        if((uint32)0x1u == ((updateParam.updateMask >> 4u) & (uint32)0x1u))
        {
            /*macsec exist*/
            if((uint32)0xffff0u != updateParam.macsecLenOri)
            {
                /*check if overlap to macsec*/
                if((uint32)(updateParam.stk2AddrOri + updateParam.stk2Len) > updateParam.macsecAddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk2 overlap to macsec\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else
            {
                /*check if exceed the rage*/
                if((uint32)(updateParam.stk2AddrOri + updateParam.stk2Len) > fwSizeMapping[flashLayout])
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk2 overlap to zone\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check_macsec(unit, updateParam, flashLayout));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check stacking unit 1 for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_update_get_update_partial_check_config_stk2().
 */
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config_stk1(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*Need update stk1*/
        if((uint32)0x1u == ((updateParam.updateMask >> 3u) & (uint32)0x1u))
        {
            /*check if stk1 exist*/
            if((uint32)0xffff0u == updateParam.stk1LenOri)
            {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "stk1 not exist\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
            /*stk2 exist*/
            if((uint32)0xffff0u != updateParam.stk2LenOri)
            {
                /*check if overlap to stk2*/
                if((uint32)(updateParam.stk1AddrOri + updateParam.stk1Len) > updateParam.stk2AddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk1 overlap to stk2\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else if((uint32)0xffff0u != updateParam.macsecLenOri)
            {
                /*check if overlap to macsec*/
                if((uint32)(updateParam.stk1AddrOri + updateParam.stk1Len) > updateParam.macsecAddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk1 overlap to macsec\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else
            {
                /*check if exceed the range*/
                if((uint32)(updateParam.stk1AddrOri + updateParam.stk1Len) > fwSizeMapping[flashLayout])
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk1 overlap to zone\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check_config_stk2(unit, updateParam, flashLayout));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check config for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_update_get_update_partial_check_config_stk1().
 */
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config_stk(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*Need update stk0*/
        if((uint32)0x1u == ((updateParam.updateMask >> 2u) & (uint32)0x1u))
        {
            /*check if stk0 exist*/
            if((uint32)0xffff0u == updateParam.stk0LenOri)
            {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "stk0 not exist\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
            /*stk1 exist*/
            if((uint32)0xffff0u != updateParam.stk1LenOri)
            {
                /*check if overlap to stk1*/
                if((uint32)(updateParam.stk0AddrOri + updateParam.stk0Len) > updateParam.stk1AddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk0 overlap to stk1\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else if((uint32)0xffff0u != updateParam.macsecLenOri)
            {
                /*check if overlap to macsec*/
                if((uint32)(updateParam.stk0AddrOri + updateParam.stk0Len) > updateParam.macsecAddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk0 overlap to macsec\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else
            {
                /*check if exceed the range*/
                if((uint32)(updateParam.stk0AddrOri + updateParam.stk0Len) > fwSizeMapping[flashLayout])
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "stk0 overlap to zone\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check_config_stk1(unit, updateParam, flashLayout));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check config for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_update_get_update_partial_check_config_stk().
 */
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_config(uint32 unit, cva_SpiFlashUpdate_t updateParam, uint8 flashLayout)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*Need update config*/
        if((uint32)0x1u == ((updateParam.updateMask >> 1u) & (uint32)0x1u))
        {
            /*check if config exist*/
            if((uint32)0xffff0u == updateParam.configLenOri)
            {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                DEBUG_INFO(DBG_MEMCTRL, "config not to exist\n");
#endif
                retVal = -RT_ERR_INPUT;
                continue;
            }
            /*stk0 exist*/
            if((uint32)0xffff0u != updateParam.stk0LenOri)
            {
                /*check if overlap to stk0*/
                if((uint32)(updateParam.configAddrOri + updateParam.configLen) > updateParam.stk0AddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "config overlap to stk0\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else if((uint32)0xffff0u != updateParam.macsecLenOri)
            {
                /*check if overlap to macsec*/
                if((uint32)(updateParam.configAddrOri + updateParam.configLen) > updateParam.macsecAddrOri)
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "config overlap to macsec\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
            else
            {
                /*check if exceed the range*/
                if((uint32)(updateParam.configAddrOri + updateParam.configLen) > fwSizeMapping[flashLayout])
                {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
                    DEBUG_INFO(DBG_MEMCTRL, "config overlap to zone\n");
#endif
                    retVal = -RT_ERR_INPUT;
                    continue;
                }
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check_config_stk(unit, updateParam, flashLayout));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to set block length for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] readbuf Header buffer (0x0 to 0xFF) \n
 *            The header buffer in flash.
 * @param[out] prebuf Header buffer (0x0 to 0xFF)\n
 *             The header buffer to update.
 * @retval RT_ERR_OK
 */
static RtkApiRet cva_drv_spi_flash_update_get_update_partial_check_set_length(uint32 unit, uint8 *prebuf, uint8 *readbuf, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        if((uint32)0x1u != (updateParam.updateMask & (uint32)0x1u))
        {
            /*write fw length back if no need updated*/
            prebuf[0x17] = readbuf[0x5];
            prebuf[0x18] = readbuf[0x6];
        }
        if((uint32)0x1u != ((updateParam.updateMask >> 1u) & (uint32)0x1u))
        {
            /*write config length back if no need updated*/
            prebuf[0x1c] = readbuf[0xa];
            prebuf[0x1d] = readbuf[0xb];
        }
        if((uint32)0x1u != ((updateParam.updateMask >> 2u) & (uint32)0x1u))
        {
            /*write stk0 length back if no need updated*/
            prebuf[0x21] = readbuf[0xf];
            prebuf[0x22] = readbuf[0x10];
        }
        if((uint32)0x1u != ((updateParam.updateMask >> 3u) & (uint32)0x1u))
        {
            /*write stk1 length back if no need updated*/
            prebuf[0x26] = readbuf[0x14];
            prebuf[0x27] = readbuf[0x15];
        }
        if((uint32)0x1u != ((updateParam.updateMask >> 4u) & (uint32)0x1u))
        {
            /*write stk2 length back if no need updated*/
            prebuf[0x2b] = readbuf[0x19];
            prebuf[0x2c] = readbuf[0x1a];
        }
        if((uint32)0x1u != ((updateParam.updateMask >> 5u) & (uint32)0x1u))
        {
            /*write macsec length back if no need updated*/
            prebuf[0x30] = readbuf[0x1e];
            prebuf[0x31] = readbuf[0x1f];
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to check image for partial updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[out] prebuf Header buffer (0x0 to 0xFF)\n
 *             The header buffer to update.
 * @param[out] updateParam Update parameter \n
 *            The structure of updating.
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_read(),cva_drv_spi_flash_update_get_update_partial_check_fw(),
 *                                cva_drv_spi_flash_update_get_update_partial_check_config(),cva_drv_spi_flash_update_get_update_partial_check_set_length().
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_get_update_partial_check(uint32 unit, uint8 *prebuf, uint8 sel, cva_SpiFlashUpdate_t *updateParam, uint8 flashLayout, uint8 ethMode)
{
    RtkApiRet    retVal = RT_ERR_OK;
    uint8        readbuf[32];
    uint8        readFlashLayout  = (uint8)0u;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*Read flash header 0x22~0x41*/
        if(CVA_SPI_UPDATE_REGIF == ethMode)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_read(unit, ((uint32)CVA_FLASH_HEADER_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout])) + (uint32)2u, readbuf, (uint32)32u));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_read(((uint32)CVA_FLASH_HEADER_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout])) + (uint32)2u, (uint32)32u, readbuf));
        }
        readFlashLayout = readbuf[0x1] & (uint8)0x7u;
        if(flashLayout != readFlashLayout)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        updateParam->fwAddrOri = ((uint32)readbuf[0x2]) | ((uint32)readbuf[0x3] << 8u) | ((uint32)readbuf[0x4] << 16u);
        updateParam->fwLenOri  = ((uint32)readbuf[0x5] << 4u) + ((uint32)readbuf[0x6] << 12u);

        updateParam->configAddrOri = ((uint32)readbuf[0x7]) | ((uint32)readbuf[0x8] << 8u) | ((uint32)readbuf[0x9] << 16u);
        updateParam->configLenOri  = ((uint32)readbuf[0xa] << 4u) + ((uint32)readbuf[0xb] << 12u);

        updateParam->stk0AddrOri = ((uint32)readbuf[0xc]) | ((uint32)readbuf[0xd] << 8u) | ((uint32)readbuf[0xe] << 16u);
        updateParam->stk0LenOri  = ((uint32)readbuf[0xf] << 4u) + ((uint32)readbuf[0x10] << 12u);

        updateParam->stk1AddrOri = ((uint32)readbuf[0x11]) | ((uint32)readbuf[0x12] << 8u) | ((uint32)readbuf[0x13] << 16u);
        updateParam->stk1LenOri  = ((uint32)readbuf[0x14] << 4u) + ((uint32)readbuf[0x15] << 12u);

        updateParam->stk2AddrOri = ((uint32)readbuf[0x16]) | ((uint32)readbuf[0x17] << 8u) | ((uint32)readbuf[0x18] << 16u);
        updateParam->stk2LenOri  = ((uint32)readbuf[0x19] << 4u) + ((uint32)readbuf[0x1a] << 12u);

        updateParam->macsecAddrOri = ((uint32)readbuf[0x1b]) | ((uint32)readbuf[0x1c] << 8u) | ((uint32)readbuf[0x1d] << 16u);
        updateParam->macsecLenOri  = ((uint32)readbuf[0x1e] << 4u) + ((uint32)readbuf[0x1f] << 12u);

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check_fw(unit, *updateParam, flashLayout));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check_config(unit, *updateParam, flashLayout));
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check_set_length(unit, prebuf, readbuf, *updateParam));
        /*write fw address back*/
        prebuf[0x14] = readbuf[0x2];
        prebuf[0x15] = readbuf[0x3];
        prebuf[0x16] = readbuf[0x4];
        /*write config address back*/
        prebuf[0x19] = readbuf[0x7];
        prebuf[0x1a] = readbuf[0x8];
        prebuf[0x1b] = readbuf[0x9];
        /*write stk0 address back*/
        prebuf[0x1e] = readbuf[0xc];
        prebuf[0x1f] = readbuf[0xd];
        prebuf[0x20] = readbuf[0xe];
        /*write stk1 address back*/
        prebuf[0x23] = readbuf[0x11];
        prebuf[0x24] = readbuf[0x12];
        prebuf[0x25] = readbuf[0x13];
        /*write stk2 address back*/
        prebuf[0x28] = readbuf[0x16];
        prebuf[0x29] = readbuf[0x17];
        prebuf[0x2a] = readbuf[0x18];
        /*write macsec address back*/
        prebuf[0x2d] = readbuf[0x1b];
        prebuf[0x2e] = readbuf[0x1c];
        prebuf[0x2f] = readbuf[0x1d];

        /*update ready bits*/
        prebuf[0x12] |= (readbuf[0] & (uint8)0x1);
        prebuf[0x12] |= (readbuf[0] & (uint8)0x2);
        prebuf[0x13] |= (readbuf[1] & (uint8)0x10);
        /*If only partial update fw, keep disable quick serach bit*/
        if(0x1u == (updateParam->updateMask & 0x3u))
        {
            prebuf[0x13] |= (readbuf[1] & (uint8)0x8);
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The sub funtion to erase for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] len Length  (0x1 to 0xffffff) \n
 *            The erase length.
 * @param[in] addr Address (0x0 to 0xffffff)\n
 *            The erase address.
 * @param[in] eraseFlag Erase flag (CVA_SPIC_CHIP_ERASE, CVA_SPIC_SECTOR_ERASE, CVA_SPIC_BLK64K_ERASE)\n
 *             The erase flag.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_INPUT
 * @retval Others Please refer to cva_drv_spi_flash_erase().
 */
static RtkApiRet cva_drv_spi_flash_update_erase_sub_function(uint32 unit, uint32 len, uint32 addr, uint8 eraseFlag)
{
    uint32    eraseNum  = (uint32)0u;
    uint32    eraseSize = CVA_FLASH_4K;
    uint32    i         = 0u;
    RtkApiRet retVal    = RT_ERR_OK;
    do
    {
        if((uint8)CVA_SPIC_BLK64K_ERASE == eraseFlag)
        {
            eraseSize = CVA_FLASH_64K;
        }
        else if((uint8)CVA_SPIC_SECTOR_ERASE == eraseFlag)
        {
            eraseSize = CVA_FLASH_4K;
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)1u > len)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        eraseNum = ((len - (uint32)1u) / ((uint32)eraseSize)) + (uint32)1u;
        for(i = 0u; i < eraseNum; i++)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase(unit, (addr + (i * eraseSize)), eraseFlag));
        }
        if(retVal != RT_ERR_OK)
        {
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The funtion to erase firmware and config for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_update_erase_sub_function().
 */
static RtkApiRet cva_drv_spi_flash_update_erase_fw_config(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase FW*/
        if((uint32)0x1u == (updateParam.updateMask & (uint32)0x1u))
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.fwLen, ((uint32)updateParam.fwAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
        }
        /* need erase Config*/
        if((uint32)0x1u == ((updateParam.updateMask >> 1u) & (uint32)0x1u))
        {
            /*If no stk0 and have macsec for partial update, use sector erase config block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u == updateParam.stk0LenOri) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.configLen, ((uint32)updateParam.configAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.configLen, ((uint32)updateParam.configAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to erase stacking unit 2 for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_update_erase_sub_function().
 */
static RtkApiRet cva_drv_spi_flash_update_erase_stk2(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* erase stk2*/
        if((uint32)0x1u == ((updateParam.updateMask >> 4u) & (uint32)0x1u))
        {
            /*If have macsec for partial update, use sector erase stk2 block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                /*use sector erase to erase stk2*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.stk2Len, ((uint32)updateParam.stk2AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                /*use block erase to erase stk2*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.stk2Len, ((uint32)updateParam.stk2AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to erase stacking unit 1 for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_update_erase_sub_function(),cva_drv_spi_flash_update_erase_stk2().
 */
static RtkApiRet cva_drv_spi_flash_update_erase_stk1(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase stk1*/
        if((uint32)0x1u == ((updateParam.updateMask >> 3u) & (uint32)0x1u))
        {
            /*If no stk2 and have macsec for partial update, use sector erase stk1 block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u == updateParam.stk2LenOri) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                /*use sector erase to erase stk1*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.stk1Len, ((uint32)updateParam.stk1AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                /*use block erase to erase stk1*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.stk1Len, ((uint32)updateParam.stk1AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_stk2(unit, sel, flashLayout, updateParam))

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to erase stacking unit for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_update_erase_sub_function(),cva_drv_spi_flash_update_erase_stk1().
 */
static RtkApiRet cva_drv_spi_flash_update_erase_stk(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase stk0*/
        if((uint32)0x1u == ((updateParam.updateMask >> 2u) & (uint32)0x1u))
        {
            /*If no stk1 and have macsec for partial update, use sector erase config block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u == updateParam.stk1LenOri) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                /*use sector erase to erase stk0*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.stk0Len, ((uint32)updateParam.stk0AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                /*use block erase to erase stk0*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.stk0Len, ((uint32)updateParam.stk0AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_stk1(unit, sel, flashLayout, updateParam));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The funtion to erase macsec for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_update_erase_sub_function().
 */
static RtkApiRet cva_drv_spi_flash_update_erase_macsec(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase macsec*/
        if((uint32)0x1u == ((updateParam.updateMask >> 5u) & (uint32)0x1u))
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function(unit, updateParam.macsecLen, ((uint32)updateParam.macsecAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to erase for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B)\n
 *            The Zone select.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_chip_unlock(),cva_drv_spi_flash_erase(),cva_drv_spi_flash_update_erase_fw_config(),
 *                                cva_drv_spi_flash_update_erase_stk(),cva_drv_spi_flash_update_erase_macsec().
 */
DYNAMIC_SCOPE RtkApiRet cva_drv_spi_flash_update_erase(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*unlock*/
        cva_g_lockForUpdate = (uint8)1u;
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_chip_unlock(unit));
        /*erase header*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase(unit, ((uint32)CVA_FLASH_SETTING_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE)); /* Flash header must be erased first before update flash */
        /*erase fw and config*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_fw_config(unit, sel, flashLayout, updateParam));
        /*erase stk*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_stk(unit, sel, flashLayout, updateParam));
        /*erase macsec*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_macsec(unit, sel, flashLayout, updateParam));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief The funtion to program for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fp File pointer (0x0 to 0xffffffff) \n
 *            The file pointer.
 * @param[in] sel Zone select (CVA_FLASH_ZONE_A,CVA_FLASH_ZONE_B) \n
 *            The Zone select.
 * @param[in] flashLayout Flash layout (0 to 7) \n
 *            The flash layout.
 * @param[in] updateParam Update parameter \n
 *            The structure of updating.
 * @param[in] dividBase Divide base (0x0 to 0xFFFFFFFF)\n
 *            The base divide .
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_drv_memset(),cva_drv_spi_flash_write_section().
 */
static RtkApiRet cva_drv_spi_flash_update_program(uint32 unit, void *fp, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam, const uint32 dividBase)
{
    RtkApiRet             retVal    = RT_ERR_OK;
    uint32                progress  = (uint32)0u;
    uint32                dividCurr = (uint32)0u;
    cva_SpiWriteSection_t writeSecParam;
    const uint32          fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                              (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                              (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(&writeSecParam, (int32)0, sizeof(writeSecParam)));
        dividCurr               = (uint32)0u;
        writeSecParam.dividBase = dividBase;

        /* update  fw */
        if((uint32)0x1u == ((updateParam.updateMask >> 0u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.fwAddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.fwAddr;
            writeSecParam.programLen = updateParam.fwLen;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section(unit, fp, writeSecParam, &progress, &dividCurr));
        }
        /* update  config */
        if((uint32)0x1u == ((updateParam.updateMask >> 1u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.configAddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.configAddr;
            writeSecParam.programLen = updateParam.configLen;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section(unit, fp, writeSecParam, &progress, &dividCurr));
        }
        /* update  stk0*/
        if((uint32)0x1u == ((updateParam.updateMask >> 2u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.stk0AddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.stk0Addr;
            writeSecParam.programLen = updateParam.stk0Len;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section(unit, fp, writeSecParam, &progress, &dividCurr));
        }
        /* update  stk1*/
        if((uint32)0x1u == ((updateParam.updateMask >> 3u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.stk1AddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.stk1Addr;
            writeSecParam.programLen = updateParam.stk1Len;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section(unit, fp, writeSecParam, &progress, &dividCurr));
        }
        /* update  stk2*/
        if((uint32)0x1u == ((updateParam.updateMask >> 4u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.stk2AddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.stk2Addr;
            writeSecParam.programLen = updateParam.stk2Len;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section(unit, fp, writeSecParam, &progress, &dividCurr));
        }
        /* update  macsec*/
        if((uint32)0x1u == ((updateParam.updateMask >> 5u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.macsecAddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.macsecAddr;
            writeSecParam.programLen = updateParam.macsecLen;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section(unit, fp, writeSecParam, &progress, &dividCurr));
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion for updating.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The image file name to input.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval Others Please refer to cva_drv_drv_memset(),rtl_file_open(),cva_drv_spi_flash_update_check_valid(),
 *                                cva_drv_spi_flash_update_get_update_block(),cva_drv_spi_flash_update_get_update_partial_check(),
 *                                cva_drv_spi_flash_crc_16_calculate(),cva_drv_spi_flash_get_lock_otp_enable(),cva_drv_spi_flash_update_erase(),
 *                                cva_drv_spi_flash_update_program(),cva_drv_spi_flash_write_setting_header().
 */
RtkApiRet cva_drv_spi_flash_update(uint32 unit, const char *fileName)
{
    void *fp = NULL;

    RtkApiRet            retVal = RT_ERR_OK;
    uint8                prebuf[118];
    cva_SpiFlashUpdate_t updateParam;
    int32                len;
    uint16               crcVal      = (uint16)0u;
    uint32               dividBase   = (uint32)0u;
    uint8                sel         = (uint8)CVA_FLASH_ZONE_B;
    uint8                flashLayout = (uint8)0u;

    do
    {

        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(&updateParam, 0, sizeof(updateParam)));
        CHK_FUN_CONTINUE(retVal, rtl_file_open(&fp, 0, fileName));
#if 1
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_check_ver(unit, fp));
#endif
        /*flash calibration pattern + header + reservation*/
        len = rtl_file_read(fp, prebuf, (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET, 118u);
        if((int32)0 == len)
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /*get flash layout*/
        flashLayout = prebuf[0x13] & (uint8)0x7u;

        /*get force update all bit*/
        updateParam.updateMode = (prebuf[0x13] >> 6u) & (uint8)0x1u;

        /*get calculate length*/
        updateParam.fwLen     = ((uint32)prebuf[0x17] << 4u) + ((uint32)prebuf[0x18] << 12u);
        updateParam.configLen = ((uint32)prebuf[0x1c] << 4u) + ((uint32)prebuf[0x1d] << 12u);
        updateParam.stk0Len   = ((uint32)prebuf[0x21] << 4u) + ((uint32)prebuf[0x22] << 12u);
        updateParam.stk1Len   = ((uint32)prebuf[0x26] << 4u) + ((uint32)prebuf[0x27] << 12u);
        updateParam.stk2Len   = ((uint32)prebuf[0x2b] << 4u) + ((uint32)prebuf[0x2c] << 12u);
        updateParam.macsecLen = ((uint32)prebuf[0x30] << 4u) + ((uint32)prebuf[0x31] << 12u);
        /*Check image valid*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_check_valid(unit, updateParam.updateMode, updateParam.fwLen, updateParam.configLen, prebuf));
        /*Get update block information*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_block(unit, prebuf, &updateParam, &dividBase));

        /*Upgrade mode is partial*/
        if(CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check(unit, prebuf, sel, &updateParam, flashLayout, CVA_SPI_UPDATE_REGIF));
        }

        dividBase /= (uint32)100u;

        /*Calculate CRC*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_crc_16_calculate(unit, &prebuf[0x10], (uint16)62u, &crcVal));
        prebuf[0x4e] = (uint8)(crcVal & (uint16)0xffu);
        prebuf[0x4f] = (uint8)((crcVal >> 8u) & (uint16)0xffu);

        /*get lock support from otp*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_get_lock_otp_enable(unit));

        /*do erase*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase(unit, sel, flashLayout, updateParam));
        /*do program*/

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_program(unit, fp, sel, flashLayout, updateParam, dividBase));

        /* program flash setting*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_setting_header(unit, sel, prebuf, flashLayout));

    } while(0u == 1u);
    if(NULL != fp)
    {
        rtl_file_close(&fp);
    }
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_get_update_block_for_ethernet(uint32 unit, uint8 *prebuf, cva_SpiFlashUpdate_t *updateParam)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /*Need update fw*/
        if(((uint32)0xffff0u != updateParam->fwLen) && ((uint32)0x0u != updateParam->fwLen))
        {
            updateParam->fwAddr    = ((uint32)prebuf[0x14]) | ((uint32)prebuf[0x15] << 8u) | ((uint32)prebuf[0x16] << 16u);
            updateParam->fwAddrOri = updateParam->fwAddr;
            updateParam->updateMask |= (uint32)1u << 0u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update fw\n");
#endif
        }
        /*Need update config*/
        if(((uint32)0xffff0u != updateParam->configLen) && ((uint32)0x0u != updateParam->configLen))
        {
            updateParam->configAddr    = ((uint32)prebuf[0x19]) | ((uint32)prebuf[0x1a] << 8u) | ((uint32)prebuf[0x1b] << 16u);
            updateParam->configAddrOri = updateParam->configAddr;
            updateParam->updateMask |= (uint32)1u << 1u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update config\n");
#endif
        }
        /*Need update stk0*/
        if(((uint32)0xffff0u != updateParam->stk0Len) && ((uint32)0x0u != updateParam->stk0Len))
        {
            updateParam->stk0Addr    = ((uint32)prebuf[0x1e]) | ((uint32)prebuf[0x1f] << 8u) | ((uint32)prebuf[0x20] << 16u);
            updateParam->stk0AddrOri = updateParam->stk0Addr;
            updateParam->updateMask |= (uint32)1u << 2u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update stk0\n");
#endif
        }
        /*Need update stk1*/
        if(((uint32)0xffff0u != updateParam->stk1Len) && ((uint32)0x0u != updateParam->stk1Len))
        {
            updateParam->stk1Addr    = ((uint32)prebuf[0x23]) | ((uint32)prebuf[0x24] << 8u) | ((uint32)prebuf[0x25] << 16u);
            updateParam->stk1AddrOri = updateParam->stk1Addr;
            updateParam->updateMask |= (uint32)1u << 3u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update stk1\n");
#endif
        }
        /*Need update stk2*/
        if(((uint32)0xffff0u != updateParam->stk2Len) && ((uint32)0x0u != updateParam->stk2Len))
        {
            updateParam->stk2Addr    = ((uint32)prebuf[0x28]) | ((uint32)prebuf[0x29] << 8u) | ((uint32)prebuf[0x2a] << 16u);
            updateParam->stk2AddrOri = updateParam->stk2Addr;
            updateParam->updateMask |= (uint32)1u << 4u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update stk2\n");
#endif
        }
        /*Need update macsec*/
        if(((uint32)0xffff0u != updateParam->macsecLen) && ((uint32)0x0u != updateParam->macsecLen))
        {
            updateParam->macsecAddr    = ((uint32)prebuf[0x2d]) | ((uint32)prebuf[0x2e] << 8u) | ((uint32)prebuf[0x2f] << 16u);
            updateParam->macsecAddrOri = updateParam->macsecAddr;
            updateParam->updateMask |= (uint32)1u << 5u;
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "Need update macsec\n");
#endif
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_check_zone_a(unit, prebuf, updateParam->updateMask, CVA_SPI_UPDATE_ETH));
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_erase_for_ethernet(uint32 unit, uint32 eraseAddr, uint8 flag)
{
    /* Flag 0 = chip erase
       Flag 1 = sector erase
       Flag 2 = block erase (64K)*/
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if((uint8)CVA_SPIC_CHIP_ERASE == flag)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "erase chip\n");
#endif
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_all_erase()); /* Erase chip*/
        }
        else if((uint8)CVA_SPIC_SECTOR_ERASE == flag)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "erase secotr 0x%x \n", eraseAddr);
#endif
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_sec_erase(eraseAddr, 1u)); /* Erase sector*/
        }
        else if((uint8)CVA_SPIC_BLK64K_ERASE == flag)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            DEBUG_INFO(DBG_MEMCTRL, "erase block 0x%x \n", eraseAddr);
#endif
            CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_blk_erase(eraseAddr, 1u)); /* Erase Block*/
        }
        else
        {
            /* do nothing */
        }

    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_erase_sub_function_for_ethernet(uint32 unit, uint32 len, uint32 addr, uint8 eraseFlag)
{
    uint32    eraseNum  = (uint32)0u;
    uint32    eraseSize = CVA_FLASH_4K;
    uint32    i         = 0u;
    RtkApiRet retVal    = RT_ERR_OK;
    do
    {
        if((uint8)CVA_SPIC_BLK64K_ERASE == eraseFlag)
        {
            eraseSize = CVA_FLASH_64K;
        }
        else if((uint8)CVA_SPIC_SECTOR_ERASE == eraseFlag)
        {
            eraseSize = CVA_FLASH_4K;
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)1u > len)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        eraseNum = ((len - (uint32)1u) / ((uint32)eraseSize)) + (uint32)1u;
        for(i = 0u; i < eraseNum; i++)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase_for_ethernet(unit, (addr + (i * eraseSize)), eraseFlag));
        }
        if(retVal != RT_ERR_OK)
        {
            continue;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_erase_fw_config_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase FW*/
        if((uint32)0x1u == (updateParam.updateMask & (uint32)0x1u))
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.fwLen, ((uint32)updateParam.fwAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
        }
        /* need erase Config*/
        if((uint32)0x1u == ((updateParam.updateMask >> 1u) & (uint32)0x1u))
        {
            /*If no stk0 and have macsec for partial update, use sector erase config block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u == updateParam.stk0LenOri) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.configLen, ((uint32)updateParam.configAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.configLen, ((uint32)updateParam.configAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_erase_stk2_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* erase stk2*/
        if((uint32)0x1u == ((updateParam.updateMask >> 4u) & (uint32)0x1u))
        {
            /*If have macsec for partial update, use sector erase stk2 block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                /*use sector erase to erase stk2*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.stk2Len, ((uint32)updateParam.stk2AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                /*use block erase to erase stk2*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.stk2Len, ((uint32)updateParam.stk2AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
    } while(0u == 1u);
    return retVal;
}
static RtkApiRet cva_drv_spi_flash_update_erase_stk1_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase stk1*/
        if((uint32)0x1u == ((updateParam.updateMask >> 3u) & (uint32)0x1u))
        {
            /*If no stk2 and have macsec for partial update, use sector erase stk1 block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u == updateParam.stk2LenOri) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                /*use sector erase to erase stk1*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.stk1Len, ((uint32)updateParam.stk1AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                /*use block erase to erase stk1*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.stk1Len, ((uint32)updateParam.stk1AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_stk2_for_ethernet(unit, sel, flashLayout, updateParam))

    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_erase_stk_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase stk0*/
        if((uint32)0x1u == ((updateParam.updateMask >> 2u) & (uint32)0x1u))
        {
            /*If no stk1 and have macsec for partial update, use sector erase config block for alignment*/
            if((CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode) && ((uint32)0xffff0u == updateParam.stk1LenOri) && ((uint32)0xffff0u != updateParam.macsecLenOri))
            {
                /*use sector erase to erase stk0*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.stk0Len, ((uint32)updateParam.stk0AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
            }
            else
            {
                /*use block erase to erase stk0*/
                CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.stk0Len, ((uint32)updateParam.stk0AddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_BLK64K_ERASE));
            }
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_stk1_for_ethernet(unit, sel, flashLayout, updateParam));
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_erase_macsec_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /* need erase macsec*/
        if((uint32)0x1u == ((updateParam.updateMask >> 5u) & (uint32)0x1u))
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_sub_function_for_ethernet(unit, updateParam.macsecLen, ((uint32)updateParam.macsecAddrOri + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE));
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_erase_for_ethernet(uint32 unit, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet    retVal           = RT_ERR_OK;
    const uint32 fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                     (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        /*erase header*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_erase_for_ethernet(unit, ((uint32)CVA_FLASH_SETTING_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout])), (uint8)CVA_SPIC_SECTOR_ERASE)); /* Flash header must be erased first before update flash */
        /*erase fw and config*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_fw_config_for_ethernet(unit, sel, flashLayout, updateParam));
        /*erase stk*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_stk_for_ethernet(unit, sel, flashLayout, updateParam));
        /*erase macsec*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_macsec_for_ethernet(unit, sel, flashLayout, updateParam));
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet cva_drv_spi_flash_write_section_for_ethernet(uint32 unit, void *fp, cva_SpiWriteSection_t writeSecParam)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint8     buf[0xfffffu] = {0u};
    int32     len           = 0;
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(buf, 0xff, sizeof(buf)));
        len = rtl_file_read(fp, buf, writeSecParam.fpOffset, writeSecParam.programLen);
        if((int32)0 == len)
        {
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_write(writeSecParam.addr, writeSecParam.programLen + (1024u - (writeSecParam.programLen % 1024u)), buf));

    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cva_drv_spi_flash_update_program_for_ethernet(uint32 unit, void *fp, uint8 sel, uint32 flashLayout, cva_SpiFlashUpdate_t updateParam)
{
    RtkApiRet             retVal = RT_ERR_OK;
    cva_SpiWriteSection_t writeSecParam;
    const uint32          fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                              (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                              (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};
    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(&writeSecParam, (int32)0, sizeof(writeSecParam)));

        /* update  fw */
        if((uint32)0x1u == ((updateParam.updateMask >> 0u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.fwAddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.fwAddr;
            writeSecParam.programLen = updateParam.fwLen;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section_for_ethernet(unit, fp, writeSecParam));
        }
        /* update  config */
        if((uint32)0x1u == ((updateParam.updateMask >> 1u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.configAddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.configAddr;
            writeSecParam.programLen = updateParam.configLen;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section_for_ethernet(unit, fp, writeSecParam));
        }
        /* update  stk0*/
        if((uint32)0x1u == ((updateParam.updateMask >> 2u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.stk0AddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.stk0Addr;
            writeSecParam.programLen = updateParam.stk0Len;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section_for_ethernet(unit, fp, writeSecParam));
        }
        /* update  stk1*/
        if((uint32)0x1u == ((updateParam.updateMask >> 3u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.stk1AddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.stk1Addr;
            writeSecParam.programLen = updateParam.stk1Len;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section_for_ethernet(unit, fp, writeSecParam));
        }
        /* update  stk2*/
        if((uint32)0x1u == ((updateParam.updateMask >> 4u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.stk2AddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.stk2Addr;
            writeSecParam.programLen = updateParam.stk2Len;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section_for_ethernet(unit, fp, writeSecParam));
        }
        /* update  macsec*/
        if((uint32)0x1u == ((updateParam.updateMask >> 5u) & (uint32)0x1u))
        {
            writeSecParam.addr       = (uint32)updateParam.macsecAddrOri + ((uint32)sel * fwSizeMapping[flashLayout]);
            writeSecParam.fpOffset   = updateParam.macsecAddr;
            writeSecParam.programLen = updateParam.macsecLen;
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_write_section_for_ethernet(unit, fp, writeSecParam));
        }
    } while(0u == 1u);
    return retVal;
}

RtkApiRet cva_drv_spi_flash_update_for_ethernet(const char *fileName)
{
    void                *fp = NULL;
    cva_SpiFlashUpdate_t updateParam;
    int32                len;
    uint32               i      = 0u;
    RtkApiRet            retVal = RT_ERR_OK;
    uint8                prebuf[118];
    uint8                settingBuf[134];
    uint8                tmp[2];
    uint8                sel              = (uint8)CVA_FLASH_ZONE_B;
    uint8                flashLayout      = (uint8)0u;
    uint16               crcVal           = (uint16)0u;
    uint32               addr             = 0;
    const uint32         fwSizeMapping[8] = {(uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB, (uint32)CVA_FLASH_ZONE_SIZE_512KB,
                                             (uint32)CVA_FLASH_ZONE_SIZE_8MB, (uint32)CVA_FLASH_ZONE_SIZE_4MB, (uint32)CVA_FLASH_ZONE_SIZE_2MB,
                                             (uint32)CVA_FLASH_ZONE_SIZE_1MB, (uint32)CVA_FLASH_ZONE_SIZE_512KB};

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(&updateParam, 0, sizeof(updateParam)));
        CHK_FUN_CONTINUE(retVal, rtl_file_open(&fp, 0, fileName));
        /*flash calibration pattern + header + reservation*/
        len = rtl_file_read(fp, prebuf, (uint32)CVA_FLASH_CALIB_PATTERN_OFFSET, (uint32)118u);
        if((int32)0 == len)
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /*get flash layout*/
        flashLayout = prebuf[0x13] & (uint8)0x7u;

        /*get force update all bit*/
        updateParam.updateMode = (prebuf[0x13] >> 6u) & (uint8)0x1u;

        /*get calculate length*/
        updateParam.fwLen     = ((uint32)prebuf[0x17] << 4u) + ((uint32)prebuf[0x18] << 12u);
        updateParam.configLen = ((uint32)prebuf[0x1c] << 4u) + ((uint32)prebuf[0x1d] << 12u);
        updateParam.stk0Len   = ((uint32)prebuf[0x21] << 4u) + ((uint32)prebuf[0x22] << 12u);
        updateParam.stk1Len   = ((uint32)prebuf[0x26] << 4u) + ((uint32)prebuf[0x27] << 12u);
        updateParam.stk2Len   = ((uint32)prebuf[0x2b] << 4u) + ((uint32)prebuf[0x2c] << 12u);
        updateParam.macsecLen = ((uint32)prebuf[0x30] << 4u) + ((uint32)prebuf[0x31] << 12u);

        /*Check image valid*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_check_valid(CVA_UNIT, updateParam.updateMode, updateParam.fwLen, updateParam.configLen, prebuf));
        /*Get update block information*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_block_for_ethernet(CVA_UNIT, prebuf, &updateParam));
        /*Upgrade mode is partial*/
        if(CVA_FLASH_UPDATE_PARTIAL == updateParam.updateMode)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_get_update_partial_check(CVA_UNIT, prebuf, sel, &updateParam, flashLayout, CVA_SPI_UPDATE_ETH));
        }

        /*do erase*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_erase_for_ethernet(CVA_UNIT, sel, flashLayout, updateParam));
        /*do program*/

        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_update_program_for_ethernet(CVA_UNIT, fp, sel, flashLayout, updateParam));

        len = rtl_file_read(fp, settingBuf, (uint32)CVA_FLASH_SETTING_OFFSET, (uint32)134u);
        for(i = 0; i < (uint32)118u; i++)
        {
            settingBuf[16u + i] = prebuf[i];
        }

        /*Calculate CRC*/
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_crc_16_calculate(CVA_UNIT, &settingBuf[0x20], (uint16)62u, &crcVal));
        settingBuf[0x5e] = (uint8)(crcVal & (uint16)0xffu);
        settingBuf[0x5f] = (uint8)((crcVal >> 8u) & (uint16)0xffu);

        tmp[0]           = settingBuf[0x20];
        tmp[1]           = settingBuf[0x21];
        settingBuf[0x20] = 0xFFu;
        settingBuf[0x21] = 0xFFu;
        addr             = (uint32)CVA_FLASH_SETTING_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout]);
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_write(addr, 134, settingBuf));

        addr = (uint32)CVA_FLASH_HEADER_OFFSET + ((uint32)sel * fwSizeMapping[flashLayout]);
        CHK_FUN_CONTINUE(retVal, cva_eth_acc_flow_flash_write(addr, 2, tmp));

    } while(0u == 1u);
    if(NULL != fp)
    {
        rtl_file_close(&fp);
    }
    return retVal;
}

/**
 * @brief The funtion to occupy the SPIC.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_write().
 */
RtkApiRet cva_drv_spi_flash_spic_lock(uint32 unit)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    regVal     = (uint32)0;
    uint32    occupyFlag = (uint32)0u;
    uint32    timeout    = (uint32)0x10000u;
    uint8     breakFlag  = (uint8)0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SW_PAGE));
        /* Get the occupied flag */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_I2C_8051_CPU_OCCUPY_STATUS, &regVal));
        occupyFlag = regVal & (uint32)CVA_REGISTER_LOCK_MASK;

        if((uint32)CVA_REGISTER_LOCK_FREE == occupyFlag)
        {
            /* Set the occupied flag */
            regVal |= (uint32)CVA_REGISTER_LOCK_CPU_EXTERNAL;
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_I2C_8051_CPU_OCCUPY_STATUS, regVal));

            /* Check the occupied flag */
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_I2C_8051_CPU_OCCUPY_STATUS, &regVal));
            occupyFlag = regVal & (uint32)CVA_REGISTER_LOCK_MASK;
            if((uint32)CVA_REGISTER_LOCK_CPU_EXTERNAL == occupyFlag)
            {
                /* Lock successfully */
                breakFlag = (uint8)1u;
            }
            else
            {
                /* Clear the occupied flag */
                regVal &= (~((uint32)CVA_REGISTER_LOCK_CPU_EXTERNAL)); /* bit 0 set 0 */
                CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_I2C_8051_CPU_OCCUPY_STATUS, regVal));
            }
        }
        else if((uint32)CVA_REGISTER_LOCK_CPU_EXTERNAL == occupyFlag)
        {
            /* Lock successfully */
            breakFlag = (uint8)1u;
        }
        else
        {
            /* Do nothing */
        }

        if((uint8)1u == breakFlag)
        {
/* Lock successfully */
#if 0
            DEBUG_INFO(DBG_MEMCTRL, "The external CPU has occupied the SPI controller successfully.");
#endif
            break;
        }
        timeout--;
    } while((RT_ERR_OK == retVal) && ((uint32)0u != timeout));

    if((uint32)0u == timeout)
    {
#if 0
        DEBUG_INFO(DBG_MEMCTRL, "The SPIC lock timeout. The SPIC is occupied by the internal CPU.");
#endif
        retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
    }

    return retVal;
}

/**
 * @brief The funtion to release occupancy of the SPIC.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_write().
 */
RtkApiRet cva_drv_spi_flash_spic_unlock(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint32)0;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SW_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_I2C_8051_CPU_OCCUPY_STATUS, &regVal));
        if((regVal & ((uint32)CVA_REGISTER_LOCK_CPU_EXTERNAL)) != (uint32)0u)
        {
            regVal &= (~((uint32)CVA_REGISTER_LOCK_CPU_EXTERNAL)); /*bit 0 set 0 */
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_I2C_8051_CPU_OCCUPY_STATUS, regVal));
#if 0
            DEBUG_INFO(DBG_MEMCTRL, "The external CPU has released the resource of the SPI controller.");
#endif
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to backup the speed.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] baudrate Baudrate (0x1 to 0xf) \n
 *            The baudrate.
 * @param[out] fbaudrate Fast baudrate (0x1 to 0xf) \n
 *            The fast baudrate.
 * @param[out] srcClk Source clock (0 to 3) \n
 *            The source clock.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read(),cva_reg_field_read().
 */
RtkApiRet cva_drv_spi_flash_speed_backup(uint32 unit, uint32 *baudrate, uint32 *fbaudrate, uint32 *srcClk)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_BAUDR, baudrate));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_FLASH_FBAUDR, fbaudrate));
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_CPU_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, (uint32)CVA_CLK_RST_CLOCK_SELECTION, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LSP, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LEN, srcClk));
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to backup the delay chain.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] delayChain Delay chain (0x0 to 0xFFFFFFFF) \n
 *            The delay chain.
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_read().
 */
RtkApiRet cva_drv_spi_flash_delaychain_backup(uint32 unit, uint32 *delayChain)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_CPU_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, delayChain));
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The sub funtion to revert the speed.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] baudrate Baudrate (0x1 to 0xf) \n
 *            The baudrate.
 * @param[in] fbaudrate Fast baudrate (0x1 to 0xf) \n
 *            The fast baudrate.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_write().
 */
static RtkApiRet cva_drv_spi_flash_speed_revert_baud(uint32 unit, uint32 baudrate, uint32 fbaudrate)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* set spic baud rate */
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_SPIC_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_SSIENR, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_BAUDR, baudrate));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_FLASH_FBAUDR, fbaudrate));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief The funtion to revert the speed.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] baudrate Baudrate (0x1 to 0xf) \n
 *            The baudrate.
 * @param[in] fbaudrate Fast baudrate (0x1 to 0xf) \n
 *            The fast baudrate.
 * @param[in] srcClk Source clock (0 to 3) \n
 *            The source clock.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_drv_spi_flash_speed_revert_baud(),cva_reg_page_write(),cva_reg_write(),cva_reg_field_set().
 */
RtkApiRet cva_drv_spi_flash_speed_revert(uint32 unit, uint32 baudrate, uint32 fbaudrate, uint32 srcClk)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regValue;

    do
    {

        /* set spic baud rate */
        CHK_FUN_CONTINUE(retVal, cva_drv_spi_flash_speed_revert_baud(unit, baudrate, fbaudrate));

        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, (uint32)CVA_CPU_PAGE));
        /* disable spic clock */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_CLK_RST_IP_ENABLE, &regValue));
        CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_IP_ENABLE, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LSP, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LEN, (uint32)0u, &regValue));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_CLK_RST_IP_ENABLE, regValue));

        /* set spic src clock */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_CLK_RST_CLOCK_SELECTION, &regValue));
        CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_CLOCK_SELECTION, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LSP, (uint32)CVA_CLK_RST_CLOCK_SELECTION_SEL_CLK_SPIC_SRC_LEN, srcClk, &regValue));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_CLK_RST_CLOCK_SELECTION, regValue));

        /* enable spic clock */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, (uint32)CVA_CLK_RST_IP_ENABLE, &regValue));
        CHK_FUN_CONTINUE(retVal, cva_reg_field_set((uint32)CVA_CLK_RST_IP_ENABLE, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LSP, (uint32)CVA_CLK_RST_IP_ENABLE_EN_CLK_SPIC_LEN, (uint32)0x1, &regValue));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_CLK_RST_IP_ENABLE, regValue));

        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to backup the delay chain.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] delayChain Delay chain (0x0 to 0xFFFFFFFF) \n
 *            The delay chain.
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval Others Please refer to cva_reg_page_write(),cva_reg_write().
 */
RtkApiRet cva_drv_spi_flash_delaychain_revert(uint32 unit, uint32 delayChain)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_CPU_PAGE));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, (uint32)CVA_SPIC_DELAYCHAIN_WITH_INVERTER_PAIR_SELECT, delayChain));
        CHK_FUN_CONTINUE(retVal, cva_reg_page_write(unit, CVA_SPIC_PAGE));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief The funtion to get the channel and speed.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] outSpiCH Cahnnel (RTK_SPI_FLASH_SINGLE_CHANNEL,RTK_SPI_FLASH_DUAL_CHANNEL,RTK_SPI_FLASH_QUAD_CHANNEL) \n
 *            The spi channel.
 * @param[out] outSpicClk Speed ((RTK_SPI_FLASH_100MHz,RTK_SPI_FLASH_83MHz,RTK_SPI_FLASH_50MHz,RTK_SPI_FLASH_33MHz,RTK_SPI_FLASH_12MHz) \n
 *             The spi speed.
 * @retval RT_ERR_OK
 */
RtkApiRet cva_drv_spi_flash_init_get(uint32 unit, uint32 *outSpiCH, uint32 *outSpicClk)
{
    RtkApiRet retVal = RT_ERR_OK;
    *outSpiCH        = cva_g_spiCh;
    *outSpicClk      = cva_g_spicClk;
    return retVal;
}
