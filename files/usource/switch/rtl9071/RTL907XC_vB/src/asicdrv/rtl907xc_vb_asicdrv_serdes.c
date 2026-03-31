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
#include "rtl907xc_vb_api_serdes.h"
#include "rtl907xc_vb_asicdrv_serdes.h"
#include "rtl907xc_vb_asicdrv_port.h"

uint32 cvb_g_rtkTimerSerdesMdioChannel = RTK_TIMER_SYSTEM_SERDES_MDIO_CHANNEL;

static RtkApiRet cvb_drv_sds_is_wtg_serdes_mode(uint32 comboIntf, uint32 sdsMode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(((CVB_COMBO_SGMII_HISGMII == comboIntf) && ((CVB_SDS_MODE_SGMII == sdsMode) || (CVB_SDS_MODE_HISGMII == sdsMode))) || ((CVB_COMBO_USXGMII == comboIntf) || ((CVB_SDS_MODE_USXGMII == sdsMode) || (CVB_SDS_MODE_5G_KR == sdsMode))))
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

static RtkApiRet cvb_drv_sds_is_serdes_mode(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret               = RT_ERR_OK;
    uint32    comboIntf         = 0u;
    uint32    sdsMode           = 0u;
    uint32    usxCtrl0UsxAddr[] = CVB_USX_CTRL_0_USX_ADDR;
    RtkPort   sysPort           = cvb_g_userPort2sysPort[userPort];

    do
    {
        if(CVB_RTK_USER_PORT_7TH == userPort)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL12, CVB_GLOBAL_MAC_INTERFACE_INTERNAL12_SDS_MODE0_SG0_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL12_SDS_MODE0_SG0_LEN, &sdsMode));
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P8_FESGRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P8_FESGRG_CB_CFG_LEN, &comboIntf));
            if((CVB_PURE_SGMII != comboIntf) || (CVB_SDS_MODE_SGMII != sdsMode))
            {
                ret = -RT_ERR_SDS_MODE;
            }
        }
        else
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, usxCtrl0UsxAddr[sysPort], CVB_SDS_MODE_LSP, CVB_SDS_MODE_LEN, &sdsMode));

            if(CVB_RTK_USER_PORT_9TH == userPort)
            {
                CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_1, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LEN, &comboIntf));
            }
            else if(CVB_RTK_USER_PORT_10TH == userPort)
            {
                CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_1, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LEN, &comboIntf));
            }
            else
            {
                CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_1, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P12_T1SG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P12_T1SG_CB_CFG_LEN, &comboIntf));
            }

            CHK_FUN_CONTINUE(ret, cvb_drv_sds_is_wtg_serdes_mode(comboIntf, sdsMode));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_sds_is_serdes_port(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = cvb_drv_sds_chk_port_range(unit, userPort);

    if(RT_ERR_OK == ret)
    {
        ret = cvb_drv_sds_is_serdes_mode(unit, userPort);
    }

    return ret;
}

RtkApiRet cvb_drv_sds_chk_port_range(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    if((CVB_RTK_USER_PORT_7TH != userPort) && (CVB_RTK_USER_PORT_9TH != userPort) && (CVB_RTK_USER_PORT_10TH != userPort) && (CVB_RTK_USER_PORT_11TH != userPort))
    {
        ret = -RT_ERR_PORT_ID;
    }
    return ret;
}

static RtkApiRet cvb_drv_sds_wtg_speed_get(uint32 unit, uint32 sdsSpeed, uint32 *portSpeed)
{
    RtkApiRet ret = RT_ERR_OK;

    if(CVB_SDS_SPEED_5000M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_5000M;
    }
    else if(CVB_SDS_SPEED_2500M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_2500M;
    }
    else if(CVB_SDS_SPEED_1000M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_1000M;
    }
    else if(CVB_SDS_SPEED_100M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_100M;
    }
    else if(CVB_SDS_SPEED_10M == sdsSpeed)
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
 * @param[in] sysPort                             Switch port (CVB_RTK_SYS_PORT_8TH,  CVB_RTK_SYS_PORT_10TH to RTK_SYS_PORT_12TH)  \n
 *                                                System port number of the switch
 * @param[out] portSpeed                          Port speed of SEDES mode (CVB_SDS_SPEED_10M to CVB_SDS_SPEED_5000M)  \n
 *                                                Pointer of read back PHY speed
 * @retval RT_ERR_OK                              API gets value of PHY speed Successfully.
 * @retval -RT_ERR_PHY_SPEED                      The interface obtains the incorrect port speed.
 */
RtkApiRet cvb_drv_sds_port_speed_get(uint32 unit, RtkPort sysPort, uint32 *portSpeed)
{
    RtkApiRet ret               = RT_ERR_OK;
    RtkPort   userPort          = cvb_g_sysPort2userPort[sysPort];
    uint32    usxCtrl1UsxAddr[] = CVB_USX_CTRL_1_USX_ADDR;
    uint32    regVal            = 0u;
    uint32    sdsSpeed          = 0u;

    do
    {
        /* check is serdes port */
        CHK_FUN_CONTINUE(ret, cvb_drv_sds_is_serdes_port(unit, userPort));

        if(CVB_RTK_SYS_PORT_7TH == sysPort)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS_SPEED_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS_SPEED_LEN, &sdsSpeed));

            if(CVB_SDS_SPEED_1000M == sdsSpeed)
            {
                *portSpeed = (uint32)EM_PORT_SPEED_1000M;
            }
            else if(CVB_SDS_SPEED_100M == sdsSpeed)
            {
                *portSpeed = (uint32)EM_PORT_SPEED_100M;
            }
            else if(CVB_SDS_SPEED_10M == sdsSpeed)
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
            CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, usxCtrl1UsxAddr[sysPort], &regVal));
            sdsSpeed = regVal & CVB_SDS_SPEED_MASK;
            CHK_FUN_CONTINUE(ret, cvb_drv_sds_wtg_speed_get(unit, sdsSpeed, portSpeed));
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_sds_p8_init(uint32 unit)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*boot sequence*/
        /*Set PA33*/
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));

        /*PA09*/
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(100u));

        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_HARDWARE_MISC, CVB_HARDWARE_MISC_SG0_RST_LSP, CVB_HARDWARE_MISC_SG0_RST_LEN, CVB_NORMAL));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_sds_p8_init_eth(void)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /*boot sequence*/
        /*Set PA33*/
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_write_field(CVB_GLOBAL_MAC_INTERFACE_INTERNAL13, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA33_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));

        /*PA09*/
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_write_field(CVB_GLOBAL_MAC_INTERFACE_INTERNAL13, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL13_SDS0_PA09_EN_SG0_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(100u));

        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_write_field(CVB_HARDWARE_MISC, CVB_HARDWARE_MISC_SG0_RST_LSP, CVB_HARDWARE_MISC_SG0_RST_LEN, CVB_NORMAL));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_sds_pcs_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint32    tryTimes           = 0u;
    uint32    accessFlag         = 0u;
    uint32    regVal             = 0u;
    uint32    i                  = 0u;
    uint32    sdsDirectAddr[]    = CVB_SDS_IN_ADDR;
    uint32    accessAddr[]       = CVB_SDS_CON;
    uint32    dataReadChannel[]  = CVB_DATA_READ_CHANNEL;
    uint32    dataWriteChannel[] = CVB_DATA_WRITE_CHANNEL;

    do
    {
        /*read*/
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsDirectAddr[sysPort], (page << 5u) | reg));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, accessAddr[sysPort], 0x1u));
        tryTimes   = 0u;
        accessFlag = 0u;
        if(CVB_S0_CON != accessAddr[sysPort])
        {
            do
            {
                CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, accessAddr[sysPort], &accessFlag));
                tryTimes++;
                if(tryTimes > cvb_g_rtkTimerSerdesMdioChannel)
                {
                    ret = -RT_ERR_BUSYWAIT_TIMEOUT;
                    break;
                }
                CHK_FUN_CONTINUE(ret, rtk_usecond_delay(1u));
            } while(accessFlag == 0x1u);
        }
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, dataReadChannel[sysPort], &regVal));
        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }
        /*write*/
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsDirectAddr[sysPort], (page << 5u) | reg));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, dataWriteChannel[sysPort], regVal | (value << startBit)));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, accessAddr[sysPort], 0x3u));
    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_sds_pcs_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet ret               = RT_ERR_OK;
    uint32    tryTimes          = 0u;
    uint32    accessFlag        = 0u;
    uint32    sdsDirectAddr[]   = CVB_SDS_IN_ADDR;
    uint32    accessAddr[]      = CVB_SDS_CON;
    uint32    dataReadChannel[] = CVB_DATA_READ_CHANNEL;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsDirectAddr[sysPort], (page << 5u) | reg));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, accessAddr[sysPort], 0x1u));
        tryTimes   = 0u;
        accessFlag = 0u;
        do
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, accessAddr[sysPort], &accessFlag));
            tryTimes++;
            if(tryTimes > cvb_g_rtkTimerSerdesMdioChannel)
            {
                ret = -RT_ERR_BUSYWAIT_TIMEOUT;
                break;
            }
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(1u));
        } while(accessFlag == 0x1u);
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, dataReadChannel[sysPort], value));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_sds_aphy_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet ret                 = RT_ERR_OK;
    uint32    i                   = 0u;
    uint32    regVal              = 0u;
    uint32    sdsMdioAddr[]       = CVB_SDS_MDIO_ADDR;
    uint32    sdsMdioContrl[]     = CVB_SDS_MDIO_CON;
    uint32    sdsMdioDataOutput[] = CVB_SDS_MDIO_DATA_OUTPUT;
    uint32    sdsMdioDataInput[]  = CVB_SDS_MDIO_DATA_INPUT;
    do
    {
        /*read*/
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsMdioAddr[sysPort], reg));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsMdioContrl[sysPort], 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, sdsMdioDataOutput[sysPort], &regVal));
        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }
        /*write*/
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsMdioAddr[sysPort], reg));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsMdioDataInput[sysPort], regVal | (value << startBit)));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsMdioContrl[sysPort], 1u));

    } while(0u == 1u);

    return ret;
}

RtkApiRet cvb_drv_sds_aphy_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet ret                 = RT_ERR_OK;
    uint32    sdsMdioAddr[]       = CVB_SDS_MDIO_ADDR;
    uint32    sdsMdioContrl[]     = CVB_SDS_MDIO_CON;
    uint32    sdsMdioDataOutput[] = CVB_SDS_MDIO_DATA_OUTPUT;
    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsMdioAddr[sysPort], reg));
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, sdsMdioContrl[sysPort], 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, sdsMdioDataOutput[sysPort], value));

    } while(0u == 1u);

    return ret;
}
