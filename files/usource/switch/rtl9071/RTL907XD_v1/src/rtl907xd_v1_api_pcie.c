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

#include "rtl907xd_v1_asicdrv_otp.h"
#include "rtl907xd_v1_asicdrv_pcie.h"
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_pcie.h"

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
RtkApiRet dv1_pcie_parm_get(uint32 unit, RtkPort portId, PcieParm_t *pcieParam)

{
    uint32    defaultIdAddr = 0u;
    uint32    macAddr       = 0u;
    uint32    smidSvidAddr  = 0u;
    uint32    serialNumAddr = 0u;
    uint32    idx           = 0u;
    uint32    offset        = 0u;
    uint16    used          = 0u;
    uint16    valid         = 0u;
    uint8     pcieOtpData[DV1_PCIE_OTP_CONFIG_SIZE];
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, dv1_drv_memset(pcieOtpData, 0xFF, DV1_PCIE_OTP_CONFIG_VALID_CTRL));
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, DV1_PCIe_OTP_START_ADDR, (&pcieOtpData[0u]), DV1_PCIE_OTP_CONFIG_VALID_CTRL));

        used  = (((uint16)pcieOtpData[0u] >> DV1_PCIE_OTP_1ST_CONFIG_USED_VALID_OFFSET) & DV1_PCIE_OTP_1ST_CONFIG_USED_VALID_MASK);
        valid = ((uint16)pcieOtpData[0u] & DV1_PCIE_OTP_1ST_CONFIG_USED_VALID_MASK);
        for(idx = 0u; idx < DV1_PCIE_OTP_CONFIG_BLOCK_NUM; idx++)
        {
            if(0u == idx)
            {
                offset = DV1_PCIE_OTP_CONFIG_VALID_CTRL;
            }
            else
            {
                offset = (idx * DV1_PCIE_OTP_CONFIG_BLOCK_SIZE);
            }

            if((0u == (used & ((uint16)0x1u << idx))) && (1u == (valid & ((uint16)0x1u << idx))))
            {
                break;
            }
        }

        if(portId == DV1_RTK_USER_PORT_9TH)
        {
            defaultIdAddr = (offset + DV1_PCIe_OTP_START_ADDR + DV1_PCIE_OTP_P9_OFFSET);
            macAddr       = (defaultIdAddr + 1u);
            smidSvidAddr  = (macAddr + MAC_ADDR_LEN);
            serialNumAddr = (smidSvidAddr + SMIDSVID_ADDR_LEN);
        }
        else if(portId == DV1_RTK_USER_PORT_11TH)
        {
            defaultIdAddr = (offset + DV1_PCIe_OTP_START_ADDR + DV1_PCIE_OTP_P11_OFFSET);
            macAddr       = (defaultIdAddr + 1u);
            smidSvidAddr  = (macAddr + MAC_ADDR_LEN);
            serialNumAddr = (smidSvidAddr + SMIDSVID_ADDR_LEN);
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, defaultIdAddr, pcieParam->defaultId, DEFAULT_ID_LEN));
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, macAddr, pcieParam->mac, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, smidSvidAddr, pcieParam->smidSvid, SMIDSVID_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, serialNumAddr, pcieParam->serialNum, SERIALNUM_ADDR_LEN));
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
 * @param[in]  portId      Port Idendification (DV1_RTK_USER_PORT_10TH). \n
 *                         Port Idendification which is used for PCIE.
 * @param[in,out] *speed    Link speed (PCIE_LINK_SPEED_GEN1 to PCIE_LINK_SPEED_GEN3). \n
 *                         Link speed used for the PCIE port.
 * @retval RT_ERR_OK       The PCIE link speed has been read from the register.
 * @retval -RT_ERR_FAILED  Timeout
 * @retval -RT_ERR_PORT_ID The portNum is invalid.
 */
RtkApiRet dv1_pcie_linkspeed_get(uint32 unit, RtkPort portId, uint32 *speed)

{
    RtkApiRet retVal             = RT_ERR_OK;
    uint32    regVal             = 0u;
    uint32    pcieLinkStatus     = 0u;
    uint32    pcieLinkSpeed      = 0u;
    uint32    timeout            = 0u;
    uint32    dumpLinkStatusLsp  = 0u;
    uint32    dumpLinkStatusLen  = 0u;
    uint32    linkStatusDummyReg = 0u;
    do
    {
        if(portId == DV1_RTK_USER_PORT_9TH)
        {
            dumpLinkStatusLsp  = DV1_PCIE_P9_DUMP_LINK_STATUS_LSP;
            dumpLinkStatusLen  = DV1_PCIE_P9_DUMP_LINK_STATUS_LEN;
            linkStatusDummyReg = DV1_PCIE_P9_LINK_STATUS_REG;
        }
        else if(portId == DV1_RTK_USER_PORT_11TH)
        {
            dumpLinkStatusLsp  = DV1_PCIE_P11_DUMP_LINK_STATUS_LSP;
            dumpLinkStatusLen  = DV1_PCIE_P11_DUMP_LINK_STATUS_LEN;
            linkStatusDummyReg = DV1_PCIE_P11_LINK_STATUS_REG;
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PCIE_SW_CONFIG_REG, dumpLinkStatusLsp, dumpLinkStatusLen, 1));
        do
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PCIE_SW_CONFIG_REG, dumpLinkStatusLsp, dumpLinkStatusLen, &regVal));
            timeout++;
        } while((0u != regVal) && (timeout < 0x100u));

        if(timeout == 0x100u)
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_read(unit, linkStatusDummyReg, &regVal));
            pcieLinkStatus = (regVal & (uint32)DV1_PCIE_SW_CONFIG_LINK_STATUS_MASK);
            pcieLinkSpeed  = ((regVal & (uint32)DV1_PCIE_SW_CONFIG_LINK_SPEED_MASK) >> DV1_PCIE_SW_CONFIG_LINK_SPEED_OFFSET);
            if((uint32)DV1_PCIE_LINK_UP_OK == pcieLinkStatus)
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
 *            Stacking unit id is used for indicating which switch to be accessed.
 * @param[in]  portId      Port Idendification (DV1_RTK_USER_PORT_9TH, DV1_RTK_USER_PORT_11TH). \n
 *                         Port Idendification which is used for PCIE.
 * @param[in,out] *pcie2LaneEn PCIE lanes get information (N/A) \n
 *                         Number of lanes used for the PCIE port.
 * @retval RT_ERR_OK The number of PCIE port lanes has been read from the register.
 * @retval -RT_ERR_PORT_ID The portId is invalid.
 */
RtkApiRet dv1_pcie_lane_get(uint32 unit, RtkPort portId, uint32 *pcie2LaneEn)

{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    pcie2LaneEnReg = 0u;
    do
    {
        if(portId == DV1_RTK_USER_PORT_9TH)
        {
            pcie2LaneEnReg = DV1_PCIE_P9_CTRL_REG;
        }
        else if(portId == DV1_RTK_USER_PORT_11TH)
        {
            pcie2LaneEnReg = DV1_PCIE_P11_CTRL_REG;
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, pcie2LaneEnReg, DV1_PCIE_2LANE_ENABLE_LSP, DV1_PCIE_2LANE_ENABLE_LEN, pcie2LaneEn));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*PCIE_LANE_ASIC*/
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
 * @param[in]  portId      Port Idendification (DV1_RTK_USER_PORT_9TH, DV1_RTK_USER_PORT_11TH). \n
 *                         Port Idendification which is used for PCIE.
 * @param[out] *pciePtmSync Information about the PCIE PTM Sync to the Switch (N/A) \n
 *                         PCIE port for PTM Sync to the Switch.
 * @retval RT_ERR_OK The PCIE port for PTM Sync to the Switch has been read from the register.
 * @retval -RT_ERR_PORT_ID The portId is invalid.
 */
RtkApiRet dv1_pcie_ptm_sync_info_get(uint32 unit, RtkPort portId, uint32 *pciePtmSync)

{
    RtkApiRet retVal;
    do
    {
        if((portId == DV1_RTK_USER_PORT_9TH) || (portId == DV1_RTK_USER_PORT_11TH))
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PCIE_PTM_CTRL_REG, DV1_PCIE_PTM_CTRL_REG_PORT_SEL_LSP, DV1_PCIE_PTM_CTRL_REG_PORT_SEL_LEN, pciePtmSync));
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
