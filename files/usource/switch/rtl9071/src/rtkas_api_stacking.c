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
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_stacking.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_stacking.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_stacking.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_stacking.h"
#endif

#include "rtkas_api_stacking.h"

/**
 * @defgroup RTK_MODULE_STACKING Stacking
 * @brief Stacking APIs \n
 * Stacking API can construct stacking topology with dynamic mode and setup stacking environment under static mode. \n
 * @{
 */
/**
 * @defgroup STK_DYNAMIC Stacking Dynamic Configure
 * @brief The APIs are used for dynamically setting up the stacking topology and checking the status of the stacking situation.
 *
 * @{
 */
/**
 * @brief Enable stacking and set port for stacking. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The stacking enable was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_enable(), dva_stacking_enable(), cvb_stacking_enable(), cva_stacking_enable().
 */
RtkApiRet rtk_stacking_enable(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_stacking_enable(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_stacking_enable(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_stacking_enable(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_stacking_enable(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Disable stacking. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The stacking disable was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_disable(), dva_stacking_disable(), cvb_stacking_disable(), cva_stacking_disable().
 */
RtkApiRet rtk_stacking_disable(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_stacking_disable(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_stacking_disable(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_stacking_disable(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_stacking_disable(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief To search the stackable switch which is connected on port S0 and S1 with this switch. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stackingDevsearch stacking devsearch information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The device search process was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_devsearch(), dva_stacking_devsearch(), cvb_stacking_devsearch(), cva_stacking_devsearch().
 */
RtkApiRet rtk_stacking_devsearch(UnitChip_t unitChip, StackingDevsearch_t *stackingDevsearch)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingDevsearch)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_devsearch(unitChip.unit, &stackingDevsearch->numOnS0, &stackingDevsearch->numOnS1);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_devsearch(unitChip.unit, &stackingDevsearch->numOnS0, &stackingDevsearch->numOnS1);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_devsearch(unitChip.unit, &stackingDevsearch->numOnS0, &stackingDevsearch->numOnS1);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_devsearch(unitChip.unit, &stackingDevsearch->numOnS0, &stackingDevsearch->numOnS1);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief After device search, select the number on each stacking port to auto construct stack configuration. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stackingAutoConfig stacking auto config information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The auto config process was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_auto_config(), dva_stacking_auto_config(), cvb_stacking_auto_config(), cva_stacking_auto_config().
 */
RtkApiRet rtk_stacking_auto_config(UnitChip_t unitChip, StackingAutoConfig_t *stackingAutoConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingAutoConfig)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_auto_config(unitChip.unit, stackingAutoConfig->selOnS0, stackingAutoConfig->selOnS1);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_auto_config(unitChip.unit, stackingAutoConfig->selOnS0, stackingAutoConfig->selOnS1);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_auto_config(unitChip.unit, stackingAutoConfig->selOnS0, stackingAutoConfig->selOnS1);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_auto_config(unitChip.unit, stackingAutoConfig->selOnS0, stackingAutoConfig->selOnS1);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Dump the current stacking status of this switch. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stackingGetStatus stacking get status information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The status get was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_get_status(), dva_stacking_get_status(), cvb_stacking_get_status(), cva_stacking_get_status().
 */
RtkApiRet rtk_stacking_get_status(UnitChip_t unitChip, StackingGetStatus_t *stackingGetStatus)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingGetStatus)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_get_status(unitChip.unit, &stackingGetStatus->status);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_get_status(unitChip.unit, &stackingGetStatus->status);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_get_status(unitChip.unit, &stackingGetStatus->status);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_get_status(unitChip.unit, &stackingGetStatus->status);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Dump the current stacking master unit Id. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stackingGetMasterId stacking get master id information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The master id get was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_get_master_id(), dva_stacking_get_master_id(), cvb_stacking_get_master_id(), cva_stacking_get_master_id().
 */
RtkApiRet rtk_stacking_get_master_id(UnitChip_t unitChip, StackingGetMasterId_t *stackingGetMasterId)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingGetMasterId)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_get_master_id(unitChip.unit, &stackingGetMasterId->masterId);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_get_master_id(unitChip.unit, &stackingGetMasterId->masterId);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_get_master_id(unitChip.unit, &stackingGetMasterId->masterId);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_get_master_id(unitChip.unit, &stackingGetMasterId->masterId);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Dump the current stacking group Id. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stackingGetGroupId stacking get group id information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The group id get was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_get_group_id(), dva_stacking_get_group_id(), cvb_stacking_get_group_id(), cva_stacking_get_group_id().
 */
RtkApiRet rtk_stacking_get_group_id(UnitChip_t unitChip, StackingGetGroupId_t *stackingGetGroupId)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingGetGroupId)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_get_group_id(unitChip.unit, &stackingGetGroupId->groupId);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_get_group_id(unitChip.unit, &stackingGetGroupId->groupId);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_get_group_id(unitChip.unit, &stackingGetGroupId->groupId);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_get_group_id(unitChip.unit, &stackingGetGroupId->groupId);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Dump the current stacking unitmap. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stackingGetUnitmap stacking get unitmap information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The unit map update was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_get_unitmap(), dva_stacking_get_unitmap(), cvb_stacking_get_unitmap(), cva_stacking_get_unitmap().
 */
RtkApiRet rtk_stacking_get_unitmap(UnitChip_t unitChip, StackingGetUnitmap_t *stackingGetUnitmap)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingGetUnitmap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_get_unitmap(unitChip.unit, stackingGetUnitmap->unitID, stackingGetUnitmap->atPort);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_get_unitmap(unitChip.unit, stackingGetUnitmap->unitID, stackingGetUnitmap->atPort);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_get_unitmap(unitChip.unit, stackingGetUnitmap->unitID, stackingGetUnitmap->atPort);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_get_unitmap(unitChip.unit, stackingGetUnitmap->unitID, stackingGetUnitmap->atPort);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief For master, check current stacking link status to all slaves. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stackingCheckConnection stacking check connection information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The connection check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_check_connection(), dva_stacking_check_connection(), cvb_stacking_check_connection(), cva_stacking_check_connection().
 */
RtkApiRet rtk_stacking_check_connection(UnitChip_t unitChip, StackingCheckConnection_t *stackingCheckConnection)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingCheckConnection)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_check_connection(unitChip.unit, stackingCheckConnection->unitID, stackingCheckConnection->linkStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_check_connection(unitChip.unit, stackingCheckConnection->unitID, stackingCheckConnection->linkStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_check_connection(unitChip.unit, stackingCheckConnection->unitID, stackingCheckConnection->linkStatus);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_check_connection(unitChip.unit, stackingCheckConnection->unitID, stackingCheckConnection->linkStatus);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* STK_DYNAMIC */

/**
 * @defgroup STK_STATIC Stacking Static Bootup
 * @brief The APIs are used for configuring otp of stacking boot.
 * @{
 */
/**
 * @brief Set OTP for stacking boot \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stackingBootOtpSet stacking boot otp set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The OTP set was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_boot_otp_set(), dva_stacking_boot_otp_set(), cvb_stacking_boot_otp_set(), cva_stacking_boot_otp_set().
 */
RtkApiRet rtk_stacking_boot_otp_set(UnitChip_t unitChip, StackingBootOtpSet_t *stackingBootOtpSet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingBootOtpSet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_boot_otp_set(unitChip.unit, stackingBootOtpSet->role, stackingBootOtpSet->portS0Enable, stackingBootOtpSet->portS1Enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_boot_otp_set(unitChip.unit, stackingBootOtpSet->role, stackingBootOtpSet->portS0Enable, stackingBootOtpSet->portS1Enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_boot_otp_set(unitChip.unit, stackingBootOtpSet->role, stackingBootOtpSet->portS0Enable, stackingBootOtpSet->portS1Enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_boot_otp_set(unitChip.unit, stackingBootOtpSet->role, stackingBootOtpSet->portS0Enable, stackingBootOtpSet->portS1Enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get OTP setting for stacking boot. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] stackingBootOtpGet stacking boot otp get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The OTP get was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_boot_otp_get(), dva_stacking_boot_otp_get(), cvb_stacking_boot_otp_get(), cva_stacking_boot_otp_get().
 */
RtkApiRet rtk_stacking_boot_otp_get(UnitChip_t unitChip, StackingBootOtpGet_t *stackingBootOtpGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingBootOtpGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_boot_otp_get(unitChip.unit, &stackingBootOtpGet->role, &stackingBootOtpGet->portS0Enable, &stackingBootOtpGet->portS1Enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_boot_otp_get(unitChip.unit, &stackingBootOtpGet->role, &stackingBootOtpGet->portS0Enable, &stackingBootOtpGet->portS1Enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_boot_otp_get(unitChip.unit, &stackingBootOtpGet->role, &stackingBootOtpGet->portS0Enable, &stackingBootOtpGet->portS1Enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_boot_otp_get(unitChip.unit, &stackingBootOtpGet->role, &stackingBootOtpGet->portS0Enable, &stackingBootOtpGet->portS1Enable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* STK_STATIC */

/**
 * @defgroup STK_FLASHLESS Stacking Flashless
 * @brief The APIs are used for constructing stacking topology under flashless mode.
 * @{
 */
/**
 * @brief Flashless update flow of stacking slave. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  stackingFlashlessUpdateSlave stacking flashless update slave information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The stacking flashless update was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_stacking_flashless_update_slave(), dva_stacking_flashless_update_slave(), cvb_stacking_flashless_update_slave(), cva_stacking_flashless_update_slave().
 */
RtkApiRet rtk_stacking_flashless_update_slave(UnitChip_t unitChip, StackingFlashlessUpdateSlave_t *stackingFlashlessUpdateSlave)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == stackingFlashlessUpdateSlave)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_stacking_flashless_update_slave(stackingFlashlessUpdateSlave->slaveUnitDV1, stackingFlashlessUpdateSlave->fwFileName, stackingFlashlessUpdateSlave->cfgFileName);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_stacking_flashless_update_slave(stackingFlashlessUpdateSlave->slaveUnitDVa, stackingFlashlessUpdateSlave->fwFileName, stackingFlashlessUpdateSlave->cfgFileName);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_stacking_flashless_update_slave(stackingFlashlessUpdateSlave->slaveUnitCVb, stackingFlashlessUpdateSlave->fwFileName, stackingFlashlessUpdateSlave->cfgFileName);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_stacking_flashless_update_slave(stackingFlashlessUpdateSlave->slaveUnitCVa, stackingFlashlessUpdateSlave->fwFileName, stackingFlashlessUpdateSlave->cfgFileName);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* STK_FLASHLESS */
/**@}*/ /* RTK_MODULE_STACKING */
