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
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_asicdrv_pcie.h"
RtkApiRet dv1_drv_pcie_macid_set(uint32 unit, uint32 type, const PcieParm_t *pcieParmBuf, uint32 offset)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    i                = 0u;
    uint32    macAddr          = 0u;
    uint32    macIdAlreadySet  = 0u;
    uint8     defaultIdBuf[1u] = {0xFF};
    uint8     pcieOtpData[MAC_ADDR_LEN];

    do
    {
        macAddr = (offset + 1u);

        /*Set DefaultId*/
        defaultIdBuf[0u] = (defaultIdBuf[0u] & (uint8)(~type));
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, offset, (&pcieOtpData[0u]), 1u));
        if(0xFFu == pcieOtpData[0u])
        {
            CHK_FUN_CONTINUE(retVal, dv1_otp_write(unit, offset, defaultIdBuf, 1u));
        }

        /*Set MACId*/
        if(0u != (type & PCIE_MACID_FLAG))
        {
            CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, macAddr, (&pcieOtpData[0u]), MAC_ADDR_LEN));
            for(i = (uint32)0u; i < MAC_ADDR_LEN; i += (uint32)1u)
            {
                if(0xFFu != pcieOtpData[i])
                {
                    macIdAlreadySet = 1u;
                }
            }
            if(1u != macIdAlreadySet)
            {
                for(i = (uint32)0u; i < MAC_ADDR_LEN; i += (uint32)1u)
                {
                    if(0xFFu != pcieParmBuf->mac[i])
                    {
                        CHK_FUN_CONTINUE(retVal, dv1_otp_write(unit, macAddr + i, &(pcieParmBuf->mac[i]), 1u));
                    }
                }
            }
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dv1_drv_pcie_smidsvid_set(uint32 unit, uint32 type, const PcieParm_t *pcieParmBuf, uint32 offset)
{
    RtkApiRet retVal             = RT_ERR_OK;
    uint32    i                  = 0u;
    uint32    smidSvidAddr       = 0u;
    uint32    smidSvidAlreadySet = 0u;
    uint8     pcieOtpData[SMIDSVID_ADDR_LEN];

    do
    {
        smidSvidAddr = (offset + 1u + MAC_ADDR_LEN);

        /*Set SMID and SVID*/
        if(0u != (type & PCIE_SMIDSVID_FLAG))
        {
            CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, smidSvidAddr, (&pcieOtpData[0u]), SMIDSVID_ADDR_LEN));
            for(i = (uint32)0u; i < SMIDSVID_ADDR_LEN; i += (uint32)1u)
            {
                if(0xFFu != pcieOtpData[i])
                {
                    smidSvidAlreadySet = 1u;
                }
            }
            if(1u != smidSvidAlreadySet)
            {
                for(i = (uint32)0u; i < SMIDSVID_ADDR_LEN; i += (uint32)1u)
                {
                    if(0xFFu != pcieParmBuf->smidSvid[i])
                    {
                        CHK_FUN_CONTINUE(retVal, dv1_otp_write(unit, smidSvidAddr + i, &(pcieParmBuf->smidSvid[i]), 1u));
                    }
                }
            }
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dv1_drv_pcie_serialnum_set(uint32 unit, uint32 type, const PcieParm_t *pcieParmBuf, uint32 offset)
{
    RtkApiRet retVal              = RT_ERR_OK;
    uint32    i                   = 0u;
    uint32    serialNumAddr       = 0u;
    uint32    serialNumAlreadySet = 0u;
    uint8     pcieOtpData[SERIALNUM_ADDR_LEN];

    do
    {
        serialNumAddr = (offset + 1u + MAC_ADDR_LEN + SMIDSVID_ADDR_LEN);

        /*Set Serial Number*/
        if(0u != (type & PCIE_SERIALNUM_FLAG))
        {
            CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, serialNumAddr, (&pcieOtpData[0u]), SERIALNUM_ADDR_LEN));
            for(i = (uint32)0u; i < SERIALNUM_ADDR_LEN; i += (uint32)1u)
            {
                if(0xFFu != pcieOtpData[i])
                {
                    serialNumAlreadySet = 1u;
                }
            }
            if(1u != serialNumAlreadySet)
            {
                for(i = (uint32)0u; i < SERIALNUM_ADDR_LEN; i += (uint32)1u)
                {
                    if(0xFFu != pcieParmBuf->serialNum[i])
                    {
                        CHK_FUN_CONTINUE(retVal, dv1_otp_write(unit, serialNumAddr + i, &(pcieParmBuf->serialNum[i]), 1u));
                    }
                }
            }
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dv1_drv_pcie_parm_init(uint32 unit)
{
    RtkApiRet retVal;
    uint8     pcieOtpUsedValidInitValue[1] = {0xEF};
    uint8     pcieOtpUsedValidValue[1];
    do
    {
        retVal = RT_ERR_OK;
        CHK_FUN_CONTINUE(retVal, dv1_otp_read(unit, DV1_PCIe_OTP_START_ADDR, (&pcieOtpUsedValidValue[0u]), 1u));
        if(0xFFu == (pcieOtpUsedValidValue[0]))
        {
            CHK_FUN_CONTINUE(retVal, dv1_otp_write(unit, DV1_PCIe_OTP_START_ADDR, pcieOtpUsedValidInitValue, 1u));
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet dv1_drv_pcie_sdsmode_set(uint32 unit, RtkPort portId)
{
    RtkApiRet retVal;
    do
    {
        if(portId == DV1_RTK_USER_PORT_10TH)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT10_WTG_CTRL_REG_0, DV1_PORT10_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DV1_PORT10_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DV1_SDS_MODE_SG));
        }
        else if(portId == DV1_RTK_USER_PORT_12TH)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT12_WTG_CTRL_REG_0, DV1_PORT12_WTG_CTRL_REG_0_RG_SDS_MODE_LSP, DV1_PORT12_WTG_CTRL_REG_0_RG_SDS_MODE_LEN, DV1_SDS_MODE_SG));
        }
        else
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}