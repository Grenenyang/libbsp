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

#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_asicdrv_serdes.h"
#include "rtl907xc_va_api_ext.h"

/**
 * @addtogroup RTK_MODULE_SERDES SERDES
 * @{
 */

/**
 * @addtogroup SERDES_GET_SET_REGISTER Serdes indirect access register
 * @{
 */

/**
 * @addtogroup SERDES_GET_SET_REGISTER_ASIC Low Level Driver
 * @{
 */

/**
 * @brief A function used to get the value of the fields in the SERDES PCS register.
 * @param[in] unit                                Stacking unit (1 to 7) \n
 *                                                Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                                Switch port (CVA_RTK_USER_PORT_7TH, CVA_RTK_USER_PORT_9TH to CVA_RTK_USER_PORT_11TH)  \n
 *                                                User port number of the switch
 * @param[in] page                                Serdes register page (0 to 54)  \n
 *                                                The page number of serdes PCS register
 * @param[in] reg                                 Serdes register (0 to 31)  \n
 *                                                Serdes PCS register number
 * @param[in] startBit                            Start bit (0 to 15)  \n
 *                                                Start bit of partial read register
 * @param[in] endBit                              End bit (0 to 15)  \n
 *                                                End bit of partial read register
 * @param[out] value                              Value (0 to 0xFFFF) \n
 *                                                Pointer to read back PCS register value
 * @retval RT_ERR_OK                              The API gets the value of PCS register successfully.
 * @retval -RT_ERR_PORT_ID                        The interface is not a serdes port.
 * @retval -RT_ERR_SDS_PAGE_ID                    Page ID out of specified range.
 * @retval -RT_ERR_SDS_REG_ID                     Register ID out of specified range.
 * @retval -RT_ERR_SDS_END_BIT                    End bit exceeds specified range.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT  Start bit is greater than end bit. Ex: start bit is bit 10, end bit is bit 6.
 * @retval -RT_ERR_SDS_NULL_VALUE                 Value is a null pointer.
 * @retval Others                                 Please refer to cva_drv_indirect_serdes_get().
 */
RtkApiRet cva_sys_sds_reg_getbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value)
{
    uint32    i            = 0u;
    uint32    mask         = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_sds_chk_port_range(unit, port));

        /* Check if page number exceeds maximum */
        if(CVA_SDS_PCS_MAX_PAGE_NUM < page)
        {
            ret = -RT_ERR_SDS_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(CVA_SDS_PCS_MAX_REG_NUM < reg)
        {
            ret = -RT_ERR_SDS_REG_ID;
            continue;
        }

        /* Check if end bit exceeds maximum */
        if(endBit > CVA_UMAC_REG_END_BIT)
        {
            ret = -RT_ERR_SDS_END_BIT;
            continue;
        }

        /* Check if start bit exceeds end bit */
        if(startBit > endBit)
        {
            ret = -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT;
            continue;
        }

        /* Check if value pointer is NULL */
        if(value == NULL)
        {
            ret = -RT_ERR_SDS_NULL_VALUE;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_CPU_DMY_1, CVA_CPU_DMY_1_BOOTUP_STATUS_LSP, CVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Initialize value */
        *value = 0u;

        /* Convert user port to system port */
        sysPort = cva_g_userPort2sysPort[port];

        if(CVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the PCS register value via FW virtual channel */
            CHK_FUN_CONTINUE(ret, cva_drv_indirect_serdes_get(unit, CVA_SDS_PCS_TYPE, sysPort, page, reg, value));
        }
        else
        {
            /* Get the PCS register value via MDC/MDIO */
            CHK_FUN_CONTINUE(ret, cva_drv_sds_pcs_direct_get(unit, sysPort, page, reg, value));
        }

        /* Get the PCS register value from start bit to end bit */
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            mask |= (uint32)0x1u << (startBit + i);
        }

        *value &= mask;
        *value >>= startBit;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief A function used to get the value of the fields in the SERDES APHY register.
 * @param[in] unit                                Stacking unit (1 to 7) \n
 *                                                The stacking unit ID is used to indicate which switch to access.
 * @param[in] port                                Switch port (CVA_RTK_USER_PORT_7TH, CVA_RTK_USER_PORT_9TH to CVA_RTK_USER_PORT_11TH)  \n
 *                                                The user port number of the switch
 * @param[in] page                                Serdes register page (0)  \n
 *                                                The page number of the serdes APHY register
 * @param[in] reg                                 Serdes register (0 to 0xFFFF)  \n
 *                                                The serdes APHY register number
 * @param[in] startBit                            Start bit (0 to 15)  \n
 *                                                The start bit of the partial read register
 * @param[in] endBit                              End bit (0 to 15)  \n
 *                                                The end bit of the partial read register
 * @param[out] value                              Value (0 to 0xFFFF)  \n
 *                                                Pointer to the read back APHY register value
 * @retval RT_ERR_OK                              The API gets the value of APHY register successfully.
 * @retval -RT_ERR_PORT_ID                        The interface is not a serdes port.
 * @retval -RT_ERR_PHY_PAGE_ID                    The page ID is not 0.
 * @retval -RT_ERR_SDS_APHY_REG_ADDR              The register address is not an APHY address.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT  The start bit is greater than the end bit. Ex: start bit is bit 10, end bit is bit 6.
 * @retval -RT_ERR_SDS_NULL_VALUE                 The value is a null pointer.
 * @retval Others                                 Please refer to cva_drv_indirect_serdes_get().
 */
RtkApiRet cva_sys_sds_reg_getbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value)
{

    uint32    i            = 0u;
    uint32    mask         = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_sds_chk_port_range(unit, port));

        /* Check if page number not zero */
        if(page != 0u)
        {
            ret = -RT_ERR_PHY_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(CVA_SDS_APHY_MAX_REG_ADDR < reg)
        {
            ret = -RT_ERR_SDS_APHY_REG_ADDR;
            continue;
        }

        /* Check if start bit exceeds end bit */
        if(startBit > endBit)
        {
            ret = -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT;
            continue;
        }

        /* Check if value pointer is NULL */
        if(value == NULL)
        {
            ret = -RT_ERR_SDS_NULL_VALUE;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_CPU_DMY_1, CVA_CPU_DMY_1_BOOTUP_STATUS_LSP, CVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Initialize value */
        *value = 0u;

        /* Convert user port to system port */
        sysPort = cva_g_userPort2sysPort[port];

        if(CVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the aphy register value via FW virtual channel */
            CHK_FUN_CONTINUE(ret, cva_drv_indirect_serdes_get(unit, CVA_SDS_APHY_TYPE, sysPort, page, reg, value));
        }
        else
        {
            /* Get the aphy register value via MDC/MDIO */
            CHK_FUN_CONTINUE(ret, cva_drv_sds_aphy_direct_get(unit, sysPort, page, reg, value));
        }

        /* Get the aphy register value from start bit to end bit */
        for(i = 0u; i < ((endBit - startBit) + 1u); i++)
        {
            mask |= (uint32)0x1u << (startBit + i);
        }

        *value &= mask;
        *value >>= startBit;
    } while(0u == 1u);

    return ret;
}

/**
 * @brief A function used to set the value of the fields in the SERDES PCS register.
 * @param[in] unit                                  Stacking unit (1 to 7) \n
 *                                                  Stacking unit ID is used to indicate which switch to be accessed.
 * @param[in] port                                  Switch port (CVA_RTK_USER_PORT_7TH to CVA_RTK_USER_PORT_11TH)  \n
 *                                                  User port number of the switch
 * @param[in] page                                  Serdes register page (0 to 54)  \n
 *                                                  The page number of serdes PCS register
 * @param[in] reg                                   Serdes register (0 to 31)  \n
 *                                                  Serdes PCS register number
 * @param[in] startBit                              Start bit (0 to 15)  \n
 *                                                  Start bit of partial read register
 * @param[in] endBit                                End bit (0 to 15)  \n
 *                                                  End bit of partial read register
 * @param[in] value                                 Value (0 to 0xFFFF)  \n
 *                                                  Set value to PCS register, value range depends on startBit and endBit
 * @param[out]
 * @retval RT_ERR_OK                                The API sets value of PCS register successfully.
 * @retval -RT_ERR_PORT_ID                          The interface is not a serdes port.
 * @retval -RT_ERR_SDS_PAGE_ID                      Page ID out of specify range.
 * @retval -RT_ERR_SDS_REG_ID                       Register ID out of specify range.
 * @retval -RT_ERR_SDS_END_BIT                      End bit exceeds specified range.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT    Start bit is greater than end bit.
 * @retval -RT_ERR_SDS_INPUT_OUT_OF_RANGE           Set value is out of specified range.
 * @retval Others                                   Please refer to cva_drv_indirect_serdes_get(), cva_reg_field_set() and cva_drv_indirect_serdes_set().
 */
RtkApiRet cva_sys_sds_reg_setbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    uint32    regVal       = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    length       = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_sds_chk_port_range(unit, port));

        /* Check if page number exceeds maximum */
        if(CVA_SDS_PCS_MAX_PAGE_NUM < page)
        {
            ret = -RT_ERR_SDS_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(CVA_SDS_PCS_MAX_REG_NUM < reg)
        {
            ret = -RT_ERR_SDS_REG_ID;
            continue;
        }

        /* Check if end bit exceeds maximum */
        if(endBit > CVA_UMAC_REG_END_BIT)
        {
            ret = -RT_ERR_SDS_END_BIT;
            continue;
        }

        /* Check if start bit exceeds end bit */
        if(startBit > endBit)
        {
            ret = -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT;
            continue;
        }

        /* Check if value exceeds allowed range based on start and end bits */
        if(value > ((uint32)1u << ((endBit - startBit) + 1u)))
        {
            ret = RT_ERR_SDS_INPUT_OUT_OF_RANGE;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_CPU_DMY_1, CVA_CPU_DMY_1_BOOTUP_STATUS_LSP, CVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Convert user port to system port */
        sysPort = cva_g_userPort2sysPort[port];

        if(CVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the PCS register value via FW virtual channel */
            CHK_FUN_CONTINUE(ret, cva_drv_indirect_serdes_get(unit, CVA_SDS_PCS_TYPE, sysPort, page, reg, &regVal));

            /* Set the PCS register value through the firmware virtual channel */
            length = (endBit - startBit) + 1u;
            CHK_FUN_RET(ret, cva_reg_field_set(reg, startBit, length, value, &regVal));
            CHK_FUN_CONTINUE(ret, cva_drv_indirect_serdes_set(unit, CVA_SDS_PCS_TYPE, sysPort, page, reg, regVal));
        }
        else
        {
            /* Set the PCS register value through the firmware virtual channel MDC/MDIO*/
            CHK_FUN_CONTINUE(ret, cva_drv_sds_pcs_direct_set(unit, sysPort, page, reg, startBit, endBit, value));
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief A function used to set the value of the fields in the SERDES APHY register.
 * @param[in] unit                                 Stacking unit (1 to 7) \n
 *                                                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                                 Switch port  (CVA_RTK_USER_PORT_7TH to CVA_RTK_USER_PORT_11TH)  \n
 *                                                 User port number of the switch
 * @param[in] page                                 Serdes register page (0)  \n
 *                                                 The page number of serdes APHY register
 * @param[in] reg                                  Serdes register (0 to 0xFFFF)  \n
 *                                                 Serdes APHY register number
 * @param[in] startBit                             Start bit (0 to 15)  \n
 *                                                 Start bit of partial read register
 * @param[in] endBit                               End bit (0 to 15)  \n
 *                                                 End bit of partial read register
 * @param[in] value                                Value (0 to 0xFFFF)  \n
 *                                                 Set value to APHY register, value range is depend on startBit and EndBit
 * @param[out]
 * @retval RT_ERR_OK                               API sets value of APHY register Successfully.
 * @retval -RT_ERR_PORT_ID                         Interface is not serdes port.
 * @retval -RT_ERR_PHY_PAGE_ID                     page id is not 0.
 * @retval -RT_ERR_SDS_APHY_REG_ADDR               Register address is not APHY address.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT   Start bit greater than end bit. ex: start bit is bit 10, end bit is bit 6.
 * @retval -RT_ERR_SDS_INPUT_OUT_OF_RANGE          Set value out of specify range.
 * @retval Others                                  Please refer to cva_drv_indirect_serdes_get(), cva_reg_field_set() and cva_drv_indirect_serdes_set().
 */
RtkApiRet cva_sys_sds_reg_setbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    uint32    regVal       = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    length       = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, cva_drv_sds_chk_port_range(unit, port));

        /* Check if page number not zero */
        if(page != 0u)
        {
            ret = -RT_ERR_PHY_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(CVA_SDS_APHY_MAX_REG_ADDR < reg)
        {
            ret = -RT_ERR_SDS_APHY_REG_ADDR;
            continue;
        }

        /* Check if start bit exceeds end bit */
        if(startBit > endBit)
        {
            ret = -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT;
            continue;
        }

        /* Check if value exceeds allowed range based on start and end bits */
        if(value > ((uint32)1u << ((endBit - startBit) + 1u)))
        {
            ret = -RT_ERR_SDS_INPUT_OUT_OF_RANGE;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_CPU_DMY_1, CVA_CPU_DMY_1_BOOTUP_STATUS_LSP, CVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Convert user port to system port */
        sysPort = cva_g_userPort2sysPort[port];

        if(CVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the aphy register value via FW virtual channel*/
            CHK_FUN_CONTINUE(ret, cva_drv_indirect_serdes_get(unit, CVA_SDS_APHY_TYPE, sysPort, page, reg, &regVal));

            /* Set the aphy register value via FW virtual channel*/
            length = (endBit - startBit) + 1u;
            CHK_FUN_RET(ret, cva_reg_field_set(reg, startBit, length, value, &regVal));

            CHK_FUN_CONTINUE(ret, cva_drv_indirect_serdes_set(unit, CVA_SDS_APHY_TYPE, sysPort, page, reg, regVal));
        }
        else
        {
            /* Set the aphy register value directly via MDC/MDIO */
            CHK_FUN_CONTINUE(ret, cva_drv_sds_aphy_direct_set(unit, sysPort, page, reg, startBit, endBit, value));
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /*SERDES_GET_SET_REGISTER_ASIC*/
/**@}*/ /*SERDES_GET_SET_REGISTER*/
/**@}*/ /*SERDES*/