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

#include "rtkas_std.h"
#include "rtkas_types.h"
#include "rtl907xc_vb_reg_list.h"
#include "rtl907xc_vb_regField_list.h"
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#include "rtl907xc_vb_asicdrv_flash.h"
#include "rtl907xc_vb_asicdrv_otp.h"
#include "rtl907xc_vb_asicdrv_phy.h"
#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_asicdrv_config.h"
#include "rtl907xc_vb_asicdrv_ptp.h"
#include "rtkas_system_porting.h"
#include "rtl907xc_vb_asicdrv_ethernet_access.h"
#include "rtl907xc_vb_api_serdes.h"
#include "rtl907xc_vb_asicdrv_serdes.h"
#include "rtl907xc_vb_asicdrv_tcam.h"
#include <stdio.h>

#define DEBUG_SYS
/******************************************************************************
 * Global Variables
 ******************************************************************************/
static uint8        cvb_extraAreaAddr                           = 0xFFu;
static const uint32 cvb_g_phyIdxToOcpIdx[CVB_NUM_OF_PORT_T1PHY] = {0x010000u, 0x020000u, 0x030000u, 0x040000u, 0x050000u, 0x060000u, 0x090000u, 0x0A0000u, 0x070000u, 0x080000u};
uint32              cvb_g_pureSDSPowerUp                        = 0u;
/******************************************************************************
 * Function Declaration
 ******************************************************************************/
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_25ua_opfsm_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_brpatch_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_sgmii_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fe_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mac_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rtpg_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_byte_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_reg_field_write_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mka_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rot_sw_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_ptp_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table0_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table1_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table2_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_5gkr_patch_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_25ua_opfsm_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_brpatch_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_sgmii_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fe_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_special_reg_eth(uint32 regAddr, uint32 *regVal, uint32 configPort);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mac_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rtpg_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_byte_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_reg_field_write_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mka_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rot_sw_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_ptp_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table0_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table1_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table2_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_5gkr_patch_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_call_handler_reg(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_call_handler_eth(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp, uint32 configPort);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_5gkr_patch_method_reg(void);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_memcpy_reg(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_memcpy_eth(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_error_reg(uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_error_eth(uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_cfg_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_br(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_rtpg(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_eth_br(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_eth_rtpg(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fw_cfg_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fw_cfg_patch_eth(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_someip_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_someip_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_extphy_patch_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_extphy_patch_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer);

/******************************************************************************
 * Functions Definition
 ******************************************************************************/

RtkApiRet cvb_drv_config_get_hdr_fw_info(void *fp, uint32 *fwAddr, uint32 *fwLength, uint8 *fwKeyType)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     fileBuf[9]  = {(uint8)0u};
    uint16    headerIdent = (uint16)0u;
    do
    {
        /* check the validation of this binary file */
        if((int32)0 == rtl_file_read(fp, fileBuf, CVB_FLASH_HEADER_OFFSET, 9u))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        headerIdent = (uint16)fileBuf[0] | ((uint16)fileBuf[1] << 8);
        printf("0x%x\n",headerIdent);
        if((uint16)CVB_CFG_SPI_IDENTIFY != headerIdent)
        {
#ifdef DEBUG_SYS
            printf("Firmware doesn't exist\n");
            (void)rtlglue_printf("Firmware doesn't exist\n");
#endif
            retVal = -RT_ERR_INPUT;
            continue;
        }

        *fwLength  = ((uint32)fileBuf[7] << 4u) + ((uint32)fileBuf[8] << 12u);
        *fwAddr    = (uint32)fileBuf[4] | ((uint32)fileBuf[5] << 8u) | ((uint32)fileBuf[6] << 16u);
        *fwKeyType = (fileBuf[2] & (uint8)0xCu) >> 2u;

        if(((uint32)0xFFFF0u == *fwLength) || ((uint32)CVB_MAX_FW_LENGTH < *fwLength))
        {
#ifdef DEBUG_SYS
            (void)rtlglue_printf("Firmware length is invalid\n");
#endif
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cvb_drv_config_sys_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if(((uint32)CVB_CFG_BR_PATCH_1_IDX == updateConfig.index) || ((uint32)CVB_CFG_BR_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_phy_patch_br(unit, fileName, updateConfig));
        }
        else if(((uint32)CVB_CFG_RTPG_PATCH_1_IDX == updateConfig.index) || ((uint32)CVB_CFG_RTPG_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_phy_patch_rtpg(unit, fileName, updateConfig));
        }
        else if(((uint32)CVB_CFG_FWCFG_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_fw_cfg_patch(unit, fileName, updateConfig));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, 0));
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cvb_drv_config_eth_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if(((uint32)CVB_CFG_BR_PATCH_1_IDX == updateConfig.index) || ((uint32)CVB_CFG_BR_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_phy_patch_eth_br(unit, fileName, updateConfig, mode));
        }
        else if(((uint32)CVB_CFG_RTPG_PATCH_1_IDX == updateConfig.index) || ((uint32)CVB_CFG_RTPG_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_phy_patch_eth_rtpg(unit, fileName, updateConfig, mode));
        }
        else if(((uint32)CVB_CFG_FWCFG_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_fw_cfg_patch_eth(unit, fileName, updateConfig, mode));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, mode));
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_eth_br(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode)
{

    RtkApiRet    retVal                                    = RT_ERR_OK;
    uint32       i                                         = (uint32)0u;
    const uint32 newPortTable[10]                          = {(uint32)1u, (uint32)2u, (uint32)3u, (uint32)4u, (uint32)5u, (uint32)6u, (uint32)9u, (uint32)10u, (uint32)7u, (uint32)8u};
    uint8        userPort2T1phy[CVB_RTL9075_INTERFACE_NUM] = {0xFFu, 2u, 3u, 4u, 5u, 8u, 9u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        for(i = (uint32)CVB_BR_PORT_START; i <= (uint32)CVB_BR_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_request(unit, newPortTable[i], mode, 0u));
            }
        }

        /* Last packet for patch request */
        CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_request(unit, 0u, mode, 1u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, mode));

        for(i = (uint32)CVB_BR_PORT_START; i <= (uint32)CVB_BR_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_release(unit, newPortTable[i], mode, 0));
            }
        }

        /* Last packet for patch release */
        CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_release(unit, 0u, mode, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_eth_rtpg(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode)
{

    RtkApiRet    retVal                                    = RT_ERR_OK;
    uint32       i                                         = (uint32)0u;
    const uint32 newPortTable[10]                          = {(uint32)1u, (uint32)2u, (uint32)3u, (uint32)4u, (uint32)5u, (uint32)6u, (uint32)9u, (uint32)10u, (uint32)7u, (uint32)8u};
    uint8        userPort2T1phy[CVB_RTL9075_INTERFACE_NUM] = {0xFFu, 2u, 3u, 4u, 5u, 8u, 9u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        for(i = (uint32)CVB_RTPG_PORT_START; i <= (uint32)CVB_RTPG_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_request(unit, newPortTable[i], mode, 0u));
            }
        }

        /* Last packet for patch request */
        CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_request(unit, 0u, mode, 1u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, mode));

        for(i = (uint32)CVB_RTPG_PORT_START; i <= (uint32)CVB_RTPG_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_release(unit, newPortTable[i], mode, 0));
            }
        }

        /* Last packet for patch release */
        CHK_FUN_CONTINUE(retVal, cvb_drv_phy_patch_release(unit, 0u, mode, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_br(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig)
{

    RtkApiRet    retVal           = RT_ERR_OK;
    uint32       i                = (uint32)0u;
    const uint32 newPortTable[10] = {(uint32)1u, (uint32)2u, (uint32)3u, (uint32)4u, (uint32)5u, (uint32)6u, (uint32)9u, (uint32)10u, (uint32)7u, (uint32)8u};

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        for(i = (uint32)CVB_BR_PORT_START; i <= (uint32)CVB_BR_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_phy_patch_request(unit, newPortTable[i], 0u, 0u));
            }
        }

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, 0));

        for(i = (uint32)CVB_BR_PORT_START; i <= (uint32)CVB_BR_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_phy_patch_release(unit, newPortTable[i], 0u, 0u));
            }
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_phy_patch_rtpg(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig)
{

    RtkApiRet    retVal           = RT_ERR_OK;
    uint32       i                = (uint32)0u;
    const uint32 newPortTable[10] = {(uint32)1u, (uint32)2u, (uint32)3u, (uint32)4u, (uint32)5u, (uint32)6u, (uint32)9u, (uint32)10u, (uint32)7u, (uint32)8u};

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        for(i = (uint32)CVB_RTPG_PORT_START; i <= (uint32)CVB_RTPG_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_phy_patch_request(unit, newPortTable[i], 0u, 0u));
            }
        }

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, 0));

        for(i = (uint32)CVB_RTPG_PORT_START; i <= (uint32)CVB_RTPG_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_phy_patch_release(unit, newPortTable[i], 0u, 0u));
            }
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fw_cfg_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig)
{

    RtkApiRet retVal         = RT_ERR_OK;
    uint32    regAddr        = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    brPhyEnMskTmp  = (uint32)0u;
    uint32    brPhyEnMsk     = (uint32)0u;

    do
    {
        regAddr = (uint32)(CVB_GLOBAL_CONFIG_ADDR + CVB_BR_PHY_EN_MSK_OFFSET) / 4u;
        regAddr = regAddr * 4u;
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));

        /* record original g_BRPHYEnMsk */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, regAddr, &brPhyEnMskTmp));
        /*BR_PHY_EN_MSK [31:16]*/
        brPhyEnMsk = brPhyEnMskTmp & 0xFFFF0000u;
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, 0));

        /* Finetune g_BRPHYEnMsk after loading fw_cfg */
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, regAddr, &brPhyEnMskTmp));

        brPhyEnMsk = (brPhyEnMsk & (brPhyEnMskTmp & 0xFFFF0000u)) | (brPhyEnMskTmp & 0xFFFFu);

        CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, regAddr, brPhyEnMsk));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fw_cfg_patch_eth(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode)
{

    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regAddr;
    uint32    brPhyEnMsk = (uint32)0u;

    do
    {
        regAddr = (uint32)(CVB_GLOBAL_CONFIG_ADDR + CVB_BR_PHY_EN_MSK_OFFSET) / 4u;
        regAddr = regAddr * 4u;
        /* record original g_BRPHYEnMsk */
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regValRx, 1u));
        /*BR_PHY_EN_MSK [31:16]*/
        brPhyEnMsk = regValRx[0] & 0xFFFF0000u;
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_cfg_patch(unit, fileName, updateConfig, mode));

        /* Finetune g_BRPHYEnMsk after loading fw_cfg */
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regValRx, 1u));
        brPhyEnMsk = (brPhyEnMsk & (regValRx[0] & 0xFFFF0000u)) | (regValRx[0] & 0xFFFFu);
        /* Set write command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, regAddr, brPhyEnMsk, 0u));
        /* Send write command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cvb_drv_config_check_eth_enable_phy(uint32 *phyPortEnIdx)
{
    RtkApiRet             retVal                                                   = RT_ERR_OK;
    uint32                i                                                        = (uint32)0u;
    uint8                 regAddr[RTK_ETHERACCESS_MAX_BURST - 1][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    uint32                regValue[RTK_ETHERACCESS_MAX_BURST - 1]                  = {(uint32)0u};
    uint8                 rxPkt[CVB_MAX_PKT_LEN], regCmd[RTK_ETHERACCESS_MAX_BURST - 1] = {(uint8)0u};
    uint32                regNum      = (uint32)0u;
    uint32                rxPktLen    = CVB_MAX_PKT_LEN;
    const uint16          opTable[10] = {(uint16)0x1624, (uint16)0x1628, (uint16)0x162c, (uint16)0x1630, (uint16)0x1634, (uint16)0x169c, (uint16)0x16A0, (uint16)0x16A4, (uint16)0x16A8, (uint16)0x16AC};
    EtherAccessMac_t      mac;
    EtherAccessL2Header_t txPkt;

    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        /* Read enabled port */
        for(i = (uint32)0; i < (uint32)10u; i++)
        {
            regCmd[i]     = (uint8)CVB_ETHERACCESS_REGISTER_RW_MAC_READ;
            regAddr[i][0] = (uint8)0u;
            regAddr[i][1] = (uint8)((opTable[i] >> 8u) & 0xFFu);
            regAddr[i][2] = (uint8)(opTable[i] & 0xFFu);
            regValue[i]   = (uint32)0u;
        }

        /* Set dummy reg in last */
        regAddr[i][0] = (uint8)0u;
        regAddr[i][1] = (uint8)0u;
        regAddr[i][2] = (uint8)0u;
        regCmd[i]     = (uint8)0u;
        regValue[i]   = (uint32)0u;
        regNum        = i;

        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVB_RTK_ETHERACCESS_HEADER_LEN + (regNum * 8u) + 8u));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, cvb_eth_acc_check_burst_rw(regNum, regValue, rxPkt));

        /* Check enabled port */
        for(i = (uint32)0u; i < (uint32)10u; i++)
        {
            if((uint32)0u != regValue[i])
            {
                *phyPortEnIdx |= ((uint32)1 << i);
            }
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cvb_drv_config_check_enable_phy(uint32 *phyPortEnIdx)
{
    RtkApiRet    retVal      = RT_ERR_OK;
    uint32       tmpdata     = (uint32)0u;
    uint32       i           = (uint32)0u;
    const uint16 opTable[10] = {(uint16)0x1624, (uint16)0x1628, (uint16)0x162c, (uint16)0x1630, (uint16)0x1634, (uint16)0x169c, (uint16)0x16A0, (uint16)0x16A4, (uint16)0x16A8, (uint16)0x16AC};

    for(i = (uint32)0u; i < (uint32)10u; i++)
    {
        CHK_FUN_BREAK(retVal, cvb_reg_read(CVB_UNIT, opTable[i], &tmpdata));

        if((uint32)0u != tmpdata)
        {
            *phyPortEnIdx |= ((uint32)0x1 << i);
        }
    }
#ifdef DEBUG_SYS
    rtlglue_printf("phyPortEnIdx 0x%x\n", *phyPortEnIdx);
#endif
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_25ua_opfsm_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while((uint32)CVB_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)CVB_PAGE_SWR_OCP | (regAddr << 1));
        CHK_FUN_BREAK(retVal, cvb_reg_field_write(CVB_UNIT, regAddr, 0u, 16u, regVal));

        offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
    }
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_brpatch_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    uint32    offset       = (uint32)0u;
    uint32    i            = (uint32)0u;
    uint32    regAddr      = (uint32)0u;
    uint32    regVal       = (uint32)0u;
    uint32    phyPortEnIdx = (uint32)0u;
    RtkApiRet retVal       = RT_ERR_OK;

    do
    {
        /* Check enabled port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_check_enable_phy(&phyPortEnIdx));
        while((uint32)CVB_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)CVB_BR_PORT_START; i <= (uint32)CVB_BR_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if((uint32)0u != (phyPortEnIdx & ((uint32)1u << i)))
                {
                    regAddr = (uint32)CVB_PAGE_SWR_OCP | ((cvb_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, cvb_reg_field_write(CVB_UNIT, regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_sgmii_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet     retVal       = RT_ERR_OK;
    const uint32  sgPCSmap[4]  = {(uint32)CVB_RTK_USER_PORT_7TH, (uint32)CVB_RTK_USER_PORT_9TH, (uint32)CVB_RTK_USER_PORT_10TH, (uint32)CVB_RTK_USER_PORT_11TH};
    const uint32  sgDPHYmap[4] = {(uint32)CVB_RTK_USER_PORT_7TH, (uint32)CVB_RTK_USER_PORT_9TH, (uint32)CVB_RTK_USER_PORT_10TH, (uint32)CVB_RTK_USER_PORT_11TH};
    const uint32 *portMap      = NULL;
    uint8         portMask     = (uint8)0u;
    uint32        offset       = (uint32)0u;
    uint8         port         = (uint8)0u;
    uint32        page         = (uint32)0u;
    uint32        reg          = (uint32)0u;
    uint32        value        = (uint32)0u;

    while(((uint32)CVB_CONFIG_SGMII_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
    {
        /* parsing configuration entry */
        portMask = buffer[offset];
        if(((portMask & 0x1u) == 0x1u) && (0u == cvb_g_pureSDSPowerUp))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_sds_p8_init(CVB_UNIT));
            cvb_g_pureSDSPowerUp = 1u;
        }

        value = (((uint32)buffer[offset + 4u] << 8) | (uint32)buffer[offset + 3u]);
        if((uint8)0u != (portMask & ((uint8)1u << 7u)))
        {
            /* pcs */
            portMap = sgPCSmap;
            page    = ((uint32)buffer[offset + 1u]);
            reg     = ((uint32)buffer[offset + 2u]);
            for(port = (uint8)0u; port < (uint8)4u; port++)
            {
                if((uint8)0u != (portMask & ((uint8)1u << port)))
                {
                    CHK_FUN_BREAK(retVal, cvb_sys_sds_reg_setbits_pcs(CVB_UNIT, portMap[port], page, reg, 0u, 15u, value));
                }
            }
        }
        else
        {
            /* D-PHY */
            portMap = sgDPHYmap;
            page    = (uint32)0u;
            reg     = (((uint32)buffer[offset + 2u] << 8) | (uint32)buffer[offset + 1u]);
            for(port = (uint8)0u; port < (uint8)4u; port++)
            {
                if((uint8)0u != (portMask & ((uint8)1u << port)))
                {
                    CHK_FUN_BREAK(retVal, cvb_sys_sds_reg_setbits_aphy(CVB_UNIT, portMap[port], page, reg, 0u, 15u, value));
                }
            }
        }
        offset += (uint32)CVB_CONFIG_SGMII_ENTRY_LEN;
    }
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fe_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while((uint32)CVB_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)CVB_PAGE_SWR_OCP | ((CVB_FE_OCP_OFST_ADDR | regAddr) << 1));
        CHK_FUN_BREAK(retVal, cvb_reg_field_write(CVB_UNIT, regAddr, 0u, 16u, regVal));
        offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mac_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    offset           = (uint32)0u;
    bool      waitRegVal       = false;
    uint32    regAddr          = (uint32)0u;
    uint32    regVal           = (uint32)0u;
    uint32    regAddrOtherPage = (uint32)0u;

    do
    {
        while(((uint32)CVB_CONFIG_MAC_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
        {
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8) | ((uint32)buffer[offset + 2u] << 16) | (uint32)CVB_PAGE_SWR_OCP;
            regVal  = (uint32)buffer[offset + 3u] | ((uint32)buffer[offset + 4u] << 8) | ((uint32)buffer[offset + 5u] << 16) | ((uint32)buffer[offset + 6u] << 24);

            if((uint32)0x4AFFFF01u == regAddr)
            {
                regAddrOtherPage = regVal;
                waitRegVal       = true;
            }
            else if((uint32)0x4AFFFF03u == regAddr)
            {
                if(waitRegVal)
                {
                    CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, ((regAddrOtherPage & 0xFF000000u) >> 24u)));
                    CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, (regAddrOtherPage & 0x00FFFFFFu), regVal));
                    CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, 0x4A));
                }

                waitRegVal = false;
            }
            else if(0x0u != regAddr)
            {
                CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, regAddr, regVal));
            }
            else
            {
                /*do nothing*/
            }

            offset += (uint32)CVB_CONFIG_MAC_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));

        /*length must equal to CVB_GLOBAL_CONFIG_SIZE*/
        if((uint32)CVB_GLOBAL_CONFIG_SIZE != configReg.length)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configReg.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, ((i * 4u) + (uint32)CVB_GLOBAL_CONFIG_ADDR), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rtpg_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    uint32    offset       = (uint32)0u;
    uint32    i            = (uint32)0u;
    uint32    regAddr      = (uint32)0u;
    uint32    regVal       = (uint32)0u;
    uint32    phyPortEnIdx = (uint32)0u;
    RtkApiRet retVal       = RT_ERR_OK;

    do
    {
        /* Check enabled port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_check_enable_phy(&phyPortEnIdx));
        while((uint32)CVB_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)CVB_RTPG_PORT_START; i <= (uint32)CVB_RTPG_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if((uint32)0u != (phyPortEnIdx & ((uint32)1u << i)))
                {
                    regAddr = (uint32)CVB_PAGE_SWR_OCP | ((cvb_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, cvb_reg_field_write(CVB_UNIT, regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_byte_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    regAddr        = (uint32)0u;
    uint32    regVal         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));
        while(((uint32)CVB_CONFIG_MAC_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
        {
            /* parsing configuration entry */
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8) | ((uint32)buffer[offset + 2u] << 16);
            regVal  = (uint32)buffer[offset + 3u] | ((uint32)buffer[offset + 4u] << 8) | ((uint32)buffer[offset + 5u] << 16) | ((uint32)buffer[offset + 6u] << 24);
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, regAddr, regVal));

            offset += (uint32)CVB_CONFIG_MAC_ENTRY_LEN;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_reg_field_write_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regAddr  = (uint32)0u;
    uint32    startBit = (uint32)0u;
    uint32    endBit   = (uint32)0u;
    uint32    regVal   = (uint32)0u;
    uint32    offset   = (uint32)0u;

    /* register field write */
    while(((uint32)CVB_CFG_REG_FIELD_WR_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
    {
        /* Load parameter */
        regAddr  = (uint32)CVB_PAGE_SWR_OCP | buffer[offset + 0u] | ((uint32)buffer[offset + 1u] << 8u) | ((uint32)buffer[offset + 2u] << 16u);
        startBit = buffer[offset + 3u];
        endBit   = buffer[offset + 4u];
        regVal   = buffer[offset + 5u] | ((uint32)buffer[offset + 6u] << 8u) | ((uint32)buffer[offset + 7u] << 16u) | ((uint32)buffer[offset + 8u] << 24u);
        if(endBit <= startBit)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(CVB_UNIT, regAddr, startBit, (endBit - startBit), regVal));
        offset += (uint32)CVB_CFG_REG_FIELD_WR_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mka_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));

        /*length must equal to CVB_MKA_CONFIG_SIZE*/
        if((uint32)CVB_MKA_CONFIG_SIZE != configReg.length)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configReg.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, ((i * 4u) + (uint32)CVB_MKA_CONFIG_ADDR), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rot_sw_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));

        offset = configReg.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_ROUTING_TBL_SIZE < (offset + configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /*not 4bytes aligment */
        if((uint32)0u != (configReg.length % ((uint32)4u)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        for(i = (uint32)0u; i < (configReg.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, ((i * 4u) + (uint32)CVB_ROUTING_TBL_ADDR + offset), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_memcpy_reg(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint32)0u;
    uint32    i      = (uint32)0u;
    do
    {
        for(i = (uint32)0u; i < length; i += 4u)
        {
            regVal = (uint32)buffer[i] | ((uint32)buffer[i + 1u] << 8) | ((uint32)buffer[i + 2u] << 16) | ((uint32)buffer[i + 3u] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, (macPhyAddr + CVB_MPY_NIC_CONFIG_HEADER_LEN + offset + i), regVal));
        }
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_error_reg(uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, macPhyAddr, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, macPhyAddr + 4u, 0u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet    retVal          = RT_ERR_OK;
    uint32       currentRegPage  = (uint32)0u;
    uint32       offset          = (uint32)0u;
    uint32       configTotalLen  = (uint32)0u;
    const uint32 macPhyAddrMap[] = {(uint32)CVB_MPY_NIC_BUF_ADDR_0, (uint32)CVB_MPY_NIC_BUF_ADDR_1};
    uint32       macPhyAddr      = macPhyAddrMap[configReg.index - CVB_CFG_MACPHY_0_CONFIG_IDX];

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, CVB_MPY_PAGE));

        offset = configReg.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_MPY_NIC_CONFIG_LIMIT_LEN < (offset + configReg.length))
        {
            /* clean the magic number and length*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_macphy_error_reg(macPhyAddr));
            retVal = -RT_ERR_INPUT;
            continue;
        }
        else
        {
            if((uint32)0u == configReg.blockIdx)
            {
                /* set magic number for identifying the config data */
                CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, macPhyAddr, CVB_MPY_NIC_CONFIG_MAGIC_NUM));
            }

            CHK_FUN_CONTINUE(retVal, cvb_drv_config_macphy_memcpy_reg(buffer, configReg.length, offset, macPhyAddr));

            configTotalLen = offset + configReg.length;
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, macPhyAddr + 4u, configTotalLen));
        }
        if(RT_ERR_OK != retVal)
        {
            /* clean the magic number and length*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_macphy_error_reg(macPhyAddr));
            retVal = -RT_ERR_FAILED;
        }
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_ptp_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));

        offset = configReg.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_PTP_STATUS_SIZE < (offset + configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /*not 4bytes aligment */
        if((uint32)0u != (configReg.length % (uint32)4u))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configReg.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, ((i * 4u) + (uint32)CVB_PTP_STATUS_ADDR + offset), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table0_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                           = RT_ERR_OK;
    uint32    entryNum                         = useLen / (tabLen + CVB_CONFIG_TABLE0_HEADER_LEN);
    uint32    tabAddr                          = (uint32)0u;
    uint32    counter                          = (uint32)0u;
    uint32    i                                = (uint32)0;
    uint32    j                                = (uint32)0u;
    uint32    tabData                          = (uint32)0u;
    uint32    tabTimeOut                       = (uint32)0u;
    uint32    tempRegVal                       = (uint32)0u;
    uint32    tabAddrMultiMap[]                = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                                  0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                                  0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                                  0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    uint32    entryData[CVB_TCAM_DATA_REG_MAX] = {0};
    uint32    dataNum                          = 0u;

    while((i < useLen) && (counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[i + 3u]) << 24u) | ((uint32)(buffer[(i + 2u)]) << 16u) | ((uint32)(buffer[(i + 1u)]) << 8u) | (uint32)buffer[i];
        tabAddr = tabAddr - (CVB_TABLE_BASE_ADDR + (CVB_TABLE_INDEX_GAP * tableType));
        tabAddr = tabAddr / (tabAddrMultiMap[cvb_table_list[tableType].datareg_num]);
        i += (uint32)CVB_CONFIG_TABLE0_HEADER_LEN;

        dataNum = 0u;
        for(j = (uint32)0u; j < tabLen; j += (uint32)CVB_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, (CVB_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE + j), tabData));
            entryData[dataNum] = tabData;
            i += (uint32)CVB_CONFIG_TABLE_DATA_LEN;
            dataNum++;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = (uint32)CVB_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, tabData));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, &tempRegVal));
            if((uint32)0u == (tempRegVal & (uint32)CVB_CONFIG_TABLE_CTRL_BIT))
            {
                break;
            }

            tabTimeOut++;

            if((uint32)0xFFFFu < tabTimeOut)
            {
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
        }

        if(RT_ERR_OK == retVal)
        {
            retVal = cvb_drv_tcam_check(CVB_UNIT, cvb_g_tableTypeList[tableType], tabAddr, entryData);
        }
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table1_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                           = RT_ERR_OK;
    uint32    entryNum                         = (useLen - CVB_CONFIG_TABLE0_HEADER_LEN) / tabLen;
    uint32    tabAddr                          = (uint32)0u;
    uint32    counter                          = (uint32)0u;
    uint32    i                                = (uint32)0;
    uint32    j                                = (uint32)0u;
    uint32    tabData                          = (uint32)0u;
    uint32    tabTimeOut                       = (uint32)0u;
    uint32    tempRegVal                       = (uint32)0u;
    uint32    tabAddrMultiMap[]                = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                                  0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                                  0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                                  0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    uint32    entryData[CVB_TCAM_DATA_REG_MAX] = {0};
    uint32    dataNum                          = 0u;

    tabAddr = CVB_TABLE_BASE_ADDR | ((uint32)(buffer[2u]) << 16u) | ((uint32)(buffer[1u]) << 8u) | (uint32)buffer[0u];
    tabAddr = tabAddr - (CVB_TABLE_BASE_ADDR + (CVB_TABLE_INDEX_GAP * tableType));
    tabAddr = tabAddr / (tabAddrMultiMap[cvb_table_list[tableType].datareg_num]);
    i += (uint32)CVB_CONFIG_TABLE1_HEADER_LEN;

    while((i < useLen) && (counter < entryNum) && (RT_ERR_OK == retVal))
    {
        dataNum = 0u;
        for(j = (uint32)0u; j < tabLen; j += (uint32)CVB_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, (CVB_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE + j), tabData));
            entryData[dataNum] = tabData;
            i += (uint32)CVB_CONFIG_TABLE_DATA_LEN;
            dataNum++;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = (uint32)CVB_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, tabData));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, &tempRegVal));
            if((uint32)0u == (tempRegVal & (uint32)CVB_CONFIG_TABLE_CTRL_BIT))
            {
                break;
            }

            tabTimeOut++;

            if((uint32)0xFFFFu < tabTimeOut)
            {
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
        }

        if(RT_ERR_OK == retVal)
        {
            retVal = cvb_drv_tcam_check(CVB_UNIT, cvb_g_tableTypeList[tableType], tabAddr, entryData);
        }
        tabAddr++;
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table2_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                           = RT_ERR_OK;
    uint32    entryNum                         = useLen / (tabLen + CVB_CONFIG_TABLE2_HEADER_LEN);
    uint32    tabAddr                          = (uint32)0u;
    uint32    counter                          = (uint32)0u;
    uint32    i                                = (uint32)0;
    uint32    j                                = (uint32)0u;
    uint32    tabData                          = (uint32)0u;
    uint32    tabTimeOut                       = (uint32)0u;
    uint32    tempRegVal                       = (uint32)0u;
    uint32    entryData[CVB_TCAM_DATA_REG_MAX] = {0};
    uint32    dataNum                          = 0u;

    while((i < useLen) && (counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[(i + 1u)]) << 8) | (uint32)buffer[i];
        i += (uint32)CVB_CONFIG_TABLE2_HEADER_LEN;

        dataNum = 0u;
        for(j = (uint32)0u; j < tabLen; j += (uint32)CVB_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, (CVB_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE + j), tabData));
            entryData[dataNum] = tabData;
            i += (uint32)CVB_CONFIG_TABLE_DATA_LEN;
            dataNum++;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = (uint32)CVB_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, tabData));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, &tempRegVal));
            if((uint32)0u == (tempRegVal & (uint32)CVB_CONFIG_TABLE_CTRL_BIT))
            {
                break;
            }

            tabTimeOut++;

            if((uint32)0xFFFFu < tabTimeOut)
            {
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
        }

        if(RT_ERR_OK == retVal)
        {
            retVal = cvb_drv_tcam_check(CVB_UNIT, cvb_g_tableTypeList[tableType], tabAddr, entryData);
        }
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    tableType = configReg.index - CVB_CFG_TBL_BASE_IDX;
    uint32    tabLen    = cvb_table_list[tableType].datareg_num * 4u;
    uint32    useLen    = (uint32)0u;
    uint32    mode      = (uint32)0u;
    do
    {
        useLen = configReg.length & 0xFFFFFu;
        mode   = (configReg.length >> 20u) & 0xFu;
        if(((uint32)4u > useLen) || ((uint32)0u == tabLen))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        switch(mode)
        {
            case(0u):
                retVal = cvb_drv_config_table0_handler_reg(tableType, tabLen, buffer, useLen);
                break;
            case(1u):
                retVal = cvb_drv_config_table1_handler_reg(tableType, tabLen, buffer, useLen);
                break;
            case(2u):
                retVal = cvb_drv_config_table2_handler_reg(tableType, tabLen, buffer, useLen);
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_5gkr_patch_method_reg(void)
{
    RtkApiRet    retVal            = RT_ERR_OK;
    uint32       regVal            = (uint32)0u;
    uint32       currentRegPage    = (uint32)0u;
    uint32       i                 = (uint32)0u;
    uint32       serdesPatchMethod = (uint32)0u;
    const uint32 krConfig[6][4]    = {
        {(uint32)CVB_INTERNAL_CPU_VARIABLE4, (uint32)0x1u, (uint32)0x0009u, (uint32)0x0801u},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE5, (uint32)0x1u, (uint32)0x000Eu, (uint32)0x0805u},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE6, (uint32)0x1u, (uint32)0x0011u, (uint32)0x0808u},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE7, (uint32)0x1u, (uint32)0x0015u, (uint32)0x080Bu},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE8, (uint32)0x1u, (uint32)0x00C1u, (uint32)0x080Fu},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE9, (uint32)0x1u, (uint32)0x00C5u, (uint32)0x0812u}};

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, (CVB_GLOBAL_CONFIG_ADDR + CVB_SERDES_PATCH_OFFSET), &regVal));
        serdesPatchMethod = regVal & (uint32)0xffu;
        /* change the register page to original page*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
        if((uint32)0u == serdesPatchMethod)
        {
            for(i = (uint32)0u; i < (uint32)6u; i++)
            {
                if(RT_ERR_OK != retVal)
                {
                    break;
                }
                CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, krConfig[i][0], &regVal));
                CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 31u, 1u, krConfig[i][1], &regVal));
                CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 0u, 12u, krConfig[i][2], &regVal));
                CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 16u, 12u, krConfig[i][3], &regVal));
                CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, krConfig[i][0], regVal));
            }
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_5gkr_patch_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    offset   = (uint32)0u;
    uint32    regAddr  = (uint32)0u;
    uint32    regVal   = (uint32)0u;
    uint32    tmpValue = (uint32)0u;
    do
    {
        /*step1: cfg_nctl_patch_mode_en =1 (Patch ON), port 10~12*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(CVB_UNIT, CVB_INTERNAL_CPU_VARIABLE12, 0u, 3u, 0x7u));

        /*step2: cfg_nctl_cpu_en = 2'b10 (Switch to RAM)*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(CVB_UNIT, CVB_INTERNAL_CPU_VARIABLE0, 14u, 2u, 0x2u));

        while(((uint32)CVB_CONFIG_SDS_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
        {
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
            regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            /*step3: Write RAM Address 12'h0800, 12'h0801, 12'h0802 ... (bit 12 must be 1)*/
            /*Patch control and address*/
            tmpValue = 0u;
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 28u, 2u, 0x3u, &tmpValue));
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 16u, 12u, regAddr, &tmpValue));
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, CVB_INTERNAL_CPU_VARIABLE10, tmpValue));

            /*step4: (Patch code e.g. 0xC677, 0xC009, 0x2800 ... ) unitl all patch cod to RAM is  completed*/
            /*Patch data*/
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(CVB_UNIT, CVB_INTERNAL_CPU_VARIABLE11, 0u, 16u, regVal));

            /*Patch done plus*/
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, CVB_INTERNAL_CPU_VARIABLE10, 0u));

            offset += (uint32)CVB_CONFIG_SDS_ENTRY_LEN;
        }
        /*step5: cfg_nctl_patch_mode_en =0 (Patch OFF)*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(CVB_UNIT, CVB_INTERNAL_CPU_VARIABLE12, 0u, 3u, 0x0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_5gkr_patch_method_reg());
        /*step6: cfg_nctl_cpu_en = 2'b01 (Switch to R0M)*/
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(CVB_UNIT, CVB_INTERNAL_CPU_VARIABLE0, 14u, 2u, 0x1u));

        /*step7: nControl will start from pc=12'h800, RAM Address[0] fetch opcode*/
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_someip_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));

        offset = configReg.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_SOME_IP_CFG_LENGTH < (offset + configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        for(i = (uint32)0u; i < (configReg.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, ((i * 4u) + (uint32)CVB_SOME_IP_ADDR + offset), cfgBuf));
        }

        if((uint32)2u == (configReg.length % ((uint32)4u)))
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8);
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(CVB_UNIT, ((i * 4u) + (uint32)CVB_SOME_IP_ADDR + offset), 0u, 16u, cfgBuf));
        }

        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cvb_drv_extphy_update_code_length_reg(uint32 phyAddr, uint32 idx)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    tempLen = 0u;

    do
    {
        tempLen = phyAddr - (uint32)CVB_EXT_PHY_PATCH_CODE_ADDR + idx;
        if((uint32)CVB_EXT_PHY_PATCH_LENGTH >= tempLen)
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, (uint32)CVB_EXT_PHY_PATCH_LEN_ADDR, tempLen));
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cvb_drv_extphy_code_offset_correction_handler_reg(uint32 entry, uint32 *phyAddr, const uint8 *buf, uint32 *shft)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    temp   = 0u;
    uint32    i      = 0u;

    do
    {
        if(0u != entry)
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_read(CVB_UNIT, *phyAddr, &temp));
            for(i = entry; i < 4u; i++)
            {
                temp |= (uint32)buf[i - entry] << (8u * i);
                (*shft) += 1u;
            }
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, *phyAddr, temp));
            *phyAddr += 4u;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_extphy_patch_handler_reg(cvb_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet     retVal          = RT_ERR_OK;
    uint32        offset          = (uint32)0u;
    uint32        cfgBuf          = (uint32)0u;
    uint32        currentRegPage  = (uint32)0u;
    uint32        i               = (uint32)0u;
    uint32        extPhyConfigLen = (uint32)0u;
    uint32        shift           = (uint32)0u;
    static uint32 extPhyPatchAddr = (uint32)0u;
    static uint32 entryIdx        = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_read(CVB_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, (uint32)0u));

        offset = configReg.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;
        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configReg.length) || ((uint32)CVB_EXT_PHY_PATCH_LENGTH < (offset + configReg.length)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if(0u == configReg.blockIdx)
        {
            extPhyPatchAddr = (uint32)CVB_EXT_PHY_PATCH_CODE_ADDR;
            entryIdx        = 0u;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_code_offset_correction_handler_reg(entryIdx, &extPhyPatchAddr, buffer, &shift));
        }

        extPhyConfigLen = configReg.length - (configReg.length % (uint32)CVB_EXT_PHY_PATCH_ENTRY_LEN) - shift;
        for(i = (uint32)0u; i < (extPhyConfigLen / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[shift + 0u + (i * 4u)] | ((uint32)buffer[shift + 1u + (i * 4u)] << 8) | ((uint32)buffer[shift + 2u + (i * 4u)] << 16) | ((uint32)buffer[shift + 3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_reg_write(CVB_UNIT, extPhyPatchAddr, cfgBuf));
            extPhyPatchAddr += 4u;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        cfgBuf = 0u;
        for(entryIdx = 0u; entryIdx < (extPhyConfigLen % (uint32)4u); entryIdx++)
        {
            cfgBuf |= (uint32)buffer[shift + entryIdx + (i * 4u)] << (8u * entryIdx);
        }
        CHK_FUN_CONTINUE(retVal, cvb_reg_write(CVB_UNIT, extPhyPatchAddr, cfgBuf));

        CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_update_code_length_reg(extPhyPatchAddr, entryIdx));

        CHK_FUN_CONTINUE(retVal, cvb_reg_page_write(CVB_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_25ua_opfsm_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while((uint32)CVB_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)CVB_PAGE_SWR_OCP | (regAddr << 1));
        CHK_FUN_BREAK(retVal, cvb_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));

        offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_brpatch_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset                                    = (uint32)0u;
    uint32    i                                         = (uint32)0u;
    uint32    regAddr                                   = (uint32)0u;
    uint32    regVal                                    = (uint32)0u;
    uint32    phyPortEnIdx                              = (uint32)0u;
    RtkApiRet retVal                                    = RT_ERR_OK;
    uint8     userPort2T1phy[CVB_RTL9075_INTERFACE_NUM] = {0xFFu, 2u, 3u, 4u, 5u, 8u, 9u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        /* Check enabled port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_check_eth_enable_phy(&phyPortEnIdx));
        while((uint32)CVB_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)CVB_BR_PORT_START; i <= (uint32)CVB_BR_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if(((uint32)0u != (phyPortEnIdx & ((uint32)1u << i))) && (i != userPort2T1phy[configEth.configPort]))
                {
                    regAddr = (uint32)CVB_PAGE_SWR_OCP | ((cvb_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, cvb_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_sgmii_pcs_timeout(uint32 regAddr)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    tryTimes                                 = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    regValRx[0] = (uint32)1u;

    while((RT_ERR_OK == retVal) && ((uint32)1u == regValRx[0]))
    {
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        tryTimes++;
        if(tryTimes > (uint32)10000u)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            break;
        }
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_sgmii_handler_eth_pcs(const uint8 *buffer, uint32 offset)
{
    RtkApiRet    retVal                                   = RT_ERR_OK;
    uint32       regAddr                                  = (uint32)0u;
    uint32       regVal                                   = (uint32)0u;
    const uint32 sgPcsChannelBase[4]                      = {(uint32)CVB_S0_CON, (uint32)CVB_US0_CON, (uint32)CVB_US1_CON, (uint32)CVB_US2_CON};
    uint8        port                                     = (uint8)0u;
    uint8        portMask                                 = (uint8)0u;
    uint32       regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    if(CVB_CONFIG_BLOCK_MAX_LENGTH > offset)
    {
        portMask = buffer[offset];
        for(port = (uint8)0u; port < (uint8)4u; port++)
        {
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            if((uint8)0u != (portMask & ((uint8)1u << port)))
            {
                /* read */
                regAddr = sgPcsChannelBase[port] + (uint32)0x4u;
                regVal  = ((uint32)buffer[(offset + 1u)] << 5u) | (uint32)buffer[(offset + 2u)];
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));

                regAddr = sgPcsChannelBase[port];
                regVal  = (uint32)0x01u;
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

                CHK_FUN_CONTINUE(retVal, cvb_drv_config_sgmii_pcs_timeout(regAddr));
                regAddr = sgPcsChannelBase[port] + (uint32)0x8u;
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));

                /*write*/
                regAddr = sgPcsChannelBase[port] + (uint32)0x4u;
                regVal  = ((uint32)buffer[(offset + 1u)] << 5u) | (uint32)buffer[(offset + 2u)];
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
                regAddr = sgPcsChannelBase[port] + (uint32)0xCu;
                regValRx[0] &= (uint32)0xFFFF0000u;
                regVal = regValRx[0] | ((uint32)buffer[(offset + 4u)] << 8u) | (uint32)buffer[(offset + 3u)];
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
                regAddr = sgPcsChannelBase[port];
                regVal  = (uint32)0x03u;
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
            }
        }
    }
    else
    {
        retVal = -RT_ERR_INPUT;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_sgmii_handler_eth_aphy(const uint8 *buffer, uint32 offset)
{
    RtkApiRet    retVal                                   = RT_ERR_OK;
    uint32       regAddr                                  = (uint32)0u;
    uint32       regVal                                   = (uint32)0u;
    const uint32 sgAphyChannelBase[4]                     = {(uint32)CVB_S0_MDIO_CON, (uint32)CVB_US0_MDIO_CON, (uint32)CVB_US1_MDIO_CON, (uint32)CVB_US2_MDIO_CON};
    uint8        port                                     = (uint8)0u;
    uint8        portMask                                 = (uint8)0u;
    uint32       regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    if(CVB_CONFIG_BLOCK_MAX_LENGTH > offset)
    {
        portMask = buffer[offset];

        for(port = (uint8)0u; port < (uint8)4u; port++)
        {
            if((uint8)0u != (portMask & ((uint8)1u << port)))
            {
                /*read*/
                regAddr = sgAphyChannelBase[port] + (uint32)0x4u;
                regVal  = ((uint32)buffer[(offset + 2u)] << 8u) | (uint32)buffer[(offset + 1u)];
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));

                regAddr = sgAphyChannelBase[port];
                regVal  = (uint32)0x00u;
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

                regAddr = sgAphyChannelBase[port] + (uint32)0x8u;
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));

                /*write*/
                regAddr = sgAphyChannelBase[port] + (uint32)0x4u;
                regVal  = ((uint32)buffer[(offset + 2u)] << 8u) | (uint32)buffer[(offset + 1u)];
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));

                regAddr = sgAphyChannelBase[port] + (uint32)0xCu;
                regValRx[0] &= (uint32)0xFFFF0000u;
                regVal = regValRx[0] | ((uint32)buffer[(offset + 4u)] << 8u) | (uint32)buffer[(offset + 3u)];
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));

                regAddr = (uint32)sgAphyChannelBase[port];
                regVal  = (uint32)0x01u;
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
            }
        }
    }
    else
    {
        retVal = -RT_ERR_INPUT;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_sgmii_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    offset   = (uint32)0u;
    uint8     portMask = (uint8)0u;

    do
    {
        while(((uint32)CVB_CONFIG_SGMII_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
        {
            /* parsing configuration entry */
            portMask = buffer[offset];
            if(((portMask & 0x1u) == 0x1u) && (0u == cvb_g_pureSDSPowerUp))
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_sds_p8_init_eth());
                cvb_g_pureSDSPowerUp = 1u;
            }

            if((uint8)0u != (portMask & ((uint8)1u << 7u)))
            {
                /* pcs */
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_sgmii_handler_eth_pcs(buffer, offset));
            }
            else
            {
                /* D-PHY */
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_sgmii_handler_eth_aphy(buffer, offset));
            }
            offset += (uint32)CVB_CONFIG_SGMII_ENTRY_LEN;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fe_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while(CVB_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)CVB_PAGE_SWR_OCP | ((CVB_FE_OCP_OFST_ADDR | regAddr) << 1));
        CHK_FUN_BREAK(retVal, cvb_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));

        offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_special_reg_eth(uint32 regAddr, uint32 *regVal, uint32 configPort)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    portNum          = (uint32)0u;
    uint32    portProperty[11] = {
        CVB_PORT2_PROPERTY_CONFIGURE, CVB_PORT3_PROPERTY_CONFIGURE, CVB_PORT4_PROPERTY_CONFIGURE, CVB_PORT5_PROPERTY_CONFIGURE, CVB_PORT6_PROPERTY_CONFIGURE,
        CVB_PORT7_PROPERTY_CONFIGURE, CVB_PORT8_PROPERTY_CONFIGURE, CVB_PORT9_PROPERTY_CONFIGURE, CVB_PORT10_PROPERTY_CONFIGURE, CVB_PORT11_PROPERTY_CONFIGURE,
        CVB_PORT12_PROPERTY_CONFIGURE};

    if(0u != configPort)
    {
        /* Check L2 MISC rx_en */
        if((uint32)CVB_PORT_L2_MISC == (regAddr & (uint32)CVB_PORT_L2_MISC_MASK))
        {
            portNum = ((regAddr - (uint32)CVB_PORT_L2_MISC) / (uint32)CVB_PORT_L2_MISC_OFFSET);
            /* Enable confiugre port rx_en */
            if((portNum == cvb_g_userPort2sysPort[configPort]) && (2u <= portNum) && (15u >= portNum))
            {
                *regVal |= (uint32)CVB_RX_EN_BIT;
#ifdef DEBUG_SYS
                rtlglue_printf("SPECIAL regAddr %x regVal %x portNum %d\n", regAddr, *regVal, portNum);
#endif
            }
        }
        else if(((uint32)CVB_DESIGN_FOR_MEASTUMENT & (uint32)0xFFFFFFu) == regAddr)
        {
            /*do not disable clk of configPort*/
            portNum = cvb_g_userPort2sysPort[configPort];
            *regVal |= (uint32)1u << (portNum + 8u);
            *regVal |= (uint32)1u << (23u);

#ifdef DEBUG_SYS
            rtlglue_printf("SPECIAL regAddr %x regVal %x portNum %d\n", regAddr, *regVal, portNum);
#endif
        }
        else if((uint32)(portProperty[configPort - 1u] & (uint32)0xFFFFFFu) == regAddr)
        {
            /*do not force configPort link down*/
            *regVal |= (uint32)0x2u;
#ifdef DEBUG_SYS
            rtlglue_printf("SPECIAL regAddr %x regVal %x\n", regAddr, *regVal);
#endif
        }
        else
        {
            /*do nothing*/
        }
    }
    return retVal;
}
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mac_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    offset           = (uint32)0u;
    bool      waitRegVal       = false;
    uint32    regAddr          = (uint32)0u;
    uint32    regVal           = (uint32)0u;
    uint32    regAddrOtherPage = (uint32)0u;

    do
    {
        while(((uint32)CVB_CONFIG_MAC_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
        {
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8) | ((uint32)buffer[offset + 2u] << 16);
            regVal  = (uint32)buffer[offset + 3u] | ((uint32)buffer[offset + 4u] << 8) | ((uint32)buffer[offset + 5u] << 16) | ((uint32)buffer[offset + 6u] << 24);
#ifdef DEBUG_SYS
            rtlglue_printf("regAddr %x regVal %x\n", regAddr, regVal);
#endif

            if((uint32)0xFFFF01u == regAddr)
            {
                regAddrOtherPage = regVal;

                waitRegVal = true;
            }
            else if((uint32)0xFFFF03u == regAddr)
            {
                if(waitRegVal)
                {
                    regAddr = regAddrOtherPage & (uint32)0x00FFFFFFu;

                    CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
                    CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_flow_page_select((regAddrOtherPage >> 24) & 0xFFu));
                    CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
                    CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
                    CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_flow_page_select(0x4A));
                }

                waitRegVal = false;
            }
            else
            {

                CHK_FUN_CONTINUE(retVal, cvb_drv_config_special_reg_eth(regAddr, &regVal, configEth.configPort));

                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
            }
            offset += (uint32)CVB_CONFIG_MAC_ENTRY_LEN;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    cfgBuf = (uint32)0u;
    uint32    i      = (uint32)0u;

    do
    {
        /*length must equal to CVB_GLOBAL_CONFIG_SIZE*/
        if((uint32)CVB_GLOBAL_CONFIG_SIZE != configEth.length)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configEth.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, ((i * 4u) + (uint32)CVB_GLOBAL_CONFIG_ADDR), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rtpg_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset                                    = (uint32)0u;
    uint32    i                                         = (uint32)0u;
    uint32    regAddr                                   = (uint32)0u;
    uint32    regVal                                    = (uint32)0u;
    uint32    phyPortEnIdx                              = (uint32)0u;
    RtkApiRet retVal                                    = RT_ERR_OK;
    uint8     userPort2T1phy[CVB_RTL9075_INTERFACE_NUM] = {0xFFu, 2u, 3u, 4u, 5u, 8u, 9u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        /* Check enabled port */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_check_eth_enable_phy(&phyPortEnIdx));
        while((uint32)CVB_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)CVB_RTPG_PORT_START; i <= (uint32)CVB_RTPG_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if(((uint32)0u != (phyPortEnIdx & ((uint32)1u << i))) && (i != userPort2T1phy[configEth.configPort]))
                {
                    regAddr = (uint32)CVB_PAGE_SWR_OCP | ((cvb_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, cvb_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)CVB_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_fwcfg_byte_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;

    do
    {
        while(((uint32)CVB_CONFIG_MAC_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
        {
            /* parsing configuration entry */
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8) | ((uint32)buffer[offset + 2u] << 16);
            regVal  = (uint32)buffer[offset + 3u] | ((uint32)buffer[offset + 4u] << 8) | ((uint32)buffer[offset + 5u] << 16) | ((uint32)buffer[offset + 6u] << 24);
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, regAddr, regVal, 0u));
            offset += (uint32)CVB_CONFIG_MAC_ENTRY_LEN;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, regAddr, regVal, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_reg_field_write_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regAddr[RTK_ETHERACCESS_MAX_BURST - 1u]  = {(uint32)0u};
    uint32    regVal[RTK_ETHERACCESS_MAX_BURST - 1u]   = {(uint32)0u};
    uint32    offset                                   = (uint32)0u;
    uint16    cnt                                      = (uint16)0u;
    uint8     startBit[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint8)0u};
    uint8     endBit[RTK_ETHERACCESS_MAX_BURST - 1u]   = {(uint8)0u};

    while(((uint32)CVB_CFG_REG_FIELD_WR_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
    {
        /* Load parameter */
        regAddr[cnt]  = buffer[offset + 0u] | ((uint32)buffer[offset + 1u] << 8u) | ((uint32)buffer[offset + 2u] << 16u);
        startBit[cnt] = buffer[offset + 3u];
        endBit[cnt]   = buffer[offset + 4u];
        regVal[cnt]   = buffer[offset + 5u] | ((uint32)buffer[offset + 6u] << 8u) | ((uint32)buffer[offset + 7u] << 16u) | ((uint32)buffer[offset + 8u] << 24u);
#ifdef DEBUG_SYS
        rtlglue_printf("regAddr %x startBit %d, endBit %d regVal %x\n", regAddr[cnt], startBit[cnt], endBit[cnt], regVal[cnt]);
#endif

        /* Set read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[cnt], regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[cnt], regValRx, 1u));
        /* Register field set */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(regAddr[cnt], (uint32)startBit[cnt], ((uint32)endBit[cnt] - (uint32)startBit[cnt]), regVal[cnt], &regValRx[cnt]));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_special_reg_eth(regAddr[cnt], &regValRx[cnt], configEth.configPort));
#ifdef DEBUG_SYS
        rtlglue_printf("regAddr %x regVal %x\n", regAddr[cnt], regVal[cnt]);
#endif

        /* Set write command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr[cnt], regValRx[cnt], 0u));
        /* Send write command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        offset += (uint32)CVB_CFG_REG_FIELD_WR_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_mka_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    cfgBuf = (uint32)0u;
    uint32    i      = (uint32)0u;

    do
    {
        /*length must equal to CVB_MKA_CONFIG_SIZE*/
        if((uint32)CVB_MKA_CONFIG_SIZE != (configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configEth.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, ((i * 4u) + (uint32)CVB_MKA_CONFIG_ADDR), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_rot_sw_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    offset = (uint32)0u;
    uint32    cfgBuf = (uint32)0u;
    uint32    i      = (uint32)0u;

    do
    {

        offset = configEth.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_ROUTING_TBL_SIZE < (offset + configEth.length))
        {
            retVal = -RT_ERR_INPUT;
        }
        /*not 4bytes aligment */
        if((uint32)0u != (configEth.length % ((uint32)4u)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configEth.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, ((i * 4u) + (uint32)CVB_ROUTING_TBL_ADDR + offset), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_memcpy_eth(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint32)0u;
    uint32    i      = (uint32)0u;
    do
    {
        for(i = (uint32)0u; i < length; i += 4u)
        {
            regVal = (uint32)buffer[i] | ((uint32)buffer[i + 1u] << 8) | ((uint32)buffer[i + 2u] << 16) | ((uint32)buffer[i + 3u] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, (macPhyAddr + CVB_MPY_NIC_CONFIG_HEADER_LEN + offset + i), regVal, 0u));
        }
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_error_eth(uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr, 0u, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr + 4u, 0u, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_macphy_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet     retVal          = RT_ERR_OK;
    static uint32 configTotalLen  = (uint32)0u;
    uint32        offset          = (uint32)0u;
    const uint32  macPhyAddrMap[] = {(uint32)CVB_MPY_NIC_BUF_ADDR_0, (uint32)CVB_MPY_NIC_BUF_ADDR_1};
    uint32        macPhyAddr      = macPhyAddrMap[configEth.index - CVB_CFG_MACPHY_0_CONFIG_IDX];
    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_flow_page_select(CVB_MPY_PAGE));
        offset = configEth.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_MPY_NIC_CONFIG_LIMIT_LEN < (offset + configEth.length))
        {
            /* clean the magic number and length*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_macphy_error_eth(macPhyAddr));
            retVal = -RT_ERR_INPUT;
            continue;
        }
        else
        {
            if((uint32)0u == configEth.blockIdx)
            {
                /* set magic number for identifying the config data */
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr, CVB_MPY_NIC_CONFIG_MAGIC_NUM, 0u));
            }

            CHK_FUN_CONTINUE(retVal, cvb_drv_config_macphy_memcpy_eth(buffer, configEth.length, offset, macPhyAddr));

            configTotalLen = offset + configEth.length;
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr + 4u, configTotalLen, 0u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        }
        if(RT_ERR_OK != retVal)
        {
            /* clean the magic number and length */
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_macphy_error_eth(macPhyAddr));
        }
        CHK_FUN_CONTINUE(retVal, cvb_drv_eth_acc_flow_page_select(0x4Au));
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_ptp_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    offset = (uint32)0u;
    uint32    cfgBuf = (uint32)0u;
    uint32    i      = (uint32)0u;

    do
    {

        offset = configEth.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_PTP_STATUS_SIZE < (offset + configEth.length))
        {
            retVal = -RT_ERR_INPUT;
        }
        /*not 4bytes aligment */
        if((uint32)0u != (configEth.length % ((uint32)4u)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configEth.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, ((i * 4u) + (uint32)CVB_PTP_STATUS_ADDR + offset), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table0_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    entryNum                                 = useLen / (tabLen + CVB_CONFIG_TABLE0_HEADER_LEN);
    uint32    tabAddr                                  = (uint32)0u;
    uint32    counter                                  = (uint32)0u;
    uint32    i                                        = (uint32)0;
    uint32    j                                        = (uint32)0u;
    uint32    tabData                                  = (uint32)0u;
    uint32    tabTimeOut                               = (uint32)0u;
    uint32    tabAddrMultiMap[]                        = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                                          0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    entryData[CVB_TCAM_DATA_REG_MAX]         = {0};
    uint32    dataNum                                  = 0u;

    while((i < useLen) && (counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[i + 3u]) << 24u) | ((uint32)(buffer[(i + 2u)]) << 16u) | ((uint32)(buffer[(i + 1u)]) << 8u) | (uint32)buffer[i];
        tabAddr = tabAddr - (CVB_TABLE_BASE_ADDR + (CVB_TABLE_INDEX_GAP * tableType));
        tabAddr = tabAddr / (tabAddrMultiMap[cvb_table_list[tableType].datareg_num]);
        i += (uint32)CVB_CONFIG_TABLE0_HEADER_LEN;

        dataNum = 0u;
        for(j = (uint32)0u; j < tabLen; j += (uint32)CVB_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, (CVB_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE + j), tabData, 0u));
            entryData[dataNum] = tabData;
            i += (uint32)CVB_CONFIG_TABLE_DATA_LEN;
            dataNum++;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = CVB_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, tabData, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, regValRx, 0u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, regValRx, 1u));
            if((uint32)0u == (regValRx[0] & (uint32)CVB_CONFIG_TABLE_CTRL_BIT))
            {
                break;
            }

            tabTimeOut++;

            if((uint32)0xFFFFu < tabTimeOut)
            {
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
        }

        if(RT_ERR_OK == retVal)
        {
            retVal = cvb_drv_tcam_check(CVB_UNIT, cvb_g_tableTypeList[tableType], tabAddr, entryData);
        }
        ++counter;
    }

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table1_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    entryNum                                 = (useLen - CVB_CONFIG_TABLE1_HEADER_LEN) / tabLen;
    uint32    tabAddr                                  = (uint32)0u;
    uint32    counter                                  = (uint32)0u;
    uint32    i                                        = (uint32)0;
    uint32    j                                        = (uint32)0u;
    uint32    tabData                                  = (uint32)0u;
    uint32    tabTimeOut                               = (uint32)0u;
    uint32    tabAddrMultiMap[]                        = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                                          0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    entryData[CVB_TCAM_DATA_REG_MAX]         = {0};
    uint32    dataNum                                  = 0u;

    tabAddr = CVB_TABLE_BASE_ADDR | ((uint32)(buffer[2u]) << 16u) | ((uint32)(buffer[1u]) << 8u) | (uint32)buffer[0u];
    tabAddr = tabAddr - (CVB_TABLE_BASE_ADDR + (CVB_TABLE_INDEX_GAP * tableType));
    tabAddr = tabAddr / (tabAddrMultiMap[cvb_table_list[tableType].datareg_num]);
    i += (uint32)CVB_CONFIG_TABLE1_HEADER_LEN;

    while((i < useLen) && (counter < entryNum) && (RT_ERR_OK == retVal))
    {
        dataNum = 0u;
        for(j = (uint32)0u; j < tabLen; j += (uint32)CVB_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, (CVB_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE + j), tabData, 0u));
            entryData[dataNum] = tabData;
            i += (uint32)CVB_CONFIG_TABLE_DATA_LEN;
            dataNum++;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = CVB_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, tabData, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, regValRx, 0u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, regValRx, 1u));
            if((uint32)0u == (regValRx[0] & (uint32)CVB_CONFIG_TABLE_CTRL_BIT))
            {
                break;
            }

            tabTimeOut++;

            if((uint32)0xFFFFu < tabTimeOut)
            {
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
        }

        if(RT_ERR_OK == retVal)
        {
            retVal = cvb_drv_tcam_check(CVB_UNIT, cvb_g_tableTypeList[tableType], tabAddr, entryData);
        }
        ++counter;
        tabAddr++;
    }

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table2_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    entryNum                                 = useLen / (tabLen + CVB_CONFIG_TABLE2_HEADER_LEN);
    uint32    tabAddr                                  = (uint32)0u;
    uint32    counter                                  = (uint32)0u;
    uint32    i                                        = (uint32)0;
    uint32    j                                        = (uint32)0u;
    uint32    tabData                                  = (uint32)0u;
    uint32    tabTimeOut                               = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    entryData[CVB_TCAM_DATA_REG_MAX]         = {0};
    uint32    dataNum                                  = 0u;

    while((i < useLen) && (counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[(i + 1u)]) << 8) | (uint32)buffer[i];
        i += (uint32)CVB_CONFIG_TABLE2_HEADER_LEN;

        dataNum = 0u;
        for(j = (uint32)0u; j < tabLen; j += (uint32)CVB_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, (CVB_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE + j), tabData, 0u));
            entryData[dataNum] = tabData;
            i += (uint32)CVB_CONFIG_TABLE_DATA_LEN;
            dataNum++;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = CVB_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, tabData, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, regValRx, 0u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, CVB_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE, regValRx, 1u));
            if((uint32)0u == (regValRx[0] & (uint32)CVB_CONFIG_TABLE_CTRL_BIT))
            {
                break;
            }

            tabTimeOut++;

            if((uint32)0xFFFFu < tabTimeOut)
            {
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
        }

        if(RT_ERR_OK == retVal)
        {
            retVal = cvb_drv_tcam_check(CVB_UNIT, cvb_g_tableTypeList[tableType], tabAddr, entryData);
        }
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_table_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    tableType = configEth.index - CVB_CFG_TBL_BASE_IDX;
    uint32    tabLen    = cvb_table_list[tableType].datareg_num * 4u;
    uint32    useLen    = (uint32)0u;
    uint32    mode      = (uint32)0u;

    do
    {
        useLen = configEth.length & 0xFFFFFu;
        mode   = (configEth.length >> 20u) & 0xFu;
        if(((uint32)4u > useLen) || ((uint32)0u == tabLen))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        switch(mode)
        {
            case(0u):
                retVal = cvb_drv_config_table0_handler_eth(tableType, tabLen, buffer, useLen);
                break;
            case(1u):
                retVal = cvb_drv_config_table1_handler_eth(tableType, tabLen, buffer, useLen);
                break;
            case(2u):
                retVal = cvb_drv_config_table2_handler_eth(tableType, tabLen, buffer, useLen);
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_5gkr_patch_method_eth(void)
{
    RtkApiRet    retVal                                   = RT_ERR_OK;
    uint32       regVal                                   = (uint32)0u;
    uint32       i                                        = (uint32)0u;
    uint32       serdesPatchMethod                        = (uint32)0u;
    uint32       regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    const uint32 krConfig[6][4]                           = {
        {(uint32)CVB_INTERNAL_CPU_VARIABLE4, (uint32)0x1u, (uint32)0x0009u, (uint32)0x0801u},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE5, (uint32)0x1u, (uint32)0x000Eu, (uint32)0x0805u},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE6, (uint32)0x1u, (uint32)0x0011u, (uint32)0x0808u},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE7, (uint32)0x1u, (uint32)0x0015u, (uint32)0x080Bu},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE8, (uint32)0x1u, (uint32)0x00C1u, (uint32)0x080Fu},
        {(uint32)CVB_INTERNAL_CPU_VARIABLE9, (uint32)0x1u, (uint32)0x00C5u, (uint32)0x0812u}};

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, (CVB_GLOBAL_CONFIG_ADDR + CVB_SERDES_PATCH_OFFSET), regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, (CVB_GLOBAL_CONFIG_ADDR + CVB_SERDES_PATCH_OFFSET), regValRx, 1u));
        serdesPatchMethod = regValRx[0] & (uint32)0xffu;
        if((uint32)0u == serdesPatchMethod)
        {
            for(i = (uint32)0u; i < (uint32)6u; i++)
            {
                if(RT_ERR_OK != retVal)
                {
                    break;
                }
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, krConfig[i][0], regValRx, 0u));
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, krConfig[i][0], regValRx, 1u));
                regVal = regValRx[0];
                CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 31u, 1u, krConfig[i][1], &regVal));
                CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 0u, 12u, krConfig[i][2], &regVal));
                CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 16u, 12u, krConfig[i][3], &regVal));
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, krConfig[i][0], regVal, 0u));
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
            }
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_5gkr_patch_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    offset   = (uint32)0u;
    uint32    regAddr  = (uint32)0u;
    uint32    regVal   = (uint32)0u;
    uint32    tmpValue = (uint32)0u;
    do
    {
        /*step1: cfg_nctl_patch_mode_en =1 (Patch ON), port 10~12*/
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field(CVB_INTERNAL_CPU_VARIABLE12, 0u, 3u, 0x7u));

        /*step2: cfg_nctl_cpu_en = 2'b10 (Switch to RAM)*/
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field(CVB_INTERNAL_CPU_VARIABLE0, 14u, 2u, 0x2u));

        while(((uint32)CVB_CONFIG_SDS_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
        {
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
            regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            /*step3: Write RAM Address 12'h0800, 12'h0801, 12'h0802 ... (bit 12 must be 1)*/
            /*Patch control and address*/
            tmpValue = 0u;
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 28u, 2u, 0x3u, &tmpValue));
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(0u, 16u, 12u, regAddr, &tmpValue));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVB_INTERNAL_CPU_VARIABLE10, tmpValue, 0u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

            /*step4: (Patch code e.g. 0xC677, 0xC009, 0x2800 ... ) unitl all patch cod to RAM is  completed*/
            /*Patch data*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field(CVB_INTERNAL_CPU_VARIABLE11, 0u, 16u, regVal));

            /*Patch done plus*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVB_INTERNAL_CPU_VARIABLE10, 0u, 0u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

            offset += (uint32)CVB_CONFIG_SDS_ENTRY_LEN;
        }
        /*step5: cfg_nctl_patch_mode_en =0 (Patch OFF)*/
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field(CVB_INTERNAL_CPU_VARIABLE12, 0u, 3u, 0x0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_5gkr_patch_method_eth());
        /*step6: cfg_nctl_cpu_en = 2'b01 (Switch to R0M)*/
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field(CVB_INTERNAL_CPU_VARIABLE0, 14u, 2u, 0x1u));

        /*step7: nControl will start from pc=12'h800, RAM Address[0] fetch opcode*/
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_someip_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    offset                                   = (uint32)0u;
    uint32    cfgBuf                                   = (uint32)0u;
    uint32    i                                        = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regVal;

    do
    {

        offset = configEth.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)CVB_SOME_IP_CFG_LENGTH < (offset + configEth.length))
        {
            retVal = -RT_ERR_INPUT;
        }

        for(i = (uint32)0u; i < (configEth.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, ((i * 4u) + (uint32)CVB_SOME_IP_ADDR + offset), cfgBuf, 0u));
        }

        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));

        if((uint32)2u == (configEth.length % ((uint32)4u)))
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8);

            /* Set read command */
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, ((i * 4u) + (uint32)CVB_SOME_IP_ADDR + offset), regValRx, 0u));
            /* Send read command */
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, ((i * 4u) + (uint32)CVB_SOME_IP_ADDR + offset), regValRx, 1u));
            regVal = regValRx[0];
            /* Register field set */
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(((i * 4u) + (uint32)CVB_SOME_IP_ADDR + offset), 0u, 16u, cfgBuf, &regVal));
            /* Set write command */
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, ((i * 4u) + (uint32)CVB_SOME_IP_ADDR + offset), regVal, 0u));
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cvb_drv_extphy_eth_read_handler(uint32 regAddr, uint32 *regVal)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regVal, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regVal, 1u));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cvb_drv_extphy_eth_write_handler(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, regAddr, regVal, 0u));
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cvb_drv_extphy_update_code_length_eth(uint32 phyAddr, uint32 idx)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    tempLen = 0u;

    do
    {
        tempLen = phyAddr - (uint32)CVB_EXT_PHY_PATCH_CODE_ADDR + idx;
        if((uint32)CVB_EXT_PHY_PATCH_LENGTH >= tempLen)
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_eth_write_handler((uint32)CVB_EXT_PHY_PATCH_LEN_ADDR, tempLen));
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet cvb_drv_extphy_code_offset_correction_handler_eth(uint32 entry, uint32 *phyAddr, uint32 *regRx, const uint8 *buf, uint32 *shft)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    temp   = 0u;
    uint32    i      = 0u;

    do
    {
        if(0u != entry)
        {
            /* Set read command */
            CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_eth_read_handler(*phyAddr, regRx));
            temp = regRx[0];
            for(i = entry; i < 4u; i++)
            {
                temp |= (uint32)buf[i - entry] << (8u * i);
                (*shft) += 1u;
            }
            /* Set write command */
            CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_eth_write_handler(*phyAddr, temp));
            *phyAddr += 4u;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_extphy_patch_handler_eth(cvb_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet     retVal                                   = RT_ERR_OK;
    uint32        offset                                   = (uint32)0u;
    uint32        cfgBuf                                   = (uint32)0u;
    uint32        i                                        = (uint32)0u;
    uint32        regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32        extPhyConfigLen                          = (uint32)0u;
    uint32        shift                                    = (uint32)0u;
    static uint32 extPhyPatchAddr                          = (uint32)0u;
    static uint32 entryIdx                                 = (uint32)0u;

    do
    {
        offset = configEth.blockIdx * (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH;
        if((0xFFFFFu < offset) || (CVB_CONFIG_BLOCK_MAX_LENGTH < configEth.length) || ((uint32)CVB_EXT_PHY_PATCH_LENGTH < (offset + configEth.length)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if(0u == configEth.blockIdx)
        {
            extPhyPatchAddr = (uint32)CVB_EXT_PHY_PATCH_CODE_ADDR;
            entryIdx        = 0u;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_code_offset_correction_handler_eth(entryIdx, &extPhyPatchAddr, regValRx, buffer, &shift));
        }

        extPhyConfigLen = configEth.length - (configEth.length % (uint32)CVB_EXT_PHY_PATCH_ENTRY_LEN) - shift;
        for(i = (uint32)0u; i < (extPhyConfigLen / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[shift + 0u + (i * 4u)] | ((uint32)buffer[shift + 1u + (i * 4u)] << 8) | ((uint32)buffer[shift + 2u + (i * 4u)] << 16) | ((uint32)buffer[shift + 3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, extPhyPatchAddr, cfgBuf, 0u));
            extPhyPatchAddr += 4u;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));

        cfgBuf = 0u;
        for(entryIdx = 0u; entryIdx < (extPhyConfigLen % (uint32)4u); entryIdx++)
        {
            cfgBuf |= (uint32)buffer[shift + entryIdx + (i * 4u)] << (8u * entryIdx);
        }
        /* Set write command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_eth_write_handler(extPhyPatchAddr, cfgBuf));

        CHK_FUN_CONTINUE(retVal, cvb_drv_extphy_update_code_length_eth(extPhyPatchAddr, entryIdx));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief
 *
 * @param[in] fileName
 * @param[in] spiStartAddr
 * @param[in] spiParaLen
 * @param[in] stkIdx 0=master,1=stk0...
 * @retval
 */
RtkApiRet cvb_drv_config_parse_flash_cfg_header(const int8 *fileName, uint32 spiStartAddr[], uint32 spiParaLen[], uint8 stkIdx)
{
    uint16    i                                   = (uint16)0u;
    uint32    spiValid                            = (uint32)0xFFFFu;
    uint8     configAddrLenBuf[5]                 = {(uint8)0u};
    uint32    configAddr                          = (uint32)0u;
    uint32    configLength                        = (uint32)0u;
    uint8     hdrBuf[CVB_FLASH_CONFIG_HEADER_LEN] = {(uint8)0u};
    uint32    domainLen                           = (uint32)0u;
    uint32    blockNum                            = (uint32)0u;
    uint32    offset                              = (uint32)2u;
    void     *fp                                  = NULL;
    RtkApiRet retVal                              = RT_ERR_OK;

    do
    {
        if((uint8)3u < stkIdx)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(fp == NULL)
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        /* Get config Address and Length from header */
        if((int32)0 == rtl_file_read(fp, configAddrLenBuf, CVB_FLASH_HEADER_OFFSET + 9u + (stkIdx * 5u), 5u))
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s read fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }
        configLength = ((uint32)configAddrLenBuf[3] << 4u) + ((uint32)configAddrLenBuf[4] << 12u);
        configAddr   = (uint32)configAddrLenBuf[0] | ((uint32)configAddrLenBuf[1] << 8u) | ((uint32)configAddrLenBuf[2] << 16u);
        if((uint32)0xFFFF0u == configLength)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if((int32)0 == rtl_file_read(fp, hdrBuf, configAddr, CVB_FLASH_CONFIG_HEADER_LEN))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        spiValid = (uint32)hdrBuf[0] | ((uint32)hdrBuf[1] << 8u);

        if((uint32)CVB_CFG_SPI_IDENTIFY != spiValid)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        spiStartAddr[0] = configAddr + (uint32)CVB_FLASH_CONFIG_HEADER_LEN;
        for(i = 0u; i < CVB_CFG_END_IDX; i++)
        {
            domainLen     = ((uint32)hdrBuf[offset] | ((uint32)hdrBuf[offset + 1u] << 8) | ((uint32)hdrBuf[offset + 2u] << 16));
            spiParaLen[i] = domainLen;
            if((uint16)1u <= i)
            {
                spiStartAddr[i] = spiStartAddr[i - 1u];
                if((uint32)0u != spiParaLen[i - 1u])
                {
                    spiStartAddr[i] += (spiParaLen[i - 1u] & (uint32)0xFFFFF);
                    /* add padding length,                                                        */
                    /* the previous domain will padding to multiple of 16 before calculate hash   */
                    spiStartAddr[i] += (((uint32)CVB_CONFIG_BLOCK_ALIGNMENT_LENGTH - ((spiParaLen[i - 1u] & (uint32)0xFFFFF) % (uint32)CVB_CONFIG_BLOCK_ALIGNMENT_LENGTH)) % (uint32)CVB_CONFIG_BLOCK_ALIGNMENT_LENGTH);
                    /* add hash length for each block of previous domain */
                    blockNum = ((((spiParaLen[i - 1u] & (uint32)0xFFFFF) - 1u) / (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH) + (uint32)1u);
                    spiStartAddr[i] += (blockNum * (uint32)CVB_CONFIG_BLOCK_HASH_LENGTH);
                }
            }
            offset += CVB_FLASH_DOMAIN_LEN;
        }
    } while(0u == 1u);

    rtl_file_close(&fp);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_call_handler_reg(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp)
{
    RtkApiRet                   retVal                               = RT_ERR_OK;
    uint32                      dataLen                              = (uint32)0u;
    uint32                      readLen                              = (uint32)0u;
    uint8                       dataBuf[CVB_CONFIG_BLOCK_MAX_LENGTH] = {(uint8)0u};
    uint32                      blockIdx                             = (uint32)0u;
    uint32                      paraLen                              = (uint32)0u;
    uint32                      readAddr                             = (uint32)0u;
    uint32                      flags                                = (uint32)0u;
    uint32                      i                                    = (uint32)0u;
    cvb_ConfigReg_t             configReg;
    static cvb_ConfigHandlerReg configHandlersReg[CVB_CFG_END_IDX] =
        {
            [CVB_CFG_25UA_IDX]            = &cvb_drv_config_25ua_opfsm_handler_reg,
            [CVB_CFG_OPFSM_IDX]           = &cvb_drv_config_25ua_opfsm_handler_reg,
            [CVB_CFG_BR_PATCH_1_IDX]      = &cvb_drv_config_brpatch_handler_reg,
            [CVB_CFG_BR_PATCH_2_IDX]      = &cvb_drv_config_brpatch_handler_reg,
            [CVB_CFG_SGMII_PATCH_IDX]     = &cvb_drv_config_sgmii_handler_reg,
            [CVB_CFG_FE_PATCH_IDX]        = &cvb_drv_config_fe_handler_reg,
            [CVB_CFG_MAC_CONF_IDX]        = &cvb_drv_config_mac_handler_reg,
            [CVB_CFG_SOMEIP_IDX]          = &cvb_drv_config_someip_handler_reg,
            [CVB_CFG_FWCFG_IDX]           = &cvb_drv_config_fwcfg_handler_reg,
            [CVB_CFG_RTPG_PATCH_1_IDX]    = &cvb_drv_config_rtpg_handler_reg,
            [CVB_CFG_RTPG_PATCH_2_IDX]    = &cvb_drv_config_rtpg_handler_reg,
            [CVB_CFG_STACKING_IDX]        = &cvb_drv_config_extphy_patch_handler_reg,
            [CVB_CFG_PCIE_IDX]            = NULL,
            [CVB_CFG_FWCFG_BYTE_IDX]      = &cvb_drv_config_fwcfg_byte_handler_reg,
            [CVB_CFG_REG_FIELD_WR_IDX]    = &cvb_drv_config_reg_field_write_handler_reg,
            [CVB_CFG_MKA_IDX]             = &cvb_drv_config_mka_handler_reg,
            [CVB_CFG_RT_GLB_CFG]          = &cvb_drv_config_rot_sw_handler_reg,
            [CVB_CFG_ROT_SW_INTF_TABLE]   = NULL,
            [CVB_CFG_MACPHY_0_CONFIG_IDX] = &cvb_drv_config_macphy_handler_reg,
            [CVB_CFG_MACPHY_1_CONFIG_IDX] = &cvb_drv_config_macphy_handler_reg,
            [CVB_CFG_PTP_IDX]             = &cvb_drv_config_ptp_handler_reg,
            [CVB_CFG_QCC_PRE_TBL_IDX]     = NULL,
            [CVB_CFG_5KR_PATCH_IDX]       = &cvb_drv_config_5gkr_patch_handler_reg};

    for(i = CVB_CFG_TBL_BASE_IDX; i <= CVB_CFG_TBL_END_IDX; i++)
    {
        configHandlersReg[i] = &cvb_drv_config_table_handler_reg;
    }
    readAddr = startAddr;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(&configReg, 0, sizeof(configReg)));
        paraLen = (imgParaLen & (uint32)0x0FFFFFu);
        flags   = (imgParaLen & (uint32)0xF00000u);
        if(NULL != configHandlersReg[index])
        {
            while((RT_ERR_OK == retVal) && ((uint32)0u < paraLen))
            {
                dataLen = ((paraLen > (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH) ? (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH : paraLen);

                /* the readLen should include padding len if exist */
                readLen = ((dataLen + (uint32)CVB_CONFIG_BLOCK_ALIGNMENT_LENGTH - (uint32)1u) & (uint32)0xFFF0u);
                if((int32)0 == rtl_file_read(fp, dataBuf, readAddr, dataLen))
                {
                    retVal = -RT_ERR_FAILED;
                    continue;
                }
                configReg.blockIdx = blockIdx;
                configReg.index    = index;
                configReg.length   = flags | dataLen;
                /* pass flags to domain handler by using parameter length[23:20] */
                retVal = configHandlersReg[index](configReg, dataBuf);
                paraLen -= dataLen;
                if(((uint32)CVB_MAXIMUM_BLOCK_NUMBER < blockIdx) || (CVB_FLASH_ZONE_SIZE_8MB < readAddr))
                {
                    retVal = -RT_ERR_FAILED;
                    continue;
                }
                readAddr += (readLen + (uint32)CVB_CONFIG_BLOCK_HASH_LENGTH);
                blockIdx++;
            }
        }

    } while(0u == 1u);

    if((RT_ERR_OK != retVal) && (-RT_ERR_INPUT != retVal) && (-RT_ERR_BUSYWAIT_TIMEOUT != retVal))
    {
        retVal = -RT_ERR_FAILED;
    }
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet cvb_drv_config_call_handler_eth(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp, uint32 configPort)
{
    RtkApiRet                   retVal                               = RT_ERR_OK;
    uint32                      dataLen                              = (uint32)0u;
    uint32                      readLen                              = (uint32)0u;
    uint8                       dataBuf[CVB_CONFIG_BLOCK_MAX_LENGTH] = {(uint8)0u};
    uint32                      blockIdx                             = (uint32)0u;
    uint32                      paraLen                              = (uint32)0u;
    uint32                      readAddr                             = (uint32)0u;
    uint32                      flags                                = (uint32)0u;
    uint32                      i                                    = (uint32)0u;
    cvb_ConfigEth_t             configEth;
    static cvb_ConfigHandlerEth configHandlersEth[CVB_CFG_END_IDX] =
        {
            [CVB_CFG_25UA_IDX]            = &cvb_drv_config_25ua_opfsm_handler_eth,
            [CVB_CFG_OPFSM_IDX]           = &cvb_drv_config_25ua_opfsm_handler_eth,
            [CVB_CFG_BR_PATCH_1_IDX]      = &cvb_drv_config_brpatch_handler_eth,
            [CVB_CFG_BR_PATCH_2_IDX]      = &cvb_drv_config_brpatch_handler_eth,
            [CVB_CFG_SGMII_PATCH_IDX]     = &cvb_drv_config_sgmii_handler_eth,
            [CVB_CFG_FE_PATCH_IDX]        = &cvb_drv_config_fe_handler_eth,
            [CVB_CFG_MAC_CONF_IDX]        = &cvb_drv_config_mac_handler_eth,
            [CVB_CFG_SOMEIP_IDX]          = &cvb_drv_config_someip_handler_eth,
            [CVB_CFG_FWCFG_IDX]           = &cvb_drv_config_fwcfg_handler_eth,
            [CVB_CFG_RTPG_PATCH_1_IDX]    = &cvb_drv_config_rtpg_handler_eth,
            [CVB_CFG_RTPG_PATCH_2_IDX]    = &cvb_drv_config_rtpg_handler_eth,
            [CVB_CFG_STACKING_IDX]        = &cvb_drv_config_extphy_patch_handler_eth,
            [CVB_CFG_PCIE_IDX]            = NULL,
            [CVB_CFG_FWCFG_BYTE_IDX]      = &cvb_drv_config_fwcfg_byte_handler_eth,
            [CVB_CFG_REG_FIELD_WR_IDX]    = &cvb_drv_config_reg_field_write_handler_eth,
            [CVB_CFG_MKA_IDX]             = &cvb_drv_config_mka_handler_eth,
            [CVB_CFG_RT_GLB_CFG]          = &cvb_drv_config_rot_sw_handler_eth,
            [CVB_CFG_ROT_SW_INTF_TABLE]   = NULL,
            [CVB_CFG_MACPHY_0_CONFIG_IDX] = &cvb_drv_config_macphy_handler_eth,
            [CVB_CFG_MACPHY_1_CONFIG_IDX] = &cvb_drv_config_macphy_handler_eth,
            [CVB_CFG_PTP_IDX]             = &cvb_drv_config_ptp_handler_eth,
            [CVB_CFG_QCC_PRE_TBL_IDX]     = NULL,
            [CVB_CFG_5KR_PATCH_IDX]       = &cvb_drv_config_5gkr_patch_handler_eth};

    for(i = CVB_CFG_TBL_BASE_IDX; i <= CVB_CFG_TBL_END_IDX; i++)
    {
        configHandlersEth[i] = &cvb_drv_config_table_handler_eth;
    }
    readAddr = startAddr;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(&configEth, 0, sizeof(configEth)));
        paraLen = (imgParaLen & (uint32)0x0FFFFFu);
        flags   = (imgParaLen & (uint32)0xF00000u);
        if(NULL != configHandlersEth[index])
        {
            while((RT_ERR_OK == retVal) && ((uint32)0u < paraLen))
            {
                if(((uint32)CVB_MAXIMUM_BLOCK_NUMBER < blockIdx) || (CVB_FLASH_ZONE_SIZE_8MB < readAddr))
                {
                    retVal = -RT_ERR_FAILED;
                    continue;
                }
                dataLen = ((paraLen > (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH) ? (uint32)CVB_CONFIG_BLOCK_MAX_LENGTH : paraLen);

                /* the readLen should include padding len if exist */
                readLen = ((dataLen + (uint32)CVB_CONFIG_BLOCK_ALIGNMENT_LENGTH - (uint32)1u) & (uint32)0xFFF0u);
                if((int32)0 == rtl_file_read(fp, dataBuf, readAddr, dataLen))
                {
                    retVal = -RT_ERR_FAILED;
                    continue;
                }
                configEth.blockIdx   = blockIdx;
                configEth.index      = index;
                configEth.length     = flags | dataLen;
                configEth.configPort = configPort;

                /* pass flags to domain handler by using parameter length[23:20] */
                retVal = configHandlersEth[index](configEth, dataBuf);
                paraLen -= dataLen;

                readAddr += (readLen + (uint32)CVB_CONFIG_BLOCK_HASH_LENGTH);
                blockIdx++;
            }
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Parse the binary to register
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName for read file
 * @param[in] updateConfig include index imgStartAddr imgParaLen phyPortEnIdx configPort
 * @param[in] pFile for write file
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 */
DYNAMIC_SCOPE RtkApiRet cvb_drv_config_cfg_patch(uint32 unit, const int8 *fileName, cvb_UpdateConfig_t updateConfig, uint8 mode)
{
    uint32    startAddr  = (uint32)0u;
    uint32    paraLen    = (uint32)0u;
    uint8     ethernetEn = (uint8)0u;
    void     *fp         = NULL;
    RtkApiRet retVal     = RT_ERR_OK;

    ethernetEn = (mode & (uint8)0x40u) >> CVB_CONFIG_ETH_MODE_BIT;

    do
    {
        if((uint32)CVB_CFG_END_IDX > updateConfig.index)
        {
            startAddr = updateConfig.imgStartAddr;
            /* the spiParaLen contains following: */
            /* [23:20]: domain flags              */
            /* [19:0] : domain length              */
            paraLen = (updateConfig.imgParaLen & (uint32)0x0FFFFFu);
        }
        else
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(NULL == fp)
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }
        if((uint32)0u < paraLen)
        {
            if((uint8)0u == ethernetEn)
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_call_handler_reg(unit, updateConfig.index, updateConfig.imgParaLen, startAddr, fp));
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_call_handler_eth(unit, updateConfig.index, updateConfig.imgParaLen, startAddr, fp, updateConfig.configPort));
            }
        }

    } while(0u == 1u);
    rtl_file_close(&fp);

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] efuStartAddr
 * @param[in] efuParaLen
 * @retval
 */
RtkApiRet cvb_drv_config_parse_efuse_cfg_header(uint32 unit, uint8 efuStartAddr[], uint8 efuParaLen[])
{
    RtkApiRet retVal                  = RT_ERR_OK;
    uint16    hdrLen                  = (uint16)0u;
    uint16    i                       = (uint16)0u;
    uint8     hdrBuf[CVB_EFU_END_IDX] = {(uint8)0u};
    uint32    j                       = (uint32)0u;
    uint8     efuValid                = (uint8)0u;
    uint16    efuParaExist            = (uint16)0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_drv_drv_memset(hdrBuf, 0x0, CVB_EFU_END_IDX));

        CHK_FUN_CONTINUE(retVal, cvb_otp_read(unit, CVB_OTP_CFG_START_ADDR, &efuValid, (uint32)1u));
        CHK_FUN_CONTINUE(retVal, cvb_otp_read(unit, CVB_OTP_CFG_START_ADDR + 1u, (uint8 *)&efuParaExist, (uint32)2u));

        efuValid &= (uint8)0xC0u;

        if((uint8)CVB_EFU_IDENTIFY == efuValid)
        {
            hdrLen = (uint16)0u;
            for(i = (uint16)0u; i < (uint16)CVB_EFU_END_IDX; i++)
            {
                if((uint16)0u != (efuParaExist & ((uint16)0x1 << i)))
                {
                    hdrLen += (uint16)1u;
                }
            }

            CHK_FUN_CONTINUE(retVal, cvb_otp_read(unit, CVB_OTP_CFG_START_ADDR + 3u, hdrBuf, hdrLen));

            /* initialize start address and data length */
            j = (uint32)0u;
            for(i = (uint16)0u; i < (uint16)CVB_EFU_END_IDX; i++)
            {
                if((uint16)0u == i)
                {
                    efuStartAddr[i] = (uint8)(CVB_OTP_CFG_START_ADDR + 3u + hdrLen);
                }
                else
                {
                    if((uint16)0u != (efuParaExist & ((uint16)0x1 << (i - 1u))))
                    {
                        efuStartAddr[i] = efuStartAddr[i - 1u] + efuParaLen[i - 1u];
                    }
                    else
                    {
                        efuStartAddr[i] = efuStartAddr[i - 1u];
                    }
                }

                if((uint16)0u != (efuParaExist & ((uint16)0x1 << i)))
                {
                    efuParaLen[i] = hdrBuf[j];
                    j++;
                }
                else
                {
                    efuParaLen[i] = (uint8)0u;
                }
            }

            cvb_extraAreaAddr = efuStartAddr[CVB_EFU_END_IDX - 1u] + efuParaLen[CVB_EFU_END_IDX - 1u];
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief
 *
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] ext_start_addr
 * @param[in] endofOTP
 * @retval
 */
RtkApiRet cvb_drv_config_efuse_cfg_ext_search(uint32 unit, uint8 *ext_start_addr, uint8 *endofOTP)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint8     searchBuf[2] = {(uint8)0u};

    *endofOTP = (uint8)0u;

    while(((uint8)CVB_OTP_EXT_CFG_END > cvb_extraAreaAddr) && ((uint8)0u == *endofOTP))
    {
        CHK_FUN_CONTINUE(retVal, cvb_otp_read(unit, cvb_extraAreaAddr, searchBuf, (uint32)1u));

        if(((uint8)0xFFu == searchBuf[0]) || ((uint8)CVB_EFU_END_IDX <= searchBuf[0]))
        {
            *endofOTP = (uint8)1u;
        }
        else
        {
            if((uint8)CVB_EFU_MAC_CONF_IDX == searchBuf[0])
            {
                if((uint8)CVB_OTP_EXT_CFG_END <= (cvb_extraAreaAddr + (uint8)CVB_CONFIG_MAC_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                cvb_extraAreaAddr += ((uint8)CVB_CONFIG_MAC_ENTRY_LEN + (uint8)1u);
            }
            else if((uint8)CVB_EFU_SGMII_PATCH_IDX == searchBuf[0])
            {
                if((uint8)CVB_OTP_EXT_CFG_END <= (cvb_extraAreaAddr + (uint8)CVB_CONFIG_SGMII_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                cvb_extraAreaAddr += ((uint8)CVB_CONFIG_SGMII_ENTRY_LEN + (uint8)1u);
            }
            else if((uint8)CVB_EFU_FWCFG_BYTE_IDX == searchBuf[0])
            {
                if((uint8)CVB_OTP_EXT_CFG_END <= (cvb_extraAreaAddr + (uint8)CVB_CONFIG_FWCFG_BYTE_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                cvb_extraAreaAddr += ((uint8)CVB_CONFIG_FWCFG_BYTE_LEN + (uint8)1u);
            }
            else
            {
                if((uint8)CVB_OTP_EXT_CFG_END <= (cvb_extraAreaAddr + (uint8)CVB_CONFIG_OCP_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                    ;
                }
                cvb_extraAreaAddr += (CVB_CONFIG_OCP_ENTRY_LEN + (uint8)1u);
            }
        }
        *ext_start_addr = cvb_extraAreaAddr;
    }

    return retVal;
}

RtkApiRet cvb_drv_config_eth_write_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 value)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regVal;

    do
    {
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        regVal = regValRx[0];
        /* Register field set */
        CHK_FUN_CONTINUE(retVal, cvb_reg_field_set(regAddr, lsp, len, value, &regVal));
        /* Set write command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
        /* Send write command */
        CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief
 *
 * @param[in] regCmd
 * @param[in] regaddr
 * @param[in] regval
 * @param[in] lastPacket
 * @retval
 */
RtkApiRet cvb_drv_config_eth_packet_assmeble(uint8 regCmd, uint32 regAddr, uint32 regVal, uint8 lastPacket)
{
    RtkApiRet             retVal                                                       = RT_ERR_OK;
    static uint8          regAddrEth[RTK_ETHERACCESS_MAX_BURST - 1u][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    static uint32         regvalEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint32)0u};
    static uint8          regCmdEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint8)0u};
    static uint32         regCnt                                                       = (uint32)0u;
    uint32                rxPktLen                                                     = (uint32)0u;
    uint8                 rxPkt[CVB_MAX_PKT_LEN]                                       = {(uint8)0u};
    EtherAccessMac_t      mac;
    EtherAccessL2Header_t txPkt;

    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        if(1u == lastPacket)
        {
            if(0u != regCnt)
            {
                /* Set dummy reg in last */
                regCmdEth[(regCnt)]     = (uint8)0u;
                regAddrEth[(regCnt)][0] = (uint8)0u;
                regAddrEth[(regCnt)][1] = (uint8)0u;
                regAddrEth[(regCnt)][2] = (uint8)0u;
                regvalEth[(regCnt)]     = (uint32)0u;

                CHK_FUN_RET(retVal, cvb_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regvalEth, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVB_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)CVB_MAX_PKT_LEN;
                CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

                regCnt = (uint32)0u;
            }
        }
        else
        {
            regCmdEth[(regCnt)]   = regCmd;
            regAddrEth[regCnt][0] = (uint8)((regAddr >> 16u) & 0xFFu);
            regAddrEth[regCnt][1] = (uint8)((regAddr >> 8u) & 0xFFu);
            regAddrEth[regCnt][2] = (uint8)((regAddr)&0xFFu);
            regvalEth[regCnt]     = regVal;

            regCnt++;

            if(((uint32)RTK_ETHERACCESS_MAX_BURST - 2u) <= regCnt)
            {
                /* Set dummy reg in last */
                regCmdEth[(regCnt)]     = (uint8)0;
                regAddrEth[(regCnt)][0] = (uint8)0;
                regAddrEth[(regCnt)][1] = (uint8)0;
                regAddrEth[(regCnt)][2] = (uint8)0;
                regvalEth[(regCnt)]     = (uint32)0;

                CHK_FUN_RET(retVal, cvb_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regvalEth, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVB_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)CVB_MAX_PKT_LEN;
                CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));

                regCnt = (uint32)0u;
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief
 *
 * @param[in] regCmd
 * @param[in] regaddr
 * @param[in] regval
 * @param[in] lastPacket
 * @retval
 */
RtkApiRet cvb_drv_config_eth_tx_packet_assmeble_rx_decode(uint8 regCmd, uint32 regAddr, uint32 *regVal, uint8 lastPacket)
{
    RtkApiRet             retVal                                                       = RT_ERR_OK;
    static uint8          regAddrEth[RTK_ETHERACCESS_MAX_BURST - 1u][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    static uint8          regCmdEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint8)0u};
    static uint32         regCnt                                                       = (uint32)0u;
    uint32                rxPktLen                                                     = (uint32)0u;
    uint8                 rxPkt[CVB_MAX_PKT_LEN]                                       = {(uint8)0u};
    EtherAccessMac_t      mac;
    EtherAccessL2Header_t txPkt;

    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_dmac_get(mac.dMac));
        if(1u == lastPacket)
        {
            if(0u != regCnt)
            {
                /* Set dummy reg in last */
                regCmdEth[(regCnt)]     = (uint8)0u;
                regAddrEth[(regCnt)][0] = (uint8)0u;
                regAddrEth[(regCnt)][1] = (uint8)0u;
                regAddrEth[(regCnt)][2] = (uint8)0u;
                regVal[(regCnt)]        = (uint32)0u;

                CHK_FUN_RET(retVal, cvb_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regVal, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, CVB_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)CVB_MAX_PKT_LEN;
                CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
                CHK_FUN_RET(retVal, cvb_eth_acc_check_burst_rw(regCnt, regVal, rxPkt));

                regCnt = (uint32)0u;
            }
        }
        else
        {
            regCmdEth[(regCnt)]   = regCmd;
            regAddrEth[regCnt][0] = (uint8)((regAddr >> 16u) & 0xFFu);
            regAddrEth[regCnt][1] = (uint8)((regAddr >> 8u) & 0xFFu);
            regAddrEth[regCnt][2] = (uint8)((regAddr)&0xFFu);

            regCnt++;

            if((RTK_ETHERACCESS_MAX_BURST - 2u) <= regCnt)
            {
                /* Set dummy reg in last */
                regCmdEth[(regCnt)]     = (uint8)0u;
                regAddrEth[(regCnt)][0] = (uint8)0u;
                regAddrEth[(regCnt)][1] = (uint8)0u;
                regAddrEth[(regCnt)][2] = (uint8)0u;
                regVal[(regCnt)]        = (uint32)0u;

                CHK_FUN_RET(retVal, cvb_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regVal, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, (uint32)CVB_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)CVB_MAX_PKT_LEN;
                CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
                CHK_FUN_RET(retVal, cvb_eth_acc_check_burst_rw(regCnt, regVal, rxPkt));

                regCnt = (uint32)0u;
            }
        }
    } while(0u == 1u);

    return retVal;
}
RtkApiRet cvb_drv_config_polling_status(uint32 unit, cvb_PollingReg_t pollingReg)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    timeOutCnt                               = 0u;
    uint32    regVal                                   = 0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    mask                                     = 0u;
    uint32    i                                        = 0u;
    uint8     breakFlag                                = 0u;
#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("cvb_drv_config_polling_status 0x%x lsp %d len %d num 0x%x\n", pollingReg.address, pollingReg.lsp, pollingReg.len, pollingReg.num);
    for(i = 0u; i < pollingReg.num; i++)
    {
        rtlglue_printf("value[%d] 0x%x\n", i, pollingReg.value[i]);
    }
#endif
    do
    {
        if(((uint32)31u < pollingReg.lsp) || ((uint32)0u == pollingReg.len) || (((uint32)31u) < ((pollingReg.lsp + pollingReg.len) - (uint32)1u)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(32u == pollingReg.len)
        {
            mask = 0xFFFFFFFFu;
        }
        else
        {
            mask = (((uint32)1u << pollingReg.len) - 1u);
        }

        while((0u == breakFlag) && (RT_ERR_OK == retVal))
        {
            if((pollingReg.timeoutValue <= timeOutCnt))
            {
                breakFlag = 1u;
                retVal    = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
            timeOutCnt++;
            if(1u == pollingReg.isEth)
            {
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, pollingReg.address, regValRx, 0u));
                /* Send read command */
                CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, pollingReg.address, regValRx, 1u));
                regVal = regValRx[0];
            }
            else
            {
                printf("1111\n");
                CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, pollingReg.address, &regVal));
            }
            printf("22222\n");
            (void)rtk_usecond_delay(10u);

            for(i = 0u; i < pollingReg.num; i++)
            {
                if(((regVal >> pollingReg.lsp) & mask) == pollingReg.value[i])
                {
                    breakFlag = 1u;
                }
            }
        }
    } while(0u == 1u);
#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("cvb_drv_config_polling_status regVal 0x%x ret %d\n", regVal, retVal);
#endif
    return retVal;
}
