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
#include "rtl907xd_va_api.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xd_va_asicdrv_mib.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_api_esm.h"

/**
 * @brief Reset system and debug counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] gReset Global reset (0 to 1) \n
 *            0: Not global reset \n
 *            1: Global reset
 * @param[out] None
 * @retval RT_ERR_OK Reset system and debug counter successfully.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Reset counter failed.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() or dva_drv_indirect_reg_field_get() for the error.
 */
RtkApiRet dva_drv_mib_counter_reset_system_debug(uint32 unit, uint32 gReset)
{
    RtkApiRet retVal                   = RT_ERR_OK;
    uint32    regSysCounterReset       = 0u;
    uint32    regSysDebugCounterReset  = 0u;
    uint32    regPortDebugCounterReset = 0u;
    uint32    startTime                = 0u;
    uint32    endTime                  = 0u;
    uint32    resetDone                = 0u;

    do
    {
        /* reset system counter */
        CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL1, DVA_MIB_COUNTER_CONTROL1_SYSCOUNTERRESET_LSP, DVA_MIB_COUNTER_CONTROL1_SYSCOUNTERRESET_LEN, gReset)));
        /* clear system debug counter */
        CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL1, DVA_MIB_COUNTER_CONTROL1_SYSDEBUGCOUNTERRESET_LSP, DVA_MIB_COUNTER_CONTROL1_SYSDEBUGCOUNTERRESET_LEN, gReset)));
        /* clear per port debug counter */
        CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL1, DVA_MIB_COUNTER_CONTROL1_PORTDEBUGCOUNTERRESET_LSP, DVA_MIB_COUNTER_CONTROL1_PORTDEBUGCOUNTERRESET_LEN, gReset)));
        /* Minimum time to reset */
        CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(MIB_RESET_MIN_US));
        CHK_FUN_CONTINUE(retVal, rtk_timer_get_current(&startTime));
        /* Check if the counters are reset. */
        do
        {
            /* Get MIB counter control registers. */
            CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_get(unit, DVA_MIB_COUNTER_CONTROL1, DVA_MIB_COUNTER_CONTROL1_SYSCOUNTERRESET_LSP, DVA_MIB_COUNTER_CONTROL1_SYSCOUNTERRESET_LEN, &regSysCounterReset)));
            CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_get(unit, DVA_MIB_COUNTER_CONTROL1, DVA_MIB_COUNTER_CONTROL1_PORTDEBUGCOUNTERRESET_LSP, DVA_MIB_COUNTER_CONTROL1_PORTDEBUGCOUNTERRESET_LEN, &regPortDebugCounterReset)));
            CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_get(unit, DVA_MIB_COUNTER_CONTROL1, DVA_MIB_COUNTER_CONTROL1_SYSDEBUGCOUNTERRESET_LSP, DVA_MIB_COUNTER_CONTROL1_SYSDEBUGCOUNTERRESET_LEN, &regSysDebugCounterReset)));

            if((0u == regSysCounterReset) && (0u == regSysDebugCounterReset) && (0u == regPortDebugCounterReset))
            {
                resetDone = 1u;
                break;
            }

            CHK_FUN_CONTINUE(retVal, rtk_timer_get_current(&endTime));
            /* Check if reset timeout occurs */
        } while(MIB_RESET_MAX_US > (endTime - startTime));

        if(0u == resetDone)
        {
            dva_esm_event_log_set(DVA_EM_EVENT_MIB_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            continue;
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Reset port counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMask Port Mask (0 to DVA_RTK_SYS_PMAP_ALL, DVA_RTK_CPU_PMAP) \n
 *            Indicate port mask for resetting port counter.
 * @param[out] None
 * @retval RT_ERR_OK Reset port counter successfully.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Reset counter failed.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() or dva_drv_indirect_reg_field_get() for the error.
 */
RtkApiRet dva_drv_mib_counter_reset_port(uint32 unit, RtkPmap portMask)
{
    RtkApiRet retVal    = RT_ERR_OK;
    RtkPort   port      = 0u;
    uint32    regVal    = 0u;
    uint32    startTime = 0u;
    uint32    endTime   = 0u;
    uint32    resetDone = 0u;

    do
    {
        /* Set MIB counter control registers for ports. */
        for(port = DVA_RTK_SYS_PORT_1ST; port <= DVA_RTK_CPU_PORT; port++)
        {
            if((DVA_RTK_SYS_PORT_END < port) && (DVA_RTK_CPU_PORT > port))
            {
                continue;
            }

            if((portMask & ((RtkPort)1u << port)) == ((RtkPort)1u << port))
            {
                CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, DVA_PORT0_MIB_COUNTER_CONTROL + (port * MIB_PORT_OFFSET), DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERRESET_LSP, DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERRESET_LEN, 1u)));
                /* Minimum time to reset */
                CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(MIB_RESET_MIN_US));
                CHK_FUN_CONTINUE(retVal, rtk_timer_get_current(&startTime));
                /* Check if the counters are reset. */
                do
                {
                    CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_get(unit, DVA_PORT0_MIB_COUNTER_CONTROL + (port * MIB_PORT_OFFSET), DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERRESET_LSP, DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERRESET_LEN, &regVal)));

                    if(0u == regVal)
                    {
                        resetDone = 1u;
                        break;
                    }

                    CHK_FUN_CONTINUE(retVal, rtk_timer_get_current(&endTime));
                    /* Check if reset timeout occurs */
                } while(MIB_RESET_MAX_US > (endTime - startTime));

                if(0u == resetDone)
                {
                    retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
                    break;
                }
                resetDone = 0u;
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Reset overflow counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] gReset Global reset (0 to 1) \n
 *            0: Not global reset \n
 *            1: Global reset
 * @param[in] portMask Port Mask (0 to DVA_RTK_SYS_PMAP_ALL, DVA_RTK_CPU_PMAP) \n
 *            Indicate port mask for resetting port counter.
 * @param[out] None
 * @retval RT_ERR_OK Reset overflow counter successfully.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() for the error.
 */
RtkApiRet dva_drv_mib_counter_reset_overflow(uint32 unit, uint32 gReset, RtkPmap portMask)
{
    RtkApiRet retVal = RT_ERR_OK;
    RtkPort   port   = 0u;

    do
    {
        /*clear overflags */
        if(gReset == 1u)
        {
            CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, DVA_SYSTEM_MIB_COUNTER_OVERFLOW_FLAG, DVA_SYSTEM_MIB_COUNTER_OVERFLOW_FLAG_DOT1DTPLEARNEDENTRYDISCARDSOF_FLG_LSP, DVA_SYSTEM_MIB_COUNTER_OVERFLOW_FLAG_DOT1DTPLEARNEDENTRYDISCARDSOF_FLG_LEN, 1u)));
        }

        /* Set MIB counter overflow flag registers for ports. */
        for(port = DVA_RTK_SYS_PORT_1ST; port <= DVA_RTK_CPU_PORT; port++)
        {
            if((DVA_RTK_SYS_PORT_END < port) && (DVA_RTK_CPU_PORT > port))
            {
                continue;
            }

            if((portMask & ((RtkPort)1u << port)) == ((RtkPort)1u << port))
            {
                CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, (DVA_PORT0_MIBCOUNTER_OVERFLOW_FLAG0 + (port * MIB_PORT_OFFSET)), 0u, 32u, 0xFFFFFFFFu)));
                CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, (DVA_PORT0_MIBCOUNTER_OVERFLOW_FLAG1 + (port * MIB_PORT_OFFSET)), 0u, 32u, 0xFFFFFFFFu)));
                CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, (DVA_PORT0_MIBCOUNTER_OVERFLOW_FLAG2 + (port * MIB_PORT_OFFSET)), 0u, 32u, 0xFFFFFFFFu)));
                CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, (DVA_PORT0_MIB_COUNTER_OVERFLOW_FLAG3 + (port * MIB_PORT_OFFSET)), 0u, 32u, 0xFFFFFFFFu)));
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Reset system, port and overflow.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] gReset Global reset (0 to 1) \n
 *            0: Not global reset \n
 *            1: Global reset
 * @param[in] portMask Port Mask (0 to DVA_RTK_SYS_PMAP_ALL, DVA_RTK_CPU_PMAP) \n
 *            Indicate port mask for resetting port counter.
 * @param[out] None
 * @retval RT_ERR_OK Reset system, port and overflow successfully.
 * @retval -RT_ERR_PORT_MASK The port mask is invalid.
 * @retval Others Refer to dva_drv_mib_counter_reset_system_debug(), dva_drv_mib_counter_reset_port() or
                  dva_drv_mib_counter_reset_overflow() for the error.
 * @note Clear all MIB counter by global resetting and clear counters associated with a particular port by mapped port resetting.
 */
RtkApiRet dva_drv_mib_counter_reset(uint32 unit, uint32 gReset, RtkPmap portMask)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if((portMask & (DVA_RTK_SYS_PMAP_ALL | DVA_RTK_CPU_PMAP)) != portMask)
        {
            retVal = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Reset system counters. */
        CHK_FUN_CONTINUE(retVal, dva_drv_mib_counter_reset_system_debug(unit, gReset));
        /* Reset port counters. */
        CHK_FUN_CONTINUE(retVal, dva_drv_mib_counter_reset_port(unit, portMask));
        /* Reset overflow flag counters. */
        CHK_FUN_CONTINUE(retVal, dva_drv_mib_counter_reset_overflow(unit, gReset, portMask));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get specified MIB counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] counterType Counter Type (N/A) \n
 *            Indicate the counter type for getting counter value.
 * @param[in] port Port Identification (0 to DVA_RTK_SYS_PMAP_ALL, DVA_RTK_CPU_PMAP, 16) \n
 *            16: System MIB counter
 * @param[out] counterValue Counter Value (0 to 4,294,967,295) \n
 *             Indicate the counter value.
 * @retval RT_ERR_OK Get specified MIB counter successfully.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_reg_read() for the error.
 * @note This API gets specified MIB counter.
 */
RtkApiRet dva_drv_mib_counter_get(uint32 unit, RtkMibCounterType_e counterType, RtkPort port, uint32 *counterValue)
{
    RtkApiRet retVal     = RT_ERR_OK;
    uint32    reg        = 0u;
    uint32    portoffset = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(NULL == counterValue)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the counter type to register address of system MIB counter. */
        if((uint32)EM_QMAXSDU_P15Q7_DROP >= (uint32)counterType)
        {
            reg = DVA_SYS_MIBCOUNTER0 + ((uint32)counterType * 0x4u);
        }

        /* Transform the counter type to register address of port MIB counter. */
        if((uint32)EM_QMAXSDU_P15Q7_DROP < (uint32)counterType)
        {
            portoffset = ((uint32)port * (uint32)MIB_PORT_OFFSET);
            reg        = DVA_PORT0_MIBCOUNTER0 + (((uint32)counterType - 1000u) * 0x4u) + portoffset;
        }

        /* Get MIB counter register. */
        retVal = dva_drv_indirect_reg_field_get(unit, reg, 0u, 32u, counterValue);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable MIB counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] enable MIB Enable State (0 to 1) \n
 *            Indicate the global state of MIB. \n
 *            0: Disabled \n
 *            1: Enabled \n
 * @param[out] None
 * @retval RT_ERR_OK Enable or disable MIB counter successfully.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() for the error.
 */
RtkApiRet dva_drv_mib_counter_enable(uint32 unit, RtkEnable enable)
{
    int32  retVal = RT_ERR_OK;
    uint32 value  = 0;

    do
    {
        /* Transform the state to register value. */
        if(DISABLED != enable)
        {
            value = 1u;
        }
        else
        {
            value = 0u;
        }

        /* Set MIB counter control registers. */
        CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL0, DVA_MIB_COUNTER_CONTROL0_ENMIBCOUNTER_LSP, DVA_MIB_COUNTER_CONTROL0_ENMIBCOUNTER_LEN, value)));
        CHK_FUN_CONTINUE(retVal, (dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL0, DVA_MIB_COUNTER_CONTROL0_ENDBMIBCOUNTER_LSP, DVA_MIB_COUNTER_CONTROL0_ENDBMIBCOUNTER_LEN, value)));

    } while(0u == 1u);

    return retVal;
}
