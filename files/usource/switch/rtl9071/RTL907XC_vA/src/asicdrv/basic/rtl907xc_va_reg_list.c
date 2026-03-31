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
#include "rtkas_debug.h"
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_reg_define.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_asicdrv_ethernet_access.h"

bool cva_reg_check_unit(uint32 unit)
{
#ifdef RTK_MODULE_STACKING
    return (unit == CVA_UNIT);
#else
    (void)unit;
    return true;
#endif
}

RtkApiRet cva_reg_read(uint32 unit, uint32 regAddr, uint32 *value)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(true == cva_reg_check_unit(unit))
    {
        retVal = reg_direct_read(regAddr, value);
    }
    else
    {
#ifdef RTK_MODULE_STACKING
        retVal = cva_drv_stacking_reg_read(unit, regAddr, value);
#endif
    }

    return retVal;
}

RtkApiRet cva_reg_write(uint32 unit, uint32 regAddr, uint32 value)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(true == cva_reg_check_unit(unit))
    {
        retVal = reg_direct_write(regAddr, value);
    }
    else
    {
#ifdef RTK_MODULE_STACKING
        retVal = cva_drv_stacking_reg_write(unit, regAddr, value);
#endif
    }

    return retVal;
}

RtkApiRet cva_reg_write_array(uint32 unit, uint32 regAddr[], uint32 value[], uint32 num)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    i      = 0u;

    if((NULL == regAddr) || (NULL == value))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == retVal)
    {
        while(i < num)
        {
            CHK_FUN_BREAK(retVal, cva_reg_write(unit, regAddr[i], value[i]));
            i++;
        }
    }

    return retVal;
}
RtkApiRet cva_reg_field_get(uint32 regAddr, uint32 lsp, uint32 len, uint32 *value, uint32 data)
{
    RtkApiRet retVal;
    uint32    ret;

    retVal = RT_ERR_OK;

    if((lsp > (uint32)31u) || (len == (uint32)0u) || (((lsp + len) - (uint32)1u) > (uint32)31u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    if(RT_ERR_OK == retVal)
    {
        ret = data >> lsp;
        if(len < CVA_REG_DATA_LEN)
        {
            *value = ret & (((uint32)1u << len) - (uint32)1u);
        }
        else
        {
            *value = ret;
        }
    }

    return retVal;
}

RtkApiRet cva_reg_field_set(uint32 regAddr, uint32 lsp, uint32 len, uint32 value, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    mask   = 0u;

    if((lsp > (uint32)31u) || (len == (uint32)0u) || (((lsp + len) - (uint32)1u) > (uint32)31u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == data)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    if(RT_ERR_OK == retVal)
    {
        if(len < CVA_REG_DATA_LEN)
        {
            mask = ((uint32)1u << len) - (uint32)1u;
        }
        else
        {
            mask = CVA_REG_DATA_MASK;
        }

        *data &= ~(mask << lsp);
        *data |= ((value & mask) << lsp);
    }

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cva_reg_field_read_internal(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value)
#else
RtkApiRet cva_reg_field_read(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    data   = 0u;

    if((lsp > (uint32)31u) || (len == (uint32)0u) || (((lsp + len) - (uint32)1u) > (uint32)31u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        retVal = cva_reg_read(unit, regAddr, &data);
        CHK_FUN_RET(retVal, cva_reg_field_get(regAddr, lsp, len, value, data));
    }

    return retVal;
}

#ifdef RTK_INTERNAL_DEV
RtkApiRet cva_reg_field_write_internal(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 value)
#else
RtkApiRet cva_reg_field_write(uint32 unit, uint32 regAddr, uint32 lsp, uint32 len, uint32 value)
#endif /* RTK_INTERNAL_DEV */
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    data   = 0u;

    retVal = cva_reg_read(unit, regAddr, &data);
    CHK_FUN_RET(retVal, cva_reg_field_set(regAddr, lsp, len, value, &data));
    CHK_FUN_RET(retVal, cva_reg_write(unit, regAddr, data));

    return retVal;
}

RtkApiRet cva_reg_bit_get(uint32 unit, uint32 reg, uint32 bit, uint32 *value)
{
    uint32 regData = 0u;
    int32  retVal  = RT_ERR_OK;

    if(bit > (uint32)31u)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        retVal = cva_reg_read(unit, reg, &regData);
        if(RT_ERR_OK == retVal)
        {
            *value = (regData >> bit) & (uint32)0x1u;
        }
    }

    return retVal;
}

RtkApiRet cva_reg_bit_set(uint32 unit, uint32 reg, uint32 bit, uint32 value)
{
    uint32 regData = 0u;
    int32  retVal  = RT_ERR_OK;

    do
    {
        if(bit > (uint32)31u)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, reg, &regData));
            if(value != 0u)
            {
                regData |= (uint32)0x1u << bit;
            }
            else
            {
                regData &= ~((uint32)0x1u << bit);
            }
            CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, reg, regData));
        }
    } while(0u == 1u);

    return retVal;
}

RtkApiRet cva_reg_page_read(uint32 unit, uint32 *page)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == page)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(true == cva_reg_check_unit(unit))
    {
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            retVal = -RT_ERR_FAILED;
        }
        else
        {
            retVal = reg_direct_read(CVA_REG_PAGE_ADDR, page);
        }
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}

RtkApiRet cva_reg_page_write(uint32 unit, uint32 page)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(true == cva_reg_check_unit(unit))
    {
        if(g_RegIF == (uint8)REGIF_ETHERNET)
        {
            retVal = cva_drv_eth_acc_flow_page_select(page);
        }
        else
        {
            retVal = reg_direct_write(CVA_REG_PAGE_ADDR, page);
        }
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}

RtkApiRet cva_reg_read_burst(uint32 unit, uint32 regAddr, uint32 *value, uint32 length)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(true == cva_reg_check_unit(unit))
    {
        retVal = reg_direct_read_burst(regAddr, value, length);
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}
