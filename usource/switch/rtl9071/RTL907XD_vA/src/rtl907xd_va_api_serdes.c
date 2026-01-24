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

#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_serdes.h"
#include "rtl907xd_va_asicdrv_port.h"
#include "rtl907xd_va_api_ext.h"

#define DVA_HW_MISC_DMY6_SDS_ROUTINE_ENABLE_LSP (12u)
#define DVA_HW_MISC_DMY6_SDS_ROUTINE_ENABLE_LEN (1u)

/**
 * @addtogroup RTK_MODULE_SERDES SERDES
 * @{
 */

/**
 * @addtogroup SERDES_GET_SET_REGISTER Serdes indirect access register
 * @{
 */

/**
 * @defgroup SERDES_GET_SET_REGISTER_ASIC Low Level Driver
 * @brief SERDES_GET_SET_REGISTER_ASIC Low Level Driver
 * @{
 */

/**
 * @brief A function used to get the value of the fields in the SERDES PCS register.
 * @param[in] unit                                Stacking unit (1 to 7) \n
 *                                                The stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                                Switch port (DVA_RTK_USER_PORT_4TH to DVA_RTK_USER_PORT_12TH)  \n
 *                                                The user port number of the switch.
 * @param[in] page                                Serdes register page (0 to 54)  \n
 *                                                The page number of the serdes PCS register.
 * @param[in] reg                                 Serdes register (0 to 31)  \n
 *                                                The serdes register number.
 * @param[in] startBit                            Start bit (0 to 15)  \n
 *                                                The start bit of the partial read register.
 * @param[in] endBit                              End bit (0 to 15)  \n
 *                                                The end bit of the partial read register.
 * @param[out] value                              Value (0 to 0xFFFF) \n
 *                                                Pointer to the read back PCS register value.
 * @retval RT_ERR_OK                              API successfully gets the value of the PCS register.
 * @retval -RT_ERR_PORT_ID                        Interface is not serdes port.
 * @retval -RT_ERR_SDS_PAGE_ID                    Page ID is out of the specified range.
 * @retval -RT_ERR_SDS_REG_ID                     Register ID is out of the specified range.
 * @retval -RT_ERR_SDS_END_BIT                    End bit exceeds the specified range.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT  Start bit is greater than end bit. Example: start bit is bit 10, end bit is bit 6.
 * @retval -RT_ERR_SDS_NULL_VALUE                 Value is a null pointer.
 * @retval Others                                 Please refer to dva_drv_indirect_serdes_get().
 */
RtkApiRet dva_sys_sds_reg_getbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value)
{
    uint32    i            = 0u;
    uint32    mask         = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_chk_port_range(port));

        /* Check if page number exceeds maximum */
        if(DVA_SDS_PCS_MAX_PAGE_NUM < page)
        {
            ret = -RT_ERR_SDS_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(DVA_SDS_PCS_MAX_REG_NUM < reg)
        {
            ret = -RT_ERR_SDS_REG_ID;
            continue;
        }

        /* Check if end bit exceeds maximum */
        if(endBit > DVA_UMAC_REG_END_BIT)
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
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Initialize value */
        *value = 0u;

        /* Convert user port to system port */
        sysPort = dva_g_userPort2sysPort[port];

        if(DVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the PCS register value via FW virtual channel */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_serdes_get(unit, DVA_SDS_PCS_TYPE, sysPort, page, reg, value));
        }
        else
        {
            /* Get the PCS register value via MDC/MDIO */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_pcs_direct_get(unit, sysPort, page, reg, value));
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
 * @param[in] port                                Switch port (DVA_RTK_USER_PORT_4TH to DVA_RTK_USER_PORT_12TH)  \n
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
 * @retval RT_ERR_OK                              The API gets the APHY register value successfully.
 * @retval -RT_ERR_PORT_ID                        The interface is not a serdes port.
 * @retval -RT_ERR_PHY_PAGE_ID                    The page ID is not 0.
 * @retval -RT_ERR_SDS_APHY_REG_ADDR              The register address is not an APHY address.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT  The start bit is greater than the end bit. e.g. start bit is bit 10, end bit is bit 6.
 * @retval -RT_ERR_SDS_NULL_VALUE                 The value is a null pointer.
 * @retval Others                                 Please refer to dva_drv_indirect_serdes_get().
 */
RtkApiRet dva_sys_sds_reg_getbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 *value)
{

    uint32    i            = 0u;
    uint32    mask         = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_chk_port_range(port));

        /* Check if page number not zero */
        if(page != 0u)
        {
            ret = -RT_ERR_PHY_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(DVA_SDS_APHY_MAX_REG_ADDR < reg)
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
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Initializes value */
        *value = 0u;

        /* Convert user port to system port */
        sysPort = dva_g_userPort2sysPort[port];

        /* Convert APHY system port 10 & port 12 to port 9 & port 11 */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_aphy_covert_port(DVA_SDS_APHY_TYPE, &sysPort));

        if(DVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the aphy register value via FW virtual channel */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_serdes_get(unit, DVA_SDS_APHY_TYPE, sysPort, page, reg, value));
        }
        else
        {
            /* Get the aphy register value via MDC/MDIO */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_aphy_direct_get(unit, sysPort, page, reg, value));
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
 *                                                  The stacking unit ID is used to indicate which switch to access.
 * @param[in] port                                  Switch port (DVA_RTK_USER_PORT_4TH to DVA_RTK_USER_PORT_12TH)  \n
 *                                                  The user port number on the switch
 * @param[in] page                                  Serdes register page (0 to 54)  \n
 *                                                  The page number of the Serdes PCS register
 * @param[in] reg                                   Serdes register (0 to 31)  \n
 *                                                  The Serdes register number
 * @param[in] startBit                              Start bit (0 to 15)  \n
 *                                                  The start bit of the partial read register
 * @param[in] endBit                                End bit (0 to 15)  \n
 *                                                  The end bit of the partial read register
 * @param[in] value                                 Value (0 to 0xFFFF)  \n
 *                                                  The value to set for the PCS register, the range depends on startBit and endBit
 * @param[out]
 * @retval RT_ERR_OK                                The API successfully set the value of the PCS register.
 * @retval -RT_ERR_PORT_ID                          The interface is not a Serdes port.
 * @retval -RT_ERR_SDS_PAGE_ID                      The page ID is out of the specified range.
 * @retval -RT_ERR_SDS_REG_ID                       The register ID is out of the specified range.
 * @retval -RT_ERR_SDS_END_BIT                      The end bit exceeds the specified range.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT    The start bit is greater than the end bit.
 * @retval -RT_ERR_SDS_INPUT_OUT_OF_RANGE           The set value is out of the specified range.
 * @retval Others                                   Please refer to dva_drv_indirect_serdes_get(), dva_reg_field_set() and dva_drv_indirect_serdes_set().
 */
RtkApiRet dva_sys_sds_reg_setbits_pcs(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    uint32    regVal       = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    length       = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_chk_port_range(port));

        /* Check if page number exceeds maximum */
        if(DVA_SDS_PCS_MAX_PAGE_NUM < page)
        {
            ret = -RT_ERR_SDS_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(DVA_SDS_PCS_MAX_REG_NUM < reg)
        {
            ret = -RT_ERR_SDS_REG_ID;
            continue;
        }

        /* Check if end bit exceeds maximum */
        if(endBit > DVA_UMAC_REG_END_BIT)
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
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Convert user port to system port */
        sysPort = dva_g_userPort2sysPort[port];

        /* Set register via firmware if fully initialized, otherwise directly */
        if(DVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the PCS register value through the firmware virtual channel */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_serdes_get(unit, DVA_SDS_PCS_TYPE, sysPort, page, reg, &regVal));

            /* Set the PCS register value through the firmware virtual channel */
            length = (endBit - startBit) + 1u;
            CHK_FUN_RET(ret, dva_reg_field_set(reg, startBit, length, value, &regVal));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_serdes_set(unit, DVA_SDS_PCS_TYPE, sysPort, page, reg, regVal));
        }
        else
        {
            /* Set the PCS register value through the firmware virtual channel MDC/MDIO*/
            CHK_FUN_CONTINUE(ret, dva_drv_sds_pcs_direct_set(unit, sysPort, page, reg, startBit, endBit, value));
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief A function used to set the value of the fields in the SERDES APHY register.
 * @param[in] unit                                 Stacking unit (1 to 7) \n
 *                                                 Stacking unit ID is used to indicate which switch to access.
 * @param[in] port                                 Switch port (DVA_RTK_USER_PORT_4TH to DVA_RTK_USER_PORT_12TH) \n
 *                                                 User port number of the switch
 * @param[in] page                                 Serdes register page (0) \n
 *                                                 The page number of serdes APHY register
 * @param[in] reg                                  Serdes register (0 to 0xFFFF) \n
 *                                                 Serdes APHY register number
 * @param[in] startBit                             Start bit (0 to 15) \n
 *                                                 Start bit of partial read register
 * @param[in] endBit                               End bit (0 to 15) \n
 *                                                 End bit of partial read register
 * @param[in] value                                Value (0 to 0xFFFF) \n
 *                                                 Set value to APHY register, value range depends on startBit and endBit
 * @param[out]
 * @retval RT_ERR_OK                               API sets value of APHY register successfully.
 * @retval -RT_ERR_PORT_ID                         Interface is not a serdes port.
 * @retval -RT_ERR_PHY_PAGE_ID                     Page id is not 0.
 * @retval -RT_ERR_SDS_APHY_REG_ADDR               Register address is not an APHY address.
 * @retval -RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT   Start bit greater than end bit. Ex: Start bit is bit 10, end bit is bit 6.
 * @retval -RT_ERR_SDS_INPUT_OUT_OF_RANGE          Set value out of specify range.
 * @retval Others                                  Please refer to dva_drv_indirect_serdes_get(), dva_reg_field_set() and dva_drv_indirect_serdes_set().
 */
RtkApiRet dva_sys_sds_reg_setbits_aphy(uint32 unit, RtkPort port, uint32 page, uint32 reg, uint32 startBit, uint32 endBit, uint32 value)
{
    uint32    regVal       = 0u;
    RtkApiRet ret          = RT_ERR_OK;
    uint32    sysPort      = 0u;
    uint32    length       = 0u;
    uint32    bootupStatus = 0u;

    do
    {
        /* Check if the input parameters are valid */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_chk_port_range(port));

        /* Check if page number not zero */
        if(page != 0u)
        {
            ret = -RT_ERR_PHY_PAGE_ID;
            continue;
        }

        /* Check if register number exceeds maximum */
        if(DVA_SDS_APHY_MAX_REG_ADDR < reg)
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
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* Convert user port to system port */
        sysPort = dva_g_userPort2sysPort[port];

        /* Convert APHY system ports 10 & 12 to ports 9 & 11 */
        CHK_FUN_CONTINUE(ret, dva_drv_sds_aphy_covert_port(DVA_SDS_APHY_TYPE, &sysPort));

        if(DVA_BOOT_UP_FULL_INITIZED == bootupStatus)
        {
            /* Get the aphy register value via FW virtual channel*/
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_serdes_get(unit, DVA_SDS_APHY_TYPE, sysPort, page, reg, &regVal));

            /* Set the aphy register value via FW virtual channel*/
            length = (endBit - startBit) + 1u;
            CHK_FUN_RET(ret, dva_reg_field_set(reg, startBit, length, value, &regVal));

            CHK_FUN_CONTINUE(ret, dva_drv_indirect_serdes_set(unit, DVA_SDS_APHY_TYPE, sysPort, page, reg, regVal));
        }
        else
        {
            /* Set the aphy register value directly via MDC/MDIO */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_aphy_direct_set(unit, sysPort, page, reg, startBit, endBit, value));
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Configure the SERDES interface for a specified port in flashless over ethernet mode.
 * @param[in] unit          Stacking unit (1 to 7)
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort      Port ID (DVA_RTK_USER_PORT_4TH to DVA_RTK_USER_PORT_12TH)
 *                          The port ID indicates which port is being configured.
 * @param[in] serdesMode    SERDES mode to be set (usxgmii, hisgmii, sgmii, 5gkr)
 *                          Specifies the SERDES mode for the port.
 * @param[in] portRole      Role of the port (e.g., master or slave)
 *                          Specifies the role of the port in the network.
 * @param[in] isForceMode   Force mode enable/disable
 *                          Specifies whether force mode should be enabled or disabled.
 * @param[in] speed         Speed setting for the port
 *                          Specifies the speed at which the port should operate.
 * @param[in] portSpeed     Port speed configuration
 *                          Specifies the speed configuration for the port.
 * @retval  RT_ERR_OK       To execute this function correctly
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval Others           Please refer to dva_drv_sds_mode_chk()
 * @note This API configures the SERDES interface for the specified port and mode.
 */
RtkApiRet dva_sys_sds_config_set(uint32 unit, RtkPort userPort, RtkModeExt_e serdesMode, uint32 portRole, uint32 isForceMode, uint32 speed, uint32 portSpeed)
{
    /* Initialize return value to indicate no error */
    RtkApiRet ret = RT_ERR_OK;
    /* Initialize boot status value */
    uint32 bootupStatus = 0u;

    do
    {
        /* Error handler for port check, which port number should be serdes user port (7, 9, 10, and 11) */
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            /* Return error if userPort is not valid */
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));

        /* if boot status is equal to flashless ethernet mode */
        if(DVA_SYS_BOOT_UP_FLASHLESS_ETH == bootupStatus)
        {
            /* Error handler for serdesMode check, which serdesMode should be usxgmii, hisgmii, sgmii, and 5gkr */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_mode_chk(serdesMode));

            /* Write to the VBGA top register */
            CHK_FUN_CONTINUE(ret, dva_reg_write(unit, DVA_VBGA_TOP_REG, 0x1D0));

            /* Disable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, DISABLE));

            /* Set the combo interface for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_combo_intf_set(unit, userPort, serdesMode));

            /* Set the SERDES mode for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_sds_mode_set(unit, userPort, serdesMode, portRole, isForceMode, speed, portSpeed));

            /* Enable eco clock for the port */
            CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, ENABLE));

            /* Disable state change latch */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, DISABLE));

            /* Enable SDS routine */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_HW_MISC_DMY6, DVA_HW_MISC_DMY6_SDS_ROUTINE_ENABLE_LSP, DVA_HW_MISC_DMY6_SDS_ROUTINE_ENABLE_LEN, ENABLE));
        }
        else
        {
            /* return error code */
            ret = -RT_ERR_ETH_FLASH_STATE;
        }

    } while(0u == 1u);

    /* Return the result of the configuration */
    return ret;
}

/**
 * @brief Set the force link status for the SERDES interface on a specified port in flashless over ethernet mode.
 * @param[in] unit          Stacking unit (1 to 7)
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] userPort      Port ID (DVA_RTK_USER_PORT_4TH to DVA_RTK_USER_PORT_12TH)
 *                          The port ID indicates which port is being configured.
 * @param[in] serdesMode    SERDES mode to be set (usxgmii, hisgmii, sgmii, 5gkr)
 *                          Specifies the SERDES mode for the port.
 * @param[in] status        Link status to be set (e.g., link up or down)
 *                          Specifies whether the link should be forced up or down.
 * @retval  RT_ERR_OK       To execute this function correctly
 * @retval -RT_ERR_PORT_ID  The port ID is out of range
 * @retval Others           Please refer to dva_drv_sds_1gmac_link_set() and dva_drv_sds_5gmac_link_set()
 * @note This API sets the force link status for the specified SERDES port and mode.
 */
RtkApiRet dva_sds_force_link_set(uint32 unit, RtkPort userPort, RtkModeExt_e serdesMode, uint32 status)
{
    /* Initialize return value to indicate no error */
    RtkApiRet ret = RT_ERR_OK;
    /* Initialize result to false */
    bool result = false;
    /* Initialize boot status value */
    uint32 bootupStatus = 0u;

    do
    {
        /* Check if the userPort is within valid range */
        if((DVA_RTK_USER_PORT_4TH > userPort) || (DVA_RTK_USER_PORT_12TH < userPort))
        {
            /* Return error if userPort is not valid */
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Get bootup status */
        CHK_FUN_CONTINUE(ret, dva_reg_field_read(unit, DVA_CPU_DMY_1, DVA_CPU_DMY_1_BOOTUP_STATUS_LSP, DVA_CPU_DMY_1_BOOTUP_STATUS_LEN, &bootupStatus));
        /* if boot status is equal to flashless ethernet mode */
        if(DVA_SYS_BOOT_UP_FLASHLESS_ETH == bootupStatus)
        {
            /* Poll to check if there are no packets being transmitted */
            CHK_FUN_CONTINUE(ret, dva_drv_port_polling_tx_nopkt(unit, userPort, &result));

            if(true == result)
            {
                /* Disable eco clock for the port */
                CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, DISABLE));

                /* Set the 1G MAC link status */
                CHK_FUN_CONTINUE(ret, dva_drv_sds_1gmac_link_set(unit, userPort, serdesMode, status));

                /* Disable state change latch */
                CHK_FUN_CONTINUE(ret, dva_drv_indirect_target_set_reg_field(unit, DVA_SMI_CONTROL, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LSP, DVA_SMI_CONTROL_ST_CHG_LATCH_EN_LEN, DISABLE));

                /* Enable eco clock for the port */
                CHK_FUN_CONTINUE(ret, dva_drv_port_mactop_ecoclk_set(unit, userPort, ENABLE));

                /* Set the 5G MAC link status */
                CHK_FUN_CONTINUE(ret, dva_drv_sds_5gmac_link_set(unit, userPort, serdesMode, status));
            }
        }
        else
        {
            /* return error code */
            ret = -RT_ERR_ETH_FLASH_STATE;
        }
    } while(0u == 1u);

    /* Return the result of the configuration */
    return ret;
}
/**@}*/ /*SERDES_GET_SET_REGISTER_ASIC*/
/**@}*/ /*SERDES_GET_SET_REGISTER*/
/**@}*/ /*Serdes*/