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

#include "rtl907xd_va_asicdrv_otp.h"
#include "rtl907xd_va_asicdrv_pcie.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_pcie.h"
#include "rtl907xd_va_asicdrv_timer.h"
#include "rtl907xd_va_api_esm.h"

/**
 * @addtogroup RTK_MODULE_PCIE PCIE
 * @brief PCIE APIs
 * @{
 */

/**
 * @addtogroup PCIE_PARM PCIE Parameters
 * @{
 */

/**
 * @addtogroup PCIE_ASIC Low Level Driver
 * @brief PCIE Low Level Driver
 * @{
 */

/**
 * @brief Get PCIE port MACID, SMIDSVID and Serial Number.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used for indicating which switch to be accessed.
 * @param[in]  portId      Port Idendification (CVA_RTK_USER_PORT_10TH). \n
 *                         Port Idendification which is used for PCIE.
 * @param[in,out] *pcieParam PCIE port parameters (N/A) \n
 *             Contain MACID, SMIDSVID and Serial Number.
 * @retval RT_ERR_OK The PCIE parameters have been read from OTP.
 * @retval -RT_ERR_PORT_ID The portNum is invalid.
 */
RtkApiRet dva_pcie_parm_get(uint32 unit, RtkPort portId, PcieParm_t *pcieParam)

{
    uint32    defaultIdAddr = 0u;
    uint32    macAddr       = 0u;
    uint32    smidSvidAddr  = 0u;
    uint32    serialNumAddr = 0u;
    uint32    idx           = 0u;
    uint32    offset        = 0u;
    uint16    used          = 0u;
    uint16    valid         = 0u;
    uint8     pcieOtpData[DVA_PCIE_OTP_CONFIG_SIZE];
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(pcieOtpData, 0xFF, DVA_PCIE_OTP_CONFIG_VALID_CTRL));
        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, DVA_PCIe_OTP_START_ADDR, (&pcieOtpData[0u]), DVA_PCIE_OTP_CONFIG_VALID_CTRL));

        used  = (((uint16)pcieOtpData[0u] >> DVA_PCIE_OTP_1ST_CONFIG_USED_VALID_OFFSET) & DVA_PCIE_OTP_1ST_CONFIG_USED_VALID_MASK);
        valid = ((uint16)pcieOtpData[0u] & DVA_PCIE_OTP_1ST_CONFIG_USED_VALID_MASK);
        for(idx = 0u; idx < DVA_PCIE_OTP_CONFIG_BLOCK_NUM; idx++)
        {
            if(0u == idx)
            {
                offset = DVA_PCIE_OTP_CONFIG_VALID_CTRL;
            }
            else
            {
                offset = (idx * DVA_PCIE_OTP_CONFIG_BLOCK_SIZE);
            }

            if((0u == (used & ((uint16)0x1u << idx))) && (1u == (valid & ((uint16)0x1u << idx))))
            {
                break;
            }
        }

        if(portId == DVA_RTK_USER_PORT_9TH)
        {
            defaultIdAddr = (offset + DVA_PCIe_OTP_START_ADDR + DVA_PCIE_OTP_P9_OFFSET);
            macAddr       = (defaultIdAddr + 1u);
            smidSvidAddr  = (macAddr + MAC_ADDR_LEN);
            serialNumAddr = (smidSvidAddr + SMIDSVID_ADDR_LEN);
        }
        else if(portId == DVA_RTK_USER_PORT_11TH)
        {
            defaultIdAddr = (offset + DVA_PCIe_OTP_START_ADDR + DVA_PCIE_OTP_P11_OFFSET);
            macAddr       = (defaultIdAddr + 1u);
            smidSvidAddr  = (macAddr + MAC_ADDR_LEN);
            serialNumAddr = (smidSvidAddr + SMIDSVID_ADDR_LEN);
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, defaultIdAddr, pcieParam->defaultId, DEFAULT_ID_LEN));
        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, macAddr, pcieParam->mac, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, smidSvidAddr, pcieParam->smidSvid, SMIDSVID_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, dva_otp_read(unit, serialNumAddr, pcieParam->serialNum, SERIALNUM_ADDR_LEN));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*PCIE_ASIC*/
/**@}*/ /*PCIE_PARM*/

/**
 * @addtogroup PCIE_SPEED PCIE Link Speed
 * @{
 */

/**
 * @addtogroup PCIE_SPEED_ASIC Low Level Driver
 * @brief PCIE Low Level Driver
 * @{
 */

/**
 * @brief Get PCIE port link speed .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portId      Port Idendification (DVA_RTK_USER_PORT_10TH). \n
 *                         Port Idendification which is used for PCIE.
 * @param[in,out] *speed   Link speed (PCIE_LINK_SPEED_GEN1 to PCIE_LINK_SPEED_GEN3). \n
 *                         Link speed used for the PCIE port.
 * @retval RT_ERR_OK       The PCIE link speed has been set in the register.
 * @retval -RT_ERR_PORT_ID The portNum is invalid.
 * @retval -RT_ERR_INPUT   The port link speed is invalid.
 */
RtkApiRet dva_pcie_linkspeed_get(uint32 unit, RtkPort portId, uint32 *speed)

{
    RtkApiRet          retVal             = RT_ERR_OK;
    uint32             regVal             = 0u;
    uint32             pcieLinkStatus     = 0u;
    uint32             pcieLinkSpeed      = 0u;
    uint32             dumpLinkStatusLsp  = 0u;
    uint32             dumpLinkStatusLen  = 0u;
    uint32             linkStatusDummyReg = 0u;
    RegPollingConfig_t pollingCfg         = {0};

    do
    {
        if(portId == DVA_RTK_USER_PORT_9TH)
        {
            dumpLinkStatusLsp  = DVA_PCIE_P9_DUMP_LINK_STATUS_LSP;
            dumpLinkStatusLen  = DVA_PCIE_P9_DUMP_LINK_STATUS_LEN;
            linkStatusDummyReg = DVA_PCIE_P9_LINK_STATUS_REG;
        }
        else if(portId == DVA_RTK_USER_PORT_11TH)
        {
            dumpLinkStatusLsp  = DVA_PCIE_P11_DUMP_LINK_STATUS_LSP;
            dumpLinkStatusLen  = DVA_PCIE_P11_DUMP_LINK_STATUS_LEN;
            linkStatusDummyReg = DVA_PCIE_P11_LINK_STATUS_REG;
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* Set pollingCfg structure members */
        pollingCfg.triggerRegAddr = DVA_PCIE_SW_CONFIG_REG;
        pollingCfg.triggerRegLsp  = dumpLinkStatusLsp;
        pollingCfg.triggerRegLen  = dumpLinkStatusLen;
        pollingCfg.action         = 1u;
        pollingCfg.pollingRegAddr = DVA_PCIE_SW_CONFIG_REG;
        pollingCfg.pollingRegLsp  = dumpLinkStatusLsp;
        pollingCfg.pollingRegLen  = dumpLinkStatusLen;
        pollingCfg.expVal         = 0u;
        pollingCfg.eventID        = DVA_EM_EVENT_PCIE_POLLING_TIMEOUT;

        CHK_FUN_RET(retVal, dva_drv_timer_polling_reg_timeout(unit, &pollingCfg, DVA_PCIE_TIME_MAX_US, DVA_PCIE_TIME_MIN_US));
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_read(unit, linkStatusDummyReg, &regVal));
            pcieLinkStatus = (regVal & (uint32)DVA_PCIE_SW_CONFIG_LINK_STATUS_MASK);
            pcieLinkSpeed  = ((regVal & (uint32)DVA_PCIE_SW_CONFIG_LINK_SPEED_MASK) >> DVA_PCIE_SW_CONFIG_LINK_SPEED_OFFSET);
            if((uint32)DVA_PCIE_LINK_UP_OK == pcieLinkStatus)
            {
                *speed = pcieLinkSpeed;
            }
            else
            {
                *speed = 0;
            }
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*PCIE_SPEED_ASIC*/
/**@}*/ /*PCIE_SPEED*/

/**
 * @addtogroup PCIE_LANE Number of PCIE lanes
 * @{
 */

/**
 * @addtogroup PCIE_LANE_ASIC Low Level Driver
 * @brief PCIE Low Level Driver
 * @{
 */

/**
 * @brief Get number of PCIE lanes.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portId      Port Idendification (DVA_RTK_USER_PORT_9TH, DVA_RTK_USER_PORT_11TH). \n
 *                         Port Idendification which is used for PCIE.
 * @param[in,out] *pcie2LaneEn PCIE lanes get information (N/A) \n
 *                         Number of lanes used for the PCIE port.
 * @retval RT_ERR_OK The number of PCIE port lanes has been read from the register.
 * @retval -RT_ERR_PORT_ID The portId is invalid.
 */
RtkApiRet dva_pcie_lane_get(uint32 unit, RtkPort portId, uint32 *pcie2LaneEn)

{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    pcie2LaneEnReg = 0u;
    do
    {
        if(portId == DVA_RTK_USER_PORT_9TH)
        {
            pcie2LaneEnReg = DVA_PCIE_P9_CTRL_REG;
        }
        else if(portId == DVA_RTK_USER_PORT_11TH)
        {
            pcie2LaneEnReg = DVA_PCIE_P11_CTRL_REG;
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, pcie2LaneEnReg, DVA_PCIE_2LANE_ENABLE_LSP, DVA_PCIE_2LANE_ENABLE_LEN, pcie2LaneEn));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*PCIE_ASIC*/
/**@}*/ /*PCIE_LANE*/

/**
 * @addtogroup PCIE_PTM PCIE PTM Sync to the Switch.
 * @{
 */

/**
 * @addtogroup PCIE_PTM_ASIC Low Level Driver
 * @brief PCIE Low Level Driver
 * @{
 */

/**
 * @brief Get the PCIE port for PTM Sync to the Switch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portId      Port Idendification (DVA_RTK_USER_PORT_9TH, DVA_RTK_USER_PORT_11TH). \n
 *                         Port Idendification which is used for PCIE.
 * @param[out] *pciePtmSync Information about the PCIE PTM Sync to the Switch (N/A) \n
 *                         PCIE port for PTM Sync to the Switch.
 * @retval RT_ERR_OK The PCIE port for PTM Sync to the Switch has been read from the register.
 * @retval -RT_ERR_PORT_ID The portId is invalid.
 */
RtkApiRet dva_pcie_ptm_sync_info_get(uint32 unit, RtkPort portId, uint32 *pciePtmSync)

{
    RtkApiRet retVal;
    do
    {
        if((portId == DVA_RTK_USER_PORT_9TH) || (portId == DVA_RTK_USER_PORT_11TH))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_PCIE_PTM_CTRL_REG, DVA_PCIE_PTM_CTRL_REG_PORT_SEL_LSP, DVA_PCIE_PTM_CTRL_REG_PORT_SEL_LEN, pciePtmSync));
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*PCIE_ASIC*/
/**@}*/ /*PCIE_PTM*/
/**@}*/ /* PCIE */
