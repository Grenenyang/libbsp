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
#include "rtl907xd_va_api_serdes.h"
#include "rtl907xd_va_asicdrv_serdes.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_timer.h"
#include "rtl907xd_va_api_esm.h"
#include "rtl907xd_va_reg_list.h"
#include "rtkas_api_interface.h"
#include "rtkas_api_serdes.h"

#define DVA_WTG_LANE0_FRONTEND_CAL_APHY_COUNT (4u)
#define DVA_WTG_LANE1_FRONTEND_CAL_APHY_COUNT (4u)
#define DVA_WRAP_APHY_SG_HSG_COUNT            (25u)
#define DVA_WTG_LANE0_APHY_SG_COUNT           (16u)
#define DVA_WTG_LANE1_APHY_SG_COUNT           (18u)
#define DVA_WTG_LANE0_APHY_HSG_COUNT          (16u)
#define DVA_WTG_LANE1_APHY_HSG_COUNT          (18u)
#define DVA_WTG_LANE0_APHY_10G_COUNT          (7u)
#define DVA_WTG_LANE1_APHY_10G_COUNT          (8u)
#define DVA_WTG_LANE0_APHY_5G_COUNT           (15u)
#define DVA_WTG_LANE1_APHY_5G_COUNT           (19u)
#define DVA_SDS_RESET                         (0x0u)
#define DVA_SDS_NORMAL                        (0x1u)
#define DVA_SDS_LINK_OK                       (1u)
#define DVA_SDS_LINK_FAIL                     (0u)

static RtkApiRet dva_drv_sds_speed_get(uint32 unit, uint32 rgSdsAbility, uint32 *portSpeed)
{
    RtkApiRet ret      = RT_ERR_OK;
    uint32    sdsSpeed = 0u;

    sdsSpeed = rgSdsAbility & DVA_SDS_SPEED_MASK;

    if(DVA_SDS_SPEED_10G == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_10000M;
    }
    else if(DVA_SDS_SPEED_5G == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_5000M;
    }
    else if(DVA_SDS_SPEED_2500M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_2500M;
    }
    else if(DVA_SDS_SPEED_1000M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_1000M;
    }
    else if(DVA_SDS_SPEED_100M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_100M;
    }
    else if(DVA_SDS_SPEED_10M == sdsSpeed)
    {
        *portSpeed = (uint32)EM_PORT_SPEED_10M;
    }
    else
    {
        ret = -RT_ERR_PHY_SPEED;
    }

    return ret;
}

RtkApiRet dva_drv_sds_pcs_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet          ret             = RT_ERR_OK;
    uint32             readAction      = 0u;
    uint32             sdsDirectAddr[] = DVA_SDS_INDIRECT_ACCESS_PCS_CTRL;
    uint32             dataChannel[]   = DVA_PCS_DATA_CHANNEL;
    RegPollingConfig_t pollingConf;

    do
    {
        /* init pollingConf */
        CHK_FUN_CONTINUE(ret, drv_memset(&pollingConf, 0, sizeof(RegPollingConfig_t)));

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* set read action */
            readAction = ((page << DVA_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP));
            readAction |= (DVA_READ_DATA << DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP);

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LEN;
        }
        else if((DVA_RTK_SYS_PORT_9TH == sysPort) || (DVA_RTK_SYS_PORT_11TH == sysPort))
        {
            /* set read action */
            readAction = ((page << DVA_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_READ_DATA << DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LEN;
        }
        else if((DVA_RTK_SYS_PORT_10TH == sysPort) || (DVA_RTK_SYS_PORT_12TH == sysPort))
        {
            /* set read action */
            readAction = ((page << DVA_INDIRECT_ACCESS_SDS_INDEX_LSP) | ((reg + 0x800u) << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_READ_DATA << DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LEN;
        }
        else
        {
            /* for Misra-c violation */
            ;
        }

        pollingConf.triggerRegAddr = sdsDirectAddr[sysPort];
        pollingConf.pollingRegAddr = sdsDirectAddr[sysPort];
        pollingConf.action         = (uint32)ENABLE;
        pollingConf.expVal         = 1u;
        pollingConf.port           = 0u;
        pollingConf.eventID        = DVA_EM_EVENT_SERDES_POLLING_TIMEOUT;
        pollingConf.chkPortClk     = INDIRECT_ACCESS_REG_TARGET;

        /* set read action to register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, sdsDirectAddr[sysPort], 0u, 31u, readAction));
        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dva_drv_timer_polling_reg_timeout(unit, &pollingConf, DVA_POLLING_SDS_CHANNEL_MAX_UNIT_MICRO_SEC, DVA_POLLING_SDS_CHANNEL_MIN_UNIT_MICRO_SEC));
        /* read data*/
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_get_reg_field(unit, dataChannel[sysPort], 0u, 31u, value));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_sds_pcs_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet          ret             = RT_ERR_OK;
    uint32             regVal          = 0u;
    uint32             i               = 0u;
    uint32             writeAction     = 0u;
    uint32             sdsDirectAddr[] = DVA_SDS_INDIRECT_ACCESS_PCS_CTRL;
    uint32             dataChannel[]   = DVA_PCS_DATA_CHANNEL;
    RegPollingConfig_t pollingConf;

    do
    {
        /* init pollingConf */
        CHK_FUN_CONTINUE(ret, drv_memset(&pollingConf, 0, sizeof(RegPollingConfig_t)));

        /*read*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_pcs_direct_get(unit, sysPort, page, reg, &regVal));

        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* set write action */
            writeAction = ((page << DVA_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP));
            writeAction |= (DVA_WRITE_DATA << DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP);

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LEN;
        }
        else if((DVA_RTK_SYS_PORT_9TH == sysPort) || (DVA_RTK_SYS_PORT_11TH == sysPort))
        {
            writeAction = ((page << DVA_INDIRECT_ACCESS_SDS_INDEX_LSP) | (reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_WRITE_DATA << DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LEN;
        }
        else if((DVA_RTK_SYS_PORT_10TH == sysPort) || (DVA_RTK_SYS_PORT_12TH == sysPort))
        {
            writeAction = ((page << DVA_INDIRECT_ACCESS_SDS_INDEX_LSP) | ((reg + 0x800u) << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_WRITE_DATA << DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_PCS_CTL_RG_SDS_PCS_REG_IF_DONE_LEN;
        }
        else
        {
            /* for Misra-c violation */
            ;
        }
        /* set write data */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, dataChannel[sysPort], 0u, 31u, regVal | (value << startBit)));
        /* set write action to register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, sdsDirectAddr[sysPort], 0u, 31u, writeAction));

        pollingConf.triggerRegAddr = sdsDirectAddr[sysPort];
        pollingConf.pollingRegAddr = sdsDirectAddr[sysPort];
        pollingConf.action         = (uint32)ENABLE;
        pollingConf.expVal         = 1u;
        pollingConf.port           = 0u;
        pollingConf.eventID        = DVA_EM_EVENT_SERDES_POLLING_TIMEOUT;
        pollingConf.chkPortClk     = INDIRECT_ACCESS_REG_TARGET;

        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dva_drv_timer_polling_reg_timeout(unit, &pollingConf, DVA_POLLING_SDS_CHANNEL_MAX_UNIT_MICRO_SEC, DVA_POLLING_SDS_CHANNEL_MIN_UNIT_MICRO_SEC));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_sds_aphy_direct_get(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 *value)
{
    RtkApiRet          ret             = RT_ERR_OK;
    uint32             readAction      = 0u;
    uint32             sdsDirectAddr[] = DVA_SDS_INDIRECT_ACCESS_DPHY_CTRL;
    uint32             dataChannel[]   = DVA_DPHY_DATA_CHANNEL;
    RegPollingConfig_t pollingConf;

    do
    {
        /* init pollingConf */
        CHK_FUN_CONTINUE(ret, drv_memset(&pollingConf, 0, sizeof(RegPollingConfig_t)));

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            readAction = ((reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_READ_DATA << DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LEN;
        }
        else if((DVA_RTK_SYS_PORT_9TH <= sysPort) && (DVA_RTK_SYS_PORT_12TH >= sysPort))
        {
            readAction = ((reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_READ_DATA << DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LEN;
        }
        else
        {
            /* for Misra-c violation */
            ;
        }

        /* set read action to register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, sdsDirectAddr[sysPort], 0u, 31u, readAction));

        pollingConf.triggerRegAddr = sdsDirectAddr[sysPort];
        pollingConf.pollingRegAddr = sdsDirectAddr[sysPort];
        pollingConf.action         = (uint32)ENABLE;
        pollingConf.expVal         = 1u;
        pollingConf.port           = 0u;
        pollingConf.eventID        = DVA_EM_EVENT_SERDES_POLLING_TIMEOUT;
        pollingConf.chkPortClk     = INDIRECT_ACCESS_REG_TARGET;

        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dva_drv_timer_polling_reg_timeout(unit, &pollingConf, DVA_POLLING_SDS_CHANNEL_MAX_UNIT_MICRO_SEC, DVA_POLLING_SDS_CHANNEL_MIN_UNIT_MICRO_SEC));

        /* read data*/
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_get_reg_field(unit, dataChannel[sysPort], 0u, 31u, value));

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_sds_aphy_direct_set(uint32 unit, RtkPort sysPort, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    RtkApiRet          ret             = RT_ERR_OK;
    uint32             i               = 0u;
    uint32             regVal          = 0u;
    uint32             writeAction     = 0u;
    uint32             sdsDirectAddr[] = DVA_SDS_INDIRECT_ACCESS_DPHY_CTRL;
    uint32             dataChannel[]   = DVA_DPHY_DATA_CHANNEL;
    RegPollingConfig_t pollingConf;

    do
    {
        /* init pollingConfig */
        CHK_FUN_CONTINUE(ret, drv_memset(&pollingConf, 0, sizeof(RegPollingConfig_t)));

        /*read*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_aphy_direct_get(unit, sysPort, page, reg, &regVal));

        /*clear*/
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            regVal &= ~((uint32)0x1u << (startBit + i));
        }

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            writeAction = ((reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_WRITE_DATA << DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_SDS_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LEN;
        }
        else if((DVA_RTK_SYS_PORT_9TH <= sysPort) && (DVA_RTK_SYS_PORT_12TH >= sysPort))
        {
            writeAction = ((reg << DVA_INDIRECT_ACCESS_SDS_REGADDR_LSP) | (DVA_WRITE_DATA << DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_WE_LSP));

            pollingConf.triggerRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LSP;
            pollingConf.triggerRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_CE_LEN;
            pollingConf.pollingRegLsp = DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LSP;
            pollingConf.pollingRegLen = DVA_PORT0_INDIRECT_ACCESS_WTG_DPHY_CTL_RG_SDS_DPHY_REG_IF_DONE_LEN;
        }
        else
        {
            /* for Misra-c violation */
            ;
        }

        /* set write data */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, dataChannel[sysPort], 0u, 31u, regVal | (value << startBit)));
        /* set write action to register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, sdsDirectAddr[sysPort], 0u, 31u, writeAction));

        pollingConf.triggerRegAddr = sdsDirectAddr[sysPort];
        pollingConf.pollingRegAddr = sdsDirectAddr[sysPort];
        pollingConf.action         = (uint32)ENABLE;
        pollingConf.expVal         = 1u;
        pollingConf.port           = 0u;
        pollingConf.eventID        = DVA_EM_EVENT_SERDES_POLLING_TIMEOUT;
        pollingConf.chkPortClk     = INDIRECT_ACCESS_REG_TARGET;

        /* polling done bit */
        CHK_FUN_CONTINUE(ret, dva_drv_timer_polling_reg_timeout(unit, &pollingConf, DVA_POLLING_SDS_CHANNEL_MAX_UNIT_MICRO_SEC, DVA_POLLING_SDS_CHANNEL_MIN_UNIT_MICRO_SEC));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_is_wtg_serdes_mode(uint32 comboIntf, uint32 sdsMode)
{
    RtkApiRet ret = RT_ERR_OK;
    if(((COMBO_INTF_SGMII_HISGMII == comboIntf) && ((DVA_SDS_MODE_SG == sdsMode) || (DVA_SDS_MODE_HSG == sdsMode))) || ((COMBO_INTF_XGMII_USXG == comboIntf) && ((DVA_SDS_MODE_USXG == sdsMode) || (DVA_SDS_MODE_KR_XFI_5G == sdsMode) || (DVA_SDS_MODE_KR_XFI_10G == sdsMode))))
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

static RtkApiRet dva_drv_sds_is_serdes_mode(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret       = RT_ERR_OK;
    uint32    comboIntf = 0u;
    uint32    sdsMode   = 0u;
    RtkPort   sysPort   = dva_g_userPort2sysPort[userPort];
    do
    {
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_PORT_CTRL0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LSP, DVA_PORT0_PORT_CTRL0_RG_INTF_CB_CFG_LEN, &comboIntf));
        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* get SerDes mode */
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, &sdsMode));
            if((COMBO_INTF_SGMII_HISGMII != comboIntf) || ((DVA_SDS_MODE_SG != sdsMode) && (DVA_SDS_MODE_HSG != sdsMode)))
            {
                ret = -RT_ERR_SDS_MODE;
            }
        }
        else
        {
            /* get SerDes mode */
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, &sdsMode));
            CHK_FUN_CONTINUE(ret, dva_drv_sds_is_wtg_serdes_mode(comboIntf, sdsMode));
        }
    } while(0u == 1u);
    return ret;
}

RtkApiRet dva_drv_sds_chk_port_range(RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    if((DVA_RTK_USER_PORT_4TH != userPort) && (DVA_RTK_USER_PORT_5TH != userPort) && (DVA_RTK_USER_PORT_6TH != userPort) && (DVA_RTK_USER_PORT_7TH != userPort) && (DVA_RTK_USER_PORT_8TH != userPort) && (DVA_RTK_USER_PORT_9TH != userPort) && (DVA_RTK_USER_PORT_10TH != userPort) && (DVA_RTK_USER_PORT_11TH != userPort) && (DVA_RTK_USER_PORT_12TH != userPort))
    {
        ret = -RT_ERR_PORT_ID;
    }

    return ret;
}

RtkApiRet dva_drv_sds_is_serdes_port(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    ret = dva_drv_sds_chk_port_range(userPort);

    if(RT_ERR_OK == ret)
    {
        ret = dva_drv_sds_is_serdes_mode(unit, userPort);
    }

    return ret;
}

RtkApiRet dva_drv_sds_aphy_covert_port(uint32 portType, RtkPort *sysPort)
{
    RtkApiRet ret = -RT_ERR_FAILED;

    if(DVA_SDS_APHY_TYPE == portType)
    {
        /* Convert APHY user port 10 to port 9 */
        if(DVA_RTK_USER_PORT_10TH == *sysPort)
        {
            *sysPort = DVA_RTK_USER_PORT_9TH;
        }
        /* Convert APHY user port 12  to port 11 */
        if(DVA_RTK_USER_PORT_12TH == *sysPort)
        {
            *sysPort = DVA_RTK_USER_PORT_11TH;
        }

        ret = RT_ERR_OK;
    }

    return ret;
}

/**
 * @brief A function used to get SERDES PHY speed
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sysPort                             Switch port (DVA_SYS_PORT_4H to DVA_RTK_SYS_PORT_12TH)  \n
 *                                                System port number of the switch
 * @param[out] portSpeed                          Port speed of SEDES mode (DVA_SDS_SPEED_10M to DVA_SDS_SPEED_10G)  \n
 *                                                Pointer of read back PHY speed
 * @retval RT_ERR_OK                              API gets value of PHY speed Successfully.
 * @retval -RT_ERR_PHY_SPEED                      The interface obtains the incorrect port speed.
 */
RtkApiRet dva_drv_sds_port_speed_get(uint32 unit, RtkPort sysPort, uint32 *portSpeed)
{
    RtkApiRet ret          = RT_ERR_OK;
    RtkPort   userPort     = dva_g_sysPort2userPort[sysPort];
    uint32    rgSdsAbility = 0u;

    do
    {
        /* check is serdes port */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_is_serdes_port(unit, userPort));

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* get WRAP SERDES ability */
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_SDS_CTRL_REG_2 + (PERPORT_REG_OFFSET * sysPort), DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LSP, DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LEN, &rgSdsAbility));
        }
        else
        {
            /* get WTG SERDES ability */
            CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_WTG_CTRL_REG_1 + (PERPORT_REG_OFFSET * sysPort), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, &rgSdsAbility));
        }

        /* get SERDES speed */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_speed_get(unit, rgSdsAbility, portSpeed));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_sds_chk_xfi_or_kr(uint32 unit, RtkPort userPort, RtkModeExt_e *result)
{
    RtkApiRet ret     = -RT_ERR_FAILED;
    uint32    sdsMode = 0u;
    uint32    Nway    = 0u;

    do
    {
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            *result = EM_MODE_EXT_RESERVED;
            ret     = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_PORT0_WTG_CTRL_REG_0 + (userPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, &sdsMode));

        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_getbits_pcs(unit, userPort, 19u, 0u, 12u, 12u, &Nway));

        if(DVA_SDS_MODE_KR_XFI_10G == sdsMode)
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
        else if(DVA_SDS_MODE_KR_XFI_5G == sdsMode)
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

RtkApiRet dva_drv_sds_wrap_serdes_init(uint32 unit, RtkPort port)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /* Check if the input port are valid */
        if((DVA_RTK_USER_PORT_4TH > port) || (DVA_RTK_USER_PORT_8TH < port))
        {
            ret = -RT_ERR_PORT_ID;
        }

        /* Covert user port to system port */
        sysPort = dva_g_userPort2sysPort[port];

        /* De-assert rst_sds_b  */
        if(DVA_RTK_SYS_PORT_4TH == sysPort)
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG0_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG0_RST_SDS_B_LEN, ENABLED));
        }
        else if(DVA_RTK_SYS_PORT_5TH == sysPort)
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG1_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG1_RST_SDS_B_LEN, ENABLED));
        }
        else if(DVA_RTK_SYS_PORT_6TH == sysPort)
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG2_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG2_RST_SDS_B_LEN, ENABLED));
        }
        else if(DVA_RTK_SYS_PORT_7TH == sysPort)
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG3_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG3_RST_SDS_B_LEN, ENABLED));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_reg_field_write(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG4_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG4_RST_SDS_B_LEN, ENABLED));
        }

    } while(0u == 1u);

    return ret;
}

/******************************************************************************************* */
static RtkApiRet dva_drv_sds_chk_link(uint32 unit, uint32 userPort, RtkModeExt_e serdesMode, uint32 *linkStatus)
{
    static SysSdsPcsRegConfig_t sysSdsRegGetbitsPcs = {0};
    UnitChip_t                  unitChip            = {.chip = CHIP_907XD_VA, .unit = unit};
    RtkApiRet                   ret                 = RT_ERR_OK;

    do
    {
        if((EM_MODE_EXT_SGMII == serdesMode) || (EM_MODE_EXT_HISGMII == serdesMode))
        {
            /* check SG or HSG mode link on serdes ports */
            sysSdsRegGetbitsPcs.port     = userPort;
            sysSdsRegGetbitsPcs.page     = 1u;
            sysSdsRegGetbitsPcs.reg      = 29u;
            sysSdsRegGetbitsPcs.startBit = 4u;
            sysSdsRegGetbitsPcs.endBit   = 4u;
        }
        else
        {
            /* check USXG/KR/XFI mode link on serdes port */
            sysSdsRegGetbitsPcs.port     = userPort;
            sysSdsRegGetbitsPcs.page     = 5u;
            sysSdsRegGetbitsPcs.reg      = 0u;
            sysSdsRegGetbitsPcs.startBit = 12u;
            sysSdsRegGetbitsPcs.endBit   = 12u;
        }

        CHK_FUN_CONTINUE(ret, rtk_sys_sds_reg_getbits_pcs(unitChip, &sysSdsRegGetbitsPcs));

        if(sysSdsRegGetbitsPcs.value == DVA_SDS_LINK_OK)
        {
            *linkStatus = DVA_SDS_LINK_OK;
        }
        else
        {
            *linkStatus = DVA_SDS_LINK_FAIL;
        }
    } while(0u == 1u);
    return ret;
}

RtkApiRet dva_drv_sds_5gmac_link_set(uint32 unit, uint32 userPort, RtkModeExt_e serdesMode, uint32 linkStatus)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sdsLink = 0u;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_sds_chk_link(unit, userPort, serdesMode, &sdsLink));

        if(DVA_SDS_LINK_OK == sdsLink)
        {
            if((EM_MODE_EXT_USXGMII <= serdesMode) && (EM_MODE_EXT_10G_XFI >= serdesMode))
            {
                switch(userPort)
                {
                    case DVA_RTK_USER_PORT_9TH:
                        /* 5g mac force link  for port 9 */
                        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT9_L2_MISC, DVA_PORT9_L2_MISC_FORCE_LINK_5G_LSP, DVA_PORT9_L2_MISC_FORCE_LINK_5G_LEN, linkStatus));
                        break;
                    case DVA_RTK_USER_PORT_10TH:
                        /* 5g mac force link for port 10 */
                        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT10_L2_MISC, DVA_PORT10_L2_MISC_FORCE_LINK_5G_LSP, DVA_PORT10_L2_MISC_FORCE_LINK_5G_LEN, linkStatus));
                        break;
                    case DVA_RTK_USER_PORT_11TH:
                        /* 5g mac force link  for port 11 */
                        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT11_L2_MISC, DVA_PORT11_L2_MISC_FORCE_LINK_5G_LSP, DVA_PORT11_L2_MISC_FORCE_LINK_5G_LEN, linkStatus));
                        break;
                    case DVA_RTK_USER_PORT_12TH:
                        /* 5g mac force link  for port 12 */
                        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT12_L2_MISC, DVA_PORT12_L2_MISC_FORCE_LINK_5G_LSP, DVA_PORT12_L2_MISC_FORCE_LINK_5G_LEN, linkStatus));
                        break;
                    default:
                        /*do nothing here*/
                        break;
                }
            }
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_sds_1gmac_link_set(uint32 unit, uint32 userPort, RtkModeExt_e serdesMode, uint32 linkStatus)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;
    uint32    sdsLink = 0u;
    sysPort           = dva_g_userPort2sysPort[userPort];

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_sds_chk_link(unit, userPort, serdesMode, &sdsLink));

        if(DVA_SDS_LINK_OK == sdsLink)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_FORCE_LINK_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_FORCE_LINK_LEN, linkStatus));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_wrap_main_reset(uint32 unit, uint32 userPort, uint32 status)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_8TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* rst_sds_b */
        if(DVA_RTK_USER_PORT_4TH == userPort)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG0_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG0_RST_SDS_B_LEN, status));
        }
        else if(DVA_RTK_USER_PORT_5TH == userPort)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG1_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG1_RST_SDS_B_LEN, status));
        }
        else if(DVA_RTK_USER_PORT_6TH == userPort)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG2_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG2_RST_SDS_B_LEN, status));
        }
        else if(DVA_RTK_USER_PORT_7TH == userPort)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG3_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG3_RST_SDS_B_LEN, status));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_SG4_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_SG4_RST_SDS_B_LEN, status));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_wtg_main_reset(uint32 unit, uint32 userPort, uint32 status)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {

        if((DVA_RTK_USER_PORT_9TH <= userPort) && (DVA_RTK_USER_PORT_12TH >= userPort))
        {
            if(DVA_RTK_USER_PORT_9TH == userPort)
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_USXG1_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_USXG1_RST_SDS_B_LEN, status));
            }
            else if(DVA_RTK_USER_PORT_10TH == userPort)
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL1, DVA_RESET_GLOBAL_CONTROL1_USXG1_RST_SDS1_B_LSP, DVA_RESET_GLOBAL_CONTROL1_USXG1_RST_SDS1_B_LEN, status));
            }
            else if(DVA_RTK_USER_PORT_11TH == userPort)
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL2, DVA_RESET_GLOBAL_CONTROL2_USXG2_RST_SDS_B_LSP, DVA_RESET_GLOBAL_CONTROL2_USXG2_RST_SDS_B_LEN, status));
            }
            else
            {
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_RESET_GLOBAL_CONTROL1, DVA_RESET_GLOBAL_CONTROL1_USXG2_RST_SDS1_B_LSP, DVA_RESET_GLOBAL_CONTROL1_USXG2_RST_SDS1_B_LEN, status));
            }
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_main_reset(uint32 unit, RtkPort userPort, uint32 status)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(DVA_SDS_NORMAL == status)
        {
            /* delay 10us */
            CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));
        }

        if(DVA_RTK_USER_PORT_8TH >= userPort)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_wrap_main_reset(unit, userPort, status));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_wtg_main_reset(unit, userPort, status));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_front_end_calibration(uint32 unit, RtkPort userPort)
{
    RtkApiRet     ret                                                                = RT_ERR_OK;
    uint32        i                                                                  = 0u;
    static uint32 dva_wtglane0FrontendCalAddr[DVA_WTG_LANE0_FRONTEND_CAL_APHY_COUNT] = {0x1026u, 0x1126u, 0x1226u, 0x1326u};
    static uint32 dva_wtglane1FrontendCalAddr[DVA_WTG_LANE1_FRONTEND_CAL_APHY_COUNT] = {0x10a6u, 0x11a6u, 0x12a6u, 0x13a6u};

    do
    {
        /*Front-End Calibration*/
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, 0x1A2Au, 0u, 15u, 0x0u));

        if((DVA_RTK_USER_PORT_9TH == userPort) || (DVA_RTK_USER_PORT_11TH == userPort))
        {
            for(i = 0u; i < DVA_WTG_LANE0_FRONTEND_CAL_APHY_COUNT; i++)
            {
                /*bit [15] = 1*/
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane0FrontendCalAddr[i], 0u, 15u, 0xC040u));
            }
        }
        else
        {
            for(i = 0u; i < DVA_WTG_LANE1_FRONTEND_CAL_APHY_COUNT; i++)
            {
                /*bit [15] = 1*/
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane1FrontendCalAddr[i], 0u, 15u, 0xC040u));
            }
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_5G_aphy_para_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet     ret                                                 = RT_ERR_OK;
    uint32        i                                                   = 0u;
    static uint32 dva_wtglane0AphyAddr5G[DVA_WTG_LANE0_APHY_5G_COUNT] = {0x1228u, 0x122Au, 0x122Cu, 0x1220u, 0x1216u, 0x1222u,
                                                                         0x121Cu, 0x1236u, 0x1204u, 0x001Au, 0x0018u, 0x001Eu, 0x0012u, 0x000Au, 0x0314u};
    static uint32 dva_wtglane0AphyVal5G[DVA_WTG_LANE0_APHY_5G_COUNT]  = {0x4020u, 0xA0A0u, 0x8020u, 0xC07Eu, 0x01F8u, 0xB87Fu,
                                                                         0x0000u, 0x800Fu, 0x1343u, 0x0380u, 0x3D10u, 0x058Du, 0x807Cu, 0x0500u, 0x7478u};
    static uint32 dva_wtglane1AphyAddr5G[DVA_WTG_LANE1_APHY_5G_COUNT] = {0x12A8u, 0x12AAu, 0x12ACu, 0x12A0u, 0x1296u, 0x12A2u, 0x129Cu,
                                                                         0x12B6u, 0x1284u, 0x032Cu, 0x0330u, 0x005Au, 0x0058u, 0x005Eu,
                                                                         0x004Au, 0x0354u, 0x0052u, 0x0B42u, 0x0B44u};
    static uint32 dva_wtglane1AphyVal5G[DVA_WTG_LANE1_APHY_5G_COUNT]  = {0x4020u, 0xA0A0u, 0x8020u, 0xC07Eu, 0x01F8u, 0xB87Fu, 0x0000u,
                                                                         0x800Fu, 0x1343u, 0x0701u, 0xC401u, 0x0380u, 0x3d10u, 0x058Du,
                                                                         0x0500u, 0x7478u, 0x807Cu, 0x06F2u, 0x06F2u};
    do
    {
        /*Front-End Calibration*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_front_end_calibration(unit, userPort));

        if((DVA_RTK_USER_PORT_9TH == userPort) || (DVA_RTK_USER_PORT_11TH == userPort))
        {
            /* LANE0 aphy config */
            for(i = 0u; i < DVA_WTG_LANE0_APHY_5G_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane0AphyAddr5G[i], 0u, 15u, dva_wtglane0AphyVal5G[i]));
            }
        }
        else if((DVA_RTK_USER_PORT_10TH == userPort) || (DVA_RTK_USER_PORT_12TH == userPort))
        {
            /* LANE1 aphy config */
            for(i = 0u; i < DVA_WTG_LANE0_APHY_5G_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane1AphyAddr5G[i], 0u, 15u, dva_wtglane1AphyVal5G[i]));
            }
        }
        else
        {
            /*do nothing*/
            ;
        }
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sds_10G_aphy_para_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet     ret                                                   = RT_ERR_OK;
    uint32        i                                                     = 0u;
    static uint32 dva_wtglane0AphyAddr10G[DVA_WTG_LANE0_APHY_10G_COUNT] = {0x1814u, 0x001Au, 0x001Cu, 0x0018u, 0x001Eu, 0x0012u, 0x000Au};
    static uint32 dva_wtglane0AphyVal10G[DVA_WTG_LANE0_APHY_10G_COUNT]  = {0x8400u, 0x4390u, 0x3D00u, 0x3D10u, 0x058Du, 0x807Cu, 0x0503u};
    static uint32 dva_wtglane1AphyAddr10G[DVA_WTG_LANE1_APHY_10G_COUNT] = {0x1894u, 0x0454u, 0x005Au, 0x005Cu, 0x0058u, 0x005Eu, 0x0052u, 0x004Au};
    static uint32 dva_wtglane1AphyVal10G[DVA_WTG_LANE1_APHY_10G_COUNT]  = {0x8400u, 0x3478u, 0x4390u, 0x3D00u, 0x3D10u, 0x058Du, 0x807Cu, 0x0503u};

    do
    {
        /*Front-End Calibration*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_front_end_calibration(unit, userPort));

        if((DVA_RTK_USER_PORT_9TH == userPort) || (DVA_RTK_USER_PORT_11TH == userPort))
        {
            /* LANE0 aphy config */
            for(i = 0u; i < DVA_WTG_LANE0_APHY_10G_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane0AphyAddr10G[i], 0u, 15u, dva_wtglane0AphyVal10G[i]));
            }
        }
        else if((DVA_RTK_USER_PORT_10TH == userPort) || (DVA_RTK_USER_PORT_12TH == userPort))
        {
            /* LANE1 aphy config */
            for(i = 0u; i < DVA_WTG_LANE1_APHY_10G_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane1AphyAddr10G[i], 0u, 15u, dva_wtglane1AphyVal10G[i]));
            }
        }
        else
        {
            /*do nothing*/
            ;
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_wtg_hsg_aphy_para_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet     ret                                                   = RT_ERR_OK;
    uint32        i                                                     = 0u;
    static uint32 dva_wtglane0AphyAddrHsg[DVA_WTG_LANE0_APHY_HSG_COUNT] = {0x1128u, 0x112Au, 0x112Cu, 0x1120u, 0x1116u, 0x1122u,
                                                                           0x111Cu, 0x1136u, 0x1104u, 0x0138u, 0x013Cu, 0x001Au,
                                                                           0x001Cu, 0x0018u, 0x001Eu, 0x000Au};
    static uint32 dva_wtglane0AphyValHsg[DVA_WTG_LANE0_APHY_HSG_COUNT]  = {0x4020u, 0xA0A0u, 0x8020u, 0xC07Eu, 0x01F8u, 0xB87Fu,
                                                                           0x0000u, 0x80AFu, 0x1443u, 0x2D08u, 0x2480u, 0x0390u,
                                                                           0x3900u, 0x3D10u, 0x058Du, 0x0500u};
    static uint32 dva_wtglane1AphyAddrHsg[DVA_WTG_LANE1_APHY_HSG_COUNT] = {0x11A8u, 0x11AAu, 0x11ACu, 0x11A0u, 0x1196u, 0x11A2u,
                                                                           0x119Cu, 0x11B6u, 0x012Cu, 0x0130u, 0x1184u, 0x005Au,
                                                                           0x005Cu, 0x0058u, 0x005Eu, 0x004Au, 0x0942u, 0x0944u};
    static uint32 dva_wtglane1AphyValHsg[DVA_WTG_LANE1_APHY_HSG_COUNT]  = {0x4020u, 0xA0A0u, 0x8020u, 0xC07Eu, 0x01F8u, 0xB87Fu,
                                                                           0x0000u, 0x80AFu, 0x0301u, 0x4400u, 0x1443u, 0x0390u,
                                                                           0x3900u, 0x3D10u, 0x058Du, 0x0500u, 0x0651u, 0x0651u};
    do
    {
        /*Front-End Calibration*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_front_end_calibration(unit, userPort));

        if((DVA_RTK_USER_PORT_9TH == userPort) || (DVA_RTK_USER_PORT_11TH == userPort))
        {
            /* LANE0 aphy config */
            for(i = 0u; i < DVA_WTG_LANE0_APHY_HSG_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane0AphyAddrHsg[i], 0u, 15u, dva_wtglane0AphyValHsg[i]));
            }
        }
        else if((DVA_RTK_USER_PORT_10TH == userPort) || (DVA_RTK_USER_PORT_12TH == userPort))
        {
            /* LANE1 aphy config */
            for(i = 0u; i < DVA_WTG_LANE0_APHY_HSG_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane1AphyAddrHsg[i], 0u, 15u, dva_wtglane1AphyValHsg[i]));
            }
        }
        else
        {
            /*do nothing*/
            ;
        }

    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sds_wrap_sg_hsg_aphy_para_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet     ret                                               = RT_ERR_OK;
    uint32        i                                                 = 0u;
    static uint32 dva_wrapAphyAddrSgHsg[DVA_WRAP_APHY_SG_HSG_COUNT] = {0x0033u, 0x00B3u, 0x0023u, 0x00A3u, 0x0020u, 0x0021u,
                                                                       0x00A0u, 0x00A1u, 0x0009u, 0x0089u, 0x000Bu, 0x008Bu,
                                                                       0x000Au, 0x008Au, 0x0001u, 0x0081u, 0x0024u, 0x00A4u,
                                                                       0x0050u, 0x00D0u, 0x00c6u, 0x00c7u, 0x0046u, 0x0047u, 0x0050u};
    static uint32 dva_wrapAphyValSgHsg[DVA_WRAP_APHY_SG_HSG_COUNT]  = {0x9542u, 0x9542u, 0x0B67u, 0x0B67u, 0xD4A4u, 0x082Au,
                                                                       0xD4A4u, 0x184Au, 0x620Cu, 0x620Cu, 0xB905u, 0xB905u,
                                                                       0xa268u, 0xa268u, 0xA84Eu, 0xA84Au, 0x4210u, 0x4210u,
                                                                       0xB208u, 0xBA08u, 0x064Du, 0x064Du, 0x050Fu, 0x050Fu, 0xB288u};
    do
    {
        /* APHY config */
        for(i = 0u; i < DVA_WRAP_APHY_SG_HSG_COUNT; i++)
        {
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wrapAphyAddrSgHsg[i], 0u, 15u, dva_wrapAphyValSgHsg[i]));
        }

    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sds_wtg_sg_aphy_para_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet     ret                                                 = RT_ERR_OK;
    uint32        i                                                   = 0u;
    static uint32 dva_wtglane0AphyAddrSg[DVA_WTG_LANE0_APHY_SG_COUNT] = {0x1028u, 0x102Au, 0x102Cu, 0x1020u, 0x1016u, 0x1022u,
                                                                         0x101Cu, 0x1036u, 0x1004u, 0x0038u, 0x003Cu, 0x001Au,
                                                                         0x001Cu, 0x0018u, 0x001Eu, 0x000Au};
    static uint32 dva_wtglane0AphyValSg[DVA_WTG_LANE0_APHY_SG_COUNT]  = {0x4020u, 0xA0A0u, 0x8020u, 0xC07Eu, 0x01F8u, 0xB87Fu,
                                                                         0x0000u, 0x80CFu, 0x1483u, 0x6119u, 0x3300u, 0x0390u,
                                                                         0x3900u, 0x3D10u, 0x058Du, 0x0500u};
    static uint32 dva_wtglane1AphyAddrSg[DVA_WTG_LANE1_APHY_SG_COUNT] = {0x10A8u, 0x10AAu, 0x10ACu, 0x10A0u, 0x1096u, 0x10A2u,
                                                                         0x109Cu, 0x10B6u, 0x002Cu, 0x0030u, 0x1084u, 0x005Au,
                                                                         0x005Cu, 0x0058u, 0x005Eu, 0x004Au, 0x0842u, 0x0844u};
    static uint32 dva_wtglane1AphyValSg[DVA_WTG_LANE1_APHY_SG_COUNT]  = {0x4020u, 0xA0A0u, 0x8020u, 0xC07Eu, 0x01F8u, 0xB87Fu,
                                                                         0x0000u, 0x80CFu, 0x4302u, 0x5448u, 0x1483u, 0x0390u,
                                                                         0x3900u, 0x3D10u, 0x058Du, 0x0500u, 0x050Cu, 0x050Cu};
    do
    {
        /*Front-End Calibration*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_front_end_calibration(unit, userPort));

        if((DVA_RTK_USER_PORT_9TH == userPort) || (DVA_RTK_USER_PORT_11TH == userPort))
        {
            /* LANE0 aphy config */
            for(i = 0u; i < DVA_WTG_LANE0_APHY_SG_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane0AphyAddrSg[i], 0u, 15u, dva_wtglane0AphyValSg[i]));
            }
        }
        else if((DVA_RTK_USER_PORT_10TH == userPort) || (DVA_RTK_USER_PORT_12TH == userPort))
        {
            /* LANE1 aphy config */
            for(i = 0u; i < DVA_WTG_LANE1_APHY_SG_COUNT; i++)
            {
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_aphy(unit, userPort, 0u, dva_wtglane1AphyAddrSg[i], 0u, 15u, dva_wtglane1AphyValSg[i]));
            }
        }
        else
        {
            /*do nothing*/
            ;
        }
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sds_10gxfi_phy_role_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if(DVA_SPEED_10000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_KR_XFI_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_10000M_FULL));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, 0x1u));
        /*main reset*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_5gxfi_phy_role_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if(DVA_SPEED_5000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_KR_XFI_5G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_5000M_FULL));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, 0x1u));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_10gkr_mac_role_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_KR_XFI_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_MAC_MODE));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_10gkr_phy_role_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if(DVA_SPEED_10000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_KR_XFI_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_10000M_FULL));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, 0x1u));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_5gkr_mac_role_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_KR_XFI_5G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_MAC_MODE));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_5gkr_phy_role_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if(DVA_SPEED_5000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_KR_XFI_5G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_5000M_FULL));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, 0x1u));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_usxgmii_phy_role_speed_set(uint32 unit, RtkPort sysPort, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(DVA_SPEED_10000MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_USXG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_10000M_FULL));
        }
        else if(DVA_SPEED_5000MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_USXG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_5000M_FULL));
        }
        /*2.5G speed*/
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_USXG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_2500M_FULL));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_usxgmii_phy_role_set(uint32 unit, RtkPort userPort, uint32 speed, uint32 portSpeed)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if((DVA_SPEED_5000MBPS != speed) && (DVA_SPEED_2500MBPS != speed) && (DVA_SPEED_10000MBPS != speed))
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        if((DVA_SPEED_5000MBPS != portSpeed) && (DVA_SPEED_10000MBPS != portSpeed))
        {
            ret = -RT_ERR_SDS_SPEED_MISMACH;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        CHK_FUN_CONTINUE(ret, dva_drv_sds_usxgmii_phy_role_speed_set(unit, sysPort, speed));

        if(DVA_SPEED_10000MBPS == portSpeed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_5G));
        }

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, 0x1u));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_wtg_sgmii_phy_role_set(uint32 unit, RtkPort sysPort, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_SG));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));

        if(DVA_SPEED_1000MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_1000M_FULL));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        }
        else if(DVA_SPEED_100MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_100M_FULL));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_100M_SPEED_FULL_DUPLEX));
        }
        /*10M speed*/
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_10M_FULL));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_10M_SPEED_FULL_DUPLEX));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_wrap_sgmii_phy_role_set(uint32 unit, RtkPort sysPort, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, DVA_SDS_MODE_SG));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));

        if(DVA_SPEED_1000MBPS == speed)
        {
            /* Config PHY Mode */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LSP, DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LEN, DVA_SDS_SPEED_1000M_FULL));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        }
        else if(DVA_SPEED_100MBPS == speed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LSP, DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LEN, DVA_SDS_SPEED_100M_FULL));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_100M_SPEED_FULL_DUPLEX));
        }
        /*10M speed*/
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LSP, DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LEN, DVA_SDS_SPEED_10M_FULL));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_10M_SPEED_FULL_DUPLEX));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_sgmii_phy_role_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(4 ~ 12)*/
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if((DVA_SPEED_1000MBPS != speed) && (DVA_SPEED_100MBPS != speed) && (DVA_SPEED_10MBPS != speed))
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_wrap_sgmii_phy_role_set(unit, sysPort, speed));
        }
        else
        {
            /* bit[10] = 1 */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x055Au));

            CHK_FUN_CONTINUE(ret, dva_drv_sds_wtg_sgmii_phy_role_set(unit, sysPort, speed));
        }

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, 0x1u));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_sgmii_mac_role_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(4 ~ 12)*/
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, DVA_SDS_MODE_SG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_MAC_MODE));
        }
        else
        {
            /* bit[10] = 1 */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x055Au));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_SG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_MAC_MODE));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_hisgmii_phy_role_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(4 ~ 12)*/
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if(DVA_SPEED_2500MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            /* Config PHY Mode */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, DVA_SDS_MODE_HSG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_2 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LSP, DVA_PORT0_SDS_CTRL_REG_2_RG_SDS0_ABLTY_LEN, DVA_SDS_SPEED_2500M_FULL));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_HSG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_PHY_MODE));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_1 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LSP, DVA_PORT0_WTG_CTRL_REG_1_RG_SDS01_SDS0_ABLTY_LEN, DVA_SDS_SPEED_2500M_FULL));
        }

        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LSP, DVA_PORT0_PROPERTY_CONFIGURE_MEDIA_CAPABILITY_LEN, AN_1000M_SPEED_FULL_DUPLEX));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_PROPERTY_CONFIGURE + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_MAC_FORCE_LEN, 0x1u));

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_hisgmii_mac_role_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    sysPort = 0u;

    do
    {
        /*error handler for port check, which port number should be serdes user port(4 ~ 12)*/
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        sysPort = dva_g_userPort2sysPort[userPort];

        if((DVA_RTK_SYS_PORT_4TH <= sysPort) && (DVA_RTK_SYS_PORT_8TH >= sysPort))
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_MODE0_LEN, DVA_SDS_MODE_HSG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_SDS_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_SDS_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_MAC_MODE));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DVA_SDS_MODE_HSG));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_USX_SUB_MX_LEN, DVA_ANALOG_SDS_10G));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_PORT0_WTG_CTRL_REG_0 + (sysPort * PERPORT_REG_OFFSET), DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LSP, DVA_PORT0_WTG_CTRL_REG_0_RG_SDS_PHY_MODE_LEN, DVA_MAC_MODE));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_10gxfi_config_set(uint32 unit, RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /*todo: error handle for portRole check, which port role should be mac or phy role*/
        if(DVA_PHY_MODE < portRole)
        {
            ret = -RT_ERR_SDS_PORT_ROLE;
            continue;
        }

        if(DVA_SPEED_10000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        /* bit [9] = 1, bit[10] = 1 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x075Au));

        if(DVA_FORCE_MODE == isForceMode)
        {
            /* bit 12 set 0 to disable nway */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 0u, 0u, 15u, 0x0u));
        }

        /* APHY parameters */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_10G_aphy_para_set(unit, userPort));

        if(DVA_PHY_MODE == portRole)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_10gxfi_phy_role_set(unit, userPort, speed));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_5gxfi_config_set(uint32 unit, RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /*todo: error handle for portRole check, which port role should be mac or phy role*/
        if((DVA_PHY_MODE != portRole) && (DVA_MAC_MODE != portRole))
        {
            ret = -RT_ERR_SDS_PORT_ROLE;
            continue;
        }

        if(DVA_SPEED_5000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        /* bit [9] = 1, bit[10] = 1 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x075Au));

        if(DVA_FORCE_MODE == isForceMode)
        {
            /* bit 12 set 0 to disable nway */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 0u, 0u, 15u, 0x0u));
        }

        /* APHY parameters */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_5G_aphy_para_set(unit, userPort));

        if(DVA_PHY_MODE == portRole)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_5gxfi_phy_role_set(unit, userPort, speed));
        }

        /*main reset*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_10gkr_force_aphy_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 17u, 0u, 15u, 0x80u));
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 18u, 0u, 15u, 0x0u));
        /* set bit[4:0] = 0x10 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 30u, 7u, 0u, 15u, 0x3F0u));
        /* bit 12 set 1 to enable nway */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 0u, 0u, 15u, 0x1000u));
        /* set bit[8:0] = 0x15 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 30u, 6u, 0u, 15u, 0x95u));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_10gkr_config_set(uint32 unit, RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /*todo: error handle for portRole check, which port role should be mac or phy role*/
        if((DVA_PHY_MODE != portRole) && (DVA_MAC_MODE != portRole))
        {
            ret = -RT_ERR_SDS_PORT_ROLE;
            continue;
        }

        if(DVA_SPEED_10000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        /* bit [9] = 1, bit[10] = 1 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x075Au));

        if(DVA_AN_MODE == isForceMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_10gkr_force_aphy_set(unit, userPort));
        }

        /* APHY parameters */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_10G_aphy_para_set(unit, userPort));

        if(DVA_PHY_MODE == portRole)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_10gkr_phy_role_set(unit, userPort, speed));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_10gkr_mac_role_set(unit, userPort));
        }

        /*main reset*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_5gkr_force_aphy_set(uint32 unit, RtkPort userPort)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 17u, 0u, 15u, 0x0u));
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 18u, 0u, 15u, 0xC002u));
        /* set bit[4:0] = 0x10 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 30u, 7u, 0u, 15u, 0x3F0u));
        /* bit 12 set 1 to enable nway */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 19u, 0u, 0u, 15u, 0x1000u));
        /* set bit[8:0] = 0x15 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 30u, 6u, 0u, 15u, 0x95u));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_5gkr_config_set(uint32 unit, RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /*todo: error handle for portRole check, which port role should be mac or phy role*/
        if((DVA_PHY_MODE != portRole) && (DVA_MAC_MODE != portRole))
        {
            ret = -RT_ERR_SDS_PORT_ROLE;
            continue;
        }

        if(DVA_SPEED_5000MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        /* bit [9] = 1, bit[10] = 1 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x075Au));

        /* bit [0] = 1, bit[15] = 1 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 7u, 6u, 0u, 15u, 0x8001u));

        if(DVA_AN_MODE == isForceMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_5gkr_force_aphy_set(unit, userPort));
        }

        /* APHY parameters */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_5G_aphy_para_set(unit, userPort));

        if(DVA_PHY_MODE == portRole)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_5gkr_phy_role_set(unit, userPort, speed));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_5gkr_mac_role_set(unit, userPort));
        }

        /*main reset*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_usxgmii_err_chk(RtkPort userPort, uint32 portRole, uint32 speed, uint32 portSpeed)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /*error handler for port check, which port number should be serdes user port(9 ~ 12)*/
        if((DVA_RTK_USER_PORT_9TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /*todo: error handle for portRole check, which port role should be mac or phy role*/
        if((DVA_PHY_MODE != portRole) && (DVA_MAC_MODE != portRole))
        {
            ret = -RT_ERR_SDS_PORT_ROLE;
            continue;
        }

        if((DVA_SPEED_5000MBPS != speed) && (DVA_SPEED_2500MBPS != speed) && (DVA_SPEED_10000MBPS != speed))
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

        if(((DVA_SPEED_5000MBPS != portSpeed) && (DVA_SPEED_10000MBPS != portSpeed)) || ((DVA_SPEED_10000MBPS == speed) && (DVA_SPEED_5000MBPS == portSpeed)))
        {
            ret = -RT_ERR_SDS_SPEED_MISMACH;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_usxgmii_config_set(uint32 unit, RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed, uint32 portSpeed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*speed error check*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_usxgmii_err_chk(userPort, portRole, speed, portSpeed));

        /* bit [9] = 1, bit[10] = 1 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x075Au));

        /* bit [0] = 1, bit[15] = 1 */
        CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 7u, 6u, 0u, 15u, 0x8001u));

        if(DVA_FORCE_MODE == isForceMode)
        {
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 7u, 17u, 0u, 15u, 0x054Cu));
        }

        /* update APHY parameters */
        if(DVA_SPEED_10000MBPS == portSpeed)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_10G_aphy_para_set(unit, userPort));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_5G_aphy_para_set(unit, userPort));
        }

        if(DVA_PHY_MODE == portRole)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_usxgmii_phy_role_set(unit, userPort, speed, portSpeed));
        }

        /*main reset*/
        CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_hisgmii_aphy_config_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(DVA_RTK_USER_PORT_9TH <= userPort)
        {
            /* Update WTG APHY parameters */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_wtg_hsg_aphy_para_set(unit, userPort));
            /* main reset */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
        }
        else
        {
            /* analog SDS reset assert */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 1u, 12u, 0u, 15u, 0x204u));
            /* main reset */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
            /* Update WRAP APHY parameters */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_wrap_sg_hsg_aphy_para_set(unit, userPort, speed));

            /* analog SDS de-reset assert */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 1u, 12u, 0u, 15u, 0x4u));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_hsgmii_err_chk(RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for port check, which port number should be serdes user port(4 ~ 12)*/
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /*todo: error handle for portRole check, which port role should be mac or phy role*/
        if((DVA_PHY_MODE != portRole) && (DVA_MAC_MODE != portRole))
        {
            ret = -RT_ERR_SDS_PORT_ROLE;
            continue;
        }

        if(DVA_SPEED_2500MBPS != speed)
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }

    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_hsgmii_pcs_config_set(uint32 unit, RtkPort userPort, uint32 isForceMode)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((DVA_RTK_USER_PORT_9TH <= userPort) && (DVA_RTK_USER_PORT_12TH >= userPort))
        {
            /* bit[10] = 1 */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 6u, 14u, 0u, 15u, 0x055Au));

            /*for HW limitation*/
            if((DVA_RTK_USER_PORT_10TH == userPort) || (DVA_RTK_USER_PORT_12TH == userPort))
            {
                /* bit[8] = 1 */
                CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, (userPort - 1u), 1u, 20u, 0u, 15u, 0x0100u));
            }
        }

        if(DVA_FORCE_MODE == isForceMode)
        {
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 0u, 2u, 0u, 15u, 0x7180u));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_hsgmii_config_set(uint32 unit, RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* error check */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_hsgmii_err_chk(userPort, portRole, isForceMode, speed));

        /* pcs config set */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_hsgmii_pcs_config_set(unit, userPort, isForceMode));

        /* aphy config set */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_hisgmii_aphy_config_set(unit, userPort, speed));

        if(DVA_PHY_MODE == portRole)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_hisgmii_phy_role_set(unit, userPort, speed));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_hisgmii_mac_role_set(unit, userPort));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_sgmii_speed_chk(uint32 unit, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;
    do
    {
        if((DVA_SPEED_1000MBPS != speed) && (DVA_SPEED_100MBPS != speed) && (DVA_SPEED_10MBPS != speed))
        {
            ret = -RT_ERR_SDS_SPEED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_sgmii_aphy_config_set(uint32 unit, RtkPort userPort, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(DVA_RTK_USER_PORT_9TH <= userPort)
        {
            /* Update WTG APHY parameters */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_wtg_sg_aphy_para_set(unit, userPort));
            /* main reset */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
        }
        else
        {
            /* analog SDS reset assert */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 1u, 12u, 0u, 15u, 0x204u));
            /* main reset */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_main_reset(unit, userPort, DVA_SDS_NORMAL));
            /* Update WRAP APHY parameters */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_wrap_sg_hsg_aphy_para_set(unit, userPort, speed));

            /* analog SDS reset de-assert */
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 1u, 12u, 0u, 15u, 0x4u));
        }
    } while(0u == 1u);

    return ret;
}

static RtkApiRet dva_drv_sds_sgmii_config_set(uint32 unit, RtkPort userPort, uint32 portRole, uint32 isForceMode, uint32 speed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for port check, which port number should be serdes user port(7, 9, 10 and 11)*/
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /*error handle for portRole check, which port role should be mac or phy role*/
        if((DVA_PHY_MODE != portRole) && (DVA_MAC_MODE != portRole))
        {
            ret = -RT_ERR_SDS_PORT_ROLE;
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_sds_sgmii_speed_chk(unit, speed));

        if(DVA_FORCE_MODE == isForceMode)
        {
            CHK_FUN_CONTINUE(ret, dva_sys_sds_reg_setbits_pcs(unit, userPort, 0u, 2u, 0u, 15u, 0x7180u));
        }
        /* aphy config set */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_sgmii_aphy_config_set(unit, userPort, speed));

        if(DVA_PHY_MODE == portRole)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_sgmii_phy_role_set(unit, userPort, speed));
        }
        else
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_sgmii_mac_role_set(unit, userPort));
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_sds_mode_set(uint32 unit, RtkPort userPort, RtkModeExt_e serdesMode, uint32 portRole, uint32 isForceMode, uint32 speed, uint32 portSpeed)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(EM_MODE_EXT_SGMII == serdesMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_sgmii_config_set(unit, userPort, portRole, isForceMode, speed));
        }

        if(EM_MODE_EXT_HISGMII == serdesMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_hsgmii_config_set(unit, userPort, portRole, isForceMode, speed));
        }

        if(EM_MODE_EXT_USXGMII == serdesMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_usxgmii_config_set(unit, userPort, portRole, isForceMode, speed, portSpeed));
        }

        if(EM_MODE_EXT_5G_KR == serdesMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_5gkr_config_set(unit, userPort, portRole, isForceMode, speed));
        }

        if(EM_MODE_EXT_10G_KR == serdesMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_10gkr_config_set(unit, userPort, portRole, isForceMode, speed));
        }

        if(EM_MODE_EXT_5G_XFI == serdesMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_5gxfi_config_set(unit, userPort, portRole, isForceMode, speed));
        }

        if(EM_MODE_EXT_10G_XFI == serdesMode)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_sds_10gxfi_config_set(unit, userPort, portRole, isForceMode, speed));
        }

    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_sds_mode_chk(RtkModeExt_e serdesMode)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /*error handler for serdesMode check, which serdesMode should be usxgmii, hisgmii, sgmii and 5gkr*/
        if((serdesMode != EM_MODE_EXT_SGMII) && (serdesMode != EM_MODE_EXT_HISGMII) && (serdesMode != EM_MODE_EXT_USXGMII) && (serdesMode != EM_MODE_EXT_5G_KR) && (serdesMode != EM_MODE_EXT_10G_KR) && (serdesMode != EM_MODE_EXT_5G_XFI) && (serdesMode != EM_MODE_EXT_10G_XFI))
        {
            ret = -RT_ERR_PORT_MODE;
            continue;
        }
    } while(0u == 1u);

    return ret;
}
