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
#include "rtkas_timer.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_api_esm.h"
#include "rtl907xd_va_asicdrv_timer.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtkas_system_porting.h"

#define ACCESS_NOT_READY (0xFFFFFFFu)
typedef RtkApiRet (*dva_RegFieldFunc)(uint32 unit, uint32 regAddr, uint32 lsb, uint32 len, uint32 *regVal);

static RtkApiRet dva_drv_timer_polling_reg_lastTime(uint32 unit, RegPollingConfig_t *pollingConf)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    flag   = 0u;
    do
    {
        /* if polling register doesn't check port clock */
        if(DIRECT_ACCESS_REG == pollingConf->chkPortClk)
        {
            /* get access done flag */
            CHK_FUN_RET(retVal, dva_reg_field_read(unit, pollingConf->pollingRegAddr, pollingConf->pollingRegLsp, pollingConf->pollingRegLen, &flag));
        }
        else if(INDIRECT_ACCESS_REG_TARGET == pollingConf->chkPortClk)
        {
            /* get access done flag */
            CHK_FUN_RET(retVal, dva_drv_indirect_target_get_reg_field(unit, pollingConf->pollingRegAddr, pollingConf->pollingRegLsp, pollingConf->pollingRegLen, &flag));
        }
        else if(INDIRECT_ACCESS_PER_PORT_REG_TARGET == pollingConf->chkPortClk)
        {
            /* get access done flag */
            CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_get(unit, pollingConf->pollingRegAddr + (PERPORT_REG_OFFSET * pollingConf->port), pollingConf->pollingRegLsp, pollingConf->pollingRegLen, &flag));
        }
        else
        {
            /*error handle*/
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if(pollingConf->expVal != flag)
        {
            /*log error code*/
            dva_esm_event_log_set(pollingConf->eventID, DVA_EM_EVENT_STATUS_FAILED);
            /* timeout */
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
        }
        else
        {
            /* success */
            retVal = RT_ERR_OK;
        }
    } while(0u == 1u);

    /*return the retVal*/
    return retVal;
}

static RtkApiRet dva_drv_timer_polling_reg(uint32 unit, RegPollingConfig_t *pollingConf, uint32 startTime, uint32 maxUs, uint32 *flag)
{
    uint32           currentTime     = startTime;
    RtkApiRet        retVal          = RT_ERR_OK;
    dva_RegFieldFunc regFieldFuncPtr = NULL;

    do
    {
        /* if polling register doesn't check port clock */
        if(INDIRECT_ACCESS_PER_PORT_REG_TARGET >= pollingConf->chkPortClk)
        {
            /* while loop: check the polling time */
            while((maxUs > (currentTime - startTime)) && (pollingConf->expVal != *flag))
            {
                /* get end time */
                CHK_FUN_RET(retVal, rtk_timer_get_current(&currentTime));

                if(DIRECT_ACCESS_REG == pollingConf->chkPortClk)
                {
                    /* Direct access done flag */
                    regFieldFuncPtr = &dva_reg_field_read;
                }
                else if(INDIRECT_ACCESS_REG_TARGET == pollingConf->chkPortClk)
                {
                    /* Indirect access done flag by reg target channel */
                    regFieldFuncPtr = &dva_drv_indirect_target_get_reg_field;
                }
                else
                {
                    /* Indirect access done flag by per port reg target channel */
                    regFieldFuncPtr = &dva_drv_indirect_reg_field_get;
                }
                CHK_FUN_RET(retVal, regFieldFuncPtr(unit, pollingConf->pollingRegAddr, pollingConf->pollingRegLsp, pollingConf->pollingRegLen, flag));
            }
        }
        else
        {
            /*error handle*/
            retVal = -RT_ERR_INPUT;
        }
    } while(0u == 1u);

    /*return the retVal*/
    return retVal;
}

static RtkApiRet dva_drv_timer_trigger_reg_set(uint32 unit, RegPollingConfig_t *pollingConf)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* if polling register doesn't check port clock */
        if(DIRECT_ACCESS_REG == pollingConf->chkPortClk)
        {
            /* trigger read or write action */
            CHK_FUN_RET(retVal, dva_reg_field_write(unit, pollingConf->triggerRegAddr, pollingConf->triggerRegLsp, pollingConf->triggerRegLen, pollingConf->action));
        }
        else if(INDIRECT_ACCESS_REG_TARGET == pollingConf->chkPortClk)
        {
            /* trigger indirect register target read or write action */
            CHK_FUN_RET(retVal, dva_drv_indirect_target_set_reg_field(unit, pollingConf->triggerRegAddr, pollingConf->triggerRegLsp, pollingConf->triggerRegLen, pollingConf->action));
        }
        else if(INDIRECT_ACCESS_PER_PORT_REG_TARGET == pollingConf->chkPortClk)
        {
            /* trigger indirect per port register target read or write action */
            CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, pollingConf->triggerRegAddr + (PERPORT_REG_OFFSET * pollingConf->port), pollingConf->triggerRegLsp, pollingConf->triggerRegLen, pollingConf->action));
        }
        else
        {
            /*error handle*/
            retVal = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    /*return the retVal*/
    return retVal;
}

RtkApiRet dva_drv_timer_polling_reg_timeout(uint32 unit, RegPollingConfig_t *pollingConf, uint32 maxUs, uint32 minUs)
{
    /*Declaration and initialization*/
    uint32    flag        = ACCESS_NOT_READY;
    RtkApiRet retVal      = RT_ERR_OK;
    uint32    startTime   = 0u;
    uint32    currentTime = 0u;
    uint32    timeSpent   = 0u;

    do
    {
        /* error check */
        if(pollingConf == NULL)
        {
            retVal = -RT_ERR_SDS_NULL_VALUE;
            continue;
        }

        /* if polling register doesn't check port clock */
        CHK_FUN_RET(retVal, dva_drv_timer_trigger_reg_set(unit, pollingConf));

        /* get start time */
        CHK_FUN_RET(retVal, rtk_timer_get_current(&startTime));
        currentTime = startTime;

        /* polling register */
        CHK_FUN_RET(retVal, dva_drv_timer_polling_reg(unit, pollingConf, startTime, maxUs, &flag));

        /* get end time */
        CHK_FUN_RET(retVal, rtk_timer_get_current(&currentTime));

        /* get time spent */
        timeSpent = currentTime - startTime;

        /* if the minUs is higher than timeSpend or the expVal is the flag */
        if((minUs > (timeSpent)) && (pollingConf->expVal == flag))
        {
            /*log error code*/
            dva_esm_event_log_set(pollingConf->eventID, DVA_EM_EVENT_STATUS_FAILED);
            /* HW failed */
            retVal = -RT_ERR_LOWER_BOUND_FAILED;
        }
        /*check if the expVal is not the flag*/
        else if(pollingConf->expVal != flag)
        {
            /* last time check */
            CHK_FUN_RET(retVal, dva_drv_timer_polling_reg_lastTime(unit, pollingConf));
        }
        else
        {
            /* success */
            retVal = RT_ERR_OK;
        }
    } while(0u == 1u);

    /*return the retVal*/
    return retVal;
}
