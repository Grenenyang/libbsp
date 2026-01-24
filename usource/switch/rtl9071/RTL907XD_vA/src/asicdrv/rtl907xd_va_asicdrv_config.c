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
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_otp.h"
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_asicdrv_config.h"
#include "rtl907xd_va_api_esm.h"
#include "rtkas_system_porting.h"
#include "rtl907xd_va_asicdrv_ethernet_access.h"

#define DVA_WAITREADY (0)
/******************************************************************************
 * Global Variables
 ******************************************************************************/
static uint8 dva_extraAreaAddr = 0xFFu;
/******************************************************************************
 * Function Declaration
 ******************************************************************************/
static RtkApiRet dva_drv_config_get_hdr_cfg_info(uint32 unit, void *fp, uint32 *cfgAddr, uint32 *cfgLength, uint8 *fwKeyType);
static RtkApiRet dva_drv_config_get_hdr_cfg_info_eth(void *fp, uint32 *cfgAddr, uint32 *cfgLength, uint8 *fwKeyType);
static RtkApiRet dva_drv_config_check_length(uint32 unit, const uint32 configLen);
static RtkApiRet dva_drv_config_check_length_eth(const uint32 configLen);

/******************************************************************************
 * Functions Definition
 ******************************************************************************/
RtkApiRet dva_drv_config_update_aes_ctr_iv(uint32 unit, void *fp, const uint32 fwKeyType, const uint32 fwLength)
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
                CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_reg_write(unit, i, fwBuf));
                j += 4u;
            }
        }
    } while(0u == 1u);

    return retVal;
}
RtkApiRet dva_drv_config_eth_update_aes_ctr_iv(void *fp, const uint32 fwKeyType, const uint32 fwLength)
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
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_flow_patch_write(fwLength, 16, fwBuf));
        }
    } while(0u == 1u);
    return retVal;
}

RtkApiRet dva_drv_config_get_hdr_fw_info(void *fp, uint32 *fwAddr, uint32 *fwLength, uint8 *fwKeyType)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     fileBuf[9]  = {(uint8)0u};
    uint16    headerIdent = (uint16)0u;
    do
    {
        /* check the validation of this binary file */
        if((int32)0 == rtl_file_read(fp, fileBuf, DVA_FLASH_HEADER_OFFSET, 9u))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        headerIdent = (uint16)fileBuf[0] | ((uint16)fileBuf[1] << 8);

        if((uint16)DVA_CFG_SPI_IDENTIFY != headerIdent)
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

        if(((uint32)0xFFFF0u == *fwLength) || ((uint32)DVA_MAX_FW_LENGTH < *fwLength))
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

static RtkApiRet dva_drv_config_get_hdr_cfg_info(uint32 unit, void *fp, uint32 *cfgAddr, uint32 *cfgLength, uint8 *fwKeyType)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     fileBuf[15] = {(uint8)0u};
    uint16    headerIdent = (uint16)0u;
    do
    {
        /* check the validation of this binary file */
        if((int32)0 == rtl_file_read(fp, fileBuf, DVA_FLASH_HEADER_OFFSET, 15u))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        headerIdent = (uint16)fileBuf[0] | ((uint16)fileBuf[1] << 8);
        if((uint16)DVA_CFG_SPI_IDENTIFY != headerIdent)
        {
#ifdef DEBUG_SYS
            (void)rtlglue_printf("Firmware doesn't exist\n");
#endif
            retVal = -RT_ERR_INPUT;
            continue;
        }
        *cfgLength = ((uint32)fileBuf[12] << 4u) + ((uint32)fileBuf[13] << 12u);
        *cfgAddr   = (uint32)fileBuf[9] | ((uint32)fileBuf[10] << 8u) | ((uint32)fileBuf[11] << 16u);
        *fwKeyType = (fileBuf[2] & (uint8)0xCu) >> 2u;
        if((uint32)0xFFFF0u == *cfgLength)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dva_drv_config_check_length(unit, *cfgLength));
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_CFG_LENGTH_LSP, DVA_CFG_LENGTH_LEN, *cfgLength));
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet dva_drv_config_get_hdr_cfg_info_eth(void *fp, uint32 *cfgAddr, uint32 *cfgLength, uint8 *fwKeyType)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     fileBuf[15] = {(uint8)0u};
    uint16    headerIdent = (uint16)0u;
    do
    {
        /* check the validation of this binary file */
        if((int32)0 == rtl_file_read(fp, fileBuf, DVA_FLASH_HEADER_OFFSET, 15u))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        headerIdent = (uint16)fileBuf[0] | ((uint16)fileBuf[1] << 8);
        if((uint16)DVA_CFG_SPI_IDENTIFY != headerIdent)
        {
#ifdef DEBUG_SYS
            (void)rtlglue_printf("Firmware doesn't exist\n");
#endif
            retVal = -RT_ERR_INPUT;
            continue;
        }
        *cfgLength = ((uint32)fileBuf[12] << 4u) + ((uint32)fileBuf[13] << 12u);
        *cfgAddr   = (uint32)fileBuf[9] | ((uint32)fileBuf[10] << 8u) | ((uint32)fileBuf[11] << 16u);
        *fwKeyType = (fileBuf[2] & (uint8)0xCu) >> 2u;
        if((uint32)0xFFFF0u == *cfgLength)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_config_check_length_eth(*cfgLength));
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_write_field(DVA_CPU_COMMAND_IN_FLASHLESS_MODE_OPCODE, DVA_CFG_LENGTH_LSP, DVA_CFG_LENGTH_LEN, *cfgLength));
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
RtkApiRet dva_drv_config_parse_flash_cfg_header(const int8 *fileName, uint32 spiStartAddr[], uint32 spiParaLen[], uint8 stkIdx)
{
    uint16    i                                   = (uint16)0u;
    uint32    spiValid                            = (uint32)0xFFFFu;
    uint8     configAddrLenBuf[5]                 = {(uint8)0u};
    uint32    configAddr                          = (uint32)0u;
    uint32    configLength                        = (uint32)0u;
    uint8     hdrBuf[DVA_FLASH_CONFIG_HEADER_LEN] = {(uint8)0u};
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
        if((int32)0 == rtl_file_read(fp, configAddrLenBuf, DVA_FLASH_HEADER_OFFSET + 9u + (stkIdx * 5u), 5u))
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

        if((int32)0 == rtl_file_read(fp, hdrBuf, configAddr, DVA_FLASH_CONFIG_HEADER_LEN))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        spiValid = (uint32)hdrBuf[0] | ((uint32)hdrBuf[1] << 8u);

        if((uint32)DVA_CFG_SPI_IDENTIFY != spiValid)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        spiStartAddr[0] = configAddr + (uint32)DVA_FLASH_CONFIG_HEADER_LEN;
        for(i = 0u; i < DVA_CFG_END_IDX; i++)
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
                    spiStartAddr[i] += (((uint32)DVA_CONFIG_BLOCK_ALIGNMENT_LENGTH - ((spiParaLen[i - 1u] & (uint32)0xFFFFF) % (uint32)DVA_CONFIG_BLOCK_ALIGNMENT_LENGTH)) % (uint32)DVA_CONFIG_BLOCK_ALIGNMENT_LENGTH);
                    /* add hash length for each block of previous domain */
                    blockNum = ((((spiParaLen[i - 1u] & (uint32)0xFFFFF) - 1u) / (uint32)DVA_CONFIG_BLOCK_MAX_LENGTH) + (uint32)1u);
                    spiStartAddr[i] += (blockNum * (uint32)DVA_CONFIG_BLOCK_HASH_LENGTH);
                }
            }
            offset += DVA_FLASH_DOMAIN_LEN;
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
RtkApiRet dva_drv_config_parse_efuse_cfg_header(uint32 unit, uint8 efuStartAddr[], uint8 efuParaLen[])
{
    RtkApiRet retVal                  = RT_ERR_OK;
    uint16    hdrLen                  = (uint16)0u;
    uint16    i                       = (uint16)0u;
    uint8     hdrBuf[DVA_EFU_END_IDX] = {(uint8)0u};
    uint32    j                       = (uint32)0u;
    uint8     efuValid                = (uint8)0u;
    uint16    efuParaExist            = (uint16)0u;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(hdrBuf, 0x0, DVA_EFU_END_IDX));

        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, DVA_OTP_CFG_START_ADDR, &efuValid, (uint32)1u));
        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, DVA_OTP_CFG_START_ADDR + 1u, (uint8 *)&efuParaExist, (uint32)2u));

        efuValid &= (uint8)0xC0u;

        if((uint8)DVA_EFU_IDENTIFY == efuValid)
        {
            hdrLen = (uint16)0u;
            for(i = (uint16)0u; i < (uint16)DVA_EFU_END_IDX; i++)
            {
                if((uint16)0u != (efuParaExist & ((uint16)0x1 << i)))
                {
                    hdrLen += (uint16)1u;
                }
            }

            CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, DVA_OTP_CFG_START_ADDR + 3u, hdrBuf, hdrLen));

            /* initialize start address and data length */
            j = (uint32)0u;
            for(i = (uint16)0u; i < (uint16)DVA_EFU_END_IDX; i++)
            {
                if((uint16)0u == i)
                {
                    efuStartAddr[i] = (uint8)(DVA_OTP_CFG_START_ADDR + 3u + hdrLen);
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

            dva_extraAreaAddr = efuStartAddr[DVA_EFU_END_IDX - 1u] + efuParaLen[DVA_EFU_END_IDX - 1u];
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
RtkApiRet dva_drv_config_efuse_cfg_ext_search(uint32 unit, uint8 *ext_start_addr, uint8 *endofOTP)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint8     searchBuf[2] = {(uint8)0u};

    *endofOTP = (uint8)0u;

    while(((uint8)DVA_OTP_EXT_CFG_END > dva_extraAreaAddr) && ((uint8)0u == *endofOTP))
    {
        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, dva_extraAreaAddr, searchBuf, (uint32)1u));

        if(((uint8)0xFFu == searchBuf[0]) || ((uint8)DVA_EFU_END_IDX <= searchBuf[0]))
        {
            *endofOTP = (uint8)1u;
        }
        else
        {
            if((uint8)DVA_EFU_MAC_CONF_IDX == searchBuf[0])
            {
                if((uint8)DVA_OTP_EXT_CFG_END <= (dva_extraAreaAddr + (uint8)DVA_CONFIG_MAC_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                dva_extraAreaAddr += ((uint8)DVA_CONFIG_MAC_ENTRY_LEN + (uint8)1u);
            }
            else if((uint8)DVA_EFU_SGMII_PATCH_IDX == searchBuf[0])
            {
                if((uint8)DVA_OTP_EXT_CFG_END <= (dva_extraAreaAddr + (uint8)DVA_CONFIG_SGMII_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                dva_extraAreaAddr += ((uint8)DVA_CONFIG_SGMII_ENTRY_LEN + (uint8)1u);
            }
            else if((uint8)DVA_EFU_FWCFG_BYTE_IDX == searchBuf[0])
            {
                if((uint8)DVA_OTP_EXT_CFG_END <= (dva_extraAreaAddr + (uint8)DVA_CONFIG_FWCFG_BYTE_LEN))
                {
                    *endofOTP = (uint8)1u;
                }
                dva_extraAreaAddr += ((uint8)DVA_CONFIG_FWCFG_BYTE_LEN + (uint8)1u);
            }
            else
            {
                if((uint8)DVA_OTP_EXT_CFG_END <= (dva_extraAreaAddr + (uint8)DVA_CONFIG_OCP_ENTRY_LEN))
                {
                    *endofOTP = (uint8)1u;
                    ;
                }
                dva_extraAreaAddr += (DVA_CONFIG_OCP_ENTRY_LEN + (uint8)1u);
            }
        }
        *ext_start_addr = dva_extraAreaAddr;
    }

    return retVal;
}

RtkApiRet dva_drv_config_eth_read_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 *value)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regVal;

    do
    {
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        regVal = regValRx[0];
        /* Register field set */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_get(regAddr, lsp, len, value, regVal));
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_config_eth_write_field(uint32 regAddr, uint32 lsp, uint32 len, uint32 value)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regVal;

    do
    {
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        regVal = regValRx[0];
        /* Register field set */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_set(regAddr, lsp, len, value, &regVal));
        /* Set write command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_packet_assmeble(DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
        /* Send write command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_packet_assmeble(DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, 0u, 0u, 1u));
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
RtkApiRet dva_drv_config_eth_packet_assmeble(uint8 regCmd, uint32 regAddr, uint32 regVal, uint8 lastPacket)
{
    RtkApiRet             retVal                                                       = RT_ERR_OK;
    static uint8          regAddrEth[RTK_ETHERACCESS_MAX_BURST - 1u][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    static uint32         regvalEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint32)0u};
    static uint8          regCmdEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint8)0u};
    static uint32         regCnt                                                       = (uint32)0u;
    static uint32         portMask                                                     = 0u;
    uint32                rxPktLen                                                     = (uint32)0u;
    uint8                 rxPkt[DVA_MAX_PKT_LEN]                                       = {(uint8)0u};
    EtherAccessMac_t      mac;
    EtherAccessL2Header_t txPkt;
    uint32                port    = 0u;
    uint8                 sendPkt = 0u;

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
                sendPkt                 = 1u;
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
            if(((regAddr & 0xFF000000u) == 0x4A000000u) && (regAddr >= DVA_INDIRECT_ACCESS_PORT_BASE_ADDRESS))
            {
                port = (regAddr & DVA_INDIRECT_ACCESS_PORT_CHK_MSK) / PERPORT_REG_OFFSET;
                portMask |= (uint32)1u << port;
            }

            if(((uint32)RTK_ETHERACCESS_MAX_BURST - 2u) <= regCnt)
            {
                /* Set dummy reg in last */
                regCmdEth[(regCnt)]     = (uint8)0;
                regAddrEth[(regCnt)][0] = (uint8)0;
                regAddrEth[(regCnt)][1] = (uint8)0;
                regAddrEth[(regCnt)][2] = (uint8)0;
                regvalEth[(regCnt)]     = (uint32)0;
                sendPkt                 = 1u;
            }
        }

        if(1u == sendPkt)
        {
            CHK_FUN_RET(retVal, dva_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regvalEth, portMask, &txPkt));
            CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
            /* Clear Rx packet buffer */
            rxPktLen = (uint32)DVA_MAX_PKT_LEN;
            CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            regCnt   = (uint32)0u;
            portMask = 0u;
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
RtkApiRet dva_drv_config_eth_tx_packet_assmeble_rx_decode(uint8 regCmd, uint32 regAddr, uint32 *regVal, uint8 lastPacket)
{
    RtkApiRet             retVal                                                       = RT_ERR_OK;
    static uint8          regAddrEth[RTK_ETHERACCESS_MAX_BURST - 1u][RTK_REG_ADDR_LEN] = {{(uint8)0u}};
    static uint8          regCmdEth[RTK_ETHERACCESS_MAX_BURST - 1u]                    = {(uint8)0u};
    static uint32         regCnt                                                       = (uint32)0u;
    static uint32         portMask                                                     = 0u;
    uint32                rxPktLen                                                     = (uint32)0u;
    uint8                 rxPkt[DVA_MAX_PKT_LEN]                                       = {(uint8)0u};
    EtherAccessMac_t      mac;
    EtherAccessL2Header_t txPkt;
    uint32                port    = 0u;
    uint8                 sendPkt = 0u;

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
                sendPkt                 = 1u;
            }
        }
        else
        {
            regCmdEth[(regCnt)]   = regCmd;
            regAddrEth[regCnt][0] = (uint8)((regAddr >> 16u) & 0xFFu);
            regAddrEth[regCnt][1] = (uint8)((regAddr >> 8u) & 0xFFu);
            regAddrEth[regCnt][2] = (uint8)((regAddr)&0xFFu);
            regCnt++;
            if(((regAddr & 0xFF000000u) == 0x4A000000u) && (regAddr >= DVA_INDIRECT_ACCESS_PORT_BASE_ADDRESS))
            {
                port     = (regAddr & DVA_INDIRECT_ACCESS_PORT_CHK_MSK) / PERPORT_REG_OFFSET;
                portMask = (uint32)1u << port;
            }

            if((RTK_ETHERACCESS_MAX_BURST - 2u) <= regCnt)
            {
                /* Set dummy reg in last */
                regCmdEth[(regCnt)]     = (uint8)0u;
                regAddrEth[(regCnt)][0] = (uint8)0u;
                regAddrEth[(regCnt)][1] = (uint8)0u;
                regAddrEth[(regCnt)][2] = (uint8)0u;
                regVal[(regCnt)]        = (uint32)0u;
                sendPkt                 = 1u;
            }
        }
        if(1u == sendPkt)
        {
            CHK_FUN_RET(retVal, dva_eth_acc_gen_burst_rw(mac, regCnt, regCmdEth, &regAddrEth[0], regVal, portMask, &txPkt));
            CHK_FUN_RET(retVal, rtk_etherAccess_tx_packet((uint8 *)&txPkt, DVA_RTK_ETHERACCESS_HEADER_LEN + (regCnt * 8u) + 8u));
            /* Clear Rx packet buffer */
            rxPktLen = (uint32)DVA_MAX_PKT_LEN;
            CHK_FUN_RET(retVal, rtk_etherAccess_rx_packet(rxPkt, &rxPktLen));
            CHK_FUN_RET(retVal, dva_eth_acc_check_burst_rw(regCnt, regVal, rxPkt));
            regCnt   = (uint32)0u;
            portMask = 0u;
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_config_polling_status(uint32 unit, dva_PollingReg_t pollingReg)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    timeOutCnt                               = 0u;
    uint32    regVal                                   = 0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    mask                                     = 0u;
    uint32    i                                        = 0u;
    uint8     breakFlag                                = 0u;
#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("dva_drv_config_polling_status 0x%x lsp %d len %d num 0x%x\n", pollingReg.address, pollingReg.lsp, pollingReg.len, pollingReg.num);
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
                dva_esm_event_log_set(DVA_EM_EVENT_FLASHLESS_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
                retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                continue;
            }
            timeOutCnt++;
            if(1u == pollingReg.isEth)
            {
                CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, pollingReg.address, regValRx, 0u));
                /* Send read command */
                CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, pollingReg.address, regValRx, 1u));
                regVal = regValRx[0];
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, pollingReg.address, &regVal));
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
    rtlglue_printf("dva_drv_config_polling_status regVal 0x%x ret %d\n", regVal, retVal);
#endif
    return retVal;
}

static RtkApiRet dva_drv_config_check_length(uint32 unit, const uint32 configLen)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    base   = 0u;
    uint32    end    = 0u;
    do
    {
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, DVA_SRAM_SHARE_START, &base));
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, DVA_SRAM_SHARE_END, &end));
        if((end - base) < configLen)
        {
            retVal = -RT_ERR_INPUT;
        }
    } while(0u == 1u);
    return retVal;
}

static RtkApiRet dva_drv_config_check_length_eth(const uint32 configLen)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    base                                     = 0u;
    uint32    end                                      = 0u;
    do
    {
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, DVA_SRAM_SHARE_START, regValRx, 0u));
        /* Set read command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, DVA_SRAM_SHARE_END, regValRx, 0u));
        /* Send read command */
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, DVA_SRAM_SHARE_END, regValRx, 1u));
        base = regValRx[0];
        end  = regValRx[1];
        if((end - base) < configLen)
        {
            retVal = -RT_ERR_INPUT;
        }
    } while(0u == 1u);
    return retVal;
}

RtkApiRet dva_drv_config_load_config(uint32 unit, const int8 *fileName)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    regVal      = 0u;
    void     *fp          = NULL;
    uint8     fileBuf[4]  = {0u};
    uint8     fwKeyType   = 0u;
    uint32    i           = 0u;
    uint32    configAddr  = 0u;
    uint32    configLen   = 0u;
    uint32    baseAddress = 0u;
    uint32    fileAddr    = 0u;

#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("dva_drv_config_load_config\n");
#endif
    do
    {
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(fp == NULL)
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dva_drv_config_get_hdr_cfg_info(unit, fp, &configAddr, &configLen, &fwKeyType));
        CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, DVA_SRAM_SHARE_START, &baseAddress));
        baseAddress &= 0xFFFFFFu;
        CHK_FUN_CONTINUE(retVal, dva_reg_page_write(unit, 0x01u));
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("start load config in sram\n");
#endif
        fileAddr = configAddr;
        for(i = (uint32)0u; i < configLen; i += (uint32)4u)
        {
            if((int32)0 == rtl_file_read(fp, fileBuf, fileAddr + i, 4u))
            {

                retVal = -RT_ERR_FAILED;
                continue;
            }
            regVal = (uint32)fileBuf[0] | ((uint32)fileBuf[1] << 8) | ((uint32)fileBuf[2] << 16) | ((uint32)fileBuf[3] << 24);
            CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_reg_write(unit, i + baseAddress, regVal));
        }
        CHK_FUN_CONTINUE_RESERVE_ERR(retVal, dva_drv_config_update_aes_ctr_iv(unit, fp, fwKeyType, configLen + baseAddress));
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("load config in sram done\n");
#endif
    } while(0u == 1u);
    rtl_file_close(&fp);
#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("dva_drv_config_load_config retVal %d\n", retVal);
#endif
    return retVal;
}

RtkApiRet dva_drv_config_load_config_eth(uint32 unit, const int8 *fileName)
{
    RtkApiRet retVal                                   = RT_ERR_OK;
    void     *fp                                       = NULL;
    uint8     fwKeyType                                = 0u;
    uint8     data[DVA_RTK_ETHERACCESS_BYTE_PER_WRITE] = {0u};
    uint32    offset                                   = 0u;
    uint32    wLen                                     = 0u;
    uint32    lenLeft                                  = 0u;
    uint32    configAddr                               = 0u;
    uint32    configLen                                = 0u;
    uint32    baseAddress                              = 0u;

#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("dva_drv_config_load_config_eth\n");
#endif
    do
    {
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(fp == NULL)
        {
#ifdef DEBUG_ASIC_CONFIG
            rtlglue_printf("FILE %s open fail!\n", fileName);
#endif
            retVal = -RT_ERR_FAILED;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dva_drv_config_get_hdr_cfg_info_eth(fp, &configAddr, &configLen, &fwKeyType));
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_read_field(DVA_SRAM_SHARE_START, 0u, 32u, &baseAddress));
        baseAddress &= 0xFFFFFFu;
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("start load config in sram baseAddress 0x%x\n", (baseAddress | 0x01000000u));
#endif

        offset = (uint32)0u;
        while((offset < configLen) && (RT_ERR_OK == retVal))
        {
            lenLeft = configLen - offset;
            if(lenLeft > (uint32)DVA_RTK_ETHERACCESS_BYTE_PER_WRITE)
            {
                wLen = (uint32)DVA_RTK_ETHERACCESS_BYTE_PER_WRITE;
            }
            else
            {
                wLen = lenLeft;
            }

            if((int32)0 == rtl_file_read(fp, data, configAddr + offset, wLen))
            {
#ifdef DEBUG_ASIC_CONFIG
                rtlglue_printf("FILE %s read fail!\n", fileName);
#endif
                retVal = -RT_ERR_FAILED;
                continue;
            }
            CHK_FUN_CONTINUE(retVal, dva_eth_acc_flow_patch_write(baseAddress + offset, wLen, data));
            offset = offset + wLen;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
#ifdef DEBUG_ASIC_CONFIG
        rtlglue_printf("load config in sram done\n");
#endif
        CHK_FUN_CONTINUE(retVal, dva_drv_config_eth_update_aes_ctr_iv(fp, fwKeyType, configLen + baseAddress));
    } while(0u == 1u);
    rtl_file_close(&fp);
#ifdef DEBUG_ASIC_CONFIG
    rtlglue_printf("dva_drv_config_load_config_eth retVal %d\n", retVal);
#endif
    return retVal;
}
