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

#include "rtl907xc_va_asicdrv_otp.h"
#include "rtl907xc_va_asicdrv_pcie.h"
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_api_pcie.h"

/**
 * @addtogroup RTK_MODULE_PCIE PCIE
 * @brief PCIE APIs
 * @{
 */

/**
 * @addtogroup PCIE_PARM PCIe Parameters
 * @{
 */

/**
 * @addtogroup PCIE_ASIC Low Level Driver
 * @brief PCIE Low Level Driver
 * @{
 */

/**
 * @brief Get PCIe port MACID, SMIDSVID and Serial Number.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used for indicating which switch to be accessed.
 * @param[in]  portId      Port Idendification (CVA_RTK_USER_PORT_10TH). \n
 *                         Port Idendification which is used for PCIe.
 * @param[in,out] *pcieParam PCIe port parameters (N/A) \n
 *             Contain MACID, SMIDSVID and Serial Number.
 * @retval RT_ERR_OK The PCIe parameters have been read from OTP.
 * @retval -RT_ERR_PORT_ID The portNum is invalid.
 */

RtkApiRet cva_pcie_parm_get(uint32 unit, cva_RtkPort portId, PcieParm_t *pcieParam)

{
    uint32    defaultIdAddr = 0u;
    uint32    macAddr       = 0u;
    uint32    smidSvidAddr  = 0u;
    uint32    serialNumAddr = 0u;
    uint32    idx           = 0u;
    uint32    offset        = 0u;
    uint16    used          = 0u;
    uint16    valid         = 0u;
    uint8     pcieOtpData[CVA_PCIE_OTP_CONFIG_SIZE];
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        if(portId == CVA_RTK_USER_PORT_10TH)
        {
            CHK_FUN_CONTINUE(retVal, cva_drv_drv_memset(pcieOtpData, 0xFF, CVA_PCIE_OTP_CONFIG_SIZE));
            CHK_FUN_CONTINUE(retVal, cva_otp_read(unit, CVA_PCIe_OTP_START_ADDR, (&pcieOtpData[0u]), CVA_PCIE_OTP_CONFIG_VALID_CTRL));

            used  = ((uint16)pcieOtpData[0] << 8) | (uint16)pcieOtpData[1];
            valid = ((uint16)pcieOtpData[2] << 8) | (uint16)pcieOtpData[3];
            for(idx = 0u; idx < CVA_PCIE_OTP_CONFIG_BLOCK_NUM; idx++)
            {
                if(0u == idx)
                {
                    offset = CVA_PCIE_OTP_CONFIG_VALID_CTRL;
                }
                else
                {
                    offset = (idx * CVA_PCIE_OTP_CONFIG_BLOCK_SIZE);
                }

                if((0u == (used & ((uint16)0x1u << idx))) && (1u == (valid & ((uint16)0x1u << idx))))
                {
                    break;
                }
            }
            defaultIdAddr = (offset + CVA_PCIe_OTP_START_ADDR);
            macAddr       = (defaultIdAddr + 1u);
            smidSvidAddr  = (macAddr + MAC_ADDR_LEN);
            serialNumAddr = (smidSvidAddr + SMIDSVID_ADDR_LEN);
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(retVal, cva_otp_read(unit, defaultIdAddr, pcieParam->defaultId, DEFAULT_ID_LEN));
        CHK_FUN_CONTINUE(retVal, cva_otp_read(unit, macAddr, pcieParam->mac, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, cva_otp_read(unit, smidSvidAddr, pcieParam->smidSvid, SMIDSVID_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, cva_otp_read(unit, serialNumAddr, pcieParam->serialNum, SERIALNUM_ADDR_LEN));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*PCIE_ASIC*/
/**@}*/ /*PCIE_PARM*/

/**
 * @addtogroup PCIE_SPEED PCIe Link Speed
 * @{
 */

/**
 * @addtogroup PCIE_SPEED_ASIC Low Level Driver
 * @brief PCIE Low Level Driver
 * @{
 */

/**
 * @brief Get PCIe port link speed .
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portId      Port Idendification (CVA_RTK_USER_PORT_10TH). \n
 *                         Port Idendification which is used for PCIe.
 * @param[in,out] *speed    Link speed (PCIE_LINK_SPEED_GEN1 to PCIE_LINK_SPEED_GEN3). \n
 *                         Link speed used for the PCIe port.
 * @retval RT_ERR_OK       The PCIe link speed has been read from the register.
 * @retval -RT_ERR_FAILED  Timeout
 * @retval -RT_ERR_PORT_ID The portNum is invalid.
 */
RtkApiRet cva_pcie_linkspeed_get(uint32 unit, cva_RtkPort portId, uint32 *speed)

{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    regVal         = 0u;
    uint32    pcieLinkStatus = 0u;
    uint32    pcieLinkSpeed  = 0u;
    uint32    timeout        = 0u;

    do
    {
        if(portId == CVA_RTK_USER_PORT_10TH)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_PCIE_SW_CONFIG_REG, CVA_PCIE_DUMP_LINK_STATUS_LSP, CVA_PCIE_DUMP_LINK_STATUS_LEN, 1));
            do
            {
                CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_PCIE_SW_CONFIG_REG, CVA_PCIE_DUMP_LINK_STATUS_LSP, CVA_PCIE_DUMP_LINK_STATUS_LEN, &regVal));
                timeout++;
            } while((0u != regVal) && (timeout < 0x100000u));

            if(timeout == 0x100000u)
            {
                retVal = -RT_ERR_FAILED;
                continue;
            }
            else
            {
                CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_PCIE_DUMP_LINK_STATUS_REG, &regVal));
                pcieLinkStatus = (regVal & (uint32)CVA_PCIE_SW_CONFIG_LINK_STATUS);
                pcieLinkSpeed  = ((regVal & (uint32)CVA_PCIE_SW_CONFIG_LINK_SPEED) >> 16);
            }

            if((uint32)CVA_PCIE_LINK_UP_OK == pcieLinkStatus)
            {
                *speed = pcieLinkSpeed;
            }
            else
            {
                *speed = 0;
            }
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /*PCIE_SPEED_ASIC*/
/**@}*/ /*PCIE_SPEED*/
/**@}*/ /* PCIE */
