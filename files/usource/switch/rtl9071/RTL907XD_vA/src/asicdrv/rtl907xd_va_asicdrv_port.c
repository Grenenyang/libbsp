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

#include "rtl907xd_va_api.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_port.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_asicdrv_config.h"
#include "rtl907xd_va_asicdrv.h"
#include "rtl907xd_va_api_interface.h"
#include "rtl907xd_va_asicdrv_serdes.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_api_interface.h"

#define DVA_PORT_TX_WITH_PKT (0u)
#define DVA_ORIGIN_PHASE     (0u)
#define DVA_INVERT_RXC       (1u)

#define DVA_MAX_SEQUENCEMODE_OFFSET (0x7u)
#define DVA_MAX_PHASE_OFFSET        (0x7u)

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] mask
 * @retval
 */
RtkApiRet dva_drv_port_iso_mask_set(uint32 unit, uint32 port, uint32 mask)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = dva_drv_indirect_reg_field_set(unit, DVA_PORT0_PORT_ISOLATION_CONTROL + (port * PERPORT_REG_OFFSET), DVA_PORT0_PORT_ISOLATION_CONTROL_TISO_LSP, DVA_PORT0_PORT_ISOLATION_CONTROL_TISO_LEN, mask);

    return ret;
}

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] pMask
 * @retval
 */
RtkApiRet dva_drv_port_iso_mask_get(uint32 unit, uint32 port, uint32 *pMask)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_ISOLATION_CONTROL + (port * PERPORT_REG_OFFSET), DVA_PORT0_PORT_ISOLATION_CONTROL_TISO_LSP, DVA_PORT0_PORT_ISOLATION_CONTROL_TISO_LEN, pMask);

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
RtkApiRet dva_drv_port_rgmii_delay_cell_set(uint32 unit, dva_RtkPort port, uint8 delay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port) || ((uint32)INTF_HW_PORT8 == port))
    {
        ret = dva_drv_indirect_reg_field_set(unit, (DVA_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET)), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LEN, delay);
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
RtkApiRet dva_drv_port_rgmii_delay_cell_get(uint32 unit, dva_RtkPort port, uint8 *pDelay)
{
    RtkApiRet ret   = RT_ERR_OK;
    uint32    delay = 0u;

    if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port) || ((uint32)INTF_HW_PORT8 == port))
    {
        ret     = dva_drv_indirect_reg_field_get(unit, (DVA_PORT0_PORT_CTRL0 + (port * PERPORT_REG_OFFSET)), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RGMII_TXDLY_LEN, &delay);
        *pDelay = (uint8)(delay & 0x000000FFu);
    }
    else
    {
        ret = -(RtkApiRet)RT_ERR_PORT_ID;
    }

    return ret;
}

static RtkApiRet dva_drv_port_rgmii_addll_set_p8(uint32 unit, RtkPortSpeed_e speed, uint32 rxdly)
{
    RtkApiRet ret;
    do
    {
        /* The following parameter should be define after ASIC verified */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LEN, (uint32)rxdly));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_FBD_SEL_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_FBD_SEL_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_LPF_SEL_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_LPF_SEL_ADDLL1_LEN, 3u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_FORCE_SEL_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_FORCE_SEL_ADDLL1_LEN, 0x2Eu));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_SEL_DLL_RXC_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_SEL_DLL_RXC_ADDLL1_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LEN, 1u));

        if(EM_PORT_SPEED_1000M == speed)
        {

            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LEN, 1u));
        }
        else
        {

            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LEN, 1u));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_port_rgmii_addll_set_p3_p7(uint32 unit, RtkPortSpeed_e speed, uint32 rxdly)
{
    RtkApiRet ret;
    do
    {
        /* The following parameter should be define after ASIC verified */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LEN, (uint32)rxdly));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_FBD_SEL_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_FBD_SEL_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_LPF_SEL_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_LPF_SEL_ADDLL0_LEN, 3u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_FORCE_SEL_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_FORCE_SEL_ADDLL0_LEN, 0x2Eu));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_SEL_DLL_RXC_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_SEL_DLL_RXC_ADDLL0_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LEN, 1u));
        if(EM_PORT_SPEED_1000M == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LEN, 1u));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LEN, 1u));
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
 * @retval -RT_ERR_PORT_ID The inputed port index is invalid
 * @retval Others Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_rgmii_addll_set(uint32 unit, dva_RtkPort port, RtkPortSpeed_e speed, uint8 delay)
{
    uint32    rxdly;
    RtkApiRet ret;

    do
    {
        rxdly = (delay == 0u) ? (uint32)3u : ((uint32)delay - 1u);

        if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port))
        {
            CHK_FUN_CONTINUE(ret, dva_drv_port_rgmii_addll_set_p3_p7(unit, speed, rxdly));
        }
        else if((uint32)INTF_HW_PORT8 == port)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_port_rgmii_addll_set_p8(unit, speed, rxdly));
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
 * @retval Others Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_rgmii_addll_get(uint32 unit, dva_RtkPort port, uint8 *pDelay)  // TBD
{
    uint32    rxdly = 0u;
    RtkApiRet ret   = RT_ERR_OK;

    do
    {
        if((uint32)INTF_HW_PORT8 == port)
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_ADDLL_1_CTRL, DVA_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LSP, DVA_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LEN, &rxdly));
        }
        else if(((uint32)INTF_HW_PORT3 == port) || ((uint32)INTF_HW_PORT7 == port))
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_ADDLL_0_CTRL, DVA_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LSP, DVA_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LEN, &rxdly));
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
 * @retval Others         Please refer to dva_reg_field_write().
 */
RtkApiRet dva_drv_port_disable_rx_en(uint32 unit, uint32 portMask)
{
    uint8     idx = 0u;
    RtkApiRet ret = RT_ERR_OK;

    for(idx = DVA_RTK_SYS_PORT_1ST; idx <= DVA_RTK_SYS_PORT_ID_MAX; idx++)
    {
        if(0u != (portMask & ((uint32)1u << idx)))
        {
            /* disable rx */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, (DVA_PORT0_L2_MISC + ((uint32)idx * PERPORT_REG_OFFSET)), DVA_PORT0_L2_MISC_ENRX_LSP, DVA_PORT0_L2_MISC_ENRX_LEN, 0));
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
 * @retval Others         Please refer to dva_reg_field_set().
 */
RtkApiRet dva_drv_port_eth_disable_rx_en(uint32 unit, uint32 portMask)
{
    uint8     idx                                      = 0u;
    RtkApiRet ret                                      = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {0u};
    uint32    regAddr                                  = 0u;

    do
    {
        for(idx = DVA_RTK_SYS_PORT_1ST; idx <= DVA_RTK_SYS_PORT_ID_MAX; idx++)
        {
            if(0u != (portMask & ((uint32)1u << idx)))
            {
                regAddr = (DVA_PORT0_L2_MISC + ((uint32)idx * PERPORT_REG_OFFSET));

                /* Set read command */
                CHK_FUN_CONTINUE(ret, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
            }
        }

        /* Send read command */
        CHK_FUN_CONTINUE(ret, dva_drv_config_eth_tx_packet_assmeble_rx_decode(DVA_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));

        for(idx = DVA_RTK_SYS_PORT_1ST; idx <= DVA_RTK_SYS_PORT_ID_MAX; idx++)
        {
            if(0u != (portMask & ((uint32)1u << idx)))
            {
                regAddr = (DVA_PORT0_L2_MISC + ((uint32)idx * PERPORT_REG_OFFSET));

                /* Set write command to disable rx_en*/
                CHK_FUN_CONTINUE(ret, dva_reg_field_set(regAddr, 1u, 1u, 0u, &regValRx[idx]));
                CHK_FUN_CONTINUE(ret, dva_drv_config_eth_packet_assmeble(DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regValRx[idx], 0u));
            }
        }

        CHK_FUN_CONTINUE(ret, dva_drv_config_eth_packet_assmeble(DVA_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regValRx[idx], 1u));
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
RtkApiRet dva_drv_port_covert_port_ability_to_reg(uint32 *regVal, PortMacAbility_t *pMacAbility)
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
static RtkApiRet dva_drv_port_sub_serdes_mode_get(uint32 unit, uint32 ifType, uint32 portNum, RtkModeExt_e *mode)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sdsMode = 0u;

    do
    {
        if((portNum >= 4u) && (portNum <= 8u))
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_SDS_CTRL_REG_0 + (portNum * PERPORT_REG_OFFSET), DVA_PORT4_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DVA_PORT4_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, &sdsMode));
        }
        else if((portNum >= 9u) && (portNum <= 12u))
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_WTG_CTRL_REG_0 + (portNum * PERPORT_REG_OFFSET), DVA_PORT9_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT9_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, &sdsMode));
        }
        else
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if((uint32)COMBO_INTF_SGMII_HISGMII == ifType)
        {
            if(DVA_SDS_MODE_HSG == sdsMode)
            {
                *mode = EM_MODE_EXT_HISGMII;
            }
            else if(DVA_SDS_MODE_SG == sdsMode)
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
            if(DVA_SDS_MODE_USXG == sdsMode)
            {
                *mode = EM_MODE_EXT_USXGMII;
            }
            else if((DVA_SDS_MODE_KR_XFI_5G == sdsMode) || (DVA_SDS_MODE_KR_XFI_10G == sdsMode))
            {
                CHK_FUN_CONTINUE(ret, dva_drv_sds_chk_xfi_or_kr(unit, portNum, mode));
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port01_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT1 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port02_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT2 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port03_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT3 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

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

                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT3 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, &regVal));

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

                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT3 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, &regVal));

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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port04_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT4 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT4, mode));
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port05_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT5 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT5, mode));
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port06_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT6 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_RTPG_T1_PHY;
                break;
            case COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_100BASE_T1_PHY;
                break;
            case COMBO_INTF_SGMII_HISGMII:
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT6, mode));
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port07_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT7 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case(uint32)COMBO_INTF_T1PHY_MII:
                *mode = EM_MODE_EXT_FAST_ETHERNET;
                break;

            case(uint32)COMBO_INTF_T1PHY_GMII:
                *mode = EM_MODE_EXT_GIGA_ETHERNET;
                break;

            case(uint32)COMBO_INTF_XMII_RGMII:
                *mode = EM_MODE_EXT_RGMII;
                break;

            case(uint32)COMBO_INTF_XMII_MII:
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT7 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, &regVal));

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
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT7 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, &regVal));

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
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT7, mode));
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port08_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT8 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_XMII_MII:

                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT8 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, &regVal));

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

                CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT8 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, &regVal));

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
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT8, mode));
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port09_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    pcieLane;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT9 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT9, mode));
                break;

            case COMBO_INTF_PCIE_XGMII:
                CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT9_PORT_CTRL1, DVA_PORT9_PORT_CTRL1_RG_PCIE_2LANE_EN_LSP, DVA_PORT9_PORT_CTRL1_RG_PCIE_2LANE_EN_LEN, &pcieLane));

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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port10_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT10 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT10, mode));
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
 * @retval Others                       Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port11_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    pcieLane;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT11 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT11, mode));
                break;

            case COMBO_INTF_PCIE_XGMII:
                CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT11_PORT_CTRL1, DVA_PORT11_PORT_CTRL1_RG_PCIE_2LANE_EN_LSP, DVA_PORT11_PORT_CTRL1_RG_PCIE_2LANE_EN_LEN, &pcieLane));

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
 * @param[in] unit   Stacking unit
 * @param[out] mode  The interface mode
 *                   To indicate which interface is set currently.
 * @retval RT_ERR_OK Get the mode status successfully.
 * @retval Others    Please refer to dva_reg_field_read().
 */
RtkApiRet dva_drv_port_sub_port12_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_PORT_CTRL0 + (INTF_HW_PORT12 * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &ifType));

        switch(ifType)
        {
            case COMBO_INTF_SGMII_HISGMII:
            case COMBO_INTF_XGMII_USXG:
                CHK_FUN_CONTINUE(ret, dva_drv_port_sub_serdes_mode_get(unit, ifType, INTF_HW_PORT12, mode));
                break;

            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Ethernet anto negotiation ability.
 * @param[in] unit         Stacking unit
 * @param[in] pEthAbility  Eth link ability \n
 *                         This parameter include link speed/duplex and auto-nego setting. \n
 *                         The supported ability set: 10M Full/10M Half/100M Full/100M Half/1000M Full.
 * @retval RT_ERR_OK       To execute this function correctly
 * @retval Others          Please refer to dva_reg_field_write()
 */
RtkApiRet dva_drv_port_eth_an_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility)
{
    uint32    regVal = 0u;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        if(0u != (pEthAbility->autoNegoCapability & AN_1000M_SPEED_FULL_DUPLEX))
        {
            /* set PHY reg9 field my1000f to 1. Advertise PHY is 1000BASE-T full duplex capabile */
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA412u << 1u), 9u, 1u, 1u));
        }

        if(0u != (pEthAbility->autoNegoCapability & AN_10M_SPEED_HALF_DUPLEX))
        {
            /* set PHY reg4 field my10h to 1. Advertise PHY is half duplex 10ASE-T ability */
            regVal |= 0x0001u;
        }

        if(0u != (pEthAbility->autoNegoCapability & AN_10M_SPEED_FULL_DUPLEX))
        {
            /* set PHY reg4 field my10f to 1. Advertise PHY is full duplex 10ASE-T ability */
            regVal |= 0x0002u;
        }

        if(0u != (pEthAbility->autoNegoCapability & AN_100M_SPEED_HALF_DUPLEX))
        {
            /* set PHY reg4 field my100h to 1. Advertise PHY is half duplex 100ASE-T ability */
            regVal |= 0x0004u;
        }

        if(0u != (pEthAbility->autoNegoCapability & AN_100M_SPEED_FULL_DUPLEX))
        {
            /* set PHY reg4 field my100f to 1. Advertise PHY is full duplex 100ASE-T ability */
            regVal |= 0x0008u;
        }
        /* set PHY reg4 to configure 10/100 duplex and speed ability */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA408u << 1u), 5u, 4u, regVal));
        /* set bit12: an_enable to enable */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA400u << 1u), 12u, 1u, 1u));
        /* set bit9: an_restart to restart auto-nego */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA400u << 1u), 9u, 1u, 1u));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set ETH linking status.
 * @param[in] unit            Stacking unit
 * @param[out] pEthAbility    ETH link ability
 *                            This parameter include link speed/duplex and link status which to be set.
 * @retval RT_ERR_OK          To execute this function correctly
 * @retval RT_ERR_PHY_SPEED   The speed parameter is not correct
 * @retval RT_ERR_PHY_DUPLEX  The duplex parameter is not correct or the configuration is 1000M half-duplex.
 * @retval Others            Please refer to dva_reg_field_write()
 */
RtkApiRet dva_drv_port_eth_force_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if((uint32)EM_PORT_SPEED_1000M < pEthAbility->speed)
        {
            ret = -(RT_ERR_PHY_SPEED);
            continue;
        }

        if(((uint32)PORT_HALF_DUPLEX != pEthAbility->duplex) && ((uint32)PORT_FULL_DUPLEX != pEthAbility->duplex))
        {
            ret = -(RT_ERR_PHY_DUPLEX);
            continue;
        }

        if(((uint32)PORT_HALF_DUPLEX == pEthAbility->duplex) && ((uint32)EM_PORT_SPEED_1000M == pEthAbility->speed))
        {
            ret = -(RT_ERR_PHY_DUPLEX);
            continue;
        }

        /**
         *                          bit6         bit13
         * RTK_PORT_SPEED_1000M(4): regspd1 = 1, regspd0 = 0
         * RTK_PORT_SPEED_100M(2):  regspd1 = 0, regspd0 = 1
         * RTK_PORT_SPEED_10M(1):   regspd1 = 0, regspd0 = 0
         */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA400u << 1u), 6u, 1u, (((pEthAbility->speed) >> 2u) & 0x1u)));
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA400u << 1u), 13u, 1u, (((pEthAbility->speed) >> 1u) & 0x1u)));
        /**
         * bit8: regdup
         * PORT_HALF_DUPLEX: regdup = 0
         * PORT_FULL_DUPLEX: regdup = 1
         */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA400u << 1u), 8u, 1u, (pEthAbility->duplex >> 1u)));
        /* set bit12: an_enable to disable */
        CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, (0xBA400u << 1u), 12u, 1u, 0u));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Eth port auto-negotination ability.
 * @param[in] unit                Stacking unit
 * @param[in] mode                Interface port mode (EM_MODE_EXT_FAST_ETHERNET or EM_MODE_EXT_GIGA_ETHERNET)
 * @param[in] phyIdx              The PHY OCP address offset
 * @param[out] autoNegoCapability Eth port auto-negotination ability
 *
 * @retval RT_ERR_OK         To execute this function correctly
 * @retval -RT_ERR_PORT_ID   The designated port is not a valid ethernet port
 * @retval -RT_ERR_PORT_MODE The input mode parameter is invalid
 * @retval Others            Please refer to dva_reg_read()
 */
RtkApiRet dva_drv_port_eth_get_an_ability(uint32 unit, RtkModeExt_e mode, RtkPort userPort, uint16 *autoNegoCapability)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPort   sysPort = 0u;
    uint32    phyIdx  = 0xBu;
    uint32    regVal  = 0u;

    do
    {
        /* get MAC port index and check the port index is valid or not */
        sysPort = dva_g_userPort2sysPort[userPort];
        if(INTF_HW_PORT7 != sysPort)
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }
        if(mode == EM_MODE_EXT_FAST_ETHERNET)
        {
            /* Fast Ethernet */
            /* Get speed & duplex */
            CHK_FUN_CONTINUE(ret, dva_reg_read(unit, (((phyIdx << 16u) + 0xA408u) << 1u), &regVal));
            *autoNegoCapability = 0u;

            if(0u != (regVal & 0x0020u))
            {
                /* 10M HALF*/
                *autoNegoCapability |= (uint8)AN_10M_SPEED_HALF_DUPLEX;
            }
            if(0u != (regVal & 0x0040u))
            {
                /* 10M FULL*/
                *autoNegoCapability |= (uint8)AN_10M_SPEED_FULL_DUPLEX;
            }
            if(0u != (regVal & 0x0080u))
            {
                /* 100M HALF*/
                *autoNegoCapability |= (uint8)AN_100M_SPEED_HALF_DUPLEX;
            }
            if(0u != (regVal & 0x0100u))
            {
                /* 100M FULL*/
                *autoNegoCapability |= (uint8)AN_100M_SPEED_FULL_DUPLEX;
            }
        }
        else if(mode == EM_MODE_EXT_GIGA_ETHERNET)
        {
            /* Giga Ethernet */
            /* For GMII restriction, GbE support 10/100 Full/Half AN capability */
            /* Get speed & duplex */
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, (((phyIdx << 16u) + 0xA412u) << 1u), 9u, 1u, &regVal));
            if(0u != regVal)
            {
                /* 1000M Full */
                *autoNegoCapability |= (uint8)AN_1000M_SPEED_FULL_DUPLEX;
            }
        }
        else
        {
            ret = -RT_ERR_PORT_MODE;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief Input error check for function dva_port_xmii_ssc_set.
 * @param[in] userPort               port id (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END)
 *                                   The port id indicates which port mode is configured.
 * @param[in] isEnable               Spread signal controller enable status. \n
 *                                   Gets spread signal controller state, 0 = enabled, 1 = disabled.
 * @param[in] sequenceModeOffset     Phase offset in sequence mode. \n
 *                                   Gets phase offset in sequence mode(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[in] phaseOffset            SSC controller output phase offset. \n
 *                                   Gets SSC controller output phase offset(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @retval RT_ERR_OK                 To execute this function correctly
 * @retval Others                    Please refer to dva_reg_read() and read_write()
 */
RtkApiRet dva_drv_port_xmii_ssc_err_chk(RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = dva_g_userPort2sysPort[userPort];

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

        if(DVA_MAX_SEQUENCEMODE_OFFSET < sequenceModeOffset)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        if(DVA_MAX_PHASE_OFFSET < phaseOffset)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

    } while(0u == 1u);

    return retVal;
}

RtkApiRet dva_drv_port_combo_intf_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode)
{
    RtkApiRet ret                   = RT_ERR_OK;
    uint32    sysPort               = 0;
    uint32    comboPortIntfRegVal[] = RTK_MODE_INTERFACE_COMBO_VALUE;

    do
    {
        if(portMode >= EM_MODE_EXT_END)
        {
            ret = -(RtkApiRet)RT_ERR_PORT_MODE;
            continue;
        }

        /* Check for valid user port ID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }
        else
        {
            sysPort = dva_g_userPort2sysPort[userPort];

            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, comboPortIntfRegVal[portMode]));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_port_polling_tx_nopkt(uint32 unit, uint32 userPort, bool *result)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    txNoPkt = 0u;
    uint32    sysPort = 0;

    do
    {
        /*init value */
        *result = false;

        /* Check for valid user port ID */
        if(0u == DVA_IS_VALID_PORT_ID(userPort))
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }
        else
        {
            sysPort = dva_g_userPort2sysPort[userPort];

            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_get_reg_field(unit, DVA_PORT0_SMI_STATUS + (PERPORT_REG_OFFSET * sysPort), DVA_PORT0_SMI_STATUS_TX_NOPKT_LSP, DVA_PORT0_SMI_STATUS_TX_NOPKT_LEN, &txNoPkt));

            if(txNoPkt == DVA_PORT_TX_WITH_PKT)
            {
                /*do onthing*/
                ;
            }
            else
            {
                *result = true;
            }
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_port_mactop_ecoclk_set(uint32 unit, RtkPort userPort, uint32 status)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0;

    sysPort = dva_g_userPort2sysPort[userPort];

    do
    {
        if((status != ENABLE) && (status != DISABLE))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        if(ENABLE == status)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_MACTOP_EN_LSP, DVA_PORT0_PORT_CTRL0_RG_MACTOP_EN_LEN, status));

            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(50u));

            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_ECO_CLK_GATED_LSP, DVA_PORT0_PORT_CTRL0_RG_ECO_CLK_GATED_LEN, status));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_ECO_CLK_GATED_LSP, DVA_PORT0_PORT_CTRL0_RG_ECO_CLK_GATED_LEN, status));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_MACTOP_EN_LSP, DVA_PORT0_PORT_CTRL0_RG_MACTOP_EN_LEN, status));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_port_mac_speed_set(uint32 unit, RtkPort sysPort, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(DVA_SPEED_1000MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        }
        else if(DVA_SPEED_100MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_100M_SPEED_FULL_DUPLEX));
        }
        else if(DVA_SPEED_10MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_10M_SPEED_FULL_DUPLEX));
        }
        else
        {
            /*do nothing*/
            ;
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_port_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 speed, uint32 linkStatus)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0;
    bool      result  = false;

    do
    {
        if((DVA_RTK_USER_PORT_3RD != userPort) && (DVA_RTK_USER_PORT_7TH != userPort) && (DVA_RTK_USER_PORT_8TH != userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_port_polling_tx_nopkt(unit, userPort, &result));

        if(true == result)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, DISABLE));

            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, ENABLE));

            CHK_FUN_CONTINUE(ret, dva_drv_port_mac_speed_set(unit, sysPort, speed));

            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, ENABLE));

            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_FORCE_LINK_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_FORCE_LINK_LEN, linkStatus));

            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, ENABLE));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_xmii_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0;

    sysPort = dva_g_userPort2sysPort[userPort];

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_XMII_EN_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_XMII_EN_LEN, ENABLE));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_PAD_TXC_EN_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_PAD_TXC_EN_LEN, ENABLE));

        if(portMode == EM_MODE_EXT_MII_MAC)
        {
            /*config mac mode*/
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, DVA_MAC_MODE));
        }
        else if(portMode == EM_MODE_EXT_MII_PHY)
        {
            /*config phy mode*/
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_MII_PHY_MODE_LEN, DVA_PHY_MODE));
        }
        else if(portMode == EM_MODE_EXT_RMII_MAC)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, DVA_MAC_MODE));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_1V2_DIS_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_1V2_DIS_LEN, ENABLE));
        }
        else if(portMode == EM_MODE_EXT_RMII_PHY)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_PHY_MODE_LEN, DVA_PHY_MODE));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_1V2_DIS_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_RMII_1V2_DIS_LEN, ENABLE));
        }
        else
        {
            /*do nothing*/
            ;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_xmii_mode_chk(RtkModeExt_e portMode)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for serdesMode check, which portMode should be mii, rmii, sgmii and rgmii*/
        if((portMode != EM_MODE_EXT_MII_PHY) && (portMode != EM_MODE_EXT_MII_MAC) && (portMode != EM_MODE_EXT_RGMII) && (portMode != EM_MODE_EXT_RMII_PHY) && (portMode != EM_MODE_EXT_RMII_MAC))
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_t1_err_chk(RtkPort userPort, RtkModeExt_e portMode)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for serdesMode check, which portMode should be mii, rmii, sgmii and rgmii*/
        if((portMode != EM_MODE_EXT_100BASE_T1_PHY) && (portMode != EM_MODE_EXT_RTPG_T1_PHY) && (portMode != EM_MODE_EXT_10BASE_T1_PHY) && (portMode != EM_MODE_EXT_10BASE_T1S_PHY))
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }

        /* Check input parameters */
        if((DVA_PHY_T1_USER_PORT_MIN > userPort) || (DVA_PHY_T1_USER_PORT_MAX < userPort))
        {
            /* Return error if userPort is out of valid range */
            ret = -(RT_ERR_PORT_ID);
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_fe_err_chk(RtkPort userPort, RtkModeExt_e portMode)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Error handler for port check, which port number should be fe/gphy */
        if(DVA_RTK_USER_PORT_7TH != userPort)
        {
            /* Return error if userPort is not valid for FE/GPHY */
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Error handler for portMode check, which portMode should be FE or Giga Ethernet */
        if((EM_MODE_EXT_FAST_ETHERNET != portMode) && (EM_MODE_EXT_GIGA_ETHERNET != portMode))
        {
            /* Return error if portMode is not FE or Giga Ethernet */
            ret = -RT_ERR_PORT_MODE;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_port_xmii_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e portMode, uint32 speed, uint32 linkStatus)
{
    /* Initialize return value to indicate no error */
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Set the force link configuration for the port */
        CHK_FUN_CONTINUE(ret, dva_drv_port_force_link_set(unit, userPort, portMode, speed, linkStatus));

        /* Configure RMII or RGMII specific settings based on portMode */
        if(portMode == EM_MODE_EXT_RMII_PHY)
        {
            /* Set RMII PHY reference clock selection with inverted RXC */
            CHK_FUN_CONTINUE(ret, dva_port_rmii_refclk_sel_set(unit, userPort, DVA_INVERT_RXC, DVA_INVERT_RXC));
        }
        else if(portMode == EM_MODE_EXT_RMII_MAC)
        {
            /* Set RMII MAC reference clock selection with original phase RXC */
            CHK_FUN_CONTINUE(ret, dva_port_rmii_refclk_sel_set(unit, userPort, DVA_INVERT_RXC, DVA_ORIGIN_PHASE));
        }
        else if(portMode == EM_MODE_EXT_RGMII)
        {
            /* Enable RGMII TX delay */
            CHK_FUN_CONTINUE(ret, dva_port_rgmii_txdly_set(unit, userPort, (uint8)ENABLE));
        }
        else
        {
            /* Do nothing for other modes */
            ;
        }

        /* Set driving strength based on speed and portMode */
        if((speed == DVA_SPEED_1000MBPS) && (portMode == EM_MODE_EXT_RGMII))
        {
            /* Set driving strength for 1000Mbps RGMII */
            CHK_FUN_CONTINUE(ret, dva_port_xmii_driving_set(unit, userPort, 3u, 2u, 7u));
        }
        else
        {
            /* Set default driving strength */
            CHK_FUN_CONTINUE(ret, dva_port_xmii_driving_set(unit, userPort, 3u, 2u, 2u));
        }
    } while(0u == 1u);

    return ret;
}