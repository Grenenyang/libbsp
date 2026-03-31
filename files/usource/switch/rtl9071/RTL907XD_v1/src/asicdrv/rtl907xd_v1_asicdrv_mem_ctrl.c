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
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_regField_list.h"
#include "rtl907xd_v1_reg_struct.h"

#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#include "rtl907xd_v1_asicdrv_flash.h"
#include "rtl907xd_v1_asicdrv_otp.h"
#include "rtkas_system_porting.h"

uint32 dv1_g_rtkTimerVirtualChannel = RTK_TIMER_SYSTEM_VIRTUAL_CHANNEL;

static uint32 dv1_g_swIndirectAccessCommandReg = DV1_INDIRECT_ACCESS_CHANNEL0_COMMAND;
static uint32 dv1_g_swIndirectAccessAddressReg = DV1_INDIRECT_ACCESS_CHANNEL0_ADDRESS;
static uint32 dv1_g_swIndirectAccessDataReg    = DV1_INDIRECT_ACCESS_CHANNEL0_DATA;

static RtkApiRet dv1_sys_fw_ver_get(uint32 unit, uint32 *version)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;
    do
    {
        if(NULL == version)
        {
            ret = -RT_ERR_NULL_POINTER;
            break;
        }

        /* Firmware exist in flash, get the version from dummy register */
        CHK_FUN_CONTINUE(ret, dv1_reg_read(unit, DV1_CPU_DMY_2, &regVal));

        *version = regVal;
    } while(false);

    return ret;
}

static RtkApiRet dv1_drv_fw_ver_chk(uint32 unit, uint32 fwVer, bool *supFlg)
{
    RtkApiRet retVal;
    uint32    curFwVer;

    retVal = dv1_sys_fw_ver_get(unit, &curFwVer);
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

RtkApiRet dv1_drv_hw_indirect_get(uint32 unit, uint32 regAddr, uint32 *regVal)
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
                if((RT_ERR_OK == retVal) && ((tmp & 0x80000000u) != 0x80000000u))
                {
                    (void)rtk_usecond_delay(1u);
                    timeout++;
                    retry = ((timeout < dv1_g_rtkTimerVirtualChannel) ? true : false);
                }
                else
                {
                    retry = false;
                }
            } while(retry);

            if(timeout == dv1_g_rtkTimerVirtualChannel)
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

RtkApiRet dv1_drv_hw_indirect_set(uint32 unit, uint32 regAddr, uint32 regVal)
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
                if((RT_ERR_OK == retVal) && ((tmp & 0x80000000u) != 0x80000000u))
                {
                    (void)rtk_usecond_delay(1u);
                    timeout++;
                    retry = ((timeout < dv1_g_rtkTimerVirtualChannel) ? true : false);
                }
                else
                {
                    retry = false;
                }
            } while(retry);

            if(timeout == dv1_g_rtkTimerVirtualChannel)
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
DYNAMIC_SCOPE RtkApiRet dv1_drv_indirect_access_read_internal(uint32 unit, uint32 target, uint32 addr, uint32 *data)
#else
DYNAMIC_SCOPE RtkApiRet dv1_drv_indirect_access_read(uint32 unit, uint32 target, uint32 addr, uint32 *data)
#endif
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    command = ((uint32)1u << DV1_INDIRECT_ACCESS_COMMAND_READ_LSB) | ((uint32)1u << DV1_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB);
    uint32    trigger = 0u;
    uint32    error   = 0u;
    uint32    timeout = 0u;
    bool      retry   = true;

    do
    {
        command |= (target << DV1_INDIRECT_ACCESS_COMMAND_TARGET_LSB);

        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, dv1_g_swIndirectAccessAddressReg, addr));
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, dv1_g_swIndirectAccessCommandReg, command));
        CHK_FUN_CONTINUE(retVal, dv1_reg_bit_get(unit, dv1_g_swIndirectAccessCommandReg, DV1_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger));

        /* polling flag with timeout */
        do
        {
            retVal = dv1_reg_bit_get(unit, dv1_g_swIndirectAccessCommandReg, DV1_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger);
            if((RT_ERR_OK == retVal) && ((uint32)0x1u == trigger))
            {
                (void)rtk_usecond_delay(1u);
                timeout++;
                retry = ((timeout < dv1_g_rtkTimerVirtualChannel) ? true : false);
            }
            else
            {
                retry = false;
            }
        } while(retry);

        if(timeout == dv1_g_rtkTimerVirtualChannel)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        retVal = dv1_reg_bit_get(unit, dv1_g_swIndirectAccessCommandReg, DV1_INDIRECT_ACCESS_COMMAND_ERROR_LSB, &error);
        if((RT_ERR_OK != retVal) || ((uint32)0x0u != error))
        {
            retVal = -RT_ERR_FAILED;
            continue;
        }

        retVal = dv1_reg_read(unit, dv1_g_swIndirectAccessDataReg, data);
    } while(false);

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
DYNAMIC_SCOPE RtkApiRet dv1_drv_indirect_access_write_internal(uint32 unit, uint32 target, uint32 addr, uint32 data)
#else
DYNAMIC_SCOPE RtkApiRet dv1_drv_indirect_access_write(uint32 unit, uint32 target, uint32 addr, uint32 data)
#endif
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    command = ((uint32)1u << DV1_INDIRECT_ACCESS_COMMAND_WRITE_LSB) | ((uint32)1u << DV1_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB);
    uint32    trigger = 0u;
    uint32    error   = 0u;
    uint32    timeout = 0u;
    bool      retry   = true;

    do
    {
        command |= (target << DV1_INDIRECT_ACCESS_COMMAND_TARGET_LSB);

        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, dv1_g_swIndirectAccessAddressReg, addr));
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, dv1_g_swIndirectAccessDataReg, data));
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, dv1_g_swIndirectAccessCommandReg, command));
        CHK_FUN_CONTINUE(retVal, dv1_reg_bit_get(unit, dv1_g_swIndirectAccessCommandReg, DV1_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger));

        /* polling flag with timeout */
        do
        {
            retVal = dv1_reg_bit_get(unit, dv1_g_swIndirectAccessCommandReg, DV1_INDIRECT_ACCESS_COMMAND_TRIGGER_LSB, &trigger);
            if((RT_ERR_OK == retVal) && ((uint32)0x1u == trigger))
            {
                (void)rtk_usecond_delay(1u);
                timeout++;
                retry = ((timeout < dv1_g_rtkTimerVirtualChannel) ? true : false);
            }
            else
            {
                retry = false;
            }
        } while(retry);

        if(timeout == dv1_g_rtkTimerVirtualChannel)
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
        retVal = dv1_reg_bit_get(unit, dv1_g_swIndirectAccessCommandReg, DV1_INDIRECT_ACCESS_COMMAND_ERROR_LSB, &error);
        if((RT_ERR_OK != retVal) || ((uint32)0x0u != error))
        {
            retVal = -RT_ERR_FAILED;
        }
    } while(false);

    return retVal;
}

RtkApiRet dv1_drv_indirect_channel_set(uint32 channel)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(channel == DV1_RTK_INDIRECT_CHANNEL_PRIMARY)
    {
        dv1_g_swIndirectAccessCommandReg = DV1_INDIRECT_ACCESS_CHANNEL0_COMMAND;
        dv1_g_swIndirectAccessAddressReg = DV1_INDIRECT_ACCESS_CHANNEL0_ADDRESS;
        dv1_g_swIndirectAccessDataReg    = DV1_INDIRECT_ACCESS_CHANNEL0_DATA;
    }
    else if(channel == DV1_RTK_INDIRECT_CHANNEL_SECONDARY)
    {
        dv1_g_swIndirectAccessCommandReg = DV1_INDIRECT_ACCESS_CHANNEL1_COMMAND;
        dv1_g_swIndirectAccessAddressReg = DV1_INDIRECT_ACCESS_CHANNEL1_ADDRESS;
        dv1_g_swIndirectAccessDataReg    = DV1_INDIRECT_ACCESS_CHANNEL1_DATA;
    }
    else
    {
        retVal = -RT_ERR_INPUT;
    }

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet dv1_drv_indirect_reg_field_get_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal)
#else
RtkApiRet               dv1_drv_indirect_reg_field_get(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal;
    bool      supFlg;
    uint32    addr;
    uint32    info = (lsb << DV1_INDIRECT_ACCESS_REG_INFO_LSB_LSB) | (len << DV1_INDIRECT_ACCESS_REG_INFO_LEN_LSB);
    uint32    infoAddr;

    retVal = dv1_drv_fw_ver_chk(unit, DV1_DRV_INDIRECT_REG_FIELD_FW_VER, &supFlg);
    if(retVal == RT_ERR_OK)
    {
        if(true == supFlg)
        {
            if(DV1_INDIRECT_ACCESS_CHANNEL0_COMMAND == dv1_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | DV1_RTK_INDIRECT_CHANNEL_PRIMARY;
                infoAddr = DV1_INDIRECT_ACCESS_CHANNEL0_DATA1;
            }
            else if(DV1_INDIRECT_ACCESS_CHANNEL1_COMMAND == dv1_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | DV1_RTK_INDIRECT_CHANNEL_SECONDARY;
                infoAddr = DV1_INDIRECT_ACCESS_CHANNEL1_DATA1;
            }
            else
            {
                retVal = -RT_ERR_INPUT;
            }

            CHK_FUN_RET(retVal, dv1_reg_write(unit, infoAddr, info));
            CHK_FUN_RET(retVal, dv1_drv_indirect_access_read(unit, DV1_INDIRECT_ACCESS_TARGET_REGISTER, addr, regVal));
        }
        else
        {
            retVal = dv1_reg_field_read(unit, regAddr, lsb, len, regVal);
        }
    }

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet dv1_drv_indirect_reg_field_set_internal(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal)
#else
RtkApiRet               dv1_drv_indirect_reg_field_set(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 regVal)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal;
    bool      supFlg;
    uint32    addr;
    uint32    info = (lsb << DV1_INDIRECT_ACCESS_REG_INFO_LSB_LSB) | (len << DV1_INDIRECT_ACCESS_REG_INFO_LEN_LSB);
    uint32    infoAddr;

    retVal = dv1_drv_fw_ver_chk(unit, DV1_DRV_INDIRECT_REG_FIELD_FW_VER, &supFlg);
    if(retVal == RT_ERR_OK)
    {
        if(true == supFlg)
        {
            if(DV1_INDIRECT_ACCESS_CHANNEL0_COMMAND == dv1_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | DV1_RTK_INDIRECT_CHANNEL_PRIMARY;
                infoAddr = DV1_INDIRECT_ACCESS_CHANNEL0_DATA1;
            }
            else if(DV1_INDIRECT_ACCESS_CHANNEL1_COMMAND == dv1_g_swIndirectAccessCommandReg)
            {
                addr     = regAddr | DV1_RTK_INDIRECT_CHANNEL_SECONDARY;
                infoAddr = DV1_INDIRECT_ACCESS_CHANNEL1_DATA1;
            }
            else
            {
                retVal = -RT_ERR_INPUT;
            }

            CHK_FUN_RET(retVal, dv1_reg_write(unit, infoAddr, info));
            CHK_FUN_RET(retVal, dv1_drv_indirect_access_write(unit, DV1_INDIRECT_ACCESS_TARGET_REGISTER, addr, regVal));
        }
        else
        {
            retVal = dv1_reg_field_write(unit, regAddr, lsb, len, regVal);
        }
    }

    return retVal;
}

RtkApiRet dv1_drv_indirect_sram_get(uint32 unit, uint32 regAddr, uint32 *regVal)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = regAddr;
    uint32    data   = 0u;

    retVal = dv1_drv_indirect_access_read(unit, DV1_INDIRECT_ACCESS_TARGET_SRAM, addr, &data);

    if(RT_ERR_OK == retVal)
    {
        *regVal = data;
    }

    return retVal;
}

RtkApiRet dv1_drv_indirect_sram_set(uint32 unit, uint32 regAddr, uint32 regVal)

{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = regAddr;
    uint32    data   = regVal;

    retVal = dv1_drv_indirect_access_write(unit, DV1_INDIRECT_ACCESS_TARGET_SRAM, addr, data);

    return retVal;
}

RtkApiRet dv1_drv_indirect_sram_1byte_get(uint32 unit, uint32 regAddr, uint8 *regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;

    retVal = dv1_drv_indirect_sram_get(unit, alignAddr, &data);

    if(RT_ERR_OK == retVal)
    {
        /* covert offset from byte to bits */
        offset *= 8u;

        *regVal = (uint8)((data >> offset) & 0xFFu);
    }

    return retVal;
}

RtkApiRet dv1_drv_indirect_sram_1byte_set(uint32 unit, uint32 regAddr, uint8 regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;

    retVal = dv1_drv_indirect_sram_get(unit, alignAddr, &data);

    if(RT_ERR_OK == retVal)
    {
        /* covert offset from byte to bits */
        offset *= 8u;
        data &= ~((uint32)0xFFu << offset);
        data |= ((uint32)regVal << offset);

        retVal = dv1_drv_indirect_sram_set(unit, alignAddr, data);
    }
    return retVal;
}

RtkApiRet dv1_drv_indirect_sram_2bytes_get(uint32 unit, uint32 regAddr, uint16 *regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;
    uint8     dataLo    = 0u;
    uint8     dataHi    = 0u;

    if(offset != 3u)
    {
        retVal = dv1_drv_indirect_sram_get(unit, alignAddr, &data);

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
        retVal = dv1_drv_indirect_sram_1byte_get(unit, regAddr, &dataLo);
        if(RT_ERR_OK == retVal)
        {
            /* set High byte for 2nd 4 byte */
            retVal = dv1_drv_indirect_sram_1byte_get(unit, regAddr + 1u, &dataHi);
        }

        if(RT_ERR_OK == retVal)
        {
            *regVal = ((uint16)dataHi << 8u) | ((uint16)dataLo);
        }
    }

    return retVal;
}

RtkApiRet dv1_drv_indirect_sram_2bytes_set(uint32 unit, uint32 regAddr, uint16 regVal)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    data      = 0u;
    uint32    offset    = (regAddr % 4u);
    uint32    alignAddr = regAddr - offset;

    if(offset != 3u)
    {
        retVal = dv1_drv_indirect_sram_get(unit, alignAddr, &data);

        if(RT_ERR_OK == retVal)
        {
            /* covert offset from byte to bits */
            offset *= 8u;
            data &= ~(((uint32)0xFFFFu) << offset);
            data |= ((uint32)regVal << offset);

            retVal = dv1_drv_indirect_sram_set(unit, alignAddr, data);
        }
    }
    else
    {
        /* the offset equal 3 means this action cross 2 4-byte alignment address */

        /* set Low byte for 1st 4 byte */
        retVal = dv1_drv_indirect_sram_1byte_set(unit, regAddr, (uint8)((regVal & 0x00FFu) >> 0u));
        /* set High byte for 2nd 4 byte */
        if(RT_ERR_OK == retVal)
        {
            retVal = dv1_drv_indirect_sram_1byte_set(unit, regAddr + 1u, (uint8)((regVal & 0xFF00u) >> 8u));
        }
    }

    return retVal;
}

RtkApiRet dv1_drv_indirect_serdes_get(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 *regVal)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = 0u;
    uint32    data   = 0u;

    addr |= ((port & 0x1Fu) << DV1_INDIDRCT_ACCESS_ADDRESS_SERDES_PORT_LSB);
    addr |= ((type & 0x1u) << DV1_INDIDRCT_ACCESS_ADDRESS_TYPE_PORT_LSB);
    addr |= ((page & 0x3FFu) << DV1_INDIDRCT_ACCESS_ADDRESS_PAGE_PORT_LSB);
    addr |= (regAddr & 0xFFFFu);

    retVal = dv1_drv_indirect_access_read(unit, DV1_INDIRECT_ACCESS_TARGET_SERDES, addr, &data);

    if(RT_ERR_OK == retVal)
    {
        *regVal = data;
    }

    return retVal;
}

RtkApiRet dv1_drv_indirect_serdes_set(uint32 unit, uint32 type, uint32 port, uint32 page, uint32 regAddr, uint32 regVal)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = 0u;
    uint32    data   = regVal;

    addr |= ((port & 0x1Fu) << DV1_INDIDRCT_ACCESS_ADDRESS_SERDES_PORT_LSB);
    addr |= ((type & 0x1u) << DV1_INDIDRCT_ACCESS_ADDRESS_TYPE_PORT_LSB);
    addr |= ((page & 0x3FFu) << DV1_INDIDRCT_ACCESS_ADDRESS_PAGE_PORT_LSB);
    addr |= (regAddr & 0xFFFFu);

    retVal = dv1_drv_indirect_access_write(unit, DV1_INDIRECT_ACCESS_TARGET_SERDES, addr, data);

    return retVal;
}
