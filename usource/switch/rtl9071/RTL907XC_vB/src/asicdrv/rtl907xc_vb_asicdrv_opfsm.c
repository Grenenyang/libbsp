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
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xc_vb_asicdrv_phy.h"
#include "rtl907xc_vb_asicdrv_opfsm.h"
#include "rtl907xc_vb_asicdrv_config.h"

const uint8 cvb_g_userPort2T1phy[CVB_RTL9075_INTERFACE_NUM] = CVB_MAP_USERPORT_TO_T1PHY;

/**
 * @brief Covert user pmap <-> T1phy pmap for RL6760. \n
 *      For RL6760, user port1, port2, port3, port4, port5, port6 are with T1-PHY interface.
 * @param[in]   srcPmap  Source port mask. (0 to 0x33C) \n
 *                       The port mask to be converted.
 * @param[in]   mode     Convert Mode. (N/A) \n
 *                       From user pmap to T1phy pmap or from T1phy pmap to user pmap.
 * @param[out]  *dstPmap Destination port mask (0 to 0x33C)\n
 *                       The port mask conversion output.
 * @retval RT_ERR_OK            Success.
 * @retval -RT_ERR_NULL_POINTER The input parameter *dstPmap is NULL.
 * @retval -RT_ERR_INPUT        The Input mode is invalid.
 */
RtkApiRet cvb_drv_t1phy_pmap_convert(RtkPmap srcPmap, RtkPmap *dstPmap, cvb_RtkT1PHYMapConvertMode_e mode)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPmap   tmpPmap = 0u;
    RtkPmap   usrPmap = CVB_RTK_USER_PMAP_WITHT1PHY;
    RtkPmap   t1Pmap  = CVB_RTK_T1PHY_PMAP_ALL;

    do
    {
        if(NULL == dstPmap)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(CVB_EM_USER_TO_T1PHY == mode)
        {
            if(CVB_RTK_USER_PMAP_WITHT1PHY < srcPmap)
            {
                ret = -RT_ERR_PORT_MASK;
                continue;
            }

            if(srcPmap != (srcPmap & usrPmap))
            {
                ret = -RT_ERR_INPUT;
                continue;
            }

            tmpPmap |= (((uint32)0x1u & (srcPmap >> CVB_RTK_USER_PORT_1ST)) << cvb_g_userPort2T1phy[CVB_RTK_USER_PORT_1ST]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> CVB_RTK_USER_PORT_2ND)) << cvb_g_userPort2T1phy[CVB_RTK_USER_PORT_2ND]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> CVB_RTK_USER_PORT_3RD)) << cvb_g_userPort2T1phy[CVB_RTK_USER_PORT_3RD]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> CVB_RTK_USER_PORT_4TH)) << cvb_g_userPort2T1phy[CVB_RTK_USER_PORT_4TH]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> CVB_RTK_USER_PORT_5TH)) << cvb_g_userPort2T1phy[CVB_RTK_USER_PORT_5TH]);
            tmpPmap |= (((uint32)0x1u & (srcPmap >> CVB_RTK_USER_PORT_6TH)) << cvb_g_userPort2T1phy[CVB_RTK_USER_PORT_6TH]);
        }
        else if(CVB_EM_T1PHY_TO_USER == mode)
        {
            if(CVB_RTK_T1PHY_PMAP_ALL < srcPmap)
            {
                ret = -RT_ERR_PORT_MASK;
                continue;
            }

            if(srcPmap != (srcPmap & t1Pmap))
            {
                ret = -RT_ERR_INPUT;
                continue;
            }

            tmpPmap |= (((srcPmap & CVB_RTK_T1PHY_PMAP_1ST) >> CVB_RTK_T1PHY_PORT_1ST) << CVB_RTK_USER_PORT_1ST);
            tmpPmap |= (((srcPmap & CVB_RTK_T1PHY_PMAP_2ND) >> CVB_RTK_T1PHY_PORT_2ND) << CVB_RTK_USER_PORT_2ND);
            tmpPmap |= (((srcPmap & CVB_RTK_T1PHY_PMAP_3RD) >> CVB_RTK_T1PHY_PORT_3RD) << CVB_RTK_USER_PORT_3RD);
            tmpPmap |= (((srcPmap & CVB_RTK_T1PHY_PMAP_4TH) >> CVB_RTK_T1PHY_PORT_4TH) << CVB_RTK_USER_PORT_4TH);
            tmpPmap |= (((srcPmap & CVB_RTK_T1PHY_PMAP_5TH) >> CVB_RTK_T1PHY_PORT_5TH) << CVB_RTK_USER_PORT_5TH);
            tmpPmap |= (((srcPmap & CVB_RTK_T1PHY_PMAP_6TH) >> CVB_RTK_T1PHY_PORT_6TH) << CVB_RTK_USER_PORT_6TH);
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        *dstPmap = tmpPmap;

    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set 1000M-T1 PHY's Sleep Capability Enable or Disable.
 *      port5 = phy7 (1000BASE-T1),  port6 = phy8 (1000BASE-T1).
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] regOffset     reg address offset (0 to 0xFFFFFFFF) \n
 *                          PHY sleep capability reg address offset.
 * @param[in] regOAMOffset  OAM reg address (0 to 0xFFFFFFFF) \n
 *                          PHY OAM capability reg address offset.
 * @param[in] sleepCapOn    The sleep capability setting. (DISABLE or ENABLE) \n
 *                          ENABLE  : Enable the sleep capability \n
 *                          DISABLE : Disable the sleep capability
 * @param[out] None
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to cvb_reg_read() and cvb_reg_write().
 */
RtkApiRet cvb_drv_phy_1000mt1_phy_sleep_cap_set(uint32 unit, uint32 regOffset, uint32 regOAMOffset, RtkEnable sleepCapOn)
{
    uint32    regValue    = 0;
    uint32    regOAMValue = 0;
    RtkApiRet retVal      = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, &regValue));
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOAMOffset, &regOAMValue));
        if((sleepCapOn != 0u) && ((regValue & (0x1u)) == (0x0u)))
        {
            /* turns 0 to 1 */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, regOffset, regValue | (0x1u)));
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, regOAMOffset, regOAMValue | (0x2u)));
        }
        else if((sleepCapOn == 0u) && ((regValue & (0x1u)) == (0x1u)))
        {
            /* turns 1 to 0 */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, regOffset, regValue & 0xFFFEu));
        }
        else
        {
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set 100M-T1 PHY's Sleep Capability Enable or Disable.
 *      port5 = phy7 (1000BASE-T1),  port6 = phy8 (1000BASE-T1).
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] regOffset     reg address offset (0 to 0xFFFFFFFF) \n
 *                          PHY sleep capability reg address offset.
 * @param[in] sleepCapOn    The sleep capability setting. (DISABLE or ENABLE) \n
 *                          ENABLE  : Enable the sleep capability \n
 *                          DISABLE : Disable the sleep capability
 * @param[out] None
 * @retval RT_ERR_OK Success.
 * @retval Others    Please refer to cvb_reg_read() and cvb_reg_write().
 */
RtkApiRet cvb_drv_phy_100mt1_phy_sleep_cap_set(uint32 unit, uint32 regOffset, RtkEnable sleepCapOn)
{
    uint32    regValue = 0;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, cvb_reg_read(unit, regOffset, &regValue));

        if((sleepCapOn != 0u) && ((regValue & (0x1u)) == (0x0u)))
        {
            /* turns 0 to 1 */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, regOffset, regValue | (0x1u)));
        }
        else if((sleepCapOn == 0u) && ((regValue & (0x1u)) == (0x1u)))
        {
            /* turns 1 to 0 */
            CHK_FUN_CONTINUE(retVal, cvb_reg_write(unit, regOffset, regValue & 0xFFFEu));
        }
        else
        {
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

/* @brief Disable WUP transmit capability of all ports and keep the original WUP transmit capability setting.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] wupPortMask Ports with WUP transmit capability.
 * @retval RT_ERR_OK - Success.
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_op_disable_wup_transmit(uint32 unit, RtkPmap *wupPortMask)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, (CVB_OP_EN_WUPR_TX << 1U), wupPortMask));
        /* disable all port wup tx */
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, (CVB_OP_EN_WUPR_TX << 1U), 0u));

    } while(0u == 1u);

    return ret;
}

/* @brief Enable WUP transmit capability of selected ports.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] wupPortMask Ports with WUP transmit capability.
 * @retval RT_ERR_OK - Success.
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_op_enable_wup_transmit(uint32 unit, RtkPmap wupPortMask)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* enable all port wup tx */
        CHK_FUN_CONTINUE(ret, cvb_reg_write(unit, (CVB_OP_EN_WUPR_TX << 1U), wupPortMask));

    } while(0u == 1u);

    return ret;
}

/* @brief When updating configuration in flashless mode via ethernet,\n
 *     returns the current port list with enabling WUP capability and disable WUP transmit capability of all ports.
 * @param[in] unit - stacking unit
 * @param[in] macaddr - mac address.
 * @param[out] wupPortMask - Ports with WUP transmit capability.
 * @retval RT_ERR_OK - Success.
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_op_eth_disable_wup_transmit(uint32 unit, RtkPmap *wupPortMask)
{
    RtkApiRet ret                                      = RT_ERR_OK;
    uint32    regValRx[RTK_ETHERACCESS_MAX_BURST - 1u] = {0u};
    uint32    regAddr                                  = 0u;
    uint32    regVal                                   = 0u;

    do
    {
        regAddr = (CVB_OP_EN_WUPR_TX << 1U);

        /* Set read command */
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 0u));
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_tx_packet_assmeble_rx_decode(CVB_ETHERACCESS_REGISTER_RW_MAC_READ, regAddr, regValRx, 1u));
        *wupPortMask = regValRx[0];

        /* Set write command to disable wup tx*/
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 0u));
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, regVal, 1u));

    } while(0u == 1u);

    return ret;
}

/* @brief When updating configuration in flashless mode via ethernet,\n
 *     enable WUP capability of selected ports.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] macaddr - mac address.
 * @param[in] wupPortMask Ports with WUP transmit capability.
 * @retval RT_ERR_OK - Success.
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_op_eth_enable_wup_transmit(uint32 unit, RtkPmap wupPortMask)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;

    do
    {
        regAddr = (CVB_OP_EN_WUPR_TX << 1U);

        /* Set write command to disable wup tx*/
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, wupPortMask, 0u));
        CHK_FUN_CONTINUE(ret, cvb_drv_config_eth_packet_assmeble(CVB_ETHERACCESS_REGISTER_RW_MAC_WRITE, regAddr, wupPortMask, 1u));

    } while(0u == 1u);

    return ret;
}

/* @brief Configure the wake o config by flash reserved header.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name
 * @retval RT_ERR_OK - Success.
 * @retval -RT_ERR_FILE_READ
 */
RtkApiRet cvb_drv_op_fetch_wakeo_config(uint32 unit, const int8 *fileName)
{
    RtkApiRet retVal             = RT_ERR_OK;
    uint32    wakeoCfg           = 0x0u;
    uint32    gpio1Cfg           = 0xFFFFu;
    uint32    gpio2Cfg           = 0xFFFFu;
    uint32    regVal1            = 0u;
    uint32    regVal2            = 0u;
    uint32    cfgBit             = 0u;
    void     *fp                 = NULL;
    uint8     wakeoGpioCfg[6]    = {0u, 0u, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
    uint8     configBit2PhyIdx[] = {2u, 3u, 4u, 5u, 8u, 9u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
    do
    {
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(fp == NULL)
        {
            retVal = -RT_ERR_FILE_READ;
            continue;
        }
        /*
         * Read Wake-out forwarding config from flash in the first-time power-on.
         * 6 bytes layout of wakeo config in flash reserved area:
         * ----------------------------------------------------------------------
         * Byte[1:0]
         *     bit[15:12]: reserved
         *     bit[11]   : wakeo_config_by_flash.
         *                 0 = wakeo config is from flash.
         *                 1 = wakeo config is not from flash
         *     bit[10]   : amt_unit_cfg
         *     bit[9:0]  : pulse_amt_cfg
         * Byte[3:2]: wakeo_en of GPIOB1
         * Byte[5:4]: wakeo_en of GPIOB2
         * ----------------------------------------------------------------------
         */
        if((int32)0 == rtl_file_read(fp, wakeoGpioCfg, CVB_WAKEO_CONFIG_FLASH_ADDRESST, 6u))
        {
            retVal = -RT_ERR_FILE_READ;
            continue;
        }
        wakeoCfg = ((uint32)wakeoGpioCfg[0] & 0xFFu) | (((uint32)wakeoGpioCfg[1] & 0xFFu) << 8u);
        gpio1Cfg = ((uint32)wakeoGpioCfg[2] & 0xFFu) | (((uint32)wakeoGpioCfg[3] & 0xFFu) << 8u);
        gpio2Cfg = ((uint32)wakeoGpioCfg[4] & 0xFFu) | (((uint32)wakeoGpioCfg[5] & 0xFFu) << 8u);
        if(0u == (wakeoCfg & 0x800u))
        {
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, (CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x005Au) << 1u)), 0u, 16u, (wakeoCfg & 0x7FFu)));
            /* RL6760 provides GPIOB1 and GPIOB2 for wakeo config.
             * ------------------------------------------------------------------------------------------
             * Config of user port mask enable wakeo on GPIOB1.
             *   [fw_dmy_0]: 16bits. (CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x005Eu) << 1u) == (0x4A0010BC)
             * Config of user port mask enable wakeo on GPIOB2.
             *   [fw_dmy_1]: 16bits. (CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x0060u) << 1u) == (0x4A0010C0)
             *
             * Config bit and user port mapping
             *   +---------+------------------------------------------------+
             *   |cfg  bit | 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0 |
             *   |user port| 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1 |
             *   +---------+------------------------------------------------+
             *
             * User-port/Sys-port/T1-PHY index mapping
             *   +-----------+----------------------------------+
             *   |user port  | 11 10 9   8  7  6  5  4  3  2  1 |
             *   |sys  port  | 12 11 10  9  8  7  6  5  4  3  2 |
             *   |T1-phy idx | -- -- -- -- --  9  8  5  4  3  2 |
             *   +-----------+----------------------------------+
             * ------------------------------------------------------------------------------------------
             */
            for(cfgBit = 0u; cfgBit < CVB_NUM_OF_PORT_T1PHY; cfgBit++)
            {
                if(0xFFu == configBit2PhyIdx[cfgBit])
                {
                    continue;
                }
                if(0u != (((uint32)1u << configBit2PhyIdx[cfgBit]) & CVB_ACTIVE_T1PHY_MASK))
                {
                    if(0u != (gpio1Cfg & ((uint32)1u << cfgBit)))
                    {
                        regVal1 |= ((uint32)1u << configBit2PhyIdx[cfgBit]);
                    }

                    if(0u != (gpio2Cfg & ((uint32)1u << cfgBit)))
                    {
                        regVal2 |= ((uint32)1u << configBit2PhyIdx[cfgBit]);
                    }
                }
            }
            /* Write "sys port mask enable wakeo of GPIOB1" to fw_dmy_0 */
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, (CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x005Eu) << 1u)), 0u, 16u, (regVal1 & 0xFFFFu)));
            /* Write "sys port mask enable wakeo of GPIOB2" to fw_dmy_1 */
            CHK_FUN_CONTINUE(retVal, cvb_reg_field_write(unit, (CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x0060u) << 1u)), 0u, 16u, (regVal2 & 0xFFFFu)));
        }

    } while(0u == 1u);

    rtl_file_close(&fp);
    return retVal;
}

/* @brief Configure the wake o config by flash reserved header.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] fileName File name
 * @retval RT_ERR_OK - Success.
 * @retval -RT_ERR_FILE_READ
 */
RtkApiRet cvb_drv_op_eth_fetch_wakeo_config(const int8 *fileName)
{
    RtkApiRet retVal             = RT_ERR_OK;
    uint32    wakeoCfg           = 0x0u;
    uint32    gpio1Cfg           = 0xFFFFu;
    uint32    gpio2Cfg           = 0xFFFFu;
    uint32    regVal1            = 0u;
    uint32    regVal2            = 0u;
    uint32    cfgBit             = 0u;
    void     *fp                 = NULL;
    uint8     wakeoGpioCfg[6]    = {0u, 0u, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
    uint8     configBit2PhyIdx[] = {2u, 3u, 4u, 5u, 8u, 9u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};

    do
    {
        retVal = rtl_file_open(&fp, 0u, fileName);
        if(fp == NULL)
        {
            retVal = -RT_ERR_FILE_READ;
            continue;
        }
        /*
         * Read Wake-out forwarding config from flash in the first-time power-on.
         * 6 bytes layout of wakeo config in flash reserved area:
         * ----------------------------------------------------------------------
         * Byte[1:0]
         *     bit[15:12]: reserved
         *     bit[11]   : wakeo_config_by_flash.
         *                 0 = wakeo config is from flash.
         *                 1 = wakeo config is not from flash
         *     bit[10]   : amt_unit_cfg
         *     bit[9:0]  : pulse_amt_cfg
         * Byte[3:2]: wakeo_en of GPIOB1
         * Byte[5:4]: wakeo_en of GPIOB2
         * ----------------------------------------------------------------------
         */
        if((int32)0 == rtl_file_read(fp, wakeoGpioCfg, CVB_WAKEO_CONFIG_FLASH_ADDRESST, 6u))
        {
            retVal = -RT_ERR_FILE_READ;
            continue;
        }
        wakeoCfg = ((uint32)wakeoGpioCfg[0] & 0xFFu) | (((uint32)wakeoGpioCfg[1] & 0xFFu) << 8u);
        gpio1Cfg = ((uint32)wakeoGpioCfg[2] & 0xFFu) | (((uint32)wakeoGpioCfg[3] & 0xFFu) << 8u);
        gpio2Cfg = ((uint32)wakeoGpioCfg[4] & 0xFFu) | (((uint32)wakeoGpioCfg[5] & 0xFFu) << 8u);
        if(0u == (wakeoCfg & 0x800u))
        {
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field((CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x005Au) << 1u)), 0u, 16u, (wakeoCfg & 0x7FFu)));
            /* RL6760 provides GPIOB1 and GPIOB2 for wakeo config.
             * ------------------------------------------------------------------------------------------
             * Config of user port mask enable wakeo on GPIOB1.
             *   [fw_dmy_0]: 16bits. (CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x005Eu) << 1u) == (0x4A0010BC)
             * Config of user port mask enable wakeo on GPIOB2.
             *   [fw_dmy_1]: 16bits. (CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x0060u) << 1u) == (0x4A0010C0)
             *
             * Config bit and user port mapping
             *   +---------+------------------------------------------------+
             *   |cfg  bit | 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0 |
             *   |user port| 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1 |
             *   +---------+------------------------------------------------+
             *
             * User-port/Sys-port/T1-PHY index mapping
             *   +-----------+----------------------------------+
             *   |user port  | 11 10 9   8  7  6  5  4  3  2  1 |
             *   |sys  port  | 12 11 10  9  8  7  6  5  4  3  2 |
             *   |T1-phy idx | -- -- -- -- --  9  8  5  4  3  2 |
             *   +-----------+----------------------------------+
             * ------------------------------------------------------------------------------------------
             */
            for(cfgBit = 0u; cfgBit < CVB_NUM_OF_PORT_T1PHY; cfgBit++)
            {
                if(0xFFu == configBit2PhyIdx[cfgBit])
                {
                    continue;
                }
                if(0u != (((uint32)1u << configBit2PhyIdx[cfgBit]) & CVB_ACTIVE_T1PHY_MASK))
                {
                    if(0u != (gpio1Cfg & ((uint32)1u << cfgBit)))
                    {
                        regVal1 |= ((uint32)1u << configBit2PhyIdx[cfgBit]);
                    }

                    if(0u != (gpio2Cfg & ((uint32)1u << cfgBit)))
                    {
                        regVal2 |= ((uint32)1u << configBit2PhyIdx[cfgBit]);
                    }
                }
            }
            /* Write "sys port mask enable wakeo of GPIOB1" to fw_dmy_0 */
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field((CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x005Eu) << 1u)), 0u, 16u, (regVal1 & 0xFFFFu)));
            /* Write "sys port mask enable wakeo of GPIOB2" to fw_dmy_1 */
            CHK_FUN_CONTINUE(retVal, cvb_drv_config_eth_write_field((CVB_PAGE_SWR_OCP | ((CVB_OFST_OP_FSM_ADDR + 0x0060u) << 1u)), 0u, 16u, (regVal2 & 0xFFFFu)));
        }

    } while(0u == 1u);

    rtl_file_close(&fp);
    return retVal;
}
