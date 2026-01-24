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

#include "rtl907xd_v1_api.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_asicdrv_port.h"
#include "rtl907xd_v1_regField_list.h"
#include "rtl907xd_v1_asicdrv_config.h"
#include "rtl907xd_v1_asicdrv.h"
#include "rtl907xd_v1_api_interface.h"
#include "rtl907xd_v1_asicdrv_serdes.h"

#define DV1_MAX_SEQUENCEMODE_OFFSET (0x7u)
#define DV1_MAX_PHASE_OFFSET        (0x7u)

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] mask
 * @retval
 */
RtkApiRet dv1_drv_port_iso_mask_set(uint32 unit, uint32 port, uint32 mask)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = dv1_reg_field_write(unit, DV1_PORT0_PORT_ISOLATION_CONTROL + (port * PERPORT_REG_OFFSET), DV1_PORT0_PORT_ISOLATION_CONTROL_TISO_LSP, DV1_PORT0_PORT_ISOLATION_CONTROL_TISO_LEN, mask);

    return ret;
}

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] pMask
 * @retval
 */
RtkApiRet dv1_drv_port_iso_mask_get(uint32 unit, uint32 port, uint32 *pMask)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = dv1_reg_field_read(unit, DV1_PORT0_PORT_ISOLATION_CONTROL + (port * PERPORT_REG_OFFSET), DV1_PORT0_PORT_ISOLATION_CONTROL_TISO_LSP, DV1_PORT0_PORT_ISOLATION_CONTROL_TISO_LEN, pMask);

    return ret;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] delay
 * @retval
 */
RtkApiRet dv1_drv_port_rgmii_delay_cell_set(uint32 unit, dv1_RtkPort port, uint8 delay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port) || ((uint32)INTF_HW_PORT8 == port))
    {
        ret = dv1_reg_field_write(unit, (DV1_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET)), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LEN, delay);
    }
    else
    {
        ret = -(RtkApiRet)RT_ERR_PORT_ID;
    }

    return ret;
}

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] pDelay
 * @retval
 */
RtkApiRet dv1_drv_port_rgmii_delay_cell_get(uint32 unit, dv1_RtkPort port, uint8 *pDelay)
{
    RtkApiRet ret   = RT_ERR_OK;
    uint32    delay = 0u;

    if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port) || ((uint32)INTF_HW_PORT8 == port))
    {
        ret     = dv1_reg_field_read(unit, (DV1_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET)), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LEN, &delay);
        *pDelay = (uint8)(delay & 0x000000FFu);
    }
    else
    {
        ret = -(RtkApiRet)RT_ERR_PORT_ID;
    }

    return ret;
}

static RtkApiRet dv1_drv_port_rgmii_addll_set_p8(uint32 unit, RtkPortSpeed_e speed, uint32 rxdly)
{
    RtkApiRet ret;
    do
    {
        /* The following parameter should be define after ASIC verified */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LEN, (uint32)rxdly));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_FBD_SEL_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_FBD_SEL_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_LPF_SEL_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_LPF_SEL_ADDLL1_LEN, 3u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_FORCE_SEL_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_FORCE_SEL_ADDLL1_LEN, 0x2Eu));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_SEL_DLL_RXC_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_SEL_DLL_RXC_ADDLL1_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LEN, 1u));

        if(EM_PORT_SPEED_1000M == speed)
        {

            CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LEN, 1u));
        }
        else
        {

            CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LEN, 1u));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dv1_drv_port_rgmii_addll_set_p3_p7(uint32 unit, RtkPortSpeed_e speed, uint32 rxdly)
{
    RtkApiRet ret;
    do
    {
        /* The following parameter should be define after ASIC verified */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LEN, (uint32)rxdly));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_FBD_SEL_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_FBD_SEL_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_LPF_SEL_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_LPF_SEL_ADDLL0_LEN, 3u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_FORCE_SEL_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_FORCE_SEL_ADDLL0_LEN, 0x2Eu));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_SEL_DLL_RXC_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_SEL_DLL_RXC_ADDLL0_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LEN, 1u));
        if(EM_PORT_SPEED_1000M == speed)
        {
            CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LEN, 1u));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LEN, 1u));
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] speed
 * @param[in] delay
 * @retval
 */
RtkApiRet dv1_drv_port_rgmii_addll_set(uint32 unit, dv1_RtkPort port, RtkPortSpeed_e speed, uint8 delay)
{
    uint32    rxdly;
    RtkApiRet ret;

    do
    {
        rxdly = (delay == 0u) ? (uint32)3u : ((uint32)delay - 1u);

        if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port))
        {
            CHK_FUN_CONTINUE(ret, dv1_drv_port_rgmii_addll_set_p3_p7(unit, speed, rxdly));
        }
        else if((uint32)INTF_HW_PORT8 == port)
        {
            CHK_FUN_CONTINUE(ret, dv1_drv_port_rgmii_addll_set_p8(unit, speed, rxdly));
        }
        else
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] pDelay
 * @retval
 */
RtkApiRet dv1_drv_port_rgmii_addll_get(uint32 unit, dv1_RtkPort port, uint8 *pDelay)  // TBD
{
    uint32    rxdly = 0u;
    RtkApiRet ret   = RT_ERR_OK;

    do
    {
        if((uint32)INTF_HW_PORT8 == port)
        {
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_ADDLL_1_CTRL, DV1_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LSP, DV1_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LEN, &rxdly));
        }
        else if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port))
        {
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_ADDLL_0_CTRL, DV1_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LSP, DV1_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LEN, &rxdly));
        }
        else
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        *pDelay = (0x3u == rxdly) ? 0u : ((uint8)((rxdly & 0x3u) + 1u));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief A function used  to disable rx_en
 * @param[in] unit
 * @param[in] portMask
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval RT_ERR_FAILED
 */
RtkApiRet dv1_drv_port_disable_rx_en(uint32 unit, uint32 portMask)
{
    uint8     idx = 0u;
    RtkApiRet ret = RT_ERR_OK;

    for(idx = DV1_RTK_SYS_PORT_1ST; idx <= DV1_RTK_SYS_PORT_ID_MAX; idx++)
    {
        if(0u != (portMask & ((uint32)1u << idx)))
        {
            /* disable rx */
            CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_PORT0_L2_MISC + ((uint32)idx * PERPORT_REG_OFFSET)), DV1_PORT0_L2_MISC_ENRX_LSP, DV1_PORT0_L2_MISC_ENRX_LEN, 0));
        }
    }

    return ret;
}

/**
 * @brief A function used to disable rx_en
 * @param[in] unit
 * @param[in] portMask
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval RT_ERR_FAILED
 */
RtkApiRet dv1_drv_port_eth_disable_rx_en(uint32 unit, uint32 portMask)
{
    uint8     idx                                      = 0u;
    RtkApiRet ret                                      = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {0u};
    uint32    regAddr                                  = 0u;

    do
    {
        for(idx = DV1_RTK_SYS_PORT_1ST; idx <= DV1_RTK_SYS_PORT_ID_MAX; idx++)
        {
            if(0u != (portMask & ((uint32)1u << idx)))
            {
                regAddr = (DV1_PORT0_L2_MISC + ((uint32)idx * PERPORT_REG_OFFSET));

                /* Set read command */
                CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
            }
        }

        /* Send read command */
        CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_tx_packet_assmeble_rx_decode(DV1_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));

        for(idx = DV1_RTK_SYS_PORT_1ST; idx <= DV1_RTK_SYS_PORT_ID_MAX; idx++)
        {
            if(0u != (portMask & ((uint32)1u << idx)))
            {
                regAddr = (DV1_PORT0_L2_MISC + ((uint32)idx * PERPORT_REG_OFFSET));

                /* Set write command to disable rx_en*/
                CHK_FUN_CONTINUE(ret, dv1_reg_field_set(regAddr, 1u, 1u, 0u, &regValRx[idx]));
                CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regValRx[idx], 0u));
            }
        }

        CHK_FUN_CONTINUE(ret, dv1_drv_config_eth_packet_assmeble(DV1_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regValRx[idx], 1u));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To convert the port ability struct to register value
 * @param[in] pMacAbility      Port link ability
 *                              The pointer of port ability configuration
 * @param[out] retVal           Register value
 *                              This vairable is used to get the mapping bit filed from pMacAbility parameter
 * @retval None
 */
RtkApiRet dv1_drv_port_covert_port_ability_to_reg(uint32 *regVal, PortMacAbility_t *pMacAbility)
{
    RtkApiRet ret = RT_ERR_OK;

    *regVal |= ((0u != pMacAbility->forcemode) ? ((uint32)0x1 << 0u) : 0u);
    *regVal |= ((0u != pMacAbility->link) ? ((uint32)0x1 << 1u) : 0u);
    *regVal |= ((0u != pMacAbility->nway) ? ((uint32)0x1 << 2u) : 0u);
    *regVal |= ((0u != pMacAbility->txpause) ? ((uint32)0x1 << 8u) : 0u);
    *regVal |= ((0u != pMacAbility->rxpause) ? ((uint32)0x1 << 9u) : 0u);

    return ret;
}

/**
 * @brief To get the serdes interface mode configuration
 * @param[in] unit                      Stacking unit
 * @param[in] ifType                    Interface type
 *                                      This value is get from register which is used to identify serdes mode or note
 * @param[in] portNum                   Port Number
 *                                      This value is used to indenify the target port for this API.
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
static RtkApiRet dv1_drv_port_sub_serdes_mode_get(uint32 unit, uint32 ifType, uint32 portNum, RtkModeExt_e *mode)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sdsMode = 0u;

    do
    {
        if((portNum >= 4u) && (portNum <= 8u))
        {
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_SDS_CTRL_REG_0 + (portNum * PERPORT_REG_OFFSET), DV1_PORT4_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DV1_PORT4_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, &sdsMode));
        }
        else if((portNum >= 9u) && (portNum <= 12u))
        {
            CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_WTG_CTRL_REG_0 + (portNum * PERPORT_REG_OFFSET), DV1_PORT9_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DV1_PORT9_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, &sdsMode));
        }
        else
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if((uint32)COMBO_INTF_SGMII_HISGMII == ifType)
        {
            if(DV1_SDS_MODE_HSG == sdsMode)
            {
                *mode = EM_MODE_EXT_HISGMII;
            }
            else if(DV1_SDS_MODE_SG == sdsMode)
            {
                *mode = EM_MODE_EXT_SGMII;
            }
            else
            {
                *mode = EM_MODE_EXT_RESERVED;
                ret   = -RT_ERR_INPUT;
            }
        }
        else if((uint32)COMBO_INTF_XGMII_USXG == ifType)
        {
            if(DV1_SDS_MODE_USXG == sdsMode)
            {
                *mode = EM_MODE_EXT_USXGMII;
            }
            else if((DV1_SDS_MODE_KR_XFI_5G == sdsMode) || (DV1_SDS_MODE_KR_XFI_10G == sdsMode))
            {
                CHK_FUN_CONTINUE(ret, dv1_drv_sds_chk_xfi_or_kr(unit, portNum, mode));
            }
            else
            {
                *mode = EM_MODE_EXT_RESERVED;
                ret   = -RT_ERR_INPUT;
            }
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
            ret   = -RT_ERR_INPUT;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port01_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT1 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SPI_MACPHY:
                *mode = EM_MODE_EXT_MACPHY;
                break;
            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port02_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT2 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SPI_MACPHY:
                *mode = EM_MODE_EXT_MACPHY;
                break;
            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port03_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT3 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case(uint32)COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case(uint32)COMBO_INTF_T1PHY_MII:

                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case(uint32)COMBO_INTF_XMII_RGMII:
                *mode = EM_MODE_EXT_RGMII;
                break;
            case(uint32)COMBO_INTF_XMII_MII:

                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT3 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, &regVal));

                if(regVal == 1u)
                {
                    *mode = EM_MODE_EXT_MII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_MII_MAC;
                }
                break;
            case(uint32)COMBO_INTF_XMII_RMII:

                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT3 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, &regVal));

                if(regVal == 1u)
                {
                    *mode = EM_MODE_EXT_RMII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_RMII_MAC;
                }
                break;
            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port04_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT4 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT4, mode));
                break;
            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port05_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT5 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT5, mode));
                break;
            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port06_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT6 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT6, mode));
                break;
            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port07_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT7 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case(uint32)COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_FAST_ETHERNET;
                break;

            case(uint32)COMBO_INTF_XMII_RGMII:
                *mode = EM_MODE_EXT_RGMII;
                break;

            case(uint32)COMBO_INTF_XMII_MII:
                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT7 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, &regVal));

                if(regVal == 1u)
                {
                    *mode = EM_MODE_EXT_MII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_MII_MAC;
                }
                break;

            case(uint32)COMBO_INTF_XMII_RMII:
                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT7 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, &regVal));

                if(regVal == 1u)
                {
                    *mode = EM_MODE_EXT_RMII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_RMII_MAC;
                }
                break;

            case(uint32)COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT7, mode));
                break;

            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port08_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT8 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_XMII_MII:

                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT8 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, &regVal));

                if(regVal == 1u)
                {
                    *mode = EM_MODE_EXT_MII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_MII_MAC;
                }
                break;

            case COMBO_INTF_XMII_RMII:

                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT8 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, &regVal));

                if(regVal == 1u)
                {
                    *mode = EM_MODE_EXT_RMII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_RMII_MAC;
                }
                break;

            case COMBO_INTF_XMII_RGMII:
                *mode = EM_MODE_EXT_RGMII;
                break;

            case COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT8, mode));
                break;

            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port09_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    pcieLane;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT9 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT9, mode));
                break;

            case COMBO_INTF_PCIE_XGMII:
                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT9_PORT_CTRL1, DV1_PORT9_PORT_CTRL1_RG_PCIE_2LANE_EN_LSP, DV1_PORT9_PORT_CTRL1_RG_PCIE_2LANE_EN_LEN, &pcieLane));

                if(0u == pcieLane)
                {
                    *mode = EM_MODE_EXT_PCIE_5G_MAC_XGMII;
                }
                else
                {
                    *mode = EM_MODE_EXT_PCIE_10G_MAC_XGMII;
                }
                break;

            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port10_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT10 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT10, mode));
                break;

            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port11_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    pcieLane;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT11 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT11, mode));
                break;

            case COMBO_INTF_PCIE_XGMII:
                CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT11_PORT_CTRL1, DV1_PORT11_PORT_CTRL1_RG_PCIE_2LANE_EN_LSP, DV1_PORT11_PORT_CTRL1_RG_PCIE_2LANE_EN_LEN, &pcieLane));

                if(0u == pcieLane)
                {
                    *mode = EM_MODE_EXT_PCIE_5G_MAC_XGMII;
                }
                else
                {
                    *mode = EM_MODE_EXT_PCIE_10G_MAC_XGMII;
                }
                break;

            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to dv1_reg_read().
 */
RtkApiRet dv1_drv_port_sub_port12_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, DV1_PORT0_PORT_CTRL0 + (INTF_HW_PORT12 * PERPORT_REG_OFFSET), DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DV1_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dv1_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT12, mode));
                break;

            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set ETH linking status with anto-nego mode.
 * @param[in] unit                      Stacking unit
 * @param[in] pEthAbility               ETH link ability \n
 *                                      This parameter include link speed/duplex and link status which to be set. \n
 *                                      The supported ability set: 10M Full/10M Half/100M Full/100M Half.
 * @retval RT_ERR_OK                    To execute this function correctly
 * @retval Others                       Please refer to dv1_reg_read() and read_write()
 */
RtkApiRet dv1_drv_port_eth_an_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility)
{
    uint32    phyIdx = 0xBu;
    uint32    regVal = 0u;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        if(0u != (pEthAbility->autoNegoCapability & AN_10M_SPEED_HALF_DUPLEX))
        {
            regVal |= 0x0001u;
        }

        if(0u != (pEthAbility->autoNegoCapability & AN_10M_SPEED_FULL_DUPLEX))
        {
            regVal |= 0x0002u;
        }

        if(0u != (pEthAbility->autoNegoCapability & AN_100M_SPEED_HALF_DUPLEX))
        {
            regVal |= 0x0004u;
        }

        if(0u != (pEthAbility->autoNegoCapability & AN_100M_SPEED_FULL_DUPLEX))
        {
            regVal |= 0x0008u;
        }

        /* bit[8:5]: speed & duplex */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (((phyIdx << 16u) + 0xA408u) << 1u), 5u, 4u, regVal));

        /* bit9: Restart auto-nego, bit12: Auto-Nego enable/disable */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 12u, 1u, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 9u, 1u, 1u));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set ETH linking status.
 * @param[in] unit                      Stacking unit
 * @param[out] pEthAbility               ETH link ability
 *                                      This parameter include link speed/duplex and link status which to be set.
 * @retval RT_ERR_OK                    To execute this function correctly
 * @retval Others                       Please refer to dv1_reg_read() and read_write()
 */
RtkApiRet dv1_drv_port_eth_force_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility)
{
    uint32    phyIdx = 0xBu;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        /* bit6: speed selection msb bit, bit16: speed selection lsb bit */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 6u, 1u, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 13u, 1u, (pEthAbility->speed >> 1u)));
        /* bit8: duplex */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 8u, 1u, (pEthAbility->duplex >> 1u)));

        /* bit12: Auto-Nego enable/disable */
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 12u, 1u, 0u));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Input error check for function dv1_port_xmii_ssc_set.
 * @param[in] userPort               Port id (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END)
 *                                   The port id indicates which port mode is configured.
 * @param[in] isEnable               Spread signal controller enable status. \n
 *                                   Gets spread signal controller state, 0 = enabled, 1 = disabled.
 * @param[in] sequenceModeOffset     Phase offset in sequence mode. \n
 *                                   Gets phase offset in sequence mode(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[in] phaseOffset            SSC controller output phase offset. \n
 *                                   Gets SSC controller output phase offset(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @retval RT_ERR_OK                 To execute this function correctly
 * @retval Others                    Please refer to dv1_reg_read() and read_write()
 */
RtkApiRet dv1_drv_port_xmii_ssc_err_chk(RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        if(0u == DV1_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dv1_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT3) && (port != INTF_HW_PORT7) && (port != INTF_HW_PORT8))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if((ENABLED != isEnable) && (DISABLED != isEnable))
        {
            retVal = -(RtkApiRet)RT_ERR_ENABLE;
            continue;
        }

        if(DV1_MAX_SEQUENCEMODE_OFFSET < sequenceModeOffset)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        if(DV1_MAX_PHASE_OFFSET < phaseOffset)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}