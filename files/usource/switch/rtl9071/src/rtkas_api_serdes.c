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
#include "rtkas_types.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_serdes.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_serdes.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_api_serdes.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_api_serdes.h"
#endif
#include "rtkas_api_serdes.h"
/**
 * @defgroup RTK_MODULE_SERDES SERDES
 * @brief SERDES APIs. The types of SERDES interface include USXGMII, HISGMII and SGMII etc. The hardware includes APHY & PCS etc.\n
 *        This component provides access to the Analog PHY (APHY) and Physical Coding Sublayer (PCS) registers of the SERDES interface.
 * @{
 */
/**
 * @defgroup SERDES_GET_SET_REGISTER Serdes indirect access
 * @brief APIs to access APHY or PCS register
 * @{
 */
/**
 * @brief A function used to get the value of the fields in the SERDES PCS register.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             The stacking unit ID is used to indicate which switch to access.
 *             The chip ID is used to indicate which chip model to access.
 * @param[in,out] sysSdsRegGetbitsPcs sys sds reg getbits pcs information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK API gets the value of the PCS register successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_sys_sds_reg_getbits_pcs(), dva_sys_sds_reg_getbits_pcs(), cvb_sys_sds_reg_getbits_pcs() and cva_sys_sds_reg_getbits_pcs().
 */
RtkApiRet rtk_sys_sds_reg_getbits_pcs(UnitChip_t unitChip, SysSdsPcsRegConfig_t *sysSdsRegGetbitsPcs)
{
    RtkApiRet ret = RT_ERR_OK;
    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_sds_reg_getbits_pcs(unitChip.unit, sysSdsRegGetbitsPcs->port, sysSdsRegGetbitsPcs->page, sysSdsRegGetbitsPcs->reg, sysSdsRegGetbitsPcs->startBit, sysSdsRegGetbitsPcs->endBit, &sysSdsRegGetbitsPcs->value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_sds_reg_getbits_pcs(unitChip.unit, sysSdsRegGetbitsPcs->port, sysSdsRegGetbitsPcs->page, sysSdsRegGetbitsPcs->reg, sysSdsRegGetbitsPcs->startBit, sysSdsRegGetbitsPcs->endBit, &sysSdsRegGetbitsPcs->value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_sds_reg_getbits_pcs(unitChip.unit, sysSdsRegGetbitsPcs->port, sysSdsRegGetbitsPcs->page, sysSdsRegGetbitsPcs->reg, sysSdsRegGetbitsPcs->startBit, sysSdsRegGetbitsPcs->endBit, &sysSdsRegGetbitsPcs->value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_sds_reg_getbits_pcs(unitChip.unit, sysSdsRegGetbitsPcs->port, sysSdsRegGetbitsPcs->page, sysSdsRegGetbitsPcs->reg, sysSdsRegGetbitsPcs->startBit, sysSdsRegGetbitsPcs->endBit, &sysSdsRegGetbitsPcs->value);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**
 * @brief A function used to get the value of the fields in the SERDES APHY register.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             The stacking unit ID is used to indicate which switch to access.
 *             The chip ID is used to indicate which chip model to access.
 * @param[in,out] sysSdsRegGetbitsAphy sys sds reg getbits aphy information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK API gets the value of APHY register successfully.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_sys_sds_reg_getbits_aphy(), dva_sys_sds_reg_getbits_aphy(), cvb_sys_sds_reg_getbits_aphy() and cva_sys_sds_reg_getbits_aphy().
 */
RtkApiRet rtk_sys_sds_reg_getbits_aphy(UnitChip_t unitChip, SysSdsAphyRegConfig_t *sysSdsRegGetbitsAphy)
{
    RtkApiRet ret = RT_ERR_OK;
    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_sds_reg_getbits_aphy(unitChip.unit, sysSdsRegGetbitsAphy->port, sysSdsRegGetbitsAphy->page, sysSdsRegGetbitsAphy->reg, sysSdsRegGetbitsAphy->startBit, sysSdsRegGetbitsAphy->endBit, &sysSdsRegGetbitsAphy->value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_sds_reg_getbits_aphy(unitChip.unit, sysSdsRegGetbitsAphy->port, sysSdsRegGetbitsAphy->page, sysSdsRegGetbitsAphy->reg, sysSdsRegGetbitsAphy->startBit, sysSdsRegGetbitsAphy->endBit, &sysSdsRegGetbitsAphy->value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_sds_reg_getbits_aphy(unitChip.unit, sysSdsRegGetbitsAphy->port, sysSdsRegGetbitsAphy->page, sysSdsRegGetbitsAphy->reg, sysSdsRegGetbitsAphy->startBit, sysSdsRegGetbitsAphy->endBit, &sysSdsRegGetbitsAphy->value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_sds_reg_getbits_aphy(unitChip.unit, sysSdsRegGetbitsAphy->port, sysSdsRegGetbitsAphy->page, sysSdsRegGetbitsAphy->reg, sysSdsRegGetbitsAphy->startBit, sysSdsRegGetbitsAphy->endBit, &sysSdsRegGetbitsAphy->value);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**
 * @brief A function used to set the value of the fields in the SERDES PCS register.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             The stacking unit ID is used to indicate which switch is to be accessed.
 *             The chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  sysSdsRegSetbitsPcs sys sds reg setbits pcs information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The API successfully sets the value of the PCS register.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_sys_sds_reg_setbits_pcs(), dva_sys_sds_reg_setbits_pcs(), cvb_sys_sds_reg_setbits_pcs(), and cva_sys_sds_reg_setbits_pcs().
 */
RtkApiRet rtk_sys_sds_reg_setbits_pcs(UnitChip_t unitChip, SysSdsPcsRegConfig_t *sysSdsRegSetbitsPcs)
{
    RtkApiRet ret = RT_ERR_OK;
    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_sds_reg_setbits_pcs(unitChip.unit, sysSdsRegSetbitsPcs->port, sysSdsRegSetbitsPcs->page, sysSdsRegSetbitsPcs->reg, sysSdsRegSetbitsPcs->startBit, sysSdsRegSetbitsPcs->endBit, sysSdsRegSetbitsPcs->value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_sds_reg_setbits_pcs(unitChip.unit, sysSdsRegSetbitsPcs->port, sysSdsRegSetbitsPcs->page, sysSdsRegSetbitsPcs->reg, sysSdsRegSetbitsPcs->startBit, sysSdsRegSetbitsPcs->endBit, sysSdsRegSetbitsPcs->value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_sds_reg_setbits_pcs(unitChip.unit, sysSdsRegSetbitsPcs->port, sysSdsRegSetbitsPcs->page, sysSdsRegSetbitsPcs->reg, sysSdsRegSetbitsPcs->startBit, sysSdsRegSetbitsPcs->endBit, sysSdsRegSetbitsPcs->value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_sds_reg_setbits_pcs(unitChip.unit, sysSdsRegSetbitsPcs->port, sysSdsRegSetbitsPcs->page, sysSdsRegSetbitsPcs->reg, sysSdsRegSetbitsPcs->startBit, sysSdsRegSetbitsPcs->endBit, sysSdsRegSetbitsPcs->value);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**
 * @brief A function used to set the value of the fields in the SERDES APHY register.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             The stacking unit ID is used to indicate which switch is to be accessed.
 *             The chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  sysSdsRegSetbitsAphy sys sds reg setbits aphy information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The API successfully sets the value of the APHY register.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_sys_sds_reg_setbits_aphy(), dva_sys_sds_reg_setbits_aphy(), cvb_sys_sds_reg_setbits_aphy(), and cva_sys_sds_reg_setbits_aphy().
 */
RtkApiRet rtk_sys_sds_reg_setbits_aphy(UnitChip_t unitChip, SysSdsAphyRegConfig_t *sysSdsRegSetbitsAphy)
{
    RtkApiRet ret = RT_ERR_OK;
    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_sys_sds_reg_setbits_aphy(unitChip.unit, sysSdsRegSetbitsAphy->port, sysSdsRegSetbitsAphy->page, sysSdsRegSetbitsAphy->reg, sysSdsRegSetbitsAphy->startBit, sysSdsRegSetbitsAphy->endBit, sysSdsRegSetbitsAphy->value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_sds_reg_setbits_aphy(unitChip.unit, sysSdsRegSetbitsAphy->port, sysSdsRegSetbitsAphy->page, sysSdsRegSetbitsAphy->reg, sysSdsRegSetbitsAphy->startBit, sysSdsRegSetbitsAphy->endBit, sysSdsRegSetbitsAphy->value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_sys_sds_reg_setbits_aphy(unitChip.unit, sysSdsRegSetbitsAphy->port, sysSdsRegSetbitsAphy->page, sysSdsRegSetbitsAphy->reg, sysSdsRegSetbitsAphy->startBit, sysSdsRegSetbitsAphy->endBit, sysSdsRegSetbitsAphy->value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_sys_sds_reg_setbits_aphy(unitChip.unit, sysSdsRegSetbitsAphy->port, sysSdsRegSetbitsAphy->page, sysSdsRegSetbitsAphy->reg, sysSdsRegSetbitsAphy->startBit, sysSdsRegSetbitsAphy->endBit, sysSdsRegSetbitsAphy->value);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Configure the SERDES settings for a specified port in flashless over ethernet mode. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. \n
 *             The stacking unit ID is used to indicate which switch is to be accessed.
 *             The chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  sysSdsConfigSet Configuration parameters for the SERDES settings. \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The API successfully configures the SERDES settings.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_sys_sds_config_set().
 */
RtkApiRet rtk_sys_sds_config_set(UnitChip_t unitChip, SysSdsConfigSet_t *sysSdsConfigSet)
{
    RtkApiRet ret = RT_ERR_OK;
    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sys_sds_config_set(unitChip.unit, sysSdsConfigSet->userPort, sysSdsConfigSet->serdesMode, sysSdsConfigSet->portRole, sysSdsConfigSet->isForceMode, sysSdsConfigSet->speed, sysSdsConfigSet->portSpeed);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Set the force link status for the SERDES interface on a specified port in flashless over ethernet mode. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID. \n
 *             The stacking unit ID is used to indicate which switch is to be accessed.
 *             The chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  sysSdsForceLinkSet Configuration parameters for forcing link status. \n
 *             Please refer to the struct field introductions in the .h files for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK The API successfully sets the force link status.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_sds_force_link_set().
 */
RtkApiRet rtk_sys_sds_force_link_set(UnitChip_t unitChip, SysSdsForceLinkSet_t *sysSdsForceLinkSet)
{
    RtkApiRet ret = RT_ERR_OK;
    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_sds_force_link_set(unitChip.unit, sysSdsForceLinkSet->userPort, sysSdsForceLinkSet->serdesMode, sysSdsForceLinkSet->linkStatus);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}
/**@}*/ /*SERDES_GET_SET_REGISTER*/
/**@}*/ /*RTK_MODULE_SERDES*/
