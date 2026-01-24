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
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_api_routing.h"
#include "rtkas_api_firewall.h"

/**
 * @defgroup RTK_MODULE_FIR Firewall
 * @brief Firewall APIs
 * @{
 */

/**
 * @defgroup RTK_MODULE_FIR_STATEFUL Stateful Firewall
 * @brief Stateful Firewall APIs
 * @{
 */

/**
 * @brief To turn ON/OFF stateful
 *          Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] *param   Set parameter (N/A) \n
 *                     To turn on/off stateful feature
 * @param[out] None
 * @retval RT_ERR_OK    Enable/Disable stateful successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 */
RtkApiRet rtk_fir_enable(UnitChip_t unitChip, const FirEnable_t *param)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            /* TBD */
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief To configure global setting of stateful
 *          Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] glbCfg   Stateful global configuration (N/A) \n
 *                     Configure global stateful setting
 * @retval RT_ERR_OK Set global configuration successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 */
RtkApiRet rtk_fir_glb_cfg_set(UnitChip_t unitChip, const FirGlbCfg_t *glbCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            /* TBD */
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Get current global configuration
 *          Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] glbCfg  The Pointer of stateful global configuration (N/A) \n
 *                     Configure global stateful setting
 * @retval RT_ERR_OK Set global configuration successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 */
RtkApiRet rtk_fir_glb_cfg_get(UnitChip_t unitChip, FirGlbCfg_t *glbCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            /* TBD */
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Set a rule allowed to pass the stateful firewall
 *          Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip  Stacking unit and chip id (N/A) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ruleEntry A rule entry (N/A) \n
 *                      A rule be allowed to pass the stateful firewall
 * @retval RT_ERR_OK Set rule successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 */
RtkApiRet rtk_fir_rule_tbl_set(UnitChip_t unitChip, const FirRuleTblEntry_t *ruleEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            /* TBD */
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**
 * @brief Get a rule entry
 *          Supported chips: RTL907XD_VA
 *
 * @param[in] unitChip  Stacking unit and chip id (N/A) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] *param Get parameter (N/A) \n
 *                      Get entry of rule table
 * @retval RT_ERR_OK Get rule successfully
 * @retval -RT_ERR_NOT_SUPPORT Not supported
 */
RtkApiRet rtk_fir_rule_tbl_get(UnitChip_t unitChip, FirRuleTblGet_t *param)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            /* TBD */
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}
/**@}*/ /* RTK_MODULE_FIR_STATEFUL */
/**@}*/ /* RTK_MODULE_FIR */