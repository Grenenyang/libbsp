/*
 * Copyright (c) 2024 Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * This software component is confidential and proprietary to Realtek
 * Semiconductor Corp. Disclosure, reproduction, redistribution, in whole
 * or in part, of this work and its derivatives without express permission
 * is prohibited.
 */

#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtl907x_d/rtl907xd_va_rtkas_reg_list.h"
#include "rtl907x_d/rtl907xd_va_rtkas_regField_list.h"
#include "rtl907xd_va_asicdrv_eee.h"
#include "rtl907xd_va_asicdrv_phy.h"
#include "rtkas_api_eee.h"
#include "rtl907xd_va_api_ext.h"

#define CHAR_MASK (0xFFu)

/**
 * @addtogroup RTK_MODULE_EEE EEE
 * @{
 */

/**
 * @addtogroup EEE_LPI_ASSERT LPI Assert Functions
 * @{
 */

/**
 * @defgroup EEE_LPI_ASSERT_ASIC Low Level Driver
 * @brief EEE Low Level Driver
 * @{
 */

/**
 * @brief Set configuration of disabling all port EEE function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] cfg  The parameters to disable the EEE function of all port. (DISABLE to ENABLE) \n
 *                 This bit is higher priority than port TX or RX EEE ability. \n
 *                 If cfg is set to 0, then EEE function disabled for all port. \n
 *                 If cfg is set to 1, then see portN_EN_TX_EEE or portN_EN_RX_EEE.
 * @param[out] None
 *
 * @retval RT_ERR_OK       Successfully set the configure bit of disabling all port EEE function.
 * @retval -RT_ERR_INPUT   The input cfg is invalid.
 * @retval Others          Please refer to dva_reg_field_write().
 */
RtkApiRet dva_eee_set_en(uint32 unit, uint32 cfg)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        /* check input cfg and assign the reg value to configure the disable all port configuration */
        if(ENABLE == cfg)
        {
            regVal = EEE_ENABLE_ALL_PORT;
        }
        else if(DISABLE == cfg)
        {
            regVal = EEE_DISABLE_ALL_PORT;
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* set disable all port EEE configuration */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_GLOBAL_EEE_MAC_CONTROL0,
                                                     DVA_GLOBAL_EEE_MAC_CONTROL0_DIS_EEE_ALLPORT_LSP,
                                                     DVA_GLOBAL_EEE_MAC_CONTROL0_DIS_EEE_ALLPORT_LEN,
                                                     regVal));
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get configuration of disabling all port EEE function.
 * @param[in] unit   Stacking unit (1 to 7) \n
 *                   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] *pCfg The configuration of control bit of disabling the EEE function of all port. (DISABLE to ENABLE) \n
 *                   - DISABLE: Disable EEE function for all ports. \n
 *                   - ENABLE: Enable EEE function for all ports.
 * @param[out] None
 *
 * @retval RT_ERR_OK            Successfully get the configure bit of disabling all port EEE function.
 * @retval -RT_ERR_NULL_POINTER The input *pCfg is NULL pointer.
 * @retval Others               Please refer to dva_reg_field_read().
 */
RtkApiRet dva_eee_get_en(uint32 unit, uint32 *pCfg)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        /* check NULL pointer parameters */
        if(NULL == pCfg)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get disable all port EEE configuration */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_GLOBAL_EEE_MAC_CONTROL0,
                                                    DVA_GLOBAL_EEE_MAC_CONTROL0_DIS_EEE_ALLPORT_LSP,
                                                    DVA_GLOBAL_EEE_MAC_CONTROL0_DIS_EEE_ALLPORT_LEN,
                                                    &regVal));
        if(EEE_ENABLE_ALL_PORT == regVal)
        {
            *pCfg = ENABLE;
        }
        else
        {
            *pCfg = DISABLE;
        }
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Set EEE function configuration of the specified port.
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                      The user port index of the selected port.
 * @param[in] enPortEee Enable Port EEE Function (0 to 1) \n
 *                      Enable EEE function for RX and TX. \n
 *                      - 1: Enable EEE function of the addressed port. \n
 *                      - 0: Disable EEE function of the addressed port.
 * @param[out] None
 *
 * @retval RT_ERR_OK       Successfully set the EEE function enable configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID The input port index is out of range.
 * @retval -RT_ERR_INPUT   The input parameter is invalid.
 * @retval Others          Please refer to dva_drv_indirect_reg_field_set().
 */
RtkApiRet dva_eee_set_port_en(uint32 unit, RtkPort port, RtkEnable enPortEee)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = dva_g_userPort2sysPort[port];

    do
    {
        /* check input enable config */
        if((ENABLE != enPortEee) && (DISABLE != enPortEee))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Enable the T1-PHY EEE function if the port interface type is 1000BASE-T1 */
        if(DVA_OP_T1_USER_PORT_MAX >= port)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_eee_set_phy_en(unit, port, enPortEee));
        }

        /* set EEE function for port EN_RX_EEE by indirect reg setting API */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EN_RX_EEE_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EN_RX_EEE_LEN,
                                                                enPortEee));

        /* set EEE function for port EN_TX_EEE by indirect reg setting API */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EN_TX_EEE_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EN_TX_EEE_LEN,
                                                                enPortEee));

        /*
         * set TX EN and RX EN configuration by indirect reg setting API
         * if either TX or RX is enabled, enable EN_EEE_100 of this port.
         * if both TX or RX is enabled, enable EN_EEE_100 of this port.
         * if both TX and RX are disabled, disable EN_EEE_100.
         */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_PROPERTY_CONFIGURE + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_PROPERTY_CONFIGURE_EN_EEE_100_LSP,
                                                                DVA_PORT0_PROPERTY_CONFIGURE_EN_EEE_100_LEN,
                                                                enPortEee));
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get EEE function configuration of the designated port.
 * @param[in] unit         Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port         Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                         The user port index of the selected port.
 * @param[out] *pEnPortEee Port EEE Enable Config (0 to 1) \n
 *                         - 1: Enable EEE function for RX and TX. \n
 *                         - 0: Disable EEE function for RX and TX.
 *
 * @retval RT_ERR_OK            Successfully get the EEE function enable configuration of the addressed port.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval -RT_ERR_PORT_ID      The input port index is out of range.
 * @retval Others          Please refer to dva_drv_indirect_reg_field_get().
 */
RtkApiRet dva_eee_get_port_en(uint32 unit, RtkPort port, RtkEnable *pEnPortEee)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = dva_g_userPort2sysPort[port];
    uint32    regVal  = 0u;

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            *pEnPortEee = DISABLE;
            retVal      = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL pointer parameters */
        if(NULL == pEnPortEee)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* get EEE function for port EN_RX_EEE and EN_TX_EEE by indirect reg getting API */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EN_TX_EEE_LSP,
                                                                (DVA_PORT0_EEE_MAC_CONTROL_EN_TX_EEE_LEN + DVA_PORT0_EEE_MAC_CONTROL_EN_RX_EEE_LEN),
                                                                &regVal));
        /* check if EN_TX_EEE(bit21) and EX_TRX_EEE(bit22) are both enabled */
        if(0x3u == (0x3u & regVal))
        {
            *pEnPortEee = ENABLE;
        }
        else
        {
            *pEnPortEee = DISABLE;
        }
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Set the timer unit of the designated timer type.
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] type      Timer type (N/A) \n
 *                      The addressed timer type to configure the unit. Please refer to the struct field introductions in the .h files for detailed content.
 * @param[in] timerUnit Timer unit (N/A) \n
 *                      The specified timer unit. Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK       Successfully set the timer unit of the addressed timer type.
 * @retval -RT_ERR_INPUT   The input timer unit or timer type is invalid.
 * @retval Others          Please refer to dva_reg_field_write().
 */
RtkApiRet dva_eee_set_timer_unit(uint32 unit, EeeTimerType_e type, EeeTimerUnit_e timerUnit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = (uint32)timerUnit;

    do
    {
        /* check the input timer unit value */
        if(EM_EEE_TIMER_UNIT_END <= timerUnit)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* set the timer unit of the addressed timer type */
        switch(type)
        {
            /* set timer unit of type RGMII 100M */
            case EM_EEE_TIMER_100M:
                retVal = dva_reg_field_write(unit, DVA_GLOBAL_EEE_MAC_CONTROL2,
                                             DVA_GLOBAL_EEE_MAC_CONTROL2_EEE_TIMER_UNIT_LSP,
                                             DVA_GLOBAL_EEE_MAC_CONTROL2_EEE_TIMER_UNIT_LEN,
                                             regVal);
                break;
            /* set timer unit of type RGMII 1000M */
            case EM_EEE_TIMER_1000M:
                retVal = dva_reg_field_write(unit, DVA_GLOBAL_EEE_MAC_CONTROL4,
                                             DVA_GLOBAL_EEE_MAC_CONTROL4_EEE_TIMER_UNIT_GIGA_LSP,
                                             DVA_GLOBAL_EEE_MAC_CONTROL4_EEE_TIMER_UNIT_GIGA_LEN,
                                             regVal);
                break;
            /* set timer unit of type RGMII 10M */
            case EM_EEE_TIMER_10M:
                retVal = dva_reg_field_write(unit, DVA_GLOBAL_EEE_MAC_CONTROL6,
                                             DVA_GLOBAL_EEE_MAC_CONTROL6_EEE_TIMER_UNIT_10M_LSP,
                                             DVA_GLOBAL_EEE_MAC_CONTROL6_EEE_TIMER_UNIT_10M_LEN,
                                             regVal);
                break;
            /* unsupported type, return error */
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get the timer unit of the designated timer type.
 * @param[in] unit         Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] type         Timer type (N/A) \n
 *                         The addressed timer type to configure the unit. Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] *pTimerUnit Timer unit (N/A) \n
 *                         The specified timer unit. Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK       Successfully get the timer unit of the addressed timer type.
 * @retval -RT_ERR_INPUT   The input timer unit or timer type is invalid.
 * @retval Others          Please refer to dva_reg_field_read().
 */
RtkApiRet dva_eee_get_timer_unit(uint32 unit, EeeTimerType_e type, EeeTimerUnit_e *pTimerUnit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        /* check NULL pointer parameters */
        if(NULL == pTimerUnit)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get the timer unit of the addressed timer type */
        switch(type)
        {
            /* get timer unit of type RGMII 100M */
            case EM_EEE_TIMER_100M:
                retVal = dva_reg_field_read(unit, DVA_GLOBAL_EEE_MAC_CONTROL2,
                                            DVA_GLOBAL_EEE_MAC_CONTROL2_EEE_TIMER_UNIT_LSP,
                                            DVA_GLOBAL_EEE_MAC_CONTROL2_EEE_TIMER_UNIT_LEN,
                                            &regVal);
                break;
            /* get timer unit of type RGMII 1000M */
            case EM_EEE_TIMER_1000M:
                retVal = dva_reg_field_read(unit, DVA_GLOBAL_EEE_MAC_CONTROL4,
                                            DVA_GLOBAL_EEE_MAC_CONTROL4_EEE_TIMER_UNIT_GIGA_LSP,
                                            DVA_GLOBAL_EEE_MAC_CONTROL4_EEE_TIMER_UNIT_GIGA_LEN,
                                            &regVal);
                break;
            /* get timer unit of type RGMII 10M */
            case EM_EEE_TIMER_10M:
                retVal = dva_reg_field_read(unit, DVA_GLOBAL_EEE_MAC_CONTROL6,
                                            DVA_GLOBAL_EEE_MAC_CONTROL6_EEE_TIMER_UNIT_10M_LSP,
                                            DVA_GLOBAL_EEE_MAC_CONTROL6_EEE_TIMER_UNIT_10M_LEN,
                                            &regVal);
                break;
            /* unsupported type, return error */
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
        /* return timer unit configuration if the get function return no error */
        if(RT_ERR_OK == retVal)
        {
            switch(regVal)
            {
                case 0u:
                    *pTimerUnit = EM_EEE_TIMER_UNIT_1US;
                    break;
                case 1u:
                    *pTimerUnit = EM_EEE_TIMER_UNIT_16US;
                    break;
                case 2u:
                    *pTimerUnit = EM_EEE_TIMER_UNIT_128US;
                    break;
                case 3u:
                    *pTimerUnit = EM_EEE_TIMER_UNIT_1024US;
                    break;
                default:
                    *pTimerUnit = EM_EEE_TIMER_UNIT_END;
                    break;
            }
        }
        else
        {
            *pTimerUnit = EM_EEE_TIMER_UNIT_END;
        }
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Set the timer value of the designated timer type.
 * @param[in] unit             Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] type             Timer type (N/A) \n
 *                             The addressed timer type to configure the unit. Please refer to the struct field introductions in the .h files for detailed content.
 * @param[in] eeeTimerValueSet Timer value (N/A) \n
 *                             The specified timer value. Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 *
 * @retval RT_ERR_OK       Successfully set the timer value of the addressed timer type.
 * @retval -RT_ERR_INPUT   The input timer value or timer type is invalid.
 * @retval Others          Please refer to dva_reg_write().
 */
RtkApiRet dva_eee_set_timer_value(uint32 unit, EeeTimerType_e type, EeeTimerValue_t eeeTimerValueSet)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        /* construct bit[7:0] of T_pause setting value */
        regVal = ((uint32)eeeTimerValueSet.tp << DVA_EEE_TIMER_TP_CFG_BIT_LSP);
        /* construct bit[15:8] of T_tx_delay setting value */
        regVal |= ((uint32)eeeTimerValueSet.td << DVA_EEE_TIMER_TD_CFG_BIT_LSP);
        /* construct bit[23:16] of T_tx_rate setting value */
        regVal |= ((uint32)eeeTimerValueSet.tr << DVA_EEE_TIMER_TR_CFG_BIT_LSP);
        /* construct bit[31:24] of T_wakeup setting value */
        regVal |= ((uint32)eeeTimerValueSet.tw << DVA_EEE_TIMER_TW_CFG_BIT_LSP);
        /* set the timer value of the addressed timer type */
        switch(type)
        {
            /* set timer value setting of type RGMII 100M */
            case EM_EEE_TIMER_100M:
                retVal = dva_reg_write(unit, DVA_GLOBAL_EEE_MAC_CONTROL1, regVal);
                break;
            /* set timer value setting of type RGMII 1000M */
            case EM_EEE_TIMER_1000M:
                retVal = dva_reg_write(unit, DVA_GLOBAL_EEE_MAC_CONTROL3, regVal);
                break;
            /* set timer value setting of type RGMII 10M */
            case EM_EEE_TIMER_10M:
                retVal = dva_reg_write(unit, DVA_GLOBAL_EEE_MAC_CONTROL5, regVal);
                break;
            /* unsupported type, return error */
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get the timer value of the designated timer type.
 * @param[in] unit                Stacking unit (1 to 7) \n
 *                                Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] type                Timer type (N/A) \n
 *                                The addressed timer type to configure the unit. Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] *pEeeTimerValueGet Timer value (N/A) \n
 *                                The specified timer value. Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK              Successfully get the timer value of the addressed timer type.
 * @retval -RT_ERR_NULL_POINTER   The input pEeeTimerValueGet is NULL.
 * @retval Others          Please refer to dva_reg_read().
 */
RtkApiRet dva_eee_get_timer_value(uint32 unit, EeeTimerType_e type, EeeTimerValue_t *pEeeTimerValueGet)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;
    uint32    tp     = 0u;
    uint32    td     = 0u;
    uint32    tr     = 0u;
    uint32    tw     = 0u;

    do
    {
        /* check NULL pointer parameters */
        if(NULL == pEeeTimerValueGet)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get the timer value of the addressed timer type */
        switch(type)
        {
            /* get timer value setting of type RGMII 100M */
            case EM_EEE_TIMER_100M:
                retVal = dva_reg_read(unit, DVA_GLOBAL_EEE_MAC_CONTROL1, &regVal);
                break;
            /* get timer value setting of type RGMII 1000M */
            case EM_EEE_TIMER_1000M:
                retVal = dva_reg_read(unit, DVA_GLOBAL_EEE_MAC_CONTROL3, &regVal);
                break;
            /* get timer value setting of type RGMII 10M */
            case EM_EEE_TIMER_10M:
                retVal = dva_reg_read(unit, DVA_GLOBAL_EEE_MAC_CONTROL5, &regVal);
                break;
            /* unsupported type, return error */
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(1u == 0u);

    /* if the timer value setting is read successfully */
    if(RT_ERR_OK == retVal)
    {
        /* get bit[7:0] of setting value: EEE_Timer_Tp */
        tp                    = (regVal >> DVA_EEE_TIMER_TP_CFG_BIT_LSP) & CHAR_MASK;
        pEeeTimerValueGet->tp = (uint8)tp;
        /* get bit[15:8] of setting value: EEE_Timer_Td */
        td                    = (regVal >> DVA_EEE_TIMER_TD_CFG_BIT_LSP) & CHAR_MASK;
        pEeeTimerValueGet->td = (uint8)td;
        /* get bit[23:16] of setting value: EEE_Timer_Tr */
        tr                    = (regVal >> DVA_EEE_TIMER_TR_CFG_BIT_LSP) & CHAR_MASK;
        pEeeTimerValueGet->tr = (uint8)tr;
        /* get bit[31:24] of setting value: EEE_Timer_Tw */
        tw                    = (regVal >> DVA_EEE_TIMER_TW_CFG_BIT_LSP) & CHAR_MASK;
        pEeeTimerValueGet->tw = (uint8)tw;
    }

    return retVal;
}
/**@}*/ /* EEE_LPI_ASSERT_ASIC */
/**@}*/ /* EEE_LPI_ASSERT */

/**
 * @addtogroup EEE_LPI_DETECT LPI Detect Function
 * @{
 */

/**
 * @defgroup EEE_LPI_DETECT_ASIC Low Level Driver
 * @brief EEE Low Level Driver
 * @{
 */

/**
 * @brief Get the T1-PHY EEE RX and TX LPI indication of the designated port.
 * @param[in] unit            Stacking unit (1 to 7) \n
 *                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port            Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_6ST) \n
 *                            The user port index of the selected port.
 * @param[out] *pEeePhyStatus To get the T1-PHY EEE RX and TX LPI indication of the addressed port. (N/A) \n
 *                            Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK            Successfully enable or disable T1-PHY EEE configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID      The input port index is out of range.
 * @retval -RT_ERR_NULL_POINTER The input pEnPhyEee is NULL.
 * @retval Others               Please refer to dva_reg_field_read().
 * @note   Only port1 to port6 need to configure the corresponding T1-PHY EEE functions.
 */
RtkApiRet dva_eee_get_phy_func_sts(uint32 unit, RtkPort port, EeePhyStatus_t *pEeePhyStatus)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regOffset = 0u;
    uint32    regVal    = 0u;

    do
    {
        /* The RTPGE port index is 1 to 6 */
        if((DVA_OP_T1_USER_PORT_MIN > port) || (DVA_OP_T1_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL pointer parameters */
        if(NULL == pEeePhyStatus)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* get the T1-PHY OCP address of the addressed port index */
        regOffset = DVA_PAGE_SWR_OCP | ((dva_g_sysPortToOcpAddr[dva_g_userPort2sysPort[port]] | DVA_OCP_PHYPCS_01A2) << 1u);
        /* get T1-PHY TX LPI Indication */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regOffset,
                                                    DVA_OCP_MMD_PCS_TX_LPI_INDICATION_LSP,
                                                    DVA_OCP_MMD_PCS_TX_LPI_INDICATION_LEN,
                                                    &regVal));
        pEeePhyStatus->phyTxSts = (uint8)(regVal & (uint8)CHAR_MASK);

        /* get T1-PHY RX LPI Indication */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regOffset,
                                                    DVA_OCP_MMD_PCS_RX_LPI_INDICATION_LSP,
                                                    DVA_OCP_MMD_PCS_RX_LPI_INDICATION_LEN,
                                                    &regVal));
        pEeePhyStatus->phyRxSts = (uint8)(regVal & (uint8)CHAR_MASK);
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get EEE function status of the addressed port.
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port          Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                          The user port index of the selected port.
 * @param[out] *pEeeStatus  EEE status (N/A) \n
 *                          Struct contains rxStatus, txStatus and status. \n
 *                          Please refer to the struct field introductions in the .h files for detailed content.
 *
 * @retval RT_ERR_OK            Successfully get the EEE function status of the addressed port.
 * @retval -RT_ERR_PORT_ID      The input port index is out of range.
 * @retval -RT_ERR_NULL_POINTER The input parameter is invalid.
 * @retval Others               Please refer to dva_drv_indirect_reg_field_get().
 */
RtkApiRet dva_eee_get_port_func_sts(uint32 unit, RtkPort port, EeePortStatus_t *pEeeStatus)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    RtkPort   sysPort = dva_g_userPort2sysPort[port];

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL pointer parameters */
        if(NULL == pEeeStatus)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get per port EEE status(both Tx and Rx), EEE TX and EEE RX by indirect reg getting API */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_STS_LSP,
                                                                (DVA_PORT0_EEE_MAC_CONTROL_EEE_STS_LEN + DVA_PORT0_EEE_MAC_CONTROL_EEE_TX_STS_LEN + DVA_PORT0_EEE_MAC_CONTROL_EEE_RX_STS_LEN),
                                                                &regVal));
        /* get per port EEE status (both TX and RX) */
        pEeeStatus->status = (uint8)(regVal & 0x1u);
        /* get per port EEE TX status */
        pEeeStatus->txStatus = (uint8)((regVal >> 1u) & 0x1u);
        /* get per port EEE RX status */
        pEeeStatus->rxStatus = (uint8)((regVal >> 2u) & 0x1u);
    } while(1u == 0u);

    return retVal;
}
/**@}*/ /* EEE_LPI_DETECT_ASIC */
/**@}*/ /* EEE_LPI_DETECT */

/**
 * @addtogroup EEE_TX_REQUEST EEE TX Request Decision Maker Configurations
 * @{
 */

/**
 * @defgroup EEE_TX_REQUEST_ASIC Low Level Driver
 * @brief EEE Low Level Driver
 * @{
 */
/**
 * @brief Set the LPI TX request decision maker configuration of the addressed port.
 * @param[in] unit              Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port              Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                              The user port index of the selected port.
 * @param[in] txRate            Enable TX Rate LPI request Decision Maker (0 to 1) \n
 *                              The switch statistically evaluates the TX traffic over a period of time, and when it falls below the threshold value, \n
 *                              it will waits until TXEN is low before sending an LPI request. \n
 *                              Please refer to dva_eee_set_port_lpi_qtx_thr() about TX rate threshold configuration.
 * @param[in] queueEmpty        Enable TX Queue Empty Decision Maker of LPI request (0 to 1) \n
 *                              EEE TX enters the LPI state when the TX queue is empty.
 * @param[in] rxPauseOnFrame    Enable RX Pause on Frame Decision Maker of LPI request (0 to 1) \n
 *                              EEE TX enters the LPI state when receiving a PAUSE Frame and the signal TXEN is low.
 * @param[out] None
 *
 * @retval RT_ERR_OK            Successfully set the LPI request decision maker configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID      The input port index is out of range.
 * @retval -RT_ERR_INPUT        The input parameter is invalid.
 * @retval Others               Please refer to dva_drv_indirect_reg_field_set().
 * @note  If the LPI TX request is configured to rxPauseOnFrame, the TX will be waken when receive no pause frame.
 */
RtkApiRet dva_eee_set_port_lpi_req(uint32 unit, RtkPort port, RtkEnable txRate, RtkEnable queueEmpty, RtkEnable rxPauseOnFrame)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    lpiCfgRegVal = 0u;
    RtkPort   sysPort      = dva_g_userPort2sysPort[port];

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* check input enable config: txRate */
        if((ENABLE != txRate) && (DISABLE != txRate))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* check input enable config: queueEmpty */
        if((ENABLE != queueEmpty) && (DISABLE != queueEmpty))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* check input enable config: rxPauseOnFrame */
        if((ENABLE != rxPauseOnFrame) && (DISABLE != rxPauseOnFrame))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /*
         * construct LPI TX request configuration value:
         * bit 0 -> Enable LPI TX request Rate decision maker for EEE
         * bit 1 -> Enable LPI TX request queue empty decision maker for EEE
         * bit 2 -> Enable LPI TX request RX pause on frame decision maker for EEE
         */
        lpiCfgRegVal = ((txRate << DVA_EEE_LPI_REQ_TXRATE_BIT_LSP) | (queueEmpty << DVA_EEE_LPI_REQ_QEMPTY_BIT_LSP) | (rxPauseOnFrame << DVA_EEE_LPI_REQ_RXPAUSE_BIT_LSP));
        /* set TX LPI request decision make configuration by indirect reg setting API */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_REQ_SET_2_0_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_REQ_SET_2_0_LEN,
                                                                lpiCfgRegVal));
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get the LPI request decision maker configuration of the addressed port.
 * @param[in] unit               Stacking unit (1 to 7) \n
 *                               Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port               Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                               The user port index of the selected port.
 * @param[out] *pTxRate          Enable TX Rate Decision Maker of LPI request (0 to 1) \n
 *                               The switch statistically evaluates the TX traffic over a period of time, and when it falls below a threshold value, \n
 *                               it will waits until TXEN is low before sending an LPI request.
 * @param[out] *pQueueEmpty      Enable TX Queue Empty Decision Maker of LPI request (0 to 1) \n
 *                               EEE TX enters the LPI state when the TX queue is empty.
 * @param[out] *pRxPauseOnFrame  Enable RX Pause on Frame Decision Maker of LPI request (0 to 1) \n
 *                               EEE TX enters the LPI state when receiving a PAUSE Frame and the signal TXEN is low.
 *
 * @retval RT_ERR_OK              Successfully get the LPI request decision maker configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID        The input port index is out of range.
 * @retval -RT_ERR_NULL_POINTER   The input pEeeTimerValueGet is NULL.
 * @retval Others                 Please refer to dva_drv_indirect_reg_field_set().
 */
RtkApiRet dva_eee_get_port_lpi_req(uint32 unit, RtkPort port, RtkEnable *pTxRate, RtkEnable *pQueueEmpty, RtkEnable *pRxPauseOnFrame)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    lpiCfgRegVal = 0u;
    RtkPort   sysPort      = dva_g_userPort2sysPort[port];

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL pointer parameters */
        if((NULL == pTxRate) || (NULL == pQueueEmpty) || (NULL == pRxPauseOnFrame))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* get TX LPI request decision maker configuration by indirect reg getting API */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_REQ_SET_2_0_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_REQ_SET_2_0_LEN,
                                                                &lpiCfgRegVal));
        /* bit 0 -> Enable LPI TX request Rate decision maker for EEE */
        if(1u == ((lpiCfgRegVal >> DVA_EEE_LPI_REQ_TXRATE_BIT_LSP) & 0x1u))
        {
            *pTxRate = ENABLE;
        }
        else
        {
            *pTxRate = DISABLE;
        }
        /* bit 1 -> Enable LPI TX request queue empty decision maker for EEE */
        if(1u == ((lpiCfgRegVal >> DVA_EEE_LPI_REQ_QEMPTY_BIT_LSP) & 0x1u))
        {
            *pQueueEmpty = ENABLE;
        }
        else
        {
            *pQueueEmpty = DISABLE;
        }
        /* bit 2 -> Enable LPI TX request RX pause on frame decision maker for EEE */
        if(1u == ((lpiCfgRegVal >> DVA_EEE_LPI_REQ_RXPAUSE_BIT_LSP) & 0x1u))
        {
            *pRxPauseOnFrame = ENABLE;
        }
        else
        {
            *pRxPauseOnFrame = DISABLE;
        }
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Set the threshold value for all Tx queue packet in unit of the designated port.
 *
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                      The user port index of the selected port.
 * @param[in] lpiQtxThr The TX rate threshold configuration value (0 to 255) \n
 *                      For TX rate LPI request decision maker.
 * @param[out] None
 *
 * @retval RT_ERR_OK       Successfully set the TX rate threshold configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID The input port index is out of range.
 * @retval Others          Please refer to dva_drv_indirect_reg_field_set().
 */
RtkApiRet dva_eee_set_port_lpi_qtx_thr(uint32 unit, RtkPort port, uint32 lpiQtxThr)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = dva_g_userPort2sysPort[port];

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* The min config value of TX rate threshold is 1. Configuring the threshold to 0 will prevent the port enter LPI state */
        if((1u > lpiQtxThr) || (0xFFu < lpiQtxThr))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Configure the threshold value of LPI TX request Rate decision maker */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_QTX_THR_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_QTX_THR_LEN,
                                                                lpiQtxThr));

    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get the threshold for all Tx queue packet in DSC unit of the designated port. \n
 *
 * @param[in] unit         Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port         Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                         The user port index of the selected port.
 * @param[out] *pLpiQtxThr The TX rate threshold configuration value (0 to 255) \n
 *                         For TX rate LPI request decision maker.\n
 *                         The default value is 48.
 *
 * @retval RT_ERR_OK            Successfully get the TX rate threshold configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID      The input port index is out of range.
 * @retval -RT_ERR_NULL_POINTER The input pEeeTimerValueGet is NULL.
 * @retval Others               Please refer to dva_drv_indirect_reg_field_set().
 */
RtkApiRet dva_eee_get_port_lpi_qtx_thr(uint32 unit, RtkPort port, uint32 *pLpiQtxThr)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = dva_g_userPort2sysPort[port];
    uint32    regVal  = 0u;

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL pointer parameters */
        if(NULL == pLpiQtxThr)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Configure the threshold value of LPI TX request Rate decision maker */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_QTX_THR_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_QTX_THR_LEN,
                                                                &regVal));
        regVal      = (regVal & CHAR_MASK);
        *pLpiQtxThr = (uint8)regVal;
    } while(1u == 0u);

    return retVal;
}
/**@}*/ /* EEE_TX_REQUEST_ASIC */
/**@}*/ /* EEE_TX_REQUEST */

/**
 * @addtogroup EEE_TX_WAKE EEE TX Wake Decision Maker Configurations
 * @{
 */

/**
 * @defgroup EEE_TX_WAKE_ASIC Low Level Driver
 * @brief EEE Low Level Driver
 * @{
 */
/**
 * @brief Set the high priority TX queue for QoS mode wake decision maker of the designated port. \n
 *
 * @param[in] unit        Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port        Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                        The user port index of the selected port.
 * @param[in] queuePriCfg High priority queue configuration (0 to 255) \n
 *                        Define the high priority queue for all eight Tx queue in LPI mode. \n
 *                        0b1: high priority queue \n
 *                        0b0: low priority queue \n
 *                        bit[0]: Tx queue 0 \n
 *                        bit[1]: Tx queue 1 \n
 *                        bit[2]: Tx queue 2 \n
 *                        bit[3]: Tx queue 3 \n
 *                        bit[4]: Tx queue 4 \n
 *                        bit[5]: Tx queue 5 \n
 *                        bit[6]: Tx queue 6 \n
 *                        bit[7]: Tx queue 7 \n
 *                        other bits are reserved.
 * @param[out] None
 *
 * @retval RT_ERR_OK       Successfully set the high priority queue configuration.
 * @retval -RT_ERR_PORT_ID The input port index is out of range.
 * @retval Others          Please refer to dva_drv_indirect_reg_field_set().
 */
RtkApiRet dva_eee_set_port_qtx_pri(uint32 unit, RtkPort port, uint32 queuePriCfg)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regValue = 0u;
    RtkPort   sysPort  = dva_g_userPort2sysPort[port];

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* Check the priority configuration is valid */
        if(queuePriCfg > 0xFFu)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* get the valid high priority queue configuration value */
        regValue = queuePriCfg & CHAR_MASK;
        /* configure the high priority of the designated port */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_Q_PRI_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_Q_PRI_LEN,
                                                                regValue));

    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get the high priority TX queue for QoS mode wake decision maker of the designated port. \n
 *
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port          Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                          The user port index of the selected port.
 * @param[in] *pQueuePriCfg High priority queue configuration (0 to 255) \n
 *                          Define the high priority queue for all eight Tx queue in LPI mode. \n
 *                          0b1: high priority queue \n
 *                          0b0: low priority queue \n
 *                          bit[0]: Tx queue 0 \n
 *                          bit[1]: Tx queue 1 \n
 *                          bit[2]: Tx queue 2 \n
 *                          bit[3]: Tx queue 3 \n
 *                          bit[4]: Tx queue 4 \n
 *                          bit[5]: Tx queue 5 \n
 *                          bit[6]: Tx queue 6 \n
 *                          bit[7]: Tx queue 7 \n
 *                          other bits are reserved.
 * @param[out] None
 *
 * @retval RT_ERR_OK             Successfully set the high priority queue configuration.
 * @retval -RT_ERR_PORT_ID       The input port index is out of range.
 * @retval -RT_ERR_NULL_POINTER  The input pWakeSet is NULL.
 * @retval Others                Please refer to dva_drv_indirect_reg_field_get().
 */
RtkApiRet dva_eee_get_port_qtx_pri(uint32 unit, RtkPort port, uint32 *pQueuePriCfg)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regValue = 0u;
    RtkPort   sysPort  = dva_g_userPort2sysPort[port];

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL pointer parameters */
        if(NULL == pQueuePriCfg)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get the high priority of the designated port */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_Q_PRI_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_Q_PRI_LEN,
                                                                &regValue));
        *pQueuePriCfg = regValue & CHAR_MASK;
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Set the TX Wake set of the addressed port. The Switch MAC TX is responsible for determining the timing for exiting the LPI state. \n
 *        The Switch provides three mechanisms to determine the conditions for exiting the LPI state. Condition 2 and 3 can be configured in this function. \n
 *	      1. Non-receipt of PAUSE Frame \n
 *	         If the EEE_PauseFlag is high, the Switch will only exit the LPI state when it does not receive a PAUSE frame. \n
 *           This wake setting is enabled after enable RX Pause on Frame Decision Maker of LPI request. \n
 *	      2. Normal mode \n
 *	         If the Switch has a packet to transmit, it will exit the LPI state. \n
 *	      3. QoS mode \n
 *	         If the Switch has a high-priority packet or control packet to transmit, it will exit the LPI state immediately. \n
 *	         If the Switch has a low-priority packet to transmit, it will accumulate packets in the queue until the timer expires or the packet number reaches the threshold. Then, the Switch will wake up
 *
 * @param[in] unit    Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port    Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                    The user port index of the selected port.
 * @param[in] wakeSet The Wake Set Configuration (N/A)
 *                    The EEE TX Wake set.
 * @param[out] None
 * @retval RT_ERR_OK        Successfully set the wake LPI decision maker configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID  The input port index is out of range.
 * @retval Others           Please refer to dva_drv_indirect_reg_field_set().
 * @note   If the LPI TX request is configured to rxPauseOnFrame, the Switch will only exit the LPI state when it does not receive a PAUSE frame.
 */
RtkApiRet dva_eee_set_port_wake(uint32 unit, RtkPort port, EeeWakeSet_e wakeSet)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = dva_g_userPort2sysPort[port];

    do
    {
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* set the wake LPI setting of the addressed timer type */
        switch(wakeSet)
        {
            /* set wake LPI mode to normal mode */
            case EM_EEE_WAKE_NORMAL_MODE:
                /* 2b'10: Normal mode. Enable LPI TX transmit wakeup decision maker for EEE */
                CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                        DVA_PORT0_EEE_MAC_CONTROL_EEE_WAKE_SET_1_0_LSP,
                                                                        DVA_PORT0_EEE_MAC_CONTROL_EEE_WAKE_SET_1_0_LEN,
                                                                        EEE_WAKE_NORMAL_MODE));
                break;
            /* set wake LPI mode to QoS mode */
            case EM_EEE_WAKE_QOS_MODE:
                /* 2b'01: QoS mode. Enable LPI TX high priority packet Wakeup decision maker for EEE */
                CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                        DVA_PORT0_EEE_MAC_CONTROL_EEE_WAKE_SET_1_0_LSP,
                                                                        DVA_PORT0_EEE_MAC_CONTROL_EEE_WAKE_SET_1_0_LEN,
                                                                        EEE_WAKE_QOS_MODE));
                break;
            /* unsupported type, return error */
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get the TX Wake set of the addressed port.
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port       Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_MAX) \n
 *                       The user port index of the selected port.
 * @param[out] *pWakeSet The Wake Set Configuration (N/A)
 *                       The EEE TX Wake set.
 *
 * @retval RT_ERR_OK              Successfully get the wake decision maker configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID        The input port index is out of range.
 * @retval -RT_ERR_NULL_POINTER   The input pWakeSet is NULL.
 * @retval -RT_ERR_NOT_SUPPORT    The return configuration value is not supported.
 * @retval Others                 Please refer to dva_drv_indirect_reg_field_get().
 * @note   If the LPI TX request is configured to rxPauseOnFrame, the Switch will only exit the LPI state when it does not receive a PAUSE frame.
 */
RtkApiRet dva_eee_get_port_wake(uint32 unit, RtkPort port, EeeWakeSet_e *pWakeSet)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    wakeCfgRegVal = 0u;
    RtkPort   sysPort       = dva_g_userPort2sysPort[port];

    do
    {
        /* check NULL pointer parameters */
        if(NULL == pWakeSet)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* check input port index */
        if((DVA_EEE_USER_PORT_MIN > port) || (DVA_EEE_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /*
         * bit0 -> 0b'1: Enable LPI TX high priority packet Wakeup decision maker for EEE
         * bit1 -> 0b'1: Enable LPI TX transmit wakeup decision maker for EEE
         * set wake setting by indirect reg getting API
         */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_EEE_MAC_CONTROL + (PERPORT_REG_OFFSET * (uint32)sysPort),
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_WAKE_SET_1_0_LSP,
                                                                DVA_PORT0_EEE_MAC_CONTROL_EEE_WAKE_SET_1_0_LEN,
                                                                &wakeCfgRegVal));
        /* if the wake LPI configuration value is QoS mode, return M_EEE_WAKE_QOS_MODE */
        if(EEE_WAKE_QOS_MODE == wakeCfgRegVal)
        {
            *pWakeSet = EM_EEE_WAKE_QOS_MODE;
        }
        /* if the wake LPI configuration value is normal mode, return EM_EEE_WAKE_NORMAL_MODE */
        else if(EEE_WAKE_NORMAL_MODE == wakeCfgRegVal)
        {
            *pWakeSet = EM_EEE_WAKE_NORMAL_MODE;
        }
        /* unsupported type, return error */
        else
        {
            retVal = -RT_ERR_NOT_SUPPORT;
        }
    } while(1u == 0u);

    return retVal;
}
/**@}*/ /* EEE_TX_WAKE_ASIC */
/**@}*/ /* EEE_TX_WAKE */
