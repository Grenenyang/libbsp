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
#include "rtl907xd_v1_api_serdes.h"
#include "rtl907xd_v1_asicdrv_serdes.h"

uint32 dv1_g_wrapSDSWorkAround = DV1_WORKAROUND_NOT_READY;

static RtkApiRet dv1_drv_sds_wrap_sds_cfg_bug_workaround(uint32 unit, uint32 accessMode)
{
    RtkApiRet ret                                      = RT_ERR_OK;
    uint32    i                                        = 0u;
    uint32    sdsMode                                  = 0u;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {(uint32)0u};
    uint32    regVal;

    do
    {
        /* wrap_sds bug workaround */
        if(DV1_IS_ETH_ACCESS == accessMode)
        {
            for(i = DV1_WRAP_SDS_START_PORT; i < DV1_WRAP_SDS_END_PORT; i++)
            {
                /* Set read command */
                CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_PORT0_SDS_CTRL_REG_0 + (i * PERPORT_REG_OFFSET), regValRx, 0u));
                /* Send read command */
                CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, DV1_PORT0_SDS_CTRL_REG_0 + (i * PERPORT_REG_OFFSET), regValRx, 1u));
                regVal = regValRx[0];
                /* Get SDS mode */
                CHK_FUN_RET(ret, dv1_reg_field_get(DV1_PORT0_SDS_CTRL_REG_0 + (i * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, &sdsMode, regVal));

                if(DV1_SDS_OFF_MODE == sdsMode)
                {
                    CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_write_field(DV1_PORT0_SDS_CTRL_REG_0 + (i * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, DV1_SDS_MODE_RESERVE));
                }
            }
        }
        else
        {
            for(i = DV1_WRAP_SDS_START_PORT; i < DV1_WRAP_SDS_END_PORT; i++)
            {
                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_SDS_CTRL_REG_0 + (i * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, &sdsMode));

                if(DV1_SDS_OFF_MODE == sdsMode)
                {
                    CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_PORT0_SDS_CTRL_REG_0 + (i * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, DV1_SDS_MODE_RESERVE));
                }
            }
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_drv_sds_wrap_serdes_init(uint32 unit, RtkPort sysPort)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*boot sequence*/
        /*Set PA33*/
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA33_EN_LSP, DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA33_EN_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));

        /*PA09*/
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA09_EN_LSP, DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA09_EN_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(100u));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_HARDWARE_MISC, DV1_HARDWARE_MISC_SG0_RST_LSP, DV1_HARDWARE_MISC_SG0_RST_LEN + (sysPort - DV1_SDS_START_PORT_OFFSET), DISABLE));

        if(DV1_WORKAROUND_NOT_READY == dv1_g_wrapSDSWorkAround)
        {
            CHK_FUN_CONTINUE(ret, dv1_drv_sds_wrap_sds_cfg_bug_workaround(unit, DV1_IS_NOT_ETH_ACCESS));
            dv1_g_wrapSDSWorkAround = DV1_WORKAROUND_READY;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_drv_sds_wrap_serdes_init_eth(uint32 unit, RtkPort sysPort)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /*boot sequence*/
        /*Set PA33*/
        CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_write_field(DV1_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA33_EN_LSP, DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA33_EN_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));

        /*PA09*/
        CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_write_field(DV1_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA09_EN_LSP, DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_PA09_EN_LEN, ENABLE));

        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(100u));

        CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_write_field(DV1_HARDWARE_MISC, DV1_HARDWARE_MISC_SG0_RST_LSP, DV1_HARDWARE_MISC_SG0_RST_LEN + (sysPort - DV1_SDS_START_PORT_OFFSET), DISABLE));

        if(DV1_WORKAROUND_NOT_READY == dv1_g_wrapSDSWorkAround)
        {
            CHK_FUN_CONTINUE(ret, dv1_drv_sds_wrap_sds_cfg_bug_workaround(unit, DV1_IS_ETH_ACCESS));
            dv1_g_wrapSDSWorkAround = DV1_WORKAROUND_READY;
        }

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dv1_drv_sds_polling_timeout(uint32 unit, uint32 reg, uint32 us)
{
    RtkApiRet ret        = RT_ERR_OK;
    uint32    accessFlag = 0u;
    uint32    timeout    = 0u;
    do
    {
        /* polling at least 5ms */
        while((0u == accessFlag) && (timeout < us))
        {
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, reg, DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LSP, DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LEN, &accessFlag));
            CHK_FUN_CONTINUE(ret, g_fp_udelay(1u));
            timeout++;
        }

        if(timeout >= us)
        {
            ret = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
    } while(0u == 1u);
    return ret;
}

RtkApiRet dv1_drv_sds_pcs_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet ret             = RT_ERR_OK;
    uint32    readAction      = 0u;
    uint32    sdsDirectAddr[] = DV1_SDS_INDIRECT_ACCESS_PCS_CTRL;
    uint32    dataChannel[]   = DV1_PCS_DATA_CHANNEL;

    do
    {
        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* set read action */
            readAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP));
            readAction |= ((DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP) | ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_9TH == sysPort) || (DV1_RTK_SYS_PORT_11TH == sysPort))
        {
            /* set read action */
            readAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            readAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP);
        }
        else if((DV1_RTK_SYS_PORT_10TH == sysPort) || (DV1_RTK_SYS_PORT_12TH == sysPort))
        {
            /* set read action */
            readAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | ((reg + 0x800u) << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            readAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP);
        }
        else
        {
            /* for Misra-c violation */
            ;
        }

        /* set read action to register */
        CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, sdsDirectAddr[sysPort], readAction));
        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_polling_timeout(unit, sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));
        /* read data*/
        CHK_FUN_CONTINUE(ret, dv1_reg_read(unit, dataChannel[sysPort], value));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_drv_sds_pcs_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet ret             = RT_ERR_OK;
    uint32    regVal          = 0u;
    uint32    i               = 0u;
    uint32    writeAction     = 0u;
    uint32    sdsDirectAddr[] = DV1_SDS_INDIRECT_ACCESS_PCS_CTRL;
    uint32    dataChannel[]   = DV1_PCS_DATA_CHANNEL;

    do
    {
        /*read*/
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_pcs_direct_get(unit, sysPort, page, reg, &regVal));

        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }

        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* set write action */
            writeAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP));
            writeAction |= ((DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP) | ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP));
        }
        else if((DV1_RTK_SYS_PORT_9TH == sysPort) || (DV1_RTK_SYS_PORT_11TH == sysPort))
        {
            writeAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            writeAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP);
        }
        else if((DV1_RTK_SYS_PORT_10TH == sysPort) || (DV1_RTK_SYS_PORT_12TH == sysPort))
        {
            writeAction = ((page << DV1_INDIRECT_ACCESS_SDS_INDEX_LSP) | ((reg + 0x800u) << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));
            writeAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP);
        }
        else
        {
            /* for Misra-c violation */
            ;
        }
        /* set write data */
        CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, dataChannel[sysPort], regVal | (value << startBit)));
        /* set write action to register */
        CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, sdsDirectAddr[sysPort], writeAction));
        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_polling_timeout(unit, sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_drv_sds_aphy_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet ret             = RT_ERR_OK;
    uint32    readAction      = 0u;
    uint32    sdsDirectAddr[] = DV1_SDS_INDIRECT_ACCESS_DPHY_CTRL;
    uint32    dataChannel[]   = DV1_DPHY_DATA_CHANNEL;
    do
    {
        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            readAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            readAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP);
        }
        else if((DV1_RTK_SYS_PORT_9TH <= sysPort) && (DV1_RTK_SYS_PORT_12TH >= sysPort))
        {
            readAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_READ_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            readAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP);
        }
        else
        {
            /* for Misra-c violation */
            ;
        }

        /* set read action to register */
        CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, sdsDirectAddr[sysPort], readAction));
        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_polling_timeout(unit, sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));
        /* read data*/
        CHK_FUN_CONTINUE(ret, dv1_reg_read(unit, dataChannel[sysPort], value));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_drv_sds_aphy_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet ret             = RT_ERR_OK;
    uint32    i               = 0u;
    uint32    regVal          = 0u;
    uint32    writeAction     = 0u;
    uint32    sdsDirectAddr[] = DV1_SDS_INDIRECT_ACCESS_DPHY_CTRL;
    uint32    dataChannel[]   = DV1_DPHY_DATA_CHANNEL;

    do
    {
        /*read*/
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_aphy_direct_get(unit, sysPort, page, reg, &regVal));

        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }

        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            writeAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            writeAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP);
        }
        else if((DV1_RTK_SYS_PORT_9TH <= sysPort) && (DV1_RTK_SYS_PORT_12TH >= sysPort))
        {
            writeAction = ((reg << DV1_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DV1_WRITE_DATA << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));
            writeAction |= ((uint32)ENABLE << DV1_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP);
        }
        else
        {
            /* for Misra-c violation */
            ;
        }

        /* set write data */
        CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, dataChannel[sysPort], regVal | (value << startBit)));
        /* set write action to register */
        CHK_FUN_CONTINUE(ret, dv1_reg_write(unit, sdsDirectAddr[sysPort], writeAction));
        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_polling_timeout(unit, sdsDirectAddr[sysPort], DV1_POLLING_TIMEOUT_UNIT_MICRO_SEC));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dv1_drv_sds_is_wtg_serdes_mode(uint32 comboIntf, uint32 sdsMode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(((DV1_INTF_SG_HSG == comboIntf) && ((DV1_SDS_MODE_SG == sdsMode) || (DV1_SDS_MODE_HSG == sdsMode))) || ((DV1_INTF_USXG == comboIntf) && ((DV1_SDS_MODE_USXG == sdsMode) || (DV1_SDS_MODE_KR_XFI_5G == sdsMode) || (DV1_SDS_MODE_KR_XFI_10G == sdsMode))))
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

static RtkApiRet dv1_drv_sds_is_serdes_mode(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret       = RT_ERR_OK;
    uint32    comboIntf = 0u;
    uint32    sdsMode   = 0u;
    RtkPort   sysPort   = dv1_g_userPort2sysPort[userPort];
    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &comboIntf));

        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* get SerDes mode */
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DV1_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, &sdsMode));
            if((DV1_INTF_SG_HSG != comboIntf) || ((DV1_SDS_MODE_SG != sdsMode) && (DV1_SDS_MODE_HSG != sdsMode)))
            {
                ret = -RT_ERR_SDS_MODE;
            }
        }
        else
        {
            /* get SerDes mode */
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DV1_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DV1_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, &sdsMode));

            CHK_FUN_CONTINUE(ret, dv1_drv_sds_is_wtg_serdes_mode(comboIntf, sdsMode));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dv1_drv_sds_is_serdes_port(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = dv1_drv_sds_chk_port_range(userPort);

    if(RT_ERR_OK == ret)
    {
        ret = dv1_drv_sds_is_serdes_mode(unit, userPort);
    }

    return ret;
}

RtkApiRet dv1_drv_sds_chk_port_range(RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    if((DV1_RTK_USER_PORT_4TH != userPort) && (DV1_RTK_USER_PORT_5TH != userPort) && (DV1_RTK_USER_PORT_6TH != userPort) && (DV1_RTK_USER_PORT_7TH != userPort) && (DV1_RTK_USER_PORT_8TH != userPort) && (DV1_RTK_USER_PORT_9TH != userPort) && (DV1_RTK_USER_PORT_10TH != userPort) && (DV1_RTK_USER_PORT_11TH != userPort) && (DV1_RTK_USER_PORT_12TH != userPort))
    {
        ret = -RT_ERR_PORT_ID;
    }

    return ret;
}

RtkApiRet dv1_drv_sds_aphy_covert_port(uint32 portType, RtkPort *sysPort)
{
    RtkApiRet ret = -RT_ERR_FAILED;

    if(DV1_SDS_APHY_TYPE == portType)
    {
        /* Convert APHY user port 10 to port 9 */
        if(DV1_RTK_USER_PORT_10TH == *sysPort)
        {
            *sysPort = DV1_RTK_USER_PORT_9TH;
        }
        /* Convert APHY user port 12  to port 11 */
        if(DV1_RTK_USER_PORT_12TH == *sysPort)
        {
            *sysPort = DV1_RTK_USER_PORT_11TH;
        }

        ret = RT_ERR_OK;
    }

    return ret;
}

static RtkApiRet dv1_drv_sds_speed_get(uint32 unit, uint32 rgSdsAbility, uint32 *portSpeed)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    sdsSpeed = 0u;

    sdsSpeed = rgSdsAbility & DV1_SDS_SPEED_MASK;

    if(DV1_SDS_SPEED_10G == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_10000M;
    }
    else if(DV1_SDS_SPEED_5G == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_5000M;
    }
    else if(DV1_SDS_SPEED_2500M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_2500M;
    }
    else if(DV1_SDS_SPEED_1000M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_1000M;
    }
    else if(DV1_SDS_SPEED_100M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_100M;
    }
    else if(DV1_SDS_SPEED_10M == sdsSpeed)
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
 * @param[in] sysPort                             Switch port (RTK_SYS_PORT_4H to DV1_RTK_SYS_PORT_12TH)  \n
 *                                                System port number of the switch
 * @param[out] portSpeed                          Port speed of SEDES mode (DV1_SDS_SPEED_10M to DV1_SDS_SPEED_10G)  \n
 *                                                Pointer of read back PHY speed
 * @retval RT_ERR_OK                              API gets value of PHY speed Successfully.
 * @retval -RT_ERR_PHY_SPEED                      The interface obtains the incorrect port speed.
 */
RtkApiRet dv1_drv_sds_port_speed_get(uint32 unit, RtkPort sysPort, uint32 *portSpeed)
{
    RtkApiRet ret          = RT_ERR_OK;
    RtkPort   userPort     = dv1_g_sysPort2userPort[sysPort];
    uint32    rgSdsAbility = 0u;

    do
    {
        /* check is serdes port */
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_is_serdes_port(unit, userPort));

        if((DV1_RTK_SYS_PORT_4TH <= sysPort) && (DV1_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* get WRAP SERDES ability */
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_SDS_CTRL_REG_2 + (DV1_SDS_PER_PORT_OFFSET * sysPort), DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LSP, DV1_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LEN, &rgSdsAbility));
        }
        else
        {
            /* get WTG SERDES ability */
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_WTG_CTRL_REG_1 + (DV1_SDS_PER_PORT_OFFSET * sysPort), DV1_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DV1_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, &rgSdsAbility));
        }

        /* get SERDES speed */
        CHK_FUN_CONTINUE(ret, dv1_drv_sds_speed_get(unit, rgSdsAbility, portSpeed));
    } while(0u == 1u);

    return ret;
}
RtkApiRet dv1_drv_sds_chk_xfi_or_kr(uint32 unit, RtkPort userPort, RtkModeExt_e *result)
{
    RtkApiRet ret     = -RT_ERR_FAILED;
    uint32    sdsMode = 0u;
    uint32    Nway    = 0u;

    do
    {
        if((DV1_RTK_USER_PORT_9TH > userPort) || (DV1_RTK_USER_PORT_12TH < userPort))
        {
            *result = EM_MODE_EXT_RESERVED;
            ret     = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_WTG_CTRL_REG_0 + (userPort * PERPORT_REG_OFFSET), DV1_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DV1_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, &sdsMode));

        CHK_FUN_CONTINUE(ret, dv1_sys_sds_reg_getbits_pcs(unit, userPort, 19u, 0u, 12u, 12u, &Nway));

        if(DV1_SDS_MODE_KR_XFI_10G == sdsMode)
        {
            if((uint32)DISABLE == Nway)
            {
                /* force mode */
                *result = EM_MODE_EXT_10G_XFI;
            }
            else
            {
                /* AN mode */
                *result = EM_MODE_EXT_10G_KR;
            }
        }
        else if(DV1_SDS_MODE_KR_XFI_5G == sdsMode)
        {
            if((uint32)DISABLE == Nway)
            {
                /* force mode */
                *result = EM_MODE_EXT_5G_XFI;
            }
            else
            {
                /* AN mode */
                *result = EM_MODE_EXT_5G_KR;
            }
        }
        else
        {
            *result = EM_MODE_EXT_RESERVED;
        }

    } while(0u == 1u);

    return ret;
}
