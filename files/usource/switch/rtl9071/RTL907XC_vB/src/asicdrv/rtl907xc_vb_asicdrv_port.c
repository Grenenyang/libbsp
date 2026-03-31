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

#include "rtl907xc_vb_api.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xc_vb_reg_list.h"
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_asicdrv_port.h"
#include "rtl907xc_vb_regField_list.h"
#include "rtl907xc_vb_asicdrv_config.h"
#include "rtl907xc_vb_asicdrv.h"
#include "rtl907xc_vb_api_interface.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"

#define CVB_MAX_SEQUENCEMODE_OFFSET (0x7u)
#define CVB_MAX_PHASE_OFFSET        (0x7u)

static RtkApiRet    cvb_drv_port_sub_serdes_mode_get(uint32 ifType, uint32 sdsMode, RtkModeExt_e *mode);
static RtkModeExt_e asicdrv_port_get_speed(uint32 spdRegVal);

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] mask
 * @retval
 */
RtkApiRet cvb_drv_port_iso_mask_set(uint32 unit, uint32 port, uint32 mask)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = cvb_reg_field_write(unit, CVB_PORT0_PORT_ISOLATION_CONTROL + (port * PERPORT_REG_OFFSET), CVB_PORT0_PORT_ISOLATION_CONTROL_TISO_LSP, CVB_PORT0_PORT_ISOLATION_CONTROL_TISO_LEN, mask);

    return ret;
}

/**
 * @brief
 * @param[in] unit
 * @param[in] port
 * @param[in] pMask
 * @retval
 */
RtkApiRet cvb_drv_port_iso_mask_get(uint32 unit, uint32 port, uint32 *pMask)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = cvb_reg_field_read(unit, CVB_PORT0_PORT_ISOLATION_CONTROL + (port * PERPORT_REG_OFFSET), CVB_PORT0_PORT_ISOLATION_CONTROL_TISO_LSP, CVB_PORT0_PORT_ISOLATION_CONTROL_TISO_LEN, pMask);

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
RtkApiRet cvb_drv_port_rgmii_delay_cell_set(uint32 unit, RtkPort port, uint8 delay)
{
    RtkApiRet ret = RT_ERR_OK;
    RtkPort   userPort;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > port) || (CVB_RTK_USER_PORT_11TH < port))
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        userPort = cvb_g_sysPort2userPort[port];
        switch(userPort)
        {
            case INTF_HW_PORT4:
                ret = cvb_reg_field_write(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGRXC_DLY_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT5:
                ret = cvb_reg_field_write(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGRXC_DLY_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT8:
                ret = cvb_reg_field_write(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGRXC_DLY_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT9:
                ret = cvb_reg_field_write(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGRXC_DLY_LEN, (uint32)delay);
                break;
            case INTF_HW_PORT11:
                ret = cvb_reg_field_write(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL6, CVB_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGRXC_DLY_LEN, (uint32)delay);
                break;
            default:
                ret = -(RtkApiRet)RT_ERR_PORT_ID;
                break;
        }
        if(RT_ERR_OK != ret)
        {
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
RtkApiRet cvb_drv_port_rgmii_delay_cell_get(uint32 unit, RtkPort port, uint8 *pDelay)
{
    RtkApiRet ret   = RT_ERR_OK;
    uint32    rxdly = 0u;
    RtkPort   userPort;

    do
    {
        if((CVB_RTK_USER_PORT_1ST > port) || (CVB_RTK_USER_PORT_11TH < port))
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        userPort = cvb_g_sysPort2userPort[port];
        switch(userPort)
        {
            case INTF_HW_PORT4:
                ret = cvb_reg_field_read(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P4_RGMII_RGRXC_DLY_LEN, &rxdly);
                break;
            case INTF_HW_PORT5:
                ret = cvb_reg_field_read(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL4_P5_RGMII_RGRXC_DLY_LEN, &rxdly);
                break;
            case INTF_HW_PORT8:
                ret = cvb_reg_field_read(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P8_RGMII_RGRXC_DLY_LEN, &rxdly);
                break;
            case INTF_HW_PORT9:
                ret = cvb_reg_field_read(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL5_P9_RGMII_RGRXC_DLY_LEN, &rxdly);
                break;
            case INTF_HW_PORT11:
                ret = cvb_reg_field_read(unit, CVB_GLOBAL_MAC_INTERFACE_INTERNAL6, CVB_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGRXC_DLY_LSP, CVB_GLOBAL_MAC_INTERFACE_INTERNAL6_P11_RGMII_RGRXC_DLY_LEN, &rxdly);
                break;
            default:
                ret = -(RtkApiRet)RT_ERR_PORT_ID;
                break;
        }
        if(RT_ERR_OK != ret)
        {
            continue;
        }
        *pDelay = (uint8)(rxdly & 0xFFu);

    } while(0u == 1u);

    return ret;
}
static RtkApiRet cvb_drv_port_rgmii_addll_set_p4(uint32 unit, RtkPortSpeed_e speed, uint32 rxdly)
{
    RtkApiRet ret;
    do
    {
        /* The following parameter should be define after ASIC verified */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LEN, (uint32)rxdly));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_FBD_SEL_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_FBD_SEL_ADDLL1_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_LPF_SEL_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_LPF_SEL_ADDLL1_LEN, 3u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_FORCE_SEL_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_FORCE_SEL_ADDLL1_LEN, 46u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_SEL_DLL_RXC_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_SEL_DLL_RXC_ADDLL1_LEN, 1u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_RESET_N_ADDLL1_LEN, 1u));

        if(EM_PORT_SPEED_1000M == speed)
        {

            CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_CLK_EN_ADDLL1_LEN, 1u));
        }
        else
        {

            CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_FORCE_SEL_EN_ADDLL1_LEN, 1u));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet cvb_drv_port_rgmii_addll_set_p5_p8_p11(uint32 unit, RtkPortSpeed_e speed, uint32 rxdly)
{
    RtkApiRet ret;
    do
    {
        /* The following parameter should be define after ASIC verified */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_CLK_EN_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_CLK_EN_ADDLL2_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_FORCE_SEL_EN_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_FORCE_SEL_EN_ADDLL2_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_RESET_N_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_RESET_N_ADDLL2_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_CLK_SEL_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_CLK_SEL_ADDLL2_LEN, (uint32)rxdly));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_FBD_SEL_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_FBD_SEL_ADDLL2_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_LPF_SEL_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_LPF_SEL_ADDLL2_LEN, 3u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_FORCE_SEL_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_FORCE_SEL_ADDLL2_LEN, 46u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_SEL_DLL_RXC_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_SEL_DLL_RXC_ADDLL2_LEN, 1u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_RESET_N_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_RESET_N_ADDLL2_LEN, 1u));
        if(EM_PORT_SPEED_1000M == speed)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_CLK_EN_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_CLK_EN_ADDLL2_LEN, 1u));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_FORCE_SEL_EN_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_FORCE_SEL_EN_ADDLL2_LEN, 1u));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet cvb_drv_port_rgmii_addll_set_p9(uint32 unit, RtkPortSpeed_e speed, uint32 rxdly)
{
    RtkApiRet ret;
    do
    {
        /* The following parameter should be define after ASIC verified */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LEN, (uint32)rxdly));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_FBD_SEL_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_FBD_SEL_ADDLL0_LEN, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_LPF_SEL_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_LPF_SEL_ADDLL0_LEN, 3u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_FORCE_SEL_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_FORCE_SEL_ADDLL0_LEN, 46u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_SEL_DLL_RXC_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_SEL_DLL_RXC_ADDLL0_LEN, 1u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_RESET_N_ADDLL0_LEN, 1u));
        if(EM_PORT_SPEED_1000M == speed)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_CLK_EN_ADDLL0_LEN, 1u));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_FORCE_SEL_EN_ADDLL0_LEN, 1u));
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
RtkApiRet cvb_drv_port_rgmii_addll_set(uint32 unit, RtkPort port, RtkPortSpeed_e speed, uint8 delay)
{
    uint32    rxdly;
    RtkApiRet ret;

    do
    {

        rxdly = (delay == 0u) ? (uint32)3u : ((uint32)delay - 1u);

        if((uint32)INTF_HW_PORT4 == port)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_port_rgmii_addll_set_p4(unit, speed, rxdly));
        }
        else if(((uint32)INTF_HW_PORT5 == port) || ((uint32)INTF_HW_PORT8 == port) || ((uint32)INTF_HW_PORT11 == port))
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_port_rgmii_addll_set_p5_p8_p11(unit, speed, rxdly));
        }
        else if((uint32)INTF_HW_PORT9 == port)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_port_rgmii_addll_set_p9(unit, speed, rxdly));
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
RtkApiRet cvb_drv_port_rgmii_addll_get(uint32 unit, RtkPort port, uint8 *pDelay)
{
    uint32    rxdly = 0u;
    RtkApiRet ret   = RT_ERR_OK;

    do
    {

        if((uint32)INTF_HW_PORT4 == port)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_ADDLL_1_CTRL, CVB_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LSP, CVB_ADDLL_1_CTRL_RG_CLK_SEL_ADDLL1_LEN, &rxdly));
        }
        else if(((uint32)INTF_HW_PORT5 == port) || ((uint32)INTF_HW_PORT8 == port) || ((uint32)INTF_HW_PORT11 == port))
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_ADDLL_2_CTRL, CVB_ADDLL_2_CTRL_RG_CLK_SEL_ADDLL2_LSP, CVB_ADDLL_2_CTRL_RG_CLK_SEL_ADDLL2_LEN, &rxdly));
        }
        else if((uint32)INTF_HW_PORT9 == port)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_ADDLL_0_CTRL, CVB_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LSP, CVB_ADDLL_0_CTRL_RG_CLK_SEL_ADDLL0_LEN, &rxdly));
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
RtkApiRet cvb_drv_port_disable_rx_en(uint32 unit, uint32 portMask)
{
    uint8     idx = 0u;
    RtkApiRet ret = RT_ERR_OK;

    for(idx = CVB_RTK_SYS_PORT_1ST; idx <= CVB_RTK_SYS_PORT_11TH; idx++)
    {
        if(0u != (portMask & ((uint32)1u << idx)))
        {
            /* disable rx */
            CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (CVB_PORT0_L2_MISC + ((uint32)idx * 0x800u)), 1, 1, 0));
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
RtkApiRet cvb_drv_port_eth_disable_rx_en(uint32 unit, uint32 portMask)
{
    uint8     idx                                      = 0u;
    RtkApiRet ret                                      = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {0u};
    uint32    regAddr                                  = 0u;

    do
    {
        for(idx = CVB_RTK_SYS_PORT_1ST; idx <= CVB_RTK_SYS_PORT_11TH; idx++)
        {
            if(0u != (portMask & ((uint32)1u << idx)))
            {
                regAddr = (CVB_PORT0_L2_MISC + ((uint32)idx * 0x800u));

                /* Set read command */
                CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
            }
        }

        /* Send read command */
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));

        for(idx = CVB_RTK_SYS_PORT_1ST; idx <= CVB_RTK_SYS_PORT_11TH; idx++)
        {
            if(0u != (portMask & ((uint32)1u << idx)))
            {
                regAddr = (CVB_PORT0_L2_MISC + ((uint32)idx * 0x800u));

                /* Set write command to disable rx_en*/
                CHK_FUN_CONTINUE(ret, cvb_reg_field_set(regAddr, 1u, 1u, 0u, &regValRx[idx]));
                CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regValRx[idx], 0u));
            }
        }

        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regValRx[idx], 1u));
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
RtkApiRet cvb_drv_port_covert_port_ability_to_reg(uint32 *regVal, PortMacAbility_t *pMacAbility)
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
 * @brief To return the port mode corresponding to the inputted speed parameter
 * @param[in] spdRegVal The input speed register value of PHY reg 0
 *
 * @retval mode The port mode
 */
static RtkModeExt_e asicdrv_port_get_speed(uint32 spdRegVal)
{
    RtkModeExt_e mode = EM_MODE_EXT_RESERVED;
    switch(spdRegVal)
    {
        /* 2'b00: 10Mb/s */
        case 0u:
            mode = EM_MODE_EXT_10BASE_T1_PHY;
            break;
        /* 2'b01: 100Mb/s */
        case 1u:
            mode = EM_MODE_EXT_100BASE_T1_PHY;
            break;
        /* 2'b10: 1000Mb/s */
        case 2u:
            mode = EM_MODE_EXT_RTPG_T1_PHY;
            break;
        /* 2'b11: Reserved */
        default:
            mode = EM_MODE_EXT_RESERVED;
            break;
    }
    return mode;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port02_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;
    uint32    regSpd = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P2_INTF_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P2_INTF_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            regVal = 0u;
            /* Get reg0 bit6: regspd1 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0x74800u, 6u, &regSpd));
            regVal += regSpd * 2u;
            /* Get reg0 bit13: regspd0 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0x74800u, 13u, &regSpd));
            regVal += regSpd;
            /* Get the port mode based on the speed reg value */
            *mode = asicdrv_port_get_speed(regVal);
            /* RTL907xC_VB supports 10BASE-T1S */
            if(EM_MODE_EXT_10BASE_T1_PHY == *mode)
            {
                /* Get duplex mode */
                CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0x74800u, 8u, &regVal));
                if(0u == regVal)
                {
                    *mode = EM_MODE_EXT_10BASE_T1S_PHY;
                }
            }
        }
        else if(1u == ifType)
        {
            *mode = EM_MODE_EXT_MACPHY;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port03_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;
    uint32    regVal = 0u;
    uint32    regSpd = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P3_INTF_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P3_INTF_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            regVal = 0u;
            /* Get reg0 bit6: regspd1 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0x94800u, 6u, &regSpd));
            regVal += regSpd * 2u;
            /* Get reg0 bit13: regspd0 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0x94800u, 13u, &regSpd));
            regVal += regSpd;
            /* Get the port mode based on the speed reg value */
            *mode = asicdrv_port_get_speed(regVal);
            /* RTL907xC_VB supports 10BASE-T1S */
            if(EM_MODE_EXT_10BASE_T1_PHY == *mode)
            {
                /* Get duplex mode */
                CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0x94800u, 8u, &regVal));
                if(0u == regVal)
                {
                    *mode = EM_MODE_EXT_10BASE_T1S_PHY;
                }
            }
        }
        else if(1u == ifType)
        {
            *mode = EM_MODE_EXT_MACPHY;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port04_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    ifType      = 0u;
    uint32    xmiiPhyMode = 0u;
    uint32    regVal      = 0u;
    uint32    regSpd      = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P4_T1GMRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P4_T1GMRG_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            *mode = EM_MODE_EXT_RGMII;
        }
        /* rmii */
        else if(1u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P4_P10_RMII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P4_P10_RMII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_RMII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_RMII_MAC;
            }
        }
        /* mii */
        else if(2u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P4_P10_MII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P4_P10_MII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_MII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_MII_MAC;
            }
        }
        else
        {
            regVal = 0u;
            /* Get reg0 bit6: regspd1 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0xB4800u, 6u, &regSpd));
            regVal += regSpd * 2u;
            /* Get reg0 bit13: regspd0 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0xB4800u, 13u, &regSpd));
            regVal += regSpd;
            /* Get the port mode based on the speed reg value */
            *mode = asicdrv_port_get_speed(regVal);
            /* RTL907xC_VB supports 10BASE-T1S */
            if(EM_MODE_EXT_10BASE_T1_PHY == *mode)
            {
                /* Get duplex mode */
                CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0xB4800u, 8u, &regVal));
                if(0u == regVal)
                {
                    *mode = EM_MODE_EXT_10BASE_T1S_PHY;
                }
            }
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port05_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    ifType      = 0u;
    uint32    xmiiPhyMode = 0u;
    uint32    regVal      = 0u;
    uint32    regSpd      = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P5_T1GMRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P5_T1GMRG_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            *mode = EM_MODE_EXT_RGMII;
        }
        /* rmii */
        else if(1u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_RMII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_RMII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_RMII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_RMII_MAC;
            }
        }
        /* mii */
        else if(2u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_MII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_MII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_MII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_MII_MAC;
            }
        }
        else
        {
            regVal = 0u;
            /* Get reg0 bit6: regspd1 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0xD4800u, 6u, &regSpd));
            regVal += regSpd * 2u;
            /* Get reg0 bit13: regspd0 */
            CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0xD4800u, 13u, &regSpd));
            regVal += regSpd;
            /* Get the port mode based on the speed reg value */
            *mode = asicdrv_port_get_speed(regVal);
            /* RTL907xC_VB supports 10BASE-T1S */
            if(EM_MODE_EXT_10BASE_T1_PHY == *mode)
            {
                /* Get duplex mode */
                CHK_FUN_CONTINUE(ret, cvb_reg_bit_get(unit, 0xD4800u, 8u, &regVal));
                if(0u == regVal)
                {
                    *mode = EM_MODE_EXT_10BASE_T1S_PHY;
                }
            }
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port06_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P6_T1GM_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P6_T1GM_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            *mode = EM_MODE_EXT_100BASE_T1_PHY;
        }
        else if(1u == ifType)
        {
            *mode = EM_MODE_EXT_RTPG_T1_PHY;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port07_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    ifType = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P7_T1GM_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P7_T1GM_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            *mode = EM_MODE_EXT_100BASE_T1_PHY;
        }
        else if(1u == ifType)
        {
            *mode = EM_MODE_EXT_RTPG_T1_PHY;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port08_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    ifType      = 0u;
    uint32    xmiiPhyMode = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P8_FESGRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P8_FESGRG_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            *mode = EM_MODE_EXT_RGMII;
        }
        /* rmii */
        else if(1u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_RMII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_RMII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_RMII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_RMII_MAC;
            }
        }
        /* mii */
        else if(2u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_MII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_MII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_MII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_MII_MAC;
            }
        }
        /* Etherent */
        else if(3u == ifType)
        {
            *mode = EM_MODE_EXT_FAST_ETHERNET;
        }
        else if(4u == ifType)
        {
            *mode = EM_MODE_EXT_SGMII;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port09_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    ifType      = 0u;
    uint32    xmiiPhyMode = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_0, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P9_T1SGRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_0_P9_T1SGRG_CB_CFG_LEN, &ifType));

        if(0u == ifType)
        {
            *mode = EM_MODE_EXT_RGMII;
        }
        /* rmii */
        else if(1u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P9_RMII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P9_RMII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_RMII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_RMII_MAC;
            }
        }
        /* mii */
        else if(2u == ifType)
        {
            CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P9_MII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P9_MII_PHY_MD_LEN, &xmiiPhyMode));

            if(0u != xmiiPhyMode)
            {
                *mode = EM_MODE_EXT_MII_PHY;
            }
            else
            {
                *mode = EM_MODE_EXT_MII_MAC;
            }
        }
        else if(3u == ifType)
        {
            *mode = EM_MODE_EXT_XMII_DUMMY;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief To get the serdes interface mode configuration
 * @param[in] ifType                    Interface type
 *                                      This value is get from register which is used to identify serdes mode or note
 * @param[in] sdsMode                  Serdes mode
 *                                      This value is get from register which is used to identify which serdes mode.
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to cvb_reg_read().
 */
static RtkApiRet cvb_drv_port_sub_serdes_mode_get(uint32 ifType, uint32 sdsMode, RtkModeExt_e *mode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(5u == ifType)
    {
        if(0x12u == sdsMode)
        {
            *mode = EM_MODE_EXT_HISGMII;
        }
        else if(0x2u == sdsMode)
        {
            *mode = EM_MODE_EXT_SGMII;
        }
        else if(0xDu == sdsMode)
        {
            *mode = EM_MODE_EXT_USXGMII;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
            ret   = -RT_ERR_INPUT;
        }
    }
    else if(6u == ifType)
    {
        if(0x0Du == sdsMode)
        {
            *mode = EM_MODE_EXT_USXGMII;
        }
        else if(0x1Du == sdsMode)
        {
            *mode = EM_MODE_EXT_5G_KR;
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

    return ret;
}

/**
 * @brief To get the port inteface mode.
 * @param[in] unit                      Stacking unit
 * @param[out] mode                     The interface mode
 *                                      To indicate which interface is set currently.
 * @retval RT_ERR_OK                    Get the mode status successfully.
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port10_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    ifType  = 0u;
    uint32    sdsMode = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_1, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P10_T1GMRG_CB_CFG_LEN, &ifType));
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, CVB_USX_CTRL_0_US0, &sdsMode));
        sdsMode &= 0x1fu;

        if(3u == ifType)
        {
            *mode = EM_MODE_EXT_100BASE_T1_PHY;
        }
        else if(4u == ifType)
        {
            *mode = EM_MODE_EXT_PCIE_1G_MAC_GMII;
        }
        else if((5u == ifType) || (6u == ifType))
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_port_sub_serdes_mode_get(ifType, sdsMode, mode));
        }
        else if(7u == ifType)
        {
            *mode = EM_MODE_EXT_PCIE_5G_MAC_GMII;
        }
        else if(8u == ifType)
        {
            *mode = EM_MODE_EXT_PCIE_5G_MAC_XGMII;
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port11_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    ifType      = 0u;
    uint32    sdsMode     = 0u;
    uint32    xmiiPhyMode = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_1, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P11_T1GMRG_CB_CFG_LEN, &ifType));
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, CVB_USX_CTRL_0_US1, &sdsMode));
        sdsMode &= 0x1fu;
        switch(ifType)
        {
            case 0u:
                *mode = EM_MODE_EXT_RGMII;
                break;
            case 1u:
                ret = cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_RMII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_RMII_PHY_MD_LEN, &xmiiPhyMode);
                if(0u != xmiiPhyMode)
                {
                    *mode = EM_MODE_EXT_RMII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_RMII_MAC;
                }
                break;
            case 2u:
                ret = cvb_reg_field_read(unit, CVB_GLOBAL_MACPHY_MODE_SEL, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_MII_PHY_MD_LSP, CVB_GLOBAL_MACPHY_MODE_SEL_P5_P11_MII_PHY_MD_LEN, &xmiiPhyMode);

                if(0u != xmiiPhyMode)
                {
                    *mode = EM_MODE_EXT_MII_PHY;
                }
                else
                {
                    *mode = EM_MODE_EXT_MII_MAC;
                }
                break;
            case 3u:
                *mode = EM_MODE_EXT_RESERVED;
                break;
            case 4u:
                *mode = EM_MODE_EXT_PCIE_1G_MAC_GMII;
                break;
            case 5u:
            case 6u:
                ret = cvb_drv_port_sub_serdes_mode_get(ifType, sdsMode, mode);
                break;
            case 7u:
                *mode = EM_MODE_EXT_PCIE_5G_MAC_GMII;
                break;
            case 8u:
                *mode = EM_MODE_EXT_PCIE_5G_MAC_XGMII;
                break;
            default:
                *mode = EM_MODE_EXT_RESERVED;
                break;
        }
        if(RT_ERR_OK != ret)
        {
            continue;
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
 * @retval Others                       Please refer to cvb_reg_read().
 */
RtkApiRet cvb_drv_port_sub_port12_mode_get(uint32 unit, RtkModeExt_e *mode)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    ifType  = 0u;
    uint32    sdsMode = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_field_read(unit, CVB_MAC_INTF_COMBO_PORT_CONFIG_1, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P12_T1SG_CB_CFG_LSP, CVB_MAC_INTF_COMBO_PORT_CONFIG_1_P12_T1SG_CB_CFG_LEN, &ifType));
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, CVB_USX_CTRL_0_US2, &sdsMode));

        sdsMode &= 0x1fu;

        if((5u == ifType) || (6u == ifType))
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_port_sub_serdes_mode_get(ifType, sdsMode, mode));
        }
        else
        {
            *mode = EM_MODE_EXT_RESERVED;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Etherent linking status with anto-nego mode.
 * @param[in] unit                      Stacking unit
 * @param[in] pEthAbility               Etherent link ability \n
 *                                      This parameter inclde link speed/duplex and link status which to be set. \n
 *                                      The supported ability set: 10M Full/10M Half/100M Full/100M Half.
 * @retval RT_ERR_OK                    To execute this function correctly
 * @retval Others                       Please refer to cvb_reg_read() and read_write()
 */
RtkApiRet cvb_drv_port_eth_an_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility)
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
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (((phyIdx << 16u) + 0xA408u) << 1u), 5u, 4u, regVal));

        /* bit9: Restart auto-nego, bit12: Auto-Nego enable/disable */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 12u, 1u, 1u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 9u, 1u, 1u));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Set Etherent linking status.
 * @param[in] unit                      Stacking unit
 * @param[out] pEthAbility              Etherent link ability
 *                                      This parameter inclde link speed/duplex and link status which to be set.
 * @retval RT_ERR_OK                    To execute this function correctly
 * @retval Others                       Please refer to cvb_reg_read() and read_write()
 */
RtkApiRet cvb_drv_port_eth_force_mode_set(uint32 unit, const PortEthLinkAbility_t *pEthAbility)
{
    uint32    phyIdx = 0xBu;
    RtkApiRet ret    = RT_ERR_OK;

    do
    {
        /* bit6: speed selection msb bit, bit16: speed selection lsb bit */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 6u, 1u, 0u));
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 13u, 1u, (pEthAbility->speed >> 1u)));
        /* bit8: duplex */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 8u, 1u, (pEthAbility->duplex >> 1u)));

        /* bit12: Auto-Nego enable/disable */
        CHK_FUN_CONTINUE(ret, cvb_reg_field_write(unit, (((phyIdx << 16u) + 0xA400u) << 1u), 12u, 1u, 0u));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Input error check for function cvb_port_xmii_ssc_set.
 * @param[in] userPort               Port id (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END)
 *                                   The port id indicates which port mode is configured.
 * @param[in] isEnable               Spread signal controller enable status. \n
 *                                   Gets spread signal controller state, 0 = enabled, 1 = disabled.
 * @param[in] sequenceModeOffset     Phase offset in sequence mode. \n
 *                                   Gets phase offset in sequence mode(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[in] phaseOffset            SSC controller output phase offset. \n
 *                                   Gets SSC controller output phase offset(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @retval RT_ERR_OK                 To execute this function correctly
 * @retval Others                    Please refer to cvb_reg_read() and read_write()
 */
RtkApiRet cvb_drv_port_xmii_ssc_err_chk(RtkPort userPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        if(0u == CVB_IS_VALID_PORT_ID(userPort))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        port = cvb_g_userPort2sysPort[userPort];

        if((port != INTF_HW_PORT4) && (port != INTF_HW_PORT5) && (port != INTF_HW_PORT8) && (port != INTF_HW_PORT9) && (port != INTF_HW_PORT11))
        {
            retVal = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if((ENABLED != isEnable) && (DISABLED != isEnable))
        {
            retVal = -(RtkApiRet)RT_ERR_ENABLE;
            continue;
        }

        if(CVB_MAX_SEQUENCEMODE_OFFSET < sequenceModeOffset)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }

        if(CVB_MAX_PHASE_OFFSET < phaseOffset)
        {
            retVal = -(RtkApiRet)RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the signal to spread over the widest range of frequencies.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sysPort               Port id (CVB_RTK_SYS_PORT_1ST to CVB_RTK_SYS_PORT_END)
 *                                  The port id indicates which port mode is configured.
 * @param[in] isEnable              Spread signal controller enable status. \n
 *                                  Gets spread signal controller state, 0 = enabled, 1 = disabled.
 * @param[in] sequenceModeOffset    Phase offset in sequence mode. \n
 *                                  Gets phase offset in sequence mode(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @param[in] phaseOffset           SSC controller output phase offset. \n
 *                                  Gets SSC controller output phase offset(0-7), 0x3 = default offset, 0x7 = maximum offset.
 * @retval RT_ERR_OK                To execute this function correctly
 * @param[out] None
 * @retval  RT_ERR_OK               To execute this function correctly
 * @retval Others                   Please refer to cvb_drv_indirect_reg_field_set()
 */
RtkApiRet cvb_drv_port_xmii_ssc_set(uint32 unit, RtkPort sysPort, RtkEnable isEnable, uint32 sequenceModeOffset, uint32 phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if(sysPort == INTF_HW_PORT4)
        {
            /* xmii 2 */
            /* enable SSC controller */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_1_CONTROL4, CVB_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LSP, CVB_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LEN, isEnable));
            /*The maximum phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_1_CONTROL2, CVB_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LSP, CVB_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LEN, sequenceModeOffset));
            /* Allow the SSC controller to output the maximum phase offset */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_1_CONTROL4, CVB_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LSP, CVB_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LEN, phaseOffset));
        }
        else if(sysPort == INTF_HW_PORT9)
        {
            /* xmii 1 */
            /* enable SSC controller */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_0_CONTROL4, CVB_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LSP, CVB_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LEN, isEnable));
            /*The maximum phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_0_CONTROL2, CVB_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LSP, CVB_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LEN, sequenceModeOffset));
            /* Allow the SSC controller to output the maximum phase offset */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_0_CONTROL4, CVB_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LSP, CVB_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LEN, phaseOffset));
        }
        else
        {
            /* xmii 0 */
            /* enable SSC controller */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_2_CONTROL4, CVB_SSC_2_CONTROL4_EN_SSC_GMII_ASSC_P2_LSP, CVB_SSC_2_CONTROL4_EN_SSC_GMII_ASSC_P2_LEN, isEnable));
            /*The maximum phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_0_CONTROL2, CVB_SSC_2_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P2_LSP, CVB_SSC_2_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P2_LEN, sequenceModeOffset));
            /* Allow the SSC controller to output the maximum phase offset */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_SSC_2_CONTROL4, CVB_SSC_2_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P2_LSP, CVB_SSC_2_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P2_LEN, phaseOffset));
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the signal to spread over the widest range of frequencies.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sysPort                 port id (CVB_RTK_SYS_PORT_1ST to CVB_RTK_SYS_PORT_END)
 *                                    The port id indicates which port mode is configured.
 * @param[out] isEnable               Spread signal contoller enable.
 *                                    Get spread signal contoller state, 0 = enable , 1 = disable spread signal contoller
 * @param[out] sequenceModeOffset     Phase offset in sequence mode
 *                                    Get Phase offset in sequence mode, 0x3 = default pffset , 0x7 = maximum phase offset
 * @param[out] phaseOffset            SSC controller to output the phase offset
 *                                    Get SSC controller to output the phase offset, 0x3 = default pffset , 0x7 = maximum phase offset
 * @param[out] None
 * @retval  RT_ERR_OK                 To execute this function correctly
 * @retval Others                     Please refer to cvb_drv_indirect_reg_field_get()
 */
RtkApiRet cvb_drv_port_xmii_ssc_get(uint32 unit, RtkPort sysPort, RtkEnable *isEnable, uint32 *sequenceModeOffset, uint32 *phaseOffset)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if(sysPort == INTF_HW_PORT4)
        {
            /* xmii 2 */
            /* Get SSC controller */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_1_CONTROL4, CVB_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LSP, CVB_SSC_1_CONTROL4_EN_SSC_GMII_ASSC_P1_LEN, isEnable));
            /* Get the phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_1_CONTROL2, CVB_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LSP, CVB_SSC_1_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P1_LEN, sequenceModeOffset));
            /* Get the SSC controller output phase offset */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_1_CONTROL4, CVB_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LSP, CVB_SSC_1_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P1_LEN, phaseOffset));
        }
        else if(sysPort == INTF_HW_PORT9)
        {
            /* xmii 1 */
            /* Get SSC controller */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_0_CONTROL4, CVB_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LSP, CVB_SSC_0_CONTROL4_EN_SSC_GMII_ASSC_P0_LEN, isEnable));
            /* Get the phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_0_CONTROL2, CVB_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LSP, CVB_SSC_0_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P0_LEN, sequenceModeOffset));
            /* Get the SSC controller output phase offset */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_0_CONTROL4, CVB_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LSP, CVB_SSC_0_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P0_LEN, phaseOffset));
        }
        else
        {
            /* xmii 0 */
            /* Get SSC controller */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_2_CONTROL4, CVB_SSC_2_CONTROL4_EN_SSC_GMII_ASSC_P2_LSP, CVB_SSC_2_CONTROL4_EN_SSC_GMII_ASSC_P2_LEN, isEnable));
            /* Get the phase offset in sequence mode*/
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_2_CONTROL2, CVB_SSC_2_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P2_LSP, CVB_SSC_2_CONTROL2_RG_SEQ_PH_MAX_GMII_ASSC_P2_LEN, sequenceModeOffset));
            /* Get the SSC controller output phase offset */
            CHK_FUN_CONTINUE(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_SSC_2_CONTROL4, CVB_SSC_2_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P2_LSP, CVB_SSC_2_CONTROL4_RG_MAX_PH_OFST_GMII_ASSC_P2_LEN, phaseOffset));
        }
    } while(0u == 1u);

    return retVal;
}