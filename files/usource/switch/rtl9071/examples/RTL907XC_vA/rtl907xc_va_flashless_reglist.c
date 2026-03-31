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
#include <stdio.h>
#include "rtkas_api_ext.h"
#include "rtkas_api_sys_boot.h"

/**
 * @addtogroup EXAMPLE
 * @{
 */

/**
 * @addtogroup FLASHLESS Flashless Example
 * @{
 */
/**
 * @addtogroup RTL907C_VA_FLASHLESS_EXAMPLE RTL907XC_VA Flashless Example
 * @{
 */
/**
 * @addtogroup RTL907C_VA_FLASHLESS_EXAMPLE_FLASHLESS_REGLIST Flashless Reglist Example
 * @brief <b>Flashless reg-List update flow</b>
 *
 * @section Purpose
 *   Use reg-list to update firmware and configuration.
 *
 * @section Description
 *   This example demonstrates how to use reg-list to update the firmware and configuration via the reg interface in flashless mode.

 * @{
 */
RtkApiRet cva_sys_update_firmware_reglist(UnitChip_t unitChip);
RtkApiRet cva_sys_update_firmware_reglist(UnitChip_t unitChip)
{
    RtkApiRet    retVal   = RT_ERR_OK;
    const int8  *fileName = "image/RTL907XC_vA/flashless/FwPatch_JsonToRegList.txt";
    FILE        *fp       = NULL;
    int8         line[100];
    uint32       address    = 0u;
    uint32       data       = 0u;
    uint32       lsp        = 0u;
    uint32       length     = 0u;
    uint32       mask       = 0u;
    uint32       regVal     = 0u;
    uint32       timeOut    = 0u;
    uint32       timeOutCnt = 0u;
    uint8        breakFlag  = 0u;
    int8         mode;
    int32        matched;
    SysCheckFl_t sysCheckFl;
    do
    {
        /* Step 1. Check flashless mode. Set byPassEth to 1 to skip link checking */
        sysCheckFl.byPassEth = 1u;
        CHK_FUN_CONTINUE(retVal, rtk_sys_check_flashless_mode(unitChip, &sysCheckFl));

        /* Step2  : Open the file, read the file and set the switch according to the corresponding format*/
        retVal = rtl_file_open((void **)&fp, 0, fileName);
        if((fp == NULL) || (RT_ERR_OK != retVal))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        while((fgets(line, 100, fp) != NULL) && (RT_ERR_OK == retVal))
        {
            /* Check for 'W' mode*/
            matched = sscanf(line, "{%c,%x,%x}", &mode, &address, &data);
            if((matched == 3) && (mode == 'W'))
            {
                CHK_FUN_CONTINUE(retVal, reg_write(unitChip, address, data));
                continue;
            }

            /* Check for 'F' mode*/
            matched = sscanf(line, "{%c,%x,%x,%x,%x}", &mode, &address, &lsp, &length, &data);
            if((matched == 5) && (mode == 'F'))
            {
                CHK_FUN_CONTINUE(retVal, reg_field_write(unitChip, address, lsp, length, data));
                continue;
            }

            /* Check for 'P' mode*/
            matched = sscanf(line, "{%c,%x,%x,%x,%x,%x}", &mode, &address, &lsp, &length, &data, &timeOut);
            if((matched == 6) && (mode == 'P'))
            {
                do
                {
                    if(((uint32)31u < lsp) || ((uint32)0u == length) || (((uint32)31u) < ((lsp + length) - (uint32)1u)))
                    {
                        retVal = -RT_ERR_INPUT;
                        continue;
                    }
                    if(32u == length)
                    {
                        mask = 0xFFFFFFFFu;
                    }
                    else
                    {
                        mask = (((uint32)1u << length) - 1u);
                    }

                    while((0u == breakFlag) && (RT_ERR_OK == retVal))
                    {
                        if((timeOut <= timeOutCnt))
                        {
                            breakFlag = 1u;
                            retVal    = -RT_ERR_BUSYWAIT_TIMEOUT;
                            continue;
                        }
                        timeOutCnt++;
                        CHK_FUN_CONTINUE(retVal, reg_read(unitChip, address, &regVal));
                        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1u));
                        if(((regVal >> lsp) & mask) == data)
                        {
                            breakFlag = 1u;
                        }
                    }
                } while(0u == 1u);
                continue;
            }
            /* Check for 'D' mode*/
            matched = sscanf(line, "{%c,%x}", &mode, &timeOut);
            if((matched == 2) && (mode == 'D'))
            {
                CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(timeOut));
            }
        }
        rtl_file_close((void **)&fp);
        /* Step 3. Leave flashless*/
        CHK_FUN_CONTINUE(retVal, rtk_sys_leave_flashless(unitChip));
        /* Step 4. Check leave flashless is successful*/
        CHK_FUN_CONTINUE(retVal, rtk_sys_check_leave_flashless(unitChip));

    } while(0u == 1u);

    /* Expected Behaviour:
     *   Check the reg 0x4A1A0004 is 0xBFxx_xxxx.
     *   The check function return value "retVal" must return RT_ERR_OK.
     */
    return retVal;
}
/**@}*/ /* FLASHLESS_REGLIST */
/**@}*/ /* RTL907C_VA_FLASHLESS_EXAMPLE */
/**@}*/ /* FLASHLESS */
/**@}*/ /* EXAMPLE */
