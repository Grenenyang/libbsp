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

#include "rtl907xc_vb_asicdrv_otp.h"
#include "rtl907xc_vb_asicdrv_phy.h"
#include "rtl907xc_vb_asicdrv_port.h"
#include "rtl907xc_vb_asicdrv_config.h"
#include "rtl907xc_vb_asicdrv_flash.h"
#include "rtl907xc_vb_asicdrv_opfsm.h"
#include "rtl907xc_vb_asicdrv_serdes.h"
#include "rtl907xc_vb_api_reset.h"
#include "rtl907xc_vb_reg_struct.h"

/**
 * @addtogroup RTK_MODULE_SYS_BOOT System Bootsup
 * @{
 */

/**
 * @addtogroup FL_SYS Update Firmware/Configuration via Register Interface
 * @{
 */
/**
 * @addtogroup FL_SYS_ASIC Low Level Driver
 * @brief Firmware/Configuration via Register Interface Low Level Driver
 * @{
 */
/**
 * @brief A function that checks if the system is in flashless mode and is ready for an external update. \n
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully entered flashless mode.
 * @retval Others Please refer to cvb_reg_read(),cvb_reset_chip_rst(),cvb_drv_config_polling_status(),
 *                                cvb_reg_write(),cvb_reg_field_write()
 */
RtkApiRet cvb_sys_check_flashless_mode(uint32 unit, uint8 byPassEth)
{
    RtkApiRet        retVal        = RT_ERR_OK;
    uint32           data          = 0u;
    uint32           enterValue[2] = {CVB_SYS_BOOT_UP_FLASHLESS_ETH, CVB_SYS_BOOT_UP_FLASHLESS};
    uint32           intEvent[2]   = {CVB_SYSMSG_READY_FOR_CONFIG_REGIF, CVB_SYSMSG_READY_FOR_CONFIG_ETHERNET};
    uint32           intValue      = 1u;
    cvb_PollingReg_t pollingReg;

    do
    {

        /* Check flashless mode.*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, CVB_CPU_DMY_1, &data));

        /* CVB_SYS_BOOT_UP_FLASHLESS_ETH and CVB_SYS_BOOT_UP_FLASHLESS both support reg interface*/
        if((CVB_SYS_BOOT_UP_FLASHLESS_ETH != ((data >> 24u) & 0xFFu)) && (CVB_SYS_BOOT_UP_FLASHLESS != ((data >> 24u) & 0xFFu)))
        {
            /* Reset chip*/
            //CHK_FUN_CONTINUE(retVal, cvb_reset_chip_rst(unit));
            (void)rtk_usecond_delay(6000u);
            pollingReg.address      = CVB_CPU_DMY_1;
            pollingReg.lsp          = 24u;
            pollingReg.len          = 8u;
            pollingReg.num          = 2u;
            pollingReg.value        = &enterValue[0];
            pollingReg.timeoutValue = CVB_FL_TIMEOUT_VALUE;
            pollingReg.isEth        = 0u;
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_polling_status(unit, pollingReg));
        }

        /* By pass port link status checking */
        if(CVB_BYPASS_ENABLE == byPassEth)
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_INTERNAL_CPU_VARIABLE19, CVB_FLASHLESS_BYPASS_10S_CMD));
        }
        /* Wait for flashless ready interrupt from switch */
        pollingReg.address      = CVB_SWITCH_INTERRUPT_MODE;
        pollingReg.lsp          = 6u;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &intValue;
        pollingReg.timeoutValue = CVB_FL_TIMEOUT_VALUE;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = CVB_SWITCH_INTERRUPT_MODE;
        pollingReg.lsp          = CVB_SWITCH_INTERRUPT_MODE_EVENT_TOEXTCPU_NUM_LSP;
        pollingReg.len          = CVB_SWITCH_INTERRUPT_MODE_EVENT_TOEXTCPU_NUM_LEN;
        pollingReg.num          = 2u;
        pollingReg.value        = &intEvent[0];
        pollingReg.timeoutValue = 1u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_polling_status(unit, pollingReg));
        /* Write 1 to clear interrupt*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, CVB_SWITCH_INTERRUPT_MODE, 6u, 1u, intValue));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief A function used to leave flashless. \n
 *        It will trigger firmware authentication if firmware exists.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully triggered exit from flashless mode.
 * @retval Others Please refer to cvb_reg_write(),cvb_drv_config_polling_status(),cvb_reg_field_write().
 */
RtkApiRet cvb_sys_leave_flashless(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Leave flashless with CVB_FLASHLESS_EXIT_CMD*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, CVB_INTERNAL_CPU_VARIABLE19, CVB_FLASHLESS_EXIT_CMD));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief A function to check if leaving flashless is successful. \n
 *        It will check firmware status and external interrupt.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully left flashless mode
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Timeout.
 * @retval Others Please refer to cvb_reg_field_write(),cvb_drv_config_polling_status().
 */
RtkApiRet cvb_sys_check_leave_flashless(uint32 unit)
{
    RtkApiRet        retVal = RT_ERR_OK;
    cvb_PollingReg_t pollingReg;
    uint32           intValue   = 1u;
    uint32           intEvent   = CVB_SYSMSG_INITZED_CONFIG_SUCCESS;
    uint32           bootStatue = CVB_SYS_BOOT_UP_FULL_INITIZED;

    do
    {
        /* Wait configuration done interrupt from switch.*/
        pollingReg.address      = CVB_SWITCH_INTERRUPT_MODE;
        pollingReg.lsp          = 6u;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &intValue;
        pollingReg.timeoutValue = CVB_FL_TIMEOUT_VALUE;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = CVB_SWITCH_INTERRUPT_MODE;
        pollingReg.lsp          = CVB_SWITCH_INTERRUPT_MODE_EVENT_TOEXTCPU_NUM_LSP;
        pollingReg.len          = CVB_SWITCH_INTERRUPT_MODE_EVENT_TOEXTCPU_NUM_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &intEvent;
        pollingReg.timeoutValue = 1u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_polling_status(unit, pollingReg));

        /* Write 1 to clear interrupt*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, CVB_SWITCH_INTERRUPT_MODE, 6u, 1u, intValue));

        pollingReg.address      = CVB_CPU_DMY_1;
        pollingReg.lsp          = 24u;
        pollingReg.len          = 8u;
        pollingReg.num          = 1u;
        pollingReg.value        = &bootStatue;
        pollingReg.timeoutValue = CVB_FL_TIMEOUT_VALUE;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_polling_status(unit, pollingReg));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief A function used to update firmware into SRAM in flashless mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The image file name to input.
 * @param[out] None
 * @retval RT_ERR_OK Successfully updated firmware.
 * @retval -RT_ERR_FAILED Firmware update failed.
 * @retval Others Please refer to rtl_file_open(),cvb_drv_config_get_hdr_fw_info(),cvb_reg_page_read(),
 *                                cvb_reg_page_write(),cvb_reg_write(),cvb_reg_field_write().
 * @note A binary with firmware should be input for this function.
 */
RtkApiRet cvb_sys_update_firmware(uint32 unit, const int8 *fileName)
{
    RtkApiRet retVal         = RT_ERR_OK;
    void     *fp             = NULL;
    uint32    fwLength       = (uint32)0u;
    uint32    fwAddr         = (uint32)0u;
    uint32    i              = (uint32)0u;
    uint32    fwBuf          = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint8     fileBuf[4]     = {(uint8)0u};
    uint32    fileAddr       = (uint32)0u;
    uint8     fwKeyType      = (uint8)0u;
    do
    {
        /* open the binary file with firmware */
        retVal = rtl_file_open(&fp, 0, fileName);
        if(RT_ERR_OK != retVal)
        {
#ifdef DEBUG_SYS
            (void)rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_get_hdr_fw_info(fp, &fwAddr, &fwLength, &fwKeyType));

        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(unit, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(unit, 0x00U));

        /* write data into SRAM */
        fileAddr = fwAddr;
        for(i = (uint32)0u; i < fwLength; i += (uint32)4u)
        {
            if((int32)0 == rtl_file_read(fp, fileBuf, fileAddr + i, 4u))
            {
                retVal = -RT_ERR_FAILED;
                continue;
            }
            fwBuf = (uint32)fileBuf[0] | ((uint32)fileBuf[1] << 8) | ((uint32)fileBuf[2] << 16) | ((uint32)fileBuf[3] << 24);

            CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_reg_write(unit, i + CVB_ADDR_L2_MEMORY_BASE, fwBuf));
        }

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_reg_page_write(unit, currentRegPage));
        /*set fwlength to dmy reg*/
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_reg_field_write(unit, CVB_INTERNAL_CPU_VARIABLE57, 4u, 20u, fwLength));
        /*set fw key type to dmy reg*/
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_reg_field_write(unit, CVB_INTERNAL_CPU_VARIABLE57, 0u, 4u, fwKeyType));
    } while(0u == 1u);

    if(fp != NULL)
    {
        rtl_file_close(&fp);
    }

    return retVal;
}

/**
 * @brief A function used to update configuration in flashless mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The image file name to input.
 * @param[out] None
 * @retval RT_ERR_OK Successfully updated configuration.
 * @retval -RT_ERR_FAILED Configuration update failed.
 * @retval Others Please refer to cvb_drv_op_disable_wup_transmit(),cvb_drv_port_disable_rx_en(),cvb_drv_drv_memset(),
 *                                cvb_drv_config_parse_flash_cfg_header(),cvb_reg_page_read(),cvb_reg_page_write(),
 *                                cvb_drv_config_check_enable_phy(),cvb_drv_config_sys_patch(),cvb_drv_op_enable_wup_transmit().
 * @note A binary with no secure boot firmware should be input for this function.
 */
RtkApiRet cvb_sys_update_config(uint32 unit, const int8 *fileName)
{
    RtkApiRet          retVal                        = RT_ERR_OK;
    uint32             spiStartAddr[CVB_CFG_END_IDX] = {(uint32)0u};
    uint32             spiParaLen[CVB_CFG_END_IDX]   = {(uint32)0u};
    uint32             i                             = (uint32)0u;
    uint32             currentRegPage                = (uint32)0u;
    uint32             phyPortEnIdx                  = (uint32)0u;
    uint32             wupPortMask                   = (uint32)0xFFFu;
    cvb_UpdateConfig_t updateConfig;

    do
    {
        /* disable wup tx*/
        CHK_FUN_CONTINUE(retVal, cvb_drv_op_disable_wup_transmit(unit, &wupPortMask));

        /* disable rx_en */
        CHK_FUN_CONTINUE(retVal, cvb_drv_port_disable_rx_en(unit, CVB_RTK_SYS_PMAP_ALL));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(&updateConfig, 0, sizeof(updateConfig)));

        /* parse the header to get the configuration type and length */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_parse_flash_cfg_header(fileName, spiStartAddr, spiParaLen, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(unit, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(unit, 0x4Au));

        /* Check enabled port */

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_check_enable_phy(&phyPortEnIdx));
        updateConfig.phyPortEnIdx = phyPortEnIdx;

        /* config */
        for(i = (uint32)0u; i < (uint32)CVB_CFG_END_IDX; i++)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            (void)rtlglue_printf("index %d, addr 0x%x, len 0x%x , pre ret %d\n", i, spiStartAddr[i], spiParaLen[i], retVal);
#endif
            updateConfig.imgParaLen   = spiParaLen[i];
            updateConfig.imgStartAddr = spiStartAddr[i];
            updateConfig.index        = i;
            CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_sys_patch(unit, fileName, updateConfig));
            if(CVB_CFG_SGMII_PATCH_IDX == i)
            {
                cvb_g_pureSDSPowerUp = 0u;
            }
        }

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_reg_page_write(unit, currentRegPage));

        /* config wake o*/
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_op_fetch_wakeo_config(unit, fileName));

        /* enable wup tx*/
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_op_enable_wup_transmit(unit, wupPortMask));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* FL_SYS_ASIC */
/**@}*/ /* FL_SYS */

/**
 * @addtogroup FL_ETH_PORT_DISABLE Disable Port for Ethernet Access Flashless
 * @{
 */
/**
 * @addtogroup FL_ETH_PORT_DISABLE_ASIC Low Level Driver
 * @brief Disable Port for Ethernet Access Flashless Low Level Driver
 * @{
 */

/**
 * @brief Program OTP memory if needed to disable port for flashless over Ethernet Access.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort User port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH) \n
 *            Specify the user port which will be disabled for flashless operation.
 * @param[out] None
 * @retval RT_ERR_OK Successfully disabled the selected port for flashless over Ethernet Access.
 * @retval -RT_ERR_PORT_ID The selected port was out of range.
 * @retval -RT_ERR_FAILED The selected port was not disabled for flashless over Ethernet Access.
 * @retval Others Please refer to cvb_otp_read(),cvb_otp_write().
 */
RtkApiRet cvb_sys_flashless_eth_port_otp_disable(uint32 unit, cvb_RtkPort userPort)
{
    RtkApiRet   retVal      = RT_ERR_OK;
    uint16      portEnWrite = (uint16)0x0u;
    uint16      portEnRead  = (uint16)0x0u;
    cvb_RtkPort sysPort     = 0u;

    do
    {
        if(0u == CVB_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        sysPort = cvb_g_userPort2sysPort[userPort];
        if((CVB_RTK_SYS_PORT_11TH < sysPort) || (CVB_RTK_SYS_PORT_1ST > sysPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        CHK_FUN_RET(retVal, cvb_otp_read(unit, CVB_OTP_FLASHLESS_ENABLE_PORT, (uint8 *)&portEnRead, 2u));
        if(0u == (portEnRead & ((uint16)1u << sysPort)))
        {
            /*Already disabled*/
            continue;
        }
        portEnWrite = portEnRead & ~((uint16)1u << sysPort);
        /* Disable the port */
        CHK_FUN_RET(retVal, cvb_otp_write(unit, CVB_OTP_FLASHLESS_ENABLE_PORT, (uint8 *)&portEnWrite, 2u));
        portEnRead = (uint16)0u;
        CHK_FUN_RET(retVal, cvb_otp_read(unit, CVB_OTP_FLASHLESS_ENABLE_PORT, (uint8 *)&portEnRead, 2u));
        if(portEnRead != portEnWrite)
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* FL_ETH_PORT_DISABLE_ASIC */
/**@}*/ /* FL_ETH_PORT_DISABLE */

/**
 * @addtogroup FL_ETH Update Firmware/Configuration via Ethernet Access
 * @{
 */
/**
 * @addtogroup FL_ETH_ASIC Low Level Driver
 * @brief Firmware/Configuration via Ethernet Access Low Level Driver
 * @{
 */

/**
 * @brief A function used to update firmware into SRAM via etherAccess in flashless mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The image file name to input.
 * @param[out] None
 * @retval RT_ERR_OK Successfully updated firmware for flashless over Ethernet Access.
 * @retval -RT_ERR_FAILED Firmware update via Ethernet Access failed.
 * @retval Others Please refer to rtl_file_open(),cvb_drv_config_get_hdr_fw_info(),
 *                cvb_eth_acc_flow_patch_write(),cvb_drv_config_eth_write_field().
 * @note A binary with firmware should be input for this function.
 */
RtkApiRet cvb_sys_eth_update_firmware(uint32 unit, const int8 *fileName)
{
    RtkApiRet retVal                   = RT_ERR_OK;
    void     *fp                       = NULL;
    uint32    fwLength                 = (uint32)0u;
    uint32    fwAddr                   = (uint32)0u;
    uint8     fwKeyType                = (uint8)0u;
    uint8     fwBuf[CVB_MAX_FW_LENGTH] = {(uint8)0u};

    do
    {
        /* open the binary file with firmware */
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(RT_ERR_OK != retVal)
        {
#ifdef DEBUG_SYS
            rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_get_hdr_fw_info(fp, &fwAddr, &fwLength, &fwKeyType));
        if((int32)0u == rtl_file_read(fp, fwBuf, fwAddr, fwLength))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_flow_patch_write(0u, fwLength, fwBuf));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field(CVB_INTERNAL_CPU_VARIABLE57, 4u, 20u, fwLength));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field(CVB_INTERNAL_CPU_VARIABLE57, 0u, 4u, fwKeyType));
    } while(0u == 1u);

    if(NULL != fp)
    {
        rtl_file_close(&fp);
    }
    return retVal;
}

/**
 * @brief A function used to update configuration in flashless mode via Ethernet Access.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort Configure user port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH) \n
 *            Specify the user port connected to link partner to do flashless updating.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The image file name to input.
 * @param[out] None
 * @retval RT_ERR_OK Successfully updated configuration for flashless over Ethernet Access.
 * @retval -RT_ERR_FAILED Configuration update via Ethernet Access failed.
 * @retval -RT_ERR_PORT_ID The configured user port was out of range.
 * @retval Others Please refer to cvb_drv_op_eth_disable_wup_transmit(),cvb_drv_drv_memset(),cvb_drv_config_parse_flash_cfg_header(),
 *                                cvb_drv_config_check_eth_enable_phy(),cvb_drv_config_eth_patch(),
 *                                cvb_drv_op_eth_fetch_wakeo_config,cvb_drv_op_eth_enable_wup_transmit().
 * @note A binary with no secure boot firmware should be input for this function.
 */
RtkApiRet cvb_sys_eth_update_config(uint32 unit, cvb_RtkPort userPort, const int8 *fileName)
{
    RtkApiRet          retVal                        = RT_ERR_OK;
    uint32             spiStartAddr[CVB_CFG_END_IDX] = {(uint32)0u};
    uint32             spiParaLen[CVB_CFG_END_IDX]   = {(uint32)0u};
    uint32             i                             = (uint32)0u;
    uint32             phyPortEnIdx                  = (uint32)0u;
    uint32             wupPortMask                   = (uint32)0xFFFu;
    uint8              mode                          = (uint8)0x0u;
    cvb_UpdateConfig_t updateConfig;

    do
    {
        if(0u == CVB_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* disable wup */
        CHK_FUN_CONTINUE(retVal, cvb_drv_op_eth_disable_wup_transmit(unit, &wupPortMask));
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(&updateConfig, 0, sizeof(updateConfig)));

        /* parse the header to get the configuration type and length */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_parse_flash_cfg_header(fileName, spiStartAddr, spiParaLen, 0u));

        /* Read enabled port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_check_eth_enable_phy(&phyPortEnIdx));
        updateConfig.configPort   = userPort;
        updateConfig.phyPortEnIdx = phyPortEnIdx;
        mode                      = mode | ((uint8)1u << CVB_CONFIG_ETH_MODE_BIT);
        /* config */
        for(i = (uint32)0u; i < (uint32)CVB_CFG_END_IDX; i++)
        {
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
            (void)rtlglue_printf("index %d, addr 0x%x, len 0x%x , pre ret %d\n", i, spiStartAddr[i], spiParaLen[i], retVal);
#endif
            updateConfig.imgParaLen   = spiParaLen[i];
            updateConfig.imgStartAddr = spiStartAddr[i];
            updateConfig.index        = i;
            CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_patch(unit, fileName, updateConfig, mode));
        }
        /* config wake o*/
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_op_eth_fetch_wakeo_config(fileName));
        /* enable wup */
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_op_eth_enable_wup_transmit(unit, wupPortMask));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief A function used to disable the RX capability of all ports except the configured port. \n
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort Configure user port (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_11TH) \n
 *            Specify the user port connected to the link partner to do flashless updating.
 * @param[out] None
 * @retval RT_ERR_OK Successfully disabled the RX capability of all ports except the configured port.
 * @retval -RT_ERR_PORT_ID The selected port was out of range.
 * @retval Others Please refer to cvb_drv_port_eth_disable_rx_en().
 */
RtkApiRet cvb_sys_eth_disable_non_configure_port_rx(uint32 unit, cvb_RtkPort userPort)
{
    RtkApiRet   retVal     = RT_ERR_OK;
    uint32      rxPortMask = CVB_RTK_SYS_PMAP_ALL;
    cvb_RtkPort sysPort    = 0u;

    do
    {
        if(0u == CVB_IS_VALID_PORT_ID_WO_CPUPORT(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        sysPort = cvb_g_userPort2sysPort[userPort];
        if((CVB_RTK_SYS_PORT_11TH < sysPort) || (CVB_RTK_SYS_PORT_1ST > sysPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        rxPortMask &= ~((uint32)1u << sysPort);
        CHK_FUN_CONTINUE(retVal, cvb_drv_port_eth_disable_rx_en(unit, rxPortMask));

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* FL_ETH_ASIC */
/**@}*/ /* FL_ETH */

/**@}*/ /* System Bootsup */
