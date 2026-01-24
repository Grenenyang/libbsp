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

#include "rtkas_std.h"
#include "rtkas_common_types.h"
#include "rtkas_error.h"

#include "rtkas_types.h"
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_reg_struct.h"

#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_asicdrv_flash.h"
#include "rtl907xc_va_asicdrv_otp.h"
#include "rtkas_system_porting.h"

uint32 cva_g_rtkTimerVirtualChannel = RTK_TIMER_SYSTEM_VIRTUAL_CHANNEL;

static uint32 cva_g_swIndirectAccessCommandReg = CVA_INDIRECT_ACCESS_CHANNEL0_COMMAND;
static uint32 cva_g_swIndirectAccessAddressReg = CVA_INDIRECT_ACCESS_CHANNEL0_ADDRESS;
static uint32 cva_g_swIndirectAccessDataReg    = CVA_INDIRECT_ACCESS_CHANNEL0_DATA;

static RtkApiRet cva_drv_fw_ver_get(uint32 unit, uint32 *version)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;
    uint32    page   = 0u;
    uint32    ver    = 0u;
    do
    {
        if(NULL == version)
        {
            ret = -RT_ERR_NULL_POINTER;
            break;
        }

        CHK_FUN_CONTINUE(ret, cva_reg_bit_get(unit, CVA_CPU_DMY_0, CVA_DRV_BS_FIRMWARE_VERIFIED_BIT, &regVal));

        if(0UL == regVal)
        {
            /* There is no data in flash, get rom code version */
            CHK_FUN_CONTINUE(ret, cva_reg_page_read(unit, &page));
            CHK_FUN_CONTINUE(ret, cva_reg_page_write(unit, 0x0u));

            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, 0x203ff0, &ver));
            ver = swapl32(ver);

            CHK_FUN_CONTINUE(ret, cva_reg_page_write(unit, page));
        }
        else
        {
            /* Firmware exist in flash, get the version from dummy register */
            CHK_FUN_CONTINUE(ret, cva_reg_read(unit, CVA_CPU_DMY_2, &ver));
        }

        *version = ver;
    } while(false);

    return ret;
}

static RtkApiRet cva_drv_fw_ver_chk(uint32 unit, uint32 fwVer, bool *supFlg)
{
    RtkApiRet retVal;
    uint32    curFwVer;

    retVal = cva_drv_fw_ver_get(unit, &curFwVer);
    if(RT_ERR_OK == retVal)
    {
        if(curFwVer >= fwVer)
        {
            *supFlg = true;
        }
        else
        {
            *supFlg = false;
        }
    }

    return retVal;
}

RtkApiRet cva_drv_hw_indirect_get(uint32 unit, uint32 regAddr, uint32 *regVal)
{
    int32  retVal  = RT_ERR_OK;
    uint32 timeout = (uint32)0u;
    uint32 tmp     = (uint32)0u;
    uint32 tmpAddr = regAddr & (uint32)0xFFFFFFu;
    bool   retry   = false;

    do
    {
        if((g_RegIF == (uint8)REGIF_I2C) || (g_RegIF == (uint8)REGIF_SPI))
        {
            CHK_FUN_CONTINUE(retVal, reg_direct_write((uint32)0xFFFF11u, ((uint32)0x3D000000u | tmpAddr)));

            /* polling flag with timeout */
            do
            {
                retVal = reg_direct_read((uint32)0xFFFF11u, &tmp);
                if((retVal == RT_ERR_OK) && ((tmp & 0x80000000u) != 0x80000000u))
                {
                    (void)rtk_usecond_delay(1u);
                    timeout++;
                    retry = ((timeout < cva_g_rtkTimerVirtualChannel) ? true : false);
                }
                else
                {
                    retry = false;
                }
            } while(retry);

            if(timeout == cva_g_rtkTimerVirtualChannel)
            {
                retVal = -(int32)RT_ERR_BUSYWAIT_TIMEOUT;
            }

            if(retVal != RT_ERR_OK)
            {
                continue;
            }
            /* get data */
            CHK_FUN_CONTINUE(retVal, reg_direct_read((uint32)0xFFFF15u, regVal));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, reg_direct_read(tmpAddr, regVal));
        }
    } while(false);

    return retVal;
}

RtkApiRet cva_drv_hw_indirect_set(uint32 unit, uint32 regAddr, uint32 regVal)
{
    int32  retVal  = RT_ERR_OK;
    uint32 timeout = (uint32)0u;
    uint32 tmp     = (uint32)0u;
    uint32 tmpAddr = regAddr & (uint32)0xFFFFFFu;
    bool   retry   = false;

    do
    {
        if((g_RegIF == (uint8)REGIF_I2C) || (g_RegIF == (uint8)REGIF_SPI))
        {

            CHK_FUN_CONTINUE(retVal, reg_direct_write((uint32)0xFFFF15u, regVal));
            CHK_FUN_CONTINUE(retVal, reg_direct_write((uint32)0xFFFF11u, ((uint32)0x3F000000u | tmpAddr)));

            /* polling flag with timeout */
            do
            {
                retVal = reg_direct_read((uint32)0xFFFF11u, &tmp);
                if((retVal == RT_ERR_OK) && ((tmp & 0x80000000u) != 0x80000000u))
                {
                    (void)rtk_usecond_delay(1u);
                    timeout++;
                    retry = ((timeout < cva_g_rtkTimerVirtualChannel) ? true : false);
                }
                else
                {
                    retry = false;
                }
            } while(retry);

            if(timeout == cva_g_rtkTimerVirtualChannel)
            {
                retVal = -(int32)RT_ERR_BUSYWAIT_TIMEOUT;
            }

            if(retVal != RT_ERR_OK)
            {
                continue;
            }
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, reg_direct_write(tmpAddr, regVal));
        }
    } while(false);

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
DYNAMIC_SCOPE RtkApiRet cva_drv_indirect_access_read_internal(uint32 unit, uint32 target, uint32 addr, uint32 *data)
#else
DYNAMIC_SCOPE RtkApiRet cva_drv_indirect_access_read(uint32 unit, uint32 target, uint32 addr, uint32 *data)
#endif
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    command = ((uint32)1u << CVA_INDIRECT_ACCESS_COMMAND_READ_LSB) | ((uint32)1u << CVA_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB);
    uint32    trigger = 0u;
    uint32    timeout = 0u;
    bool      retry   = true;

    do
    {
        command |= (target << CVA_INDIRECT_ACCESS_COMMAND_TARGET_LSB);

        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, cva_g_swIndirectAccessAddressReg, addr));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, cva_g_swIndirectAccessCommandReg, command));
        CHK_FUN_CONTINUE(retVal, cva_reg_bit_get(unit, cva_g_swIndirectAccessCommandReg, CVA_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger));

        /* polling flag with timeout */
        do
        {
            retVal = cva_reg_bit_get(unit, cva_g_swIndirectAccessCommandReg, CVA_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger);
            if((retVal == RT_ERR_OK) && (trigger == (uint32)0x1u))
            {
                (void)rtk_usecond_delay(1u);
                timeout++;
                retry = ((timeout < cva_g_rtkTimerVirtualChannel) ? true : false);
            }
            else
            {
                retry = false;
            }
        } while(retry);

        if(timeout == cva_g_rtkTimerVirtualChannel)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
        }

        if(retVal == RT_ERR_OK)
        {
            retVal = cva_reg_read(unit, cva_g_swIndirectAccessDataReg, data);
        }

    } while(false);

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
DYNAMIC_SCOPE RtkApiRet cva_drv_indirect_access_write_internal(uint32 unit, uint32 target, uint32 addr, uint32 data)
#else
DYNAMIC_SCOPE RtkApiRet cva_drv_indirect_access_write(uint32 unit, uint32 target, uint32 addr, uint32 data)
#endif
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    command = ((uint32)1u << CVA_INDIRECT_ACCESS_COMMAND_WRITE_LSB) | ((uint32)1u << CVA_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB);
    uint32    trigger = 0u;
    uint32    timeout = 0u;
    bool      retry   = true;

    do
    {
        command |= (target << CVA_INDIRECT_ACCESS_COMMAND_TARGET_LSB);

        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, cva_g_swIndirectAccessAddressReg, addr));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, cva_g_swIndirectAccessDataReg, data));
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, cva_g_swIndirectAccessCommandReg, command));
        CHK_FUN_CONTINUE(retVal, cva_reg_bit_get(unit, cva_g_swIndirectAccessCommandReg, CVA_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger));

        /* polling flag with timeout */
        do
        {
            retVal = cva_reg_bit_get(unit, cva_g_swIndirectAccessCommandReg, CVA_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger);
            if((retVal == RT_ERR_OK) && (trigger == (uint32)0x1u))
            {
                (void)rtk_usecond_delay(1u);
                timeout++;
                retry = ((timeout < cva_g_rtkTimerVirtualChannel) ? true : false);
            }
            else
            {
                retry = false;
            }
        } while(retry);

        if(timeout == cva_g_rtkTimerVirtualChannel)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
        }

    } while(false);

    return retVal;
}

RtkApiRet cva_drv_indirect_channel_set(uint32 channel)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(channel == CVA_RTK_INDIRECT_CHANNEL_PRIMARY)
    {
        cva_g_swIndirectAccessCommandReg = CVA_INDIRECT_ACCESS_CHANNEL0_COMMAND;
        cva_g_swIndirectAccessAddressReg = CVA_INDIRECT_ACCESS_CHANNEL0_ADDRESS;
        cva_g_swIndirectAccessDataReg    = CVA_INDIRECT_ACCESS_CHANNEL0_DATA;
    }
    else if(channel == CVA_RTK_INDIRECT_CHANNEL_SECONDARY)
    {
        cva_g_swIndirectAccessCommandReg = CVA_INDIRECT_ACCESS_CHANNEL1_COMMAND;
        cva_g_swIndirectAccessAddressReg = CVA_INDIRECT_ACCESS_CHANNEL1_ADDRESS;
        cva_g_swIndirectAccessDataReg    = CVA_INDIRECT_ACCESS_CHANNEL1_DATA;
    }
    else
    {
        retVal = -RT_ERR_INPUT;
    }

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cva_drv_indirect_reg_field_get_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal)
#else
RtkApiRet               cva_drv_indirect_reg_field_get(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal;
    bool      supFlg;
    uint32    addr;
    uint32    info = (lsb << CVA_INDIRECT_ACCESS_REG_INFO_LSB_LSB) | (len << CVA_INDIRECT_ACCESS_REG_INFO_LEN_LSB);
    uint32    infoAddr;

    retVal = cva_drv_fw_ver_chk(unit, CVA_DRV_INDIRECT_REG_FIELD_FW_VER, &supFlg);
    if(retVal == RT_ERR_OK)
    {
        if(true == supFlg)
        {
            if(CVA_INDIRECT_ACCESS_CHANNEL0_COMMAND == cva_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | CVA_RTK_INDIRECT_CHANNEL_PRIMARY;
                infoAddr = CVA_INDIRECT_ACCESS_CHANNEL0_DATA1;
            }
            else if(CVA_INDIRECT_ACCESS_CHANNEL1_COMMAND == cva_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | CVA_RTK_INDIRECT_CHANNEL_SECONDARY;
                infoAddr = CVA_INDIRECT_ACCESS_CHANNEL1_DATA1;
            }
            else
            {
                retVal = -RT_ERR_INPUT;
            }

            CHK_FUN_RET(retVal, cva_reg_write(unit, infoAddr, info));
            CHK_FUN_RET(retVal, cva_drv_indirect_access_read(unit, CVA_INDIRECT_ACCESS_TARGET_REGISTER, addr, regVal));
        }
        else
        {
            retVal = cva_reg_field_read(unit, regAddr, lsb, len, regVal);
        }
    }

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cva_drv_indirect_reg_field_set_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal)
#else
RtkApiRet               cva_drv_indirect_reg_field_set(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal;
    bool      supFlg;
    uint32    addr;
    uint32    info = (lsb << CVA_INDIRECT_ACCESS_REG_INFO_LSB_LSB) | (len << CVA_INDIRECT_ACCESS_REG_INFO_LEN_LSB);
    uint32    infoAddr;

    retVal = cva_drv_fw_ver_chk(unit, CVA_DRV_INDIRECT_REG_FIELD_FW_VER, &supFlg);
    if(retVal == RT_ERR_OK)
    {
        if(true == supFlg)
        {
            if(CVA_INDIRECT_ACCESS_CHANNEL0_COMMAND == cva_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | CVA_RTK_INDIRECT_CHANNEL_PRIMARY;
                infoAddr = CVA_INDIRECT_ACCESS_CHANNEL0_DATA1;
            }
            else if(CVA_INDIRECT_ACCESS_CHANNEL1_COMMAND == cva_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | CVA_RTK_INDIRECT_CHANNEL_SECONDARY;
                infoAddr = CVA_INDIRECT_ACCESS_CHANNEL1_DATA1;
            }
            else
            {
                retVal = -RT_ERR_INPUT;
            }

            CHK_FUN_RET(retVal, cva_reg_write(unit, infoAddr, info));
            CHK_FUN_RET(retVal, cva_drv_indirect_access_write(unit, CVA_INDIRECT_ACCESS_TARGET_REGISTER, addr, regVal));
        }
        else
        {
            retVal = cva_reg_field_write(unit, regAddr, lsb, len, regVal);
        }
    }

    return retVal;
}

RtkApiRet cva_drv_indirect_sram_get(uint32 unit, uint32 regAddr, uint32 *regVal)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = regAddr;
    uint32    data   = 0u;

    retVal = cva_drv_indirect_access_read(unit, CVA_INDIRECT_ACCESS_TARGET_SRAM, addr, &data);

    if(retVal == RT_ERR_OK)
    {
        *regVal = data;
    }

    return retVal;
}

RtkApiRet cva_drv_indirect_sram_set(uint32 unit, uint32 regAddr, uint32 regVal)

{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = regAddr;
    uint32    data   = regVal;

    retVal = cva_drv_indirect_access_write(unit, CVA_INDIRECT_ACCESS_TARGET_SRAM, addr, data);

    return retVal;
}

RtkApiRet cva_drv_indirect_sram_1byte_get(uint32 unit, uint32 regAddr, uint8 *regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;

    retVal = cva_drv_indirect_sram_get(unit, alignAddr, &data);

    if(retVal == RT_ERR_OK)
    {
        /* covert offset from byte to bits */
        offset *= 8u;

        *regVal = (uint8)((data >> offset) & 0xFFu);
    }

    return retVal;
}

RtkApiRet cva_drv_indirect_sram_1byte_set(uint32 unit, uint32 regAddr, uint8 regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;

    retVal = cva_drv_indirect_sram_get(unit, alignAddr, &data);

    if(retVal == RT_ERR_OK)
    {
        /* covert offset from byte to bits */
        offset *= 8u;
        data &= ~((uint32)0xFFu << offset);
        data |= ((uint32)regVal << offset);

        retVal = cva_drv_indirect_sram_set(unit, alignAddr, data);
    }
    return retVal;
}

RtkApiRet cva_drv_indirect_sram_2bytes_get(uint32 unit, uint32 regAddr, uint16 *regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;
    uint8     dataLo    = 0u;
    uint8     dataHi    = 0u;

    if(offset != 3u)
    {
        retVal = cva_drv_indirect_sram_get(unit, alignAddr, &data);

        if(RT_ERR_OK == retVal)
        {
            /* covert offset from byte to bits */
            offset *= 8u;
            *regVal = (uint16)((data >> offset) & 0xFFFFu);
        }
    }
    else
    {
        /* the offset equal 3 means this action cross 2 4-byte alignment address */

        /* set Low byte for 1st 4 byte */
        retVal = cva_drv_indirect_sram_1byte_get(unit, regAddr, &dataLo);
        if(RT_ERR_OK == retVal)
        {
            /* set High byte for 2nd 4 byte */
            retVal = cva_drv_indirect_sram_1byte_get(unit, regAddr + 1u, &dataHi);
        }

        if(RT_ERR_OK == retVal)
        {
            *regVal = ((uint16)dataHi << 8u) | ((uint16)dataLo);
        }
    }

    return retVal;
}

RtkApiRet cva_drv_indirect_sram_2bytes_set(uint32 unit, uint32 regAddr, uint16 regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;

    if(offset != 3u)
    {
        retVal = cva_drv_indirect_sram_get(unit, alignAddr, &data);

        if(RT_ERR_OK == retVal)
        {
            /* covert offset from byte to bits */
            offset *= 8u;
            data &= ~(((uint32)0xFFFFu) << offset);
            data |= ((uint32)regVal << offset);

            retVal = cva_drv_indirect_sram_set(unit, alignAddr, data);
        }
    }
    else
    {
        /* the offset equal 3 means this action cross 2 4-byte alignment address */

        /* set Low byte for 1st 4 byte */
        retVal = cva_drv_indirect_sram_1byte_set(unit, regAddr, (uint8)((regVal & 0x00FFu) >> 0u));
        /* set High byte for 2nd 4 byte */
        if(RT_ERR_OK == retVal)
        {
            retVal = cva_drv_indirect_sram_1byte_set(unit, regAddr + 1u, (uint8)((regVal & 0xFF00u) >> 8u));
        }
    }

    return retVal;
}

RtkApiRet cva_drv_indirect_serdes_get(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 *regVal)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = 0u;
    uint32    data   = 0u;

    addr |= ((port & 0x1Fu) << CVA_INDIDRCT_ACCESS_ADDRESS_SERDES_PORT_LSB);
    addr |= ((type & 0x1u) << CVA_INDIDRCT_ACCESS_ADDRESS_TYPE_PORT_LSB);
    addr |= ((page & 0x3FFu) << CVA_INDIDRCT_ACCESS_ADDRESS_PAGE_PORT_LSB);
    addr |= (regAddr & 0xFFFFu);

    retVal = cva_drv_indirect_access_read(unit, CVA_INDIRECT_ACCESS_TARGET_SERDES, addr, &data);

    if(retVal == RT_ERR_OK)
    {
        *regVal = data;
    }

    return retVal;
}

RtkApiRet cva_drv_indirect_serdes_set(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = 0u;
    uint32    data   = regVal;

    addr |= ((port & 0x1Fu) << CVA_INDIDRCT_ACCESS_ADDRESS_SERDES_PORT_LSB);
    addr |= ((type & 0x1u) << CVA_INDIDRCT_ACCESS_ADDRESS_TYPE_PORT_LSB);
    addr |= ((page & 0x3FFu) << CVA_INDIDRCT_ACCESS_ADDRESS_PAGE_PORT_LSB);
    addr |= (regAddr & 0xFFFFu);

    retVal = cva_drv_indirect_access_write(unit, CVA_INDIRECT_ACCESS_TARGET_SERDES, addr, data);

    return retVal;
}
