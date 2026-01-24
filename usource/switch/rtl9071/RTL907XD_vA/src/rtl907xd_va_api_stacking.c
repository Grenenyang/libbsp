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

#include "rtl907xd_va_asicdrv_stacking.h"
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_otp.h"
#include "rtl907xd_va_api_esm.h"

uint32 dva_g_stackingMaster = DVA_STACK_DEFAULT_UNIT;

/**
 * @defgroup RTK_MODULE_STACKING Stacking
 * @brief Stacking APIs
 * @{
 */

/**
 * @defgroup STK_DYNAMIC Stacking Dynamic Configure
 * @{
 */

/**
 * @addtogroup STK_DYNAMIC_ASIC Low Level Driver
 * @brief Stacking Dynamic Low Level Driver
 * @{
 */

/**
 * @brief Enable stacking and set port for stacking.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The stacking enable was successful.
 * @retval -RT_ERR_STACK_ALREADY_INIT The device already enable stacking.
 * @retval -RT_ERR_STACK_NOT_INIT The device enable stacking fail.
 * @retval Others Please refer to dva_reg_write, dva_reg_read, dva_drv_stacking_command_polling
 */
RtkApiRet dva_stacking_enable(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;

    /* Check current stacking status */
    CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, DVA_STACK_COMMAND_DUMP_REG));
    CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_COMMAND_REG_STATUS, &tmpVal));

    if((tmpVal & 0x000000FFu) != 0u)
    {
        retVal = -RT_ERR_STACK_ALREADY_INIT;
    }
    else
    {
        /* Enable stacking */
        CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, DVA_STACK_COMMAND_INITIAL));
        CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));

        /* Check stacking is enable successfully */
        CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, DVA_STACK_COMMAND_DUMP_REG));
        CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
        CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_COMMAND_REG_STATUS, &tmpVal));
        if((tmpVal & 0x000000FFu) == DVA_STACK_STS_INIT)
        {
            retVal = RT_ERR_OK;
        }
        else
        {
            retVal = -RT_ERR_STACK_NOT_INIT;
        }
    }

    return retVal;
}

/**
 * @brief Disable stacking.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The stacking disable was successful.
 * @retval Others Please refer to dva_reg_write, dva_drv_stacking_command_polling, dva_stacking_get_status
 */
RtkApiRet dva_stacking_disable(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     status = 0u;

    CHK_FUN_RET(retVal, dva_stacking_get_status(unit, &status));
    if(status != 0u)
    {
        /* disable stacking */
        CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, DVA_STACK_COMMAND_DISABLE));
        CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
    }
    return retVal;
}

/**
 * @brief To search the stackable switch which is connected on port S0 and S1 with this switch. \n
 *      User can set the search at most 3 hops, since the current design supports at most 4 stacking switches. \n
 *      The SDK will return the number of stackable neighbor switches.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] *numOnS0 Number on S0 (0 to 3) \n
 *                 Return the number of stackable switch neighbor on port S0
 * @param[out] *numOnS1 Number on S1 (0 to 3) \n
 *                 Return the number of stackable switch neighbor on port S1
 * @param[out] None
 * @retval RT_ERR_OK The device search process was successful.
 * @retval -RT_ERR_STACK_NOT_INIT The device does not enable stacking.
 * @retval Others Please refer to dva_reg_write, dva_reg_read, dva_drv_stacking_command_polling, rtk_usecond_delay
 */
RtkApiRet dva_stacking_devsearch(uint32 unit, uint32 *numOnS0, uint32 *numOnS1)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    tmpVal  = 0u;
    uint32    timeout = 0u;
    uint32    nS0     = 0u;
    uint32    nS1     = 0u;
    uint32    hop     = DVA_STACK_MAX_HOP;

    do
    {
        /* Check stacking is enable */
        CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, DVA_STACK_COMMAND_DUMP_REG));
        CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
        CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_COMMAND_REG_STATUS, &tmpVal));
        if((tmpVal & 0x000000FFu) == 0u)
        {
            retVal = -RT_ERR_STACK_NOT_INIT;
            continue;
        }

        /* Issue device search */
        CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, ((hop << 16u) | DVA_STACK_COMMAND_DEVSEARCH)));
        CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));

        /* Wait for response */
        while(timeout < dva_g_stackingTimeout)
        {
            timeout++;
            CHK_FUN_BREAK(retVal, rtk_usecond_delay(1));
        }

        /* Get the search result */
        CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, DVA_STACK_COMMAND_DUMP_REG));
        CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
        CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_COMMAND_REG_STATUS, &tmpVal));
        nS0 = ((tmpVal & 0xFF000000u) >> 24u);
        nS1 = ((tmpVal & 0x00FF0000u) >> 16u);
        if(nS0 > DVA_STACK_MAX_HOP)
        {
            nS0 = DVA_STACK_MAX_HOP;
        }
        if(nS1 > DVA_STACK_MAX_HOP)
        {
            nS1 = DVA_STACK_MAX_HOP;
        }
        *numOnS0 = nS0;
        *numOnS1 = nS1;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief After device search, select the number on each stacking port to auto construct stack configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] selOnS0 Select number on S0 (0 to 3) \n
 *                 Select number of switch to join stacking group on port S0
 * @param[in] selOnS1 Select number on S1 (0 to 3) \n
 *                 Select number of switch to join stacking group on port S1
 * @param[out] None
 * @retval RT_ERR_OK The auto config process was successful.
 * @retval -RT_ERR_STACK_CONFIGURE_ERROR The input was wrong.
 * @retval Others Please refer to dva_reg_write, dva_drv_stacking_dump_reg, dva_drv_stacking_command_polling
 */
RtkApiRet dva_stacking_auto_config(uint32 unit, uint32 selOnS0, uint32 selOnS1)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_StackSwitchInfo_t thisSwitch;
    uint32                tmpCmd;

    /* Get the number of stackable neighbors */
    retVal = dva_drv_stacking_dump_reg(unit, 0u, &thisSwitch);

    /* Check the select number range */
    if(((selOnS0 + selOnS1) > 3u) || ((selOnS0 + selOnS1) == 0u))
    {
        retVal = -RT_ERR_STACK_CONFIGURE_ERROR;
    }
    if(selOnS0 > (uint32)thisSwitch.numOnS0)
    {
        retVal = -RT_ERR_STACK_CONFIGURE_ERROR;
    }
    if(selOnS1 > (uint32)thisSwitch.numOnS1)
    {
        retVal = -RT_ERR_STACK_CONFIGURE_ERROR;
    }

    if(retVal == RT_ERR_OK)
    {
        /* Auto config stacking */
        tmpCmd = (((selOnS0) << 24u) + ((selOnS1) << 16u) + DVA_STACK_COMMAND_AUTOCFG);
        CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, tmpCmd));
        CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
    }

    return retVal;
}

/**
 * @brief Dump the current stacking status of this switch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] status Status (0 to 0x4) \n
 *                 pointer for stacking status \n
 *                 0: Disable \n
 *                 1: Standby (initial done) \n
 *                 2: Enable (dynamic construct, Master) \n
 *                 3: Enable (dynamic construct, Slave ) \n
 *                 4: Enable (preset) \n
 *                 Other: Error
 * @retval RT_ERR_OK The status get was successful.
 * @retval -RT_ERR_STACK_OUTPUT_ERROR The output value is wrong
 * @retval Others Please refer to dva_reg_write, dva_reg_read, dva_drv_stacking_command_polling
 */
RtkApiRet dva_stacking_get_status(uint32 unit, uint8 *status)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;

    /* Dump stacking status to register */
    CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, ((tmpVal << 16u) | DVA_STACK_COMMAND_DUMP_REG)));
    CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_3, &tmpVal));

    /* Return stacking status */
    *status = 0;
    if(retVal == RT_ERR_OK)
    {
        *status = (uint8)(tmpVal & 0x000000FFu);
        if(*status > DVA_STACK_STS_ERROR)
        {
            retVal = -RT_ERR_STACK_OUTPUT_ERROR;
        }
    }

    return retVal;
}

/**
 * @brief Dump the current stacking master unit Id.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] masterId Master ID (1 to 7) \n
 *                 stacking master unit ID
 * @retval RT_ERR_OK The master id get was successful.
 * @retval -RT_ERR_STACK_OUTPUT_ERROR The output value is wrong
 * @retval Others Please refer to dva_reg_write, dva_reg_read, dva_drv_stacking_command_polling
 */
RtkApiRet dva_stacking_get_master_id(uint32 unit, uint8 *masterId)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;

    /* Dump stacking status to register */
    *masterId = 0u;
    CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, ((tmpVal << 16u) | DVA_STACK_COMMAND_DUMP_REG)));
    CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_4, &tmpVal));

    /* Return master ID */
    if(retVal == RT_ERR_OK)
    {
        *masterId = (uint8)((tmpVal & 0x000000F0u) >> 4u);
        if((*masterId == 0u) || (*masterId > DVA_STACK_MAX_UNIT_ID))
        {
            retVal = -RT_ERR_STACK_OUTPUT_ERROR;
        }
    }

    return retVal;
}

/**
 * @brief Dump the current stacking group Id.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] groupId Group ID (1 to 0x1FFF) \n
 *                 stacking group ID
 * @retval RT_ERR_OK The group id get was successful.
 * @retval Others Please refer to dva_reg_write, dva_reg_read, dva_drv_stacking_command_polling
 */
RtkApiRet dva_stacking_get_group_id(uint32 unit, uint16 *groupId)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;

    /* Dump stacking status to register */
    *groupId = 0u;
    CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, ((tmpVal << 16u) | DVA_STACK_COMMAND_DUMP_REG)));
    CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_4, &tmpVal));

    /* Return group ID */
    if(retVal == RT_ERR_OK)
    {
        *groupId = (uint16)((tmpVal & 0x1FFF0000u) >> 16u);
    }

    return retVal;
}

/**
 * @brief Dump the current stacking unitmap.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] unitID[4] unit ID (1 to 7 for 4 elements) \n
 *                 a list of stacking member unit ID in an array
 * @param[out] atPort[4] at local or  at port (0 to 2 and 0xF for 4 elements) \n
 *                 a list of stacking unit is at local or connected at port \n
 *                 0x0: The stacking unit does not exist \n
 *                 0x1: The stacking unit is at port S0 \n
 *                 0x2: The stacking unit is at port S1 \n
 *                 0xF: The stacking unit is at local \n
 * @retval RT_ERR_OK The unit map update was successful.
 * @retval -RT_ERR_STACK_OUTPUT_ERROR The output value is wrong
 * @retval Others Please refer to dva_reg_write, dva_reg_read, dva_drv_stacking_command_polling
 */
RtkApiRet dva_stacking_get_unitmap(uint32 unit, uint8 *unitID, uint8 *atPort)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;
    uint32    i      = 0u;

    /* Initial value */
    for(i = 0u; i < DVA_STACK_MAX_UNIT; i++)
    {
        unitID[i] = 0u;
        atPort[i] = 0u;
    }

    /* Dump stacking status to register */
    CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, ((tmpVal << 16u) | DVA_STACK_COMMAND_DUMP_REG)));
    CHK_FUN_RET(retVal, dva_drv_stacking_command_polling(unit));
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_5, &tmpVal));

    /* Return unit map */
    if(retVal == RT_ERR_OK)
    {
        unitID[0] = (uint8)(tmpVal & 0x0000000Fu);
        atPort[0] = (uint8)((tmpVal & 0x000000F0u) >> 4u);
        unitID[1] = (uint8)((tmpVal & 0x00000F00u) >> 8u);
        atPort[1] = (uint8)((tmpVal & 0x0000F000u) >> 12u);
        unitID[2] = (uint8)((tmpVal & 0x000F0000u) >> 16u);
        atPort[2] = (uint8)((tmpVal & 0x00F00000u) >> 20u);
        unitID[3] = (uint8)((tmpVal & 0x0F000000u) >> 24u);
        atPort[3] = (uint8)((tmpVal & 0xF0000000u) >> 28u);

        /* Check error value */
        for(i = 0u; i < DVA_STACK_MAX_UNIT; i++)
        {
            if(((atPort[i] > 0x2u) && (atPort[i] < 0xFFu)) && ((unitID[i] == 0u) || (unitID[i] > DVA_STACK_MAX_UNIT_ID)))
            {
                retVal = -RT_ERR_STACK_OUTPUT_ERROR;
                break;
            }
        }
    }

    return retVal;
}

/**
 * @brief For master, check current stacking link status to all slaves.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] unitID[4] unit ID (1 to 7 for 4 elements) \n
 *                 a list of stacking member unit ID in an array
 * @param[out] linkStatus[4] Link status (0 to 3 for 4 elements) \n
 *                 The link status of each stacking member \n
 *                 0: Not member \n
 *                 1: Normal \n
 *                 2: Disconnect \n
 *                 3: Local port
 * @retval RT_ERR_OK The connection check was successful.
 * @retval -RT_ERR_STACK_OUTPUT_ERROR The output value is wrong
 * @retval -RT_ERR_STACK_NOT_INIT The device does not enable stacking.
 * @retval -RT_ERR_STACK_LINK_FAIL Check linkstatus is failed
 * @retval Others Please refer to dva_reg_write, dva_reg_read, dva_drv_stacking_check_connection_status
 */
RtkApiRet dva_stacking_check_connection(uint32 unit, uint8 *unitID, uint8 *linkStatus)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    tmpVal = 0u;
    uint32    i      = 0u;
    uint8     atPort[DVA_STACK_MAX_UNIT];
    uint32    testValue = 0u;
    uint32    readValue = 0u;

    /* Initial value */
    for(i = 0u; i < DVA_STACK_MAX_UNIT; i++)
    {
        linkStatus[i] = 0u;
        unitID[i]     = 0u;
    }

    /* Check stacking status to register */
    CHK_FUN_RET(retVal, dva_drv_stacking_check_connection_status(unit));

    /* Get unit map */
    CHK_FUN_RET(retVal, dva_reg_read(unit, DVA_STACK_STATUS_REG_5, &tmpVal));

    if(retVal == RT_ERR_OK)
    {
        unitID[0] = (uint8)(tmpVal & 0x0000000Fu);
        atPort[0] = (uint8)((tmpVal & 0x000000F0u) >> 4u);
        unitID[1] = (uint8)((tmpVal & 0x00000F00u) >> 8u);
        atPort[1] = (uint8)((tmpVal & 0x0000F000u) >> 12u);
        unitID[2] = (uint8)((tmpVal & 0x000F0000u) >> 16u);
        atPort[2] = (uint8)((tmpVal & 0x00F00000u) >> 20u);
        unitID[3] = (uint8)((tmpVal & 0x0F000000u) >> 24u);
        atPort[3] = (uint8)((tmpVal & 0xF0000000u) >> 28u);

        for(i = 0u; i < DVA_STACK_MAX_UNIT; i++)
        {
            if(retVal != RT_ERR_OK)
            {
                break;
            }
            if(unitID[i] > DVA_STACK_MAX_UNIT_ID)
            {
                retVal = -RT_ERR_STACK_OUTPUT_ERROR;
            }
            else if(atPort[i] == 0x0u)
            {
                /* Not member, neglect */
                linkStatus[i] = 0u;
            }
            else if(atPort[i] == 0xFu)
            {
                /* Self port, neglect */
                linkStatus[i] = 3u;
            }
            else
            {
                /* Test stacking slave by read/write register */
                readValue = 0;
                testValue = 0xDB5A5A00u + i;
                CHK_FUN_CONTINUE(retVal, dva_reg_write(unitID[i], DVA_INTERNAL_CPU_VARIABLE45, testValue));
                CHK_FUN_CONTINUE(retVal, dva_reg_read(unitID[i], DVA_INTERNAL_CPU_VARIABLE45, &readValue));

                if(readValue == testValue)
                {
                    linkStatus[i] = 1u;
                }
                else
                {
                    linkStatus[i] = 2u;
                    retVal        = -RT_ERR_STACK_LINK_FAIL;
                }
            }
        }
    }

    return retVal;
}

/**@}*/ /* STK_DYNAMIC_ASIC */
/**@}*/ /* STK_DYNAMIC */

/**
 * @defgroup STK_STATIC Stacking Static Bootup
 * @{
 */

/**
 * @addtogroup STK_STATIC_ASIC Low Level Driver
 * @brief Stacking Static Low Level Driver
 * @{
 */

/**
 * @brief Set OTP for stacking boot
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] role Role (0 to 1) \n
 *                 Stacking role: Master(0) or slave(1)
 * @param[in] portS0Enable Port S0 enable (0 to 1) \n
 *                 Port S0 for stacking boot
 * @param[in] portS1Enable Port S1 enable (0 to 1) \n
 *                 Port S1 for stacking boot
 * @param[out] None
 * @retval RT_ERR_OK The OTP set was successful.
 * @retval Others Please refer to dva_otp_read, dva_otp_write
 */
RtkApiRet dva_stacking_boot_otp_set(uint32 unit, uint32 role, uint32 portS0Enable, uint32 portS1Enable)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     otpValue    = 0u;
    uint8     otpEnaStack = 0u;

    /* Read stacking OTP value */
    retVal = dva_otp_read(unit, 0xC4u, &otpValue, 1u);
    CHK_FUN_RET(retVal, dva_otp_read(unit, 0x1103u, &otpEnaStack, 1u));

    if(retVal == RT_ERR_OK)
    {
        if(role == STACK_BOOT_SLAVE)
        {
            otpValue &= 0xBFu;
        }
        if(portS0Enable != 0u)
        {
            otpValue &= 0x7Bu;
        }
        if(portS1Enable != 0u)
        {
            otpValue &= 0x77u;
        }
        otpEnaStack &= 0xF8u;
    }

    /* Program stacking OTP value */
    CHK_FUN_RET(retVal, dva_otp_write(unit, 0xC4u, &otpValue, 1u));
    CHK_FUN_RET(retVal, dva_otp_write(unit, 0x1103u, &otpEnaStack, 1u));

    return retVal;
}

/**
 * @brief Get OTP setting for stacking boot.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] role Role (0 to 1) \n
 *                 Stacking role: Master(0) or slave(1)
 * @param[out] portS0Enable Port S0 enable (0 to 1) \n
 *                 Port S0 for stacking boot
 * @param[out] portS1Enable Port S1 enable (0 to 1) \n
 *                 Port S1 for stacking boot
 * @retval RT_ERR_OK The OTP get was successful.
 * @retval Others Please refer to dva_otp_read
 */
RtkApiRet dva_stacking_boot_otp_get(uint32 unit, uint32 *role, uint32 *portS0Enable, uint32 *portS1Enable)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint8     otpValue = 0u;

    /* Read stacking OTP value */
    retVal = dva_otp_read(unit, 0xC4, &otpValue, 1u);

    if(retVal == RT_ERR_OK)
    {
        if((otpValue & 0x40u) == 0x0u)
        {
            *role = 1u;
        }
        else
        {
            *role = 0u;
        }
        if((otpValue & 0x04u) == 0x0u)
        {
            *portS0Enable = 1u;
        }
        else
        {
            *portS0Enable = 0u;
        }
        if((otpValue & 0x08u) == 0x0u)
        {
            *portS1Enable = 1u;
        }
        else
        {
            *portS1Enable = 0u;
        }
    }

    return retVal;
}

/**@}*/ /* STK_STATIC_ASIC */
/**@}*/ /* STK_STATIC */

/**
 * @defgroup STK_FLASHLESS Stacking Flashless
 * @{
 */

/**
 * @addtogroup STK_FLASHLESS_ASIC Low Level Driver
 * @brief Stacking Flashless Low Level Driver
 * @{
 */

/**
 * @brief Flashless update flow of stacking slave.
 * @param[in] slaveUnit Slave unit (1 to 7) \n
 *                 Slave stacking unit going to update
 * @param[in] fwFileName Firmware file name (1 to 255 characters) \n
 *                 File name of firmware field
 * @param[in] cfgFileName Configuration file name (1 to 255 characters) \n
 *                 File name of configuration field
 * @retval RT_ERR_OK The stacking flashless update was successful.
 * @retval -RT_ERR_NULL_POINTER The input was null pointer.
 * @retval -RT_ERR_FAILED The file process failed.
 * @retval Others Please refer to dva_drv_stacking_cfg_data_get, rtl_file_open, rtl_file_close, dva_drv_stacking_load_fw, dva_drv_stacking_load_cfg, dva_reg_field_read, dva_drv_stacking_parse_flash_slave_cfg_header, dva_reg_write
 */
RtkApiRet dva_stacking_flashless_update_slave(uint32 slaveUnit, const int8 *fwFileName, const int8 *cfgFileName)
{
    RtkApiRet retVal                     = RT_ERR_OK;
    uint32    stkReg[DVA_STACK_CFG_SIZE] = {(uint32)0u};
    uint32    spiStartAddr               = 0u;
    uint32    spiParaLen                 = 0u;
    void     *pFwFile                    = NULL;
    uint32    slaveLoc                   = 0u;
    uint32    unit                       = DVA_UNIT;
    uint32    data                       = 0u;
    uint32    retryTime                  = 0u;

    do
    {
        /* Check NULL pointer */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_flashless_update_slave_check(fwFileName, cfgFileName));
        /* Check DUT is ready to start load firmware and configuration to slave */
        CHK_FUN_CONTINUE(retVal, dva_drv_stacking_check_alert());
        do
        {
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_CPU_SPECIAL0, 0u, 8u, &data));
            retryTime++;
        } while((DVA_FWSTS_MAIN_STACKBOOT_FLASHLESS_MASTER != data) && (DVA_TIMEOUT_VALUE > retryTime) && (retVal == RT_ERR_OK));

        if(DVA_TIMEOUT_VALUE > retryTime)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_stacking_cfg_data_get(cfgFileName, stkReg, slaveUnit, &slaveLoc));
            CHK_FUN_CONTINUE(retVal, dva_drv_stacking_parse_flash_slave_cfg_header(fwFileName, &spiStartAddr, &spiParaLen, (uint8)(slaveLoc & 0xFFu)));

            /* Open files */
            retVal = rtl_file_open(&pFwFile, 0u, fwFileName);
            if(pFwFile == NULL)
            {
#ifdef DEBUG_ASIC_CONFIG
                rtlglue_printf("FILE %s open fail!\n", fwFileName);
#endif
                retVal = -RT_ERR_FAILED;
                continue;
            }
            /* Load slave firmware and configuration */
            CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_fw(pFwFile, stkReg, slaveUnit, slaveLoc));
            CHK_FUN_CONTINUE(retVal, dva_drv_stacking_load_cfg(pFwFile, stkReg, slaveUnit, spiStartAddr, spiParaLen, slaveLoc));

            /* Set stacking command to finish update. */
            CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, DVA_STACK_COMMAND_REGISTER, DVA_STACK_COMMAND_EXIT_BOOT_FLOW));
        }
        else
        {
            retVal = -RT_ERR_BUSYWAIT_TIMEOUT;
            dva_esm_event_log_set(DVA_EM_EVENT_STACKING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        }
    } while(0u == 1u);
    rtl_file_close(&pFwFile);

    return retVal;
}

/**@}*/ /* STK_FLASHLESS_ASIC */
/**@}*/ /* STK_FLASHLESS */
/**@}*/ /* RTK_MODULE_STACKING */
