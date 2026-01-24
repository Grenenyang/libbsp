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
#include "rtkas_types.h"
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_asicdrv_opfsm.h"
#include "rtl907xd_va_asicdrv_phy.h"
#include "rtl907xd_va_asicdrv_eee.h"

/**
 * @brief Enable or disable PHY EEE configuration of the designated port.
 * @param[in] unit     Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port     Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_6ST) \n
 *                     The user port index of the selected port.
 * @param[in] enPhyEee The configuration of control bit of disabling the EEE function of all port. (0, 1) \n
 *                     1: Enable PHY EEE function of the addressed ports. \n
 *                     0: Disable PHY EEE function of the addressed ports.
 * @param[out] None
 *
 * @retval RT_ERR_OK       Successfully enable or disable PHY EEE configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID The input port index is out of range.
 * @retval -RT_ERR_INPUT   The input enPhyEee is invalid.
 * @retval Others          Please refer to dva_reg_field_write().
 * @note   Only port1 to port6 need to configure the corresponding PHY EEE functions.
 */
RtkApiRet dva_drv_eee_set_phy_en(uint32 unit, RtkPort port, RtkEnable enPhyEee)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regVal    = ((ENABLE == enPhyEee) ? 1u : 0u);
    uint32    regOffset = 0u;

    do
    {
        /* check input port index. */
        if((DVA_OP_T1_USER_PORT_MIN > port) || (DVA_OP_T1_USER_PORT_MAX < port))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* check input enable config: enPhyEee */
        if((ENABLE != enPhyEee) && (DISABLE != enPhyEee))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* get the PHY OCP address of the addressed port index */
        regOffset = DVA_PAGE_SWR_OCP | ((dva_g_sysPortToOcpAddr[dva_g_userPort2sysPort[port]] | DVA_OCP_OAM_CAP_GIGAPHY) << 1u);
        /* set PHY oam enable config */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regOffset,
                                                     DVA_OCP_RG_OAM_EN_LSP,
                                                     DVA_OCP_RG_OAM_EN_LEN,
                                                     regVal));

        /* set PHY EEE enable config */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regOffset,
                                                     DVA_OCP_RG_EEE_EN_LSP,
                                                     DVA_OCP_RG_EEE_EN_LEN,
                                                     regVal));

        regOffset = DVA_PAGE_SWR_OCP | ((dva_g_sysPortToOcpAddr[dva_g_userPort2sysPort[port]] | DVA_OCP_BMCR) << 1u);
        /* set PHY reset */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regOffset,
                                                     DVA_PHY_RST_LSP,
                                                     DVA_PHY_RST_LEN,
                                                     regVal));

        regOffset = DVA_PAGE_SWR_OCP | ((dva_g_sysPortToOcpAddr[dva_g_userPort2sysPort[port]] | DVA_OCP_PHYCORE_D15C) << 1u);
        /* set PHY AFE MCK EEE disable config */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regOffset,
                                                     DVA_OCP_RG_AFE_MCK_EEE_DISABLE_LSP,
                                                     DVA_OCP_RG_AFE_MCK_EEE_DISABLE_LEN,
                                                     regVal));
    } while(1u == 0u);

    return retVal;
}

/**
 * @brief Get the PHY EEE configuration of the designated port.
 * @param[in] unit        Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port        Port ID (DVA_EEE_USER_PORT_MIN to DVA_EEE_USER_PORT_6ST) \n
 *                        The user port index of the selected port.
 * @param[out] *pEnPhyEee To get the PHY EEE enable configuration of the addressed port. (0, 1) \n
 *                        1: PHY EEE function of the addressed ports is enabled. \n
 *                        0: PHY EEE function of the addressed ports is disabled.
 *
 * @retval RT_ERR_OK            Successfully get enable or disable PHY EEE configuration of the addressed port.
 * @retval -RT_ERR_PORT_ID      The input port index is out of range.
 * @retval -RT_ERR_NULL_POINTER The input pEnPhyEee is NULL.
 * @retval Others               Please refer to dva_reg_field_read().
 * @note   Only port1 to port6 need to configure the corresponding PHY EEE functions.
 */
RtkApiRet dva_drv_eee_get_phy_en(uint32 unit, RtkPort port, RtkEnable *pEnPhyEee)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regOffset = 0u;
    uint32    regVal    = 0u;

    do
    {
        /* check input port index. */
        if((DVA_OP_T1_USER_PORT_MIN > port) || (DVA_OP_T1_USER_PORT_MAX < port))
        {
            *pEnPhyEee = DISABLE;
            retVal     = -RT_ERR_PORT_ID;
            continue;
        }
        /* check NULL pointer parameters */
        if(NULL == pEnPhyEee)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* get the PHY OCP address of the addressed port index */
        regOffset = DVA_PAGE_SWR_OCP | ((dva_g_sysPortToOcpAddr[dva_g_userPort2sysPort[port]] | DVA_OCP_OAM_CAP_GIGAPHY) << 1u);
        /* get PHY oam enable(bit[1]) and PHY EEE function enable(bit[0]) */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regOffset,
                                                    DVA_OCP_RG_EEE_EN_LSP,
                                                    (DVA_OCP_RG_EEE_EN_LEN + DVA_OCP_RG_OAM_EN_LEN),
                                                    &regVal));
        /* check PHY OAM and PHY EEE function are enabled */
        if(0x3u != regVal)
        {
            *pEnPhyEee = DISABLE;
            continue;
        }

        regOffset = DVA_PAGE_SWR_OCP | ((dva_g_sysPortToOcpAddr[dva_g_userPort2sysPort[port]] | DVA_OCP_PHYCORE_D15C) << 1u);
        /* get PHY AFE MCK EEE disable config */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regOffset,
                                                    DVA_OCP_RG_AFE_MCK_EEE_DISABLE_LSP,
                                                    DVA_OCP_RG_AFE_MCK_EEE_DISABLE_LEN,
                                                    &regVal));
        /* check PHY AFE MCK EEE disable config is set or not */
        if(1u != regVal)
        {
            *pEnPhyEee = DISABLE;
            continue;
        }

        /* return ENABLE only if all above configurations value are ENABLE */
        *pEnPhyEee = ENABLE;
    } while(1u == 0u);

    return retVal;
}
