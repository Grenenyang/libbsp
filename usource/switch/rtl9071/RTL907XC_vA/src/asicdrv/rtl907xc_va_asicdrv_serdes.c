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

#include "rtkas_common_types.h"
#include "rtl907xc_va_api_serdes.h"
#include "rtl907xc_va_asicdrv_serdes.h"
#include "rtl907xc_va_asicdrv_port.h"

uint32 cva_g_rtkTimerSerdesMdioChannel = RTK_TIMER_SYSTEM_SERDES_MDIO_CHANNEL;

static RtkApiRet cva_drv_sds_is_wtg_serdes_mode(uint32 comboIntf, uint32 sdsMode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(((CVA_COMBO_SGMII_HISGMII == comboIntf) && ((CVA_SDS_MODE_SGMII == sdsMode) || (CVA_SDS_MODE_HISGMII == sdsMode))) || ((CVA_COMBO_USXGMII == comboIntf) || ((CVA_SDS_MODE_USXGMII == sdsMode) || (CVA_SDS_MODE_5G_KR == sdsMode))))
    {
        /* do nothing */
        ;
    }
    else
    {
        ret = -RT_ERR_SDS_MODE;
    }
    return ret;
}

static RtkApiRet cva_drv_sds_is_serdes_mode(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret               = RT_ERR_OK;
    uint32    comboIntf         = 0u;
    uint32    sdsMode           = 0u;
    uint32    usxCtrl0UsxAddr[] = CVA_USX_CTRL_0_USX_ADDR;
    RtkPort   sysPort           = cva_g_userPort2sysPort[userPort];

    do
    {
        if(CVA_RTK_USER_PORT_7TH == userPort)
        {
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL12, CVA_GLOBAL_MAC_INTERFACE_INTERNAL12_SDS_MODE0_SG0_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL12_SDS_MODE0_SG0_LEN, &sdsMode));
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_0, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P8_FESGRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_0_P8_FESGRG_CB_CFG_LEN, &comboIntf));
            if((CVA_PURE_SGMII != comboIntf) || (CVA_SDS_MODE_SGMII != sdsMode))
            {
                ret = -RT_ERR_SDS_MODE;
            }
        }
        else
        {
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, usxCtrl0UsxAddr[sysPort], CVA_SDS_MODE_LSP, CVA_SDS_MODE_LEN, &sdsMode));

            if(CVA_RTK_USER_PORT_9TH == userPort)
            {
                CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LEN, &comboIntf));
            }
            else if(CVA_RTK_USER_PORT_10TH == userPort)
            {
                CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LEN, &comboIntf));
            }
            else
            {
                CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P12_T1SG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P12_T1SG_CB_CFG_LEN, &comboIntf));
            }

            CHK_FUN_CONTINUE(ret, cva_drv_sds_is_wtg_serdes_mode(comboIntf, sdsMode));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_is_serdes_port(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = cva_drv_sds_chk_port_range(unit, userPort);

    if(RT_ERR_OK == ret)
    {
        ret = cva_drv_sds_is_serdes_mode(unit, userPort);
    }

    return ret;
}

RtkApiRet cva_drv_sds_chk_port_range(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    if((CVA_RTK_USER_PORT_7TH != userPort) && (CVA_RTK_USER_PORT_9TH != userPort) && (CVA_RTK_USER_PORT_10TH != userPort) && (CVA_RTK_USER_PORT_11TH != userPort))
    {
        ret = -RT_ERR_PORT_ID;
    }
    return ret;
}

static RtkApiRet cva_drv_sds_wtg_speed_get(uint32 unit, uint32 sdsSpeed, uint32 *portSpeed)
{
    RtkApiRet ret = RT_ERR_OK;

    if(CVA_SDS_SPEED_5000M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_5000M;
    }
    else if(CVA_SDS_SPEED_2500M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_2500M;
    }
    else if(CVA_SDS_SPEED_1000M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_1000M;
    }
    else if(CVA_SDS_SPEED_100M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_100M;
    }
    else if(CVA_SDS_SPEED_10M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_10M;
    }
    else
    {
        ret = -RT_ERR_PHY_SPEED;
    }

    return ret;
}

/**
 * @brief A function used to get SERDES PHY speed
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sysPort                             Switch port (CVA_RTK_SYS_PORT_8TH,  CVA_RTK_SYS_PORT_10TH to RTK_SYS_PORT_12TH)  \n
 *                                                System port number of the switch
 * @param[out] portSpeed                          Port speed of SEDES mode (CVA_SDS_SPEED_10M to CVA_SDS_SPEED_5000M)  \n
 *                                                Pointer of read back PHY speed
 * @retval RT_ERR_OK                              API gets value of PHY speed Successfully.
 * @retval -RT_ERR_PHY_SPEED                      The interface obtains the incorrect port speed.
 */
RtkApiRet cva_drv_sds_port_speed_get(uint32 unit, RtkPort sysPort, uint32 *portSpeed)
{
    RtkApiRet ret               = RT_ERR_OK;
    RtkPort   userPort          = cva_g_sysPort2userPort[sysPort];
    uint32    usxCtrl1UsxAddr[] = CVA_USX_CTRL_1_USX_ADDR;
    uint32    regVal            = 0u;
    uint32    sdsSpeed          = 0u;

    do
    {
        /* check is serdes port */
        CHK_FUN_CONTINUE(ret, cva_drv_sds_is_serdes_port(unit, userPort));

        if(CVA_RTK_SYS_PORT_7TH == sysPort)
        {
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS_SPEED_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS_SPEED_LEN, &sdsSpeed));

            if(CVA_SDS_SPEED_1000M == sdsSpeed)
            {
                *portSpeed = (uint32)EM_PORT_SPEED_1000M;
            }
            else if(CVA_SDS_SPEED_100M == sdsSpeed)
            {
                *portSpeed = (uint32)EM_PORT_SPEED_100M;
            }
            else if(CVA_SDS_SPEED_10M == sdsSpeed)
            {
                *portSpeed = (uint32)EM_PORT_SPEED_10M;
            }
            else
            {
                ret = -RT_ERR_PHY_SPEED;
            }
        }
        else
        {
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, usxCtrl1UsxAddr[sysPort], &regVal));
            sdsSpeed = regVal & CVA_SDS_SPEED_MASK;
            CHK_FUN_CONTINUE(ret, cva_drv_sds_wtg_speed_get(unit, sdsSpeed, portSpeed));
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_p8_init(uint32 unit)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*boot sequence*/
        /*Set PA33*/
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));

        /*PA09*/
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(100u));

        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_HARDWARE_MISC, CVA_HARDWARE_MISC_SG0_RST_LSP, CVA_HARDWARE_MISC_SG0_RST_LEN, CVA_NORMAL));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_p8_init_eth(void)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /*boot sequence*/
        /*Set PA33*/
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_write_field(CVA_GLOBAL_MAC_INTERFACE_INTERNAL13, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));

        /*PA09*/
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_write_field(CVA_GLOBAL_MAC_INTERFACE_INTERNAL13, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LSP, CVA_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(100u));

        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_write_field(CVA_HARDWARE_MISC, CVA_HARDWARE_MISC_SG0_RST_LSP, CVA_HARDWARE_MISC_SG0_RST_LEN, CVA_NORMAL));

    } while(0u == 1u);

    return ret;
}
RtkApiRet cva_drv_sds_rst_otpv3_stk_config(void)
{
    RtkApiRet ret  = RT_ERR_OK;
    uint32    unit = CVA_UNIT;

    do
    {
        /* config P10 to rgmii mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LEN, CVA_RGMII_MODE));
        /* config P11 to rgmii mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LEN, CVA_RGMII_MODE));
        /* delay 10us to wait for the tx queue to empty*/
        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));
        /* config P10 to reserve mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LEN, CVA_RESERVE_MODE));
        /* config P11 to reserve mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LEN, CVA_RESERVE_MODE));
        /* config P10 serdes to off mode */
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_USX_CTRL_0_US0, CVA_SDS_OFF_MODE));
        /* config P11 serdes to off mode */
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_USX_CTRL_0_US1, CVA_SDS_OFF_MODE));
        /* reset P10 to default configuration */
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_PORT10_PROPERTY_CONFIGURE, CVA_P10_MAC_FORCE_LINK_DOWN));
        /* reset P11 to default configuration */
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_PORT11_PROPERTY_CONFIGURE, CVA_P11_MAC_FORCE_LINK_DOWN));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_eth_rst_otpv3_stk_config(void)
{
    RtkApiRet ret                                      = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {0u};

    do
    {
        /* Set read command */
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_tx_packet_assmeble_rx_decode(CVA_ETHERACCESS_REGISTER_RW_MAC_READ, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, regValRx, 0u));
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_tx_packet_assmeble_rx_decode(CVA_ETHERACCESS_REGISTER_RW_MAC_READ, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, regValRx, 1u));

        /* config P10 to rgmii mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LEN, CVA_RGMII_MODE, &regValRx[0]));
        /* config P11 to rgmii mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LEN, CVA_RGMII_MODE, &regValRx[0]));
        /* Set write command*/
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, regValRx[0], 0u));
        /* send config mac_intf_combo_port_config_1 to rgmii mode*/
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, regValRx[0], 1u));
        /* delay 10us to wait for the tx queue to empty*/
        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));
        /* config P10 to reserve mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LEN, CVA_RESERVE_MODE, &regValRx[0]));
        /* config P11 to reserve mode */
        CHK_FUN_CONTINUE(ret, cva_reg_field_set(CVA_MAC_INTF_COMBO_PORT_CONFIG_1, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LSP, CVA_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LEN, CVA_RESERVE_MODE, &regValRx[0]));
        /* send config mac_intf_combo_port_config_1 to reserve mode*/
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_MAC_INTF_COMBO_PORT_CONFIG_1, regValRx[0], 0u));
        /* send config P10 serdes to off mode */
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_USX_CTRL_0_US0, CVA_SDS_OFF_MODE, 0u));
        /* send config P11 serdes to off mode */
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_USX_CTRL_1_US0, CVA_SDS_OFF_MODE, 0u));
        /* send config P10 to default configuration*/
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_PORT10_PROPERTY_CONFIGURE, CVA_P10_MAC_FORCE_LINK_DOWN, 0u));
        /* sen reset P11 to default configuration */
        CHK_FUN_CONTINUE(ret, cva_drv_config_eth_packet_assmeble(CVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, CVA_PORT11_PROPERTY_CONFIGURE, CVA_P11_MAC_FORCE_LINK_DOWN, 1u));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_pcs_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint32    tryTimes           = 0u;
    uint32    accessFlag         = 0u;
    uint32    regVal             = 0u;
    uint32    i                  = 0u;
    uint32    sdsDirectAddr[]    = CVA_SDS_IN_ADDR;
    uint32    accessAddr[]       = CVA_SDS_CON;
    uint32    dataReadChannel[]  = CVA_DATA_READ_CHANNEL;
    uint32    dataWriteChannel[] = CVA_DATA_WRITE_CHANNEL;

    do
    {
        /*read*/
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsDirectAddr[sysPort], (page << 5u) | reg));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, accessAddr[sysPort], 0x1u));
        tryTimes   = 0u;
        accessFlag = 0u;
        if(CVA_S0_CON != accessAddr[sysPort])
        {
            do
            {
                CHK_FUN_CONTINUE(ret, cva_reg_read(unit, accessAddr[sysPort], &accessFlag));
                tryTimes++;
                if(tryTimes > cva_g_rtkTimerSerdesMdioChannel)
                {
                    ret = -RT_ERR_BUSYWAIT_TIMEOUT;
                    break;
                }
                CHK_FUN_CONTINUE(ret, rtk_usecond_delay(1u));
            } while(accessFlag == 0x1u);
        }
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, dataReadChannel[sysPort], &regVal));
        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }
        /*write*/
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsDirectAddr[sysPort], (page << 5u) | reg));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, dataWriteChannel[sysPort], regVal | (value << startBit)));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, accessAddr[sysPort], 0x3u));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_pcs_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet ret               = RT_ERR_OK;
    uint32    tryTimes          = 0u;
    uint32    accessFlag        = 0u;
    uint32    sdsDirectAddr[]   = CVA_SDS_IN_ADDR;
    uint32    accessAddr[]      = CVA_SDS_CON;
    uint32    dataReadChannel[] = CVA_DATA_READ_CHANNEL;

    do
    {
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsDirectAddr[sysPort], (page << 5u) | reg));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, accessAddr[sysPort], 0x1u));
        tryTimes   = 0u;
        accessFlag = 0u;
        do
        {
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, accessAddr[sysPort], &accessFlag));
            tryTimes++;
            if(tryTimes > cva_g_rtkTimerSerdesMdioChannel)
            {
                ret = -RT_ERR_BUSYWAIT_TIMEOUT;
                break;
            }
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(1u));
        } while(accessFlag == 0x1u);
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, dataReadChannel[sysPort], value));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_aphy_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet ret                 = RT_ERR_OK;
    uint32    i                   = 0u;
    uint32    regVal              = 0u;
    uint32    sdsMdioAddr[]       = CVA_SDS_MDIO_ADDR;
    uint32    sdsMdioContrl[]     = CVA_SDS_MDIO_CON;
    uint32    sdsMdioDataOutput[] = CVA_SDS_MDIO_DATA_OUTPUT;
    uint32    sdsMdioDataInput[]  = CVA_SDS_MDIO_DATA_INPUT;
    do
    {
        /*read*/
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsMdioAddr[sysPort], reg));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsMdioContrl[sysPort], 0u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, sdsMdioDataOutput[sysPort], &regVal));
        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }
        /*write*/
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsMdioAddr[sysPort], reg));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsMdioDataInput[sysPort], regVal | (value << startBit)));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsMdioContrl[sysPort], 1u));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cva_drv_sds_aphy_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet ret                 = RT_ERR_OK;
    uint32    sdsMdioAddr[]       = CVA_SDS_MDIO_ADDR;
    uint32    sdsMdioContrl[]     = CVA_SDS_MDIO_CON;
    uint32    sdsMdioDataOutput[] = CVA_SDS_MDIO_DATA_OUTPUT;
    do
    {
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsMdioAddr[sysPort], reg));
        CHK_FUN_CONTINUE(ret, cva_reg_write(unit, sdsMdioContrl[sysPort], 0u));
        CHK_FUN_CONTINUE(ret, cva_reg_read(unit, sdsMdioDataOutput[sysPort], value));

    } while(0u == 1u);

    return ret;
}
