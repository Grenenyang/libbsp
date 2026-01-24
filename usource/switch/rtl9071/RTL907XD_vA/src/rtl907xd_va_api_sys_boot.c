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

#include "rtl907xd_va_asicdrv_otp.h"
#include "rtl907xd_va_asicdrv_phy.h"
#include "rtl907xd_va_asicdrv_port.h"
#include "rtl907xd_va_asicdrv_config.h"
#include "rtl907xd_va_asicdrv_flash.h"
#include "rtl907xd_va_asicdrv_opfsm.h"
#include "rtl907xd_va_api_reset.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_serdes.h"

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
 * @retval Others Please refer to dva_reg_read(),dva_reset_chip_rst(),dva_reg_field_write(),
 *                                dva_drv_config_polling_status().
 */
RtkApiRet dva_sys_check_flashless_mode(uint32 unit, uint8 byPassEth)
{
    RtkApiRet        retVal        = RT_ERR_OK;
    RtkApiRet        tmpRet        = RT_ERR_OK;
    uint32           data          = 0u;
    uint32           enterValue[3] = {DVA_SYS_BOOT_UP_FLASHLESS_ENTERED, DVA_SYS_BOOT_UP_FLASHLESS_ETH, DVA_SYS_BOOT_UP_FLASHLESS};
    uint32           intValue      = 1u;
    dva_PollingReg_t pollingReg;

    do
    {
        /* Check flashless mode.*/
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, DVA_CPU_DMY_1, &data));

        /* DVA_SYS_BOOT_UP_FLASHLESS_ETH and DVA_SYS_BOOT_UP_FLASHLESS both support reg interface*/
        if((DVA_SYS_BOOT_UP_FLASHLESS_ENTERED != ((data >> 24u) & 0xFFu)) && (DVA_SYS_BOOT_UP_FLASHLESS_ETH != ((data >> 24u) & 0xFFu)) && (DVA_SYS_BOOT_UP_FLASHLESS != ((data >> 24u) & 0xFFu)))
        {
            /* Reset chip*/
            CHK_FUN_CONTINUE(retVal, dva_reset_chip_rst(unit));
            pollingReg.address      = DVA_CPU_DMY_1;
            pollingReg.lsp          = 24u;
            pollingReg.len          = 8u;
            pollingReg.num          = 3u;
            pollingReg.value        = &enterValue[0];
            pollingReg.timeoutValue = DVA_FL_TIMEOUT_VALUE;
            pollingReg.isEth        = 0u;
            CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        }
        /* By pass port link status checking */
        if(DVA_BYPASS_ENABLE == byPassEth)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_EXIT_CMD_LSP, DVA_EXIT_CMD_LEN, DVA_FLASHLESS_BYPASS_ETH_CMD));
        }
        /* Wait flashless ready interrupt from switch */

        pollingReg.address      = DVA_RG_INTERRUPT_TOP_EXT_INTR_MODE;
        pollingReg.lsp          = 6u;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &intValue;
        pollingReg.timeoutValue = DVA_FL_TIMEOUT_VALUE;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        /* Write 1 to clear interrupt*/
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_RG_INTERRUPT_TOP_EXT_INTR_MODE, 6u, 1u, 1u));

        pollingReg.address      = DVA_SYS_EVENT_PENDING_MASK_REG;
        pollingReg.lsp          = DVA_EXINT_BOOT_READY_FOR_CONFIG_REGIF;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &intValue;
        pollingReg.timeoutValue = 1u;
        pollingReg.isEth        = 0u;
        tmpRet                  = dva_drv_config_polling_status(unit, pollingReg);
        if(RT_ERR_OK != tmpRet)
        {
            pollingReg.address      = DVA_SYS_EVENT_PENDING_MASK_REG;
            pollingReg.lsp          = DVA_EXINT_BOOT_READY_FOR_CONFIG_ETHERNET;
            pollingReg.len          = 1u;
            pollingReg.num          = 1u;
            pollingReg.value        = &intValue;
            pollingReg.timeoutValue = 1u;
            pollingReg.isEth        = 0u;
            CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief A function that transitions from the firmware loading stage to the configuration loading stage. \n
 *        It will trigger firmware authentication if the firmware exists. \n
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully entered configuration stage.
 * @retval Others Please refer to dva_reg_field_write(),dva_drv_config_polling_status().
 */
RtkApiRet dva_sys_leave_fw_to_cfg(uint32 unit)
{
    RtkApiRet        retVal = RT_ERR_OK;
    uint32           stage  = DVA_CONFIG_STAGE;
    uint32           status = 0u;
    dva_PollingReg_t pollingReg;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_EXIT_CMD_LSP, DVA_EXIT_CMD_LEN, DVA_FLASHLESS_EXIT_CMD));
        /* Polling stage to DVA_CONFIG_STAGE and status no error*/
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STAGE_LSP;
        pollingReg.len          = DVA_STAGE_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &stage;
        pollingReg.timeoutValue = 0x1000u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STATUS_LSP;
        pollingReg.len          = DVA_STATUS_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &status;
        pollingReg.timeoutValue = 0x1u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief A function used to leave flashless.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully triggered exit from flashless mode.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Timeout.
 * @retval Others Please refer to dva_reg_field_write(),dva_drv_config_polling_status().
 */
RtkApiRet dva_sys_leave_flashless(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Leave cfg flashless*/
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_EXIT_CMD_LSP, DVA_EXIT_CMD_LEN, DVA_FLASHLESS_EXIT_CFG_CMD));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief A function to check if leaving flashless is successful. \n
 *        It will check firmware status and external interrupt. \n
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Successfully left flashless mode
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Timeout.
 * @retval Others Please refer to dva_reg_field_write(),dva_drv_config_polling_status().
 */
RtkApiRet dva_sys_check_leave_flashless(uint32 unit)
{
    RtkApiRet        retVal     = RT_ERR_OK;
    uint32           stage[2]   = {DVA_FLASHLESS_DONE_STAGE, DVA_FLASHLESS_DONE_NO_FW_STAGE};
    uint32           status     = 0u;
    uint32           intValue   = 1u;
    uint32           bootStatue = DVA_SYS_BOOT_UP_FULL_INITIZED;
    dva_PollingReg_t pollingReg;

    do
    {
        /* Polling stage to DVA_FLASHLESS_DONE_STAGE stage and status no error*/
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STAGE_LSP;
        pollingReg.len          = DVA_STAGE_LEN;
        pollingReg.num          = 2u;
        pollingReg.value        = &stage[0];
        pollingReg.timeoutValue = DVA_FL_TIMEOUT_VALUE;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STATUS_LSP;
        pollingReg.len          = DVA_STATUS_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &status;
        pollingReg.timeoutValue = 1u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = DVA_RG_INTERRUPT_TOP_EXT_INTR_MODE;
        pollingReg.lsp          = 6u;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &intValue;
        pollingReg.timeoutValue = DVA_FL_TIMEOUT_VALUE;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        /* Write 1 to clear interrupt*/
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_RG_INTERRUPT_TOP_EXT_INTR_MODE, 6u, 1u, intValue));
        pollingReg.address      = DVA_SYS_EVENT_PENDING_MASK_REG;
        pollingReg.lsp          = DVA_EXINT_BOOT_INITZED_CONFIG_SUCCESS;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &intValue;
        pollingReg.timeoutValue = 1u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = DVA_CPU_DMY_1;
        pollingReg.lsp          = 24u;
        pollingReg.len          = 8u;
        pollingReg.num          = 1u;
        pollingReg.value        = &bootStatue;
        pollingReg.timeoutValue = DVA_FL_TIMEOUT_VALUE;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));

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
 * @retval Others Please refer to rtl_file_open(),dva_drv_config_get_hdr_fw_info(),dva_drv_config_polling_status(),
 *                                dva_reg_page_write(),dva_reg_write(),dva_reg_field_write(),dva_drv_config_update_aes_ctr_iv().
 * @note A binary with firmware should be input for this function.
 */
RtkApiRet dva_sys_update_firmware(uint32 unit, const int8 *fileName)
{
    RtkApiRet        retVal     = RT_ERR_OK;
    void            *fp         = NULL;
    uint32           fwLength   = (uint32)0u;
    uint32           fwAddr     = (uint32)0u;
    uint32           i          = (uint32)0u;
    uint32           fwBuf      = (uint32)0u;
    uint8            fileBuf[4] = {(uint8)0u};
    uint32           fileAddr   = (uint32)0u;
    uint8            fwKeyType  = (uint8)0u;
    dva_PollingReg_t pollingReg;
    uint32           stage = DVA_FIRMWARE_STAGE;
    do
    {
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STAGE_LSP;
        pollingReg.len          = DVA_STAGE_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &stage;
        pollingReg.timeoutValue = 0x10000u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
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

        CHK_FUN_CONTINUE(retVal, dva_drv_config_get_hdr_fw_info(fp, &fwAddr, &fwLength, &fwKeyType));

        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dva_reg_page_write(unit, DVA_PAGE_SYS_MEM));

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

            CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_reg_write(unit, i, fwBuf));
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_drv_config_update_aes_ctr_iv(unit, fp, fwKeyType, fwLength));

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_reg_page_write(unit, DVA_PAGE_SW));
        /*set fwlength to dmy reg*/
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_reg_field_write(unit, DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_2, 4u, 20u, fwLength));
        /*set fw key type to dmy reg*/
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_reg_field_write(unit, DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_2, 0u, 4u, fwKeyType));
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
 * @retval Others Please refer to dva_drv_port_disable_rx_en(),dva_drv_config_load_config(),dva_drv_config_trigger_and_polling(),
 * @note A binary with firmware should be input for this function.
 */
RtkApiRet dva_sys_update_config(uint32 unit, const int8 *fileName)
{
    RtkApiRet        retVal = RT_ERR_OK;
    dva_PollingReg_t pollingReg;
    uint32           trigger = 0u;
    uint32           status  = 0u;
    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_port_disable_rx_en(unit, DVA_RTK_SYS_PMAP_ALL));
        CHK_FUN_CONTINUE(retVal, dva_drv_config_load_config(unit, fileName));
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("Trigger\n");
#endif
        CHK_FUN_CONTINUE(retVal, dva_reg_page_write(unit, 0x4Au));
        CHK_FUN_CONTINUE(retVal, dva_reg_bit_set(unit, DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_TRIGGER_LSP, 1u));
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE;
        pollingReg.lsp          = DVA_TRIGGER_LSP;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &trigger;
        pollingReg.timeoutValue = 0x5000u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STATUS_LSP;
        pollingReg.len          = DVA_STATUS_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &status;
        pollingReg.timeoutValue = 0x1u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("Config done\n");
#endif
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
 * @brief Program OTP memory if need to disable port for flashless over ehternet.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort User port (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_11TH) \n
 *            Specify the user port which will be disabled for flashless operation.
 * @param[out] None
 * @retval RT_ERR_OK Successfully disabled the selected port for flashless over Ethernet Access.
 * @retval -RT_ERR_PORT_ID The selected port was out of range.
 * @retval -RT_ERR_FAILED The selected port was not disabled for flashless over Ethernet Access.
 * @retval Others Please refer to dva_otp_read(),dva_otp_write().
 */
RtkApiRet dva_sys_flashless_eth_port_otp_disable(uint32 unit, RtkPort userPort)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint16    portEnWrite = (uint16)0x0u;
    uint16    portEnRead  = (uint16)0x0u;
    RtkPort   sysPort     = 0u;

    do
    {
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        sysPort = dva_g_userPort2sysPort[userPort];
        if((DVA_RTK_SYS_PORT_12TH < sysPort) || (DVA_RTK_SYS_PORT_1ST > sysPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        CHK_FUN_RET(retVal, dva_otp_read(unit, DVA_OTP_FLASHLESS_ENABLE_PORT, (uint8 *)&portEnRead, 2u));
        if(0u == (portEnRead & ((uint16)1u << sysPort)))
        {
            /*Already disabled*/
            continue;
        }
        portEnWrite = portEnRead & ~((uint16)1u << sysPort);
        /* Disable the port */
        CHK_FUN_RET(retVal, dva_otp_write(unit, DVA_OTP_FLASHLESS_ENABLE_PORT, (uint8 *)&portEnWrite, 2u));
        portEnRead = (uint16)0u;
        CHK_FUN_RET(retVal, dva_otp_read(unit, DVA_OTP_FLASHLESS_ENABLE_PORT, (uint8 *)&portEnRead, 2u));
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
 * @brief A function that transitions from the firmware loading stage to the configuration loading stage via etherAccess. \n
 *        It will trigger firmware authentication if firmware exists.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Leave success.
 * @retval Others Please refer to dva_drv_config_eth_write_field(),dva_drv_config_polling_status().
 */
RtkApiRet dva_sys_eth_leave_fw_to_cfg(uint32 unit)
{
    RtkApiRet        retVal = RT_ERR_OK;
    dva_PollingReg_t pollingReg;
    uint32           stage  = DVA_CONFIG_STAGE;
    uint32           status = 0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_write_field(DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_EXIT_CMD_LSP, DVA_EXIT_CMD_LEN, DVA_FLASHLESS_EXIT_CMD));
        /* Polling stage to DVA_CONFIG_STAGE and status no error*/
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STAGE_LSP;
        pollingReg.len          = DVA_STAGE_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &stage;
        pollingReg.timeoutValue = 0x1000u;
        pollingReg.isEth        = 1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STATUS_LSP;
        pollingReg.len          = DVA_STATUS_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &status;
        pollingReg.timeoutValue = 0x1u;
        pollingReg.isEth        = 1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief A function used to update firmware into SRAM via etherAccess in flashless mode.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The image file name to input.
 * @param[out] None
 * @retval RT_ERR_OK Successfully updated firmware for flashless over Ethernet Access.
 * @retval -RT_ERR_FAILED  Firmware update via Ethernet Access failed.
 * @retval Others Please refer to rtl_file_open(),dva_drv_config_polling_status(),
 *                dva_drv_config_get_hdr_fw_info(),rtl_file_read(),dva_eth_acc_flow_patch_write(),
 *                dva_drv_config_eth_update_aes_ctr_iv(),dva_drv_config_eth_write_field().
 * @note A binary with firmware should be input for this function.
 */
RtkApiRet dva_sys_eth_fw_update(uint32 unit, const int8 *fileName)
{
    RtkApiRet        retVal                                    = RT_ERR_OK;
    void            *fp                                        = NULL;
    uint32           fwLength                                  = (uint32)0u;
    uint32           fwAddr                                    = (uint32)0u;
    uint32           offset                                    = (uint32)0u;
    uint32           lenLeft                                   = (uint32)0u;
    uint32           wLen                                      = (uint32)0u;
    uint8            fwKeyType                                 = (uint8)0u;
    uint8            fwBuf[DVA_RTK_ETHERACCESS_BYTE_PER_WRITE] = {(uint8)0u};
    dva_PollingReg_t pollingReg;
    uint32           stage = DVA_FIRMWARE_STAGE;

    do
    {
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STAGE_LSP;
        pollingReg.len          = DVA_STAGE_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &stage;
        pollingReg.timeoutValue = 0x10000u;
        pollingReg.isEth        = 1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
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

        CHK_FUN_CONTINUE(retVal, dva_drv_config_get_hdr_fw_info(fp, &fwAddr, &fwLength, &fwKeyType));
        offset = (uint32)0u;
        while((offset < fwLength) && (RT_ERR_OK == retVal))
        {
            lenLeft = fwLength - offset;
            if(lenLeft > (uint32)DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
            {
                wLen = (uint32)DVA_RTK_ETHERACCESS_BYTE_PER_WRITE;
            }
            else
            {
                wLen = lenLeft;
            }

            if((int32)0 == rtl_file_read(fp, fwBuf, fwAddr + offset, wLen))
            {
#ifdef DEBUG_SYS
                rtlglue_printf("FILE %s read fail!\n", fileName);
#endif
                retVal = -RT_ERR_FAILED;
                continue;
            }
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_flow_patch_write(0u + offset, wLen, fwBuf));
            offset += wLen;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_update_aes_ctr_iv(fp, fwKeyType, fwLength));

        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_write_field(DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_2, 0u, 24u, ((fwLength << 4u) | (fwKeyType & 0xFu))));
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
 * @param[in] userPort Configure user port (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_11TH) \n
 *            Specify the user port connected to link partner to do flashless updating.
 * @param[in] fileName File name (1 to 255 characters) \n
 *            The image file name to input.
 * @param[out] None
 * @retval RT_ERR_OK Successfully updated configuration for flashless over Ethernet Access.
 * @retval Others Please refer to dva_drv_config_load_config_eth().
 * @note A binary with no secure boot firmware should be input for this function.
 */
RtkApiRet dva_sys_eth_cfg_update(uint32 unit, const int8 *fileName)
{
    RtkApiRet        retVal = RT_ERR_OK;
    dva_PollingReg_t pollingReg;
    uint32           trigger = 0u;
    uint32           status  = 0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_config_load_config_eth(unit, fileName));
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("Trigger\n");
#endif
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_write_field(DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_TRIGGER_LSP, 1u, 1u));
        pollingReg.lsp          = DVA_TRIGGER_LSP;
        pollingReg.len          = 1u;
        pollingReg.num          = 1u;
        pollingReg.value        = &trigger;
        pollingReg.timeoutValue = 0x5000u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
        pollingReg.address      = DVA_CPU_COMMAND_IN_FLASHLESS_MODE_PARAMETER_1;
        pollingReg.lsp          = DVA_STATUS_LSP;
        pollingReg.len          = DVA_STATUS_LEN;
        pollingReg.num          = 1u;
        pollingReg.value        = &status;
        pollingReg.timeoutValue = 0x1u;
        pollingReg.isEth        = 0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_config_polling_status(unit, pollingReg));
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("Config done\n");
#endif

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief A function used to disable the RX capability of all ports except the configured port. \n
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort Configure user port (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_11TH) \n
 *            Specify the user port connected to link partner to do flashless updating.
 * @param[out] None
 * @retval RT_ERR_OK Successfully disabled the RX capability of all ports except the configured port.
 * @retval -RT_ERR_PORT_ID The selected port was out of range.
 * @retval Others Please refer to dva_drv_port_eth_disable_rx_en().
 */
RtkApiRet dva_sys_eth_disable_non_configure_port_rx(uint32 unit, RtkPort userPort)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    rxPortMask = DVA_RTK_SYS_PMAP_ALL;
    RtkPort   sysPort    = 0u;

    do
    {
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];
        if((DVA_RTK_SYS_PORT_12TH < sysPort) || (DVA_RTK_SYS_PORT_1ST > sysPort))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        rxPortMask &= ~((uint32)1u << sysPort);
        CHK_FUN_CONTINUE(retVal, dva_drv_port_eth_disable_rx_en(unit, rxPortMask));

    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* FL_ETH_ASIC */
/**@}*/ /* FL_ETH */

/**@}*/ /* System Bootsup */
