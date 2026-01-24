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
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_regField_list.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_table_struct.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#include "rtl907xd_v1_asicdrv_flash.h"
#include "rtl907xd_v1_asicdrv_otp.h"
#include "rtl907xd_v1_asicdrv_phy.h"
#include "rtl907xd_v1_api.h"
#include "rtl907xd_v1_asicdrv_config.h"
#include "rtl907xd_v1_asicdrv_serdes.h"
#include "rtl907xd_v1_asicdrv_ptp.h"
#include "rtkas_system_porting.h"
#include "rtl907xd_v1_asicdrv_ethernet_access.h"
#include "rtl907xd_v1_api_serdes.h"

#define DV1_WAITREADY (0)

/******************************************************************************
 * Global Variables
 ******************************************************************************/
static uint8        dv1_extraAreaAddr                           = 0xFFu;
static const uint32 dv1_g_phyIdxToOcpIdx[DV1_NUM_OF_PORT_T1PHY] = {0x010000u, 0x020000u, 0x030000u, 0x040000u, 0x050000u, 0x060000u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu};
uint32              dv1_g_wrapSDSNotPowerUp                     = DV1_WRAP_SDS_PORT_BITMASK;
/******************************************************************************
 * Function Declaration
 ******************************************************************************/
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_25ua_opfsm_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_brpatch_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fe_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mac_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rtpg_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fwcfg_byte_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_reg_field_write_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mka_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_intf_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_ptp_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table0_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table1_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table2_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane0(uint32 regAddr, uint32 regVal);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane1(uint32 regAddr, uint32 regVal);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_25ua_opfsm_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_brpatch_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_polling_timeout(uint32 regAddr, uint32 us);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_get_pcs(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_set_pcs(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_get_aphy(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 *value);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_set_aphy(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fe_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_special_reg_eth(uint32 regAddr, uint32 *regVal, uint32 configPort);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mac_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rtpg_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fwcfg_byte_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_reg_field_write_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mka_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_handler_check_eth(dv1_ConfigEth_t configEth);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_intf_handler_check_eth(dv1_ConfigEth_t configEth);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_intf_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_ptp_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table0_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table1_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table2_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane0_patch_eth(uint32 regAddr, uint32 regVal);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane1_patch_eth(uint32 regAddr, uint32 regVal);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane0_eth(uint32 regAddr, uint32 regVal);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane1_eth(uint32 regAddr, uint32 regVal);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_method_eth(void);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_method_reg(void);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_call_handler_reg(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_call_handler_eth(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp, uint32 configPort);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_memcpy_reg(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_memcpy_eth(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_error_reg(uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_error_eth(uint32 macPhyAddr);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_cfg_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_br(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_rtpg(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_eth_br(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_eth_rtpg(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fw_cfg_byte_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fw_cfg_byte_patch_eth(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_extphy_patch_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer);
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_extphy_patch_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer);

/******************************************************************************
 * Functions Definition
 ******************************************************************************/
RtkApiRet dv1_drv_config_update_Aes_Ctr_Iv(uint32 unit, void *fp, const uint32 fwKeyType, const uint32 fwLength)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint8     fileBuf[4] = {(uint8)0u};
    uint32    fwBuf      = (uint32)0u;
    uint32    i          = (uint32)0u;
    uint32    j          = (uint32)0u;
    do
    {

        if(0u == fwKeyType)
        {
            for(i = fwLength; i < (fwLength + 16u); i += 4u)
            {

                if((int32)0 == rtl_file_read(fp, fileBuf, 0x100u + j, 4u))
                {
                    retVal = -RT_ERR_FAILED;
                    continue;
                }
                fwBuf = (uint32)fileBuf[0] | ((uint32)fileBuf[1] << 8) | ((uint32)fileBuf[2] << 16) | ((uint32)fileBuf[3] << 24);
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_reg_write(unit, i, fwBuf));
                j += 4u;
            }
        }
    } while(0u == 1u);

    return retVal;
}
RtkApiRet dv1_drv_config_eth_update_aes_ctr_iv(void *fp, const uint32 fwKeyType, const uint32 fwLength)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint8     fwBuf[16] = {(uint8)0u};
    do
    {
        if(0u == fwKeyType)
        {
            if((int32)0 == rtl_file_read(fp, fwBuf, 0x100u, 16u))
            {
                retVal = -RT_ERR_FAILED;
                continue;
            }
            CHK_FUN_CONTINUE(retVal, dv1_eth_acc_flow_patch_write(fwLength, 16, fwBuf));
        }
    } while(0u == 1u);
    return retVal;
}

RtkApiRet dv1_drv_config_get_hdr_fw_info(void *fp, uint32 *fwAddr, uint32 *fwLength, uint8 *fwKeyType)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     fileBuf[9]  = {(uint8)0u};
    uint16    headerIdent = (uint16)0u;
    do
    {
        /* check the validation of this binary file */
        if((int32)0 == rtl_file_read(fp, fileBuf, DV1_FLASH_HEADER_OFFSET, 9u))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        headerIdent = (uint16)fileBuf[0] | ((uint16)fileBuf[1] << 8);

        if((uint16)DV1_CFG_SPI_IDENTIFY != headerIdent)
        {
#ifdef DEBUG_SYS

            (void)rtlglue_printf("Firmware doesn't exist\n");
#endif
            retVal = -RT_ERR_INPUT;
            continue;
        }

        *fwLength  = ((uint32)fileBuf[7] << 4u) + ((uint32)fileBuf[8] << 12u);
        *fwAddr    = (uint32)fileBuf[4] | ((uint32)fileBuf[5] << 8u) | ((uint32)fileBuf[6] << 16u);
        *fwKeyType = (fileBuf[2] & (uint8)0xCu) >> 2u;

        if(((uint32)0xFFFF0u == *fwLength) || ((uint32)DV1_MAX_FW_LENGTH < *fwLength))
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

RtkApiRet dv1_drv_config_sys_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if(((uint32)DV1_CFG_BR_PATCH_1_IDX == updateConfig.index) || ((uint32)DV1_CFG_BR_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_phy_patch_br(unit, fileName, updateConfig));
        }
        else if(((uint32)DV1_CFG_RTPG_PATCH_1_IDX == updateConfig.index) || ((uint32)DV1_CFG_RTPG_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_phy_patch_rtpg(unit, fileName, updateConfig));
        }
        else if(((uint32)DV1_CFG_FWCFG_BYTE_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_fw_cfg_byte_patch(unit, fileName, updateConfig));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, 0));
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dv1_drv_config_eth_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if(((uint32)DV1_CFG_BR_PATCH_1_IDX == updateConfig.index) || ((uint32)DV1_CFG_BR_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_phy_patch_eth_br(unit, fileName, updateConfig, mode));
        }
        else if(((uint32)DV1_CFG_RTPG_PATCH_1_IDX == updateConfig.index) || ((uint32)DV1_CFG_RTPG_PATCH_2_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_phy_patch_eth_rtpg(unit, fileName, updateConfig, mode));
        }
        else if(((uint32)DV1_CFG_FWCFG_BYTE_IDX == updateConfig.index))
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_fw_cfg_byte_patch_eth(unit, fileName, updateConfig, mode));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, mode));
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_eth_br(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode)
{

    RtkApiRet    retVal                                    = RT_ERR_OK;
    uint32       i                                         = (uint32)0u;
    const uint32 newPortTable[10]                          = {(uint32)1u, (uint32)2u, (uint32)3u, (uint32)4u, (uint32)5u, (uint32)6u, (uint32)9u, (uint32)10u, (uint32)7u, (uint32)8u};
    uint8        userPort2T1phy[DV1_RTL9075_INTERFACE_NUM] = {0xFFu, 0u, 1u, 2u, 3u, 4u, 5u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        for(i = (uint32)DV1_BR_PORT_START; i <= (uint32)DV1_BR_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_request(unit, newPortTable[i], mode, 0u));
            }
        }

        /* Last packet for patch request */
        CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_request(unit, 0u, mode, 1u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, mode));

        for(i = (uint32)DV1_BR_PORT_START; i <= (uint32)DV1_BR_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_release(unit, newPortTable[i], mode, 0));
            }
        }

        /* Last packet for patch release */
        CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_release(unit, 0u, mode, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_eth_rtpg(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode)
{

    RtkApiRet    retVal                                    = RT_ERR_OK;
    uint32       i                                         = (uint32)0u;
    const uint32 newPortTable[10]                          = {(uint32)1u, (uint32)2u, (uint32)3u, (uint32)4u, (uint32)5u, (uint32)6u, (uint32)9u, (uint32)10u, (uint32)7u, (uint32)8u};
    uint8        userPort2T1phy[DV1_RTL9075_INTERFACE_NUM] = {0xFFu, 0u, 1u, 2u, 3u, 4u, 5u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        for(i = (uint32)DV1_RTPG_PORT_START; i <= (uint32)DV1_RTPG_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_request(unit, newPortTable[i], mode, 0u));
            }
        }

        /* Last packet for patch request */
        CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_request(unit, 0u, mode, 1u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, mode));

        for(i = (uint32)DV1_RTPG_PORT_START; i <= (uint32)DV1_RTPG_PORT_END; i++)
        {
            if(((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1 << i))) && (i != userPort2T1phy[updateConfig.configPort]))
            {
                CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_release(unit, newPortTable[i], mode, 0));
            }
        }

        /* Last packet for patch release */
        CHK_FUN_CONTINUE(retVal, dv1_drv_phy_patch_release(unit, 0u, mode, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_br(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig)
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
        for(i = (uint32)DV1_BR_PORT_START; i <= (uint32)DV1_BR_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_phy_patch_request(unit, newPortTable[i], 0u, 0u));
            }
        }

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, 0));

        for(i = (uint32)DV1_BR_PORT_START; i <= (uint32)DV1_BR_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_phy_patch_release(unit, newPortTable[i], 0u, 0u));
            }
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_phy_patch_rtpg(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig)
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
        for(i = (uint32)DV1_RTPG_PORT_START; i <= (uint32)DV1_RTPG_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_phy_patch_request(unit, newPortTable[i], 0u, 0u));
            }
        }

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, 0));

        for(i = (uint32)DV1_RTPG_PORT_START; i <= (uint32)DV1_RTPG_PORT_END; i++)
        {
            if((uint32)0u != (updateConfig.phyPortEnIdx & ((uint32)0x1u << i)))
            {
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_phy_patch_release(unit, newPortTable[i], 0u, 0u));
            }
        }
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fw_cfg_byte_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig)
{

    RtkApiRet retVal         = RT_ERR_OK;
    uint32    regAddr        = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    brPhyEnMskTmp  = (uint32)0u;
    uint32    brPhyEnMsk     = (uint32)0u;

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        regAddr = (uint32)DV1_BR_PHY_EN_MSK_ADDR;
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));

        /* record original g_BRPHYEnMsk */
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, regAddr, &brPhyEnMskTmp));
        /*BR_PHY_EN_MSK [15:0]*/
        brPhyEnMsk = brPhyEnMskTmp & 0x0000FFFFu;
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, 0));

        /* Finetune g_BRPHYEnMsk after loading fw_cfg */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, regAddr, &brPhyEnMskTmp));

        brPhyEnMsk = (brPhyEnMsk & (brPhyEnMskTmp & 0x0000FFFFu)) | (brPhyEnMskTmp & 0xFFFF0000u);

        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, regAddr, brPhyEnMsk));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fw_cfg_byte_patch_eth(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode)
{

    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regAddr;
    uint32    brPhyEnMsk = (uint32)0u;

    do
    {
        if(0u == updateConfig.imgParaLen)
        {
            continue;
        }
        regAddr = (uint32)DV1_BR_PHY_EN_MSK_ADDR;
        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
        /* record original g_BRPHYEnMsk */
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        /*BR_PHY_EN_MSK [15:0]*/
        brPhyEnMsk = regValRx[0] & 0x0000FFFFu;
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_cfg_patch(unit, fileName, updateConfig, mode));

        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
        /* Finetune g_BRPHYEnMsk after loading fw_cfg */
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        brPhyEnMsk = (brPhyEnMsk & (regValRx[0] & 0x0000FFFFu)) | (regValRx[0] & 0xFFFF0000u);
        /* Set write command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, brPhyEnMsk, 0u));
        /* Send write command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* select page to 0x4A */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SW));
    } while(0u == 1u);

    return retVal;
}
RtkApiRet dv1_drv_config_check_eth_enable_phy(uint32 *phyPortEnIdx)
{
    RtkApiRet             retVal                                                   = RT_ERR_OK;
    uint32                i                                                        = (uint32)0u;
    uint8                 regAddr[RTK_ETHERACCESS_MAX_BURST - 1][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    uint32                regValue[RTK_ETHERACCESS_MAX_BURST - 1]                  = {(uint32)0u};
    uint8                 rxPkt[DV1_MAX_PKT_LEN], regCmd[RTK_ETHERACCESS_MAX_BURST - 1] = {(uint8)0u};
    uint32                regNum      = (uint32)0u;
    uint32                rxPktLen    = (uint32)0u;
    const uint16          opTable[10] = {(uint16)0x1624, (uint16)0x1628, (uint16)0x162c, (uint16)0x1630, (uint16)0x1634, (uint16)0x169c, (uint16)0x16A0, (uint16)0x16A4, (uint16)0x16A8, (uint16)0x16AC};
    EtherAccessMac_t      mac;
    EtherAccessL2Header_t txPkt;

    do
    {
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_smac_get(mac.sMac));
        /* Read enabled port */
        for(i = (uint32)0; i < (uint32)10u; i++)
        {
            regCmd[i]     = (uint8)DV1_ETHERACCESS_REGISTER_RW_MAC_READ;
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
        CHK_FUN_CONTINUE(retVal, dv1_eth_acc_gen_burst_rw(mac, regNum, regCmd, &regAddr[0], regValue, &txPkt));
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DV1_RTK_ETHERACCESS_HEADER_LEN + (regNum * 8u) + 8u));
        rxPktLen = DV1_MAX_PKT_LEN;
        CHK_FUN_CONTINUE(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
        CHK_FUN_CONTINUE(retVal, dv1_eth_acc_check_burst_rw(regNum, regValue, rxPkt));

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

RtkApiRet dv1_drv_config_check_enable_phy(uint32 *phyPortEnIdx)
{
    RtkApiRet    retVal      = RT_ERR_OK;
    uint32       tmpdata     = (uint32)0u;
    uint32       i           = (uint32)0u;
    const uint16 opTable[10] = {(uint16)0x1624, (uint16)0x1628, (uint16)0x162c, (uint16)0x1630, (uint16)0x1634, (uint16)0x169c, (uint16)0x16A0, (uint16)0x16A4, (uint16)0x16A8, (uint16)0x16AC};

    for(i = (uint32)0u; i < (uint32)10u; i++)
    {
        CHK_FUN_BREAK(retVal, dv1_reg_read(DV1_UNIT, opTable[i], &tmpdata));

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

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_25ua_opfsm_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while((uint32)DV1_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)DV1_PAGE_SWR_OCP | (regAddr << 1));
        CHK_FUN_BREAK(retVal, dv1_reg_field_write(DV1_UNIT, regAddr, 0u, 16u, regVal));

        offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
    }
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_brpatch_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
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
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_check_enable_phy(&phyPortEnIdx));
        while((uint32)DV1_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)DV1_BR_PORT_START; i <= (uint32)DV1_BR_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if((uint32)0u != (phyPortEnIdx & ((uint32)1u << i)))
                {
                    regAddr = (uint32)DV1_PAGE_SWR_OCP | ((dv1_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, dv1_reg_field_write(DV1_UNIT, regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    portMask = 0u;
    uint32    offset   = 0u;
    uint8     port     = 0u;
    uint32    page     = 0u;
    uint32    reg      = 0u;
    uint32    value    = 0u;

    while(((uint32)DV1_CONFIG_SGMII_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
    {
        /* parsing configuration entry */
        portMask = (((uint32)buffer[offset] << 8) | (uint32)buffer[offset + 1u]) & (uint32)DV1_SGMII_DOMAIN4_SDS_PORT_BITMASK;

        if(DV1_IS_NOT_SDS != (portMask & DV1_WRAP_SDS_PORT_BITMASK & dv1_g_wrapSDSNotPowerUp))
        {
            for(port = (uint8)4u; port < (uint8)9u; port++)
            {
                if(DV1_IS_WRAP_SDS == ((portMask >> port) & 0x1u))
                {
                    CHK_FUN_CONTINUE(retVal, dv1_drv_sds_wrap_serdes_init(DV1_UNIT, port));
                    dv1_g_wrapSDSNotPowerUp &= ~((uint32)0x1u << port);
                }
            }
        }

        value = (((uint32)buffer[offset + 5u] << 8) | (uint32)buffer[offset + 4u]);

        if((uint8)0u != ((buffer[offset] >> 7u) & (uint8)1u))
        {
            /* pcs */
            page = ((uint32)buffer[offset + 2u]);
            reg  = ((uint32)buffer[offset + 3u]);
            for(port = (uint8)0u; port < (uint8)15u; port++)
            {
                if(DV1_IS_SDS_PORT == ((portMask >> port) & 0x1u))
                {
                    CHK_FUN_BREAK(retVal, dv1_sys_sds_reg_setbits_pcs(DV1_UNIT, dv1_g_sysPort2userPort[port], page, reg, 0u, 15u, value));
                }
            }
        }
        else
        {
            /* D-PHY */
            page = (uint32)0u;
            reg  = (((uint32)buffer[offset + 3u] << 8) | (uint32)buffer[offset + 2u]);
            for(port = (uint8)0u; port < (uint8)15u; port++)
            {
                if(DV1_IS_SDS_PORT == ((portMask >> port) & 0x1u))
                {
                    CHK_FUN_BREAK(retVal, dv1_sys_sds_reg_setbits_aphy(DV1_UNIT, dv1_g_sysPort2userPort[port], page, reg, 0u, 15u, value));
                }
            }
        }
        offset += (uint32)DV1_CONFIG_SGMII_ENTRY_LEN;
    }
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fe_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while((uint32)DV1_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)DV1_PAGE_SWR_OCP | ((DV1_FE_OCP_OFST_ADDR | regAddr) << 1));
        CHK_FUN_BREAK(retVal, dv1_reg_field_write(DV1_UNIT, regAddr, 0u, 16u, regVal));
        offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mac_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    offset           = (uint32)0u;
    bool      waitRegVal       = false;
    uint32    regAddr          = (uint32)0u;
    uint32    regVal           = (uint32)0u;
    uint32    regAddrOtherPage = (uint32)0u;

    do
    {
        while(((uint32)DV1_CONFIG_MAC_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
        {
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8) | ((uint32)buffer[offset + 2u] << 16) | (uint32)DV1_PAGE_SWR_OCP;
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
                    CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, ((regAddrOtherPage & 0xFF000000u) >> 24u)));
                    CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, (regAddrOtherPage & 0x00FFFFFFu), regVal));
                    CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, 0x4A));
                }

                waitRegVal = false;
            }
            else if(0x0u != regAddr)
            {
                CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, regAddr, regVal));
            }
            else
            {
                /*do nothing*/
            }

            offset += (uint32)DV1_CONFIG_MAC_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rtpg_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
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
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_check_enable_phy(&phyPortEnIdx));
        while((uint32)DV1_CONFIG_OCP_ENTRY_LEN <= (configReg.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)DV1_RTPG_PORT_START; i <= (uint32)DV1_RTPG_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if((uint32)0u != (phyPortEnIdx & ((uint32)1u << i)))
                {
                    regAddr = (uint32)DV1_PAGE_SWR_OCP | ((dv1_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, dv1_reg_field_write(DV1_UNIT, regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dv1_drv_extphy_update_code_length_reg(uint32 phyAddr, uint32 idx)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    tempLen = 0u;

    do
    {
        tempLen = phyAddr - (uint32)DV1_EXT_PHY_PATCH_CODE_ADDR + idx;
        if((uint32)DV1_EXT_PHY_PATCH_LENGTH >= tempLen)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, (uint32)DV1_EXT_PHY_PATCH_LEN_ADDR, tempLen));
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dv1_drv_extphy_code_offset_correction_handler_reg(uint32 entry, uint32 *phyAddr, const uint8 *buf, uint32 *shft)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    temp   = 0u;
    uint32    i      = 0u;

    do
    {
        if(0u != entry)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, *phyAddr, &temp));
            for(i = entry; i < 4u; i++)
            {
                temp |= (uint32)buf[i - entry] << (8u * i);
                (*shft) += 1u;
            }
            CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, *phyAddr, temp));
            *phyAddr += 4u;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_extphy_patch_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
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
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)0u));

        offset = configReg.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;
        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configReg.length) || ((uint32)DV1_EXT_PHY_PATCH_LENGTH < (offset + configReg.length)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if(0u == configReg.blockIdx)
        {
            extPhyPatchAddr = (uint32)DV1_EXT_PHY_PATCH_CODE_ADDR;
            entryIdx        = 0u;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_code_offset_correction_handler_reg(entryIdx, &extPhyPatchAddr, buffer, &shift));
        }

        extPhyConfigLen = configReg.length - (configReg.length % (uint32)DV1_EXT_PHY_PATCH_ENTRY_LEN) - shift;
        for(i = (uint32)0u; i < (extPhyConfigLen / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[shift + 0u + (i * 4u)] | ((uint32)buffer[shift + 1u + (i * 4u)] << 8) | ((uint32)buffer[shift + 2u + (i * 4u)] << 16) | ((uint32)buffer[shift + 3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, extPhyPatchAddr, cfgBuf));
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
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, extPhyPatchAddr, cfgBuf));

        CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_update_code_length_reg(extPhyPatchAddr, entryIdx));

        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fwcfg_byte_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    regAddr        = (uint32)0u;
    uint32    regVal         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));
        while(((uint32)DV1_CONFIG_MAC_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
        {
            /* parsing configuration entry */
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8) | ((uint32)buffer[offset + 2u] << 16);
            regVal  = (uint32)buffer[offset + 3u] | ((uint32)buffer[offset + 4u] << 8) | ((uint32)buffer[offset + 5u] << 16) | ((uint32)buffer[offset + 6u] << 24);
            if((DV1_ADDR_BOOTCFG_BASE > (regAddr | ((uint32)DV1_PAGE_SYS_MEM << 24))) || ((DV1_ADDR_BOOTCFG_BASE + DV1_ADDR_BOOTCFG_SIZE) < (regAddr | ((uint32)DV1_PAGE_SYS_MEM << 24))))
            {
                offset += (uint32)DV1_CONFIG_MAC_ENTRY_LEN;
                continue;
            }
            CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, regAddr, regVal));

            offset += (uint32)DV1_CONFIG_MAC_ENTRY_LEN;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_reg_field_write_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regAddr  = (uint32)0u;
    uint32    startBit = (uint32)0u;
    uint32    endBit   = (uint32)0u;
    uint32    regVal   = (uint32)0u;
    uint32    offset   = (uint32)0u;

    /* register field write */
    while(((uint32)DV1_CFG_REG_FIELD_WR_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
    {
        /* Load parameter */
        regAddr  = (uint32)DV1_PAGE_SWR_OCP | buffer[offset + 0u] | ((uint32)buffer[offset + 1u] << 8u) | ((uint32)buffer[offset + 2u] << 16u);
        startBit = buffer[offset + 3u];
        endBit   = buffer[offset + 4u];
        regVal   = buffer[offset + 5u] | ((uint32)buffer[offset + 6u] << 8u) | ((uint32)buffer[offset + 7u] << 16u) | ((uint32)buffer[offset + 8u] << 24u);
        if(endBit <= startBit)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, regAddr, startBit, (endBit - startBit), regVal));
        offset += (uint32)DV1_CFG_REG_FIELD_WR_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mka_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));

        /*length must equal to DV1_MKA_CONFIG_SIZE*/
        if((uint32)DV1_MKA_CONFIG_SIZE != configReg.length)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        for(i = (uint32)0u; i < (configReg.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, ((i * 4u) + (uint32)DV1_MKA_CONFIG_ADDR), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;
    uint32    lengthLeft     = (uint32)0u;
    uint32    tmpAddr        = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));

        offset = configReg.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_ROUTING_TBL_SIZE < (offset + configReg.length))
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

        lengthLeft = configReg.length - 2u;
        tmpAddr    = (uint32)DV1_ROUTING_TBL_ADDR + offset - 2u;
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, tmpAddr, &cfgBuf));
        cfgBuf = (cfgBuf & 0xFFFFu) | ((uint32)buffer[0] << 16) | ((uint32)buffer[1] << 24);
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, tmpAddr, cfgBuf));
        for(i = (uint32)0u; i < (lengthLeft / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8) | ((uint32)buffer[4u + (i * 4u)] << 16) | ((uint32)buffer[5u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, ((i * 4u) + (uint32)DV1_ROUTING_TBL_ADDR + offset + 2u), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        tmpAddr = ((i * 4u) + (uint32)DV1_ROUTING_TBL_ADDR + offset + 2u);
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, tmpAddr, &cfgBuf));
        cfgBuf = (cfgBuf & 0xFFFF0000u) | (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8);
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, tmpAddr, cfgBuf));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_intf_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;
    uint32    lengthLeft     = (uint32)0u;
    uint32    tmpAddr        = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));

        offset = configReg.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_ROUTING_INTF_TBL_SIZE < (offset + configReg.length))
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
        lengthLeft = configReg.length - 2u;
        tmpAddr    = (uint32)DV1_ROUTING_INTF_TBL_ADDR + offset - 2u;
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, tmpAddr, &cfgBuf));
        cfgBuf = (cfgBuf & 0xFFFFu) | ((uint32)buffer[0] << 16) | ((uint32)buffer[1] << 24);
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, tmpAddr, cfgBuf));

        for(i = (uint32)0u; i < (lengthLeft / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8) | ((uint32)buffer[4u + (i * 4u)] << 16) | ((uint32)buffer[5u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, ((i * 4u) + (uint32)DV1_ROUTING_INTF_TBL_ADDR + offset + 2u), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tmpAddr = ((i * 4u) + (uint32)DV1_ROUTING_INTF_TBL_ADDR + offset + 2u);
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, tmpAddr, &cfgBuf));
        cfgBuf = (cfgBuf & 0xFFFF0000u) | (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8);
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, tmpAddr, cfgBuf));

        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_memcpy_reg(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint32)0u;
    uint32    i      = (uint32)0u;
    do
    {
        for(i = (uint32)0u; i < length; i += 4u)
        {
            regVal = (uint32)buffer[i] | ((uint32)buffer[i + 1u] << 8) | ((uint32)buffer[i + 2u] << 16) | ((uint32)buffer[i + 3u] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, (macPhyAddr + DV1_MPY_NIC_CONFIG_HEADER_LEN + offset + i), regVal));
        }
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_error_reg(uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, macPhyAddr, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, macPhyAddr + 4u, 0u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet    retVal          = RT_ERR_OK;
    uint32       currentRegPage  = (uint32)0u;
    uint32       offset          = (uint32)0u;
    uint32       configTotalLen  = (uint32)0u;
    const uint32 macPhyAddrMap[] = {(uint32)DV1_MPY_NIC_BUF_ADDR_0, (uint32)DV1_MPY_NIC_BUF_ADDR_1};
    uint32       macPhyAddr      = macPhyAddrMap[configReg.index - DV1_CFG_MACPHY_0_CONFIG_IDX];

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, DV1_MPY_PAGE));

        offset = configReg.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_MPY_NIC_CONFIG_LIMIT_LEN < (offset + configReg.length))
        {
            /* clean the magic number and length*/
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_macphy_error_reg(macPhyAddr));
            retVal = -RT_ERR_INPUT;
            continue;
        }
        else
        {
            if((uint32)0u == configReg.blockIdx)
            {
                /* set magic number for identifying the config data */
                CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, macPhyAddr, DV1_MPY_NIC_CONFIG_MAGIC_NUM));
            }

            CHK_FUN_CONTINUE(retVal, dv1_drv_config_macphy_memcpy_reg(buffer, configReg.length, offset, macPhyAddr));

            configTotalLen = offset + configReg.length;
            CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, macPhyAddr + 4u, configTotalLen));
        }
        if(RT_ERR_OK != retVal)
        {
            /* clean the magic number and length*/
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_macphy_error_reg(macPhyAddr));
            retVal = -RT_ERR_FAILED;
        }
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_ptp_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    offset         = (uint32)0u;
    uint32    cfgBuf         = (uint32)0u;
    uint32    currentRegPage = (uint32)0u;
    uint32    i              = (uint32)0u;

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));

        offset = configReg.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configReg.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_PTP_STATUS_SIZE < (offset + configReg.length))
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
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, ((i * 4u) + (uint32)DV1_PTP_STATUS_ADDR + offset), cfgBuf));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table0_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal            = RT_ERR_OK;
    uint32    entryNum          = useLen / (tabLen + DV1_CONFIG_TABLE0_HEADER_LEN);
    uint32    tabAddr           = (uint32)0u;
    uint32    counter           = (uint32)0u;
    uint32    i                 = (uint32)0;
    uint32    j                 = (uint32)0u;
    uint32    tabData           = (uint32)0u;
    uint32    tabTimeOut        = (uint32)0u;
    uint32    tempRegVal        = (uint32)0u;
    uint32    controlReg        = (uint32)0u;
    uint32    dataBaseReg       = (uint32)0u;
    uint32    tabBaseAddr       = (uint32)0u;
    uint32    tabAddrMultiMap[] = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                   0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                   0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                   0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    switch(tableType)
    {
        case(uint32)DV1_QBV_GATE_CONTROL_LIST_TABLE:
        case(uint32)DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE:
        case(uint32)DV1_RH_TABLE:
        case(uint32)DV1_OIL_TABLE:
        case(uint32)DV1_EGIF_TABLE:
            controlReg  = DV1_MAC_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_MAC_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_MAC_BASE_ADDR;
            break;
        default:
            controlReg  = DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_ALE_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_ALE_BASE_ADDR;
            break;
    }

    while((counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[i + 3u]) << 24u) | ((uint32)(buffer[(i + 2u)]) << 16u) | ((uint32)(buffer[(i + 1u)]) << 8u) | (uint32)buffer[i];
        tabAddr = tabAddr - (tabBaseAddr + (DV1_TABLE_INDEX_GAP * tableType));
        tabAddr = tabAddr / (tabAddrMultiMap[dv1_table_list[tableType].datareg_num]);
        i += (uint32)DV1_CONFIG_TABLE0_HEADER_LEN;

        for(j = (uint32)0u; j < tabLen; j += (uint32)DV1_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, (dataBaseReg + j), tabData));
            i += (uint32)DV1_CONFIG_TABLE_DATA_LEN;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = (uint32)DV1_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, controlReg, tabData));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, controlReg, &tempRegVal));
            if((uint32)0u == (tempRegVal & (uint32)DV1_CONFIG_TABLE_CTRL_BIT))
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
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table1_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal            = RT_ERR_OK;
    uint32    entryNum          = (useLen - DV1_CONFIG_TABLE0_HEADER_LEN) / tabLen;
    uint32    tabAddr           = (uint32)0u;
    uint32    counter           = (uint32)0u;
    uint32    i                 = (uint32)0;
    uint32    j                 = (uint32)0u;
    uint32    tabData           = (uint32)0u;
    uint32    tabTimeOut        = (uint32)0u;
    uint32    tempRegVal        = (uint32)0u;
    uint32    controlReg        = (uint32)0u;
    uint32    dataBaseReg       = (uint32)0u;
    uint32    tabBaseAddr       = (uint32)0u;
    uint32    tabAddrMultiMap[] = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                   0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                   0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                   0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    switch(tableType)
    {
        case(uint32)DV1_QBV_GATE_CONTROL_LIST_TABLE:
        case(uint32)DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE:
        case(uint32)DV1_RH_TABLE:
        case(uint32)DV1_OIL_TABLE:
        case(uint32)DV1_EGIF_TABLE:
            controlReg  = DV1_MAC_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_MAC_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_MAC_BASE_ADDR;
            break;
        default:
            controlReg  = DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_ALE_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_ALE_BASE_ADDR;
            break;
    }

    tabAddr = tabBaseAddr | ((uint32)(buffer[2u]) << 16u) | ((uint32)(buffer[1u]) << 8u) | (uint32)buffer[0u];
    tabAddr = tabAddr - (tabBaseAddr + (DV1_TABLE_INDEX_GAP * tableType));
    tabAddr = tabAddr / (tabAddrMultiMap[dv1_table_list[tableType].datareg_num]);
    i += (uint32)DV1_CONFIG_TABLE1_HEADER_LEN;

    while((counter < entryNum) && (RT_ERR_OK == retVal) && ((uint32)DV1_TABLE_ENTRY_MAX > tabAddr))
    {

        for(j = (uint32)0u; j < tabLen; j += (uint32)DV1_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, (dataBaseReg + j), tabData));
            i += (uint32)DV1_CONFIG_TABLE_DATA_LEN;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = (uint32)DV1_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, controlReg, tabData));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, controlReg, &tempRegVal));
            if((uint32)0u == (tempRegVal & (uint32)DV1_CONFIG_TABLE_CTRL_BIT))
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
        tabAddr++;
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table2_handler_reg(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    entryNum    = useLen / (tabLen + DV1_CONFIG_TABLE2_HEADER_LEN);
    uint32    tabAddr     = (uint32)0u;
    uint32    counter     = (uint32)0u;
    uint32    i           = (uint32)0;
    uint32    j           = (uint32)0u;
    uint32    tabData     = (uint32)0u;
    uint32    tabTimeOut  = (uint32)0u;
    uint32    tempRegVal  = (uint32)0u;
    uint32    controlReg  = (uint32)0u;
    uint32    dataBaseReg = (uint32)0u;

    switch(tableType)
    {
        case(uint32)DV1_QBV_GATE_CONTROL_LIST_TABLE:
        case(uint32)DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE:
        case(uint32)DV1_RH_TABLE:
        case(uint32)DV1_OIL_TABLE:
        case(uint32)DV1_EGIF_TABLE:
            controlReg  = DV1_MAC_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_MAC_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            break;
        default:
            controlReg  = DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_ALE_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            break;
    }

    while((counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[(i + 1u)]) << 8) | (uint32)buffer[i];
        i += (uint32)DV1_CONFIG_TABLE2_HEADER_LEN;

        for(j = (uint32)0u; j < tabLen; j += (uint32)DV1_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_reg_write(DV1_UNIT, (dataBaseReg + j), tabData));
            i += (uint32)DV1_CONFIG_TABLE_DATA_LEN;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = (uint32)DV1_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, controlReg, tabData));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, controlReg, &tempRegVal));
            if((uint32)0u == (tempRegVal & (uint32)DV1_CONFIG_TABLE_CTRL_BIT))
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
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    tableType = configReg.index - DV1_CFG_TBL_BASE_IDX;
    uint32    tabLen    = dv1_table_list[tableType].datareg_num * 4u;
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
                retVal = dv1_drv_config_table0_handler_reg(tableType, tabLen, buffer, useLen);
                break;
            case(1u):
                retVal = dv1_drv_config_table1_handler_reg(tableType, tabLen, buffer, useLen);
                break;
            case(2u):
                retVal = dv1_drv_config_table2_handler_reg(tableType, tabLen, buffer, useLen);
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_method_reg(void)
{
    RtkApiRet    retVal            = RT_ERR_OK;
    uint32       regVal            = (uint32)0u;
    uint32       currentRegPage    = (uint32)0u;
    uint32       i                 = (uint32)0u;
    uint32       serdesPatchMethod = (uint32)0u;
    const uint32 krConfig[4][10]   = {
        {(uint32)DV1_CFG_NCTL_REG_11,
           (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_EN_P04_LSP, (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_EN_P04_LEN,
           (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_ADDR_P04_LSP, (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_ADDR_P04_LEN,
           (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_TARG_P04_LSP, (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_TARG_P04_LEN,
           (uint32)ENABLE, (uint32)0x00C1u, (uint32)0x080Fu},
        {(uint32)DV1_CFG_NCTL_REG_12,
           (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_EN_P05_LSP, (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_EN_P05_LEN,
           (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_ADDR_P05_LSP, (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_ADDR_P05_LEN,
           (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_TARG_P05_LSP, (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_TARG_P05_LEN,
           (uint32)ENABLE, (uint32)0x00C5u, (uint32)0x0812u},
        {(uint32)DV1_CFG_NCTL_REG_36,
           (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_EN_P04_S1_LSP, (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_EN_P04_S1_LEN,
           (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_ADDR_P04_S1_LSP, (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_ADDR_P04_S1_LEN,
           (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_TARG_P04_S1_LSP, (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_TARG_P04_S1_LEN,
           (uint32)ENABLE, (uint32)0x00C1u, (uint32)0x080Fu},
        {(uint32)DV1_CFG_NCTL_REG_37,
           (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_EN_P05_S1_LSP, (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_EN_P05_S1_LEN,
           (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_ADDR_P05_S1_LSP, (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_ADDR_P05_S1_LEN,
           (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_TARG_P05_S1_LSP, (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_TARG_P05_S1_LEN,
           (uint32)ENABLE, (uint32)0x00C5u, (uint32)0x0812u}};

    do
    {
        /* change the register page to memory space */
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_read(DV1_UNIT, &currentRegPage));
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, (uint32)DV1_PAGE_SYS_MEM));
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, DV1_SERDES_PATCH_METHOD_ADDRESS, &regVal));
        serdesPatchMethod = regVal & (uint32)0xffffu;
        /* change the register page to original page*/
        CHK_FUN_CONTINUE(retVal, dv1_reg_page_write(DV1_UNIT, currentRegPage));
        if((uint32)0u == serdesPatchMethod)
        {
            for(i = (uint32)0u; i < (uint32)4u; i++)
            {
                if(RT_ERR_OK != retVal)
                {
                    break;
                }
                CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, krConfig[i][0], &regVal));
                CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, krConfig[i][1], krConfig[i][2], krConfig[i][7], &regVal));
                CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, krConfig[i][3], krConfig[i][4], krConfig[i][8], &regVal));
                CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, krConfig[i][5], krConfig[i][6], krConfig[i][9], &regVal));
                CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, krConfig[i][0], regVal));
            }
        }
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane0(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    tmpValue = (uint32)0u;

    do
    {
        /* Write RAM Address 12'h080F, 12'h0810, 12'h0811 ... (bit 12 must be 1) */
        /* Patch control and address */
        /* LAN0 */
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, DV1_CFG_NCTL_REG_23, &tmpValue));
        /* rg_cfg_nctl_cmd_s0 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LEN, ENABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s0 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LEN, ENABLE, &tmpValue));
        /*rg_cfg_nctl_addr_s0 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, DV1_CFG_NCTL_REG_23, tmpValue));

        /* (Patch code e.g. 0xC655, 0xC000, 0x20C2 ... ) unitl all patch cod to RAM is  completed */
        /* Patch data */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_24, DV1_CFG_NCTL_REG_24_RG_CFG_NCTL_RAM_WDT_S0_LSP, DV1_CFG_NCTL_REG_24_RG_CFG_NCTL_RAM_WDT_S0_LEN, regVal));

        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, DV1_CFG_NCTL_REG_23, &tmpValue));
        /* Patch done plus */
        /* rg_cfg_nctl_cmd_s0 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LEN, DISABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s0 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LEN, DISABLE, &tmpValue));
        /* rg_cfg_nctl_addr_s0 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, DV1_CFG_NCTL_REG_23, tmpValue));
    } while(0u == 1u);
    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane1(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    tmpValue = (uint32)0u;

    do
    {
        /* LAN1 */
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, DV1_CFG_NCTL_REG_48, &tmpValue));
        /* rg_cfg_nctl_cmd_s1 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LEN, ENABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s1 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LEN, ENABLE, &tmpValue));
        /*rg_cfg_nctl_addr_s1 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, DV1_CFG_NCTL_REG_48, tmpValue));

        /* (Patch code e.g. 0xC655, 0xC000, 0x20C2 ... ) unitl all patch cod to RAM is  completed */
        /*Patch data*/
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_49, DV1_CFG_NCTL_REG_49_RG_CFG_NCTL_RAM_WDT_S1_LSP, DV1_CFG_NCTL_REG_49_RG_CFG_NCTL_RAM_WDT_S1_LEN, regVal));

        CHK_FUN_CONTINUE(retVal, dv1_reg_read(DV1_UNIT, DV1_CFG_NCTL_REG_48, &tmpValue));
        /* Patch done plus */
        /* rg_cfg_nctl_cmd_s1 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LEN, DISABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s1 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LEN, DISABLE, &tmpValue));
        /*rg_cfg_nctl_addr_s1 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(DV1_UNIT, DV1_CFG_NCTL_REG_48, tmpValue));
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_reg(dv1_ConfigReg_t configReg, const uint8 *buffer)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    do
    {
        /* cfg_nctl_patch_mode_en =1 (Patch ON), LANE0 & LANE1 */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_23, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LEN, ENABLE));
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_48, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LEN, ENABLE));

        /* cfg_nctl_cpu_en = 2'b10 (Switch to RAM) */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_6, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LSP, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LEN, 0x2u));
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_31, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LSP, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LEN, 0x2u));

        while(((uint32)DV1_CONFIG_SDS_ENTRY_LEN <= (configReg.length - offset)) && (RT_ERR_OK == retVal))
        {
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
            regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_handler_Lane0(regAddr, regVal));
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_handler_Lane1(regAddr, regVal));

            offset += (uint32)DV1_CONFIG_SDS_ENTRY_LEN;
        }
        /* LANE0 & LANE1 cfg_nctl_patch_mode_en =0 (Patch OFF) */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_23, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LEN, DISABLE));
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_48, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LEN, DISABLE));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_method_reg());
        /* cfg_nctl_cpu_en = 2'b01 (Switch to R0M), LANE0*/
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_6, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LSP, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LEN, ENABLE));
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(DV1_UNIT, DV1_CFG_NCTL_REG_31, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LSP, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LEN, ENABLE));

        /*step7: nControl will start from pc=12'h800, RAM Address[0] fetch opcode*/
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_25ua_opfsm_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while((uint32)DV1_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)DV1_PAGE_SWR_OCP | (regAddr << 1));
        CHK_FUN_BREAK(retVal, dv1_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));

        offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_brpatch_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset                                    = (uint32)0u;
    uint32    i                                         = (uint32)0u;
    uint32    regAddr                                   = (uint32)0u;
    uint32    regVal                                    = (uint32)0u;
    uint32    phyPortEnIdx                              = (uint32)0u;
    RtkApiRet retVal                                    = RT_ERR_OK;
    uint8     userPort2T1phy[DV1_RTL9075_INTERFACE_NUM] = {0xFFu, 0u, 1u, 2u, 3u, 4u, 5u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        /* Check enabled port */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_check_eth_enable_phy(&phyPortEnIdx));
        while((uint32)DV1_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)DV1_BR_PORT_START; i <= (uint32)DV1_BR_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if(((uint32)0u != (phyPortEnIdx & ((uint32)1u << i))) && (i != userPort2T1phy[configEth.configPort]))
                {
                    regAddr = (uint32)DV1_PAGE_SWR_OCP | ((dv1_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, dv1_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_polling_timeout(uint32 regAddr, uint32 us)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    timeout                                  = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    while((RT_ERR_OK == retVal) && ((uint32)0u == regValRx[0]))
    {
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        CHK_FUN_CONTINUE(retVal, g_fp_udelay(1u));
        timeout++;
        if(timeout >= us)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            break;
        }
    }

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_get_pcs(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    readAction                               = 0u;
    uint32    sdsDirectAddr[]                          = DV1_SDS_INDIRECT_ACCESS_PCS_CTRL;
    uint32    dataChannel[]                            = DV1_PCS_DATA_CHANNEL;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    do
    {
        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* set read action */
            readAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP));
            readAction |= (((uint32)DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP) | ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_9TH == sysPort) || (DV1_RTK_SYS_PORT_11TH == sysPort))
        {
            /* set read action */
            readAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            readAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_10TH == sysPort) || (DV1_RTK_SYS_PORT_12TH == sysPort))
        {
            /* set read action */
            readAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | ((uint32)(reg + 0x800u) << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            readAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else
        {
            retVal = RT_ERR_PORT_ID;
            continue;
        }

        /* set read action to register */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, sdsDirectAddr[sysPort], readAction, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* polling done bit */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_sgmii_polling_timeout(sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));
        /* read data*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, dataChannel[sysPort], regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, dataChannel[sysPort], regValRx, 1u));
        *value = regValRx[0];
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_set_pcs(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet retVal          = RT_ERR_OK;
    uint32    regVal          = 0u;
    uint32    i               = 0u;
    uint32    writeAction     = 0u;
    uint32    sdsDirectAddr[] = DV1_SDS_INDIRECT_ACCESS_PCS_CTRL;
    uint32    dataChannel[]   = DV1_PCS_DATA_CHANNEL;

    do
    {
        /*read*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_sgmii_eth_get_pcs(sysPort, page, reg, &regVal));

        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }

        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* set write action */
            writeAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP));
            writeAction |= (((uint32)DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP) | ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_9TH == sysPort) || (DV1_RTK_SYS_PORT_11TH == sysPort))
        {
            writeAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            writeAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_10TH == sysPort) || (DV1_RTK_SYS_PORT_12TH == sysPort))
        {
            writeAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | ((uint32)(reg + 0x800u) << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            writeAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else
        {
            retVal = RT_ERR_PORT_ID;
            continue;
        }
        /* set write data */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, dataChannel[sysPort], regVal | (value << startBit), 0u));
        /* set write action to register */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, sdsDirectAddr[sysPort], regVal | writeAction, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* polling done bit */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_sgmii_polling_timeout(sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));

    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_get_aphy(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    readAction                               = 0u;
    uint32    sdsDirectAddr[]                          = DV1_SDS_INDIRECT_ACCESS_DPHY_CTRL;
    uint32    dataChannel[]                            = DV1_DPHY_DATA_CHANNEL;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    do
    {
        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            readAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            readAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_9TH <= sysPort) && (DV1_RTK_SYS_PORT_12TH >= sysPort))
        {
            readAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            readAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP));
        }
        else
        {
            retVal = RT_ERR_PORT_ID;
            continue;
        }

        /* set read action to register */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, sdsDirectAddr[sysPort], readAction, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* polling done bit */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_sgmii_polling_timeout(sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));
        /* read data*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, dataChannel[sysPort], regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, dataChannel[sysPort], regValRx, 1u));
        *value = regValRx[0];
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_eth_set_aphy(dv1_RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet retVal          = RT_ERR_OK;
    uint32    i               = 0u;
    uint32    regVal          = 0u;
    uint32    writeAction     = 0u;
    uint32    sdsDirectAddr[] = DV1_SDS_INDIRECT_ACCESS_DPHY_CTRL;
    uint32    dataChannel[]   = DV1_DPHY_DATA_CHANNEL;

    do
    {
        /*read*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_sgmii_eth_get_aphy(sysPort, page, reg, &regVal));

        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }

        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            writeAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            writeAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_9TH <= sysPort) && (DV1_RTK_SYS_PORT_12TH >= sysPort))
        {
            writeAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | ((uint32)DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            writeAction |= (((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP));
        }
        else
        {
            retVal = RT_ERR_PORT_ID;
            continue;
        }

        /* set write data */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, dataChannel[sysPort], regVal | (value << startBit), 0u));
        /* set write action to register */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, sdsDirectAddr[sysPort], writeAction, 0u));

        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* polling done bit */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_sgmii_polling_timeout(sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));

    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_sgmii_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    portMask = 0u;
    uint32    offset   = 0u;
    uint8     port     = 0u;
    uint32    page     = 0u;
    uint32    reg      = 0u;
    uint32    value    = 0u;

    do
    {
        while(((uint32)DV1_CONFIG_SGMII_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
        {
            /* parsing configuration entry */
            portMask = (((uint32)buffer[offset] << 8) | (uint32)buffer[offset + 1u]) & (uint32)DV1_SGMII_DOMAIN4_SDS_PORT_BITMASK;

            if(DV1_IS_NOT_SDS != (portMask & DV1_WRAP_SDS_PORT_BITMASK & dv1_g_wrapSDSNotPowerUp))
            {
                for(port = (uint8)4u; port < (uint8)9u; port++)
                {
                    if(DV1_IS_WRAP_SDS == ((portMask >> port) & 0x1u))
                    {
                        CHK_FUN_CONTINUE(retVal, dv1_drv_sds_wrap_serdes_init_eth(DV1_UNIT, port));
                        dv1_g_wrapSDSNotPowerUp &= ~((uint32)0x1u << port);
                    }
                }
            }

            value = (((uint32)buffer[offset + 5u] << 8) | (uint32)buffer[offset + 4u]);
            if((uint8)0u != ((buffer[offset] >> 7u) & (uint8)1u))
            {
                /* pcs */
                page = ((uint32)buffer[offset + 2u]);
                reg  = ((uint32)buffer[offset + 3u]);
                for(port = (uint8)0u; port < (uint8)15u; port++)
                {
                    if(DV1_IS_SDS_PORT == ((portMask >> port) & 0x1u))
                    {
                        CHK_FUN_BREAK(retVal, dv1_drv_config_sgmii_eth_set_pcs(port, page, reg, 0u, 15u, value));
                    }
                }
            }
            else
            {
                /* D-PHY */
                page = (uint32)0u;
                reg  = (((uint32)buffer[offset + 3u] << 8) | (uint32)buffer[offset + 2u]);
                for(port = (uint8)0u; port < (uint8)15u; port++)
                {
                    if(DV1_IS_SDS_PORT == ((portMask >> port) & 0x1u))
                    {
                        CHK_FUN_BREAK(retVal, dv1_drv_config_sgmii_eth_set_aphy(port, page, reg, 0u, 15u, value));
                    }
                }
            }
            offset += (uint32)DV1_CONFIG_SGMII_ENTRY_LEN;
        }

        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fe_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    RtkApiRet retVal  = RT_ERR_OK;

    while(DV1_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
    {
        regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
        regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

        regAddr = ((uint32)DV1_PAGE_SWR_OCP | ((DV1_FE_OCP_OFST_ADDR | regAddr) << 1));
        CHK_FUN_BREAK(retVal, dv1_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));

        offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_special_reg_eth(uint32 regAddr, uint32 *regVal, uint32 configPort)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    portNum          = (uint32)0u;
    uint32    portProperty[12] = {
        DV1_PORT1_PROPERTY_CONFIGURE, DV1_PORT2_PROPERTY_CONFIGURE, DV1_PORT3_PROPERTY_CONFIGURE, DV1_PORT4_PROPERTY_CONFIGURE, DV1_PORT5_PROPERTY_CONFIGURE, DV1_PORT6_PROPERTY_CONFIGURE,
        DV1_PORT7_PROPERTY_CONFIGURE, DV1_PORT8_PROPERTY_CONFIGURE, DV1_PORT9_PROPERTY_CONFIGURE, DV1_PORT10_PROPERTY_CONFIGURE, DV1_PORT11_PROPERTY_CONFIGURE,
        DV1_PORT12_PROPERTY_CONFIGURE};

    if(0u != configPort)
    {
        /* Check L2 MISC rx_en */
        if((uint32)DV1_PORT_L2_MISC == (regAddr & (uint32)DV1_PORT_L2_MISC_MASK))
        {
            portNum = ((regAddr - (uint32)DV1_PORT_L2_MISC) / (uint32)DV1_PORT_L2_MISC_OFFSET);
            /* Enable confiugre port rx_en */
            if((portNum == dv1_g_userPort2sysPort[configPort]) && (1u <= portNum) && (15u >= portNum))
            {
                *regVal |= (uint32)DV1_RX_EN_BIT;
#ifdef DEBUG_SYS
                rtlglue_printf("SPECIAL regAddr %x regVal %x portNum %d\n", regAddr, *regVal, portNum);
#endif
            }
        }
        /* Check port eco clk gated */
        if((uint32)DV1_PORT_CTRL == (regAddr & (uint32)DV1_PORT_CTRL_MASK))
        {
            portNum = ((regAddr - (uint32)DV1_PORT_CTRL) / (uint32)DV1_PORT_CTRL_OFFSET);
            /* Enable confiugre port rx_en */
            if((portNum == dv1_g_userPort2sysPort[configPort]) && (1u <= portNum) && (15u >= portNum))
            {
                *regVal |= (uint32)1u << DV1_PORT0_PORT_CTRL0_RG_ECO_CLK_GATED_LSP;
#ifdef DEBUG_SYS
                rtlglue_printf("SPECIAL regAddr %x regVal %x portNum %d\n", regAddr, *regVal, portNum);
#endif
            }
        }
#if 0 /* DEPRECATED after 6764A!!!*/
        else if(((uint32)DV1_DESIGN_FOR_MEASTUMENT & (uint32)0xFFFFFFu) == regAddr)
        {
            /*do not disable clk of configPort*/
            portNum = dv1_g_userPort2sysPort[configPort];
            *regVal |= (uint32)1u << (portNum + 8u);
            *regVal |= (uint32)1u << (23u);

#ifdef DEBUG_SYS
            rtlglue_printf("SPECIAL regAddr %x regVal %x portNum %d\n", regAddr, *regVal, portNum);
#endif
        }
#endif
        else if((uint32)(portProperty[configPort - 1u] & (uint32)0xFFFFFFu) == regAddr)
        {
            /*do not force configPort link down*/
            *regVal |= (uint32)1u << DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_FORCE_LINK_LSP;
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
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mac_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    offset           = (uint32)0u;
    bool      waitRegVal       = false;
    uint32    regAddr          = (uint32)0u;
    uint32    regVal           = (uint32)0u;
    uint32    regAddrOtherPage = (uint32)0u;

    do
    {
        while(((uint32)DV1_CONFIG_MAC_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
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

                    CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
                    CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select((regAddrOtherPage >> 24) & 0xFFu));
                    CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
                    CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
                    CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(0x4A));
                }

                waitRegVal = false;
            }
            else
            {

                CHK_FUN_CONTINUE(retVal, dv1_drv_config_special_reg_eth(regAddr, &regVal, configEth.configPort));

                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
            }
            offset += (uint32)DV1_CONFIG_MAC_ENTRY_LEN;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rtpg_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    uint32    offset                                    = (uint32)0u;
    uint32    i                                         = (uint32)0u;
    uint32    regAddr                                   = (uint32)0u;
    uint32    regVal                                    = (uint32)0u;
    uint32    phyPortEnIdx                              = (uint32)0u;
    RtkApiRet retVal                                    = RT_ERR_OK;
    uint8     userPort2T1phy[DV1_RTL9075_INTERFACE_NUM] = {0xFFu, 0u, 1u, 2u, 3u, 4u, 5u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        /* Check enabled port */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_check_eth_enable_phy(&phyPortEnIdx));
        while((uint32)DV1_CONFIG_OCP_ENTRY_LEN <= (configEth.length - offset))
        {
            regVal = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);

            for(i = (uint32)DV1_RTPG_PORT_START; i <= (uint32)DV1_RTPG_PORT_END; i++)
            {
                regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
                if(((uint32)0u != (phyPortEnIdx & ((uint32)1u << i))) && (i != userPort2T1phy[configEth.configPort]))
                {
                    regAddr = (uint32)DV1_PAGE_SWR_OCP | ((dv1_g_phyIdxToOcpIdx[i] | regAddr) << 1u);
                    CHK_FUN_BREAK(retVal, dv1_drv_config_eth_write_field(regAddr, 0u, 16u, regVal));
                }
            }
            if(RT_ERR_OK != retVal)
            {
                break;
            }
            offset += (uint32)DV1_CONFIG_OCP_ENTRY_LEN;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dv1_drv_extphy_eth_read_handler(uint32 regAddr, uint32 *regVal)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regVal, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_SRAM_READ, regAddr, regVal, 1u));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dv1_drv_extphy_eth_write_handler(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_SRAM_WRITE, regAddr, regVal, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dv1_drv_extphy_update_code_length_eth(uint32 phyAddr, uint32 idx)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    tempLen = 0u;

    do
    {
        tempLen = phyAddr - (uint32)DV1_EXT_PHY_PATCH_CODE_ADDR + idx;
        if((uint32)DV1_EXT_PHY_PATCH_LENGTH >= tempLen)
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_eth_write_handler((uint32)DV1_EXT_PHY_PATCH_LEN_ADDR, tempLen));
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

static RtkApiRet dv1_drv_extphy_code_offset_correction_handler_eth(uint32 entry, uint32 *phyAddr, uint32 *regRx, const uint8 *buf, uint32 *shft)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    temp   = 0u;
    uint32    i      = 0u;

    do
    {
        if(0u != entry)
        {
            /* Set read command */
            CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_eth_read_handler(*phyAddr, regRx));
            temp = regRx[0];
            for(i = entry; i < 4u; i++)
            {
                temp |= (uint32)buf[i - entry] << (8u * i);
                (*shft) += 1u;
            }
            /* Set write command */
            CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_eth_write_handler(*phyAddr, temp));
            *phyAddr += 4u;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_extphy_patch_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
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
        offset = configEth.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;
        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configEth.length) || ((uint32)DV1_EXT_PHY_PATCH_LENGTH < (offset + configEth.length)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if(0u == configEth.blockIdx)
        {
            extPhyPatchAddr = (uint32)DV1_EXT_PHY_PATCH_CODE_ADDR;
            entryIdx        = 0u;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_code_offset_correction_handler_eth(entryIdx, &extPhyPatchAddr, regValRx, buffer, &shift));
        }

        extPhyConfigLen = configEth.length - (configEth.length % (uint32)DV1_EXT_PHY_PATCH_ENTRY_LEN) - shift;
        for(i = (uint32)0u; i < (extPhyConfigLen / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[shift + 0u + (i * 4u)] | ((uint32)buffer[shift + 1u + (i * 4u)] << 8) | ((uint32)buffer[shift + 2u + (i * 4u)] << 16) | ((uint32)buffer[shift + 3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_SRAM_WRITE, extPhyPatchAddr, cfgBuf, 0u));
            extPhyPatchAddr += 4u;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_SRAM_WRITE, 0u, 0u, 1u));

        cfgBuf = 0u;
        for(entryIdx = 0u; entryIdx < (extPhyConfigLen % (uint32)4u); entryIdx++)
        {
            cfgBuf |= (uint32)buffer[shift + entryIdx + (i * 4u)] << (8u * entryIdx);
        }
        /* Set write command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_eth_write_handler(extPhyPatchAddr, cfgBuf));

        CHK_FUN_CONTINUE(retVal, dv1_drv_extphy_update_code_length_eth(extPhyPatchAddr, entryIdx));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_fwcfg_byte_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;

    do
    {
        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
        while(((uint32)DV1_CONFIG_MAC_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
        {
            /* parsing configuration entry */
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8) | ((uint32)buffer[offset + 2u] << 16);
            regVal  = (uint32)buffer[offset + 3u] | ((uint32)buffer[offset + 4u] << 8) | ((uint32)buffer[offset + 5u] << 16) | ((uint32)buffer[offset + 6u] << 24);
            if((DV1_ADDR_BOOTCFG_BASE > (regAddr | ((uint32)DV1_PAGE_SYS_MEM << 24))) || ((DV1_ADDR_BOOTCFG_BASE + DV1_ADDR_BOOTCFG_SIZE) < (regAddr | ((uint32)DV1_PAGE_SYS_MEM << 24))))
            {
                offset += (uint32)DV1_CONFIG_MAC_ENTRY_LEN;
                continue;
            }
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
            offset += (uint32)DV1_CONFIG_MAC_ENTRY_LEN;
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 1u));
        /* select page to 0x4A */
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SW));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_reg_field_write_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regAddr[RTK_ETHERACCESS_MAX_BURST - 1u]  = {(uint32)0u};
    uint32    regVal[RTK_ETHERACCESS_MAX_BURST - 1u]   = {(uint32)0u};
    uint32    offset                                   = (uint32)0u;
    uint16    cnt                                      = (uint16)0u;
    uint8     startBit[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint8)0u};
    uint8     endBit[RTK_ETHERACCESS_MAX_BURST - 1u]   = {(uint8)0u};

    while(((uint32)DV1_CFG_REG_FIELD_WR_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
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
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[cnt], regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[cnt], regValRx, 1u));
        /* Register field set */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(regAddr[cnt], (uint32)startBit[cnt], ((uint32)endBit[cnt] - (uint32)startBit[cnt]), regVal[cnt], &regValRx[cnt]));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_special_reg_eth(regAddr[cnt], &regValRx[cnt], configEth.configPort));
#ifdef DEBUG_SYS
        rtlglue_printf("regAddr %x regVal %x\n", regAddr[cnt], regVal[cnt]);
#endif

        /* Set write command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr[cnt], regValRx[cnt], 0u));
        /* Send write command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        offset += (uint32)DV1_CFG_REG_FIELD_WR_ENTRY_LEN;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_mka_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    cfgBuf = (uint32)0u;
    uint32    i      = (uint32)0u;

    do
    {
        /*length must equal to DV1_MKA_CONFIG_SIZE*/
        if((uint32)DV1_MKA_CONFIG_SIZE != (configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
        for(i = (uint32)0u; i < (configEth.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((i * 4u) + (uint32)DV1_MKA_CONFIG_ADDR), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* select page to 0x4A */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SW));
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_handler_check_eth(dv1_ConfigEth_t configEth)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    offset = (uint32)0u;
    do
    {

        offset = configEth.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_ROUTING_TBL_SIZE < (offset + configEth.length))
        {
            retVal = -RT_ERR_INPUT;
        }
        /*not 4bytes aligment */
        if((uint32)0u != (configEth.length % ((uint32)4u)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
    } while(0u == 1u);
    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regAddr[RTK_ETHERACCESS_MAX_BURST - 1u]  = {(uint32)0u};
    uint32    offset                                   = (uint32)0u;
    uint32    cfgBuf                                   = (uint32)0u;
    uint32    i                                        = (uint32)0u;
    uint32    lengthLeft                               = (uint32)0u;

    do
    {

        offset = configEth.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        /* Check valid and select Page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_rot_sw_handler_check_eth(configEth));
        lengthLeft = configEth.length - 2u;
        regAddr[0] = (uint32)DV1_ROUTING_TBL_ADDR + offset - 2u;
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 1u));
        cfgBuf = (regValRx[0] & 0xFFFFu) | ((uint32)buffer[0] << 16) | ((uint32)buffer[1] << 24);
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr[0], cfgBuf, 0u));
        for(i = (uint32)0u; i < (lengthLeft / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8) | ((uint32)buffer[4u + (i * 4u)] << 16) | ((uint32)buffer[5u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((i * 4u) + (uint32)DV1_ROUTING_TBL_ADDR + offset + 2u), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        regAddr[0] = ((i * 4u) + (uint32)DV1_ROUTING_TBL_ADDR + offset + 2u);
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 1u));

        cfgBuf = (regValRx[0] & 0xFFFF0000u) | (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8);
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr[0], cfgBuf, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* select page to 0x4A */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SW));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_intf_handler_check_eth(dv1_ConfigEth_t configEth)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    offset = (uint32)0u;
    do
    {

        offset = configEth.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_ROUTING_INTF_TBL_SIZE < (offset + configEth.length))
        {
            retVal = -RT_ERR_INPUT;
        }
        /*not 4bytes aligment */
        if((uint32)0u != (configEth.length % ((uint32)4u)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_rot_sw_intf_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regAddr[RTK_ETHERACCESS_MAX_BURST - 1u]  = {(uint32)0u};
    uint32    offset                                   = (uint32)0u;
    uint32    cfgBuf                                   = (uint32)0u;
    uint32    i                                        = (uint32)0u;
    uint32    lengthLeft                               = (uint32)0u;

    do
    {

        offset = configEth.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        /* Check valid and select Page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_rot_sw_intf_handler_check_eth(configEth));
        lengthLeft = configEth.length - 2u;
        regAddr[0] = (uint32)DV1_ROUTING_INTF_TBL_ADDR + offset - 2u;
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 1u));
        cfgBuf = (regValRx[0] & 0xFFFFu) | ((uint32)buffer[0] << 16) | ((uint32)buffer[1] << 24);
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr[0], cfgBuf, 0u));
        for(i = (uint32)0u; i < (lengthLeft / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8) | ((uint32)buffer[4u + (i * 4u)] << 16) | ((uint32)buffer[5u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((i * 4u) + (uint32)DV1_ROUTING_INTF_TBL_ADDR + offset + 2u), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        regAddr[0] = ((i * 4u) + (uint32)DV1_ROUTING_INTF_TBL_ADDR + offset + 2u);
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr[0], regValRx, 1u));

        cfgBuf = (regValRx[0] & 0xFFFF0000u) | (uint32)buffer[2u + (i * 4u)] | ((uint32)buffer[3u + (i * 4u)] << 8);
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr[0], cfgBuf, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* select page to 0x4A */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SW));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_memcpy_eth(const uint8 *buffer, uint32 length, uint32 offset, uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint32)0u;
    uint32    i      = (uint32)0u;
    do
    {
        for(i = (uint32)0u; i < length; i += 4u)
        {
            regVal = (uint32)buffer[i] | ((uint32)buffer[i + 1u] << 8) | ((uint32)buffer[i + 2u] << 16) | ((uint32)buffer[i + 3u] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, (macPhyAddr + DV1_MPY_NIC_CONFIG_HEADER_LEN + offset + i), regVal, 0u));
        }
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_error_eth(uint32 macPhyAddr)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr, 0u, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr + 4u, 0u, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_macphy_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet     retVal          = RT_ERR_OK;
    static uint32 configTotalLen  = (uint32)0u;
    uint32        offset          = (uint32)0u;
    const uint32  macPhyAddrMap[] = {(uint32)DV1_MPY_NIC_BUF_ADDR_0, (uint32)DV1_MPY_NIC_BUF_ADDR_1};
    uint32        macPhyAddr      = macPhyAddrMap[configEth.index - DV1_CFG_MACPHY_0_CONFIG_IDX];
    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_MPY_PAGE));
        offset = configEth.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_MPY_NIC_CONFIG_LIMIT_LEN < (offset + configEth.length))
        {
            /* clean the magic number and length*/
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_macphy_error_eth(macPhyAddr));
            retVal = -RT_ERR_INPUT;
            continue;
        }
        else
        {
            if((uint32)0u == configEth.blockIdx)
            {
                /* set magic number for identifying the config data */
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr, DV1_MPY_NIC_CONFIG_MAGIC_NUM, 0u));
            }

            CHK_FUN_CONTINUE(retVal, dv1_drv_config_macphy_memcpy_eth(buffer, configEth.length, offset, macPhyAddr));

            configTotalLen = offset + configEth.length;
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, macPhyAddr + 4u, configTotalLen, 0u));
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        }
        if(RT_ERR_OK != retVal)
        {
            /* clean the magic number and length */
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_macphy_error_eth(macPhyAddr));
        }
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(0x4Au));
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_ptp_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    offset = (uint32)0u;
    uint32    cfgBuf = (uint32)0u;
    uint32    i      = (uint32)0u;

    do
    {

        offset = configEth.blockIdx * (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH;

        if((0xFFFFFu < offset) || (DV1_CONFIG_BLOCK_MAX_LENGTH < configEth.length))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((uint32)DV1_PTP_STATUS_SIZE < (offset + configEth.length))
        {
            retVal = -RT_ERR_INPUT;
        }
        /*not 4bytes aligment */
        if((uint32)0u != (configEth.length % ((uint32)4u)))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
        for(i = (uint32)0u; i < (configEth.length / (uint32)4u); i++)
        {
            cfgBuf = (uint32)buffer[0u + (i * 4u)] | ((uint32)buffer[1u + (i * 4u)] << 8) | ((uint32)buffer[2u + (i * 4u)] << 16) | ((uint32)buffer[3u + (i * 4u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, ((i * 4u) + (uint32)DV1_PTP_STATUS_ADDR + offset), cfgBuf, 0u));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
        /* select page to 0x4A */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SW));
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table0_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    entryNum                                 = useLen / (tabLen + DV1_CONFIG_TABLE0_HEADER_LEN);
    uint32    tabAddr                                  = (uint32)0u;
    uint32    counter                                  = (uint32)0u;
    uint32    i                                        = (uint32)0;
    uint32    j                                        = (uint32)0u;
    uint32    tabData                                  = (uint32)0u;
    uint32    tabTimeOut                               = (uint32)0u;
    uint32    controlReg                               = (uint32)0u;
    uint32    dataBaseReg                              = (uint32)0u;
    uint32    tabBaseAddr                              = (uint32)0u;
    uint32    tabAddrMultiMap[]                        = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                                          0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    switch(tableType)
    {
        case(uint32)DV1_QBV_GATE_CONTROL_LIST_TABLE:
        case(uint32)DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE:
        case(uint32)DV1_RH_TABLE:
        case(uint32)DV1_OIL_TABLE:
        case(uint32)DV1_EGIF_TABLE:
            controlReg  = DV1_MAC_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_MAC_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_MAC_BASE_ADDR;
            break;
        default:
            controlReg  = DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_ALE_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_ALE_BASE_ADDR;
            break;
    }
    while((counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[i + 3u]) << 24u) | ((uint32)(buffer[(i + 2u)]) << 16u) | ((uint32)(buffer[(i + 1u)]) << 8u) | (uint32)buffer[i];
        tabAddr = tabAddr - (tabBaseAddr + (DV1_TABLE_INDEX_GAP * tableType));
        tabAddr = tabAddr / (tabAddrMultiMap[dv1_table_list[tableType].datareg_num]);
        i += (uint32)DV1_CONFIG_TABLE0_HEADER_LEN;

        for(j = (uint32)0u; j < tabLen; j += (uint32)DV1_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, (dataBaseReg + j), tabData, 0u));
            i += (uint32)DV1_CONFIG_TABLE_DATA_LEN;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = DV1_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, controlReg, tabData, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, controlReg, regValRx, 0u));
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, controlReg, regValRx, 1u));
            if((uint32)0u == (regValRx[0] & (uint32)DV1_CONFIG_TABLE_CTRL_BIT))
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
        ++counter;
    }

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table1_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    entryNum                                 = (useLen - DV1_CONFIG_TABLE1_HEADER_LEN) / tabLen;
    uint32    tabAddr                                  = (uint32)0u;
    uint32    counter                                  = (uint32)0u;
    uint32    i                                        = (uint32)0;
    uint32    j                                        = (uint32)0u;
    uint32    tabData                                  = (uint32)0u;
    uint32    tabTimeOut                               = (uint32)0u;
    uint32    controlReg                               = (uint32)0u;
    uint32    dataBaseReg                              = (uint32)0u;
    uint32    tabBaseAddr                              = (uint32)0u;
    uint32    tabAddrMultiMap[]                        = {0xFFu, 0x8u, 0x8u, 0x10u, 0x10u, 0x20u, 0x20u, 0x20u, 0x20u,
                                                          0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u, 0x40u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
                                                          0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    switch(tableType)
    {
        case(uint32)DV1_QBV_GATE_CONTROL_LIST_TABLE:
        case(uint32)DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE:
        case(uint32)DV1_RH_TABLE:
        case(uint32)DV1_OIL_TABLE:
        case(uint32)DV1_EGIF_TABLE:
            controlReg  = DV1_MAC_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_MAC_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_MAC_BASE_ADDR;
            break;
        default:
            controlReg  = DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_ALE_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            tabBaseAddr = DV1_TABLE_ALE_BASE_ADDR;
            break;
    }

    tabAddr = tabBaseAddr | ((uint32)(buffer[2u]) << 16u) | ((uint32)(buffer[1u]) << 8u) | (uint32)buffer[0u];
    tabAddr = tabAddr - (tabBaseAddr + (DV1_TABLE_INDEX_GAP * tableType));
    tabAddr = tabAddr / (tabAddrMultiMap[dv1_table_list[tableType].datareg_num]);
    i += (uint32)DV1_CONFIG_TABLE1_HEADER_LEN;

    while((counter < entryNum) && (RT_ERR_OK == retVal) && ((uint32)DV1_TABLE_ENTRY_MAX > tabAddr))
    {
        for(j = (uint32)0u; j < tabLen; j += (uint32)DV1_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, (dataBaseReg + j), tabData, 0u));
            i += (uint32)DV1_CONFIG_TABLE_DATA_LEN;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = DV1_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, controlReg, tabData, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, controlReg, regValRx, 0u));
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, controlReg, regValRx, 1u));
            if((uint32)0u == (regValRx[0] & (uint32)DV1_CONFIG_TABLE_CTRL_BIT))
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
        ++counter;
        tabAddr++;
    }

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table2_handler_eth(uint32 tableType, uint32 tabLen, const uint8 *buffer, uint32 useLen)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    entryNum                                 = useLen / (tabLen + DV1_CONFIG_TABLE2_HEADER_LEN);
    uint32    tabAddr                                  = (uint32)0u;
    uint32    counter                                  = (uint32)0u;
    uint32    i                                        = (uint32)0;
    uint32    j                                        = (uint32)0u;
    uint32    tabData                                  = (uint32)0u;
    uint32    tabTimeOut                               = (uint32)0u;
    uint32    controlReg                               = (uint32)0u;
    uint32    dataBaseReg                              = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    switch(tableType)
    {
        case(uint32)DV1_QBV_GATE_CONTROL_LIST_TABLE:
        case(uint32)DV1_QBV_SERIAL_GATE_OPEN_TIME_TABLE:
        case(uint32)DV1_RH_TABLE:
        case(uint32)DV1_OIL_TABLE:
        case(uint32)DV1_EGIF_TABLE:
            controlReg  = DV1_MAC_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_MAC_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            break;
        default:
            controlReg  = DV1_ALE_INDIRECT_CONTROL_FOR_EXTERNAL_DEVICE;
            dataBaseReg = DV1_ALE_INDIRECT_DATA0_FOR_EXTERNAL_DEVICE;
            break;
    }

    while((counter < entryNum) && (RT_ERR_OK == retVal))
    {
        tabAddr = ((uint32)(buffer[(i + 1u)]) << 8) | (uint32)buffer[i];
        i += (uint32)DV1_CONFIG_TABLE2_HEADER_LEN;

        for(j = (uint32)0u; j < tabLen; j += (uint32)DV1_CONFIG_TABLE_DATA_LEN)
        {
            tabData = (uint32)buffer[i] | ((uint32)buffer[(i + 1u)] << 8) | ((uint32)buffer[(i + 2u)] << 16) | ((uint32)buffer[(i + 3u)] << 24);
            CHK_FUN_BREAK(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, (dataBaseReg + j), tabData, 0u));
            i += (uint32)DV1_CONFIG_TABLE_DATA_LEN;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        tabData = DV1_CONFIG_TABLE_CTRL_BIT | ((uint32)tableType << 16) | tabAddr;

        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, controlReg, tabData, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));

        while(RT_ERR_OK == retVal)
        {
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, controlReg, regValRx, 0u));
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, controlReg, regValRx, 1u));
            if((uint32)0u == (regValRx[0] & (uint32)DV1_CONFIG_TABLE_CTRL_BIT))
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
        ++counter;
    }

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_table_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    tableType = configEth.index - DV1_CFG_TBL_BASE_IDX;
    uint32    tabLen    = dv1_table_list[tableType].datareg_num * 4u;
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
                retVal = dv1_drv_config_table0_handler_eth(tableType, tabLen, buffer, useLen);
                break;
            case(1u):
                retVal = dv1_drv_config_table1_handler_eth(tableType, tabLen, buffer, useLen);
                break;
            case(2u):
                retVal = dv1_drv_config_table2_handler_eth(tableType, tabLen, buffer, useLen);
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(0u == 1u);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_method_eth(void)
{
    RtkApiRet    retVal                                   = RT_ERR_OK;
    uint32       regVal                                   = (uint32)0u;
    uint32       i                                        = (uint32)0u;
    uint32       serdesPatchMethod                        = (uint32)0u;
    uint32       regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    const uint32 krConfig[4][10]                          = {
        {(uint32)DV1_CFG_NCTL_REG_11,
                                  (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_EN_P04_LSP, (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_EN_P04_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_ADDR_P04_LSP, (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_ADDR_P04_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_TARG_P04_LSP, (uint32)DV1_CFG_NCTL_REG_11_RG_CFG_NCTL_BP_TARG_P04_LEN,
                                  (uint32)ENABLE, (uint32)0x00C1u, (uint32)0x080Fu},
        {(uint32)DV1_CFG_NCTL_REG_12,
                                  (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_EN_P05_LSP, (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_EN_P05_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_ADDR_P05_LSP, (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_ADDR_P05_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_TARG_P05_LSP, (uint32)DV1_CFG_NCTL_REG_12_RG_CFG_NCTL_BP_TARG_P05_LEN,
                                  (uint32)ENABLE, (uint32)0x00C5u, (uint32)0x0812u},
        {(uint32)DV1_CFG_NCTL_REG_36,
                                  (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_EN_P04_S1_LSP, (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_EN_P04_S1_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_ADDR_P04_S1_LSP, (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_ADDR_P04_S1_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_TARG_P04_S1_LSP, (uint32)DV1_CFG_NCTL_REG_36_RG_CFG_NCTL_BP_TARG_P04_S1_LEN,
                                  (uint32)ENABLE, (uint32)0x00C1u, (uint32)0x080Fu},
        {(uint32)DV1_CFG_NCTL_REG_37,
                                  (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_EN_P05_S1_LSP, (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_EN_P05_S1_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_ADDR_P05_S1_LSP, (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_ADDR_P05_S1_LEN,
                                  (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_TARG_P05_S1_LSP, (uint32)DV1_CFG_NCTL_REG_37_RG_CFG_NCTL_BP_TARG_P05_S1_LEN,
                                  (uint32)ENABLE, (uint32)0x00C5u, (uint32)0x0812u}};

    do
    {
        /* select page to 0x01 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SYS_MEM));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_SERDES_PATCH_METHOD_ADDRESS, regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_SERDES_PATCH_METHOD_ADDRESS, regValRx, 1u));
        serdesPatchMethod = regValRx[0] & (uint32)0xffu;
        CHK_FUN_CONTINUE(retVal, dv1_drv_eth_acc_flow_page_select(DV1_PAGE_SW));
        if((uint32)0u == serdesPatchMethod)
        {
            for(i = (uint32)0u; i < (uint32)4u; i++)
            {
                if(RT_ERR_OK != retVal)
                {
                    break;
                }
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, krConfig[i][0], regValRx, 0u));
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, krConfig[i][0], regValRx, 1u));
                regVal = regValRx[0];
                CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, krConfig[i][1], krConfig[i][2], krConfig[i][7], &regVal));
                CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, krConfig[i][3], krConfig[i][4], krConfig[i][8], &regVal));
                CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, krConfig[i][5], krConfig[i][6], krConfig[i][9], &regVal));
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, krConfig[i][0], regVal, 0u));
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
            }
        }
    } while(0u == 1u);

    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane0_patch_eth(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    tmpValue                                 = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    do
    {
        /* Write RAM Address 12'h080F, 12'h0810, 12'h0811 ... (bit 12 must be 1) */
        /* Patch control and address */
        /* LAN0 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_23, regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_23, regValRx, 1u));
        tmpValue = regValRx[0];
        /* rg_cfg_nctl_cmd_s0 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LEN, ENABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s0 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LEN, ENABLE, &tmpValue));
        /*rg_cfg_nctl_addr_s0 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_23, tmpValue, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_23, tmpValue, 1u));
        /* (Patch code e.g. 0xC655, 0xC000, 0x20C2 ... ) unitl all patch cod to RAM is  completed */
        /*Patch data*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_24, DV1_CFG_NCTL_REG_24_RG_CFG_NCTL_RAM_WDT_S0_LSP, DV1_CFG_NCTL_REG_24_RG_CFG_NCTL_RAM_WDT_S0_LEN, regVal));

    } while(0u == 1u);
    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane0_eth(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    tmpValue                                 = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_handler_Lane0_patch_eth(regAddr, regVal));

        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_23, regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_23, regValRx, 1u));
        tmpValue = regValRx[0];
        /* Patch done plus */
        /* rg_cfg_nctl_cmd_s1 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_CMD_S0_LEN, DISABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s1 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_WR_S0_LEN, DISABLE, &tmpValue));
        /*rg_cfg_nctl_addr_s1 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_ADDR_S0_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_23, tmpValue, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_23, tmpValue, 1u));
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane1_patch_eth(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    tmpValue                                 = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    do
    {
        /* LAN1 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_48, regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_48, regValRx, 1u));
        tmpValue = regValRx[0];
        /* rg_cfg_nctl_cmd_s1 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LEN, DISABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s1 enable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LEN, DISABLE, &tmpValue));
        /*rg_cfg_nctl_addr_s1 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_48, tmpValue, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_48, tmpValue, 1u));
        /* (Patch code e.g. 0xC655, 0xC000, 0x20C2 ... ) unitl all patch cod to RAM is  completed */
        /*Patch data*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_49, DV1_CFG_NCTL_REG_49_RG_CFG_NCTL_RAM_WDT_S1_LSP, DV1_CFG_NCTL_REG_49_RG_CFG_NCTL_RAM_WDT_S1_LEN, regVal));

    } while(0u == 1u);
    return retVal;
}
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_Lane1_eth(uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    tmpValue                                 = (uint32)0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_handler_Lane1_patch_eth(regAddr, regVal));

        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_48, regValRx, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_CFG_NCTL_REG_48, regValRx, 1u));
        tmpValue = regValRx[0];
        /* Patch done plus */
        /* rg_cfg_nctl_cmd_s1 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_CMD_S1_LEN, ENABLE, &tmpValue));
        /* rg_cfg_nctl_wr_s1 disable */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_WR_S1_LEN, ENABLE, &tmpValue));
        /*rg_cfg_nctl_addr_s1 address write */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(0u, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_ADDR_S1_LEN, regAddr, &tmpValue));
        /* write data */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_48, tmpValue, 0u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, DV1_CFG_NCTL_REG_48, tmpValue, 1u));
    } while(0u == 1u);
    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_5gkr_patch_handler_eth(dv1_ConfigEth_t configEth, const uint8 *buffer)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    offset  = (uint32)0u;
    uint32    regAddr = (uint32)0u;
    uint32    regVal  = (uint32)0u;
    do
    {
        /* cfg_nctl_patch_mode_en =1 (Patch ON), LANE0 & LANE1 */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_23, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LEN, ENABLE));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_48, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LEN, ENABLE));

        /* cfg_nctl_cpu_en = 2'b10 (Switch to RAM) */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_6, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LSP, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LEN, 0x2u));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_31, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LSP, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LEN, 0x7u));

        while(((uint32)DV1_CONFIG_SDS_ENTRY_LEN <= (configEth.length - offset)) && (RT_ERR_OK == retVal))
        {
            regAddr = (uint32)buffer[offset] | ((uint32)buffer[offset + 1u] << 8);
            regVal  = (uint32)buffer[offset + 2u] | ((uint32)buffer[offset + 3u] << 8);
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_handler_Lane0_eth(regAddr, regVal));
            CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_handler_Lane1_eth(regAddr, regVal));

            offset += (uint32)DV1_CONFIG_SDS_ENTRY_LEN;
        }
        /* LANE0 & LANE1 cfg_nctl_patch_mode_en =0 (Patch OFF) */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_23, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LSP, DV1_CFG_NCTL_REG_23_RG_CFG_NCTL_PATCH_MODE_EN_LEN, DISABLE));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_48, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LSP, DV1_CFG_NCTL_REG_48_RG_CFG_NCTL_PATCH_MODE_EN_S1_LEN, DISABLE));
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_5gkr_patch_method_eth());
        /* cfg_nctl_cpu_en = 2'b01 (Switch to R0M), LANE0*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_6, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LSP, DV1_CFG_NCTL_REG_6_RG_CFG_NCTL_CPU_EN_S0_LEN, ENABLE));
        /* cfg_nctl_cpu_en = 2'b01 (Switch to R0M), LANE1*/
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_write_field(DV1_CFG_NCTL_REG_31, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LSP, DV1_CFG_NCTL_REG_31_RG_CFG_NCTL_CPU_EN_S1_LEN, ENABLE));

        /* nControl will start from pc=12'h800, RAM Address[0] fetch opcode */
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
RtkApiRet dv1_drv_config_parse_flash_cfg_header(const int8 *fileName, uint32 spiStartAddr[], uint32 spiParaLen[], uint8 stkIdx)
{
    uint16    i                                   = (uint16)0u;
    uint32    spiValid                            = (uint32)0xFFFFu;
    uint8     configAddrLenBuf[5]                 = {(uint8)0u};
    uint32    configAddr                          = (uint32)0u;
    uint32    configLength                        = (uint32)0u;
    uint8     hdrBuf[DV1_FLASH_CONFIG_HEADER_LEN] = {(uint8)0u};
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
        if((int32)0 == rtl_file_read(fp, configAddrLenBuf, DV1_FLASH_HEADER_OFFSET + 9u + (stkIdx * 5u), 5u))
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

        if((int32)0 == rtl_file_read(fp, hdrBuf, configAddr, DV1_FLASH_CONFIG_HEADER_LEN))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        spiValid = (uint32)hdrBuf[0] | ((uint32)hdrBuf[1] << 8u);

        if((uint32)DV1_CFG_SPI_IDENTIFY != spiValid)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        spiStartAddr[0] = configAddr + (uint32)DV1_FLASH_CONFIG_HEADER_LEN;
        for(i = 0u; i < DV1_CFG_END_IDX; i++)
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
                    spiStartAddr[i] += (((uint32)DV1_CONFIG_BLOCK_ALIGNMENT_LENGTH - ((spiParaLen[i - 1u] & (uint32)0xFFFFF) % (uint32)DV1_CONFIG_BLOCK_ALIGNMENT_LENGTH)) % (uint32)DV1_CONFIG_BLOCK_ALIGNMENT_LENGTH);
                    /* add hash length for each block of previous domain */
                    blockNum = ((((spiParaLen[i - 1u] & (uint32)0xFFFFF) - 1u) / (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH) + (uint32)1u);
                    spiStartAddr[i] += (blockNum * (uint32)DV1_CONFIG_BLOCK_HASH_LENGTH);
                }
            }
            offset += DV1_FLASH_DOMAIN_LEN;
        }
    } while(0u == 1u);

    rtl_file_close(&fp);

    return retVal;
}

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_call_handler_reg(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp)
{
    RtkApiRet                   retVal                               = RT_ERR_OK;
    uint32                      dataLen                              = (uint32)0u;
    uint32                      readLen                              = (uint32)0u;
    uint8                       dataBuf[DV1_CONFIG_BLOCK_MAX_LENGTH] = {(uint8)0u};
    uint32                      blockIdx                             = (uint32)0u;
    uint32                      paraLen                              = (uint32)0u;
    uint32                      readAddr                             = (uint32)0u;
    uint32                      flags                                = (uint32)0u;
    uint32                      i                                    = (uint32)0u;
    dv1_ConfigReg_t             configReg;
    static dv1_ConfigHandlerReg configHandlersReg[DV1_CFG_END_IDX] =
        {
            [DV1_CFG_25UA_IDX]            = &dv1_drv_config_25ua_opfsm_handler_reg,
            [DV1_CFG_OPFSM_IDX]           = &dv1_drv_config_25ua_opfsm_handler_reg,
            [DV1_CFG_BR_PATCH_1_IDX]      = &dv1_drv_config_brpatch_handler_reg,
            [DV1_CFG_BR_PATCH_2_IDX]      = &dv1_drv_config_brpatch_handler_reg,
            [DV1_CFG_SGMII_PATCH_IDX]     = &dv1_drv_config_sgmii_handler_reg,
            [DV1_CFG_FE_PATCH_IDX]        = &dv1_drv_config_fe_handler_reg,
            [DV1_CFG_MAC_CONF_IDX]        = &dv1_drv_config_mac_handler_reg,
            [DV1_CFG_SOMEIP_IDX]          = NULL,
            [DV1_CFG_RESERVED_1]          = NULL,
            [DV1_CFG_RTPG_PATCH_1_IDX]    = &dv1_drv_config_rtpg_handler_reg,
            [DV1_CFG_RTPG_PATCH_2_IDX]    = &dv1_drv_config_rtpg_handler_reg,
            [DV1_CFG_STACKING_IDX]        = &dv1_drv_config_extphy_patch_handler_reg,
            [DV1_CFG_PCIE_IDX]            = NULL,
            [DV1_CFG_FWCFG_BYTE_IDX]      = &dv1_drv_config_fwcfg_byte_handler_reg,
            [DV1_CFG_REG_FIELD_WR_IDX]    = &dv1_drv_config_reg_field_write_handler_reg,
            [DV1_CFG_MKA_IDX]             = &dv1_drv_config_mka_handler_reg,
            [DV1_CFG_ROT_SW_TABLE]        = &dv1_drv_config_rot_sw_handler_reg,
            [DV1_CFG_ROT_SW_INTF_TABLE]   = &dv1_drv_config_rot_sw_intf_handler_reg,
            [DV1_CFG_MACPHY_0_CONFIG_IDX] = &dv1_drv_config_macphy_handler_reg,
            [DV1_CFG_MACPHY_1_CONFIG_IDX] = &dv1_drv_config_macphy_handler_reg,
            [DV1_CFG_PTP_IDX]             = &dv1_drv_config_ptp_handler_reg,
            [DV1_CFG_QCC_PRE_TBL_IDX]     = NULL,
            [DV1_CFG_5KR_PATCH_IDX]       = &dv1_drv_config_5gkr_patch_handler_reg,
            [DV1_CFG_RESERVED_2]          = NULL,
        };

    for(i = DV1_CFG_TBL_BASE_IDX; i <= DV1_CFG_TBL_END_IDX; i++)
    {
        configHandlersReg[i] = &dv1_drv_config_table_handler_reg;
    }
    readAddr = startAddr;

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_memset(&configReg, 0, sizeof(configReg)));
        paraLen = (imgParaLen & (uint32)0x0FFFFFu);
        flags   = (imgParaLen & (uint32)0xF00000u);
        if(NULL != configHandlersReg[index])
        {
            while((RT_ERR_OK == retVal) && ((uint32)0u < paraLen))
            {
                dataLen = ((paraLen > (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH) ? (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH : paraLen);

                /* the readLen should include padding len if exist */
                readLen = ((dataLen + (uint32)DV1_CONFIG_BLOCK_ALIGNMENT_LENGTH - (uint32)1u) & (uint32)0xFFF0u);
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
                if(((uint32)DV1_MAXIMUM_BLOCK_NUMBER < blockIdx) || (DV1_FLASH_ZONE_SIZE_8MB < readAddr))
                {
                    retVal = -RT_ERR_FAILED;
                    continue;
                }
                readAddr += (readLen + (uint32)DV1_CONFIG_BLOCK_HASH_LENGTH);
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

DYNAMIC_SCOPE RtkApiRet dv1_drv_config_call_handler_eth(uint32 unit, uint32 index, uint32 imgParaLen, uint32 startAddr, void *fp, uint32 configPort)
{
    RtkApiRet                   retVal                               = RT_ERR_OK;
    uint32                      dataLen                              = (uint32)0u;
    uint32                      readLen                              = (uint32)0u;
    uint8                       dataBuf[DV1_CONFIG_BLOCK_MAX_LENGTH] = {(uint8)0u};
    uint32                      blockIdx                             = (uint32)0u;
    uint32                      paraLen                              = (uint32)0u;
    uint32                      readAddr                             = (uint32)0u;
    uint32                      flags                                = (uint32)0u;
    uint32                      i                                    = (uint32)0u;
    dv1_ConfigEth_t             configEth;
    static dv1_ConfigHandlerEth configHandlersEth[DV1_CFG_END_IDX] =
        {
            [DV1_CFG_25UA_IDX]            = &dv1_drv_config_25ua_opfsm_handler_eth,
            [DV1_CFG_OPFSM_IDX]           = &dv1_drv_config_25ua_opfsm_handler_eth,
            [DV1_CFG_BR_PATCH_1_IDX]      = &dv1_drv_config_brpatch_handler_eth,
            [DV1_CFG_BR_PATCH_2_IDX]      = &dv1_drv_config_brpatch_handler_eth,
            [DV1_CFG_SGMII_PATCH_IDX]     = &dv1_drv_config_sgmii_handler_eth,
            [DV1_CFG_FE_PATCH_IDX]        = &dv1_drv_config_fe_handler_eth,
            [DV1_CFG_MAC_CONF_IDX]        = &dv1_drv_config_mac_handler_eth,
            [DV1_CFG_SOMEIP_IDX]          = NULL,
            [DV1_CFG_RESERVED_1]          = NULL,
            [DV1_CFG_RTPG_PATCH_1_IDX]    = &dv1_drv_config_rtpg_handler_eth,
            [DV1_CFG_RTPG_PATCH_2_IDX]    = &dv1_drv_config_rtpg_handler_eth,
            [DV1_CFG_STACKING_IDX]        = &dv1_drv_config_extphy_patch_handler_eth,
            [DV1_CFG_PCIE_IDX]            = NULL,
            [DV1_CFG_FWCFG_BYTE_IDX]      = &dv1_drv_config_fwcfg_byte_handler_eth,
            [DV1_CFG_REG_FIELD_WR_IDX]    = &dv1_drv_config_reg_field_write_handler_eth,
            [DV1_CFG_MKA_IDX]             = &dv1_drv_config_mka_handler_eth,
            [DV1_CFG_ROT_SW_TABLE]        = &dv1_drv_config_rot_sw_handler_eth,
            [DV1_CFG_ROT_SW_INTF_TABLE]   = &dv1_drv_config_rot_sw_intf_handler_eth,
            [DV1_CFG_MACPHY_0_CONFIG_IDX] = &dv1_drv_config_macphy_handler_eth,
            [DV1_CFG_MACPHY_1_CONFIG_IDX] = &dv1_drv_config_macphy_handler_eth,
            [DV1_CFG_PTP_IDX]             = &dv1_drv_config_ptp_handler_eth,
            [DV1_CFG_QCC_PRE_TBL_IDX]     = NULL,
            [DV1_CFG_5KR_PATCH_IDX]       = &dv1_drv_config_5gkr_patch_handler_eth,
            [DV1_CFG_RESERVED_2]          = NULL,
        };

    for(i = DV1_CFG_TBL_BASE_IDX; i <= DV1_CFG_TBL_END_IDX; i++)
    {
        configHandlersEth[i] = &dv1_drv_config_table_handler_eth;
    }
    readAddr = startAddr;

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_memset(&configEth, 0, sizeof(configEth)));
        paraLen = (imgParaLen & (uint32)0x0FFFFFu);
        flags   = (imgParaLen & (uint32)0xF00000u);
        if(NULL != configHandlersEth[index])
        {
            while((RT_ERR_OK == retVal) && ((uint32)0u < paraLen))
            {
                if(((uint32)DV1_MAXIMUM_BLOCK_NUMBER < blockIdx) || (DV1_FLASH_ZONE_SIZE_8MB < readAddr))
                {
                    retVal = -RT_ERR_FAILED;
                    continue;
                }
                dataLen = ((paraLen > (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH) ? (uint32)DV1_CONFIG_BLOCK_MAX_LENGTH : paraLen);

                /* the readLen should include padding len if exist */
                readLen = ((dataLen + (uint32)DV1_CONFIG_BLOCK_ALIGNMENT_LENGTH - (uint32)1u) & (uint32)0xFFF0u);
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

                readAddr += (readLen + (uint32)DV1_CONFIG_BLOCK_HASH_LENGTH);
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
DYNAMIC_SCOPE RtkApiRet dv1_drv_config_cfg_patch(uint32 unit, const int8 *fileName, dv1_UpdateConfig_t updateConfig, uint8 mode)
{
    uint32    startAddr  = (uint32)0u;
    uint32    paraLen    = (uint32)0u;
    uint8     ethernetEn = (uint8)0u;
    void     *fp         = NULL;
    RtkApiRet retVal     = RT_ERR_OK;

    ethernetEn = (mode & (uint8)0x40u) >> DV1_CONFIG_ETH_MODE_BIT;

    do
    {
        if((uint32)DV1_CFG_END_IDX > updateConfig.index)
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
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_call_handler_reg(unit, updateConfig.index, updateConfig.imgParaLen, startAddr, fp));
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_call_handler_eth(unit, updateConfig.index, updateConfig.imgParaLen, startAddr, fp, updateConfig.configPort));
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
RtkApiRet dv1_drv_config_parse_efuse_cfg_header(uint32 unit, uint8 efuStartAddr[], uint8 efuParaLen[])
{
    RtkApiRet retVal                  = RT_ERR_OK;
    uint16    hdrLen                  = (uint16)0u;
    uint16    i                       = (uint16)0u;
    uint8     hdrBuf[DV1_EFU_END_IDX] = {(uint8)0u};
    uint32    j                       = (uint32)0u;
    uint8     efuValid                = (uint8)0u;
    uint16    efuParaExist            = (uint16)0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_memset(hdrBuf, 0x0, DV1_EFU_END_IDX));

        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, DV1_OTP_CFG_START_ADDR, &efuValid, (uint32)1u));
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, DV1_OTP_CFG_START_ADDR + 1u, (uint8 *)&efuParaExist, (uint32)2u));

        efuValid &= (uint8)0xC0u;

        if((uint8)DV1_EFU_IDENTIFY == efuValid)
        {
            hdrLen = (uint16)0u;
            for(i = (uint16)0u; i < (uint16)DV1_EFU_END_IDX; i++)
            {
                if((uint16)0u != (efuParaExist & ((uint16)0x1 << i)))
                {
                    hdrLen += (uint16)1u;
                }
            }

            CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, DV1_OTP_CFG_START_ADDR + 3u, hdrBuf, hdrLen));

            /* initialize start address and data length */
            j = (uint32)0u;
            for(i = (uint16)0u; i < (uint16)DV1_EFU_END_IDX; i++)
            {
                if((uint16)0u == i)
                {
                    efuStartAddr[i] = (uint8)(DV1_OTP_CFG_START_ADDR + 3u + hdrLen);
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

            dv1_extraAreaAddr = efuStartAddr[DV1_EFU_END_IDX - 1u] + efuParaLen[DV1_EFU_END_IDX - 1u];
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
RtkApiRet dv1_drv_config_efuse_cfg_ext_search(uint32 unit, uint8 *ext_start_addr, uint8 *endofOTP)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint8     searchBuf[2] = {(uint8)0u};

    *endofOTP = (uint8)0u;

    while(((uint8)DV1_OTP_EXT_CFG_END > dv1_extraAreaAddr) && ((uint8)0u == *endofOTP))
    {
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, dv1_extraAreaAddr, searchBuf, (uint32)1u));

        if(((uint8)0xFFu == searchBuf[0]) || ((uint8)DV1_EFU_END_IDX <= searchBuf[0]))
        {
            *endofOTP = (uint8)1u;
        }
        else
        {
            if((uint8)DV1_EFU_MAC_CONF_IDX == searchBuf[0])
            {
                if((uint8)DV1_OTP_EXT_CFG_END <= (dv1_extraAreaAddr + (uint8)DV1_CONFIG_MAC_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                dv1_extraAreaAddr += ((uint8)DV1_CONFIG_MAC_ENTRY_LEN + (uint8)1u);
            }
            else if((uint8)DV1_EFU_SGMII_PATCH_IDX == searchBuf[0])
            {
                if((uint8)DV1_OTP_EXT_CFG_END <= (dv1_extraAreaAddr + (uint8)DV1_CONFIG_SGMII_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                dv1_extraAreaAddr += ((uint8)DV1_CONFIG_SGMII_ENTRY_LEN + (uint8)1u);
            }
            else if((uint8)DV1_EFU_FWCFG_BYTE_IDX == searchBuf[0])
            {
                if((uint8)DV1_OTP_EXT_CFG_END <= (dv1_extraAreaAddr + (uint8)DV1_CONFIG_FWCFG_BYTE_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                dv1_extraAreaAddr += ((uint8)DV1_CONFIG_FWCFG_BYTE_LEN + (uint8)1u);
            }
            else
            {
                if((uint8)DV1_OTP_EXT_CFG_END <= (dv1_extraAreaAddr + (uint8)DV1_CONFIG_OCP_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                    ;
                }
                dv1_extraAreaAddr += (DV1_CONFIG_OCP_ENTRY_LEN + (uint8)1u);
            }
        }
        *ext_start_addr = dv1_extraAreaAddr;
    }

    return retVal;
}

RtkApiRet dv1_drv_config_eth_write_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 value)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regVal;

    do
    {
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        regVal = regValRx[0];
        /* Register field set */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_set(regAddr, lsp, len, value, &regVal));
        /* Set write command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
        /* Send write command */
        CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
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
RtkApiRet dv1_drv_config_eth_packet_assmeble(uint8 regCmd, uint32 regAddr, uint32 regVal, uint8 lastPacket)
{
    RtkApiRet             retVal                                                       = RT_ERR_OK;
    static uint8          regAddrEth[RTK_ETHERACCESS_MAX_BURST - 1u][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    static uint32         regvalEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint32)0u};
    static uint8          regCmdEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint8)0u};
    static uint32         regCnt                                                       = (uint32)0u;
    uint32                rxPktLen                                                     = (uint32)0u;
    uint8                 rxPkt[DV1_MAX_PKT_LEN]                                       = {(uint8)0u};
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

                CHK_FUN_RET(retVal, dv1_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regvalEth, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DV1_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)DV1_MAX_PKT_LEN;
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

                CHK_FUN_RET(retVal, dv1_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regvalEth, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DV1_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)DV1_MAX_PKT_LEN;
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
RtkApiRet dv1_drv_config_eth_tx_packet_assmeble_rx_decode(uint8 regCmd, uint32 regAddr, uint32 *regVal, uint8 lastPacket)
{
    RtkApiRet             retVal                                                       = RT_ERR_OK;
    static uint8          regAddrEth[RTK_ETHERACCESS_MAX_BURST - 1u][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    static uint8          regCmdEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint8)0u};
    static uint32         regCnt                                                       = (uint32)0u;
    uint32                rxPktLen                                                     = (uint32)0u;
    uint8                 rxPkt[DV1_MAX_PKT_LEN]                                       = {(uint8)0u};
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

                CHK_FUN_RET(retVal, dv1_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regVal, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DV1_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)DV1_MAX_PKT_LEN;
                CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
                CHK_FUN_RET(retVal, dv1_eth_acc_check_burst_rw(regCnt, regVal, rxPkt));

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

                CHK_FUN_RET(retVal, dv1_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regVal, &txPkt));
                CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, (uint32)DV1_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
                /* Clear Rx packet buffer */
                rxPktLen = (uint32)DV1_MAX_PKT_LEN;
                CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
                CHK_FUN_RET(retVal, dv1_eth_acc_check_burst_rw(regCnt, regVal, rxPkt));

                regCnt = (uint32)0u;
            }
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dv1_drv_config_polling_status(uint32 unit, dv1_PollingReg_t pollingReg)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    timeOutCnt                               = 0u;
    uint32    regVal                                   = 0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    mask                                     = 0u;
    uint32    i                                        = 0u;
    uint8     breakFlag                                = 0u;
#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("dv1_drv_config_polling_status 0x%x lsp %d len %d num 0x%x\n", pollingReg.address, pollingReg.lsp, pollingReg.len, pollingReg.num);
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
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, pollingReg.address, regValRx, 0u));
                /* Send read command */
                CHK_FUN_CONTINUE(retVal, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, pollingReg.address, regValRx, 1u));
                regVal = regValRx[0];
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, dv1_reg_read(unit, pollingReg.address, &regVal));
            }
            (void)rtk_usecond_delay(1u);

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
    rtlglue_printf("dv1_drv_config_polling_status regVal 0x%x ret %d\n", regVal, retVal);
#endif
    return retVal;
}
